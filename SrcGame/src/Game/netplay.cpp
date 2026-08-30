#include "globals.h"

#include "Utils/common.h"

#include "smlib3d\\smd3d.h"
#include <zlib.h>
#include "smwsock.h"
#include "character.h"
#include "playmain.h"
#include "fileread.h"

#pragma comment(lib, "rpcrt4.lib")

#include "particle.h"
#include "sinbaram\\sinlinkheader.h"
#include "record.h"
#include "netplay.h"
#include "playsub.h"
#include "field.h"
#include "cracker.h"
#include "srcLang\\jts.h"
#include "TextMessage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\help.h"
#include "damage.h"
#include "skillsub.h"
#include "GlobalsShared.h"
#include "AreaServer.h"
#include "BellatraFontEffect.h"
#include "srcServer\onserver.h"
#include "WinInt\\WinIntThread.h"
#include "WinInt\\WavIntHttp.h"
#include "WinInt\\ZipLib.h"
#include "CurseFilter.h"
#include "HUD\\RestaureWindow.h"
#include "HUD\\MixWindow.h"
#include "Shop\\NewShop.h"
#include "Shop\\NewShopTime.h"
#include "Montarias\\CMountHandler.h"
#include "cSkinChanger.h"

int MixEvento;

extern int WinSizeX;
extern int WinSizeY;
extern HWND hwnd;

extern BOOL bNewTeleportWindow;
extern sQUEST_CHANGEJOB3	sinQuest_Daily;

#define SERVER_CHAR_INFO
#define RECORD_TIME_STEP		(5*60*1000)
#define NET_LOADING_WAIT_LIMIT		10000


extern CRITICAL_SECTION	cSection;
extern rsRECORD_DBASE	rsRecorder;
DWORD	dwLastRecordTime = 0;
int MatBuffKill[4];
BUFF_KILL BuffKill[32];
CRITICAL_SECTION	cServSection;

int TimeStreak = 20;
int killStreak = 0;

int Debug_SendCount = 0;
int Debug_RecvCount1 = 0;
int Debug_RecvCount2 = 0;
int Debug_RecvCount3 = 0;

extern sITEM TempPerfectItem;
extern sITEM TempPerfectItem2;

int NetWorkInitFlag = 0;
int	BellatraEffectInitFlag = 0;
SBL_SetFontEffect SoD_SetFontEffect;

int needRelog;
int getSodSkill;

DWORD	dwTime_ServerT = 0;
DWORD	dwTime_ConnectMS = 0;

#define	CLIENT_VERSION_NUM		1000
#define CLIENT_VERSION_CHECK	(-CLIENT_VERSION_NUM*2)


int	Client_Version = CLIENT_VERSION_NUM;		//CLIENT VERSION
int	Server_LimitVersion = 168;					//SERVER VERSION 

int	Version_WareHouse = 1;
int	Version_CharInfo = 1;

BOOL ReiPVPEffect = FALSE;						// rei pvp player xxstr
BOOL OtherPlayerReiPVPEffect = FALSE;		    // rei pvp player xxstr
BOOL TopLVLEffect = FALSE;						// Top lvl player xxstr
BOOL OtherPlayerTopLVLEffect = FALSE;		    // Top lvl player xxstr

smWINSOCK* smWsockServer = 0;
smWINSOCK* smWsockDataServer = 0;
smWINSOCK* smWsockUserServer = 0;
smWINSOCK* smWsockExtendServer = 0;

int	ReconnDataServer = 0;
int	ReconnServer = 0;

char	szConnServerName[16];

DWORD	dwRecvUserServerTime = 0;
DWORD	dwRecvDataServerTime = 0;
DWORD	dwRecvServerTime = 0;
DWORD	dwRecvExtendServerTime = 0;


int	rsRecvDataServer = TRUE;
int	rsRecvServer = TRUE;
int	rsRecvUserServer = TRUE;
int	rsRecvExtendServer = TRUE;

#define CHATBUFF_MAX		64
#define CHATBUFF_MASK		63

DWORD dwMyIP;

CHATBUFF ChatBuff[CHATBUFF_MAX];
CHATBUFF ChatBuff2[6][CHATBUFF_MAX];


int	ChatBuffCnt = 0;
int	ChatBuffCnt2[6] = { 0,0,0,0,0,0 };
int ChatDispCnt = 0;
int	ChatBuffCompCnt = 0;

char* szDefaultServIP = "127.0.0.1";

char* szNetLogFile = "debugnet.log";
FILE* fpNetLog = 0;
// showicon scrollchat xxstr
int	ShowIncoScrol = 0;

int	DisconnectFlag = 0;
int	DisconnectServerCode = 0;

int	WarningHack = 0;

DWORD	dwQuestDelayTime = 0;

DWORD	dwLoadingTime = 0;

char szServIP[32];
DWORD dwServPort;
char szDataServIP[32];
DWORD dwDataServPort;
char szUserServIP[32];
DWORD dwUserServPort;
char szExtendServIP[32];
DWORD dwExtendServPort;



DWORD	dwConnectedClientTime = 0;
DWORD	dwConnectedServerTime = 0;
DWORD	dwLastRecvGameServerTime = 0;
DWORD	dwLastRecvGameServerTime2 = 0;
DWORD	dwLastRecvGameServerTime3 = 0;
DWORD	dwLastRecvGameServerTime4 = 0;

typedef DWORD(*LPFN_CheckMem)(TRANS_FUNC_MEMORY* TransFuncMem, smTRANS_COMMAND* lpTransCommand);
LPFN_CheckMem	fnChkMem2;

static int	CompWareHouseMoney = 0;
static int	CompCaravanMoney = 0;

int	WareHouseSubMoney = 0;
int	CaravanSubMoney = 0;

int TotalSubMoney = 0;
int TotalAddMoney = 0;
int	TotalAddExp = 0;
int	TotalSubExp = 0;


smTRANS_COMMAND* lpTransVirtualPotion = 0;


LPFN_CheckMem	fnChkMem;
DWORD	dwMemFunChkCode = 0;

int	RecvMemFuncData(TRANS_FUNC_MEMORY* TransFuncMem);
int	RecvMemFuncData2(TRANS_FUNC_MEMORY* TransFuncMem);

DWORD funcCheckMemSum(DWORD FuncPoint, int count);

int CheckProcessModule();


smPLAYDATA	PlayData;
char	TransBuff[smSOCKBUFF_SIZE];
int		TransLen;


TRANS_ITEMINFO	TransThrowItem;
TRANS_ITEMINFO	TransRecvItem;

TRANS_TRADE_ITEMKEY	LastTransTradeItemKey;
DWORD				dwLastTransTradeKeyTime = 0;

int					InitClanMode = 0;

DWORD				dwYahooTime = 0;

int					ServerHideMode = 0;


PK_FIELD_STATE	PK_FieldState;

struct TRANS_TRADE_CHECKITEM {
	int size, code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;

	DWORD	dwSendItemCode[MAX_TRADE_ITEM];
	DWORD	dwSendItemSum[MAX_TRADE_ITEM];

	DWORD	dwRecvItemCode[MAX_TRADE_ITEM];
	DWORD	dwRecvItemSum[MAX_TRADE_ITEM];
};



int		TradeItemSucessFlag = 0;
int		TradeRecvItemKeyFlag = 0;
int		TradeSendSucessFlag = 0;
DWORD	dwTradeMaskTime = 0;
int		TradeItemCancelCount = 0;

extern int AgingEvento;
extern int AgingPrice;

TRANS_TRADEITEMS	TransLastRecvTrade;
TRANS_TRADEITEMS	TransTradeItems_MyShop;

DWORD	dwLastWareHouseChkSum = 0;
DWORD	dwLastCaravanChkSum = 0;

int		TransAgingItemFlag;

smTRANS_COMMAND	TransServerConnectInfo;

int ConnectOtherPlayer(DWORD dwIP);

int HoRecvMessage(DWORD dwCode, void* RecvBuff);

int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY* lpTransTradeItemKey, sTRADE* lpTrade);
int SendTradeSucessKey(sTRADE* lpTrade, DWORD dwSender);
int RecvTradeCheckItem(TRANS_TRADE_CHECKITEM* lpTransTradeCheckItem);
int SendCrackWindow(HWND hWnd);

int ThrowItemToServer(smTRANS_COMMAND_EX* lpTransCommand);
int RecvCheckItemFromServer(TRANS_ITEM_CODE* lpTransItemCode);
int	SendClientFuncPos();

int OpenTimeCheckThread();
int SendPlayTimerMax();


TRANS_ITEMINFO* FindRecvTransItemQue(DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
int	PushRecvTransItemQue(TRANS_ITEMINFO* lpTransItemInfo);

int RecvProcessSkill(smTRANS_COMMAND* lpTransCommand);
int RecvPartySkillFromServer(TRANS_PARTY_SKILL* lpTransPartySkill);

int FiltQuestItem(TRANS_ITEMINFO* lpTransItemInfo, DWORD dwPacketCode);
int SucessLinkCore(smTRANS_COMMAND_EX* lpTransCommandEx);
int RecvSodGameInfomation(void* Info);

int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2* lpTransItemGroup2);

int XTrap_Recv(smTRANS_COMMAND* lpPacket, smWINSOCK* lpsmSock);

rsBLESS_CASTLE	rsBlessCastleOld;

DWORD	dwResistance_SendingTime = 0;


#define	ATT_MONSTER_MAX	128
#define	ATT_MONSTER_MASK	(ATT_MONSTER_MAX-1)

DWORD	dwAttMonsterCodeList[ATT_MONSTER_MAX];
int		AttMonsterCodeCount = 0;


int ClearAttMonsterCode()
{
	int cnt;
	for (cnt = 0; cnt < ATT_MONSTER_MAX; cnt++) {
		dwAttMonsterCodeList[cnt] = 0;
	}
	return TRUE;
}

int	AddAttMonsterCode(DWORD dwCode)
{
	int mcnt;
	if (dwCode) {
		mcnt = (AttMonsterCodeCount - 1) & ATT_MONSTER_MASK;
		if (dwAttMonsterCodeList[mcnt] == dwCode) return TRUE;
		mcnt = AttMonsterCodeCount & ATT_MONSTER_MASK;
		dwAttMonsterCodeList[mcnt] = dwCode;
		AttMonsterCodeCount++;
		return TRUE;
	}

	return FALSE;
}

int	CheckAttMonsterCode(DWORD dwCode)
{
	int cnt;
	for (cnt = 0; cnt < ATT_MONSTER_MAX; cnt++) {
		if (dwAttMonsterCodeList[cnt] == dwCode) return TRUE;
	}
	return FALSE;
}

struct Cheat
{
	int										iLength;
	int										iHeader;

	int										LParam;
	int										iCheatID;
	int										SParam;
	int										EParam;

	int										LxParam;
	int										WxParam;
	int										SxParam;
	int										ExParam;
};

int CheckServerInfo(smTRANS_COMMAND* lpTransCommand)
{
	SetGameError(0);
	return TRUE;

	if ((-Client_Version * 2) != CLIENT_VERSION_CHECK) {
		SetGameError(1);
		return TRUE;
	}

	if (Client_Version < lpTransCommand->WParam) {
		SetGameError(1);
		return TRUE;
	}
	if (lpTransCommand->LParam > 0) {
		SetGameError(2);
		return TRUE;
	}
	SetGameError(0);
	return TRUE;
}

int SendProcessInfo()
{
	TRANS_CHATMESSAGE	smTransChatMessage;

	if (smWsockDataServer)
	{
		smTransChatMessage.code = smTRANSCODE_PROCESSINFO;
		smTransChatMessage.dwIP = dwExeCheckSum;
		smTransChatMessage.dwObjectSerial = dwExeCheckSum;

		strcpy_s(smTransChatMessage.szMessage, szProcessPath);
		smTransChatMessage.size = 32 + lstrlen(szProcessPath);

		return smWsockDataServer->Send2((char*)&smTransChatMessage, smTransChatMessage.size, TRUE);
	}
	return FALSE;
}


smWINSOCK* ConnectServer_Main()
{
	if (!smWsockServer)
	{
		if (smConfig.szServerIP[0])
			strcpy_s(szServIP, smConfig.szServerIP);
		else
			strcpy_s(szServIP, szDefaultServIP);

		if (smConfig.dwServerPort)
			dwServPort = TCP_SERVPORT;
		else
			dwServPort = TCP_SERVPORT;

		smWsockServer = smConnectSock(szServIP, (WORD)dwServPort);
	}

	if (smWsockServer && !smWsockDataServer)
	{
		if (smConfig.szDataServerIP[0] && lstrcmpi(smConfig.szDataServerIP, smConfig.szServerIP) != 0)
		{
			strcpy_s(szDataServIP, smConfig.szDataServerIP);
			dwDataServPort = smConfig.dwDataServerPort;
			smWsockDataServer = smConnectSock(szDataServIP, (WORD)dwDataServPort);
		}
		else
		{
			strcpy_s(szDataServIP, szServIP);
			dwDataServPort = dwServPort;
			smWsockDataServer = smWsockServer;
		}
	}

	if (smWsockServer && smWsockDataServer && !smWsockUserServer)
	{
		if (smConfig.szUserServerIP[0] && lstrcmpi(smConfig.szUserServerIP, smConfig.szServerIP) != 0) {
			strcpy_s(szUserServIP, smConfig.szUserServerIP);
			dwUserServPort = smConfig.dwUserServerPort;
			smWsockUserServer = smConnectSock(szUserServIP, (WORD)dwUserServPort);
		}
		else {
			strcpy_s(szUserServIP, szServIP);
			dwUserServPort = dwServPort;
			smWsockUserServer = smWsockServer;
		}
	}

	if (smWsockServer && smWsockDataServer && !smWsockExtendServer)
	{
		if (smConfig.szExtendServerIP[0] && lstrcmpi(smConfig.szExtendServerIP, smConfig.szServerIP) != 0) {
			strcpy_s(szExtendServIP, smConfig.szExtendServerIP);
			dwExtendServPort = smConfig.dwExtendServerPort;
			smWsockExtendServer = smConnectSock(szExtendServIP, (WORD)dwExtendServPort);
		}
		else
		{
			strcpy_s(szExtendServIP, szServIP);
			dwExtendServPort = dwServPort;
			smWsockExtendServer = smWsockServer;
		}
	}


	if (smWsockServer && smWsockDataServer && smWsockUserServer)
	{

		if (smConfig.DebugMode) {
		}

		extern int ReconnServer;
		ReconnServer = 0;
		return smWsockServer;
	}

	return NULL;
}

extern int isItemCostume;
extern sITEM InvenCostume;
int limitebuff = 0;
int DisconnectServerFull()
{
	Utils_Log(3, "DisconnectServerFull()");
	if (smWsockDataServer && smWsockDataServer != smWsockServer)
	{
		smWsockDataServer->CloseSocket();
		smWsockDataServer = 0;
	}
	if (smWsockUserServer && smWsockUserServer != smWsockServer)
	{
		smWsockUserServer->CloseSocket();
		smWsockUserServer = 0;
	}
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
	{
		smWsockExtendServer->CloseSocket();
		smWsockExtendServer = 0;
	}
	if (smWsockServer)
	{
		smWsockServer->CloseSocket();
		smWsockServer = 0;
	}

	DisconnectFlag = 0;

	return TRUE;
}
smWINSOCK* ConnectServer_InfoMain()
{
	if (!smWsockDataServer)
	{
		if (smConfig.szDataServerIP[0])
		{
			dwDataServPort = TCP_SERVPORT;

			smWsockDataServer = smConnectSock(smConfig.szDataServerIP, (WORD)dwDataServPort);
			ZeroMemory(&TransServerConnectInfo, sizeof(smTRANS_COMMAND));
			return smWsockDataServer;
		}
	}


	return NULL;
}

smWINSOCK* ConnectServer_GameMain(char* szIP1, DWORD dwPort1, char* szIP2, DWORD dwPort2)
{
	int ServerInfoFlag = 0;

	lstrcpy(smConfig.szServerIP, szIP1);
	lstrcpy(smConfig.szUserServerIP, szIP2);

	if (smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP1) != 0) {
		DisconnectServer_GameMain();
	}
	if (smWsockUserServer && lstrcmp(smWsockUserServer->szIPAddr, szIP2) != 0) {
		DisconnectServer_GameMain();
	}

	if (!smWsockServer)
	{
		if (smWsockDataServer && lstrcmp(smWsockDataServer->szIPAddr, szIP1) == 0 && dwDataServPort == dwPort1)
		{
			smWsockServer = smWsockDataServer;
			dwServPort = dwDataServPort;

			ServerInfoFlag = TRUE;
		}
		else
		{
			smWsockServer = smConnectSock(szIP1, (WORD)dwPort1);
			dwServPort = dwPort1;
		}
		lstrcpy(smConfig.szServerIP, szIP1);
	}

	if (!smWsockUserServer)
	{
		if (smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP2) == 0 && dwServPort == dwPort2)
		{

			smWsockUserServer = smWsockServer;
			dwUserServPort = dwServPort;
		}
		else
		{
			smWsockUserServer = smConnectSock(szIP2, (WORD)dwPort2);
			dwUserServPort = dwPort2;
		}
		lstrcpy(smConfig.szUserServerIP, szIP2);
	}


	if (smWsockServer && smWsockUserServer)
	{
		if (ServerInfoFlag) {
			CheckServerInfo(&TransServerConnectInfo);
		}

		return smWsockServer;
	}
	return NULL;
}

char szServerTime[32] = { 0 };
char szBossTime[32] = { 0 };

smWINSOCK* ConnectServer_GameMain(char* szIP1, DWORD dwPort1, char* szIP2, DWORD dwPort2, char* szIP3, DWORD dwPort3)
{
	int ServerInfoFlag = 0;

	lstrcpy(smConfig.szServerIP, szIP1);
	lstrcpy(smConfig.szUserServerIP, szIP2);
	lstrcpy(smConfig.szExtendServerIP, szIP3);


	if (smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP1) != 0) {
		DisconnectServer_GameMain();
	}
	if (smWsockUserServer && lstrcmp(smWsockUserServer->szIPAddr, szIP2) != 0) {
		DisconnectServer_GameMain();
	}
	if (smWsockExtendServer && lstrcmp(smWsockExtendServer->szIPAddr, szIP3) != 0) {
		DisconnectServer_GameMain();
	}

	if (!smWsockServer)
	{
		if (smWsockDataServer && lstrcmp(smWsockDataServer->szIPAddr, szIP1) == 0 && dwDataServPort == dwPort1)
		{
			smWsockServer = smWsockDataServer;
			dwServPort = dwDataServPort;

			ServerInfoFlag = TRUE;
		}
		else
		{
			smWsockServer = smConnectSock(szIP1, (WORD)dwPort1);
			dwServPort = dwPort1;
		}
		lstrcpy(smConfig.szServerIP, szIP1);
	}

	if (!smWsockUserServer)
	{
		if (smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP2) == 0 && dwServPort == dwPort2)
		{
			smWsockUserServer = smWsockServer;
			dwUserServPort = dwServPort;
		}
		else
		{
			smWsockUserServer = smConnectSock(szIP2, (WORD)dwPort2);
			dwUserServPort = dwPort2;
		}
		lstrcpy(smConfig.szUserServerIP, szIP2);
	}

	if (!smWsockExtendServer)
	{
		if ((smWsockServer && lstrcmp(smWsockServer->szIPAddr, szIP3) == 0 && dwServPort == dwPort3) || !szIP3[0])
		{
			smWsockExtendServer = smWsockServer;
			dwExtendServPort = dwServPort;
		}
		else
		{
			smWsockExtendServer = smConnectSock(szIP3, (WORD)dwPort3);
			dwExtendServPort = dwPort3;
		}
		lstrcpy(smConfig.szExtendServerIP, szIP3);
	}

	if (smWsockServer && smWsockUserServer && smWsockExtendServer)
	{
		if (ServerInfoFlag)
		{
			CheckServerInfo(&TransServerConnectInfo);
		}

		return smWsockServer;
	}
	return NULL;
}


int DisconnectServer_GameMain()
{
	Utils_Log(3, "DisconnectServer_GameMain()");
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
	{
		if (smWsockDataServer != smWsockExtendServer)
		{
			smWsockExtendServer->CloseSocket();
		}
	}
	smWsockExtendServer = 0;

	if (smWsockUserServer && smWsockUserServer != smWsockServer)
	{
		if (smWsockDataServer != smWsockUserServer)
		{
			smWsockUserServer->CloseSocket();
		}
	}
	smWsockUserServer = 0;


	if (smWsockServer && smWsockDataServer != smWsockServer)
	{
		if (smWsockDataServer != smWsockServer)
		{
			smWsockServer->CloseSocket();
		}
	}
	smWsockServer = 0;

	DisconnectFlag = 0;

	return TRUE;
}

smWINSOCK* GetServerSock(int ServerCode)
{
	switch (ServerCode)
	{
	case PLAY_SERVER_CODE_EXTEND:
		if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
			return smWsockExtendServer;
		else
			return smWsockUserServer;

	case PLAY_SERVER_CODE_USER:
		return smWsockUserServer;

	case PLAY_SERVER_CODE_MAIN:
		return smWsockServer;

	case PLAY_SERVER_CODE_AREA1:
		return lpWSockServer_DispArea[0];

	case PLAY_SERVER_CODE_AREA2:
		return lpWSockServer_DispArea[1];

	}

	return NULL;
}


smWINSOCK* GetAreaServerSock()
{
	int ServerCode;

	if (lpCurPlayer->OnStageField >= 0)
	{
		if (AreaServerMode)
		{
			return lpWSockServer_DispArea[lpCurPlayer->OnStageField];
		}
		else
		{
			ServerCode = StageField[lpCurPlayer->OnStageField]->ServerCode;
			return GetServerSock(ServerCode);
		}
	}

	return NULL;
}


scITEM* FindScItem(int x, int z)
{
	int cnt;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++)
	{
		if (scItems[cnt].Flag && scItems[cnt].pX == x && scItems[cnt].pZ == z)
		{
			return &scItems[cnt];
		}
	}

	return NULL;
}

scITEM* FindEmptyScItem()
{
	int cnt;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++) {
		if (!scItems[cnt].Flag) return &scItems[cnt];
	}

	return NULL;
}

int GetTradeDistance(smCHAR* lpChar)
{
	int	x, y, z;
	int dist;

	x = (lpCurPlayer->pX - lpChar->pX) >> FLOATNS;
	y = (lpCurPlayer->pY - lpChar->pY) >> FLOATNS;
	z = (lpCurPlayer->pZ - lpChar->pZ) >> FLOATNS;

	dist = x * x + y * y + z * z;

	if (dist < (256 * 256)) return TRUE;

	return FALSE;
}

int GetTradeDistanceFromCode(DWORD dwObjectSerial)
{
	smCHAR* lpChar;

	lpChar = FindAutoPlayer(dwObjectSerial);

	if (lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0]) {

		return  GetTradeDistance(lpChar);

	}
	return FALSE;
}

int	CheckStartCharInfo(int lvl)
{
	smCHAR_INFO	smCharInfo;
	int cnt;

	memcpy(&smCharInfo, sinChar, sizeof(smCHAR_INFO));

	CheckCharForm();

	if (lvl > 0)
	{
		SetExp64(&lpCurPlayer->smCharInfo, ExpLevelTable[lvl - 1]);
		lpCurPlayer->smCharInfo.Level = lvl;
		lpCurPlayer->smCharInfo.Next_Exp = static_cast<int>(ExpLevelTable[lvl + 1]);
	}

	CodeXorExp = dwPlayTime + smCharInfo.Exp;
	CodeXorExp_High = dwPlayTime | (dwPlayTime * dwPlayTime);
	CodeXorLife = (dwPlayTime * smCharInfo.Life[0]) & 0xFFFF;
	cnt = smCharInfo.Dexterity + smCharInfo.Health + smCharInfo.Spirit + smCharInfo.Strength + smCharInfo.Talent;

	if (smCharInfo.Level >= 2 || cnt > 102 || smCharInfo.Exp || smCharInfo.ChangeJob || smCharInfo.Money)
	{
		SendSetHackUser2(6000, smCharInfo.Money);
		WarningHack = TRUE;
		return FALSE;
	}

	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;

	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	ReformCharForm();

	return TRUE;
}

int	RecvMemFuncData2(TRANS_FUNC_MEMORY* TransFuncMem)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = -1;
	fnChkMem2 = (LPFN_CheckMem)((void*)TransFuncMem->szData);
	fnChkMem2(TransFuncMem, &smTransCommand);

	ZeroMemory(TransFuncMem, 1500);

	if (smTransCommand.size > 0 && smWsockDataServer)
	{
		smTransCommand.code = smTRANSCODE_FUNCTION_MEM2;
		smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	return TRUE;
}



sSERVER_MONEY	sServerMoney[3] = { {0,0,0,0,0},{0,0,0,0,0} };
sSERVER_EXP		sServerExp[3] = { {0,0,0,0},{0,0,0,0} };
DWORD			dwLastSendTotalExpMoenyTime = 0;

int	SendTotalExpMoney()
{
	TRANS_TOTAL_EXPMONEY	TransTotalExpMoney;

	if ((dwLastSendTotalExpMoenyTime + 30000) > dwPlayTime) return FALSE;

	TransTotalExpMoney.code = smTRANSCODE_CHECK_EXPMONEY;
	TransTotalExpMoney.size = sizeof(TRANS_TOTAL_EXPMONEY);

	if (smWsockServer) {
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[0], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[0], sizeof(sSERVER_MONEY));
		smWsockServer->Send2((char*)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}
	if (smWsockUserServer && smWsockUserServer != smWsockServer) {
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[1], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[1], sizeof(sSERVER_MONEY));
		smWsockUserServer->Send2((char*)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer) {
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[2], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[2], sizeof(sSERVER_MONEY));
		smWsockExtendServer->Send2((char*)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}

	dwLastSendTotalExpMoenyTime = dwPlayTime;

	return TRUE;
}

extern int TeleportMars();
extern int TeleportArena();
extern int TeleportBA();
extern int TradeBPEXP();

int CheckServerMoney(smWINSOCK* lpsmSock, TRANS_ITEMINFO* lpTransItemInfo)
{
	sSERVER_MONEY* lpServerMoney;

	lpServerMoney = 0;

	if (lpsmSock == smWsockServer) {
		lpServerMoney = &sServerMoney[0];
	}
	else {
		if (lpsmSock == smWsockUserServer) {
			lpServerMoney = &sServerMoney[1];
		}
		else if (lpsmSock == smWsockExtendServer) {
			lpServerMoney = &sServerMoney[2];
		}
	}

	if (lpServerMoney) {
		lpServerMoney->InputMoney += lpTransItemInfo->Item.Money;
		lpServerMoney->Counter++;
		lpServerMoney->TotalX = lpTransItemInfo->x;
		lpServerMoney->TotalY = lpTransItemInfo->y;
		lpServerMoney->TotalZ = lpTransItemInfo->z;

		int cnt;
		cnt = lpServerMoney->TotalZ ^ (lpServerMoney->TotalX + lpServerMoney->TotalY);
		cnt += 200;
		if (cnt < lpServerMoney->InputMoney) {
			//SendSetHackUser( 82 );
		}
	}

	return TRUE;
}

int CheckServerExp(smWINSOCK* lpsmSock, smTRANS_COMMAND* lpTransCommand)
{
	sSERVER_EXP* lpServerExp;

	lpServerExp = 0;

	if (lpsmSock == smWsockServer)
	{
		lpServerExp = &sServerExp[0];
	}
	else
	{
		if (lpsmSock == smWsockUserServer)
		{
			lpServerExp = &sServerExp[1];
		}
		else if (lpsmSock == smWsockExtendServer)
		{
			lpServerExp = &sServerExp[2];
		}
	}

	if (lpServerExp)
	{
		lpServerExp->InputExp += lpTransCommand->WParam;
		lpServerExp->Counter++;
		lpServerExp->Total1 = lpTransCommand->SParam;
		lpServerExp->Total2 = lpTransCommand->EParam;

		int cnt = (lpServerExp->Total1 ^ lpServerExp->Total2) + 1600000;
		if (cnt < lpServerExp->InputExp)
		{
			SendSetHackUser(83);
		}

	}

	return TRUE;
}

int CreateConnectThread(DWORD dwIP);
int CreateDisconnectThread(smWINSOCK* lpsmSock);

int FindNewOtherPlayer()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (!chrOtherPlayer[cnt].Flag)
			return cnt;
	}

	return -1;
}

smCHAR* FindAutoPlayer(DWORD dwObjectSerial)
{
	for (int cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial == dwObjectSerial)
		{
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

extern BOOL bIsPerfect;
extern BOOL bIsPerfect2;
extern bool bcanadd;

smCHAR* FindChrPlayer(DWORD dwObjectSerial)
{
	int cnt;

	if (lpCurPlayer->dwObjectSerial == dwObjectSerial)
		return lpCurPlayer;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial == dwObjectSerial) {
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

BOOL bReiPVP = FALSE; // rei pvp xxstr 
BOOL bTopLVL = FALSE; // top lvl xxstr 
BOOL bPlayerPvP = FALSE; // modo pk

smCHAR* FindNearMonster(smCHAR* lpCurChar)
{
	int cnt;
	smCHAR* lpChar;
	int dist, x, y, z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300 * 300;

	if (!lpCurChar) return NULL;

	if (lpCurChar != lpCurPlayer) {
		x = (lpCurChar->pX - lpCurPlayer->pX) >> FLOATNS;
		y = (lpCurChar->pY - lpCurPlayer->pY) >> FLOATNS;
		z = (lpCurChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

		dist = x * x + z * z + y * y;
		if (dist < NearDist) {
			if (!lpCurChar->smCharInfo.ClassClan || lpCurChar->smCharInfo.ClassClan != lpCurPlayer->smCharInfo.ClassClan) {
				lpChar = lpCurPlayer;
				NearDist = dist;
			}
		}
	}

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY && lpCurChar != &chrOtherPlayer[cnt]) {
			if (chrOtherPlayer[cnt].smCharInfo.Brood != smCHAR_MONSTER_USER && chrOtherPlayer[cnt].smCharInfo.Life[0] > 0 &&
				(!chrOtherPlayer[cnt].smCharInfo.ClassClan || chrOtherPlayer[cnt].smCharInfo.ClassClan != lpCurChar->smCharInfo.ClassClan))
			{
				x = (lpCurChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpCurChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpCurChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x * x + z * z + y * y;

				if (dist < NearDist) {
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}

smCHAR* FindEmptyChar()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (!chrOtherPlayer[cnt].Flag)
			return &chrOtherPlayer[cnt];
	}

	return NULL;
}

int	ChatBuffFilter(char* szMsg, DWORD dwIP)
{
	return TRUE;
}

DWORD	ConvSysTimeToGameTime(DWORD dwTime)
{
	return dwTime / GAME_WORLDTIME_MIN;
}

#define RECV_DATA_QUE_COUNT		64
#define RECV_DATA_QUE_MASK		63


class	rsTRANS_SERVER {
	char	TransBuff[smSOCKBUFF_SIZE];

	rsRECORD_DBASE* lpRecorder;


	smTHREADSOCK* RecvDataQue[RECV_DATA_QUE_COUNT];
	int				RecvDataQuePush;
	int				RecvDataQuePop;

	TRANS_PLAYPOS	TransPlayPosLast;
	int				TransPlayPosCount;

public:
	smWINSOCK* lpsmSock;


	int	Init(smWINSOCK* lpsmsock);
	int SendChat(char* szMessage);
	int SendPlayData(smCHAR* lpChar);
	int RecvMessage(smTHREADSOCK* SockInfo);
	int RecvMessageQue();

};


int rsTRANS_SERVER::RecvMessageQue()
{
	int cnt;

	if (RecvDataQuePop >= RecvDataQuePush) return TRUE;
	if (MessageLoading) return FALSE;

	EnterCriticalSection(&cSection);

	while (1) {
		if (RecvDataQuePop >= RecvDataQuePush) break;

		cnt = RecvDataQuePop & RECV_DATA_QUE_MASK;

		if (RecvDataQue[cnt])
		{
			RecvMessage(RecvDataQue[cnt]);

			delete RecvDataQue[cnt];
			RecvDataQue[cnt] = 0;
		}

		RecvDataQuePop++;
	}

	LeaveCriticalSection(&cSection);

	return TRUE;
}


DWORD	dwGetCharInfoTime = 0;

#ifndef _A
#define _A
#include "AntiCheat.h"
#endif
#include "HUD/RankingWindow.h"
#include "HUD/SodWindow.h"
#include "HUD/Roleta.h"
#include "../Shared/Skills/base.h"
#include "../Shared/Skills/mechanician.h"
#include "../Shared/Skills/fighter.h"
#include "../Shared/Skills/pikeman.h"
#include "../Shared/Skills/archer.h"
#include "../Shared/Skills/knight.h"
#include "../Shared/Skills/atalanta.h"
#include "../Shared/Skills/priestess.h"
#include "../Shared/Skills/magician.h"
#include "Quest/Quest.h"


extern CAntiCheat* pcAntiCheat;
int firstLogin = 1;
int msgCabelo = 1;

int ChatCnt;

extern string lastReceiveWhisper;
extern string lastWhisper;

extern std::vector<sITEM> ItemsDetails;
extern std::vector<sITEM> ItemsDetailsRestaure;
extern std::vector<sITEM> ItemsDetailsTime;

bool deleteCoin = false;
int delayDeleteCoin = 0;

// Pacotes recebidor do servidor
int rsTRANS_SERVER::RecvMessage(smTHREADSOCK* pData)
{
	DWORD* IPData;
	int cnt;
	int x, z;
	int ServerCode;
	smCHAR* lpChar;
	int RcvCnt;

	TRANS_ITEMINFO* lpTransItemInfo;
	smPLAYDATA* lpPlayData1;
	smTRANS_COMMAND				TransCommand;
	smTRANS_COMMAND_EX			TransCommandEx;
	smTRANS_COMMAND* lpTransCommand;
	smTRANS_COMMAND_EX* lpTransCommandEx;
	smTRANS_ACITON_ITEM* lpTransActionItem;
	smTRANS_CHAR_COMMAND* lpTransCharCommand;
	smTRNAS_PLAYERINFO* lpTransPlayerInfo;
	TRANS_CHATMESSAGE* lpTransChatMessage;
	TRANS_POST_ITEM* lpTransPostItem;
	TRANS_PREMIUM_TIME* lpTransPremiumTime;

	DWORD	dwTime;
	DWORD	dwDataKey;
	smCHAR_INFO* lpCharInfo_MsgBack = 0;
	int		addExp, addMoney;
	DWORD	dwEncCode;

	IPData = (DWORD*)(pData->Buff);

	if (MessageLoading)
	{
		switch (IPData[1])
		{
		case smTRANSCODE_ATTACKDATA2:
		case smTRANSCODE_ATTACKDATA:
		case smTRANSCODE_PLAYDATA1:
		case smTRANSCODE_PLAYDATA2:
		case smTRANSCODE_PLAYDATA3:
		case smTRANSCODE_TRANSPLAYDATA:
		case smTRANSCODE_PLAYDATAGROUP:
		case smTRANSCODE_PLAYITEM:
			return TRUE;
		}

		cnt = RecvDataQuePush - RecvDataQuePop;

		if (cnt >= RECV_DATA_QUE_COUNT) return FALSE;

		cnt = RecvDataQuePush & RECV_DATA_QUE_MASK;

		if (!RecvDataQue[cnt])
		{
			RecvDataQue[cnt] = new smTHREADSOCK;
			if (RecvDataQue[cnt])
			{
				memcpy(RecvDataQue[cnt], pData, sizeof(smTHREADSOCK));
				RecvDataQuePush++;
				return TRUE;
			}
		}
		return FALSE;
	}

	dwEncCode = (IPData[1] & 0xFFFF0000);

	if (dwEncCode == smTRANSCODE_ENCODE_PACKET || dwEncCode == smTRANSCODE_ENCODE_PACKET2)
	{
		smTHREADSOCK smEncData;

		addExp = 0;
		addMoney = 0;

		memcpy(&smEncData, pData, sizeof(smTHREADSOCK) - smSOCKBUFF_SIZE);

		if (dwEncCode == smTRANSCODE_ENCODE_PACKET2) {
			if (fnDecodePacket)
				dwDataKey = fnDecodePacket((BYTE*)pData->Buff, (BYTE*)smEncData.Buff);
			else
				return FALSE;
		}
		else
			dwDataKey = DecodePacket((BYTE*)pData->Buff, (BYTE*)smEncData.Buff);

		if (!dwDataKey)
			return FALSE;

		cnt = dwDataKey & 0xFFFF;

		if (pData->smMySock->CheckEncRecvPacket(cnt) == TRUE) {
			return FALSE;
		}

		if (cnt < (pData->smMySock->EncPacketRecvCount - ENC_PACKET_REC_MAX)) {
			return FALSE;
		}
		pData->smMySock->PushEncRecvPacket(cnt);


		pData = &smEncData;
		IPData = (DWORD*)(pData->Buff);

		lpCharInfo_MsgBack = new smCHAR_INFO;
		if (lpCharInfo_MsgBack) {
			memcpy(lpCharInfo_MsgBack, &lpCurPlayer->smCharInfo, sizeof(smCHAR_INFO));
		}
	}

#ifdef _DEBUG
	Utils_Log(LOG_DEBUG, "Recv %s (%x) %d\r\n", GetTranscodeNameByOpcode(IPData[1]), IPData[1], IPData[0]);
#endif

	switch (IPData[1])
	{
	case smTRANSCODE_SKILLS:
	{
		skillPacket* packet = (skillPacket*)pData->Buff;

		switch (packet->classId)
		{
		case GROUP_MECHANICIAN:
		{
			switch (packet->skillId)
			{
			case SKILL_EXTREME_SHIELD:
			{
				E_Shield* e_shield = (E_Shield*)packet;

				CopyMemory(&E_Shield_BlockRate, &e_shield->E_Shield_BlockRate, sizeof(E_Shield_BlockRate));
				CopyMemory(&E_Shield_UseMana, &e_shield->E_Shield_UseMana, sizeof(E_Shield_UseMana));
				CopyMemory(&E_Shield_UseTime, &e_shield->E_Shield_UseTime, sizeof(E_Shield_UseTime));
			}
			break;

			case SKILL_MECHANIC_BOMB:
			{
				Mechanic_Bomb* mechanic_Bomb = (Mechanic_Bomb*)packet;

				CopyMemory(&Mechanic_Bomb_Damage, &mechanic_Bomb->Mechanic_Bomb_Damage, sizeof(Mechanic_Bomb_Damage));
				CopyMemory(&Mechanic_Bomb_Attack_Range, &mechanic_Bomb->Mechanic_Bomb_Attack_Range, sizeof(Mechanic_Bomb_Attack_Range));
				CopyMemory(&Mechanic_Bomb_UseMana, &mechanic_Bomb->Mechanic_Bomb_UseMana, sizeof(Mechanic_Bomb_UseMana));
			}
			break;

			case SKILL_PHYSICAL_ABSORB:
			{
				Physical_Absorb* physical_Absorb = (Physical_Absorb*)packet;

				CopyMemory(&P_Absorb, &physical_Absorb->P_Absorb, sizeof(P_Absorb));
				CopyMemory(&P_Absorb_UseTime, &physical_Absorb->P_Absorb_UseTime, sizeof(P_Absorb_UseTime));
				CopyMemory(&P_Absor_bUseMana, &physical_Absorb->P_Absor_bUseMana, sizeof(P_Absor_bUseMana));
			}
			break;

			case SKILL_POISON_ATTRIBUTE:
			{
				Poison_Resistence* poison_Resistence = (Poison_Resistence*)packet;

				CopyMemory(&PlusPoison, &poison_Resistence->PlusPoison, sizeof(PlusPoison));
			}
			break;

			case SKILL_GREAT_SMASH:
			{
				G_Smash* g_Smash = (G_Smash*)packet;

				CopyMemory(&G_Smash_Attack_Rate, &g_Smash->G_Smash_Attack_Rate, sizeof(G_Smash_Attack_Rate));
				CopyMemory(&G_Smash_Attack_Rage, &g_Smash->G_Smash_Attack_Rage, sizeof(G_Smash_Attack_Rage));
				CopyMemory(&G_Smash_UseMana, &g_Smash->G_Smash_UseMana, sizeof(G_Smash_UseMana));
			}
			break;

			case SKILL_MAXIMIZE:
			{
				Maximize* maximize = (Maximize*)packet;

				CopyMemory(&Maximize_Damage, &maximize->Maximize_Damage, sizeof(Maximize_Damage));
				CopyMemory(&Maximize_Time, &maximize->Maximize_Time, sizeof(Maximize_Time));
				CopyMemory(&Maximize_UseMana, &maximize->Maximize_UseMana, sizeof(Maximize_UseMana));
			}
			break;

			case SKILL_AUTOMATION:
			{
				Automation* automation = (Automation*)packet;

				CopyMemory(&Automation_Damage, &automation->Automation_Damage, sizeof(Automation_Damage));
				CopyMemory(&Automation_Speed, &automation->Automation_Speed, sizeof(Automation_Speed));
				CopyMemory(&Automation_Time, &automation->Automation_Time, sizeof(Automation_Time));
				CopyMemory(&Automation_UseMana, &automation->Automation_UseMana, sizeof(Automation_UseMana));
			}
			break;

			case SKILL_SPARK:
			{
				Spark* spark = (Spark*)packet;

				CopyMemory(&Spark_Damage, &spark->Spark_Damage, sizeof(Spark_Damage));
				CopyMemory(&Spark_Area, &spark->Spark_Area, sizeof(Spark_Area));
				CopyMemory(&Spark_UseMana, &spark->Spark_UseMana, sizeof(Spark_UseMana));
			}
			break;

			case SKILL_METAL_ARMOR:
			{
				Metal_Armor* metal_Armor = (Metal_Armor*)packet;

				CopyMemory(&Metal_Armor_Defense, &metal_Armor->Metal_Armor_Defense, sizeof(Metal_Armor_Defense));
				CopyMemory(&Metal_Armor_Time, &metal_Armor->Metal_Armor_Time, sizeof(Metal_Armor_Time));
				CopyMemory(&Metal_Armor_UseMana, &metal_Armor->Metal_Armor_UseMana, sizeof(Metal_Armor_UseMana));
			}
			break;

			case SKILL_GRAND_SMASH:
			{
				Grand_Smash* grand_Smash = (Grand_Smash*)packet;

				CopyMemory(&Grand_Smash_AttackRate, &grand_Smash->Grand_Smash_AttackRate, sizeof(Grand_Smash_AttackRate));
				CopyMemory(&Grand_Smash_Damage, &grand_Smash->Grand_Smash_Damage, sizeof(Grand_Smash_Damage));
				CopyMemory(&Grand_Smash_UseMana, &grand_Smash->Grand_Smash_UseMana, sizeof(Grand_Smash_UseMana));
			}
			break;

			case SKILL_MECHANIC_WEAPON:
			{
				Mechanician_Weapon_Mastery* mechanician_Weapon_Mastery = (Mechanician_Weapon_Mastery*)packet;

				CopyMemory(&M_Weapon_Mastey, &mechanician_Weapon_Mastery->M_Weapon_Mastey, sizeof(M_Weapon_Mastey));
			}
			break;

			case SKILL_SPARK_SHIELD:
			{
				Spark_Shield* spark_Shield = (Spark_Shield*)packet;

				CopyMemory(&Spark_Shield_Damage, &spark_Shield->Spark_Shield_Damage, sizeof(Spark_Shield_Damage));
				CopyMemory(&Spark_Shield_Defense, &spark_Shield->Spark_Shield_Defense, sizeof(Spark_Shield_Defense));
				CopyMemory(&Spark_Shield_Time, &spark_Shield->Spark_Shield_Time, sizeof(Spark_Shield_Time));
				CopyMemory(&Spark_Shield_UseMana, &spark_Shield->Spark_Shield_UseMana, sizeof(Spark_Shield_UseMana));
			}
			break;

			case SKILL_IMPULSION:
			{
				Impulsion* impulsion = (Impulsion*)packet;

				CopyMemory(&Impulsion_LightingDamage, &impulsion->Impulsion_LightingDamage, sizeof(Impulsion_LightingDamage));
				CopyMemory(&Impulsion_Range, &impulsion->Impulsion_Range, sizeof(Impulsion_Range));
				CopyMemory(&Impulsion_LightingNum, &impulsion->Impulsion_LightingNum, sizeof(Impulsion_LightingNum));
				CopyMemory(&Impulsion_UseMana, &impulsion->Impulsion_UseMana, sizeof(Impulsion_UseMana));
			}
			break;

			case SKILL_COMPULSION:
			{
				Compulsion* compulsion = (Compulsion*)packet;

				CopyMemory(&Compulsion_AddAbsorb, &compulsion->Compulsion_AddAbsorb, sizeof(Compulsion_AddAbsorb));
				CopyMemory(&Compulsion_Area, &compulsion->Compulsion_Area, sizeof(Compulsion_Area));
				CopyMemory(&Compulsion_Time, &compulsion->Compulsion_Time, sizeof(Compulsion_Time));
				CopyMemory(&Compulsion_UseMana, &compulsion->Compulsion_UseMana, sizeof(Compulsion_UseMana));
			}
			break;

			case SKILL_MAGNETIC_SPHERE:
			{
				Magnetic_Sphere* magnetic_Sphere = (Magnetic_Sphere*)packet;

				CopyMemory(&Magnetic_Sphere_AddDamage, &magnetic_Sphere->Magnetic_Sphere_AddDamage, sizeof(Magnetic_Sphere_AddDamage));
				CopyMemory(&Magnetic_Sphere_Area, &magnetic_Sphere->Magnetic_Sphere_Area, sizeof(Magnetic_Sphere_Area));
				CopyMemory(&Magnetic_Sphere_AttackDelay, &magnetic_Sphere->Magnetic_Sphere_AttackDelay, sizeof(Magnetic_Sphere_AttackDelay));
				CopyMemory(&Magnetic_Sphere_Time, &magnetic_Sphere->Magnetic_Sphere_Time, sizeof(Magnetic_Sphere_Time));
				CopyMemory(&Magnetic_Sphere_UseMana, &magnetic_Sphere->Magnetic_Sphere_UseMana, sizeof(Magnetic_Sphere_UseMana));
			}
			break;

			case SKILL_METAL_GOLEM:
			{
				Metal_Golem* metal_Golem = (Metal_Golem*)packet;

				CopyMemory(&Metal_Golem_Damage, &metal_Golem->Metal_Golem_Damage, sizeof(Metal_Golem_Damage));
				CopyMemory(&Metal_Golem_Hit, &metal_Golem->Metal_Golem_Hit, sizeof(Metal_Golem_Hit));
				CopyMemory(&Metal_Golem_Defense, &metal_Golem->Metal_Golem_Defense, sizeof(Metal_Golem_Defense));
				CopyMemory(&Metal_Golem_Life, &metal_Golem->Metal_Golem_Life, sizeof(Metal_Golem_Life));
				CopyMemory(&Metal_Golem_UseMana, &metal_Golem->Metal_Golem_UseMana, sizeof(Metal_Golem_UseMana));
			}
			break;
			}
		}
		break;

		case GROUP_FIGHTER:
		{
			switch (packet->skillId)
			{
			case SKILL_MELEE_MASTERY:
			{
				Melee_Mastery* melee_mastery = (Melee_Mastery*)packet;

				CopyMemory(&Melee_Mastery_DamagePercent, melee_mastery->Melee_Mastery_DamagePercent, sizeof(Melee_Mastery_DamagePercent));
			}
			break;

			case SKILL_FIRE_ATTRIBUTE:
			{
				Plus_Fire* plus_fire = (Plus_Fire*)packet;

				CopyMemory(&PlusFire, plus_fire->PlusFire, sizeof(PlusFire));
			}
			break;

			case SKILL_RAVING:
			{
				Raving* raving = (Raving*)packet;

				CopyMemory(&Raving_Damage, raving->Raving_Damage, sizeof(Raving_Damage));
				CopyMemory(&Ravind_Speed, raving->Ravind_Speed, sizeof(Ravind_Speed));
				CopyMemory(&Raving_UseLife, raving->Raving_UseLife, sizeof(Raving_UseLife));
				CopyMemory(&Raving_UseMana, raving->Raving_UseMana, sizeof(Raving_UseMana));
			}
			break;

			case SKILL_IMPACT:
			{
				Impact* impact = (Impact*)packet;

				CopyMemory(&Impact_Attack_Rating, impact->Impact_Attack_Rating, sizeof(Impact_Attack_Rating));
				CopyMemory(&Impact_Damage, impact->Impact_Damage, sizeof(Impact_Damage));
				CopyMemory(&Impact_UseMana, impact->Impact_UseMana, sizeof(Impact_UseMana));
			}
			break;

			case SKILL_TRIPLE_IMPACT:
			{
				T_Impact* t_impact = (T_Impact*)packet;

				CopyMemory(&T_Impact_Damage, t_impact->T_Impact_Damage, sizeof(T_Impact_Damage));
				CopyMemory(&T_Impact_Hit, t_impact->T_Impact_Hit, sizeof(T_Impact_Hit));
				CopyMemory(&T_Impact_UseMana, t_impact->T_Impact_UseMana, sizeof(T_Impact_UseMana));

			}
			break;

			case SKILL_BRUTAL_SWING:
			{
				B_Swing* b_swing = (B_Swing*)packet;

				CopyMemory(&B_Swing_Damage, b_swing->B_Swing_Damage, sizeof(B_Swing_Damage));
				CopyMemory(&B_Swing_Critical, b_swing->B_Swing_Critical, sizeof(B_Swing_Critical));
				CopyMemory(&B_Swing_UseMana, b_swing->B_Swing_UseMana, sizeof(B_Swing_UseMana));
			}
			break;

			case SKILL_ROAR:
			{
				Roar* roar = (Roar*)packet;

				CopyMemory(&Roar_Range, roar->Roar_Range, sizeof(Roar_Range));
				CopyMemory(&Roar_Time, roar->Roar_Time, sizeof(Roar_Time));
				CopyMemory(&Roar_UseMana, roar->Roar_UseMana, sizeof(Roar_UseMana));
			}
			break;

			case SKILL_RAGE_OF_ZECRAM:
			{
				R_Zecram* r_zecram = (R_Zecram*)packet;

				CopyMemory(&R_Zecram_Damage, r_zecram->R_Zecram_Damage, sizeof(R_Zecram_Damage));
				CopyMemory(&R_Zecram_UseMana, r_zecram->R_Zecram_UseMana, sizeof(R_Zecram_UseMana));
			}
			break;

			case SKILL_CONCENTRATION:
			{
				Concentration* concentration = (Concentration*)packet;

				CopyMemory(&Concentration_AttackRate, concentration->Concentration_AttackRate, sizeof(Concentration_AttackRate));
				CopyMemory(&Concentration_Time, concentration->Concentration_Time, sizeof(Concentration_Time));
				CopyMemory(&Concentration_UseMana, concentration->Concentration_UseMana, sizeof(Concentration_UseMana));
			}
			break;

			case SKILL_AVANGING_CRASH:
			{
				A_Crash* a_crash = (A_Crash*)packet;

				CopyMemory(&A_Crash_Damage, a_crash->A_Crash_Damage, sizeof(A_Crash_Damage));
				CopyMemory(&A_Crash_AttackRate, a_crash->A_Crash_AttackRate, sizeof(A_Crash_AttackRate));
				CopyMemory(&A_Crash_UseMana, a_crash->A_Crash_UseMana, sizeof(A_Crash_UseMana));
			}
			break;

			case SKILL_SWIFT_AXE:
			{
				Swift_Axe* swift_axe = (Swift_Axe*)packet;

				CopyMemory(&Swift_Axe_Speed, swift_axe->Swift_Axe_Speed, sizeof(Swift_Axe_Speed));
				CopyMemory(&Swift_Axe_Time, swift_axe->Swift_Axe_Time, sizeof(Swift_Axe_Time));
				CopyMemory(&Swift_Axe_UseMana, swift_axe->Swift_Axe_UseMana, sizeof(Swift_Axe_UseMana));
			}
			break;

			case SKILL_BONE_CRASH:
			{
				B_Crash* b_crash = (B_Crash*)packet;

				CopyMemory(&B_Crash_Damage, b_crash->B_Crash_Damage, sizeof(B_Crash_Damage));
				CopyMemory(&B_Crash_DemonDamage, b_crash->B_Crash_DemonDamage, sizeof(B_Crash_DemonDamage));
				CopyMemory(&B_Crash_UseMana, b_crash->B_Crash_UseMana, sizeof(B_Crash_UseMana));
			}
			break;

			case SKILL_DETORYER:
			{
				Destroyer* destroyer = (Destroyer*)packet;

				CopyMemory(&Destoryer_DamagePercent, destroyer->Destoryer_DamagePercent, sizeof(Destoryer_DamagePercent));
				CopyMemory(&Destoryer_AddCritical, destroyer->Destoryer_AddCritical, sizeof(Destoryer_AddCritical));
				CopyMemory(&Destoryer_UseMana, destroyer->Destoryer_UseMana, sizeof(Destoryer_UseMana));
			}
			break;

			case SKILL_BERSERKER:
			{
				Berserker* berserker = (Berserker*)packet;

				CopyMemory(&Berserker_AddAttack, berserker->Berserker_AddAttack, sizeof(Berserker_AddAttack));
				CopyMemory(&Berserker_SubAbsorb, berserker->Berserker_SubAbsorb, sizeof(Berserker_SubAbsorb));
				CopyMemory(&Berserker_Time, berserker->Berserker_Time, sizeof(Berserker_Time));
				CopyMemory(&Berserker_UseMana, berserker->Berserker_UseMana, sizeof(Berserker_UseMana));
			}
			break;

			case SKILL_CYCLONE_STRIKE:
			{
				Cyclone_Strike* cyclone_strike = (Cyclone_Strike*)packet;

				CopyMemory(&Cyclone_Strike_DamagePercent, cyclone_strike->Cyclone_Strike_DamagePercent, sizeof(Cyclone_Strike_DamagePercent));
				CopyMemory(&Cyclone_Strike_AreaDamage, cyclone_strike->Cyclone_Strike_AreaDamage, sizeof(Cyclone_Strike_AreaDamage));
				CopyMemory(&Cyclone_Strike_AttackNum, cyclone_strike->Cyclone_Strike_AttackNum, sizeof(Cyclone_Strike_AttackNum));
				CopyMemory(&Cyclone_Strike_Area, cyclone_strike->Cyclone_Strike_Area, sizeof(Cyclone_Strike_Area));
				CopyMemory(&Cyclone_Strike_UseMana, cyclone_strike->Cyclone_Strike_UseMana, sizeof(Cyclone_Strike_UseMana));
			}
			break;

			case SKILL_BOOST_HEALTH:
			{
				Boost_Health* boost_health = (Boost_Health*)packet;

				CopyMemory(&Boost_Health_Life, boost_health->Boost_Health_Life, sizeof(Boost_Health_Life));
			}
			break;

			}

		}
		break;

		case GROUP_PIKEMAN:
		{
			switch (packet->skillId)
			{
			case SKILL_PIKE_WIND:
			{
				Pike_Wind* pike_wind = (Pike_Wind*)packet;

				CopyMemory(&Pike_Wind_Damage, pike_wind->Pike_Wind_Damage, sizeof(Pike_Wind_Damage));
				CopyMemory(&Pike_Wind_Push_Lenght, pike_wind->Pike_Wind_Push_Lenght, sizeof(Pike_Wind_Push_Lenght));
				CopyMemory(&Pike_Wind_UseMana, pike_wind->Pike_Wind_UseMana, sizeof(Pike_Wind_UseMana));
			}
			break;

			case SKILL_ICE_ATTRIBUTE:
			{
				Plus_Ice* plus_ice = (Plus_Ice*)packet;

				CopyMemory(&PlusIce, plus_ice->PlusIce, sizeof(PlusIce));
			}
			break;

			case SKILL_CRITICAL_HIT:
			{
				Critical_Hit* critical_hit = (Critical_Hit*)packet;

				CopyMemory(&Critical_Hit_Critical, critical_hit->Critical_Hit_Critical, sizeof(Critical_Hit_Critical));
				CopyMemory(&Critical_Hit_UseMana, critical_hit->Critical_Hit_UseMana, sizeof(Critical_Hit_UseMana));
			}
			break;

			case SKILL_JUMPING_CRASH:
			{
				Jumping_Crash* jumping_crash = (Jumping_Crash*)packet;

				CopyMemory(&Jumping_Crash_UseMana, jumping_crash->Jumping_Crash_UseMana, sizeof(Jumping_Crash_UseMana));
				CopyMemory(&Jumping_Crash_Attack_Rating, jumping_crash->Jumping_Crash_Attack_Rating, sizeof(Jumping_Crash_Attack_Rating));
				CopyMemory(&Jumping_Crash_Damage, jumping_crash->Jumping_Crash_Damage, sizeof(Jumping_Crash_Damage));
			}
			break;

			case SKILL_GROUND_PIKE:
			{
				G_Pike* g_pike = (G_Pike*)packet;

				CopyMemory(&G_Pike_Damage, g_pike->G_Pike_Damage, sizeof(G_Pike_Damage));
				CopyMemory(&G_Pike_Range, g_pike->G_Pike_Range, sizeof(G_Pike_Range));
				CopyMemory(&G_Pike_UseMana, g_pike->G_Pike_UseMana, sizeof(G_Pike_UseMana));
				CopyMemory(&G_Pike_Time, g_pike->G_Pike_Time, sizeof(G_Pike_Time));
			}
			break;

			case SKILL_TORNADO:
			{
				Tornado* tornado = (Tornado*)packet;

				CopyMemory(&Tornado_Damage, tornado->Tornado_Damage, sizeof(Tornado_Damage));
				CopyMemory(&Tornado_Stun, tornado->Tornado_Stun, sizeof(Tornado_Stun));
				CopyMemory(&Tornado_Range, tornado->Tornado_Range, sizeof(Tornado_Range));
				CopyMemory(&Tornado_UseMana, tornado->Tornado_UseMana, sizeof(Tornado_UseMana));
			}
			break;

			case SKILL_WEAPONE_DEFENCE_MASTERY:
			{
				W_D_Mastery* w_d_mastery = (W_D_Mastery*)packet;

				CopyMemory(&W_D_Mastery_Block, w_d_mastery->W_D_Mastery_Block, sizeof(W_D_Mastery_Block));
			}
			break;

			case SKILL_EXPANSION:
			{
				Expansion* expansion = (Expansion*)packet;

				CopyMemory(&Expansion_Size, expansion->Expansion_Size, sizeof(Expansion_Size));
				CopyMemory(&Expansion_Damage, expansion->Expansion_Damage, sizeof(Expansion_Damage));
				CopyMemory(&Expansion_UseMana, expansion->Expansion_UseMana, sizeof(Expansion_UseMana));
			}
			break;

			case SKILL_VENOM_SPEAR:
			{
				Venom_Spear* venom_spear = (Venom_Spear*)packet;

				CopyMemory(&VenomSpear_Damage, venom_spear->VenomSpear_Damage, sizeof(VenomSpear_Damage));
				CopyMemory(&VenomSpear_Num, venom_spear->VenomSpear_Num, sizeof(VenomSpear_Num));
				CopyMemory(&VenomSpear_Time, venom_spear->VenomSpear_Time, sizeof(VenomSpear_Time));
				CopyMemory(&VenomSpear_Range, venom_spear->VenomSpear_Range, sizeof(VenomSpear_Range));
				CopyMemory(&VenomSpear_UseMana, venom_spear->VenomSpear_UseMana, sizeof(VenomSpear_UseMana));
			}
			break;

			case SKILL_VANISH:
			{
				Vanish* vanish = (Vanish*)packet;

				CopyMemory(&Vanish_Damage, vanish->Vanish_Damage, sizeof(Vanish_Damage));
				CopyMemory(&Vanish_Range, vanish->Vanish_Range, sizeof(Vanish_Range));
				CopyMemory(&Vanish_Time, vanish->Vanish_Time, sizeof(Vanish_Time));
				CopyMemory(&Vanish_UseMana, vanish->Vanish_UseMana, sizeof(Vanish_UseMana));
			}
			break;

			case SKILL_CRITICAL_MASTERY:
			{
				Critical_Mastery* critical_mastery = (Critical_Mastery*)packet;

				CopyMemory(&Critical_Mastery_Critical, critical_mastery->Critical_Mastery_Critical, sizeof(Critical_Mastery_Critical));
			}
			break;

			case SKILL_CHAIN_LANCE:
			{
				Chain_Lance* chain_lance = (Chain_Lance*)packet;

				CopyMemory(&Chain_Lance_Damage, chain_lance->Chain_Lance_Damage, sizeof(Chain_Lance_Damage));
				CopyMemory(&Chain_Lance_UseMana, chain_lance->Chain_Lance_UseMana, sizeof(Chain_Lance_UseMana));
			}
			break;

			case SKILL_ASSASSIN_EYE:
			{
				Assassin_Eye* assassin_eye = (Assassin_Eye*)packet;

				CopyMemory(&Assassin_Eye_AddCritical, assassin_eye->Assassin_Eye_AddCritical, sizeof(Assassin_Eye_AddCritical));
				CopyMemory(&Assassin_Eye_M_SubCritical, assassin_eye->Assassin_Eye_M_SubCritical, sizeof(Assassin_Eye_M_SubCritical));
				CopyMemory(&Assassin_Eye_Time, assassin_eye->Assassin_Eye_Time, sizeof(Assassin_Eye_Time));
				CopyMemory(&Assassin_Eye_UseMana, assassin_eye->Assassin_Eye_UseMana, sizeof(Assassin_Eye_UseMana));
			}
			break;

			case SKILL_CHARGING_STRIKE:
			{
				Charging_Strike* charging_strike = (Charging_Strike*)packet;

				CopyMemory(&Charging_Strike_DamagePercent, charging_strike->Charging_Strike_DamagePercent, sizeof(Charging_Strike_DamagePercent));
				CopyMemory(&Charging_Strike_CharingDamagePercent, charging_strike->Charging_Strike_CharingDamagePercent, sizeof(Charging_Strike_CharingDamagePercent));
				CopyMemory(&Charging_Strike_Time, charging_strike->Charging_Strike_Time, sizeof(Charging_Strike_Time));
				CopyMemory(&Charging_Strike_UseMana, charging_strike->Charging_Strike_UseMana, sizeof(Charging_Strike_UseMana));

			}
			break;

			case SKILL_VAGUE:
			{
				Vague* vague = (Vague*)packet;

				CopyMemory(&Vague_EvasionPercent, vague->Vague_EvasionPercent, sizeof(Vague_EvasionPercent));
				CopyMemory(&Vague_Time, vague->Vague_Time, sizeof(Vague_Time));
				CopyMemory(&Vague_UseMana, vague->Vague_UseMana, sizeof(Vague_UseMana));
			}
			break;

			case SKILL_SHADOW_MASTER:
			{
				Shadow_Master* shadow_master = (Shadow_Master*)packet;

				CopyMemory(&Shadow_Master_DamagePercent, shadow_master->Shadow_Master_DamagePercent, sizeof(Shadow_Master_DamagePercent));
				CopyMemory(&Shadow_Master_ShadowNum, shadow_master->Shadow_Master_ShadowNum, sizeof(Shadow_Master_ShadowNum));
				CopyMemory(&Shadow_Master_UseMana, shadow_master->Shadow_Master_UseMana, sizeof(Shadow_Master_UseMana));
			}
			break;

			}
		}
		break;

		case GROUP_ARCHER:
		{
			switch (packet->skillId)
			{
			case SKILL_SCOUT_HAWK:
			{
				Scout_Hawk* scout_hawk = (Scout_Hawk*)packet;

				CopyMemory(&Scout_Hawk_Attack_Rate, scout_hawk->Scout_Hawk_Attack_Rate, sizeof(Scout_Hawk_Attack_Rate));
				CopyMemory(&Scout_Hawk_UseTime, scout_hawk->Scout_Hawk_UseTime, sizeof(Scout_Hawk_UseTime));
				CopyMemory(&Scout_Hawk_UseMana, scout_hawk->Scout_Hawk_UseMana, sizeof(Scout_Hawk_UseMana));
			}
			break;

			case SKILL_SHOOTING_MASTERY:
			{
				S_Mastery* s_mastery = (S_Mastery*)packet;

				CopyMemory(&S_Mastery_DamagePercent, s_mastery->S_Mastery_DamagePercent, sizeof(S_Mastery_DamagePercent));
			}
			break;

			case SKILL_WIND_ARROW:
			{
				Wind_Arrow* wind_arrow = (Wind_Arrow*)packet;

				CopyMemory(&Wind_Arrow_Speed, wind_arrow->Wind_Arrow_Speed, sizeof(Wind_Arrow_Speed));
				CopyMemory(&Wind_Arrow_Damage, wind_arrow->Wind_Arrow_Damage, sizeof(Wind_Arrow_Damage));
				CopyMemory(&Wind_Arrow_UseMana, wind_arrow->Wind_Arrow_UseMana, sizeof(Wind_Arrow_UseMana));
			}
			break;

			case SKILL_PERFECT_AIM:
			{

				Perfect_Aim* perfect_aim = (Perfect_Aim*)packet;

				CopyMemory(&Perfect_Aim_Attack_Rate, perfect_aim->Perfect_Aim_Attack_Rate, sizeof(Perfect_Aim_Attack_Rate));
				CopyMemory(&Perfect_Aim_Damage, perfect_aim->Perfect_Aim_Damage, sizeof(Perfect_Aim_Damage));
				CopyMemory(&Perfect_Aim_Damage_LV, perfect_aim->Perfect_Aim_Damage_LV, sizeof(Perfect_Aim_Damage_LV));
				CopyMemory(&Perfect_Aim_UseMana, perfect_aim->Perfect_Aim_UseMana, sizeof(Perfect_Aim_UseMana));

			}
			break;

			case SKILL_DIONS_EYE:
			{
				D_Eye* d_eye = (D_Eye*)packet;

				CopyMemory(&D_Eye_Attack_Rate, d_eye->D_Eye_Attack_Rate, sizeof(D_Eye_Attack_Rate));
			}
			break;

			case SKILL_FALCON:
			{
				Falcon* falcon = (Falcon*)packet;

				CopyMemory(&Falcon_Damage, falcon->Falcon_Damage, sizeof(Falcon_Damage));
				CopyMemory(&Falcon_Time, falcon->Falcon_Time, sizeof(Falcon_Time));
				CopyMemory(&Falcon_UseMana, falcon->Falcon_UseMana, sizeof(Falcon_UseMana));
			}
			break;

			case SKILL_ARROW_OF_RAGE:
			{
				A_Rage* a_rage = (A_Rage*)packet;

				CopyMemory(&A_Rage_ArrowNum, a_rage->A_Rage_ArrowNum, sizeof(A_Rage_ArrowNum));
				CopyMemory(&A_Rage_Damage, a_rage->A_Rage_Damage, sizeof(A_Rage_Damage));
				CopyMemory(&A_Rage_Area, a_rage->A_Rage_Area, sizeof(A_Rage_Area));
				CopyMemory(&A_Rage_UseMana, a_rage->A_Rage_UseMana, sizeof(A_Rage_UseMana));
			}
			break;

			case SKILL_AVALANCHE:
			{
				Avalanche* avalanche = (Avalanche*)packet;

				CopyMemory(&Avalanche_Speed, avalanche->Avalanche_Speed, sizeof(Avalanche_Speed));
				CopyMemory(&Avalanche_Damage, avalanche->Avalanche_Damage, sizeof(Avalanche_Damage));
				CopyMemory(&Avalanche_ArrowNum, avalanche->Avalanche_ArrowNum, sizeof(Avalanche_ArrowNum));
				CopyMemory(&Avalanche_UseMana, avalanche->Avalanche_UseMana, sizeof(Avalanche_UseMana));
			}
			break;

			case SKILL_ELEMENTAL_SHOT:
			{
				Elemental_Shot* elemental_shot = (Elemental_Shot*)packet;

				CopyMemory(&Elemental_Shot_Fire, elemental_shot->Elemental_Shot_Fire, sizeof(Elemental_Shot_Fire));
				CopyMemory(&Elemental_Shot_Lightning, elemental_shot->Elemental_Shot_Lightning, sizeof(Elemental_Shot_Lightning));
				CopyMemory(&Elemental_Shot_UseMana, elemental_shot->Elemental_Shot_UseMana, sizeof(Elemental_Shot_UseMana));
			}
			break;

			case SKILL_GOLDEN_FALCON:
			{
				Golden_Falcon* golden_falcon = (Golden_Falcon*)packet;

				CopyMemory(&Golden_Falcon_Damage, golden_falcon->Golden_Falcon_Damage, sizeof(Golden_Falcon_Damage));
				CopyMemory(&Golden_Falcon_LifeRegen, golden_falcon->Golden_Falcon_LifeRegen, sizeof(Golden_Falcon_LifeRegen));
				CopyMemory(&Golden_Falcon_Time, golden_falcon->Golden_Falcon_Time, sizeof(Golden_Falcon_Time));
				CopyMemory(&Golden_Falcon_UseMana, golden_falcon->Golden_Falcon_UseMana, sizeof(Golden_Falcon_UseMana));
			}
			break;

			case SKILL_BOMB_SHOT:
			{
				Bomb_Shot* bomb_shot = (Bomb_Shot*)packet;

				CopyMemory(&BombShot_Damage, bomb_shot->BombShot_Damage, sizeof(BombShot_Damage));
				CopyMemory(&BombShot_DemonDamage, bomb_shot->BombShot_DemonDamage, sizeof(BombShot_DemonDamage));
				CopyMemory(&BombShot_AddDamage, bomb_shot->BombShot_AddDamage, sizeof(BombShot_AddDamage));
				CopyMemory(&BombShot_Area, bomb_shot->BombShot_Area, sizeof(BombShot_Area));
				CopyMemory(&BombShot_UseMana, bomb_shot->BombShot_UseMana, sizeof(BombShot_UseMana));
			}
			break;

			case SKILL_PERFORATION:
			{
				Perforation* perforation = (Perforation*)packet;

				CopyMemory(&Perforation_Damage, perforation->Perforation_Damage, sizeof(Perforation_Damage));
				CopyMemory(&Perforation_Critical, perforation->Perforation_Critical, sizeof(Perforation_Critical));
				CopyMemory(&Perforation_UseMana, perforation->Perforation_UseMana, sizeof(Perforation_UseMana));
				CopyMemory(&Perforation_Attack_Range, perforation->Perforation_Attack_Range, sizeof(Perforation_Attack_Range));
			}
			break;

			case SKILL_RECALL_WOLVERIN:
			{
				Recall_Wolverin* recall_wolverin = (Recall_Wolverin*)packet;

				CopyMemory(&Recall_Wolverin_Damage, recall_wolverin->Recall_Wolverin_Damage, sizeof(Recall_Wolverin_Damage));
				CopyMemory(&Recall_Wolverin_Hit, recall_wolverin->Recall_Wolverin_Hit, sizeof(Recall_Wolverin_Hit));
				CopyMemory(&Recall_Wolverin_Defense, recall_wolverin->Recall_Wolverin_Defense, sizeof(Recall_Wolverin_Defense));
				CopyMemory(&Recall_Wolverin_Life, recall_wolverin->Recall_Wolverin_Life, sizeof(Recall_Wolverin_Life));
				CopyMemory(&Recall_Wolverin_UseMana, recall_wolverin->Recall_Wolverin_UseMana, sizeof(Recall_Wolverin_UseMana));
			}
			break;

			case SKILL_EVASION_MASTERY:
			{
				Evasion_Mastery* evasion_mastery = (Evasion_Mastery*)packet;

				CopyMemory(&Evasion_Mastery_AddPercent, evasion_mastery->Evasion_Mastery_AddPercent, sizeof(Evasion_Mastery_AddPercent));
				CopyMemory(&Evasion_Mastery_AddSpeed, evasion_mastery->Evasion_Mastery_AddSpeed, sizeof(Evasion_Mastery_AddSpeed));
				CopyMemory(&Evasion_Mastery_AddReduce, evasion_mastery->Evasion_Mastery_AddReduce, sizeof(Evasion_Mastery_AddReduce));
				CopyMemory(&Evasion_Mastery_Time, evasion_mastery->Evasion_Mastery_Time, sizeof(Evasion_Mastery_Time));
				CopyMemory(&Evasion_Mastery_UseMana, evasion_mastery->Evasion_Mastery_UseMana, sizeof(Evasion_Mastery_UseMana));
			}
			break;

			case SKILL_PHOENIX_SHOT:
			{
				Phoenix_Shot* phoenix_shot = (Phoenix_Shot*)packet;

				CopyMemory(&Phoenix_Shot_DamagePercent, phoenix_shot->Phoenix_Shot_DamagePercent, sizeof(Phoenix_Shot_DamagePercent));
				CopyMemory(&Phoenix_Shot_Time, phoenix_shot->Phoenix_Shot_Time, sizeof(Phoenix_Shot_Time));
				CopyMemory(&Phoenix_Shot_UseMana, phoenix_shot->Phoenix_Shot_UseMana, sizeof(Phoenix_Shot_UseMana));
			}
			break;

			case SKILL_FORCE_OF_NATURE:
			{
				Force_Of_Nature* force_of_nature = (Force_Of_Nature*)packet;

				CopyMemory(&Force_Of_Nature_AddDamage, force_of_nature->Force_Of_Nature_AddDamage, sizeof(Force_Of_Nature_AddDamage));
				CopyMemory(&Force_Of_Nature_AddHit, force_of_nature->Force_Of_Nature_AddHit, sizeof(Force_Of_Nature_AddHit));
				CopyMemory(&Force_Of_Nature_FalconAddDamage, force_of_nature->Force_Of_Nature_FalconAddDamage, sizeof(Force_Of_Nature_FalconAddDamage));
				CopyMemory(&Force_Of_Nature_Time, force_of_nature->Force_Of_Nature_Time, sizeof(Force_Of_Nature_Time));
				CopyMemory(&Force_Of_Nature_UseMana, force_of_nature->Force_Of_Nature_UseMana, sizeof(Force_Of_Nature_UseMana));
			}
			break;
			}
		}
		break;

		case GROUP_KNIGHT:
		{
			switch (packet->skillId)
			{
			case SKILL_SWORD_BLAST:
			{
				Sword_Blast* sword_blast = (Sword_Blast*)packet;

				CopyMemory(&Sword_Blast_UseMana, sword_blast->Sword_Blast_UseMana, sizeof(Sword_Blast_UseMana));
				CopyMemory(&Sword_Blast_Damage, sword_blast->Sword_Blast_Damage, sizeof(Sword_Blast_Damage));
				CopyMemory(&Sword_Blast_ShootingRange, sword_blast->Sword_Blast_ShootingRange, sizeof(Sword_Blast_ShootingRange));
			}
			break;

			case SKILL_HOLY_BODY:
			{
				Holy_Body* holy_body = (Holy_Body*)packet;

				CopyMemory(&HolyBody_UseMana, holy_body->HolyBody_UseMana, sizeof(HolyBody_UseMana));
				CopyMemory(&HolyBody_Absorb, holy_body->HolyBody_Absorb, sizeof(HolyBody_Absorb));
				CopyMemory(&HolyBody_Time, holy_body->HolyBody_Time, sizeof(HolyBody_Time));
			}
			break;

			case SKILL_PHYSICAL_TRANING:
			{
				P_Traning* p_traning = (P_Traning*)packet;

				CopyMemory(&P_Traning_Stamina, p_traning->P_Traning_Stamina, sizeof(P_Traning_Stamina));
			}
			break;

			case SKILL_DOUBLE_CRASH:
			{
				D_Crash* d_crash = (D_Crash*)packet;

				CopyMemory(&D_Crash_UseMana, d_crash->D_Crash_UseMana, sizeof(D_Crash_UseMana));
				CopyMemory(&D_Crash_Damage, d_crash->D_Crash_Damage, sizeof(D_Crash_Damage));
				CopyMemory(&D_Crash_Critical, d_crash->D_Crash_Critical, sizeof(D_Crash_Critical));
			}
			break;

			case SKILL_HOLY_VALOR:
			{
				Holy_Valor* holy_valor = (Holy_Valor*)packet;

				CopyMemory(&Holy_Valor_Damage, holy_valor->Holy_Valor_Damage, sizeof(Holy_Valor_Damage));
				CopyMemory(&Holy_Valor_Time, holy_valor->Holy_Valor_Time, sizeof(Holy_Valor_Time));
				CopyMemory(&Holy_Valor_Mana, holy_valor->Holy_Valor_Mana, sizeof(Holy_Valor_Mana));
			}
			break;

			case SKILL_BRANDISH:
			{
				Brandish* brandish = (Brandish*)packet;

				CopyMemory(&Brandish_Range, brandish->Brandish_Range, sizeof(Brandish_Range));
				CopyMemory(&Brandish_Damage, brandish->Brandish_Damage, sizeof(Brandish_Damage));
				CopyMemory(&Brandish_Mana, brandish->Brandish_Mana, sizeof(Brandish_Mana));
			}
			break;

			case SKILL_PIERCING:
			{
				Piercing* piercing = (Piercing*)packet;

				CopyMemory(&Piercing_Damage, piercing->Piercing_Damage, sizeof(Piercing_Damage));
				CopyMemory(&Piercing_Attack_Rating, piercing->Piercing_Attack_Rating, sizeof(Piercing_Attack_Rating));
				CopyMemory(&Piercing_Mana, piercing->Piercing_Mana, sizeof(Piercing_Mana));
			}
			break;

			case SKILL_DRASTIC_SPIRIT:
			{
				Drastic_Spirit* drastic_spirit = (Drastic_Spirit*)packet;

				CopyMemory(&Drastic_Spirit_Defense, drastic_spirit->Drastic_Spirit_Defense, sizeof(Drastic_Spirit_Defense));
				CopyMemory(&Drastic_Spirit_Time, drastic_spirit->Drastic_Spirit_Time, sizeof(Drastic_Spirit_Time));
				CopyMemory(&Drastic_Spirit_Mana, drastic_spirit->Drastic_Spirit_Mana, sizeof(Drastic_Spirit_Mana));
			}
			break;

			case SKILL_SWORD_MASTERY:
			{
				Sword_Mastery* sword_mastery = (Sword_Mastery*)packet;

				CopyMemory(&Sword_Mastery_DamagePercent, sword_mastery->Sword_Mastery_DamagePercent, sizeof(Sword_Mastery_DamagePercent));
			}
			break;

			case SKILL_DIVINE_INHALATION:
			{
				D_Inhalation* d_inhalation = (D_Inhalation*)packet;

				CopyMemory(&D_Inhalation_Life, d_inhalation->D_Inhalation_Life, sizeof(D_Inhalation_Life));
				CopyMemory(&D_Inhalation_Block, d_inhalation->D_Inhalation_Block, sizeof(D_Inhalation_Block));
				CopyMemory(&D_Inhalation_Time, d_inhalation->D_Inhalation_Time, sizeof(D_Inhalation_Time));
				CopyMemory(&D_Inhalation_UseMana, d_inhalation->D_Inhalation_UseMana, sizeof(D_Inhalation_UseMana));
			}
			break;

			case SKILL_HOLY_INCANTATION:
			{
				H_Incantation* h_incantation = (H_Incantation*)packet;

				CopyMemory(&H_Incantation_Success, h_incantation->H_Incantation_Success, sizeof(H_Incantation_Success));
				CopyMemory(&H_Incantation_AddLife, h_incantation->H_Incantation_AddLife, sizeof(H_Incantation_AddLife));
				CopyMemory(&H_Incantation_Time, h_incantation->H_Incantation_Time, sizeof(H_Incantation_Time));
				CopyMemory(&H_Incantation_UseMana, h_incantation->H_Incantation_UseMana, sizeof(H_Incantation_UseMana));
			}
			break;

			case SKILL_GRAND_CROSS:
			{
				Grand_Cross* grand_cross = (Grand_Cross*)packet;

				CopyMemory(&Grand_Cross_Damage, grand_cross->Grand_Cross_Damage, sizeof(Grand_Cross_Damage));
				CopyMemory(&Grand_Cross_AttackRate, grand_cross->Grand_Cross_AttackRate, sizeof(Grand_Cross_AttackRate));
				CopyMemory(&Grand_Cross_UndeadDamage, grand_cross->Grand_Cross_UndeadDamage, sizeof(Grand_Cross_UndeadDamage));
				CopyMemory(&Grand_Cross_UseMana, grand_cross->Grand_Cross_UseMana, sizeof(Grand_Cross_UseMana));
			}
			break;

			case SKILL_SWORD_OF_JUSTICE:
			{
				Sword_Of_Justice* sword_of_justice = (Sword_Of_Justice*)packet;

				CopyMemory(&Sword_Of_Justice_DamagePercent, sword_of_justice->Sword_Of_Justice_DamagePercent, sizeof(Sword_Of_Justice_DamagePercent));
				CopyMemory(&Sword_Of_Justice_Area, sword_of_justice->Sword_Of_Justice_Area, sizeof(Sword_Of_Justice_Area));
				CopyMemory(&Sword_Of_Justice_UseMana, sword_of_justice->Sword_Of_Justice_UseMana, sizeof(Sword_Of_Justice_UseMana));
			}
			break;

			case SKILL_GODLY_SHIELD:
			{
				Godly_Shield* godly_shield = (Godly_Shield*)packet;

				CopyMemory(&Godly_Shield_AbsorbPercent, godly_shield->Godly_Shield_AbsorbPercent, sizeof(Godly_Shield_AbsorbPercent));
				CopyMemory(&Godly_Shield_Time, godly_shield->Godly_Shield_Time, sizeof(Godly_Shield_Time));
				CopyMemory(&Godly_Shield_UseMana, godly_shield->Godly_Shield_UseMana, sizeof(Godly_Shield_UseMana));
			}
			break;

			case SKILL_GOD_BLESS:
			{
				God_Bless* god_bless = (God_Bless*)packet;

				CopyMemory(&God_Bless_AddDamage, god_bless->God_Bless_AddDamage, sizeof(God_Bless_AddDamage));
				CopyMemory(&God_Bless_Time, god_bless->God_Bless_Time, sizeof(God_Bless_Time));
				CopyMemory(&God_Bless_UseMana, god_bless->God_Bless_UseMana, sizeof(God_Bless_UseMana));
			}
			break;

			case SKILL_DIVINE_PIERCING:
			{
				Divine_Piercing* divine_piercing = (Divine_Piercing*)packet;

				CopyMemory(&Divine_Piercing_DamagePercent, divine_piercing->Divine_Piercing_DamagePercent, sizeof(Divine_Piercing_DamagePercent));
				CopyMemory(&Divine_Piercing_AddHit, divine_piercing->Divine_Piercing_AddHit, sizeof(Divine_Piercing_AddHit));
				CopyMemory(&Divine_Piercing_AttackNum, divine_piercing->Divine_Piercing_AttackNum, sizeof(Divine_Piercing_AttackNum));
				CopyMemory(&Divine_Piercing_UseMana, divine_piercing->Divine_Piercing_UseMana, sizeof(Divine_Piercing_UseMana));

			}
			break;
			}
		}
		break;

		case GROUP_ATALANTA:
		{
			switch (packet->skillId)
			{
			case SKILL_SHIELD_STRIKE:
			{
				S_Strike* s_strike = (S_Strike*)packet;

				CopyMemory(&S_Strike_UseMana, s_strike->S_Strike_UseMana, sizeof(S_Strike_UseMana));
				CopyMemory(&S_Strike_Damage, s_strike->S_Strike_Damage, sizeof(S_Strike_Damage));
			}
			break;

			case SKILL_FARINA:
			{
				Farina* farina = (Farina*)packet;

				CopyMemory(&Farina_UseMana, farina->Farina_UseMana, sizeof(Farina_UseMana));
				CopyMemory(&Farina_Speed, farina->Farina_Speed, sizeof(Farina_Speed));
				CopyMemory(&Farina_AttackRate, farina->Farina_AttackRate, sizeof(Farina_AttackRate));
			}
			break;

			case SKILL_THROWING_MASTERY:
			{
				T_Mastery* t_mastery = (T_Mastery*)packet;

				CopyMemory(&T_Mastery_Damage, t_mastery->T_Mastery_Damage, sizeof(T_Mastery_Damage));
			}
			break;

			case SKILL_VIGOR_SPEAR:
			{
				V_Spear* v_spear = (V_Spear*)packet;

				CopyMemory(&V_Spear_UseMana, v_spear->V_Spear_UseMana, sizeof(V_Spear_UseMana));
				CopyMemory(&V_Spear_Damage, v_spear->V_Spear_Damage, sizeof(V_Spear_Damage));
			}
			break;

			case SKILL_WINDY:
			{
				Wind* wind = (Wind*)packet;

				CopyMemory(&Windy_Attack_Rating, wind->Windy_Attack_Rating, sizeof(Windy_Attack_Rating));
				CopyMemory(&Windy_Time, wind->Windy_Time, sizeof(Windy_Time));
				CopyMemory(&Windy_Mana, wind->Windy_Mana, sizeof(Windy_Mana));
			}
			break;

			case SKILL_TWIST_JAVELIN:
			{
				Twist_Javelin* twist_javelin = (Twist_Javelin*)packet;

				CopyMemory(&Twist_Javelin_Damage, twist_javelin->Twist_Javelin_Damage, sizeof(Twist_Javelin_Damage));
				CopyMemory(&Twist_Javelin_Attack_Rating, twist_javelin->Twist_Javelin_Attack_Rating, sizeof(Twist_Javelin_Attack_Rating));
				CopyMemory(&Twist_Javelin_Mana, twist_javelin->Twist_Javelin_Mana, sizeof(Twist_Javelin_Mana));
			}
			break;

			case SKILL_SOUL_SUCKER:
			{
				Soul_Sucker* soul_sucker = (Soul_Sucker*)packet;

				CopyMemory(&Soul_Sucker_Absorb, soul_sucker->Soul_Sucker_Absorb, sizeof(Soul_Sucker_Absorb));
				CopyMemory(&Soul_Sucker_Mana, soul_sucker->Soul_Sucker_Mana, sizeof(Soul_Sucker_Mana));
			}
			break;

			case SKILL_FIRE_JAVELIN:
			{
				Fire_Javelin* fire_javelin = (Fire_Javelin*)packet;

				CopyMemory(&Fire_Javelin_Damage, fire_javelin->Fire_Javelin_Damage, sizeof(Fire_Javelin_Damage));
				CopyMemory(&Fire_Javelin_Mana, fire_javelin->Fire_Javelin_Mana, sizeof(Fire_Javelin_Mana));
			}
			break;

			case SKILL_SPLIT_JAVELIN:
			{
				Split_Javelin* split_javelin = (Split_Javelin*)packet;

				CopyMemory(&Split_Javelin_AttackNum, split_javelin->Split_Javelin_AttackNum, sizeof(Split_Javelin_AttackNum));
				CopyMemory(&Split_Javelin_Damage, split_javelin->Split_Javelin_Damage, sizeof(Split_Javelin_Damage));
				CopyMemory(&Split_Javelin_AttackRate, split_javelin->Split_Javelin_AttackRate, sizeof(Split_Javelin_AttackRate));
				CopyMemory(&Split_Javelin_UseMana, split_javelin->Split_Javelin_UseMana, sizeof(Split_Javelin_UseMana));
			}
			break;

			case SKILL_TRIUMPH_OF_VALHALLA:
			{
				T_Of_Valhalla* t_of_valhalla = (T_Of_Valhalla*)packet;

				CopyMemory(&T_Of_Valhalla_Damage, t_of_valhalla->T_Of_Valhalla_Damage, sizeof(T_Of_Valhalla_Damage));
				CopyMemory(&T_Of_Valhalla_Time, t_of_valhalla->T_Of_Valhalla_Time, sizeof(T_Of_Valhalla_Time));
				CopyMemory(&T_Of_Valhalla_UseMana, t_of_valhalla->T_Of_Valhalla_UseMana, sizeof(T_Of_Valhalla_UseMana));
			}
			break;

			case SKILL_LIGHTNING_JAVELIN:
			{
				Lightning_Javelin* lightning_javelin = (Lightning_Javelin*)packet;

				CopyMemory(&Lightning_Javelin_Damage, lightning_javelin->Lightning_Javelin_Damage, sizeof(Lightning_Javelin_Damage));
				CopyMemory(&Lightning_Javelin_UseMana, lightning_javelin->Lightning_Javelin_UseMana, sizeof(Lightning_Javelin_UseMana));
			}
			break;

			case SKILL_STORM_JAVELIN:
			{
				Storm_Javelin* storm_javelin = (Storm_Javelin*)packet;

				CopyMemory(&Storm_Javelin_Damage, storm_javelin->Storm_Javelin_Damage, sizeof(Storm_Javelin_Damage));
				CopyMemory(&Storm_Javelin_UseMana, storm_javelin->Storm_Javelin_UseMana, sizeof(Storm_Javelin_UseMana));
			}
			break;

			case SKILL_HALL_OF_VALHALLA:
			{
				Hall_Of_Valhalla* hall_of_valhalla = (Hall_Of_Valhalla*)packet;

				CopyMemory(&Hall_Of_Valhalla_AddEvation, hall_of_valhalla->Hall_Of_Valhalla_AddEvation, sizeof(Hall_Of_Valhalla_AddEvation));
				CopyMemory(&Hall_Of_Valhalla_Time, hall_of_valhalla->Hall_Of_Valhalla_Time, sizeof(Hall_Of_Valhalla_Time));
				CopyMemory(&Hall_Of_Valhalla_UseMana, hall_of_valhalla->Hall_Of_Valhalla_UseMana, sizeof(Hall_Of_Valhalla_UseMana));
			}
			break;

			case SKILL_X_RAGE:
			{
				X_Rage* x_rage = (X_Rage*)packet;

				CopyMemory(&X_Rage_DamagePercent, x_rage->X_Rage_DamagePercent, sizeof(X_Rage_DamagePercent));
				CopyMemory(&X_Rage_Area, x_rage->X_Rage_Area, sizeof(X_Rage_Area));
				CopyMemory(&X_Rage_UseMana, x_rage->X_Rage_UseMana, sizeof(X_Rage_UseMana));
			}
			break;

			case SKILL_FROST_JAVELIN:
			{
				Frost_Javelin* frost_javelin = (Frost_Javelin*)packet;

				CopyMemory(&Frost_Javelin_IceAddDamage, frost_javelin->Frost_Javelin_IceAddDamage, sizeof(Frost_Javelin_IceAddDamage));
				CopyMemory(&Frost_Javelin_SpeedSubPercent, frost_javelin->Frost_Javelin_SpeedSubPercent, sizeof(Frost_Javelin_SpeedSubPercent));
				CopyMemory(&Frost_Javelin_IceTime, frost_javelin->Frost_Javelin_IceTime, sizeof(Frost_Javelin_IceTime));
				CopyMemory(&Frost_Javelin_Time, frost_javelin->Frost_Javelin_Time, sizeof(Frost_Javelin_Time));
				CopyMemory(&Frost_Javelin_UseMana, frost_javelin->Frost_Javelin_UseMana, sizeof(Frost_Javelin_UseMana));
			}
			break;

			case SKILL_VENGEANCE:
			{
				Vengeance* vengeance = (Vengeance*)packet;

				CopyMemory(&Vengeance_DamagePercent, vengeance->Vengeance_DamagePercent, sizeof(Vengeance_DamagePercent));
				CopyMemory(&Vengeance_Range, vengeance->Vengeance_Range, sizeof(Vengeance_Range));
				CopyMemory(&Vengeance_UseMana, vengeance->Vengeance_UseMana, sizeof(Vengeance_UseMana));
				CopyMemory(&Vengeance_AddCritical, vengeance->Vengeance_AddCritical, sizeof(Vengeance_AddCritical));

			}
			break;
			}
		}
		break;

		case GROUP_PRIESTESS:
		{
			switch (packet->skillId)
			{
			case SKILL_HEALING:
			{
				Healing* healing = (Healing*)packet;

				CopyMemory(&Healing_UseMana, healing->Healing_UseMana, sizeof(Healing_UseMana));
				CopyMemory(&Healing_Heal, healing->Healing_Heal, sizeof(Healing_Heal));
			}
			break;

			case SKILL_HOLY_BOLT:
			{
				HolyBolt* holybolt = (HolyBolt*)packet;

				CopyMemory(&HolyBolt_UseMana, holybolt->HolyBolt_UseMana, sizeof(HolyBolt_UseMana));
				CopyMemory(&HolyBolt_Damage, holybolt->HolyBolt_Damage, sizeof(HolyBolt_Damage));
			}
			break;

			case SKILL_MULTISPARK:
			{
				M_Spark* m_spark = (M_Spark*)packet;

				CopyMemory(&M_Spark_Damage, m_spark->M_Spark_Damage, sizeof(M_Spark_Damage));
				CopyMemory(&M_Spark_Num, m_spark->M_Spark_Num, sizeof(M_Spark_Num));
			}
			break;

			case SKILL_HOLY_MIND:
			{
				HolyMind* holymind = (HolyMind*)packet;

				CopyMemory(&HolyMind_UseMana, holymind->HolyMind_UseMana, sizeof(HolyMind_UseMana));
				CopyMemory(&HolyMind_DecDamage, holymind->HolyMind_DecDamage, sizeof(HolyMind_DecDamage));
				CopyMemory(&HolyMind_Time, holymind->HolyMind_Time, sizeof(HolyMind_Time));
			}
			break;

			case SKILL_MEDITATION:
			{
				Meditation* meditation = (Meditation*)packet;

				CopyMemory(&Meditation_Regen, meditation->Meditation_Regen, sizeof(Meditation_Regen));
			}
			break;

			case SKILL_DIVINE_LIGHTNING:
			{
				Divine_Lightning* divine_lightning = (Divine_Lightning*)packet;

				CopyMemory(&Divine_Lightning_Damage, divine_lightning->Divine_Lightning_Damage, sizeof(Divine_Lightning_Damage));
				CopyMemory(&Divine_Lightning_Num, divine_lightning->Divine_Lightning_Num, sizeof(Divine_Lightning_Num));
				CopyMemory(&Divine_Lightning_Mana, divine_lightning->Divine_Lightning_Mana, sizeof(Divine_Lightning_Mana));
			}
			break;

			case SKILL_HOLY_REFLECTION:
			{
				Holy_Reflection* holy_reflection = (Holy_Reflection*)packet;

				CopyMemory(&Holy_Reflection_Return_Damage, holy_reflection->Holy_Reflection_Return_Damage, sizeof(Holy_Reflection_Return_Damage));
				CopyMemory(&Holy_Reflection_Time, holy_reflection->Holy_Reflection_Time, sizeof(Holy_Reflection_Time));
				CopyMemory(&Holy_Reflection_Mana, holy_reflection->Holy_Reflection_Mana, sizeof(Holy_Reflection_Mana));
			}
			break;

			case SKILL_GRAND_HEALING:
			{
				GrandHealing* grandhealing = (GrandHealing*)packet;

				CopyMemory(&Grand_Healing, grandhealing->Grand_Healing, sizeof(Grand_Healing));
				CopyMemory(&Grand_Healing_Range, grandhealing->Grand_Healing_Range, sizeof(Grand_Healing_Range));
				CopyMemory(&Grand_Healing_Mana, grandhealing->Grand_Healing_Mana, sizeof(Grand_Healing_Mana));
			}
			break;

			case SKILL_VIGOR_BALL:
			{
				Vigor_Ball* vigor_ball = (Vigor_Ball*)packet;

				CopyMemory(&Vigor_Ball_Damage, vigor_ball->Vigor_Ball_Damage, sizeof(Vigor_Ball_Damage));
				CopyMemory(&Vigor_Ball_UseMana, vigor_ball->Vigor_Ball_UseMana, sizeof(Vigor_Ball_UseMana));
			}
			break;

			case SKILL_RESURRECTION:
			{
				Resurrection* resurrection = (Resurrection*)packet;

				CopyMemory(&Resurrection_Percent, resurrection->Resurrection_Percent, sizeof(Resurrection_Percent));
				CopyMemory(&Resurrection_UseMana, resurrection->Resurrection_UseMana, sizeof(Resurrection_UseMana));
			}
			break;

			case SKILL_EXTINCTION:
			{
				Extinction* extinction = (Extinction*)packet;

				CopyMemory(&Extinction_Percent, extinction->Extinction_Percent, sizeof(Extinction_Percent));
				CopyMemory(&Extinction_Amount, extinction->Extinction_Amount, sizeof(Extinction_Amount));
				CopyMemory(&Extinction_UseMana, extinction->Extinction_UseMana, sizeof(Extinction_UseMana));
			}
			break;

			case SKILL_VIRTUAL_LIFE:
			{
				Virtual_Life* virtual_life = (Virtual_Life*)packet;

				CopyMemory(&Virtual_Life_Percent, virtual_life->Virtual_Life_Percent, sizeof(Virtual_Life_Percent));
				CopyMemory(&Virtual_Life_Time, virtual_life->Virtual_Life_Time, sizeof(Virtual_Life_Time));
				CopyMemory(&Virtual_Life_UseMana, virtual_life->Virtual_Life_UseMana, sizeof(Virtual_Life_UseMana));
			}
			break;

			case SKILL_GLACIAL_SPIKE:
			{
				Glacial_Spike* glacial_spike = (Glacial_Spike*)packet;

				CopyMemory(&Glacial_Spike_Damage, glacial_spike->Glacial_Spike_Damage, sizeof(Glacial_Spike_Damage));
				CopyMemory(&Glacial_Spike_Area, glacial_spike->Glacial_Spike_Area, sizeof(Glacial_Spike_Area));
				CopyMemory(&Glacial_Spike_UseMana, glacial_spike->Glacial_Spike_UseMana, sizeof(Glacial_Spike_UseMana));
			}
			break;

			case SKILL_REGENERATION_FIELD:
			{
				Regeneration_Field* regeneration_field = (Regeneration_Field*)packet;

				CopyMemory(&Regeneration_Field_LifeRegen, regeneration_field->Regeneration_Field_LifeRegen, sizeof(Regeneration_Field_LifeRegen));
				CopyMemory(&Regeneration_Field_ManaRegen, regeneration_field->Regeneration_Field_ManaRegen, sizeof(Regeneration_Field_ManaRegen));
				CopyMemory(&Regeneration_Field_Time, regeneration_field->Regeneration_Field_Time, sizeof(Regeneration_Field_Time));
				CopyMemory(&Regeneration_Field_Area, regeneration_field->Regeneration_Field_Area, sizeof(Regeneration_Field_Area));
				CopyMemory(&Regeneration_Field_UseMana, regeneration_field->Regeneration_Field_UseMana, sizeof(Regeneration_Field_UseMana));

			}
			break;

			case SKILL_CHAIN_LIGHTNING:
			{
				Chain_Lightning* chain_lightning = (Chain_Lightning*)packet;

				CopyMemory(&Chain_Lightning_Damage, chain_lightning->Chain_Lightning_Damage, sizeof(Chain_Lightning_Damage));
				/*CopyMemory(&Chain_Lightning_Num, chain_lightning->Chain_Lightning_Num, sizeof(Chain_Lightning_Num));*/
				CopyMemory(&Chain_Lightning_Range, chain_lightning->Chain_Lightning_Range, sizeof(Chain_Lightning_Range));
				CopyMemory(&Chain_Lightning_UseMana, chain_lightning->Chain_Lightning_UseMana, sizeof(Chain_Lightning_UseMana));
			}
			break;

			case SKILL_SUMMON_MUSPELL:
			{
				Summon_Muspell* summon_muspell = (Summon_Muspell*)packet;

				CopyMemory(&Summon_Muspell_Damage, summon_muspell->Summon_Muspell_Damage, sizeof(Summon_Muspell_Damage));
				CopyMemory(&Summon_Muspell_Attack_Delay, summon_muspell->Summon_Muspell_Attack_Delay, sizeof(Summon_Muspell_Attack_Delay));
				CopyMemory(&Summon_Muspell_UndeadAbsorbPercent, summon_muspell->Summon_Muspell_UndeadAbsorbPercent, sizeof(Summon_Muspell_UndeadAbsorbPercent));
				CopyMemory(&Summon_Muspell_BlockPercent, summon_muspell->Summon_Muspell_BlockPercent, sizeof(Summon_Muspell_BlockPercent));
				CopyMemory(&Summon_Muspell_Time, summon_muspell->Summon_Muspell_Time, sizeof(Summon_Muspell_Time));
				CopyMemory(&Summon_Muspell_UseMana, summon_muspell->Summon_Muspell_UseMana, sizeof(Summon_Muspell_UseMana));
			}
			break;
			}
		}
		break;

		case GROUP_MAGICIAN:
		{
			switch (packet->skillId)
			{
			case SKILL_AGONY:
			{
				Agony* agony = (Agony*)packet;

				CopyMemory(&Agony_UseMana, agony->Agony_UseMana, sizeof(Agony_UseMana));
				CopyMemory(&Agony_ConvLife, agony->Agony_ConvLife, sizeof(Agony_ConvLife));
			}
			break;

			case SKILL_FIRE_BOLT:
			{
				FireBolt* firebolt = (FireBolt*)packet;

				CopyMemory(&FireBolt_UseMana, firebolt->FireBolt_UseMana, sizeof(FireBolt_UseMana));
				CopyMemory(&FireBolt_Damage, firebolt->FireBolt_Damage, sizeof(FireBolt_Damage));
			}
			break;

			case SKILL_ZENITH:
			{
				Zenith* zenith = (Zenith*)packet;

				CopyMemory(&Zenith_UseMana, zenith->Zenith_UseMana, sizeof(Zenith_UseMana));
				CopyMemory(&Zenith_Element, zenith->Zenith_Element, sizeof(Zenith_Element));
				CopyMemory(&Zenith_Time, zenith->Zenith_Time, sizeof(Zenith_Time));

			}
			break;

			case SKILL_FIRE_BALL:
			{
				FireBall* fireball = (FireBall*)packet;

				CopyMemory(&FireBall_UseMana, fireball->FireBall_UseMana, sizeof(FireBall_UseMana));
				CopyMemory(&FireBall_Damage, fireball->FireBall_Damage, sizeof(FireBall_Damage));
				CopyMemory(&FireBall_Area, fireball->FireBall_Area, sizeof(FireBall_Area));
				CopyMemory(&FireBall_Range, fireball->FireBall_Range, sizeof(FireBall_Range));
			}
			break;

			case SKILL_MENTAL_MASTERY:
			{
				Mental_Mastery* mental_mastery = (Mental_Mastery*)packet;

				CopyMemory(&Mental_Mastery_Mana, mental_mastery->Mental_Mastery_Mana, sizeof(Mental_Mastery_Mana));
			}
			break;

			case SKILL_WATORNADO:
			{
				Watornado* watornado = (Watornado*)packet;

				CopyMemory(&Watornado_Damage, watornado->Watornado_Damage, sizeof(Watornado_Damage));
				CopyMemory(&Watornado_Area, watornado->Watornado_Area, sizeof(Watornado_Area));
				CopyMemory(&Watornado_Range, watornado->Watornado_Range, sizeof(Watornado_Range));
				CopyMemory(&Watornado_Mana, watornado->Watornado_Mana, sizeof(Watornado_Mana));
			}
			break;

			case SKILL_ENCHANT_WEAPON:
			{
				Enchant_Weapon* enchant_weapon = (Enchant_Weapon*)packet;

				CopyMemory(&Enchant_Weapon_Damage_Ice, enchant_weapon->Enchant_Weapon_Damage_Ice, sizeof(Enchant_Weapon_Damage_Ice));
				CopyMemory(&Enchant_Weapon_Damage_Fire, enchant_weapon->Enchant_Weapon_Damage_Fire, sizeof(Enchant_Weapon_Damage_Fire));
				CopyMemory(&Enchant_Weapon_Damage_Lightning, enchant_weapon->Enchant_Weapon_Damage_Lightning, sizeof(Enchant_Weapon_Damage_Lightning));
				CopyMemory(&Enchant_Weapon_Time, enchant_weapon->Enchant_Weapon_Time, sizeof(Enchant_Weapon_Time));
				CopyMemory(&Enchant_Weapon_Mana, enchant_weapon->Enchant_Weapon_Mana, sizeof(Enchant_Weapon_Mana));
			}
			break;

			case SKILL_DEAD_RAY:
			{
				Dead_Ray* dead_ray = (Dead_Ray*)packet;

				CopyMemory(&Dead_Ray_Damage, dead_ray->Dead_Ray_Damage, sizeof(Dead_Ray_Damage));
				CopyMemory(&Dead_Ray_Range, dead_ray->Dead_Ray_Range, sizeof(Dead_Ray_Range));
				CopyMemory(&Dead_Ray_Mana, dead_ray->Dead_Ray_Mana, sizeof(Dead_Ray_Mana));
			}
			break;

			case SKILL_ENERGY_SHIELD:
			{
				Energy_Shield* energy_shield = (Energy_Shield*)packet;

				CopyMemory(&Energy_Shield_DecDamage, energy_shield->Energy_Shield_DecDamage, sizeof(Energy_Shield_DecDamage));
				CopyMemory(&Energy_Shield_Time, energy_shield->Energy_Shield_Time, sizeof(Energy_Shield_Time));
				CopyMemory(&Energy_Shield_UseMana, energy_shield->Energy_Shield_UseMana, sizeof(Energy_Shield_UseMana));
			}
			break;

			case SKILL_DIASTROPHISM:
			{
				Diastrophism* diastrophism = (Diastrophism*)packet;

				CopyMemory(&Diastrophism_Damage, diastrophism->Diastrophism_Damage, sizeof(Diastrophism_Damage));
				CopyMemory(&Diastrophism_Area, diastrophism->Diastrophism_Area, sizeof(Diastrophism_Area));
				CopyMemory(&Diastrophism_UseMana, diastrophism->Diastrophism_UseMana, sizeof(Diastrophism_UseMana));
			}
			break;

			case SKILL_SPIRIT_ELEMENTAL:
			{
				Spirit_Elemental* spirit_elemental = (Spirit_Elemental*)packet;

				CopyMemory(&Spirit_Elemental_RegenMana, spirit_elemental->Spirit_Elemental_RegenMana, sizeof(Spirit_Elemental_RegenMana));
				CopyMemory(&Spirit_Elemental_Damage, spirit_elemental->Spirit_Elemental_Damage, sizeof(Spirit_Elemental_Damage));
				CopyMemory(&Spirit_Elemental_Time, spirit_elemental->Spirit_Elemental_Time, sizeof(Spirit_Elemental_Time));
				CopyMemory(&Spirit_Elemental_UseMana, spirit_elemental->Spirit_Elemental_UseMana, sizeof(Spirit_Elemental_UseMana));
			}
			break;

			case SKILL_DANCING_SWORD:
			{
				Dancing_Sword* dancing_sword = (Dancing_Sword*)packet;

				CopyMemory(&Dancing_Sword_IceDamage, dancing_sword->Dancing_Sword_IceDamage, sizeof(Dancing_Sword_IceDamage));
				CopyMemory(&Dancing_Sword_FireDamage, dancing_sword->Dancing_Sword_FireDamage, sizeof(Dancing_Sword_FireDamage));
				CopyMemory(&Dancing_Sword_AttackDelay, dancing_sword->Dancing_Sword_AttackDelay, sizeof(Dancing_Sword_AttackDelay));
				CopyMemory(&Dancing_Sword_Time, dancing_sword->Dancing_Sword_Time, sizeof(Dancing_Sword_Time));
				CopyMemory(&Dancing_Sword_UseMana, dancing_sword->Dancing_Sword_UseMana, sizeof(Dancing_Sword_UseMana));
			}
			break;

			case SKILL_FIRE_ELEMENTAL:
			{
				Fire_Elemental* fire_elemental = (Fire_Elemental*)packet;

				CopyMemory(&Fire_Elemental_Damage, fire_elemental->Fire_Elemental_Damage, sizeof(Fire_Elemental_Damage));
				CopyMemory(&Fire_Elemental_Hit, fire_elemental->Fire_Elemental_Hit, sizeof(Fire_Elemental_Hit));
				CopyMemory(&Fire_Elemental_Life, fire_elemental->Fire_Elemental_Life, sizeof(Fire_Elemental_Life));
				CopyMemory(&Fire_Elemental_UseMana, fire_elemental->Fire_Elemental_UseMana, sizeof(Fire_Elemental_UseMana));
			}
			break;

			case SKILL_FLAME_WAVE:
			{
				Flame_Wave* flame_wave = (Flame_Wave*)packet;

				CopyMemory(&Flame_Wave_FireDamage, flame_wave->Flame_Wave_FireDamage, sizeof(Flame_Wave_FireDamage));
				CopyMemory(&Flame_Wave_Area, flame_wave->Flame_Wave_Area, sizeof(Flame_Wave_Area));
				CopyMemory(&Flame_Wave_UseMana, flame_wave->Flame_Wave_UseMana, sizeof(Flame_Wave_UseMana));
			}
			break;

			case SKILL_DISTORTION:
			{
				Distortion* distortion = (Distortion*)packet;

				CopyMemory(&Distortion_SpeedSubPercent, distortion->Distortion_SpeedSubPercent, sizeof(Distortion_SpeedSubPercent));
				CopyMemory(&Distortion_DamageSubPercent, distortion->Distortion_DamageSubPercent, sizeof(Distortion_DamageSubPercent));
				CopyMemory(&Distortion_Area, distortion->Distortion_Area, sizeof(Distortion_Area));
				CopyMemory(&Distortion_Time, distortion->Distortion_Time, sizeof(Distortion_Time));
				CopyMemory(&Distortion_UseMana, distortion->Distortion_UseMana, sizeof(Distortion_UseMana));
			}
			break;

			case SKILL_M_METEO:
			{
				Meteo* meteo = (Meteo*)packet;

				CopyMemory(&Meteo_Damage, meteo->Meteo_Damage, sizeof(Meteo_Damage));
				CopyMemory(&Meteo_Area, meteo->Meteo_Area, sizeof(Meteo_Area));
				CopyMemory(&Meteo_UseMana, meteo->Meteo_UseMana, sizeof(Meteo_UseMana));
			}
			break;
			}
		}
		break;
		}

	}
	break;
	case smTRANSCODE_OPEN_NPC:
	{
		QuestWindow::GetInstance()->openFlag = TRUE;
	}
	break;
	case smTRANSCODE_GET_QUEST_STATUS:
	{
		Quest::GetInstance()->getAllStatus((QUEST_INFOPCKG*)pData->Buff);
	}
	break;
	case smTRANSCODE_GET_ALL_QUESTS:
	{
		Quest::GetInstance()->getAllQuests((QUEST_COMPRESSEDPCKG*)pData->Buff);
	}
	break;
	case smTRANSCODE_ADD_EXP_QUEST:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;
		setexp();
		AddExp((INT64)smPacket->LParam);
		break;
	}
	case NewShopItems_ReceiveItems:
		NewShop::GetInstance()->ReceiveItems((NEWSHOP_COMPRESSEDPCKG*)pData->Buff);
		break;
	case PACKET_SEND_COIN_GAME:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		NewShop::GetInstance()->RecvCoin(smPacket->WParam);
	}
	break;
	case NewShopItems_ReceiveItems_Time:
		NewShopTime::GetInstance()->ReceiveItems((NewShopTime_COMPRESSEDPCKG*)pData->Buff);
		break;
	case PACKET_SEND_TIME_GAME:
		NewShopTime::GetInstance()->ReceiveTime((sCoinPlayerTime*)pData->Buff);
		break;
	case 0x50700009:
	{
		MixWindow::GetInstance()->ReadMix();
		break;
	}
	case 0x50600008:
	{
		// Ao invés de ter que enviar a struct de todas as quest, envia somente 1 flag pra quest que foi completada
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;
		extern QUEST_INFOPCKG questInfoPlayer;
		if (questInfoPlayer.QuestPckg[smPacket->LParam].questID)
		{
			questInfoPlayer.QuestPckg[smPacket->LParam].questProgress[0] = smPacket->WParam;
			questInfoPlayer.QuestPckg[smPacket->LParam].questProgress[1] = smPacket->EParam;
			questInfoPlayer.QuestPckg[smPacket->LParam].questProgress[2] = smPacket->SParam;
			questInfoPlayer.QuestPckg[smPacket->LParam].isActive = 0;
			questInfoPlayer.QuestPckg[smPacket->LParam].isReadyToComplete = 1;
		}
		break;
	}
	case 0x50600009:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;
		smCHAR* Monster = FindAutoPlayer(smPacket->LParam);
		Quest::GetInstance()->updateStatus(Monster);
		break;
	}
	case 0x50600010:
	{
		// Atualiza os kills do pvp
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;
		extern QUEST_INFOPCKG questInfoPlayer;
		questInfoPlayer.QuestPckg[smPacket->LParam].killProgress += 1;
		break;
	}
	break;
	// Estágio do Covil
	case 0x50600019:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		extern int StageEragonLair;
		StageEragonLair = smPacket->LParam;
	}
	break;
	case 0x50600020:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;
		int timer = GetTickCount();

		CreateBellatraFontEffect();
		BellatraEffectInitFlag = TRUE;
		sSodScore.EffectStartCount = 70 * 3;
		sSodScore.EffectCode = sSodScore.NextRound;
		sprintf_s(szSOD_String, sizeof(szSOD_String), "Ganhou %d Coins!!!", smPacket->LParam);
		bSOD_StringColor[0] = 128;
		bSOD_StringColor[1] = 255;
		bSOD_StringColor[2] = 128;
		SOD_StringCount = 256;

		extern void deleteCoinFromInventory();
		deleteCoinFromInventory();
	}
	break;
	case 0x50600021:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		extern int tickCountBoss;
		tickCountBoss = smPacket->LParam;
		extern int equipeVencedora;
		equipeVencedora = smPacket->WParam;

		lpCurPlayer->nEquipeArena = equipeVencedora;

		EventoArena::GetInstance()->stageArena = 10;
	}
	break;
	case 0x50600022:
	{
		EventoArena::GetInstance()->stageArena = 11;
	}
	break;
	case 0x50600023:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		extern int horaHost;
		extern int minHost;
		extern int secHost;
		extern int diaHost;

		horaHost = smPacket->LParam;
		minHost = smPacket->WParam;
		secHost = smPacket->EParam;
		diaHost = smPacket->SParam;

		EventoArena::GetInstance()->openFlag = TRUE;
	}
	break;
	case 0x50600024:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		extern int horaHost;
		extern int minHost;
		extern int secHost;
		extern int diaHost;

		horaHost = smPacket->LParam;
		minHost = smPacket->WParam;
		secHost = smPacket->EParam;
		diaHost = smPacket->SParam;
	}
	break;
	case 0x6A6A0006:
	{
		pcAntiCheat->ReceivePacketHandler((PacketChecksumFunctionList*)pData->Buff);
	}
	break;
	case 0x6A6A000E:
	{
		pcAntiCheat->HandlePacket((PacketWindowList*)pData->Buff);
	}
	break;
	case NewShopItems_CheckNick:
		extern int nickIsAvailable;

		{
			smTRANS_COMMAND* smPacket;
			smPacket = (smTRANS_COMMAND*)pData->Buff;

			if (smPacket->LParam == 0)
				nickIsAvailable = 1;
			else
				nickIsAvailable = 2;
		}

		break;
	case NewShopItems_ChangeNick:
		extern char NewNick[30];
		{
			sprintf(lpCurPlayer->smCharInfo.szName, NewNick);
		}

		break;
	case OPEN_SODWINDOW_NPC:
		SodWindow::GetInstance()->RetrieveSodRanking((RankingSodPckg*)pData->Buff);
		break;
	case ROLETA_RECEIVE_TOPPLAYERS:
		Roleta::GetInstance()->ReceiveTopPlayers((RoletaPckg*)pData->Buff);
		break;
	case ROLETA_ENTRAR_RODADA:
		Roleta::GetInstance()->UpdatePlayers((RoletaPckg*)pData->Buff);
		break;
	case ROLETA_DIVIDIR_IGUALMENTE:
		Roleta::GetInstance()->dividirIgualmente = true;
		break;
	case ROLETA_PARTICIPAR_RODADA:
	{
		struct PckItem
		{
			int size;
			int code;
			int chance;
			char itemID[32];
			BYTE trans[sizeof(sITEM)];

			PckItem()
			{
				size = 0;
				code = 0;
				chance = 0;
				ZeroMemory(itemID, sizeof(itemID));
				ZeroMemory(trans, sizeof(sITEM));
			}
		};

		{
			// Limpa os valores da roleta
			extern float delayRoleta;
			delayRoleta = 100;

			extern BOOL bIsPerfect;
			bIsPerfect = FALSE;

			extern bool showDetailsRoleta;
			showDetailsRoleta = FALSE;

			Roleta::GetInstance()->participarRodada = FALSE;

			extern bool participante;
			participante = FALSE;

			extern RoletaPckg sPlayers;
			ZeroMemory(&sPlayers, sizeof(RoletaPckg));
		}


		PckItem* smPacket;
		smPacket = (PckItem*)pData->Buff;

		char itemId[32] = { 0 };
		sprintf_s(itemId, sizeof(itemId), "it%s.bmp", smPacket->itemID);

		DecodeCompress((BYTE*)&smPacket->trans, (BYTE*)&TempPerfectItem);
		bIsPerfect = TRUE;

		Roleta::GetInstance()->updateItemImage(itemId);

		Roleta::GetInstance()->openFlag = false;
		Roleta::GetInstance()->openRoleta = true;
		Roleta::GetInstance()->participarRodada = true;
	}
	break;
	case RETRIEVE_SOD_MONEY:
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		sinPlusMoney(smPacket->LParam);
		ReformCharForm();
		AddInvenMoney(smPacket->LParam);
		break;
	case PACKET_ADD_MONEY: // Ouro pego pela caravana
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		if (smPacket->LParam)
		{
			sinPlusMoney(smPacket->LParam);
			AddInvenMoney(smPacket->LParam);
		}
	}
	break;
	case 0x44550005: // OURO INICIAL
		sinPlusMoney(50000);
		ReformCharForm();
		AddInvenMoney(50000);

		/*extern void CompleteAllQuests();
		CompleteAllQuests();*/

		chaPremiumitem.SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 30);
		chaPremiumitem.SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_ONEDAY, true, UpKeepItemName[6], 20);
		chaPremiumitem.SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS, SINITEM_TIME_ONEDAY, true, UpKeepItemName[2]);
		chaPremiumitem.SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[4]);
		chaPremiumitem.SetUpKeepItem(nsPremiumItem::STAMINA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[12], 10);
		chaPremiumitem.SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 10);

		chaPremiumitem.SetPhenixPetTime(SINITEM_TIME_ONEDAY);
		cPCBANGPet.PetKind = TRUE;
		cPCBANGPet.ShowPet();

		SaveGameData();

		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x44550005;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.LParam = 0;
		smTransCommand.EParam = 0;

		if (smWsockDataServer)
			smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		break;
	case 0x43550001:
	{
		smTRANS_COMMAND* smPacket;
		smPacket = (smTRANS_COMMAND*)pData->Buff;

		int iCount = 0;
		int iTimeLeft = smPacket->WParam;
		int iItemType = smPacket->SParam;

		if (iItemType > 0 && iTimeLeft > 0)
		{
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY, true, UpKeepItemName[13], 30);
			chaPremiumitem.SetItemPremiumTime(nsPremiumItem::BIGHEAD1, iTimeLeft);

			chaPremiumitem.SetBigHeadType(iItemType);
			cInvenTory.SetItemToChar();

			switch (chaPremiumitem.GetBigHeadType())
			{
			case 1:
				ChangeHairModel(13);
				break;
			case 2:
				//ChangeHairModel( 14 );
				break;
			case 3:
				//ChangeHairModel( 15 );
				break;
			case 4:
				//ChangeHairModel( 16 );
				break;
			}
		}
	}
	break;
	case 0x43550008:
		chaPremiumitem.SetBigHead1Time(0);
		chaPremiumitem.SetBigHead3Time(0);
		chaPremiumitem.SetBigHead4Time(0);
		chaPremiumitem.SetBigHeadTime(0);
		chaPremiumitem.SetBigHeadType(0);

		extern void ResetHairModel();
		ResetHairModel();

		cInvenTory.SetItemToChar();
		break;
	case 0x43600004:
	{
		struct Caravan_Info
		{
			char caravanName[32];
			int caravanType;
			int timeLeft;
		};

		struct Caravan_Info_Pckg {
			int size;
			int code;
			Caravan_Info Pckg;
		};

		Caravan_Info_Pckg* smPacket;
		smPacket = (Caravan_Info_Pckg*)pData->Buff;

		Caravana::GetInstance()->myCaravan.caravanName = smPacket->Pckg.caravanName;
		Caravana::GetInstance()->myCaravan.timeRemaining = smPacket->Pckg.timeLeft;

		switch (smPacket->Pckg.caravanType)
		{
		case 0:
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::CARAVAN_ARMA, smPacket->Pckg.timeLeft, true, smPacket->Pckg.caravanName, 0);
			break;
		case 1:
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::CARAVAN_HOPY, smPacket->Pckg.timeLeft, true, smPacket->Pckg.caravanName, 0);
			break;
		case 2:
			chaPremiumitem.SetUpKeepItem(nsPremiumItem::CARAVAN_BUMA, smPacket->Pckg.timeLeft, true, smPacket->Pckg.caravanName, 0);
			break;
		}
	}
	break;
	case smTRANSCODE_PLAYLIST:
		break;

	case smTRANSCODE_PLAYERINFO:
		lpTransPlayerInfo = (smTRNAS_PLAYERINFO*)pData->Buff;

		lpChar = FindAutoPlayer(lpTransPlayerInfo->dwObjectSerial);

		if (lpChar)
		{
			lpChar->RecvPlayData(pData);
		}

		if (lpTransPlayerInfo->smCharInfo.State == smCHAR_STATE_ENEMY && lpTransPlayerInfo->smCharInfo.Brood != smCHAR_MONSTER_USER)
			SaveCharInfoCache(lpTransPlayerInfo);

		break;

	case smTRANSCODE_PLAYERINFO_Q:
		lpChar = FindAutoPlayer(((smTRNAS_PLAYERINFO_QUICK*)pData->Buff)->dwObjectSerial);

		if (lpChar)
		{
			lpChar->RecvPlayData(pData);
		}
		break;

	case smTRANSCODE_PLAYDATAGROUP:
		smPLAYDATA_GROUP* lpPlayDataGroup;
		char* lpTransBuff;

		lpPlayDataGroup = (smPLAYDATA_GROUP*)pData->Buff;
		lpTransBuff = pData->Buff + sizeof(smPLAYDATA_GROUP);

		if (!(lpPlayDataGroup->dwSendCode & 0x80000000)) {
			rsRecvUserServer = TRUE;
		}

		ServerCode = 0;
		RcvCnt = 0;

		if (smWsockUserServer == pData->smMySock) {
			ServerCode = PLAY_SERVER_CODE_USER;
		}
		else {
			if (smWsockExtendServer == pData->smMySock)
				ServerCode = PLAY_SERVER_CODE_EXTEND;
			else
				ServerCode = PLAY_SERVER_CODE_MAIN;
		}

		if (AreaServerMode && ServerCode == PLAY_SERVER_CODE_MAIN) {
			if (lpWSockServer_DispArea[0] == pData->smMySock) {
				ServerCode = PLAY_SERVER_CODE_AREA1;
			}
			else  if (lpWSockServer_DispArea[1] == pData->smMySock) {
				ServerCode = PLAY_SERVER_CODE_AREA2;
			}
		}

		// Recebe informações dos npcs próximos e outros jogadores
		for (cnt = 0; cnt < lpPlayDataGroup->PlayDataCnt; cnt++) {

			lpPlayData1 = (smPLAYDATA*)lpTransBuff;
			lpChar = FindAutoPlayer(lpPlayData1->dwObjectSerial);

			if (lpChar)
			{
				lpChar->RecvPlayData2(lpTransBuff);
				if (lpChar->PlayBuffCnt < (lpChar->PlayBuffPosi_End - 70)) { // teste aqui
					lpChar->PlayBuffCnt = lpChar->PlayBuffPosi_End - 70;
				}
			}
			else
			{
				if (RcvCnt < 8 || (RcvCnt < 16 && ((int*)lpTransBuff)[1] == smTRANSCODE_TRANSPLAYDATA))
				{
					RcvCnt++;
					lpChar = FindEmptyChar();
					if (lpChar)
					{
						lpChar->Init();
						lpChar->Flag = TRUE;
						lpChar->smCharInfo.szName[0] = 0;
						lpChar->ActionPattern = 99;
						lpChar->Pattern = 0;
						lpChar->AutoPlayer = TRUE;
						lpChar->TransSock = 0;
						lpChar->AutoPlayer = TRUE;
						lpChar->DisplayFlag = FALSE;
						lpChar->MotionInfo = 0;

						lpChar->dwObjectSerial = lpPlayData1->dwObjectSerial;
						lpChar->ServerCode = ServerCode;

						lpChar->ResetAttackTrans();
						lpTransPlayerInfo = GetCharInfoCache(lpTransBuff);

						if (lpTransPlayerInfo)
						{
							lpChar->SetTransPlayerInfo(lpTransPlayerInfo);
						}
						else
						{
							TransCommand.code = smTRANSCODE_GETPLAYERINFO;
							TransCommand.size = sizeof(smTRANS_COMMAND);
							TransCommand.WParam = lpPlayData1->dwObjectSerial;

							if (pData->smMySock->sock) {
								pData->smMySock->Send((char*)&TransCommand, TransCommand.size, TRUE);
							}
						}
					}
				}
			}
			lpTransBuff += lpPlayData1->size;
		}

		lpTransCommand = (smTRANS_COMMAND*)lpTransBuff;
		if (lpTransCommand->code == smTRANSCODE_PLAY_COMMAND && lpTransCommand->WParam == lpCurPlayer->dwObjectSerial) {
			//Áö·É Àü´Þ‰çÀ½
			switch (lpTransCommand->LParam) {
			case smCOMMAND_PLAY_BABEL:
				//¹Ùº§ ÃâÇöÁß
				dwBabelPlayTime = dwPlayTime + 5000;		//¹Ùº§À¯Áö½Ã°£ 5ÃÊ ¼³Á¤
				BabelPlayField = lpTransCommand->SParam;
				break;
				/*
									case smCOMMAND_PLAY_KELVEZU:
										//ÄÌº£Áê ÃâÇöÁß
										dwKelvezuPlayTime = dwPlayTime+5000;		//ÄÌº£ÁêÀ¯Áö½Ã°£ 5ÃÊ ¼³Á¤
										KelvezuPlayField = lpTransCommand->SParam;
										break;
				*/
			case smCOMMAND_PLAY_PK_FIELD:
				PK_FieldState.dwPlay_FieldTime = dwPlayTime + 10000;
				PK_FieldState.FieldCode = lpTransCommand->SParam;
				break;

			case smCOMMAND_PLAY_BELSS_CASTLE:
				//ºí·¹½º Ä³½½
				memcpy(&rsBlessCastleOld, &rsBlessCastle, sizeof(rsBLESS_CASTLE));

				lpTransCommandEx = (smTRANS_COMMAND_EX*)lpTransBuff;

				rsBlessCastle.CastleMode = lpTransCommandEx->SParam >> 16;
				rsBlessCastle.DefenceLevel = lpTransCommandEx->SParam & 0xFFFF;
				rsBlessCastle.dwMasterClan = lpTransCommandEx->EParam;
				((int*)rsBlessCastle.TimeSec)[0] = lpTransCommandEx->WxParam;
				((int*)rsBlessCastle.bCrystalSolderCount)[0] = lpTransCommandEx->LxParam;

				chaSiege.ShowPlayTime(rsBlessCastle.TimeSec[1] - rsBlessCastle.TimeSec[0]);

				//ºí·¹½ºÄ³½½ µ¥¹ÌÁö ¿Í Å³¸µ Ä«¿îÆ®
				lpCurPlayer->sBlessCastle_Damage[0] += lpTransCommandEx->SxParam & 0xFFFF;
				lpCurPlayer->sBlessCastle_Damage[1] += lpTransCommandEx->SxParam >> 16;

				if (rsBlessCastle.CastleMode == 3) {
					if (rsBlessCastleOld.CastleMode == 1) {
						//ºí·¡½º Ä³½½ Á¾·á Àá½Ã´ë±â
						RestartPlayCount = 400;		//6ÃÊ µ¿¾È ¹«Àû
						StopBGM();
						esPlayContSound(14);
						chaSiege.ShowExitMessage();		//°ø¼ºÀü Á¾·á½Ã ¸Þ¼¼Áö

						if (rsBlessCastle.dwMasterClan && rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan)) {
							lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);
						}
					}
				}
				else if (rsBlessCastleOld.CastleMode && rsBlessCastle.CastleMode == 0) {
					//¿ÏÀüÁ¾·á 
					if (!lpCurPlayer->smCharInfo.ClassClan || rsBlessCastle.dwMasterClan != GetClanCode(lpCurPlayer->smCharInfo.ClassClan)) {
						//Å¬·£ÁÖÀÎ º¯°æµÆÀ½ ¼º¹ÛÀ¸·Î ³ª°¡±â
						WarpField2(rsCASTLE_FIELD);
					}
				}

				//¼Ó¼ºÁ¤º¸ 1ºÐ °£°ÝÀ¸·Î º¸³¿
				if ((dwResistance_SendingTime + 1000 * 60) < dwPlayTime) {
					SendResistanceToServer();
				}

				if (rsBlessCastle.dwMasterClan != rsBlessCastleOld.dwMasterClan) {
					//¼ºÁÖ ¹Ù²åÀ½
					//Ä³½½ ¸¶½ºÅÍ ¾÷µ¥ÀÌÆ®
					UpdateCastleMasterClan(rsBlessCastle.dwMasterClan);
				}


				break;
			}
		}
		break;

	case smTRANSCODE_ATTACKDATA2:
#ifdef _USE_DYNAMIC_ENCODE
		if (fnDecodeDamagePacket)
			fnDecodeDamagePacket(0, pData->Buff);

		if (((TRANS_ATTACKDATA*)pData->Buff)->dwDamageChkSum != dm_GetDamgeChkSum_S2V((TRANS_ATTACKDATA*)pData->Buff)) {
			SendSetHackUser3(8580, ((TRANS_ATTACKDATA*)pData->Buff)->Power, (DWORD)fnDecodeDamagePacket);
			break;
		}
#else
		dm_DecodePacket(0, pData->Buff);

		if (((TRANS_ATTACKDATA*)pData->Buff)->dwDamageChkSum != dm_GetDamgeChkSum_S2V((TRANS_ATTACKDATA*)pData->Buff))
		{
			SendSetHackUser3(8580, ((TRANS_ATTACKDATA*)pData->Buff)->Power, (DWORD)dm_DecodePacket);
			break;
		}
#endif
	case smTRANSCODE_ATTACKDATA:
		CheckInvenItemToServer();		//ÀÎº¥Åä¸®ÀÇ ÀåÂøµÈ ¾ÆÀÌÅÛÀ» ¼­¹ö·Î º¸³»¼­ °Ë»ç
		Record_RecvDamage(pData->smMySock, ((TRANS_ATTACKDATA*)pData->Buff)->Power); //¹ÞÀº °ø°Ý·Â ±â·Ï

	case smTRANSCODE_PLAYDATA1:
	case smTRANSCODE_PLAYDATA2:
	case smTRANSCODE_PLAYDATA3:
	case smTRANSCODE_TRANSPLAYDATA:
		//¿ÀÅä ÇÃ·¹ÀÌ µ¥ÀÌÅ¸ ÀÔ¼ö

		ServerCode = 0;

		if (smWsockUserServer == pData->smMySock) {
			ServerCode = PLAY_SERVER_CODE_USER;
		}
		else {
			if (smWsockExtendServer != smWsockServer && smWsockExtendServer == pData->smMySock)
				ServerCode = PLAY_SERVER_CODE_EXTEND;
			else
				ServerCode = PLAY_SERVER_CODE_MAIN;
		}


		if (AreaServerMode && ServerCode == PLAY_SERVER_CODE_MAIN) {
			if (lpWSockServer_DispArea[0] == pData->smMySock) {
				ServerCode = PLAY_SERVER_CODE_AREA1;
			}
			else  if (lpWSockServer_DispArea[1] == pData->smMySock) {
				ServerCode = PLAY_SERVER_CODE_AREA2;
			}
		}


		lpPlayData1 = (smPLAYDATA*)pData->Buff;
		lpChar = FindAutoPlayer(lpPlayData1->dwObjectSerial);
		if (lpChar) {
			lpChar->RecvPlayData(pData);
			if (lpChar->PlayBuffCnt < (lpChar->PlayBuffPosi_End - 70)) {
				lpChar->PlayBuffCnt = lpChar->PlayBuffPosi_End - 70;
			}
		}
		/*
					else {
						if ( dwGetCharInfoTime && (dwGetCharInfoTime+300)>dwPlayTime ) break;

						dwGetCharInfoTime = dwPlayTime;

						lpChar = FindEmptyChar();
						if ( lpChar ) {
							//Ä³¸¯ÅÍ ½Å±Ô µî·Ï
							lpChar->Init();
							lpChar->Flag = TRUE;
							lpChar->smCharInfo.szName[0]=0;
							lpChar->ActionPattern = 99;
							lpChar->Pattern = 0;
							lpChar->AutoPlayer = TRUE;
							lpChar->TransSock = 0;
							lpChar->AutoPlayer = TRUE;

							lpChar->dwObjectSerial = lpPlayData1->dwObjectSerial;
							lpChar->ServerCode = ServerCode;

							//¿©±â´Ù°¡ ¸ðµ¨ ¹× Ä³¸¯ÅÍ ¼³Á¤ Á¤º¸¸¦ º¸³»´Þ¶ó±¸ ¼­¹ö¿¡ ¿ä±¸ÇÏ´Â
							//°ÍÀ» Ãß°¡ ÇØ¾ßÇÔ


		//					Send_GetCharInfo( lpPlayData1->dwObjectSerial );
						}

					}
		*/
		break;

	case smTRANSCODE_ATTACK_SLASH_LIST:
		//Æ¨°Ü¼­ ¸Â´Â µ¥¹ÌÁö À¯Àú ¸ñ·Ï µµÂø
		memcpy(&Trans_SplashCharList, pData->Buff, sizeof(TRANS_SKIL_ATTACKDATA));
		break;


	case smTRANSCODE_PLAYITEM:
		smTRANS_ITEMS* lpTransItems;
		scITEM* lpScItem;

		lpTransItems = (smTRANS_ITEMS*)pData->Buff;

		if (smWsockUserServer == pData->smMySock) {
			ServerCode = PLAY_SERVER_CODE_USER;
		}
		else {
			if (smWsockExtendServer != smWsockServer && smWsockExtendServer == pData->smMySock)
				ServerCode = PLAY_SERVER_CODE_EXTEND;
			else
				ServerCode = PLAY_SERVER_CODE_MAIN;
		}

		if (AreaServerMode && ServerCode == PLAY_SERVER_CODE_MAIN) {
			if (lpWSockServer_DispArea[0] == pData->smMySock) {
				ServerCode = PLAY_SERVER_CODE_AREA1;
			}
			else  if (lpWSockServer_DispArea[1] == pData->smMySock) {
				ServerCode = PLAY_SERVER_CODE_AREA2;
			}
		}

		for (cnt = 0; cnt < lpTransItems->Count; cnt++) {
			lpScItem = FindScItem(lpTransItems->Items[cnt].x, lpTransItems->Items[cnt].z);
			if (lpScItem) {
				lpScItem->dwLastTransTime = dwPlayTime;
			}
			else {
				lpScItem = FindEmptyScItem();
				if (lpScItem) {
					lpScItem->SetTransItem(&lpTransItems->Items[cnt]);
					lpScItem->ServerCode = ServerCode;
				}
			}
		}
		break;

	case smTRANSCODE_DELITEM:
		//¾ÆÀÌÅÛ Á¦°Å
		lpTransActionItem = (smTRANS_ACITON_ITEM*)pData->Buff;
		lpScItem = FindScItem(lpTransActionItem->x, lpTransActionItem->z);
		if (lpScItem) {

			//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
			EnterCriticalSection(&cDrawSection);
			smEnterTextureCriticalSection();

			lpScItem->Close();

			//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
			smLeaveTextureCriticalSection();
			LeaveCriticalSection(&cDrawSection);

		}
		break;

	case smTRANSCODE_PUTITEM:
		lpTransItemInfo = (TRANS_ITEMINFO*)pData->Buff;

		if (FiltQuestItem(lpTransItemInfo, smTRANSCODE_PUTITEM) == FALSE) break;

		if (PushRecvTransItemQue(lpTransItemInfo) == TRUE) {
			TransCommandEx.code = smTRANSCODE_GETITEM;
			TransCommandEx.size = sizeof(smTRANS_COMMAND_EX);
			TransCommandEx.WParam = lpTransItemInfo->Item.CODE;
			TransCommandEx.LParam = lpTransItemInfo->Item.ItemHeader.Head;
			TransCommandEx.SParam = lpTransItemInfo->Item.ItemHeader.dwChkSum;
			TransCommandEx.EParam = lpTransItemInfo->Item.Money ^ smTRANSCODE_GETITEM ^ lpTransItemInfo->Item.ItemHeader.dwChkSum;

			TransCommandEx.WxParam = lpTransItemInfo->dwSeCode[0];
			TransCommandEx.LxParam = lpTransItemInfo->dwSeCode[1];
			TransCommandEx.SxParam = lpTransItemInfo->dwSeCode[2];
			TransCommandEx.ExParam = lpTransItemInfo->dwSeCode[3];
			if (smWsockDataServer)
				smWsockDataServer->Send2((char*)&TransCommandEx, TransCommandEx.size, TRUE);

			break;
		}
		ComparePotion();

		memcpy(&TransRecvItem, lpTransItemInfo, sizeof(TRANS_ITEMINFO));

		if (CheckItemForm(&lpTransItemInfo->Item) == TRUE) {
			if (lpTransItemInfo->Item.CODE == (sinGG1 | sin01)) {
				if (cInvenTory.CheckMoneyLimit(lpTransItemInfo->Item.Money) == TRUE) {
					lpCurPlayer->smCharInfo.Money += lpTransItemInfo->Item.Money;
					ReformCharForm();
					sinPlaySound(SIN_SOUND_COIN, 400);

					addMoney = lpTransItemInfo->Item.Money;

					CheckServerMoney(pData->smMySock, lpTransItemInfo);

				}
				else {
					ThrowPutItem2(&lpTransItemInfo->Item, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
				}
			}
			else {
				if ((lpTransItemInfo->Item.CODE & sinITEM_MASK2) == sinQT1 || lpTransItemInfo->Item.ItemKindCode == ITEM_KIND_QUEST_WEAPON) {

					if (sinSetQuestItem(&lpTransItemInfo->Item) == TRUE) {
						SaveGameData();
					}
				}
				else {
					sinSetInvenItem(&lpTransItemInfo->Item);

					if (lpTransItemInfo->Item.Price >= 3000)
						SaveGameData();

					if ((lpTransItemInfo->Item.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
						ResetPotion2();
					}
				}
			}

		}
		ZeroMemory(&TransRecvItem, sizeof(TRANS_ITEMINFO));
		ResetInvenItemCode();
		break;

	case smTRANSCODE_GETITEM:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		lpTransItemInfo = FindRecvTransItemQue(lpTransCommand->WParam, lpTransCommand->LParam, lpTransCommand->SParam);

		if (!lpTransItemInfo) break;

		if (FiltQuestItem(lpTransItemInfo, smTRANSCODE_GETITEM) == FALSE) break;

		ComparePotion();

		memcpy(&TransRecvItem, lpTransItemInfo, sizeof(TRANS_ITEMINFO));

		if (CheckItemForm(&lpTransItemInfo->Item) == TRUE) {
			if (lpTransItemInfo->Item.CODE == (sinGG1 | sin01)) {
				if (cInvenTory.CheckMoneyLimit(lpTransItemInfo->Item.Money) == TRUE) {

					lpCurPlayer->smCharInfo.Money += lpTransItemInfo->Item.Money;
					ReformCharForm();
					sinPlaySound(SIN_SOUND_COIN, 400);

					addMoney = lpTransItemInfo->Item.Money;

					CheckServerMoney(pData->smMySock, lpTransItemInfo);

				}
				else {

					ThrowPutItem2(&lpTransItemInfo->Item, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
				}
			}
			else {
				if ((lpTransItemInfo->Item.CODE & sinITEM_MASK2) == sinQT1 || lpTransItemInfo->Item.ItemKindCode == ITEM_KIND_QUEST_WEAPON) {
					if (sinSetQuestItem(&lpTransItemInfo->Item) == TRUE) {
						SaveGameData();
					}
				}
				else {
					if ((lpTransItemInfo->Item.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
						if (smConfig.DebugMode && VRKeyBuff['P'] && VRKeyBuff['O']) {
							lpTransItemInfo->Item.PotionCount *= 2;
						}
					}

					sinSetInvenItem(&lpTransItemInfo->Item);
					if (lpTransItemInfo->Item.Price >= 3000)
						SaveGameData();

					if ((lpTransItemInfo->Item.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
						ResetPotion2();
					}
				}
			}

		}
		ZeroMemory(&TransRecvItem, sizeof(TRANS_ITEMINFO));
		ZeroMemory(lpTransItemInfo, sizeof(TRANS_ITEMINFO));

		ResetInvenItemCode();
		break;
	case smTRANSCODE_MESSAGEBOX:
		lpTransChatMessage = (TRANS_CHATMESSAGE*)pData->Buff;
		cMessageBox.ShowMessageEvent(lpTransChatMessage->szMessage);
		break;
	case PACKET_CHAT_GAME:
		CHATBOX->HandlePacket((PacketChatBoxMessage*)pData->Buff);
		break;
	case PACKET_CHATITEMLINK:
		CHATBOX->HandlePacket((PacketItemLinkChat*)pData->Buff);
		break;
	case PACKET_UPDATE_BOSSTIME:
		CMINIMAPHANDLE->HandlePacket((PacketBossTimeUpdate*)pData->Buff);
		break;
	case PACKET_GET_NPCS:
		CMINIMAPHANDLE->HandlePacket((PacketNpcsInMapAndNext*)pData->Buff);
		break;
	case PACKET_GET_INDICATORS:
		CMINIMAPHANDLE->HandlePacket((PacketMapIndicators*)pData->Buff);
		break;
	case PACKET_ADD_EXP:
	{
		struct sAddExp
		{
			int size;
			int code;
			INT64 Exp;
		};

		auto sPacketExp = (sAddExp*)pData->Buff;
		setexp();
		AddExp(sPacketExp->Exp);
	}
	break;
	case smTRANSCODE_WHISPERMESSAGE:
		break;
	case 0x48470024:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		AgingEvento = lpTransCommand->WParam;
		MixEvento = lpTransCommand->LParam;

		break;
	case 0x48470025:
		smConfig.DebugMode = 1;
		break;
		// DESATIVADO: REI PVP

	case 0x48480055:
	{
		WarpField2(53);
		break;
	}
	case 0x48480056:
	{
		// Reset PVP Semanal
		lpCurPlayer->sBlessCastle_Damage[1] = 0;
		break;
	}
	case 0x48480057:
	{
		EventoArena::GetInstance()->stageArena = 2;
		EventoArena::GetInstance()->getRankingFromServer((RankingPckg*)pData->Buff);
		break;
	}
	case 0x48480058:
	{
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		lpCurPlayer->nEquipeArena = lpTransCommand->LParam;
		EventoArena::GetInstance()->qtJogadores = lpTransCommand->WParam;

		extern int tickCountArena;
		tickCountArena = lpTransCommand->EParam;
		break;
	}
	case 0x48480059:
	{
		EventoArena::GetInstance()->stageArena = 0;
		lpCurPlayer->nEquipeArena = 0;
		EventoInvasao::GetInstance()->isInvasaoActive = FALSE;
		break;
	}
	case 0x48480060:
	{
		EventoArena::GetInstance()->stageArena = 1;
		break;
	}
	case 0x48480061:
	{
		EventoInvasao::GetInstance()->isInvasaoActive = TRUE;
		break;
	}
	case 0x48480062:
	{
		char szBuff[256];
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		if (!MatBuffKill[0])
		{
			MatBuffKill[0] = CreateTextureMaterial("game\\images\\kill\\2.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
			MatBuffKill[1] = CreateTextureMaterial("game\\images\\kill\\3.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
			MatBuffKill[2] = CreateTextureMaterial("game\\images\\kill\\4.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
			MatBuffKill[3] = CreateTextureMaterial("game\\images\\kill\\5.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
			ReadTextures();
		}

		TimeStreak = 20;
		killStreak += 1;
		lpTransCommand->WParam = killStreak;

		if (lpTransCommand->WParam > 1)
		{
			for (int i = 0; i < 32; i++)
			{
				if (!BuffKill[i].flag)
				{
					BuffKill[i].flag = 1;
					if (lpTransCommand->WParam == 2)
						BuffKill[i].image = MatBuffKill[0];
					else
						if (lpTransCommand->WParam == 3)
							BuffKill[i].image = MatBuffKill[1];
						else
							if (lpTransCommand->WParam == 4) {
								BuffKill[i].image = MatBuffKill[2];

							}
							else
								if (lpTransCommand->WParam >= 5) {
									BuffKill[i].image = MatBuffKill[3];
								}
					BuffKill[i].x = 0;
					BuffKill[i].y = 100;
					BuffKill[i].alpha = 0;
					ReadTextures();
					break;
				}
			}
		}
	}
	break;
	case 0x48480063:
	{
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		extern int tickCountArena;
		tickCountArena = lpTransCommand->WParam;

		EventoArena::GetInstance()->qtJogadores = lpTransCommand->LParam;
		EventoArena::GetInstance()->flagClan = TRUE;
		EventoArena::GetInstance()->timeArena = TRUE;
		//EventoArena::GetInstance()->stageArena = 2;
		break;
	}
	case 0x48480064:
	{
		EventoArena::GetInstance()->getTeamPlayersFromServer((playersFromTeamPckg*)pData->Buff);
		break;
	}
	case 0x48480065:
	{
		int x, z;
		WarpFieldNearPos(49, -23893 * fONE, 22207 * fONE, &x, &z);
		lpCurPlayer->SetPosi(-23893 * fONE, 0, 22207 * fONE, 0, 0, 0);
		TraceCameraPosi.x = lpCurPlayer->pX;
		TraceCameraPosi.y = lpCurPlayer->pY;
		TraceCameraPosi.z = lpCurPlayer->pZ;
		TraceTargetPosi.x = lpCurPlayer->pX;
		TraceTargetPosi.y = lpCurPlayer->pY;
		TraceTargetPosi.z = lpCurPlayer->pZ;
		break;
	}
	// Set VIP Status
	case 0x48480066:
	{
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		VIP::GetInstance()->setVipLevel(lpTransCommand->LParam);
		break;
	}
	// Abre npcs do vip
	case 0x48480067:
	{
		//OpenEachPlayer(lpCharMsTrace);
		DispEachMode = TRUE;
		//VIP::GetInstance()->getnpcsFromServer((npcsVIP*)pData->Buff);
		break;
	}
	// Seta time premiuns VIP 3
	case 0x48480068:
	{
		VIP::GetInstance()->setTimePremiuns();
		//VIP::GetInstance()->getnpcsFromServer((npcsVIP*)pData->Buff);
		break;
	}
	case 0x48480069:
	{
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		EventoArena::GetInstance()->stageArena = lpTransCommand->LParam;
		break;
	}
	case 0x48480070:
		WarpField2(3); // Teleporta para RIC depois da arena
		break;
	case GET_FAILED_ITEMS:
	{
		RestaureWindow::GetInstance()->ReceiveFailedItems((FailedItemsPckg*)pData->Buff);
		//RestaureWindow::GetInstance()->openFlag = true;
		break;
	}
	case OPEN_RANKING_NPC:
	{
		RankingWindow::GetInstance()->ReceivePlayers((TopPlayerPckg*)pData->Buff);
		RankingWindow::GetInstance()->openFlag = true;
		break;
	}
	case 0x48470028: // rei pvp xxstr
	{
		smTRANS_COMMAND* smTransCommand = (smTRANS_COMMAND*)pData->Buff;


		sSKILL haPVP;

		if (smTransCommand->SParam == 1 || smTransCommand->SParam == 3)
		{
			sSKILL haPVP;

			//ReiPVP = TRUE;

			lpCurPlayer->PlayCursePvP = TRUE;

			bReiPVP = TRUE;

			for (int j = 0; j < SIN_MAX_SKILL; j++)
			{
				if (sSkill[j].CODE == REIPVP)
				{
					memcpy(&haPVP, &sSkill[j], sizeof(sSKILL));
					haPVP.UseTime = 604800;
					sinContinueSkillSet(&haPVP);
					bReiPVP = TRUE;
					break;
				}
			}

			if (smTransCommand->SParam == 3) {
				lpCurPlayer->sBlessCastle_Damage[1] = smTransCommand->WParam;
			}
		}
		else
		{
			if (smTransCommand->SParam == 2)
			{

				bReiPVP = FALSE;
				lpCurPlayer->PlayCursePvP = FALSE; //TIRA EFEITO REIV PVP xxstr
				break;
			}
			else
			{
				lpCurPlayer->sBlessCastle_Damage[1] = smTransCommand->WParam;
			}
		}


		//TESTE BUG DE RECEBER MAIS DANO QUANDO NÃO TROCA PORÇÃO CAPILAR

		if (firstLogin)
		{
			//extern int getHairModel();
			//extern void ResetHairModel();
			//int iHairModel = getHairModel();
			//ResetHairModel();
			//ChangeHairModel(iHairModel);

			firstLogin = 0;

			RestaureWindow::GetInstance()->getFailedItems();
		}

		break;
	}


	// DESATIVADO: TOP LVL
	case 0x48470027: // select top lvl server xxstr
	{
		smTRANS_COMMAND* smTransCommand;
		smTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (smTransCommand->SParam)
		{
			// seta atual player como top lvl
			lpCurPlayer->PlayCurseTopLVL = TRUE;

			bTopLVL = TRUE;

			sSKILL haTopLVL;

			for (int j = 0; j < SIN_MAX_SKILL; j++)
			{
				if (sSkill[j].CODE == TOPLVL)
				{
					memcpy(&haTopLVL, &sSkill[j], sizeof(sSKILL));
					haTopLVL.UseTime = 604800;
					sinContinueSkillSet(&haTopLVL);
					bTopLVL = TRUE;
					break;
				}
			}

		}
		break;
	}

	case 0x48690029:
	{
		smTRANS_COMMAND* smTransCommand;
		smTransCommand = (smTRANS_COMMAND*)pData->Buff;

		wsprintf(szServerTime, "%02d/%02d/%d", smTransCommand->SParam, smTransCommand->EParam, smTransCommand->LParam);
		wsprintf(szBossTime, "XX:%02d", smTransCommand->WParam);

		break;
	}
	case 0x43550002:
	{
		struct Pck
		{
			int size;
			int code;
			int spec;
			bool shopTime;
			BYTE trans[sizeof(sITEM)];
		};

		Pck* smPacket;
		smPacket = (Pck*)pData->Buff;

		DecodeCompress((BYTE*)&smPacket->trans, (BYTE*)&TempPerfectItem);
		bIsPerfect = TRUE;

		if (smPacket->shopTime)
			ItemsDetailsTime.push_back(TempPerfectItem);
		else
			ItemsDetails.push_back(TempPerfectItem);

		break;
	}
	case 0x43550003:
		struct Pckk
		{
			int size;
			int code;
			int spec;
			BYTE trans[sizeof(sITEM)];
		};

		Pckk* smPacket2;
		smPacket2 = (Pckk*)pData->Buff;

		DecodeCompress((BYTE*)&smPacket2->trans, (BYTE*)&TempPerfectItem);
		bIsPerfect = TRUE;

		ItemsDetailsRestaure.push_back(TempPerfectItem);

		break;
	case 0x43550004:
	{
		struct Pck
		{
			int size;
			int code;
			int spec;
			BYTE trans[sizeof(sITEM)];
		};

		Pck* smPacket;
		smPacket = (Pck*)pData->Buff;

		DecodeCompress((BYTE*)&smPacket->trans, (BYTE*)&TempPerfectItem2);
		bIsPerfect2 = TRUE;
	}
	break;
	case smTRANSCODE_STARTMOUNT:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		chaPremiumitem.m_MountTime = lpTransCommand->WParam;
		chaPremiumitem.m_MountParam = lpTransCommand->LParam;

		chaPremiumitem.SetUpKeepItem(nsPremiumItem::MOUNT, lpTransCommand->WParam, true, "Montaria");
		cMountManager.AddMount(lpCurPlayer, static_cast<EMountID>(chaPremiumitem.m_MountParam), true);
		break;
	case smTRANSCODE_CONNECTED:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		SaveCheckSkill();
		lpCurPlayer->dwObjectSerial = lpCurPlayer->smCharInfo.dwObjectSerial;
		ReformSkillInfo();
		ReadTextures();

		if (pData->smMySock == smWsockServer) {
			dwGameWorldTime = ConvSysTimeToGameTime(lpTransCommand->LParam);
			dwGameHour = dwGameWorldTime / 60;
			dwGameHour = dwGameHour - (((int)(dwGameHour / 24)) * 24);
			dwGameMin = dwGameWorldTime - (((int)(dwGameWorldTime / 60)) * 60);

			dwConnectedClientTime = GetCurrentTime();
			dwConnectedServerTime = lpTransCommand->LParam;
			dwLastRecvGameServerTime = dwConnectedClientTime;

			if (smWsockServer != smWsockUserServer)
				SendSetObjectSerial_First(lpCurPlayer->smCharInfo.dwObjectSerial);

			SendNetworkQuality(smConfig.NetworkQuality);
		}
		break;

	case smTRANSCODE_RECORDDATA:
		if (!lpRecorder) {
			lpRecorder = new rsRECORD_DBASE;
			lpRecorder->TransLastPartCount = 0;
			ZeroMemory(lpRecorder->TransDataBlock, 64);
		}

		if (lpRecorder->RecvRecordDataFromServer((TRANS_RECORD_DATAS*)pData->Buff) == TRUE) {

			if (AdminCharDisable) {
				ZeroMemory(cInvenTory.InvenItem, sizeof(sITEM) * INVENTORY_MAXITEM);
				ZeroMemory(cInvenTory.InvenItemTemp, sizeof(sITEM) * INVENTORY_MAXITEM);

				lpCurPlayer->SetTool(0, hvPOSI_LHAND);
				lpCurPlayer->SetTool(0, hvPOSI_RHAND);
			}

			lpRecorder->ResotrRecordData(&lpCurPlayer->smCharInfo, cInvenTory.InvenItem, cInvenTory.InvenItemTemp, &MouseItem);

			if (lpRecorder)
			{
				delete lpRecorder;
				lpRecorder = 0;
			}

			dwLoadingTime = 0;

			if ((lpCurPlayer->smCharInfo.JOB_CODE == 3 || lpCurPlayer->smCharInfo.JOB_CODE == 5 || lpCurPlayer->smCharInfo.JOB_CODE == 7)
				&& lpCurPlayer->Pattern) {

				lpCurPlayer->SetPattern(lpCurPlayer->Pattern);
			}

			if (smConfig.DebugMode && smConfig.szFile_Player[0]) {

				lstrcpy(lpCurPlayer->smCharInfo.szModelName, smConfig.szFile_Player);
				lpCurPlayer->smCharInfo.szModelName2[0] = 0;

				CharLower(lpCurPlayer->smCharInfo.szModelName);

				lpCurPlayer->smCharInfo.State = smCHAR_STATE_USER;

				if (smConfig.szFile_Enemy[0]) {
					if (strstr(lpCurPlayer->smCharInfo.szModelName, "monster") != 0)
						lpCurPlayer->smCharInfo.State = smCHAR_STATE_ENEMY;
					if (strstr(lpCurPlayer->smCharInfo.szModelName, "npc") != 0)
						lpCurPlayer->smCharInfo.State = smCHAR_STATE_NPC;
				}

				AddLoaderPattern(lpCurPlayer, lpCurPlayer->smCharInfo.szModelName, lpCurPlayer->smCharInfo.szModelName2);
				ReformCharForm();

			}

			lpCurPlayer->Flag = TRUE;

			if (!lpCurPlayer->smCharInfo.dwEventTime_T || GetPlayTime_T() > lpCurPlayer->smCharInfo.dwEventTime_T) {
				lpCurPlayer->smCharInfo.SizeLevel = 1;
				lpCurPlayer->smCharInfo.dwEventTime_T = 0;
			}


			if (!chaPremiumitem.m_Hat && !chaPremiumitem.GetBigHeadType())
			{
				if (lpCurPlayer->smCharInfo.SizeLevel > 0x1000 && lpCurPlayer->smCharInfo.SizeLevel <= 0x1002)
				{
					ChangeModelSkin(lpCurPlayer, 1, lpCurPlayer->smCharInfo.SizeLevel + 3 - 0x1000, 0, 0);
					ReformCharForm();
					cInvenTory.IncreViewDamage(15);
				}
				else if (lpCurPlayer->smCharInfo.dwEventTime_T > 0 && lpCurPlayer->smCharInfo.SizeLevel == 1)
				{
					ChangeModelSkin(lpCurPlayer, 0, 6, 0, 0);
					ReformCharForm();
				}
				else
				{
					if (lpCurPlayer->smCharInfo.ChangeJob >= 0 && lpCurPlayer->smCharInfo.ChangeJob < 4)
					{
						ChangeModelSkin(lpCurPlayer, 0, lpCurPlayer->smCharInfo.ChangeJob, 0, GetFaceHairSkin(lpCurPlayer));
						ReformCharForm();
					}
				}
			}


#ifdef  __CLANSUBCHIP__
			if (cldata.myPosition == 100 || cldata.myPosition == 101 || cldata.myPosition == 104)
			{
#else
			if (cldata.myPosition == 100 || cldata.myPosition == 101) {
#endif
				lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
				lpCurPlayer->dwClanManageBit = cldata.CNFlag;

				/*cSINSOD2 * pSOD = new cSINSOD2;
				pSOD->SetSodSkill( lpCurPlayer->dwClanManageBit );
				delete pSOD;*/

				// SOD FIX XXSTR
				int SetSODSkill(int SODSkill);
				SetSODSkill(cldata.CNFlag);
			}
			else
				lpCurPlayer->smCharInfo.ClassClan = 0;

			int SetSODSkill(int SODSkill);
			SetSODSkill(getSodSkill);

			if (cldata.myPosition)
				InitClanMode = cldata.myPosition;

			if (!AdminCharDisable) {
				//´ÙÀ½ ÀúÀå ½Ã°£
				rsRecorder.dwNextRecordTime = dwPlayTime + RECORD_TIME_STEP;

				lpCurPlayer->SendCharInfo(smWsockServer);			//ÇÃ·¹ÀÌ¾î Á¤º¸ Àü¼Û
			}
			else {
				AddLoaderPattern(lpCurPlayer, lpCurPlayer->smCharInfo.szModelName, lpCurPlayer->smCharInfo.szModelName2);
				//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
				ReformCharForm();
			}

			//¿¡³ÊÁö°¡ 0ÀÎ°æ¿ì ¸¶À»¿¡¼­ ½ÃÀÛ
			if (lpCurPlayer->smCharInfo.Life[0] == 0) {
				//ÇÊµå ½ÃÀÛ
				lpCurPlayer->MoveFlag = FALSE;
				WarpStartField(&x, &z);
				lpCurPlayer->SetPosi(x, 0, z, 0, 0, 0);
				TraceCameraPosi.x = lpCurPlayer->pX;
				TraceCameraPosi.y = lpCurPlayer->pY;
				TraceCameraPosi.z = lpCurPlayer->pZ;
				TraceTargetPosi.x = lpCurPlayer->pX;
				TraceTargetPosi.y = lpCurPlayer->pY;
				TraceTargetPosi.z = lpCurPlayer->pZ;

				lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_RESTART);		//½ÃÀÛ¸ð¼Ç
				StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_GAME_START1);
				SetDynLight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 100, 100, 100, 0, 200);
				CharPlaySound(lpCurPlayer);
				dwBattleTime = 0;
				dwBattleQuitTime = 0;

				//¹Ù´Ú È®ÀÎ
				lpCurPlayer->OnStageField = -1;
				if (smGameStage[0]) {
					cnt = smGameStage[0]->GetFloorHeight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpCurPlayer->Pattern->SizeHeight);
					lpCurPlayer->OnStageField = 0;
				}
				if (cnt == CLIP_OUT && smGameStage[1]) {
					lpCurPlayer->OnStageField = 1;
				}
			}

			//sinSkill.SkillPoint = lpCurPlayer->smCharInfo.Level;
			cnt = lpCurPlayer->smCharInfo.Life[0];
			if (cnt > lpCurPlayer->smCharInfo.Life[1] || cnt < 0) {
				//Àß¸øµÈ »ý¸í·Â °­Á¦ º¸Á¤
				CheckCharForm();
				lpCurPlayer->smCharInfo.Life[0] = lpCurPlayer->smCharInfo.Life[1] / 2;
				ReformCharForm();
			}

			CheckCharForm();
			CodeXorExp = dwPlayTime + lpCurPlayer->smCharInfo.Exp;					//°æÇèÄ¡ ¼û±è ÄÚµå
			CodeXorExp_High = dwPlayTime | (dwPlayTime * dwPlayTime);						//°æÇèÄ¡ ¼û±è ÄÚµå
			CodeXorLife = (dwPlayTime * lpCurPlayer->smCharInfo.Life[0]) & 0xFFFF;		//»ý¸í·Â ¼û±è ÄÚµå
			lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
			lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
			lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
			ReformCharForm();

			ResetPotion();						//¹°¾à °¹¼ö »õ·Î ¼³Á¤
			ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­

			ReformSkillInfo();					//½ºÅ³º¸È£°ª ÀüºÎ ÃÊ±âÈ­

			//ÇÁ·Î¼¼½º Á¤º¸ º¸³¿
			//SendProcessInfo();

			//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ResetEnergyGraph(4);

			//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
			ResetInvenMoney();

			CheckPlaySkill();		//½ºÅ³°¨½Ã


		}

		break;

	case smTRANSCODE_FAILRECORDDATA:
		//·Îµù ½ÇÆÐ
		lpTransCharCommand = (smTRANS_CHAR_COMMAND*)pData->Buff;
		lpCurPlayer->smCharInfo.dwObjectSerial = lpTransCharCommand->WParam;
		lpCurPlayer->smCharInfo.dwLoginServerSafeKey = lpTransCharCommand->LParam;		//·Î±×ÀÎ ¼­¹ö ¾ÈÀüÅ° ¼³Ä¡

		if (!lpCurPlayer->smCharInfo.JOB_CODE && !smConfig.DebugMode) {
			//·Îµù ½ÇÆÐ
			//quit = TRUE;				//ºüÁ® ³ª°¡±â
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 11");
			DisconnectFlag = GetCurrentTime();
			break;
		}

		if (!AdminCharDisable) {

			CheckStartCharInfo(lpTransCharCommand->SParam);
			rsRecorder.dwNextRecordTime = dwPlayTime + RECORD_TIME_STEP;
			dwLoadingTime = 0;
			lpCurPlayer->SendCharInfo(smWsockServer);

			InitHelp();
			cHelpPet.CheckPetData();
			cPCBANGPet.CheckPetData();
		}
		break;

	case smTRANSCODE_RECORDRESULT:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		rsRecorder.dwRecordTime = 0;
		rsRecorder.dwNextRecordTime = dwPlayTime + RECORD_TIME_STEP;
		dwLastRecordTime = dwPlayTime;
		if (lpTransCommand->WParam) {
			RecordFailCount = 0;
			if (smConfig.DebugMode) CHATGAMEHANDLE->AddChatBoxText("> Jogo Salvo!", EChatColor::CHATCOLOR_Error);
		}
		else {
			RecordFailCount++;
			if (smConfig.DebugMode) CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "> Falha ao salvar (%d)", RecordFailCount);
		}

		cSinSiege.SetTaxRate(lpTransCommand->SParam);
		SetBlessCastleMaster(lpTransCommand->EParam, 0);

		break;

	case smTRANSCODE_BLESSCASTLE_TAX:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		cSinSiege.SetTaxRate(lpTransCommand->SParam);
		break;

	case smTRANSCODE_ADDEXP:
	case smTRANSCODE_ADDEXP_PARTY:
		smTRANS_EXP64* lpTransEXP;

		lpTransEXP = (smTRANS_EXP64*)pData->Buff;
		lpChar = FindAutoPlayer(lpTransEXP->LParam);
		//lpChar = FindChrPlayer(lpTransEXP->LParam);

		//-325953879
		if (lpChar && !lpChar->RecvExp)// Vale level
		{
			if (lpTransEXP->SParam == ((lpTransEXP->WParam * 3 + lpTransEXP->LParam * 13) * 2002))
			{
				//CheckServerExp( pData->smMySock, lpTransEXP);
				setexp();
				AddExp(lpTransEXP->WParam);

				// Vale level ?
				lpChar->RecvExp += lpTransEXP->WParam;
				addExp = lpTransEXP->WParam;

				Quest::GetInstance()->updateStatus(lpChar);

				if (IPData[1] == smTRANSCODE_ADDEXP)
				{
					// Conta monstros quest diária

					//CheckDailyQuestMonsters(lpChar);					
					CheckChangeJob3QuestItem(lpChar->smCharInfo.StatePoint);
				}
			}
			else {
				SendSetHackUser2(1800, lpTransCommand->WParam);
			}
		}
		else {
			//SendSetHackUser2( 1810 , lpTransCommand->WParam );
		}
		break;

	case smTRANSCODE_SERVER_INFO:
		TRANS_SERVER_LIST* lpTransServerList;

		lpTransServerList = (TRANS_SERVER_LIST*)pData->Buff;
		sinItemTime = lpTransServerList->dwServerTime_t;
		dwTime_ServerT = sinItemTime;
		dwTime_ConnectMS = GetCurrentTime();

		cnt = lpTransServerList->ClanServerCount;
		if (cnt) {
			cnt = lpTransServerList->ServerCount;
			web_DB_ip_port_init(lpTransServerList->ServerInfo[cnt].szServerIp1, lpTransServerList->ServerInfo[cnt].dwServerPort1,
				lpTransServerList->ServerInfo[cnt].szServerIp2, lpTransServerList->ServerInfo[cnt].dwServerPort2,
				lpTransServerList->ClanTicket);
		}
		//////////////////////////////////////////////////////

		CheckProcessModule();		//¸ðµâ°Ë»ç ÇÏ¿© ¼­¹ö·Î º¸³»±â

		//////////////////////////////////////////////////////
		HoRecvMessage(IPData[1], pData->Buff);
		break;

	case smTRANSCODE_ID_SETUSERINFO:
		if (smConfig.DebugMode) {
			//µð¹ö±× ¸ðµå ¿î¿µÀÚ Ä³¸¯ ¿À·ù º¸Á¤
			TRANS_USERCHAR_INFO* lpTransUserCharInfo = (TRANS_USERCHAR_INFO*)pData->Buff;
			FILE* fp;
			for (cnt = 0; cnt < lpTransUserCharInfo->PlayUserCount; cnt++) {
				if (!lpTransUserCharInfo->CharInfo[cnt].szModelName2[0]) {
					fp = fopen(lpTransUserCharInfo->CharInfo[cnt].szModelName, "rb");
					if (!fp) {
						x = lstrlen(lpTransUserCharInfo->CharInfo[cnt].szModelName);
						lpTransUserCharInfo->CharInfo[cnt].szModelName[x - 1] = 'x';
						fp = fopen(lpTransUserCharInfo->CharInfo[cnt].szModelName, "rb");
					}
					if (fp) {
						fclose(fp);
					}
					else {
						lstrcpy(lpTransUserCharInfo->CharInfo[cnt].szModelName,
							"char\\monster\\hopy\\hopy.ini");
					}
				}
			}
		}

	case smTRANSCODE_ISRECORDDATA:
	case smTRANSCODE_INSRECORDDATA:
	case smTRANSCODE_FAILCONNECT:
		//È£¸Þ¼¼Áö ¸®½Ã¹ö
		HoRecvMessage(IPData[1], pData->Buff);
		break;

	case smTRANSCODE_SHOP_ITEMLIST:
		//»óÁ¡ ¾ÆÀÌÅÛ ¸ñ·Ï ¹ÞÀ½

		//¹°¾à°¹¼ö ºñ±³
		ComparePotion();

		sinRecvMessage(IPData[1], pData->Buff);

		Send_GetBlessCastleTax();		//¼¼À² Á¤º¸ ¿ä±¸
/*
			sITEMINFO *lpItemInfo;
			BYTE *lpBuff;
			lpTransSellItem = (smTRANS_SELL_ITEM *)pData->Buff;

			lpItemInfo = new sITEMINFO[ lpTransSellItem->ItemCounter ];
			lpBuff = (BYTE *)pData->Buff+sizeof(smTRANS_SELL_ITEM);
			for( cnt=0;cnt<lpTransSellItem->ItemCounter;cnt++) {
				lpBuff += DecodeCompress( lpBuff , (BYTE *)&lpItemInfo[cnt] );
			}

			delete lpItemInfo;
*/
		break;

	case smTRANSCODE_SKILL_MENU:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		//½ºÅ³ ¸Þ´º ¿ÀÇÂ ¿ä±¸

		if (lpCurPlayer->OnStageField >= 0) {

			if (dwQuestDelayTime > dwPlayTime) lpTransCommand->WParam = 0;

			//if (StageField[lpCurPlayer->OnStageField]->FieldCode == 3) {
			//	//¸®Ä«¸£ÅÙ
			//	if (lpCurPlayer->smCharInfo.JOB_CODE < 5) {
			//		sinSkillMaster(lpTransCommand->WParam);
			//	}
			//}
			//if (StageField[lpCurPlayer->OnStageField]->FieldCode == 21) {
			//	//ÇÊ¶óÀÌ
			//	if (lpCurPlayer->smCharInfo.JOB_CODE >= 5)
			//	{
			//		sinSkillMaster(lpTransCommand->WParam);
			//	}
			//}

			sinSkillMaster(lpTransCommand->WParam);

			dwQuestDelayTime = dwPlayTime + 1000 * 5;		//5ÃÊ µô·¹ÀÌ
		}

		//sinSkillMaster(lpTransCommand->WParam);
		break;

	case smTRANSCODE_VERSION:
		//¹öÀü È®ÀÎ
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (pData->smMySock == smWsockDataServer) {
			//µ¥ÀÌÅ¸ ¼­¹ö Á¤º¸ ±â·Ï
			memcpy(&TransServerConnectInfo, lpTransCommand, sizeof(smTRANS_COMMAND));
		}
		if (pData->smMySock == smWsockDataServer || pData->smMySock == smWsockServer) {
			//¼­¹ö Á¤º¸ È®ÀÎ
			CheckServerInfo(lpTransCommand);
		}
		break;

	case smTRANSCODE_CHECK_NETSTATE:
		//µ¥µå¶ô È®ÀÎ ÆÐÅ¶

		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		//½Ã½ºÅÛ ½Ã°£À» °ÔÀÓ½Ã°£À¸·Î º¯È¯
		dwTime = ConvSysTimeToGameTime(lpTransCommand->SParam);
		//½Ã°£Â÷ÀÌ°¡ ¸¹ÀÌ ³ª¸é ½Ã°èº¸Á¤
		if (abs((long)dwGameWorldTime - (long)dwTime) > 10)
			dwGameWorldTime = dwTime;

		dwLastRecvGameServerTime = GetCurrentTime();
		rsRecvServer = TRUE;

		if (lpTransCommand->LParam) {
			//µ¥ÀÌÅ¸ ¼­¹ö¿¡¼­ ¿À´Â ³×Æ®¿÷µ¥ÀÌÅ¸ÀÇ LParam°ªÀÌ ÀÖÀ¸¸é nProtect °Ë»ç ½Ç½Ã
			if (Check_nProtect() == FALSE) {
				quit = TRUE;
			}
		}

		if (lpTransCommand->WParam && lpTransCommand->EParam) {
			//¼­¹ö¿¡¼­ ¸Þ¸ð¸® °ªÀ» Á¤ÇØ¼­ °¡Á®°£´Ù
			dwMemFunChkCode = funcCheckMemSum(lpTransCommand->WParam, lpTransCommand->EParam);
			dwMemFunChkCode = (dwMemFunChkCode << 16) | (lpTransCommand->WParam & 0xFFFF);
		}

		break;

	case smTRANSCODE_CHECK_NETSTATE2:
	case smTRANSCODE_CHECK_NETSTATE20:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		if (lpTransCommand->LParam) {
			//µ¥ÀÌÅ¸ ¼­¹ö¿¡¼­ ¿À´Â ³×Æ®¿÷µ¥ÀÌÅ¸ÀÇ LParam°ªÀÌ ÀÖÀ¸¸é nProtect °Ë»ç ½Ç½Ã
			if (Check_nProtect() == FALSE) {
				quit = TRUE;
			}
		}

		dwLastRecvGameServerTime2 = GetCurrentTime();
		rsRecvDataServer = TRUE;

		if (lpTransCommand->WParam && lpTransCommand->EParam)
		{
			dwMemFunChkCode = funcCheckMemSum(lpTransCommand->WParam, lpTransCommand->EParam);
			dwMemFunChkCode = (dwMemFunChkCode << 16) | (lpTransCommand->WParam & 0xFFFF);
		}
		break;

	case smTRANSCODE_CHECK_NETSTATE3:
		dwLastRecvGameServerTime3 = GetCurrentTime();
		rsRecvUserServer = TRUE;
		break;

	case smTRANSCODE_CHECK_NETSTATE4:
	case smTRANSCODE_CHECK_NETSTATE5:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (AreaServerMode) {
			if (lpWSockServer_Area[0] && lpWSockServer_Area[0] == (smWINSOCK*)lpTransCommand->EParam) {
				lpWSockServer_Area[0]->dwDeadLockTime = GetCurrentTime();
			}
			if (lpWSockServer_Area[1] && lpWSockServer_Area[1] == (smWINSOCK*)lpTransCommand->EParam) {
				lpWSockServer_Area[1]->dwDeadLockTime = GetCurrentTime();
			}
		}
		else {
			dwLastRecvGameServerTime4 = GetCurrentTime();
			rsRecvExtendServer = TRUE;
		}
		break;

	case smTRANSCODE_WARPFIELD:
		//º¯°æÇÒ À§Ä¡ ¹ÞÀ½
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpTransCommand->WParam == rsSOD_FIELD &&
			lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsSOD_VILLAGE) {
			//SOD Àå ÀÔÀå À¯¹« ·ÎµùÈ­¸é Ç¥½Ã
			if (!BellatraEffectInitFlag) {
				CreateBellatraFontEffect();
				BellatraEffectInitFlag = TRUE;
			}

			if (SoD_SetFontEffect.eBL_Type != E_BL_LODING) {
				SetBellatraFontEffect(E_BL_LODING, &SoD_SetFontEffect);
				sSodScore.SodNextStageNum = -2;
				sSodScore.dwSoD_NextStageTime = dwPlayTime + 3000;
				sSodScore.NextRound = 0;
				StopBGM();
			}
		}


		CloseEachPlayer();

		WarpFieldNearPos(lpTransCommand->WParam, lpTransCommand->SParam, lpTransCommand->LParam, &x, &z);

		lpCurPlayer->SetPosi(lpTransCommand->SParam, 0, lpTransCommand->LParam, 0, 0, 0);
		TraceCameraPosi.x = lpCurPlayer->pX;
		TraceCameraPosi.y = lpCurPlayer->pY;
		TraceCameraPosi.z = lpCurPlayer->pZ;
		TraceTargetPosi.x = lpCurPlayer->pX;
		TraceTargetPosi.y = lpCurPlayer->pY;
		TraceTargetPosi.z = lpCurPlayer->pZ;

		//¹Ù´Ú È®ÀÎ
		lpCurPlayer->OnStageField = -1;
		if (smGameStage[0]) {
			cnt = smGameStage[0]->GetFloorHeight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpCurPlayer->Pattern->SizeHeight);
			lpCurPlayer->OnStageField = 0;
		}
		if (cnt == CLIP_OUT && smGameStage[1]) {
			lpCurPlayer->OnStageField = 1;
		}
		break;

	case smTRANSCODE_TRADEITEMS:
		//¾ÆÀÌÅÛ ±³È¯Ã¢ Á¤º¸ ¼ö½Å
		//±³È¯Ã¢ ¾ÆÀÌÅÛ ¼ö½Å
		if (cTrade.OpenFlag && cTrade.TradeCharCode == ((TRANS_TRADEITEMS*)pData->Buff)->dwSender) {
			RecvTradeItems((TRANS_TRADEITEMS*)pData->Buff);
		}
		break;

	case smTRANSCODE_TRADE_ITEMKEY:
		//Æ®·¹ÀÌµå ¿Ï·á Å° ¼ö½Å
		//¾ÆÀÌÅÛ ±³È¯ ÀÎÁõÅ° ¼ö½Å
		if (DisconnectFlag)
			break;
		//if ( cTrade.OpenFlag && !TradeRecvItemKeyFlag ) {
		if (!TradeRecvItemKeyFlag) {
			if (RecvTradeSucessKey((TRANS_TRADE_ITEMKEY*)pData->Buff, &sTradeRecv) == TRUE) {
				TradeRecvItemKeyFlag = TRUE;
				//ÀÎÁõ ¼º°ø ¿Ï·á
				if (TradeItemSucessFlag) {
					if (sTrade.Money) AddInvenMoney(-(sTrade.Money - 193));			//ÀÎº¥ µ· Ã½Å©
					if (sTradeRecv.Money) AddInvenMoney(sTradeRecv.Money - 193);	//ÀÎº¥ µ· Ã½Å©

					memset(sTrade.TradeItem, 0, sizeof(sITEM) * MAX_TRADE_ITEM);
					sTrade.Money = 0;
					ReformTrade(&sTrade);

					CloseEachPlayer();
					cTrade.CheckOkTradeItem();
					ResetPotion();						//¹°¾à °¹¼ö »õ·Î ¼³Á¤
					SaveGameData();						//°ÔÀÓÀ» ÀúÀå
					ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
				}
			}
			else {
				//ÀÎÁõ ½ÇÆÐ
				cTrade.CancelTradeItem();
			}
		}
		break;

	case smTRANSCODE_TRADE_READYITEM:
		//¾ÆÀÌÅÛ ±³È¯ ³»¿ë ÃÖÁ¾ È®ÀÎ
		//if ( cTrade.OpenFlag ) {
			//if(sTrade.CheckFlag && sTradeRecv.CheckFlag){ //±³È¯!		
		if (DisconnectFlag)
			break;
		if (TradeItemSucessFlag)
			break;

		if (sTrade.CheckFlag) {
			//¾ÆÀÌÅÛ ±³È¯ ³»¿ë È®ÀÎ ¼ö½Å
			TradeItemSucessFlag = TRUE;
			sTradeRecv.CheckFlag = TRUE;
			if (RecvTradeCheckItem((TRANS_TRADE_CHECKITEM*)pData->Buff) == TRUE) {
				SendTradeSucessKey(((TRANS_TRADE_CHECKITEM*)pData->Buff)->dwSender);

				if (TradeRecvItemKeyFlag) {
					if (sTrade.Money) AddInvenMoney(-(sTrade.Money - 193));			//ÀÎº¥ µ· Ã½Å©
					if (sTradeRecv.Money) AddInvenMoney(sTradeRecv.Money - 193);	//ÀÎº¥ µ· Ã½Å©

					memset(sTrade.TradeItem, 0, sizeof(sITEM) * MAX_TRADE_ITEM);
					sTrade.Money = 0;
					ReformTrade(&sTrade);

					CloseEachPlayer();
					cTrade.CheckOkTradeItem();
					ResetPotion();						//¹°¾à °¹¼ö »õ·Î ¼³Á¤
					SaveGameData();						//°ÔÀÓÀ» ÀúÀå
					ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
				}
			}
			break;
		}
		//}
		//}
	//}
	//±³È¯ ¿À·ù ½ÇÆÐ
		cTrade.CancelTradeItem();
		break;


	case SMTRANSCODE_DISPLAY_DAMAGE:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpTransCommand->LParam)
		{
			cSHOW_DMG::getInstance()->AddDef(lpTransCommand->WParam, Type(lpTransCommand->SParam), lpTransCommand->LParam);
		}
		break;

	case SMTRANSCODE_DISPLAY_DEF:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpTransCommand->LParam)
		{
			cSHOW_DMG::getInstance()->AddDef(lpTransCommand->WParam, Type(lpTransCommand->LParam - 1));
		}
		break;

	case smTRANSCODE_TRADE_REQUST:
		//¾ÆÀÌÅÛ ±³È¯ ½ÅÃ»
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		lpChar = FindAutoPlayer(lpTransCommand->SParam);

		if (lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0] &&
			!cWareHouse.OpenFlag && !cCraftItem.OpenFlag && !cAging.OpenFlag && !cMyShop.OpenFlag && !Caravana::GetInstance()->OpenFlag || cMixtureReset.OpenFlag) { // ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã¢ Ãß°¡

			switch (lpTransCommand->LParam) {
			case 0:
				//±³È¯ °Å·¡ °Å¸® È®ÀÎ
				if (GetTradeDistance(lpChar) == FALSE) break;

				if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
					cMessageBox.ShowMessageEvent(mgWeightOver);
					break;
				}

				if (!cTrade.OpenFlag) {
					sinRequestTrade(lpTransCommand->SParam, lpChar->smCharInfo.szName);
					if (DispEachMode == 0)
					{
						OpenEachPlayer(lpChar);
						DispEachMode = 0;
						if (lpChar) InterfaceParty.latest_Insert(lpChar->smCharInfo.szName);
					}
				}
				break;
			case 1:
				//±³È¯ °Å·¡ °Å¸® È®ÀÎ
				if (GetTradeDistance(lpChar) == FALSE) break;
				if (cShop.OpenFlag) break;

				if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
					cMessageBox.ShowMessageEvent(mgWeightOver);		//¹«°èÃÊ°ú
					break;
				}

				//°Å·¡ ½ÂÀÎ ¼ö½Å
				if (!cTrade.OpenFlag)
					SendRequestTrade(lpTransCommand->SParam, 2);
			case 2:
				//°Å·¡Ã¢ ¿­±â
				if (!cTrade.OpenFlag) {
					cTrade.OpenFlag = TRUE;
					cInvenTory.OpenFlag = TRUE;
					cInterFace.CheckAllBox(SIN_TRADE);
					cTrade.TradeCharCode = lpTransCommand->SParam;
					TradeItemSucessFlag = 0;
					TradeRecvItemKeyFlag = 0;
					TradeSendSucessFlag = 0;
					TradeItemCancelCount = 0;
					TransLastRecvTrade.code = 0;
					memset(&sTrade, 0, sizeof(sTRADE));
					memset(&sTradeRecv, 0, sizeof(sTRADE));
					ReformTrade((void*)&sTrade);
					ReformTrade((void*)&sTradeRecv);
					lstrcpy(cTrade.szTradeCharName, lpChar->smCharInfo.szName);

					if (lpCurPlayer->MoveFlag) {
						//ÀÌµ¿ÁßÀÎ Ä³¸¯ÅÍ Á¤Áö ½ÃÅ°±â
						CancelAttack();
						lpCurPlayer->MoveFlag = 0;
					}
					EachTradeButton = 0;
					OpenEachPlayer(lpChar);
					DispEachMode = TRUE;
					if (lpChar) InterfaceParty.latest_Insert(lpChar->smCharInfo.szName);	//°Å·¡ÇÑ »ç¶÷ ÃÖ±Ù¸ñ·Ï¿¡ µî·Ï
				}
				break;
			case 3:
				//°Å·¡ Ãë¼Ò
				if (cTrade.OpenFlag) {
					if (TradeRecvItemKeyFlag) {
						memset(sTrade.TradeItem, 0, sizeof(sITEM) * MAX_TRADE_ITEM);
						sTrade.Money = 0;
						ReformTrade(&sTrade);
					}
					cTrade.CancelTradeItem();
				}
				break;
			}
		}
		break;

	case smTRANSCODE_FINDCRACK:
		//¼­¹ö·Î ºÎÅÍ Å©·¢ °Ë»ö ¿ä±¸
		//Å©·¢ Ã½Å©
		CheckCracker((TRANS_FIND_CRACKER*)pData->Buff);
		break;


	case smTRANSCODE_PARTY_REQUEST:
		PARTYHANDLER->HandlePacket((PacketRequestParty*)pData->Buff);
		break;
	case smTRANSCODE_PARTY_UPDATE:
		PARTYHANDLER->HandlePacket((PacketUpdateParty*)pData->Buff);
		break;
	case smTRANSCODE_PARTY_COMMAND:
		PARTYHANDLER->HandlePacket((PacketUpdatePartyData*)pData->Buff);
		break;
	case smTRANSCODE_REQUEST_RAID:
		PARTYHANDLER->HandlePacket((PacketRequestRaid*)pData->Buff);
		break;
	case smTRANSCODE_WAREHOUSE:
		//¾ÆÀÌÅÛ º¸°ü Ã¢°í µ¥ÀÌÅ¸ ÀÔ¼ö
		//Ã¢°í °¡Á®¿À±â
		if (!cWareHouse.OpenFlag && !cTrade.OpenFlag && !cMyShop.OpenFlag) {
			if (LoadWareHouse((TRANS_WAREHOUSE*)pData->Buff, &sWareHouse) == TRUE) {
				cWareHouse.LoadWareHouseItemIamge();
				cWareHouse.BackUpInvenItem();
				ResetInvenItemCode();
				ResetInvenMoney();
			}
		}
		break;

	case smTRANSCODE_CARAVAN:
		if (!cWareHouse.OpenFlag && !cTrade.OpenFlag && !cMyShop.OpenFlag && !Caravana::GetInstance()->OpenFlag)
		{
			if (LoadCaravanHouse((TRANS_CARAVAN*)pData->Buff, &sCaravan) == TRUE)
			{
				Caravana::GetInstance()->LoadCaravanItemImage();
				Caravana::GetInstance()->BackUpInvenItem();
				ResetInvenItemCode();
				ResetInvenMoney();
			}
		}
		break;
	case smTRANSCODE_OPEN_WAREHOUSE:
		//Ã¢°í µ¥ÀÌÅ¸ ¿ä±¸
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
			cMessageBox.ShowMessageEvent(mgWeightOver);		//¹«°èÃÊ°ú
			break;
		}

		if (smWsockDataServer && !cWareHouse.OpenFlag && !cTrade.OpenFlag) {
			smWsockDataServer->Send2((char*)lpTransCommand, lpTransCommand->size, TRUE);
		}
		break;

	case smTRANSCODE_OPEN_CARAVAN:

		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
			cMessageBox.ShowMessageEvent(mgWeightOver);
			break;
		}

		if (smWsockDataServer && !cWareHouse.OpenFlag && !cTrade.OpenFlag && !Caravana::GetInstance()->OpenFlag) {
			smWsockDataServer->Send2((char*)lpTransCommand, lpTransCommand->size, TRUE);
		}
		break;

	case smTRANSCODE_OPEN_MIXITEM:
		//¾ÆÀÌÅÛ Á¶ÇÕÃ¢ ¿­±â
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
			cMessageBox.ShowMessageEvent(mgWeightOver);		//¹«°èÃÊ°ú
			break;
		}

		if (lpTransCommand->WParam == 200) {
			//¿¬±Ý¼ú»ç
			cInterFace.CheckAllBox(SIN_CRAFTITEM);
			cCraftItem.ForceFlag = 1;
		}
		else
			cInterFace.CheckAllBox(SIN_CRAFTITEM);		//¾ÆÀÌÅÛ ¹Í½º

		break;

	case smTRANSCODE_OPEN_COLLECT:
		//½º¸Å½Ì TV ÀÌº¥Æ® ( ¸ó½ºÅÍ Àâ±â )
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		if (lpTransCommand->WParam == 5) {
			//SOD È¸°è Ã³¸®
			cSinSod2.ShowSod2MessageBox();
		}
		else
			sinShowEventSmashingTV();	//SOD ÀÔÀå

		break;

	case smTRANSCODE_OPEN_EVENTGIFT:

		if (sinChar->wVersion[1] == 1)
			cMessageBox.ShowMessage(MESSAGE_ALREADY_INITSTATE);
		else
			cMessageBox.ShowMessage3(MESSAGE_EVENT_GYUNGFUM, "");

		break;

	case smTRANSCODE_OPEN_AGING:
		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
			cMessageBox.ShowMessageEvent(mgWeightOver);
			break;
		}
		cInterFace.CheckAllBox(SIN_AGING);
		break;
	case smTRANSCODE_OPEN_SMELTING:
		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1])
		{
			cMessageBox.ShowMessageEvent(mgWeightOver);
			break;
		}
		cInterFace.CheckAllBox(SIN_SMELTING);
		break;
	case smTRANSCODE_OPEN_MANUFACTURE:
		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1])
		{
			cMessageBox.ShowMessageEvent(mgWeightOver);
			break;
		}
		cInterFace.CheckAllBox(SIN_MANUFACTURE);
		break;
	case smTRANSCODE_OPEN_MIXTURE_RESET:
		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1])
		{
			cMessageBox.ShowMessageEvent(mgWeightOver);
			break;
		}
		cInterFace.CheckAllBox(SIN_MIXTURE_RESET);
		break;
	case smTRANSCODE_SAVETHROWITEM2:
		ThrowItemToServer((smTRANS_COMMAND_EX*)pData->Buff);
		break;

	case smTRANSCODE_CLOSECLIENT:
		DisconnectServerCode = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 13");
		DisconnectFlag = GetCurrentTime();
		break;

	case smTRANSCODE_PLAYERKILLING:

		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;


		if (DeadPlayerExp(lpCurPlayer) == TRUE)
		{
			//°æÇèÄ¡ ÃÖÇÏÀ§ ( °¨¿ÁÀ¸·Î °£´Ù )
			//WarpPrisonField( &x,&z );
			//lpCurPlayer->SetPosi( x,0, z ,0,0,0 );
			//TraceCameraPosi.x = lpCurPlayer->pX;
			//TraceCameraPosi.y = lpCurPlayer->pY;
			//TraceCameraPosi.z = lpCurPlayer->pZ;
			//TraceTargetPosi.x = lpCurPlayer->pX;
			//TraceTargetPosi.y = lpCurPlayer->pY;
			//TraceTargetPosi.z = lpCurPlayer->pZ;

			//lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_RESTART );		//½ÃÀÛ¸ð¼Ç
			//StartEffect( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, EFFECT_GAME_START1 );
			//SetDynLight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ, 100, 100,100,0, 200 );
			//CharPlaySound( lpCurPlayer );
			//dwBattleTime = 0;
			//dwBattleQuitTime = 0;

			////¹Ù´Ú È®ÀÎ
			//lpCurPlayer->OnStageField = -1;
			//if ( smGameStage[0] ) {
			//	cnt = smGameStage[0]->GetFloorHeight( lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ ,lpCurPlayer->Pattern->SizeHeight );	
			//	lpCurPlayer->OnStageField = 0;
			//}
			//if ( cnt==CLIP_OUT && smGameStage[1] ) {
			//	lpCurPlayer->OnStageField = 1;
			//}
			//if ( smConfig.DebugMode ) {
			//	lpCurPlayer->smCharInfo.wPlayerKilling[0] = 16*2;			//2ºÐµ¿¾È °¨¿Á¿¡ °¤Èû
			//}
			//else {
			//	lpCurPlayer->smCharInfo.wPlayerKilling[0] = 16*30;			//30ºÐµ¿¾È °¨¿Á¿¡ °¤Èû
			//}
		}
		break;

	case smTRANSCODE_SEND_DATASERVER:
		if (smWsockDataServer)
		{
			smWsockDataServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}

		break;

	case smTRANSCODE_SEND_GAMESERVER:
		if (smWsockServer) {
			smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		if (smWsockUserServer && smWsockUserServer != smWsockServer) {
			smWsockUserServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		if (smWsockExtendServer && smWsockExtendServer != smWsockServer) {
			smWsockExtendServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		break;

	case smTRANSCODE_SEND_AREASERVER:
		//Áö¿ª¼­¹ö·Î ¿¬°á Àü¼Û
		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		if (lpsmSock)
			lpsmSock->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		else
			smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

		break;

	case smTRANSCODE_SEND_AGAME_SERVER:
		//°ÔÀÓ Áö¿ª¼­¹ö ¸ðµÎ Àü¼Û
		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½

		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{

			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::THIRD_EYES)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_ThirdEyesTime = 0;
				}
			}

			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::EXPUP_POTION)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_ExpUpPotionTime = 0;
				}
			}

			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_VampiricCuspidTime = 0;
				}
			}

			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MANA_RECHAR_P)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_ManaRechargingPTime = 0;
				}
			}
		}



		if (lpsmSock) {
			lpsmSock->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

			if (lpsmSock != smWsockServer)
				smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		else
			smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

		break;

	case smTRANSCODE_FUNCTION_MEM2:
		//°Ë»ç ¸Þ¸ð¸® Æã¼ÇÀÌ ÅëÃ¤·Î µµÂø2
		RecvMemFuncData2((TRANS_FUNC_MEMORY*)pData->Buff);
		break;

	case smTRANSCODE_CRAFTITEM:
		//¾ÆÀÌÅÛ ¹Í½ºÃÄ °á°ú ¼ö½Å
		if (((sCRAFTITEM_SERVER*)pData->Buff)->Result == TRUE) {

			cCraftItem.sinRecvCraftItemResult((sCRAFTITEM_SERVER*)pData->Buff);

			cInvenTory.DeleteInvenItemToServer(((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.CODE,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum);


			//ÀÎº¥Åä¸®ÀÇ ÁöÁ¤ÇÑ ¾ÆÀÌÅÛÀ» Ã£¾Æ °Ë»öÇÏ¿© Ã£´Â´Ù
			if (FindInvenItem(((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.CODE,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum) != 0) {
				//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ º¹»ç ¿À·ù
				SendInvenItemError(0, ((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.CODE,
					((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
					((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum);
				break;
			}

			ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
			SaveGameData();
			break;

		}
		cCraftItem.sinRecvCraftItemResult((sCRAFTITEM_SERVER*)pData->Buff);
		ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		SaveGameData();
		break;

	case smTRANSCODE_AGINGITEM:
		extern DWORD SpecialSheltomCode;
		SpecialSheltomCode = 0;

		if (((sCRAFTITEM_SERVER*)pData->Buff)->Result)
		{

			cAging.sinRecvAgingItemResult((sCRAFTITEM_SERVER*)pData->Buff);

			cInvenTory.DeleteInvenItemToServer(((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.CODE,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum);


			//ÀÎº¥Åä¸®ÀÇ ÁöÁ¤ÇÑ ¾ÆÀÌÅÛÀ» Ã£¾Æ °Ë»öÇÏ¿© Ã£´Â´Ù
			if (FindInvenItem(((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.CODE,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
				((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum) != 0) {
				//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ º¹»ç ¿À·ù
				SendInvenItemError(0, ((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.CODE,
					((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpKey,
					((sCRAFTITEM_SERVER*)pData->Buff)->DesCraftItem.sItemInfo.BackUpChkSum);
				break;
			}

			ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
			SaveGameData();
			break;
		}
		cAging.sinRecvAgingItemResult((sCRAFTITEM_SERVER*)pData->Buff);
		ResetInvenItemCode();
		SaveGameData();

		break;

	case smTRANSCODE_AGING_UPGRADE:
		//¿¡ÀÌÂ¡ ¿Ï·áµÈ ¾ÆÀÌÅÛ ¹Þ±â
		sinRecvAgingIncreStateItem(&((TRANS_ITEMINFO*)pData->Buff)->Item);
		TransAgingItemFlag = FALSE;
		ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		SaveGameData();

		//ÁÖº¯ Ä³¸¯ÅÍ¿¡ ÀÌº¥Æ® ¸í·É ÄÚµå º¸³»±â
		SendCommandUser(smCOMMNAD_USER_AGINGUP, 0, 0, 0);

		if ((((TRANS_ITEMINFO*)pData->Buff)->Item).CODE == cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE &&
			cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.ItemName[0]) {

			//¿À¸¥¼Õ
			sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, TRUE);
		}
		else
			if ((((TRANS_ITEMINFO*)pData->Buff)->Item).CODE == cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE &&
				cInvenTory.InvenItem[sInven[1].ItemIndex - 1].sItemInfo.ItemName[0]) {

				//¿Þ¼Õ
				sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, TRUE);

			}
		break;

	case smTRANSCODE_MAKE_LINKCORE:
		//µî·ÏµÈ ¸µÅ© ÄÚ¾î µµÂø
		cInvenTory.RecvLinkCore(&((TRANS_ITEMINFO*)pData->Buff)->Item);
		ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		SaveGameData();
		break;

	case smTRANSCODE_USE_LINKCORE:
		//¸µÅ© ÄÚ¾î »ç¿ë ¼º°ø
		SucessLinkCore((smTRANS_COMMAND_EX*)pData->Buff);
		break;

	case smTRANSCODE_WINGITEM:
		//À® ¾ÆÀÌÅÛ È¹µæ µµÂø
		/*
		if ( ((sCRAFTITEM_SERVER *)pData->Buff)->Result!=FALSE ) {
			wsprintf( szBuff, mgRecvItem , ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.ItemName );
			cMessageBox.ShowMessageEvent( szBuff );
		}
		*/
		RecvWingItem((sCRAFTITEM_SERVER*)pData->Buff);
		ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		SaveGameData();
		break;

	case smTRANSCODE_WINGITEMEVENT:
		//À® ¾ÆÀÌÅÛ È¹µæ µµÂø
		/*
		if ( ((sCRAFTITEM_SERVER *)pData->Buff)->Result!=FALSE ) {
		wsprintf( szBuff, mgRecvItem , ((sCRAFTITEM_SERVER *)pData->Buff)->DesCraftItem.sItemInfo.ItemName );
		cMessageBox.ShowMessageEvent( szBuff );
		}
		*/
		RecvWingItem((sCRAFTITEM_SERVER*)pData->Buff);
		ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		SaveGameData();
		break;

	case smTRANSCODE_ITEM_FORCEORB:
		//Æ÷½º¿Àºê ¾ÆÀÌÅÛ È¹µæ µµÂø
		RecvForceOrbItem((TRANS_ITEMINFO_GROUP2*)pData->Buff);
		ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		SaveGameData();
		break;

	case smTRANSCODE_SHOPTITEM:
		//¹°¾à°¹¼ö ºñ±³
		if ((((TRANS_BUY_SHOPITEM*)pData->Buff)->sItem.sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
			ComparePotion();
		}

		//»óÁ¡¿¡¼­ ¾ÆÀÌÅÛ ±¸ÀÔ
		cShop.RecvBuyItemToServer(&((TRANS_BUY_SHOPITEM*)pData->Buff)->sItem, ((TRANS_BUY_SHOPITEM*)pData->Buff)->ItemCount);

		if ((((TRANS_BUY_SHOPITEM*)pData->Buff)->sItem.sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
			ResetPotion();
		}

		ResetInvenItemCode();
		break;

	case smTRANSCODE_SHOP_SELLITEM:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (FindInvenItem(lpTransCommand->WParam, lpTransCommand->LParam, lpTransCommand->SParam) == 0) {
			CheckCharForm();
			sinPlusMoney(lpTransCommand->EParam);
			ReformCharForm();
			AddInvenMoney(lpTransCommand->EParam);
		}
		else {
			SendInvenItemError(0, lpTransCommand->WParam, lpTransCommand->LParam, lpTransCommand->SParam);
		}
		break;

	case smTRANSCODE_CHECKITEM:
	case smTRANSCODE_ERRORITEM:
	case smTRANSCODE_CLEARPOTION:
		RecvCheckItemFromServer((TRANS_ITEM_CODE*)pData->Buff);
		break;

	case smTRANSCODE_GETCLIENT_FUNCPOS:
		SendClientFuncPos();
		break;

	case smTRANSCODE_FUNCTION_MEM:
		//°Ë»ç ¸Þ¸ð¸® Æã¼ÇÀÌ ÅëÃ¤·Î µµÂø
		RecvMemFuncData((TRANS_FUNC_MEMORY*)pData->Buff);
		break;

	case smTRANSCODE_PARTY_POTION:
		//µ¿·á È¸º¹
		SetPartyPosion((smTRANS_COMMAND*)pData->Buff);
		break;

	case smTRANSCODE_HEALING:
	case smTRANSCODE_GRAND_HEALING:
	case smTRANSCODE_VAMPRIC_CUSPID:
	case smTRANSCODE_VAMPRIC_CUSPID_EX: // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	case smTRANSCODE_MANA_RECHARGING:
		//Ä¡·á ¹Þ±â
		SetPartyPosion((smTRANS_COMMAND*)pData->Buff);
		break;

	case smTRANSCODE_SAVECLIENT:
		SaveGameData();
		break;

	case smTRANSCODE_OPEN_SKINCHANGE:
		cSkinChanger.Open();
		break;

	case smTRANSCODE_DEAD_PLAYER:

		if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD)
		{
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
			sinSetLife(0);
			ReformCharForm();
			CharPlaySound(lpCurPlayer);
			DeadPlayerExp(lpCurPlayer);
			sinCheckQuest85Die();
			SaveGameData();
			ResetEnergyGraph(0);
			msgCabelo = 1;
			EventoArena::GetInstance()->flagClan = TRUE;
		}
		break;

	case smTRANSCODE_COMMAND_USER:
		//ÁÖº¯ Ä³¸¯ÅÍ¿ë ÀÌº¥Æ® ¸í·É ÄÚµå
		RecvCommandUser((smTRANS_COMMAND_EX*)pData->Buff);
		break;

	case smTRANSCODE_NPROTECT:
		//nProtect ¸í·É
		switch (((smTRANS_COMMAND*)pData->Buff)->WParam) {
		case 100:
			//nProtect Á¦°Å
			Remove_nProtect();
			break;
		}
		break;

	case smTRANSCODE_GAMEGUARD_AUTH:
		//GameGuard º¸¾ÈÃ½Å©
		npAuth(((smTRANS_COMMAND_DWORD*)pData->Buff)->WParam);
		break;

	case smTRANSCODE_OPEN_CLANMENU:
		//Å¬·£ ¸Þ´º ¿­±â
		void CLANmenu_meetNPC();	//netplay.cpp¿¡¼­ »ç¿ëÇÔ.
		CLANmenu_meetNPC();

		///////////////// Å×½ºÆ® ¸Ó¸®Ä¿Áö±â //////////////////
		if (!lpCurPlayer->smCharInfo.dwEventTime_T && smConfig.DebugMode && VRKeyBuff[VK_CONTROL]) {
			ChangeBigHeadMode((rand() % 2) + 1, 60 * 5);
			StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
			SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½
		}
		break;

	case smTRANSCODE_COMIC_SKIN:
		//ÄÚ¹Í ¸ðµå·Î ÀüÈ¯ ( ¾óÅ«ÀÌ )
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		if (!lpCurPlayer->smCharInfo.dwEventTime_T) {
			ChangeBigHeadMode(lpTransCommand->WParam, lpTransCommand->LParam);
			StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
			SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½

			// ¹ÚÀç¿ø - 2010 ¿ùµåÄÅ ÀÌº¥Æ® - Ãà±¸°ø Æ÷¼Ç »ç¿ë
			if (!lpTransCommand->SParam) // ¾óÅ«ÀÌ Æ÷¼ÇÀ» »ç¿ëÇÒ °æ¿ì¿¡¸¸ µ¥¹ÌÁö¸¦ ¿Ã·ÁÁØ´Ù
				cInvenTory.IncreViewDamage(15);		//µ¥¹ÌÁö °¡»óÀ¸·Î ¿Ã·ÁÁØ´Ù
		}
		break;

	case smTRANSCODE_CLAN_SERVICE:
		//Å¬·£ °¡ÀÔ ¼­ºñ½º
		RecvClanJoinService((smTRANS_CHAR_COMMAND2*)pData->Buff);
		break;


	case smTRANSCODE_VIRTURAL_POTION:
		//»ý¸í,±â·Â,±Ù·Â Áõ°¡
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (!lpTransVirtualPotion) {
			lpTransVirtualPotion = new smTRANS_COMMAND;
			if (lpTransVirtualPotion) {
				memcpy(lpTransVirtualPotion, lpTransCommand, sizeof(smTRANS_COMMAND));
			}
		}
		break;

	case smTRANSCODE_PROCESS_SKILL:
	case smTRANSCODE_PROCESS_SKILL2:
		//½ºÅ³Ã³¸® ¸®ÅÏ
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		RecvProcessSkill(lpTransCommand);
		break;

	case smTRANSCODE_PROCESS_CLAN_SKILL:
		//Å¬·£ ½ºÅ³ Àû¿ë
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		chaSiege.SetClanSkill(lpTransCommand->WParam);

		//Áö¿ª¼­¹ö¿¡ º¸³»¼­ Àû¿ë
		lpsmSock = GetAreaServerSock();
		if (lpsmSock) lpsmSock->Send2((char*)lpTransCommand, lpTransCommand->size, TRUE);
		break;

	case smTRANSCODE_CANCEL_SKILL:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		cnt = 0;

		{
			int iCrystalCode = lpTransCommand->LParam;

			if (iCrystalCode != 0)
			{
				sSKILL haCrystalTimer;
				int iCodeToCancel = 0;
				for (int j = 0; j < SIN_MAX_SKILL; j++)
				{
					if (sSkill[j].SkillTaget_CODE == iCrystalCode)
					{
						sSkill[j].SkillTaget_CODE = 0;
						iCodeToCancel = sSkill[j].CODE;
						limitebuff--;
						break;
					}
				}

				if (iCodeToCancel)
					CSKILL->CancelContinueSkill(iCodeToCancel);
			}
		}

		switch (lpTransCommand->WParam)
		{
		case SKILL_PLAY_METAL_GOLEM:
			cnt = SKILL_METAL_GOLEM;
			break;
		case SKILL_PLAY_RECALL_WOLVERIN:
			cnt = SKILL_RECALL_WOLVERIN;
			break;
		case SKILL_PLAY_FIRE_ELEMENTAL:
			cnt = SKILL_FIRE_ELEMENTAL;
			break;
		}

		if (cnt)
			CSKILL->CancelContinueSkill(cnt);

		break;

	case smTRANSCODE_PARTY_SKILL:
		//ÆÄÆ¼ ½ºÅ³ ÆÐÅ¶ ¼ö½Å
		RecvPartySkillFromServer((TRANS_PARTY_SKILL*)pData->Buff);
		break;

	case smTRANSCODE_CLAN_UPDATE:
		//Å¬·£ µ¥ÀÌÅ¸ ¾÷µ¥ÀÌÆ®
		Updatecldata();
		break;

	case smTRANSCODE_QUEST_MESSAGE:
		//Äù½ºÆ® ½ÇÇà ¸Þ¼¼Áö
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpTransCommand->WParam == SIN_QUEST_CODE_CHANGEJOB4) {
			CancelAttack();

			lpChar = FindAutoPlayer(lpTransCommand->LParam);
			if (lpChar) {
				OpenEachPlayer(lpChar);
				DispEachMode = TRUE;
			}

			sinBattleNpcResult(1);		//3Â÷ Àü¾÷ ¸¶½ºÅÍ¸¦ ÀÌ±è
		}
		else if (lpTransCommand->WParam == HAQUEST_CODE_FURYOFPHANTOM) {
			//Ç»¸® ¹èÆ² ½Â¸®
			chaQuest.RecvBattleNpcResult(1);
		}
		else
			sinMorayionNpcChangeJob_Two();

		break;

	case smTRANSCODE_ATTACK_RESULT:
		//°ø°Ý ½Ãµµ °á°ú ¹ÞÀ½ ( Å©¸®Æ¼ÄÃ ¼º°ø ¿©ºÎ )
		if (lpCurPlayer->AttackCritcal >= 0) {
			lpCurPlayer->AttackCritcal = ((smTRANS_COMMAND*)pData->Buff)->WParam;
		}
		break;

	case smTRANSCODE_DISP_DAMAGE:
		//°ø°Ý¹ÞÀº °ª Ãâ·Â¿ë
		LastAttackDamage = ((smTRANS_COMMAND*)pData->Buff)->WParam;
		break;

	case smTRANSCODE_DAMAGE_ENCODE_MEM:
		//µ¥¹ÌÁö ¾ÏÈ£È­ ¸ðµâ ¼ö½Å
		RecvDamagePacketModule((TRANS_FUNC_MEMORY*)pData->Buff);
		break;

	case smTRANSCODE_DAMAGE_ENCODE_MEM2:
		//µ¥¹ÌÁö ¾ÏÈ£È­ ¸ðµâ ¼ö½Å
		RecvDamagePacketModule2((TRANS_FUNC_MEMORY*)pData->Buff);
		break;

	case smTRANSCODE_PACKET_DYNFUNC:
		//ÆÐÅ¶ µ¿Àû ¾ÏÈ£È­ ¸ðµâ ¼ö½Å 
		RecvDynPacketModule((TRANS_FUNC_MEMORY*)pData->Buff);
		break;

	case smTRANSCODE_GETPLAYERINFO:
		//Ä³¸¯ÅÍ Á¤º¸ ¾÷µ¥ÀÌÆ® ¿ä±¸ ( ¾÷µ¥ÀÌÆ® ÆÐÅ¶À» ÀÒ¾î ¹ö¸° °É·Î ÃßÃø - Àç¹ß¼Û )
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		if (lpTransCommand->LParam != lpCurPlayer->smCharInfo.bUpdateInfo[0]) {
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;

	case smTRANSCODE_AREA_SERVER_MAP:
		//Áö¿ª ¼­¹ö¸Ê Á¤º¸ ÀÔ¼ö
		RecvAreaServerMap((TRANS_SEVER_MAP*)pData->Buff);
		break;

	case smTRANSCODE_ITEM_EXPRESS:
		//¾ÆÀÌÅÛ ¹è´Þ Á¤º¸ ÀÔ¼ö
		lpTransPostItem = (TRANS_POST_ITEM*)pData->Buff;
		/*
					if ( lpTransPostItem->dwItemFlag ) {
						cInvenTory.SetInvenToItemInfo( (sITEMINFO *)(pData->Buff+sizeof(TRANS_POST_ITEM) ));
						ResetInvenItemCode();
					}
		*/
		if (lpTransPostItem->dwItemFlag) {
			ComparePotion();			//¹°¾à°¹¼ö ºñ±³
			cInvenTory.SetInvenToItemInfo((sITEMINFO*)(pData->Buff + sizeof(TRANS_POST_ITEM)));
			ResetInvenItemCode();

			if ((((sITEMINFO*)(pData->Buff + sizeof(TRANS_POST_ITEM)))->CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
				//¹°¾à °¹¼ö »õ·Î ¼³Á¤
				ResetPotion2();
			}
			SaveGameData();
		}

		if (lpCurPlayer->smCharInfo.Weight[0] > lpCurPlayer->smCharInfo.Weight[1]) {
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);    //¹«°ÔÃÊ°ú ¸Þ¼¼Áö
			break;
		}

		if (lpTransPostItem->dwItemCode) { //¹ÞÀ» ¾ÆÀÌÅÛÀÌ ÀÖÀ»°æ¿ì °ø°£À» Ã¼Å©ÇÑÈÄ ¸Þ¼¼Áö¹Ú½º¸¦¶ç¿î´Ù

			// pluto ÇØ¿Ü ºô¸µ ¾ÆÀÌÅÛ ¹«°Ô
			if (sinChar->Weight[0] + lpTransPostItem->Weight > sinChar->Weight[1])
			{
				cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);    //¹«°ÔÃÊ°ú ¸Þ¼¼Áö
				break;
			}

			memset(&sMessageBox_RecvItem.RecvItem, 0, sizeof(sITEM));
			sMessageBox_RecvItem.RecvItem.CODE = lpTransPostItem->dwItemCode;
			sMessageBox_RecvItem.Flag = 1;
			sMessageBox_RecvItem.szItem[0] = 0;
			cInvenTory.OpenFlag = 1;
			sMessageBox_RecvItem.Param[0] = lpTransPostItem->dwParam[0];

			//µ·ÀÏ °æ¿ì 
			if (sMessageBox_RecvItem.RecvItem.CODE == (sinGG1 | sin01)) {
				cInvenTory.LoadMoneyExpImage(&sMessageBox_RecvItem.RecvItem);
				sMessageBox_RecvItem.RecvItem.SellPrice = lpTransPostItem->dwItemJobCode;
				lstrcpy(sMessageBox_RecvItem.szDoc, lpTransPostItem->szDoc);
				break;

			}
			//°æÇèÄ¡ÀÏ °æ¿ì 
			if (sMessageBox_RecvItem.RecvItem.CODE == (sinGG1 | sin02)) {
				cInvenTory.LoadMoneyExpImage(&sMessageBox_RecvItem.RecvItem);
				sMessageBox_RecvItem.RecvItem.OldX = lpTransPostItem->dwItemJobCode;
				lstrcpy(sMessageBox_RecvItem.szDoc, lpTransPostItem->szDoc);
				break;
			}
			//¹°¾àÀÏ°æ¿ì
			if ((sMessageBox_RecvItem.RecvItem.CODE & sinITEM_MASK1) == 0x04000000) {
				sMessageBox_RecvItem.RecvItem.PotionCount = lpTransPostItem->dwItemJobCode;
				cInvenTory.CheckInvenEmpty(&sMessageBox_RecvItem.RecvItem); //ÀÌ¹ÌÁö·Îµå
				lstrcpy(sMessageBox_RecvItem.szItem, lpTransPostItem->szItemName);
				lstrcpy(sMessageBox_RecvItem.szDoc, lpTransPostItem->szDoc);
				break;

			}

			//¾ÆÀÌÅÛ
			cInvenTory.CheckInvenEmpty(&sMessageBox_RecvItem.RecvItem); //ÀÌ¹ÌÁö·Îµå
			lstrcpy(sMessageBox_RecvItem.szItem, lpTransPostItem->szItemName);
			lstrcpy(sMessageBox_RecvItem.szDoc, lpTransPostItem->szDoc);
		}
		break;

	case smTRANSCODE_SOD_RESULT:
		//SoD °á°ú Ãâ·Â
		cSinHelp.ShowSodRanking((TRANS_SOD_RESULT*)pData->Buff, 1);
		break;

	case smTRANSCODE_SOD_RESULT2:
		//SoD °á°ú Ãâ·Â ( NPC¶û °°ÀÌ Ãâ·Â )
		cSinHelp.ShowSodRanking((TRANS_SOD_RESULT*)pData->Buff, 0);
		break;

	case smTRANSCODE_SOD_INFOMATION:
		//SoD ÁøÇà Á¤º¸ ¹ÞÀ½
		RecvSodGameInfomation(pData->Buff);
		break;


	case smTRANSCODE_WING_NPC:
		//Wing Äù½ºÆ® NPC ¸¸³ª´Ù
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		if (lpTransCommand->LParam) {
			//°¡Â¥ ¿öÇÁ°ÔÀÌÆ® (ÅÚ·¹Æ÷Æ®)
			switch (lpTransCommand->LParam) {
			case 1:
				sinShowTeleportMap();
				break;
			case 2:
				haShowCastleTeleportMap();
				break;
			case 3:
				TeleportDungeon();	// pluto Ãß°¡´øÀüÀ¸·Î ÅÚ·¹Æ÷Æ®
				break;
			case 4:
				TeleportMars();
				break;
			case 5:
				TeleportArena();
				break;
			case 6:
				TeleportBA();
				break;
			case 7:
				TradeBPEXP();
				break;
			case 8:
				bNewTeleportWindow = TRUE;
				break;
			}
		}

		if (lpTransCommand->LParam == 8)
			break;

		switch (lpTransCommand->WParam)
		{
		case 0:
		case 1:
			cInvenTory.sinWingQuestNpc();		//À® Äù½ºÆ®
			break;
		case 2:
			cInvenTory.SendPuzzleNpc();			//ÆÛÁñ¸ÂÃß±â
			//cInvenTory.CheckPristonAlphabetEventItem();
			break;
		case 3:
			cInvenTory.CheckEventItem();		//¹Ùº§ÀÇ»Ô
			break;
		case 4:
			cInvenTory.CheckFoxEventItem();		//±¸¹ÌÈ£ÀÌº¥Æ®
			break;
		case 5:
			cInvenTory.CheckCristalEventItem();	//Å©¸®½ºÅ» ¸ð¾Æ ÀÌº¥Æ®
			break;
		case 8:
			cInvenTory.CheckWatermelonEventItem();	// ¹ÚÀç¿ø - ¼ö¹Ú ÀÌº¥Æ®(È£¹Ú)  //¹ÚÀç¿ø - ¹ãÇÏ´ÃÀÇ ¼Ò¿ø ÀÌº¥Æ®  
			break;
		case 9:
			cInvenTory.CheckValentineEventItem();	// Àåº° - ¹ß·»Å¸ÀÎ ÀÌº¥Æ®
			break;
		case 10:
			cInvenTory.CheckPristonAlphabetEventItem();	//evento letras priston xxstr
			break;
		case 11:
			cInvenTory.CheckCandydaysEventItem();	// Àåº° - Äµµðµ¥ÀÌÁî
			break;
		case 12:
			cInvenTory.CheckMagicalGreenEventItem();	// Àåº° - ¸ÅÁöÄÃ±×¸°
			break;

		case 13:
			cInvenTory.CheckTearOfKaraEventItem();	// Àåº° - Ä«¶óÀÇ ´«¹°
			break;

		case 14:
			cInvenTory.CheckFindinvestigatorEventItem();	// Àåº° - Á¶»ç¿øÀ» Ã£¾Æ¶ó(¸¶·Â ¿¬±¸ÀÚ)
			break;
		case 15:
			cInvenTory.CheckFindinvestigatorKamiuEventItem();	// Àåº° - Á¶»ç¿øÀ» Ã£¾Æ¶ó(Ä«¹ÌÀ¯)
			break;
		case 16:
			cInvenTory.CheckFindinvestigatorEtoEventItem();	// Àåº° - Á¶»ç¿øÀ» Ã£¾Æ¶ó(¿¡Åä)
			break;

		}
		break;

	case smTRANSCODE_OPEN_STARPOINT:
		//º° Æ÷ÀÎÆ® NPC ¸¸³ª´Ù
		StarShopNPC();
		break;

	case smTRANSCODE_STARPOINT:
		//º° Æ÷ÀÎÆ® °ª
		sinStarPoint(((smTRANS_COMMAND*)pData->Buff)->WParam);
		//ÀÎÀÚ WParam ¿¡ ÇöÀç Àû¸³±Ý¾× Ç¥½Ã 
		break;

	case smTRANSCODE_CLANMONEY:
		//Å¬·£¸Ó´Ï µ¥ÀÌÅ¸
		sinRecvClanMoney(((smTRANS_COMMAND*)pData->Buff)->WParam, ((smTRANS_COMMAND*)pData->Buff)->LParam);
		break;

	case smTRANSCODE_OPEN_GIVEMONEY:
		//µ· ±âºÎ NPC ¸¸³ª´Ù
		cMessageBox.ShowMessage2(MESSAGE_GIVE_MONEY);
		break;

	case smTRANSCODE_CHANGE_EVENT:
		//ÀÌº¥Æ® Á¤º¸ º¯°æ
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		if (sBiInfo) {
			sBiInfo->EventPlay[0] = lpTransCommand->WParam & 0xFFFF;
			sBiInfo->EventPlay[1] = (lpTransCommand->WParam >> 16) & 0xFFFF;
			sBiInfo->PCRNo = lpTransCommand->LParam;
			cInvenTory.SetItemToChar();					//¼³Á¤ Àû¿ë
		}

		if (lpTransCommand->EParam) {
			GoNotice_chking = 1;								//¾È³»¸Þ¼¼Áö ½ÇÇà
		}
		break;

	case smTRANSCODE_CALL_MYSHOP:
		//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ Á¤º¸ ¿ä±¸
		if (lpCurPlayer->dwTradeMsgCode)
			Send_MyShopItemList((smTRANS_COMMAND*)pData->Buff);
		break;

	case smTRANSCODE_MYSHOP_ITEM:
		//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ¸ñ·Ï ¹ÞÀ½
		Recv_MyShopItemList((TRANS_TRADEITEMS*)pData->Buff);
		break;

	case smTRANSCODE_MYSHOP_MESSAGE:
		//°³ÀÎ»óÁ¡ ¸Þ¼¼Áö ¹ÞÀ½
		lpTransChatMessage = (TRANS_CHATMESSAGE*)pData->Buff;

		lpChar = FindAutoPlayer(lpTransChatMessage->dwObjectSerial);
		if (lpChar && lpChar->dwTradeMsgCode) {

			/*
			//Áß±¹ °³ÀÎ»óÁ¡ Æ¯Á¤¾ð¾î »ç¿ë±ÝÁö
			#ifdef _LANGUAGE_CHINESE
							ConvertStringTHAI(lpTransChatMessage->szMessage,  strlen(lpTransChatMessage->szMessage));
			#endif
			*/
			wsprintf(lpChar->szTradeMessage, "%s: %s", lpChar->smCharInfo.szName, lpTransChatMessage->szMessage);
		}
		break;

	case smTRANSCODE_MYSHOP_TRADE:
		//°³ÀÎ »óÁ¡ ¹°°Ç ±¸ÀÔ
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;

		cMyShop.RecvMyShopItem(lpTransCommand->SParam,
			(sMYSHOP_ITEM_SERVER*)(((smTRANS_COMMAND_BUFF*)pData->Buff)->Buff));

		break;

	case smTRANSCODE_SUCCESS_EVATION:
		//Ãß°¡È¸ÇÇ ¼º°ø Á¤º¸
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		//Ãâ·ÂºÎºÐ ÈÄ¿¡ Ãß°¡
		AddTimeEffect(smTRANSCODE_SUCCESS_EVATION, dwPlayTime + 500, lpTransCommand->WParam, lpTransCommand->LParam, lpTransCommand->SParam);
		break;

	case smTRANSCODE_BLESSCASTLE_INFO:
		//ºí·¡½ºÄ³½½ ¼³Á¤Á¤º¸ ¼ö½Å
		RecvBlessCastInfo(pData->Buff);
		break;

	case smTRANSCODE_MEDIA_NPC:
		//¿µ»ó Àç»ý NPC
		lpTransChatMessage = (TRANS_CHATMESSAGE*)pData->Buff;
		chaSiege.ShowCastlePlayMovie(lpTransChatMessage->szMessage,
			lpTransChatMessage->szMessage + lstrlen(lpTransChatMessage->szMessage) + 1,
			0);

		break;

	case smTRANSCODE_EXITINFO_URL:
		//Á¾·á½Ã À¥ÆäÀÌÁö ¿ÀÇÂ
		lpTransChatMessage = (TRANS_CHATMESSAGE*)pData->Buff;
		lstrcpy(szExitInfo_URL, lpTransChatMessage->szMessage);
		break;

	case smTRANSCODE_PUBLIC_POLLING:
		//¼³¹®Á¶»ç ½Ç½Ã ÆÐÅ¶
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		cSinHelp.ShowResearchMenu(lpTransCommand->WParam, lpTransCommand->LParam);
		break;

	case smTRANSCODE_NOTICE_BOX:
		//¼³¸í ¹®±¸Ã¢ Ç¥½Ã
		//ÃÊº¸ ¾ÆÀÌÅÛ °¡ÀÌµå ¸»
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		cHelp_Menu.haGuidHelpLevel_1(lpTransCommand->WParam);
		break;

	case smTRANSCODE_PCBANG_PET:
		//PC¹æ Æê ½ÇÇö
		//cPCBANGPet.PetKind = TRUE;
		//cPCBANGPet.ShowPet();
		break;
	case smTRANSCODE_PHENIX_PET:	// pluto Æê(ÇØ¿Ü) ¼öÁ¤
		//cPCBANGPet.PetKind = TRUE;
		//cPCBANGPet.ShowPet();
		break;

	case smTRANSCODE_HELP_PET: // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
		break;

#ifdef _xTrap_GUARD
	case smTRANSCODE_XTRAP_PACKET:	//XTrap ÆÐÅ¶¼ö½Å
		XTrap_Recv((smTRANS_COMMAND*)pData->Buff, smWsockDataServer);
		break;
#endif

	case smTRANSCODE_SMELTINGITEM:	// pluto Á¦·Ã
		SmeltingItem.sinRecvSmeltingItemResult((sSMELTINGITEM_SERVER*)pData->Buff);
		ResetInvenItemCode();
		SaveGameData();
		break;
	case smTRANSCODE_MANUFACTURE:	// pluto Á¦ÀÛ
		ManufactureItem.RecvManufactureItemResult((SManufactureItem_Server*)pData->Buff);
		ResetInvenItemCode();
		SaveGameData();
		break;
	case smTRANSCODE_MIXTURE_RESET_ITEM:
		// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Å¬¶óÀÌ¾ðÆ® Ã³¸®
		cMixtureReset.sinRecvMixtureItemResetResult((sMIXTURE_RESET_ITEM_SERVER*)pData->Buff);
		ResetInvenItemCode();
		SaveGameData();
		break;
	case smTRANSCODE_MANUFACTURE_WEIGHT_PRICE:	// pluto Á¦ÀÛ ¾ÆÀÌÅÛ °¡°Ý, ¹«°Ô Á¤º¸ °¡Á® ¿Ã·Á°í
		ManufactureItemResultInfo((SManufacture_ResultItemInfo_Server*)pData->Buff);
		break;
	case smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA2:		// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
		//°ÔÀÓ Áö¿ª¼­¹ö ¸ðµÎ Àü¼Û
		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MIGHT_OF_AWELL)	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£	¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£2
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_MightofAwellTime = 0;
					chaPremiumitem.m_MightofAwellWeight = 0;
				}
			}

			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MANA_REDUCE_P)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_ManaReducePotiontime = 0;
					chaPremiumitem.m_ManaReducePotionValue = 0;
				}
			}
			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::MIGHT_OF_AWELL)	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£	¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£2
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_MightofAwellTime = 0;
					chaPremiumitem.m_MightofAwellWeight = 0;
				}
			}
			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::PHENIX_PET)	// pluto Æê(ÇØ¿Ü)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_PhenixPetTime = 0;
					cPCBANGPet.ClosePet();
				}
			}
		}
		if (lpsmSock) {
			lpsmSock->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

			if (lpsmSock != smWsockServer)
				smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		else
			smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		break;

		// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê
	case smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA3:
		//°ÔÀÓ Áö¿ª¼­¹ö ¸ðµÎ Àü¼Û
		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_TERRY)	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					// Àåº° - Å×¸® ¼Ó¼º
					chaPremiumitem.m_TerryTime = 0;

					chaPremiumitem.m_HelpPetTime = 0;
					sinChar->GravityScroolCheck[1] = 0;
					cHelpPet.ClosePet();
				}
			}
			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_NEPSYS)	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_HelpPetTime = 0;
					cHelpPet.ClosePet();
				}
			}
			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_IO)	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_HelpPetTime = 0;
					cHelpPet.ClosePet();
				}
			}
			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_MUTE)	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_HelpPetTime = 0;
					sinChar->GravityScroolCheck[1] = 0;
					cHelpPet.ClosePet();
				}
			}
			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::STAMINA_REDUCE_P) // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_StaminaReducePotiontime = 0;
					chaPremiumitem.m_StaminaReducePotionValue = 0;
					chaPremiumitem.SetStaminaReducePotionTime(chaPremiumitem.m_StaminaReducePotiontime);
				}
			}

			else if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS_EX)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_VampiricCuspidEXTime = 0;
				}
			}
		}

		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS_EX)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_VampiricCuspidEXTime = 0;
				}
			}

		}


		if (lpsmSock) {
			lpsmSock->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

			if (lpsmSock != smWsockServer)
				smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		else
			smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

		break;


	case smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA4:		// Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	//°ÔÀÓ Áö¿ª¼­¹ö ¸ðµÎ Àü¼Û
		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == nsPremiumItem::VAMPIRIC_CUS_EX)
			{
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] - chaPremiumitem.UpKeepItem[i].IconTime[0] <= 0)
				{
					chaPremiumitem.m_VampiricCuspidEXTime = 0;
				}
			}

		}
		if (lpsmSock) {
			lpsmSock->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);

			if (lpsmSock != smWsockServer)
				smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		}
		else
			smWsockServer->Send2(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff, ((smTRANS_COMMAND*)(((TRANS_SEND_DATASERVER*)pData->Buff)->szBuff))->size, TRUE);
		break;

	case smTRANSCODE_COUPLERING:		// pluto Ä¿ÇÃ¸µ
	{
		cInvenTory.DeleteCoupleRing();
	}
	break;

	case smTRANSCODE_PREMIUMITEM_INIT:
		lpTransCommand = (smTRANS_COMMAND*)pData->Buff;
		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (chaPremiumitem.UpKeepItem[i].TGAImageNumber == lpTransCommand->WParam)
			{
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.UpKeepItem[i].IconTime[0];
				if (lpTransCommand->WParam == 8 || lpTransCommand->WParam == 9 || lpTransCommand->WParam == 10 || lpTransCommand->WParam == 11) // ¹ÚÀç¿ø - µµ¿ì¹Ì Æê ÃÊ±âÈ­
				{
					cHelpPet.ClosePet();
				}
				else if (lpTransCommand->WParam == 12) // ¹ÚÀç¿ø - ÇÇ´Ð½º Æê ÃÊ±âÈ­
				{
					cPCBANGPet.ClosePet();
				}
				else if (lpTransCommand->WParam == 6) // ¹ÚÀç¿ø - ¸¶³ª ¸®µà½º Æ÷¼Ç ÃÊ±âÈ­
				{
					chaPremiumitem.m_ManaReducePotiontime = 0;
					chaPremiumitem.SetManaReducePotionTime(chaPremiumitem.m_ManaReducePotiontime);
				}
				else if (lpTransCommand->WParam == 7) // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç ÃÊ±âÈ­
				{
					chaPremiumitem.m_StaminaReducePotiontime = 0;
					chaPremiumitem.SetStaminaReducePotionTime(chaPremiumitem.m_StaminaReducePotiontime);
				}
			}
		}
		break;

	case SMTRANSCODE_UPDATE_PRIMEITEM:

		if (chaPremiumitem.bUpdateTimeDelayFlag == true) break;

		lpTransPremiumTime = (TRANS_PREMIUM_TIME*)pData->Buff;

		chaPremiumitem.m_ThirdEyesTime = lpTransPremiumTime->ThirdEye;
		chaPremiumitem.m_ExpUpPotionTime = lpTransPremiumTime->ExpUp;
		chaPremiumitem.m_VampiricCuspidTime = lpTransPremiumTime->VampCuspid;
		chaPremiumitem.m_ManaRechargingPTime = lpTransPremiumTime->ManaRecharg;
		chaPremiumitem.m_ManaReducePotiontime = lpTransPremiumTime->ManaReduce;
		chaPremiumitem.m_MightofAwellTime = lpTransPremiumTime->MightofAwell;
		if (!chaPremiumitem.m_MightofAwellTime)
			chaPremiumitem.m_MightofAwellTime = lpTransPremiumTime->MightofAwell2;
		chaPremiumitem.m_PhenixPetTime = lpTransPremiumTime->PhenixPet;
		chaPremiumitem.m_HelpPetTime = lpTransPremiumTime->HelpPet;
		chaPremiumitem.m_VampiricCuspidEXTime = lpTransPremiumTime->VampCuspidEx;
		chaPremiumitem.m_StaminaReducePotiontime = lpTransPremiumTime->StaminaReduce;
		chaPremiumitem.m_CaravanArma = lpTransPremiumTime->CaravanArma;
		chaPremiumitem.m_CaravanHopy = lpTransPremiumTime->CaravanHopy;
		chaPremiumitem.m_CaravanBuma = lpTransPremiumTime->CaravanBuma;
		chaPremiumitem.m_Hat = lpTransPremiumTime->Hat;
		chaPremiumitem.m_MountTime = lpTransPremiumTime->Mount;

		if (chaPremiumitem.m_ThirdEyesTime > MAX_TIME_PRIMEITEM || chaPremiumitem.m_ExpUpPotionTime > MAX_TIME_PRIMEITEM ||
			chaPremiumitem.m_VampiricCuspidTime > MAX_TIME_PRIMEITEM || chaPremiumitem.m_ManaRechargingPTime > MAX_TIME_PRIMEITEM ||
			chaPremiumitem.m_ManaReducePotiontime > MAX_TIME_PRIMEITEM || chaPremiumitem.m_MightofAwellTime > MAX_TIME_PRIMEITEM || chaPremiumitem.m_PhenixPetTime > MAX_TIME_PRIMEITEM ||
			chaPremiumitem.m_HelpPetTime > MAX_TIME_PRIMEITEM || chaPremiumitem.m_StaminaReducePotiontime > MAX_TIME_PRIMEITEM || chaPremiumitem.m_VampiricCuspidEXTime > MAX_TIME_PRIMEITEM
			|| chaPremiumitem.m_CaravanArma > MAX_TIME_PRIMEITEM || chaPremiumitem.m_CaravanHopy > MAX_TIME_PRIMEITEM || chaPremiumitem.m_CaravanBuma > MAX_TIME_PRIMEITEM || chaPremiumitem.m_Hat > MAX_TIME_PRIMEITEM || chaPremiumitem.m_MountTime > MAX_TIME_PRIMEITEM)
		{
			break;
		}

		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			int TGANumber = chaPremiumitem.UpKeepItem[i].TGAImageNumber;

			switch (TGANumber)
			{
			case nsPremiumItem::THIRD_EYES:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_ThirdEyesTime;
				break;

			case nsPremiumItem::EXPUP_POTION:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_ExpUpPotionTime;
				break;

			case nsPremiumItem::VAMPIRIC_CUS:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_VampiricCuspidTime;
				break;

			case nsPremiumItem::MANA_RECHAR_P:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_ManaRechargingPTime;
				break;

			case nsPremiumItem::MIGHT_OF_AWELL:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_MightofAwellTime;
				break;

			case nsPremiumItem::MANA_REDUCE_P:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_ManaReducePotiontime;
				break;

			case nsPremiumItem::PHENIX_PET:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_PhenixPetTime;
				break;

			case nsPremiumItem::HELP_PET_TERRY:
			case nsPremiumItem::HELP_PET_NEPSYS:
			case nsPremiumItem::HELP_PET_IO:
			case nsPremiumItem::HELP_PET_MUTE:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_HelpPetTime;
				break;

			case nsPremiumItem::VAMPIRIC_CUS_EX:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_VampiricCuspidEXTime;
				break;

			case nsPremiumItem::STAMINA_REDUCE_P:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_StaminaReducePotiontime;
				break;
			case nsPremiumItem::CARAVAN_ARMA:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_CaravanArma;
				break;
			case nsPremiumItem::CARAVAN_HOPY:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_CaravanHopy;
				break;
			case nsPremiumItem::CARAVAN_BUMA:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_CaravanBuma;
				break;
			case nsPremiumItem::HAT:
			case nsPremiumItem::HAT2:
			case nsPremiumItem::HAT3:
			case nsPremiumItem::HAT4:
			case nsPremiumItem::HAT5:
			case nsPremiumItem::HAT6:
				if (chaPremiumitem.UpKeepItem[i].IconTime[1] && !chaPremiumitem.m_Hat)
					chaPremiumitem.ChangeBigHead(0, 0);

				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_Hat;
				break;
			case nsPremiumItem::MOUNT:
				chaPremiumitem.UpKeepItem[i].IconTime[1] = chaPremiumitem.m_MountTime;
				break;
			}

		}
		break;

	}

	if (lpCharInfo_MsgBack) {

		if (addExp) {

			cnt = (lpCurPlayer->smCharInfo.Exp ^ CodeXorExp) - (lpCharInfo_MsgBack->Exp ^ CodeXorExp);
			if (cnt > addExp) {
				memcpy(&lpCurPlayer->smCharInfo, lpCharInfo_MsgBack, sizeof(smCHAR_INFO));
				SaveGameData();						//°ÔÀÓÀ» ÀúÀå
				lpCurPlayer->smCharInfo.Exp++;
			}
		}

		if (addMoney) {
			cnt = lpCurPlayer->smCharInfo.Money - lpCharInfo_MsgBack->Money;

			if (cnt >= addMoney) {
				if (addMoney >= 2000)
					SaveGameData();						//°ÔÀÓÀ» ÀúÀå
			}
			else {
				//µ¥ÀÌÅ¸°¡ Æ²¸²
				lpCurPlayer->smCharInfo.Money = lpCharInfo_MsgBack->Money;		//º¹±¸½ÃÅ°¸é¼­ ¿¡·¯³ª°ÔÇÔ
			}

			//ÀÎº¥Åä¸® °Ë»ç¿ëµ· Ãß°¡
			AddInvenMoney(addMoney);

		}

		delete lpCharInfo_MsgBack;
	}

	return TRUE;
}

int rsTRANS_SERVER::Init(smWINSOCK * lpsmsock)
{

	lpsmSock = lpsmsock;
	lpRecorder = 0;

	InitializeCriticalSection(&cServSection);

	RecvDataQuePush = 0;
	RecvDataQuePop = 0;

	ZeroMemory(RecvDataQue, RECV_DATA_QUE_COUNT * 4);

	return TRUE;
}


extern int szSpaceSorting(char* lpString);

int rsTRANS_SERVER::SendChat(char* szMessage)
{
	return TRUE;
}

extern void ResetHairModel();

int rsTRANS_SERVER::SendPlayData(smCHAR * lpChar)
{

	TRANS_PLAYPOS* lpTransPlayPos;
	int	dist, x, y, z;

	TransPlayPosCount++;

	if ((TransPlayPosCount & 1) == 0) {
		x = (TransPlayPosLast.rsPlayPos.x - lpChar->pX) >> FLOATNS;
		y = (TransPlayPosLast.rsPlayPos.y - lpChar->pY) >> FLOATNS;
		z = (TransPlayPosLast.rsPlayPos.z - lpChar->pZ) >> FLOATNS;
		dist = x * x + y * y + z * z;

		if (sinGetLife() > 0 && dist < (16 * 16 * 16)) {
			return TRUE;
		}
	}

	lpTransPlayPos = (TRANS_PLAYPOS*)TransBuff;
	lpTransPlayPos->code = smTRANSCODE_POSITION;
	lpTransPlayPos->size = sizeof(TRANS_PLAYPOS);

	if (lpCurPlayer->OnStageField >= 0)
		lpTransPlayPos->rsPlayPos.Area = StageField[lpCurPlayer->OnStageField]->FieldCode;
	else
		lpTransPlayPos->rsPlayPos.Area = -1;

	lpTransPlayPos->rsPlayPos.x = lpChar->pX;
	lpTransPlayPos->rsPlayPos.y = lpChar->pY;
	lpTransPlayPos->rsPlayPos.z = lpChar->pZ;

	if (lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD)
		lpTransPlayPos->Hp[0] = 0;
	else
		lpTransPlayPos->Hp[0] = sinGetLife();

	lpTransPlayPos->Hp[1] = lpChar->smCharInfo.Life[1];

	if (smWsockServer)
		smWsockServer->Send2((char*)lpTransPlayPos, lpTransPlayPos->size, TRUE);

	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
		smWsockExtendServer->Send2((char*)lpTransPlayPos, lpTransPlayPos->size, TRUE);


	int	MainArea, SubArea;

	if (AreaServerMode) {
		if (lpCurPlayer->OnStageField >= 0) {
			MainArea = (lpCurPlayer->OnStageField) & 1;
			SubArea = (lpCurPlayer->OnStageField + 1) & 1;

			if (lpWSockServer_DispArea[SubArea] && lpWSockServer_DispArea[MainArea] != lpWSockServer_DispArea[SubArea]) {
				lpWSockServer_DispArea[SubArea]->Send2((char*)lpTransPlayPos, lpTransPlayPos->size, TRUE);
			}
		}
	}


	return TRUE;
}


rsTRANS_SERVER	rsMainServer;

#define SAVE_CHAT_COMMAND_MAX		16
#define SAVE_CHAT_COMMAND_MASK		15

char szSaveChatCommands[SAVE_CHAT_COMMAND_MAX][256];
int ChatCommandSaveCnt = 0;
int ChatSaveRecallCnt = 0;

int rsCompString(char* src1, char* src2)
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

char szLastChatMessage[256];
DWORD	dwLastChatTime;
DWORD	dwContiueChatCount = 0;
#include "CurseFilter.h"
#include "TitleBox/TitleBox.h"

char* rsGetWord(char* q, char* p);

extern int WaveCameraMode;

extern int rsRecordDamageError(rsPLAYINFO * lpPlayInfo, smTRANS_COMMAND * lpTransCommand);

int SendChatMessageToServer(std::string szChatMessage)
{
	return TRUE;
}

int ChatScrollPoint = 0;

int DisplayChatMessage(HDC hdc, int x, int y, int MaxLine)
{
	return TRUE;
}


int GetNextLineCount(char* szString, int LineMax)
{
	int cnt;
	int len;
	char* lp;

	len = lstrlen(szString);
	if (len <= LineMax) return 0;

	lp = szString;

	cnt = 0;

	while (1) {
		//2¹ÙÀÌÆ® ÄÚµå È®ÀÎ
		if (CheckCode_2Byte(lp) == 2) {
			cnt += 2;
			lp += 2;
		}
		else {
			cnt++;
			lp++;
		}

		if (cnt > (LineMax - 2)) break;
	}


	return cnt;
}

int colorText = 0;

int SetIpColor(HDC hdc, DWORD dwIP)
{

	if (dwIP == dwMyIP) {
		SetFontTextColor(RGB(255, 128, 96));
		colorText = 1;
		return TRUE;
	}

	switch (dwIP) {
	case 0:
		SetFontTextColor(RGB(255, 128, 96));
		colorText = 2;
		break;
	case 1:
		//		SetFontTextColor( hdc, RGB(255, 190, 150) );
		SetFontTextColor(RGB(150, 190, 255));			//±Ó¸»
		break;
	case 2:
		//		SetFontTextColor( hdc, RGB(255, 140, 120) );
		SetFontTextColor(RGB(150, 190, 255));			//
		break;
	case 3:
		SetFontTextColor(RGB(230, 160, 255));
		colorText = 3;
		break;
	case 4:
		SetFontTextColor(RGB(200, 255, 0));				//µ¿·áÃ¤ÆÃ
		break;
	case 5:
		SetFontTextColor(RGB(255, 255, 80));			//Å¬·£Ã¤ÆÃ
		break;
	case 6:
		SetFontTextColor(RGB(200, 255, 255));			//°Å·¡Ã¤ÆÃ
		break;

	case 9:
		SetFontTextColor(RGB(200, 128, 128));			//º¸Á¶ Á¤º¸Ã¢
		break;

	default:
		//		SetFontTextColor( hdc, RGB(180, 160, 255) );
		SetFontTextColor(RGB(255, 255, 192));
	}

	return TRUE;
}

static int	DisplayChatFilter = -1;
int	DispChatMsgHeight = 6;
int	DispChatMode = 0;

int delayValeLevel = 0;

int funcValeLevel(int LevelToGo, DWORD ItemCode, DWORD ItemHead, DWORD ItemChkSum)
{
	int flag = 0;

	if (LevelToGo == 1 && (sinChar->Level < 110 || sinChar->Level > 119)) flag = 1;
	if (LevelToGo == 2 && (sinChar->Level < 120 || sinChar->Level > 129)) flag = 1;
	if (LevelToGo == 3 && (sinChar->Level < 130 || sinChar->Level > 134)) flag = 1;
	//if (LevelToGo == 4 && (sinChar->Level < 125 || sinChar->Level > 130)) flag = 1;

	if (flag)
	{
		TitleBox::GetInstance()->SetText("Nível incompatível", 3);
		CHATBOX->AddMessage("> Nível Incompatível");
		return FALSE;
	}

	if (needRelog)//GetTickCount() - delayValeLevel <= 60000)
		return -2;
	//else
		//delayValeLevel = GetTickCount();

	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = smTRANSCODE_VALE_LEVEL;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = LevelToGo;
	smTransCommand.SParam = ItemCode;
	smTransCommand.LParam = ItemHead;
	smTransCommand.EParam = ItemChkSum;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);

	needRelog = 1;

	return TRUE;
}

int	SetChatMsgFilter(int mode)
{

	DisplayChatFilter = -1;
	ChatScrollPoint = 0;

	switch (mode) {
	case 0:			//ÀüÃ¼
		DisplayChatFilter = -1;
		break;

	case 1:			//Å¬·£
		DisplayChatFilter = 5;
		break;

	case 2:			//µ¿·á
		DisplayChatFilter = 4;
		break;

	case 3:			//°Å·¡
		DisplayChatFilter = 6;
		break;

	case 4:			//±Ó¸»
		DisplayChatFilter = 1;
		break;
	}

	DispChatMode = mode;

	return TRUE;
}

int mCnt, nCnt;

#include "Utils\\strings.h"


int ChatBuffSort()
{
	int cnt, cnt2;
	int mCnt, nCnt;
	int	Col;

	if (ChatBuffCnt <= ChatBuffCompCnt) return FALSE;

	for (cnt = ChatBuffCompCnt; cnt < ChatBuffCnt; cnt++)
	{
		mCnt = cnt & CHATBUFF_MASK;
		Col = 0;
		switch (ChatBuff[mCnt].dwIP)
		{
		case 5:
			Col = 1;
			break;
		case 4:
			Col = 2;
			break;

		case 6:
			Col = 3;
			break;
		case 1:
			Col = 4;
			break;

		case 9:
			Col = 5;
			break;
		}
		nCnt = ChatBuffCnt2[Col] & CHATBUFF_MASK;
		memcpy(&ChatBuff2[Col][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
		ChatBuffCnt2[Col]++;

		if (Col > 0 && Col < 5)
		{
			nCnt = ChatBuffCnt2[0] & CHATBUFF_MASK;
			memcpy(&ChatBuff2[0][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
			ChatBuffCnt2[0]++;
		}

		if (Col == 0 && ChatBuff[mCnt].dwIP < 10)
		{
			for (cnt2 = 1; cnt2 < 5; cnt2++)
			{
				nCnt = ChatBuffCnt2[cnt2] & CHATBUFF_MASK;
				memcpy(&ChatBuff2[cnt2][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
				ChatBuffCnt2[cnt2]++;
			}
		}
	}

	ChatBuffCompCnt = ChatBuffCnt;

	return TRUE;
}
//Ã¤ÆÃ Á¤º¸ È­¸é¿¡ Ç¥½Ã ( º¸Á¶ )
int DisplayChatMessage3(HDC hdc, int x, int y, int StrMax, int LineMax, int Mode)
{
	int result;
	int ModeBackup = DispChatMode;
	int	ScrollBackup = ChatScrollPoint;

	DispChatMode = Mode;
	if (LineMax <= 6)
		ChatScrollPoint = 0;

	result = DisplayChatMessage2(hdc, x, y, StrMax, LineMax);

	DispChatMode = ModeBackup;
	ChatScrollPoint = ScrollBackup;
	return result;
}

//Ã¤ÆÃ Á¤º¸ È­¸é¿¡ Ç¥½Ã
int DisplayChatMessage2(HDC hdc, int x, int y, int StrMax, int LineMax)
{
	int cnt;
	int chatCnt;
	char* lpString;
	int LineCnt;
	int CutPos;
	int len;
	int start, end;
	int fl_Count;

	DWORD shColor = RGB(74, 74, 74);


	//if ( ChatDispCnt==0 ) return FALSE;
	ChatBuffSort();

	LineCnt = 0;

	if (ChatScrollPoint > (CHATBUFF_MAX - 32))
		ChatScrollPoint = (CHATBUFF_MAX - 32);
	if (ChatScrollPoint < 0)
		ChatScrollPoint = 0;

	fl_Count = DispChatMode;
	if (fl_Count < 0) fl_Count = 0;

	start = ChatBuffCnt2[fl_Count] - 1 - ChatScrollPoint;
	end = ChatBuffCnt2[fl_Count] - 32 - ChatScrollPoint;

	if (start >= 12)
	{
		ShowIncoScrol = 1;
	}
	//	for( cnt=ChatBuffCnt-1;cnt>=ChatBuffCnt-32;cnt--) {

	DispChatMsgHeight = 0;

	for (cnt = start; cnt >= end; cnt--)
	{
		if (cnt >= 0)
		{
			chatCnt = cnt & CHATBUFF_MASK;

			//#ifdef _LANGUAGE_CHINESE //Áß±¹ Ã¤ÆÃÃ¢ Æ¯Á¤¾ð¾î »ç¿ë±ÝÁö
			//			lpString = ChatBuff2[fl_Count][chatCnt].szMessage;
			//			ConvertStringTHAI(lpString, lstrlen(lpString)+16);
			//#else
			lpString = ChatBuff2[fl_Count][chatCnt].szMessage;
			//#endif

			//if ( DisplayChatFilter<0 || DisplayChatFilter==ChatBuff[chatCnt].dwIP || ChatBuff[chatCnt].dwIP==0 || ChatBuff[chatCnt].dwIP==3 ) {


			DispChatMsgHeight++;			//³ôÀÌ ÃøÁ¤¿ë

			len = lstrlen(lpString);
			if (len > StrMax)
			{
				//ÄÆÆ®ÇÒ ¶óÀÎ Ä«¿îÅÍ
				CutPos = GetNextLineCount(lpString, StrMax);

				SetFontTextColor(shColor);
				dsTextLineOut(hdc, x - 1, y - 1, lpString + CutPos, len - CutPos);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
				dsTextLineOut(hdc, x, y, lpString + CutPos, len - CutPos);

				y -= 12;
				LineCnt++;
				if (LineCnt >= LineMax) break;

				SetFontTextColor(shColor);
				dsTextLineOut(hdc, x - 1, y - 1, lpString, CutPos);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);

				char semi[MAX_PATH], presemi[MAX_PATH], backsemi[MAX_PATH];
				ZeroMemory(presemi, MAX_PATH);
				ZeroMemory(backsemi, MAX_PATH);
				int bsemi = 0;
				strcpy(semi, lpString);
				int k = 0, prelen = 0, lenth = 0;
				lenth = strlen(semi);
				//¹®ÀÚ¿­ Â÷·Ê´ë·Î °Ë»ç
				for (int i = 0; i < lenth; i++)
				{
					backsemi[k] = semi[i];
					k++;
					if ((bsemi == 0) && (semi[i] == ':'))
					{
						strcpy(presemi, backsemi);
						ZeroMemory(backsemi, MAX_PATH);
						bsemi++;
						k = 0;
					}
				}
				//:ÀÖ¾úÀ¸¸é »ö±òÀ» ¹Ù²ÙÀÚ
				if (bsemi)
				{
					SIZE size;
					//:¾Õ¹®ÀÚ¿­
					prelen = strlen(presemi);
					SetFontTextColor(RGB(100, 255, 50));
					dsTextLineOut(hdc, x, y, presemi, prelen);
					GetFontTextExtentPoint(presemi, prelen, &size);
					//µÚ¹®ÀÚ¿­
					int j = 0;
					j = x + size.cx;
					SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
					dsTextLineOut(hdc, j, y, backsemi, CutPos - prelen);
				}
				else dsTextLineOut(hdc, x, y, lpString, CutPos);

				y -= 12;
				LineCnt++;
				if (LineCnt >= LineMax) break;

			}
			else
			{

				SetFontTextColor(shColor);
				dsTextLineOut(hdc, x - 1, y - 1, lpString, len);
				SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);

				char semi[MAX_PATH], presemi[MAX_PATH], backsemi[MAX_PATH];
				ZeroMemory(presemi, MAX_PATH);
				ZeroMemory(backsemi, MAX_PATH);
				int bsemi = 0;
				strcpy(semi, lpString);
				int k = 0, prelen = 0, lenth = 0;
				lenth = strlen(semi);
				//¹®ÀÚ¿­ Â÷·Ê´ë·Î °Ë»ç
				for (int i = 0; i < lenth; i++)
				{
					backsemi[k] = semi[i];
					k++;
					if ((bsemi == 0) && (semi[i] == ':'))
					{
						strcpy(presemi, backsemi);
						ZeroMemory(backsemi, MAX_PATH);
						bsemi++;
						k = 0;
					}
				}
				//:ÀÖ¾úÀ¸¸é »ö±òÀ» ¹Ù²ÙÀÚ
				if (bsemi)
				{
					SIZE size;
					//:¾Õ¹®ÀÚ¿­
					prelen = strlen(presemi);
					SetFontTextColor(RGB(100, 255, 50));
					dsTextLineOut(hdc, x, y, presemi, prelen);
					GetFontTextExtentPoint(presemi, prelen, &size);
					//µÚ¹®ÀÚ¿­
					int j = 0;
					j = x + size.cx;
					SetIpColor(hdc, ChatBuff2[fl_Count][chatCnt].dwIP);
					dsTextLineOut(hdc, j, y, backsemi, strlen(backsemi));
				}
				else dsTextLineOut(hdc, x, y, lpString, len);

				y -= 12;
				LineCnt++;
				if (LineCnt >= LineMax) break;
			}
			//}
		}
	}

	return TRUE;
}

//»õ ÇÃ·¹ÀÌ¾î¿Í ¿¬°á
int ConnectPlayer(smWINSOCK * lpsmSock)
{
	Utils_Log(LOG_DEBUG, "ConnectPlayer");
	int OthPlayerCnt;


	EnterCriticalSection(&cSection);

	OthPlayerCnt = FindNewOtherPlayer();
	if (OthPlayerCnt >= 0)
	{
		lpsmSock->ExtData1 = (void*)&chrOtherPlayer[OthPlayerCnt];
		chrOtherPlayer[OthPlayerCnt].Init();
		chrOtherPlayer[OthPlayerCnt].TransSendWait = 1;
		chrOtherPlayer[OthPlayerCnt].TransSock = lpsmSock;
		chrOtherPlayer[OthPlayerCnt].TransLastSendCnt = PlayCounter - 70;
		chrOtherPlayer[OthPlayerCnt].Flag = 1;
		chrOtherPlayer[OthPlayerCnt].smCharInfo.szName[0] = 0;
		chrOtherPlayer[OthPlayerCnt].ActionPattern = 99;
		chrOtherPlayer[OthPlayerCnt].Pattern = 0;
		chrOtherPlayer[OthPlayerCnt].AutoPlayer = 0;

		lpCurPlayer->SendCharInfo(lpsmSock);
		//		SendPlayerInfo( lpsmSock );

	}

	//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
	LeaveCriticalSection(&cSection);

	return TRUE;
}

//ÇÃ·¹ÀÌ¾î ¿¬°á ²ö¾îÁü
int DisconnectPlayer(smWINSOCK * lpsmSock)
{
	//	int cnt;
	smCHAR* lpPlayer;
	DWORD	dwTime;

	dwTime = GetCurrentTime();

	if (smWsockServer && lpsmSock->sock == smWsockServer->sock)
	{
		CHATGAMEHANDLE->AddChatBoxText("> Conexão Perdida com o servidor", EChatColor::CHATCOLOR_Error);
		if (smWsockServer == smWsockDataServer)
			smWsockDataServer = 0;
		if (smWsockServer == smWsockUserServer)
			smWsockUserServer = 0;
		if (smWsockServer == smWsockExtendServer)
			smWsockExtendServer = 0;

		smWsockServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 14");
		DisconnectFlag = dwTime;
		DisconnectServerCode = 1;

		//°Å·¡ÁßÀÌ¸é °Å·¡ Ãë¼Ò ½ÃÅ´
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//°Å·¡ Ãë¼Ò ¿ä±¸
			cTrade.CancelTradeItem();								//°Å·¡Ã¢ ´Ý±â
		}
		return TRUE;
	}

	if (smWsockDataServer && lpsmSock->sock == smWsockDataServer->sock)
	{
		CHATGAMEHANDLE->AddChatBoxText("> Conexão Perdida com o banco de dados", EChatColor::CHATCOLOR_Error);
		smWsockDataServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 15");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 2;

		//°Å·¡ÁßÀÌ¸é °Å·¡ Ãë¼Ò ½ÃÅ´
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//°Å·¡ Ãë¼Ò ¿ä±¸
			cTrade.CancelTradeItem();								//°Å·¡Ã¢ ´Ý±â
		}
		return TRUE;
	}

	if (smWsockUserServer && lpsmSock->sock == smWsockUserServer->sock) {
		CHATGAMEHANDLE->AddChatBoxText("> Conexão Perdida com o servidor", EChatColor::CHATCOLOR_Error);
		smWsockUserServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 16");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//°Å·¡ÁßÀÌ¸é °Å·¡ Ãë¼Ò ½ÃÅ´
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//°Å·¡ Ãë¼Ò ¿ä±¸
			cTrade.CancelTradeItem();					//°Å·¡Ã¢ ´Ý±â
		}
		return TRUE;
	}

	if (smWsockExtendServer && lpsmSock->sock == smWsockExtendServer->sock) {
		CHATGAMEHANDLE->AddChatBoxText("> Conexão Perdida com o servidor", EChatColor::CHATCOLOR_Error);
		smWsockExtendServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 18");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//°Å·¡ÁßÀÌ¸é °Å·¡ Ãë¼Ò ½ÃÅ´
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);		//°Å·¡ Ãë¼Ò ¿ä±¸
			cTrade.CancelTradeItem();							//°Å·¡Ã¢ ´Ý±â
		}
		return TRUE;

	}

	char szBuff[256];

	//Áö¿¬ ¼­¹ö ¿¬°á ²ö¾îÁü
	if (AreaServerMode) {
		if (lpWSockServer_Area[0] && lpWSockServer_Area[0]->sock == lpsmSock->sock) {
			if (smConfig.DebugMode) {
				CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "> Conexão perdida (%s)", lpWSockServer_Area[0]->szIPAddr);
			}
			if (lpWSockServer_Area[0] == lpWSockServer_DispArea[0]) lpWSockServer_DispArea[0] = 0;
			if (lpWSockServer_Area[0] == lpWSockServer_DispArea[1]) lpWSockServer_DispArea[1] = 0;

			lpWSockServer_Area[0] = 0;
			return TRUE;
		}
		if (lpWSockServer_Area[1] && lpWSockServer_Area[1]->sock == lpsmSock->sock) {
			if (smConfig.DebugMode) {
				CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "> Conexão perdida (%s)", lpWSockServer_Area[1]->szIPAddr);
			}

			if (lpWSockServer_Area[1] == lpWSockServer_DispArea[0]) lpWSockServer_DispArea[0] = 0;
			if (lpWSockServer_Area[1] == lpWSockServer_DispArea[1]) lpWSockServer_DispArea[1] = 0;

			lpWSockServer_Area[1] = 0;
			return TRUE;
		}
	}



	//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
	EnterCriticalSection(&cSection);

	lpPlayer = (smCHAR*)lpsmSock->ExtData1;
	if (lpPlayer) {
		lpPlayer->Close();
	}

	//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
	LeaveCriticalSection(&cSection);

	return TRUE;
}



//¸¶Áö¸·À¸·Î ¼­¹ö¿¡ µ¥ÀÌÅ¸¸¦ º¸³½ Ä«¿îÅÍ
int Ts_LastSendCounter;
DWORD	dwLastSendPosiTime = 0;		//¸¶Áö¸· Àü¼Û ½Ã°£ (À§Ä¡)
DWORD	dwLastSendPlayTime = 0;		//¸¶Áö¸· Àü¼Û ½Ã°£ (¸ð¼Çµ¥ÀÌÅ¸)

//¼­¹ö·Î ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸¸¦ À§Ä¡¸¦ Àü¼Û
int SendPlayDataToServer()
{
	/*
	int cnt;
	int	*lpTransBuff;
	rsPLAYPOS *lpTransPlayPos;

	lpTransBuff = (int *)TransBuff;
	lpTransPlayPos = (rsPLAYPOS *)&lpTransBuff[2];
	lpTransBuff[0] = sizeof( rsPLAYPOS ) + sizeof( int )*2;
	lpTransBuff[1] = smTRANSCODE_POSITION;

	lpTransPlayPos->Area = 0;
	lpTransPlayPos->x = lpCurPlayer->pX;
	lpTransPlayPos->y = lpCurPlayer->pY;
	lpTransPlayPos->z = lpCurPlayer->pZ;

	smWsockServer->Send( TransBuff , lpTransBuff[0] );

	return TRUE;
	*/

	/*
		if ( smWsockServer!=smWsockUserServer && rsRecvServer ) {
			rsRecvServer = 0;
			return rsMainServer.SendPlayData( lpCurPlayer );
		}
	*/
	if (smWsockServer != smWsockUserServer || AreaServerMode) {
		return rsMainServer.SendPlayData(lpCurPlayer);
	}

	return TRUE;
}

int ConnectOtherPlayer(DWORD dwIP)
{
	smWINSOCK* lpsmSock;
	int OthPlayerCnt;

	OthPlayerCnt = FindNewOtherPlayer();
	if (OthPlayerCnt >= 0) {
		lpsmSock = smConnectSock2(ConvStrIP(dwIP), TCP_GAMEPORT);
		if (lpsmSock) {
			ConnectPlayer(lpsmSock);
			return TRUE;
		}
	}
	return FALSE;
}

DWORD WINAPI ConnectPlayThreadProc(void* pInfo)
{

	ConnectOtherPlayer((DWORD)pInfo);

	ExitThread(TRUE);
	return TRUE;
}
//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°á Á¾·á ¾²·¹µå
DWORD WINAPI DisconnectPlayThreadProc(void* pInfo)
{
	Utils_Log(3, "DisconnectPlayThreadProc");
	((smWINSOCK*)pInfo)->CloseSocket();

	ExitThread(TRUE);
	return TRUE;
}



//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°á ¾²·¹µå¸¦ »ý¼º
int CreateConnectThread(DWORD dwIP)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, ConnectPlayThreadProc, (void*)dwIP, 0, &dwMsgId);

	return TRUE;
}

int CreateDisconnectThread(smWINSOCK * lpsmSock)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, DisconnectPlayThreadProc, (void*)lpsmSock, 0, &dwMsgId);

	return TRUE;
}

/// <summary>
/// Verifica a distância do jogador em relação aos outros jogadores
/// </summary>
/// <returns>Atualiza o chrOtherPlayer pela distância ou conexão dos outros jogadores</returns>
int DisconnectFarPlayer()
{
	int mx, mz, dDist;
	int cnt;
	int timeover;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++)
	{
		if (chrOtherPlayer[cnt].Flag) 
		{
			if ((chrOtherPlayer[cnt].dwLastTransTime + DIS_TIME_OVER) < dwPlayTime)
				timeover = TRUE;
			else
				timeover = FALSE;

			mx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
			mz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
			dDist = mx * mx + mz * mz;

			if ((chrOtherPlayer[cnt].DisplayFlag && dDist > DIST_TRANSLEVEL_DISCONNECT) || timeover)
			{
				chrOtherPlayer[cnt].Flag = 0;

				if (chrOtherPlayer[cnt].TransSock)
				{
					Utils_Log(3, "netplay.cpp 6083");
					chrOtherPlayer[cnt].TransSock->CloseSocket();
				}
				else
				{
					EnterCriticalSection(&cSection);
					chrOtherPlayer[cnt].Close();
					LeaveCriticalSection(&cSection);
				}
			}
		}
	}

	return TRUE;
}

DWORD	dwExtendServ_RecvTime = 0;
DWORD	dwUserServ_RecvTime = 0;
DWORD	dwMainServ_RecvTime = 0;
DWORD	dwDataServ_RecvTime = 0;

int RecvPlayData(smTHREADSOCK * pData)
{
	DWORD	dwTime;

	EnterCriticalSection(&cSection);

	dwTime = GetCurrentTime();

	if (pData->smMySock == smWsockDataServer)
	{
		dwRecvDataServerTime = dwTime;
		dwDataServ_RecvTime = dwTime;
	}
	if (pData->smMySock == smWsockUserServer)
	{
		dwRecvUserServerTime = dwTime;
		dwUserServ_RecvTime = dwTime;
		Debug_RecvCount2++;
	}
	if (pData->smMySock == smWsockExtendServer) {
		dwRecvExtendServerTime = dwTime;
		dwExtendServ_RecvTime = dwTime;
		Debug_RecvCount3++;
	}
	if (pData->smMySock == smWsockServer) {
		Debug_RecvCount1++;
		dwRecvServerTime = dwTime;
		dwMainServ_RecvTime = dwTime;
	}

	rsMainServer.RecvMessage(pData);

	LeaveCriticalSection(&cSection);

	return TRUE;
}

int PlayRecvMessageQue()
{
	return rsMainServer.RecvMessageQue();
}


int LastSendCnt;

#define	NET_SENDING_WAIT_TIME		400
#define	NET_SENDING_WAIT_TIME2		1000

int NetStandCnt = 0;
extern int GameMode;

DWORD	NetScoopTime = 2000;

int NetWorkPlay()
{
	//	char *szIP;
	int cnt;
	DWORD	dwTime;
	DWORD	dwTime2;
	smWINSOCK* lpsmSock;

	//¹ÚÀç¿ø - XignCode
#ifdef _XIGNCODE_CLIENT
	ZCWAVE_Init();
#endif

	if (NetWorkInitFlag == 0)
	{
		if (!smWsockServer || !smWsockDataServer || !smWsockUserServer || !smWsockExtendServer)
		{
			if (ConnectServer_Main() == NULL)
				return FALSE;
			//smWsockServer = smConnectSock( szServIP , dwServPort );
		}

		//Sleep( 60*1000*3 );		//3ºÐ ´ë±â

		NetWorkInitFlag = TRUE;

		rsMainServer.Init(smWsockServer);

		//¼­¹ö·Î ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸¸¦ À§Ä¡¸¦ Àü¼Û
		SendPlayDataToServer();

		//¼­¹ö¿¡ ±ÇÇÑ ¼³Á¤
		if (smConfig.DebugMode)
			SendAdminMode(TRUE);


		//		if ( smWsockServer )
		//			lpCurPlayer->SendCharInfo( smWsockServer );			//ÇÃ·¹ÀÌ¾î Á¤º¸ Àü¼Û

				//ÀúÀåµÈ Ä³¸¯ÅÍ µ¥ÀÌÅ¸ ºÒ·¯¿À±â ¿ä±¸
		if (smWsockDataServer)
		{
			SendGetRecordData(lpCurPlayer->smCharInfo.szName);

			//¼­¹ö IP Ã½Å© ( ±º¼­¹ö¿¡ Á¢¼ÓÇÑ IPÅëº¸ )
			if (smWsockServer && smWsockDataServer != smWsockServer)
			{
				SendCheckIP(1, smWsockServer);
			}
			if (smWsockUserServer && smWsockDataServer != smWsockUserServer)
			{
				SendCheckIP(2, smWsockUserServer);
			}
			if (smWsockExtendServer && smWsockExtendServer != smWsockServer && smWsockDataServer != smWsockExtendServer)
			{
				SendCheckIP(3, smWsockExtendServer);
			}
		}

		ChatBuffCnt = 0;
		ChatDispCnt = 0;

		Ts_LastSendCounter = PlayCounter;
		TransThrowItem.code = 0;			//¾ÆÀÌÅÛ ¹ö¸®±â ÃÊ±âÈ­

		RecordFailCount = 0;			//ÀúÀå ½ÇÆÐ Ä«¿îÅÍ
		dwLastWareHouseChkSum = 0;		//Ã¢°í Ã¼Å©¼¶ ÄÚµå
		dwLastCaravanChkSum = 0;

		TransAgingItemFlag = 0;			//¿¡ÀÌÂ¡ ¿Ï·á ¾ÆÀÌÅÛ Àü¼Û ÇÃ·¢


		ZeroMemory(&TransRecvItem, sizeof(TRANS_ITEMINFO));

		Trans_SplashCharList.code = 0;	//½ºÇÃ·¹½Ã °ø°Ý ¹öÆÛ ÃÊ±âÈ­

		ZeroMemory(&PK_FieldState, sizeof(PK_FIELD_STATE));	//PK ±¸Á¶ ÃÊ±âÈ­
		PK_FieldState.FieldCode = -1;


		//¸ó½ºÅÍ ÄÚµå Å¥¿¡ °ü¸®
		ClearAttMonsterCode();

		Init_RecordDamage();			//°ø°Ý·Â ±â·Ï ÃÊ±âÈ­

		switch (smConfig.NetworkQuality)
		{
		case 0:
			NetScoopTime = 3000;
			break;
		case 1:
			NetScoopTime = 2000;
			break;
		case 2:
			NetScoopTime = 1000;
			break;
		case 3:
			NetScoopTime = 5000;
			smTransTurbRcvMode = 50;
			break;
		}

	}
	else
	{
		if (cTrade.OpenFlag)
		{
			if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag) {
				SendTradeCheckItem(cTrade.TradeCharCode);
				TradeSendSucessFlag = TRUE;
			}
		}
		if (dwTradeMaskTime)
		{
			if (dwTradeMaskTime < dwPlayTime) dwTradeMaskTime = 0;
		}

		dwTime = dwPlayTime - dwLastSendPosiTime;
		dwTime2 = dwPlayTime - dwRecvServerTime;

		if ((PlayCounter - Ts_LastSendCounter) > PLAYSERVER_SENDCOUNTER)
		{
			SendPlayDataToServer();

			// Verifica distância do jogador entre outros players (chrotherplayer)
			DisconnectFarPlayer();

			Ts_LastSendCounter = PlayCounter;
			dwLastSendPosiTime = dwPlayTime;
		}

		if (smWsockUserServer)
		{
			dwTime = dwPlayTime - dwLastSendPlayTime;
			dwTime2 = dwPlayTime - dwRecvUserServerTime;


			if ((lpCurPlayer->PlayBuffCnt & 0x3F) == 0 && lpCurPlayer->dwObjectSerial &&
				dwTime > NET_SENDING_WAIT_TIME)
			{

				if (smConfig.DebugMode && AdminCharDisable) {
					if (smWsockServer == smWsockUserServer) {
						rsMainServer.SendPlayData(lpCurPlayer);
					}
				}
				else {
					if (AreaServerMode) {
						if (lpCurPlayer->OnStageField >= 0) {
							lpsmSock = lpWSockServer_DispArea[lpCurPlayer->OnStageField];
						}

					}
					else {
						lpsmSock = smWsockUserServer;
					}

					if (lpsmSock) {
						lpCurPlayer->MakeTransPlayData(lpCurPlayer->srTransBuff, 0x40);
						if (((smTRANS_PLAYDATA*)lpCurPlayer->srTransBuff)->PlayBuffCnt > 1) {
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send(lpCurPlayer->srTransBuff, ((smTRANS_PLAYDATA*)lpCurPlayer->srTransBuff)->size, TRUE);
							NetStandCnt = 0;
						}
						else {
							//¼­ÀÖ´Â µ¿ÀÛ ³×Æ®¿÷ ºÎÇÏ ÃÖ¼Ò ( ÇÑ¹ø Àü¼ÛÈÄ ³×Æ®Ÿp ½½¸³ )
							//if ( NetStandCnt==0 )
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send(lpCurPlayer->srTransBuff, ((smTRANS_PLAYDATA*)lpCurPlayer->srTransBuff)->size, TRUE);
							NetStandCnt++;
						}
					}
				}

				Debug_SendCount++;

				rsRecvUserServer = FALSE;
				dwLastSendPlayTime = dwPlayTime;
			}
		}

		//ÇöÀç °ÔÀÓ ·Îµù Áß
		if (dwLoadingTime) {
			dwTime = GetCurrentTime();
			if (dwTime > (dwLoadingTime + NET_LOADING_WAIT_LIMIT)) {
				//½Ã°£ ÃÊ°ú Á¾·á
				Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 19");
				DisconnectFlag = GetCurrentTime();
				dwLoadingTime = 0;
			}
		}


		cnt = PlayCounter & 0x1FF;
		if (cnt == 0) {
			//µ¥µå¶ô Ã½Å© ( ¸Þ¼¼Áö°¡ µ¹¾Æ ¿ÀÁö ¾ÊÀ¸¸é ³×Æ®¿÷ ´Ù¿î )
			TransCheckNetwork();
		}
		else if (cnt == 0x80) {
			//ÇÁ·Î¼¼½º ½Ã°£ °¨½ÃÁ¤º¸ ¼­¹ö·Î ¼ÛºÎ
			SendPlayTimerMax();
		}


		if ((PlayCounter & 0x1F) == 0) {
			dwTime = GetCurrentTime();
			if (GameMode == 2 && dwMainServ_RecvTime && smWsockServer && (dwTime - dwMainServ_RecvTime) > 3000) {

				if (smWsockServer->WaitReadMessage >= 0) {
					smWsockServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						CHATGAMEHANDLE->AddChatBoxText("> Scoop Recv Route (Main)", EChatColor::CHATCOLOR_Error);
				}
				else {
					if (smConfig.DebugMode)
						CHATGAMEHANDLE->AddChatBoxText("> Error Server (Main)", EChatColor::CHATCOLOR_Error);
				}
			}
			if (GameMode == 2 && smWsockUserServer && dwUserServ_RecvTime &&
				smWsockServer != smWsockUserServer && (dwTime - dwUserServ_RecvTime) > 3000) {
				//dwUserServ_RecvTime = dwTime;
				if (smWsockUserServer->WaitReadMessage >= 0) {
					smWsockUserServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						CHATGAMEHANDLE->AddChatBoxText("> Scoop Recv Route (User)", EChatColor::CHATCOLOR_Error);
				}
				else {
					if (smConfig.DebugMode)
						CHATGAMEHANDLE->AddChatBoxText("> Error Server (User)", EChatColor::CHATCOLOR_Error);
				}
			}
			if (GameMode == 2 && smWsockExtendServer && dwExtendServ_RecvTime &&
				smWsockServer != smWsockExtendServer && (dwTime - dwExtendServ_RecvTime) > 3000) {
				//dwUserServ_RecvTime = dwTime;
				if (smWsockExtendServer->WaitReadMessage >= 0) {
					smWsockExtendServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						CHATGAMEHANDLE->AddChatBoxText("> Scoop Recv Route (Extend)", EChatColor::CHATCOLOR_Error);
				}
				else {
					if (smConfig.DebugMode)
						CHATGAMEHANDLE->AddChatBoxText("> Error Server (Extend)", EChatColor::CHATCOLOR_Error);
				}
			}

			if (GameMode == 2 && ((dwTime - dwDataServ_RecvTime) > 7000 || dwLoadingTime) && smWsockDataServer && smWsockDataServer != smWsockServer)
			{
				smWsockDataServer->WaitReadMessage += 24;
				if (smConfig.DebugMode && DisplayDebug)
					CHATGAMEHANDLE->AddChatBoxText("> Scoop Recv Route (Data)", EChatColor::CHATCOLOR_Error);
			}

			if (GameMode == 2 && AreaServerMode)
			{
				if (lpWSockServer_Area[0] && lpWSockServer_Area[0]->dwLastRecvTime)
				{
					if ((dwTime - lpWSockServer_Area[0]->dwLastRecvTime) > 2000)
					{
						lpWSockServer_Area[0]->WaitReadMessage += 24;
						smCheckWaitMessage(lpWSockServer_Area[0]);
					}
				}
				if (lpWSockServer_Area[1] && lpWSockServer_Area[1]->dwLastRecvTime)
				{
					if ((dwTime - lpWSockServer_Area[1]->dwLastRecvTime) > 2000)
					{
						lpWSockServer_Area[1]->WaitReadMessage += 24;
						smCheckWaitMessage(lpWSockServer_Area[1]);
					}
				}

			}

		}

	}
#ifndef _LANGUAGE_CHINESE	//À©µµ¿ì ¸ðµå
#ifndef _LANGUAGE_VEITNAM	//À©µµ¿ì ¸ðµå
#ifndef _LANGUAGE_PHILIPIN	//À©µµ¿ì ¸ðµå
#ifndef _LANGUAGE_ENGLISH	//À©µµ¿ì ¸ðµå
	HWND	hWnd;

	if ((PlayCounter & 0x1FF) == 0 && !smConfig.DebugMode) {
		hWnd = WinFocusCrack();
		//À©µµ¿ì Æ÷Ä¿½º¸¦ °¨½Ã
		if (hWnd) {
			//Å©·¢ À©µµ¿ì ½Å°í
			SendCrackWindow(hWnd);
}
	}
#endif
#endif
#endif
#endif


	//°Å·¡ÈÄ È®ÀÎ»ï¾Æ Àç½Ãµµ ( 0.8ÃÊÈÄ )
	if (dwLastTransTradeKeyTime && (dwLastTransTradeKeyTime + 800) < dwPlayTime)
	{
		if (smWsockDataServer)
			smWsockDataServer->Send2((char*)&LastTransTradeItemKey, LastTransTradeItemKey.size, TRUE);

		dwLastTransTradeKeyTime = 0;
	}


	if (ChatDispCnt > 0) ChatDispCnt--;

	return TRUE;
}


//¾ÆÀÌÅÛ ¹ö¸®±â
int ThrowPutItem(sITEM * lpItem, int x, int y, int z)
{
	if (lpItem->CODE == 117506304)
		return FALSE;

	//¼­¹ö »óÅÂ¿¡ ¹®Á¦ ¹ß»ý½Ã ¾ÆÀÌÅÛ ¹ö¸®±â ±ÝÁö ( 4ÃÊ ÀÌ»ó ¼ö½ÅÀÌ ¾ø´Â »óÅÂ )
	if ((dwRecvServerTime + 4000) < dwPlayTime || QuitSave || TransThrowItem.code)
		return FALSE;

	if (AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer)
		return FALSE;

	//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
	if (!SendSaveThrowItem2(&lpItem->sItemInfo))
		return FALSE;

	TransThrowItem.code = smTRANSCODE_THROWITEM;
	TransThrowItem.size = sizeof(TRANS_ITEMINFO);
	TransThrowItem.x = x;
	TransThrowItem.y = y;
	TransThrowItem.z = z;

	memcpy(&TransThrowItem.Item, &lpItem->sItemInfo, sizeof(sITEMINFO));

	return TRUE;
}
//¾ÆÀÌÅÛ ¹ö¸®±â
int ThrowPutItem2(sITEMINFO * lpItem, int x, int y, int z)
{

	//¼­¹ö »óÅÂ¿¡ ¹®Á¦ ¹ß»ý½Ã ¾ÆÀÌÅÛ ¹ö¸®±â ±ÝÁö ( 4ÃÊ ÀÌ»ó ¼ö½ÅÀÌ ¾ø´Â »óÅÂ )
	if ((dwRecvServerTime + 4000) < dwPlayTime || QuitSave || TransThrowItem.code)
		return FALSE;

	if (AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer)
		return FALSE;

	//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
	if (!SendSaveThrowItem2(lpItem))
		return FALSE;

	TransThrowItem.code = smTRANSCODE_THROWITEM;
	TransThrowItem.size = sizeof(TRANS_ITEMINFO);
	TransThrowItem.x = x;
	TransThrowItem.y = y;
	TransThrowItem.z = z;

	memcpy(&TransThrowItem.Item, lpItem, sizeof(sITEMINFO));

	/*
		if ( lpItem->CODE==(sinGG1|sin01) ) {
			SendSaveMoney();
		}
	*/
	//return smWsockServer->Send( (char *)&TransItemInfo , TransItemInfo.size , TRUE );

	return TRUE;
}


//¹ö¸± ¾ÆÀÌÅÛ ÀúÀå
//TRANS_ITEMINFO	TransThrowItem;
//			ThrowItemToServer((TRANS_ITEMINFO *)pData->Buff );

int	ThrowPotionCount = 0;

//ÀÓ½Ã ÀúÀåµÈ ¹ö¸± ¾ÆÀÌÅÛÀ» È®ÀÎÇÏ¿© ÀÏÄ¡ÇÏ¸é ¼­¹ö·Î º¸³¿
int ThrowItemToServer(smTRANS_COMMAND_EX * lpTransCommand)
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK* lpsmSock;
	DWORD	dwItemCodeMask;
	int		cnt;

	if (TransThrowItem.code &&
		TransThrowItem.Item.CODE == (DWORD)lpTransCommand->WParam &&
		TransThrowItem.Item.ItemHeader.Head == (DWORD)lpTransCommand->LParam &&
		TransThrowItem.Item.ItemHeader.dwChkSum == (DWORD)lpTransCommand->SParam) {

		if ((TransThrowItem.Item.CODE & sinITEM_MASK1) != (sinPM1 & sinITEM_MASK1)) {
			cInvenTory.DeleteInvenItemToServer(TransThrowItem.Item.CODE, TransThrowItem.Item.ItemHeader.Head, TransThrowItem.Item.ItemHeader.dwChkSum);

			//ÀÎº¥Åä¸®ÀÇ ÁöÁ¤ÇÑ ¾ÆÀÌÅÛÀ» Ã£¾Æ °Ë»öÇÏ¿© Ã£´Â´Ù
			if (FindInvenItem(TransThrowItem.Item.CODE,
				TransThrowItem.Item.ItemHeader.Head,
				TransThrowItem.Item.ItemHeader.dwChkSum) != 0) {

				SendInvenItemError(0, TransThrowItem.Item.CODE,
					TransThrowItem.Item.ItemHeader.Head,
					TransThrowItem.Item.ItemHeader.dwChkSum);

				TransThrowItem.code = 0;
				return TRUE;
			}
		}
		else {
			if ((dwPlayTime - dwLastRecordTime) > 4000 && TransThrowItem.Item.PotionCount >= 10) {
				//¹°¾àÀÌ 10°³ ÀÌ»ó ±×·ìÀ¸·Î ¹ö¸±°æ¿ì (ÀúÀå / ¹°¾àº¹»ç 10°³ÀÌ»ó ¾ÈµÇµµ·Ï )
				SaveGameData();
				ThrowPotionCount = 0;
			}
			else {
				ThrowPotionCount += TransThrowItem.Item.PotionCount;
				if (ThrowPotionCount >= 15) {		//¹°¾à ³ª°£ °¹¼ö°¡ 15°³ ÃÊ°ú¸é ÀúÀåÇÏ°í ÇÃ·¢ ÃÊ±âÈ­
					ThrowPotionCount = 0;
					SaveGameData();
				}
			}
		}

		if ((TransThrowItem.Item.CODE & sinITEM_MASK2) == sinBC1) {

			if (TransThrowItem.Item.PotionCount == 100)		//ÀÓ½Ã 100 µé¾î¿À¸é 200À¸·Î ¹Ù²Þ
				TransThrowItem.Item.PotionCount = 200;

			if (TransThrowItem.Item.CODE == (sinBC1 | sin04) && TransThrowItem.Item.PotionCount == 200) {
				//ÆÄÆ¼ È¸º¹ ( ¼ÛÆí°ú °°À½ )
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.code = smTRANSCODE_PARTY_POTION;
				smTransCommand.WParam = 3000;	//»ý¸í·Â
				smTransCommand.LParam = 3000;	//±â·Â
				smTransCommand.SParam = 3000;	//±Ù·Â
				smTransCommand.EParam = lpCurPlayer->dwObjectSerial;

				if (smWsockUserServer)
					smWsockUserServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

				SetPartyPosion(&smTransCommand);

				TransThrowItem.code = 0;
				return TRUE;
			}
		}

		if (!TransThrowItem.Item.SpecialItemFlag[0]) {		//( º¹»ç ÁõÁ¤¿ë ¾ÆÀÌÅÛÀº »ç¶óÁü )
			TransThrowItem.dwSeCode[0] = lpTransCommand->WxParam;
			TransThrowItem.dwSeCode[1] = lpTransCommand->LxParam;
			TransThrowItem.dwSeCode[2] = lpTransCommand->SxParam;
			TransThrowItem.dwSeCode[3] = lpTransCommand->ExParam;

			dwItemCodeMask = TransThrowItem.Item.CODE & sinITEM_MASK2;

			// Àåº° - ¼Ò¿ï½ºÅæ
			if (((dwItemCodeMask == sinGP1 || dwItemCodeMask == sinCH1 || dwItemCodeMask == sinFO1 || dwItemCodeMask == sinGP2) && TransThrowItem.Item.PotionCount == 100) ||
				(TransThrowItem.Item.CODE == (sinGG1 | sin01) && TransThrowItem.Item.ItemKindCode == smTRANSCODE_COLLECTMONEY) ||
				(dwItemCodeMask == sinBC1 && TransThrowItem.Item.PotionCount == 200) ||
				(dwItemCodeMask == sinBI1 && TransThrowItem.Item.PotionCount == 300) ||
				(dwItemCodeMask == sinBI2 && TransThrowItem.Item.PotionCount == 300))
			{
				lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
				if (lpsmSock)
					lpsmSock->Send2((char*)&TransThrowItem, TransThrowItem.size, TRUE);
				else
					smWsockServer->Send2((char*)&TransThrowItem, TransThrowItem.size, TRUE);

				if (dwItemCodeMask == sinFO1) {
					//Æ÷½º¿Àºê ½ÃÀü
					cnt = (TransThrowItem.Item.CODE & sinITEM_MASK3) >> 8;
					cnt--;
					if (cnt >= 0 && cnt < 16) {  // ¹ÚÀç¿ø : ºô¸µ Æ÷½º Ãß°¡·Î Æ÷½º °¹¼ö 12·Î ¿¬Àå
						lpCurPlayer->dwForceOrbCode = TransThrowItem.Item.CODE;		//Æ÷½º¿Àºê Àû¿ë ÄÚµå
						lpCurPlayer->dwForceOrbTime = dwPlayTime + ForceOrbUseTime[cnt] * 1000;
						AssaParticle_ShelltomWeapon(lpCurPlayer, ForceOrbUseTime[cnt] * 70, cnt);

						//ÁÖº¯ Ä³¸¯ÅÍ¿¡ ÀÌº¥Æ® ¸í·É ÄÚµå º¸³»±â
						SendCommandUser(smCOMMNAD_USER_FORCEORB, 0, 0, 0);
						StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
						SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½
					}
					// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º Ãß°¡
					else if (cnt >= 20 && cnt < 32) {
						lpCurPlayer->dwForceOrbCode = TransThrowItem.Item.CODE;		//Æ÷½º¿Àºê Àû¿ë ÄÚµå
						lpCurPlayer->dwForceOrbTime = dwPlayTime + MagicForceOrbUseTime[cnt - 20] * 1000;
						// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º´Â ÀÏ¹Ý Æ÷½º¿Í °°Àº ÀÌÆåÆ®·Î Àû¿ëÇÑ´Ù.
						AssaParticle_ShelltomWeapon(lpCurPlayer, MagicForceOrbUseTime[cnt - 20] * 70, cnt); //

						//ÁÖº¯ Ä³¸¯ÅÍ¿¡ ÀÌº¥Æ® ¸í·É ÄÚµå º¸³»±â
						SendCommandUser(smCOMMNAD_USER_FORCEORB, 0, 0, 0);
						StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
						SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½
					}
					else if (cnt >= 34 && cnt < 37) {
						lpCurPlayer->dwForceOrbCode = TransThrowItem.Item.CODE;		//Æ÷½º¿Àºê Àû¿ë ÄÚµå
						lpCurPlayer->dwForceOrbTime = dwPlayTime + BillingMagicForceOrbUseTime[cnt - 34] * 1000;
						// ¹ÚÀç¿ø - ºô¸µ ¸ÅÁ÷ Æ÷½º´Â ¹Ì¶óÁö, ÀÎÆä¸£³ª, ÀÌ´Ï±×¸¶¿Í °°Àº ÀÌÆåÆ®·Î Àû¿ëÇÑ´Ù.
						AssaParticle_ShelltomWeapon(lpCurPlayer, BillingMagicForceOrbUseTime[cnt - 34] * 70, cnt); //

						//ÁÖº¯ Ä³¸¯ÅÍ¿¡ ÀÌº¥Æ® ¸í·É ÄÚµå º¸³»±â
						SendCommandUser(smCOMMNAD_USER_FORCEORB, 0, 0, 0);
						StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
						SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½
					}
				}

				//À¯·á ¾ÆÀÌÅÛ »ç¿ë½Ã ½Ã°£ Ç¥½Ã
				if (dwItemCodeMask == sinBI1 || dwItemCodeMask == sinBI2) {
					if (TransThrowItem.Item.CODE == (sinBI1 | sin12)) {
						//chaPremiumitem.SetThirdEyesTime( PRIME_ITEM_TIME_THIRDEYES );
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_EXPUP);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin13)) {
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_EXPUP);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin14)) {
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_3DAY);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin15)) {
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_3DAY);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin23)) {	//ÆøÁ×
						cnt = (rand() % 8192) * 2;
						DWORD	dwTable = GetTableNum(cnt);
						AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime, (dwTable >> 24) & 0xFF, 0, 0, lpCurPlayer);
						AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 500, (dwTable >> 16) & 0xFF, 0, 0, lpCurPlayer);
						AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 1000, (dwTable >> 8) & 0xFF, 0, 0, lpCurPlayer);
						AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 1500, dwTable & 0xFF, 0, 0, lpCurPlayer);
						lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_YAHOO;
						SendCommandUser(smCOMMNAD_USER_FIREFLOWER, cnt, 0, 0);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin24)) {		//º£Æ®³²¿äÃ»
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_EXPUP);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin25)) {		//º£Æ®³²¿äÃ»
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_EXPUPx7);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin26))	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ 7ÀÏ
					{
						chaPremiumitem.SetMightOfAwellTime(PRIME_ITEM_TIME_EXPUPx7);
						chaPremiumitem.SetMightOfAwellWeight(300);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin27))	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ 30ÀÏ
					{
						//chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_PACKAGE4 );
						chaPremiumitem.SetMightOfAwellTime(60 * 60 * 3);
						chaPremiumitem.SetMightOfAwellWeight(300);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin28))		// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 1ÀÏ
					{
						//Mudança tempo do item premium
						//chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_EXPUP );
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_EXPUP);
						chaPremiumitem.SetManaReducePotionValue(30); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin29))		// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 7ÀÏ
					{
						//chaPremiumitem.SetManaReducePotionTime( PRIME_ITEM_TIME_EXPUPx7 );
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_3DAY);
						chaPremiumitem.SetManaReducePotionValue(30); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin30))	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£2 7ÀÏ
					{
						//chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_EXPUPx7 );
						chaPremiumitem.SetMightOfAwellTime(PRIME_ITEM_TIME_EXPUP);
						chaPremiumitem.SetMightOfAwellWeight(500);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin31))	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£2 30ÀÏ
					{
						//chaPremiumitem.SetMightOfAwellTime( PRIME_ITEM_TIME_PACKAGE4 );
						chaPremiumitem.SetMightOfAwellTime(PRIME_ITEM_TIME_3DAY);
						chaPremiumitem.SetMightOfAwellWeight(500);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin32))	// pluto Æê(ÇØ¿Ü)
					{
						//chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_EXPUP );
						chaPremiumitem.SetPhenixPetTime(PRIME_ITEM_TIME_EXPUP);
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin33))	// pluto Æê(ÇØ¿Ü)
					{
						chaPremiumitem.SetPhenixPetTime(PRIME_ITEM_TIME_3DAY);
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin34))	// pluto Æê(ÇØ¿Ü)
					{
						//chaPremiumitem.SetPhenixPetTime( PRIME_ITEM_TIME_VAMPCUSPID );
						chaPremiumitem.SetPhenixPetTime(60 * 60 * 1);
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin43)) {		// ¹ÚÀç¿ø - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 1ÀÏ Ãß°¡)
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_EXPUP);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin44)) {		// ¹ÚÀç¿ø - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 7ÀÏ Ãß°¡)
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_EXPUPx7);
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin50)) {		// ¹ÚÀç¿ø - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 30ÀÏ Ãß°¡)
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE4); // 30ÀÏ
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin51))	// ¹ÚÀç¿ø - ÇÈ´Ð½º Æê(30ÀÏ) Ãß°¡
					{
						chaPremiumitem.SetPhenixPetTime(PRIME_ITEM_TIME_PACKAGE4);
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin52))	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE4); // ¿ì¼± Å×½ºÆ®·Î 1ºÐÀ¸·Î ¼ÂÆÃ(60) 

						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin53))	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE4);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin54))	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE4);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin55))	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE4);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}


					else if (TransThrowItem.Item.CODE == (sinBI1 | sin69))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE2);

						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin70))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE2);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin71))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE2);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin72))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE2);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}

					// Àåº° - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 1½Ã°£)
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin80)) {
						chaPremiumitem.SetExpUpPotionTime(60 * 60 * 24); // 1½Ã°£
					}


					// Àåº° - ÇÇ´Ð½ºÆê(1½Ã°£)
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin77))	// pluto Æê(ÇØ¿Ü)
					{
						chaPremiumitem.SetPhenixPetTime(PRIME_ITEM_TIME_PACKAGE5);
						cPCBANGPet.PetKind = TRUE;
						cPCBANGPet.ShowPet();
					}

					else if (TransThrowItem.Item.CODE == (sinBI1 | sin65))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸®) 7ÀÏ
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE3);

						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin66))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º)
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE3);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin67))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À)
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE3);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin68))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ®)
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE3);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}

					else if (TransThrowItem.Item.CODE == (sinBI1 | sin73))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸® 1½Ã°£) 
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE5);

						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Terry + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin74))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º 1½Ã°£)
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE5);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Nepsys + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin75))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À 1½Ã°£)
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE5);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Io + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin76))	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ® 1½Ã°£)
					{
						chaPremiumitem.SetHelpPetTimeTime(PRIME_ITEM_TIME_PACKAGE5);
						// 1:Terry  2:Nepsys  3:Io  4:Mute
						cHelpPet.PetKind = HoLowLevelPet::Type::Mute + 1;
						cHelpPet.PetShow = 1;
						cHelpPet.ShowPet();
					}

					// Àåº° - ½áµå ¾ÆÀÌÁî(1½Ã°£)
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin78)) {
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_PACKAGE5);
					}

					// Àåº° - °æÇèÄ¡Áõ°¡Æ÷¼Ç(1½Ã°£)
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin79))
					{
						chaPremiumitem.SetExpUpPotionTime(60 * 60 * 3);
					}

					else if (TransThrowItem.Item.CODE == (sinBI1 | sin80))
					{
						chaPremiumitem.SetExpUpPotionTime(60 * 60 * 24);
					}

					// Àåº° - ¸¶³ª ¸®µà½º(1½Ã°£)
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin83))
					{
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE5);
						chaPremiumitem.SetManaReducePotionValue(30); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
					}

					else if (TransThrowItem.Item.CODE == (sinBI1 | sin91))		 // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç(1ÀÏ)
					{
						//chaPremiumitem.SetStaminaReducePotionTime( PRIME_ITEM_TIME_EXPUP ); 
						chaPremiumitem.SetStaminaReducePotionTime(60 * 60 * 1);
						chaPremiumitem.SetStaminaReducePotionValue(10);			// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin92))		 // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç(7ÀÏ)
					{
						//chaPremiumitem.SetStaminaReducePotionTime( PRIME_ITEM_TIME_EXPUPx7 ); // PRIME_ITEM_TIME_EXPUPx7
						chaPremiumitem.SetStaminaReducePotionTime(PRIME_ITEM_TIME_3DAY);
						chaPremiumitem.SetStaminaReducePotionValue(10);			// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
					}
					else if (TransThrowItem.Item.CODE == (sinBI1 | sin93))		 // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç(7ÀÏ)
					{
						//chaPremiumitem.SetStaminaReducePotionTime( PRIME_ITEM_TIME_EXPUPx7 ); // PRIME_ITEM_TIME_EXPUPx7
						chaPremiumitem.SetStaminaReducePotionTime(PRIME_ITEM_TIME_EXPUP);
						chaPremiumitem.SetStaminaReducePotionValue(10);			// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
					}


					if (TransThrowItem.Item.CODE == ((sinBI2 | sin01))) {
						int test = 1;
					}

					switch (TransThrowItem.Item.CODE)
					{
					case (sinBI2 | sin01):
					case (sinBI2 | sin05):
					case (sinBI2 | sin09):
					case (sinBI2 | sin23):	// pluto ½´Æä¸®¾î ÆÐÅ°Áö 3½Ã°£
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE1);
						break;
					case (sinBI2 | sin02):
					case (sinBI2 | sin06):
					case (sinBI2 | sin10):
					case (sinBI2 | sin24):	// pluto ½´Æä¸®¾î ÆÐÅ°Áö 1ÀÏ
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE2);
						break;
					case (sinBI2 | sin03):
					case (sinBI2 | sin07):
					case (sinBI2 | sin11):
					case (sinBI2 | sin25):	// pluto ½´Æä¸®¾î ÆÐÅ°Áö 7ÀÏ
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE3);
						break;
					case (sinBI2 | sin04):
					case (sinBI2 | sin08):
					case (sinBI2 | sin12):
						chaPremiumitem.SetThirdEyesTime(PRIME_ITEM_TIME_PACKAGE4);
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE4);
						break;
					case (sinBI2 | sin26):			// pluto ºê·ÐÁî ÆÐÅ°Áö2 3½Ã°£
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionValue(10); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 10% °¨¼Ò
						break;
					case (sinBI2 | sin27):			// pluto ºê·ÐÁî ÆÐÅ°Áö2 1ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionValue(10); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 10% °¨¼Ò
						break;
					case (sinBI2 | sin28):			// pluto ºê·ÐÁî ÆÐÅ°Áö2 7ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionValue(10); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 10% °¨¼Ò
						break;
					case (sinBI2 | sin29):			// pluto ½Ç¹ö ÆÐÅ°Áö2 3½Ã°£
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionValue(20); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 20% °¨¼Ò
						break;
					case (sinBI2 | sin30):			// pluto ½Ç¹ö ÆÐÅ°Áö2 1ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionValue(20); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 20% °¨¼Ò
						break;
					case (sinBI2 | sin31):			// pluto ½Ç¹ö ÆÐÅ°Áö2 7ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionValue(20); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 20% °¨¼Ò
						break;
					case (sinBI2 | sin32):			// pluto °ñµå ÆÐÅ°Áö2 3½Ã°£
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionValue(30); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
						break;
					case (sinBI2 | sin33):			// pluto °ñµå ÆÐÅ°Áö2 1ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionValue(30); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
						break;
					case (sinBI2 | sin34):			// pluto °ñµå ÆÐÅ°Áö2 7ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionValue(30); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 30% °¨¼Ò
						break;
					case (sinBI2 | sin35):			// pluto ½´Æä¸®¾î ÆÐÅ°Áö2 3½Ã°£
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE1);
						chaPremiumitem.SetManaReducePotionValue(40); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 40% °¨¼Ò
						break;
					case (sinBI2 | sin36):			// pluto ½´Æä¸®¾î ÆÐÅ°Áö2 1ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE2);
						chaPremiumitem.SetManaReducePotionValue(40); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 40% °¨¼Ò
						break;
					case (sinBI2 | sin37):			// pluto ½´Æä¸®¾î ÆÐÅ°Áö2 7ÀÏ
						chaPremiumitem.SetExpUpPotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionTime(PRIME_ITEM_TIME_PACKAGE3);
						chaPremiumitem.SetManaReducePotionValue(40); // pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 40% °¨¼Ò
						break;
					}
				}

			}
			else {
				smWsockServer->Send2((char*)&TransThrowItem, TransThrowItem.size, TRUE);
			}
		}

	}

	TransThrowItem.code = 0;
	return TRUE;
}


//¸ó½ºÅÍ ½ÃÀÛ ÁöÁ¡ Ãß°¡
int SendAddStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADD_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	smWINSOCK* lpsmSock;
	lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
	if (lpsmSock) {
		return lpsmSock->Send((char*)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}

//¸ó½ºÅÍ ½ÃÀÛÁöÁ¡ Á¦°Å
int SendDeleteStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;
	scITEM* lpItem;
	smWINSOCK* lpsmSock;


	TransCommand.code = smTRANSCODE_DEL_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	//¶³¾îÁø ¾ÆÀÌÅÛ Ã£±â
	lpItem = FindScItem(x, z);
	if (lpItem) {
		lpsmSock = GetServerSock(lpItem->ServerCode);		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		lpItem->Close();
	}

	if (lpsmSock) {
		return lpsmSock->Send((char*)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}


int SendAdd_Npc(smCHAR * lpCharSample, int state)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;

	smTransPlayerInfo.code = smTRANSCODE_ADD_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy(&smTransPlayerInfo.smCharInfo, &lpCharSample->smCharInfo, sizeof(smCHAR_INFO));
	smTransPlayerInfo.smCharInfo.State = 0;

	lstrcpy(smTransPlayerInfo.smCharInfo.szModelName2, NpcSelectedName);

	smTransPlayerInfo.smCharInfo.Life[0] = 100;
	smTransPlayerInfo.smCharInfo.Mana[0] = 100;
	smTransPlayerInfo.smCharInfo.Life[1] = 100;

	smTransPlayerInfo.x = lpCharSample->pX;
	smTransPlayerInfo.y = lpCharSample->pY;
	smTransPlayerInfo.z = lpCharSample->pZ;

	smTransPlayerInfo.ax = lpCharSample->Angle.x;
	smTransPlayerInfo.ay = lpCharSample->Angle.y;
	smTransPlayerInfo.az = lpCharSample->Angle.z;

	smTransPlayerInfo.state = state;

	smWINSOCK* lpsmSock;
	lpsmSock = GetAreaServerSock();
	if (lpsmSock) {
		return lpsmSock->Send((char*)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}

	return NULL;
}


int SendDelete_Npc(smCHAR * lpCharSample)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;
	smWINSOCK* lpsmSock;

	smTransPlayerInfo.code = smTRANSCODE_DEL_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy(&smTransPlayerInfo.smCharInfo, &lpCharSample->smCharInfo, sizeof(smCHAR_INFO));

	smTransPlayerInfo.x = lpCharSample->pX;
	smTransPlayerInfo.y = lpCharSample->pY;
	smTransPlayerInfo.z = lpCharSample->pZ;

	smTransPlayerInfo.ax = lpCharSample->Angle.x;
	smTransPlayerInfo.ay = lpCharSample->Angle.y;
	smTransPlayerInfo.az = lpCharSample->Angle.z;

	smTransPlayerInfo.dwObjectSerial = lpCharSample->dwObjectSerial;

	lpsmSock = GetServerSock(lpCharSample->ServerCode);		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
	lpCharSample->Close();

	if (lpsmSock) {
		return lpsmSock->Send((char*)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}
	return NULL;
}

//ÀúÀåµÈ Ä³¸¯ÅÍ µ¥ÀÌÅ¸ ºÒ·¯¿À±â ¿ä±¸
int SendGetRecordData(char* szName)
{

	smTRANS_CHAR_COMMAND	smTransCharCommand;

	if (smWsockDataServer)
	{
		smTransCharCommand.code = smTRANSCODE_GETRECORDDATA;
		smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);

		smTransCharCommand.WParam = dwExeCheckSum;					//½ÇÇàÆÄÀÏ Ã¼Å©¼¶
		smTransCharCommand.LParam = FALSE;
		smTransCharCommand.SParam = Client_Version;

		lstrcpy(smTransCharCommand.szName, szName);

		dwLoadingTime = GetCurrentTime();			//·Îµù ½Ãµµ ½Ã°£ ±â·Ï

		return smWsockDataServer->Send((char*)&smTransCharCommand, smTransCharCommand.size, TRUE);
	}

	return FALSE;
}

char* szCmdOpenMonster[10] = {
	"È©°íºí¸°",
	"È©°íºí¸°",
	"È©°íºí¸°",
	"Å·È£ÇÇ",
	"Å¸ÀÌÅº",
	0,0,0,0,0
};


//¸ó½ºÅÍ »ý¼º
int SendOpenMonster(int State)
{
	smTRANS_CHAR_COMMAND	smTransCharCommand;
	char* szMonName;

	smTransCharCommand.code = smTRANSCODE_OPENMONSTER;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	smTransCharCommand.WParam = State;
	smTransCharCommand.LParam = 0;
	smTransCharCommand.SParam = 0;

	if (State >= 0) {
		szMonName = smConfig.szCmdOpenMonster[State];
		if (!szMonName[0]) szMonName = szCmdOpenMonster[State];
	}

	if (State >= 0 && szMonName)
		lstrcpy(smTransCharCommand.szName, szMonName);
	else
		smTransCharCommand.szName[0];


	smWINSOCK* lpsmSock;
	lpsmSock = GetAreaServerSock();

	if (lpsmSock) {
		return lpsmSock->Send((char*)&smTransCharCommand, smTransCharCommand.size, TRUE);
	}

	return FALSE;

}

int	CodeXorCharInfo()
{
	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	return TRUE;
}

int SaveCheckSkill()
{

	ReformSkillInfo();

	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point) {
			if (CSKILL->CheckSkillPointForm(&sinSkill.UseSkill[j]) == FALSE)
				return FALSE;
			if (CSKILL->CheckSkillMasteryForm(&sinSkill.UseSkill[j]) == FALSE)
				return FALSE;
		}
	}

	return TRUE;
}

int ReformSkillInfo()
{
	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		CSKILL->ReformSkillMasteryForm(&sinSkill.UseSkill[j]);
		CSKILL->ReformSkillPointForm(&sinSkill.UseSkill[j]);
	}
	return TRUE;
}

int	SaveGameData()
{
	if (smWsockDataServer && !rsRecorder.dwRecordTime &&
		lpCurPlayer->smCharInfo.JOB_CODE && !AdminCharDisable && !cTrade.OpenFlag && !cWareHouse.OpenFlag && !QuitSave && !Caravana::GetInstance()->OpenFlag)
	{
		if (!WarningHack && CheckCharForm() != FALSE && SaveCheckSkill() == TRUE)
		{
			CodeXorCharInfo();
			ReformCharForm();

			rsRecorder.MakeRecordData(&lpCurPlayer->smCharInfo, cInvenTory.InvenItem, cInvenTory.InvenItemTemp, &MouseItem);
			rsRecorder.SendRecordDataToServer(smWsockDataServer);
			rsRecorder.dwRecordTime = dwPlayTime;

			CodeXorCharInfo();
			ReformCharForm();
			CheckPlaySkill();
			return TRUE;
		}
	}
	return FALSE;
}

int GetSaveResult()
{
	if (rsRecorder.dwRecordTime) return FALSE;

	return TRUE;
}

char* GetMacAddress()
{
	char szaMacAddress[64] = { 0 };

	UUID uuid;
	UuidCreateSequential(&uuid);

	wsprintfA(szaMacAddress, "%02X-%02X-%02X-%02X-%02X-%02X", uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);

	return szaMacAddress;
}

smTRANS_COMMAND	smMessageCommandBuff;

int getVolumeNumber(char* volumeNumber, int capacity, char diskN[12])
{
	for (int i = 0; i < capacity; i++) {
		volumeNumber[i] = '\0';
	}
	TCHAR volumeName[MAX_PATH + 1] = { 0 };
	TCHAR fileSystemName[MAX_PATH + 1] = { 0 };
	DWORD serialNumber = 0;
	DWORD maxComponentLen = 0;
	DWORD fileSystemFlags = 0;

	char szdiskN[10] = { 0 };
	sprintf_s(szdiskN, sizeof(szdiskN), "%s:\\", diskN);

	if (GetVolumeInformation(_T(szdiskN), volumeName, ARRAYSIZE(volumeName), &serialNumber, &maxComponentLen, &fileSystemFlags, fileSystemName, ARRAYSIZE(fileSystemName)) != 0)
	{
		sprintf(volumeNumber, "%lu", serialNumber);
		return 0;
	}
	else
	{
		return 1;
	}
}

int	TransUserCommand(DWORD dwCode, char* szID, char* szName)
{
	smTRANS_CHAR_COMMAND2	TransCharCommand2;

	TransCharCommand2.code = dwCode;
	TransCharCommand2.size = sizeof(smTRANS_CHAR_COMMAND2);
	TransCharCommand2.WParam = Client_Version;
	TransCharCommand2.LParam = _PACKET_PASS_XOR;

	if (szID)
		strcpy_s(TransCharCommand2.szId, szID);

	if (szName)
		strcpy_s(TransCharCommand2.szName, szName);

	TransCharCommand2.szMac[23] = { 0 };

	// Mac Address
	{
		UUID uuid;

		// Ask OS to create UUID
		UuidCreateSequential(&uuid);

		// Bytes 2 through 7 inclusive 
		sprintf_s(TransCharCommand2.szMac, 20, "%02X-%02X-%02X-%02X-%02X-%02X",
			uuid.Data4[2], uuid.Data4[3], uuid.Data4[4], uuid.Data4[5], uuid.Data4[6], uuid.Data4[7]);
	}

	//Directory
	{
		GetModuleFileName(NULL, TransCharCommand2.szPath, MAX_PATH);
	}

	{
		char disk[12] = { 0 };
		sprintf_s(disk, sizeof(disk), "%c", TransCharCommand2.szPath[0]);
		getVolumeNumber(TransCharCommand2.szHDMac, 32, disk);
	}

	//PCName
	{
		DWORD size = sizeof(TransCharCommand2.szPCName) / sizeof(TransCharCommand2.szPCName[0]);
		GetComputerName(TransCharCommand2.szPCName, &size);
	}

	return smWsockDataServer->Send2((char*)&TransCharCommand2, TransCharCommand2.size, TRUE);
}

int	Send_GetCharInfo(DWORD	dwObjectSerial, int ServerCode)
{
	smTRANS_COMMAND			TransCommand;
	smWINSOCK* lpsmSock;

	TransCommand.code = smTRANSCODE_GETPLAYERINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;

	lpsmSock = GetServerSock(ServerCode);

	if (lpsmSock) {
		return lpsmSock->Send((char*)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}

#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)

static DWORD dwCheckNetCount = 0;
static DWORD dwLastTransCheckTime = 0;
static int	ReconnectCount = 0;

int	TransCheckNetwork()
{
	smTRANS_COMMAND			TransCommand;
	DWORD	dwTime, dwTime2;
	smWINSOCK* lpOldSock;

	if (!dwConnectedServerTime)
		return FALSE;

	OpenTimeCheckThread();

	dwTime = GetCurrentTime();
	dwTime2 = dwTime;

	if ((dwTime - dwLastTransCheckTime) < 4000)
		return FALSE;

	dwLastTransCheckTime = dwTime;

	if ((dwMainServ_RecvTime && (dwMainServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime) ||
		(dwLastRecvGameServerTime + DEADLOCK_CHECK_LIMIT_TIME) < dwTime ||
		!smWsockServer || smWsockServer->WaitReadMessage < 0)
	{
		dwLastRecvGameServerTime = dwTime;

		lpOldSock = smWsockServer;

		if (lpOldSock == smWsockDataServer)
			smWsockDataServer = 0;
		if (lpOldSock == smWsockUserServer)
			smWsockUserServer = 0;
		if (lpOldSock == smWsockExtendServer)
			smWsockExtendServer = 0;

		smWsockServer = 0;
		ConnectServer_Main();

		if (smWsockServer)
		{
			ReconnServer++;

			if (ReconnServer > 10)
			{
				QuitGame();
			}

			SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockServer);

			if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
			if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
			if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
			if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

			CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "> Reconectando ao servidor: %d/10", ReconnServer);

			return TRUE;
		}

		DisconnectServerCode = 1;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 20");
		DisconnectFlag = GetCurrentTime();
		return FALSE;
	}


	dwTime = (dwTime - dwConnectedClientTime) + dwConnectedServerTime;

	TransCommand.code = smTRANSCODE_CHECK_NETSTATE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwTime;
	TransCommand.LParam = dwCheckNetCount;
	TransCommand.SParam = GetSafeCode2() + ((dwMemError ^ dwPlayTime) - 8080);
	TransCommand.EParam = dwMemFunChkCode;

	dwCheckNetCount++;

	if (smWsockServer) smWsockServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockDataServer && smWsockServer != smWsockDataServer)
	{
		if (dwLastRecvGameServerTime2 && (dwLastRecvGameServerTime2 + DEADLOCK_CHECK_LIMIT_TIME2) < dwTime2)
		{
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 21");
			DisconnectFlag = GetCurrentTime();
			DisconnectServerCode = 2;
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE2;
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime2) dwLastRecvGameServerTime2 = dwTime2;
	}


	if (smWsockUserServer && smWsockServer != smWsockUserServer)
	{
		if ((dwLastRecvGameServerTime3 && (dwLastRecvGameServerTime3 + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2) ||
			(dwUserServ_RecvTime && (dwUserServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime2) ||
			smWsockUserServer->WaitReadMessage < 0) {

			smWsockUserServer = 0;
			ConnectServer_Main();

			if (smWsockUserServer) {
				SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockUserServer);

				CHATGAMEHANDLE->AddChatBoxText("> Reconnect Server (User)", EChatColor::CHATCOLOR_Error);


				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				ReconnDataServer++;

				return TRUE;
			}

			DisconnectServerCode = 3;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 22");
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE3;
		smWsockUserServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime3) dwLastRecvGameServerTime3 = dwTime2;

	}

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer) {
		if ((dwLastRecvGameServerTime4 && (dwLastRecvGameServerTime4 + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2) ||
			(dwExtendServ_RecvTime && (dwExtendServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime2) ||
			smWsockExtendServer->WaitReadMessage < 0) {

			smWsockExtendServer = 0;
			ConnectServer_Main();

			if (smWsockExtendServer) {
				SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockExtendServer);


				CHATGAMEHANDLE->AddChatBoxText("> Reconnect Server (Extend)", EChatColor::CHATCOLOR_Error);

				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				return TRUE;
			}

			DisconnectServerCode = 3;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 23");
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE4;
		smWsockExtendServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime4) dwLastRecvGameServerTime4 = dwTime2;

	}
	if (AreaServerMode) {
		int cnt;
		for (cnt = 0; cnt < 2; cnt++) {
			if (lpWSockServer_Area[cnt]) {
				if ((lpWSockServer_Area[cnt]->dwDeadLockTime + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2 ||
					lpWSockServer_Area[cnt]->WaitReadMessage < 0) {

					lpOldSock = lpWSockServer_Area[cnt];
					lpWSockServer_Area[cnt] = smConnectSock(lpOldSock->szIPAddr, TCP_SERVPORT);

					if (lpWSockServer_Area[cnt]) {
						SendContinueServer(lpCurPlayer->dwObjectSerial, lpWSockServer_Area[cnt]);

						CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "Reconnect Server Area (%d)", cnt);
					}
					else {
						Utils_Log(3, "netplay.cpp 7680");
						lpOldSock->CloseSocket();
					}
				}

				if (lpWSockServer_Area[cnt]) {
					TransCommand.code = smTRANSCODE_CHECK_NETSTATE5;
					TransCommand.EParam = (DWORD)lpWSockServer_Area[cnt];
					lpWSockServer_Area[cnt]->Send2((char*)&TransCommand, TransCommand.size, TRUE);
				}
			}
		}
	}

	SendTotalExpMoney();

	CheckMaxDamage();


	return TRUE;
}

int SendAdminMode(DWORD AdminNum)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	if (smWsockDataServer && smWsockServer != smWsockDataServer)
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer && smWsockDataServer != smWsockUserServer)
		smWsockUserServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer && smWsockDataServer != smWsockExtendServer)
		smWsockExtendServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);


	return smWsockServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
}

//¼­¹ö¿¡ °ü¸®ÀÚ ¸ðµå ¼³Á¤
int SendAdminMode(DWORD AdminNum, smWINSOCK * lpsmSock)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	return lpsmSock->Send3((char*)&TransCommand, TransCommand.size, TRUE);
}

//¼­¹ö¿¡ ³×Æ®¿öÅ© Ç®Áú ¸ðµå ¼³Á¤
int SendNetworkQuality(DWORD NetworkQuality)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_NETWORK_QUALITY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = NetworkQuality;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send((char*)&TransCommand, TransCommand.size, TRUE);

	return smWsockServer->Send((char*)&TransCommand, TransCommand.size, TRUE);
}

//IPÁ¢¼Ó ÁÖ¼Ò¸¦ ±º ¼­¹ö¿¡ º¸³½´Ù
int SendCheckIP(int Code, smWINSOCK * lpsmSock)
{
	smTRANS_CHAR_COMMAND	TransCharCommand;

	TransCharCommand.code = smTRANSCODE_CHECKIP;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = Code;
	TransCharCommand.LParam = 0;
	TransCharCommand.SParam = 0;

	lstrcpy(TransCharCommand.szName, lpsmSock->szIPAddr);

	return smWsockDataServer->Send2((char*)&TransCharCommand, TransCharCommand.size, TRUE);
}

//¼­¹ö¿¡ ¿ÀºêÁ§Æ® ¹øÈ£ ¼³Á¤
int SendSetObjectSerial(DWORD dwObjectSerial)
{
	//»õ¹öÀü 
	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy(&TransPlayerInfo.smCharInfo, &lpCurPlayer->smCharInfo, sizeof(smCHAR_INFO));
	TransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	smWsockServer->Send2((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockUserServer)
		smWsockUserServer->Send2((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send2((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (AreaServerMode) {
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send2((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send2((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
	}

	return TRUE;
}

//¼­¹ö¿¡ ¿ÀºêÁ§Æ® ¹øÈ£ ¼³Á¤ ( Ã³À½ ÇÑ¹ø¸¸ )
int SendSetObjectSerial_First(DWORD dwObjectSerial)
{

	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy(&TransPlayerInfo.smCharInfo, &lpCurPlayer->smCharInfo, sizeof(smCHAR_INFO));
	TransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO2;
	TransPlayerInfo.dwObjectSerial = lpCurPlayer->dwObjectSerial;

	TransPlayerInfo.x = lpCurPlayer->pX;
	TransPlayerInfo.y = lpCurPlayer->pY;
	TransPlayerInfo.z = lpCurPlayer->pZ;

	//smWsockServer->Send2( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if (smWsockServer != smWsockUserServer)
		smWsockUserServer->Send3((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send3((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	return TRUE;
}


int SendContinueServer(DWORD dwObjectSerial, smWINSOCK * lpsmSock)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_CONT_OBJSERIAL;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return lpsmSock->Send((char*)&TransCommand, TransCommand.size, TRUE);
}

//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
int SendSetHackUser3(int Type, int LParam, int SParam)
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser3: Type(%d)  LParam(%d)  SParam(%d)", Type, LParam, SParam);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST2;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Type;
	TransCommand.LParam = LParam;
	TransCommand.SParam = SParam;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}

//Å©·¢ Ã½Å©
int CheckCracker()
{
	char* szName;
	char TransBuff[1024];
	int len;

	szName = FindCracker();

	if (szName)
	{

		if (smWsockDataServer)
		{
			len = lstrlen(szName) + 9;
			strcpy_s(TransBuff + 8, sizeof(TransBuff) - 8, szName);
			((int*)TransBuff)[0] = len;
			((int*)TransBuff)[1] = smTRANSCODE_FINDCRACK;

			smWsockDataServer->Send(TransBuff, len, TRUE);
		}
		return TRUE;
	}
	return FALSE;
}

//Å©·¢ Ã½Å© ( ¸Þ¼¼Áö Åëº¸µµ °°ÀÌ »ç¿ë )
int CheckCracker(TRANS_FIND_CRACKER * lpTransFindCrack)
{
	char* szName;
	char TransBuff[1024];
	int len;
	char* lpBuff;

	if (lpTransFindCrack)
		szName = FindCracker(lpTransFindCrack);
	else
		szName = FindCrackCont();

	if (szName)
	{

		if (smWsockDataServer)
		{
			len = lstrlen(szName) + 9;
			strcpy_s(TransBuff + 8, sizeof(TransBuff) - 8, szName);
			((int*)TransBuff)[0] = len;
			((int*)TransBuff)[1] = smTRANSCODE_FINDCRACK;

			smWsockDataServer->Send(TransBuff, len, TRUE);
		}
		return TRUE;
	}

	if (lpTransFindCrack)
	{
		if (lpTransFindCrack->size > (lpTransFindCrack->FindCounter * ((int)sizeof(_CRACK_NAME)) + 16))
		{
			lpBuff = (char*)&lpTransFindCrack->CrackName[lpTransFindCrack->FindCounter];

			if (lpBuff[0])
			{

				GetNoticeUrl(lpBuff);
			}
		}
	}

	return FALSE;
}


//Å©·¢ À©µµ¿ì ½Å°í
int SendCrackWindow(HWND hWnd)
{
	Utils_Log(LOG_DEBUG, "SendCrackWindow");

	char* szName;
	char TransBuff[1024];
	int len;

	//Å©·¢ ¹ß°ß
	if (smWsockServer) {

		szName = TransBuff + 8;
		GetWindowText(hWnd, szName, 32);
		len = lstrlen(szName) + 9;
		((int*)TransBuff)[0] = len;
		((int*)TransBuff)[1] = smTRANSCODE_FINDCRACK2;

		smWsockServer->Send2(TransBuff, len, TRUE);
	}
	return TRUE;
}

//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
int SendSetHackUser(int StopGame)
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser: StopGame(%d)", StopGame);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = StopGame;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	WarningHack = StopGame;		//ÇØÅ· °æ°í

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (WarningHack)
	{
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 25");
		DisconnectFlag = GetCurrentTime();
	}

	return TRUE;
}

//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
int SendSetHackUser2(int Type, int LParam)
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser2: Type(%d)  LParam(%d)", Type, LParam);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = Type;
	TransCommand.LParam = LParam;
	TransCommand.SParam = 0;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}

//¼­¹ö¿¡ °ü¸®ÀÚ ¸ðµå ¼³Á¤
int SendAdminMode2(DWORD AdminNum)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE2;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 1;
	TransCommand.SParam = 0;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	return TRUE;

}

//º¹»ç ¾ÆÀÌÅÛÀ» ¼ÒÁöÇÑ »ç¶÷ ½Å°í 
int SendCopyItemUser(int CopyItemCount)
{
	Utils_Log(LOG_DEBUG, "SendCopyItemUser: CopyItemCount(%d)", CopyItemCount);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_ITEMCOPYLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = CopyItemCount;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}

//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
int SendSaveThrowItem(sITEMINFO * lpItemInfo)
{
	smTRANS_COMMAND	TransCommand;

	//¹°¾àÀÎ °æ¿ì¸¸ ÀúÀåÇÏµµ·Ï ¼öÁ¤
	if ((lpItemInfo->CODE & sinITEM_MASK1) != (sinPM1 & sinITEM_MASK1))	return FALSE;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;

	if ((lpItemInfo->CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1))
		ComparePotion();

	return smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
}

int	SendRecordTradeItems(sTRADE * lpTrade)
{

	TRANS_RECORD_TRADE_ITEMS	TransRecordTradeItems;
	int cnt;

	ZeroMemory(&TransRecordTradeItems, sizeof(TRANS_RECORD_TRADE_ITEMS));

	TransRecordTradeItems.code = smTRANSCODE_RECORD_TRADE;
	TransRecordTradeItems.Money = lpTrade->Money - 193;
	lstrcpy(TransRecordTradeItems.szTradeUserName, cTrade.szTradeCharName);

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (lpTrade->TradeItem[cnt].Flag && TransRecordTradeItems.ThrowItemCount < TRADE_RECV_ITEMS_MAX) {
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwCode = lpTrade->TradeItem[cnt].sItemInfo.CODE;
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwHead = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransRecordTradeItems.Items[TransRecordTradeItems.ThrowItemCount].dwChkSum = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			TransRecordTradeItems.ThrowItemCount++;
		}
	}


	TransRecordTradeItems.size = TRANS_RECORD_TRADE_DEFSIZE + TransRecordTradeItems.ThrowItemCount * sizeof(_TRANS_RECORD_TRADE_ITEM);

	return smWsockDataServer->Send2((char*)&TransRecordTradeItems, TransRecordTradeItems.size, TRUE);
}

int SendSaveThrowItem2(sITEMINFO * lpItemInfo)
{
	smTRANS_COMMAND_EX	TransCommand;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM2;
	TransCommand.size = sizeof(smTRANS_COMMAND_EX);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;
	TransCommand.EParam = lpItemInfo->PotionCount ^ TransCommand.SParam ^ TransCommand.code;

	TransCommand.WxParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LxParam = EncodeParam(TransCommand.WxParam);
	TransCommand.SxParam = GetTotalMoney() ^ TransCommand.LxParam;
	TransCommand.ExParam = lpItemInfo->Money;

	if ((lpItemInfo->CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1))
		ComparePotion();

	AddInvenMoney(-lpItemInfo->Money);

	return smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
}

int SendSaveMoney()
{
	smTRANS_COMMAND	TransCommand;

	if ((cWareHouse.OpenFlag && CompWareHouseMoney) || (Caravana::GetInstance()->OpenFlag && CompCaravanMoney)) return FALSE;

	TransCommand.code = smTRANSCODE_SAVETHROWMONEY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LParam = EncodeParam(TransCommand.WParam);
	TransCommand.SParam = GetTotalMoney() ^ TransCommand.LParam;

	return smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
}

int SendCopyOtherUser(char* szName, int Mode)
{
	smTRANS_CHAR_COMMAND	TransCharCommand;
	int cnt, len;

	TransCharCommand.code = smTRANSCODE_GETRECORDDATA;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = dwExeCheckSum;
	TransCharCommand.LParam = Mode;
	TransCharCommand.SParam = 0;

	//20020912
	if (Mode == 2) {
		len = lstrlen(szName);
		for (cnt = 0; cnt < len; cnt++) {
			if (szName[cnt] == ' ') {
				TransCharCommand.LParam = atoi(&szName[cnt]);
				szName[cnt] = 0;
				if (TransCharCommand.LParam < 1000) TransCharCommand.LParam = Mode;
				break;
			}
		}
	}

	lstrcpy(TransCharCommand.szName, szName);

	AdminCharDisable = TRUE;

	if (smWsockDataServer)
		return smWsockDataServer->Send((char*)&TransCharCommand, TransCharCommand.size, TRUE);

	return NULL;
}


int SendPlayUpdateInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 0;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;
	TransCommand.SParam = 0;
	TransCommand.EParam = 0;

	return smWsockServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
}

// Sem clan durante a arena
int SendUpdateClanInfo()
{
	smTRANS_COMMAND	TransCommand;
	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);

	if (StageField[lpCurPlayer->OnStageField]->FieldCode == FIELD_ARENA && EventoArena::GetInstance()->stageArena > 0) {
		TransCommand.WParam = 0;
	}
	else {
		TransCommand.WParam = 1;
	}

	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;
	TransCommand.SParam = lpCurPlayer->smCharInfo.ClassClan;
	TransCommand.EParam = lpCurPlayer->smCharInfo.bUpdateInfo[0];


	if (smWsockServer)
		smWsockServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (smWsockDataServer && smWsockServer != smWsockDataServer && rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan))
		smWsockDataServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	if (AreaServerMode) {
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send2((char*)&TransCommand, TransCommand.size, TRUE);
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send2((char*)&TransCommand, TransCommand.size, TRUE);
	}

	return TRUE;
}

TRANS_SKIL_ATTACKDATA	Back_TransSkilAttackData;

int Skil_RangeAttack(int x, int y, int z, int range, int power, int TargetState)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR* lpChar;
	smWINSOCK* lpsmSendSock = 0;


	Record_ClinetLogFile("±¸¹öÀü °ø°ÝÆÐÅ¶ - Skil_RangeAttack ");
	quit = 1;
	return TRUE;


	mCnt = 0;

	dRange = range >> FLOATNS;
	dRange *= dRange;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY) {
			lpChar = &chrOtherPlayer[cnt];

			dx = (x - lpChar->pX) >> FLOATNS;
			dy = (y - lpChar->pY) >> FLOATNS;
			dz = (z - lpChar->pZ) >> FLOATNS;
			dist = dx * dx + dy * dy + dz * dz;

			if (dist < dRange) {
				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

				if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
					lpChar->EnableStateBar = TRUE;
				}
			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD) * SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD) * mCnt + 16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = range;
	TransSkilAttackData.Power = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);

	LastAttackDamage = power;

	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));

	lpsmSendSock = GetAreaServerSock();

	if (lpsmSendSock && mCnt > 0) {
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send2((char*)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	if (PkMode) {
		Skil_RangeAttack_User(x, y, z, range, power, TargetState);
	}

	return TRUE;
}


int Skil_RangeAttack_User(int x, int y, int z, int range, int power, int TargetState)
{
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR* lpChar;
	smWINSOCK* lpsmSendSock = 0;

	mCnt = 0;

	dRange = range >> FLOATNS;
	dRange *= dRange;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER) {
			lpChar = &chrOtherPlayer[cnt];

			dx = (x - lpChar->pX) >> FLOATNS;
			dy = (y - lpChar->pY) >> FLOATNS;
			dz = (z - lpChar->pZ) >> FLOATNS;
			dist = dx * dx + dy * dy + dz * dz;

			if (dist < dRange) {

				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKUSER;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD) * SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD) * mCnt + 16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = range;
	TransSkilAttackData.Power = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);


	if (mCnt > 0) {
		Record_SendDamage(smWsockUserServer, TransSkilAttackData.Power);
		smWsockUserServer->Send2((char*)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	return TRUE;
}

int Skil_RangeBoxAttack(smCHAR * lpCurChar, RECT * lpRect, int power, int TargetState, int UseAttackRating)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	int dx, dy, dz;
	int Attack, accuracy;
	smCHAR* lpChar;
	smWINSOCK* lpsmSendSock = 0;


	Record_ClinetLogFile("±¸¹öÀü °ø°ÝÆÐÅ¶ - Skil_RangeBoxAttack ");
	quit = 1;
	return TRUE;


	mCnt = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY) {
			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX - lpCurChar->pX) >> FLOATNS;
			dy = (lpChar->pY - lpCurChar->pY) >> FLOATNS;
			dz = (lpChar->pZ - lpCurChar->pZ) >> FLOATNS;

			GetMoveLocation(dx, dy, dz, 0, (-lpCurChar->Angle.y) & ANGCLIP, 0);

			if (GeoResult_X > lpRect->left && GeoResult_X<lpRect->right &&
				GeoResult_Z>lpRect->top && GeoResult_Z < lpRect->bottom && abs(dy) < 256) {

				Attack = TRUE;

				if (UseAttackRating) {
					accuracy = sinGetAccuracy(chrOtherPlayer[cnt].smCharInfo.Level, chrOtherPlayer[cnt].smCharInfo.Defence);
					if ((rand() % 100) > accuracy)
						Attack = FALSE;
				}

				if (Attack) {
					TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

					if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
						lpChar->EnableStateBar = TRUE;
					}
				}
			}

		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD) * SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD) * mCnt + 16;

	TransSkilAttackData.x = lpCurChar->pX;
	TransSkilAttackData.y = lpCurChar->pY;
	TransSkilAttackData.z = lpCurChar->pZ;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = 0;
	TransSkilAttackData.Power = power;

	LastAttackDamage = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// Á¶ÀÛ ¹æÁö ÄÚµå


	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));		//¹é¾÷

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â

	if (lpsmSendSock && mCnt > 0) {
		//º¸³½ °ø°Ý·Â ±â·Ï
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send2((char*)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}
	return TRUE;
}


int Skill_CharRangeEffect(DWORD dwSkillCode, smCHAR * lpCurChar, smCHAR * lpCharTarget, int point)
{
	int time, rs;

	switch (dwSkillCode) {
	case SKILL_PLAY_ROAR:
		time = Roar_Time[point - 1];

		rs = lpCharTarget->smCharInfo.Resistance[sITEMINFO_BIONIC];
		if (rs < 0) rs = 0;
		if (rs >= 100) rs = 100;
		rs = 100 - rs;
		time = (time * rs) / 100;

		if (time) {
			if (time > 2) time = 2;
			AssaParticle_Stune(lpCharTarget, time);
			return TRUE;
		}
		break;

	case SKILL_PLAY_SOUL_SUCKER:
		if (lpCharTarget->smCharInfo.Brood != smCHAR_MONSTER_UNDEAD &&
			((lpCharTarget->smCharInfo.Life[1] * 50) / 100) < lpCharTarget->smCharInfo.Life[0]) {

			sinSkillEffect_Soul_Sucker(lpCurChar, lpCharTarget);
			return TRUE;
		}
		break;

	}
	return FALSE;
}


//¹üÀ§Çü °ø°Ý ÀÌÆåÆ® ¿¬Ãâ
int Skil_RangeBoxEffect(smCHAR * lpCurChar, RECT * lpRect, DWORD dwSkillCode, int TargetState)
{

	int cnt, mCnt;
	int dx, dy, dz;
	smCHAR* lpChar;

	mCnt = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY &&
			lpCurChar != &chrOtherPlayer[cnt]) {

			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX - lpCurChar->pX) >> FLOATNS;
			dy = (lpChar->pY - lpCurChar->pY) >> FLOATNS;
			dz = (lpChar->pZ - lpCurChar->pZ) >> FLOATNS;

			// ÀÌµ¿µÈ Áö¿ª ÁÂÇ¥°ªÀ» ±¸ÇÏ´Â °ÍÀÌ±¸³ª! ³î¶ø´Ù.
			GetMoveLocation(dx, dy, dz, 0, (-lpCurChar->Angle.y) & ANGCLIP, 0);

			if (GeoResult_X > lpRect->left && GeoResult_X<lpRect->right &&
				GeoResult_Z>lpRect->top && GeoResult_Z < lpRect->bottom && abs(dy) < 256) {

				if (Skill_CharRangeEffect(dwSkillCode, lpCurChar, &chrOtherPlayer[cnt], 0) == TRUE) {
					mCnt++;
				}
			}

		}
	}

	return mCnt;
}

//¹üÀ§Çü °ø°Ý ÀÌÆåÆ® ¿¬Ãâ
int Skil_RangeEffect(smCHAR * lpCurChar, int Range, DWORD dwSkillCode, int TargetState, int point)
{

	int cnt, mCnt;
	int dx, dy, dz;
	smCHAR* lpChar;
	int	dRange;

	dRange = Range * Range;

	mCnt = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (mCnt >= SKIL_ATTACK_CHAR_MAX) break;
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY &&
			lpCurChar != &chrOtherPlayer[cnt]) {

			lpChar = &chrOtherPlayer[cnt];

			dx = (lpChar->pX - lpCurChar->pX) >> FLOATNS;
			dy = (lpChar->pY - lpCurChar->pY) >> FLOATNS;
			dz = (lpChar->pZ - lpCurChar->pZ) >> FLOATNS;

			if ((dx * dx + dy * dy + dz * dz) < dRange) {
				if (Skill_CharRangeEffect(dwSkillCode, lpCurChar, &chrOtherPlayer[cnt], point) == TRUE) {
					mCnt++;
				}
			}

		}
	}

	return mCnt;
}




//ÄÚµå ¾ÏÈ£ Ã½Å©¼¶ ¸¸µé±â
DWORD EncodeParam(DWORD Param)
{
	return (Param + (Param << 4)) * 2002;
}



//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ 
int QuickSendTransAttack(smCHAR * lpChar, int power, int AttackState, int Resistance)
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK* lpsmSendSock = 0;
	int pow;

	//Å¬¶óÀÌ¾ðÆ® ·Î±× ÆÄÀÏ ±â·Ï
	Record_ClinetLogFile("±¸¹öÀü °ø°ÝÆÐÅ¶ - QuickSendTransAttack ");
	quit = 1;
	return TRUE;


	if (RestartPlayCount != 0) return FALSE;

	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = lpChar->pX;
	TransAttackData.y = lpChar->pY;
	TransAttackData.z = lpChar->pZ;

	TransAttackData.AttackState = AttackState;
	TransAttackData.AttackSize = 48 * fONE;
	TransAttackData.Power = power;

	if (Resistance) {
		TransAttackData.AttackState |= (Resistance << 16);
		pow = Resistance >> 4;
		if (pow > 0 && pow < 512) {
			TransAttackData.Power += pow;			//¼Ó¼º °ø°Ý·Â Ãß°¡ ( ¼­¹ö¿¡¼­ Ãß°¡µÈ °ø°Ý·Â µû·Î °è»ê )
		}
	}

	TransAttackData.dwDestObjectSerial = lpCurPlayer->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;
	TransAttackData.dwChkSum = TransAttackData.Power * 2002 +
		(TransAttackData.x * TransAttackData.y * TransAttackData.z);// Á¶ÀÛ ¹æÁö ÄÚµå

	LastAttackDamage = TransAttackData.Power;

	//Ã³À½ °ø°ÝÇÏ´Â Ä³¸¯ÇÑÅ×¸¸ Ã¼·Â¹Ù Ãâ·Â
	if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
		lpChar->EnableStateBar = TRUE;
	}

	if (lpChar->ServerCode) {
		lpsmSendSock = GetServerSock(lpChar->ServerCode);		//¼­¹ö ÄÚµå ±¸ÇÏ±â
	}


	if (lpsmSendSock) {
		//º¸³½ °ø°Ý·Â ±â·Ï
		Record_SendDamage(lpsmSendSock, TransAttackData.Power);

		return lpsmSendSock->Send2((char*)&TransAttackData, TransAttackData.size, TRUE);
	}

	return NULL;
}

//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ 
int QuickSendTransAttack(smCHAR * lpChar, int power)
{
	return QuickSendTransAttack(lpChar, power, 1, 0);
}
//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ 
int QuickSendTransAttack(smCHAR * lpChar, int power, int Resistance)
{
	return QuickSendTransAttack(lpChar, power, 1, Resistance);
}

///////////////////////// Æ®·¹ÀÌµå °ü·Ã //////////////////////////////
/*
sTRADE sTrade;	   //º¸³¾ Á¤º¸
sTRADE sTradeRecv; //¹ÞÀº Á¤º¸
*/

//¾ÆÀÌÅÛ ±³È¯ ½ÅÃ»
int SendRequestTrade(int dwObjectSerial, int Flag)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_TRADE_REQUST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = Flag;
	TransCommand.SParam = lpCurPlayer->dwObjectSerial;

	if (smWsockUserServer)
		return smWsockUserServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);

	return FALSE;
}

//¾ÆÀÌÅÛ ±³È¯ Á¤º¸ Àü¼Û
int SendTradeItem(sTRADE * lpTrade, DWORD dwSender)
{
	int cnt;
	int len;
	int	PotionError = 0;
	TRANS_TRADEITEMS TransTradeItems;
	sTRADE	sTradeBuff;

	len = 0;

	//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ È®ÀÎ
	if (CheckTrade((void*)lpTrade) == FALSE) {
		SendRequestTrade(dwSender, 3);			//°Å·¡ Ãë¼Ò ¿ä±¸
		cTrade.CancelTradeItem();					//°Å·¡Ã¢ ´Ý±â
		return FALSE;
	}

	memcpy(&sTradeBuff, lpTrade, sizeof(sTRADE));

	//¹°¾à°¹¼ö ºñ±³
	PotionError = ComparePotion();

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (!sTradeBuff.TradeItem[cnt].Flag) {
			ZeroMemory(&sTradeBuff.TradeItem[cnt], sizeof(sITEM));
		}
		else {
			sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head = 0;		//Å°°ª Áö¿ò
			if (PotionError) {
				sTradeBuff.TradeItem[cnt].sItemInfo.PotionCount = sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head + 1;	//¹°¾à°¹¼ö ¿À·ù½Ã ¸ðµç Æ÷¼ÇÄ«¿îÆ®¸¦ 1·Î ¹Ù²Û´Ù
			}
			len++;
		}
	}

	//µ¥ÀÌÅ¸ ¾ÐÃà ( Z/NZ ¹æ½Ä )
	len = EecodeCompress((BYTE*)&sTradeBuff, TransTradeItems.TradeBuff, sizeof(sTRADE));
	if (len > TRANS_TRADE_BUFF_SIZE) return FALSE;

	TransTradeItems.code = smTRANSCODE_TRADEITEMS;
	TransTradeItems.size = len + 48;
	TransTradeItems.dwSum = 0;
	TransTradeItems.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems.dwRecver = dwSender;
	TransTradeItems.Temp[0] = 0;
	TransTradeItems.Temp[1] = 0;
	TransTradeItems.Temp[2] = 0;
	TransTradeItems.Temp[3] = 0;

	if (smWsockUserServer)
		return smWsockServer->Send2((char*)&TransTradeItems, TransTradeItems.size, TRUE);

	return FALSE;
}

//¾ÆÀÌÅÛ ±³È¯ Á¤º¸ ¼ö½Å 
int RecvTradeItems(TRANS_TRADEITEMS * lpTransTradeItems, sTRADE * lpTrade)
{
	//¾ÐÃà µ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE*)lpTrade);
	memcpy(&TransLastRecvTrade, lpTransTradeItems, lpTransTradeItems->size);		//¹ÞÀº Æ®·¹ÀÌµå µ¥ÀÌÅ¸ ÀÓ½Ã º¹»ç
	return TRUE;
}

//±³È¯Ã¢ ¾ÆÀÌÅÛ ¼Û½Å
int SendTradeItem(DWORD dwSender)
{
	return SendTradeItem(&sTrade, dwSender);
}
//±³È¯Ã¢ ¾ÆÀÌÅÛ ¼ö½Å
int RecvTradeItems(TRANS_TRADEITEMS * lpTransTradeItems)
{
	int res;

	res = RecvTradeItems(lpTransTradeItems, &sTradeRecv);
	cTrade.LoadTradeItemIamge();
	cTrade.CheckTradeButtonOk();

	ItemPickUpFlag = 1;

	//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ È®ÀÎ
	if (CheckTrade(&sTradeRecv) == FALSE) {
		SendRequestTrade(lpTransTradeItems->dwSender, 3);	//±³È¯ Ãë¼Ò
		cTrade.CancelTradeItem();
		return FALSE;
	}

	if (sTrade.CheckFlag && !sTradeRecv.CheckFlag) {
		sTrade.CheckFlag = 0;
		TradeItemCancelCount++;
		if (TradeItemCancelCount >= 3) {
			//3¹ø Ãë¼Ò ´çÇÔ
			SendRequestTrade(lpTransTradeItems->dwSender, 3);	//±³È¯ Ãë¼Ò
			cTrade.CancelTradeItem();
			return FALSE;
		}
		else {
			cMessageBox.ShowMessage(MESSAGE_MOVE_TRADE_ITEM);
			SendTradeItem(lpTransTradeItems->dwSender);
		}
	}

	return res;
}

//¾ÆÀÌÅÛ ±³È¯ ÀÎÁõÅ° Àü¼Û
int SendTradeSucessKey(DWORD dwSender)
{
	//¾ÆÀÌÅÛ ±³È¯ ÀÎÁõÅ° Àü¼Û
	return SendTradeSucessKey(&sTrade, dwSender);
}



//-------> ÀÌÇÔ¼ö°¡ °ÅÀÇ ÅëÃ¤·Î ¸¹ÀÌ ¹Ù²åÀ¸´Ï ±¸¹öÀü Àû¿ë½Ã È®ÀÎ //////////////

//¾ÆÀÌÅÛ ±³È¯ ÀÎÁõÅ° Àü¼Û
int SendTradeSucessKey(sTRADE * lpTrade, DWORD dwSender)
{

	TRANS_TRADE_ITEMKEY	TransTradeItemKey;
	int cnt;
	int	TradeOpenFlag, CancelFlag;

	if (DisconnectFlag)
		return FALSE;

	TransTradeItemKey.code = smTRANSCODE_TRADE_ITEMKEY;
	TransTradeItemKey.size = sizeof(TRANS_TRADE_ITEMKEY);
	TransTradeItemKey.dwSender = lpCurPlayer->dwObjectSerial;;
	TransTradeItemKey.dwRecver = dwSender;
	TransTradeItemKey.Money = lpTrade->Money;
	TransTradeItemKey.Coin = lpTrade->Coin;

	TradeOpenFlag = cTrade.OpenFlag;
	cTrade.OpenFlag = 0;
	CancelFlag = 0;

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (lpTrade->TradeItem[cnt].Flag) {
			TransTradeItemKey.dwItemCode[cnt] = lpTrade->TradeItem[cnt].sItemInfo.CODE;
			TransTradeItemKey.dwItemKey[cnt] = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
			TransTradeItemKey.dwItemSum[cnt] = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			if (FindInvenItem(lpTrade->TradeItem[cnt].sItemInfo.CODE,
				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head,
				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum) != 0) {
				//º¸³¾ ¾ÆÀÌÅÛÀÌ ÀÎº¥¿¡ ³²¾Æ ÀÖÀ½À» ¹ß°ß
				CancelFlag = TRUE;

				//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ º¹»ç ¿À·ù
				SendInvenItemError(1, lpTrade->TradeItem[cnt].sItemInfo.CODE,
					lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head,
					lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum);
			}

			if ((TransTradeItemKey.dwItemCode[cnt] & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
				//¹°¾àÀÌ´Ù! ( ¾ÆÀÌÅÛ Ã¼Å©¼¶¿¡ ¹°¾à°¹¼ö¸¦ ¼³Á¤ )
				TransTradeItemKey.dwItemSum[cnt] = lpTrade->TradeItem[cnt].sItemInfo.PotionCount & 0xFFFF;
				TransTradeItemKey.dwItemSum[cnt] |= (lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum) << 16;
				TransTradeItemKey.dwItemSum[cnt] ^= (TransTradeItemKey.dwItemSum[cnt] >> 16) ^ 0x5cbc;
			}

		}
		else {
			TransTradeItemKey.dwItemCode[cnt] = 0;
			TransTradeItemKey.dwItemKey[cnt] = 0;
			TransTradeItemKey.dwItemSum[cnt] = 0;
		}
	}

	cTrade.OpenFlag = TradeOpenFlag;

	if (!CancelFlag) {

		/*
				TRANS_THROW_ITEMS	TransThrowItems;

				TransThrowItems.ThrowItemCount = 0;

				for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
					if ( lpTrade->TradeItem[cnt].Flag ) {
						TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = lpTrade->TradeItem[cnt].sItemInfo.CODE;
						TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head;
						TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
						TransThrowItems.ThrowItemCount++;
						//SendSaveThrowItem( &lpTrade->TradeItem[cnt].sItemInfo );
					}
				}
				//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ÃÖ´ë 32°³ µ¿½Ã ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
				SendSaveThrowItems( &TransThrowItems );

				if ( smWsockServer )
					smWsockServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;

				if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
					smWsockUserServer->Send2( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;
		*/
		if (smWsockDataServer)
			smWsockDataServer->Send2((char*)&TransTradeItemKey, TransTradeItemKey.size, TRUE);


		//¾ÆÀÌÅÛ ±³È¯Å°
		memcpy(&LastTransTradeItemKey, &TransTradeItemKey, sizeof(TRANS_TRADE_ITEMKEY));
		dwLastTransTradeKeyTime = dwPlayTime;
	}

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}


//¾ÆÀÌÅÛ ±³È¯ ÀÎÁõÅ° ¼ö½Å
int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY * lpTransTradeItemKey, sTRADE * lpTrade)
{
	int cnt;
	sTRADE sChkTrade;
	DWORD	dwChkSum1, dwChkSum2;
	int	ItemFlag;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (TransLastRecvTrade.code) {
		//¸¶Áö¸· ¹ÞÀº µ¥ÀÌÅ¸·Î ¾ÐÃà µ¥ÀÌÅ¸ ´Ù½Ã ÇØµ¶ ( Z/NZ ¹æ½Ä )
		DecodeCompress(TransLastRecvTrade.TradeBuff, (BYTE*)&sChkTrade);
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			memcpy(&lpTrade->TradeItem[cnt].sItemInfo, &sChkTrade.TradeItem[cnt].sItemInfo, sizeof(sITEMINFO));
		}
	}
	//cTrade.LoadTradeItemIamge();

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (lpTrade->TradeItem[cnt].Flag) {

			dwChkSum1 = lpTransTradeItemKey->dwItemSum[cnt];
			dwChkSum2 = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			if ((lpTrade->TradeItem[cnt].sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
				//¹°¾àÀÎ°æ¿ì
				ItemFlag = 1;
				dwChkSum1 = dwChkSum1 >> 16;
				dwChkSum2 = dwChkSum2 & 0xFFFF;
			}
			else
				ItemFlag = 0;

			if (lpTransTradeItemKey->dwItemCode[cnt] == lpTrade->TradeItem[cnt].sItemInfo.CODE && dwChkSum1 == dwChkSum2) {
				//			lpTransTradeItemKey->dwItemSum[cnt]==lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head = lpTransTradeItemKey->dwItemKey[cnt];

				//Àß¸øµÈ ¾ÆÀÌÅÛ ¼ö½Å
				if (!lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head)
					lpTrade->TradeItem[cnt].Flag = 0;
				else {
					if (ItemFlag == 1) {
						//¹°¾à ÀçÀÎÁõ
						ReformItem(&lpTrade->TradeItem[cnt].sItemInfo);
					}
				}
			}
			else {
				return FALSE;
			}
		}
		else {
			if (lpTransTradeItemKey->dwItemCode[cnt])
				return FALSE;

		}
	}

	//SendRecordTradeItems( lpTrade );			//±³È¯ ³»¿ë ¼­¹ö¿¡ ±â·Ï

	return TRUE;
}



//¾ÆÀÌÅÛ ±³È¯ ³»¿ë È®ÀÎ
int SendTradeCheckItem(DWORD dwSender)
{

	TRANS_TRADE_CHECKITEM	TransTradeCheckItem;
	int cnt;

	if (DisconnectFlag)
		return FALSE;

	TransTradeCheckItem.code = smTRANSCODE_TRADE_READYITEM;
	TransTradeCheckItem.size = sizeof(TRANS_TRADE_CHECKITEM);
	TransTradeCheckItem.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeCheckItem.dwRecver = dwSender;

	if (sTrade.Money) {
		SendSaveMoney();			//µ· ÀúÀå ( º¹»ç¹æÁö )
	}

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (sTrade.TradeItem[cnt].Flag) {
			TransTradeCheckItem.dwSendItemCode[cnt] = sTrade.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwSendItemSum[cnt] = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			//SendSaveThrowItem( &sTrade.TradeItem[cnt].sItemInfo );		//¾ÆÀÌÅÛ º¹»ç ¹æÁö

			//¾ÆÀÌÅÛ ÀÎÁõ È®ÀÎ
			if (CheckItemForm(&sTrade.TradeItem[cnt].sItemInfo) == FALSE)
				return FALSE;		//Àß¸øµÈ ¾ÆÀÌÅÛ 

		}
		else {
			TransTradeCheckItem.dwSendItemCode[cnt] = 0;
			TransTradeCheckItem.dwSendItemSum[cnt] = 0;
		}
	}


	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (sTradeRecv.TradeItem[cnt].Flag) {
			TransTradeCheckItem.dwRecvItemCode[cnt] = sTradeRecv.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwRecvItemSum[cnt] = sTradeRecv.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
		}
		else {
			TransTradeCheckItem.dwRecvItemCode[cnt] = 0;
			TransTradeCheckItem.dwRecvItemSum[cnt] = 0;
		}
	}
	/*
		TRANS_THROW_ITEMS	TransThrowItems;
		TransThrowItems.ThrowItemCount = 0;
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( sTrade.TradeItem[cnt].Flag ) {
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = sTrade.TradeItem[cnt].sItemInfo.CODE;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.Head;
				TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
				TransThrowItems.ThrowItemCount++;
			}
		}
		//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ÃÖ´ë 32°³ µ¿½Ã ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
		SendSaveThrowItems( &TransThrowItems );
	*/

	if (smWsockServer)
		smWsockServer->Send2((char*)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send2((char*)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}

//¾ÆÀÌÅÛ ±³È¯ ³»¿ë È®ÀÎ ¼ö½Å
int RecvTradeCheckItem(TRANS_TRADE_CHECKITEM * lpTransTradeCheckItem)
{
	int cnt;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (DisconnectFlag)
		return FALSE;

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (sTrade.TradeItem[cnt].Flag) {
			if (lpTransTradeCheckItem->dwRecvItemCode[cnt] != sTrade.TradeItem[cnt].sItemInfo.CODE ||
				((lpTransTradeCheckItem->dwRecvItemCode[cnt] & sinITEM_MASK1) != (sinPM1 & sinITEM_MASK1) &&
					lpTransTradeCheckItem->dwRecvItemSum[cnt] != sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum)) {

				return FALSE;

			}
		}
		else {
			if (lpTransTradeCheckItem->dwRecvItemCode[cnt])
				return FALSE;
		}

		if (sTradeRecv.TradeItem[cnt].Flag) {
			if (lpTransTradeCheckItem->dwSendItemCode[cnt] != sTradeRecv.TradeItem[cnt].sItemInfo.CODE ||
				((lpTransTradeCheckItem->dwSendItemCode[cnt] & sinITEM_MASK1) != (sinPM1 & sinITEM_MASK1) &&
					lpTransTradeCheckItem->dwSendItemSum[cnt] != sTradeRecv.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum)) {

				return FALSE;

			}
		}
		else {
			if (lpTransTradeCheckItem->dwSendItemCode[cnt])
				return FALSE;
		}
	}

	return TRUE;
}







//ÆÄÆ¼ Çã¶ô 
int SendJoinPartyUser(DWORD dwObjectCode)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_PARTY_JOIN;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectCode;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return smWsockUserServer->Send((char*)&TransCommand, TransCommand.size, TRUE);
}

//ÆÄÆ¼ ÀÎÅÍÆäÀÌ½º ¹öÆ°ÀÔ·Â 
int	PartyButton(int Num, int Button)
{
	// Num ÆÄÆ¼¿ø ¹øÈ£
	// Button  0-ÀÌÀÓ 1-ÇØ»ê 2-Ãß¹æ 3-Å»Åð
	DWORD	dwCode;
	smTRANS_COMMAND	smTransCommand;

	if (!InterfaceParty.PartyMemberCount) return FALSE;

	dwCode = 0;

	if (Num >= 0)
		dwCode = InterfaceParty.PartyMember[Num].ChrCode;
	else
		dwCode = 0;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_PARTY_COMMAND;
	smTransCommand.WParam = dwCode;
	smTransCommand.LParam = Button;
	smTransCommand.SParam = 0;

	if (Button < 3 && InterfaceParty.PartyMember[0].ChrCode != lpCurPlayer->dwObjectSerial) {
		//ÆÄÆ¼Àå ±ÇÇÑ
		return FALSE;
	}

	return smWsockUserServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
}





////////////////////////// µ·°ú °æÇèÄ¡ ÀÌµ¿ ±â·Ï ///////////////

int	SetTotalSubMoney(int Money)
{
	TotalSubMoney += Money;

	return TRUE;
}

int	SetTotalAddMoney(int Money)
{
	TotalAddMoney += Money;

	return TRUE;
}

int	SetTotalAddExp(int Exp)
{
	TotalAddExp += Exp;

	return TRUE;
}

int	SetTotalSubExp(int Exp)
{
	TotalSubExp += Exp;

	return TRUE;
}

int	GetTotalMoney()
{
	int m;

	m = sServerMoney[0].InputMoney + sServerMoney[1].InputMoney + sServerMoney[2].InputMoney + WareHouseSubMoney + TotalAddMoney - TotalSubMoney;

	return m;
}

int GetTotalSubExp()
{
	int m;

	m = (sServerExp[0].InputExp + sServerExp[1].InputExp + sServerExp[2].InputExp) - (TotalAddExp - TotalSubExp);

	return m;
}

int GetTotalExp()
{
	int m;

	m = sServerExp[0].InputExp + sServerExp[1].InputExp + sServerExp[2].InputExp + TotalAddExp - TotalSubExp;

	return m;
}


int	SaveWareHouse(sWAREHOUSE * lpWareHouse, TRANS_WAREHOUSE * lpTransWareHouse)
{
	TRANS_WAREHOUSE	TransWareHouse;
	sWAREHOUSE	WareHouseCheck;
	int	CompSize;
	int cnt;
	DWORD	dwChkSum;
	char* szComp1, * szComp2;
	int	flag;

	if (lpTransWareHouse) flag = 1;
	else flag = 0;

	if (!flag && QuitSave)	return FALSE;

	for (cnt = 0; cnt < 100; cnt++) {
		if (!lpWareHouse->WareHouseItem[cnt].Flag) {
			ZeroMemory(&lpWareHouse->WareHouseItem[cnt], sizeof(sITEM));
		}
	}


	CompSize = EecodeCompress((BYTE*)lpWareHouse, (BYTE*)TransWareHouse.Data, sizeof(sWAREHOUSE), sizeof(sITEM) * 100);


	if (!flag && CompSize > (smSOCKBUFF_SIZE - 256))
		return FALSE;

	if (flag && CompSize > (smSOCKBUFF_SIZE - 140))
		return FALSE;


	DecodeCompress((BYTE*)TransWareHouse.Data, (BYTE*)&WareHouseCheck, sizeof(sWAREHOUSE));

	szComp1 = (char*)lpWareHouse;
	szComp2 = (char*)&WareHouseCheck;

	dwChkSum = 0;

	for (cnt = 0; cnt < sizeof(sWAREHOUSE); cnt++) {
		if (szComp1[cnt] != szComp2[cnt]) {
			return FALSE;
		}
		else {
			dwChkSum += szComp1[cnt] * (cnt + 1);
		}
	}

	WareHouseSubMoney += (CompWareHouseMoney - lpWareHouse->Money);
	CompWareHouseMoney = 0;

	TransWareHouse.code = smTRANSCODE_WAREHOUSE;
	TransWareHouse.size = sizeof(TRANS_WAREHOUSE) - (sizeof(sWAREHOUSE) - CompSize);
	TransWareHouse.DataSize = CompSize;
	TransWareHouse.dwChkSum = dwChkSum;
	TransWareHouse.wVersion[0] = Version_WareHouse;
	TransWareHouse.wVersion[1] = 0;

	if (!flag) {
		TransWareHouse.WareHouseMoney = lpWareHouse->Money ^ (dwChkSum ^ smTRANSCODE_WAREHOUSE);
		TransWareHouse.UserMoney = lpCurPlayer->smCharInfo.Money ^ (dwChkSum ^ smTRANSCODE_WAREHOUSE);
	}

	TransWareHouse.dwTemp[0] = 0;
	TransWareHouse.dwTemp[1] = 0;
	TransWareHouse.dwTemp[2] = 0;
	TransWareHouse.dwTemp[3] = 0;
	TransWareHouse.dwTemp[4] = 0;

	dwLastWareHouseChkSum = dwChkSum;

	if (flag) {
		memcpy(lpTransWareHouse, &TransWareHouse, sizeof(TRANS_WAREHOUSE));
		return TRUE;
	}

	if (TransWareHouse.size > smSOCKBUFF_SIZE) TransWareHouse.size = smSOCKBUFF_SIZE;

	if (smWsockDataServer) {
		smWsockDataServer->Send2((char*)&TransWareHouse, TransWareHouse.size, TRUE);

		SaveGameData();
		return TRUE;
	}

	return FALSE;
}

int	SaveCaravan(sCARAVAN * lpCaravan, TRANS_CARAVAN * lpTransCaravan)
{
	Caravana::TRANS_CARAVAN	TransCaravan;
	sCARAVAN	CaravanCheck;
	int	CompSize;
	int cnt;
	DWORD	dwChkSum;
	char* szComp1, * szComp2;
	int	flag;

	if (lpTransCaravan) flag = 1;
	else flag = 0;

	if (!flag && QuitSave)	return FALSE;

	for (cnt = 0; cnt < 100; cnt++) {
		if (!lpCaravan->CaravanItem[cnt].Flag) {
			ZeroMemory(&lpCaravan->CaravanItem[cnt], sizeof(sITEM));
		}
	}


	CompSize = EecodeCompress((BYTE*)lpCaravan, (BYTE*)TransCaravan.Data, sizeof(sCARAVAN), sizeof(sITEM) * 100);


	if (!flag && CompSize > (smSOCKBUFF_SIZE - 256))
		return FALSE;

	if (flag && CompSize > (smSOCKBUFF_SIZE - 140))
		return FALSE;


	DecodeCompress((BYTE*)TransCaravan.Data, (BYTE*)&CaravanCheck, sizeof(sCARAVAN));

	szComp1 = (char*)lpCaravan;
	szComp2 = (char*)&CaravanCheck;

	dwChkSum = 0;

	for (cnt = 0; cnt < sizeof(sCARAVAN); cnt++) {
		if (szComp1[cnt] != szComp2[cnt]) {
			return FALSE;
		}
		else {
			dwChkSum += szComp1[cnt] * (cnt + 1);
		}
	}

	CaravanSubMoney += (CompCaravanMoney - lpCaravan->Money);
	CompCaravanMoney = 0;

	TransCaravan.code = smTRANSCODE_CARAVAN;
	TransCaravan.size = sizeof(TRANS_CARAVAN) - (sizeof(sCARAVAN) - CompSize);
	TransCaravan.DataSize = CompSize;
	TransCaravan.dwChkSum = dwChkSum;
	TransCaravan.wVersion[0] = Version_WareHouse;
	TransCaravan.wVersion[1] = 0;

	if (!flag) {
		TransCaravan.WareHouseMoney = lpCaravan->Money ^ (dwChkSum ^ smTRANSCODE_CARAVAN);
		TransCaravan.UserMoney = lpCurPlayer->smCharInfo.Money ^ (dwChkSum ^ smTRANSCODE_CARAVAN);
	}

	TransCaravan.dwTemp[0] = 0;
	TransCaravan.dwTemp[1] = 0;
	TransCaravan.dwTemp[2] = 0;
	TransCaravan.dwTemp[3] = 0;
	TransCaravan.dwTemp[4] = 0;

	dwLastCaravanChkSum = dwChkSum;

	if (flag) {
		memcpy(lpTransCaravan, &TransCaravan, sizeof(TRANS_CARAVAN));
		return TRUE;
	}

	if (TransCaravan.size > smSOCKBUFF_SIZE) TransCaravan.size = smSOCKBUFF_SIZE;

	if (smWsockDataServer) {
		smWsockDataServer->Send2((char*)&TransCaravan, TransCaravan.size, TRUE);

		SaveGameData();
		return TRUE;
	}

	return FALSE;
}


int	SaveWareHouse(sWAREHOUSE * lpWareHouse)
{
	return SaveWareHouse(lpWareHouse, 0);
}

int	SaveCaravan(sCARAVAN * lpCaravan)
{
	return SaveCaravan(lpCaravan, 0);
}

int	LoadWareHouse(TRANS_WAREHOUSE * lpTransWareHouse, sWAREHOUSE * lpWareHouse, int flag)
{
	sWAREHOUSE	WareHouseCheck;
	int cnt;
	DWORD	dwChkSum;
	char* szComp;
	char	szMsgBuff[128];

	if (lpTransWareHouse->DataSize == 0) {
		ZeroMemory(lpWareHouse, sizeof(sWAREHOUSE));
		lpWareHouse->Money = 2023;
		lpWareHouse->Weight[0] = 197;
		CompWareHouseMoney = lpWareHouse->Money;
		return TRUE;
	}

	DecodeCompress((BYTE*)lpTransWareHouse->Data, (BYTE*)&WareHouseCheck);

	dwChkSum = 0;

	szComp = (char*)&WareHouseCheck;

	for (cnt = 0; cnt < sizeof(sWAREHOUSE); cnt++) {
		dwChkSum += szComp[cnt] * (cnt + 1);
	}
	if (dwChkSum == lpTransWareHouse->dwChkSum) {

		if (!flag && dwLastWareHouseChkSum && dwLastWareHouseChkSum != lpTransWareHouse->dwChkSum && !smConfig.DebugMode) {
			return FALSE;
		}

		memcpy(lpWareHouse, &WareHouseCheck, sizeof(sWAREHOUSE));

		CompWareHouseMoney = lpWareHouse->Money;

		for (cnt = 0; cnt < 100; cnt++) {
			if (lpWareHouse->WareHouseItem[cnt].Flag)
			{
				if (!lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0)
					lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0 = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum + lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;

				if (!lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime) {
					lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime =
						(lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head >> 2) ^ (lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum << 2);
				}

				if ((lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE & sinITEM_MASK2) == sinSP1) {
					if (lpWareHouse->WareHouseItem[cnt].sItemInfo.PotionCount > 1)	lpWareHouse->WareHouseItem[cnt].Flag = 0;
				}

				// Deleta item temporário do Baú
				if (DeleteEventItem_TimeOut(&lpWareHouse->WareHouseItem[cnt].sItemInfo) == TRUE) {
					lpWareHouse->WareHouseItem[cnt].Flag = 0;

					CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, mgItemTimeOut, lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemName);

					// Fix: Baú não salvando quando o item de tempo é expirado dentro dele
					SaveWareHouse(lpWareHouse);
				}
			}
		}
		return TRUE;
	}

	return FALSE;
}


int	LoadCaravanHouse(TRANS_CARAVAN * lpTransCaravan, sCARAVAN * lpCaravan, int flag)
{
	sCARAVAN	CaravanCheck;
	int cnt;
	DWORD	dwChkSum;
	char* szComp;
	char	szMsgBuff[128];

	if (lpTransCaravan->DataSize == 0) {
		ZeroMemory(lpCaravan, sizeof(sCARAVAN));
		lpCaravan->Money = 2023;
		lpCaravan->Weight[0] = 197;
		CompCaravanMoney = lpCaravan->Money;
		return TRUE;
	}

	DecodeCompress((BYTE*)lpTransCaravan->Data, (BYTE*)&CaravanCheck);

	dwChkSum = 0;

	szComp = (char*)&CaravanCheck;

	for (cnt = 0; cnt < sizeof(sCARAVAN); cnt++) {
		dwChkSum += szComp[cnt] * (cnt + 1);
	}
	if (dwChkSum == lpTransCaravan->dwChkSum) {

		if (!flag && dwLastCaravanChkSum && dwLastCaravanChkSum != lpTransCaravan->dwChkSum && !smConfig.DebugMode) {
			return FALSE;
		}

		memcpy(lpCaravan, &CaravanCheck, sizeof(sCARAVAN));

		CompCaravanMoney = lpCaravan->Money;

		for (cnt = 0; cnt < 100; cnt++) {
			if (lpCaravan->CaravanItem[cnt].Flag)
			{
				if (!lpCaravan->CaravanItem[cnt].sItemInfo.Temp0)
					lpCaravan->CaravanItem[cnt].sItemInfo.Temp0 = lpCaravan->CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum + lpCaravan->CaravanItem[cnt].sItemInfo.CODE;

				if (!lpCaravan->CaravanItem[cnt].sItemInfo.ItemHeader.dwTime) {
					lpCaravan->CaravanItem[cnt].sItemInfo.ItemHeader.dwTime =
						(lpCaravan->CaravanItem[cnt].sItemInfo.ItemHeader.Head >> 2) ^ (lpCaravan->CaravanItem[cnt].sItemInfo.ItemHeader.dwChkSum << 2);
				}

				if ((lpCaravan->CaravanItem[cnt].sItemInfo.CODE & sinITEM_MASK2) == sinSP1) {
					if (lpCaravan->CaravanItem[cnt].sItemInfo.PotionCount > 1)	lpCaravan->CaravanItem[cnt].Flag = 0;
				}

				// Deleta item temporário do Baú
				if (DeleteEventItem_TimeOut(&lpCaravan->CaravanItem[cnt].sItemInfo) == TRUE) {
					lpCaravan->CaravanItem[cnt].Flag = 0;

					CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, mgItemTimeOut, lpCaravan->CaravanItem[cnt].sItemInfo.ItemName);

					// Fix: Baú não salvando quando o item de tempo é expirado dentro dele
					SaveCaravan(lpCaravan);
				}
			}
		}
		return TRUE;
	}

	return FALSE;
}


int	LoadWareHouse(TRANS_WAREHOUSE * lpTransWareHouse, sWAREHOUSE * lpWareHouse)
{
	return LoadWareHouse(lpTransWareHouse, lpWareHouse, 0);
}

int	LoadCaravanHouse(TRANS_CARAVAN * lpTransCaravan, sCARAVAN * lpCaravan)
{
	return LoadCaravanHouse(lpTransCaravan, lpCaravan, 0);
}

int SendCollectMoney(int Money)
{
	sITEM	sItemMoney;
	int j;


	memset(&sItemMoney, 0, sizeof(sITEM));
	sItemMoney.sItemInfo.CODE = sinGG1 | sin01;
	for (j = 0; j < MAX_ITEM; j++) {
		if (sItem[j].CODE == sItemMoney.sItemInfo.CODE) { //ÄÚÀÎÀÌ¸é 
			memcpy(&sItemMoney, &sItem[j], sizeof(sITEM));
			sItemMoney.sItemInfo.Money = Money;			//±Ý¾×À» ³Ñ°ÜÁØ´Ù 
			sItemMoney.sItemInfo.ItemKindCode = smTRANSCODE_COLLECTMONEY;
			ReformItem(&sItemMoney.sItemInfo);		//ÀÎÁõ 
			if (sinThrowItemToFeild(&sItemMoney)) {
				AddInvenMoney(-Money);				//ÀÎº¥ µ· È®ÀÎ
				return TRUE;
			}
		}
	}

	return FALSE;
}

//PK´çÇØ¼­ Á×À½
int PlayerKilling(DWORD dwObjectSerial)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_PLAYERKILLING;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;				//»ó´ë¹æ ÄÚµå
	TransCommand.LParam = lpCurPlayer->dwObjectSerial;		//³» ÄÚµå
	TransCommand.SParam = 0;

	if (smWsockUserServer) {
		return smWsockUserServer->Send2((char*)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}



//¹Í½ºÃÄ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendCraftItemToServer(void* lpsCraftItem_Send)
{

	sCRAFTITEM_SERVER* lpCraftItem_Send = (sCRAFTITEM_SERVER*)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_CRAFTITEM;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)lpCraftItem_Send, lpCraftItem_Send->size, TRUE);
	}

	return FALSE;
}

//À§ÇÁ¿ë À® ¾ÆÀÌÅÛ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendWingItemToServer(void* lpsCraftItem_Send)
{

	sCRAFTITEM_SERVER* lpCraftItem_Send = (sCRAFTITEM_SERVER*)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_WINGITEM;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)lpCraftItem_Send, lpCraftItem_Send->size, TRUE);
	}

	return FALSE;
}

//Event priston xxstr
int SendWingItemEVENTPRISTON_ToServer(void* lpsCraftItem_Send)
{

	sCRAFTITEM_SERVER* lpCraftItem_Send = (sCRAFTITEM_SERVER*)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_WINGITEMEVENT;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)lpCraftItem_Send, lpCraftItem_Send->size, TRUE);
	}

	return FALSE;
}


//¿¡ÀÌÂ¡ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendAgingItemToServer(void* lpsCraftItem_Send)
{

	sCRAFTITEM_SERVER* lpCraftItem_Send = (sCRAFTITEM_SERVER*)lpsCraftItem_Send;

	lpCraftItem_Send->size = sizeof(sCRAFTITEM_SERVER);
	lpCraftItem_Send->code = smTRANSCODE_AGINGITEM;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)lpCraftItem_Send, lpCraftItem_Send->size, TRUE);
	}

	return FALSE;
}

int SendLinkCoreToServer(sITEMINFO * lpItem)
{
	TRANS_ITEMINFO	TransItemInfo;
	int len;

	TransItemInfo.size = sizeof(TRANS_ITEMINFO);
	TransItemInfo.code = smTRANSCODE_MAKE_LINKCORE;
	memcpy(&TransItemInfo.Item, lpItem, sizeof(sITEMINFO));

	TransItemInfo.x = lpCurPlayer->pX;
	TransItemInfo.y = lpCurPlayer->pY;
	TransItemInfo.z = lpCurPlayer->pZ;

	TransItemInfo.dwSeCode[0] = 0;
	TransItemInfo.dwSeCode[1] = 0;
	TransItemInfo.dwSeCode[2] = 0;
	TransItemInfo.dwSeCode[3] = 0;

	len = lstrlen(lpItem->ItemName);
	if (lpItem->ItemName[len + 1]) {
		TransItemInfo.code = smTRANSCODE_USE_LINKCORE;

		if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM) {
			return FALSE;
		}

		if (smWsockServer)
			return smWsockServer->Send2((char*)&TransItemInfo, TransItemInfo.size, TRUE);

	}

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)&TransItemInfo, TransItemInfo.size, TRUE);
	}

	return FALSE;
}

int SucessLinkCore(smTRANS_COMMAND_EX * lpTransCommandEx)
{
	int x, z, cnt;

	sITEMINFO* lpItem;

	lpItem = FindInvenItem(lpTransCommandEx->WxParam, lpTransCommandEx->LxParam, lpTransCommandEx->SxParam);

	if (lpItem) {
		SendUseItemCodeToServer(lpItem);
		cInvenTory.DeleteInvenItemToServer(lpTransCommandEx->WxParam, lpTransCommandEx->LxParam, lpTransCommandEx->SxParam);
		ResetInvenItemCode();

		if (FindInvenItem(lpTransCommandEx->WxParam, lpTransCommandEx->LxParam, lpTransCommandEx->SxParam) != 0) return FALSE;

	}
	else
		return FALSE;


	CloseEachPlayer();

	WarpFieldNearPos(lpTransCommandEx->EParam, lpTransCommandEx->WParam, lpTransCommandEx->SParam, &x, &z);

	lpCurPlayer->SetPosi(lpTransCommandEx->WParam, lpTransCommandEx->LParam, lpTransCommandEx->SParam, 0, 0, 0);
	TraceCameraPosi.x = lpCurPlayer->pX;
	TraceCameraPosi.y = lpCurPlayer->pY;
	TraceCameraPosi.z = lpCurPlayer->pZ;
	TraceTargetPosi.x = lpCurPlayer->pX;
	TraceTargetPosi.y = lpCurPlayer->pY;
	TraceTargetPosi.z = lpCurPlayer->pZ;

	lpCurPlayer->OnStageField = -1;
	if (smGameStage[0]) {
		cnt = smGameStage[0]->GetFloorHeight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpCurPlayer->Pattern->SizeHeight);
		lpCurPlayer->OnStageField = 0;
	}
	if (cnt == CLIP_OUT && smGameStage[1]) {
		lpCurPlayer->OnStageField = 1;
	}

	StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 48 * fONE, lpCurPlayer->pZ, EFFECT_RETURN1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½
	RestartPlayCount = 350;		//5ÃÊ µ¿¾È ¹«Àû

	return TRUE;
}


//¿¡ÀÌÂ¡ ¿Ï·áµÈ ¾ÆÀÌÅÛÀ» ¼­¹ö·Î º¸³¿
int SendAgingUpgradeItemToServer(sITEMINFO * lpItem)
{
	TRANS_ITEMINFO	TransItemInfo;

	if (TransAgingItemFlag) return FALSE;

	TransItemInfo.code = smTRANSCODE_AGING_UPGRADE;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);
	TransItemInfo.x = 0;
	TransItemInfo.y = 0;
	TransItemInfo.z = 0;
	memcpy(&TransItemInfo.Item, lpItem, sizeof(sITEMINFO));

	if (smWsockDataServer)
	{
		if (smWsockDataServer->Send2((char*)&TransItemInfo, TransItemInfo.size, TRUE) == TRUE)
		{
			TransAgingItemFlag = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}

/*
//¾ÆÀÌÅÛ È®ÀÎ
#define smTRANSCODE_CHECKITEM			0x50320300
//¾ÆÀÌÅÛ ¿À·ù È®ÀÎ
#define smTRANSCODE_ERRORITEM			0x50320310

struct TRANS_ITEMINFO {
	int	size,code;

	//char	szItemName[32];
	//DWORD	ItemCode;

	sITEMINFO	Item;

	int x,y,z;

};

struct	TRANS_ITEM_CODE {
	int size,code;

	DWORD	dwFlag;

	DWORD	dwItemCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
	DWORD	dwTime;
}


*/


//ÀÎº¥Åä¸®ÀÇ ¹°¾à°¹¼ö¸¦ ÀüºÎ 1·Î ¼¼ÆÃ
int ClearInvenPotionCount()
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag && (cInvenTory.InvenItem[cnt].sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
			cInvenTory.InvenItem[cnt].sItemInfo.PotionCount = 1;
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag && (cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
			cInvenTory.InvenItemTemp[cnt].sItemInfo.PotionCount = 1;
		}
	}

	if (cTrade.OpenFlag) {
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			if (sTrade.TradeItem[cnt].Flag && (sTrade.TradeItem[cnt].sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
				sTrade.TradeItem[cnt].sItemInfo.PotionCount = 1;
			}
		}
	}

	if (MouseItem.Flag && (MouseItem.sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1))
		MouseItem.sItemInfo.PotionCount = 1;

	cInvenTory.ReFormPotionNum();

	SaveGameData();

	return TRUE;
}

sITEMINFO* FindItemFromCode(DWORD dwItemCode)
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].sItemInfo.CODE == dwItemCode) {
			return &cInvenTory.InvenItem[cnt].sItemInfo;
		}
	}
	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag && cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE == dwItemCode) {
			return &cInvenTory.InvenItemTemp[cnt].sItemInfo;
		}
	}

	return NULL;
}

sITEMINFO* FindItemFromName(char name[64])
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag && (lstrcmpi(cInvenTory.InvenItem[cnt].sItemInfo.ItemName, name) == 0)) {
			return &cInvenTory.InvenItem[cnt].sItemInfo;
		}
	}
	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag && (lstrcmpi(cInvenTory.InvenItemTemp[cnt].sItemInfo.ItemName, name) == 0)) {
			return &cInvenTory.InvenItem[cnt].sItemInfo;
		}
	}

	return nullptr;
}

sITEM* FindItemFromName2(char name[64])
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if ((lstrcmpi(cInvenTory.InvenItem[cnt].sItemInfo.ItemName, name) == 0)) {
			return &cInvenTory.InvenItem[cnt];
		}
	}

	return NULL;
}

DWORD	dwLastCheckItemTime = 0;

//¾ÆÀÌÅÛÀ» ¼­¹ö¿¡ º¸³»¼­ È®ÀÎ
int	SendCheckItemToServer(sITEMINFO * lpItem)
{

	TRANS_ITEMINFO	TransItemInfo;

	//return FALSE;

	if ((dwLastCheckItemTime + 10000) > dwPlayTime) {
		//º¸³½Áö 10ÃÊ ¹Ì¸¸
		return FALSE;
	}
	//if ( !lpItem->ItemName[0] ) return FALSE;

	TransItemInfo.code = smTRANSCODE_CHECKITEM;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);

	memcpy(&TransItemInfo.Item, lpItem, sizeof(sITEMINFO));

	TransItemInfo.x = 0;
	TransItemInfo.y = 0;
	TransItemInfo.z = 0;

	if (smWsockUserServer) {
		smWsockUserServer->Send2((char*)&TransItemInfo, TransItemInfo.size, TRUE);
	}
	dwLastCheckItemTime = dwPlayTime;

	return TRUE;
}

//¾ÆÀÌÅÛ È®ÀÎ °á°ú µµÂø
int RecvCheckItemFromServer(TRANS_ITEM_CODE * lpTransItemCode)
{

	switch (lpTransItemCode->code) {
	case smTRANSCODE_CHECKITEM:
		//¾ÆÀÌÅÛ ¼­¹ö È®ÀÎ ÄÚµå ¹ÞÀ½
		//cInvenTory.ResetInvenItemToServer( lpTransItemCode->dwItemCode , lpTransItemCode->dwHead ,
		//	lpTransItemCode->dwChkSum , lpTransItemCode->dwTime );
		break;

	case smTRANSCODE_ERRORITEM:
		//¾ÆÀÌÅÛ ¼­¹öÈ®ÀÎ ÄÚµå ¿À·ù ( ¾ÆÀÌÅÛ Á¦°Å )
		cInvenTory.DeleteInvenItemToServer(lpTransItemCode->dwItemCode, lpTransItemCode->dwHead, lpTransItemCode->dwChkSum);

		//ÀÎº¥Åä¸®ÀÇ ÁöÁ¤ÇÑ ¾ÆÀÌÅÛÀ» Ã£¾Æ °Ë»öÇÏ¿© Ã£´Â´Ù
		if (FindInvenItem(lpTransItemCode->dwItemCode, lpTransItemCode->dwHead, lpTransItemCode->dwChkSum) == 0) {

			SendInvenItemError(0, lpTransItemCode->dwItemCode, lpTransItemCode->dwHead, lpTransItemCode->dwChkSum);
			ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
		}

		break;

	case smTRANSCODE_CLEARPOTION:
		//ÀÎº¥Åä¸®ÀÇ ¹°¾à°¹¼ö¸¦ ÀüºÎ 1·Î ¼¼ÆÃ
		ClearInvenPotionCount();
		break;
	}

	return TRUE;
}

int	SellItemToServer(sITEMINFO * lpItem, int Count)
{

	TRANS_ITEMINFO	TransItemInfo;

	TransItemInfo.code = smTRANSCODE_SHOP_SELLITEM;
	TransItemInfo.size = sizeof(TRANS_ITEMINFO);

	memcpy(&TransItemInfo.Item, lpItem, sizeof(sITEMINFO));

	TransItemInfo.x = Count;
	TransItemInfo.y = lpCurPlayer->smCharInfo.Money;
	TransItemInfo.z = 0;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)&TransItemInfo, TransItemInfo.size, TRUE);
	}

	return FALSE;
}


int Chk_InventPosCount = 1;
DWORD	dwCheckInvenItemTime = 0;

int CheckInvenItemToServer()
{
	int cnt;

	if (dwCheckInvenItemTime > dwPlayTime) return TRUE;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].ItemPosition == Chk_InventPosCount) {
			//¾ÆÀÌÅÛÀ» ¼­¹ö¿¡ º¸³»¼­ È®ÀÎ
			SendCheckItemToServer(&cInvenTory.InvenItem[cnt].sItemInfo);
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].ItemPosition == Chk_InventPosCount) {
			//¾ÆÀÌÅÛÀ» ¼­¹ö¿¡ º¸³»¼­ È®ÀÎ
			SendCheckItemToServer(&cInvenTory.InvenItemTemp[cnt].sItemInfo);
		}
	}

	Chk_InventPosCount++;
	dwCheckInvenItemTime = dwPlayTime + 55 * 1000;

	if (Chk_InventPosCount >= 11 || Chk_InventPosCount < 1) Chk_InventPosCount = 1;

	return TRUE;
}

////////////////////////////// ¸Þ¸ð¸® Å¸ÀÓ Ã½Å© ¾²·¹µå ///////////////////////////////

HANDLE	hTimeCheckThread = 0;
int		TimeCheckCounter;

DWORD WINAPI TimeCheckThread(void* pInfo)
{
	HANDLE	hThread;
	DWORD	dwTime;
	DWORD	dwLastTime;
	int		cnt;

	hThread = GetCurrentThread();
	dwTime = GetCurrentTime();
	dwLastTime = dwTime;
	TimeCheckCounter = 0;

	while (1) {
		dwTime = GetCurrentTime();
		cnt = dwTime - dwLastTime;
		if (cnt > PlayTimerMax) PlayTimerMax = cnt;
		dwLastTime = dwTime;
		TimeCheckCounter++;
		Sleep(800);
	}

	ExitThread(TRUE);
	return TRUE;
}


//·ÎµùÁß È­¸é Ç¥½Ã
int OpenTimeCheckThread()
{
	DWORD	dwTimeCheckThreadID;

	if (!hTimeCheckThread) {
		hTimeCheckThread = CreateThread(NULL, 0, TimeCheckThread, 0, 0, &dwTimeCheckThreadID);
	}

	return TRUE;
}

int	PlayTimerMax_SendingCount = 0;

int SendPlayTimerMax()
{
	smTRANS_COMMAND	smTransCommand;

	if (PlayTimerMax >= 10000 || (PlayTimerMax_SendingCount & 0x7) == 0)
	{
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_PROCESS_TIMEMAX;
		smTransCommand.WParam = PlayTimerMax;
		smTransCommand.LParam = TimeCheckCounter;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		if (smWsockDataServer)
			smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

		PlayTimerMax = 0;
	}

	PlayTimerMax_SendingCount++;

	return TRUE;
}


////////////////////////////////////////////////////////////////////////////////////////




//¼­¹ö¿¡¼­ ¸Þ¸ð¸® °ªÀ» Á¤ÇØ¼­ °¡Á®°£´Ù
DWORD funcCheckMemSum(DWORD FuncPoint, int count)
{
	int cnt;
	DWORD dwSum;
	int	xcnt;
	DWORD* lpGetPrtectPoint;


	lpGetPrtectPoint = (DWORD*)FuncPoint;

	dwSum = 0;
	xcnt = 1;

	for (cnt = 0; cnt < count; cnt++) {
		xcnt += cnt;
		dwSum += lpGetPrtectPoint[cnt] * xcnt;
	}

	return dwSum;
}

/*
		dwResult = fnChkMem( TransFuncMem->Param[0] , TransFuncMem->Param[1] );

		smTransCommand.code = smTRANSCODE_FUNCTON_RESULT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = TransFuncMem->Param[0];
		smTransCommand.LParam = TransFuncMem->Param[1];
		smTransCommand.SParam = TransFuncMem->Param[2];
		smTransCommand.EParam = dwResult;

*/
//¼­¹ö¿¡¼­ ¸Þ¸ð¸® °ªÀ» Á¤ÇØ¼­ °¡Á®°£´Ù
//DWORD funcCheckMemSum2( DWORD FuncPoint , int count )

///////////////////////////////////////////////////////////////////////////////

DWORD	dwFuncList[][2] = {
	{ (DWORD)funcCheckMemSum << 1				,	512		},
	{ (DWORD)Check_PlaySubReleaseFunc << 1	,	512		},
	{ (DWORD)PlayPat3D << 1					,	2048	},
	{ (DWORD)Check_CodeSafe << 1				,	1024	},
	{ (DWORD)AddExp << 1						,	512		},
	{ (DWORD)sinSetLife << 1					,	512		},
	{ (DWORD)NetWorkPlay << 1					,	2800	},

	{ (DWORD)E_Shield_BlockRate << 1			,	((Meteo_UseMana - E_Shield_BlockRate) + 10) },		//½ºÅ³ Å×ÀÌºí º¸¾È

	{ (DWORD)Code_VRamBuffOpen << 1			,	3600	},			//¸¶Áö¸· ²¨´Â º°µµ Ã½Å© ÇÑ´Ù
	{ 0,0 }
};



//Å¬¶óÀÌ¾ðÆ® Æã¼Ç À§Ä¡¿Í °ªÀ» ¼­¹ö¿¡ º¸°í
int	SendClientFuncPos()
{

	TRANS_CLIENT_FUNPOS	TransClientFuncPos;
	int cnt;


	ZeroMemory(&TransClientFuncPos, sizeof(TRANS_CLIENT_FUNPOS));

	TransClientFuncPos.code = smTRANSCODE_CLIENT_FUNCPOS;
	TransClientFuncPos.size = sizeof(TRANS_CLIENT_FUNPOS);
	TransClientFuncPos.ClientVersion = Client_Version;

	cnt = 0;
	TransClientFuncPos.dwFunc_CheckMemSum = dwFuncList[cnt][0] >> 1;
	TransClientFuncPos.dwFuncValue[cnt].dwFunc = dwFuncList[cnt][0] >> 1;
	TransClientFuncPos.dwFuncValue[cnt].dwLen = dwFuncList[cnt][1];
	TransClientFuncPos.dwFuncValue[cnt].dwChkSum = funcCheckMemSum(TransClientFuncPos.dwFuncValue[cnt].dwFunc, TransClientFuncPos.dwFuncValue[cnt].dwLen);
	//TransClientFuncPos.dwFuncValue[cnt].dwChkSum = fnChkMem( TransClientFuncPos.dwFuncValue[cnt].dwFunc , TransClientFuncPos.dwFuncValue[cnt].dwLen );

	while (1) {

		if (dwFuncList[cnt][0] == 0) break;
		if (cnt >= dwFUNC_VALUE_MAX)  break;

		TransClientFuncPos.dwFuncValue[cnt].dwFunc = dwFuncList[cnt][0] >> 1;
		TransClientFuncPos.dwFuncValue[cnt].dwLen = dwFuncList[cnt][1];
		TransClientFuncPos.dwFuncValue[cnt].dwChkSum = funcCheckMemSum(TransClientFuncPos.dwFuncValue[cnt].dwFunc, TransClientFuncPos.dwFuncValue[cnt].dwLen);

		cnt++;
	}

	TransClientFuncPos.FuncCount = cnt;

	//¼­¹ö¿¡ º¸°í
	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&TransClientFuncPos, TransClientFuncPos.size, TRUE);

	return FALSE;
}


//¸Þ¸ð¸® °Ë»ç ¸ðµâ ¹ÞÀ½
int	RecvMemFuncData(TRANS_FUNC_MEMORY * TransFuncMem)
{
	smTRANS_COMMAND	smTransCommand;

	fnChkMem = (LPFN_CheckMem)((void*)TransFuncMem->szData);

	smTransCommand.size = 0;

	fnChkMem(TransFuncMem, &smTransCommand);

	if (smWsockDataServer && smTransCommand.size > 0)
		smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}


////////////////////////////// ¾ÆÀÌÅÛ ÀÓÁö ÀúÀå Å¥Ã³¸® //////////////////////////

#define	RECV_TRANSITEM_QUE_MAX			256
#define	RECV_TRANSITEM_QUE_MASK			255

TRANS_ITEMINFO	Recv_TransItemInfo_Que[RECV_TRANSITEM_QUE_MAX];

int	RecvTransItemQue_Push = 0;
int	RecvTransItemQue_Pop = 0;

//¾ÆÀÌÅÛ Å¥¿¡ ÀÓ½Ã ÀúÀå
int	PushRecvTransItemQue(TRANS_ITEMINFO * lpTransItemInfo)
{
	int mCnt;

	mCnt = RecvTransItemQue_Push & RECV_TRANSITEM_QUE_MASK;

	memcpy(&Recv_TransItemInfo_Que[mCnt], lpTransItemInfo, sizeof(TRANS_ITEMINFO));
	RecvTransItemQue_Push++;
	RecvTransItemQue_Pop = RecvTransItemQue_Push - RECV_TRANSITEM_QUE_MASK;
	if (RecvTransItemQue_Pop < 0) RecvTransItemQue_Pop = 0;

	return TRUE;
}

//ÀÓ½ÃÀúÀåµÈ ¾ÆÀÌÅÛ Å¥¿¡¼­ Ã£±â
TRANS_ITEMINFO* FindRecvTransItemQue(DWORD dwCode, DWORD dwHead, DWORD dwChkSum)
{
	int cnt, mCnt;

	for (cnt = RecvTransItemQue_Pop; cnt < RecvTransItemQue_Push; cnt++) {
		mCnt = cnt & RECV_TRANSITEM_QUE_MASK;

		if (Recv_TransItemInfo_Que[mCnt].Item.CODE == dwCode &&
			Recv_TransItemInfo_Que[mCnt].Item.ItemHeader.Head == dwHead &&
			Recv_TransItemInfo_Que[mCnt].Item.ItemHeader.dwChkSum == dwChkSum) {

			return &Recv_TransItemInfo_Que[mCnt];
		}
	}

	return NULL;
}

//ÀÓ½ÃÀúÀåµÈ ¾ÆÀÌÅÛ Å¥¿¡ ¾ÆÀÌÅÛ ´ë±âÁßÀÎÁö °Ë»ç
int CheckRecvTrnsItemQue()
{
	int cnt, mCnt;

	for (cnt = RecvTransItemQue_Pop; cnt < RecvTransItemQue_Push; cnt++) {
		mCnt = cnt & RECV_TRANSITEM_QUE_MASK;
		if (Recv_TransItemInfo_Que[mCnt].Item.CODE) return FALSE;
	}

	return TRUE;
}

//»ç¿ëÇÑ ¾ÆÀÌÅÛ ÄÚµå ¼­¹ö·Î º¸³¿
int SendUseItemCodeToServer(sITEMINFO * lpItem)
{
	smTRANS_COMMAND_EX	smTransCommandEx;

	smTransCommandEx.code = smTRANSCODE_USEITEM_CODE;
	smTransCommandEx.size = sizeof(smTRANS_COMMAND);
	smTransCommandEx.WParam = lpItem->CODE;
	smTransCommandEx.LParam = lpItem->ItemHeader.Head;
	smTransCommandEx.SParam = lpItem->ItemHeader.dwChkSum;
	smTransCommandEx.EParam = lpItem->PotionCount;

	smTransCommandEx.WxParam = 0;
	smTransCommandEx.LxParam = 0;
	smTransCommandEx.SxParam = 0;
	smTransCommandEx.ExParam = 0;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)&smTransCommandEx, smTransCommandEx.size, TRUE);
	}

	return FALSE;
}

DWORD dwTickCrystal = 0;

int RecvCommandUser(smTRANS_COMMAND_EX * lpTransCommandEx)
{
	smCHAR* lpChar;
	smCHAR* lpChar2;

	switch (lpTransCommandEx->WParam) {
	case smCOMMNAD_USER_WARP:

		if (lpTransCommandEx->WxParam != 502)
		{
			StartEffect(lpTransCommandEx->LParam, lpTransCommandEx->SParam, lpTransCommandEx->EParam, EFFECT_RETURN1);
			SkillPlaySound(SKILL_SOUND_LEARN, lpTransCommandEx->LParam, lpTransCommandEx->SParam, lpTransCommandEx->EParam);
		}

		if (lpTransCommandEx->SxParam == lpCurPlayer->dwObjectSerial)
		{
			int iCode = 0;
			bool newCrystal = false;

			// Verifica o LxParam, se for TRUE é cristal
			if (lpTransCommandEx->LxParam == TRUE)
			{
				switch (lpTransCommandEx->WxParam)
				{
					// Verifica qual cristal é
				case sinGP1 | sin01:
					iCode = 1;
					break;
				case sinGP1 | sin02:
					iCode = 2;
					break;
				case sinGP1 | sin03:
					iCode = 3;
					break;
				case sinGP1 | sin04:
					iCode = 4;
					break;
				case sinGP1 | sin05:
					iCode = 5;
					break;
				case sinGP1 | sin06:
					iCode = 6;
					break;
				case sinGP1 | sin07:
					iCode = 7;
					break;
				case sinGP1 | sin08:
					iCode = 8;
					break;
				case sinGP1 | sin09:
					iCode = 9;
					break;
				case sinGP1 | sin10:
					iCode = 10;
					break;
				case sinGP1 | sin11:
					iCode = 11;
					break;
				case sinGP1 | sin12:
					iCode = 12;
					break;
				case sinGP1 | sin13:
					iCode = 13;
					break;
				case sinGP1 | sin14:
					iCode = 14;
					break;
				case sinGP1 | sin15:
					iCode = 15;
					break;
				case sinGP1 | sin16:
					iCode = 16;
					break;
				case sinGP1 | sin23:
					iCode = 23;
					break;
				case sinGP1 | sin24:
					iCode = 24;
					break;
				case sinGP1 | sin25:
					iCode = 25;
					break;
				default:
					break;
				}

				// Criar o Buff
				sSKILL haCrystalTimer;
				for (int j = 0; j < SIN_MAX_SKILL; j++)
				{
					if (sSkill[j].CODE == iCode && !sSkill[j].SkillTaget_CODE)
					{
						limitebuff++;

						sSkill[j].SkillTaget_CODE = lpTransCommandEx->ExParam;
						memcpy(&haCrystalTimer, &sSkill[j], sizeof(sSKILL));
						haCrystalTimer.UseTime = 500; // 5 minutos
						haCrystalTimer.SkillTaget_CODE = lpTransCommandEx->ExParam;
						sinContinueSkillSet(&haCrystalTimer);
						break;
					}
				}
			}
		}

		break;

	case smCOMMNAD_USER_AGINGUP:
		lpChar = FindChrPlayer(lpTransCommandEx->WxParam);
		if (lpChar) {
			StartEffect(lpChar->pX, lpChar->pY, lpChar->pZ, EFFECT_AGING);
			esPlaySound(7, GetDistVolume(lpChar->pX, lpChar->pY, lpChar->pZ), 1600);
		}
		break;

	case smCOMMNAD_USER_HOLY_INCANTATION:
		lpChar = FindChrPlayer(lpTransCommandEx->LParam);
		if (lpChar) {
			AssaParticle_HolyIncantationPart(lpChar, lpTransCommandEx->SParam);
		}
		break;

	case smCOMMNAD_USER_RESURRECTION:
		lpChar = FindChrPlayer(lpTransCommandEx->LParam);
		if (lpChar) {
			AssaParticle_ResurrectionPart(lpChar);
		}
		break;

	case smCOMMNAD_USER_FORCEORB:
		lpChar = FindChrPlayer(lpTransCommandEx->WxParam);
		if (lpChar) {
			StartSkill(lpChar->pX, lpChar->pY, lpChar->pZ, 0, 0, 0, SKILL_UP1);
			SkillPlaySound(SKILL_SOUND_LEARN, lpChar->pX, lpChar->pY, lpChar->pZ);
		}
		break;

	case smCOMMNAD_USER_VALENTO:
		lpChar = FindChrPlayer(lpTransCommandEx->LParam);
		lpChar2 = FindChrPlayer(lpTransCommandEx->SParam);
		if (lpChar && lpChar2) {
			sinSkillEffect_Soul_Sucker(lpChar, lpChar2);
		}
		break;

	case smCOMMNAD_USER_FIREFLOWER:
		lpChar = FindChrPlayer(lpTransCommandEx->WxParam);
		if (lpChar && lpChar != lpCurPlayer) {
			DWORD	dwTable = GetTableNum(lpTransCommandEx->LParam);
			AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 1000, (dwTable >> 24) & 0xFF, 0, 0, lpChar);
			AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 1500, (dwTable >> 16) & 0xFF, 0, 0, lpChar);
			AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 2000, (dwTable >> 8) & 0xFF, 0, 0, lpChar);
			AddTimeEffect(SKILL_EFFECT_FIREFLOWER, dwPlayTime + 2500, dwTable & 0xFF, 0, 0, lpChar);
		}
		break;

	}

	return TRUE;
}

int	SendCommandUser(int wParam, int lParam, int sParam, int eParam)
{
	smTRANS_COMMAND_EX	smTransCommandEx;

	smTransCommandEx.code = smTRANSCODE_COMMAND_USER;
	smTransCommandEx.size = sizeof(smTRANS_COMMAND_EX);
	smTransCommandEx.WParam = wParam;
	smTransCommandEx.LParam = lParam;
	smTransCommandEx.SParam = sParam;
	smTransCommandEx.EParam = eParam;

	smTransCommandEx.WxParam = lpCurPlayer->dwObjectSerial;
	smTransCommandEx.LxParam = 0;
	smTransCommandEx.SxParam = 0;
	smTransCommandEx.ExParam = 0;

	if (smWsockUserServer) {
		return smWsockUserServer->Send((char*)&smTransCommandEx, smTransCommandEx.size, TRUE);
	}

	return FALSE;
}

int	SendCommand_AreaServer(DWORD dwCode, int wParam, int lParam, int sParam, int eParam)
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK* lpsmSock;

	smTransCommand.code = dwCode;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = wParam;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock) {
		return lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	return FALSE;
}

int	SendCommand_DataServer(DWORD dwCode, int wParam, int lParam, int sParam, int eParam)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.code = dwCode;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = wParam;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	if (smWsockDataServer) {
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	return FALSE;
}

struct	sDAMAGE_RECORD {
	int	Damage;
	int	Count;
};

sDAMAGE_RECORD	sRecvDamage[3];
sDAMAGE_RECORD	sSendDamage[3];

////////////////////////////////////////////////////////////////////////////
//°ø°Ý·Â ±â·Ï ÃÊ±âÈ­
int	Init_RecordDamage()
{
	ZeroMemory(sRecvDamage, sizeof(sDAMAGE_RECORD) * 3);
	ZeroMemory(sSendDamage, sizeof(sDAMAGE_RECORD) * 3);

	return TRUE;
}

//¹ÞÀº °ø°Ý·Â ±â·Ï
int	Record_RecvDamage(smWINSOCK * lpsmSock, int Damage)
{
	smTRANS_COMMAND	smTransCommand;

	if (smWsockServer == lpsmSock) {
		sRecvDamage[0].Damage -= Damage;
		sRecvDamage[0].Count--;

		if ((sRecvDamage[0].Count & 0x1FF) == 0) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[0].Count;
			smTransCommand.LParam = sRecvDamage[0].Damage;
			smTransCommand.SParam = sSendDamage[0].Count;
			smTransCommand.EParam = sSendDamage[0].Damage;
			if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		}
	}
	else if (smWsockUserServer == lpsmSock) {
		sRecvDamage[1].Damage -= Damage;
		sRecvDamage[1].Count--;

		if ((sRecvDamage[1].Count & 0x1FF) == 0) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[1].Count;
			smTransCommand.LParam = sRecvDamage[1].Damage;
			smTransCommand.SParam = sSendDamage[1].Count;
			smTransCommand.EParam = sSendDamage[1].Damage;
			if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		}

	}
	else if (smWsockExtendServer == lpsmSock) {
		sRecvDamage[2].Damage -= Damage;
		sRecvDamage[2].Count--;

		if ((sRecvDamage[2].Count & 0x1FF) == 0) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[2].Count;
			smTransCommand.LParam = sRecvDamage[2].Damage;
			smTransCommand.SParam = sSendDamage[2].Count;
			smTransCommand.EParam = sSendDamage[2].Damage;
			if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		}
	}

	return TRUE;
}


int	Record_SendDamage(smWINSOCK * lpsmSock, int Damage)
{
	smTRANS_COMMAND	smTransCommand;

	if (smWsockServer == lpsmSock) {
		sSendDamage[0].Damage -= Damage;
		sSendDamage[0].Count--;

		if ((sRecvDamage[0].Count & 0x1FF) == 0) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[0].Count;
			smTransCommand.LParam = sRecvDamage[0].Damage;
			smTransCommand.SParam = sSendDamage[0].Count;
			smTransCommand.EParam = sSendDamage[0].Damage;
			if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		}
	}
	else if (smWsockUserServer == lpsmSock) {
		sSendDamage[1].Damage -= Damage;
		sSendDamage[1].Count--;

		if ((sRecvDamage[1].Count & 0x1FF) == 0) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[1].Count;
			smTransCommand.LParam = sRecvDamage[1].Damage;
			smTransCommand.SParam = sSendDamage[1].Count;
			smTransCommand.EParam = sSendDamage[1].Damage;
			if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		}

	}
	else if (smWsockExtendServer == lpsmSock) {
		sSendDamage[2].Damage -= Damage;
		sSendDamage[2].Count--;

		if ((sRecvDamage[2].Count & 0x1FF) == 0) {
			smTransCommand.code = smTRANSCODE_REC_DAMAGEDATA;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = sRecvDamage[2].Count;
			smTransCommand.LParam = sRecvDamage[2].Damage;
			smTransCommand.SParam = sSendDamage[2].Count;
			smTransCommand.EParam = sSendDamage[2].Damage;
			if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
		}
	}

	return TRUE;
}

// Skills em PT aqui
int	SendPartySkillToServer(DWORD dwSkillCode, int SkillLevel, int Around, int wParam, int lParam, int sParam, int eParam)
{
	int x, y, z, dist;
	int cnt;
	int PartyCount;
	TRANS_PARTY_SKILL	TransPartySkill;
	smWINSOCK* lpsmSock;

	TransPartySkill.code = smTRANSCODE_PARTY_SKILL;
	TransPartySkill.dwSkillCode = dwSkillCode;
	TransPartySkill.wParam = wParam;
	TransPartySkill.lParam = lParam;
	TransPartySkill.sParam = sParam;
	TransPartySkill.eParam = eParam;

	PartyCount = 0;

	TransPartySkill.dwPartyUser[PartyCount++] = lpCurPlayer->dwObjectSerial;

	if (PARTYHANDLER->GetPartyData().iMembersCount > 0)
	{
		std::vector<smCHAR*> vUnitData = PARTYHANDLER->GetPartyMembers();

		for (auto& p : vUnitData)
		{
			if (p != UNITDATA)
			{
				if (p && p->Flag)
				{
					x = (UNITDATA->pX - p->pX) >> FLOATNS;
					y = (UNITDATA->pY - p->pY) >> FLOATNS;
					z = (UNITDATA->pZ - p->pZ) >> FLOATNS;
					dist = x * x + y * y + z * z;

					if (dist < (Around * Around))
					{
						TransPartySkill.dwPartyUser[PartyCount++] = p->dwObjectSerial;

						if (TransPartySkill.PartyCount == _countof(TransPartySkill.dwPartyUser))
							break;
					}
				}
			}
		}
	}

	TransPartySkill.PartyCount = PartyCount;
	TransPartySkill.Point = SkillLevel;
	TransPartySkill.size = sizeof(TRANS_PARTY_SKILL) - (8 - PartyCount) * 4;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock) lpsmSock->Send2((char*)&TransPartySkill, TransPartySkill.size, TRUE);

	return TRUE;
}

//½ºÅ³ Ãë¼Ò
int SendCancelSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam)
{
	smWINSOCK* lpsmSock;
	smTRANS_COMMAND smTransCommand;

	smTransCommand.code = smTRANSCODE_CANCEL_SKILL;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwSkillCode;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}

//½ºÅ³ º¯°æ
int SendUpdateSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam)
{
	smWINSOCK* lpsmSock;
	smTRANS_COMMAND smTransCommand;

	smTransCommand.code = smTRANSCODE_UPDATEL_SKILL;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwSkillCode;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock) lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}

//¼­¹ö¿¡ ½ºÅ³¼öÇà º¸³¿
int SendProcessSKillToServer(DWORD dwSkillCode, int point, int Param1, int Param2)
{

	smWINSOCK* lpsmSock;
	smTRANS_COMMAND smTransCommand;

	smTransCommand.code = smTRANSCODE_PROCESS_SKILL2;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwSkillCode | (point << 8);
	smTransCommand.LParam = lpCurPlayer->dwObjectSerial;
	smTransCommand.SParam = Param1;
	smTransCommand.EParam = Param2;

	smTransCommand.LParam = dm_GetCommandChkSum(&smTransCommand, lpCurPlayer->dwObjectSerial); //ÄÚ¸Çµå Ã½Å©¼¶

#ifdef _USE_DYNAMIC_ENCODE
	if (fnEncodeDamagePacket)
	{
		fnEncodeDamagePacket(&smTransCommand);									//ÆÐÅ¶ ¾ÏÈ£È­

		lpsmSock = GetAreaServerSock();
		if (lpsmSock)
			lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
}
#else
	dm_EncodePacket(&smTransCommand);										//ÆÐÅ¶ ¾ÏÈ£È­

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
#endif

	return TRUE;


}

int RecvPartySkillFromServer(TRANS_PARTY_SKILL * lpTransPartySkill)
{
	int cnt, cnt2, cnt3;
	int time;
	int flag;

	switch (lpTransPartySkill->dwSkillCode) {
	case SKILL_PLAY_HOLY_VALOR:
		if (lpTransPartySkill->Point < 1 || lpTransPartySkill->Point>10) break;

		time = Holy_Valor_Time[lpTransPartySkill->Point - 1];
		for (cnt = 1; cnt < lpTransPartySkill->PartyCount; cnt++) {
			if (lpCurPlayer->dwObjectSerial == lpTransPartySkill->dwPartyUser[cnt]) {

				flag = 0;

				/*
				for (cnt3 = 0; cnt3 < 10; cnt3++) {
					if (ContinueSkill[cnt].Flag) {
						if (ContinueSkill[cnt].CODE == SKILL_DRASTIC_SPIRIT || ContinueSkill[cnt].CODE == SKILL_HOLY_BODY) {
							flag++;
						}
					}
				} */

				if (flag) {
					SendCancelSkillToServer(lpTransPartySkill->dwSkillCode, 0, 0, 0);
				}
				else {
					//½ºÅ³ ÀÌÆåÆ® Àû¿ë
					AssaParticle_HolyValor_Member(lpCurPlayer, time);
					sSKILL sSkill;

					ZeroMemory(&sSkill, sizeof(sSKILL));
					sSkill.CODE = SKILL_HOLY_VALOR;
					sSkill.UseTime = time;
					sSkill.Flag = TRUE;
					sSkill.Point = lpTransPartySkill->Point;
					sinContinueSkillSet(&sSkill);
				}
			}
			else {
				for (cnt2 = 0; cnt2 < OTHER_PLAYER_MAX; cnt2++) {
					if (chrOtherPlayer[cnt2].Flag &&
						chrOtherPlayer[cnt2].dwObjectSerial == lpTransPartySkill->dwPartyUser[cnt]) {
						AssaParticle_HolyValor_Member(&chrOtherPlayer[cnt2], time);
					}
				}
			}
		}
		break;

		/*
				case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
					//Æ®¸®¾öÇÁ¹ßÇÒ¶ó
					if ( lpTransPartySkill->Point<1 || lpTransPartySkill->Point>10 ) break;

					time = T_Of_Valhalla_Time[lpTransPartySkill->Point-1];

					lpChar = FindChrPlayer( lpTransPartySkill->dwPartyUser[0] );		//½ÃÀüÀÚ

					if ( lpChar ) {
						if ( lpChar!=lpCurPlayer ) {
							AssaParticle_TriumphOfValhallaMember( lpChar , lpCurPlayer , time );
						}

						for( cnt=1;cnt<lpTransPartySkill->PartyCount;cnt++ ) {
							for( cnt2=0;cnt2<OTHER_PLAYER_MAX;cnt2++) {
								if ( chrOtherPlayer[cnt2].Flag &&
									chrOtherPlayer[cnt2].dwObjectSerial==lpTransPartySkill->dwPartyUser[cnt] ) {

										if ( lpChar!=&chrOtherPlayer[cnt2] ) {
											AssaParticle_TriumphOfValhallaMember( lpChar , &chrOtherPlayer[cnt2] , time );
										}
									}
							}
						}

					}
					break;
		*/

	case SKILL_PLAY_EXTINCTION:
		//¼º°ø ÀÌÆåÆ® ¹ÞÀ½ ( ¸ó½ºÅÍ²¨ )
		for (cnt = 0; cnt < lpTransPartySkill->PartyCount; cnt++) {
			for (cnt2 = 0; cnt2 < OTHER_PLAYER_MAX; cnt2++) {
				if (chrOtherPlayer[cnt2].Flag &&
					chrOtherPlayer[cnt2].dwObjectSerial == lpTransPartySkill->dwPartyUser[cnt]) {
					AssaParticle_Extinction(&chrOtherPlayer[cnt2]);
					SkillPlaySound(SKILL_SOUND_SKILL_EXTINTION, chrOtherPlayer[cnt2].pX, chrOtherPlayer[cnt2].pY, chrOtherPlayer[cnt2].pZ);
				}
			}
		}
		break;

	}


	return TRUE;
}

DWORD	dwLastClanUserCode = 0;

//Å¬·£ ¸Þ¼¼Áö º¸³»±â
int SendClanJoinService(DWORD dwMsgCode, smCHAR * lpChar)
{
	smTRANS_CHAR_COMMAND2	smTransCharCommand;

#ifdef  __CLANSUBCHIP__
	if (cldata.myPosition != 101 && cldata.myPosition != 104) return FALSE;
#else
	if (cldata.myPosition != 101) return FALSE;
#endif

	smTransCharCommand.code = smTRANSCODE_CLAN_SERVICE;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND2);
	smTransCharCommand.WParam = dwMsgCode;
	smTransCharCommand.LParam = lpChar->dwObjectSerial;
	smTransCharCommand.SParam = lpCurPlayer->dwObjectSerial;
	lstrcpy(smTransCharCommand.szName, cldata.name);
	lstrcpy(smTransCharCommand.szId, UserAccount);

	if (smWsockUserServer)
		smWsockUserServer->Send2((char*)&smTransCharCommand, smTransCharCommand.size, TRUE);

	return TRUE;
}

//Å¬·£ °¡ÀÔ ½ÂÀÎ ¸Þ¼¼Áö º¸³»±â
int SendJoinClan()
{
	smTRANS_CHAR_COMMAND2	smTransCharCommand;

	if (!dwLastClanUserCode) return FALSE;

	smTransCharCommand.code = smTRANSCODE_CLAN_SERVICE;
	smTransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND2);
	smTransCharCommand.WParam = 2;
	smTransCharCommand.LParam = dwLastClanUserCode;
	smTransCharCommand.SParam = lpCurPlayer->dwObjectSerial;
	lstrcpy(smTransCharCommand.szName, lpCurPlayer->smCharInfo.szName);
	lstrcpy(smTransCharCommand.szId, UserAccount);

	if (smWsockUserServer)
		smWsockUserServer->Send2((char*)&smTransCharCommand, smTransCharCommand.size, TRUE);

	return TRUE;
}

//Å¬·£ ¸Þ¼¼Áö ¹Þ±â
int RecvClanJoinService(smTRANS_CHAR_COMMAND2 * lpTransCharCommand)
{
	smCHAR* lpChar;

	switch (lpTransCharCommand->WParam) {
	case 1:		//°¡ÀÔ ±ÇÀ¯
		if (cldata.myPosition == 102 || cldata.myPosition == 103) {

			lpChar = FindAutoPlayer(lpTransCharCommand->SParam);
			if (lpChar) {
				ClanJoin(lpTransCharCommand->szName, lpChar->smCharInfo.szName);
				OpenEachPlayer(lpChar);
				DispEachMode = TRUE;
				dwLastClanUserCode = lpTransCharCommand->SParam;
			}
		}
		break;

	case 2:
#ifdef  __CLANSUBCHIP__
		if (cldata.myPosition == 101 || cldata.myPosition == 104) {
#else
		if (cldata.myPosition == 101) {
#endif
			lpChar = FindAutoPlayer(lpTransCharCommand->SParam);
			if (lpChar)
				menu_joinOpen_Chip(1, lpTransCharCommand->szId, lpTransCharCommand->szName, lpChar->smCharInfo.JOB_CODE, lpChar->smCharInfo.Level);
			else
				menu_joinOpen_Chip(1, lpTransCharCommand->szId, lpTransCharCommand->szName, 0, 0);
		}
		break;
	}

	return TRUE;
}


/////////// DivineLightning ½ºÅ³ ¼­ºê ÇÔ¼ö ///////////////////

DWORD	dwSkill_DivineLightning_Target[8];
int		DivineLightning_Target_Count = 0;
int		DivineLightning_FindCount = 0;

//µð¹ÙÀÎ ¶óÀÌÆ®´× ½ºÅ³ ¿¬Ãâ ( ÁØºñÇÔ¼ö - ÁÖÀÎ°ø¸¸ )
int	SkillPlay_DivineLightning_Select(smCHAR * lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	smWINSOCK* lpsmSendSock = 0;

	//Å¬¶óÀÌ¾ðÆ® ·Î±× ÆÄÀÏ ±â·Ï
	Record_ClinetLogFile("±¸¹öÀü °ø°ÝÆÐÅ¶ - SkillPlay_DivineLightning_Select ");
	quit = 1;
	return TRUE;

	if (lpChar != lpCurPlayer) return FALSE;

	TargetCount = 0;

	LightningCount = Divine_Lightning_Num[SkillPoint - 1];

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {

		cCount = (DivineLightning_FindCount + cnt) & OTHER_PLAYER_MASK;

		if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
			lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
			chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
			chrOtherPlayer[cCount].smCharInfo.Life[0] > 0) {

			x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
			y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
			z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

			dist = x * x + y * y + z * z;

			if (dist < (160 * 160)) {
				dwSkill_DivineLightning_Target[TargetCount] = chrOtherPlayer[cCount].dwObjectSerial;
				TransSkilAttackData.dwTarObjectSerial[TargetCount] = chrOtherPlayer[cCount].dwObjectSerial;
				TargetCount++;

				if (chrOtherPlayer[cCount].smCharInfo.Life[1] && chrOtherPlayer[cCount].smCharInfo.Life[0] == chrOtherPlayer[cCount].smCharInfo.Life[1]) {
					chrOtherPlayer[cCount].EnableStateBar = TRUE;
				}
				if (TargetCount >= LightningCount) {
					DivineLightning_FindCount = cCount;
					break;
				}
			}
		}
	}

	TransSkilAttackData.TargetCount = TargetCount;
	DivineLightning_Target_Count = TargetCount;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD) * SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD) * TargetCount + 16;

	TransSkilAttackData.x = lpChar->pX;
	TransSkilAttackData.y = lpChar->pY;
	TransSkilAttackData.z = lpChar->pZ;

	TransSkilAttackData.AttackState = 103;			//¾ðµ¥µå °¡Áß
	TransSkilAttackData.AttackSize = 160 * 160;
	TransSkilAttackData.Power = Divine_Lightning_Damage[SkillPoint - 1];

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â

	if (lpsmSendSock && TargetCount > 0) {
		//º¸³½ °ø°Ý·Â ±â·Ï
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send2((char*)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	return TRUE;
}

//µð¹ÙÀÎ ¶óÀÌÆ®´× ½ºÅ³ ¿¬Ãâ ( ÀÌÆåÆ® ½ÇÇö )
int SkillPlay_DivineLightning_Effect(smCHAR * lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	smCHAR* lpTarChar;

	TargetCount = 0;

	if (lpChar == lpCurPlayer) {
		for (cnt = 0; cnt < DivineLightning_Target_Count; cnt++) {
			lpTarChar = FindAutoPlayer(dwSkill_DivineLightning_Target[cnt]);
			if (lpTarChar) {
				AssaParticle_DivineLighting(lpTarChar);
				TargetCount++;
			}
		}
	}
	else {
		LightningCount = Divine_Lightning_Num[SkillPoint - 1];

		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
			cCount = (DivineLightning_FindCount + cnt) & OTHER_PLAYER_MASK;
			if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0] > 0) {

				x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

				dist = x * x + y * y + z * z;

				if (dist < (160 * 160)) {
					AssaParticle_DivineLighting(&chrOtherPlayer[cCount]);
					TargetCount++;
					if (TargetCount >= LightningCount) {
						DivineLightning_FindCount = cCount;
						break;
					}
				}
			}
		}

		if (TargetCount < LightningCount && lpChar->smCharInfo.State == smCHAR_STATE_ENEMY) {
			x = (lpChar->pX - lpCurPlayer->pX) >> FLOATNS;
			y = (lpChar->pY - lpCurPlayer->pY) >> FLOATNS;
			z = (lpChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

			dist = x * x + y * y + z * z;

			if (dist < (160 * 160)) {
				AssaParticle_DivineLighting(lpCurPlayer);
				TargetCount++;
			}
		}

	}


	return TargetCount;
}


//µð¹ÙÀÎ ¶óÀÌÆ®´× ½ºÅ³ ¿¬Ãâ ( ¸Ó¹Ì·Îµå )
int SkillPlay_MummyLord_Effect(smCHAR * lpChar, int Range)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	dRange = Range * Range;

	TargetCount = 0;

	if (lpChar->smCharInfo.Brood != smCHAR_MONSTER_USER) {
		//¸ó½ºÅÍ°¡ À¯Àú °ø°Ý
		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
			if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER &&
				chrOtherPlayer[cnt].smCharInfo.Life[0] > 0)
			{

				x = (lpChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x * x + y * y + z * z;

				if (dist < dRange) {
					ParkAssaParticle_DivineLighting(&chrOtherPlayer[cnt]);
					TargetCount++;
				}

			}

		}

		if (lpChar->smCharInfo.State == smCHAR_STATE_ENEMY) {
			x = (lpChar->pX - lpCurPlayer->pX) >> FLOATNS;
			y = (lpChar->pY - lpCurPlayer->pY) >> FLOATNS;
			z = (lpChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

			dist = x * x + y * y + z * z;

			if (dist < dRange) {
				ParkAssaParticle_DivineLighting(lpCurPlayer);
				TargetCount++;
			}
		}
	}
	else {
		//À¯Àú¶û °°Àº Æí ( ¸ó½ºÅÍ °ø°Ý )
		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
			if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cnt].smCharInfo.Life[0] > 0)
			{

				x = (lpChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x * x + y * y + z * z;

				if (dist < dRange) {
					ParkAssaParticle_DivineLighting(&chrOtherPlayer[cnt]);
					TargetCount++;
				}

			}
		}
	}


	return TargetCount;
}


//º£³ð ½ºÇÇ¾î ½ºÅ³ ¿¬Ãâ ( ÀÌÆåÆ® ½ÇÇö )
int SkillPlay_VenomSpear_Effect(smCHAR * lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	smCHAR* lpTarChar;
	int range;

	range = VenomSpear_Range[SkillPoint - 1];
	range *= range;

	TargetCount = 0;

	if (lpChar == lpCurPlayer) {
		for (cnt = 0; cnt < DivineLightning_Target_Count; cnt++) {
			lpTarChar = FindAutoPlayer(dwSkill_DivineLightning_Target[cnt]);
			if (lpTarChar) {
				AssaParticle_VeonmSpear(lpTarChar);
				TargetCount++;
			}
		}
	}
	else {
		LightningCount = Divine_Lightning_Num[SkillPoint - 1];

		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
			cCount = (DivineLightning_FindCount + cnt) & OTHER_PLAYER_MASK;
			if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
				lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
				chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
				chrOtherPlayer[cCount].smCharInfo.Life[0] > 0) {

				x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
				y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
				z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

				dist = x * x + y * y + z * z;

				if (dist < range) {
					AssaParticle_VeonmSpear(&chrOtherPlayer[cCount]);
					TargetCount++;
					if (TargetCount >= LightningCount) {
						DivineLightning_FindCount = cCount;
						break;
					}
				}
			}
		}
	}


	return TargetCount;
}

//¸ó½ºÅÍ ½ºÅ³ ÀÌÆåÆ® ½ÇÇö ( ÀÌÆåÆ® ½ÇÇö )
int SkillPlay_Monster_Effect(smCHAR * lpChar, int EffectKind, int Range)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int dDist = Range * Range;


	TargetCount = 0;

	x = (lpChar->pX - lpCurPlayer->pX) >> FLOATNS;
	y = (lpChar->pY - lpCurPlayer->pY) >> FLOATNS;
	z = (lpChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

	dist = x * x + y * y + z * z;

	if (dist < dDist) {
		ParkAssaParticle_ChaosKaraSkill_User(lpCurPlayer);	// Ä«¿À½ºÄ«¶ó ¹ìÆÄÀÌ¾î¸¯ (À¯Àú)
		TargetCount++;
	}


	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
			lpChar != &chrOtherPlayer[cnt] && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER &&
			chrOtherPlayer[cnt].smCharInfo.Life[0] > 0)
		{
			x = (lpChar->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
			y = (lpChar->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
			z = (lpChar->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

			dist = x * x + y * y + z * z;

			if (dist < dDist) {
				ParkAssaParticle_ChaosKaraSkill_User(&chrOtherPlayer[cnt]);	// Ä«¿À½ºÄ«¶ó ¹ìÆÄÀÌ¾î¸¯ (À¯Àú)
				TargetCount++;
			}
		}
	}

	return TargetCount;
}




//½ºÅ³ ½ÇÇà ÆÐÅ¶ ¼ö½Å
int RecvProcessSkill(smTRANS_COMMAND * lpTransCommand)
{
	int cnt;
	int dwSkillCode, point;
	smCHAR* lpChar, * lpChar2;
	POINT3D	Pos1, Pos2;

	dwSkillCode = lpTransCommand->WParam & 0xFF;
	point = (lpTransCommand->WParam >> 8) & 0xF;

	switch (dwSkillCode) {
	case SKILL_PLAY_HOLY_REFLECTION:
		//È¦¸® ¸®ÇÃ·º¼Ç Å¸°Ý ÀÌÆåÆ®
		sinSkillEffect_Holy_Reflection_Defense(lpCurPlayer);
		break;

	case SKILL_PLAY_ENCHANT_WEAPON:
		//ÀÎÃ¾Æ® ¿þÆù
		cnt = Enchant_Weapon_Time[point - 1];				//lpTransCommand->LParam-1];
		lpCurPlayer->EnchantEffect_Point = point;		//lpTransCommand->LParam;

		CSKILL->SetEnchant_Weapon(cnt, point);

		switch (lpTransCommand->EParam) {
		case 0:
			AssaParticle_EnchantWeaponIceJang(lpCurPlayer, cnt);
			lpCurPlayer->WeaponEffect = sITEMINFO_ICE + 1;
			lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt * 1000;
			break;
		case 1:
			AssaParticle_EnchantWeaponLightJang(lpCurPlayer, cnt);
			lpCurPlayer->WeaponEffect = sITEMINFO_LIGHTING + 1;
			lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt * 1000;
			break;
		case 2:
			AssaParticle_EnchantWeaponFireJang(lpCurPlayer, cnt);
			lpCurPlayer->WeaponEffect = sITEMINFO_FIRE + 1;
			lpCurPlayer->dwWeaponEffectTime = dwPlayTime + cnt * 1000;
			break;
		}
		break;

	case SKILL_PLAY_RESURRECTION:
		//Á×ÀºÀÚ »ì¸°±â
		if (lpCurPlayer->MotionInfo && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) {
			SendProcessSKillToServer(SKILL_PLAY_RESURRECTION, point, 0, 0);

			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_RESTART);		//½ÃÀÛ¸ð¼Ç
			StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_GAME_START1);
			SetDynLight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 100, 100, 100, 0, 200);
			CharPlaySound(lpCurPlayer);

			sinSetLife(lpCurPlayer->smCharInfo.Life[1] / 2);
			ResetEnergyGraph(4);								//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			lpCurPlayer->MoveFlag = FALSE;
			RestartPlayCount = 350;								//5ÃÊ µ¿¾È ¹«Àû
			ReStartFlag = 0;
		}
		break;

	case SKILL_PLAY_VIRTUAL_LIFE:
		//¹öÃò¾ó ¶óÀÌÇÁ Àû¿ë
		CSKILL->SetVirtualLife(Virtual_Life_Time[point - 1], point);
		AssaParticle_VirtualLife(lpCurPlayer, Virtual_Life_Time[point - 1]);
		break;

	case SKILL_PLAY_LOVELY_LIFE:
		//»ç¶ûÀÇ ÃÊÄÝ·¿
		if (lpTransCommand->LParam == lpCurPlayer->dwObjectSerial) {
			CSKILL->SetVirtualLife(Virtual_Life_Time[point - 1], point);
			AssaParticle_VirtualLife(lpCurPlayer, Virtual_Life_Time[point - 1]);
			SkillPlaySound(SKILL_SOUND_SKILL_VIRTUAL_LIFE, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
		}
		else {
			lpChar = FindChrPlayer(lpTransCommand->LParam);
			if (lpChar) {
				AssaParticle_VirtualLife(lpChar, Virtual_Life_Time[point - 1]);
				SkillPlaySound(SKILL_SOUND_SKILL_VIRTUAL_LIFE, lpChar->pX, lpChar->pY, lpChar->pZ);
			}
		}
		break;

	case SKILL_PLAY_VANISH:
		//¹è´Ï½¬ ½ºÅ³ ÇØÁ¦
		lpCurPlayer->PlayVanish = 0;
		CSKILL->CancelContinueSkill(SKILL_VANISH);

		if (dwM_BlurTime) dwM_BlurTime = dwPlayTime;
		break;

	case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
		//Æ®¶óÀÌ¾öÇÁ
		CSKILL->SetT_Of_Valhalla(point, lpTransCommand->EParam);
		AssaParticle_TriumphOfValhalla(lpCurPlayer, T_Of_Valhalla_Time[point - 1]);
		break;

	case SKILL_PLAY_SPARK_SHIELD:
		//½ºÆÄÅ©½¯µå ÀÌÆåÆ®

		// À¯Àú¹øÈ£·Î Ã£´Â´Ù
		lpChar = FindChrPlayer(lpTransCommand->LParam);
		lpChar2 = FindChrPlayer(lpTransCommand->SParam);
		if (lpChar && lpChar2) {
			//½ºÆÄÅ© ½¯µå
			if (lpChar->HvLeftHand.PatTool) {
				if (lpChar->GetToolBipPoint(&lpChar->HvLeftHand, &Pos1.x, &Pos1.y, &Pos1.z)) {

					Pos2.x = lpChar2->pX;
					Pos2.y = lpChar2->pY + 24 * fONE;
					Pos2.z = lpChar2->pZ;

					AssaParticle_SparkShieldDefence(&Pos1, &Pos2);
					SkillPlaySound(SKILL_SOUND_SKILL_SPARK2, lpChar->pX, lpChar->pY, lpChar->pZ);
				}
			}
		}
		break;

	case SKILL_PLAY_HALL_OF_VALHALLA:
		//È¦¿Àºê¹ßÇÒ¶ó
		CSKILL->HellOfValhalla(Hall_Of_Valhalla_Time[point - 1], point, lpTransCommand->SParam, lpTransCommand->EParam, 1);
		SkillValhallaHallOfValhallaHandEffect(lpCurPlayer, (float)lpTransCommand->EParam);
		lpCurPlayer->dwHallOfValhallaTime = dwPlayTime + Hall_Of_Valhalla_Time[point - 1] * 1000;	//È¦¿Àºê¹ßÇÒ¶ó À¯Áö½Ã°£
		break;

	case SKILL_PLAY_FORCE_OF_NATURE:
		//Æ÷½º¿Àºê ³×ÀÌÃÄ
		CSKILL->ForeceOfNature(Force_Of_Nature_Time[point - 1], point, 1);
		SkillSagittarionForceOfNature1(lpCurPlayer, (float)Force_Of_Nature_Time[point - 1]);
		lpCurPlayer->dwForceOfNatureTime = dwPlayTime + Force_Of_Nature_Time[point - 1] * 1000;	//Æ÷½º¿Àºê ³×ÀÌÃÄ À¯Áö½Ã°£
		break;


	case SKILL_PLAY_SOD_ITEM:
		//SOD ±ôÂ¦ ¾ÆÀÌÅÛ ÀÌÆåÆ®

		switch (lpTransCommand->SParam) {
		case 1:	//ÆøÅº
			SetBellatraFontEffect(E_BL_QUAKE_SEAL);
			SetBellatraFontEffect(E_BL_FONT_QUAKE_SEAL);
			EffectWaveCamera(50, 3);
			SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
			break;

		case 2:	//½Ã°£
			SetBellatraFontEffect(E_BL_STUN_SEAL);
			SetBellatraFontEffect(E_BL_FONT_STUN_SEAL);
			EffectWaveCamera(50, 3);
			SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
			break;

		case 3:	//¾óÀ½
			SetBellatraFontEffect(E_BL_FREEZE_SEAL);
			SetBellatraFontEffect(E_BL_FONT_FREEZE_SEAL);
			EffectWaveCamera(50, 3);
			SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
			break;

		case 4:	//Åä³¢
			SetBellatraFontEffect(E_BL_RABBIE_SEAL);
			SetBellatraFontEffect(E_BL_FONT_RABBIE_SEAL);
			EffectWaveCamera(50, 3);
			SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
			break;

		case 5:	//´Þ¼öÁ¤
			SetBellatraFontEffect(E_BL_STYGIAN_SEAL);
			SetBellatraFontEffect(E_BL_FONT_STYGIAN_SEAL);
			break;

		case 6:	//ÅÂ¾ç¼öÁ¤
			SetBellatraFontEffect(E_BL_GUARDIAN_SEAL);
			SetBellatraFontEffect(E_BL_FONT_GUARDIAN_SEAL);
			break;

		case 7:	//Ãµ°ø¼öÁ¤
			SetBellatraFontEffect(E_BL_POINT_SEAL);
			SetBellatraFontEffect(E_BL_FONT_POINT_SEAL);
			break;
		}
		esPlaySound(19, 360);		//SOD ¾ÆÀÌÅÛ ¼Ò¸®
		break;

	case SKILL_PLAY_HEALING:
		//¸ÅÁ÷ ¸¶½ºÅÍ Èú¸µ»ç¿ë
		lpChar = FindChrPlayer(lpTransCommand->LParam);
		if (lpChar) {
			sinEffect_Healing2(lpChar);		//Èú¸µ ÀÌÆåÆ®
			SkillPlaySound(SKILL_SOUND_SKILL_HEALING, lpChar->pX, lpChar->pY, lpChar->pZ);		//Èú¸µ »ç¿îµå
		}
		break;

	case SKILL_PLAY_BLESS_SIEGE_ITEM:
		//ºí·¡½º Ä³½½ °ø¼º ¾ÆÀÌÅÛ »ç¿ë

		switch (lpTransCommand->SParam) {

			////////////// ºô¸µ¿ë /////////////////
		case (sinBI1 | sin05):	//¹«Àû½ºÅ©·Ñ
			lpCurPlayer->PlayInvincible = 70 * 30;		//30ÃÊ
			break;
		case (sinBI1 | sin06):	//Å©¸®Æ¼ÄÃ ½ºÅ©·Ñ
			break;
		case (sinBI1 | sin07):	//Ãß°¡ È¸ÇÇ ½ºÅ©·Ñ
			break;

			//////////////// °ø¼º¿ë //////////////////

		case (sinBC1 | sin01):	//¹«Àû½ºÅ©·Ñ
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_INVU);
			lpCurPlayer->PlayInvincible = 70 * 30;		//30ÃÊ
			break;
		case (sinBC1 | sin02):	//Å©¸®Æ¼ÄÃ ½ºÅ©·Ñ
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_CRITICAL);
			break;
		case (sinBC1 | sin03):	//Ãß°¡ È¸ÇÇ ½ºÅ©·Ñ
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_S_EVASION);
			break;
			/*
				case (sinBC1|sin04):	//ÆÄÆ¼¿ø Àü¿ø È¸º¹
				case (sinBC1|sin05):	//ºÎÈ° ½ºÅ©·Ñ
					return FALSE;
			*/
		case (sinBC1 | sin06):	//ÆÄÀÌ¾î Å©¸®½ºÅ» ÀúÇ×¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_FIRE_C);
			break;
		case (sinBC1 | sin07):	//¾ÆÀÌ½º Å©¸®½ºÅ» ÀúÇ×¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_ICE_C);
			break;
		case (sinBC1 | sin08):	//¶óÀÌÆ®´× Å©¸®½ºÅ» ÀúÇ×¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_R_LIGHTING_C);
			break;
		case (sinBC1 | sin09):	//´ë ÆÄÀÌÅÍ °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_FIGHTER);
			break;
		case (sinBC1 | sin10):	//´ë ¸ÞÄ«´Ï¼Ç °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_MECHANICIAN);
			break;
		case (sinBC1 | sin11):	//´ëÆÄÀÌÅ©¸Ç °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_PIKEMAN);
			break;
		case (sinBC1 | sin12):	//´ë ÀÌÃÄ °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_ARCHER);
			break;
		case (sinBC1 | sin13):	//´ë ³ªÀÌÆ® °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_KNIGHT);
			break;
		case (sinBC1 | sin14):	//´ë ¾ÆÅ»¶õÅ¸ °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_ATALANTA);
			break;
		case (sinBC1 | sin15):	//´ë ¸ÞÁö¼Ç °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_MAGICIAN);
			break;
		case (sinBC1 | sin16):	//´ë ÇÁ¸®½ºÆ¼½º °ø°Ý·Â °­È­¼®
			//chaSiege.SetCastleItemSkill( SIN_CASTLEITEMSKILL_A_PRIESTESS);
			break;
		}
		break;

	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////

//int ReadClanInfo(DWORD dwClanNum)
//CLANINFO ClanInfo[CLAN_MAX]; //Å¬·£ Á¤º¸ ÀúÀå
//
//cldata ¾øµ¥ÀÌÆ®¿ë
//void Updatecldata();

//Å¬·£ ¾÷µ¥ÀÌÆ® ¿ä±¸ ¸Þ¼¼Áö
//#define	smTRANSCODE_CLAN_UPDATE			0x50320C10


//Å¬·£¿ø Á¤º¸ Àü¼Û¿ë ÇÔ¼ö
int SendClanCommandUser(smWINSOCK * lpsmSock, DWORD	dwCode, int Param1, int Param2, CLANWONLIST * lpClanUserList, int ClanUserCount)
{

	TRANS_CLAN_COMMAND_USER	TransClanUser;
	int	cnt, len;
	char* lpBuff;
	DWORD	dwSpeedSum;
	int	BuffSize;

	TransClanUser.code = dwCode;

	TransClanUser.Param[0] = Param1;
	TransClanUser.Param[1] = Param2;
	TransClanUser.Param[2] = 0;
	TransClanUser.Param[3] = 0;
	TransClanUser.UserCount = ClanUserCount;

	lpBuff = TransClanUser.szUserBuff;

	for (cnt = 0; cnt < ClanUserCount; cnt++) {
		dwSpeedSum = GetSpeedSum(lpClanUserList->clanWon[cnt]);
		((DWORD*)lpBuff)[0] = dwSpeedSum;
		lpBuff += sizeof(DWORD);
		lstrcpy(lpBuff, lpClanUserList->clanWon[cnt]);
		len = lstrlen(lpClanUserList->clanWon[cnt]);
		lpBuff += len + 1;
	}

	BuffSize = lpBuff - TransClanUser.szUserBuff;
	BuffSize += 16;

	TransClanUser.size = 32 + BuffSize;

	if (lpsmSock && ClanUserCount > 0) {
		lpsmSock->Send2((char*)&TransClanUser, TransClanUser.size, TRUE);
	}

	return TRUE;
}

//Å¬·£¿ø Á¤º¸ ÇØµ¶
int	RecvClanCommand(TRANS_CLAN_COMMAND_USER * lpTransClanUser, _CLAN_USER_INFO * ClanUserInfo)// char **lpClanUserName , DWORD *dwUserSpeedSum )
{
	int cnt;
	int cnt2;
	char* lpBuff;
	char	ch;

	lpBuff = lpTransClanUser->szUserBuff;

	if (lpTransClanUser->UserCount >= CLAN_USER_MAX) return FALSE;

	for (cnt = 0; cnt < lpTransClanUser->UserCount; cnt++) {
		ClanUserInfo[cnt].dwSpeedSum = ((DWORD*)lpBuff)[0];
		lpBuff += sizeof(DWORD);
		for (cnt2 = 0; cnt2 < 32; cnt2++) {
			ch = *lpBuff;
			ClanUserInfo[cnt].szName[cnt2] = ch;
			lpBuff++;
			if (!ch) break;
		}
		ClanUserInfo[cnt].szName[31] = 0;
	}

	return TRUE;
}

//Äù½ºÆ® ¾ÆÀÌÅÛ ÀÔ¼ö ÇÊÅÍ¸µ
int FiltQuestItem(TRANS_ITEMINFO * lpTransItemInfo, DWORD dwPacketCode)
{
	int cnt;

	if (chaQuest.sHaQuestElementary[6].CODE != HAQUEST_CODE_ELEMENTARY_G) {

		cnt = cInvenTory.SearchItemCode(sinQT1 | sin16);
		if (cnt >= 1) { DeleteQuestItem(sinQT1 | sin16); }
		if (lpTransItemInfo->Item.CODE == (sinQT1 | sin16)) { return FALSE; }		//¼º±Ù¼öÁ¤

	}

	switch (sinQuest_ChangeJob2.CODE) {
	case SIN_QUEST_CODE_CHANGEJOB2_NPC_M:
		//¹ú²Ü ¾ÆÀÌÅÛ (ÃÖ´ë °¹¼ö È®ÀÎ )
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin01)) {
			if (!cInvenTory.SearchItemCode(sinMA1 | sin01)) return FALSE;		//À¯¸®º´ ¾ø´Â°æ¿ì
			if (cInvenTory.SearchItemCode(sinQT1 | sin04)) return FALSE;		//·Î¾âÁ¦¸® ÀÖ´Â °æ¿ì
			cnt = cInvenTory.SearchItemCode(sinMA2 | sin01);
			if (cnt >= 9) return FALSE;
		}
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin02)) return FALSE;
		return TRUE;

	case SIN_QUEST_CODE_CHANGEJOB2_NPC_D:
		//±â¸§ ¾ÆÀÌÅÛ (ÃÖ´ë °¹¼ö È®ÀÎ )
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin02)) {
			if (!cInvenTory.SearchItemCode(sinMA1 | sin01)) return FALSE;		//À¯¸®º´ ¾ø´Â°æ¿ì
			if (cInvenTory.SearchItemCode(sinQT1 | sin05)) return FALSE;		//¹ß¸ðÁ¦ ÀÖ´Â °æ¿ì
			cnt = cInvenTory.SearchItemCode(sinMA2 | sin02);
			if (cnt >= 7) return FALSE;
		}
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin01)) return FALSE;
		return TRUE;
	}

	//3Â÷ ÀüÁ÷ Äù½ºÆ®
	//¹ìÇÁ
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin06)) {
		if (sinQuest_ChangeJob3.CODE == SIN_QUEST_CODE_CHANGEJOB3) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin06);
			if (cnt >= 3) return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	//·¹º§ Äù½ºÆ®¿ë ¹ìÇÁ 1
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin09)) {
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin09);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	//·¹º§ Äù½ºÆ®¿ë ¹ìÇÁ 2
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin10)) {
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin10);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	//·¹º§ Äù½ºÆ®¿ë ¹ìÇÁ 3
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin11)) {
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin11);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}


	//3Â÷ Àü¾÷ Äù½ºÆ®¿ë 
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin13)) {

		if (sinQuest_ChangeJob4.CODE == SIN_QUEST_CODE_CHANGEJOB4 && sinQuest_ChangeJob4.State == 5) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin13);
			if (cnt >= 1 || CheckAttMonsterCode(sinQuest_ChangeJob4.Kind) == FALSE)
				return FALSE;

			return TRUE;
		}
		return FALSE;
	}



	//¾öÇÁÀÇ ¸ÁÄ¡ Äù½ºÆ® 
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin16)) {

		if (chaQuest.sHaQuestElementary[6].CODE == HAQUEST_CODE_ELEMENTARY_G) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin16);

			if (cnt >= 1)
				return FALSE;

			return TRUE;
		}
		return FALSE;
	}


	if ((lpTransItemInfo->Item.CODE & sinITEM_MASK2) == sinMA1 ||
		(lpTransItemInfo->Item.CODE & sinITEM_MASK2) == sinMA2) {
		//Äù½ºÆ®°¡ ¾ø´Â°æ¿ì Äù½ºÆ® ¾ÆÀÌÅÛ µé¾î¿Â °æ¿ì

		return FALSE;
	}

	return TRUE;
}

POINT GetProcessModule();


int CheckProcessModule()
{
	POINT p;
	OSVERSIONINFO VersionInfo;
	BOOL Result;
	smTRANS_COMMAND	smTransCommand;


	return TRUE;

	VersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CLIENTINFO;

	Result = GetVersionEx(&VersionInfo);
	p = GetProcessModule();

	if (Result != FALSE) {
		smTransCommand.WParam = VersionInfo.dwPlatformId;
		smTransCommand.LParam = (VersionInfo.dwMajorVersion * 100) + VersionInfo.dwMinorVersion;
		smTransCommand.LParam = (smTransCommand.LParam << 16) | (VersionInfo.dwBuildNumber & 0xFFFF);
		smTransCommand.SParam = p.x;
		smTransCommand.EParam = p.y;

		smTransCommand.WParam ^= ((DWORD*)(UserAccount + 0))[0];
		smTransCommand.LParam ^= ((DWORD*)(UserAccount + 1))[0];
		smTransCommand.SParam ^= ((DWORD*)(UserAccount + 2))[0];
		smTransCommand.EParam ^= ((DWORD*)(UserAccount + 3))[0];

		if (smWsockDataServer)
			smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	return TRUE;
}

int SendFalconDamage(smCHAR * lpChar)
{
	DWORD point;
	int cnt;

	if (lpChar == lpCurPlayer && lpChar->chrAttackTarget) {
		point = ((SkillFalconPoint ^ ((DWORD)chrOtherPlayer ^ lpCurPlayer->dwObjectSerial)) + 1) << 8;
		point |= SKILL_PLAY_FALCON;
		cnt = dm_SendTransDamage(lpChar->chrAttackTarget, 0, 0, 0, 0, point, FALSE);
		DamageExp(lpChar->chrAttackTarget, cnt);
		return cnt;
	}
	return NULL;
}

int SendDancingSwordDamage(smCHAR * lpChar)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar) {

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = 1 << 8;
		point |= SKILL_PLAY_DANCING_SWORD;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		if ((rand() % 2) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK1, lpChar->pX, lpChar->pY, lpChar->pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK2, lpChar->pX, lpChar->pY, lpChar->pZ);

		return cnt;
	}
	return FALSE;
}


//¸Þ±×³×Æ½ ½ºÇÇ¾î µ¥¹ÌÁö º¸³»±â
int SendMegneticSphereDamage(smCHAR * lpChar)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar) {

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = 1 << 8;
		point |= SKILL_PLAY_MAGNETIC_SPHERE;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		if ((rand() % 2) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK1, lpChar->pX, lpChar->pY, lpChar->pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK2, lpChar->pX, lpChar->pY, lpChar->pZ);

		return cnt;
	}
	return FALSE;
}

//¸Ó½ºÆç µ¥¹ÌÁö º¸³»±â
int SendMuspellDamage(smCHAR * lpChar)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar) {

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = 1 << 8;
		point |= SKILL_PLAY_SUMMON_MUSPELL;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		if ((rand() % 2) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK1, lpChar->pX, lpChar->pY, lpChar->pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD_ATK2, lpChar->pX, lpChar->pY, lpChar->pZ);

		return cnt;
	}
	return FALSE;
}



//Àú·¦¿ë Æê µ¥¹ÌÁö
int SendLowLevelPetDamage(smCHAR * lpChar, int petType)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar) {

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = (1 + petType) << 8;
		point |= SKILL_PLAY_PET_ATTACK;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		return cnt;
	}
	return FALSE;
}


int SendPCBangPetDamage(smCHAR * lpChar, int petType)
{
	DWORD point;
	int cnt;
	int wpDamage[2];

	if (lpChar) {

		wpDamage[0] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0];
		wpDamage[1] = cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1];

		point = (1 + petType) << 8;
		point |= SKILL_PLAY_PET_ATTACK2;
		cnt = dm_SendTransDamage(lpChar, wpDamage[0], wpDamage[1], 0, 0, point, FALSE);
		DamageExp(lpChar, cnt);

		return cnt;
	}

	return FALSE;
}


int	SendItemExpress(DWORD	dwItemCode, char* szPassCode)
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory((char*)&smTransCommand, sizeof(smTRANS_COMMAND));
	smTransCommand.code = smTRANSCODE_ITEM_EXPRESS;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwItemCode;

	if (szPassCode)
		smTransCommand.LParam = GetSpeedSum(szPassCode);

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}

//¾ÆÀÌÅÛ ÅÃ¹è ¼­ºñ½º ¼ö½Å ¿ä±¸
int	SendItemExpress(DWORD	dwItemCode)
{
	return	SendItemExpress(dwItemCode, 0);
}


//¾ßÈ£ ¸ð¼Ç ¼­¹ö·Î º¸³»±â
int SendClanYahooMotion()
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK* lpsmSock;

	if (lpCurPlayer->smCharInfo.ClassClan) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_YAHOO_MOTION;
		smTransCommand.WParam = dwPlayTime;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		if (lpsmSock)
			lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

		return TRUE;

	}
	return FALSE;
}

//º° Æ÷ÀÎÆ® Á¤º¸¸¦ ¼­¹ö¿¡ º¸³½´Ù
int	SendStarPointToServer(int Price, int Cash)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_STARPOINT;
	smTransCommand.WParam = Price;
	smTransCommand.LParam = Cash;
	smTransCommand.SParam = 0;
	smTransCommand.EParam = 0;

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}

//±âºÎÇÑµ·À» ¼­¹ö¿¡ º¸³½´Ù
int	SendGiveMoneyToServer(int Money)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_GIVEMONEY;
	smTransCommand.WParam = Money;
	smTransCommand.LParam = 0;
	smTransCommand.SParam = 0;
	smTransCommand.EParam = 0;

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}

//Å¬·£¸Ó´Ï Á¤º¸¸¦ ¼­¹ö¿¡ º¸³½´Ù
int	SendClanMoneyToServer(int Money, int Flag, int Kind)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CLANMONEY;
	smTransCommand.WParam = Money;
	smTransCommand.LParam = Flag;
	smTransCommand.SParam = Kind;
	smTransCommand.EParam = 0;

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}

int	SendClanMoneyToServer(int Money, int Flag)
{
	return	SendClanMoneyToServer(Money, Flag, 0);
}

//ÀÌ¿ë¿ä±Ý Á¤º¸¸¦ ¼­¹ö¿¡ º¸³½´Ù
int	SendPaymentMoneyToServer(int Money, int Flag)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_PAYMENT_MONEY;
	smTransCommand.WParam = Flag;
	smTransCommand.LParam = Money;
	smTransCommand.SParam = 0;
	smTransCommand.EParam = 0;

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}



/*
//°³ÀÎ»óÁ¡ ¿ÀÇÂ
#define smTRANSCODE_OPEN_PERTRADE	0x48478A90
//°³ÀÎ»óÁ¡ ºÎ¸£±â
#define smTRANSCODE_CALL_PERTRADE	0x48478AA0
//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ
#define	smTRANSCODE_MYSHOP_ITEM		0x48478AB0

int	Send_ShowMyShopItem( smCHAR *lpChar );							//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë ¿ä±¸
int	Send_MyShopItemList( smTRANS_COMMAND	*lpTransCommand );		//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë »ó´ë¹æ¿¡ º¸³»±â
int	Recv_MyShopItemList( TRANS_TRADEITEMS *lpTransTradeItems );		//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë ¹ÞÀ½
*/


//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë ¿ä±¸
int	Send_ShowMyShopItem(smCHAR * lpChar)
{
	smTRANS_COMMAND	smTransCommand;
	int	result;
	smWINSOCK* lpsmSock;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CALL_MYSHOP;
	smTransCommand.WParam = 0;
	smTransCommand.LParam = lpChar->dwObjectSerial;
	smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smTransCommand.EParam = 0;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return result;
}

//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë »ó´ë¹æ¿¡ º¸³»±â
int	Send_MyShopItemList(smTRANS_COMMAND * lpTransCommand)
{
	int	result;
	smWINSOCK* lpsmSock;

	TransTradeItems_MyShop.dwRecver = lpTransCommand->SParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char*)&TransTradeItems_MyShop, TransTradeItems_MyShop.size, TRUE);

	return result;
}


//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë ¹ÞÀ½
int	Recv_MyShopItemList(TRANS_TRADEITEMS * lpTransTradeItems)
{
	sMYSHOP MyShopItem;
	smCHAR* lpChar;

	//¾ÐÃà µ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE*)&MyShopItem);

	//»ó·Ä¾¾ ÇÔ¼ö È£Ãâ ¿¹Á¤
	cCharShop.RecvShopItem(&MyShopItem);


	lpChar = FindAutoPlayer(lpTransTradeItems->dwSender);

	if (lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0]) {
		OpenEachPlayer(lpChar);
		DispEachMode = TRUE;
	}

	return TRUE;
}


//°³ÀÎ »óÁ¡ ¹°°Ç ±¸ÀÔ
int	Send_PersonalShopItem(DWORD dwCharCode, void* lpShopItem)
{
	smTRANS_COMMAND_BUFF	smtBuff;


	smtBuff.smTransCommand.code = smTRANSCODE_MYSHOP_TRADE;
	smtBuff.smTransCommand.size = sizeof(smTRANS_COMMAND) + sizeof(sMYSHOP_ITEM_SERVER);
	smtBuff.smTransCommand.WParam = 0;
	smtBuff.smTransCommand.LParam = dwCharCode;
	smtBuff.smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smtBuff.smTransCommand.EParam = 0;

	memcpy(smtBuff.Buff, lpShopItem, sizeof(sMYSHOP_ITEM_SERVER));

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smtBuff, smtBuff.smTransCommand.size, TRUE);

	return FALSE;
}

//°³ÀÎ »óÁ¡ ¾ÆÀÌÅÛ Á¤º¸ ¾÷µ¥ÀÌÆ®
int	UpdateMyShopList(void* lpMyShop)
{
	sMYSHOP* lpMyShopItem = (sMYSHOP*)lpMyShop;
	int	result = 0;
	int		len;

	ZeroMemory(&TransTradeItems_MyShop, sizeof(TRANS_TRADEITEMS));

	//µ¥ÀÌÅ¸ ¾ÐÃà ( Z/NZ ¹æ½Ä )
	len = EecodeCompress((BYTE*)lpMyShopItem, TransTradeItems_MyShop.TradeBuff, sizeof(sMYSHOP));
	if (len > TRANS_TRADE_BUFF_SIZE) return FALSE;

	TransTradeItems_MyShop.code = smTRANSCODE_MYSHOP_ITEM;
	TransTradeItems_MyShop.size = len + 48;
	TransTradeItems_MyShop.dwSum = 0;
	TransTradeItems_MyShop.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems_MyShop.dwRecver = 0;
	TransTradeItems_MyShop.Temp[0] = 0;
	TransTradeItems_MyShop.Temp[1] = 0;
	TransTradeItems_MyShop.Temp[2] = 0;
	TransTradeItems_MyShop.Temp[3] = 0;

	return TRUE;
}

//°³ÀÎ »óÁ¡ ¿ÀÇÂ
int SendOpenPersonalTrade(char* szTradeMsg, void* lpPersTrade)
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK* lpsmSock;
	int	result = 0;
	DWORD	dwCode;
	sMYSHOP* lpMyShopItem = (sMYSHOP*)lpPersTrade;
	int		len;

	//smTRANSCODE_OPEN_PERTRADE
	//lpMyShopItem->

	ZeroMemory(&TransTradeItems_MyShop, sizeof(TRANS_TRADEITEMS));
	//memcpy( &TransMyShop.sMyShop , lpMyShopItem , sizeof(sMYSHOP) );

	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	//µ¥ÀÌÅ¸ ¾ÐÃà ( Z/NZ ¹æ½Ä )
	len = EecodeCompress((BYTE*)lpMyShopItem, TransTradeItems_MyShop.TradeBuff, sizeof(sMYSHOP));
	if (len > TRANS_TRADE_BUFF_SIZE) return FALSE;

	TransTradeItems_MyShop.code = smTRANSCODE_MYSHOP_ITEM;
	TransTradeItems_MyShop.size = len + 48;
	TransTradeItems_MyShop.dwSum = 0;
	TransTradeItems_MyShop.dwSender = lpCurPlayer->dwObjectSerial;
	TransTradeItems_MyShop.dwRecver = 0;
	TransTradeItems_MyShop.Temp[0] = 0;
	TransTradeItems_MyShop.Temp[1] = 0;
	TransTradeItems_MyShop.Temp[2] = 0;
	TransTradeItems_MyShop.Temp[3] = 0;


	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy(TransChatMessage.szMessage, szTradeMsg);

	dwCode = GetSpeedSum(szTradeMsg);
	TransChatMessage.dwIP = dwCode;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		//ÇØ¿Ü Áß±¹(°³ÀÎ»óÁ¡ÀÌ¸§°ü·Ã)
		//#ifdef _LANGUAGE_CHINESE
		//		ConvertStringTHAI((char *)&TransChatMessage, lstrlen((char *)&TransChatMessage)+16);
		//#endif
		result = lpsmSock->Send2((char*)&TransChatMessage, TransChatMessage.size, TRUE);

	if (result) {
		/*
		//ÇØ¿Ü Áß±¹(°³ÀÎ»óÁ¡ÀÌ¸§°ü·Ã)
		#ifdef _LANGUAGE_CHINESE
				ConvertStringTHAI(szTradeMsg, lstrlen(szTradeMsg)+16);
		#endif
		*/
		wsprintf(lpCurPlayer->szTradeMessage, "%s: %s", lpCurPlayer->smCharInfo.szName, szTradeMsg);
		lpCurPlayer->dwTradeMsgCode = dwCode;
	}

	return result;
}

//»óÁ¡ ´Ý±â
int SendClosePersonalTrade()
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK* lpsmSock;
	int	result = 0;

	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char*)&TransChatMessage, TransChatMessage.size, TRUE);

	lpCurPlayer->dwTradeMsgCode = 0;
	lpCurPlayer->szTradeMessage[0] = 0;

	return result;
}

int SendsServerDoc(char* szTradeMsg)
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK* lpsmSock;
	int	result = 0;


	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	TransChatMessage.code = smTRANSCODE_ITEMDOC;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy(TransChatMessage.szMessage, szTradeMsg);

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char*)&TransChatMessage, TransChatMessage.size, TRUE);

	return result;
}

int DeleteEventItem_TimeOut(sITEMINFO * lpsItem)
{
	if (lpsItem->CODE == (sinBI1 | sin32) || lpsItem->CODE == (sinBI1 | sin33) || lpsItem->CODE == (sinBI1 | sin34) || lpsItem->CODE == (sinBI1 | sin51) ||
		lpsItem->CODE == (sinBI1 | sin36) || lpsItem->CODE == (sinBI1 | sin37) ||
		lpsItem->CODE == (sinBI1 | sin38) || lpsItem->CODE == (sinBI1 | sin39) ||
		lpsItem->CODE == (sinBI1 | sin40) || lpsItem->CODE == (sinBI1 | sin41) ||
		lpsItem->CODE == (sinBI1 | sin42) || lpsItem->CODE == (sinBI1 | sin43) ||
		lpsItem->CODE == (sinBI1 | sin44) || lpsItem->CODE == (sinBI1 | sin45) ||
		lpsItem->CODE == (sinBI1 | sin46) || lpsItem->CODE == (sinBI1 | sin47) ||
		lpsItem->CODE == (sinBI1 | sin48) || lpsItem->CODE == (sinBI1 | sin49) ||
		lpsItem->CODE == (sinBI1 | sin50) || lpsItem->CODE == (sinBI1 | sin52) ||
		lpsItem->CODE == (sinBI1 | sin53) || lpsItem->CODE == (sinBI1 | sin54) ||

		lpsItem->CODE == (sinBI1 | sin65) || lpsItem->CODE == (sinBI1 | sin66) ||
		lpsItem->CODE == (sinBI1 | sin67) || lpsItem->CODE == (sinBI1 | sin68) ||
		lpsItem->CODE == (sinBI1 | sin69) || lpsItem->CODE == (sinBI1 | sin70) ||
		lpsItem->CODE == (sinBI1 | sin71) || lpsItem->CODE == (sinBI1 | sin72) ||
		lpsItem->CODE == (sinBI1 | sin55) || lpsItem->CODE == (sinBI1 | sin77) ||
		lpsItem->CODE == (sinBI1 | sin62) || lpsItem->CODE == (sinBI1 | sin63) ||
		lpsItem->CODE == (sinBI1 | sin64) ||
		lpsItem->CODE == (sinBI1 | sin73) || lpsItem->CODE == (sinBI1 | sin74) ||
		lpsItem->CODE == (sinBI1 | sin75) || lpsItem->CODE == (sinBI1 | sin76))
	{
		return FALSE;
	}

	struct	tm	tm;
	time_t		ttm;
	DWORD		dwCreateTime;
	DWORD		dwItemCode;

	dwItemCode = lpsItem->CODE & sinITEM_MASK2;

	DWORD NowTime = GetPlayTime_T();

	if (lpsItem->expireTime > 0) {
		if (lpsItem->dwCreateTime + (60 * 60 * lpsItem->expireTime) < NowTime)
			return true;
	}

	if (lpsItem->CODE != (sinOR2 | sin01))
		return FALSE;

	tm.tm_year = 2021 - 1900;
	tm.tm_mon = tm.tm_mon + 1;
	tm.tm_mday = tm.tm_mday;
	tm.tm_hour = tm.tm_hour;
	tm.tm_min = tm.tm_min;
	tm.tm_sec = tm.tm_sec;
	ttm = mktime(&tm);

	dwCreateTime = (DWORD)ttm;

	if (lpsItem->dwCreateTime < dwCreateTime) return TRUE;

	return FALSE;
}

int RecvSodGameInfomation(void* Info)
{
	smTRANS_COMMAND_SOD* lpTransCommand = (smTRANS_COMMAND_SOD*)Info;

	if (!BellatraEffectInitFlag) {
		CreateBellatraFontEffect();
		BellatraEffectInitFlag = TRUE;
	}

	switch (lpTransCommand->smTransCommand.WParam) {
	case 1:
		if (SoD_SetFontEffect.eBL_Type == E_BL_LODING) {
			CheckBellatraFontEffect(&SoD_SetFontEffect, TRUE);	//·Îµù Á¾·á
		}

		if (lpTransCommand->smTransCommand.SParam < 0) {
			//SoD ¶ó¿îµå ÀÌÆåÆ®
			//Sod_SodEffect( lpTransCommand->smTransCommand.LParam+1 );
			sSodScore.EffectStartCount = 70 * 3;
			sSodScore.EffectCode = lpTransCommand->smTransCommand.LParam + 1;
		}
		else {
			if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) break;		//Á×Àº°æ¿ì ¹«½Ã

			sSodScore.SodNextStageNum = lpTransCommand->smTransCommand.SParam;
			sSodScore.dwSoD_NextStageTime = dwPlayTime + 9000;
			sSodScore.dwSoD_CloseStageTime = dwPlayTime + 5000;
			sSodScore.NextRound = lpTransCommand->smTransCommand.LParam + 1;

			StopBGM();
			esPlayContSound(14);
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);

			SetBellatraFontEffect(E_BL_FONT_STAGE);
			SetBellatraFontEffect(E_BL_FONT_COMPLETE);
			SoDGateFlag = TRUE;
		}
		break;

	case 3:
		//½ÇÆÐ µ¹¾Æ°¡±â
		if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) break;		//Á×Àº°æ¿ì ¹«½Ã

		sSodScore.SodNextStageNum = -1;
		sSodScore.dwSoD_NextStageTime = dwPlayTime + 8000;
		sSodScore.dwSoD_CloseStageTime = dwPlayTime + 6000;
		sSodScore.NextRound = -1;

		StopBGM();
		esPlayContSound(15);

		SetBellatraFontEffect(E_BL_FONT_STAGE);
		SetBellatraFontEffect(E_BL_FONT_FAIL);
		SoDGateFlag = TRUE;
		break;

	case 4:
		//¸ðµÎ Å¬¸®¾î
		if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_DEAD) break;		//Á×Àº°æ¿ì ¹«½Ã

		sSodScore.SodNextStageNum = lpTransCommand->smTransCommand.SParam;
		sSodScore.dwSoD_NextStageTime = dwPlayTime + 7000;
		sSodScore.NextRound = lpTransCommand->smTransCommand.LParam + 1;

		StopBGM();
		esPlayContSound(16);
		lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_YAHOO);

		//SetBellatraFontEffect( E_BL_FONT_STAGE );
		SetBellatraFontEffect(E_BL_FONT_CON);
		SoDGateFlag = FALSE;

		//È¹µæÇÑ ±Ý¾× Ãâ·Â
		char szBuff[128];
		wsprintf(szBuff, mgSOD_Clear, lpTransCommand->smTransCommand.EParam);
		cMessageBox.ShowMessageEvent(szBuff);

		break;

	case 2:
		//Á¡¼ö ¸ñ·Ï ¹ÞÀ½
		if (lpTransCommand->smTransCommand.LParam) {
			//³»Á¡¼ö È¹µæ¿¡ ÀÇÇÑ Á¡¼ö ¹ÞÀ½
			sSodScore.ScoreEffectCount = 255;

		}
		else {
			//ÁÖ±âÀûÀ¸·Î º¸³»Áö´Â ÀüÃ¼ Á¡¼ö


		}

		if (!sSodScore.dwSoD_NextStageTime) {
			if (sSodScore.Round != lpTransCommand->smTransCommand.SParam) {
				switch (lpTransCommand->smTransCommand.SParam) {
				case 1:		//1¶ó¿îµå ½ÃÀÛ
					sSodScore.dwPlayTime = dwPlayTime;
					PlayBGM_Direct(BGM_CODE_SOD1);
					break;

				case 4:
					PlayBGM_Direct(BGM_CODE_SOD2);
					break;

				case 7:
					PlayBGM_Direct(BGM_CODE_SOD3);
					break;
				}
			}
			sSodScore.Round = lpTransCommand->smTransCommand.SParam;
		}

		sSodScore.dwDispTime = dwPlayTime + 10 * 1000;
		sSodScore.MyTeam = lpTransCommand->MyTeam;
		sSodScore.Score = lpTransCommand->MyScore;
		sSodScore.TeamScore[0] = lpTransCommand->TeamScore[0];
		sSodScore.TeamScore[1] = lpTransCommand->TeamScore[1];
		sSodScore.TeamScore[2] = lpTransCommand->TeamScore[2];
		sSodScore.TeamScore[3] = lpTransCommand->TeamScore[3];

		break;

	case smCODE_SOD_EFFECT:					//ÁÖ¹® ½Ã½ºÅÛ Ãß°¡ - SOD ÀÌÆåÆ® Àç»ç¿ë
		if (!BellatraEffectInitFlag) {
			CreateBellatraFontEffect();
			BellatraEffectInitFlag = TRUE;
		}
		SetBellatraFontEffect((EBL_FontEffectType)lpTransCommand->smTransCommand.LParam);
		break;

	}

	return TRUE;
}

//Æ÷½º¿Àºê ¾ÆÀÌÅÛ ÀÔ¼ö
int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2 * lpTransItemGroup2)
{
	TRANS_ITEMINFO_GROUP	TransItemGroup;

	ZeroMemory(&TransItemGroup, sizeof(TRANS_ITEMINFO_GROUP));
	memcpy(&TransItemGroup, lpTransItemGroup2, TRANS_GROUPITEM_HEADER_SIZE);

	DecodeCompress((BYTE*)lpTransItemGroup2->szBuff, (BYTE*)TransItemGroup.sItemInfo, sizeof(sITEMINFO) * TRANS_GROUPITEM_MAX);

	//¹Ú»ó·Ä ÇÔ¼ö È£Ãâ
	//Æ÷½º ¾ÆÀÌÅÛÀ» ¹Þ´Â´Ù
	sinRecvForceOrb(TransItemGroup.sItemInfo, TransItemGroup.ItemCount);


	return TRUE;
}

//Äù½ºÆ® ¸í·É º¸³»±â
int SendQuestCommandToServer(DWORD dwQuestCode, int Param1, int Param2, int Param3)
{

	smTRANS_COMMAND	smTransCommand;
	int	result = 0;
	smWINSOCK* lpsmSock;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_QUEST_COMMAND;
	smTransCommand.WParam = dwQuestCode;
	smTransCommand.LParam = Param1;
	smTransCommand.SParam = Param2;
	smTransCommand.EParam = Param3;

	if (dwQuestCode >= HAQUEST_CODE_ELEMENTARY_A && dwQuestCode <= HAQUEST_CODE_FURYOFPHANTOM)
	{
		lpsmSock = smWsockDataServer;
		if (lpsmSock)
			result = lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

		return result;
	}

	lpsmSock = smWsockServer;
	if (lpsmSock)
		result = lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return result;
}

//´ëÀü Äù½ºÆ® ½ÇÇà
int Start_QuestArena(DWORD dwQuestCode, int Param1, int Param2)
{
	smTRANS_COMMAND	smTransCommand;
	int	result = 0;
	smWINSOCK* lpsmSock;

	//if ( dwQuestCode==SIN_QUEST_CODE_CHANGEJOB4 ) {

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_QUEST_COMMAND;
	smTransCommand.WParam = dwQuestCode;
	smTransCommand.LParam = QUEST_ARENA_FIELD;
	smTransCommand.SParam = Param1;
	smTransCommand.EParam = Param2;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	if (result)
		WarpField2(QUEST_ARENA_FIELD);

	//}

	return FALSE;
}

//ºí·¡½ºÄ³½½ ¼³Á¤Á¤º¸ ¼­¹ö¿¡ º¸³¿
int	SendBlessCastleToServer(smTRANS_BLESSCASTLE * lpBlessCastleSetup, int Mode)
{
	smWINSOCK* lpsmSock;

	lpBlessCastleSetup->smTransCommand.size = sizeof(smTRANS_BLESSCASTLE);
	lpBlessCastleSetup->smTransCommand.code = smTRANSCODE_BLESSCASTLE_INFO;
	lpBlessCastleSetup->smTransCommand.WParam = 1;
	lpBlessCastleSetup->smTransCommand.LParam = Mode;
	lpBlessCastleSetup->smTransCommand.SParam = 0;
	lpBlessCastleSetup->smTransCommand.EParam = 0;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		return lpsmSock->Send2((char*)lpBlessCastleSetup, lpBlessCastleSetup->smTransCommand.size, TRUE);

	return FALSE;
}

//ºí·¡½ºÄ³½½ ¼¼À² Á¤º¸ ¼­¹ö¿¡ ¿ä±¸
int	Send_GetBlessCastleTax()
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));

	smTransCommand.code = smTRANSCODE_BLESSCASTLE_INFO;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.SParam = cSinSiege.GetTaxRate();

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}
//ºí·¡½ºÄ³½½ ¼³Á¤Á¤º¸ ¼ö½Å
int RecvBlessCastInfo(void* lpPacket)
{
	smTRANS_BLESSCASTLE* lpBlessCastleSetup = (smTRANS_BLESSCASTLE*)lpPacket;
	rsUSER_LIST_TOP10* lpClanListTop10;

	switch (lpBlessCastleSetup->smTransCommand.WParam) {
	case 1:		//¼³Á¤Á¤º¸ ¼ö½Å
		chaSiege.ShowSiegeMenu(lpBlessCastleSetup);
		break;

	case 2:		//¼øÀ§ Á¤º¸ ¼ö½Å
		lpClanListTop10 = (rsUSER_LIST_TOP10*)lpPacket;
		//¼øÀ§ Ãâ·Â ÇÔ¼ö È£Ãâ
		chaSiege.ShowSiegeScore(lpClanListTop10);
		break;
	}

	return TRUE;
}

//ºí·¹½º Ä³½½ ¸¶½ºÅÍ Å¬·£ ¼³Á¤
int SetBlessCastleMaster(DWORD dwClanCode, int Mode)
{

	if (!Mode && lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD) {

	}
	else {
		if (rsBlessCastle.dwMasterClan != dwClanCode) {
			//Ä³½½ ¸¶½ºÅÍ ¾÷µ¥ÀÌÆ®
			UpdateCastleMasterClan(rsBlessCastle.dwMasterClan);
		}

		rsBlessCastle.dwMasterClan = dwClanCode;
	}

	return TRUE;
}

//¼Ó¼ºÁ¤º¸ ¼­¹ö·Î Àü¼Û
int	SendResistanceToServer()
{
	smTRANS_RESISTANCE	smTransResistance;
	smWINSOCK* lpsmSock;

	dwResistance_SendingTime = dwPlayTime;

	smTransResistance.code = smTRANSCODE_RESISTANCE;
	smTransResistance.size = sizeof(smTRANS_RESISTANCE);
	smTransResistance.Param = 0;
	memcpy(smTransResistance.Resistance, lpCurPlayer->smCharInfo.Resistance, sizeof(short) * 8);
	smTransResistance.Absorb = lpCurPlayer->smCharInfo.Absorption;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		return lpsmSock->Send2((char*)&smTransResistance, smTransResistance.size, TRUE);

	return FALSE;
}

//¼³¹®Á¶»ç °á°ú ¼­¹ö¿¡ º¸³»±â
int	SendPublicPollingToServer(int PollCode, int PollCnt, BYTE * bPollingData)
{
	smTRANS_COMMAND_POLLING	smTransPolling;

	ZeroMemory(&smTransPolling, sizeof(smTRANS_COMMAND_POLLING));

	smTransPolling.smTransCommand.code = smTRANSCODE_PUBLIC_POLLING;
	smTransPolling.smTransCommand.size = sizeof(smTRANS_COMMAND_POLLING);
	smTransPolling.smTransCommand.WParam = PollCode;
	smTransPolling.smTransCommand.LParam = PollCnt;
	memcpy(smTransPolling.bPolling, bPollingData, PollCnt);

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&smTransPolling, smTransPolling.smTransCommand.size, TRUE);


	return TRUE;
}


#ifdef _xTrap_GUARD
#include "XTrapSrc\\XTrap.h"
//¿¢½ºÆ®·¦ ÆÐÅ¶¼ö½Å
int XTrap_Recv(smTRANS_COMMAND * lpPacket, smWINSOCK * lpsmSock)
{
	smTRANS_XTRAP_ADDR* lpTransXTrapAddr;
	smTRANS_XTRAP_SECURITY* lpTransXTrapSecurity;
	UCHAR KFBuf[CREATEKF_OUT_SIZE * 2] = { 0, };

	//FILE *fp;
	//char szBuff[256];


	switch (lpPacket->WParam)
	{
	case smXTRAP_COMMAND_LOGIN:
		lpTransXTrapAddr = (smTRANS_XTRAP_ADDR*)lpPacket;
		lpTransXTrapSecurity = (smTRANS_XTRAP_SECURITY*)lpPacket;

		CreateKFEx(NULL, (PUCHAR)lpTransXTrapAddr->Addr, 2, KFBuf);

		lpTransXTrapSecurity->smTransCommand.size = sizeof(smTRANS_XTRAP_SECURITY);
		lpTransXTrapSecurity->smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
		lpTransXTrapSecurity->smTransCommand.WParam = smXTRAP_COMMAND_SECURITY;
		memcpy(&lpTransXTrapSecurity->serialkey, KFBuf, CREATEKF_OUT_SIZE * 2);
		lpsmSock->Send2((char*)lpTransXTrapSecurity, lpTransXTrapSecurity->smTransCommand.size, TRUE);

		break;

	case smXTRAP_COMMAND_SECURITY:
		if (lpPacket->LParam == 0) SendLogToMgr();
		DisconnectServerCode = 4;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 30");
		DisconnectFlag = GetCurrentTime();
		break;

	}
	return TRUE;
};
#else
#ifdef _XTRAP_GUARD_4_CLIENT
#include "XTrapSrcD5\Client\XTrap4Client.h"	//XTrapD5
//		#include "XTrapSrc4\XTrap.h"
		//¿¢½ºÆ®·¦ ÆÐÅ¶¼ö½Å
int XTrap_Recv(smTRANS_COMMAND * lpPacket, smWINSOCK * lpsmSock)
{
	char debug[512];
	smTRANS_XTRAP_ADDR* lpTransXTrapAddr = (smTRANS_XTRAP_ADDR*)lpPacket;

	sprintf(debug, "XTrap_Recv");
	OutputDebugString(debug);

	if (lpPacket->WParam == XTRAP_CMD_SEEDKEY)
	{//¼­¹ö¿¡¼­ ÆÐÅ¶À» ¹ÞÀ¸¸é
		sprintf(debug, "CMD_SEEDKEY");
		OutputDebugString(debug);

		sprintf(debug, "before : Packet Buf: %02X %02X %02X %02X %02X %02X",
			lpTransXTrapAddr->PacketBuf[0],
			lpTransXTrapAddr->PacketBuf[1],
			lpTransXTrapAddr->PacketBuf[2],
			lpTransXTrapAddr->PacketBuf[3],
			lpTransXTrapAddr->PacketBuf[4],
			lpTransXTrapAddr->PacketBuf[5]);
		OutputDebugString(debug);

		//----[ 1. ¼­¹ö·Î ºÎÅÍ ¹ÞÀº Á¤º¸ °Ë»ç ]
		smTRANS_XTRAP_ADDR	smTransXTrapAddr;
		memset(&smTransXTrapAddr, 0, sizeof(smTransXTrapAddr));

		unsigned int usResult = 1;
		//usResult = XTrapCC_STEP2( (PUCHAR)lpTransXTrapAddr->PacketBuf, (PUCHAR)smTransXTrapAddr.PacketBuf, XTRAP_PROTECT_PE|XTRAP_PROTECT_TEXT);
		usResult = XTrap_CS_Step2((PUCHAR)lpTransXTrapAddr->PacketBuf, (PUCHAR)smTransXTrapAddr.PacketBuf,
			XTRAP_PROTECT_PE | XTRAP_PROTECT_TEXT | XTRAP_PROTECT_EXCEPT_VIRUS);

		//----[ 2. °á°ú Á¤º¸ ¼­¹ö·Î º¸³»±â ]
		smTransXTrapAddr.smTransCommand.size = sizeof(smTRANS_XTRAP_ADDR);
		smTransXTrapAddr.smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
		smTransXTrapAddr.smTransCommand.WParam = XTRAP_CMD_UNIQKEY;

		if (smWsockDataServer)
			smWsockDataServer->Send((char*)&smTransXTrapAddr, smTransXTrapAddr.smTransCommand.size, TRUE);

		//----[ 3. ÇÔ¼ö ¸®ÅÏ°ª °Ë»ç ]
		if (usResult == 0)
		{//Á¤»ó
		}
		else
		{//ºñÁ¤»ó
			DisconnectServerCode = 4;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 31");
			DisconnectFlag = GetCurrentTime();
		}

		sprintf(debug, "after : Packet Buf: %02X %02X %02X %02X %02X %02X",
			lpTransXTrapAddr->PacketBuf[0],
			lpTransXTrapAddr->PacketBuf[1],
			lpTransXTrapAddr->PacketBuf[2],
			lpTransXTrapAddr->PacketBuf[3],
			lpTransXTrapAddr->PacketBuf[4],
			lpTransXTrapAddr->PacketBuf[5]);
		OutputDebugString(debug);

	}

	return TRUE;
};

VOID __stdcall XTrapCS_SendUniqKey(PUCHAR pUniqKey)
{}

#else
//¿¢½ºÆ®·¦ ÆÐÅ¶¼ö½Å
int XTrap_Recv(smTRANS_COMMAND * lpPacket, smWINSOCK * lpsmSock)
{
	return TRUE;
}

#endif
#endif



int	HackTrap_OpenFlagMask = 0;

//ÇÙÅø Ã½Å©¿ë NPC±â´É ±¸Çö½Ã ¼­¹ö¿¡ °Ë»çÅëº¸
int HackTrap_CheckOpenFlag()
{
	int ChkCnt = 0;

	// ¹ÚÀç¿ø - À¯Àú¸ðµå¿¡¼­ ÀÌµ¿ »óÁ¡ ¾ÆÀÌÅÛ »ç¿ë½Ã¿¡´Â ÇØÅ· ·Î±×¸¦ ¾È³²±ä´Ù. cShop.UseItemFlag==0
	if (cShop.OpenFlag && cShop.UseItemFlag == 0) {								//»óÁ¡ 
		if ((HackTrap_OpenFlagMask & sinNPC_SHOP) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_SHOP;
			SendHackTrapToServer(100, sinNPC_SHOP);
		}
		ChkCnt++;
	}

	if (cWareHouse.OpenFlag) {						//Ã¢°í 
		if ((HackTrap_OpenFlagMask & sinNPC_WARE) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_WARE;
			SendHackTrapToServer(100, sinNPC_WARE);
		}
		ChkCnt++;
	}

	if (Caravana::GetInstance()->OpenFlag) {						//Ã¢°í 
		if ((HackTrap_OpenFlagMask & sinNPC_WARE) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_WARE;
			SendHackTrapToServer(100, sinNPC_WARE);
		}
		ChkCnt++;
	}

	if (cCraftItem.OpenFlag) {							// Á¶ÇÕ 
		if (cCraftItem.ForceFlag)return false;

		if ((HackTrap_OpenFlagMask & sinNPC_MIX) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_MIX;
			SendHackTrapToServer(100, sinNPC_MIX);
		}

		ChkCnt++;
	}													//Æ÷½º Á¶ÇÕ 
	if (cCraftItem.ForceFlag) {
		if (cCraftItem.OpenFlag)return false;

		if ((HackTrap_OpenFlagMask & sinNPC_FORCE) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_FORCE;
			SendHackTrapToServer(100, sinNPC_FORCE);
		}
		ChkCnt++;
	}
	if (cAging.OpenFlag) {							//¿¡ÀÌÂ¡
		if ((HackTrap_OpenFlagMask & sinNPC_AGING) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_AGING;
			SendHackTrapToServer(100, sinNPC_AGING);
		}
		ChkCnt++;
	}

	// pluto Á¦·Ã
	if (SmeltingItem.OpenFlag) // pluto NPC Ãß°¡ ÇÒ¶§ HackTrap.h ¿ä±âµµ Ãß°¡ÇØ¶ó (ÇÊµåNPC±â´É °æ°í)
	{
		if ((HackTrap_OpenFlagMask & sinNPC_SMELTING) == 0)
		{
			HackTrap_OpenFlagMask |= sinNPC_SMELTING;
			SendHackTrapToServer(100, sinNPC_SMELTING);
		}
		ChkCnt++;
	}
	// pluto Á¦ÀÛ
	if (ManufactureItem.m_OpenFlag)
	{
		if ((HackTrap_OpenFlagMask & sinNPC_MANUFACTURE) == 0)
		{
			HackTrap_OpenFlagMask |= sinNPC_MANUFACTURE;
			SendHackTrapToServer(100, sinNPC_MANUFACTURE);
		}
		ChkCnt++;
	}

	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â 
	if (cMixtureReset.OpenFlag)
	{
		if ((HackTrap_OpenFlagMask & sinNPC_MIXTURE_RESET) == 0)
		{
			HackTrap_OpenFlagMask |= sinNPC_MIXTURE_RESET;
			SendHackTrapToServer(100, sinNPC_MIXTURE_RESET);
		}

		ChkCnt++;
	}

	if (SkillMasterFlag) {							//½ºÅ³¸¶½ºÅÍ
		if ((HackTrap_OpenFlagMask & sinNPC_SKILL) == 0) {
			HackTrap_OpenFlagMask |= sinNPC_SKILL;
			SendHackTrapToServer(100, sinNPC_SKILL);
		}
		ChkCnt++;
	}

	if (!ChkCnt)
		HackTrap_OpenFlagMask = 0;

	return	TRUE;
}


//ÇÙÅø Ã½Å©¿ë ÇÔÁ¤
int	SendHackTrapToServer(DWORD dwTrapCode, int Param)
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));

	smTransCommand.code = smTRANSCODE_HACKTRAP;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwTrapCode;
	smTransCommand.LParam = Param;

	if (dwTrapCode == 100) {
		//NPC ±â´É±¸Çö½Ã È£Ãâ
		if (lpCurPlayer->OnStageField >= 0) {
			smTransCommand.SParam = StageField[lpCurPlayer->OnStageField]->FieldCode;
		}
		else
			return FALSE;
	}

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);


	return TRUE;
}

// pluto Á¦·Ã µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendSmeltingItemToServer(void* lpsSmeltingItem_Send)
{
	sSMELTINGITEM_SERVER* lpSmeltingItem_Send = (sSMELTINGITEM_SERVER*)lpsSmeltingItem_Send;

	lpSmeltingItem_Send->size = sizeof(sSMELTINGITEM_SERVER);
	lpSmeltingItem_Send->code = smTRANSCODE_SMELTINGITEM;

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)lpSmeltingItem_Send, lpSmeltingItem_Send->size, TRUE);

	return FALSE;
}
// pluto Á¦ÀÛ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendManufactureItemToServer(void* lpsManufactureItem_Send)
{
	SManufactureItem_Server* lpManufactureItem_Send = (SManufactureItem_Server*)lpsManufactureItem_Send;

	lpManufactureItem_Send->size = sizeof(SManufactureItem_Server);
	lpManufactureItem_Send->code = smTRANSCODE_MANUFACTURE;

	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)lpManufactureItem_Send, lpManufactureItem_Send->size, TRUE);

	return FALSE;
}

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀ» ¼­¹ö·Î Àü¼ÛÇÑ´Ù.
int SendMixtureResetItemToServer(void* lpsMixtureResetItem_Send)
{
	// ¾ÆÀÌÅÛÀÇ Á¤º¸¸¦ ÀúÀå
	sMIXTURE_RESET_ITEM_SERVER* lpMixtureResetItem_Send = (sMIXTURE_RESET_ITEM_SERVER*)lpsMixtureResetItem_Send;

	// »çÀÌÁî¿Í ¹Í½ºÃÄ ¸®¼ÂÀÌ ¼öÇàµÉ ÄÚµå¸¦ ¼³Á¤
	lpMixtureResetItem_Send->size = sizeof(sMIXTURE_RESET_ITEM_SERVER);
	lpMixtureResetItem_Send->code = smTRANSCODE_MIXTURE_RESET_ITEM;

	// µ¥ÀÌÅÍ ¼­¹ö·Î º¸³»¶ó!!!
	if (smWsockDataServer)
		return smWsockDataServer->Send2((char*)lpMixtureResetItem_Send, lpMixtureResetItem_Send->size, TRUE);

	return FALSE;
}

#ifdef _XIGNCODE_CLIENT
// ¹ÚÀç¿ø - XignCode

int Xigncode_Client_Start()
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));

	smTransCommand.code = smTRANSCODE_XIGNCODE_START;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = TRUE;

	if (smWsockDataServer)
		smWsockDataServer->Send2((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}


XIGNCODE_PACKET return_packet;

int Xigncode_Client_Recv(smWINSOCK * lpsmSock, XIGNCODE_PACKET * pack)
{
	//	XIGNCODE_PACKET return_packet;
	memset(&return_packet, 0, sizeof(return_packet));

	if (ZCWAVE_Probe((char*)pack->data, (char*)return_packet.data, sizeof(return_packet.data)))
	{
		return_packet.size = sizeof(return_packet);
		return_packet.code = smTRANSCODE_XIGNCODE_PACKET;
		lpsmSock->Send2((char*)&return_packet, return_packet.size, TRUE);
	}

	return TRUE;
}
#endif
/*
struct smTRANS_COMMAND_POLLING {
	smTRANS_COMMAND		smTransCommand;
	BYTE	bPolling[10];
};
//¼³¹®Á¶»ç
#define	smTRANSCODE_PUBLIC_POLLING		0x50320A70
//¼³¸í°øÁö ¹Ú½º
#define	smTRANSCODE_NOTICE_BOX			0x50320A80
*/
