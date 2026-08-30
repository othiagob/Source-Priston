#pragma once
#include "fileread.h"
#include "Character/character.h"
#include "smPacket.h"

int	rsStart_CombatCastle();
int	rsEnd_CombatCastle(int DefenceFlag);
int	rsEnd_CombatCastle();
int	rsEasy_CastleMode();
int rsEnterHardCoreEvent(rsPLAYINFO* lpPlayInfo, int Money);
TRANS_SOD_RESULT* rsSod_GetRealTimeScore(rsPLAYINFO* lpPlayInfo, int TeamNum);
rsPLAYINFO* FindUserFromID(char* szID);
smCHAR_INFO* FindMonsterFromName(char* szName);

#define SENDPACKET SendPacket
#define	LIMIT_PK_LEVEL	20

#define AUTOPLAYER_MAX		2048
#define AUTOPLAYER_MASK		2047

#define	PRIME_ITEM_TIME_THIRDEYES		(60*60*24)
#define	PRIME_ITEM_TIME_EXPUP			(60*60*24)
#define	PRIME_ITEM_TIME_3DAY			(60*60*24*3)

#define	PRIME_ITEM_TIME_THIRDEYESx7		(60*60*3)//(60*60*24*7)
#define	PRIME_ITEM_TIME_EXPUPx7			(60*60*24*7)

#define	PRIME_ITEM_TIME_VAMPCUSPID		(60*60*3)	
#define	PRIME_ITEM_TIME_MANARECHARG		(60*60*3)

#define	PRIME_ITEM_TIME_VAMPCUSPIDx2	(60*60*24)
#define	PRIME_ITEM_TIME_MANARECHARGx2	(60*60*24)

#define	PRIME_ITEM_TIME_PACKAGE1		(60*60*3)
#define	PRIME_ITEM_TIME_PACKAGE2		(60*60*24)
#define	PRIME_ITEM_TIME_PACKAGE3		(60*60*24*7)
#define	PRIME_ITEM_TIME_PACKAGE4		(60*60*24*30)
#define	PRIME_ITEM_TIME_PACKAGE5		(60*60*1)


#define	PRIME_ITEM_PACKAGE_NONE			0
#define	PRIME_ITEM_PACKAGE_BRONZE		1
#define	PRIME_ITEM_PACKAGE_SILVER		2
#define	PRIME_ITEM_PACKAGE_GOLD			3
#define	PRIME_ITEM_PACKAGE_ULTRA		4

#define PRIME_ITEM_MIGHTOFAWELL			5
#define PRIME_ITEM_MANAREDUCE			6

#define	rsSOD_FIELD			30
#define	rsSOD_VILLAGE		9
#define	rsBABEL_FIELD		27
#define	rsKELVEZU_FIELD		36
#define	rsMOKOVA_FIELD		38
#define	rsCASTLE_FIELD	33
#define	rsACTION_FIELD	39

#define	QUEST_ARENA_FIELD		32
#define	QUEST_ARENA_NPC_POS_X	(22091*fONE)
#define	QUEST_ARENA_NPC_POS_Z	(9673*fONE)


#define	WHERE_TRADE				300
#define	WHERE_GIFT_EXPRESS		600
#define	WHERE_AGINGITEM			400
#define	WHERE_CRAFTITEM			420
#define	WHERE_FORECORB			440
#define	WHERE_LEARNSKILL		460
#define	WHERE_WARPGATE			470
#define	WHERE_BUY_ITEM			200
#define	WHERE_BUY_POTION		210
#define	WHERE_SELL_ITEM			220
#define	WHERE_LOAD_PLAYER		100
#define	WHERE_THROW_ITEM		120
#define	WHERE_GET_ITEM			130
#define	WHERE_STAR_POINT		620
#define	WHERE_CLAN_MONEY		630
#define	WHERE_GIVE_MONEY		640
#define	WHERE_PERSONAL_SHOP		320
#define	WHERE_OPEN_WAREHOUES	500


#define STG_START_POINT_MAX			200

struct STG_CHAR_INFO {

	smCHAR_INFO	smCharInfo;

	int		PosiState;

	int		StartFixed;
	POINT	StartPosi;

};

struct STG_START_POINT {
	int state;
	int x, z;
};

#define STG_ITEM_MAX	1024

struct psITEM {
	int		state;
	int		Serial;

	sITEMINFO	ItemInfo;
};

struct	STG_ITEMS {
	int		state;
	int		x, y, z;
	psITEM	Item;
	DWORD	dwCreateTime;
	DWORD	dwLoseTime;
	DWORD	Level;
	int		BeginMode;
};

struct Rune_Formula
{
	DWORD Recipe;
	DWORD Rune[3];
};

#define FIX_CHAR_MAX	120
#define	STG_MONSTER_MAX		256

class smSTAGE3D;
class sFIELD;

class STG_AREA {
public:
	DWORD	Head;

	smSTAGE3D* lpStage;
	sFIELD* lpField;

	char				szStartPointFile[64];
	char				szCharFixedFile[64];
	char				szCharMonsterFile[64];

	STG_START_POINT		StartPoint[STG_START_POINT_MAX];
	int					StartPointNearPlay[STG_START_POINT_MAX];
	int					StartPointMonCount[STG_START_POINT_MAX];	//�ش� ���� ī����
	int					StartPointCnt;
	int					StartPointLastOpenCount;					//�ֱ� ���� ���� ����Ʈ ī����
	DWORD				dwStartPoint_OpenTime[STG_START_POINT_MAX];	//���� �ֱ� �ð� ���

	int					StartPointOpenCount;						//��ü ������ ��
	int					StartPointActiveCount;						//Ȱ��ȭ�� ������ ��

	STG_ITEMS			StgItems[STG_ITEM_MAX];					//�ʻ� �����ϴ� ������

	STG_CHAR_INFO* lpStageCharInfo;

	smTRNAS_PLAYERINFO	TransCharFixed[FIX_CHAR_MAX];			//������ ĳ���� ���� ( NPC�� )

	rsSTG_MONSTER_LIST	rsMonsterList;							//���� ���� ���̺�

	int					StgCharInfoCnt;

	int					ItemSettingCount;
	int					MonsterSettingCount;

	int					Counter;

	DWORD				FieldNightDay;							//�� ���� ��ȭ�� �����ϱ� ���� ����

	smCHAR* lpCharMonster[STG_MONSTER_MAX];		//���� ���� ���� ���̺�
	int					MonsterCount;							//���� ���� ī����
	int					MonsterLastPoint;						//���� ���̺� ������ ����Ʈ

	int					EvnMonster;								//���� �Ͻ������� �ʰ� ����

	DWORD				Event_dwMonsterTime;					//�̺�Ʈ ���� ���� �ð�
	smCHAR_INFO* Event_lpChrMonster;					//�̺�Ʈ ���� ĳ�� ������
	int					Event_MonsterPercentage;				//�̺�Ʈ ���� �߻� Ȯ��

	DWORD				dwActiveMode;							//���� ���� ( spp , spc ��� )

	STG_AREA();
	//	~STG_AREA();

	int	Close();										//Ŭ���� �ݱ�

	int LoadStage(char* szFile, char* szFile2);						//��� �޽� �ε� 

	smCHAR* OpenMonster(STG_CHAR_INFO* lpStgCharInfo, int gGroup = 1);	//���� ����
	smCHAR* OpenCaravan(STG_CHAR_INFO* lpStgCharInfo, rsPLAYINFO* Player);
	int Main();

	int SendStartPos(rsPLAYINFO* lpPlayInfo);


	int LoadStartPoint(char* szFile);
	int SaveStartPoint(char* szFile);
	int FindStartPoint(int x, int z);					//���� ���� �˻�
	int AddStartPoint(int x, int z, int state);		//���� ���� �߰�
	int DeleteStartPoint(int x, int z);				//���� ���� ����
	int SetStartPosChar(smCHAR* lpChar);				//���ο� ���� ���� ��ġ�� ã�� ���� 
	int SetStartPosCaravan(smCHAR* lpChar, rsPLAYINFO* Player);
	int SetStartPosNearChar(smCHAR* lpChar, int cx, int cy, int cz);	//Ư�� ��ǥ �ֺ��� ĳ���� ��ġ�� ��ġ�� 
	int SetPosNearChar(smCHAR* lpChar, int cx, int cy, int cz);	//Ư�� ��ǥ �ֺ��� ĳ���͸� �̵��� 
	int StartPosRandomize();							//���� ���� ��ġ ����ȭ �Ѵ� 

	int SendStartPointFlag();							//�������� �����ġ ����

	smTRNAS_PLAYERINFO* FindCharInfoFixed(smTRNAS_PLAYERINFO* lpTransPlayerInfo);		//���� ĳ���� ���� ã��
	int	AddCharInfoFixed(smTRNAS_PLAYERINFO* lpTransPlayerInfo);		//���� ĳ���� ���� �߰�
	int	DeleteCharInfoFixed(smTRNAS_PLAYERINFO* lpTransPlayerInfo);	//���� ĳ���� ���� ����
	int OpenNpc(smTRNAS_PLAYERINFO* lpTransPlayerInfo);				//NPC ĳ���� ����
	int LoadCharInfoFixed(char* szFile);								//���� ĳ���� ���� �о����
	int SaveCharInfoFixed(char* szFile);								//���� ĳ���� ���� ����

	STG_ITEMS* FindItem(int x, int y, int z);							//�������� ã�´�
	STG_ITEMS* FindNearGold(int x, int y, int z);
	STG_ITEMS* AddItem(psITEM* lpItem, int x, int y, int z, int BeginMode = 0);	//�������� ��ġ�Ѵ�
	int	DeleteItem(int x, int y, int z);								//�ش� ��ġ�� �������� ������
	int DeleteItem(STG_ITEMS* lpStgItem);								//�ش� �������� ������
	int SendStgItem(rsPLAYINFO* lpPlayInfo);							//������ ���� ���� 
	int SendStgItems();													//������ ������ �� �����鿡�� ����
	int SendStgItem(rsPLAYINFO* lpPlayInfo, STG_ITEMS* lpStgItem);	//������ ���� 1���� ���� 
	int SendStgItemToNearUsers(STG_ITEMS* lpStgItem);					//������ ���� 1���� ���� 
	int SendStgItemToPartyUsers(rsPLAYINFO* lpPlayInfo, STG_ITEMS* lpStgItem);	//������ ���� 1���� ��ó�� ��Ƽ������ ���� ���� 

	int	FindEmptyMonTable();											//�� ���� ���̺� ��ȣ ã��
	int	AddMonTable(smCHAR* lpChar);									//���� ���̺��� ������ �߰�
	int	DeleteMonTable(smCHAR* lpChar);								//���� ���̺��� ������ ����


	int LoadEnemyList(char* szFile);
};

//StgArea �ִ� ���� 
#define STAGE_AREA_MAX		64

extern STG_AREA* StageArea;				//Stg_Area ����


extern DWORD		dwPlayServTime;
extern rsSERVER_CONFIG		rsServerConfig;				//���� ���� ����

// N�vel inicial configur�vel
extern int g_LevelInicial;

// N�vel m�ximo configur�vel
extern int g_LevelFinal;
extern smCHAR* chrAutoPlayer;					//[AUTOPLAYER_MAX];
extern rsPLAYINFO* rsPlayInfo;				//[CONNECTMAX];

extern int Server_SqlReady;
extern HWND	hDebugWnd;
extern CRITICAL_SECTION	cSerSection;
extern int Server_SqlReady;

extern time_t		tServerTime;

extern DWORD		dwBabelPlayTime;			//�Ŵ���� �ٺ� �����ð�
extern int			BabelPlayField;				//�Ŵ���� �ٺ� �ʵ�

extern DWORD		dwKelvezuPlayTime;			//�Ŵ���� �̺��� �����ð�
extern int			KelvezuPlayField;			//�Ŵ���� �̺��� �ʵ�

///////////////////////////////////////////////////////////////////////

//������ ĳ�� �뺴 ���Ѽ��� ����
#define srCASLTE_SOLDER_CLAN_MAX	20
#define srCASLTE_SOLDER_MAX			1
#define srCASLTE_SOLDER_KIND_MAX	5

//������ ĳ�� ���� ����
struct	rsBLESS_CASTLE {

	int		CastleMode;
	DWORD	dwMasterClan;
	int		DefenceLevel;
	DWORD	dwLimitTime;
	int		Counter;

	int		Tax;				//���� ����
	int		NextTax;			//���� ����

	DWORD	dwBattleOverTime;	//���� ���� �ð�
	int		SoulFountain_PotionCount;		//��ȥ�� ���� ���� ����
	DWORD	dwSoulFountainTime;				//��ȥ�� ���� ���� �߻��ð�
	DWORD	dwSoulFountainTime2;			//��ȥ�� ���� ���� �����ð�
	int		SoulFountain_LastUserCount;

	DWORD	dwStartTime;
	short	TimeSec[2];						//����ð���[0] ����ð���[1]	
	int		SelectedSkill;

	int		Temp[10];

	smTRANS_BLESSCASTLE	BlessCastleSetup;
	rsUSER_LIST_TOP10	ClanTop10;
	ATTACK_DAMAGE_LIST* lpAttackDamageList;
	char	bCrystalSolderCount[4];
	char	bCrystalTowerCount[4];
	smCHAR* lpChar_ValhallaTower;
	DWORD	dwScoreLogTime;				//�������� ����ð�

};

extern rsBLESS_CASTLE	rsBlessCastle;				//������ ĳ�� ���� ����



struct	rsCURRENCY_LOG {
	//���� �� ��ȭ�� ���
	INT64	int64_CraftMoney;
	INT64	int64_AgingMoney;
	INT64	int64_ShopMoney;
	INT64	int64_ShopPotionMoney[3];

	INT64	int64_ForceOrbMoney;
	INT64	int64_WarpgateMoney;
	INT64	int64_LearnSkillMoney;
};

extern	rsCURRENCY_LOG	CurrencyLog;

//////////////////////////////////////////////////////////////////////////

#define	srMONSTER_KILL_QUE_MAX		1024
#define	srMONSTER_KILL_QUE_MASK		1023

struct	rsKILL_MONSTER_INFO {
	DWORD	dwAutoCharCode;
	DWORD	dwCharSoundCode;
	DWORD	dwMonObjectSerial;
	DWORD	dwUserObjectSerial;
	int		x, y, z, Area;
	int		PartyFlag;
	DWORD	dwParam[4];
};

struct	TRANS_KILL_MONSTER_INFO {
	int		size, code;
	DWORD	tCode;
	int		KillInfo_Count;
	int		Temp[4];
};

//////////////////////////////////////////////////////////////////////////

/////////////// ����Ʈ�� ���� ��ġ ����ü //////////////////////////////
//������ ī����
#define	rsHARDCORE_COUNT_END	8
#define	hCORE_ROUND_MAX		9

struct	_sHCORE_MONSTER {
	char	szName[50];
	smCHAR_INFO* lpCharInfo;
	int		perCount;
};

struct sMYSHOP_ITEM_SERVER
{
	DWORD CODE;
	DWORD CkSum;
	DWORD Head;
	DWORD sTime;
	DWORD Price;
	DWORD SendFlag;
	DWORD Buyer;

	DWORD Temp[4];

};

struct	sHCORE_MONSTER {

	_sHCORE_MONSTER	hCoreMonster1;
	_sHCORE_MONSTER	hCoreMonster2;
	_sHCORE_MONSTER	hCoreMonster3;
	_sHCORE_MONSTER	hCoreMonster4;
	_sHCORE_MONSTER	hCoreMonsterBoss;

	int		PerCount;
	int		StartHour;
	int		State;
	int		MaxMonster;
	int		DelayTime;			//���۽� ������ �ð�
};

int rsPutItem(rsPLAYINFO* lpPlayInfo, DWORD dwItemCode, int spJobCode);
extern char* sinGold;
extern char* UpKeepItemName[];

STG_AREA* FindStageField(int x, int z);

//����Ÿ������ ��������
int rsSendDataServer(smWINSOCK* lpsmSock, void* szData);
//����Ʈ ���� ó��
int rsProcessQuest(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int QuestCode, int QuestParam);
//���� ������ ��� Ŭ���̾�Ʈ�� ����
int SendShopItemList(smWINSOCK* lpsmSock, smCHAR* lpChar);
//��ų ���� NPC
int SendSkillMenu(smWINSOCK* lpsmSock, smCHAR* lpChar, int Mode);
//â�� ����
int SendOpenWareHouse(smWINSOCK* lpsmSock);

int SendOpenCaravan(smWINSOCK* lpsmSock);
//������ ����â ����
int SendOpenMixItem(smWINSOCK* lpsmSock, int MixFlag);
//����� ����
int SendOpenCollectMoney(smWINSOCK* lpsmSock);
//��ǰ�̺�Ʈ
int SendOpenEventGift(smWINSOCK* lpsmSock);
//����¡ â ����
int SendOpenAgingItem(smWINSOCK* lpsmSock);
//Ŭ�� �޴� ����
int SendOpenClanMenu(smWINSOCK* lpsmSock);

int Serv_ConnectPlayer(smWINSOCK* lpsmSock);

smCHAR* Serv_CheckOtherPlayPosi(int x, int y, int z);

int Serv_DisconnectPlayer(smWINSOCK* lpsmSock);
//�̺�Ʈ NPC
int SendOpenEvent(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo, int EventFlag);
//���� ���
int SendOpenGiftExpress(rsPLAYINFO* lpPlayInfo, DWORD	dwItemCode, DWORD dwPassCode_Param);
// pluto ������ ����â ����
int SendOpenSmelting(smWINSOCK* lpsmSock);
// pluto ������ ����â ����
int SendOpenManufacture(smWINSOCK* lpsmSock);
//SoD ���� ��Ȳ ���
TRANS_SOD_RESULT* rsSod_GetRealTimeScore(rsPLAYINFO* lpPlayInfo);
// ������ - �ͽ��� ���� â ����
int SendOpenMixtureItemReset(smWINSOCK* lpsmSock);

int createItemLuckyBox(psITEM* lpsItem, rsPLAYINFO* Player, char* itCode, int JobCode);

//Ŭ���̾�Ʈ ���� ( ũ��ƼĮ ��� ���� )
int DisconnectUser(smWINSOCK* lpsmSock);
//��ŷ �õ� ��� ���Ϸ� ����
int RecordHackLogFile(rsPLAYINFO* lpPlayInfo, void* lpTransCommand);

//������ ���� ����
int RecvRangeAttack(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData);
//������ ���� ���� ( ���� ���� )
int RecvRangeAttackUser(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData);
INT64 GetExpForValeLevel(rsPLAYINFO* lpPlayInfo);

int CreateItemPerf(psITEM* lpsItem, char* szCoed, int JobCode);

smCHAR* srFindCharFromSerial(DWORD dwObjectSerial, int AreaNum);
//ĳ���͸� ã�´� ( ���߿� ���� ������ �ְ� �ڵ�ȭ ���� �ٲ� ��� �� )
smCHAR* srFindCharFromSerial(DWORD dwObjectSerial);
//ĳ���͸� ã�´� ( ���߿� ���� ������ �ְ� �ڵ�ȭ ���� �ٲ� ��� �� )
rsPLAYINFO* srFindUserFromSerial(DWORD dwObjectSerial);
//�׷�������
int rsPlayGrandHealing(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND* lpTransCommand);
//����
int	rsPlayHealing(rsPLAYINFO* playInfo, smTRANS_COMMAND* lpTransCommand);

//���� ���� �ð� ǥ��
int srDisplayBillingTime(rsPLAYINFO* lpPlayInfo, int Add_BillingTime);

//�ֺ� �������� ���� ����
int	rsSendCommandUser(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND_EX* lpTransCommandEx);
//�ֺ� �������� ���� ����
int	rsSendCommandUser2(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND* lpTransCommand);
rsPLAYINFO* FindUserFromName(char* szName);
rsPLAYINFO* FindUserFromName2(char* szName);
//������ �����Ų�� ( �����Ű�µ� �ɸ��� �ð��� �� 6~10�� ���� �ҿ� )
int	rsShutDown();
//������ �ּҸ� ����� ��ȯ
DWORD GetSwapIPCode(char* szIP);

//�̺�Ʈ �α׿� ����ó��
int rsEventLogOn_Sucess(rsPLAYINFO* lpPlayInfo);


//SOD ������ȣ ���ϱ�
int rsGetSoD_AreaCode(int x, int y, int z);
//����� �Ϲݰ��ݽ� ��ų�� ���� ���� (��������)
int rsSendRangeDamage(smCHAR* lpChar, rsPLAYINFO* lpMainPlayInfo, int Range, int SubDamgePers);
//���� ������ ���ġ ���ϴ� �Լ�
int rsGetMonsterLifeRegen(smCHAR* lpChar);

//Ŭ�� �Ӵ� ���
int	rsClanMoneyLog(rsPLAYINFO* lpPlayInfo, int Money, int RestMoney);


//���� ���ǿ� ���� �߰�
int rsAddServerPotion(rsPLAYINFO* lpPlayInfo, DWORD dwPotionCode, int PotionCount);
//���� ���ǿ��� ���� ���ϱ�
int rsGetServerPotion(rsPLAYINFO* lpPlayInfo, DWORD dwPotionCode);

//��ŷ �õ� ��� ���Ϸ� ����
int RecordHackLogFile(rsPLAYINFO* lpPlayInfo, void* lpBuff);


//��ų ��ȯ���� ���� ��Ŵ
smCHAR* OpenMonsterFromSkill(DWORD SkillCode, int x, int y, int z, rsPLAYINFO* lpPlayInfo, smCHAR* lpEnemyChar);
//��ȯ����� ã��
smCHAR* rsGetLinkChar(smCHAR* lpMasterChar);


//������ĳ�� �������� ����
int rsRecvBlessCastInfo(rsPLAYINFO* lpPlayInfo, void* lpPacket);
//������ĳ�� �������� �۽�
int rsSendBlessCastInfo(rsPLAYINFO* lpPlayInfo, int Mode);

void SendPacket(rsPLAYINFO* lpPlayInfo, char* data, int len, int mode = 0);

//Ŭ�� �ڵ� ���ϱ�
DWORD GetClanCode(int ClassClanCode);


//�ٸ������� ������Ʈ ���� ����
int	rsUpdateServerParam(rsPLAYINFO* lpPlayInfo, DWORD dwUpdateCode, int Param1, int Param2, int TargetMode);


///////////////////////// ����׿� ��� //////////////////////
int rsDebugMessageToClient(rsPLAYINFO* lpPlayInfo, char* szDmgMsg);	//����� �޼��� ���


////////////////////////// ��Ʈ�� ����ġ �ð��й� /////////////////////////////
int rsGetExp_GameTime(rsPLAYINFO* lpPlayInfo);
// pluto ���� �� �˻� // �庰 - ���������� ȥ
DWORD ManufactureCheckRune(rsPLAYINFO* lpPlayInfo, DWORD RecipeCode, DWORD* pRuneCode);

////////////////////////// �߱� �ð����� �����ð��� ���� �޼��� ��� ///////////////////
int rsDispay_GameTime(rsPLAYINFO* lpPlayInfo, DWORD dwTimeOld);

// ����� - 2010 ������ �̺�Ʈ - �౸�� ���� ���
int	rsUseSoccerBallPotionItem(DWORD dwItemCode, rsPLAYINFO* lpPlayInfo);

// ����� - ����¡�������(����¡ ������ �����)
int SetAgingItem(sITEMINFO* pItem, int AgingNum);
int SetAgingItemRestaure(sITEMINFO* pItem, int AgingNum);

int rsSendGameServer_PrimeItem2(rsPLAYINFO* lpPlayInfo);


int rsSendGameServer_PrimeItem3(rsPLAYINFO* lpPlayInfo);
