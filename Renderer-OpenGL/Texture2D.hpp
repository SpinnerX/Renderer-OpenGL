#pragma once
#include <cstdint>
#include <string>
#include <Renderer-OpenGL/Image.hpp>

class Texture2D{
public:
    Texture2D(const std::string& p_Filename);
    //! @note 0x84C0 is the value for GL_TEXTURE0 from glad.h
    //! @note I am setting this as a default that can also still be set
    void Bind(int p_TextureIndex=0x84C0);
    void Unbind();
    bool IsTextureLoaded() const { return m_ImageData.IsImageLoaded(); }
private:
    Image m_ImageData;
    unsigned int m_TextureID;
};