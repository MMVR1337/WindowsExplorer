#include "../includes.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
        return true;
    return CallWindowProc(original_windowproc, hwnd, uMsg, wParam, lParam);
}

HRESULT present_hook(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags) {
    if (!device) {
        swapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
        device->GetImmediateContext(&immediate_context);
        ID3D11Texture2D* renderTarget = nullptr;
        swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
        device->CreateRenderTargetView(renderTarget, nullptr, &render_target_view);
        renderTarget->Release();
        ImGui::CreateContext();
        ImGui_ImplWin32_Init(game_window);
        ImGui_ImplDX11_Init(device, immediate_context);
        ImGui::StyleColorsDark();
    }

    immediate_context->OMSetRenderTargets(1, &render_target_view, nullptr);
    D3D11_VIEWPORT viewport;
    UINT vps = 1;
    immediate_context->RSGetViewports(&vps, &viewport);
    ImVec2 screen_size = { viewport.Width, viewport.Height };

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    Decoration();
    ExplorerMenu();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return present_original(swapChain, SyncInterval, Flags);
}

void Start() {

    DXGI_SWAP_CHAIN_DESC sd = {};
    ZeroMemory(&sd, sizeof sd);
    sd.BufferCount = 1;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = game_window;
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
        MessageBoxW(0, L"Failed to create D3D11 device and swap chain", L"Failure", MB_ICONERROR);
        return;
    }
    else {
        auto table = *reinterpret_cast<PVOID**>(swapChain);
        present_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT)>(table[8]);

        context->Release();
        device->Release();
        swapChain->Release();

        original_windowproc = (WNDPROC)SetWindowLongPtrW(game_window, GWLP_WNDPROC, (LONG_PTR)WndProcHook);

        // Attach hooks
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(LPVOID&)present_original, (PBYTE)present_hook);
        DetourTransactionCommit();
    }
}

void CleanupRenderTarget() {
    if (render_target_view) { render_target_view->Release(); render_target_view = NULL; }
}

void CleanupDeviceD3D() {
    CleanupRenderTarget();
    if (swapChain) { swapChain->Release(); swapChain = NULL; }
    if (context) { context->Release(); context = NULL; }
    if (device) { device->Release(); device = NULL; }
}

void main_thread(HINSTANCE hModule) {
    Start();
}

BOOL APIENTRY DllMain(HMODULE hMod, DWORD call_reason, LPVOID lpReserved) {
    if (call_reason == DLL_PROCESS_ATTACH) {
        const auto handle = CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(main_thread), hMod, 0, nullptr);
        if (handle != NULL) {
            CloseHandle(handle);
        }
    }
    else if (call_reason == DLL_PROCESS_DETACH) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
        CleanupDeviceD3D();
    }
    return TRUE;
}
