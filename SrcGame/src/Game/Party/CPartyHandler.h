#pragma once

#include "Party/PartyHeader.h"
#include "HUD/Party/CPartyWindow.h"

class CPartyHandler: public CBaseWindow
{
public:
	static              CPartyHandler* GetInstance() { static CPartyHandler instance; return &instance; }

	//Constructor
	CPartyHandler(EWindow eID);
	CPartyHandler();
	~CPartyHandler();

	//Handle Packets
	void							HandlePacket(PacketUpdateParty* psPacket);
	void							HandlePacket(PacketRequestParty* psPacket);
	void							HandlePacket(PacketUpdatePartyData* psPacket);
	void							HandlePacket(PacketRequestRaid* psPacket);

	void							ProcessPacket(PacketJoinParty* psPacket);
	void							ProcessPacket(PacketJoinRaid* psPacket);
	void							ProcessPacket(PacketRequestParty* psPacket);
	void							ProcessPacket(PacketRequestRaid* psPacket);
	void							ProcessPacket(PacketActionParty* psPacket);
	void							OnResolutionChanged();
	//Getters and Setters
	CPartyData						GetPartyData() { return sPartyData; }

	void							Init();
	void							Render();

	void							Update(float fTime);

	void							OnMouseMove(class CMouse* pcMouse);
	BOOL							OnMouseClick(class CMouse* pcMouse);

	void							AcceptPartyRequest();
	void							JoinPartyRequest();
	void							JoinRaidRequest();

	BOOL							IsPartyMember(int iID);
	BOOL							IsRaidMember(int iID);

	std::vector<smCHAR*>			GetPartyMembers(bool bRaid = true);
	smCHAR*							GetDeadPartyMember(bool bRaid = true);
private:
	CPartyWindow* pcPartyWindow;
	CPartyData						sPartyData;

	PacketRequestParty			    sPacketRequestParty;
	PacketRequestRaid				sPacketRequestRaid;

	void							UpdateSafeMembers(BOOL bSafe);
};

