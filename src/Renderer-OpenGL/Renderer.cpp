#include "Camera.hpp"
#include <Renderer-OpenGL/Renderer.hpp>
#include <Renderer-OpenGL/VertexArray.hpp>

static Camera g_Camera;

void Renderer::Initialize(){
}

void Renderer::Begin(Camera& camera){
    g_Camera = camera;
}

void Renderer::SetViewport(uint32_t Width, uint32_t Height){
    glViewport(0, 0, Width, Height);
}

void Renderer::End(){
}

void Renderer::DrawQuadPrimitive(const VertexArray& p_Vao){
    p_Vao.Bind();
    if(p_Vao.HasIndices()){
        int count = p_Vao.GetCurrentIbo().GetIndices().size();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_BYTE, 0);
    }
    else{
        //! TODO: Modify this glDrawArrays to dynamically tell if we are drawing by sequential counts rather then having indices 
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    p_Vao.Unbind();
}

void Renderer::DrawEnvironmentMap(EnvironmentMap& p_Map, glm::mat4& projection, uint32_t width, uint32_t height){
    p_Map.OnUpdate(g_Camera, projection, width, height);
}