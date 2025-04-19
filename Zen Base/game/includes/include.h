#include <string>
#include <iostream>
#include "../../driver/driver.h"
#include <Windows.h>
#include "../informations/offsets/offsets.h"
#include <cstdint>
#include <Mmsystem.h>
#include <mciapi.h>
#include <shobjidl_core.h>
#include "../features/features.h"
#include <winioctl.h>
#include <random>
#include <msxml.h>
#include <atomic>
#include <mutex>
#include <TlHelp32.h>
#include <vector>
#include <memoryapi.h>
#include <thread>
#include <chrono>
#include <string_view>
#include <Psapi.h>
#include <tchar.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <WinInet.h>
#include "../informations/offsets/actors.h"
#include "lazy.h"
#include "xor.hpp"
#include "utils.h"
#include "../features/esp/drawing.h"
#include "../informations/getbone.h"
#include "../informations/w2s.h"
#include "../features/esp/skeleton.h"
#include "../features/overlay/overlay.hpp"
static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;


typedef struct _FNlEntity
{
	uint64_t Actor;
	int ID;
	uint64_t mesh;
}FNlEntity;
std::vector<FNlEntity> entityList;

#include "D3DX/d3dx9.h"
#include <d3d9.h>
#include "keybinds.h"
#include <urlmon.h>


#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "urlmon.lib")

ImFont* m_pFont;

Vector3 localactorpos;


#include "../../menu.hpp"
#include "../features/aimbot/aimbot.h"

