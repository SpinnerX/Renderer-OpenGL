#include <Renderer-OpenGL/InputPoll.hpp>
#include "InputPoll.hpp"

static GLFWwindow* s_Window = nullptr;

void InputPoll::Initialize(GLFWwindow* Window){
    s_Window = Window;
}

bool InputPoll::IsKeyPressed(KeyCode keycode){
    auto state = glfwGetKey(s_Window, static_cast<int32_t>(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool InputPoll::IsMousePressed(MouseCode mouseCode){
    auto state = glfwGetMouseButton(s_Window, static_cast<int32_t>(mouseCode));
    return state == GLFW_PRESS;
}

glm::vec2 InputPoll::GetMousePosition(){
    double xPos, yPos;
    glfwGetCursorPos(s_Window, &xPos, &yPos);

    return {xPos, yPos};
}

float InputPoll::GetMouseX(){
    return GetMousePosition().x;
}

float InputPoll::GetMouseY(){
    return GetMousePosition().y;
}

void InputPoll::UpdateEvents(){
    glfwPollEvents();
}