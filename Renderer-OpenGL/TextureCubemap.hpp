#pragma once
#include <span>
#include <string>

class TextureCubemap{
public:
    TextureCubemap(std::span<std::string>);
    void Bind();
    void Unbind();
private:
    uint32_t m_TextureCubemapID = -1;
};