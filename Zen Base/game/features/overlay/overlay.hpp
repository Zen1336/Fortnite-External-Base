#include "../../../Imgui/InitializeImGui.h"

LPDIRECT3D9 DirectX = NULL;
LPDIRECT3DDEVICE9 DirectX_device = NULL;
LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

D3DPRESENT_PARAMETERS DirectX_paramters = {};


RECT GameRect = { 0 };

static void xCreateWindow();


LRESULT WINAPI WndProc(HWND hWindow, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
        return 0;
    }
    else
    {
        ImGui_ImplWin32_WndProcHandler(hWindow, msg, wParam, lParam);
        return DefWindowProc(hWindow, msg, wParam, lParam);
    }
}

HRESULT CreateDeviceD3D(HWND hWindow)
{
    if ((DirectX = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)  return E_FAIL;
    RtlZeroMemory(&DirectX_paramters, sizeof(DirectX_paramters));
 
    DirectX_paramters.Windowed = TRUE;
    DirectX_paramters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    DirectX_paramters.hDeviceWindow = hWindow;
    DirectX_paramters.MultiSampleQuality = D3DMULTISAMPLE_NONE;
    DirectX_paramters.BackBufferFormat = D3DFMT_A8R8G8B8;
    DirectX_paramters.EnableAutoDepthStencil = TRUE;
    DirectX_paramters.AutoDepthStencilFormat = D3DFMT_D16;
    DirectX_paramters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

    if (DirectX->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWindow, D3DCREATE_HARDWARE_VERTEXPROCESSING, &DirectX_paramters, &DirectX_device) < 0)   return E_FAIL;
   
    return S_OK;
}

void xCreateWindow()
{
    if (hwnd)
    {
        ZeroMemory(&GameRect, sizeof(GameRect));
        GetWindowRect(hwnd, &GameRect);
        Width = GameRect.right - GameRect.left;
        Height = GameRect.bottom - GameRect.top;
        DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

        if (dwStyle & WS_BORDER)
        {
            GameRect.top += 32;
            Height -= 39;
        }
        settings::screen_center_x = Width / 2;
        settings::screen_center_y = Height / 2;
        MoveWindow(Window, GameRect.left, GameRect.top, Width, Height, true);
    }

    WNDCLASSEX WindowClass = {
        sizeof(WNDCLASSEX),
        CS_CLASSDC,
        WndProc,
        0,
        0,
        GetModuleHandle(NULL),
        NULL,
        NULL,
        NULL,
        NULL,
        "WindowClass",
        NULL
    };
    RegisterClassEx(&WindowClass);

    Window = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT,
        "WindowClass",
        "Performance Counter",
        WS_POPUP,
        0,
        0,
        1920,
        1080,
        0,
        0,
        WindowClass.hInstance,
        0
    );

    SetLayeredWindowAttributes(Window, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margins = { -1 };
    DwmExtendFrameIntoClientArea(Window, &margins);

    CreateDeviceD3D(Window);

    ShowWindow(Window, SW_SHOWDEFAULT);
    UpdateWindow(Window);
    SetWindowPos(Window, HWND_TOPMOST, 0, 0, 1920, 1080, NULL);

    ImGui::CreateContext();
    ImGui_ImplWin32_Init(Window);
    ImGui_ImplDX9_Init(DirectX_device);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
    ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.0f, 0.0f, 0.0f, 0.0f });

    DirectX_device->SetRenderState(D3DRS_ZENABLE, FALSE);
    DirectX_device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    DirectX_device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
}
