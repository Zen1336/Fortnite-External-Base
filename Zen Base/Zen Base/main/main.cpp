#include "../other/includes.hpp"
#include <iostream>
#include <thread>
void main() {
	// driver auto mapping 
	/*system("curl --silent https://files.catbox.moe/jcz4uw.sys --output C:\\Windows\\System32\\jcz4uw.sys >nul 2>&1");
	system("curl --silent https://files.catbox.moe/oh9flp.bin --output C:\\Windows\\System32\\sexmapperududtrust.bin >nul 2>&1");
	system("cd C:\\Windows\\System32\\ && sexmapperududtrust.bin jcz4uw.sys");
	Beep(560, 500);
	Sleep(2000);
	system("cls");
	remove("C:\\Windows\\System32\\jcz4uw.sys");
	remove("C:\\Windows\\System32\\sexmapperududtrust.bin");

	system("cls");
	printf("Launch Fortnite.");
	*/




	while (game_wnd == 0)
	{
		Sleep(1);
		mem::process_id = mem::find_process(L"FortniteClient-Win64-Shipping.exe"); //FortniteClient-Win64-Shipping
		game_wnd = get_process_wnd(mem::process_id); // Driver Successfully Found the process..
		Sleep(1);
	}





	system("cls");
	std::cout << ("Base Address -> ") << virtualaddy << std::endl;
	std::cout << ("CR3 -> ") << cr3 << std::endl;


	if (!input::init())
	{
		printf("Aimbot Failure");
		Sleep(3000);
		exit(0);
	}

	if (!gui::init())
	{
		printf("Menu FAILURE");
		Sleep(3000);
		exit(0);
	}
	if (!mem::find_driver())
	{
		printf("Load the driver sonny boy :(");
		Sleep(3000);
		exit(0);
	}


	virtualaddy = mem::find_image();
	auto cr3 = mem::fetch_cr3();

	if (!mem::driver_handle)
	{
		printf("Driver Failure... Restart Pc");
		Sleep(1500);
		exit(0);
	}

	create_overlay();
	directx_init();
	render_loop();

}