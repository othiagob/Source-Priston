#include "CPartyHandler.h"
#include <Utils/Logs/utils.h>

CPartyHandler::CPartyHandler()
{
}

CPartyHandler::~CPartyHandler()
{
}

void CPartyHandler::ProcessPacket(rsPLAYINFO* pcUser, PacketRequestParty* psPacket)
{
	psPacket->size = sizeof(PacketRequestParty);
	psPacket->code = smTRANSCODE_PARTY_REQUEST;
	SENDPACKET(pcUser, (char*)psPacket, psPacket->size, TRUE);
}

void CPartyHandler::ProcessPacket(rsPLAYINFO* pcUser, PacketUpdateParty* psPacket)
{
	psPacket->size = sizeof(PacketUpdateParty);
	psPacket->code = smTRANSCODE_PARTY_UPDATE;
	SENDPACKET(pcUser, (char*)psPacket, psPacket->size, TRUE);
}

void CPartyHandler::ProcessPacket(rsPLAYINFO* pcUser, PacketUpdatePartyData* psPacket)
{
	psPacket->size = sizeof(PacketUpdatePartyData);
	psPacket->code = smTRANSCODE_PARTY_COMMAND;
	SENDPACKET(pcUser, (char*)psPacket, psPacket->size, TRUE);
}

void CPartyHandler::ProcessPacket(rsPLAYINFO* pcUser, PacketRequestRaid* psPacket)
{
	psPacket->size = sizeof(PacketRequestRaid);
	psPacket->code = smTRANSCODE_REQUEST_RAID;
	SENDPACKET(pcUser, (char*)psPacket, psPacket->size, TRUE);
}

void CPartyHandler::HandlePacket(rsPLAYINFO* pcUser, PacketRequestParty* psPacket)
{
	if (!pcUser && !psPacket)
		return;

	if (psPacket->iTargetID && psPacket->eState != PARTYSTATE_Failed)
	{
		rsPLAYINFO* pcTargetUser = srFindUserFromSerial(psPacket->iTargetID);

		if (!pcTargetUser || !pcUser)
			return;

		int l_PlayerLevelA = pcUser->smCharInfo.Level;
		int l_PlayerLevelB = pcTargetUser->smCharInfo.Level;

		int l_Difference = std::abs(l_PlayerLevelA - l_PlayerLevelB);

		if (l_Difference > 25)
		{
			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Diferença de Nível muito grande!");
		}

		if (l_Difference <= 25)
		{
			//User found?
			if (pcTargetUser)
			{
				if (pcTargetUser->PartyBlocked)
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s desabilitou o pedido de Grupo!", pcTargetUser->smCharInfo.szName);
					return;
				}

				//Request Party to myself
				if (pcUser == pcTargetUser)
				{
					SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Você não pode enviar uma solicitação de Grupo para si mesmo.");
					return;
				}

				//Party disabled on User
				if (pcTargetUser->PartyBlocked)
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s desativou o pedido de Grupo.", pcTargetUser->smCharInfo.szName);
					return;
				}

				//User already on Party?
				if (pcUser->bParty)
				{
					if (pcUser->psParty)
					{
						if (pcTargetUser->bParty && pcTargetUser->psParty)
						{
							psPacket->eState = PARTYSTATE_AlreadyParty;
							ProcessPacket(pcUser, psPacket);

							SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s já está em um Grupo!", pcTargetUser->smCharInfo.szName);
							return;
						}

						//User it's a Party Leader?
						if (pcUser->psParty->pcLeader == pcUser)
						{
							psPacket->iID = pcUser->dwObjectSerial;
							psPacket->eState = PARTYSTATE_Success;
							STRINGCOPY(psPacket->szCharacterName, pcUser->smCharInfo.szName);
							ProcessPacket(pcTargetUser, psPacket);

							SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Pedido de Grupo enviado para %s!", pcTargetUser->smCharInfo.szName);
						}
						else
						{
							psPacket->iTargetID = pcTargetUser->dwObjectSerial;
							psPacket->eState = PARTYSTATE_RequestInvite;
							STRINGCOPY(psPacket->szCharacterName, pcTargetUser->smCharInfo.szName);
							ProcessPacket(pcUser->psParty->pcLeader, psPacket);

							SendChatParty(pcUser, FormatString("> %s recomendou %s para entrar no Grupo!", pcUser->smCharInfo.szName, pcTargetUser->smCharInfo.szName), CHATCOLOR_Error);
						}
					}
				}
				else if (pcTargetUser->bParty)
				{
					if (pcTargetUser->psParty)
					{
						//Target it's a Leader from Party?
						if (pcTargetUser->psParty->pcLeader == pcTargetUser)
						{
							if (pcTargetUser->psParty->iMembersCount == MAX_PARTY_MEMBERS)
							{
								psPacket->eState = PARTYSTATE_FullParty;
								ProcessPacket(pcUser, psPacket);

								SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Grupo está cheio!");
								return;
							}
							else if (pcTargetUser->psParty == pcUser->psParty)
							{
								psPacket->eState = PARTYSTATE_AlreadyParty;
								ProcessPacket(pcUser, psPacket);

								SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s já está em um Grupo!", pcTargetUser->smCharInfo.szName);
							}
							else
							{
								psPacket->iID = pcUser->dwObjectSerial;
								psPacket->eState = PARTYSTATE_Success;
								STRINGCOPY(psPacket->szCharacterName, pcUser->smCharInfo.szName);
								ProcessPacket(pcTargetUser, psPacket);

								SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Pedido de Grupo enviado para %s!", pcTargetUser->smCharInfo.szName);
							}
						}
						else
						{
							psPacket->eState = PARTYSTATE_AlreadyParty;
							ProcessPacket(pcUser, psPacket);

							SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s já está em um Grupo!", pcTargetUser->smCharInfo.szName);
							return;
						}
					}
				}
				else
				{
					psPacket->iID = pcUser->dwObjectSerial;
					psPacket->eState = PARTYSTATE_Success;
					STRINGCOPY(psPacket->szCharacterName, pcUser->smCharInfo.szName);
					ProcessPacket(pcTargetUser, psPacket);

					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Pedido de Grupo enviado para %s!", pcTargetUser->smCharInfo.szName);
				}
			}
		}
	}
}

void CPartyHandler::HandlePacket(rsPLAYINFO* pcUser, PacketJoinParty* psPacket)
{
	if (!pcUser || !psPacket)
		return;

	rsPLAYINFO* pcOtherUser = srFindUserFromSerial(psPacket->iID);

	if (pcOtherUser && pcOtherUser->lpsmSock)
	{
		if (!pcUser->bParty && !pcOtherUser->bParty)
			DoParty(pcUser, pcOtherUser);

		else if (!pcUser->bParty && pcOtherUser->bParty)
			JoinParty(pcUser, pcOtherUser->psParty);

		else if (pcUser->psParty && !pcOtherUser->bParty)
			JoinParty(pcOtherUser, pcUser->psParty);
	}
}

void CPartyHandler::HandlePacket(rsPLAYINFO* pcUser, PacketActionParty* psPacket)
{
	if (!pcUser || !psPacket)
		return;

	if (!pcUser->bParty || !pcUser->psParty)
		return;

	switch (psPacket->eAction)
	{
	case PARTYACTION_Leave:
		this->LeaveParty(pcUser);
		break;
	case PARTYACTION_Kick:
		if (pcUser->psParty->pcLeader == pcUser)
		{
			rsPLAYINFO* pcOtherUser = srFindUserFromSerial(psPacket->iUserID);
			if (pcOtherUser && pcOtherUser->lpsmSock)
			{
				//Same party?
				if ((pcOtherUser->bParty && pcOtherUser->psParty) && pcUser->psParty == pcOtherUser->psParty)
					this->LeaveParty(pcOtherUser, true);
			}
		}
		break;
	case PARTYACTION_DisbandParty:
		if (pcUser->psParty->pcLeader == pcUser)
			this->DeleteParty(pcUser->psParty);
		break;

	case PARTYACTION_DisbandRaid:
		if (pcUser->psParty->pcLeader == pcUser)
			this->DeleteParty(pcUser->psParty);
		break;

	case PARTYACTION_Delegate:
		if (pcUser->psParty->pcLeader == pcUser)
		{
			rsPLAYINFO* pcOtherUser = srFindUserFromSerial(psPacket->iUserID);
			if (pcOtherUser && pcOtherUser->lpsmSock)
			{
				//Same party?
				if ((pcOtherUser->bParty && pcOtherUser->psParty) && pcUser->psParty == pcOtherUser->psParty)
					this->DelegateParty(pcOtherUser);
			}
		}
		break;
	case PARTYACTION_ChangeMode:
		if (pcUser->psParty->pcLeader == pcUser)
			this->ChangePartyMode(pcUser->psParty);
		break;
	}
}

void CPartyHandler::HandlePacket(rsPLAYINFO* pcUser, PacketRequestRaid* psPacket)
{
	if (!pcUser || !psPacket)
		return;

	rsPLAYINFO* pcTargetUser = srFindUserFromSerial(psPacket->iTargetID);

	if (pcTargetUser)
	{
		//Both in a party?
		if ((pcUser->bParty && pcUser->psParty) && (pcTargetUser->bParty && pcTargetUser->psParty))
		{
			//Are both leaders of Party?
			if (pcUser->psParty->pcLeader == pcUser && pcTargetUser->psParty->pcLeader == pcTargetUser)
			{
				//Raid isn't full?
				if ((pcUser->psParty->iRaidCount < MAX_RAID - 1) && (pcTargetUser->psParty->iRaidCount < MAX_RAID - 1))
				{
					psPacket->iID = pcUser->dwObjectSerial;
					psPacket->eState = RAIDSTATE_Success;
					STRINGCOPY(psPacket->szCharacterName, pcUser->smCharInfo.szName);
					ProcessPacket(pcTargetUser, psPacket);

					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Pedido de Raid enviado para %s!", pcTargetUser->smCharInfo.szName);
				}
				else
				{
					SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> O Raid já está cheio!");
					return;
				}
			}
			else
			{
				SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> O Raid deve ser formado entre os líderes dos Grupos!");
				return;
			}
		}
	}
}

void CPartyHandler::HandlePacket(rsPLAYINFO* pcUser, PacketJoinRaid* psPacket)
{
	if (!pcUser || !psPacket)
		return;

	rsPLAYINFO* pcOtherUser = srFindUserFromSerial(psPacket->iID);
	if ((pcUser->bParty && pcUser->psParty) && pcOtherUser && pcOtherUser->lpsmSock && pcOtherUser->psParty)
	{
		if (!pcUser->psParty->pcRaidLeader && !pcOtherUser->psParty->pcRaidLeader)
			DoRaid(pcOtherUser, pcUser);
		else if (pcOtherUser->psParty->pcRaidLeader == pcOtherUser->psParty && !pcUser->psParty->pcRaidLeader)
			JoinRaid(pcUser, pcOtherUser->psParty);
	}
}

void CPartyHandler::SendChatParty(rsPLAYINFO* pcUser, std::string strMessage, EChatColor eChatColor, bool bSendRaid)
{
	if (pcUser && pcUser->bParty && pcUser->psParty)
	{
		//Send message to Party Members
		for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			rsPLAYINFO* p = i == 0 ? pcUser->psParty->pcLeader : pcUser->psParty->pcaMembers[i - 1];

			if (p && p->lpsmSock)
				SERVERCHAT->SendChat(p, eChatColor, strMessage.c_str());
		}

		//Send message to others Party (Raid)
		if (pcUser->psParty->iRaidCount > 0 && bSendRaid)
		{
			for (size_t i = 0; i < MAX_RAID; i++)
			{
				PartyInfo* psParty = i == 0 ? pcUser->psParty->pcRaidLeader : pcUser->psParty->pcaRaid[i - 1];
				if (psParty && pcUser->psParty != psParty)
				{
					for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
					{
						rsPLAYINFO* p = j == 0 ? psParty->pcLeader : psParty->pcaMembers[j - 1];

						if (p && p->lpsmSock)
							SERVERCHAT->SendChat(p, eChatColor, strMessage.c_str());
					}
				}
			}
		}
	}
}

void CPartyHandler::SendChatRaid(rsPLAYINFO* pcUser, std::string strMessage, EChatColor eChatColor)
{
	if (pcUser && pcUser->psParty && pcUser->bParty)
	{
		//Send message to others Party (Raid)
		if (pcUser->psParty->iRaidCount > 0)
		{
			for (size_t i = 0; i < MAX_RAID; i++)
			{
				PartyInfo* psParty = i == 0 ? pcUser->psParty->pcRaidLeader : pcUser->psParty->pcaRaid[i - 1];
				if (psParty)
				{
					for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
					{
						rsPLAYINFO* p = j == 0 ? psParty->pcLeader : psParty->pcaMembers[j - 1];

						if (p && p->lpsmSock)
							SERVERCHAT->SendChat(p, eChatColor, strMessage.c_str());
					}
				}
			}
		}
	}
}

void CPartyHandler::LeaveParty(rsPLAYINFO* pcUser, bool bKick)
{
	if (pcUser)
	{
		if (pcUser->bParty)
		{
			PartyInfo* psParty = pcUser->psParty;

			//Valid Party?
			if (psParty)
			{
				//Have more than one member?
				if (psParty->iMembersCount > 2)
				{
					//It's a party Leader?
					if (psParty->pcLeader == pcUser)
					{
						psParty->pcLeader = psParty->pcaMembers[0];
						psParty->pcaMembers[0] = NULL;

						ArrangePartyMembers(psParty);
						psParty->iMembersCount--;

						UpdateParty(psParty);

						//Delete Party from User
						pcUser->bParty = FALSE;
						pcUser->psParty = NULL;
						ProcessPacket(pcUser, &PacketUpdateParty());

						SendChatParty(psParty->pcLeader, FormatString("> %s saiu do Grupo.", pcUser->smCharInfo.szName), CHATCOLOR_Error);
						SendChatParty(psParty->pcLeader, FormatString("> %s foi delegado como Novo Líder.", psParty->pcLeader->smCharInfo.szName), CHATCOLOR_Error);
					}
					else
					{
						//Update Party Settings
						for (size_t i = 0; i < MAX_PARTY_MEMBERS - 1; i++)
						{
							if (psParty->pcaMembers[i] == pcUser)
							{
								psParty->pcaMembers[i] = NULL;

								ArrangePartyMembers(psParty);
								psParty->iMembersCount--;

								UpdateParty(psParty);

								SendChatParty(psParty->pcLeader, FormatString(bKick ? "> %s foi expulso do Grupo." : "> %s saiu do Grupo.", pcUser->smCharInfo.szName), CHATCOLOR_Error);
							}
						}

						//Delete Party from User
						pcUser->bParty = FALSE;
						pcUser->psParty = NULL;
						ProcessPacket(pcUser, &PacketUpdateParty());
					}
				}
				else
					DeleteParty(psParty);
			}
		}
	}
}

void CPartyHandler::UpdatePartyData(rsPLAYINFO* pcUser, PartyInfo* psParty)
{
	if (psParty && pcUser && psParty->pcLeader)
	{
		PacketUpdatePartyData sPacket;
		ZeroMemory(&sPacket, sizeof(PacketUpdatePartyData));

		//Update Party Data
		for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			rsPLAYINFO* p = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];

			if (p && p->lpsmSock && p != pcUser)
			{
				bool bUpdate = false;

				//Map it's different or User it's longer? So update it
				if (p->Position.Area != pcUser->Position.Area)
					bUpdate = true;
				else if (!p->Position.Position.WithinXZDistance(&pcUser->Position.Position, DISTANCE_MAX_PARTY, DISTANCE_MISC_Y_EX))
					bUpdate = true;

				//Update Member Data
				if (bUpdate)
				{
					sPacket.saMembers[sPacket.cMembersCount].iID = p->dwObjectSerial;
					sPacket.saMembers[sPacket.cMembersCount].sLevel = p->smCharInfo.Level;
					sPacket.saMembers[sPacket.cMembersCount].life[0] = p->smCharInfo.Life[0];
					sPacket.saMembers[sPacket.cMembersCount].life[1] = p->smCharInfo.Life[1];
					sPacket.saMembers[sPacket.cMembersCount].mana[0] = p->smCharInfo.Mana[0];
					sPacket.saMembers[sPacket.cMembersCount].mana[1] = p->smCharInfo.Mana[1];
					sPacket.saMembers[sPacket.cMembersCount].sPosition.iX = p->Position.x;
					sPacket.saMembers[sPacket.cMembersCount].sPosition.iY = p->Position.y;
					sPacket.saMembers[sPacket.cMembersCount].sPosition.iZ = p->Position.z;

					sPacket.cMembersCount++;
				}
			}
		}

		//Update Raid Party Data
		if (psParty->iRaidCount > 0)
		{
			int iIndex = 0;
			for (size_t i = 0; i < MAX_RAID; i++)
			{
				PartyInfo* pRaidParty = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i - 1];
				if (pRaidParty && pRaidParty != psParty)
				{
					int iRaidMemberCount = 0;
					for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
					{
						rsPLAYINFO* p = j == 0 ? pRaidParty->pcLeader : pRaidParty->pcaMembers[j - 1];
						if (p && p->lpsmSock && p != pcUser)
						{
							bool bUpdate = false;

							//Map it's different or User it's longer? So update it
							if (p->Position.Area != pcUser->Position.Area)
								bUpdate = true;
							else if (!p->Position.Position.WithinXZDistance(&pcUser->Position.Position, DISTANCE_MAX_PARTY, DISTANCE_MISC_Y_EX))
								bUpdate = true;

							//Update Member Data
							if (bUpdate)
							{
								sPacket.saRaidMember[iIndex][iRaidMemberCount].iID = p->dwObjectSerial;
								sPacket.saRaidMember[iIndex][iRaidMemberCount].life[0] = p->smCharInfo.Life[0];
								sPacket.saRaidMember[iIndex][iRaidMemberCount].life[1] = p->smCharInfo.Life[1];
								sPacket.saRaidMember[iIndex][iRaidMemberCount].sPosition = p->Position.Position;
								iRaidMemberCount++;
								sPacket.cRaidMembersCount++;
							}
						}
					}

					iIndex++;
				}
			}
		}

		if (sPacket.cMembersCount > 0 || sPacket.cRaidMembersCount > 0)
			ProcessPacket(pcUser, &sPacket);
	}
}

/// <summary>
/// Calculate the weighed average of players level in the party
/// this adds more weighing on higher levels
/// </summary>
int CPartyHandler::GetWeighedAveragePartyLevel(rsPLAYINFO* pcUser)
{
	if (pcUser == NULL || pcUser->lpsmSock == FALSE)
		return 0;

	if (pcUser->psParty == NULL)
		return pcUser->smCharInfo.Level;

	vector<rsPLAYINFO*> pMembers = PARTYHANDLER->GetPartyMembersIncludingSelf(pcUser, FALSE); //this will include leader too

	float fTotalLevel = 0;
	for (rsPLAYINFO* pMember : pMembers)
	{
		fTotalLevel += static_cast<float>(pMember->smCharInfo.Level);
	}

	float fWeightedTotal = 0;

	for (rsPLAYINFO* pMember : pMembers)
	{
		fWeightedTotal += (static_cast<float>(pMember->smCharInfo.Level) / fTotalLevel) * static_cast<float>(pMember->smCharInfo.Level);
	}

	return static_cast<int>(round(fWeightedTotal));
}

std::vector<rsPLAYINFO*> CPartyHandler::GetPartyMembersIncludingSelf(rsPLAYINFO* pcSelf, bool bIncludeRaid, int iXZDist)
{
	std::vector<rsPLAYINFO*> v;

	if (pcSelf->bParty)
	{
		for (rsPLAYINFO* pcMember : GetPartyMembers(pcSelf->psParty, bIncludeRaid))
		{
			if (iXZDist == -1)
			{
				v.push_back(pcMember);
			}
			else
			{
				if (pcMember->Position.Position.WithinXZDistance(&pcSelf->Position.Position, iXZDist, DISTANCE_MISC_Y_EX))
				{
					v.push_back(pcMember);
				}
			}
		}
	}
	else
	{
		v.push_back(pcSelf);
	}

	return v;
}

std::vector<rsPLAYINFO*> CPartyHandler::GetPartyMembers(PartyInfo* psParty, bool bRaid)
{
	std::vector<rsPLAYINFO*> v;
	if (psParty)
	{
		//Get Party Members
		if (psParty->iMembersCount > 1)
		{
			for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
			{
				rsPLAYINFO* u = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];

				if (u && u->lpsmSock)
				{
					v.push_back(u);
				}
			}
		}

		//Get Raid Members
		if (bRaid && psParty->iRaidCount > 0)
		{
			for (size_t i = 0; i < MAX_RAID; i++)
			{
				PartyInfo* p = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i - 1];
				if (p && p != psParty)
				{
					for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
					{
						rsPLAYINFO* u = j == 0 ? p->pcLeader : p->pcaMembers[j - 1];
						if (u && u->lpsmSock)
						{
							v.push_back(u);
						}
					}
				}
			}
		}
	}

	return v;
}

BOOL CPartyHandler::DoParty(rsPLAYINFO* pcUser, rsPLAYINFO* pcOtherUser)
{
	if (!pcOtherUser)
		return FALSE;

	PartyInfo* psParty = new PartyInfo;
	ZeroMemory(psParty, sizeof(PartyInfo));

	psParty->iID = GetTickCount();
	psParty->iMembersCount = 1;
	psParty->pcLeader = pcOtherUser;

	if (pcUser)
	{
		psParty->pcaMembers[psParty->iMembersCount - 1] = pcUser;
		psParty->iMembersCount++;

		//Setting Other User
		pcUser->bParty = TRUE;
		pcUser->psParty = psParty;
	}

	//Setting User
	pcOtherUser->bParty = TRUE;
	pcOtherUser->psParty = psParty;

	//Update Party
	UpdateParty(psParty);

	return TRUE;
}

BOOL CPartyHandler::JoinParty(rsPLAYINFO* pcUser, PartyInfo* psParty) // here
{
	if (pcUser && psParty)
	{
		if (psParty->iMembersCount >= MAX_PARTY_MEMBERS)
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Grupo está cheio!");
			return FALSE;
		}

		psParty->pcaMembers[psParty->iMembersCount - 1] = pcUser;
		psParty->iMembersCount++;

		//Setting Other User
		pcUser->bParty = TRUE;
		pcUser->psParty = psParty;

		//Update Party
		UpdateParty(psParty);

		SendChatParty(pcUser, FormatString("> %s entrou no Grupo!", pcUser->smCharInfo.szName), CHATCOLOR_Error);
	}

	return TRUE;
}

BOOL CPartyHandler::DeleteParty(PartyInfo* psParty)
{
	if (psParty)
	{
		//Delete party from Raid
		if (psParty->iRaidCount > 0)
		{
			//A loop to make a verification on each party inside Raid
			for (size_t i = 0; i < MAX_RAID; i++)
			{
				PartyInfo* p = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i - 1];
				if (p && p != psParty)
				{
					//Party is leaving its a raid leader?
					if (p->pcRaidLeader == psParty)
					{
						p->pcRaidLeader = NULL;

						if (p->pcaRaid[0] != p)
						{
							p->pcRaidLeader = p->pcaRaid[0];
							p->pcaRaid[0] = NULL;
							ArrangeRaidParty(p);
						}

						p->iRaidCount--;
					}
					else
					{
						//Check Raid array to delete the party from this
						for (size_t j = 0; j < MAX_RAID - 1; j++)
						{
							PartyInfo* pPartyToDelete = p->pcaRaid[j];
							if (pPartyToDelete == psParty)
							{
								p->pcaRaid[j] = NULL;
								p->iRaidCount--;
								ArrangeRaidParty(p);
								break;
							}
						}
					}

					//Don't have Raid Leader anymore? So delete Raid
					if (!p->pcRaidLeader || p->iRaidCount < 1)
					{
						p->pcRaidLeader = NULL;
						p->iRaidCount = 0;
						ZeroMemory(p->pcaRaid, 4 * (MAX_RAID - 1));
						SendChatParty(p->pcLeader, "> O Raid foi desfeito!", CHATCOLOR_Error, false);
					}
					else
						SendChatParty(p->pcLeader, "> O Grupo saiu do Raid.", CHATCOLOR_Error, true);
				}
			}
		}

		psParty->iMembersCount = 0;

		UpdateParty(psParty);

		//Delete party from User
		for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			rsPLAYINFO* p = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];
			if (p)
			{
				p->bParty = NULL;
				p->psParty = NULL;
			}
		}

		SAFE_DELETE(psParty);

		return TRUE;
	}

	return FALSE;
}

BOOL CPartyHandler::DeleteRaid(PartyInfo* psUserPartyInfo)
{
	if (psUserPartyInfo)
	{
		//Delete party from Raid
		if (psUserPartyInfo->iRaidCount > 0)
		{
			//A loop to make a verification on each party inside Raid
			for (size_t i = 0; i < MAX_RAID; i++)
			{
				PartyInfo* p = i == 0 ? psUserPartyInfo->pcRaidLeader : psUserPartyInfo->pcaRaid[i - 1];

				if (p)
				{
					p->pcRaidLeader = NULL;
					p->iRaidCount = 0;
					ZeroMemory(p->pcaRaid, 4 * (MAX_RAID - 1));

					UpdateParty(p);

					SendChatParty(p->pcLeader, "> Raid foi desfeito!", CHATCOLOR_Error, false);
				}
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CPartyHandler::DelegateParty(rsPLAYINFO* pcUser)
{
	if (pcUser->bParty && pcUser->psParty)
	{
		rsPLAYINFO* pLeader = pcUser->psParty->pcLeader;
		pcUser->psParty->pcLeader = pcUser;

		for (size_t i = 0; i < MAX_PARTY_MEMBERS - 1; i++)
		{
			rsPLAYINFO* p = pcUser->psParty->pcaMembers[i];

			if (p && p->lpsmSock)
			{
				if (p->smCharInfo.dwObjectSerial == pcUser->smCharInfo.dwObjectSerial)
				{
					pcUser->psParty->pcaMembers[i] = pLeader;
					SendChatParty(pcUser, FormatString("> %s delegou %s como Novo Líder!", pLeader->smCharInfo.szName, pcUser->smCharInfo.szName), CHATCOLOR_Error);
					break;
				}
			}
		}

		UpdateParty(pcUser->psParty);
	}
}

void CPartyHandler::ChangePartyMode(PartyInfo* psParty)
{
	if (psParty)
	{
		psParty->iMode = (psParty->iMode + 1) % 3;

		char* pszPartyMode = GetPartyMode((EPartyMode)psParty->iMode);

		for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			rsPLAYINFO* p = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];

			if (p && p->lpsmSock)
				SERVERCHAT->SendChatEx(p, CHATCOLOR_Error, "> O Grupo mudou para %s!", pszPartyMode);
		}

		UpdateParty(psParty);
	}
}

BOOL CPartyHandler::DoRaid(rsPLAYINFO* pcUser, rsPLAYINFO* pcOtherUser)
{
	if (!pcUser || !pcOtherUser)
		return FALSE;

	PartyInfo* psParty1 = pcUser->psParty;
	PartyInfo* psParty2 = pcOtherUser->psParty;

	if (!psParty1 || !psParty2)
		return FALSE;

	//Update Party 1 Info
	psParty1->iRaidCount = 1;
	psParty1->pcRaidLeader = psParty1;
	psParty1->pcaRaid[0] = psParty2;

	//Update Party 2 Info
	psParty2->iRaidCount = 1;
	psParty2->pcRaidLeader = psParty1;
	psParty2->pcaRaid[0] = psParty2;

	//Update Party Leader from Raid
	UpdateParty(psParty1);

	return TRUE;
}

BOOL CPartyHandler::JoinRaid(rsPLAYINFO* pcUser, PartyInfo* psParty)
{
	if (!pcUser || !psParty)
		return FALSE;

	PartyInfo* psPartyLeader = pcUser->psParty;

	if (!psParty || !psPartyLeader)
		return FALSE;

	if (psPartyLeader->iRaidCount < (MAX_RAID - 1))
	{
		for (int i = 0; i < psPartyLeader->iRaidCount; i++)
		{
			psPartyLeader->pcaRaid[i]->pcaRaid[psPartyLeader->iRaidCount] = psParty;
			psPartyLeader->pcaRaid[i]->iRaidCount++;

			psParty->pcaRaid[i] = psPartyLeader->pcaRaid[i];
		}

		psPartyLeader->iRaidCount++;
		psParty->iRaidCount = psPartyLeader->iRaidCount;
		psParty->pcRaidLeader = psPartyLeader;
	}
	return TRUE;
}

void CPartyHandler::ArrangePartyMembers(PartyInfo* psParty)
{
	if (psParty)
	{
		rsPLAYINFO* pcaNewVetor[MAX_PARTY_MEMBERS - 1] = { 0 };
		int iIndex = 0;

		for (size_t i = 0; i < MAX_PARTY_MEMBERS - 1; i++)
		{
			rsPLAYINFO* p = psParty->pcaMembers[i];

			if (p && p->lpsmSock)
			{
				pcaNewVetor[iIndex] = p;
				iIndex++;
			}
		}

		CopyMemory(psParty->pcaMembers, pcaNewVetor, 4 * (MAX_PARTY_MEMBERS - 1));
	}
}

void CPartyHandler::ArrangeRaidParty(PartyInfo* psParty)
{
	if (psParty)
	{
		PartyInfo* pcaNewVetor[MAX_RAID - 1] = { 0 };
		int iIndex = 0;

		for (size_t i = 0; i < MAX_RAID - 1; i++)
		{
			PartyInfo* p = psParty->pcaRaid[i];
			if (p)
			{
				pcaNewVetor[iIndex] = p;
				iIndex++;
			}
		}

		CopyMemory(psParty->pcaRaid, pcaNewVetor, 4 * (MAX_RAID - 1));
	}
}

void CPartyHandler::UpdateParty(PartyInfo* psParty, bool bUpdateToRaid)
{
	if (psParty)
	{
		//Prepare Leader Party info to send on packet
		if (psParty->pcLeader)
		{
			PacketUpdateParty sPacket;
			ZeroMemory(&sPacket, sizeof(PacketUpdateParty));

			sPacket.cMembersCount = (char)psParty->iMembersCount;
			sPacket.cPartyMode = (char)psParty->iMode;

			//Prepare Party Info
			for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
			{
				rsPLAYINFO* pcMember = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];

				if (pcMember && pcMember->lpsmSock)
				{
					CopyMemory(sPacket.saMembers[i].szCharacterName, pcMember->smCharInfo.szName, 32);
					sPacket.saMembers[i].sData.iID = pcMember->dwObjectSerial;
					sPacket.saMembers[i].cCharacterClass = (char)pcMember->smCharInfo.JOB_CODE;
				}
			}

			//Prepare Raid Party Info
			if (psParty->iRaidCount > 0)
			{
				sPacket.cRaidCount = (char)psParty->iRaidCount;

				int iIndex = 0;
				for (size_t i = 0; i < MAX_RAID; i++)
				{
					PartyInfo* pRaidParty = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i - 1];
					if (pRaidParty && pRaidParty != psParty)
					{
						sPacket.saRaid[iIndex].cRaidID = (char)(i + 1);

						for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
						{
							rsPLAYINFO* u = j == 0 ? pRaidParty->pcLeader : pRaidParty->pcaMembers[j - 1];
							if (u && u->lpsmSock)
							{
								STRINGCOPY(sPacket.saRaid[iIndex].saMembers[j].szCharacterName, u->smCharInfo.szName);
								sPacket.saRaid[iIndex].saMembers[j].sData.iID = u->dwObjectSerial;
							}
						}

						if (!bUpdateToRaid)
							UpdateParty(pRaidParty, true);

						iIndex++;
					}
				}
			}

			//Process Packets to Update Party Client-Side
			for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
			{
				rsPLAYINFO* u = i == 0 ? psParty->pcLeader : psParty->pcaMembers[i - 1];

				if (u && u->lpsmSock)
				{
					//Party it's over?
					if (psParty->iMembersCount == 0)
						SERVERCHAT->SendChat(u, CHATCOLOR_Error, "> O Grupo foi desfeito.");

					ProcessPacket(u, &sPacket);
				}
			}
		}
	}
}