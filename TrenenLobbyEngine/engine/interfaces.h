#pragma once

template<typename T = DWORD>
class Interface
{
	friend class cInterfaces;
public:
	// Operator Overloads
	inline T* operator=(T* const INF)
	{
		return m_pInterface = INF;
	}

	template<typename B>
	inline T* operator=(B* const INF)
	{
		return m_pInterface = (T*)INF;
	}

	inline T* operator()() const
	{
		return m_pInterface;
	}

protected:
	// Searching for Interfaces
	static T* FindInterface(std::string InterfaceName, std::string ModuleName)
	{
		// Wait on module to ensure it is loaded
		HINSTANCE Module = GetModuleHandle(ModuleName.c_str());
		while (Module == nullptr)
		{
			Module = GetModuleHandle(ModuleName.c_str());
			std::this_thread::sleep_for(0.25s);
		}

		// Get CreateInterface for Module
		auto Factory = (void* (__cdecl*)(const char*, int*))(GetProcAddress(Module, "CreateInterface"));
		if (!Factory)
		{
			TLE_CRITICAL("Cannot import createinterface");
			throw 0;
		}

		// Find our interface
		T* Interface = (T*)Factory(InterfaceName.c_str(), nullptr);

		if (!Interface)
		{
			TLE_CRITICAL("Failed to find interface");
			throw 0;
		}

		TLE_DEBUG("FOUND INTERFACE {0}", InterfaceName);
		return Interface;
	}

	// Learn_more's pattern scanning
	static T* FindPattern(HINSTANCE Module, std::string Pattern)
	{
#define INRANGE(x,a,b)    (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

		const char* pat = Pattern.c_str();
		DWORD firstMatch = 0;
		DWORD rangeStart = (DWORD)Module;
		MODULEINFO miModInfo;
		GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
		DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
		for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
		{
			if (!*pat)
			{
				TLE_DEBUG("PATTERN SCAN SUCCESS FOR PATTERN {0}", Pattern);
				return (T*)firstMatch;
			}

			if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
			{
				if (!firstMatch) firstMatch = pCur;

				if (!pat[2])
				{
					TLE_DEBUG("PATTERN SCAN SUCCESS FOR PATTERN {0}", Pattern);
					return (T*)firstMatch;
				}

				if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
					pat += 3;
				else pat += 2; //one ?
			}
			else
			{
				pat = Pattern.c_str();
				firstMatch = 0;
			}
		}

		TLE_CRITICAL("PATTERN SCAN FAIL FOR PATTERN {0}", Pattern);
		return nullptr;
	}
private:
	// Pointer to local interface
	T* m_pInterface = nullptr;
};

class cInterfaces
{
public:
	void Setup();

	Interface<ISteamClient>				SteamClient;
	Interface<ISteamMatchmaking>		SteamMatchmaking;
	Interface<ISteamUser>				SteamUser;
	Interface<ISteamFriends>			SteamFriends;
	Interface<IDirect3DDevice9>			DXDevice;
};

extern cInterfaces& I;