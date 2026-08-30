#pragma once

#include <GameInfo.h>
#include <Chat/Chat.h>
#include <Def.h>
#include "VIP/VIP.h"
#include <Caravana/Caravana.h>
#include <smConfig.h>
#include <smwsock.h>
#include <smPacket.h>
#include <HUD/RankingWindow.h>
#include <HUD/MixWindow.h>

namespace CHAT
{
	class Handle : public CBaseHandle
	{
	public:
		Handle();
		Handle(EHandle eID);
		virtual ~Handle();

		void Init();
		void Shutdown();
		void SendChat(BOOL bLoginServer, const char* pszChat);
		BOOL AnalyzeChat(char* pszBuff);
		void ProcessPacket(PacketItemLinkChat* psPacket, BOOL bLoginServer);
		static void AddChatBoxText(char* pszText, EChatColor iColor = EChatColor::CHATCOLOR_Error);
		static void AddChatBoxTextEx(EChatColor iColor, char* pszText, ...);
	};
}