#include "global.h"

ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
IDXGISwapChain* g_pSwapChain = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// 窗口管理
HWND hwnd = nullptr;
int winWidth{ 560 };
int winHeight{ 320 };
bool isOpen{ true };

// 贴图资源映射
std::unordered_map<std::string, ID3D11ShaderResourceView*> textures = {};
