# TrenenLobbyEngine
    This is a sample application demonstating the use of modifying lobby messages to provide modified output within CSGO's lobbies.
    
    oh yea, and i was REALLY drunk while writing this hence the lazy parsing e.t.c.

## Important Notes
    The application does not properly parse the buffer and I am purposely not doing it properly - people who know what they are doing can properly modify the messages with a little bit of work - but this is the bare minimum work.

## Features Available
### Chat Colours
    Standard, Red, Green, Yellow
### Endline Spammer
    Breaks layout of chatbox if a few of these messages are sent!
### Queue Bomb
    Sends multiple Game::EnteringQueue messages to play queue start sound (pretty much a bass boosted BOOOM) multiple times ( between 10-10000 )
### Hide who you invite
    Engine can hide which user you invite
    
## Features developers can add
### Change info of other users within lobby
	Ranks, Level, Badge, e.t.c.
### yea i cbf listing the rest
