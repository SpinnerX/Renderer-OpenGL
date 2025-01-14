#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

out vec3 Position;
out vec3 TexCoords;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    // Position = aPosition;
    // Position = vec3(model * vec4(aPosition, 1.0));
    // Normal = aNormal;
    // Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aPosition;
    vec4 pos = projection * view * model * vec4(aPosition, 1.0);
    gl_Position = pos.xyww;
    // gl_Position = projection * view * model * vec4(aPosition, 1.0);
}