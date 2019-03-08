#include <pch.h>

Singleton_CPP(Hooks);

bool __stdcall SendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody)
{
	typedef bool(__thiscall* SendLobbyChatMessage_t)(ISteamMatchmaking*, CSteamID, const void*, int);
	static auto Original_SendLobbyChatMessage = Hooks::Get()->SteamMatchmaking.Original<SendLobbyChatMessage_t>(26);
	
	TLE_DEBUG("SendLobbyChatMessage Called");


	return Original_SendLobbyChatMessage(I.SteamMatchmaking(), steamIdLobby, pvMsgBody, cubMsgBody);
}

void Hooks::Init()
{
	SteamMatchmaking.Attach(I.SteamMatchmaking());
	SteamMatchmaking.Hook(26, SendLobbyChatMessage);
	TLE_DEBUG("Hooks Applied");
}

void Hooks::Destroy()
{
	SteamMatchmaking.Release();
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
