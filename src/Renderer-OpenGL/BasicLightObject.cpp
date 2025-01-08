#include <Renderer-OpenGL/BasicLightObject.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <span>


BasicLight::BasicLight(std::span<float> p_Vertices, std::span<uint32_t> p_Indices){
    m_Vao = VertexArray(p_Vertices, p_Indices);

    m_Vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT, "objectColor", false}
    });

    m_LightShader = Shader("shaders/model_loading/intro_lighting.vs", "shaders/model_loading/intro_lighting.fs");

}

void BasicLight::UpdateLocation(Camera& camera){

    glm::mat4 proj = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    m_LightShader.Bind();
    m_LightShader.Set("objectColor", {1.0f, 0.5f, 0.31f});
    m_LightShader.Set("lightColor", {1.0f, 1.0f, 1.0f});

    m_Model = glm::mat4(1.0f);
    m_Model = glm::translate(m_Model, m_LightPosition);
    m_Model = glm::scale(m_Model, glm::vec3(0.2f));
    
    m_LightShader.Set("projection", proj);
    m_LightShader.Set("view", view);
    m_LightShader.Set("model", m_Model);

    m_Vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}