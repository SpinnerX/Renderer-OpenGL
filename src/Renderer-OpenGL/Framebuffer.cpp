#include <Renderer-OpenGL/Framebuffer.hpp>
#include <fmt/core.h>
#include <glad/glad.h>
#include <array>

static uint32_t s_Width = -1;
static uint32_t s_Height = -1;
/*
static uint32_t TextureTarget(bool multisample){
    return multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

static bool IsDepthFormat(FramebufferAttachmentType Type){
    switch(Type){
        case FramebufferAttachmentType::DEPTH: return true;
    }

    return false;
}

static void AttachColorTexture(uint32_t TextureID, int MultiSamples, GLenum Format, uint32_t Width, uint32_t Height, int index){
    if(MultiSamples > 1){
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, MultiSamples, Format, Width, Height, GL_FALSE);
    }
    else{
        glTexImage2D(GL_TEXTURE_2D, 0, Format, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(MultiSamples), TextureID, 0);
}

static void AttachDepthTexture(uint32_t TextureID, int Multisample, GLenum Format, GLenum AttachmentType,  uint32_t Width, uint32_t Height){
    
    if(Multisample > 1){
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, Multisample, Format, Width, Height, GL_FALSE);
    }
    else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, AttachmentType, TextureTarget(Multisample), TextureID, 0); 
}

FramebufferTexture::FramebufferTexture(FramebufferAttachmentType Type, uint32_t TextureTarget, uint32_t p_Format, uint32_t p_Multisample, uint32_t index) : m_TextureTarget(TextureTarget), m_Multisample(p_Multisample), m_Format(p_Format){
    // OnCreate(Type, TextureTarget);
    glGenTextures(TextureTarget, &m_TextureID);
    
    Bind();

    switch(Type){
    case FramebufferAttachmentType::RGBA8:
        AttachColorTexture(m_TextureID, m_Multisample, p_Format, s_Width, s_Height, index);
    case FramebufferAttachmentType::DEPTH:
    }

    
    Unbind();
}

FramebufferTexture::FramebufferTexture(FramebufferAttachmentType Type, uint32_t TextureTarget, uint32_t p_Index){
    glGenTextures(TextureTarget, &m_TextureID);

    Bind();
    
    switch(Type){
    case FramebufferAttachmentType::RGBA8:
        AttachColorTexture(m_TextureID, m_Multisample, GL_RGBA, s_Width, s_Height, p_Index);
        break;
    case FramebufferAttachmentType::DEPTH:
        AttachDepthTexture(m_TextureID, m_Multisample, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, s_Width, s_Height);
    }

    Unbind();   
}

void FramebufferTexture::OnCreate(FramebufferAttachmentType Type, uint32_t TextureTarget){
    glGenTextures(TextureTarget, &m_TextureID);

    Bind();

    //! @note Attaching multisamples
    // switch(Type){
    // case FramebufferAttachmentType::RGBA8:
    //     AttachColorTexture(m_TextureID, m_Multisample, GL_RGBA8, s_Width, s_Height, index);
    // }

    Unbind();
}


void FramebufferTexture::Bind(){
    glBindTexture(m_TextureTarget, m_TextureID);
}

void FramebufferTexture::Unbind(){
    glBindTexture(m_TextureTarget, 0);
}
*/

FramebufferTexture::FramebufferTexture(FramebufferAttachmentType Type, uint32_t Width, uint32_t Height, uint32_t Samples) : m_Width(Width), m_Height(Height){
    hasMultisamples = (Samples > 1);
    if(hasMultisamples){
        m_TextureTarget = GL_TEXTURE_2D_MULTISAMPLE;
    }
    else{
        m_TextureTarget = GL_TEXTURE_2D;
    }

    glGenTextures(1, &m_TextureID);

    Bind();

    switch (Type){
    case FramebufferAttachmentType::RGBA8: break;
    case FramebufferAttachmentType::DEPTH: break;
    default: break;
    }

    Unbind();
}

void FramebufferTexture::Bind(){
    glBindTexture(m_TextureTarget, m_TextureID);
}

void FramebufferTexture::Unbind(){
    glBindTexture(m_TextureTarget, 0);
}

void FramebufferTexture::AttachColorTexture(){
    if(hasMultisamples){

    }
    else{}
}

void FramebufferTexture::AttachDepthColorTexture(){
    if(hasMultisamples){}
    else{}
}

Framebuffer::Framebuffer(uint32_t Width, uint32_t Height, uint32_t p_Samples) : m_Samples(p_Samples){
    //! @note Setting up framebuffer texture attachments
    //! @note Replacing how framebuffer attachments are specfied
    s_Width = Width;
    s_Height = Height;

    OnCreate(Width, Height);
}

Framebuffer::~Framebuffer(){
    glDeleteFramebuffers(1, &m_FramebufferID);
}

void Framebuffer::OnCreate(uint32_t Width, uint32_t Height){
    glGenFramebuffers(1, &m_FramebufferID);
    Bind();

    
    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // attaching color attachment to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
    
    //! @note Setting depth attachment
    glGenTextures(1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Width, Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

    // attaching depth attachment to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 1, GL_TEXTURE_2D, m_DepthAttachment, 0);

    // Unbind();

    //! @note Creating texture for specifying framebuffer depth map (for shadow passes)
    glGenTextures(1, &m_DepthMapFbo);
    // m_FramebufferID = m_DepthMapFbo;
    glBindTexture(GL_TEXTURE_2D, m_DepthMapFbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //! @note Attaching depth/shadow mapping to this framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapFbo, 0);
    // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + 2, GL_TEXTURE_2D, m_DepthMapFbo, 0);
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    


    if(glCheckFramebufferStatus(m_FramebufferID) != GL_FRAMEBUFFER_COMPLETE){
        fmt::print("Framebuffer Incomplete!\n");
    }
    else{
        fmt::print("Framebuffers Completed!!!\n");
    }

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    Unbind();
}

void Framebuffer::Bind(){
    // glEnable(GL_FRAMEBUFFER_SRGB);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
}

void Framebuffer::Unbind(){
    // glDisable()
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::ClearFramebuffer(uint32_t index, int data){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClearBufferiv(GL_COLOR_ATTACHMENT0, 0, &data);
}

void Framebuffer::OnViewportResize(uint32_t Width, uint32_t Height){
    
}

int Framebuffer::Read(int attachmentIndex, int x, int y){
    glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
    int pixel_data;
    glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel_data);
    return pixel_data;
}