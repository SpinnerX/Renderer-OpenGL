#pragma once
#include <cstdint>
#include <span>
#include <vector>

/**
    @name VertexBuffer
    @note Buffer that contains all our vertices
*/
class VertexBuffer{
public:
    VertexBuffer(std::span<float> p_Vertices);

    void Bind();


    void WriteData(std::span<float> p_Vertices);

private:
    std::vector<float> m_Vertices;
    uint32_t m_VboID = -1;
};