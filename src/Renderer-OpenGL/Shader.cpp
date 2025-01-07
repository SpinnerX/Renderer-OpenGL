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


Shader::Shader(const std::string& p_VertexShader, const std::string& p_FragmentShader) : m_IsShaderLoaded(true){
    //! @note We initialize our shader program to then take in our shader modules and used for linking them
    m_ShaderProgramID = glCreateProgram();

    // Load, Build, Compile our vertex and fragment shader modules
    int success;
    bool is_vert_loaded = LoadShaderModule(p_VertexShader, Shader_t::VERTEX);

    if(!is_vert_loaded){
        fmt::print("Vertex Shader DID NOT LOAD CORRECTLY!\n");
        return;
    }

    bool is_frag_loaded = LoadShaderModule(p_FragmentShader, Shader_t::FRAGMENT);
    
    if(!is_frag_loaded){
        fmt::print("FRAG SHADER did not load correctly!\n");
        return;
    }

    glLinkProgram(m_ShaderProgramID);

    glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
    if(!success){
        fmt::print("Could NOT load shader program!\n");
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
    const char* shader_src = ss.str().c_str();

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

bool Shader::IsLoaded() const {
    return m_IsShaderLoaded;
}

int Shader::GetLocation(const std::string& p_Name){
    return glGetUniformLocation(m_ShaderProgramID, p_Name.c_str());
}

void Shader::Set(const std::string& name, int p_IntValue){
    glUniform1i(GetLocation(name), p_IntValue);
}

void Shader::Set(const std::string& name, float p_IntValue){
    glUniform1i(GetLocation(name), p_IntValue);
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