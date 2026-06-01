#version 330

in vec2 fragTexCoord;
in vec3 fragPosition;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

uniform float time;
uniform vec4 colDiffuse;

void main() {
    vec3 lightDir = normalize(vec3(sin(time), 1.0, cos(time)));



    float diff = 1.0;
    // if (length(fragNormal) > 0.0) {
    {
        diff = (1 + dot(normalize(fragNormal), lightDir)) / 2;
    }
    
    float ambient = 0.5;
    float intensity = float(int((ambient + (1.0 - ambient) * diff) * 13.0)) / 13.0;    

    float alpha = fragColor.a == 0.0 ? 1.0 : fragColor.a;
    // finalColor = vec4(vec3(intensity), alpha);
    finalColor = vec4(colDiffuse.rgb * intensity, alpha);
}
