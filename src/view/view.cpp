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

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
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
        const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
        static int item_current_idx = 0;
        ImGui::Text("Full-width:");
        if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
        {
            for (int n = 0; n < IM_ARRAYSIZE(items); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(items[n], is_selected))
                {
                    item_current_idx = n;
                }

                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndListBox();
        }
    }

    {
        // showImage("assets/images/test.png", 30, 30);
    }

    ImGui::Text("This is view");

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
        const char* cstr = loadingTxt.c_str();
        const ImVec2 textSize = ImGui::CalcTextSize(cstr);
        const float center_x = (winWidth - textSize.x) / 2;
        const float center_y = winHeight / 2 + 25;
        ImGui::SetCursorPos(ImVec2(center_x, center_y));
        ImGui::Text(cstr);
    }

    // 取消初始化按钮，点击后退出软件
    {
        ImGui::SetCursorPos(ImVec2(249, 300));
        if (ImGui::Button("Close")) {
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