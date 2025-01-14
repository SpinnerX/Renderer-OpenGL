#pragma once
#include "Camera.hpp"
#include "Shader.hpp"
#include "TextureCubemap.hpp"
#include "VertexArray.hpp"
#include <string>
#include <span>

class EnvironmentMap{
public:
    // EnvironmentMap(const std::string& p_Filepath);
    EnvironmentMap(std::span<std::string> p_Faces);

    //! @note Used for loading .hdr maps
    EnvironmentMap(const std::string& p_Filepath, bool isHDRIEnabled=false);

    bool IsLoaded() const;

    void Bind();
    void Unbind();

    void OnUpdate(Camera& camera, glm::mat4& projection, uint32_t width, uint32_t height);
private:
    bool usingHDR = false;
    VertexArray m_CubemapVao;
    VertexArray m_SkyboxVao;
    TextureCubemap m_CubemapTextures;
    glm::mat4 m_Model;
    glm::mat4 m_View;
    glm::mat4 m_Projection;

    Shader m_CubemapShader;
    Shader m_SkyboxShader;
    Shader hdrShader;

    uint32_t m_HdrMap;
};