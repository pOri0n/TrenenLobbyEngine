#pragma once

class LobbyMod
{
public:
	// returns true if message has been overwritten
	bool InterpretLobbyMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody);

private:
	// a few easy things to get around the file fast AF
	const char* FindStringEnd(const char* Message);
	const char* FindString(const char* in, const char* pat, unsigned int mxdst = 1000);

	bool ModifyStandardChatMessage(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnEnterMatchmakingQueue(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnChatInviteMessage(CSteamID Lobby, const char* pMessage, const size_t MessageSize);
	bool OnSetPlayerRanking(CSteamID Lobby, const char* pMessage, const size_t MessageSize);

	bool CallOriginalSendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody);

private:
	Singleton(LobbyMod);
};