//SIMPLE ASS SETTINGS HERE
#pragma once
namespace settings
{
	 int width = GetSystemMetrics(SM_CXSCREEN);
	 int height = GetSystemMetrics(SM_CYSCREEN);
	 int screen_center_x = width / 2;
	 int screen_center_y = height / 2;
	 bool show_menu = true;
	 int tab = 0;
	namespace aimbot
	{
		 bool enable = false;
		 bool show_fov = false;
		 float fov = 160;
		 float smoothness = 6;
		 bool visible_check = false;
		 static int current_key = VK_RBUTTON;
	}
	namespace visuals
	{
		 bool enable = true;
		 bool line = false;
		 bool Box = false;
		 bool skeleton = false;
	}
	namespace Features
	{
		 bool Watermark = true;
		 bool info = false;
		 bool crosshair = false;
		 bool fpswater = true;
	}
	namespace trigger
	{
		 static int current_key = VK_LBUTTON;
		 bool Triggerbot = false;
		 float TriggerDelay = 70;
	}
}