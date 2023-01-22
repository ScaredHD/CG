#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightInWorld;
uniform vec3 lightColor;


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 posInView = vec3(view * model * vec4(aPos, 1.0));
    vec3 normalInView = mat3(transpose(inverse(view * model))) * aNormal;
    vec3 lightInView = vec3(view * vec4(lightInWorld, 1.0));

    vec3 n = normalize(normalInView);
    vec3 l = normalize(lightInView - posInView);
    vec3 e = normalize(-posInView);

    // ambient
    vec3 ka = vec3(1.0);
    vec3 ambient = ka * objectColor;

    // diffuse
    vec3 diffuse = lightColor * objectColor * max(0.0, dot(n, l));

    // specular
    float p = 128.0;
    vec3 h = normalize(e + l);
    vec3 specular = lightColor * pow(max(0.0, dot(h, n)), p);

    // total
    vec3 result = ambient + diffuse + specular;
    color = vec4(result, 1.0);
}