#pragma once

class VMTH
{
public:
	~VMTH();

	void Attach(DWORD** Object);

	template<typename T>
	void Attach(T Object) { Attach((DWORD * *)Object); }

	void Release();

	void Hook(unsigned int Index, DWORD NewFunction);

	template<typename T>
	void Hook(unsigned int Index, T NewFunction)
	{ Hook(Index, (DWORD)NewFunction); }

	// Get Original Function
	template<typename T = FARPROC>
	T Original(unsigned int I) { return (T)m_pOriginalTable[I]; }

private:
	DWORD** m_pObject = NULL;
	DWORD* m_pOriginalTable = NULL;
	DWORD* m_pCustomTable = NULL;
	unsigned int m_TableSize = 0;
};

class Hooks
{
public:
	void Init();
	void Destroy();

	// i should probs not expose this lol
	VMTH SteamMatchmaking;
	VMTH DirectX;
private:
	Singleton(Hooks);
};