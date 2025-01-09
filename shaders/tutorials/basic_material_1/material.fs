#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light{

    vec3 position;


    // strength intensity per light source
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// Usage: Shader::Set("material.ambient", {x, y, z});
uniform Light light;
uniform Material material;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(){
    // ambient
    float ambientStrength = 0.1;
    // vec3 ambient = ambientStrength * lightColor;
    // vec3 ambient = lightColor * material.ambient;
    vec3 ambient = light.ambient * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    // vec3 diffuse = diff * lightColor;
    // vec3 diffuse = lightColor * (diff * material.diffuse);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // vec3 specular = specularStrength * spec * lightColor;  
    // vec3 specular = lightColor * (spec * material.specular);
    vec3 specular = light.specular * (spec * material.specular);
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}