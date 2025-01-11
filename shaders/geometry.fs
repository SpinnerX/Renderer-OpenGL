#version 410 core

layout(location = 0) out vec4 color;
layout(location = 1) out int color2;

in vec3 Position;
in vec4 Color;
// in float TexIndex;
in vec2 TexCoords;

// uniform sampler2D Textures[16];

uniform sampler2D Texture;

void main(){
    color = texture(Texture, TexCoords) * Color;
}