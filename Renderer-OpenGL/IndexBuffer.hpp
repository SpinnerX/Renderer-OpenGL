#pragma once
#include <span>

class IndexBuffer{
public:
    IndexBuffer() = default;
    IndexBuffer(std::span<uint32_t> p_Indices);

    void Bind();
    void Unbind();

    std::span<uint32_t> GetIndices() const { return m_Indices; }

    void WriteData(std::span<uint32_t> p_Indices);

private:
    std::span<uint32_t> m_Indices;
    uint32_t m_IndexBufferObjectID = -1;
};