
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
#include "skillsub.h"
#include "damage.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "language.h"


#include "damage.h"
#include "AreaServer.h"

#include "srcserver\onserver.h"

extern HWND hwnd;

//필드맵 로딩
int LoadFieldMap( int PosNum , sFIELD *lpField , smSTAGE3D *lpStage );


//무대 세트 클래스다!
smSTAGE3D		*smStage = 0;
smSTAGE3D		*smGameStage[2] = { 0,0 };
sFIELD			*StageField[2] = { 0,0 };


rsRECORD_DBASE	rsRecorder;					//서버에 게임데이타 기록장치

char szGameStageName[2][64];
DWORD			dwPlayTime=0;					//현재의 윈도우 시간 ( 플레이 용 )
DWORD			dwMemError=8080;				//메모리 에러 확인 코드

int	smStageCount = 0;
int SkipNextField = 0;				//다음 필드로 넘어감

int	NightDayTime=0;					//밤낮 구분

int invPlayerArmor = 0;

DWORD	dwBattleTime =0;				//최근 전투 시간
DWORD	dwBattleQuitTime =0;			//전투중 종료 하려 할때
DWORD	dwCloseBoxTime = 0;				//작업창 종료 메세지

int		RestartPlayCount = 0;					//재시작 무적 시작 카운트

DWORD SkillFalconPoint;					//팔콘 스킬 포인트 저장용

int InvArmorFileCode[] =
{ 0 ,9,1, 5 };

int	DarkLevel = 0;					//어둠 값
int	BackColor_R = 0;				//배경색 R
int	BackColor_G = 0;				//배경색 G
int	BackColor_B = 0;				//배경색 B

int	BackColor_mR = 0;				//배경색 변경값 R
int	BackColor_mG = 0;				//배경색 변경값 G
int	BackColor_mB = 0;				//배경색 변경값 B

int	DarkLevel_Fix = 0;				//어둠 값 고정

int	DarkLightRange = 180*fONE;		//어두울때 조명 범위

DWORD	dwGameWorldTime = 0;			//게임의 월드시간
DWORD	dwGameHour = 12;					//게임의 시
DWORD	dwGameMin  = 0;					//게임의 분
DWORD	dwGameTimeOffset = 0;

int DarkLight = 0;

//날씨 관련
int WeatherMode = 0;
int Thunder = 0;

//PK모드
int	PkMode = 0;

int DispApp_SkillPoint = 0;			//설정된 시간적용 스킬 포인트

int rcnt2;

int modelX = 255;
int modelZ = 512;
int wflag = 0;
int wrad = 0;

DWORD	UseEtherCoreCode = 0;			//에티르 코어 사용

int	RecordFailCount =0;				//저장 실피 카운터

//공격 받았을때 회피 카운터
int	Record_BlockCount = 0;
int	Record_DefenceCount = 0;
int Record_RecvDamageCount = 0;
int Record_TotalRecvDamageCount = 0;


//오토 마우스 감지용
DWORD	AutoMouse_LastCheckTime = 0;
DWORD	AutoMouse_StartCheckTime = 0;
int		AutoMouse_WM_Count = 0;
int 	AutoMouse_Distance = 0;
int		AutoMouse_Attack_Count = 0;

//화살
//smPAT3D *PatTest;

//화살
smPAT3D *PatArrow;
//방패 이펙트
smPAT3D *PatEffectShield =0;
//스파크쉴드 이펙트
smPAT3D *PatSparkShield =0;
//디바인인할레이션
smPAT3D *PatDivineShield =0;
//가들리 쉴드
smPAT3D *PatGodlyShield =0;
//블레스캐슬 왕관
smPAT3D *PatCrown =0;

//그림자
smPAT3D *PatShadow[SHADOW_PAT_MAX] = { 0,0,0,0 };

//######################################################################################
//작 성 자 : 오 영 석
smPAT3D *PatIceFoot[ ICE_FOOT_PAT_MAX ];
//######################################################################################

int	Moving_DblClick = 0;		//마우스 이동 첵크 더블 클릭
POINT3D	MousePos_DblClick;

POINT3D	TracePos_DblClick;
int TraceMode_DblClick = 0;

int	ActionGameMode = 0;	//액션게임 모드


//smCHAR	chrPlayer;
smCHAR		*lpCurPlayer = 0;

DWORD	dwRotPlayDelayTime = 0;			//주인공 포인터 변경 딜레이

/*
//회전 플레이어 포인터 선택
smCHAR *SelectRotPlayer( smCHAR *lpCurrentChar );
//회전 플레이어 포인터 종료
int	CloseRotPlayer();
//회전 플레이어 포인터 초기화
smCHAR *InitRotPlayer();

lpCurPlayer = InitRotPlayer();

*/


scITEM	scItems[ DISP_ITEM_MAX ];			//ITEM

smCHAR	chrOtherPlayer[OTHER_PLAYER_MAX];

smCHAR_INFO	smCharInfo_Back;			//NPC 설치시 기본 데이타 임시 저장


int	MatEachBox;				//얼굴 확대 테두리 그림 메트리얼
int	MatEachMenuBox;			//메뉴 버튼 테두리
int MatEnergyBox[2];		//에너지 메트리얼
int MatLight[4];				//조명 빛 이미지 메트리얼 //glow.bmp

int	MatEachTradeButton[2];	//메뉴 아이템 교환 버튼 
int MatMonBroodImage[8];	//몬스터 종족 이미지

//동료 회복 예약값
smTRANS_COMMAND	sPartyPosion;
DWORD			dwPartyPosionTime;

void*	lpDDS_ParTradeButton[4][2];			//메뉴및 트레이드 버튼
void*	lpDDS_ParTradeToolTip[4];			//메뉴및 트레이드 툴팁


//불꽃 애니메이션
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
//char *szShadowImageFile = "image\\shadow.tga";

//######################################################################################
//작 성 자 : 오 영 석
char *g_IceFootImageFileBuff[ ICE_FOOT_PAT_MAX ] =
{
	"image\\mk-r.tga",
	"image\\mk-l.tga",
};
//######################################################################################



//배경 로더 초기화
void InitStageLoader()
{
	szGameStageName[0][0] = 0;
	szGameStageName[1][0] = 0;
	smGameStage[0]=0;
	smGameStage[1]=0;
}

//배경을 읽어 온다
smSTAGE3D *LoadStageFromField( sFIELD *lpField , sFIELD *lpSecondField )
{
	smSTAGE3D *lpstg;
	char *szStageFile;
	int cnt;
	int	Bip;
	char	szBuff[128];

	szStageFile = lpField->szName;

	if ( lpField==StageField[0] ) return smGameStage[0];
	if ( lpField==StageField[1] ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// 라이트 벡터
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast		= smConfig.MapContrast;

	//배경을 새 쓰레드에 통하여 로드 한다
	AddLoaderStage( lpstg , szStageFile );

	if ( !lpstg->StageObject ) {
		//알수 없는 에러
		//클라이언트 로그 파일 기록
		wsprintf( szBuff , "Stage Loading Error ( %s )",szStageFile );
		Record_ClinetLogFile( szBuff );
		delete lpstg;
		return NULL;
	}

	//보조 오브젝트 추가
	for( cnt=0;cnt<lpField->StgObjCount;cnt++ ) {
		Bip = lpField->GetStageObjectName( cnt , szBuff );
		if ( szBuff[0] ) {
			if ( Bip )
				lpstg->StageObject->AddObjectFile( szBuff , szBuff );
			else
				lpstg->StageObject->AddObjectFile( szBuff );
		}
	}

	if ( lpSecondField && lpSecondField==StageField[0] ) {
		if ( smGameStage[1] ) delete smGameStage[1];
		smGameStage[1] = lpstg;
		StageField[1] = lpField;
		LoadFieldMap( 1, lpField , lpstg );				//필드맵 로딩
	}
	else {
		if ( !lpSecondField || lpSecondField==StageField[1] ) {
			if ( smGameStage[0] ) delete smGameStage[0];
			smGameStage[0] = lpstg;
			StageField[0] = lpField;
			LoadFieldMap( 0, lpField , lpstg );			//필드맵 로딩
		}
	}


	if ( StageField[0] ) lstrcpy( szGameStageName[0] , StageField[0]->szName );
	if ( StageField[1] ) lstrcpy( szGameStageName[1] , StageField[1]->szName );


	DWORD	dwTime = GetCurrentTime();

	if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime ) dwLastRecvGameServerTime=dwTime;
	if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime ) dwLastRecvGameServerTime2 = dwTime;
	if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime ) dwLastRecvGameServerTime3 = dwTime;
	if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime ) dwLastRecvGameServerTime4 = dwTime;

	if ( AreaServerMode ) {
		//지연 서버 데드락확인 시간 보정
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->dwDeadLockTime = dwTime;
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->dwDeadLockTime = dwTime;
	}

	return lpstg;
}


//배경을 읽어 온다 
smSTAGE3D *LoadStageFromField2( sFIELD *lpField , sFIELD *lpSecondField )
{
	smSTAGE3D *lpstg;
	char *szStageFile;
	int cnt;
	int Bip;
	char szBuff[128];


	szStageFile = lpField->szName;

	if ( lpField==StageField[0] ) return smGameStage[0];
	if ( lpField==StageField[1] ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// 라이트 벡터
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast		= smConfig.MapContrast;

	//배경을 로드 한다
//	smSTAGE3D_ReadASE( szStageFile , lpstg );
//	lpstg->Head = TRUE;

	//배경을 새 쓰레드에 통하여 로드 한다
	AddLoaderStage( lpstg , szStageFile );

	if ( !lpstg->StageObject ) {
		//알수 없는 에러
		//클라이언트 로그 파일 기록
		wsprintf( szBuff , "Stage Loading Error ( %s )",szStageFile );
		Record_ClinetLogFile( szBuff );
		delete lpstg;
		return NULL;
	}

	//보조 오브젝트 추가
	for( cnt=0;cnt<lpField->StgObjCount;cnt++ ) {
		Bip = lpField->GetStageObjectName( cnt , szBuff );
		if ( szBuff[0] ) {
			if ( Bip )
				lpstg->StageObject->AddObjectFile( szBuff , szBuff );
			else
				lpstg->StageObject->AddObjectFile( szBuff );
		}
	}

	if ( lpSecondField && lpSecondField==StageField[0] ) {
		if ( smGameStage[1] ) delete smGameStage[1];
		smGameStage[1] = lpstg;
		StageField[1] = lpField;
		//필드맵 로딩
		LoadFieldMap( 1, lpField , lpstg );
	}
	else {
		if ( !lpSecondField || lpSecondField==StageField[1] ) {
			if ( smGameStage[0] ) delete smGameStage[0];
			smGameStage[0] = lpstg;
			StageField[0] = lpField;
			//필드맵 로딩
			LoadFieldMap( 0, lpField , lpstg );
		}
	}

	if ( StageField[0] ) lstrcpy( szGameStageName[0] , StageField[0]->szName );
	if ( StageField[1] ) lstrcpy( szGameStageName[1] , StageField[1]->szName );

	DWORD	dwTime = GetCurrentTime();
	if ( dwLastRecvGameServerTime && dwLastRecvGameServerTime<dwTime ) dwLastRecvGameServerTime=dwTime;
	if ( dwLastRecvGameServerTime2 && dwLastRecvGameServerTime2<dwTime ) dwLastRecvGameServerTime2 = dwTime;
	if ( dwLastRecvGameServerTime3 && dwLastRecvGameServerTime3<dwTime ) dwLastRecvGameServerTime3 = dwTime;
	if ( dwLastRecvGameServerTime4 && dwLastRecvGameServerTime4<dwTime ) dwLastRecvGameServerTime4 = dwTime;

	if ( AreaServerMode ) {
		//지연 서버 데드락확인 시간 보정
		if ( lpWSockServer_Area[0] ) lpWSockServer_Area[0]->dwDeadLockTime = dwTime;
		if ( lpWSockServer_Area[1] ) lpWSockServer_Area[1]->dwDeadLockTime = dwTime;
	}

	return lpstg;
}

//배경을 읽어 온다
smSTAGE3D *LoadStage( char *szStageFile )
{
	smSTAGE3D *lpstg;

	if ( lstrcmpi( szGameStageName[0] , szStageFile )==0 ) return smGameStage[0];
	if ( lstrcmpi( szGameStageName[1] , szStageFile )==0 ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// 라이트 벡터
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast	= smConfig.MapContrast;

	//배경을 새 쓰레드에 통하여 로드 한다
	AddLoaderStage( lpstg , szStageFile );

	smStageCount = (smStageCount+1)&1;

	if ( smGameStage[smStageCount] ) delete smGameStage[smStageCount];

	smGameStage[smStageCount] = lpstg;
	//ReadTextures();
	return lpstg;
}

//배경을 읽어 온다
smSTAGE3D *LoadStage2( char *szStageFile )
{
	smSTAGE3D *lpstg;

	if ( lstrcmpi( szGameStageName[0] , szStageFile )==0 ) return smGameStage[0];
	if ( lstrcmpi( szGameStageName[1] , szStageFile )==0 ) return smGameStage[1];

	lpstg = new smSTAGE3D;

	// 라이트 벡터
	lpstg->VectLight.x = smConfig.MapLightVector.x;
	lpstg->VectLight.y = smConfig.MapLightVector.y;
	lpstg->VectLight.z = smConfig.MapLightVector.z;

	lpstg->Bright		= smConfig.MapBright;
	lpstg->Contrast	= smConfig.MapContrast;

	//배경을 새 쓰레드에 통하여 로드 한다
	smSTAGE3D_ReadASE( szStageFile , lpstg );
	lpstg->Head = TRUE;

	smStageCount = (smStageCount+1)&1;

	if ( smGameStage[smStageCount] ) delete smGameStage[smStageCount];

	smGameStage[smStageCount] = lpstg;
	//ReadTextures();
	return lpstg;
}


int PlayCounter = 0;

char PlayerName[64] = { 0,0,0,0 };


void InitStage()
{
//	int cnt;

	szGameStageName[0][0] = 0;
	szGameStageName[1][0] = 0;

/*
	smStage = new smSTAGE3D;

	// 라이트 벡터
	smStage->VectLight.x = smConfig.MapLightVector.x;
	smStage->VectLight.y = smConfig.MapLightVector.y;
	smStage->VectLight.z = smConfig.MapLightVector.z;

	smStage->Bright		= smConfig.MapBright;
	smStage->Contrast	= smConfig.MapContrast;


//	smStage = smSTAGE3D_ReadASE( smConfig.szFile_Stage  );
	smStage = smSTAGE3D_ReadASE( smConfig.szFile_Stage , smStage );
*/
//	smStage = LoadStage( smConfig.szFile_Stage );
//	smStage = LoadStage( "field\\forest.ASE" );
/*
	smStage = LoadStage( "field\\village.ASE" );
	smStage = LoadStage( "field\\forest(attach).ASE" );
*/
//	smStage = LoadStage2( "field\\village.ASE" );
//	smStage = LoadStage2( smConfig.szFile_Stage  );
//	smStage = LoadStage2( "field\\forest(attach).ASE" );

	//필드 맵 초기화
	InitFieldMap();


	 InitField();

	LoadFieldMap( 0, StageField[0] , smGameStage[0] );

/*
	for( cnt=0;cnt<smConfig.StageObjCnt;cnt++) {
		smStage->StageObject->AddObjectFile( smConfig.szFile_StageObject[cnt] );
	}
*/
}


void CloseStage()
{

	//if( smStage ) delete smStage;

	if ( smGameStage[1] ) delete smGameStage[1];
	if ( smGameStage[0] ) delete smGameStage[0];

}


struct _DRAW_LIGHTLIST {
	smLIGHT3D	*smLight;
	int			x,y,z;
};


_DRAW_LIGHTLIST	DrawLight[64];
_DRAW_LIGHTLIST	DrawLightSmall[64];
int DrawLightCnt;
int DrawLightSmallCnt;


int DrawLightImage(smSTAGE3D* lpStage, int x, int y, int z) { return TRUE; }


int DrawLightImage() { return TRUE; }

int SetStageDynLight(smSTAGE3D *lpStage , int x, int y, int z) { return TRUE; }




int doop = 0;



int DisplayStage(int x , int y, int z, int angX, int angY, int angZ ) { return TRUE; }

//######################################################################################
//작 성 자 : 오 영 석
void CreatePatIceFoot( int Num, int sizeX, int sizeZ ) { }
//######################################################################################

void CreatePatShadow( int Num , int sizeX , int sizeZ ) { }

void CreatePatShadow2(int Num, int sizeX, int sizeZ) {}


//관리자 캐릭터 모드
int AdminCharMode = 0;
//관리자 캐릭터 모드시 상대방에 데이타 송출 금지
int AdminCharDisable = 0;

//캐릭터 파일 리스트
static char szDataDirectory[256];
char szCharFileList[256][256];
int CharFileCnt;
int CharPlayCnt = 0;

//파일 이름으로 부터 디렉토리를 찾아 설정
static char *SetDirectoryFromFile( char *filename )
{
	int len;
	int cnt;

	lstrcpy( szDataDirectory , filename );

	len = lstrlen( szDataDirectory );

	for( cnt=len; cnt>=0 ; cnt-- )
		if ( szDataDirectory[cnt]=='\\' ) break;

	szDataDirectory[cnt+1] = NULL;

	return szDataDirectory;
}


//캐릭터 파일 리스트 설정
int SetCharFileList( char *szCharFile )
{
	CharFileCnt = 0;
	char szFindPath[256];

	SetDirectoryFromFile( szCharFile );

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	lstrcpy( szFindPath , szDataDirectory );
	lstrcat( szFindPath , "\\*.ini" );

	//파일을 찾음( 확장자 변경 파일 )
	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle==INVALID_HANDLE_VALUE ) {
		FindClose( hFindHandle );
		return 0;
	}
	while(1) {
		lstrcpy( szCharFileList[CharFileCnt] , szDataDirectory );
		lstrcat( szCharFileList[CharFileCnt] , fd.cFileName );
		CharFileCnt++;

		if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		if ( CharFileCnt>255 ) break;
	}
	FindClose( hFindHandle );


	return TRUE;
}

void SetNextPlayerPattern()
{
	if ( lpCurPlayer->PatLoading ) return;

	//관리자 캐릭터 모드시 상대방에 데이타 송출 금지
	AdminCharDisable = TRUE;

	if ( AdminCharMode ) {
		memcpy( &lpCurPlayer->smCharInfo , &smCharInfo_Back , sizeof( smCHAR_INFO ) );
		ReformCharForm();
		AdminCharMode = 0;
		lpCurPlayer->PatLoading = TRUE;
		AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName, lpCurPlayer->smCharInfo.szModelName2 );
		lpCurPlayer->DisplayTools = TRUE;
		return;
	}

	AdminCharMode = 0;
	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szCharFileList[CharPlayCnt], lpCurPlayer->smCharInfo.szModelName2 );
	lpCurPlayer->DisplayTools = TRUE;

	CharPlayCnt++;
	if ( CharPlayCnt>=CharFileCnt ) CharPlayCnt = 0;
}

void SetPlayerBodyPattern( char *szBodyFile )
{
	char szBuff[256];
	FILE *fp;


	if ( lpCurPlayer->PatLoading ) return;

	wsprintf( szBuff , "char\\tmABCD\\%s.ini" , szBodyFile );
	fp = fopen( szBuff , "rb" );
	if ( !fp ) return;
	fclose(fp);
/*
	//관리자 캐릭터 모드시 상대방에 데이타 송출 금지
	AdminCharDisable = TRUE;
	AdminCharMode = 0;
*/
	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szBuff , lpCurPlayer->smCharInfo.szModelName2 );
	lpCurPlayer->DisplayTools = TRUE;
}

void SetPlayerPattern( char *szFile )
{
	FILE *fp;


	if ( lpCurPlayer->PatLoading ) return;

	fp = fopen( szFile , "rb" );
	if ( !fp ) return;
	fclose(fp);

	lpCurPlayer->PatLoading = TRUE;
	AddLoaderPattern( lpCurPlayer , szFile , 0 );
	lpCurPlayer->DisplayTools = TRUE;

	ReformCharForm();

	lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
	SendSetObjectSerial( lpCurPlayer->dwObjectSerial ); 

}


char szNpcFileList[64][64];
int NpcFileCnt;
int NpcPlayCnt = 0;

char *NpcSelectedName = 0;

int SetNpcFileList( char *szNpcDirectory )
{
	NpcFileCnt = 0;
	char szFindPath[256];

	HANDLE hFindHandle;
	WIN32_FIND_DATA	fd;

	lstrcpy( szFindPath , szNpcDirectory );
	lstrcat( szFindPath , "*.npc" );

	hFindHandle = FindFirstFile( szFindPath , &fd );
	if ( hFindHandle==INVALID_HANDLE_VALUE ) {
		FindClose( hFindHandle );
		return 0;

	}
	while(1) {
		lstrcpy( szNpcFileList[NpcFileCnt] , szNpcDirectory ); 
		lstrcat( szNpcFileList[NpcFileCnt] , fd.cFileName );
		NpcFileCnt++;

		if ( FindNextFile( hFindHandle , &fd )==FALSE ) break;
		if ( NpcFileCnt>64 ) break;

	}
	FindClose( hFindHandle );

	return TRUE;
}

void SetNextNpcPattern()
{
	if ( lpCurPlayer->PatLoading ) return;

	if ( AdminCharMode==0 ) {
		memcpy( &smCharInfo_Back , &lpCurPlayer->smCharInfo , sizeof( smCHAR_INFO ) );
	}

	//관리자 캐릭터 모드시 상대방에 데이타 송출 금지
	AdminCharDisable = TRUE;

	AdminCharMode = 1;				//NPC 설정 모드


	lpCurPlayer->smCharInfo.dwCharSoundCode = 0;

	//캐릭터 정보 파일에서 해독하여 설정한다
	if ( smCharDecode( szNpcFileList[NpcPlayCnt] , &lpCurPlayer->smCharInfo ,0 ) ) {
		lpCurPlayer->PatLoading = TRUE;
		AddLoaderPattern( lpCurPlayer , lpCurPlayer->smCharInfo.szModelName );

		lpCurPlayer->DisplayTools = FALSE;
		lpCurPlayer->smCharInfo.SizeLevel = 1;

		NpcSelectedName = szNpcFileList[NpcPlayCnt];

		NpcPlayCnt++;
		if ( NpcPlayCnt>=NpcFileCnt ) NpcPlayCnt = 0;
	}
	ReformCharForm();
}

//////////////// 동료 회복 함수 ////////////////
int LaunchPartyPostion() { return TRUE; }

//동료 회복 설정
int SetPartyPosion( smTRANS_COMMAND *lpTransCommand )
{
	if ( dwPartyPosionTime ) {
		//이미 예약중
		LaunchPartyPostion();
	}

	dwPartyPosionTime = dwPlayTime+1000;
	memcpy( &sPartyPosion , lpTransCommand , sizeof(smTRANS_COMMAND) );

	return TRUE;
}



char szPlayerCode[32];
int CharacterJob = 0;			//직업 코드 넘겨줌



extern	char *szNpcInfoPath;

DWORD	dwSoD_ResultTime = 0;		//SoD 상황창 갱신 시간


sBILLING_INFO	*sBiInfo = 0;


int InitPat3D() { return TRUE; }

int ClosePat3D() { return TRUE; }

int PartCnt = 0;



//int LoadTest = 0;


int		MsTraceCount = 0;
int		MsTraceMode = 0;
RECT	MsSelRect;
POINT	MsSelPos;

int		SelMouseButton =0;			//사용 선택된 마우스 버튼

smCHAR	*lpCharSelPlayer = 0;		//선택 캐릭터
smCHAR	*lpCharMsTrace = 0;			//이동 목표 캐릭터

scITEM	*lpSelItem = 0;				//선택된 아이템
scITEM	*lpMsTraceItem = 0;			//이동 목표 아이템

int AttFlag = 0;
int	AttCancel;						//공격 취소
int	AttAutoCont = 0;				//연속 공격 모드

//공격 설정
int TraceAttackPlay() { return TRUE; }

//공격 취소
int CancelAttack()
{
	SelMouseButton = 0;
	MsTraceMode = FALSE;
	lpCharMsTrace = 0;
	lpMsTraceItem = 0;
	AttFlag = 0;

	return TRUE;
}


//데미지에 따른 경험치 상승
int	DamageExp( smCHAR *lpChar , int AttackDamage )
{
	int AttackResult;

//#ifdef _LANGUAGE_CHINESE
//	lpCurPlayer->AttackExp = 0;
//	return TRUE;
//#endif

	//때리기만 해도 경험치 올라감
	if ( AttackDamage && lpChar->smCharInfo.State==smCHAR_STATE_ENEMY && 
		lpChar->smCharInfo.Brood!=smCHAR_MONSTER_USER && lpChar->smCharInfo.Life[0]>0 ) {

		AttackResult = (AttackDamage<<8)/lpChar->smCharInfo.Life[1];
		if ( AttackResult>256 ) AttackResult=256;
		AttackResult = ( lpChar->smCharInfo.Exp * AttackResult )>>8;
		AttackResult/=10;
		if ( AttackResult<=0 ) AttackResult = 1;
		lpCurPlayer->AttackExp = AttackResult;
	}

	return TRUE;
}


#define AUTO_DEAD_COUNT		(70*60*8)
int AutoDeathCounter = 0;
int FixedWidth = 200;
DWORD	dwClassDelayTime = 0;


//DWORD	dwLastModuleChk = 0;

int PlayPat3D() { return TRUE; }

extern int BGMatNum[2];
extern int RestoreTexFlag;



//마우스로 찍은 플레이어로의 각도를 구해준다
int GetMouseSelAngle( ) { return TRUE; }

//2명의 플레이어의 각도를 구해준다
int GetCharAngle( smCHAR *lpChar1 , smCHAR *lpChar2 ) { return TRUE; }


int testframe = 0;
int	LastRendSucessCount = 0;			//최근 렌더링 성공한 캐릭터 수
int	DispCharLimitMode = 0;				//0-전체 표시 1-근방만 표시

#define	DISP_CHAR_LIMIT			20
#define	DISP_CHAR_LIMIT_MAX		30
#define	DISP_CHAR_NEAR_DIST		( (10*64)*(10*64) )

int DrawPat3D( int x, int y, int z, int ax, int ay, int az ) { return TRUE; }

int DrawPat3D_Alpha() { return TRUE; }

//그림자 그리기
int DrawPatShadow(int x, int y, int z, int ax, int ay, int az ) { return TRUE; }

extern BYTE VRKeyBuff[256];

int Draw2DCounter = 0;

int DrawPat2D( int x, int y, int z, int ax, int ay, int az ) { return TRUE; }

extern int ViewPointLen ;
//int ViewPointLen = 28*SizeMAPCELL;
extern DWORD smBackColor;

int BGFace[2][3] = {
	{ 0 ,  1 , 2 },
	{ 1 ,  3 , 2 }
};




smOBJ3D *smBackGround;

void* *BackVideoSurface;
int BackVideoFrame;
int LastFrame ;

int BGMatNum[2];


int InitBackGround() { return TRUE; }

int CloseBackGround() { return TRUE; }


int BgFrame = 0;

int BGMain() { return TRUE; }

int ChangeBG( int BgNum ) { return TRUE; }

int DrawBG( int x, int y, int z, int ax, int ay, int az ) { return TRUE; }

RECT	PartyTradeButtonRect[4] = {
	{	00,0, 20, 20	},	
	{	25,0, 20, 20	},	
	{	50,0, 20, 20	},	
	{	75,0, 20, 20	}	
};

RECT	PartyTradeToolTipRect = { 1,18,70,28 };

POINT	PartyButtonPoint = { -1, -1 };

//파티 거래 신청 버튼 위치를 얻음
int GetPartyTradeButtonPos( int x, int y ) { return TRUE; }

//파티 트레이드 신청 버트 클릭
int DisplayPartyTradeButton() { return TRUE; }


/////////////////// 확대한 캐릭터 //////////////
smCHAR	chrEachPlayer;
smCHAR *chrEachMaster = 0;
int		EachTradeButton = 0;
int		EachTradeButtonMode = 0;
RECT	RectTradeButton;

POINT3D EachCameraPos;
int	DispEachMode = 0;


//BIP의 위치 포인트 좌표를 구한다
int FindBipPoint( smPAT3D *lpPat , char *szObjNodeName ,int frame, int *nX, int *nY, int *nZ );

//확대 캐릭터 오픈
int OpenEachPlayer( smCHAR *lpChar )
{
	if ( !lpChar || !lpChar->Flag || !lpChar->DisplayFlag ) return FALSE;
	if ( EachTradeButton ) return FALSE;

	memcpy( &chrEachPlayer , lpChar , sizeof( smCHAR ) );
	chrEachPlayer.SetMotionFromCode( CHRMOTION_STATE_STAND );
	chrEachPlayer.ActionPattern = 101;
	chrEachMaster = lpChar;

	chrEachPlayer.Flag = TRUE;

	if ( FindBipPoint( chrEachPlayer.AnimPattern , "Bip01 Head" , chrEachPlayer.frame ,
		&EachCameraPos.x , &EachCameraPos.y, &EachCameraPos.z )==NULL ) {

		EachCameraPos.y = chrEachPlayer.PatHeight-10*fONE;
		EachCameraPos.x = 0;
		EachCameraPos.z = 0;
	}

	//카메라 위치 보정
	if ( lpChar->smCharInfo.State==smCHAR_STATE_ENEMY || lpChar->smCharInfo.State==smCHAR_STATE_NPC ) {
		EachCameraPos.y += lpChar->smCharInfo.ArrowPosi[0]*fONE;
		EachCameraPos.z += lpChar->smCharInfo.ArrowPosi[1]*fONE;
		//EachCameraPos.y += -8000;//lpChar->smCharInfo.ArrowPosi[0]*fONE;
		//EachCameraPos.z += 8000;//lpChar->smCharInfo.ArrowPosi[1]*fONE;
	}

	return TRUE;
}

//확대 캐릭터 닫기
int CloseEachPlayer()
{
	chrEachPlayer.Flag = 0;
	chrEachMaster  = 0;
	EachTradeButton = 0;

	return TRUE;
}

//확대 캐릭터 메인
int EachPlayerMain() { return TRUE; }

int DrawClanMark_EachBox( void* clanMark , int x, int y) { return TRUE; }


//확대 캐릭터 그리기
int DrawEachPlayer( float fx , float fy , int SizeMode ) { return TRUE; }








/*
char *szBipName_RightHand = "Bip weapon01";//"Bip01 R Hand";//
	HvRightHand.ObjBip = AnimPat->GetObjectFromName(szBipName_RightHand) ;
//특정 오브젝트의 트리를 추적하여 애니메이션 시킴
int AnimObjectTree( smOBJ3D *tObj , int frame , int ax, int ay, int az )
*/

//BIP의 위치 포인트 좌표를 구한다
int FindBipPoint( smPAT3D *lpPat , char *szObjNodeName , int frame , int *nX, int *nY, int *nZ )
{

	smOBJ3D	*lpObj;

	lpObj = lpPat->GetObjectFromName(szObjNodeName);
	if ( !lpObj ) return FALSE;


	smMATRIX *mWorld;
	POINT3D angle;

	ZeroMemory( &angle, sizeof( POINT3D ) );
	angle.y = (-angle.y+ANGLE_180)&ANGCLIP;

	mWorld = &lpObj->mWorld;

	AnimObjectTree( lpObj , frame , angle.x , angle.y , angle.z );

	*nX = mWorld->_41;
	*nZ = mWorld->_42;
	*nY = mWorld->_43;

	return TRUE;

}


//클라이언트 로그 파일 기록
int Record_ClinetLogFile( char *szMessage )
{
	HANDLE	hFile;
	DWORD	dwAcess;
	DWORD	FileLength;
	SYSTEMTIME	st;
	int len;
	char	szFileName[128];
	char	szBuff[256];

	GetLocalTime( &st );
	wsprintf( szFileName , "game.log" , st.wMonth , st.wDay );

	hFile = CreateFile( "game.log" , GENERIC_WRITE , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile==INVALID_HANDLE_VALUE ) return FALSE;

	wsprintf( szBuff , "%d-%d (%d:%d:%d) - %s" , st.wMonth , st.wDay , st.wHour, st.wMinute , st.wSecond , szMessage );
	len = lstrlen(szBuff);
	if ( szBuff[len-1]!='\n' ) {
		szBuff[len]='\r';
		szBuff[len+1]='\n';
		len +=2;
	}

	FileLength = GetFileSize( hFile , NULL );
	SetFilePointer( hFile , FileLength , NULL , FILE_BEGIN );
	WriteFile( hFile , szBuff , len , &dwAcess , NULL );
	CloseHandle( hFile );

	return TRUE;
}



int ParkPlayInit = 0;		//초기화 플랙
int ParkPlayMode = 0;		//-플립금지 0-일반 +슬립시간(최대 1000)
int ParkPlaying = 0;		//동영상 플레이중
RECT ParkPlayRect;

//플레이어 초기화
int Init_ParkPlayer()
{
		return TRUE;
}

//플레이어 재생
int Play_ParkPlayer( char *szPath , int x, int y, int w, int h , int PlayMode )
{


	return TRUE;
}

//플레이어 중지
int Stop_ParkPlayer()
{


	return TRUE;
}
