#include <Renderer-OpenGL/TextureCubemap.hpp>
#include <fmt/core.h>
#include <Renderer-OpenGL/stb_image.hpp>
#include <glad/glad.h>

TextureCubemap::TextureCubemap(std::span<std::string> p_Faces){
    glGenTextures(1, &m_TextureCubemapID);
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

TextureCubemap::TextureCubemap(uint32_t Width, uint32_t Height, uint32_t p_FaceSize){
    glGenTextures(1, &m_TextureCubemapID);
    Bind();

    for(uint32_t i = 0; i < 6; i++){
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, Width, Height, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // enable pre-filter mipmap sampling (combatting visible dots artifact)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Unbind();
}

void TextureCubemap::Bind(){
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureCubemapID);
}

void TextureCubemap::Unbind(){
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}