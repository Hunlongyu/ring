#ifndef HELPER_H
#define HELPER_H
#pragma once

#include "../global/global.h"

bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();

/*
* =====================================
* === 加载资源，操作资源              ===
* === 图片显示、音频播放              ===
* =====================================
*/
// 从本地加载图片
ID3D11ShaderResourceView* loadImageAsTexture(const std::string& img_path);
// 按需加载图片 
ID3D11ShaderResourceView* getTextureID(const std::string& img_path);
// 显示图片
void showImage(const std::string& img_path, const float& width, const float& height);
// 释放所有图片资源
void releaseImages();

/*
* =====================================
* === 判断电脑是否安装一下软件         ===
* === 微信、企业微信、QQ、钉钉         ===
* === 替换对应的铃声文件              ===
* =====================================
*/
// 检查指定进程名是否存在
bool checkAppStateByProcessName(const std::string& app);
// 检查用户输入的路径是否正确
bool checkUserInputDir(const std::string& dir);
// 获取用户拖拽输入的图标路径
std::string getDragAppDir();

// 检查 微信 默认安装路径
bool checkWeChatDefaultDir();
// 检查 企业微信 默认安装路径
bool checkWeWorkDefaultDir();
// 检查 QQ 默认安装路径
bool checkQQDefaultDir();
// 检查 钉钉 默认安装路径
bool checkDingTalkDefaultDir();

// 获取 微信 安装目录
bool getWeChatDir();
// 获取 企业微信 安装目录
bool getWeWorkDir();
// 获取 QQ 安装目录
bool getQQDir();
// 获取 钉钉 安装目录
bool getDingTalkDir();

// 备份指定的文件
bool backupAudioFile(const std::string& file);
// 恢复指定的文件
bool restoreAudioFile(const std::string& file);
// 替换指定的文件
bool replaceAudioFile(const std::string& file);
// 微信 替换铃声
bool replaceWeChatAudio(const std::string& file);
#endif
