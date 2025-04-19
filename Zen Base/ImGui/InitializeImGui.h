static void InitializeImGui();
#include "ImGui/imgui_impl_dx9.h"
D3DPRESENT_PARAMETERS d3dpp;
static HWND Window = NULL;
IDirect3D9Ex* p_Object = NULL;
IDirect3DDevice9Ex* D3dDevice = NULL;

static void InitializeImGui()
{
    if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
        exit(3);

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.BackBufferWidth = Width;
    d3dpp.BackBufferHeight = Height;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.hDeviceWindow = Window;
    d3dpp.Windowed = TRUE;
    d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

    if (FAILED(p_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, 0, &D3dDevice)))
    {
        p_Object->Release();
        exit(4);
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplWin32_Init(Window);
    ImGui_ImplDX9_Init(D3dDevice);

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 16.f);
    io.IniFilename = 0;
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;
    style->WindowBorderSize = 1;
    style->FrameBorderSize = 1;

    ImGui::StyleColorsDark();
    p_Object->Release();
}