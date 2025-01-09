#pragma once

#include "VertexArray.hpp"
#include <glm/fwd.hpp>
class Renderer{
public:
    static void Initialize();

    static void Begin();
    static void End();


    static void DrawQuadPrimitive(const VertexArray& p_Vao);
};