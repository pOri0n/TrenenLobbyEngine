#include <pch.h>

Singleton_CPP(LobbyMod);

/*
	NOTE TO ALL USING THIS:

	The buffer passed in pvMsgBody is infact a proper data structure that should be handled properly

	but 
	1. im writing this at 1am atm
	2. im only writing it cuz im sick of people asking questions on how it works dispite giving enough info

	BUT IN THIS IMPLEMENTATION I AM BEING LAZY JUST TO GIVE EXAMPLE SO I CAN STOP ANSWERING QUESTIONS
	( ++ it will be pretty obv if they just pasted entire thing cuz they wont parse shit properly :O )

	Also this WILL break after a few updates cuz im lazy but you guys can figure out how to fix it

	also to my future employers yea i know this is really poorly done just ignore this project and look at my
	other stuff <3 k thanks :)

*/

bool LobbyMod::InterpretLobbyMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody)
{
	const char* pMessage = (const char*)pvMsgBody;
	const char* MessageIterator = pMessage + 5; // 5 Bytes in we have the big SysSession::Command


	auto steamIdOwner = I.SteamMatchmaking()->GetLobbyOwner(steamIdLobby);
	auto steamIdMe = I.SteamUser()->GetSteamID();

	IsLobbyOwner = steamIdOwner == steamIdMe;

#if 0
	// For those who wish to look at the structure of the messages
	auto DumpFile = std::ofstream("LobbyChatMessageDump.bin", std::ios::binary | std::ios::trunc);

	if (DumpFile.is_open())
	{
		DumpFile.write((const char*)pvMsgBody, cubMsgBody);
		DumpFile.close();
	}
#endif

	// Do note this is not all the messages sent - have a poke around and dump some out and figure what they are!

	if (strcmp(MessageIterator, "SysSession::Command") == 0)
	{
		MessageIterator = FindStringEnd(MessageIterator) + 1;

		if (strcmp(MessageIterator, "Game::Chat") == 0)
			return ModifyStandardChatMessage(steamIdLobby, pMessage, cubMsgBody);

		else if (strcmp(MessageIterator, "Game::EnteringQueue") == 0)
			return OnEnterMatchmakingQueue(steamIdLobby, pMessage, cubMsgBody);

		else if (strcmp(MessageIterator, "Game::ChatInviteMessage") == 0)
			return OnChatInviteMessage(steamIdLobby, pMessage, cubMsgBody);

		else if (strcmp(MessageIterator, "Game::SetPlayerRanking") == 0)
			return OnSetPlayerRanking(steamIdLobby, pMessage, cubMsgBody);

	}
	else if (strcmp(MessageIterator, "SysSession::OnPlayerUpdated") == 0)
		return OnPlayerUpdated(steamIdLobby, pMessage, cubMsgBody);
	
	return false;
}

const char* LobbyMod::FindStringEnd(const char* Message)
{
	while (*Message != 0)
		Message++;
	return Message + 1; // Include null terminator
}

const char* LobbyMod::FindString(const char* in, const char* pat, unsigned int mxdst)
{
	for (unsigned int i = 0; i < mxdst; i++)
	{
		if (strcmp(in + i, pat) == 0)
			return in + i;
	}

	return in;
}

bool LobbyMod::ModifyStandardChatMessage(CSteamID Lobby, const char* pMessage, const size_t MessageSize)
{
	if (CFG->LobbyChat_ColourIndex == Color_Standard)
		return false;

	// Note: green / yellow require player to be lobby owner
	const std::vector<std::pair<std::string, std::string>> ChatColourTypes =
	{
		{ "Game::Chat",							"chat"	 }, // Standard // no longer in use
		{ "Game::ChatReportMatchmakingStatus",	"status" }, // lets user define custom font colour :O
		{ "Game::ChatReportError",				"error"  }, // Red
		{ "Game::ChatReportGreen",				"green"  }, // well i mean really do i have to spell it out for these two
		{ "Game::ChatReportYellow",				"yellow" }
	};

	const auto ChatColour = ChatColourTypes[ CFG->LobbyChat_ColourIndex ];

	TLE_INFO("Game::Chat REPLACED WITH {0}", ChatColour.first);

	// ##### SysSession::Command ## 
	const std::vector<char> MessageStage0 =
	{
		0x00, 0x00, 0x35, 0x71,
		0x00, 0x53, 0x79, 0x73,
		0x53, 0x65, 0x73, 0x73,
		0x69, 0x6F, 0x6e, 0x3a,
		0x3a, 0x43, 0x6F, 0x6d,
		0x6d, 0x61, 0x6e, 0x64,
		0x00, 0x00
	};

	// ## run # all ## xuid #
	const std::vector<char> MessageStage1 =
	{
		0x00, 0x01, 0x72, 0x75,
		0x6E, 0x00, 0x61, 0x6C,
		0x6c, 0x00, 0x07, 0x78,
		0x75, 0x69, 0x64, 0x00,
	};

	// XUID ( steam id ) currently starts at 0x34 for chat messages
	// Has to be current players steamid
	const std::vector<char> UserXUID =
	{
		pMessage[0x34], pMessage[0x35], pMessage[0x36], pMessage[0x37],
		pMessage[0x38], pMessage[0x39], pMessage[0x3A], pMessage[0x3B]
	};

	// Note, we dont accually have to copy in the name to the message
	// as the game engine just gets player name from the XUID, so pass 
	// in a null string cuz we lazy

	// # name ###
	const std::vector<char> MessageStage2 =
	{
		 0x01, 0x6e, 0x61, 0x6d,
		 0x65, 0x00, 0x00, 0x01
	};

	// ####
	const std::vector<char> MessageStage3 =
	{
		0x00, 0x0b, 0x0b, 0x0b
	};

	// Generate Our Message
	auto GeneratedMessage = MessageStage0;

	std::copy(ChatColour.first.begin(), ChatColour.first.end(), std::back_inserter(GeneratedMessage));
	std::copy(MessageStage1.begin(), MessageStage1.end(), std::back_inserter(GeneratedMessage));
	std::copy(UserXUID.begin(), UserXUID.end(), std::back_inserter(GeneratedMessage));
	std::copy(MessageStage2.begin(), MessageStage2.end(), std::back_inserter(GeneratedMessage));
	std::copy(ChatColour.second.begin(), ChatColour.second.end(), std::back_inserter(GeneratedMessage));
	GeneratedMessage.push_back(0); // strings dont seem to copy over null terminator :(

	// Your message is always stored right after chat
	const auto MessageText = std::string(FindString(pMessage, "chat") + 5);
	const auto NameText = std::string(FindString(pMessage, "name") + 5);

	std::string Text = "";// fmt::format(CFG->LobbyChat_Format, NameText, MessageText);

	// insert our custom message
	if (CFG->LobbyChat_ColourIndex == Color_Custom) 
		Text = fmt::format(CFG->LobbyChat_Format, NameText, MessageText); // user defined custom format
	else if (CFG->LobbyChat_PrependName)
		Text = fmt::format("{0} {1}", NameText, MessageText);
	else
		Text = fmt::format("{1}", NameText, MessageText);

	std::copy(Text.begin(), Text.end(), std::back_inserter(GeneratedMessage));

	// Sending enough messages will cause the chat box
	// in CS to break!, probably a layout glitch
	// sending lots of endlines makes it happen after
	// a few messages
	if (CFG->LobbyChat_EndlineSpam)
	{
		// Speed up that insert :)
		GeneratedMessage.reserve(GeneratedMessage.size() + 10000);

		for (auto i = 0; i < 10000; i++)
			GeneratedMessage.push_back(0x0A);
	}
	
	std::copy(MessageStage3.begin(), MessageStage3.end(), std::back_inserter(GeneratedMessage));

	const auto RepPower = std::powf(10, CFG->LobbyChat_RepeatExponent);

	for (auto i = 0.f; i < RepPower; i++)
		 CallOriginalSendLobbyChatMessage(Lobby, GeneratedMessage.data(), GeneratedMessage.size());

	return true;
}

bool LobbyMod::OnEnterMatchmakingQueue(CSteamID Lobby, const char* pMessage, const size_t MessageSize)
{
	if (CFG->LobbyNuke_Enable)
	{
		// This is a big RIP to peoples games lol
		const auto NukePower = std::powf(10, CFG->LobbyNuke_Exponent);

		for (auto i = 0.f; i < NukePower; i++)
			CallOriginalSendLobbyChatMessage(Lobby, pMessage, MessageSize);
		
		return true;
	}

	return false;
}

bool LobbyMod::OnChatInviteMessage(CSteamID Lobby, const char* pMessage, const size_t MessageSize)
{
	if (!CFG->LobbyInvite_HideInviteNames)
		return false;

	std::vector<char> Message;

	// For remembering players :O
	static std::map<uint64_t, unsigned int> Players;
	static unsigned int PlayerCount = 0;

	// The Friends steamid
	const auto FriendXUID = *(uint64_t*)(pMessage + 0x51);

	// If we havnt seen this player before assign him a his own number
	if (!Players.count(FriendXUID))
	{
		Players[FriendXUID] = ++PlayerCount;
	}

	// Player's ID
	const auto PlayerIndex = Players[FriendXUID];

	// new playername
	const std::string PlayerName = std::string("player #") + std::to_string(PlayerIndex);

	// all those messages ending in ZERO BEE BEE BEE
	const std::vector<char> MessageEnd =
	{
		0x00, 0x0b, 0x0b, 0x0b
	};

	// Copy the original message upto the players name
	std::copy(pMessage, pMessage + 0x65, std::back_inserter(Message));
	std::copy(PlayerName.begin(), PlayerName.end(), std::back_inserter(Message));
	std::copy(MessageEnd.begin(), MessageEnd.end(), std::back_inserter(Message));

	// Set XUID to a random steam account of mine so they can never tell which
	// nonamer was invited even if they really wanted to!
	*(uint64_t*)(Message.data() + 0x51) = 76561198881516729u;

	TLE_INFO("Game::ChatInviteMessage - REPLACED PLAYER NAME WITH {0}", PlayerName);

	return CallOriginalSendLobbyChatMessage(Lobby, Message.data(), Message.size());
}

bool LobbyMod::OnSetPlayerRanking(CSteamID Lobby, const char* pMessage, const size_t MessageSize)
{
	if (!CFG->LobbyRank_ModifyProfiles)
		return false;

	std::vector<char> Message;
	std::copy(pMessage, pMessage + MessageSize, std::back_inserter(Message));

	*(char*)(FindString(Message.data(), "level") + 9) = CFG->LobbyRank_PlayerLevel;
	*(char*)(FindString(Message.data(), "ranking") + 11) = CFG->LobbyRank_PlayerRank;
	*(char*)(FindString(Message.data(), "prime") + 9) = CFG->LobbyRank_Prime;

	if (*(char*)(FindString(Message.data(), "wins") + 8) < 10)
		* (char*)(FindString(Message.data(), "wins") + 8) = 10;
	
	return CallOriginalSendLobbyChatMessage(Lobby, Message.data(), Message.size());
}

bool LobbyMod::OnPlayerUpdated(CSteamID Lobby, const char* pMessage, const size_t MessageSize)
{
	if (!CFG->LobbyRank_ModifyProfiles)
		return false;

	std::vector<char> Message;
	std::copy(pMessage, pMessage + MessageSize, std::back_inserter(Message));

	*(char*)(FindString(Message.data(), "level") + 9) = CFG->LobbyRank_PlayerLevel;
	*(char*)(FindString(Message.data(), "ranking") + 11) = CFG->LobbyRank_PlayerRank;
	*(char*)(FindString(Message.data(), "prime") + 9) = CFG->LobbyRank_Prime;

	if (*(char*)(FindString(Message.data(), "wins") + 8) < 10)
		* (char*)(FindString(Message.data(), "wins") + 8) = 10;

	return CallOriginalSendLobbyChatMessage(Lobby, Message.data(), Message.size());
}

bool LobbyMod::CallOriginalSendLobbyChatMessage(CSteamID steamIdLobby, const void* pvMsgBody, int cubMsgBody)
{
	typedef bool(__thiscall * SendLobbyChatMessage_t)(ISteamMatchmaking*, CSteamID, const void*, int);
	static auto Original_SendLobbyChatMessage = Hooks::Get()->SteamMatchmaking.Original<SendLobbyChatMessage_t>(26);

	return Original_SendLobbyChatMessage(I.SteamMatchmaking(), steamIdLobby, pvMsgBody, cubMsgBody);
}

