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
	Interface<ISteamFriends>			SteamFriends;
};

extern cInterfaces& I;