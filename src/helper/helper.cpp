#include "helper.h"

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    constexpr D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
        featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain,
        &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
    {
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags,
            featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice,
            &featureLevel, &g_pd3dDeviceContext);
    } // Try high-performance WARP software driver if hardware is not available.
    if (res != S_OK)
    {
        return false;
    }

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    releaseImages();
    CleanupRenderTarget();
    if (g_pSwapChain)
    {
        g_pSwapChain->Release();
        g_pSwapChain = nullptr;
    }
    if (g_pd3dDeviceContext)
    {
        g_pd3dDeviceContext->Release();
        g_pd3dDeviceContext = nullptr;
    }
    if (g_pd3dDevice)
    {
        g_pd3dDevice->Release();
        g_pd3dDevice = nullptr;
    }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView)
    {
        g_mainRenderTargetView->Release();
        g_mainRenderTargetView = nullptr;
    }

}

ID3D11ShaderResourceView* loadImageAsTexture(const std::string& img_path)
{
    ID3D11ShaderResourceView* textureView = nullptr;

    try
    {
        DirectX::CreateWICTextureFromFile(g_pd3dDevice, std::wstring(img_path.begin(), img_path.end()).c_str(), nullptr, &textureView);
    }
    catch (const std::exception& e)
    {
        // 打印错误消息或者进行其他类型的错误处理
        std::cerr << "An error occurred: " << e.what() << '\n';
    }
    catch (...)
    {
        // 打印通用错误消息或者进行其他类型的错误处理
        std::cerr << "An unknown error occurred while loading the texture.\n";
    }

    return textureView;
}

ID3D11ShaderResourceView* getTextureID(const std::string& img_path)
{
    // 检查贴图是否已经被加载
    auto it = textures.find(img_path);
    if (it == textures.end())
    {
        // 如果贴图未加载，则加载它
        ID3D11ShaderResourceView* pTextureView = loadImageAsTexture(img_path);
        it = textures.insert({ img_path, pTextureView }).first;
    }

    // 返回贴图对象
    return it->second;
}

void showImage(const std::string& img_path, const float& width, const float& height)
{
    auto ImTextureID = getTextureID(img_path);
    ImGui::Image(ImTextureID, ImVec2(width, height));
}

void releaseImages()
{
    for (auto& kv : textures) {
        kv.second->Release();
    }
}

bool checkAppStateByProcessName(const std::string& app)
{
    return false;
}

bool checkUserInputDir(const std::string& dir)
{
    return false;
}

std::string getDragAppDir()
{
    return std::string();
}

bool checkWeChatDefaultDir()
{
    return false;
}

bool checkWeWorkDefaultDir()
{
    return false;
}

bool checkQQDefaultDir()
{
    return false;
}

bool checkDingTalkDefaultDir()
{
    return false;
}

bool getWeChatDir()
{
    return false;
}

bool getWeWorkDir()
{
    return false;
}

bool getQQDir()
{
    return false;
}

bool getDingTalkDir()
{
    return false;
}

bool backupAudioFile(const std::string& file)
{
    return false;
}

bool restoreAudioFile(const std::string& file)
{
    return false;
}

bool replaceAudioFile(const std::string& file)
{
    return false;
}

bool replaceWeChatAudio(const std::string& file)
{
    return false;
}

