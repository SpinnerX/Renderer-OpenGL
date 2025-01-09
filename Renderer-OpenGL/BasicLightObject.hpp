#pragma once
#include <Renderer-OpenGL/Shader.hpp>
#include <Renderer-OpenGL/VertexArray.hpp>
#include <glm/glm.hpp>
#include <span>
#include <Renderer-OpenGL/Camera.hpp>

class BasicLight{
public:
    BasicLight() = default;
    BasicLight(std::span<float> p_Vertices, std::span<uint32_t> p_Indices);
    BasicLight(std::span<float> p_Vertices);

    void SetAmbient(const glm::vec3& p_Val){
        m_Ambient = p_Val;
    }

    void SetSpecular(const glm::vec3& p_Val){
        m_Specular = p_Val;
    }

    void SetDiffuse(const glm::vec3& p_Val){
        m_Diffuse = p_Val;
    }

    void UpdateLocation(Camera& camera);


private:
    Shader m_LightShader;
    glm::vec3 m_Ambient {0.f};
    glm::vec3 m_Specular{0.f};
    glm::vec3 m_Diffuse{0.f};

    VertexBuffer m_Vbo;
    VertexArray m_Vao;
};