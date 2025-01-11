#version 410 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aNormal;
// layout(location = 3) in float aTexIndex;
// layout(location = 4) in float aTilingFactor;
// layout(location = 5) in float aEntityID;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

out vec3 Position;
out vec4 Color;
out vec2 TexCoords;
out vec3 Normal;

void main(){
    Position = aPosition;
    Color = aColor;
    TexCoords = aTexCoords;
    Normal = aNormal;

    gl_Position = model * view * projection * vec4(aPosition, 1.0);
}