#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_internal.h"

#include <d3d11.h>
#include <tchar.h>
#include <filesystem>
#include <vector>
#include <string>
#include <stack>
#include <windows.h>
#include <winnt.h>
#include <minwindef.h>


#include "Hooks/detours/detours.h"


#pragma comment (lib, "d3d11.lib")


static ID3D11Device* device = NULL;
static ID3D11DeviceContext* context = NULL;
static IDXGISwapChain* swapChain = NULL;
static ID3D11RenderTargetView* render_target_view = NULL;
static ID3D11DeviceContext* immediate_context = NULL;
static WNDPROC original_windowproc = nullptr;

HWND game_window = NULL;

HRESULT __stdcall present_hook(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags);
HRESULT(__fastcall* present_original)(IDXGISwapChain*, UINT, UINT) = nullptr;
HRESULT(__fastcall* resize_original)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT) = nullptr;

namespace fs = std::filesystem;

#include "menu/expmenu.h"