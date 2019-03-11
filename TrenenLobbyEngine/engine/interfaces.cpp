#include "pch.h"

#define WaitOnFind(EXP) \
	while ((EXP) == 0) { std::this_thread::sleep_for(0.1s); }

cInterfaces l_Interfaces;
cInterfaces& I = l_Interfaces;

void cInterfaces::Setup()
{
	HMODULE SteamDLLHandle = nullptr;
	HMODULE ShaderDLLHandle = nullptr;
	WaitOnFind( SteamDLLHandle = GetModuleHandle("steam_api.dll") );
	WaitOnFind( ShaderDLLHandle = GetModuleHandle("shaderapidx9.dll") );

	SteamClient = Interface<>::FindInterface("SteamClient017", "steamclient.dll");

	auto GetHSteamPipe = reinterpret_cast<HSteamPipe(*)()>(GetProcAddress(SteamDLLHandle, "GetHSteamPipe"));
	auto GetHSteamUser = reinterpret_cast<HSteamUser(*)()>(GetProcAddress(SteamDLLHandle, "GetHSteamUser"));

	SteamMatchmaking = SteamClient()->GetISteamMatchmaking(GetHSteamUser(), GetHSteamPipe(), "SteamMatchMaking009");
	SteamFriends = SteamClient()->GetISteamFriends(GetHSteamUser(), GetHSteamPipe(), "SteamFriends015");
	SteamUser = SteamClient()->GetISteamUser(GetHSteamUser(), GetHSteamPipe(), "SteamUser019");

	DXDevice = Interface<>::FindPattern(ShaderDLLHandle, "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C");
	DXDevice = (IDirect3DDevice9*)**(DWORD**)((DWORD)DXDevice() + 1);

	TLE_DEBUG("INTERFACE Direct3DDevice: 0x{0:08x}",		(DWORD)DXDevice());
	TLE_DEBUG("INTERFACE SteamMatchMaking009: 0x{0:08x}",	(DWORD)SteamMatchmaking());
	TLE_DEBUG("INTERFACE SteamFriends015: 0x{0:08x}",		(DWORD)SteamFriends());
}
