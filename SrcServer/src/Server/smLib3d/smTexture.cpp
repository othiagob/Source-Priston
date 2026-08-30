
#include "smd3d.h"



//######################################################################################
//작 성 자 : 오 영 석
int		g_IsReadTextures = 1;

BYTE	g_DibImageDateBuff[ (1024*768)*4 ];
int		g_DibLastReadWidth;
int		g_DibLastReadHeight;
int		g_TgaLastBitCnt;
int		g_TgaLastSwapHeight;

int						new_smCreateTexture( smTEXTUREHANDLE *lpTexHandle, int MipMap = 0 );

int						new_LoadDib( char *Filename );
int						new_LoadTga( char *Filename );
int						New_DrawSurfaceFromTga( void* lpDDSurface );

void*	new_LoadBmpCreateTexSurface( char *Filename, int MipMap, int &rTexHalf );
void*	new_LoadTgaCreateTexSurface( char *Filename, int MipMap, int &rTexHalf );
void*	new_LoadBmpSurfaceOffscreen( char *Filename );
//######################################################################################




#define ERROR_LIST_MAX		32
#define ERROR_LIST_MASK		(ERROR_LIST_MAX-1)

char szErrorList[ERROR_LIST_MAX][64];
int ErrorListCnt  = 0;


int AddErrorList( char *szErrMsg )
{
	/*
	int cnt;

	cnt = ErrorListCnt & ERROR_LIST_MASK;

	lstrcpy( szErrorList[cnt] , szErrMsg );

	ErrorListCnt++;
	*/
	return ErrorListCnt;
}

int DisplayError( HDC hdc , int x, int y )
{
	int cnt,mcnt;
	int len;
	int dy;

	dy = y;

	for(cnt=ErrorListCnt-ERROR_LIST_MASK;cnt<ErrorListCnt;cnt++) {
		if ( cnt>=0 ) {
			mcnt = cnt & ERROR_LIST_MASK;
			len = lstrlen( szErrorList[mcnt] );
			//dsTextLineOut( hdc , x,dy ,szErrorList[mcnt] , len );
			dy+=14;
		}
	}

	return TRUE;
}


//비디오 사용 메모리 예상치
DWORD	VramTotal = 0;


//######################################################################################
//작 성 자 : 오 영 석
#define	MAX_MAP_BLINK_COLOR_TIME				8

const char *szBlinkTimeScript[ MAX_MAP_BLINK_COLOR_TIME ] =
{
	"BLINK_TIME1:",
	"BLINK_TIME2:",
	"BLINK_TIME3:",
	"BLINK_TIME4:",
	"BLINK_TIME5:",
	"BLINK_TIME6:",
	"BLINK_TIME7:",
	"BLINK_TIME8:",
};

const DWORD dwBlinkTimeCode[ MAX_MAP_BLINK_COLOR_TIME ] =
{
    9, 10, 11, 12, 13, 14, 15, 16,
};
//######################################################################################


//######################################################################################
//작 성 자 : 오 영 석
int MipMapModeCreate;						// 밉맵 사용 가능 한지.
int TextureSwapMode	= TRUE;					// 그래픽 퀄리티에 의해서 밉맵 사용 할지 알기위해. 전역으로.
static int TexturHalfSize = FALSE;			// 텍스쳐 절반으로 크기 줄임

// 6.0 에서의 밉맵 싸이즈 레벨 단계 256×256, 128×128, 64×64, 32×32, 16×16
// 또한 256×128 로 시작 할수도 있지만. 카드별로 안되는 것도 있다고 합니다.
// 프테 에서는 최대 3단계 까지만 사용 하기로 했음.
static char MipMapFileListBuff[ MIPMAP_FILELIST ][16] = { "", "_mm1", "_mm2", "_mm3", "_mm4", };

static int  MipMapCount = 0;
static char MipMapFileBuff[ MIPMAP_FILELIST ][126] = { 0,};
static int  MipMapCountA = 0;
static char MipMapFileABuff[ MIPMAP_FILELIST ][126] = { 0,};
static char FileTypeBuff[16] = { 0,};
//######################################################################################


int		   MaterialFrame;

//텍스쳐 메모리 스왑 모드 설정
int SetTextureSwapMode( int mode )
{
	int OldValue;

	OldValue = TextureSwapMode;

	TextureSwapMode = mode;

	return OldValue;
}

//텍스쳐의 레벨 설정 ( 0-고화질 1-보통 2-저화질 )
int smSetTextureLevel( int Level )
{
	switch( Level ) {
	case 0:	//( 비디오 메모리가 큰 시스템에서 유리 )
		//모든 텍스쳐를 비디오 메모리로 올린다
		TextureSwapMode = FALSE;
		TexturHalfSize = 0;
		break;

	case 1: //( 메모리가 큰 시스템에서 유리 )
		//텍스쳐를 비디오와 시스템 메모리에 섞음
		TextureSwapMode = TRUE;
		TexturHalfSize = 0;
		break;

	case 2: //( 모두 후진 시스템에서 사용 )
		//모든 텍스쳐를 절반크기로 줄여서 비디오 메모리에 올림
		TextureSwapMode = TRUE;
		TexturHalfSize = 1;
		break;

	case 3: //( VRAM 8메가 이하 절대 후진 시스템에서 사용 )
		//모든 텍스쳐를 1/4크기로 줄여서 비디오 메모리에 올림
		TextureSwapMode = TRUE;
		TexturHalfSize = 2;
		break;
	}

	return TRUE;
}



smTEXTURE::smTEXTURE()
{
	Init();
}

smTEXTURE::~smTEXTURE()
{
//	Remove();
}

void smTEXTURE::Init()
{
	//######################################################################################
	//작 성 자 : 오 영 석
	ZeroMemory( TexSpeedSum, sizeof(TexSpeedSum) );
	//######################################################################################

	ZeroMemory( Texture , sizeof( smTEXTUREHANDLE ) * MAX_TEXTURE ); 
	ZeroMemory( SwapTexture , sizeof( smSWAPTEXTURE ) * SWAPTEXTURE_MAX ); 
	TextureCnt	= 0;
	SwapCount	= 0;

	//크리티컬 섹션 초기화
	InitializeCriticalSection( &cTextureSection );

}

int smTEXTURE::Remove()
{
	return TRUE;
}


//######################################################################################
//작 성 자 : 오 영 석
//int smTEXTURE::Find( char *name )
int smTEXTURE::Find( DWORD dwCode, char *Name )
{
	int cnt;
	for( cnt=0; cnt < MAX_TEXTURE; cnt++ )
	{
		if( TexSpeedSum[cnt] == dwCode )
		{
			if( Texture[cnt].UseCounter && _stricmp( Texture[cnt].Name, Name ) == 0 )
				return cnt;
		}
	}

	return -1;
}
//######################################################################################


//텍스쳐 목록을 추가
smTEXTUREHANDLE *smTEXTURE::Add( char *name , char *nameA )
{
	int TexNum;
	int cnt;

	//######################################################################################
	//작 성 자 : 오 영 석	
	extern DWORD GetSpeedSum( char *szName );

	DWORD dwCode = GetSpeedSum(name);
	TexNum = Find( dwCode, name );
	//TexNum = Find( name );
	//######################################################################################

	if ( TexNum>=0 ) {
		LastTextureCnt = TexNum;
		Texture[ TexNum ].UseCounter++;
		return &Texture[ TexNum ];
	}

	for( cnt=0;cnt<MAX_TEXTURE;cnt++) {
		if ( Texture[ cnt ].UseCounter==0 ) break;
	}
	if ( cnt==MAX_TEXTURE ) return NULL;
	TextureCnt = cnt;

	lpLastTextureSurface = &Texture[ TextureCnt ].lpDDSurface;
	lpLastTexture = &Texture[ TextureCnt ].lpD3DTexture;

	//초기 핸들 값은 0 이다
	Texture[TextureCnt].lpDDSurface = NULL;
	Texture[TextureCnt].lpD3DTexture = NULL;

	lstrcpy( Texture[ TextureCnt ].Name , name );
	if ( nameA )
		lstrcpy( Texture[ TextureCnt ].NameA , nameA );
	else 
		Texture[ TextureCnt ].NameA[0] = 0;

	Texture[ TextureCnt ].TexSwapMode = TextureSwapMode;			// 기본적으로 스왑모드 허가
	Texture[ TextureCnt ].UseCounter++;
	Texture[ TextureCnt ].lpD3DTextureLarge = NULL;

	//######################################################################################
	//작 성 자 : 오 영 석	
	TexSpeedSum[ TextureCnt ] = dwCode;
	//######################################################################################

	return &Texture[ TextureCnt ];
}

//텍스쳐를 제거한다 ( 1단계식 제거하여 메트리얼과의 링크가 없을 경우 완전 제거 )
int smTEXTURE::Delete( smTEXTUREHANDLE *lpTex )
{
	int cnt;

	if ( lpTex->UseCounter<=0 ) return FALSE;

	lpTex->UseCounter--;

	if ( lpTex->UseCounter<=0 ) {
		//스왑 텍스쳐에서 사용중이면 스왑텍스쳐를 찾아내어 제거
		if ( lpTex->lpD3DTextureLarge ) {
			for( cnt=0; cnt<SWAPTEXTURE_MAX ; cnt++ ) {
				if ( SwapTexture[cnt].SwapState && SwapTexture[cnt].smTextureMaster==lpTex ) {
					DeleteSwapTexture( cnt );
					break;
				}
			}
		}

		lpTex->lpD3DTexture = 0;
		lpTex->lpDDSurface = 0;
		lpTex->lpDDSSysMemory = 0;

		VramTotal -= lpTex->Width * lpTex->Height  * 2 ;

	}

	return lpTex->UseCounter;
}

/*
LPDIRECT3DTEXTURE2 *smTEXTURE::AddName( char *name )
{
	int TexNum;

	TexNum = Find( name );
	if ( TexNum>=0 ) {
		LastTextureCnt = TexNum;
		return &Texture[ TexNum ].lpD3DTexture;
	}

	lpLastTextureSurface = &Texture[ TextureCnt ].lpDDSurface;
	lpLastTexture = &Texture[ TextureCnt ].lpD3DTexture;

	lstrcpy( Texture[ TextureCnt ].Name , name );
	Texture[ TextureCnt ].NameA[0] = 0;
	TextureCnt++;

	return &Texture[ TextureCnt-1 ].lpD3DTexture;
}

LPDIRECT3DTEXTURE2 *smTEXTURE::AddName( char *name , char *nameA )
{
	int TexNum;
	char szBuff[64];
	LPDIRECT3DTEXTURE2 *lpTexture;
	FILE *fp;

	TexNum = Find( name );
	if ( TexNum>=0 ) {
		LastTextureCnt = TexNum;
		return &Texture[ TexNum ].lpD3DTexture;
	}

	lpTexture = &Texture[ TextureCnt ].lpD3DTexture ;

	lpLastTextureSurface = &Texture[ TextureCnt ].lpDDSurface;
	lpLastTexture = &Texture[ TextureCnt ].lpD3DTexture;
	LastTextureCnt = TextureCnt;

	lstrcpy( Texture[ TextureCnt ].Name , name );
	lstrcpy( Texture[ TextureCnt ].NameA , nameA );
	TextureCnt++;

	return lpTexture;
}
*/


//SWAP 텍스쳐를 지움
int smTEXTURE::DeleteSwapTexture( int TexNum )
{
	return TRUE;
}

//큰텍스쳐를 SWAP 하여 만든다 
int smTEXTURE::SetLargeTexture( smTEXTUREHANDLE *TexHandle )
{
	return TRUE;
}

//텍스쳐 
int smTEXTURE::ReadTexture()
{
	//######################################################################################
	//작 성 자 : 오 영 석
	if( g_IsReadTextures == 0 )
		return 0;
	//######################################################################################

	int cnt;
	char szErrMsg[128];
	int	Result;
	int ReadCount = 0;

	for( cnt=0;cnt<MAX_TEXTURE;cnt++ ) {
		if ( Texture[cnt].UseCounter && !Texture[cnt].lpD3DTexture ) {

			EnterCriticalSection( &cTextureSection );			//크리티칼 섹션 선언

			//######################################################################################
			//작 성 자 : 오 영 석
			//Result = smCreateTexture( &Texture[cnt] );
			Result = new_smCreateTexture( &Texture[cnt] );
			//######################################################################################

			if ( Result<=0 ) {
				switch( Result ) {
				case 0:
					wsprintf( szErrMsg , "TEX_D3D 오류 [%s]" , Texture[cnt].Name );
					break;
				case -1:
					wsprintf( szErrMsg , "TEX_FILE 오류 [%s]" , Texture[cnt].Name );
					break;
				case -2:
					wsprintf( szErrMsg , "TEX_ALPHA 오류 [%s]" , Texture[cnt].Name );
					break;
				}
				//오류 목록 삽입
				AddErrorList( szErrMsg );
			}
			ReadCount++;

			LeaveCriticalSection( &cTextureSection );		//크리티칼 섹션 해제

			if ( (ReadCount&0x7)==0 )	smPlayWindowMessage();	//메세지 처리
		}
	}

	return TRUE;
}

//잃어 버린 텍스쳐를 지우고 다시 불러 들인다
int smTEXTURE::Restore()
{
	return TRUE;
}


smTEXTURE smTexture;

////////////////////// smMATERIAL_GROUP ////////////////////////


smMATERIAL_GROUP::smMATERIAL_GROUP()
{

}

smMATERIAL_GROUP::smMATERIAL_GROUP( int MaterialMax )
{
	Init( MaterialMax );
}

smMATERIAL_GROUP::~smMATERIAL_GROUP()
{
	Close();
}

// 메트리얼 초기화
void smMATERIAL_GROUP::Init( int MaterialMax )
{

	MaterialCount = 0;
	MaxMaterial = MaterialMax;
	ReformTexture = 0;

	smMaterial = new smMATERIAL[ MaterialMax ];
	ZeroMemory( smMaterial , sizeof( smMATERIAL )*MaterialMax );

}

// 메트리얼 말기화
int smMATERIAL_GROUP::Close()
{
	DWORD cnt , tcnt;

	for( cnt=0;cnt<MaterialCount;cnt++ ) {
		if ( smMaterial[cnt].InUse ) {
			//텍스쳐를 제거한다
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter; tcnt++ ) 
				if ( smMaterial[cnt].smTexture[tcnt] )
					smTexture.Delete (  smMaterial[cnt].smTexture[tcnt]  );

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter; tcnt++ ) 
				if ( smMaterial[cnt].smAnimTexture[tcnt] )
					smTexture.Delete (  smMaterial[cnt].smAnimTexture[tcnt]  );

		}
	}

	delete smMaterial;

	return TRUE;
}

#define TEXFILENAME_SIZE	64

//저장할 데이타의 크기를 미리 알려줌
int smMATERIAL_GROUP::GetSaveSize()
{
	int size;
	DWORD cnt ,tcnt;
	int	len,alen;

	size	 = sizeof( smMATERIAL_GROUP );

	for(cnt=0;cnt<MaterialCount;cnt++) {
		size+= sizeof( smMATERIAL );
		if ( smMaterial[cnt].InUse ) {
			size += sizeof(int);					//텍스쳐 이름 길이가 들어갈 int형
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smTexture[tcnt]->Name )+1;
				alen = lstrlen( smMaterial[cnt].smTexture[tcnt]->NameA )+1;
				size += len;
				size += alen;
			}

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->Name )+1;
				alen = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->NameA )+1;
				size += len;
				size += alen;
			}

		}
	}
	return size;
}

//데이타를 파일로 저장 
int smMATERIAL_GROUP::SaveFile( HANDLE hFile )
{
	DWORD	dwAcess;
	DWORD cnt ,tcnt;
	int	len;
	int size;

	size =	WriteFile( hFile , &Head	, sizeof( smMATERIAL_GROUP )	, &dwAcess , NULL );

	for(cnt=0;cnt<MaterialCount;cnt++) {
		//메트리얼 저장
		size+= WriteFile( hFile , &smMaterial[cnt] , sizeof( smMATERIAL )	, &dwAcess , NULL );

		if ( smMaterial[cnt].InUse ) {

			//텍스쳐 파일 목록에 대한 이름 길이 계산
			len = 0;
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				len += lstrlen( smMaterial[cnt].smTexture[tcnt]->Name )+1;
				len += lstrlen( smMaterial[cnt].smTexture[tcnt]->NameA )+1;
			}
			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				len += lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->Name )+1;
				len += lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->NameA )+1;
			}
			//문자열 길이 미리 계산
			size+= WriteFile( hFile , &len , sizeof(int) , &dwAcess , NULL );

			//텍스쳐 파일 이름을 연속적으로 저장
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smTexture[tcnt]->Name )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smTexture[tcnt]->Name , len , &dwAcess , NULL );
				len = lstrlen( smMaterial[cnt].smTexture[tcnt]->NameA )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smTexture[tcnt]->NameA , len , &dwAcess , NULL );
			}

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				len = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->Name )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smAnimTexture[tcnt]->Name , len , &dwAcess , NULL );
				len = lstrlen( smMaterial[cnt].smAnimTexture[tcnt]->NameA )+1;
				size+= WriteFile( hFile , smMaterial[cnt].smAnimTexture[tcnt]->NameA , len , &dwAcess , NULL );
			}

		}
	}

	return size;
}

//파일에서 데이타를 로드 
int smMATERIAL_GROUP::LoadFile( HANDLE hFile )
{
	DWORD	dwAcess;
	DWORD	cnt ,tcnt;
	int		StrLen;
	int		size;
	char	szNameBuff[4096];
	char	*lpNameBuff;
	char	*szName , *szNameA;

	//그룹의 내용을 읽어옴
	size=ReadFile( hFile , &Head , sizeof( smMATERIAL_GROUP ) , &dwAcess , NULL );

	//메트리얼 메모리 잡음
	smMaterial = new smMATERIAL[ MaterialCount ];

	for(cnt=0;cnt<MaterialCount;cnt++) {
		//메트리얼 데이타 읽어옴
		size+=	ReadFile( hFile , &smMaterial[cnt] , sizeof( smMATERIAL ) , &dwAcess , NULL );

		if ( smMaterial[cnt].InUse ) {
			//텍스쳐 네임 버퍼 크기 읽음
			size+=	ReadFile( hFile , &StrLen , sizeof( int ) , &dwAcess , NULL );
			//텍스쳐 네임 버퍼 크기만큼 데이타 읽음 
			size+=	ReadFile( hFile , szNameBuff , StrLen, &dwAcess , NULL );

			lpNameBuff = szNameBuff;

			//텍스쳐 설정
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter ; tcnt++) {
				szName = lpNameBuff;
				lpNameBuff += lstrlen( szName )+1;
				szNameA = lpNameBuff;
				lpNameBuff += lstrlen( szNameA )+1;

				if ( szNameA[0] )
					smMaterial[cnt].smTexture[tcnt] = smTexture.Add( szName , szNameA );
				else
					smMaterial[cnt].smTexture[tcnt] = smTexture.Add( szName );
			}

			//에니메이션 텍스쳐 설정
			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter ; tcnt++) {
				szName = lpNameBuff;
				lpNameBuff += lstrlen( szName )+1;
				szNameA = lpNameBuff;
				lpNameBuff += lstrlen( szNameA )+1;

				if ( szNameA[0] )
					smMaterial[cnt].smAnimTexture[tcnt] = smTexture.Add( szName , szNameA );
				else
					smMaterial[cnt].smAnimTexture[tcnt] = smTexture.Add( szName );
			}
		}

	}

	return size;
}

//사용되지 않는 메트리얼 제거
int smMATERIAL_GROUP::DeleteUnuseMaterial()
{
	DWORD cnt , dcnt , tcnt;

	dcnt = 0;
	for( cnt=0;cnt<MaterialCount;cnt++ ) {
		if ( smMaterial[cnt].InUse==TRUE ) {
			//텍스쳐를 제거한다
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter; tcnt++ ) 
				smTexture.Delete (  smMaterial[cnt].smTexture[tcnt]  );

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter; tcnt++ ) 
				smTexture.Delete (  smMaterial[cnt].smAnimTexture[tcnt]  );

			ZeroMemory( &smMaterial[cnt] , sizeof( smMATERIAL ) );
			//smMaterial[cnt].InUse = FALSE;
			dcnt++;
		}
	}

	return dcnt;
}


//메트리얼을 등록
int smMATERIAL_GROUP::AddMaterial( ASE_MATERIAL *aseMaterial , char *szScript )
{
	int cnt,len;
	int	AnimCount,FrameSpeed;
	int MatNum;
	char	*lpAnimFileList[16];
	char	*szDirectory;
	char	*lpScript;
	char	szTemp[64];

	MatNum = MaterialCount;
	MaterialCount++;

	smMaterial[MatNum].InUse  = TRUE;
	smMaterial[MatNum].Shade  = TRUE;
	smMaterial[MatNum].BlendType = SMMAT_BLEND_ALPHA;
	smMaterial[MatNum].TwoSide = aseMaterial->TwoSide;
	smMaterial[MatNum].ReformTexture = 0;

	// 맵을 텍스쳐에 등록
	if ( !aseMaterial->MAP_OPACITY[0] ) {
		for( cnt=0;cnt<aseMaterial->TextureCounter;cnt++) {
			smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt] );
			smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
			smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
			smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
		}
		smMaterial[MatNum].MapOpacity = FALSE;
	}
	else {
		//######################################################################################
		//작 성 자 : 오 영 석
		if( aseMaterial->TextureCounter > 1 )
		{
			for( cnt=0; cnt < aseMaterial->TextureCounter-1; cnt++ )
			{
				smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt], aseMaterial->MAP_OPACITY );
				smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
				smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
				smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
			}

			smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt] );
			smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
			smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
			smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
		}
		else
		{
			for( cnt=0; cnt < aseMaterial->TextureCounter; cnt++ )
			{
				smMaterial[MatNum].smTexture[cnt] = smTexture.Add( aseMaterial->BITMAP[cnt] , aseMaterial->MAP_OPACITY );
				smMaterial[MatNum].TextureStageState[cnt] = aseMaterial->BitmapStateState[cnt];
				smMaterial[MatNum].TextureFormState[cnt] = aseMaterial->BitmapFormState[cnt];
				smMaterial[MatNum].ReformTexture += aseMaterial->BitmapFormState[cnt];
			}
		}

		smMaterial[MatNum].MapOpacity = TRUE;
		//######################################################################################
	}

	//좌표 변형 텍스쳐 유무 첵크하여 저장
	if ( smMaterial[MatNum].ReformTexture )	ReformTexture ++;

	smMaterial[MatNum].TextureCounter = aseMaterial->TextureCounter;	//텍스쳐맵의 수
	smMaterial[MatNum].Transparency   = aseMaterial->Transparency ;		//반투명도
	smMaterial[MatNum].SelfIllum	  = aseMaterial->SelfIllum ;		//자체발광
	smMaterial[MatNum].Diffuse        = aseMaterial->Diffuse;			//기본색상

	smMaterial[MatNum].UseState			= aseMaterial->ScriptState;
	smMaterial[MatNum].TextureType		= SMTEX_TYPE_MULTIMIX;

	if( aseMaterial->BlendType )
		smMaterial[MatNum].BlendType = aseMaterial->BlendType;

	//바닥 충돌 체크
	if ( smMaterial[MatNum].Transparency==0 ) 
		smMaterial[MatNum].MeshState		= SMMAT_STAT_CHECK_FACE;

	//나무 흔들리기
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WIND) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ1;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDX1) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDX1;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDX2) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDX2;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDZ1) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ1;
		smMaterial[MatNum].MeshState		= 0;
	}
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDZ2) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ2;
		smMaterial[MatNum].MeshState		= 0;
	}

	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WINDZ2) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WINDZ2;
		smMaterial[MatNum].MeshState		= 0;
	}

	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_WATER) ) {
		smMaterial[MatNum].WindMeshBottom = sMATS_SCRIPT_WATER;
		smMaterial[MatNum].MeshState		= 0;
	}

	//메트리얼 충돌 체크 허가
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_NOTPASS) ) {
		smMaterial[MatNum].MeshState		= SMMAT_STAT_CHECK_FACE;	//허가
	}
	else {
		if ( (aseMaterial->ScriptState&sMATS_SCRIPT_PASS) ) {
			smMaterial[MatNum].MeshState		= 0;					//불가
		}
	}
	//레더링 나중에 그리기
	if ( (aseMaterial->ScriptState&sMATS_SCRIPT_RENDLATTER) ) {
		smMaterial[MatNum].MeshState |= sMATS_SCRIPT_RENDLATTER;
	}

	//######################################################################################
	//작 성 자 : 오 영 석
	if( (aseMaterial->ScriptState & sMATS_SCRIPT_CHECK_ICE) )
		smMaterial[MatNum].MeshState |= sMATS_SCRIPT_CHECK_ICE;
	//######################################################################################

	//######################################################################################
	//작 성 자 : 오 영 석
	if( (aseMaterial->ScriptState & sMATS_SCRIPT_ORG_WATER) )
		smMaterial[MatNum].MeshState = sMATS_SCRIPT_ORG_WATER;
	//######################################################################################

	//자동 에니메이션
	AnimCount = 0;
	if ( szScript ) {
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM2 ) AnimCount=2;
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM4 ) AnimCount=4;
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM8 ) AnimCount=8;
		if ( aseMaterial->ScriptState&sMATS_SCRIPT_ANIM16 ) AnimCount=16;
	}

	if ( AnimCount ) {
		len = lstrlen(szScript);

		for( cnt=0;cnt<len;cnt++) {
			if ( szScript[cnt]=='a' && szScript[cnt+1]=='n' && 
				szScript[cnt+2]=='i' && szScript[cnt+3]=='m' ) {
					for( cnt=cnt;cnt<len;cnt++) {
						if ( szScript[cnt]==':' ) break;
					}
					break;
			}
		}

		if ( cnt<len ) {
			cnt++;
			lpScript = &szScript[cnt];
			FrameSpeed = 0;

			for(cnt=cnt;cnt<len;cnt++) {
				if ( szScript[cnt]==':' ) {
					FrameSpeed = atoi( &szScript[cnt+1] );
					szScript[cnt] = 0;
					break;
				}
			}

			szDirectory = SetDirectoryFromFile( aseMaterial->BITMAP[0] );

			if ( szDirectory ) {
				for(cnt=0;cnt<AnimCount;cnt++) {
					lpAnimFileList[cnt] = new char[64];
					lstrcpy( szTemp , szDirectory );
					lstrcat( szTemp , lpScript );
					wsprintf( lpAnimFileList[cnt] , szTemp , cnt );
				}
			
				ChangeMaterialToAnimation( &smMaterial[MatNum] , lpAnimFileList , AnimCount , smMaterial[MatNum].BlendType , TRUE );
				if ( FrameSpeed ) smMaterial[MatNum].Shift_FrameSpeed = FrameSpeed;			//에니메이션 타이머 쉬프트

				smMaterial[MatNum].Transparency   = aseMaterial->Transparency ;		//반투명도

				for(cnt=AnimCount-1;cnt>=0;cnt--) delete lpAnimFileList[cnt];
			}
		}
	}

	//######################################################################################
	// 작 성 자 : 오 영 석
	if( (aseMaterial->ScriptState & sMATS_SCRIPT_BLINK_COLOR) && smMaterial[MatNum].WindMeshBottom == 0 )
	{
		for( cnt=0; cnt < MAX_MAP_BLINK_COLOR_TIME; cnt++ )
		{
			if( strstr( szScript, szBlinkTimeScript[cnt] ) != 0 )
				break;
		}

		if( cnt >= MAX_MAP_BLINK_COLOR_TIME )
			smMaterial[MatNum].WindMeshBottom = dwBlinkTimeCode[0];
		else
			smMaterial[MatNum].WindMeshBottom = dwBlinkTimeCode[cnt];
	}
	//######################################################################################

	aseMaterial->Regist ++;												//메트리얼 목록에 등록
	aseMaterial->RegistNum = MatNum;

	return MatNum;
}

//텍스쳐 메트리얼 생성
int smMATERIAL_GROUP::CreateTextureMaterial( char *szBitmap , char *szOpacity , int TwoSide ,int Shade , DWORD TexSwap , DWORD BlendType )
{
	int MatNum;


	MatNum = MaterialCount;
	MaterialCount++;

	smMaterial[MatNum].InUse  = TRUE;
	smMaterial[MatNum].Shade  = Shade;
	smMaterial[MatNum].TwoSide = TwoSide;

	if ( szOpacity ) 
		smMaterial[MatNum].smTexture[0] = smTexture.Add( szBitmap , szOpacity );
	else 
		smMaterial[MatNum].smTexture[0] = smTexture.Add( szBitmap );


	smMaterial[MatNum].ReformTexture = 0;

	smMaterial[MatNum].TextureStageState[0] = 0;
	smMaterial[MatNum].TextureFormState[0] = 0;

	smMaterial[MatNum].smTexture[0]->TexSwapMode = TexSwap;
	smMaterial[MatNum].MapOpacity = FALSE;

	smMaterial[MatNum].TextureCounter = 1;			//텍스쳐맵의 수
	smMaterial[MatNum].Transparency   = 0;			//반투명도
//	smMaterial[MatNum].Diffuse        = 0;			//기본색상

	smMaterial[MatNum].UseState			= 0;
	smMaterial[MatNum].TextureType		= SMTEX_TYPE_MULTIMIX;
	smMaterial[MatNum].BlendType		= BlendType;


	smMaterial[MatNum].InUse++;

	return MatNum;
}

//메트리얼에 텍스쳐를 추가한다
int	smMATERIAL_GROUP::AddTexture( int MatNum , char *szBitmap , char *szOpacity )
{
	int cnt;

	cnt = smMaterial[MatNum].TextureCounter;			//텍스쳐맵의 수

	if ( szOpacity ) 
		smMaterial[MatNum].smTexture[cnt] = smTexture.Add( szBitmap , szOpacity );
	else 
		smMaterial[MatNum].smTexture[cnt] = smTexture.Add( szBitmap );

	if ( smMaterial[MatNum].smTexture[cnt] )
		smMaterial[MatNum].TextureCounter++;

	return TRUE;
}

//텍스쳐 이름으로 다음 메트리얼을 찾는다
smMATERIAL *smMATERIAL_GROUP::FindNextMaterialFromTextureName()
{
	DWORD cnt,cnt2;

	char *szName;

	szName = szLastSearchName;


	//빈 메트리얼 찾기
	for( cnt=LastSearchMaterial;cnt<MaterialCount;cnt++) {
		if ( smMaterial[cnt].InUse ) {

			for(cnt2=0;cnt2<smMaterial[cnt].TextureCounter;cnt2++) {
				if ( lstrcmpi( smMaterial[cnt].smTexture[cnt2]->Name , szName )==0 ) {
					return	&smMaterial[cnt];
				}
			}

			for(cnt2=0;cnt2<smMaterial[cnt].AnimTexCounter;cnt2++) {
				if ( lstrcmpi( smMaterial[cnt].smAnimTexture[cnt2]->Name , szName )==0 ) {
					return	&smMaterial[cnt];
				}
			}

		}
	}

	return NULL;
}

//텍스쳐 이름으로 메트리얼을 찾는다
smMATERIAL *smMATERIAL_GROUP::FindMaterialFromTextureName( char *szName )
{
	LastSearchMaterial = 0;

	lstrcpy( szLastSearchName, szName );

	return FindNextMaterialFromTextureName();
}

//메트리얼의 에니메이션 프레임을 설정
int smMATERIAL_GROUP::SetMaterialAnimFrame( smMATERIAL *lpMaterial , int frame )
{
	int OldFrame;

	OldFrame = lpMaterial->AnimationFrame;
	lpMaterial->AnimationFrame = frame;

	return OldFrame;
}



//메트리얼을 에니메이션 가능하게 변경
int smMATERIAL_GROUP::ChangeMaterialToAnimation( smMATERIAL *lpMaterial , char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	int cnt;

	for( cnt=0;cnt<NumTex;cnt++) {
		lpMaterial->smAnimTexture[cnt] = smTexture.Add( szBitmap[cnt] );
		lpMaterial->smAnimTexture[cnt]->TexSwapMode = FALSE;
	}


	lpMaterial->MapOpacity = FALSE;
	lpMaterial->TextureCounter = 1;//NumTex;		//텍스쳐맵의 수
	lpMaterial->Transparency   = 0;			//반투명도

	lpMaterial->AnimTexCounter	= NumTex;		//에니메이션 텍스쳐 카운터
	lpMaterial->FrameMask		= NumTex -1;	//에니메이션 프레임 마스크
	lpMaterial->Shift_FrameSpeed = 6;			//에니메이션 타이머 쉬프트

	lpMaterial->UseState			= 0;
	lpMaterial->TextureType		= SMTEX_TYPE_ANIMATION;
	lpMaterial->BlendType		= BlendType;

	//에니메이션 자동
	if ( AutoAnimMode )	
		lpMaterial->AnimationFrame	= SMTEX_AUTOANIMATION;
	else
		lpMaterial->AnimationFrame	= 0;

	lpMaterial->InUse++;

	return TRUE;
}


//애니메이션 메트리얼 생성
int smMATERIAL_GROUP::CreateAnimationMaterial( char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	int cnt;

	smMATERIAL *lpMaterial;

	cnt = MaterialCount;
	MaterialCount++;

	lpMaterial = &smMaterial[cnt];

	lpMaterial->InUse  = TRUE;
	lpMaterial->Shade  = FALSE;
	lpMaterial->TwoSide = TRUE;

	ChangeMaterialToAnimation( lpMaterial , szBitmap , NumTex , BlendType , AutoAnimMode );

	return cnt;
}



//텍스쳐 갱신용 프레임
int smMATERIAL_GROUP::IncTextureFrame()
{
	return TRUE;
}



//메트리얼의 텍스쳐 스와핑값을 조사후 설정한다 ( 로딩 완료후 가능 )
int smMATERIAL_GROUP::CheckMatreialTextureSwap()
{
	//######################################################################################
	//작 성 자 : 오 영 석
    return TRUE;
	//######################################################################################

	DWORD cnt , cnt2;

	//빈 메트리얼 찾기
	for( cnt=0;cnt<MaterialCount;cnt++) 
		if ( smMaterial[cnt].InUse ) {
			if ( smMaterial[cnt].TextureType==SMTEX_TYPE_MULTIMIX ) {
				for( cnt2=0; cnt2<smMaterial[cnt].TextureCounter; cnt2++ ) {
					if ( smMaterial[cnt].smTexture[cnt2] && smMaterial[cnt].smTexture[cnt2]->lpDDSSysMemory ) {
						smMaterial[cnt].TextureSwap = TRUE;
						break;
					}
				}
			}

			if ( smMaterial[cnt].TextureType==SMTEX_TYPE_ANIMATION ) {
				for( cnt2=0; cnt2<smMaterial[cnt].AnimTexCounter; cnt2++ ) {
					if ( smMaterial[cnt].smAnimTexture[cnt2] && smMaterial[cnt].smAnimTexture[cnt2]->lpDDSSysMemory ) {
						smMaterial[cnt].TextureSwap = TRUE;
						break;
					}
				}
			}

		}

	return TRUE;
}


//메트리얼의 스와핑 텍스쳐를 찾아 설정한다
int smMATERIAL_GROUP::ReSwapMaterial( smMATERIAL *lpMaterial )
{
	//######################################################################################
	//작 성 자 : 오 영 석
    return TRUE;
	//######################################################################################

	DWORD cnt;
	int result;

	lpMaterial->MatFrame = MaterialFrame;

	lpMaterial->TextureClip = FALSE;

	if ( lpMaterial->TextureSwap ) {
		result = 0;
		for( cnt=0;cnt<lpMaterial->TextureCounter;cnt++) {
			result |= smTexture.SetLargeTexture( lpMaterial->smTexture[cnt] );
		}

		if ( result ) {
			lpMaterial->TextureClip = TRUE;
			return TRUE;
		}
	}

	return FALSE;
}

//######################################################################################
//작 성 자 : 오 영 석
//해당 메트리얼의 모든 텍스쳐를 로딩
int smMATERIAL_GROUP::ReadTextures( int MipMap )
//######################################################################################
{
	//######################################################################################
	//작 성 자 : 오 영 석
	if( g_IsReadTextures == 0 )
		return 0;
	//######################################################################################

	DWORD cnt;
	DWORD tcnt;
	int	Result;
	int	ReadCount;
	smTEXTUREHANDLE	*lpTexture;

	//텍스쳐 영역 크리티칼 섹션 선언
	smEnterTextureCriticalSection();

	ReadCount = 0;

	for( cnt=0;cnt<MaterialCount;cnt++ ) {
		if ( smMaterial[cnt].InUse ) {
			for( tcnt=0; tcnt<smMaterial[cnt].TextureCounter; tcnt++ ) {
				if ( smMaterial[cnt].smTexture[tcnt] ) {
					lpTexture = smMaterial[cnt].smTexture[tcnt];

					if ( lpTexture->UseCounter && !lpTexture->lpD3DTexture )
					{
						//######################################################################################
						//작 성 자 : 오 영 석
						//Result = smCreateTexture( lpTexture, MipMap );
						Result = new_smCreateTexture( lpTexture, MipMap );
						//######################################################################################

						ReadCount++;
					}
				}
			}

			for( tcnt=0; tcnt<smMaterial[cnt].AnimTexCounter; tcnt++ ) {
				if ( smMaterial[cnt].smAnimTexture[tcnt] ) {
					lpTexture = smMaterial[cnt].smAnimTexture[tcnt];
					if ( lpTexture->UseCounter && !lpTexture->lpD3DTexture )
					{
						//######################################################################################
						//작 성 자 : 오 영 석
						//Result = smCreateTexture( lpTexture, MipMap );
						Result = new_smCreateTexture( lpTexture, MipMap );
						//######################################################################################

						ReadCount++;
					}
				}
			}
		}
	}

	//텍스쳐 영역 크리티칼 섹션 해제
	smLeaveTextureCriticalSection();

	return ReadCount;
}































////////////////////////////////////////////////////////////////////////////////////////////////


//smMATERIAL smMaterial[ MATRIAL_MAX ];
smMATERIAL *smMaterial;

smMATERIAL_GROUP	*smMaterialGroup;

// 메트리얼 초기화
int InitMaterial()
{
//	smMaterialGroup.Init( MATRIAL_MAX );
	smMaterialGroup = new smMATERIAL_GROUP( MATRIAL_MAX );
	smMaterial = smMaterialGroup->smMaterial;

	MaterialFrame = 0;

	return TRUE;
}

//메트리얼 종료
int CloseMaterial()
{
//	smMaterialGroup.Close();
	delete smMaterialGroup;
	return TRUE;
}

//사용되지 않는 메트리얼 제거
int DeleteUnuseMaterial()
{
	return smMaterialGroup->DeleteUnuseMaterial();
}

//메트리얼을 등록
int AddMaterial( ASE_MATERIAL *aseMaterial )
{
	return smMaterialGroup->AddMaterial( aseMaterial );
}

//메트리얼에 텍스쳐를 추가한다
int	AddTexture( int MatNum , char *szBitmap , char *szOpacity )
{
	return smMaterialGroup->AddTexture( MatNum , szBitmap , szOpacity );
}

//텍스쳐 이름으로 다음 메트리얼을 찾는다
smMATERIAL *FindNextMaterialFromTextureName()
{
	return smMaterialGroup->FindNextMaterialFromTextureName();
}

//텍스쳐 이름으로 메트리얼을 찾는다
smMATERIAL *FindMaterialFromTextureName( char *szName )
{
	return smMaterialGroup->FindMaterialFromTextureName( szName );
}

//메트리얼의 에니메이션 프레임을 설정
int SetMaterialAnimFrame( smMATERIAL *lpMaterial , int frame )
{
	return smMaterialGroup->SetMaterialAnimFrame( lpMaterial , frame );
}


//메트리얼을 에니메이션 가능하게 변경
int ChangeMaterialToAnimation( smMATERIAL *lpMaterial , char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	return smMaterialGroup->ChangeMaterialToAnimation( lpMaterial, szBitmap, NumTex, BlendType, AutoAnimMode );
}


//애니메이션 메트리얼 생성
int CreateAnimationMaterial( char **szBitmap , int NumTex , DWORD BlendType , int AutoAnimMode )
{
	return smMaterialGroup->CreateAnimationMaterial( szBitmap, NumTex, BlendType, AutoAnimMode );
}


//텍스쳐 갱신용 프레임
int IncTextureFrame()
{
	MaterialFrame++;
	return smTexture.SwapCount ++;
}

//메트리얼의 텍스쳐 스와핑값을 조사후 설정한다 ( 로딩 완료후 가능 )
int CheckMatreialTextureSwap()
{
	return smMaterialGroup->CheckMatreialTextureSwap();
}


//메트리얼의 스와핑 텍스쳐를 찾아 설정한다
int ReSwapMaterial( smMATERIAL *lpMaterial )
{
	return smMaterialGroup->ReSwapMaterial( lpMaterial );
}




int InitTexture()
{
	return TRUE;
}

int CloseTexture()
{
	smTexture.Remove();

	return TRUE;
}

int ReadTextures()
{
	//######################################################################################
	//작 성 자 : 오 영 석
	if( g_IsReadTextures == 0 )
		return 0;
	//######################################################################################

	int val;

	val = smTexture.ReadTexture();
	CheckMatreialTextureSwap();

	return val;
}

//텍스쳐를 첵크하여 잃어버린 텍스쳐를 다시 만든다
int RestoreTextures()
{
	return smTexture.Restore();
}

/*
int LoadDibTextureNum( char *Filename )
{

	if ( smTexture.AddName( Filename ) )
		return smTexture.TextureCnt;
	else 
		return -1;
}
*/





void* *GetLastTextureSurface()
{
	return smTexture.lpLastTextureSurface;
}



//텍스쳐 영역 크리티칼 섹션 선언
void smEnterTextureCriticalSection()
{
	EnterCriticalSection( &smTexture.cTextureSection );
}

//텍스쳐 영역 크리티칼 섹션 해제
void smLeaveTextureCriticalSection()
{
	LeaveCriticalSection( &smTexture.cTextureSection );
}

int hresult;


////////////////// BMP FILE READ ///////////////////
DWORD GetDibInfoHeaderSize(BYTE  *lpDib)
{
	return ((BITMAPINFOHEADER  *)lpDib)->biSize;
}
WORD GetDibWidth(BYTE  *lpDib)
{
	if (GetDibInfoHeaderSize(lpDib)+sizeof(BITMAPCOREHEADER))
		return (WORD) (((BITMAPCOREHEADER  *)lpDib)->bcWidth);
	else
		return (WORD) (((BITMAPINFOHEADER  *)lpDib)->biWidth);
}
WORD GetDibHeight (BYTE  *lpDib)
{
	if (GetDibInfoHeaderSize(lpDib)==sizeof(BITMAPCOREHEADER))
		return (WORD) (((BITMAPCOREHEADER  *) lpDib)->bcHeight);
	else
		return (WORD) (((BITMAPINFOHEADER  *) lpDib)->biHeight);
}

WORD GetDibBitCount(BYTE  *lpDib)
{
	return ((BITMAPINFOHEADER  *)lpDib)->biBitCount;
}

BYTE  *GetDibBitsAddr(BYTE  *lpDib)
{
	DWORD dwNumColors, dwColorTableSize;
	WORD  wBitCount;

	if (GetDibInfoHeaderSize(lpDib)==sizeof(BITMAPCOREHEADER))
	{
		wBitCount=((BITMAPCOREHEADER  *) lpDib)->bcBitCount;
		if(wBitCount!=24)
			dwNumColors=1L<<wBitCount;
		else dwNumColors=0;
		dwColorTableSize=dwNumColors*sizeof(RGBTRIPLE);
	}
	else
	{
		wBitCount=((BITMAPINFOHEADER  *)lpDib)->biBitCount;
		if(GetDibInfoHeaderSize(lpDib)>=36)
			dwNumColors=((BITMAPINFOHEADER  *)lpDib)->biClrUsed;
		else
			dwNumColors=0;
		if(dwNumColors==0)
		{
			if(wBitCount!=24)
				dwNumColors=1L<<wBitCount;
			else
				dwNumColors=0;
		}
		dwColorTableSize=dwNumColors*sizeof(RGBQUAD);
	}
	return lpDib+GetDibInfoHeaderSize(lpDib)+dwColorTableSize;
}



/////////////////////////[ BMP 파일 로드 ]///////////////////////////////
BYTE *LoadDib( char *Filename )
{
	BITMAPFILEHEADER	bmfh;
	BYTE 				*lpDib;
	DWORD				dwDibSize,dwHeaderSize;
	HANDLE	hFile;
	DWORD	dwAcess;


	//파일을 연다
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return NULL;
	//비트맵 헤더 읽음
	ReadFile( hFile , &bmfh , sizeof( BITMAPFILEHEADER ) , &dwAcess , NULL );
	if (bmfh.bfType != *(WORD *)"BM" && bmfh.bfType != *(WORD *)"C8" && bmfh.bfType != *(WORD *)"A8" ) 
	{ CloseHandle( hFile ); return NULL; }

	//헤더 암호화 버전
	if ( bmfh.bfType == *(WORD *)"A8" ) {
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//헤더를 암호화
		lpHeader = (BYTE *)&bmfh;
		for( cnt=0;cnt<sizeof(bmfh);cnt++) {
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	dwDibSize=bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	lpDib = new BYTE [dwDibSize];
	if(lpDib==NULL)	{ CloseHandle( hFile );	return NULL; }
	//비트맵 읽음
	ReadFile( hFile , lpDib , dwDibSize , &dwAcess , NULL );
	CloseHandle( hFile );
	dwHeaderSize=GetDibInfoHeaderSize(lpDib);
	if (dwHeaderSize<12||(dwHeaderSize>12 && dwHeaderSize < 16) ) 
	{	delete lpDib; return NULL; }

	return lpDib;
}

//////////////////// TGA 파일 읽어들이는 크래스 //////////////////
class cTGA	{
public:
	BYTE	*m_pBuff;
	BYTE	*m_pData;
	int		Width , Height;
	int		SwapHeight;
	int		BitCount;

	cTGA();
	~cTGA();

	int LoadTga( char *Filename );
	int GetWidth();
	int GetHeight();
	int GetBitCount();

};

cTGA::cTGA()
{
	m_pBuff = 0;
	m_pData = 0;
	Width = 0;
	Height = 0;
	BitCount = 0;
}

cTGA::~cTGA()
{
	if ( m_pBuff ) delete m_pData;
}

// TGA파일 로드
int cTGA::LoadTga( char *Filename )
{
	int len;
	BYTE TGAHeader[0x12];
	WORD *Bit16Buff;
	WORD r,g,b;

	int DestCnt;
	int SrcCnt;

	HANDLE	hFile;
	DWORD	dwAcess;

	//파일을 연다
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if ( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	//TGA 헤더 읽음
	ReadFile( hFile , TGAHeader , 0x12 , &dwAcess , NULL );

	//헤더 암호화 버전
	if ( TGAHeader[0]=='G' && TGAHeader[1]=='8' ) {
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//헤더를 암호화
		lpHeader = (BYTE *)&TGAHeader;
		for( cnt=0;cnt<sizeof(TGAHeader);cnt++) {
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	Width = *((WORD *)(&TGAHeader[0xC]));
	Height = *((WORD *)(&TGAHeader[0xE]));

	if ( (TGAHeader[0x11]&0x20) ) {
		SwapHeight = FALSE;
	}
	else
		SwapHeight = TRUE;

	switch( TGAHeader[0x10] ) {
	case 16:		//555 를 RGB 888 로 변환
		m_pBuff = new BYTE [Width*Height*3];
		len = Width*Height;
		Bit16Buff = new WORD [ len ];
		ReadFile( hFile , Bit16Buff , len*2 , &dwAcess , NULL );
		//fread( Bit16Buff , len*2 , 1, fp );
		DestCnt = 0;
		for( SrcCnt=0;SrcCnt<len;SrcCnt++ ) {
			r = ( Bit16Buff[SrcCnt]>>10 ) & 0x1F;
			g = ( Bit16Buff[SrcCnt]>>5 ) & 0x1F;
			b = ( Bit16Buff[SrcCnt] ) & 0x1F;

			m_pBuff[ DestCnt   ] = (r<<3) | (r>>2);
			m_pBuff[ DestCnt+1 ] = (g<<3) | (g>>2);
			m_pBuff[ DestCnt+2 ] = (b<<3) | (b>>2);
			DestCnt+=3;
		}
		delete Bit16Buff;
		BitCount = 24;
		break;
	case 24:
		m_pBuff = new BYTE [Width*Height*3];
		ReadFile( hFile , m_pBuff , Width*Height*3 , &dwAcess , NULL );
		//fread( m_pBuff , Width*Height*3 , 1, fp );
		BitCount = 24;
		break;
	case 32:
		m_pBuff = new BYTE [Width*Height*4];
		ReadFile( hFile , m_pBuff , Width*Height*4 , &dwAcess , NULL );
		//fread( m_pBuff , Width*Height*4 , 1, fp );
		BitCount = 32;
		break;
	}
	CloseHandle( hFile );

	m_pData = m_pBuff;

	return TRUE;
}

int cTGA::GetWidth()
{
	return Width;
}

int cTGA::GetHeight()
{
	return Height;
}

int cTGA::GetBitCount()
{
	return BitCount;
}

#define smTEXFILETYPE_BMP		0
#define smTEXFILETYPE_JPG		1
#define smTEXFILETYPE_TGA		2

//파일의 확장자를 검사하여 파일 포맷을 찾는다
int GetTextureFileType( char *Filename ) 
{
	int len;
	int cnt;
	char *szFileExt = 0;

	len = lstrlen( Filename );

	for(cnt=len-1;cnt>=0;cnt--) {
		if ( Filename[cnt]=='.' ) {
			szFileExt = Filename+cnt+1;
			break;
		}
	}

	if ( !szFileExt ) 
		return -1;

	if ( ( szFileExt[0]=='j' || szFileExt[0]=='J') && 
		 ( szFileExt[1]=='p' || szFileExt[1]=='P') &&
		 ( szFileExt[2]=='g' || szFileExt[2]=='G') ) 
		 return smTEXFILETYPE_JPG;

	if ( ( szFileExt[0]=='b' || szFileExt[0]=='B') && 
		 ( szFileExt[1]=='m' || szFileExt[1]=='M') &&
		 ( szFileExt[2]=='p' || szFileExt[2]=='P') ) 
		 return smTEXFILETYPE_BMP;

	if ( ( szFileExt[0]=='t' || szFileExt[0]=='T') && 
		 ( szFileExt[1]=='g' || szFileExt[1]=='G') &&
		 ( szFileExt[2]=='a' || szFileExt[2]=='A') ) 
		 return smTEXFILETYPE_TGA;


	if ( ( szFileExt[0]=='s' || szFileExt[0]=='S') && 
		 ( szFileExt[1]=='t' || szFileExt[1]=='T') &&
		 ( szFileExt[2]=='g' || szFileExt[2]=='G') ) 
		 return smTEXFILETYPE_JPG;

	if ( ( szFileExt[0]=='s' || szFileExt[0]=='S') && 
		 ( szFileExt[1]=='t' || szFileExt[1]=='T') &&
		 ( szFileExt[2]=='a' || szFileExt[2]=='A') ) 
		 return smTEXFILETYPE_TGA;

	return -1;
}



int LoadDibSurfaceAlphaMipMap( void* lpDDSurface, char *Filename, char *FileAlpha, int CheckWidth, int CheckHeight )
{
	return TRUE;
}


/////////[ BMP파일을 텍스쳐 서페이스로 만든다 ( 알파채널 사용 ) ] ///////////
void* LoadDibSurfaceAlpha( char *Filename , char *FileAlpha , int SysMem )
{

	return nullptr;

}


//////////////// [ BMP파일을 텍스쳐 서페이스로 만든다 ] ///////////////
void* LoadDibSurface( char *Filename , int SysMem )
{
	return nullptr;
}

// Dib를 서페이스에 그린다
int DrawSurfaceFromDib(void* lpDDSurface, BYTE* lpDib) { return TRUE; }






int MipMapCreateTextureCheck( int width, int height, int MipMapCount )
{
	if( width > 256 || height > 256 )
		return 0;

	switch( MipMapCount )
	{
	case 5:
		if( width == 256 && height == 256 )
			return 1;
		break;

	case 4:
		if( width >= 128 && height >= 128 )
			return 1;
		break;

	case 3:
		if( width >= 64 && height >= 64 )
			return 1;
		break;

	case 2:
		if( width >= 32 && height >= 32 )
			return 1;
		break;

	case 1:
		return 0;

	default:
		return 0;
	}

	return 0;
}
//######################################################################################





//######################################################################################
//작 성 자 : 오 영 석
//////////////// [ BMP파일을 텍스쳐 서페이스로 만든다 (자동 스왑) ] ///////////////
int smCreateTexture(smTEXTUREHANDLE* lpTexHandle, int MipMap) { return TRUE; }












extern HWND hwnd;


void*	lpDDSMapTexture[256];
void*	lpDDSMapTextureVideo[8*8];
void*	lpD3DMapTexture[8*8];

int MapTexCode[8][8];

int MapTextureSize;

int LoadDibMapTexture( char *Filename, int tSize ) { return TRUE; }

int RemoveMapTexture() { return TRUE; }



int SetMapTextureMem( int x, int y ) { return TRUE; }

int SetMapTextureWidth( int x, int y )
{
	int cnt;

	for( cnt=0;cnt<8;cnt++ ) {
		SetMapTextureMem( x+cnt, y );
	}

	return TRUE;
}

int SetMapTextureHeight( int x, int y )
{
	int cnt;

	for( cnt=0;cnt<8;cnt++ ) {
		SetMapTextureMem( x, y+cnt );
	}


	return TRUE;
}

int SetMapImage( int x , int y )
{
	int sx,sy;
	int cntX,cntY;

	sx = x>>(ShiftMAPCELL_DIV+3);
	sy = y>>(ShiftMAPCELL_DIV+3);


	for( cntX=sx-3;cntX<sx+4;cntX++) {
		for( cntY=sy-3;cntY<sy+4;cntY++) {
			SetMapTextureMem( cntX, cntY );
		}
	}


/*
	SetMapTextureWidth( sx-3 , sy+3 );
	SetMapTextureWidth( sx-3 , sy-3 );

	SetMapTextureWidth( sx+3 , sy-3 );
	SetMapTextureWidth( sx-3 , sy-3 );
*/
	return TRUE;

}








void* LoadDibBGTexture(char* Filename) { return nullptr; }



void*	lpDDSAnimBG[32];

int DrawAnimBG(void* lpDDSSrc, int frame) { return TRUE; }

int LoadAnimBG() { return TRUE; }

int ClearBG( void* lpDDSSrc, DWORD color ) { return TRUE; }

int CloseAnimBG() { return TRUE; }


//######################################################################################
//작 성 자 : 오 영 석
int new_smCreateTexture( smTEXTUREHANDLE *lpTexHandle, int MipMap ) { return TRUE; }


/////////////////////////////////////////////////////////////////////////////////////////
//
int new_LoadDib( char *Filename )
{
	HANDLE				hFile;
	DWORD				dwAcess;
	BITMAPFILEHEADER	bmfh;

	//파일을 연다
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;

	//비트맵 헤더 읽음
	ReadFile( hFile , &bmfh , sizeof( BITMAPFILEHEADER ) , &dwAcess , NULL );
	if( bmfh.bfType != *(WORD *)"BM" && bmfh.bfType != *(WORD *)"C8" && bmfh.bfType != *(WORD *)"A8" ) 
	{
		CloseHandle( hFile );
		return NULL;
	}

	//헤더 암호화 버전
	if( bmfh.bfType == *(WORD *)"A8" )
	{
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//헤더를 암호화
		lpHeader = (BYTE *)&bmfh;
		for( cnt=0;cnt<sizeof(bmfh);cnt++)
		{
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	DWORD dwDibSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	BYTE *lpDib     = g_DibImageDateBuff;
	
	//비트맵 읽음
	ReadFile( hFile , lpDib , dwDibSize , &dwAcess , NULL );
	CloseHandle( hFile );

	DWORD dwHeaderSize = GetDibInfoHeaderSize(lpDib);
	if( dwHeaderSize < 12 || (dwHeaderSize>12 && dwHeaderSize < 16) ) 
		return NULL;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
int new_LoadTga( char *Filename )
{
    BYTE	TGAHeader[0x12];
	HANDLE	hFile;
	DWORD	dwAcess;

	//파일을 연다
	hFile = CreateFile( Filename , GENERIC_READ , FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL );
	if( hFile == INVALID_HANDLE_VALUE )
		return NULL;

	//TGA 헤더 읽음
	ReadFile( hFile , TGAHeader , 0x12 , &dwAcess , NULL );

	//헤더 암호화 버전
	if ( TGAHeader[0]=='G' && TGAHeader[1]=='8' ) {
		BYTE	ch;
		BYTE	*lpHeader;
		int		cnt;

		//헤더를 암호화
		lpHeader = (BYTE *)&TGAHeader;
		for( cnt=0;cnt<sizeof(TGAHeader);cnt++) {
			ch = (BYTE)( lpHeader[cnt]-(cnt*cnt) );
			lpHeader[cnt] = ch;
		}
	}

	g_DibLastReadWidth  = *((WORD *)(&TGAHeader[0xC]));
	g_DibLastReadHeight = *((WORD *)(&TGAHeader[0xE]));

	if ( (TGAHeader[0x11]&0x20) )
		g_TgaLastSwapHeight = FALSE;
	else
		g_TgaLastSwapHeight = TRUE;

	switch( TGAHeader[0x10] )
	{
	//case 16:
	case 24:
		g_TgaLastBitCnt = 24;
		ReadFile( hFile, g_DibImageDateBuff, g_DibLastReadWidth*g_DibLastReadHeight*3, &dwAcess, NULL );
		break;

	case 32:
		g_TgaLastBitCnt = 32;
		ReadFile( hFile, g_DibImageDateBuff, g_DibLastReadWidth*g_DibLastReadHeight*4, &dwAcess, NULL );
		break;

	default:
		CloseHandle( hFile );
		return NULL;
	}

	CloseHandle( hFile );
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////
//
int New_DrawSurfaceFromTga( void* lpDDSurface ) { return TRUE; }


/////////////////////////////////////////////////////////////////////////////////////////
//
void* new_LoadBmpCreateTexSurface( char *Filename, int MipMap, int &rTexHalf ) { return nullptr; }


/////////////////////////////////////////////////////////////////////////////////////////
//
void* new_LoadTgaCreateTexSurface( char *Filename, int MipMap, int &rTexHalf ) { return nullptr; }


/////////////////////////////////////////////////////////////////////////////////////////
//
void* new_LoadBmpSurfaceOffscreen( char *Filename ) { return nullptr; }
//######################################################################################


