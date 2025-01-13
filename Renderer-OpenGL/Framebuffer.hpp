#pragma once
#include <cstdint>
#include <Renderer-OpenGL/Texture2D.hpp>
#include <span>
#include <vector>

enum class FramebufferFormatAttribute{
    NONE = 0,
    DEPTH_WITH_STENCIL, // SPECIFY UTILIZING BOTH DEPTH AND STENCIL BUFFERS
    DEPTH_NO_STENCIL, // SPECIFY UTILIZING ONLY DEPTH BUFFERS
    ONLY_STENCIL // SPECIFY UTILIZING ONLY STENCIL BUFFERS
};


enum class FramebufferAttachmentType{
    NONE = 0,
    RGBA8,
    RGBA16,
    DEPTH24STENCIL8,
    SHADOW_MAP,
    NORMAL_MAP,
    DEPTH = DEPTH24STENCIL8,
};

enum class FramebufferSpecification{
    DEPTH,
    SHADOW_MAP
};


/*
struct FramebufferAttachments{
    FramebufferAttachments() = default;
    FramebufferAttachments(const std::span<FramebufferAttachmentType>& p_Attachments) : Attachments(p_Attachments){}
    // FramebufferAtt
    std::span<FramebufferAttachmentType> Attachments;
    // counter if textures have multisamples
    uint32_t MultiSampleCount = 1;

    bool SwapchainTarget = false;
};

class FramebufferTexture{
public:
    FramebufferTexture() = default;
    FramebufferTexture(FramebufferAttachmentType Type, uint32_t TextureTarget, uint32_t Format, uint32_t p_Multisample, uint32_t index);
    FramebufferTexture(FramebufferAttachmentType Type, uint32_t TextureTarget, uint32_t index);

    void OnCreate(FramebufferAttachmentType Type, uint32_t TextureTarget);

    void Bind();
    void Unbind();
private:
    uint32_t m_TextureTarget = -1;
    uint32_t m_Multisample;
    uint32_t m_Format = -1;
    uint32_t m_TextureID = -1;
};
*/

class FramebufferTexture{
public:
    FramebufferTexture() = default;
    FramebufferTexture(FramebufferAttachmentType Type, uint32_t Width, uint32_t Height, uint32_t Samples = 1);

    void Bind();

    void Unbind();

private:
    void AttachColorTexture();

    void AttachDepthColorTexture();

private:
    uint32_t m_Width = -1;
    uint32_t m_Height = -1;
    uint32_t m_TextureTarget = -1;
    bool hasMultisamples=false;
    uint32_t m_TextureID = -1;
};

/*

FramebufferAttachments attachments = {
    GL_RED
    RGBA8,
    DEPTH24STENCIL8,
}

Framebuffer frame_buffer = Framebuffer(Width, Height, attachments);


*/


class Framebuffer{
public:
    Framebuffer() = default;
    Framebuffer(uint32_t Width, uint32_t Height, uint32_t Samples=1);
    ~Framebuffer();
    
    void Bind();
    void Unbind();

    void ClearFramebuffer(uint32_t index, int);

    void OnViewportResize(uint32_t Width, uint32_t Height);

    uint32_t GetFramebufferID() { return m_FramebufferID; }

    uint32_t GetColorAttachmentID() { return m_ColorAttachment; }
    uint32_t GetDepthAttachmentID() { return m_DepthAttachment; }

    int Read(int attachmentIndex, int x, int y);

private:
    void OnCreate(uint32_t Width, uint32_t Height);

private:
    uint32_t m_FramebufferID = -1;

    uint32_t m_ColorAttachment = -1;

    uint32_t m_DepthAttachment = -1;

    uint32_t m_DepthMapFbo = -1;

    uint32_t m_Samples = 1;

    std::vector<FramebufferAttachmentType> m_ColorAttachmentsFormat;

    //! @note once this works, then we will remove m_ColorAttachment and m_DepthAttachment altogether
    FramebufferAttachmentType m_Color_Attachment;
    FramebufferAttachmentType m_Depth_Attachment;
    FramebufferAttachmentType m_DepthFormatCheck = FramebufferAttachmentType::NONE;
    // FramebufferAttachments m_Attachments;
    // Texture2D m_ColorAttachment;

    // uint32_t m_RenderBufferObject;
};