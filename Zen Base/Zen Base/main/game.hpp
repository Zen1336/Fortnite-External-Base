#pragma once
#include "../Gui/imgui.h"
#include "../Gui/imgui_impl_dx9.h"
#include "../Gui/imgui_impl_win32.h"
#include <dwmapi.h>
#include <d3d9.h>
#include "../other/includes.hpp"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dwmapi.lib")
IDirect3D9Ex* p_object = NULL;
IDirect3DDevice9Ex* p_device = NULL;
D3DPRESENT_PARAMETERS p_params = { NULL };
MSG messager = { NULL };
HWND my_wnd = NULL;
HWND game_wnd = NULL;
HRESULT directx_init()
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_object))) exit(3);
	ZeroMemory(&p_params, sizeof(p_params));
	p_params.Windowed = TRUE;
	p_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	p_params.hDeviceWindow = my_wnd;
	p_params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	p_params.BackBufferFormat = D3DFMT_A8R8G8B8;
	p_params.BackBufferWidth = settings::width;
	p_params.BackBufferHeight = settings::height;
	p_params.EnableAutoDepthStencil = TRUE;
	p_params.AutoDepthStencilFormat = D3DFMT_D16;
	p_params.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	if (FAILED(p_object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, my_wnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &p_params, 0, &p_device)))
	{
		p_object->Release();
		exit(4);
	}
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(my_wnd);
	ImGui_ImplDX9_Init(p_device);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = 0;


	ImFontConfig config;
	config.MergeMode = false;
	ImFont* impactFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Bahnschrift.ttf", 15.0f);
	config.MergeMode = true;
	config.PixelSnapH = true;
	io.Fonts->AddFontDefault();


	
}

void create_overlay()
{
	WNDCLASSEXA wcex = {
		sizeof(WNDCLASSEXA),
		0,
		DefWindowProcA,
		0,
		0,
		0,
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		0,
		0,
		"ZenBase",
		LoadIcon(0, IDI_APPLICATION)
	};
	ATOM rce = RegisterClassExA(&wcex);
	RECT rect;
	GetWindowRect(GetDesktopWindow(), &rect);
	my_wnd = gui::create_window_in_band(0, rce, L"ZenBase", WS_POPUP, rect.left, rect.top, rect.right, rect.bottom, 0, 0, wcex.hInstance, 0, gui::ZBID_UIACCESS);
	SetWindowLong(my_wnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(my_wnd, RGB(0, 0, 0), 255, LWA_ALPHA);
	MARGINS margin = { -1 };
	DwmExtendFrameIntoClientArea(my_wnd, &margin);
	ShowWindow(my_wnd, SW_SHOW);
	UpdateWindow(my_wnd);
}



void radar_range(float* x, float* y, float range)
{
	if (fabs((*x)) > range || fabs((*y)) > range)
	{
		if ((*y) > (*x))
		{
			if ((*y) > -(*x))
			{
				(*x) = range * (*x) / (*y);
				(*y) = range;
			}
			else
			{
				(*y) = -range * (*y) / (*x);
				(*x) = -range;
			}
		}
		else
		{
			if ((*y) > -(*x))
			{
				(*y) = range * (*y) / (*x);
				(*x) = range;
			}
			else
			{
				(*x) = -range * (*x) / (*y);
				(*y) = -range;
			}
		}
	}
}

void aimbot(uintptr_t target_mesh)
{
	if (settings::aimbot::visible_check)
	{
		if (!target_mesh || !is_visible(target_mesh)) // Clarified visibility check condition
			return;
	}
	else
	{

	}
	Vector3 head3d = get_entity_bone(target_mesh, 110);
	Vector2 head2d = project_world_to_screen(head3d);
	Vector2 target{};
	if (head2d.x != 0)
	{
		if (head2d.x > settings::screen_center_x)
		{
			target.x = -(settings::screen_center_x - head2d.x);
			target.x /= settings::aimbot::smoothness;
			if (target.x + settings::screen_center_x > settings::screen_center_x * 2) target.x = 0;
		}
		if (head2d.x < settings::screen_center_x)
		{
			target.x = head2d.x - settings::screen_center_x;
			target.x /= settings::aimbot::smoothness;
			if (target.x + settings::screen_center_x < 0) target.x = 0;
		}
	}
	if (head2d.y != 0)
	{
		if (head2d.y > settings::screen_center_y)
		{
			target.y = -(settings::screen_center_y - head2d.y);
			target.y /= settings::aimbot::smoothness;
			if (target.y + settings::screen_center_y > settings::screen_center_y * 2) target.y = 0;
		}
		if (head2d.y < settings::screen_center_y)
		{
			target.y = head2d.y - settings::screen_center_y;
			target.y /= settings::aimbot::smoothness;
			if (target.y + settings::screen_center_y < 0) target.y = 0;
		}
	}
	input::move_mouse(target.x, target.y);
}


void Box(int X, int Y, int W, int H, const ImColor color, int thickness)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)X, (float)Y }, ImVec2{ (float)(X + W), (float)Y }, color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)(X + W), (float)Y }, ImVec2{ (float)(X + W), (float)(Y + H) }, color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)X, (float)(Y + H) }, ImVec2{ (float)(X + W), (float)(Y + H) }, color, thickness);
	ImGui::GetForegroundDrawList()->AddLine(ImVec2{ (float)X, (float)Y }, ImVec2{ (float)X, (float)(Y + H) }, color, thickness);
}

void draw_line(Vector2 target, const ImColor color)
{
	ImGui::GetForegroundDrawList()->AddLine(ImVec2(settings::screen_center_x, settings::height), ImVec2(target.x, target.y), color, 0.1f);
}



void Fpswtr() {

	constexpr float fontSize = 30.0f;
	constexpr ImVec2 watermarkPos = ImVec2(10.0f, 5.0f);

	static float lastTime = 0.0f;
	static int fps = 0;

	float currentTime = ImGui::GetTime();
	if (currentTime - lastTime >= 1.0f) {
		fps = static_cast<int>(ImGui::GetIO().Framerate);
		lastTime = currentTime;
	}

	char fpsText[32];
	snprintf(fpsText, sizeof(fpsText), "FPS: %.2f", ImGui::GetIO().Framerate);
	ImVec2 fpsSize = ImGui::CalcTextSize(fpsText);
	ImVec2 fpsPos = ImVec2(watermarkPos.x + 3.0f, watermarkPos.y + 3.0f);

}



#include "../other/keybinds.h"

#include <chrono>

using namespace std::chrono;



void DrawCrosshair(ImVec2 center, ImVec4 color, float size) {
	ImGui::GetForegroundDrawList()->AddLine(
		ImVec2(center.x - size, center.y), ImVec2(center.x + size, center.y),
		ImGui::ColorConvertFloat4ToU32(color), 2.0f
	);
	ImGui::GetForegroundDrawList()->AddLine(
		ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size),
		ImGui::ColorConvertFloat4ToU32(color), 2.0f
	);
}


void game_loop()
{
	cache::uworld = read<uintptr_t>(virtualaddy + UWORLD);
	cache::game_instance = read<uintptr_t>(cache::uworld + GAME_INSTANCE);
	cache::local_players = read<uintptr_t>(read<uintptr_t>(cache::game_instance + LOCAL_PLAYERS));
	cache::player_controller = read<uintptr_t>(cache::local_players + PLAYER_CONTROLLER);
	cache::local_pawn = read<uintptr_t>(cache::player_controller + LOCAL_PAWN);
	if (cache::local_pawn != 0)
	{
		cache::root_component = read<uintptr_t>(cache::local_pawn + ROOT_COMPONENT);
		cache::relative_location = read<Vector3>(cache::root_component + RELATIVE_LOCATION);
		cache::player_state = read<uintptr_t>(cache::local_pawn + PLAYER_STATE);
		cache::my_team_id = read<int>(cache::player_state + TEAM_INDEX);
	}
	cache::game_state = read<uintptr_t>(cache::uworld + GAME_STATE);
	cache::player_array = read<uintptr_t>(cache::game_state + PLAYER_ARRAY);
	cache::player_count = read<int>(cache::game_state + (PLAYER_ARRAY + sizeof(uintptr_t)));
	cache::closest_distance = FLT_MAX;
	cache::closest_mesh = NULL;
	for (int i = 0; i < cache::player_count; i++)
	{
		uintptr_t player_state = read<uintptr_t>(cache::player_array + (i * sizeof(uintptr_t)));
		if (!player_state) continue;
		int player_team_id = read<int>(player_state + TEAM_INDEX);
		if (player_team_id == cache::my_team_id) continue;
		uintptr_t pawn_private = read<uintptr_t>(player_state + PAWN_PRIVATE);
		if (!pawn_private) continue;
		if (pawn_private == cache::local_pawn) continue;
		uintptr_t mesh = read<uintptr_t>(pawn_private + MESH);
		if (!mesh) continue;
		Vector3 head3d = get_entity_bone(mesh, 110);
		Vector2 head2d = project_world_to_screen(Vector3(head3d.x, head3d.y, head3d.z + 20));
		Vector3 bottom3d = get_entity_bone(mesh, 0);
		Vector2 bottom2d = project_world_to_screen(bottom3d);
		float box_height = abs(head2d.y - bottom2d.y);
		float box_width = box_height * 0.50f;
		float distance = cache::relative_location.distance(bottom3d) / 100;
		if (settings::visuals::enable)
		{
			if (settings::visuals::Box)
			{
				if (is_visible(mesh))
				{
					Box(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(255, 0, 255, 250), 2);
				}
				else
				{
					Box(head2d.x - (box_width / 2), head2d.y, box_width, box_height, ImColor(255, 0, 255, 250), 2);
				}
			}
		}
		if (settings::visuals::enable)
		{


		}
		if (settings::visuals::line)
		{
			if (is_visible(mesh))
			{
				draw_line(bottom2d, ImColor(36, 207, 2, 250));
			}
			else
			{
				draw_line(bottom2d, ImColor(250, 0, 0, 250));
			}
		}

		if (settings::visuals::skeleton) {

			std::vector<Vector3> bones = {
		get_entity_bone(mesh, 66),  // neck
		get_entity_bone(mesh, 9),   // left shoulder
		get_entity_bone(mesh, 10),  // left elbow
		get_entity_bone(mesh, 11),  // left hand
		get_entity_bone(mesh, 38),  // right shoulder
		get_entity_bone(mesh, 39),  // right elbow
		get_entity_bone(mesh, 40),  // right hand
		get_entity_bone(mesh, 2),   // pelvis
		get_entity_bone(mesh, 71),  // left hip
		get_entity_bone(mesh, 72),  // left knee
		get_entity_bone(mesh, 78),  // right hip
		get_entity_bone(mesh, 79),  // right knee
		get_entity_bone(mesh, 75),  // left foot
		get_entity_bone(mesh, 82),  // right foot
		get_entity_bone(mesh, 67)   // head
			};

			std::vector<Vector2> screenPositions(bones.size());
			for (size_t i = 0; i < bones.size(); ++i) {
				screenPositions[i] = project_world_to_screen(bones[i]);
			}

			if (is_visible(mesh))
			{
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[4].x, screenPositions[4].y), ImColor(0, 0, 170, 250), 3.f);   // left shoulder to right shoulder
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[2].x, screenPositions[2].y), ImColor(0, 0, 170, 250), 3.f);   // left shoulder to left elbow
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[2].x, screenPositions[2].y), ImVec2(screenPositions[3].x, screenPositions[3].y), ImColor(0, 0, 170, 250), 3.f);   // left elbow to left hand
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[4].x, screenPositions[4].y), ImVec2(screenPositions[5].x, screenPositions[5].y), ImColor(0, 0, 170, 250), 3.f);   // right shoulder to right elbow
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[5].x, screenPositions[5].y), ImVec2(screenPositions[6].x, screenPositions[6].y), ImColor(0, 0, 170, 250), 3.f);   // right elbow to right hand
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[0].x, screenPositions[0].y), ImVec2(screenPositions[7].x, screenPositions[7].y), ImColor(0, 0, 170, 250), 3.f);   // neck to pelvis
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[8].x, screenPositions[8].y), ImColor(0, 0, 170, 250), 3.f);   // pelvis to left hip
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[8].x, screenPositions[8].y), ImVec2(screenPositions[9].x, screenPositions[9].y), ImColor(0, 0, 170, 250), 3.f);   // left hip to left knee
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[9].x, screenPositions[9].y), ImVec2(screenPositions[12].x, screenPositions[12].y), ImColor(0, 0, 170, 250), 3.f);  // left knee to left foot
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[10].x, screenPositions[10].y), ImColor(0, 0, 170, 250), 3.f);  // pelvis to right hip
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[10].x, screenPositions[10].y), ImVec2(screenPositions[11].x, screenPositions[11].y), ImColor(0, 0, 170, 250), 3.f); // right hip to right knee
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[11].x, screenPositions[11].y), ImVec2(screenPositions[13].x, screenPositions[13].y), ImColor(0, 0, 170, 250), 3.f); // right knee to right foot

			}
			else
			{
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[4].x, screenPositions[4].y), ImColor(250, 0, 0, 250), 3.f);   // left shoulder to right shoulder
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[1].x, screenPositions[1].y), ImVec2(screenPositions[2].x, screenPositions[2].y), ImColor(250, 0, 0, 250), 3.f);   // left shoulder to left elbow
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[2].x, screenPositions[2].y), ImVec2(screenPositions[3].x, screenPositions[3].y), ImColor(250, 0, 0, 250), 3.f);   // left elbow to left hand
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[4].x, screenPositions[4].y), ImVec2(screenPositions[5].x, screenPositions[5].y), ImColor(250, 0, 0, 250), 3.f);   // right shoulder to right elbow
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[5].x, screenPositions[5].y), ImVec2(screenPositions[6].x, screenPositions[6].y), ImColor(250, 0, 0, 250), 3.f);   // right elbow to right hand
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[0].x, screenPositions[0].y), ImVec2(screenPositions[7].x, screenPositions[7].y), ImColor(250, 0, 0, 250), 3.f);   // neck to pelvis
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[8].x, screenPositions[8].y), ImColor(250, 0, 0, 250), 3.f);   // pelvis to left hip
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[8].x, screenPositions[8].y), ImVec2(screenPositions[9].x, screenPositions[9].y), ImColor(250, 0, 0, 250), 3.f);   // left hip to left knee
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[9].x, screenPositions[9].y), ImVec2(screenPositions[12].x, screenPositions[12].y), ImColor(250, 0, 0, 250), 3.f);  // left knee to left foot
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[7].x, screenPositions[7].y), ImVec2(screenPositions[10].x, screenPositions[10].y), ImColor(250, 0, 0, 250), 3.f);  // pelvis to right hip
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[10].x, screenPositions[10].y), ImVec2(screenPositions[11].x, screenPositions[11].y), ImColor(250, 0, 0, 250), 3.f); // right hip to right knee
				ImGui::GetForegroundDrawList()->AddLine(ImVec2(screenPositions[11].x, screenPositions[11].y), ImVec2(screenPositions[13].x, screenPositions[13].y), ImColor(250, 0, 0, 250), 3.f); // right knee to right foot
			}

		}
		double dx = head2d.x - settings::screen_center_x;
		double dy = head2d.y - settings::screen_center_y;
		float dist = sqrtf(dx * dx + dy * dy);
		if (dist <= settings::aimbot::fov && dist < cache::closest_distance)
		{
			cache::closest_distance = dist;
			cache::closest_mesh = mesh;
		}
	}
	if (settings::aimbot::enable)
	{
		if (GetAsyncKeyState(settings::aimbot::current_key)) aimbot(cache::closest_mesh);
	}
	if (settings::Features::crosshair)
	{
		ImVec2 center = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
		ImVec4 color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		float size = 10.0f;
		DrawCrosshair(center, color, size);
	}
}




 int tab = 0;
void DrawMenu()
{
	ImGui::SetNextWindowSize({ 575, 350 });
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
	ImGui::Begin("ZenBase", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);

	if (ImGui::Button("Aimbot", { 196, 20 })) tab = 0;
	ImGui::SameLine();
	if (ImGui::Button("Visuals", { 196, 20 })) tab = 1;
	switch (tab)
	{
	case 0:
	{
		ImGui::Checkbox("Enable", &settings::aimbot::enable);
		ImGui::Checkbox("Draw Fov", &settings::aimbot::show_fov);
		HotkeyButton(hotkeys::aimkey, ChangeKey, keystatus); ImGui::SameLine(); ImGui::Text("Aimkey");
		ImGui::SliderFloat("Fov Size", &settings::aimbot::fov, 1, 400);
		ImGui::SliderFloat("Smoothness", &settings::aimbot::smoothness, 1, 10);

		break;
	}
	case 1:
	{
		ImGui::Checkbox("Box", &settings::visuals::Box);
		ImGui::Checkbox("Skeleton", &settings::visuals::skeleton);
		ImGui::Checkbox("Snaplines", &settings::visuals::line);

		break;
	}
	}

	ImGui::PopStyleVar();
	ImGui::End();
}

HWND get_process_wnd(uint32_t pid)
{
	std::pair<HWND, uint32_t> params = { 0, pid };
	BOOL bresult = EnumWindows([](HWND hwnd, LPARAM lparam) -> BOOL
		{
			auto pparams = (std::pair<HWND, uint32_t>*)(lparam);
			uint32_t processid = 0;
			if (GetWindowThreadProcessId(hwnd, reinterpret_cast<LPDWORD>(&processid)) && processid == pparams->second)
			{
				SetLastError((uint32_t)-1);
				pparams->first = hwnd;
				return FALSE;
			}
			return TRUE;
		}, (LPARAM)&params);
	if (!bresult && GetLastError() == -1 && params.first) return params.first;
	return 0;
}

WPARAM render_loop()
{
	static RECT old_rc;
	ZeroMemory(&messager, sizeof(MSG));
	while (messager.message != WM_QUIT)
	{
		if (PeekMessage(&messager, my_wnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&messager);
			DispatchMessage(&messager);
		}
		if (game_wnd == NULL) exit(0);
		HWND active_wnd = GetForegroundWindow();

		

		//if (active_wnd == game_wnd)
		//{
		//	HWND target_wnd = GetWindow(active_wnd, GW_HWNDPREV);
		//	SetWindowPos(my_wnd, target_wnd, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	//	}
	//	else
	//	{
	//		game_wnd = get_process_wnd(handler::process_id);
		//	Sleep(250);
	//	}
		RECT rc;
		POINT xy;
		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		rc.left = xy.x;
		rc.top = xy.y;
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;
		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;
		if (GetAsyncKeyState(0x1))
		{
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
		{
			io.MouseDown[0] = false;
		}
		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			settings::width = rc.right;
			settings::height = rc.bottom;
			p_params.BackBufferWidth = settings::width;
			p_params.BackBufferHeight = settings::height;
			SetWindowPos(my_wnd, (HWND)0, xy.x, xy.y, settings::width, settings::height, SWP_NOREDRAW);
			p_device->Reset(&p_params);
		}
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		game_loop();
		DrawMenu();
		ImGui::EndFrame();
		p_device->SetRenderState(D3DRS_ZENABLE, false);
		p_device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		p_device->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
		p_device->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		if (p_device->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			p_device->EndScene();
		}
		HRESULT result = p_device->Present(0, 0, 0, 0);
		if (result == D3DERR_DEVICELOST && p_device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			p_device->Reset(&p_params);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	if (p_device != 0)
	{
		p_device->EndScene();
		p_device->Release();
	}
	if (p_object != 0) p_object->Release();
	DestroyWindow(my_wnd);
	return messager.wParam;
}