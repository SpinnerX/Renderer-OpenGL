#pragma once
#include <cstdint>
#include <string>

//! @note Specifying what textures to load the texture 2d in.
enum class TextureSpecifications{
    DEFAULT,
    DEPTH_MAP,
    NORMAL_MAP,
    GAMMA_CORRECTION
};

class Texture2D{
public:
    Texture2D() = default;
    Texture2D(const std::string& p_Filename, bool gammaCorrectionEnabled=false);
    Texture2D(const std::string& p_Filename, TextureSpecifications property_on_load);
    
    Texture2D(uint32_t Width, uint32_t Height, TextureSpecifications property_on_load);

    Texture2D(uint32_t Width, uint32_t Height, bool attachBothDepthAndStencile=false);
    //! @note 0x84C0 is the value for GL_TEXTURE0 from glad.h
    //! @note I am setting this as a default that can also still be set
    void Bind(int p_TextureIndex=0x84C0);
    void Unbind();

    uint32_t GetID() const { return m_TextureID; }

private:
    //! @note OnReload
    void OnCreate(const std::string& p_Filename, TextureSpecifications property_on_load);

private:
    uint32_t m_TextureID;
};