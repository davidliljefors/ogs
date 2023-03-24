#version 330 core

in vec3 f_Normal;
in vec2 f_TexCoord;
in vec3 f_World;

struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform vec3 u_AmbientLight;
uniform vec3 u_EyePos;

uniform Light u_Light;

uniform Material u_Material;

out vec4 FragColor;

float attenuation(float dist, float constant, float linear, float quadratic)
{
    return 1.0 / (constant + linear * dist + quadratic * (dist * dist));
}

void main()
{
    // ambient
    vec3 ambient = u_Light.ambient * texture(u_Material.diffuse, f_TexCoord).rgb;
  	
    // diffuse 
    vec3 norm = normalize(f_Normal);
    vec3 lightDir = normalize(u_Light.position - f_World);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = u_Light.diffuse * diff * texture(u_Material.diffuse, f_TexCoord).rgb;  
    
    // specular
    vec3 viewDir = normalize(u_EyePos - f_World);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    vec3 specular = u_Light.specular * spec * texture(u_Material.specular, f_TexCoord).rgb;  
    
    float dist = length(f_World - u_Light.position);

    vec3 result = attenuation(dist, 0.5, 0.001, 0.00) * (ambient +diffuse + specular);
    
    // Final color
    FragColor = vec4(result, 1.0);
}