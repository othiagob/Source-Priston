#include "ChatServer.h"
#include "Party\CPartyHandler.h"

extern rsPLAYINFO* FindUserFromName(char* szName);
extern rsPLAYINFO* rsPlayInfo;

CServerChat::CServerChat()
{

}
CServerChat::~CServerChat()
{

}

void CServerChat::SendDebugChat(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszFormat, ...)
{
	if (pcUser)
	{
		if (pcUser->AdminMode)
		{
			va_list ap;

			PacketChatBoxMessage sPacket;
			ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
			sPacket.size = sizeof(PacketChatBoxMessage);
			sPacket.code = PACKET_CHAT_GAME;
			sPacket.iChatColor = eColor;

			va_start(ap, pszFormat);
			vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
			va_end(ap);

			SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);
		}
	}
}
void CServerChat::SendChatAllUsersInStage(int iStage, EChatColor eColor, const char* pszFormat, ...)
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
	sPacket.size = sizeof(PacketChatBoxMessage);
	sPacket.code = PACKET_CHAT_GAME;
	sPacket.iChatColor = eColor;

	va_start(ap, pszFormat);
	vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
	va_end(ap);

	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock)
		{
			if (rsPlayInfo[i].Position.Area == iStage)
				SENDPACKET(&rsPlayInfo[i], (char*)&sPacket, sPacket.size, 1);
		}
	}
}
void CServerChat::SendChatAll(EChatColor eColor, const char* pszText)
{
	PacketChatBoxMessage sPacket;
	ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
	sPacket.size = sizeof(PacketChatBoxMessage);
	sPacket.code = PACKET_CHAT_GAME;
	sPacket.iChatColor = eColor;

	STRINGCOPY(sPacket.szChatBoxMessage, pszText);

	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock)
		{
			SENDPACKET(&rsPlayInfo[i], (char*)&sPacket, sPacket.size, 1);
		}
	}
}
void CServerChat::SendChatAllEx(EChatColor eColor, const char* pszFormat, ...)
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
	sPacket.size = sizeof(PacketChatBoxMessage);
	sPacket.code = PACKET_CHAT_GAME;
	sPacket.iChatColor = eColor;

	va_start(ap, pszFormat);
	vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
	va_end(ap);

	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock)
		{
			SENDPACKET(&rsPlayInfo[i], (char*)&sPacket, sPacket.size, 1);
		}
	}
}
void CServerChat::SendChatAllUsersInRange(int iX, int iZ, int iRange, EChatColor eColor, const char* pszFormat, ...)
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
	sPacket.size = sizeof(PacketChatBoxMessage);
	sPacket.code = PACKET_CHAT_GAME;
	sPacket.iChatColor = eColor;

	va_start(ap, pszFormat);
	vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
	va_end(ap);

	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock && rsPlayInfo[i].dwObjectSerial)
		{
			auto rx = (iX - rsPlayInfo[i].Position.x) >> FLOATNS;
			auto rz = (iZ - rsPlayInfo[i].Position.z) >> FLOATNS;
			auto dist = rx * rx + rz * rz;

			if (dist < DIST_TRANSLEVEL_HIGH && abs(rx) < (64 * 64) + iRange && abs(rz) < (64 * 64 + iRange))
			{
				rsPlayInfo[i].lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
			}
		}
	}
}
void CServerChat::SendChatAllUsersInClan(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszFormat, ...)
{
	if (pcUser && pcUser->lpsmSock)
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.size = sizeof(PacketChatBoxMessage);
		sPacket.code = PACKET_CHAT_GAME;
		sPacket.iChatColor = eColor;

		va_start(ap, pszFormat);
		vsnprintf_s(sPacket.szChatBoxMessage, 256, pszFormat, ap);
		va_end(ap);

		for (int i = 0; i < CONNECTMAX; i++)
		{
			if (rsPlayInfo[i].lpsmSock)
			{
				if (rsPlayInfo[i].dwClanCode == pcUser->dwClanCode && rsPlayInfo[i].lpsmSock != pcUser->lpsmSock)
					SENDPACKET(&rsPlayInfo[i], (char*)&sPacket, sPacket.size);
			}
		}
	}
}
void CServerChat::SendChat(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszText)
{
	if (pcUser)
	{
		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.code = PACKET_CHAT_GAME;
		sPacket.iChatColor = eColor;

		STRINGCOPY(sPacket.szChatBoxMessage, pszText);

		sPacket.size = 16 + lstrlenA(sPacket.szChatBoxMessage) + 12;

		SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);
	}
}
void CServerChat::SendChatEx(rsPLAYINFO* pcUser, EChatColor eColor, const char* pszFormat, ...)
{
	if (pcUser)
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.code = PACKET_CHAT_GAME;
		sPacket.iChatColor = eColor;

		va_start(ap, pszFormat);
		vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
		va_end(ap);

		sPacket.size = 16 + STRLEN(sPacket.szChatBoxMessage) + 12;

		SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);
	}
}
void CServerChat::SendPersonalShopChat(rsPLAYINFO* pcUser, DWORD dwObjectID, const char* pszFormat, ...)
{
	if (pcUser)
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.code = PACKET_CHAT_GAME;
		sPacket.iChatColor = CHATCOLOR_Error;
		sPacket.lID = dwObjectID;

		va_start(ap, pszFormat);
		vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
		va_end(ap);

		sPacket.size = 16 + lstrlenA(sPacket.szChatBoxMessage) + 12;

		SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);
	}
}
void CServerChat::SendUserBoxChat(rsPLAYINFO* pcUser, DWORD dwObjectID, const char* pszFormat, ...)
{
	if (pcUser)
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.code = PACKET_CHAT_GAME;
		sPacket.iChatColor = CHATCOLOR_Error;
		sPacket.lID = dwObjectID;

		va_start(ap, pszFormat);
		vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
		va_end(ap);

		sPacket.size = 16 + lstrlenA(sPacket.szChatBoxMessage) + 12;

		SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);
	}
}
void CServerChat::SendUserBoxChatRange(int iX, int iZ, int iRange, DWORD dwObjectID, const char* pszFormat, ...)
{
	va_list ap;

	PacketChatBoxMessage sPacket;
	ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
	sPacket.code = PACKET_CHAT_GAME;
	sPacket.iChatColor = CHATCOLOR_Error;
	sPacket.lID = dwObjectID;

	va_start(ap, pszFormat);
	vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
	va_end(ap);

	sPacket.size = 16 + lstrlenA(sPacket.szChatBoxMessage) + 12;

	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock && rsPlayInfo[i].dwObjectSerial)
		{
			auto rx = (iX - rsPlayInfo[i].Position.x) >> FLOATNS;
			auto rz = (iZ - rsPlayInfo[i].Position.z) >> FLOATNS;
			auto dist = rx * rx + rz * rz;

			if (dist < DIST_TRANSLEVEL_HIGH && abs(rx) < (64 * 64) + iRange && abs(rz) < (64 * 64 + iRange))
			{
				rsPlayInfo[i].lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
			}
		}
	}
}
void CServerChat::SendChatMessageBox(rsPLAYINFO* pcUser, const char* pszFormat, ...)
{
	if (pcUser)
	{
		va_list ap;

		PacketChatBoxMessage sPacket;
		ZeroMemory(&sPacket, sizeof(PacketChatBoxMessage));
		sPacket.code = PACKET_CHAT_GAME;
		sPacket.iChatColor = CHATCOLOR_Error;

		va_start(ap, pszFormat);
		vsnprintf_s(sPacket.szChatBoxMessage, 255, pszFormat, ap);
		va_end(ap);
		sPacket.size = 16 + lstrlenA(sPacket.szChatBoxMessage) + 12;

		SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);
	}
}
void CServerChat::SendChatSpyMember(rsPLAYINFO* pcUser, void* pPacket)
{

}
void CServerChat::SendChatTrade(rsPLAYINFO* pcUser, std::string strMessage)
{
	if (pcUser)
	{
		//if (CanSendMessage(pcUser, strMessage, CHATCOLOR_Trade))
		//{
		strMessage.erase(0, 7);
		SendChatAll(CHATCOLOR_Trade, FormatString("[T]%s: %s", pcUser->smCharInfo.szName, strMessage.c_str()));
		pcUser->dwChatTradeTime = GetCurrentTime() + ((2 * 60) * 1000);
		//}
	}
}
void CServerChat::SendWhisperItemLink(rsPLAYINFO* pcUserSender, rsPLAYINFO* pcUserReceiver, PacketItemLinkChat* psPacket)
{
	if (!pcUserReceiver->WhisperMode)
	{
		sITEM pItem = { 0 };
		DecodeCompress((BYTE*)psPacket->trans, (BYTE*)&pItem);

		if (!pItem.sItemInfo.ItemName[0])
			return;

		std::string strMessage = psPacket->szMessage;

		if (strMessage.length() == 0)
			strMessage = pItem.sItemInfo.ItemName;

		if (strMessage.find("<item>") != std::string::npos)
			strMessage.replace(strMessage.find("<item>"), STRLEN("<item>"), pItem.sItemInfo.ItemName);

		//To Receiver
		STRINGCOPY(psPacket->szMessage, GetFormatWhisper(pcUserSender, strMessage.c_str(), TRUE).c_str());
		SENDPACKET(pcUserReceiver, (char*)psPacket, psPacket->size, 1);

		//To Sender
		STRINGCOPY(psPacket->szMessage, GetFormatWhisper(pcUserReceiver, strMessage.c_str(), FALSE).c_str());
		SENDPACKET(pcUserSender, (char*)psPacket, psPacket->size, 1);
	}
	else
		SERVERCHAT->SendChatEx(pcUserSender, CHATCOLOR_Error, "%s desabilitou as mensagens privadas!", pcUserReceiver->smCharInfo.szName);
}
void CServerChat::HandlePacket(rsPLAYINFO* pcUser, PacketItemLinkChat* psPacket)
{
	sITEM pItem = { 0 };
	DecodeCompress((BYTE*)&psPacket->trans, (BYTE*)&pItem);

	if (!pItem.sItemInfo.ItemName[0])
		return;

	pItem.sItemInfo.BackUpKey += RandomI(1000, 20000);
	pItem.sItemInfo.BackUpChkSum += RandomI(2000, 10000);

	ZeroMemory(&psPacket->trans, sizeof(sITEM));
	EecodeCompress((BYTE*)&pItem, psPacket->trans, sizeof(sITEM));

	if (psPacket->szCharacterName[0] != 0)
	{
		rsPLAYINFO* pcUserReceiver = FindUserFromName(psPacket->szCharacterName);

		if (pcUserReceiver)
			SendWhisperItemLink(pcUser, pcUserReceiver, psPacket);
		else
			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s está offline ou não foi encontrado!", psPacket->szCharacterName);
	}
	else
	{
		std::string strMessage = GetMessageString(psPacket->szMessage, psPacket->eChatColor);

		char FormatedName[32] = { 0 };
		STRINGFORMAT(FormatedName, "[%s]", pItem.sItemInfo.ItemName);

		if (strMessage.length() == 0)
			strMessage = pItem.sItemInfo.ItemName;

		if (strMessage.find("<item>") != std::string::npos)
			strMessage.replace(strMessage.find("<item>"), STRLEN("<item>"), pItem.sItemInfo.ItemName);

		if (strMessage.find(pItem.sItemInfo.ItemName) != std::string::npos)
			strMessage.replace(strMessage.find(pItem.sItemInfo.ItemName), STRLEN(pItem.sItemInfo.ItemName), FormatedName);

		if (psPacket->eChatColor == CHATCOLOR_Normal)
		{
			STRINGFORMAT(psPacket->szMessage, "%s: %s", pcUser->smCharInfo.szName, strMessage.c_str());

			UTILS->PacketChatToNearPlayers(pcUser, psPacket);

		}
		else if (psPacket->eChatColor == CHATCOLOR_Trade)
		{
			STRINGFORMAT(psPacket->szMessage, "[%c]%s: %s", 'T', pcUser->smCharInfo.szName, strMessage.c_str());

			UTILS->PacketChatToAllPlayers(pcUser, psPacket);
		}
		else if ((psPacket->eChatColor == CHATCOLOR_Party) || (psPacket->eChatColor == CHATCOLOR_Raid))
		{
			STRINGFORMAT(psPacket->szMessage, "[%c]%s: %s", 'P', pcUser->smCharInfo.szName, strMessage.c_str());

			if (CanSendMessage(pcUser, psPacket->szMessage, psPacket->eChatColor))
			{
				if (pcUser->bParty && pcUser->psParty && pcUser->psParty->pcLeader)
				{
					for (auto& v : PARTYHANDLER->GetPartyMembers(pcUser->psParty, false))
					{
						if (v && v != pcUser)
							SENDPACKET(v, (char*)&psPacket, psPacket->size);
					}
				}

				SENDPACKET(pcUser, (char*)psPacket, psPacket->size, 1);
			}
		}
		else if (psPacket->eChatColor == CHATCOLOR_Clan)
		{
			STRINGFORMAT(psPacket->szMessage, "%s: %s", pcUser->smCharInfo.szName, strMessage.c_str());

			if (pcUser->dwClanCode)
			{
				UTILS->PacketChatToClanPlayers(pcUser, psPacket);
			}
		}
	}
}
void CServerChat::AddMute(const char* pszName)
{
	m_vMutedNames.push_back(pszName);
}
void CServerChat::ClearMute()
{
	m_vMutedNames.clear();
}

std::string CServerChat::GetFormatWhisper(rsPLAYINFO* pcUser, const char* pszMessage, BOOL bReceiverTime)
{
	return FormatString("%s> %s: %s", bReceiverTime ? "De" : "Para", pcUser->smCharInfo.szName, pszMessage);
}

void CServerChat::SendWhisper(rsPLAYINFO* pcUserSender, rsPLAYINFO* pcUserReceiver, const char* pszMessage)
{
	if (pszMessage[0] != 0)
	{
		if (!pcUserReceiver->WhisperMode)
		{
			//To Receiver
			SERVERCHAT->SendChat(pcUserReceiver, CHATCOLOR_Blue, GetFormatWhisper(pcUserSender, pszMessage, TRUE).c_str());

			//To Sender
			SERVERCHAT->SendChat(pcUserSender, CHATCOLOR_Blue, GetFormatWhisper(pcUserReceiver, pszMessage, FALSE).c_str());
		}
		else
			SERVERCHAT->SendChatEx(pcUserSender, CHATCOLOR_Error, "%s desabilitou as mensagens privadas!", pcUserReceiver->smCharInfo.szName);
	}
}

std::string CServerChat::GetMessageString(std::string strMessage, int iChatColor)
{
	std::string str = strMessage;

	if (iChatColor == CHATCOLOR_Trade)
	{
		if (strMessage.length() >= 7)
		{
			if (strMessage.find("TRADE>") != std::string::npos)
			{
				str = strMessage.substr(strMessage.find_first_of(">") + 1);
				str = trim(str);
			}
		}
	}
	else if (iChatColor == CHATCOLOR_Clan)
	{
		if (strMessage.length() >= 6)
		{
			if (strMessage.find("CLAN>") != std::string::npos)
			{
				str = strMessage.substr(strMessage.find_first_of(">") + 1);
				str = trim(str);
			}
		}
	}
	else if ((iChatColor == CHATCOLOR_Party) || (iChatColor == CHATCOLOR_Raid))
	{
		if ((strMessage.length() >= 1) && ((strMessage[0] == '@') || (strMessage[0] == '#')))
		{
			str = strMessage.substr(1);
			str = trim(str);
		}
	}

	return str;
}

BOOL CServerChat::CanSendMessage(rsPLAYINFO* pcUser, std::string strMessage, int iChatColor)
{
	BOOL bRet = TRUE;

	if (iChatColor == CHATCOLOR_Trade)
	{
		if (strMessage.length() >= 7)
		{
			strMessage = strMessage.substr(strMessage.find_first_of(">") + 1);
			strMessage = trim(strMessage);

			std::string strFind = strMessage;

			if (strMessage.length() > 0)
			{
				//Can talk?
				if (pcUser->bMuted == FALSE)
				{
					BOOL bMutedPlayer = FALSE;
					for (const auto v : m_vMutedNames)
					{
						if (STRINGCOMPAREI(pcUser->smCharInfo.szName, v.c_str()))
						{
							bMutedPlayer = TRUE;
							bRet = FALSE;
							break;
						}
					}

					if (bMutedPlayer == FALSE)
					{
						//To lower
						for (UINT i = 0; i < strMessage.length(); i++)
							strFind[i] = tolower(strFind[i]);

						char cFirstLetter = strMessage[0];
						char cSecondLetter = strMessage[1];

						BOOL bFreeTrade = TRUE;

						//Not GM and on time delay?
						if (pcUser->AdminMode == 1 && pcUser->dwChatTradeTime > GetCurrentTime())
						{
							if (!FREE_TRADECHAT)
							{
								//Warning user
								SendChatEx(pcUser, CHATCOLOR_Error, "> Wait %d seconds to write again", (pcUser->dwChatTradeTime - GetCurrentTime()) / 1000);
								bFreeTrade = FALSE;
								bRet = FALSE;
							}
						}
						if (bRet)
						{
							for (int i = 0; i < _countof(pszaWordsTrade); i++)
							{
								//Is found words? don't write in chat
								if (strstr(strFind.c_str(), pszaWordsTrade[i]))
								{
									bRet = FALSE;
									break;
								}
							}
						}
					}
				}
			}
		}
		else
			bRet = FALSE;
	}

	return bRet;
}