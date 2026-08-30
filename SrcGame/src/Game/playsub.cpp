#include "globals.h"

#include "Utils/common.h"

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
#include "resource.h"
#include "drawsub.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "srcServer\onserver.h"

#include "BellatraFontEffect.h"
#include "TextMessage.h"

//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
#include "WinInt\\WinIntThread.h"
#include "FullZoomMap.h"
#include "FontImage.h"
//######################################################################################



extern HINSTANCE hinst;

/////////////////////// ¼­¹öÃø ÇÔ¼ö ºÒ·¯´Ù ¾´´Ù //////////////////////
//ÀÌº¥Æ® Áö¿ªÀÎÁö Ã½Å©
int rsCheckHardCoreEventArea(int x, int y, int z);
extern POINT3D	rsHardCoreEventOpenPos[7][3];

//////////////////////////////////////////////////////////////////////

int	CodeXorExp = 0;				//°æÇèÄ¡ ¼û±èÄÚµå_
int	CodeXorExp_High = 0;			//°æÇèÄ¡ ¼û±èÄÚµå
int	CodeXorLife = 0;				//»ý¸í·Â ¼û±èÄÚµå
int	CodeXorMana = 0;				//±â·Â ¼û±èÄÚµå
int	CodeXorStamina = 0;				//±Ù·Â ¼û±èÄÚµå
int	CodeXorMoney = 0;				//µ· ¼û±èÄÚµå

//extern int isItemCostume;

//ÀÌÂÊ ¸ðµâ º¸È£ Ã½Å©¼¶ ÄÚµå
#define	PLAYSUB_CHECKRESULT			                                                               1884018394

DWORD	dwPlaySub_CheckResult = PLAYSUB_CHECKRESULT;

static char* szArmorDirectory = "char\\tmABCD\\";
static char* szNpcDirectory = "char\\npc\\";

#define MAX_JOBNUM	8

#include "playmodel.h"

int ArmorJobNum[9] = { 0, 1, 0, 3, 2, 5, 4, 7, 6 };

int	PlayerArmor = 0;

//extern sITEM		InvenCostume;

#define TEMP_DYNPAT_MAX		2400

smDPAT* DynTempPat[TEMP_DYNPAT_MAX];

int DynTempPatCnt = 0;

int	PlayUsed_Resurrection = 0;		//ºÎÈ° ¾ÆÀÌÅÛ »ç¿ë


DWORD	dwSafeCodeSum;
//0xE9
BYTE	dwAsmJmpCode = 0xE0;

DWORD	dwStartFuncPoint = 0;
DWORD	dwFuncLen = 0;
DWORD	dwFuncSafeMemCount;
DWORD	dwFuncSafeMemCode;

//////////////////////////////////////////
DWORD	dwFuncChkTime = 0;
DWORD	dwFuncSafeCode = 0;
//////////////////////////////////////////

char* lpFuncBackupBuff = 0;



LPDIRECT3DTEXTURE9	lpDDS_CodeSaveBuff = 0;

extern char* szBangImage;

DWORD* lpCodeVram = 0;

//½ÃÀÛ Æã¼Ç
int	PlaySub_StartFunc()
{
	return FALSE;
}

//VRAM¿¡ µ¥ÀÌÅ¸ ÀúÀå
int Code_VRamBuffOpen()
{

	lpCodeVram = new DWORD[256];

	return (DWORD)lpCodeVram;

}

int Code_VramClear()
{
	lpCodeVram[0] = 777;
	lpCodeVram[1] = 0;
	lpCodeVram[2] = 777;
	lpCodeVram[3] = 0;

	return TRUE;
}

int Code_VramSave(DWORD Param1, DWORD Param2, DWORD Param3)
{
	DWORD	dwTime;

	if (lpCodeVram) {
		dwTime = GetCurrentTime();

		lpCodeVram[3] = dwTime;
		lpCodeVram[0] = Param1 ^ dwTime;
		lpCodeVram[1] = Param2 ^ dwTime;
		lpCodeVram[2] = Param3 ^ dwTime;
	}

	return TRUE;
}

extern int iCostumeFlag;

int Code_VramLoad(DWORD* Param1, DWORD* Param2, DWORD* Param3)
{
	*Param1 = lpCodeVram[0] ^ lpCodeVram[3];
	*Param2 = lpCodeVram[1] ^ lpCodeVram[3];
	*Param3 = lpCodeVram[2] ^ lpCodeVram[3];

	return TRUE;
}

int	Mix_CodeVram()
{
	DWORD	dwTime;
	DWORD	dwNewTime;


	dwTime = (dwPlayTime >> 4) & 0xFF;
	dwNewTime = (dwTime << 24) | (dwTime << 16) | (dwTime << 8) | dwTime;

	dwTime = lpCodeVram[3];

	lpCodeVram[0] = (lpCodeVram[0] ^ dwTime) ^ dwNewTime;
	lpCodeVram[1] = (lpCodeVram[1] ^ dwTime) ^ dwNewTime;
	lpCodeVram[2] = (lpCodeVram[2] ^ dwTime) ^ dwNewTime;
	lpCodeVram[3] = dwNewTime;

	return TRUE;
}


DWORD Check_CodeSafe(DWORD dwStartFunc)
{
	BYTE* lpFunc;
	DWORD	dwCode;
	DWORD* lpdwFunc;
	BYTE	JmpCode = 9;
	DWORD	dwHeaderPos = 0x30;


	dwCode = 1697348954;

	dwFuncChkTime = GetCurrentTime();
	DWORD FnCode = dwCode ^ ((dwFuncChkTime << 16) | (dwFuncChkTime & 0xFFFF));

	dwFuncSafeCode = FnCode;
	dwFuncSafeMemCount = 0;
	dwFuncSafeMemCode = 0;

	Code_VramSave(FnCode, dwFuncChkTime, dwCode);

	lpFunc = 0;
	lpdwFunc = 0;

	return dwCode;
}


DWORD Check_CodeSafe2()
{
	DWORD* lpdwFunc;
	DWORD	dwCode;
	int cnt;

	lpdwFunc = (DWORD*)(dwStartFuncPoint ^ (dwFuncLen << 8));

	if (dwFuncLen > 200000)
		return FALSE;

	for (cnt = 0; cnt < 32; cnt++)
	{
		dwFuncSafeMemCode = dwFuncSafeMemCode + (lpdwFunc[dwFuncSafeMemCount++] * ((dwFuncSafeMemCount & 0x3) + 1));

		if (dwFuncSafeMemCount >= dwFuncLen)
		{
			dwFuncChkTime = GetCurrentTime();
			dwCode = dwFuncSafeMemCode ^ ((dwFuncChkTime << 16) | (dwFuncChkTime & 0xFFFF));
			dwFuncSafeCode = dwCode;

			Code_VramSave(dwCode, dwFuncChkTime, dwFuncSafeMemCode);

			dwFuncSafeMemCount = 0;
			dwFuncSafeMemCode = 0;
			lpdwFunc = 0;

			return TRUE;
		}
	}

	lpdwFunc = 0;
	return FALSE;
}


DWORD	GetSafeCode()
{
	return dwFuncSafeCode ^ ((dwFuncChkTime << 16) | (dwFuncChkTime & 0xFFFF));
}

DWORD	GetSafeCode2()
{
	DWORD	dwCode, dwFnChkTime, dwFnSafeCode;
	DWORD	dwOriginal;

	Code_VramLoad(&dwCode, &dwFnChkTime, &dwFnSafeCode);

	dwOriginal = dwCode ^ ((dwFnChkTime << 16) | (dwFnChkTime & 0xFFFF));

	if ((dwOriginal ^ dwFnSafeCode) == 0) return dwOriginal;
	return FALSE;
}

int	LoadOftenMeshPattern(char* szFileName)
{
	if (DynTempPatCnt < TEMP_DYNPAT_MAX)
	{
		DynTempPat[DynTempPatCnt] = smPattern.LoadCharactor(szFileName);
		if (DynTempPat[DynTempPatCnt])
		{
			DynTempPat[DynTempPatCnt]->UseCount = 99;
			DynTempPatCnt++;
			return TRUE;
		}
	}

	return FALSE;
}

int	LoadOftenFullPattern()
{
	int cnt, cnt2;
	int	ErrorCount;
	char	szBuff[128];

	ErrorCount = 0;

	for (cnt = 0; cnt < 8; cnt++) {
		for (cnt2 = 0; cnt2 < ARMOR_FILE_MAX; cnt2++) {
			strcpy_s(szBuff, szArmorDirectory);
			lstrcat(szBuff, szArmorNumFile[cnt][cnt2]);

			if (LoadOftenMeshPattern(szBuff) == FALSE) {
				ErrorCount++;
			}
		}
	}

	for (cnt = 0; cnt < 8; cnt++) {
		for (cnt2 = 0; cnt2 < ARMOR_FILE_MAX; cnt2++) {
			strcpy_s(szBuff, szArmorDirectory);
			lstrcat(szBuff, szArmorNumFile_Comic[cnt][cnt2]);

			if (LoadOftenMeshPattern(szBuff) == FALSE) {
				ErrorCount++;
			}
		}
	}


	for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
		for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {		//0,3 ¸Ó¸® ¹Ì¸® ¿Ã¸²
			if (LoadOftenMeshPattern(szModel_FighterFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_MechFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_ArcherFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_PikeFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_KnightFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_AtalantaFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_PristessFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_MagicanFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
		}
	}




	//////////////////// NPC Æú´õ ////////////////////
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	char	szFindPath[64];
	char	szFilePath[64];

	lstrcpy(szFindPath, szNpcDirectory);
	lstrcat(szFindPath, "*.ini");

	hFindHandle = FindFirstFile(szFindPath, &fd);
	if (hFindHandle != INVALID_HANDLE_VALUE) {
		while (1) {
			strcpy_s(szFilePath, szNpcDirectory);
			lstrcat(szFilePath, fd.cFileName);

			LoadOftenMeshPattern(szFilePath);

			//´ÙÀ½ ÆÄÀÏ Ã£À½
			if (FindNextFile(hFindHandle, &fd) == FALSE) break;
		}
	}
	FindClose(hFindHandle);


	return ErrorCount;
}



//ÀÚÁÖ»ç¿ëÇÏ´Â ¸ðµ¨µé ·Îµå
int LoadOftenModels()
{

	return TRUE;
}

char* szOftenNPC[] = {
	/*
		"char\\npc\\npc-gard-sword.INI",
		"char\\npc\\npc-gard-spear.INI",
		"char\\npc\\npc-01.ini",
		"char\\npc\\npc-02.INI",
		"char\\npc\\npc-04.ini",
		"char\\npc\\npc-06.ini",
		"char\\npc\\npc-09.ini",
	*/
		0
};

//Æú´õ ³»ÀÇ ini ÆÄÀÏ ªO¾Æ ÀÐ±â
int LoadFolder_Ini(char* szPath, char* szWildCard, char* szChgExt, int OftSave);

int Make_CharAll_SMD()
{
	LoadOftenFullPattern();
	LoadFolder_Ini("Char\\Npc", "*.ini", 0, 0);
	LoadFolder_Ini("Char\\Monster", "*.ini", 0, 0);

	return TRUE;
}


//NPC ¹Ì¸® ·Îµù
int LoadOftenNpc()
{
	//537°³
		//Make_CharAll_SMD();

#ifdef _W_SERVER
/*
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	char	*lpPath;
	char	szFindPath[64];
	char	szFilePath[64];
	char	szPath[64];


	lstrcpy( szFindPath , szNpcDirectory );
	lstrcat( szFindPath , "*.inx" );

	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
		while(1) {
			lstrcpy( szFilePath , szNpcDirectory );
			lstrcat( szFilePath, fd.cFileName );

			lpPath = ChangeFileExt( szFilePath , "ini" );
			lstrcpy( szPath , lpPath );

			LoadOftenMeshPattern(szPath);

			//´ÙÀ½ ÆÄÀÏ Ã£À½
			if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		}
	}
	FindClose( hFindHandle );
*/
	LoadFolder_Ini("Char\\Npc", "*.inx", "ini", TRUE);

#else

	int cnt;

	cnt = 0;
	while (1) {
		if (!szOftenNPC[cnt]) break;
		LoadOftenMeshPattern(szOftenNPC[cnt]);
		cnt++;
	}

#endif

	return TRUE;

}


//¾ó±¼ÆÄÀÏ ¹Ì¸® ·Îµù
int LoadOftenFace()
{
	/*
		HANDLE hFindHandle;
		WIN32_FIND_DATA	fd;

		char	szFindPath[64];
		char	szFilePath[64];

		lstrcpy( szFindPath , szArmorDirectory );
		lstrcat( szFindPath , "*.inf" );

		hFindHandle = FindFirstFile( szFindPath , &fd );
		if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
			while(1) {
				lstrcpy( szFilePath , szArmorDirectory );
				lstrcat( szFilePath, fd.cFileName );

				LoadOftenMeshPattern(szFilePath);

				//´ÙÀ½ ÆÄÀÏ Ã£À½
				if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
			}
		}
		FindClose( hFindHandle );
	*/
	int cnt, cnt2;
	int ErrorCount = 0;

	for (cnt = 0; cnt < 6; cnt++) {
		//for(cnt2=0;cnt2<3;cnt2++) {					//0,3 ¸Ó¸® ¹Ì¸® ¿Ã¸²
		for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {		//0,3 ¸Ó¸® ¹Ì¸® ¿Ã¸²
			if (LoadOftenMeshPattern(szModel_FighterFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_MechFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_ArcherFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_PikeFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_KnightFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_AtalantaFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_PristessFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
			if (LoadOftenMeshPattern(szModel_MagicanFaceName[cnt][cnt2]) == FALSE)
				ErrorCount++;
		}
	}

	if (ErrorCount)
		return FALSE;

	return TRUE;

}


//ÀÚÁÖ¾²´Â °©¿Ê ¹Ì¸® ·Îµù
int LoadOftenArmor()
{
	int cntW, cntH;
	char szName[128];

	for (cntH = 0; cntH < MAX_JOBNUM; cntH++) {
		for (cntW = 0; cntW < 31; cntW++) {
			if (cntW == 0 || cntW >= 10) {
				lstrcpy(szName, szArmorDirectory);
				lstrcat(szName, szArmorNumFile[cntH][cntW]);
				LoadOftenMeshPattern(szName);
			}
		}
	}

	return TRUE;
}

/*----------------------------------------------------------------------------*
*					    ÀÚÁÖ¾²´Â ¾ÆÀÌÅÛ ¹Ì¸® ·Îµù
*-----------------------------------------------------------------------------*/
int  LoadOftenItem()
{
	int cnt;
	char szBuff[256];

	for (cnt = 0; cnt < MAX_ITEM; cnt++) {
		if (sItem[cnt].DorpItem == NULL) continue;
		wsprintf(szBuff, "Image\\sinImage\\Items\\DropItem\\it%s.ASE", sItem[cnt].DorpItem);

#ifdef _CHECK_MESH
		LoadOftenMeshPatternCheckSum(szBuff);			//ÀÚÁÖ¾²´Â ¸Þ½Ã ·Îµå ( ¾ÆÀÌÅÛ¸Þ½¬ Á¶ÀÛ°Ë»ç ±â´É Ãß°¡ )
/*
		FILE  *fp = NULL;
		fp=fopen("c:\\Itemlist.txt","a+t");

		if(fp)
		{
			fprintf(fp,"%s\n",szBuff);
		}
		fclose(fp);
*/
#else
		LoadOftenMeshPattern(szBuff);
#endif
		if (sItem[cnt].CODE == (sinGG1 | sin01)) break;		//µ· ±×¸²ÀÌ ¸¶Áö¸·
	}

#ifdef _CHECK_MESH
	Send_OftPatSumToDataServer();							//¾ÆÀÌÅÛ¸Þ½¬ Á¶ÀÛ°Ë»ç °á°ú°ª Àü¼Û
#endif

	return TRUE;
}

int CheckOftenMeshTextureSwap()
{
	return TRUE;

	int cnt;

	for (cnt = 0; cnt < DynTempPatCnt; cnt++) {
		if (DynTempPat[cnt] && DynTempPat[cnt]->Pat && DynTempPat[cnt]->Pat->smMaterialGroup) {
			DynTempPat[cnt]->Pat->smMaterialGroup->CheckMatreialTextureSwap();
		}
	}
	return TRUE;
}

extern int isItemCostume;

int SetPlayerArmor(int armor)
{
	char szbuff[256];
	int jobNum;

	if (isItemCostume != TRUE)
	{
		if (PlayerArmor == armor) return FALSE;
		if (lpCurPlayer->PatLoading) return FALSE;
		if (!lpCurPlayer->smCharInfo.szModelName2[0]) return FALSE;
	}
	if (armor == -1) armor = 0;

	jobNum = lpCurPlayer->smCharInfo.JOB_CODE;
	if (armor > ARMOR_FILE_MAX - 1) armor = ARMOR_FILE_MAX - 1;
	if (jobNum > MAX_JOBNUM) jobNum = 0;

	strcpy_s(szbuff, szArmorDirectory);

	if (lpCurPlayer->smCharInfo.SizeLevel > 0x1000 && lpCurPlayer->smCharInfo.SizeLevel <= 0x1002)
		lstrcat(szbuff, szArmorNumFile_Comic[ArmorJobNum[jobNum]][armor]);
	else
		lstrcat(szbuff, szArmorNumFile[ArmorJobNum[jobNum]][armor]);

	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern(lpCurPlayer, szbuff, lpCurPlayer->smCharInfo.szModelName2);

	PlayerArmor = armor;

	return TRUE;
}

int SetChrArmor(smCHAR* lpChar, int ItemNum)
{
	char szbuff[256] = { 0 };
	int armor = 0;
	int jobNum = 0;

	if (!lpChar || lpChar == nullptr)
		return FALSE;

	if (!lpChar->DisplayFlag || lpChar->wStickItems[smPLAYBUFF_ITEM_BODY] == ItemNum) return FALSE;
	if (lpChar->PatLoading) return FALSE;
	if (!lpChar->smCharInfo.szModelName2[0]) return FALSE;

	int iExtraCostume = 0;
	if ((sItem[ItemNum].CODE & sinITEM_MASK2) == sinCA1 || (sItem[ItemNum].CODE & sinITEM_MASK2) == sinCA2)
		iExtraCostume = 59;

	if (((sItem[ItemNum].CODE & sinITEM_MASK3) >> 8) > 29)
		armor = (((sItem[ItemNum].CODE & sinITEM_MASK3) >> 8) - 17) + iExtraCostume;
	else
		armor = ((sItem[ItemNum].CODE & sinITEM_MASK3) >> 8) + iExtraCostume;

	jobNum = lpChar->smCharInfo.JOB_CODE;
	if (armor > ARMOR_FILE_MAX - 1)
		armor = ARMOR_FILE_MAX - 1;
	if (jobNum > MAX_JOBNUM) jobNum = 0;

	lstrcpy(szbuff, szArmorDirectory);

	if (lpChar->smCharInfo.SizeLevel > 0x1000 && lpChar->smCharInfo.SizeLevel <= 0x1002)
		lstrcat(szbuff, szArmorNumFile_Comic[ArmorJobNum[jobNum]][armor]);
	else
		lstrcat(szbuff, szArmorNumFile[ArmorJobNum[jobNum]][armor]);

	lpChar->PatLoading = TRUE;
	AddLoaderPattern(lpChar, szbuff, lpChar->smCharInfo.szModelName2);

	return TRUE;
}

extern int iCostumeFlag;

//¾ó±¼ º¹±¸¿ë ÄÚµå »ý¼º // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
DWORD GetHairSkinCode(int FaceCode, int HairCode)
{
	return ((FaceCode & 0xFF) << 8) | (HairCode & 0xFF) | ((((FaceCode + 1) + (HairCode + 1) * 3) & 0xFF) << 16);
}

//¾ó±¼ º¹±¸ ÄÚµå Á¤»óÀÎÁö È®ÀÎ // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
int CheckHairSkinCode(DWORD dwModelCode)
{
	if (dwModelCode)
	{
		int FaceCode = (dwModelCode >> 8) & 0xFF;
		int HairCode = dwModelCode & 0xFF;
		int Sum = (dwModelCode >> 16) & 0xFF;
		if ((((FaceCode + 1) + (HairCode + 1) * 3) & 0xFF) == Sum) return TRUE;
	}
	return FALSE;
}

//¾ó±¼ º¹±¸ // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
char* GetFaceHairSkin(smCHAR* lpChar)
{

	DWORD dwModelCode = lpChar->smCharInfo.ModelNameCode2;
	int FaceCode = (dwModelCode >> 8) & 0xFF;
	int HairCode = dwModelCode & 0xFF;

	if (CheckHairSkinCode(dwModelCode) == FALSE) return NULL;

	char* szHeadName = 0;

	switch (lpChar->smCharInfo.JOB_CODE) {
	case 1:		//ÆÄÀÌÅÍ
		szHeadName = szModel_FighterFaceName[FaceCode][HairCode];
		break;
	case 2:		//¸ÞÄ«´Ï¼Ç
		szHeadName = szModel_MechFaceName[FaceCode][HairCode];
		break;
	case 3:		//¾ÆÃÄ
		szHeadName = szModel_ArcherFaceName[FaceCode][HairCode];
		break;
	case 4:		//ÆÄÀÌÅ©¸Ç
		szHeadName = szModel_PikeFaceName[FaceCode][HairCode];
		break;
	case 5:		//¾ÆÅ»¶õÅ¸
		szHeadName = szModel_AtalantaFaceName[FaceCode][HairCode];
		break;
	case 6:		//³ªÀÌÆ®
		szHeadName = szModel_KnightFaceName[FaceCode][HairCode];
		break;
	case 7:		//¸ÅÁö¼Ç
		szHeadName = szModel_MagicanFaceName[FaceCode][HairCode];
		break;
	case 8:		//ÇÁ¸®½ºÆ¼½º
		szHeadName = szModel_PristessFaceName[FaceCode][HairCode];
		break;
	}

	return szHeadName;
}

// ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
int FindFaceSkin(smCHAR* lpChar, char* lpHeadName, char*** lpSelFace, char** lpSelFaceName)
{

	int cnt, cnt2;
	int FindCount = 0;
	int FaceCode;
	int HairCode;

	switch (lpChar->smCharInfo.JOB_CODE) {
	case 1:		//ÆÄÀÌÅÍ
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_FighterFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_FighterFaceName[cnt];
					*lpSelFaceName = szModel_FighterFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	case 2:		//¸ÞÄ«´Ï¼Ç
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_MechFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_MechFaceName[cnt];
					*lpSelFaceName = szModel_MechFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	case 3:		//¾ÆÃÄ
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_ArcherFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_ArcherFaceName[cnt];
					*lpSelFaceName = szModel_ArcherFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	case 4:		//ÆÄÀÌÅ©¸Ç
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_PikeFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_PikeFaceName[cnt];
					*lpSelFaceName = szModel_PikeFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;

		}
		break;
	case 5:		//¾ÆÅ»¶õÅ¸
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_AtalantaFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_AtalantaFaceName[cnt];
					*lpSelFaceName = szModel_AtalantaFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	case 6:		//³ªÀÌÆ®
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_KnightFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_KnightFaceName[cnt];
					*lpSelFaceName = szModel_KnightFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	case 7:		//¸ÅÁö¼Ç
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_MagicanFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_MagicanFaceName[cnt];
					*lpSelFaceName = szModel_MagicanFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	case 8:		//ÇÁ¸®½ºÆ¼½º
		for (cnt = 0; cnt < FACE_MODEL_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < HEAREPOTION_MAX; cnt2++) {
				if (lstrcmpi(szModel_PristessFaceName[cnt][cnt2], lpHeadName) == 0)
				{
					*lpSelFace = szModel_PristessFaceName[cnt];
					*lpSelFaceName = szModel_PristessFaceName[cnt][cnt2];
					FaceCode = cnt; HairCode = cnt2; FindCount++;
					break;
				}
			}
			if (cnt2 < HEAREPOTION_MAX) break;
		}
		break;
	}

	if (!FindCount) return -1;


	return (FaceCode << 8) | HairCode;

}

//¸ðµ¨ ½ºÅ² º¯°æ
int	ChangeModelSkin(smCHAR* lpChar, int ComicArmor, int ComicHeadNum, char* szArmor, char* szHead)
{
	char szArmorName[32];
	char szHeadName[32];
	char szbuff1[256];
	char szbuff2[256];

	int len, cnt; // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	int jobNum;
	int SelCount;
	int FindArmor, FindHead;
	char* lpArmorFile;
	char* lpHeadFile;

	// ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	char** lpSelFace = 0;
	char* lpSelFaceName = 0;

	jobNum = lpChar->smCharInfo.JOB_CODE;
	if (jobNum > MAX_JOBNUM) jobNum = 0;

	if (szArmor) lpArmorFile = szArmor;
	else lpArmorFile = lpChar->smCharInfo.szModelName;

	if (szHead) lpHeadFile = szHead;
	else lpHeadFile = lpChar->smCharInfo.szModelName2;

	szArmorName[0] = 0;
	szHeadName[0] = 0;

	len = lstrlen(lpArmorFile);
	for (cnt = len - 1; cnt >= 0; cnt--) {
		if (lpArmorFile[cnt] == '\\') {
			lstrcpy(szArmorName, &lpArmorFile[cnt + 1]);
			break;
		}
	}

	lstrcpy(szHeadName, lpHeadFile);


	if (!szArmorName[0] || !szHeadName[0]) return FALSE;


	SelCount = ComicHeadNum;

	FindArmor = 0;
	FindHead = 0;

	for (cnt = 0; cnt < ARMOR_FILE_MAX; cnt++) {
		if (lstrcmpi(szArmorName, szArmorNumFile[ArmorJobNum[jobNum]][cnt]) == 0) {
			if (ComicArmor)
				strcpy_s(szArmorName, szArmorNumFile_Comic[ArmorJobNum[jobNum]][cnt]);
			else
				strcpy_s(szArmorName, szArmorNumFile[ArmorJobNum[jobNum]][cnt]);

			FindArmor++;
			break;
		}
	}
	if (!FindArmor) {
		for (cnt = 0; cnt < ARMOR_FILE_MAX; cnt++) {
			if (lstrcmpi(szArmorName, szArmorNumFile_Comic[ArmorJobNum[jobNum]][cnt]) == 0) {
				if (ComicArmor)
					strcpy_s(szArmorName, szArmorNumFile_Comic[ArmorJobNum[jobNum]][cnt]);
				else
					strcpy_s(szArmorName, szArmorNumFile[ArmorJobNum[jobNum]][cnt]);

				FindArmor++;
				break;
			}
		}
	}

	// ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	int FindFaceValue = FindFaceSkin(lpChar, szHeadName, &lpSelFace, &lpSelFaceName);
	if (FindFaceValue < 0) return FALSE;
	int HairCode = FindFaceValue & 0xFF;
	int FaceCode = FindFaceValue >> 8;

	//º¹±¸¿ë ¾ó±¼ ÄÚµå ÃÖÃÊ »ý¼º
	if (CheckHairSkinCode(lpChar->smCharInfo.ModelNameCode2) == FALSE) {
		lpChar->smCharInfo.ModelNameCode2 = GetHairSkinCode(FaceCode, HairCode);
	}

	switch (jobNum)
	{
	case 1:		//ÆÄÀÌÅÍ
		strcpy_s(szHeadName, szModel_FighterFaceName[SelCount][HairCode]);
		break;
	case 2:		//¸ÞÄ«´Ï¼Ç
		strcpy_s(szHeadName, szModel_MechFaceName[SelCount][HairCode]);
		break;
	case 3:		//¾ÆÃÄ
		strcpy_s(szHeadName, szModel_ArcherFaceName[SelCount][HairCode]);
		break;
	case 4:		//ÆÄÀÌÅ©¸Ç
		strcpy_s(szHeadName, szModel_PikeFaceName[SelCount][HairCode]);
		break;
	case 5:		//¾ÆÅ»¶õÅ¸
		strcpy_s(szHeadName, szModel_AtalantaFaceName[SelCount][HairCode]);
		break;
	case 6:		//³ªÀÌÆ®
		strcpy_s(szHeadName, szModel_KnightFaceName[SelCount][HairCode]);
		break;
	case 7:		//¸ÅÁö¼Ç
		strcpy_s(szHeadName, szModel_MagicanFaceName[SelCount][HairCode]);
		break;
	case 8:		//ÇÁ¸®½ºÆ¼½º
		strcpy_s(szHeadName, szModel_PristessFaceName[SelCount][HairCode]);
		break;
	}


	strcpy_s(szbuff1, szArmorDirectory);
	lstrcat(szbuff1, szArmorName);
	strcpy_s(szbuff2, szHeadName);

	lpChar->PatLoading = TRUE;
	AddLoaderPattern(lpChar, szbuff1, szbuff2);

	return TRUE;
}

void ResetHairModel()
{
	char szDefaultHeadModel[260] = { 0 };

	char c[2] = { 0 };
	int r = (int)lpCurPlayer->smCharInfo.ChangeJob;
	if (r > 0)
		c[0] = ('a' + (r - 1));

	char cl[2] = { 0 };
	if (r == 2)
		cl[0] = '_';
	else
		cl[0] = '-';

	switch (lpCurPlayer->smCharInfo.JOB_CODE)
	{
	case 1:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\tmh%sB01%s.inf", cl, c);
		break;
	case 2:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\tmh%sA01%s.inf", cl, c);
		break;
	case 3:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\tfh%sD01%s.inf", cl, c);
		break;
	case 4:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\tmh%sC01%s.inf", cl, c);
		break;
	case 5:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\Mfh%sB01%s.inf", cl, c);
		break;
	case 6:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\Mmh%sA01%s.inf", cl, c);
		break;
	case 7:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\Mmh%sD01%s.inf", cl, c);
		break;
	case 8:
		wsprintf(szDefaultHeadModel, "char\\tmABCD\\Mfh%sC01%s.inf", cl, c);
		break;
	}

	char** lpSelFace = 0;
	char* lpSelFaceName = 0;

	int FindFaceValue = FindFaceSkin(lpCurPlayer, szDefaultHeadModel, &lpSelFace, &lpSelFaceName);

	if (FindFaceValue < 0)
		return;

	int HairCode = FindFaceValue & 0xFF;
	int FaceCode = FindFaceValue >> 8;

	lpCurPlayer->smCharInfo.ModelNameCode2 = GetHairSkinCode(FaceCode, HairCode);

	lstrcpy(lpCurPlayer->smCharInfo.szModelName2, szDefaultHeadModel);

	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern(lpCurPlayer, lpCurPlayer->smCharInfo.szModelName, szDefaultHeadModel);
	lpCurPlayer->DisplayTools = TRUE;

	ReformCharForm();

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);

	StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);

	SaveGameData();
}

int funcLuckyBox(int BoxType, DWORD ItemCode, DWORD ItemHead, DWORD ItemChkSum) {
	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = smTRANSCODE_LUCKY_BOX;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = BoxType;
	smTransCommand.SParam = ItemCode;
	smTransCommand.LParam = ItemHead;
	smTransCommand.EParam = ItemChkSum;

	if (smWsockDataServer)
		smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
	SaveGameData();
	StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
	return TRUE;

}

int getHairModel() {

	smCHAR* lpChar = lpCurPlayer;
	char* lpHeadName = lpChar->smCharInfo.szModelName2;
	char** lpSelFace = 0;
	char* lpSelFaceName = 0;

	int FindFaceValue = FindFaceSkin(lpChar, lpHeadName, &lpSelFace, &lpSelFaceName);

	return FindFaceValue;
}
extern int msgCabelo;

int ChangeHairModel(int HairCode)
{
	smCHAR* lpChar = lpCurPlayer;
	int jobNum = lpChar->smCharInfo.JOB_CODE;
	char* lpHeadName = lpChar->smCharInfo.szModelName2;
	char** lpSelFace = 0;
	char* lpSelFaceName = 0;

	// Bloqueia a troca de cabelo se estiver morto
	if (lpChar->MotionInfo->State == CHRMOTION_STATE_DEAD) {

		struct Pck
		{
			int size;
			int code;
		};

		Pck smPacket;
		ZeroMemory(&smPacket, sizeof(Pck));
		smPacket.code = 0x48476971;
		smPacket.size = sizeof(Pck);

		if (msgCabelo)
			smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);

		msgCabelo = 0;
		return FALSE;
	}

	if (HairCode < 0 || HairCode >= HEAREPOTION_MAX) return FALSE;

	int FindFaceValue = FindFaceSkin(lpChar, lpHeadName, &lpSelFace, &lpSelFaceName);

	if (FindFaceValue < 0 || lpSelFace == 0) return FALSE;

	int FaceCode = FindFaceValue >> 8;

	if (lstrcmpi(lpChar->smCharInfo.szModelName2, lpSelFace[HairCode]) == 0)
		return FALSE;

	lstrcpy(lpChar->smCharInfo.szModelName2, lpSelFace[HairCode]);
	lpChar->smCharInfo.ModelNameCode2 = GetHairSkinCode(FaceCode, HairCode);

	lpChar->PatLoading = TRUE;
	AddLoaderPattern(lpChar, lpChar->smCharInfo.szModelName, lpChar->smCharInfo.szModelName2);

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();
	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
	SaveGameData();

	StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);


	return TRUE;
}

const int  CONST_AGINGBCOLOR_MAX = 22;

// Cores do Aging
static short	AgingBlinkColor[CONST_AGINGBCOLOR_MAX][6] =
{
	{ 100, 40, 90, 0, -1, 0 },
	{ 120, 100, 10, 0, -1, 0 },
	{ 0, 110, 30, 0, -1, 0 },
	{ 0, 50, 140, 0, 0, smTEXSTATE_FS_SCROLL5 },
	{ 100, 0, 90, 0, 1, smTEXSTATE_FS_SCROLL6 },
	{ 150, 60, 0, 0, 2, smTEXSTATE_FS_SCROLL6 },
	{ 150, 10, 0, 0, 3, smTEXSTATE_FS_SCROLL8 },
	{ 20, 220, 190, 0, 4, smTEXSTATE_FS_SCROLL8 },
	{ 10, 220, 30, 0, 4, smTEXSTATE_FS_SCROLL8 },
	{ 170, 40, 170, 0, 4, smTEXSTATE_FS_SCROLL9 },
	{ 250, 30, 160, 0, 4, smTEXSTATE_FS_SCROLL9 },
	{ 30, 190, 255, 0, 4, smTEXSTATE_FS_SCROLL9 },
	{ 250, 130, 30, 0, 4, smTEXSTATE_FS_SCROLL9 },
	// { 120, 30, 30, 0, 5, smTEXSTATE_FS_SCROLL10 }, + 17 // Update 25/09/2019 + 18
	{ 120, 30, 30, 0, 5, smTEXSTATE_FS_SCROLL10 },
	//{ 51, 0, 51, 0, 5, smTEXSTATE_FS_SCROLL10 }, // + 18
	{ 130,	  0,	255,	0,	 5,	smTEXSTATE_FS_SCROLL10	}, // +18
	{ 220, 240, 70, 0, 5, smTEXSTATE_FS_SCROLL10 },
	{ 240, 240, 240, 0, 2, smTEXSTATE_FS_SCROLL10 },
	{ 1024, 256, 256, 0, 2, smTEXSTATE_FS_SCROLL10 },
	{ 256, 256, 640, 0, 2, smTEXSTATE_FS_SCROLL10 },
};

static short	CraftBlinkColor[6][6] =
{
	{   13,	  0,	  5,	0,	 9,	smTEXSTATE_FS_SCROLL4	},
	{	13,	  0,	  6,	0,	 5,	smTEXSTATE_FS_SCROLL4	},
	{	13,	  0,	  7,	0,	 6,	smTEXSTATE_FS_SCROLL5	},
	{	13,	  0,	  8,	0,	 7,	smTEXSTATE_FS_SCROLL5	},
	{	13,	  0,	  9,	0,	 8,	smTEXSTATE_FS_SCROLL5	},
	{   13,	  0,	  10,	0,	 9,	smTEXSTATE_FS_SCROLL5	}
};

extern BOOL isEnemy;

int GetItemKindFromBliankColor(smCHARTOOL* lpCharTool)
{
	return NULL;
}

extern sITEM InvenCostume;
extern BOOL bCostumeSlot;
extern int isItemCostume;

extern Graphics::Material* itemAgingMaterial[30];
extern Graphics::Material* itemMixingMaterial[20];

int sinSetCharItem(DWORD CODE, DWORD Posi, BOOL SetFlag)
{
	DWORD	dwCode = 0;
	sITEMINFO* lpItemInfo = nullptr;
	DWORD	dwMotionCode = 0;
	int		FrameCounter = 0;
	int		cnt = 0;

	switch (Posi)
	{
	case INVENTORY_POS_LHAND:
		if (SetFlag)
		{
			lpCurPlayer->SetTool(CODE, hvPOSI_LHAND);

			lpItemInfo = &cInvenTory.InvenItem[sInven[1].ItemIndex - 1].sItemInfo;
			if (!lpItemInfo->ItemName[0])
				lpItemInfo = &cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo;

			lpCurPlayer->HvLeftHand.ColorBlink = lpItemInfo->EffectBlink[0];
			memcpy(lpCurPlayer->HvLeftHand.sColors, lpItemInfo->EffectColor, sizeof(short) * 4);
			lpCurPlayer->HvLeftHand.DispEffect = lpItemInfo->DispEffect;
			lpCurPlayer->HvLeftHand.BlinkScale = lpItemInfo->ScaleBlink[0];
			lpCurPlayer->HvLeftHand.EffectKind = 0;
			lpCurPlayer->HvLeftHand.blendingMaterial = nullptr;
			lpCurPlayer->HvLeftHand.iAgeLevel = 0;
			lpCurPlayer->HvLeftHand.iMixID = 0;

			if (lpItemInfo->ItemKindCode == ITEM_KIND_AGING)
			{
				cnt = lpItemInfo->ItemAgingNum[0];
				if (cnt >= 4 && cnt <= CONST_AGINGBCOLOR_MAX)
				{
					lpCurPlayer->HvLeftHand.blendingMaterial = itemAgingMaterial[cnt - 4];
					lpCurPlayer->HvLeftHand.iAgeLevel = cnt;
				}

			}

			if (lpItemInfo->ItemKindCode == ITEM_KIND_CRAFT)
			{
				cnt = lpItemInfo->ItemAgingNum[0];
				if (cnt >= 0)
				{
					lpCurPlayer->HvLeftHand.ColorBlink = 9;
					lpCurPlayer->HvLeftHand.iMixID = cnt;
					lpCurPlayer->HvLeftHand.blendingMaterial = itemMixingMaterial[cnt];
				}
			}

			if (lpCurPlayer->HvLeftHand.DispEffect) AssaParticle_ClassUpWeapon(lpCurPlayer);


		}
		else
			lpCurPlayer->SetTool(0, hvPOSI_LHAND);
		break;

	case INVENTORY_POS_RHAND:
		if (SetFlag) {
			lpCurPlayer->SetTool(CODE, hvPOSI_RHAND);

			lpItemInfo = &cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo;
			lpCurPlayer->HvRightHand.ColorBlink = lpItemInfo->EffectBlink[0];
			memcpy(lpCurPlayer->HvRightHand.sColors, lpItemInfo->EffectColor, sizeof(short) * 4);
			lpCurPlayer->HvRightHand.DispEffect = lpItemInfo->DispEffect;
			lpCurPlayer->HvRightHand.BlinkScale = lpItemInfo->ScaleBlink[0];
			lpCurPlayer->HvRightHand.EffectKind = 0;
			lpCurPlayer->HvRightHand.blendingMaterial = nullptr;
			lpCurPlayer->HvRightHand.iAgeLevel = 0;
			lpCurPlayer->HvRightHand.iMixID = 0;


			if (lpItemInfo->ItemKindCode == ITEM_KIND_AGING)
			{
				cnt = lpItemInfo->ItemAgingNum[0];
				if (cnt >= 4 && cnt <= CONST_AGINGBCOLOR_MAX)
				{
					lpCurPlayer->HvRightHand.blendingMaterial = itemAgingMaterial[cnt - 4];
					lpCurPlayer->HvRightHand.iAgeLevel = cnt;
				}
			}

			if (lpItemInfo->ItemKindCode == ITEM_KIND_CRAFT)
			{

				cnt = lpItemInfo->ItemAgingNum[0];
				if (cnt >= 0)
				{
					lpCurPlayer->HvRightHand.ColorBlink = 9;
					lpCurPlayer->HvRightHand.iMixID = cnt;
					lpCurPlayer->HvRightHand.blendingMaterial = itemMixingMaterial[cnt];
				}
			}

			if (lpCurPlayer->HvRightHand.DispEffect) AssaParticle_ClassUpWeapon(lpCurPlayer);

		}
		else
			lpCurPlayer->SetTool(0, hvPOSI_RHAND);
		break;

	case INVENTORY_POS_ARMOR:
		dwMotionCode = lpCurPlayer->MotionInfo->State;
		FrameCounter = lpCurPlayer->FrameCounter;

		if (CODE != NULL)
		{
			if (SetFlag)
			{
				dwCode = CODE;

				if ((dwCode & sinITEM_MASK3) >> 8 > 29) // Char Armor
					SetPlayerArmor(((dwCode & sinITEM_MASK3) >> 8) - 17);
				else
					SetPlayerArmor((dwCode & sinITEM_MASK3) >> 8);

				lpCurPlayer->wStickItems[smPLAYBUFF_ITEM_BODY] = GetSinItemNumFromCode(dwCode);
			}
			else
			{
				SetPlayerArmor(1);
				lpCurPlayer->wStickItems[smPLAYBUFF_ITEM_BODY] = -1;
			}
			if (dwMotionCode == CHRMOTION_STATE_DEAD)
			{
				lpCurPlayer->SetMotionFromCode(dwMotionCode);
				lpCurPlayer->frame = (lpCurPlayer->MotionInfo->EndFrame - 1) * 160;
				lpCurPlayer->FrameCounter = FrameCounter;
			}
		}
		else
		{
			SetPlayerArmor(1);
			lpCurPlayer->wStickItems[smPLAYBUFF_ITEM_BODY] = -1;
		}

		break;
		// costume xxstr
	case INVENTORY_POS_COSTUME:

		if (cInvenTory.CostumeFlag)
		{
			dwMotionCode = lpCurPlayer->MotionInfo->State;
			FrameCounter = lpCurPlayer->FrameCounter;

			if (CODE != NULL)
			{
				if (SetFlag)
				{
					dwCode = CODE;
					if ((dwCode & sinITEM_MASK3) >> 8 > 25)
						SetPlayerArmor((((dwCode & sinITEM_MASK3) >> 8) - 17) + 59);
					else
						SetPlayerArmor(((dwCode & sinITEM_MASK3) >> 8) + 59);

					lpCurPlayer->wStickItems[smPLAYBUFF_ITEM_BODY] = GetSinItemNumFromCode(dwCode);
				}
				else
				{
					SetPlayerArmor(0);
					lpCurPlayer->wStickItems[smPLAYBUFF_ITEM_BODY] = -1;
				}

				if (dwMotionCode == CHRMOTION_STATE_DEAD)
				{
					lpCurPlayer->SetMotionFromCode(dwMotionCode);
					lpCurPlayer->frame = (lpCurPlayer->MotionInfo->EndFrame - 1) * 160;
					lpCurPlayer->FrameCounter = FrameCounter;
				}
			}
			else
			{
				SetPlayerArmor(0);
				lpCurPlayer->wStickItems[smPLAYBUFF_ITEM_BODY] = -1;
			}
		}
		break;
	}
	return TRUE;
}

//¾ÆÀÌÅÛ ¹ö¸®±â ÇÔ¼ö
int sinThrowItemToFeild(sITEM* pItem)
{

	sITEM	sThrowItem;
	int		ThrowItemBackup = 0;

	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI1 || (pItem->sItemInfo.CODE & sinITEM_MASK2) == sinBI2) {   //ÆÐÅ°Áö ¾ÆÀÌÅÛµµ ¹ö¸±¼ö¾ø´Ù
		if (pItem->sItemInfo.PotionCount == 0)
			return FALSE;
	}

	if (cWareHouse.OpenFlag)
	{
		if (!cWareHouse.CloseWareHouse()) {
			pItem->Flag = 0;
			return FALSE;
		}
	}

	if (Caravana::GetInstance()->OpenFlag)
	{
		if (!Caravana::GetInstance()->CloseCaravan()) {
			pItem->Flag = 0;
			return FALSE;
		}
	}

	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == QUEST_ARENA_FIELD) {
		//°æ±âÀå ÇÊµå (Æ÷½º¿Í ÃÊÄÚ·¿Àº »ç¿ë°¡´É)
		if ((pItem->sItemInfo.CODE & sinITEM_MASK2) != sinFO1 && (pItem->sItemInfo.CODE & sinITEM_MASK2) != sinCH1)
			return FALSE;	//¾ÆÀÌÅÛ ¹ö·ÁÁöÁö ¾Ê°Ô

		if (pItem->sItemInfo.PotionCount != 100)
			return FALSE;

	}

	if (pItem->sItemInfo.CODE != (sinGG1 | sin01) && !pItem->Flag) return FALSE;

	if (pItem->sItemInfo.PotionCount == 100) {

		//°ø¼º ¿ëº´ Å©¸®½ºÅ» Ã½Å©Áß
		int BlessCristalCode = -1;
		int BlessSolderTotal;

		switch (pItem->sItemInfo.CODE) {
		case sinGP1 | sin14:
			BlessCristalCode = 0;
			break;
		case sinGP1 | sin15:
			BlessCristalCode = 1;
			break;
		case sinGP1 | sin16:
			BlessCristalCode = 2;
			break;
		}
		if (BlessCristalCode >= 0) {
			if (!rsBlessCastle.CastleMode ||
				(lpCurPlayer->OnStageField >= 0 && sField[lpCurPlayer->OnStageField].FieldCode != rsCASTLE_FIELD) &&
				!lpCurPlayer->Clan_CastleMasterFlag)
			{
				//°ø¼º¸ðµå ¾Æ´Ï°Å´Ù »ç¿ëÇÒ¼ö ¾ø´Â À§Ä¡ ¶Ç´Â ¼ºÁÖÅ¬·£ÀÌ ¾Æ´Ô ( »ç¿ëºÒ°¡ )
				cMessageBox.ShowMessageEvent(mgBlessCrystal_03);
				return FALSE;
			}

			BlessSolderTotal = rsBlessCastle.bCrystalSolderCount[0] +
				rsBlessCastle.bCrystalSolderCount[1] +
				rsBlessCastle.bCrystalSolderCount[2] +
				rsBlessCastle.bCrystalSolderCount[3];

			if (rsBlessCastle.bCrystalSolderCount[BlessCristalCode] >= srCASLTE_SOLDER_KIND_MAX ||
				BlessSolderTotal >= srCASLTE_SOLDER_CLAN_MAX) {
				//¹ß»ý¼ö·® ÃÊ°ú
				cMessageBox.ShowMessageEvent(mgBlessCrystal_01);
				return FALSE;
			}
		}
	}

	if (!CheckItemForm(&pItem->sItemInfo)) {
		SendSetHackUser(80);
		return TRUE;
	}
	if (cShop.OpenFlag || SkillMasterFlag || cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag) {
		if (pItem->sItemInfo.CODE == (sinGG1 | sin01)) return FALSE;
	}

	if ((pItem->sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1))
	{
		ThrowItemBackup = 1;
		memcpy(&sThrowItem, pItem, sizeof(sITEM));
		ReformItem(&pItem->sItemInfo);
	}

	if (ThrowPutItem(pItem, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ) == TRUE) {
		SubInvenItemCode(&pItem->sItemInfo);
		pItem->Flag = 0;

		return TRUE;
	}

	if (ThrowItemBackup)
		memcpy(pItem, &sThrowItem, sizeof(sITEM));

	return FALSE;
}

int sinActionPotion()
{
	if (sinGetLife() > 0 && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_EAT
		&& lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
		if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_ATTACK || lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_SKILL)
			lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_EAT;
		else
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_EAT);
		return TRUE;
	}

	return FALSE;
}

int	ActionEtherCore(sITEM* lpItem)
{

	DWORD	dwCode = lpItem->sItemInfo.CODE;

	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM)
	{
		return TRUE;
	}
	if (lpCurPlayer->smCharInfo.wPlayerKilling[0])
	{
		return TRUE;
	}

	if (sinGetLife() > 0 && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {

		if (lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_ATTACK || lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_SKILL)
			lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_EAT;
		else {
			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_EAT);
			StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 48 * fONE, lpCurPlayer->pZ, EFFECT_RETURN1);
			SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);
		}

		UseEtherCoreCode = dwCode;

		if ((lpItem->sItemInfo.CODE & sinITEM_MASK2) == sinEC1) {
			//»ç¿ëÇÑ ¾ÆÀÌÅÛ ÄÚµå ¼­¹ö·Î º¸³¿
			SendUseItemCodeToServer(&lpItem->sItemInfo);
		}

		return TRUE;
	}

	return FALSE;
}


int GetDeathPenalty()
{
	sITEMINFO* lpItemInfo;

	int	Penalty = 0;

	if (sInven[4].ItemIndex) {
		lpItemInfo = &cInvenTory.InvenItem[sInven[4].ItemIndex - 1].sItemInfo;
		if (lpItemInfo->CODE == (sinOR2 | sin01) && lpItemInfo->ItemKindCode == ITEM_KIND_AGING) {
			if (lpItemInfo->ItemAgingNum[0] >= Penalty) Penalty = lpItemInfo->ItemAgingNum[0];
		}
	}

	if (sInven[5].ItemIndex) {
		lpItemInfo = &cInvenTory.InvenItem[sInven[5].ItemIndex - 1].sItemInfo;
		if (lpItemInfo->CODE == (sinOR2 | sin01) && lpItemInfo->ItemKindCode == ITEM_KIND_AGING) {
			if (lpItemInfo->ItemAgingNum[0] >= Penalty) Penalty = lpItemInfo->ItemAgingNum[0];
		}
	}

	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_CASTLE &&
		StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD)
	{
		return 0;
	}

	// Não cobra ressu na arena e não perde experiência
	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == FIELD_ARENA)
	{
		PlayUsed_Resurrection++;
		return 0;
	}

	lpItemInfo = FindItemFromCode(sinBI1 | sin04);

	if (lpItemInfo)
	{
		SendUseItemCodeToServer(lpItemInfo);
		cInvenTory.DeleteInvenItemToServer(lpItemInfo->CODE, lpItemInfo->ItemHeader.Head, lpItemInfo->ItemHeader.dwChkSum);
		PlayUsed_Resurrection++;
		return 0;
	}

	if (Penalty >= 8) return 100;
	if (Penalty >= 6) return 50;

	if ((sinQuest_levelLog & QUESTBIT_LEVEL_85) != 0 && lpCurPlayer->smCharInfo.Level >= 85)
	{
		return 50;
	}

	return 33;
}


INT64 ExpLevelTable[] = {
#include "..\\Shared\LevelTable.h"
	- 1 };

int LevelTableSum = 1508185620;

INT64 CodeXorCharInfo_Exp()
{
	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;

	INT64 exp1 = lpCurPlayer->smCharInfo.Exp;
	INT64 exp2 = lpCurPlayer->smCharInfo.Exp_High;

	return (exp2 << 32) | (exp1 & 0xFFFFFFFF);
}


int CheckLevelTable()
{
	int cnt = 0;
	int ChkSum = 0;

	while (1) {
		if (ExpLevelTable[cnt] == -1) break;
		ChkSum += ExpLevelTable[cnt] * (cnt + 1);
		cnt++;
	}

	return ChkSum;
}

int	CheckLevelExp(int Level, INT64 Exp)
{
	INT64 xp;
	INT64 np;

	if (Level > 0 && Level <= CHAR_LEVEL_MAX) {
		xp = ExpLevelTable[Level - 1];
		np = ExpLevelTable[Level];

		if (Exp >= xp && Exp <= np) {
			return TRUE;
		}

	}
	return FALSE;
}

//°æÇèÄ¡·Î ·¹º§ Ãß»ê
int	GetLevelFromExp(INT64 Exp)
{
	int	cnt;

	for (cnt = 0; cnt < CHAR_LEVEL_MAX; cnt++) {
		if (Exp >= ExpLevelTable[cnt] && Exp < ExpLevelTable[cnt + 1]) {
			return cnt + 1;
		}
	}

	return NULL;
}

//64ºñÆ® °æÇèÄ¡ ¹ÝÈ¯
INT64 GetExp64(smCHAR_INFO* smCharInfo)
{
	INT64 exp1, exp2;
	exp1 = (INT64)smCharInfo->Exp;

#ifdef _MODE_EXP64
	exp2 = (INT64)smCharInfo->Exp_High;
#else
	exp2 = 0;
#endif

	return (exp2 << 32) | (exp1 & 0xFFFFFFFF);
}


INT64 GetXorExp64(smCHAR_INFO* smCharInfo)
{
	INT64 exp1, exp2;

	exp1 = (INT64)(smCharInfo->Exp ^ CodeXorExp);
	exp2 = (INT64)(smCharInfo->Exp_High ^ CodeXorExp_High);

	return (exp2 << 32) | (exp1 & 0xFFFFFFFF);
}


int	SetExp64(smCHAR_INFO* smCharInfo, INT64 Exp64)
{
#ifdef _MODE_EXP64
	smCharInfo->Exp_High = (int)(Exp64 >> 32);
#endif
	smCharInfo->Exp = (int)(Exp64 & 0xFFFFFFFF);

	return smCharInfo->Exp;
}

bool bcanadd = false;
void setexp()
{
	bcanadd = true;
}

int AddExp(INT64 Exp)
{
	if (!bcanadd)
		return FALSE;

	INT64 exp64 = CodeXorCharInfo_Exp();
	//int mlevel = CHAR_LEVEL_MAX - 2;

	bool flag = false;
	// DEFINE O LEVEL MÁXIMO PARA GANHAR EXP NO CASO, 119.
	int mlevel = CHAR_LEVEL_MAX - 1;

	if (exp64 < ExpLevelTable[mlevel])
	{
		exp64 += Exp;
		flag = true;
	}

	if (flag)
	{
		char szBuf[64] = { 0 };
		char szTemp[32] = { 0 };

		NumLineComa64(Exp, szTemp);

		CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Notice, "> Você Ganhou %s de experiência!", szTemp);
	}

	SetExp64(&lpCurPlayer->smCharInfo, exp64);
	CodeXorCharInfo_Exp();
	ReformCharForm();

	int Level = lpCurPlayer->smCharInfo.Level;
	INT64 iexp = ExpLevelTable[Level];

	if (LevelTableSum != CheckLevelTable())
	{
		bcanadd = false;
		DisconnectFlag = GetCurrentTime();
		return FALSE;
	}

	while (iexp >= 0 && exp64 >= iexp && lpCurPlayer->smCharInfo.Level < CHAR_LEVEL_MAX)
	{

		lpCurPlayer->smCharInfo.Level++;
		lpCurPlayer->smCharInfo.Next_Exp = ExpLevelTable[Level + 1];
		StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 32 * fONE, lpCurPlayer->pZ, EFFECT_LEVELUP1);
		esPlaySound(7, 400);
		ReformCharForm();

		SendPlayUpdateInfo();
		SaveGameData();

		Level = lpCurPlayer->smCharInfo.Level;
		iexp = ExpLevelTable[Level];

		extern cCHARSTATUS cCharStatus;
		cCharStatus.CheckLevel();
	}

	bcanadd = false;

	return TRUE;
}

int CheckFieldContExp()
{
	int Level;
	INT64 LowExp;
	INT64 HighExp;
	INT64 exp;
	INT64 exp64;

	Level = lpCurPlayer->smCharInfo.Level;
	if (Level >= 11) {
		LowExp = ExpLevelTable[Level - 1];
		HighExp = ExpLevelTable[Level];
		exp = (HighExp - LowExp) / 100;

		exp64 = GetXorExp64(&lpCurPlayer->smCharInfo);

		exp = exp64 - exp;
		if (exp < LowExp) {
			return FALSE;
		}
	}
	else {
		if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_FOREST)
		{
			return FALSE;
		}
	}

	return TRUE;
}

INT64 GetNextExp(int Level)
{
	return ExpLevelTable[Level];
}

int DeadPlayerExp(smCHAR* lpChar)
{
	int Level;
	INT64 LowExp;
	INT64 HighExp;
	INT64 exp;
	INT64 ep;
	INT64 exp64;
	int Money;
	int ExpLower = 0;
	int	Penalty = GetDeathPenalty();

	Level = lpChar->smCharInfo.Level;
	if (Level >= 11) {
		LowExp = ExpLevelTable[Level - 1];
		HighExp = ExpLevelTable[Level];
		if (Penalty)
			exp = (HighExp - LowExp) / Penalty;
		else
			exp = 0;

		exp64 = CodeXorCharInfo_Exp();
		if (exp64 >= LowExp) {
			ep = exp64;
			exp64 -= exp;
			if (exp64 < LowExp) {
				exp64 = LowExp;
				ExpLower = TRUE;
			}
			SetExp64(&lpCurPlayer->smCharInfo, exp64);
			ep = ep - exp64;
			SetTotalSubExp((int)ep);
		}
		CodeXorCharInfo_Exp();

		if (Penalty)
			Money = lpChar->smCharInfo.Money / Penalty;
		else
			Money = 0;

		lpChar->smCharInfo.Money -= Money;

		if (lpChar->smCharInfo.Money < 0)
		{
			Money = -Money;
			lpChar->smCharInfo.Money = 0;
		}

		SetTotalSubMoney(Money);
		ReformCharForm();
		ResetInvenMoney();
	}

	return ExpLower;
}

int ContGameExp(smCHAR* lpChar)
{
	int Level;
	INT64 LowExp;
	INT64 HighExp;
	INT64 exp;
	INT64 ep;
	INT64 exp64;
	int Money;

	Level = lpChar->smCharInfo.Level;
	if (Level >= 11) {
		LowExp = ExpLevelTable[Level - 1];
		HighExp = ExpLevelTable[Level];
		exp = (HighExp - LowExp) / 100;

		exp64 = CodeXorCharInfo_Exp();

		if (exp64 > LowExp) {
			ep = exp64;
			exp64 -= exp;
			if (exp64 < LowExp)
				exp64 = LowExp;

			SetExp64(&lpCurPlayer->smCharInfo, exp64);

			ep = ep - exp64;
			SetTotalSubExp((int)ep);		//°æÇèÄ¡ Áõ°¨ ±â·Ï
		}

		CodeXorCharInfo_Exp();
		//lpChar->smCharInfo.Exp ^= CodeXorExp;		//°æÇèÄ¡ ÄÚµå¹ÝÀü

		Money = lpChar->smCharInfo.Money / 100;	//µ· 1% ¾ø¾îÁü
		lpChar->smCharInfo.Money -= Money;
		if (lpChar->smCharInfo.Money < 0) {
			Money = -Money;
			lpChar->smCharInfo.Money = 0;
		}

		SetTotalSubMoney(Money);	//µ· °¨¼Ò ±â·Ï

		//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
		ReformCharForm();
		ResetInvenMoney();		//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
	}

	return TRUE;
}



//¹æ¾îÀ² °è»ê
int GetPlayDefence(smCHAR_INFO* lpAttackCharInfo, smCHAR_INFO* lpDefenceCharInfo)
{
	int defence;

	float ar, def, dLv, aLv;
	int weight;

	//¹æ¾îÀ² °è»ê
	ar = (float)lpAttackCharInfo->Attack_Rating;
	aLv = (float)lpAttackCharInfo->Level;
	def = (float)lpDefenceCharInfo->Defence;
	dLv = (float)lpDefenceCharInfo->Level;
	weight = lpDefenceCharInfo->Weight[0];
	defence = (int)(((((ar / (ar + (def / 2))) - ((dLv - aLv) / 50))) * 100)) - (weight / 10000);

	if (defence > 95) defence = 95;

	return defence;
}


int RainMaterial[4][8];


int InitWeatherSystem()
{
	if (smConfig.TextureQuality >= 2) return FALSE;

	RainMaterial[0][0] = CreateTextureMaterial("rain\\ll.bmp", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ADDCOLOR);

	RainMaterial[1][0] = CreateTextureMaterial("rain\\45rain01.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[1][1] = CreateTextureMaterial("rain\\45rain02.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[1][2] = CreateTextureMaterial("rain\\45rain03.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[1][3] = CreateTextureMaterial("rain\\45rain04.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[1][4] = CreateTextureMaterial("rain\\45rain05.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[1][5] = CreateTextureMaterial("rain\\45rain06.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	RainMaterial[2][0] = CreateTextureMaterial("rain\\80rain01.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[2][1] = CreateTextureMaterial("rain\\80rain02.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[2][2] = CreateTextureMaterial("rain\\80rain03.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[2][3] = CreateTextureMaterial("rain\\80rain04.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[2][4] = CreateTextureMaterial("rain\\80rain05.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[2][5] = CreateTextureMaterial("rain\\80rain06.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	RainMaterial[3][0] = CreateTextureMaterial("rain\\90rain01.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[3][1] = CreateTextureMaterial("rain\\90rain02.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[3][2] = CreateTextureMaterial("rain\\90rain03.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[3][3] = CreateTextureMaterial("rain\\90rain04.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[3][4] = CreateTextureMaterial("rain\\90rain05.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	RainMaterial[3][5] = CreateTextureMaterial("rain\\90rain06.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	return TRUE;
}


extern int WeatherMode;

int DrawWeather(int ax)
{
	/*
		if ( WeatherMode==2 ) {
			if ( Thunder==0 && (Draw2DCounter&0x1FF)==0x80 ) {
				Thunder = timeGetTime();
				if ( ((Thunder>>4)&1 )==0 ) {
					PlayWav2( 2 , 0 , 400 , 2205 );
				}
				else
					Thunder=0;
			}
		}
	*/

	if (smConfig.TextureQuality >= 2) return FALSE;

	smFACE2D face2d;

	int tm;
	float fx;
	int RainMode;

	if (ax < ANGLE_45 - 256) RainMode = 0;
	else {
		if (ax > ANGLE_45 + 256) RainMode = 3;
		else {
			if (ax > ANGLE_45) RainMode = 2;
			else RainMode = 1;
		}
	}

	if (WeatherMode == 2) {

		if (lpCurPlayer->OnStageField >= 0 &&
			(StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_DUNGEON ||
				StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM))
			return TRUE;
		//	if ( WeatherMode<0 ) {
		//	if ( WeatherMode>=0 ) {
				//ºñ¿À´Â ³¯¾¾ ±×¸²
		/*
			if ( Thunder ) {
				tm = timeGetTime();
				if ( (Thunder+1000)<tm ) Thunder = 0;

				tm = ( tm>>1 )&0x7F;
				smRender.Color_R = tm;
				smRender.Color_G = tm;
				smRender.Color_B = tm;

			}
			else {
				smRender.Color_R = -32;
				smRender.Color_G = -32;
				smRender.Color_B = -32;
			}
		*/


		smRender.Init();

		switch (RainMode) {
		case 0:
			tm = timeGetTime();
			tm = (tm >> 2) & 0x3FF;
			fx = ((float)tm) / 256;
			/*
					face2d.TexRect.left = fx;
					face2d.TexRect.right = fx+4;
					face2d.TexRect.top = fx;
					face2d.TexRect.bottom = fx+0.8;
					face2d.width  = WinSizeX;
					face2d.height = WinSizeY;
					face2d.Transparency = 255;
					face2d.r = 255; face2d.g = 255; face2d.b = 255;
					face2d.x = 0;
					face2d.z = 64*6;
					face2d.y = 0;
					face2d.MatNum = RainMaterial[RainMode][0];
					smRender.AddFace2D_Direct ( &face2d );
			*/
			tm = timeGetTime();
			tm = (tm) & 0x3FF;
			fx = ((float)tm) / 256;

			face2d.TexRect.left = fx;
			face2d.TexRect.right = fx + 3;
			face2d.TexRect.top = fx;
			face2d.TexRect.bottom = fx + 0.5f;
			face2d.width = WinSizeX;
			face2d.height = WinSizeY;
			face2d.Transparency = 255;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = 0;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][0];
			smRender.AddFace2D_Direct(&face2d);

			break;

		case 1:
			tm = timeGetTime() >> 6;

			face2d.TexRect.left = 1;
			face2d.TexRect.right = 0;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY;
			face2d.Transparency = 160;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = 0;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][tm % 6];
			smRender.AddFace2D_Direct(&face2d);

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY;
			face2d.Transparency = 160;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = WinSizeX / 2;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][(tm + 3) % 6];
			smRender.AddFace2D_Direct(&face2d);
			break;

		case 2:

			tm = timeGetTime() >> 6;

			face2d.TexRect.left = 1;
			face2d.TexRect.right = 0;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY;
			face2d.Transparency = 128;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = 0;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][tm % 6];
			smRender.AddFace2D_Direct(&face2d);

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY;
			face2d.Transparency = 128;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = WinSizeX / 2;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][(tm + 3) % 6];
			smRender.AddFace2D_Direct(&face2d);
			break;

		case 3:
			tm = timeGetTime() >> 6;

			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY / 2;
			face2d.Transparency = 128;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = 0;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][tm % 6];
			smRender.AddFace2D_Direct(&face2d);

			face2d.TexRect.left = 1;
			face2d.TexRect.right = 0;
			face2d.TexRect.top = 0;
			face2d.TexRect.bottom = 1;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY / 2;
			face2d.Transparency = 128;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = WinSizeX / 2;
			face2d.z = 0;
			face2d.y = 0;
			face2d.MatNum = RainMaterial[RainMode][(tm + 3) % 6];
			smRender.AddFace2D_Direct(&face2d);

			/////////////////////////////////////////////////
			face2d.TexRect.left = 0;
			face2d.TexRect.right = 1;
			face2d.TexRect.top = 1;
			face2d.TexRect.bottom = 0;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY / 2;
			face2d.Transparency = 128;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = 0;
			face2d.z = 0;
			face2d.y = WinSizeY >> 1;
			face2d.MatNum = RainMaterial[RainMode][(tm + 1) % 6];
			smRender.AddFace2D_Direct(&face2d);

			face2d.TexRect.left = 1;
			face2d.TexRect.right = 0;
			face2d.TexRect.top = 1;
			face2d.TexRect.bottom = 0;
			face2d.width = WinSizeX / 2;
			face2d.height = WinSizeY / 2;
			face2d.Transparency = 128;
			face2d.r = 255; face2d.g = 255; face2d.b = 255;
			face2d.x = WinSizeX / 2;
			face2d.z = 0;
			face2d.y = WinSizeY >> 1;
			face2d.MatNum = RainMaterial[RainMode][(tm + 4) % 6];
			smRender.AddFace2D_Direct(&face2d);

		}

		smRender.SetClipStates(0);
		smRender.ClipRendFace();							// ÀüÃ¼ ·»´õ¸µ ÆäÀÌ½º¸¦ Å¬¸®ÇÎ
		smRender.RenderD3D();
	}

	return TRUE;
}



HANDLE	hLoadingThread = 0;
HBITMAP hBitmapLamp = 0;
HDC	hdcBitmapLamp = 0;
HBITMAP hBitmapLoadImage = 0;
HBITMAP hBitmapLoadImage2 = 0;
HDC	hdcBitmapLoadImage = 0;

int DispLoading;



//IMG LOADING XXSTR
int InitLoadingLamp(HWND hWnd)
{
	Utils_Log(LOG_DEBUG, "InitLoadingLamp");
	HDC hdc = GetDC(hWnd);
	hBitmapLamp = LoadBitmap(hinst, "IDB_LOADINGLAMP");
	hdcBitmapLamp = CreateCompatibleDC(hdc);
	SelectObject(hdcBitmapLamp, hBitmapLamp);

	hBitmapLoadImage = LoadBitmap(hinst, "IDB_LOADIMG");
	hdcBitmapLoadImage = CreateCompatibleDC(hdc);
	SelectObject(hdcBitmapLoadImage, hBitmapLoadImage);

	ReleaseDC(hWnd, hdc);

	return TRUE;
}

int CloseLoadingLamp()
{

	if (hdcBitmapLamp) DeleteDC(hdcBitmapLamp);
	if (hBitmapLamp) DeleteObject(hBitmapLamp);

	hdcBitmapLamp = 0; hBitmapLamp = 0;

	return TRUE;
}

DWORD dwLampThreadId;
DWORD dwCurrentThreadId;

int CloseDispLoading()
{

	DispLoading = FALSE;
	return TRUE;
}

#define DYN_REMOVER_MAX		64
#define DYN_REMOVER_MASK	(DYN_REMOVER_MAX-1)

smDPAT* DynRemoveList[DYN_REMOVER_MAX];
int DynRemoveCnt = 0;
int DynRemovePush = 0;

HANDLE	hRemoverThread = 0;

DWORD WINAPI RemoveDynaPatternThreadProc(void* pInfo)
{
	HANDLE	hThread;
	int Counter = 0;

	int RemoveCnt;

	hThread = GetCurrentThread();

	while (1) {
		if (quit) break;

		if ((DynRemovePush - DynRemoveCnt) > 32) {
			EnterCriticalSection(&cLoadSection);
			EnterCriticalSection(&cDrawSection);

			while (1) {
				RemoveCnt = DynRemoveCnt & DYN_REMOVER_MASK;
				smDPAT_Delete(DynRemoveList[RemoveCnt]);
				DynRemoveCnt++;
				if (DynRemovePush <= DynRemoveCnt) break;
			}

			LeaveCriticalSection(&cDrawSection);
			LeaveCriticalSection(&cLoadSection);
		}
		else
			SuspendThread(hThread);

		Counter++;
	}

	ExitThread(TRUE);
	return TRUE;
}

int RemoveDynPatSub()
{
	int RemoveCnt;

	while (1) {
		if ((DynRemovePush - DynRemoveCnt) > 32) {
			while (1) {
				RemoveCnt = DynRemoveCnt & DYN_REMOVER_MASK;
				smDPAT_Delete(DynRemoveList[RemoveCnt]);
				DynRemoveCnt++;
				if (DynRemovePush <= DynRemoveCnt) break;
			}
		}
		else
			break;
	}

	return TRUE;
}


int AddRemoveDynPattern(smDPAT* lpDinaPattern)
{
	if (ServerMode) {
		if (lpDinaPattern->UseCount > 1)
			smDPAT_Delete(lpDinaPattern);

		return TRUE;
	}

	DynRemoveList[DynRemovePush & DYN_REMOVER_MASK] = lpDinaPattern;
	DynRemovePush++;
	RemoveDynPatSub();

	return DynRemovePush;
}

int MatArrow;

int psDrawTexImage(smTEXTUREHANDLE* hTexHandle, float x, float y, float w, float h,
	float fx, float fy, float fw, float fh, int Transp, int specular);

int psDrawTexImage_Point(smTEXTUREHANDLE* hTexHandle, float x, float y, float w, float h,
	float fx, float fy, float fw, float fh, int Transp, int specular);

int psDrawTexImage2(smTEXTUREHANDLE* hTexHandle, float* x, float* y,
	float fx, float fy, float fw, float fh, int Transp, int specular);

int FieldMapDrawMode = 2;

int FieldMapDrawSize[3] = { 8 , 16 , 24 };
int FieldMapViewSize1 = 18 * 64 * fONE;
int FieldMapViewSize2 = 8 * 64 * fONE;

int MainFieldMap()
{
	if (FieldMapViewSize1 > FieldMapViewSize2) {
		FieldMapViewSize2 += 2048;
		if (FieldMapViewSize1 < FieldMapViewSize2)
			FieldMapViewSize2 = FieldMapViewSize1;
	}

	if (FieldMapViewSize1 < FieldMapViewSize2) {
		FieldMapViewSize2 -= 2048;
		if (FieldMapViewSize1 > FieldMapViewSize2)
			FieldMapViewSize2 = FieldMapViewSize1;
	}


	return TRUE;
}
extern int MatEachBox;

int	GetFieldMapCursor()
{
	int px = 656 + (WinSizeX - 800);
	int py = 426 + (WinSizeY - 600);

	if (WinSizeX <= 800 && CSKILL->OpenFlag) return FALSE;

	if (pCursorPos.x > px && pCursorPos.x<px + 128 && pCursorPos.y>py && pCursorPos.y < py + 128 && lpCurPlayer->dwTradeMsgCode == 0) {

		if (lpCurPlayer->OnStageField >= 0 &&
			(StageField[lpCurPlayer->OnStageField]->FieldCode == rsSOD_FIELD || StageField[lpCurPlayer->OnStageField]->FieldCode == QUEST_ARENA_FIELD)) {

			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

int psDrawTexImage(smTEXTUREHANDLE* hTexHandle, float x, float y, float w, float h,
	float fx, float fy, float fw, float fh, int Transp, int specular)
{
	D3DTLVERTEX tlVertex[4];
	int cnt;
	for (cnt = 0; cnt < 4; cnt++)

	{
		tlVertex[cnt].rhw = 1;
		tlVertex[cnt].color = D3DCOLOR_RGBA(255, 255, 255, Transp);
		tlVertex[cnt].specular = D3DCOLOR_RGBA(specular, specular, specular, 0);
		tlVertex[cnt].sz = 0;
	}
	tlVertex[0].sx = x;
	tlVertex[0].sy = y;
	tlVertex[0].tu = fx;
	tlVertex[0].tv = fy;
	tlVertex[1].sx = x;
	tlVertex[1].sy = y + h;
	tlVertex[1].tu = fx;
	tlVertex[1].tv = fh;
	tlVertex[2].sx = x + w;
	tlVertex[2].sy = y;
	tlVertex[2].tu = fw;
	tlVertex[2].tv = fy;
	tlVertex[3].sx = x + w;
	tlVertex[3].sy = y + h;
	tlVertex[3].tu = fw;
	tlVertex[3].tv = fh;
	MagFilter = true;
	DrawTexture(hTexHandle->lpD3DTexture, tlVertex, true);
	MagFilter = false;
	return TRUE;
}

int psDrawTexImage_Point(smTEXTUREHANDLE* hTexHandle, float x, float y, float w, float h,
	float fx, float fy, float fw, float fh, int Transp, int specular)
{
	D3DTLVERTEX tlVertex[4];
	int cnt;
	for (cnt = 0; cnt < 4; cnt++)

	{
		tlVertex[cnt].rhw = 1;
		tlVertex[cnt].color = D3DCOLOR_RGBA(255, 255, 255, Transp);
		tlVertex[cnt].specular = D3DCOLOR_RGBA(specular, specular, specular, 0);
		tlVertex[cnt].sz = 0;
	}
	tlVertex[0].sx = x;
	tlVertex[0].sy = y;
	tlVertex[0].tu = fx;
	tlVertex[0].tv = fy;
	tlVertex[1].sx = x;
	tlVertex[1].sy = y + h;
	tlVertex[1].tu = fx;
	tlVertex[1].tv = fh;
	tlVertex[2].sx = x + w;
	tlVertex[2].sy = y;
	tlVertex[2].tu = fw;
	tlVertex[2].tv = fy;
	tlVertex[3].sx = x + w;
	tlVertex[3].sy = y + h;
	tlVertex[3].tu = fw;
	tlVertex[3].tv = fh;
	DrawTexture(hTexHandle->lpD3DTexture, tlVertex, true);
	return TRUE;
}

int psDrawTexImage2(smTEXTUREHANDLE* hTexHandle, float* x, float* y, float fx, float fy, float fw, float fh, int Transp, int specular)
{

	D3DTLVERTEX tlVertex[4];
	int cnt;
	for (cnt = 0; cnt < 4; cnt++)

	{
		tlVertex[cnt].rhw = 1;
		tlVertex[cnt].color = D3DCOLOR_RGBA(255, 255, 255, Transp);
		tlVertex[cnt].specular = D3DCOLOR_RGBA(specular, specular, specular, 0);
		tlVertex[cnt].sz = 0;
	}
	tlVertex[0].sx = x[0];
	tlVertex[0].sy = y[0];
	tlVertex[0].tu = fx;
	tlVertex[0].tv = fy;
	tlVertex[1].sx = x[1];
	tlVertex[1].sy = y[1];
	tlVertex[1].tu = fx;
	tlVertex[1].tv = fh;
	tlVertex[2].sx = x[2];
	tlVertex[2].sy = y[2];
	tlVertex[2].tu = fw;
	tlVertex[2].tv = fy;
	tlVertex[3].sx = x[3];
	tlVertex[3].sy = y[3];
	tlVertex[3].tu = fw;
	tlVertex[3].tv = fh;
	DrawTexture(hTexHandle->lpD3DTexture, tlVertex, true);
	return TRUE;
}


//µ¥ÀÌÅ¸ ¾ÐÃà ( Z/NZ ¹æ½Ä )
int EecodeCompress(BYTE* lpSrcData, BYTE* lpDestData, int size, int DestSize)
{
	int EncCounter;
	int cnt;
	int Counter;
	int pSet;
	BYTE c1, c2;
	int lsize;

	Counter = 0;
	EncCounter = 4;
	lsize = size - 1;

	while (1) {
		if (Counter >= size) break;
		if (EncCounter >= DestSize) break;

		c1 = lpSrcData[Counter];
		if (Counter < lsize) c2 = lpSrcData[Counter + 1];
		else c2 = c1;

		if (c1 == 0 && c2 == 0) {
			// 0 ¹Ýº¹ È½¼ö ±¸ÇÏ¿© ¾ÐÃà
			for (cnt = 0; cnt < 0x7F; cnt++) {
				if (Counter >= size) break;
				if (lpSrcData[Counter] != 0) break;
				Counter++;
			}
			if (EncCounter >= DestSize) break;
			lpDestData[EncCounter++] = cnt | 0x80;
		}
		else {
			// 0ÀÌ ¾Æ´Ñ µ¥ÀÌÅ¸ ¹Ýº¹ È½¼ö ±¸ÇÏ¿© ¾ÐÃà
			pSet = EncCounter;
			EncCounter++;

			for (cnt = 0; cnt < 0x7F; cnt++) {

				c1 = lpSrcData[Counter];
				if (Counter < lsize) c2 = lpSrcData[Counter + 1];
				else c2 = c1;

				if (Counter >= size) break;
				if (EncCounter >= DestSize) break;
				if (c1 == 0 && c2 == 0) break;
				lpDestData[EncCounter++] = lpSrcData[Counter++];
			}
			lpDestData[pSet] = cnt;
		}
	}

	//¸ÇÃ³À½ DWORD´Â ¾ÐÃàµÈ ±æÀÌ
	((int*)lpDestData)[0] = EncCounter;

	return EncCounter;
}





int DecodeCompress(BYTE* lpSrcData, BYTE* lpDestData, int dCount)
{

	int DecCounter;
	int cnt;
	int Counter;
	int size;
	int len;

	size = ((int*)lpSrcData)[0];

	Counter = 4;
	DecCounter = 0;

	while (1) {
		if (Counter >= size) break;
		if (lpSrcData[Counter] & 0x80) {
			//0 ¹Ýº¹ µ¥ÀÌÅ¸
			len = lpSrcData[Counter++] & 0x7F;
			for (cnt = 0; cnt < len; cnt++) {
				if (DecCounter >= dCount) return Counter;
				lpDestData[DecCounter++] = 0;
			}

		}
		else {
			len = lpSrcData[Counter++] & 0x7F;
			for (cnt = 0; cnt < len; cnt++) {
				if (DecCounter >= dCount) return Counter;
				lpDestData[DecCounter++] = lpSrcData[Counter++];
			}
		}
	}

	return Counter;
}


//¾ÐÃàµ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
int DecodeCompress(BYTE* lpSrcData, BYTE* lpDestData)
{
	return DecodeCompress(lpSrcData, lpDestData, 0x7FFFF);
}
//µ¥ÀÌÅ¸ ¾ÐÃà ( Z/NZ ¹æ½Ä )
int EecodeCompress(BYTE* lpSrcData, BYTE* lpDestData, int size)
{
	return EecodeCompress(lpSrcData, lpDestData, size, 0x7FFFF);
}

//´Ù¸¥ Ä³¸¯ÅÍ¿ÍÀÇ À§Ä¡ °ãÄ¡´ÂÁö È®ÀÎ
smCHAR* CheckOtherPlayPosi(int x, int y, int z)
{
	int cnt;
	int dist;
	int dx, dz;

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.Life[0] > 0 &&
			(chrOtherPlayer[cnt].dwLastTransTime + DISPLAY_TRANS_TIME_OVER) > dwPlayTime) {
			if ((y - 16 * fONE) < chrOtherPlayer[cnt].pY && (y + 16 * fONE) > chrOtherPlayer[cnt].pY) {

				dx = (chrOtherPlayer[cnt].pX - x) >> FLOATNS;
				dz = (chrOtherPlayer[cnt].pZ - z) >> FLOATNS;
				dist = dx * dx + dz * dz;
				if (dist < (16 * 16)) return &chrOtherPlayer[cnt];
			}
		}
	}

	return NULL;
}



//·ÎµùÁß ¸Þ¼¼Áö Ã³¸®
int MessageLoading = 0;

//À©µµ ¸Þ¼¼Áö Ã³¸® ÇÔ¼ö
int smPlayWindowMessage()
{
	MSG msg;

	MessageLoading = TRUE;

	while (1) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {

			TranslateMessage(&msg);
			if (msg.message == WM_SYSKEYDOWN || msg.message == WM_SYSKEYUP) continue;
			DispatchMessage(&msg);
		}
		else
			break;
	}

	MessageLoading = FALSE;
	return TRUE;
}

/*
struct sITEMINFO{

	DWORD	dwSize;						//¾ÆÀÌÅÛ ±¸Á¶ Å©±â

	sITEM_CREATE	ItemHeader;			//¾ÆÀÌÅÛ »ý¼º Á¤º¸

	short	Durability[2];				//³»±¸·Â

	DWORD	CODE;				//¾ÆÀÌÅÛ ÄÚµå
	char	ItemName[32];		//¾ÆÀÌÅÛ ÀÌ¸§

	int     Weight;				//¹«°Ô
	int     Price;				//°¡°Ý

	/////////////////////////////////////////
	int		Index;				//ÀÎµ¦½º
	int		PotionCount;		//Æ÷¼ÇÄ«¿îÅÍ
	////////////////////////////////////////

	short	Resistance[8];		//¿ø¼Ò¿¡´ëÇÑ ÀúÇ×·Â

	int		Sight;				//½Ã¾ß

	DWORD	Temp0;

	short	Damage[2];			//°ø°Ý·Â
	int     Shooting_Range;		//»çÁ¤°Å¸®
	int     Attack_Speed;		//°ø°Ý¼Óµµ
	int     Attack_Rating;		//¸íÁß·Â
	int     Critical_Hit;		//1.5¹è µ¥¹ÌÁöÈ®À²


	float	fAbsorb;				//Èí¼ö·Â
	int     Defence;			//¹æ¾î·Â
	float   fBlock_Rating;		//(¹æÆÐ)ºí·°À²

	float     fSpeed;				//ÀÌµ¿ ¼Óµµ


	int		Potion_Space;		//¼ÒÄÏ °ø°£ ÇÒ´ç

	float   fMagic_Mastery;     //¸¶¹ý¼÷·Ãµµ
	float   fMana_Regen;		//¸¶³ª Àç»ý
	float   fLife_Regen;		//¶óÀÌÇÁ Àç»ý
	float   fStamina_Regen;	    //½ºÅ×¹Ì³ª Àç»ý
	float   fIncrease_Life;     //¶óÀÌÇÁ ÃÖ´ëÄ¡»ó½Â
	float   fIncrease_Mana;     //¸¶³ª ÃÖ´ëÄ¡»ó½Â
	float   fIncrease_Stamina;  //½ºÅ×¹Ì³ª ÃÖ´ëÄ¡»ó½Â

	int     Level;				//·¹º§
	int     Strength;			//Èû
	int     Spirit;				//Á¤½Å·Â
	int     Talent;				//Àç´É
	int     Dexterity;			//¹ÎÃ¸¼º
	int     Health;				//°Ç°­

	short   Mana[2];            //¸¶³ªÈ¸º¹(ÃÖ¼Ò)(ÃÖ´ë)
	short   Life[2];            //¶óÀÌÇÁÈ¸º¹(ÃÖ¼Ò)(ÃÖ´ë)
	short   Stamina[2];         //½ºÅ×¹Ì³ÊÈ¸º¹(ÃÖ¼Ò)(ÃÖ´ë)


	int		Money;				//º¸À¯ÇÑ µ·
	int     NotUseFlag;         //»ç¿ëºÒ°¡´É ÇÃ·¢


	DWORD	dwTemp1[7];			//¿¹ºñ

	DWORD	DispEffect;			//¾ÆÀÌÅÛ Ç¥½Ã ÀÌÆåÆ® (¿¹Á¤)


	DWORD			JobCodeMask;		//Æ¯È­ Á÷¾÷	(Á÷¾÷º° ºñÆ® ¸¶½ºÅ©)
	sITEM_SPECIAL	JobItem;			//Æ¯È­ ¾ÆÀÌÅÛ ¼º´É

	DWORD	dwTemp[16];			//¿¹ºñ
};
*/
static DWORD iFormCode;
static DWORD iFromCnt;

static DWORD ReformStateCode(char* lpState, int size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++) {
		iFormCode += ((DWORD)lpState[cnt]) * ((iFromCnt & 0x1FFF) + 1);
		iFromCnt++;
	}

	return iFormCode;
}

static DWORD CompareStateCode(char* lpState1, char* lpState2, int size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++) {
		if (lpState1[cnt] != lpState2[cnt])
			return 1;
	}

	return 0;
}


//±¸¹öÀü ÀÎÁõ Ã³¸®
static DWORD GetRefromItemCode_Old(sITEMINFO* lpItemInfo, int Key) {

	iFormCode = 0;
	iFromCnt = Key;

	ReformStateCode((char*)&lpItemInfo->CODE, sizeof(DWORD));
	ReformStateCode((char*)lpItemInfo->ItemName, 32);

	ReformStateCode((char*)&lpItemInfo->Weight, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Price, sizeof(int));

	ReformStateCode((char*)lpItemInfo->Resistance, sizeof(short) * 8);
	ReformStateCode((char*)lpItemInfo->Damage, sizeof(short) * 2);

	//ReformStateCode( (char *)&lpItemInfo->Defence , sizeof(int) );

	ReformStateCode((char*)&lpItemInfo->Shooting_Range, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Attack_Speed, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->Attack_Rating, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Critical_Hit, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->fSpeed, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->Potion_Space, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->fMagic_Mastery, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fMana_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fLife_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fStamina_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Life, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Mana, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Stamina, sizeof(float));

	ReformStateCode((char*)&lpItemInfo->Level, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Strength, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Spirit, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Talent, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Dexterity, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Health, sizeof(int));

	ReformStateCode((char*)lpItemInfo->Mana, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->Life, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->Stamina, sizeof(short) * 2);

	ReformStateCode((char*)&lpItemInfo->Money, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->JobCodeMask, sizeof(DWORD));

	ReformStateCode((char*)&lpItemInfo->JobItem, sizeof(sITEM_SPECIAL));

	return iFormCode;
}



static DWORD GetRefromItemCode_V10(sITEMINFO* lpItemInfo, int Key) {

	iFormCode = 0;
	iFromCnt = Key;

	ReformStateCode((char*)&lpItemInfo->CODE, sizeof(DWORD));
	ReformStateCode((char*)lpItemInfo->ItemName, 32);

	ReformStateCode((char*)&lpItemInfo->Weight, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Price, sizeof(int));

	ReformStateCode((char*)lpItemInfo->Resistance, sizeof(short) * 8);
	ReformStateCode((char*)lpItemInfo->Damage, sizeof(short) * 2);

	ReformStateCode((char*)&lpItemInfo->Defence, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->Shooting_Range, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Attack_Speed, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->Attack_Rating, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Critical_Hit, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->fSpeed, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->Potion_Space, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->fMagic_Mastery, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fMana_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fLife_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fStamina_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Life, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Mana, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Stamina, sizeof(float));

	ReformStateCode((char*)&lpItemInfo->Level, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Strength, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Spirit, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Talent, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Dexterity, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Health, sizeof(int));

	ReformStateCode((char*)lpItemInfo->Mana, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->Life, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->Stamina, sizeof(short) * 2);

	ReformStateCode((char*)&lpItemInfo->Money, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->JobCodeMask, sizeof(DWORD));

	ReformStateCode((char*)&lpItemInfo->JobItem, sizeof(sITEM_SPECIAL));

	return iFormCode;
}

//static DWORD CompareStateCode( char *lpState1 , char *lpState2 , int size )

static int CompareReformItemCode_V10(sITEMINFO* lpItemInfo1, sITEMINFO* lpItemInfo2)
{

	int code;

	code = 0;

	code += CompareStateCode((char*)&lpItemInfo1->CODE, (char*)&lpItemInfo2->CODE, sizeof(DWORD));
	code += CompareStateCode((char*)lpItemInfo1->ItemName, (char*)lpItemInfo2->ItemName, 32);

	code += CompareStateCode((char*)&lpItemInfo1->Weight, (char*)&lpItemInfo2->Weight, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Price, (char*)&lpItemInfo2->Price, sizeof(int));

	code += CompareStateCode((char*)lpItemInfo1->Resistance, (char*)lpItemInfo2->Resistance, sizeof(short) * 8);
	code += CompareStateCode((char*)lpItemInfo1->Damage, (char*)lpItemInfo2->Damage, sizeof(short) * 2);

	code += CompareStateCode((char*)&lpItemInfo1->Defence, (char*)&lpItemInfo2->Defence, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->Shooting_Range, (char*)&lpItemInfo2->Shooting_Range, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Attack_Speed, (char*)&lpItemInfo2->Attack_Speed, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->Attack_Rating, (char*)&lpItemInfo2->Attack_Rating, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Critical_Hit, (char*)&lpItemInfo2->Critical_Hit, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->fSpeed, (char*)&lpItemInfo2->fSpeed, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->Potion_Space, (char*)&lpItemInfo2->Potion_Space, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->fMagic_Mastery, (char*)&lpItemInfo2->fMagic_Mastery, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fMana_Regen, (char*)&lpItemInfo2->fMana_Regen, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fLife_Regen, (char*)&lpItemInfo2->fLife_Regen, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fStamina_Regen, (char*)&lpItemInfo2->fStamina_Regen, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fIncrease_Life, (char*)&lpItemInfo2->fIncrease_Life, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fIncrease_Mana, (char*)&lpItemInfo2->fIncrease_Mana, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fIncrease_Stamina, (char*)&lpItemInfo2->fIncrease_Stamina, sizeof(float));

	code += CompareStateCode((char*)&lpItemInfo1->Level, (char*)&lpItemInfo2->Level, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Strength, (char*)&lpItemInfo2->Strength, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Spirit, (char*)&lpItemInfo2->Spirit, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Talent, (char*)&lpItemInfo2->Talent, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Dexterity, (char*)&lpItemInfo2->Dexterity, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Health, (char*)&lpItemInfo2->Health, sizeof(int));

	code += CompareStateCode((char*)lpItemInfo1->Mana, (char*)lpItemInfo2->Mana, sizeof(short) * 2);
	code += CompareStateCode((char*)lpItemInfo1->Life, (char*)lpItemInfo2->Life, sizeof(short) * 2);
	code += CompareStateCode((char*)lpItemInfo1->Stamina, (char*)lpItemInfo2->Stamina, sizeof(short) * 2);

	code += CompareStateCode((char*)&lpItemInfo1->Money, (char*)&lpItemInfo2->Money, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->JobCodeMask, (char*)&lpItemInfo2->JobCodeMask, sizeof(DWORD));

	code += CompareStateCode((char*)&lpItemInfo1->JobItem, (char*)&lpItemInfo2->JobItem, sizeof(sITEM_SPECIAL));

	return code;
}

static DWORD GetRefromItemCode(sITEMINFO* lpItemInfo, int Key) {

	iFormCode = 0;
	iFromCnt = Key;

	ReformStateCode((char*)&lpItemInfo->CODE, sizeof(DWORD));
	ReformStateCode((char*)lpItemInfo->ItemName, 32);

	ReformStateCode((char*)&lpItemInfo->Weight, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Price, sizeof(int));

	ReformStateCode((char*)lpItemInfo->Resistance, sizeof(short) * 8);
	ReformStateCode((char*)lpItemInfo->Damage, sizeof(short) * 2);

	ReformStateCode((char*)&lpItemInfo->fAbsorb, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->Defence, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->fBlock_Rating, sizeof(float));

	ReformStateCode((char*)&lpItemInfo->Shooting_Range, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Attack_Speed, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->Attack_Rating, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Critical_Hit, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->fSpeed, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->Potion_Space, sizeof(int));

	ReformStateCode((char*)&lpItemInfo->fMagic_Mastery, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fMana_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fLife_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fStamina_Regen, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Life, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Mana, sizeof(float));
	ReformStateCode((char*)&lpItemInfo->fIncrease_Stamina, sizeof(float));

	ReformStateCode((char*)&lpItemInfo->Level, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Strength, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Spirit, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Talent, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Dexterity, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->Health, sizeof(int));

	ReformStateCode((char*)lpItemInfo->Mana, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->Life, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->Stamina, sizeof(short) * 2);

	ReformStateCode((char*)&lpItemInfo->Money, sizeof(int));
	ReformStateCode((char*)&lpItemInfo->JobCodeMask, sizeof(DWORD));
	ReformStateCode((char*)&lpItemInfo->JobItem, sizeof(sITEM_SPECIAL));

	ReformStateCode((char*)&lpItemInfo->UniqueItem, sizeof(DWORD));
	ReformStateCode((char*)lpItemInfo->EffectBlink, sizeof(short) * 2);
	ReformStateCode((char*)lpItemInfo->EffectColor, sizeof(short) * 4);
	ReformStateCode((char*)lpItemInfo->ScaleBlink, sizeof(short) * 2);
	ReformStateCode((char*)&lpItemInfo->DispEffect, sizeof(DWORD));

	return iFormCode;
}


///////////////////// ¼­¹ö¿ë /////////////////////////////
#ifdef _W_SERVER

static DWORD rsReformStateCode(char* lpState, int size)
{
	int cnt;

	for (cnt = 0; cnt < size; cnt++) {
		iFormCode += ((DWORD)lpState[cnt]) * ((iFromCnt & 0x7FF) + 1);
		iFromCnt++;
	}

	return iFormCode;
}


static DWORD rsGetRefromItemCode(sITEMINFO* lpItemInfo, int Key) {

	iFormCode = 0;
	iFromCnt = Key;

	rsReformStateCode((char*)&lpItemInfo->CODE, sizeof(DWORD));
	rsReformStateCode((char*)lpItemInfo->ItemName, 32);

	rsReformStateCode((char*)&lpItemInfo->Weight, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Price, sizeof(int));

	rsReformStateCode((char*)lpItemInfo->Resistance, sizeof(short) * 8);
	rsReformStateCode((char*)lpItemInfo->Damage, sizeof(short) * 2);

	rsReformStateCode((char*)&lpItemInfo->fAbsorb, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->Defence, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->fBlock_Rating, sizeof(float));

	rsReformStateCode((char*)&lpItemInfo->Shooting_Range, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Attack_Speed, sizeof(int));

	rsReformStateCode((char*)&lpItemInfo->Attack_Rating, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Critical_Hit, sizeof(int));

	rsReformStateCode((char*)&lpItemInfo->fSpeed, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->Potion_Space, sizeof(int));

	rsReformStateCode((char*)&lpItemInfo->fMagic_Mastery, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->fMana_Regen, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->fLife_Regen, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->fStamina_Regen, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->fIncrease_Life, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->fIncrease_Mana, sizeof(float));
	rsReformStateCode((char*)&lpItemInfo->fIncrease_Stamina, sizeof(float));

	rsReformStateCode((char*)&lpItemInfo->Level, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Strength, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Spirit, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Talent, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Dexterity, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->Health, sizeof(int));

	rsReformStateCode((char*)lpItemInfo->Mana, sizeof(short) * 2);
	rsReformStateCode((char*)lpItemInfo->Life, sizeof(short) * 2);
	rsReformStateCode((char*)lpItemInfo->Stamina, sizeof(short) * 2);

	rsReformStateCode((char*)&lpItemInfo->Money, sizeof(int));
	rsReformStateCode((char*)&lpItemInfo->JobCodeMask, sizeof(DWORD));
	rsReformStateCode((char*)&lpItemInfo->JobItem, sizeof(sITEM_SPECIAL));

	rsReformStateCode((char*)&lpItemInfo->UniqueItem, sizeof(DWORD));
	rsReformStateCode((char*)lpItemInfo->EffectBlink, sizeof(short) * 2);
	rsReformStateCode((char*)lpItemInfo->EffectColor, sizeof(short) * 4);
	rsReformStateCode((char*)lpItemInfo->ScaleBlink, sizeof(short) * 2);
	ReformStateCode((char*)&lpItemInfo->DispEffect, sizeof(DWORD));

	return iFormCode;
}

#ifdef _NEW_ITEM_FORMCODE3
#include "ItemForm2.h"
#else
#include "ItemForm.h"
#endif

//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ°ª ¾ò±â
int	rsGetItemForm_Server(sITEMINFO* lpItemInfo)
{

	return rsGetItem_ServerForm(lpItemInfo);
	/*
		DWORD dwCode;

	#ifdef	_LANGUAGE_CHINESE
		//Áß±¹ÆÇ »õ ÀÎÁõ¹æ¹ý

		//¼­¹ö¿ë Àç ÀÎÁõ ¾ÆÀÌÅÛ ( ½Å¹öÀü ÀÎÁõ )
		dwCode = rsGetRefromItemCode2( lpItemInfo , lpItemInfo->ItemHeader.dwChkSum );
		dwCode = dwCode ^ ((lpItemInfo->ItemAgingProtect[0]*7)^(lpItemInfo->ItemAgingProtect[1]*9));	//Ãß°¡ (¹Í½º/¿¡ÀÌÂ¡) ÇÁ·ÎÅØÆ®
		dwCode+= (lpItemInfo->dwCreateTime&0xFFFF)<<2;
		dwCode = dwCode ^ (lpItemInfo->ItemHeader.Head^((lpItemInfo->ItemHeader.dwChkSum+dwCode)<<15));

	#else

		//¼­¹ö¿ë Àç ÀÎÁõ ¾ÆÀÌÅÛ
		dwCode = rsGetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.dwChkSum );

	#ifdef	_NEW_ITEM_FORMCODE
		dwCode = dwCode ^ ((lpItemInfo->ItemAgingProtect[0]*7)^(lpItemInfo->ItemAgingProtect[1]*9));	//Ãß°¡ (¹Í½º/¿¡ÀÌÂ¡) ÇÁ·ÎÅØÆ®
		dwCode+= lpItemInfo->dwCreateTime>>8;
	#endif

		dwCode = dwCode ^ lpItemInfo->ItemHeader.Head;

	#endif

		return dwCode;
	*/
}


//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ¸¸ ¹Þ±â
int	rsReformItem_Server(sITEMINFO* lpItemInfo)
{
	DWORD dwCode;

	//¼­¹ö¿ë Àç ÀÎÁõ ¾ÆÀÌÅÛ
	dwCode = rsGetItemForm_Server(lpItemInfo);

	lpItemInfo->ItemHeader.dwTime = dwCode;

	return TRUE;
}
#else
int	rsGetItemForm_Server(sITEMINFO* lpItemInfo)
{

	return TRUE;
}

int	rsReformItem_Server(sITEMINFO* lpItemInfo)
{

	return TRUE;
}


#endif

//static DWORD CompareStateCode( char *lpState1 , char *lpState2 , int size )

static int CompareReformItemCode(sITEMINFO* lpItemInfo1, sITEMINFO* lpItemInfo2)
{

	int code;

	code = 0;

	code += CompareStateCode((char*)&lpItemInfo1->CODE, (char*)&lpItemInfo2->CODE, sizeof(DWORD));
	code += CompareStateCode((char*)lpItemInfo1->ItemName, (char*)lpItemInfo2->ItemName, 32);

	code += CompareStateCode((char*)&lpItemInfo1->Weight, (char*)&lpItemInfo2->Weight, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Price, (char*)&lpItemInfo2->Price, sizeof(int));

	code += CompareStateCode((char*)lpItemInfo1->Resistance, (char*)lpItemInfo2->Resistance, sizeof(short) * 8);
	code += CompareStateCode((char*)lpItemInfo1->Damage, (char*)lpItemInfo2->Damage, sizeof(short) * 2);

	code += CompareStateCode((char*)&lpItemInfo1->fAbsorb, (char*)&lpItemInfo2->fAbsorb, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->Defence, (char*)&lpItemInfo2->Defence, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->fBlock_Rating, (char*)&lpItemInfo2->fBlock_Rating, sizeof(float));

	code += CompareStateCode((char*)&lpItemInfo1->Shooting_Range, (char*)&lpItemInfo2->Shooting_Range, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Attack_Speed, (char*)&lpItemInfo2->Attack_Speed, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->Attack_Rating, (char*)&lpItemInfo2->Attack_Rating, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Critical_Hit, (char*)&lpItemInfo2->Critical_Hit, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->fSpeed, (char*)&lpItemInfo2->fSpeed, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->Potion_Space, (char*)&lpItemInfo2->Potion_Space, sizeof(int));

	code += CompareStateCode((char*)&lpItemInfo1->fMagic_Mastery, (char*)&lpItemInfo2->fMagic_Mastery, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fMana_Regen, (char*)&lpItemInfo2->fMana_Regen, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fLife_Regen, (char*)&lpItemInfo2->fLife_Regen, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fStamina_Regen, (char*)&lpItemInfo2->fStamina_Regen, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fIncrease_Life, (char*)&lpItemInfo2->fIncrease_Life, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fIncrease_Mana, (char*)&lpItemInfo2->fIncrease_Mana, sizeof(float));
	code += CompareStateCode((char*)&lpItemInfo1->fIncrease_Stamina, (char*)&lpItemInfo2->fIncrease_Stamina, sizeof(float));

	code += CompareStateCode((char*)&lpItemInfo1->Level, (char*)&lpItemInfo2->Level, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Strength, (char*)&lpItemInfo2->Strength, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Spirit, (char*)&lpItemInfo2->Spirit, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Talent, (char*)&lpItemInfo2->Talent, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Dexterity, (char*)&lpItemInfo2->Dexterity, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->Health, (char*)&lpItemInfo2->Health, sizeof(int));

	code += CompareStateCode((char*)lpItemInfo1->Mana, (char*)lpItemInfo2->Mana, sizeof(short) * 2);
	code += CompareStateCode((char*)lpItemInfo1->Life, (char*)lpItemInfo2->Life, sizeof(short) * 2);
	code += CompareStateCode((char*)lpItemInfo1->Stamina, (char*)lpItemInfo2->Stamina, sizeof(short) * 2);

	code += CompareStateCode((char*)&lpItemInfo1->Money, (char*)&lpItemInfo2->Money, sizeof(int));
	code += CompareStateCode((char*)&lpItemInfo1->JobCodeMask, (char*)&lpItemInfo2->JobCodeMask, sizeof(DWORD));
	code += CompareStateCode((char*)&lpItemInfo1->JobItem, (char*)&lpItemInfo2->JobItem, sizeof(sITEM_SPECIAL));

	code += CompareStateCode((char*)&lpItemInfo1->UniqueItem, (char*)&lpItemInfo2->UniqueItem, sizeof(DWORD));
	code += CompareStateCode((char*)lpItemInfo1->EffectBlink, (char*)lpItemInfo2->EffectBlink, sizeof(short) * 2);
	code += CompareStateCode((char*)lpItemInfo1->EffectColor, (char*)lpItemInfo2->EffectColor, sizeof(short) * 4);
	code += CompareStateCode((char*)lpItemInfo1->ScaleBlink, (char*)lpItemInfo2->ScaleBlink, sizeof(short) * 2);
	code += CompareStateCode((char*)&lpItemInfo1->DispEffect, (char*)&lpItemInfo2->DispEffect, sizeof(DWORD));

	return code;
}



//¾ÆÀÌÅÛ ±³È¯Ã¢ ³»¿ë ÀÎÁõ
static DWORD GetRefromTradeCode(sTRADE* lpTrade, int Key) {

	int cnt;

	iFormCode = 0;
	iFromCnt = Key;

	for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
		if (lpTrade->TradeItem[cnt].Flag) {
			ReformStateCode((char*)&lpTrade->TradeItem[cnt].sItemInfo.CODE, sizeof(DWORD));
			ReformStateCode((char*)&lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum, sizeof(DWORD));
		}
	}
	ReformStateCode((char*)&lpTrade->Money, sizeof(int));

	return iFormCode;
}


static DWORD GetRefromCharState(smCHAR_INFO* lpCharInfo, int Key) {

	iFormCode = 0;
	iFromCnt = Key;

	/*
	int   Level;			//·¹º§
	int   Strength;			//Èû
	int   Spirit;			//Á¤½Å·Â
	int   Talent;			//Àç´É
	int   Dexterity;		//¹ÎÃ¸¼º
	int   Health;			//°Ç°­

	short	Resistance[8];			//¿ø¼Ò¿¡´ëÇÑ ÀúÇ×·Â
	short	Attack_Resistance[8];	//¿ø¼Ò¿¡´ëÇÑ °ø°Ý·Â
	short   Life[2];			//0 ÇöÀç 1 ÃÖ´ëÄ¡
	short   Mana[2];			//0 ÇöÀç 1 ÃÖ´ëÄ¡
	short   Stamina[2];			//0 ÇöÀç 1 ÃÖ´ëÄ¡
	int		Exp;
	int		Next_Exp;
	int		Money;				//º¸À¯ÇÑ µ·
	int		StatePoint;			//´É·ÂÄ¡ Æ÷ÀÎÆ®
	*/

	ReformStateCode((char*)&lpCharInfo->szModelName2, 64);

	ReformStateCode((char*)&lpCharInfo->Level, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Strength, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Spirit, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Talent, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Dexterity, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Health, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Level, sizeof(int));


	ReformStateCode((char*)&lpCharInfo->Accuracy, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Attack_Rating, sizeof(int));
	ReformStateCode((char*)lpCharInfo->Attack_Damage, sizeof(int) * 2);
	ReformStateCode((char*)&lpCharInfo->Attack_Speed, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Shooting_Range, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Critical_Hit, sizeof(int));

	ReformStateCode((char*)&lpCharInfo->Defence, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Chance_Block, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Absorption, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Move_Speed, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Sight, sizeof(int));
	ReformStateCode((char*)lpCharInfo->Weight, sizeof(short) * 2);

	ReformStateCode((char*)lpCharInfo->Resistance, sizeof(short) * 8);
	ReformStateCode((char*)lpCharInfo->Attack_Resistance, sizeof(short) * 8);

	ReformStateCode((char*)lpCharInfo->Life, sizeof(short) * 2);
	ReformStateCode((char*)lpCharInfo->Mana, sizeof(short) * 2);
	ReformStateCode((char*)lpCharInfo->Stamina, sizeof(short) * 2);

	ReformStateCode((char*)&lpCharInfo->Exp, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Next_Exp, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->Money, sizeof(int));
	ReformStateCode((char*)&lpCharInfo->StatePoint, sizeof(int));

#ifdef _MODE_EXP64
	ReformStateCode((char*)&lpCharInfo->Exp_High, sizeof(int));
#endif

	return iFormCode;
}


static int ItemFormVersion = 12;
static int ItemFormKey = 0;

//¾ÆÀÌÅÛ ÀÎÁõ ¹Þ±â
int ReformItem(sITEMINFO* lpItemInfo)
{

	//½Å±Ô ÀÎÁõ
	DWORD dwCode;

#ifdef _W_SERVER
	// ¼­¹öÃø ÀÎÁõ 
	extern time_t		tServerTime;

	lpItemInfo->ItemHeader.Head = ItemFormKey + GetCurrentTime();

	dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);

	lpItemInfo->ItemHeader.dwChkSum = dwCode;
	lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	lpItemInfo->dwCreateTime = (DWORD)tServerTime;

	if (!lpItemInfo->BackUpKey || !lpItemInfo->BackUpChkSum) {
		lpItemInfo->BackUpKey = lpItemInfo->ItemHeader.Head;
		lpItemInfo->BackUpChkSum = lpItemInfo->ItemHeader.dwChkSum;
	}

	ReformMixItem(lpItemInfo);			//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Þ±â
	rsReformItem_Server(lpItemInfo);		//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ¸¸ ¹Þ±â

	lpItemInfo->Temp0 = lpItemInfo->ItemHeader.dwChkSum + lpItemInfo->CODE;

#else
	// Å¬¶óÀÌ¾ðÆ®Ãø ÀÎÁõ ( ºÎºÐÀÎÁõ )
	if (!lpItemInfo->CODE || lpItemInfo->CODE == (sinGG1 | sin01) || (lpItemInfo->CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
		lpItemInfo->ItemHeader.Head = ItemFormKey + GetCurrentTime();

		dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);

		lpItemInfo->ItemHeader.dwChkSum = dwCode;
		lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	}
#endif

	ItemFormKey++;

	return TRUE;
}

//¾ÆÀÌÅÛ ÀÎÁõ ¹Þ±â ( »ý¼º½Ã°£ º¸Á¤ )
int ReformItem_AdjustTime(sITEMINFO* lpItemInfo, int TimeAdjust)
{

#ifdef _W_SERVER

	//½Å±Ô ÀÎÁõ
	DWORD dwCode;

	// ¼­¹öÃø ÀÎÁõ 
	extern time_t		tServerTime;

	lpItemInfo->ItemHeader.Head = ItemFormKey + GetCurrentTime();

	dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);

	lpItemInfo->ItemHeader.dwChkSum = dwCode;
	lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	lpItemInfo->dwCreateTime = (DWORD)tServerTime + TimeAdjust;

	if (!lpItemInfo->BackUpKey || !lpItemInfo->BackUpChkSum) {
		lpItemInfo->BackUpKey = lpItemInfo->ItemHeader.Head;
		lpItemInfo->BackUpChkSum = lpItemInfo->ItemHeader.dwChkSum;
	}

	ReformMixItem(lpItemInfo);			//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Þ±â
	rsReformItem_Server(lpItemInfo);		//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ¸¸ ¹Þ±â

	lpItemInfo->Temp0 = lpItemInfo->ItemHeader.dwChkSum + lpItemInfo->CODE;

#endif

	ItemFormKey++;

	return TRUE;
}



#ifdef _W_SERVER
//¾ÆÀÌÅÛ ÀÎÁõ È®ÀÎ
int CheckItemForm(sITEMINFO* lpItemInfo)
{
	DWORD dwCode;
	//DWORD dwChkCode;
/*
	if ( !lpWareHouse->WareHouseItem[cnt].ItemHeader.dwTime ) {
		//ÄÚµå À§Àå
		sRecordItem.sItemInfo.ItemHeader.dwTime =
			(sRecordItem.sItemInfo.ItemHeader.Head>>2)^(sRecordItem.sItemInfo.ItemHeader.dwChkSum<<2);
	}
*/

//µ·¶Ç´Â ¹°¾àÀÏ °æ¿ì ÀÏ¹Ý ÀÎÁõ Ã½Å© »ç¿ë
	if (lpItemInfo->CODE == (sinGG1 | sin01) || (lpItemInfo->CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
		dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);
		if (lpItemInfo->ItemHeader.dwChkSum == dwCode) return TRUE;
		return FALSE;
	}
	/*
		dwChkCode = (lpItemInfo->ItemHeader.Head>>2)^(lpItemInfo->ItemHeader.dwChkSum<<2);
		if ( dwChkCode==lpItemInfo->ItemHeader.dwTime ) {
			//À§ÀåÄÚµåÀÏ¶§ »õ·Î ÀÎÁõ ¹Þ±â
			dwCode = GetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.Head );
			if ( lpItemInfo->ItemHeader.dwChkSum!=dwCode ) return FALSE;

			//¼­¹ö¿ë Àç ÀÎÁõ ¾ÆÀÌÅÛ
			dwCode = rsGetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.dwChkSum );
			dwCode = dwCode ^ ((lpItemInfo->ItemAgingProtect[0]*7)^(lpItemInfo->ItemAgingProtect[1]*9));	//Ãß°¡ (¹Í½º/¿¡ÀÌÂ¡) ÇÁ·ÎÅØÆ®
			dwCode+= lpItemInfo->dwCreateTime>>8;
			dwCode = dwCode ^ lpItemInfo->ItemHeader.Head;


			lpItemInfo->ItemHeader.dwTime = dwCode;

			return 0x100;
		}
	*/

	//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ°ª ¾ò±â
	dwCode = rsGetItemForm_Server(lpItemInfo);

	if (lpItemInfo->ItemHeader.dwTime == dwCode) return TRUE;

	return FALSE;
}

#else
//¾ÆÀÌÅÛ ÀÎÁõ È®ÀÎ
int CheckItemForm(sITEMINFO* lpItemInfo)
{
	DWORD dwCode;
	/*
		if ( lpItemInfo->ItemHeader.dwVersion==0 ) {
			//±¸¹öÀü ÀÎÁõ È®ÀÎ ÈÄ »õ¹öÀüÀ¸·Î Àç ÀÎÁõ
			dwCode = GetRefromItemCode_Old( lpItemInfo , lpItemInfo->ItemHeader.Head );
			if ( lpItemInfo->ItemHeader.dwChkSum==dwCode ) {
				//±¸¹öÀü Á¶ÀÛ ¾ÆÀÌÅÛ Á¦°Å
				if ( lpItemInfo->fAbsorb>=20 || lpItemInfo->Defence>=200 || lpItemInfo->Damage[1]>=40 ) {
					return FALSE;
				}
				//±¸¹öÀü ÀÎÁõ È®ÀÎ / »õ ÀÎÁõ ¹ß±Þ
				ReformItem( lpItemInfo );
				return TRUE;
			}
			return FALSE;
		}

		if ( lpItemInfo->ItemHeader.dwVersion==10 ) {
			//±¸¹öÀü ÀÎÁõ È®ÀÎ ÈÄ »õ¹öÀüÀ¸·Î Àç ÀÎÁõ
			dwCode = GetRefromItemCode_V10( lpItemInfo , lpItemInfo->ItemHeader.Head );
			if ( lpItemInfo->ItemHeader.dwChkSum==dwCode ) {
				//±¸¹öÀü Á¶ÀÛ ¾ÆÀÌÅÛ Á¦°Å
				if ( lpItemInfo->fAbsorb>=20 || lpItemInfo->Defence>=200 || lpItemInfo->Damage[1]>40 ) {
					return FALSE;
				}
				//±¸¹öÀü ÀÎÁõ È®ÀÎ / »õ ÀÎÁõ ¹ß±Þ
				ReformItem( lpItemInfo );
				return TRUE;
			}
			return FALSE;
		}
	*/
	dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);


	if (lpItemInfo->ItemHeader.dwChkSum == dwCode) return TRUE;

	return FALSE;
}
#endif


int CompareItems(sITEMINFO* lpItemInfo1, sITEMINFO* lpItemInfo2)
{
	if (lpItemInfo1->ItemHeader.dwVersion != (DWORD)ItemFormVersion) CheckItemForm(lpItemInfo1);
	if (lpItemInfo2->ItemHeader.dwVersion != (DWORD)ItemFormVersion) CheckItemForm(lpItemInfo2);

	if (lpItemInfo1->CODE != lpItemInfo2->CODE) return FALSE;
	if (lpItemInfo1->ItemHeader.Head != lpItemInfo2->ItemHeader.Head) return FALSE;
	if (lpItemInfo1->ItemHeader.dwChkSum != lpItemInfo2->ItemHeader.dwChkSum) return FALSE;

	if (CompareReformItemCode(lpItemInfo1, lpItemInfo2) == 0)
		return TRUE;

	return FALSE;
}


int ReformCharForm(smCHAR_INFO* lpCharInfo)
{
	DWORD dwCode;

	dwCode = GetRefromCharState(lpCharInfo, 10);

	lpCharInfo->RefomCode = dwCode;

	ItemFormKey++;

	return TRUE;
}

//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckCharForm(smCHAR_INFO* lpCharInfo)
{
	DWORD dwCode;

	dwCode = GetRefromCharState(lpCharInfo, 10);

	if (lpCharInfo->RefomCode == dwCode) return TRUE;

	return FALSE;
}


//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
int ReformCharForm()
{
	return ReformCharForm(&lpCurPlayer->smCharInfo);
}

int ReformCharFormCostume(sITEM* sItem)
{
	lpCurPlayer->smCharInfo.Defence += sItem->sItemInfo.Defence;
	return ReformCharForm(&lpCurPlayer->smCharInfo);
}

#ifndef _GU
#define _GU

#include "AntiCheat.h"

#endif

extern CAntiCheat* pcAntiCheat;

//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckCharForm()
{
	int val;

	val = CheckCharForm(&lpCurPlayer->smCharInfo);

	pcAntiCheat->CheckThread();

	if (val == FALSE) {//|| ( VRKeyBuff['0'] && DisconnectFlag==0 )) {

		//ÇØÅ· ½Ãµµ ¾Ë¸²
		SendSetHackUser(81);

		DisconnectServerCode = 4;
		DisconnectFlag = GetCurrentTime();

	}

	return val;
}

//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ ¹Þ±â
int ReformTrade(void* lpTrade)
{
	DWORD dwCode;

	dwCode = GetRefromTradeCode((sTRADE*)lpTrade, 84);

	((sTRADE*)lpTrade)->CheckMoney = dwCode;

	ItemFormKey++;

	return TRUE;
}

//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckTrade(void* lpTrade)
{
	DWORD dwCode;

	dwCode = GetRefromTradeCode((sTRADE*)lpTrade, 84);

	if (((sTRADE*)lpTrade)->CheckMoney == (int)dwCode) return TRUE;

	//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í ( Æ®·¹ÀÌµå ÀÎÁõ ¿À·ù ¼­¹ö¿¡ ¾Ë¸² )
	SendSetHackUser2(4000, 0);

	return FALSE;
}

/*
	short   ItemAgingNum[2];	//0Àº ¿¡ÀÌÂ¡+¸î 1Àº ¿¡ÀÌÂ¡ÀÌ µÇ°íÀÖ³ª ¾Æ´Ñ°¡?
	short   ItemAgingCount[2];	//0¾ÆÀÌÅÛ ¼÷·Ã Ä«¿îÆ® 1¼÷·ÃÄ¡ ÃÖ´ë
	short	ItemAgingProtect[2];

//ItemAgingProtect
*/

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõÄÚµå
short GetMixItemForm(sITEMINFO* lpItemInfo)
{
	short sCode = 0;

#ifdef _W_SERVER

	DWORD dwCode;
	DWORD dwItemKey;

	if (lpItemInfo->ItemKindCode) {
		dwItemKey = lpItemInfo->ItemHeader.dwChkSum;
		/*
				dwCode = lpItemInfo->ItemKindCode*(
					lpItemInfo->ItemAgingNum[0]*3+lpItemInfo->ItemAgingNum[1]*7+
					lpItemInfo->ItemAgingCount[0]*15+lpItemInfo->ItemAgingCount[1]+17 );
		*/

		//¿¡ÀÌÂ¡ Ä«¿îÅÍ(ItemAgingCount) [1]Àº ¼­¹öÃø º¸¾È , [0]Àº Å¬¶óÀÌ¾ðÆ® º¸¾ÈÀ¸·Î º¯°æ
		dwCode = lpItemInfo->ItemKindCode * (
			lpItemInfo->ItemAgingNum[0] * 3 + lpItemInfo->ItemAgingNum[1] * 7 + lpItemInfo->ItemAgingCount[1] * 17 + 17);

		dwCode = dwCode ^ dwItemKey;

		sCode = (short)((dwCode >> 16) ^ (dwCode & 0xFFFF));
	}
#endif

	return sCode;
}

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Þ±â
int ReformMixItem(sITEMINFO* lpItemInfo)
{
#ifdef _W_SERVER
	lpItemInfo->ItemAgingProtect[0] = GetMixItemForm(lpItemInfo);
#endif
	return TRUE;
}

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ È®ÀÎ
int CheckMixItem(sITEMINFO* lpItemInfo)
{

#ifdef _W_SERVER
	if (GetMixItemForm(lpItemInfo) == lpItemInfo->ItemAgingProtect[0]) {
		return TRUE;
	}
#else
	return TRUE;
#endif

	return FALSE;
}

//Èí¼ö·Â º¸Á¤ÇÏ¿© ±¸ÇÏ±â
float GetItemAbsorb(sITEMINFO* lpItemInfo)
{
	float	fAborb;
	DWORD	dwCode;


	dwCode = lpItemInfo->CODE & sinITEM_MASK2;
	fAborb = lpItemInfo->fAbsorb;

	return	fAborb;
}


#define WAIT_BATTLE_QUIT_TIME		10000

extern void WriteIniStr(char* Section, char* Key, char* szFileIni, char* Value);

//°ÔÀÓ Á¾·á
int QuitGame()
{
	if (Settings::GetInstance()->bRememberLogin)
		WriteIniStr("Game", "LastAccount", "game.ini", UserAccount);
	else
		WriteIniStr("Game", "LastAccount", "game.ini", "");

	if (quit) return TRUE;

	if (dwBattleTime) {
		if ((dwBattleTime + WAIT_BATTLE_QUIT_TIME) > dwPlayTime) {
			dwBattleQuitTime = dwPlayTime;
			return FALSE;
		}
	}

	if (cWareHouse.OpenFlag || cCraftItem.OpenFlag || cTrade.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag || Caravana::GetInstance()->OpenFlag)
	{
		dwCloseBoxTime = dwPlayTime + 2000;
		return FALSE;
	}

	/*
	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = 0x48476666;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = 0;
	smTransCommand.SParam = 0;
	smTransCommand.LParam = 0;
	smTransCommand.EParam = 0;

	if ( smWsockDataServer )
		smWsockDataServer->Send( (char *)&smTransCommand , smTransCommand.size , TRUE ); */

	quit = 1;

	return TRUE;
}

#define CHARINFO_CACHE_MAX		200

smTRNAS_PLAYERINFO	CharInfo_Cache[CHARINFO_CACHE_MAX];
DWORD	dwCharInfo_CacheCode[CHARINFO_CACHE_MAX];
int	CharInfoCacheCnt = 0;

//Ä³¸¯ÅÍ Á¤º¸¸¦ Ä³½Ãµ¥ÀÌÅ¸·Î ºÎÅÍ ¾ò´Â´Ù
smTRNAS_PLAYERINFO* GetCharInfoCache(char* lpData)
{
	DWORD	dwChkCode;
	int cnt;
	smPLAYDATA* lpPlayData;
	smTRANS_PLAYDATA* lpTransPlayData;
	smTRANS_PLAYBUFF* lpTransPlayBuff;
	int	RecvCode;
	smTRNAS_PLAYERINFO* lpTransPlayInfo;

	RecvCode = ((int*)lpData)[1];

	dwChkCode = 0;

	switch (RecvCode) {
	case smTRANSCODE_PLAYDATA1:
		lpPlayData = (smPLAYDATA*)lpData;
		dwChkCode = lpPlayData->dwAutoCharCode;
		break;
	case smTRANSCODE_TRANSPLAYDATA:
		lpTransPlayData = (smTRANS_PLAYDATA*)lpData;
		lpTransPlayBuff = (smTRANS_PLAYBUFF*)(lpData + sizeof(smTRANS_PLAYDATA));
		if (lpTransPlayData->PlayBuffCnt > 0)
			dwChkCode = lpTransPlayData->dwAutoCharCode;
		break;
	}

	if (!dwChkCode) return NULL;

	for (cnt = 0; cnt < CharInfoCacheCnt; cnt++) {
		if (dwChkCode == dwCharInfo_CacheCode[cnt]) {
			//Ä³½Ã µ¥ÀÌÅ¸ Ã£À½
			lpTransPlayInfo = &CharInfo_Cache[cnt];

			switch (RecvCode) {
			case smTRANSCODE_PLAYDATA1:
				//°ÔÀÓ ÁøÇà ¼ö½Å ( ÀúÁú µ¥ÀÌÅ¸ )
				lpTransPlayInfo->x = lpPlayData->x;
				lpTransPlayInfo->y = lpPlayData->y;
				lpTransPlayInfo->z = lpPlayData->z;
				lpTransPlayInfo->ax = lpPlayData->x;
				lpTransPlayInfo->ay = lpPlayData->y;
				lpTransPlayInfo->az = lpPlayData->z;
				lpTransPlayInfo->dwObjectSerial = lpPlayData->dwObjectSerial;
				lpTransPlayInfo->smCharInfo.dwObjectSerial = lpPlayData->dwObjectSerial;
				break;

			case smTRANSCODE_TRANSPLAYDATA:
				//°ÔÀÓ ÁøÇà ¼ö½Å ( Á¤¹Ð µ¥ÀÌÅ¸ )
				lpTransPlayBuff = (smTRANS_PLAYBUFF*)(((char*)lpTransPlayData) + sizeof(smTRANS_PLAYDATA));
				lpTransPlayBuff += lpTransPlayData->PlayBuffCnt - 1;

				lpTransPlayInfo->x = lpTransPlayBuff->PlayBuff.x;
				lpTransPlayInfo->y = lpTransPlayBuff->PlayBuff.y;
				lpTransPlayInfo->z = lpTransPlayBuff->PlayBuff.z;

				lpTransPlayInfo->ax = lpTransPlayBuff->PlayBuff.angle[0];
				lpTransPlayInfo->ay = lpTransPlayBuff->PlayBuff.angle[1];
				lpTransPlayInfo->az = lpTransPlayBuff->PlayBuff.angle[2];
				lpTransPlayInfo->dwObjectSerial = lpTransPlayData->dwObjectSerial;
				lpTransPlayInfo->smCharInfo.dwObjectSerial = lpTransPlayData->dwObjectSerial;
				break;
			}

			lpTransPlayInfo->smCharInfo.Life[0] = lpTransPlayInfo->smCharInfo.Life[1];
			return lpTransPlayInfo;
		}
	}

	return NULL;
}

//Ä³¸¯ÅÍ Á¤º¸¸¦ Ä³½Ãµ¥ÀÌÅ¸¿¡ ÀúÀå
int SaveCharInfoCache(smTRNAS_PLAYERINFO* lpTransPlayInfo)
{
	int cnt;
	DWORD	dwCode;

	dwCode = GetSpeedSum(lpTransPlayInfo->smCharInfo.szName);

	//±âÁ¸ µ¥ÀÌÅ¸ ºñ±³
	for (cnt = 0; cnt < CharInfoCacheCnt; cnt++) {
		if (dwCode == dwCharInfo_CacheCode[cnt]) return NULL;
	}

	//»õ·Î ÀúÀå
	dwCharInfo_CacheCode[CharInfoCacheCnt] = dwCode;
	memcpy(&CharInfo_Cache[CharInfoCacheCnt], lpTransPlayInfo, sizeof(smTRNAS_PLAYERINFO));
	CharInfoCacheCnt++;

	return TRUE;
}


smCHAR	chrPartyPlayer;

//ÆÄÆ¼ ÇÃ·¹ÀÌ¾î ¾ó±¼ ¿ÀÇÂ
int OpenPartyPlayer(char* szName, char* szModelFile1, char* szModelFile2)
{
	//¾ÆÀÌÅÛÀ» »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
	AddLoaderPattern(&chrPartyPlayer, szModelFile1, szModelFile2);

	chrPartyPlayer.SetMotionFromCode(CHRMOTION_STATE_STAND);
	chrPartyPlayer.ActionPattern = 101;
	chrPartyPlayer.Flag = TRUE;
	chrPartyPlayer.lpStage = 0;
	chrPartyPlayer.OnStageField = lpCurPlayer->OnStageField;

	lstrcpy(chrPartyPlayer.smCharInfo.szName, szName);

	OpenEachPlayer(&chrPartyPlayer);

	return TRUE;
}

//extern HIMC hImc;

char	szLastWhisperName[32] = { 0 };
int		LastWhisperLen = 0;
bool enterPartyMsg = false;

// Quando da 2 click no membro da party
int	WhisperPartyPlayer(char* szName)
{

	CHATBOX->OnEnableChatBox();
	CHATBOX->SetInputFieldText(FormatString("/:%s ", szName));

	return TRUE;
}

int	SetClanChatting()
{
	char szBuff[128];
	int len;

	szLastWhisperName[0] = 0;
	hFocusWnd = hTextWnd;
	cInterFace.ChatFlag = TRUE;
	wsprintf(szBuff, "/CLAN> ");
	len = lstrlen(szBuff);
	LastWhisperLen = len;
	SetWindowText(hFocusWnd, szBuff);
	SendMessage(hFocusWnd, EM_SETSEL, len, len);

	SetIME_Mode(TRUE);

	return TRUE;
}

//Å¬·£ Ã¤ÆÃ ¸ðµå ¼³Á¤
int IsClanChatting()
{
	if (lpCurPlayer->smCharInfo.ClassClan) {
		if (!chatlistSPEAKERflag()) {
			chatlistSPEAKERflagChg(TRUE);		//Å¬·£ Ã¤ÆÃ ¹öÆ° ÄÑ±â
			SetClanChatting();
			return TRUE;
		}
		else {
			chatlistSPEAKERflagChg(FALSE);		//Å¬·£ Ã¤ÆÃ ¹öÆ° ÄÑ±â
			SetWindowText(hFocusWnd, "");
			return TRUE;
		}
		szLastWhisperName[0] = 0;
	}

	return FALSE;
}


int	PlayAttackCount = 0;			//°ø°Ý ¹ÞÀº Ä«¿îÅÍ
int	MotionAttackCount = 0;			//³ª¸¦ °ø°ÝÇÑ ¸ð¼Çµé Ä«¿îÅÍ


//°ø°Ý ¹ÞÀº ÆÐÅ¶ Ä«¿îÅÍ
int	AttackRecvCountUp()
{
	PlayAttackCount++;
	return PlayAttackCount;
}

//°ø°Ý ¹ÞÀº ¸ð¼Ç Ä«¿îÅÍ
int AttackMotionCountUp()
{
	MotionAttackCount++;
	if (MotionAttackCount > 30) {
		//°ø°Ý ºñÀ² Ã½Å©
		CheckPlayAttackCount();
	}

	return MotionAttackCount;
}

//°ø°Ý ºñÀ² Ã½Å©
int	CheckPlayAttackCount()
{
	//30¹ø °ø°Ý ¹Þ´Â µ¿¾È °ø°Ý ÆÐÅ¶ÀÌ 3 ÀÌÇÏÀÏ °æ¿ì ¹®Á¦ÀÖ´Ù°í ÆÇ´ÜÇÔ
	if (MotionAttackCount > 30 && PlayAttackCount <= 3) {
		//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
		//SendSetHackUser2( 1500 , MotionAttackCount-PlayAttackCount );
	}

	MotionAttackCount = 0;
	PlayAttackCount = 0;

	return TRUE;
}

struct	sCOPY_ITEM_INFO {
	DWORD	dwItemCode;
	int		ChkSum;
	int		Header;
};

sCOPY_ITEM_INFO	sCopyItemList[] = {
	/*
	#include "SrcCopyItems\\°øº´ºÎÄÚµå.h"
	#include "SrcCopyItems\\¸¶Ãµ»çÄÚµå.h"
	#include "SrcCopyItems\\Dragon_½Å°Ë ÄÚµå.h"

		//Å¬·¹ÀÌ¸Ó [ÆÄÀÌÅÍÆ¯ 13-24,46¸íÁß [¹Í½º] - ±×¸®ºê ¾ÆÀÌÅÛÁß - º¹»çº» ( ´çÁø°æÂû¼­ )
		{	sinWS2|sin11,	481819,		1127932		},
	*/
		{	0,0,0	}

};

//µî·ÏµÈ ºÒ·® ¾ÆÀÌÅÛ °Ë»ç
int	CheckCopyItem(sITEMINFO* lpItem)
{
	int cnt;

	cnt = 0;

	if (!lpItem->ItemName[0])	return FALSE;			//²®µ¥±â ¾ÆÀÌÅÛ (¾ç¼Õ¹«±â)
	if (lpItem->SpecialItemFlag[0])	return FALSE;		//ÀÌ¹Ì µî·ÏµÈ °ªÀÌ ÀÖÀ½

	if (!lpItem->BackUpKey || !lpItem->BackUpChkSum) {
		lpItem->BackUpKey = lpItem->ItemHeader.Head;
		lpItem->BackUpChkSum = lpItem->ItemHeader.dwChkSum;
	}

	while (1) {
		if (sCopyItemList[cnt].dwItemCode == 0) break;


		if (sCopyItemList[cnt].dwItemCode == lpItem->CODE) {
			if (sCopyItemList[cnt].ChkSum == (int)lpItem->ItemHeader.dwChkSum &&
				sCopyItemList[cnt].Header == (int)lpItem->ItemHeader.Head) {

				lpItem->SpecialItemFlag[0] = CHECK_COPY_ITEM;
				return TRUE;
			}
			if (sCopyItemList[cnt].ChkSum == (int)lpItem->BackUpChkSum &&
				sCopyItemList[cnt].Header == (int)lpItem->BackUpKey) {

				lpItem->SpecialItemFlag[0] = CHECK_COPY_ITEM;
				return TRUE;
			}
		}
		cnt++;
	}


	return FALSE;
}


//¾ó±¼ º¯°æÇÑ°Å Àû¿ë
int SetJobFace(char* szModelName)
{

	lstrcpy(lpCurPlayer->smCharInfo.szModelName2, szModelName);
	AddLoaderPattern(lpCurPlayer, lpCurPlayer->smCharInfo.szModelName, lpCurPlayer->smCharInfo.szModelName2);
	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();	//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â

	//¼­¹ö¿¡ º¯°æµ¥ÀÌÅ¸ ¼ÛºÎ
	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);

	return TRUE;
}


//char *szTempPikeFaceName[2][3] = {
/*
char **szFaceName[5] = {
	szTempFighterFaceName,
	szTempFighterFaceName,
	szTempMechFaceName,
	szTempArcherFaceName,
	szTempPikeFaceName
};
*/

#define	_CHANGE_JOB_COUNT_MAX		FACE_MODEL_MAX
#define	_FACE_COUNT_MAX				HEAREPOTION_MAX

//¸ðµ¨ÀÌ ¸Â´ÂÁö È®ÀÎ
int	CheckUserModel(smCHAR_INFO* lpCharInfo)
{
	int cnt, cnt2, len;
	char	szBuff[64];
	char* lpBuff;


	lpCharInfo->szModelName[31] = 0;
	lpCharInfo->szModelName2[31] = 0;

	len = lstrlen(lpCharInfo->szModelName);
	lstrcpy(szBuff, lpCharInfo->szModelName);
	for (cnt = len - 1; cnt >= 0; cnt--) {
		if (szBuff[cnt] == '\\') {
			szBuff[cnt + 1] = 0;
			break;
		}
	}

	if (lstrcmpi(szBuff, "char\\tmABCD\\") != 0) return FALSE;
	lpBuff = &lpCharInfo->szModelName[len - 3];
	if (lstrcmpi(lpBuff, "ini") != 0) return FALSE;

	//Á÷¾÷º° ¾ó±¼ È®ÀÎ
	switch (lpCharInfo->JOB_CODE) {
	case 1:
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_FighterFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_FighterFaceName[0][0]);
			return TRUE;
		}
		break;
	case 2:
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_MechFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_MechFaceName[0][0]);
			return TRUE;
		}
		break;
	case 3:
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_ArcherFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_ArcherFaceName[0][0]);
			return TRUE;
		}

		break;
	case 4:
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_PikeFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_PikeFaceName[0][0]);
			return TRUE;
		}
		break;

	case 0:
		return TRUE;
	case 5:	//¾ÆÅ»¶õÅ¸
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_AtalantaFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_AtalantaFaceName[0][0]);
			return TRUE;
		}
		break;
	case 6:	//³ªÀÌÆ®
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_KnightFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_KnightFaceName[0][0]);
			return TRUE;
		}
		break;
	case 7:	//¸ÞÁö¼Ç
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_MagicanFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_MagicanFaceName[0][0]);
			return TRUE;
		}
		break;
	case 8:	//ÇÁ¸®½ºÆ¼½º
		for (cnt = 0; cnt < _CHANGE_JOB_COUNT_MAX; cnt++) {
			for (cnt2 = 0; cnt2 < _FACE_COUNT_MAX; cnt2++) {
				if (lstrcmpi(szModel_PristessFaceName[cnt][cnt2], lpCharInfo->szModelName2) == 0) {
					return TRUE;
				}
			}
		}
		if (!lpCharInfo->szModelName2[0]) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy(lpCharInfo->szModelName2, szModel_PristessFaceName[0][0]);
			return TRUE;
		}
		break;
	}

	return FALSE;
}


//Áý¾÷ º¯°æ ¾ó±¼ º¯È­
int ChangeJobFace()
{
	int cnt1;
	//int cnt2;

	cnt1 = 0;

	StartSkill(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, SKILL_UP1);
	SkillPlaySound(SKILL_SOUND_LEARN, lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ);			//½ºÅ³ È¿°úÀ½

	CheckCharForm();
	/*
		for( cnt2=0;cnt2<3;cnt2++) {
			if ( lstrcmpi(	lpCurPlayer->smCharInfo.szModelName2 , szTempPikeFaceName[cnt1][cnt2] )==0 ) {
				cnt1++;
				SetJobFace( szTempPikeFaceName[cnt1][cnt2] );
				return TRUE;
			}
			if ( lstrcmpi(	lpCurPlayer->smCharInfo.szModelName2 , szTempArcherFaceName[cnt1][cnt2] )==0 ) {
				cnt1++;
				SetJobFace( szTempArcherFaceName[cnt1][cnt2] );
				return TRUE;
			}
			if ( lstrcmpi(	lpCurPlayer->smCharInfo.szModelName2 , szTempMechFaceName[cnt1][cnt2] )==0 ) {
				cnt1++;
				SetJobFace( szTempMechFaceName[cnt1][cnt2] );
				return TRUE;
			}
			if ( lstrcmpi(	lpCurPlayer->smCharInfo.szModelName2 , szTempFighterFaceName[cnt1][cnt2] )==0 ) {
				cnt1++;
				SetJobFace( szTempFighterFaceName[cnt1][cnt2] );
				return TRUE;
			}
		}
	*/

	ChangeModelSkin(lpCurPlayer, 0, lpCurPlayer->smCharInfo.ChangeJob, 0, 0);			//Àü¾÷¾ó±¼
	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();
	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);

	return FALSE;
}


////////////////////////////// ¿¡ÆåÆ® ¿¹¾à ½Ã½ºÅÛ ///////////////////////////////

#define	TIME_EFFECT_BUFF_MAX	16

struct sTIME_EFFECT_BUFF {
	DWORD	dwCode;
	DWORD	dwTime;
	int		x, y, z;
	smCHAR* lpChar;
};

sTIME_EFFECT_BUFF TimeEffectBuff[TIME_EFFECT_BUFF_MAX];

//¿¹¾à ÀÌÆåÆ® ÃÊ±âÈ­
int InitTimeEffect()
{
	int cnt;
	for (cnt = 0; cnt < TIME_EFFECT_BUFF_MAX; cnt++) {
		TimeEffectBuff[cnt].dwCode = 0;
	}

	return TRUE;
}

//ÀÌÆåÆ® ¿¹¾à Ãß°¡
int AddTimeEffect(DWORD EffectCode, DWORD dwTime, int x, int y, int z, smCHAR* lpChar)
{
	int cnt;
	for (cnt = 0; cnt < TIME_EFFECT_BUFF_MAX; cnt++) {
		if (!TimeEffectBuff[cnt].dwCode) {
			TimeEffectBuff[cnt].dwCode = EffectCode;
			TimeEffectBuff[cnt].dwTime = dwTime;
			TimeEffectBuff[cnt].x = x;
			TimeEffectBuff[cnt].y = y;
			TimeEffectBuff[cnt].z = z;
			TimeEffectBuff[cnt].lpChar = lpChar;
			return TRUE;
		}
	}
	return FALSE;
}

int AddTimeEffect(DWORD EffectCode, DWORD dwTime, int x, int y, int z)
{
	return AddTimeEffect(EffectCode, dwTime, x, y, z, 0);
}

//¿¹¾à ÀÌÆåÆ® ½ÇÇà
int TimeEffectMain()
{
	int cnt;
	for (cnt = 0; cnt < TIME_EFFECT_BUFF_MAX; cnt++) {
		if (TimeEffectBuff[cnt].dwCode && TimeEffectBuff[cnt].dwTime < dwPlayTime) {

			switch (TimeEffectBuff[cnt].dwCode) {
			case SKILL_RAGE_OF_ZECRAM_HIT2:
				//·¹ÀÌÁö ¿Àºê Á¦Å©¶÷ ÀÌÆåÆ®
				StartSkill(TimeEffectBuff[cnt].x, TimeEffectBuff[cnt].y, TimeEffectBuff[cnt].z, 0, 0, 0, SKILL_RAGE_OF_ZECRAM_HIT2);
				SkillPlaySound(SKILL_SOUND_SKILL_RAGE_OF_ZECRAM2, TimeEffectBuff[cnt].x, TimeEffectBuff[cnt].y, TimeEffectBuff[cnt].z);
				SetDynLight(TimeEffectBuff[cnt].x, TimeEffectBuff[cnt].y, TimeEffectBuff[cnt].z, 100, 0, 0, 0, 500);
				break;

			case smTRANSCODE_SUCCESS_EVATION:
				//Ãß°¡È¸ÇÇ Å¬¶óÀÌ¾ðÆ®¿¡ ½Ã°£Â÷ Ãâ·Â
				cInterFace.SetStringEffect(HA_EVASION);
				break;

			case SKILL_EFFECT_FIREFLOWER:
				if (TimeEffectBuff[cnt].lpChar && TimeEffectBuff[cnt].lpChar->Flag) {
					ParkAssaParticle_FireCracker(TimeEffectBuff[cnt].lpChar, TimeEffectBuff[cnt].x);
					esPlaySound(23 + (rand() % 3), GetDistVolume(TimeEffectBuff[cnt].lpChar->pX, TimeEffectBuff[cnt].lpChar->pY, TimeEffectBuff[cnt].lpChar->pZ));
				}
				break;
			}

			TimeEffectBuff[cnt].dwCode = 0;
		}
	}
	return TRUE;
}

#define		ROT_PLAYER_MAX		128
#define		ROT_PLAYER_MIN		80

smCHAR* lpChrPlayers[ROT_PLAYER_MAX];


//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ÃÊ±âÈ­
smCHAR* InitRotPlayer()
{
	int cnt;
	for (cnt = 0; cnt < ROT_PLAYER_MAX; cnt++)
		lpChrPlayers[cnt] = 0;

	return SelectRotPlayer(0);
}

//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ Á¾·á
int	CloseRotPlayer()
{
	int cnt;
	for (cnt = 0; cnt < ROT_PLAYER_MAX; cnt++)
		if (lpChrPlayers[cnt]) {
			delete	lpChrPlayers[cnt];
			lpChrPlayers[cnt] = 0;
		}

	return TRUE;
}

//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ¼±ÅÃ
smCHAR* SelectRotPlayer(smCHAR* lpCurrentChar)
{
	int rnd;
	smCHAR* lpChar;

	rnd = rand() % ROT_PLAYER_MIN;
	lpChar = lpChrPlayers[rnd];
	if (!lpChar) {
		lpChar = new smCHAR;
		lpChrPlayers[rnd] = lpChar;
	}

	if (!lpChar) return NULL;


	if (lpCurrentChar && lpCurrentChar != lpChar) {
		memcpy(lpChar, lpCurrentChar, sizeof(smCHAR));
	}

	if (!lpCurrentChar && lpChar) {
		ZeroMemory(lpChar, sizeof(smCHAR));
	}

	return lpChar;
}


///////////////////////////// ¸Þ¸ð¸® Æã¼Ç 2Â÷ °¨½Ã //////////////////////////////////
/*
DWORD	dwKeepMemStartPoint;
DWORD	dwKeepMemCount;
DWORD	dwKeepMemSrcPoint;
DWORD	dwKeepLen;

int	InitKeepMemFunc()
{
	BYTE	*lpFunc;
	DWORD	cnt;
	DWORD	PointMin,PointMax;
	//DWORD	dwCode,dwCnt;
	DWORD	dwLen;
	DWORD	*lpdwFunc;
	BYTE	JmpCode = 9;
	DWORD	dwHeaderPos = 0x30;
	DWORD	dwCodeSeg;
	DWORD	dwCodeAddr;

	DWORD dwStartFunc = (DWORD)InitKeepMemFunc;

	JmpCode+=dwAsmJmpCode;

	lpFunc = (BYTE *)dwStartFunc;

	dwCodeSeg = dwStartFunc&0xFFF00000;
	dwCodeAddr = dwStartFunc&0x000FFFFF;

	lpdwFunc = (DWORD *)(dwCodeSeg+dwHeaderPos+0xC);
	lpdwFunc = (DWORD *)(lpdwFunc[0]+dwCodeSeg);
	PointMax = lpdwFunc[0x4B]+dwCodeSeg;
	PointMin = dwCodeSeg+0x1000;

	dwLen = PointMax-PointMin;

	lpFuncBackupBuff = new char[dwLen];
	memcpy( lpFuncBackupBuff , (void *)PointMin , dwLen );

	dwLen>>=2;
	dwKeepLen = dwLen;

	dwKeepMemCount = 0xcccccccc;

	for(cnt=0;cnt<dwLen;cnt++) {
		((DWORD *)lpFuncBackupBuff)[cnt] ^= dwKeepMemCount;
	}

	dwKeepMemStartPoint = PointMin ^dwKeepMemCount;
	dwKeepMemSrcPoint = (DWORD)lpFuncBackupBuff;
	dwKeepMemSrcPoint ^= dwKeepMemCount;

	lpFuncBackupBuff = (char *)dwAsmJmpCode;

	return TRUE;
}

//¸Þ¸ð¸® Ã½Å© Á¾·á
int CloseKeepMem()
{
	if ( lpFuncBackupBuff ) {
		lpFuncBackupBuff = (char *)(dwKeepMemSrcPoint^0xcccccccc);
		delete lpFuncBackupBuff;
		lpFuncBackupBuff = 0;
	}

	return TRUE;
}

//¸Þ¸ð¸® Ã½Å©
int	CheckKeepMem(DWORD ChkCount )
{
	DWORD	*lpFunc;
	DWORD	*lpFunc2;
	DWORD	dwCnt;
	DWORD	cnt;
	DWORD	ccCode = 0xcccccccc;
	DWORD	RoleCount;
	DWORD	dwCode;
	DWORD	dwTotal;
	DWORD	dwChkCnt;

	lpFunc = (DWORD *)(dwKeepMemStartPoint^ccCode);
	dwCnt = dwKeepMemCount^ccCode;

	lpFunc2 = (DWORD *)(dwKeepMemSrcPoint^ccCode);


	RoleCount = 0;

	if ( !ChkCount ) {
		dwChkCnt = 0xFFFFFFFF;
	}
	else {
		dwChkCnt = ChkCount;
	}

	dwTotal = 0;

	if ( ChkCount ) {
		for( cnt=dwCnt;cnt<dwKeepLen;cnt++ ) {
			dwCode = lpFunc2[cnt]^ccCode;
			if ( lpFunc[cnt]!=dwCode ) {
				Code_VramClear();
				return FALSE;
			}
			if ( RoleCount>=dwChkCnt ) break;
			RoleCount++;
		}
	}
	else {
		for( cnt=dwCnt;cnt<dwKeepLen;cnt++ ) {
			dwCode = lpFunc2[cnt]^ccCode;
			if ( lpFunc[cnt]!=dwCode ) {
				dwMemError = 111;
				dwMemError ^= dwPlayTime;
				Code_VramClear();
				return FALSE;
			}
			dwTotal += (dwCode*((cnt&0x3)+1));
			if ( RoleCount>=dwChkCnt ) break;
			RoleCount++;
		}
	}

	if ( cnt==dwKeepLen ) {
		dwCnt = 0;
		RoleCount = 0;
	}

	dwKeepMemCount = (dwCnt+RoleCount)^ccCode;

	if ( !ChkCount )
		return dwTotal;

	return TRUE;
}


//¸Þ¸ð¸® ÀüÃ¼ Ã½Å©
int	CheckKeepMemFull()
{
	DWORD dwCode1, dwFnTimer , Param3;
	DWORD cnt;
	DWORD dwCode;
	smTRANS_COMMAND	TransCommand;

	dwCode = CheckKeepMem( 0 );

	Code_VramLoad( &dwCode1 , &dwFnTimer , &Param3 );

	cnt = dwCode1 ^ ( (dwFnTimer<<16)|(dwFnTimer&0xFFFF) );

	if ( smWsockDataServer ) {
		TransCommand.code = smTRANSCODE_CHECK_NETSTATE2;
		TransCommand.size = sizeof( smTRANS_COMMAND );
		TransCommand.WParam = 0;
		TransCommand.LParam = 0;
		TransCommand.SParam = cnt;
		TransCommand.EParam = 0;
		smWsockDataServer->Send( (char *)&TransCommand , TransCommand.size , TRUE );
	}

	return TRUE;
}
*/

//½ÃÀÛ Æã¼Ç
int	PlaySub_EndFunc()
{


	return TRUE;
}

//DWORD	dwPlaySub_CheckResult = 1834343222;


int	Check_PlaySubReleaseFunc()
{
	DWORD	dwStartFunc;
	DWORD	dwEndFunc;
	DWORD* lpStartFunc;
	int		cnt, len;
	DWORD	dwChkSum;

	dwStartFunc = (DWORD)Code_VRamBuffOpen;
	dwEndFunc = (DWORD)Check_PlaySubReleaseFunc;
	len = (dwEndFunc - dwStartFunc) >> 2;

	dwChkSum = 0;

	lpStartFunc = (DWORD*)dwStartFunc;

	if (len < 0) return FALSE;

	for (cnt = 0; cnt < len; cnt++) {
		dwChkSum += (lpStartFunc[cnt] * ((cnt & 0x3FF) + 1));
	}

	dwPlaySub_CheckResult = dwChkSum;

	if (PLAYSUB_CHECKRESULT != dwChkSum) {
		/*
				FILE	*fp;
				char	szBuff[256];

				fp = fopen( "playsub_chk.txt" , "wb" );
				if ( fp ) {
					wsprintf( szBuff,  "½ÃÀÛ( %d ) ³¡( %d )  °á°ú ( %d )\r\n" , dwStartFunc , dwEndFunc , dwChkSum );
					fwrite( szBuff , lstrlen(szBuff),1,fp);


					wsprintf( szBuff,  "Code_VRamBuffOpen - %d\r\n" , Code_VRamBuffOpen );
					fwrite( szBuff , lstrlen(szBuff),1,fp);
					wsprintf( szBuff,  "Check_CodeSafe - %d\r\n" , Check_CodeSafe );
					fwrite( szBuff , lstrlen(szBuff),1,fp);
					wsprintf( szBuff,  "CheckTrade - %d\r\n" , CheckTrade );
					fwrite( szBuff , lstrlen(szBuff),1,fp);
					wsprintf( szBuff,  "CheckKeepMemFull - %d\r\n" , CheckKeepMemFull );
					fwrite( szBuff , lstrlen(szBuff),1,fp);
					wsprintf( szBuff,  "Check_PlaySubReleaseFunc - %d\r\n" , Check_PlaySubReleaseFunc );
					fwrite( szBuff , lstrlen(szBuff),1,fp);

					fclose(fp);
				}
		*/
		return dwChkSum;
	}

	return NULL;
}



sCHK_POTION_COUNT	sPotionTotalCount;
int	tXorPotionCount = 0;

//¹°¾à°¹¼ö ±â·Ï ¹ÝÀü
int	XorCheckPotion(sCHK_POTION_COUNT* ChkPotion)
{
	ChkPotion->Pl101 ^= tXorPotionCount;
	ChkPotion->Pl102 ^= tXorPotionCount;
	ChkPotion->Pl103 ^= tXorPotionCount;
	ChkPotion->Pl104 ^= tXorPotionCount;

	ChkPotion->Pm101 ^= tXorPotionCount;
	ChkPotion->Pm102 ^= tXorPotionCount;
	ChkPotion->Pm103 ^= tXorPotionCount;
	ChkPotion->Pm104 ^= tXorPotionCount;

	ChkPotion->Ps101 ^= tXorPotionCount;
	ChkPotion->Ps102 ^= tXorPotionCount;
	ChkPotion->Ps103 ^= tXorPotionCount;
	ChkPotion->Ps104 ^= tXorPotionCount;

	return TRUE;
}

//¹°¾à°¹¼ö ºñ±³
int	ComparePotion()
{
	sCHK_POTION_COUNT	ChkPotionCount;
	int	CompareError = 0;

	XorCheckPotion(&sPotionTotalCount);

	GetInvenPotionCount(&ChkPotionCount);

	if (ChkPotionCount.Pl101 > sPotionTotalCount.Pl101)
		CompareError++;
	if (ChkPotionCount.Pl102 > sPotionTotalCount.Pl102)
		CompareError++;
	if (ChkPotionCount.Pl103 > sPotionTotalCount.Pl103)
		CompareError++;
	if (ChkPotionCount.Pl104 > sPotionTotalCount.Pl104)
		CompareError++;

	if (ChkPotionCount.Pm101 > sPotionTotalCount.Pm101)
		CompareError++;
	if (ChkPotionCount.Pm102 > sPotionTotalCount.Pm102)
		CompareError++;
	if (ChkPotionCount.Pm103 > sPotionTotalCount.Pm103)
		CompareError++;
	if (ChkPotionCount.Pm104 > sPotionTotalCount.Pm104)
		CompareError++;

	if (ChkPotionCount.Ps101 > sPotionTotalCount.Ps101)
		CompareError++;
	if (ChkPotionCount.Ps102 > sPotionTotalCount.Ps102)
		CompareError++;
	if (ChkPotionCount.Ps103 > sPotionTotalCount.Ps103)
		CompareError++;
	if (ChkPotionCount.Ps104 > sPotionTotalCount.Ps104)
		CompareError++;

	int cnt1, cnt2;
	cnt1 = ChkPotionCount.Pl101 + ChkPotionCount.Pl102 + ChkPotionCount.Pl103 + ChkPotionCount.Pl104 +
		ChkPotionCount.Pm101 + ChkPotionCount.Pm102 + ChkPotionCount.Pm103 + ChkPotionCount.Pm104 +
		ChkPotionCount.Ps101 + ChkPotionCount.Ps102 + ChkPotionCount.Ps103 + ChkPotionCount.Ps104;

	cnt2 = sPotionTotalCount.Pl101 + sPotionTotalCount.Pl102 + sPotionTotalCount.Pl103 + sPotionTotalCount.Pl104 +
		sPotionTotalCount.Pm101 + sPotionTotalCount.Pm102 + sPotionTotalCount.Pm103 + sPotionTotalCount.Pm104 +
		sPotionTotalCount.Ps101 + sPotionTotalCount.Ps102 + sPotionTotalCount.Ps103 + sPotionTotalCount.Ps104;

	if (CompareError) {
		//¹°¾à ¿À·ù ¹ß°ß
		//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
		SendSetHackUser3(1530, CompareError, cnt1 - cnt2);
	}

	tXorPotionCount = dwPlayTime & 0xFFFFFFF;
	memcpy(&sPotionTotalCount, &ChkPotionCount, sizeof(sCHK_POTION_COUNT));
	XorCheckPotion(&sPotionTotalCount);

	return CompareError;
}



//¹°¾à °¹¼ö »õ·Î ¼³Á¤
int	ResetPotion()
{
	GetInvenPotionCount(&sPotionTotalCount);
	tXorPotionCount = dwPlayTime & 0xFFFFFFF;
	XorCheckPotion(&sPotionTotalCount);

	return TRUE;
}

//¹°¾à °¹¼ö »õ·Î ¼³Á¤
int	ResetPotion2()
{
	int cnt1, cnt2;
	sCHK_POTION_COUNT	ChkPotionCount;

	XorCheckPotion(&sPotionTotalCount);
	GetInvenPotionCount(&ChkPotionCount);

	//¾ÆÀÌÅÛ ¹°¾à ÀÔ¼ö
	cnt1 = ChkPotionCount.Pl101 + ChkPotionCount.Pl102 + ChkPotionCount.Pl103 + ChkPotionCount.Pl104 +
		ChkPotionCount.Pm101 + ChkPotionCount.Pm102 + ChkPotionCount.Pm103 + ChkPotionCount.Pm104 +
		ChkPotionCount.Ps101 + ChkPotionCount.Ps102 + ChkPotionCount.Ps103 + ChkPotionCount.Ps104;

	cnt2 = sPotionTotalCount.Pl101 + sPotionTotalCount.Pl102 + sPotionTotalCount.Pl103 + sPotionTotalCount.Pl104 +
		sPotionTotalCount.Pm101 + sPotionTotalCount.Pm102 + sPotionTotalCount.Pm103 + sPotionTotalCount.Pm104 +
		sPotionTotalCount.Ps101 + sPotionTotalCount.Ps102 + sPotionTotalCount.Ps103 + sPotionTotalCount.Ps104;

	if (cnt1 > (cnt2 + TransRecvItem.Item.PotionCount))
		return TRUE;

	memcpy(&sPotionTotalCount, &ChkPotionCount, sizeof(sCHK_POTION_COUNT));
	tXorPotionCount = dwPlayTime & 0xFFFFFFF;
	XorCheckPotion(&sPotionTotalCount);

	return FALSE;
}

//¾ÆÀÌÅÛÀÌ ¹°¾àÀÎ °æ¿ì ±â·Ï
int	IncPotionCount(sCHK_POTION_COUNT* ChkPotion, sITEMINFO* lpItem)
{
	int	PotionCount;

	PotionCount = lpItem->PotionCount;

	if (!PotionCount) return FALSE;

	if (lpItem->CODE == (sinPL1 | sin01))
		ChkPotion->Pl101 += PotionCount;
	if (lpItem->CODE == (sinPL1 | sin02))
		ChkPotion->Pl102 += PotionCount;
	if (lpItem->CODE == (sinPL1 | sin03))
		ChkPotion->Pl103 += PotionCount;
	if (lpItem->CODE == (sinPL1 | sin04))
		ChkPotion->Pl104 += PotionCount;


	if (lpItem->CODE == (sinPM1 | sin01))
		ChkPotion->Pm101 += PotionCount;
	if (lpItem->CODE == (sinPM1 | sin02))
		ChkPotion->Pm102 += PotionCount;
	if (lpItem->CODE == (sinPM1 | sin03))
		ChkPotion->Pm103 += PotionCount;
	if (lpItem->CODE == (sinPM1 | sin04))
		ChkPotion->Pm104 += PotionCount;


	if (lpItem->CODE == (sinPS1 | sin01))
		ChkPotion->Ps101 += PotionCount;
	if (lpItem->CODE == (sinPS1 | sin02))
		ChkPotion->Ps102 += PotionCount;
	if (lpItem->CODE == (sinPS1 | sin03))
		ChkPotion->Ps103 += PotionCount;
	if (lpItem->CODE == (sinPS1 | sin04))
		ChkPotion->Ps104 += PotionCount;

	return TRUE;
}


//ÀÎº¥Åä¸®¿Í °Å·¡Ã¢¿¡¼­ ¹°¾àÀ» Ã£¾î °¹¼ö ±â·Ï
int GetInvenPotionCount(sCHK_POTION_COUNT* ChkPotion)
{
	int cnt;

	ZeroMemory(ChkPotion, sizeof(sCHK_POTION_COUNT));

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag) {
			IncPotionCount(ChkPotion, &cInvenTory.InvenItem[cnt].sItemInfo);
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag) {
			IncPotionCount(ChkPotion, &cInvenTory.InvenItemTemp[cnt].sItemInfo);
		}
	}

	if (cTrade.OpenFlag) {
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			if (sTrade.TradeItem[cnt].Flag) {
				IncPotionCount(ChkPotion, &sTrade.TradeItem[cnt].sItemInfo);
			}
		}
	}

	if (MouseItem.Flag)
		IncPotionCount(ChkPotion, &MouseItem.sItemInfo);

	return TRUE;
}

int	GetItemCheckCode(sITEMINFO* lpItem)
{
	if (lpItem->CODE < sinPM1 && lpItem->ItemName[0]) {
		return lpItem->ItemHeader.dwChkSum;
	}
	return 0;
}


DWORD GetInvenItemCode()
{
	int cnt;

	DWORD	dwCode;


	dwCode = 0;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag) {
			dwCode += GetItemCheckCode(&cInvenTory.InvenItem[cnt].sItemInfo);
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag) {
			dwCode += GetItemCheckCode(&cInvenTory.InvenItemTemp[cnt].sItemInfo);
		}
	}

	if (cTrade.OpenFlag) {
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			if (sTrade.TradeItem[cnt].Flag) {
				dwCode += GetItemCheckCode(&sTrade.TradeItem[cnt].sItemInfo);
			}
		}
	}


	if (MouseItem.Flag) {
		dwCode += GetItemCheckCode(&MouseItem.sItemInfo);
	}

	if (cWareHouse.OpenFlag) {
		for (cnt = 0; cnt < 100; cnt++) {
			if (sWareHouse.WareHouseItem[cnt].Flag) {
				dwCode += GetItemCheckCode(&sWareHouse.WareHouseItem[cnt].sItemInfo);
			}
		}
	}


	if (Caravana::GetInstance()->OpenFlag) {
		for (cnt = 0; cnt < 100; cnt++) {
			if (sCaravan.CaravanItem[cnt].Flag) {
				dwCode += GetItemCheckCode(&sCaravan.CaravanItem[cnt].sItemInfo);
			}
		}
	}

	if (cCraftItem.OpenFlag) {
		for (cnt = 0; cnt < 15; cnt++) {
			if (sCraftItem.CraftItem[cnt].Flag) {
				dwCode += GetItemCheckCode(&sCraftItem.CraftItem[cnt].sItemInfo);
			}
		}
	}

	if (cAging.OpenFlag) {
		for (cnt = 0; cnt < 15; cnt++) {
			if (sAgingItem.AgingItem[cnt].Flag) {
				dwCode += GetItemCheckCode(&sAgingItem.AgingItem[cnt].sItemInfo);
			}
		}
	}

	// pluto Á¦·Ã
	if (SmeltingItem.OpenFlag)
	{
		for (cnt = 0; cnt < 5; cnt++)
		{
			if (S_smeltingItem.SmeltingItem[cnt].Flag)
			{
				dwCode += GetItemCheckCode(&S_smeltingItem.SmeltingItem[cnt].sItemInfo);
			}
		}
	}

	// pluto Á¦ÀÛ
	if (ManufactureItem.m_OpenFlag)
	{
		for (cnt = 0; cnt < 4; cnt++)
		{
			if (g_sManufactureItem.ManufactureItem[cnt].Flag)
			{
				dwCode += GetItemCheckCode(&g_sManufactureItem.ManufactureItem[cnt].sItemInfo);
			}
		}
	}
	//¹«±â¸¦ ÆÈ¶§ , ¹Í½ºÃÄ ¿¡ÀÌÂ¡ Á¾·á½Ã

	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã¢ÀÌ ¿­·ÁÀÖÀ»½Ã...
	if (cMixtureReset.OpenFlag)
	{
		for (cnt = 0; cnt < 2; cnt++)
		{
			if (sMixtureResetItem.MixtureResetItem[cnt].Flag)
			{
				dwCode += GetItemCheckCode(&sMixtureResetItem.MixtureResetItem[cnt].sItemInfo);
			}
		}
	}

	return dwCode;
}


DWORD	dwTotal_InvenItemCode;
DWORD	dwTotal_InvenItemPlayTime;


//ÀÌº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå ÃÊ±âÈ­
int	ResetInvenItemCode()
{
	//ÀÎº¥Åä¸®ÀÇ ¸ðµç ¾ÆÀÌÅÛÀ» È®ÀÎÇÏ¿© ÄÚµå¸¦ ¹ÝÈ¯
	dwTotal_InvenItemCode = GetInvenItemCode();
	dwTotal_InvenItemCode = dwPlayTime ^ dwTotal_InvenItemCode;
	dwTotal_InvenItemPlayTime = dwPlayTime;

	return TRUE;
}

//ÀÌº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå¿¡¼­ Æ¯Á¤ ¾ÆÀÌÅÛ Á¦°Å
int SubInvenItemCode(sITEMINFO* lpItem)
{
	DWORD	dwCode;

	dwCode = dwTotal_InvenItemCode ^ dwTotal_InvenItemPlayTime;
	dwTotal_InvenItemCode = dwCode - GetItemCheckCode(lpItem);

	dwTotal_InvenItemCode = dwPlayTime ^ dwTotal_InvenItemCode;
	dwTotal_InvenItemPlayTime = dwPlayTime;

	return TRUE;
}


//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå È®ÀÎ
int	CheckInvenItemCode()
{
	DWORD dwCode;
	smTRANS_COMMAND	smTransCommand;

	dwCode = GetInvenItemCode();

	if (dwCode != (dwTotal_InvenItemCode ^ dwTotal_InvenItemPlayTime)) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST3;
		smTransCommand.WParam = 7010;
		smTransCommand.LParam = dwTotal_InvenItemCode ^ dwTotal_InvenItemPlayTime;
		smTransCommand.SParam = dwCode;
		if (smWsockDataServer) {
			smWsockDataServer->Send((char*)&smTransCommand, sizeof(smTRANS_COMMAND), TRUE);
		}
	}

	dwTotal_InvenItemCode = dwPlayTime ^ dwCode;
	dwTotal_InvenItemPlayTime = dwPlayTime;

	return TRUE;
}


void CheckPlaySubFunc2()
{
	if (dwPlaySub_CheckResult != PLAYSUB_CHECKRESULT) {
		SendSetHackUser2(1120, dwPlaySub_CheckResult);
		DisconnectFlag = dwPlayTime;
	}
}

short	sChkLife[2];
short	sChkMana[2];
short	sChkStamina[2];
int		EnergyGraphErrorCount[4] = { 0,0,0,0 };


//¿¡³ÊÁö ±×·¡ÇÁ ¿À·ù Ã½Å©
int CheckEnergyGraphError()
{
	int Life, Mana, Stamina;
	smTRANS_COMMAND	smTransCommand;


	Life = EnergyGraphErrorCount[0] - EnergyGraphErrorCount[3];
	Mana = EnergyGraphErrorCount[1] - EnergyGraphErrorCount[3];
	Stamina = EnergyGraphErrorCount[2] - EnergyGraphErrorCount[3];

	//if ( Life>3 || Mana>3 || Stamina>3 ) {

	if (Life > 0 || Mana > 0 || Stamina > 0) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_CLIENT_ERROR;
		smTransCommand.WParam = 0;

		smTransCommand.LParam = Life;
		smTransCommand.SParam = Mana;
		smTransCommand.EParam = Stamina;

		if (smWsockDataServer)
			smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}

	EnergyGraphErrorCount[3] = lpCurPlayer->pX;
	EnergyGraphErrorCount[0] = EnergyGraphErrorCount[3] + Life;
	EnergyGraphErrorCount[1] = EnergyGraphErrorCount[3] + Life;
	EnergyGraphErrorCount[2] = EnergyGraphErrorCount[3] + Life;

	return TRUE;
}

//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
int	ResetEnergyGraph(int Num)
{
	int	Pos;

	Pos = lpCurPlayer->pZ;

	switch (Num) {
	case 0:
		//LIFE
		sChkLife[0] = Pos;
		sChkLife[1] = Pos ^ (-1);
		break;

	case 1:
		//MANA
		sChkMana[0] = Pos;
		sChkMana[1] = Pos ^ (-1);
		break;

	case 2:
		//STAMINA
		sChkStamina[0] = Pos;
		sChkStamina[1] = Pos ^ (-1);
		break;

	case 3:
	case 4:
		//ALL
		sChkLife[0] = Pos;
		sChkLife[1] = Pos ^ (-1);
		sChkMana[0] = Pos;
		sChkMana[1] = Pos ^ (-1);
		sChkStamina[0] = Pos;
		sChkStamina[1] = Pos ^ (-1);
		break;

	}

	return TRUE;
}

//¿¡³ÊÁö ±×·¡ÇÁÀÇ ¿À·ù Ã½Å©
int CheckEnergyGraph()
{
	int	Pos;
	int	Grp;
	int	Last_Grp;
	int cnt;

	Pos = lpCurPlayer->pZ;

	Last_Grp = sChkLife[0] ^ sChkLife[1];
	Grp = sinGetLife();
	if (Last_Grp != -1) {
		cnt = Grp - Last_Grp;
		if (cnt > 15) {
			EnergyGraphErrorCount[0] ++;
		}
	}
	sChkLife[0] = Pos;
	sChkLife[1] = Pos ^ Grp;

	Last_Grp = sChkMana[0] ^ sChkMana[1];
	Grp = sinGetMana();
	if (Last_Grp != -1) {
		cnt = Grp - Last_Grp;
		if (cnt > 15) {
			EnergyGraphErrorCount[1] ++;
		}
	}
	sChkMana[0] = Pos;
	sChkMana[1] = Pos ^ Grp;

	Last_Grp = sChkStamina[0] ^ sChkStamina[1];
	Grp = sinGetStamina();
	if (Last_Grp != -1) {
		cnt = Grp - Last_Grp;
		if (cnt > 50) {
			EnergyGraphErrorCount[2] ++;
		}
	}
	sChkStamina[0] = Pos;
	sChkStamina[1] = Pos ^ Grp;

	return TRUE;
}


sITEMINFO* FindInvenItem(DWORD dwItemCode, DWORD dwHead, DWORD dwChkSum)
{
	int cnt;
	sITEMINFO* lpItem;

	if ((dwItemCode & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) return NULL;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag) {
			lpItem = &cInvenTory.InvenItem[cnt].sItemInfo;
			if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
				return lpItem;
			}
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag) {
			lpItem = &cInvenTory.InvenItemTemp[cnt].sItemInfo;
			if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
				return lpItem;
			}
		}
	}

	if (cTrade.OpenFlag) {
		for (cnt = 0; cnt < MAX_TRADE_ITEM; cnt++) {
			if (sTrade.TradeItem[cnt].Flag) {
				lpItem = &sTrade.TradeItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
					return lpItem;
				}
			}
		}
	}

	if (MouseItem.Flag) {
		lpItem = &MouseItem.sItemInfo;
		if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
			return lpItem;
		}
	}

	if (cWareHouse.OpenFlag) {
		for (cnt = 0; cnt < 100; cnt++) {
			if (sWareHouse.WareHouseItem[cnt].Flag) {
				lpItem = &sWareHouse.WareHouseItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
					return lpItem;
				}
			}
		}
	}

	if (Caravana::GetInstance()->OpenFlag) {
		for (cnt = 0; cnt < 100; cnt++) {
			if (sCaravan.CaravanItem[cnt].Flag) {
				lpItem = &sCaravan.CaravanItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
					return lpItem;
				}
			}
		}
	}

	if (cCraftItem.OpenFlag) {
		for (cnt = 0; cnt < 15; cnt++) {
			if (sCraftItem.CraftItem[cnt].Flag) {
				lpItem = &sCraftItem.CraftItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
					return lpItem;
				}
			}
		}
	}

	if (cAging.OpenFlag) {
		for (cnt = 0; cnt < 15; cnt++) {
			if (sAgingItem.AgingItem[cnt].Flag) {
				lpItem = &sAgingItem.AgingItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum) {
					return lpItem;
				}
			}
		}
	}
	// pluto Á¦·Ã
	if (SmeltingItem.OpenFlag)
	{
		for (cnt = 0; cnt < 5; cnt++)
		{
			if (S_smeltingItem.SmeltingItem[cnt].Flag)
			{
				lpItem = &S_smeltingItem.SmeltingItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum)
				{
					return lpItem;
				}
			}
		}
	}

	// pluto Á¦ÀÛ
	if (ManufactureItem.m_OpenFlag)
	{
		for (cnt = 0; cnt < 4; cnt++)
		{
			if (g_sManufactureItem.ManufactureItem[cnt].Flag)
			{
				lpItem = &g_sManufactureItem.ManufactureItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum)
				{
					return lpItem;
				}
			}
		}
	}

	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã³¸®
	if (cMixtureReset.OpenFlag)
	{
		for (cnt = 0; cnt < 2; cnt++)
		{
			if (sMixtureResetItem.MixtureResetItem[cnt].Flag)
			{
				lpItem = &sMixtureResetItem.MixtureResetItem[cnt].sItemInfo;
				if (lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum)
					return lpItem;
			}
		}
	}

	return NULL;
}

//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ º¹»ç ¿À·ù
int SendInvenItemError(DWORD dwKind, DWORD dwItemCode, DWORD dwHead, DWORD dwChkSum)
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_INVEN_ERR_ITEM;
	smTransCommand.WParam = dwKind;

	smTransCommand.LParam = dwItemCode;
	smTransCommand.SParam = dwHead;
	smTransCommand.EParam = dwChkSum;

	if (smWsockDataServer)
		smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}

int	GetTotalInvenMoney()
{
	int Money;

	Money = lpCurPlayer->smCharInfo.Money;

	if (cTrade.OpenFlag && sTrade.Money)
		Money += sTrade.Money - 193;

	if (cWareHouse.OpenFlag)
		Money += sWareHouse.Money - 2023;

	if (Caravana::GetInstance()->OpenFlag)
		Money += sCaravan.Money - 2023;

	return Money;
}

DWORD	dwTotal_CheckMoney = 0;
DWORD	dwTotal_CheckMoneyTime = 0;
#define	TOTAL_CHECK_MONEY_MASK	0x0018cccc
int		NChkMode = 0;


//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
int	ResetInvenMoney()
{
	dwTotal_CheckMoney = GetTotalInvenMoney();
	dwTotal_CheckMoney = dwPlayTime ^ dwTotal_CheckMoney;
	dwTotal_CheckMoneyTime = dwPlayTime ^ TOTAL_CHECK_MONEY_MASK;

	return TRUE;
}


//ÀÎº¥Åä¸® °Ë»ç¿ëµ· Ãß°¡
int	AddInvenMoney(int Money)
{
	int	m;

	m = dwTotal_CheckMoney ^ (dwTotal_CheckMoneyTime ^ TOTAL_CHECK_MONEY_MASK);
	m += Money;
	if (m < 0) m = 0;

	dwTotal_CheckMoney = dwPlayTime ^ m;
	dwTotal_CheckMoneyTime = dwPlayTime ^ TOTAL_CHECK_MONEY_MASK;

	return TRUE;
}

//ÀÎº¥Åä¸® µ· °Ë»ç
int	CheckInvenMoney()
{
	DWORD dwCode;
	smTRANS_COMMAND	smTransCommand;

	if (cShop.OpenFlag || SkillMasterFlag || cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag)  // ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã¢ Ãß°¡
	{
		NChkMode = TRUE;
		return TRUE;
	}

	if (NChkMode) {
		NChkMode = 0;
		ResetInvenMoney();
		if (NChkMode == 0) return TRUE;
		dwCode = 0;
	}
	else {
		dwCode = GetTotalInvenMoney();
	}

	if (dwCode != (dwTotal_CheckMoney ^ (dwTotal_CheckMoneyTime ^ TOTAL_CHECK_MONEY_MASK))) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_INVEN_ERR_MONEY;
		smTransCommand.WParam = 0;
		smTransCommand.LParam = dwCode;
		smTransCommand.SParam = (dwTotal_CheckMoney ^ (dwTotal_CheckMoneyTime ^ TOTAL_CHECK_MONEY_MASK));
		smTransCommand.EParam = 0;
		if (smWsockDataServer) {
			smWsockDataServer->Send((char*)&smTransCommand, sizeof(smTRANS_COMMAND), TRUE);
		}
	}

	dwTotal_CheckMoney = dwPlayTime ^ dwCode;
	dwTotal_CheckMoneyTime = dwPlayTime ^ TOTAL_CHECK_MONEY_MASK;

	return TRUE;
}



//ÀÎº¥Åä¸®ÀÇ ÀåÂø ¾ÆÀÌÅÛ Á¤º¸ ¼­¹ö¿¡ º¸³»±â
int SendUsingItemListToServer()
{

	int cnt;
	sITEMINFO* lpItem;

	TRANS_USINGITEM	TransUsingItem;

	ZeroMemory(&TransUsingItem, sizeof(TRANS_USINGITEM));

	TransUsingItem.size = sizeof(TRANS_USINGITEM);
	TransUsingItem.code = smTRANSCODE_INVEN_POSITION;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].ItemPosition) {
			lpItem = &cInvenTory.InvenItem[cnt].sItemInfo;
			if (lpItem->ItemName[0]) {
				if (TransUsingItem.ItemListCount >= 16) break;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].dwCode = lpItem->CODE;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[0] = cInvenTory.InvenItem[cnt].ItemPosition;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[1] = lpItem->Damage[1];
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[2] = lpItem->Defence;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[3] = (short)lpItem->fBlock_Rating;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[4] = (short)lpItem->fAbsorb;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[5] = lpItem->Attack_Rating;
				TransUsingItem.ItemListCount++;
			}
		}
	}

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItemTemp[cnt].Flag && cInvenTory.InvenItemTemp[cnt].ItemPosition) {
			lpItem = &cInvenTory.InvenItemTemp[cnt].sItemInfo;
			if (lpItem->ItemName[0]) {
				if (TransUsingItem.ItemListCount >= 16) break;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].dwCode = lpItem->CODE;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[0] = cInvenTory.InvenItemTemp[cnt].ItemPosition | 0x100;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[1] = lpItem->Damage[1];
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[2] = lpItem->Defence;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[3] = (short)lpItem->fBlock_Rating;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[4] = (short)lpItem->fAbsorb;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[5] = lpItem->Attack_Rating;
				TransUsingItem.ItemListCount++;
			}
		}
	}

	smWINSOCK* lpsmSendSock;

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â
	if (lpsmSendSock) {
		return lpsmSendSock->Send((char*)&TransUsingItem, TransUsingItem.size, TRUE);
	}

	return NULL;
}

//ÀÎº¥Åä¸®ÀÇ ÀåÂø ¾ÆÀÌÅÛ Á¤º¸ ¼­¹ö¿¡ º¸³»±â
int SendUsingItemListToServer2()
{

	int cnt;
	sITEMINFO* lpItem;

	TRANS_USINGITEM	TransUsingItem;

	ZeroMemory(&TransUsingItem, sizeof(TRANS_USINGITEM));

	TransUsingItem.size = sizeof(TRANS_USINGITEM);
	TransUsingItem.code = smTRANSCODE_INVEN_POSITION2;

	for (cnt = 0; cnt < INVENTORY_MAXITEM; cnt++) {
		if (cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].ItemPosition) {
			lpItem = &cInvenTory.InvenItem[cnt].sItemInfo;
			if (lpItem->ItemName[0]) {
				if (TransUsingItem.ItemListCount >= 16) break;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].dwCode = lpItem->CODE;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[0] = cInvenTory.InvenItem[cnt].ItemPosition;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[1] = lpItem->Damage[1];
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[2] = lpItem->Defence;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[3] = (short)lpItem->fBlock_Rating;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[4] = (short)lpItem->fAbsorb;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[5] = lpItem->Attack_Rating;
				TransUsingItem.ItemListCount++;
			}
		}
	}

	smWINSOCK* lpsmSendSock;

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â
	if (lpsmSendSock) {
		return lpsmSendSock->Send((char*)&TransUsingItem, TransUsingItem.size, TRUE);
	}

	return NULL;
}


//½Ã°£ ±¸ÇÏ±â (time_t)
DWORD	GetPlayTime_T()
{
	DWORD dwTime;

	if (dwTime_ServerT) {
		dwTime = (dwPlayTime - dwTime_ConnectMS) / 1000;
		dwTime = dwTime_ServerT + dwTime;				//ÇöÀçÀÇ Time_T
		return dwTime;
	}

	return NULL;
}


//¸Ó¸®°¡ Ä¿Áö´Â ¸ðµå ¼³Á¤ 
int	ChangeBigHeadMode(int Mode, int Time, bool bNew)
{
	DWORD dwTime;

	if (bNew)
	{

	}
	else
	{
		if (Mode)
		{
			if (Mode == 3) // ¹ÚÀç¿ø - 2010 ¿ùµåÄÅ ÀÌº¥Æ® - Ãà±¸°ø Æ÷¼Ç »ç¿ë
			{
				lpCurPlayer->smCharInfo.SizeLevel = 1; // Á¤»ó ¾ó±¼
				ChangeModelSkin(lpCurPlayer, 0, 6, 0, 0);	// Ãà±¸°ø ¾´ ¾ó±¼
			}
			else
			{
				lpCurPlayer->smCharInfo.SizeLevel = Mode + 0x1000;
				ChangeModelSkin(lpCurPlayer, 1, lpCurPlayer->smCharInfo.SizeLevel + 3 - 0x1000, 0, 0);	//Å«¾ó±¼
			}
			dwTime = GetPlayTime_T();

			if (dwTime)
				lpCurPlayer->smCharInfo.dwEventTime_T = dwTime + Time;	//¸ðµ¨Á¦ÇÑ ½Ã°£
			else
				lpCurPlayer->smCharInfo.dwEventTime_T = 0;				//½Ã°£Á¦ÇÑ ¾øÀ½ ( ÀçÁ¢ ÇÒ¶§±îÁö )
		}
		else
		{
			lpCurPlayer->smCharInfo.SizeLevel = 1;
			lpCurPlayer->smCharInfo.dwEventTime_T = 0;
			ChangeModelSkin(lpCurPlayer, 0, lpCurPlayer->smCharInfo.ChangeJob, 0, GetFaceHairSkin(lpCurPlayer));				//Á¤»ó ¾ó±¼ // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
		}
	}

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();
	SendSetObjectSerial(lpCurPlayer->dwObjectSerial);

	if (RestartPlayCount < 350)
		RestartPlayCount = 350;

	return TRUE;
}

int GetAttackSpeedMainFrame(int AttackSpeed)
{
	int cnt;

	cnt = AttackSpeed - 6;
	if (cnt < 0) cnt = 0;
	if (cnt > 6) cnt = 6;

	return fONE + 32 * cnt;
}

//°ø°Ý¼Óµµ¸¦ ÇÁ·¹ÀÓ¼Óµµ·Î º¯È¯
int	GetAttackSpeedFrame(int AttackSpeed, int AddSpeed)
{
	int cnt;

	cnt = GetAttackSpeedMainFrame(AttackSpeed);
	if (AddSpeed < 6 && AddSpeed>0)
		cnt += AddSpeed * 32;

	return (80 * cnt) >> FLOATNS;
}

//°ø°Ý¼Óµµ¸¦ ÇÁ·¹ÀÓ¼Óµµ·Î º¯È¯
int	GetAttackSpeedFrame(int AttackSpeed)
{
	int cnt;

	cnt = GetAttackSpeedMainFrame(AttackSpeed);

	return (80 * cnt) >> FLOATNS;
}







//Æú´õ ³»ÀÇ ini ÆÄÀÏ ªO¾Æ ÀÐ±â
int LoadFolder_Ini(char* szPath, char* szWildCard, char* szChgExt, int OftSave)
{

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szFilePath[64];
	char* lpPath;

	HANDLE hFindHandle2;
	WIN32_FIND_DATA	fd2;
	char	szFindPath2[64];

	smDPAT* lpDPatTest[65536];
	int		DPatCount = 0;
	int		cnt;

	lstrcpy(szFindPath, szPath);
	lstrcat(szFindPath, "\\*.*");


	hFindHandle = FindFirstFile(szFindPath, &fd);
	if (hFindHandle != INVALID_HANDLE_VALUE) {
		while (1) {
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {


				wsprintf(szFindPath2, "%s\\%s\\%s", szPath, fd.cFileName, szWildCard);

				hFindHandle2 = FindFirstFile(szFindPath2, &fd2);
				if (hFindHandle2 != INVALID_HANDLE_VALUE) {
					while (1) {
						wsprintf(szFilePath, "%s\\%s\\%s", szPath, fd.cFileName, fd2.cFileName);


						if (OftSave) {
							//ÀÚÁÖ ¾²±â ±â·Ï

							if (szChgExt)
								lpPath = ChangeFileExt(szFilePath, szChgExt);
							else
								lpPath = 0;

							if (!lpPath) lpPath = szFilePath;

							lstrcpy(szFilePath, lpPath);

							LoadOftenMeshPattern(szFilePath);
						}
						else {
							//´Ü¼ø Å×½ºÆ®
							lpDPatTest[DPatCount] = smPattern.LoadCharactor(szFilePath);
							if (lpDPatTest[DPatCount]) DPatCount++;
							else {
								DPatCount = DPatCount;
							}
						}

						if (FindNextFile(hFindHandle2, &fd2) == FALSE) break;
					}
					FindClose(hFindHandle2);
				}
			}

			//´ÙÀ½ ÆÄÀÏ Ã£À½
			if (FindNextFile(hFindHandle, &fd) == FALSE) break;
		}
		FindClose(hFindHandle);
	}

	for (cnt = 0; cnt < DPatCount; cnt++) {
		smDPAT_Delete(lpDPatTest[cnt]);
	}

	return TRUE;
}

sSOD_SCORE	sSodScore;
int			SodScoreFlag = 0;
extern SBL_SetFontEffect SoD_SetFontEffect;	//SoD ¿ë ÆùÆ® ÀÌÆåÆ®

//sSodScore.dwSoD_NextStageTime

//SoD ¶ó¿îµå ÀÌÆåÆ®
int	Sod_SodEffect(int Round)
{

	if (!BellatraEffectInitFlag) return FALSE;

	SetBellatraFontEffect(E_BL_FONT_ROUND);

	switch (Round) {
	case 1:
		SetBellatraFontEffect(E_BL_FONT_COUNT_1);
		break;
	case 2:
		SetBellatraFontEffect(E_BL_FONT_COUNT_2);
		break;
	case 3:
		SetBellatraFontEffect(E_BL_FONT_COUNT_3);
		break;
	case 4:
		SetBellatraFontEffect(E_BL_FONT_COUNT_4);
		break;
	case 5:
		SetBellatraFontEffect(E_BL_FONT_COUNT_5);
		break;
	case 6:
		SetBellatraFontEffect(E_BL_FONT_COUNT_6);
		break;
	case 7:
		SetBellatraFontEffect(E_BL_FONT_COUNT_7);
		break;
	case 8:
		SetBellatraFontEffect(E_BL_FONT_COUNT_8);
		break;
	}
	SetBellatraFontEffect(E_BL_FONT_START);

	esPlayContSound(Round);
	//esPlayContSound( 0 );	//Start


	return TRUE;
}



SBL_SetFontEffect SoD_SetFontEffect_CloseScreen;	//SoD ¿ë ÆùÆ® ÀÌÆåÆ®
char	szSOD_String[64] = { 0, };
BYTE	bSOD_StringColor[4];
int		SOD_StringCount = 0;
int		TeamScoreSort[4];
int		TeamMarkSort[4] = { 3 , 0 , 2 , 1 };
int		SoDGateFlag;


/*
struct  SBL_EventScore
{
	int  TeamNum;
	int  TeamMarkBuff[4];
	char szTeamPointBuff[4][32];

	int  EntryTeamNum;
	int  EntryTeamMarkBuff[2];
};
*/

//SOD Á¡¼ö Ãâ·ÂÇ¥ ÀÛ¼º
int	SetSodScoreResult()
{
	int cnt;

	g_EventScore.TeamNum = 0;
	g_EventScore.EntryTeamNum = 0;

	for (cnt = 0; cnt < 4; cnt++) {
		g_EventScore.TeamMarkBuff[cnt] = TeamMarkSort[TeamScoreSort[cnt]];
		wsprintf(g_EventScore.szTeamPointBuff[cnt], "%d", sSodScore.TeamScore[TeamScoreSort[cnt]]);
	}
	for (cnt = 0; cnt < 2; cnt++) {
		g_EventScore.EntryTeamMarkBuff[cnt] = g_EventScore.TeamMarkBuff[cnt];
	}

	if (sSodScore.Round > 0 && sSodScore.Round <= 4) {
		g_EventScore.TeamNum = 4;
		g_EventScore.EntryTeamNum = 2;
	}
	if (sSodScore.Round > 4 && sSodScore.Round <= 6) {
		g_EventScore.TeamNum = 2;
		g_EventScore.EntryTeamNum = 1;
	}

	if (!g_EventScore.TeamNum) return FALSE;

	return TRUE;
}


//SoD ¿¬Ãâ
int	SodScoreMain()
{
	int dx, dy, dz;
	int x, z, cnt;
	//	char	szBuff[256];
	/*
		g_EventScore.TeamNum = 4;
		g_EventScore.TeamMarkBuff[0]=1;
		g_EventScore.TeamMarkBuff[1]=0;
		g_EventScore.TeamMarkBuff[2]=2;
		g_EventScore.TeamMarkBuff[3]=3;

		lstrcpy( g_EventScore.szTeamPointBuff[0] , "100" );
		lstrcpy( g_EventScore.szTeamPointBuff[1] , "100" );
		lstrcpy( g_EventScore.szTeamPointBuff[2] , "100" );
		lstrcpy( g_EventScore.szTeamPointBuff[3] , "100" );

		g_EventScore.EntryTeamNum = 2;
		g_EventScore.EntryTeamMarkBuff[0] = 1;
		g_EventScore.EntryTeamMarkBuff[1] = 2;

		SetBellatraFontEffect( E_BL_QUAKE_SEAL );
		SetBellatraFontEffect( E_BL_FONT_QUAKE_SEAL );

		E_BL_QUAKE_SEAL	   = 18,
		E_BL_STUN_SEAL	   = 19,
		E_BL_FREEZE_SEAL   = 20,
		E_BL_RABBIE_SEAL   = 21,
		E_BL_STYGIAN_SEAL  = 22,
		E_BL_GUARDIAN_SEAL = 23,
		E_BL_POINT_SEAL	   = 24,
		E_BL_FONT_QUAKE_SEAL	= 25,
		E_BL_FONT_STUN_SEAL	    = 26,
		E_BL_FONT_FREEZE_SEAL   = 27,
		E_BL_FONT_RABBIE_SEAL   = 28,
		E_BL_FONT_STYGIAN_SEAL  = 29,
		E_BL_FONT_GUARDIAN_SEAL = 30,
		E_BL_FONT_POINT_SEAL	= 31,

	*/


	if (!SodScoreFlag) {
		SodScoreFlag = TRUE;
		ZeroMemory(&sSodScore, sizeof(sSOD_SCORE));
	}

	if (SOD_StringCount) SOD_StringCount--;


	if (sSodScore.dwSoD_NextStageTime) {

		if (sSodScore.dwSoD_CloseStageTime && sSodScore.dwSoD_CloseStageTime < dwPlayTime) {

			if (SoD_SetFontEffect_CloseScreen.eBL_Type != E_BL_CHANGE_TRUE) {

				if (SoDGateFlag == TRUE && SetSodScoreResult() == TRUE)
					SetBellatraFontEffect(E_BL_CHANGE_TRUE, &SoD_SetFontEffect_CloseScreen, E_BL_EVENT_SCORE);		//´Ý´Â È­¸é ½ÃÀÛ
				else
					SetBellatraFontEffect(E_BL_CHANGE_TRUE, &SoD_SetFontEffect_CloseScreen);		//´Ý´Â È­¸é ½ÃÀÛ


				esPlaySound(18, 400);
			}

			sSodScore.dwSoD_CloseStageTime = 0;
		}

		if (sSodScore.dwSoD_NextStageTime < dwPlayTime) {

			if (SoD_SetFontEffect_CloseScreen.eBL_Type == E_BL_CHANGE_TRUE) {
				CheckBellatraFontEffect(&SoD_SetFontEffect_CloseScreen, TRUE);					//´Ý´Â È­¸é Á¦°Å
				SetBellatraFontEffect(E_BL_CHANGE_FALSE, &SoD_SetFontEffect_CloseScreen);		//¿©´Â È­¸é ½ÃÀÛ
				esPlaySound(18, 400);
			}

			if (sSodScore.SodNextStageNum >= 0 && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD) {
				if (rsCheckHardCoreEventArea(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ) == TRUE && sSodScore.SodNextStageNum < 100) {


					dx = rsHardCoreEventOpenPos[sSodScore.SodNextStageNum][2].x * fONE;
					dy = rsHardCoreEventOpenPos[sSodScore.SodNextStageNum][2].y * fONE;
					dz = rsHardCoreEventOpenPos[sSodScore.SodNextStageNum][2].z * fONE;

					CloseEachPlayer();

					WarpFieldNearPos(rsSOD_FIELD, dx, dz, &x, &z);

					lpCurPlayer->SetPosi(dx, 0, dz, 0, 0, 0);
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

					sSodScore.EffectStartCount = 70 * 1;
					sSodScore.EffectCode = sSodScore.NextRound;
				}
			}
			else if (sSodScore.SodNextStageNum == -1) {
				//½ÇÆÐ Å»¶ô
				dx = sField[rsSOD_VILLAGE].cX * fONE;
				dz = sField[rsSOD_VILLAGE].cZ * fONE;

				CloseEachPlayer();

				WarpFieldNearPos(rsSOD_VILLAGE, dx, dz, &x, &z);

				lpCurPlayer->SetPosi(dx, 0, dz, 0, 0, 0);
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
			}
			else if (sSodScore.SodNextStageNum == -2) {
				//·Îµù È­¸é Á¾·á
				if (SoD_SetFontEffect.eBL_Type == E_BL_LODING) {
					CheckBellatraFontEffect(&SoD_SetFontEffect, TRUE);	//·Îµù Á¾·á
				}
			}

			lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_STAND);
			lpCurPlayer->dwNextMotionCode = 0;
			sSodScore.dwSoD_NextStageTime = 0;
		}
		else {
			if (sSodScore.SodNextStageNum >= 0)
				lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_YAHOO;
		}

	}

	if (sSodScore.EffectStartCount) {
		sSodScore.EffectStartCount--;
		if (sSodScore.EffectStartCount == 0) {
			switch (sSodScore.EffectCode) {
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				//SoD ¶ó¿îµå ÀÌÆåÆ®
				Sod_SodEffect(sSodScore.EffectCode);
				break;
			}
		}
	}


	if (sSodScore.ScoreEffectCount) {
		sSodScore.ScoreEffectCount -= 8;
		if (sSodScore.ScoreEffectCount < 0)
			sSodScore.ScoreEffectCount = 0;
	}

	//°ø¼ºÀü BlessCastle ½Â¸® ¾ßÈ£ÇÏ±â
	if (rsBlessCastle.CastleMode == 3) {
		if (rsBlessCastle.dwMasterClan && rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan)) {
			if (!lpCurPlayer->dwNextMotionCode) {
				lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_YAHOO;
			}
		}
	}


	return TRUE;

}



//Sod ÆÀ¼øÀ§ ¼³Á¤
int SodTeamSort()
{
	int cnt, cnt1;
	int r1, r2;

	for (cnt = 0; cnt < 4; cnt++) TeamScoreSort[cnt] = cnt;

	//sSodScore.TeamScore[cnt]

	for (cnt = 0; cnt < 4; cnt++) {
		for (cnt1 = cnt; cnt1 < 4; cnt1++) {
			r1 = TeamScoreSort[cnt];
			r2 = TeamScoreSort[cnt1];

			if (sSodScore.TeamScore[r1] < sSodScore.TeamScore[r2]) {
				TeamScoreSort[cnt] = r2;
				TeamScoreSort[cnt1] = r1;
			}
		}
	}

	return TRUE;
}

//Pontuacao de SOD
int	DisplaySodScore()
{
	char szBuff[256];
	int x, y;
	int cnt, cnt2;
	DWORD	dwColor;
	DWORD tM, tS;
	int len;
	int RnkCnt = 0;

	if (!SodScoreFlag) {
		SodScoreFlag = TRUE;
		ZeroMemory(&sSodScore, sizeof(sSOD_SCORE));
	}

	//psSetDefaultRenderState();

	if (SOD_StringCount) {
		len = lstrlen(szSOD_String);
		if ((SOD_StringCount & 0x8) == 0) {
			DrawFontImage(szSOD_String, MidX - len * 14, (MidY / 2) - (MidY - 500),
				D3DCOLOR_RGBA(bSOD_StringColor[0], bSOD_StringColor[1], bSOD_StringColor[2], 255), 2.0f);
		}
	}

	if (sSodScore.dwDispTime > dwPlayTime && rsCheckHardCoreEventArea(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ) == TRUE) {

		x = (WinSizeX / 2) - 80; y = 8;

		wsprintf(szBuff, "ROUND %d", sSodScore.Round);
		DrawFontImage(szBuff, x - 40, y, D3DCOLOR_RGBA(80, 255, 40, 255), 0.9f);

		if (sSodScore.dwPlayTime) {
			tM = (dwPlayTime - sSodScore.dwPlayTime) / 1000;
			tS = tM % 60;
			tM = tM / 60;
			if (tM > 99 || tM < 0) tM = 99;
		}
		else {
			tM = 0;
			tS = 0;
		}

		wsprintf(szBuff, "PlayTime %d:%d", tM, tS);
		DrawFontImage(szBuff, x + 160, y + 2, D3DCOLOR_RGBA(80, 160, 255, 255), 0.7f);

		y += 24;

		wsprintf(szBuff, "Score %d", sSodScore.Score);

		if (sSodScore.ScoreEffectCount)
			DrawFontImage(szBuff, x - 40, y + 8, D3DCOLOR_RGBA(255, 128, 64, 255 - sSodScore.ScoreEffectCount), 0.9f);
		else
			DrawFontImage(szBuff, x - 40, y + 8, D3DCOLOR_RGBA(255, 255, 0, 255), 0.9f);


		SodTeamSort();

		for (cnt = 0; cnt < 4; cnt++) {
			cnt2 = TeamScoreSort[cnt];

			if (sSodScore.MyTeam == cnt2)
				dwColor = D3DCOLOR_RGBA(255, 255, 0, 255);
			else
				dwColor = D3DCOLOR_RGBA(255, 128, 0, 255);

			DrawSpecialFontImage(TeamMarkSort[cnt2], x + 160, y, D3DCOLOR_RGBA(255, 255, 255, 255), 0.7f);

			wsprintf(szBuff, "Team %d", sSodScore.TeamScore[cnt2]);
			DrawFontImage(szBuff, x + 190, y, dwColor, 0.7f);
			y += 18;
		}
	}

	return FALSE;
}






