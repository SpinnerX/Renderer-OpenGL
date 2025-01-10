#pragma once
#include "Camera.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include <string>
#include <span>

class EnvironmentMap{
public:
    // EnvironmentMap(const std::string& p_Filepath);
    EnvironmentMap(std::span<std::string> p_Faces, bool isDepthMaskEnabled=false);

    bool IsLoaded() const;

    void Bind();
    void Unbind();

    void BindTexture();

    void OnUpdate(Camera& camera, glm::mat4& view, glm::mat4 proj);

    bool GetDepthMaskStatus() { return m_IsDepthMaskEnabled; }


private:
    Shader m_CubemapObjectShader;
    Shader m_SkyboxShader;

    VertexArray m_CubemapVao;
    VertexArray m_SkyboxVao;

    bool m_IsDepthMaskEnabled=false;
    uint32_t m_CubemapTextureID = -1;
    bool m_IsEnvironmentMapLoaded = false;

};