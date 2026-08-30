#include "smlib3d\\smd3d.h"

#include "smwsock.h"
#include "Character/character.h"
#include "Character/playmain.h"
#include "fileread.h"
#include "netplay.h"
#include "sinbaram\\sinlinkheader.h"
#include "field.h"
#include "effectsnd.h"


char szFieldDirectory[64];				
char szMapDirectory[64];				
char szTitleDirectory[64];				
DWORD	dwWarpDelayTime = 0;
DWORD	dwNextWarpDelay = 0;
int		WingWarpField = -1;			
sFIELD	*lpLastWarpField = 0;


int FieldLimitLevel_Table[] = {
	0, //0
	0, //1
	0, //2
	0, //3
	0, //4
	0, //5
	0, //6
	0, //7
	0, //8
	0, //9
	0, //10
	55, //11
	55, //12
	40, //13
	50, //14
	60, //15
	0, //16
	0, //17
	0, //18
	0, //19
	0, //20
	0, //21
	70,	//22
	75, //23
	55, //24	
	55, //25
	65, //26
	80, //27
	85, //28
	90, //29
	0, //30
	90, //31
	0, //32
	0, //33
	70, //34
	90, //35
	90, //36
	95, //37
	100, //38
	0, //39
	102, //40
	105, //41
	100, //42
	110, //43
	120, //44
	115, //45
	112, //46
	118, //47
	80, //48
	80, //49
	150, //50
	135, //51
	0 //52
};

extern std::vector<std::string> FieldName;

int sFIELD::SetName( char *lpName, char *lpNameMap )
{
	char mapPath[64] = { 0 };
	lstrcpy( szName, szFieldDirectory );
	lstrcat( szName, lpName );

	if( lpNameMap )
	{
		sprintf_s( szNameMap, "%s%s.tga", szMapDirectory, lpNameMap );
		sprintf_s( szNameTitle, "%s%st.tga", szTitleDirectory, lpNameMap );
		sprintf_s(mapPath, "%s\\%s", "Maps", lpNameMap);
		FieldName.push_back(mapPath);
	}
	else
	{
		FieldName.push_back("Maps\\Office");
		szNameMap[0] = 0;
		szNameTitle[0] = 0;
	}

	return TRUE;
}


//필드 중앙 좌표 입력
int sFIELD::SetCenterPos( int x, int z )
{
	cX = x;
	cZ = z;
	return TRUE;
}


//배경 보조 오브젝트 추가
int	sFIELD::AddStageObject( char *szStgObjName, int BipAnimation )
{
	if( StgObjCount >= FIELD_STAGE_OBJ_MAX ) return FALSE;

	lpStageObjectName[StgObjCount] = szStgObjName;
	StgObjBip[StgObjCount] = BipAnimation;

	StgObjCount++;

	return StgObjCount;
}

//배경 보조 오브젝트 이름 얻기
int	sFIELD::GetStageObjectName( int num, char *szNameBuff )
{
	if( StgObjCount <= num )
	{
		szNameBuff[0] = 0;
		return FALSE;
	}

	lstrcpy( szNameBuff, szFieldDirectory );
	lstrcat( szNameBuff, lpStageObjectName[num] );

	return StgObjBip[num];
}


int sFIELD::AddGate( sFIELD *lpsField, int x, int z, int y )
{
	if( GateCount >= FILED_GATE_MAX ) return FALSE;

	FieldGate[GateCount].x = x;
	FieldGate[GateCount].y = y;
	FieldGate[GateCount].z = z;
	FieldGate[GateCount].lpsField = lpsField;

	GateCount++;

	//상대방 필드게이트 에도 좌표 자동 설정
	lpsField->AddGate2( (sFIELD *)&head, x, z, y );

	return TRUE;
}

int sFIELD::AddGate2( sFIELD *lpsField, int x, int z, int y )
{
	if( GateCount >= FILED_GATE_MAX ) return FALSE;

	FieldGate[GateCount].x = x;
	FieldGate[GateCount].y = y;
	FieldGate[GateCount].z = z;
	FieldGate[GateCount].lpsField = lpsField;

	GateCount++;

	return TRUE;
}

/*
int		WarpGateCount;					//워프 게이트의 수
sWARPGATE	WarpGate[FILED_GATE_MAX];	//워프 게이트
int	AddWarpGate( int x, int y, int z, int size , int height );	//워프게이트 추가
int	AddWarpOutGate( sFIELD *lpsField , int x, int z, int y );	//워프게이트 출구 추가

//필드의 게이트
struct sWARPGATE	{
int	x,z,y;								//필드 게이트 위치
int	height,size;						//범위
sFGATE	OutGate[FILED_GATE_MAX];		//출구 위치
int		OutGateCount;					//출구 카운터
};
//필드의 게이트
struct sFGATE	{
int	x,z,y;				//필드 게이트 위치
sFIELD	*lpsField;		//필드 포인터
};

*/
//워프게이트 추가
int sFIELD::AddWarpGate( int x, int z, int y, int size, int height )
{

	if( WarpGateCount >= FILED_GATE_MAX ) return FALSE;

	WarpGateActiveNum = WarpGateCount;

	WarpGate[WarpGateCount].x = x << FLOATNS;
	WarpGate[WarpGateCount].y = y << FLOATNS;
	WarpGate[WarpGateCount].z = z << FLOATNS;
	WarpGate[WarpGateCount].height = height << FLOATNS;
	WarpGate[WarpGateCount].size = size << FLOATNS;

	WarpGate[WarpGateCount].OutGateCount = 0;

	WarpGateCount++;

	return WarpGateCount;
}

//워프게이트 출구 추가
int	sFIELD::AddWarpOutGate( sFIELD *lpsField, int x, int z, int y )
{

	sWARPGATE	*lpWarpGate;
	int		cnt;

	lpWarpGate = &WarpGate[WarpGateActiveNum];
	cnt = lpWarpGate->OutGateCount;

	if( cnt >= FILED_GATE_MAX ) return FALSE;

	lpWarpGate->OutGate[cnt].lpsField = lpsField;
	lpWarpGate->OutGate[cnt].x = x << FLOATNS;
	lpWarpGate->OutGate[cnt].z = z << FLOATNS;
	lpWarpGate->OutGate[cnt].y = y << FLOATNS;

	lpWarpGate->OutGateCount++;

	if( lpsField == this )
	{
		PosWarpOut.x = x << FLOATNS;
		PosWarpOut.y = y << FLOATNS;
		PosWarpOut.z = z << FLOATNS;
	}

	return lpWarpGate->OutGateCount;
}


int sFIELD::AddStartPoint( int x, int z )
{
	if( StartPointCnt >= FIELD_START_POINT_MAX ) return FALSE;

	StartPoint[StartPointCnt].x = x;
	StartPoint[StartPointCnt].y = z;
	StartPointCnt++;

	return TRUE;
}

//제일 근접한 시작 좌표를 얻는다 
int sFIELD::GetStartPoint( int x, int z, int *mx, int *mz )
{
	int cnt;
	int dist;
	int NearDist;
	int	NearCnt;
	int dx, dz;

	if( StartPointCnt == 0 )
	{
		*mx = cX*fONE;
		*mz = cZ*fONE;
		return TRUE;
	}

	x >>= FLOATNS;
	z >>= FLOATNS;

	dx = ( x - StartPoint[0].x );
	dz = ( z - StartPoint[0].y );

	NearDist = dx*dx + dz*dz;
	NearCnt = 0;

	for( cnt = 1; cnt<StartPointCnt; cnt++ )
	{
		dx = ( x - StartPoint[cnt].x );
		dz = ( z - StartPoint[cnt].y );
		dist = dx*dx + dz*dz;
		if( dist<NearDist )
		{
			NearDist = dist; NearCnt = cnt;
		}
	}

	*mx = StartPoint[NearCnt].x << FLOATNS;
	*mz = StartPoint[NearCnt].y << FLOATNS;

	return TRUE;
}


//시작 좌표상에 캐릭터가 있는지 확인 
int sFIELD::CheckStartPoint( int x, int z )
{
	int cnt;
	int dx, dz;

	for( cnt = 0; cnt<StartPointCnt; cnt++ )
	{
		dx = StartPoint[cnt].x << FLOATNS;
		dz = StartPoint[cnt].y << FLOATNS;

		if( abs( dx - x )<24 * fONE && abs( dz - z )<24 * fONE ) return TRUE;
	}

	return FALSE;
}


//배경 효과음 추가
int sFIELD::AddAmbientPos( int x, int y, int z, int round, int AmbCode )
{

	if( AmbentCount >= FIELD_AMBENT_MAX ) return FALSE;

	AmbientPos[AmbentCount].x = x*fONE;
	AmbientPos[AmbentCount].y = y*fONE;
	AmbientPos[AmbentCount].z = z*fONE;
	AmbientPos[AmbentCount].round = round;
	AmbientPos[AmbentCount].AmbentNum = AmbCode;

	AmbentCount++;

	return AmbentCount;
}

//배경 효과음 연주 ( 주기적 호출 )
int	sFIELD::PlayAmbient()
{
	return TRUE;
}


sFIELD	sField[FIELD_MAX];
int FieldCount = 0;
int CustomField = 0;

static int SelCount = 0;

//필드 초기화
int InitField()
{
	int cnt;

	lstrcpy( szFieldDirectory, "field\\" );
	lstrcpy( szMapDirectory, "field\\map\\" );
	lstrcpy( szTitleDirectory, "field\\title\\" );

	ZeroMemory( sField, sizeof( sFIELD ) * FIELD_MAX );

	for( cnt = 0; cnt<FIELD_MAX; cnt++ )
		sField[cnt].FieldCode = cnt;

	cnt = 0;

	sField[cnt].SetName( "forest\\fore-3.ase", "fore-3" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( -16419, -7054 );
	sField[cnt].AddStartPoint( -10585, -11810 );
	sField[cnt].AddStartPoint( -13659, -9753 );
	cnt++;

	sField[cnt].SetName( "forest\\fore-2.ase", "fore-2" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( -5427, -9915 );
	sField[cnt].AddStartPoint( -2272, -10314 );
	sField[cnt].AddStartPoint( -7192, -11175 );

	cnt++;

	sField[cnt].SetName( "forest\\fore-1.ase", "fore-1" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 4184, -11016 );
	sField[cnt].AddStartPoint( 1350, -13672 );
	sField[cnt].AddStartPoint( 1761, -10815 );
	sField[cnt].AddStartPoint( 4777, -10916 );
	cnt++;

	sField[cnt].SetName( "ricarten\\village-2.ase", "village-2" );
	sField[cnt].State = FIELD_STATE_VILLAGE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 2596, -18738 );
	sField[cnt].AddStartPoint( 2592, -18566 );
	sField[cnt].AddStartPoint( -1047, -16973 );
	cnt++;

	sField[cnt].SetName( "Ruin\\ruin-4.ase", "ruin-4" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 1384, -918 );
	sField[cnt].AddStartPoint( 2578, -5124 );
	sField[cnt].AddStartPoint( 1873, 3060 );
	cnt++;

	sField[cnt].SetName( "Ruin\\ruin-3.ase", "ruin-3" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 4953, 10922 );
	sField[cnt].AddStartPoint( 875, 7910 );
	sField[cnt].AddStartPoint( 1576, 14307 );
	cnt++;

	sField[cnt].SetName( "Ruin\\ruin-2.ase", "ruin-2" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 7459, 23984 );
	sField[cnt].AddStartPoint( 3976, 19645 );
	sField[cnt].AddStartPoint( 5832, 23751 );
	sField[cnt].AddStartPoint( 5615, 25117 );
	cnt++;

	sField[cnt].SetName( "Ruin\\ruin-1.ase", "ruin-1" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 16362, 14959 );
	sField[cnt].AddStartPoint( 12242, 16034 );
	sField[cnt].AddStartPoint( 12194, 8969 );
	cnt++;

	sField[cnt].SetName( "desert\\De-1.ase", "De-1" );
	sField[cnt].State = FIELD_STATE_DESERT;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 15005, -1421 );
	sField[cnt].AddStartPoint( 13306, 2000 );
	sField[cnt].AddStartPoint( 13083, -2249 );
	cnt++;

	sField[cnt].SetName( "forest\\village-1.ase", "village-1" );
	sField[cnt].State = FIELD_STATE_DESERT;
	sField[cnt].FieldEvent = FIELD_EVENT_NIGHTMARE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 22214, -1182 );
	sField[cnt].AddStartPoint( 22214, -1182 );
	cnt++;

	sField[cnt].SetName( "desert\\De-2.ase", "De-2" );
	sField[cnt].State = FIELD_STATE_DESERT;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 15887, -11161 );
	sField[cnt].AddStartPoint( 11859, -11257 );
	sField[cnt].AddStartPoint( 16169, -12768 );
	cnt++;

	sField[cnt].SetName( "desert\\De-3.ase", "De-3" );
	sField[cnt].State = FIELD_STATE_DESERT;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 34758, -1323 );
	sField[cnt].AddStartPoint( 34758, -1323 );
	sField[cnt].AddStartPoint( 29424, 322 );
	cnt++;

	sField[cnt].SetName( "desert\\De-4.ase", "De-4" );
	sField[cnt].State = FIELD_STATE_DESERT;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 43988, 12053 );
	sField[cnt].AddStartPoint( 33740, 6491 );
	sField[cnt].AddStartPoint( 40691, 11056 );
	cnt++;

	sField[cnt].SetName( "dungeon\\dun-1.ase", "dun-1" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -15384, -24310 );
	cnt++;

	sField[cnt].SetName( "dungeon\\dun-2.ase", "dun-2" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -6108, -26880 );
	cnt++;

	sField[cnt].SetName( "dungeon\\dun-3.ase", "dun-3" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 1827, -28586 );
	cnt++;

	sField[cnt].SetName( "room\\office.ase", 0 );
	sField[cnt].State = FIELD_STATE_ROOM;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( -200000, 200000 );
	cnt++;

	sField[cnt].SetName( "forever-fall\\forever-fall-04.ASE", "forever-fall-04" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -1583, 37905 );
	sField[cnt].AddStartPoint( -2135, 33668 );
	sField[cnt].AddStartPoint( 4073, 32119 );
	cnt++;

	sField[cnt].SetName( "forever-fall\\forever-fall-03.ASE", "forever-fall-03" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -5823, 43244 );
	sField[cnt].AddStartPoint( -1516, 45437 );
	sField[cnt].AddStartPoint( -3586, 42886 );
	cnt++;

	sField[cnt].SetName( "forever-fall\\forever-fall-02.ASE", "forever-fall-02" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 1691, 52599 );
	sField[cnt].AddStartPoint( -1024, 45437 );
	sField[cnt].AddStartPoint( -1543, 52333 );
	sField[cnt].AddStartPoint( -1310, 54945 );
	cnt++;

	sField[cnt].SetName( "forever-fall\\forever-fall-01.ASE", "forever-fall-01" );
	sField[cnt].State = FIELD_STATE_FOREST;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 2032, 71183 );
	sField[cnt].AddStartPoint( 1875, 69871 );
	sField[cnt].AddStartPoint( 2637, 60219 );
	cnt++;

	sField[cnt].SetName( "forever-fall\\pilai.ASE", "pilai" );
	sField[cnt].State = FIELD_STATE_VILLAGE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 2981, 75486 );
	sField[cnt].AddStartPoint( 2287, 74131 );
	sField[cnt].AddStartPoint( 3547, 75500 );
	cnt++;

	sField[cnt].SetName( "dungeon\\dun-4.ase", "dun-4" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -11108, -41681 );
	cnt++;

	sField[cnt].SetName( "dungeon\\dun-5.ase", "dun-5" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].SetCenterPos( -3675, -36597 );
	cnt++;

	sField[cnt].SetName( "cave\\Tcave.ase", "Tcave" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 120126, 26064 );
	sField[cnt].AddStartPoint( 119319, 26034 );
	sField[cnt].AddStartPoint( 125606, 24541 );
	cnt++;

	sField[cnt].SetName( "cave\\Mcave.ase", "Mcave" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 119966, 35466 );
	sField[cnt].AddStartPoint( 119320, 35680 );
	sField[cnt].AddStartPoint( 124380, 33260 );
	cnt++;

	sField[cnt].SetName( "cave\\Dcave.ase", "Dcave" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 158814, 20070 );
	cnt++;

	sField[cnt].SetName( "Iron\\iron-1.ASE", "iron-1" );						// 혼돈의 철로
	sField[cnt].State = FIELD_STATE_IRON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 47617, 13626 );
	sField[cnt].AddStartPoint( 46905, 13478 );
	sField[cnt].AddStartPoint( 47357, 20845 );
	sField[cnt].AddStartPoint( 46741, 21349 );
	cnt++;

	sField[cnt].SetName( "Iron\\iron-2.ASE", "iron-2" );
	sField[cnt].State = FIELD_STATE_IRON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 33938, 24229 );
	sField[cnt].AddStartPoint( 35149, 24326 );
	sField[cnt].AddStartPoint( 44893, 21380 );
	cnt++;

	sField[cnt].SetName( "Ice\\ice_ura.ASE", "ice_ura" );
	sField[cnt].State = FIELD_STATE_ICE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 32621, 23865 );
	cnt++;

	sField[cnt].SetName( "Sod\\sod-1.ASE", "sod-1" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 22018, 6569 );
	cnt++;



	sField[cnt].SetName( "Ice\\ice1.ASE", "ice1" );
	sField[cnt].State = FIELD_STATE_ICE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 32313, 28529 );
	cnt++;


	sField[cnt].SetName( "quest\\quest_IV.ASE", "quest_IV" );
	sField[cnt].State = FIELD_STATE_QUEST_ARENA;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 22497, 9605 );
	cnt++;



	sField[cnt].SetName( "castle\\castle.ase", "castle" );
	sField[cnt].State = FIELD_STATE_CASTLE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 35225, -23847 );
	sField[cnt].AddStartPoint( 33920, -23479 );
	sField[cnt].AddStartPoint( 35247, -23355 );
	sField[cnt].AddStartPoint( 36164, -23446 );
	cnt++;



	//그리디 호수
	sField[cnt].SetName( "Greedy\\Greedy.ase", "Greedy" );
	sField[cnt].State = FIELD_STATE_ICE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 13930, 23206 );
	cnt++;



	sField[cnt].SetName( "Ice\\ice_2.ASE", "ice2" );
	sField[cnt].State = FIELD_STATE_ICE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 36264, 40182 );
	cnt++;


	sField[cnt].SetName( "Boss\\Boss.ASE", "Boss" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 33000, 50036 );
	cnt++;


	sField[cnt].SetName( "lost\\lostisland.ASE", "lost" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -12555, -1113 );
	cnt++;


	sField[cnt].SetName( "Losttemple\\lost_temple.ASE", "Losttemple" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -11718, 10794 );	
	cnt++;

	sField[cnt].SetName( "Fall_Game\\fall_game.ASE", 0 );
	sField[cnt].State = FIELD_STATE_ACTION;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 5800, 36747 );
	cnt++; 

	sField[cnt].SetName( "endless\\dun-7.ase", "dun-7" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 14255, -39099 );
	cnt++;

	////////////////////////// pluto 추가 던전 /////////////////////////
	sField[cnt].SetName( "endless\\dun-8.ase", "dun-8" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 5255, -37897 );

	cnt++;


	sField[cnt].SetName( "dungeon\\Dun-6a.ASE", "dun-6" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -3660, -45200 );
	cnt++;

	sField[cnt].SetName( "endless\\dun-9.ASE", "dun-9" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 4896, -42330 );
	cnt++;

	sField[cnt].SetName( "slab\\slab.ASE", "slab" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -11918, -62447 );
	cnt++;

	sField[cnt].SetName( "AncientW\\ancientW.ASE", "AncientW" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 12856, -73928 );
	cnt++;

	sField[cnt].SetName( "Mine\\mine-1.ASE", "mine-1" );
	sField[cnt].State = FIELD_STATE_ICE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( 18006, -34740 );
	cnt++;

	sField[cnt].SetName( "lost\\lost3.ASE", "lost3" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].SetCenterPos( 116360, 91328 );
	cnt++;

	sField[cnt].SetName( "boss\\dark_boss.ASE", "dc1" );
	sField[cnt].State = FIELD_STATE_DUNGEON;
	sField[cnt].SetCenterPos( 198279, 240452 );
	cnt++;

	sField[cnt].SetName( "old_forest\\ba1.ase", "ba1" );
	sField[cnt].State = FIELD_STATE_RUIN;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( -23879, 22340 );
	cnt++;

	sField[cnt].SetName( "Iron\\iron3.ASE", "iron3" );
	sField[cnt].State = FIELD_STATE_IRON;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -18286, 21104 );
	cnt++;

	sField[cnt].SetName( "SeaA\\SeaA.ASE", "SeaA" );
	sField[cnt].State = FIELD_STATE_ICE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	sField[cnt].SetCenterPos( -20699, -4205 );
	cnt++;

	//sField[cnt].SetName( "old_forest\\old_fore-3.ase", "old_fore-3" );
	//sField[cnt].State = FIELD_STATE_FOREST;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	//sField[cnt].SetCenterPos( 989880, 989360 );
	//cnt++;

	//sField[cnt].SetName( "old_forest\\old_fore-2.ase", "old_fore-2" );
	//sField[cnt].State = FIELD_STATE_FOREST;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	//sField[cnt].SetCenterPos( 994573, 990085 );
	//cnt++;

	//sField[cnt].SetName( "old_forest\\old_fore-1.ase", "old_fore-1" );
	//sField[cnt].State = FIELD_STATE_FOREST;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	//sField[cnt].SetCenterPos( 1004184, 988984 );
	//cnt++;

	sField[cnt].SetName( "old_forest\\old_village-2.ase", "old_village-2" );
	sField[cnt].State = FIELD_STATE_VILLAGE;
	sField[cnt].ServerCode = PLAY_SERVER_CODE_MAIN;
	sField[cnt].SetCenterPos( 1002275, 982802 );
	cnt++;

	//sField[cnt].SetName( "old_dungeon\\old_dun-1.ase", "old_dun-1" );
	//sField[cnt].State = FIELD_STATE_DUNGEON;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	//sField[cnt].SetCenterPos( 984616, 975690 );
	//cnt++;

	//sField[cnt].SetName( "old_dungeon\\old_dun-2.ase", "old_dun-2" );
	//sField[cnt].State = FIELD_STATE_DUNGEON;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	//sField[cnt].SetCenterPos( 993892, 973120 );
	//cnt++;

	//sField[cnt].SetName( "old_dungeon\\old_dun-3.ase", "old_dun-3" );
	//sField[cnt].State = FIELD_STATE_DUNGEON;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	//sField[cnt].SetCenterPos( 1001827, 971414 );
	//cnt++;

	//sField[cnt].SetName( "old_forest\\sanc1.ase", "sanc1" );
	//sField[cnt].State = FIELD_STATE_DUNGEON;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	//sField[cnt].SetCenterPos( 33825, -36598 );
	//cnt++;

	//sField[cnt].SetName( "old_forest\\sanc2.ase", "sanc2" );
	//sField[cnt].State = FIELD_STATE_DUNGEON;
	//sField[cnt].ServerCode = PLAY_SERVER_CODE_EXTEND;
	//sField[cnt].SetCenterPos( 26338, -45548 );
	//cnt++;


	if( smConfig.szFile_Stage[0] )
	{
		CustomField = cnt;
		sField[cnt].SetName( smConfig.szFile_Stage );

		//######################################################################################
		//작 성 자 : 오 영 석
		for( int cnt2 = 0; cnt2 < smConfig.StageObjCnt; cnt2++ )
		{
			if( smConfig.szFile_StageObject[cnt2][0] )
			{
				if( smConfig.IsStageAniObject[cnt2] )
					sField[cnt].AddStageObject( smConfig.szFile_StageObject[cnt2], 1 );
				else
					sField[cnt].AddStageObject( smConfig.szFile_StageObject[cnt2] );
			}
		}

		if( strstr( smConfig.szFile_Stage, "dun" ) != 0 )
			sField[cnt].State = FIELD_STATE_DUNGEON;
		else if( strstr( smConfig.szFile_Stage, "ice" ) != 0 )
			sField[cnt].State = FIELD_STATE_ICE;
		else
			sField[cnt].State = FIELD_STATE_FOREST;
	
		sField[cnt].ServerCode = PLAY_SERVER_CODE_USER;
		sField[cnt].SetCenterPos( smConfig.Posi_Stage.x >> FLOATNS, smConfig.Posi_Stage.y >> FLOATNS );

		cnt++;
	}

	FieldCount = cnt;

	//확장서버일경우 확장서버 유무 확인하여 보정
	if( smWsockServer && smWsockServer == smWsockExtendServer )
	{
		for( cnt = 0; cnt<FieldCount; cnt++ )
		{
			if( sField[cnt].ServerCode == PLAY_SERVER_CODE_EXTEND )
			{
				sField[cnt].ServerCode = PLAY_SERVER_CODE_USER;
			}
		}
	}

	return TRUE;
}


int StartField()
{

	return TRUE;
}

