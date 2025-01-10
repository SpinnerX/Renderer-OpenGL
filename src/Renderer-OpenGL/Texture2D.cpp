#include <Renderer-OpenGL/Texture2D.hpp>
#include <glad/glad.h>
#include <fmt/core.h>

// #ifndef STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>
// #endif
#include <Renderer-OpenGL/stb_image.hpp>

Texture2D::Texture2D(const std::string& p_Filename){
    glGenTextures(1, &m_TextureID);
    Bind();
    
    // load and generate the texture
    int image_width, image_height, pixel_channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(p_Filename.c_str(), &image_width, &image_height, &pixel_channels, 0);
    
    fmt::print("Pixel Channels = {}\n", pixel_channels);
    GLenum internal_format;
    GLenum format_to_use;

    if(pixel_channels == 1){
        internal_format = GL_RED;
        format_to_use = GL_RED;
    }
    else if(pixel_channels == 3){
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

        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        // std::cout << "Failed to load texture" << std::endl;
        fmt::print("Failed to load texture!!\n");
    }
    stbi_image_free(data);
    

}

Texture2D::Texture2D(uint32_t Width, uint32_t Height, bool attachBothDepthAndStencile){
    glGenTextures(1, &m_TextureID);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);

    if(attachBothDepthAndStencile){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Width, Height, 0, 
            GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    }
    else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture2D::Bind(int p_TextureIndex){
    if(p_TextureIndex == 0){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_TextureID);
        return;
    }
    
    glActiveTexture(GL_TEXTURE0 + p_TextureIndex);
    glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture2D::Unbind(){
    glBindTexture(GL_TEXTURE_2D, 0);
}