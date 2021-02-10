#version 330 core

in vec2 a_Position;
in vec2 a_TexCoord;

out vec2 f_TexCoord;

uniform mat4 u_Model;

void main()
{
    f_TexCoord = a_TexCoord;
    gl_Position =  u_Model * vec4(a_Position, 0.0, 1.0);
}