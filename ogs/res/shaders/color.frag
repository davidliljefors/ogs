#version 330 core

in vec2 f_TexCoord;

out vec4 FragColor;

uniform vec4 u_Color;

void main()
{
    FragColor = u_Color;
}