#pragma once
#include <cstdint>
#include <string>

enum class TextureType : uint8_t {
    DIFFUSE, // diffuse texture
    SPECULAR, // specular texture
    NORMAL, // normal texture
    HEIGHT, // height map texture 
    AMBIENT
};

struct TextureSettings {
    uint32_t ID = -1;
    std::string Type="Imported Texture Type as String";
    
};

class Texture{
public:
    Texture(const std::string& p_Filename, TextureType p_Type);

    //! @note 0x84C0 is the value for GL_TEXTURE0 from glad.h
    //! @note I am setting this as a default that can also still be set
    void Bind(int p_TextureIndex=0);
    void Unbind();

    bool IsLoaded() const { return m_IsTextureLoaded; }
    std::string GetPath() const { return m_Path; }
private:
    std::string m_Path;
    uint32_t m_TextureID = -1;
    bool m_IsTextureLoaded = false;
};