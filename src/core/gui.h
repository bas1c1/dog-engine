#ifndef DOG_ENGINE_GUI
#define DOG_ENGINE_GUI
#pragma once

#include <d3d9.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx9.h>
#include <imgui/imgui_impl_win32.h>
#include "definitions.h"

namespace dgui {
	constexpr int _WIDTH = 500;
	constexpr int _HEIGHT = 800;

	static bool isRunning = true;

	static HWND window = nullptr;
	static WNDCLASSEX windowClass = { };

	static POINTS position = { };

	static PDIRECT3D9 d3d = nullptr;
	static LPDIRECT3DDEVICE9 device = nullptr;
	static D3DPRESENT_PARAMETERS presentParameters = { };

	void CreateHWindow(char* windowName) noexcept;
	void DestroyHWindow() noexcept;

	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
}

#endif