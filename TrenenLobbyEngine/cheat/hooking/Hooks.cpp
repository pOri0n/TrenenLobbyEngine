#include <pch.h>

Singleton_CPP(Hooks);

bool __stdcall Hooked_SendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody)
{
	typedef bool(__thiscall* SendLobbyChatMessage_t)(ISteamMatchmaking*, CSteamID, const void*, int);
	static auto Original_SendLobbyChatMessage = Hooks::Get()->SteamMatchmaking.Original<SendLobbyChatMessage_t>(26);
	
	if (!LobbyMod::Get()->InterpretLobbyMessage(steamIdLobby, pvMsgBody, cubMsgBody))
		return Original_SendLobbyChatMessage(I.SteamMatchmaking(), steamIdLobby, pvMsgBody, cubMsgBody);

	return true;
}

HRESULT __stdcall Hooked_Reset(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	typedef HRESULT(__stdcall * Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	static auto Original_Reset = Hooks::Get()->DirectX.Original<Reset_t>(16);
	
	TLE_TRACE("RESET");
	
	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT Res = Original_Reset(pDevice, pPresentationParameters);
	ImGui_ImplDX9_CreateDeviceObjects();
	return Res;
}

HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
{
	typedef HRESULT(__stdcall * Endscene_t)(IDirect3DDevice9*);
	static auto Original_EndScene = Hooks::Get()->DirectX.Original<Endscene_t>(42);

	UI::Get()->Render(pDevice);

	return Original_EndScene(pDevice);
}

WNDPROC OrigWindowProc = nullptr;
LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (UI::Get()->OnWndProc(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(OrigWindowProc, hWnd, uMsg, wParam, lParam);
}

void Hooks::Init()
{
	SteamMatchmaking.Attach(I.SteamMatchmaking());
	SteamMatchmaking.Hook(26, Hooked_SendLobbyChatMessage);

	DirectX.Attach(I.DXDevice());
	DirectX.Hook(16, Hooked_Reset);
	DirectX.Hook(42, Hooked_EndScene);

	HWND hWindow = nullptr;
	while (!(hWindow = FindWindowA("Valve001", NULL)))
		std::this_thread::sleep_for(0.2s);
	OrigWindowProc = (WNDPROC)SetWindowLongPtr(hWindow, GWL_WNDPROC, (LONG_PTR)Hooked_WndProc);

	TLE_DEBUG("Hooks Applied");
}

void Hooks::Destroy()
{
	SteamMatchmaking.Release();
	TLE_DEBUG("Hooks Released");
}

VMTH::~VMTH()
{
	// These should never fail as we should always Release b4 this object
	// is destroyed, otherwise we got issues!
	assert(!m_pObject);
	assert(!m_pOriginalTable);
	assert(!m_pCustomTable);
}

void VMTH::Attach(DWORD** Object)
{
	assert(Object);
	assert(m_pObject == NULL);
	assert(m_pOriginalTable == NULL);
	assert(m_pCustomTable == NULL);

	m_pObject = Object;
	m_pOriginalTable = *m_pObject;

	auto FindTableSize = [](DWORD * Table) -> int {
		int Index = 0;
		while (!IsBadCodePtr((FARPROC)Table[Index]))
		{
			Index++;
		}

		return Index;
	};

	m_TableSize = FindTableSize(m_pOriginalTable);

	// Create Our New Table
	m_pCustomTable = (DWORD*)malloc(sizeof(DWORD) * m_TableSize);

	if (m_pCustomTable == nullptr)
	{
		TLE_CRITICAL("COULD NOT ALLOCATE NEW VTABLE - FAILURE");
		return;
	}

	memcpy(m_pCustomTable, m_pOriginalTable, m_TableSize * sizeof(DWORD));

	*Object = m_pCustomTable;
}

void VMTH::Release()
{
	assert(m_pObject);
	assert(m_pOriginalTable);
	assert(m_pCustomTable);

	// Free our shit
	*m_pObject = m_pOriginalTable;
	free(m_pCustomTable);

	m_pObject = NULL;
	m_pOriginalTable = NULL;
	m_pCustomTable = NULL;
	m_TableSize = 0;
}

void VMTH::Hook(unsigned int Index, DWORD NewFunction)
{
	m_pCustomTable[Index] = NewFunction;
}
