#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "imgui\imgui.h"
#include "imgui\dx9\imgui_dx9.h"

#pragma warning(disable : 4244)
#pragma warning(disable : 4227)
#pragma warning(disable : 4172)
#pragma warning(disable : 4307)
#pragma warning(disable : 4996)

class UI
{
public:
	void Render(IDirect3DDevice9* pDevice);
	bool OnWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	Singleton(UI);

	void Setup(IDirect3DDevice9* pDevice);
	HANDLE hWindow = nullptr;
	bool IsSetup = false;

	bool IsOpen = false;
};