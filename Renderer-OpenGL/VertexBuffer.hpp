#pragma once
#include <cstdint>
#include <fmt/core.h>
#include <span>
#include <vector>
#include <string>
#include <glm/glm.hpp>

//! @note Specifying vertex attribute

enum class VertexAttributeType : uint8_t {
    NONE = 0,
    FLOAT, FLOAT2, FLOAT3, FLOAT4,
    MAT3, MAT4,
    INT, INT2, INT3, INT4,
    UINT, UINT32, UINT64,
    BOOL
};

static uint32_t VertexAttributeByteSize(VertexAttributeType Type){
    switch (Type){
    case VertexAttributeType::NONE:         return 0;
    case VertexAttributeType::FLOAT:        return 4;
    case VertexAttributeType::FLOAT2:       return 4 * 2;
    case VertexAttributeType::FLOAT3:       return 4 * 3;
    case VertexAttributeType::FLOAT4:       return 4 * 4;
    case VertexAttributeType::MAT3:         return 4 * 3 * 3;
    case VertexAttributeType::MAT4:         return 4 * 4 * 4;
    case VertexAttributeType::INT:          return 4;
    case VertexAttributeType::INT2:         return 4 * 2;
    case VertexAttributeType::INT3:         return 4 * 3;
    case VertexAttributeType::UINT:         return 4;
    case VertexAttributeType::UINT32:       return 4;
    case VertexAttributeType::UINT64:       return 4;
    case VertexAttributeType::BOOL:         return 1;
    default:                                return 0;
    }
}

static uint32_t RetrieveVertexAttributeSize(VertexAttributeType Type){
    switch (Type){
    case VertexAttributeType::NONE:         return 0;
    case VertexAttributeType::FLOAT:        return 1;
    case VertexAttributeType::FLOAT2:       return 2;
    case VertexAttributeType::FLOAT3:       return 3;
    case VertexAttributeType::FLOAT4:       return 4;
    case VertexAttributeType::MAT3:         return 3 * 3;
    case VertexAttributeType::MAT4:         return 4 * 4;
    case VertexAttributeType::INT:          return 1;
    case VertexAttributeType::INT2:         return 2;
    case VertexAttributeType::INT3:         return 3;
    case VertexAttributeType::INT4:         return 4;
    case VertexAttributeType::BOOL:         return 1;
    default:                                return 0;
    }
}

struct VertexAttributeElement{
    VertexAttributeElement() = default;
    VertexAttributeElement(VertexAttributeType AttributeType, const std::string& Name, bool p_IsNormalized = false) : m_Name(Name), m_AttributeType(AttributeType), m_Offset(0), m_IsNormalized(p_IsNormalized), m_Size(RetrieveVertexAttributeSize(AttributeType)){}
    
    /**
     * @note Does:
     * @note Because the learnopengl uses offsetof(Vertex, Normal) and offsetof(Vertex, TexCoords) sets the offset
     * {"ourColor", FLOAT2, false sizeof(Vertex), offsetof(Vertex, Normal)};
    */
    // VertexAttributeElement(const std::string& Name, VertexAttributeType AttributeType, bool doTakeCustomType=false, size_t p_CustomSizeInBytes, bool p_IsNormalized = false) :
    //                         m_Name(Name),
    //                         m_AttributeType(AttributeType),
    //                         m_Offset(0),
    //                         m_IsNormalized(p_IsNormalized),
    //                         m_Size(RetrieveVertexAttributeSize(AttributeType)){}


    std::string m_Name;
    VertexAttributeType m_AttributeType;
    uint32_t m_Offset;
    uint32_t m_Size; // size of the buffer
    bool m_IsNormalized = false;
};

class VertexAttributes{
public:
    VertexAttributes() = default;
    VertexAttributes(const std::initializer_list<VertexAttributeElement>& p_List) : m_Elements(p_List){
        CalculateStride();
    }

    uint32_t GetStride() const{
        return m_Stride;
    }

    std::vector<VertexAttributeElement>::iterator begin(){
        return m_Elements.begin();
    }

    std::vector<VertexAttributeElement>::iterator end(){
        return m_Elements.end();
    }

    std::vector<VertexAttributeElement>::const_iterator begin() const{
        return m_Elements.begin();
    }

    std::vector<VertexAttributeElement>::const_iterator end() const{
        return m_Elements.end();
    }


    std::vector<VertexAttributeElement> GetElements() const{
        return m_Elements;
    }


private:
    void CalculateStride(){
        uint32_t offset = 0;
        m_Stride = 0;

        for(auto& element : m_Elements){
            element.m_Offset = offset;
            offset += element.m_Size;
            m_Stride += element.m_Size;
            fmt::print("offset = {}", offset);
            fmt::print("Elements.Size = {}\n", element.m_Size);
        }
    }
    

private:
    std::vector<VertexAttributeElement> m_Elements;
    uint32_t m_Stride = 0;
};

struct Vertex{
    static constexpr uint32_t MAX_BONE_INFLUENCE = 4;
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

/**
    @name VertexBuffer
    @note Buffer that contains all our vertices
*/
class VertexBuffer{
public:
    VertexBuffer() = default;
    VertexBuffer(std::span<float> p_Vertices);
    VertexBuffer(std::span<Vertex> p_Vertices);
    ~VertexBuffer();

    void Bind();
    void Unbind();


    void SetVertexAttributes(const VertexAttributes& p_VertexAttributesLayout);


    void WriteData(std::span<float> p_Vertices);
    void WriteData(std::span<Vertex> p_Vertices);

private:
    std::vector<float> m_Vertices;
    uint32_t m_VboID = -1;

    uint32_t m_Index = -1;
};