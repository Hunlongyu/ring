#include "global/global.h"

#include "view/view.h"
#include "win/win.h"

int APIENTRY WinMain(HINSTANCE hCurrentInstance, HINSTANCE hPreviousInstance, LPSTR lpcmdline, int nCmdShow)
{
    hwnd = CreateApplicationWindow();

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        UnregisterWindowClass();
        return 1;
    }

    // Show the window
    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    // 初始化界面 UI
    viewInit(hwnd);

    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (isOpen && msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // views code here
        viewUI();

        // Rendering
        ImGui::Render();
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        constexpr float clearColor[4] = { 0.00f, 0.00f, 0.00f, 1.00f }; // RGBA
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clearColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
        g_pSwapChain->Present(1, 0); // Present with vsync
    }

    // Cleanup
    viewDestroy();
    CleanupDeviceD3D();
    UnregisterWindowClass();
    return 0;
}
