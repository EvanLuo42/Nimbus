#pragma once
#include <string>

class CommandList
{
public:
    void SetRenderTarget(const std::string& name);
    void DrawScene();
};
