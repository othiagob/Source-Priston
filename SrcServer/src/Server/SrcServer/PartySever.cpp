#define _WIN32_WINNT    0x0601
#define _WIN32_WINDOWS  0x0601

#include <windows.h>
#include <io.h>
#include <stdio.h> 
#include <string.h>
#include <memory.h>
#include <process.h> 
#include "Character/record.h"
#include "Resource/resource.h"
#include "smwsock.h"
#include "English/e_PartyMsg.h"
#include "smlib3d\\smd3d.h"
#include "Character/character.h"
#include "fileread.h"
#include "Character/playmain.h"
#include "field.h"
#include "sinbaram\\sinlinkheader.h"
#include "language.h"
#include "onserver.h"
#include "smPacket.h"
#include <Chat/ChatServer.h>
#include <Party/Party.h>
#include "Party/CPartyHandler.h"

extern DWORD	dwPlayServTime;

rsPLAYINFO* srFindUserFromSerial(DWORD dwObjectSerial);
int	rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);


int rsPLAYINFO::SendPartyData(rsPLAYINFO* lpPlayInfo_From, char* szData)
{
	return TRUE;
}

int rsPLAYINFO::SendPartyMessage_Player(std::string message)
{
	return TRUE;
}

int rsPLAYINFO::SendPartyMessage(std::string message, DWORD dwCode)
{

	return TRUE;
}

int rsPLAYINFO::CreateParty(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	rsPLAYINFO::JoinPartyPlayer(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	rsPLAYINFO::DeletePartyPlayer(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	rsPLAYINFO::ChangePartyPlayer(rsPLAYINFO* lpOldPlayInfo, rsPLAYINFO* lpNewPlayInfo)
{
	return TRUE;
}

int	rsPLAYINFO::UpdatePartyPlayer()
{
	return TRUE;
}

int	rsPLAYINFO::UpdatePartyPlayInfo()
{
	return TRUE;
}

int	rsPLAYINFO::UpdateServerPartyPlayer(TRANS_PARTY_PLAYINFO* lpTransPartyPlayInfo)
{
	return TRUE;
}


INT64 srGetTotalExp(INT64 Exp, int Level);

/*
//#ifdef _PLAY_MORYON
int	PartyExpTable[8][2] = {
	{	100	,	0 },
	{	60	,	60 },	//120
	{	48	,	46 },	//140
	{	40	,	40 },	//160
	{	36	,	36 },	//180
	{	35	,	33 },	//200
	{	22	,	21 },	//
	{	20	,	20 }	//
};
*/

float PartyExpTable[8][2] = {
	{ 1.f, 0 },
	{ 0.70f, 0.70f },
	{ 0.60f, 0.60f },
	{ 0.55f, 0.55f },
	{ 0.52f, 0.52f },
	{ 0.50f, 0.50f },
	{ 0.50f, 0.50f },
	{ 0.50f, 0.50f },
};

extern float Multiplicador[151];

int rsPLAYINFO::GetPartyExp(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo)
{
	if (!psParty || !lpsmSock) return FALSE;

	smTRANS_EXP64	smTransCommand;
	INT64	Exp;
	INT64	PartyExp[2];
	int	cnt;
	int x, z, dist;
	int Sucess = 0;
	int	ExpUp;

	smTransCommand.size = sizeof(smTRANS_EXP64);
	smTransCommand.WParam = 0;
	smTransCommand.LParam = lpChar->dwObjectSerial;

	Exp = srGetTotalExp(lpChar->smMonsterInfo.GetExp, lpChar->smCharInfo.Level - PARTYHANDLER->GetWeighedAveragePartyLevel(lpPlayInfo));

	PartyExp[0] = static_cast<int>(Exp * PartyExpTable[lpPlayInfo->psParty->iMembersCount - 1][0]);
	PartyExp[1] = static_cast<int>(Exp * PartyExpTable[lpPlayInfo->psParty->iMembersCount - 1][1]);

	if (lpPlayInfo->psParty->iRaidCount > 0)
	{
		for (int i = 0; i < MAX_RAID; i++)
		{
			PartyInfo* psP = i == 0 ? psParty->pcRaidLeader : psParty->pcaRaid[i - 1];

			if (psP)
			{
				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
				{
					rsPLAYINFO* p = j == 0 ? psP->pcLeader : psP->pcaMembers[j - 1];

					if (p && p->lpsmSock)
					{
						if (p == lpPlayInfo)
						{
							Quest::GetInstance()->updateStatus(lpPlayInfo, lpChar, 1);

							smTransCommand.code = smTRANSCODE_ADDEXP;
							smTransCommand.WParam = PartyExp[0];

							Sucess++;
						}
						else
						{
							smTransCommand.code = smTRANSCODE_ADDEXP_PARTY;
							x = (lpPlayInfo->Position.x - p->Position.x) >> FLOATNS;
							z = (lpPlayInfo->Position.z - p->Position.z) >> FLOATNS;
							dist = x * x + z * z;

							if (dist < PARTY_GETTING_DIST)
							{
								if (p->Position.Area == lpPlayInfo->Position.Area)
								{
									Quest::GetInstance()->updateStatus(p, lpChar, 1);
								}

								smTransCommand.WParam = PartyExp[1];
							}
							else
							{
								smTransCommand.WParam = 0;
							}
						}

						if (smTransCommand.WParam)
						{
							// Verifica se o personagem já atingiu o nível máximo
							if (p->smCharInfo.Level >= g_LevelFinal)
							{
								// Personagem já atingiu o nível máximo, não ganha XP
								smTransCommand.WParam = 0;
							}
							else
							{
								ExpUp = 0;

								if (p->dwTime_PrimeItem_ExpUp > 0)
							{
								switch (p->dwPrimeItem_PackageCode)
								{
								case PRIME_ITEM_PACKAGE_NONE:
									ExpUp += 30;
									break;
								case PRIME_ITEM_PACKAGE_NONE_50_EXPUP:
									ExpUp += 50;
									break;
								case PRIME_ITEM_PACKAGE_SILVER:
									ExpUp += 20;
									break;
								default:
									ExpUp += 30;
									break;
								}
							}

								// Modo exp party
								if (p->psParty)
								{
									if (p->psParty->iMode == EPartyMode::PARTYMODE_EXP)
									{
										ExpUp += 10;
									}
									else if (p->psParty->iMode == EPartyMode::PARTYMODE_Hunt)
									{
										ExpUp -= 10; // Modo hunt
									}
								}

								if (p->vipLevel > 0)
									ExpUp += 10;

								if (p->dwTime_PrimeItem_PhenixPet > 0)
								{
									ExpUp += 20;
								}

								if (rsServerConfig.ExpGameTimeMode) {
									smTransCommand.WParam = (smTransCommand.WParam * rsGetExp_GameTime(p)) / 100;
								}

								if (ExpUp)
								{
									smTransCommand.WParam += (smTransCommand.WParam * ExpUp) / 100;
								}

								if (p->smCharInfo.Level < 100)
								{
									extern int eventoxp;
									smTransCommand.WParam *= eventoxp;
								}

								// Multiplicador de exp (grupo)
								if (Multiplicador[p->smCharInfo.Level] > 0)
								{
									smTransCommand.WParam *= Multiplicador[p->smCharInfo.Level];
								}

								p->spEXP_Out += static_cast<int>(smTransCommand.WParam);
								smTransCommand.SParam = (smTransCommand.WParam * 3 + smTransCommand.LParam * 13) * 2002;
								smTransCommand.EParam = smTransCommand.SParam ^ p->spEXP_Out;

								p->LastExp += smTransCommand.WParam;

								p->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
							} // Fecha o else da verificação de nível máximo
						}
					}
				}
			}
		}
	}
	else
	{
		for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			rsPLAYINFO* p = i == 0 ? lpPlayInfo->psParty->pcLeader : lpPlayInfo->psParty->pcaMembers[i - 1];

			if (p && p->lpsmSock)
			{
				if (p == lpPlayInfo)
				{
					Quest::GetInstance()->updateStatus(lpPlayInfo, lpChar, 1);

					smTransCommand.code = smTRANSCODE_ADDEXP;
					smTransCommand.WParam = PartyExp[0];

					Sucess++;
				}
				else
				{
					smTransCommand.code = smTRANSCODE_ADDEXP_PARTY;
					x = (lpPlayInfo->Position.x - p->Position.x) >> FLOATNS;
					z = (lpPlayInfo->Position.z - p->Position.z) >> FLOATNS;
					dist = x * x + z * z;

					if (dist < PARTY_GETTING_DIST)
					{
						if (p->Position.Area == lpPlayInfo->Position.Area)
						{
							Quest::GetInstance()->updateStatus(p, lpChar, 1);
						}

						smTransCommand.WParam = PartyExp[1];
					}
					else {
						smTransCommand.WParam = 0;
					}
				}

				if (smTransCommand.WParam)
				{
					ExpUp = 0;

					if (p->dwTime_PrimeItem_ExpUp > 0)
					{
						switch (p->dwPrimeItem_PackageCode)
						{
						case PRIME_ITEM_PACKAGE_NONE:
							ExpUp += 30;
							break;
						case PRIME_ITEM_PACKAGE_NONE_50_EXPUP:
							ExpUp += 50;
							break;
						case PRIME_ITEM_PACKAGE_SILVER:
							ExpUp += 20;
							break;
						default:
							ExpUp += 30;
							break;
						}
					}

					if (p->psParty)
					{
						// Modo exp party
						if (p->psParty->iMode == EPartyMode::PARTYMODE_EXP)
						{
							ExpUp += 10;
						}
						else if (p->psParty->iMode == EPartyMode::PARTYMODE_Hunt)
						{
							ExpUp -= 10; // Modo hunt
						}
					}

					if (p->vipLevel > 0)
						ExpUp += 10;

					if (p->dwTime_PrimeItem_PhenixPet > 0)
					{
						ExpUp += 20;
					}

					if (rsServerConfig.ExpGameTimeMode) {
						smTransCommand.WParam = (smTransCommand.WParam * rsGetExp_GameTime(p)) / 100;
					}

					if (ExpUp)
					{
						smTransCommand.WParam += (smTransCommand.WParam * ExpUp) / 100;
					}

					// depois multiplica pelo evento exp se tiver
					if (p->smCharInfo.Level < 100)
					{
						extern int eventoxp;
						smTransCommand.WParam *= eventoxp;
					}

					// e depois o total multiplicado pelo multiplicador entendi sim
					// Multiplicador de exp (grupo)
					if (Multiplicador[p->smCharInfo.Level] > 0)
					{
						smTransCommand.WParam *= Multiplicador[p->smCharInfo.Level];
					}

					p->spEXP_Out += static_cast<int>(smTransCommand.WParam);
					smTransCommand.SParam = (smTransCommand.WParam * 3 + smTransCommand.LParam * 13) * 2002;
					smTransCommand.EParam = smTransCommand.SParam ^ p->spEXP_Out;

					p->LastExp += smTransCommand.WParam;

					p->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				} // Fecha o else da verificação de nível máximo
			}
		}
	}

	if (!Sucess)	return FALSE;

	return TRUE;
}

int rsPLAYINFO::GetPartyMoney(sITEMINFO* lpDefItemInfo, rsPLAYINFO* lpPlayInfo)
{
	TRANS_ITEMINFO	TransItemInfo;
	int	PartyMoney[2];
	int	cnt;
	int x, z, dist;
	int Sucess = 0;

	if (!psParty || !lpsmSock || !lpPlayInfo || !psParty->pcRaidLeader || !psParty->pcaRaid) return FALSE;

	PartyMoney[1] = lpDefItemInfo->Money / (psParty->iMembersCount ? psParty->iMembersCount : 1);
	PartyMoney[0] = PartyMoney[1] + (lpDefItemInfo->Money % psParty->iMembersCount);

	TransItemInfo.code = smTRANSCODE_PUTITEM;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);
	memcpy(&TransItemInfo.Item, lpDefItemInfo, sizeof(sITEMINFO));

	for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
	{
		rsPLAYINFO* p = i == 0 ? lpPlayInfo->psParty->pcLeader : lpPlayInfo->psParty->pcaMembers[i - 1];

		if (p && p->lpsmSock && p->smCharInfo.dwObjectSerial)
		{
			if (p == lpPlayInfo)
			{
				wsprintf(TransItemInfo.Item.ItemName, sinGold, PartyMoney[0]);
				TransItemInfo.Item.Money = PartyMoney[0];
				ReformItem(&TransItemInfo.Item);
				Sucess++;
			}
			else
			{
				x = (lpPlayInfo->Position.x - p->Position.x) >> FLOATNS;
				z = (lpPlayInfo->Position.z - p->Position.z) >> FLOATNS;
				dist = x * x + z * z;

				if (dist < PARTY_GETTING_DIST)
				{
					wsprintf(TransItemInfo.Item.ItemName, sinGold, PartyMoney[1]);
					TransItemInfo.Item.Money = PartyMoney[1];
					ReformItem(&TransItemInfo.Item);
				}
				else
				{
					TransItemInfo.Item.Money = 0;
				}
			}

			if (TransItemInfo.Item.Money)
			{
				p->spMoney_Out += TransItemInfo.Item.Money;

				TransItemInfo.x = dwPlayServTime;
				TransItemInfo.y = TransItemInfo.Item.ItemHeader.dwChkSum;
				TransItemInfo.z = p->spMoney_Out ^ (TransItemInfo.x + TransItemInfo.y);

				rsRegist_ItemSecCode(p, &TransItemInfo, 0);

				p->lpsmSock->Send((char*)&TransItemInfo, TransItemInfo.size, TRUE);
			}
		}

	}

	if (!Sucess)	return FALSE;

	return TRUE;
}


int rsPLAYINFO::PartyChatting(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}


//ÆÄÆ¼Àå ÀÌÀÓ
int	rsPLAYINFO::LeavePartyMaster(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	rsPLAYINFO::ReleaseParty()
{
	return TRUE;
}


//ÆÄÆ¼ °¡ÀÔ
int	rsPLAYINFO::JoinParty(rsPLAYINFO* lpPlayMaster)
{
	return TRUE;
}


//ÆÄÆ¼ Å»Åð
int	rsPLAYINFO::SecedeParty()
{
	return TRUE;
}

rsPLAYINFO* LastDisPlayer;
rsPLAYINFO* LastDisPartyMaster;
DWORD		LastDisPartyInfo;

//ÆÄÆ¼¿ø ¿¬°á ²ö¾îÁü
int	rsPLAYINFO::DisconnectPartyUser()
{
	return TRUE;
}


//////////////////////// ¼­¹ö ¸Ó´Ï °ü¸® ///////////////////////
/*
	int	AddServerMoney( int Money , int WhereParam=0 );		//¼­¹ö ¸Ó´Ï Ãß°¡
	int	SubServerMoney( int Money , int WhereParam=0 );		//¼­¹ö ¸Ó´Ï °¨¼Ò
	int	SetServerMoney( int Money , int WhereParam=0 );		//¼­¹ö ¸Ó´Ï ¼³Á¤
*/

/*
#define	WHERE_TRADE				300
#define	WHERE_GIFT_EXPRESS		600
#define	WHERE_AGINGITEM			400
#define	WHERE_CRAFTITEM			420
#define	WHERE_FORECORB			440
#define	WHERE_BUY_ITEM			200
#define	WHERE_BUY_POTION		210
#define	WHERE_SELL_ITEM			220
#define	WHERE_LOAD_PLAYER		100
#define	WHERE_THROW_ITEM		120
#define	WHERE_GET_ITEM			130
#define	WHERE_STAR_POINT		620
#define	WHERE_CLAN_MONEY		630
#define	WHERE_GIVE_MONEY		640
#define	WHERE_PERSONAL_SHOP		320
#define	WHERE_OPEN_WAREHOUES	500
*/

char* GetWhereParamString(int WareParam)
{
	char* lpString;

	lpString = "Unknow";

#ifdef _W_SERVER

	switch (WareParam) {

	case	WHERE_TRADE:
		lpString = "Trade";	break;
	case	WHERE_GIFT_EXPRESS:
		lpString = "GiftExpress";	break;
	case	WHERE_AGINGITEM:
		lpString = "AgingItem";	break;
	case	WHERE_CRAFTITEM:
		lpString = "CraftItem";	break;
	case	WHERE_FORECORB:
		lpString = "ForceOrb";	break;
	case	WHERE_BUY_ITEM:
		lpString = "BuyItem";	break;
	case	WHERE_BUY_POTION:
		lpString = "BuyPotion";	break;
	case	WHERE_SELL_ITEM:
		lpString = "SellItem";	break;
	case	WHERE_LOAD_PLAYER:
		lpString = "LoadPlayer";	break;
	case	WHERE_THROW_ITEM:
		lpString = "ThrowItem";	break;
	case	WHERE_GET_ITEM:
		lpString = "GetItem";	break;
	case	WHERE_STAR_POINT:
		lpString = "StarPoint";	break;
	case	WHERE_CLAN_MONEY:
		lpString = "ClanMoney";	break;
	case	WHERE_GIVE_MONEY:
		lpString = "GiveMoney";	break;
	case	WHERE_PERSONAL_SHOP:
		lpString = "PersonalShop";	break;
	case	WHERE_OPEN_WAREHOUES:
		lpString = "WareHouse";	break;
	}
#endif

	return lpString;
}


#define	CHK_SERVER_MONEY_MAX	1000000000
#define	CHK_SERVER_MONEY_MAX2	1000000000
#define	CHK_SERVER_MONEY_MIN	-1000000000

int	rsPLAYINFO::AddServerMoney(int Money, int WhereParam)	//¼­¹ö ¸Ó´Ï Ãß°¡
{
	int money = ServerMoney;

	ServerMoney += Money;

	if ((money >= 0 && ServerMoney < 0) || Money<0 || Money>CHK_SERVER_MONEY_MAX) {
		//µ· ¿À¹ö ÇÃ·Î¿ì ( ·Î±× ³²±â±â )

		smTRANS_COMMAND_EX	smTransCommandEx;

		char szStrBuff[128];
		wsprintf(szStrBuff, "*ADD_MONEY [%s]*", GetWhereParamString(WhereParam));

		smTransCommandEx.WParam = 8840;
		smTransCommandEx.WxParam = WhereParam;
		smTransCommandEx.LxParam = (int)szStrBuff;
		smTransCommandEx.LParam = money;
		smTransCommandEx.SParam = Money;
		smTransCommandEx.EParam = ServerMoney;
		RecordHackLogFile(this, &smTransCommandEx);

		ServerMoney = 0;

		return FALSE;
	}

	return TRUE;
}

int	rsPLAYINFO::SubServerMoney(int Money, int WhereParam)	//¼­¹ö ¸Ó´Ï °¨¼Ò
{
	int money = ServerMoney;

	ServerMoney -= Money;

	if ((money >= 0 && ServerMoney < 0) || Money<0 || Money>CHK_SERVER_MONEY_MAX) {
		//µ· ¿À¹ö ÇÃ·Î¿ì ( ·Î±× ³²±â±â )

		smTRANS_COMMAND_EX	smTransCommandEx;

		char szStrBuff[128];
		wsprintf(szStrBuff, "*SUB_MONEY [%s]*", GetWhereParamString(WhereParam));

		smTransCommandEx.WParam = 8840;
		smTransCommandEx.WxParam = WhereParam;
		smTransCommandEx.LxParam = (int)szStrBuff;
		smTransCommandEx.LParam = money;
		smTransCommandEx.SParam = Money;
		smTransCommandEx.EParam = ServerMoney;
		RecordHackLogFile(this, &smTransCommandEx);


		ServerMoney = 0;


		return FALSE;
	}

	return TRUE;
}

int	rsPLAYINFO::SetServerMoney(int Money, int WhereParam)	//¼­¹ö ¸Ó´Ï ¼³Á¤
{
	ServerMoney = Money;

	if (Money > CHK_SERVER_MONEY_MAX || Money < 0) {
		//µ· ¿À¹ö ÇÃ·Î¿ì ( ·Î±× ³²±â±â )

		smTRANS_COMMAND_EX	smTransCommandEx;

		char szStrBuff[128];
		wsprintf(szStrBuff, "*SET_MONEY [%s]*", GetWhereParamString(WhereParam));

		smTransCommandEx.WParam = 8840;
		smTransCommandEx.WxParam = WhereParam;
		smTransCommandEx.LxParam = (int)szStrBuff;
		smTransCommandEx.LParam = 0;
		smTransCommandEx.SParam = Money;
		smTransCommandEx.EParam = ServerMoney;
		RecordHackLogFile(this, &smTransCommandEx);


		ServerMoney = 0;

		return FALSE;
	}

	return TRUE;
}


int	rsPLAYINFO::CallBack_ClanMarkNum(int ClanMarkNum)			//Å¬·£¸¶Å© ¹øÈ£ ¾Ë·ÁÁÜ
{
	DWORD dwCode;
	smTRANS_COMMAND	smTransCommand;

	if (ClanMarkNum == -1) return TRUE;

	dwCode = GetClanCode(ClanMarkNum);
	if (dwClanCode != dwCode) {

		smTransCommand.WParam = 8870;
		smTransCommand.LParam = dwClanCode;
		smTransCommand.SParam = dwCode;
		smTransCommand.EParam = rsBlessCastle.dwMasterClan;
		RecordHackLogFile(this, &smTransCommand);

		SucessCheck_ClanCode = -1;
		//Å¬·£ ¸¶Å© ÄÚµå°¡ Æ²¸®´Ù
		return FALSE;
	}

	dwClanCode = dwCode;
	SucessCheck_ClanCode = TRUE;

	return TRUE;
}
