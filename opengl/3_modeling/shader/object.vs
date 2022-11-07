#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 posInView;
out vec3 normalInView;
out vec3 lightInView;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightInWorld;


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    posInView = vec3(view * model * vec4(aPos, 1.0));
    normalInView = mat3(transpose(inverse(view * model))) * aNormal;
    lightInView = vec3(view * vec4(lightInWorld, 1.0));
}