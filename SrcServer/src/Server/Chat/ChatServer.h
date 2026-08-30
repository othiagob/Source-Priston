#pragma once

#include <Windows.h>
#include "smPacket.h"
#include <Utils/strings.h>
#include <Utils/_common.h>
#include "Chat.h"
#include <Character/playsub.h>
#include <Ranking/PVPRanking.h>
#include <Ranking/TopRanking.h>
#include <Quest/Quest.h>
#include <Skills/Skills.h>
#include <Eventos/Arena.h>
#include <Shop/NewShop.h>
#include "SrcServer/onserver.h"
#include "field.h"


#define FREE_TRADECHAT			FALSE
#define USER_TALKRANGE			1200
#define USER_TALKRANGE_FAR		307200

static char* pszaWordsTrade[]
{
		"nobody cares",
		"fuck",
		"fruit",
		"bpt",
		"ept",
		"kpt",
		"upt",
		"mpt",
		"rpt",
		"realm",
		"unique",
		"sandurr",
		"kenni",
		"magicpt",
		"zado",
		"lost kingdom",
		"lostkingdom",
		"dick",
		"penis",
		"giromba",
		"prog",
		"3dfx",
		"argos",
		"horus",
		"elsa",
		"im gm",
		"n4p4lm",
		"n4palm",
		"nap4lm",
		"napa1m",
		"n4p41m",
		"eat",
		"dog",
		"mama",
		"mother",
		"mommy",
		"suck",
		"8=",
		"bucet",
		"piroc",
		"bitch",
		"unban",
		"unb4n",
		"1nb4n",
		"un ban",
		"fode",
		"f0d3",
		"server",
		"http",
		"www.",
		".net",
		".com",
		"ticket",
		"t1ck3t",
		"gm pls",
		"merda",
		"m3rd4",
		"merd4",
		"m3rda",
		"desban",
		"p r o g",
		"plog",
		"vince",
		"v1nc3",
		"vinc3",
		"Naiany", //Namorada do lee
		"Naiane",
		"Naiani",
		"N4i4n3",
		"N414n3",
		"brocco",
		"br0cc0",
		"b r o c c o",
		"b r 0 c c 0"
};

#define SERVERCHAT					CServerChat::GetInstance()

class CServerChat
{
public:
	static              CServerChat* GetInstance() { static CServerChat instance; return &instance; }

	CServerChat();
	virtual ~CServerChat();
	void SendDebugChat(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszFormat, ...);
	void SendChatAllUsersInStage(int iStage, EChatColor eColor, const char* pszFormat, ...);
	void SendChatAll(EChatColor eColor, const char* pszText);
	void SendChatAllEx(EChatColor eColor, const char* pszFormat, ...);
	void SendChatAllUsersInRange(int iX, int iZ, int iRange, EChatColor eColor, const char* pszFormat, ...);
	void SendChatAllUsersInClan(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszFormat, ...);
	void SendChat(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszText);
	void SendChatEx(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszFormat, ...);
	void SendPersonalShopChat(rsPLAYINFO* pcUser, DWORD dwObjectID, const char* pszFormat, ...);
	void SendUserBoxChat(rsPLAYINFO* pcUser, DWORD dwObjectID, const char* pszFormat, ...);
	void SendUserBoxChatRange(int iX, int iZ, int iRange, DWORD dwObjectID, const char* pszFormat, ...);
	void SendChatMessageBox(rsPLAYINFO* pcUser, const char* pszFormat, ...);
	void SendChatSpyMember(rsPLAYINFO* pcUser, void* pPacket);
	void SendChatTrade(rsPLAYINFO* pcUser, std::string strMessage);
	void SendWhisperItemLink(rsPLAYINFO* pcUserSender, rsPLAYINFO* pcUserReceiver, PacketItemLinkChat* psPacket);

	void HandlePacket(rsPLAYINFO* pcUser, PacketItemLinkChat* psPacket);

	void AddMute(const char* pszName);
	void ClearMute();

	std::string GetFormatWhisper(rsPLAYINFO* pcUser, const char* pszMessage, BOOL bReceiverTime);

	void SendWhisper(rsPLAYINFO* pcUserSender, rsPLAYINFO* pcUserReceiver, const char* pszMessage);

	std::string GetMessageString(std::string strMessage, int iChatColor);
public:
	BOOL bFreeTradeChat = FALSE;
protected:
	BOOL CanSendMessage(rsPLAYINFO* pcUser, std::string strMessage, int iChatColor);
private:
	std::vector<std::string> m_vMutedNames;
};