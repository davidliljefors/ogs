#version 330 core
in vec2 a_Position;

uniform mat4 u_Model;

void main()
{
    gl_Position =  u_Model * vec4(a_Position, 0.0, 1.0);
}