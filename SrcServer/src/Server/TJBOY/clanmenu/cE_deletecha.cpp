#define CE_DELETECHA_CPP
#include <windows.h>

#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		
#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"

#endif


#include "tjclan.h"
#include "cE_deletecha.h"
#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"



CLANEntity_deletecha::CLANEntity_deletecha()
{
	nHowDeleteChaFlag = 0;
}

CLANEntity_deletecha::~CLANEntity_deletecha()
{
}


int CLANEntity_deletecha::Init(int Life)
{
	life = Life;
	return 1;
}


void CLANEntity_deletecha::SetDeleteChaInfo(char *id, char *gserver, char *chaname)
{
	strcpy(cldata.szID, id);
	strcpy(cldata.ChaName, chaname);
	strcpy(cldata.gserver, gserver);
}


int CLANEntity_deletecha::Clear()
{
	return 1;
}




void CLANEntity_deletecha::Main()
{
	int i;
	switch(C_menuN)
	{
		case CLAN_DELETECHA_START:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;
			nHowDeleteChaFlag = 0;

			WebDB.Resume();
			WebDB.isClanMember(cldata.szID, cldata.gserver, cldata.ChaName);
			C_menuN2 = 1;
			break;
			case 1:

			if(ANSdata.ret_val != -1)
				ANSdata.ret_val = ANSdata.ret_val;

			switch(ANSdata.ret_val)
			{
				case 4:
				case 3:
				case 0:
				WebDB.Suspend();
				bIsLoading = FALSE;
				C_menuN2 = -1;
				C_menuN = CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
				nHowDeleteChaFlag = 0;


			#ifdef VOICE_CHAT
				if(bCheckClan)
				{
					g_CtrlProcess.SetSelectCha(cldata.szID, cldata.ChaName,
											   NULL, 0, cldata.myPosition);


				}
			#endif


				break;


				case 1:
				WebDB.Suspend();
				ParsingIsClanMember(ANSdata.ret_Buf);

				if(bFlag)
				{
					C_menuN = CLAN_DELETECHA_SECESSION_BEFORE;
					menuInit(C_menuN);
					cldata.myPosition = CLANUSER;
					nHowDeleteChaFlag = 0;
					C_menuN2 = -1;
				}
				else
				{
					C_menuN = CLAN_DIRECT_DELETE;
					menuInit(C_menuN);
					cldata.myPosition = CLANUSER;
					nHowDeleteChaFlag = 0;
					C_menuN2 = 0;

				}

			#ifdef VOICE_CHAT
				if(bCheckClan)
				{
					g_CtrlProcess.SetSelectCha(cldata.szID, cldata.ChaName,
											   cldata.name, (DWORD)atoi(cldata.ClanMark), cldata.myPosition);


				}
			#endif


				break;

				case 2:
				WebDB.Suspend();

				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				nHowDeleteChaFlag = 3;
				break;

				case 100:
				WebDB.Suspend();
				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				nHowDeleteChaFlag = 3;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				nHowDeleteChaFlag = 3;
				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					bIsLoading = FALSE;
					C_menuN2 = 0;
					C_menuN = CLAN_AllEnd;
					cldata.myPosition = CLAN_NOCONNECT;
					C_menuN2 = -1;
					nHowDeleteChaFlag = 3;
				}
				break;
			}
			break;
		}
		break;





		case CLAN_DELETECHA_RELEASECLAN_BEFORE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;
			nHowDeleteChaFlag = 1;

			WebDB.Resume();
			WebDB.BreakUP_Clan(cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name);
			C_menuN2 = 1;
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanDeleteChaError.szMsg);
				cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
				CLAN_ERROR_ret = 255;
				nHowDeleteChaFlag = 3;
				break;
				case 1:
				WebDB.Suspend();
				C_menuN = CLAN_DELETECHA_RELEASECLAN;
				menuInit(C_menuN);
				C_menuN2 = -1;
				nHowDeleteChaFlag = 2;
				break;








				case 100:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanDeleteChaError.szMsg);
				cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
				CLAN_ERROR_ret = 255;
				nHowDeleteChaFlag = 3;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanServerConnectErrorMsg.szMsg);
				cClanReport.menuInit(C_menuN, clanServerConnectErrorMsg.pos.x, clanServerConnectErrorMsg.pos.y);
				nHowDeleteChaFlag = 3;
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();

					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(clanDeleteChaError.szMsg);
					cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
					CLAN_ERROR_ret = 255;
					C_menuN2 = -1;
					nHowDeleteChaFlag = 3;
				}
				break;
			}
			break;
		}

		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;
			case 1:
			menuMouseOver[1] = 1;
			break;
			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;

		}
		
		{
			
			switch(i)
			{
				case 0:
				C_menuN2 = 0;
				break;
				case 1:
				C_menuN = CLAN_AllEnd;
				break;
				default:
				break;
			}
		}
		break;


		case CLAN_DELETECHA_RELEASECLAN:
		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;

			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;

		}

		
		{
			
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_DIRECT_DELETE:
		switch(C_menuN2)
		{

			case 0:
			ANSdata.ret_val = -1;
			nHowDeleteChaFlag = 1;

			WebDB.Resume();

			WebDB.Secession(cldata.szID, cldata.gserver, cldata.ChaName, cldata.name);
			C_menuN2 = 1;
			break;
			case 1:

			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanDeleteChaError.szMsg);
				cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
				CLAN_ERROR_ret = CLAN_USER;
				C_menuN2 = -1;
				nHowDeleteChaFlag = 3;
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;
				C_menuN = CLAN_AllEnd;
				nHowDeleteChaFlag = 2;
				break;









				case 3:
				WebDB.Suspend();

				C_menuN2 = -1;
				C_menuN = CLAN_AllEnd;
				nHowDeleteChaFlag = 2;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanServerConnectErrorMsg.szMsg);
				cClanReport.menuInit(C_menuN, clanServerConnectErrorMsg.pos.x, clanServerConnectErrorMsg.pos.y);
				nHowDeleteChaFlag = 3;
				break;
				case 2:
				case 4:
				case 100:
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();



					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(clanDeleteChaError.szMsg);
					cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
					CLAN_ERROR_ret = CLAN_USER;
					C_menuN2 = -1;
					nHowDeleteChaFlag = 3;
				}
				break;
			}
			break;


		}
		break;
		case CLAN_DELETECHA_SECESSION_BEFORE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;
			nHowDeleteChaFlag = 1;

			WebDB.Resume();

			WebDB.Secession(cldata.szID, cldata.gserver, cldata.ChaName, cldata.name);
			C_menuN2 = 1;
			break;
			case 1:

			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanDeleteChaError.szMsg);
				cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
				CLAN_ERROR_ret = CLAN_USER;
				C_menuN2 = -1;
				nHowDeleteChaFlag = 3;
				break;
				case 1:
				WebDB.Suspend();

				C_menuN = CLAN_DELETECHA_SECESSION;
				menuInit(C_menuN);

				C_menuN2 = -1;
				nHowDeleteChaFlag = 2;
				break;









				case 3:
				WebDB.Suspend();


				C_menuN = CLAN_DELETECHA_SECESSION;
				menuInit(C_menuN);
				C_menuN2 = -1;
				nHowDeleteChaFlag = 2;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanServerConnectErrorMsg.szMsg);
				cClanReport.menuInit(C_menuN, clanServerConnectErrorMsg.pos.x, clanServerConnectErrorMsg.pos.y);
				nHowDeleteChaFlag = 3;
				break;
				case 2:
				case 4:
				case 100:
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();



					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(clanDeleteChaError.szMsg);
					cClanReport.menuInit(C_menuN, clanDeleteChaError.pos.x, clanDeleteChaError.pos.y);
					CLAN_ERROR_ret = CLAN_USER;
					C_menuN2 = -1;
					nHowDeleteChaFlag = 3;
				}
				break;
			}
			break;
		}

		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;
			case 1:
			menuMouseOver[1] = 1;
			break;
			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}

		
		{
			
			switch(i)
			{
				case 1:
				C_menuN = CLAN_AllEnd;
				break;
				case 0:
				C_menuN2 = 0;

				default:
				break;
			}
		}
		break;


		case CLAN_DELETECHA_SECESSION:
		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;

			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;

		}

		
		{
			
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;
				break;
				default:
				break;
			}
		}
		break;
	}
}

void CLANEntity_deletecha::Draw() {}

void CLANEntity_deletecha::menuInit(int num) {}

