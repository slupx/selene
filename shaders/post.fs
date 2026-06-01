#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;      // Color texture bound to slot 0 (unused)
uniform sampler2D depthTexture;  // Depth texture bound to slot 1
uniform float time;
uniform float pixelSize;

const float nearPlane = 0.1;
const float farPlane = 100.0;

void main() {
    vec2 uv = fragTexCoord.xy;

    float depth = texture(depthTexture, uv).r;
    
    float linearDepth = min(max(pow(depth * 15, 1.5), 0.0), 1.0);
    
    vec3 col = mix(texture(texture0, uv).xyz, vec3(0.92, 0.89, 0.8), linearDepth);
    if (
        (depth - texture(depthTexture, uv + vec2(0, pixelSize)).r > 0.001)
        || (depth - texture(depthTexture, uv + vec2(0, -pixelSize)).r > 0.001)
        || (depth - texture(depthTexture, uv + vec2(pixelSize, 0)).r > 0.001)
        || (depth - texture(depthTexture, uv + vec2(-pixelSize, 0)).r > 0.001)
    ) {
        col *= 0.9;
    }

    finalColor = vec4(col, 1.0) * fragColor;
    // finalColor = vec4(vec3(linearDepth), 1.0);
}
