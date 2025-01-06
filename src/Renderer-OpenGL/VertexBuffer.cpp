#include <Renderer-OpenGL/VertexBuffer.hpp>
#include <glad/glad.h>

VertexBuffer::VertexBuffer(std::span<float> p_Vertices){
    // glGenBuffers(1, &m_VboID);
    // glCreateBuffers(1, &m_VboID);
    glGenBuffers(1, &m_VboID);
    this->WriteData(p_Vertices);
}

void VertexBuffer::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
}


void VertexBuffer::WriteData(std::span<float> p_Vertices){
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, p_Vertices.size(), p_Vertices.data(), GL_STATIC_DRAW);
}