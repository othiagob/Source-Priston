#include "Utils\common.h"
#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "Character/character.h"
#include "Character/playmain.h"
#include "fileread.h"


#include "sinbaram\\sinlinkheader.h"
#include "Character/record.h"
#include "netplay.h"
#include "Character/playsub.h"
#include "field.h"

#include "language.h"
#include "srcLang\\jts.h"
#include "TextMessage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\help.h"
#include "Character/damage.h"
#include "Character/skillsub.h"

#include "AreaServer.h"

#include "srcserver\\onserver.h"
#include "ConnectReader.h"

extern HWND hwnd;

#define SERVER_CHAR_INFO
#define RECORD_TIME_STEP		(5*60*1000)
#define NET_LOADING_WAIT_LIMIT		10000

extern CRITICAL_SECTION	cSection;
extern rsRECORD_DBASE	rsRecorder;
DWORD	dwLastRecordTime = 0;

CRITICAL_SECTION	cServSection;
INT PlayTimerMax = 0;
int Debug_SendCount = 0;
int Debug_RecvCount1 = 0;
int Debug_RecvCount2 = 0;
int Debug_RecvCount3 = 0;

int NetWorkInitFlag = 0;
int	BellatraEffectInitFlag = 0;


DWORD	dwTime_ServerT = 0;
DWORD	dwTime_ConnectMS = 0;

#define	CLIENT_VERSION_NUM		1000

#define CLIENT_VERSION_CHECK	(-CLIENT_VERSION_NUM*2)
int	Client_Version = CLIENT_VERSION_NUM;
int	Server_LimitVersion = 168;

int	Version_WareHouse = 1;
int	Version_CharInfo = 1;


smWINSOCK	*smWsockServer = 0;
smWINSOCK	*smWsockDataServer = 0;
smWINSOCK	*smWsockUserServer = 0;
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

char* szDefaultServIP = "127.0.0.1"; // Valor padrão

// Função para inicializar o szDefaultServIP com o valor do Connect.ini
void InicializarDefaultServIP()
{
	std::string strIP = LerIPConnect();
	if (!strIP.empty()) {
		// Aloca memória para o novo IP e copia o valor
		static char szNewIP[32];
		strcpy(szNewIP, strIP.c_str());
		szDefaultServIP = szNewIP;
		std::cout << "szDefaultServIP inicializado com: " << szDefaultServIP << std::endl;
	}
	else {
		std::cout << "Não foi possível ler o IP do Connect.ini. Usando valor padrão: " << szDefaultServIP << std::endl;
	}
}


char *szNetLogFile = "debugnet.log";
FILE *fpNetLog = 0;

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

typedef DWORD(*LPFN_CheckMem)(TRANS_FUNC_MEMORY *TransFuncMem, smTRANS_COMMAND	*lpTransCommand);
LPFN_CheckMem	fnChkMem2;

static int	CompWareHouseMoney = 0;
int	WareHouseSubMoney = 0;
int TotalSubMoney = 0;
int TotalAddMoney = 0;
int	TotalAddExp = 0;
int	TotalSubExp = 0;


smTRANS_COMMAND	*lpTransVirtualPotion = 0;


LPFN_CheckMem	fnChkMem;
DWORD	dwMemFunChkCode = 0;
int	RecvMemFuncData(TRANS_FUNC_MEMORY *TransFuncMem);
int	RecvMemFuncData2(TRANS_FUNC_MEMORY *TransFuncMem);
DWORD funcCheckMemSum(DWORD FuncPoint, int count);
int CheckProcessModule();


smPLAYDATA	PlayData;
char	TransBuff[smSOCKBUFF_SIZE];
int		TransLen;

TRANS_ITEMINFO	TransThrowItem;
TRANS_ITEMINFO	TransRecvItem;

TRANS_TRADE_ITEMKEY	LastTransTradeItemKey;
DWORD				dwLastTransTradeKeyTime = 0;
DWORD				dwYahooTime = 0;

int					InitClanMode = 0;
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

TRANS_TRADEITEMS	TransLastRecvTrade;
/*
//°³ÀÎ»óÁ¡ Á¤º¸
struct	TRANS_MYSHOP_ITEM {
	int		size,code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;
	DWORD	Temp[4];

	sMYSHOP	sMyShop;
};
*/
TRANS_TRADEITEMS	TransTradeItems_MyShop;

DWORD	dwLastWareHouseChkSum = 0;

int		TransAgingItemFlag;

smTRANS_COMMAND	TransServerConnectInfo;

int ConnectOtherPlayer(DWORD dwIP);

int HoRecvMessage(DWORD dwCode, void *RecvBuff);

int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY *lpTransTradeItemKey, sTRADE *lpTrade);
int SendTradeSucessKey(sTRADE *lpTrade, DWORD dwSender);
int RecvTradeCheckItem(TRANS_TRADE_CHECKITEM *lpTransTradeCheckItem);
int SendCrackWindow(HWND hWnd);

int ThrowItemToServer(smTRANS_COMMAND_EX *lpTransCommand);
int RecvCheckItemFromServer(TRANS_ITEM_CODE *lpTransItemCode);
int	SendClientFuncPos();

int OpenTimeCheckThread();
int SendPlayTimerMax();

TRANS_ITEMINFO	*FindRecvTransItemQue(DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
int	PushRecvTransItemQue(TRANS_ITEMINFO *lpTransItemInfo);

int RecvProcessSkill(smTRANS_COMMAND *lpTransCommand);
int RecvPartySkillFromServer(TRANS_PARTY_SKILL *lpTransPartySkill);

int FiltQuestItem(TRANS_ITEMINFO	*lpTransItemInfo, DWORD dwPacketCode);
int RecvSodGameInfomation(void *Info);

int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2);

int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock);

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
		mcnt = (AttMonsterCodeCount - 1)&ATT_MONSTER_MASK;
		if (dwAttMonsterCodeList[mcnt] == dwCode) return TRUE;
		mcnt = AttMonsterCodeCount&ATT_MONSTER_MASK;
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
int CheckServerInfo(smTRANS_COMMAND *lpTransCommand)
{
	return TRUE;
}

int SendProcessInfo()
{

	return TRUE;
}


smWINSOCK *ConnectServer_Main()
{
	if (!smWsockServer)
	{
		if (smConfig.szServerIP[0])
			lstrcpy(szServIP, smConfig.szServerIP);
		else
			lstrcpy(szServIP, szDefaultServIP);

		if (smConfig.dwServerPort)
			dwServPort = smConfig.dwServerPort;
		else
			dwServPort = TCP_SERVPORT;

		smWsockServer = smConnectSock(szServIP, (WORD)dwServPort);
	}

	if (smWsockServer && !smWsockDataServer)
	{
		if (smConfig.szDataServerIP[0] && lstrcmpi(smConfig.szDataServerIP, smConfig.szServerIP) != 0)
		{
			lstrcpy(szDataServIP, smConfig.szDataServerIP);
			dwDataServPort = smConfig.dwDataServerPort;
			smWsockDataServer = smConnectSock(szDataServIP, (WORD)dwDataServPort);
		}
		else
		{
			lstrcpy(szDataServIP, szServIP);
			dwDataServPort = dwServPort;
			smWsockDataServer = smWsockServer;
		}
	}

	if (smWsockServer && smWsockDataServer && !smWsockUserServer)
	{
		if (smConfig.szUserServerIP[0] && lstrcmpi(smConfig.szUserServerIP, smConfig.szServerIP) != 0) {
			lstrcpy(szUserServIP, smConfig.szUserServerIP);
			dwUserServPort = smConfig.dwUserServerPort;
			smWsockUserServer = smConnectSock(szUserServIP, (WORD)dwUserServPort);
		}
		else {
			lstrcpy(szUserServIP, szServIP);
			dwUserServPort = dwServPort;
			smWsockUserServer = smWsockServer;
		}
	}

	if (smWsockServer && smWsockDataServer && !smWsockExtendServer)
	{
		if (smConfig.szExtendServerIP[0] && lstrcmpi(smConfig.szExtendServerIP, smConfig.szServerIP) != 0) {
			lstrcpy(szExtendServIP, smConfig.szExtendServerIP);
			dwExtendServPort = smConfig.dwExtendServerPort;
			smWsockExtendServer = smConnectSock(szExtendServIP, (WORD)dwExtendServPort);
		}
		else
		{
			lstrcpy(szExtendServIP, szServIP);
			dwExtendServPort = dwServPort;
			smWsockExtendServer = smWsockServer;
		}
	}


	if (smWsockServer && smWsockDataServer && smWsockUserServer)
	{
		return smWsockServer;
	}

	return NULL;
}

int DisconnectServerFull()
{

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

smWINSOCK *ConnectServer_InfoMain()
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

smWINSOCK *ConnectServer_GameMain(char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2)
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

smWINSOCK *ConnectServer_GameMain(char *szIP1, DWORD dwPort1, char *szIP2, DWORD dwPort2, char *szIP3, DWORD dwPort3)
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
		//°ÔÀÓ ¼­¹ö ¿¬°á
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
			//¼­¹ö Á¤º¸ È®ÀÎ
			CheckServerInfo(&TransServerConnectInfo);
		}

		return smWsockServer;
	}
	return NULL;
}


//°ÔÀÓ ¼­¹ö ¿¬°á ²÷À½
int DisconnectServer_GameMain()
{
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

//¼­¹ö ÄÚµå·Î ¼ÒÄÏÀ» Ã£À½
smWINSOCK *GetServerSock(int ServerCode)
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

//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
smWINSOCK *GetAreaServerSock()
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


//¶³¾îÁø ¾ÆÀÌÅÛ Ã£±â
scITEM *FindScItem(int x, int z)
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

//ºó ¾ÆÀÌÅÛ °ø°£Ã£±â
scITEM *FindEmptyScItem()
{
	int cnt;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++) {
		if (!scItems[cnt].Flag) return &scItems[cnt];
	}

	return NULL;
}

//±³È¯ °Å·¡ °Å¸® È®ÀÎ
int GetTradeDistance(smCHAR *lpChar)
{
	int	x, y, z;
	int dist;

	x = (lpCurPlayer->pX - lpChar->pX) >> FLOATNS;
	y = (lpCurPlayer->pY - lpChar->pY) >> FLOATNS;
	z = (lpCurPlayer->pZ - lpChar->pZ) >> FLOATNS;

	dist = x*x + y*y + z*z;

	if (dist < (256 * 256)) return TRUE;

	return FALSE;
}

int GetTradeDistanceFromCode(DWORD dwObjectSerial)
{
	smCHAR *lpChar;

	lpChar = FindAutoPlayer(dwObjectSerial);

	if (lpChar && lpChar->DisplayFlag && lpChar->smCharInfo.szName[0]) {

		return  GetTradeDistance(lpChar);

	}
	return FALSE;
}

int	CheckStartCharInfo()
{
	smCHAR_INFO	smCharInfo;
	int cnt;

	memcpy(&smCharInfo, sinChar, sizeof(smCHAR_INFO));

	CheckCharForm();
	CodeXorExp = dwPlayTime + smCharInfo.Exp;
	CodeXorExp_High = dwPlayTime | (dwPlayTime*dwPlayTime);
	CodeXorLife = (dwPlayTime*smCharInfo.Life[0]) & 0xFFFF;
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

int	RecvMemFuncData2(TRANS_FUNC_MEMORY *TransFuncMem)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = -1;
	fnChkMem2 = (LPFN_CheckMem)((void *)TransFuncMem->szData);
	fnChkMem2(TransFuncMem, &smTransCommand);

	ZeroMemory(TransFuncMem, 1500);

	if (smTransCommand.size > 0 && smWsockDataServer)
	{
		smTransCommand.code = smTRANSCODE_FUNCTION_MEM2;
		smWsockDataServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
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
		smWsockServer->Send((char *)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}
	if (smWsockUserServer && smWsockUserServer != smWsockServer) {
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[1], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[1], sizeof(sSERVER_MONEY));
		smWsockUserServer->Send((char *)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}
	if (smWsockExtendServer && smWsockExtendServer != smWsockServer) {
		memcpy(&TransTotalExpMoney.sServerExp, &sServerExp[2], sizeof(sSERVER_EXP));
		memcpy(&TransTotalExpMoney.sServerMoney, &sServerMoney[2], sizeof(sSERVER_MONEY));
		smWsockExtendServer->Send((char *)&TransTotalExpMoney, TransTotalExpMoney.size, TRUE);
	}

	dwLastSendTotalExpMoenyTime = dwPlayTime;

	return TRUE;
}

//¼­¹ö¿¡¼­ µé¾î¿Â µ·ÀÇ ¾ç È®ÀÎ
int CheckServerMoney(smWINSOCK *lpsmSock, TRANS_ITEMINFO	*lpTransItemInfo)
{
	sSERVER_MONEY	*lpServerMoney;

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

		/*
							TransItemInfo.x = dwPlayServTime;											//ÇØµ¶ Å°
							TransItemInfo.y = TransItemInfo.Item.ItemHeader.dwChkSum;					//ÇØµ¶ Å°2
							TransItemInfo.z = rsPlayInfo[cnt].spMoney_Out^(TransItemInfo.x+TransItemInfo.y);	//ÅäÅ» ±â·Ï Àü¼Û
		*/

		int cnt;
		cnt = lpServerMoney->TotalZ ^ (lpServerMoney->TotalX + lpServerMoney->TotalY);
		cnt += 200;
		if (cnt < lpServerMoney->InputMoney) {
			//SendSetHackUser( 82 );
		}
	}

	return TRUE;
}

//¼­¹ö¿¡¼­ µé¾î¿Â °æÇèÄ¡ÀÇ ¾ç È®ÀÎ
int CheckServerExp(smWINSOCK *lpsmSock, smTRANS_COMMAND	*lpTransCommand)
{
	sSERVER_EXP	*lpServerExp;

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

		// Àåº° - °æÇèÄ¡ ¿¡·¯ À§ÇØ (800000 - > 1600000 = °æÇèÄ¡ 2¹è ÀÌº¥Æ®)
		int cnt = (lpServerExp->Total1^lpServerExp->Total2) + 1600000;		//¿ÀÂ÷À² Àû¿ë
		if (cnt < lpServerExp->InputExp)
		{
			SendSetHackUser(83);
		}

	}

	return TRUE;
}

//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°á ¾²·¹µå¸¦ »ý¼º
int CreateConnectThread(DWORD dwIP);
//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°áÁ¾·á ¾²·¹µå »ý¼º
int CreateDisconnectThread(smWINSOCK *lpsmSock);

//´Ù¸¥ ´Ù¸¥ ÇÃ·¹ÀÌ¾îÀÇ ºó ¿µ¿ªÀ» Ã£À½
int FindNewOtherPlayer()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (!chrOtherPlayer[cnt].Flag)
			return cnt;
	}

	return -1;
}

//ÀÚµ¿ ÇÃ·¹ÀÌ¾î¸¦ Ã£´Â´Ù
smCHAR *FindAutoPlayer(DWORD dwObjectSerial)
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial == dwObjectSerial) {
			return &chrOtherPlayer[cnt];
		}
	}

	return NULL;
}

// À¯Àú¹øÈ£·Î Ã£´Â´Ù
smCHAR *FindChrPlayer(DWORD dwObjectSerial)
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

//Á×ÀºÆÄÆ¼À¯Àú¸¦ Ã£´Â´Ù
smCHAR *FindDeadPartyUser()
{
	int cnt;
	smCHAR *lpChar;
	int dist, x, y, z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300 * 300;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].PartyFlag && chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER) {
			if (chrOtherPlayer[cnt].MotionInfo->State == CHRMOTION_STATE_DEAD) {

				x = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				y = (lpCurPlayer->pY - chrOtherPlayer[cnt].pY) >> FLOATNS;
				z = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;

				dist = x*x + z*z + y*y;

				if (dist < NearDist) {
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}

//ÃÖ±ÙÁ¢ ¸ó½ºÅÍ¸¦ Ã£´Â´Ù
smCHAR *FindNearMonster(smCHAR *lpCurChar)
{
	int cnt;
	smCHAR *lpChar;
	int dist, x, y, z;
	int	NearDist;

	lpChar = 0;
	NearDist = 300 * 300;

	if (!lpCurChar) return NULL;

	if (lpCurChar != lpCurPlayer) {
		x = (lpCurChar->pX - lpCurPlayer->pX) >> FLOATNS;
		y = (lpCurChar->pY - lpCurPlayer->pY) >> FLOATNS;
		z = (lpCurChar->pZ - lpCurPlayer->pZ) >> FLOATNS;

		dist = x*x + z*z + y*y;
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

				dist = x*x + z*z + y*y;

				if (dist < NearDist) {
					lpChar = &chrOtherPlayer[cnt];
					NearDist = dist;
				}
			}
		}
	}

	return lpChar;
}

//ºó ÇÃ·¹ÀÌ¾î¸¦ Ã£´Â´Ù
smCHAR *FindEmptyChar()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (!chrOtherPlayer[cnt].Flag)
			return &chrOtherPlayer[cnt];
	}

	return NULL;
}

//Ã¤ÆÃÃ¢¿¡ ¹®ÀÚ »ðÀÔ
int AddChatBuff(char *szMsg, DWORD dwIP)
{
	int ChatCnt;

	ChatCnt = ChatBuffCnt & CHATBUFF_MASK;
	ChatBuff[ChatCnt].dwIP = dwIP;
	lstrcpy(ChatBuff[ChatCnt].szMessage, szMsg);
	ChatBuffCnt++;
	ChatDispCnt = 70 * 10;
	if (ChatScrollPoint) {
		ChatScrollPoint++;
	}

	return ChatBuffCnt;
}

//Ã¤ÆÃÃ¢¿¡ ¹®ÀÚ »ðÀÔ
int AddChatBuff(char *szMsg)
{
	return AddChatBuff(szMsg, 0);
}

int	ChatBuffFilter(char* szMsg, DWORD dwIP) { return TRUE; }


//½Ã½ºÅÛ ½Ã°£À» °ÔÀÓ½Ã°£À¸·Î º¯È¯
DWORD	ConvSysTimeToGameTime(DWORD dwTime)
{
	return dwTime / GAME_WORLDTIME_MIN;
}

//DispLoading

#define RECV_DATA_QUE_COUNT		64
#define RECV_DATA_QUE_MASK		63


class	rsTRANS_SERVER {
	char	TransBuff[smSOCKBUFF_SIZE];

	rsRECORD_DBASE	*lpRecorder;


	smTHREADSOCK	*RecvDataQue[RECV_DATA_QUE_COUNT];
	int				RecvDataQuePush;
	int				RecvDataQuePop;

	TRANS_PLAYPOS	TransPlayPosLast;
	int				TransPlayPosCount;

public:
	smWINSOCK	*lpsmSock;


	//Å¬·¡½º ÃÊ±âÈ­
	int	Init(smWINSOCK *lpsmsock);
	//¼­¹ö·Î ¸Þ¼¼Áö Àü¼Û
	int SendChat(char *szMessage);
	//¼­¹ö·Î ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸¸¦ À§Ä¡¸¦ Àü¼Û
	int SendPlayData(smCHAR *lpChar);

	//¼­¹ö·Î ºÎÅÍ ¹ÞÀº ¸Þ¼¼Áö Ã³¸®
	int RecvMessage(smTHREADSOCK *SockInfo);

	//¸Þ¼¼Áö Å¥ÀÇ ¸Þ¼¼Áö¸¦ Ã³¸®
	int RecvMessageQue();

};

/*
		case smTRANSCODE_ATTACKDATA:
		case smTRANSCODE_PLAYDATA1:
		case smTRANSCODE_PLAYDATA2:
		case smTRANSCODE_PLAYDATA3:
		case smTRANSCODE_TRANSPLAYDATA:
		case smTRANSCODE_PLAYDATAGROUP:
		case smTRANSCODE_PLAYITEM:
*/

//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_STAND
//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_WALK
//lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_RUN


//¸Þ¼¼Áö Å¥ÀÇ ¸Þ¼¼Áö¸¦ Ã³¸®
int rsTRANS_SERVER::RecvMessageQue()
{
	int cnt;

	if (RecvDataQuePop >= RecvDataQuePush) return TRUE;
	if (MessageLoading) return FALSE;

	//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
	EnterCriticalSection(&cSection);

	while (1) {
		if (RecvDataQuePop >= RecvDataQuePush) break;

		cnt = RecvDataQuePop & RECV_DATA_QUE_MASK;

		if (RecvDataQue[cnt]) {
			RecvMessage(RecvDataQue[cnt]);

			delete RecvDataQue[cnt];
			RecvDataQue[cnt] = 0;
		}

		RecvDataQuePop++;
	}

	//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
	LeaveCriticalSection(&cSection);

	return TRUE;
}


DWORD	dwGetCharInfoTime = 0;

int rsTRANS_SERVER::RecvMessage(smTHREADSOCK *pData) { return TRUE; }

int rsTRANS_SERVER::Init(smWINSOCK *lpsmsock)
{

	lpsmSock = lpsmsock;
	lpRecorder = 0;

	//Å©¸®Æ¼Ä® ¼½¼Ç ÃÊ±âÈ­
	InitializeCriticalSection(&cServSection);

	RecvDataQuePush = 0;
	RecvDataQuePop = 0;

	ZeroMemory(RecvDataQue, RECV_DATA_QUE_COUNT * 4);

	return TRUE;
}

int rsTRANS_SERVER::SendPlayData(smCHAR *lpChar)
{
	//	int cnt;
	//	int	*lpTransBuff;
	TRANS_PLAYPOS *lpTransPlayPos;
	int	dist, x, y, z;

	TransPlayPosCount++;

	if ((TransPlayPosCount & 1) == 0) {
		x = (TransPlayPosLast.rsPlayPos.x - lpChar->pX) >> FLOATNS;
		y = (TransPlayPosLast.rsPlayPos.y - lpChar->pY) >> FLOATNS;
		z = (TransPlayPosLast.rsPlayPos.z - lpChar->pZ) >> FLOATNS;
		dist = x*x + y*y + z*z;

		if (sinGetLife() > 0 && dist < (16 * 16 * 16)) {
			//À§Ä¡°¡ º¯ÇÑ°Ô ¾ø´Â °æ¿ì ( 1¹ø¾¿ °É·¯¼­ º¸³¿ )
			return TRUE;
		}
	}

	lpTransPlayPos = (TRANS_PLAYPOS *)TransBuff;
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
		smWsockServer->Send((char *)lpTransPlayPos, lpTransPlayPos->size, TRUE);

	if (smWsockExtendServer && smWsockExtendServer != smWsockServer)
		smWsockExtendServer->Send((char *)lpTransPlayPos, lpTransPlayPos->size, TRUE);


	int	MainArea, SubArea;

	if (AreaServerMode) {
		if (lpCurPlayer->OnStageField >= 0) {
			MainArea = (lpCurPlayer->OnStageField) & 1;
			SubArea = (lpCurPlayer->OnStageField + 1) & 1;

			if (lpWSockServer_DispArea[SubArea] && lpWSockServer_DispArea[MainArea] != lpWSockServer_DispArea[SubArea]) {
				lpWSockServer_DispArea[SubArea]->Send((char *)lpTransPlayPos, lpTransPlayPos->size, TRUE);
			}
		}
	}


	return TRUE;
}


//¼­¹ö Åë½Å Å¬·¡½º ¼±¾ð
rsTRANS_SERVER	rsMainServer;







/*
//ÇÃ·¹ÀÌ¾î Á¤º¸¸¦ »ó´ë¹æ¿¡ Àü¼Û
int SendPlayerInfo( smWINSOCK *lpsmSock )
{

	smTRNAS_PLAYERINFO	*lpTransPlayerInfo;

	lpTransPlayerInfo = (smTRNAS_PLAYERINFO *)TransBuff;

	lstrcpy( lpTransPlayerInfo->smCharInfo.szName , PlayerName );
	lstrcpy( lpTransPlayerInfo->smCharInfo.szModelName , lpCurPlayer->lpDinaPattern->szPatName );

	lpTransPlayerInfo->size = sizeof( smTRNAS_PLAYERINFO );
	lpTransPlayerInfo->code = smTRANSCODE_PLAYERINFO;
	lpsmSock->Send( (char *)lpTransPlayerInfo , lpTransPlayerInfo->size );

	return TRUE;
}
*/

#define SAVE_CHAT_COMMAND_MAX		16
#define SAVE_CHAT_COMMAND_MASK		15

char szSaveChatCommands[SAVE_CHAT_COMMAND_MAX][256];
int ChatCommandSaveCnt = 0;
int ChatSaveRecallCnt = 0;

//Ã¤ÆÃ ÄÚ¸Çµå ÀúÀå
int SaveChatCommand(char *szMessage)
{
	int len, cnt;
	int mcnt;
	char szBuff[256];

	len = lstrlen(szMessage);
	lstrcpy(szBuff, szMessage);

	for (cnt = 0; cnt < len; cnt++) {
		if (szBuff[cnt] == ' ') {
			szBuff[cnt] = 0;
			break;
		}
	}
	lstrcat(szBuff, " ");

	//µ¿ÀÏ¹®ÀÚ È®ÀÎ
	for (cnt = 0; cnt < ChatCommandSaveCnt; cnt++) {
		if (cnt >= SAVE_CHAT_COMMAND_MAX) break;
		if (lstrcmp(szSaveChatCommands[cnt], szBuff) == 0)
			return TRUE;
	}

	mcnt = ChatCommandSaveCnt & SAVE_CHAT_COMMAND_MASK;
	lstrcpy(szSaveChatCommands[mcnt], szBuff);

	ChatCommandSaveCnt++;
	ChatSaveRecallCnt = ChatCommandSaveCnt;

	return TRUE;
}

//Ã¤ÆÃ ÀúÀå ÄÚ¸Çµå º¹±¸ 
int RecallSavedChatCommand(HWND hChatWnd, int Arrow)
{
	int mcnt;
	int len;

	if (ChatCommandSaveCnt <= 0) return FALSE;

	if (Arrow) {
		ChatSaveRecallCnt--;
		if (ChatSaveRecallCnt < 0) {
			ChatSaveRecallCnt = ChatCommandSaveCnt - 1;
		}
	}
	else {
		ChatSaveRecallCnt++;
		if (ChatSaveRecallCnt >= ChatCommandSaveCnt) {
			ChatSaveRecallCnt = ChatCommandSaveCnt - 1;
			SetWindowText(hChatWnd, "");
			return TRUE;
		}
	}

	mcnt = ChatSaveRecallCnt & SAVE_CHAT_COMMAND_MASK;
	SetWindowText(hChatWnd, szSaveChatCommands[mcnt]);

	len = lstrlen(szSaveChatCommands[mcnt]);
	SendMessage(hChatWnd, EM_SETSEL, len, len);

	return TRUE;
}

// ¹®ÀÚ¿­ ºñ±³ ÂªÀº ¹®ÀåÀÇ ±æÀÌ¸¸Å­ ºñ±³ÇÏ¿© ºñ±³ Á¾·á Æ÷ÀÎÅÍ ¹ÝÈ¯
int rsCompString(char *src1, char *src2)
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


char *rsGetWord(char *q, char *p);		//¼­¹ö¿¡ ¼±¾ðµÈ °ª


//Ã¤ÆÃ ¹®ÀÚ¸¦ ¼­¹ö·Î Àü¼Û
int SendChatMessageToServer(char *szChatMessage) { return TRUE; }


//Ã¤ÆÃ ½ºÅ©·Ñ Æ÷ÀÎÆ®
int ChatScrollPoint = 0;

//Ã¤ÆÃ Á¤º¸ È­¸é¿¡ Ç¥½Ã
int DisplayChatMessage(HDC hdc, int x, int y, int MaxLine) { return TRUE; }







//250 , 60 ,342 , 70

//ÄÆÆ®ÇÒ ¶óÀÎ Ä«¿îÅÍ
int GetNextLineCount(char *szString, int LineMax)
{

	return TRUE;
}

int SetIpColor(HDC hdc, DWORD dwIP) { return TRUE; }

static int	DisplayChatFilter = -1;
int	DispChatMsgHeight = 6;			//ÃÖ±Ù Ç¥½ÃµÈ Ã¤ÆÃÃ¢ ÁÙ ³ôÀÌ
int	DispChatMode = 0;

//Ã¤ÆÃÃ¢ ÇÊÅÍ¸µ
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

int ChatBuffSort()
{
	int cnt, cnt2;
	int mCnt, nCnt;
	int	Col;

	if (ChatBuffCnt <= ChatBuffCompCnt) return FALSE;

	for (cnt = ChatBuffCompCnt; cnt < ChatBuffCnt; cnt++) {
		mCnt = cnt&CHATBUFF_MASK;
		Col = 0;
		switch (ChatBuff[mCnt].dwIP) {
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

		if (Col > 0 && Col < 5) {
			nCnt = ChatBuffCnt2[0] & CHATBUFF_MASK;
			memcpy(&ChatBuff2[0][nCnt], &ChatBuff[mCnt], sizeof(CHATBUFF));
			ChatBuffCnt2[0]++;
		}

		if (Col == 0 && ChatBuff[mCnt].dwIP < 10) {
			for (cnt2 = 1; cnt2 < 5; cnt2++) {
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
int DisplayChatMessage3(HDC hdc, int x, int y, int StrMax, int LineMax, int Mode) { return TRUE; }

//Ã¤ÆÃ Á¤º¸ È­¸é¿¡ Ç¥½Ã
int DisplayChatMessage2(HDC hdc, int x, int y, int StrMax, int LineMax) { return TRUE; }


int ConnectPlayer(smWINSOCK *lpsmSock)
{

	Utils_Log(LOG_DEBUG, "(%s) ConnectPlayer", lpsmSock->szIPAddr);
	int OthPlayerCnt;

	if (ServerMode)
		return Serv_ConnectPlayer(lpsmSock);


	//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
	EnterCriticalSection(&cSection);

	OthPlayerCnt = FindNewOtherPlayer();
	if (OthPlayerCnt >= 0)
	{
		lpsmSock->ExtData1 = (void *)&chrOtherPlayer[OthPlayerCnt];
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
int DisconnectPlayer(smWINSOCK *lpsmSock)
{
	//	int cnt;
	smCHAR *lpPlayer;
	DWORD	dwTime;

	dwTime = GetCurrentTime();

	if (ServerMode)
		return Serv_DisconnectPlayer(lpsmSock);

	if (smWsockServer && lpsmSock->sock == smWsockServer->sock)
	{
		AddChatBuff("Connection lost to Server");
		if (smWsockServer == smWsockDataServer)
			smWsockDataServer = 0;
		if (smWsockServer == smWsockUserServer)
			smWsockUserServer = 0;
		if (smWsockServer == smWsockExtendServer)
			smWsockExtendServer = 0;

		smWsockServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 14");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 1;
		//°Å·¡ÁßÀÌ¸é °Å·¡ Ãë¼Ò ½ÃÅ´
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//°Å·¡ Ãë¼Ò ¿ä±¸
			cTrade.CancelTradeItem();								//°Å·¡Ã¢ ´Ý±â
		}
		return TRUE;
	}

	if (smWsockDataServer && lpsmSock->sock == smWsockDataServer->sock)
	{
		AddChatBuff("Connection lost to Dataserver");
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
		AddChatBuff("Connection lost to Server");
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
		AddChatBuff("Connection lost to Server");
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
				wsprintf(szBuff, "Disconnect Area Server (%s)", lpWSockServer_Area[0]->szIPAddr);
				AddChatBuff(szBuff, 0);
			}
			if (lpWSockServer_Area[0] == lpWSockServer_DispArea[0]) lpWSockServer_DispArea[0] = 0;
			if (lpWSockServer_Area[0] == lpWSockServer_DispArea[1]) lpWSockServer_DispArea[1] = 0;

			lpWSockServer_Area[0] = 0;
			return TRUE;
		}
		if (lpWSockServer_Area[1] && lpWSockServer_Area[1]->sock == lpsmSock->sock) {
			if (smConfig.DebugMode) {
				wsprintf(szBuff, "Disconnect Area Server (%s)", lpWSockServer_Area[1]->szIPAddr);
				AddChatBuff(szBuff, 0);
			}

			if (lpWSockServer_Area[1] == lpWSockServer_DispArea[0]) lpWSockServer_DispArea[0] = 0;
			if (lpWSockServer_Area[1] == lpWSockServer_DispArea[1]) lpWSockServer_DispArea[1] = 0;

			lpWSockServer_Area[1] = 0;
			return TRUE;
		}
	}



	//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
	EnterCriticalSection(&cSection);

	lpPlayer = (smCHAR *)lpsmSock->ExtData1;
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
int SendPlayDataToServer() { return TRUE; }


//´Ù¸¥ À¯Àú¿Í ¿¬°á ½ÃÅ²´Ù
int ConnectOtherPlayer(DWORD dwIP)
{
	smWINSOCK *lpsmSock;
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
//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°á ¾²·¹µå
DWORD WINAPI ConnectPlayThreadProc(void *pInfo)
{

	ConnectOtherPlayer((DWORD)pInfo);

	ExitThread(TRUE);
	return TRUE;
}
//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°á Á¾·á ¾²·¹µå
DWORD WINAPI DisconnectPlayThreadProc(void *pInfo)
{

	((smWINSOCK *)pInfo)->CloseSocket();

	ExitThread(TRUE);
	return TRUE;
}



//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°á ¾²·¹µå¸¦ »ý¼º
int CreateConnectThread(DWORD dwIP)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, ConnectPlayThreadProc, (void *)dwIP, 0, &dwMsgId);

	return TRUE;
}

//´Ù¸¥ ÇÃ·¹ÀÌ¾î¿ÍÀÇ ¿¬°áÁ¾·á ¾²·¹µå »ý¼º
int CreateDisconnectThread(smWINSOCK *lpsmSock)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, DisconnectPlayThreadProc, (void *)lpsmSock, 0, &dwMsgId);

	return TRUE;
}


//°Å¸®°¡ ¸Õ ´Ù¸¥ À¯ÀúµéÀÇ ¿¬°áÀ» ²ö´Â´Ù
int DisconnectFarPlayer()
{
	int mx, mz, dDist;
	int cnt;
	int timeover;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		//°Å¸®¸¦ È®ÀÎÇÏ¿© ¸Õ °æ¿ì¿¡´Â ¿¬°áÀ» ²ö¾î¹ö¸°´Ù
		if (chrOtherPlayer[cnt].Flag) {

			if ((chrOtherPlayer[cnt].dwLastTransTime + DIS_TIME_OVER) < dwPlayTime)
				timeover = TRUE;
			else
				timeover = FALSE;

			//°Å¸®°è»ê
			mx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
			mz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
			dDist = mx*mx + mz*mz;

			if ((chrOtherPlayer[cnt].DisplayFlag && dDist > DIST_TRANSLEVEL_DISCONNECT) || timeover) {
				//¿¬°á Á¾·á/ ¼ÒÄÏ ´ÝÀ½
				chrOtherPlayer[cnt].Flag = 0;

				if (chrOtherPlayer[cnt].TransSock) {
					chrOtherPlayer[cnt].TransSock->CloseSocket();
				}
				else {
					//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
					EnterCriticalSection(&cSection);
					chrOtherPlayer[cnt].Close();
					//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
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

//µ¥ÀÌÅ¸ ÀÔ¼ö
int RecvPlayData(smTHREADSOCK *pData)
{

	DWORD *IPData;
	//smCHAR *lpPlayer;

	DWORD	dwTime;

	//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
	EnterCriticalSection(&cSection);


	IPData = (DWORD *)(pData->Buff);


	/*
		if ( pData->smMySock==smWsockServer ||
			 pData->smMySock==smWsockUserServer ||
			 pData->smMySock==smWsockDataServer ||
			  pData->smMySock==smWsockExtendServer ) {
	*/
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

	//¼­¹öÃø µ¥ÀÌÅ¸ Ã³¸®
	rsMainServer.RecvMessage(pData);
	/*
		}
		else {
			//Å¬¶óÀÌ¾ðÆ®¿ÍÀÇ ±³½Å..

			lpPlayer = (smCHAR *)pData->smMySock->ExtData1;
			if ( lpPlayer ) lpPlayer->RecvPlayData( pData );
		}
	*/
	//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
	LeaveCriticalSection(&cSection);

	return TRUE;
}

//¸Þ¼¼Áö Å¥ÀÇ ¸Þ¼¼Áö¸¦ Ã³¸®
int PlayRecvMessageQue()
{
	return rsMainServer.RecvMessageQue();
}


int LastSendCnt;
/*
int SendPlayData( smCHAR *player )
{
	int cnt;

	for(cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].Pattern
			&& chrOtherPlayer[cnt].AutoPlayer==0 ) {
			chrOtherPlayer[cnt].SendPlayData( player );
		}
	}

	return TRUE;
}
*/
#define	NET_SENDING_WAIT_TIME		400
#define	NET_SENDING_WAIT_TIME2		1000

int NetStandCnt = 0;
extern int GameMode;

DWORD	NetScoopTime = 2000;

//³Ý ÇÃ·¹ÀÌ ¸ÞÀÎ
int NetWorkPlay()
{
	//	char *szIP;
	int cnt;
	DWORD	dwTime;
	DWORD	dwTime2;
	smWINSOCK	*lpsmSock;

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
		if (cTrade.OpenFlag) {
			//¾ÆÀÌÅÛ ±³È¯½Ã ¿Ï·á È®ÀÎ
			if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag) {
				SendTradeCheckItem(cTrade.TradeCharCode);
				TradeSendSucessFlag = TRUE;
			}
		}
		if (dwTradeMaskTime) {
			//Æ®·¹ÀÌµå ¸¶½ºÅ© Å¸ÀÌ¸Ó ÃÊ±âÈ­
			if (dwTradeMaskTime < dwPlayTime) dwTradeMaskTime = 0;
		}

		dwTime = dwPlayTime - dwLastSendPosiTime;
		dwTime2 = dwPlayTime - dwRecvServerTime;

		//¼­¹ö·ÎÀÇ µ¥ÀÌÅ¸ Àü¼Û
		if ((PlayCounter - Ts_LastSendCounter) > PLAYSERVER_SENDCOUNTER) {//&& dwTime>NET_SENDING_WAIT_TIME &&
//			dwTime2<NET_SENDING_WAIT_TIME ) {
			//¼­¹ö·Î ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸¸¦ À§Ä¡¸¦ Àü¼Û
			SendPlayDataToServer();
			//°Å¸®°¡ ¸Õ ´Ù¸¥ À¯ÀúµéÀÇ ¿¬°áÀ» ²ö´Â´Ù
			DisconnectFarPlayer();

			//Ä«¿îÅÍ Áõ°¡
			Ts_LastSendCounter = PlayCounter;
			dwLastSendPosiTime = dwPlayTime;
		}

		//		if ( (xxcnt&0x3)==3 ) SendPlayData( lpCurPlayer );

				//´Ù¸¥ À¯Àúµé°úÀÇ µ¥ÀÌÅ¸ ±³È¯
				//SendPlayData( lpCurPlayer );

		if (smWsockUserServer) {// && rsRecvUserServer ) {
			//ÇÃ·¹ÀÌ¾î µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î Àü¼Û (40/70) ÃÊ´ÜÀ§ ±¸ºÐ 
			//if ( (lpCurPlayer->PlayBuffCnt&0x3F)==0 ) {

			dwTime = dwPlayTime - dwLastSendPlayTime;
			dwTime2 = dwPlayTime - dwRecvUserServerTime;


			if ((lpCurPlayer->PlayBuffCnt & 0x3F) == 0 && lpCurPlayer->dwObjectSerial &&
				dwTime > NET_SENDING_WAIT_TIME) {//&& dwTime2<NET_SENDING_WAIT_TIME2 ) {

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
						if (((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->PlayBuffCnt > 1) {
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send(lpCurPlayer->srTransBuff, ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->size, TRUE);
							NetStandCnt = 0;
						}
						else {
							//¼­ÀÖ´Â µ¿ÀÛ ³×Æ®¿÷ ºÎÇÏ ÃÖ¼Ò ( ÇÑ¹ø Àü¼ÛÈÄ ³×Æ®Ÿp ½½¸³ )
							//if ( NetStandCnt==0 )
							//for(int cnt=0;cnt<40;cnt++) 
							lpsmSock->Send(lpCurPlayer->srTransBuff, ((smTRANS_PLAYDATA *)lpCurPlayer->srTransBuff)->size, TRUE);
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
						AddChatBuff("Scoop Recv Route(Main)");
				}
				else {
					if (smConfig.DebugMode)
						AddChatBuff("Error Server(Main)");
				}
			}
			if (GameMode == 2 && smWsockUserServer && dwUserServ_RecvTime &&
				smWsockServer != smWsockUserServer && (dwTime - dwUserServ_RecvTime) > 3000) {
				//dwUserServ_RecvTime = dwTime;
				if (smWsockUserServer->WaitReadMessage >= 0) {
					smWsockUserServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						AddChatBuff("Scoop Recv Route(User)");
				}
				else {
					if (smConfig.DebugMode)
						AddChatBuff("Error Server(User)");
				}
			}
			if (GameMode == 2 && smWsockExtendServer && dwExtendServ_RecvTime &&
				smWsockServer != smWsockExtendServer && (dwTime - dwExtendServ_RecvTime) > 3000) {
				//dwUserServ_RecvTime = dwTime;
				if (smWsockExtendServer->WaitReadMessage >= 0) {
					smWsockExtendServer->WaitReadMessage += 24;
					if (smConfig.DebugMode)
						AddChatBuff("Scoop Recv Route(Extend)");
				}
				else {
					if (smConfig.DebugMode)
						AddChatBuff("Error Server(Extend)");
				}
			}

			//			if ( GameMode==2 && dwLoadingTime && 
			if (GameMode == 2 && ((dwTime - dwDataServ_RecvTime) > 7000 || dwLoadingTime) && smWsockDataServer && smWsockDataServer != smWsockServer)
			{
				smWsockDataServer->WaitReadMessage += 24;
				if (smConfig.DebugMode)
					AddChatBuff("Scoop Recv Route(Data)");
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

				/*
					if ( AreaServerMode ) {
						if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->Send( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
						if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->Send( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );
					}
				*/

			}

			//dwLastRecvTime


		}

		/*
			for(cnt=start;cnt<CONNECTMAX;cnt+=0x40 ) {
				if ( rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].dwObjectSerial &&//) {
					(dwPlayServTime-rsPlayInfo[cnt].dwLastRecvTime)<5000 ) {
					rsPlayInfo[cnt].lpsmSock->WaitReadMessage++;
				}
			}
		*/
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
			smWsockDataServer->Send((char *)&LastTransTradeItemKey, LastTransTradeItemKey.size, TRUE);

		dwLastTransTradeKeyTime = 0;
	}


	if (ChatDispCnt > 0) ChatDispCnt--;

	return TRUE;
}


//¾ÆÀÌÅÛ ¹ö¸®±â
int ThrowPutItem(sITEM *lpItem, int x, int y, int z)
{

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
int ThrowPutItem2(sITEMINFO *lpItem, int x, int y, int z)
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
int ThrowItemToServer(smTRANS_COMMAND_EX *lpTransCommand) { return TRUE; }


//¸ó½ºÅÍ ½ÃÀÛ ÁöÁ¡ Ãß°¡
int SendAddStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADD_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
	if (lpsmSock) {
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}

//¸ó½ºÅÍ ½ÃÀÛÁöÁ¡ Á¦°Å
int SendDeleteStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;
	scITEM	*lpItem;
	smWINSOCK *lpsmSock;


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
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}



//NPC Ä³¸¯ÅÍ Ãß°¡
int SendAdd_Npc(smCHAR *lpCharSample, int state)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;

	smTransPlayerInfo.code = smTRANSCODE_ADD_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy(&smTransPlayerInfo.smCharInfo, &lpCharSample->smCharInfo, sizeof(smCHAR_INFO));
	smTransPlayerInfo.smCharInfo.State = 0;

	//NPC Á¤º¸ÆÄÀÏ À» ¸ðµ¨ÀÌ¸§2¿¡ ³Ö¾î¼­ º¸³¿
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

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
	if (lpsmSock) {
		return lpsmSock->Send((char *)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}

	return NULL;
}

//NPC Ä³¸¯ÅÍ Á¦°Å
int SendDelete_Npc(smCHAR *lpCharSample)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;
	smWINSOCK *lpsmSock;

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
		return lpsmSock->Send((char *)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}
	return NULL;
}

//ÀúÀåµÈ Ä³¸¯ÅÍ µ¥ÀÌÅ¸ ºÒ·¯¿À±â ¿ä±¸
int SendGetRecordData(char* szName) { return TRUE; }

char *szCmdOpenMonster[10] = {
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
	char *szMonName;

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


	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
	if (lpsmSock) {
		return lpsmSock->Send((char *)&smTransCharCommand, smTransCharCommand.size, TRUE);
	}

	return FALSE;

}

//°¢ ÄÚµå µ¥ÀÌÅ¸¸¦ ¹ÝÀü
int	CodeXorCharInfo()
{
	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	return TRUE;
}

//½ºÅ³°ª ÀüºÎ È®ÀÎ
int SaveCheckSkill()
{

	ReformSkillInfo();

	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point) {
			if (cSkill.CheckSkillPointForm(&sinSkill.UseSkill[j]) == FALSE)
				return FALSE;
			if (cSkill.CheckSkillMasteryForm(&sinSkill.UseSkill[j]) == FALSE)
				return FALSE;
		}
	}

	return TRUE;
}

//½ºÅ³º¸È£°ª ÀüºÎ ÃÊ±âÈ­
int ReformSkillInfo()
{
	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		cSkill.ReformSkillMasteryForm(&sinSkill.UseSkill[j]);
		cSkill.ReformSkillPointForm(&sinSkill.UseSkill[j]);
	}
	return TRUE;
}

//°ÔÀÓÀ» ÀúÀå
int	SaveGameData()
{
	if (smWsockDataServer && !rsRecorder.dwRecordTime &&
		lpCurPlayer->smCharInfo.JOB_CODE && !AdminCharDisable && !cTrade.OpenFlag && !cWareHouse.OpenFlag && !QuitSave)
	{
		//¹°¾à°¹¼ö ºñ±³
		//if ( ComparePotion()>0 ) return FALSE;		//¹°¾à ¿À·ù

		if (!WarningHack && CheckCharForm() != FALSE && SaveCheckSkill() == TRUE)
		{
			CodeXorCharInfo();
			ReformCharForm();

			rsRecorder.MakeRecordData(&lpCurPlayer->smCharInfo, cInvenTory.InvenItem, cInvenTory.InvenItemTemp, &MouseItem);
			rsRecorder.SendRecordDataToServer(smWsockDataServer);
			rsRecorder.dwRecordTime = dwPlayTime;					//°ÔÀÓ ÀúÀå ½Ã°£ È®ÀÎ

			if (smConfig.DebugMode)
				AddChatBuff("GameSave");

			CodeXorCharInfo();
			ReformCharForm();
			CheckPlaySkill();		//½ºÅ³°¨½Ã
			return TRUE;
		}
	}
	return FALSE;
}

//°ÔÀÓ ÀúÀå »óÅÂ ( FALSE ÀúÀåÁß ( ¼­¹ö·ÎºÎÅÍ °á°ú¸¦ ¹ÞÁö ¸øÇÔ ) )
int GetSaveResult()
{
	if (rsRecorder.dwRecordTime) return FALSE;

	return TRUE;
}


///////////////////// º¸³»±â °ü·Ã ////////////////////
	//smTRANSCODE_ID_GETUSERINFO:
	//ID·Î »ç¿ëÀÚ Ä³¸¯ÅÍ ¸®½ºÆ® ¹× Á¤º¸¸¦ ±¸ÇÔ

	//smTRANSCODE_INSRECORDDATA:
	//ID¿¡ »õ·Î¿î Ä³¸¯ÅÍ »ðÀÔ

	//smTRANSCODE_DELRECORDDATA:
	//ID¿¡ »õ·Î¿î Ä³¸¯ÅÍ »èÁ¦

	//smTRANSCODE_ISRECORDDATA:
	//Ä³¸¯ÅÍ°¡ Á¸Àç ÇÏ´ÂÁö È®ÀÎ

//À¯Àú Á¤º¸ °ü·Ã Åë½Å ÄÚ¸àµå
//int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )

///////////////////////////////////////////////////////

///////////////////// ¹Þ±â °ü·Ã ////////////////////
	//smTRANSCODE_ID_SETUSERINFO:
	//ID·Î »ç¿ëÀÚ Ä³¸¯ÅÍ ¸®½ºÆ® ¹× Á¤º¸¸¦ ±¸ÇÔ 
	//(TRANS_USERCHAR_INFO ±¸Á¶Ã¼·Î °á°ú µé¾î¿È )

	//smTRANSCODE_ISRECORDDATA:
	//Ä³¸¯ÅÍ°¡ Á¸Àç ÇÏ´ÂÁö È®ÀÎ 
	//( smTRANS_CHAR_COMMAND2 ->wParam ¿¡ °á°ú µé¾î¿È )
///////////////////////////////////////////////////////

smTRANS_COMMAND	smMessageCommandBuff;

//À¯Àú Á¤º¸ °ü·Ã Åë½Å ÄÚ¸àµå ( µ¥ÀÌÅ¸ ¼­¹ö¿ë )
int	TransUserCommand(DWORD dwCode, char *szID, char *szName)
{
	smTRANS_CHAR_COMMAND2	TransCharCommand2;

	TransCharCommand2.code = dwCode;
	TransCharCommand2.size = sizeof(smTRANS_CHAR_COMMAND2);

#ifdef _LANGUAGE_KOREAN
	if (dwCode == smTRANSCODE_ID_GETUSERINFO) {
		int cnt, len;

		len = lstrlen(szName);
		for (cnt = 0; cnt < len; cnt++) {
			if ((szName[cnt] & 0x80) != 0) {
				smMessageCommandBuff.code = smTRANSCODE_FAILCONNECT;
				smMessageCommandBuff.size = sizeof(smTRANS_COMMAND);
				smMessageCommandBuff.WParam = -19;			//Æ¯¼ö¹®ÀÚ ¿À·ù
				smMessageCommandBuff.LParam = 0;
				smMessageCommandBuff.EParam = 2;
				HoRecvMessage(smTRANSCODE_FAILCONNECT, &smMessageCommandBuff);
				return FALSE;
			}
		}
}
#endif

	if (szID)
		lstrcpy(TransCharCommand2.szId, szID);

	if (szName)
		lstrcpy(TransCharCommand2.szName, szName);

	return smWsockDataServer->Send((char *)&TransCharCommand2, TransCharCommand2.size, TRUE);
}

//Ä³¸¯ÅÍ Á¤º¸ Àü¼Û À¯±¸
int	Send_GetCharInfo(DWORD	dwObjectSerial, int ServerCode)
{
	smTRANS_COMMAND			TransCommand;
	smWINSOCK	*lpsmSock;

	TransCommand.code = smTRANSCODE_GETPLAYERINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;

	lpsmSock = GetServerSock(ServerCode);		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½

	if (lpsmSock) {
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}

#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)

/*
#define DEADLOCK_CHECK_LIMIT_TIME		(30*1000)
#define DEADLOCK_CHECK_LIMIT_TIME2		(60*1000)
#define RECONNECT_LIMIT_TIME			(7*1000)
*/
static DWORD dwCheckNetCount = 0;
static DWORD dwLastTransCheckTime = 0;
static int	ReconnectCount = 0;

//µ¥µå¶ô Ã½Å© ( ¸Þ¼¼Áö°¡ µ¹¾Æ ¿ÀÁö ¾ÊÀ¸¸é ³×Æ®¿÷ ´Ù¿î )
int	TransCheckNetwork()
{
	smTRANS_COMMAND			TransCommand;
	DWORD	dwTime, dwTime2;
	smWINSOCK	*lpOldSock;

	//return TRUE;

	if (!dwConnectedServerTime)
		return FALSE;

	//ÇÁ·Î¼¼½º ½Ã°£ °¨½ÃÇÏ´Â ¾²·¹µå °¡µ¿

	OpenTimeCheckThread();

	//È®ÀÎ 4ÃÊ ÀÌÈÄ¿¡³ª È®ÀÎ ¸Þ¼¼Áö º¸³¿
	dwTime = GetCurrentTime();
	dwTime2 = dwTime;

	if ((dwTime - dwLastTransCheckTime) < 4000)
		return FALSE;

	dwLastTransCheckTime = dwTime;

	//if ( !smConfig.DebugMode ) {
	if ((dwMainServ_RecvTime && (dwMainServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime) ||
		(dwLastRecvGameServerTime + DEADLOCK_CHECK_LIMIT_TIME) < dwTime ||
		!smWsockServer || smWsockServer->WaitReadMessage < 0)
	{
		//¿¬°á ²ö¾î Áü ( ¼­¹ö¿ÍÀÇ ±³½Å ºÒ´É )

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
			//Sleep(200);
			//¼­¹ö¿¡ ÀçÁ¢¼Ó ½Ãµµ
			SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockServer);

			char szBuff[256];
			wsprintf(szBuff, "Reconnect Server(Main)");
			AddChatBuff(szBuff);

			if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
			if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
			if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
			if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;
			ReconnServer++;		//Àç¿¬°á Ä«¿îÅÍ

			return TRUE;
		}

		DisconnectServerCode = 1;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 20");
		DisconnectFlag = GetCurrentTime();
		return FALSE;
	}
	//}


	//¿¹»ó ¼­¹ö ½Ã°£
	dwTime = (dwTime - dwConnectedClientTime) + dwConnectedServerTime;

	TransCommand.code = smTRANSCODE_CHECK_NETSTATE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwTime;
	TransCommand.LParam = dwCheckNetCount;
	TransCommand.SParam = GetSafeCode2() + ((dwMemError^dwPlayTime) - 8080);
	TransCommand.EParam = dwMemFunChkCode;

	dwCheckNetCount++;

	if (smWsockServer) smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockDataServer && smWsockServer != smWsockDataServer)
	{
		if (dwLastRecvGameServerTime2 && (dwLastRecvGameServerTime2 + DEADLOCK_CHECK_LIMIT_TIME2) < dwTime2)
		{
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 21");
			DisconnectFlag = GetCurrentTime();
			DisconnectServerCode = 2;
			return FALSE;
		}
		//		else
		//			dwLastRecvGameServerTime2=dwTime;


		TransCommand.code = smTRANSCODE_CHECK_NETSTATE2;
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime2) dwLastRecvGameServerTime2 = dwTime2;
	}


	if (smWsockUserServer && smWsockServer != smWsockUserServer) {
		if ((dwLastRecvGameServerTime3 && (dwLastRecvGameServerTime3 + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2) ||
			(dwUserServ_RecvTime && (dwUserServ_RecvTime + RECONNECT_LIMIT_TIME) < dwTime2) ||
			smWsockUserServer->WaitReadMessage < 0) {

			smWsockUserServer = 0;
			ConnectServer_Main();

			if (smWsockUserServer) {
				//¼­¹ö¿¡ ÀçÁ¢¼Ó ½Ãµµ
				//Sleep(200);
				SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockUserServer);

				char szBuff[256];
				wsprintf(szBuff, "Reconnect Server(User)");
				AddChatBuff(szBuff);
				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				ReconnDataServer++;		//¼­¹ö Àç¿¬°á Ä«¿îÅÍ

				return TRUE;
			}

			DisconnectServerCode = 3;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 22");
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE3;
		smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
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

				char szBuff[256];
				wsprintf(szBuff, "Reconnect Server(Extend)");
				AddChatBuff(szBuff);
				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				//ReconnDataServer++;		//¼­¹ö Àç¿¬°á Ä«¿îÅÍ

				return TRUE;
			}

			DisconnectServerCode = 3;
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 23");
			DisconnectFlag = GetCurrentTime();
			return FALSE;
		}

		TransCommand.code = smTRANSCODE_CHECK_NETSTATE4;
		smWsockExtendServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
		if (!dwLastRecvGameServerTime4) dwLastRecvGameServerTime4 = dwTime2;

	}
	if (AreaServerMode) {
		//Áö¿ª¼­¹ö µ¥µå¶ô Ã½Å©
		int cnt;
		for (cnt = 0; cnt < 2; cnt++) {
			if (lpWSockServer_Area[cnt]) {
				if ((lpWSockServer_Area[cnt]->dwDeadLockTime + DEADLOCK_CHECK_LIMIT_TIME) < dwTime2 ||
					lpWSockServer_Area[cnt]->WaitReadMessage < 0) {

					lpOldSock = lpWSockServer_Area[cnt];
					lpWSockServer_Area[cnt] = smConnectSock(lpOldSock->szIPAddr, TCP_SERVPORT);

					if (lpWSockServer_Area[cnt]) {
						SendContinueServer(lpCurPlayer->dwObjectSerial, lpWSockServer_Area[cnt]);

						char szBuff[256];
						wsprintf(szBuff, "Reconnect Server Area (%d)", cnt);
						AddChatBuff(szBuff);

					}
					else {
						lpOldSock->CloseSocket();
					}
				}

				if (lpWSockServer_Area[cnt]) {
					TransCommand.code = smTRANSCODE_CHECK_NETSTATE5;
					TransCommand.EParam = (DWORD)lpWSockServer_Area[cnt];
					lpWSockServer_Area[cnt]->Send((char *)&TransCommand, TransCommand.size, TRUE);
					//lpWSockServer_Area[cnt]->dwDeadLockTime = dwTime2;
				}
			}
		}
	}


	//¹ÞÀº °æÇèÄ¡ µ·µ¥ÀÌÅ¸ ¼öÄ¡¸¦ ¼­¹ö¿¡ º¸³»¼­ È®ÀÎ
	SendTotalExpMoney();

	CheckMaxDamage();			//ÃÖ´ë µ¥¹ÌÁö °Ë»ç


	return TRUE;
}

//¼­¹ö¿¡ °ü¸®ÀÚ ¸ðµå ¼³Á¤
int SendAdminMode(DWORD AdminNum)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	if (smWsockDataServer && smWsockServer != smWsockDataServer)
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer && smWsockDataServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer && smWsockDataServer != smWsockExtendServer)
		smWsockExtendServer->Send((char *)&TransCommand, TransCommand.size, TRUE);


	return smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//¼­¹ö¿¡ °ü¸®ÀÚ ¸ðµå ¼³Á¤
int SendAdminMode(DWORD AdminNum, smWINSOCK *lpsmSock)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADMINMODE;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = AdminNum;
	TransCommand.LParam = 0;
	TransCommand.SParam = ServerHideMode;

	return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
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
		smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	return smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//IPÁ¢¼Ó ÁÖ¼Ò¸¦ ±º ¼­¹ö¿¡ º¸³½´Ù
int SendCheckIP(int Code, smWINSOCK *lpsmSock)
{
	smTRANS_CHAR_COMMAND	TransCharCommand;

	TransCharCommand.code = smTRANSCODE_CHECKIP;
	TransCharCommand.size = sizeof(smTRANS_CHAR_COMMAND);
	TransCharCommand.WParam = Code;
	TransCharCommand.LParam = 0;
	TransCharCommand.SParam = 0;

	lstrcpy(TransCharCommand.szName, lpsmSock->szIPAddr);

	return smWsockDataServer->Send((char *)&TransCharCommand, TransCharCommand.size, TRUE);
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

	smWsockServer->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (AreaServerMode) {
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
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

	//smWsockServer->Send( (char *)&TransPlayerInfo , TransPlayerInfo.size , TRUE );

	if (smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	if (smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send((char *)&TransPlayerInfo, TransPlayerInfo.size, TRUE);

	return TRUE;
}

//¼­¹ö¿¡ ÀçÁ¢¼Ó ½Ãµµ
int SendContinueServer(DWORD dwObjectSerial, smWINSOCK *lpsmSock)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_CONT_OBJSERIAL;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
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
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}

//Å©·¢ Ã½Å©
int CheckCracker() { return TRUE; }

//Å©·¢ Ã½Å© ( ¸Þ¼¼Áö Åëº¸µµ °°ÀÌ »ç¿ë )
int CheckCracker(TRANS_FIND_CRACKER *lpTransFindCrack) { return TRUE; }

//Å©·¢ À©µµ¿ì ½Å°í
int SendCrackWindow(HWND hWnd)
{
	Utils_Log(LOG_DEBUG, "SendCrackWindow");

	char *szName;
	char TransBuff[1024];
	int len;

	//Å©·¢ ¹ß°ß
	if (smWsockServer) {

		szName = TransBuff + 8;
		GetWindowText(hWnd, szName, 32);
		len = lstrlen(szName) + 9;
		((int *)TransBuff)[0] = len;
		((int *)TransBuff)[1] = smTRANSCODE_FINDCRACK2;

		smWsockServer->Send(TransBuff, len, TRUE);
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
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

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
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

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
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

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
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	return TRUE;
}

//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
int SendSaveThrowItem(sITEMINFO *lpItemInfo)
{
	smTRANS_COMMAND	TransCommand;

	//¹°¾àÀÎ °æ¿ì¸¸ ÀúÀåÇÏµµ·Ï ¼öÁ¤
	if ((lpItemInfo->CODE&sinITEM_MASK1) != (sinPM1&sinITEM_MASK1))	return FALSE;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;

	if ((lpItemInfo->CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		ComparePotion();	//¹°¾à°¹¼ö ºñ±³

	return smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}
/*
//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ÃÖ´ë 32°³ µ¿½Ã ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
int	SendSaveThrowItems( TRANS_THROW_ITEMS	*lpTransThrowItems )
{
	if ( lpTransThrowItems->ThrowItemCount<0 ) return FALSE;
	if ( lpTransThrowItems->ThrowItemCount>THROW_ITEMS_MAX )
		lpTransThrowItems->ThrowItemCount = THROW_ITEMS_MAX;

	lpTransThrowItems->size = TRANS_THROW_ITEMS_DEFSIZE + lpTransThrowItems->ThrowItemCount * sizeof(_TRANS_THROW_ITEM);
	lpTransThrowItems->code = smTRANSCODE_SAVETHROWITEMS;

	return smWsockDataServer->Send( (char *)lpTransThrowItems , lpTransThrowItems->size , TRUE );
}
*/

//¾ÆÀÌÅÛ °Å·¡³»¿ª ¼­¹ö¿¡ º¸³»±â
int	SendRecordTradeItems(sTRADE *lpTrade)
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

	return smWsockDataServer->Send((char *)&TransRecordTradeItems, TransRecordTradeItems.size, TRUE);
}

//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
int SendSaveThrowItem2(sITEMINFO *lpItemInfo)
{
	smTRANS_COMMAND_EX	TransCommand;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM2;
	TransCommand.size = sizeof(smTRANS_COMMAND_EX);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;
	TransCommand.EParam = lpItemInfo->PotionCount^TransCommand.SParam^TransCommand.code;	//¹°¾à °¹¼ö

	TransCommand.WxParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LxParam = EncodeParam(TransCommand.WxParam);
	TransCommand.SxParam = GetTotalMoney() ^ TransCommand.LxParam;
	TransCommand.ExParam = lpItemInfo->Money;

	if ((lpItemInfo->CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		ComparePotion();	//¹°¾à°¹¼ö ºñ±³

	//ÀÌº¥Åä¸® µ· °Ë»ç ¼³Á¤
	AddInvenMoney(-lpItemInfo->Money);

	return smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//µ¥ÀÌÅ¸ ¼­¹ö¿¡ µ· ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
int SendSaveMoney()
{
	smTRANS_COMMAND	TransCommand;

	if (cWareHouse.OpenFlag && CompWareHouseMoney) return FALSE;

	TransCommand.code = smTRANSCODE_SAVETHROWMONEY;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LParam = EncodeParam(TransCommand.WParam);
	TransCommand.SParam = GetTotalMoney() ^ TransCommand.LParam;

	return smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//´Ù¸¥ Ä³¸¯ÅÍ Ä«ÇÇ Àü¼Û ¿ä±¸
int SendCopyOtherUser(char *szName, int Mode) { return TRUE; }

//¼­¹ö¿¡ º¯°æµÈ Ä³¸¯ÅÍ Á¤º¸ º¸³¿
int SendPlayUpdateInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 0;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			//·¹º§ Á¤º¸
	TransCommand.SParam = 0;
	TransCommand.EParam = 0;

	return smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//¼­¹ö¿¡ º¯°æµÈ Å¬·£ Á¤º¸ º¸³¿
int SendUpdateClanInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 1;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			//·¹º§ Á¤º¸
	TransCommand.SParam = lpCurPlayer->smCharInfo.ClassClan;
	TransCommand.EParam = lpCurPlayer->smCharInfo.bUpdateInfo[0];

	if (smWsockServer)
		smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockExtendServer && smWsockServer != smWsockExtendServer)
		smWsockExtendServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (smWsockDataServer && smWsockServer != smWsockDataServer && rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan))
		smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	if (AreaServerMode) {
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->Send((char *)&TransCommand, TransCommand.size, TRUE);
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}

	return TRUE;
}






/*
smTRANSCODE_SKIL_ATTACKDATA
#define SKIL_ATTACK_CHAR_MAX		32

struct TRANS_SKIL_ATTACKDATA {
	int size,code;

	DWORD	dwDestObjectSerial;	//°ø°ÝÇÏ´Â Ä³¸¯ÅÍ °íÀ¯¹øÈ£

	int x,y,z;			// ÁÂÇ¥
	int AttackState;	// ¼Ó¼º
	int AttackSize;		// °ø°Ý ¹üÀ§
	int Power;			// °ø°Ý ÆÄ¿ö

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//°ø°Ý´ë»ó Ä³¸¯ÅÍ °íÀ¯¹øÈ£
};
*/


TRANS_SKIL_ATTACKDATA	Back_TransSkilAttackData;

//¹üÀ§Çü °ø°Ý
int Skil_RangeAttack(int x, int y, int z, int range, int power, int TargetState)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	//int	accuracy;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;


	//Å¬¶óÀÌ¾ðÆ® ·Î±× ÆÄÀÏ ±â·Ï
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
			dist = dx*dx + dy*dy + dz*dz;

			if (dist < dRange) {
				//accuracy = sinGetAccuracy( lpChar->smCharInfo.Level , lpChar->smCharInfo.Defence );
				//if ( (rand()%100)<accuracy ) {

				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

				//Ã³À½ °ø°ÝÇÏ´Â Ä³¸¯ÇÑÅ×¸¸ Ã¼·Â¹Ù Ãâ·Â
				if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
					lpChar->EnableStateBar = TRUE;
				}
				//}
			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt + 16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = range;
	TransSkilAttackData.Power = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// Á¶ÀÛ ¹æÁö ÄÚµå

	LastAttackDamage = power;

	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));		//¹é¾÷

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â

	if (lpsmSendSock && mCnt > 0) {
		//º¸³½ °ø°Ý·Â ±â·Ï
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	if (PkMode) {
		//¹üÀ§Çü °ø°Ý( À¯Àú¸¸ °ø°Ý )
		Skil_RangeAttack_User(x, y, z, range, power, TargetState);
	}

	return TRUE;
}


//¹üÀ§Çü °ø°Ý( À¯Àú¸¸ °ø°Ý )
int Skil_RangeAttack_User(int x, int y, int z, int range, int power, int TargetState)
{
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	//int	accuracy;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;

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
			dist = dx*dx + dy*dy + dz*dz;

			if (dist < dRange) {

				TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

			}
		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKUSER;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt + 16;

	TransSkilAttackData.x = x;
	TransSkilAttackData.y = y;
	TransSkilAttackData.z = z;

	TransSkilAttackData.AttackState = TargetState;
	TransSkilAttackData.AttackSize = range;
	TransSkilAttackData.Power = power;

	TransSkilAttackData.dwChkSum = TransSkilAttackData.Power * 2002 +
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);// Á¶ÀÛ ¹æÁö ÄÚµå


	if (mCnt > 0) {
		//º¸³½ °ø°Ý·Â ±â·Ï
		Record_SendDamage(smWsockUserServer, TransSkilAttackData.Power);

		smWsockUserServer->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}


	//	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â

	//	if ( lpsmSendSock ) {
	//		lpsmSendSock->Send( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	//	}
	return TRUE;
}

//¹üÀ§Çü °ø°Ý
int Skil_RangeBoxAttack(smCHAR *lpCurChar, RECT *lpRect, int power, int TargetState, int UseAttackRating)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	int dx, dy, dz;
	int Attack, accuracy;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;


	//Å¬¶óÀÌ¾ðÆ® ·Î±× ÆÄÀÏ ±â·Ï
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

			// ÀÌµ¿µÈ Áö¿ª ÁÂÇ¥°ªÀ» ±¸ÇÏ´Â °ÍÀÌ±¸³ª! ³î¶ø´Ù.
			GetMoveLocation(dx, dy, dz, 0, (-lpCurChar->Angle.y)&ANGCLIP, 0);

			if (GeoResult_X > lpRect->left && GeoResult_X<lpRect->right  &&
				GeoResult_Z>lpRect->top && GeoResult_Z < lpRect->bottom && abs(dy) < 256) {

				Attack = TRUE;

				if (UseAttackRating) {
					accuracy = sinGetAccuracy(chrOtherPlayer[cnt].smCharInfo.Level, chrOtherPlayer[cnt].smCharInfo.Defence);
					if ((rand() % 100) > accuracy)
						Attack = FALSE;
				}

				if (Attack) {
					TransSkilAttackData.dwTarObjectSerial[mCnt++] = chrOtherPlayer[cnt].dwObjectSerial;

					//Ã³À½ °ø°ÝÇÏ´Â Ä³¸¯ÇÑÅ×¸¸ Ã¼·Â¹Ù Ãâ·Â
					if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
						lpChar->EnableStateBar = TRUE;
					}
				}
			}

		}
	}

	TransSkilAttackData.TargetCount = mCnt;

	TransSkilAttackData.code = smTRANSCODE_SKIL_ATTACKDATA;
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*mCnt + 16;

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

		lpsmSendSock->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}
	return TRUE;
}


int Skill_CharRangeEffect(DWORD dwSkillCode, smCHAR *lpCurChar, smCHAR *lpCharTarget, int point) { return TRUE; }


//¹üÀ§Çü °ø°Ý ÀÌÆåÆ® ¿¬Ãâ
int Skil_RangeBoxEffect(smCHAR *lpCurChar, RECT *lpRect, DWORD dwSkillCode, int TargetState) { return TRUE; }

//¹üÀ§Çü °ø°Ý ÀÌÆåÆ® ¿¬Ãâ
int Skil_RangeEffect(smCHAR *lpCurChar, int Range, DWORD dwSkillCode, int TargetState, int point) { return TRUE; }




//ÄÚµå ¾ÏÈ£ Ã½Å©¼¶ ¸¸µé±â
DWORD EncodeParam(DWORD Param)
{
	return (Param + (Param << 4)) * 2002;
}



//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ 
int QuickSendTransAttack(smCHAR *lpChar, int power, int AttackState, int Resistance)
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK *lpsmSendSock = 0;
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

		return lpsmSendSock->Send((char *)&TransAttackData, TransAttackData.size, TRUE);
	}

	return NULL;
}

//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ 
int QuickSendTransAttack(smCHAR *lpChar, int power)
{
	return QuickSendTransAttack(lpChar, power, 1, 0);
}
//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ 
int QuickSendTransAttack(smCHAR *lpChar, int power, int Resistance)
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
		return smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);

	return FALSE;
}

//¾ÆÀÌÅÛ ±³È¯ Á¤º¸ Àü¼Û
int SendTradeItem(sTRADE *lpTrade, DWORD dwSender)
{
	int cnt;
	int len;
	int	PotionError = 0;
	TRANS_TRADEITEMS TransTradeItems;
	sTRADE	sTradeBuff;

	len = 0;

	//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ È®ÀÎ
	if (CheckTrade((void *)lpTrade) == FALSE) {
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
	len = EecodeCompress((BYTE *)&sTradeBuff, TransTradeItems.TradeBuff, sizeof(sTRADE));
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
		return smWsockServer->Send((char *)&TransTradeItems, TransTradeItems.size, TRUE);

	return FALSE;
}

//¾ÆÀÌÅÛ ±³È¯ Á¤º¸ ¼ö½Å 
int RecvTradeItems(TRANS_TRADEITEMS *lpTransTradeItems, sTRADE *lpTrade)
{
	//¾ÐÃà µ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE *)lpTrade);
	memcpy(&TransLastRecvTrade, lpTransTradeItems, lpTransTradeItems->size);		//¹ÞÀº Æ®·¹ÀÌµå µ¥ÀÌÅ¸ ÀÓ½Ã º¹»ç
	return TRUE;
}

//±³È¯Ã¢ ¾ÆÀÌÅÛ ¼Û½Å
int SendTradeItem(DWORD dwSender)
{
	return SendTradeItem(&sTrade, dwSender);
}
//±³È¯Ã¢ ¾ÆÀÌÅÛ ¼ö½Å
int RecvTradeItems(TRANS_TRADEITEMS *lpTransTradeItems)
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
int SendTradeSucessKey(sTRADE *lpTrade, DWORD dwSender)
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

			if ((TransTradeItemKey.dwItemCode[cnt] & sinITEM_MASK1) == (sinPM1&sinITEM_MASK1)) {
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
					smWsockServer->Send( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;

				if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
					smWsockUserServer->Send( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;
		*/
		if (smWsockDataServer)
			smWsockDataServer->Send((char *)&TransTradeItemKey, TransTradeItemKey.size, TRUE);


		//¾ÆÀÌÅÛ ±³È¯Å°
		memcpy(&LastTransTradeItemKey, &TransTradeItemKey, sizeof(TRANS_TRADE_ITEMKEY));
		dwLastTransTradeKeyTime = dwPlayTime;
	}

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}


//¾ÆÀÌÅÛ ±³È¯ ÀÎÁõÅ° ¼ö½Å
int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY *lpTransTradeItemKey, sTRADE *lpTrade)
{
	int cnt;
	sTRADE sChkTrade;
	DWORD	dwChkSum1, dwChkSum2;
	int	ItemFlag;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (TransLastRecvTrade.code) {
		//¸¶Áö¸· ¹ÞÀº µ¥ÀÌÅ¸·Î ¾ÐÃà µ¥ÀÌÅ¸ ´Ù½Ã ÇØµ¶ ( Z/NZ ¹æ½Ä )
		DecodeCompress(TransLastRecvTrade.TradeBuff, (BYTE *)&sChkTrade);
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			memcpy(&lpTrade->TradeItem[cnt].sItemInfo, &sChkTrade.TradeItem[cnt].sItemInfo, sizeof(sITEMINFO));
		}
	}
	//cTrade.LoadTradeItemIamge();

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (lpTrade->TradeItem[cnt].Flag) {

			dwChkSum1 = lpTransTradeItemKey->dwItemSum[cnt];
			dwChkSum2 = lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;
			if ((lpTrade->TradeItem[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1)) {
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
		smWsockServer->Send((char *)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}

//¾ÆÀÌÅÛ ±³È¯ ³»¿ë È®ÀÎ ¼ö½Å
int RecvTradeCheckItem(TRANS_TRADE_CHECKITEM *lpTransTradeCheckItem)
{
	int cnt;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (DisconnectFlag)
		return FALSE;

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (sTrade.TradeItem[cnt].Flag) {
			if (lpTransTradeCheckItem->dwRecvItemCode[cnt] != sTrade.TradeItem[cnt].sItemInfo.CODE ||
				((lpTransTradeCheckItem->dwRecvItemCode[cnt] & sinITEM_MASK1) != (sinPM1&sinITEM_MASK1) &&
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
				((lpTransTradeCheckItem->dwSendItemCode[cnt] & sinITEM_MASK1) != (sinPM1&sinITEM_MASK1) &&
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



int SendJoinPartyUser(DWORD dwObjectCode)
{
	return TRUE;
}

int	PartyButton(int Num, int Button) { return TRUE; }





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

//Ã¢°í ÀúÀå
int	SaveWareHouse(sWAREHOUSE *lpWareHouse, TRANS_WAREHOUSE *lpTransWareHouse)
{
	TRANS_WAREHOUSE	TransWareHouse;
	//TRANS_THROW_ITEMS	TransThrowItems;
	sWAREHOUSE	WareHouseCheck;
	int	CompSize;
	int cnt;
	DWORD	dwChkSum;
	char	*szComp1, *szComp2;
	int	flag;

	if (lpTransWareHouse) flag = 1;
	else flag = 0;

	if (!flag && QuitSave)	return FALSE;

	for (cnt = 0; cnt < 100; cnt++) {
		if (!lpWareHouse->WareHouseItem[cnt].Flag) {
			ZeroMemory(&lpWareHouse->WareHouseItem[cnt], sizeof(sITEM));
		}
	}


	CompSize = EecodeCompress((BYTE *)lpWareHouse, (BYTE *)TransWareHouse.Data, sizeof(sWAREHOUSE), sizeof(sITEM) * 100);


	//¿ë·® Ã½Å©
	if (!flag && CompSize > (smSOCKBUFF_SIZE - 256))
		return FALSE;

	if (flag && CompSize > (smSOCKBUFF_SIZE - 140))
		return FALSE;


	DecodeCompress((BYTE *)TransWareHouse.Data, (BYTE *)&WareHouseCheck, sizeof(sWAREHOUSE));

	szComp1 = (char *)lpWareHouse;
	szComp2 = (char *)&WareHouseCheck;

	dwChkSum = 0;

	for (cnt = 0; cnt < sizeof(sWAREHOUSE); cnt++) {
		if (szComp1[cnt] != szComp2[cnt]) {
			return FALSE;
		}
		else {
			dwChkSum += szComp1[cnt] * (cnt + 1);
		}
	}


	//Ã¢°í°¡ ¿­·ÈÀ»¶§¿Í ´ÝÇûÀ»¶§ÀÇ µ·ÀÇ Â÷¸¦ °Ë»çÇÏ±â À§ÇÑ º¯¼ö
	WareHouseSubMoney += (CompWareHouseMoney - lpWareHouse->Money);
	CompWareHouseMoney = 0;
	/*
		if ( !flag ) {
			//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
			SendSaveMoney();				//µ·

			TransThrowItems.ThrowItemCount = 0;
			for( cnt=0;cnt<THROW_ITEMS_MAX;cnt++) {
				if ( lpWareHouse->WareHouseItem[cnt].Flag ) {
					TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwCode   = lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;
					TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwHead   = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head;
					TransThrowItems.ThrowItems[TransThrowItems.ThrowItemCount].dwChkSum = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum;
					TransThrowItems.ThrowItemCount++;
				}
			}
			//µ¥ÀÌÅ¸ ¼­¹ö¿¡ ¾ÆÀÌÅÛ ÀúÀå ¿ä±¸ ÃÖ´ë 32°³ µ¿½Ã ( º¹»ç ¾ÆÀÌÅÛ ¹æÁö )
			SendSaveThrowItems( &TransThrowItems );
		}
	*/

	TransWareHouse.code = smTRANSCODE_WAREHOUSE;
	TransWareHouse.size = sizeof(TRANS_WAREHOUSE) - (sizeof(sWAREHOUSE) - CompSize);
	TransWareHouse.DataSize = CompSize;
	TransWareHouse.dwChkSum = dwChkSum;
	TransWareHouse.wVersion[0] = Version_WareHouse;
	TransWareHouse.wVersion[1] = 0;

	if (!flag) {
		//µ· ÀúÀå ( ¼­¹ö¿¡¼­ »ç¿ëÇÒ ³»¿ë / ºÒ·¯¿Ã¶§´Â ¼­¹ö¿¡¼­ ÃÊ±âÈ­ ÇØÁà¾ß ÇÔ )
		TransWareHouse.WareHouseMoney = lpWareHouse->Money ^ (dwChkSum^smTRANSCODE_WAREHOUSE);
		TransWareHouse.UserMoney = lpCurPlayer->smCharInfo.Money ^ (dwChkSum^smTRANSCODE_WAREHOUSE);
	}

	TransWareHouse.dwTemp[0] = 0;
	TransWareHouse.dwTemp[1] = 0;
	TransWareHouse.dwTemp[2] = 0;
	TransWareHouse.dwTemp[3] = 0;
	TransWareHouse.dwTemp[4] = 0;

	dwLastWareHouseChkSum = dwChkSum;				//ÃÖÁ¾ ÀúÀå Ã¢°í ÄÚµå

	if (flag) {
		memcpy(lpTransWareHouse, &TransWareHouse, sizeof(TRANS_WAREHOUSE));
		return TRUE;
	}

	if (TransWareHouse.size > smSOCKBUFF_SIZE) TransWareHouse.size = smSOCKBUFF_SIZE;

	if (smWsockDataServer) {
		smWsockDataServer->Send((char *)&TransWareHouse, TransWareHouse.size, TRUE);

		SaveGameData();						//°ÔÀÓÀ» ÀúÀå
		return TRUE;
	}

	return FALSE;
}

int	SaveCaravan2(sCARAVAN* lpWareHouse, TRANS_CARAVAN* lpTransWareHouse)
{
	TRANS_CARAVAN	TransWareHouse;
	//TRANS_THROW_ITEMS	TransThrowItems;
	sCARAVAN	WareHouseCheck;
	int	CompSize;
	int cnt;
	DWORD	dwChkSum;
	char* szComp1, * szComp2;
	int	flag;

	if (lpTransWareHouse) flag = 1;
	else flag = 0;

	if (!flag && QuitSave)	return FALSE;

	for (cnt = 0; cnt < 100; cnt++) {
		if (!lpWareHouse->CaravanItem[cnt].Flag) {
			ZeroMemory(&lpWareHouse->CaravanItem[cnt], sizeof(sITEM));
		}
	}


	CompSize = EecodeCompress((BYTE*)lpWareHouse, (BYTE*)TransWareHouse.Data, sizeof(sCARAVAN), sizeof(sITEM) * 100);


	//¿ë·® Ã½Å©
	if (!flag && CompSize > (smSOCKBUFF_SIZE - 256))
		return FALSE;

	if (flag && CompSize > (smSOCKBUFF_SIZE - 140))
		return FALSE;


	DecodeCompress((BYTE*)TransWareHouse.Data, (BYTE*)&WareHouseCheck, sizeof(sCARAVAN));

	szComp1 = (char*)lpWareHouse;
	szComp2 = (char*)&WareHouseCheck;

	dwChkSum = 0;

	for (cnt = 0; cnt < sizeof(sCARAVAN); cnt++) {
		if (szComp1[cnt] != szComp2[cnt]) {
			return FALSE;
		}
		else {
			dwChkSum += szComp1[cnt] * (cnt + 1);
		}
	}

	TransWareHouse.code = smTRANSCODE_CARAVAN;
	TransWareHouse.size = sizeof(TRANS_CARAVAN) - (sizeof(sCARAVAN) - CompSize);
	TransWareHouse.DataSize = CompSize;
	TransWareHouse.dwChkSum = dwChkSum;
	TransWareHouse.wVersion[0] = Version_WareHouse;
	TransWareHouse.wVersion[1] = 0;

	TransWareHouse.dwTemp[0] = 0;
	TransWareHouse.dwTemp[1] = 0;
	TransWareHouse.dwTemp[2] = 0;
	TransWareHouse.dwTemp[3] = 0;
	TransWareHouse.dwTemp[4] = 0;

	//dwLastWareHouseChkSum = dwChkSum;

	if (flag) {
		memcpy(lpTransWareHouse, &TransWareHouse, sizeof(TRANS_CARAVAN));
		return TRUE;
	}

	if (TransWareHouse.size > smSOCKBUFF_SIZE) TransWareHouse.size = smSOCKBUFF_SIZE;

	if (smWsockDataServer) {
		smWsockDataServer->Send((char*)&TransWareHouse, TransWareHouse.size, TRUE);

		SaveGameData();
		return TRUE;
	}

	return FALSE;
}





//Ã¢°í ÀúÀå
int	SaveWareHouse(sWAREHOUSE *lpWareHouse)
{
	return SaveWareHouse(lpWareHouse, 0);
}

int	SaveCaravan2(sCARAVAN* lpWareHouse)
{
	return SaveCaravan2(lpWareHouse, 0);
}


int	LoadWareHouse(TRANS_WAREHOUSE *lpTransWareHouse, sWAREHOUSE *lpWareHouse, int flag)
{
	sWAREHOUSE	WareHouseCheck;
	int cnt;
	DWORD	dwChkSum;
	char	*szComp;
	char	szMsgBuff[128];

	if (lpTransWareHouse->DataSize == 0) {
		//ÃÊ±â°ª ¾Æ¹«°Íµµ ¾øÀ½
		ZeroMemory(lpWareHouse, sizeof(sWAREHOUSE));
		lpWareHouse->Money = 2023;
		lpWareHouse->Weight[0] = 197;
		CompWareHouseMoney = lpWareHouse->Money;
		return TRUE;
	}

	DecodeCompress((BYTE *)lpTransWareHouse->Data, (BYTE *)&WareHouseCheck);

	dwChkSum = 0;

	szComp = (char *)&WareHouseCheck;

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
			if (lpWareHouse->WareHouseItem[cnt].Flag) {

				if (!lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0)
					lpWareHouse->WareHouseItem[cnt].sItemInfo.Temp0 = lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum + lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE;


				if (!lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime) {
					lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwTime =
						(lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.Head >> 2) ^ (lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemHeader.dwChkSum << 2);
				}

				if ((lpWareHouse->WareHouseItem[cnt].sItemInfo.CODE&sinITEM_MASK2) == sinSP1) {
					if (lpWareHouse->WareHouseItem[cnt].sItemInfo.PotionCount > 1)	lpWareHouse->WareHouseItem[cnt].Flag = 0;
				}

				if (DeleteEventItem_TimeOut(&lpWareHouse->WareHouseItem[cnt].sItemInfo) == TRUE) {
					lpWareHouse->WareHouseItem[cnt].Flag = 0;
					wsprintf(szMsgBuff, mgItemTimeOut, lpWareHouse->WareHouseItem[cnt].sItemInfo.ItemName);
					AddChatBuff(szMsgBuff, 0);
				}
			}
		}
		return TRUE;
	}

	return FALSE;
}


int	LoadWareHouse(TRANS_WAREHOUSE *lpTransWareHouse, sWAREHOUSE *lpWareHouse)
{
	return LoadWareHouse(lpTransWareHouse, lpWareHouse, 0);
}

int SendCollectMoney(int Money)
{
	/*
		smTRANS_COMMAND	TransCommand;

		TransCommand.code =	smTRANSCODE_COLLECTMONEY;
		TransCommand.size = sizeof(smTRANS_COMMAND);
		TransCommand.WParam = Money;							//±âºÎÇÑ µ·
		TransCommand.LParam = lpCurPlayer->smCharInfo.Money;		//º¸À¯ÇÑ µ·
		TransCommand.SParam = 0;

		AddInvenMoney( -Money );				//ÀÎº¥ µ· È®ÀÎ

		if ( smWsockServer ) {
			return smWsockServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );
		}
	*/

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
		return smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}



//¹Í½ºÃÄ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendCraftItemToServer(void *lpsCraftItem_Send) { return TRUE; }

//À§ÇÁ¿ë À® ¾ÆÀÌÅÛ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendWingItemToServer(void *lpsCraftItem_Send) { return TRUE; }


//¿¡ÀÌÂ¡ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendAgingItemToServer(void *lpsCraftItem_Send) { return TRUE; }

//¸µÅ©ÄÚ¾î µî·Ï º¸³¿
int SendLinkCoreToServer(sITEMINFO *lpItem) { return TRUE; }



//¿¡ÀÌÂ¡ ¿Ï·áµÈ ¾ÆÀÌÅÛÀ» ¼­¹ö·Î º¸³¿
int SendAgingUpgradeItemToServer(sITEMINFO *lpItem) { return TRUE; }

int ClearInvenPotionCount()
{
	int cnt;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag && (cInvenTory.InvenItem[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1)) {
			cInvenTory.InvenItem[cnt].sItemInfo.PotionCount = 1;
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag && (cInvenTory.InvenItemTemp[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1)) {
			cInvenTory.InvenItemTemp[cnt].sItemInfo.PotionCount = 1;
		}
	}

	if (cTrade.OpenFlag) {
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			if (sTrade.TradeItem[cnt].Flag && (sTrade.TradeItem[cnt].sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1)) {
				sTrade.TradeItem[cnt].sItemInfo.PotionCount = 1;
			}
		}
	}

	if (MouseItem.Flag && (MouseItem.sItemInfo.CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		MouseItem.sItemInfo.PotionCount = 1;

	cInvenTory.ReFormPotionNum();

	SaveGameData();

	return TRUE;
}

sITEMINFO *FindItemFromCode(DWORD dwItemCode)
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



DWORD	dwLastCheckItemTime = 0;

//¾ÆÀÌÅÛÀ» ¼­¹ö¿¡ º¸³»¼­ È®ÀÎ
int	SendCheckItemToServer(sITEMINFO *lpItem) { return TRUE; }

//¾ÆÀÌÅÛ È®ÀÎ °á°ú µµÂø
int RecvCheckItemFromServer(TRANS_ITEM_CODE *lpTransItemCode)
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

//ÆÇ¸ÅÇÒ ¾ÆÀÌÅÛÀ» ¼­¹ö·Î º¸³¿
int	SellItemToServer(sITEMINFO *lpItem, int Count) { return TRUE; }





int Chk_InventPosCount = 1;
DWORD	dwCheckInvenItemTime = 0;

//ÀÎº¥Åä¸®ÀÇ ÀåÂøµÈ ¾ÆÀÌÅÛÀ» ¼­¹ö·Î º¸³»¼­ °Ë»ç
int CheckInvenItemToServer() { return TRUE; }

////////////////////////////// ¸Þ¸ð¸® Å¸ÀÓ Ã½Å© ¾²·¹µå ///////////////////////////////

HANDLE	hTimeCheckThread = 0;
int		TimeCheckCounter;

DWORD WINAPI TimeCheckThread(void *pInfo)
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
			smWsockDataServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

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
	DWORD *lpGetPrtectPoint;


	lpGetPrtectPoint = (DWORD *)FuncPoint;

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

	{ (DWORD)E_Shield_BlockRate << 1			,	((Meteo_UseMana - E_Shield_BlockRate) + 10ul) },		//½ºÅ³ Å×ÀÌºí º¸¾È

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
		return smWsockDataServer->Send((char *)&TransClientFuncPos, TransClientFuncPos.size, TRUE);

	return FALSE;
}


//¸Þ¸ð¸® °Ë»ç ¸ðµâ ¹ÞÀ½
int	RecvMemFuncData(TRANS_FUNC_MEMORY *TransFuncMem)
{
	smTRANS_COMMAND	smTransCommand;

	fnChkMem = (LPFN_CheckMem)((void *)TransFuncMem->szData);

	smTransCommand.size = 0;

	fnChkMem(TransFuncMem, &smTransCommand);

	if (smWsockDataServer && smTransCommand.size > 0)
		smWsockDataServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}


////////////////////////////// ¾ÆÀÌÅÛ ÀÓÁö ÀúÀå Å¥Ã³¸® //////////////////////////

#define	RECV_TRANSITEM_QUE_MAX			256
#define	RECV_TRANSITEM_QUE_MASK			255

TRANS_ITEMINFO	Recv_TransItemInfo_Que[RECV_TRANSITEM_QUE_MAX];

int	RecvTransItemQue_Push = 0;
int	RecvTransItemQue_Pop = 0;

//¾ÆÀÌÅÛ Å¥¿¡ ÀÓ½Ã ÀúÀå
int	PushRecvTransItemQue(TRANS_ITEMINFO *lpTransItemInfo)
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
TRANS_ITEMINFO	*FindRecvTransItemQue(DWORD dwCode, DWORD dwHead, DWORD dwChkSum)
{
	int cnt, mCnt;

	for (cnt = RecvTransItemQue_Pop; cnt < RecvTransItemQue_Push; cnt++) {
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;

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
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;
		if (Recv_TransItemInfo_Que[mCnt].Item.CODE) return FALSE;
	}

	return TRUE;
}

//»ç¿ëÇÑ ¾ÆÀÌÅÛ ÄÚµå ¼­¹ö·Î º¸³¿
int SendUseItemCodeToServer(sITEMINFO *lpItem) { return TRUE; }

//ÁÖº¯ Ä³¸¯ÅÍ¿ë ÀÌº¥Æ® ¸í·É ÄÚµå
int RecvCommandUser(smTRANS_COMMAND_EX *lpTransCommandEx) { return TRUE; }

//ÁÖº¯ Ä³¸¯ÅÍ¿¡ ÀÌº¥Æ® ¸í·É ÄÚµå º¸³»±â
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
		return smWsockUserServer->Send((char *)&smTransCommandEx, smTransCommandEx.size, TRUE);
	}
	return FALSE;
}

//Áö¿ª¼­¹ö¿¡ ¸í·É ÄÚµå Àü¼Û
int	SendCommand_AreaServer(DWORD dwCode, int wParam, int lParam, int sParam, int eParam)
{
	smTRANS_COMMAND	smTransCommand;
	smWINSOCK	*lpsmSock;

	smTransCommand.code = dwCode;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = wParam;
	smTransCommand.LParam = lParam;
	smTransCommand.SParam = sParam;
	smTransCommand.EParam = eParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock) {
		return lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
	}

	return FALSE;
}

//Áö¿ª¼­¹ö¿¡ ¸í·É ÄÚµå Àü¼Û
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
		return smWsockDataServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
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
int	Record_RecvDamage(smWINSOCK *lpsmSock, int Damage)
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
			if (lpsmSock) lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
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
			if (lpsmSock) lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
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
			if (lpsmSock) lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
		}
	}

	return TRUE;
}

//º¸³½ °ø°Ý·Â ±â·Ï
int	Record_SendDamage(smWINSOCK *lpsmSock, int Damage)
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
			if (lpsmSock) lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
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
			if (lpsmSock) lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
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
			if (lpsmSock) lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);
		}
	}

	return TRUE;
}

//ÆÄÆ¼¿ø¿¡°Ô ½ºÅ³ »ç¿ë Á¤º¸ º¸³»±â
int	SendPartySkillToServer(DWORD dwSkillCode, int SkillLevel, int Around, int wParam, int lParam, int sParam, int eParam) { return TRUE; }

//½ºÅ³ Ãë¼Ò
int SendCancelSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam) { return TRUE; }

//½ºÅ³ º¯°æ
int SendUpdateSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam) { return TRUE; }

//¼­¹ö¿¡ ½ºÅ³¼öÇà º¸³¿
int SendProcessSKillToServer(DWORD dwSkillCode, int point, int Param1, int Param2) { return TRUE; }

//ÆÄÆ¼ ½ºÅ³ ÆÐÅ¶ ¼ö½Å
int RecvPartySkillFromServer(TRANS_PARTY_SKILL *lpTransPartySkill)
{
	int cnt, cnt3;
	int time;
	int flag;
	//smCHAR *lpChar;

	switch (lpTransPartySkill->dwSkillCode) {
	case SKILL_PLAY_HOLY_VALOR:
		//È¦¸®¹ë·¯
		if (lpTransPartySkill->Point < 1 || lpTransPartySkill->Point>10) break;

		time = Holy_Valor_Time[lpTransPartySkill->Point - 1];
		for (cnt = 1; cnt < lpTransPartySkill->PartyCount; cnt++) {
			if (lpCurPlayer->dwObjectSerial == lpTransPartySkill->dwPartyUser[cnt]) {

				flag = 0;
				//if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_HOLY_BODY)!=0 ) flag ++;
				for (cnt3 = 0; cnt3 < 10; cnt3++) {
					if (ContinueSkill[cnt].Flag) {
						if (ContinueSkill[cnt].CODE == SKILL_DRASTIC_SPIRIT || ContinueSkill[cnt].CODE == SKILL_HOLY_BODY) {
							flag++;
						}
					}
				}

				if (flag) {
					//È¦¸®¹Ùµð »ç¿ëÁß ( ½ºÅ³À» Ãë¼Ò ) / ½º·¡½ºÆ½ ½ºÇÇ¸®Æ® »ç¿ëÁßÀÏ¶§µµ Ãë¼Ò
					//- Ãß°¡»çÇ× - È¦¸® ¹ë·¯¸¦ ÀÌ¹Ì »ç¿ëÁßÀÌ¸é ÀÌÆåÆ®¿¡¼­ Áßº¹ Ç¥½ÃµÇÁö ¾Êµµ·Ï Ã³¸® 
					//(³ªÁß¿¡ ½ÇÇàÇÑ ÀÌÆåÆ®·Î Àû¿ë )

					SendCancelSkillToServer(lpTransPartySkill->dwSkillCode, 0, 0, 0);
				}
				else {
					//½ºÅ³ ÀÌÆåÆ® Àû¿ë
					sSKILL sSkill;

					ZeroMemory(&sSkill, sizeof(sSKILL));
					sSkill.CODE = SKILL_HOLY_VALOR;
					sSkill.UseTime = time;
					sSkill.Flag = TRUE;
					sSkill.Point = lpTransPartySkill->Point;
					sinContinueSkillSet(&sSkill);
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
		break;

	}


	return TRUE;
}

DWORD	dwLastClanUserCode = 0;

//Å¬·£ ¸Þ¼¼Áö º¸³»±â
int SendClanJoinService(DWORD dwMsgCode, smCHAR* lpChar) { return TRUE; }

//Å¬·£ °¡ÀÔ ½ÂÀÎ ¸Þ¼¼Áö º¸³»±â
int SendJoinClan() { return TRUE; }

//Å¬·£ ¸Þ¼¼Áö ¹Þ±â
int RecvClanJoinService(smTRANS_CHAR_COMMAND2 *lpTransCharCommand)
{
	smCHAR *lpChar;

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
int	SkillPlay_DivineLightning_Select(smCHAR *lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	smWINSOCK *lpsmSendSock = 0;

	//Å¬¶óÀÌ¾ðÆ® ·Î±× ÆÄÀÏ ±â·Ï
	Record_ClinetLogFile("±¸¹öÀü °ø°ÝÆÐÅ¶ - SkillPlay_DivineLightning_Select ");
	quit = 1;
	return TRUE;

	if (lpChar != lpCurPlayer) return FALSE;

	TargetCount = 0;

	LightningCount = Divine_Lightning_Num[SkillPoint - 1];

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {

		cCount = (DivineLightning_FindCount + cnt)&OTHER_PLAYER_MASK;

		if (chrOtherPlayer[cCount].Flag && chrOtherPlayer[cCount].dwObjectSerial &&
			lpChar != &chrOtherPlayer[cCount] && chrOtherPlayer[cCount].smCharInfo.State == smCHAR_STATE_ENEMY &&
			chrOtherPlayer[cCount].smCharInfo.Brood != smCHAR_MONSTER_USER &&
			chrOtherPlayer[cCount].smCharInfo.Life[0] > 0) {

			x = (lpChar->pX - chrOtherPlayer[cCount].pX) >> FLOATNS;
			y = (lpChar->pY - chrOtherPlayer[cCount].pY) >> FLOATNS;
			z = (lpChar->pZ - chrOtherPlayer[cCount].pZ) >> FLOATNS;

			dist = x*x + y*y + z*z;

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
	TransSkilAttackData.size = (sizeof(TRANS_SKIL_ATTACKDATA) - sizeof(DWORD)*SKIL_ATTACK_CHAR_MAX);
	TransSkilAttackData.size += sizeof(DWORD)*TargetCount + 16;

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

		lpsmSendSock->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	return TRUE;
}

//µð¹ÙÀÎ ¶óÀÌÆ®´× ½ºÅ³ ¿¬Ãâ ( ÀÌÆåÆ® ½ÇÇö )
int SkillPlay_DivineLightning_Effect(smCHAR *lpChar, int SkillPoint) { return TRUE; }


//µð¹ÙÀÎ ¶óÀÌÆ®´× ½ºÅ³ ¿¬Ãâ ( ¸Ó¹Ì·Îµå )
int SkillPlay_MummyLord_Effect(smCHAR *lpChar, int Range) { return TRUE; }


//º£³ð ½ºÇÇ¾î ½ºÅ³ ¿¬Ãâ ( ÀÌÆåÆ® ½ÇÇö )
int SkillPlay_VenomSpear_Effect(smCHAR *lpChar, int SkillPoint) { return TRUE; }

//¸ó½ºÅÍ ½ºÅ³ ÀÌÆåÆ® ½ÇÇö ( ÀÌÆåÆ® ½ÇÇö )
int SkillPlay_Monster_Effect(smCHAR *lpChar, int EffectKind, int Range) { return TRUE; }




//½ºÅ³ ½ÇÇà ÆÐÅ¶ ¼ö½Å
int RecvProcessSkill(smTRANS_COMMAND *lpTransCommand) { return TRUE; }



/////////////////////////////////////////////////////////////////

//int ReadClanInfo(DWORD dwClanNum)
//CLANINFO ClanInfo[CLAN_MAX]; //Å¬·£ Á¤º¸ ÀúÀå
//
//cldata ¾øµ¥ÀÌÆ®¿ë
//void Updatecldata();

//Å¬·£ ¾÷µ¥ÀÌÆ® ¿ä±¸ ¸Þ¼¼Áö
//#define	smTRANSCODE_CLAN_UPDATE			0x50320C10


//Å¬·£¿ø Á¤º¸ Àü¼Û¿ë ÇÔ¼ö
int SendClanCommandUser(smWINSOCK *lpsmSock, DWORD	dwCode, int Param1, int Param2, CLANWONLIST *lpClanUserList, int ClanUserCount)
{

	TRANS_CLAN_COMMAND_USER	TransClanUser;
	int	cnt, len;
	char *lpBuff;
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
		((DWORD *)lpBuff)[0] = dwSpeedSum;
		lpBuff += sizeof(DWORD);
		lstrcpy(lpBuff, lpClanUserList->clanWon[cnt]);
		len = lstrlen(lpClanUserList->clanWon[cnt]);
		lpBuff += len + 1;
	}

	BuffSize = lpBuff - TransClanUser.szUserBuff;
	BuffSize += 16;

	TransClanUser.size = 32 + BuffSize;

	if (lpsmSock && ClanUserCount > 0) {
		lpsmSock->Send((char *)&TransClanUser, TransClanUser.size, TRUE);
	}

	return TRUE;
}

//Å¬·£¿ø Á¤º¸ ÇØµ¶
int	RecvClanCommand(TRANS_CLAN_COMMAND_USER *lpTransClanUser, _CLAN_USER_INFO *ClanUserInfo)// char **lpClanUserName , DWORD *dwUserSpeedSum )
{
	int cnt;
	int cnt2;
	char *lpBuff;
	char	ch;

	lpBuff = lpTransClanUser->szUserBuff;

	if (lpTransClanUser->UserCount >= CLAN_USER_MAX) return FALSE;

	for (cnt = 0; cnt < lpTransClanUser->UserCount; cnt++) {
		ClanUserInfo[cnt].dwSpeedSum = ((DWORD *)lpBuff)[0];
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
int FiltQuestItem(TRANS_ITEMINFO	*lpTransItemInfo, DWORD dwPacketCode)
{
	int cnt;


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

		return FALSE;
	}


	if ((lpTransItemInfo->Item.CODE&sinITEM_MASK2) == sinMA1 ||
		(lpTransItemInfo->Item.CODE&sinITEM_MASK2) == sinMA2) {
		//Äù½ºÆ®°¡ ¾ø´Â°æ¿ì Äù½ºÆ® ¾ÆÀÌÅÛ µé¾î¿Â °æ¿ì

		return FALSE;
	}

	return TRUE;
}

POINT GetProcessModule();

//¸ðµâ°Ë»ç ÇÏ¿© ¼­¹ö·Î º¸³»±â
int CheckProcessModule() { return TRUE; }

//ÆÈ°ï µ¥¹ÌÁö º¸³»±â
int SendFalconDamage(smCHAR *lpChar)
{
	DWORD point;
	int cnt;

	if (lpChar == lpCurPlayer && lpChar->chrAttackTarget) {
		point = ((SkillFalconPoint ^ ((DWORD)chrOtherPlayer^lpCurPlayer->dwObjectSerial)) + 1) << 8;
		point |= SKILL_PLAY_FALCON;
		cnt = dm_SendTransDamage(lpChar->chrAttackTarget, 0, 0, 0, 0, point, FALSE);
		DamageExp(lpChar->chrAttackTarget, cnt);
		return cnt;
	}
	return NULL;
}

//´í½Ì¼Òµå µ¥¹ÌÁö º¸³»±â
int SendDancingSwordDamage(smCHAR *lpChar) { return TRUE; }


//¸Þ±×³×Æ½ ½ºÇÇ¾î µ¥¹ÌÁö º¸³»±â
int SendMegneticSphereDamage(smCHAR *lpChar) { return TRUE; }

//¸Ó½ºÆç µ¥¹ÌÁö º¸³»±â
int SendMuspellDamage(smCHAR *lpChar) { return TRUE; }



//Àú·¦¿ë Æê µ¥¹ÌÁö
int SendLowLevelPetDamage(smCHAR *lpChar, int petType) { return TRUE; }


//PC¹æ¿ë Æê µ¥¹ÌÁö
int SendPCBangPetDamage(smCHAR *lpChar, int petType)
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



//¾ÆÀÌÅÛ ÅÃ¹è ¼­ºñ½º ¼ö½Å ¿ä±¸
int	SendItemExpress(DWORD	dwItemCode, char *szPassCode)
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory((char *)&smTransCommand, sizeof(smTRANS_COMMAND));
	smTransCommand.code = smTRANSCODE_ITEM_EXPRESS;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwItemCode;

	if (szPassCode)
		smTransCommand.LParam = GetSpeedSum(szPassCode);

	if (smWsockDataServer)
		return smWsockDataServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

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
	smWINSOCK *lpsmSock;

	if (lpCurPlayer->smCharInfo.ClassClan) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_YAHOO_MOTION;
		smTransCommand.WParam = dwPlayTime;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;

		lpsmSock = GetAreaServerSock();		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
		if (lpsmSock)
			lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

		return TRUE;

	}
	return FALSE;
}

//º° Æ÷ÀÎÆ® Á¤º¸¸¦ ¼­¹ö¿¡ º¸³½´Ù
int	SendStarPointToServer(int Price, int Cash) { return TRUE; }

//±âºÎÇÑµ·À» ¼­¹ö¿¡ º¸³½´Ù
int	SendGiveMoneyToServer(int Money) { return TRUE; }

//Å¬·£¸Ó´Ï Á¤º¸¸¦ ¼­¹ö¿¡ º¸³½´Ù
int	SendClanMoneyToServer(int Money, int Flag, int Kind) { return TRUE; }

int	SendClanMoneyToServer(int Money, int Flag) { return TRUE; }

//ÀÌ¿ë¿ä±Ý Á¤º¸¸¦ ¼­¹ö¿¡ º¸³½´Ù
int	SendPaymentMoneyToServer(int Money, int Flag) { return TRUE; }



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
int	Send_ShowMyShopItem(smCHAR *lpChar)
{
	smTRANS_COMMAND	smTransCommand;
	int	result;
	smWINSOCK	*lpsmSock;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_CALL_MYSHOP;
	smTransCommand.WParam = 0;
	smTransCommand.LParam = lpChar->dwObjectSerial;
	smTransCommand.SParam = lpCurPlayer->dwObjectSerial;
	smTransCommand.EParam = 0;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

	return result;
}

//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë »ó´ë¹æ¿¡ º¸³»±â
int	Send_MyShopItemList(smTRANS_COMMAND	*lpTransCommand)
{
	int	result;
	smWINSOCK	*lpsmSock;

	TransTradeItems_MyShop.dwRecver = lpTransCommand->SParam;

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send((char *)&TransTradeItems_MyShop, TransTradeItems_MyShop.size, TRUE);

	return result;
}


//°³ÀÎ»óÁ¡ ¾ÆÀÌÅÛ ³»¿ë ¹ÞÀ½
int	Recv_MyShopItemList(TRANS_TRADEITEMS *lpTransTradeItems) { return TRUE; }


//°³ÀÎ »óÁ¡ ¹°°Ç ±¸ÀÔ
int	Send_PersonalShopItem(DWORD dwCharCode, void *lpShopItem) { return TRUE; }

//°³ÀÎ »óÁ¡ ¾ÆÀÌÅÛ Á¤º¸ ¾÷µ¥ÀÌÆ®
int	UpdateMyShopList(void *lpMyShop) { return TRUE; }

//°³ÀÎ »óÁ¡ ¿ÀÇÂ
int SendOpenPersonalTrade(char *szTradeMsg, void *lpPersTrade) { return TRUE; }

//»óÁ¡ ´Ý±â
int SendClosePersonalTrade()
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;

	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	TransChatMessage.code = smTRANSCODE_OPEN_MYSHOP;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send((char *)&TransChatMessage, TransChatMessage.size, TRUE);

	lpCurPlayer->dwTradeMsgCode = 0;
	lpCurPlayer->szTradeMessage[0] = 0;

	return result;
}

// Àåº° - ÀÔ·ÂÃ¢
int SendsServerDoc(char *szTradeMsg)
{
	TRANS_CHATMESSAGE	TransChatMessage;
	smWINSOCK	*lpsmSock;
	int	result = 0;


	ZeroMemory(&TransChatMessage, sizeof(TRANS_CHATMESSAGE));

	TransChatMessage.code = smTRANSCODE_ITEMDOC;
	TransChatMessage.size = sizeof(TRANS_CHATMESSAGE);
	lstrcpy(TransChatMessage.szMessage, szTradeMsg);

	lpsmSock = GetAreaServerSock();
	if (lpsmSock)
		result = lpsmSock->Send((char *)&TransChatMessage, TransChatMessage.size, TRUE);

	return result;
}



int DeleteEventItem_TimeOut(sITEMINFO	*lpsItem)
{
	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ, ½ºÅ³ ¸¶½ºÅÍ Ãß°¡
	if (lpsItem->CODE == (sinBI1 | sin32) || lpsItem->CODE == (sinBI1 | sin33) || lpsItem->CODE == (sinBI1 | sin34) || lpsItem->CODE == (sinBI1 | sin51) ||// pluto Æê(ÇØ¿Ü)
	//	lpsItem->CODE == (sinBI1|sin35) || lpsItem->CODE == (sinBI1|sin36) || lpsItem->CODE == (sinBI1|sin37) || lpsItem->CODE == (sinBI1|sin38) )	//¼öÇ¥
		lpsItem->CODE == (sinBI1 | sin36) || lpsItem->CODE == (sinBI1 | sin37) ||
		lpsItem->CODE == (sinBI1 | sin38) || lpsItem->CODE == (sinBI1 | sin39) ||
		lpsItem->CODE == (sinBI1 | sin40) || lpsItem->CODE == (sinBI1 | sin41) ||
		lpsItem->CODE == (sinBI1 | sin42) || lpsItem->CODE == (sinBI1 | sin43) ||
		lpsItem->CODE == (sinBI1 | sin44) || lpsItem->CODE == (sinBI1 | sin45) ||
		lpsItem->CODE == (sinBI1 | sin46) || lpsItem->CODE == (sinBI1 | sin47) ||
		lpsItem->CODE == (sinBI1 | sin48) || lpsItem->CODE == (sinBI1 | sin49) ||
		lpsItem->CODE == (sinBI1 | sin50) || lpsItem->CODE == (sinBI1 | sin52) || // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
		lpsItem->CODE == (sinBI1 | sin53) || lpsItem->CODE == (sinBI1 | sin54) ||

		lpsItem->CODE == (sinBI1 | sin65) || lpsItem->CODE == (sinBI1 | sin66) || // Àåº° - ºô¸µ µµ¿ì¹Ì Æê (7ÀÏ, 1ÀÏ)
		lpsItem->CODE == (sinBI1 | sin67) || lpsItem->CODE == (sinBI1 | sin68) ||
		lpsItem->CODE == (sinBI1 | sin69) || lpsItem->CODE == (sinBI1 | sin70) ||
		lpsItem->CODE == (sinBI1 | sin71) || lpsItem->CODE == (sinBI1 | sin72) ||
		lpsItem->CODE == (sinBI1 | sin55) || lpsItem->CODE == (sinBI1 | sin77) ||	// Àåº° - ÇÇ´Ð½ºÆê(1½Ã°£)
		lpsItem->CODE == (sinBI1 | sin62) || lpsItem->CODE == (sinBI1 | sin63) ||	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) ¾ÆÀÌÅÛ Ãß°¡ D, E
		lpsItem->CODE == (sinBI1 | sin64) ||										// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) ¾ÆÀÌÅÛ Ãß°¡ F
		lpsItem->CODE == (sinBI1 | sin73) || lpsItem->CODE == (sinBI1 | sin74) || // Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸® 1½Ã°£ ¿Ü 4Á¾)
		lpsItem->CODE == (sinBI1 | sin75) || lpsItem->CODE == (sinBI1 | sin76)) // Àåº° - ºô¸µ µµ¿ì¹Ì Æê (1½Ã°£)
	{
		return FALSE;
	}

	struct	tm	tm {};;
	time_t		ttm;
	DWORD		dwCreateTime;

	DWORD		dwItemCode;

	dwItemCode = lpsItem->CODE&sinITEM_MASK2;

	DWORD NowTime = GetPlayTime_T();


	if (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].Class == ITEM_CLASS_WEAPON_TWO && cInvenTory.InvenItem[sInven[0].ItemIndex - 1].ItemPosition) {
		if (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].ItemPosition == 1) {
			if (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.dwCreateTime + (60) < NowTime) {
				if (lpsItem->expireTime > 0)
					return true;
			}
		}
	}

	if (lpsItem->expireTime > 0) {
		if (lpsItem->dwCreateTime + (60 * 60 * lpsItem->expireTime) < NowTime)
			return true;
	}

	
	if (lpsItem->CODE != (sinOR2 | sin01))
		return FALSE;

#ifdef _LANGUAGE_THAI_DELETE_RING
	if (lpsItem->CODE == (sinOR2 | sin01)) return TRUE;
#endif

	tm.tm_year = 2020 - 1900;
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

int RecvSodGameInfomation(void *Info) { return TRUE; }

//Æ÷½º¿Àºê ¾ÆÀÌÅÛ ÀÔ¼ö
int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2)
{
	TRANS_ITEMINFO_GROUP	TransItemGroup;

	ZeroMemory(&TransItemGroup, sizeof(TRANS_ITEMINFO_GROUP));
	memcpy(&TransItemGroup, lpTransItemGroup2, TRANS_GROUPITEM_HEADER_SIZE);

	DecodeCompress((BYTE *)lpTransItemGroup2->szBuff, (BYTE *)TransItemGroup.sItemInfo, sizeof(sITEMINFO)*TRANS_GROUPITEM_MAX);

	//¹Ú»ó·Ä ÇÔ¼ö È£Ãâ
	//Æ÷½º ¾ÆÀÌÅÛÀ» ¹Þ´Â´Ù
	sinRecvForceOrb(TransItemGroup.sItemInfo, TransItemGroup.ItemCount);


	return TRUE;
}

//Äù½ºÆ® ¸í·É º¸³»±â
int SendQuestCommandToServer(DWORD dwQuestCode, int Param1, int Param2, int Param3) { return TRUE; }

//´ëÀü Äù½ºÆ® ½ÇÇà

//ºí·¡½ºÄ³½½ ¼³Á¤Á¤º¸ ¼­¹ö¿¡ º¸³¿
int	SendBlessCastleToServer(smTRANS_BLESSCASTLE *lpBlessCastleSetup, int Mode) { return TRUE; }

//ºí·¡½ºÄ³½½ ¼¼À² Á¤º¸ ¼­¹ö¿¡ ¿ä±¸
int	Send_GetBlessCastleTax()
{
	smTRANS_COMMAND	smTransCommand;

	ZeroMemory(&smTransCommand, sizeof(smTRANS_COMMAND));

	smTransCommand.code = smTRANSCODE_BLESSCASTLE_INFO;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.SParam = cSinSiege.GetTaxRate();

	if (smWsockDataServer)
		return smWsockDataServer->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

	return FALSE;
}
//ºí·¡½ºÄ³½½ ¼³Á¤Á¤º¸ ¼ö½Å
int RecvBlessCastInfo(void *lpPacket)
{
	smTRANS_BLESSCASTLE *lpBlessCastleSetup = (smTRANS_BLESSCASTLE *)lpPacket;
	rsUSER_LIST_TOP10	*lpClanListTop10;

	switch (lpBlessCastleSetup->smTransCommand.WParam) {
	case 1:		//¼³Á¤Á¤º¸ ¼ö½Å
		chaSiege.ShowSiegeMenu(lpBlessCastleSetup);
		break;

	case 2:		//¼øÀ§ Á¤º¸ ¼ö½Å
		lpClanListTop10 = (rsUSER_LIST_TOP10 *)lpPacket;
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
int	SendResistanceToServer() { return TRUE; }

//¼³¹®Á¶»ç °á°ú ¼­¹ö¿¡ º¸³»±â
int	SendPublicPollingToServer(int PollCode, int PollCnt, BYTE *bPollingData) { return TRUE; }


#ifdef _xTrap_GUARD
#include "XTrapSrc\\XTrap.h"
//¿¢½ºÆ®·¦ ÆÐÅ¶¼ö½Å
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock)
{
	smTRANS_XTRAP_ADDR		*lpTransXTrapAddr;
	smTRANS_XTRAP_SECURITY	*lpTransXTrapSecurity;
	UCHAR KFBuf[CREATEKF_OUT_SIZE * 2] = { 0, };

	//FILE *fp;
	//char szBuff[256];


	switch (lpPacket->WParam)
	{
	case smXTRAP_COMMAND_LOGIN:
		lpTransXTrapAddr = (smTRANS_XTRAP_ADDR *)lpPacket;
		lpTransXTrapSecurity = (smTRANS_XTRAP_SECURITY *)lpPacket;

		CreateKFEx(NULL, (PUCHAR)lpTransXTrapAddr->Addr, 2, KFBuf);

		lpTransXTrapSecurity->smTransCommand.size = sizeof(smTRANS_XTRAP_SECURITY);
		lpTransXTrapSecurity->smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
		lpTransXTrapSecurity->smTransCommand.WParam = smXTRAP_COMMAND_SECURITY;
		memcpy(&lpTransXTrapSecurity->serialkey, KFBuf, CREATEKF_OUT_SIZE * 2);
		lpsmSock->Send((char *)lpTransXTrapSecurity, lpTransXTrapSecurity->smTransCommand.size, TRUE);

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
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock)
{
	char debug[512];
	smTRANS_XTRAP_ADDR		*lpTransXTrapAddr = (smTRANS_XTRAP_ADDR *)lpPacket;

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
			smWsockDataServer->Send((char *)&smTransXTrapAddr, smTransXTrapAddr.smTransCommand.size, TRUE);

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
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock)
{
	return TRUE;
}

#endif
#endif



int	HackTrap_OpenFlagMask = 0;

//ÇÙÅø Ã½Å©¿ë NPC±â´É ±¸Çö½Ã ¼­¹ö¿¡ °Ë»çÅëº¸
int HackTrap_CheckOpenFlag() { return TRUE; }


//ÇÙÅø Ã½Å©¿ë ÇÔÁ¤
int	SendHackTrapToServer(DWORD dwTrapCode, int Param) { return TRUE; }

// pluto Á¦·Ã µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendSmeltingItemToServer(void *lpsSmeltingItem_Send) { return TRUE; }
// pluto Á¦ÀÛ µ¥ÀÌÅ¸¸¦ ¼­¹ö·Î º¸³¿
int SendManufactureItemToServer(void *lpsManufactureItem_Send) { return TRUE; }

// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ¾ÆÀÌÅÛÀ» ¼­¹ö·Î Àü¼ÛÇÑ´Ù.
int SendMixtureResetItemToServer(void *lpsMixtureResetItem_Send) { return TRUE; }

