#pragma once
#include <string>
#include <span>

class Image{
public:
    Image() = default;
    Image(const std::string& p_Filename);
    ~Image();

    bool IsImageLoaded() const { return m_IsImageLoaded; }

    std::span<unsigned char> GetData();
    unsigned char* GetLiteralData(){ return m_LiteralData; }
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetPixelChannels() const { return m_PixelChannels; }

private:
    bool m_IsImageLoaded = false;
    std::span<unsigned char> m_Data;
    unsigned char* m_LiteralData = nullptr;
    int m_Width;
    int m_Height;
    int m_PixelChannels;
};