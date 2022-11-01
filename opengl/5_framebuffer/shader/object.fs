#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D diffuse0;
uniform sampler2D diffuse1;
uniform sampler2D diffuse2;

uniform sampler2D specular0;
uniform sampler2D specular1;
uniform sampler2D specular2;

uniform sampler2D tex0;


void main()
{    
    vec4 texColor = texture(tex0, TexCoords);
    FragColor = texColor;
}