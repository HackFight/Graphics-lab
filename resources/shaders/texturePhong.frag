#version 430

out vec4 FragColor;

in vec2 UV;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

//Diffuse texture unit = 0
//Specular texture unit = 1
//Emission texture unit = 2

uniform	sampler2D colorTexUnit;
uniform	sampler2D specularTexUnit;
uniform sampler2D emissionTexUnit;
float shininess = 32.0f;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;  

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    
    vec3 ambient  = light.ambient  * vec3(texture(colorTexUnit, UV));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(colorTexUnit, UV));
    vec3 specular = light.specular * spec * vec3(texture(specularTexUnit, UV));
    vec3 emission = texture(emissionTexUnit, UV).rgb;
    
    //result
    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}