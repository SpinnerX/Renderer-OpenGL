#pragma once
#include <cstdint>
#include <span>
#include <Renderer-OpenGL/IndexBuffer.hpp>
#include <Renderer-OpenGL/VertexBuffer.hpp>

class VertexArray{
public:
    VertexArray() = default;
    VertexArray(std::span<float> p_Vertices, std::span<uint32_t> p_Indices);
    VertexArray(std::span<float> p_Vertices);
    VertexArray(const VertexBuffer* p_Vbo);
    VertexArray(const VertexBuffer& vbo, const IndexBuffer& ibo);
    ~VertexArray();

    void Bind() const;

    void Unbind() const;

    uint32_t GetVaoID() const { return m_VertexArrayID; }
    uint32_t GetCurrentVBOID() const { return m_Vbo.GetID(); }

    void AddVertexBuffer(const VertexBuffer& p_Buffer);

    void SetVertexAttribute(const VertexAttributes& p_VertexAttributesLayout);

    void WriteData(std::span<float> p_Vertices, std::span<float> p_Indices);

    bool HasIndices() const { return m_DoesHaveIndices; }

    VertexBuffer GetCurrentVbo() const { return m_Vbo; }
    IndexBuffer GetCurrentIbo() const { return m_Ibo; }

private:
    bool m_DoesHaveVerticesAndIndices = true;
    bool m_DoesHaveIndices = false;
    VertexBuffer m_Vbo;
    IndexBuffer m_Ibo;
    uint32_t m_VertexArrayID = -1;
};