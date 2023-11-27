#include "../global/global.h"
#include "win.h"
#include "imgui_impl_win32.h"

bool isRunAsAdmin()
{
    bool isRunAsAdmin = false;
    HANDLE hToken = nullptr;
    // 打开当前进程的访问令牌
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        // 检索指定的访问令牌信息
        TOKEN_ELEVATION elevation;
        DWORD dwSize;
        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            // TokenElevation 表示此进程是否处于提升的状态，TokenIsElevated的值为1表示已提升
            isRunAsAdmin = elevation.TokenIsElevated;
        }
    }
    if (hToken) { CloseHandle(hToken); }
    return isRunAsAdmin;
}

bool ElevateNow()
{
    bool isAdmin = false;
    try { isAdmin = isRunAsAdmin(); }
    catch (...) {
        MessageBox(nullptr, L"", L"", NULL);
        return false;
    }

    if (!isAdmin) {
        WCHAR szPath[MAX_PATH];
        if (GetModuleFileName(nullptr, szPath, ARRAYSIZE(szPath))) {
            SHELLEXECUTEINFO sei = { sizeof(sei) };
            sei.lpVerb = L"runas";// 操作，runas表示以管理员权限运行
            sei.lpFile = szPath;// 要运行的程序
            sei.hwnd = nullptr;
            sei.nShow = SW_NORMAL;

            if (!ShellExecuteEx(&sei)) {
                MessageBox(nullptr, L"软件提升管理员权限失败，请右键属性自行勾选管理员权限。", L"错误", NULL);
                return false;
            }
            exit(0);
        }
    }
    return true;
}

HWND CreateApplicationWindow()
{
    // Create application window
    ImGui_ImplWin32_EnableDpiAwareness();
    const WNDCLASSEXW wc = {
        sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
        L"gus.exe", nullptr
    };
    RegisterClassExW(&wc);

    const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    constexpr int winWidth = 1080;
    constexpr int winHeight = 800;
    const int startX = (screenWidth - winWidth) / 2;
    const int startY = (screenHeight - winHeight) / 2;

    const HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Git user switch tool", WS_OVERLAPPEDWINDOW, startX, startY,
        winWidth, winHeight, nullptr, nullptr, wc.hInstance, nullptr);

    return hwnd;
}

void UnregisterWindowClass()
{
    DestroyWindow(hwnd);
    const HINSTANCE hInstance = GetModuleHandle(nullptr);
    UnregisterClassW(L"gus.exe", hInstance);
}

// Win32 message handler
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam)) {
        return true;
    }

    switch (msg)
    {
    case WM_SIZE: {
        if (wParam == SIZE_MINIMIZED) {
            return 0;
        }
        g_ResizeWidth = static_cast<UINT>(LOWORD(lParam)); // Queue resize
        g_ResizeHeight = static_cast<UINT>(HIWORD(lParam));
        return 0;
    }
    case WM_SYSCOMMAND: {
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        return 0;
    }
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}
