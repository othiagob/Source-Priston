#pragma once

#include	"sinbaram\sinItem.h"
#include	<atltime.h>
#include "smwsock.h"
#include <Utils/Geometry.h>
#include <string>

#define	smTRANSCODE_OPENLIVE		0x38000000

#define	smTRANSCODE_SYSTEM			0x48400000

#define	smTRANSCODE_CONNECTED		0x48470080
#define smTRANSCODE_RECORDDATA		0x48470081
#define smTRANSCODE_GETRECORDDATA	0x48470082
#define smTRANSCODE_FAILRECORDDATA	0x48470083
#define smTRANSCODE_RECORDRESULT	0x48470084

#define smTRANSCODE_ID_GETUSERINFO	0x48470085
#define smTRANSCODE_ID_SETUSERINFO	0x48470086
#define smTRANSCODE_DELRECORDDATA	0x48470087
#define smTRANSCODE_INSRECORDDATA	0x48470088
#define smTRANSCODE_ISRECORDDATA	0x48470089

#define smTRANSCODE_VERSION			0x4847008A
#define smTRANSCODE_CHECK_NETSTATE	0x4847008B
#define smTRANSCODE_ADMINMODE		0x4847008C
#define smTRANSCODE_SET_OBJSERIAL	0x4847008D
#define smTRANSCODE_CHECK_NETSTATE2	0x4847008E
#define smTRANSCODE_CHECK_NETSTATE3	0x4847008F
#define smTRANSCODE_CHECK_NETSTATE4	0x48470090
#define smTRANSCODE_CHECK_NETSTATE5	0x48470091

#define smTRANSCODE_IPLIST			0x48471000
#define smTRANSCODE_POSITION		0x48471002
#define smTRANSCODE_PLAYLIST		0x48471003
#define smTRANSCODE_CONTINUEDATA	0x48471004
#define smTRANSCODE_WHISPERMESSAGE	0x48471005
#define smTRANSCODE_COUPLERING		0x48471006

#define smTRANSCODE_PLAYDATA1		0x48470010
#define smTRANSCODE_PLAYDATA2		0x48470011
#define smTRANSCODE_PLAYDATA3		0x48470012
#define smTRANSCODE_TRANSPLAYDATA	0x48470013
#define smTRANSCODE_PLAYDATAGROUP	0x48470014
#define smTRANSCODE_PLAY_COMMAND	0x48470018

#define smTRANSCODE_PARTY_CHATMSG	0x48471015

#define smTRANSCODE_ADDEXP_PARTY	0x48470029
#define smTRANSCODE_ATTACKDATA		0x48470030
#define smTRANSCODE_ADDEXP			0x48470031
#define smTRANSCODE_PLAYERINFO2		0x48470032
#define smTRANSCODE_SET_BLACKLIST	0x48470033
#define smTRANSCODE_SET_ITEMCOPYLIST 0x48470034
#define smTRANSCODE_SKIL_ATTACKDATA	0x48470035
#define smTRANSCODE_FINDCRACK		0x48470036
#define smTRANSCODE_MODULE_WARNIG	0x48470037
#define smTRANSCODE_UPDATE_CINFO	0x48470038
#define smTRANSCODE_OPENMONSTER		0x48470039
#define smTRANSCODE_WARPFIELD		0x48470040
#define smTRANSCODE_TRADEITEMS		0x48470041
#define smTRANSCODE_TRADE_REQUST	0x48470042
#define smTRANSCODE_TRADE_ITEMKEY	0x48470043
#define smTRANSCODE_TRADE_READYITEM	0x48470044
#define smTRANSCODE_SKILL_MENU		0x48470045
#define smTRANSCODE_FINDCRACK2		0x48470046
#define smTRANSCODE_WAREHOUSE		0x48470047

#define smTRANSCODE_OPEN_WAREHOUSE	0x48470048
#define smTRANSCODE_OPEN_MIXITEM	0x48470049
#define smTRANSCODE_OPEN_COLLECT	0x4847004A
#define smTRANSCODE_COLLECTMONEY	0x4847004B
#define	smTRANSCODE_PLAYERINFO_Q	0x4847004C
#define smTRANSCODE_OPEN_AGING		0x4847004D

#define smTRANSCODE_SKIL_ATTACKUSER	0x4847004E

#define smTRANSCODE_OPEN_EVENTGIFT	0x4847004F

#define smTRANSCODE_PLAYERINFO		0x48470020
#define smTRANSCODE_GETPLAYERINFO	0x48470021
#define smTRANSCODE_FAILCONNECT		0x48470023

#define smTRANSCODE_AGINGFREE		0x48470024
#define smTRANSCODE_GAMEMASTERMODE	0x48470025


#define smTRANSCODE_PLAYITEM		0x48470050
#define smTRANSCODE_DELITEM			0x48470051
#define smTRANSCODE_PUTITEM			0x48470052
#define smTRANSCODE_THROWITEM		0x48470053
#define smTRANSCODE_SHOP_ITEMLIST	0x48470054
#define smTRANSCODE_SAVETHROWITEM	0x48470055
#define smTRANSCODE_SAVETHROWMONEY	0x48470056
#define smTRANSCODE_SAVETHROWITEM2	0x48470057
#define smTRANSCODE_SAVETHROWITEMS	0x48470058
#define smTRANSCODE_PLAYERKILLING	0x48470059
#define smTRANSCODE_GETITEM			0x4847005A
#define smTRANSCODE_USEITEM_CODE	0x4847005B
#define smTRANSCODE_COMMAND_USER	0x4847005C
#define smTRANSCODE_WING_NPC		0x4847005D
#define smTRANSCODE_PAYMENT_MONEY	0x4847005E
#define smTRANSCODE_MEDIA_NPC		0x4847005F

#define smTRANSCODE_ADD_STARTPOSI	0x48470060
#define smTRANSCODE_DEL_STARTPOSI	0x48470061
#define smTRANSCODE_ADD_NPC			0x48470070
#define smTRANSCODE_DEL_NPC			0x48470071
#define smTRANSCODE_ADMIN_COPYCHAR	0x48470062

#define	smTRANSCODE_PARTY_UPDATE		0x484700A0
#define	smTRANSCODE_PARTY_REQUEST		0x484700A2
#define	smTRANSCODE_PARTY_JOIN			0x484700A3
#define	smTRANSCODE_PARTY_COMMAND		0x484700A4
#define	smTRANSCODE_REQUEST_RAID		0x484700A5
#define	smTRANSCODE_JOIN_RAID			0x484700A7

#define smTRANSCODE_OPEN_MIXTURE_RESET	0x484700A6

#define smTRANSCODE_PCBANG_PET			0x484700B0
#define smTRANSCODE_PHENIX_PET			0x484700B1	
#define smTRANSCODE_HELP_PET			0x484700B2

#define	smTRANSCODE_SERVER_INFO			0x484700C0

#define	smTRANSCODE_PARTY_POTION		0x484700D0

#define	smTRANSCODE_HEALING				0x484700D2
#define	smTRANSCODE_HOLYMIND			0x484700D3
#define	smTRANSCODE_GRAND_HEALING		0x484700D4

#define	smTRANSCODE_VAMPRIC_CUSPID		0x484700D6
#define	smTRANSCODE_VAMPRIC_CUSPID_EX		0x484700D7
#define	smTRANSCODE_MANA_RECHARGING		0x484700D8

#define	smTRANSCODE_RECORD_TRADE		0x48480010


#define smTRANSCODE_NETWORK_QUALITY	0x484700E0
#define smTRANSCODE_PROCESSINFO		0x484700E4
#define	smTRANSCODE_CLOSECLIENT		0x484700E6
#define smTRANSCODE_CHECKIP			0x484700E7

#define	smTRANSCODE_SAVECLIENT		0x484700E8

#define smTRANSCODE_EFFECT_ITEM		0x48478000
#define smTRANSCODE_EFFECT_WEAPON	0x48478002
#define smTRANSCODE_EFFECT_STUN		0x48478004
#define smTRANSCODE_EFFECT_CURSE	0x48478006
#define smTRANSCODE_EFFECT_ICE		0x48478008
#define smTRANSCODE_EFFECT_POISION	0x4847800A
#define smTRANSCODE_EFFECT_VANISH	0x4847800C
#define smTRANSCODE_EFFECT_PERSHOP	0x4847800E
#define smTRANSCODE_EFFECT_LOWPET	0x4847800F
#define smTRANSCODE_EFFECT_CLANMGR	0x48479000
#define smTRANSCODE_EFFECT_FORCEORB	0x48479010
#define smTRANSCODE_EFFECT_CURSE_QUEST	0x48479011
#define smTRANSCODE_EFFECT_BLESS_SCORE	0x48479012
#define smTRANSCODE_EFFECT_KEEPSKILL	0x48479020
#define smTRANSCODE_EFFECT_TOPLEVELSRV	0x48479021

#define smTRANSCODE_EFFECT_PVP_ON 0x48479030
#define smTRANSCODE_EFFECT_PVP_OFF 0x48479031

#define smTRANSCODE_CONT_OBJSERIAL	0x48478010

#define smTRANSCODE_MESSAGEBOX		0x48478100

#define	smTRANSCODE_FUNCERROR		0x48478200
#define smTRANSCODE_FUNCTION_MEM2	0x48478300
#define	smTRANSCODE_SET_BLACKLIST2	0x48478400
#define smTRANSCODE_SET_BLACKLIST3	0x48478500

#define	smTRANSCODE_BAD_PLAYER		0x48478600

#define	smTRANSCODE_FIX_ATTACK		0x48478700
#define	smTRANSCODE_DEAD_PLAYER		0x48478800

#define smTRANSCODE_CLIENT_ERROR	0x48478900

#define	smTRANSCODE_INVEN_ERR_ITEM	0x48478910

#define	smTRANSCODE_INVEN_ERR_MONEY	0x48478920

#define	smTRANSCODE_INVEN_POSITION	0x48478930
#define	smTRANSCODE_INVEN_POSITION2	0x48478931

#define smTRANSCODE_ENCODE_PACKET	0x80010000
#define smTRANSCODE_ENCODE_PACKET2	0x90010000

#define smTRANSCODE_OPEN_CLANMENU	0x48478A00
#define smTRANSCODE_OPEN_STARPOINT	0x48478A10
#define smTRANSCODE_STARPOINT		0x48478A12
#define smTRANSCODE_OPEN_GIVEMONEY	0x48478A14
#define smTRANSCODE_GIVEMONEY		0x48478A16
#define smTRANSCODE_CLANMONEY		0x48478A18

#define smTRANSCODE_ITEM_EXPRESS	0x48478A80

#define smTRANSCODE_OPEN_MYSHOP		0x48478A90
#define smTRANSCODE_CALL_MYSHOP		0x48478AA0
#define	smTRANSCODE_MYSHOP_ITEM		0x48478AB0
#define	smTRANSCODE_MYSHOP_MESSAGE	0x48478AC0
#define	smTRANSCODE_MYSHOP_TRADE	0x48478AD0


#define smTRANSCODE_EFFECT_PCBANGPET	0x48478AE0

#define smTRANSCODE_ADMINMODE2			0x5047108C
#define smTRANSCODE_CHECK_NETSTATE20	0x5047008E

#define smTRANSCODE_CHECK_EXPMONEY		0x50320010
#define smTRANSCODE_CHECK_EXPDATA		0x50320020
#define smTRANSCODE_FORCEORB_DATA		0x50320030
#define smTRANSCODE_PRIMEITEM_DATA		0x50320031


#define smTRANSCODE_PRIMEITEM_DATA2	0x50320032
#define smTRANSCODE_PRIMEITEM_DATA3	0x50320033
#define smTRANSCODE_PRIMEITEM_DATA4	0x50320036

#define smTRANSCODE_BOOSTER_DATA		0x50320034

#define smTRANSCODE_AGING_DATA			0x50320035
#define smTRANSCODE_SKILLDELAY_DATA			0x50320036

#define smTRANSCODE_SEND_DATASERVER		0x50320100
#define smTRANSCODE_SEND_GAMESERVER		0x50320110
#define smTRANSCODE_SEND_AREASERVER		0x50320120
#define smTRANSCODE_SEND_AGAME_SERVER	0x50320130


#define smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA2	0x50320140

#define smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA3	0x50320150
#define smTRANSCODE_SEND_AGAME_PRIMEITEM_DATA4	0x50320160


#define smTRANSCODE_CRAFTITEM			0x50320200
#define smTRANSCODE_AGINGITEM			0x50320202
#define	smTRANSCODE_AGING_UPGRADE		0x50320204
#define	smTRANSCODE_MAKE_LINKCORE		0x50320205
#define	smTRANSCODE_USE_LINKCORE		0x50320206
#define smTRANSCODE_WINGITEM			0x50320208
#define smTRANSCODE_WINGITEMEVENT		0x50320720

#define smTRANSCODE_SHOPTITEM			0x50320210
#define smTRANSCODE_SHOP_SELLITEM		0x50320212

#define	smTRANSCODE_ITEM_FORCEORB		0x50320214

#define	smTRANSCODE_QUEST_COMMAND		0x50320220
#define	smTRANSCODE_EXITINFO_URL		0x50320230

#define smTRANSCODE_CHECKITEM			0x50320300
#define smTRANSCODE_ERRORITEM			0x50320310
#define smTRANSCODE_CLEARPOTION			0x50320320

#define smTRANSCODE_CLIENT_FUNCPOS		0x50320400
#define smTRANSCODE_GETCLIENT_FUNCPOS	0x50320410

#define smTRANSCODE_FUNCTION_MEM		0x50320420
#define smTRANSCODE_FUNCTON_RESULT		0x50320430

#define smTRANSCODE_PROCESS_TIMEMAX		0x50320500

#define smTRANSCODE_NSPRITE				0x50320600
#define smTRANSCODE_NPROTECT			0x50320700
#define smTRANSCODE_GAMEGUARD_AUTH		0x50320710

#define	smTRANSCODE_REC_DAMAGEDATA		0x50320800

#define	smTRANSCODE_SOD_RESULT			0x50320900
#define	smTRANSCODE_SOD_RESULT2			0x50320901

#define	smTRANSCODE_PARTY_SKILL			0x50320A00

#define	smTRANSCODE_CANCEL_SKILL		0x50320A10
#define	smTRANSCODE_PROCESS_SKILL		0x50320A20
#define smTRANSCODE_PROCESS_SKILL2		0x50320A30
#define smTRANSCODE_PROCESS_CLAN_SKILL	0x50320A35
#define	smTRANSCODE_UPDATEL_SKILL		0x50320A40
#define	smTRANSCODE_SUCCESS_EVATION		0x50320A50
#define	smTRANSCODE_RESISTANCE			0x50320A60

#define	smTRANSCODE_PUBLIC_POLLING		0x50320A70
#define	smTRANSCODE_NOTICE_BOX			0x50320A80
#define	smTRANSCODE_HACKTRAP			0x50320A90

#define	smTRANSCODE_VIRTURAL_POTION		0x50320B00
#define	smTRANSCODE_CLAN_SERVICE		0x50320C00
#define	smTRANSCODE_CLAN_UPDATE			0x50320C10

#define	smTRANSCODE_QUEST_MESSAGE		0x50320D00

#define	smTRANSCODE_DEAD_USER			0x50320E00
#define	smTRANSCODE_YAHOO_MOTION		0x50320E10
#define	smTRANSCODE_CHANGE_EVENT		0x50320F00

#define	smTRANSCODE_CLIENTINFO			0x50321000

#define	smTRANSCODE_WARNING_BLINK_ATT	0x50321010

#define smTRANSCODE_ATTACK_DAMAGE		0x50322000
#define smTRANSCODE_RANGE_DAMAGE		0x50322010
#define smTRANSCODE_ATTACK_RESULT		0x50322020
#define smTRANSCODE_DISP_DAMAGE			0x50322030
#define smTRANSCODE_ATTACKDATA2			0x50322040

#define smTRANSCODE_LIMIT_DAMAGE		0x50322050

#define smTRANSCODE_WARNING_CLIENT		0x50322060
#define smTRANSCODE_WARNING_SKILL		0x50322080
#define smTRANSCODE_COMIC_SKIN			0x50322070
#define	smTRANSCODE_REC_ADMIN_COMMAND	0x50322090

#define smTRANSCODE_CHECK_ADMINMODE		0x503220A0

#define smTRANSCODE_SOD_INFOMATION		0x503220B0

#define smTRANSCODE_BLESSCASTLE_INFO	0x503220C0
#define smTRANSCODE_BLESSCASTLE_TAX		0x503220D0

#define smTRANSCODE_ATTACK_SLASH_LIST	0x503220E0
#define smTRANSCODE_UPDATE_SERVER_PARAM	0x503220F0
#define smUPDATE_PARAM_LOWUSER_PRESENT	0x100

#define smTRANSCODE_USER_ID				0x50322100
#define smDELETE_ITEM_PREMIUM			0x50322101

#define smTRANSCODE_DAMAGE_ENCODE_MEM	0x50322F00
#define smTRANSCODE_DAMAGE_ENCODE_MEM2	0x50322EC0

#define	smTRANSCODE_PACKET_DYNFUNC		0x50323000

#define	smTRANSCODE_AREA_SERVER_MAP		0x50324000
#define smTRANSCODE_OPEN_SMELTING		0x50325000
#define smTRANSCODE_SMELTINGITEM		0x50325001
#define smTRANSCODE_OPEN_MANUFACTURE	0x50326000
#define smTRANSCODE_MANUFACTURE			0x50326001
#define smTRANSCODE_MANUFACTURE_WEIGHT_PRICE 0x50326002

#define smTRANSCODE_GIFTBOX_MONEY		0x50326003
#define smTRANSCODE_PREMIUMITEM_INIT	0x50326004
#define smTRANSCODE_MIXTURE_RESET_ITEM	0x50326006

#define smTRANSCODE_EFFECT_CURSE_PVP	0x48479021
#define smTRANSCODE_EFFECT_CURSE_TOPLVLSRV	0x48479022
#define smTRANSCODE_VALE_LEVEL	0x50501001
#define smTRANSCODE_LUCKY_BOX	0x50501002
#define smTRANSCODE_ADD_PLAYER_ARENA	0x50501004
#define smTRANSCODE_UPDATE_PREMIUMITEM_TIME	0x50501005
#define SMTRANSCODE_DISPLAY_DAMAGE 0x50501006
#define SMTRANSCODE_UPDATE_PRIMEITEM 0x50501007
#define SMTRANSCODE_DISPLAY_DEF 0x50501008
#define smTRANSCODE_SKILLS 0x41502865
#define smTRANSCODE_SKINCHANGE 0x50509900
#define smTRANSCODE_OPEN_SKINCHANGE 0x50509901

#define PACKET_ADD_EXP 0x4580608
#define PACKET_WAR_MODE 0x45800609
#define PACKET_ADD_MONEY 0x4580610
#define smTRANSCODE_STARTMOUNT 0x4580611
#define PACKET_CHATITEMLINK 0x4580612

#define	rsHARDCORE_EVENT_FLAG		0x100

#define smCODE_SOD_EFFECT		100

#define smTRANSCODE_EVENT_GAME		0x50326007
#define smCODE_EVENT_STATE_INIT		100


#define smTRANSCODE_ITEMDOC			0x50326008

#define TRANS_BUFF_SIZE		4096

#define	smCHAR_STATE_NPC		0
#define	smCHAR_STATE_ENEMY		1
#define	smCHAR_STATE_USER		0x80

#define	smCHAR_MONSTER_NATURAL	0x80
#define	smCHAR_MONSTER_GOOD		0x81
#define	smCHAR_MONSTER_EVIL		0x82

#define	smCHAR_MONSTER_NORMAL	0x00
#define	smCHAR_MONSTER_UNDEAD	0x90
#define	smCHAR_MONSTER_MUTANT	0x91
#define	smCHAR_MONSTER_DEMON	0x92
#define	smCHAR_MONSTER_MECHANIC	0x93

#define	smCHAR_MONSTER_USER		0xA0
#define	smCHAR_CARAVAN		0x240

#define	smCOMMNAD_USER_WARP		0x100
#define	smCOMMNAD_USER_AGINGUP	0x110
#define	smCOMMNAD_USER_HOLY_INCANTATION	0x120
#define	smCOMMNAD_USER_RESURRECTION	0x130
#define	smCOMMNAD_USER_FORCEORB	0x140
#define	smCOMMNAD_USER_VALENTO	0x150
#define	smCOMMNAD_USER_FIREFLOWER	0x160

#define	smCOMMAND_PLAY_BABEL		0x200
#define	smCOMMAND_PLAY_BELSS_CASTLE	0x210
#define	smCOMMAND_PLAY_PK_FIELD		0x220
#define	smCOMMAND_PLAY_KELVEZU		0x230

struct SKINCHANGE_INFO
{
	int size, code;

	sITEM scItem;
	sITEM scStone;

	DWORD newCode;
};

class	smCHAR;

class	rsRECORD_DBASE;

#define	FALLITEM_MAX			400
#define	FALLITEM2_MAX			3

struct STG_ITEMS;
struct PartyInfo;

struct _FALLITEM{
	DWORD	dwItemCode;
	int		Percentage;
	short	sPrice[2];
};

struct QUEST_BODY {
	int questID;
	int questObjective;
	int questType;
	int monsterNum[3];
	int monsterMap[3];
	int playersToKill;
	int minLevel;
	int maxLevel;

	char wantedMonster[3][32];
	char questName[128];
	char questDescription[512];

	int rewardGold;
	INT64 rewardExp;
	char rewardItem[3][64];

	int qtItens[3];
	char idItem[3][32];
	char itemDescription[3][64];
};


struct updateQuest
{
	char wantedMonster[3][32];
	int monsterNum[3];
	int monsterMap[3];
};

struct QUESTPCKG {
	int size;
	int code;
	int total;
	int totalSent;
	QUEST_BODY QuestPckg[61] = { 0 };
};

struct QUEST_INFO {
	bool isComplete;
	bool isActive;
	bool isReadyToComplete;
	int questID;
	int questType;
	int questObjective;
	int questProgress[3];
	int questMap[3];
	int killProgress;
	SYSTEMTIME EndTime;
};

struct QUEST_INFOPCKG {
	int size;
	int code;
	QUEST_INFO QuestPckg[61] = {0};
};

struct BUFF_KILL
{
	int flag;
	int x;
	int y;
	int alpha;
	int image;
};

#define	NPC_MESSAGE_MAX		20

enum class CaravanTypes
{
	CARAVAN_ARMA,
	CARAVAN_HOPY,
	CARAVAN_BUMA,
	EMPTY
};

enum class CaravanStates
{
	FOLLOW,
	STAY
};

struct Caravan
{
	CaravanTypes CaravanType;
	CaravanStates CaravanState;
	int timeRemaining;
	std::string caravanName;
	smCHAR* Model;

	Caravan()
	{
		CaravanType = CaravanTypes::EMPTY;
		CaravanState = CaravanStates::FOLLOW;
		timeRemaining = 0;
		caravanName = "";
		Model = { 0 };
	}
};

struct smCHAR_MONSTER_INFO {
	char	szName[32];			//ÀÌ¸§

	short	SkillDamage[2];		//½ºÅ³ °ø°Ý·Â
	int		SkillRange;			//½ºÅ³ Àû¿ë ¹üÀ§
	int		SkillRating;		//½ºÅ³ °ø°Ý·ü
	int		SkillDistance;		//Àû¿ë ½ºÅ³ÀÇ °Å¸® 
	RECT	SkillRangeRect;		//¹üÀ§ ½ºÅ³Àû¿ë½Ã À§Ä¡
	int		SkillCurse;			//ÀúÁÖ °É±â

	int		AttackPattern;		//°ø°Ý ÆÐÅÏ
	int		ActiveHour;			//È°µ¿½Ã°£
	int		IQ;					//Áö´ÉÁö¼ö
	int		Nature;				//Ç°¼º
	int		Undead;				//¾ðµ¥µå À¯¹«
	int		GenerateGroup[2];	//µ¿½Ã¿¡ ÃâÇö¼ö ( ÃÖ¼Ò / ÃÖ´ë )
	INT64		GetExp;				//Á×¾úÀ»¶§ °æÇèÄ¡ ºÎ¿©·®
	int		SpAttackPercetage;	//Æ¯¼ö °ø°Ý »ç¿ë È®·ü
	int		MoveRange;			//ÀÌµ¿¹üÀ§

	int		DamageStunPers;		//°ø°Ý¹ÞÀ»½Ã ½ºÅÏÈ®·ü

	int		PotionCount;		//¹°¾à º¸À¯¼ö
	int		PotionPercent;		//¹°¾à º¸À¯·ü

	int		AllSeeItem;			//¶³¾îÁú‹š ´Ùº¸ÀÎ´Ù
	int		FallItemCount;		//¶³¾î¶ß¸± ¾ÆÀÌÅÛµéÀÇ Ä«¿îÅÍ
	int		FallItemPerMax;		//¾ÆÀÌÅÛ »ý¼º È®·ü ÃÖ´ëÄ¡
	int		FallItemMax;		//µ¿½Ã¿¡ ¶³¾î¶ß¸± ¾ÆÀÌÅÛ ÃÖ´ëÄ¡
	int		FallItemPlusCount;	//¶³¾î¶ß¸± ¾ÆÀÌÅÛµéÀÇ Ä«¿îÅÍ (Ãß°¡)

	int		ClassCode;			//¸ó½ºÅÍ ±¸º° ÄÚµå

	int		UseEventModel;		//ÀÌº¥Æ® ¸ðµ¨ ÀúÀåÀ¯¹«

	int		Real_Sight;			//½ÇÁ¦ ½Ã¾ß °Å¸® ( Á¦°ö±Ù ¾Æ´Ô )

	_FALLITEM	FallItems[FALLITEM_MAX];
	_FALLITEM	FallItems_Plus[FALLITEM2_MAX];		//Ãß°¡¾ÆÀÌÅÛ


	//NPC »óÁ¡ ÆÇ¸Å Á¤º¸
	int		SellAttackItemCount;
	void	*SellAttackItem[32];	//°ø°Ý ¾ÆÀÌÅÛ
	int		SellDefenceItemCount;
	void	*SellDefenceItem[32];	//¹æ¾î ¾ÆÀÌÅÛ
	int		SellEtcItemCount;
	void	*SellEtcItem[32];		//±âÅ¸ ¾ÆÀÌÅÛ

	int		SkillMaster;			//½ºÅ³ ¼ö·Ã NPC
	int		SkillChangeJob;			//ÀüÁ÷ °¡´É NPC
	int		WareHouseMaster;		//¾ÆÀÌÅÛ º¸°ü NPC
	int		ItemMix;				//¾ÆÀÌÅÛ Á¶ÇÕ NPC
	int		ItemAging;				//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡
	int		CollectMoney;			//µ·¸ð±Ý NPC
	int		EventGift;				//°æÇ° ÀÌº¥Æ® NPC
	int		Smelting;				// pluto	Á¦·Ã NPC
	int		Manufacture;			// pluto	Á¦ÀÛ NPC
	int		MixtureReset;			// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â NPC
	int     ItemShop;
	int     ItemShopTime;
	int		BPShop;
	int		NpcQuest;
	int		NpcArena;
	int		NpcRanking;
	int		NpcListaMix;
	int		NpcSodRanking;
	int     Caravana;
	int		CaravanMapNum;
	int		Skin;
	DWORD   GoldTimeIn;
	STG_ITEMS *GoldToCollet;
	CaravanTypes CaravanType;
	CaravanStates CaravanState;

#ifdef _LANGUAGE_VEITNAM
	int		WowEvent;
#endif

	int		EventCode;				//ÀÌº¥Æ® ÄÚµå
	int		EventInfo;				//ÀÌº¥Æ® Á¤º¸
	DWORD	dwEvnetItem;			//ÀÌº¥Æ® ¾ÆÀÌÅÛ
	int		EventNPC;				//ÀÌº¥Æ® NPC Á¤º¸
	int		ClanNPC;				//Å¬·» NPC
	int		GiftExpress;			//°æÇ° ¹è´Þ
	int		WingQuestNpc;			//À® Äù½ºÆ® NPC
	int		StarPointNpc;			//º° Æ÷ÀÎÆ® NPC
	int		GiveMoneyNpc;			//µ· ¸ð±Ý NPC
	int		TelePortNpc;			//ÅÚ·¹Æ÷Æ® NPC
	int		BlessCastleNPC;			//ºí·¹½º Ä³½½ NPC
	int		PollingNpc;				//¼³¹®Á¶»ç NPC

	char	*szMediaPlayNPC_Title;		//µ¿¿µ»ó NPC
	char	*szMediaPlayNPC_Path;		//µ¿¿µ»ó NPC

	int		QuestCode;				//Äù½ºÆ® Ã³¸®ÄÚµå
	int		QuestParam;				//Äù½ºÆ® ÆÄ¶ó¸ÞÅÍ

	short	OpenCount[2];			//ÃâÇö °£°Ý ¹øÈ£

	DWORD	dwAutoCharCode;			//ÀÚµ¿Ä³¸¯ÅÍ ±¸ºÐ ÄÚµå (SpeedSum °ª)

	char	*lpNpcMessage[NPC_MESSAGE_MAX];	//NPC ´ëÈ­ ¸Þ¼¼Áö
	int		NpcMsgCount;					//NPC ´ëÈ­ Ä«¿îÅÍ

};

/*----------------------------------------------------------------------------*
*				(Ä³¸¯ÅÍ ½ºÅ×ÀÌÅÍ½º)Ä³¸¯ÅÍ Á¤º¸ ±¸Á¶Ã¼
*-----------------------------------------------------------------------------*/

struct smCHAR_INFO {
	char	szName[32];				//ÀÌ¸§
	char	szModelName[64];		//¸ðµ¨ ÀÌ¸§
	char	szModelName2[60];		//¸ðµ¨ ÀÌ¸§2  // szModelName2[64] -> szModelName2[60] ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	DWORD	ModelNameCode2;			//¸ðµ¨ ÄÚµå Ãß°¡ (Àå¼ø¸ñ) // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤

	DWORD	dwObjectSerial;			//°´Ã¼ÀÇ °íÀ¯°ª

	//int		AutoPlayer;				//ÀÚµ¿ ¼Ó¼º
	int		ClassClan;				//¼Ò¼ÓÅ¬·£
	int		State;					//°´Ã¼ÀÇ ¼Ó¼º ( smCHAR_STATE_  NPC / ENEMY / USER )
	int		SizeLevel;				//Å©±â ´Ü°è ( 0-¼ÒÇü ~ 3-´ëÇü )
	DWORD	dwCharSoundCode;		//»ç¿îµå Ä³¸¯ÅÍ ÄÚµå

	/*-------------------------*
	*			´É·ÂÄ¡ 
	*--------------------------*/
	DWORD JOB_CODE;			//Á÷¾÷ ÄÚµå 
	int   Level;			//·¹º§ 
	int   Strength;			//Èû 
	int   Spirit;			//Á¤½Å·Â 
	int   Talent;			//Àç´É 
	int   Dexterity;		//¹ÎÃ¸¼º 
	int   Health;			//°Ç°­ 

	/*-------------------------*
	*			ÀüÅõ°ü·Ã
	*--------------------------*/
	int	  Accuracy;			//»ó´ëÀû ¸íÁß·ü
	int   Attack_Rating;	//¸íÁß·ü
	int   Attack_Damage[2];	//Å¸°Ý¼öÄ¡ ( ÃÖ¼Ò / ÃÖ´ë )
	int	  Attack_Speed;		//°ø°Ý¼Óµµ
	int	  Shooting_Range;	//»çÁ¤°Å¸® ( ¹ß»çÇü¹«±â )
	int	  Critical_Hit;		//Å©¸®Æ¼ÄÃÈý ( 1.5¹è µ¥¹ÌÁö È®·ü )

	int   Defence;			//¹æ¾îÀ² 			
	int   Chance_Block;		//ºí·°À² 
	int   Absorption;		//Èí¼öÀ² 

	int   Move_Speed;		//ÀÌµ¿ ¼Óµµ 
	int   Sight;			//½Ã¾ß
	short Weight[2];			//¿î¹Ý ´É·Â 

	/*-------------------------*
	*			ÀúÇ×·Â 
	*--------------------------*/
	short	Resistance[8];			//¿ø¼Ò¿¡´ëÇÑ ÀúÇ×·Â 
	short	Attack_Resistance[8];	//¿ø¼Ò¿¡´ëÇÑ °ø°Ý·Â 

	/*-------------------------*
	*			...
	*--------------------------*/
	short   Life[2];			//0 ÇöÀç 1 ÃÖ´ëÄ¡ 
	short   Mana[2];			//0 ÇöÀç 1 ÃÖ´ëÄ¡ 
	short   Stamina[2];			//0 ÇöÀç 1 ÃÖ´ëÄ¡

	float	Life_Regen;			//Àç»ý´É·Â 
	float	Mana_Regen;			//Àç»ý´É·Â 
	float	Stamina_Regen;		//Àç»ý´É·Â 

	INT64		Exp;
	INT64		Next_Exp;

	int		Money;				//º¸À¯ÇÑ µ·

	smCHAR_MONSTER_INFO	*lpMonInfo;	//¸ó½ºÅÍ ±¸Á¶Ã¼ Æ÷ÀÎÅÍ

	DWORD	Brood;				//Á¾Á·

	int		StatePoint;			//´É·ÂÄ¡ Æ÷ÀÎÆ®
	BYTE	bUpdateInfo[4];		//³»¿ë º¯°æ Ä«¿îÅÍ
	short	ArrowPosi[2];		//ÀÎº¥Åä¸® È­»ìÇ¥ À§Ä¡
	int		Potion_Space;		//Æ÷¼Ç ÁÖ¸Ó´Ï Å©±â

	int     LifeFunction;		//»ý¸í·Â ÇÔ¼ö 
	int     ManaFunction;		//±â·Â ÇÔ¼ö 
	int     StaminaFunction;	//±Ù·Â ÇÔ¼ö 
	short   DamageFunction[2];  //0 ±ÙÁ¢ 1 ¿ø°Å¸®   

	DWORD	RefomCode;			//¸®Æû ÄÚµå

	DWORD	ChangeJob;
	DWORD	JobBitMask;

	WORD	wPlayerKilling[2];	//°¨¿Á°¤Çô ÀÖÀ½
	WORD	wPlayClass[2];		//°è±Þ ( ¸ó½ºÅÍÀÇ º¸½º¼³Á¤ )

	int		Exp_High;			//°æÇèÄ¡ »óÀ§4ºñÆ®
	DWORD	dwEventTime_T;		//ÀÌº¥Æ® Á¦ÇÑ ½Ã°£ - ( ¾óÅ«ÀÌ ½Ã°£ )
	short	sEventParam[2];		//ÀÌº¥Æ® ÆÄ¶ó¸ÞÅÍ¿ë [0] ÀÌº¥Æ®ÄÚµå [1] ÀÌº¥Æ® ÆÄ¶ó¸ÞÅÍ

	short	sPresentItem[2];	//¿ì¿¬À» °¡ÀåÇÑ ÇÊ¿¬ÀûÀÎ ¾ÆÀÌÅÛ Áö±Þ

	// Àåº° - ±×¶óºñÆ¼ ½ºÅ©·Ñ
	short GravityScroolCheck[2];

	DWORD	dwTemp[11];			//¿¹ºñ ¹öÆÛ // Àåº° - ±×¶óºñÆ¼ ½ºÅ©·Ñ(12->11)

	DWORD	dwLoginServerIP;		//·Î±×ÀÎÇÑ ¼­¹öÀÇ IP
	DWORD	dwLoginServerSafeKey;	//·Î±×ÀÎÇÑ ¼­¹ö¿¡¼­ º¸³½ º¸¾ÈÅ°

	WORD	wVersion[2];		//ÀÌ ±¸Á¶Ã¼ÀÇ ¹öÀü

	bool bPartyLeader;
};

//Ä³¸¯ÅÍ Á¤º¸ ´ÜÃà Å©±â
#define	smCHAR_INFO_QUICK_SIZE		192
#define	smCHAR_INFO_QUICK_SIZE2		352

#define	THROW_ITEM_INFO_MAX		64
#define	THROW_ITEM_INFO_MASK	63
#define	INVEN_ITEM_INFO_MAX		300


struct	sTHROW_ITEM_INFO {

	DWORD	dwCode;
	DWORD	dwKey;
	DWORD	dwSum;
};


struct rsPLAYPOS 
{
	DWORD	Area;
	int		x,y,z;
	Point3D Position;
};


struct TRANS_PLAYPOS {
	int size,code;
	
	rsPLAYPOS	rsPlayPos;
	short Hp[2];
};


struct TRANS_ATTACKDATA {
	int size,code;

	DWORD	dwDestObjectSerial;	//°ø°ÝÇÏ´Â Ä³¸¯ÅÍ °íÀ¯¹øÈ£
	DWORD	dwTarObjectSerial;	//°ø°Ý´ë»ó Ä³¸¯ÅÍ °íÀ¯¹øÈ£

	int x,y,z;			// ÁÂÇ¥
	int AttackState;	// ¼Ó¼º
	int AttackSize;		// °ø°Ý ¹üÀ§
	int Power;			// °ø°Ý ÆÄ¿ö
	DWORD	dwChkSum;

	short	sRating[2];	//[0]-·¹º§ [1]-¸íÁß·Â
	DWORD	dwDamageChkSum;
};

struct TRANS_ATTACKDATA2 {
	int size,code;

	DWORD	dwChkSum;

	DWORD	dwDestObjectSerial;	//°ø°ÝÇÏ´Â Ä³¸¯ÅÍ °íÀ¯¹øÈ£
	DWORD	dwTarObjectSerial;	//°ø°Ý´ë»ó Ä³¸¯ÅÍ °íÀ¯¹øÈ£

	int		x,y,z;			// ÁÂÇ¥
	int		AttackState;	// ¼Ó¼º
	int		AttackSize;		// °ø°Ý ¹üÀ§
	short	Power[2];		// °ø°Ý ÆÄ¿ö
	short	Power2[2];		// °ø°Ý ÆÄ¿ö ( Ãß°¡ °ø°Ý )
	short	Critical[2];	// Å©¸®Æ¼ÄÃ
	int		SkillCode;		// ½ºÅ³ÄÚµå
	DWORD	dwTime;			// Å¬¶óÀÌ¾ðÆ® ½Ã°£
	int		AttackCount;	// °ø°Ý Ä«¿îÅÍ
	short	MotionCount[2];	// °ø°Ý ¸ð¼Ç Ä«¿îÅÍ
	DWORD	dwWeaponCode;	// ÁÖ °ø°Ý ¹«±â
	short	Area[2];		// Áö¿ª
	int		Temp[4];		// ¿¹ºñ
};


// Àåº° - ¹üÀ§Çü ½ºÅ³( 32 -> 42 )
#define SKIL_ATTACK_CHAR_MAX		42

struct TRANS_SKIL_ATTACKDATA {
	int size,code;

	DWORD	dwDestObjectSerial;	//°ø°ÝÇÏ´Â Ä³¸¯ÅÍ °íÀ¯¹øÈ£

	int x,y,z;			// ÁÂÇ¥
	int AttackState;	// ¼Ó¼º
	int AttackSize;		// °ø°Ý ¹üÀ§
	int Power;			// °ø°Ý ÆÄ¿ö

	DWORD	dwChkSum;	//Á¶ÀÛ ¹æÁö ÄÚµå

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//°ø°Ý´ë»ó Ä³¸¯ÅÍ °íÀ¯¹øÈ£
};

struct TRANS_SKIL_ATTACKDATA2 {
	int size,code;

	DWORD	dwDestObjectSerial;	//°ø°ÝÇÏ´Â Ä³¸¯ÅÍ °íÀ¯¹øÈ£

	DWORD	dwChkSum;

	int		x,y,z;			// ÁÂÇ¥
	int		AttackState;	// ¼Ó¼º
	int		AttackSize;		// °ø°Ý ¹üÀ§
	short	Power[2];		// °ø°Ý ÆÄ¿ö
	short	Power2[2];		// °ø°Ý ÆÄ¿ö ( Ãß°¡ °ø°Ý )
	short	Critical[2];	// Å©¸®Æ¼ÄÃ
	int		SkillCode;		// ½ºÅ³ÄÚµå
	DWORD	dwTime;			// Å¬¶óÀÌ¾ðÆ® ½Ã°£
	int		AttackCount;	// °ø°Ý Ä«¿îÅÍ
	short	MotionCount[2];	// °ø°Ý ¸ð¼Ç Ä«¿îÅÍ
	DWORD	dwWeaponCode;	// ÁÖ °ø°Ý ¹«±â
	short	Area[2];		// Áö¿ª
	int		Temp[4];		// ¿¹ºñ

	DWORD	dwMainTargetObject;		//ÁÖ Å¸°Ù

	int	TargetCount;
	DWORD	dwTarObjectSerial[SKIL_ATTACK_CHAR_MAX];	//°ø°Ý´ë»ó Ä³¸¯ÅÍ °íÀ¯¹øÈ£
};



//³»¿ë : Áö¿ª¿¡ Á¸ÀçÇÏ´Â ´Ù¸¥ ÇÃ·¹ÀÌ¾î Á¤º¸ ¸ñ·Ï Àü¼Û
//´ë»ó : ¼­¹ö -> Å¬¶óÀÌ¾ðÆ®
//ÄÚµå : smTRANSCODE_IPLIST
struct TRANS_AREAINFO_HEADER {
	int		size;
	int		code;
	DWORD	dwIP;
	int		Stats;					//Àü¼Û ¿Ï·á¸é TRUE ¾ÆÁ÷ ´õ º¸³¾ µ¥ÀÌÅ¸°¡ ÀÖÀ¸¸é FALSE
	int		TransListCnt;			//Àü¼ÛÇÏ´Â TRANS_IPLIST °¹¼ö
};
struct TRANS_IPLIST {
	DWORD	dwIP;
	rsPLAYPOS	Pos;
};

/////////////// ¾ÆÀÌÅÛ ÅÃ¹è ¼­ºñ½º /////////////////
#define	POST_ITEM_MAX				500

struct _POST_BOX_ITEM {
	int		Flag;
	char	szCharName[32];
	char	szItemCode[32];
	char	szSpeJob[32];
	char	szDoc[128];
	char	szFormCode[64];
	char	szPassCode[16];

	DWORD	dwItemCode;
	DWORD	dwJobCode;
	DWORD	dwFormCode;
	DWORD	dwPassCode;
	DWORD	dwParam[4];
};

struct	rsPOST_BOX_ITEM {
	DWORD	dwHead;
	int		ItemCounter;

	_POST_BOX_ITEM	PostItem[POST_ITEM_MAX];
};

struct	TRANS_POST_ITEM {
	int		size,code;

	char	szItemName[32];
	char	szDoc[128];
	DWORD	dwItemCode;
	DWORD	dwItemJobCode;
	DWORD	dwParam[4];

	DWORD	dwItemFlag;
	int		Weight;			// pluto ÇØ¿Ü ºô¸µ ¾ÆÀÌÅÛ ¹«°Ô
};
//	sITEM_INFO	sItemInfo;


///////////////////////////////////////////////////////


#define	srPLAYBUFF_MAX		4
#define	srPLAYBUFF_MASK		3

#define	PARTY_PLAYER_MAX		6

// Àåº° - ¿¹ºñ¹öÆÛ
#define	srRECORD_DEFAULT_SIZE	16384
//#define	srRECORD_DEFAULT_SIZE	32768
#define	srRECORD_MEMORY_SIZE	24576
//#define	srRECORD_MEMORY_SIZE	49152

#define SKILL_POINT_COLUM_MAX	16

struct	TRANS_PARTY_PLAYINFO;

struct	npGAME_GUARD_AUTH_INFO {
	 DWORD	Auth_Value;		//°ÔÀÓ°¡µå È®ÀÎÄÚµå ( °ªÀ» ¸®ÅÏ ¹ÞÀ½ )
	 DWORD	Auth_Time;		//°ÔÀÓ°¡µå È®ÀÎ½Ã°£ ( ´ÙÀ½Ã³¸® ½Ã°£ )
	 DWORD	Auth_Count;		//°ÔÀÓ°¡µå È®ÀÎÄ«¿îÅÍ ( ÃÑ º¸³½ È½¼ö )
	 DWORD	Auth_Retry;		//°ÔÀÓ°¡µå È®ÀÎ Àç½Ãµµ ( ÀÀ´äÀÌ 60ÃÊ ¾È¿¡ ¾È¿À¸é ¸î¹ø±îÁö Àç½Ãµµ )
};

//¾ÆÀÌÅÛ ±¸¸Å Ä«¿îÅÍ
struct	sMYSHOP_SELL_INFO {
	DWORD	dwCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
	int		Price;
};

#define	MYSHOP_LIST_MAX		32
#define	MYSHOP_LIST_MASK	31


struct	sCASTLE_MASTER {
	int		Flag;
	DWORD	dwCheckTime;
	int		SelectSkill;
};

//µ¥ÀÌÅ¸ ¿¡·¯ ºñÆ® ÇÃ·¢
#define	rsDATA_ERROR_WAREHOUSE			1


//°£´ÜÇÑ ÄÚ¸àµå Àü¼Û¿ë ±¸Á¶
struct smTRANS_COMMAND
{
	int	size;
	int code;

	int LParam;
	int WParam;
	int SParam;
	int EParam;
};

struct smTRANS_EXP64
{
	int	size;
	int code;

	INT64 LParam;
	INT64 WParam;
	INT64 SParam;
	INT64 EParam;
};

//¹ÚÀç¿ø - XignCode
#define PACKET_SIZE 512


//¹ÚÀç¿ø - XignCode
typedef struct _XIGNCODE_PACKET {
	int size;
	int code;
	BYTE data[PACKET_SIZE]; //¹ÚÀç¿ø - XignCode
} XIGNCODE_PACKET;

typedef struct _XIGNCODE_TempData{
	BOOL m_type;
	BOOL m_ceComplete;
	DWORD	dwWaitTime_NC;
	DWORD	dwWaitTime_CE;
	BOOL   dwClientReady;
	HANDLE xigncode_handle;
	int m_pcount;
} XIGNCODE_TempData;

//¹ÚÀç¿ø - XignCode
#define smTRANSCODE_XIGNCODE_PACKET		0x48470400
#define smTRANSCODE_XIGNCODE_START		0x48470500


//typedef struct _WSABUF {
//	u_long      len;     
//	char FAR *  buf;     
//} WSABUF, FAR * LPWSABUF;



//////////////////// ¿¢½ºÆ®·¦ ////////////////////
#ifdef _xTrap_GUARD

#include "XTrapSvrSrc/XTrapCC.h"
//smPacket.h
#define	smTRANSCODE_XTRAP_PACKET		0x48470300
#define	smXTRAP_COMMAND_LOGIN			0x00001000
#define	smXTRAP_COMMAND_SECURITY		0x00001010

struct	smTRANS_XTRAP_ADDR {
	smTRANS_COMMAND	smTransCommand;
	char Addr[528];				/* Client CREATEKF_IN_SIZE == Server XC_ADDR_SIZE */

};
struct	smTRANS_XTRAP_SECURITY {

	smTRANS_COMMAND	smTransCommand;
	char serialkey[1088];		/* Client CREATEKF_OUT_SIZE == Server XC_CLIENTBUF_SIZE */

};

struct	smXTRAP_PLAYINFO {
	//XC_CMF				Cmf[2];
	XC_KEYVALUE			KeyValue[2];
	DWORD				dwKeyGenTime;
	int					KeyGenWaitCount;
};

#ifdef _W_SERVER
#define _xTrap_GUARD_SERVER
#endif

#endif
////////////////////////////////////////////////////

//////////////////// ¿¢½ºÆ®·¦ ////////////////////
#ifdef _XTRAP_GUARD_4_

#ifdef _W_SERVER

#define _XTRAP_GUARD_4_SERVER
#include "XTrapSrcD5\\Server\\XTrap4Server.h"	//XTrapD5

#else

#define _XTRAP_GUARD_4_CLIENT

#endif

#include "XTrapSrcD5\\Server\\MessageProtocol.h"
//smPacket.h
#define	smTRANSCODE_XTRAP_PACKET		0x48470300
#define	smXTRAP_COMMAND_LOGIN			0x00001000
#define	smXTRAP_COMMAND_SECURITY		0x00001010


struct	smTRANS_XTRAP_ADDR {
	smTRANS_COMMAND	smTransCommand;
	char PacketBuf[XTRAP_CC4_BUFFERSIZE_COMMUNICATION_BASE];
};


#endif
////////////////////////////////////////////////////

/////////////////////// ¼­¹ö¿¡¼­ÀÇ ÇÃ·¹ÀÌ¾î Á¤º¸ ±¸Á¶Ã¼ ////////////////////////////////
class rsPLAYINFO {
public:

	char			szServerID[32];			//¼­¹öID		00

	char			szID[32];				//À¯Àú ID	32
	char			szName[32];				//À¯Àú Ä³¸¯ÅÍ ÀÌ¸§	64
	DWORD			dwCode_ID;				//ID¸¦ »¡¸® Ã£±âÀ§ÇÑ ÄÚµå	96
	DWORD			dwCode_Name;			//NameÀ» »¡¸® Ã£±â À§ÇÑ ÄÚµå	100

	int				AdminOperMode;			//°ü¸®ÀÚ º¹±¸ ·ÎµùÁß	104

	DWORD			dwDataError;			//µ¥ÀÌÅ¸ ¿¡·¯ ºñÆ® ÇÃ·¢	108

	int				TransInfoFlag;			//Åë½Å ÇÃ·¢ ( 0-ÁØºñÁß 1-»ç¿ë°¡´É )	112
	int				ChatBuffCnt;			// 116

	int				SystemLine;				//½Ã½ºÅÛ ¸ðµå - ´Ù¸¥ ¼­¹ö¿¡¼­ ¿¬°áµÊ	120

	int				CharLoadingCount;		//Ä³¸¯ÅÍ ·Îµù ½Ãµµ Ä«¿îÅÍ (ÇÑ°ÔÀÓ µ·º¹»ç Á¶»çÁß)	124

	rsPLAYPOS		Position;
	rsPLAYINFO		**lpAreaPlayInfo;		//Áö¿ªº° ÇÃ·¹ÀÌ¾î Á¤º¸ ¸µÅ©Æ÷ÀÎÅÍ 

	int				SendListCnt;

	DWORD			dwDecPacketCode;		//ÆÐÅ¶ ¾ÏÈ£ ÄÚµå
	BYTE			bPacketXor[2];			//ÆÐÅ¶ ¾ÏÈ£ ÇØµ¶¿ë °ª
	DWORD			dwDecPacketTime;		//ÆÐÅ¶ ¾ÏÈ£ Àû¿ë ½Ã°£
	DWORD			dwDecPacketTime2;		//ÆÐÅ¶ ¾ÏÈ£ Àû¿ë º¸·ù ½Ã°£

	smCHAR_INFO		smCharInfo;				//Ä³¸¯ÅÍ Á¤º¸

	int				CharLevelMax;			//Ä³¸¯ÅÍ ÃÖ°í ·¹º§ ( ÇöÀç º¸À¯ÇÑ °èÁ¤¿¡ ÀÖ´Â Ä³¸¯ÅÍÁß ÃÖ°í·¹º§ )
	DWORD			dwNoticeCount;			//°øÁö»çÇ× ¸Þ¼¼Áö Ä«¿îÅÍ

	char			srPlayBuff[srPLAYBUFF_MAX+1][TRANS_BUFF_SIZE];		//Àü¼ÛÇÒ µ¥ÀÌÅ¸ ´ë±â¿ë ¹öÆÛ
	int				srPlayBuffSize[srPLAYBUFF_MAX+1];					//Àü¼ÛÇÒ µ¥ÀÌÅ¸ ´ë±â¿ë ¹öÆÛ Å©±â

	int				PlayBuffSleep;									//Àü¼Û µ¥ÀÌÅ¸ ½½¸³

	sTHROW_ITEM_INFO	ThrowItemInfo[THROW_ITEM_INFO_MAX];			//¹ö·ÁÁø ¾ÆÀÌÅÛ Á¤º¸

	sTHROW_ITEM_INFO	InvenItemInfo[INVEN_ITEM_INFO_MAX+16];			//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ Á¤º¸
	sTHROW_ITEM_INFO	WareHouseItemInfo[120];		
	sTHROW_ITEM_INFO	CaravanItemInfo[120];
			//Ã¢°í ¾ÆÀÌÅÛ Á¤º¸
	int				OpenWarehouseInfoFlag;			
	int				OpenCaravanInfoFlag;	//Ã¢°í ¾ÆÀÌÅÛ Á¤º¸ À¯¹«
	//sTHROW_ITEM_INFO	*lpWareHouseItemInfo;						//Ã¢°í ¾ÆÀÌÅÛ ³»¿ë Á¤º¸

	DWORD			dwTradePotionInfo[16];							//¹°¾à °Å·¡ Á¤º¸ ÀÓ½ÃÀúÀå
	int				TradePotionInfoCount;							//¹°¾à °Å·¡ Á¤º¸ ÀúÀå Ä«¿îÅä

	sMYSHOP_SELL_INFO	sMyShopList[ MYSHOP_LIST_MAX ];					//°³ÀÎ »óÁ¡ °Å·¡ ¸ñ·Ï ¹öÆÛ
	int				MyShopListCount;								//°³ÀÎ »óÁ¡ °Å·¡ ½Ãµµ ¸ñ·Ï Ä«¿îÅÍ
	DWORD			dwMyShopTradeTime;								//°³ÀÎ»óÁ¡ °Å·¡ ´ë±â ½Ã°£

	int				ThrowItemCount;											//¹ö·ÁÁø ¾ÆÀÌÅÛ Ä«¿îÅÍ

	int				UnsaveMoney;									//ÀúÀåµÇÁö ¾ÊÀº µ·

	int				ServerMoney;									//¼­¹ö¿¡¼¼ ½ÇÁ¦ °è»êÇÑ µ·
	int				WareHouseMoney;									//Ã¢°í ±Ý¾×

	DWORD			dwRecvTimePlayBuff;								//Àü¼Û¹ÞÀº ½Ã°£
	DWORD			srSendPlayBuffCnt;								//º¸³½ ¹öÆÛ Ä«¿îÅÍ
	DWORD			srRecvPlayBuffCnt;								//¹ÞÀº ¹öÆÛ Ä«¿îÅÍ

	DWORD			dwObjectSerial;	//°´Ã¼ÀÇ °íÀ¯°ª

	int				ChatMsgMode;			//Ã¤ÆÃ ¸Þ¼¼Áö ¸ðµå
	char			szChatMessage[256];		//Ã¤ÆÃ ¸Þ¼¼Áö

	//»óÁ¡
	char			szTradeMessage[128];	//»óÁ¡ ¸Þ¼¼Áö ¹®ÀÚ¿­
	DWORD			dwTradeMsgCode;			//»óÁ¡ ¸Þ¼¼Áö ÄÚµå

	smWINSOCK		*lpsmSock;

	DWORD			dwLastRecvTime;
	DWORD			dwLastMotionRecvTime;

	int				UpdateInfo_WaitCounter;	//¾÷µ¥ÀÌÆ® Á¤º¸ ´ë±â Ä«¿îÅÍ
	int				UpdateInfo_WarnigCounter;	//¾÷µ¥ÀÌÆ® Á¤º¸ °æ°í (´©ÀûµÇ¸é ¿¬°á Á¾·á )

	rsRECORD_DBASE	*lpRecordBase;			//ÀúÀå Å¬·¡½º Æ÷ÀÎÅÍ
	rsPOST_BOX_ITEM	*lpPostBoxItem;			//¾ÆÀÌÅÛ ¹è´Þ ±¸Á¶Ã¼
	int				PostPassFailCount;		//¾ÆÀÌÅÛ ¹è´Þ ºñ¹Ð¹øÈ£ ½ÇÆÐ È½¼ö

	int				AdminMode;				//°ü¸®ÀÚ ¸ðµå
	int				HideMode;				//Åõ¸í ¸ðµå ( °ü¸®ÀÚ°¡ ³²¿¡°Ô ¾Èº¸ÀÌ°Ô ´Ù´Ò¶§ )
	int				AdminIP;				//°ü¸®ÀÚ ¾ÆÀÌÇÇ ÁÖ¼Ò·Î Á¢¼Ó

	DWORD			dwConnectTime;			//Á¢¼ÓÇÑ ½Ã°£

	int				LastTransPlayUserCount;	//¸¶Áö¸·À¸·Î Àü¼ÛµÈ ´Ù¸¥ ÇÃ·¹ÀÌ¾î ¹øÈ£
	int				LastTransAutoPlayCount;	//¸¶Áö¸·À¸·Î Àü¼ÛµÈ ´Ù¸¥ ÀÚµ¿ Ä³¸¯ÅÍ ¹øÈ£

	DWORD			dwConnectCount;			//ÀüÃ¼ ¿¬°á Ä«¿îÅÍ

	int				frame;
	DWORD			SendPlayGroupCount;		//º¸³½ µ¥ÀÌÅ¸ ±×·ì Ä«¿îÅÍ

	DWORD			MonTranCount;			//¸ó½ºÅÍ/NPC Àü¼Û ÁØºñ
	DWORD			CharTransCount;			//Ä³¸®ÅÍ Àü¼Û ÁØºñ

	int				HitMonsterCount;		//°ø°ÝÇÑ ¸ó½ºÅÍ Ä«¿îÅÍ
	smCHAR			*lpChrTarget;			//À¯Àú°¡ ÃÖ±Ù °ø°ÝÇÑ ¸ó½ºÅÍ ±â·Ï(¿ì¼±¼øÀ§ Àü¼Û)
	DWORD			dwTarObjSerial;			//À¯Àú°¡ ÃÖ±Ù °ø°ÝÇÑ ¸ó½ºÅÍ °íÀ¯¹øÈ£

	int				UserPacketLoop;			//À¯Àú 1·çÇÎ½Ã Àü¼ÛµÈ ÆÐÅ¶ ¼öÄ¡ ( Àü¼ÛÅë°è ¼öÄ¡ )
	int				BefUserPacketLoop;		//ÀÌÀü 1·çÇÎ½Ã Àü¼ÛµÈ ÆÐÅ¶ ¼öÄ¡ ( Àü¼ÛÅë°è ¼öÄ¡ )

	int				AttackCount;			//°ø°Ý Ä«¿îÅÍ			
	DWORD			dwAttackTime;			//°ø°Ý ½Ã°£
	DWORD			dwLastAttackTime;		//ÃÖ±Ù °ø°Ý½Ã°£
	int				DamagePacketCount;		//°ø°Ý ÆÐÅ¶ Ä«¿îÅÍ
	DWORD			dwDamagePacketTime;		//µ¥¹ÌÁö ÆÐÅ¶À» º¸³½ Å¬¶óÀÌ¾ðÆ® Ãø ½Ã°£

	int				MonsterKillCounter;		//¸ó½ºÆ® Á×ÀÎ Ä«¿îÅÍ
	DWORD			dwLastTradeChksum;		//¸¶Áö¸· °Å·¡½Ã Ã½Å©¼¶
	DWORD			dwLastTradeTime;		//¸¶Áö¸· °Å·¡½Ã ½Ã°£

	//////////// Àú·¹º§ ¼±¹° ///////////////
	short			sLowLevel_PresentItem[2];	//Àú·¹º§ ¾ÆÀÌÅÛ ¼±¹°

	DWORD			dwStartUser_PresentItem_Time;	//½ÃÀÛÀ¯Àú ¾ÆÀÌÅÛ ¼±¹°
	DWORD			dwStartUser_PresentItem_Count;	//½ÃÀÛÀ¯Àú ¾ÆÀÌÅÛ ¼±¹°

	bool bMuted = FALSE;
	DWORD				dwChatTradeTime;

	////////////////////////////////////////////////
	// °ø°Ý ¼Û¼ö½Å µ¥ÀÌÅ¸

	int				Send_AttackCount;
	int				Send_AttackDamage;
	int				Recv_AttackCount;
	int				Recv_AttackDamage;

	/////////////// ¸ÅÅ©·Î °¨Áö ////////////////////

	DWORD			dwMacroTime;
	int				Macro_PosX;
	int				Macro_PosZ;
	int				Macro_AngY;
	DWORD			dwMacroAngTime;
	DWORD			dwMacroChkTime;


	/////////////// ±âÅ¸ ///////////////////////////
	DWORD			dwSendFrames[CONNECTMAX];	//´Ù¸¥ Ä³¸¯ÅÍµé µ¥ÀÌÅ¸ Àü¼Û ÇÁ·¹ÀÓ

	int				CrackWarning;				//Å©·¢ »ç¿ë À¯Àú
	DWORD			dwTime_Disconnect;			//¿¬°á²ö±â ¿¹¾à
	int				BadPlayer;					//³ª»Û ÇÃ·¹ÀÌ¾î ( ¸ðµç °ø°Ý ¹«È¿ )
	DWORD			dwPacketAttackCount;		//°úµµÇÑ ÆÐÅ¶À¸·Î ÀÎÇÑ °ø°Ý °¨Áö 
	DWORD			dwPacketAttackTime;			//°úµµÇÑ ÆÐÅ°°ø°Ý °¨Áö½ÃÀÛ ½Ã°£
	int				WarningCount;				//°æ°í Ä«¿îÆ®

	char			*lpRecordDataBuff;			//·¹ÄÚµåµ¥ÀÌÅ¸ ¹öÆÛ ( °ÔÀÓÀúÀå ¸Þ¸ð¸®¹öÆÛ )
	DWORD			dwRecordDataTime;			//ÃÖ±Ù ±â·Ï½Ã°£

	int				EventMode;					//ÀÌº¥Æ® ÁøÇà
	int				EventScore;					//ÀÌº¥Æ® Á¡¼ö
	int				EventCount;					//µæÁ¡ Ä«¿îÆ®
	DWORD			dwEventTime;				//ÀÌº¥Æ® °ü·Ã ½Ã°£
	short			sEventParam[2];				//ÀÌº¥Æ® ÁøÇà ÆÄ¶ó¸ÞÅÍ

	int				AreaOutCount;				//¿µ¿ªÄ§¹ü Ä«¿îÅÍ

	int				BlockTime;					//°èÁ¤ ºí·°½ÃÅ³ ½Ã°£

	int				WhisperMode;				//±Ó¸» ¸ðµå

	int				Event_StarTicketLevel;		//º° »óÇ°±Ç ¹ß»ý ·¹º§


	////////////// º¸¿Ï °ü·Ã ¼öÄ¡ Åë°è //////////////////////

	INT64			spExp_Start;				//½ÃÀÛ½Ã °æÇèÄ¡
	int				spMoney_Start;				//½ÃÀÛ½Ã µ·
	int				spLevel_Start;				//½ÃÀÛ½Ã ·¹º§

	int				spExp_InOut_Total;			//°æÇèÄ¡ Ãâ·Â ÅäÅ»
	int				spMoney_In_Total;			//µ· ÀÔ±Ý ÅäÅ»
	int				spMoney_InOut_Total;		//µ· Ãâ±Ý ÅäÅ»

	int				spEXP_Out;					//°æÇèÄ¡ Ãâ·Â
	int				spMoney_In;					//µ· ÀÔ±Ý
	int				spMoney_Out;				//µ· Ãâ±Ý

	DWORD			dwLastExpMoneyCheckTime;	//¸¶Áö¸·À¸·Î °æÇèÄ¡ ±Ý¾× ºñ±³ÇÑ ½Ã°£
	DWORD			dwLastExpMoneyTransServer;	//±º¼­¹ö·Î °æÇèÄ¡ ±Ý¾× ¼öÄ¡ Àü¼ÛÇÑ ½Ã°£

	DWORD			dwHopeDisconnectTime;		//°­Á¦ÀûÀ¸·Î ¼³Á¤µÈ ½Ã°£ÀÌ µÇ¸é ¿¬°á Á¾·á

	int				Client_Version;				//»ç¿ëÁßÀÎ Å¬¶óÀÌ¾ðÆ®ÀÇ ¹öÀü Á¤º¸

	DWORD			dwLastSaveTime;				//¸¶Áö¸·À¸·Î ÀúÀåÇÑ ½Ã°£

	DWORD			dwGameServerExp[8];			//°ÔÀÓ¼­¹ö °æÇèÄ¡ ÀúÀå

	DWORD			dwNSpriteTime;				//nSprite °Ë»çÇÑ ½Ã°£

	//Å¬¶óÀÌ¾ðÆ® Æã¼Ç Ã½Å©
	DWORD			dwClientFuncChk_1;			//Ã½Å©ÇÑ Æã¼Ç1
	DWORD			dwClientFuncChk_2;			//ÄÊÅ©ÇÑ Æã¼Ç2
	int				FuncChkFailCount1;			//½ÇÆÐ
	int				FuncChkFailCount2;			//½ÇÆÐ
	int				FuncChkSucessCount1;		//¼º°ø
	int				FuncChkSucessCount2;		//¼º°ø
	DWORD			dwFuncChkErrorFlag;			//¿¡·¯
	int				ClientFunChk1_Count;
	int				ClientFunChk2_Count;

	DWORD			dwClientFunChk_NextTime;	//´ÙÀ½ Ã½Å©ÇÒ ½Ã°£

	int				RecordWarningCount;			//°ÔÀÓ ÀúÀå±â·Ï °æ°í Ä«¿îÅÍ
	int				RecordWarningExp;			//°æ°í ¹ÞÀº °æÇèÄ¡

	DWORD			dwClentOS;					//Å¬¶óÀÌ¾ðÆ® OS
	WORD			wClentOSVersion[2];			//Å¬¶óÀÌ¾ðÆ® ¹öÀü [0]-¹öÀü [1]-ºôµå¹øÈ£
	DWORD			dwClentModulChkSum;			//Å¬¶óÀÌ¾ðÆ® ¸ðµâ Ã½Å©¼¶
	int				ClentModuleCount;			//Å¬¶óÀÌ¾ðÆ® ¸ðÆ« °¹¼ö

	DWORD			dwDamagePacketKey[2];		//µ¥¹ÌÁö ÆÐÅ¶ ¾ÏÈ£¿ë Å° ( ¼ö½Å¿ë )
	DWORD			dwDamagePacketKey2[2];		//µ¥¹ÌÁö ÆÐÅ¶ ¾ÏÈ£¿ë Å° ( ¼Û½Å¿ë )

	BOOL									bParty;
	PartyInfo*								psParty;
	BOOL PartyBlocked;

	///////////// Å¬·£ °ü·Ã Á¤º¸ ////////////////////
	DWORD			dwClanCode;							//°¡ÀÔÇÑ Å¬·£ ÄÚµå
	int				ClanTicketCode;						//Å¬·£ Æ¼ÄÏ ÄÚµå ( Clan.DLL ¿¡¼­ ¹ß±Þ )
	int				SucessCheck_ClanCode;				//Å¬·£ ÄÚµå È®ÀÎ ¼º°ø

	char	szGuid[128];
	char	szPwd[32];
	char	szGPCode[16];
	time_t	DisuseDay;
	int		UsePeriod;
	int		GpCode;
	int		GpType;
	DWORD	dwPeriodTime;
	int		Bl_RNo;
	int		Bl_Meter;
	DWORD	dwPeriodTime2;



	//Kill streak
	int iKillStreak = 0;
	//Numero da equipe na arena
	int nEquipeArena = 0;
	
	//Nível do vip
	int vipLevel = 0;

	// Modo PK
	DWORD	PkMode;
	
	updateQuest updatequest[100];
	QUEST_INFOPCKG questInfoPckg;

	int		SndItemSecCodeCount;			//¾ÆÀÌÅÛ º¸¾È »ý¼º ÄÚµå Ä«¿îÅÍ
	int		RcvItemSecCodeBuff[8][16];		//¹ÞÀº ¾ÆÀÌÅÛ º¸¾È Ä«¿îÅÍ ÀÓÁö ÀúÀå ¹öÆÛ

	/////////////////////////////////////////////////////////////////

	short	sLimitDamage[2];				//°ø°Ý·Â Á¦ÇÑ
	short	sLimitCritical[2];				//Å©¸®Æ¼ÄÃ Á¦ÇÑ

	/////////////////////////////////////////////////////////////////

	int		ServerPotion[3][4];				//¹°¾à°¹¼ö È®ÀÎ¹öÆÛ

	//////////////////////////// ½ºÅ³ Ã³¸® //////////////////////////////

	//¿ÀÅä¸ÞÀÌ¼Ç
	DWORD	dwSkill_AutoMation_Time;
	int		dwSkill_AutoMation_Param;
	//¸Æ½Ã¸¶ÀÌÁî
	DWORD	dwSkill_Maximize_Time;
	int		dwSkill_Maximize_Param;
	//ÇÇÁöÄÃ
	DWORD	dwSkill_PhysicalAbsorb_Time;
	int		dwSkill_PhysicalAbsorb_Param;
	//ÀÍ½ºÆ®¸² ½¯µå
	DWORD	dwSkill_ExtreamShield_Time;
	int		dwSkill_ExtreamShield_Param;

	//È¦¸® ¹Ùµð
	DWORD	dwSkill_HolyBody_Time;
	int		dwSkill_HolyBody_Param;
	//È¦¸® ¹ë·¯
	DWORD	dwSkill_HolyValor_Time;
	int		dwSkill_HolyValor_Param;
	//È¦¸® ¸®ÇÃ·º¼Ç
	DWORD	dwSkill_HolyReflection_Time;
	int		dwSkill_HolyReflection_Param;
	//ÀÎÃ¾Æ® ¿öÆù
	DWORD	dwSkill_EnchantWeapon_Time;
	int		dwSkill_EnchantWeapon_Param;


	//¸ÞÅ»¾Æ¸Ó
	DWORD	dwSkill_MetalArmor_Time;
	int		dwSkill_MetalArmor_Param;

	//½ºÆÄÅ© ½¯µå
	DWORD	dwSkill_SparkShield_Time;
	int		dwSkill_SparkShield_Param;

	//ÆÓÄÁ ½Ã¸®Áî
	DWORD	dwSKill_Falcon_Time;
	short	dwSKill_Falcon_Damage[2];

	//µð¹ÙÀÎ ÀÎÈ¦·¹ÀÌ¼Ç
	DWORD	dwSkill_DivineInhalation_Time;
	DWORD	dwSkill_DivineInhalation_Param;

	//Æ®¶óÀÌ¾öÇÁ ¹ßÇÒ¶ó
	DWORD	dwSkill_TriumphOfValhalla_Time;
	DWORD	dwSkill_TriumphOfValhalla_Param;

	//¹öÃò¾ó ¶óÀÌÇÁ
	DWORD	dwSkill_VirtualLife_Time;
	DWORD	dwSkill_VirtualLife_Param;

	//¿¡³ÊÁö½¯µå
	DWORD	dwSkill_EnergyShield_Time;
	DWORD	dwSkill_EnergyShield_Param;

	//½ºÇÇ¸´¿¤¸®¸àÅ»
	DWORD	dwSkill_SpiritElemental_Time;
	DWORD	dwSkill_SpiritElemental_Param;

	//´í½Ì¼Òµå
	DWORD	dwSkill_DancingSword_Time;
	DWORD	dwSkill_DancingSword_Param;
	DWORD	dwSkill_DancingSword_DelayTime;

	//¹è´Ï½¬
	DWORD	dwSkill_Vanish_Time;
	DWORD	dwSkill_Vanish_Param;

	//¹è´Ï½¬
	DWORD	dwSkill_Compulsion_Time;
	DWORD	dwSkill_Compulsion_Param;

	//¸¶±×³×Æ½½ºÇÇ¾î
	DWORD	dwSkill_MagneticSphere_Time;
	DWORD	dwSkill_MagneticSphere_Param;

	//¹ö¼­Ä¿
	DWORD	dwSkill_Berserker_Time;
	DWORD	dwSkill_Berserker_Param;

	//¾î¼¼½Å¾ÆÀÌ
	DWORD	dwSkill_AssassinEye_Time;
	DWORD	dwSkill_AssassinEye_Param;

	//º¸±×
	DWORD	dwSkill_Vague_Time;
	DWORD	dwSkill_Vague_Param;

	DWORD	dwSkill_EvasionMastery_Time;
	DWORD	dwSkill_EvasionMastery_Param;

	//Æ÷½º¿Àºê³×ÀÌÃÄ
	DWORD	dwSkill_ForceOfNature_Time;
	WORD	wSkill_ForceOfNature_Param[2];

	//°¡µé¸® ½¯µå
	DWORD	dwSkill_GodlyShied_Time;
	DWORD	dwSkill_GodlyShied_Param;

	//°«½ººí·¹½º
	DWORD	dwSkill_GodsBless_Time;
	DWORD	dwSkill_GodsBless_Param;

	//È¦¿Àºê¹ßÇÒ¶ó
	DWORD	dwSkill_HallOfValhalla_Time;
	WORD	wSkill_HallOfValhalla_Param[2];

	//ÇÁ·Î½ºÆ®Àçºí¸°
	DWORD	dwSkill_FrostJavelin_Time;
	DWORD	dwSkill_FrostJavelin_Param;

	//¸®Á¦³Ê·²ÇÊµå
	DWORD	dwSkill_RegenerationField_Time;

	//¼­¸ó ¹«½ºÆç
	DWORD	dwSkill_SummonMuspell_Time;
	DWORD	dwSkill_SummonMuspell_Param;


	//////////////////////////// ÀúÁÖ Ã³¸® //////////////////////////////
	//¼­Å¥¹ö½º ½Ã¸®Áî ÀúÁÖ
	DWORD	dwCurse_Attack_Time;		//°ø°Ý ÀúÁÖ
	DWORD	dwCurse_Attack_Param;		//°ø°Ý ÀúÁÖ

	DWORD	dwCurse_Defence_Time;		//¹æ¾î ÀúÁÖ
	DWORD	dwCurse_Defence_Param;		//¹æ¾î ÀúÁÖ

	////////////////////////// ½ºÅ³ Æ÷ÀÎÆ® °¨½Ã //////////////////////

	BYTE	bSkillPoint[SKILL_POINT_COLUM_MAX];

	////////////////////////////////////////////////////////////////


	//////////////////////////// Æ÷½º¿Àºê ////////////////////////////

	DWORD	dwForceOrb_Code;			//Æ÷½º¿Àºê ¾ÆÀÌÅÛ ½ÇÇàÄÚµå
	DWORD	dwForceOrb_Time;			//Æ÷½º¿Àºê À¯Áö½Ã°£
	DWORD	dwForceOrb_Damage;			//Æ÷½º¿Àºê µ¥¹ÌÁö

	DWORD	dwForceOrb_SaveCode;		//Æ÷½º¿Àºê ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwForceOrb_SaveTime;		//Æ÷½º¿Àºê ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwForceOrb_SaveDamage;		//Æ÷½º¿Àºê ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwForceOrb_NextSetTime;		//Æ÷½º¿Àºê »ç¿ëÁ¤º¸ º¸³»±â ¿¹¾à½Ã°£

	// ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â)
	DWORD	dwLifeBooster_Code;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) ½ÇÇàÄÚµå
	DWORD	dwLifeBooster_Time;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) À¯Áö½Ã°£
	DWORD	dwLifeBooster_Data;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) °¡Áß ÆÛ¼¾Æ®

	DWORD	dwLifeBooster_SaveCode;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwLifeBooster_SaveTime;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwLifeBooster_SaveData;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) ±º¼­¹ö ÀúÀå¿ë °¡Áß ÆÛ¼¾Æ®
	DWORD	dwLifeBooster_NextSetTime;	//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) »ç¿ëÁ¤º¸ º¸³»±â ¿¹¾à½Ã°£

	// ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â)
	DWORD	dwManaBooster_Code;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) ½ÇÇàÄÚµå
	DWORD	dwManaBooster_Time;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) À¯Áö½Ã°£
	DWORD	dwManaBooster_Data;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) °¡Áß ÆÛ¼¾Æ®

	DWORD	dwManaBooster_SaveCode;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwManaBooster_SaveTime;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwManaBooster_SaveData;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) ±º¼­¹ö ÀúÀå¿ë °¡Áß ÆÛ¼¾Æ®
	DWORD	dwManaBooster_NextSetTime;	//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) »ç¿ëÁ¤º¸ º¸³»±â ¿¹¾à½Ã°£

	// ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(Ã¼·Â)
	DWORD	dwStaminaBooster_Code;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) ½ÇÇàÄÚµå
	DWORD	dwStaminaBooster_Time;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) À¯Áö½Ã°£
	DWORD	dwStaminaBooster_Data;			//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) °¡Áß ÆÛ¼¾Æ®

	DWORD	dwStaminaBooster_SaveCode;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwStaminaBooster_SaveTime;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) ±º¼­¹ö ÀúÀå¿ë
	DWORD	dwStaminaBooster_SaveData;		//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) ±º¼­¹ö ÀúÀå¿ë °¡Áß ÆÛ¼¾Æ®
	DWORD	dwStaminaBooster_NextSetTime;	//ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) »ç¿ëÁ¤º¸ º¸³»±â ¿¹¾à½Ã°£


	//½Ã°£Á¦ ºô¸µ ¾ÆÀÌÅÛ ¼³Á¤
	DWORD	dwPrimeItem_NextSetTime;	//½Ã°£Á¦ ºô¸µ¾ÆÀÌÅÛ ¸ÞÀÎ¼­¹öÅëº¸ ¿¹¾à½Ã°£
	DWORD	dwPrimeItem_NextSetTime_T;
	DWORD	dwTime_PrimeItem_X2;		//¾ÆÀÌÅÛ 2¹è µå·Ó
	DWORD	dwTime_PrimeItem_ExpUp;		//°æÇèÄ¡¾÷ Æ÷¼Ç
	DWORD	dwPrimeItem_PackageCode;	//ÆÐÅ°Áö¾ÆÀÌÅÛ ÄÚµå

	DWORD	dwTime_PrimeItem_VampCuspid;	//¹ìÇÇ¸¯Ä¿½ºÇÍ
	DWORD	dwTime_PrimeItem_ManaRecharg;	//¸¶³ª¸®Â÷Â¡
	DWORD	dwTime_PrimeItem_MightofAwell;	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
	DWORD	dwTime_PrimeItem_MightofAwell2;	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
	DWORD	dwTime_PrimeItem_ManaReduce;	// pluto ¸¶³ª ¸®µà½º
	DWORD	dwTime_PrimeItem_PhenixPet;		// pluto Æê(ÇØ¿Ü)
	DWORD	dwTime_PrimeItem_HelpPet;	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	DWORD	dwTime_PrimeItem_VampCuspid_EX;	// Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	DWORD	dwTime_PrimeItem_StaminaReduce;	// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç

	DWORD	dwTime_Hat;
	DWORD	HatKind;
	DWORD	dwTime_PrimeItem_Mount;
	DWORD	dwPrimeItem_MountParam;

	DWORD				  dwTime_CaravanArma;
	DWORD				  dwTime_CaravanHopy;
	DWORD				  dwTime_CaravanBuma;

	Caravan sCaravan;

	//pC¹æ Æê
	DWORD	dwPCBang_Pet_AttackTime;	//PC¹æ Æê °ø°ÝÅ¸ÀÓ
	DWORD	dwPCBang_Pet_NextSetTime;	//PC¹æ Æê Áö¿ª ¼­¹öÅëº¸ ¿¹¾à½Ã°£

	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	DWORD	dwHelp_Pet_AttackTime;	//ºô¸µ µµ¿ì¹Ì Æê °ø°ÝÅ¸ÀÓ
	DWORD	dwHelp_Pet_NextSetTime;	//ºô¸µ µµ¿ì¹Ì Æê Áö¿ª ¼­¹öÅëº¸ ¿¹¾à½Ã°£

	// Àåº° - ½ºÅ³ µô·¹ÀÌ
	DWORD	dwSkillDelay_Code;			
	DWORD	dwSkillDelay_Time;			
	DWORD	dwSkillDelay_Data;			

	DWORD	dwSkillDelay_SaveCode;		
	DWORD	dwSkillDelay_SaveTime;		
	DWORD	dwSkillDelay_SaveData;		
	DWORD	dwSkillDelay_NextSetTime;


	smCHAR	*lpLinkChar;
	DWORD	dwLinkCharCode;

	/////////////////////////// °ø¼º °ü·Ã //////////////////////////
	short	sBlessCastle_Damage[2];		//0-Damage 1-Killing Count
	DWORD	dwCheck_MasterClanTime;		//¼ºÁÖ Å¬·£ È®ÀÎ ½Ã°£


	sCASTLE_MASTER	CastleMaster;		//¼ºÁÖ ¼³Á¤ Á¤º¸
	DWORD	dwBlessSkill_Code;			//½ºÅ³ ¼³Á¤ ÄÚµå
	DWORD	dwBlessSkill_Time;			//½ºÅ³ À¯Áö ½Ã°£
	DWORD	dwBlessSkill_Param;			//½ºÅ³ ¼³Á¤ ÆÄ¶ó¸ÞÅÍ

	DWORD	dwSiegeItem_Scroll_Time;	//°ø¼º ¾ÆÀÌÅÛ ½ºÅ©·Ñ
	DWORD	dwSiegeItem_Scroll_Code;
	DWORD	dwSiegeItem_Scroll_Param;

	DWORD	dwSiegeItem_Stone1_Time;		//°ø¼º ¾ÆÀÌÅÛ ÀúÇ×¼®
	DWORD	dwSiegeItem_Stone1_Code;
	DWORD	dwSiegeItem_Stone1_Param;

	DWORD	dwSiegeItem_Stone2_Time;		//°ø¼º ¾ÆÀÌÅÛ °­È­¼®
	DWORD	dwSiegeItem_Stone2_Code;
	DWORD	dwSiegeItem_Stone2_Param;

	short	sCheckPlayField_Count[2];		//À¯Àú ÇÊµå À§Ä¡ È®ÀÎ Ä«¿îÅÍ


	//////////// Quest ÁøÇà ÄÚµå º¸¾È¿ë //////////////

	DWORD	dwHaQuestItemCount;					//ÇÏÄù½ºÆ® ¾ÆÀÌÅÛ Áö±Þ Ä«¿îÆ®


	///////////// º£Æ®³² °æÇèÄ¡ ºÐ¹è °ü·Ã /////////////////////
	char	szUserExpTimeID[32];		//À¯Àú ID(º£Æ®³² ¸ÞÀÎ¼­¹ö)
	char	szUserExpTimeName[32];		//À¯Àú Name(º£Æ®³² ¸ÞÀÎ¼­¹ö)
	DWORD	dwExpGameTime;				//ÇÃ·¹ÀÌ½Ã°£
	DWORD	dwExpGameTimeCheck;			//ÇÃ·¹ÀÌ½Ã°£Ã½Å©¿ë

	INT64  LastExp;

	int	CreateParty( rsPLAYINFO *lpPlayInfo );			//ÆÄÆ¼»ý¼º - ( ÃÊ±â ÆÄÆ¼¿ø Á¤º¸ )
	int	JoinPartyPlayer( rsPLAYINFO *lpPlayInfo );		//ÆÄÆ¼¿¡ °¡ÀÔ
	int	DeletePartyPlayer( rsPLAYINFO *lpPlayInfo );	//ÆÄÆ¼¿¡¼­ Å»Åð
	int ChangePartyPlayer( rsPLAYINFO *lpOldPlayInfo , rsPLAYINFO *lpNewPlayInfo );	//ÆÄÆ¼¿ø º¯°æ
	int	UpdatePartyPlayer();							//ÆÄÆ¼¿ø ¾÷µ¥ÀÌÆ®
	int	UpdatePartyPlayInfo();							//ÆÄÆ¼ÇÃ·¹ÀÌ ¾÷µ¥ÀÌÆ®
	int GetPartyMoney( sITEMINFO *lpDefItemInfo , rsPLAYINFO *lpPlayInfo );	//ÆÄÆ¼¿ø µ· È¹µæ
	int GetPartyExp( smCHAR *lpChar , rsPLAYINFO *lpPlayInfo );				//ÆÄÆ¼¿ø °æÇèÄ¡ È¹µæ
	int PartyChatting( rsPLAYINFO *lpPlayInfo );			//ÆÄÆ¼¿ø Ã¤ÆÃ¸Þ¼¼Áö º¸³¿

	int	UpdateServerPartyPlayer( TRANS_PARTY_PLAYINFO *lpTransPartyPlayInfo );
	int AddServerMoney(int Money, int WhereParam);
	int	SubServerMoney(int Money, int WhereParam);
	int	SetServerMoney(int Money, int WhereParam);
	int	LeavePartyMaster( rsPLAYINFO *lpPlayInfo );
	int	ReleaseParty();

	int SendPartyMessage(std::string message, DWORD dwCode);
	int SendPartyMessage_Player(std::string message);
	int SendPartyData( rsPLAYINFO *lpPlayInfo_From , char *szData  );
	int	JoinParty(rsPLAYINFO *lpPlayMaster);
	int	SecedeParty();
	int	DisconnectPartyUser();
	int	CallBack_ClanMarkNum(int ClanMarkNum);

};

#define BILLING_GPTYPE_DAY		0
#define BILLING_GPTYPE_TIME		1
#define BILLING_GPTYPE_FREE		2
#define BILLING_GPTYPE_POTAL	3
#define BILLING_PGTYPE_PERSONAL	0
#define BILLING_PGTYPE_PCROOM	0

struct TRANS_CHATBUFF 
{
	int		size;
	int		code;
	DWORD	dwIP;
};

struct CHATBUFF 
{
	DWORD	dwIP;
	char szMessage[256];
};

struct TRANS_CHATMESSAGE {
	int		size;
	int		code;
	DWORD	dwIP;
	DWORD	dwObjectSerial;

	char szMessage[256];
};


struct smPLAYDATA_GROUP {
	int		size,code;
	int		PlayDataCnt;
	DWORD	dwSendCode;
};


#define smPLAYBUFF_FRAME_NONE			0xFFFF0000
#define	smPLAYBUFF_ITEM_BODY			0
#define	smPLAYBUFF_ITEM_LEFT			1
#define	smPLAYBUFF_ITEM_RIGHT			2
#define	smPLAYBUFF_ITEM_SKIL			3

struct smPLAYBUFF {
	int		x,y,z;
	DWORD	frame;
	short	angle[4];

	short	wStickItems[4];
	DWORD	dwTargetSerial;
};

struct smPLAYDATA {
	int	size;
	int code;

	DWORD	dwObjectSerial;

	int x,y,z;
	short angle[4];		//012->xyz 3->action code

	DWORD	frame;
	DWORD	dwAutoCharCode;

};

struct smPLAYDATA2 {
	int	size;
	int code;

	DWORD	dwObjectSerial;

	short	BuffCount[2];			//PlayBuff Time Posi
	smPLAYBUFF	PlayBuff[2];		//0-Begin 1-last
	DWORD	dwTarget;
};

struct smPLAYDATA3 {
	int	size;
	int code;

	DWORD	dwObjectSerial;

	short	BuffCount[3];			//PlayBuff Time Posi
	smPLAYBUFF	PlayBuff[3];		//0-Begin 1-last 2-change
	DWORD	dwTarget;
};

struct smTRANS_PLAYBUFF {
	int		BuffCount;
	smPLAYBUFF	PlayBuff;
};

#define TRANS_PLAYDATA_EVENT_LEVEL		0
#define TRANS_PLAYDATA_EVENT_POTION		1
#define TRANS_PLAYDATA_EVENT_SKIL		2
#define TRANS_PLAYDATA_EVENT_EXT		3

struct smTRANS_PLAYDATA
{
	int	size;
	int code;

	DWORD	dwObjectSerial;
	DWORD	dwTarget;

	int		PlayBuffCnt;
	int		StartPosi;

	short	Hp[2];
	short	Mp[2];
	BOOL	bPartyLeader;
	DWORD	dwAutoCharCode;
	short	wArea[2];
	BYTE	bUpdateInfo[4];
	short	iMountID;
	BYTE	bEventInfo[4];
};


struct smEFFECT_ITEM {
	int size, code;

	DWORD	dwItemCode;

	int		ColorBlink;
	union
	{
		short	sColors[4];

		struct
		{
			int iAgeLevel;
			int iMixID;
		};
	};
	DWORD	DispEffect;
	DWORD	BlinkScale;
};



struct smTRANS_CHAR_COMMAND
{
	int	size;
	int code;

	int LParam;
	int WParam;
	int SParam;

	char	szName[32];
};

struct smTRANS_CHAR_COMMAND2
{
	int	size;
	int code;

	int LParam;
	int WParam;
	int SParam;

	char	szId[32];
	char	szName[32];
	char	szMac[24];
	char	szHDMac[32];
	char	szPath[MAX_PATH];
	char	szPCName[MAX_COMPUTERNAME_LENGTH + 1];
};

struct smTRNAS_PLAYERINFO
{
	int	size;
	int code;

	smCHAR_INFO	smCharInfo;

	DWORD	dwObjectSerial;

	int	x,y,z;
	int ax,ay,az;
	int state;
};

struct smTRNAS_PLAYERINFO_QUICK
{
	int	size;
	int code;

	DWORD	dwObjectSerial;

	int	x,y,z;
	int ax,ay,az;
	int state;

	smCHAR_INFO	smCharInfo;
};

struct smTRANS_COMMAND_DWORD
{
	int	size;
	int code;

	DWORD LParam;
	DWORD WParam;
	DWORD SParam;
	DWORD EParam;
};

struct smTRANS_COMMAND_EX
{
	int	size;
	int code;

	int LParam;
	int WParam;
	int SParam;
	int EParam;

	int LxParam;
	int WxParam;
	int SxParam;
	int ExParam;

};


struct smTRANS_COMMAND_BUFF {
	smTRANS_COMMAND		smTransCommand;
	char	*Buff[256];
};

struct smTRANS_COMMAND_SOD {
	smTRANS_COMMAND		smTransCommand;
	int		MyScore;
	int		MyTeam;
	int		TeamScore[4];
	int		Temp[4];
};

struct smTRANS_COMMAND_POLLING {
	smTRANS_COMMAND		smTransCommand;
	BYTE	bPolling[10];
};



//¼Ó¼º È£Ãâ
struct smTRANS_RESISTANCE {
	int	size;
	int code;
	int	Param;
	short Resistance[8];
	int	Absorb;
};


//°ø¼º
struct smTRANS_BLESSCASTLE {
	smTRANS_COMMAND		smTransCommand;
	int				TaxRate;
	short			Tower[12];		//Å¸¿ö¼³Á¤	
	unsigned char	MercenaryNum[4];	//int		MercenaryType;	//¿ëº´¼³Á¤
	int				ClanSkill;		//Å¬·£½ºÅ³
	int				Price;
	DWORD			dwMasterClan;	//¼ºÁÖÅ¬·£
	int				Temp[4];
};

//°ø°Ý¼øÀ§ Åé10
struct	rsUSER_LIST_TOP10 {
	//SYSTEM HEADER
	int		size,code;

	int		tCode;
	DWORD	dwObjectSerial;
	DWORD	dwNameCode;
	int		x,y,z;

	//BODY

	DWORD	dwCharCode;
	DWORD	dwUpdateTime;
	DWORD	dwTotalDamage;
	char	bCrystalTowerCount[4];
	DWORD	dwTemp[2];

	DWORD	dwUserCode[10];
	int		Damage[10];
	int		Counter[10];
};

/////////////// ¾ÆÀÌÅÛÀ» ±×·ìÀ¸·Î ¹­¾î¼­ Àü¼Û /////////////////////

/*
/////¼­¹ö·Î º¸³»Áú ¾ÆÀÌÅÛ ±¸Á¶Ã¼ 
struct sCRAFTITEM_SERVER{
	int size, code;
	int DocIndex;
	int Result;
	sITEM DesCraftItem;
	DWORD SheltomCode[12];
	DWORD Head[12];
	DWORD CheckSum[12];
	int Index;
	int Money;
};
*/


//¾ÆÀÌÅÛ ±×·ìÀ¸·Î Àü¼Û
#define	TRANS_GROUPITEM_MAX		16
#define	TRANS_GROUPITEM_HEADER_SIZE		(14*4)

//¾ÐÃàÀ» Ç¬°æ¿ì
struct	TRANS_ITEMINFO_GROUP {
	int	size,code;

	int Index;
	int Result;
	int Money;
	int	ItemCount;

	DWORD	dwTemp[8];

	sITEMINFO	sItemInfo[TRANS_GROUPITEM_MAX];
};

//¾ÐÃàÆ÷¸Ë
struct	TRANS_ITEMINFO_GROUP2 {
	int	size,code;

	int Index;
	int Result;
	int Money;
	int	ItemCount;

	DWORD	dwTemp[8];

	char	szBuff[ sizeof(sITEMINFO) * TRANS_GROUPITEM_MAX ];
};

struct TRANS_ITEMINFO {
	int	size,code;
	sITEMINFO	Item;
	int x,y,z;
	DWORD	dwSeCode[4];
};

struct	TRANS_ITEM_CODE {
	int size,code;

	DWORD	dwFlag;

	DWORD	dwItemCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
	DWORD	dwTime;
};


struct TRANS_ITEM {
	int			state;

	int			type;
	int			ItemCode;

	int			x,y,z;
	char		szName[64];
	void		*lpStgArea;
	DWORD       iClass;
	//char szModelName[64];
};


struct _TRANS_THROW_ITEM {
	DWORD	dwCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
};

#define	THROW_ITEMS_MAX				32
//±¸Á¶Ã¼ Çì´õ »çÀÌÁî
#define	TRANS_THROW_ITEMS_DEFSIZE	(sizeof(int)*4)

struct TRANS_THROW_ITEMS {
	int		size,code;
	int		ThrowItemCount;
	int		Temp;

	_TRANS_THROW_ITEM	ThrowItems[THROW_ITEMS_MAX];
};



//°Å·¡ÈÄ ¾ÆÀÌÅÛ º¸°í
struct _TRANS_RECORD_TRADE_ITEM {
	DWORD	dwCode;
	DWORD	dwHead;
	DWORD	dwChkSum;
	int		State;
};

//sinTrade.h ¿¡¼­ ÀçÁ¤ÀÇ
#define _MAX_TRADE_ITEM			20

struct TRANS_TRADE_ITEMKEY	{
	int size,code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;

	DWORD	dwItemCode[_MAX_TRADE_ITEM];
	DWORD	dwItemKey[_MAX_TRADE_ITEM];
	DWORD	dwItemSum[_MAX_TRADE_ITEM];

	int		Money;
	int Coin;
};


#define	TRADE_RECV_ITEMS_MAX				32

//±¸Á¶Ã¼ Çì´õ »çÀÌÁî
#define	TRANS_RECORD_TRADE_DEFSIZE		(sizeof(int)*16)

struct TRANS_RECORD_TRADE_ITEMS {
	int		size,code;
	int		Money;
	int		ThrowItemCount;
	char	szTradeUserName[32];			//»ó´ë¹æ Ä³¸¯¸í
	int		Temp[4];

	_TRANS_RECORD_TRADE_ITEM	Items[TRADE_RECV_ITEMS_MAX];
};


#define TRANS_ITEM_MAX			1024

//¾ÆÀÌÅÛ Á¤º¸
struct smTRANS_ITEMS
{
	int size,code;

	int state;
	int Count;

	TRANS_ITEM	Items[ TRANS_ITEM_MAX ];
};

//¾ÆÀÌÅÛ Á¤º¸
struct smTRANS_ACITON_ITEM
{
	int size,code;

	int state;

	int			x,y,z;
	void		*lpStgArea;
};
//»óÁ¡ ¾ÆÀÌÅÛ Á¤º¸
struct smTRANS_SELL_ITEM {
	int size,code;
	int	ItemKind;					//°ø°Ý / ¹æ¾î /±âÅ¸ µîµîÀÇ ±¸ºÐ
	int	ItemCounter;				//ÆÇ¸Å ¾ÆÀÌÅÛÀÇ ¼ö
	int	Ext[4];						//¿¹ºñ
};

//¾ÆÀÌÅÛ ±³È¯ Ã¢
#define TRANS_TRADE_BUFF_SIZE		4000
struct TRANS_TRADEITEMS {
	int size,code;
	DWORD	dwSender;
	DWORD	dwRecver;
	DWORD	dwSum;
	DWORD	Temp[4];

	BYTE	TradeBuff[TRANS_TRADE_BUFF_SIZE];
};

#define	SOD_RESULT_MAX	32

struct	_SOD_RESULT {
	char		szName[32];
	rsPLAYINFO	*lpPlayInfo;
	int			JobCode;
	int			Class;
	int			Score;
	int			Monster;
	DWORD		dwTemp[4];
};

//SoD ¼øÀ§ Á¤º¸
struct	TRANS_SOD_RESULT {
	int	size,code;

	DWORD	dwGameCode;
	DWORD	dwGameTime;
	DWORD	dwDisplayMode;
	short	TeamRound[2];

	DWORD	dwTemp[15];

	int		ResultCount;

	_SOD_RESULT	Result[SOD_RESULT_MAX];
};



//////// ¼­¹ö¿Í Å¬¶óÀÌ¾ðÆ®ÀÇ µ·,°æÇèÄ¡ ¼öÄ¡ Á¶È¸ //////////
struct	sSERVER_MONEY {
	int	InputMoney;
	int	Counter;
	int	TotalX;
	int	TotalY;
	int	TotalZ;
};

struct sSERVER_EXP {
	int	Counter;
	int	InputExp;
	int	Total1;
	int	Total2;
};

struct TRANS_TOTAL_EXPMONEY {
	int	size,code;

	sSERVER_MONEY	sServerMoney;
	sSERVER_EXP		sServerExp;
};

//µ¥ÀÌÅ¸ ¼­¹ö·Î Å¬¶óÀÌ¾ðÆ® °æÀ¯ Àü¼Û
struct	TRANS_SEND_DATASERVER {
	int size,code;
	char	szBuff[1024];
};


//¾ÆÀÌÅÛ ±¸ÀÔ¿ë ±¸Á¶Ã¼
struct	TRANS_BUY_SHOPITEM {
	int	size,code;

	sITEM	sItem;			//±¸ÀÔÇÏ´Â ¾ÆÀÌÅÛ
	int		ItemCount;		//±¸ÀÔÇÏ´Â ¾ÆÀÌÅÛ °¹¼ö
};



//°Å·¡½Ã ¾ÆÀÌÅÛ º¸³½ÈÄ º¸³½ ¾ÆÀÌÅÛÀÌ ÀÎº¥¿¡ Á¸Àç ÇÏ´ÂÁö È®ÀÎ
struct	_TRANS_USINGITEM {
	DWORD	dwCode;
	short	Performance[8];
};

struct	TRANS_USINGITEM {
	int	size,code;
	int	ItemListCount;
	_TRANS_USINGITEM	ItemList[16];
};


struct TRANS_PREMIUM_TIME
{
	int	size;
	int code;

	DWORD	ThirdEye;
	DWORD	ExpUp;
	DWORD	VampCuspid;
	DWORD	ManaRecharg;
	DWORD	MightofAwell;
	DWORD	MightofAwell2;
	DWORD	ManaReduce;
	DWORD	PhenixPet;
	DWORD	HelpPet;
	DWORD	VampCuspidEx;
	DWORD	StaminaReduce;

	DWORD	CaravanArma;
	DWORD	CaravanHopy;
	DWORD	CaravanBuma;
	DWORD   Mount;

	DWORD	Hat;

	TRANS_PREMIUM_TIME()
	{
		size = 0;
		code = 0;
		ThirdEye = 0;
		ExpUp = 0;
		VampCuspid = 0;
		ManaRecharg = 0;
		MightofAwell = 0;
		MightofAwell2 = 0;
		ManaReduce = 0;
		PhenixPet = 0;
		HelpPet = 0;
		VampCuspidEx = 0;
		StaminaReduce = 0;
		CaravanArma = 0;
		CaravanHopy = 0;
		CaravanBuma = 0;
		Hat = 0;
		Mount = 0;
	}
};

#define	FIND_CRACK_LIST_MAX		64

struct	_CRACK_NAME {
	char	szName1[12];
	char	szName2[12];
	char	szClass[12];
};

struct	TRANS_FIND_CRACKER {
	int		size,code;

	int		FindCounter;

	_CRACK_NAME	CrackName[FIND_CRACK_LIST_MAX];

	char	szTemp[128];
};


#define	PARTY_STATE_NONE		0
#define	PARTY_STATE_MASTER		1
#define	PARTY_STATE_MEMBER		2


struct _PARTY_USER {
	char	szName[32];				//ÀÌ¸§
	char	szModelName[64];		//¸ðµ¨ ÀÌ¸§
	char	szModelName2[64];		//¸ðµ¨ ÀÌ¸§2

	DWORD	dwObjectSerial;			//°´Ã¼ °íÀ¯¹øÈ£
	DWORD	Level;
	short	Life[2];				//»ý¸í·Â
	int		x,z;
};

struct TRANS_PARTY_USERS {
	int		size,code;

	DWORD	dwObjectSerial;
	DWORD	dwTemp[8];

	int		PartyUserCount;

	_PARTY_USER		PartyUser[PARTY_PLAYER_MAX];
};

struct _PARTY_PLAYINFO {
	DWORD	dwObjectSerial;			//°´Ã¼ °íÀ¯¹øÈ£
	DWORD	Level;					//·¹º§
	short	Life[2];				//»ý¸í·Â
	int		x,z;
};

struct	TRANS_PARTY_PLAYINFO {
	int		size,code;

	DWORD	dwObjectSerial;
	DWORD	dwTemp;

	int		PartyUserCount;

	_PARTY_PLAYINFO	PlayInfo[PARTY_PLAYER_MAX];
};

struct	TRANS_PARTY_SKILL {
	int		size,code;
	DWORD	dwSkillCode;
	int		Point;
	int		wParam;
	int		lParam;
	int		sParam;
	int		eParam;
	int		PartyCount;
	DWORD	dwPartyUser[8];
};

struct TRANS_CLAN_COMMAND_USER {
	int size,code;
	int	Param[4];
	int	UserCount;

	char	szUserBuff[2048];
};

#define	CLAN_USER_MAX	64

struct _CLAN_USER_INFO {
	char		szName[32];
	DWORD		dwSpeedSum;
	DWORD		dwObjectSerial;
	rsPLAYINFO	*lpPlayInfo;
};

struct	rsCLAN_INFOMATION {
	DWORD	dwClanCode;
	DWORD	dwTemp[32];

	_CLAN_USER_INFO	ClanUserInfo[CLAN_USER_MAX];
};


//Ã¢°í ÀúÀå ±¸Á¶ 
struct	TRANS_WAREHOUSE {
	int size,code;

	DWORD	dwChkSum;
	WORD	wVersion[2];
	int		WareHouseMoney;
	int		UserMoney;
	DWORD	dwTemp[5];

	int		DataSize;
	BYTE	Data[ sizeof( sITEM )*100+256 ];
};

struct	TRANS_CARAVAN {
	int size, code;

	DWORD	dwChkSum;
	WORD	wVersion[2];
	int		WareHouseMoney;
	int		UserMoney;
	DWORD	dwTemp[5];

	int		DataSize;
	BYTE	Data[sizeof(sITEM) * 100 + 256];
};

#define	TRANS_SERVER_MAX		8

//¼­¹ö Á¤º¸
struct _TRANS_SERVER_INFO {
	char	szServerName[32];
	char	szServerIp1[20];
	char	szServerIp2[20];
	char	szServerIp3[20];

	DWORD	dwServerPort1;
	DWORD	dwServerPort2;
	DWORD	dwServerPort3;

	DWORD	ConnState;
};


//¼­¹ö ¼±ÅÃ ¸ñ·Ï 
struct TRANS_SERVER_LIST {
	int size,code;

	char	szServerName[16];

	DWORD	dwServerTime_t;
	int		ClanTicket;
	int	dwTemp[1];
	int	ClanServerCount;
	int	ServerCount;
	_TRANS_SERVER_INFO ServerInfo[TRANS_SERVER_MAX];
};

#define	SERVER_MAP_MAX		64

struct	TRANS_SEVER_MAP {
	int size,code;

	DWORD	dwTemp[30];

	int		IP_Count;
	DWORD	dwIP[SERVER_MAP_MAX];
};

struct	TT_DATA_SERVER_INFO {
	char	szServerID[16];
	char	szServerPath[128];
};


#define	SYSTEM_IP_MAX			32
#define	LOGIN_IP_MAX			16
#define	DISABLE_IP_MAX			500
#define	ENABLE_IP_MAX			100
#define	ADMIN_IP_MAX			16
#define CLAN_SERVER_MAX			3
#define ENABLE_MODULE_CHECK_MAX	64	
#define	TT_DATA_SERVER_MAX		16

//¼­¹ö ¼³Á¤ ±¸Á¶
struct rsSERVER_CONFIG	{

	DWORD	ServerCode;
	DWORD	AutoPlayer;
	DWORD	DisableParty;		//µ¿·á±ÝÁö
	DWORD	EnablePlayerKill;	//PKÇã°¡
	DWORD	ConnectUserMax;		//ÃÖ´ë Á¢¼Ó Çã°¡ ÀÎ¿ø

	int		TestSeverMode;		//Å×½ºÆ® ¼­¹ö¿ë

	int		UseRecorMemory;		//ÀúÀåµ¥ÀÌÅ¸ ¸Þ¸ð¸® ¹öÆÛ»ç¿ë
	int		PotionMonitor;		//¹°¾à °¨½Ã

	int		ClientCheckSumCount;	//Å¬¶óÀÌ¾ðÆ® Ã¼Å©¼¶ Ä«¿îÅÍ
	int		ClientCheckOut;			//¹®Á¦ Å¬¶óÀÌ¾ðÆ® Á¢¼Ó Á¾·á
	DWORD	dwClientCheckSum[32];	//Á¢¼Ó °¡´ÉÇÑ Ã¼Å©¼¶ ¸ñ·Ï

	int		SafeClientCodeCount;	//Å¬¶óÀÌ¾ðÆ®ÀÇ Æã¼ÇÃ½Å© ÄÚµåÄ«¿îÆ®
	int		ClientCodeCheckOut;		//Æã¼Ç¹®Á¦ Å¬¶óÀÌ¾ðÆ® Á¢¼Ó Á¾·á
	DWORD	dwSafeClientCode[32];	//Å¬¶óÀÌ¾ðÆ®ÀÇ Æã¼ÇÃ½Å© ÄÚµå

	int		LimitVersion;			//Á¢¼Ó °¡´ÉÇÑ ÃÖÇÏ ¹öÀü
	int		LimitVersionMax;		//Á¢¼Ó °¡´ÉÇÑ ÃÖ»ó ¹öÀü
	char	szAdminPassword[3][32];	//°ü¸®ÀÚ ±ÇÇÑ ºÎ¿© ÆÐ½º¿öµå
	char	szAdminName[32];		//°ü¸®ÀÚ ÀÌ¸§

	char	szAdminIP[16];			//°ü¸®ÀÚ IP
	char	szAdminLogPassword[32];	//°ü¸®ÀÚ Password

	char	szServerName[64];		//¼­¹ö ÀÌ¸§
	int		Enable_PK;				//PKÇã°¡
	int		CheckProtect;			//nProtect ¼ö½Ã·Î °Ë»çÀ¯¹«
	DWORD	RemoveNProtectTime;		//°ÔÀÓÁß¿¡ nProtectÁ¦°Å ½Ã°£

	char	szDebugIP[10][32];		//µðºñ±ë¿ë IP
	int		DebugIP_Count;			//µð¹ö±ë IP Ä«¿îÅÍ

	char	szDebugID[32][32];		//µð¹ö±ë¿ë ID
	int		DebugID_Count;			//µð¹ö±ë¿ë ID Ä«¿îÅÍ

	int		BlockLevel;				//ID ÀÓ½Ã ºí·°ÀÇ ·¹º§
	int		BlockLevel_LogOut;		//·Î±× ¾Æ¿ô½Ã¿¡¸¸ Àû¿ë( Áß±¹Ãø ¿ä±¸ )
	int		ItemCodeOut;			//¾ÆÀÌÅÛ ÄÚµå ¿À·ù½Ã Á¾·á

	int		ClientFuncCheckMode;	//Å¬¶óÀÌ¾ðÆ® Æã¼Ç °Ë»ç ¸ðµå
	int		Enable_PartyItem;		//µ¿·á°¡ È¹µæÇÑ ¾ÆÀÌÅÛ Ãâ·Â

	
	int		Event_Mandoo;			//¸¸µÎ(¼ÛÆí) ÀÌº¥Æ®
	int		Event_HardCore;			//ÇÏµåÄÚ¾î ÀÌº¥Æ®
	int		Event_DamageOff;		//°ø°Ý·Â È°ÀÎ ÀÌº¥Æ®
//ÇØ¿Ü¹öÀü¿ë/////////////////////////////////////////////////////////////////////////////
	//ÇØ¿Ü
//#ifdef _LANGUAGE_PHILIPIN
	int		Event_DamageOff1;		//¸ó½ºÅÍ °ø°Ý·Â ÇÒÀÎ ÀÌº¥Æ®1 (ÇÊ¸®ÇÉ¿ë)
	int		Event_DamageOff2;		//¸ó½ºÅÍ °ø°Ý·Â ÇÒÀÎ ÀÌº¥Æ®2 (ÇÊ¸®ÇÉ¿ë)
//#endif

	int		Event_DefenseOff;		//¸ó½ºÅÍ ¹æ¾î·Â ¾àÈ­ ÀÌº¥Æ®
/////////////////////////////////////////////////////////////////////////////////////////
	int		Event_Child;			//¾î¸°ÀÌ ÀÌº¥Æ®
	int		Event_Cristal;			//Å©¸®½ºÅ» ÀÌº¥Æ® ( Æ¯Á¤ ½Ã°£´ë¿¡ Å©¸®½ºÅ» ¸ó½ºÅÍ °­È­ )
	int		Event_Potion;			//¹°¾à ÀÌº¥Æ® ( ¹°¾à µ¿½Ã¿¡ ¶³¾îÁö´Â °¹¼ö Áõ°¡ )
	int		Event_StarPointTicket;	//º°Æ÷ÀÎÆ® ÀÌº¥Æ® ( º°»óÇ°±Ç ¹ß»ý )
	int		Event_ValentineDay;		//¹ß·»Å¸ÀÎ µ¥ÀÌ ÀÌº¥Æ®
	int		Event_WhiteDay;			//È­ÀÌÆ® µ¥ÀÌ ÀÌº¥Æ®
	int		Event_MorifSheltom;		//¸ð¸®ÇÁÀÇ ½©ÅÒ Å»Ãë ÀÌº¥Æ®
	int		Event_MollyWolf;		//¸ô¸®¿Í´Á´ë ÀÌº¥Æ®
	int		Event_Ghost;			//°í½ºÆ® ÀÌº¥Æ®
	int		Event_SantaGoblin;		//»êÅ¸°íºí¸° ÀÌº¥Æ®
	int		Event_WorldCup;			//¿ùµåÄÅ ÀÌº¥Æ®
	int		Event_ExpUp;			//°æÇèÄ¡¾÷ ÀÌº¥Æ®
	int     Event_FastExpUp1To2;
	int		Event_CoupleRing;		// pluto Ä¿ÇÃ¸µ ÀÌº¥Æ®
	int		Event_Watermelon;		// ¹ÚÀç¿ø - ¼ö¹Ú ÀÌº¥Æ®
	int		Event_Star;				// ¹ÚÀç¿ø - ¹ãÇÏ´ÃÀÇ ¼Ò¿øÀÌº¥Æ®
	int		Event_Valentine;		// Àåº° - ¹ß·»Å¸ÀÎ ÀÌº¥Æ®
	int		Event_PristonAlphabet;	// ¹ÚÀç¿ø - ¾ËÆÄºª Á¶ÇÕ ÀÌº¥Æ®
	int		Event_WhiteContrast;	// Àåº° - È­ÀÌÆ®ÄÜÆ®¶ó½ºÆ® ÀÌº¥Æ®
	int		Event_Candydays;		// Àåº° - Äµµðµ¥ÀÌÁî
	int		Event_BlackSmith;		// Àåº° - ´ëÀåÀåÀÌÀÇ È¥
	int		Event_MagicalGreen;	// Àåº° - ¸ÅÁöÄÃ±×¸°
	int		Event_TearOfKara;		// Àåº° - Ä«¶óÀÇ ´«¹°
	int		Event_WarerMelonMonster;		// Àåº° - ¼ö¹Ú¸ó½ºÅÍ ÀÌº¥Æ®
	int		Event_Chichen_Day;		// ¹ÚÀç¿ø - º¹³¯ ÀÌº¥Æ®(¾Æ±â ²¿°í ¸ó½ºÅÍ°¡ ³ªÅ¸³­´Ù)
	int		Event_MovieBloodyBattle; // Àåº° - ¿µÈ­ Ç÷Åõ


#ifdef _LANGUAGE_ARGENTINA
	int		Event_ExpUp_latin;		//°æÇèÄ¡¾÷ ÀÌº¥Æ®(¾Æ¸£ÇîÆ¼³ª) //ÇØ¿Ü Kyle
#endif

	int		Event_SummerChicken;	//»ï°èÅÁ ÀÌº¥Æ®
	int		Event_GiftBox;			// pluto ¼±¹°»óÀÚ ÀÌº¥Æ®

	int		BlessCastleMode;			//ºí·¹½ºÄ³½½ ¼³Á¤
	short	sBlessCastle_StartTime[2];	//ºí·¹½ºÄ³½½ ½ÃÀÛ ½Ã°£ ¿¹¾à [0]¿äÀÏ [1]½Ã°£

	int		Record_JobChanger;		//Àü¾÷ÇÑÀÚ ±â·Ï
	int		ForeverBlockMode;		//¿µ¿øÇÑ ºí·°

	int		Permit_Exp;				//Çã¿ë¿ÀÂ÷ °æÇèÄ¡
	int		Permit_Money;			//Çã¿ë¿ÀÂ÷ µ·

	int		ClientProcessTimeChk;	//Å¬¶óÀÌ¾ðÆ® ÇÁ·Î¼¼½º ½Ã°£°¨½Ã ÃÖ´ëÄ¡
	int		ClientProcessTimeChkOut;//Å¬¶óÀÌ¾ðÆ® ÇÁ·Î¼¼½º ½Ã°£ ÃÊ°ú½Ã ¿¬°áÁ¾·á
	int		Dis_PotionError;		//Æ÷¼Ç ¿¡·¯³ª¸é ¿¬°á ²öÀ½

	int		ExpFixMode;				//¿À·ù °æÇèÄ¡ º¸Á¤

	char	szBlockUserPath[64];	//À¯Àú ºí·° µ¥ÀÌÅ¸ °æ·Î

	int		DispBillingTime;		//ºô¸µ ³²Àº½Ã°£ Ç¥½Ã

	int		Disable_LogIn;			//·Î±×ÀÎ ±ÝÁö
	int		Disable_LogQue;			//·Î±×Å¥ Ã³¸® ±ÝÁö
	int		Disable_CheckLogOut;	//·Î±×¾Æ¿ô Å¥°Ë»ç ±ÝÁö

	int		Disable_LogIn2;			//·Î±×ÀÎ ±ÝÁö2¹øÂ° ½Å±Ô ·Î±×ÀÎ¸¸ ±ÝÁö
	int		Disable_PCBang;			//PC¹æ ±ÝÁö ¸ðµå

	int		ExpGameTimeMode;		//°æÇèÄ¡ ½Ã°£º° ºÐ¹è(º£Æ®³²)

	int		ServerInventoryMode;	//¼­¹öÀÇ °¡»óÀÎº¥Åä¸®·Î ¾ÆÀÌÅÛ °ü¸®

	DWORD	DisableIP[DISABLE_IP_MAX][2];		//Á¢±Ù Á¦ÇÑ IP
	int		DisableIP_Count;					//Á¢±Ù Á¦ÇÑ IP ¼³Á¤ ¼ö

	DWORD	EnableIP[ENABLE_IP_MAX][2];			//Á¢±Ù Á¦ÇÑ IPÁß Çã°¡µÇ´Â IP
	int		EnableIP_Count;						//Á¢±Ù Á¦ÇÑ IP ¼³Á¤ ¼ö

	DWORD	AdminIP[ADMIN_IP_MAX][2];			//°ü¸®ÀÚ IP ¸ñ·Ï
	int		AdminIP_Count;						//°ü¸®ÀÚ IP ¼³Á¤ ¼ö

	DWORD	SystemIP[SYSTEM_IP_MAX][2];			//½Ã½ºÅÛ IP ¸ñ·Ï
	int		SystemIP_Count;						//½Ã½ºÅÛ IP ¼³Á¤ ¼ö

	DWORD	LoginServerIP[LOGIN_IP_MAX][2];		//·Î±ä¼­¹ö IP ¸ñ·Ï
	int		LoginServerIP_Count;				//·Î±ä¼­¹ö IP ¼³Á¤ ¼ö

	char	szBackupPath[80];					//*¹é¾÷Æú´õÀÌ¸§

	char	szClanServerIP[CLAN_SERVER_MAX][20];	//Å¬·£ ¼­¹ö	ÁÖ¼Ò
	int		ClanServerPort[CLAN_SERVER_MAX];		//Å¬·£ ¼­¹ö Æ÷Æ®
	int		ClanServerCount;


	DWORD	Enable_ClientModule[ENABLE_MODULE_CHECK_MAX];	//Å¬¶óÀÌ¾ðÆ® ¸ðÆ«Ã½Å©¼¶
	int		Enable_ClientModuleCount;						//Å¬¶óÀÌ¾ðÆ® ¸ðµâ Ã½Å©¼¶ Ä«¿îÅÍ

	int		Disable_DecSkillPoint;							//½ºÅ³ Æ÷ÀÎÆ® °¨¼Ò Ã½Å© Ã³¸®

	int		Disable_Quest;									//±ÝÁö Äù½ºÆ® ¼³Á¤

	int		CrazyPacketMode;							//º¸¾È ÆÐÅ¶ ¸ðµå

	char	szNoticeURL[256];							//Notice URL ¼³Á¤


	TRANS_SEVER_MAP	AreaServer;							//Áö¿ªº° ¼­¹ö ¸ÅÇÎ

	char	szLinkServerIP[32];							//¸µÅ©µÈ ¼­¹ö IP

	int		RecordAdminCommand;							//¿î¿µÀÚ ¸í·É ±â·Ï

	int		FreeLevel;									//¹«·á ·¹º§
	char	szFreeGpCode[32];							//¹«·á»ç¿ë GPCODE
	int		PresentLevel;								//¼±¹°ÁÖ´Â ·¹º§
	int		Event_ComicBirthDay;						//»ýÀÏ¼±¹° ¾óÅ«ÀÌ
	int		Event_Puzzle;								//ÆÛÁñ ¸ÂÃß±â ÀÌº¥Æ®
	int		Event_BabelHorn;							//¹Ùº§»ÔÀÌº¥Æ®
	int		Event_NineFox;								//±¸¹ÌÈ£ÀÌº¥Æ®
	int		EVENT_Christmas;							//Å©¸®½º¸¶½º ÀÌº¥Æ®
	int		EventPlay_BitMask;							//ÀÌº¥Æ® ÇÃ·¹ÀÌ ºñÆ®¸¶½ºÅ©
	int		Event_PCBangPet;							//PC¹æ Æê ÀÌº¥Æ®

	int		StartUser_PresentItem;						//½ÃÀÛÀ¯Àú¿¡°Ô ¼±¹° ÁÖ±â

	int		GameGuard_Auth;								//°ÔÀÓ°¡µå º¸¾È ¼³Á¤

#ifdef _XTRAP_GUARD_4_
	int		XTRAP_Timer;								//XTRAP °Ë»çÁÖ±â		XTRAP_GUARD
	int		XTRAP_MaxDealy;								//XTRAP °Ë»çÁöÃ¼ Á¦ÇÑÄ¡  XTRAP_GUARD
#endif

	//Åä³Ê¸ÕÆ®¼­¹ö °èÁ¤ °ü¸®
	int		TT_DataServer_Count;									//Åä³Ê¸ÕÆ®¼­¹ö µ¥ÀÌÅ¸ °æ·Î °¹¼ö
	TT_DATA_SERVER_INFO		TT_DataServer[TT_DATA_SERVER_MAX];		//Åä³Ê¸ÕÆ®¼­¹ö µ¥ÀÌÅ¸ Á¤º¸

	int		FixedStartField;							//½ÃÀÛ ÇÊµå °­Á¦ °íÁ¤½ÃÅ°±â
	int		DisableThrowItem;							//¾ÆÀÌÅÛ ¶¥¿¡ ¸ø¹ö¸²

	int		CheckPlayField;								//Å¬¶óÀÌ¾ðÆ® ¿Í ¼­¹öÀÇ  ÇÊµå ÁöÇü ºñ±³ °Ë»ç

	int		Enable_PKField;								// PKÇã¿ë ÇÊµå ¹øÈ£

	int		Enable_PKField_All;							// pluto PKÇã¿ë ÇÊµå ÀüºÎ

	//½ºÅ²ÇÙ ¹æÁö
	int		CheckModelFile;								//¸ðµ¨ÆÄÀÏ È®ÀÎ ¿É¼Ç


	#ifdef LOG_THE_PK
		//PK_LOG°ü·Ã //ÇØ¿Ü
		bool	bPK_LOG;
		rsSERVER_CONFIG()
		{
			bPK_LOG = false;
		};
	#endif

};

//¼­¹ö ±âµ¿ 
extern int	ServerMode;


#define	TOTAL_CHECK_MONEY_MAX		52000
#define	TOTAL_CHECK_EXP_MAX			100000




/////////////////// Å¬¶óÀÌ¾ðÆ® Æã¼Ç À§Ä¡ ±¸Á¶Ã¼ ////////////////////////
#define	dwFUNC_VALUE_MAX	64

struct	sFUNC_VALUE {
	DWORD	dwFunc;
	DWORD	dwLen;
	DWORD	dwChkSum;
};

struct	TRANS_CLIENT_FUNPOS {
	int	size,code;

	int	ClientVersion;

	DWORD	dwFunc_CheckMemSum;
	int		FuncCount;

	sFUNC_VALUE	dwFuncValue[dwFUNC_VALUE_MAX];
};


struct	TRANS_FUNC_MEMORY	{
	int		size,code;
	int		Param[8];
	char	szData[6000];
};


struct	sCHK_POTION_COUNT  {
	int	Ps101;
	int	Ps102;
	int	Ps103;
	int	Ps104;

	int	Pm101;
	int	Pm102;
	int	Pm103;
	int	Pm104;

	int	Pl101;
	int	Pl102;
	int	Pl103;
	int	Pl104;
};

//SOD Á¡¼ö Ãâ·Â
struct	sSOD_SCORE {
	int		Flag;
	DWORD	dwDispTime;
	int		Score;
	int		MyTeam;
	int		TeamScore[4];
	int		Round;
	DWORD	dwPlayTime;

	/////// SOD ¿¬Ãâ ÀÌº¥Æ® /////////

	DWORD	dwSoD_NextStageTime;		//´ÙÀ½ ½ºÅ×ÀÌÁö ³Ñ¾î°¡´Â ½Ã°£
	int		SodNextStageNum;			//³Ñ¾î°¥ ½ºÅ×ÀÌÁö ¹øÈ£
	int		NextRound;
	DWORD	dwSoD_CloseStageTime;		//È­¸éÀº ´Ý´Â ÀÌÆåÆ® Àû¿ë½Ã°£

	///////// SOD ¿¹¾à ¿¬Ãâ //////////////
	int		EffectStartCount;			//ÀÌÆåÆ® ½ÃÀÛ±îÁöÀÇ Ä«¿îÅÍ
	int		EffectCode;					//ÀÌÆåÆ® ÄÚµå

	//////// SOD µæÁ¡ È¿°ú /////////////
	int		ScoreEffectCount;				//µæÁ¡ È¿°ú

};

#define	JOBCODE_MECHANICIAN			2
#define	JOBCODE_FIGHTER				1
#define	JOBCODE_PIKEMAN				4
#define	JOBCODE_ARCHER				3

#define	JOBCODE_KNIGHT				6
#define	JOBCODE_ATALANTA			5
#define	JOBCODE_PRIESTESS			8
#define	JOBCODE_MAGICIAN			7


#define	CHAR_EVENT_STARPOINT		300

#define	ITEM_UNIQUE_CODE_NORMAL		1
#define	ITEM_UNIQUE_CODE_QUEST		2


#define BIMASK_PRORATE_USER			1
#define BIMASK_FREE_USER			2
#define BIMASK_VIP_USER				4
#define BIMASK_BIRTHDAY_USER		8
#define BIMASK_TIMELEVEL_USER		16
#define BIMASK_ADULT_USER			32


#define	MONSTER_CLASS_NORMAL		0
#define	MONSTER_CLASS_BOSS			1
#define	MONSTER_CLASS_HAMMER		200
#define	MONSTER_CLASS_GHOST			300