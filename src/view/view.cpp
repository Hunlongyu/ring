#include "view.h"
void viewInit(const HWND hwnd)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc", 18.0f, nullptr, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
}

void viewDestroy()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void homeUI()
{
    ImGuiWindowFlags win_flags = 0;
    win_flags |= ImGuiWindowFlags_NoMove;
    win_flags |= ImGuiWindowFlags_NoResize;
    win_flags |= ImGuiWindowFlags_NoCollapse;
    win_flags |= ImGuiWindowFlags_NoSavedSettings;
    win_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);

    ImGui::Begin("Ring", &showHome, win_flags);

    {
        ImGui::SetCursorPos(ImVec2(30, 70));
        ImGui::Text("选择程序");
        ImGui::SameLine();
        const char* items[] = { "微信", "企业微信", "QQ", "钉钉" };
        ImGui::SetNextItemWidth(240);
        ImGui::Combo("##sApp", &sApp, items, IM_ARRAYSIZE(items));
    }

    {
        ImGui::SetCursorPos(ImVec2(30, 120));
        ImGui::Text("消息铃声");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(240);
        ImGui::InputText("##msgDir", msgDir, IM_ARRAYSIZE(msgDir), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("选择")) {
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("播放")) {
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("替换")) {
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("还原")) {
            // TODO
        }
    }

    {
        ImGui::SetCursorPos(ImVec2(30, 170));
        ImGui::Text("来电铃声");
        ImGui::SameLine();
        ImGui::SetNextItemWidth(240);
        ImGui::InputText("##callDir", callDir, IM_ARRAYSIZE(callDir), ImGuiInputTextFlags_ReadOnly);
        ImGui::SameLine();
        if (ImGui::Button("选择")) {
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("播放")) {
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("替换")) {
            // TODO
        }
        ImGui::SameLine();
        if (ImGui::Button("还原")) {
            // TODO
        }
    }

    {
        ImGui::SetCursorPos(ImVec2(30, 250));
        TextButton("v1.0.0", 0, 1, checkUpdate);
        ImGui::SetItemTooltip("点击检查更新");
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(20.0f, 0.0f));
        ImGui::SameLine();
        TextURL("Hunlongyu", "https://github.com/Hunlongyu", 1, 1);
        ImGui::SetItemTooltip("开发者，欢迎大家点点关注~");
        ImGui::Dummy(ImVec2(20.0f, 0.0f));
        TextURL("GitHub", "https://github.com/Hunlongyu/ring",1 , 1);
        ImGui::SetItemTooltip("软件官网，动动发财的小手，帮忙点点 star ~");
    }

    ImGui::End();
}

void loadingUI() {
    if (!showLoading) return;
    ImGuiWindowFlags win_flags = 0;
    win_flags |= ImGuiWindowFlags_NoMove;
    win_flags |= ImGuiWindowFlags_NoResize;
    win_flags |= ImGuiWindowFlags_NoTitleBar;
    win_flags |= ImGuiWindowFlags_NoCollapse;
    win_flags |= ImGuiWindowFlags_NoSavedSettings;
    win_flags |= ImGuiWindowFlags_NoScrollWithMouse;
    ImGui::SetNextWindowSize(ImVec2(winWidth, winHeight), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Once);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 1));
    ImGui::Begin("Loading", &showLoading, win_flags);
    ImGui::PopStyleColor();

    // Loading 动画
    {
        const float center_x = winWidth / 2 - 37;
        const float center_y = winHeight / 2 - 45;
        ImGui::SetCursorPos(ImVec2(center_x, center_y));
        ImSpinner::SpinnerBarsScaleMiddle("SpinnerBarsScaleMiddle", 8, ImColor::HSV(++hue * 0.005f, 0.8f, 0.8f), 6, 4);
    }

    // 初始化描述文字
    {
        const char* cstr = "初始化...";
        const ImVec2 textSize = ImGui::CalcTextSize(cstr);
        const float center_x = (winWidth - textSize.x) / 2;
        const float center_y = winHeight / 2 + 25;
        ImGui::SetCursorPos(ImVec2(center_x, center_y));
        ImGui::Text(cstr);
    }

    // 取消初始化按钮，点击后退出软件
    {
        ImGui::SetCursorPos(ImVec2(249, 230));
        if (ImGui::Button("关闭")) {
            showHome = false;
        }
    }

    {
        static bool once{ false };
        if (!once) {
            once = true;
            std::thread fnThread(checkFilesInit);
            fnThread.detach();
        }
    }

    ImGui::End();
}