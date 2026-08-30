#define CE_CMAKE_CPP

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include "Character/playmain.h"
#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		


#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"
#include "../../SrcLang/jts.h"

#endif

#include "tjclan.h"
#include "cE_Cmake.h"
#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"



int usehFocus = 0;
int oldLIMITchattingWnd = 0;
extern char	szConnServerName[16];
extern int g_ClanMakeMoney;



CLANEntity_make::CLANEntity_make()
{
	m_bEventClanMoney = false;
}
CLANEntity_make::~CLANEntity_make()
{

}



int CLANEntity_make::Init(int Life)
{
	life = Life;
	ZeroMemory(szClanMakeMsg, sizeof(szClanMakeMsg));
	return 1;
}

int CLANEntity_make::Clear()
{
	return 1;
}



void CLANEntity_make::Main()
{
	char szFile[256];
	char szFile2[256];
	char server;
	char UniqueClanMark[64];
	int  nMoney = 500000;
	if(life == 0) return;

	int i;
	switch(C_menuN)
	{
		case CLAN_ABILITYDOWN:
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
			
			i = i;
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;



				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANMAKE_REAMMAKR_ERROR:
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
			
			i = i;
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;



				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLAN_GO_MAKE:
		C_menuN = CLAN_isCheckClanJang;
		menuInit(C_menuN);
		C_menuN2 = 1;
		break;
		case CLAN_isCheckClanJang_LOADING:
		if(ANSdata.ret_val != -1) C_menuN = CLAN_isCheckClanJang;
		break;



		case CLAN_isCheckClanJang:
		switch(C_menuN2)
		{
			case 1:
			ANSdata.ret_val = -1;
			WebDB.Resume();
			WebDB.isCheckClanJang(cldata.szID, cldata.gserver);
			C_menuN2 = 2;
			break;
			case 2:






			switch(ANSdata.ret_val)
			{
				case -1:


				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = 0;


				C_menuN = CLAN_isCheckClanJang_Result;
				menuInit(C_menuN);

				break;
				case 0:
				WebDB.Suspend();
				C_menuN2 = 0;
				C_menuN = CLAN_MAKE_is;
				menuInit(C_menuN);

				cldata.myPosition = CLANNO_REGESTER;
				ZeroMemory(cldata.name, sizeof(cldata.name));
				ZeroMemory(cldata.explanation, sizeof(cldata.explanation));
				ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name));
				ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark));
				cldata.intClanMark = 0;
				ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));


				break;

				case 100:

				WebDB.Suspend();
				C_menuN2 = 0;


				wsprintf(szErrorMsg, "%s (1)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				wsprintf(szErrorMsg, "%s (0)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN2 = 0;

					wsprintf(szErrorMsg, "%s (2)", clanszError_Msg.szMsg);
					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = CLAN_AllEnd;
				}
				break;
			}
			break;
		}
		break;
		case CLANMAKE_NOMONEY:
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
			
			i = i;
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;



				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLAN_isCheckClanJang_Result:
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
			
			i = i;
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;



				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;











		case CLAN_MAKE_is:
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
		}

		
		{
			
			switch(i)
			{
				case 0:








				if((cldata.money - g_ClanMakeMoney) < 0)
				{



					C_menuN = CLANMAKE_NOMONEY;
					menuInit(C_menuN);



					CLAN_ERROR_ret = CLAN_AllEnd;
					break;
				}


				if(cldata.ability < ABILITY)
				{


					C_menuN = CLAN_ABILITYDOWN;
					menuInit(C_menuN);
					CLAN_ERROR_ret = CLAN_AllEnd;
					break;
				}



				C_menuN = CLAN_MONEY_is;
				menuInit(C_menuN);

				break;

				case 1:


				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanCancelMakeClan.szMsg);
				cClanReport.menuInit(C_menuN, clanCancelMakeClan.pos.x, clanCancelMakeClan.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;

				default:
				break;
			}
		}
		break;

		case CLAN_MONEY_is:
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









				C_menuN = CLAN_NAME_is;
				menuInit(C_menuN);
				C_menuN2 = -1;
				break;

				case 1:



				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanCancelMakeClan.szMsg);
				cClanReport.menuInit(C_menuN, clanCancelMakeClan.pos.x, clanCancelMakeClan.pos.y);

				CLAN_ERROR_ret = CLAN_AllEnd;
				break;

				default:
				break;
			}
		}
		break;

		case CLAN_NAME_is:



		switch(C_menuN2)
		{
			case 1:
			ANSdata.ret_val = -1;
			WebDB.Resume();
			WebDB.isCheckClanName(cldata.name, cldata.gserver);
			C_menuN2 = 2;
			break;
			case 2:

			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN2 = 0;
				C_menuN = CLAN_MARK_expl;;
				menuInit(C_menuN);

				break;
				case 1:

				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_NAME_BEING;
				menuInit(C_menuN);
				break;

				case 100:
				WebDB.Suspend();
				C_menuN2 = 0;

				wsprintf(szErrorMsg, "%s (3)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;

				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				wsprintf(szErrorMsg, "%s (4)", clanszError_Msg.szMsg);

				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN2 = 0;
					wsprintf(szErrorMsg, "%s (5)", clanszError_Msg.szMsg);
					C_menuN = CLAN_ERROR;
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = CLAN_AllEnd;
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


			break;
			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}

		
		{
			
			switch(i)
			{
				case 0:
					


				if(cldata.name[0] == 0)
				{
				#ifdef USE_PROSTONTALE		

					usehFocus = 0;
					

				#endif
					C_menuN = CLAN_NONAME;
					menuInit(C_menuN);
					break;
				}
				C_menuN2 = 1;
			#ifdef USE_PROSTONTALE		

				usehFocus = 0;
			
			#endif

				break;


				default:
				break;
			}
		}
		break;
		case CLAN_NONAME:
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
				C_menuN = CLAN_NAME_is;
				menuInit(C_menuN);


				break;
			}
		}
		break;
		case CLAN_NAME_BEING:
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


		break;

		case CLAN_MARK_expl:

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


		break;


		case CLAN_INSPECTION_expl:


		switch(C_menuN2)
		{
			case 1:
			ANSdata.ret_val = -1;
			WebDB.Resume();
		#ifdef USE_PROSTONTALE		
			WebDB.make_Clan(cldata.szID, cldata.gserver, cldata.ChaName,
							cldata.name,
							"설명없음.", sinChar->JOB_CODE, sinChar->Level);
		#else
			WebDB.make_Clan(cldata.szID, cldata.gserver, cldata.ChaName,
							cldata.name,
							"설명없음.", 1, 50);
		#endif
			C_menuN2 = 2;
			break;
			case 2:






			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();



				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (6)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 1:

				WebDB.Suspend();
				C_menuN2 = 0;
				cldata.myPosition = CLANCHIP;
				C_menuN = CLANMAKE_DOWN_INFO;
				menuInit(C_menuN);






			#ifdef USE_PROSTONTALE		



				int sinMinusMoney(int Money, int Kind = 0);
				sinMinusMoney(g_ClanMakeMoney);








			#endif






				break;
				case 2:
				WebDB.Suspend();
				C_menuN2 = 0;

				wsprintf(szErrorMsg, "%s (7)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_NAME_is;
				break;
				case 3:
				WebDB.Suspend();
				C_menuN2 = 0;


				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (8)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				CLAN_ERROR_ret = CLAN_NAME_is;
				break;

				case 100:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (9)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (10)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN2 = 0;
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (11)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = CLAN_AllEnd;
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
			case 3:
			menuMouseOver[3] = 1;
			break;
			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}

		
		{
			
			switch(i)
			{
				case 0:


				C_menuN2 = 1;
				break;
				case 3:


				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(clanCancelMakeClan.szMsg);
				cClanReport.menuInit(C_menuN, clanCancelMakeClan.pos.x, clanCancelMakeClan.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;

				break;
			}
		}
		break;

		case CLAN_NotMake:

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
			}
		}
		break;
		case CLANMAKE_DOWN_INFO:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.isClanMember(cldata.szID, cldata.gserver, cldata.ChaName);
			C_menuN2 = 1;
			break;
			case 1:




			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				bIsLoading = FALSE;
				C_menuN = CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;

				break;


				case 1:
				WebDB.Suspend();
				bIsLoading = FALSE;

				ParsingIsClanMember(ANSdata.ret_Buf);
				cldata.myPosition = CLANUSER;
				C_menuN2 = 0;



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


				ParsingIsClanMember(ANSdata.ret_Buf);
				WriteMyClanInfo();
				C_menuN2 = 0;
				C_menuN = CLANMAKE_READ_CLANMARK;
				menuInit(C_menuN);


				cldata.myPosition = CLANCHIP;
			#ifdef VOICE_CHAT
				if(bCheckClan)
				{
					g_CtrlProcess.SetSelectCha(cldata.szID, cldata.ChaName,
											   cldata.name, (DWORD)cldata.intClanMark, cldata.myPosition);


				}
			#endif

				break;
				case 3:
				WebDB.Suspend();
				ParsingIsClanMember2(ANSdata.ret_Buf);
				bIsLoading = FALSE;
				C_menuN2 = 0;
				C_menuN = CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;

			#ifdef VOICE_CHAT
				if(bCheckClan)
				{
					g_CtrlProcess.SetSelectCha(cldata.szID, cldata.ChaName,
											   NULL, 0, cldata.myPosition);


				}
			#endif

				break;
				case 100:
				WebDB.Suspend();
				cldata.myPosition = CLAN_NOCONNECT;
				C_menuN = CLAN_AllEnd;
				menuInit(C_menuN);
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				cldata.myPosition = CLAN_NOCONNECT;
				C_menuN = CLAN_AllEnd;
				menuInit(C_menuN);

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					bIsLoading = FALSE;
					C_menuN2 = 0;
					C_menuN = CLAN_AllEnd;
					cldata.myPosition = CLAN_NOCONNECT;
				}
			}
		}
		break;

		case CLANMAKE_READ_CLANMARK:


		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.MarkREAD(cldata.name, cldata.ClanMark);
			C_menuN2 = 1;
			break;
			case 1:

			if(ANSdata.ret_val != -1)
				ANSdata.ret_val = ANSdata.ret_val;


			switch(ANSdata.ret_val)
			{

				case 1:
				WebDB.Suspend();

				server = cldata.ClanMark[0];
				strcpy(UniqueClanMark, &cldata.ClanMark[3]);
				wsprintf(szFile, "%s\\%s\\%c_%s.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%c_%s_16.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);

				C_menuN = CLAN_MAKE_FINISH;
				menuInit(C_menuN);
				C_menuN2 = -1;



				break;
				case 2:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (65)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);






				C_menuN2 = -1;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;

				wsprintf(szErrorMsg, "%s (66)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);








				C_menuN2 = -1;
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();

					C_menuN = CLAN_AllEnd;
					C_menuN2 = -1;
					CLAN_ERROR_ret = -1;
				}
				break;
			}
		}
		break;
		case CLAN_MAKE_FINISH:
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
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;




				break;
			}
		}
		break;
	}

}



void CLANEntity_make::RenderMain()
{
	if(life == 0) return;

}





void CLANEntity_make::menuInit(int num)
{
	int bx = 0, by = 0;

	switch(num)
	{
		case CLAN_ABILITYDOWN:
		menuSu = 3;
	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else			
		bx = 64;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanAbilityErrorMsg.pos.x;
		menuPos[1].top = BackStartPos.y + clanAbilityErrorMsg.pos.y;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		break;
		case CLANMAKE_REAMMAKR_ERROR:
		menuSu = 3;
	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else			
		bx = 64;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + 30;
		menuPos[1].top = BackStartPos.y + 40;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		break;
		case CLANMAKE_NOMONEY:
		menuSu = 3;
	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else			
		bx = 64;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanMoneyErrorMsg.pos.x;
		menuPos[1].top = BackStartPos.y + clanMoneyErrorMsg.pos.y;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		break;
		case CLAN_isCheckClanJang_Result:
		menuSu = 3;
	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else			
		bx = 64;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + 18;
		menuPos[1].top = BackStartPos.y + 40;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		break;
		case CLANMAKE_DOWN_INFO:
		menuPos[0].left = BackStartPos.x + clanClanMakedowninfo.pos.x;
		menuPos[0].top = BackStartPos.y + clanClanMakedowninfo.pos.y;
		menuPos[0].right = 0;
		menuPos[0].bottom = 0;
		break;
		case CLANMAKE_READ_CLANMARK:
		menuPos[0].left = BackStartPos.x + clanClanMakedowninfo.pos.x;
		menuPos[0].top = BackStartPos.y + clanClanMakedowninfo.pos.y;
		menuPos[0].right = 0;
		menuPos[0].bottom = 0;
		break;

		case CLAN_isCheckClanJang:

		menuPos[0].left = BackStartPos.x + clanLoading.pos.x;
		menuPos[0].top = BackStartPos.y + clanLoading.pos.y;
		menuPos[0].right = 0;
		menuPos[0].bottom = 0;
		break;
		case CLAN_MAKE_is:
		menuSu = 2;

	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else			
		bx = 32;
		by = 16;
	#endif

		menuPos[0].left = BackStartPos.x + BUTTON2_X;
		menuPos[0].top = BackStartPos.y + +BUTTON2_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;



	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else			
		bx = 32;
		by = 16;
	#endif

		menuPos[1].left = BackStartPos.x + BUTTON3_X;
		menuPos[1].top = BackStartPos.y + BUTTON3_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;


		menuPos[2].left = BackStartPos.x + clanMake1.pos.x;
		menuPos[2].top = BackStartPos.y + clanMake1.pos.y;
		menuPos[2].right = menuPos[2].left + 280;
		menuPos[2].bottom = menuPos[2].top + 250;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else			
		bx = 68;
		by = 27;
	#endif

		menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;
		menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;

	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else			
		bx = 68;
		by = 27;
	#endif
		menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;
		menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;
		menuPos[4].right = menuPos[4].left + bx;
		menuPos[4].bottom = menuPos[4].top + by;

		break;
		case CLAN_MONEY_is:
		menuSu = 2;
	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif

		menuPos[0].left = BackStartPos.x + BUTTON2_X;
		menuPos[0].top = BackStartPos.y + BUTTON2_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;



	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif

		menuPos[1].left = BackStartPos.x + BUTTON3_X;
		menuPos[1].top = BackStartPos.y + BUTTON3_Y;
		menuPos[1].right = menuPos[1].left + bx;
		menuPos[1].bottom = menuPos[1].top + by;


		menuPos[2].left = BackStartPos.x + clanMake2.pos.x;
		menuPos[2].top = BackStartPos.y + clanMake2.pos.y;
		menuPos[2].right = menuPos[2].left + 280;
		menuPos[2].bottom = menuPos[2].top + 250;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[3].left = BackStartPos.x + BUTTON2_BOX_X;
		menuPos[3].top = BackStartPos.y + BUTTON2_BOX_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;

	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;
		menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;
		menuPos[4].right = menuPos[4].left + bx;
		menuPos[4].bottom = menuPos[4].top + by;

		break;
		case CLAN_NAME_is:

		menuSu = 2;

	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else
		bx = 32;
		by = 16;
	#endif

		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;





		menuPos[2].left = BackStartPos.x + 60;
		menuPos[2].top = BackStartPos.y + 40;
		menuPos[2].right = menuPos[2].left + 190;
		menuPos[2].bottom = menuPos[2].top + 180;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[3].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[3].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;

	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else

		bx = 68;
		by = 27;
	#endif

		menuPos[4].left = BackStartPos.x + CLANINPUTNAME_X;
		menuPos[4].top = BackStartPos.y + CLANINPUTNAME_Y;
		menuPos[4].right = menuPos[4].left + bx;
		menuPos[4].bottom = menuPos[4].top + by;


		break;
		case CLAN_NONAME:

		menuSu = 1;
	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanNoName.pos.x;
		menuPos[1].top = BackStartPos.y + clanNoName.pos.y;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;
		case CLAN_NAME_BEING:
		menuSu = 3;
	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanClanNameError.pos.x;
		menuPos[1].top = BackStartPos.y + clanClanNameError.pos.y;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;
		case CLAN_MARK_expl:
		menuSu = 1;

	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else
		bx = 32;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanMake4.pos.x;
		menuPos[1].top = BackStartPos.y + clanMake4.pos.y;
		menuPos[1].right = menuPos[1].left + 190;
		menuPos[1].bottom = menuPos[1].top + 180;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;
		case CLAN_INSPECTION_expl:
		menuSu = 5;

	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON2_X;
		menuPos[0].top = BackStartPos.y + BUTTON2_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanMake5.pos.x;
		menuPos[1].top = BackStartPos.y + clanMake5.pos.y;
		menuPos[1].right = menuPos[1].left + 260;
		menuPos[1].bottom = menuPos[1].top + 180;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[2].left = BackStartPos.x + BUTTON2_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON2_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif
		menuPos[3].left = BackStartPos.x + BUTTON3_X;
		menuPos[3].top = BackStartPos.y + BUTTON3_Y;
		menuPos[3].right = menuPos[3].left + bx;
		menuPos[3].bottom = menuPos[3].top + by;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[4].left = BackStartPos.x + BUTTON3_BOX_X;
		menuPos[4].top = BackStartPos.y + BUTTON3_BOX_Y;
		menuPos[4].right = menuPos[4].left + bx;
		menuPos[4].bottom = menuPos[4].top + by;
		break;
		case CLAN_NotMake:
		menuSu = 3;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 64;
	#else
		bx = 64;
		by = 64;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanMsg254.pos.x;
		menuPos[1].top = BackStartPos.y + clanMsg254.pos.y;
		menuPos[1].right = menuPos[1].left + 260;
		menuPos[1].bottom = menuPos[1].top + 180;


	#ifdef USE_PROSTONTALE		
		bx = 68;
		by = 27;
	#else
		bx = 68;
		by = 27;
	#endif

		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;
		case CLAN_MAKE_FINISH:
		menuSu = 3;

	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;
	#else

		bx = 32;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + clanMake6.pos.x;
		menuPos[1].top = BackStartPos.y + clanMake6.pos.y;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		bx = 64;
		by = 16;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;
		break;


	}
}

void CLANEntity_make::Draw() {}




void KeyFilter(char* szBuffer) {}