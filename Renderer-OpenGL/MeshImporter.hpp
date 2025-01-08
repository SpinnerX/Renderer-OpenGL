#pragma once
#include <Renderer-OpenGL/VertexBuffer.hpp>
#include <Renderer-OpenGL/IndexBuffer.hpp>
#include <Renderer-OpenGL/Texture.hpp>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Mesh{
    VertexBuffer VertexBufferHandler;
    IndexBuffer IndexBufferHandler;
    std::vector<Texture> Textures;
};

class MeshImporter{
public:
    MeshImporter() = default;
    MeshImporter(const std::string& p_Filename);

    bool ImportMesh(aiNode* root_node, const aiScene* scene);


    Mesh ExtractMeshDataFromAssimp(aiMesh* p_Node, const aiScene* p_Scene);

    std::vector<Mesh> GetMeshes() { return m_MeshData; }

private:
    std::vector<Texture> LoadMaterials(aiMaterial *mat, aiTextureType type);
private:
    std::vector<Mesh> m_MeshData;
    std::vector<Texture> m_TexturesLoaded; // also used for checking if the texture is loaded already.
};