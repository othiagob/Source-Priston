#include "Utils\common.h"
#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "character.h"
#include "playmain.h"

#include "fileread.h"


#include "netplay.h"

#include "sinbaram\\sinlinkheader.h"


#include "field.h"
#include "effectsnd.h"

#include "record.h"
#include "playsub.h"
#include "Resource/resource.h"

#include "language.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "srcserver\\onserver.h"


#include "TextMessage.h"




extern HINSTANCE hinst; 

/////////////////////// ¼­¹öÃø ÇÔ¼ö ºÒ·¯´Ù ¾´´Ù //////////////////////
//ÀÌº¥Æ® Áö¿ªÀÎÁö Ã½Å©
int rsCheckHardCoreEventArea( int x, int y, int z ) ;
extern POINT3D	rsHardCoreEventOpenPos[7][3];

//////////////////////////////////////////////////////////////////////

int	CodeXorExp  = 0;				//°æÇèÄ¡ ¼û±èÄÚµå_
int	CodeXorExp_High  = 0;			//°æÇèÄ¡ ¼û±èÄÚµå
int	CodeXorLife = 0;				//»ý¸í·Â ¼û±èÄÚµå
int	CodeXorMana = 0;				//±â·Â ¼û±èÄÚµå
int	CodeXorStamina = 0;				//±Ù·Â ¼û±èÄÚµå
int	CodeXorMoney  = 0;				//µ· ¼û±èÄÚµå

//ÀÌÂÊ ¸ðµâ º¸È£ Ã½Å©¼¶ ÄÚµå
#define	PLAYSUB_CHECKRESULT			                                                               1884018394

DWORD	dwPlaySub_CheckResult = PLAYSUB_CHECKRESULT;

static char *szArmorDirectory = "char\\tmABCD\\";
static char *szNpcDirectory = "char\\npc\\";


#ifdef _PLAY_MORYON
#define MAX_JOBNUM	8
#else
#define MAX_JOBNUM	4
#endif

#include "playmodel.h"

int ArmorJobNum[9] = { 0, 1, 0, 3, 2, 5, 4, 7, 6 };

int	PlayerArmor = 0;

#define TEMP_DYNPAT_MAX		2048

smDPAT	*DynTempPat[TEMP_DYNPAT_MAX];

int DynTempPatCnt = 0;

int	PlayUsed_Resurrection = 0;		//ºÎÈ° ¾ÆÀÌÅÛ »ç¿ë


DWORD	dwSafeCodeSum;
//0xE9
BYTE	dwAsmJmpCode = 0xE0;

DWORD	dwStartFuncPoint=0;
DWORD	dwFuncLen=0;
DWORD	dwFuncSafeMemCount;
DWORD	dwFuncSafeMemCode;

//////////////////////////////////////////
DWORD	dwFuncChkTime = 0;
DWORD	dwFuncSafeCode = 0;
//////////////////////////////////////////

char	*lpFuncBackupBuff = 0;



void*	lpDDS_CodeSaveBuff =0;

extern char *szBangImage;

DWORD	*lpCodeVram = 0;

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

int Code_VramSave( DWORD Param1, DWORD Param2 , DWORD Param3 )
{
	DWORD	dwTime;

	if ( lpCodeVram ) {
		dwTime = GetCurrentTime();

		lpCodeVram[3] = dwTime;
		lpCodeVram[0] = Param1^dwTime;
		lpCodeVram[1] = Param2^dwTime;
		lpCodeVram[2] = Param3^dwTime;
	}

	return TRUE;
}

int Code_VramLoad( DWORD *Param1, DWORD *Param2 , DWORD *Param3 )
{
	*Param1 = lpCodeVram[0]^lpCodeVram[3];
	*Param2 = lpCodeVram[1]^lpCodeVram[3];
	*Param3 = lpCodeVram[2]^lpCodeVram[3];

	return TRUE;
}

int	Mix_CodeVram()
{
	DWORD	dwTime;
	DWORD	dwNewTime;


	dwTime = (dwPlayTime>>4)&0xFF;
	dwNewTime = (dwTime<<24)|(dwTime<<16)|(dwTime<<8)|dwTime;

	dwTime = lpCodeVram[3];

	lpCodeVram[0] = (lpCodeVram[0]^dwTime)^dwNewTime;
	lpCodeVram[1] = (lpCodeVram[1]^dwTime)^dwNewTime;
	lpCodeVram[2] = (lpCodeVram[2]^dwTime)^dwNewTime;
	lpCodeVram[3] = dwNewTime;

	return TRUE;
}


//////////////// ÇÁ·Î±×·¡¹Ö ÄÚµå º¸È£ /////////////
/////////////// ( µð½º¾î¼Àºí Á¶ÀÛ¹æÁö ) ///////////
//ÄÚµåº¸È£
DWORD Check_CodeSafe( DWORD dwStartFunc )
{
	BYTE	*lpFunc;
	//int		cnt;

	//DWORD	dwFunc;
	DWORD	dwCode;

	DWORD	*lpdwFunc;
	BYTE	JmpCode = 9;


	DWORD	dwHeaderPos = 0x30;

	//DRZ_EDIT (remove for prevent crashing)
	/*
	JmpCode+=dwAsmJmpCode;

	dwStartFunc = 0x4c11cc;

	lpFunc = (BYTE *)dwStartFunc;
	dwCodeSeg = dwStartFunc&0xFFF00000;
	dwCodeAddr = dwStartFunc&0x000FFFFF;

	lpdwFunc = (DWORD *)(dwCodeSeg+dwHeaderPos+0xC);
	lpdwFunc = (DWORD *)(lpdwFunc[0]+dwCodeSeg);
	PointMax = lpdwFunc[0x4B]+dwCodeSeg;
	PointMin = dwCodeSeg+0x1000;
	*/

/*
	dwFunc = ((DWORD *)(lpFunc+1))[0];
	dwFunc += (DWORD)(lpFunc+5);

	PointMin = dwFunc;
	PointMax = dwFunc;

	cnt = 0;
	while(1) {
		if ( lpFunc[cnt]!=JmpCode ) break;
		dwFunc = ((DWORD *)(lpFunc+cnt+1))[0];
		dwCode = (DWORD)(lpFunc+cnt);
		dwCode += dwFunc+5;

		if ( dwCode>PointMax ) PointMax = dwCode;
		if ( dwCode<PointMin ) PointMin = dwCode;
		cnt-=5;
	}

	cnt = 0;
	while(1) {
		if ( lpFunc[cnt]!=JmpCode ) break;
		dwFunc = ((DWORD *)(lpFunc+cnt+1))[0];
		dwCode = (DWORD)(lpFunc+cnt);
		dwCode += dwFunc+5;

		if ( dwCode>PointMax ) PointMax = dwCode;
		if ( dwCode<PointMin ) PointMin = dwCode;
		cnt+=5;
	}
*/

	//DRZ_EDIT (remove for prevent crashing)
	/*
	dwCode = 0;
	dwLen = (PointMax-PointMin)>>2;
	if ( dwLen>200000 ) dwLen = 200000;

	lpdwFunc = (DWORD *)PointMin;

	dwStartFuncPoint = PointMin^(dwLen<<8);
	dwFuncLen = dwLen;

	for(dwCnt=0;dwCnt<dwLen;dwCnt++) 
	{
		dwCode = dwCode+(lpdwFunc[dwCnt]*((dwCnt&0x3)+1));
	}
	*/
/*
	FILE *fp;
	char szBuff[256];
	fp=fopen("memcode.txt","wb");
	if ( fp ) {
		wsprintf( szBuff , "%d %d ( %d ) (Code: %d ) ( %d )",PointMin , PointMax , dwStartFunc ,  dwCode , (DWORD)GetSafeCode );
		fwrite( szBuff , lstrlen(szBuff),1,fp );
		fclose(fp);
	}
*/

	//DRZ_EDIT (manuelly set dwCode for prevent crashing)
	dwCode = 1697348954;

	dwFuncChkTime = GetCurrentTime();
	DWORD FnCode = dwCode ^ ((dwFuncChkTime << 16) | (dwFuncChkTime & 0xFFFF));

	dwFuncSafeCode = FnCode;
	dwFuncSafeMemCount = 0;
	dwFuncSafeMemCode = 0;

	Code_VramSave( FnCode, dwFuncChkTime , dwCode );			//VRAM ¿¡ ÀúÀå

	lpFunc = 0;
	lpdwFunc = 0;	

	return dwCode;
}

//ÄÚµåº¸È£2
DWORD Check_CodeSafe2()
{
	//DWORD	dwCode,dwCnt;
	DWORD	*lpdwFunc;
	DWORD	dwCode;
	int cnt;

	lpdwFunc = (DWORD *)(dwStartFuncPoint^(dwFuncLen<<8));

	if ( dwFuncLen>200000 ) 
		return FALSE;

	for( cnt=0;cnt<32;cnt++) 
	{
	//for( cnt=0;cnt<7;cnt++) {

		dwFuncSafeMemCode = dwFuncSafeMemCode+(lpdwFunc[dwFuncSafeMemCount++] * ((dwFuncSafeMemCount&0x3)+1));

		if ( dwFuncSafeMemCount>=dwFuncLen ) 
		{
			dwFuncChkTime = GetCurrentTime();
			dwCode = dwFuncSafeMemCode ^ ( (dwFuncChkTime<<16)|(dwFuncChkTime&0xFFFF) );
			dwFuncSafeCode = dwCode;

			Code_VramSave( dwCode, dwFuncChkTime , dwFuncSafeMemCode );			//VRAM ¿¡ ÀúÀå

			/*
			FILE *fp;
			char szBuff[256];
			fp=fopen("memcode2.txt","wb");
			if ( fp ) {
				wsprintf( szBuff , "%d %d ( %d ) (Code: %d ) ( %d )",(DWORD)lpdwFunc , dwFuncLen , 0 ,  dwFuncSafeMemCode , (DWORD)GetSafeCode );
				fwrite( szBuff , lstrlen(szBuff),1,fp );
				fclose(fp);
			}
			*/

			dwFuncSafeMemCount = 0;
			dwFuncSafeMemCode = 0;
			lpdwFunc = 0;

			return TRUE;
		}
	}
	
	lpdwFunc = 0;
	return FALSE;
}


//º¸È£ ÄÚµå ±¸ÇÏ±â
DWORD	GetSafeCode()
{
	return dwFuncSafeCode ^ ( (dwFuncChkTime<<16)|(dwFuncChkTime&0xFFFF) );
}

//º¸È£ ÄÚµå ±¸ÇÏ±â
DWORD	GetSafeCode2()
{
	DWORD	dwCode , dwFnChkTime , dwFnSafeCode;
	DWORD	dwOriginal;

	Code_VramLoad( &dwCode, &dwFnChkTime , &dwFnSafeCode );			//VRAM ¿¡ ·Îµù

	dwOriginal = dwCode ^ ( (dwFnChkTime<<16)|(dwFnChkTime&0xFFFF) );

	if ( (dwOriginal^dwFnSafeCode)==0 ) return dwOriginal;
	return FALSE;
}

//ÀÚÁÖ¾²´Â ¸Þ½Ã ·Îµå
int	LoadOftenMeshPattern( char *szFileName )
{

	DynTempPat[DynTempPatCnt] = smPattern.LoadCharactor( szFileName );
	if ( DynTempPat[DynTempPatCnt] )
	{
		DynTempPat[DynTempPatCnt]->UseCount = 99;
		DynTempPatCnt++;
		return TRUE;
	}

	return FALSE;
}

#ifdef _CHECK_MESH
//ÀÚÁÖ¾²´Â ¸Þ½Ã ·Îµå ( ¾ÆÀÌÅÛ¸Þ½¬ Á¶ÀÛ°Ë»ç ±â´É Ãß°¡ )
static DWORD dwOftPatSum = 0;

int	LoadOftenMeshPatternCheckSum( char *szFileName )
{
	smPAT3D *lpPat3D;

	DynTempPat[DynTempPatCnt] = smPattern.LoadCharactor( szFileName );
	if ( DynTempPat[DynTempPatCnt] ) {
		lpPat3D = DynTempPat[DynTempPatCnt]->Pat;
		int szX=0;int szN=0;
		for( int cnt=0;cnt<lpPat3D->nObj3d;cnt++) {
			if ( szX<lpPat3D->obj3d[cnt]->maxY ) 
				szX=lpPat3D->obj3d[cnt]->maxY;
			if ( szN<lpPat3D->obj3d[cnt]->minY ) 
				szN=lpPat3D->obj3d[cnt]->minY;
		}
		DynTempPat[DynTempPatCnt]->UseCount = 99;
		DynTempPatCnt++;

		DWORD dwSpdSum = GetSpeedSum(szFileName);
		DWORD dwSizSum = (szX<<4) | (szN<<1) ;
		dwOftPatSum += dwSpdSum + (dwSpdSum * dwSizSum ) + dwSizSum;
		return TRUE;
	}

	return FALSE;
}

//¾ÆÀÌÅÛ¸Þ½¬ Á¶ÀÛ°Ë»ç °á°ú°ª Àü¼Û
int Send_OftPatSumToDataServer()
{

	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = smTRANSCODE_CHECK_ITEMMESH;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = dwOftPatSum;
	smTransCommand.SParam = 0;
	smTransCommand.LParam = 0;
	smTransCommand.EParam = 0;

	dwOftPatSum = 0;

	if ( smWsockDataServer ) 
		return smWsockDataServer->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return FALSE;
}
#endif

int	LoadOftenFullPattern()
{
	int cnt,cnt2;
	int	ErrorCount;
	char	szBuff[128];

	ErrorCount = 0;

	for(cnt=0;cnt<8;cnt++) {
		for(cnt2=0;cnt2<ARMOR_FILE_MAX;cnt2++) {
			lstrcpy( szBuff , szArmorDirectory );
			lstrcat( szBuff, szArmorNumFile[cnt][cnt2] );

			if ( LoadOftenMeshPattern( szBuff )==FALSE ) {
				ErrorCount++;
			}
		}
	}

	for(cnt=0;cnt<8;cnt++) {
		for(cnt2=0;cnt2<ARMOR_FILE_MAX;cnt2++) {
			lstrcpy( szBuff , szArmorDirectory );
			lstrcat( szBuff, szArmorNumFile_Comic[cnt][cnt2] );

			if ( LoadOftenMeshPattern( szBuff )==FALSE ) {
				ErrorCount++;
			}
		}
	}


	for(cnt=0;cnt<FACE_MODEL_MAX;cnt++) {
		for(cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++) {		//0,3 ¸Ó¸® ¹Ì¸® ¿Ã¸²
			if ( LoadOftenMeshPattern( szModel_FighterFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_MechFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_ArcherFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_PikeFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_KnightFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_AtalantaFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_PristessFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_Magican[cnt][cnt2] )==FALSE )
				ErrorCount++;
		}
	}




	//////////////////// NPC Æú´õ ////////////////////
	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	char	szFindPath[64];
	char	szFilePath[64];

	lstrcpy( szFindPath , szNpcDirectory );
	lstrcat( szFindPath , "*.ini" );

	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
		while(1) {
			lstrcpy( szFilePath , szNpcDirectory );
			lstrcat( szFilePath, fd.cFileName );

			LoadOftenMeshPattern(szFilePath);

			//´ÙÀ½ ÆÄÀÏ Ã£À½
			if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		}
	}
	FindClose( hFindHandle );


	return ErrorCount;
}



//ÀÚÁÖ»ç¿ëÇÏ´Â ¸ðµ¨µé ·Îµå
int LoadOftenModels()
{

	return TRUE;
}

char *szOftenNPC[] = {
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
int LoadFolder_Ini( char *szPath , char *szWildCard , char *szChgExt , int OftSave );

int Make_CharAll_SMD()
{
	LoadOftenFullPattern();
	LoadFolder_Ini( "Char\\Npc" , "*.ini" , 0, 0 );
	LoadFolder_Ini( "Char\\Monster" , "*.ini" , 0, 0 );

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
	LoadFolder_Ini( "Char\\Npc" , "*.inx" , "ini" , TRUE );

#else

	int cnt;

	cnt=0;
	while(1) {
		if ( !szOftenNPC[cnt] ) break;
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
	int cnt,cnt2;
	int ErrorCount = 0;

	for(cnt=0;cnt<6;cnt++) {
		//for(cnt2=0;cnt2<3;cnt2++) {					//0,3 ¸Ó¸® ¹Ì¸® ¿Ã¸²
		for(cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++) {		//0,3 ¸Ó¸® ¹Ì¸® ¿Ã¸²
 			if ( LoadOftenMeshPattern( szModel_FighterFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_MechFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_ArcherFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_PikeFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_KnightFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_AtalantaFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_PristessFaceName[cnt][cnt2] )==FALSE )
				ErrorCount++;
			if ( LoadOftenMeshPattern( szModel_Magican[cnt][cnt2] )==FALSE )
				ErrorCount++;
		}
	}

	if ( ErrorCount ) 
		return FALSE;

	return TRUE;

}


//ÀÚÁÖ¾²´Â °©¿Ê ¹Ì¸® ·Îµù
int LoadOftenArmor()
{
	int cntW,cntH;
	char szName[128];

	for( cntH=0;cntH<MAX_JOBNUM; cntH++ ) {
		for( cntW=0;cntW<18; cntW++ ) {
			if ( cntW==0 || cntW>=10 ) {
				lstrcpy( szName , szArmorDirectory );
				lstrcat( szName , szArmorNumFile[cntH][cntW] );
				LoadOftenMeshPattern( szName  );
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

	for( cnt=0 ; cnt< MAX_ITEM ; cnt++){
		if( sItem[cnt].DorpItem == NULL ) continue;
		wsprintf(szBuff ,"Image\\sinImage\\Items\\DropItem\\it%s.ASE",sItem[cnt].DorpItem);

#ifdef _CHECK_MESH
		LoadOftenMeshPatternCheckSum( szBuff  );			//ÀÚÁÖ¾²´Â ¸Þ½Ã ·Îµå ( ¾ÆÀÌÅÛ¸Þ½¬ Á¶ÀÛ°Ë»ç ±â´É Ãß°¡ )
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
		LoadOftenMeshPattern( szBuff  );
#endif
		if ( sItem[cnt].CODE==(sinGG1|sin01) ) break;		//µ· ±×¸²ÀÌ ¸¶Áö¸·
	}

#ifdef _CHECK_MESH
	Send_OftPatSumToDataServer();							//¾ÆÀÌÅÛ¸Þ½¬ Á¶ÀÛ°Ë»ç °á°ú°ª Àü¼Û
#endif

	return TRUE;	
}

//ÀÚÁÖ¾µ ¸Þ½Ã ÅØ½ºÃÄ ½º¿ÒÃ½Å©
int CheckOftenMeshTextureSwap()
{

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
    return TRUE;
	//######################################################################################

	int cnt;

	for( cnt=0;cnt<DynTempPatCnt;cnt++ ) {
		if ( DynTempPat[cnt] && DynTempPat[cnt]->Pat && DynTempPat[cnt]->Pat->smMaterialGroup ) {
			DynTempPat[cnt]->Pat->smMaterialGroup->CheckMatreialTextureSwap();
		}
	}
	return TRUE;
}


//°©¿Ê ¸ð¾ç ¹Ù²Ù±â
int SetPlayerArmor( int armor )
{
	char szbuff[256];
	int jobNum;

	if ( PlayerArmor==armor ) return FALSE;
	if ( lpCurPlayer->PatLoading ) return FALSE;
	if ( !lpCurPlayer->smCharInfo.szModelName2[0] ) return FALSE;		//¸Ó¸® ¾ø´Â Ä³¸¯ÅÍ °ª¿Ê ¼³Á¤ ±ÝÁö // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	if ( armor == -1) armor=0; 
	
	jobNum = lpCurPlayer->smCharInfo.JOB_CODE;
	if ( armor>ARMOR_FILE_MAX-1 ) armor=ARMOR_FILE_MAX-1;
	if ( jobNum>MAX_JOBNUM ) jobNum=0;

	lstrcpy( szbuff, szArmorDirectory );

	if ( lpCurPlayer->smCharInfo.SizeLevel>0x1000 && lpCurPlayer->smCharInfo.SizeLevel<=0x1002 )
		lstrcat( szbuff, szArmorNumFile_Comic[ArmorJobNum[jobNum]][ armor ] );
	else
		lstrcat( szbuff, szArmorNumFile[ArmorJobNum[jobNum]][ armor ] );

	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szbuff , lpCurPlayer->smCharInfo.szModelName2 );

	PlayerArmor = armor;

	return TRUE;
}

//°©¿Ê ¸ð¾ç ¹Ù²Ù±â
int SetChrArmor( smCHAR *lpChar , int ItemNum )
{
	char szbuff[256];
	int armor;
	int jobNum;

//	if ( lpChar->PlayerArmor==armor ) return FALSE;
//	if ( lpCurPlayer->PatLoading ) return FALSE;

	if ( !lpChar->DisplayFlag || lpChar->wStickItems[smPLAYBUFF_ITEM_BODY]==ItemNum ) return FALSE;
	if ( lpChar->PatLoading ) return FALSE; 
	if ( !lpChar->smCharInfo.szModelName2[0] ) return FALSE;		//¸Ó¸® ¾ø´Â Ä³¸¯ÅÍ °ª¿Ê ¼³Á¤ ±ÝÁö // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤

	if ( ItemNum>=0 ){
		//ÀÛ¼ºÀÚ ÇÏ ´ë ¿ë
		//¾ÆÀÌÅÛ ÄÚµå ¼öÁ¤(sin25 ÀÌÈÄ¿£ -16À» ÇØÁØ´Ù.)---------------------------------
		if(((sItem[ItemNum].CODE&sinITEM_MASK3)>>8) > 25 )
			armor = (((sItem[ItemNum].CODE&sinITEM_MASK3)>>8)-17);			
		else
			armor = (sItem[ItemNum].CODE&sinITEM_MASK3)>>8;			
	}
	else
		armor = 0;

	jobNum = lpChar->smCharInfo.JOB_CODE;
	if ( armor>ARMOR_FILE_MAX-1 ) armor=ARMOR_FILE_MAX-1;
	if ( jobNum>MAX_JOBNUM ) jobNum=0;

	lstrcpy( szbuff, szArmorDirectory );

	if ( lpChar->smCharInfo.SizeLevel>0x1000 && lpChar->smCharInfo.SizeLevel<=0x1002 )
		lstrcat( szbuff, szArmorNumFile_Comic[ArmorJobNum[jobNum]][ armor ] );
	else
		lstrcat( szbuff, szArmorNumFile[ArmorJobNum[jobNum]][ armor ] );

	lpChar->PatLoading = TRUE;
	AddLoaderPattern( lpChar , szbuff , lpChar->smCharInfo.szModelName2 );

	return TRUE;
}

//¾ó±¼ º¹±¸¿ë ÄÚµå »ý¼º // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
DWORD GetHairSkinCode( int FaceCode , int HairCode )
{
	return ((FaceCode&0xFF)<<8) | (HairCode&0xFF) | ((((FaceCode+1)+(HairCode+1)*3)&0xFF)<<16);
}

//¾ó±¼ º¹±¸ ÄÚµå Á¤»óÀÎÁö È®ÀÎ // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
int CheckHairSkinCode( DWORD dwModelCode )
{
	if ( dwModelCode ) 
	{
		int FaceCode = (dwModelCode>>8)&0xFF;
		int HairCode = dwModelCode&0xFF;
		int Sum = (dwModelCode>>16)&0xFF;
		if ( (((FaceCode+1)+(HairCode+1)*3)&0xFF)==Sum ) return TRUE;
	}
	return FALSE;
}

//¾ó±¼ º¹±¸ // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
char *GetFaceHairSkin( smCHAR *lpChar )
{

	DWORD dwModelCode = lpChar->smCharInfo.ModelNameCode2;
	int FaceCode = (dwModelCode>>8)&0xFF;
	int HairCode = dwModelCode&0xFF;

	if ( CheckHairSkinCode(dwModelCode)==FALSE ) return NULL;

	char *szHeadName = 0;

	switch( lpChar->smCharInfo.JOB_CODE ) {
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
			szHeadName = szModel_Magican[FaceCode][HairCode];
			break;
		case 8:		//ÇÁ¸®½ºÆ¼½º
			szHeadName = szModel_PristessFaceName[FaceCode][HairCode];
			break;
	}

	return szHeadName;
}

// ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
int FindFaceSkin( smCHAR *lpChar , char *lpHeadName , char ***lpSelFace , char **lpSelFaceName )
{

	int cnt,cnt2;
	int FindCount = 0;
	int FaceCode;
	int HairCode;

	switch( lpChar->smCharInfo.JOB_CODE ) {
		case 1:		//ÆÄÀÌÅÍ
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_FighterFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_FighterFaceName[cnt];
						*lpSelFaceName = szModel_FighterFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
		case 2:		//¸ÞÄ«´Ï¼Ç
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_MechFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_MechFaceName[cnt];
						*lpSelFaceName = szModel_MechFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
		case 3:		//¾ÆÃÄ
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_ArcherFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_ArcherFaceName[cnt];
						*lpSelFaceName = szModel_ArcherFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
		case 4:		//ÆÄÀÌÅ©¸Ç
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_PikeFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_PikeFaceName[cnt];
						*lpSelFaceName = szModel_PikeFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;

			}
			break;
		case 5:		//¾ÆÅ»¶õÅ¸
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_AtalantaFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_AtalantaFaceName[cnt];
						*lpSelFaceName = szModel_AtalantaFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
		case 6:		//³ªÀÌÆ®
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_KnightFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_KnightFaceName[cnt];
						*lpSelFaceName = szModel_KnightFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
		case 7:		//¸ÅÁö¼Ç
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_Magican[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_Magican[cnt];
						*lpSelFaceName = szModel_Magican[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
		case 8:		//ÇÁ¸®½ºÆ¼½º
			for( cnt=0;cnt<FACE_MODEL_MAX;cnt++ ) {
				for( cnt2=0;cnt2<HEAREPOTION_MAX;cnt2++ ) {
					if ( lstrcmpi(szModel_PristessFaceName[cnt][cnt2] , lpHeadName )==0 ) 
					{
						*lpSelFace = szModel_PristessFaceName[cnt];
						*lpSelFaceName = szModel_PristessFaceName[cnt][cnt2];
						FaceCode = cnt; HairCode= cnt2 ; FindCount++;
						break;
					}
				}
				if ( cnt2<HEAREPOTION_MAX ) break;
			}
			break;
	}

	if ( !FindCount ) return -1;


	return (FaceCode<<8)|HairCode;

}

//¸ðµ¨ ½ºÅ² º¯°æ
int	ChangeModelSkin( smCHAR *lpChar , int ComicArmor , int ComicHeadNum , char *szArmor , char *szHead )
{
	char szArmorName[32];
	char szHeadName[32];
	char szbuff1[256];
	char szbuff2[256];

	int len,cnt; // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	int jobNum;
	int SelCount;
	int FindArmor , FindHead;
	char *lpArmorFile;
	char *lpHeadFile;

	// ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	char **lpSelFace = 0;
	char *lpSelFaceName = 0;

	jobNum = lpChar->smCharInfo.JOB_CODE;
	if ( jobNum>MAX_JOBNUM ) jobNum=0;

	if ( szArmor ) lpArmorFile = szArmor;
	else lpArmorFile = lpChar->smCharInfo.szModelName;

	if ( szHead ) lpHeadFile = szHead;
	else lpHeadFile = lpChar->smCharInfo.szModelName2;

	szArmorName[0] = 0;
	szHeadName[0] = 0;

	//¿øº» ¸ðµ¨ ÃßÃâ
	len = lstrlen( lpArmorFile );
	for( cnt=len-1;cnt>=0;cnt-- ) {
		if ( lpArmorFile[cnt]=='\\' ) {
			lstrcpy( szArmorName , &lpArmorFile[cnt+1] );
			break;
		}
	}
/*
	len = lstrlen( lpHeadFile );
	for( cnt=len-1;cnt>=0;cnt-- ) {
		if ( lpHeadFile[cnt]=='\\' ) {
			lstrcpy( szHeadName , &lpHeadFile[cnt+1] );
			break;
		}
	}
*/
	lstrcpy( szHeadName , lpHeadFile );


	if ( !szArmorName[0] || !szHeadName[0] ) return FALSE;


	SelCount = ComicHeadNum;

	FindArmor = 0;
	FindHead = 0;

	for( cnt=0;cnt<ARMOR_FILE_MAX;cnt++ ) {
		if ( lstrcmpi( szArmorName, szArmorNumFile[ArmorJobNum[jobNum]][cnt] )==0 ) {
			if ( ComicArmor )
				lstrcpy( szArmorName , szArmorNumFile_Comic[ArmorJobNum[jobNum]][cnt] );
			else
				lstrcpy( szArmorName , szArmorNumFile[ArmorJobNum[jobNum]][cnt] );

			FindArmor++;
			break;
		}
	}
	if ( !FindArmor ) {
		for( cnt=0;cnt<ARMOR_FILE_MAX;cnt++ ) {
			if ( lstrcmpi( szArmorName, szArmorNumFile_Comic[ArmorJobNum[jobNum]][cnt] )==0 ) {
				if ( ComicArmor )
					lstrcpy( szArmorName , szArmorNumFile_Comic[ArmorJobNum[jobNum]][cnt] );
				else
					lstrcpy( szArmorName , szArmorNumFile[ArmorJobNum[jobNum]][cnt] );

				FindArmor++;
				break;
			}
		}
	}

	// ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	int FindFaceValue = FindFaceSkin( lpChar , szHeadName , &lpSelFace , &lpSelFaceName );
	if ( FindFaceValue<0 ) return FALSE;
	int HairCode = FindFaceValue&0xFF;
	int FaceCode = FindFaceValue>>8;

	//º¹±¸¿ë ¾ó±¼ ÄÚµå ÃÖÃÊ »ý¼º
	if ( CheckHairSkinCode(lpChar->smCharInfo.ModelNameCode2)==FALSE ) {
		lpChar->smCharInfo.ModelNameCode2 = GetHairSkinCode( FaceCode , HairCode );
	}

	switch( jobNum ) 
	{
		case 1:		//ÆÄÀÌÅÍ
			lstrcpy( szHeadName , szModel_FighterFaceName[SelCount][HairCode] );
			break;
		case 2:		//¸ÞÄ«´Ï¼Ç
			lstrcpy( szHeadName , szModel_MechFaceName[SelCount][HairCode] );
			break;
		case 3:		//¾ÆÃÄ
			lstrcpy( szHeadName , szModel_ArcherFaceName[SelCount][HairCode] );
			break;
		case 4:		//ÆÄÀÌÅ©¸Ç
			lstrcpy( szHeadName , szModel_PikeFaceName[SelCount][HairCode] );
			break;
		case 5:		//¾ÆÅ»¶õÅ¸
			lstrcpy( szHeadName , szModel_AtalantaFaceName[SelCount][HairCode] );
			break;
		case 6:		//³ªÀÌÆ®
			lstrcpy( szHeadName , szModel_KnightFaceName[SelCount][HairCode] );
			break;
		case 7:		//¸ÅÁö¼Ç
			lstrcpy( szHeadName , szModel_Magican[SelCount][HairCode] );
			break;
		case 8:		//ÇÁ¸®½ºÆ¼½º
			lstrcpy( szHeadName , szModel_PristessFaceName[SelCount][HairCode] );
			break;
	}


	lstrcpy( szbuff1, szArmorDirectory );
	lstrcat( szbuff1, szArmorName );
	lstrcpy( szbuff2, szHeadName );

	lpChar->PatLoading = TRUE;
	AddLoaderPattern( lpChar , szbuff1 , szbuff2 );

	return TRUE;
}


//¸Ó¸®¸ð¾ç ¹Ù²ã¹ö¸®±â  // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
int ChangeHairModel( int HairCode )
{

	smCHAR *lpChar = lpCurPlayer;
	int jobNum = lpChar->smCharInfo.JOB_CODE;
	char *lpHeadName = lpChar->smCharInfo.szModelName2;
	char **lpSelFace = 0;
	char *lpSelFaceName = 0;

	if ( HairCode<0 || HairCode >= HEAREPOTION_MAX ) return FALSE;

	int FindFaceValue = FindFaceSkin( lpChar , lpHeadName , &lpSelFace , &lpSelFaceName );

	if ( FindFaceValue<0 || lpSelFace==0 ) return FALSE;

	int FaceCode = FindFaceValue>>8;

	if ( lstrcmpi( lpChar->smCharInfo.szModelName2 , lpSelFace[HairCode] ) == 0 ) 
		return FALSE;

	lstrcpy( lpChar->smCharInfo.szModelName2 , lpSelFace[HairCode] );
	lpChar->smCharInfo.ModelNameCode2 = GetHairSkinCode( FaceCode , HairCode);		//¸Ó¸®º¯°æ ÄÚµå ÀúÀå (³ªÁß¿¡ º¹±¸ÇÏ±â À§ÇØ¼­....)

	lpChar->PatLoading = TRUE;
	AddLoaderPattern( lpChar , lpChar->smCharInfo.szModelName , lpChar->smCharInfo.szModelName2 );

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();
	SendSetObjectSerial( lpCurPlayer->dwObjectSerial );
	SaveGameData();


 

	return TRUE;
}


/*
static short	AgingBlinkColor[8][4] = {
	{ 100,	 60,	 90,	0	},		//¾àÇÑº¸¶ó
	{ 140,	120,	  0,	0	},		//³ë¶û
	{	0,	130,	 40,	0	},		//ÃÊ·Ï
	{	0,	 80,	180,	0	},		//ÆÄ¶û
	{ 100,	  0,	160,	0	},		//º¸¶ó
	{ 200,	 80,	  0,	0	},		//ÁÖÈ²
	{ 180,	 20,	  0,	0	},		//»¡°­
	{ 255,	 80,	 30,	0	}		//¹àÀº»¡°­ee
};
*/
/*----------------------------------------------------------------------------*
*Desc: ¿¡ÀÌÂ¡ Â÷¼ö¿¡ µû¸¥ ¹«±â ÄÃ·¯°ª  		
*-----------------------------------------------------------------------------*/
const int  CONST_AGINGBCOLOR_MAX = 22;

static short	AgingBlinkColor[CONST_AGINGBCOLOR_MAX][6] =
{
	{ 100,	 40,	 90,	0,	-1,	0			},		//¾àÇÑº¸¶ó	
	{ 120,	100,	 10,	0,	-1,	0			},		//³ë¶û
	{   0,	110,	 30,	0,	-1,	0			},		//ÃÊ·Ï
	{   0,	 50,	140,	0,	 0,	smTEXSTATE_FS_SCROLL5	},		//ÆÄ¶û
	{ 100,	  0,	 90,	0,	 1,	smTEXSTATE_FS_SCROLL6	},		//º¸¶ó
	{ 150,	 60,	  0,	0,	 2,	smTEXSTATE_FS_SCROLL6	},		//ÁÖÈ²
	{ 150,	 10,	  0,	0,	 3,	smTEXSTATE_FS_SCROLL8	},		//»¡°­
	{  20,	220,	190,	0,	 4,	smTEXSTATE_FS_SCROLL8	},		//Ã»·Ï
	{  10,	220,	 30,	0,	 4,	smTEXSTATE_FS_SCROLL8	},		//¿¬µÎ
	{ 170,	 40,	170,	0,	 4,	smTEXSTATE_FS_SCROLL9	},		//º¸¶ó
	{ 250,	 30,	160,	0,	 4,	smTEXSTATE_FS_SCROLL9	},		//ÁøÇÑºÐÈ«
	{  30,	190,	255,	0,	 4,	smTEXSTATE_FS_SCROLL9	},		//ÇÏ´Ã
	{ 250,	130,	 30,	0,	 4,	smTEXSTATE_FS_SCROLL9	},		//¿¬ÇÑÁÖÈ²
	{ 120,	 30,	 30,	0,	 5,	smTEXSTATE_FS_SCROLL10	},		//°íµ¿
	{ 130,	  0,	255,	0,	 5,	smTEXSTATE_FS_SCROLL10	},		//Ã»º¸¶ó
	{ 220,	240,	 70,	0,	 5,	smTEXSTATE_FS_SCROLL10	},		//¿¬ÇÑ³ë¶û
	{ 240,	240,	240,	0,	 5,	smTEXSTATE_FS_SCROLL10	},
	{ 153, 237, 218, 0, 5, smTEXSTATE_FS_SCROLL10 },
	{ 121, 176, 238, 0, 5, smTEXSTATE_FS_SCROLL10 },
};

static short	CraftBlinkColor[6][6] =
{
	{   13,	  0,	  5,	0,	 9,	smTEXSTATE_FS_SCROLL4	},		//Å¬·¡½Ä
	{	13,	  0,	  6,	0,	 5,	smTEXSTATE_FS_SCROLL4	},		//Æ®·£
	{	13,	  0,	  7,	0,	 6,	smTEXSTATE_FS_SCROLL5	},		//¸ÓÅ°
	{	13,	  0,	  8,	0,	 7,	smTEXSTATE_FS_SCROLL5	},		//µ¥ºñ
	{	13,	  0,	  9,	0,	 8,	smTEXSTATE_FS_SCROLL5	},		//¼¿·¹
	{   13,	  0,	  10,	0,	 9,	smTEXSTATE_FS_SCROLL5	}		//???
};


//»ö»óÀ¸·Î ¾ÆÀÌÅÛÀÇ »óÅÂ¸¦ ÆÇº°ÇÑ´Ù
int GetItemKindFromBliankColor( smCHARTOOL	*lpCharTool )
{
	int cnt;

	lpCharTool->TexMixCode = -1;
	lpCharTool->TexScroll  = 0;

	for( cnt=0;cnt<CONST_AGINGBCOLOR_MAX;cnt++ ) {
		if ( AgingBlinkColor[cnt][0]==lpCharTool->sColors[SMC_R] && 
			AgingBlinkColor[cnt][1]==lpCharTool->sColors[SMC_G] && 
			AgingBlinkColor[cnt][2]==lpCharTool->sColors[SMC_B] && 
			AgingBlinkColor[cnt][3]==lpCharTool->sColors[SMC_A] ) {

			lpCharTool->TexMixCode = AgingBlinkColor[cnt][4];
			lpCharTool->TexScroll  = AgingBlinkColor[cnt][5];
			return TRUE;
		}
	}

	for( cnt=0;cnt<6;cnt++ ) {
		if ( CraftBlinkColor[cnt][0]==lpCharTool->sColors[SMC_R] && 
			CraftBlinkColor[cnt][1]==lpCharTool->sColors[SMC_G] && 
			CraftBlinkColor[cnt][2]==lpCharTool->sColors[SMC_B] && 
			CraftBlinkColor[cnt][3]==lpCharTool->sColors[SMC_A] ) {

			lpCharTool->TexMixCode = CraftBlinkColor[cnt][4];
			lpCharTool->TexScroll  = CraftBlinkColor[cnt][5];
			return TRUE;
		}
	}


	return NULL;

}

/////////////// ½Å¹Ù¶÷ ÀÎº¥Åä¸® º¯°æ È£Ãâ ÇÔ¼ö ///////////////
int sinSetCharItem(DWORD CODE, DWORD Posi, BOOL SetFlag) { return TRUE; }

//¾ÆÀÌÅÛ ¹ö¸®±â ÇÔ¼ö
int sinThrowItemToFeild(sITEM* pItem) { return TRUE; }

//¹°¾à ¸Ô±â
int sinActionPotion()
{
	if ( sinGetLife()>0 && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_EAT 
		&& lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_DEAD ) {
		if ( lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_ATTACK || lpCurPlayer->MotionInfo->State==CHRMOTION_STATE_SKILL ) 
			lpCurPlayer->dwNextMotionCode = CHRMOTION_STATE_EAT;
		else
			lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_EAT );
		return TRUE;
	}

	return FALSE;
}

//±ÍÈ¯¾ÆÀÌÅÛ »ç¿ë
int	ActionEtherCore( sITEM	*lpItem ) { return TRUE; }


//Á×´Â°æ¿ì ÆÐ³ÎÆ¼ °ª
int GetDeathPenalty() 
{
	sITEMINFO	*lpItemInfo;

	int	Penalty = 0;

	if(sInven[4].ItemIndex){
		lpItemInfo = &cInvenTory.InvenItem[sInven[4].ItemIndex-1].sItemInfo;
		if( lpItemInfo->CODE==(sinOR2|sin01) && lpItemInfo->ItemKindCode==ITEM_KIND_AGING ) {
			if ( lpItemInfo->ItemAgingNum[0]>=Penalty ) Penalty=lpItemInfo->ItemAgingNum[0];
		}
	}

	if(sInven[5].ItemIndex){
		lpItemInfo = &cInvenTory.InvenItem[sInven[5].ItemIndex-1].sItemInfo;
		if( lpItemInfo->CODE==(sinOR2|sin01) && lpItemInfo->ItemKindCode==ITEM_KIND_AGING ) {
			if ( lpItemInfo->ItemAgingNum[0]>=Penalty ) Penalty=lpItemInfo->ItemAgingNum[0];
		}
	}

	//°ø¼ºÇÊµå °æÇèÄ¡ ÇÏ¶ô ¾øÀ½
	if ( lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->State==FIELD_STATE_CASTLE &&
		StageField[lpCurPlayer->OnStageField]->FieldCode==rsCASTLE_FIELD )
	{
		return 0;
	}

	//ºÎÈ° ÁÖ¹®¼­
	lpItemInfo = FindItemFromCode(sinBI1|sin04);
	if ( lpItemInfo ) {
		SendUseItemCodeToServer( lpItemInfo );
		cInvenTory.DeleteInvenItemToServer(lpItemInfo->CODE  , lpItemInfo->ItemHeader.Head , lpItemInfo->ItemHeader.dwChkSum );
		PlayUsed_Resurrection ++;
		return 0;								
	}

#ifndef _DEATH_PANALTY
	if ( sBiInfo && (sBiInfo->EventPlay[0]&BIMASK_VIP_USER)!=0 ) return 100;			//È°ÀÎ VIP PC¹æ IP
#endif

	//ÅÂ±¹¼öÁ¤
#ifdef _LANGUAGE_THAI_RING
	if ( Penalty>=8 ) return 1000;														//ÁöÁ¸¹ÝÁö
	if ( Penalty>=7 ) return 700;
	if ( Penalty>=6 ) return 600;
	if ( Penalty>=5 ) return 500;														//ÁöÁ¸¹ÝÁö
	if ( Penalty>=4 ) return 400;
	if ( Penalty>=3 ) return 300;														//ÁöÁ¸¹ÝÁö
	if ( Penalty>=2 ) return 200;
	if ( Penalty>=1 ) return 100;
#endif
	if ( Penalty>=8 ) return 100;														//ÁöÁ¸¹ÝÁö
	if ( Penalty>=6 ) return 50;

	if ( (sinQuest_levelLog&QUESTBIT_LEVEL_85)!=0 && lpCurPlayer->smCharInfo.Level>=85 ) {
		//90·¹º§ Äù½ºÆ® ¿Ï·á
#ifdef _DEATH_PANALTY //µ¥¾²Æä³ÎÆ¼ ÀÌº¥Æ®½Ã 0.5%
	return 200;
#endif
		return 50;
	}
#ifdef _DEATH_PANALTY //µ¥¾²Æä³ÎÆ¼ ÀÌº¥Æ®½Ã 1.0%
	return 110;
#endif


	return 33;
}


INT64 ExpLevelTable[] = {
#include "..\\Shared\LevelTable.h"
	-1 };

INT64 LevelTableSum = 30649;


INT64 CodeXorCharInfo_Exp()
{
	INT64 exp1,exp2;

	lpCurPlayer->smCharInfo.Exp ^= CodeXorExp;
	lpCurPlayer->smCharInfo.Exp_High ^= CodeXorExp_High;

	exp1 = lpCurPlayer->smCharInfo.Exp;
	exp2 = lpCurPlayer->smCharInfo.Exp_High;

	return (exp2<<32)|(exp1&0xFFFFFFFF);
}


//°æÇèÄ¡ Å×ÀÌºí Ã½Å©
INT64 CheckLevelTable()
{
	INT64 cnt = 0;
	INT64 ChkSum = 0;

	while(1) {
		if (ExpLevelTable[cnt]==-1) break;
		ChkSum += ExpLevelTable[cnt]*(cnt+1);
		cnt++;
	}

	return ChkSum;
}

//·¹º§°ú °æÇèÄ¡°¡ ¸Â´ÂÁö È®ÀÎ
int	CheckLevelExp( int Level , INT64 Exp )
{
	INT64 xp;
	INT64 np;

	if ( Level>0 && Level<=CHAR_LEVEL_MAX ) {
		xp = ExpLevelTable[Level-1];
		np = ExpLevelTable[Level];

		if ( Exp>=xp && Exp<=np ) {
			return TRUE;
		}

	}
	return FALSE;
}

//°æÇèÄ¡·Î ·¹º§ Ãß»ê
int	GetLevelFromExp( INT64 Exp )
{
	int	cnt;

	for(cnt=0;cnt<CHAR_LEVEL_MAX;cnt++) {
		if ( Exp>=ExpLevelTable[cnt] && Exp<ExpLevelTable[cnt+1] ) {
			return cnt+1;
		}
	}

	return NULL;
}

//64ºñÆ® °æÇèÄ¡ ¹ÝÈ¯
INT64 GetExp64( smCHAR_INFO *smCharInfo )
{
	INT64 exp1,exp2;
	exp1 = (INT64)smCharInfo->Exp;

#ifdef _MODE_EXP64
	exp2 = (INT64)smCharInfo->Exp_High;
#else
	exp2 = 0;
#endif

	return (exp2<<32)|(exp1&0xFFFFFFFF);
}

INT64 GetXorExp64( smCHAR_INFO *smCharInfo )
{
	INT64 exp1,exp2;

	exp1 = (INT64)(smCharInfo->Exp^CodeXorExp);
	exp2 = (INT64)(smCharInfo->Exp_High^CodeXorExp_High);

	return (exp2<<32)|(exp1&0xFFFFFFFF);
}

int	SetExp64( smCHAR_INFO *smCharInfo , INT64 Exp64 )
{
#ifdef _MODE_EXP64
	smCharInfo->Exp_High = (int)(Exp64>>32);
#endif
	smCharInfo->Exp = (int)(Exp64&0xFFFFFFFF);

	return smCharInfo->Exp;
}


int AddExp( int Exp )
{
	int Level;
	INT64 iexp;
	INT64 ChkSum;
	INT64 exp64;
	int mlevel;


	exp64 = CodeXorCharInfo_Exp();

	mlevel = CHAR_LEVEL_MAX - 1;

	if (exp64 < ExpLevelTable[mlevel]) exp64 += Exp;


	if (exp64 > ExpLevelTable[99] && lpCurPlayer->smCharInfo.ChangeJob < 3)
	{
		CodeXorCharInfo_Exp();
		return TRUE;
	}

	SetExp64(&lpCurPlayer->smCharInfo, exp64);

	CodeXorCharInfo_Exp();


	ReformCharForm();

	Level = lpCurPlayer->smCharInfo.Level;
	iexp = ExpLevelTable[Level];

	// Verifica se o personagem pode ganhar XP (não pode ultrapassar o nível máximo)
	if (iexp >= 0 && exp64 >= iexp && lpCurPlayer->smCharInfo.Level < CHAR_LEVEL_MAX)
	{
		// Verificação adicional para garantir que não ultrapasse o nível máximo
		if (lpCurPlayer->smCharInfo.Level >= g_LevelFinal)
		{
			// Personagem já atingiu o nível máximo, não ganha mais XP
			return TRUE;
		}

		ChkSum = CheckLevelTable();
		if (LevelTableSum != ChkSum) return TRUE;

		lpCurPlayer->smCharInfo.Level++;
		lpCurPlayer->smCharInfo.Next_Exp = (int)ExpLevelTable[Level + 1];
		ReformCharForm();


		SendPlayUpdateInfo();
		SaveGameData();
	}

	return TRUE;
}

//ÇÊµå Àç½ÃÀÛ °¡´ÉÇÑÁö È®ÀÎ
int CheckFieldContExp()
{
	int Level;
	INT64 LowExp;
	INT64 HighExp;
	INT64 exp;
	INT64 exp64;

	Level = lpCurPlayer->smCharInfo.Level;
	if ( Level>=11 ) {
		LowExp = ExpLevelTable[Level-1];
		HighExp = ExpLevelTable[Level];
		exp = (HighExp - LowExp)/100;			//¾à 1%

		exp64 = GetXorExp64( &lpCurPlayer->smCharInfo );

		exp = exp64 - exp;
		if ( exp<LowExp ) {
			return FALSE;
		}
	}
	else {
		if ( lpCurPlayer->OnStageField>=0 && StageField[lpCurPlayer->OnStageField]->State!=FIELD_STATE_FOREST ) {
			//Àú·¦Àº ½£¿¡¼­¸¸ Àç½ÃÀÛ °¡´É
			return FALSE;
		}
	}

	return TRUE;
}

INT64 GetNextExp( int Level )
{
	return ExpLevelTable[Level];
}

INT64 GetExpFromLevel(int iLevel)
{
	return ExpLevelTable[iLevel - 1];
}

int DeadPlayerExp( smCHAR *lpChar )
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
	if ( Level>=11 ) {
		LowExp = ExpLevelTable[Level-1];
		HighExp = ExpLevelTable[Level];
		if ( Penalty )
			exp = (HighExp - LowExp)/Penalty;			//¾à 3%
		else
			exp = 0;

		exp64 = CodeXorCharInfo_Exp();
		if ( exp64>=LowExp ) {
			ep = exp64;
			exp64 -= exp;
			if ( exp64<LowExp ) {
				exp64=LowExp;
				ExpLower = TRUE;
			}
			SetExp64( &lpCurPlayer->smCharInfo , exp64 );
			ep = ep-exp64;
			SetTotalSubExp( (int)ep );		//°æÇèÄ¡ Áõ°¨ ±â·Ï
		}
		CodeXorCharInfo_Exp();				//°æÇèÄ¡ ÄÚµå¹ÝÀü

		//Money = lpChar->smCharInfo.Money/25;	//µ· 4% ¾ø¾îÁü
		if ( Penalty )
			Money = lpChar->smCharInfo.Money/Penalty;	//µ· 3% ¾ø¾îÁü
		else
			Money = 0;

		lpChar->smCharInfo.Money -= Money;
		if ( lpChar->smCharInfo.Money<0 ) {
			Money = -Money;
			lpChar->smCharInfo.Money = 0;
		}
		SetTotalSubMoney( Money );	//µ· °¨¼Ò ±â·Ï

		//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
		ReformCharForm();
		ResetInvenMoney();		//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
	}

	return ExpLower;
}

//ÇÊµå¿¡¼­ Àç½ÃÀÛ Ä³¸¯ÅÍ °æÇèÄ¡ Ãß°¡ ±ïÀÓ
int ContGameExp( smCHAR *lpChar )
{
	int Level;
	INT64 LowExp;
	INT64 HighExp;
	INT64 exp;
	INT64 ep;
	INT64 exp64;
	int Money;

	Level = lpChar->smCharInfo.Level;
	if ( Level>=11 ) {
		LowExp = ExpLevelTable[Level-1];
		HighExp = ExpLevelTable[Level];
		exp = (HighExp - LowExp)/100;			//¾à 1%

		//lpChar->smCharInfo.Exp ^= CodeXorExp;		//°æÇèÄ¡ ÄÚµå¹ÝÀü
		exp64 = CodeXorCharInfo_Exp();

		if ( exp64>LowExp ) {
			ep = exp64;
			exp64 -= exp;
			if ( exp64<LowExp ) 
				exp64=LowExp;

			SetExp64( &lpCurPlayer->smCharInfo , exp64 );

			ep = ep - exp64;
			SetTotalSubExp( (int)ep );		//°æÇèÄ¡ Áõ°¨ ±â·Ï
		}

		CodeXorCharInfo_Exp();
		//lpChar->smCharInfo.Exp ^= CodeXorExp;		//°æÇèÄ¡ ÄÚµå¹ÝÀü

		Money = lpChar->smCharInfo.Money/100;	//µ· 1% ¾ø¾îÁü
		lpChar->smCharInfo.Money -= Money;
		if ( lpChar->smCharInfo.Money<0 ) {
			Money = -Money;
			lpChar->smCharInfo.Money = 0;
		}

		SetTotalSubMoney( Money );	//µ· °¨¼Ò ±â·Ï

		//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
		ReformCharForm();
		ResetInvenMoney();		//ÀÌº¥Åä¸® µ· °Ë»ç ÃÊ±âÈ­
	}

	return TRUE;
}



//¹æ¾îÀ² °è»ê
int GetPlayDefence( smCHAR_INFO *lpAttackCharInfo , smCHAR_INFO *lpDefenceCharInfo )
{
	int defence;

	float ar,def,dLv,aLv;
	int weight;

	//¹æ¾îÀ² °è»ê
	ar = (float)lpAttackCharInfo->Attack_Rating;
	aLv = (float)lpAttackCharInfo->Level;
	def = (float)lpDefenceCharInfo->Defence;
	dLv = (float)lpDefenceCharInfo->Level;
	weight = lpDefenceCharInfo->Weight[0];
	defence = (int) (((((ar/(ar+(def/2))) - ((dLv-aLv)/50)))  * 100)) - (weight/10000) ;

	if ( defence>95 ) defence=95;

	return defence;
}


int RainMaterial[4][8];


int InitWeatherSystem()
{
	return TRUE;
}


extern int WeatherMode;

int DrawWeather( int ax ) { return TRUE; }



HANDLE	hLoadingThread = 0;
HBITMAP hBitmapLamp = 0;
HDC	hdcBitmapLamp = 0;
HBITMAP hBitmapLoadImage = 0;
HBITMAP hBitmapLoadImage2 = 0;
HDC	hdcBitmapLoadImage = 0;

int DispLoading;


//·Îµù·¥ÇÁ
int InitLoadingLamp( HWND hWnd ) { return TRUE; }
//HBITMAP hBitmapLoadImage = 0;
//HDC	hdcBitmapLoadImage = 0;

//·Îµù ·¥ÇÁ Á¦°Å
int CloseLoadingLamp() { return TRUE; }

DWORD dwLampThreadId;
DWORD dwCurrentThreadId;

//·ÎµùÁß È­¸é Ç¥½Ã
int OpenDispLoading() { return TRUE; }
//·Îµù Ç¥½ÃÁ¾·á
int CloseDispLoading()
{

	DispLoading = FALSE;
	return TRUE;
}

CRITICAL_SECTION cDrawSection;

#define DYN_REMOVER_MAX		64
#define DYN_REMOVER_MASK	(DYN_REMOVER_MAX-1)

smDPAT *DynRemoveList[DYN_REMOVER_MAX];
int DynRemoveCnt = 0;
int DynRemovePush = 0;

HANDLE	hRemoverThread=0;

DWORD WINAPI RemoveDynaPatternThreadProc( void *pInfo )
{
	HANDLE	hThread;
//	HDC	hdc;
	int Counter = 0;
//	int cnt;

	int RemoveCnt;

	hThread = GetCurrentThread();

	while(1) {
		if ( quit ) break;

		if ( (DynRemovePush-DynRemoveCnt)>32 ) {
			//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
			EnterCriticalSection( &cLoadSection );
			EnterCriticalSection( &cDrawSection );

			while(1) {
				RemoveCnt = DynRemoveCnt&DYN_REMOVER_MASK;
				smDPAT_Delete( DynRemoveList[RemoveCnt] );
				DynRemoveCnt++;
				if ( DynRemovePush<=DynRemoveCnt ) break;
			}

			//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
			LeaveCriticalSection( &cDrawSection );
			LeaveCriticalSection( &cLoadSection );
		}
		else
			SuspendThread(hThread);

		Counter++;
	}

	ExitThread( TRUE );
	return TRUE;
}

int RemoveDynPatSub()
{
	int RemoveCnt;

	while(1) {
		if ( (DynRemovePush-DynRemoveCnt)>32 ) {
			while(1) {
				RemoveCnt = DynRemoveCnt&DYN_REMOVER_MASK;
				smDPAT_Delete( DynRemoveList[RemoveCnt] );
				DynRemoveCnt++;
				if ( DynRemovePush<=DynRemoveCnt ) break;
			}
		}
		else
			break;
	}

	return TRUE;
}


//³ªÀÌ³ª¹Í ÆÐÅÏ »èÁ¦ Ãß°¡
int AddRemoveDynPattern( smDPAT *lpDinaPattern )
{

	//DWORD dwMsgId;

	if ( ServerMode ) {
		if ( lpDinaPattern->UseCount>1 )
			smDPAT_Delete( lpDinaPattern );

		return TRUE;
	}
/*
	if ( !hRemoverThread ) 
		hRemoverThread = CreateThread( NULL , 0, RemoveDynaPatternThreadProc , 0 , 0, &dwMsgId );
	else {
		DynRemoveList[DynRemovePush&DYN_REMOVER_MASK] = lpDinaPattern;
		DynRemovePush++;
		ResumeThread( hRemoverThread );
	}
*/

	DynRemoveList[DynRemovePush&DYN_REMOVER_MASK] = lpDinaPattern;
	DynRemovePush++;
	RemoveDynPatSub();

	return DynRemovePush;
}


//		smDPAT_Delete( lpDinaPattern2 );
//		lpDinaPattern2 = 0;

struct	sCOMPACT_MAP {
	smTEXTUREHANDLE	*hTexHandle;
	smTEXTUREHANDLE	*hTexTitleHandle;
	RECT	StageRect;
	sFIELD	*lpField;
	int		rx,ry;

};

sCOMPACT_MAP sCompactMap[4];

int MatArrow;
int MatMapBox;
int	MatPartyPos;
int	MatNpcPos;

//ÇÊµå ¸Ê ÃÊ±âÈ­
int InitFieldMap()
{
	return TRUE;
}

//ÇÊµå¸Ê ·Îµù
int LoadFieldMap( int PosNum , sFIELD *lpField , smSTAGE3D *lpStage )
{
	int ReadTexFlag = 0;

	if ( !lpField || !lpStage || sCompactMap[PosNum].lpField==lpField ) 
		return TRUE;

	sCompactMap[PosNum].lpField = lpField;

	memcpy( &sCompactMap[PosNum].StageRect , &lpStage->StageMapRect , sizeof( RECT ) );

	if (  sCompactMap[PosNum].hTexHandle ) {
		smTexture.Delete( sCompactMap[PosNum].hTexHandle );
		sCompactMap[PosNum].hTexHandle = 0;
	}
	if (  sCompactMap[PosNum].hTexTitleHandle ) {
		smTexture.Delete( sCompactMap[PosNum].hTexTitleHandle );
		sCompactMap[PosNum].hTexTitleHandle = 0;
	}


	if ( lpField->szNameMap[0] ) {
		sCompactMap[PosNum].hTexHandle = smTexture.Add( lpField->szNameMap, 0 );
		sCompactMap[PosNum].hTexHandle->TexSwapMode = FALSE;

		sCompactMap[PosNum].rx = (lpStage->StageMapRect.right - lpStage->StageMapRect.left)>>FLOATNS;
		sCompactMap[PosNum].ry = (lpStage->StageMapRect.bottom - lpStage->StageMapRect.top)>>FLOATNS;

		ReadTexFlag = TRUE;
	}
	else {
		sCompactMap[PosNum].hTexHandle = 0;
		return FALSE;
	}

	if ( lpField->szNameTitle[0] ) {
		sCompactMap[PosNum].hTexTitleHandle = smTexture.Add( lpField->szNameTitle, 0 );
		sCompactMap[PosNum].hTexTitleHandle->TexSwapMode = FALSE;

		ReadTexFlag = TRUE;
	}
	else {
		sCompactMap[PosNum].hTexTitleHandle = 0;
		return FALSE;
	}

	if ( ReadTexFlag )	ReadTextures();

	return TRUE;
}
//±×¸² Ãâ·Â
int psDrawTexImage( smTEXTUREHANDLE *hTexHandle , float x, float y, float w, float h ,
				   float fx, float fy, float fw, float fh, int Transp , int specular);//±×¸² Ãâ·Â

int psDrawTexImage_Point( smTEXTUREHANDLE *hTexHandle , float x, float y, float w, float h ,
				   float fx, float fy, float fw, float fh, int Transp , int specular);

int psDrawTexImage2( smTEXTUREHANDLE *hTexHandle , float *x, float *y, 
					float fx, float fy, float fw, float fh, int Transp , int specular);

int DrawMapArrow( int x, int y, int Angle );
int psSetDefaultRenderState();

//ÇÊµå ¸Ê ±×¸®±â
int DrawFieldMap( sCOMPACT_MAP *lpCompactMap , int px, int py , int size );

int FieldMapDrawMode = 2;

int FieldMapDrawSize[3] = { 8 , 16 , 24 };
int FieldMapViewSize1 = 18*64*fONE;
int FieldMapViewSize2 = 8*64*fONE;

//ÇÊµå ¸Ê ±×¸®±â
int MainFieldMap()
{
	if ( FieldMapViewSize1>FieldMapViewSize2 ) {
		FieldMapViewSize2 += 2048;
		if ( FieldMapViewSize1<FieldMapViewSize2 )
			FieldMapViewSize2 = FieldMapViewSize1;
	}

	if ( FieldMapViewSize1<FieldMapViewSize2 ) {
		FieldMapViewSize2 -= 2048;
		if ( FieldMapViewSize1>FieldMapViewSize2 )
			FieldMapViewSize2 = FieldMapViewSize1;
	}


	return TRUE;
}

extern int MatEachBox;
//¹Ì´Ï¸Ê¿¡ ÆÄÆ¼ À¯Àú Ç¥½Ã
int	DrawMapPartyPlayer( int px, int py );
//¹Ì´Ï¸Ê¿¡ NPC Ç¥½Ã
int	DrawMapNPC( int px, int py );

//ÇÊµå¹Ì´Ï¸ÊÀÇ ¸¶¿ì½º À§Ä¡ È®ÀÎ
int	GetFieldMapCursor() { return TRUE; }

//ÇÊµå ¸Ê ±×¸®±â
int DrawFieldMap() { return TRUE; }


//¹Ì´Ï¸Ê¿¡ ÆÄÆ¼ À¯Àú Ç¥½Ã
int	DrawMapPartyPlayer( int px, int py ) { return TRUE; }


//¹Ì´Ï¸Ê¿¡ NPC Ç¥½Ã
int	DrawMapNPC( int px, int py ) { return TRUE; }




int DrawMapArrow( int x, int y, int Angle ) { return TRUE; }

//ÇÊµå ¸Ê ±×¸®±â
int DrawFieldMap( sCOMPACT_MAP *lpCompactMap , int px, int py , int size ) { return TRUE; }

int psSetDefaultRenderState() { return TRUE; }

int psSetDefaultRenderState_PointFilter() { return TRUE; }

//±×¸² Ãâ·Â
int psDrawTexImage( smTEXTUREHANDLE *hTexHandle , float x, float y, float w, float h ,
				   float fx, float fy, float fw, float fh, int Transp , int specular) {
	return TRUE;
}

//±×¸² Ãâ·Â
int psDrawTexImage_Point( smTEXTUREHANDLE *hTexHandle , float x, float y, float w, float h ,
				   float fx, float fy, float fw, float fh, int Transp , int specular) {
	return TRUE;
}

//±×¸² Ãâ·Â
int psDrawTexImage2( smTEXTUREHANDLE *hTexHandle , float *x, float *y, float fx, float fy, float fw, float fh, int Transp , int specular) { return TRUE; }


int EecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int size , int DestSize )
{
	int EncCounter;
	int cnt;
	int Counter;
	int pSet;
	BYTE c1,c2;
	int lsize;

	Counter = 0;
	EncCounter = 4;
	lsize = size-1;

	while(1) {		
		if ( Counter>=size ) break;
		if ( EncCounter>=DestSize ) break;

		c1 = lpSrcData[Counter];
		if ( Counter<lsize ) c2 = lpSrcData[Counter+1];
		else c2 = c1;

		if ( c1==0 && c2==0 ) {
			for(cnt=0;cnt<0x7F;cnt++) {
				if ( Counter>=size ) break;
				if ( lpSrcData[Counter]!=0 ) break;
				Counter++;
			}
			if ( EncCounter>=DestSize ) break;
			lpDestData[EncCounter++]=cnt|0x80;
		}
		else {
			pSet = EncCounter;
			EncCounter++;

			for(cnt=0;cnt<0x7F;cnt++) {

				c1 = lpSrcData[Counter];
				if ( Counter<lsize ) c2 = lpSrcData[Counter+1];
				else c2 = c1;

				if ( Counter>=size ) break;
				if ( EncCounter>=DestSize ) break;
				if ( c1==0 && c2==0 ) break;
				lpDestData[EncCounter++]=lpSrcData[Counter++];
			}
			lpDestData[pSet]=cnt;
		}
	}

	((int *)lpDestData)[0] = EncCounter;

	return EncCounter;
}



//¾ÐÃàµ¥ÀÌÅ¸ ¼­¹ö ¾ÆÀÌÅÛ ÇØµ¶¿ë ( Z/NZ ¹æ½Ä )
int DecodeCompress_ItemPotion( rsPLAYINFO *lpPlayInfo , BYTE *lpSrcData , BYTE *lpDestData , int deCount , void *lpTransRecdata )
{

	int DecCounter;
	int cnt;
	int Counter;
	int size;
	int len;
	int dCount = deCount;
	int pCnt;
	TRANS_RECORD_DATA	*lpTransRecordData = (TRANS_RECORD_DATA *)lpTransRecdata;

	sRECORD_ITEM	*lpRecordItem = (sRECORD_ITEM *)lpDestData;

	size = ((int *)lpSrcData)[0];

	Counter = 4;
	DecCounter = 0;

	while(1) {		
		if ( Counter>=size ) break;
		if ( lpSrcData[Counter]&0x80 ) {
			//0 ¹Ýº¹ µ¥ÀÌÅ¸
			len = lpSrcData[Counter++]&0x7F;
			for(cnt=0;cnt<len;cnt++) {
				if ( DecCounter>=dCount ) {
					if ( dCount>=sizeof(sRECORD_ITEM) ) goto Skip_Decmp;
					if ( (lpRecordItem->sItemInfo.CODE&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) ) 
						return Counter;

					dCount = sizeof(sRECORD_ITEM);
				}
				lpDestData[DecCounter++] = 0;
			}

		}
		else {
			len = lpSrcData[Counter++]&0x7F;
			for(cnt=0;cnt<len;cnt++) {
				if ( DecCounter>=dCount ) {
					if ( dCount>=sizeof(sRECORD_ITEM) ) goto Skip_Decmp;
					if ( (lpRecordItem->sItemInfo.CODE&sinITEM_MASK1)!=(sinPM1&sinITEM_MASK1) ) 
						return Counter;

					dCount = sizeof(sRECORD_ITEM);
				}
				lpDestData[DecCounter++] = lpSrcData[Counter++];
			}
		}
	}

Skip_Decmp:

	if ( lpTransRecordData ) {
		//¹ö·ÁÁø ¾ÆÀÌÅÛ È®ÀÎ
		for( cnt=0;cnt<lpTransRecordData->ThrowItemCount;cnt++ ) {

			if ( lpTransRecordData->ThrowItemInfo[cnt].dwCode==lpRecordItem->sItemInfo.CODE &&
				lpTransRecordData->ThrowItemInfo[cnt].dwKey==lpRecordItem->sItemInfo.ItemHeader.Head &&
				lpTransRecordData->ThrowItemInfo[cnt].dwSum==lpRecordItem->sItemInfo.ItemHeader.dwChkSum ) {

					//Á¦°ÅµÉ ¾ÆÀÌÅÛ ÀÌ´Ù. (µî·ÏÃë¼ÒÇÏ°í ¸®ÅÏÇÑ´Ù)
					return Counter;

				}
		}
	}

	pCnt = lpRecordItem->sItemInfo.PotionCount;
	if ( pCnt==0 ) pCnt=1;

	rsAddServerPotion( lpPlayInfo , lpRecordItem->sItemInfo.CODE , pCnt );
	return Counter;
}


//¾ÐÃàµ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int dCount )
{

	int DecCounter;
	int cnt;
	int Counter;
	int size;
	int len;

	size = ((int *)lpSrcData)[0];

	Counter = 4;
	DecCounter = 0;

	while(1) {		
		if ( Counter>=size ) break;
		if ( lpSrcData[Counter]&0x80 ) {
			//0 ¹Ýº¹ µ¥ÀÌÅ¸
			len = lpSrcData[Counter++]&0x7F;
			for(cnt=0;cnt<len;cnt++) {
				if ( DecCounter>=dCount ) return Counter;
				lpDestData[DecCounter++] = 0;
			}

		}
		else {
			len = lpSrcData[Counter++]&0x7F;
			for(cnt=0;cnt<len;cnt++) {
				if ( DecCounter>=dCount ) return Counter;
				lpDestData[DecCounter++] = lpSrcData[Counter++];
			}
		}
	}

	return Counter;
}


//¾ÐÃàµ¥ÀÌÅ¸ ÇØµ¶ ( Z/NZ ¹æ½Ä )
int DecodeCompress( BYTE *lpSrcData , BYTE *lpDestData )
{
	return DecodeCompress(lpSrcData, lpDestData , 0x7FFFF );
}
//µ¥ÀÌÅ¸ ¾ÐÃà ( Z/NZ ¹æ½Ä )
int EecodeCompress( BYTE *lpSrcData , BYTE *lpDestData , int size )
{
	return EecodeCompress( lpSrcData , lpDestData , size , 0x7FFFF );
}

//´Ù¸¥ Ä³¸¯ÅÍ¿ÍÀÇ À§Ä¡ °ãÄ¡´ÂÁö È®ÀÎ
smCHAR *CheckOtherPlayPosi( int x, int y, int z )
{
	int cnt;
	int dist;
	int dx,dz;


	if ( ServerMode )
		return Serv_CheckOtherPlayPosi( x,y,z );

	for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++) {
		if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].smCharInfo.Life[0]>0 && 
			(chrOtherPlayer[cnt].dwLastTransTime+DISPLAY_TRANS_TIME_OVER)>dwPlayTime ) {
			if ( (y-16*fONE)<chrOtherPlayer[cnt].pY && (y+16*fONE)>chrOtherPlayer[cnt].pY ) {

				dx = (chrOtherPlayer[cnt].pX-x)>>FLOATNS;
				dz = (chrOtherPlayer[cnt].pZ-z)>>FLOATNS;
				dist =dx*dx+dz*dz;
				if ( dist<(16*16) ) return &chrOtherPlayer[cnt];
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
  			 if ( msg.message==WM_SYSKEYDOWN || msg.message==WM_SYSKEYUP ) continue;
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

static DWORD ReformStateCode( char *lpState , int size )
{
	int cnt;

	for(cnt=0;cnt<size;cnt++) {
		iFormCode += ((DWORD)lpState[cnt])*((iFromCnt&0x1FFF)+1);
		iFromCnt++;
	}

	return iFormCode;
}

static DWORD CompareStateCode( char *lpState1 , char *lpState2 , int size )
{
	int cnt;

	for(cnt=0;cnt<size;cnt++) {
		if ( lpState1[cnt]!=lpState2[cnt] )
			return 1;
	}

	return 0;
}


//±¸¹öÀü ÀÎÁõ Ã³¸®
static DWORD GetRefromItemCode_Old( sITEMINFO *lpItemInfo , int Key ) {

	iFormCode = 0;
	iFromCnt = Key;

	ReformStateCode( (char *)&lpItemInfo->CODE , sizeof(DWORD) );
	ReformStateCode( (char *)lpItemInfo->ItemName , 32 );

	ReformStateCode( (char *)&lpItemInfo->Weight , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Price , sizeof(int) );

	ReformStateCode( (char *)lpItemInfo->Resistance , sizeof(short)*8 );
	ReformStateCode( (char *)lpItemInfo->Damage , sizeof(short)*2 );

	//ReformStateCode( (char *)&lpItemInfo->Defence , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->Shooting_Range , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Attack_Speed , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->Attack_Rating , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Critical_Hit , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->fSpeed , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->Potion_Space , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->fMagic_Mastery , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fMana_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fLife_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fStamina_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Life , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Mana , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Stamina , sizeof(float) );

	ReformStateCode( (char *)&lpItemInfo->Level , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Strength , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Spirit , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Talent , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Dexterity , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Health , sizeof(int) );

	ReformStateCode( (char *)lpItemInfo->Mana , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->Life , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->Stamina , sizeof(short)*2 );
	
	ReformStateCode( (char *)&lpItemInfo->Money , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->JobCodeMask , sizeof(DWORD) );

	ReformStateCode( (char *)&lpItemInfo->JobItem , sizeof(sITEM_SPECIAL) );

	return iFormCode;
}



static DWORD GetRefromItemCode_V10( sITEMINFO *lpItemInfo , int Key ) {

	iFormCode = 0;
	iFromCnt = Key;

	ReformStateCode( (char *)&lpItemInfo->CODE , sizeof(DWORD) );
	ReformStateCode( (char *)lpItemInfo->ItemName , 32 );

	ReformStateCode( (char *)&lpItemInfo->Weight , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Price , sizeof(int) );

	ReformStateCode( (char *)lpItemInfo->Resistance , sizeof(short)*8 );
	ReformStateCode( (char *)lpItemInfo->Damage , sizeof(short)*2 );

	ReformStateCode( (char *)&lpItemInfo->Defence , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->Shooting_Range , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Attack_Speed , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->Attack_Rating , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Critical_Hit , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->fSpeed , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->Potion_Space , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->fMagic_Mastery , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fMana_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fLife_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fStamina_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Life , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Mana , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Stamina , sizeof(float) );

	ReformStateCode( (char *)&lpItemInfo->Level , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Strength , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Spirit , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Talent , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Dexterity , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Health , sizeof(int) );

	ReformStateCode( (char *)lpItemInfo->Mana , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->Life , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->Stamina , sizeof(short)*2 );
	
	ReformStateCode( (char *)&lpItemInfo->Money , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->JobCodeMask , sizeof(DWORD) );

	ReformStateCode( (char *)&lpItemInfo->JobItem , sizeof(sITEM_SPECIAL) );

	return iFormCode;
}

//static DWORD CompareStateCode( char *lpState1 , char *lpState2 , int size )

static int CompareReformItemCode_V10( sITEMINFO *lpItemInfo1 , sITEMINFO *lpItemInfo2 ) 
{

	int code;

	code = 0;

	code += CompareStateCode( (char *)&lpItemInfo1->CODE , (char *)&lpItemInfo2->CODE , sizeof(DWORD) );
	code += CompareStateCode( (char *)lpItemInfo1->ItemName , (char *)lpItemInfo2->ItemName, 32 );

	code += CompareStateCode( (char *)&lpItemInfo1->Weight , (char *)&lpItemInfo2->Weight , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Price , (char *)&lpItemInfo2->Price , sizeof(int) );

	code += CompareStateCode( (char *)lpItemInfo1->Resistance , (char *)lpItemInfo2->Resistance , sizeof(short)*8 );
	code += CompareStateCode( (char *)lpItemInfo1->Damage , (char *)lpItemInfo2->Damage , sizeof(short)*2 );

	code += CompareStateCode( (char *)&lpItemInfo1->Defence , (char *)&lpItemInfo2->Defence , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->Shooting_Range , (char *)&lpItemInfo2->Shooting_Range , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Attack_Speed , (char *)&lpItemInfo2->Attack_Speed , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->Attack_Rating , (char *)&lpItemInfo2->Attack_Rating , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Critical_Hit , (char *)&lpItemInfo2->Critical_Hit , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->fSpeed , (char *)&lpItemInfo2->fSpeed , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->Potion_Space , (char *)&lpItemInfo2->Potion_Space , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->fMagic_Mastery , (char *)&lpItemInfo2->fMagic_Mastery , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fMana_Regen , (char *)&lpItemInfo2->fMana_Regen , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fLife_Regen , (char *)&lpItemInfo2->fLife_Regen , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fStamina_Regen , (char *)&lpItemInfo2->fStamina_Regen , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fIncrease_Life , (char *)&lpItemInfo2->fIncrease_Life , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fIncrease_Mana , (char *)&lpItemInfo2->fIncrease_Mana , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fIncrease_Stamina , (char *)&lpItemInfo2->fIncrease_Stamina , sizeof(float) );

	code += CompareStateCode( (char *)&lpItemInfo1->Level , (char *)&lpItemInfo2->Level , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Strength , (char *)&lpItemInfo2->Strength , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Spirit , (char *)&lpItemInfo2->Spirit , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Talent , (char *)&lpItemInfo2->Talent , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Dexterity , (char *)&lpItemInfo2->Dexterity , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Health , (char *)&lpItemInfo2->Health , sizeof(int) );

	code += CompareStateCode( (char *)lpItemInfo1->Mana , (char *)lpItemInfo2->Mana , sizeof(short)*2 );
	code += CompareStateCode( (char *)lpItemInfo1->Life , (char *)lpItemInfo2->Life , sizeof(short)*2 );
	code += CompareStateCode( (char *)lpItemInfo1->Stamina , (char *)lpItemInfo2->Stamina , sizeof(short)*2 );
	
	code += CompareStateCode( (char *)&lpItemInfo1->Money , (char *)&lpItemInfo2->Money , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->JobCodeMask , (char *)&lpItemInfo2->JobCodeMask , sizeof(DWORD) );

	code += CompareStateCode( (char *)&lpItemInfo1->JobItem , (char *)&lpItemInfo2->JobItem , sizeof(sITEM_SPECIAL) );

	return code;
}

static DWORD GetRefromItemCode( sITEMINFO *lpItemInfo , int Key ) {

	iFormCode = 0;
	iFromCnt = Key;

	ReformStateCode( (char *)&lpItemInfo->CODE , sizeof(DWORD) );
	ReformStateCode( (char *)lpItemInfo->ItemName , 32 );

	ReformStateCode( (char *)&lpItemInfo->Weight , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Price , sizeof(int) );

	ReformStateCode( (char *)lpItemInfo->Resistance , sizeof(short)*8 );
	ReformStateCode( (char *)lpItemInfo->Damage , sizeof(short)*2 );

	ReformStateCode( (char *)&lpItemInfo->fAbsorb , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->Defence , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->fBlock_Rating , sizeof(float) );

	ReformStateCode( (char *)&lpItemInfo->Shooting_Range , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Attack_Speed , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->Attack_Rating , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Critical_Hit , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->fSpeed , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->Potion_Space , sizeof(int) );

	ReformStateCode( (char *)&lpItemInfo->fMagic_Mastery , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fMana_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fLife_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fStamina_Regen , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Life , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Mana , sizeof(float) );
	ReformStateCode( (char *)&lpItemInfo->fIncrease_Stamina , sizeof(float) );

	ReformStateCode( (char *)&lpItemInfo->Level , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Strength , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Spirit , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Talent , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Dexterity , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->Health , sizeof(int) );

	ReformStateCode( (char *)lpItemInfo->Mana , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->Life , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->Stamina , sizeof(short)*2 );
	
	ReformStateCode( (char *)&lpItemInfo->Money , sizeof(int) );
	ReformStateCode( (char *)&lpItemInfo->JobCodeMask , sizeof(DWORD) );
	ReformStateCode( (char *)&lpItemInfo->JobItem , sizeof(sITEM_SPECIAL) );

	ReformStateCode( (char *)&lpItemInfo->UniqueItem , sizeof(DWORD) );
	ReformStateCode( (char *)lpItemInfo->EffectBlink , sizeof(short)*2 );
	ReformStateCode( (char *)lpItemInfo->EffectColor , sizeof(short)*4 );
	ReformStateCode( (char *)lpItemInfo->ScaleBlink , sizeof(short)*2 );
	ReformStateCode( (char *)&lpItemInfo->DispEffect , sizeof(DWORD) );

	return iFormCode;
}


///////////////////// ¼­¹ö¿ë /////////////////////////////
#ifdef _W_SERVER

static DWORD rsReformStateCode( char *lpState , int size )
{
	int cnt;

	for(cnt=0;cnt<size;cnt++) {
		iFormCode += ((DWORD)lpState[cnt])*((iFromCnt&0x7FF)+1);
		iFromCnt++;
	}

	return iFormCode;
}


static DWORD rsGetRefromItemCode( sITEMINFO *lpItemInfo , int Key ) {

	iFormCode = 0;
	iFromCnt = Key;

	rsReformStateCode( (char *)&lpItemInfo->CODE , sizeof(DWORD) );
	rsReformStateCode( (char *)lpItemInfo->ItemName , 32 );

	rsReformStateCode( (char *)&lpItemInfo->Weight , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Price , sizeof(int) );

	rsReformStateCode( (char *)lpItemInfo->Resistance , sizeof(short)*8 );
	rsReformStateCode( (char *)lpItemInfo->Damage , sizeof(short)*2 );

	rsReformStateCode( (char *)&lpItemInfo->fAbsorb , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->Defence , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->fBlock_Rating , sizeof(float) );

	rsReformStateCode( (char *)&lpItemInfo->Shooting_Range , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Attack_Speed , sizeof(int) );

	rsReformStateCode( (char *)&lpItemInfo->Attack_Rating , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Critical_Hit , sizeof(int) );

	rsReformStateCode( (char *)&lpItemInfo->fSpeed , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->Potion_Space , sizeof(int) );

	rsReformStateCode( (char *)&lpItemInfo->fMagic_Mastery , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->fMana_Regen , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->fLife_Regen , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->fStamina_Regen , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->fIncrease_Life , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->fIncrease_Mana , sizeof(float) );
	rsReformStateCode( (char *)&lpItemInfo->fIncrease_Stamina , sizeof(float) );

	rsReformStateCode( (char *)&lpItemInfo->Level , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Strength , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Spirit , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Talent , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Dexterity , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->Health , sizeof(int) );

	rsReformStateCode( (char *)lpItemInfo->Mana , sizeof(short)*2 );
	rsReformStateCode( (char *)lpItemInfo->Life , sizeof(short)*2 );
	rsReformStateCode( (char *)lpItemInfo->Stamina , sizeof(short)*2 );
	
	rsReformStateCode( (char *)&lpItemInfo->Money , sizeof(int) );
	rsReformStateCode( (char *)&lpItemInfo->JobCodeMask , sizeof(DWORD) );
	rsReformStateCode( (char *)&lpItemInfo->JobItem , sizeof(sITEM_SPECIAL) );

	rsReformStateCode( (char *)&lpItemInfo->UniqueItem , sizeof(DWORD) );
	rsReformStateCode( (char *)lpItemInfo->EffectBlink , sizeof(short)*2 );
	rsReformStateCode( (char *)lpItemInfo->EffectColor , sizeof(short)*4 );
	rsReformStateCode( (char *)lpItemInfo->ScaleBlink , sizeof(short)*2 );
	ReformStateCode( (char *)&lpItemInfo->DispEffect , sizeof(DWORD) );

	return iFormCode;
}

#ifdef _NEW_ITEM_FORMCODE3
#include "ItemForm2.h"
#else
#include "ItemForm.h"
#endif

//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ°ª ¾ò±â
int	rsGetItemForm_Server( sITEMINFO *lpItemInfo )
{

	return rsGetItem_ServerForm( lpItemInfo );
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
int	rsReformItem_Server( sITEMINFO *lpItemInfo )
{
	DWORD dwCode;

	//¼­¹ö¿ë Àç ÀÎÁõ ¾ÆÀÌÅÛ
	dwCode = rsGetItemForm_Server( lpItemInfo );

	lpItemInfo->ItemHeader.dwTime = dwCode;

	return TRUE;
}
#else
int	rsGetItemForm_Server( sITEMINFO *lpItemInfo )
{

	return TRUE;
}

int	rsReformItem_Server( sITEMINFO *lpItemInfo )
{

	return TRUE;
}


#endif

//static DWORD CompareStateCode( char *lpState1 , char *lpState2 , int size )

static int CompareReformItemCode( sITEMINFO *lpItemInfo1 , sITEMINFO *lpItemInfo2 ) 
{

	int code;

	code = 0;

	code += CompareStateCode( (char *)&lpItemInfo1->CODE , (char *)&lpItemInfo2->CODE , sizeof(DWORD) );
	code += CompareStateCode( (char *)lpItemInfo1->ItemName , (char *)lpItemInfo2->ItemName, 32 );

	code += CompareStateCode( (char *)&lpItemInfo1->Weight , (char *)&lpItemInfo2->Weight , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Price , (char *)&lpItemInfo2->Price , sizeof(int) );

	code += CompareStateCode( (char *)lpItemInfo1->Resistance , (char *)lpItemInfo2->Resistance , sizeof(short)*8 );
	code += CompareStateCode( (char *)lpItemInfo1->Damage , (char *)lpItemInfo2->Damage , sizeof(short)*2 );

	code += CompareStateCode( (char *)&lpItemInfo1->fAbsorb , (char *)&lpItemInfo2->fAbsorb , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->Defence , (char *)&lpItemInfo2->Defence , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->fBlock_Rating , (char *)&lpItemInfo2->fBlock_Rating , sizeof(float) );

	code += CompareStateCode( (char *)&lpItemInfo1->Shooting_Range , (char *)&lpItemInfo2->Shooting_Range , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Attack_Speed , (char *)&lpItemInfo2->Attack_Speed , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->Attack_Rating , (char *)&lpItemInfo2->Attack_Rating , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Critical_Hit , (char *)&lpItemInfo2->Critical_Hit , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->fSpeed , (char *)&lpItemInfo2->fSpeed , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->Potion_Space , (char *)&lpItemInfo2->Potion_Space , sizeof(int) );

	code += CompareStateCode( (char *)&lpItemInfo1->fMagic_Mastery , (char *)&lpItemInfo2->fMagic_Mastery , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fMana_Regen , (char *)&lpItemInfo2->fMana_Regen , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fLife_Regen , (char *)&lpItemInfo2->fLife_Regen , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fStamina_Regen , (char *)&lpItemInfo2->fStamina_Regen , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fIncrease_Life , (char *)&lpItemInfo2->fIncrease_Life , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fIncrease_Mana , (char *)&lpItemInfo2->fIncrease_Mana , sizeof(float) );
	code += CompareStateCode( (char *)&lpItemInfo1->fIncrease_Stamina , (char *)&lpItemInfo2->fIncrease_Stamina , sizeof(float) );

	code += CompareStateCode( (char *)&lpItemInfo1->Level , (char *)&lpItemInfo2->Level , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Strength , (char *)&lpItemInfo2->Strength , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Spirit , (char *)&lpItemInfo2->Spirit , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Talent , (char *)&lpItemInfo2->Talent , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Dexterity , (char *)&lpItemInfo2->Dexterity , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->Health , (char *)&lpItemInfo2->Health , sizeof(int) );

	code += CompareStateCode( (char *)lpItemInfo1->Mana , (char *)lpItemInfo2->Mana , sizeof(short)*2 );
	code += CompareStateCode( (char *)lpItemInfo1->Life , (char *)lpItemInfo2->Life , sizeof(short)*2 );
	code += CompareStateCode( (char *)lpItemInfo1->Stamina , (char *)lpItemInfo2->Stamina , sizeof(short)*2 );
	
	code += CompareStateCode( (char *)&lpItemInfo1->Money , (char *)&lpItemInfo2->Money , sizeof(int) );
	code += CompareStateCode( (char *)&lpItemInfo1->JobCodeMask , (char *)&lpItemInfo2->JobCodeMask , sizeof(DWORD) );
	code += CompareStateCode( (char *)&lpItemInfo1->JobItem , (char *)&lpItemInfo2->JobItem , sizeof(sITEM_SPECIAL) );

	code += CompareStateCode( (char *)&lpItemInfo1->UniqueItem , (char *)&lpItemInfo2->UniqueItem , sizeof(DWORD) );
	code += CompareStateCode( (char *)lpItemInfo1->EffectBlink , (char *)lpItemInfo2->EffectBlink , sizeof(short)*2 );
	code += CompareStateCode( (char *)lpItemInfo1->EffectColor , (char *)lpItemInfo2->EffectColor , sizeof(short)*4 );
	code += CompareStateCode( (char *)lpItemInfo1->ScaleBlink , (char *)lpItemInfo2->ScaleBlink , sizeof(short)*2 );
	code += CompareStateCode( (char *)&lpItemInfo1->DispEffect , (char *)&lpItemInfo2->DispEffect , sizeof(DWORD) );

	return code;
}



//¾ÆÀÌÅÛ ±³È¯Ã¢ ³»¿ë ÀÎÁõ
static DWORD GetRefromTradeCode( sTRADE *lpTrade , int Key ) {

	int cnt;

	iFormCode = 0;
	iFromCnt = Key;

	for(cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
		if ( lpTrade->TradeItem[cnt].Flag ) {
			ReformStateCode( (char *)&lpTrade->TradeItem[cnt].sItemInfo.CODE  , sizeof(DWORD) );
			ReformStateCode( (char *)&lpTrade->TradeItem[cnt].sItemInfo.ItemHeader.dwChkSum , sizeof(DWORD) );
		}
	}
	ReformStateCode( (char *)&lpTrade->Money , sizeof(int) );

	return iFormCode;
}


static DWORD GetRefromCharState( smCHAR_INFO *lpCharInfo , int Key ) {

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

	ReformStateCode( (char *)&lpCharInfo->szModelName2 , 64 );

	ReformStateCode( (char *)&lpCharInfo->Level , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Strength , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Spirit , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Talent , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Dexterity , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Health , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Level , sizeof(int) );


	ReformStateCode( (char *)&lpCharInfo->Accuracy , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Attack_Rating , sizeof(int) );
	ReformStateCode( (char *)lpCharInfo->Attack_Damage , sizeof(int)*2 );
	ReformStateCode( (char *)&lpCharInfo->Attack_Speed , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Shooting_Range , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Critical_Hit , sizeof(int) );

	ReformStateCode( (char *)&lpCharInfo->Defence , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Chance_Block , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Absorption , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Move_Speed , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Sight , sizeof(int) );
	ReformStateCode( (char *)lpCharInfo->Weight , sizeof(short)*2 );

	ReformStateCode( (char *)lpCharInfo->Resistance , sizeof(short)*8 );
	ReformStateCode( (char *)lpCharInfo->Attack_Resistance , sizeof(short)*8 );

	ReformStateCode( (char *)lpCharInfo->Life , sizeof(short)*2 );
	ReformStateCode( (char *)lpCharInfo->Mana , sizeof(short)*2 );
	ReformStateCode( (char *)lpCharInfo->Stamina , sizeof(short)*2 );

	ReformStateCode( (char *)&lpCharInfo->Exp , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Next_Exp , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->Money , sizeof(int) );
	ReformStateCode( (char *)&lpCharInfo->StatePoint , sizeof(int) );

#ifdef _MODE_EXP64
	ReformStateCode( (char *)&lpCharInfo->Exp_High , sizeof(int) );
#endif

	return iFormCode;
}


static int ItemFormVersion = 12;
static int ItemFormKey = 0;

//¾ÆÀÌÅÛ ÀÎÁõ ¹Þ±â
int ReformItem( sITEMINFO *lpItemInfo )
{

	//½Å±Ô ÀÎÁõ
	DWORD dwCode;

#ifdef _W_SERVER
	// ¼­¹öÃø ÀÎÁõ 
extern time_t		tServerTime;

	lpItemInfo->ItemHeader.Head = ItemFormKey+GetCurrentTime();

	dwCode = GetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.Head );

	lpItemInfo->ItemHeader.dwChkSum = dwCode;
	lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	lpItemInfo->dwCreateTime = (DWORD)tServerTime;

	if ( !lpItemInfo->BackUpKey || !lpItemInfo->BackUpChkSum ) {
		lpItemInfo->BackUpKey = lpItemInfo->ItemHeader.Head;
		lpItemInfo->BackUpChkSum = lpItemInfo->ItemHeader.dwChkSum;
	}

	ReformMixItem( lpItemInfo );			//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Þ±â
	rsReformItem_Server( lpItemInfo );		//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ¸¸ ¹Þ±â

	lpItemInfo->Temp0 = lpItemInfo->ItemHeader.dwChkSum+lpItemInfo->CODE;

#else
	// Å¬¶óÀÌ¾ðÆ®Ãø ÀÎÁõ ( ºÎºÐÀÎÁõ )
	if ( !lpItemInfo->CODE || lpItemInfo->CODE==(sinGG1|sin01) || (lpItemInfo->CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
		lpItemInfo->ItemHeader.Head = ItemFormKey+GetCurrentTime();

		dwCode = GetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.Head );

		lpItemInfo->ItemHeader.dwChkSum = dwCode;
		lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	}
#endif

	ItemFormKey++;

	return TRUE;
}

int ReformItemRestaure(sITEMINFO* lpItemInfo)
{
	DWORD dwCode;

	extern time_t		tServerTime;

	//lpItemInfo->ItemHeader.Head = ItemFormKey + GetCurrentTime();

	dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);

	//lpItemInfo->ItemHeader.dwChkSum = dwCode;
	//lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	//lpItemInfo->dwCreateTime = (DWORD)tServerTime;

	ReformMixItem(lpItemInfo);
	//rsReformItem_Server(lpItemInfo);

	lpItemInfo->Temp0 = lpItemInfo->ItemHeader.dwChkSum + lpItemInfo->CODE;

	if (!lpItemInfo->CODE || lpItemInfo->CODE == (sinGG1 | sin01) || (lpItemInfo->CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1)) {
		lpItemInfo->ItemHeader.Head = ItemFormKey + GetCurrentTime();

		dwCode = GetRefromItemCode(lpItemInfo, lpItemInfo->ItemHeader.Head);

		lpItemInfo->ItemHeader.dwChkSum = dwCode;
		lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	}

	//ItemFormKey++;

	return TRUE;
}

//¾ÆÀÌÅÛ ÀÎÁõ ¹Þ±â ( »ý¼º½Ã°£ º¸Á¤ )
int ReformItem_AdjustTime( sITEMINFO *lpItemInfo , int TimeAdjust )
{

#ifdef _W_SERVER

	//½Å±Ô ÀÎÁõ
	DWORD dwCode;

	// ¼­¹öÃø ÀÎÁõ 
extern time_t		tServerTime;

	lpItemInfo->ItemHeader.Head = ItemFormKey+GetCurrentTime();

	dwCode = GetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.Head );

	lpItemInfo->ItemHeader.dwChkSum = dwCode;
	lpItemInfo->ItemHeader.dwVersion = ItemFormVersion;
	lpItemInfo->dwCreateTime = ( DWORD )tServerTime + TimeAdjust;

	if ( !lpItemInfo->BackUpKey || !lpItemInfo->BackUpChkSum ) {
		lpItemInfo->BackUpKey = lpItemInfo->ItemHeader.Head;
		lpItemInfo->BackUpChkSum = lpItemInfo->ItemHeader.dwChkSum;
	}

	ReformMixItem( lpItemInfo );			//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Þ±â
	rsReformItem_Server( lpItemInfo );		//¾ÆÀÌÅÛ ¼­¹ö ÀÎÁõ¸¸ ¹Þ±â

	lpItemInfo->Temp0 = lpItemInfo->ItemHeader.dwChkSum+lpItemInfo->CODE;

#endif

	ItemFormKey++;

	return TRUE;
}



#ifdef _W_SERVER
//¾ÆÀÌÅÛ ÀÎÁõ È®ÀÎ
int CheckItemForm( sITEMINFO *lpItemInfo )
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
	if ( lpItemInfo->CODE==(sinGG1|sin01) || (lpItemInfo->CODE&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) {
		dwCode = GetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.Head );
		if ( lpItemInfo->ItemHeader.dwChkSum==dwCode ) return TRUE;
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
	dwCode = rsGetItemForm_Server( lpItemInfo );

	if ( lpItemInfo->ItemHeader.dwTime==dwCode ) return TRUE;

	return FALSE;
}

#else
//¾ÆÀÌÅÛ ÀÎÁõ È®ÀÎ
int CheckItemForm( sITEMINFO *lpItemInfo )
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
	dwCode = GetRefromItemCode( lpItemInfo , lpItemInfo->ItemHeader.Head );

	if ( lpItemInfo->ItemHeader.dwChkSum==dwCode ) return TRUE;

	return FALSE;
}
#endif

//¾ÆÀÌÅÛ 2°³¸¦ ºñ±³ÇÑ´Ù
int CompareItems( sITEMINFO *lpItemInfo1 , sITEMINFO *lpItemInfo2 )
{

	if ( lpItemInfo1->ItemHeader.dwVersion!=(DWORD)ItemFormVersion ) CheckItemForm(lpItemInfo1);
	if ( lpItemInfo2->ItemHeader.dwVersion!=(DWORD)ItemFormVersion ) CheckItemForm(lpItemInfo2);

	if ( lpItemInfo1->CODE!=lpItemInfo2->CODE ) return FALSE;
	if ( lpItemInfo1->ItemHeader.Head!=lpItemInfo2->ItemHeader.Head ) return FALSE;
	if ( lpItemInfo1->ItemHeader.dwChkSum!=lpItemInfo2->ItemHeader.dwChkSum ) return FALSE;

	if ( CompareReformItemCode( lpItemInfo1 , lpItemInfo2 )==0 ) 
		return TRUE;

	return FALSE;
}




//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
int ReformCharForm( smCHAR_INFO *lpCharInfo )
{
	DWORD dwCode;

	dwCode = GetRefromCharState( lpCharInfo , 10 );

	lpCharInfo->RefomCode = dwCode;

	ItemFormKey++;

	return TRUE;
}

//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckCharForm( smCHAR_INFO *lpCharInfo )
{
	DWORD dwCode;

	dwCode = GetRefromCharState( lpCharInfo , 10 );

	if ( lpCharInfo->RefomCode==dwCode ) return TRUE;

	return FALSE;
}


//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â
int ReformCharForm()
{
	//lpCurPlayer->smCharInfo.Attack_Damage[1] = 300;
	//lpCurPlayer->smCharInfo.Attack_Damage[0] = 300;

	return ReformCharForm( &lpCurPlayer->smCharInfo );

}

//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckCharForm()
{
	int val;

	val = CheckCharForm( &lpCurPlayer->smCharInfo );

	if ( val==FALSE ) {//|| ( VRKeyBuff['0'] && DisconnectFlag==0 )) {

		//ÇØÅ· ½Ãµµ ¾Ë¸²
		SendSetHackUser( 81 );

		DisconnectServerCode = 4;
		DisconnectFlag = GetCurrentTime();

	}

	return val;
}

//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ ¹Þ±â
int ReformTrade( void *lpTrade )
{
	DWORD dwCode;

	dwCode = GetRefromTradeCode( (sTRADE *)lpTrade , 84 );

	((sTRADE *)lpTrade)->CheckMoney = dwCode;

	ItemFormKey++;

	return TRUE;
}

//Æ®·¹ÀÌµå Á¤º¸ ÀÎÁõ È®ÀÎ
int CheckTrade( void *lpTrade )
{
	DWORD dwCode;

	dwCode = GetRefromTradeCode( (sTRADE *)lpTrade , 84 );

	if ( ((sTRADE *)lpTrade)->CheckMoney==(int)dwCode ) return TRUE;

	//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í ( Æ®·¹ÀÌµå ÀÎÁõ ¿À·ù ¼­¹ö¿¡ ¾Ë¸² )
	SendSetHackUser2( 4000 , 0 );

	return FALSE;
}

/*
	short   ItemAgingNum[2];	//0Àº ¿¡ÀÌÂ¡+¸î 1Àº ¿¡ÀÌÂ¡ÀÌ µÇ°íÀÖ³ª ¾Æ´Ñ°¡?
	short   ItemAgingCount[2];	//0¾ÆÀÌÅÛ ¼÷·Ã Ä«¿îÆ® 1¼÷·ÃÄ¡ ÃÖ´ë 
	short	ItemAgingProtect[2];

//ItemAgingProtect
*/

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõÄÚµå
short GetMixItemForm( sITEMINFO *lpItemInfo )
{
	short sCode=0;

#ifdef _W_SERVER

	DWORD dwCode;
	DWORD dwItemKey;

	if ( lpItemInfo->ItemKindCode ) {
		dwItemKey = lpItemInfo->ItemHeader.dwChkSum;
/*
		dwCode = lpItemInfo->ItemKindCode*(
			lpItemInfo->ItemAgingNum[0]*3+lpItemInfo->ItemAgingNum[1]*7+
			lpItemInfo->ItemAgingCount[0]*15+lpItemInfo->ItemAgingCount[1]+17 );
*/

		//¿¡ÀÌÂ¡ Ä«¿îÅÍ(ItemAgingCount) [1]Àº ¼­¹öÃø º¸¾È , [0]Àº Å¬¶óÀÌ¾ðÆ® º¸¾ÈÀ¸·Î º¯°æ
		dwCode = lpItemInfo->ItemKindCode*(
			lpItemInfo->ItemAgingNum[0]*3+lpItemInfo->ItemAgingNum[1]*7+lpItemInfo->ItemAgingCount[1]*17+17 );

		dwCode= dwCode^dwItemKey;

		sCode = (short)((dwCode>>16)^(dwCode&0xFFFF));
	}
#endif

	return sCode;
}

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ ¹Þ±â
int ReformMixItem( sITEMINFO *lpItemInfo )
{
#ifdef _W_SERVER
	lpItemInfo->ItemAgingProtect[0] = GetMixItemForm( lpItemInfo );
#endif
	return TRUE;
}

//¾ÆÀÌÅÛ ¿¡ÀÌÂ¡ ÀÎÁõ È®ÀÎ
int CheckMixItem( sITEMINFO *lpItemInfo )
{

#ifdef _W_SERVER
	if ( GetMixItemForm( lpItemInfo )==lpItemInfo->ItemAgingProtect[0] ) {
		return TRUE;
	}
#else
	return TRUE;
#endif

	return FALSE;
}

float GetItemAbsorb( sITEMINFO *lpItemInfo )
{
	float	fAborb;
	DWORD	dwCode;

	dwCode = lpItemInfo->CODE&sinITEM_MASK2;
	fAborb = lpItemInfo->fAbsorb;

	return	fAborb;
}



#define WAIT_BATTLE_QUIT_TIME		10000

//°ÔÀÓ Á¾·á
int QuitGame()
{
	#ifdef _XIGNCODE_CLIENT
	//¹ÚÀç¿ø - XignCode
	ZCWAVE_Cleanup();
	ZCWAVE_SysExit();
	#endif

	if ( quit ) return TRUE;

	if ( dwBattleTime ) {
		if ( (dwBattleTime+WAIT_BATTLE_QUIT_TIME)>dwPlayTime ) {
			dwBattleQuitTime = dwPlayTime;
			return FALSE;
		}
	}

	// Àåº° - Á¦·ÃÃ¢, Á¦ÀÛÃ¢ Á¾·á // ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã¢ Ãß°¡
	if ( cWareHouse.OpenFlag || cCraftItem.OpenFlag || cTrade.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag) 
	{ 
		dwCloseBoxTime = dwPlayTime+2000;
		return FALSE;
	}

	quit = 1;

	return TRUE;
}

#define CHARINFO_CACHE_MAX		200

smTRNAS_PLAYERINFO	CharInfo_Cache[CHARINFO_CACHE_MAX];
DWORD	dwCharInfo_CacheCode[CHARINFO_CACHE_MAX];
int	CharInfoCacheCnt = 0;

//Ä³¸¯ÅÍ Á¤º¸¸¦ Ä³½Ãµ¥ÀÌÅ¸·Î ºÎÅÍ ¾ò´Â´Ù
smTRNAS_PLAYERINFO	*GetCharInfoCache( char *lpData )
{
	DWORD	dwChkCode;
	int cnt;
	smPLAYDATA *lpPlayData;
	smTRANS_PLAYDATA	*lpTransPlayData;
	smTRANS_PLAYBUFF	*lpTransPlayBuff;
	int	RecvCode;
	smTRNAS_PLAYERINFO	*lpTransPlayInfo;

	RecvCode = ((int *)lpData)[1];

	dwChkCode = 0;

	switch( RecvCode ) {
	case smTRANSCODE_PLAYDATA1:
		lpPlayData = (smPLAYDATA *)lpData;
		dwChkCode = lpPlayData->dwAutoCharCode;
		break;
	case smTRANSCODE_TRANSPLAYDATA:
		lpTransPlayData = (smTRANS_PLAYDATA *)lpData;
		lpTransPlayBuff = (smTRANS_PLAYBUFF *)(lpData + sizeof(smTRANS_PLAYDATA) );
		if ( lpTransPlayData->PlayBuffCnt>0 )
			dwChkCode = lpTransPlayData->dwAutoCharCode;
		break;
	}

	if ( !dwChkCode ) return NULL;

	for(cnt=0;cnt<CharInfoCacheCnt;cnt++) {
		if ( dwChkCode==dwCharInfo_CacheCode[cnt] ) {
			//Ä³½Ã µ¥ÀÌÅ¸ Ã£À½
			lpTransPlayInfo = &CharInfo_Cache[cnt];

			switch( RecvCode ) {
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
				lpTransPlayBuff = (smTRANS_PLAYBUFF *) ( ((char *)lpTransPlayData)+sizeof(smTRANS_PLAYDATA) );
				lpTransPlayBuff += lpTransPlayData->PlayBuffCnt-1;

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
int SaveCharInfoCache( smTRNAS_PLAYERINFO *lpTransPlayInfo )
{
	int cnt;
	DWORD	dwCode;

	dwCode = GetSpeedSum( lpTransPlayInfo->smCharInfo.szName );

	//±âÁ¸ µ¥ÀÌÅ¸ ºñ±³
	for(cnt=0;cnt<CharInfoCacheCnt;cnt++) {
		if ( dwCode==dwCharInfo_CacheCode[cnt] ) return NULL;
	}

	//»õ·Î ÀúÀå
	dwCharInfo_CacheCode[CharInfoCacheCnt] = dwCode;
	memcpy( &CharInfo_Cache[CharInfoCacheCnt] , lpTransPlayInfo , sizeof(smTRNAS_PLAYERINFO) );
	CharInfoCacheCnt++;

	return TRUE;
}

//ÆÄÆ¼ °¡ÀÔ¿©ºÎ È®ÀÎ
int	RequestPartyPlayers( smCHAR	*lpChar ) { return TRUE; }


smCHAR	chrPartyPlayer;

//ÆÄÆ¼ ÇÃ·¹ÀÌ¾î ¾ó±¼ ¿ÀÇÂ
int OpenPartyPlayer( char *szName , char *szModelFile1 , char *szModelFile2 )
{
	//¾ÆÀÌÅÛÀ» »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
	AddLoaderPattern( &chrPartyPlayer , szModelFile1 , szModelFile2 );

	chrPartyPlayer.SetMotionFromCode( CHRMOTION_STATE_STAND );
	chrPartyPlayer.ActionPattern = 101;
	chrPartyPlayer.Flag = TRUE;
	chrPartyPlayer.lpStage = 0;
	chrPartyPlayer.OnStageField = lpCurPlayer->OnStageField;

	lstrcpy( chrPartyPlayer.smCharInfo.szName , szName );

	OpenEachPlayer( &chrPartyPlayer );

	return TRUE;
}

//extern HIMC hImc;

char	szLastWhisperName[32] = { 0 };
int		LastWhisperLen = 0;

//ÆÄÆ¼¿ø ±Ó¸» º¸³»±â
int	WhisperPartyPlayer( char *szName ) { return TRUE; }

//Å¬·£¿ø Ã¤ÆÃ º¸³»±â
int	SetClanChatting() { return TRUE; }

//Å¬·£ Ã¤ÆÃ ¸ðµå ¼³Á¤
int IsClanChatting()
{
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
	if ( MotionAttackCount>30 ) {
		//°ø°Ý ºñÀ² Ã½Å©
		CheckPlayAttackCount();
	}

	return MotionAttackCount;
}

//°ø°Ý ºñÀ² Ã½Å©
int	CheckPlayAttackCount()
{
	//30¹ø °ø°Ý ¹Þ´Â µ¿¾È °ø°Ý ÆÐÅ¶ÀÌ 3 ÀÌÇÏÀÏ °æ¿ì ¹®Á¦ÀÖ´Ù°í ÆÇ´ÜÇÔ
	if ( MotionAttackCount>30 && PlayAttackCount<=3 ) {
		//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
		//SendSetHackUser2( 1500 , MotionAttackCount-PlayAttackCount );
	}

	MotionAttackCount = 0;
	PlayAttackCount = 0;

	return TRUE;
}

struct	sCOPY_ITEM_INFO	{
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
int	CheckCopyItem( sITEMINFO *lpItem )
{
	int cnt;

	cnt = 0;

	if ( !lpItem->ItemName[0] )	return FALSE;			//²®µ¥±â ¾ÆÀÌÅÛ (¾ç¼Õ¹«±â)
	if ( lpItem->SpecialItemFlag[0] )	return FALSE;		//ÀÌ¹Ì µî·ÏµÈ °ªÀÌ ÀÖÀ½

	if ( !lpItem->BackUpKey || !lpItem->BackUpChkSum ) {
		lpItem->BackUpKey = lpItem->ItemHeader.Head;
		lpItem->BackUpChkSum = lpItem->ItemHeader.dwChkSum;
	}

	while(1) {
		if ( sCopyItemList[cnt].dwItemCode==0 ) break;


		if ( sCopyItemList[cnt].dwItemCode==lpItem->CODE ) {
			if ( sCopyItemList[cnt].ChkSum==(int)lpItem->ItemHeader.dwChkSum &&
				sCopyItemList[cnt].Header==(int)lpItem->ItemHeader.Head ) {

				lpItem->SpecialItemFlag[0] = CHECK_COPY_ITEM;
				return TRUE;
			}
			if ( sCopyItemList[cnt].ChkSum==(int)lpItem->BackUpChkSum &&
				sCopyItemList[cnt].Header==(int)lpItem->BackUpKey ) {

				lpItem->SpecialItemFlag[0] = CHECK_COPY_ITEM;
				return TRUE;
			}
		}
		cnt++;
	}


	return FALSE;
}


//¾ó±¼ º¯°æÇÑ°Å Àû¿ë
int SetJobFace( char *szModelName )
{

	lstrcpy( lpCurPlayer->smCharInfo.szModelName2 , szModelName );
	AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName , lpCurPlayer->smCharInfo.szModelName2 );
	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();	//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ ¹Þ±â

	//¼­¹ö¿¡ º¯°æµ¥ÀÌÅ¸ ¼ÛºÎ
	SendSetObjectSerial( lpCurPlayer->dwObjectSerial );

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
int	CheckUserModel( smCHAR_INFO *lpCharInfo )
{
	int cnt,cnt2,len;
	char	szBuff[64];
	char	*lpBuff;


	lpCharInfo->szModelName[31] = 0;
	lpCharInfo->szModelName2[31] = 0;

	len = lstrlen( lpCharInfo->szModelName );
	lstrcpy( szBuff , lpCharInfo->szModelName );
	for(cnt=len-1;cnt>=0;cnt--) {
		if ( szBuff[cnt]=='\\' ) {
			szBuff[cnt+1] = 0;
			break;
		}
	}

	if ( lstrcmpi( szBuff , "char\\tmABCD\\" )!=0 ) return FALSE;
	lpBuff = &lpCharInfo->szModelName[len-3];
	if ( lstrcmpi( lpBuff , "ini" )!=0 ) return FALSE;

	//Á÷¾÷º° ¾ó±¼ È®ÀÎ
	switch( lpCharInfo->JOB_CODE ) {
	case 1:
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_FighterFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_FighterFaceName[0][0] );
			return TRUE;
		}
		break;
	case 2:
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_MechFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_MechFaceName[0][0] );
			return TRUE;
		}
		break;
	case 3:
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_ArcherFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_ArcherFaceName[0][0] );
			return TRUE;
		}

		break;
	case 4:
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_PikeFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_PikeFaceName[0][0] );
			return TRUE;
		}
		break;

	case 0:
		return TRUE;
	case 5:	//¾ÆÅ»¶õÅ¸
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_AtalantaFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_AtalantaFaceName[0][0] );
			return TRUE;
		}
		break;
	case 6:	//³ªÀÌÆ®
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_KnightFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_KnightFaceName[0][0] );
			return TRUE;
		}
		break;
	case 7:	//¸ÞÁö¼Ç
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_Magican[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_Magican[0][0] );
			return TRUE;
		}
		break;
	case 8:	//ÇÁ¸®½ºÆ¼½º
		for(cnt=0;cnt<_CHANGE_JOB_COUNT_MAX;cnt++) {
			for(cnt2=0;cnt2<_FACE_COUNT_MAX;cnt2++) {
				if ( lstrcmpi( szModel_PristessFaceName[cnt][cnt2] , lpCharInfo->szModelName2 )==0 ) {
					return TRUE;
				}
			}
		}
		if ( !lpCharInfo->szModelName2[0] ) {
			//¾ó±¼¾øÀ½ ¾Æ¹«¾ó±¼ÀÌ³ª ¼³Á¤ÇØÁÖÀÚ!
			lstrcpy( lpCharInfo->szModelName2 , szModel_PristessFaceName[0][0] );
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

	ChangeModelSkin( lpCurPlayer , 0 , lpCurPlayer->smCharInfo.ChangeJob , 0 , 0 );			//Àü¾÷¾ó±¼
	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();
	SendSetObjectSerial( lpCurPlayer->dwObjectSerial );

	return FALSE;
}


////////////////////////////// ¿¡ÆåÆ® ¿¹¾à ½Ã½ºÅÛ ///////////////////////////////

#define	TIME_EFFECT_BUFF_MAX	16

struct sTIME_EFFECT_BUFF {
	DWORD	dwCode;
	DWORD	dwTime;
	int		x,y,z;
	smCHAR	*lpChar;
};

sTIME_EFFECT_BUFF TimeEffectBuff[TIME_EFFECT_BUFF_MAX];

//¿¹¾à ÀÌÆåÆ® ÃÊ±âÈ­
int InitTimeEffect()
{
	int cnt;
	for(cnt=0;cnt<TIME_EFFECT_BUFF_MAX;cnt++) {
		TimeEffectBuff[cnt].dwCode = 0;
	}
	
	return TRUE;
}

//ÀÌÆåÆ® ¿¹¾à Ãß°¡
int AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z , smCHAR *lpChar ) { return TRUE; }

int AddTimeEffect( DWORD EffectCode , DWORD dwTime , int x, int y, int z  ) { return TRUE; }

//¿¹¾à ÀÌÆåÆ® ½ÇÇà
int TimeEffectMain() { return TRUE; }

#define		ROT_PLAYER_MAX		128
#define		ROT_PLAYER_MIN		80

smCHAR	*lpChrPlayers[ROT_PLAYER_MAX];


//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ÃÊ±âÈ­
smCHAR *InitRotPlayer()
{
	int cnt;
	for(cnt=0;cnt<ROT_PLAYER_MAX;cnt++)
		lpChrPlayers[cnt] = 0;

	return SelectRotPlayer( 0 );
}

//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ Á¾·á
int	CloseRotPlayer()
{
	int cnt;
	for(cnt=0;cnt<ROT_PLAYER_MAX;cnt++)
		if ( lpChrPlayers[cnt] ) {
			delete	lpChrPlayers[cnt];
			lpChrPlayers[cnt] = 0;
		}

	return TRUE;
}

//È¸Àü ÇÃ·¹ÀÌ¾î Æ÷ÀÎÅÍ ¼±ÅÃ
smCHAR *SelectRotPlayer( smCHAR *lpCurrentChar )
{
	int rnd;
	smCHAR *lpChar;

	rnd = rand()%ROT_PLAYER_MIN;
	lpChar = lpChrPlayers[rnd];
	if ( !lpChar ) {
		lpChar = new smCHAR;
		lpChrPlayers[rnd] = lpChar;
	}

	if ( !lpChar ) return NULL;


	if ( lpCurrentChar && lpCurrentChar!=lpChar ) {
		memcpy( lpChar , lpCurrentChar , sizeof(smCHAR) );
	}

	if ( !lpCurrentChar && lpChar ) {
		ZeroMemory( lpChar , sizeof(smCHAR) );
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
	DWORD	*lpStartFunc;
	int		cnt,len;
	DWORD	dwChkSum;

	dwStartFunc = (DWORD)Code_VRamBuffOpen;
	dwEndFunc = (DWORD)Check_PlaySubReleaseFunc;
	len = (dwEndFunc-dwStartFunc)>>2;

	dwChkSum = 0;

	lpStartFunc = (DWORD *)dwStartFunc;

	if ( len<0 ) return FALSE;

	for( cnt=0;cnt<len;cnt++ ) {
		dwChkSum+= (lpStartFunc[cnt]*((cnt&0x3FF)+1));
	}

	dwPlaySub_CheckResult = dwChkSum;

	if ( PLAYSUB_CHECKRESULT!=dwChkSum ) {
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
int	XorCheckPotion( sCHK_POTION_COUNT *ChkPotion )
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

	XorCheckPotion( &sPotionTotalCount );

	GetInvenPotionCount( &ChkPotionCount );

	if ( ChkPotionCount.Pl101>sPotionTotalCount.Pl101 )
		CompareError ++;
	if ( ChkPotionCount.Pl102>sPotionTotalCount.Pl102 )
		CompareError ++;
	if ( ChkPotionCount.Pl103>sPotionTotalCount.Pl103 )
		CompareError ++;
	if ( ChkPotionCount.Pl104>sPotionTotalCount.Pl104 )
		CompareError ++;

	if ( ChkPotionCount.Pm101>sPotionTotalCount.Pm101 )
		CompareError ++;
	if ( ChkPotionCount.Pm102>sPotionTotalCount.Pm102 )
		CompareError ++;
	if ( ChkPotionCount.Pm103>sPotionTotalCount.Pm103 )
		CompareError ++;
	if ( ChkPotionCount.Pm104>sPotionTotalCount.Pm104 )
		CompareError ++;

	if ( ChkPotionCount.Ps101>sPotionTotalCount.Ps101 )
		CompareError ++;
	if ( ChkPotionCount.Ps102>sPotionTotalCount.Ps102 )
		CompareError ++;
	if ( ChkPotionCount.Ps103>sPotionTotalCount.Ps103 )
		CompareError ++;
	if ( ChkPotionCount.Ps104>sPotionTotalCount.Ps104 )
		CompareError ++;

		int cnt1,cnt2;
		cnt1 = ChkPotionCount.Pl101+ChkPotionCount.Pl102+ChkPotionCount.Pl103+ChkPotionCount.Pl104+
			ChkPotionCount.Pm101+ChkPotionCount.Pm102+ChkPotionCount.Pm103+ChkPotionCount.Pm104+
			ChkPotionCount.Ps101+ChkPotionCount.Ps102+ChkPotionCount.Ps103+ChkPotionCount.Ps104;

		cnt2 = sPotionTotalCount.Pl101+sPotionTotalCount.Pl102+sPotionTotalCount.Pl103+sPotionTotalCount.Pl104+
			sPotionTotalCount.Pm101+sPotionTotalCount.Pm102+sPotionTotalCount.Pm103+sPotionTotalCount.Pm104+
			sPotionTotalCount.Ps101+sPotionTotalCount.Ps102+sPotionTotalCount.Ps103+sPotionTotalCount.Ps104;

	if ( CompareError ) {
		//¹°¾à ¿À·ù ¹ß°ß
		//ÇØÅ· ½ÃµµÇÑ À¯Àú ÀÚµ¿ ½Å°í
		SendSetHackUser3( 1530 , CompareError , cnt1-cnt2  );
	}

	tXorPotionCount = dwPlayTime&0xFFFFFFF;
	memcpy( &sPotionTotalCount , &ChkPotionCount , sizeof(sCHK_POTION_COUNT) );
	XorCheckPotion( &sPotionTotalCount );

	return CompareError;
}



//¹°¾à °¹¼ö »õ·Î ¼³Á¤
int	ResetPotion()
{
	GetInvenPotionCount( &sPotionTotalCount );
	tXorPotionCount = dwPlayTime&0xFFFFFFF;
	XorCheckPotion( &sPotionTotalCount );

	return TRUE;
}

//¹°¾à °¹¼ö »õ·Î ¼³Á¤
int	ResetPotion2()
{
	int cnt1,cnt2;
	sCHK_POTION_COUNT	ChkPotionCount;

	XorCheckPotion( &sPotionTotalCount );
	GetInvenPotionCount( &ChkPotionCount );

	//¾ÆÀÌÅÛ ¹°¾à ÀÔ¼ö
	cnt1 = ChkPotionCount.Pl101+ChkPotionCount.Pl102+ChkPotionCount.Pl103+ChkPotionCount.Pl104+
			ChkPotionCount.Pm101+ChkPotionCount.Pm102+ChkPotionCount.Pm103+ChkPotionCount.Pm104+
			ChkPotionCount.Ps101+ChkPotionCount.Ps102+ChkPotionCount.Ps103+ChkPotionCount.Ps104;

	cnt2 = sPotionTotalCount.Pl101+sPotionTotalCount.Pl102+sPotionTotalCount.Pl103+sPotionTotalCount.Pl104+
			sPotionTotalCount.Pm101+sPotionTotalCount.Pm102+sPotionTotalCount.Pm103+sPotionTotalCount.Pm104+
			sPotionTotalCount.Ps101+sPotionTotalCount.Ps102+sPotionTotalCount.Ps103+sPotionTotalCount.Ps104;

	if ( cnt1>(cnt2+TransRecvItem.Item.PotionCount) )
		return TRUE;

	memcpy( &sPotionTotalCount , &ChkPotionCount , sizeof(sCHK_POTION_COUNT) );
	tXorPotionCount = dwPlayTime&0xFFFFFFF;
	XorCheckPotion( &sPotionTotalCount );

	return FALSE;
}

//¾ÆÀÌÅÛÀÌ ¹°¾àÀÎ °æ¿ì ±â·Ï
int	IncPotionCount( sCHK_POTION_COUNT *ChkPotion , sITEMINFO *lpItem ) 
{
	int	PotionCount;

	PotionCount = lpItem->PotionCount;

	if ( !PotionCount ) return FALSE;

	if(lpItem->CODE==(sinPL1|sin01) ) 
		ChkPotion->Pl101+=PotionCount;
	if(lpItem->CODE==(sinPL1|sin02) ) 
		ChkPotion->Pl102+=PotionCount;
	if(lpItem->CODE==(sinPL1|sin03) ) 
		ChkPotion->Pl103+=PotionCount;
	if(lpItem->CODE==(sinPL1|sin04) ) 
		ChkPotion->Pl104+=PotionCount;


	if(lpItem->CODE==(sinPM1|sin01) ) 
		ChkPotion->Pm101+=PotionCount;
	if(lpItem->CODE==(sinPM1|sin02) ) 
		ChkPotion->Pm102+=PotionCount;
	if(lpItem->CODE==(sinPM1|sin03) ) 
		ChkPotion->Pm103+=PotionCount;
	if(lpItem->CODE==(sinPM1|sin04) ) 
		ChkPotion->Pm104+=PotionCount;


	if(lpItem->CODE==(sinPS1|sin01) ) 
		ChkPotion->Ps101+=PotionCount;
	if(lpItem->CODE==(sinPS1|sin02) ) 
		ChkPotion->Ps102+=PotionCount;
	if(lpItem->CODE==(sinPS1|sin03) ) 
		ChkPotion->Ps103+=PotionCount;
	if(lpItem->CODE==(sinPS1|sin04) ) 
		ChkPotion->Ps104+=PotionCount;

	return TRUE;
}


//ÀÎº¥Åä¸®¿Í °Å·¡Ã¢¿¡¼­ ¹°¾àÀ» Ã£¾î °¹¼ö ±â·Ï
int GetInvenPotionCount( sCHK_POTION_COUNT *ChkPotion )
{
	int cnt;

	ZeroMemory( ChkPotion , sizeof(sCHK_POTION_COUNT) );

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItem[cnt].Flag){
			IncPotionCount(  ChkPotion , &cInvenTory.InvenItem[cnt].sItemInfo );
		}
	}

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItemTemp[cnt].Flag){
			IncPotionCount(  ChkPotion , &cInvenTory.InvenItemTemp[cnt].sItemInfo );
		}
	}

	if ( cTrade.OpenFlag ) {
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( sTrade.TradeItem[cnt].Flag ) {
				IncPotionCount(  ChkPotion , &sTrade.TradeItem[cnt].sItemInfo );
			}
		}
	}

	if ( MouseItem.Flag )
		IncPotionCount( ChkPotion , &MouseItem.sItemInfo );

	return TRUE;
}

int	GetItemCheckCode( sITEMINFO *lpItem )
{
	if ( lpItem->CODE<sinPM1 && lpItem->ItemName[0] ) {
		return lpItem->ItemHeader.dwChkSum;
	}
	return 0;
}


//ÀÎº¥Åä¸®ÀÇ ¸ðµç ¾ÆÀÌÅÛÀ» È®ÀÎÇÏ¿© ÄÚµå¸¦ ¹ÝÈ¯
DWORD GetInvenItemCode()
{
	int cnt;

	DWORD	dwCode;


	dwCode = 0;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItem[cnt].Flag ){
			dwCode+=GetItemCheckCode( &cInvenTory.InvenItem[cnt].sItemInfo );
		}
	}

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItemTemp[cnt].Flag ){
			dwCode+=GetItemCheckCode(&cInvenTory.InvenItemTemp[cnt].sItemInfo);
		}
	}

	if ( cTrade.OpenFlag ) {
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( sTrade.TradeItem[cnt].Flag ) {
				dwCode+=GetItemCheckCode( &sTrade.TradeItem[cnt].sItemInfo );
			}
		}
	}


	if ( MouseItem.Flag ) {
		dwCode +=  GetItemCheckCode( &MouseItem.sItemInfo );
	}

	if ( cWareHouse.OpenFlag ) {
		for( cnt=0;cnt<100;cnt++) {
			if ( sWareHouse.WareHouseItem[cnt].Flag ) {
				dwCode +=  GetItemCheckCode( &sWareHouse.WareHouseItem[cnt].sItemInfo );
			}
		}
	}

	if ( cCraftItem.OpenFlag ) {
		for( cnt=0;cnt<15;cnt++) {
			if ( sCraftItem.CraftItem[cnt].Flag ) {
				dwCode += GetItemCheckCode( &sCraftItem.CraftItem[cnt].sItemInfo );
			}
		}
	}

	if ( cAging.OpenFlag ) {
		for( cnt=0;cnt<15;cnt++) {
			if ( sAgingItem.AgingItem[cnt].Flag ) {
				dwCode += GetItemCheckCode( &sAgingItem.AgingItem[cnt].sItemInfo );
			}
		}
	}

	// pluto Á¦·Ã
	if ( SmeltingItem.OpenFlag )
	{
		for( cnt=0;cnt<5;cnt++)
		{
			if ( S_smeltingItem.SmeltingItem[cnt].Flag )
			{
				dwCode += GetItemCheckCode( &S_smeltingItem.SmeltingItem[cnt].sItemInfo );
			}
		}
	}

	// pluto Á¦ÀÛ
	if( ManufactureItem.m_OpenFlag )
	{
		for( cnt = 0; cnt < 4; cnt++ )
		{
			if( g_sManufactureItem.ManufactureItem[cnt].Flag )
			{
				dwCode += GetItemCheckCode( &g_sManufactureItem.ManufactureItem[cnt].sItemInfo );
			}
		}
	}
	//¹«±â¸¦ ÆÈ¶§ , ¹Í½ºÃÄ ¿¡ÀÌÂ¡ Á¾·á½Ã

	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã¢ÀÌ ¿­·ÁÀÖÀ»½Ã...
	if( cMixtureReset.OpenFlag )
	{
		for( cnt=0; cnt<2; cnt++ )
		{
			if( sMixtureResetItem.MixtureResetItem[cnt].Flag )
			{
				dwCode += GetItemCheckCode( &sMixtureResetItem.MixtureResetItem[cnt].sItemInfo );
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
	dwTotal_InvenItemCode = dwPlayTime^dwTotal_InvenItemCode;
	dwTotal_InvenItemPlayTime = dwPlayTime;

	return TRUE;
}

//ÀÌº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå¿¡¼­ Æ¯Á¤ ¾ÆÀÌÅÛ Á¦°Å
int SubInvenItemCode( sITEMINFO *lpItem )
{
	DWORD	dwCode;

	dwCode = dwTotal_InvenItemCode^dwTotal_InvenItemPlayTime;
	dwTotal_InvenItemCode = dwCode - GetItemCheckCode( lpItem );

	dwTotal_InvenItemCode = dwPlayTime^dwTotal_InvenItemCode;
	dwTotal_InvenItemPlayTime = dwPlayTime;

	return TRUE;
}


//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ ÄÚµå È®ÀÎ
int	CheckInvenItemCode()
{
	DWORD dwCode;
	smTRANS_COMMAND	smTransCommand;

	dwCode = GetInvenItemCode();

	if ( dwCode!=(dwTotal_InvenItemCode^dwTotal_InvenItemPlayTime) ) {
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.code = smTRANSCODE_SET_BLACKLIST3;
		smTransCommand.WParam = 7010;
		smTransCommand.LParam = dwTotal_InvenItemCode^dwTotal_InvenItemPlayTime;
		smTransCommand.SParam = dwCode;
		if ( smWsockDataServer ) {
			smWsockDataServer->Send( (char *)&smTransCommand , sizeof(smTRANS_COMMAND) , TRUE );
		}
	}

	dwTotal_InvenItemCode = dwPlayTime^dwCode;
	dwTotal_InvenItemPlayTime = dwPlayTime;

	return TRUE;
}


void CheckPlaySubFunc2()
{
	if ( dwPlaySub_CheckResult!=PLAYSUB_CHECKRESULT ) {
		SendSetHackUser2( 1120 , dwPlaySub_CheckResult );
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
	int Life,Mana,Stamina;
	smTRANS_COMMAND	smTransCommand;


	Life = EnergyGraphErrorCount[0] - EnergyGraphErrorCount[3];
	Mana = EnergyGraphErrorCount[1] - EnergyGraphErrorCount[3];
	Stamina = EnergyGraphErrorCount[2] - EnergyGraphErrorCount[3];

	//if ( Life>3 || Mana>3 || Stamina>3 ) {

	if ( Life>0 || Mana>0 || Stamina>0 ) {
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_CLIENT_ERROR;
		smTransCommand.WParam = 0;

		smTransCommand.LParam = Life;
		smTransCommand.SParam = Mana;
		smTransCommand.EParam = Stamina;

		if ( smWsockDataServer )
			smWsockDataServer->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
	}

	EnergyGraphErrorCount[3] = lpCurPlayer->pX;
	EnergyGraphErrorCount[0] = EnergyGraphErrorCount[3]+Life;
	EnergyGraphErrorCount[1] = EnergyGraphErrorCount[3]+Life;
	EnergyGraphErrorCount[2] = EnergyGraphErrorCount[3]+Life;

	return TRUE;
}

//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
int	ResetEnergyGraph( int Num )
{
	int	Pos;

	Pos = lpCurPlayer->pZ;

	switch( Num ) {
		case 0:
			//LIFE
			sChkLife[0] = Pos;
			sChkLife[1] = Pos^(-1);
			break;

		case 1:
			//MANA
			sChkMana[0] = Pos;
			sChkMana[1] = Pos^(-1);
			break;

		case 2:
			//STAMINA
			sChkStamina[0] = Pos;
			sChkStamina[1] = Pos^(-1);
			break;

		case 3:
		case 4:
			//ALL
			sChkLife[0] = Pos;
			sChkLife[1] = Pos^(-1);
			sChkMana[0] = Pos;
			sChkMana[1] = Pos^(-1);
			sChkStamina[0] = Pos;
			sChkStamina[1] = Pos^(-1);
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

	Last_Grp = sChkLife[0]^sChkLife[1];
	Grp = sinGetLife();
	if ( Last_Grp!=-1 ) {
		cnt = Grp-Last_Grp;
		if ( cnt>15 ) {
			EnergyGraphErrorCount[0] ++;
		}
	}
	sChkLife[0] = Pos;
	sChkLife[1] = Pos^Grp;

	Last_Grp = sChkMana[0]^sChkMana[1];
	Grp = sinGetMana();
	if ( Last_Grp!=-1 ) {
		cnt = Grp-Last_Grp;
		if ( cnt>15 ) {
			EnergyGraphErrorCount[1] ++;
		}
	}
	sChkMana[0] = Pos;
	sChkMana[1] = Pos^Grp;

	Last_Grp = sChkStamina[0]^sChkStamina[1];
	Grp = sinGetStamina();
	if ( Last_Grp!=-1 ) {
		cnt = Grp-Last_Grp;
		if ( cnt>50 ) {
			EnergyGraphErrorCount[2] ++;
		}
	}
	sChkStamina[0] = Pos;
	sChkStamina[1] = Pos^Grp;

	return TRUE;
}


//ÀÎº¥Åä¸®ÀÇ ÁöÁ¤ÇÑ ¾ÆÀÌÅÛÀ» Ã£¾Æ °Ë»öÇÏ¿© Ã£´Â´Ù
sITEMINFO *FindInvenItem( DWORD dwItemCode , DWORD dwHead , DWORD dwChkSum )
{
	int cnt;
	sITEMINFO *lpItem;

	if ( (dwItemCode&sinITEM_MASK1)==(sinPM1&sinITEM_MASK1) ) return NULL;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItem[cnt].Flag ){
			lpItem = &cInvenTory.InvenItem[cnt].sItemInfo;
			if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
				return lpItem;
			}
		}
	}

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItemTemp[cnt].Flag ){
			lpItem = &cInvenTory.InvenItemTemp[cnt].sItemInfo;
			if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
				return lpItem;
			}
		}
	}

	if ( cTrade.OpenFlag ) {
		for( cnt=0;cnt<MAX_TRADE_ITEM;cnt++) {
			if ( sTrade.TradeItem[cnt].Flag ) {
				lpItem = &sTrade.TradeItem[cnt].sItemInfo;
				if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
					return lpItem;
				}
			}
		}
	}

	if ( MouseItem.Flag ) {
		lpItem = &MouseItem.sItemInfo;
		if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
			return lpItem;
		}
	}

	if ( cWareHouse.OpenFlag ) {
		for( cnt=0;cnt<100;cnt++) {
			if ( sWareHouse.WareHouseItem[cnt].Flag ) {
				lpItem = &sWareHouse.WareHouseItem[cnt].sItemInfo;
				if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
					return lpItem;
				}
			}
		}
	}

	if ( cCraftItem.OpenFlag ) {
		for( cnt=0;cnt<15;cnt++) {
			if ( sCraftItem.CraftItem[cnt].Flag ) {
				lpItem = &sCraftItem.CraftItem[cnt].sItemInfo;
				if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
					return lpItem;
				}
			}
		}
	}

	if ( cAging.OpenFlag ) {
		for( cnt=0;cnt<15;cnt++) {
			if ( sAgingItem.AgingItem[cnt].Flag ) {
				lpItem = &sAgingItem.AgingItem[cnt].sItemInfo;
				if ( lpItem->CODE==dwItemCode && lpItem->ItemHeader.Head==dwHead && lpItem->ItemHeader.dwChkSum==dwChkSum ) { 
					return lpItem;
				}
			}
		}
	}
	// pluto Á¦·Ã
	if( SmeltingItem.OpenFlag )
	{
		for( cnt = 0; cnt < 5; cnt++ )
		{
			if( S_smeltingItem.SmeltingItem[cnt].Flag)
			{
				lpItem = &S_smeltingItem.SmeltingItem[cnt].sItemInfo;
				if( lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum )
				{
					return lpItem;
				}
			}
		}
	}

	// pluto Á¦ÀÛ
	if( ManufactureItem.m_OpenFlag )
	{
		for( cnt = 0; cnt < 4; cnt++ )
		{
			if( g_sManufactureItem.ManufactureItem[cnt].Flag )
			{
				lpItem = &g_sManufactureItem.ManufactureItem[cnt].sItemInfo;
				if( lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum )
				{
					return lpItem;
				}
			}
		}
	}

	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã³¸®
	if( cMixtureReset.OpenFlag )
	{
		for( cnt=0; cnt<2; cnt++ )
		{
			if( sMixtureResetItem.MixtureResetItem[cnt].Flag )
			{
				lpItem = &sMixtureResetItem.MixtureResetItem[cnt].sItemInfo;
				if( lpItem->CODE == dwItemCode && lpItem->ItemHeader.Head == dwHead && lpItem->ItemHeader.dwChkSum == dwChkSum )
					return lpItem;
			}
		}
	}

	return NULL;
}

//ÀÎº¥Åä¸® ¾ÆÀÌÅÛ º¹»ç ¿À·ù
int SendInvenItemError( DWORD dwKind , DWORD dwItemCode , DWORD dwHead , DWORD dwChkSum )
{
	smTRANS_COMMAND	smTransCommand;

	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.code = smTRANSCODE_INVEN_ERR_ITEM;
	smTransCommand.WParam = dwKind;

	smTransCommand.LParam = dwItemCode;
	smTransCommand.SParam = dwHead;
	smTransCommand.EParam = dwChkSum;

	if ( smWsockDataServer )
		smWsockDataServer->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );

	return TRUE;
}

int	GetTotalInvenMoney()
{
	int Money;

	Money = lpCurPlayer->smCharInfo.Money;

	if ( cTrade.OpenFlag && sTrade.Money ) 
		Money += sTrade.Money-193;

	if ( cWareHouse.OpenFlag ) 
		Money += sWareHouse.Money -2023;

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
	dwTotal_CheckMoney = dwPlayTime^dwTotal_CheckMoney;
	dwTotal_CheckMoneyTime = dwPlayTime^TOTAL_CHECK_MONEY_MASK;

	return TRUE;
}


//ÀÎº¥Åä¸® °Ë»ç¿ëµ· Ãß°¡
int	AddInvenMoney( int Money )
{
	int	m;

	m = dwTotal_CheckMoney^(dwTotal_CheckMoneyTime^TOTAL_CHECK_MONEY_MASK);
	m += Money;
	if ( m<0 ) m=0;

	dwTotal_CheckMoney = dwPlayTime^m;
	dwTotal_CheckMoneyTime = dwPlayTime^TOTAL_CHECK_MONEY_MASK;

	return TRUE;
}

//ÀÎº¥Åä¸® µ· °Ë»ç
int	CheckInvenMoney()
{
	DWORD dwCode;
	smTRANS_COMMAND	smTransCommand;

	if (  SkillMasterFlag || cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag)  // ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ã¢ Ãß°¡
	{
		NChkMode = TRUE;
		return TRUE;
	}

	if ( NChkMode ) {
		NChkMode = 0;
		ResetInvenMoney();
		if ( NChkMode==0 ) return TRUE;
		dwCode = 0;
	}
	else {
		dwCode = GetTotalInvenMoney();
	}

	if ( dwCode!=(dwTotal_CheckMoney^(dwTotal_CheckMoneyTime^TOTAL_CHECK_MONEY_MASK)) ) {
		smTransCommand.size = sizeof( smTRANS_COMMAND );
		smTransCommand.code = smTRANSCODE_INVEN_ERR_MONEY;
		smTransCommand.WParam = 0;
		smTransCommand.LParam = dwCode;
		smTransCommand.SParam = (dwTotal_CheckMoney^(dwTotal_CheckMoneyTime^TOTAL_CHECK_MONEY_MASK));
		smTransCommand.EParam = 0;
		if ( smWsockDataServer ) {
			smWsockDataServer->Send( (char *)&smTransCommand , sizeof(smTRANS_COMMAND) , TRUE );
		}
	}

	dwTotal_CheckMoney = dwPlayTime^dwCode;
	dwTotal_CheckMoneyTime = dwPlayTime^TOTAL_CHECK_MONEY_MASK;

	return TRUE;
}



//ÀÎº¥Åä¸®ÀÇ ÀåÂø ¾ÆÀÌÅÛ Á¤º¸ ¼­¹ö¿¡ º¸³»±â
int SendUsingItemListToServer()
{

	int cnt;
	sITEMINFO *lpItem;

	TRANS_USINGITEM	TransUsingItem;

	ZeroMemory( &TransUsingItem , sizeof(TRANS_USINGITEM) );

	TransUsingItem.size = sizeof(TRANS_USINGITEM);
	TransUsingItem.code = smTRANSCODE_INVEN_POSITION;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].ItemPosition ) {
			lpItem = &cInvenTory.InvenItem[cnt].sItemInfo;
			if ( lpItem->ItemName[0] ) {
				if ( TransUsingItem.ItemListCount>=16 ) break;
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

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if(cInvenTory.InvenItemTemp[cnt].Flag && cInvenTory.InvenItemTemp[cnt].ItemPosition ){
			lpItem = &cInvenTory.InvenItemTemp[cnt].sItemInfo;
			if ( lpItem->ItemName[0] ) {
				if ( TransUsingItem.ItemListCount>=16 ) break;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].dwCode = lpItem->CODE;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[0] = cInvenTory.InvenItemTemp[cnt].ItemPosition|0x100;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[1] = lpItem->Damage[1];
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[2] = lpItem->Defence;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[3] = (short)lpItem->fBlock_Rating;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[4] = (short)lpItem->fAbsorb;
				TransUsingItem.ItemList[TransUsingItem.ItemListCount].Performance[5] = lpItem->Attack_Rating;
				TransUsingItem.ItemListCount++;
			}
		}
	}

	smWINSOCK	*lpsmSendSock;

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â
	if ( lpsmSendSock ) {
		return lpsmSendSock->Send( (char *)&TransUsingItem , TransUsingItem.size , TRUE );
	}

	return NULL;
}

//ÀÎº¥Åä¸®ÀÇ ÀåÂø ¾ÆÀÌÅÛ Á¤º¸ ¼­¹ö¿¡ º¸³»±â
int SendUsingItemListToServer2()
{

	int cnt;
	sITEMINFO *lpItem;

	TRANS_USINGITEM	TransUsingItem;

	ZeroMemory( &TransUsingItem , sizeof(TRANS_USINGITEM) );

	TransUsingItem.size = sizeof(TRANS_USINGITEM);
	TransUsingItem.code = smTRANSCODE_INVEN_POSITION2;

	for( cnt=0;cnt<INVENTORY_MAXITEM ; cnt++){ 
		if( cInvenTory.InvenItem[cnt].Flag && cInvenTory.InvenItem[cnt].ItemPosition ) {
			lpItem = &cInvenTory.InvenItem[cnt].sItemInfo;
			if ( lpItem->ItemName[0] ) {
				if ( TransUsingItem.ItemListCount>=16 ) break;
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

	smWINSOCK	*lpsmSendSock;

	lpsmSendSock = GetAreaServerSock();		//º¸³¾ ¼­¹ö±¸ÇÏ±â
	if ( lpsmSendSock ) {
		return lpsmSendSock->Send( (char *)&TransUsingItem , TransUsingItem.size , TRUE );
	}

	return NULL;
}


//½Ã°£ ±¸ÇÏ±â (time_t)
DWORD	GetPlayTime_T()
{
	DWORD dwTime;

	if ( dwTime_ServerT ) {
		dwTime = (dwPlayTime-dwTime_ConnectMS)/1000;
		dwTime = dwTime_ServerT+dwTime;				//ÇöÀçÀÇ Time_T
		return dwTime;
	}

	return NULL;
}


//¸Ó¸®°¡ Ä¿Áö´Â ¸ðµå ¼³Á¤ 
int	ChangeBigHeadMode( int Mode , int Time )
{
	DWORD	dwTime;

	if ( Mode ) 
	{
		if(Mode == 3) // ¹ÚÀç¿ø - 2010 ¿ùµåÄÅ ÀÌº¥Æ® - Ãà±¸°ø Æ÷¼Ç »ç¿ë
		{
			lpCurPlayer->smCharInfo.SizeLevel = 1; // Á¤»ó ¾ó±¼
			ChangeModelSkin( lpCurPlayer , 0 , 6 , 0 , 0 );	// Ãà±¸°ø ¾´ ¾ó±¼
		}
		else
		{
			lpCurPlayer->smCharInfo.SizeLevel = Mode+0x1000;
			ChangeModelSkin( lpCurPlayer , 1 , lpCurPlayer->smCharInfo.SizeLevel+3-0x1000 , 0 , 0 );	//Å«¾ó±¼
		}
		dwTime=GetPlayTime_T();

		if ( dwTime ) 
			lpCurPlayer->smCharInfo.dwEventTime_T = dwTime + Time;	//¸ðµ¨Á¦ÇÑ ½Ã°£
		else
			lpCurPlayer->smCharInfo.dwEventTime_T  = 0;				//½Ã°£Á¦ÇÑ ¾øÀ½ ( ÀçÁ¢ ÇÒ¶§±îÁö )
	}
	else {
		lpCurPlayer->smCharInfo.SizeLevel = 1;
		lpCurPlayer->smCharInfo.dwEventTime_T = 0;
		ChangeModelSkin( lpCurPlayer , 0 , lpCurPlayer->smCharInfo.ChangeJob , 0 , GetFaceHairSkin(lpCurPlayer) );				//Á¤»ó ¾ó±¼ // ¾óÅ«ÀÌ ¿À·ù ¼öÁ¤
	}

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	ReformCharForm();
	SendSetObjectSerial( lpCurPlayer->dwObjectSerial );

	if ( RestartPlayCount<350 ) RestartPlayCount=350;		//Àá½Ãµ¿¾È ¹«Àû (3ÃÊ)

	return TRUE;
}

//°ø°Ý¼Óµµ¸¦ ÇÁ·¹ÀÓ¼Óµµ·Î º¯È¯
int GetAttackSpeedMainFrame( int AttackSpeed )
{
	int cnt;

	cnt = AttackSpeed-6;
	if ( cnt<0 ) cnt=0;
	if ( cnt>6 ) cnt=6;

	return fONE + 32*cnt;
}

//°ø°Ý¼Óµµ¸¦ ÇÁ·¹ÀÓ¼Óµµ·Î º¯È¯
int	GetAttackSpeedFrame( int AttackSpeed , int AddSpeed )
{
	int cnt;

	cnt = GetAttackSpeedMainFrame( AttackSpeed );
	if ( AddSpeed<6 && AddSpeed>0 )
		cnt += AddSpeed*32;

	return (80*cnt)>>FLOATNS;
}

//°ø°Ý¼Óµµ¸¦ ÇÁ·¹ÀÓ¼Óµµ·Î º¯È¯
int	GetAttackSpeedFrame( int AttackSpeed )
{
	int cnt;

	cnt = GetAttackSpeedMainFrame( AttackSpeed );

	return (80*cnt)>>FLOATNS;
}







//Æú´õ ³»ÀÇ ini ÆÄÀÏ ªO¾Æ ÀÐ±â
int LoadFolder_Ini( char *szPath , char *szWildCard , char *szChgExt , int OftSave )
{

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;
	char	szFindPath[64];
	char	szFilePath[64];
	char	*lpPath;

	HANDLE hFindHandle2;
	WIN32_FIND_DATA	fd2;
	char	szFindPath2[64];

	smDPAT	*lpDPatTest[65536];
	int		DPatCount = 0;
	int		cnt;

	lstrcpy( szFindPath , szPath );
	lstrcat( szFindPath , "\\*.*" );


	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle!=INVALID_HANDLE_VALUE ) {
		while(1) {
			if ( (fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0 ) {


				wsprintf( szFindPath2 , "%s\\%s\\%s" , szPath , fd.cFileName , szWildCard );

				hFindHandle2 = FindFirstFile( szFindPath2 , &fd2 );
				if ( hFindHandle2!=INVALID_HANDLE_VALUE ) {
					while(1) {
						wsprintf( szFilePath , "%s\\%s\\%s" , szPath , fd.cFileName , fd2.cFileName );


						if ( OftSave ) {
							//ÀÚÁÖ ¾²±â ±â·Ï

							if ( szChgExt )
								lpPath = ChangeFileExt( szFilePath , szChgExt );
							else
								lpPath = 0;
							
							if ( !lpPath ) lpPath = szFilePath;

							lstrcpy( szFilePath , lpPath );

							LoadOftenMeshPattern(szFilePath);
						}
						else {
							//´Ü¼ø Å×½ºÆ®
							lpDPatTest[DPatCount] = smPattern.LoadCharactor( szFilePath );
							if ( lpDPatTest[DPatCount] ) DPatCount++;
							else {
								DPatCount = DPatCount;
							}
						}

						if ( FindNextFile( hFindHandle2 , &fd2 )==FALSE ) break;
					}
					FindClose( hFindHandle2 );
				}
			}

			//´ÙÀ½ ÆÄÀÏ Ã£À½
			if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		}
		FindClose( hFindHandle );
	}

	for(cnt=0;cnt<DPatCount;cnt++) {
		smDPAT_Delete( lpDPatTest[cnt] );
	}

	return TRUE;
}

sSOD_SCORE	sSodScore;
int			SodScoreFlag = 0;


//sSodScore.dwSoD_NextStageTime

//SoD ¶ó¿îµå ÀÌÆåÆ®
int	Sod_SodEffect( int Round ) { return TRUE; }




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
int	SetSodScoreResult() { return TRUE; }


//SoD ¿¬Ãâ
int	SodScoreMain() { return TRUE; }



//Sod ÆÀ¼øÀ§ ¼³Á¤
int SodTeamSort()
{
	int cnt,cnt1;
	int r1,r2;

	for(cnt=0;cnt<4;cnt++) TeamScoreSort[cnt] = cnt;

	//sSodScore.TeamScore[cnt]

	for( cnt=0;cnt<4;cnt++) {
		for(cnt1=cnt;cnt1<4;cnt1++) {
			r1 = TeamScoreSort[cnt];
			r2 = TeamScoreSort[cnt1];

			if ( sSodScore.TeamScore[r1]<sSodScore.TeamScore[r2] ) {
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
	return FALSE;
}






