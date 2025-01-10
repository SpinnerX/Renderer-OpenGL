#include <Renderer-OpenGL/Framebuffer.hpp>
#include <fmt/core.h>
#include <glad/glad.h>

//! @note On Video 39:12 cherno's video on framebuffers with imgui viewport
Framebuffer::Framebuffer(uint32_t Width, uint32_t Height){
    OnCreate(Width, Height);
}

Framebuffer::~Framebuffer(){
    glDeleteFramebuffers(1, &m_FramebufferID);
}

void Framebuffer::OnCreate(uint32_t Width, uint32_t Height){
    glGenFramebuffers(1, &m_FramebufferID);
    Bind();

    //! @note Setting up framebuffer texture
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
    
    if(glCheckFramebufferStatus(m_FramebufferID) != GL_FRAMEBUFFER_COMPLETE){
        fmt::print("Framebuffer Incomplete!\n");
    }
    else{
        fmt::print("Framebuffers Completed!!!\n");
    }

    Unbind();
}

void Framebuffer::Bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
}

void Framebuffer::Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::OnViewportResize(uint32_t Width, uint32_t Height){
    
}