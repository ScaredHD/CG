#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;

uniform sampler2D specular0;
uniform sampler2D specular1;
uniform sampler2D specular2;


void main()
{    
    vec4 diffuse = texture(diffuse0, TexCoords);
    vec4 specular = texture(specular0, TexCoords);
    vec4 res = diffuse;
    FragColor = res;

    // float ndc = gl_FragCoord.z * 2.0 - 1.0;
    // float far = 100.0;
    // float near = 0.1;
    // float depth = (2.0 * near * far) / (far + near - ndc * (far - near));
    // FragColor = vec4(vec3(depth), 1.0);

    // FragColor = vec4(gl_FragCoord.zzz, 1.0);
}