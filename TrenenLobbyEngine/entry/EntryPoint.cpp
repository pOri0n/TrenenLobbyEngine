#include <pch.h>

auto InitThread = std::thread([]() { 
	Logger::Get()->Init();
	
	TLE_DEBUG("PROGRAM BEGIN");

	I.Setup();
	Hooks::Get()->Init();
	});

auto DestroyThread = []() {
	Hooks::Get()->Destroy();
	Logger::Get()->Destroy();
};

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		InitThread.detach();
		return TRUE;
	case DLL_PROCESS_DETACH:
		DestroyThread();
		return TRUE;
	default:
		return TRUE;
	}
}
