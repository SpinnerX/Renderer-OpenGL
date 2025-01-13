#pragma once
#include <glm/glm.hpp>

struct PointLight{
    glm::vec3 Position;
    
    float Constant;
    float Linear;
    float Quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirectionLight{
    glm::vec3 Direction;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct SpotLight{
    glm::vec3 Position;
    glm::vec3 Direction;
    float cut_off;
    float outer_cut_off;

    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};