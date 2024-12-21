#version 430
out vec4 FragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

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
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 4

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;


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
    result += vec3(texture(material.emission, UV));
    
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
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

        // combine results
        vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, UV));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, UV));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));
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
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    
        // attenuation
        float distance    = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    
        // combine results
        vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, UV));
        vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, UV));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, UV));
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
