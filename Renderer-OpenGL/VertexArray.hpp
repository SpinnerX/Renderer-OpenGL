#pragma once
#include <cstdint>
#include <span>
#include <Renderer-OpenGL/IndexBuffer.hpp>
#include <Renderer-OpenGL/VertexBuffer.hpp>

class VertexArray{
public:
    VertexArray() = default;
    VertexArray(std::span<float> p_Vertices, std::span<uint32_t> p_Indices);
    VertexArray(const VertexBuffer& vbo, const IndexBuffer& ibo);
    ~VertexArray();

    void Bind();

    void Unbind();

    void AddVertexBuffer(const VertexBuffer& p_Buffer);

    void SetVertexAttribute(const VertexAttributes& p_VertexAttributesLayout);

    void WriteData(std::span<float> p_Vertices, std::span<float> p_Indices);

    bool HasIndices() { return m_DoesHaveIndices; }

private:
    bool m_DoesHaveIndices = false;
    VertexBuffer m_Vbo;
    IndexBuffer m_Ibo;
    uint32_t m_VertexArrayID = -1;
};