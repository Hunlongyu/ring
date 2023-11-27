#ifndef WIN_H
#define WIN_H
#pragma once
#include "../global/global.h"

// 判断是否是管理员权限启动
bool isRunAsAdmin();

// 尝试管理员权限启动
bool ElevateNow();

// 创建窗口
HWND CreateApplicationWindow();

// 销毁窗口
void UnregisterWindowClass();

// 事件管理
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
