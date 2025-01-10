#pragma once
#include <glm/glm.hpp>
#include <span>
#include <string>

class PointLight{
public:
    PointLight(std::span<float> vertices);

private:
    std::string m_PointLightTag="Undefined";
    glm::mat4 m_Model={1.f};
};