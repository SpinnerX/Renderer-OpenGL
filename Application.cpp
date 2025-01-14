#include "Renderer-OpenGL/Components.hpp"
#include "Renderer-OpenGL/EnvironmentMap.hpp"
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

#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <Renderer-OpenGL/Framebuffer.hpp>
// #include <imgui.h>
#include <imgui.h>
#include <span>
#include <Renderer-OpenGL/ModelTutorial.hpp>
#include <imgui.h>

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
    // glViewport(0, 0, width, height);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable){
        GLFWwindow* backup_ctx = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();

        glfwMakeContextCurrent(Window);
    }
}

void DockspaceWindow(GLFWwindow* window, int Width, int Height, Framebuffer& frame_buffer, const std::function<void()>& p_UpdateUI){

    bool dockspace_open = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    if(opt_fullscreen){
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if(dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode){
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("Dockspace Demo", &dockspace_open, window_flags);
    ImGui::PopStyleVar();

    if(opt_fullscreen){
        ImGui::PopStyleVar(2);
    }

    // Dockspace
    ImGuiIO& io = ImGui::GetIO();
    if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable){
        ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.f, 0.f), dockspace_flags);
    }

    if(ImGui::BeginMenuBar()){
        if(ImGui::MenuItem("Exit")){
            glfwSetWindowShouldClose(window, true);
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Viewport");
    auto fb_id = frame_buffer.GetColorAttachmentID();
    ImGui::Image((void*)fb_id, ImVec2(Width, Height));
    ImGui::End();

    ImGui::Begin("Panels");
    p_UpdateUI();
    ImGui::End();

    ImGui::End();
}

namespace ImGui{
    //! @note We need to see if this works
    //! @note Because imgui implements this but the header file for some reason does not contain this implementation...
    void PushMultiItemsWidths(int components, float w_full);
};

static void DrawVec3UI(const std::string& Tag, glm::vec3& Position, float reset_value = 0.f){
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushID(Tag.c_str());

    float columnWidth = 100.0f;

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", Tag.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    
    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

    if(ImGui::Button("X", buttonSize)){
        Position.x = reset_value;
        // ImGui::End();
    }

    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &Position.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();


    // Setting up for the Y button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

    if(ImGui::Button("Y", buttonSize)){
        Position.y = reset_value;
        // ImGui::End();
    }

    // ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &Position.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    // Setting up for the Z button
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
    if(ImGui::Button("Z", buttonSize)){
        Position.z = reset_value;
        // ImGui::End();
    }

    // ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &Position.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    
    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();



}

static void DrawFloatUI(const std::string& Tag, float& value, float reset_value=0.f){
    ImGuiIO& io = ImGui::GetIO();
    ImGui::PushID(Tag.c_str());

    float columnWidth = 100.0f;

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, columnWidth);
    ImGui::Text("%s", Tag.c_str());
    ImGui::NextColumn();

    ImGui::PushItemWidth(ImGui::CalcItemWidth());
    // ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

    if(ImGui::Button("R_X")){
        value = reset_value;
        // ImGui::End();
    }

    // ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &value, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();
}

/*

Parameters
T = is the type the component we want to draw to the UI
UFunction = callback that defines what data in the component to be displayed in this panel that handles the UI layout of that component

USAGE:

DrawPanelCompoent<Component>("Transform", [](){
    DrawVec3("Position", SomePosition);
    // etc....
});

*/

template<typename T, typename UFunction>
static void DrawPanelComponent(const std::string& Tag, const UFunction& p_UFunction){
    const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

    ImVec2 contentRegion = ImGui::GetContentRegionAvail();
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4,4});

    float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
    ImGui::Separator();

    bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, "%s", Tag.c_str());
    ImGui::PopStyleVar();

    ImGui::SameLine(contentRegion.x - lineHeight * 0.05f);

    if(ImGui::Button("+", ImVec2(lineHeight, lineHeight))){
        ImGui::OpenPopup("ComponentSettings");
    }

    bool isRemovedComponent = false; // @note for deferring when to delete component.
    if(ImGui::BeginPopup("ComponentSettings")){
        if(ImGui::MenuItem("Remove Component"))
            isRemovedComponent = true;

        ImGui::EndPopup();
    }

    if(opened){
        p_UFunction();
        ImGui::TreePop();
    }

}


//! @note These are just to initialize a few objects within our scene
//! @note Typically this would be done in their own respective mesh classes (or something like that)
void InitializeCube(VertexArray& vao){

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

    VertexBuffer vbo = VertexBuffer(vertices);
    vao = VertexArray(&vbo);
}


void InitializePlatform(VertexArray& vao){
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float planeVertices[] = {
        // positions            // normals         // texcoords
         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
    };

    VertexBuffer platform_vbo = VertexBuffer(planeVertices);
    vao = VertexArray(&platform_vbo);
}

//! @note Span is the amounf of textures needed to be read
void UpdateCube(const glm::vec3& Position, VertexArray& cube_vao, Shader& cube_shader, std::span<Texture2D> p_Textures){
    glm::mat4 model = glm::mat4(1.f);
    cube_shader.Set("model", model);

    //! @note Calling this cube1
    cube_shader.Bind();
    model = glm::translate(model, Position);
    cube_shader.Set("model", model);
    // container_diffuse.Bind();
    // container_specular.Bind(1);
    // for(Texture2D t : p_Textures){
    //     t.Bind();
    // }
    for(uint32_t i = 0; i < p_Textures.size(); i++){
        p_Textures[i].Bind(i);
    }

    Renderer::DrawQuadPrimitive(cube_vao);
    for(uint32_t i = 0; i < p_Textures.size(); i++){
        p_Textures[i].Unbind();
    }
    // container_diffuse.Unbind();
    // container_specular.Unbind();
    
    cube_shader.Unbind();
}

void UpdatePlatform(const glm::vec3& Position, VertexArray& platform_vao, Shader& platform_shader, std::span<Texture2D> p_Textures){
    platform_shader.Bind();
    glm::mat4 model = glm::mat4(1.0f);
    platform_shader.Set("model", model);
    // platform_texture.Bind();
    for(uint32_t i = 0; i < p_Textures.size(); i++){
        p_Textures[i].Bind(i);
    }

    model = glm::mat4(1.0f);
    model = glm::translate(model, Position);
    platform_shader.Set("model", model);
    Renderer::DrawQuadPrimitive(platform_vao);

    for(uint32_t i = 0; i < p_Textures.size(); i++){
        p_Textures[i].Unbind();
    }
    platform_shader.Unbind();
    
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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        // std::cout << "Failed to initialize GLAD" << std::endl;
        fmt::print("GLAD ERROR: Failed to initialize GLAD!\n");
        return -1;
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

    // Setting custom dark themed to imgui
    ImGuiLayoutColorModification();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");
    // stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader lighting_shader(
        "shaders/tutorials/basic_multiple_lights_4/multiple_lights.vs", 
        "shaders/tutorials/basic_multiple_lights_4/multiple_lights.fs"
    );

    Shader cube_shader(
        "shaders/model_loading/basic_cube.vs",
        "shaders/model_loading/basic_cube.fs"
    );

    // library.Add(Shader());


    // VertexArray cube_vao = VertexArray(&vbo);
    VertexArray cube_vao;
    InitializeCube(cube_vao);
    cube_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT3, "aNormal", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    glm::vec3 cube1_position = {0.f, 0.f, 0.f};

    //! @note How to get the current vao that is binded (used for debugging)
    // int current_vao;
    // glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);

    //! @note NOTE TO SELF: IF the cube or primitive does not look right
    //! @note These vertex attributes have to align to the layouts within the glsl
    //! @note Layouts like layout(location = 0), layout(location = 1), etc.
    //! @note Uncomment this...
    // VertexArray light_cube_vao;
    // InitializeCube(light_cube_vao);
    // light_cube_vao.SetVertexAttribute({
    //     {VertexAttributeType::FLOAT3, "aPos", false},
    //     {VertexAttributeType::FLOAT3, "aNormal", false},
    //     {VertexAttributeType::FLOAT2, "aTexCoords", false}
    // });

    // PointLight my_pointlight = PointLight("Point light 1");

    Texture2D container_diffuse = Texture2D("assets/container_diffuse.png");
    Texture2D container_specular = Texture2D("assets/container_specular.png");
    std::array<Texture2D, 2> container_textures = {
        container_diffuse,
        container_specular
    };
    
    lighting_shader.Bind();
    lighting_shader.Set("material.diffuse", 0);
    lighting_shader.Set("material.specular", 1);
    lighting_shader.Set("material.emission", 2);
    // lighting_shader.Unbind();

    // lighting
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    std::string light_variable_name = "light";

    glm::vec3 point_light_positions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    glm::vec3 point_light_position = {0.f, 0.f, 0.f};


    //! @note Setting up platform object
    VertexArray platform_vao;
    InitializePlatform(platform_vao);
    platform_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT3, "aNormal", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    Texture2D platform_texture = Texture2D("assets/wood.png", true);
    std::array<Texture2D, 1> platform_container_textures = {
        platform_texture
    };

    glm::vec3 platform_position = {0.0f, 1.10f, 0.0f};

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

    library.Add(
        Shader("shaders/tutorials/basic_multiple_lights_4/multiple_lights.vs", 
        "shaders/tutorials/basic_multiple_lights_4/multiple_lights.fs"
        )
    );
    library.Add(
        Shader(
        "shaders/model_loading/basic_cube.vs",
        "shaders/model_loading/basic_cube.fs"
        )
    );


    //! @note Loading 3D model
    // Model test_model("assets/backpack/backpack.obj");
    std::array<Texture2D, 6> texture_mappings = {
        Texture2D("assets/robo-pose/textures/Texture_1K.jpg"),
        Texture2D("assets/robo-pose/textures/LP_BodyNormalsMap_1K.jpg"),
        Texture2D("assets/robo-pose/textures/specular.jpeg"),
        Texture2D("assets/robo-pose/textures/diffuse.jpeg"),
    };
    Model test_model("assets/robo-pose/source/robot-pose.obj", true, texture_mappings);

    // -------------------------------------------
    //! @note Skybox/Cubemap Configurations!
    // -------------------------------------------
    
    //! @note Loading skybox faces
    std::array<std::string, 6> faces = {
        "assets/skybox/right.jpg",
        "assets/skybox/left.jpg",
        "assets/skybox/top.jpg",
        "assets/skybox/bottom.jpg",
        "assets/skybox/front.jpg",
        "assets/skybox/back.jpg"
    };
    {
    /*
    VertexArray cubemap_vao;
    InitializeCubemap(cubemap_vao);
    cubemap_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false}
    });

    VertexArray skybox_vao;
    InitializeSkybox(skybox_vao);
    skybox_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPos", false}
    });

    //! @note Setting up cubemap shaders to use with textures loaded
    TextureCubemap cubemap_texture = TextureCubemap(faces);
    auto cubemap_shader = ShaderLibrary::GetShader("cubemap");
    cubemap_shader.Bind();
    cubemap_shader.Set("skybox", 0);

    auto skybox_shader = ShaderLibrary::GetShader("skybox");
    skybox_shader.Bind();
    skybox_shader.Set("skybox", 0);
    */
    }

    // EnvironmentMap environment_map = EnvironmentMap(faces);

    library.Add(
        Shader(
        "shaders/actual_shaders/environment/hdr.vs", 
        "shaders/actual_shaders/environment/hdr.fs"
        )
    );

    //! @note Rather then loading a skybox, we are going to load .hdri maps instead for our environments
    EnvironmentMap environment_map = EnvironmentMap("assets/environments/newport_loft.hdr", true);

    //! @note TODO: Configuring Framebuffer attachments
    //! @note Having multiple frame buffers for handling shadow-passes, geometry-passes, etc. For post-processing effects
    // FramebufferAttachments attachments = {
    //     {
    //         FramebufferAttachmentType::DEPTH
    //     }
    // };
    // Framebuffer frame_buffer = Framebuffer(width, height, attachments);
    Framebuffer frame_buffer = Framebuffer(width, height);
    // 3d model properties
    glm::vec3 model_position = {0.f, 0.50f, 0.f};
    glm::vec3 model_scale = {.5f, .5f, .5f};
    // float model_rotation_angle = 45.0f;
    glm::vec3 model_rotation = {0.f, 0.f, 1.f};
    float model_rotation_angle = 176.80f;
    // float model_rotation_angle = 0.f;
    // glm::vec3 model_rotate = {0.f, 0.f, 0.f};


    //! @note Testing geometry
    // VertexBuffer vbo = VertexBuffer()
    // VertexArray geometry_vao = VertexArray();
    Shader geometry_shader = Shader("shaders/geometry.vs", "shaders/geometry.fs");
    VertexArray geometry_vao;
    InitializeCube(geometry_vao);

    Texture2D geometry_texture = Texture2D("assets/wood.png");

    glm::vec3 geometry_position = {0.f, 0.f, 0.f};

    geometry_vao.SetVertexAttribute({
        {VertexAttributeType::FLOAT3, "aPosition", false},
        {VertexAttributeType::FLOAT4, "aColor", false},
        {VertexAttributeType::FLOAT2, "aTexCoords", false},
        {VertexAttributeType::FLOAT3, "aNormal", false},
    });

    std::array<Texture2D, 6> model2_textures = {
        Texture2D("assets/blue-archive-kaiser-pmc-military-robots/textures/mat1_c.jpg")
    };
    Model test_model2("assets/blue-archive-kaiser-pmc-military-robots/source/Kaiser PMC Military Robots Blue Archive.obj", true, model2_textures);

    // model2 properties
    glm::vec3 model_position2 = {8.50, 0.50, 7.60};
    glm::vec3 model_scale2 = {0.3, 0.3, 0.3};
    glm::vec3 model_rotation2 = {0.f, 0.f, 1.f};



    std::array<Texture2D, 7> model3_textures = {
        Texture2D("assets/mule_robot/textures/boot3_specularGlossiness.png"),
        Texture2D("assets/mule_robot/textures/boot3_diffuse.jpeg"),
        Texture2D("assets/mule_robot/textures/boot3_normal.jpeg"),
        Texture2D("assets/mule_robot/textures/mateshadows_diffuse.png"),
        Texture2D("assets/mule_robot/textures/boot3_emissive.jpeg"),
        Texture2D("assets/mule_robot/textures/boot3_occlusion.png"),
    };

    // Model test_model3("assets/mule_robot/scene.gltf", true, model3_textures);
    Model test_model3("dont worry about this not loading...");
    
    glm::vec3 model_position3 = {8.50, 0.50, 7.60};
    glm::vec3 model_scale3 = {0.9, 0.9, 0.9};
    glm::vec3 model_rotation3 = {0.f, 0.f, 1.f};
    
    
    
    float camera_sensitivity = 5.f;
    float camera_mouse_sensitivity = 0.1;


    //! @note Creating PointLight object
    // PointLight my_pointlight = PointLight("Point light 1");
    // my_pointlight.SetPosition(glm::vec3( 2.3f, -3.3f, -4.0f));
    glm::vec3 pointlight_pos_test = glm::vec3( 2.3f, -3.3f, -4.0f);
    glm::vec3 pointlight_scale_test = {.2f, .2f, .2f};
    glm::vec3 pointlight_rotation_test = {0.f, 0.f, 0.f};

    Renderer::SetViewport(width, height);

    Shader hdr_shader = library.Get("hdr");
    hdr_shader.Set("equirectangularMap", 0);

    //! @note Lighting Properties
    DirectionLight dir_light;
    SpotLight spot_light;
    spot_light.cut_off = glm::cos(glm::radians(12.5f));
    spot_light.outer_cut_off = glm::cos(glm::radians(15.0f));

    std::array<glm::vec2, 2> viewportBounds;
    //! @note TO ENABLE ANTI_ALIASING
    // glEnable(GL_MULTISAMPLE);
    while (!glfwWindowShouldClose(window)){
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glm::vec2 pos = InputPoll::GetMousePosition();

        frame_buffer.Bind();
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        frame_buffer.ClearFramebuffer(1, -1);
        Renderer::Begin(camera);

        //! @note Processing Event Inputs
        {
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
        if(InputPoll::IsKeyPressed(KEY_Q)){
            // UP
            camera.ProcessKeyboard(UP, deltaTime);
        }
        if(InputPoll::IsKeyPressed(KEY_E)){
            // DOWN
            camera.ProcessKeyboard(DOWN, deltaTime);
        }

        //! @note Press shift key to move using the mouse to rotate around
        if(InputPoll::IsKeyPressed(KEY_LEFT_SHIFT)){
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
                float velocity = yPosIn * deltaTime;
                camera.ProcessMouseMovement(0.f, velocity);
            }

            if(InputPoll::IsKeyPressed(KEY_SPACE)){
                double xPosIn, yPosIn;
                glfwGetCursorPos(window, &xPosIn, &yPosIn);
                float velocity = yPosIn * deltaTime;
                camera.ProcessMouseMovement(0.f, -velocity);
            }


        }

        if(InputPoll::IsMousePressed(MOUSE_BUTTON_MIDDLE)){
            double xPosIn, yPosIn;
            glfwGetCursorPos(window, &xPosIn, &yPosIn);
            
            float xPos = static_cast<float>(xPosIn) * deltaTime;
            float yPos = static_cast<float>(xPosIn) * deltaTime;

            if (firstMouse){
                lastX = xPos;
                lastY = yPos;
                firstMouse = false;
            }

            float xoffset = xPos - lastX;
            float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

            lastX = xPos;
            lastY = yPos;

            float newYPOS = yPos * deltaTime;

            camera.ProcessMouseMovement(0.f, newYPOS);
        }
        }


        //! @note Rendering stuff here

        // be sure to activate shader when setting uniforms/drawing objects
        {
        lighting_shader.Bind();

        // directional light
        lighting_shader.Set("dir_light.direction", dir_light.Direction);
        lighting_shader.Set("dir_light.ambient", dir_light.ambient);
        lighting_shader.Set("dir_light.diffuse", dir_light.diffuse);
        lighting_shader.Set("dir_light.specular", dir_light.specular);
        
        // testing single point light
        lighting_shader.Set("pointLightCount", 0);
        lighting_shader.Set("directionLightCount", 1);
        lighting_shader.Set("spotLightCount", 1);
        // lighting_shader.Set("point_lights.position", point_light_positions[1]);
        // lighting_shader.Set("point_light.ambient", {0.05f, 0.05f, 0.05f});
        // lighting_shader.Set("point_light.diffuse", {0.8f, 0.8f, 0.8f});
        // lighting_shader.Set("point_light.specular", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("point_light.constant", 1.0f);
        // lighting_shader.Set("point_light.linear", 0.09f);
        // lighting_shader.Set("point_light.quadratic", 0.032f);

        // point light 1
        // lighting_shader.Set("point_lights[0].position", point_light_positions[0]);
        // lighting_shader.Set("point_lights[0].ambient", {0.05f, 0.05f, 0.05f});
        // lighting_shader.Set("point_lights[0].diffuse", {0.8f, 0.8f, 0.8f});
        // lighting_shader.Set("point_lights[0].specular", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("point_lights[0].constant", 1.0f);
        // lighting_shader.Set("point_lights[0].linear", 0.09f);
        // lighting_shader.Set("point_lights[0].quadratic", 0.032f);

        // point light 2
        // lighting_shader.Set("point_lights[1].position", point_light_positions[1]);
        // lighting_shader.Set("point_lights[1].ambient", {0.05f, 0.05f, 0.05f});
        // lighting_shader.Set("point_lights[1].diffuse", {0.8f, 0.8f, 0.8f});
        // lighting_shader.Set("point_lights[1].specular", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("point_lights[1].constant", 1.0f);
        // lighting_shader.Set("point_lights[1].linear", 0.09f);
        // lighting_shader.Set("point_lights[1].quadratic", 0.032f);
        // // // point light 3
        // lighting_shader.Set("point_lights[2].position", point_light_positions[2]);
        // lighting_shader.Set("point_lights[2].ambient", {0.05f, 0.05f, 0.05f});
        // lighting_shader.Set("point_lights[2].diffuse", {0.8f, 0.8f, 0.8f});
        // lighting_shader.Set("point_lights[2].specular", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("point_lights[2].constant", 1.0f);
        // lighting_shader.Set("point_lights[2].linear", 0.09f);
        // lighting_shader.Set("point_lights[2].quadratic", 0.032f);
        // // // point light 4
        // lighting_shader.Set("point_lights[3].position", point_light_positions[3]);
        // lighting_shader.Set("point_lights[3].ambient", {0.05f, 0.05f, 0.05f});
        // lighting_shader.Set("point_lights[3].diffuse", {0.8f, 0.8f, 0.8f});
        // lighting_shader.Set("point_lights[3].specular", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("point_lights[3].constant", 1.0f);
        // lighting_shader.Set("point_lights[3].linear", 0.09f);
        // lighting_shader.Set("point_lights[3].quadratic", 0.032f);
        // spot light
        // Unused: Keeping it here for now...
        // lighting_shader.Set("spot_light.direction", camera.Front);
        //! @note TOOD: Make a SpotLight class that contains these properties for easier of use
        lighting_shader.Set("spot_light.position", camera.Position);
        lighting_shader.Set("spot_light.direction", spot_light.Direction);
        lighting_shader.Set("spot_light.ambient", spot_light.ambient);
        lighting_shader.Set("spot_light.diffuse", spot_light.diffuse);
        lighting_shader.Set("spot_light.specular", spot_light.specular);
        lighting_shader.Set("spot_light.constant", spot_light.constant);
        lighting_shader.Set("spot_light.linear", spot_light.linear);
        lighting_shader.Set("spot_light.quadratic", spot_light.quadratic);
        
        // lighting_shader.Set("spot_light.direction", {1.f, 0.f, 0.f});
        // lighting_shader.Set("spot_light.ambient", {0.0f, 0.0f, 0.0f});
        // lighting_shader.Set("spot_light.diffuse", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("spot_light.specular", {1.0f, 1.0f, 1.0f});
        // lighting_shader.Set("spot_light.constant", 1.0f);
        // lighting_shader.Set("spot_light.linear", 0.09f);
        // lighting_shader.Set("spot_light.quadratic", 0.032f);
        lighting_shader.Set("spot_light.cut_off", spot_light.cut_off);
        lighting_shader.Set("spot_light.outer_cut_off", spot_light.outer_cut_off);

        // lighting_shader.Set(fmt::format("{}.ambient", l

        // lighting_shader.Set("material.ambient", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("material.diffuse", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("material.specular", {0.5f, 0.5f, 0.5f});
        lighting_shader.Set("material.shininess", static_cast<float>(32.0f));
        }
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width/(float)height, 0.1f, 100.0f);
        // glm::mat4 projection = glm::ortho(0.0f, 1000.0f, 0.0f, 1000.0f, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();

        hdr_shader.Set("projection", projection);


        lighting_shader.Set("projection", projection);
        lighting_shader.Set("view", view);

        // world transformations
        // render cube
        glm::mat4 model = glm::mat4(1.0f);
        lighting_shader.Set("model", model);


       //! @note Testing geometry cube
        geometry_shader.Bind();
        // geometry_texture.Bind();
        model = glm::mat4(1.0f);
        model = glm::translate(model, geometry_position);
        // geometry_shader.Set("view", view);
        // geometry_shader.Set("projection", projection);

        geometry_shader.Set("model", model);
        // geometry_shader.Set("color", glm::vec4(1.0f, 0.f, 0.f, 1.f));
        Renderer::DrawQuadPrimitive(geometry_vao);
        // geometry_texture.Unbind();
        geometry_shader.Unbind();

        //! @note This updates, renders, and modifies the cube within our current scene
        //! @note For now just to make things easier on me eyes
        // lighting_shader.Bind();
        // lighting_shader.Set("color", glm::vec4{1.f, 1.f, 1.f, 1.f});
        UpdateCube(cube1_position, cube_vao, lighting_shader, container_textures);
        //! @note Rendering platform
        UpdatePlatform(platform_position, platform_vao, lighting_shader, platform_container_textures);




        //-----------------------------------------
        //! @note Drawing lamp object
        //! @note ALl rendering below are for the lighting and the lamps
        //-----------------------------------------

        //! @note We only bind to the data we want.
        //! @note In this case its only the lamp objects that we want to write our shader data to the GPU
        cube_shader.Bind();

        //! @note Drawing our model
        lighting_shader.Bind();
        test_model.Draw(lighting_shader, model_position, model_scale, model_rotation, model_rotation_angle);
        lighting_shader.Unbind();

        //! @note drawing model2
        lighting_shader.Bind();
        test_model2.Draw(lighting_shader, model_position2, model_scale2, model_rotation2, model_rotation_angle);
        lighting_shader.Unbind();

        lighting_shader.Bind();
        test_model3.Draw(lighting_shader, model_position3, model_scale3, model_rotation3, model_rotation_angle);
        lighting_shader.Unbind();
        
        // -------------------------------------------
        //! @note Rendering skybox
        //! @note drawing cubemap stuff
        // -------------------------------------------
        Renderer::DrawEnvironmentMap(environment_map, projection, width, height);


        Renderer::End();

        frame_buffer.Unbind();

        //! @note BeginFrame starts imgui frame for entire UI
        BeginFrame();


        // DockspaceWindow(window, width, height, frame_buffer, [&platform_position, &cube1_position, &point_light_positions, &model_position, &model_scale, &model_rotation_angle, &model_rotation, &test_model, &geometry_position, &model_position2, &model_scale2, &model_rotation2, &camera, &camera_sensitivity, &camera_mouse_sensitivity, &my_pointlight, &pointlight_pos_test, &pointlight_scale_test, &pointlight_rotation_test](){
        // DockspaceWindow(window, width, height, frame_buffer, [&spot_light, &dir_light, &platform_position, &cube1_position, &point_light_positions, &model_position, &model_scale, &model_rotation_angle, &model_rotation, &test_model, &geometry_position, &model_position2, &model_scale2, &model_rotation2, &camera, &camera_sensitivity, &camera_mouse_sensitivity, &model_position3, &model_scale3, &model_rotation3](){
        DockspaceWindow(window, width, height, frame_buffer, [&](){
            ImGui::Begin("Properties Panel");
            auto contentRegion = ImGui::GetContentRegionAvail();

            // float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f; // @note calculating height of button
            float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y * 2.0f;
            
            
            //! @note TODO: Eventually have this be easily modifyable by not specifying the name of transforms they are associated with
            //! @note Rather just call transform and make this better

            DrawPanelComponent<Model>("Scene Property", [&](){
                DrawVec3UI("platform", platform_position);
                DrawVec3UI("cube1", cube1_position);
                DrawFloatUI("camera sensitivity", camera_sensitivity);
                DrawFloatUI("camera mouse sensitivity", camera_mouse_sensitivity);
            });

            DrawPanelComponent<Model>("Model 1", [&](){
                DrawVec3UI("model", model_position);
                DrawVec3UI("model scale", model_scale);
                DrawVec3UI("model rotation", model_rotation);
                DrawFloatUI("model angle", model_rotation_angle);
            });


            // MODEL2 PROPERTIES
            DrawPanelComponent<Model>("Model 2", [&](){
                DrawVec3UI("model2 pos", model_position2);
                DrawVec3UI("model2 scale", model_scale2);
                DrawVec3UI("model2 rotation", model_rotation2);
            });

            DrawPanelComponent<Model>("Model 3", [&](){
                DrawVec3UI("model3 pos", model_position3);
                DrawVec3UI("model3 scale", model_scale3);
                DrawVec3UI("model3 rotation", model_rotation3);
            });

            // // SPOT LIGHT STUFF
            DrawPanelComponent<Model>("Spot Light", [&](){
                DrawVec3UI("Spot Light Position", spot_light.Position);
                DrawVec3UI("Spot Light Direction", spot_light.Direction);
                DrawVec3UI("Spot Light Ambient", spot_light.ambient);
                DrawVec3UI("Spot Light Diffuse", spot_light.diffuse);
                DrawVec3UI("Spot Light Specular", spot_light.specular);
            });

            DrawPanelComponent<Model>("Direction Light 1", [&](){
                DrawVec3UI("Direction", dir_light.Direction);
                DrawVec3UI("Diffuse", dir_light.diffuse);
                DrawVec3UI("Specular", dir_light.specular);
                DrawVec3UI("Ambient", dir_light.ambient);
            });

            ImGui::End();

            camera.SetCameraMovementSpeed(camera_sensitivity);
            camera.SetCameraMouseSpeed(camera_mouse_sensitivity);
        });

        //! @note EndFrame ends per frame for entire imgui setup
        EndFrame(window, width, height);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}