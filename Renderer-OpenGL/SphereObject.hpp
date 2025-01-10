#pragma once
#include <string>

class SphereObject{
public:
    SphereObject(const std::string& Tag);

    void Bind();

    void Unbind();
};