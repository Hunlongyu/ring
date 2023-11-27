#ifndef GLOBAL_H
#define GLOBAL_H
#pragma once

// 系统API
#include <windows.h>

// Directx11 API
#include <d3d11.h>
#include "WICTextureLoader.h"

// 标准 API
#include <unordered_map>
#include <string>
#include <iostream>

// ImGui API
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "imspinner.h"

// std_image API
#include "stb_image.h"
#define STB_IMAGE_IMPLEMENTATION

// 自定义函数
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
extern bool showHome;
extern bool showLoading;
extern string loadingTxt;
extern int hue;
// 图片资源
extern unordered_map<string, ID3D11ShaderResourceView*> textures;

// 微信、企业微信、QQ、钉钉 是否存在，以及他们的安装目录，音频文件路径
extern bool hasWeChat;
extern string WeChatDir;
extern string WeChatDLL;

extern bool hasWeWork;
extern string WeWorkDir;
extern string WeWorkMsgAudioPath;
extern string WeWorkCallAudioPath;

extern bool hasQQ;
extern string QQDir;
extern string QQMsgAudioPath;
extern string QQCallAudioPath;

extern bool hasDingTalk;
extern string DingTalkDir;
extern string DingTalkMsgAudioPath;
extern string DingTalkCallAudioPath;
#endif

