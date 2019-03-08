#pragma once

class Config
{
public:
	unsigned int LobbyChat_ColourIndex = 0;
	bool LobbyChat_PrependName = true;
	bool LobbyChat_EndlineSpam = false;

	bool LobbyNuke_Enable = false;
	float LobbyNuke_Exponent = 1.0f;

	bool LobbyInvite_HideInviteNames = false;

private:
	Singleton(Config);
};

#define CFG Config::Get()
