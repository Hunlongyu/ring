﻿#include "global.h"

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// 窗口管理
HWND hwnd = nullptr;
float winWidth{ 540 }; // 540 1080
float winHeight{ 360 }; // 360 720
bool showLoading{ true };
bool showHome{ true };
string loadingTxt = "|chu shi hua zhong|";
int hue{ 0 };

// 贴图资源映射
std::unordered_map<std::string, ID3D11ShaderResourceView*> textures = {};


bool hasWeChat{ false };
string WeChatDir = "";
string WeChatDLL = "";

bool hasWXWork{ false };
string WXWorkDir = "";
string WXWorkMsgAudioPath = "";
string WXWorkCallAudioPath = "";

bool hasQQ{ false };
bool hasQQNT{ false };
string QQDir = "";
string QQNTDir = "";
string QQMsgAudioPath = "";
string QQNTMsgAudioPath = "";
string QQCallAudioPath = "";
string QQNTCallAudioPath = "";

bool hasDingTalk{ false };
string DingTalkDir = "";
string DingTalkMsgAudioPath = "";
string DingTalkCallAudioPath = "";