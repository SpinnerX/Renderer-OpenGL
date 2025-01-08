#pragma once
#include "Shader.hpp"
#include <Renderer-OpenGL/VertexArray.hpp>
#include <Renderer-OpenGL/MeshImporter.hpp>

class AssetMesh{
public:
    AssetMesh(const std::string& p_Path);

    void Draw(Shader& shader);
private:
    MeshImporter m_Importer;
    std::vector<Mesh> m_AssetMeshHandler;
    VertexArray m_Vao;
};