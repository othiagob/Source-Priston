#include "globals.h"

#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "avictrl.h"
#include "playmain.h"
#include "srcsound\\dxwav.h"
#include "fileread.h"

#include "particle.h"
#include "netplay.h"

#include "sinbaram\\sinlinkheader.h"
#include "hobaram\\holinkheader.h"

#include "field.h"
#include "effectsnd.h"

#include "record.h"
#include "playsub.h"
#include "skillsub.h"
#include "damage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "cracker.h"
#include "damage.h"
#include "AreaServer.h"
#include "srcServer\onserver.h"
#include "Engine\\Directx\\DXFont.h"
#include "Engine\\Directx\\DXSelectGlow.h"
#include "Engine\\Directx\\DXGraphicEngine.h"
#include "Montarias\\CMountHandler.h"
#include "TitleBox/TitleBox.h"

extern int WinSizeX;
extern int WinSizeY;
extern HWND hwnd;
extern int MouseButton[3];
// top lvl xxstr
extern smCHAR *ChrTopLVL;
extern int ObjectTopLVL;

smSTAGE3D		*smStage = 0;
smSTAGE3D		*smGameStage[2] = { 0,0 };
sFIELD			*StageField[2] = { 0,0 };


rsRECORD_DBASE	rsRecorder;					//¼­¹ö¿¡ °ÔÀÓµ¥ÀÌÅ¸ ±â·ÏÀåÄ¡

char szGameStageName[2][64];
DWORD			dwPlayTime = 0;					//ÇöÀçÀÇ À©µµ¿ì ½Ã°£ ( ÇÃ·¹ÀÌ ¿ë )
DWORD			dwMemError = 8080;				//¸Þ¸ð¸® ¿¡·¯ È®ÀÎ ÄÚµå

int	smStageCount = 0;
int SkipNextField = 0;				//´ÙÀ½ ÇÊµå·Î ³Ñ¾î°¨

int	NightDayTime = 0;					//¹ã³· ±¸ºÐ

int invPlayerArmor = 0;

DWORD	dwBattleTime = 0;				//ÃÖ±Ù ÀüÅõ ½Ã°£
DWORD	dwBattleQuitTime = 0;			//ÀüÅõÁß Á¾·á ÇÏ·Á ÇÒ¶§
DWORD	dwCloseBoxTime = 0;				//ÀÛ¾÷Ã¢ Á¾·á ¸Þ¼¼Áö

int		RestartPlayCount = 0;					//Àç½ÃÀÛ ¹«Àû ½ÃÀÛ Ä«¿îÆ®

DWORD SkillFalconPoint;					//ÆÈÄÜ ½ºÅ³ Æ÷ÀÎÆ® ÀúÀå¿ë

int InvArmorFileCode[] =
{ 0 ,9,1, 5 };

int	DarkLevel = 0;					//¾îµÒ °ª
int	BackColor_R = 0;				//¹è°æ»ö R
int	BackColor_G = 0;				//¹è°æ»ö G
int	BackColor_B = 0;				//¹è°æ»ö B

int	BackColor_mR = 0;				//¹è°æ»ö º¯°æ°ª R
int	BackColor_mG = 0;				//¹è°æ»ö º¯°æ°ª G
int	BackColor_mB = 0;				//¹è°æ»ö º¯°æ°ª B

int	DarkLevel_Fix = 0;				//¾îµÒ °ª °íÁ¤

int	DarkLightRange = 180 * fONE;		//¾îµÎ¿ï¶§ Á¶¸í ¹üÀ§

DWORD	dwGameWorldTime = 0;			//°ÔÀÓÀÇ ¿ùµå½Ã°£
DWORD	dwGameHour = 12;					//°ÔÀÓÀÇ ½Ã
DWORD	dwGameMin = 0;					//°ÔÀÓÀÇ ºÐ
DWORD	dwGameTimeOffset = 0;

int DarkLight = 0;

//³¯¾¾ °ü·Ã
int WeatherMode = 0;
int Thunder = 0;

//PK¸ðµå
int	PkMode = 0;

int DispApp_SkillPoint = 0;			//¼³Á¤µÈ ½Ã°£Àû¿ë ½ºÅ³ Æ÷ÀÎÆ®

int rcnt2;

int modelX = 255;
int modelZ = 512;
int wflag = 0;
int wrad = 0;

DWORD	UseEtherCoreCode = 0;			//¿¡Æ¼¸£ ÄÚ¾î »ç¿ë

int	RecordFailCount = 0;				//ÀúÀå ½ÇÇÇ Ä«¿îÅÍ

//°ø°Ý ¹Þ¾ÒÀ»¶§ È¸ÇÇ Ä«¿îÅÍ
int	Record_BlockCount = 0;
int	Record_DefenceCount = 0;
int Record_RecvDamageCount = 0;
int Record_TotalRecvDamageCount = 0;


//¿ÀÅä ¸¶¿ì½º °¨Áö¿ë
DWORD	AutoMouse_LastCheckTime = 0;
DWORD	AutoMouse_StartCheckTime = 0;
int		AutoMouse_WM_Count = 0;
int 	AutoMouse_Distance = 0;
int		AutoMouse_Attack_Count = 0;

//È­»ì
//smPAT3D *PatTest;

//È­»ì
smPAT3D *PatArrow;
//¹æÆÐ ÀÌÆåÆ®
smPAT3D *PatEffectShield = 0;
//½ºÆÄÅ©½¯µå ÀÌÆåÆ®
smPAT3D *PatSparkShield = 0;
//µð¹ÙÀÎÀÎÇÒ·¹ÀÌ¼Ç
smPAT3D *PatDivineShield = 0;
//°¡µé¸® ½¯µå
smPAT3D *PatGodlyShield = 0;
//ºí·¹½ºÄ³½½ ¿Õ°ü
smPAT3D *PatCrown = 0;

//±×¸²ÀÚ
smPAT3D *PatShadow[SHADOW_PAT_MAX] = { 0,0,0,0 };

//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
smPAT3D *PatIceFoot[ICE_FOOT_PAT_MAX];
//######################################################################################

int	Moving_DblClick = 0;		//¸¶¿ì½º ÀÌµ¿ Ã½Å© ´õºí Å¬¸¯
POINT3D	MousePos_DblClick;

POINT3D	TracePos_DblClick;
int TraceMode_DblClick = 0;

int	ActionGameMode = 0;	//¾×¼Ç°ÔÀÓ ¸ðµå


//smCHAR	chrPlayer;
smCHAR		*lpCurPlayer = 0;

DWORD	dwRotPlayDelayTime = 0;			//ÁÖÀÎ°ø Æ÷ÀÎÅÍ º¯°æ µô·¹ÀÌ

/*
//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ¼±ÅÃ
smCHAR *SelectRotPlayer( smCHAR *lpCurrentChar );
//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ Á¾·á
int	CloseRotPlayer();
//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ÃÊ±âÈ­
smCHAR *InitRotPlayer();

lpCurPlayer = InitRotPlayer();

*/


scITEM	scItems[DISP_ITEM_MAX];			//ITEM

smCHAR	chrOtherPlayer[OTHER_PLAYER_MAX];

smCHAR_INFO	smCharInfo_Back;			//NPC ¼³Ä¡½Ã ±âº» µ¥ÀÌÅ¸ ÀÓ½Ã ÀúÀå


int	MatEachBox;				//¾ó±¼ È®´ë Å×µÎ¸® ±×¸² ¸ÞÆ®¸®¾ó
int	MatEachMenuBox;			//¸Þ´º ¹öÆ° Å×µÎ¸®
int MatEnergyBox[8];		//¿¡³ÊÁö ¸ÞÆ®¸®¾ó
int MatLight[4];				//Á¶¸í ºû ÀÌ¹ÌÁö ¸ÞÆ®¸®¾ó //glow.bmp
int iMask;
int emoticons[10];

int	MatEachTradeButton[2];	//¸Þ´º ¾ÆÀÌÅÛ ±³È¯ ¹öÆ° 
int MatMonBroodImage[9];	//¸ó½ºÅÍ Á¾Á· ÀÌ¹ÌÁö

//µ¿·á È¸º¹ ¿¹¾à°ª
smTRANS_COMMAND	sPartyPosion;
DWORD			dwPartyPosionTime;

LPDIRECT3DTEXTURE9	lpDDS_ParTradeButton[4][2];			//¸Þ´º¹× Æ®·¹ÀÌµå ¹öÆ°
LPDIRECT3DTEXTURE9	lpDDS_ParTradeToolTip[4];			//¸Þ´º¹× Æ®·¹ÀÌµå ÅøÆÁ
LPDIRECT3DTEXTURE9 Texture_Interface;
LPD3DXSPRITE Sprite_Interface;

//ºÒ²É ¾Ö´Ï¸ÞÀÌ¼Ç
char *szFlame[8] = {
	"image\\flame1.bmp",
	"image\\flame2.bmp",
	"image\\flame3.bmp",
	"image\\flame4.bmp",
	"image\\flame5.bmp",
	"image\\flame6.bmp",
	"image\\flame7.bmp",
	"image\\flame8.bmp"
};

char *szShine[] = {
	"image\\shine0.bmp",
	"image\\shine2.bmp",
	"image\\shine4.bmp",
	"image\\shine6.bmp",
	"image\\shine8.bmp",
	""
};

char *szBlood1[] = {
	"image\\blood\\b1_001.tga",
	"image\\blood\\b1_002.tga",
	"image\\blood\\b1_003.tga",
	"image\\blood\\b1_004.tga"
};

char *szBlood2[] = {
	"image\\blood\\b2_001.tga",
	"image\\blood\\b2_002.tga",
	"image\\blood\\b2_003.tga",
	"image\\blood\\b2_004.tga"
};

char *szShadowImageFile = "image\\bshadow.bmp";

char *g_IceFootImageFileBuff[ICE_FOOT_PAT_MAX] =
{
	"image\\mk-r.tga",
	"image\\mk-l.tga",
};

extern int pX;
extern int pY;
extern int pZ;
extern int ViewAnx;
extern int any_pt;
extern int anz;

extern int ConfigUseDynamicShadows;

void RenderCharacter()
{
	if (ConfigUseDynamicShadows)
	{
		for (int i = 0; i < OTHER_PLAYER_MAX; i++)
		{
			if (chrOtherPlayer[i].Flag && chrOtherPlayer[i].DisplayAlpha == 0)
			{
				chrOtherPlayer[i].Draw();
			}
		}

		g_DrawPetSystem(pX, pY, pZ, (ViewAnx & ANGCLIP), (any_pt & ANGCLIP), (anz & ANGCLIP));
		g_PCBang_DrawPetSystem(pX, pY, pZ, (ViewAnx & ANGCLIP), (any_pt & ANGCLIP), (anz & ANGCLIP));

		if (lpCurPlayer)
			lpCurPlayer->Draw();
	}
}

void InitStageLoader()
{
	szGameStageName[0][0] = 0;
	szGameStageName[1][0] = 0;
	smGameStage[0] = 0;
	smGameStage[1] = 0;
}

smSTAGE3D *LoadStageFromField(sFIELD *lpField, sFIELD *lpSecondField)
{
	smSTAGE3D *lpstg;
	char *szStageFile;
	int cnt;
	int	Bip;
	char	szBuff[128];

	szStageFile = lpField->szName;

	if (lpField == StageField[0]) return smGameStage[0];
	if (lpField == StageField[1]) return smGameStage[1];

	lpstg = new smSTAGE3D;

	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright = smConfig.MapBright;
	lpstg->Contrast = smConfig.MapContrast;

	AddLoaderStage(lpstg, szStageFile);

	if (!lpstg->StageObject) 
	{
		wsprintf(szBuff, "Stage Loading Error ( %s )", szStageFile);
		Record_ClinetLogFile(szBuff);
		delete lpstg;
		return NULL;
	}

	for (cnt = 0; cnt < lpField->StgObjCount; cnt++) {
		Bip = lpField->GetStageObjectName(cnt, szBuff);
		if (szBuff[0]) {
			if (Bip)
				lpstg->StageObject->AddObjectFile(szBuff, szBuff);
			else
				lpstg->StageObject->AddObjectFile(szBuff);
		}
	}

	if (lpField->StgObjCount && GRAPHICDEVICE) {
		ReadTextures();
		lpstg->smMaterialGroup->CheckMatreialTextureSwap();
	}

	if (lpSecondField && lpSecondField == StageField[0]) 
	{
		if (smGameStage[1]) delete smGameStage[1];
		smGameStage[1] = lpstg;
		StageField[1] = lpField;
		
		CMINIMAPHANDLE->LoadMiniMap(1, lpField, lpstg);
	}
	else 
	{
		if (!lpSecondField || lpSecondField == StageField[1]) {
			if (smGameStage[0]) delete smGameStage[0];
			smGameStage[0] = lpstg;
			StageField[0] = lpField;

			CMINIMAPHANDLE->LoadMiniMap(0, lpField, lpstg);
		}
	}


	if (StageField[0]) lstrcpy(szGameStageName[0], StageField[0]->szName);
	if (StageField[1]) lstrcpy(szGameStageName[1], StageField[1]->szName);

	DWORD	dwTime = GetCurrentTime();

	if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime) dwLastRecvGameServerTime = dwTime;
	if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime) dwLastRecvGameServerTime2 = dwTime;
	if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime) dwLastRecvGameServerTime3 = dwTime;
	if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime) dwLastRecvGameServerTime4 = dwTime;

	if (AreaServerMode) 
	{
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->dwDeadLockTime = dwTime;
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->dwDeadLockTime = dwTime;
	}

	return lpstg;
}

smSTAGE3D *LoadStageFromField2(sFIELD *lpField, sFIELD *lpSecondField)
{
	smSTAGE3D *lpstg;
	char *szStageFile;
	int cnt;
	int Bip;
	char szBuff[128];

	szStageFile = lpField->szName;

	if (lpField == StageField[0]) return smGameStage[0];
	if (lpField == StageField[1]) return smGameStage[1];

	lpstg = new smSTAGE3D;

	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright = smConfig.MapBright;
	lpstg->Contrast = smConfig.MapContrast;

	AddLoaderStage(lpstg, szStageFile);

	if (!lpstg->StageObject) 
	{
		wsprintf(szBuff, "Stage Loading Error ( %s )", szStageFile);
		Record_ClinetLogFile(szBuff);
		delete lpstg;
		return NULL;
	}

	for (cnt = 0; cnt < lpField->StgObjCount; cnt++) {
		Bip = lpField->GetStageObjectName(cnt, szBuff);
		if (szBuff[0]) {
			if (Bip)
				lpstg->StageObject->AddObjectFile(szBuff, szBuff);
			else
				lpstg->StageObject->AddObjectFile(szBuff);
		}
	}

	if (lpField->StgObjCount && GRAPHICDEVICE) {
		ReadTextures();
		lpstg->smMaterialGroup->CheckMatreialTextureSwap();
	}



	if (lpSecondField && lpSecondField == StageField[0]) {
		if (smGameStage[1]) delete smGameStage[1];
		smGameStage[1] = lpstg;
		StageField[1] = lpField;

		CMINIMAPHANDLE->LoadMiniMap(1, lpField, lpstg);
	}
	else {
		if (!lpSecondField || lpSecondField == StageField[1]) {
			if (smGameStage[0]) delete smGameStage[0];
			smGameStage[0] = lpstg;
			StageField[0] = lpField;

			CMINIMAPHANDLE->LoadMiniMap(0, lpField, lpstg);
		}
	}

	if (StageField[0]) lstrcpy(szGameStageName[0], StageField[0]->szName);
	if (StageField[1]) lstrcpy(szGameStageName[1], StageField[1]->szName);

	DWORD	dwTime = GetCurrentTime();
	if (dwLastRecvGameServerTime && dwLastRecvGameServerTime < dwTime) dwLastRecvGameServerTime = dwTime;
	if (dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2 < dwTime) dwLastRecvGameServerTime2 = dwTime;
	if (dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3 < dwTime) dwLastRecvGameServerTime3 = dwTime;
	if (dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4 < dwTime) dwLastRecvGameServerTime4 = dwTime;

	if (AreaServerMode) {
		//Áö¿¬ ¼­¹ö µ¥µå¶ôÈ®ÀÎ ½Ã°£ º¸Á¤
		if (lpWSockServer_Area[0]) lpWSockServer_Area[0]->dwDeadLockTime = dwTime;
		if (lpWSockServer_Area[1]) lpWSockServer_Area[1]->dwDeadLockTime = dwTime;
	}

	return lpstg;
}

//¹è°æÀ» ÀÐ¾î ¿Â´Ù
smSTAGE3D *LoadStage(char *szStageFile)
{
	smSTAGE3D *lpstg;

	if (lstrcmpi(szGameStageName[0], szStageFile) == 0) return smGameStage[0];
	if (lstrcmpi(szGameStageName[1], szStageFile) == 0) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// ¶óÀÌÆ® º¤ÅÍ
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright = smConfig.MapBright;
	lpstg->Contrast = smConfig.MapContrast;

	//¹è°æÀ» »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
	AddLoaderStage(lpstg, szStageFile);

	smStageCount = (smStageCount + 1) & 1;

	if (smGameStage[smStageCount]) delete smGameStage[smStageCount];

	smGameStage[smStageCount] = lpstg;
	//ReadTextures();
	return lpstg;
}

//¹è°æÀ» ÀÐ¾î ¿Â´Ù
smSTAGE3D *LoadStage2(char *szStageFile)
{
	smSTAGE3D* lpstg;

	if (lstrcmpi(szGameStageName[0], szStageFile) == 0) return smGameStage[0];
	if (lstrcmpi(szGameStageName[1], szStageFile) == 0) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// ¶óÀÌÆ® º¤ÅÍ
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright = smConfig.MapBright;
	lpstg->Contrast = smConfig.MapContrast;

	//¹è°æÀ» »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
	smSTAGE3D_ReadASE(szStageFile, lpstg);
	lpstg->Head = TRUE;

	smStageCount = (smStageCount + 1) & 1;

	if (smGameStage[smStageCount]) delete smGameStage[smStageCount];

	smGameStage[smStageCount] = lpstg;
	//ReadTextures();
	return lpstg;
}


int PlayCounter = 0;

char PlayerName[64] = { 0,0,0,0 };


void InitStage()
{
	szGameStageName[0][0] = 0;
	szGameStageName[1][0] = 0;

	InitField();

	if (GameStartField < 0) 
	{
		StartField();
	}
	else {

		if (CharacterPosX == PrisonX && CharacterPosZ == PrisonZ) 
		{
			WarpPrisonField((int *)&smConfig.Posi_Player.x, (int *)&smConfig.Posi_Player.y);
		}
		else {
			WarpFieldNearPos(GameStartField, CharacterPosX, CharacterPosZ, (int *)&smConfig.Posi_Player.x, (int *)&smConfig.Posi_Player.y);
		}
	}

	smRender.Color_R = 0;
	smRender.Color_G = 0;
	smRender.Color_B = 0;
	smRender.Color_A = 0;
}


void CloseStage()
{
	if (smGameStage[1]) delete smGameStage[1];
	if (smGameStage[0]) delete smGameStage[0];

}

BOOL  SameSpecFilterDropItem(scITEM * item)
{
	char szBuff[32];
	BOOL bCanView = FALSE;

	switch (cItem.GetJobCode(item->iClass))
	{
	case 1:
		bCanView = Settings::GetInstance()->cFS;
		break;
	case 2:
		bCanView = Settings::GetInstance()->cMS;
		break;
	case 3:
		bCanView = Settings::GetInstance()->cAS;
		break;
	case 4:
		bCanView = Settings::GetInstance()->cPS;
		break;
	case 5:
		bCanView = Settings::GetInstance()->cATS;
		break;
	case 6:
		bCanView = Settings::GetInstance()->cKS;
		break;
	case 7:
		bCanView = Settings::GetInstance()->cMGS;
		break;
	case 8:
		bCanView = Settings::GetInstance()->cPRS;
		break;
	default:
		bCanView = Settings::GetInstance()->cNaked;
		break;
	}

	if ((item->ItemCode & sinITEM_MASK1) == 0x04000000 || (item->ItemCode & sinITEM_MASK2) == sinGP1 || (item->ItemCode & sinITEM_MASK2) == sinGG1)
		bCanView = TRUE;

	return bCanView;
}

// Filtro de itens
BOOL CanViewDropItem(scITEM * item)
{
	if (Settings::GetInstance()->cFilter)
	{
		BOOL bRet = FALSE;

		if ((item->ItemCode & sinITEM_MASK1) == 0x04000000)
		{
			if ((item->ItemCode & sinITEM_MASK2) == sinPL1)
				if (Settings::GetInstance()->cHP)
					bRet = TRUE;
			if ((item->ItemCode & sinITEM_MASK2) == sinPM1)
				if (Settings::GetInstance()->cMP)
					bRet = TRUE;
			if ((item->ItemCode & sinITEM_MASK2) == sinPS1)
				if (Settings::GetInstance()->cSP)
					bRet = TRUE;
		}
		else if (item->ItemCode == 0x05010100)
		{
			if (Settings::GetInstance()->cGold)
				bRet = TRUE;
		}
		else if ((item->ItemCode & sinITEM_MASK2) == sinOA1)
		{
			if (Settings::GetInstance()->cAmulets)
				bRet = TRUE;
		}
		else if (((item->ItemCode & sinITEM_MASK2) == sinOR1) || ((item->ItemCode & sinITEM_MASK2) == sinOR2))
		{
			if (Settings::GetInstance()->cRings)
				bRet = TRUE;
		}
		else if ((item->ItemCode & sinITEM_MASK2) == sinOS1)
		{
			if (Settings::GetInstance()->cSheltoms)
				bRet = TRUE;
		}
		else if ((item->ItemCode & sinITEM_MASK2) == sinFO1)
		{
			if (Settings::GetInstance()->cForce)
				bRet = TRUE;
		}
		else if (((item->ItemCode & sinITEM_MASK2) == sinBI1) || ((item->ItemCode & sinITEM_MASK2) == sinBI2))
		{
			if (Settings::GetInstance()->cPremiums)
				bRet = TRUE;
		}
		else if ((item->ItemCode & sinITEM_MASK2) == sinGP1)
		{
			if (Settings::GetInstance()->cCrystal)
				bRet = TRUE;
		}
		else if (((item->ItemCode & sinITEM_MASK1) == 0x02000000) || ((item->ItemCode & sinITEM_MASK2) == sinOM1))
		{
			if (Settings::GetInstance()->cDefItem)
				bRet = TRUE;
		}
		else if (((item->ItemCode & sinITEM_MASK1) == 0x01000000) || ((item->ItemCode & sinITEM_MASK2) == sinOA2))
		{
			if (Settings::GetInstance()->cOffItem)
				bRet = TRUE;
		}
		else if (Settings::GetInstance()->cElse)
			bRet = TRUE;

		if (bRet)
		{
			if (Settings::GetInstance()->cFilterSpec)
				bRet = SameSpecFilterDropItem(item);
		}

		return bRet;
	}

	return TRUE;
}

struct _DRAW_LIGHTLIST {
	smLIGHT3D	*smLight;
	int			x, y, z;
};


_DRAW_LIGHTLIST	DrawLight[64];
_DRAW_LIGHTLIST	DrawLightSmall[64];
int DrawLightCnt;
int DrawLightSmallCnt;


int DrawLightImage(smSTAGE3D *lpStage, int x, int y, int z)
{

	// ÁÂÇ¥¸¦ 2D ¿µ¿ªÀ¸·Î º¯È¯ÇÏ¿© µ¹·ÁÁØ´Ù  
	//int smRENDER3D::GetRect2D ( int x, int y, int z, int width, int height, RECT *Rect ,POINT *Pos )

	POINT	Pos;
	RECT	Rect;

	int cnt, cnt2;
	int dx, dy, dz, dist;
	int tz;

	//	smLIGHT3D	*DrawLight[64];
	//	smLIGHT3D	*DrawLightSmall[64];

	int x1, y1, z1, x2, y2, z2, x3, y3, z3;
	smRENDFACE *rendface;
	smRENDVERTEX *rv;
	int flg;

	//DDSURFACEDESC2		ddsd;
	//WORD *Dest;
	//DWORD dwZCode;
	//DWORD dwZPoint;

	int type;


	for (cnt = 0; cnt < lpStage->nLight; cnt++) {

		type = lpStage->smLight[cnt].type;

		if ((type&smLIGHT_TYPE_LENS) &&
			(!(lpStage->smLight[cnt].type&smLIGHT_TYPE_NIGHT) || NightDayTime)) {

			dx = (x - lpStage->smLight[cnt].x) >> FLOATNS;
			dy = (y - lpStage->smLight[cnt].y) >> FLOATNS;
			dz = (z - lpStage->smLight[cnt].z) >> FLOATNS;
			dist = dx * dx + dy * dy + dz * dz;

			if (dist < DIST_TRANSLEVEL_LOW && abs(dx) < 64 * 38 && abs(dz) < 64 * 38) {
				if (tz = smRender.GetRect2D(lpStage->smLight[cnt].x, lpStage->smLight[cnt].y, lpStage->smLight[cnt].z,
					0, 0, &Rect, &Pos)) {

					if (Pos.x > 0 && Pos.x < WinSizeX && Pos.y>0 && Pos.y < WinSizeY) {
						tz -= 64 * fONE;

						if (tz > (64 * 22 * fONE)) {
							DrawLightSmall[DrawLightSmallCnt].smLight = &lpStage->smLight[cnt];
							DrawLightSmall[DrawLightSmallCnt].x = Pos.x;
							DrawLightSmall[DrawLightSmallCnt].y = Pos.y;
							DrawLightSmall[DrawLightSmallCnt].z = tz;
							DrawLightSmallCnt++;
						}
						else {
							//dwZPoint = (DWORD)(	smConvZ3D( tz>>FLOATNS )*65535 );
							for (cnt2 = 0; cnt2 < smRender.nRendFace; cnt2++) {
								if (!(smRender.RendFace[cnt2].ClipStatus&SMCLIP_DISPLAYOUT)) {
									rendface = &smRender.RendFace[cnt2];

									rv = rendface->lpRendVertex[0];
									x1 = (int)rv->xy[0];
									y1 = (int)rv->xy[1];
									z1 = rv->tz;
									rv = rendface->lpRendVertex[1];
									x2 = (int)rv->xy[0];
									y2 = (int)rv->xy[1];
									z2 = rv->tz;
									rv = rendface->lpRendVertex[2];
									x3 = (int)rv->xy[0];
									y3 = (int)rv->xy[1];
									z3 = rv->tz;

									flg = 0;
									if (tz > z1 && tz > z2 && tz > z3) {

										if (Pos.x < x1 && Pos.x < x2 && Pos.x < x3) flg++;
										if (Pos.x > x1 && Pos.x > x2 && Pos.x > x3) flg++;
										if (Pos.y < y1 && Pos.y < y2 && Pos.y < y3) flg++;
										if (Pos.y > y1 && Pos.y > y2 && Pos.y > y3) flg++;
										if (flg == 0) break;

										//									if ( PointInTri( (float)x1,(float)x2,(float)x3,(float)y1,(float)y2,(float)y3,(float)Pos.x,(float)Pos.y)==true ) 
										//									break;
									}
								}

							}

							if (cnt2 >= smRender.nRendFace) {
								DrawLight[DrawLightCnt].smLight = &lpStage->smLight[cnt];
								DrawLight[DrawLightCnt].x = Pos.x;
								DrawLight[DrawLightCnt].y = Pos.y;
								DrawLight[DrawLightCnt].z = tz;
								DrawLightCnt++;
							}
							else {
								DrawLightSmall[DrawLightSmallCnt].smLight = &lpStage->smLight[cnt];
								DrawLightSmall[DrawLightSmallCnt].x = Pos.x;
								DrawLightSmall[DrawLightSmallCnt].y = Pos.y;
								DrawLightSmall[DrawLightSmallCnt].z = tz;
								DrawLightSmallCnt++;
							}
							/*
													flg = 0;
													dsDrawColorBox( 65535 , Pos.x-1 , Pos.y-1 , 4,4 );
													ddsd.dwSize		= sizeof(DDSURFACEDESC2);
													lpDDSZBuffer->Lock(NULL,&ddsd,DDLOCK_WAIT,NULL);
													Dest = (WORD *)ddsd.lpSurface;
													if ( Dest ) {
														dwZCode = Dest[((Pos.y * ((int)ddsd.lPitch/2))+Pos.x)];
														if (dwZCode>dwZPoint ) flg = 1;

													}
													lpDDSZBuffer->Unlock(NULL);
													if ( flg )
														DrawLight[DrawLightCnt++] = &lpStage->smLight[cnt];
							*/
						}

					}
				}
			}
		}
	}

	return TRUE;
}


int DrawLightImage()
{
	smFACE2D face2d;
	int size;
	smRENDFACE *RendFace;
	int cnt;

	smRender.SetMaterialGroup(smMaterialGroup);

	smRender.Init();

	for (cnt = 0; cnt < DrawLightSmallCnt; cnt++) {

		size = 40 * fONE;

		if (size > 0) {

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = size;
			face2d.height = size;
			face2d.Transparency = 255;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = DrawLightSmall[cnt].smLight->x;
			face2d.y = DrawLightSmall[cnt].smLight->y;
			face2d.z = DrawLightSmall[cnt].smLight->z;
			face2d.MatNum = MatLight[1];

			RendFace = smRender.AddFace2D(&face2d);
		}
	}

	if (smRender.nRendFace > 0) {
		smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
		smRender.ClipRendFace();							// ÀüÃ¼ ·»´õ¸µ ÆäÀÌ½º¸¦ Å¬¸®ÇÎ
		smRender.GeomVertex2D(FALSE);				// ¹öÅØ½º¸¦ 2DÁÂÇ¥·Î º¯È¯  
		smRender.RenderD3D();
	}


	smRender.Init();

	for (cnt = 0; cnt < DrawLightCnt; cnt++) {
		size = 160 * fONE;

		/*
		if ( DrawLight[cnt].z>(64*18*fONE) ) {
			size -= (DrawLight[cnt].z - (64*18*fONE))*2;
		}
		*/

		if (size > 0) {

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = size;
			face2d.height = size;
			face2d.Transparency = 255;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = DrawLight[cnt].smLight->x;
			face2d.y = DrawLight[cnt].smLight->y;
			face2d.z = DrawLight[cnt].smLight->z;
			face2d.MatNum = MatLight[0];

			RendFace = smRender.AddFace2D(&face2d);
		}
	}

	if (smRender.nRendFace > 0) {
		smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
		smRender.ClipRendFace();							// ÀüÃ¼ ·»´õ¸µ ÆäÀÌ½º¸¦ Å¬¸®ÇÎ
		smRender.GeomVertex2D(FALSE);				// ¹öÅØ½º¸¦ 2DÁÂÇ¥·Î º¯È¯  
		for (cnt = 0; cnt < smRender.nRendVertex; cnt++) {
			smRender.RendVertex[cnt].zb = 0.001f;
		}
		//GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ZENABLE, FALSE );
		smRender.RenderD3D();
		//GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ZENABLE, TRUE );
	}


	return TRUE;

}

int SetStageDynLight(smSTAGE3D *lpStage, int x, int y, int z)
{

	int cnt;
	int dx, dy, dz, dist;
	int type;

	extern int ConfigUseDynamicLights;

	for (cnt = 0; cnt < lpStage->nLight; cnt++) {

		dx = (x - lpStage->smLight[cnt].x) >> FLOATNS;
		dy = (y - lpStage->smLight[cnt].y) >> FLOATNS;
		dz = (z - lpStage->smLight[cnt].z) >> FLOATNS;
		dist = dx * dx + dy * dy + dz * dz;

		type = lpStage->smLight[cnt].type;

		if (dist < 0x300000 && (type & smLIGHT_TYPE_DYNAMIC) && abs(dx) < 0x2000 && abs(dz) < 0x2000) {
			if ((lpStage->smLight[cnt].type & smLIGHT_TYPE_NIGHT) && NightDayTime) {

				//######################################################################################
				//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
				smRender.AddDynamicLight(lpStage->smLight[cnt].type,
					lpStage->smLight[cnt].x, lpStage->smLight[cnt].y, lpStage->smLight[cnt].z,
					lpStage->smLight[cnt].r, lpStage->smLight[cnt].g, lpStage->smLight[cnt].b,
					0, lpStage->smLight[cnt].Range);

				if (ConfigUseDynamicLights)
				{
					Graphics::RenderLight light;
					light.position.x = lpStage->smLight[cnt].x / 256.0f;
					light.position.y = lpStage->smLight[cnt].y / 256.0f;
					light.position.z = lpStage->smLight[cnt].z / 256.0f;
					light.color.r = lpStage->smLight[cnt].r / 255.0f;
					light.color.g = lpStage->smLight[cnt].g / 255.0f;
					light.color.b = lpStage->smLight[cnt].b / 255.0f;
					light.color.a = 1.0f;
					light.range = lpStage->smLight[cnt].Range;
					Graphics::Graphics::GetInstance()->GetRenderer()->PushLight(light);

					RenderLight lightPT;
					lightPT.position.x = lpStage->smLight[cnt].x / 256.0f;
					lightPT.position.y = lpStage->smLight[cnt].y / 256.0f;
					lightPT.position.z = lpStage->smLight[cnt].z / 256.0f;
					lightPT.color.r = lpStage->smLight[cnt].r / 255.0f;
					lightPT.color.g = lpStage->smLight[cnt].g / 255.0f;
					lightPT.color.b = lpStage->smLight[cnt].b / 255.0f;
					lightPT.color.a = 1.0f;
					lightPT.range = lpStage->smLight[cnt].Range;
					smRender.Lights.push_back(lightPT);
				}
			}
			else {
				if (DarkLevel > 0) {
					//######################################################################################
					//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
					smRender.AddDynamicLight(lpStage->smLight[cnt].type,
						lpStage->smLight[cnt].x, lpStage->smLight[cnt].y, lpStage->smLight[cnt].z,
						//r,g,b,
						(lpStage->smLight[cnt].r * DarkLevel) >> 8,
						(lpStage->smLight[cnt].g * DarkLevel) >> 8,
						(lpStage->smLight[cnt].b * DarkLevel) >> 8,
						0, lpStage->smLight[cnt].Range);

					if (ConfigUseDynamicLights)
					{
						Graphics::RenderLight light;
						light.position.x = lpStage->smLight[cnt].x / 256.0f;
						light.position.y = lpStage->smLight[cnt].y / 256.0f;
						light.position.z = lpStage->smLight[cnt].z / 256.0f;
						light.color.r = ((lpStage->smLight[cnt].r * DarkLevel) >> 8) / 255.0f;
						light.color.g = ((lpStage->smLight[cnt].g * DarkLevel) >> 8) / 255.0f;
						light.color.b = ((lpStage->smLight[cnt].b * DarkLevel) >> 8) / 255.0f;
						light.color.a = 1.0f;
						light.range = lpStage->smLight[cnt].Range;
						Graphics::Graphics::GetInstance()->GetRenderer()->PushLight(light);

						RenderLight lightPT;
						lightPT.position.x = lpStage->smLight[cnt].x / 256.0f;
						lightPT.position.y = lpStage->smLight[cnt].y / 256.0f;
						lightPT.position.z = lpStage->smLight[cnt].z / 256.0f;
						lightPT.color.r = ((lpStage->smLight[cnt].r * DarkLevel) >> 8) / 255.0f;
						lightPT.color.g = ((lpStage->smLight[cnt].g * DarkLevel) >> 8) / 255.0f;
						lightPT.color.b = ((lpStage->smLight[cnt].b * DarkLevel) >> 8) / 255.0f;
						lightPT.color.a = 1.0f;
						lightPT.range = lpStage->smLight[cnt].Range;
						smRender.Lights.push_back(lightPT);
					}
				}
			}
		}
	}

	return TRUE;

}




int doop = 0;



int DisplayStage(int x, int y, int z, int angX, int angY, int angZ)
{
	int cnt;
	int r, g, b, l;



	DrawLightCnt = 0;
	DrawLightSmallCnt = 0;


	smRender.ZWriteAuto = TRUE;


	for (cnt = 0; cnt < 2; cnt++)
	{
		if (smGameStage[cnt])
		{



			if (DarkLevel > 0 && StageField[cnt]->State == FIELD_STATE_VILLAGE)
			{

				r = smRender.Color_R;
				g = smRender.Color_G;
				b = smRender.Color_B;
				l = smRender.nRendLight;

				smRender.Color_R >>= 1;
				smRender.Color_G >>= 1;
				smRender.Color_B >>= 1;

				if (smRender.nRendLight > 0 && smRender.smRendLight[0].Range == DarkLightRange)
				{
					smRender.smRendLight[0].Range = 0;
					l = 1;
				}
				else
					l = 0;

				if (smGameStage[cnt]->DrawStage2(x, y, z, angX, angY, angZ) == TRUE)
				{




					DrawLightImage(smGameStage[cnt], x, y, z);
					smGameStage[cnt]->StageObject->Draw(x, y, z, angX, angY, angZ);
				}

				smRender.Color_R = r;
				smRender.Color_G = g;
				smRender.Color_B = b;

				if (l) smRender.smRendLight[0].Range = DarkLightRange;



			}
			else
			{
				if (smGameStage[cnt]->DrawStage2(x, y, z, angX, angY, angZ) == TRUE)
				{



					DrawLightImage(smGameStage[cnt], x, y, z);
					smGameStage[cnt]->StageObject->Draw(x, y, z, angX, angY, angZ);
				}
			}

		}
	}




	smRender.ZWriteAuto = FALSE;

	if ((MaterialFrame & 0x3F) == 0)
	{


		CheckInvenItemCode();
	}

	return TRUE;
}

//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
void CreatePatIceFoot(int Num, int sizeX, int sizeZ)
{
	int MatShadow = CreateTextureMaterial(g_IceFootImageFileBuff[Num], 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	smTPOINT tPoint[4];
	tPoint[0].u = 0;
	tPoint[0].v = 0;
	tPoint[1].u = 256;
	tPoint[1].v = 0;
	tPoint[2].u = 0;
	tPoint[2].v = 256;
	tPoint[3].u = 256;
	tPoint[3].v = 256;

	smOBJ3D *objIceFoot = new smOBJ3D(4, 2);
	objIceFoot->AddVertex(-sizeX, -sizeZ, 0);
	objIceFoot->AddVertex(sizeX, -sizeZ, 0);
	objIceFoot->AddVertex(-sizeX, sizeZ, 0);
	objIceFoot->AddVertex(sizeX, sizeZ, 0);

	objIceFoot->AddFace(0, 1, 2, &tPoint[2], &tPoint[3], &tPoint[0]);
	objIceFoot->AddFace(2, 1, 3, &tPoint[0], &tPoint[3], &tPoint[1]);
	objIceFoot->ZeroNormals();

	objIceFoot->SetFaceMaterial(0, MatShadow);
	objIceFoot->SetFaceMaterial(1, MatShadow);

	PatIceFoot[Num] = new smPAT3D;
	PatIceFoot[Num]->AddObject(objIceFoot);
}
//######################################################################################

void CreatePatShadow(int Num, int sizeX, int sizeZ)
{
	smTPOINT tPoint[4];
	smOBJ3D *objShadow;

	int MatShadow;

	MatShadow = CreateTextureMaterial(szShadowImageFile, 0, 0, 0, 0, SMMAT_BLEND_SHADOW);
	smMaterial[MatShadow].SelfIllum = 1;

	tPoint[0].u = 0;
	tPoint[0].v = 0;
	tPoint[1].u = 256;
	tPoint[1].v = 0;
	tPoint[2].u = 0;
	tPoint[2].v = 256;
	tPoint[3].u = 256;
	tPoint[3].v = 256;


	objShadow = new smOBJ3D(4, 2);
	objShadow->AddVertex(-sizeX, -sizeZ, 0);
	objShadow->AddVertex(sizeX, -sizeZ, 0);
	objShadow->AddVertex(-sizeX, sizeZ, 0);
	objShadow->AddVertex(sizeX, sizeZ, 0);

	objShadow->AddFace(0, 1, 2, &tPoint[0], &tPoint[1], &tPoint[2]);
	objShadow->AddFace(2, 1, 3, &tPoint[2], &tPoint[1], &tPoint[3]);
	objShadow->ZeroNormals();

	objShadow->SetFaceMaterial(0, MatShadow);
	objShadow->SetFaceMaterial(1, MatShadow);

	PatShadow[Num] = new smPAT3D;
	PatShadow[Num]->AddObject(objShadow);
}

void CreatePatShadow2(int Num, int sizeX, int sizeZ)
{
	smTPOINT tPoint[9];
	smOBJ3D *objShadow;

	int MatShadow;

	MatShadow = CreateTextureMaterial(szShadowImageFile, 0, 0, 0, 0, SMMAT_BLEND_SHADOW);
	smMaterial[MatShadow].SelfIllum = 1;

	tPoint[0].u = 0;
	tPoint[0].v = 0;
	tPoint[1].u = 128;
	tPoint[1].v = 0;
	tPoint[2].u = 256;
	tPoint[2].v = 0;

	tPoint[3].u = 0;
	tPoint[3].v = 128;
	tPoint[4].u = 128;
	tPoint[4].v = 128;
	tPoint[5].u = 256;
	tPoint[5].v = 128;

	tPoint[6].u = 0;
	tPoint[6].v = 256;
	tPoint[7].u = 128;
	tPoint[7].v = 256;
	tPoint[8].u = 256;
	tPoint[8].v = 256;


	objShadow = new smOBJ3D(9, 8);
	objShadow->AddVertex(-sizeX, -sizeZ, 0);
	objShadow->AddVertex(0, -sizeZ, 0);
	objShadow->AddVertex(sizeX, -sizeZ, 0);

	objShadow->AddVertex(-sizeX, 0, 0);
	objShadow->AddVertex(0, 0, 0);
	objShadow->AddVertex(sizeX, 0, 0);

	objShadow->AddVertex(-sizeX, sizeZ, 0);
	objShadow->AddVertex(0, sizeZ, 0);
	objShadow->AddVertex(sizeX, sizeZ, 0);

	objShadow->AddFace(0, 1, 3, &tPoint[0], &tPoint[1], &tPoint[3]);
	objShadow->AddFace(3, 1, 4, &tPoint[3], &tPoint[1], &tPoint[4]);

	objShadow->AddFace(1, 2, 4, &tPoint[0], &tPoint[2], &tPoint[4]);
	objShadow->AddFace(4, 2, 5, &tPoint[4], &tPoint[2], &tPoint[5]);

	objShadow->AddFace(3, 4, 6, &tPoint[3], &tPoint[4], &tPoint[6]);
	objShadow->AddFace(6, 4, 7, &tPoint[6], &tPoint[4], &tPoint[7]);

	objShadow->AddFace(4, 5, 7, &tPoint[4], &tPoint[5], &tPoint[7]);
	objShadow->AddFace(7, 5, 8, &tPoint[7], &tPoint[5], &tPoint[8]);

	objShadow->SetFaceMaterial(0, MatShadow);
	objShadow->SetFaceMaterial(1, MatShadow);
	objShadow->SetFaceMaterial(2, MatShadow);
	objShadow->SetFaceMaterial(3, MatShadow);
	objShadow->SetFaceMaterial(4, MatShadow);
	objShadow->SetFaceMaterial(5, MatShadow);
	objShadow->SetFaceMaterial(6, MatShadow);
	objShadow->SetFaceMaterial(7, MatShadow);

	PatShadow[Num] = new smPAT3D;
	PatShadow[Num]->AddObject(objShadow);
}


//°ü¸®ÀÚ Ä³¸¯ÅÍ ¸ðµå
int AdminCharMode = 0;
//°ü¸®ÀÚ Ä³¸¯ÅÍ ¸ðµå½Ã »ó´ë¹æ¿¡ µ¥ÀÌÅ¸ ¼ÛÃâ ±ÝÁö
int AdminCharDisable = 0;

//Ä³¸¯ÅÍ ÆÄÀÏ ¸®½ºÆ®
static char szDataDirectory[256];
char szCharFileList[256][256];
int CharFileCnt;
int CharPlayCnt = 0;

//ÆÄÀÏ ÀÌ¸§À¸·Î ºÎÅÍ µð·ºÅä¸®¸¦ Ã£¾Æ ¼³Á¤
static char *SetDirectoryFromFile(char *filename)
{
	int len;
	int cnt;

	lstrcpy(szDataDirectory, filename);

	len = lstrlen(szDataDirectory);

	for (cnt = len; cnt >= 0; cnt--)
		if (szDataDirectory[cnt] == '\\') break;

	szDataDirectory[cnt + 1] = NULL;

	return szDataDirectory;
}


//Ä³¸¯ÅÍ ÆÄÀÏ ¸®½ºÆ® ¼³Á¤
int SetCharFileList(char *szCharFile)
{
	CharFileCnt = 0;
	char szFindPath[256];

	SetDirectoryFromFile(szCharFile);

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	lstrcpy(szFindPath, szDataDirectory);
	lstrcat(szFindPath, "\\*.ini");

	//ÆÄÀÏÀ» Ã£À½( È®ÀåÀÚ º¯°æ ÆÄÀÏ )
	hFindHandle = FindFirstFile(szFindPath, &fd);
	if (hFindHandle == INVALID_HANDLE_VALUE) {
		FindClose(hFindHandle);
		return 0;
	}
	while (1) {
		lstrcpy(szCharFileList[CharFileCnt], szDataDirectory);
		lstrcat(szCharFileList[CharFileCnt], fd.cFileName);
		CharFileCnt++;

		if (FindNextFile(hFindHandle, &fd) == FALSE) break;
		if (CharFileCnt > 255) break;
	}
	FindClose(hFindHandle);


	return TRUE;
}

void SetNextPlayerPattern()
{
	if (lpCurPlayer->PatLoading) return;

	//°ü¸®ÀÚ Ä³¸¯ÅÍ ¸ðµå½Ã »ó´ë¹æ¿¡ µ¥ÀÌÅ¸ ¼ÛÃâ ±ÝÁö
	AdminCharDisable = TRUE;

	if (AdminCharMode) {
		memcpy(&lpCurPlayer->smCharInfo, &smCharInfo_Back, sizeof(smCHAR_INFO));
		ReformCharForm();
		AdminCharMode = 0;
		lpCurPlayer->PatLoading = TRUE;
		AddLoaderPattern(lpCurPlayer, lpCurPlayer->smCharInfo.szModelName, lpCurPlayer->smCharInfo.szModelName2);
		lpCurPlayer->DisplayTools = TRUE;
		return;
	}

	AdminCharMode = 0;
	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern(lpCurPlayer, szCharFileList[CharPlayCnt], lpCurPlayer->smCharInfo.szModelName2);
	lpCurPlayer->DisplayTools = TRUE;

	CharPlayCnt++;
	if (CharPlayCnt >= CharFileCnt) CharPlayCnt = 0;
}

void SetPlayerBodyPattern(char *szBodyFile)
{
	char szBuff[256];
	FILE *fp;


	if (lpCurPlayer->PatLoading) return;

	wsprintf(szBuff, "char\\tmABCD\\%s.ini", szBodyFile);
	fp = fopen(szBuff, "rb");
	if (!fp) return;
	fclose(fp);
	/*
		//°ü¸®ÀÚ Ä³¸¯ÅÍ ¸ðµå½Ã »ó´ë¹æ¿¡ µ¥ÀÌÅ¸ ¼ÛÃâ ±ÝÁö
		AdminCharDisable = TRUE;
		AdminCharMode = 0;
	*/
	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern(lpCurPlayer, szBuff, lpCurPlayer->smCharInfo.szModelName2);
	lpCurPlayer->DisplayTools = TRUE;
}

void SetPlayerPattern(char *szFile)
{
	FILE *fp;


	if (lpCurPlayer->PatLoading) return;

	fp = fopen(szFile, "rb");
	if (!fp) return;
	fclose(fp);

	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern(lpCurPlayer, szFile, 0);
	lpCurPlayer->DisplayTools = TRUE;

	ReformCharForm();

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);

}


char szNpcFileList[64][64];
int NpcFileCnt;
int NpcPlayCnt = 0;

char *NpcSelectedName = 0;

//Ä³¸¯ÅÍ ÆÄÀÏ ¸®½ºÆ® ¼³Á¤
int SetNpcFileList(char *szNpcDirectory)
{
	NpcFileCnt = 0;
	char szFindPath[256];

	//	SetDirectoryFromFile( szCharFile );

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	lstrcpy(szFindPath, szNpcDirectory);
	lstrcat(szFindPath, "*.npc");

	//ÆÄÀÏÀ» Ã£À½( È®ÀåÀÚ º¯°æ ÆÄÀÏ )
	hFindHandle = FindFirstFile(szFindPath, &fd);
	if (hFindHandle == INVALID_HANDLE_VALUE) {
		FindClose(hFindHandle);
		return 0;

	}
	while (1) {
		lstrcpy(szNpcFileList[NpcFileCnt], szNpcDirectory);
		lstrcat(szNpcFileList[NpcFileCnt], fd.cFileName);
		NpcFileCnt++;

		if (FindNextFile(hFindHandle, &fd) == FALSE) break;
		if (NpcFileCnt > 64) break;

	}
	FindClose(hFindHandle);

	return TRUE;
}

void SetNextNpcPattern()
{
	if (lpCurPlayer->PatLoading) return;

	if (AdminCharMode == 0) {
		memcpy(&smCharInfo_Back, &lpCurPlayer->smCharInfo, sizeof(smCHAR_INFO));
	}

	//°ü¸®ÀÚ Ä³¸¯ÅÍ ¸ðµå½Ã »ó´ë¹æ¿¡ µ¥ÀÌÅ¸ ¼ÛÃâ ±ÝÁö
	AdminCharDisable = TRUE;

	AdminCharMode = 1;				//NPC ¼³Á¤ ¸ðµå


	lpCurPlayer->smCharInfo.dwCharSoundCode = 0;

	//Ä³¸¯ÅÍ Á¤º¸ ÆÄÀÏ¿¡¼­ ÇØµ¶ÇÏ¿© ¼³Á¤ÇÑ´Ù
	if (smCharDecode(szNpcFileList[NpcPlayCnt], &lpCurPlayer->smCharInfo, 0)) {
		lpCurPlayer->PatLoading = TRUE;
		AddLoaderPattern(lpCurPlayer, lpCurPlayer->smCharInfo.szModelName);

		lpCurPlayer->DisplayTools = FALSE;
		lpCurPlayer->smCharInfo.SizeLevel = 1;

		NpcSelectedName = szNpcFileList[NpcPlayCnt];

		NpcPlayCnt++;
		if (NpcPlayCnt >= NpcFileCnt) NpcPlayCnt = 0;
	}
	ReformCharForm();
}

//////////////// µ¿·á È¸º¹ ÇÔ¼ö ////////////////
int LaunchPartyPostion()
{
	int x, y, z, dist, ddist;
	int cnt;
	smCHAR *lpChar;

	x = lpCurPlayer->pX;
	y = lpCurPlayer->pY + 48 * fONE;
	z = lpCurPlayer->pZ;

	if (sPartyPosion.code == smTRANSCODE_PARTY_POTION) {

		if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
			CheckCharForm();
			if (sPartyPosion.WParam) {
				sinSetLife(sinGetLife() + sPartyPosion.WParam);
				StartEffect(x, y, z, EFFECT_POTION1);
			}
			if (sPartyPosion.LParam) {
				sinSetMana(sinGetMana() + sPartyPosion.LParam);
				StartEffect(x, y, z, EFFECT_POTION2);
			}
			if (sPartyPosion.SParam) {
				sinSetStamina(sinGetStamina() + sPartyPosion.SParam);
				StartEffect(x, y, z, EFFECT_POTION3);
			}
			ResetEnergyGraph(3);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ReformCharForm();
			sinPlaySound(SIN_SOUND_EAT_POTION, GetDistVolume(x, y, z));			//¹°¾à¸Ô´Â ¼Ò¸®

		}



		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
			if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && chrOtherPlayer[cnt].PartyFlag &&
				chrOtherPlayer[cnt].dwObjectSerial != sPartyPosion.EParam &&
				chrOtherPlayer[cnt].MotionInfo->State != CHRMOTION_STATE_DEAD &&
				chrOtherPlayer[cnt].RendSucess) {

				x = chrOtherPlayer[cnt].pX;
				y = chrOtherPlayer[cnt].pY + 48 * fONE;
				z = chrOtherPlayer[cnt].pZ;

				if (sPartyPosion.WParam) 	StartEffect(x, y, z, EFFECT_POTION1);
				if (sPartyPosion.LParam) 	StartEffect(x, y, z, EFFECT_POTION2);
				if (sPartyPosion.SParam) 	StartEffect(x, y, z, EFFECT_POTION3);
			}
		}


	}

	switch (sPartyPosion.code) {
	case smTRANSCODE_HEALING:
		//Ä¡·á
		if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
			CheckCharForm();
			if (sPartyPosion.WParam) {
				sinSetLife(sinGetLife() + sPartyPosion.WParam);
			}
			ResetEnergyGraph(0);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ReformCharForm();
		}
		break;

	case smTRANSCODE_GRAND_HEALING:
		//±×·£µå Èú¸µ
		if (sPartyPosion.EParam == lpCurPlayer->dwObjectSerial) {
			lpChar = lpCurPlayer;
		}
		else
			lpChar = FindAutoPlayer(sPartyPosion.EParam);

		if (!lpChar) break;

		ddist = sPartyPosion.SParam*sPartyPosion.SParam;

		x = (lpCurPlayer->pX - lpChar->pX) >> FLOATNS;
		y = (lpCurPlayer->pY - lpChar->pY) >> FLOATNS;
		z = (lpCurPlayer->pZ - lpChar->pZ) >> FLOATNS;
		dist = x * x + y * y + z * z;
		if (dist < ddist && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
			//À¯È¿°Å¸®³» Èú¸µ ½ÇÇà
			if (sPartyPosion.LParam) {
				CheckCharForm();
				sinSetLife(sinGetLife() + sPartyPosion.LParam);
				ResetEnergyGraph(0);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
				ReformCharForm();
			}

			if (lpChar != lpCurPlayer)
				sinSkillEffect_Great_Healing_Party(lpCurPlayer);
		}


		//ÆÄÆ¼¿øµé Èú¸µ ÀÌÆåÆ®
		for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
			if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && lpChar != &chrOtherPlayer[cnt] &&
				chrOtherPlayer[cnt].PartyFlag && chrOtherPlayer[cnt].MotionInfo->State != CHRMOTION_STATE_DEAD) {

				x = (chrOtherPlayer[cnt].pX - lpChar->pX) >> FLOATNS;
				y = (chrOtherPlayer[cnt].pY - lpChar->pY) >> FLOATNS;
				z = (chrOtherPlayer[cnt].pZ - lpChar->pZ) >> FLOATNS;
				dist = x * x + y * y + z * z;

				if (dist < ddist) {
					sinSkillEffect_Great_Healing_Party(&chrOtherPlayer[cnt]);
				}
			}
		}
		break;

	case smTRANSCODE_VAMPRIC_CUSPID:
		if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
			CheckCharForm();
			if (sPartyPosion.WParam) {
				sinSetLife(sinGetLife() + sPartyPosion.WParam);
			}
			ResetEnergyGraph(0);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ReformCharForm();
		}
		break;

		// Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	case smTRANSCODE_VAMPRIC_CUSPID_EX:
		if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
			CheckCharForm();
			if (sPartyPosion.WParam) {
				sinSetLife(sinGetLife() + sPartyPosion.WParam);
			}
			ResetEnergyGraph(0);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ReformCharForm();
		}
		break;

	case smTRANSCODE_MANA_RECHARGING:
		if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
			CheckCharForm();
			if (sPartyPosion.WParam) {
				sinSetMana(sinGetMana() + sPartyPosion.WParam);
			}
			ResetEnergyGraph(1);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ReformCharForm();
		}
		break;

	}




	sPartyPosion.code = 0;
	dwPartyPosionTime = 0;


	return TRUE;
}

//µ¿·á È¸º¹ ¼³Á¤
int SetPartyPosion(smTRANS_COMMAND *lpTransCommand)
{
	if (dwPartyPosionTime) {
		//ÀÌ¹Ì ¿¹¾àÁß
		LaunchPartyPostion();
	}

	dwPartyPosionTime = dwPlayTime + 1000;
	memcpy(&sPartyPosion, lpTransCommand, sizeof(smTRANS_COMMAND));

	return TRUE;
}



char szPlayerCode[32];
int CharacterJob = 0;			//Á÷¾÷ ÄÚµå ³Ñ°ÜÁÜ

char *szAnimShieldEffectFile[] = {
	"Weapons\\test1.tga",
	"Weapons\\test2.tga",
	"Weapons\\test3.tga",
	"Weapons\\test2.tga"
};

char *szBangImage = "weapons\\bang2.bmp";
char *szClowImage = "weapons\\crown.tga";

extern	char *szNpcInfoPath;

DWORD	dwSoD_ResultTime = 0;		//SoD »óÈ²Ã¢ °»½Å ½Ã°£


sBILLING_INFO	*sBiInfo = 0;


int InitPat3D()
{
	int cnt, cnt2, size;
	POINT3D Posi, Angle;

	//	smMODELINFO	modelInfo;
	DWORD nsize = 62;

	if (!sBiInfo) {
		sBiInfo = new sBILLING_INFO;
		ZeroMemory(sBiInfo, sizeof(sBILLING_INFO));
	}

	if (PlayerName[0] == 0)
		GetComputerName(PlayerName, &nsize);

	InterfaceParty.chatbuild_INIfiles();	//µ¿·á->Ä£±¸	ÃÊ±â¼³Á¤

	//ÀÚµ¿ ¼¼ÀÌºê ÃÊ±âÈ­
	rsRecorder.dwRecordTime = 0;
	rsRecorder.dwNextRecordTime = GetCurrentTime() + 10 * 60 * 1000;


	//ÀÎÅÍÆäÀÌ½º ¼³Á¤°ª(ÅÂ±¹¼öÁ¤)
#ifdef	_LANGUAGE_THAI		//0x57
	cInterFace.sInterFlags.CameraAutoFlag = 0;			//Ä«¸Þ¶ó °íÁ¤ ( ±âº» )
#else	
	cInterFace.sInterFlags.CameraAutoFlag = 2;			//Ä«¸Þ¶ó °íÁ¤ ( ±âº» )
#endif	

	cInterFace.sInterFlags.MapOnFlag = TRUE;			//¹Ì´Ï¸Ê Ç¥½Ã
	cInterFace.sInterFlags.RunFlag = TRUE;				//´Þ¸®±â ¸ðµå

	if (((GetCurrentTime() >> 4) & 1) == 0) lstrcpy(szPlayerCode, "p");
	else lstrcpy(szPlayerCode, "f");

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	CreatePatIceFoot(0, 6 * fONE, 6 * fONE);
	CreatePatIceFoot(1, 6 * fONE, 6 * fONE);
	//######################################################################################

	//±×¸²ÀÚ ¸¸µé±â
	CreatePatShadow(0, 12 * fONE, 10 * fONE);
	CreatePatShadow(1, 17 * fONE, 14 * fONE);
	CreatePatShadow2(2, 26 * fONE, 20 * fONE);
	CreatePatShadow2(3, 38 * fONE, 30 * fONE);

	//	sizeX = 10*fONE;
	//	sizeZ = 8*fONE;

	Posi.x = smConfig.Posi_Player.x;
	Posi.z = smConfig.Posi_Player.y;
	Posi.y = 0;
	Angle.x = 0;
	Angle.y = 0;
	Angle.z = 0;

	//	cnt = GetInvenToryItem(100);
	invPlayerArmor = 0;//cnt;

	//Ä³¸¯ÅÍ ÆÄÀÏ ¸®½ºÆ® ¼³Á¤
	//SetCharFileList( smConfig.szFile_Player );

	// edit xxstr otimizando game login
	
	SetCharFileList("char\\tmABCD\\a001.ini");

	SetNpcFileList(szNpcInfoPath);

	/*LoadOftenArmor();
	LoadOftenFace();
	LoadOftenItem();
	LoadOftenNpc();
	LoadOftenModels();*/

	sinChar = &lpCurPlayer->smCharInfo;

	CheckCharForm();

	if (CharacterName1[0]) {
		cnt = NewCharInfo.Dexterity + NewCharInfo.Health + NewCharInfo.Spirit + NewCharInfo.Strength + NewCharInfo.Talent;

		lpCurPlayer->smCharInfo.JOB_CODE = NewCharInfo.JobCode;

		if (cnt < 103) {
			//Ä³¸¯ÅÍ ¼¿·ºÆ®¿¡¼­ ³Ñ¾î¿Â ±âº» µ¥ÀÌÅ¸ °ª
			lpCurPlayer->smCharInfo.Dexterity = NewCharInfo.Dexterity;
			lpCurPlayer->smCharInfo.Health = NewCharInfo.Health;
			lpCurPlayer->smCharInfo.Spirit = NewCharInfo.Spirit;
			lpCurPlayer->smCharInfo.Strength = NewCharInfo.Strength;
			lpCurPlayer->smCharInfo.Talent = NewCharInfo.Talent;
		}
		else {
			lpCurPlayer->smCharInfo.Dexterity = 1;
			lpCurPlayer->smCharInfo.Health = 8;//NewCharInfo.Health;
			lpCurPlayer->smCharInfo.Spirit = 1;//NewCharInfo.Spirit;
			lpCurPlayer->smCharInfo.Strength = 8;//NewCharInfo.Strength;
			lpCurPlayer->smCharInfo.Talent = 1;//NewCharInfo.Talent;
			lpCurPlayer->smCharInfo.StatePoint = 0;//NewCharInfo.Talent;
		}
	}

	//ÇÃ·¹ÀÌ¾î ¼³Á¤
	if (CharacterName1[0]) {
		SetLoadPattern(lpCurPlayer, CharacterName1, CharacterName2, &Posi, &Angle);

		cnt2 = 0;

		if (lpCurPlayer->smMotionInfo) 
		{
			for (cnt = 0; cnt < (int)lpCurPlayer->smMotionInfo->MotionCount; cnt++) {
				if (lpCurPlayer->smMotionInfo->MotionInfo[cnt].State == CHRMOTION_STATE_DEAD) {
					//Á×´Â µ¿ÀÛ È®ÀÎ
					size = lpCurPlayer->smMotionInfo->MotionInfo[cnt].EndFrame - lpCurPlayer->smMotionInfo->MotionInfo[cnt].StartFrame;
					if (size > 8) cnt2++;
				}

				if (lpCurPlayer->smMotionInfo->MotionInfo[cnt].State == CHRMOTION_STATE_ATTACK) {
					size = lpCurPlayer->smMotionInfo->MotionInfo[cnt].EndFrame - lpCurPlayer->smMotionInfo->MotionInfo[cnt].StartFrame;
					if (size < 40 && !smConfig.DebugMode) {
						DisconnectFlag = GetCurrentTime();
						quit = TRUE;
					}
				}
			}
		}

		if (cnt2 == 0 && !smConfig.DebugMode) {
			//Á×´Â µ¿ÀÛÀÌ ¾ø´Â °æ¿ì
			DisconnectFlag = GetCurrentTime();
			quit = TRUE;
		}
	}
	else
		SetLoadPattern(lpCurPlayer, smConfig.szFile_Player, 0, &Posi, &Angle);


	lpCurPlayer->smCharInfo.Level = 1;

	lpCurPlayer->smCharInfo.Attack_Damage[0] = 5;
	lpCurPlayer->smCharInfo.Attack_Damage[1] = 10;

	lstrcpy(lpCurPlayer->smCharInfo.szName, PlayerName);
	lpCurPlayer->smCharInfo.State = smCHAR_STATE_USER;				//Ä³¸¯ÅÍ ¼Ó¼º ¼³Á¤
	lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_RESTART);		//½ÃÀÛ¸ð¼Ç

	lpCurPlayer->smCharInfo.SizeLevel = 1;							//º¸Åë Å©±â
	lpCurPlayer->PatSizeLevel = 1;									//º¸Åë Å©±â

	lpCurPlayer->smCharInfo.Attack_Rating = 100;
	lpCurPlayer->smCharInfo.Next_Exp = 1000;

	lpCurPlayer->smMonsterInfo.dwAutoCharCode = 0;

	lpCurPlayer->smCharInfo.Life[0] = 1000;
	lpCurPlayer->smCharInfo.Life[1] = 1000;

	lpCurPlayer->MoveMode = TRUE;
	lpCurPlayer->MoveCnt = 0;

	PkMode = 0;				//PK±â´É ÃÊ±âÈ­

	ReformCharForm();


	//¹Ù´Ú È®ÀÎ
	lpCurPlayer->OnStageField = -1;
	if (smGameStage[0]) {
		cnt = smGameStage[0]->GetFloorHeight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, lpCurPlayer->Pattern->SizeHeight);
		lpCurPlayer->OnStageField = 0;
	}
	if (cnt == CLIP_OUT && smGameStage[1]) {
		lpCurPlayer->OnStageField = 1;
	}


	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		chrOtherPlayer[cnt].Init();
	}


	smMATERIAL *mat;

	smASE_SetPhysique(0);
	smRender.LoadUsingNewRender = true;
	PatArrow = smASE_Read("weapons\\arrow.ase");
	PatSparkShield = smASE_Read("weapons\\Mc3_sparkShield.ASE");
	PatDivineShield = smASE_Read("weapons\\Nt3_DivineI.ASE");
	PatGodlyShield = smASE_Read("weapons\\Godly_Shield.ASE");
	smRender.LoadUsingNewRender = false;
	
	PatEffectShield = smASE_Read("weapons\\Test8.ASE");
	PatCrown = smASE_Read("weapons\\Crown.ASE");				//ºí·¹½ºÄ³½½ ¿Õ°ü

	//PatEffectShield = smASE_Read( "weapons\\arrow.ase" );

	if (PatEffectShield) {
		PatEffectShield->Frame = 0;

		mat = PatEffectShield->smMaterialGroup->FindMaterialFromTextureName(szBangImage);
		if (mat) {
			mat->BlendType = SMMAT_BLEND_ADDCOLOR;
			mat->SelfIllum = 1;
			/*
			PatEffectShield->smMaterialGroup->ChangeMaterialToAnimation(
				mat, szAnimShieldEffectFile , 4 , SMMAT_BLEND_LAMP , TRUE );

			mat->TextureFormState[0] = 0;
			mat->Shift_FrameSpeed = 8;
			*/

		}
	}

	if (PatCrown) {
		PatCrown->Frame = 0;

		mat = PatCrown->smMaterialGroup->FindMaterialFromTextureName(szClowImage);
		if (mat) {
			mat->BlendType = SMMAT_BLEND_ADDCOLOR;
			mat->SelfIllum = 1;
		}
	}

	/*
		// ÇÇÁöÅ© µ¥ÀÌÅ¸ ÃÊ±âÈ­
		smASE_SetPhysique( 0 );

		smModelDecode( smConfig.szFile_Enemy , &modelInfo );
		TmPenguin=smASE_ReadBone( modelInfo.szMotionFile );		// ¾Ö´Ï¸ÞÀÌ¼Ç ÀÐ±â
		PatPenguin=smASE_Read( modelInfo.szModelFile , modelInfo.szModelName );

		for( cnt=0;cnt<3;cnt++) {
			chrPenguin[cnt].Init();
			chrPenguin[cnt].SetPosi( smConfig.Posi_Enemy.x  , 0, smConfig.Posi_Enemy.y ,0,0,0 );
			chrPenguin[cnt].SetPattern( PatPenguin );
			chrPenguin[cnt].SetAnimatioInfo( &modelInfo );
			chrPenguin[cnt].SetTargetPosi( cnt*100 , cnt*100 );
			chrPenguin[cnt].ActionPattern = 1;
		}
	*/
	// ÇÇÁöÅ© µ¥ÀÌÅ¸ ÃÊ±âÈ­
	smASE_SetPhysique(0);

	//Å×µÎ¸®¾ó±¼ ¹Ú½º
	MatEachBox = CreateTextureMaterial("image\\cw.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	//¸Þ´º ¹öÆ° Å×µÎ¸®
	MatEachMenuBox = CreateTextureMaterial("image\\cw-1.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEachTradeButton[0] = CreateTextureMaterial("image\\CW-trade0.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEachTradeButton[1] = CreateTextureMaterial("image\\CW-trade1.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	//ÆÄÆ¼ Æ®·¹ÀÌµå ÁýÇÕ
	lpDDS_ParTradeButton[0][0] = LoadDibSurfaceOffscreen("image\\party\\icon-c5_.bmp");
	lpDDS_ParTradeButton[0][1] = LoadDibSurfaceOffscreen("image\\party\\icon-c5.bmp");

	lpDDS_ParTradeButton[1][0] = LoadDibSurfaceOffscreen("image\\party\\icon-c1_.bmp");
	lpDDS_ParTradeButton[1][1] = LoadDibSurfaceOffscreen("image\\party\\icon-c1.bmp");

	lpDDS_ParTradeButton[2][0] = LoadDibSurfaceOffscreen("image\\party\\icon-c2_.bmp");
	lpDDS_ParTradeButton[2][1] = LoadDibSurfaceOffscreen("image\\party\\icon-c2.bmp");

	lpDDS_ParTradeButton[3][0] = LoadDibSurfaceOffscreen("image\\party\\icon_clan_.bmp");
	lpDDS_ParTradeButton[3][1] = LoadDibSurfaceOffscreen("image\\party\\icon_clan.bmp");


	lpDDS_ParTradeToolTip[0] = LoadDibSurfaceOffscreen("image\\party\\ButtonInfo-c5.bmp");
	lpDDS_ParTradeToolTip[1] = LoadDibSurfaceOffscreen("image\\party\\ButtonInfo-c1.bmp");
	lpDDS_ParTradeToolTip[2] = LoadDibSurfaceOffscreen("image\\party\\ButtonInfo-c0.bmp");
	lpDDS_ParTradeToolTip[3] = LoadDibSurfaceOffscreen("image\\party\\ButtonInfo-c10.bmp");

	MatEnergyBox[0] = CreateTextureMaterial("game\\images\\UI\\target\\target.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEnergyBox[1] = CreateTextureMaterial("game\\images\\UI\\target\\targethp.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEnergyBox[2] = CreateTextureMaterial("game\\images\\UI\\target\\targetbackground.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	MatEnergyBox[3] = CreateTextureMaterial("image\\Energy_Red.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEnergyBox[4] = CreateTextureMaterial("image\\Energy_Blue.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEnergyBox[5] = CreateTextureMaterial("game\\images\\NewGUI\\target\\infobackground.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEnergyBox[6] = CreateTextureMaterial("game\\images\\NewGUI\\target\\more.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatEnergyBox[7] = CreateTextureMaterial("game\\images\\NewGUI\\target\\more_.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);


	emoticons[0] = CreateTextureMaterial("Emoji\\awn.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	//Á¶¸í
	MatLight[0] = CreateTextureMaterial("image\\glow01.bmp", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ADDCOLOR);
	smMaterial[MatLight[0]].SelfIllum = TRUE;
	MatLight[1] = CreateTextureMaterial("image\\glow02.bmp", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ADDCOLOR);
	smMaterial[MatLight[1]].SelfIllum = TRUE;

	MatMonBroodImage[0] = CreateTextureMaterial("game\\images\\NewGUI\\type\\M_Nm.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[1] = CreateTextureMaterial("game\\images\\NewGUI\\type\\M_Mt.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[2] = CreateTextureMaterial("game\\images\\NewGUI\\type\\M_Un.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[3] = CreateTextureMaterial("game\\images\\NewGUI\\type\\M_D.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[4] = CreateTextureMaterial("game\\images\\NewGUI\\type\\M_mc.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[5] = CreateTextureMaterial("game\\images\\NewGUI\\type\\npc.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[6] = CreateTextureMaterial("game\\images\\NewGUI\\type\\player.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[7] = CreateTextureMaterial("game\\images\\NewGUI\\type\\background.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	MatMonBroodImage[8] = CreateTextureMaterial("game\\images\\NewGUI\\type\\cristal.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	iMask = CreateTextureMaterial("image\\MiniMap_Mask.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	//Clima Mapas xxstr
	InitWeatherSystem();

	//ÆÄÆ¼Å¬ ÃÊ±âÈ­
	InitParticle();
	//µ¿Àû ±¤¿ø ÃÊ±âÈ­
	InitDynLight();

	UseEtherCoreCode = 0;			//±ÍÈ¯ ¾ÆÀÌÅÛ »ç¿ëÄÚµå

	dwPartyPosionTime = 0;			//µ¿·á È¸º¹ ¿¹¾à ½Ã°£

	//¿¹¾à ÀÌÆåÆ® ÃÊ±âÈ­
	InitTimeEffect();

#ifdef	_SET_PROCTECT_MEM2
	//PlaySub Æã¼ÇÄÚµå Á¶ÀÛ È®ÀÎ
	cnt = Check_PlaySubReleaseFunc();
	if (cnt) {
		SendSetHackUser2(1120, cnt);
	}
#endif

	ResetEnergyGraph(3);
	ResetInvenItemCode();				//ÀÌº¥ ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
	ResetInvenMoney();					//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­

	CheckDLL_Module();

	ZeroMemory(&rsBlessCastle, sizeof(rsBLESS_CASTLE));		//ºí·¹½º Ä³½½ °ø¼ºÁ¤º¸ ÃÊ±âÈ­

	return TRUE;

}

int ClosePat3D()
{
	int cnt;

	ClosePatterns();

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	if (PatArrow)
	{
		delete PatArrow;
		PatArrow = NULL;
	}

	if (PatEffectShield)
	{
		delete PatEffectShield;
		PatEffectShield = NULL;
	}
	//######################################################################################

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	for (cnt = 0; cnt < ICE_FOOT_PAT_MAX; cnt++)
	{
		if (PatIceFoot[cnt])
		{
			delete PatIceFoot[cnt];
			PatIceFoot[cnt] = NULL;
		}
	}
	//######################################################################################


	for (cnt = SHADOW_PAT_MAX - 1; cnt >= 0; cnt--)
		if (PatShadow[cnt]) delete PatShadow[cnt];

	//ÆÄÆ¼ Æ®·¹ÀÌµå ¹öÆ° Á¦°Å
	if (lpDDS_ParTradeButton[0][0])	lpDDS_ParTradeButton[0][0]->Release();
	if (lpDDS_ParTradeButton[0][1])	lpDDS_ParTradeButton[0][1]->Release();
	if (lpDDS_ParTradeButton[1][0])	lpDDS_ParTradeButton[1][0]->Release();
	if (lpDDS_ParTradeButton[1][1])	lpDDS_ParTradeButton[1][1]->Release();
	if (lpDDS_ParTradeButton[2][0])	lpDDS_ParTradeButton[2][0]->Release();
	if (lpDDS_ParTradeButton[2][1])	lpDDS_ParTradeButton[2][1]->Release();
	if (lpDDS_ParTradeButton[3][0])	lpDDS_ParTradeButton[3][0]->Release();
	if (lpDDS_ParTradeButton[3][1])	lpDDS_ParTradeButton[3][1]->Release();

	if (lpDDS_ParTradeToolTip[0])	lpDDS_ParTradeToolTip[0]->Release();
	if (lpDDS_ParTradeToolTip[1])	lpDDS_ParTradeToolTip[1]->Release();
	if (lpDDS_ParTradeToolTip[2])	lpDDS_ParTradeToolTip[2]->Release();
	if (lpDDS_ParTradeToolTip[3])	lpDDS_ParTradeToolTip[3]->Release();

	if (sBiInfo) delete sBiInfo;

	return TRUE;
}

int PartCnt = 0;

int		MsTraceCount = 0;
int		MsTraceMode = 0;
RECT	MsSelRect;
POINT	MsSelPos;

int		SelMouseButton = 0;

smCHAR	*lpCharSelPlayer = 0;
smCHAR	*lpCharMsTrace = 0;

scITEM	*lpSelItem = 0;
scITEM	*lpMsTraceItem = 0;

int AttFlag = 0;
int	AttCancel;
int	AttAutoCont = 0;

extern smCHAR* NpcPremium;

int TraceAttackPlay()
{
	if (lpCharSelPlayer) 
	{
		MsTraceMode = TRUE;
		lpCharMsTrace = lpCharSelPlayer;
		AttFlag = TRUE;
		if (VRKeyBuff[VK_SHIFT] && lpCharSelPlayer->smCharInfo.State == smCHAR_STATE_ENEMY) AttAutoCont = TRUE;
		else AttAutoCont = FALSE;

		if (VRKeyBuff['O'] && smConfig.DebugMode)
			SkillWarriorBetserker(lpCharSelPlayer, 15);

		return TRUE;
	}

	if (lpSelItem) 
	{
		MsTraceMode = TRUE;
		lpMsTraceItem = lpSelItem;
		return TRUE;
	}

	return FALSE;
}

int CancelAttack()
{
	SelMouseButton = 0;
	MsTraceMode = FALSE;
	lpCharMsTrace = 0;
	lpMsTraceItem = 0;
	AttFlag = 0;

	return TRUE;
}

int	DamageExp(smCHAR *lpChar, int AttackDamage)
{
	int AttackResult;
	if (AttackDamage && lpChar->smCharInfo.State == smCHAR_STATE_ENEMY &&
		lpChar->smCharInfo.Brood != smCHAR_MONSTER_USER && lpChar->smCharInfo.Life[0] > 0) {

		AttackResult = (AttackDamage << 8) / lpChar->smCharInfo.Life[1];
		if (AttackResult > 256) AttackResult = 256;
		AttackResult = (lpChar->smCharInfo.Exp * AttackResult) >> 8;
		AttackResult /= 10;
		if (AttackResult <= 0) AttackResult = 1;
		lpCurPlayer->AttackExp = AttackResult;
	}

	return TRUE;
}

int AutoDeathCounter = 0;
int FixedWidth = 200;
DWORD	dwClassDelayTime = 0;

int PlayPat3D()
{
	int cnt;
	int attack;
	int	attack_UserMonster;
	int	attack_RegState;

	int wx, wz, x, z;
	int	attackMode;
	sSKILL *lpAttackSkill;
	smWINSOCK *lpsmSock;
	int	skFlag;

#ifdef	_WINMODE_DEBUG

	if (SkipNextField) {
		if (SkipNextField == 1) {
			WarpNextField(&wx, &wz);
			lpCurPlayer->SetPosi(wx, 0, wz, 0, 0, 0);
			SkipNextField = 0;
			TraceCameraPosi.x = lpCurPlayer->pX;
			TraceCameraPosi.y = lpCurPlayer->pY;
			TraceCameraPosi.z = lpCurPlayer->pZ;
			TraceTargetPosi.x = lpCurPlayer->pX;
			TraceTargetPosi.y = lpCurPlayer->pY;
			TraceTargetPosi.z = lpCurPlayer->pZ;
		}
		else {
			//Ä¿½ºÅÒ ÇÊµå·Î ÀÌµ¿
			WarpCustomField(&wx, &wz);
			lpCurPlayer->SetPosi(wx, 0, wz, 0, 0, 0);
			SkipNextField = 0;
			TraceCameraPosi.x = lpCurPlayer->pX;
			TraceCameraPosi.y = lpCurPlayer->pY;
			TraceCameraPosi.z = lpCurPlayer->pZ;
			TraceTargetPosi.x = lpCurPlayer->pX;
			TraceTargetPosi.y = lpCurPlayer->pY;
			TraceTargetPosi.z = lpCurPlayer->pZ;

		}
	}

#endif

	lpCurPlayer->Main();

	EachPlayerMain();

	AutoDeathCounter++;

	if (PkMode > 0) PkMode--;			//PK¸ðµå ½Ã°£ Ä«¿îÅÍ

#ifdef _LANGUAGE_BRAZIL
	if (VRKeyBuff[VK_CONTROL]) PkMode = FALSE;
#else
	if (VRKeyBuff[VK_CONTROL]) PkMode = TRUE; // libera atk clan em BC xxstr
#endif

	else PkMode = FALSE;


	//½ºÅÏ °ø°Ý ¹Þ¾Ò´Ù
	if (lpCurPlayer->PlayStunCount) {
		if (MsTraceMode) {
			CancelAttack();
			lpCurPlayer->MoveFlag = 0;
			MouseButton[0] = 0;
			MouseButton[1] = 0;
			MouseButton[2] = 0;
		}
		if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_WALK || lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_RUN) {
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_STAND);
			CancelAttack();
			lpCurPlayer->MoveFlag = 0;
			MouseButton[0] = 0;
			MouseButton[1] = 0;
			MouseButton[2] = 0;
		}
		lpCurPlayer->dwNextMotionCode = 0;
		lpCurPlayer->MotionLoop = 0;
	}


	if (MsTraceMode && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_ATTACK && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_EAT &&
		lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_SKILL) {

		attackMode = FALSE;
		AttCancel = FALSE;
		attack_UserMonster = FALSE;

		CheckPlaySubFunc2();

		if ((MsTraceCount & 0x1F) == 0) {
			//¸¶¿ì½º·Î ÂïÀº Ä³¸¯ÅÍ µû¶ó°¨
			//lpCurPlayer->SetTargetPosi( 0,0 );
			lpCurPlayer->MoveFlag = TRUE;
			lpCurPlayer->Angle.y = GetMouseSelAngle();
		}

		int AttackRange = 0;
		int	AttackResult = 0;

		lpAttackSkill = 0;

		switch (SelMouseButton) 
		{
		case 1:
			if (sinSkill.pLeftSkill && sinSkill.pLeftSkill->CODE != SKILL_NORMAL_ATTACK)
				lpAttackSkill = sinSkill.pLeftSkill;
			break;
		case 2:
			if (sinSkill.pRightSkill && sinSkill.pRightSkill->CODE != SKILL_NORMAL_ATTACK)
				lpAttackSkill = sinSkill.pRightSkill;
			break;
		}

		// não ataca em cima da montaria
		if ((lpCurPlayer->pMount) && lpCurPlayer->pMount->IsVisible())
		{
			if (lpCharMsTrace)
			{
				if (lpCharMsTrace->smCharInfo.State == smCHAR_STATE_ENEMY)
				{
					lpAttackSkill = 0;
					attackMode = FALSE;
					AttCancel = FALSE;
					attack_UserMonster = FALSE;
					CancelAttack();
				}
			}
		}

		if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
		{
			lpAttackSkill = 0;
		}

		if (lpCharMsTrace) {
			if (lpCharMsTrace->smCharInfo.Life[0] <= 0 ||
				(lpCharMsTrace->dwLastTransTime + DISPLAY_TRANS_TIME_OVER) < dwPlayTime ||
				cTrade.OpenFlag) {
				//ÀÌ¹Ì Á×¾úÀ½ / È­¸é ¹Û¿¡¼­ »ç¶÷Áü
				CancelAttack();
			}
			else {
				//if ( !MouseButton[0] ) AttFlag = 0;
				if (!MouseButton[0] && !MouseButton[1]) AttFlag = 0;

				if (lpCurPlayer->smCharInfo.State == smCHAR_STATE_ENEMY) {
					//ÁÖÀÎ°ø ¸ó½ºÅÍ ½ºÅ²
					attackMode = TRUE;
				}
				else {
					if (PkMode)
						attackMode = TRUE;
					//VRKeyBuff[VK_CONTROL];
				}

				if (lpCharMsTrace->smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU) {
					AttackRange = 130 * fONE;							//¸Ç¼Ó °ø°Ý °Å¸® 
				}
				else
					AttackRange = 50 * fONE;							//¸Ç¼Ó °ø°Ý °Å¸® 

				if (lpCurPlayer->dwActionItemCode) {
					AttackRange += lpCurPlayer->AttackToolRange;	//¹«±â ±æÀÌ ´õÇÔ
				}

				//¿ø°Å¸® °ø°Ý °Å¸®
				if (lpCurPlayer->ShootingMode && lpCharMsTrace->smCharInfo.State) {
					if (lpCharMsTrace->smCharInfo.State != smCHAR_STATE_USER || attackMode)
						AttackRange = lpCurPlayer->smCharInfo.Shooting_Range*fONE;

					//ÀÓ½Ã ¸ð¶óÀÌ¿Â ÁöÆÎÀÌ »çÁ¤°Å¸®
					//if ( lpCurPlayer->smCharInfo.JOB_CODE>=7 ) 
					//	AttackRange = 60*fONE;
				}

				if (lpCharMsTrace->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B)
					AttackRange += 40 * fONE;	//¹ßÇÒ¶óÅ¾ ±æÀÌ ´õÇÔ

				attack = lpCharMsTrace->smCharInfo.State;

				if (lpCharMsTrace->smCharInfo.State == smCHAR_STATE_USER) {
					if (!attackMode ||
						(lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE))
						attack = 0;
				}

				if (lpCharMsTrace->smCharInfo.State == smCHAR_STATE_ENEMY && lpCharMsTrace->smCharInfo.Brood == smCHAR_MONSTER_USER) {
					if (!PkMode) {
						attack_UserMonster = TRUE;;
						attack = 0;
					}
				}

				if (lpAttackSkill) {
					if (lpAttackSkill->CODE == SKILL_HEALING || lpAttackSkill->CODE == SKILL_ENCHANT_WEAPON ||
						lpAttackSkill->CODE == SKILL_VIRTUAL_LIFE || lpAttackSkill->CODE == SKILL_TRIUMPH_OF_VALHALLA)
					{
						if (attack_UserMonster && lpAttackSkill->CODE == SKILL_HEALING)		//¼ÒÈ¯¸ó½ºÅÍ Èú¸µ °¡´ÉÇÏµµ·Ï Ã³¸®
							attack_UserMonster = 0;

						skFlag = 1;
					}
					else
						skFlag = 0;

					if ((lpCharMsTrace->smCharInfo.State == smCHAR_STATE_ENEMY && !attack_UserMonster) ||
						(PkMode && lpCharMsTrace->smCharInfo.State == smCHAR_STATE_USER) || skFlag) {

						//½ºÅ³ Àû¿ë °Å¸®¸¦ ±¸ÇÑ´Ù
						cnt = GetSkillDistRange(lpAttackSkill, AttackRange, attack);
						if (cnt)
							AttackRange = cnt;

						attack = 0;			//½ºÅ³Àº ±äº» °ø°Ý ¿ÀÇÁ
					}
				}

				if (!lpCurPlayer->ShootingFlag &&
					lpCurPlayer->PlayAttackFromPosi(
						lpCharMsTrace->pX,
						lpCharMsTrace->pY,
						lpCharMsTrace->pZ,
						AttackRange, attack) == TRUE &&
					lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_EAT) {//&& 
					//!attack_UserMonster ) {

					LastAttackDamage = 0;			//ÃÖ½Å °ø°Ý µ¥¹ÌÁö ÃÊ±âÈ­
					AutoMouse_Attack_Count++;		//¿ÀÅä¸¶¿ì½º °¨Áö¿ë Ä«¿îÅÍ

					if (lpCharMsTrace->smCharInfo.State == smCHAR_STATE_USER) {
						if (lpCharMsTrace->dwTradeMsgCode) {		//°³ÀÎ»óÁ¡ Á¤º¸ ¿ÀÇÂ
							Send_ShowMyShopItem(lpCharMsTrace);
						}
						else {
							EachTradeButton = 1;

#ifdef  __CLANSUBCHIP__
							if ((cldata.myPosition == 101 || cldata.myPosition == 104) && !lpCharMsTrace->smCharInfo.ClassClan)
#else
							if (cldata.myPosition == 101 && !lpCharMsTrace->smCharInfo.ClassClan)
#endif
								EachTradeButtonMode = TRUE;						//Å¬·£ÀåÀÌ Å¬·£¿ø ¸ðÁý
							else
								EachTradeButtonMode = FALSE;					//Å¬·£ ¸ðÁý ¸Þ´º ±ÝÁö
						}

					}
					else {
						EachTradeButton = 0;
					}

					if (!lpCharMsTrace->smCharInfo.State) {
						OpenEachPlayer(lpCharMsTrace);
						DispEachMode = TRUE;
					}

					lpCurPlayer->chrAttackTarget = lpCharMsTrace;
					lpCurPlayer->AttackExp = 0;
					/*
						if ( ((lpCurPlayer->dwActionItemCode^sinWM1)&sinITEM_MASK2)!=0 &&	//¸¶¹ýÁöÆÎÀÌ Å©¸®Æ¼ÄÃ ¾øÀ½
							(rand()%100)<sinGetCritical( lpCharMsTrace->smCharInfo.Level ) ) {

							lpCurPlayer->AttackCritcal = TRUE;			//Å©¸®Æ¼ÄÃ °ø°Ý ¼º°ø
						}
						else
							lpCurPlayer->AttackCritcal = FALSE;
					*/
					lpCurPlayer->AttackCritcal = FALSE;

					//¸¶À» ¿Ü¿¡¼­¸¸ °ø°Ý µÊ
					if (!attack_UserMonster &&
						(lpCharMsTrace->smCharInfo.State != smCHAR_STATE_USER ||
						(lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_VILLAGE))) {

						AttackResult = TRUE;
						
						
						if (!PlaySkillAttack(lpAttackSkill, lpCharMsTrace)) {
							
							if (attack || lpCharMsTrace->smCharInfo.State != smCHAR_STATE_USER) {

								if (lpCurPlayer->ShootingMode && lpCharMsTrace->smCharInfo.State != smCHAR_STATE_NPC) {
									if (lpCurPlayer->CheckShootingTest(lpCharMsTrace) == TRUE) {
										AttackResult = dm_SendTransDamage(lpCharMsTrace, 0, 0, 0, 0,
											lpCurPlayer->AttackSkil, TRUE);

									}
									else {
										lpCurPlayer->AttackCritcal = -1;
										lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_STAND);
									}
								}
								else {
									attack_RegState = 0;
									
									if (lpCharMsTrace->smCharInfo.State == smCHAR_STATE_NPC) {
										if (sinQuest_ChangeJob2.CODE) {
											attack_RegState = (sinQuest_ChangeJob2.CODE << 16);
											if (sinQuest_ChangeJob2.CODE == SIN_QUEST_CODE_CHANGEJOB2_NPC_M ||
												sinQuest_ChangeJob2.CODE == SIN_QUEST_CODE_CHANGEJOB2_NPC_D) {
												if (!cInvenTory.SearchItemCode(sinMA1 | sin01) &&
													!cInvenTory.SearchItemCode(sinQT1 | sin04) &&
													!cInvenTory.SearchItemCode(sinQT1 | sin05))
													attack_RegState |= 1;

											}
										}
										
										//CÓDIGO DO NPC AQUI
										if (dwClassDelayTime < dwPlayTime) {
											if (lpCharMsTrace->smCharInfo.wPlayClass[0]) {
												switch (lpCharMsTrace->smCharInfo.wPlayClass[0]) {
													//NPC Äù½ºÆ® Ã³¸®
												case 1001:
													if (lpCurPlayer->smCharInfo.JOB_CODE <= 4)
														sinCheckLevelQuest30();
													break;
												case 1002:
													if (lpCurPlayer->smCharInfo.JOB_CODE > 4)
														sinCheckLevelQuest30();
													break;
												case 1003:
													if (lpCurPlayer->smCharInfo.JOB_CODE <= 4)
														sinCheckLevelQuest55();
													break;
												case 1004:
													if (lpCurPlayer->smCharInfo.JOB_CODE > 4)
														sinCheckLevelQuest55_2();
													break;
												case 1005:
													sinCheckLevelQuest70();
													sinCheckLevelQuest80();
													sinCheckLevelQuest85();
													break;
												case 1006:
													sinCheckLevelQuest90();
													break;

												case 1010:
													sinCheckLevelQuest80_2(0);
													break;
												case 1011:
													sinCheckLevelQuest80_2(1);
													break;
												case 1012:
													sinCheckLevelQuest80_2(0);
													sinCheckLevelQuest90_2(0);
													break;
												case 1013:
													sinCheckLevelQuest90_2(1);
													chaQuest.showFuryQuest();
													break;
												/*case 1015:// open npc dailyquest
													sinCheckDailyQuest(); 
													break;*/
												}
								
												//HaÄù½ºÆ® NPC ¸¸³µÀ»¶§ÀÇ Ã³¸®
												HaCheckQuestNpc(lpCharMsTrace->smCharInfo.wPlayClass[0]);

												dwClassDelayTime = dwPlayTime + 1000 * 5;		//5ÃÊ µô·¹ÀÌ
											}
										}
										//}

									}
									
									//lpCharMsTrace = NpcPremium;

									//ABRE NPC AQUI
								    AttackResult = dm_SendTransDamage(lpCharMsTrace, 0, 0, 0, attack_RegState,
									lpCurPlayer->AttackSkil, TRUE);
							
								}

								// Àåº° - ½ºÅ³ ¼öÁ¤
								if ((AttackResult && lpCurPlayer->WeaponEffect == (sITEMINFO_LIGHTING + 1) || AttackResult && lpCurPlayer->WeaponEffect == (sITEMINFO_ICE + 1) || AttackResult && lpCurPlayer->WeaponEffect == (sITEMINFO_FIRE + 1))
									&& lpCurPlayer->EnchantEffect_Point > 0 && lpCurPlayer->EnchantEffect_Point <= 10) {
									//¶óÀÌÆ®´× ¼Ó¼ºÀº ¹üÀ§ °ø°Ý Ãß°¡
									/*
									EnchantPower = GetRandomPos(
										Enchant_Weapon_Damage_Lightning[lpCurPlayer->EnchantEffect_Point-1][0] ,
										Enchant_Weapon_Damage_Lightning[lpCurPlayer->EnchantEffect_Point-1][1] );

									Skil_RangeAttack( lpCharMsTrace->pX, lpCharMsTrace->pY, lpCharMsTrace->pZ, 50*fONE , EnchantPower , 105 );
									*/
									if (lpCurPlayer->AttackCritcal >= 0) {
										//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
										dm_SelectRange(lpCharMsTrace->pX, lpCharMsTrace->pY, lpCharMsTrace->pZ, 50, TRUE);
										//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
										dm_SendRangeDamage(lpCharMsTrace->pX, lpCharMsTrace->pY, lpCharMsTrace->pZ, 0, 0, 0, 0, 0,
											(SKILL_PLAY_ENCHANT_WEAPON | (lpCurPlayer->EnchantEffect_Point << 8)));
									}
								}
							}
							if (!lpAttackSkill && !AttackResult) {
								lpCurPlayer->AttackCritcal = -1;
							}
							else {
								//µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ »ó½Â
								DamageExp(lpCharMsTrace, AttackResult);
							}
						}
					}
					/*
					if ( LastAttackDamage==0 ) {
						lpCurPlayer->AttackCritcal = -1;
					}
					*/
					
					if (!AttackResult)	lpCurPlayer->AttackCritcal = -1;

					if (!AttCancel) {

						lpCurPlayer->MoveFlag = FALSE;
						lpCurPlayer->CriticalLog = lpCurPlayer->AttackCritcal;

						if (lpCharMsTrace && (lpCharMsTrace->smCharInfo.State == smCHAR_STATE_NPC || lpCharMsTrace->dwTradeMsgCode)) {
							MouseButton[0] = 0;
							MouseButton[1] = 0;
							AttAutoCont = FALSE;
						}

						if (!AttFlag && !AttAutoCont) {
							MsTraceMode = 0;
							lpCharMsTrace = 0;
						}
					}
				}
				else {
					if (!lpCharMsTrace->Flag || lpCharMsTrace->MotionInfo->State == CHRMOTION_STATE_DEAD) {
						lpCharMsTrace = NULL;
						MsTraceMode = 0;
						lpCurPlayer->MoveFlag = FALSE;
						AttAutoCont = FALSE;
					}
				}
			}
		}
		
		if (lpMsTraceItem) {
			if (lpCurPlayer->PlayAttackFromPosi(
				lpMsTraceItem->pX,
				lpMsTraceItem->pY,
				lpMsTraceItem->pZ,
				8000, FALSE) == TRUE)
			{
				smTRANS_ACITON_ITEM		TransActionItem;
				char	szBuff[64];
				int		len;

				TransActionItem.code = smTRANSCODE_PUTITEM;
				TransActionItem.size = sizeof(smTRANS_ACITON_ITEM);

				TransActionItem.x = lpMsTraceItem->pX;
				TransActionItem.y = lpMsTraceItem->pY;
				TransActionItem.z = lpMsTraceItem->pZ;
				TransActionItem.lpStgArea = lpMsTraceItem->lpStgArea;


				
				if (CheckRecvTrnsItemQue() == TRUE) {

					cnt = 0;

					if ((lpMsTraceItem->ItemCode&sinITEM_MASK2) == sinCH1) {
						switch (lpMsTraceItem->ItemCode) {
#ifdef _LANGUAGE_ENGLISH
						case (sinCH1 | sin01):
							if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ATALANTA ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS) {

								cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_MALE);
								cnt = 1;
							}
							break;
						case (sinCH1 | sin02):
							if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_FIGHTER ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PIKEMAN ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MAGICIAN) {

								cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_FEMALE);
								cnt = 1;
							}
							break;
#else
						case (sinCH1 | sin01):
						case (sinCH1 | sin02):
							//ÃÊÄÚ·¿
							if (lpCurPlayer->smCharInfo.JOB_CODE != JOBCODE_ATALANTA &&
								lpCurPlayer->smCharInfo.JOB_CODE != JOBCODE_ARCHER &&
								lpCurPlayer->smCharInfo.JOB_CODE != JOBCODE_PRIESTESS) {

								cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_FEMALE);
								cnt = 1;
							}
							break;
#endif
						case (sinCH1 | sin03):
						case (sinCH1 | sin04):
							//»çÅÁ
							if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ATALANTA ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER ||
								lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS) {

								cMessageBox.ShowMessage(MESSAGE_NOT_PICKUP_MALE);
								cnt = 1;

							}
							break;


						}
					}

					if (!cnt) {

						// Pega ouro do chão aqui
						if (lpMsTraceItem->ItemCode == (sinGG1 | sin01)) 
						{
							lstrcpy(szBuff, lpMsTraceItem->szName);
							len = lstrlen(szBuff);
							for (cnt = 0; cnt < len; cnt++) {
								if (szBuff[cnt]<'0' || szBuff[cnt]>'9') {
									szBuff[cnt] = 0;
									break;
								}
							}
							cnt = atoi(szBuff);			//ÀÌ¸§¿¡¼­ ±Ý¾×À» ¾ò´Â´Ù
							if (cInvenTory.CheckMoneyLimit(cnt) == TRUE) {
								lpsmSock = GetServerSock(lpMsTraceItem->ServerCode);		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
								if (lpsmSock) {
									lpsmSock->Send((char *)&TransActionItem, TransActionItem.size, TRUE);
								}
							}
						}
						else {
							lpsmSock = GetServerSock(lpMsTraceItem->ServerCode);		//Áö¿ª¼­¹ö ¼ÒÄÏÀ» Ã£À½
							if (lpsmSock) {
								lpsmSock->Send((char *)&TransActionItem, TransActionItem.size, TRUE);
							}
							
						}
						
					}
					
				}
				
				MsTraceMode = 0;
				lpMsTraceItem = NULL;
				lpCurPlayer->MoveFlag = FALSE;
				MouseButton[0] = 0;
			}
			else {
				if (!lpMsTraceItem->Flag) {
					lpMsTraceItem = NULL;
					MsTraceMode = 0;
					lpCurPlayer->MoveFlag = FALSE;
				}
			}
		}


	}
	MsTraceCount++;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag) {
			chrOtherPlayer[cnt].Main();
			if (chrOtherPlayer[cnt].dwTarget) {
				if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
					chrOtherPlayer[cnt].MotionInfo->State == CHRMOTION_STATE_ATTACK)
					//					lpCurPlayer->ChangeMotion( 67 );
					lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
			}
		}
	}


#ifdef _WINMODE_DEBUG
	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	//if ( smConfig.DebugMode && VRKeyBuff[VK_SHIFT] ) {
	if (smConfig.DebugMode && VRKeyBuff[VK_SHIFT] && !VRKeyBuff[VK_CONTROL]) {
		//######################################################################################
		if (VRKeyBuff[VK_F4]) {
			SetNextPlayerPattern();
			VRKeyBuff[VK_F4] = 0;
		}
		if (VRKeyBuff[VK_F3]) {
			SetNextNpcPattern();
			VRKeyBuff[VK_F3] = 0;
		}
	}
#endif
	//DRZ_EDIT (remove for prevent crashing)
	//Check_CodeSafe2();				//ÇÁ·Î±×·¥ ÄÚµå°Ë»ç

	//ÇÊµå ¹Ì´Ï ¸Ê
	MainFieldMap();

	//ÆÄÆ¼Å¬ ½ÇÇà
	PlayParticle();
	//µ¿Àû ±¤¿ø ¸ÞÀÎ
	DynLightMain();

	BGMain();		//¹è°æ ¸ÞÀÎ

	//¹è°æÀ½¾Ç ¸ÞÀÎ
	BGM_Main();

	//SoD ¸ÞÀÎ
	SodScoreMain();


	PlayCounter++;
	//ÇÊµå ÀÚµ¿ º¯°æ ¸ÞÀÎ
	FieldMain(lpCurPlayer->pX, 0, lpCurPlayer->pZ);


	int fldSky, fflag;

	int	RainTime = 0x200;		//ºñ¿À´Â ½Ã°£
	int	RainLength = 60 * 7;		//7½Ã°£
	int	BackSel = 0;

	fldSky = -1;
	fflag = 0;

	if ((PlayCounter & 0xFF) == 0 && lpCurPlayer->dwObjectSerial) {
		/*
						if ( cldata.myPosition==100 || cldata.myPosition==101 ) {
							//Å¬·£ °¡ÀÔ¿ø
							lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
						}
						else
							lpCurPlayer->smCharInfo.ClassClan = 0;

						if ( cldata.myPosition )
							InitClanMode = cldata.myPosition;
		*/
		/*
				if ( !InitClanMode && cldata.myPosition ) {
					//Å¬·£ µ¥ÀÌÅ¸ ÀÔ¼ö
					if ( cldata.myPosition==100 || cldata.myPosition==101 ) {
						//Å¬·£ °¡ÀÔ¿ø
						lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
						lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
						SendSetObjectSerial( lpCurPlayer->dwObjectSerial );
					}
					InitClanMode = cldata.myPosition;
				}
		*/


		if (dwSoD_ResultTime < dwPlayTime) {
			//SoD »óÈ²Ã¢ ¶° ÀÖÀ½
			dwSoD_ResultTime = dwPlayTime + 12000;		//12ÃÊ °£°Ý
			if (cSinHelp.GetSodRankBoxState() == TRUE) {
				SendCommand_AreaServer(smTRANSCODE_SOD_RESULT2, 0, 0, 0, 0);		//SoD °á°ú Àü¼Û ¿ä±¸
			}
		}

		if (cldata.myPosition) {
			if (lpCurPlayer->smCharInfo.ClassClan != cldata.intClanMark) {
#ifdef  __CLANSUBCHIP__				
				if (cldata.myPosition == 100 || cldata.myPosition == 101 || cldata.myPosition == 104) {
#else
				if (cldata.myPosition == 100 || cldata.myPosition == 101) {
#endif
					//Å¬·£ °¡ÀÔ¿ø
					lpCurPlayer->smCharInfo.ClassClan = cldata.intClanMark;
					lpCurPlayer->dwClanManageBit = cldata.CNFlag;
					lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
					SendUpdateClanInfo();					//¼­¹ö¿¡ º¯°æµÈ Å¬·£ Á¤º¸ º¸³¿

					//SendSetObjectSerial( lpCurPlayer->dwObjectSerial );

					//Å×½ºÆ®
					//Å¬·£¿ø Á¤º¸ Àü¼Û¿ë ÇÔ¼ö
					//SendClanCommandUser( smWsockDataServer , smTRANSCODE_CLAN_UPDATE  , 0,0, cldata.clanWon , cldata.szRegiD char **szClanUserList , int ClanUserCount )

				}
				else {
					cldata.intClanMark = 0;
					lpCurPlayer->smCharInfo.ClassClan = 0;
					lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
					//SendSetObjectSerial( lpCurPlayer->dwObjectSerial );
					SendUpdateClanInfo();					//¼­¹ö¿¡ º¯°æµÈ Å¬·£ Á¤º¸ º¸³¿
				}
			}
		}


		if (lpCurPlayer->smCharInfo.wPlayerKilling[0] > 0) {
			lpCurPlayer->smCharInfo.wPlayerKilling[0]--;
			if (lpCurPlayer->smCharInfo.wPlayerKilling[0] == 0) {

				//¸¶À»¿¡¼­ ´Ù½Ã ½ÃÀÛ
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
		}

#ifndef _LANGUAGE_CHINESE	//À©µµ¿ì ¸ðµå
#ifndef _LANGUAGE_VEITNAM	//À©µµ¿ì ¸ðµå
#ifndef _LANGUAGE_PHILIPIN	//À©µµ¿ì ¸ðµå
#ifndef _LANGUAGE_ENGLISH	//À©µµ¿ì ¸ðµå
#ifdef _WINMODE_DEBUG
		if (!smConfig.DebugMode && (PlayCounter & 0x3FF) == 0) {
			if (GetSystemMetrics(SM_CXSCREEN) != WinSizeX) {
				DisconnectFlag = dwPlayTime;
				quit = TRUE;
			}
		}
#else
		if (GetSystemMetrics(SM_CXSCREEN) != WinSizeX) {
			DisconnectFlag = dwPlayTime;
			quit = TRUE;
		}
#endif
#endif
#endif
#endif
#endif

		if (WeatherMode == 0) {
			if (dwGameHour < GAME_HOUR_DAY || dwGameHour >= GAME_HOUR_NIGHT) {
				BackSel = 2;
			}
			else {
				if (dwGameHour >= GAME_HOUR_DAY && dwGameHour < GAME_HOUR_GLOW)
					BackSel = 0;
				else
					BackSel = 1;
			}
		}

		//¹è°æÀ½¾Ç ¹Ù²Ù±â
		if (lpCurPlayer->OnStageField >= 0) {

			if (StageField[lpCurPlayer->OnStageField]->FieldCode == rsSOD_FIELD) {
				//SOD ÇÊµå ÇÏ´Ã¼³Á¤
				cnt = rsGetSoD_AreaCode(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
				switch (cnt) {
				case 0:
				case 1:
				case 2:
				case 3:
					fldSky = FIELD_BACKIMAGE_SODNONE;
					break;

				case 4:
					fldSky = FIELD_BACKIMAGE_SODSUN;
					break;
				case 5:
					fldSky = FIELD_BACKIMAGE_SODMOON;
					break;
				case 6:
					fldSky = FIELD_BACKIMAGE_SODSKY;
					break;
				}
				if (cnt == 2)
					esPlayEffectBgm(4);		//¹Ù¶÷ÀÇ ¹æ
				else
					esPlayEffectBgm(3);		//ÀÏ¹Ý ¹æ

				if (fldSky >= 0) ChangeSky(fldSky);
			}
			else if (StageField[lpCurPlayer->OnStageField]->FieldCode == BabelPlayField && dwBabelPlayTime > dwPlayTime) {
				//¾ÆÀÌ¾ð ¹Ùº§ ÃâÇö
				ChangeSky(FIELD_BACKIMAGE_IRONBOSS);
				esPlayEffectBgm(11);		//¹Ùº§
			}
			else {
				fldSky = StageField[lpCurPlayer->OnStageField]->BackImageCode[BackSel];

				if (!WeatherMode && fldSky >= 0) ChangeSky(fldSky);	//ChangeBG(fldSky);

				if (WeatherMode == 0) {
					switch (StageField[lpCurPlayer->OnStageField]->State) {
						//¹è°æ È¿°úÀ½ (¹Ù¶÷¼Ò¸®)
					case FIELD_STATE_RUIN:
						esPlayEffectBgm(3);
						break;

					case FIELD_STATE_DESERT:
						esPlayEffectBgm(4);
						break;

					case FIELD_STATE_DUNGEON:
						if (StageField[lpCurPlayer->OnStageField]->FieldCode <= 16 ||
							StageField[lpCurPlayer->OnStageField]->FieldCode == 32) {
							esPlayEffectBgm(5);		//´øÀü
							break;
						}
						if (StageField[lpCurPlayer->OnStageField]->FieldCode <= 23) {
							esPlayEffectBgm(6);		//½ÅÀü
							break;
						}
						if (StageField[lpCurPlayer->OnStageField]->FieldCode <= 25) {
							esPlayEffectBgm(7);		//µ¿±¼
							break;
						}
						if (StageField[lpCurPlayer->OnStageField]->FieldCode <= 26) {
							esPlayEffectBgm(8);		//¼ºÀü
							break;
						}
						break;


					case FIELD_STATE_FOREST:
						if (dwGameHour < GAME_HOUR_DAY || dwGameHour >= GAME_HOUR_DARKNESS) {
							esPlayEffectBgm(2);			//¹ã
						}
						else {
							esPlayEffectBgm(1);			//³·
						}
						break;

					case FIELD_STATE_VILLAGE:
						esPlayEffectBgm(0);
						break;

					case FIELD_STATE_ROOM:
						esPlayEffectBgm(-1);
						break;

					case FIELD_STATE_IRON:
						esPlayEffectBgm(9);		//¾ÆÀÌ¾ð
						break;

					case FIELD_STATE_ICE:
						esPlayEffectBgm(12);
						break;

					}
				}
				else
					esPlayEffectBgm(-1);

				if (StageField[lpCurPlayer->OnStageField]->BackMusicCode)
					ChangeBGM(StageField[lpCurPlayer->OnStageField]->BackMusicCode);
			}

		}
	}

	if ((PlayCounter & 0x3) == 0) {
		//¹ã ³·ÀÇ º¯È­
		if (StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_ROOM &&
			(dwGameHour < GAME_HOUR_DAY || dwGameHour >= GAME_HOUR_DARKNESS ||
				StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_DUNGEON))
		{
			NightDayTime = 1;
		}
		else
		{
			NightDayTime = 0;
		}

		cnt = DarkLight;
		if (DarkLevel < cnt)
			DarkLevel++;
		if (DarkLevel > cnt)
			DarkLevel--;

		if ((PlayCounter & 0x7) == 0) {
			if (BackColor_mR < BackColor_R) BackColor_R--;
			if (BackColor_mR > BackColor_R) BackColor_R++;

			if (BackColor_mG < BackColor_G) BackColor_G--;
			if (BackColor_mG > BackColor_G) BackColor_G++;

			if (BackColor_mB < BackColor_B) BackColor_B--;
			if (BackColor_mB > BackColor_B) BackColor_B++;
		}

		if (smConfig.DebugMode) {
			if (DarkLevel_Fix) DarkLevel = DarkLevel_Fix;
		}
		/*
				if ( VRKeyBuff['9'] ) {
					BackColor_mR = 20;
					BackColor_mG = -10;
					BackColor_mB = -10;
					DarkLevel = 220;
				}

				if ( VRKeyBuff['8'] ) {
					BackColor_mR = 20;
					BackColor_mG = 20;
					BackColor_mB = -20;
					DarkLevel = 0;
				}

				if ( VRKeyBuff['7'] ) {
					BackColor_mR = 0;
					BackColor_mG = 0;
					BackColor_mB = 0;
				}
		*/

		if ((PlayCounter & 0xFF) == 0) {
			Ptect_GetAttackCount();

			if ((PlayCounter & 0xFFF) == 0) {
				if ((PlayCounter & 0x3FFF) == 0)
					SendUsingItemListToServer2();
				else
					SendUsingItemListToServer();
			}

			if (smConfig.WeatherSwitch)
			{
				cnt = dwGameWorldTime & 0x1FFF;//4000

				if (cnt > RainTime && cnt < (RainTime + RainLength)) {
					if (cnt > RainTime + 60 && cnt < (RainTime + RainLength - 60)) {
						if (WeatherMode != 2 && (lpCurPlayer->OnStageField >= 0 &&
							StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_DUNGEON &&
							StageField[lpCurPlayer->OnStageField]->FieldCode != rsSOD_FIELD &&
							StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_ICE)) {

							PlayWav(1, 1, 400, 2205);
							WeatherMode = 2;	//³¯¾¾ ºñ¿È
							ChangeSky(0);	//ChangeBG(0);			//Èå¸° ÇÏ´Ã
						}
					}
					else {
						switch (WeatherMode) {
						case 0:
							ChangeSky(0);	//ChangeBG(0);			//Èå¸° ÇÏ´Ã
							break;
						case 2:
							StopWav(1);
							break;
						}

						WeatherMode = 1;		//³¯¾¾ Èå¸²
					}
				}
				else
					WeatherMode = 0;		//³¯¾¾ ¸¼À½
			}

			if (rsRecorder.dwNextRecordTime < dwPlayTime) {

				if (GetSaveResult() == TRUE) {
					SaveGameData();
				}
			}
			if (rsRecorder.dwRecordTime && (rsRecorder.dwRecordTime + 1000 * 8) < dwPlayTime) 
			{

#ifdef _WINMODE_DEBUG
				if (smConfig.DebugMode) CHATGAMEHANDLE->AddChatBoxText("SaveGame TimeOut / Retry SaveGame", EChatColor::CHATCOLOR_Error);
#endif
				RecordFailCount++;
				rsRecorder.dwRecordTime = 0;
				SaveGameData();
			}

			//6È¸ ÀÌ»ó ÀúÀå ½ÇÆÐ
			if (!DisconnectFlag && RecordFailCount >= 6) {
				//¿¬¼Ó ÀúÀå ½ÇÆÐ ¿À·ù ¼­¹ö¿¡ ¾Ë¸²
				SendSetHackUser2(2700, RecordFailCount);
				DisconnectFlag = dwPlayTime;
#ifdef _WINMODE_DEBUG 		
				if (smConfig.DebugMode) CHATGAMEHANDLE->AddChatBoxText("SaveGame Failed over times", EChatColor::CHATCOLOR_Error);
#endif
			}

			//¹ÝµðºÒ 
			if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_FOREST &&
				(dwGameHour < GAME_HOUR_DAY || dwGameHour >= GAME_HOUR_DARKNESS) && (PlayCounter & 0x1FF) == 0) {

				if (((dwPlayTime >> 3) & 1) == 0) {
					StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_BANDI1);
				}
			}
		}

		/*
				if ( lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->FieldCode==13 ) {
					//¹°Ã¼ °Å¸®¿¡ µû¸¥ »ç¿îµå
					esPlayObjectSound( 5 , -15385*fONE, 100*fONE , -24949*fONE );
				}
				else {
					esPlayObjectSound( -1 , 0,0,0 );
				}
		*/
		if (lpCurPlayer->OnStageField >= 0) {
			//¹è°æ È¿°úÀ½ ¿¬ÁÖ ( ÁÖ±âÀû È£Ãâ )
			StageField[lpCurPlayer->OnStageField]->PlayAmbient();
		}
		else {
			if (ResetLastVolumeDist() > 800) {
				esPlayObjectSound2(-1, 0, 0, 0, 0);
			}
		}

		/*
					cnt = 0;
					//¹°Ã¼ °Å¸®¿¡ µû¸¥ »ç¿îµå
					esPlayObjectSound2( 0 , 128, -15385*fONE, 100*fONE , -24949*fONE );
					esPlayObjectSound2( 1 , 128, -14047*fONE, 100*fONE , -25762*fONE );
					//cnt += esPlayObjectSound2( 1 , -15385*fONE, 100*fONE , -24949*fONE , 0 );

				}

				if ( ResetLastVolumeDist()>800 ) {
					esPlayObjectSound2( -1 , 0,0,0,0 );
				}
		*/
	}
	/*
		if ( PatEffectShield ) {
			PatEffectShield->Frame += 80;
			if ( PatEffectShield->MaxFrame>=PatEffectShield->Frame )
				PatEffectShield->Frame =0;
		}
	*/


	dsMenuCursorPos(&pCursorPos, 4);
	//ÆÄÆ¼ ¸Þ´º ¸ÞÀÎ
	MainInterfaceParty();
	dsMenuCursorPos(&pCursorPos, 0);


	//¿öÇÁ ÇÊµå °Ë»ç
	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]) {
		StageField[lpCurPlayer->OnStageField]->CheckWarpGate(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
	}

	if (RestartPlayCount > 0) RestartPlayCount--;


	if ((PlayCounter & 0xFFF) == 0) 
	{
		CheckCracker(NULL);
		CheckCharForm();

		if (!lpCurPlayer->HoSkillCode && !lpCurPlayer->dwDispAppSkill) {

			if (!dwRotPlayDelayTime || dwRotPlayDelayTime < dwPlayTime) {
				smCHAR	*lpTempChar;
				smCHAR	*lpOldChar;

				lpOldChar = lpCurPlayer;

				lpTempChar = SelectRotPlayer(lpCurPlayer);
				if (lpTempChar) {
					lpCurPlayer = lpTempChar;
					sinChar = &lpCurPlayer->smCharInfo;

	
					for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
						if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].chrAttackTarget == lpOldChar)
							chrOtherPlayer[cnt].chrAttackTarget = lpCurPlayer;
					}

				}
			}
		}
		else {
			dwRotPlayDelayTime = dwPlayTime + 30 * 1000;
		}

		if ((PlayCounter & 0x3FFF) == 0 && smConfig.DebugMode) {
			SendAdminMode2(smConfig.DebugMode);
		}

		CheckEnergyGraphError();
	}

	TimeEffectMain();
	CheckEnergyGraph();

	if (dwPartyPosionTime && dwPartyPosionTime < dwPlayTime) 
	{
		LaunchPartyPostion();
	}

	if ((PlayCounter & 0x1FF) == 0) 
	{
		CheckEnergyGraphError();
		if (AutoMouse_StartCheckTime) {
			if ((AutoMouse_StartCheckTime + 30000) < dwPlayTime) {

				if (AutoMouse_Distance > 20000 && AutoMouse_Attack_Count > 20 && AutoMouse_WM_Count > 110 && (AutoMouse_LastCheckTime + 30 * 60 * 1000) < dwPlayTime) {
					SendSetHackUser2(8100, AutoMouse_WM_Count);	

					if (smConfig.DebugMode) CHATGAMEHANDLE->AddChatBoxText("> Warning AutoMouse", EChatColor::CHATCOLOR_Error);
				}

				AutoMouse_StartCheckTime = dwPlayTime;
				AutoMouse_WM_Count = 0;
				AutoMouse_Distance = 0;
				AutoMouse_Attack_Count = 0;
			}
		}
		else {
			AutoMouse_StartCheckTime = dwPlayTime;
			AutoMouse_WM_Count = 0;
			AutoMouse_Distance = 0;
			AutoMouse_Attack_Count = 0;
		}


	}

	HackTrap_CheckOpenFlag();			//ÇÙÅø Ã¼Å©¿ë NPC¿ÀÇÂÇÃ·º °Ë»ç

#ifdef _TEST_SERVER
	//Ä¡Æ® ½Ã¹°·¹ÀÌ¼Ç
	if (smConfig.DebugMode && VRKeyBuff[VK_CONTROL] && VRKeyBuff['N'] && sinGetKeyClick('M')) {
		/*
		sinPlusMoney( 2101234532 );
		//lpCurPlayer->smCharInfo.szName[0] = 0;
		lpCurPlayer->smCharInfo.Level = 2;
		ReformCharForm();
		*/
		/*
		if ( MouseItem.Flag ) {
			MouseItem.sItemInfo.PotionCount = 200;
		}
		*/

		if (!ParkPlaying)
			Play_ParkPlayer("mms://hapkido.re.kr/hapkido/8833/movie_158.asf", 200, 200, 300, 300, 150);
		else
			Stop_ParkPlayer();

	}
#endif

	return TRUE;
}

extern int BGMatNum[2];
extern int RestoreTexFlag;
extern POINT	pCursorPos;


//¸¶¿ì½º·Î ÂïÀº ÇÃ·¹ÀÌ¾î·ÎÀÇ °¢µµ¸¦ ±¸ÇØÁØ´Ù
int GetMouseSelAngle()
{
	int x1, x2, z1, z2;

	if (lpCharMsTrace) {
		x1 = lpCurPlayer->pX >> FLOATNS;
		z1 = lpCurPlayer->pZ >> FLOATNS;

		x2 = lpCharMsTrace->pX >> FLOATNS;
		z2 = lpCharMsTrace->pZ >> FLOATNS;

		return GetRadian2D(x1, z1, x2, z2);
	}

	if (lpMsTraceItem) {
		x1 = lpCurPlayer->pX >> FLOATNS;
		z1 = lpCurPlayer->pZ >> FLOATNS;

		x2 = lpMsTraceItem->pX >> FLOATNS;
		z2 = lpMsTraceItem->pZ >> FLOATNS;

		return GetRadian2D(x1, z1, x2, z2);
	}

	return -1;
}

//2¸íÀÇ ÇÃ·¹ÀÌ¾îÀÇ °¢µµ¸¦ ±¸ÇØÁØ´Ù
int GetCharAngle(smCHAR *lpChar1, smCHAR *lpChar2)
{
	int x1, x2, z1, z2;

	x1 = lpChar1->pX >> FLOATNS;
	z1 = lpChar1->pZ >> FLOATNS;

	x2 = lpChar2->pX >> FLOATNS;
	z2 = lpChar2->pZ >> FLOATNS;

	return GetRadian2D(x1, z1, x2, z2);
}



int	LastRendSucessCount = 0;			//ÃÖ±Ù ·»´õ¸µ ¼º°øÇÑ Ä³¸¯ÅÍ ¼ö
int	DispCharLimitMode = 0;				//0-ÀüÃ¼ Ç¥½Ã 1-±Ù¹æ¸¸ Ç¥½Ã

#define	DISP_CHAR_LIMIT			20
#define	DISP_CHAR_LIMIT_MAX		30
#define	DISP_CHAR_NEAR_DIST		( (10*64)*(10*64) )

int DrawPat3D(int x, int y, int z, int ax, int ay, int az)
{
	//	POINT3D cp , ca;
	//	POINT3D pp , pa;

	int cnt;
	int SelZ;
	int sez;
	RECT	rect;
	RECT	SelRect;
	POINT	SelPos;
	RECT	*lpRect;
	//	POINT	Pos;
	smCHAR	*lpSelChar;
	scITEM	*lpItem;
	int		dx, dz;
	int		pow;

	POINT	sPos;
	int		RendSucessCount;			//·»´õ¸µ ¼º°ø Ä³¸¯ÅÍ Ä«¿îÅÍ
	int		DispFlag;
	int		r, g, b;
	DWORD	dwDispAppSKill;


	dwDispAppSKill = lpCurPlayer->dwDispAppSkill;		//½ºÅ³ »óÅÂ
	lpCurPlayer->dwDispAppSkill = 0;

	for (cnt = 0; cnt < 10; cnt++) {
		if (ContinueSkill[cnt].Flag) {
			switch (ContinueSkill[cnt].CODE) {
			case SKILL_EXTREME_SHIELD:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_EXTREME_SHIELD;
				break;

			case SKILL_PHYSICAL_ABSORB:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_PHYSICAL_ABSORB;
				break;

			case SKILL_AUTOMATION:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_AUTOMATION;
				break;

			case SKILL_MAXIMIZE:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_AUTOMATION;
				break;

			case SKILL_CRITICAL_HIT:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_ANGER;
				break;

			case SKILL_HOLY_BODY:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_PHYSICAL_ABSORB;
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_HOLY_BODY;
				DispApp_SkillPoint = ContinueSkill[cnt].Point;
				break;

			case SKILL_ZENITH:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_ANGER;
				break;

			case SKILL_SPARK_SHIELD:
			case SKILL_DIVINE_INHALATION:
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_SPARK_SHIELD;
				break;

			case SKILL_GODLY_SHIELD:		//°¡µé¸® ½¯µå
			case SKILL_FROST_JAVELIN:		//ÇÁ·Î½ºÆ® Á¦ºí¸° °ú °â¿ë
			case SKILL_SUMMON_MUSPELL:		//¼­¸óºÎ½ºÆç °ú °â¿ë
				lpCurPlayer->dwDispAppSkill |= SKILL_APPMASK_GODLY_SHIELD;
				break;
			}
		}
	}

	//ÇÇÁöÄÃ  »ç¿îµå ¿ÀÇÁ
	if ((lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_PHYSICAL_ABSORB) == 0 && (dwDispAppSKill&SKILL_APPMASK_PHYSICAL_ABSORB) != 0) {
		SkillStopSound(SKILL_SOUND_PHYSICAL_ABSORB3);
	}
	//¸Æ½Ã¸¶ÀÌÁî ¶Ç´À ¿ÀÅä¸ÅÀÌ¼Ç  »ç¿îµå ¿ÀÇÁ
	if ((lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_AUTOMATION) == 0 && (dwDispAppSKill&SKILL_APPMASK_AUTOMATION) != 0) {
		SkillStopSound(SKILL_SOUND_SKILL_AUTOMATION3);
	}
	//È¦¸®¹Ùµð »ç¿îµå ¿ÀÇÁ
	if ((lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_HOLY_BODY) == 0 && (dwDispAppSKill&SKILL_APPMASK_HOLY_BODY) != 0) {
		SkillStopSound(SKILL_SOUND_PHYSICAL_ABSORB3);
	}

	/*
				AttackAnger+=2;
				if ( AttackAnger>100 ) AttackAnger=100;
	*/

	SelZ = 65536 * 16384;

	smRender.SetCameraPosi(x, y, z, x, ay, az);

	if (smGameStage[0])
		SetStageDynLight(smGameStage[0], x, y, z);
	if (smGameStage[1])
		SetStageDynLight(smGameStage[1], x, y, z);


	// Ä³¸¯ÅÍ¿¡ »ç¿ëÇÒ Ä«¸Þ¶ó À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù
	smCHAR_SetCameraPosi(x, y, z, ax, ay, az);
	if (!DebugPlayer) {

		cnt = RestartPlayCount;
		if (lpCurPlayer->PlayInvincible > 0) cnt = lpCurPlayer->PlayInvincible;

		if (cnt && (cnt & 0x10) == 0) {
			r = smRender.Color_R;
			g = smRender.Color_G;
			b = smRender.Color_B;

			smRender.Color_R += 80;
			smRender.Color_B += 80;
			//smRender.Color_B += 80;


			lpCurPlayer->Draw();

			smRender.Color_R = r;
			smRender.Color_G = g;
			smRender.Color_B = b;

		}
		else {
			if (lpCurPlayer->PlayVanish <= 0) lpCurPlayer->Draw();
		}

		//ÇÃ·¹ÀÌ¾î Å¬·£ ¸¶½ºÅÍ È®ÀÎ
		if (rsBlessCastle.dwMasterClan && lpCurPlayer->smCharInfo.ClassClan)
			SetClanMaster_Player(rsBlessCastle.dwMasterClan);

	}

	lpSelChar = 0;
	RendSucessCount = 0;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].DisplayAlpha == 0) {

			DispFlag = TRUE;

			if (DispCharLimitMode) 
			{
				dx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
				dz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
				if ((dx*dx + dz * dz) > DISP_CHAR_NEAR_DIST) DispFlag = FALSE;
			}

			if (DispFlag) {

				r = smRender.Color_R;
				g = smRender.Color_G;
				b = smRender.Color_B;

				if (lpCurPlayer->HoSkillCode && chrOtherPlayer[cnt].smCharInfo.Life[0]) 
				{
					switch (lpCurPlayer->HoSkillCode) 
					{
					case SKILL_SCOUT_HAWK:
						pow = GetSkillPower(lpCurPlayer, &chrOtherPlayer[cnt], 0, 0, 0);
						if (pow) 
						{
							pow = DarkLevel >> 2;
							smRender.Color_R += 40 + pow;
							smRender.Color_G += 40 + pow;
							smRender.Color_B += 100 + pow;
						}
						break;
					}
				}

				DWORD OutLineColor = 0;

				if (lpCharSelPlayer && lpCharSelPlayer == &chrOtherPlayer[cnt]) {
					smRender.Color_R = r + 40;
					smRender.Color_G = g + 60;
					smRender.Color_B = b + 60;
					
					//CORES DO TITULOS
					if (lpCharSelPlayer->smCharInfo.State == smCHAR_STATE_NPC)
					{
						//Green
						OutLineColor = D3DCOLOR_ARGB(255, 0, 255, 100);
					}
					else if (lpCharSelPlayer->smCharInfo.State == smCHAR_STATE_USER)
					{
						//White
						OutLineColor = D3DCOLOR_ARGB(255, 255, 255, 255);
					}
					else
					{
						//Red
						OutLineColor = D3DCOLOR_ARGB(255, 255, 30, 0);
					}

					//int ix = (lpCharSelPlayer->pX - smCHAR_CameraX) >> FLOATNS;
					//int iy = (lpCharSelPlayer->pY - smCHAR_CameraY) >> FLOATNS;
					//int iz = (lpCharSelPlayer->pZ - smCHAR_CameraZ) >> FLOATNS;

					//int Dist = (int)sqrt(ix*ix + iy * iy + iz * iz);
					//smRender.SetD3D_OutLine(1, Dist, 1.4f, 0, OutLineColor);

				}

				if (!chrOtherPlayer[cnt].RenderLatter && chrOtherPlayer[cnt].AttackIce == 0 && chrOtherPlayer[cnt].RendAlpha == 0 && chrOtherPlayer[cnt].PlayVanish <= 0) {
					
					if (chrOtherPlayer[cnt].PlayInvincible && (chrOtherPlayer[cnt].PlayInvincible & 0x10) == 0) 
					{
						smRender.Color_R += 80;
						smRender.Color_B += 80;
					}

					if (OutLineColor > 0) 
					{
						DX::cSelectGlow1.Begin(OutLineColor, TRUE);
						chrOtherPlayer[cnt].Draw();
						DX::cSelectGlow1.End(TRUE);
						DX::cSelectGlow1.Begin(OutLineColor, FALSE);
						chrOtherPlayer[cnt].Draw();
						DX::cSelectGlow1.End(FALSE);
					}

					chrOtherPlayer[cnt].Draw();
				}
				else {

					if (OutLineColor > 0)
					{
						DX::cSelectGlow1.Begin(OutLineColor, TRUE);
						chrOtherPlayer[cnt].VirtualDraw();
						DX::cSelectGlow1.End(TRUE);
						DX::cSelectGlow1.Begin(OutLineColor, FALSE);
						chrOtherPlayer[cnt].VirtualDraw();
						DX::cSelectGlow1.End(FALSE);
					}

					chrOtherPlayer[cnt].VirtualDraw();
				}

				smRender.Color_R = r;
				smRender.Color_G = g;
				smRender.Color_B = b;

				smRender.SetD3D_OutLine(0);

				if (chrOtherPlayer[cnt].RendSucess) {

					RendSucessCount++;

					sez = chrOtherPlayer[cnt].RendPoint.z;
					lpRect = &chrOtherPlayer[cnt].RendRect2D;

					if (chrOtherPlayer[cnt].smCharInfo.Life[0] > 0 && sez > (96 * fONE) && sez < SelZ &&
						lpRect->left<pCursorPos.x && lpRect->right>pCursorPos.x &&
						lpRect->top<pCursorPos.y && lpRect->bottom>pCursorPos.y) {

						SelZ = sez;
						lpSelChar = &chrOtherPlayer[cnt];
					}
				}
			}
			else {
				chrOtherPlayer[cnt].RendSucess = FALSE;
				RendSucessCount++;
			}
		}
	}


	LastRendSucessCount = RendSucessCount;

	//È­¸é Ç¥½Ã Ä³¸¯ÅÍ ¸ðµå Á¶Àý
	if (DispCharLimitMode) {
		if (RendSucessCount < DISP_CHAR_LIMIT)
			DispCharLimitMode = FALSE;
	}
	else {
		if (RendSucessCount > DISP_CHAR_LIMIT_MAX)
			DispCharLimitMode = TRUE;
	}

	if (VRKeyBuff['A']) {
		lpSelChar = 0;
		SelZ = 65536 * 16384;
	}


	lpItem = 0;

	for (cnt = 0; cnt < DISP_ITEM_MAX; cnt++) 
	{
		if (scItems[cnt].Flag)
		{
			if ((scItems[cnt].ItemCode != 0 || DisplayDebug) && CanViewDropItem(&scItems[cnt]))
			{
				scItems[cnt].Draw();

				smRender.SetCameraPosi(x, y, z, ax, ay, az);

				sez = smRender.GetRect2D(scItems[cnt].pX, scItems[cnt].pY + (16 * fONE), scItems[cnt].pZ,
					32 * fONE, 32 * fONE, &rect, &sPos);

				scItems[cnt].RendPoint.x = sPos.x;
				scItems[cnt].RendPoint.y = sPos.y;
				scItems[cnt].RendPoint.z = sez;

				if (sez > 0 && sez < SelZ &&
					rect.left<pCursorPos.x && rect.right>pCursorPos.x &&
					rect.top<pCursorPos.y && rect.bottom>pCursorPos.y) {

					SelZ = sez;
					lpItem = &scItems[cnt];
					memcpy(&SelRect, &rect, sizeof(RECT));
					memcpy(&SelPos, &sPos, sizeof(POINT));

					DWORD dwColor = D3DCOLOR_XRGB(255, 255, 0);
					DX::cSelectGlow2.Begin(dwColor, TRUE);
					scItems[cnt].Draw();
					DX::cSelectGlow2.End(TRUE);
					DX::cSelectGlow2.Begin(dwColor, FALSE);
					scItems[cnt].Draw();
					DX::cSelectGlow2.End(FALSE);
				}
			}

		}
	}

	if (lpSelChar) {
		lpCharSelPlayer = lpSelChar;
		lpItem = 0;
	}
	else
		lpCharSelPlayer = 0;

	if (lpItem) {
		lpSelItem = lpItem;
		memcpy(&MsSelRect, &SelRect, sizeof(RECT));
		memcpy(&MsSelPos, &SelPos, sizeof(POINT));
	}
	else
		lpSelItem = 0;

	if (cShop.OpenFlag || cCharShop.OpenFlag) 
	{
		lpCharSelPlayer = 0;
		lpSelItem = 0;

		if (CursorClass == SIN_CURSOR_ATTACK ||
			CursorClass == SIN_CURSOR_TALK ||
			CursorClass == SIN_CURSOR_GETITEM2 ||
			CursorClass == SIN_CURSOR_GETITEM1) {

			CursorClass = 1;
		}
		return TRUE;
	}

	if (MouseItem.Flag) 
	{
		CursorClass = 8; // Segurando o item no inventário
	}
	else 
	{
		CursorClass = 1;

		if (lpCharSelPlayer) 
		{
			switch (lpCharSelPlayer->smCharInfo.State) 
			{
			case smCHAR_STATE_ENEMY:
				CursorClass = SIN_CURSOR_ATTACK;
				break;
			case smCHAR_STATE_USER:
				break;
			case smCHAR_STATE_NPC:
				CursorClass = SIN_CURSOR_TALK;
				break;
			}
		}

		if (lpSelItem) 
		{
			if (MouseButton[0])
				CursorClass = SIN_CURSOR_GETITEM2;
			else
				CursorClass = SIN_CURSOR_GETITEM1;
		}
	}

	return TRUE;
}

int DrawPat3D_Alpha()
{
	int cnt = 0;
	int DispFlag = 0;
	int dx = 0, dz = 0;
	int ap = 0;
	int	RendFalg = 0;
	int BackFrame = 0;
	int cnt2 = 0;

	if (lpCurPlayer->PlayVanish > 0) 
	{
		ap = lpCurPlayer->DisplayAlpha;
		lpCurPlayer->DisplayAlpha = -160;

		if (lpCurPlayer->PlayVague) {
			SetItem2PassTexture(8, smTEXSTATE_FS_SCROLL5);
			lpCurPlayer->DisplayAlpha = -60;
		}
		lpCurPlayer->Draw();

		SetItem2PassTexture(-1);
		lpCurPlayer->DisplayAlpha = ap;
	}

	if ((lpCurPlayer->AttackSkil & 0xFF) == SKILL_PLAY_SHADOW_MASTER && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_SKILL) 
	{
		ap = lpCurPlayer->DisplayAlpha;
		BackFrame = lpCurPlayer->frame;
		lpCurPlayer->DisplayAlpha = -140;
		for (cnt2 = 0; cnt2 < 6; cnt2++) {
			lpCurPlayer->frame -= 160 * 2;
			if (lpCurPlayer->frame > (lpCurPlayer->MotionInfo->StartFrame * 160)) {
				lpCurPlayer->Draw();
			}
			lpCurPlayer->DisplayAlpha -= 20;
		}
		lpCurPlayer->frame = BackFrame;
		lpCurPlayer->DisplayAlpha = ap;
	}

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag) {
			RendFalg = 0;
			if (chrOtherPlayer[cnt].DisplayAlpha != 0 || chrOtherPlayer[cnt].RendAlpha != 0 || chrOtherPlayer[cnt].AttackIce != 0 || chrOtherPlayer[cnt].RenderLatter || chrOtherPlayer[cnt].PlayVanish) {
				DispFlag = TRUE;

				if (DispCharLimitMode && chrOtherPlayer[cnt].AttackIce == 0) 
				{
					dx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
					dz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
					if ((dx*dx + dz * dz) > DISP_CHAR_NEAR_DIST) DispFlag = FALSE;
				}

				if (chrOtherPlayer[cnt].PlayVanish > 0 && lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD) 
				{
					if (lpCharSelPlayer != &chrOtherPlayer[cnt]) 
					{
						// Distância mínima para os outros players visualizarem o jogador usando vanish (skill do ps)
						dx = (lpCurPlayer->pX - chrOtherPlayer[cnt].pX) >> FLOATNS;
						dz = (lpCurPlayer->pZ - chrOtherPlayer[cnt].pZ) >> FLOATNS;
						if ((dx*dx + dz * dz) > DIST_TRANSLEVEL_NEAR) DispFlag = FALSE;
					}
				}

				if (DispFlag) 
				{
					if (lpCharSelPlayer && lpCharSelPlayer == &chrOtherPlayer[cnt])
					{
						DWORD OutLineColor;
						switch (lpCharSelPlayer->smCharInfo.State)
						{
						case smCHAR_STATE_USER:
							OutLineColor = D3DCOLOR_RGBA(255, 255, 255, 255);
							break;

						case smCHAR_STATE_NPC:
							OutLineColor = D3DCOLOR_RGBA(0, 255, 100, 255);
							break;

						default:
							OutLineColor = D3DCOLOR_RGBA(255, 30, 0, 255);
							break;
						}

						int ix = (lpCharSelPlayer->pX - smCHAR_CameraX) >> FLOATNS;
						int iy = (lpCharSelPlayer->pY - smCHAR_CameraY) >> FLOATNS;
						int iz = (lpCharSelPlayer->pZ - smCHAR_CameraZ) >> FLOATNS;

						int Dist = (int)sqrt(ix*ix + iy * iy + iz * iz);
						smRender.SetD3D_OutLine(1, Dist, 1.4f, 0, OutLineColor);
					}

					ap = chrOtherPlayer[cnt].DisplayAlpha;
					if (chrOtherPlayer[cnt].PlayVanish > 0) 
					{
						chrOtherPlayer[cnt].DisplayAlpha = -160;
						if (chrOtherPlayer[cnt].PlayVague) SetItem2PassTexture(8, smTEXSTATE_FS_SCROLL5);
					}

					if (chrOtherPlayer[cnt].RendAlpha && ap > chrOtherPlayer[cnt].RendAlpha) {
						//if ( chrOtherPlayer[cnt].MotionInfo->State<0x100 || chrOtherPlayer[cnt].AttackIce!=0 ) {
						chrOtherPlayer[cnt].DisplayAlpha = chrOtherPlayer[cnt].RendAlpha;	//¿ø·¡ ¾ËÆÄ Ä³¸¯ÀÎ °æ¿ì

						if (chrOtherPlayer[cnt].MotionInfo->State >= 0x100 &&
							(chrOtherPlayer[cnt].smCharInfo.dwCharSoundCode == snCHAR_SOUND_ILLUSIONKNIGHT || chrOtherPlayer[cnt].PlayVanish > 0)) {
							chrOtherPlayer[cnt].DisplayAlpha = ap;
						}

						chrOtherPlayer[cnt].Draw();
						chrOtherPlayer[cnt].DisplayAlpha = ap;		//ÀÓ½Ã¾ËÆÄ º¹±¸
					//}
					}
					else {
						chrOtherPlayer[cnt].Draw();
					}
					RendFalg++;

					SetItem2PassTexture(-1);
					chrOtherPlayer[cnt].DisplayAlpha = ap;
					//######################################################################################
					// ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
					smRender.SetD3D_OutLine(0);
					//######################################################################################

				}
			}

			if (RendFalg == 0) {
				if ((chrOtherPlayer[cnt].AttackSkil & 0xFF) == SKILL_PLAY_SHADOW_MASTER && chrOtherPlayer[cnt].MotionInfo->State == CHRMOTION_STATE_SKILL) {
					ap = chrOtherPlayer[cnt].DisplayAlpha;		//ÀÓ½Ã¾ËÆÄ ¹é¾÷
					BackFrame = chrOtherPlayer[cnt].frame;
					chrOtherPlayer[cnt].DisplayAlpha = -160;
					for (cnt2 = 0; cnt2 < 4; cnt2++) {
						chrOtherPlayer[cnt].frame -= 160 * 4;
						if (chrOtherPlayer[cnt].frame > (chrOtherPlayer[cnt].MotionInfo->StartFrame * 160)) {
							chrOtherPlayer[cnt].Draw();
						}
					}
					chrOtherPlayer[cnt].frame = BackFrame;
					chrOtherPlayer[cnt].DisplayAlpha = ap;
				}
			}
		}
	}

	return TRUE;
}

//±×¸²ÀÚ ±×¸®±â
int DrawPatShadow(int x, int y, int z, int ax, int ay, int az)
{
	int cnt;

	if (!Settings::GetInstance()->cShadows)
		return TRUE;

	smCHAR_SetCameraPosi(x, y, z, ax, ay, az);
	//	AddDepthZ = -6;


	//   GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_EQUAL            );

	cnt = 0;

	lpCurPlayer->DrawShadow();

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	if (lpCurPlayer->OnStageField >= 0)
	{
		if (StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ICE)
			lpCurPlayer->DrawIceFoot();
	}
	//######################################################################################

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].DisplayAlpha == 0 && chrOtherPlayer[cnt].RendAlpha == 0) {
			//±×¸²ÀÚ ±×¸®±â
			chrOtherPlayer[cnt].DrawShadow();
		}
	}
	//    GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ZFUNC,   D3DCMP_LESSEQUAL );
	//	AddDepthZ = 0;

	return TRUE;
}

extern BYTE VRKeyBuff[256];

int Draw2DCounter = 0;

int DrawPat2D(int x, int y, int z, int ax, int ay, int az)
{
	int cnt;

	if (lpCurPlayer->Pattern == 0) return FALSE;

	smCHAR_SetCameraPosi(x, y, z, ax, ay, az);
	smRender.SetMaterialGroup(smMaterialGroup);

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag) {
			chrOtherPlayer[cnt].DrawMotionBlur();
		}
	}

	lpCurPlayer->DrawMotionBlur();

	DrawParticle(x, y, z, ax, ay, az);

	Draw2DCounter++;

	DrawLightImage();

	DrawWeather(ax);


	return TRUE;

}

extern int ViewPointLen;
extern DWORD smBackColor;

int BGFace[2][3] = {
	{ 0 ,  1 , 2 },
	{ 1 ,  3 , 2 }
};



LPDIRECT3DTEXTURE9*hBGTexture;
LPDIRECT3DTEXTURE9*hBGTexture2;
smOBJ3D *smBackGround;

LPDIRECT3DTEXTURE9 *BackVideoSurface;
int BackVideoFrame;
int LastFrame;

int BGMatNum[2];


#include "pbackground.h"

smBACKGROUND	smBackMesh[2];
smBACKGROUND	*lpCurrentBG;


int InitBackGround()
{
	lpCurrentBG = 0;
	ChangeSky(0);

	return TRUE;
}

int CloseBackGround()
{
	smBackMesh[1].Close();
	smBackMesh[0].Close();
	return TRUE;
}


int BgFrame = 0;

int BGMain()
{
	smBackMesh[0].Main();
	smBackMesh[1].Main();

	if (lpCurrentBG && lpCurrentBG->FadeShowCount == 0) {
		if (&smBackMesh[0] != lpCurrentBG && smBackMesh[0].smObjBG)
			smBackMesh[0].Close();

		if (&smBackMesh[1] != lpCurrentBG && smBackMesh[1].smObjBG)
			smBackMesh[1].Close();
	}

	return TRUE;
}

int ChangeBG(int BgNum)
{

	if (!lpCurrentBG) {
		smBackMesh[0].Init(BgNum);
		lpCurrentBG = &smBackMesh[0];
		if (smBackMesh[0].lpMaterialGroup)
			smBackMesh[0].lpMaterialGroup->ReadTextures();

		smBackMesh[0].FadeShowCount = 0;

		return TRUE;
	}

	if (lpCurrentBG->BackGroundNum == BgNum) return FALSE;

	if (&smBackMesh[0] != lpCurrentBG && !smBackMesh[0].smObjBG) {
		smBackMesh[0].Init(BgNum);
		lpCurrentBG = &smBackMesh[0];
		if (smBackMesh[0].lpMaterialGroup)
			smBackMesh[0].lpMaterialGroup->ReadTextures();
		return TRUE;
	}

	if (&smBackMesh[1] != lpCurrentBG && !smBackMesh[1].smObjBG) {
		smBackMesh[1].Init(BgNum);
		lpCurrentBG = &smBackMesh[1];
		if (smBackMesh[1].lpMaterialGroup)
			smBackMesh[1].lpMaterialGroup->ReadTextures();
		return TRUE;
	}

	return FALSE;
}

int DrawBG(int x, int y, int z, int ax, int ay, int az)
{
	if (StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_DUNGEON) return TRUE;
	if (StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM) return TRUE;

	GRAPHICDEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);

	if (smBackMesh[0].FadeShowCount > smBackMesh[1].FadeShowCount) {
		smBackMesh[0].Draw(x, y, z, ax, ay, az);
		smBackMesh[1].Draw(x, y, z, ax, ay, az);
	}
	else {
		smBackMesh[1].Draw(x, y, z, ax, ay, az);
		smBackMesh[0].Draw(x, y, z, ax, ay, az);
	}

	GRAPHICDEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);

	return TRUE;
}

RECT	PartyTradeButtonRect[4] = {
	{	00,0, 20, 20	},
	{	25,0, 20, 20	},
	{	50,0, 20, 20	},
	{	75,0, 20, 20	}
};

RECT	PartyTradeToolTipRect = { 1,18,70,28 };

POINT	PartyButtonPoint = { -1, -1 };

//Posicao dos botoes no player
//RESOLVER
int GetPartyTradeButtonPos(int x, int y)
{
	int cnt, sx;
	int AdjustX;
	int MaxCnt;

	int wx, wy;
	if (PartyButtonPoint.x < 0) return -1;

	wx = PartyButtonPoint.x ;
	wy = PartyButtonPoint.y;

	if (EachTradeButtonMode) { AdjustX = -10; MaxCnt = 4; }
	else { AdjustX = 0; MaxCnt = 3; }

	for (cnt = 0; cnt < MaxCnt; cnt++) {
		sx = PartyTradeButtonRect[cnt].left + AdjustX + wx;
		if (sx<x && (sx + PartyTradeButtonRect[cnt].right)>x &&
			PartyTradeButtonRect[cnt].top + wy<y && (PartyTradeButtonRect[cnt].top + wy + PartyTradeButtonRect[cnt].bottom)>y) {

			return cnt;

		}
	}

	return -1;
}

int DisplayPartyTradeButton()
{
	int SelNum;
	int cnt;
	int AdjustX;
	int MaxCnt;

	int wx, wy;
	if (PartyButtonPoint.x < 0) return -1;
	wx = PartyButtonPoint.x;
	wy = PartyButtonPoint.y;

	if (EachTradeButtonMode) { AdjustX = -10; MaxCnt = 4; }
	else { AdjustX = 0; MaxCnt = 3; }

	SelNum = GetPartyTradeButtonPos(pCursorPos.x, pCursorPos.y);

	for (cnt = 0; cnt < MaxCnt; cnt++) {
		if (SelNum == cnt) {
			if (lpDDS_ParTradeButton[cnt][1]) {
				DrawSprite(PartyTradeButtonRect[cnt].left + AdjustX + wx, PartyTradeButtonRect[cnt].top + wy,
					lpDDS_ParTradeButton[cnt][1], 0, 0, PartyTradeButtonRect[cnt].right, PartyTradeButtonRect[cnt].bottom);
			}
		}
		else {
			if (lpDDS_ParTradeButton[cnt][0]) {
				DrawSprite(PartyTradeButtonRect[cnt].left + AdjustX + wx, PartyTradeButtonRect[cnt].top + wy,
					lpDDS_ParTradeButton[cnt][0], 0, 0, PartyTradeButtonRect[cnt].right, PartyTradeButtonRect[cnt].bottom);
			}
		}
	}

	if (SelNum >= 0) {
		if (lpDDS_ParTradeToolTip[SelNum]) {
			DrawSprite(PartyTradeButtonRect[SelNum].left - 60 + AdjustX + wx, PartyTradeToolTipRect.top + wy,
				lpDDS_ParTradeToolTip[SelNum], 0, 0, PartyTradeToolTipRect.right, PartyTradeToolTipRect.bottom);
		}
	}

	return TRUE;
}

smCHAR	chrEachPlayer;
smCHAR *chrEachMaster = 0;
int		EachTradeButton = 0;
int		EachTradeButtonMode = 0;
RECT	RectTradeButton;

POINT3D EachCameraPos;
int	DispEachMode = 0;

int FindBipPoint(smPAT3D *lpPat, char *szObjNodeName, int frame, int *nX, int *nY, int *nZ);
// era pra ser aqui
int OpenEachPlayer(smCHAR *lpChar)
{
	if (!lpChar || !lpChar->Flag || !lpChar->DisplayFlag) return FALSE;

	memcpy(&chrEachPlayer, lpChar, sizeof(smCHAR));
	chrEachPlayer.SetMotionFromCode(CHRMOTION_STATE_STAND);
	chrEachPlayer.ActionPattern = 101;
	chrEachMaster = lpChar;

	chrEachPlayer.Flag = TRUE;

	if (FindBipPoint(chrEachPlayer.AnimPattern, "Bip01 Head", chrEachPlayer.frame,
		&EachCameraPos.x, &EachCameraPos.y, &EachCameraPos.z) == NULL) {

		EachCameraPos.y = chrEachPlayer.PatHeight - 10 * fONE;
		EachCameraPos.x = 0;
		EachCameraPos.z = 0;
	}

	if (lpChar->smCharInfo.State == smCHAR_STATE_ENEMY || lpChar->smCharInfo.State == smCHAR_STATE_NPC) {
		EachCameraPos.y += lpChar->smCharInfo.ArrowPosi[0] * fONE;
		EachCameraPos.z += lpChar->smCharInfo.ArrowPosi[1] * fONE;
	}

	return TRUE;
}

int CloseEachPlayer()
{
	chrEachPlayer.Flag = 0;
	chrEachMaster = 0;
	EachTradeButton = 0;

	return TRUE;
}

int EachPlayerMain()
{
	if (!chrEachMaster) {
		return TRUE;
	}

	if (!chrEachMaster->Flag || !chrEachMaster->dwObjectSerial || !chrEachMaster->Pattern) {
		CloseEachPlayer();
		return TRUE;
	}

	if (chrEachMaster->Flag) {
		if (chrEachMaster->MotionInfo->State == CHRMOTION_STATE_DEAD && chrEachMaster->FrameCounter > 0x50)			CloseEachPlayer();
	}


	return TRUE;
}

int DrawClanMark_EachBox(LPDIRECT3DTEXTURE9 clanMark, int x, int y)
{

	RECT ddRect;

	ddRect.left = 0;
	ddRect.right = 16;
	ddRect.top = 0;
	ddRect.bottom = 16;

	if (clanMark)
		DrawSprite(x, y, clanMark, 0, 0, 16, 16, true);

	return TRUE;
}


bool MouseAction(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x&& pRealCursorPos.x < x + w && pRealCursorPos.y > y&& pRealCursorPos.y < y + h)
		return TRUE;

	return FALSE;
}

extern BOOL isEnemy;
extern BOOL showDetails;

HRESULT WINAPI EndScene_Detour(LPDIRECT3DDEVICE9 Device_Interface)
{
	D3DXVECTOR3 Position;
	Position.x = 150.0f;
	Position.y = 200.0f;
	Sprite_Interface->Begin(D3DXSPRITE_ALPHABLEND);
	Sprite_Interface->Draw(Texture_Interface, NULL, NULL, &Position, 0xFFFFFFFF);
	Sprite_Interface->End();

	return 0;
}


int DrawEachPlayer(float fx, float fy, int SizeMode)
{
	int x, y, z, w, h;
	int mx, my, he;
	POINT3D angle;
	int dist, len;
	HDC	hdc = NULL;
	DWORD	dwColor;
	int		ObjSizeBack;
	int		ModeFlag;
	int		ClanX, ClanY, ClanLen;
	bool isNPC = false;

	if (chrEachMaster && chrEachMaster->ActionPattern == 99) 
	{
		if ((chrEachMaster->dwLastTransTime + DISPLAY_TRANS_TIME_OVER) < dwPlayTime) {
			CloseEachPlayer();
			return TRUE;
		}
	}

	if (lpCharMsTrace) 
	{
		if (chrEachMaster != lpCharMsTrace) 
		{
			EachTradeButton = 0;
			OpenEachPlayer(lpCharMsTrace);
			DispEachMode = 0;
		}
	}
	else 
	{
		if (lpCharSelPlayer && (chrEachMaster != lpCharSelPlayer) &&
			(chrEachMaster != &chrPartyPlayer) && !DispEachMode) 
		{
			OpenEachPlayer(lpCharSelPlayer);
			DispEachMode = 0;
		}
	}

	return FALSE;
}

int FindBipPoint(smPAT3D *lpPat, char *szObjNodeName, int frame, int *nX, int *nY, int *nZ)
{

	smOBJ3D	*lpObj;

	lpObj = lpPat->GetObjectFromName(szObjNodeName);
	if (!lpObj) return FALSE;


	smMATRIX *mWorld;
	POINT3D angle;

	ZeroMemory(&angle, sizeof(POINT3D));
	angle.y = (-angle.y + ANGLE_180)&ANGCLIP;

	mWorld = &lpObj->mWorld;

	AnimObjectTree(lpObj, frame, angle.x, angle.y, angle.z);

	*nX = mWorld->_41;
	*nZ = mWorld->_42;
	*nY = mWorld->_43;

	return TRUE;

}

int Record_ClinetLogFile(char *szMessage)
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	SYSTEMTIME	st;
	int len;
	char	szFileName[128];
	char	szBuff[256];

	GetLocalTime(&st);
	wsprintf(szFileName, "game.log", st.wMonth, st.wDay);

	hFile = CreateFile("game.log", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return FALSE;

	wsprintf(szBuff, "%d-%d (%d:%d:%d) - %s", st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, szMessage);
	len = lstrlen(szBuff);
	if (szBuff[len - 1] != '\n') {
		szBuff[len] = '\r';
		szBuff[len + 1] = '\n';
		len += 2;
	}

	FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);
	WriteFile(hFile, szBuff, len, &dwAcess, NULL);
	CloseHandle(hFile);

	return TRUE;
}

//////////////// ¹ÚÃ¶È£ µ¿¿µ»ó ÇÃ·¹ÀÌ¾î /////////////
#include "TJBOY\\park\\ParkPlayer\\ParkDXplayer.h"

int ParkPlayInit = 0;		//ÃÊ±âÈ­ ÇÃ·¢
int ParkPlayMode = 0;		//-ÇÃ¸³±ÝÁö 0-ÀÏ¹Ý +½½¸³½Ã°£(ÃÖ´ë 1000)
int ParkPlaying = 0;		//µ¿¿µ»ó ÇÃ·¹ÀÌÁß
RECT ParkPlayRect;

//ÇÃ·¹ÀÌ¾î ÃÊ±âÈ­
int Init_ParkPlayer()
{
	//if ( !ParkPlayInit ) {
	ParkPlayInit = TRUE;
	ParkDXPlay.InitPlayer();
	ParkPlayMode = 0;
	ParkPlaying = 0;
	ParkPlayRect.left = 0;
	ParkPlayRect.top = 0;
	ParkPlayRect.right = WinSizeX;
	ParkPlayRect.bottom = WinSizeY;

	return TRUE;
	//}

	//return FALSE;
}

//ÇÃ·¹ÀÌ¾î Àç»ý
int Play_ParkPlayer(char *szPath, int x, int y, int w, int h, int PlayMode)
{
	if (ParkPlaying)
		Stop_ParkPlayer();

	Init_ParkPlayer();

	if (ParkDXPlay.LoadPlayerFile(hwnd, szPath)) {
		ParkDXPlay.PlaySize(x, y, w, h);
		ParkDXPlay.PlayPlayer();
		ParkPlayMode = PlayMode;
		ParkPlaying = TRUE;

		ParkPlayRect.left = x;
		ParkPlayRect.top = y;
		ParkPlayRect.right = x + w;
		ParkPlayRect.bottom = y + h;

		return TRUE;
	}

	return FALSE;
}

//ÇÃ·¹ÀÌ¾î ÁßÁö
int Stop_ParkPlayer()
{
	ParkDXPlay.StopPlayer();
	ParkPlayMode = 0;
	ParkPlaying = FALSE;

	return TRUE;
}
