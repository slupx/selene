#version 330

flat in int fragInstanceID;
out vec4 finalColor;

void main() {
    float x = float(int(fragInstanceID / 16));
    float y = fragInstanceID - x * 16;
    vec3 col = vec3(0.4, 0.839, 0.474) * (0.8 + (sin(x) + sin(y)) / 16);
    finalColor = vec4(col, 1.0);
}