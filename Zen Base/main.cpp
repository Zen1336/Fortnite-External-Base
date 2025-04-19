#include "game/includes/include.h"
#include "ImGui/imgui_impl_dx9.h"
#include "ImGui/imgui_internal.h"

void ActorLoop();
static void xMainLoop();
static void xShutdown();
static void InitializeImGui();
static void xCreateWindow();

Vector3 RelativeLocation;
bool isDeviceLost = false;
void HandleDeviceLost(IDirect3DDevice9* device);
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam);
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void LoadDriver()
{
	URLDownloadToFileA(NULL, "https://github.com/Jaxploit/EthernalFree/raw/refs/heads/main/NiggerMapper.exe", "C:\\Windows\\System32\\NiggerMapper.exe", 0, NULL);
	URLDownloadToFileA(NULL, "https://github.com/Jaxploit/EthernalFree/raw/refs/heads/main/ZeusDriver.sys", "C:\\Windows\\System32\\ZeusDriver.sys", 0, NULL);

	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	const char* exePath = "C:\\Windows\\System32\\NiggerMapper.exe C:\\Windows\\System32\\ZeusDriver.sys";
	if (CreateProcessA(NULL, (LPSTR)exePath, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		CloseHandle(pi.hThread);
		Sleep(2500);
		TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess);
	}

	std::remove("C:\\Windows\\System32\\NiggerMapper.exe");
	std::remove("C:\\Windows\\System32\\ZeusDriver.sys");
	system("cls");
}

int main() 
{
	SetConsoleTitleA("Base");

	if (!driver::Init()) {
		system("cls");
		std::cout << "\n [-] Driver not loaded | Loading it";
		LoadDriver();

		if (driver::Init()) {
			system("cls");
			std::cout << "\n [+] Successfully loaded driver | Please restart the loader";
			Sleep(2500);
			exit(0);
		}
		else {
			system("cls");
			std::cout << "\n [-] Failed to load driver";
			Sleep(2500);
			exit(0);
		}
	}

	std::cout << "\n [!] Waiting for Fortnite";
	while (hwnd == NULL) {
		XorS(wind, "Fortnite  ");
		hwnd = FindWindowA(0, wind.decrypt());
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	processID = driver::GetPID("FortniteClient-Win64-Shipping.exe"); //FortniteClient-Win64-Shipping
	base_address = driver::GetBase();
	auto CR3 = driver::Fetch_CR3();

	system("cls");
	std::cout << "\n [+] Cr3: " << CR3;

	xCreateWindow();
	InitializeImGui();
	xMainLoop();
	xShutdown();

	return 0;
}

void ActorLoop()
{
	if (aimbot::drawfov)
	{
		DrawCircle(settings::screen_center_x, settings::screen_center_y, aimbot::fovsize, &Col.white, 150, 1);
	}

	Actors::Uworld = read<DWORD_PTR>(base_address + OFFSETS::UWorld);
	Actors::GameInstance = read<DWORD_PTR>(Actors::Uworld + OFFSETS::GameInstance);
	Actors::LocalPlayers = read<DWORD_PTR>(Actors::GameInstance + OFFSETS::LocalPlayers);
	Actors::Localplayer = read<DWORD_PTR>(Actors::LocalPlayers);
	Actors::PlayerController = read<DWORD_PTR>(Actors::Localplayer + OFFSETS::PlayerController);
	Actors::LocalPawn = read<DWORD_PTR>(Actors::PlayerController + OFFSETS::LocalPawn);
	Actors::PlayerState = read<DWORD_PTR>(Actors::LocalPawn + OFFSETS::PlayerState);
	Actors::MyTeamId = read<int>(Actors::PlayerState + OFFSETS::TeamId);
	Actors::RootComp = read<DWORD_PTR>(Actors::LocalPawn + OFFSETS::RootComponent);
	Actors::GameState = read<uintptr_t>(Actors::Uworld + OFFSETS::GameState);
	Actors::PlayerArray = read<uintptr_t>(Actors::GameState + OFFSETS::PlayerArray);
	Actors::ActorCount = read<int>(Actors::GameState + (OFFSETS::PlayerArray + sizeof(uintptr_t)));
	RelativeLocation = read<Vector3>(Actors::RootComp + OFFSETS::RelativeLocation);

	float ClosestDistance = FLT_MAX;
	DWORD_PTR ClosestPawn = NULL;

	for (int i = 0; i < Actors::ActorCount; i++)
	{
		uintptr_t PlayerState = read<uintptr_t>(Actors::PlayerArray + (i * sizeof(uintptr_t)));
		if (!PlayerState) continue;

		int EnemyTeamId = read<int>(PlayerState + OFFSETS::TeamId);
		if (EnemyTeamId == Actors::MyTeamId) continue;

		uintptr_t PawnPrivate = read<uintptr_t>(PlayerState + OFFSETS::PawnPrivate);
		if (!PawnPrivate || PawnPrivate == Actors::LocalPawn) continue;

		uintptr_t Mesh = read<uintptr_t>(PawnPrivate + OFFSETS::Mesh);
		if (!Mesh) continue;

		Vector3 Head3d = GetBoneWithRotation(Mesh, 110);
		Vector2 Head2d = ProjectWorldToScreen(Vector3(Head3d.x, Head3d.y, Head3d.z + 15));
		Vector3 Bottom3d = GetBoneWithRotation(Mesh, 0);
		Vector2 Bottom2d = ProjectWorldToScreen(Bottom3d);
		float CornerHeight = abs(Head2d.y - Bottom2d.y);
		float CornerWidth = CornerHeight * 0.40;
		float Distance = RelativeLocation.distance(Bottom3d) / 100;

		double Dx = Head2d.x - settings::screen_center_x;
		double Dy = Head2d.y - settings::screen_center_y;
		float Dist = sqrtf(Dx * Dx + Dy * Dy);
		if (Dist <= aimbot::fovsize && Dist < ClosestDistance)
		{
			ClosestDistance = Dist;
			ClosestPawn = PawnPrivate;
		}

		if (esp::corneredboxes)
		{
			ImColor Color = isVisible(Mesh) ? ImColor(0, 255, 0) : ImColor(255, 0, 0);
			DrawCorneredBox(Head2d.x - (CornerWidth / 2), Head2d.y, CornerWidth, CornerHeight, Color, esp::thickness);
		}

		if (esp::boxes)
		{
			ImColor Color = isVisible(Mesh) ? ImColor(0, 255, 0) : ImColor(255, 0, 0);
			DrawBox(Head2d.x - (CornerWidth / 2), Head2d.y, CornerWidth, CornerHeight, Color, esp::thickness);
		}

		if (esp::distance)
		{
			char DistText[64];
			sprintf_s(DistText, "%.fm", Distance);
			float TextWidth = ImGui::CalcTextSize(DistText).x;
			float XCentered = Bottom2d.x - TextWidth / 2;
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(XCentered, Bottom2d.y), ImColor(255, 255, 255), DistText);
		}

		if (esp::snaplines)
		{
			ImColor Color = isVisible(Mesh) ? ImColor(0, 255, 0) : ImColor(255, 0, 0);
			ImGui::GetForegroundDrawList()->AddLine(ImVec2(settings::screen_center_x, settings::width), ImVec2(Bottom2d.x, Bottom2d.y), Color, esp::thickness);
		}

		if (esp::skeleton)
		{
			DrawSkeleton(Mesh);
		}
	}

	if (aimbot::enabled && GetAsyncKeyState(hotkeys::aimkey))
	{
		aimbotPlayer(ClosestPawn);
	}
}

void Render()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		menu::ShowMenu = !menu::ShowMenu;
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (menu::ShowMenu)
	{
		DrawMenu();
	}

	ActorLoop();

	ImGui::EndFrame();

	D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (D3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		D3dDevice->EndScene();
	}

	HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);
	if (result == D3DERR_DEVICELOST)
	{
		isDeviceLost = true;
	}

	if (isDeviceLost && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		HandleDeviceLost(D3dDevice);
		isDeviceLost = false;
	}
}

void HandleDeviceLost(IDirect3DDevice9* device)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = device->Reset(&d3dpp);
	if (hr == D3D_OK)
	{
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void xMainLoop()
{
	MSG Message;
	RECT old_rc;

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		if (GetForegroundWindow() == hwnd)
			SetWindowPos(Window, GetWindow(GetForegroundWindow(), GW_HWNDPREV), 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

		if (GetAsyncKeyState(0x23) & 1)
			exit(8);

		RECT rc;
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, reinterpret_cast<POINT*>(&rc.left));

		auto& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = static_cast<float>(p.x - rc.left);
		io.MousePos.y = static_cast<float>(p.y - rc.top);

		io.MouseDown[0] = GetAsyncKeyState(VK_LBUTTON);
		if (io.MouseDown[0])
		{
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0] = io.MousePos;
		}

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;
			Width = rc.right;
			Height = rc.bottom;
			d3dpp.BackBufferWidth = Width;
			d3dpp.BackBufferHeight = Height;
			SetWindowPos(Window, (HWND)0, rc.left, rc.top, Width, Height, SWP_NOREDRAW);
			D3dDevice->Reset(&d3dpp);
		}
		Render();
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	DestroyWindow(Window);
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		xShutdown();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = D3dDevice->Reset(&d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}

void xShutdown()
{
	TriBuf->Release();
	D3dDevice->Release();
	p_Object->Release();

	DestroyWindow(Window);
	UnregisterClass("fgers", NULL);
}