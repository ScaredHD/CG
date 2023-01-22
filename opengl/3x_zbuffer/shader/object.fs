#version 460 core
out vec4 FragColor;

in vec3 posInView;
in vec3 normalInView;
in vec3 lightInView;

uniform vec3 objectColor;
uniform vec3 lightColor;


void main() {
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}