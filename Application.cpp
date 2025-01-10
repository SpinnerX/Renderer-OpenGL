#include "Renderer-OpenGL/EnvironmentMap.hpp"
#include "Renderer-OpenGL/TextureCubemap.hpp"
#include <Renderer-OpenGL/Renderer.hpp>
#include <Renderer-OpenGL/Shader.hpp>
#include <array>
#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Renderer-OpenGL/Camera.hpp>
#include <Renderer-OpenGL/InputPoll.hpp>

#include <imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <Renderer-OpenGL/Framebuffer.hpp>

static void ImGuiLayoutColorModification(){
    auto& colors = ImGui::GetStyle().Colors; // @note Colors is ImVec4
		
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		
		
		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };


		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		
		// Titles
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.1f, 0.150f, 0.951f, 1.0f };
}

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

void DockspaceWindow(GLFWwindow* window, int Width, int Height, Framebuffer& frame_buffer, const std::function<void()>& p_UpdateUI){
    // READ THIS !!!
    // TL;DR; this demo is more complicated than what most users you would normally use.
    // If we remove all options we are showcasing, this demo would become:
    //     void ShowExampleAppDockSpace()
    //     {
    //         ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
    //     }
    // In most cases you should be able to just call DockSpaceOverViewport() and ignore all the code below!
    // In this specific demo, we are not using DockSpaceOverViewport() because:
    // - (1) we allow the host window to be floating/moveable instead of filling the viewport (when opt_fullscreen == false)
    // - (2) we allow the host window to have padding (when opt_padding == true)
    // - (3) we expose many flags and need a way to have them visible.
    // - (4) we have a local menu bar in the host window (vs. you could use BeginMainMenuBar() + DockSpaceOverViewport()
    //      in your code, but we don't here because we allow the window to be floating)

    static bool docking_enabled = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    
    if (opt_fullscreen){
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else{
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode){
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &docking_enabled, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar()){
        if (ImGui::BeginMenu("Options")){
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Flag: NoDockingOverCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingOverCentralNode) != 0)) { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingOverCentralNode; }
            if (ImGui::MenuItem("Flag: NoDockingSplit",         "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingSplit) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingSplit; }
            if (ImGui::MenuItem("Flag: NoUndocking",            "", (dockspace_flags & ImGuiDockNodeFlags_NoUndocking) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoUndocking; }
            if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                   { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
            if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))             { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
            if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
            ImGui::Separator();

            if (ImGui::MenuItem("Close Dockspace", nullptr, false)){
                glfwSetWindowShouldClose(window, true);
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Before we render we add render style
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("Viewport");

    //! @note This is to ensure that we are resizing our framebuffer to fit our window-resized event
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

    // fmt::print("viewportPanelSize.x = {} and viewportPanelSize.y = {}\n", viewportPanelSize.x, viewportPanelSize.y);
    if(Width != viewportPanelSize.x and Height != viewportPanelSize.y and viewportPanelSize.x > 0 and viewportPanelSize.y > 0){
        frame_buffer.OnViewportResize((uint32_t)Width, (uint32_t)Height);
    }
    // if(Width != )

    auto fb_id = frame_buffer.GetColorAttachmentID();
    ImGui::Image((void*)fb_id, ImVec2(Width, Height));

    p_UpdateUI();
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::End();
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

int main(){
    // camera
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    // settings
    const unsigned int width = 1800;
    const unsigned int height = 820;
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Renderer", NULL, NULL);
    if (window == NULL){
        // std::cout << "Failed to create GLFW window" << std::endl;
        fmt::print("GLFW ERROR: Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    InputPoll::Initialize(window);
    bool firstMouse = true;

    // timing
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        // std::cout << "Failed to initialize GLAD" << std::endl;
        fmt::print("GLAD ERROR: Failed to initialize GLAD!\n");
        return -1;
    }

    float vertices[] = {
        //              positions            // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // Framebuffer frame_buffer = Framebuffer(width, height);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    //io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    // Setting custom dark themed to imgui
    ImGuiLayoutColorModification();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // Shader lighting_shader("shaders/tutorials/basic_lighting_maps_2/lighting_map.vs", "shaders/tutorials/basic_lighting_maps_2/lighting_map.fs");
    // Shader lighting_shader("shaders/tutorials/basic_lighting_casters_3/basic_lighting_casters.vs", "shaders/tutorials/basic_lighting_casters_3/basic_lighting_casters.fs");
    Shader lighting_shader(
        "shaders/tutorials/basic_multiple_lights_4/multiple_lights.vs", 
        "shaders/tutorials/basic_multiple_lights_4/multiple_lights.fs"
    );

    Shader cube_shader("shaders/model_loading/basic_cube.vs", "shaders/model_loading/basic_cube.fs");

    // cube vbo, and vao
    VertexBuffer vbo = VertexBuffer(vertices);

    //! @note This is how to get currently binded vertex buffer id
    // int current_vbo;
    // glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &current_vbo);

    //! @note These are two different cubes
    //! @note In OpenGL the idea is to have each vertex array per cube (based on this example).
    VertexArray cube_vao = VertexArray(&vbo);
    cube_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT3, "aNormal", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    //! @note How to get the current vao that is binded (used for debugging)
    // int current_vao;
    // glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);

    //! @note NOTE TO SELF: IF the cube or primitive does not look right
    //! @note These vertex attributes have to align to the layouts within the glsl
    //! @note Layouts like layout(location = 0), layout(location = 1), etc.
    VertexArray light_cube_vao = VertexArray(&vbo);
    light_cube_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT3, "aNormal", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    Texture2D container_diffuse = Texture2D("assets/container_diffuse.png");
    Texture2D container_specular = Texture2D("assets/container_specular.png");
    
    lighting_shader.Bind();
    lighting_shader.Set("material.diffuse", 0);
    lighting_shader.Set("material.specular", 1);
    lighting_shader.Unbind();

    // lighting
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    std::string light_variable_name = "light";

    glm::vec3 cube_positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 point_light_positions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


























    //! @note Loading skybox
    std::array<std::string, 6> faces = {
        "assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
    };

    ShaderLibrary library;
    library.Add(
        Shader(
            "shaders/tutorials/cubemap/skybox.vs",
            "shaders/tutorials/cubemap/skybox.fs"
        )
    );

    library.Add(
        Shader(
            "shaders/tutorials/cubemap/cubemap.vs",
            "shaders/tutorials/cubemap/cubemap.fs"
        )
    );

    // EnvironmentMap skybox = EnvironmentMap(faces, true);
    // skybox.BindTexture();

    // -------------------------------------------
    //! @note Skybox/Cubemap Configurations!
    // -------------------------------------------

    VertexBuffer cubemap_vbo = VertexBuffer(cubeVertices);
    VertexBuffer skybox_vbo = VertexBuffer(skyboxVertices);

    VertexArray cubemap_vao = VertexArray(&cubemap_vbo);
    cubemap_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    VertexArray skybox_vao = VertexArray(&skybox_vbo);
    skybox_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false}
    });

    // Texture2D cubemap_texture()
    TextureCubemap cubemap_texture = TextureCubemap(faces);


    auto cubemap_shader = ShaderLibrary::GetShader("cubemap");
    cubemap_shader.Bind();
    cubemap_shader.Set("texture1", 0);

    auto skybox_shader = ShaderLibrary::GetShader("skybox");
    skybox_shader.Bind();
    skybox_shader.Set("skybox", 0);

    //! @note Configuring Framebuffers
    Framebuffer frame_buffer = Framebuffer(width, height);

    while (!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        frame_buffer.Bind();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (InputPoll::IsKeyPressed(KEY_ESCAPE)){
            glfwSetWindowShouldClose(window, true);
        }
        if (InputPoll::IsKeyPressed(KEY_W)){
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (InputPoll::IsKeyPressed(KEY_S)){
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (InputPoll::IsKeyPressed(KEY_A)){
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (InputPoll::IsKeyPressed(KEY_D)){
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }

        // UP
        if(InputPoll::IsKeyPressed(KEY_Q)){
            // UP
            camera.ProcessKeyboard(UP, deltaTime);
        }
        if(InputPoll::IsKeyPressed(KEY_E)){
            // DOWN
            camera.ProcessKeyboard(DOWN, deltaTime);
        }
        if(InputPoll::IsMousePressed(MOUSE_BUTTON_RIGHT)){
            double xPosIn, yPosIn;
            glfwGetCursorPos(window, &xPosIn, &yPosIn);

            float x_offset = xPosIn;
            float velocity = x_offset * deltaTime;
            camera.ProcessMouseMovement(velocity, 0.f);
        }

        if(InputPoll::IsMousePressed(MOUSE_BUTTON_LEFT)){
            double xPosIn, yPosIn;
            glfwGetCursorPos(window, &xPosIn, &yPosIn);

            float x_offset = xPosIn;
            float velocity = x_offset * deltaTime;
            camera.ProcessMouseMovement(-velocity, 0.f);
        }

        if(InputPoll::IsMousePressed(MOUSE_BUTTON_MIDDLE)){
            double xPosIn, yPosIn;
            glfwGetCursorPos(window, &xPosIn, &yPosIn);
            
            float xPos = static_cast<float>(xPosIn);
            float yPos = static_cast<float>(xPosIn);

            if (firstMouse){
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
            }

            float xoffset = xPos - lastX;
            float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

            lastX = xPos;
            lastY = yPos;

            camera.ProcessMouseMovement(xoffset, yoffset);
        }


        //! @note Rendering stuff here

        // be sure to activate shader when setting uniforms/drawing objects
        lighting_shader.Bind();
        // lighting_shader.Set("objectColor", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("light.position", lightPos);

        //! @note Just to NOTE in-case I forget
        //! @note I just made this into fmt::format so I can make changes to which light I want to use
        //! @note Such as if I want to utilize spotlight, I can change these variables names to {spotlight}.position or something like that
        //! @note This is because since I dont hvae distinguished objects from the different types of lighting
        // lighting_shader.Set(fmt::format("{}.direction", light_variable_name), camera.Front);
        // lighting_shader.Set(fmt::format("{}.position", light_variable_name), camera.Position);
        // lighting_shader.Set(fmt::format("{}.direction", light_variable_name), camera.Front);
        // lighting_shader.Set(fmt::format("{}.cut_off", light_variable_name), (float)glm::cos(glm::radians(12.5)));
        // lighting_shader.Set("viewPos", camera.Position);

        // lighting_shader.Set("point_lights[0].constant", 1.0f);
        // directional light
        lighting_shader.Set("dir_light.direction", {-0.2f, -1.0f, -0.3f});
        lighting_shader.Set("dir_light.ambient", {0.05f, 0.05f, 0.05f});
        lighting_shader.Set("dir_light.diffuse", {0.4f, 0.4f, 0.4f});
        lighting_shader.Set("dir_light.specular", {0.5f, 0.5f, 0.5f});
        // point light 1
        lighting_shader.Set("point_lights[0].position", point_light_positions[0]);
        lighting_shader.Set("point_lights[0].ambient", {0.05f, 0.05f, 0.05f});
        lighting_shader.Set("point_lights[0].diffuse", {0.8f, 0.8f, 0.8f});
        lighting_shader.Set("point_lights[0].specular", {1.0f, 1.0f, 1.0f});
        lighting_shader.Set("point_lights[0].constant", 1.0f);
        lighting_shader.Set("point_lights[0].linear", 0.09f);
        lighting_shader.Set("point_lights[0].quadratic", 0.032f);
        // point light 2
        lighting_shader.Set("point_lights[1].position", point_light_positions[1]);
        lighting_shader.Set("point_lights[1].ambient", {0.05f, 0.05f, 0.05f});
        lighting_shader.Set("point_lights[1].diffuse", {0.8f, 0.8f, 0.8f});
        lighting_shader.Set("point_lights[1].specular", {1.0f, 1.0f, 1.0f});
        lighting_shader.Set("point_lights[1].constant", 1.0f);
        lighting_shader.Set("point_lights[1].linear", 0.09f);
        lighting_shader.Set("point_lights[1].quadratic", 0.032f);
        // point light 3
        lighting_shader.Set("point_lights[2].position", point_light_positions[2]);
        lighting_shader.Set("point_lights[2].ambient", {0.05f, 0.05f, 0.05f});
        lighting_shader.Set("point_lights[2].diffuse", {0.8f, 0.8f, 0.8f});
        lighting_shader.Set("point_lights[2].specular", {1.0f, 1.0f, 1.0f});
        lighting_shader.Set("point_lights[2].constant", 1.0f);
        lighting_shader.Set("point_lights[2].linear", 0.09f);
        lighting_shader.Set("point_lights[2].quadratic", 0.032f);
        // point light 4
        lighting_shader.Set("point_lights[3].position", point_light_positions[3]);
        lighting_shader.Set("point_lights[3].ambient", {0.05f, 0.05f, 0.05f});
        lighting_shader.Set("point_lights[3].diffuse", {0.8f, 0.8f, 0.8f});
        lighting_shader.Set("point_lights[3].specular", {1.0f, 1.0f, 1.0f});
        lighting_shader.Set("point_lights[3].constant", 1.0f);
        lighting_shader.Set("point_lights[3].linear", 0.09f);
        lighting_shader.Set("point_lights[3].quadratic", 0.032f);
        // spot light
        lighting_shader.Set("spot_light.position", camera.Position);
        lighting_shader.Set("spot_light.direction", camera.Front);
        lighting_shader.Set("spot_light.ambient", {0.0f, 0.0f, 0.0f});
        lighting_shader.Set("spot_light.diffuse", {1.0f, 1.0f, 1.0f});
        lighting_shader.Set("spot_light.specular", {1.0f, 1.0f, 1.0f});
        lighting_shader.Set("spot_light.constant", 1.0f);
        lighting_shader.Set("spot_light.linear", 0.09f);
        lighting_shader.Set("spot_light.quadratic", 0.032f);
        lighting_shader.Set("spot_light.cut_off", glm::cos(glm::radians(12.5f)));
        lighting_shader.Set("spot_light.outer_cut_off", glm::cos(glm::radians(15.0f)));
        //! @note Setting the directional lighting to be set to our front camera point to the objects

        //! @note Setting our actual shader struct for the lighting-effect

        lighting_shader.Set(fmt::format("{}.ambient", light_variable_name), {0.1f, 0.1f, 0.1f});
        lighting_shader.Set(fmt::format("{}.diffuse", light_variable_name), {0.8f, 0.8f, 0.8f});
        lighting_shader.Set(fmt::format("{}.specular", light_variable_name), {1.0f, 1.0f, 1.0f});

        lighting_shader.Set(fmt::format("{}.constant", light_variable_name),  1.0f);
        lighting_shader.Set(fmt::format("{}.linear", light_variable_name),    0.09f);
        lighting_shader.Set(fmt::format("{}.quadratic", light_variable_name), 0.032f);

        // lighting_shader.Set("material.ambient", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("material.diffuse", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("material.specular", {0.5f, 0.5f, 0.5f});
        lighting_shader.Set("material.shininess", static_cast<float>(32.0f));

        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width/(float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        // -------------------------------------------
        //! @note Rendering skybox
        // -------------------------------------------
        // skybox.OnUpdate(camera, view, projection);
        // glDepthMask(GL_FALSE);
        // skybox.Bind();
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDepthMask(GL_TRUE);


        lighting_shader.Set("projection", projection);
        lighting_shader.Set("view", view);

        // world transformation
        // render cube
        glm::mat4 model = glm::mat4(1.0f);
        lighting_shader.Set("model", model);

        //! @note APPLYING TEXTURES
        //! @note If I add an editor here are a few things to add related to the diffuse and specular-added textures
        //! @note Add a toggle for enabling which parts of the textures like diffuse or specular to be toggled

        //! @note Generating a bunch of cubes
        for(uint32_t i = 0; i < 10; i++){
            glm::mat4 reset_model = glm::mat4(1.0);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lighting_shader.Set("model", model);
            
            container_diffuse.Bind();
            container_specular.Bind(1);
            Renderer::DrawQuadPrimitive(cube_vao);
        }

        // container_diffuse.Bind();
        // container_specular.Bind(1);
        // Renderer::DrawQuadPrimitive(cube_vao);

        
        //-----------------------------------------
        //! @note Drawing lamp object
        //! @note ALl rendering below are for the lighting and the lamps
        //-----------------------------------------

        cube_shader.Bind();
        cube_shader.Set("projection", projection);
        cube_shader.Set("view", view);

        for(uint32_t i = 0; i < 4; i++){
            model = glm::mat4(.5f);
            model = glm::translate(model, point_light_positions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            cube_shader.Set("model", model);
            Renderer::DrawQuadPrimitive(light_cube_vao);
        }







        //! @note Rendering skybox
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        
        skybox_shader.Bind();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        skybox_shader.Set("view", view);
        skybox_shader.Set("projection", projection);

        // skybox cube
        skybox_vao.Bind();
        cubemap_texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        frame_buffer.Unbind();

        //! @note OnUI Update. This should be done after be bind our frame buffer to our scene
        //! @note This is just a quick and easy way to getting widgets setup with imgui
        //! @note Meaning these widgets can be associated with the viewport itself.

        //! @note BeginFrame starts imgui frame for entire UI
        BeginFrame();
        DockspaceWindow(window, width, height, frame_buffer, [](){
            ImGui::Begin("Properties Panel");
            ImGui::End();
        });
        //! @note EndFrame ends per frame for entire imgui setup
        EndFrame(window, width, height);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}