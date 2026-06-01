#version 330

// Input vertex attributes
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

// Raylib instanced rendering transform attribute
in mat4 instanceTransform;

// Input uniform values (View-Projection matrix when instancing)
uniform mat4 mvp;

// Output vertex attributes (passed to fragment shader)
out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
flat out int fragInstanceID;

void main() {
    fragTexCoord = vertexTexCoord;
    fragColor = vertexColor;
    fragInstanceID = gl_InstanceID;
    
    // Transform vertex normal using instance rotation (top-left 3x3 of instanceTransform)
    fragNormal = normalize(vec3(instanceTransform * vec4(vertexNormal, 0.0)));
    
    // Transform vertex position using instance transform matrix
    vec4 worldPos = instanceTransform * vec4(vertexPosition, 1.0);
    
    gl_Position = mvp * worldPos;
}
