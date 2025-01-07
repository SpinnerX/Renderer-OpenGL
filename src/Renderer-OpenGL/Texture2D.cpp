#include <Renderer-OpenGL/Texture2D.hpp>
#include <glad/glad.h>
#include <fmt/core.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D::Texture2D(const std::string& p_Filename){
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // load and generate the texture
    int image_width, image_height, nrChannels;
    unsigned char *data = stbi_load("assets/wall.jpg", &image_width, &image_height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image_width, image_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        // std::cout << "Failed to load texture" << std::endl;
        fmt::print("Failed to load texture!!\n");
    }
    stbi_image_free(data);

}

void Texture2D::Bind(){
    // glBindTexture(GL_TEXTURE_2D, m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}