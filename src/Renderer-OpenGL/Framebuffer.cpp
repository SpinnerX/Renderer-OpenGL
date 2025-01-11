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


    //! @note Creating texture for specifying framebuffer depth map (for shadow passes)
    glGenTextures(1, &m_DepthMapFbo);
    glBindTexture(GL_TEXTURE_2D, m_DepthMapFbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Width, Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //! @note Attaching depth/shadow mapping to this framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapFbo, 0);
    // glDrawBuffer(GL_NONE);
    // glReadBuffer(GL_NONE);



    if(glCheckFramebufferStatus(m_FramebufferID) != GL_FRAMEBUFFER_COMPLETE){
        fmt::print("Framebuffer Incomplete!\n");
    }
    else{
        fmt::print("Framebuffers Completed!!!\n");
    }

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

void Framebuffer::OnViewportResize(uint32_t Width, uint32_t Height){
    
}