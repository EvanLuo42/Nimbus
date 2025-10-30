#pragma once

#include "Win32Application.h"

#include <string>

#include <Windows.h>

namespace DirectXRenderer::Editor
{
class Editor
{
  public:
    ~Editor() = default;
    Editor(int width, int height, const char *str);

    void OnInit();
    void OnUpdate();
    void OnRender();
    void OnDestroy();

    [[nodiscard]] const WCHAR *GetTitle() const
    {
        return reinterpret_cast<const WCHAR *>(Title.c_str());
    }

    [[nodiscard]] UINT GetWidth() const
    {
        return Width;
    }
    [[nodiscard]] UINT GetHeight() const
    {
        return Height;
    }

  private:
    std::string Title;
    int Width, Height;
};
} // namespace DirectXRenderer::Editor