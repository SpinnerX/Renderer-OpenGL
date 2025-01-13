#pragma once

#include "EnvironmentMap.hpp"
#include "VertexArray.hpp"
#include <glm/fwd.hpp>
class Renderer{
public:
    static void Initialize();

    static void Begin(Camera& camera);
    static void End();

    static void SetViewport(uint32_t Width, uint32_t Height);

    // static void DrawPointLight();
    static void DrawQuadPrimitive(const VertexArray& p_Vao);
    static void DrawEnvironmentMap(EnvironmentMap& p_Map, glm::mat4& projection, uint32_t width, uint32_t height);
};