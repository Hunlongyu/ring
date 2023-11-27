#ifndef GLOBAL_H
#define GLOBAL_H
#pragma once
#include <d3d11.h>
#include <unordered_map>
#include <string>
#include <iostream>

extern ID3D11Device* g_pd3dDevice;
extern ID3D11DeviceContext* g_pd3dDeviceContext;
extern IDXGISwapChain* g_pSwapChain;
extern UINT g_ResizeWidth;
extern UINT g_ResizeHeight;
extern ID3D11RenderTargetView* g_mainRenderTargetView;
extern HWND hwnd;

// 图片资源
extern std::unordered_map<std::string, ID3D11ShaderResourceView*> textures;

// view 视图显示控制
extern bool show_view;

// dev 视图
extern bool show_demo;
#endif

