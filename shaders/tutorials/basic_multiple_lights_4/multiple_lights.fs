#version 330 core
out vec4 FragColor;

struct Material{
    // vec3 ambient;
    // vec3 diffuse;
    sampler2D diffuse;
    sampler2D specular;
    // vec3 specular;
    float shininess;
};

struct DirectionalLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cut_off;
    float outer_cut_off;
    
    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

const int MAX_POINT_LIGHTS = 4;
uniform PointLight point_lights[MAX_POINT_LIGHTS];

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Updated to use variable for shadow-mapping
in vec4 FragPosLightSpace;

// Usage: Shader::Set("material.ambient", {x, y, z});
uniform vec3 viewPos;
uniform Material material;
uniform sampler2D shadowMap; // ADDED: for handling shadow mapping
uniform mat4 lightSpaceMatrix; // ADDED: for shadow passes

// uniform Light light;
// for the chapter light casters, directional lighting is what will be used with the examples
// uniform SpotLight light;
uniform DirectionalLight dir_light;

uniform SpotLight spot_light;

float ShadowMappingCaluclation(vec4 fragPosLightSpace){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closeDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    // Handling shadow biases
    float bias = 0.005;
    float shadow = (currentDepth - bias) > closeDepth ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));
    
    // combine results
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    // Add shadows to the lighting..
    // float shadow = ShadowMappingCaluclation(FragPosLightSpace);
    // return (ambient + (1.0 - shadow) + (diffuse + specular));
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(normal, FragPos), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    float dist = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
    

    // getting actual results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    return (ambient + diffuse + specular);
}

void main(){
    float gamma = 2.2;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dir_light, norm, viewDir);

    for(int i = 0; i < 4; i++){
        result += CalcPointLight(point_lights[i], norm, FragPos, viewDir);
    }

    result += CalcSpotLight(spot_light, norm, FragPos, viewDir);

    FragColor = vec4(result, 1.0);
}