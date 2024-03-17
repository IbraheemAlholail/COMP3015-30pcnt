#version 460

layout (binding = 0) uniform samplerCube SkyBoxTex;
in vec3 Vec;

layout (location = 0) out vec4 FragColor;

void main() {
    vec3 texColor = texture(SkyBoxTex, Vec).rgb;

	FragColor = vec4(texColor, 1.0);
}