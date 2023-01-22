#version 460 core
out vec4 FragColor;

in vec3 posInView;
in vec3 normalInView;
in vec3 lightInView;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main() {
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
    FragColor = vec4(result, 1.0);
}