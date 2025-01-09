#include "VertexBuffer.hpp"
#include <Renderer-OpenGL/BasicLightObject.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <span>


BasicLight::BasicLight(std::span<float> p_Vertices, std::span<uint32_t> p_Indices){

}

BasicLight::BasicLight(std::span<float> p_Vertices){
    m_Vbo = VertexBuffer(p_Vertices);
    m_Vao = VertexArray(&m_Vbo);

    // m_LightShader = Shader();
}

void BasicLight::UpdateLocation(Camera& camera){
}