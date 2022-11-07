#version 460 core
out vec4 FragColor;

in vec3 positionInView;
in vec3 normalInView;

uniform samplerCube skybox;

void main() {
    vec3 n = normalize(normalInView);
    vec3 eye = normalize(-positionInView);
    // vec3 r = normalize(-eye + 2 * dot(eye, n) * n);
    vec3 r = reflect(normalize(positionInView), normalize(normalInView));
    FragColor = vec4(texture(skybox, r).rgb, 1.0);
}