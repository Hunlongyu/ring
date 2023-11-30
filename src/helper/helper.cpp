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
    DirectX::CreateWICTextureFromFile(g_pd3dDevice, std::wstring(img_path.begin(), img_path.end()).c_str(), nullptr, &textureView);
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

bool checkAppStateByProcessName(const string& app)
{
    wstring wapp = string_to_wstring(app);  // Convert the app name to wide character

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    PROCESSENTRY32 processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(snapshot, &processEntry))
    {
        CloseHandle(snapshot);
        return false;
    }

    do
    {
        if (!wcscmp(processEntry.szExeFile, wapp.c_str()))
        {

            if (app == "WeChat.exe") {
                hasWeChat = true;
                getWeChatDir(processEntry.th32ProcessID);
            }
            else if (app == "WXWork.exe") {
                hasWXWork = true;
                getWXWorkDir(processEntry.th32ProcessID);
            }
            else if (app == "QQ.exe") {
                getQQDir(processEntry.th32ProcessID);
            }
            else if (app == "DingTalk.exe") {
                hasDingTalk = true;
                getDingTalkDir(processEntry.th32ProcessID);
            }
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32Next(snapshot, &processEntry));

    CloseHandle(snapshot);
    return false;
}

// TODO
bool checkUserInputDir(const std::string& app, const std::string& dir)
{
    return false;
}

// TODO
std::string getDragAppDir()
{
    return std::string("");
}

string findFile(const string& dir, const string& file)
{
    namespace fs = std::filesystem;
    fs::path search_dir = dir;
    fs::file_time_type latest_time;
    fs::path latest_file;
    for (auto& p : fs::recursive_directory_iterator(search_dir)) {
        if (p.path().filename() == file) {
            auto ftime = fs::last_write_time(p);
            if (ftime > latest_time) {
                latest_time = ftime;
                latest_file = p;
            }
        }
    }
    return latest_file.string();
}

void checkFilesInit()
{
    if (!checkWeChatDefaultDir()) {
        checkAppStateByProcessName("WeChat.exe");
    }
    if (!checkWXWorkDefaultDir()) {
        checkAppStateByProcessName("WXWork.exe");
    }
    if (!checkQQDefaultDir()) {
        checkAppStateByProcessName("QQ.exe");
    }
    if (!checkDingTalkDefaultDir()) {
        checkAppStateByProcessName("DingTalk.exe");
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    //showLoading = false;
}

bool checkWeChatDefaultDir()
{
    namespace fs = std::filesystem;
    std::string path = "C:\\Program Files (x86)\\Tencent\\WeChat";
    if (std::filesystem::exists(path)) {
        hasWeChat = true;
        WeChatDir = path;
        WeChatDLL = findFile(WeChatDir, "WeChatResource.dll");
        return true;
    }
    return false;
}

bool checkWXWorkDefaultDir()
{
    namespace fs = std::filesystem;
    std::string path = "C:\\Program Files (x86)\\WXWork";
    if (std::filesystem::exists(path)) {
        hasWXWork = true;
        WXWorkDir = path;
        auto app = findFile(WXWorkDir, "WXWork.exe");
        if (app == "") {
            hasWXWork = false;
            WXWorkDir = "";
            return false;
        }
        WXWorkMsgAudioPath = findFile(WXWorkDir, "msgRemSound.wav");
        WXWorkCallAudioPath = findFile(WXWorkDir, "video_call.wav");
        return true;
    }
    return false;
}

bool checkQQDefaultDir()
{
    namespace fs = std::filesystem;
    std::string path = "C:\\Program Files (x86)\\Tencent\\QQ";
    std::string pathNT = "C:\\Program Files\\Tencent\\QQNT";
    if (std::filesystem::exists(path)) {
        hasQQ = true;
        QQDir = path;
        QQMsgAudioPath = findFile(QQDir, "msg.wav");
        QQCallAudioPath = findFile(QQDir, "Audio.wav");
        return true;
    }
    if (std::filesystem::exists(pathNT)) {
        hasQQNT = true;
        QQNTDir = path;
        QQNTMsgAudioPath = findFile(QQNTDir, "msg.wav");
        QQNTCallAudioPath = findFile(QQNTDir, "ringtone-callee.mp3");
        return true;
    }
    hasQQ = false;
    hasQQNT = false;
    return false;
    return false;
}

bool checkDingTalkDefaultDir()
{
    namespace fs = std::filesystem;
    std::string path = "C:\\Program Files (x86)\\DingDing\\main\\current_new";
    if (std::filesystem::exists(path)) {
        hasDingTalk = true;
        DingTalkDir = path;
        DingTalkMsgAudioPath = findFile(DingTalkDir, "message.wav");
        DingTalkCallAudioPath = findFile(DingTalkDir, "callNew.wav");
        return true;
    }
    return false;
}

void getWeChatDir(DWORD processID)
{
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (processHandle == NULL) return;

    char path[MAX_PATH];
    if (GetModuleFileNameExA(processHandle, NULL, path, sizeof(path) / sizeof(char)) == 0)
    {
        CloseHandle(processHandle);
        return;
    }

    CloseHandle(processHandle);
    std::string processPath(path);
    size_t pos = processPath.rfind('\\');
    if (pos != std::string::npos)
    {
        WeChatDir = processPath.substr(0, pos);
    }
    else
    {
        WeChatDir = processPath;
    }
    WeChatDLL = findFile(WeChatDir, "WeChatResource.dll");
}

void getWXWorkDir(DWORD processID)
{
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (processHandle == NULL) return;
    char path[MAX_PATH];
    if (GetModuleFileNameExA(processHandle, NULL, path, sizeof(path) / sizeof(char)) == 0)
    {
        CloseHandle(processHandle);
        return;
    }
    CloseHandle(processHandle);
    std::string processPath(path);
    size_t pos = processPath.rfind('\\');
    if (pos != std::string::npos)
    {
        WXWorkDir = processPath.substr(0, pos);
    }
    else
    {
        WXWorkDir = processPath;
    }
    WXWorkMsgAudioPath = findFile(WXWorkDir, "msgRemSound.wav");
    WXWorkCallAudioPath = findFile(WXWorkDir, "video_call.wav");
}

void getQQDir(DWORD processID)
{
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (processHandle == NULL) return;
    char path[MAX_PATH];
    if (GetModuleFileNameExA(processHandle, NULL, path, sizeof(path) / sizeof(char)) == 0)
    {
        CloseHandle(processHandle);
        return;
    }
    CloseHandle(processHandle);
    std::string processPath(path);
    size_t pos = processPath.rfind('\\');
    if (pos != std::string::npos)
    {
        QQDir = processPath.substr(0, pos);
    }
    else
    {
        QQDir = processPath;
    }
    if (QQDir.size() >= 2 && QQDir.substr(QQDir.size() - 2) == "QQ") {
        hasQQ = true;
    }
    else if (QQDir.size() >= 4 && QQDir.substr(QQDir.size() - 4) == "QQNT") {
        hasQQNT = true;
    }
    if (hasQQ) {
        QQMsgAudioPath = findFile(QQDir, "msg.wav");
        QQCallAudioPath = findFile(QQDir, "Audio.wav");
    }
    else if (hasQQNT) {
        QQMsgAudioPath = findFile(QQDir, "msg.wav");
        QQCallAudioPath = findFile(QQDir, "ringtone-callee.mp3");
    }
}

void getDingTalkDir(DWORD processID)
{
    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (processHandle == NULL) return;
    char path[MAX_PATH];
    if (GetModuleFileNameExA(processHandle, NULL, path, sizeof(path) / sizeof(char)) == 0)
    {
        CloseHandle(processHandle);
        return;
    }
    CloseHandle(processHandle);
    std::string processPath(path);
    size_t pos = processPath.rfind('\\');
    if (pos != std::string::npos)
    {
        DingTalkDir = processPath.substr(0, pos);
    }
    else
    {
        DingTalkDir = processPath;
    }
    DingTalkMsgAudioPath = findFile(DingTalkDir, "message.wav");
    DingTalkCallAudioPath = findFile(DingTalkDir, "callNew.wav");
}

// TODO
bool backupAudioFile(const std::string& file)
{
    return false;
}

// TODO
bool restoreAudioFile(const std::string& file)
{
    return false;
}

// TODO
bool replaceAudioFile(const std::string& file)
{
    return false;
}

// TODO
bool replaceWeChatAudio(const std::string& file)
{
    return false;
}

std::wstring string_to_wstring(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstr_to(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstr_to[0], size_needed);
    return wstr_to;
}

void checkUpdate()
{
    httplib::Client cli("https://api.github.com");
    auto response = cli.Get("/repos/Hunlongyu/Textify/releases/latest");
    if (response && response->status == 200) {
        // TODO
        auto body = response->body;
    }
    else {
        return;
    }
}

void AddUnderLine(ImColor col_)
{
    ImVec2 min = ImGui::GetItemRectMin();
    ImVec2 max = ImGui::GetItemRectMax();
    min.y = max.y;
    ImGui::GetWindowDrawList()->AddLine(min, max, col_, 1.0f);
    void AddUnderLine(ImColor col_);
}

void TextURL(const char* name_, const std::string& URL_, uint8_t SameLineBefore_, uint8_t SameLineAfter_)
{
    if (1 == SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
    ImGui::Text(name_);
    ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            auto url = string_to_wstring(URL_);
            ShellExecute(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOW);
        }
        AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
    }
    else
    {
        AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
    }
    if (1 == SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
}

void TextButton(const char* name_, uint8_t SameLineBefore_, uint8_t SameLineAfter_, std::function<void()> callback)
{
    if (1 == SameLineBefore_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
    ImGui::Text(name_);
    ImGui::PopStyleColor();
    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseClicked(0))
        {
            callback();
        }
        AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
    }
    else
    {
        AddUnderLine(ImGui::GetStyle().Colors[ImGuiCol_Button]);
    }
    if (1 == SameLineAfter_) { ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x); }
}

