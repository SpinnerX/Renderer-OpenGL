#pragma once
#include "IndexBuffer.hpp"
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


private:

};