#version 430 core
#pragma stage : vert

out vec4 FragColor;

// in vec3 TexCoords;
in vec3 Normal;
in vec3 Position;
in vec3 LocalPosition;

uniform vec3 cameraPos;
uniform samplerCube skybox;

uniform sampler2D equirectangularMap;


const vec2 inverseAtan = vec2(0.1591, 0.3183);

vec2 SampleSphericMap(vec3 v){
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= inverseAtan;
    uv += 0.5;
    return uv;
}



void main(){
    // FragColor = texture(skybox, TexCoords);
    vec2 uv = SampleSphericMap(normalize(LocalPosition));
    vec3 I = normalize(Position - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    // FragColor = vec4(texture(skybox, R).rgb, 1.0);
    FragColor = vec4(texture(equirectangularMap, uv).rgb, 1.0);
}