#version 450

layout(binding = 0) uniform GlobalUbo {
    mat4 view;
    mat4 proj;
    vec4 cameraPosition;
    vec4 lightDirection;
    vec4 lightColor;
    vec4 ambientColor;
    vec4 debugView;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 modelMatrix;
    vec4 baseColorFactor;
    vec4 emissiveFactor;
    vec4 specularColorAndShininess;
} pushConstants;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragWorldPosition;
layout(location = 2) in vec3 fragWorldNormal;

layout(location = 0) out vec4 outColor;

vec3 applyGamma(vec3 linearColor) {
    return pow(max(linearColor, vec3(0.0)), vec3(1.0 / 2.2));
}

void main() {
    vec3 normal = normalize(fragWorldNormal);
    vec3 baseColor = fragColor * pushConstants.baseColorFactor.rgb;

    if (ubo.debugView.x > 1.5) {
        outColor = vec4(normal * 0.5 + 0.5, 1.0);
        return;
    }

    if (ubo.debugView.x > 0.5) {
        vec3 unlitColor = applyGamma(baseColor + pushConstants.emissiveFactor.rgb);
        outColor = vec4(unlitColor, pushConstants.baseColorFactor.a);
        return;
    }

    vec3 lightDir = normalize(-ubo.lightDirection.xyz);
    vec3 viewDir = normalize(ubo.cameraPosition.xyz - fragWorldPosition);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambient = ubo.ambientColor.rgb * baseColor;

    float diffuseFactor = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = baseColor * diffuseFactor * ubo.lightColor.rgb * ubo.lightColor.a;

    float specularFactor = 0.0;
    if (diffuseFactor > 0.0) {
        specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), max(pushConstants.specularColorAndShininess.a, 1.0));
    }

    vec3 specular = pushConstants.specularColorAndShininess.rgb * specularFactor * ubo.lightColor.rgb * ubo.lightColor.a;
    vec3 finalColor = ambient + diffuse + specular + pushConstants.emissiveFactor.rgb;

    finalColor = applyGamma(finalColor);
    outColor = vec4(finalColor, pushConstants.baseColorFactor.a);
}