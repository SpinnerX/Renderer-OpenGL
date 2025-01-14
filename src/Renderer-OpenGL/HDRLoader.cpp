#include <Renderer-OpenGL/HDRLoader.hpp>
#include <fmt/core.h>
#include <glad/glad.h>
#include <Renderer-OpenGL/stb_image.hpp>

HDRLoader::HDRLoader(const std::string& p_Filepath){
    int w, h, pixel_channels;
    float* data = stbi_loadf(p_Filepath.c_str(), &w, &h, &pixel_channels, 0);

    if(data){
        glGenTextures(1, &m_HdrID);
        glBindTexture(GL_TEXTURE_2D, m_HdrID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, data); // note how we specify the texture's data value to be float

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else{
        fmt::print("Could not load filepath = {}\n", p_Filepath);
        return;
    }
}

bool HDRLoader::IsLoaded() const{}

void HDRLoader::Bind(){}

void HDRLoader::Unbind(){}