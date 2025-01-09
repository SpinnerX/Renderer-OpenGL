#include <Renderer-OpenGL/Framebuffer.hpp>
#include <fmt/core.h>
#include <glad/glad.h>

//! @note On Video 39:12 cherno's video on framebuffers with imgui viewport
Framebuffer::Framebuffer(uint32_t Width, uint32_t Heigh, FramebufferAttributes fb_attribute){
    glGenFramebuffers(1, &m_FramebufferID);
    
    Bind();
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fmt::print("OpenGL Framebuffer was Incomplete!!!\n");
        return;
    }

    m_ColorAttachmentTexture = Texture2D(800, 600);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentTexture.GetID(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_ColorAttachmentTexture.GetID(), 0);

    Unbind();
}

void Framebuffer::InitializeAttachments(){
}

Framebuffer::~Framebuffer(){
    glDeleteFramebuffers(1, &m_FramebufferID);
}

void Framebuffer::Bind(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferID);
}

void Framebuffer::Unbind(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::OnViewportResize(uint32_t Width, uint32_t Height){

}
