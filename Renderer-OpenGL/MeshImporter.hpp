#pragma once
// #include "IndexBuffer.hpp"
// #include "Texture2D.hpp"
#include <Renderer-OpenGL/IndexBuffer.hpp>
#include <Renderer-OpenGL/Texture2D.hpp>
#include <Renderer-OpenGL/VertexBuffer.hpp>
#include <Renderer-OpenGL/VertexArray.hpp>

class MeshImport{
public:
    MeshImport(const std::string& p_Filepath);

    bool IsLoaded() const { return m_DidMeshImportSuccessfully; }


private:

private:
    bool m_DidMeshImportSuccessfully = false;
    VertexBuffer m_MeshVbo;
    IndexBuffer m_Ibo;
    VertexArray m_Vao;

    std::vector<Texture2D> m_Textures;
};