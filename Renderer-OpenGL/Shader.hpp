#pragma once
#include <string>
#include <glm/glm.hpp>
#include <unordered_map>

enum class Shader_t{
    VERTEX, FRAGMENT
};

class Shader{
public:
    Shader() = default;
    Shader(const std::string& p_VertexShader, const std::string& p_FragmentShader);
    ~Shader();

    void Bind();
    void Bind(uint32_t p_Index);
    void Unbind(uint32_t p_Index);
    void Unbind();
    bool IsLoaded() const;

    void Set(const std::string& name, const int& p_IntValue);
    void Set(const std::string& name, const float& p_FloatValue);

    void Set(const std::string& name, const glm::vec2& p_Values);
    void Set(const std::string& name, const glm::vec3& p_Values);
    void Set(const std::string& name, const glm::vec4& p_Values);
    void Set(const std::string& name, const glm::mat2& p_Values);
    void Set(const std::string& name, const glm::mat3& p_Values);
    void Set(const std::string& name, const glm::mat4& p_Values);

    std::string GetShaderName() const;
private:

    bool LoadShaderModule(const std::string& p_Filename, Shader_t ShaderType);
    
    int GetLocation(const std::string& p_Name);

private:
    std::string m_Filepath;
    bool m_IsShaderLoaded=false;

    // ID for vertex shader
    uint32_t m_VertShaderID = -1;

    // ID for fragment shader
    uint32_t m_FragShaderID = -1;

    uint32_t m_ShaderProgramID = -1;
};

//! @note Used as our lookup for all our loaded shaders the renderer will utilize
class ShaderLibrary{
public:
    ShaderLibrary();
    //! @note Add shader to the library
    //! @note Add shader and a custom shader name to the associated shader
    void Add(const Shader& p_Shader);
    void Add(const std::string& p_Filename, const Shader& p_Shader);

    Shader& Get(const std::string& p_ShaderTag);
    //! @note Enables to load the shader and use it immediately, that automatically gets added to lookup.
    Shader Load(const std::string& p_VertShader, const std::string& p_FragShader);

private:
    static ShaderLibrary* s_CurrentShaderLibraryInstance;
    std::unordered_map<std::string, Shader> m_ShaderLibs;
};