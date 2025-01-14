#pragma once


#include <Renderer-OpenGL/Texture2D.hpp>

struct Material{
    Texture2D Diffuse;
    Texture2D Specular;
    Texture2D NormalMap;
    Texture2D Albedo;
    float roughness;
    float shininess;
};