#version 440 core

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec4 lightVector;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform Light light;
uniform Material material;

uniform vec3 viewPos;

void main()
{
    vec3 lightDir;
    // Check the w conponent to define a directionial light or point light
    if (light.lightVector.w == 0.0) // Directional light
        lightDir = normalize(-vec3(light.lightVector));
    else if (light.lightVector.w == 1.0) // Point light
        lightDir = normalize(vec3(light.lightVector) - FragPos);
    
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    float intensity, lambertian, spec;
    vec3 ambient, diffuse, specular;

    vec3 result;

    // Ambient
    ambient = material.ambient * light.ambient;

    // Diffuse
    lambertian = max(dot(norm, lightDir), 0);
    diffuse = material.diffuse * lambertian * light.diffuse;

    // Specular
    vec3 halfDir = normalize(lightDir + viewDir);

    spec = pow(max(dot(halfDir, norm), 0), material.shininess);
    specular = material.specular * spec * light.specular;

    result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}