#pragma once

class Config
{
public:
	bool LobbyChat_Enable = false;
	int LobbyChat_ColourIndex = 0;
	bool LobbyChat_PrependName = false;
	bool LobbyChat_EndlineSpam = false;
	float LobbyChat_RepeatExponent = 0.0f;

	bool LobbyNuke_Enable = false;
	float LobbyNuke_Exponent = 1.0f;

	bool LobbyInvite_HideInviteNames = false;

	bool LobbyRank_ModifyProfiles = false;
	int LobbyRank_PlayerRank = 1;
	int LobbyRank_PlayerLevel = 2;
	bool LobbyRank_Prime = false;

private:
	Singleton(Config);
};

#define CFG Config::Get()
