#include "Renderer-OpenGL/IndexBuffer.hpp"
#include "Renderer-OpenGL/Shader.hpp"
#include "Renderer-OpenGL/VertexArray.hpp"
#include <iostream>
#include <fmt/core.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
using namespace std;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos, 1.0);\n"
    "   vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\n\0";

int main(){
    if(!glfwInit()){
        printf("glfwInit() failed!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800, height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    //! @note Init glad
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        fmt::print("Failed to initialize glad!");
        return -2;
    }

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    // Actual code here
    Shader shader = Shader("shaders/shader_0/shader_0.vert.glsl", "shaders/shader_0/shader_0.frag.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // VertexBuffer vbo = VertexBuffer(vertices);

    //! @note We are setting the vertex buffer attributes and then adding that to the vertex array
    //! @note Does the equivalent to doing
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // vbo.SetVertexAttributes({
    //     {VertexAttributeType::FLOAT3, "ourColor", false}
    // });

    // fmt::print("sizeof(indices) = {}\n", sizeof(indices));

    // IndexBuffer ibo = IndexBuffer(indices);
    // VertexArray vao = VertexArray(vbo, ibo);

    
    
    // unsigned int VBO, IBO, VAO;
    // unsigned int IBO, VAO;
    // unsigned int VAO;
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // VertexBuffer vbo = VertexBuffer(vertices);
    // glGenBuffers(1, &IBO);
    
    // glBindVertexArray(VAO);
    // IndexBuffer ibo = IndexBuffer(indices);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

    //! @note Vertex/Index buffers are stored in the vertex array, we can directly set our vertices here.
    //! @note Vertex Arrays store the attributes and you need to bind them so they know which buffer to associate these attributes to which vertex buffer
    VertexArray vao = VertexArray(vertices, indices);
    vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "ourColor", false}
    });

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    // glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // */


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        // glUseProgram(shaderProgram);
        shader.Bind();

        // update uniform color
        float time_val = glfwGetTime();
        float green_val = std::sin(time_val / 2.0f) + 0.5f;
        // shader.Set("ourColor", )
        // int vert_color_location = glGetUniformLocation(shaderProgram, "ourColor");
        // glUniform4f(vert_color_location, 0.0f, green_val, 0.0f, 1.0f);


        shader.Set("ourColor", glm::vec4(0.0f, green_val, 0.0f, 1.0f));

        vao.Bind();
        // glBindVertexArray(VAO);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        //! @note Should check if we have indices so we can only print by vertices or by vertices only (something todo)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
