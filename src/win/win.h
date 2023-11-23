#ifndef WIN_H
#define WIN_H
#pragma once

#include <Windows.h>
#include "imgui.h"

HWND CreateApplicationWindow();
void UnregisterWindowClass();

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif
