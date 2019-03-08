#include <Windows.h>
#include <util/util.h>

auto InitThread = std::thread([]() { 
	Logger::Get()->Init();

	TLE_DEBUG("PROGRAM BEGIN");


	
	
	
	});

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		InitThread.detach();
		return TRUE;
	case DLL_PROCESS_DETACH:

		return TRUE;
	default:
		return TRUE;
	}
}
