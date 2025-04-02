// gui.h

#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx9.h"

#include <d3d9.h>
#include <tchar.h>
#include <windows.h>

namespace gui
{
    // constant window size
    constexpr int WIDTH = 400;
    constexpr int HEIGHT = 500;

    inline bool exit = true;

    // winapi window class
    inline HWND window = nullptr;
    inline WNDCLASSEXW windowClass = { };

    // points for window movement
    inline POINTS position = { };

    //direct x state vars
    inline PDIRECT3D9 d3d = nullptr;
    inline LPDIRECT3DDEVICE9 device = nullptr;
    inline D3DPRESENT_PARAMETERS presentParameters = { };

    // handle window creation & destruction
	void CreateHWindow(
		const wchar_t* windowName,
		const wchar_t* className) noexcept;
	void DestroyHWindow() noexcept;

	// handle device creation & destruction
	bool CreateDevice() noexcept;
	void ResetDevice() noexcept;
	void DestroyDevice() noexcept;

	// handle ImGUI creation & destruction
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;

}
