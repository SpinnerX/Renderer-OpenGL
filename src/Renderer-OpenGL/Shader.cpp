#include <Renderer-OpenGL/Shader.hpp>
#include <glad/glad.h>
#include <fmt/format.h>
#include <array>
#include <fstream>

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/quaternion.hpp>
// #include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
using namespace fmt;

ShaderLibrary* ShaderLibrary::s_CurrentShaderLibraryInstance = nullptr;

static int RetrieveShaderType(Shader_t ShaderStageType){
    if(ShaderStageType == Shader_t::VERTEX) return GL_VERTEX_SHADER;
    else if(ShaderStageType == Shader_t::FRAGMENT) return GL_FRAGMENT_SHADER;
    return 0;
}

static std::string ShaderTypeToString(Shader_t ShaderStage){
    if(ShaderStage == Shader_t::VERTEX) return "Vertex";
    else if(ShaderStage == Shader_t::FRAGMENT) return "Fragment";
    
    return "Undefined Shader!";
}


Shader::Shader(const std::string& p_VertexShader, const std::string& p_FragmentShader) : m_IsShaderLoaded(true), m_Filepath(p_VertexShader){
    //! @note We initialize our shader program to then take in our shader modules and used for linking them
    m_ShaderProgramID = glCreateProgram();

    // Load, Build, Compile our vertex and fragment shader modules
    int success;
    bool is_vert_loaded = LoadShaderModule(p_VertexShader, Shader_t::VERTEX);

    if(!is_vert_loaded){
        fmt::print("Vertex Shader DID NOT LOAD CORRECTLY!\n");
        fmt::print("Could not load filepath = {}\n", p_VertexShader);
        return;
    }

    bool is_frag_loaded = LoadShaderModule(p_FragmentShader, Shader_t::FRAGMENT);
    
    if(!is_frag_loaded){
        fmt::print("FRAG SHADER did not load correctly!\n");
        fmt::print("Could not load filepath = {}\n", p_FragmentShader);
        return;
    }

    glLinkProgram(m_ShaderProgramID);

    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
    if(!success){
        char infoLog[512];
        glGetProgramInfoLog(m_ShaderProgramID, 512, nullptr, infoLog);
        fmt::print("Could NOT load shader program!\n");
        fmt::print("{}\n", infoLog);
        return;
    }


}

Shader::~Shader(){
    glUseProgram(0);
}

bool Shader::LoadShaderModule(const std::string& p_Filename, Shader_t ShaderType){
    std::ifstream ins(p_Filename);
    int success;
    std::array<char, 512> infoLog;

    if(!ins){
        return false;
    }

    std::stringstream ss;
    ss << ins.rdbuf();
    // This will contain the actual shader literal src
    // const char* shader_src = ss.str().c_str();
    std::string src = ss.str();
    const char* shader_src = src.c_str();

    m_VertShaderID = glCreateShader(RetrieveShaderType(ShaderType));
    glShaderSource(m_VertShaderID, 1, &shader_src, nullptr);
    glCompileShader(m_VertShaderID);
    glGetShaderiv(m_VertShaderID, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(m_VertShaderID, infoLog.size(), nullptr, infoLog.data());
        fmt::print("{} SHADER ERROR\n", ShaderTypeToString(ShaderType));
        fmt::print("{}\n", infoLog.data());
        return false;
    }

    glAttachShader(m_ShaderProgramID, m_VertShaderID);
    return true;
}

void Shader::Bind(){
    glUseProgram(m_ShaderProgramID);
}

void Shader::Unbind(){
    glUseProgram(0);
}

void Shader::Bind(uint32_t p_Index){
    glUseProgram(p_Index);
}

void Shader::Unbind(uint32_t p_Index){
    glUseProgram(0);
}

bool Shader::IsLoaded() const {
    return m_IsShaderLoaded;
}

int Shader::GetLocation(const std::string& p_Name){
    return glGetUniformLocation(m_ShaderProgramID, p_Name.c_str());
}

void Shader::Set(const std::string& name, const int& p_IntValue){
    glUniform1i(GetLocation(name), p_IntValue);
}

void Shader::Set(const std::string& name, const float& p_FloatValue){
    glUniform1f(GetLocation(name), p_FloatValue);
}

void Shader::Set(const std::string& name, const glm::vec2& p_Values){
    glUniform2f(GetLocation(name), p_Values.x, p_Values.y);
}

void Shader::Set(const std::string& name, const glm::vec3& p_Values){
    glUniform3f(GetLocation(name), p_Values.x, p_Values.y, p_Values.z);
}

void Shader::Set(const std::string& name, const glm::vec4& p_Values){
    glUniform4f(GetLocation(name), p_Values.x, p_Values.y, p_Values.z, p_Values.w);
}

void Shader::Set(const std::string& name, const glm::mat2& p_Values){
    glUniformMatrix2fv(GetLocation(name), 1, false, glm::value_ptr(p_Values));
}

void Shader::Set(const std::string& name, const glm::mat3& p_Values){
    glUniformMatrix3fv(GetLocation(name), 1, false, glm::value_ptr(p_Values));
}

void Shader::Set(const std::string& name, const glm::mat4& p_Values){
    glUniformMatrix4fv(GetLocation(name), 1, false, glm::value_ptr(p_Values));
}

ShaderLibrary::ShaderLibrary(){
    s_CurrentShaderLibraryInstance = this;
}

std::string Shader::GetShaderName() const{
    //! @note Getting our name from our shader
    //! @note Usage:
    //! @note Reading assets/shaders/texture.glsl
    //! @note GetShaderName returns 'texture' as the name of the shader itself rather then the entire path.
    if(m_Filepath.empty()){
        return "[SHADER] FAILED TO GET NAME";
    }

    auto last_slash = m_Filepath.find_last_of("/\\");
    last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;

    auto last_dot = m_Filepath.rfind('.');
    auto count = last_dot == std::string::npos ? m_Filepath.size() - last_slash : last_dot - last_slash;

    std::string name = m_Filepath.substr(last_slash, count);

    return name;
}

bool ShaderLibrary::IsExist(const std::string& p_ShaderName){
    return s_CurrentShaderLibraryInstance->m_ShaderLibs.contains(p_ShaderName);
}

Shader& ShaderLibrary::GetShader(const std::string& p_ShaderName){
    return s_CurrentShaderLibraryInstance->m_ShaderLibs.at(p_ShaderName);
}



void ShaderLibrary::Add(const Shader& p_Shader){
    m_ShaderLibs.insert({p_Shader.GetShaderName(), p_Shader});
}

void ShaderLibrary::Add(const std::string& p_Filename, const Shader& p_Shader){
    m_ShaderLibs.insert({p_Filename, p_Shader});
}

Shader ShaderLibrary::Load(const std::string& p_VertShader, const std::string& p_FragShader){
    Shader new_shader = Shader(p_VertShader, p_FragShader);
    Add(new_shader.GetShaderName(), new_shader);
    return new_shader;
}

Shader& ShaderLibrary::Get(const std::string& p_ShaderTag){
    return m_ShaderLibs.at(p_ShaderTag);
}