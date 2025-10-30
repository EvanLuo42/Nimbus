#include "Editor.h"

#include <iostream>

DirectXRenderer::Editor::Editor::Editor(const int width, const int height, const char *str) : Title(str), Width(width), Height(height)  {}

void DirectXRenderer::Editor::Editor::OnInit()
{
    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
}

void DirectXRenderer::Editor::Editor::OnUpdate()
{
}

void DirectXRenderer::Editor::Editor::OnRender()
{
}

void DirectXRenderer::Editor::Editor::OnDestroy()
{
    FreeConsole();
}