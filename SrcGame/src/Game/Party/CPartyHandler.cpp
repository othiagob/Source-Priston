#include "stdafx.h"
#include "CPartyHandler.h"
#include <Utils/Logs/utils.h>
#include <GameCore.h>

extern short sinGetLife();
extern short sinGetMana();

CPartyHandler::CPartyHandler(EWindow eID) : CBaseWindow(eID)
{
}

CPartyHandler::CPartyHandler()
{
}

CPartyHandler::~CPartyHandler()
{
	//if (pcPartyWindow)
	//	DELET(pcPartyWindow);
}

void CPartyHandler::HandlePacket(PacketUpdateParty* psPacket)
{
	if (psPacket)
	{
		UpdateSafeMembers(FALSE);

		//Setting Party Data
		ZeroMemory(&sPartyData, sizeof(CPartyData));
		sPartyData.iMembersCount = (int)psPacket->cMembersCount;
		sPartyData.eMode = (EPartyMode)psPacket->cPartyMode;

		//Deleted Party
		if (sPartyData.iMembersCount < 1)
		{
			pcPartyWindow->LeaveParty();
			return;
		}

		//Add Party Members to Party Data
		for (char i = 0; i < psPacket->cMembersCount; i++)
			CopyMemory(&sPartyData.saMembers[i], &psPacket->saMembers[i], sizeof(PartyMember));

		//Party it's in a Raid
		if (psPacket->cRaidCount > 0)
		{
			sPartyData.iRaidCount = (int)psPacket->cRaidCount;

			for (size_t i = 0; i < MAX_RAID - 1; i++)
			{
				sPartyData.saRaid[i].cRaidID = psPacket->saRaid[i].cRaidID;

				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
				{
					if (psPacket->saRaid[i].saMembers[j].sData.iID)
						CopyMemory(&sPartyData.saRaid[i].saMembers[j], &psPacket->saRaid[i].saMembers[j], sizeof(PartyRaidMember));
				}
			}
		}

		//Update Party Window
		if (sPartyData.iMembersCount > 1)
			pcPartyWindow->UpdateParty(&sPartyData);

		UpdateSafeMembers(TRUE);
	}
}

void CPartyHandler::OnResolutionChanged()
{
	if (sPartyData.iMembersCount > 1)
		pcPartyWindow->UpdateParty(&sPartyData);
}

void CPartyHandler::HandlePacket(PacketRequestParty* psPacket)
{
	if (psPacket)
	{
		if (psPacket->eState == PARTYSTATE_Success || psPacket->eState == PARTYSTATE_RequestInvite)
			pcPartyWindow->ShowPartyNotification(psPacket->szCharacterName, psPacket->eState == PARTYSTATE_Success ? 1 : 2);

		CopyMemory(&sPacketRequestParty, psPacket, sizeof(PacketRequestParty));
	}
}

void CPartyHandler::HandlePacket(PacketUpdatePartyData* psPacket)
{
	if (psPacket)
	{
		UpdateSafeMembers(FALSE);

		//Find a Party member on PartyData of Client side to update the data
		if (psPacket->cMembersCount > 0)
			for (char i = 0; i < psPacket->cMembersCount; i++)
				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
					if (sPartyData.saMembers[j].sData.iID == psPacket->saMembers[i].iID)
						CopyMemory(&sPartyData.saMembers[j].sData, &psPacket->saMembers[i], sizeof(PartyMemberData));

		//Find a Raid Party Member on PartyData of Client side to update the data
		for (size_t i = 0; i < MAX_RAID - 1; i++)
			for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
				if (psPacket->saRaidMember[i][j].iID != 0)
					for (size_t k = 0; k < MAX_RAID - 1; k++)
						for (size_t l = 0; l < MAX_PARTY_MEMBERS; l++)
							if (sPartyData.saRaid[k].saMembers[l].sData.iID == psPacket->saRaidMember[i][j].iID)
								CopyMemory(&sPartyData.saRaid[k].saMembers[l].sData, &psPacket->saRaidMember[i][j], sizeof(PartyRaidMemberData));

		UpdateSafeMembers(TRUE);
	}
}

void CPartyHandler::HandlePacket(PacketRequestRaid* psPacket)
{
	if (psPacket)
	{
		if (psPacket->eState == PARTYSTATE_Success)
			pcPartyWindow->ShowRaidNotification(psPacket->szCharacterName);

		CopyMemory(&sPacketRequestRaid, psPacket, sizeof(PacketRequestRaid));
	}
}

void CPartyHandler::ProcessPacket(PacketJoinParty* psPacket)
{
	psPacket->size = sizeof(PacketJoinParty);
	psPacket->code = smTRANSCODE_PARTY_JOIN;
	smWsockDataServer->Send((char*)psPacket, psPacket->size, TRUE);

	ZeroMemory(&sPacketRequestParty, sizeof(PacketRequestParty));
}

void CPartyHandler::ProcessPacket(PacketJoinRaid* psPacket)
{
	psPacket->size = sizeof(PacketJoinRaid);
	psPacket->code = smTRANSCODE_JOIN_RAID;
	smWsockDataServer->Send((char*)psPacket, psPacket->size, TRUE);

	ZeroMemory(&sPacketRequestRaid, sizeof(PacketRequestParty));
}

void CPartyHandler::ProcessPacket(PacketRequestParty* psPacket)
{
	psPacket->size = sizeof(PacketRequestParty);
	psPacket->code = smTRANSCODE_PARTY_REQUEST;
	smWsockDataServer->Send((char*)psPacket, psPacket->size, TRUE);

	ZeroMemory(&sPacketRequestParty, sizeof(PacketRequestParty));
}

void CPartyHandler::ProcessPacket(PacketRequestRaid* psPacket)
{
	psPacket->size = sizeof(PacketRequestRaid);
	psPacket->code = smTRANSCODE_REQUEST_RAID;
	smWsockDataServer->Send((char*)psPacket, psPacket->size, TRUE);

	ZeroMemory(&sPacketRequestRaid, sizeof(PacketRequestRaid));
}

void CPartyHandler::ProcessPacket(PacketActionParty* psPacket)
{
	psPacket->size = sizeof(PacketActionParty);
	psPacket->code = smTRANSCODE_PARTY_COMMAND;
	smWsockDataServer->Send((char*)psPacket, psPacket->size, TRUE);
}

void CPartyHandler::Init()
{
	pcPartyWindow = new CPartyWindow();
	ZeroMemory(&sPartyData, sizeof(CPartyData));

	INIT(pcPartyWindow);
}

void CPartyHandler::Render()
{
	if (pcPartyWindow)
		pcPartyWindow->Render();
}

void CPartyHandler::Update(float fTime)
{
	UNITDATA->bPartyLeader = false;

	for (int i = 0; i < sPartyData.iMembersCount; i++)
	{
		smCHAR* p = FindChrPlayer(sPartyData.saMembers[i].sData.iID);

		if (p && p->Flag)
		{
			sPartyData.saMembers[i].sData.life[0] = p->dwObjectSerial == UNITDATA->dwObjectSerial ? sinGetLife() : p->smCharInfo.Life[0];
			sPartyData.saMembers[i].sData.life[1] = p->dwObjectSerial == UNITDATA->dwObjectSerial ? UNITDATA->smCharInfo.Life[1] : p->smCharInfo.Life[1];
			sPartyData.saMembers[i].sData.mana[0] = p->dwObjectSerial == UNITDATA->dwObjectSerial ? sinGetMana() : p->smCharInfo.Mana[0];
			sPartyData.saMembers[i].sData.mana[1] = p->dwObjectSerial == UNITDATA->dwObjectSerial ? UNITDATA->smCharInfo.Mana[1] : p->smCharInfo.Mana[1];
			sPartyData.saMembers[i].sData.sLevel = p->smCharInfo.Level;
			sPartyData.saMembers[i].sData.sPosition = p->sPosition;

			// Líder da PT
			if (p->dwObjectSerial == UNITDATA->dwObjectSerial && i == 0)
				UNITDATA->bPartyLeader = true;
		}
	}

	for (int i = 0; i < sPartyData.iRaidCount; i++)
	{
		for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
		{
			smCHAR* p = FindChrPlayer(sPartyData.saRaid[i].saMembers[j].sData.iID);

			if (p && p->Flag)
			{
				sPartyData.saRaid[i].saMembers[j].sData.life[0] = p->dwObjectSerial == lpCurPlayer->dwObjectSerial ? sinGetLife() : p->smCharInfo.Life[0];
				sPartyData.saRaid[i].saMembers[j].sData.life[1] = p->dwObjectSerial == lpCurPlayer->dwObjectSerial ? lpCurPlayer->smCharInfo.Life[1] : p->smCharInfo.Life[1];
				sPartyData.saRaid[i].saMembers[j].sData.sPosition = p->sPosition;
				continue;
			}
		}
	}

	UpdateSafeMembers(TRUE);

	if (pcPartyWindow)
		pcPartyWindow->Update(fTime);
}

void CPartyHandler::OnMouseMove(CMouse* pcMouse)
{
	if (pcPartyWindow)
		pcPartyWindow->OnMouseMove(pcMouse);
}

BOOL CPartyHandler::OnMouseClick(CMouse* pcMouse)
{
	BOOL bRet = FALSE;

	if (pcPartyWindow)
		bRet = pcPartyWindow->OnMouseClick(pcMouse);

	return bRet;
}

void CPartyHandler::AcceptPartyRequest()
{
	sPacketRequestParty.eState = PARTYSTATE_Success;
	ProcessPacket(&sPacketRequestParty);
}

void CPartyHandler::JoinPartyRequest()
{
	PacketJoinParty p;
	p.iID = sPacketRequestParty.iID;
	ProcessPacket(&p);
}

void CPartyHandler::JoinRaidRequest()
{
	PacketJoinRaid p;
	p.iID = sPacketRequestRaid.iID;
	ProcessPacket(&p);
}

BOOL CPartyHandler::IsPartyMember(int iID)
{
	for (int i = 0; i < sPartyData.iMembersCount; i++)
		if (sPartyData.saMembers[i].sData.iID == iID)
			return TRUE;

	return FALSE;
}

BOOL CPartyHandler::IsRaidMember(int iID)
{
	for (int i = 0; i < sPartyData.iRaidCount; i++)
		for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
			if (sPartyData.saRaid[i].saMembers[j].sData.iID == iID)
				return TRUE;

	return FALSE;
}

smCHAR* CPartyHandler::GetDeadPartyMember(bool bRaid)
{
	smCHAR* DeadUser = nullptr;

	if (sPartyData.iMembersCount > 0)
	{
		for (int i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			if (sPartyData.saMembers[i].sData.iID && sPartyData.saMembers[i].sData.life[0] == 0)
			{
				DeadUser = FindAutoPlayer(sPartyData.saMembers[i].sData.iID);

				if (DeadUser)
					break;

			}
		}

		// Não encontrou na PT, procura na RAID
		if (bRaid && !DeadUser)
		{
			for (int i = 0; i < MAX_RAID - 1; i++)
			{
				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
				{
					if (sPartyData.saRaid[i].saMembers[j].sData.iID != 0 && sPartyData.saRaid[i].saMembers[j].szCharacterName[0] != 0)
					{
						smCHAR* DeadUser = FindAutoPlayer(sPartyData.saRaid[i].saMembers[j].sData.iID);

						if (DeadUser)
							break;
					}
				}

				if (DeadUser)
					break;
			}
		}
	}

	return DeadUser;
}

std::vector<smCHAR*> CPartyHandler::GetPartyMembers(bool bRaid)
{
	std::vector<smCHAR*> v;

	if (sPartyData.iMembersCount > 0)
	{
		for (int i = 0; i < MAX_PARTY_MEMBERS; i++)
			if (sPartyData.saMembers[i].sData.iID)
				v.push_back(FindChrPlayer(sPartyData.saMembers[i].sData.iID));

		if (bRaid)
		{
			for (int i = 0; i < MAX_RAID - 1; i++)
				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
					if (sPartyData.saRaid[i].saMembers[j].sData.iID != 0 && sPartyData.saRaid[i].saMembers[j].szCharacterName[0] != 0)
						v.push_back(FindChrPlayer(sPartyData.saRaid[i].saMembers[j].sData.iID));
		}
	}

	return v;
}

void CPartyHandler::UpdateSafeMembers(BOOL bSafe)
{
	std::vector<smCHAR*> vMembers = GetPartyMembers();

	for (auto& v : vMembers)
	{
		if (v && v != UNITDATA)
		{
			if (bSafe)
			{
				if (PARTYHANDLER->IsPartyMember(v->dwObjectSerial))
					v->PartyFlag = 1;
				else if (PARTYHANDLER->IsRaidMember(v->dwObjectSerial))
					v->PartyFlag = 2;
			}
			else
				v->PartyFlag = 0;
		}
	}
}
