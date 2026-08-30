#pragma once
#include "character.h"

//Level Maximo - Nivel Maximo  DRACKO

// Nível máximo configurável (lido do arquivo Base.ini)
extern int g_LevelFinal;
#define	CHAR_LEVEL_MAX	g_LevelFinal // Nível maximo aqui

#define sinNPC_SHOP				0x00000001
#define sinNPC_MIX				0x00000002
#define sinNPC_AGING			0x00000004
#define sinNPC_WARE				0x00000008
#define sinNPC_SKILL			0x00000010
#define sinNPC_FORCE			0x00000020
#define sinNPC_SMELTING			0x00000030
#define sinNPC_MANUFACTURE		0x00000040
#define sinNPC_MIXTURE_RESET	0x00000080


extern DWORD	dwFuncChkTime;

DWORD Check_CodeSafe(DWORD dwStartFunc);
DWORD Check_CodeSafe2();
DWORD	GetSafeCode();
DWORD	GetSafeCode2();
int Code_VRamBuffOpen();
int	Mix_CodeVram();

extern char	szLastWhisperName[32];
extern int	LastWhisperLen;
extern int	PlayerArmor;
extern INT64 ExpLevelTable[];

extern int	PlayUsed_Resurrection;

int SetPlayerArmor(int armor);
char* GetFaceHairSkin(smCHAR* lpChar);

//ÀÎº¥Åä¸®¿Í °Å·¡Ã¢¿¡¼­ ¹°¾àÀ» Ã£¾î °¹¼ö ±â·Ï
int GetInvenPotionCount( sCHK_POTION_COUNT *ChkPotion );
//¹°¾à °¹¼ö »õ·Î ¼³Á¤
int	ResetPotion();
//¹°¾à °¹¼ö »õ·Î ¼³Á¤
int	ResetPotion2();
//¹°¾à°¹¼ö ºñ±³
int	ComparePotion();
//¹°¾à°¹¼ö ±â·Ï ¹İÀü
int	XorCheckPotion( sCHK_POTION_COUNT *ChkPotion );
///////////////////////////////////////////////////////////////////////

//ÆÄÆ¼¿ø ±Ó¸» º¸³»±â
int	WhisperPartyPlayer( char *szName );
//Å¬·£¿ø Ã¤ÆÃ º¸³»±â
int	SetClanChatting();
//Å¬·£ Ã¤ÆÃ ¸ğµå ¼³Á¤
int IsClanChatting();

//°æÇèÄ¡ Ãß°¡
int AddExp( int Exp );
//´ÙÀ½ °æÇèÄ¡ ±¸ÇÏ±â
INT64 GetNextExp( int Level );

INT64 GetExpFromLevel(int iLevel);

//·¹º§°ú °æÇèÄ¡°¡ ¸Â´ÂÁö È®ÀÎ
int	CheckLevelExp( int Level , INT64 Exp );
//°æÇèÄ¡·Î ·¹º§ Ãß»ê
int	GetLevelFromExp( INT64 Exp );
//64ºñÆ® °æÇèÄ¡ ¼³Á¤
int	SetExp64( smCHAR_INFO *smCharInfo , INT64 Exp64 );
//64ºñÆ® °æÇèÄ¡ ¹İÈ¯
INT64 GetExp64( smCHAR_INFO *smCharInfo );
//64ºñÆ® °æÇèÄ¡ ¹İÈ¯
INT64 GetXorExp64( smCHAR_INFO *smCharInfo );
//°æÇèÄ¡ µ¥ÀÌÅ¸¸¦ ¹İÀü
INT64 CodeXorCharInfo_Exp();

//ÇÊµå Àç½ÃÀÛ °¡´ÉÇÑÁö È®ÀÎ
int CheckFieldContExp();


//±ÍÈ¯¾ÆÀÌÅÛ »ç¿ë
int	ActionEtherCore( sITEM	*lpItem );

//Á×¾î¼­ °æÇèÄ¡ ±ïÀÓ
int DeadPlayerExp( smCHAR *lpChar );
//ÇÊµå¿¡¼­ Àç½ÃÀÛ Ä³¸¯ÅÍ °æÇèÄ¡ Ãß°¡ ±ïÀÓ
int ContGameExp( smCHAR *lpChar );
//¹æ¾îÀ² °è»ê
int GetPlayDefence( smCHAR_INFO *lpAttackCharInfo , smCHAR_INFO *lpDefenceCharInfo );

//³¯¾¾
int DrawWeather( int ax );
int InitWeatherSystem();
//·ÎµùÁß È­¸é Ç¥½Ã
int OpenDispLoading();
//·Îµù Ç¥½ÃÁ¾·á
int CloseDispLoading();
//³ªÀÌ³ª¹Í ÆĞÅÏ »èÁ¦ Ãß°¡
int AddRemoveDynPattern( smDPAT *lpDinaPattern );

//·Îµù ·¥ÇÁ Á¦°Å
int CloseLoadingLamp();
//·Îµù·¥ÇÁ
int InitLoadingLamp( HWND hWnd );

//ÀÚÁÖ»ç¿ëÇÏ´Â ¸ğµ¨µé ·Îµå
int LoadOftenModels();

//ÀÚÁÖ¾²´Â ¸Ş½Ã ·Îµå
int	LoadOftenMeshPattern( char *szFileName );
//ÀÚÁÖ¾²´Â °©¿Ê ¹Ì¸® ·Îµù
int LoadOftenArmor();
//ÀÚÁÖ¾²´Â ¾ÆÀÌÅÛ ¹Ì¸® ·Îµù
int LoadOftenItem();
//NPC ¹Ì¸® ·Îµù
int LoadOftenNpc();
//¾ó±¼ÆÄÀÏ ¹Ì¸® ·Îµù
int LoadOftenFace();
//ÀÚÁÖ¾µ ¸Ş½Ã ÅØ½ºÃÄ ½º¿ÒÃ½Å©
int CheckOftenMeshTextureSwap();
//¸ğµ¨ ½ºÅ² º¯°æ
int	ChangeModelSkin( smCHAR *lpChar , int ComicArmor , int ComicHeadNum , char *szArmor , char *szHead );
//¸Ó¸®¸ğ¾ç ¹Ù²ã¹ö¸®±â
int ChangeHairModel( int HairCode );


//ÇÊµå ¸Ê ÃÊ±âÈ­
int InitFieldMap();

//ÇÊµå ¸Ê ±×¸®±â
int MainFieldMap();
//ÇÊµå ¸Ê ±×¸®±â
int DrawFieldMap();

int GetFieldMapCursor();		//¹Ì´Ï¸Ê À§Ä¡¿¡ ¸¶¿ì½º À¯¹«

int psSetDefaultRenderState();


//µ¥ÀÌÅ¸ ¾ĞÃà ( Z/NZ ¹æ½Ä )
int EecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int size );
//¾ĞÃàµ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData );
//¾ĞÃàµ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int dCount );
//µ¥ÀÌÅ¸ ¾ĞÃà ( Z/NZ ¹æ½Ä )
int EecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int size , int DestSize );

//¾ĞÃàµ¥ÀÌÅ¸ ¼­¹ö ¾ÆÀÌÅÛ ÇØµ¶¿ë ( Z/NZ ¹æ½Ä )
int DecodeCompress_ItemPotion( rsPLAYINFO *lpPlayInfo , BYTE *lpSrcData , BYTE *lpDestData , int deCount , void *lpTransRecdata );


//´Ù¸¥ Ä³¸¯ÅÍ¿ÍÀÇ À§Ä¡ °ãÄ¡´ÂÁö È®ÀÎ
smCHAR *CheckOtherPlayPosi( int x, int y, int z );

//¾ÆÀÌÅÛ ÀÎÁõ ¹Ş±â
int ReformItem( sITEMINFO *lpItemInfo );
//¾ÆÀÌÅÛ ÀÎÁõ ¹Ş±â ( »ı¼º½Ã°£ º¸Á¤ )
int ReformItem_AdjustTime( sITEMINFO *lpItemInfo , int TimeAdjust );

//¾ÆÀÌÅÛ ÀÎÁõ È®ÀÎ
int CheckItemForm( sITEMINFO *lpItemInfo );
//¾ÆÀÌÅÛ 2°³¸¦ ºñ±³ÇÑ´Ù
int CompareItems( sITEMINFO *lpItemInfo1 , sITEMINFO *lpItemInfo2 );
//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ¸¸ ¹Ş±â
int	rsReformItem_Server( sITEMINFO *lpItemInfo );

//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckCharForm();
//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Ş±â
int ReformCharForm();
//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Ş±â
int ReformCharForm( smCHAR_INFO *lpCharInfo );
//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckCharForm( smCHAR_INFO *lpCharInfo );
//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ ¹Ş±â
int ReformTrade( void *lpTrade );
//Æ®·¹ÀÌµåÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckTrade( void *lpTrade );

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Ş±â
int ReformMixItem( sITEMINFO *lpItemInfo );
//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ È®ÀÎ
int CheckMixItem( sITEMINFO *lpItemInfo );

//Èí¼ö·Â º¸Á¤ÇÏ¿© ±¸ÇÏ±â
float GetItemAbsorb( sITEMINFO *lpItemInfo );

//ÆÄÆ¼ °¡ÀÔ¿©ºÎ È®ÀÎ
int	RequestPartyPlayers( smCHAR	*lpChar );

int psSetDefaultRenderState();

//Ä³¸¯ÅÍ Á¤º¸¸¦ Ä³½Ãµ¥ÀÌÅ¸·Î ºÎÅÍ ¾ò´Â´Ù
smTRNAS_PLAYERINFO	*GetCharInfoCache( char *lpData );
//Ä³¸¯ÅÍ Á¤º¸¸¦ Ä³½Ãµ¥ÀÌÅ¸¿¡ ÀúÀå
int SaveCharInfoCache( smTRNAS_PLAYERINFO *lpTransPlayInfo );

extern smCHAR	chrPartyPlayer;
//ÆÄÆ¼ ÇÃ·¹ÀÌ¾î ¾ó±¼ ¿ÀÇÂ
int OpenPartyPlayer( char *szName , char *szModelFile1 , char *szModelFile2 );


//°ø°İ ºñÀ² Ã½Å©
int	CheckPlayAttackCount();
//°ø°İ ¹ŞÀº ¸ğ¼Ç Ä«¿îÅÍ
int AttackMotionCountUp();
//°ø°İ ¹ŞÀº ÆĞÅ¶ Ä«¿îÅÍ
int	AttackRecvCountUp();
//µî·ÏµÈ ºÒ·® ¾ÆÀÌÅÛ °Ë»ç
int	CheckCopyItem( sITEMINFO *lpItem );
//Áı¾÷ º¯°æ ¾ó±¼ º¯È­
int ChangeJobFace();


//¿¹¾à ÀÌÆåÆ® ÃÊ±âÈ­
int InitTimeEffect();
//ÀÌÆåÆ® ¿¹¾à Ãß°¡
int AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z );
int AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z , smCHAR *lpChar );
//¿¹¾à ÀÌÆåÆ® ½ÇÇà
int TimeEffectMain();


//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ¼±ÅÃ
smCHAR *SelectRotPlayer( smCHAR *lpCurrentChar );
//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ Á¾·á
int	CloseRotPlayer();
//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ÃÊ±âÈ­
smCHAR *InitRotPlayer();

//PLAYSUB Æã¼Ç·çÆ¾ Ã½Å©
int Check_PlaySubReleaseFunc();

//¾ó±¼ º¯°æÇÑ°Å Àû¿ë
int SetJobFace( char *szModelName );
//¸ğµ¨ÀÌ ¸Â´ÂÁö È®ÀÎ
int	CheckUserModel( smCHAR_INFO *lpCharInfo );

//ÀÌº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
int	ResetInvenItemCode();
//ÀÌº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå¿¡¼­ Æ¯Á¤ ¾ÆÀÌÅÛ Á¦°Å
int SubInvenItemCode( sITEMINFO *lpItem );
//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå È®ÀÎ
int	CheckInvenItemCode();

//¸ğµâ Ã½Å© 2
void CheckPlaySubFunc2();



//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
int	ResetEnergyGraph( int Num );
//¿¡³ÊÁö ±×·¡ÇÁ ¿À·ù Ã½Å©
int CheckEnergyGraphError();
//¿¡³ÊÁö ±×·¡ÇÁÀÇ ¿À·ù Ã½Å©
int CheckEnergyGraph();

//ÀÎº¥Åä¸®ÀÇ ÁöÁ¤ÇÑ ¾ÆÀÌÅÛÀ» Ã£¾Æ °Ë»öÇÏ¿© Ã£´Â´Ù
sITEMINFO *FindInvenItem( DWORD dwItemCode , DWORD dwHead , DWORD dwChkSum );
//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ º¹»ç ¿À·ù
int SendInvenItemError( DWORD dwKind , DWORD dwItemCode , DWORD dwHead , DWORD dwChkSum );
//ÀÎº¥Åä¸®ÀÇ ÀåÂø ¾ÆÀÌÅÛ Á¤º¸ ¼­¹ö¿¡ º¸³»±â
int SendUsingItemListToServer();
//ÀÎº¥Åä¸®ÀÇ ÀåÂø ¾ÆÀÌÅÛ Á¤º¸ ¼­¹ö¿¡ º¸³»±â
int SendUsingItemListToServer2();


//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
int	ResetInvenMoney();
//ÀÎº¥Åä¸® °Ë»ç¿ëµ· Ãß°¡
int	AddInvenMoney( int Money );
//ÀÎº¥Åä¸® µ· °Ë»ç
int	CheckInvenMoney();


//°ø°İ¼Óµµ¸¦ ÇÁ·¹ÀÓ¼Óµµ·Î º¯È¯
int GetAttackSpeedMainFrame( int AttackSpeed );
int	GetAttackSpeedFrame( int AttackSpeed , int AddSpeed );
int	GetAttackSpeedFrame( int AttackSpeed );


extern int DispLoading;			//·Îµù »óÅÂ Ç¥½Ã À¯¹«
extern int MessageLoading;		//·ÎµùÁß ¸Ş¼¼Áö Ã³¸®

//¿¡Æ¼¸£ÄÚ¾î Æ÷¼Ç ·Î±× ÄÚµå
#define POTION_ETHERCORE		0x40

extern int	CodeXorExp;					//°æÇèÄ¡ ¼û±èÄÚµå
extern int	CodeXorExp_High;			//°æÇèÄ¡ ¼û±èÄÚµå
extern int	CodeXorLife;				//»ı¸í·Â ¼û±èÄÚµå
extern int	CodeXorMana;				//±â·Â ¼û±èÄÚµå
extern int	CodeXorStamina;				//±Ù·Â ¼û±èÄÚµå
extern int	CodeXorMoney;				//µ· ¼û±èÄÚµå


//»ö»óÀ¸·Î ¾ÆÀÌÅÛÀÇ »óÅÂ¸¦ ÆÇº°ÇÑ´Ù
int GetItemKindFromBliankColor( smCHARTOOL	*lpCharTool );


extern char	szSOD_String[64];
extern BYTE	bSOD_StringColor[4];
extern int	SOD_StringCount;

extern sSOD_SCORE	sSodScore;
extern int		SoDGateFlag;

int	DisplaySodScore();	//SodÁ¡¼ö Ç¥½Ã ÇÔ¼ö
int	SodScoreMain();		//SoD ¿¬Ãâ
int	Sod_SodEffect( int Round );			//SoD ¶ó¿îµå ÀÌÆåÆ®


