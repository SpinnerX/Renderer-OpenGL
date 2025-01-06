#pragma once
#include <string>

class Shader{
public:
    Shader(const std::string& p_VertexShader, const std::string& p_FragmentShader);

    bool IsLoaded() const;

private:
    bool m_IsShaderLoaded=false;
    std::string m_Filename;

    // ID for vertex shader
    uint32_t m_VertShaderID = -1;

    // ID for fragment shader
    uint32_t m_FragShaderID = -1;
};