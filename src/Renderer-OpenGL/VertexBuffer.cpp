#include <Renderer-OpenGL/VertexBuffer.hpp>
#include <glad/glad.h>
#include <fmt/core.h>

static GLenum VertexAttributeTypeToOpenGlBaseTypeConversion(VertexAttributeType type){
    switch (type){
        case VertexAttributeType::NONE: return 0;
        case VertexAttributeType::FLOAT:  return GL_FLOAT;
        case VertexAttributeType::FLOAT2: return GL_FLOAT;
        case VertexAttributeType::FLOAT3: return GL_FLOAT;
        case VertexAttributeType::FLOAT4: return GL_FLOAT;
        case VertexAttributeType::MAT3:   return GL_FLOAT;
        case VertexAttributeType::MAT4:   return GL_FLOAT;
        case VertexAttributeType::INT:    return GL_INT;
        case VertexAttributeType::INT2:   return GL_INT;
        case VertexAttributeType::INT3:   return GL_INT;
        case VertexAttributeType::INT4:   return GL_INT;
        case VertexAttributeType::BOOL:   return GL_BOOL;
    }

    return 0;
}

static std::string ShaderDataTypeToString(VertexAttributeType type){
    switch (type){
        case VertexAttributeType::NONE: return "VertexAttributeType::NONE";
        case VertexAttributeType::FLOAT:  return "VertexAttributeType::FLOAT";
        case VertexAttributeType::FLOAT2: return "VertexAttributeType::FLOAT2";
        case VertexAttributeType::FLOAT3: return "VertexAttributeType::FLOAT3";
        case VertexAttributeType::FLOAT4: return "VertexAttributeType::FLOAT4";
        case VertexAttributeType::MAT3:   return "VertexAttributeType::MAT3";
        case VertexAttributeType::MAT4:   return "VertexAttributeType::MAT4";
        case VertexAttributeType::INT:    return "VertexAttributeType::INT";
        case VertexAttributeType::INT2:   return "VertexAttributeType::INT2";
        case VertexAttributeType::INT3:   return "VertexAttributeType::INT3";
        case VertexAttributeType::INT4:   return "VertexAttributeType::INT4";
        case VertexAttributeType::BOOL:   return "VertexAttributeType::BOOL";
    }

    return "Default: meaning no type was given!";
}

VertexBuffer::VertexBuffer(std::span<float> p_Vertices){
    glGenBuffers(1, &m_VboID);

    if(p_Vertices.size() != 0){
        m_Vertices = p_Vertices;
        this->WriteData(p_Vertices);
    }
}

VertexBuffer::VertexBuffer(std::span<Vertex> p_Vertices){
    glGenBuffers(1, &m_VboID);

    if(p_Vertices.size() != 0){
        this->WriteData(p_Vertices);
    }
}

VertexBuffer::~VertexBuffer(){
    this->Unbind();
}

void VertexBuffer::Bind(){
    glBindBuffer(GL_ARRAY_BUFFER, m_VboID);
}

void VertexBuffer::Unbind(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetVertexAttributes(const VertexAttributes& p_VertexAttributesLayout){
    this->Bind();
    m_Index = 0;

    const auto& vert_attributes_data = p_VertexAttributesLayout;

    for(const auto& vertex_attribute : vert_attributes_data.GetElements()){
        switch (vertex_attribute.m_AttributeType){
        case VertexAttributeType::NONE:  
        case VertexAttributeType::FLOAT: 
        case VertexAttributeType::FLOAT2:
        case VertexAttributeType::FLOAT3:
        {
            // fmt::print("FLOAT3 Executed Case Switch!\n");
            fmt::print("Index = {}\n", m_Index);
            fmt::print("Attribute Size = {}\n", RetrieveVertexAttributeSize(vertex_attribute.m_AttributeType));
            fmt::print("Type = {}\n", ShaderDataTypeToString(vertex_attribute.m_AttributeType));
            fmt::print("Stride = {}\n", p_VertexAttributesLayout.GetStride());
            fmt::print("IsNormalized = {}\n", (vertex_attribute.m_IsNormalized ? "GL_TRUE" : "GL_FALSE"));
            fmt::print("offset = {}\n", vertex_attribute.m_Offset);
            glVertexAttribPointer(m_Index,
                                  RetrieveVertexAttributeSize(vertex_attribute.m_AttributeType),
                                  VertexAttributeTypeToOpenGlBaseTypeConversion(vertex_attribute.m_AttributeType),
                                  vertex_attribute.m_IsNormalized ? GL_TRUE : GL_FALSE,
                                  p_VertexAttributesLayout.GetStride() * sizeof(float),
                                  (const void*)0
                                //   (const void*)vertex_attribute.m_Offset
            );
            glEnableVertexAttribArray(m_Index);
            m_Index++;
        }
        break;
        case VertexAttributeType::FLOAT4:
        {
            fmt::print("FLOAT4 Executed Case Switch!\n");
            glEnableVertexAttribArray(m_Index);
            glVertexAttribPointer(m_Index,
                                  RetrieveVertexAttributeSize(vertex_attribute.m_AttributeType),
                                  VertexAttributeTypeToOpenGlBaseTypeConversion(vertex_attribute.m_AttributeType),
                                  vertex_attribute.m_IsNormalized ? GL_TRUE : GL_FALSE,
                                  p_VertexAttributesLayout.GetStride(),
                                  (const void*)vertex_attribute.m_Offset
            );
            m_Index++;
        }
        break;
        case VertexAttributeType::MAT3:  
        case VertexAttributeType::MAT4:  
        case VertexAttributeType::INT:   
        case VertexAttributeType::INT2:  
        case VertexAttributeType::INT3:  
        case VertexAttributeType::INT4:  
        case VertexAttributeType::BOOL:
        {
            glEnableVertexAttribArray(m_Index);
            glVertexAttribIPointer(m_Index,
                RetrieveVertexAttributeSize(vertex_attribute.m_AttributeType),
                                  VertexAttributeTypeToOpenGlBaseTypeConversion(vertex_attribute.m_AttributeType),
                                  p_VertexAttributesLayout.GetStride(),
                                  (const void*)vertex_attribute.m_Offset
            );

            m_Index++;
            break;
        }
        default:
            break;
        }
    }
    this->Unbind();
}

void VertexBuffer::WriteData(std::span<float> p_Vertices){

    this->Bind();
    fmt::print("VertexBuffer::WriteData::sizeof(vertices) = {}\n", p_Vertices.size_bytes());
    glBufferData(GL_ARRAY_BUFFER, p_Vertices.size_bytes(), p_Vertices.data(), GL_STATIC_DRAW);
}

void VertexBuffer::WriteData(std::span<Vertex> p_Vertices){
    this->Bind();
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), p_Vertices.data(), GL_STATIC_DRAW);
}