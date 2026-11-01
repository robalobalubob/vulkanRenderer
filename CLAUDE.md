# CLAUDE.md — vkeng (vulkanRenderer)

C++17 Vulkan renderer. **Scope (rescoped 2026-10-31): a renderer, not a game engine foundation.** Physics, audio and scene serialization were removed as unused stubs (last present at commit 607f22d) and are out of scope. ~18k lines under `src/` and `include/vulkan-engine/`, 4 GLSL shaders. Three apps: `test_scene` (TestSceneApp), `model_viewer` (ModelViewerApp), and `rd_viewer` (RdViewerApp — loads `.rd` scene files). `Engine` owns the `Renderer`, which owns all rendering resources; an app only builds a scene and calls `renderer_->drawFrame(root, camera)`. New rendering features belong in `Renderer`, not in the apps. Architecture details live in [README.md](README.md).

## Build (primary: native Windows, since 2026-07-19)

Development migrated from WSL2 to native Windows to avoid WSL graphical issues.

- CMake: VS 2022 bundled, at `C:\Program Files\Microsoft Visual Studio\2022\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe` (not on PATH)
- Dependencies: vcpkg at `C:\Users\liber\vcpkg` (classic mode, glfw3 + glm installed x64-windows); Vulkan SDK 1.4.313.2 (`VULKAN_SDK` set); VMA is a git submodule

```
cmake -B build -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE=C:/Users/liber/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Debug --parallel
build\Debug\test_scene.exe
```

**WSL fallback:** Ubuntu-22.04 distro (`wsl -d Ubuntu-22.04 -u robalob` — default distro is docker-desktop, don't use it), repo copy at `~/projects/vulkanRenderer`, full toolchain installed.

**Gotchas:**
- Only `assets/rd/` and `assets/cube.obj` are tracked. The other assets have unknown licensing and stay gitignored, so copy them by hand for a new checkout; `test_scene` wants `testPNG.png`.
- New engine `.cpp` files must be registered in the `vulkan-engine` STATIC library list in `CMakeLists.txt`.

## Testing

`ctest --test-dir build -C Debug --output-on-failure` (`ctest.exe` sits beside `cmake.exe`). Each test renders a scene through `rd_viewer` or `model_viewer`, compares it with `tests/golden/`, and fails on any validation message or error. Run it before and after rendering changes. Set `VKENG_UPDATE_GOLDENS=1` to replace the images after an intended visual change, and review them. The tests do not cover resizing or `test_scene` (its animation depends on frame timing), so after changing swapchain or lifecycle code, also resize, minimize and close each app and check its log for `Validation:`.

## Status (verified 2026-10-31)

Build passing with no warnings on Windows (MSVC Debug and Release, default warning level); all three apps run with zero validation messages, including through resize, minimize and close.

Added 2026-10-31 (uncommitted): **rendering setup moved out of the apps.** `Renderer` now owns the render pass, pipeline manager, descriptors, uniform buffers and shadow pass, and rebuilds them itself on resize; `Engine` owns the `Renderer` and handles `requestCapture()`, so every app can capture. Path resolution moved to `core/RuntimePaths`. `HelloTriangleApp` was renamed `TestSceneApp`. Renders were byte-identical before and after.

Added 2026-07-26 (committed in 607f22d): **RD support completed to full geometry-spec coverage.**
- **Pipeline topology/polygon mode**: `PipelineConfig` gained `topology` (TriangleList/LineList/LineStrip/
  PointList) and `polygonMode` (Fill/Line/Point); `Mesh` carries its topology and the renderer picks a
  matching variant. `VulkanDevice` now queries and enables `fillModeNonSolid`/`wideLines`/`largePoints`
  (it previously enabled *no* optional features). This is what makes RD lines, points and
  `OptionBool "Wireframe"` renderable at all.
- **New primitives**: `createRdCylinder/RdCone/RdDisk/Hyperboloid/Paraboloid/BezierPatch/BezierCurve/
  LineSet/PointSet`, all z-up RD convention with partial angular extents.
- **Loader**: every remaining geometry command, `ObjectBegin`/`ObjectInstance` with `$n` parameters
  (implemented by recording the token stream and replaying it through an `RdTokenSource` interface), and
  multi-frame `FrameBegin`/`FrameEnd` parsing. `load()` now returns `RdSceneFile` (frames + display
  settings) rather than a single `RdScene`.
- **rd_viewer**: animation playback (Space/arrows) and `--capture <out.ppm> [--frame n]` headless capture.
- **Screenshots**: `docs/screenshots/rd_coverage.png`, `rd_neutron_static.png`, `rd_neutron_dynamic.png`.
- **New test scenes** (gitignored with the rest of `assets/`): `coverage.rd` (hand-written, exercises every
  command the generators don't emit), `neutron_static.rd`, `neutron_dynamic.rd` (40 frames) — the latter two
  generated from the portfolio generators via WSL g++.

Added 2026-07-26 (uncommitted): **material data moved out of push constants**, after auditing whether RD
support had dragged in RD-era simplifications. The audit found RD's *semantics* stayed properly inside
`RdSceneLoader`; these were the leaks, all now fixed:
- **Material data now lives in a per-frame storage buffer** (set 0, binding 1), indexed by a material index
  in `MeshPushConstants` — which is now just `modelMatrix` + index (80 of 128 bytes). `GpuMaterial` carries
  an explicit `MaterialFlagBits` bitfield. This removed a sentinel I had added (negative `emissiveFactor.w`
  meaning "unlit") *and* the pre-existing `alphaCutoff`/`pbrFactors` packing. Adding a material property no
  longer means finding a spare float component.
- **`GlobalDescriptors.hpp`**: the set-0 layout/pool/set creation the three apps each duplicated verbatim
  now has one definition, so the layout cannot drift out of sync with the shaders per-app.
- **Wireframe moved from `MaterialFactors` to `MeshRenderer`** — it is rasterizer state, not a surface
  property (glTF has no such concept); nodes sharing a material can now differ in it.
- **`Renderer::captureFrame` routes through MemoryManager/VMA** instead of hand-rolled
  `vkCreateBuffer`/`vkAllocateMemory`. Note it needs `BufferUsage::TransferDst`, not `createStagingBuffer`
  (which is TRANSFER_SRC, for uploads).
- **`createRdCylinder/RdCone/RdDisk` renamed `createCylinderZ/ConeZ/DiskZ`** — the real distinction is the
  sweep axis, not the file format that motivated them.
- **Fixed a latent `Buffer::unmap()` bug** surfaced by the capture path: host-visible buffers are created
  `VMA_ALLOCATION_CREATE_MAPPED_BIT` (persistently mapped), so `map()` is a no-op but `unmap()` still called
  `vmaUnmapMemory` → VMA assertion. Now tracked with `m_explicitlyMapped`. Nothing had called `unmap()` before.

Verified byte-identical renders before/after the refactor (`md5sum` on captured PPMs).

**Winding convention (hard-won — do not re-derive):** front faces are standard CCW, i.e. the triangle's
right-hand normal points *toward* the viewer. `glm::perspective`'s handedness flip and the
`m_projectionMatrix[1][1] *= -1` at `Camera.cpp:134` cancel out; reasoning about either one alone gets the
answer backwards. Verified by rendering shapes single-sided and looking. `PrimitiveFactory`'s
`emitGridIndices` encodes this for all swept surfaces.

Added 2026-07-20 (uncommitted): **RD scene support** — the graphics milestone enabling the portfolio
scenes from `C:\Users\liber\projects\portfolio\rd_view` to be re-rendered in this engine:
- `RdSceneLoader` (`src/scene/RdSceneLoader.cpp`): parses `.rd` files into scene graphs. See the header
  for the lighting-model mapping (RD Ka/Kd/Ks/Specular → material slots; ambient via renderer global
  ambient with Ka≈Kd; point-light-hosting primitives get an emissive glow since RD has no emission).
- `PrimitiveFactory::createSuperquadricSphere/Torus`: RD-convention z-up superquadrics, analytic normals,
  partial sweeps. RD `Sphere`/`Torus` route through them with exponents 1.
- `rd_viewer` app + `assets/rd/*.rd` test scenes (generated from the portfolio generators via WSL g++;
  gitignored with the rest of assets/). Screenshots: `docs/screenshots/rd_superquadrics.png`, `rd_terrain.png`.
- Renderer: settable global ambient (`setAmbientColor`) and background clear color (`setClearColor`).
- **Point/spot attenuation changed** from `(1-d/range)²` to windowed inverse-square (`window²/d²`) —
  physically correct and required by the RD spec. Existing scenes' point/spot intensities read differently
  (brighter near, dimmer far); test_scene still looks fine.
- **Sync fix**: render-finished semaphores are now per swapchain image (indexed by `imageIndex`), not per
  frame in flight — fixes intermittent `VUID-vkQueueSubmit-pSignalSemaphores-00067` at startup.

Conventions worth knowing:
- RD authored colors are display-space; the loader decodes them to linear (`pow 2.2`) so the sRGB
  swapchain round-trips them. FarLight vectors point **toward** the light (rd_view/OpenGL convention,
  opposite of the RD spec text — the scenes' comments confirm rd_view behaved this way).
- Windows note: `timeout N ./app.exe` with exit 124 = healthy run; check logs for `Validation:`, the prefix the debug callback actually writes.

Fixed 2026-07-19 (committed since): `Logger::log()` null source-location crash — `std::string filename = file;` with `file == nullptr` (the LOG_* macros never pass source location); instant 0xC0000005 on MSVC debug CRT, latent on Linux.

## Known Gaps

| Gap | Severity |
|---|---|
| Lighting is Blinn-Phong with metallic/roughness adjustments (`shader.frag:184`), not a physically based BRDF | Medium |
| OBJ is the only model format; the material slots follow glTF but there is no glTF loader | Medium |
| No post-processing pass (bloom, HDR resolve; tone mapping now done in-shader) | Medium |
| No mipmaps, anisotropic filtering or MSAA | Medium |
| Shadows: first directional light only, fixed 20-unit box at the origin (`Renderer::computeLightSpaceMatrix`) | Medium |
| Instanced rendering plumbed but `CommandBuffer` always draws 1 instance | Medium |
| `EventSystem` and the fixed-timestep loop have no users now the stubs are gone | Low |
| Frustum culling per-mesh only, no hierarchical subtree rejection | Low |
| RD `Map*` texture commands parsed but not applied; `Subdivision` renders its control cage | Low |
| `Buffer.cpp:151`: debug naming pending debug utils extension | Low |

## Roadmap (priority order — rescoped to a renderer 2026-10-31)

Done: Windows smoke test, shadow sampling, RD scene support to full geometry-spec coverage, stub removal,
render regression tests, rendering setup moved out of the apps.

1. **HDR render target and bloom** (makes the RD sun/star glow bleed properly; unlocks later screen-space effects)
2. Mipmaps and anisotropic filtering (`samplerAnisotropy` is not enabled; mipmaps must come first to matter)
3. Cook-Torrance PBR with glTF loading
4. Shadows fitted to the scene
5. MSAA
6. Later, if wanted: skybox, RD texture mapping (`Map*` family + PPM loader), Catmull-Clark subdivision,
   instancing wiring, hierarchical culling

Fixed 2026-07-19: the washed-out look was double gamma — `shader.frag` applied `pow(1/2.2)` on top of the sRGB swapchain's hardware encoding. Removed manual gamma, added ACES tone mapping, dropped test-scene material specular 0.35→0.06. Before/after: `docs/screenshots/blown_out_look.png` vs `tonemapped_look.png`. Note: the color pipeline convention is now "shader outputs linear; swapchain encodes" — any future shader must not gamma-correct manually.
