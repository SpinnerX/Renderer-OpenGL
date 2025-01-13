#include "Renderer-OpenGL/EnvironmentMap.hpp"
#include "Renderer-OpenGL/PointLight.hpp"
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
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8, 0.1f, 0.15f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2, 0.1f, 0.2f, 1.0f});

    if(ImGui::Button("X")){
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

    if(ImGui::Button("Y")){
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
    if(ImGui::Button("Z")){
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

static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    // glViewport(0, 0, width, height);
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

/*
void InitializeCubemap(VertexArray& vao){
    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    VertexBuffer cubemap_vbo = VertexBuffer(cubeVertices);
    vao = VertexArray(&cubemap_vbo);
}

void InitializeSkybox(VertexArray& vao){
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

    VertexBuffer skybox_vbo = VertexBuffer(skyboxVertices);
    vao = VertexArray(&skybox_vbo);
}
*/

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

void UpdateAndRenderLightLamps(const glm::vec3& Position, VertexArray& lamp_vao, Shader& shader, std::span<Texture2D> p_Textures){
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

    PointLight my_pointlight = PointLight("Point light 1");

    Texture2D container_diffuse = Texture2D("assets/container_diffuse.png");
    Texture2D container_specular = Texture2D("assets/container_specular.png");
    std::array<Texture2D, 2> container_textures = {
        container_diffuse,
        container_specular
    };
    
    lighting_shader.Bind();
    lighting_shader.Set("material.diffuse", 0);
    lighting_shader.Set("material.specular", 1);
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

   EnvironmentMap environment_map = EnvironmentMap(faces);

    //! @note Configuring Framebuffers
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

    // Model test_model2("assets/backpack/backpack.obj");
    std::array<Texture2D, 2> texture2 = {
        Texture2D("assets/obj-nat-rock-01/textures/diffuse.jpeg"),
        Texture2D("assets/obj-nat-rock-01/textures/normal.jpeg")
    };
    // Model test_model2("assets/obj-nat-rock-01/source/rock.obj", true, texture2);
    // Model test_model2("assets/XArmour.fbx");
    Model test_model2("assets/Bloom.obj");
    // std::array<Texture2D, 6> texture_mappings2 = {
    //     Texture2D("assets/robo-pose/textures/Texture_1K.jpg"),
    //     Texture2D("assets/robo-pose/textures/LP_BodyNormalsMap_1K.jpg"),
    //     Texture2D("assets/robo-pose/textures/specular.jpeg"),
    //     Texture2D("assets/robo-pose/textures/diffuse.jpeg"),
    // };
    // Model test_model2("assets/robo-pose/source/robot-pose.obj", true, texture_mappings2);

    glm::vec3 model_position2 = {8.50f, 0.50f, 0.f};
    glm::vec3 model_scale2 = {.1f, .1f, .1f};
    glm::vec3 model_rotation2 = {0.f, 0.f, 1.f};
    float camera_sensitivity = 5.f;
    float camera_mouse_sensitivity = 0.1;


    //! @note Creating PointLight object
    // PointLight my_pointlight = PointLight("Point light 1");
    // my_pointlight.SetPosition(glm::vec3( 2.3f, -3.3f, -4.0f));
    glm::vec3 pointlight_pos_test = glm::vec3( 2.3f, -3.3f, -4.0f);
    glm::vec3 pointlight_scale_test = {.2f, .2f, .2f};
    glm::vec3 pointlight_rotation_test = {0.f, 0.f, 0.f};

    Renderer::SetViewport(width, height);


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
        lighting_shader.Set("dir_light.direction", {-0.2f, -1.0f, -0.3f});
        lighting_shader.Set("dir_light.ambient", {0.05f, 0.05f, 0.05f});
        lighting_shader.Set("dir_light.diffuse", {0.4f, 0.4f, 0.4f});
        lighting_shader.Set("dir_light.specular", {0.5f, 0.5f, 0.5f});
        
        // testing single point light
        lighting_shader.Set("pointLightCount", 0);
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
        //! @note TOOD: Make a SpotLight class that contains these properties for easier of use
        lighting_shader.Set("spot_light.position", camera.Position);
        // lighting_shader.Set("spot_light.direction", camera.Front);
        lighting_shader.Set("spot_light.direction", {1.f, 0.f, 0.f});
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

        // lighting_shader.Set(fmt::format("{}.ambient", light_variable_name), {0.1f, 0.1f, 0.1f});
        // lighting_shader.Set(fmt::format("{}.diffuse", light_variable_name), {0.8f, 0.8f, 0.8f});
        // lighting_shader.Set(fmt::format("{}.specular", light_variable_name), {1.0f, 1.0f, 1.0f});

        // lighting_shader.Set(fmt::format("{}.constant", light_variable_name),  1.0f);
        // lighting_shader.Set(fmt::format("{}.linear", light_variable_name),    0.09f);
        // lighting_shader.Set(fmt::format("{}.quadratic", light_variable_name), 0.032f);

        // lighting_shader.Set("material.ambient", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("material.diffuse", {1.0f, 0.5f, 0.31f});
        // lighting_shader.Set("material.specular", {0.5f, 0.5f, 0.5f});
        lighting_shader.Set("material.shininess", static_cast<float>(32.0f));
        }
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)width/(float)height, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();


        lighting_shader.Set("projection", projection);
        lighting_shader.Set("view", view);

        // world transformations
        // render cube
        glm::mat4 model = glm::mat4(1.0f);
        lighting_shader.Set("model", model);

        //! @note APPLYING TEXTURES
        //! @note If I add an editor here are a few things to add related to the diffuse and specular-added textures
        //! @note Add a toggle for enabling which parts of the textures like diffuse or specular to be toggled

        //! @note Generating a bunch of cubes
        /*
        for(uint32_t i = 0; i < 10; i++){
            glm::mat4 reset_model = glm::mat4(1.0);
            model = glm::translate(model, cube_positions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            lighting_shader.Bind();
            lighting_shader.Set("model", model);
            //! @note IF you want specific objects to show reflections.
            //! @note YOU must specifically bind those specific shaders to those objects.
            //! @note In this example I choose to set our cubes to use reflections off of the cubemap
            // cubemap_shader.Bind();
            // cubemap_shader.Set("model", model);
            container_diffuse.Bind();
            container_specular.Bind(1);
            Renderer::DrawQuadPrimitive(cube_vao);
            container_diffuse.Unbind();
            container_specular.Unbind();
        }
        */

        //! @note Calling this cube1
        /*
        lighting_shader.Bind();
        model = glm::translate(model, cube1_position);
        lighting_shader.Set("model", model);
        container_diffuse.Bind();
        container_specular.Bind(1);
        Renderer::DrawQuadPrimitive(cube_vao);
        container_diffuse.Unbind();
        container_specular.Unbind();
        lighting_shader.Unbind();
        */

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
        {
        /*
        lighting_shader.Bind();
        platform_texture.Bind();
        // lighting_shader.Set("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, platform_position);
        // lighting_shader.Set("projection", projection);
        lighting_shader.Set("model", model);
        Renderer::DrawQuadPrimitive(platform_vao);
        lighting_shader.Unbind();
        */
        }
        //! @note Rendering platform
        UpdatePlatform(platform_position, platform_vao, lighting_shader, platform_container_textures);




        //-----------------------------------------
        //! @note Drawing lamp object
        //! @note ALl rendering below are for the lighting and the lamps
        //-----------------------------------------

        //! @note We only bind to the data we want.
        //! @note In this case its only the lamp objects that we want to write our shader data to the GPU
        cube_shader.Bind();
        // cube_shader.Set("cameraPos", camera.Position);
        {
        // ! @note Creating multiple light objects
        // for(uint32_t i = 0; i < 4; i++){
        //     model = glm::mat4(.5f);
        //     model = glm::translate(model, point_light_positions[i]);
        //     model = glm::scale(model, glm::vec3(0.2f));
        //     cube_shader.Set("model", model);
        //     Renderer::DrawQuadPrimitive(light_cube_vao);
        // }

        // lamp 1
        // model = glm::mat4(.5f);
        // model = translate(model, point_light_positions[0]);
        // model = glm::scale(model, glm::vec3(0.2f));
        // lighting_shader.Set("model", model);
        // Renderer::DrawQuadPrimitive(light_cube_vao);
        // lighting_shader.Unbind();


        // // lamp 2
        //! @note This is the one that we want to see since this one is more visible!
        /*
        lighting_shader.Bind();
        model = glm::mat4(.5f);
        model = translate(model, point_light_positions[1]);
        model = glm::scale(model, glm::vec3(0.2f));
        lighting_shader.Set("model", model);
        Renderer::DrawQuadPrimitive(light_cube_vao);
        lighting_shader.Unbind();
        */
        //! @note Customizing my point light creation
        // my_pointlight.OnUpdate(camera, projection);

        // // lamp 3
        // lighting_shader.Bind();
        // model = glm::mat4(.5f);
        // model = translate(model, point_light_positions[2]);
        // model = glm::scale(model, glm::vec3(0.2f));
        // lighting_shader.Set("model", model);
        // Renderer::DrawQuadPrimitive(light_cube_vao);
        // lighting_shader.Bind();

        // // lamp 4
        // lighting_shader.Bind();
        // model = glm::mat4(.5f);
        // model = translate(model, point_light_positions[3]);
        // model = glm::scale(model, glm::vec3(0.2f));
        // lighting_shader.Set("model", model);
        // Renderer::DrawQuadPrimitive(light_cube_vao);
        // lighting_shader.Unbind();
        }

        //! @note Drawing our model
        lighting_shader.Bind();
        test_model.Draw(lighting_shader, model_position, model_scale, model_rotation, model_rotation_angle);
        lighting_shader.Unbind();

        //! @note drawing model2
        lighting_shader.Bind();
        test_model2.Draw(lighting_shader, model_position2, model_scale2, model_rotation2, model_rotation_angle);
        lighting_shader.Unbind();
        
        // -------------------------------------------
        //! @note Rendering skybox
        //! @note drawing cubemap stuff
        // -------------------------------------------
        {
        /*
        cubemap_shader.Bind();
        cubemap_shader.Set("model", model);
        cubemap_shader.Set("view", view);
        cubemap_shader.Set("projection", projection);
        cubemap_shader.Set("cameraPos", camera.Position);
        cube_vao.Bind();
        cubemap_texture.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 36);
        cube_vao.Unbind();
        */

        // glDrawArrays(GL_TRIANGLES, 0, 36);

        //! @note Rendering skybox
        /*
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
        skybox_vao.Unbind();
        */
        }

        Renderer::DrawEnvironmentMap(environment_map, projection, width, height);


        Renderer::End();

        frame_buffer.Unbind();

        //! @note OnUI Update. This should be done after be bind our frame buffer to our scene
        //! @note This is just a quick and easy way to getting widgets setup with imgui
        //! @note Meaning these widgets can be associated with the viewport itself.

        //! @note BeginFrame starts imgui frame for entire UI
        BeginFrame();


        // DockspaceWindow(window, width, height, frame_buffer, [&platform_position, &cube1_position, &point_light_positions, &model_position, &model_scale, &model_rotation_angle, &model_rotation, &test_model, &geometry_position, &model_position2, &model_scale2, &model_rotation2, &camera, &camera_sensitivity, &camera_mouse_sensitivity, &my_pointlight, &pointlight_pos_test, &pointlight_scale_test, &pointlight_rotation_test](){
        DockspaceWindow(window, width, height, frame_buffer, [&platform_position, &cube1_position, &point_light_positions, &model_position, &model_scale, &model_rotation_angle, &model_rotation, &test_model, &geometry_position, &model_position2, &model_scale2, &model_rotation2, &camera, &camera_sensitivity, &camera_mouse_sensitivity](){
            // ImGui::Begin("Properties Panel");
            //! @note These transform panels are for messing around with various objects
            //! @note Eventually I'll make an attempt at actually having "proper" scene management system in place for making life-easier, but as of right now not the point of this project
            // ImGui::Begin("Transforms Panels");
            
            ImGui::Begin("Platform Properties");
            
            
            //! @note TODO: Eventually have this be easily modifyable by not specifying the name of transforms they are associated with
            //! @note Rather just call transform and make this better
            DrawVec3UI("platform", platform_position);
            DrawVec3UI("cube1", cube1_position);
            DrawVec3UI("light pos 1", point_light_positions[0]);
            DrawVec3UI("light pos 2", point_light_positions[1]);
            DrawVec3UI("light pos 3", point_light_positions[2]);
            DrawVec3UI("light pos 4", point_light_positions[3]);
            DrawVec3UI("model", model_position);
            DrawVec3UI("model scale", model_scale);
            DrawVec3UI("model rotation", model_rotation);
            DrawFloatUI("model angle", model_rotation_angle);
            DrawVec3UI("geometry", geometry_position);

            DrawVec3UI("model2 pos", model_position2);
            DrawVec3UI("model2 scale", model_scale2);
            DrawVec3UI("model2 rotation", model_rotation2);
            DrawFloatUI("camera sensitivity", camera_sensitivity);
            DrawFloatUI("camera mouse sensitivity", camera_mouse_sensitivity);

            DrawVec3UI("geometry pos", geometry_position);
            // DrawVec3UI("pl pos 1", pointlight_pos_test);
            // DrawVec3UI("pl scale 1", pointlight_scale_test);
            // DrawVec3UI("pl rotate 1", pointlight_rotation_test);

            camera.SetCameraMovementSpeed(camera_sensitivity);
            camera.SetCameraMouseSpeed(camera_mouse_sensitivity);

            // my_pointlight.SetPosition(pointlight_pos_test);
            // my_pointlight.SetScale(pointlight_scale_test);
            // my_pointlight.SetRotation(pointlight_rotation_test);


            // Begin drag-drop sources
            ImGui::BeginDragDropSource();
            std::string path = "assets/";
            ImGui::SetDragDropPayload("BROWSE_ITEM",path.c_str(), path.size());
            ImGui::EndDragDropSource();

            ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

            ImGui::BeginDragDropTarget();
            if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")){
                const char* filepath = (const char*)payload->Data;
                std::string texturePath = std::string(fmt::format("{} / {}", path, filepath));
                test_model.OnReload(texturePath);

                fmt::print("Model loaded path during rumtime was = {}\n", texturePath);
            }
            ImGui::EndDragDropTarget();


            ImGui::End();

            ImGui::End();
        });


        // auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		// auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		// // @note If tab bar is expanded, then the cursor will be expanded
		// auto viewportOffset = ImGui::GetWindowPos();

		// viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
		// viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

        // auto[mouseX, mouseY] = ImGui::GetMousePos();
        // mouseX -= viewportBounds[0].x;
        // mouseY -= viewportBounds[0].y;

        // glm::vec2 viewportSize = viewportBounds[1] - viewportBounds[0];
		// mouseY = viewportSize.y - mouseY; // This makes our bottom left (0, 0)
		// int current_mouse_x = (int)mouseX;
		// int current_mouse_y = (int)mouseY;
        // int pixel = frame_buffer.Read(1, current_mouse_x, current_mouse_y);
        // fmt::print("Framebuffer Read Pixel = {}\n", pixel);

        // fmt::print("camera sensitivity = {}\n", camera_sensitivity);
        // fmt::print("new camera sensitivity set = {}\n", camera.GetCameraSensitivity());
        //! @note EndFrame ends per frame for entire imgui setup
        EndFrame(window, width, height);



        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}