#pragma once
#include <span>

class IndexBuffer{
public:
    IndexBuffer() = default;
    IndexBuffer(std::span<uint32_t> p_Indices);

    void Bind();
    void Unbind();

    void WriteData(std::span<uint32_t> p_Indices);

private:
    uint32_t m_IndexBufferObjectID = -1;
};