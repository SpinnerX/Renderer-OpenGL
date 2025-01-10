#include <Renderer-OpenGL/EnvironmentMap.hpp>
#include <fmt/core.h>
#include <glad/glad.h>
#include <Renderer-OpenGL/stb_image.hpp>
#include <Renderer-OpenGL/Shader.hpp>

EnvironmentMap::EnvironmentMap(std::span<std::string> p_Faces, bool isDepthMaskEnabled) : m_IsDepthMaskEnabled(isDepthMaskEnabled){
    if(ShaderLibrary::IsExist("cubemap")){
        fmt::print("Shader named cubemap exists!\n");
    }

    if(ShaderLibrary::IsExist("skybox")){
        fmt::print("Shader named skybox exists!\n");
    }

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
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

    VertexBuffer cube_map_vbo = VertexBuffer(cubeVertices);
    VertexBuffer skybox_vbo = VertexBuffer(skyboxVertices);

    m_CubemapVao = VertexArray(&cube_map_vbo);
    m_CubemapVao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    m_SkyboxVao = VertexArray(&skybox_vbo);
    m_SkyboxVao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false}
    });




    glGenTextures(1, &m_CubemapTextureID);
    Bind();
    int w, h, pixel_channels;
    unsigned char* data;

    for(uint32_t i = 0; i < p_Faces.size(); i++){
        data = stbi_load(p_Faces[i].c_str(), &w, &h, &pixel_channels, 0);
        if(data == nullptr){
            fmt::print("Could not load cubemap texture location filepath of = {}", p_Faces[i]);
            continue;
        }
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
        0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data
        );
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    Unbind();
}

void EnvironmentMap::BindTexture(){
    Shader& cubemap_shader = ShaderLibrary::GetShader("cubemap");
    Shader& skybox_shader = ShaderLibrary::GetShader("skybox");

    cubemap_shader.Bind();
    cubemap_shader.Set("texture1", 0);

    skybox_shader.Bind();
    skybox_shader.Set("skybox", 0);
}

void EnvironmentMap::Bind() {
}

void EnvironmentMap::Unbind(){
}

void EnvironmentMap::OnUpdate(Camera& camera, glm::mat4& view, glm::mat4 proj){
    Shader cubemap_shader = ShaderLibrary::GetShader("cubemap");
    Shader skybox_shader = ShaderLibrary::GetShader("skybox");

    // cubemap
    cubemap_shader.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    cubemap_shader.Set("model", model);
    cubemap_shader.Set("view", view);
    cubemap_shader.Set("projection", proj);


    // skybox
    skybox_shader.Bind();
    skybox_shader.Set("view", view);
    skybox_shader.Set("projection", proj);
    
    glDepthMask(GL_FALSE);
    m_SkyboxVao.Bind();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubemapTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    m_SkyboxVao.Unbind();
    glDepthMask(GL_TRUE);
}

bool EnvironmentMap::IsLoaded() const {
    return m_IsEnvironmentMapLoaded;
}