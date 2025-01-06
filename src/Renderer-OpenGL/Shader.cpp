#include <Renderer-OpenGL/Shader.hpp>
#include <glad/glad.h>
#include <fmt/format.h>

Shader::Shader(const std::string& p_VertexShader, const std::string& p_FragmentShader){
    m_VertShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_VertShaderID, 1, (const GLchar *const *)p_VertexShader.data(), nullptr);
    glCompileShader(m_VertShaderID);

    int success;
    // std::string infoLog;
    char infoLog[512];
    glGetShaderiv(m_VertShaderID, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(m_VertShaderID, 512, nullptr, infoLog);
        fmt::print("Vertex Shader Compiled Failed with Log Message === \"{}\"", infoLog);
        return;
    }

    m_FragShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_FragShaderID, 1, (const GLchar *const *)p_FragmentShader.data(), nullptr);
    glCompileShader(m_FragShaderID);

    glGetShaderiv(m_FragShaderID, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(m_FragShaderID, 512, nullptr, infoLog);
        fmt::print("Fragment Shader Compiled Failed with Log Message === \"{}\"", infoLog);
        return;
    }
}

bool Shader::IsLoaded() const {
    return m_IsShaderLoaded;
}