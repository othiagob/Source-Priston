
#define CE_CHATLIST_CPP
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

#include "tjscroll.h"
#include "cE_chatlist.h"




#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"

#include "..\\ygy\\ygyheader.h"


#ifdef USE_PROSTONTALE		
#else
extern int MouseDblClick;
#endif

extern int TJmb2[3];

extern int vrunRuning;



extern int sinChatEnter;
extern int SubChatHeight;


#define MOUSE0_2() TJmb2[0]


int OpenBuf_x[50] = { 800, 800, 800, 800, 800, 800,
800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800, 800,
750,700,650,600,560,550,548,546,544,540,539,-1,-1 };


int CloseBuf_x[50] = { 500,556,606,656,706,756,800 };


extern char szConnServerName[16];


#ifdef PARKMENU
int g_nMicBtn;
#endif



int chatlistSPEAKERflag()
{
	return cClanChatting.Chatting_button;
}


void chatlistSPEAKERflagChg(int flag)
{
	cClanChatting.Chatting_button = flag;


	if(flag == 0)
	{
		for(int i = 0; i < 40; i++)
		{
			banisF[i] = 0;
		}
		void clan_chat_WhisperPartyPlayer_close();
		clan_chat_WhisperPartyPlayer_close();
	}
}




CLANEntity_chatting::CLANEntity_chatting()
{
	ZeroMemory(&ClanChatList, sizeof(CHATLIST));
	nBoxMove = 0;
	bIsDrawBoxFinish = 0;
	hChatListBoxLine = 0;
	hChatListBoxTitle = NULL;
	for(int i = 0; i < 2; i++)
	{
		hButtonClose[i] = NULL;
		hClanWon[i] = NULL;
		hChatIcon[i] = NULL;

	#ifdef PARKMENU
		hMicOption[i] = NULL;
		hMicVOLUPDOWN[i] = NULL;
	#endif
	}


	Chatting_button = 0;
	nSaveClanWonPos = -1;
	nDoublClick = -1;


#ifdef PARKMENU
	MicOpt_button = 0;
	g_nMicBtn = 0;
	hMicOptTitle = NULL;
	hMicSortLine = NULL;
	hMicONBack = NULL;
	hMicONChk = NULL;
	hMicVolBar = NULL;
#endif
}

CLANEntity_chatting::~CLANEntity_chatting()
{
}



int CLANEntity_chatting::Init(int Life)
{
	life = Life;
	InitClanChatList();
	return 1;
}

void CLANEntity_chatting::Close()
{
}













int cN_Pressf = 0;
int cN_PressfDBL = 0;



void cN_PressfDBL_clear()
{
	cN_PressfDBL = 0;
}



void CLANEntity_chatting::chat_WhisperPartyPlayer2(int num)
{

	if(cN_Pressf == 0)
	{
		cN_Pressf = num;
		return;
	}
	else
	{
		cN_PressfDBL = cN_Pressf;
		cN_Pressf = 0;
	#ifdef  USE_PROSTONTALE
		WhisperPartyPlayer(szChatClanWon[(cN_PressfDBL - 4)]);
	#endif
	}

}






void CLANEntity_chatting::chat_WhisperPartyPlayer(int num)
{


	if(num != -1)
	{
		cN_Pressf = num;
		return;
	}



	else
	{
		if(cN_PressfDBL == cN_Pressf)
		{
			cN_PressfDBL = 0;
			return;
		}

		cN_PressfDBL = cN_Pressf;
	#ifdef  USE_PROSTONTALE

		WhisperPartyPlayer(szChatClanWon[(cN_PressfDBL - 4)]);
	#endif
	}

}


void clan_chat_WhisperPartyPlayer_close()
{
	cClanChatting.chat_WhisperPartyPlayer_close();
}

void CLANEntity_chatting::chat_WhisperPartyPlayer_close()
{

	cN_Pressf = 0;




}














void chatList_Main_CHATLIST_END()
{
	bOpenChatList = FALSE;
}



int ret_menuN = 0;
void CLANEntity_chatting::Main() {}









void CLANEntity_chatting::menuInit(int num) {}




void CLANEntity_chatting::Draw() {}


void CLANEntity_chatting::InitClanWon()
{


	nTotalClanWon = 6;
	nRest_Num = nTotalClanWon - CHATING_DISPLAY_MAX;
}

void CLANEntity_chatting::SortClanWonList()
{

	int ZangL = strlen(cldata.ClanZang_Name);
	int ZangL2 = strlen(cldata.SubChip);
	BOOL bFlag = FALSE;
	int i;

	ZeroMemory(szChatClanWon, sizeof(szChatClanWon));

	if(cldata.myPosition == CLANCHIP)
	{
		if(cldata.SubChip[0] != 0)
		{
			if(cldata.SubChip[0] != 0)
				strcpy(szChatClanWon[0], cldata.SubChip);
			for(i = 0; i < nClanWon_Num; i++)
			{
				if(i > CLANWON_MAX) break;
				else
				{
					if(bFlag)
					{
						strcpy(szChatClanWon[i + 1], cldata.clanWon[i + 1]);
					}
					else
					{

						if(((ZangL2 == lstrlen(cldata.clanWon[i])) && strcmp(cldata.clanWon[i], cldata.SubChip) == 0))
						{
							strcpy(szChatClanWon[i + 1], cldata.clanWon[i + 1]);
							bFlag = TRUE;
						}
						else
						{
							strcpy(szChatClanWon[i + 1], cldata.clanWon[i]);
						}

					}
				}
			}
		}
		else
		{
			memcpy(szChatClanWon, cldata.clanWon, sizeof(cldata.clanWon));
		}
	}


	int j = 0;
	if((cldata.myPosition == CLANUSER) || (cldata.myPosition == CLAN_SUBCHIP))
	{
		i = 0;
		strcpy(szChatClanWon[0], cldata.ClanZang_Name);
		if(cldata.SubChip[0] != 0)
		{
			if(strcmp(cldata.ChaName, cldata.SubChip) != 0)
			{
				strcpy(szChatClanWon[1], cldata.SubChip);
				i = 1;
			}

		}
		while(i < nClanWon_Num)
		{


			if(((ZangL == lstrlen(cldata.clanWon[j])) && strcmp(cldata.clanWon[j], cldata.ClanZang_Name) == 0)
			   || ((ZangL2 == lstrlen(cldata.clanWon[j])) && strcmp(cldata.clanWon[j], cldata.SubChip) == 0))
			{
				j++;

			}
			else
			{
				++i;
				strcpy(szChatClanWon[i], cldata.clanWon[j]);
				j++;

			}

		}
	}

}

void CLANEntity_chatting::Load2() {}

void CLANEntity_chatting::Load() {}

void CLANEntity_chatting::DisplayInfo() {}

void CLANEntity_chatting::ChatListUserData(char *id, char *gserver, char *chaid)
{
	strcpy(ClanChatList.szUserID, id);
	strcpy(ClanChatList.szChaName, chaid);
	strcpy(ClanChatList.szGameServer, gserver);
}

void CLANEntity_chatting::ChatListParsingIsClanMember(char* data)
{



	chk_readStrBuf("CZang=", data, ClanChatList.szClanZang_Name);









}


void CLANEntity_chatting::DrawBox() {}









int cnt__00 = 0;
void CLANEntity_chatting::Draw_C_menuN() {}

void CLANEntity_chatting::InitClanChatList()
{
	OCcnt = 0;
	ChatListBoxX = 800;
	ChatListBoxY = 94;

	cnt = 0;
	AddSize = 0;
}

void CLANEntity_chatting::MoveClose()
{
	ChatList_CmenuN = CLAN_DISPLAY_CLOSE;
}

void CLANEntity_chatting::NoScroll()
{
	OCcnt = 28;
	ChatListBoxX = OpenBuf_x[OCcnt];
	OCcnt += 1;

	bOpenChatList = TRUE;
}




int CLANEntity_chatting::chkMenu(int x, int y, RECT *r)
{
	if(r->left <= x   &&   x <= r->right)
		if(r->top <= y   &&   y <= r->bottom)
			return 1;
	return 0;
}
int CLANEntity_chatting::chkeckMenuN()
{

	return 1;
}



void CLANEntity_chatting::DoubleClick(int i)
{
	if(banisF[(i - 4) + nScl_Bar] == 1)
		banisF[(i - 4) + nScl_Bar] = 2;
	else if(banisF[(i - 4) + nScl_Bar] == 2) banisF[(i - 4) + nScl_Bar] = 0;
	else
	{
		banisF[(i - 4) + nScl_Bar] = 2;
	}
}