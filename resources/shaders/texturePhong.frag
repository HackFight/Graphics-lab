#version 430
out vec4 FragColor;

struct DirLight {
    bool enabled;
    vec3 direction;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    bool enabled;
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

#define NR_POINT_LIGHTS 1

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform sampler2D emission0;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    // properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);

    // phase 2: Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);  

    // phase 3: emission
    result += vec3(texture(emission0, UV));
    
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    if(light.enabled)
    {
        vec3 lightDir = normalize(-light.direction);

        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);

        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);

        // combine results
        vec3 ambient  = light.ambient  * vec3(texture(diffuse0, UV));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse0, UV));
        vec3 specular = light.specular * spec * vec3(texture(specular0, UV));
        return (ambient + diffuse + specular);
    }
    else
    {
        return vec3(0.0, 0.0, 0.0);
    }
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    if(light.enabled)
    {
        vec3 lightDir = normalize(light.position - fragPos);
    
        // diffuse shading
        float diff = max(dot(normal, lightDir), 0.0);
    
        // specular shading
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    
        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
        // combine results
        vec3 ambient  = light.ambient  * vec3(texture(diffuse0, UV));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse0, UV));
        vec3 specular = light.specular * spec * vec3(texture(specular0, UV));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        return (ambient + diffuse + specular);
    }
    else
    {
        return vec3(0.0, 0.0, 0.0);
    }
} 
