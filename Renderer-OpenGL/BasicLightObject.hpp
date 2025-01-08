#pragma once
#include "Shader.hpp"
#include <Renderer-OpenGL/VertexArray.hpp>
#include <glm/glm.hpp>
#include <span>
#include <Renderer-OpenGL/Camera.hpp>

class BasicLight{
public:
    BasicLight() = default;
    BasicLight(std::span<float> p_Vertices, std::span<uint32_t> p_Indices);

    void UpdateLocation(Camera& camera);

    glm::mat4 GetView() { return m_View; } 
    glm::mat4 GetModel() { return m_Projection; }
    glm::mat4 GetProjection() { return m_Model; }

    glm::vec3 GetPosition() { return m_LightPosition; }

private:
    Shader m_LightShader;
    VertexArray m_Vao;
    VertexArray m_CubeVao;
    glm::vec3 m_LightPosition = {1.2f, 1.0f, 2.0f};
    glm::mat4 m_View;
    glm::mat4 m_Projection;
    glm::mat4 m_Model;
};