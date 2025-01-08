#include <Renderer-OpenGL/Texture.hpp>
#include <glad/glad.h>
#include <fmt/core.h>
#include <Renderer-OpenGL/stb_image.hpp>

Texture::Texture(const std::string& p_Filename, TextureType p_Type){
    glGenTextures(1, &m_TextureID);
    Bind();
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load and generate the texture
    int image_width, image_height, pixel_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(p_Filename.c_str(), &image_width, &image_height, &pixel_channels, 0);
    
    // if(nrChannels)
    fmt::print("Pixel Channels = {}\n", pixel_channels);
    GLenum internal_format;
    GLenum format_to_use;
    if(pixel_channels == 1){
        internal_format = GL_RED;
        format_to_use = GL_RED;
    }
    if(pixel_channels == 3){
        internal_format = GL_RGB;
        format_to_use = GL_RGB;
    }
    else if(pixel_channels == 4){
        internal_format = GL_RGBA;
        format_to_use = GL_RGBA;
    }

    

    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, image_width, image_height, 0, format_to_use, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        // std::cout << "Failed to load texture" << std::endl;
        fmt::print("Failed to load texture!!\n");
    }
    stbi_image_free(data);
}

void Texture::Bind(int p_TextureIndex){
    glActiveTexture(GL_TEXTURE0 + p_TextureIndex);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}