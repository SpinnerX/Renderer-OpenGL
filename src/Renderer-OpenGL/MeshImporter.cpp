#include "Texture.hpp"
#include <Renderer-OpenGL/IndexBuffer.hpp>
#include <Renderer-OpenGL/MeshImporter.hpp>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <fmt/core.h>
#include <glm/fwd.hpp>


MeshImporter::MeshImporter(const std::string& p_Filename){
    //! @note Importing mesh
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(p_Filename, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode){
        fmt::print("ASSIMP ERROR: Failed to load {}!\n", p_Filename);
        return;
    }

    std::string directory = p_Filename.substr(0, p_Filename.find_last_of('/'));

    bool successful = ImportMesh(scene->mRootNode, scene);

    if(!successful){
        fmt::print("FAILED TO IMPORT ASSET FILEPATH = {}\n", p_Filename);
        return;
    }
}


bool MeshImporter::ImportMesh(aiNode* root_node, const aiScene* scene){

    /*
    1.) Vertex (vertices like positions, normals, texture coordinates)
    2.) mesh faces. (retrieving corresponding indices to the mesh being imported)
    3.) material processing (specular, diffuse, normal, etc)
    4.) Returning our mesh data
    */

    for(uint32_t i = 0; i < root_node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[root_node->mMeshes[i]];
        m_MeshData.push_back(ExtractMeshDataFromAssimp(mesh, scene));
    }
    return true;
}

Mesh MeshImporter::ExtractMeshDataFromAssimp(aiMesh* p_Mesh, const aiScene* p_Scene){
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    //! @note Adding our vertices first
    for(uint32_t i = 0; i < p_Mesh->mNumVertices; i++){
        Vertex single_vertice;
        single_vertice.Position = {p_Mesh->mVertices[i].x, p_Mesh->mVertices[i].y, p_Mesh->mVertices[i].z};

        if(p_Mesh->HasNormals()){
            single_vertice.Normal = {p_Mesh->mNormals[i].x, p_Mesh->mNormals[i].y, p_Mesh->mNormals[i].z};
        }

        if(p_Mesh->mTextureCoords[0]){
            single_vertice.TexCoords = {p_Mesh->mTextureCoords[0][i].x, p_Mesh->mTextureCoords[0][i].y};

            single_vertice.Tangent = {p_Mesh->mTangents[i].x, p_Mesh->mTangents[i].y, p_Mesh->mTangents[i].z};

            single_vertice.Bitangent = {p_Mesh->mBitangents[i].x, p_Mesh->mBitangents[i].y, p_Mesh->mBitangents[i].z};
        }
        else{
            single_vertice.TexCoords = {0.f, 0.f};
        }

        vertices.push_back(single_vertice);
    }

    //! @note Getting all of our indices by iterating through the faces of our mesh
    for(uint32_t i = 0; i < p_Mesh->mNumFaces; i++){
        aiFace face = p_Mesh->mFaces[i];
        for(uint32_t j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[i]);
        }
    }

    VertexBuffer vbo(vertices);
    IndexBuffer ibo(indices);

    //! @note Processing materials
    aiMaterial* material = p_Scene->mMaterials[p_Mesh->mMaterialIndex];

    std::vector<Texture> diffuse_textures = LoadMaterials(material, aiTextureType_DIFFUSE);
    textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());

    std::vector<Texture> specular_specular_texturestextures = LoadMaterials(material, aiTextureType_SPECULAR);
    textures.insert(textures.end(), diffuse_textures.begin(), diffuse_textures.end());

    std::vector<Texture> height_textures = LoadMaterials(material, aiTextureType_HEIGHT);
    textures.insert(textures.end(), height_textures.begin(), height_textures.end());

    std::vector<Texture> ambient_textures = LoadMaterials(material, aiTextureType_AMBIENT);
    textures.insert(textures.end(), ambient_textures.begin(), ambient_textures.end());

    return Mesh(vbo, ibo, textures);
}

std::vector<Texture> MeshImporter::LoadMaterials(aiMaterial *material, aiTextureType texture_t){
    std::vector<Texture> textures;
    TextureType type;
    if(texture_t == aiTextureType_DIFFUSE){
        type = TextureType::DIFFUSE;
    }
    else if(texture_t == aiTextureType_SPECULAR){
        type = TextureType::SPECULAR;
    }
    else if(texture_t == aiTextureType_HEIGHT){
        type = TextureType::HEIGHT;
    }
    else if(texture_t == aiTextureType_AMBIENT){
        type = TextureType::AMBIENT;
    }

    for(uint32_t i = 0; i < material->GetTextureCount(texture_t); i++){
        aiString str;
        material->GetTexture(texture_t, i, &str);
        bool skip = false;

        for(uint32_t j = 0; j < m_TexturesLoaded.size(); j++){
            if(std::strcmp(m_TexturesLoaded[j].GetPath().data(), str.C_Str()) == 0){
                textures.push_back(m_TexturesLoaded[j]);
                skip = true;
                break;
            }
        }

        if(!skip){
            Texture new_texture(str.C_Str(), type);
            textures.push_back(new_texture);
            m_TexturesLoaded.push_back(new_texture);
        }
    }
    return textures;
}