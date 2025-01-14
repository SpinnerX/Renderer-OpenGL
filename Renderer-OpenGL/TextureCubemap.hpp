#pragma once
#include <span>
#include <string>

enum EnvironmentMapType{
    CUBEMAP,
    HDRI
};

class TextureCubemap{
public:
    TextureCubemap() = default;
    TextureCubemap(std::span<std::string>);
    // TextureCubemap(const std::string& p_Filepath, EnvironmentMapType Type);
    TextureCubemap(uint32_t Width, uint32_t Height, uint32_t p_FaceSize);
    
    void Bind();
    void Unbind();
private:
    uint32_t m_TextureCubemapID = -1;
};