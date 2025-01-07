cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    src/Renderer-OpenGL/VertexBuffer.cpp
    src/Renderer-OpenGL/IndexBuffer.cpp
    src/Renderer-OpenGL/Shader.cpp
    src/Renderer-OpenGL/VertexArray.cpp
    src/Renderer-OpenGL/Image.cpp
    src/Renderer-OpenGL/Texture2D.cpp
    Application.cpp
    
    PACKAGES
    glfw3
    glm
    glad
    stb
    fmt
    imguidocking
    Vulkan
    VulkanHeaders
    
    LINK_PACKAGES
    glfw
    glm::glm
    glad::glad
    stb::stb
    fmt::fmt
    imguidocking::imguidocking
    Vulkan::Vulkan
    vulkan-headers::vulkan-headers
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/Renderer-OpenGL)