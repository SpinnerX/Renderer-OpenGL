#pragma once

#include <string>
class HDRLoader{
public:
    HDRLoader(const std::string& p_Filepath);

    bool IsLoaded() const;

    void Bind();
    void Unbind();

private:
    uint32_t m_HdrID = -1;
};