#include "Renderer-OpenGL/Shader.hpp"
#include <Renderer-OpenGL/Texture2D.hpp>
#include "Renderer-OpenGL/VertexArray.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <fmt/core.h>
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <stb_image.h>
using namespace std;


static void BeginFrame(){
    // Begin Imgui Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

static void EndFrame(GLFWwindow* Window, uint32_t width, uint32_t height){
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = width;
    io.DisplaySize.y = height;
    ImGui::Render();

    //! @note Clear each frame
    glViewport(0, 0, width, height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        GLFWwindow* backup_ctx = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        glfwMakeContextCurrent(Window);
    }
}

int main(){
    if(!glfwInit()){
        printf("glfwInit() failed!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800, height = 600;

    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Renderer", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    fmt::print("GLFW VERSION!!!\n");
    fmt::print("{}\n", glfwGetVersionString());

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
    ImGui_ImplOpenGL3_Init("#version 410");


    Shader shader = Shader("shaders/shader_0/shader_0.vert.glsl", "shaders/shader_0/shader_0.frag.glsl");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // float vertices[] = {
    //  0.5f,  0.5f, 0.0f,  // top right
    //  0.5f, -0.5f, 0.0f,  // bottom right
    // -0.5f, -0.5f, 0.0f,  // bottom left
    // -0.5f,  0.5f, 0.0f   // top left 
    // };
    
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    // std::string filename = "assets/wall.jpg";
    // Texture2D wall_texture = Texture2D(filename);

    // if(!wall_texture.IsTextureLoaded()){
    //     fmt::print("Wall Texture using filepath = {} DID NOT LOAD CORRECTLY!\n", filename);
    // }

    //! @note Vertex/Index buffers are stored in the vertex array, we can directly set our vertices here.
    //! @note Vertex Arrays store the attributes and you need to bind them so they know which buffer to associate these attributes to which vertex buffer
    VertexArray vao = VertexArray(vertices, indices);
    // std::string filename = "assets/wall.jpg";
    // Texture2D wall_texture = Texture2D(filename);

    //! @note We are setting the vertex buffer attributes and then adding that to the vertex array
    //! @note Does the equivalent to doing these two lines
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);
    //! @note These specify our vertex attribuets which also correspond to our layout bindings
    //! @note When we do layout(location = n) ...
    //! @note Reference to side-comments for more details
    vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},           // layout (location = 0) in vec3 aPos;
        {VertexAttributeType::FLOAT3, "aColor", false},         // layout (location = 1) in vec3 aColor;
        {VertexAttributeType::FLOAT2, "aTexCoord", false}       // layout (location = 2) in vec2 aTexCoord;
    });

    Texture2D wall_texture = Texture2D("assets/wall.jpg");

    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

    while (!glfwWindowShouldClose(window)){
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // BeginFrame();

        float time_val = glfwGetTime();
        float green_val = std::sin(time_val / 2.0f) + 0.5f;


        // ImGui::Begin("Test");
        // ImGui::DragFloat4("Set Color", glm::value_ptr(color));
        // ImGui::End();
        // shader.Set("ourColor", glm::vec4(0.0f, green_val, 0.0f, 1.0f));
        // shader.Set("ourColor", glm::vec3(color.x, color.y, color.z));
        // shader.Set("texture2", 1);

        // wall_texture.Bind();

        // binding our texture to correspond to texture units
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, texture);
        wall_texture.Bind();
        shader.Bind();
        vao.Bind();

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        //! @note Should check if we have indices so we can only print by vertices or by vertices only (something todo)
        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // EndFrame(window, width, height);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
}
