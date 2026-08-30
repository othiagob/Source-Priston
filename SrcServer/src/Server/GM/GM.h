#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "smPacket.h"
#include "Chat\Chat.h"

using namespace std;

const int MAX_GMS = 20;

#define UTILS GameMasters::getInstance()

struct sAddExp 
{
	int size;
	int code;
	INT64 Exp;
};

class GameMasters
{
private:

	struct gmList {
		string gmName;
		int gmLevel;
	};

	struct gmList gmLIST[MAX_GMS];

	int x = 0;
public:
	GameMasters();
	~GameMasters();

	bool readFromDatabase();

	void print() const;

	static GameMasters* getInstance();
	int Ban(rsPLAYINFO* lpPlayInfo, bool isOnline, char* id, char* reason);
	int UnBan(char* id);
	int Alert(rsPLAYINFO* lpPlayInfo, char szMessage[256]);
	int Packet(rsPLAYINFO* Player, int code, int lParam = 0, int WParam = 0, int EParam = 0, int SParam=0);
	int PacketToAll(int code, int lParam = 0, int WParam = 0, int EParam = 0, int SParam = 0);
	void PacketChatToNearPlayers(rsPLAYINFO* Player, PacketItemLinkChat* Packet);
	void PacketChatToAllPlayers(rsPLAYINFO* Player, PacketItemLinkChat* Packet);
	void PacketChatToClanPlayers(rsPLAYINFO* Player, PacketItemLinkChat* Packet);
	void AddExpToPlayer(rsPLAYINFO* Player, INT64 Exp);

	char szAccountName[32] = { 0 };
	int iGameLevel = 0;
};



