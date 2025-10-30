#pragma once

#include "Editor.h"

#include <Windows.h>

namespace DirectXRenderer::Editor
{
class Editor;

class Win32Application
{
public:
	static int Run(Editor* editor, HINSTANCE hInstance, int nShowCmd);
protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private:
	static HWND Hwnd;
};
} // namespace DirectXRenderer::Editor
