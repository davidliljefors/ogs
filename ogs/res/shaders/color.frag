#version 330 core

in vec2 f_TexCoord;
in vec3 f_Position;
in vec3 f_Normal;

out vec4 FragColor;

uniform sampler2D u_Tex0;
uniform sampler2D u_Tex1;

uniform float u_Time;
uniform vec4 u_Color;

void main()
{
    vec4 col_a = texture(u_Tex0, f_TexCoord);
    vec4 col_b = texture(u_Tex1, f_TexCoord);
    //FragColor = mix(col_a, col_b, sin(u_Time + f_Position.x + f_Position.y ) * 0.5 + 0.5) * u_Color;
    FragColor = vec4(f_Normal, 1.0) * u_Color;
}