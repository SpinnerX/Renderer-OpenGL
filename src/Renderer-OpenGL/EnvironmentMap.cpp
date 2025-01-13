#include "Renderer.hpp"
#include <Renderer-OpenGL/EnvironmentMap.hpp>
#include <fmt/core.h>
#include <glad/glad.h>
#include <Renderer-OpenGL/stb_image.hpp>
#include <Renderer-OpenGL/Shader.hpp>


void InitializeCubemap(VertexArray& vao){
    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    VertexBuffer cubemap_vbo = VertexBuffer(cubeVertices);
    vao = VertexArray(&cubemap_vbo);
}

void InitializeSkybox(VertexArray& vao){
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    VertexBuffer skybox_vbo = VertexBuffer(skyboxVertices);
    vao = VertexArray(&skybox_vbo);
}


EnvironmentMap::EnvironmentMap(std::span<std::string> p_Faces){
    // m_CubemapVao = VertexArray();
    // InitializeCubemap(m_CubemapVao);

    // m_CubemapVao.SetVertexAttribute({
    //     {VertexAttributeType::FLOAT3, "aPos", false},
    //     {VertexAttributeType::FLOAT2, "aTexCoords", false}
    // });

    m_SkyboxVao = VertexArray();
    InitializeSkybox(m_SkyboxVao);
    m_SkyboxVao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        // {VertexAttributeType::FLOAT3, "aNormal", false},
        // {VertexAttributeType::FLOAT3, "a", false}
    });

    m_CubemapTextures = TextureCubemap(p_Faces);
    // m_CubemapShader = ShaderLibrary::GetShader("cubemap");

    // m_CubemapShader.Bind();
    // m_CubemapShader.Set("skybox", 0);

    m_SkyboxShader = ShaderLibrary::GetShader("skybox");
    m_SkyboxShader.Bind();
    m_SkyboxShader.Set("skybox", 0);
}

void EnvironmentMap::Bind() {
}

void EnvironmentMap::Unbind(){
}

void EnvironmentMap::OnUpdate(Camera& camera, glm::mat4& projection, uint32_t width, uint32_t height){
    m_Model = glm::mat4(1.f);
    //! @note TODO: Consider more testing if we really need CubeMap shader.... (???)
    // m_Projection = glm::perspective(glm::radians(camera.Zoom), (float)width/(float)height, 0.1f, 100.0f);
    
    //! @note Activate our shader to write the model, view, and projection matrices
    // m_CubemapShader.Bind();
    // m_CubemapShader.Set("model", m_Model);
    // m_CubemapShader.Set("view", m_View);
    // m_CubemapShader.Set("projection", projection);
    // m_CubemapShader.Set("cameraPos", camera.Position);

    //! @note activate our texture for cubemap then unactivate once we do draw call
    // m_CubemapTextures.Bind();
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    // m_CubemapVao.Unbind();

    glDepthFunc(GL_LEQUAL);
    //! @note Only the skybox needs it.
    m_SkyboxShader.Bind();
    m_View = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    m_SkyboxShader.Set("cameraPos", camera.Position);
    m_SkyboxShader.Set("view", m_View);
    m_SkyboxShader.Set("projection", projection);
    m_SkyboxShader.Set("model", m_Model);
    
    m_SkyboxVao.Bind();
    
    //! @note We activate the cubemap face textures to render for our skybox
    m_CubemapTextures.Bind();
    // glDrawArrays(GL_TRIANGLES, 0, 36);
    Renderer::DrawQuadPrimitive(m_SkyboxVao);
    glDepthFunc(GL_LESS);
    m_SkyboxVao.Unbind();
}

bool EnvironmentMap::IsLoaded() const {
    return false;
}