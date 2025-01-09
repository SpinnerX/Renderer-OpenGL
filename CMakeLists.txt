cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    src/Renderer-OpenGL/InputPoll.cpp
    src/Renderer-OpenGL/VertexBuffer.cpp
    src/Renderer-OpenGL/IndexBuffer.cpp
    src/Renderer-OpenGL/Shader.cpp
    src/Renderer-OpenGL/VertexArray.cpp
    src/Renderer-OpenGL/Framebuffer.cpp
    src/Renderer-OpenGL/stb_image.cpp
    src/Renderer-OpenGL/Image.cpp
    src/Renderer-OpenGL/Texture2D.cpp
    src/Renderer-OpenGL/BasicLightObject.cpp
    src/Renderer-OpenGL/Camera.cpp
    src/Renderer-OpenGL/Renderer.cpp
    Application.cpp
    
    PACKAGES
    glfw3
    glm
    glad
    assimp
    stb
    fmt
    imguidocking
    Vulkan
    VulkanHeaders
    
    LINK_PACKAGES
    glfw
    glm::glm
    glad::glad
    assimp::assimp
    stb::stb
    fmt::fmt
    imguidocking::imguidocking
    Vulkan::Vulkan
    vulkan-headers::vulkan-headers
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/Renderer-OpenGL)