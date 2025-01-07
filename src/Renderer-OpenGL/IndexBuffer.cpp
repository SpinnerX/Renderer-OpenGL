#include <Renderer-OpenGL/IndexBuffer.hpp>
#include <glad/glad.h>
#include <fmt/core.h>

IndexBuffer::IndexBuffer(std::span<uint32_t> p_Indices){
    glGenBuffers(1, &m_IndexBufferObjectID);
    WriteData(p_Indices);
}

void IndexBuffer::Bind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferObjectID);
}

void IndexBuffer::Unbind(){
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::WriteData(std::span<uint32_t> p_Indices){
    this->Bind();
    // fmt::print("span<uint32_t>::size = {}\n", p_Indices.size());
    // fmt::print("span<uint32_t>::size / sizeof(uint32_t) = {}\n", p_Indices.size()/sizeof(uint32_t));
    // fmt::print("span<uint32_t>::size_bytes = {}\n", p_Indices.size_bytes());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_Indices.size_bytes(), p_Indices.data(), GL_STATIC_DRAW);
}