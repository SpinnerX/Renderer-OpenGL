#pragma once
#include <string>

/**

    @note GltfImport will use tinygltf to import models (both meshes and primitives this way)
    - Since currently we are only taking in raw vertices 
*/


class GltfImporter{
public:
    GltfImporter(const std::string& p_Filepath);


private:
    std::string m_Filepath;
};