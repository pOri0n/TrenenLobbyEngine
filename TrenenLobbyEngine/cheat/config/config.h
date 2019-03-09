#pragma once

class Config
{
public:
	bool LobbyChat_Enable = false;
	int LobbyChat_ColourIndex = 0;
	bool LobbyChat_PrependName = false;
	bool LobbyChat_EndlineSpam = false;

	bool LobbyNuke_Enable = false;
	float LobbyNuke_Exponent = 1.0f;

	bool LobbyInvite_HideInviteNames = false;

private:
	Singleton(Config);
};

#define CFG Config::Get()
