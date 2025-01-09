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

struct Light{
    vec3 position;
    // strength intensity per light source
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3 direction;
    float cut_off;
    float outer_cut_off;

    // strength intensity per light source
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // reflection over distance
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Usage: Shader::Set("material.ambient", {x, y, z});
uniform vec3 viewPos;
uniform Material material;
// uniform Light light;
// for the chapter light casters, directional lighting is what will be used with the examples
uniform SpotLight light;

void main(){
    // ambient
    // vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(light.position - FragPos);
    // vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position-FragPos);
    
    // for smoother intensity
    float epsilon = light.cut_off - light.outer_cut_off;
    float theta = dot(lightDir, normalize(-light.direction));
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);
    
    if(theta > light.cut_off){
        // do light calculation
        // ambient
        vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

        // diffuse
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

        // specular
        vec3 viewDir = normalize(viewPos-FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

        diffuse *= intensity;
        specular *= intensity;

        // attenuation
        float dist = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

        vec3 result = ambient + diffuse + specular;
        FragColor = vec4(result, 1.0);
    }
    else{
        FragColor = vec4(light.ambient * texture(material.diffuse, TexCoords).rgb, 1.0);
    }
}