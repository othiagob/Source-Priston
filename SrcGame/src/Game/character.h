#pragma once

#include "smPacket.h"
#include "smLib3d\smObj3d.h"
#include "smLib3d/smStage3d.h"

#define PLAYBUFF_SIZE		256
#define PLAYBUFF_MASK		255

#define PLAYCOUNT_PER_SECOND		70

#define DIST_TRANSLEVEL_NEAR	0x12000				//((64*3)^2)*2	
#define DIST_TRANSLEVEL_HIGH	0x48000				//((64*6)^2)*2	
#define DIST_TRANSLEVEL_MHIGH	0xC8000				//((64*10)^2)*2	
#define DIST_TRANSLEVEL_MID		0x120000			//((64*12)^2)*2		
#define DIST_TRANSLEVEL_LOW		0x320000			//((64*20)^2)*2	

#define DIST_TRANSLEVEL_DISCONNECT	0x320000			//((64*20)^2)*2	
#define DIST_TRANSLEVEL_CONNECT		0x120000			//((64*12)^2)*2	

#define	PARTY_GETTING_DIST		(18*64*18*64)
#define	PARTY_GETTING_DIST2		(17*64*17*64)

#define PLAYSERVER_SENDCOUNTER		(70*1)

#define	ICE_FOOT_COUNT_MAX		20
#define	ICE_FOOT_LIFE_TIME		5000
#define	ICE_FOOT_ALPHA_STEP		(200.f / float(ICE_FOOT_LIFE_TIME))

struct SIceFootInfo
{
	int     Life;
	int     TypeNum;
	DWORD   dwPlayTime;
	POINT3D FootPos;
	POINT3D FootAngle;
};

#define DPAT_MAX		2048

#include <list>

namespace Delta3D::Graphics
{
	class Mesh;
};

struct ModelGroup
{
	std::list<Delta3D::Graphics::Mesh*> meshes;
};

struct smDPAT 
{
	smDPAT* lpTalkLink;

	smDPAT* smDinaLink;
	smPAT3D* Pat;

	char	szPatName[64];
	int		UseCount;
	DWORD	dwSpeedFindSum;

	smMODELINFO* lpModelInfo;

	struct ModelsGroup
	{
		ModelGroup sHighModel;
		ModelGroup sDefaultModel;
		ModelGroup sLowModel;
	};
	ModelsGroup* sModelsGroup = nullptr;

	DWORD	LastUsedTime;
};


class smPATTERN 
{
public:

	smPATTERN *BipPattern;

	smDPAT	DinaPat[ DPAT_MAX ];

	smPATTERN(void);
	~smPATTERN(void);

	void Init();
	int	Close();

	int GetNew();
	int FindFromName( char *szName );
	int FindFromCode( DWORD dwCode );
	smDPAT *LoadCharactor( char *szFileName ) ;
	smDPAT *LoadBipPattern( char *szFileName ) ;

};

#define	DISP_MODE_PATMAIN	1
#define	DISP_MODE_PATSUB	2
#define	DISP_MODE_PATTALK	4
#define CHAR_FRAME_MASK		0x00FFFFFF
#define CHAR_FRAME_SELSHIFT	24	

namespace Delta3D::Graphics
{
	class Material;
};

struct smCHARTOOL
{
	smOBJ3D* ObjBip;
	smPAT3D* PatTool;
	DWORD	dwItemCode;

	int	SizeMax, SizeMin;

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
	int		BlinkScale;
	int		EffectKind;

	union
	{
		Delta3D::Graphics::Material* blendingMaterial = nullptr;
		int		TexMixCode;
	};
	int TexScroll;
};



//////////// ¸ÞÄ«´Ï¼Ç ½ºÅ³ //////////////
#define SKILL_PLAY_EXTREME_SHIELD	0x01
#define SKILL_PLAY_MECHANIC_BOMB	0x02
#define	SKILL_PLAY_PHYSICAL_ABSORB	0x03
#define	SKILL_PLAY_POISON_ATTRIBUTE	0x04

#define	SKILL_PLAY_GREAT_SMASH		0x05
#define	SKILL_PLAY_MAXIMIZE			0x06
#define	SKILL_PLAY_AUTOMATION		0x07
#define	SKILL_PLAY_SPARK			0x08

#define	SKILL_PLAY_METAL_ARMOR		0x09
#define	SKILL_PLAY_GRAND_SMASH		0x0A
#define	SKILL_PLAY_SPARK_SHIELD		0x0B

#define	SKILL_PLAY_IMPULSION		0x0C
#define	SKILL_PLAY_COMPULSION		0x0D
#define	SKILL_PLAY_MAGNETIC_SPHERE	0x0E
#define	SKILL_PLAY_METAL_GOLEM		0x0F


//////////// ÆÄÀÌÅ©¸Ç ½ºÅ³ //////////////
#define SKILL_PLAY_PIKEWIND			0x12	// ¹ÚÀç¿ø - ¸ÅÁ÷ Æ÷½º »ç¿ë½Ã 1Â÷½ºÅ³·Î ÀÎ½Ä ¸øÇÏ¿©¼­ ÇÒ´çµÈ ÄÚµå ¼öÁ¤(0x10 -> 0x12 ·Î ¼öÁ¤ÇÔ)
#define SKILL_PLAY_CRITICAL_HIT		0x13
#define SKILL_PLAY_JUMPING_CRASH	0x14

#define SKILL_PLAY_GROUND_PIKE		0x15
#define SKILL_PLAY_TORNADO			0x16
#define SKILL_PLAY_EXPANSION		0x18

#define SKILL_PLAY_VENOM_SPEAR		0x19

#define SKILL_PLAY_BLADE_OF_BLAZE	0x1A		//(±¸)
#define SKILL_PLAY_VANISH			0x1A		//SKILL_PLAY_BLADE_OF_BLAZE ¶û °°Àº ÄÚµå

#define SKILL_PLAY_CHAIN_LANCE		0x1B

#define SKILL_PLAY_ASSASSIN_EYE		0x1C
#define SKILL_PLAY_CHARGING_STRIKE	0x1D
#define SKILL_PLAY_VAGUE			0x1E
#define SKILL_PLAY_SHADOW_MASTER	0x1F


//////////// ÆÄÀÌÅÍ ½ºÅ³ //////////////
#define SKILL_PLAY_RAVING			0x23
#define SKILL_PLAY_IMPACT			0x24

#define SKILL_PLAY_TRIPLE_IMPACT	0x25
#define SKILL_PLAY_BRUTAL_SWING		0x26
#define SKILL_PLAY_ROAR				0x27
#define SKILL_PLAY_RAGEOF_ZECRAM	0x28

#define SKILL_PLAY_CONCENTRATION	0x29
#define SKILL_PLAY_AVANGING_CRASH	0x2A
#define SKILL_PLAY_SWIFT_AXE		0x2B
#define SKILL_PLAY_BONE_SMASH		0x2C

#define SKILL_PLAY_DESTROYER		0x2D
#define SKILL_PLAY_BERSERKER		0x2E
#define SKILL_PLAY_CYCLONE_STRIKE	0x2F

//////////// ¾ÆÃÄ ½ºÅ³ //////////////

#define SKILL_PLAY_SCOUT_HAWK		0x41
#define SKILL_PLAY_WIND_ARROW		0x43
#define SKILL_PLAY_PERFECT_AIM		0x44

#define SKILL_PLAY_FALCON			0x46
#define SKILL_PLAY_ARROWOF_RAGE		0x47
#define SKILL_PLAY_AVALANCHE		0x48

#define SKILL_PLAY_ELEMENTAL_SHOT	0x49
#define SKILL_PLAY_GOLDEN_FALCON	0x4A
#define SKILL_PLAY_BOMB_SHOT		0x4B
#define SKILL_PLAY_PERFORATION		0x4C

#define SKILL_PLAY_RECALL_WOLVERIN	0x4D
#define SKILL_PLAY_PHOENIX_SHOT		0x4E
#define SKILL_PLAY_FORCE_OF_NATURE	0x4F


////////////// ³ªÀÌÆ® ½ºÅ³ //////////////
#define SKILL_PLAY_SWORD_BLAST		0x51
#define SKILL_PLAY_HOLY_BODY		0x52
#define SKILL_PLAY_DOUBLE_CRASH		0x54

#define SKILL_PLAY_HOLY_VALOR		0x55
#define SKILL_PLAY_BRANDISH			0x56
#define SKILL_PLAY_PIERCING			0x57
#define SKILL_PLAY_DRASTIC_SPIRIT	0x58

#define SKILL_PLAY_FLAME_BRANDISH		0x59
#define SKILL_PLAY_DIVINE_INHALATION	0x5A
#define SKILL_PLAY_HOLY_INCANTATION		0x5B
#define SKILL_PLAY_GRAND_CROSS			0x5C

#define SKILL_PLAY_DIVINE_PIERCING		0x5D
#define SKILL_PLAY_GODLY_SHIELD			0x5E
#define SKILL_PLAY_GODS_BLESS			0x5F
#define SKILL_PLAY_SWORD_OF_JUSTICE		0x50


////////////// ¾ÆÅ»¶õÅ¸ ½ºÅ³ ///////////////
#define SKILL_PLAY_SHIELD_STRIKE	0x61
#define SKILL_PLAY_FARINA			0x62
#define SKILL_PLAY_VIGOR_SPEAR		0x64

#define SKILL_PLAY_WINDY			0x65
#define SKILL_PLAY_TWIST_JAVELIN	0x66
#define SKILL_PLAY_SOUL_SUCKER		0x67
#define SKILL_PLAY_FIRE_JAVELIN		0x68

#define SKILL_PLAY_SPLIT_JAVELIN		0x69
#define SKILL_PLAY_TRIUMPH_OF_VALHALLA	0x6A
#define SKILL_PLAY_LIGHTNING_JAVELIN	0x6B
#define SKILL_PLAY_STORM_JAVELIN		0x6C

#define SKILL_PLAY_HALL_OF_VALHALLA		0x6D
#define SKILL_PLAY_X_RAGE				0x6E
#define SKILL_PLAY_FROST_JAVELIN		0x6F
#define SKILL_PLAY_VENGEANCE			0x60


////////////// ÇÁ¸®½ºÆ¼½º ½ºÅ³ ///////////////
#define SKILL_PLAY_HEALING			0x71
#define SKILL_PLAY_HOLY_BOLT		0x72
#define SKILL_PLAY_MULTI_SPARK		0x73
#define SKILL_PLAY_HOLY_MIND		0x74

//#define	SKILL_PLAY_MEDITATION		0x75
#define	SKILL_PLAY_DIVINE_LIGHTNING	0x76
#define	SKILL_PLAY_HOLY_REFLECTION	0x77
#define	SKILL_PLAY_GREAT_HEALING	0x78

#define	SKILL_PLAY_VIGOR_BALL		0x79
#define	SKILL_PLAY_RESURRECTION		0x7A
#define	SKILL_PLAY_EXTINCTION		0x7B
#define	SKILL_PLAY_VIRTUAL_LIFE		0x7C

#define	SKILL_PLAY_GLACIAL_SPIKE	0x7D
#define	SKILL_PLAY_REGENERATION_FIELD	0x7E
#define	SKILL_PLAY_CHAIN_LIGHTNING	0x7F
#define	SKILL_PLAY_SUMMON_MUSPELL	0x70


////////////// ¸ÅÁö¼Ç ½ºÅ³ ///////////////
#define SKILL_PLAY_AGONY			0x81
#define SKILL_PLAY_FIRE_BOLT		0x82
#define SKILL_PLAY_ZENITH			0x83
#define SKILL_PLAY_FIRE_BALL		0x84

//#define SKILL_PLAY_MENTAL_MASTERY	0x85
#define SKILL_PLAY_COLUMN_OF_WATER	0x86
#define SKILL_PLAY_ENCHANT_WEAPON	0x87
#define SKILL_PLAY_DEAD_RAY			0x88

#define SKILL_PLAY_ENERGY_SHIELD	0x89
#define SKILL_PLAY_DIASTROPHISM		0x8A
#define SKILL_PLAY_SPIRIT_ELEMENTAL	0x8B
#define SKILL_PLAY_DANCING_SWORD	0x8C

#define SKILL_PLAY_FIRE_ELEMENTAL	0x8D
#define SKILL_PLAY_FLAME_WAVE		0x8E
#define SKILL_PLAY_DISTORTION		0x8F
#define SKILL_PLAY_METEO			0x80

///////////// ¸ð¼Ç È®Àå //////////////////
#define SKILL_PLAY_DIVINE_PIERCING2		0x90
#define SKILL_PLAY_DIVINE_PIERCING3		0x91

#define SKILL_TOPLVL					0x92
#define SKILL_PLAY_EVASION_MASTERY		0x93
////////////////////////////////////////////

#define SKILL_PLAY_BLESS_ABSORB			0xA0
#define SKILL_PLAY_BLESS_DAMAGE			0xA1
#define SKILL_PLAY_BLESS_EVADE			0xA2
#define SKILL_PLAY_BLESS_SIEGE_ITEM		0xA8

#define SKILL_PLAY_FIRE_CRYSTAL			0xB0
#define SKILL_PLAY_LIGHTNING_CRYSTAL	0xB1
#define SKILL_PLAY_ICE_CRYSTAL			0xB2
#define SKILL_PLAY_CHAOSCARA_VAMP		0xB8

#define SKILL_PLAY_PET_ATTACK		0xC0
#define SKILL_PLAY_PET_ATTACK2		0xC1

#define SKILL_PLAY_SOD_ITEM			0xD0
//#define SKILL_PLAY_METEO			0xE0
#define SKILL_PLAY_SPEACIAL			0xF0
#define	SKILL_PLAY_LOVELY_LIFE		0xF1

#define	SKILL_EFFECT_FIREFLOWER		0x12000010


//½ºÅ³ Àû¿ë ¸¶½ºÅ©
#define	SKILL_APPMASK_EXTREME_SHIELD		0x0001
#define	SKILL_APPMASK_PHYSICAL_ABSORB		0x0002
#define	SKILL_APPMASK_AUTOMATION			0x0004
#define	SKILL_APPMASK_ANGER					0x0008
#define	SKILL_APPMASK_SPARK_SHIELD			0x0010
#define	SKILL_APPMASK_GODLY_SHIELD			0x0020


#define	SKILL_APPMASK_HOLY_BODY				0x00010000


//½ºÅ³ Àû¿ë ¸¶½ºÅ© ( ¸ó½ºÅÍ ¼Ó¼º )
//#define	SKILL_APPMASK_ICE					0x0010


//µ¥¹ÌÁö¸¦ ÁØ À¯Àú¸¦ ±â¾ï½ÃÅ´
struct	ATTACK_DAMAGE_LIST	{
	DWORD	dwUserCode;
	int		DamageCount;
	int		Count;
};

#define	ATTACK_DAMAGE_LIST_MAX			100


class smCHAR {

	DWORD	Head;

public:
	
	int		DisplayFlag;		//È­¸é Ãâ·Â ¿©ºÎ
	//int		DisplayHead;		//¸Ó¸® Ãú·Â ¿©ºÎ
	int		DisplayTools;		//¹«±â Ãâ·Â ¿©ºÎ

	smSTAGE3D	*lpStage;		//ÇØ´ç ¹è°æ Æ÷ÀÎÆ®
	DWORD		dwObjectSerial;	//°´Ã¼ °íÀ¯ ¹øÈ£
	int			AutoPlayer;		//ÀÚµ¿ ÇÃ·¹ÀÌ¾î ( TRUE ÀÏ °æ¿ì ¼­¹ö·Î ºÎÅÍ ¿òÁ÷ÀÓÀ» ¹Þ´Â´Ù )

	//Equipe na arena
	int nEquipeArena = 0;

	//Nível do vip
	int vipLevel = 0;

	smPAT3D	*Pattern;			//ÀÏÂ÷ ÆÐÅÏ
	smPAT3D	*Pattern2;			//¸Ó¸® ÆÐÅÏ
	smPAT3D *AnimPattern;

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	smPAT3D *TalkPattern;		// Ç¥Á¤ ÆÐÅÏ. ( ´ÙÀÌ³ª¹Í ÆÐÅÏÀº ini ¿¡ µû¶ó¼­ lpDinaPattern ¶Ç´Â lpDinaPattern2 )
	//######################################################################################

	smDPAT  *lpDinaPattern;		//´ÙÀÌ³ª¹Í ÆÐÅÏ Æ÷ÀÎÅÍ (ÆÐÅÏÀÇ °ü¸®ÀÚ)
	smDPAT  *lpDinaPattern2;	//´ÙÀÌ³ª¹Í ÆÐÅÏ Æ÷ÀÎÅÍ ¸Ó¸®(ÆÐÅÏÀÇ °ü¸®ÀÚ)

	smDPAT  *lpDinaLeftPattern;		//¿ÞÂÊ ¹«±â
	smDPAT  *lpDinaRightPattern;	//¿À¸¥ÂÊ ¹«±â

	smCHARTOOL	HvLeftHand;		//¿Þ¼Õ¿¡ Âø¿ë µµ±¸
	smCHARTOOL	HvRightHand;	//¿À¸¥¼Õ¿¡ Âø¿ë µµ±¸

	smMATRIX	HvLeftHand_Matrix;	//¿Þ¼Õ ¹«±â Çà·Ä º¹»ç
	smMATRIX	HvRightHand_Matrix; //¿À¸¥¼Õ ¹«±â Çà·Ä º¹»ç

	int		Rend_HvLeftHand;		//¹«±â ·»´õ¸µ À¯¹«
	int		Rend_HvRightHand;		//¹«±â ·»´õ¸µ À¯¹«


	smOBJ3D	*BackObjBip[3];		//µîÂÊ ¹«±â Âø¿ë
	smOBJ3D	*AttackObjBip;		//°ø°Ý ÆÐµå ¿ÀºêÁ§Æ®
	smOBJ3D	*ShieldObjBip;		//¹æÆÐ ÆÐµå ¿ÀºêÁ§Æ®

	int		AttackToolRange;	//°ø°Ý »çÁ¤°Å¸®
	int		AttackAnger;		//°ø°Ý½Ã ºÐµµÄ¡ ( ±×³É º¸±â ÁÁ°Ô ÇÏ±â À§ÇÔ )
	int		AttackIce;			//¾óÀ½ °ø°Ý ¹ÞÀ½

	BOOL	bActive = TRUE;
	BOOL	bPartyLeader = FALSE;
	union
	{
		struct
		{
			int pX;
			int pY;
			int pZ;
		};

		Point3D sPosition;
	};
	
	int		Flag;
	int		AttackX,AttackY,AttackZ;		// ÁÂÇ¥
	int		PHeight;		// ¹Ù´Ú ³ôÀÌ ÁÂÇ¥
	int		FallHeight;		// ¶³¾îÁø ³ôÀÌ 
	int		OnStageField;	// ¹â°í ¼­ÀÖ´Â ¹Ù´Ú ÇÊµå ¹øÈ£
	int		OnStageFieldState;	//¹Ù´Ú ÇÊµåÀÇ ¼Ó¼º°ª

	int		PatLoading;		//ÇöÀç ·Îµù ÁßÀÎÁö...

	int		PatWidth;		//Ä³¸¯ÅÍ ³ÐÀÌ
	int		PatHeight;		//Ä³¸¯ÅÍ ³ôÀÌ

	int		PatSizeLevel;	//Ä³¸¯ÅÍ Å©±â ´Ü°è ( 0 ~ 3 )

	int		OverLapPosi;	//´Ù¸¥ Ä³¸¯ÅÍ¿Í À§Ä¡°¡ °ãÄ§
	int		OverLapPosi2;	//´Ù¸¥ Ä³¸¯ÅÍ¿Í À§Ä¡°¡ °ãÄ§
	smCHAR	*lpCharOverLap;	//°ãÄ£ Ä³¸¯ÅÍÀÇ Æ÷ÀÎÆ®

	POINT3D Posi;

	union
	{
		struct
		{
			POINT3D Angle;
		};

		Point3D sAngle;
	};

	int		MoveMode;	//ÀÌµ¿ ¸ðµå ( 0 - °È±â 1-´Þ¸®±â )
	int		MoveFlag;
	int		MoveCnt;	// Åë½Å½Ã ¹ÞÀº µ¥ÀÌÅ¸°¡ ¾øÀ»°æ¿ì ÀÚµ¿ ÀÌµ¿ÇÒ¶§ °Å¸® Á¦ÇÑ Ä«¿îÅÍ
	int		tx,tz;		// ¸ñÇ¥ ÀÌµ¿ ÁÂÇ¥
	int		TargetMoveCount;	//¸ñÇ¥ ÀÌµ¿ ÁÂÇ¥ Ä«¿îÅÍ
	int		WaterHeight;		//¹° ³ôÀÌ

	DWORD	dwActionItemCode;	//ÁÖ ¿òÁ÷ÀÓ¿¡ °ü·ÃµÈ ÄÚµå
	DWORD	dwItemSetting;		//¾ÆÀÌÅÛ ¼¼ÆÃ ¹æ¹ý( 0-±¸ºÐ ¾øÀ½ 1-¸¶À»ÇÊµå ±¸ºÐ )
	DWORD	dwActionItemTwoHand;	//¾ç¼Õ ¹«±â Ã½Å©

	short	wStickItems[4];		//ÀåÂø ¾ÆÀÌÅÛ ¹øÈ£

	//È­»ì ¹× ´øÁö±â ¸ðµå
	int		ShootingMode;	//¹ß»çÇü °ø°Ý ¸ðµå ( TRUE )
	int		ShootingFlag;	//ÇöÀç ¹ß»ç‰ç´ÂÁö ¿©ºÎ
	int		ShootingKind;	//¹ß»ç ±¸ºÐ
	POINT3D	ShootingPosi;	//¹ß»ç À§Ä¡

	POINT3D	ShootingAngle;	//¹ß»ç ¹æÇâ
	int		ShootingCount;	//¹ß»ç Ä«¿îÅÍ

	smCHAR	*chrAttackTarget;	//°ø°Ý ¸ñÇ¥ Ä³¸¯ÅÍ
	int		AttackCritcal;		//Å©¸®Æ¼ÄÃ °ø°Ý À¯¹«
	int		AttackCriticalCount;//Å©¸®Æ¼ÄÃ °ø°Ý Ä«¿îÅÍ ( ¿¬¼Ó °ø°Ý½Ã )
	int		AttackExp;			//°ø°Ý½Ã °æÇèÄ¡
	int		AttackSkil;			//½ºÅ³ °ø°Ý ÄÚµå
	int		AttackEffect;		//°ø°Ý½Ã Æ¯¼ö ÀÌÆåÆ® Ç¥½Ã
	int		LastSkillParam;		//ÃÖ±Ù ½ºÅ³°ø°Ý ±¸ºÐ ÄÚµå ( Ä«¿À½ºÄ«¶ó ¶«¿¡ ¶«»§¿ëÀ¸·Î Ãß°¡ ) 

	int		RecvExp;			//°æÇèÄ¡ ºÎ¿© ¹ÞÀ½

	POINT3D	ptNextTarget;	//´ÙÀ½ ÀÌµ¿ ¸ñÇ¥
	POINT3D	PosiAround;		// ÁÖº¯ À§Ä¡ ÀÌ°÷À» ¸Ö¸® ¹þ¾î³ªÁö ¸øÇÏ°Ô ÇÔ (±ÙÃ³¿¡¼­¸¸..)
	int		DistAroundDbl;	// ÁÖº¯ °æ°è °Å¸® ÇÑ°èÄ¡
	int		TragetTraceMode;	//ÃßÀû ¸ðµå °ª (¼­¹ö»ç¿ë )

	POINT3D	PosBeginMove;		//ÀÌµ¿ ½ÃÀÛ½ÃÀÇ ÁÂÇ¥
	DWORD	PosBeginCount;

	DWORD	dwNextMotionCode;	//´ÙÀ½ µ¿ÀÛ ¿¹¾à

	int		ChargingFlag;		//Â÷Â¡ ÇÃ·¢

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	DWORD	TalkFrame;		// Ç¥Á¤ ÇÁ·¹ÀÓ
	DWORD	TalkSoundSum;

	SMotionStEndInfo	FrameInfo;
	//######################################################################################

	int		action;
	DWORD	frame;
	int		FrameCnt;
	int		FrameStep;		//ÇÁ·¹ÀÓ ¹Ù²ñ °ª
	int		MoveSpeed;		//ÀÌµ¿ ¿òÁ÷ÀÓ °ª
	int		AttackSpeed;	//°ø°Ý ¼Óµµ

	int		ActionPattern;	// ¿òÁ÷ÀÓ ÆÐÅÏ

	DWORD	dwEventFrameExt[4];	//ÀÌº¥Æ® ÇÁ·¹ÀÓ È®Àå

	int		NearPlayCount;	//±ÙÃ³¿¡ ´Ù¸¥ À¯Àú°¡ Á¸Àç ÇÏ´ÂÁö È®ÀÎ ( NPC/¸ó½ºÅÍ )
	int		ReopenCount;	//¸ó½ºÅÍ ½Ã°£ÀÌ Áö³ª ÀçÃâÇö ½ÃÅ³ Ä«¿îÅÍ
	int		AutoMoveStep;	//ÀÚµ¿ ÀÌµ¿½Ã ½ºÅÜ Ä«¿îÅÍ
	short	sMoveStepCount[2];		//ÀÌµ¿ ½ºÅÜ ·çÇÁ °¡¿îÅÍ

	int		Counter;
	int		FrameCounter;			//¸ð¼Ç ½ÃÀÛºÎÅÍ ³¡³¯¶§ ±îÁö »ç¿ëµÇ´Â Ä«¿îÅÍ

	int		RendSucess;				//È­¸é»óÀÇ ·»´õ¸µ ¼º°ø ¿©ºÎ
	RECT	RendRect2D;				//·»´õ¸µµÈ 2D ÁÂÇ¥ ¿µ¿ª
	POINT3D	RendPoint;				//·»´õ¸µµÈ 2D ÁÂÇ¥ ( x,y,z )
	int		FlagShow;				//È­¸éÇ¥½Ã ÇÃ·¢ ( 0 - È­¸é Ãâ·Â ºÒÇã )

	//Ã¤ÆÃ
	char	szChatMessage[256];		//Ã¤ÆÃ ¹®ÀÚ¿­
	DWORD	dwChatMessageTimer;		//Ã¤ÆÃ ¹®ÀÚ¸¦ Ç¥½Ã Á¾·á½Ã°£ ±â·Ï

	//»óÁ¡
	char	szTradeMessage[128];	//»óÁ¡ ¸Þ¼¼Áö ¹®ÀÚ¿­
	DWORD	dwTradeMsgCode;			//»óÁ¡ ¸Þ¼¼Áö ÄÚµå


	char	srTransBuff[TRANS_BUFF_SIZE];	//Àü¼ÛÇÒ µ¥ÀÌÅ¸ ´ë±â¿ë ¹öÆÛ
	int		srTransBuffSize;				//Àü¼ÛÇÒ µ¥ÀÌÅ¸ ´ë±â¿ë ¹öÆÛ Å©±â

	//Åë½Å °ü·Ã
	int		ServerCode;				//ÇØ´ç Ä³¸¯ÅÍ ¼­¹ö ÄÚµå

	smWINSOCK *TransSock;

	int		TransSendWait;			//Åë½Å Æ÷Æ® ±â´Ù¸² 
	int		TransLastSendCnt;		//Åë½Å ¸¶Áö¸· Àü¼Û ½Ã°£
	int		TransLastSendTime;		//Åë½Å ¸¶Áö¸· Àü¼ÛÀÇ ½Ã°£Â÷ ( ¼Óµµ Ã½Å©¿ë )
	int		TransMinCnt;			//ÃÖ¼Ò ±³½Å Ä«¿îÅÍ
	int		TransLevel;				//Åë½ÅÀÇ Áß¿ä ´Ü°è
	int		TransDelayMax;			//Åë½Å Áö¿¬ ÃÖ´ëÄ¡ ( ÀÌ½Ã°£À» ³ÑÀ¸¸é Àç½Ãµµ )

	smPLAYBUFF	PlayBuff[PLAYBUFF_SIZE];	//ÇÃ·¹ÀÌ ÁøÇà »óÈ²À» ¹öÆÛ¿¡ ÀúÀåÇÏ¿© Åë½Å½Ã ¹öÆÛ¸µÇÔ
	int		PlayBuffCnt;					//PlayBuff ÀÇ Ä«¿îÅÍ
	int		PlayBuffPosi_End;				//PlayBuff ¹öÆÛ¸µ ³¡ À§Ä¡

	DWORD	dwLastTransTime;				//¸¶Áö¸· ±³½Å ½Ã°£
	int		LastPlayDataType;				//¸¶Áö¸· ±³½ÅÇÑ µ¥ÀÌÅ¸ Å¸ÀÔ

	smCHAR_INFO		smCharInfo;				//À¯Àú Á¤º¸
	smCHAR_MONSTER_INFO	smMonsterInfo;		//¸ó½ºÅÍ °ü·Ã ±¸Á¶

	int		AnimDispMode;			//µ¿ÀÛ Ç¥½Ã ¸ðµå ( 0-¸ðµÎ Ç¥½Ã 1-ÇÑ°³¾¿ Ç¥½Ã)
	int		PatDispMode;			//ÆÐÅÏ Ç¥½Ã ¸ðµå ( 0-ºñÇ¥½Ã 1-¸ÞÀÎ 2-¼­ºê¸¸ 3-¸ðµÎ )

	int		MotionSelectFrame;		//ÇöÀçÀÇ ÇÁ·¹ÀÓÀÇ ¸ð¼ÇÀÇ ¹øÈ£ ( 0 , 1 ) 

	smMODELINFO		*smMotionInfo;	//¸ðµ¨ÀÇ µ¿ÀÛº° ÇÁ·¹ÀÓ°ú Á¤º¸
	smMODELINFO		*smMotionInfo2;	//¸ðµ¨ÀÌ 2°³ÀÏ °æ¿ì 1Â÷¸ðµ¨¿¡¼­ µ¥ÀÌÅ¸°¡ ¾ø´Â°æ¿ì 2Â÷¸ðµ¨ÀÇ µ¥ÀÌÅ¸¸¦ Ãâ·Â 
	smMOTIONINFO	*MotionInfo;

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	smMODELINFO		*lpTalkModelParent;
	smMOTIONINFO	*lpTalkMotionInfo;
	//######################################################################################


	DWORD			OldMotionState;			//ÀÌÀüÀÇ ¸ð¼Ç ¼Ó¼º Á¤º¸
	DWORD			dwTarget;				//»ó´ë¹æÀÌ ÁÖÀÎ°øÀ» °Ü³ÉÇÑ ¾×¼ÇÀÏ°æ¿ì 

	//¼ÒÈ¯µÈ Ä³¸¯ÅÍ ( ¸¶½ºÅÍ À¯Àú°¡ Á¸Àç )
	rsPLAYINFO		*lpMasterPlayInfo;		//ÁÖÀÎ Ä³¸¯ÅÍ (¼­¹ö¿ë)

	int						nCheckChar;		//³»°¡ ºÒ·¯³½ °ø°Ý Ä³¸¯ÅÍ ±¸ºÐ (¼­¹ö¿ë) // Àåº° - ¼Ò¿ï½ºÅæ

	//¸ñÀû Ä³¸¯ÅÍ ~~ 
	rsPLAYINFO		*lpTargetPlayInfo;		//¸ñÇ¥ Ä³¸¯ÅÍ (¼­¹ö¿ë)
	smCHAR			*lpTargetChar;			//¸ñÇ¥ Ä³¸¯ÅÍ (Å¬¶óÀÌ¾ðÆ®¿ë)
	int				AttackUserFlag;			//¸ó½ºÅÍ°¡ À¯Àú¸¦ °ø°ÝÇÔ
	DWORD			dwTargetLockTime;		//¸ñÇ¥ º¯°æ ±ÝÁö

	DWORD			dwLinkObjectCode;		//¿¬°áµÈ ¿ÀºêÁ§Æ® ÄÚµå
	rsPLAYINFO		*lpLinkPlayInfo;		//¿¬°áµÈ ÇÃ·¹ÀÌ¾î
	smCHAR			*lpLinkChar;			//¿¬°áµÈ Ä³·¢ÅÍ

	//°ø°Ý´çÇÑ Ä³¸¯ÅÍ Á¤º¸ ( ¸ó½ºÅÍ °æÇèÄ¡ ¹èºÐÀ» ¸ñÀû - ¼­¹ö¿ë )
	rsPLAYINFO		*lpExpAttackPlayInfo;
	int				ExpAttackLife;
	DWORD			dwExpAttackTime;
	TRANS_ATTACKDATA AttackTrans;			//°ø°Ý µ¥ÀÌÅ¸ ¹öÆÛ
	DWORD			dwAttackPlayTime;		//°ø°Ý Àû¿ë ½Ã°£

	int				PotionLog;				//Æ÷¼Ç »ç¿ë ±â·Ï
	int				LevelLog;				//·¹º§ º¯È­ ±â·Ï
	int				CriticalLog;			//Å©¸®Æ¼ÄÃ ·Î±×

	int				EnableStateBar;			//Ã¼·Â¹Ù ±×¸®±â ¿É¼Ç
	int				MotionRecordCount;		//¸ð¼Ç Á¤º¸ ±â·Ï Ä«¿îÅÍ
	int				OpenStartPostion;		//Ä³¸¯ÅÍÀÇ ½ÃÀÛ Ãâ½Å±ê¹ß ¹øÈ£ (¼­¹ö¿ë)

	int				DisplayAlpha;			//¹ÝÅõ¸íµµ ( ÀÓ½Ã ¹ÝÅõ¸í Ãâ·Â )
	int				RendAlpha;				//¹ÝÅõ¸íµµ ( ¿ø·¡ ¹ÝÅõ¸í Ä³¸¯ÅÍ )
	int				RenderLatter;			//·»´õ¸µ ³ªÁß¿¡

	DWORD			dwDispAppSkill;			//½ºÅ³ Àû¿ëÇ¥½Ã
	DWORD			dwDispAppSkillMask;		//½ºÅ³ Àû¿ëÇ¥½Ã ¸¶½ºÅ© ( µ¿±â¸¦ ¸ÂÃß±â À§ÇÔ )
	int				HideWeapon;				//¹«±â ¼û±è
	int				MotionLoop;				//¹Ýº¹ ¸ð¼Ç È½¼ö
	int				MotionLoopSpeed;		//¹Ýº¹ ¸ð¼Ç¿ë ÇÁ·¹ÀÓ ¼Óµµ
	int				MotionEvent;			//¸ð¼Ç ÀÌº¥Æ® ÇÃ·¢
	int				WeaponEffect;			//¹«±â¿ë ¼Ó¼º ÀÌÆåÆ®
	DWORD			dwWeaponEffectTime;		//¹«±â¿ë ¼Ó¼º ÀÌÆåÆ® Ä«¿îÅÍ
	int				EnchantEffect_Point;	//½ºÅ³ EnchantWeapon ÀÇ ½ºÅ³ Æ÷ÀÎÆ®

	rsPLAYINFO		*lpCompulsionPlayInfo;	//½Ã¼±²ø±â Ä³¸¯ÅÍ
	DWORD			dwCompulsionTime;		//½Ã¼±²ø±â ½Ã°£

	DWORD			dwAssassinEyeTime;		//¾î¼¼½Å¾ÆÀÌ Àû¿ë ¸ó½ºÅÍ
	int 			AssassinEyeParam;		//¾î¼¼½Å¾ÆÀÌ Àû¿ë ¸ó½ºÅÍ


	////////////////////// Å¬·£ //////////////////////////
	int				ClanInfoNum;			//Å¬·£ Á¤º¸ ¹øÈ£
	DWORD			dwClanInfoTime;			//Å¬·£ Á¤º¸ ¹ÞÀº ½Ã°£
	DWORD			dwClanManageBit;		//Å¬·£ °ü¸® Ç¥½Ã
	int				Clan_CastleMasterFlag;	//¼ºÁÖ Å¬·£¿ø ÀÌ´Ù!

	//È®Àå¿ë
	void			*lpExt1;				//(¼­¹ö) ÇÊµå ±¸Á¶Ã¼ ¿¬°á Æ÷ÀÎÅÍ 
	void			*lpExt2;
	void			*lpExt3;
	void			*lpExt4;

	POINT3D			HoSkillPos;				//½ºÅ³ °ü·Ã ÁÂÇ¥
	int				HoSkillCode;			//°ü·Ã ½ºÅ³ ÄÚµå
	int				HoSkillMode;			//½ºÅ³ ¼ÒÈ¯¼ö Çàµ¿¸ðµå

	int				PartyFlag;				//ÆÄÆ¼ ÇÃ·¢
	int				PartyParam;				//ÆÄÆ¼ ÆÄ¶ó¸ÞÅÍ

	int				PlaySlowCount;		//´À·ÁÁö´Â °ø°Ý ¹ÞÀ½
	int				PlaySlowSpeed;		//´À·ÁÁö´Â ¼Óµµ
	int				PlayStunCount;		//Áß¸³ »óÅÂ Ä«¿îÅÍ
	int				PlayStopCount;		//¿òÁ÷ÀÓÀÌ ¸ØÃã Ä«¿îÅÍ
	short			PlayHolyMind[2];	//µ¥¹ÌÁö ¾àÈ­ ±â´É ( [0] °¨¼ÒµÈ µ¥¹ÌÁö% [1] À¯Áö½Ã°£ )
	short			PlayHolyPower[2];	//µ¥¹ÌÁö °­È­ ±â´É ( [0] °­È­µÈ µ¥¹ÌÁö% [1] À¯Áö½Ã°£ )
	short			PlayPoison[2];		//µ¶¿¡ °ø°Ý ( [0] °¨¼ÒµÉ µ¥¹ÌÁö [1] ÇØµ¶½Ã°£ )
	short			PlayHolyIncantation[2];	//¸ó½ºÅÍ ÇöÈ¤ ½ºÅ³ ( [0] ÀÓ½Ã [1] À¯Áö½Ã°£ )
	int				PlayVanish;			//¹è´Ï½¬ ½ºÅ³Àû¿ë
	int				PlayCurseQuest;		//ÀúÁÖ Äù½ºÆ® ¸ðµå
	int				PlayVague;			//º¸±× ½ºÅ³Àû¿ë
	int				PlayDistortion;		//µð½ºÅä¼Ç (¸ó½ºÅÍ ¿Ö°î)
	int				PlayInvincible;		//¹«Àû ¾ÆÀÌÅÛ
	int				PlayCursePvP;		// reipvp xxstr
	int			    PlayCurseTopLVL;	// toplvl xxstr
	int				PlayEvasionMastery;
	int				PlayerPvPMode;
	BOOL			CheckFlagIce;


	int				DispPoison;

	DWORD			dwForceOfNatureTime;
	DWORD			dwHallOfValhallaTime;


	int				AttackSkillRange;

	int				EventAttackCount;
	DWORD			dwEventAttackParam;

	short			sAddColorEffect[4];
	int				DispLifeBar;

	DWORD			dwUpdateCharInfoTime;
	DWORD			dwLastRecvAttackTime;

	int				TryAttackCount;

	DWORD			dwForceOrbCode;
	DWORD			dwForceOrbTime;

	DWORD			dwLifeBoosterCode;
	DWORD			dwLifeBoosterTime;
	DWORD			dwManaBoosterCode;
	DWORD			dwManaBoosterTime;
	DWORD			dwStaminaBoosterCode;
	DWORD			dwStaminaBoosterTime;

	DWORD			dwSiegeItem_ScrollCode_Eternal;
	DWORD			dwSiegeItem_ScrollTime_Eternal;			
	DWORD			dwSiegeItem_ScrollCode_Fatal;
	DWORD			dwSiegeItem_ScrollTime_Fatal;		
	DWORD			dwSiegeItem_ScrollCode_Avert;
	DWORD			dwSiegeItem_ScrollTime_Avert;	

	DWORD			dwSkillDelayCode;
	DWORD			dwSkillDelayTime;

	int				PkMode_CharState; //(smCHAR_STATE_NPC / ENEMY / USER)
	int				DontMoveFlag;
	DWORD			dwClanCode;

	int				UseObject_VirtualLife;
	short			sObject_VirtualLife[2];
	short			sObject_DisplayLife[2];

	short			sBlessCastle_Damage[2];		//0-Damage 1-Killing Count


	ATTACK_DAMAGE_LIST	*lpAttackDamageList;
	ATTACK_DAMAGE_LIST* lpAttackDamageList_BlessCastle;

	float DissolveAmount = 0.f;
	int iMountID = 0;
	class CMountHandler* pMount = nullptr;

	smCHAR();
	~smCHAR();
	void Init();
	int Close();

	int SetPosi( int x, int y, int z , int angX, int angY, int angZ );

	int SetTargetPosi( int x, int z );
	int SetTargetPosi2( int x, int z );

	int GetAttackPoint( int *nX, int *nY, int *nZ );
	int GetToolBipPoint( smCHARTOOL *ChrTool, int *nX, int *nY, int *nZ );
	int SetNextTarget( int x, int y, int z );

	int MoveAngle( int step );
	int MoveAngle2( int step );

	int ChangeMoveMode();

	int StartHandEffect(int Mode);

	int SetAction( int nAction );
	int ChangeMotion( int Motion , int DpMode=0 );

	int AutoChangeTalkMotion( int TalkModeType );

	int FindActionFromFrame( int sframe );

	int SetDinaPattern( smDPAT	*lpDPat );
	int SetDinaPattern2( smDPAT	*lpDPat );

	int SetTool( DWORD dwItemCode , int hvPosi  );

	int SetPattern( smPAT3D *pat );
	int SetPattern2( smPAT3D *pat );

	int SavePlayBuff();

	int SavePlayBuff2();

	int Main();
	int Draw(bool DrawCharacterTarget = false);
	int VirtualDraw();

	int ShootingMain();
	
	int	ChargingSkill();
	int EventAttack( int Flag=0 );

	int CheckShootingTest(smCHAR *lpChrTarget);

	int CheckShootingTest_LastAttackTime( smCHAR *lpChar );

	int	SetSmoking( int level );

	SIceFootInfo	m_IceFootInfo[ ICE_FOOT_COUNT_MAX ];

	void SetIceFoot( int TypeNum, DWORD dwPlayTime, int x, int z );
	int  DrawIceFoot(void);	

	int DrawShadow();
	int DrawStateBar( int x, int y );
	int DrawStateBar2( int x, int y );
	int DrawChargingBar( int x, int y , int Charging , int ChargingMax );

	int DrawMotionBlurTool( smCHARTOOL *ChrTool );
	int DrawMotionBlur();

	int SetLoadPattern( char *szName );

	int MakePlayBuffFromPosi( int px, int py, int pz , int anX,int anY, int anZ , int act );
	int MakePlayBuffFromRecvData( smPLAYBUFF *StartBuff , smPLAYBUFF *EndBuff , int len );

	int MakeTransPlayData( char *lpTargetBuff , int SendTime , int pBuffStep=1 );

	int SetTransEffectItems(smEFFECT_ITEM *lpEffectItem);
	int SetTransEffectWeapon(smEFFECT_ITEM *lpEffectItem);

	int TransDelayCheck();
	int SetDistSendCnt( smCHAR *player );

	int SetMotionFromCode( DWORD MotionCode );

	int FindMotionCountFromCode( DWORD MotionCode );

	int FormSetPlayerInfo();

	int SetTransPlayerInfo( smTRNAS_PLAYERINFO *lpTransPlayerInfo );

	int SetTransPlayerInfoQuick( smTRNAS_PLAYERINFO_QUICK *lpTransPlayerInfo );

	DWORD GetAttackTrans_XorCode();
	int ResetAttackTrans();
	int XorAttackTrans();

	int PlayAttack( int EventFlag=0 );

	int RecvPlayData2( char *lpData );
	int RecvPlayData( smTHREADSOCK *RecvData );
	int SendPlayData( smCHAR *player );


	int SendCharInfo( smWINSOCK *lpsmsock );

	int SendPlayDataSock( smWINSOCK *lpsmsock , char *lpTransBuff , int ex, int ey, int ez );

	int SendTransAttack( smCHAR *lpChar , smWINSOCK *lpsmsock , int AttackCode , int Add_Damage=0 , int Resistance=0 );

	int PlayAttackFromPosi( int ex, int ey, int ez, int Dist ,int attack=TRUE );

	int BeginSkill( int SkilCode , int Level , smCHAR *lpTarChar , int x, int y, int z );
	int EventSkill();
	int BeginSkill_Monster();
	int EventSkill_Monster();
	int BeginAttack_Monster();
	int EndSkill();

};

/*
#define CHRMOTION_STAND		0
#define CHRMOTION_WALK		1
#define CHRMOTION_RUN		2
#define CHRMOTION_JUMP		3
#define CHRMOTION_ATTACK	4
*/

#define CHRMOTION_EXT		10

#define hvPOSI_RHAND	0x0001
#define hvPOSI_LHAND	0x0002


#define CHRMOTION_STATE_STAND		0x40
#define CHRMOTION_STATE_WALK		0x50
#define CHRMOTION_STATE_RUN			0x60
#define CHRMOTION_STATE_FALLDOWN	0x80

#define CHRMOTION_STATE_ATTACK		0x100
#define CHRMOTION_STATE_DAMAGE		0x110
#define CHRMOTION_STATE_DEAD		0x120
#define CHRMOTION_STATE_SOMETIME	0x130
#define CHRMOTION_STATE_EAT			0x140
#define CHRMOTION_STATE_SKILL		0x150

#define CHRMOTION_STATE_FALLSTAND	0x170
#define CHRMOTION_STATE_FALLDAMAGE	0x180

#define CHRMOTION_STATE_RESTART		0x200
#define CHRMOTION_STATE_WARP		0x210

#define CHRMOTION_STATE_YAHOO		0x220

#define CHRMOTION_STATE_HAMMER		0x300

#define CHRMOTION_STATE_TALK_AR			0x400
#define CHRMOTION_STATE_TALK_E			0x410
#define CHRMOTION_STATE_TALK_OH			0x420
#define CHRMOTION_STATE_TALK_EYE		0x430
#define CHRMOTION_STATE_TALK_BLANK		0
#define CHRMOTION_STATE_TALK_SMILE		0x440
#define CHRMOTION_STATE_TALK_GRUMBLE	0x450
#define CHRMOTION_STATE_TALK_SORROW		0x460
#define CHRMOTION_STATE_TALK_STARTLED	0x470
#define CHRMOTION_STATE_TALK_NATURE		0x480
#define CHRMOTION_STATE_TALK_SPECIAL	0x490

int smCHAR_SetCameraPosi( int x,int y,int z, int angX,int angY,int angZ );

extern CRITICAL_SECTION	cLoadSection;

int smDPAT_Delete( smDPAT *dPat );
int AnimObjectTree( smOBJ3D *tObj , int frame , int ax, int ay, int az );

int TestSetNewText( char *str ) ;

int InitMotionBlur();

int GetSinItemNumFromCode( DWORD CODE );

int InitPatterns();
int ClosePatterns();
int SetLoadPattern( smCHAR *smChar , char *szName , POINT3D *Posi , POINT3D *Angle );
int SetLoadPattern( smCHAR *smChar , char *szName , char *szName2 , POINT3D *Posi , POINT3D *Angle );

int	AddLoaderPattern( smCHAR *lpChar , char *szName , char *szName2=0 );
int AddLoaderStage( smSTAGE3D *lpStage , char *szName );
int AnimObjectTree( smOBJ3D *tObj , int frame , int ax, int ay, int az );

class scITEM {

public:
	DWORD	Head;

	int		DisplayFlag;

	int		Flag;
	int		pX,pY,pZ;

	char	szModelName[64];
	char	szName[64];

	DWORD   iClass;

	void	*lpStgArea;

	smPAT3D	*Pattern;
	smDPAT  *lpDinaPattern;

	int		PatLoading;

	int		ItemCode;

	DWORD	dwLastTransTime;

	int		ServerCode;	

	POINT3D	RendPoint;


	POINT3D Posi;
	POINT3D Angle;


	scITEM();
	~scITEM();

	void Init();
	int Close();

	int SetPosi( int x, int y, int z );
	int SetTransItem( TRANS_ITEM *lpTransItem );

	int SetPattern( smPAT3D *pat );
	int SetPosi( int x, int y, int z , int angX, int angY, int angZ );
	int Draw();

};

int	AddLoaderItem( scITEM *lpItem , char *szName );
DWORD GetSpeedSum( char *szName );

extern smPATTERN smBipPattern;
extern smPATTERN smPattern;


int	Ptect_IncAttackCount( int Flag );
DWORD Ptect_GetAttackCount();

extern int	SkillChargingFlag;

extern TRANS_SKIL_ATTACKDATA	Trans_SplashCharList;

extern BOOL isEnemy;

extern int smCHAR_CameraX;
extern int smCHAR_CameraY;
extern int smCHAR_CameraZ;

extern int CastleBattleZone_LineZ;
extern int CastleBattleZone_DoorLineZ;
extern int CastleBattleZone_DoorLineX[2];
extern int CastleBattleZone_BridgeZ;
extern int CastleSoulFountain_Area[4];

int UpdateCastleMasterClan( DWORD dwMasterClan );
int SetClanMaster_Player( DWORD dwMasterClan );