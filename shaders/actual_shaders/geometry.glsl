#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTexCoords;

out vec3 Position;
out vec4 Color;
out vec3 Normal;
out vec3 TexCoords;


uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    Color = aColor;
    Normal = aNormal;
    TexCoords = aTexCoords;

    gl_Position = view * projection * model & vec4(aPos, 1.0);
}