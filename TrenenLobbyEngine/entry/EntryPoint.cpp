#include <pch.h>

auto InitThread = std::thread([]() { 
	Logger::Get()->Init();
	
	TLE_WARN("Trenen Engine");
	TLE_WARN("       A module which allows users to quickly modify lobby chat communications");
	TLE_WARN("       This is for example only and is the minimum nessissary code");
	TLE_WARN("       Developers should properly parse the lobby messages instead of doing it the lazy way");

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
