#define CE_CHIP_CPP
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>

#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		
#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"

#endif

#include "tjclan.h"
#include "cE_chip.h"
#include "tjscroll.h"
#include "cE_Report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "..\\ygy\\ygyheader.h"


CLANEntity_chip::CLANEntity_chip()
{
	Money = 0;
	bClan6Down = FALSE;
	ZeroMemory(szTemp, sizeof(szTemp));
	bRemoveSecondCheck = FALSE;
}

CLANEntity_chip::~CLANEntity_chip()
{

}



int CLANEntity_chip::Init(int Life)
{
	life = Life;
	bPFlag = FALSE;
	bRelease = FALSE;
	bSubChip = FALSE;
	bisSubChip = FALSE;
	return 1;
}

int CLANEntity_chip::Clear()
{
	return 1;
}



void CLANEntity_chip::Main()
{
	int i;
	char szClanMark[32];
	if(life == 0) return;

	switch(C_menuN)
	{
	#ifdef __CLANSUBCHIP__

		case CLANCHIP_NOSUBCHIP:
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
				C_menuN = CLAN_CHIP;

				menuInit(C_menuN);

				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}

		break;
		case CLANCHIP_SUBCHIP_LOAD_DATA:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.ReadClan(cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA에서 WebDB.ReadClan(아이디 : %s,게임서버 : %s,캐릭터이름 : %s)", cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#endif
			C_menuN2 = 1;
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (101)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				CLAN_ERROR_ret = 255;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 리턴코드 0 가입한 클랜이 없음");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				ParsigClanWon(ANSdata.ret_Buf);
				C_menuN = CLANCHIP_SUBCHIP_LIST;
				menuInit(C_menuN);
				break;
				case 100:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (102)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 리턴코드 100");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (103)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 서버에 접속할수 없음");
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
					DebugMessage("CLAN_CHIP_ReadDATA에서 디폴트에러");
				#endif
				}
				break;

			}
			break;
		}
		break;


		case CLANCHIP_SUBCHIP_READMSG_FLAG:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			if(cldata.nKFlag == 4)
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if(cldata.nKFlag == 16)
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if(cldata.nKFlag == 5)
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 7) && (cldata.nKFlag_Count))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 17) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if((cldata.nKFlag == 19) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if((cldata.nKFlag == 20) && (cldata.nKFlag_Count == 1) && (bSubChip == TRUE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if((cldata.nKFlag == 20) && (cldata.nKFlag_Count == 2) && (bSubChip == TRUE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 20) && (cldata.nKFlag_Count == 1) && (bSubChip == FALSE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 20) && (cldata.nKFlag_Count == 2) && (bSubChip == FALSE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if(cldata.nKFlag == 52)
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1) && (bSubChip == TRUE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 2) && (bSubChip == TRUE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1) && (bSubChip == FALSE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 2) && (bSubChip == FALSE))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if((cldata.nKFlag == 23) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if((cldata.nKFlag == 23) && (cldata.nKFlag_Count == 2))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else if(cldata.nKFlag == 28)
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "32", "0");
			}
			else if(cldata.nKFlag == 55)
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "8", "0");
			}
			else
			{
			}



			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 WebDB.isPFlag(아이디 : %s,클랜이름 : %s,캐릭터이름 : %s,게임서버 : %s,플래그 : %s,0)", cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, cldata.PFlag);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (86)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 삽입실패");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;
				if((cldata.nKFlag == 20) && (cldata.nKFlag_Count == 1))
				{
					cldata.nKFlag_Count = 2;
					if(bSubChip) C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					else C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1))
				{
					cldata.nKFlag_Count = 2;
					if(bSubChip == TRUE) C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					else	C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if((cldata.nKFlag == 23) && (cldata.nKFlag_Count == 1))
				{
					cldata.nKFlag_Count = 2;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else
				{
				}
				if((cldata.nKFlag == 4) ||
					(cldata.nKFlag == 16) ||
				   (cldata.nKFlag == 17) ||
				   (cldata.nKFlag == 19) ||
				   (cldata.nKFlag == 5) ||
				   (cldata.nKFlag == 7) ||
				   (cldata.nKFlag == 52) ||
				   ((cldata.nKFlag == 23) && (cldata.nKFlag_Count == 2)) ||
				   ((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 2)) ||
				   (cldata.nKFlag == 55))
				{
					cldata.nKFlag_Count = 0;
				}


				if((strcmp(cldata.stats, "0") == 0) && (strcmp(cldata.PFlag, "0") == 0))
				{
					wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);
					strcpy(szTemp, clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg, szDrawMenuMsg);
					CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
					strcpy(clanMsg6.szMsg, szTemp);

					if(cldata.bIsOneDay)
					{

						wsprintf(szDrawMenuMsg, clanMsg7.szMsg, cldata.nHour, cldata.nMinute);
						strcpy(szTemp, clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
						strcpy(clanMsg6.szMsg, szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);

						strcpy(szTemp, clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);

						strcpy(clanMsg6.szMsg, szTemp);
					}

					C_menuN = CLAN_NOMAKE;
					menuInit(C_menuN);
					bPFlag = FALSE;
					C_menuN2 = -1;
					break;
				}
				else if((strcmp(cldata.stats, "1") == 0) && (strcmp(cldata.PFlag, "0") == 0))
				{
					strcpy(cldata.PFlag, "2");
					strcpy(szTemp, clanMakeFinishClan.szMsg);
					wsprintf(szDrawMenuMsg, clanMakeFinishClan.szMsg, cldata.name);
					strcpy(clanMakeFinishClan.szMsg, szDrawMenuMsg);

					CheckMsgPos(&clanMakeFinishClan, BOX_MAX_WIDTH, 0);
					C_menuN = CLANCHIP_MAKE_FIRSTMESSAGE;
					menuInit(C_menuN);
					strcpy(clanMakeFinishClan.szMsg, szTemp);
					C_menuN2 = -1;
					bPFlag = TRUE;
					break;
				}








				else if((strcmp(cldata.stats, "1") == 0) && (strcmp(cldata.PFlag, "2") == 0) && (nClanWon_Num < 5))
				{

					wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);
					strcpy(szTemp, clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg, szDrawMenuMsg);
					CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
					strcpy(clanMsg6.szMsg, szTemp);

					if(cldata.bIsOneDay)
					{

						wsprintf(szDrawMenuMsg, clanMsg7.szMsg, cldata.nHour, cldata.nMinute);
						strcpy(szTemp, clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
						strcpy(clanMsg6.szMsg, szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);

						strcpy(szTemp, clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);

						strcpy(clanMsg6.szMsg, szTemp);
					}

					C_menuN = CLAN_NOMAKE;
					menuInit(C_menuN);
					C_menuN2 = -1;
					bPFlag = FALSE;
				}














				else
				{
					C_menuN = CLAN_CHIP;
					menuInit(C_menuN);
					C_menuN2 = -1;
					bPFlag = TRUE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGE에서 리턴코드 1 성공");
			#endif


				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 삽입성공");
			#endif
				break;
				default:

				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (87)", clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;
		case CLANCHIP_SUBCHIP_APPOINT_FLAG:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "200", "1");

			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 WebDB.isPFlag(아이디 : %s,클랜이름 : %s,캐릭터이름 : %s,게임서버 : %s,플래그 : %s,0)", cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, cldata.PFlag);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (88)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 삽입실패");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;

				C_menuN = CLANCHIP_LEAVECLAN_SUCESS;
				menuInit(C_menuN);

				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 삽입성공");
			#endif
				break;
				default:

				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (89)", clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;
		case CLANCHIP_SUBCHIP_APPOINTMSG:
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

				C_menuN = CLANCHIP_SUBCHIP_READMSG_FLAG;

				menuInit(C_menuN);

				C_menuN2 = 0;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_SUBCHIP_RELEASEMSG:
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
				C_menuN = CLANCHIP_SUBCHIP_READMSG_FLAG;
				menuInit(C_menuN);

				C_menuN2 = 0;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_SUBCHIP_LIST:
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



		if(TJwheel != 0)
		{
			scrIconPos.top = tscr.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}



		
		{
			switch(i)
			{
				case 12:
				break;

				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:

				if(banisF[i - 4 + nScl_Bar] > 0)
				{
					banisF[i - 4 + nScl_Bar] = !banisF[i - 4 + nScl_Bar];

					nSavePos = i - 4 + nScl_Bar;
				}
				else
				{
					ZeroMemory(banisF, sizeof(banisF));
					banisF[i - 4 + nScl_Bar] = !banisF[i - 4 + nScl_Bar];
					nSavePos = i - 4 + nScl_Bar;
				}













				
				break;
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:

				if(banisF[i - 15 + nScl_Bar] > 0)
				{
					banisF[i - 15 + nScl_Bar] = !banisF[i - 15 + nScl_Bar];
					nSavePos = i - 14 + nScl_Bar;

				}
				else
				{
					ZeroMemory(banisF, sizeof(banisF));
					banisF[i - 15 + nScl_Bar] = !banisF[i - 15 + nScl_Bar];
					nSavePos = i - 15 + nScl_Bar;
				}













				
				break;

				case 0:

				C_menuN2 = 0;
				if(nSavePos < 0)
				{
					C_menuN = CLAN_CHIP;
				}
				else
				{

					if(strcmp(cldata.clanWon[nSavePos], cldata.SubChip) == 0)
					{
						C_menuN = CLANCHIP_RELEASESUBCHIP;
						bisSubChip = FALSE;
					}
					else if((cldata.clanWon[nSavePos][0] != 0) && (cldata.SubChip[0] != 0) && (strcmp(cldata.clanWon[nSavePos], cldata.SubChip) != 0))
					{
						C_menuN = CLANCHIP_RELEASESUBCHIP;
						bisSubChip = TRUE;
					}
					else
					{
						C_menuN = CLANCHIP_RESUBCHIP_APPOINT;
					}

				}

				menuInit(C_menuN);

				
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				
				break;
				default:

				
				break;
			}
		}
		break;
		case CLANCHIP_RESUBCHIP_APPOINT:
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


				C_menuN2 = 1;
				C_menuN = CLANCHIP_APPOINTSUBCHIP_DO;
				menuInit(C_menuN);

				break;

				case 1:


				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;

				default:
				break;
			}
		}
		break;
		case CLANCHIP_RESUBCHIP_APPOINT_END:
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
				C_menuN = CLAN_CHIP;

				menuInit(C_menuN);
				C_menuN2 = -1;

				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_RELEASESUBCHIP:
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


				C_menuN2 = 1;
				C_menuN = CLANCHIP_RELEASESUBCHIP_DO;
				menuInit(C_menuN);

				break;

				case 1:


				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;

				default:
				break;
			}
		}
		break;
		case CLANCHIP_RELEASESUBCHIP_END:
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
				ZeroMemory(cldata.SubChip, sizeof(cldata.SubChip));
				C_menuN = CLAN_CHIP;

				menuInit(C_menuN);

				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_APPOINTSUBCHIP_DO:
		switch(C_menuN2)
		{
			case 1:
			ANSdata.ret_val = -1;
			WebDB.Resume();
			WebDB.AppointSubChip(cldata.szID, cldata.ChaName, cldata.name, cldata.gserver, cldata.clanWon[nSavePos]);
			C_menuN2 = 2;

			break;
			case 2:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (90)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 3:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (100)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 2:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (91)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = 0;
				strcpy(cldata.SubChip, cldata.clanWon[nSavePos]);

				C_menuN = CLANCHIP_RESUBCHIP_APPOINT_END;
				menuInit(C_menuN);


				void SetClanWon();
				SetClanWon();
				break;


				case 100:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (92)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (93)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN2 = 0;
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (94)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = CLAN_AllEnd;
				}
				break;
			}
		}
		break;
		case CLANCHIP_SUBCHIP_LOAD:
		break;
		case CLANCHIP_RELEASESUBCHIP_DO:
		switch(C_menuN2)
		{
			case 1:
			ANSdata.ret_val = -1;
			WebDB.Resume();

			WebDB.ReleaseSubChip(cldata.szID, cldata.ChaName, cldata.name, cldata.gserver, cldata.SubChip);
			C_menuN2 = 2;

			break;
			case 2:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (95)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 2:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (96)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLANCHIP_RELEASESUBCHIP_END;
				menuInit(C_menuN);


				void SetClanWon();
				SetClanWon();

				if(bisSubChip)
				{
					C_menuN = CLANCHIP_RESUBCHIP_APPOINT;
					menuInit(C_menuN);
				}
				break;

				case 100:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (97)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (98)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN2 = 0;
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (99)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = CLAN_AllEnd;
				}
				break;
			}
		}
		break;

	#endif

		case CLANCHIP_GO_RELEASEMESSAGE:
		C_menuN = CLANCHIP_RELEASEMESSAGE;
		menuInit(C_menuN);
		break;
		case CLANCHIP_RELEASEMESSAGE:
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

				C_menuN = CLANCHIP_RELEASE_FLAG;

				menuInit(C_menuN);
				C_menuN2 = 0;

				break;
				default:
				i = i;
				break;

			}
		}
		break;

		case CLAN_REMOVE_SHORTCLANWON_LOADING:
		if(ANSdata.ret_val != -1) C_menuN = CLAN_REMOVE_SHORTCLANWON;
		break;
		case CLAN_REMOVE_LOADING:
		if(ANSdata.ret_val != -1) C_menuN = CLAN_REMOVE;
		break;
		case CLAN_RELEASE_LOADING:
		if(ANSdata.ret_val != -1) C_menuN = CLAN_RECONF_RELEASE;
		break;
		case CLAN_CHIP:
		i = chkeckMenuN();
		memset(menuMouseOver, -1, sizeof(menuMouseOver));
		switch(i)
		{

			case 2:
			menuMouseOver[2] = 1;
			break;
			case 3:
			menuMouseOver[3] = 1;
			break;
			case 4:
			menuMouseOver[4] = 1;
			break;

			case 5:
			menuMouseOver[5] = 1;
			break;
			case 6:

			menuMouseOver[6] = 1;
			break;
			case 7:
			menuMouseOver[7] = 1;
			break;
			case 18:
			if(cldata.stats[0] == '0') menuMouseOver[18] = 0;
			else menuMouseOver[18] = 1;
			break;


		#ifdef __CLANSUBCHIP__
			case 19:
			if(cldata.stats[0] == '0') menuMouseOver[19] = 0;
			else menuMouseOver[19] = 1;
			break;
		#endif

			default:
			memset(menuMouseOver, -1, sizeof(menuMouseOver));
			break;
		}

		
		{

			

			switch(i)
			{
				case 2:

				wsprintf(szClanMark, "%s", cldata.ClanMark);
				nMark = chkMark(szClanMark);
				switch(nMark)
				{
					case 0:
					C_menuN = CLAN_MARK1;
					menuInit(CLAN_MARK1);
					break;
					case 1:
					C_menuN = CLAN_MARK2;
					menuInit(CLAN_MARK2);
					break;
					case 2:
					C_menuN = CLAN_MARK3;
					menuInit(CLAN_MARK3);
					break;
				}
				break;
				case 3:
				ZeroMemory(banisF, sizeof(banisF));
				ZeroMemory(banisBuf1, sizeof(banisBuf1));
				ZeroMemory(banisBuf2, sizeof(banisBuf2));
				banisCnt = 1;

				C_menuN = CLAN_REMOVE;
				menuInit(CLAN_REMOVE);
				C_menuN2 = -1;
				break;
				case 4:
				C_menuN2 = -1;
				C_menuN = CLAN_RELEASE;
				menuInit(CLAN_RELEASE);
				break;
				case 5:


				LoadHelpImage();
				C_menuN2 = -1;

				LoadHelp(HELPCONTENT_TXT);
				C_menuN = CLAN_HELP_CONTENT;
				menuInit(C_menuN);

				break;
				case 6:
				C_menuN2 = -1;
				C_menuN = CLAN_AllEnd;
				break;
				case 18:
				if(cldata.stats[0] == '0')
				{
					C_menuN = CLANCHIP_NOLEAVECLAN;
				}
				else
				{
					C_menuN = CLANCHIP_LEAVECLAN_CONFIRM;
				}

				menuInit(C_menuN);
				break;


			#ifdef __CLANSUBCHIP__
				case 19:


				if(cldata.stats[0] == '0')
				{
					C_menuN = CLANCHIP_NOSUBCHIP;
					menuInit(C_menuN);

				}
				else
				{
					C_menuN = CLANCHIP_SUBCHIP_LOAD_DATA;
					menuInit(C_menuN);
					C_menuN2 = 0;
				}

				break;

			#endif
				default:
				break;
			}
		}
		break;
		case CLAN_WAREHOUSE:
		wsprintf(szErrorMsg, "창고메뉴는 준비중입니다.");
		C_menuN = CLAN_ERROR;
		menuInit(C_menuN);
		CLAN_ERROR_ret = CLAN_CHIP;
		break;


		case CLAN_MARK1:
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
				C_menuN = 4;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_MARK2:
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
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_MARK3:
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
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;






		case CLANCHIP_LEAVECLAN_MESSAGE:
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
				C_menuN = CLANCHIP_LEAVECLAN_MESSAGEEND;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_LEAVECLAN_NOLEVEL:
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
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_LEAVECLANDOWNLEVEL:
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
				C_menuN = CLAN_CHIP;

				menuInit(C_menuN);

				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;

		case CLANCHIP_NOLEAVECLAN:
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
				C_menuN = CLAN_CHIP;

				menuInit(C_menuN);

				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_LEAVECLAN_MESSAGEEND:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			if((cldata.nKFlag == 1) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "2", "0");
			}
			else if((cldata.nKFlag == 5) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "2", "0");
			}
			else if((cldata.nKFlag == 17) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "2", "0");
			}
			else if((cldata.nKFlag == 21) && (cldata.nKFlag_Count == 1))
			{
				WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "2", "0");
			}
			else
			{
			}

			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 WebDB.isPFlag(아이디 : %s,클랜이름 : %s,캐릭터이름 : %s,게임서버 : %s,플래그 : %s,0)", cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, cldata.PFlag);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (86)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 삽입실패");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;

				if((cldata.nKFlag == 1) && (cldata.nKFlag_Count == 1))
				{
					cldata.nKFlag_Count = 0;
				}
				else if(cldata.nKFlag == 17)
				{
					cldata.nKFlag_Count = 1;

					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 5)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 21)
				{
					cldata.nKFlag_Count = 1;
					if(cldata.SubChip[0] == 0)
					{
						bSubChip = FALSE;
						C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					}
					else
					{
						bSubChip = TRUE;
						C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					}

					menuInit(C_menuN);
					break;
				}
				else
				{
				}


				if((strcmp(cldata.stats, "0") == 0) && (strcmp(cldata.PFlag, "0") == 0))
				{
					wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);
					strcpy(szTemp, clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg, szDrawMenuMsg);
					CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
					strcpy(clanMsg6.szMsg, szTemp);

					if(cldata.bIsOneDay)
					{

						wsprintf(szDrawMenuMsg, clanMsg7.szMsg, cldata.nHour, cldata.nMinute);
						strcpy(szTemp, clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
						strcpy(clanMsg6.szMsg, szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);

						strcpy(szTemp, clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);

						strcpy(clanMsg6.szMsg, szTemp);
					}

					C_menuN = CLAN_NOMAKE;
					menuInit(C_menuN);
					bPFlag = FALSE;
					C_menuN2 = -1;
					break;
				}
				else if((strcmp(cldata.stats, "1") == 0) && (strcmp(cldata.PFlag, "0") == 0))
				{
					strcpy(cldata.PFlag, "2");
					strcpy(szTemp, clanMakeFinishClan.szMsg);
					wsprintf(szDrawMenuMsg, clanMakeFinishClan.szMsg, cldata.name);
					strcpy(clanMakeFinishClan.szMsg, szDrawMenuMsg);

					CheckMsgPos(&clanMakeFinishClan, BOX_MAX_WIDTH, 0);
					C_menuN = CLANCHIP_MAKE_FIRSTMESSAGE;
					menuInit(C_menuN);
					strcpy(clanMakeFinishClan.szMsg, szTemp);
					C_menuN2 = -1;
					bPFlag = TRUE;
					break;
				}








				else if((strcmp(cldata.stats, "1") == 0) && (strcmp(cldata.PFlag, "2") == 0) && (nClanWon_Num < 5))
				{

					wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);
					strcpy(szTemp, clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg, szDrawMenuMsg);
					CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
					strcpy(clanMsg6.szMsg, szTemp);

					if(cldata.bIsOneDay)
					{

						wsprintf(szDrawMenuMsg, clanMsg7.szMsg, cldata.nHour, cldata.nMinute);
						strcpy(szTemp, clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
						strcpy(clanMsg6.szMsg, szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);

						strcpy(szTemp, clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);

						strcpy(clanMsg6.szMsg, szTemp);
					}

					C_menuN = CLAN_NOMAKE;
					menuInit(C_menuN);
					C_menuN2 = -1;
					bPFlag = FALSE;
				}















				else
				{
					C_menuN = CLAN_CHIP;
					menuInit(C_menuN);
					C_menuN2 = -1;
					bPFlag = TRUE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGE에서 리턴코드 1 성공");
			#endif


				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 삽입성공");
			#endif
				break;
				default:

				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (87)", clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLANCHIP_LEAVECLAN_MESSAGEEND에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;


		case CLANCHIP_LEAVECLAN_PFLAG:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.isKPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "200", "1");

			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 WebDB.isPFlag(아이디 : %s,클랜이름 : %s,캐릭터이름 : %s,게임서버 : %s,플래그 : %s,0)", cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, cldata.PFlag);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (88)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 삽입실패");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;

				C_menuN = CLANCHIP_LEAVECLAN_SUCESS;
				menuInit(C_menuN);

				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 삽입성공");
			#endif
				break;
				default:

				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (89)", clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLANCHIP_LEAVECLAN_PFLAG에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;


		case CLANCHIP_LEAVECLAN_DO:
		switch(C_menuN2)
		{
			case 1:
			ANSdata.ret_val = -1;
			WebDB.Resume();
			WebDB.LeaveClan(cldata.name, cldata.clanWon[nSavePos], cldata.gserver);
			C_menuN2 = 2;

			break;
			case 2:
			switch(ANSdata.ret_val)
			{
				case 1:

				WebDB.Suspend();
				C_menuN2 = 0;
			#ifdef USE_PROSTONTALE		

				int sinMinusMoney(int Money, int Kind = 0);
				sinMinusMoney(300000);
			#endif
				C_menuN = CLANCHIP_LEAVECLAN_PFLAG;
				menuInit(C_menuN);
				break;
				case 2:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (83)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 3:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (82)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 4:
				WebDB.Suspend();
				C_menuN2 = 0;
				C_menuN = CLANCHIP_LEAVECLAN_NOLEVEL;
				menuInit(C_menuN);





				break;
				case 5:
				WebDB.Suspend();
				C_menuN2 = 0;
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (80)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case 6:
				WebDB.Suspend();
				C_menuN2 = 0;
				C_menuN = CLANCHIP_LEAVECLAN_NOLEVEL;
				menuInit(C_menuN);


				break;
				case 100:
				WebDB.Suspend();
				C_menuN2 = 0;

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (76)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (77)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN2 = 0;
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (78)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = CLAN_AllEnd;
				}
				break;
			}
		}
		break;
		case CLANCHIP_LEAVECLAN_RECONFIRM:

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


				C_menuN2 = 1;
				C_menuN = CLANCHIP_LEAVECLAN_DO;
				menuInit(C_menuN);

				break;

				case 1:


				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;

				default:
				break;
			}
		}
		break;
		case CLANCHIP_LEAVECLAN_CONFIRM:
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

				if(cldata.money < CLANCHIP_LEAVECLAN_MONEY)
				{
					C_menuN = CLANCHIP_LEAVECLAN_NOMONEY;
				}
				else
				{
					C_menuN = CLANCHIP_LEAVECLAN;
				}

				menuInit(C_menuN);

				break;

				case 1:

				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;

				default:
				break;
			}
		}
		break;
		case CLANCHIP_LEAVECLAN_NOMONEY:
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
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);




				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLANCHIP_LEAVECLAN_SUCESS:
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
		case CLANCHIP_LEAVECLAN:
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



		if(TJwheel != 0)
		{
			scrIconPos.top = tscr.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}



		
		{
			switch(i)
			{
				case 12:

				break;

				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:

				if(banisF[i - 4 + nScl_Bar] > 0)
				{
					banisF[i - 4 + nScl_Bar] = !banisF[i - 4 + nScl_Bar];

					nSavePos = i - 4 + nScl_Bar;
				}
				else
				{
					ZeroMemory(banisF, sizeof(banisF));
					banisF[i - 4 + nScl_Bar] = !banisF[i - 4 + nScl_Bar];
					nSavePos = i - 4 + nScl_Bar;
				}













				
				break;
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:

				if(banisF[i - 15 + nScl_Bar] > 0)
				{
					banisF[i - 15 + nScl_Bar] = !banisF[i - 15 + nScl_Bar];
					nSavePos = i - 14 + nScl_Bar;

				}
				else
				{
					ZeroMemory(banisF, sizeof(banisF));
					banisF[i - 15 + nScl_Bar] = !banisF[i - 15 + nScl_Bar];
					nSavePos = i - 15 + nScl_Bar;
				}













				
				break;

				case 0:

				C_menuN2 = 0;
				if(nSavePos < 0)
				{
					C_menuN = CLAN_CHIP;
				}
				else
				{
					C_menuN = CLANCHIP_LEAVECLAN_RECONFIRM;
				}

				menuInit(C_menuN);

				
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				
				break;
				default:

				
				break;
			}
		}
		break;




		case CLAN_REMOVE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.Banishiment(cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name, szRemoveClanWon);
			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE에서 WebDB.Banishiment(아이디 : %s,게임서버 : %s,클랜장 : %s,클랜이름 : %s,클랜원%s)", cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name, szRemoveClanWon);
		#endif
			break;
			case 1:

			if(ANSdata.ret_val != -1)
			{
				ANSdata.ret_val = ANSdata.ret_val;
			}

			switch(ANSdata.ret_val)
			{
				case -1:
				C_menuN = CLAN_REMOVE_LOADING;
				menuInit(C_menuN);
				break;

				case 0:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (12)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = 255;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE에서 트랜잭션오류");
			#endif
				break;
				case 1:
				case 3:

				WebDB.Suspend();
				void SetClanWon();
				SetClanWon();
				C_menuN2 = 0;
				C_menuN = CLAN_REMOVE_RESULT;
				menuInit(C_menuN);
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE에서 추방성공(%d)", ANSdata.ret_val);
			#endif

				break;
				case 2:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (13)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				CLAN_ERROR_ret = 255;

				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE에서 클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우(%d)", ANSdata.ret_val);
			#endif
				break;
				case 100:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (14)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				CLAN_ERROR_ret = 255;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE에서 리턴코드 100");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				wsprintf(szErrorMsg, "%s (15)", clanszError_Msg.szMsg);
				C_menuN = CLAN_ERROR;
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE에서 서버에접속할수 없음");
			#endif
				break;


				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();


					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (16)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = 255;
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLAN_REMOVE에서 디폴트에러");
				#endif
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



		if(TJwheel != 0)
		{
			scrIconPos.top = tscr.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}



		
		{
			switch(i)
			{
				case 12:
				break;

				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				if(nReleaseClanWon >= 5)
				{
					if(banisF[i - 4 + nScl_Bar] > 0)
					{
						banisF[i - 4 + nScl_Bar] = !banisF[i - 4 + nScl_Bar];
						nReleaseClanWon -= 1;
					}
				}
				else
				{

					banisF[i - 4 + nScl_Bar] = !banisF[i - 4 + nScl_Bar];
					if(banisF[i - 4 + nScl_Bar])
					{
						nReleaseClanWon += 1;
					}
					else
					{
						nReleaseClanWon -= 1;
					}
				}


				
				break;
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:
				if(nReleaseClanWon >= 5)
				{
					if(banisF[i - 14 + nScl_Bar] > 0)
					{
						banisF[i - 4 + nScl_Bar] = !banisF[i - 14 + nScl_Bar];
						nReleaseClanWon -= 1;
					}
				}
				else
				{

					banisF[i - 14 + nScl_Bar] = !banisF[i - 14 + nScl_Bar];
					if(banisF[i - 14 + nScl_Bar])
					{
						nReleaseClanWon += 1;
					}
					else
					{
						nReleaseClanWon -= 1;
					}
				}


				
				break;

				case 0:
				ZeroMemory(szRemoveClanWon, sizeof(szRemoveClanWon));
				for(i = 0; i < nClanWon_Num; i++)
				{
					if(banisF[i])
					{
						wsprintf(banisBuf1, "clwon%d=%s", banisCnt, cldata.clanWon[i]);

						if(banisCnt == 1)
						{
							wsprintf(banisBuf2, "%s", banisBuf1);
						}
						else
						{
							wsprintf(banisBuf2, "%s&%s", szRemoveClanWon, banisBuf1);
						}
						memcpy(szRemoveClanWon, banisBuf2, strlen(banisBuf2) + 1);
						banisCnt++;
					}
				}
				if(szRemoveClanWon[0] == 0)
				{
					C_menuN = CLAN_CHIP;
					menuInit(C_menuN);

					
					break;
				}

				ReMoveClanWon(szRemoveClanWon);
				if((nClanWon_Num - nReleaseClanWon) < 5)
				{
					bRemoveSecondCheck = FALSE;
					C_menuN = CLAN_REMOVE_SHORTCLANWON;
					menuInit(C_menuN);

					
					break;
				}
				else
				{

					bRemoveSecondCheck = TRUE;
					C_menuN = CLAN_REMOVE_SHORTCLANWON;
					menuInit(C_menuN);

					
					break;
				}
				C_menuN2 = 0;

				
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				
				break;
				default:

				
				break;
			}
		}
		break;
		case CLAN_REMOVE_SHORTCLANWON:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.Banishiment(cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name, szRemoveClanWon);

			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_REMOVE_SHORTCLANWON에서 WebDB.Banishiment(아이디 : %s,게임서버 : %s,클랜장 : %s,클랜이름 : %s,클랜원%s)", cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name, szRemoveClanWon);
		#endif
			break;
			case 1:

			switch(ANSdata.ret_val)
			{
				case -1:
				C_menuN = CLAN_REMOVE_SHORTCLANWON_LOADING;
				menuInit(C_menuN);
				break;

				case 0:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (17)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				CLAN_ERROR_ret = 255;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE_SHORTCLANWON에서 트랜잭션오류");
			#endif
				break;
				case 1:
				case 3:

				WebDB.Suspend();
				C_menuN2 = 0;

				void SetClanWon();
				SetClanWon();



				if(nReleaseClanWon > 0) cldata.nCurrentClanWon_Num -= nReleaseClanWon;

				C_menuN = CLAN_REMOVE_SHORTCLANWON_RESULT;
				menuInit(C_menuN);




				break;
				case 2:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (18)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);

				menuInit(C_menuN);
				CLAN_ERROR_ret = 255;

				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE_SHORTCLANWON에서 클랜이 존재하지 않거나 클랜 장이 아닌경우 또는 클랜원이 아닌경우(2)");
			#endif
				break;

				case 100:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (19)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = 255;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE_SHORTCLANWON에서 리턴코드 100");
			#endif

				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (20)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_REMOVE_SHORTCLANWON에서 서버에 접속할수 없음");
			#endif
				break;


				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();

					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (21)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = 255;
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLAN_REMOVE_SHORTCLANWON에서 디폴트에러");
				#endif
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
				ZeroMemory(banisBuf1, sizeof(banisBuf1));
				ZeroMemory(banisBuf2, sizeof(banisBuf2));

				memset(banisF, 0, sizeof(int) * 100);
				ZeroMemory(szRemoveClanWon, sizeof(char) * 500);
				ZeroMemory(szRemoveClanWonResult, sizeof(szRemoveClanWonResult));
				banisCnt = 1;
				C_menuN = CLAN_REMOVE;
				menuInit(C_menuN);
				break;
			}
		}
		break;
		case CLAN_REMOVE_SHORTCLANWON_RESULT:
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
				bClan6Down = TRUE;
				C_menuN2 = 0;
				C_menuN = CLAN_CHIP_ReadDATA;

				menuInit(C_menuN);

				break;
				default:
				break;
			}
		}
		break;
		case CLAN_REMOVE_RESULT:
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
				C_menuN2 = 0;
				C_menuN = CLAN_CHIP_ReadDATA;

				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;

		case CLAN_RELEASE:
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
				if(strcmp(cldata.stats, "0") == 0)
				{

					wsprintf(szReconfReleaseMsg, clanNoRecordReleaseMsg.szMsg);
				}
				else
				{

					wsprintf(szReconfReleaseMsg, clanszReConfReleaseMsg.szMsg);

				}
				C_menuN = CLAN_RECONF_RELEASE;
				menuInit(C_menuN);
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_RECONF_RELEASE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.BreakUP_Clan(cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name);
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_RECONF_RELEASE에서 WebDB.BreakUP_Clan(아이디 : %s,게임서버 이름 : %s,캐릭터이름 : %s,클랜이름 : %s)", cldata.szID, cldata.gserver, cldata.ClanZang_Name, cldata.name);
		#endif
			C_menuN2 = 1;
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case -1:
				C_menuN = CLAN_RELEASE_LOADING;
				menuInit(C_menuN);
				break;
				case 0:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (22)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = 255;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_RECONF_RELEASE에서 트랜잭션 오류");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				cldata.myPosition = CLANNO_REGESTER;
				C_menuN = CLAN_FINISH_RELEASE;
				menuInit(C_menuN);

				C_menuN2 = 0;

				void SetClanWon();
				SetClanWon();


				cldata.myPosition = CLANNO_REGESTER;
				ZeroMemory(cldata.name, sizeof(cldata.name));
				ZeroMemory(cldata.explanation, sizeof(cldata.explanation));
				ZeroMemory(cldata.ClanZang_Name, sizeof(cldata.ClanZang_Name));
				ZeroMemory(cldata.ClanMark, sizeof(cldata.ClanMark));
				cldata.intClanMark = 0;
				ZeroMemory(cldata.PFlag, sizeof(cldata.PFlag));

				ZeroMemory(szChatClanWon, sizeof(szChatClanWon));

			#ifdef VOICE_CHAT
				if(bCheckClan)
				{
					g_CtrlProcess.SetSelectCha(cldata.szID, cldata.ChaName,
											   NULL, 0, cldata.myPosition);


				}
			#endif


			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_RECONF_RELEASE에서 클랜해체 성공");
			#endif				


				break;
				case 2:
				WebDB.Suspend();


				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (23)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = 255;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_RECONF_RELEASE에서 현재 캐릭명이 현재 클랜의 장이 아니거나 해당클랜이 없음(2)");
			#endif
				break;
				case 100:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (24)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = 255;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_RECONF_RELEASE에서 리턴코드 100");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (25)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_RECONF_RELEASE에서 서버에 접속할수 없음");
			#endif
				break;

				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();

					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (26)", clanszError_Msg.szMsg);
					cClanReport.InitMsg(szErrorMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					CLAN_ERROR_ret = 255;
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLAN_RECONF_RELEASE에서 디폴트에러");
				#endif
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
				bRelease = TRUE;
				break;
				case 1:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_FINISH_RELEASE:
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

				C_menuN = CLAN_AllEnd;;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_HELP:
		i = chkeckMenuN();
		memset(menuMouseOver, -1, sizeof(int) * 30);

		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;
			case 2:
			menuMouseOver[2] = 1;
			break;
			case 3:
			menuMouseOver[3] = 1;
			break;
			case 4:
			menuMouseOver[4] = 1;
			break;
			case 5:
			menuMouseOver[5] = 1;
			break;
			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}


		
		{
			switch(i)
			{
				case 0:

				DestroyClanHelpIcon();
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				

				break;
				case 2:
				case 3:
				case 4:
				case 5:





				LoadHelp(ClanHelpIcon[i + nScl_Bar - 2].TextFilePath);

				C_menuN = CLAN_HELP_CONTENT;
				menuInit(C_menuN);

				

				break;


				case 6:
				break;

				default:

				
				break;
			}
		}
		break;
		case CLAN_HELP_CONTENT:
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




		if(TJwheel != 0)
		{
			scrIconPos.top = tscr.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}



		
		{
			switch(i)
			{
				case 0:
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				


				tscr.Init(0);
				break;
				case 5:

				break;

				default:

				
				break;
			}
		}

		break;
		case CLANCHIP_RELEASE_FLAG:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.isPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, "100", "0");
			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("CLANCHIP_RELEASE_FLAG에서 WebDB.isPFlag(아이디 : %s,클랜이름 : %s,캐릭터이름 : %s,게임서버 : %s,100,0)", cldata.szID, cldata.name, cldata.ChaName, cldata.gserver);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (27)", clanszError_Msg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_RELEASE_FLAG에서 삽입실패");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;
				C_menuN = CLAN_AllEnd;



			#ifdef CLAN_DEBUG
				DebugMessage("CLANCHIP_RELEASE_FLAG에서 삽입성공");
			#endif

				break;
				default:
				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (28)", clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("CLANCHIP_RELEASE_FLAG에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;
		case ISPFLAG_CHIP:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.isPFlag(cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, cldata.PFlag, "0");

			C_menuN2 = 1;
		#ifdef CLAN_DEBUG
			DebugMessage("ISPFLAG_CHIP에서 WebDB.isPFlag(아이디 : %s,클랜이름 : %s,캐릭터이름 : %s,게임서버 : %s,플래그 : %s,0)", cldata.szID, cldata.name, cldata.ChaName, cldata.gserver, cldata.PFlag);
		#endif
			break;
			case 1:
			switch(ANSdata.ret_val)
			{
				case 0:
				WebDB.Suspend();
				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (29)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_CHIP에서 삽입실패");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				C_menuN2 = -1;
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);

				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("ISPFLAG_CHIP에서 삽입성공");
			#endif
				break;
				default:

				if(ANSdata.ret_val != -1)
				{
					WebDB.Suspend();
					C_menuN = CLAN_ERROR;
					wsprintf(szErrorMsg, "%s (30)", clanszError_Msg.szMsg);
					cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
					cClanReport.InitMsg(szErrorMsg);
					C_menuN2 = -1;
				#ifdef CLAN_DEBUG
					DebugMessage("ISPFLAG_CHIP에서 디폴트에러");
				#endif
				}
				break;
			}
		}
		break;

		case CLANCHIP_MAKE_FIRSTMESSAGE:
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
				C_menuN = ISPFLAG_CHIP;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;
				default:
				break;
			}
		}
		break;
		case CLANCHIP_DOWN6_CLANWON_MESSAGE:
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
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;
				default:
				break;
			}
		}
		break;

		case CLANCHIP_DOWN6_CLANWON:
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
				C_menuN = ISPFLAG_CHIP;
				menuInit(C_menuN);
				C_menuN2 = 0;
				break;
				default:
				break;
			}
		}
		break;
		case CLAN_NOMAKE:
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
				C_menuN = CLAN_CHIP;
				menuInit(C_menuN);
				break;
				default:
				break;
			}
		}

		break;


		case CLAN_CHIP_ReadDATA:

		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();

			WebDB.ReadClan(cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#ifdef CLAN_DEBUG
			DebugMessage("CLAN_CHIP_ReadDATA에서 WebDB.ReadClan(아이디 : %s,게임서버 : %s,캐릭터이름 : %s)", cldata.szID, cldata.gserver, cldata.ClanZang_Name);
		#endif
			C_menuN2 = 1;
			break;
			case 1:

			switch(ANSdata.ret_val)
			{
				case -1:


				break;

				case 0:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (31)", clanszError_Msg.szMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				cClanReport.InitMsg(szErrorMsg);
				CLAN_ERROR_ret = 255;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 리턴코드 0 가입한 클랜이 없음");
			#endif
				break;
				case 1:
				WebDB.Suspend();
				ParsigClanWon(ANSdata.ret_Buf);
				if(bClan6Down)
				{
					bClan6Down = FALSE;
					C_menuN = CLAN_CHIP;
					menuInit(C_menuN);
					break;
				}

				if(cldata.nKFlag == 1)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 16)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 4)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 17)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 19)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 5)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 7)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 20)
				{
					if(cldata.SubChip[0] == 0)
					{
						bSubChip = FALSE;
						cldata.nKFlag_Count = 1;
						C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
						menuInit(C_menuN);
					}
					else
					{
						bSubChip = TRUE;
						cldata.nKFlag_Count = 1;
						C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
						menuInit(C_menuN);
					}
					break;
				}
				else if(cldata.nKFlag == 52)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 21)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_LEAVECLAN_MESSAGE;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 23)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 28)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_RELEASEMSG;
					menuInit(C_menuN);
					break;
				}
				else if(cldata.nKFlag == 55)
				{
					cldata.nKFlag_Count = 1;
					C_menuN = CLANCHIP_SUBCHIP_APPOINTMSG;
					menuInit(C_menuN);
					break;
				}
				else
				{
				}







				if((strcmp(cldata.stats, "0") == 0) && (strcmp(cldata.PFlag, "0") == 0))
				{
					wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);
					strcpy(szTemp, clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg, szDrawMenuMsg);
					CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
					strcpy(clanMsg6.szMsg, szTemp);

					if(cldata.bIsOneDay)
					{

						wsprintf(szDrawMenuMsg, clanMsg7.szMsg, cldata.nHour, cldata.nMinute);
						strcpy(szTemp, clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
						strcpy(clanMsg6.szMsg, szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);

						strcpy(szTemp, clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);

						strcpy(clanMsg6.szMsg, szTemp);
					}

					C_menuN = CLAN_NOMAKE;
					menuInit(C_menuN);
					bPFlag = FALSE;
					C_menuN2 = -1;
					break;
				}
				else if((strcmp(cldata.stats, "1") == 0) && (strcmp(cldata.PFlag, "0") == 0))
				{
					strcpy(cldata.PFlag, "2");
					strcpy(szTemp, clanMakeFinishClan.szMsg);
					wsprintf(szDrawMenuMsg, clanMakeFinishClan.szMsg, cldata.name);
					strcpy(clanMakeFinishClan.szMsg, szDrawMenuMsg);

					CheckMsgPos(&clanMakeFinishClan, BOX_MAX_WIDTH, 0);
					C_menuN = CLANCHIP_MAKE_FIRSTMESSAGE;
					menuInit(C_menuN);
					strcpy(clanMakeFinishClan.szMsg, szTemp);
					C_menuN2 = -1;
					bPFlag = TRUE;
					break;
				}








				else if((strcmp(cldata.stats, "1") == 0) && (strcmp(cldata.PFlag, "2") == 0) && (nClanWon_Num < 5))
				{

					wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);
					strcpy(szTemp, clanMsg6.szMsg);
					strcpy(clanMsg6.szMsg, szDrawMenuMsg);
					CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
					strcpy(clanMsg6.szMsg, szTemp);

					if(cldata.bIsOneDay)
					{

						wsprintf(szDrawMenuMsg, clanMsg7.szMsg, cldata.nHour, cldata.nMinute);
						strcpy(szTemp, clanMsg6.szMsg);
						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);
						strcpy(clanMsg6.szMsg, szTemp);
					}
					else
					{
						wsprintf(szDrawMenuMsg, clanMsg6.szMsg, cldata.nLastDay);

						strcpy(szTemp, clanMsg6.szMsg);

						strcpy(clanMsg6.szMsg, szDrawMenuMsg);

						CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);

						strcpy(clanMsg6.szMsg, szTemp);
					}

					C_menuN = CLAN_NOMAKE;
					menuInit(C_menuN);
					C_menuN2 = -1;
					bPFlag = FALSE;
				}














				else
				{
					C_menuN = CLAN_CHIP;
					menuInit(C_menuN);
					C_menuN2 = -1;
					bPFlag = TRUE;
				}
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 리턴코드 1 성공");
			#endif

				break;
				case 100:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (32)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
				CLAN_ERROR_ret = CLAN_AllEnd;
				C_menuN2 = -1;
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 리턴코드 100");
			#endif
				break;
				case ISAO_ERR_TCP_CONNECT:
				WebDB.Suspend();

				C_menuN = CLAN_ERROR;
				wsprintf(szErrorMsg, "%s (33)", clanszError_Msg.szMsg);
				cClanReport.InitMsg(szErrorMsg);
				cClanReport.menuInit(C_menuN, clanszError_Msg.pos.x, clanszError_Msg.pos.y);
			#ifdef CLAN_DEBUG
				DebugMessage("CLAN_CHIP_ReadDATA에서 서버에 접속할수 없음");
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
					DebugMessage("CLAN_CHIP_ReadDATA에서 디폴트에러");
				#endif
				}
				break;
			}
			break;
		}
		break;



	}

}




void CLANEntity_chip::RenderMain()
{
	if(life == 0) return;

}





void CLANEntity_chip::menuInit(int num) {}






int CLANEntity_chip::chkMark(char *clanName)
{
	nChkMark = -1;
	if(!SearchMarkClient(clanName))
	{
		if(!SearchMarkServer(clanName))
		{
			nChkMark = 0;
		}
	}
	else if(SearchMarkClient(clanName))
	{
		if(!SearchMarkServer(clanName))
		{
			nChkMark = 1;
		}
		else
		{
			nChkMark = 2;
		}
	}

	return nChkMark;
}

BOOL CLANEntity_chip::CheckMark(char* filename)
{
	return TRUE;
}

BOOL CLANEntity_chip::SearchMarkClient(char *clanName)
{
	return TRUE;
}

BOOL CLANEntity_chip::SearchMarkServer(char *clanName)
{
	return TRUE;
}




void CLANEntity_chip::Draw() {}


