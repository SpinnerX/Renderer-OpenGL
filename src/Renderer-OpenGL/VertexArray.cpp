#include <Renderer-OpenGL/VertexArray.hpp>
#include <glad/glad.h>

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

VertexArray::VertexArray(std::span<float> p_Vertices, std::span<uint32_t> p_Indices){
    glGenVertexArrays(1, &m_VertexArrayID);
    Bind();
    //! @note Writing our vertices to our vertex buffer
    m_Vbo = VertexBuffer(p_Vertices);

    this->Bind();
    m_Ibo = IndexBuffer(p_Indices);
    this->Unbind();

    if(p_Vertices.size() == 0 and p_Indices.size() == 0){
        m_DoesHaveVerticesAndIndices = false;
        fmt::print("No vertices added to vertex array!\n");
    }

    m_DoesHaveIndices = true;
}

VertexArray::VertexArray(std::span<float> p_Vertices){
    glGenVertexArrays(1, &m_VertexArrayID);
    m_Vbo = VertexBuffer(p_Vertices);
    m_DoesHaveIndices = false;
}

VertexArray::VertexArray(const VertexBuffer* p_Vbo){
    glGenVertexArrays(1, &m_VertexArrayID);
    Bind();
    
    m_Vbo = *p_Vbo;
    m_DoesHaveIndices = false;
}

VertexArray::VertexArray(const VertexBuffer& p_Vbo, const IndexBuffer& p_Ibo){
    m_Vbo = p_Vbo;

    
    this->Bind();
    m_Ibo = p_Ibo;
    this->Unbind();
}

VertexArray::~VertexArray(){
    Unbind();
}

void VertexArray::AddVertexBuffer(const VertexBuffer& p_Buffer){
    m_Vbo = p_Buffer;
}

void VertexArray::Bind() const {
    glBindVertexArray(m_VertexArrayID);
}

void VertexArray::Unbind() const {
    glBindVertexArray(0);
}

void VertexArray::SetVertexAttribute(const VertexAttributes& p_VertexAttributesLayout){
    // Bind vertex array
    this->Bind();
    m_Vbo.Bind();

    uint32_t m_Index = 0;

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
            fmt::print("\n\n");
            glVertexAttribPointer(m_Index,
                                  RetrieveVertexAttributeSize(vertex_attribute.m_AttributeType),
                                  VertexAttributeTypeToOpenGlBaseTypeConversion(vertex_attribute.m_AttributeType),
                                  vertex_attribute.m_IsNormalized ? GL_TRUE : GL_FALSE,
                                  p_VertexAttributesLayout.GetStride() * sizeof(float),
                                //   (const void*)0
                                  (const void*)(vertex_attribute.m_Offset * sizeof(float))
            );
            glEnableVertexAttribArray(m_Index);
            m_Index++;
        }
        break;
        case VertexAttributeType::FLOAT4:
        {
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
            // glEnableVertexAttribArray(m_Index);
            glVertexAttribIPointer(m_Index,
                RetrieveVertexAttributeSize(vertex_attribute.m_AttributeType),
                                  VertexAttributeTypeToOpenGlBaseTypeConversion(vertex_attribute.m_AttributeType),
                                  p_VertexAttributesLayout.GetStride(),
                                  (const void*)vertex_attribute.m_Offset
            );
            glEnableVertexAttribArray(m_Index);

            m_Index++;
            break;
        }
        default:
            break;
        }
    }
    m_Vbo.Unbind();
    this->Unbind();
}


void VertexArray::WriteData(std::span<float> p_Vertices, std::span<float> p_Indices){
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 *sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}