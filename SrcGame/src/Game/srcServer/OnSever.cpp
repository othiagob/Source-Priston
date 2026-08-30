
#define _WIN32_WINNT    0x0500  
#define _WIN32_WINDOWS  0x0500

#include "..\globals.h"

#include <windows.h>
#include <io.h>
#include <stdio.h>         
#include <string.h>        
#include <memory.h>
#include <process.h>       



#include "utils/Logs/utils.h"

#include "..\\resource.h"
#include "..\\smwsock.h"



#include "..\\smlib3d\\smd3d.h"
#include "..\\character.h"
#include "..\\fileread.h"
#include "..\\playmain.h"
#include "..\\field.h"
#include "..\\sinbaram\\sinlinkheader.h"
#include "..\\effectsnd.h"

#include "..\\record.h"
#include "..\playsub.h"




#include "..\\sinbaram\\sinitem.h"

#include "ClientFuncPos.h"

#include "OnServer.h"



#include "..\\damage.h"














int AgingEvento = 0;
char* szNpcInfoPath = "Game\\NPCs\\";


CRITICAL_SECTION	cSerSection;
static CRITICAL_SECTION	cOdbcSection;

char	szServer_DebugString[256];
DWORD	dwServer_DebugMessage;
char	szServer_DebugLastUserID[32];
char	szServer_DebugLastUserName[32];
int		Server_DebugCount;

int		rsNSpriteMode = 0;

int		rsOpenNPC_RandomPos = 0;

HWND	hDebugWnd = 0;
#define	SWM_DISPDEBUG	(WM_USER+77)


int	ServerMode = 0;
int	Server_SqlReady = 0;
int	ServerNameCode = 0;

static HINSTANCE hInst;
extern HWND hwnd;
static HWND hMsgTextWnd;
static HDC hdc = {};
static int Quit = 0;
static char* szAppName = "호떡 온라인 게임서버";
static int WinWidth = 320;
static int WinHeight = 240;

static int WSA_RecvMsgCount = 0;

static	int AccountPerl = 100;
static	int	ShutDownFlag = 0;
static	int	ShutDownLeftTime = 0;





char szMessageBuff[256];
DWORD ListBuff[smSOCKBUFF_SIZE / 4];
int PlayerCount;


#define DIST_AROUND_NEAR	(256*256)
#define DIST_AROUND_MIDDLE	(16*64 * 16*64)

#define CHATBUFF_MAX		1024
#define CHATBUFF_MASK		1023

#define TRANS_IPMAX			(1000/sizeof(TRANS_IPLIST))


#define AREA_USERMAX		256
#define AREA_MAX			256

#define AUTOPLAYER_MAX		2048
#define AUTOPLAYER_MASK		2047


int rsConnectUserLimit = 800;


#define	TRANS_MAX_CHAR_LIMIT	28
#define	TRANS_MAX_USER_LIMIT	18

#define	OVER_TRANS_TIME		1500

#define STAGE_MAX			10
smSTAGE3D* lpsmStage[STAGE_MAX];

DWORD		dwPlayServTime = 0;
DWORD		dwOffsetTime = 0;

#define	CHR_MONSTER_MAX		400
#define	DEF_ITEM_MAX		1000 


#define	REOPEN_COUNT_NUM	256


#define TRANS_VIEW_LIMIT		(64*64)

#define OPEN_START_MONSTER_MAX		3

#define	STG_ITEM_WAIT_TIME		(1000*60*3)
#define	STG_ITEM_WAIT_TIME_LOW	(1000*90)


#define ITEM_SET_DIST	24

int	LoginServer = 0;
int	AutoPlayServer = 0;
int	UserPlayServer = 0;

int chrMonsterCnt;
int	EventMonster = 0;


#define	LIMIT_PK_LEVEL	20				


int	rsHardCoreEvent = 0;
int	rsDamgeOffEvent = 0;



int	rsDamgeOffEvent1 = 0;
int	rsDamgeOffEvent2 = 0;


int rsDefenseOffEvent = 0;

int	rsCristalEvent = 0;
DWORD	dwBabelPlayTime = 0;
int		BabelPlayField = -1;


static int	ChatBuffCnt;
static char TransTemp[1024];
static char	szChkID[32];


rsPLAYINFO* rsPlayInfo;

smCHAR_INFO* chrNpcMonsterInfo = 0;



int ActivePlayCounter;
int	srAutoPlayCount = 0;

smCHAR* lpActiveChar;

time_t		tServerTime = 0;

char* rsRecordDataBuff = 0;


static int			spJobCode = 0;
static int			spItemNormal = 0;
static DWORD		spJobCodeTime = 0;

#define	rsBLESSCASTLE_TAX_TIME			4


rsBLESS_CASTLE		rsBlessCastle;


DWORD	GetGameTimeHour()
{
	DWORD	dwTime;

	dwTime = GetCurrentTime() - dwOffsetTime;
	dwTime = (dwTime / GAME_WORLDTIME_MIN) / 60;
	dwTime = dwTime - (((int)(dwTime / 24)) * 24);

	return dwTime;
}






POINT3D	rsHardCoreEventOpenPos[7][3] = {
	{
		{ 18967,  1154,	3055 },
		{ 19995,  1154,	2134 },
		{ 19485,  1154,	2615 }
	},
	{
		{ 20838,  1154,	3024 },
		{ 21671,  1154,	2193 },
		{ 21255,  1154,	2615 }
	},
	{
		{ 22478,  1154,	3049 },
		{ 23437,  1154,	2135 },
		{ 22948,  1154,	2615 }
	},
	{
		{ 24204,  1154,	3094 },
		{ 25173,  1154,	2135 },
		{ 24674,  1154,	2615 }
	},

	{
		{ 20509,  1621,	5284 },
		{ 21544,  1621,	4261 },
		{ 21012,  1621,	4773 }
	},
	{
		{ 22575,  1621,	5253 },
		{ 23610,  1621,	4230 },
		{ 23077,  1621,	4742 }
	},

	{
		{ 21538,  1638,	7057 },
		{ 22527,  1638,	6079 },
		{ 22018,  1638,	6569 }
	}
};


int	rsSod_RoundTable[8][2] = {
	{	0,	4	},
	{	0,	4	},
	{	0,	4	},
	{	4,	6	},
	{	4,	6	},
	{	4,	6	},
	{	6,	7	},
	{	6,	7	}
};



int rsHardCoreEventArea[4] = { 18516 , 25715, 1541 , 7178 };







int	rsHardCoreEventInterLineZ = -32 * fONE;
int	rsHardCoreEventInterLineZ2 = -64 * fONE;
int	rsHardCoreEventPosY = (950 - 32) * fONE;
int	rsHardCoreEventOutLineZ = -600 * fONE;
int	rsHardCoreEvent_TicketHour = 4;

DWORD	dwHardCoreEvent_OutTime;

int	rsEventTicketCount;
int	rsEventTicketLevel[4];

int	rsHardCoreEventMode = 0;
int	rsHardCoreEventTimeOver = 0;

#define	rsEVENT_TICKET_LEVEL_TOTAL		549


struct	sSOD_USER {
	rsPLAYINFO* lpPlayInfo;
	DWORD		dwConnCode;
	DWORD		Temp[4];
};

extern void LeIniStr(char* Section, char* Key, char* szFileIni, char* Var1);

#define	rsSOD_USER_MAX	120
#define	rsSOD_UPDATE_TIME	6000

sSOD_USER	rsSodUser[rsSOD_USER_MAX];
int	rsSodUserCount;
DWORD	rsTransSodTime = 0;
DWORD	dwSoD_DelayTime = 0;

int	rsSodTeamScore[4];
int	rsSodTeamRank[4];




int	rsInitHardCoreEvent()
{
	return TRUE;
}
int rsHardCoreEvent_OpenMonster(STG_AREA* lpStgArea, STG_CHAR_INFO* lpStgCharInfo)
{
	return TRUE;
}

int rsShowHardCoreEventScore(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}



int rsCheckHardCoreEventArea(int x, int y, int z)
{
	int mx, my, mz;

	mx = x >> FLOATNS;
	my = y >> FLOATNS;
	mz = z >> FLOATNS;

	if (y >= rsHardCoreEventOpenPos[0][0].y - 32) {

		if (rsHardCoreEventArea[0]<mx && rsHardCoreEventArea[1]>mx &&
			rsHardCoreEventArea[2]<mz && rsHardCoreEventArea[3]>mz) {


			return TRUE;
		}
	}

	return FALSE;
}


int rsGetSoD_AreaCode(int x, int y, int z)
{
	int mx, my, mz;
	int cnt;


	mx = x >> FLOATNS;
	my = y >> FLOATNS;
	mz = z >> FLOATNS;

	if (y >= rsHardCoreEventOpenPos[0][0].y - 32) {

		if (rsHardCoreEventArea[0]<mx && rsHardCoreEventArea[1]>mx &&
			rsHardCoreEventArea[2]<mz && rsHardCoreEventArea[3]>mz) {

			for (cnt = 0; cnt < 7; cnt++) {

				if (mx > rsHardCoreEventOpenPos[cnt][0].x && mx<rsHardCoreEventOpenPos[cnt][1].x &&
					mz>rsHardCoreEventOpenPos[cnt][1].z && mz < rsHardCoreEventOpenPos[cnt][0].z) {

					return cnt;
				}
			}

		}
	}

	return -1;
}




static int rsCompString(char* src1, char* src2)
{
	int cnt;
	int len1, len2;
	int len;

	len1 = lstrlen(src1);
	len2 = lstrlen(src2);

	if (len1 < len2) len = len1;
	else len = len2;

	if (!len) return NULL;

	for (cnt = 0; cnt < len1; cnt++) {
		if (src1[cnt] == 0) break;
		if (src1[cnt] != src2[cnt]) return NULL;
	}

	return cnt;
}

char* rsGetWord(char* q, char* p)
{


	if (*p == 0) { *q = 0; return p; }

	while ((*p == 32) || (*p == 9) || (*p == 58))
	{
		p++;
	}

	while ((*p != 32) && (*p != 9) && (*p != 58) && (*p != 0))
	{
		if (*p == '\n' || *p == '\r') break;
		*q++ = *p++;
	}
	*q++ = 0;



	return p;
}



DWORD GetClanCode(int ClassClanCode)
{
	DWORD	dwCode, dwCode1, dwCode2;

	dwCode = ClassClanCode % 1000000;
	dwCode1 = ClassClanCode / 100000000;
	dwCode2 = dwCode1 * 100000000;

	return dwCode + dwCode2;
}
