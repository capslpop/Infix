// when checking for errors check CMake output log instead of the terminal
#version 450

layout(binding = 0) uniform UniformBufferObject 
{
    vec3 camDir;
    vec3 camPos;
} ubo;

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(ubo.camDir, 1.0);
}  