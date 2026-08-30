//ktj : ºÎÅ¬·£ÀÇ ±ÇÀ¯°ü·Ã : netplay.cpp¿¡µµ ÀÖÀ½.

//====================================================================
//ktj : runmap3d.suoÆÄÀÏ¶§¹®¿¡ ¼¼ÀÌºê°¡¾ÈµÇ´Â°æ¿ì°¡ ÀÖÀ¸¹Ç·Î ±×·²¶§´Â ±× ÆÄÀÏÀ» Áö¿ï°Í
//====================================================================
//5.7 P4.28
//sinMain.cpp, sinInterFace.cpp¿¡¼­ ¿î¿µµÊ
#define TJCLAN_CPP

#include "tjclanDEF.h"


#ifdef USE_PROSTONTALE		//=========== ÇÁ¸®½ºÅÏ¼Ò½ºÀÎ°æ¿ì




#include "..\\..\\sinbaram\\SinLinkHeader.h"

//#include "HoParty.h"
#include "..\\..\\language.h"


#include "tjclan.h"
#include "tjscroll.h"
#include "cE_Cmake.h"
#include "cE_chip.h"
#include "cE_user.h"
#include "cE_isclanmember.h"
#include "cE_report.h"
#include "cE_chatlist.h"
#include "cE_deletecha.h"
#include "cE_Cjoin.h"
#include "cE_CViewClanInfo.h"
#include "cE_Updatecldata.h"
#include "cE_Notice.h"

#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "Help.h"
#include "GuideHelp.h" 

#include "..\\ygy\\ygyheader.h"


//#include "NpcWav.h" //npc ¿þÀÌºê ¹Þ¾Æ¿À±â
//#include "Zip.h"


//#define USE_YAHEL		//¾ÆÀ£¼­¹ö¸¸ »ç¿ëÇÏ´Â°æ¿ì´Â ¿­¾îµÑ°Í




extern char	szConnServerName[16];	//Á¢¼ÓÇÑ ¼­¹öÀÌ¸§ÀÌ µé¾îÀÖ´Â ¹öÆÛ






#else						//=========== Æ÷¸®½ºÅÏ¼Ò½º°¡ ¾Æ´Ñ°æ¿ì

#include <windows.h>
#include <stdio.h>
#include "tjclan.h"
#include "tjscroll.h"

#include "clan_Enti.h"
#include "cE_Cmake.h"
#include "cE_chip.h"
#include "cE_user.h"
#include "cE_isclanmember.h"
#include "cE_report.h"
#include "cE_chatlist.h"
#include "cE_deletecha.h"
#include "cE_CJoin.h"
#include "cE_CViewClanInfo.h"
#include "cE_Updatecldata.h"
#include "cE_Notice.h"

#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "../../netplay.h"





#define MAX_PARTY_MEMBER	6

#define PARTY_NONE		500
#define PARTY_START		1000
#define PARTY_PROCESS	2000
#define PARTY_END		3000

char szConnServerName[16];	//Á¢¼ÓÇÑ ¼­¹öÀÌ¸§ÀÌ µé¾îÀÖ´Â ¹öÆÛ



extern HWND    g_hwnd;





#endif


int ClanCharMove = 0;	//Å¬·£¸Þ´ºÆÇ¾ÈÀÇ ¸¶¿ì½ºÀÇ ¿òÁ÷ÀÓ¿¡ Ä³¸¯ÅÍ°¡ ¿µÇâ¾øµµ·Ï ÇÑ´Ù.
int ClanCLCharMove = 0;//Å¬·£Ã¤ÆÃ¸®½ºÆ®¸Þ´ºÆÇ¾ÈÀÇ ¸¶¿ì½ºÀÇ ¿òÁ÷ÀÓ¿¡ Ä³¸¯ÅÍ°¡ ¿µÇâ¾øµµ·Ï ÇÑ´Ù.


int  banisF[100];

//int TJwheel=0; //ktj : ÈÙµ¥ÀÌÅ¸



//À¯±¤¿­ µµ¿ò¸» ¸¶¿ì½º ÀÌº¥Æ® °®¾î¿À±â====START
int GuideHelpButton[3] = { 0,0,0 }; //°¡ÀÌµå µµ¿ò¸»
int LevelpUpButton[3] = { 0,0,0 }; //·¹º§¾÷ µµ¿ò¸»
//END

BOOL bKeyDown = FALSE;
int nKeyDown = 0;
BOOL bKeyUp = FALSE;
int nKeyUp = 0;
BOOL bLButtonDown = FALSE;
BOOL bKeyPageDown = FALSE;
BOOL bKeyPageUp = FALSE;

BOOL IsDrawGuideHelp = FALSE; //°¡ÀÌµå µµ¿ò¸»ÀÌ ¶°Á®ÀÖÀ½

/*void GuidHelpMouse(int msg,WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_KEYUP:
		switch(wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = FALSE; //°¡ÀÌµå µµ¿ò¸»
			bKeyUp = FALSE;
			bKeyDown = FALSE;
			break;
		case VK_UP:
			bKeyDown = FALSE;
			bKeyUp = FALSE;
			GuideHelpButton[0] = FALSE; //°¡ÀÌµå µµ¿ò¸»
			break;
		}
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = TRUE; //°¡ÀÌµå µµ¿ò¸»
			bKeyDown = TRUE;
			bKeyUp = FALSE;
			nKeyDown++;
			break;
		case VK_UP:
			GuideHelpButton[0] = TRUE; //°¡ÀÌµå µµ¿ò¸»
			bKeyUp = TRUE;
			bKeyDown = FALSE;
			nKeyUp++;
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		//À¯±¤¿­==============start
		bKeyDown = FALSE;
		bKeyUp = FALSE;
		GuideHelpButton[0] = TRUE; //°¡ÀÌµå µµ¿ò¸»
		LevelpUpButton[0]=TRUE; //·¹º§¾÷ µµ¿ò¸»
		//end
		break;
	case WM_LBUTTONUP:
		bKeyDown = FALSE;
		bKeyUp = FALSE;
		GuideHelpButton[0] = FALSE; //°¡ÀÌµå µµ¿ò¸»
		LevelpUpButton[0]=FALSE; //·¹º§¾÷ µµ¿ò¸»
		break;
	}
}
*/
void clan_Mouse(int msg, WPARAM wParam)
{
	switch (msg) {
		//À¯±¤¿­ WM_KEYUPÃß°¡
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = FALSE; //°¡ÀÌµå µµ¿ò¸»
			bKeyUp = FALSE;
			bKeyDown = FALSE;
			break;
		case VK_UP:
			bKeyDown = FALSE;
			bKeyUp = FALSE;
			GuideHelpButton[0] = FALSE; //°¡ÀÌµå µµ¿ò¸»			
			break;

		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
			GuideHelpButton[0] = TRUE; //°¡ÀÌµå µµ¿ò¸»
			bKeyDown = TRUE;
			bKeyUp = FALSE;
			nKeyDown++;
			break;
		case VK_UP:
			GuideHelpButton[0] = TRUE; //°¡ÀÌµå µµ¿ò¸»
			bKeyUp = TRUE;
			bKeyDown = FALSE;
			nKeyUp++;
			break;
		case VK_ESCAPE://escÅ°·Î µµ¿ò¸» ²ô±â
			g_GuideHelp.GuidHelpEnd();
			g_Help.HelpEnd();
			break;

		}
		break;
		//==============END

	case WM_LBUTTONDOWN:
		TJmb2[0] = TJmb[0] = TRUE;
		//À¯±¤¿­==============start		
		bKeyDown = FALSE;
		bKeyUp = FALSE;

		GuideHelpButton[0] = TRUE; //°¡ÀÌµå µµ¿ò¸»
		LevelpUpButton[0] = TRUE; //·¹º§¾÷ µµ¿ò¸»	

		//end
		break;

	case WM_LBUTTONUP:
		TJmb2[0] = TJmb[0] = FALSE;
		//À¯±¤¿­==============start
		bKeyDown = FALSE;
		bKeyUp = FALSE;
		bLButtonDown = FALSE;

		GuideHelpButton[0] = FALSE; //°¡ÀÌµå µµ¿ò¸»
		LevelpUpButton[0] = FALSE; //·¹º§¾÷ µµ¿ò¸»
		//==============END
		break;

	case WM_RBUTTONDOWN:
		TJmb2[1] = TJmb[1] = TRUE; //ktj
		break;

	case WM_RBUTTONUP:
		TJmb2[1] = TJmb[1] = FALSE; //ktj
		break;

	case WM_MBUTTONDOWN:
		TJmb2[2] = TJmb[2] = TRUE; //ktj
		break;

	case WM_MBUTTONUP:
		TJmb2[2] = TJmb[2] = FALSE; //ktj
		break;
	}
}









void ktj_imsi_clanMENU_Main() {}

void ktj_imsi_clanMENU_Draw() {}


void clanMENUinit()
{
	Interface_clanmenu.CLANmenu_INIT();
	//ktj : Å¬·£»ó¿¡¼­ÀÇ ÇöÀç À§Ä¡¸¦ ¾Ë¾Æ³½´Ù.
	Interface_clanmenu.CLANmenu_clanPOSITION();
}


//·¹º§¾÷À» ÇßÀ»¶§ ÇÔ¼ö¸¦ È£ÃâÇØÁØ´Ù.
//=========·¹º§¾÷ µµ¿ò¸»==========================
//int ng = 1;
void InitHelp()
{
	if (!g_GuideHelp.GetIsOn()) return; //·¹º§¾÷ µµ¿ò¸»ÀÌ ²¨Á®ÀÖÀ» °æ¿ì
	//if((sinChar->Level == 20)&&(sinChar->JOB_CODE > 4)) return;
	g_Help.SetStartPos(0, 0);
	if (!g_Help.CheckLevel(sinChar->Level, sinChar->JOB_CODE)) return;
	g_Help.SetHelpN(LEVELUP_HELP);
	g_Help.menuInit(LEVELUP_HELP);
	/*switch(ng)
	{
	case 1:
		g_Help.CheckLevel(1,2);
		break;
	case 2:
		g_Help.CheckLevel(1,5);
		break;
	case 3:
		g_Help.CheckLevel(2,2);
		break;
	case 4:
		g_Help.CheckLevel(3,2);
		break;
	case 5:
		g_Help.CheckLevel(4,2);
		break;
	case 6:
		g_Help.CheckLevel(5,2);
		break;
	case 7:
		g_Help.CheckLevel(6,2);
		break;
	case 8:
		g_Help.CheckLevel(8,2);
		break;
	case 9:
		g_Help.CheckLevel(10,2);
		break;
	case 10:
		g_Help.CheckLevel(10,5);
		break;
	case 11:
		g_Help.CheckLevel(12,2);
		break;
	case 12:
		g_Help.CheckLevel(15,2);
		break;
	case 13:
		g_Help.CheckLevel(17,2);
		break;
	case 14:
		g_Help.CheckLevel(20,2);
		break;
	case 15:
		g_Help.CheckLevel(22,2);
		break;
	case 16:
		g_Help.CheckLevel(25,2);
		break;
	case 17:
		g_Help.CheckLevel(30,2);
		break;
	case 18:
		g_Help.CheckLevel(40,2);
		break;
	case 19:
		g_Help.CheckLevel(50,2);
		break;
	case 20:
		g_Help.CheckLevel(60,2);
		break;
	case 21:
		g_Help.CheckLevel(65,2);
		break;
	case 22:
		g_Help.CheckLevel(70,2);
		break;
	case 23:
		g_Help.CheckLevel(80,2);
		ng = 0;
		break;
	}


	ng++;*/
}

void InitGuideHelp()
{
	g_GuideHelp.SetDoing(TRUE);
	g_GuideHelp.SetStartPos(0, 0);
	g_GuideHelp.CheckLevel(sinChar->Level);
	//g_GuideHelp.CheckLevel(17);
	g_GuideHelp.SetHelpN(GUIDEHELP_LIST);
	g_GuideHelp.menuInit(GUIDEHELP_LIST);

	IsDrawGuideHelp = TRUE;
}



void HelpDraw()
{
	if (g_Help.GetHelpN() != -1)
	{
		switch (g_Help.GetHelpN())
		{
		case LEVELUP_HELP:
			g_Help.Draw(0);
			break;
		case LEVELUP_HELP_CONTENT:

			g_Help.Draw(0);
			break;
		}
	}
}

void CLAN_MENU::GuideHelpDraw()
{
	if (g_GuideHelp.GetHelpN() != -1)
	{
		switch (g_GuideHelp.GetHelpN())
		{
		case GUIDEHELP_LIST:
			DrawBox(NOTICE_X, NOTICE_Y - 50, 9, 9);
			g_GuideHelp.Draw();
			break;
		case GUIDEHELP_CONTENT:
			DrawBox(NOTICE_X, NOTICE_Y - 50, 9, 9);
			g_GuideHelp.Draw();
			break;

		}
	}
}
/*//========¼­¹ö¿¡¼­ ¿þÀÌºê ¹Þ±â ¼¼ÆÃ===================================================
int GoWavDown_Check = 0;

char szWavUrl[255] = {0,};
BOOL bWavError = FALSE;

//1.¼­¹ö url¸¦ °¡Áö°í ¿Â´Ù
void GetWaveUrl(char* url)
{
	if((url == NULL)||(url[0] == 0 ))
	{
		bWavError = TRUE;
		return;
	}

	ZeroMemory(szWavUrl,sizeof(szWavUrl));
	strcpy(szWavUrl,url);
	GoWavDown_Check = 1;
}

//2.¸ÞÀÎ¿¡¼­ ½ÇÇàÇÏ´Â ÇÔ¼ö
int DoWavDown_check(char* FileName)
{
	if(OpenFlag == 1) return -1;  //´Ù¸¥°Ô ¿­·ÁÀÖÀ¸¸é ¾ÈÇÏ°í ³ªÁß¿¡ ÇÑ´Ù.

	GoWavDown_Check = 1;
	if(GoWavDown_Check == 1)
	{
		void DoWavDown(char* FileName);
		DoWavDown(FileName);
	}

	return 0;
}

void DoWavDown(char* FileName)
{
	C_menuN = -1;
	OpenFlag = 1;
	g_cNpcWav.SetNpcWavNum(NPCWAVE_CALL);
	g_cNpcWav.SetFileName(FileName);
}
*/
//=============°×µé¾î°¡±âÁ÷Àü¿¡ ³ª¿À´Â °æ°í ¸Þ¼¼Áö=====================
int GoNotice_chking = 0;
int m_nNoticeCount = -1;
char szNoticeUrl[255] = { 0, };
//1. ¼ÂÆÃÇÔ¼ö
int GetNoticeUrl(char* url)
{
	GoNotice_chking = 0;
	ZeroMemory(szNoticeUrl, sizeof(szNoticeUrl));
	strcpy(szNoticeUrl, url);

	GoNotice_chking = 1;
	return 1;
}
//2. ¸ÞÀÎ¿¡¼­ ½ÇÇàÀ» ±ÇÇÏ´Â ÇÔ¼ö
void GoNotice_chk()
{
	if (OpenFlag == 1) return;  //´Ù¸¥°Ô ¿­·ÁÀÖÀ¸¸é ¾ÈÇÏ°í ³ªÁß¿¡ ÇÑ´Ù.

	if (GoNotice_chking == 1) {
		if (m_nNoticeCount < NOTICECOUNT - 1)
		{
			GoNotice_chking = 0;
			++m_nNoticeCount;
			void GoNotice();
			GoNotice();
		}

	}
}

//3. ½ÇÇàÅä·ÏÇÏ´Â ÇÔ¼ö
void GoNotice()
{
	C_menuN2 = 0;
	OpenFlag = 1;
	C_menuN = CLAN_NOTICE;

	//cClanNotice.SetNoticeCount(1);

}




/*==================================================================================
//
// ¿ÜºÎ¿¡¼­ »ç¿ëµÇ´Â Å¬·£ÀÇ ±¸µ¿ ¼ÂÆÃ ÇÔ¼öµé								   start
//
==================================================================================*/


/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : Å¬·£ ¸Ç Ã³À½ ¼ÂÆÃ
///////////////////////////////////////////////////////////////////////////*/

//ktj : Å¬·£¸Þ´º ÀÎÅÍÆäÀÌ½º init
//setN : 0 : ÃÖÃÊ ¼ÂÆÃ  1: µÎ¹øÂ°¼ÂÆÃ(¸ðµç 
void clanMENU_INIT(int setN)
{
}


//hoLogin.cpp¿¡¼­ »ç¿ëÇÔ.
int isDELETE_character(char* charName)
{
	if (!bCheckHostNameandPort) return 1;		//1Àº Ä³¸¯ÅÍ¸¦ Áö¿öµµµÈ´Ù´Â¶æ


	int web_DB_state_FLAG_WAIT();
	while (1) {
		if (web_DB_state_FLAG_WAIT() == 1)
			break;
	}


	Interface_clanmenu.CLANmenu_clanPOSITION();
	cldata.myPosition = 0;//CLANNO_REGESTER;
	while (1) {
		web_DB_main();					//web db¸¦ »ç¿ëÇÏ´Â Å¬·£ ¸Þ´º¿¡¼­¸¸ ºÒ¸®¿öÁü.	
		cIsClanMember.Main();			//ktj : Å¬·£¿øÀÇ Æ÷Áö¼Ç Ã¼Å© ¸ÞÀÎ

		if (cldata.myPosition != 0)
			break;
	}

	while (1) {
		if (web_DB_state_FLAG_WAIT() == 1)
			break;
	}

	/*
	#define CLANUSER					100		//ÀÏ¹ÝÀ¯Àú
	#define CLANCHIP					101		//¯…
	#define CLANNO_REGESTER				102		//¹«µî·ÏÀÚ
	#define CLAN_NOCONNECT				103		//ÄÁ³ØÆ®°¡ ¾ÈµÇ¼­ ¼³Á¤ºÒ°¡
	*/
	return cldata.myPosition;
}



void CLAN_MENU::Init2() {}

//notice¸Þ½ÃÁö·Îµå¿ëÀÇ Init2()¿¡ »óÀÀÇÏ´Â ÇÔ¼ö
void CLAN_MENU::NoticeMessage_Init2() {}




void CLAN_MENU::CLANmenu_INIT()
{
}


/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : °èÁ¤¿¡ ¸¸µé¾îÁø Ä³¸¯ÅÍµéÀÇ Å¬·£Á¤º¸ ¾Ë¾Æ¿À±â
///////////////////////////////////////////////////////////////////////////*/
BOOL bGetChaClan = FALSE; //°èÁ¤¿¡ °ü·ÃµÈ Ä³¸¯ÅÍµéÀÇ Å¬·£Á¤º¸¸¦ ÀÐ¾î¿À´ÂÁö Ã¼Å©
int  nGetChaClan = 0;   //0: ÁøÇà¾ÈµÊ 1: ÁøÇàµÇ°íÀÖÀ½ 2: ÁøÇà¿Ï·á  3: ¿¡·¯
int nStatusCount = 0;


int cntsu = 0;
char* szSelectCha[5][2];


void InitSelectCha()
{
	if (!bCheckHostNameandPort) return;

	nStatusCount = 0;
	ZeroMemory(chacldata, sizeof(chacldata));
	ZeroMemory(szSelectCha, sizeof(szSelectCha));
	cntsu = 0;
}

void ClearSelectCha()
{
	if (!bCheckHostNameandPort) return;
	bGetChaClan = FALSE;
	ZeroMemory(szSelectCha, sizeof(szSelectCha));
	ZeroMemory(chacldata, sizeof(chacldata));
	ZeroMemory(&cldata, sizeof(CLANDATA));
	nStatusCount = 0;

}

//Ä³¸¯ÅÍµéÀÇ ÀÌ¸§À» ÀÔ·Â
int HONG_GetChaClanName(char* id, char* chaname)
{
	if (!bCheckHostNameandPort) return 0;


	int i;
	int nReturn = 0;;
	for (i = 0; i < 5; i++)
	{
		if (szSelectCha[i][0] == NULL)
		{
			szSelectCha[i][0] = id;
			szSelectCha[i][1] = chaname;
			nReturn = 1;

			break;
		}
	}

	cntsu = i + 1;

	if (i == 5) return -1;

	return nReturn;
}




int  main_chaC()
{
	return 0;
}


int GetChaClanName(char* id, char* chaname, char* gserver)
{
	return 1;
}









/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : Å¬·£¿øÀ¸·Î¼­ÀÇ À§Ä¡¸¦ ¾Ë¾Æ³¿.
///////////////////////////////////////////////////////////////////////////*/

#ifdef VOICE_CHAT
BOOL bCheckClan = FALSE;
#endif
int CLAN_MENU::CLANmenu_clanPOSITION()
{

	if (!bCheckHostNameandPort) {
		cldata.myPosition = CLAN_NOCONNECT;
		return 0;
	}

	if (OpenFlag == 1) return 0;		//ÀÌ ¸Þ´º°¡ ´ÝÇôÀÖÀ»¶§¸¸ °¡´ÉÇÔ.

	C_menuN2 = 0;
	OpenFlag = 1;
	joinAnswer = 0;
	C_menuN = CLAN_isClanWon;
	menuInit(C_menuN);
	ClanCharMove = 1;			//ktj : ¸¶¿ì½º°¡ Ã½Å©ÇÏ´Â ¹Ú½ºÃ½Å© 

#ifdef VOICE_CHAT	
	bCheckClan = TRUE;
#endif
	return 1;
}


/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : NPC¿Í ¸¸³µÀ»¶§
///////////////////////////////////////////////////////////////////////////*/
void CLANmenu_meetNPC()	//newplay.cpp¿¡¼­ »ç¿ëÇÔ.
{
	Interface_clanmenu.CLANmenu_meetNPC();
}
int CLAN_MENU::CLANmenu_meetNPC()
{
	if (OpenFlag == 1) {
		if (C_menuN == CLAN_DISPLAY_INFO)
		{
			C_menuN = CLAN_CLOSE_END_BEFORE;

		}
		return 0;		//ÀÌ ¸Þ´º°¡ ´ÝÇôÀÖÀ»¶§¸¸ °¡´ÉÇÔ.
	}


	C_menuN2 = 0;
	OpenFlag = 1;
	joinAnswer = 0;
	if (!bCheckHostNameandPort)
	{
		C_menuN = CLANSERVER_INSPECT_BEFORE;

	}
	else
	{
		C_menuN = CLAN_GO_NPCSTART;
	}


	//menuInit(C_menuN );

	ClanCharMove = 1;			//ktj : ¸¶¿ì½º°¡ Ã½Å©ÇÏ´Â ¹Ú½ºÃ½Å© 
	return 1;
}






/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : Å¬·£¿øµéÀÇ Ã¤ÆÃ¸®½ºÆ®¸¦ º¼¶§
///////////////////////////////////////////////////////////////////////////*/
void CLANmenu_ChatList(int sw)	//newplay.cpp¿¡¼­ »ç¿ëÇÔ.l
{
	Interface_clanmenu.CLANmenu_ChatList(sw);
}
void CLAN_MENU::CLANmenu_ChatList(int sw)
{

	if (!bCheckHostNameandPort) return;
	if (OpenFlag == 1) return;
	if (bOpenChatList) return;

	ChatList_CmenuN2 = 0;

	switch (sw)
	{
	case 0: //½ºÅ©·Ñ µÇ°Ô

		cClanChatting.InitClanChatList();
		if (bIsReadData) ChatList_CmenuN = CLAN_DISPLAY_OPEN;
		else	ChatList_CmenuN = CHATLIST_READDATA;

		cClanChatting.menuInit(ChatList_CmenuN);

		break;
	case 1: //½ºÅ©·Ñ ¾ÈµÇ°Ô
		cClanChatting.NoScroll();
		//if(bIsReadData) ChatList_CmenuN = CLAN_DISPLAY_OPEN;
		//else	ChatList_CmenuN = CHATLIST_READDATA;		

		ChatList_CmenuN = CLAN_DISPLAY_INFO;
		cClanChatting.menuInit(ChatList_CmenuN);

#ifdef USE_PROSTONTALE		//=========== ÇÁ¸®½ºÅÏ¼Ò½ºÀÎ°æ¿ì

#else
		OpenFlag = 1;
		C_menuN = ChatList_CmenuN;

#endif


		break;
	}
}




void CLANmenu_ChatList_Main()	//newplay.cpp¿¡¼­ »ç¿ëÇÔ.l
{
	cClanChatting.Main();			//Å¬·£ Ã¤ÆÃ¸®½ºÆ®
}
void CLANmenu_ChatList_Draw()	//newplay.cpp¿¡¼­ »ç¿ëÇÔ.l
{
	cClanChatting.Draw();			//Å¬·£ Ã¤ÆÃ¸®½ºÆ®
}


/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : cldata¾÷µ¥ÀÌÆ® ÇÒ¶§
///////////////////////////////////////////////////////////////////////////*/
void Updatecldata()
{
	//Interface_clanmenu.Updatecldata();
	g_UpdateCldata.Updatecldata();
}


/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í : ±ÇÀ¯ÇÏ´Â ¸Þ´ºµé.
: ÇÏ ´Â ÀÏ : ÀÏ¹ÝÀÎ¿¡°Ô Å¬·£ÀÇ °¡ÀÔÀ» ±ÇÀ¯ÇÏ¸é ±ÇÀ¯ÇÑ´Ù´Â ¸Þ´º°¡ ¶á´Ù.
///////////////////////////////////////////////////////////////////////////*/

//±ÇÀ¯¹Þ´Â »ç¶÷ÀÌ »ç¿ëÇÏ´Â ÇÔ¼ö
void ClanJoin(char* oneClanName, char* oneChipCharid)
{
	//Interface_clanmenu.SetJoinData("¿ïÆ®¶ó","ÇåÆ®");
	Interface_clanmenu.SetJoinData(oneClanName, oneChipCharid);
	Interface_clanmenu.ClanJoin();
}

//ktj : ±ÇÀ¯¹ÞÀºÀÚ°¡ ¸®ÅÏÇØÁÖ´Â °ª(ÀÌÇÔ¼ö¸¦ »ç¿ëÇØ ¸®ÅÏÇØÁØ´Ù.)
//0:Ãë¼Ò 1:OK
int JoinAnswer()
{
	return cClanJoin.nJoinAnswer;
}

void CLAN_MENU::ClanJoin()
{
	if (OpenFlag == 1) return;
	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;			//ktj : ¸¶¿ì½º°¡ Ã½Å©ÇÏ´Â ¹Ú½ºÃ½Å© 	
	C_menuN = CLAN_JOIN_before;

}

/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :±ÇÀ¯ÇÏ´Â»ç¶÷ÀÌ »ç¿ë
: ÇÏ ´Â ÀÏ : Å¬·£ÀåÀÌ Å¬·£¿øÀ» µî·Ï½ÃÅ³¶§
///////////////////////////////////////////////////////////////////////////*/

//===================================
//ktj:»ç¿ë¾ÈÇÔ.
//====================================
void joinClanMember()
{
	Interface_clanmenu.joinClanMember();
}

void CLAN_MENU::joinClanMember()
{
	if (OpenFlag == 1) return;


	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;			//ktj : ¸¶¿ì½º°¡ Ã½Å©ÇÏ´Â ¹Ú½ºÃ½Å© 	
	C_menuN = CLAN_JOIN_chip;
}

//ygy: Ä³¸¯ÅÍ »èÁ¦ ÀÓ½Ã
void CLANmenu_DeleteCha(char* id, char* gserver, char* chaname)
{
	cClanDeleteCha.SetDeleteChaInfo(id, gserver, chaname);
	Interface_clanmenu.CLANmenu_DeleteCha();

	cClanDeleteCha.bFlag = FALSE;
}


/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º¸í :
: ÇÏ ´Â ÀÏ : Ä³¸¯ÅÍ »èÁ¦½Ã
///////////////////////////////////////////////////////////////////////////*/
void ktj_IMSI_DELETE()
{
	//extern char UserAccount[256];		//id°¡ µé¾îÀÖ´Â ¹öÆÛ
	//extern char szConnServerName[16];	//Á¢¼ÓÇÑ ¼­¹öÀÌ¸§ÀÌ µé¾îÀÖ´Â ¹öÆÛ
	//CLANmenu_DeleteCha_SetCharacter(UserAccount, szConnServerName, "Çã°Æ´ö");
}



//ktj :  1. Ä³¸¯ Áö¿ì±âÀü¿¡ ¼ÂÆÃÇÏ´Â ÇÔ¼ö
void CLANmenu_DeleteCha_SetCharacter(char* id, char* gserver, char* chaname)
{
	cClanDeleteCha.SetDeleteChaInfo(id, gserver, chaname);

	//ktj : 2. Áö¿ì´Â ¸Þ´º¸¦ ¼ÂÆÃÇÑ´Ù.
	Interface_clanmenu.CLANmenu_DeleteCha();
}

//ktj : Ä³¸¯ Áö¿ì±âÀü¿¡ ºÒ¸®¿ì´Â ÇÔ¼ö
void CLAN_MENU::CLANmenu_DeleteCha()
{
	C_menuN2 = 0;
	OpenFlag = 1;
	ClanCLCharMove = 1;			//ktj : ¸¶¿ì½º°¡ Ã½Å©ÇÏ´Â ¹Ú½ºÃ½Å© 	
	C_menuN = CLAN_DELETECHA_START;

	cClanDeleteCha.SetHowDeleteChaFlag();
}

//ktj : 3. »óÅÂ ÇÃ·¡±×¸¦ º¸°í 2ÀÌ¸é Ä³¸¯À» »èÁ¦ÇÏ°í 3ÀÌ¸é »èÁ¦ ¸øÇÔ.(0,1ÀÌ¸é 2,3µÉ¶§±îÁö ±â´Ù¸°´Ù)
int CLANmenu_DeleteCha_flag()
{
	//»èÁ¦Àü : 0
	//»èÁ¦Áß : 1
	//»èÁ¦ ok : 2
	//»èÁ¦ ¾ÈµÊ : 3
	return cClanDeleteCha.GetHowDeleteChaFlag();
}



//////////////////////////////////////////////////////////////////////////////////
//Å¬·£ Á¤º¸ °®¾î¿À±â
//
/////////////////////////////////////////////////////////////////////////////////


int g_ClanCount = -1;
int ClanMark[10] = { 0,0,0,0,0,0,0,0,0,0 };

//============= µð¹ö±ë¿ë Å¬·£ÀÎÆ÷ µ¥ÀÌÅ¸¼¼ÀÌºê
int ReadClanInfo(DWORD dwClanNum)
{
	//µð¹ö±ë¿ëÀ¸·Î ±×¸®±â À§ÇÑ°Í
	g_ClanCount++;
	if (g_ClanCount > 9) g_ClanCount = 0;
	ClanMark[g_ClanCount] = dwClanNum;
	//============================

	/*
	if(OpenFlag==1) {
		return -1;		//ÀÌ ¸Þ´º°¡ ´ÝÇôÀÖÀ»¶§¸¸ °¡´ÉÇÔ.
	}
	*/

	g_ViewClanInfo.SearchClan(dwClanNum);
	return ClanInfo_nNum;		//0~99;
}


// ÇöÀç 16*16 À¸·Î µÈ ±×¸²¸¸ ·ÎµåµÇÀÖÀ¸³ª 32*32°¡ ÇÊ¿äÇÑ °æ¿ì ºÎ¸¥´Ù.
// 32*32°¡ ¸Þ¸ð¸®¸¦ ¸¹ÀÌ Â÷ÁöÇØ¼­ ÀÌ·¸°Ô µû·Î ÇÊ¿äÇÑ ±Ø¼Ò¼öÀÇ °æ¿ì¿¡¸¸ ºÎ¸¥´Ù.
int ReadClanInfo_32X32(DWORD dwClanNum)
{
	int claninfo_n = ReadClanInfo(dwClanNum);
	if (claninfo_n >= 0) {
		int clanINFO_Load32X32(int claninfo_n);
		clanINFO_Load32X32(claninfo_n);
	}
	return claninfo_n;
}







//==================================================================================
// ¿ÜºÎ¿¡¼­ »ç¿ëµÇ´Â Å¬·£ÀÇ ±¸µ¿ ¼ÂÆÃ ÇÔ¼öµé								    end
//=================================================================================







/*==================================================================================
//
// Å¬·£¸Þ´º																	   start
//
==================================================================================*/
CLAN_MENU::CLAN_MENU()
{
	nClanWon_Num = 0;
	nScl_Bar = 0;           //½ºÅ©·Ñ¹Ù
	g_nClanIconNum;			//ÇïÇÁ¾ÆÀÌÄÜ ÃÑ¼ö
	g_HelpIconNumber = 0;
	g_HelpDisPlay = 0;

}

CLAN_MENU::~CLAN_MENU()
{
}


//¸¶¿ì½ºÀ§Ä¡¸¦ Ã½Å©¾ÈÅä·ÏÇÏ´Â°Í hoLinkHeader.cpp¿¡¼­ »ç¿ë
POINT clanBoxPos;
POINT clanCLBoxPos;
void CLAN_MENU::Init()
{
	OpenFlag = 0;

	C_menuN = 0;
	C_menuN2 = 0;



	//BackStartPos.x = 70;
	//BackStartPos.y = 94;

	//BackStartPos.x = 100;
	//BackStartPos.y = 94;

	BackStartPos.x = 101;
	BackStartPos.y = 114;


	//Å¬·£¸Þ´º¿¡¼­ ¿ùµåÂÊ¿¡ ¸¶¿ì½º°¡ ¿òÁ÷ÀÓÀÌ Àü´Þ¾ÈµÇµµ·ÏÇÑ´Ù.
	clanBoxPos.x = BackStartPos.x;
	clanBoxPos.y = BackStartPos.y;

	//Å¬·£Ã¤ÆÃ ¸®½ºÆ®¸Þ´º¿¡¼­ ¿ùµåÂÊ¿¡ ¸¶¿ì½º°¡ ¿òÁ÷ÀÓÀÌ Àü´Þ¾ÈµÇµµ·ÏÇÑ´Ù.
	clanCLBoxPos.x = 540 + 60;		//60Àº ±×¸²ÀÌ ¿À¸¥Æí¿¡ ºÙ¾îÀÖ±â ¶§¹®ÀÓ
	clanCLBoxPos.y = CHATLIST_BOX_Y;




	ZeroMemory(szRemoveClanWon, sizeof(char) * 500);
	ZeroMemory(ClanHelpIcon, sizeof(CLANHELPICON) * CLANHELPICON_NUM);
	ZeroMemory(cldata.szJoinClanName, sizeof(char) * 64);   //Á¶ÀÎ Å¬·£ ÀÌ¸§
	ZeroMemory(cldata.ChaName, sizeof(char) * 64);  //Á¶ÀÎÇÏ´Â À¯Àú Ä³¸¯ÅÍ ÀÌ¸§
	ZeroMemory(cldata.szJoinClanChipName, sizeof(char) * 64);   //Á¶ÀÎÇÏ´Â Å¬·£Àå ÀÌ¸§
	ZeroMemory(szReconfReleaseMsg, sizeof(char) * 2048);


	nClanWon_Num = 0;			//Å¬·£¿øÆÄ½ÌÈÄ Å¬·£¿øÀÇ ¸í¼ö(Å¬·£ÀåÁ¦¿Ü)
	nReleaseClanWon = 0;		//»èÁ¦ Å¬·£¿ø
	//membmp = new MENUBMP;

	//hMark = NULL;
#ifndef USE_PROSTONTALE		//=========== Æ÷¸®½ºÅÏ¼Ò½ºÀÎ°æ¿ì
	int LoadClanMessage();
	LoadClanMessage();
#endif

	Load();
	cClanChatting.Load();

	//ktj : ¸Þ´º °ü·Ã Å¬¶ó½º ¼ÂÆÃ ===================

	cBaseMenu.Load();


	cIsClanMember.Init(1);
	cIsClanMember.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cIsClanMember);		//¸®¼Ò½º Ä«ÇÇ

	cClanChip.Init(1);
	cClanChip.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanChip);		//¸®¼Ò½º Ä«ÇÇ
	cBaseMenu.SetBase(&cClanChip);

	cClanUser.Init(1);
	cClanUser.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanUser);		//¸®¼Ò½º Ä«ÇÇ
	cBaseMenu.SetBase(&cClanUser);

	cClanMake.Init(1);
	cClanMake.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanMake);		//¸®¼Ò½º Ä«ÇÇ

	cClanReport.Init(1);
	cClanReport.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanReport);		//¸®¼Ò½º Ä«ÇÇ


	cClanChatting.Init(1);
	cClanChatting.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanChatting);		//¸®¼Ò½º Ä«ÇÇ
	cBaseMenu.SetBase(&cClanChatting);


	cClanDeleteCha.Init(1);
	cClanDeleteCha.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanDeleteCha);		//¸®¼Ò½º Ä«ÇÇ

	cClanJoin.Init(1);
	cClanJoin.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanJoin);		//¸®¼Ò½º Ä«ÇÇ	


	cClanNotice.Init(1);
	cClanNotice.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanNotice);		//¸®¼Ò½º Ä«ÇÇ	

	//µµ¿ò¸» ÃÊ±âÈ­
	//g_Help.Init(1);
	//g_Help.setParent(&cBaseMenu);
	//cBaseMenu.copy_LoadResource(&g_Help);		//¸®¼Ò½º Ä«ÇÇ	

	//guie ÃÊ±âÈ­
	g_GuideHelp.Init(1);
	g_GuideHelp.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&g_GuideHelp);		//¸®¼Ò½º Ä«ÇÇ	
	g_GuideHelp.InitLevelHelpLabel();

}


//notice¸Þ½ÃÁö´Â µû·Î ¼ÂÆÃµÇ¸ç Init()°ú µ¿½Ã¿¡ ›¾ÆÃÇÒ¼ö¾øÀ½.
//Å¬·£ ¼­¹ö°¡ Á×¾î ÀÖÀ» °æ¿ì ¼¼ÆÃ
void CLAN_MENU::NoticeMessage_Init()
{
	BackStartPos.x = 101;
	BackStartPos.y = 114;

	cBaseMenu.Load();

	cClanNotice.Init(1);
	cClanNotice.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&cClanNotice);		//¸®¼Ò½º Ä«ÇÇ	

	g_Help.Load();
	g_GuideHelp.Load();

	g_GuideHelp.Init(1);
	g_GuideHelp.setParent(&cBaseMenu);
	cBaseMenu.copy_LoadResource(&g_GuideHelp);		//¸®¼Ò½º Ä«ÇÇ	
	g_GuideHelp.InitLevelHelpLabel();

}


/*///////////////////////////////////////////////////////////////////////////
: ÇÔ¼ö¸í   : menuInit
: ÇÏ ´Â ÀÏ : ¸Þ´º ÃÑ°ý Init
: ¹Ý È¯ °ª :
: ÆÄ¶ó¹ÌÅÍ :
///////////////////////////////////////////////////////////////////////////*/
void CLAN_MENU::menuInit(int num)
{

	cIsClanMember.menuInit(num);		//Å¬·£¸¸µé±â ¸Þ´º initialize
	cClanMake.menuInit(num);		//Å¬·£¸¸µé±â ¸Þ´º initialize
	cClanChip.menuInit(num);		//Å¬·»¯… ¸Þ´º initialize
	cClanUser.menuInit(num);		//Å¬·£À¯Àú ¸Þ´º initialize
	cClanChatting.menuInit(num);
	cClanDeleteCha.menuInit(num);
	cClanJoin.menuInit(num);
}







/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º ¸í:
: ÇÏ ´Â ÀÏ : ¸¶¿ì½º·Î ´©¸¥ ¸Þ´º ¹øÈ£ Ã½Å©ÇÔ¼ö.
: ¹Ý È¯ °ª :
:ÆÄ¶ó¹ÌÅÍ  :
///////////////////////////////////////////////////////////////////////////*/
int CLAN_MENU::chkMenu(int x, int y, RECT* r)
{
	if (r->left <= x && x <= r->right)
		if (r->top <= y && y <= r->bottom)
			return 1;
	return 0;			//¾Æ¹«°Íµµ ¼±ÅÃ¾ÈÇÔ.
}
int CLAN_MENU::chkeckMenuN()
{

	return 1;
}





//¯…ÀÌ ±ÇÀ¯ÇÏ¸é ±ÇÀ¯¹ÞÀº »ç¶÷ÇÑÅ× ¶ß´Â ¸Þ´ºÀÓ.(ÇöÀç netplay.cpp¿¡¼­ »ç¿ëÁß)
void menu_joinOpen(char* chipCharID)
{
	//Interface_clanmenu.menu_joinOpen(chipCharID);
}


//Á¶ÀÎ¸Þ´º´Â Å¬·£¯…ÀÌ ±ÇÀ¯ÇÏ¸é ±ÇÀ¯¹ÞÀº »ç¶÷ÇÑÅ× ¸Þ´º°¡ ¶á´Ù.
int CLAN_MENU::menu_joinOpen(char* chipCharID)		//¯…ÀÇ Ä³¸¯ÅÍ id
{
	/*
	if(OpenFlag==1) return 0;					//ÀÌ ¸Þ´º°¡ ´ÝÇôÀÖÀ»¶§¸¸ °¡´ÉÇÔ.
	if( cldata.myPosition == 100 ||				//100(Å¬·£¿ø), 101:Å¬·£¯…¿¡°Ô ±ÇÀ¯ÇÒ¼ö ¾ø´Ù.
		cldata.myPosition == 101)				return 0;

	strcpy(cldata.chipCharID, chipCharID);
	C_menuN   = CLAN_JOIN_before;		//Á¶ÀÎ¿¡ ÀûÇÕÇÑÁö ¿©ºÎ.
	C_menuN2  = 0;
	OpenFlag  = 1;
	joinAnswer= 0;

	//ktj
	ClanCharMove = 1;
	*/

	return 1;

}





void menu_joinOpen_Chip(int isOK, char* clanWonID, char* clanWonCharID)
{
	Interface_clanmenu.menu_joinOpen_Chip(isOK, clanWonID, clanWonCharID, 0, 0);		//¯…ÀÇ Ä³¸¯ÅÍ id;
}
void menu_joinOpen_Chip(int isOK, char* clanWonID, char* clanWonCharID, int clanWonJob, int clanWonLevel)
{
	Interface_clanmenu.menu_joinOpen_Chip(isOK, clanWonID, clanWonCharID, clanWonJob, clanWonLevel);		//¯…ÀÇ Ä³¸¯ÅÍ id;
}

//È£µ¿ClanÃß°¡
void g_IsCheckClanMember(smCHAR* pChar)
{
	Interface_clanmenu.IsCheckClanMember(pChar);
}

int CLAN_MENU::IsCheckClanMember(smCHAR* pChar)
{
	if (OpenFlag == 1)
		return 0;

	C_menuN = CLAN_IS_CHECK_MEMBER;	//Å¬·£¿¡ °¡ÀÔÇÑÁö 4ÀÏÀÌ Áö³µ´ÂÁö °Ë»ç¸¦ ÇÑ´Ù.
	cldata.TargetChar = NULL;
	if (pChar)
		cldata.TargetChar = pChar;

	//strcpy(cldata.ChaName, pChar->smCharInfo.szName);

	C_menuN2 = 0;
	OpenFlag = 1;
	joinAnswer = 0;
	ClanCharMove = 1;

	return 0;
}



//======================================================================
//±ÇÀ¯ÇÏ´Â ¸Þ´º¸¦ ´©¸£¸é ÀÌÇÔ¼ö¸¦ ºÒ·¯ ¸®ÅÏÀÌ 1ÀÌ¸é ±ÇÀ¯ÇÏ´Â µ¥ÀÌÅ¸¸¦ º¸³»°í ¾Æ´Ï¸é ¸»°Í.
// 0 ÀÌ¸é ´õÀÌ»ó Å¬·£¿øÀ» Ãß°¡ ÇÒ¼ö ¾øÀ½(ÀÚÃ¼¸Þ´º¿¡¼­ µî·Ï½ÃÅ³¼ö¾ø´Ù°í ³ª¿Â´Ù)
BOOL CheckClanWon(int level);		//

//======================================================================


//±ÇÀ¯¹ÞÀº »ç¶÷ÀÌ OKÇÏ¸é ±×¸í·ÉÀÌ Å¬·£¯…¿¡°Ô µÇµ¹¾Æ¿Í Å¬·£¯…ÀÌ °¡ÀÔ½ÃÅ°´Â ¸Þ´ºÀÓ.
//ÁÖÀÇ : È­¸é»ó¿¡´Â ¸Þ´º°¡ ¶ßÁö¾ÊÀ½.
int CLAN_MENU::menu_joinOpen_Chip(int joinOK, char* clanWonID, char* clanWonCharID, int clanWonJob, int clanWonLevel)		//¯…ÀÇ Ä³¸¯ÅÍ id
{
	if (OpenFlag == 1) return 0;		//ÀÌ ¸Þ´º°¡ ´ÝÇôÀÖÀ»¶§¸¸ °¡´ÉÇÔ.


#ifdef __CLANSUBCHIP__
	//if(cldata.myPosition != 101 && cldata.myPosition != 104 ) return 0;	//101:Å¬·£¯…,ºÎÅ¬·£¯…¸¸ »ç¿ë°¡´ÉÇÑ ¸Þ´ºÀÓ.
	if (cldata.myPosition == 101 || cldata.myPosition == 104) goto jmp1;	//101:Å¬·£¯…,ºÎÅ¬·£¯…¸¸ »ç¿ë°¡´ÉÇÑ ¸Þ´ºÀÓ.
	return 0;
jmp1:
#else
	if (cldata.myPosition != 101) return 0;	//101:Å¬·£¯…¸¸ »ç¿ë°¡´ÉÇÑ ¸Þ´ºÀÓ.
#endif




#ifdef USE_PROSTONTALE		//=========== ÇÁ¸®½ºÅÏ¼Ò½ºÀÎ°æ¿ì
	int chk = CheckClanWon(sinChar->Level);
	if (chk == 0) {			//Å¬·£¿øÀ» Ãß°¡ÇÒ¼ö ¾ø½À´Ï´Ù.
		return 0;
	}
#endif

	/*
	if(joinOK==0) {
		C_menuN= CLAN_JOIN_chip_NO	;		//Å¬·£¿¡ °¡ÀÔÇÏ±â¸¦ °ÅºÎÇÔ.
		C_menuN2= -1;
	}
	else {
		*/
	C_menuN = CLAN_JOIN_chip;	//Å¬·£¿¡ µî·Ï½ÃÅ²´Ù.
	strcpy(cldata.clanJoinID, clanWonID);
	strcpy(cldata.clanJoinCharID, clanWonCharID);
	cldata.clanJoinJob = clanWonJob;
	cldata.clanJoinLevel = clanWonLevel;


	C_menuN2 = 0;
	//}
	OpenFlag = 1;
	joinAnswer = 0;

	//ktj
	ClanCharMove = 1;


	return 1;
}








//int chkN11 = -1;
//int chkN22 = -1;

extern BOOL bip_port_error;
int CLAN_MENU::Main(int sw) { return TRUE; }




char loadis[256] = { 0, };
char loadis2[256] = { 0, };
char updateis[256] = { 0, };





//ÀÌ°Ô 1ÀÌ¸é ÀÎÆ÷¸¦ µå·ÎÀÎÇÑ´Ù.
int ktj_imsiDRAWinfo = 0;
//========================================================================³ªÁß¿¡ Áö¿ö¾ßÇÔ¿¡ ÁÖÀÇ

int CLAN_MENU::Draw() { return TRUE; }


//sw==0 : ÀÏ¹Ý sw==1: Å°ÀÔ·ÂÀ» ¹Þ´Â°æ¿ì sw==2: Å¬·£ÀÌ¸§À» Å¸ÀÌÆ²¿¡ ¾µ°æ¿ì»ç¿ë(±×¸²ÀÚÃ³¸®)
void CLAN_MENU::Draw_C_Text(char* str, int x, int y, int sw = 0) {}






//ycy : ¹Ú½º ±×¸®±â
int g_nTotalWidth = 0;
int CLAN_MENU::DrawBox(int x, int y, int x_width, int y_height) { return TRUE; }

void CLAN_MENU::RenderMenu() {}
/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º ¸í: class CLAN_MENU
: ÇÏ ´Â ÀÏ :
: ¼­    ½Ä :
: ÀÎ    ÀÚ :
: ¹Ý È¯ °ª :
: ³í    Æò : À¥db¸¦ »ç¿ëÇØ µ¥ÀÌÅ¸¸¦ »Ì¾Æ³»¾ß ÇÒ°æ¿ì º» Å¬¶ó½º¸¦ ÀÌ¿ëÇÑ´Ù.
: ¹ö    Àü :
: PROJECT  :
///////////////////////////////////////////////////////////////////////////*/
void CLAN_MENU::Draw_C_menuN() {}


//=============µå·Î¿ì °ü·Ã ÇÔ¼ö ==================================================================end







/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º ¸í: °èÁ¤¿¡ ¸¸µé¾îÁø Ä³¸¯ÅÍµéÀÌ °¡ÀÔÇÑ Å¬·£À» ¾Ë±â À§ÇØ¼­
: ÇÏ ´Â ÀÏ :
: ¹Ý È¯ °ª :
///////////////////////////////////////////////////////////////////////////*/
//À¯Àú id, Ä³¸¯ÅÍ id, ±º¼­¹ö ÀÌ¸§.
void SetUserData2(char* id, char* chaname, char* gserver)
{
	strcpy(cldata.szID, id);
	strcpy(cldata.ChaName, chaname);
	strcpy(cldata.gserver, gserver);

}

/*///////////////////////////////////////////////////////////////////////////
: Å¬¶ó½º ¸í: Å¬·£ ½ºÆ®·°Ã³ µ¥ÀÌÅ¸ ¼ÂÆÃ ÇÔ¼öµé
: ÇÏ ´Â ÀÏ :
: ¹Ý È¯ °ª :
///////////////////////////////////////////////////////////////////////////*/
//À¯Àú id, Ä³¸¯ÅÍ id, ±º¼­¹ö ÀÌ¸§.
void CLAN_MENU::SetUserData(char* id, char* chaname, char* gserver, int money, int ability)
{
	strcpy(cldata.szID, id);
	strcpy(cldata.ChaName, chaname);
	strcpy(cldata.gserver, gserver);

	cldata.money = money;
	//ktj : ÀÓ½Ã
	//if(cldata.money ==0) cldata.money = 100000;
	cldata.ability = ability;
}



//±ÇÀ¯¸¦ ¹Þ´Â »ç¶÷ÇÑÅ×¿À´Â µ¥ÀÌÅ¸
void CLAN_MENU::SetJoinData(char* joinclanname, char* joinclanchip)
{
	strcpy(cldata.szJoinClanName, joinclanname);			//°¡ÀÔµÉ Å¬·£ ÀÌ¸§
	strcpy(cldata.szJoinClanChipName, joinclanchip);		//°¡ÀÔµÉ Å¬·£ÀÇ Å¬·£Àå id
}











#ifdef USE_PROSTONTALE		//=========== Æ÷¸®½ºÅÏ¼Ò½ºÀÎ°æ¿ì

void DrawSprite_TJB(int winX, int winY, int winxlen, int winylen, void* pdds, int x, int y, int width, int height, int Bltflag) {}
#endif



void Text_Out1(HDC hdc, int x, int y, char* str)
{
}

void Text_Out2(HDC hdc, int x, int y, char* str)
{
}

void Text_Out3(HDC hdc, int x, int y, char* str)
{
}
void Text_Out(HDC hdc, int x, int y, char* str) {}



void CLAN_MENU::Load() {}



//simMain.cpp¿¡¼­ ºÒ·¯¼­ Å¬·ÎÁî ÇÑ´Ù.
void clanMENU_close()
{
	Interface_clanmenu.Close();
}

void CLAN_MENU::Close() {}

void CLAN_MENU::Loading() {}


#ifdef USE_PROSTONTALE		//=========== Æ÷¸®½ºÅÏ¼Ò½ºÀÎ°æ¿ì
//Å¬·£¿ø Á¤º¸ Àü¼Û¿ë ÇÔ¼ö
int SendClanCommandUser(smWINSOCK* lpsmSock, DWORD	dwCode, int Param1, int Param2, CLANWONLIST* lpClanUserList, int ClanUserCount);
#endif


void SetClanWon()
{
}





//savedata\\clanDATA ¸¸µé±â==============================================start
BOOL SearchDirectory(char* dir)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	char t[50];
	wsprintf(t, "%s\\*.*", dir);
	hFind = FindFirstFile(t, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	return TRUE;
}

void MakeDirectory(char* dir)
{
	CreateDirectory(dir, NULL);
}


void CreateSaveData_dir()
{
	char szTemp[100];
	if (!SearchDirectory(SAVEDATA_DIR)) MakeDirectory(SAVEDATA_DIR);
	if (!SearchDirectory(CLANDATA_DIR)) MakeDirectory(CLANDATA_DIR);

	ZeroMemory(szTemp, sizeof(szTemp));
	wsprintf(szTemp, "%s", CLANDATA_DIR);
	wsprintf(szTemp, "%s\\%s", szTemp, szConnServerName);

	if (!SearchDirectory(szTemp)) MakeDirectory(szTemp);

}



//savedata\\clanDATA ¸¸µé±â==============================================end

//npcwav µð·ºÅä¸® ¸¸µé±â=========================start

/*void CreateNPCWav_dir()
{
	if(!SearchDirectory(NPCWAV_DIR))
	{
		MakeDirectory(NPCWAV_DIR);
	}
}
*/


//Å¬·£ ¸Þ¼¼Áö Ãâ·ÂÇÏ±â ==========================================================================start
#define CLANMESSAGE_DIR "image/clanImage/Help/clanText.msg"//msg"

int LoadClanMessage()
{
	char* g_ClanMessage;


	HANDLE hFile = NULL;
	DWORD dwFileSize;
	DWORD dwRead = 0;
	char c;
	c = '\n';
	hFile = CreateFile(CLANMESSAGE_DIR, GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == NULL) return -1;

	dwFileSize = GetFileSize(hFile, NULL);

	g_ClanMessage = new char[dwFileSize];

	ReadFile(hFile, g_ClanMessage, dwFileSize, &dwRead, NULL);
	CloseHandle(hFile);

	int ReadMessage(char* save, char* savename, char* readbuf);
	//void CheckMsgPos(CLANMSG* clanmsg,int width, int height);
	ReadMessage(clanMake1.szMsg, "szMsg1", g_ClanMessage);
	CheckMsgPos(&clanMake1, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake2.szMsg, "szMsg2", g_ClanMessage);
	CheckMsgPos(&clanMake2, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake4.szMsg, "szMsg4", g_ClanMessage);
	CheckMsgPos(&clanMake4, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake5.szMsg, "szMsg5", g_ClanMessage);
	CheckMsgPos(&clanMake5, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMake6.szMsg, "szClanMakeFinishMsg", g_ClanMessage);
	CheckMsgPos(&clanMake6, BOX_MAX_WIDTH, 0);


	ReadMessage(szMsg3, "szMsg3", g_ClanMessage);

	ReadMessage(clanMsg21.szMsg, "szMsg21", g_ClanMessage);
	CheckMsgPos(&clanMsg21, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMsg254.szMsg, "szMsg254", g_ClanMessage);  //CLAN_NotMake
	CheckMsgPos(&clanMsg254, BOX_MAX_WIDTH, 0);
	ReadMessage(clanMsg15.szMsg, "szMsg15", g_ClanMessage);
	CheckMsgPos(&clanMsg15, BOX_MAX_WIDTH, 0);	  //CLAN_RELEASE
	ReadMessage(clanJoin.szMsg, "szJoin", g_ClanMessage); //CLAN_JOIN
	CheckMsgPos(&clanJoin, BOX_MAX_WIDTH, 0);
	ReadMessage(clanSecession.szMsg, "szSecession", g_ClanMessage); //CLAN_SECESSION
	CheckMsgPos(&clanSecession, BOX_MAX_WIDTH, 0);
	ReadMessage(clanServerConnectErrorMsg.szMsg, "szServerConnectErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanServerConnectErrorMsg, BOX_MAX_WIDTH, 0);            //ISAO_ERR_TCP_CONNECT
	ReadMessage(clanszError_Msg.szMsg, "szError_Msg", g_ClanMessage); //ISAO_ERR_TCP_CONNECT
	CheckMsgPos(&clanszError_Msg, BOX_MAX_WIDTH, 0);
	ReadMessage(clanCancelMakeClan.szMsg, "szCancelMakeClan", g_ClanMessage);
	CheckMsgPos(&clanCancelMakeClan, BOX_MAX_WIDTH, 0);
	ReadMessage(clanClanNameError.szMsg, "szClanNameError", g_ClanMessage);
	CheckMsgPos(&clanClanNameError, BOX_MAX_WIDTH, 0);


	//ReadMessage(szClanMakeFinishMsg,"szClanMakeFinishMsg",g_ClanMessage);
	//ReadMessage(szMsg254,"szMsg254",g_ClanMessage);
	//ReadMessage(szMsg15,"szMsg15",g_ClanMessage);
	//ReadMessage(szMark1,"szMark1",g_ClanMessage);
	//ReadMessage(szMark2,"szMark2",g_ClanMessage);
	//ReadMessage(szMark3,"szMark3",g_ClanMessage);
	//ReadMessage(szJoin,"szJoin",g_ClanMessage);
	//ReadMessage(szSecession,"szSecession",g_ClanMessage);

	//ReadMessage(szMsg6,"szMsg6",g_ClanMessage);
	//ReadMessage(szServerConnectErrorMsg,"szServerConnectErrorMsg",g_ClanMessage);
	//ReadMessage(szReadClanMarkErrorMsg,"szReadClanMarkErrorMsg",g_ClanMessage);
	//ReadMessage(szCancelMakeClan,"szCancelMakeClan",g_ClanMessage);
	//ReadMessage(szTranzationError,"szTranzationError",g_ClanMessage);
	//ReadMessage(szInsertClanWonError1,"szInsertClanWonError1",g_ClanMessage);
	//ReadMessage(szClanNameError,"szClanNameError",g_ClanMessage);
	//ReadMessage(sz100Error,"sz100Error",g_ClanMessage);
	//ReadMessage(szDefaultError,"szDefaultError",g_ClanMessage);
	//ReadMessage(szReleseError,"szReleseError",g_ClanMessage);
	//ReadMessage(szSecessionSuccess,"szSecessionSuccess",g_ClanMessage);
	//ReadMessage(szSecession6DownMsg,"szSecession6DownMsg",g_ClanMessage);clanSecession6DownMsg
	ReadMessage(clanSecession6DownMsg.szMsg, "szSecession6DownMsg", g_ClanMessage);  //CLAN_DELETECHA_SECESSION
	CheckMsgPos(&clanSecession6DownMsg, BOX_MAX_WIDTH, 0);
	//ReadMessage(szSecessionChipMsg,"szSecessionChipMsg",g_ClanMessage);
	//ReadMessage(szSecession2ErrorMsg,"szSecession2ErrorMsg",g_ClanMessage);
	//ReadMessage(szClanJoinMsg,"szClanJoinMsg",g_ClanMessage);
	ReadMessage(clanClanJoinMsg.szMsg, "szClanJoinMsg", g_ClanMessage);
	CheckMsgPos(&clanClanJoinMsg, BOX_MAX_WIDTH, 0);

	//ReadMessage(szClanJoinError2Msg,"szClanJoinError2Msg",g_ClanMessage);
	//ReadMessage(szClanJoinError3Msg,"szClanJoinError3Msg",g_ClanMessage);
	//ReadMessage(szClanJoinError4Msg,"szClanJoinError4Msg",g_ClanMessage);
	//ReadMessage(szClanJoinCancelMsg,"szClanJoinCancelMsg",g_ClanMessage);CLANMSG clanClanJoinCancelMsg;
	ReadMessage(clanClanJoinCancelMsg.szMsg, "szClanJoinCancelMsg", g_ClanMessage);
	CheckMsgPos(&clanClanJoinCancelMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanJoinsucesssMsg.szMsg, "szClanJoinsucesssMsg", g_ClanMessage);
	CheckMsgPos(&clanClanJoinsucesssMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanszReMoveClanWonMsg.szMsg, "szReMoveClanWonMsg", g_ClanMessage);
	CheckMsgPos(&clanszReMoveClanWonMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanReMoveHeaderMsg.szMsg, "szReMoveHeaderMsg", g_ClanMessage);
	CheckMsgPos(&clanReMoveHeaderMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanszReConfReleaseMsg.szMsg, "szReConfReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanszReConfReleaseMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanNoRecordReleaseMsg.szMsg, "szNoRecordReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanNoRecordReleaseMsg, BOX_MAX_WIDTH, 0);

	//ReadMessage(szFinishReleaseMsg,"szFinishReleaseMsg",g_ClanMessage);clanFinishReleaseMsg
	ReadMessage(clanFinishReleaseMsg.szMsg, "szFinishReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanFinishReleaseMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanSecessionMsg.szMsg, "szSecessionMsg", g_ClanMessage);
	CheckMsgPos(&clanSecessionMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanFinishSecessionMsg.szMsg, "szFinishSecessionMsg", g_ClanMessage);
	CheckMsgPos(&clanFinishSecessionMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanisCheckClanJangErrorMsg.szMsg, "szisCheckClanJangErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanisCheckClanJangErrorMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMoneyErrorMsg.szMsg, "szMoneyErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanMoneyErrorMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanAbilityErrorMsg.szMsg, "szAbilityErrorMsg", g_ClanMessage);
	CheckMsgPos(&clanAbilityErrorMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanszRemoveShortClanWonMsg.szMsg, "szRemoveShortClanWonMsg", g_ClanMessage);
	CheckMsgPos(&clanszRemoveShortClanWonMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanRemoveShowrtClanWonResultMsg.szMsg, "szRemoveShowrtClanWonResultMsg", g_ClanMessage);
	CheckMsgPos(&clanRemoveShowrtClanWonResultMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanNoName.szMsg, "szNoName", g_ClanMessage);
	CheckMsgPos(&clanNoName, BOX_MAX_WIDTH, 0);

	ReadMessage(clanLoading.szMsg, "szLoading", g_ClanMessage);
	CheckMsgPos(&clanLoading, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMsg6.szMsg, "szMsg6", g_ClanMessage);
	CheckMsgPos(&clanMsg6, BOX_MAX_WIDTH, 0);



	/////////Ä³¸¯ÅÍ »èÁ¦ ¸Þ¼¼Áö
	ReadMessage(szDeleteChaReleaseBefore, "szDeleteChaReleaseBefore", g_ClanMessage);
	ReadMessage(szDeleteChaReleaseFinish, "szDeleteChaReleaseFinish", g_ClanMessage);
	ReadMessage(szDeleteChaSecessionBefore, "szDeleteChaSecessionBefore", g_ClanMessage);
	ReadMessage(szDeleteChaSecessionFinish, "szDeleteChaSecessionFinish", g_ClanMessage);
	//ReadMessage(szDeleteChaError,"szDeleteChaError",g_ClanMessage);

	ReadMessage(clanDeleteChaError.szMsg, "szDeleteChaError", g_ClanMessage);
	CheckMsgPos(&clanDeleteChaError, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMakeFinishClan.szMsg, "szMakeFinishClan", g_ClanMessage);
	CheckMsgPos(&clanMakeFinishClan, BOX_MAX_WIDTH, 0);


	ReadMessage(szClanWon6Down, "szClanWon6Down", g_ClanMessage);

	ReadMessage(clanClanUserNoMakeMsg.szMsg, "szClanUserNoMakeMsg", g_ClanMessage);
	CheckMsgPos(&clanClanUserNoMakeMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanMakeFinishClanWon.szMsg, "szMakeFinishClanWon", g_ClanMessage);
	CheckMsgPos(&clanMakeFinishClanWon, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanServerInspect.szMsg, "ClanServerInspect", g_ClanMessage);
	CheckMsgPos(&clanClanServerInspect, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanUserRelease.szMsg, "szClanUserRelease", g_ClanMessage);
	CheckMsgPos(&clanClanUserRelease, BOX_MAX_WIDTH, 0);


	//ReadMessage(Replay,"Replay",g_ClanMessage);

	ReadMessage(clanClanMakedowninfo.szMsg, "szClanMakedowninfo", g_ClanMessage);
	CheckMsgPos(&clanClanMakedowninfo, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanUserReleaseMsg.szMsg, "szClanUserReleaseMsg", g_ClanMessage);
	CheckMsgPos(&clanClanUserReleaseMsg, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanUserRemove.szMsg, "szClanUserRemove", g_ClanMessage);
	CheckMsgPos(&clanClanUserRemove, BOX_MAX_WIDTH, 0);

	ReadMessage(clanClanWonOver.szMsg, "szClanWonOver", g_ClanMessage);
	CheckMsgPos(&clanClanWonOver, BOX_MAX_WIDTH, 0);

	//ReadMessage(szError_Msg,"szError_Msg",g_ClanMessage);
	ReadMessage(clanMsg7.szMsg, "szMsg7", g_ClanMessage);
	CheckMsgPos(&clanMsg7, BOX_MAX_WIDTH, 0);

	ReadMessage(clanNoLevel.szMsg, "szNoLevel", g_ClanMessage);
	CheckMsgPos(&clanNoLevel, BOX_MAX_WIDTH, 0);

	ReadMessage(clanRemoveSecond.szMsg, "szRemoveSecond", g_ClanMessage);
	CheckMsgPos(&clanRemoveSecond, BOX_MAX_WIDTH, 0);

	ReadMessage(clanRemoveSecondResult.szMsg, "szRemoveSecondResult", g_ClanMessage);
	CheckMsgPos(&clanRemoveSecondResult, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanConfirm.szMsg, "szLeaveClanConfirm", g_ClanMessage);
	CheckMsgPos(&LeaveClanConfirm, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanNoMoney.szMsg, "szLeaveClanNoMoney", g_ClanMessage);
	CheckMsgPos(&LeaveClanNoMoney, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanSuccess.szMsg, "szLeaveClanSuccess", g_ClanMessage);
	CheckMsgPos(&LeaveClanSuccess, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanNoLevel.szMsg, "szLeaveClanNoLevel", g_ClanMessage);
	CheckMsgPos(&LeaveClanNoLevel, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanReConfirm.szMsg, "szLeaveClanReConfirm", g_ClanMessage);
	CheckMsgPos(&LeaveClanReConfirm, BOX_MAX_WIDTH, 0);

	ReadMessage(NoLeaveClan.szMsg, "szNoLeaveClan", g_ClanMessage);
	CheckMsgPos(&NoLeaveClan, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanMessage.szMsg, "szLeaveClanMessage", g_ClanMessage);
	CheckMsgPos(&LeaveClanMessage, BOX_MAX_WIDTH, 0);

	ReadMessage(LeaveClanDownLevel.szMsg, "szLeaveClanDownLevel", g_ClanMessage);
	CheckMsgPos(&LeaveClanDownLevel, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipAppoint.szMsg, "szSubChipAppoint", g_ClanMessage);
	CheckMsgPos(&SubChipAppoint, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipAppointEnd.szMsg, "szSubChipAppointEnd", g_ClanMessage);
	CheckMsgPos(&SubChipAppointEnd, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipRelease.szMsg, "szSubChipRelease", g_ClanMessage);
	CheckMsgPos(&SubChipRelease, BOX_MAX_WIDTH, 0);

	ReadMessage(SubChipReleaseEnd.szMsg, "szSubChipReleseEnd", g_ClanMessage);
	CheckMsgPos(&SubChipReleaseEnd, BOX_MAX_WIDTH, 0);

	ReadMessage(NoSubChip.szMsg, "szNoSubChip", g_ClanMessage);
	CheckMsgPos(&NoSubChip, BOX_MAX_WIDTH, 0);


	//È£µ¿ClanÃß°¡
	ReadMessage(ClanIs4DayCheckMsg.szMsg, "szClanIs4DayCheckMsg", g_ClanMessage);
	CheckMsgPos(&ClanIs4DayCheckMsg, BOX_MAX_WIDTH, 0);






	delete[] g_ClanMessage;
	g_ClanMessage = NULL;
	return 1;
}

int ReadMessage(char* save, char* savename, char* readbuf)
{
	char szTemp[256];
	char* pBuf;
	char szReadData[1024];
	int nCount = 0;
	int nCount1 = 0;
	BOOL bFlag = FALSE;

	ZeroMemory(szReadData, sizeof(szReadData));
	wsprintf(szTemp, "*%s", savename);

	pBuf = strstr(readbuf, szTemp);
	if (pBuf == NULL)	//¹ß°ßÇÒ ¼ö ¾ø¾ú´Ù.
	{
		return -1;
	}

	pBuf = pBuf + strlen(szTemp) + 2;
	nCount = (int)(pBuf - readbuf);

	if (readbuf[nCount] == 0x0a) {
		nCount++;
	}

	//DRZ_EDIT (prevent buffer overrun)
	while (nCount < (int)strlen(szReadData))
	{
		if ((readbuf[nCount] == 0x5c))
		{
			szReadData[nCount1] = '\n';
			nCount1++;
			nCount += 3;
			if (readbuf[nCount] == 0x0a)
			{
				nCount++;
				continue;
			}

		}
		if ((readbuf[nCount] == 0x0d) || (readbuf[nCount] == 0x0a)) break;
		szReadData[nCount1] = readbuf[nCount];
		nCount++;
		nCount1++;

	}

	strcpy(save, szReadData);
	return 1;
}



//xsu´Â ¿©·¯°³ÀÇ xÃàÁß ÃÖ´ë±æÀÌÀÓ.
void MsgXsu_Ysu(CLANMSG* clanmsg, int* xsu, int* ysu)
{
	char strbuf[256], strbuf2[256];
	ZeroMemory(strbuf, sizeof(strbuf));

	int nCount = 0;
	int maxXsu = 0;
	int cntX = 0, cntY = 0;

	while (1)
	{
		strbuf[cntX] = clanmsg->szMsg[nCount];

		if (clanmsg->szMsg[nCount] == '\n')			//ÇÑÁÙÀÇ ³¡
		{
			strbuf[cntX] = 0;
			if (cntX > maxXsu) {
				maxXsu = cntX;
				strcpy(strbuf2, strbuf);
			}

			cntY++;
			cntX = -1;
		}

		if (clanmsg->szMsg[nCount] == 0x00)//0x0d)			//³¡
		{
			strbuf[cntX] = 0;
			if (cntX > maxXsu) {
				maxXsu = cntX;
				strcpy(strbuf2, strbuf);
			}
			cntY++;
			break;
		}


		cntX++;
		nCount += 1;
	}

	*xsu = maxXsu;
	*ysu = cntY;
}



void CheckMsgPos(CLANMSG* clanmsg, int width, int height)
{
	int nCount = 0;
	int nXMax = 0, nYMax;
	int nX = 0;
	//nX = (((4 * BOXCENTER_WIDTH) + (2*BOXCORNER_WIDTH) + 5) ); //261ÀÌ ¹Ú½ºÀÇ x±æÀÌ
	nX = width; //261ÀÌ ¹Ú½ºÀÇ x±æÀÌ
	nX = nX / 2;
	int nY = 40;

	MsgXsu_Ysu(clanmsg, &nXMax, &nYMax);
	int strDotlen = nXMax * 6;     //½ºÆ®¸µ ÇÑ¹ÙÀÌÆ®ÀÇ µµÆ®¼ö´Â 6dot
	strDotlen /= 2;

	if (nYMax == 1) nY = 80;
	if (nYMax == 2) nY = 60;

	clanmsg->pos.x = (nX - strDotlen);
	clanmsg->pos.y = nY;
}



int g_nCount = 0;
void DebugMessage(char* msg, ...)
{
#define DEBUG_MSG_MAX 4096
	HANDLE hFile;
	char buff[DEBUG_MSG_MAX];
	char szDebugMsg[5120];
	DWORD dwWrite = 0;
	int nMsgStrOver = 0;
	SYSTEMTIME	SystemTime;


	va_list args;

	va_start(args, msg);
	vsprintf(buff, msg, args);
	va_end(args);



	//strcat( buff, "\r\n" );

	GetLocalTime(&SystemTime);

	hFile = CreateFile("ClanDebug.txt", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	wsprintf(szDebugMsg, "%d.%d³â%d¿ù%dÀÏ : %s\r\n", g_nCount++,
		SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay, buff);


	if (hFile != INVALID_HANDLE_VALUE) {
		SetFilePointer(hFile, 0, NULL, FILE_END);

		WriteFile(hFile, szDebugMsg, lstrlen(szDebugMsg), &dwWrite, NULL);
		if (lstrlen(buff) > DEBUG_MSG_MAX - 2)
		{
			SetFilePointer(hFile, 0, NULL, FILE_END);
			char szMsg[256] = "..\n¸Þ½ÃÁö°¡ ³Ê¹«±æ¾î¼­ Àß¶úÀ½";
			WriteFile(hFile, szMsg, lstrlen(szMsg), &dwWrite, NULL);
		}



	}


	if (hFile) CloseHandle(hFile);
}

//#define DEBUGOPTION  "image\\clanimage\\help\\clan.inf"
#define DEBUGOPTION  "DDD2.txt"
BOOL	g_bDebugOption = FALSE;
void ReadWEBDebugOption()
{
	HANDLE hFile = NULL;
	DWORD dwRead;
	char DebugOption;
	hFile = CreateFile(DEBUGOPTION, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		ReadFile(hFile, &DebugOption, 1, &dwRead, NULL);
		CloseHandle(hFile);

		DebugOption = '1';
	}
	else {
		//ÆÄÀÏÀÌ ¾øÀ¸¸é µð¹ö±ë ¾ÈÇÑ´Ù.
		//hFile = CreateFile( DEBUGOPTION, GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
		//					NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
		//WriteFile(hFile,&DebugOption,1,&dwRead,NULL);
		//CloseHandle(hFile);
		DebugOption = '0';
	}
	if (DebugOption == '1') g_bDebugOption = TRUE;
	else g_bDebugOption = FALSE;
}

