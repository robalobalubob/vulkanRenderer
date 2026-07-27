#include "vulkan-engine/scene/RdSceneLoader.hpp"

#include "vulkan-engine/components/Light.hpp"
#include "vulkan-engine/components/MeshRenderer.hpp"
#include "vulkan-engine/core/Logger.hpp"
#include "vulkan-engine/math/Transform.hpp"
#include "vulkan-engine/rendering/Vertex.hpp"
#include "vulkan-engine/resources/Material.hpp"
#include "vulkan-engine/resources/Mesh.hpp"
#include "vulkan-engine/resources/PrimitiveFactory.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace vkeng {

namespace {

// Matches MAX_LIGHTS in Uniforms.hpp / shader.frag (GlobalUbo light array size)
constexpr size_t kMaxLights = 8;

/// RD colors are authored in display space (the course renderer wrote them
/// straight to the framebuffer); decode to linear for the engine's pipeline.
glm::vec3 displayToLinear(const glm::vec3& c) {
    return {std::pow(std::max(c.r, 0.0f), 2.2f),
            std::pow(std::max(c.g, 0.0f), 2.2f),
            std::pow(std::max(c.b, 0.0f), 2.2f)};
}

[[noreturn]] void failAt(size_t line, const std::string& what) {
    throw std::runtime_error("RD parse error (line " + std::to_string(line) + "): " + what);
}

/// A single lexed token, retaining whether it was quoted so replayed object
/// bodies can tell a string argument from a bare one.
struct RdToken {
    std::string text;
    bool quoted = false;
};

/**
 * @class RdTokenSource
 * @brief Abstract token stream feeding the RD command dispatch.
 *
 * Two implementations exist: RdTokenizer reads the file text directly, and
 * RdTokenReplay walks a recorded object body substituting $n parameters. The
 * dispatch runs unchanged over either, which is what makes ObjectInstance work.
 */
class RdTokenSource {
public:
    virtual ~RdTokenSource() = default;

    virtual bool atEnd() = 0;
    virtual RdToken nextRaw() = 0;
    virtual size_t line() const = 0;

    std::string next() { return nextRaw().text; }

    std::string nextQuoted() {
        const RdToken token = nextRaw();
        if (!token.quoted) {
            failAt(line(), "expected quoted string, found \"" + token.text + "\"");
        }
        return token.text;
    }

    virtual float nextFloat() {
        const RdToken token = nextRaw();
        const char* start = token.text.c_str();
        char* end = nullptr;
        const float value = std::strtof(start, &end);
        if (end == start) failAt(line(), "expected number, found \"" + token.text + "\"");
        return value;
    }

    virtual int nextInt() {
        const RdToken token = nextRaw();
        const char* start = token.text.c_str();
        char* end = nullptr;
        const long value = std::strtol(start, &end, 10);
        if (end == start) failAt(line(), "expected integer, found \"" + token.text + "\"");
        return static_cast<int>(value);
    }

    glm::vec3 nextVec3() {
        const float x = nextFloat();
        const float y = nextFloat();
        const float z = nextFloat();
        return {x, y, z};
    }

    /// Reads a boolean in any of the RD spellings (true/yes/on/1 and negations).
    bool nextBool() {
        const std::string token = next();
        if (token == "true" || token == "yes" || token == "on" || token == "1") return true;
        if (token == "false" || token == "no" || token == "off" || token == "0") return false;
        failAt(line(), "expected boolean, found \"" + token + "\"");
    }
};

/**
 * @class RdTokenizer
 * @brief Whitespace/comment-aware token stream over an RD file's contents.
 *
 * Throws std::runtime_error with line information on malformed input; the
 * loader catches this and converts it to a Result Error.
 */
class RdTokenizer : public RdTokenSource {
public:
    explicit RdTokenizer(std::string content) : m_content(std::move(content)) {}

    bool atEnd() override {
        skipWhitespaceAndComments();
        return m_pos >= m_content.size();
    }

    RdToken nextRaw() override {
        skipWhitespaceAndComments();
        if (m_pos >= m_content.size()) {
            failAt(m_line, "unexpected end of file");
        }
        if (m_content[m_pos] == '"') {
            return {nextQuotedText(), true};
        }
        const size_t start = m_pos;
        while (m_pos < m_content.size() && !isSpace(m_content[m_pos])) {
            ++m_pos;
        }
        return {m_content.substr(start, m_pos - start), false};
    }

    // Numeric fast paths: parse straight out of the buffer rather than
    // materialising a std::string per value. Scene files run to hundreds of
    // thousands of floats (the terrain PolySet alone is ~50k lines), so this
    // is the difference between a snappy load and a visibly slow one.
    float nextFloat() override {
        skipWhitespaceAndComments();
        if (m_pos >= m_content.size()) failAt(m_line, "expected number, found end of file");
        const char* start = m_content.c_str() + m_pos;
        char* end = nullptr;
        const float value = std::strtof(start, &end);
        if (end == start) failAt(m_line, "expected number");
        m_pos += static_cast<size_t>(end - start);
        return value;
    }

    int nextInt() override {
        skipWhitespaceAndComments();
        if (m_pos >= m_content.size()) failAt(m_line, "expected integer, found end of file");
        const char* start = m_content.c_str() + m_pos;
        char* end = nullptr;
        const long value = std::strtol(start, &end, 10);
        if (end == start) failAt(m_line, "expected integer");
        m_pos += static_cast<size_t>(end - start);
        return static_cast<int>(value);
    }

    size_t line() const override { return m_line; }

private:
    static bool isSpace(char c) {
        return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v';
    }

    std::string nextQuotedText() {
        ++m_pos; // opening quote
        const size_t start = m_pos;
        while (m_pos < m_content.size() && m_content[m_pos] != '"') {
            if (m_content[m_pos] == '\n') ++m_line;
            ++m_pos;
        }
        if (m_pos >= m_content.size()) {
            failAt(m_line, "unterminated quoted string");
        }
        std::string result = m_content.substr(start, m_pos - start);
        ++m_pos; // closing quote
        return result;
    }

    void skipWhitespaceAndComments() {
        while (m_pos < m_content.size()) {
            const char c = m_content[m_pos];
            if (c == '\n') {
                ++m_line;
                ++m_pos;
            } else if (isSpace(c)) {
                ++m_pos;
            } else if (c == '#') {
                while (m_pos < m_content.size() && m_content[m_pos] != '\n') {
                    ++m_pos;
                }
            } else {
                break;
            }
        }
    }

    std::string m_content;
    size_t m_pos = 0;
    size_t m_line = 1;
};

/**
 * @class RdTokenReplay
 * @brief Replays a recorded object body, substituting $n parameter references.
 *
 * Object bodies are small, so this deliberately favours simplicity over the
 * numeric fast paths RdTokenizer uses.
 */
class RdTokenReplay : public RdTokenSource {
public:
    RdTokenReplay(const std::vector<RdToken>& body, const std::vector<RdToken>& params, size_t line)
        : m_body(body), m_params(params), m_line(line) {}

    bool atEnd() override { return m_pos >= m_body.size(); }

    RdToken nextRaw() override {
        if (m_pos >= m_body.size()) {
            failAt(m_line, "unexpected end of object body");
        }
        const RdToken& token = m_body[m_pos++];

        // $1, $2, ... refer to the arguments supplied at the instance site.
        // Only unquoted tokens are substituted: a literal "$1" inside quotes is
        // a string, not a reference.
        if (!token.quoted && token.text.size() >= 2 && token.text[0] == '$') {
            const std::string digits = token.text.substr(1);
            if (digits.find_first_not_of("0123456789") == std::string::npos) {
                const size_t index = static_cast<size_t>(std::stoul(digits));
                if (index == 0 || index > m_params.size()) {
                    failAt(m_line, "object parameter " + token.text + " is out of range (" +
                                       std::to_string(m_params.size()) + " supplied)");
                }
                return m_params[index - 1];
            }
        }
        return token;
    }

    size_t line() const override { return m_line; }

private:
    const std::vector<RdToken>& m_body;
    const std::vector<RdToken>& m_params;
    size_t m_pos = 0;
    size_t m_line;
};

/// Snapshot of the RD drawing-attribute state at the time a primitive is emitted.
struct RdSurfaceState {
    enum class Shader { Matte, Plastic, Metal };

    glm::vec3 color{1.0f, 1.0f, 1.0f};      ///< Current drawing color (display space)
    float opacity = 1.0f;
    Shader shader = Shader::Matte;           ///< RD default surface is "matte"
    float ka = 1.0f;
    float kd = 1.0f;
    float ks = 0.0f;
    glm::vec3 specularColor{1.0f, 1.0f, 1.0f};
    float specularExponent = 10.0f;
};

struct RdPrimitiveRecord {
    glm::mat4 xform{1.0f};
    std::shared_ptr<Mesh> mesh;
    RdSurfaceState surface;
    glm::vec3 emissiveBoost{0.0f};  ///< Extra emission for primitives hosting a light source
    bool doubleSided = false;
    bool wireframe = false;
    bool unlit = false;             ///< Lines and points: colour comes straight from the drawing colour
    glm::vec3 unlitColor{1.0f};     ///< Linear; white when the vertices already carry colours
    std::string name;
};

struct RdLightRecord {
    LightType type = LightType::Point;
    glm::vec3 positionOrDirection{0.0f};
    glm::vec3 color{1.0f};   ///< Linear
    float intensity = 1.0f;
    glm::vec3 spotTarget{0.0f};
    float innerHalfAngle = 0.0f;  ///< Radians (spot only)
    float outerHalfAngle = 0.0f;  ///< Radians (spot only)
};

/// Vertex attribute layout described by an RD vertex type string like "PC".
struct RdVertexType {
    bool hasNormal = false;
    bool hasColor = false;
    bool hasTexCoord = false;
    int floatsPerVertex = 0;
};

RdVertexType parseVertexType(const std::string& type, size_t line) {
    RdVertexType result;
    bool hasPosition = false;
    for (char c : type) {
        switch (c) {
            case 'P': hasPosition = true; result.floatsPerVertex += 3; break;
            case 'C': result.hasColor = true; result.floatsPerVertex += 3; break;
            case 'N':
            case 'D': result.hasNormal = true; result.floatsPerVertex += 3; break;
            case 'T': result.hasTexCoord = true; result.floatsPerVertex += 2; break;
            case 'w': result.floatsPerVertex += 1; break;  // weight: parsed, unused
            case 'O': result.floatsPerVertex += 1; break;  // opacity: parsed, unused
            default:
                failAt(line, "unknown vertex attribute '" + std::string(1, c) + "'");
        }
    }
    if (!hasPosition) {
        failAt(line, "vertex type \"" + type + "\" lacks position attribute P");
    }
    return result;
}

/// A definition recorded by ObjectBegin, replayed by ObjectInstance.
struct RdObjectDefinition {
    int parameterCount = 0;
    std::vector<RdToken> body;
};

/// Conservative world-space scale factor of a matrix (longest basis vector).
float maxScaleFactor(const glm::mat4& m) {
    const float sx = glm::length(glm::vec3(m[0]));
    const float sy = glm::length(glm::vec3(m[1]));
    const float sz = glm::length(glm::vec3(m[2]));
    return std::max(sx, std::max(sy, sz));
}

std::string materialKey(const glm::vec4& baseColor, const glm::vec3& specular,
                        const glm::vec3& emissive, float shininess,
                        bool doubleSided, bool wireframe, bool unlit) {
    char buffer[200];
    std::snprintf(buffer, sizeof(buffer),
                  "%.4f,%.4f,%.4f,%.4f|%.4f,%.4f,%.4f|%.4f,%.4f,%.4f|%.2f|%d%d%d",
                  baseColor.r, baseColor.g, baseColor.b, baseColor.a,
                  specular.r, specular.g, specular.b,
                  emissive.r, emissive.g, emissive.b, shininess,
                  doubleSided ? 1 : 0, wireframe ? 1 : 0, unlit ? 1 : 0);
    return buffer;
}

/**
 * @class RdParser
 * @brief Command dispatch and scene construction for one RD file.
 *
 * State is split into three lifetimes, matching the RD spec:
 *  - file-scoped: display settings, object definitions, the mesh cache
 *  - persistent across frames: camera, background, surface attributes, options
 *    ("Attribute values hold between frames of an animation as well")
 *  - per-frame: primitives, lights and the transform stack ("All lighting
 *    information is lost at the end of a frame block or world block")
 */
class RdParser {
public:
    RdParser(std::shared_ptr<MemoryManager> memoryManager, RdSceneFile& out)
        : m_memoryManager(std::move(memoryManager)), m_out(out) {}

    void run(RdTokenSource& src);

    /// Emits a final frame at end of file, even for a file that drew nothing.
    void flushAtEof() {
        if (hasPendingContent() || m_out.frames.empty()) {
            flushFrame();
        }
    }

private:
    bool hasPendingContent() const { return !m_primitives.empty() || !m_lights.empty(); }

    /// Emits a frame only when geometry or lights have actually accumulated,
    /// so a well-formed FrameBegin/WorldBegin pair does not produce an empty one.
    void flushPendingContent() {
        if (hasPendingContent()) flushFrame();
    }

    // ---- Diagnostics -------------------------------------------------------

    void warn(const std::string& message) {
        if (m_warned.insert(message).second) {
            m_out.warnings.push_back(message);
            LOG_WARN(GENERAL, "RdSceneLoader: {}", message);
        }
    }

    // ---- Tessellation ------------------------------------------------------
    //
    // Honour the file's Divisions but never drop below a quality baseline; the
    // modern renderer can afford far more than the course rasterizer could.
    uint32_t latSegments() const { return static_cast<uint32_t>(std::max(m_divisions, 32)); }
    uint32_t lonSegments() const { return static_cast<uint32_t>(std::max(m_divisions * 2, 48)); }

    // ---- Emission ----------------------------------------------------------

    void emitPrimitive(std::shared_ptr<Mesh> mesh, const glm::mat4& extraXform,
                       const char* kind, bool doubleSided = false) {
        if (!mesh) return;
        RdPrimitiveRecord record;
        record.xform = m_xformStack.back() * extraXform;
        record.mesh = std::move(mesh);
        record.surface = m_surface;
        record.doubleSided = doubleSided;
        record.wireframe = m_wireframe;
        record.name = std::string("rd_") + kind + "_" + std::to_string(m_primitiveCount++);
        m_primitives.push_back(std::move(record));
    }

    /**
     * @brief Emits a line or point mesh, which renders unlit.
     *
     * @p verticesCarryColor selects where the colour comes from: per-vertex when
     * the RD vertex type included "C", otherwise the current drawing colour.
     * Exactly one of the two is non-white so the shader's fragColor * emissive
     * product lands on the authored colour either way.
     */
    void emitUnlit(std::shared_ptr<Mesh> mesh, const char* kind, bool verticesCarryColor) {
        if (!mesh) return;
        RdPrimitiveRecord record;
        record.xform = m_xformStack.back();
        record.mesh = std::move(mesh);
        record.surface = m_surface;
        record.unlit = true;
        record.unlitColor = verticesCarryColor ? glm::vec3(1.0f) : displayToLinear(m_surface.color);
        record.name = std::string("rd_") + kind + "_" + std::to_string(m_primitiveCount++);
        m_primitives.push_back(std::move(record));
    }

    /// Reads @p count vertices in the given layout, converting to engine Vertices.
    std::vector<Vertex> readVertices(RdTokenSource& src, const RdVertexType& type, int count) {
        std::vector<Vertex> vertices(static_cast<size_t>(count));
        for (auto& vertex : vertices) {
            vertex.pos = src.nextVec3();
            if (type.hasColor) vertex.color = displayToLinear(src.nextVec3());
            if (type.hasNormal) vertex.normal = src.nextVec3();
            if (type.hasTexCoord) {
                vertex.texCoord.x = src.nextFloat();
                vertex.texCoord.y = src.nextFloat();
            }
            // Consume any remaining attributes (w, O) to stay aligned
            int consumed = 3 + (type.hasColor ? 3 : 0) +
                           (type.hasNormal ? 3 : 0) + (type.hasTexCoord ? 2 : 0);
            for (; consumed < type.floatsPerVertex; ++consumed) src.nextFloat();
        }
        return vertices;
    }

    /// Reads -1 terminated index runs until @p count runs have been read.
    std::vector<std::vector<uint32_t>> readIndexRuns(RdTokenSource& src, int count, int vertexCount) {
        std::vector<std::vector<uint32_t>> runs;
        runs.reserve(static_cast<size_t>(count));
        for (int i = 0; i < count; ++i) {
            std::vector<uint32_t> run;
            for (int idx = src.nextInt(); idx != -1; idx = src.nextInt()) {
                if (idx < 0 || idx >= vertexCount) {
                    failAt(src.line(), "index " + std::to_string(idx) + " out of range");
                }
                run.push_back(static_cast<uint32_t>(idx));
            }
            runs.push_back(std::move(run));
        }
        return runs;
    }

    /// Builds a triangle mesh from indexed convex polygons, fanning each face.
    std::shared_ptr<Mesh> buildPolygonMesh(const std::string& name,
                                           std::vector<Vertex> vertices,
                                           const std::vector<std::vector<uint32_t>>& faces,
                                           bool hasAuthoredNormals);

    void handleGeometry(RdTokenSource& src, const std::string& cmd);
    bool handleAttributeOrLight(RdTokenSource& src, const std::string& cmd);
    void recordObject(RdTokenSource& src);
    void instanceObject(RdTokenSource& src);
    void flushFrame();

    // ---- File-scoped state -------------------------------------------------

    std::shared_ptr<MemoryManager> m_memoryManager;
    RdSceneFile& m_out;
    std::set<std::string> m_warned;
    std::map<std::string, std::shared_ptr<Mesh>> m_meshCache;
    std::unordered_map<std::string, RdObjectDefinition> m_objects;
    int m_primitiveCount = 0;
    int m_polySetCount = 0;
    int m_instanceDepth = 0;

    // ---- State persisting across frames ------------------------------------

    RdCamera m_camera;
    glm::vec3 m_background{0.0f};
    RdSurfaceState m_surface;
    int m_divisions = 20;         // rd_view default tessellation
    bool m_wireframe = false;

    // ---- Per-frame state ---------------------------------------------------

    std::vector<glm::mat4> m_xformStack{glm::mat4(1.0f)};
    std::vector<RdPrimitiveRecord> m_primitives;
    std::vector<RdLightRecord> m_lights;
    glm::vec3 m_ambient{1.0f};    // RD default: one white ambient light at intensity 1.0
    int m_frameNumber = 1;
};

std::shared_ptr<Mesh> RdParser::buildPolygonMesh(const std::string& name,
                                                 std::vector<Vertex> vertices,
                                                 const std::vector<std::vector<uint32_t>>& faces,
                                                 bool hasAuthoredNormals) {
    std::vector<uint32_t> indices;
    for (const auto& face : faces) {
        // Triangulate by fan (faces are convex polygons in CCW order)
        for (size_t i = 2; i < face.size(); ++i) {
            indices.push_back(face[0]);
            indices.push_back(face[i - 1]);
            indices.push_back(face[i]);
        }
    }
    if (indices.empty()) return nullptr;

    MeshNormalSource normalSource = MeshNormalSource::Authored;
    if (!hasAuthoredNormals) {
        // Area-weighted smooth normals from the triangle topology
        for (auto& vertex : vertices) vertex.normal = glm::vec3(0.0f);
        for (size_t i = 0; i + 2 < indices.size(); i += 3) {
            const glm::vec3& a = vertices[indices[i]].pos;
            const glm::vec3& b = vertices[indices[i + 1]].pos;
            const glm::vec3& c = vertices[indices[i + 2]].pos;
            const glm::vec3 faceNormal = glm::cross(b - a, c - a);
            vertices[indices[i]].normal += faceNormal;
            vertices[indices[i + 1]].normal += faceNormal;
            vertices[indices[i + 2]].normal += faceNormal;
        }
        for (auto& vertex : vertices) {
            const float len = glm::length(vertex.normal);
            vertex.normal = (len > 1e-8f) ? vertex.normal / len : glm::vec3(0.0f, 0.0f, 1.0f);
        }
        normalSource = MeshNormalSource::GeneratedSmooth;
    }

    Vertex::computeTangents(vertices, indices);
    return std::make_shared<Mesh>(name, m_memoryManager, vertices, indices, normalSource);
}

// ---------------------------------------------------------------------------
// Object blocks
// ---------------------------------------------------------------------------

void RdParser::recordObject(RdTokenSource& src) {
    // ObjectBegin [n_params] "name" — the parameter count is optional, so the
    // first argument is an integer when present and the name when not.
    int parameterCount = 0;
    RdToken first = src.nextRaw();
    std::string name;
    if (first.quoted) {
        name = first.text;
    } else {
        const char* start = first.text.c_str();
        char* end = nullptr;
        const long value = std::strtol(start, &end, 10);
        if (end == start) {
            failAt(src.line(), "ObjectBegin expects a parameter count or a quoted name");
        }
        parameterCount = static_cast<int>(value);
        name = src.nextQuoted();
    }

    RdObjectDefinition definition;
    definition.parameterCount = parameterCount;

    // Commands inside the block are not executed now; they are replayed at each
    // ObjectInstance. Blocks cannot nest, so scanning to the first ObjectEnd is
    // unambiguous.
    while (true) {
        if (src.atEnd()) failAt(src.line(), "ObjectBegin \"" + name + "\" has no matching ObjectEnd");
        RdToken token = src.nextRaw();
        if (!token.quoted && token.text == "ObjectEnd") break;
        definition.body.push_back(std::move(token));
    }

    m_objects[name] = std::move(definition);
}

void RdParser::instanceObject(RdTokenSource& src) {
    const std::string name = src.nextQuoted();
    auto it = m_objects.find(name);
    if (it == m_objects.end()) {
        warn("ObjectInstance \"" + name + "\" refers to an undefined object, skipped");
        return;
    }

    const RdObjectDefinition& definition = it->second;
    std::vector<RdToken> parameters;
    parameters.reserve(static_cast<size_t>(definition.parameterCount));
    for (int i = 0; i < definition.parameterCount; ++i) {
        parameters.push_back(src.nextRaw());
    }

    // Objects are not recursive, but a definition may instance *other* objects.
    // Cap the nesting so a file that violates that rule fails loudly instead of
    // recursing until the stack runs out.
    if (m_instanceDepth >= 32) {
        warn("ObjectInstance \"" + name + "\" nested more than 32 deep, skipped (recursive definition?)");
        return;
    }

    // "The current transformation is implicitly pushed upon the stack before
    // every ObjectInstance command" and popped afterwards. Surface attributes
    // deliberately are *not* restored: the spec keeps them in effect on return.
    m_xformStack.push_back(m_xformStack.back());
    ++m_instanceDepth;

    RdTokenReplay replay(definition.body, parameters, src.line());
    run(replay);

    --m_instanceDepth;
    m_xformStack.pop_back();
}

// ---------------------------------------------------------------------------
// Attributes, lights and options
// ---------------------------------------------------------------------------

bool RdParser::handleAttributeOrLight(RdTokenSource& src, const std::string& cmd) {
    // ---- Display / options ----
    if (cmd == "Display") {
        m_out.title = src.nextQuoted();
        src.nextQuoted();  // type ("Screen", "PNM")
        src.nextQuoted();  // mode
    } else if (cmd == "Format") {
        m_out.displayWidth = static_cast<uint32_t>(std::max(src.nextInt(), 1));
        m_out.displayHeight = static_cast<uint32_t>(std::max(src.nextInt(), 1));
    } else if (cmd == "Background") {
        m_background = displayToLinear(src.nextVec3());
    } else if (cmd == "OptionReal") {
        const std::string name = src.nextQuoted();
        const float value = src.nextFloat();
        if (name == "Divisions" || name == "Sides") {
            m_divisions = std::max(static_cast<int>(value), 3);
        } else {
            warn("OptionReal \"" + name + "\" ignored");
        }
    } else if (cmd == "OptionBool") {
        const std::string name = src.nextQuoted();
        const bool value = src.nextBool();
        if (name == "Wireframe") {
            m_wireframe = value;
        } else if (name == "Interpolate") {
            // The engine always interpolates vertex normals and colours, which
            // is this option's default; turning it off is not reproducible.
            if (!value) warn("OptionBool \"Interpolate\" false ignored (shading is always interpolated)");
        } else {
            warn("OptionBool \"" + name + "\" ignored");
        }
    } else if (cmd == "OptionString") {
        const std::string name = src.nextQuoted();
        src.next();  // value
        warn("OptionString \"" + name + "\" ignored");
    } else if (cmd == "OptionArray") {
        const std::string name = src.nextQuoted();
        const int count = src.nextInt();
        for (int i = 0; i < count; ++i) src.nextFloat();
        warn("OptionArray \"" + name + "\" ignored");
    } else if (cmd == "OptionList") {
        const std::string name = src.nextQuoted();
        const int count = src.nextInt();
        for (int i = 0; i < count; ++i) src.nextQuoted();
        warn("OptionList \"" + name + "\" ignored");
    }

    // ---- Camera ----
    else if (cmd == "CameraEye") {
        m_camera.eye = src.nextVec3();
    } else if (cmd == "CameraAt") {
        m_camera.at = src.nextVec3();
    } else if (cmd == "CameraUp") {
        m_camera.up = src.nextVec3();
    } else if (cmd == "CameraFOV") {
        m_camera.fovDegrees = src.nextFloat();
    } else if (cmd == "Clipping") {
        m_camera.nearPlane = src.nextFloat();
        m_camera.farPlane = src.nextFloat();
    }

    // ---- Transform stack ----
    else if (cmd == "XformPush") {
        m_xformStack.push_back(m_xformStack.back());
    } else if (cmd == "XformPop") {
        if (m_xformStack.size() > 1) {
            m_xformStack.pop_back();
        } else {
            warn("XformPop with empty transform stack");
        }
    } else if (cmd == "Translate") {
        m_xformStack.back() = glm::translate(m_xformStack.back(), src.nextVec3());
    } else if (cmd == "Scale") {
        m_xformStack.back() = glm::scale(m_xformStack.back(), src.nextVec3());
    } else if (cmd == "Rotate") {
        const std::string axis = src.nextQuoted();
        const float angle = glm::radians(src.nextFloat());
        glm::vec3 axisVec{0.0f};
        if (axis == "X") axisVec = {1.0f, 0.0f, 0.0f};
        else if (axis == "Y") axisVec = {0.0f, 1.0f, 0.0f};
        else if (axis == "Z") axisVec = {0.0f, 0.0f, 1.0f};
        else warn("Rotate around unknown axis \"" + axis + "\" ignored");
        if (axisVec != glm::vec3(0.0f)) {
            m_xformStack.back() = glm::rotate(m_xformStack.back(), angle, axisVec);
        }
    }

    // ---- Drawing attributes ----
    else if (cmd == "Color") {
        m_surface.color = src.nextVec3();
    } else if (cmd == "Opacity") {
        m_surface.opacity = std::clamp(src.nextFloat(), 0.0f, 1.0f);
    } else if (cmd == "Surface") {
        const std::string shader = src.nextQuoted();
        if (shader == "matte") m_surface.shader = RdSurfaceState::Shader::Matte;
        else if (shader == "metal") m_surface.shader = RdSurfaceState::Shader::Metal;
        else if (shader == "plastic") m_surface.shader = RdSurfaceState::Shader::Plastic;
        else if (shader == "painted plastic") {
            m_surface.shader = RdSurfaceState::Shader::Plastic;
            warn("Surface \"painted plastic\" treated as plastic (texture maps are not loaded)");
        } else {
            warn("Surface shader \"" + shader + "\" unknown, using plastic");
            m_surface.shader = RdSurfaceState::Shader::Plastic;
        }
    } else if (cmd == "Ka") {
        m_surface.ka = src.nextFloat();
    } else if (cmd == "Kd") {
        m_surface.kd = src.nextFloat();
    } else if (cmd == "Ks") {
        m_surface.ks = src.nextFloat();
    } else if (cmd == "Specular") {
        m_surface.specularColor = src.nextVec3();
        m_surface.specularExponent = std::max(src.nextFloat(), 1.0f);
    }

    // ---- Lights ----
    else if (cmd == "AmbientLight") {
        const glm::vec3 color = src.nextVec3();
        const float intensity = src.nextFloat();
        // RD spec: only one ambient light exists; later commands replace it
        m_ambient = displayToLinear(color) * intensity;
    } else if (cmd == "FarLight") {
        RdLightRecord light;
        light.type = LightType::Directional;
        // The spec says the vector is "the direction the light shines", but
        // rd_view (and scenes authored against it, e.g. "FarLight 0 0 1  # main
        // light from above" in a Z-up scene) treat it as pointing toward the
        // light, OpenGL-style. Follow rd_view.
        light.positionOrDirection = -src.nextVec3();
        light.color = displayToLinear(src.nextVec3());
        light.intensity = src.nextFloat();
        m_lights.push_back(light);
    } else if (cmd == "PointLight") {
        RdLightRecord light;
        light.type = LightType::Point;
        light.positionOrDirection = src.nextVec3();
        light.color = displayToLinear(src.nextVec3());
        light.intensity = src.nextFloat();
        m_lights.push_back(light);
    } else if (cmd == "ConeLight") {
        RdLightRecord light;
        light.type = LightType::Spot;
        light.positionOrDirection = src.nextVec3();
        light.spotTarget = src.nextVec3();
        // RD gives full angular extents; the engine wants half-angles
        light.innerHalfAngle = glm::radians(src.nextFloat() * 0.5f);
        light.outerHalfAngle = glm::radians(src.nextFloat() * 0.5f);
        light.color = displayToLinear(src.nextVec3());
        light.intensity = src.nextFloat();
        m_lights.push_back(light);
    }

    // ---- Attribute maps (parsed and skipped; textures are not loaded) ----
    else if (cmd == "MapLoad") {
        src.nextQuoted();
        src.nextQuoted();
        warn("MapLoad skipped (texture mapping is not implemented)");
    } else if (cmd == "Map") {
        src.nextQuoted();
        src.nextQuoted();
        warn("Map skipped (texture mapping is not implemented)");
    } else if (cmd == "MapSample") {
        src.nextQuoted();
        src.nextQuoted();
        src.nextQuoted();
        warn("MapSample skipped (texture mapping is not implemented)");
    } else if (cmd == "MapBound") {
        src.nextQuoted();
        for (int i = 0; i < 4; ++i) src.nextFloat();
        warn("MapBound skipped (texture mapping is not implemented)");
    } else if (cmd == "MapBorder") {
        src.nextQuoted();
        src.nextQuoted();
        src.nextQuoted();
        warn("MapBorder skipped (texture mapping is not implemented)");
    }

    else {
        return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Geometry
// ---------------------------------------------------------------------------

void RdParser::handleGeometry(RdTokenSource& src, const std::string& cmd) {
    if (cmd == "Sphere" || cmd == "SqSphere") {
        const float radius = src.nextFloat();
        float north = 1.0f, east = 1.0f;
        if (cmd == "SqSphere") {
            north = src.nextFloat();
            east = src.nextFloat();
        }
        const float zMin = src.nextFloat();
        const float zMax = src.nextFloat();
        const float thetaMax = src.nextFloat();

        char key[128];
        std::snprintf(key, sizeof(key), "sqsphere|%.5f|%.5f|%.5f|%.5f|%.5f|%.5f|%d",
                      radius, north, east, zMin, zMax, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createSuperquadricSphere(
                m_memoryManager, radius, north, east, zMin, zMax, thetaMax,
                latSegments(), lonSegments());
        }
        // A partial sweep exposes the inside of the shell, so render it two-sided
        const bool partial = thetaMax < 359.0f;
        emitPrimitive(mesh, glm::mat4(1.0f), cmd == "Sphere" ? "sphere" : "sqsphere", partial);

    } else if (cmd == "Torus" || cmd == "SqTorus") {
        const float radius1 = src.nextFloat();
        const float radius2 = src.nextFloat();
        float north = 1.0f, east = 1.0f;
        if (cmd == "SqTorus") {
            north = src.nextFloat();
            east = src.nextFloat();
        }
        const float phiMin = src.nextFloat();
        const float phiMax = src.nextFloat();
        const float thetaMax = src.nextFloat();

        char key[160];
        std::snprintf(key, sizeof(key), "sqtorus|%.5f|%.5f|%.5f|%.5f|%.5f|%.5f|%.5f|%d",
                      radius1, radius2, north, east, phiMin, phiMax, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createSuperquadricTorus(
                m_memoryManager, radius1, radius2, north, east, phiMin, phiMax, thetaMax,
                lonSegments(), latSegments());
        }
        const bool partial = thetaMax < 359.0f || (phiMax - phiMin) < 359.0f;
        emitPrimitive(mesh, glm::mat4(1.0f), cmd == "Torus" ? "torus" : "sqtorus", partial);

    } else if (cmd == "Cube") {
        // RD cube spans -1..+1; the engine cube spans -0.5..+0.5
        auto& mesh = m_meshCache["cube"];
        if (!mesh) mesh = PrimitiveFactory::createCube(m_memoryManager);
        emitPrimitive(mesh, glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)), "cube");

    } else if (cmd == "Cylinder") {
        const float radius = src.nextFloat();
        const float zMin = src.nextFloat();
        const float zMax = src.nextFloat();
        const float thetaMax = src.nextFloat();

        char key[128];
        std::snprintf(key, sizeof(key), "rdcylinder|%.5f|%.5f|%.5f|%.5f|%d",
                      radius, zMin, zMax, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createRdCylinder(m_memoryManager, radius, zMin, zMax,
                                                      thetaMax, lonSegments());
        }
        // RenderMan-style cylinders are open tubes with no end caps
        emitPrimitive(mesh, glm::mat4(1.0f), "cylinder", /*doubleSided=*/true);

    } else if (cmd == "Cone") {
        const float height = src.nextFloat();
        const float radius = src.nextFloat();
        const float thetaMax = src.nextFloat();

        char key[128];
        std::snprintf(key, sizeof(key), "rdcone|%.5f|%.5f|%.5f|%d",
                      height, radius, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createRdCone(m_memoryManager, height, radius,
                                                  thetaMax, lonSegments());
        }
        emitPrimitive(mesh, glm::mat4(1.0f), "cone", /*doubleSided=*/true);

    } else if (cmd == "Disk") {
        const float height = src.nextFloat();
        const float radius = src.nextFloat();
        const float thetaMax = src.nextFloat();

        char key[128];
        std::snprintf(key, sizeof(key), "rddisk|%.5f|%.5f|%.5f|%d",
                      height, radius, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createRdDisk(m_memoryManager, height, radius,
                                                  thetaMax, lonSegments());
        }
        emitPrimitive(mesh, glm::mat4(1.0f), "disk");

    } else if (cmd == "Hyperboloid") {
        const glm::vec3 p1 = src.nextVec3();
        const glm::vec3 p2 = src.nextVec3();
        const float thetaMax = src.nextFloat();

        char key[192];
        std::snprintf(key, sizeof(key), "hyperboloid|%.5f,%.5f,%.5f|%.5f,%.5f,%.5f|%.5f|%d",
                      p1.x, p1.y, p1.z, p2.x, p2.y, p2.z, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createHyperboloid(m_memoryManager, p1, p2, thetaMax,
                                                       lonSegments(), latSegments());
        }
        emitPrimitive(mesh, glm::mat4(1.0f), "hyperboloid", /*doubleSided=*/true);

    } else if (cmd == "Paraboloid") {
        const float radius = src.nextFloat();
        const float zMin = src.nextFloat();
        const float zMax = src.nextFloat();
        const float thetaMax = src.nextFloat();

        char key[160];
        std::snprintf(key, sizeof(key), "paraboloid|%.5f|%.5f|%.5f|%.5f|%d",
                      radius, zMin, zMax, thetaMax, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createParaboloid(m_memoryManager, radius, zMin, zMax,
                                                      thetaMax, lonSegments(), latSegments());
        }
        emitPrimitive(mesh, glm::mat4(1.0f), "paraboloid", /*doubleSided=*/true);

    } else if (cmd == "Tube") {
        const glm::vec3 p1 = src.nextVec3();
        const glm::vec3 p2 = src.nextVec3();
        const float radius = src.nextFloat();

        // "Nothing here that can't be done with a Cylinder and the proper
        // transformations" — build a unit-length cylinder along +Z once and
        // orient it per instance, so a scene full of tubes shares one mesh.
        const glm::vec3 delta = p2 - p1;
        const float length = glm::length(delta);
        if (length < 1e-6f) {
            warn("Tube with coincident endpoints skipped");
            return;
        }

        char key[96];
        std::snprintf(key, sizeof(key), "tube|%.5f|%d", radius, m_divisions);
        auto& mesh = m_meshCache[key];
        if (!mesh) {
            mesh = PrimitiveFactory::createRdCylinder(m_memoryManager, radius, 0.0f, 1.0f,
                                                      360.0f, lonSegments());
        }

        glm::mat4 fix = glm::translate(glm::mat4(1.0f), p1);
        fix *= glm::mat4_cast(TransformUtils::shortestArcRotation(glm::vec3(0.0f, 0.0f, 1.0f),
                                                                  delta / length));
        // Only z scales: scaling x/y here would also scale the radius
        fix = glm::scale(fix, glm::vec3(1.0f, 1.0f, length));
        emitPrimitive(mesh, fix, "tube", /*doubleSided=*/true);

    } else if (cmd == "PolySet") {
        const size_t declLine = src.line();
        const RdVertexType vertexType = parseVertexType(src.nextQuoted(), declLine);
        const int vertexCount = src.nextInt();
        const int faceCount = src.nextInt();
        // Zero counts are degenerate but legal — generators emit them when a
        // frame has nothing to draw. Only negative counts are malformed.
        if (vertexCount < 0 || faceCount < 0) {
            failAt(declLine, "PolySet with negative counts");
        }

        std::vector<Vertex> vertices = readVertices(src, vertexType, vertexCount);
        const auto faces = readIndexRuns(src, faceCount, vertexCount);
        auto mesh = buildPolygonMesh("rd_polyset_" + std::to_string(m_polySetCount++),
                                     std::move(vertices), faces, vertexType.hasNormal);
        emitPrimitive(std::move(mesh), glm::mat4(1.0f), "polyset", /*doubleSided=*/true);

    } else if (cmd == "Subdivision") {
        const size_t declLine = src.line();
        const std::string type = src.nextQuoted();
        const RdVertexType vertexType = parseVertexType(src.nextQuoted(), declLine);
        const int vertexCount = src.nextInt();
        const int faceCount = src.nextInt();
        const int creaseCount = src.nextInt();
        if (vertexCount < 0 || faceCount < 0 || creaseCount < 0) {
            failAt(declLine, "Subdivision with negative counts");
        }

        std::vector<Vertex> vertices = readVertices(src, vertexType, vertexCount);
        const auto faces = readIndexRuns(src, faceCount, vertexCount);
        // Each crease is an index run followed by an equally sized run of
        // sharpness values; consume both so the stream stays aligned.
        for (int i = 0; i < creaseCount; ++i) {
            const auto crease = readIndexRuns(src, 1, vertexCount);
            for (size_t j = 0; j < crease[0].size(); ++j) src.nextFloat();
        }

        warn("Subdivision \"" + type + "\" renders its control cage; the limit surface is not computed");
        auto mesh = buildPolygonMesh("rd_subdivision_" + std::to_string(m_polySetCount++),
                                     std::move(vertices), faces, vertexType.hasNormal);
        emitPrimitive(std::move(mesh), glm::mat4(1.0f), "subdivision", /*doubleSided=*/true);

    } else if (cmd == "Patch") {
        const size_t declLine = src.line();
        const std::string patchType = src.nextQuoted();
        const RdVertexType vertexType = parseVertexType(src.nextQuoted(), declLine);
        const int degreeU = src.nextInt();
        const int degreeV = src.nextInt();
        if (degreeU < 1 || degreeV < 1) failAt(declLine, "Patch with non-positive degree");

        const int count = (degreeU + 1) * (degreeV + 1);
        std::vector<Vertex> control = readVertices(src, vertexType, count);
        if (patchType != "Bezier") {
            warn("Patch \"" + patchType + "\" is not supported, skipped");
            return;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> colors;
        positions.reserve(control.size());
        for (const auto& vertex : control) {
            positions.push_back(vertex.pos);
            if (vertexType.hasColor) colors.push_back(vertex.color);
        }

        const uint32_t steps = static_cast<uint32_t>(std::max(m_divisions, 16));
        auto mesh = PrimitiveFactory::createBezierPatch(m_memoryManager, positions, colors,
                                                        degreeU, degreeV, steps, steps);
        // A patch is an open surface, so it must stay visible from both sides
        emitPrimitive(std::move(mesh), glm::mat4(1.0f), "patch", /*doubleSided=*/true);

    } else if (cmd == "Curve") {
        const size_t declLine = src.line();
        const std::string curveType = src.nextQuoted();
        const RdVertexType vertexType = parseVertexType(src.nextQuoted(), declLine);
        const int degree = src.nextInt();
        if (degree < 1) failAt(declLine, "Curve with non-positive degree");

        std::vector<Vertex> control = readVertices(src, vertexType, degree + 1);
        if (curveType != "Bezier") {
            warn("Curve \"" + curveType + "\" is not supported, skipped");
            return;
        }

        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> colors;
        positions.reserve(control.size());
        for (const auto& vertex : control) {
            positions.push_back(vertex.pos);
            if (vertexType.hasColor) colors.push_back(vertex.color);
        }

        const uint32_t steps = static_cast<uint32_t>(std::max(m_divisions, 32));
        auto mesh = PrimitiveFactory::createBezierCurve(m_memoryManager, positions, colors,
                                                        degree, steps);
        emitUnlit(std::move(mesh), "curve", vertexType.hasColor);

    } else if (cmd == "Point") {
        Vertex vertex{};
        vertex.pos = src.nextVec3();
        emitUnlit(PrimitiveFactory::createPointSet(m_memoryManager,
                                                   "rd_point_" + std::to_string(m_primitiveCount),
                                                   {vertex}),
                  "point", /*verticesCarryColor=*/false);

    } else if (cmd == "PointSet") {
        const size_t declLine = src.line();
        const RdVertexType vertexType = parseVertexType(src.nextQuoted(), declLine);
        const int vertexCount = src.nextInt();
        if (vertexCount < 0) failAt(declLine, "PointSet with negative count");

        std::vector<Vertex> vertices = readVertices(src, vertexType, vertexCount);
        emitUnlit(PrimitiveFactory::createPointSet(m_memoryManager,
                                                   "rd_pointset_" + std::to_string(m_primitiveCount),
                                                   vertices),
                  "pointset", vertexType.hasColor);

    } else if (cmd == "Line") {
        Vertex a{};
        Vertex b{};
        a.pos = src.nextVec3();
        b.pos = src.nextVec3();
        emitUnlit(PrimitiveFactory::createLineSet(m_memoryManager,
                                                  "rd_line_" + std::to_string(m_primitiveCount),
                                                  {a, b}, {{0, 1}}),
                  "line", /*verticesCarryColor=*/false);

    } else if (cmd == "LineSet") {
        const size_t declLine = src.line();
        const RdVertexType vertexType = parseVertexType(src.nextQuoted(), declLine);
        const int vertexCount = src.nextInt();
        const int sequenceCount = src.nextInt();
        if (vertexCount < 0 || sequenceCount < 0) {
            failAt(declLine, "LineSet with negative counts");
        }

        std::vector<Vertex> vertices = readVertices(src, vertexType, vertexCount);
        const auto polylines = readIndexRuns(src, sequenceCount, vertexCount);
        emitUnlit(PrimitiveFactory::createLineSet(m_memoryManager,
                                                  "rd_lineset_" + std::to_string(m_primitiveCount),
                                                  vertices, polylines),
                  "lineset", vertexType.hasColor);

    } else if (cmd == "Circle") {
        const glm::vec3 center = src.nextVec3();
        const float radius = src.nextFloat();

        // RD draws this screen-aligned, which a static mesh cannot reproduce
        // without a billboarding pass. Emit it in the XY plane and say so.
        warn("Circle is drawn in the XY plane rather than screen-aligned");

        const uint32_t segments = static_cast<uint32_t>(std::max(m_divisions * 2, 48));
        std::vector<Vertex> vertices;
        std::vector<uint32_t> loop;
        vertices.reserve(segments);
        loop.reserve(segments + 1);
        for (uint32_t i = 0; i < segments; ++i) {
            const float theta = (static_cast<float>(i) / static_cast<float>(segments)) *
                                glm::radians(360.0f);
            Vertex vertex{};
            vertex.pos = center + glm::vec3{radius * std::cos(theta), -radius * std::sin(theta), 0.0f};
            vertices.push_back(vertex);
            loop.push_back(i);
        }
        loop.push_back(0);  // close the loop

        emitUnlit(PrimitiveFactory::createLineSet(m_memoryManager,
                                                  "rd_circle_" + std::to_string(m_primitiveCount),
                                                  vertices, {loop}),
                  "circle", /*verticesCarryColor=*/false);

    } else if (cmd == "Fill") {
        src.nextVec3();
        // A screen-space flood fill has no equivalent in a depth-buffered
        // rasterizer: there is no "region bounded by what was already drawn".
        warn("Fill is a screen-space flood fill with no equivalent in this renderer, skipped");
    }
}

// ---------------------------------------------------------------------------
// Dispatch
// ---------------------------------------------------------------------------

void RdParser::run(RdTokenSource& src) {
    static const std::set<std::string> kGeometryCommands = {
        "Sphere", "SqSphere", "Torus", "SqTorus", "Cube", "Cylinder", "Cone",
        "Disk", "Hyperboloid", "Paraboloid", "Tube", "PolySet", "Subdivision",
        "Patch", "Curve", "Point", "PointSet", "Line", "LineSet", "Circle", "Fill"
    };

    while (!src.atEnd()) {
        const std::string cmd = src.next();

        // ---- Structure ----
        if (cmd == "WorldBegin") {
            m_xformStack.assign(1, glm::mat4(1.0f));
        } else if (cmd == "WorldEnd") {
            // Each frame block holds exactly one world block, so this is the
            // reliable frame boundary even in files with no FrameBegin.
            flushFrame();
        } else if (cmd == "FrameBegin") {
            flushPendingContent();
            m_frameNumber = src.nextInt();
        } else if (cmd == "FrameEnd") {
            flushPendingContent();
        } else if (cmd == "ObjectBegin") {
            recordObject(src);
        } else if (cmd == "ObjectEnd") {
            warn("ObjectEnd without a matching ObjectBegin, ignored");
        } else if (cmd == "ObjectInstance") {
            instanceObject(src);
        } else if (kGeometryCommands.count(cmd)) {
            handleGeometry(src, cmd);
        } else if (!handleAttributeOrLight(src, cmd)) {
            warn("unknown RD command \"" + cmd + "\" skipped (following tokens may misparse)");
        }
    }
}

// ---------------------------------------------------------------------------
// Frame construction
// ---------------------------------------------------------------------------

void RdParser::flushFrame() {
    RdScene scene;
    scene.root = std::make_shared<SceneNode>("RdSceneRoot");
    scene.camera = m_camera;
    scene.background = m_background;
    scene.ambient = m_ambient;
    scene.frameNumber = m_frameNumber;

    // RD has no emission concept: scenes represent glowing objects (suns, light
    // stars) by placing a PointLight at a primitive's center and leaning on
    // ambient to make the mesh visible. Interpret that intent in the modern
    // renderer by giving the primitive hosting each point light an emissive glow
    // scaled from the light's color and intensity.
    for (const auto& lightRecord : m_lights) {
        if (lightRecord.type != LightType::Point) continue;

        RdPrimitiveRecord* host = nullptr;
        float hostRadius = std::numeric_limits<float>::max();
        for (auto& prim : m_primitives) {
            if (prim.unlit) continue;  // lines and points are already self-coloured
            const glm::vec3 center = glm::vec3(prim.xform * glm::vec4(prim.mesh->getBoundsCenter(), 1.0f));
            const float radius = prim.mesh->getBoundingRadius() * maxScaleFactor(prim.xform);
            const float distance = glm::length(center - lightRecord.positionOrDirection);
            // The light must sit inside the primitive; prefer the smallest such
            // primitive (a sun at the origin beats rings centered there)
            if (distance < radius * 0.5f + 1e-3f && radius < hostRadius) {
                host = &prim;
                hostRadius = radius;
            }
        }
        if (host) {
            host->emissiveBoost += lightRecord.color * std::clamp(lightRecord.intensity * 0.25f, 0.3f, 3.0f);
        }
    }

    // Scene bounds (world space) for camera framing and light ranges
    glm::vec3 boundsMin{std::numeric_limits<float>::max()};
    glm::vec3 boundsMax{std::numeric_limits<float>::lowest()};

    std::map<std::string, std::shared_ptr<Material>> materialCache;
    int materialCount = 0;

    for (const auto& record : m_primitives) {
        const RdSurfaceState& s = record.surface;
        const glm::vec3 colorLinear = displayToLinear(s.color);
        const glm::vec3 specLinear = displayToLinear(s.specularColor);

        // RD shading formulas mapped onto the engine's material slots:
        //   matte:   Cs*(Ka*La + Kd*Id)                 -> diffuse Cs*Kd, no specular
        //   plastic: Cs*(Ka*La + Kd*Id) + Cspec*Ks*Is   -> diffuse Cs*Kd, specular Cspec*Ks
        //   metal:   Cs*(Ka*La + Ks*Is)                 -> no diffuse, specular Cs*Ks
        // The ambient term goes through the renderer's global ambient, which
        // multiplies the diffuse color (so vertex-colored PolySets stay tinted);
        // Ka is approximated by Kd there (see header).
        glm::vec3 diffuse{0.0f};
        glm::vec3 specular{0.0f};
        glm::vec3 emissive = record.emissiveBoost;

        if (record.unlit) {
            // Zero diffuse and specular remove every lighting term, leaving the
            // emissive slot to carry the drawing colour verbatim.
            emissive = record.unlitColor;
        } else {
            switch (s.shader) {
                case RdSurfaceState::Shader::Matte:
                    diffuse = colorLinear * s.kd;
                    break;
                case RdSurfaceState::Shader::Plastic:
                    diffuse = colorLinear * s.kd;
                    specular = specLinear * s.ks;
                    break;
                case RdSurfaceState::Shader::Metal:
                    // RD metal = Cs*(Ka*La + Ks*Is). Route Cs*Ka through the
                    // diffuse slot (which the renderer multiplies by the global
                    // ambient) so the metal has an ambient body instead of a
                    // black silhouette; specular carries Cs*Ks. This adds a faint
                    // Lambertian term the pure RD model lacks, an accepted
                    // approximation for the shared slot.
                    diffuse = colorLinear * s.ka;
                    specular = colorLinear * s.ks;
                    break;
            }
        }

        const glm::vec4 baseColor{diffuse, s.opacity};
        const std::string key = materialKey(baseColor, specular, emissive, s.specularExponent,
                                            record.doubleSided, record.wireframe, record.unlit);
        auto& material = materialCache[key];
        if (!material) {
            MaterialFactors factors;
            factors.baseColorFactor = baseColor;
            factors.specularColor = specular;
            factors.emissiveFactor = emissive;
            factors.shininess = s.specularExponent;
            factors.roughnessFactor = 0.0f;  // pass the RD specular exponent through unmodified
            factors.metallicFactor = 0.0f;
            factors.doubleSided = record.doubleSided;
            factors.wireframe = record.wireframe;
            factors.unlit = record.unlit;
            const AlphaMode alphaMode = (s.opacity < 0.999f) ? AlphaMode::Blend : AlphaMode::Opaque;
            material = std::make_shared<Material>("rd_material_" + std::to_string(materialCount++),
                                                  factors, alphaMode);
        }

        auto node = std::make_shared<SceneNode>(record.name);
        glm::vec3 position, scale;
        glm::quat rotation;
        TransformUtils::decomposeMatrix(record.xform, position, rotation, scale);
        node->getTransform().setPosition(position);
        node->getTransform().setRotation(rotation);
        node->getTransform().setScale(scale);
        node->addComponent<MeshRenderer>(record.mesh, material);
        scene.root->addChild(node);

        const glm::vec3 worldCenter = glm::vec3(record.xform * glm::vec4(record.mesh->getBoundsCenter(), 1.0f));
        const float worldRadius = record.mesh->getBoundingRadius() * maxScaleFactor(record.xform);
        boundsMin = glm::min(boundsMin, worldCenter - glm::vec3(worldRadius));
        boundsMax = glm::max(boundsMax, worldCenter + glm::vec3(worldRadius));
    }

    if (!m_primitives.empty()) {
        scene.boundsCenter = (boundsMin + boundsMax) * 0.5f;
        scene.boundsRadius = std::max(glm::length(boundsMax - boundsMin) * 0.5f, 0.001f);
    }

    // RD point lights have pure inverse-square falloff (no cutoff); give the
    // engine's windowed falloff a range generous enough to cover the scene
    const float lightRange = std::max(scene.boundsRadius * 4.0f, 10.0f);

    std::vector<RdLightRecord> lights = m_lights;
    if (lights.size() > kMaxLights) {
        std::ostringstream msg;
        msg << "frame " << m_frameNumber << " has " << lights.size() << " lights; engine supports "
            << kMaxLights << ", extra lights dropped";
        warn(msg.str());
        lights.resize(kMaxLights);
    }

    int lightCount = 0;
    for (const auto& record : lights) {
        auto node = std::make_shared<SceneNode>("rd_light_" + std::to_string(lightCount++));
        auto light = node->addComponent<Light>();
        light->setType(record.type);
        light->setColor(record.color);
        light->setIntensity(record.intensity);

        if (record.type == LightType::Directional) {
            // Engine light direction is the node's forward (-Z); aim it along
            // the direction the RD light shines
            const glm::vec3 dir = glm::normalize(record.positionOrDirection);
            node->getTransform().setRotation(
                TransformUtils::shortestArcRotation(glm::vec3(0.0f, 0.0f, -1.0f), dir));
        } else {
            node->getTransform().setPosition(record.positionOrDirection);
            light->setRange(lightRange);
            if (record.type == LightType::Spot) {
                const glm::vec3 dir = glm::normalize(record.spotTarget - record.positionOrDirection);
                node->getTransform().setRotation(
                    TransformUtils::shortestArcRotation(glm::vec3(0.0f, 0.0f, -1.0f), dir));
                light->setConeAngles(record.innerHalfAngle, record.outerHalfAngle);
            }
        }
        scene.root->addChild(node);
    }

    LOG_INFO(GENERAL, "RdSceneLoader: frame {} ({} primitives, {} lights, {} materials)",
             m_frameNumber, m_primitives.size(), lights.size(), materialCount);

    m_out.frames.push_back(std::move(scene));

    // Reset per-frame state. Surface attributes, camera and options persist:
    // "Attribute values hold between frames of an animation as well."
    m_primitives.clear();
    m_lights.clear();
    m_ambient = glm::vec3(1.0f);
    m_xformStack.assign(1, glm::mat4(1.0f));
    ++m_frameNumber;
}

} // namespace

Result<RdSceneFile> RdSceneLoader::load(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        return Result<RdSceneFile>(Error("Failed to open RD file: " + path));
    }
    std::stringstream buffer;
    buffer << file.rdbuf();

    RdSceneFile out;
    RdParser parser(m_memoryManager, out);

    try {
        RdTokenizer tok(buffer.str());
        parser.run(tok);
        parser.flushAtEof();
    } catch (const std::exception& e) {
        return Result<RdSceneFile>(Error(std::string(e.what()) + " [" + path + "]"));
    }

    if (out.frames.empty()) {
        return Result<RdSceneFile>(Error("RD file contains no renderable frames: " + path));
    }

    // Aggregate bounds across every frame so an animation frames consistently
    // instead of the camera jumping as geometry moves.
    glm::vec3 boundsMin{std::numeric_limits<float>::max()};
    glm::vec3 boundsMax{std::numeric_limits<float>::lowest()};
    bool any = false;
    for (const auto& frame : out.frames) {
        if (frame.boundsRadius <= 0.001f) continue;
        boundsMin = glm::min(boundsMin, frame.boundsCenter - glm::vec3(frame.boundsRadius));
        boundsMax = glm::max(boundsMax, frame.boundsCenter + glm::vec3(frame.boundsRadius));
        any = true;
    }
    if (any) {
        out.boundsCenter = (boundsMin + boundsMax) * 0.5f;
        out.boundsRadius = std::max(glm::length(boundsMax - boundsMin) * 0.5f, 0.001f);
    }

    LOG_INFO(GENERAL, "RdSceneLoader: loaded {} ({} frames, {} warnings)",
             path, out.frames.size(), out.warnings.size());

    return Result<RdSceneFile>(std::move(out));
}

} // namespace vkeng
