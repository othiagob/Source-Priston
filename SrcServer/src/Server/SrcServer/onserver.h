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
	int					StartPointMonCount[STG_START_POINT_MAX];	//ï¿½Ø´ï¿½ ï¿½ï¿½ï¿½ï¿½ Ä«ï¿½ï¿½ï¿½ï¿½
	int					StartPointCnt;
	int					StartPointLastOpenCount;					//ï¿½Ö±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Æ® Ä«ï¿½ï¿½ï¿½ï¿½
	DWORD				dwStartPoint_OpenTime[STG_START_POINT_MAX];	//ï¿½ï¿½ï¿½ï¿½ ï¿½Ö±ï¿½ ï¿½Ã°ï¿½ ï¿½ï¿½ï¿½

	int					StartPointOpenCount;						//ï¿½ï¿½Ã¼ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½
	int					StartPointActiveCount;						//È°ï¿½ï¿½È­ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½

	STG_ITEMS			StgItems[STG_ITEM_MAX];					//ï¿½Ê»ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½

	STG_CHAR_INFO* lpStageCharInfo;

	smTRNAS_PLAYERINFO	TransCharFixed[FIX_CHAR_MAX];			//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ( NPCï¿½ï¿½ )

	rsSTG_MONSTER_LIST	rsMonsterList;							//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìºï¿½

	int					StgCharInfoCnt;

	int					ItemSettingCount;
	int					MonsterSettingCount;

	int					Counter;

	DWORD				FieldNightDay;							//ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½È­ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ï±ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

	smCHAR* lpCharMonster[STG_MONSTER_MAX];		//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìºï¿½
	int					MonsterCount;							//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ Ä«ï¿½ï¿½ï¿½ï¿½
	int					MonsterLastPoint;						//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìºï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Æ®

	int					EvnMonster;								//ï¿½ï¿½ï¿½ï¿½ ï¿½Ï½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ê°ï¿½ ï¿½ï¿½ï¿½ï¿½

	DWORD				Event_dwMonsterTime;					//ï¿½Ìºï¿½Æ® ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ã°ï¿½
	smCHAR_INFO* Event_lpChrMonster;					//ï¿½Ìºï¿½Æ® ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	int					Event_MonsterPercentage;				//ï¿½Ìºï¿½Æ® ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ È®ï¿½ï¿½

	DWORD				dwActiveMode;							//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ( spp , spc ï¿½ï¿½ï¿½ )

	STG_AREA();
	//	~STG_AREA();

	int	Close();										//Å¬ï¿½ï¿½ï¿½ï¿½ ï¿½Ý±ï¿½

	int LoadStage(char* szFile, char* szFile2);						//ï¿½ï¿½ï¿½ ï¿½Þ½ï¿½ ï¿½Îµï¿½ 

	smCHAR* OpenMonster(STG_CHAR_INFO* lpStgCharInfo, int gGroup = 1);	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	smCHAR* OpenCaravan(STG_CHAR_INFO* lpStgCharInfo, rsPLAYINFO* Player);
	int Main();

	int SendStartPos(rsPLAYINFO* lpPlayInfo);


	int LoadStartPoint(char* szFile);
	int SaveStartPoint(char* szFile);
	int FindStartPoint(int x, int z);					//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ë»ï¿½
	int AddStartPoint(int x, int z, int state);		//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½
	int DeleteStartPoint(int x, int z);				//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	int SetStartPosChar(smCHAR* lpChar);				//ï¿½ï¿½ï¿½Î¿ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ Ã£ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 
	int SetStartPosCaravan(smCHAR* lpChar, rsPLAYINFO* Player);
	int SetStartPosNearChar(smCHAR* lpChar, int cx, int cy, int cz);	//Æ¯ï¿½ï¿½ ï¿½ï¿½Ç¥ ï¿½Öºï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ 
	int SetPosNearChar(smCHAR* lpChar, int cx, int cy, int cz);	//Æ¯ï¿½ï¿½ ï¿½ï¿½Ç¥ ï¿½Öºï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½Í¸ï¿½ ï¿½Ìµï¿½ï¿½ï¿½ 
	int StartPosRandomize();							//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ ï¿½ï¿½ï¿½ï¿½È­ ï¿½Ñ´ï¿½ 

	int SendStartPointFlag();							//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ä¡ ï¿½ï¿½ï¿½ï¿½

	smTRNAS_PLAYERINFO* FindCharInfoFixed(smTRNAS_PLAYERINFO* lpTransPlayerInfo);		//ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ Ã£ï¿½ï¿½
	int	AddCharInfoFixed(smTRNAS_PLAYERINFO* lpTransPlayerInfo);		//ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½
	int	DeleteCharInfoFixed(smTRNAS_PLAYERINFO* lpTransPlayerInfo);	//ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	int OpenNpc(smTRNAS_PLAYERINFO* lpTransPlayerInfo);				//NPC Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	int LoadCharInfoFixed(char* szFile);								//ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ð¾ï¿½ï¿½ï¿½ï¿½
	int SaveCharInfoFixed(char* szFile);								//ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

	STG_ITEMS* FindItem(int x, int y, int z);							//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ã£ï¿½Â´ï¿½
	STG_ITEMS* FindNearGold(int x, int y, int z);
	STG_ITEMS* AddItem(psITEM* lpItem, int x, int y, int z, int BeginMode = 0);	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ï¿½Ñ´ï¿½
	int	DeleteItem(int x, int y, int z);								//ï¿½Ø´ï¿½ ï¿½ï¿½Ä¡ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	int DeleteItem(STG_ITEMS* lpStgItem);								//ï¿½Ø´ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
	int SendStgItem(rsPLAYINFO* lpPlayInfo);							//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 
	int SendStgItems();													//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½é¿¡ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	int SendStgItem(rsPLAYINFO* lpPlayInfo, STG_ITEMS* lpStgItem);	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 1ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 
	int SendStgItemToNearUsers(STG_ITEMS* lpStgItem);					//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 1ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 
	int SendStgItemToPartyUsers(rsPLAYINFO* lpPlayInfo, STG_ITEMS* lpStgItem);	//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 1ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ã³ï¿½ï¿½ ï¿½ï¿½Æ¼ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ 

	int	FindEmptyMonTable();											//ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìºï¿½ ï¿½ï¿½È£ Ã£ï¿½ï¿½
	int	AddMonTable(smCHAR* lpChar);									//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìºï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½
	int	DeleteMonTable(smCHAR* lpChar);								//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ìºï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½


	int LoadEnemyList(char* szFile);
};

//StgArea ï¿½Ö´ï¿½ ï¿½ï¿½ï¿½ï¿½ 
#define STAGE_AREA_MAX		64

extern STG_AREA* StageArea;				//Stg_Area ï¿½ï¿½ï¿½ï¿½


extern DWORD		dwPlayServTime;
extern rsSERVER_CONFIG		rsServerConfig;				//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

// Nível inicial configurável
extern int g_LevelInicial;

// Nível máximo configurável
extern int g_LevelFinal;
extern smCHAR* chrAutoPlayer;					//[AUTOPLAYER_MAX];
extern rsPLAYINFO* rsPlayInfo;				//[CONNECTMAX];

extern int Server_SqlReady;
extern HWND	hDebugWnd;
extern CRITICAL_SECTION	cSerSection;
extern int Server_SqlReady;

extern time_t		tServerTime;

extern DWORD		dwBabelPlayTime;			//ï¿½Å´ï¿½ï¿½ï¿½ï¿½ ï¿½Ùºï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ã°ï¿½
extern int			BabelPlayField;				//ï¿½Å´ï¿½ï¿½ï¿½ï¿½ ï¿½Ùºï¿½ ï¿½Êµï¿½

extern DWORD		dwKelvezuPlayTime;			//ï¿½Å´ï¿½ï¿½ï¿½ï¿½ ï¿½Ìºï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ã°ï¿½
extern int			KelvezuPlayField;			//ï¿½Å´ï¿½ï¿½ï¿½ï¿½ ï¿½Ìºï¿½ï¿½ï¿½ ï¿½Êµï¿½

///////////////////////////////////////////////////////////////////////

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ ï¿½ëº´ ï¿½ï¿½ï¿½Ñ¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
#define srCASLTE_SOLDER_CLAN_MAX	20
#define srCASLTE_SOLDER_MAX			1
#define srCASLTE_SOLDER_KIND_MAX	5

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
struct	rsBLESS_CASTLE {

	int		CastleMode;
	DWORD	dwMasterClan;
	int		DefenceLevel;
	DWORD	dwLimitTime;
	int		Counter;

	int		Tax;				//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	int		NextTax;			//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½

	DWORD	dwBattleOverTime;	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ã°ï¿½
	int		SoulFountain_PotionCount;		//ï¿½ï¿½È¥ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	DWORD	dwSoulFountainTime;				//ï¿½ï¿½È¥ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ß»ï¿½ï¿½Ã°ï¿½
	DWORD	dwSoulFountainTime2;			//ï¿½ï¿½È¥ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ã°ï¿½
	int		SoulFountain_LastUserCount;

	DWORD	dwStartTime;
	short	TimeSec[2];						//ï¿½ï¿½ï¿½ï¿½Ã°ï¿½ï¿½ï¿½[0] ï¿½ï¿½ï¿½ï¿½Ã°ï¿½ï¿½ï¿½[1]	
	int		SelectedSkill;

	int		Temp[10];

	smTRANS_BLESSCASTLE	BlessCastleSetup;
	rsUSER_LIST_TOP10	ClanTop10;
	ATTACK_DAMAGE_LIST* lpAttackDamageList;
	char	bCrystalSolderCount[4];
	char	bCrystalTowerCount[4];
	smCHAR* lpChar_ValhallaTower;
	DWORD	dwScoreLogTime;				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ã°ï¿½

};

extern rsBLESS_CASTLE	rsBlessCastle;				//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ä³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½



struct	rsCURRENCY_LOG {
	//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½ï¿½È­ï¿½ï¿½ ï¿½ï¿½ï¿½
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

/////////////// ï¿½ï¿½ï¿½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Ä¡ ï¿½ï¿½ï¿½ï¿½Ã¼ //////////////////////////////
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ Ä«ï¿½ï¿½ï¿½ï¿½
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
	int		DelayTime;			//ï¿½ï¿½ï¿½Û½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ã°ï¿½
};

int rsPutItem(rsPLAYINFO* lpPlayInfo, DWORD dwItemCode, int spJobCode);
extern char* sinGold;
extern char* UpKeepItemName[];

STG_AREA* FindStageField(int x, int z);

//ï¿½ï¿½ï¿½ï¿½Å¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
int rsSendDataServer(smWINSOCK* lpsmSock, void* szData);
//ï¿½ï¿½ï¿½ï¿½Æ® ï¿½ï¿½ï¿½ï¿½ Ã³ï¿½ï¿½
int rsProcessQuest(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar, int QuestCode, int QuestParam);
//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ Å¬ï¿½ï¿½ï¿½Ì¾ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int SendShopItemList(smWINSOCK* lpsmSock, smCHAR* lpChar);
//ï¿½ï¿½Å³ ï¿½ï¿½ï¿½ï¿½ NPC
int SendSkillMenu(smWINSOCK* lpsmSock, smCHAR* lpChar, int Mode);
//Ã¢ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int SendOpenWareHouse(smWINSOCK* lpsmSock);
int SendOpenPostBox(smWINSOCK* lpsmSock);

struct POSTBOX_AUDIT {
	char	EventType[24];
	DWORD	EntryId;
	int		Kind;
	char	DestAccount[32];
	char	DestChar[32];
	char	SenderAccount[32];
	char	SenderChar[32];
	char	ItemCode[32];
	char	ItemName[64];
	DWORD	ItemBinCode;
	DWORD	ItemHead;
	DWORD	ItemChkSum;
	int		Quantity;
	int		Weight;
	char	Message[128];
	int		HasPass;
	DWORD	DepositedAt;
	DWORD	ExpireAt;
	char	Reason[48];
	char	Source[24];
	char	DestIP[48];
};
void rsPostBoxAuditLog(const POSTBOX_AUDIT* row);

int SendOpenCaravan(smWINSOCK* lpsmSock);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ã¢ ï¿½ï¿½ï¿½ï¿½
int SendOpenMixItem(smWINSOCK* lpsmSock, int MixFlag);
//ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int SendOpenCollectMoney(smWINSOCK* lpsmSock);
//ï¿½ï¿½Ç°ï¿½Ìºï¿½Æ®
int SendOpenEventGift(smWINSOCK* lpsmSock);
//ï¿½ï¿½ï¿½ï¿½Â¡ Ã¢ ï¿½ï¿½ï¿½ï¿½
int SendOpenAgingItem(smWINSOCK* lpsmSock);
//Å¬ï¿½ï¿½ ï¿½Þ´ï¿½ ï¿½ï¿½ï¿½ï¿½
int SendOpenClanMenu(smWINSOCK* lpsmSock);

int Serv_ConnectPlayer(smWINSOCK* lpsmSock);

smCHAR* Serv_CheckOtherPlayPosi(int x, int y, int z);

int Serv_DisconnectPlayer(smWINSOCK* lpsmSock);
//ï¿½Ìºï¿½Æ® NPC
int SendOpenEvent(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo, int EventFlag);
//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
int SendOpenGiftExpress(rsPLAYINFO* lpPlayInfo, DWORD	dwItemCode, DWORD dwPassCode_Param);
// pluto ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ã¢ ï¿½ï¿½ï¿½ï¿½
int SendOpenSmelting(smWINSOCK* lpsmSock);
// pluto ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ã¢ ï¿½ï¿½ï¿½ï¿½
int SendOpenManufacture(smWINSOCK* lpsmSock);
//SoD ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½È² ï¿½ï¿½ï¿½
TRANS_SOD_RESULT* rsSod_GetRealTimeScore(rsPLAYINFO* lpPlayInfo);
// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ - ï¿½Í½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ Ã¢ ï¿½ï¿½ï¿½ï¿½
int SendOpenMixtureItemReset(smWINSOCK* lpsmSock);

int createItemLuckyBox(psITEM* lpsItem, rsPLAYINFO* Player, char* itCode, int JobCode);

//Å¬ï¿½ï¿½ï¿½Ì¾ï¿½Æ® ï¿½ï¿½ï¿½ï¿½ ( Å©ï¿½ï¿½Æ¼Ä® ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ )
int DisconnectUser(smWINSOCK* lpsmSock);
//ï¿½ï¿½Å· ï¿½Ãµï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ï·ï¿½ ï¿½ï¿½ï¿½ï¿½
int RecordHackLogFile(rsPLAYINFO* lpPlayInfo, void* lpTransCommand);

//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int RecvRangeAttack(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ( ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ )
int RecvRangeAttackUser(rsPLAYINFO* lpPlayInfo, TRANS_SKIL_ATTACKDATA* lpTransSkilAttackData);
INT64 GetExpForValeLevel(rsPLAYINFO* lpPlayInfo);

int CreateItemPerf(psITEM* lpsItem, char* szCoed, int JobCode);

smCHAR* srFindCharFromSerial(DWORD dwObjectSerial, int AreaNum);
//Ä³ï¿½ï¿½ï¿½Í¸ï¿½ Ã£ï¿½Â´ï¿½ ( ï¿½ï¿½ï¿½ß¿ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö°ï¿½ ï¿½Úµï¿½È­ ï¿½ï¿½ï¿½ï¿½ ï¿½Ù²ï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ )
smCHAR* srFindCharFromSerial(DWORD dwObjectSerial);
//Ä³ï¿½ï¿½ï¿½Í¸ï¿½ Ã£ï¿½Â´ï¿½ ( ï¿½ï¿½ï¿½ß¿ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö°ï¿½ ï¿½Úµï¿½È­ ï¿½ï¿½ï¿½ï¿½ ï¿½Ù²ï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ )
rsPLAYINFO* srFindUserFromSerial(DWORD dwObjectSerial);
//ï¿½×·ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
int rsPlayGrandHealing(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND* lpTransCommand);
//ï¿½ï¿½ï¿½ï¿½
int	rsPlayHealing(rsPLAYINFO* playInfo, smTRANS_COMMAND* lpTransCommand);

//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ã°ï¿½ Ç¥ï¿½ï¿½
int srDisplayBillingTime(rsPLAYINFO* lpPlayInfo, int Add_BillingTime);

//ï¿½Öºï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int	rsSendCommandUser(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND_EX* lpTransCommandEx);
//ï¿½Öºï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int	rsSendCommandUser2(rsPLAYINFO* lpPlayInfo, smTRANS_COMMAND* lpTransCommand);
rsPLAYINFO* FindUserFromName(char* szName);
rsPLAYINFO* FindUserFromName2(char* szName);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Å²ï¿½ï¿½ ( ï¿½ï¿½ï¿½ï¿½ï¿½Å°ï¿½Âµï¿½ ï¿½É¸ï¿½ï¿½ï¿½ ï¿½Ã°ï¿½ï¿½ï¿½ ï¿½ï¿½ 6~10ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ò¿ï¿½ )
int	rsShutDown();
int	rsIsShuttingDown();
int	rsShutDownMinutesLeft();
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ö¼Ò¸ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½î¼­ ï¿½ï¿½È¯
DWORD GetSwapIPCode(char* szIP);

//ï¿½Ìºï¿½Æ® ï¿½Î±×¿ï¿½ ï¿½ï¿½ï¿½ï¿½Ã³ï¿½ï¿½
int rsEventLogOn_Sucess(rsPLAYINFO* lpPlayInfo);


//SOD ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½È£ ï¿½ï¿½ï¿½Ï±ï¿½
int rsGetSoD_AreaCode(int x, int y, int z);
//ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ï¹Ý°ï¿½ï¿½Ý½ï¿½ ï¿½ï¿½Å³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ (ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½)
int rsSendRangeDamage(smCHAR* lpChar, rsPLAYINFO* lpMainPlayInfo, int Range, int SubDamgePers);
//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ä¡ ï¿½ï¿½ï¿½Ï´ï¿½ ï¿½Ô¼ï¿½
int rsGetMonsterLifeRegen(smCHAR* lpChar);

//Å¬ï¿½ï¿½ ï¿½Ó´ï¿½ ï¿½ï¿½ï¿½
int	rsClanMoneyLog(rsPLAYINFO* lpPlayInfo, int Money, int RestMoney);


//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ç¿ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ß°ï¿½
int rsAddServerPotion(rsPLAYINFO* lpPlayInfo, DWORD dwPotionCode, int PotionCount);
//ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ç¿ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ï±ï¿½
int rsGetServerPotion(rsPLAYINFO* lpPlayInfo, DWORD dwPotionCode);

//ï¿½ï¿½Å· ï¿½Ãµï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½Ï·ï¿½ ï¿½ï¿½ï¿½ï¿½
int RecordHackLogFile(rsPLAYINFO* lpPlayInfo, void* lpBuff);


//ï¿½ï¿½Å³ ï¿½ï¿½È¯ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½Å´
smCHAR* OpenMonsterFromSkill(DWORD SkillCode, int x, int y, int z, rsPLAYINFO* lpPlayInfo, smCHAR* lpEnemyChar);
//ï¿½ï¿½È¯ï¿½ï¿½ï¿½ï¿½ï¿½ Ã£ï¿½ï¿½
smCHAR* rsGetLinkChar(smCHAR* lpMasterChar);


//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int rsRecvBlessCastInfo(rsPLAYINFO* lpPlayInfo, void* lpPacket);
//ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä³ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Û½ï¿½
int rsSendBlessCastInfo(rsPLAYINFO* lpPlayInfo, int Mode);

void SendPacket(rsPLAYINFO* lpPlayInfo, char* data, int len, int mode = 0);

//Å¬ï¿½ï¿½ ï¿½Úµï¿½ ï¿½ï¿½ï¿½Ï±ï¿½
DWORD GetClanCode(int ClassClanCode);


//ï¿½Ù¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
int	rsUpdateServerParam(rsPLAYINFO* lpPlayInfo, DWORD dwUpdateCode, int Param1, int Param2, int TargetMode);


///////////////////////// ï¿½ï¿½ï¿½ï¿½×¿ï¿½ ï¿½ï¿½ï¿½ //////////////////////
int rsDebugMessageToClient(rsPLAYINFO* lpPlayInfo, char* szDmgMsg);	//ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½


////////////////////////// ï¿½ï¿½Æ®ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ä¡ ï¿½Ã°ï¿½ï¿½Ð¹ï¿½ /////////////////////////////
int rsGetExp_GameTime(rsPLAYINFO* lpPlayInfo);
// pluto ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ ï¿½Ë»ï¿½ // ï¿½åº° - ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ È¥
DWORD ManufactureCheckRune(rsPLAYINFO* lpPlayInfo, DWORD RecipeCode, DWORD* pRuneCode);

////////////////////////// ï¿½ß±ï¿½ ï¿½Ã°ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½Ã°ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Þ¼ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ ///////////////////
int rsDispay_GameTime(rsPLAYINFO* lpPlayInfo, DWORD dwTimeOld);

// ï¿½ï¿½ï¿½ï¿½ï¿½ - 2010 ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½Ìºï¿½Æ® - ï¿½à±¸ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½
int	rsUseSoccerBallPotionItem(DWORD dwItemCode, rsPLAYINFO* lpPlayInfo);

// ï¿½ï¿½ï¿½ï¿½ï¿½ - ï¿½ï¿½ï¿½ï¿½Â¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½(ï¿½ï¿½ï¿½ï¿½Â¡ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½)
int SetAgingItem(sITEMINFO* pItem, int AgingNum);
int SetAgingItemRestaure(sITEMINFO* pItem, int AgingNum);

int rsSendGameServer_PrimeItem2(rsPLAYINFO* lpPlayInfo);


int rsSendGameServer_PrimeItem3(rsPLAYINFO* lpPlayInfo);
