#include "Texture2D.hpp"
#include <Renderer-OpenGL/MeshImporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fmt/core.h>

MeshImport::MeshImport(const std::string& p_Filepath){
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(p_Filepath, aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        fmt::print("ASSIMP ERROR: GetErrorString() = {}\n", importer.GetErrorString());
        return;
    }

    std::string directory = p_Filepath.substr(0, p_Filepath.find_last_of("/"));
    fmt::print("Directory = {}\n", directory);

    // processing all of our nodes
    aiNode* node = scene->mRootNode;

    for(uint32_t i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        //! @note Processing all meshes
        Vertex each_mesh_vertex;

        each_mesh_vertex.Position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};

        //! @note We are checking if the asset being loaded has normals, texture coordinates, and faces (to get the indices from)
        if(mesh->HasNormals()){
            each_mesh_vertex.Normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
        }

        if(mesh->mTextureCoords[0]){
            each_mesh_vertex.TexCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        }
        else{
            each_mesh_vertex.TexCoords = {0.f, 0.f};
        }

        //! @note Fetching faces for indices
        for(uint32_t i = 0; i < mesh->mNumFaces; i++){
            aiFace face = mesh->mFaces[i];

            for(uint32_t j = 0; j < face.mNumIndices; j++){
                indices.push_back(face.mIndices[i]);
            }
        }

        //! @note Now loading textures materials and textures
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // m_Textures = LoadTextureFromFile()
        aiTextureType texture_type = aiTextureType_DIFFUSE;
        
        // Diffuse
        for(uint32_t i = 0; i < material->GetTextureCount(texture_type); i++){
            aiString str;
            material->GetTexture(texture_type, i, &str);

            Texture2D texture = Texture2D(str.C_Str());
            m_Textures.push_back(texture);
        }

        // specular
        texture_type = aiTextureType_SPECULAR;
        for(uint32_t i = 0; i < material->GetTextureCount(texture_type); i++){
            aiString str;
            material->GetTexture(texture_type, i, &str);

            Texture2D texture = Texture2D(str.C_Str());
            m_Textures.push_back(texture);
        }

        //! @note Normal
        texture_type = aiTextureType_NORMALS;
        for(uint32_t i = 0; i < material->GetTextureCount(texture_type); i++){
            aiString str;
            material->GetTexture(texture_type, i, &str);

            Texture2D texture = Texture2D(str.C_Str());
            m_Textures.push_back(texture);
        }

        texture_type = aiTextureType_HEIGHT;
        for(uint32_t i = 0; i < material->GetTextureCount(texture_type); i++){
            aiString str;
            material->GetTexture(texture_type, i, &str);

            Texture2D texture = Texture2D(str.C_Str());
            m_Textures.push_back(texture);
        }

        
    }


    m_MeshVbo = VertexBuffer(vertices);
    m_Ibo = IndexBuffer();
    m_Vao = VertexArray(m_MeshVbo, m_Ibo);
}