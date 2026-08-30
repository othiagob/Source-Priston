#define CLAN_EITI_CPP
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>

#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		
#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"

#include "cE_CViewClanInfo.h"

#endif

#include "tjclan.h"
#include "clan_enti.h"
#include "cE_report.h"

#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"

extern char szConnServerName[16];

#ifdef USE_PROSTONTALE		


#else						


#endif						







int g_ClanMakeMoney = 0;


#ifdef PARKMENU
extern int g_nMicBtn;
#endif

CLANEntity::CLANEntity()
{
	life = 0;
	bImageDown = FALSE;







#ifdef USE_PROSTONTALE		
	hBt_Help = NULL;
	hMark_box = NULL;
	::ZeroMemory(hSecession, sizeof(hSecession));
	::ZeroMemory(hClose, sizeof(hClose));
	hIcon = NULL;
	hIcon_Click = NULL;
	hHelp1 = NULL;
	hClinetMark = NULL;
	hServerMark = NULL;
	::ZeroMemory(hWareHouse, sizeof(hWareHouse));
	::ZeroMemory(hWHRight, sizeof(hWHRight));
	::ZeroMemory(hClanMark, sizeof(hClanMark));
	::ZeroMemory(hRemove, sizeof(hRemove));
	::ZeroMemory(hClanRelease, sizeof(hClanRelease));
	::ZeroMemory(hHelp, sizeof(hHelp));
	::ZeroMemory(hOut, sizeof(hOut));

	hButton_Box = NULL;
	hClanTitle = NULL;
	::ZeroMemory(hRemoveToMenu, sizeof(hRemoveToMenu));
	hScl_Bar = NULL;
	hScl_icon = NULL;
	::ZeroMemory(hClanHelpIcon, sizeof(hClanHelpIcon));
	hClanNameInput = NULL;

#ifndef _LANGUAGE_BRAZIL	
	::ZeroMemory(hTxt_Remove, sizeof(hTxt_Remove));
	::ZeroMemory(hTxt_Release, sizeof(hTxt_Release));
#endif
	::ZeroMemory(hTxt_Mark, sizeof(hTxt_Mark));
	::ZeroMemory(hTxt_Help, sizeof(hTxt_Help));
	::ZeroMemory(hTxt_Secession, sizeof(hTxt_Secession));

#ifndef _LANGUAGE_BRAZIL	
	::ZeroMemory(hTxt_LeaveClan, sizeof(hTxt_LeaveClan));
#endif


	::ZeroMemory(szHelpMsg1, sizeof(szHelpMsg1));
	::ZeroMemory(szErrorMsg, sizeof(szErrorMsg));
#endif


}


CLANEntity::~CLANEntity()
{
}




int CLANEntity::Init()
{
	return 1;
}
int CLANEntity::Clear()
{
	return 1;
}



extern BOOL bGetChaClan;

extern int nStatusCount;

void CLANEntity::Main()
{
	char szFile[255];
	char szFile2[255];


	HANDLE fp, fp2;
	fp = fp = NULL;

	char server[4];
	char UniqueClanMark[64];
	int nClanMark;
	int nClanMarklen = 0;
	ZeroMemory(server, sizeof(server));

	int i;

	switch(C_menuN)
	{
		case CLAN_NOLEVEL:

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
		case CLANSERVER_INSPECT_BEFORE:
		C_menuN = CLANSERVER_INSPECT;
		menuInit(C_menuN);
		break;
		case CLANSERVER_INSPECT:
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
		case UPDATE_ISCLANMEMBER:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.isClanMember(cldata.szID, cldata.gserver, cldata.ChaName);
		#ifdef CLAN_DEBUG
			DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember(아이디 : %s,게임서버 : %s,캐릭터이름 : %s)", cldata.szID, cldata.gserver, cldata.ChaName);
		#endif
			C_menuN2 = 1;
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				bIsLoading = FALSE;
				C_menuN2 = 0;
				C_menuN = CLAN_AllEnd;
				cldata.myPosition = CLANNO_REGESTER;
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember()호출하여 무등록 나옴");
			#endif

				break;


				case 1:
				WebDB.Suspend();
				bIsLoading = FALSE;

				ParsingIsClanMember(ANSdata.ret_Buf);

				cldata.myPosition = CLANUSER;
				C_menuN2 = 0;
				C_menuN = UPDATE_CLANWON;
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember()호출하여 클랜원으로 나옴");
			#endif
				break;

				case 2:
				WebDB.Suspend();
				bIsLoading = FALSE;

				ParsingIsClanMember(ANSdata.ret_Buf);
				C_menuN2 = 0;
				C_menuN = UPDATE_CLANWON;
				cldata.myPosition = CLANCHIP;
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember()호출하여 클랜원으로 나옴");
			#endif
				break;

				case 100:
				WebDB.Suspend();
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember()호출하여 리턴코드 100나옴");
			#endif
				wsprintf(szErrorMsg, "%s (67)", clanszError_Msg.szMsg);
				C_menuN = CLAN_AllEnd;
				menuInit(C_menuN);
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember()호출하여 서버에 접속할수 없음");
			#endif
				wsprintf(szErrorMsg, "%s (68)", clanszError_Msg.szMsg);
				C_menuN = CLAN_AllEnd;
				menuInit(C_menuN);

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
				#ifdef CLAN_DEBUG
					DebugMessage("UPDATE_ISCLANMEMBER에서 WebDB.isClanMember()호출하여 디폴트에러 나옴");
				#endif
					wsprintf(szErrorMsg, "%s (69)", clanszError_Msg.szMsg);
					bIsLoading = FALSE;
					C_menuN2 = 0;
					C_menuN = CLAN_AllEnd;
					cldata.myPosition = CLAN_NOCONNECT;
				}
				break;
			}
		}
		break;
		case MYCLAN_UPDATE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();


			WebDB.ReadClan(cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#ifdef CLAN_DEBUG
			DebugMessage("MYCLAN_UPDATE에서 WebDB.ReadClan(아이디 : %s,게임서버 : ,캐릭터이름 : %s)", cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#endif
			C_menuN2 = 1;
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 1:
				WebDB.Suspend();

				ParsigClanWon(ANSdata.ret_Buf);


				C_menuN2 = 0;
				C_menuN = MYCLAN_CLANMARK_UPDATE;
				menuInit(C_menuN);
				bImageDown = FALSE;

			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_UPDATE에서 리턴코드 1 정상적으로처리");
			#endif




				break;
				case 100:
				WebDB.Suspend();
				wsprintf(szErrorMsg, "%s (70)", clanszError_Msg.szMsg);

				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				if(bGetChaClan)
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_UPDATE에서 리터코드 100");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				wsprintf(szErrorMsg, "%s (71)", clanszError_Msg.szMsg);

				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				if(bGetChaClan)
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_UPDATE에서 서버에 접속 할수 없음");
			#endif
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					wsprintf(szErrorMsg, "%s (72)", clanszError_Msg.szMsg);
					C_menuN = CLAN_AllEnd;
					C_menuN2 = -1;
					CLAN_ERROR_ret = -1;
					if(bGetChaClan)
					{
						++nStatusCount;
						bGetChaClan = FALSE;
					}
				}
			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_UPDATE에서 디폴트에러");
			#endif
				break;
			}
		}
		break;
		case MYCLAN_CLANMARK_UPDATE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.MarkREAD(cldata.name, cldata.ClanMark);
		#ifdef CLAN_DEBUG
			DebugMessage("MYCLAN_CLANMARK_UPDATE에서 WebDB.MarkREAD(클랜이름 : %s,클랜마크번호 : %s)", cldata.name, cldata.ClanMark);
		#endif
			C_menuN2 = 1;
			break;
			case 1:

			switch(ANSdata.ret_val)
			{

				case 1:
				WebDB.Suspend();




				nClanMarklen = lstrlen(cldata.ClanMark);


				if(nClanMarklen == 9)
				{
					server[0] = cldata.ClanMark[0];
					strcpy(UniqueClanMark, &cldata.ClanMark[3]);
				}
				else
				{
					server[0] = cldata.ClanMark[0];
					server[1] = cldata.ClanMark[1];
					strcpy(UniqueClanMark, &cldata.ClanMark[4]);
				}


				ZeroMemory(szFile2, sizeof(szFile2));
				ZeroMemory(szFile, sizeof(szFile));
				wsprintf(szFile, "%s\\%s\\%s_%s.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%s_%s_16.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);


				if(bGetChaClan)
				{
					for(int i = 0; i < 5; i++)
					{
						if(chacldata[i].name[0] == 0)
						{
							memcpy(&chacldata[i], &cldata, sizeof(CLANDATA));


							if(bGetChaClan) ++nStatusCount;
							bGetChaClan = FALSE;
							break;
						}
					}
				}

				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;

			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_CLANMARK_UPDATE에서 클랜마크 읽기 성공");
			#endif

				break;
				case 2:
				WebDB.Suspend();
				wsprintf(szErrorMsg, "%s (73)", clanszError_Msg.szMsg);
				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				if(bGetChaClan)
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_CLANMARK_UPDATE에서 클랜마크 읽기 실패 리턴코드 2");
			#endif

				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				wsprintf(szErrorMsg, "%s (74)", clanszError_Msg.szMsg);
				C_menuN = CLAN_AllEnd;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				if(bGetChaClan)
				{
					++nStatusCount;
					bGetChaClan = FALSE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_CLANMARK_UPDATE에서 서버에 접속할수 없음");
			#endif
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					wsprintf(szErrorMsg, "%s (75)", clanszError_Msg.szMsg);

					C_menuN = CLAN_AllEnd;
					C_menuN2 = -1;
					CLAN_ERROR_ret = -1;
					if(bGetChaClan)
					{
						++nStatusCount;
						bGetChaClan = FALSE;
					}
				}
			#ifdef CLAN_DEBUG
				DebugMessage("MYCLAN_CLANMARK_UPDATE에서 디폴트 에러");
			#endif
				break;
			}
		}
		break;

		case UPDATE_CLANWON:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.ReadClan(cldata.szID, cldata.gserver, cldata.ClanZang_Name);
			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("UPDATE_CLANWON에서 WebDB.ReadClan(아이디 : %s,게임서버 : %s,캐릭터 이름 : %s);", cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 1:
				WebDB.Suspend();
				ParsigClanWon(ANSdata.ret_Buf);
				C_menuN = CLAN_AllEnd;
				CLAN_ERROR_ret = CLAN_AllEnd;
				C_menuN2 = -1;

				if(bImageDown)
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);
					bImageDown = FALSE;
					break;
				}

				nClanMarklen = lstrlen(cldata.ClanMark);

				if(nClanMarklen == 9)
				{
					server[0] = cldata.ClanMark[0];
					strcpy(UniqueClanMark, &cldata.ClanMark[3]);
				}
				else
				{
					server[0] = cldata.ClanMark[0];
					server[1] = cldata.ClanMark[1];
					strcpy(UniqueClanMark, &cldata.ClanMark[4]);
				}




				wsprintf(szFile, "%s\\%s\\%s_%s.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%s_%s_16.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);


				if((cldata.hClanMark) && (cldata.hClanMark16)) break;
				else
				{
					nClanMark = atoi(cldata.ClanMark);
					int ReadClanInfo(DWORD dwClanNum);
					ReadClanInfo(nClanMark);


				}
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_CLANWON에서 정상적으로 처리 ");
			#endif


				break;
				case 100:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;

				wsprintf(szErrorMsg, "%s (59)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				CLAN_ERROR_ret = CLAN_AllEnd;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_CLANWON에서 리턴코드 100 ");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (60)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
			#ifdef CLAN_DEBUG
				DebugMessage("UPDATE_CLANWON에서 서버에 접속 할수 없음 ");
			#endif
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_AllEnd;
					menuInit(C_menuN);

					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("UPDATE_CLANWON에서 디폴트에러");
				#endif
				}

				break;
			}
		}
		break;


		case CLAN_AllEnd:

		OpenFlag = 0;
		C_menuN = 0;
		C_menuN2 = 0;

		ClanCharMove = 0;
		ClanCLCharMove = 0;


	#ifdef PARKMENU
		g_nMicBtn = 0;
	#endif

		void TJBscrollWheelClear();
		TJBscrollWheelClear();
		isDrawClanMenu = 0;
		break;



		case CLAN_READ_CLANMARK:


		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.MarkREAD(cldata.name, cldata.ClanMark);
			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_READ_CLANMARK에서 WebDB.MarkREAD(클랜이름 : %s,클랜마크 번호 : %s);");
		#endif
			break;
			case 1:

			switch(ANSdata.ret_val)
			{

				case 1:
				WebDB.Suspend();




				nClanMarklen = lstrlen(cldata.ClanMark);

				if(nClanMarklen == 9)
				{
					server[0] = cldata.ClanMark[0];
					strcpy(UniqueClanMark, &cldata.ClanMark[3]);
				}
				else
				{
					server[0] = cldata.ClanMark[0];
					server[1] = cldata.ClanMark[1];
					strcpy(UniqueClanMark, &cldata.ClanMark[4]);
				}



				wsprintf(szFile, "%s\\%s\\%s_%s.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%s_%s_16.%s", SAVE_DIR, szConnServerName, server, UniqueClanMark, SAVE_EXE);

				if((cldata.myPosition == CLANUSER) || (cldata.myPosition == CLAN_SUBCHIP))
				{
					C_menuN = CLAN_USER_ReadDATA;
					C_menuN2 = 0;

				}
				if(cldata.myPosition == CLANCHIP)
				{
					C_menuN = CLAN_CHIP_ReadDATA;
					menuInit(C_menuN);
					C_menuN2 = 0;
				}

			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_READ_CLANMARK에서 클랜마크 읽기 성공");
			#endif


				break;
				case 2:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (48)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);


				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_READ_CLANMARK에서 클랜마크 읽기 실패 리턴코드 2");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (49)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);


				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_READ_CLANMARK에서 서버에 접속할수 없음");
			#endif
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();

					C_menuN = CLAN_AllEnd;
					menuInit(C_menuN);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLAN_READ_CLANMARK에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;

		case CLAN_REAMMAKR_ERROR:

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



		case CLAN_GO_NPCSTART:
		C_menuN2 = 0;
		C_menuN = CLAN_NPC_START;
		menuInit(C_menuN);
		break;

		case CLAN_NPC_START:

		switch(C_menuN2)
		{
			case 0:


		#ifdef  USE_PROSTONTALE
			cldata.money = sinChar->Money;


		#else

		#endif

			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.isClanMember(cldata.szID, cldata.gserver, cldata.ChaName);
			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_NPC_START에서 WebDB.isClanMember(아이디 : %s, 게임서버 : %s, 캐릭터이름 : %s)", cldata.szID, cldata.gserver, cldata.ChaName);
		#endif
			break;

			case 1:
			if(ANSdata.ret_val != -1)
			{
				ANSdata.ret_val = ANSdata.ret_val;
			}


			switch(ANSdata.ret_val)
			{
				case 0:

				g_ClanMakeMoney = 0;
				WebDB.Suspend();

				ZeroMemory(szChatClanWon, sizeof(szChatClanWon));
				ZeroMemory(cldata.clanWon, sizeof(cldata.clanWon));
				nClanWon_Num = 0;
				ParsingMakeClanMoney(ANSdata.ret_Buf, &g_ClanMakeMoney);




			#ifdef USE_PROSTONTALE		
				if(sinChar->Level < CLAN_MAKELEVEL)
				{
					C_menuN = CLAN_NOLEVEL;
					menuInit(C_menuN);
				}
				else
				{
					C_menuN = CLAN_GO_MAKE;
				}
			#else
				if(50 < CLAN_MAKELEVEL)
				{
					C_menuN = CLAN_NOLEVEL;
					menuInit(C_menuN);
				}
				else
				{
					C_menuN = CLAN_GO_MAKE;
				}
			#endif


				C_menuN2 = 1;
				cldata.myPosition = CLANNO_REGESTER;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 무등록자");
			#endif
				break;

				case 1:

				WebDB.Suspend();


				ParsingIsClanMember(ANSdata.ret_Buf);
				WriteMyClanInfo();

				if(bImageDown)
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);
					bImageDown = FALSE;
					break;
				}


				cldata.myPosition = CLANUSER;










				wsprintf(szFile, "%s\\%s\\%s_%s.%s", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%s_%s_16.%s", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum, SAVE_EXE);



				fp = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				fp2 = CreateFile(szFile2, GENERIC_READ, FILE_SHARE_READ,
								 NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


				if((fp != NULL) && (fp2 != NULL))
				{
				
					strcpy(cldata.szClanMarkName, szFile);
					C_menuN = CLAN_USER_FLAG;

				}
				else
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);
				}






				if(fp != NULL) CloseHandle(fp);
				if(fp2 != NULL) CloseHandle(fp2);

			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 클랜원");
			#endif
				break;

				case 2:
				WebDB.Suspend();

				C_menuN2 = 0;
				ParsingIsClanMember(ANSdata.ret_Buf);
				cldata.isSubChip[0] = '0';

				WriteMyClanInfo();


				cldata.myPosition = CLANCHIP;

				if(bImageDown)
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);
					bImageDown = FALSE;
					break;
				}

				wsprintf(szFile, "%s\\%s\\%c_%s.%s", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%c_%s_16.%s", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum, SAVE_EXE);



				fp = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				fp2 = CreateFile(szFile2, GENERIC_READ, FILE_SHARE_READ,
								 NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


				if((fp != NULL) && (fp2 != NULL))
				{
					C_menuN2 = 0;
					C_menuN = CLAN_CHIP_ReadDATA;
					menuInit(C_menuN);

				}
				else
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);

				}

				if(fp != NULL) CloseHandle(fp);
				if(fp2 != NULL) CloseHandle(fp2);



			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 클랜장");
			#endif
				break;
				case 3:
				WebDB.Suspend();
				ParsingIsClanMember2(ANSdata.ret_Buf);
				C_menuN = CLANCHIP_GO_RELEASEMESSAGE;
				C_menuN2 = -1;
				cldata.myPosition = CLANNO_REGESTER;

			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 클랜장이 클랜을 해체했을경우 리턴코드 3");
			#endif
				break;
				case 4:
				WebDB.Suspend();
				ParsingIsClanMember2(ANSdata.ret_Buf);
				C_menuN = CLANUSER_CHECKPFLAG;
				C_menuN2 = -1;
				cldata.myPosition = CLANNO_REGESTER;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 클랜이 해체된 클랜원");
			#endif
				break;
				case 5:

				WebDB.Suspend();


				ParsingIsClanMember(ANSdata.ret_Buf);
				cldata.isSubChip[0] = '1';
				WriteMyClanInfo();

				if(bImageDown)
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);
					bImageDown = FALSE;
					break;
				}


				cldata.myPosition = CLAN_SUBCHIP;










				wsprintf(szFile, "%s\\%s\\%c_%s.%s", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum, SAVE_EXE);
				wsprintf(szFile2, "%s\\%s\\%c_%s_16.%s", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum, SAVE_EXE);




				fp = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ,
								NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

				fp2 = CreateFile(szFile2, GENERIC_READ, FILE_SHARE_READ,
								 NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);


				if((fp != NULL) && (fp2 != NULL))
				{					
					strcpy(cldata.szClanMarkName, szFile);
					C_menuN = CLAN_USER_FLAG;

				}
				else
				{
					C_menuN2 = 0;
					C_menuN = CLAN_READ_CLANMARK;
					menuInit(C_menuN);
				}




				if(fp != NULL) CloseHandle(fp);
				if(fp2 != NULL) CloseHandle(fp2);

			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 클랜원");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (57)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				cldata.myPosition = CLAN_NOCONNECT;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_NPC_START에서 클랜서버에 접속할수 없음");
			#endif
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();

					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (58)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);


					cldata.myPosition = CLAN_NOCONNECT;
					C_menuN2 = 0;
				#ifdef CLAN_DEBUG
					DebugMessage("CLAN_NPC_START에서 디폴트에러");
				#endif
				}
				break;
			}
			break;
		}
		break;


	}

}



int CLANEntity::chkMenu(int x, int y, RECT *r)
{
	if(r->left <= x   &&   x <= r->right)
		if(r->top <= y   &&   y <= r->bottom)
			return 1;
	return 0;
}
int CLANEntity::chkeckMenuN()
{
	return 1;
}







void CLANEntity::RenderMain()
{


}















void CLANEntity::Load2()
{
}

void CLANEntity::Load() {}





void CLANEntity::Close() {}





void CLANEntity::copy_LoadResource2(CLANEntity *copyDest)
{
#ifdef USE_PROSTONTALE
	copyDest->hScl_Bar_tj = this->hScl_Bar_tj;
	copyDest->hScl_icon_tj = this->hScl_icon_tj;
	copyDest->hClanNameInput = this->hClanNameInput;
#endif
	copyDest->hLine = this->hLine;

}


void CLANEntity::copy_LoadResource(CLANEntity* copyDest) {}







int CLANEntity::chk_readStrBuf(char *cmd, char *readbuf, char *retBuf)
{

	char *pBuf;

	pBuf = strstr(readbuf, cmd);
	if(pBuf == NULL)
	{
		return -1;
	}

	pBuf = pBuf + strlen(cmd);
	while(1)
	{
		if(*pBuf == '\n') return 0;
		if(*pBuf != ' ') break;
		pBuf++;
	}
	sscanf(pBuf, "%s", retBuf);

	return 1;
}

void CLANEntity::ParsingData(char* cmd, char* data, char* save)
{
	char *pBuf;
	int nCount = 0;
	char szTemp[500];
	ZeroMemory(szTemp, sizeof(char) * 500);
	pBuf = strstr(data, cmd);
	if(pBuf == NULL)
	{
		return;
	}

	pBuf = pBuf + strlen(cmd);
	strcpy(szTemp, pBuf);
	if(pBuf == NULL)
	{
		return;
	}


	while(1)
	{

		if(szTemp[nCount] == '\r') break;
		save[nCount] = szTemp[nCount];
		nCount++;

	}

}






int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf)
{
	int iRejectCode = -1;
	char *pBuf;

	pBuf = strstr(readbuf, cmd);
	if(pBuf == NULL)
	{
		return iRejectCode;
	}

	pBuf = pBuf + strlen(cmd);
	while(1)
	{
		if(*pBuf == '\n') return 0;
		if(*pBuf != ' ') break;
		pBuf++;
	}
	sscanf(pBuf, "%s", retBuf);

	return iRejectCode;
}

void ParsingData(char* cmd, char* data, char* save)
{
	char *pBuf;
	int nCount = 0;
	char szTemp[500];
	ZeroMemory(szTemp, sizeof(char) * 500);
	pBuf = strstr(data, cmd);
	pBuf = pBuf + strlen(cmd);
	strcpy(szTemp, pBuf);
	if(pBuf == NULL)
	{
		return;
	}


	while(1)
	{
		if(szTemp[nCount] == '\r') break;
		save[nCount] = szTemp[nCount];
		nCount++;

		save[nCount] = 0;

		if(nCount >= 79)
		{
			break;
		}

	}
}



#if 1


void ParsingIsClanMember(char* data, int myposition)
{
	char szClanWonNum[64];
	switch(myposition)
	{
		case 4:
		case 3:
		case 0:
		cldata.myPosition = CLANNO_REGESTER;
		ZeroMemory(cldata.name, sizeof(cldata.name));
		ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark));
		ZeroMemory(szChatClanWon, sizeof(szChatClanWon));




		cldata.intClanMark = 0;
		break;
		case 5:
		case 1:
		case 2:
		if(myposition == 1)   cldata.myPosition = CLANUSER;
		else if(myposition == 5)
		{
			cldata.myPosition = CLAN_SUBCHIP;
			cldata.isSubChip[0] = '1';
		}
		else
		{
			cldata.myPosition = CLANCHIP;
			cldata.isSubChip[0] = '0';
		}

		chk_readStrBuf("CName=", data, cldata.name);

		chk_readStrBuf("CNote=", data, cldata.explanation);
		cBaseMenu.CheckClanExp(cldata.explanation);

		ZeroMemory(cldata.SubChip, sizeof(cldata.SubChip));
		chk_readStrBuf("CSubChip=", data, cldata.SubChip);

		chk_readStrBuf("CMCnt=", data, szClanWonNum);
		cldata.nCurrentClanWon_Num = atoi(szClanWonNum);



		chk_readStrBuf("CZang=", data, cldata.ClanZang_Name);
		chk_readStrBuf("CStats=", data, cldata.stats);
		chk_readStrBuf("CSec=", data, cldata.sec);


		chk_readStrBuf("CRegiD=", data, cldata.szRegiD);
		chk_readStrBuf("CLimitD=", data, cldata.limitD);
		chk_readStrBuf("CDelActive=", data, cldata.DelActive);

		chk_readStrBuf("CIMG=", data, cldata.ClanMark);

		cldata.intClanMark = atoi(cldata.ClanMark);

		chk_readStrBuf("CPFlag=", data, cldata.PFlag);


		{


			cldata.CNFlag = 0;
			char szCNFlag[64];
			if(1 == chk_readStrBuf("CNFlag=", data, szCNFlag))
			{
				cldata.CNFlag = atoi(szCNFlag);
			}


		}



		break;
	}
}
#endif


void CLANEntity::ParsingMakeClanMoney(char* data, int* money)
{
	char szMoney[100];
	ZeroMemory(szMoney, sizeof(szMoney));
	chk_readStrBuf("CMoney=", data, szMoney);
	*money = atoi(szMoney);
}

void CLANEntity::ParsingIsClanMember(char* data)
{

	int nBefore_mark;
	int nAfter_mark;
	char szclanmark[64];
	char szClanWonNum[64];
	int nSecond;
	int nOneDay = 86400;
	float fLastday;


	chk_readStrBuf("CName=", data, cldata.name);
	ParsingData("CNote=", data, cldata.explanation);
	CheckClanExp(cldata.explanation);

	chk_readStrBuf("CZang=", data, cldata.ClanZang_Name);
	chk_readStrBuf("CStats=", data, cldata.stats);
	chk_readStrBuf("CSec=", data, cldata.sec);
	chk_readStrBuf("CMCnt=", data, szClanWonNum);
	cldata.nCurrentClanWon_Num = atoi(szClanWonNum);



	chk_readStrBuf("CRegiD=", data, cldata.szRegiD);
	chk_readStrBuf("CLimitD=", data, cldata.limitD);
	chk_readStrBuf("CDelActive=", data, cldata.DelActive);
	ZeroMemory(cldata.SubChip, sizeof(cldata.SubChip));
	chk_readStrBuf("CSubChip=", data, cldata.SubChip);


	chk_readStrBuf("CIMG=", data, szclanmark);
	nBefore_mark = atoi(cldata.ClanMark);
	nAfter_mark = atoi(szclanmark);
	if(nAfter_mark != nBefore_mark) bImageDown = TRUE;
	strcpy(cldata.ClanMark, szclanmark);
	cldata.intClanMark = atoi(cldata.ClanMark);
	chk_readStrBuf("CPFlag=", data, cldata.PFlag);
	chk_readStrBuf("CKFlag=", data, cldata.LeaveClanFlag);
	cldata.nKFlag = atoi(cldata.LeaveClanFlag);
	chk_readStrBuf("COldChipName=", data, cldata.OldChipChaName);
	chk_readStrBuf("COldSubChipName=", data, cldata.OldSubChip);


	cldata.CNFlag = 0;
	char szCNFlag[64];
	if(1 == chk_readStrBuf("CNFlag=", data, szCNFlag))
	{
		cldata.CNFlag = atoi(szCNFlag);
	}



	nSecond = atoi(cldata.sec);
	fLastday = (float)(nSecond / (float)nOneDay);
	cldata.nLastDay = 10 - (int)floor(fLastday);


	int n9day = 777600;







	int lastday;

	if(nSecond > n9day)
	{

		lastday = nSecond - n9day;
		lastday = nOneDay - lastday;
		cldata.bIsOneDay = TRUE;

		cldata.nHour = lastday / 3600;
		lastday = (lastday - (cldata.nHour * 3600));
		cldata.nMinute = lastday / 60;



	}
	else
	{
		cldata.bIsOneDay = FALSE;
	}

}

void CLANEntity::ParsingIsClanMember2(char *data)
{
	ZeroMemory(cldata.name, sizeof(cldata.name));

	chk_readStrBuf("CName=", data, cldata.name);
	chk_readStrBuf("CPFlag=", data, cldata.PFlag);



	cldata.CNFlag = 0;
	char szCNFlag[64];
	if(1 == chk_readStrBuf("CNFlag=", data, szCNFlag))
	{
		cldata.CNFlag = atoi(szCNFlag);
	}

}

void CLANEntity::WriteMyClanInfo()
{
	char szTemp[100];
	HANDLE hFile;
	DWORD dwWrite;
	int nClanMarklen = 0;

	strcpy(MyClanInfo.ClanMark_Num, cldata.ClanMark);
	strcpy(MyClanInfo.ClanName, cldata.name);
	strcpy(MyClanInfo.ClanReport, cldata.explanation);
	MyClanInfo.nClanMark = atoi(MyClanInfo.ClanMark_Num);






	nClanMarklen = lstrlen(cldata.ClanMark);

	if(nClanMarklen == 9)
	{
		MyClanInfo.Gserver[0] = cldata.ClanMark[0];
		strcpy(MyClanInfo.UniqueClaClanInfo_nNum, &cldata.ClanMark[3]);
	}
	else
	{
		MyClanInfo.Gserver[0] = cldata.ClanMark[0];
		MyClanInfo.Gserver[1] = cldata.ClanMark[1];
		strcpy(MyClanInfo.UniqueClaClanInfo_nNum, &cldata.ClanMark[4]);
	}

	wsprintf(szTemp, "%s\\%s\\%s_%s.rep", SAVE_DIR, szConnServerName, MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum);
	hFile = CreateFile(szTemp, GENERIC_WRITE, FILE_SHARE_WRITE,
					   NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		WriteFile(hFile, &MyClanInfo, sizeof(MYCLANINFOHEADER), &dwWrite, NULL);
		CloseHandle(hFile);
	}
}



void ParsigClanWon(char* data)
{
	cBaseMenu.ParsigClanWon(data);
}


void CLANEntity::ParsigClanWon(char* data)
{
	int i = 0, j = 0;
	int datastrlen;
	char szTemp[2048];
	char szClanWon[30];
	char *pBuf;
	int nPos, nClanWonlen;
	int k = 0;
	ZeroMemory(szTemp, sizeof(szTemp));
	memcpy(szTemp, data, strlen(data));
	datastrlen = lstrlen(data);
	pBuf = strstr(szTemp, "CMem");
	nPos = pBuf - szTemp;
	nClanWonlen = datastrlen - nPos;
	k = nPos + 5;




	nClanWon_Num = 0;



	ZeroMemory(szClanWon, sizeof(char) * 30);
	ZeroMemory(cldata.clanWon, sizeof(cldata.clanWon));


	while(k < datastrlen)
	{
		if(szTemp[k] == 0) break;
		if(szTemp[k] == '\r')
		{
			k += 6;
			i += 1;
			j = 0;
			nClanWon_Num += 1;
		}
		else
		{


			cldata.clanWon[i][j] = szTemp[k];
			k++;
			j++;
		}
	}


	int ZangL = strlen(cldata.ChaName);
	for(i = 0; i < nClanWon_Num; i++)
	{

		int nameL = strlen(cldata.clanWon[i]);
		if(ZangL == nameL &&
		   strcmp(cldata.clanWon[i], cldata.ChaName) == 0)
		{
			for(j = i; j < (nClanWon_Num); j++)
			{
				memcpy(&cldata.clanWon[j], &cldata.clanWon[j + 1], sizeof(char) * 30);
			}

			nClanWon_Num--;
			break;
		}
	}
	cldata.nTotalClanWonCount = nClanWon_Num;
	ParsingClanWonStrLen();
}



void CLANEntity::ParsingClanWonStrLen()
{
	ZeroMemory(cldata.clanWonStrLen, sizeof(int) * 41);
	int i, j;
	for(i = 0; i < nClanWon_Num; i++)
	{
		for(j = 0; j < 30; j++)
		{
			if(cldata.clanWon[i][j] == 0) break;
			if((((cldata.clanWon[i][j] & 0xffffff) >= 0x65) && ((cldata.clanWon[i][j] & 0xffffff) <= 0x90)) || (((cldata.clanWon[i][j] & 0xffffff) >= 0x97) && ((cldata.clanWon[i][j] & 0xffffff) <= 0x122)))
			{
				cldata.clanWonStrLen[i] += 5;
			}
			else if(((cldata.clanWon[i][j] & 0xffffff) >= 0x255))
			{
				cldata.clanWonStrLen[i] += 8;
			}
			else cldata.clanWonStrLen[i] += 4;


		}

	}


	cldata.nTotalClanWonCount = nClanWon_Num;
}


BOOL CLANEntity::ReadClanHelpScr(char *filename)
{
	HANDLE hFile = NULL;
	DWORD dwRead;
	BOOL  bReturn;
	BOOL bFileEnd = FALSE;
	char szTemp[1];
	int nCount = 0;
	g_nClanIconNum = 0;




	ZeroMemory(ClanHelpIcon, sizeof(CLANHELPICON)*CLANHELPICON_NUM);

	hFile = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) return FALSE;
	while(1)
	{
		while(1)
		{
			memset(szTemp, 0x00, sizeof(char));
			bReturn = ReadFile(hFile, szTemp, 1, &dwRead, NULL);
			if(dwRead == 0)
			{
				bFileEnd = TRUE;

				break;
			}
			if((bReturn != FALSE) && (dwRead == 0))
			{
				bFileEnd = TRUE;
				break;
			}
			if((szTemp[0] == 0x20))
			{
				break;
			}
			if((szTemp[0] == 0x0D) || (szTemp[0] == 0x0A))
			{
				continue;
			}
			else
			{
				ClanHelpIcon[g_nClanIconNum].ImagePath[nCount] = szTemp[0];
				nCount++;
			}

		}
		if(bFileEnd) break;
		nCount = 0;
		memset(szTemp, 0x00, sizeof(char));
		while(1)
		{
			bReturn = ReadFile(hFile, szTemp, 1, &dwRead, NULL);
			if(dwRead == 0)
			{
				bFileEnd = TRUE;

				break;
			}
			if((bReturn != FALSE) && (dwRead == 0))
			{
				bFileEnd = TRUE;
				break;
			}
			if(szTemp[0] == 0x20)
			{
				break;
			}
			if((szTemp[0] == 0x0D) || (szTemp[0] == 0x0A))
			{
				break;
			}
			else
			{
				ClanHelpIcon[g_nClanIconNum].ImagePath2[nCount] = szTemp[0];
			}
			nCount++;
		}
		if(bFileEnd) break;
		nCount = 0;
		memset(szTemp, 0x00, sizeof(char));
		while(1)
		{
			bReturn = ReadFile(hFile, szTemp, 1, &dwRead, NULL);
			if(dwRead == 0)
			{
				bFileEnd = TRUE;

				break;
			}
			if((bReturn != FALSE) && (dwRead == 0))
			{
				bFileEnd = TRUE;
				break;
			}
			if(szTemp[0] == 0x20)
			{
				break;
			}
			if((szTemp[0] == 0x0D) || (szTemp[0] == 0x0A))
			{
				break;
			}
			else
			{
				ClanHelpIcon[g_nClanIconNum].ImagePath3[nCount] = szTemp[0];
			}
			nCount++;
		}


		if(bFileEnd) break;
		nCount = 0;
		memset(szTemp, 0x00, sizeof(char));
		while(1)
		{
			bReturn = ReadFile(hFile, szTemp, 1, &dwRead, NULL);
			if(dwRead == 0)
			{
				bFileEnd = TRUE;

				break;
			}
			if((bReturn != FALSE) && (dwRead == 0))
			{
				bFileEnd = TRUE;
				break;
			}
			if(szTemp[0] == 0x20)
			{
				continue;
			}
			if((szTemp[0] == 0x0D) || (szTemp[0] == 0x0A))
			{
				break;
			}
			else
			{
				ClanHelpIcon[g_nClanIconNum].TextFilePath[nCount] = szTemp[0];
			}
			nCount++;
		}
		if(bFileEnd) break;
		g_nClanIconNum++;
		nCount = 0;
	}



	CloseHandle(hFile);
	return TRUE;
}

void CLANEntity::LoadHelpImage() {}

void CLANEntity::DestroyClanHelpIcon() {}


void CLANEntity::LoadHelp(char *help)
{
	g_HelpDisPlay = 0;

	HANDLE hFile = NULL;
	DWORD dwRead;
	BOOL  bReturn;
	BOOL bFileEnd = FALSE;
	BOOL bFlag = FALSE;
	int nCount = 0;
	char szTemp[1];
	ZeroMemory(HelpContents, sizeof(HELPCONTENTS) * 100);
	hFile = CreateFile(help, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE) return;
	while(1)
	{
		memset(szTemp, 0x00, sizeof(char));
		bReturn = ReadFile(hFile, szTemp, 1, &dwRead, NULL);
		if(dwRead == 0)
		{
			bFileEnd = TRUE;

			break;
		}
		if((bReturn != FALSE) && (dwRead == 0))
		{
			bFileEnd = TRUE;
			break;
		}
		if(szTemp[0] == 0x0D) continue;
		if((szTemp[0] != 0x0A) && (szTemp[0] != 0x0D)) bFlag = FALSE;
		if((szTemp[0] == 0x0A))
		{
			if(!bFlag)
			{
				g_HelpDisPlay++;
				nCount = 0;
			}
			bFlag = TRUE;

		}
		else
		{
			HelpContents[g_HelpDisPlay].szHelpContent[nCount] = szTemp[0];


			nCount++;

			if(nCount >= 60)
				nCount = nCount;
		}
	}

	CloseHandle(hFile);


}


void CLANEntity::ReMoveClanWon(char *src)
{
	int i = 0, j = 0;
	int datastrlen;
	char szTemp[1024];
	char szClanWon[30];
	char *pBuf;
	int nPos, nClanWonlen;
	int k = 0;
	ZeroMemory(szTemp, sizeof(char) * 1024);
	memcpy(szTemp, src, strlen(src));
	datastrlen = lstrlen(src);
	pBuf = strstr(szTemp, "=");
	nPos = pBuf - szTemp;
	nClanWonlen = datastrlen - nPos;
	k = nPos + 1;


	ZeroMemory(szClanWon, sizeof(char) * 30);
	ZeroMemory(szRemoveClanWonResult, sizeof(szRemoveClanWonResult));

	while(k < datastrlen)
	{
		if(szTemp[k] == 0) break;
		if(szTemp[k] == '&')
		{
			k += 8;
			i += 1;
			j = 0;

		}
		else
		{



			szRemoveClanWonResult[i][j] = szTemp[k];
			k++;
			j++;
		}
	}
}

void CLANEntity::Draw() {}


void CLANEntity::menuInit(int num) {}









void CLANEntity::Draw_C_Text(char* str, int x, int y, int sw) {}


void CLANEntity::Draw_C_Text2(char* str, int x, int y, int sw) {}


void CLANEntity::Draw_C_Text3(char* str, int x, int y, int sw) {}


void LoadToFile()
{
	char szTemp[64];
	wsprintf(szTemp, "%c_%s.rep", MyClanInfo.Gserver, MyClanInfo.UniqueClaClanInfo_nNum);
	FILE *fp;
	fp = fopen(szTemp, "rb");
	fread(&MyClanInfo, sizeof(MYCLANINFOHEADER), 1, fp);
	fclose(fp);
}

int CLANEntity::CheckMark(BOOL bFlag)
{
	FILE *fp;
	char szTemp[256];
	char GServer;
	char szNum[64];
	char szFile[256];
	char szFile2[256];
	MYCLANINFOHEADER myclaninfo;
	GServer = cldata.ClanMark[0];
	strcpy(szNum, &cldata.ClanMark[3]);

	wsprintf(szTemp, "%s\\%s\\%c_%s.rep", CLANINFO_DIR, szConnServerName, GServer, szNum);
	fp = fopen(szTemp, "rb");
	if(fp > 0)
	{
		fread(&myclaninfo, sizeof(MYCLANINFOHEADER), 1, fp);
		fclose(fp);

	}
	else
	{
		return -1;
	}

	int nClanMark = atoi(cldata.ClanMark);
	int nClanMark2 = atoi(myclaninfo.ClanMark_Num);

	if(nClanMark != nClanMark2) return 0;
	else
	{
		if(bFlag)
		{
			wsprintf(szFile, "%s\\%s\\%c_%s.%s", SAVE_DIR, szConnServerName, GServer, szNum, SAVE_EXE);
			wsprintf(szFile2, "%s\\%s\\%c_%s_16.%s", SAVE_DIR, szConnServerName, GServer, szNum, SAVE_EXE);
		#ifdef  USE_PROSTONTALE						

			for(int i = 0; i < 5; i++)
			{
				if(chacldata[i].name[0] == 0)
				{
					memcpy(&chacldata[i], &cldata, sizeof(CLANDATA));


					if((chacldata[i].hClanMark16 == 0) || (chacldata[i].hClanMark == 0))
						return 0;

					++nStatusCount;
					bFlag = FALSE;
					break;
				}
			}



		#else
			wsprintf(szFile, "%s\\%s\\%c_%s.%s", SAVE_DIR, szConnServerName, GServer, szNum, SAVE_EXE);
			wsprintf(szFile2, "%s\\%s\\%c_%s_16.%s", SAVE_DIR, szConnServerName, GServer, szNum, SAVE_EXE);

			if(cldata.hClanMark16)
			{

				DeleteObject(cldata.hClanMark16);

			}

			if(cldata.hClanMark)
			{
				DeleteObject(cldata.hClanMark);
			}

			cldata.hClanMark16 = (HBITMAP)LoadImage(NULL, szFile2,
													IMAGE_BITMAP,
													0, 0,
													LR_LOADFROMFILE);
			cldata.hClanMark = (HBITMAP)LoadImage(NULL, szFile,
												  IMAGE_BITMAP,
												  0, 0,
												  LR_LOADFROMFILE);

			for(int i = 0; i < 5; i++)
			{
				if(chacldata[i].name[0] == 0)
				{
					memcpy(&chacldata[i], &cldata, sizeof(CLANDATA));
					++nStatusCount;
					bFlag = FALSE;
					break;
				}
			}

		#endif

		}
	}


	return 1;

}

#define CLAN_EXP_MAX 80
void CLANEntity::CheckClanExp(char* byte)
{
	int nStrLen;
	int nCount = 0;
	BOOL bReturn;


	nStrLen = lstrlen(byte);

	if(nStrLen < CLAN_EXP_MAX)
	{
		return;
	}

	while(nCount < nStrLen)
	{
		bReturn = IsDBCSLeadByte(byte[nCount]);



		if(bReturn)
		{
			if(nCount >= CLAN_EXP_MAX - 2)
			{
				byte[nCount] = 0;
				break;
			}
			nCount += 2;
		}
		else
		{
			if(nCount >= CLAN_EXP_MAX - 2)
			{
				byte[nCount + 1] = 0;
				break;
			}

			nCount += 1;
		}
	}
	byte[CLAN_EXP_MAX - 1] = 0;

}




void CLANEntity::Draw_C_TextPark(char* str, int x, int y, int RedColor, int GreenColor, int BlueColor, int sw) {}
