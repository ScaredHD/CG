#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec3 fragInView;
out vec3 lightInView;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    normal = mat3(transpose(inverse(view * model))) * aNormal;
    fragInView = vec3(view * model * vec4(aPos, 1.0));
    texCoord = aTexCoord;
}