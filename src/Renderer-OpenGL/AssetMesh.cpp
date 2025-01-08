#include <Renderer-OpenGL/AssetMesh.hpp>


AssetMesh::AssetMesh(const std::string& p_Path){
    m_Importer = MeshImporter(p_Path);
    m_AssetMeshHandler = m_Importer.GetMeshes();
}

void AssetMesh::Draw(Shader& shader){
    shader.Bind();
    
}