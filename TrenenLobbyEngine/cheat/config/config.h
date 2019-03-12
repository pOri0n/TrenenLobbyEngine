#pragma once

class Config
{
public:
	bool LobbyChat_Enable = false;
	int LobbyChat_ColourIndex = 0;
	bool LobbyChat_EndlineSpam = false;
	float LobbyChat_RepeatExponent = 0.0f;

	char LobbyChat_Format[256] = "{0} {1}";

	bool LobbyNuke_Enable = false;
	float LobbyNuke_Exponent = 1.0f;

	bool LobbyInvite_HideInviteNames = false;

	bool LobbyRank_ModifyProfiles = false;
	int LobbyRank_PlayerRank = 1;
	int LobbyRank_PlayerLevel = 2;
	bool LobbyRank_Prime = false;

	int LobbyRank_OtherPlayersRank = 1;
	int LobbyRank_OtherPlayersLevel = 2;
	bool LobbyRank_OtherPlayersPrime = false;

private:
	Singleton(Config);
};

#define CFG Config::Get()
