#version 460 core
out vec4 FragColor;

in vec3 normal;
in vec3 fragInView;
in vec2 texCoord;

uniform mat4 view;

struct Light {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 
  
uniform Material material;
const int lightCnt = 10;
uniform Light lights[lightCnt];

vec3 calcPointLight(Light light);

void main() {
    vec3 res = vec3(0.0);
    for (int i = 0; i < lightCnt; ++i) {
        res += calcPointLight(lights[i]);
    }
    FragColor = vec4(res, 1.0);
}

vec3 calcPointLight(Light light) {
    vec3 lightInView = vec3(view * vec4(light.pos, 1.0));

    float dist = length(lightInView - fragInView);
    float attenuation = 1.0 / (1.0 + dist * dist);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoord));

    // diffuse
    vec3 n = normalize(normal);
    vec3 l = normalize(lightInView - fragInView);
    vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, texCoord)) * max(dot(n, l), 0.0);

    // specular
    vec3 e = normalize(-fragInView);
    vec3 h = normalize(l + e);
    vec3 specular = light.specular * vec3(texture(material.specular, texCoord)) * pow(max(dot(n, h), 0.0), material.shininess);

    // mix
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return ambient + diffuse + specular;
}