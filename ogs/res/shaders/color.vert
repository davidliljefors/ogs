#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

out vec2 f_TexCoord;
out vec3 f_Position;
out vec3 f_Normal;

uniform mat4 u_Model;
uniform mat4 u_VP;

void main()
{
    f_TexCoord = a_TexCoord;
    f_Normal = a_Normal;
    gl_Position =  u_VP * u_Model * vec4(a_Position, 1.0);
    f_Position = gl_Position.xyz;
}