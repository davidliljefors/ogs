#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_Model;
uniform mat4 u_VP;

out vec3 f_World;
out vec3 f_Normal;
out vec2 f_TexCoord;

void main()
{
    f_World = (u_Model * vec4(a_Position, 1.0)).xyz;
    f_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;
    f_TexCoord = a_TexCoord;

    gl_Position = u_VP * u_Model * vec4(a_Position, 1.0);
}