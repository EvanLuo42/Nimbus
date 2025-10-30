#include "Editor.h"
#include "Win32Application.h"

#include <Windows.h>

_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd)
{
    DirectXRenderer::Editor::Editor editor(1280, 720, "DirectX 12 Renderer");
    return DirectXRenderer::Editor::Win32Application::Run(&editor, hInstance, nShowCmd);
}