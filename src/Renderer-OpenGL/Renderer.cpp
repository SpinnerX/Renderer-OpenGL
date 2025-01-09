#include "Camera.hpp"
#include <Renderer-OpenGL/Renderer.hpp>
#include <Renderer-OpenGL/VertexArray.hpp>

void Renderer::Initialize(){
}

void Renderer::Begin(){
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
}