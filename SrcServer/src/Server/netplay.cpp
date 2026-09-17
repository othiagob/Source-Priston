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

char* szDefaultServIP = "127.0.0.1"; // Valor padro

// Funo para inicializar o szDefaultServIP com o valor do Connect.ini
void InicializarDefaultServIP()
{
	std::string strIP = LerIPConnect();
	if (!strIP.empty()) {
		// Aloca memria para o novo IP e copia o valor
		static char szNewIP[32];
		strcpy(szNewIP, strIP.c_str());
		szDefaultServIP = szNewIP;
		std::cout << "szDefaultServIP inicializado com: " << szDefaultServIP << std::endl;
	}
	else {
		std::cout << "No foi possvel ler o IP do Connect.ini. Usando valor padro: " << szDefaultServIP << std::endl;
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
//? 
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
		//  
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
			//  ?
			CheckServerInfo(&TransServerConnectInfo);
		}

		return smWsockServer;
	}
	return NULL;
}


//   
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

// ?  ?
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

//  ?
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


//  ?
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

//  ?
scITEM *FindEmptyScItem()
{
	int cnt;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++) {
		if (!scItems[cnt].Flag) return &scItems[cnt];
	}

	return NULL;
}

//? ?  ?
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

//    ?
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
							TransItemInfo.x = dwPlayServTime;											//? ?
							TransItemInfo.y = TransItemInfo.Item.ItemHeader.dwChkSum;					//? ?2
							TransItemInfo.z = rsPlayInfo[cnt].spMoney_Out^(TransItemInfo.x+TransItemInfo.y);	//?  
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

//  ?  ?
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

		// ? - ?   (800000 - > 1600000 = ? 2 ??)
		int cnt = (lpServerExp->Total1^lpServerExp->Total2) + 1600000;		// 
		if (cnt < lpServerExp->InputExp)
		{
			SendSetHackUser(83);
		}

	}

	return TRUE;
}

//? ?  ? 
int CreateConnectThread(DWORD dwIP);
//? ?   
int CreateDisconnectThread(smWINSOCK *lpsmSock);

//? ? ?   ?
int FindNewOtherPlayer()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (!chrOtherPlayer[cnt].Flag)
			return cnt;
	}

	return -1;
}

//? ?? ?
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

// ? ?
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

//? ?
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

//? ? ?
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

// ?? ?
smCHAR *FindEmptyChar()
{
	int cnt;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (!chrOtherPlayer[cnt].Flag)
			return &chrOtherPlayer[cnt];
	}

	return NULL;
}

//??  
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

//??  
int AddChatBuff(char *szMsg)
{
	return AddChatBuff(szMsg, 0);
}

int	ChatBuffFilter(char* szMsg, DWORD dwIP) { return TRUE; }


// ?? ??? ?
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


	//? ??
	int	Init(smWINSOCK *lpsmsock);
	// ? 
	int SendChat(char *szMessage);
	// ?  ? 
	int SendPlayData(smCHAR *lpChar);

	//   ? 
	int RecvMessage(smTHREADSOCK *SockInfo);

	//? ? ? 
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


//? ? ? 
int rsTRANS_SERVER::RecvMessageQue()
{
	int cnt;

	if (RecvDataQuePop >= RecvDataQuePush) return TRUE;
	if (MessageLoading) return FALSE;

	//???  
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

	//???  
	LeaveCriticalSection(&cSection);

	return TRUE;
}


DWORD	dwGetCharInfoTime = 0;

int rsTRANS_SERVER::RecvMessage(smTHREADSOCK *pData) { return TRUE; }

int rsTRANS_SERVER::Init(smWINSOCK *lpsmsock)
{

	lpsmSock = lpsmsock;
	lpRecorder = 0;

	//???  ??
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
			//? ?   ( 1 ?  )
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


//  ? 
rsTRANS_SERVER	rsMainServer;







/*
//?  ? 
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

//? ?? 
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

	//? ?
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

//?  ??  
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

// ?    ?? ?    ?
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


char *rsGetWord(char *q, char *p);		//  


//? ?  
int SendChatMessageToServer(char *szChatMessage) { return TRUE; }


//? ? ?
int ChatScrollPoint = 0;

//?  ?? ?
int DisplayChatMessage(HDC hdc, int x, int y, int MaxLine) { return TRUE; }







//250 , 60 ,342 , 70

//?  ?
int GetNextLineCount(char *szString, int LineMax)
{

	return TRUE;
}

int SetIpColor(HDC hdc, DWORD dwIP) { return TRUE; }

static int	DisplayChatFilter = -1;
int	DispChatMsgHeight = 6;			//? ? ?  
int	DispChatMode = 0;

//? ?
int	SetChatMsgFilter(int mode)
{

	DisplayChatFilter = -1;
	ChatScrollPoint = 0;

	switch (mode) {
	case 0:			//
		DisplayChatFilter = -1;
		break;

	case 1:			//?
		DisplayChatFilter = 5;
		break;

	case 2:			//
		DisplayChatFilter = 4;
		break;

	case 3:			//?
		DisplayChatFilter = 6;
		break;

	case 4:			//?
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
//?  ?? ? (  )
int DisplayChatMessage3(HDC hdc, int x, int y, int StrMax, int LineMax, int Mode) { return TRUE; }

//?  ?? ?
int DisplayChatMessage2(HDC hdc, int x, int y, int StrMax, int LineMax) { return TRUE; }


int ConnectPlayer(smWINSOCK *lpsmSock)
{

	Utils_Log(LOG_DEBUG, "(%s) ConnectPlayer", lpsmSock->szIPAddr);
	int OthPlayerCnt;

	if (ServerMode)
		return Serv_ConnectPlayer(lpsmSock);


	//???  
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

	//???  
	LeaveCriticalSection(&cSection);

	return TRUE;
}

//?  
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
		//?? ?  ?
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//?  ?
			cTrade.CancelTradeItem();								//? ?
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

		//?? ?  ?
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//?  ?
			cTrade.CancelTradeItem();								//? ?
		}
		return TRUE;
	}

	if (smWsockUserServer && lpsmSock->sock == smWsockUserServer->sock) {
		AddChatBuff("Connection lost to Server");
		smWsockUserServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 16");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//?? ?  ?
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);			//?  ?
			cTrade.CancelTradeItem();					//? ?
		}
		return TRUE;
	}

	if (smWsockExtendServer && lpsmSock->sock == smWsockExtendServer->sock) {
		AddChatBuff("Connection lost to Server");
		smWsockExtendServer = 0;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 18");
		DisconnectFlag = dwTime;
		//DisconnectServerCode = 3;
		//?? ?  ?
		if (cTrade.OpenFlag) {
			SendRequestTrade(cTrade.TradeCharCode, 3);		//?  ?
			cTrade.CancelTradeItem();							//? ?
		}
		return TRUE;

	}

	char szBuff[256];

	//   
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



	//???  
	EnterCriticalSection(&cSection);

	lpPlayer = (smCHAR *)lpsmSock->ExtData1;
	if (lpPlayer) {
		lpPlayer->Close();
	}

	//???  
	LeaveCriticalSection(&cSection);

	return TRUE;
}



//    ?
int Ts_LastSendCounter;
DWORD	dwLastSendPosiTime = 0;		//   (?)
DWORD	dwLastSendPlayTime = 0;		//   (?)

// ?  ? 
int SendPlayDataToServer() { return TRUE; }


//?   ?
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
//? ?  
DWORD WINAPI ConnectPlayThreadProc(void *pInfo)
{

	ConnectOtherPlayer((DWORD)pInfo);

	ExitThread(TRUE);
	return TRUE;
}
//? ?   
DWORD WINAPI DisconnectPlayThreadProc(void *pInfo)
{

	((smWINSOCK *)pInfo)->CloseSocket();

	ExitThread(TRUE);
	return TRUE;
}



//? ?  ? 
int CreateConnectThread(DWORD dwIP)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, ConnectPlayThreadProc, (void *)dwIP, 0, &dwMsgId);

	return TRUE;
}

//? ?   
int CreateDisconnectThread(smWINSOCK *lpsmSock)
{
	DWORD dwMsgId;
	CreateThread(NULL, 0, DisconnectPlayThreadProc, (void *)lpsmSock, 0, &dwMsgId);

	return TRUE;
}


//  ?   
int DisconnectFarPlayer()
{
	int mx, mz, dDist;
	int cnt;
	int timeover;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		// ??  ?  
		if (chrOtherPlayer[cnt].Flag) {

			if ((chrOtherPlayer[cnt].dwLastTransTime + DIS_TIME_OVER) < dwPlayTime)
				timeover = TRUE;
			else
				timeover = FALSE;

			//
			mx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
			mz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
			dDist = mx*mx + mz*mz;

			if ((chrOtherPlayer[cnt].DisplayFlag && dDist > DIST_TRANSLEVEL_DISCONNECT) || timeover) {
				// /  
				chrOtherPlayer[cnt].Flag = 0;

				if (chrOtherPlayer[cnt].TransSock) {
					chrOtherPlayer[cnt].TransSock->CloseSocket();
				}
				else {
					//???  
					EnterCriticalSection(&cSection);
					chrOtherPlayer[cnt].Close();
					//???  
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

// ?
int RecvPlayData(smTHREADSOCK *pData)
{

	DWORD *IPData;
	//smCHAR *lpPlayer;

	DWORD	dwTime;

	//???  
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

	//  
	rsMainServer.RecvMessage(pData);
	/*
		}
		else {
			//??? ..

			lpPlayer = (smCHAR *)pData->smMySock->ExtData1;
			if ( lpPlayer ) lpPlayer->RecvPlayData( pData );
		}
	*/
	//???  
	LeaveCriticalSection(&cSection);

	return TRUE;
}

//? ? ? 
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

//  
int NetWorkPlay()
{
	//	char *szIP;
	int cnt;
	DWORD	dwTime;
	DWORD	dwTime2;
	smWINSOCK	*lpsmSock;

	// - XignCode
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

		//Sleep( 60*1000*3 );		//3 

		NetWorkInitFlag = TRUE;

		rsMainServer.Init(smWsockServer);

		// ?  ? 
		SendPlayDataToServer();

		//  
		if (smConfig.DebugMode)
			SendAdminMode(TRUE);


		//		if ( smWsockServer )
		//			lpCurPlayer->SendCharInfo( smWsockServer );			//?  

				// ?  ? ?
		if (smWsockDataServer)
		{
			SendGetRecordData(lpCurPlayer->smCharInfo.szName);

			// IP ? (   IP? )
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
		TransThrowItem.code = 0;			//  ??

		RecordFailCount = 0;			//  ?
		dwLastWareHouseChkSum = 0;		//? ? ?

		TransAgingItemFlag = 0;			// ?   


		ZeroMemory(&TransRecvItem, sizeof(TRANS_ITEMINFO));

		Trans_SplashCharList.code = 0;	//   ??

		ZeroMemory(&PK_FieldState, sizeof(PK_FIELD_STATE));	//PK  ??
		PK_FieldState.FieldCode = -1;


		// ? ? 
		ClearAttMonsterCode();

		Init_RecordDamage();			//?  ??

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
			// ? ? ?
			if (!TradeSendSucessFlag && sTrade.CheckFlag && sTradeRecv.CheckFlag) {
				SendTradeCheckItem(cTrade.TradeCharCode);
				TradeSendSucessFlag = TRUE;
			}
		}
		if (dwTradeMaskTime) {
			//?? ? ? ??
			if (dwTradeMaskTime < dwPlayTime) dwTradeMaskTime = 0;
		}

		dwTime = dwPlayTime - dwLastSendPosiTime;
		dwTime2 = dwPlayTime - dwRecvServerTime;

		//  
		if ((PlayCounter - Ts_LastSendCounter) > PLAYSERVER_SENDCOUNTER) {//&& dwTime>NET_SENDING_WAIT_TIME &&
//			dwTime2<NET_SENDING_WAIT_TIME ) {
			// ?  ? 
			SendPlayDataToServer();
			//  ?   
			DisconnectFarPlayer();

			//? 
			Ts_LastSendCounter = PlayCounter;
			dwLastSendPosiTime = dwPlayTime;
		}

		//		if ( (xxcnt&0x3)==3 ) SendPlayData( lpCurPlayer );

				//?   ?
				//SendPlayData( lpCurPlayer );

		if (smWsockUserServer) {// && rsRecvUserServer ) {
			//?    (40/70) ?  
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
							//?  ?  ? ( ?  ?p  )
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

		//  ? 
		if (dwLoadingTime) {
			dwTime = GetCurrentTime();
			if (dwTime > (dwLoadingTime + NET_LOADING_WAIT_LIMIT)) {
				// ? 
				Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 19");
				DisconnectFlag = GetCurrentTime();
				dwLoadingTime = 0;
			}
		}


		cnt = PlayCounter & 0x1FF;
		if (cnt == 0) {
			// ? ( ?    ? ? )
			TransCheckNetwork();
		}
		else if (cnt == 0x80) {
			//?    ?
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
#ifndef _LANGUAGE_CHINESE	// 
#ifndef _LANGUAGE_VEITNAM	// 
#ifndef _LANGUAGE_PHILIPIN	// 
#ifndef _LANGUAGE_ENGLISH	// 
	HWND	hWnd;

	if ((PlayCounter & 0x1FF) == 0 && !smConfig.DebugMode) {
		hWnd = WinFocusCrack();
		// ? 
		if (hWnd) {
			//?  ?
			SendCrackWindow(hWnd);
		}
}
#endif
#endif
#endif
#endif


	//? ??  ( 0.8 )
	if (dwLastTransTradeKeyTime && (dwLastTransTradeKeyTime + 800) < dwPlayTime)
	{
		if (smWsockDataServer)
			smWsockDataServer->Send((char *)&LastTransTradeItemKey, LastTransTradeItemKey.size, TRUE);

		dwLastTransTradeKeyTime = 0;
	}


	if (ChatDispCnt > 0) ChatDispCnt--;

	return TRUE;
}


// 
int ThrowPutItem(sITEM *lpItem, int x, int y, int z)
{

	//   ?    ( 4 ?    )
	if ((dwRecvServerTime + 4000) < dwPlayTime || QuitSave || TransThrowItem.code)
		return FALSE;

	if (AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer)
		return FALSE;

	//    ? (    )
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
// 
int ThrowPutItem2(sITEMINFO *lpItem, int x, int y, int z)
{

	//   ?    ( 4 ?    )
	if ((dwRecvServerTime + 4000) < dwPlayTime || QuitSave || TransThrowItem.code)
		return FALSE;

	if (AdminCharDisable || WarningHack || !smWsockDataServer || !smWsockServer)
		return FALSE;

	//    ? (    )
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


//  
//TRANS_ITEMINFO	TransThrowItem;
//			ThrowItemToServer((TRANS_ITEMINFO *)pData->Buff );

int	ThrowPotionCount = 0;

//?    ?? ??  
int ThrowItemToServer(smTRANS_COMMAND_EX *lpTransCommand) { return TRUE; }


//   ?
int SendAddStartPoint(int x, int z)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_ADD_STARTPOSI;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.LParam = x;
	TransCommand.WParam = z;
	TransCommand.SParam = TRUE;

	smWINSOCK *lpsmSock;
	lpsmSock = GetAreaServerSock();		//  ?
	if (lpsmSock) {
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}

//  
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

	//  ?
	lpItem = FindScItem(x, z);
	if (lpItem) {
		lpsmSock = GetServerSock(lpItem->ServerCode);		//  ?
		lpItem->Close();
	}

	if (lpsmSock) {
		return lpsmSock->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return NULL;
}



//NPC ? ?
int SendAdd_Npc(smCHAR *lpCharSample, int state)
{
	smTRNAS_PLAYERINFO	smTransPlayerInfo;

	smTransPlayerInfo.code = smTRANSCODE_ADD_NPC;
	smTransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	memcpy(&smTransPlayerInfo.smCharInfo, &lpCharSample->smCharInfo, sizeof(smCHAR_INFO));
	smTransPlayerInfo.smCharInfo.State = 0;

	//NPC   ?2 ?? 
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
	lpsmSock = GetAreaServerSock();		//  ?
	if (lpsmSock) {
		return lpsmSock->Send((char *)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}

	return NULL;
}

//NPC ? 
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

	lpsmSock = GetServerSock(lpCharSample->ServerCode);		//  ?
	lpCharSample->Close();

	if (lpsmSock) {
		return lpsmSock->Send((char *)&smTransPlayerInfo, smTransPlayerInfo.size, TRUE);
	}
	return NULL;
}

// ?  ? ?
int SendGetRecordData(char* szName) { return TRUE; }

char *szCmdOpenMonster[10] = {
	"?",
	"?",
	"?",
	"??",
	"?",
	0,0,0,0,0
};


// 
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
	lpsmSock = GetAreaServerSock();		//  ?
	if (lpsmSock) {
		return lpsmSock->Send((char *)&smTransCharCommand, smTransCharCommand.size, TRUE);
	}

	return FALSE;

}

// ?  
int	CodeXorCharInfo()
{
	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;
	lpCurPlayer->smCharInfo.Life[0] ^= CodeXorLife;
	return TRUE;
}

//?  ?
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

//??  ??
int ReformSkillInfo()
{
	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		cSkill.ReformSkillMasteryForm(&sinSkill.UseSkill[j]);
		cSkill.ReformSkillPointForm(&sinSkill.UseSkill[j]);
	}
	return TRUE;
}

// 
int	SaveGameData()
{
	if (smWsockDataServer && !rsRecorder.dwRecordTime &&
		lpCurPlayer->smCharInfo.JOB_CODE && !AdminCharDisable && !cTrade.OpenFlag && !cWareHouse.OpenFlag && !QuitSave)
	{
		//? 
		//if ( ComparePotion()>0 ) return FALSE;		// 

		if (!WarningHack && CheckCharForm() != FALSE && SaveCheckSkill() == TRUE)
		{
			CodeXorCharInfo();
			ReformCharForm();

			rsRecorder.MakeRecordData(&lpCurPlayer->smCharInfo, cInvenTory.InvenItem, cInvenTory.InvenItemTemp, &MouseItem);
			rsRecorder.SendRecordDataToServer(smWsockDataServer);
			rsRecorder.dwRecordTime = dwPlayTime;					//   ?

			if (smConfig.DebugMode)
				AddChatBuff("GameSave");

			CodeXorCharInfo();
			ReformCharForm();
			CheckPlaySkill();		//?
			return TRUE;
		}
	}
	return FALSE;
}

//   ( FALSE  ( ?    ) )
int GetSaveResult()
{
	if (rsRecorder.dwRecordTime) return FALSE;

	return TRUE;
}


/////////////////////   ////////////////////
	//smTRANSCODE_ID_GETUSERINFO:
	//ID  ? ?   

	//smTRANSCODE_INSRECORDDATA:
	//ID ? ? 

	//smTRANSCODE_DELRECORDDATA:
	//ID ? ? 

	//smTRANSCODE_ISRECORDDATA:
	//??  ? ?

//    ?
//int	TransUserCommand ( DWORD dwCode , char *szID , char *szName )

///////////////////////////////////////////////////////

///////////////////// ?  ////////////////////
	//smTRANSCODE_ID_SETUSERINFO:
	//ID  ? ?    
	//(TRANS_USERCHAR_INFO    )

	//smTRANSCODE_ISRECORDDATA:
	//??  ? ? 
	//( smTRANS_CHAR_COMMAND2 ->wParam    )
///////////////////////////////////////////////////////

smTRANS_COMMAND	smMessageCommandBuff;

//    ? (   )
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
				smMessageCommandBuff.WParam = -19;			//? 
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

//?   
int	Send_GetCharInfo(DWORD	dwObjectSerial, int ServerCode)
{
	smTRANS_COMMAND			TransCommand;
	smWINSOCK	*lpsmSock;

	TransCommand.code = smTRANSCODE_GETPLAYERINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;

	lpsmSock = GetServerSock(ServerCode);		//  ?

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

// ? ( ?    ? ? )
int	TransCheckNetwork()
{
	smTRANS_COMMAND			TransCommand;
	DWORD	dwTime, dwTime2;
	smWINSOCK	*lpOldSock;

	//return TRUE;

	if (!dwConnectedServerTime)
		return FALSE;

	//?  ?  

	OpenTimeCheckThread();

	//? 4 ? ? ? 
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
		//   (   ? )

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
			//  
			SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockServer);

			char szBuff[256];
			wsprintf(szBuff, "Reconnect Server(Main)");
			AddChatBuff(szBuff);

			if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
			if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
			if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
			if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;
			ReconnServer++;		//? ?

			return TRUE;
		}

		DisconnectServerCode = 1;
		Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 20");
		DisconnectFlag = GetCurrentTime();
		return FALSE;
	}
	//}


	//  
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
				//  
				//Sleep(200);
				SendContinueServer(lpCurPlayer->dwObjectSerial, smWsockUserServer);

				char szBuff[256];
				wsprintf(szBuff, "Reconnect Server(User)");
				AddChatBuff(szBuff);
				if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime2) dwLastRecvGameServerTime = dwTime2;
				if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime2) dwLastRecvGameServerTime2 = dwTime2;
				if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime2) dwLastRecvGameServerTime3 = dwTime2;
				if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime2) dwLastRecvGameServerTime4 = dwTime2;

				ReconnDataServer++;		// ? ?

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

				//ReconnDataServer++;		// ? ?

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
		//  ?
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


	// ?  ?   ?
	SendTotalExpMoney();

	CheckMaxDamage();			//?  ?


	return TRUE;
}

//   
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

//   
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

// ?? ?  
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

//IP ??   
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

// ? ? 
int SendSetObjectSerial(DWORD dwObjectSerial)
{
	// 
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

// ? ?  (  ? )
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

//  
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

//?   ? ?
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

//? ?
int CheckCracker() { return TRUE; }

//? ? ( ? ?   )
int CheckCracker(TRANS_FIND_CRACKER *lpTransFindCrack) { return TRUE; }

//?  ?
int SendCrackWindow(HWND hWnd)
{
	Utils_Log(LOG_DEBUG, "SendCrackWindow");

	char *szName;
	char TransBuff[1024];
	int len;

	//? ?
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

//?   ? ?
int SendSetHackUser(int StopGame)
{
	Utils_Log(LOG_DEBUG, "SendSetHackUser: StopGame(%d)", StopGame);

	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_SET_BLACKLIST;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = StopGame;
	TransCommand.LParam = 0;
	TransCommand.SParam = 0;

	WarningHack = StopGame;		//? 

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

//?   ? ?
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

//   
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

//    ? 
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

//    ? (    )
int SendSaveThrowItem(sITEMINFO *lpItemInfo)
{
	smTRANS_COMMAND	TransCommand;

	// ? ? 
	if ((lpItemInfo->CODE&sinITEM_MASK1) != (sinPM1&sinITEM_MASK1))	return FALSE;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;

	if ((lpItemInfo->CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		ComparePotion();	//? 

	return smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}
/*
//    ? ? 32  (    )
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

// ?  
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

//    ? (    )
int SendSaveThrowItem2(sITEMINFO *lpItemInfo)
{
	smTRANS_COMMAND_EX	TransCommand;


	TransCommand.code = smTRANSCODE_SAVETHROWITEM2;
	TransCommand.size = sizeof(smTRANS_COMMAND_EX);
	TransCommand.WParam = lpItemInfo->CODE;
	TransCommand.LParam = lpItemInfo->ItemHeader.Head;
	TransCommand.SParam = lpItemInfo->ItemHeader.dwChkSum;
	TransCommand.EParam = lpItemInfo->PotionCount^TransCommand.SParam^TransCommand.code;	// 

	TransCommand.WxParam = lpCurPlayer->smCharInfo.Money;
	TransCommand.LxParam = EncodeParam(TransCommand.WxParam);
	TransCommand.SxParam = GetTotalMoney() ^ TransCommand.LxParam;
	TransCommand.ExParam = lpItemInfo->Money;

	if ((lpItemInfo->CODE&sinITEM_MASK1) == (sinPM1&sinITEM_MASK1))
		ComparePotion();	//? 

	//??  ? 
	AddInvenMoney(-lpItemInfo->Money);

	return smWsockDataServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//    ? (    )
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

//? ? ?  ?
int SendCopyOtherUser(char *szName, int Mode) { return TRUE; }

//  ?  
int SendPlayUpdateInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 0;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			// 
	TransCommand.SParam = 0;
	TransCommand.EParam = 0;

	return smWsockServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
}

//  ?  
int SendUpdateClanInfo()
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_UPDATE_CINFO;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = 1;
	TransCommand.LParam = lpCurPlayer->smCharInfo.Level;			// 
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

	DWORD	dwDestObjectSerial;	//? ? ?

	int x,y,z;			// ?
	int AttackState;	// ?
	int AttackSize;		//  
	int Power;			//  ?

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//? ? ?
};
*/


TRANS_SKIL_ATTACKDATA	Back_TransSkilAttackData;

// 
int Skil_RangeAttack(int x, int y, int z, int range, int power, int TargetState)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	//int	accuracy;
	int	dRange;
	int dx, dy, dz, dist;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;


	//??? ?  
	Record_ClinetLogFile(" ? - Skil_RangeAttack ");
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

				// ? ??  
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
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);//   ?

	LastAttackDamage = power;

	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));		//

	lpsmSendSock = GetAreaServerSock();		// ?

	if (lpsmSendSock && mCnt > 0) {
		// ? 
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	if (PkMode) {
		// (   )
		Skil_RangeAttack_User(x, y, z, range, power, TargetState);
	}

	return TRUE;
}


// (   )
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
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);//   ?


	if (mCnt > 0) {
		// ? 
		Record_SendDamage(smWsockUserServer, TransSkilAttackData.Power);

		smWsockUserServer->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}


	//	lpsmSendSock = GetAreaServerSock();		// ?

	//	if ( lpsmSendSock ) {
	//		lpsmSendSock->Send( (char *)&TransSkilAttackData , TransSkilAttackData.size , TRUE );
	//	}
	return TRUE;
}

// 
int Skil_RangeBoxAttack(smCHAR *lpCurChar, RECT *lpRect, int power, int TargetState, int UseAttackRating)
{

	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	int cnt, mCnt;
	int dx, dy, dz;
	int Attack, accuracy;
	smCHAR	*lpChar;
	smWINSOCK *lpsmSendSock = 0;


	//??? ?  
	Record_ClinetLogFile(" ? - Skil_RangeBoxAttack ");
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

			// ?  ? ? ?! .
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

					// ? ??  
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
		(TransSkilAttackData.x * TransSkilAttackData.y * TransSkilAttackData.z);//   ?


	memcpy(&Back_TransSkilAttackData, &TransSkilAttackData, sizeof(TRANS_SKIL_ATTACKDATA));		//

	lpsmSendSock = GetAreaServerSock();		// ?

	if (lpsmSendSock && mCnt > 0) {
		// ? 
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}
	return TRUE;
}


int Skill_CharRangeEffect(DWORD dwSkillCode, smCHAR *lpCurChar, smCHAR *lpCharTarget, int point) { return TRUE; }


//  ? 
int Skil_RangeBoxEffect(smCHAR *lpCurChar, RECT *lpRect, DWORD dwSkillCode, int TargetState) { return TRUE; }

//  ? 
int Skil_RangeEffect(smCHAR *lpCurChar, int Range, DWORD dwSkillCode, int TargetState, int point) { return TRUE; }




//? ? ? 
DWORD EncodeParam(DWORD Param)
{
	return (Param + (Param << 4)) * 2002;
}



// ??   
int QuickSendTransAttack(smCHAR *lpChar, int power, int AttackState, int Resistance)
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK *lpsmSendSock = 0;
	int pow;

	//??? ?  
	Record_ClinetLogFile(" ? - QuickSendTransAttack ");
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
			TransAttackData.Power += pow;			//? ? ? (  ? ?   )
		}
	}

	TransAttackData.dwDestObjectSerial = lpCurPlayer->dwObjectSerial;
	TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;
	TransAttackData.dwChkSum = TransAttackData.Power * 2002 +
		(TransAttackData.x * TransAttackData.y * TransAttackData.z);//   ?

	LastAttackDamage = TransAttackData.Power;

	// ? ??  
	if (lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
		lpChar->EnableStateBar = TRUE;
	}

	if (lpChar->ServerCode) {
		lpsmSendSock = GetServerSock(lpChar->ServerCode);		// ? ?
	}


	if (lpsmSendSock) {
		// ? 
		Record_SendDamage(lpsmSendSock, TransAttackData.Power);

		return lpsmSendSock->Send((char *)&TransAttackData, TransAttackData.size, TRUE);
	}

	return NULL;
}

// ??   
int QuickSendTransAttack(smCHAR *lpChar, int power)
{
	return QuickSendTransAttack(lpChar, power, 1, 0);
}
// ??   
int QuickSendTransAttack(smCHAR *lpChar, int power, int Resistance)
{
	return QuickSendTransAttack(lpChar, power, 1, Resistance);
}

///////////////////////// ??  //////////////////////////////
/*
sTRADE sTrade;	   // 
sTRADE sTradeRecv; // 
*/

// ? 
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

// ?  
int SendTradeItem(sTRADE *lpTrade, DWORD dwSender)
{
	int cnt;
	int len;
	int	PotionError = 0;
	TRANS_TRADEITEMS TransTradeItems;
	sTRADE	sTradeBuff;

	len = 0;

	//??   ?
	if (CheckTrade((void *)lpTrade) == FALSE) {
		SendRequestTrade(dwSender, 3);			//?  ?
		cTrade.CancelTradeItem();					//? ?
		return FALSE;
	}

	memcpy(&sTradeBuff, lpTrade, sizeof(sTRADE));

	//? 
	PotionError = ComparePotion();

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (!sTradeBuff.TradeItem[cnt].Flag) {
			ZeroMemory(&sTradeBuff.TradeItem[cnt], sizeof(sITEM));
		}
		else {
			sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head = 0;		//? 
			if (PotionError) {
				sTradeBuff.TradeItem[cnt].sItemInfo.PotionCount = sTradeBuff.TradeItem[cnt].sItemInfo.ItemHeader.Head + 1;	//?   ?? 1 ??
			}
			len++;
		}
	}

	//  ( Z/NZ  )
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

// ?   
int RecvTradeItems(TRANS_TRADEITEMS *lpTransTradeItems, sTRADE *lpTrade)
{
	//  ? ( Z/NZ  )
	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE *)lpTrade);
	memcpy(&TransLastRecvTrade, lpTransTradeItems, lpTransTradeItems->size);		// ??  ? 
	return TRUE;
}

//?  ?
int SendTradeItem(DWORD dwSender)
{
	return SendTradeItem(&sTrade, dwSender);
}
//?  
int RecvTradeItems(TRANS_TRADEITEMS *lpTransTradeItems)
{
	int res;

	res = RecvTradeItems(lpTransTradeItems, &sTradeRecv);
	cTrade.LoadTradeItemIamge();
	cTrade.CheckTradeButtonOk();

	ItemPickUpFlag = 1;

	//??   ?
	if (CheckTrade(&sTradeRecv) == FALSE) {
		SendRequestTrade(lpTransTradeItems->dwSender, 3);	//? 
		cTrade.CancelTradeItem();
		return FALSE;
	}

	if (sTrade.CheckFlag && !sTradeRecv.CheckFlag) {
		sTrade.CheckFlag = 0;
		TradeItemCancelCount++;
		if (TradeItemCancelCount >= 3) {
			//3  
			SendRequestTrade(lpTransTradeItems->dwSender, 3);	//? 
			cTrade.CancelTradeItem();
			return FALSE;
		}
	}

	return res;
}

// ? ? 
int SendTradeSucessKey(DWORD dwSender)
{
	// ? ? 
	return SendTradeSucessKey(&sTrade, dwSender);
}



//-------> ?  ?  ?   ? //////////////

// ? ? 
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
				//  ?   ?
				CancelFlag = TRUE;

				//??   
				SendInvenItemError(1, lpTrade->TradeItem[cnt].sItemInfo.CODE,
					lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head,
					lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum);
			}

			if ((TransTradeItemKey.dwItemCode[cnt] & sinITEM_MASK1) == (sinPM1&sinITEM_MASK1)) {
				//?! (  ? ?  )
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
				//    ? ? 32  (    )
				SendSaveThrowItems( &TransThrowItems );

				if ( smWsockServer )
					smWsockServer->Send( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;

				if ( smWsockUserServer && smWsockServer!=smWsockUserServer )
					smWsockUserServer->Send( (char *)&TransTradeItemKey , TransTradeItemKey.size , TRUE ) ;
		*/
		if (smWsockDataServer)
			smWsockDataServer->Send((char *)&TransTradeItemKey, TransTradeItemKey.size, TRUE);


		// ??
		memcpy(&LastTransTradeItemKey, &TransTradeItemKey, sizeof(TRANS_TRADE_ITEMKEY));
		dwLastTransTradeKeyTime = dwPlayTime;
	}

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}


// ? ? 
int RecvTradeSucessKey(TRANS_TRADE_ITEMKEY *lpTransTradeItemKey, sTRADE *lpTrade)
{
	int cnt;
	sTRADE sChkTrade;
	DWORD	dwChkSum1, dwChkSum2;
	int	ItemFlag;

	dwTradeMaskTime = dwPlayTime + 3000;

	if (TransLastRecvTrade.code) {
		//     ? ? ( Z/NZ  )
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
				//?
				ItemFlag = 1;
				dwChkSum1 = dwChkSum1 >> 16;
				dwChkSum2 = dwChkSum2 & 0xFFFF;
			}
			else
				ItemFlag = 0;

			if (lpTransTradeItemKey->dwItemCode[cnt] == lpTrade->TradeItem[cnt].sItemInfo.CODE && dwChkSum1 == dwChkSum2) {
				//			lpTransTradeItemKey->dwItemSum[cnt]==lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum ) {

				lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head = lpTransTradeItemKey->dwItemKey[cnt];

				//?  
				if (!lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.Head)
					lpTrade->TradeItem[cnt].Flag = 0;
				else {
					if (ItemFlag == 1) {
						// 
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

	//SendRecordTradeItems( lpTrade );			//?   

	return TRUE;
}



// ?  ?
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
		SendSaveMoney();			//  (  )
	}

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (sTrade.TradeItem[cnt].Flag) {
			TransTradeCheckItem.dwSendItemCode[cnt] = sTrade.TradeItem[cnt].sItemInfo.CODE;
			TransTradeCheckItem.dwSendItemSum[cnt] = sTrade.TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum;

			//SendSaveThrowItem( &sTrade.TradeItem[cnt].sItemInfo );		//  

			//  ?
			if (CheckItemForm(&sTrade.TradeItem[cnt].sItemInfo) == FALSE)
				return FALSE;		//?  

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
		//    ? ? 32  (    )
		SendSaveThrowItems( &TransThrowItems );
	*/

	if (smWsockServer)
		smWsockServer->Send((char *)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	if (smWsockUserServer && smWsockServer != smWsockUserServer)
		smWsockUserServer->Send((char *)&TransTradeCheckItem, TransTradeCheckItem.size, TRUE);

	dwTradeMaskTime = dwPlayTime + 3000;

	return TRUE;
}

// ?  ? 
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





//////////////////////////  ? ?  ///////////////

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

//? 
int	SaveWareHouse(sWAREHOUSE *lpWareHouse, TRANS_WAREHOUSE *lpTransWareHouse)
{
	if (!lpWareHouse)
		return FALSE;
	(void)lpTransWareHouse;
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


	//? ?
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





//? 
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
		//?? ?? 
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
		TransCommand.WParam = Money;							// 
		TransCommand.LParam = lpCurPlayer->smCharInfo.Money;		// 
		TransCommand.SParam = 0;

		AddInvenMoney( -Money );				//?  ?

		if ( smWsockServer ) {
			return smWsockServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );
		}
	*/

	sITEM	sItemMoney;
	int j;


	memset(&sItemMoney, 0, sizeof(sITEM));
	sItemMoney.sItemInfo.CODE = sinGG1 | sin01;
	for (j = 0; j < MAX_ITEM; j++) {
		if (sItem[j].CODE == sItemMoney.sItemInfo.CODE) { //? 
			memcpy(&sItemMoney, &sItem[j], sizeof(sITEM));
			sItemMoney.sItemInfo.Money = Money;			//? ?? 
			sItemMoney.sItemInfo.ItemKindCode = smTRANSCODE_COLLECTMONEY;
			ReformItem(&sItemMoney.sItemInfo);		// 
			if (sinThrowItemToFeild(&sItemMoney)) {
				AddInvenMoney(-Money);				//?  ?
				return TRUE;
			}
		}
	}

	return FALSE;
}

//PK? 
int PlayerKilling(DWORD dwObjectSerial)
{
	smTRANS_COMMAND	TransCommand;

	TransCommand.code = smTRANSCODE_PLAYERKILLING;
	TransCommand.size = sizeof(smTRANS_COMMAND);
	TransCommand.WParam = dwObjectSerial;				// ?
	TransCommand.LParam = lpCurPlayer->dwObjectSerial;		// ?
	TransCommand.SParam = 0;

	if (smWsockUserServer) {
		return smWsockUserServer->Send((char *)&TransCommand, TransCommand.size, TRUE);
	}
	return FALSE;
}



//?   
int SendCraftItemToServer(void *lpsCraftItem_Send) { return TRUE; }

//     
int SendWingItemToServer(void *lpsCraftItem_Send) { return TRUE; }


//   
int SendAgingItemToServer(void *lpsCraftItem_Send) { return TRUE; }

//??  
int SendLinkCoreToServer(sITEMINFO *lpItem) { return TRUE; }



// ?   
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

//   ?
int	SendCheckItemToServer(sITEMINFO *lpItem) { return TRUE; }

// ?  
int RecvCheckItemFromServer(TRANS_ITEM_CODE *lpTransItemCode)
{

	switch (lpTransItemCode->code) {
	case smTRANSCODE_CHECKITEM:
		//  ? ? 
		//cInvenTory.ResetInvenItemToServer( lpTransItemCode->dwItemCode , lpTransItemCode->dwHead ,
		//	lpTransItemCode->dwChkSum , lpTransItemCode->dwTime );
		break;

	case smTRANSCODE_ERRORITEM:
		// ? ?  (   )
		cInvenTory.DeleteInvenItemToServer(lpTransItemCode->dwItemCode, lpTransItemCode->dwHead, lpTransItemCode->dwChkSum);

		//??   ? ?? ?
		if (FindInvenItem(lpTransItemCode->dwItemCode, lpTransItemCode->dwHead, lpTransItemCode->dwChkSum) == 0) {

			SendInvenItemError(0, lpTransItemCode->dwItemCode, lpTransItemCode->dwHead, lpTransItemCode->dwChkSum);
			ResetInvenItemCode();				//?  ? ??
		}

		break;

	case smTRANSCODE_CLEARPOTION:
		//?? ?  1 
		ClearInvenPotionCount();
		break;
	}

	return TRUE;
}

//?   
int	SellItemToServer(sITEMINFO *lpItem, int Count) { return TRUE; }





int Chk_InventPosCount = 1;
DWORD	dwCheckInvenItemTime = 0;

//??     ?
int CheckInvenItemToServer() { return TRUE; }

////////////////////////////// ?  ?  ///////////////////////////////

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


//? ? ?
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




// ?  ? 
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
// ?  ? 
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

	{ (DWORD)E_Shield_BlockRate << 1			,	((Meteo_UseMana - E_Shield_BlockRate) + 10ul) },		//? ? 

	{ (DWORD)Code_VRamBuffOpen << 1			,	3600	},			//   ? ?
	{ 0,0 }
};



//???  ?   
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

	// 
	if (smWsockDataServer)
		return smWsockDataServer->Send((char *)&TransClientFuncPos, TransClientFuncPos.size, TRUE);

	return FALSE;
}


//? ?  
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


//////////////////////////////    ? //////////////////////////

#define	RECV_TRANSITEM_QUE_MAX			256
#define	RECV_TRANSITEM_QUE_MASK			255

TRANS_ITEMINFO	Recv_TransItemInfo_Que[RECV_TRANSITEM_QUE_MAX];

int	RecvTransItemQue_Push = 0;
int	RecvTransItemQue_Pop = 0;

// ? ? 
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

//?  ? ?
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

//?  ?   ?
int CheckRecvTrnsItemQue()
{
	int cnt, mCnt;

	for (cnt = RecvTransItemQue_Pop; cnt < RecvTransItemQue_Push; cnt++) {
		mCnt = cnt&RECV_TRANSITEM_QUE_MASK;
		if (Recv_TransItemInfo_Que[mCnt].Item.CODE) return FALSE;
	}

	return TRUE;
}

//  ?  
int SendUseItemCodeToServer(sITEMINFO *lpItem) { return TRUE; }

//? ?? ??  ?
int RecvCommandUser(smTRANS_COMMAND_EX *lpTransCommandEx) { return TRUE; }

//? ?? ??  ? 
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

//  ? 
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

//  ? 
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
//?  ??
int	Init_RecordDamage()
{
	ZeroMemory(sRecvDamage, sizeof(sDAMAGE_RECORD) * 3);
	ZeroMemory(sSendDamage, sizeof(sDAMAGE_RECORD) * 3);

	return TRUE;
}

// ? 
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

// ? 
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

//? ?   
int	SendPartySkillToServer(DWORD dwSkillCode, int SkillLevel, int Around, int wParam, int lParam, int sParam, int eParam) { return TRUE; }

//? 
int SendCancelSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam) { return TRUE; }

//? 
int SendUpdateSkillToServer(DWORD dwSkillCode, int lParam, int sParam, int eParam) { return TRUE; }

// ? 
int SendProcessSKillToServer(DWORD dwSkillCode, int point, int Param1, int Param2) { return TRUE; }

//? ? ? 
int RecvPartySkillFromServer(TRANS_PARTY_SKILL *lpTransPartySkill)
{
	int cnt, cnt3;
	int time;
	int flag;
	//smCHAR *lpChar;

	switch (lpTransPartySkill->dwSkillCode) {
	case SKILL_PLAY_HOLY_VALOR:
		//??
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
					//??  ( ?  ) / ? ?? ? 
					//- ? - ? ? ? ? ? ? ? ?  
					//(?  ?  )

					SendCancelSkillToServer(lpTransPartySkill->dwSkillCode, 0, 0, 0);
				}
				else {
					//? ? 
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
					//??
					if ( lpTransPartySkill->Point<1 || lpTransPartySkill->Point>10 ) break;

					time = T_Of_Valhalla_Time[lpTransPartySkill->Point-1];

					lpChar = FindChrPlayer( lpTransPartySkill->dwPartyUser[0] );		//

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

//? ? 
int SendClanJoinService(DWORD dwMsgCode, smCHAR* lpChar) { return TRUE; }

//?   ? 
int SendJoinClan() { return TRUE; }

//? ? ?
int RecvClanJoinService(smTRANS_CHAR_COMMAND2 *lpTransCharCommand)
{
	smCHAR *lpChar;

	switch (lpTransCharCommand->WParam) {
	case 1:		// 
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


/////////// DivineLightning ?  ? ///////////////////

DWORD	dwSkill_DivineLightning_Target[8];
int		DivineLightning_Target_Count = 0;
int		DivineLightning_FindCount = 0;

// ? ?  ( ?? - ? )
int	SkillPlay_DivineLightning_Select(smCHAR *lpChar, int SkillPoint)
{
	int cnt, TargetCount;
	int x, y, z, dist;
	int	LightningCount;
	int cCount;
	TRANS_SKIL_ATTACKDATA	TransSkilAttackData;
	smWINSOCK *lpsmSendSock = 0;

	//??? ?  
	Record_ClinetLogFile(" ? - SkillPlay_DivineLightning_Select ");
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

	TransSkilAttackData.AttackState = 103;			//?? 
	TransSkilAttackData.AttackSize = 160 * 160;
	TransSkilAttackData.Power = Divine_Lightning_Damage[SkillPoint - 1];

	lpsmSendSock = GetAreaServerSock();		// ?

	if (lpsmSendSock && TargetCount > 0) {
		// ? 
		Record_SendDamage(lpsmSendSock, TransSkilAttackData.Power);

		lpsmSendSock->Send((char *)&TransSkilAttackData, TransSkilAttackData.size, TRUE);
	}

	return TRUE;
}

// ? ?  ( ?  )
int SkillPlay_DivineLightning_Effect(smCHAR *lpChar, int SkillPoint) { return TRUE; }


// ? ?  ( ??? )
int SkillPlay_MummyLord_Effect(smCHAR *lpChar, int Range) { return TRUE; }


// ? ?  ( ?  )
int SkillPlay_VenomSpear_Effect(smCHAR *lpChar, int SkillPoint) { return TRUE; }

// ? ?  ( ?  )
int SkillPlay_Monster_Effect(smCHAR *lpChar, int EffectKind, int Range) { return TRUE; }




//?  ? 
int RecvProcessSkill(smTRANS_COMMAND *lpTransCommand) { return TRUE; }



/////////////////////////////////////////////////////////////////

//int ReadClanInfo(DWORD dwClanNum)
//CLANINFO ClanInfo[CLAN_MAX]; //?  
//
//cldata ?
//void Updatecldata();

//? ? ? ?
//#define	smTRANSCODE_CLAN_UPDATE			0x50320C10


//?  ? ?
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

//?  ?
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

//?  ? ?
int FiltQuestItem(TRANS_ITEMINFO	*lpTransItemInfo, DWORD dwPacketCode)
{
	int cnt;


	switch (sinQuest_ChangeJob2.CODE) {
	case SIN_QUEST_CODE_CHANGEJOB2_NPC_M:
		//  (?  ? )
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin01)) {
			if (!cInvenTory.SearchItemCode(sinMA1 | sin01)) return FALSE;		// 
			if (cInvenTory.SearchItemCode(sinQT1 | sin04)) return FALSE;		//? ? 
			cnt = cInvenTory.SearchItemCode(sinMA2 | sin01);
			if (cnt >= 9) return FALSE;
		}
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin02)) return FALSE;
		return TRUE;

	case SIN_QUEST_CODE_CHANGEJOB2_NPC_D:
		//?  (?  ? )
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin02)) {
			if (!cInvenTory.SearchItemCode(sinMA1 | sin01)) return FALSE;		// 
			if (cInvenTory.SearchItemCode(sinQT1 | sin05)) return FALSE;		//? ? 
			cnt = cInvenTory.SearchItemCode(sinMA2 | sin02);
			if (cnt >= 7) return FALSE;
		}
		if (lpTransItemInfo->Item.CODE == (sinMA2 | sin01)) return FALSE;
		return TRUE;
	}

	//3  ?
	//
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin06)) {
		if (sinQuest_ChangeJob3.CODE == SIN_QUEST_CODE_CHANGEJOB3) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin06);
			if (cnt >= 3) return FALSE;
			return TRUE;
		}
		return FALSE;
	}

	// ?  1
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin09)) {
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin09);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	// ?  2
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin10)) {
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin10);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}
	// ?  3
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin11)) {
		if (sinQuest_Level80_2.CODE == SIN_QUEST_CODE_LEVEL80_2) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin11);
			if (cnt >= 1) return FALSE;
			return TRUE;
		}
		return FALSE;
	}


	//3  ? 
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin13)) {

		if (sinQuest_ChangeJob4.CODE == SIN_QUEST_CODE_CHANGEJOB4 && sinQuest_ChangeJob4.State == 5) {
			cnt = cInvenTory.SearchItemCode(sinQT1 | sin13);
			if (cnt >= 1 || CheckAttMonsterCode(sinQuest_ChangeJob4.Kind) == FALSE)
				return FALSE;

			return TRUE;
		}
		return FALSE;
	}



	// ? ? 
	if (lpTransItemInfo->Item.CODE == (sinQT1 | sin16)) {

		return FALSE;
	}


	if ((lpTransItemInfo->Item.CODE&sinITEM_MASK2) == sinMA1 ||
		(lpTransItemInfo->Item.CODE&sinITEM_MASK2) == sinMA2) {
		//?  ?   

		return FALSE;
	}

	return TRUE;
}

POINT GetProcessModule();

//? ?  
int CheckProcessModule() { return TRUE; }

//?  
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

//??  
int SendDancingSwordDamage(smCHAR *lpChar) { return TRUE; }


//??? ?  
int SendMegneticSphereDamage(smCHAR *lpChar) { return TRUE; }

//?  
int SendMuspellDamage(smCHAR *lpChar) { return TRUE; }



//  
int SendLowLevelPetDamage(smCHAR *lpChar, int petType) { return TRUE; }


//PC  
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



//    ?
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

//    ?
int	SendItemExpress(DWORD	dwItemCode)
{
	return	SendItemExpress(dwItemCode, 0);
}


//?   
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

		lpsmSock = GetAreaServerSock();		//  ?
		if (lpsmSock)
			lpsmSock->Send((char *)&smTransCommand, smTransCommand.size, TRUE);

		return TRUE;

	}
	return FALSE;
}

// ?   
int	SendStarPointToServer(int Price, int Cash) { return TRUE; }

//?  
int	SendGiveMoneyToServer(int Money) { return TRUE; }

//??   
int	SendClanMoneyToServer(int Money, int Flag, int Kind) { return TRUE; }

int	SendClanMoneyToServer(int Money, int Flag) { return TRUE; }

//?   
int	SendPaymentMoneyToServer(int Money, int Flag) { return TRUE; }



/*
//? 
#define smTRANSCODE_OPEN_PERTRADE	0x48478A90
//? ?
#define smTRANSCODE_CALL_PERTRADE	0x48478AA0
//? 
#define	smTRANSCODE_MYSHOP_ITEM		0x48478AB0

int	Send_ShowMyShopItem( smCHAR *lpChar );							//?   ?
int	Send_MyShopItemList( smTRANS_COMMAND	*lpTransCommand );		//?   ? 
int	Recv_MyShopItemList( TRANS_TRADEITEMS *lpTransTradeItems );		//?   
*/


//?   ?
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

//?   ? 
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


//?   
int	Recv_MyShopItemList(TRANS_TRADEITEMS *lpTransTradeItems) { return TRUE; }


//   
int	Send_PersonalShopItem(DWORD dwCharCode, void *lpShopItem) { return TRUE; }

//    ?
int	UpdateMyShopList(void *lpMyShop) { return TRUE; }

//  
int SendOpenPersonalTrade(char *szTradeMsg, void *lpPersTrade) { return TRUE; }

// ?
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

// ? - ?
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
	//  -  , ?  ?
	if (lpsItem->CODE == (sinBI1 | sin32) || lpsItem->CODE == (sinBI1 | sin33) || lpsItem->CODE == (sinBI1 | sin34) || lpsItem->CODE == (sinBI1 | sin51) ||// pluto (?)
	//	lpsItem->CODE == (sinBI1|sin35) || lpsItem->CODE == (sinBI1|sin36) || lpsItem->CODE == (sinBI1|sin37) || lpsItem->CODE == (sinBI1|sin38) )	//?
		lpsItem->CODE == (sinBI1 | sin36) || lpsItem->CODE == (sinBI1 | sin37) ||
		lpsItem->CODE == (sinBI1 | sin38) || lpsItem->CODE == (sinBI1 | sin39) ||
		lpsItem->CODE == (sinBI1 | sin40) || lpsItem->CODE == (sinBI1 | sin41) ||
		lpsItem->CODE == (sinBI1 | sin42) || lpsItem->CODE == (sinBI1 | sin43) ||
		lpsItem->CODE == (sinBI1 | sin44) || lpsItem->CODE == (sinBI1 | sin45) ||
		lpsItem->CODE == (sinBI1 | sin46) || lpsItem->CODE == (sinBI1 | sin47) ||
		lpsItem->CODE == (sinBI1 | sin48) || lpsItem->CODE == (sinBI1 | sin49) ||
		lpsItem->CODE == (sinBI1 | sin50) || lpsItem->CODE == (sinBI1 | sin52) || //  -    ?
		lpsItem->CODE == (sinBI1 | sin53) || lpsItem->CODE == (sinBI1 | sin54) ||

		lpsItem->CODE == (sinBI1 | sin65) || lpsItem->CODE == (sinBI1 | sin66) || // ? -    (7, 1)
		lpsItem->CODE == (sinBI1 | sin67) || lpsItem->CODE == (sinBI1 | sin68) ||
		lpsItem->CODE == (sinBI1 | sin69) || lpsItem->CODE == (sinBI1 | sin70) ||
		lpsItem->CODE == (sinBI1 | sin71) || lpsItem->CODE == (sinBI1 | sin72) ||
		lpsItem->CODE == (sinBI1 | sin55) || lpsItem->CODE == (sinBI1 | sin77) ||	// ? - ??(1)
		lpsItem->CODE == (sinBI1 | sin62) || lpsItem->CODE == (sinBI1 | sin63) ||	//  -  (2)  ? D, E
		lpsItem->CODE == (sinBI1 | sin64) ||										//  -  (2)  ? F
		lpsItem->CODE == (sinBI1 | sin73) || lpsItem->CODE == (sinBI1 | sin74) || // ? -    ?(? 1  4)
		lpsItem->CODE == (sinBI1 | sin75) || lpsItem->CODE == (sinBI1 | sin76)) // ? -    (1)
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

//  ?
int RecvForceOrbItem(TRANS_ITEMINFO_GROUP2 *lpTransItemGroup2)
{
	TRANS_ITEMINFO_GROUP	TransItemGroup;

	ZeroMemory(&TransItemGroup, sizeof(TRANS_ITEMINFO_GROUP));
	memcpy(&TransItemGroup, lpTransItemGroup2, TRANS_GROUPITEM_HEADER_SIZE);

	DecodeCompress((BYTE *)lpTransItemGroup2->szBuff, (BYTE *)TransItemGroup.sItemInfo, sizeof(sITEMINFO)*TRANS_GROUPITEM_MAX);

	//? ? ?
	//  ?
	sinRecvForceOrb(TransItemGroup.sItemInfo, TransItemGroup.ItemCount);


	return TRUE;
}

//?  
int SendQuestCommandToServer(DWORD dwQuestCode, int Param1, int Param2, int Param3) { return TRUE; }

// ? 

//?   
int	SendBlessCastleToServer(smTRANS_BLESSCASTLE *lpBlessCastleSetup, int Mode) { return TRUE; }

//?    ?
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
//?  
int RecvBlessCastInfo(void *lpPacket)
{
	smTRANS_BLESSCASTLE *lpBlessCastleSetup = (smTRANS_BLESSCASTLE *)lpPacket;
	rsUSER_LIST_TOP10	*lpClanListTop10;

	switch (lpBlessCastleSetup->smTransCommand.WParam) {
	case 1:		// 
		chaSiege.ShowSiegeMenu(lpBlessCastleSetup);
		break;

	case 2:		//  
		lpClanListTop10 = (rsUSER_LIST_TOP10 *)lpPacket;
		//  ? ?
		chaSiege.ShowSiegeScore(lpClanListTop10);
		break;
	}

	return TRUE;
}

// ?  ? 
int SetBlessCastleMaster(DWORD dwClanCode, int Mode)
{

	if (!Mode && lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD) {

	}
	else {
		if (rsBlessCastle.dwMasterClan != dwClanCode) {
			//?  ?
			UpdateCastleMasterClan(rsBlessCastle.dwMasterClan);
		}

		rsBlessCastle.dwMasterClan = dwClanCode;
	}

	return TRUE;
}

//?  
int	SendResistanceToServer() { return TRUE; }

//   
int	SendPublicPollingToServer(int PollCode, int PollCnt, BYTE *bPollingData) { return TRUE; }


#ifdef _xTrap_GUARD
#include "XTrapSrc\\XTrap.h"
//? ?
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
		//? ?
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock)
{
	char debug[512];
	smTRANS_XTRAP_ADDR		*lpTransXTrapAddr = (smTRANS_XTRAP_ADDR *)lpPacket;

	sprintf(debug, "XTrap_Recv");
	OutputDebugString(debug);

	if (lpPacket->WParam == XTRAP_CMD_SEEDKEY)
	{// ? 
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

		//----[ 1.     ? ]
		smTRANS_XTRAP_ADDR	smTransXTrapAddr;
		memset(&smTransXTrapAddr, 0, sizeof(smTransXTrapAddr));

		unsigned int usResult = 1;
		//usResult = XTrapCC_STEP2( (PUCHAR)lpTransXTrapAddr->PacketBuf, (PUCHAR)smTransXTrapAddr.PacketBuf, XTRAP_PROTECT_PE|XTRAP_PROTECT_TEXT);
		usResult = XTrap_CS_Step2((PUCHAR)lpTransXTrapAddr->PacketBuf, (PUCHAR)smTransXTrapAddr.PacketBuf,
			XTRAP_PROTECT_PE | XTRAP_PROTECT_TEXT | XTRAP_PROTECT_EXCEPT_VIRUS);

		//----[ 2.     ]
		smTransXTrapAddr.smTransCommand.size = sizeof(smTRANS_XTRAP_ADDR);
		smTransXTrapAddr.smTransCommand.code = smTRANSCODE_XTRAP_PACKET;
		smTransXTrapAddr.smTransCommand.WParam = XTRAP_CMD_UNIQKEY;

		if (smWsockDataServer)
			smWsockDataServer->Send((char *)&smTransXTrapAddr, smTransXTrapAddr.smTransCommand.size, TRUE);

		//----[ 3. ? ? ? ]
		if (usResult == 0)
		{//
		}
		else
		{//
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
//? ?
int XTrap_Recv(smTRANS_COMMAND *lpPacket, smWINSOCK *lpsmSock)
{
	return TRUE;
}

#endif
#endif



int	HackTrap_OpenFlagMask = 0;

// ? NPC   ??
int HackTrap_CheckOpenFlag() { return TRUE; }


// ? 
int	SendHackTrapToServer(DWORD dwTrapCode, int Param) { return TRUE; }

// pluto    
int SendSmeltingItemToServer(void *lpsSmeltingItem_Send) { return TRUE; }
// pluto    
int SendManufactureItemToServer(void *lpsManufactureItem_Send) { return TRUE; }

//  - ?    ?.
int SendMixtureResetItemToServer(void *lpsMixtureResetItem_Send) { return TRUE; }

