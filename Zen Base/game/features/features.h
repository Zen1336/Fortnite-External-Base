namespace settings
{
	inline int width = GetSystemMetrics(SM_CXSCREEN);
	inline int height = GetSystemMetrics(SM_CYSCREEN);
	inline int screen_center_x = width / 2;
	inline int screen_center_y = height / 2;
}

namespace aimbot
{
	bool enabled = true;
	bool drawfov = false;
	int fovsize = 100;
	float smoothing = 1;

	inline static const char* aimbone[] = { "Head", "Chest" };
	inline static int current_aimbone = 0;
}

namespace esp
{
	bool boxes = false;
	bool corneredboxes = false;
	bool skeleton = false;
	bool distance = false;
	bool snaplines = false;
	bool vischeck = false;
	float thickness = 1;
}

namespace menu
{
	bool ShowMenu = true;
}