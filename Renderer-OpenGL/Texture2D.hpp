#pragma once
#include <cstdint>
#include <string>
#include <Renderer-OpenGL/Image.hpp>

class Texture2D{
public:
    Texture2D(const std::string& p_Filename);
    void Bind();
    void Unbind();
    bool IsTextureLoaded() const { return m_ImageData.IsImageLoaded(); }
private:
    Image m_ImageData;
    unsigned int m_TextureID;
};