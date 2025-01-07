#pragma once
#include <string>
#include <glm/glm.hpp>

enum class Shader_t{
    VERTEX, FRAGMENT
};

class Shader{
public:
    Shader(const std::string& p_VertexShader, const std::string& p_FragmentShader);
    ~Shader();

    void Bind();
    void Unbind();
    bool IsLoaded() const;

    void Set(const std::string& name, int p_IntValue);
    void Set(const std::string& name, float p_IntValue);

    void Set(const std::string& name, const glm::vec2& p_Values);
    void Set(const std::string& name, const glm::vec3& p_Values);
    void Set(const std::string& name, const glm::vec4& p_Values);
    void Set(const std::string& name, const glm::mat2& p_Values);
    void Set(const std::string& name, const glm::mat3& p_Values);
    void Set(const std::string& name, const glm::mat4& p_Values);

private:
    std::string IsFileLoaded(const std::string& p_Filename);

    // bool LoadShaderModule(const std::string& p_Filename);
    bool LoadShaderModule(const std::string& p_Filename, Shader_t ShaderType);
    
    int GetLocation(const std::string& p_Name);

private:
    bool m_IsShaderLoaded=false;
    std::string m_Filename;

    // ID for vertex shader
    uint32_t m_VertShaderID = -1;

    // ID for fragment shader
    uint32_t m_FragShaderID = -1;

    uint32_t m_ShaderProgramID = -1;
};