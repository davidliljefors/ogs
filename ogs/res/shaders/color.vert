#version 330 core
in vec2 a_Position;

uniform mat4 model_matrix;

void main()
{
    gl_Position =  model_matrix * vec4(a_Position, 0.0, 1.0);
}