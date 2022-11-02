#version 460 core

in vec2 TexCoords;

uniform sampler2D tex0;


void main()
{    
    vec4 texColor = texture(tex0, TexCoords);
    gl_FragColor = texColor;
}