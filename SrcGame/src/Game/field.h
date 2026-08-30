#pragma once

#define FILED_GATE_MAX			12
#define FIELD_MAX				256
#define FIELD_AMBENT_MAX		80
class sFIELD;

// Número dos Mapas
enum EMapID : int
{
	MAPID_Invalid = -1,
	MAPID_AcasiaForest = 0,
	MAPID_BambooForest = 1,
	MAPID_GardenOfFreedom = 2,
	MAPID_RicartenTown = 3,
	MAPID_RefugeOfTheAncients = 4,
	MAPID_CastleOfTheLost = 5,
	MAPID_RuinenVillage = 6,
	MAPID_CursedLand = 7,
	MAPID_ForgottenLand = 8,
	MAPID_NaviskoTown = 9,
	MAPID_Oasis = 10,
	MAPID_AncientsBattlefield = 11,
	MAPID_ForbiddenLand = 12,
	MAPID_AncientPrisonF1 = 13,
	MAPID_AncientPrisonF2 = 14,
	MAPID_AncientPrisonF3 = 15,
	MAPID_ChessRoom = 16,
	MAPID_ForestOfSpirits = 17,
	MAPID_LandOfDusk = 18,
	MAPID_ValleyOfTranquility = 19,
	MAPID_RoadToTheWind = 20,
	MAPID_PillaiTown = 21,
	MAPID_CursedTempleF1 = 22,
	MAPID_CursedTempleF2 = 23,
	MAPID_MushroomCave = 24,
	MAPID_BeehiveCave = 25,
	MAPID_DarkSanctuary = 26,
	MAPID_RailwayOfChaos = 27,
	MAPID_HeartOfPerum = 28,
	MAPID_Eura = 29,
	MAPID_Bellatra = 30,
	MAPID_GallubiaValley = 31,
	MAPID_QuestArena = 32,
	MAPID_BlessCastle = 33,
	MAPID_GreedyLake = 34,
	MAPID_FrozenSanctuary = 35,
	MAPID_KelvezuLair = 36,
	MAPID_LandOfChaos = 37,
	MAPID_LostTemple = 38,
	MAPID_GhostCastle = 39,
	MAPID_EndlessTowerF1 = 40,
	MAPID_EndlessTowerF2 = 41,
	MAPID_CursedTempleF3 = 42,
	MAPID_EndlessTowerF3 = 43,
	MAPID_IceMineF1 = 44,
	MAPID_Atlantis = 45,
	MAPID_MysteryForest1 = 46,
	MAPID_MysteryForest2 = 47,
	MAPID_MysteryForest3 = 48,
	MAPID_BattleTown = 49
};

struct sFGATE	{
	int	x,z,y;				//ÇÊµå °ÔÀÌÆ® À§Ä¡
	sFIELD	*lpsField;		//ÇÊµå Æ÷ÀÎÅÍ
};

//ÇÊµåÀÇ °ÔÀÌÆ®
struct sWARPGATE	{
	int	x,z,y;								//ÇÊµå °ÔÀÌÆ® À§Ä¡
	int	height,size;						//¹üÀ§
	sFGATE	OutGate[FILED_GATE_MAX];		//Ãâ±¸ À§Ä¡
	int		OutGateCount;					//Ãâ±¸ Ä«¿îÅÍ
	int		LimitLevel;						//·¹º§Á¦ÇÑ
	int		SpecialEffect;					//Æ¯¼ö È¿°ú
};

struct	sAMBIENT_POS {
	int	x,y,z;
	int	round;
	int	AmbentNum;
};

#define FIELD_STATE_VILLAGE		0x100
#define FIELD_STATE_FOREST		0x200
#define FIELD_STATE_DESERT		0x300
#define FIELD_STATE_RUIN		0x400
#define FIELD_STATE_DUNGEON		0x500
#define FIELD_STATE_IRON		0x600
#define FIELD_STATE_ROOM		0x800
#define FIELD_STATE_QUEST_ARENA	FIELD_STATE_DUNGEON

//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
#define FIELD_STATE_ICE			0x900
//######################################################################################

#define FIELD_STATE_CASTLE		0xA00
#define FIELD_STATE_ACTION		0xB00


#define FIELD_STATE_ALL			0x1000

#define FIELD_BACKIMAGE_RAIN		0x00
#define FIELD_BACKIMAGE_NIGHT		0x01
#define FIELD_BACKIMAGE_DAY			0x02
#define FIELD_BACKIMAGE_GLOWDAY		0x03
#define FIELD_BACKIMAGE_DESERT		0x04
#define FIELD_BACKIMAGE_GLOWDESERT	0x05
#define FIELD_BACKIMAGE_NIGHTDESERT	0x06
#define FIELD_BACKIMAGE_RUIN1		0x07
#define FIELD_BACKIMAGE_RUIN2		0x08
#define FIELD_BACKIMAGE_NIGHTRUIN1	0x09
#define FIELD_BACKIMAGE_NIGHTRUIN2	0x0A

#define FIELD_BACKIMAGE_GLOWRUIN1	0x0B
#define FIELD_BACKIMAGE_GLOWRUIN2	0x0C


#define FIELD_BACKIMAGE_NIGHTFALL	0x11
#define FIELD_BACKIMAGE_DAYFALL		0x12
#define FIELD_BACKIMAGE_GLOWFALL	0x13

#define FIELD_BACKIMAGE_NIGHTIRON1	0x14
#define FIELD_BACKIMAGE_NIGHTIRON2	0x15
#define FIELD_BACKIMAGE_DAYIRON		0x16
#define FIELD_BACKIMAGE_GLOWIRON	0x17

#define FIELD_BACKIMAGE_SODSKY      0x18    //Ãµ°øÀÇ È¦
#define FIELD_BACKIMAGE_SODMOON     0x19    //´ÞÀÇ Àå
#define FIELD_BACKIMAGE_SODSUN      0x20    //ÅÂ¾çÀÇ Àå
#define FIELD_BACKIMAGE_SODNONE     0x21    //¾Æ¹«°ÍµÎ ¾È³ª¿À´Â ÇÏ´Ã   ³ª¸ÓÁö ÀåµéÀº ÇÏ´ÃÀ» ¾ø¾Ö ¹ö¸³´Ï´Ù.
#define FIELD_BACKIMAGE_IRONBOSS    0x22    //¾ÆÀÌ¾ð º¸½º¸÷ ³ª¿À´Â ÇÏ´Ã

#define FIELD_BACKIMAGE_DAYSNOW		0x23    //
#define FIELD_BACKIMAGE_GLOWSNOW    0x24    //
#define FIELD_BACKIMAGE_NIGHTSNOW   0x25    //

#define FIELD_BACKIMAGE_DAYGREDDY	 0x26    //
#define FIELD_BACKIMAGE_GLOWGREDDY   0x27    //
#define FIELD_BACKIMAGE_NIGHTGREDDY  0x28    //


#define FIELD_BACKIMAGE_DAYLOST		 0x29    //·Î½ºÆ®¿ùµå ³· (¼º±ÙÃß°¡)
#define FIELD_BACKIMAGE_GLOWLOST	 0x2A    //·Î½ºÆ®¿ùµå Àú³á 
#define FIELD_BACKIMAGE_NIGHTLOST    0x2B    //·Î½ºÆ®¿ùµå ¹ã

#define FIELD_BACKIMAGE_DAYTEMPLE	 0x2C	//·Î½ºÆ®ÅÛÇÃ ³·
#define FIELD_BACKIMAGE_GLOWTEMPLE	 0x2D	//·Î½ºÆ®ÅÛÇÃ Àú³á
#define FIELD_BACKIMAGE_NIGHTTEMPLE	 0x2E	//·Î½ºÆ®ÅÛÇÃ ¹ã


#define	FIELD_EVENT_NIGHTMARE		0x01

#define	FIELD_START_POINT_MAX		8

#define	FIELD_STAGE_OBJ_MAX			50


////////////////// ½ÃÀÛ ÇÊµå ¹øÈ£ ///////////////////////
#define START_FIELD_NUM			3
#define START_FIELD_NEBISCO		9
#define START_FIELD_MORYON		21
#define START_FIELD_CASTLE		33

struct	ACTION_FIELD_CAMERA {
	POINT3D	FixPos;
	int		LeftX,RightX;
};

//ÇÊµå ±¸Á¶
class sFIELD {
	DWORD	head;
public:
	char	szName[64];						//ÇÊµå ÆÄÀÏ ÀÌ¸§
	char	szNameMap[64];					//Áö¿ª Áöµµ ÀÌ¹ÌÁö ÆÄÀÏ
	char	szNameTitle[64];				//Áö¿ª ÀÌ¸§ ÀÌ¹ÌÁö ÆÄÀÏ
	int		State;							//ÇÊµå ¼Ó¼º
	int		BackImageCode[3];				//±âº» ¹è°æ ÇÏ´Ã ¹øÈ£
	int		BackMusicCode;					//¹è°æÀ½¾Ç ÄÚµå
	int		FieldEvent;						//ÇÊµåÀÇ ÀÌº¥Æ®

	int		GateCount;						//ÇÊµå °ÔÀÌÆ®ÀÇ ¼ö
	sFGATE	FieldGate[FILED_GATE_MAX];		//ÇÊµå °ÔÀÌÆ® ( ¿¬°áµÇ´Â ÇÊµå )

	int		WarpGateCount;					//¿öÇÁ °ÔÀÌÆ®ÀÇ ¼ö
	int		WarpGateActiveNum;				//¿öÇÁÄÉÀÌÆ® È°¼º ¹øÈ£
	sWARPGATE	WarpGate[FILED_GATE_MAX];	//¿öÇÁ °ÔÀÌÆ®
	POINT3D	PosWarpOut;						//¿öÇÁ °ÔÀÌÆ® Ãâ±¸

	sAMBIENT_POS	AmbientPos[FIELD_AMBENT_MAX];	//¹è°æ È¿°úÀ½
	int		AmbentCount;							//¹è°æ È¿°úÀ½ ¼ö


	int		LimitLevel;						//·¹º§Á¦ÇÑ
	int		FieldSight;						//ÇÊµå½Ã¾ß

	char	*lpStageObjectName[FIELD_STAGE_OBJ_MAX];		//¹è°æ º¸Á¶ ¿ÀºêÁ§Æ®
	DWORD	StgObjBip[FIELD_STAGE_OBJ_MAX];					//¹è°æ º¸Á¶ ¿ÀºêÁ§Æ® BIP¾Ö´Ï¸ÞÀÌ¼Ç »ç¿ëÀ¯¹«
	int		StgObjCount;

	int		cX,cZ;											//ÇÊµå Áß¾Ó
	int		FieldCode;										//ÇÊµåÀÇ ÄÚµå ¹øÈ£
	int		ServerCode;

	POINT	StartPoint[FIELD_START_POINT_MAX];				//ÇÊµå ½ÃÀÛ ÁöÁ¡
	int		StartPointCnt;									//½ÃÀÛ ÁöÁ¡ Æ÷ÀÎÆ®

	ACTION_FIELD_CAMERA	ActionCamera;					//¾×¼ÇÇÊµå¿¡¼­ÀÇ Ä«¸Þ¶ó Á¤º¸
	int	AddStageObject( char *szStgObjName , int BipAnimation=0 );	//¹è°æ º¸Á¶ ¿ÀºêÁ§Æ® Ãß°¡
	int	GetStageObjectName( int num , char *szNameBuff );			//¹è°æ º¸Á¶ ¿ÀºêÁ§Æ® ÀÌ¸§ ¾ò±â

	int	SetCenterPos( int x, int z );								//ÇÊµå Áß¾Ó ÁÂÇ¥ ÀÔ·Â
	int AddGate( sFIELD *lpsField , int x, int z, int y );			//°ÔÀÌÆ® Ãß°¡
	int SetName( char *lpName , char *lpNameMap=0 );				//ÀÌ¸§ ¼³Á¤
	int AddGate2( sFIELD *lpsField , int x, int z, int y );			//°ÔÀÌÆ® Ãß°¡

	int	AddWarpGate(int x, int z, int y, int size, int height, int plus = 0);	//¿öÇÁ°ÔÀÌÆ® Ãß°¡
	int	AddWarpOutGate( sFIELD *lpsField , int x, int z, int y );	//¿öÇÁ°ÔÀÌÆ® Ãâ±¸ Ãß°¡
	int	CheckWarpGate( int x, int y, int z );						//¿öÇÁ°ÔÀÌÆ® È®ÀÎ

	int AddStartPoint( int x, int z );							//½ÃÀÛ ÁÂÇ¥¸¦ ¼³Á¤ Ãß°¡
	int GetStartPoint( int x, int z , int *mx , int *mz );		//Á¦ÀÏ ±ÙÁ¢ÇÑ ½ÃÀÛ ÁÂÇ¥¸¦ ¾ò´Â´Ù 
	int CheckStartPoint( int x, int z );						//½ÃÀÛ ÁÂÇ¥»ó¿¡ Ä³¸¯ÅÍ°¡ ÀÖ´ÂÁö È®ÀÎ 

	int AddAmbientPos( int x, int y, int z , int round , int AmbCode );	//¹è°æ È¿°úÀ½ Ãß°¡
	int	PlayAmbient();											//¹è°æ È¿°úÀ½ ¿¬ÁÖ ( ÁÖ±âÀû È£Ãâ )

};



//ÇÊµå ÀÚµ¿ º¯°æ ¸ÞÀÎ
int FieldMain( int x, int y, int z );
//ÇÊµå ÃÊ±âÈ­
int InitField();
//ÇÊµå ½ÃÀÛ
int StartField();
//ÇÊµå ½ÃÀÛ
int WarpNextField( int *mx, int *mz );
//ÇÊµå ¿öÇÁ
int WarpField( int FieldNum , int *mx, int *mz );
//ÇÊµå ¿öÇÁ
int WarpFieldNearPos( int FieldNum , int x, int z , int *mx, int *mz );
//ÇÊµå ½ÃÀÛ
int WarpStartField( int *mx, int *mz );
//Ä¿½ºÅÒ ÇÊµå·Î ÀÌµ¿
int WarpCustomField( int *mx, int *mz );

//ÇÊµå ½ÃÀÛ ( ±ÍÈ¯¿ë )
int WarpStartField( int FieldNum , int *mx, int *mz );

//°¨¿Á¿¡¼­ ½ÃÀÛ
int WarpPrisonField( int *mx, int *mz );

//À®À» »ç¿ëÇÑ ¿öÇÁ
int	WingWarpGate_Field( int dwFieldCode );

//¿öÇÁ ÇÊµå
int WarpField2( int Num );

//¿öÇÁ ºí·¹½º Ä³½½ ÇÊµå
int WarpCastleField( int MasterClan , int *dx, int *dz );



extern sFIELD	sField[ FIELD_MAX ];
extern int	PrisonX;		//°¨¿ÁÀ§Ä¡
extern int	PrisonZ;		//°¨¿ÁÀ§Ä¡
extern RECT	PrisonRect;		//°¨¿Á Áö¿ª
extern DWORD	dwNextWarpDelay;	//¿öÇÁ µô·¹ÀÌ

/////////////////////// playmain.cpp ¼±¾ð ////////////////////////
//¹è°æÀ» ÀÐ¾î ¿Â´Ù ( ¾²·¹µå »ç¿ë )
smSTAGE3D *LoadStageFromField( sFIELD *lpField , sFIELD *lpSecondField );
//¹è°æÀ» ÀÐ¾î ¿Â´Ù 
smSTAGE3D *LoadStageFromField2( sFIELD *lpField , sFIELD *lpSecondField );

extern	sFIELD	*StageField[2];

extern  int FieldLimitLevel_Table[];

//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
extern int FieldCount;
//######################################################################################
