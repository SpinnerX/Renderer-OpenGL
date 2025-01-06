cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    src/Renderer-OpenGL/VertexBuffer.cpp
    Application.cpp
    
    PACKAGES glfw3 glad fmt imguidocking Vulkan VulkanHeaders
    LINK_PACKAGES glfw glad::glad fmt::fmt imguidocking::imguidocking Vulkan::Vulkan vulkan-headers::vulkan-headers
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/Renderer-OpenGL)