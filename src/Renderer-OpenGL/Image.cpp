#include <Renderer-OpenGL/Image.hpp>
#include <fmt/core.h>
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

Image::Image(const std::string& p_Filename){
    // int width, height, pixel_channels;
    // m_LiteralData = stbi_load(p_Filename.c_str(), &width, &height, &pixel_channels, 0);
    // m_Width = width;
    // m_Height = height;
    // m_PixelChannels = pixel_channels;
    
    // if(m_LiteralData == nullptr){
    //     m_IsImageLoaded = false;
    // }
    // else{
    //     m_IsImageLoaded = true;
    // }

    // m_Data = std::span<unsigned char>(m_LiteralData, width*height*pixel_channels);
    // // stbi_image_free(data);

    // fmt::print("Stb_Image Image::Image loaded successfull!\n");
}

Image::~Image(){
    // stbi_image_free(m_LiteralData);
}

std::span<unsigned char> Image::GetData(){
    return m_Data;
}