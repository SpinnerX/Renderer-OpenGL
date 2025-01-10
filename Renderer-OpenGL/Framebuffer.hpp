#pragma once
#include <cstdint>
#include <Renderer-OpenGL/Texture2D.hpp>

enum class FramebufferFormatAttribute{
    NONE = 0,
    DEPTH_WITH_STENCIL, // SPECIFY UTILIZING BOTH DEPTH AND STENCIL BUFFERS
    DEPTH_NO_STENCIL, // SPECIFY UTILIZING ONLY DEPTH BUFFERS
    ONLY_STENCIL // SPECIFY UTILIZING ONLY STENCIL BUFFERS
};

/*

FramebufferAttributes fb_attr = { "Framebuffer1", DEPTH_WITH_STENCLI, true};

*/
struct FramebufferAttributes{

    FramebufferFormatAttribute FormatAttribute;
};

class Framebuffer{
public:
    Framebuffer() = default;
    Framebuffer(uint32_t Width, uint32_t Height);
    ~Framebuffer();
    
    void Bind();
    void Unbind();

    void OnViewportResize(uint32_t Width, uint32_t Height);

    uint32_t GetFramebufferID() { return m_FramebufferID; }

    uint32_t GetColorAttachmentID() { return m_ColorAttachment; }
    uint32_t GetDepthAttachmentID() { return m_DepthAttachment; }

private:
    void OnCreate(uint32_t Width, uint32_t Height);

private:
    uint32_t m_FramebufferID = -1;

    uint32_t m_ColorAttachment = -1;

    uint32_t m_DepthAttachment = -1;

    // Texture2D m_ColorAttachment;

    // uint32_t m_RenderBufferObject;
};