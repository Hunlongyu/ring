#ifndef GLOBAL_H
#define GLOBAL_H
#pragma once
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "DirectXTK.lib")
#pragma comment(lib, "psapi.lib")

// 系统API
#include <windows.h>

// Directx11 API
#include <d3d11.h>
#include "WICTextureLoader.h"

// 标准 API
#include <unordered_map>
#include <string>
#include <iostream>
#include <tlhelp32.h>
#include <psapi.h>
#include <filesystem>
#include <chrono>
#include <thread>
#include <atomic>

// ImGui API
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imspinner.h"

// std_image API
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

// 自定义工具函数
#include "../helper/helper.h"

using namespace std;

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern ID3D11RenderTargetView* g_mainRenderTargetView;

// 窗口管理
extern HWND hwnd;
extern float winWidth;
extern float winHeight;
extern bool showLoading;
extern bool showHome;
extern string loadingTxt;
extern int hue;
// 图片资源
extern unordered_map<string, ID3D11ShaderResourceView*> textures;

// 微信、企业微信、QQ、钉钉 是否存在，以及他们的安装目录，音频文件路径
extern bool hasWeChat;
extern string WeChatDir;
extern string WeChatDLL;

extern bool hasWXWork;
extern string WXWorkDir;
extern string WXWorkMsgAudioPath;
extern string WXWorkCallAudioPath;

extern bool hasQQ;
extern bool hasQQNT;
extern string QQDir;
extern string QQNTDir;
extern string QQMsgAudioPath;
extern string QQNTMsgAudioPath;
extern string QQCallAudioPath;
extern string QQNTCallAudioPath;

extern bool hasDingTalk;
extern string DingTalkDir;
extern string DingTalkMsgAudioPath;
extern string DingTalkCallAudioPath;
#endif

