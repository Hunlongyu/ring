#ifndef HELPER_H
#define HELPER_H
#pragma once

#include "../global/global.h"

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

// 从本地加载图片
ID3D11ShaderResourceView* loadImageAsTexture(const std::string& img_path);
// 按需加载图片 
ID3D11ShaderResourceView* getTextureID(const std::string& img_path);
// 显示图片
void showImage(const std::string& img_path, const float& width, const float& height);
// 释放所有图片资源
void releaseImages();
#endif
