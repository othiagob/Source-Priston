#pragma once

#define MAX_CHATLENGTH			100
#define MAX_CHATMESSAGES		320

#define PACKET_CHAT_GAME 0x48471001

#include "GameInfo.h"
#include <deque>
#include <Windows.h>

#include "sinbaram/sinItem.h"

enum class EChatColor
{
	CHATCOLOR_Error = 0,
	CHATCOLOR_Whisper = 1,
	CHATCOLOR_Blue = 2,
	CHATCOLOR_Global = 3,
	CHATCOLOR_Party = 4,
	CHATCOLOR_Clan = 5,
	CHATCOLOR_Trade = 6,
	CHATCOLOR_Notice = 9,
	CHATCOLOR_ClanLeader = 10,
	CHATCOLOR_Normal = 11,
	CHATCOLOR_Raid = 12,
};

struct ItemLinkData
{
	int								iID;

	std::deque<sITEM*>			vItemData;
};

struct PacketItemLinkChat
{
	int size;
	int code;

	char					szCharacterName[32];
	char					szMessage[256];
	EChatColor				eChatColor;

	BYTE trans[sizeof(sITEM)];
};

struct PacketChatBoxMessage
{
	int size;
	int code;
	EChatColor	iChatColor;
	DWORD			lID;
	char		szChatBoxMessage[256 + 8];
};

struct PacketChat
{
	int size;
	int code;
	char szChat[128];
};