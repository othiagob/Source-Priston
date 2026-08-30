#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <Party/Party.h>

#define PARTYHANDLER					CPartyHandler::GetInstance()

class CPartyHandler
{
public:
	static              CPartyHandler* GetInstance() { static CPartyHandler instance; return &instance; }

	CPartyHandler();
	virtual				   ~CPartyHandler();

	void					ProcessPacket(rsPLAYINFO* pcUser, PacketRequestParty* psPacket);
	void					ProcessPacket(rsPLAYINFO* pcUser, PacketUpdateParty* psPacket);
	void					ProcessPacket(rsPLAYINFO* pcUser, PacketUpdatePartyData* psPacket);
	void					ProcessPacket(rsPLAYINFO* pcUser, PacketRequestRaid* psPacket);

	void					HandlePacket(rsPLAYINFO* pcUser, PacketRequestParty* psPacket);
	void					HandlePacket(rsPLAYINFO* pcUser, PacketJoinParty* psPacket);
	void					HandlePacket(rsPLAYINFO* pcUser, PacketActionParty* psPacket);
	void					HandlePacket(rsPLAYINFO* pcUser, PacketRequestRaid* psPacket);
	void					HandlePacket(rsPLAYINFO* pcUser, PacketJoinRaid* psPacket);

	void					SendChatParty(rsPLAYINFO* pcUser, std::string strMessage, EChatColor eChatColor, bool bSendRaid = false);
	void					SendChatRaid(rsPLAYINFO* pcUser, std::string strMessage, EChatColor eChatColor);
	void					LeaveParty(rsPLAYINFO* pcUser, bool bKick = false);
	void					UpdatePartyData(rsPLAYINFO* pcUser, PartyInfo* psParty);

	int						GetWeighedAveragePartyLevel(rsPLAYINFO* pcUser);
	std::vector<rsPLAYINFO*>		GetPartyMembersIncludingSelf(rsPLAYINFO* pcSelf, bool bIncludeRaid = false, int iXZDist = -1);
	std::vector<rsPLAYINFO*>		GetPartyMembers(PartyInfo* psParty, bool bRaid = true);
private:
	BOOL					DoParty(rsPLAYINFO* pcUser, rsPLAYINFO* pcOtherUser);
	BOOL					JoinParty(rsPLAYINFO* pcUser, PartyInfo* psParty);
	BOOL					DeleteRaid(PartyInfo* psParty);
	BOOL					DeleteParty(PartyInfo* psParty);
	void					DelegateParty(rsPLAYINFO* pcUser);
	void					ChangePartyMode(PartyInfo* psParty);

	BOOL					DoRaid(rsPLAYINFO* pcUser, rsPLAYINFO* pcOtherUser);
	BOOL					JoinRaid(rsPLAYINFO* pcUser, PartyInfo* psParty);

	void					ArrangePartyMembers(PartyInfo* psParty);
	void					ArrangeRaidParty(PartyInfo* psParty);
	void					UpdateParty(PartyInfo* psParty, bool bUpdateToRaid = false);
};