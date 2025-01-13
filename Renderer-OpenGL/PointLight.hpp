#pragma once
#include "Camera.hpp"
#include "VertexArray.hpp"
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <span>
#include <string>

class PointLight{
public:
    PointLight(const std::string& Tag);
    PointLight(std::span<float> vertices, std::span<uint32_t> indices);

    void SetPosition(const glm::vec3& Position);
    void SetRotation(const glm::vec3& Rotation);
    void SetScale(const glm::vec3& Scale);
    // void SetColor(const glm::vec4& Color);

    VertexArray& GetVao() { return m_MeshVao; }

    void OnUpdate(Camera& camera, glm::mat4& projection);

private:
    std::string m_PointLightTag="Undefined";
    glm::vec3 m_Position={0.f, 0.f, 0.f};
    glm::vec3 m_Rotation={0.f, 0.f, 0.f};
    glm::vec3 m_Scale={0.2f, 0.2f, 0.2f};
    glm::mat4 m_Model={1.f};

    //! @note Think of vao as the container of mesh properties
    //! @note Mesh properties that we attach our shader to for specifically point lights.
    VertexArray m_MeshVao;
};