
#include "sinLinkHeader.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"
#include "fileread.h"

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "field.h"
#include "SrcServer\\onserver.h"
//ReformItem( sITEMINFO *lpItemInfo ); //아이템 인증받기 
//CheckItemForm( sITEMINFO *lpItemInfo ); //아이템 인증확인 


//ReformCharForm(); //재인증 
//CheckCharForm(); //인증 
int LbuttonUpCheckFlag = 0; //이벤트 딜레이를 주기위해서 

int sinInterHeight2 = -50; //상점위치에 있는 인터페이스의 위치를 보정한다 

int sinTempDamage2[2] = {0,0}; //디스플레이시에만 보여줄 데미지
int sinTempAbsorb = 0;
int sinTempBlock  = 0;

sITEMINFO TempLinkCoreItem;
POINT UnionCorePosi = {0,0};
int   sinOverWeightCnt = 0;
int sinMyShopItemPass = 0;  


// 장별 - 입력창
int nName = 0;
int nYesButton= 0;
int nNoButton = 0;
int BackUpPosX , BackUpPosY = 0;
int BackUpButton2X , BackUpButton2Y = 0;
int BackUpButton3X , BackUpButton3Y = 0;

/*----------------------------------------------------------------------------*
*								전역변수 			
*-----------------------------------------------------------------------------*/	
cINVENTORY	cInvenTory;

sINVENTORY_EFFECT	sInven_Effect[MAX_INVEN_PARTICLE];	//인벤 파티클 
sINVENTORY_EFFECT   UnionCore_Effect[100];				//유니온 코어 인벤 파티클 
sINVENTORY_EFFECT   UnionCore_Effect_Bone[10];			//유니온 코어 인벤 파티클 
sINVENTORY_EFFECT   UnionCore_Effect_Bomb[50];			//유니온 코어 터지는 이펙트

sINVENTORY_EFFECT   ForceOrb_Effect;  //ForceOrb 이펙트 
int MatForceOrbEffect[8] = {0};
sITEM MyShopMouseItem;

sELEMENT_ATTACK	sElement_Attack;
sUNIONCORE_EFFECT   sUCore_Effect[10];

int StartX;  //인벤 박스의 시작점 X
int StartY;  // Y

int EndX;    //박스크기하나를 뺀 마지막 지점 X
int EndY;    //Y 

int ColorIndex;					//컬러 박스의 인덱스 
RECT ColorRect = {0,0,0,0};		//색이 표시될 영역 

int CrashItemIndex[2] = {0,0};	//아이템이 2개이상 겹치게되면 셋팅을 할수없다 
int AutoSetItemIndex = 0;       //양손무기를 셋팅할경우 자동으로 셋팅될 아이템의 인덱스 

int SelectInvenItemIndex=0;		//선택될 아이템의 인덱스 

int SelectPotionIndex = 0;

int ArrowState[2] = {0,0};

int InvenArrowPosi = 0;

sITEM *pUsePotion = 0;          //사용할 포션의 포인터 

int sinLoadOK=0;				//로드가 되면 1을 준다 

int PotionIndex=0;              //박스 거래를 위해 인덱스를 저장한다

sITEM *sinPosionItem= 0;        //물약 문제해결을 위한 포인터 
int BackUpItemIndex = 0;		//물약 문제해겨을 위한 아이템 인덱스 
int NotPotionFlag = 0;			//물약을 집었는지 아닌지를 채크 	

char sinTestBuff[128];			//테스트를 하기위한 버퍼 

int GhostPotionCheckFlag=0;		// 'E'키로인한 물약 사라지는 버그를 수정하기위한 날치기 플랙 

int sinChangeSetFlag = 0;

int sinUsePotionDelayFlag = 0;
int PotionCheckNumJumpFlag = 1;

/////////////// 스킬관련
int sinUndeadAbsorb = 0;

int sinItemPickUpDelayFlag = 0;
float   sinAdd_fMagic_Mastery	= 0;			//마법숙련도 
int sinDamageFunction[3] = {0,0,0};

void*	lpInvenMain;

void*	lpUnionCoreEffect[4];

int MatUnionCoreEffect = 0;

void*   lpGold7=0;
void*   lpExp7=0;


void*   lpSpecialItem[2];
void*   lpSpecialItemInfo[2];

void*   lpSpecialInven;


int IndiShopIconFlag = 0;

//아이템 Make
sWINGQUEST sWingQuest[MAX_WINGQUEST] = {
	#include "sinWingItem.h"
};


int SpecialItemShowFlag = 0;
int NotChangeSetItemFlag = 0;
int PuzzleOkFlag = 0;

int PuzzleOkY = 0;

int sinTestFlag2 = 0;

/*
void haTest(char *buff)
{
	HANDLE hFile;
    DWORD dwAcess =0;
	
    hFile = CreateFile( "ha.log", GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE,
                        NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

    if( hFile != INVALID_HANDLE_VALUE ) {
        SetFilePointer( hFile , 0 , NULL , FILE_END );
		strcat(buff,"\r\n");
        //데이타 쓰기     버퍼    쓸크기    실제로 쓰여진 바이트수
        int chk = WriteFile( hFile , buff , lstrlen(buff) , &dwAcess , NULL );
        //파일 닫기
        CloseHandle( hFile );
   }
}
*/
///////////캐릭터 능력치 + 아이템 (색을 바꿔주려고 외부선언함 Flag 안쓰려고 음하하하)
/*----------------------------------------------------------------------------*
*   	                      아이템이 셋팅될 영역 
*-----------------------------------------------------------------------------*/	
sINVENTORY sInven[INVENTORY_MAX_POS] = {
	{INVENTORY_POS_RHAND  ,{ 574+1 , 433+1 , 640 , 544}},  //오른손			0
	{INVENTORY_POS_LHAND  ,{ 722+1 , 433+1 , 788 , 544}},  //왼손			1
	{INVENTORY_POS_ARMOR  ,{ 648+1 , 433+1 , 714 , 544}},  //갑옷			2
	{INVENTORY_POS_AMULET ,{ 304+1 , 438+1 , 326 , 460}},  //목걸이			3
	{INVENTORY_POS_RRING  ,{ 356+1 , 438+1 , 378 , 460}},  //오른쪽 링		4
	{INVENTORY_POS_LRING  ,{ 330+1 , 438+1 , 352 , 460}},  //왼쪽 링		5
	{INVENTORY_POS_SHELTOM,{ 382+1 , 438+1 , 404 , 460}},  //쉘텀			6
	{INVENTORY_POS_ARMLET ,{ 411+1 , 438+1 , 455 , 482}},  //암렛			7
	{INVENTORY_POS_GLOVES ,{ 462+1 , 438+1 , 506 , 482}},  //장갑			8
	{INVENTORY_POS_BOOTS  ,{ 513+1 , 438+1 , 557 , 482}},  //신발			9
	{INVENTORY_POS_POTION ,{ 495   , 565   , 495+26 , 565+26}},  //포션 1			10
	{INVENTORY_POS_POTION ,{ 495+26 , 565  , 495+26+26 , 565+26}},  //포션 2			11
	{INVENTORY_POS_POTION ,{ 495+26+26 , 565 , 495+26+26+26 , 565+26}},  //포션 3			12
};

//화살표 좌표 
int ArrowPosi[9][4] = {
	{26,572,26+22,591},
	{48,572,48+22,591},
	{519,514,538,533},
	{541,514,560,533},
	{266,572,266+22,591},//인벤 닫기 
	{302,467,321,486},//무게 아이콘 
	{120,572,120+20,591},//돈 아이콘 
	{70,572,70+22,591},//퀘스트 아이템
	{98,572,98+22,591},//개인상점
};


int InvenTextPosi[2][4] = {
	{121,579,209,592}, //돈 
	{322,473,404,486}, //무게 

};
//Force Orb 를 사용한다
int ForceOrbUseLevel[][2] = {												 // 박재원 : 빌링 포스 추가(요구 레벨 50+)
	{1,19},{1,29},{1,39},{1,49},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},{50,200},
};

// 박재원 - 매직 포스(사용 레벨, 요구 레벨)
int MagicForceOrbUseLevel[][2] = {	
	{1,19},{1,29},{1,39},{1,49},{50,200},{50,200},{60,200},{70,200},{80,200},{80,200},{90,200},{90,200},
};

// 박재원 - 빌링 매직 포스 추가(사용 레벨)
int BillingMagicForceOrbUseLevel[][2] = {	// 박재원 - 빌링 매직 포스 추가(요구 레벨)
	{1,200},{1,200},{1,200},
};

int ForceOrbUseTime[] = { 500,500,500,500,700,900,1800,2400,3600,4800,6000,7200,8400,10000,12000 };

// 박재원 - 매직 포스 추가 
int MagicForceOrbUseTime[] = {500,500,500,500,700,900,1800,2400,3600,3600,3600,3600,3600,3600}; // 박재원 - 매직 포스 추가(유지시간 추가)

// 박재원 - 빌링 매직 포스 추가
int BillingMagicForceOrbUseTime[] = {3600, 10800, 86400}; // 박재원 - 빌링 매직 포스 유지시간 추가

int ForceOrbDamage[]  = {2,4,7,10,15,25,40,60,80,100,120,140,160,180,200}; // 박재원 - 빌링 포스 추가 공격력(벨룸, 오르도 포스 추가)
int MagicForceOrbDamage[]  = {2,4,7,10,15,25,40,60,80,100,120,140,160,180}; // 박재원 - 매직 포스 추가 공격력 
int BillingMagicForceOrbDamage[]  = {0,0,0}; // 박재원 - 빌링 매직 포스 추가 공격력 

int BoosterItem_UseTime[] = {3600,10800,86400}; // 박재원 - 부스터 아이템(1시간, 3시간, 1일) 유지시간 추가
int BoosterItem_DataPercent[] = {15,15,15}; // 박재원 - 부스터 아이템(생명력, 기력, 근력) 가중 퍼센트

// 장별 - 스킬 딜레이
int nSkillDelay_UseTime[] = {300,10800,86400};

/*----------------------------------------------------------------------------*
*						(인벤 토리)인벤  컬러 초기화 
*-----------------------------------------------------------------------------*/	


//일곱가지 크리스탈 보은  크리스탈 코드
DWORD CristalEventCODE[12]={(sinGP1|sin01),(sinGP1|sin02),(sinGP1|sin03),(sinGP1|sin04),(sinGP1|sin05)
					       ,(sinGP1|sin06),(sinGP1|sin07),(sinGP1|sin08),(sinGP1|sin09),
						    (sinGP1|sin11),(sinGP1|sin12),(sinGP1|sin13)};

//박재원 - 수박 모아오기(호박) //박재원 - 밤하늘의 소원 이벤트
DWORD WatermelonEventCODE = (sinSP1|sin42);
// 장별 - 발렌타인 이벤트
DWORD ValentineEventCODE = (sinSP1|sin35);

// 박재원 - 알파벳 조합 이벤트
DWORD PristonAlphabetEventCODE[7] = { (sinSP1|sin27), (sinSP1|sin28), (sinSP1|sin29),
									  (sinSP1|sin30), (sinSP1|sin31), (sinSP1|sin32), (sinSP1|sin33) };

// 장별 - 캔디데이즈
DWORD CandydaysEventCODE = (sinSP1|sin36);

// 장별 - 매지컬그린
DWORD MagicalGreenEventCODE[2] = { (sinSP1|sin37), (sinSP1|sin38) };

// 장별 - 카라의 눈물
DWORD TeatOfKaraEventCODE = (sinSP1|sin39);

// 장별 - 조사원을 찾아라 - 마력연구자
DWORD FindinvestigatorEventCODE[2] = { (sinGF1|sin07), (sinGF1|sin08) };

/*----------------------------------------------------------------------------*
*							클래스 초기, 종료 
*-----------------------------------------------------------------------------*/	
cINVENTORY::cINVENTORY()
{
	//인벤토리 영역 초기화 
	for(int i=0 ; i<INVENTORY_MAXITEM;i++)
		memset(&InvenItem[i],0,sizeof(sITEM));

}
cINVENTORY::~cINVENTORY()
{

}

/*----------------------------------------------------------------------------*
*							     초기화 
*-----------------------------------------------------------------------------*/
void cINVENTORY::Init() {}
/*----------------------------------------------------------------------------*
*						    Bmp파일을 로드한다 
*-----------------------------------------------------------------------------*/	
void cINVENTORY::Load() {}
void cINVENTORY::InvenItemLoad() {}
//아이템을 Release한다 
void cINVENTORY::InvenItemRelease() {}

/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cINVENTORY::Release() {}
/*----------------------------------------------------------------------------*
*							     그리기  
*-----------------------------------------------------------------------------*/	
int CoreEffectTime = 0;
int CoreEffectFlag = 0;
void cINVENTORY::Draw() {}
/*----------------------------------------------------------------------------*
*							     메인  
*-----------------------------------------------------------------------------*/	
int InvenMainCount = 0;   // 인벤토리 메인 카운트 
int SetDelayFlag = 0;     // 셋팅 딜레이 플렉 
bool bGiftBoxDelay = false;	// pluto 선물상자
DWORD dwUsePotionDelayTime = 0;
DWORD dwCheckHackCharStateTime = 0;
DWORD dwCheckHackCharStateFlag = 0;
DWORD dwCheckPickUpItemTime = 0;
int	  CharToItemFirstFlag = 0;
DWORD CharToItemFirstTime = 0;
DWORD dwUseCristalDelayTime  = 0;
int   sinUseCristalDelayFlag = 0;


DWORD dwCheckAutoMouseTime = 0;

int   CheckPosiFlag = 0;
#define   CHECKAUTOMOUSE_DISTANCE		(200*200)
#define   CHECKAUTOMOUSE_SETITEM_MAX	500
#define   CHECKAUTOMOUSE_TIME			(70*60*30)

//#define   CHECKAUTOMOUSE_SETITEM_MAX	100
//#define   CHECKAUTOMOUSE_TIME			(70*60*10)

int   CheckX = 0 , CheckZ = 0;
int   CheckResultX = 0 , CheckResultZ = 0;

DWORD dwPuzzleOkTime = 0;
int ReversEffectFlag = 0;
DWORD dwForceOrbTime = 0;
int   ForceOrbEffectIndex = 0;
void cINVENTORY::Main()
{		
	dwForceOrbTime++; 
	if(dwForceOrbTime > 8){   
		dwForceOrbTime = 0;
		ForceOrbEffectIndex++;
		if(ForceOrbEffectIndex > 7)
			ForceOrbEffectIndex = 0;
	}

	if(PuzzleOkFlag){    
		dwPuzzleOkTime++;
		if(dwPuzzleOkTime >2){ 
			dwPuzzleOkTime = 0;
			if(PuzzleOkY  > 82)ReversEffectFlag=1;
			if(ReversEffectFlag){
				PuzzleOkY --;
				if(PuzzleOkY <=0 )
					ReversEffectFlag = 0;
			}
			else PuzzleOkY ++;

		}
	}
	else{
		PuzzleOkY   = 0;
		dwPuzzleOkTime = 0;
	}
	//////////테스트
//	sInven[10].Rect.left = 499;   
//	sInven[10].Rect.top  = 575;


	int dist = 0;
	//////////오토마우스를 체크한다
	if(!CheckPosiFlag){
		CheckX = lpCurPlayer->pX;
		CheckZ = lpCurPlayer->pZ;
		CheckPosiFlag = 1;
	}

	dwCheckAutoMouseTime++;
	if(dwCheckAutoMouseTime >= CHECKAUTOMOUSE_TIME){
		if(sinOverWeightCnt > CHECKAUTOMOUSE_SETITEM_MAX){
			CheckResultX = ( lpCurPlayer->pX-CheckX )>>FLOATNS;
			CheckResultZ = ( lpCurPlayer->pZ-CheckZ )>>FLOATNS;
			dist   = CheckResultX*CheckResultX+CheckResultZ*CheckResultZ;
			if(CHECKAUTOMOUSE_DISTANCE >=dist){
				//신고 
				sinOverWeightCnt = 0;
				CheckPosiFlag = 0;
				dwCheckAutoMouseTime = 0;
			}
			else{
				//무게 초과나 공간초과 카운트는 넘었으나 캐릭터 이동거리가 클경우는 리셋한다
				sinOverWeightCnt = 0;
				CheckPosiFlag = 0;
				dwCheckAutoMouseTime = 0;
			}

		}
		else{
			sinOverWeightCnt = 0;
			CheckPosiFlag = 0;
			dwCheckAutoMouseTime = 0;
		}

	}

	if(sinUseCristalDelayFlag){
		dwUseCristalDelayTime++;
		if(dwUseCristalDelayTime >70){
			sinUseCristalDelayFlag = 0;
			dwUseCristalDelayTime  = 0;
		}
	}

	UnionCoreParticleProcess();
	//해킹방지를 위해서 30초마다 한번씩 체크한다
	dwCheckHackCharStateTime++; 
	if(dwCheckHackCharStateTime >= 30*70){
		SetItemToChar(TRUE);
		dwCheckHackCharStateTime = 0;

	}


	if(sinUsePotionDelayFlag){
		dwUsePotionDelayTime++;
		if(dwUsePotionDelayTime > 50){
			dwUsePotionDelayTime = 0;
			sinUsePotionDelayFlag = 0;

		}

	}

	if(SetDelayFlag){ //셋팅할때 Delay시간을 준다 
		InvenMainCount++;	
		if(InvenMainCount >=70){ //1초 딜레이를 준다 
			SetDelayFlag = 0;
			InvenMainCount = 0;
		}

	}


	// pluto 선물상자
	if( bGiftBoxDelay )
	{
		InvenMainCount++;
		if(InvenMainCount >= 140 )
		{
			bGiftBoxDelay = false;
			InvenMainCount = 0;
		}
	}
	InvenArrowPosi = 0; //리턴하기전에 값을 초기화해준다 
	int i = 0;
			


	int BackdsDrawOffsetArray = 0;
	if(MouseItem.Flag){
		SetInvenItemAreaCheck(&MouseItem); 
		sinItemPickUpDelayFlag =1;

	}
	if(sinItemPickUpDelayFlag){
		dwCheckPickUpItemTime++;
		if(dwCheckPickUpItemTime >  70){
			sinItemPickUpDelayFlag = 0;
			dwCheckPickUpItemTime = 0;
		}

	}
	else{
		sinItemPickUpDelayFlag = 0;
		dwCheckPickUpItemTime = 0;
	}

	if(!PotionCheckNumJumpFlag)
		CheckPotionNumForm();

	//인벤토리 이펙트 즐~
	if(OpenFlag){
		//ShowInvenEffectParticle(); //인벤토리 파티클을 셋팅한다  
		//MoveInvenParticle(); //인벤토르 파티클을 움직인다 
	}

	//캐릭터 능력보정을 위해 시작후 한번 보정해준다 
	if(!CharToItemFirstFlag){
		CharToItemFirstTime++;
		if(CharToItemFirstTime >= 70*3){
			SetItemToChar(0); 
			SetItemToChar(); 
			CharToItemFirstFlag = 1;
		}
	}

}
/*----------------------------------------------------------------------------*
*							     종료 
*-----------------------------------------------------------------------------*/	
void cINVENTORY::Close()
{
	Release();	
}
/*----------------------------------------------------------------------------*
*							    Down
*-----------------------------------------------------------------------------*/	
#include "..\\language.h"
int sinButtonFlag=0;
void cINVENTORY::LButtonDown(int x, int y) {}
/*----------------------------------------------------------------------------*

*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cINVENTORY::LButtonUp(int x, int y) {}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/

DWORD dwBoosterItemCode[] = {sin21,sin22,sin23,sin24,sin25,sin26,sin27,sin28,sin29,0}; // 박재원 - 부스터 아이템
DWORD dwCastlItemCODE[]={sin01,sin02,sin03,sin04,sin05,sin06,sin07,sin08,sin09,sin10,sin11,sin12,sin13,sin14,sin15,sin16,0};
DWORD dwNotUseVillageItemCODE[3]={(sinBI1|sin05),(sinBI1|sin06),(sinBI1|sin07)};
DWORD dwNotUseItemCODE[6][3]={{SCROLL_INVULNERABILITY,(sinBC1|sin01),1},{SCROLL_CRITICAL,(sinBC1|sin02),2},{SCROLL_EVASION,(sinBC1|sin03),3},
							  {SCROLL_P_INVULNERABILITY,(sinBI1|sin05),1},{SCROLL_P_CRITICAL,(sinBI1|sin06),2},{SCROLL_P_EVASION,(sinBI1|sin07),3}};
DWORD dwSkillDelayItemCode[] = {sin30,sin31,sin32, 0}; // 장별 - 스킬 딜레이
void cINVENTORY::RButtonDown(int x, int y) {}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cINVENTORY::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
void cINVENTORY::KeyDown() {}
/*----------------------------------------------------------------------------*
*							  클래스 함수 
*-----------------------------------------------------------------------------*/	
int cINVENTORY::LoadItemInfo() { return TRUE; }

//특별한 아이템을 퀘스트 인벤토리에 셋팅한다 (해상도 변경으로 인한 좌표변경을 보정해준다)
int cINVENTORY::ReSetQuestItemPosi()
{

	sITEM *pItem=0;
	for(int  i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else{
			if(!InvenItemTemp[i-100].Flag)continue;
			pItem = &InvenItemTemp[i-100];
		}
		if(pItem->x >1000 && pItem->x < 2000){
			pItem->x += 9000;
		}
	}
	return TRUE;
}

int cINVENTORY::SetSpecialItem(sITEM *pItem)
{
	if(pItem->sItemInfo.SpecialItemFlag[1] != ITEM_KIND_SPECIAL)return FALSE;

	int cntx,cnty;
	int cx,cy;

	RECT	rect;
	int		flag;

	cx = (44*6)-pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (44*3)-pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	int TempWidth = 0;
	if(SpecialItemShowFlag){
		TempWidth = 0;
	}
	else
		TempWidth = 10000;

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
	for( cntx=(StartX+TempWidth) ; cntx <= StartX+TempWidth+cx ; cntx+=44){
		for( cnty=StartY ; cnty <= StartY+cy ; cnty+=44){ 
			flag = 0;
			for(int i=84 ; i<INVENTORY_MAXITEM ; i++){
				if(InvenItem[i].Flag && InvenItem[i].sItemInfo.SpecialItemFlag[1] ==ITEM_KIND_SPECIAL){
					if(InvenItem[i].x < TempWidth){
						rect.left   = InvenItem[i].x+TempWidth;
						rect.right  = InvenItem[i].x+TempWidth+InvenItem[i].w;
					}
					else{
						rect.left   = InvenItem[i].x;
						rect.right  = InvenItem[i].x+InvenItem[i].w;
					}
					rect.top    = InvenItem[i].y;
					rect.bottom = InvenItem[i].y+InvenItem[i].h;

					if( cntx > rect.left-pItem->w && cntx< rect.right && 
						cnty > rect.top-pItem->h && cnty< rect.bottom )
						flag++;
				}
			}
			if ( flag==0 ) {
				pItem->SetX = cntx+((44-pItem->w)/2);
				pItem->SetY = cnty+((44-pItem->h)/2);
				pItem->ItemPosition = 0; 

				for(int j=84 ; j < INVENTORY_MAXITEM ; j++){
					if(InvenItem[j].Flag == 0){
						CheckInvenItemForm();
						memcpy(&InvenItem[j],pItem,sizeof(sITEM));
						InvenItem[j].x = pItem->SetX;
						InvenItem[j].y = pItem->SetY;
						ReFormInvenItem();
						SetItemToChar();
						CheckWeight();   //무게를 체크 

						//////////////// 요기서 서버로 메세지를 보낸다 
						SendCheckItemToServer(&InvenItem[j].sItemInfo);
						CheckDamage(); //최대데미지가 나올 무기로 계산하여 서버로 보내준다 
						ReFormPotionNum(); //포션카운트를 갱신해준다
						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

//오토 셋팅 
int cINVENTORY::AutoSetInvenItem(sITEM *pItem , int PostBox ) { return TRUE; }

//인벤토리에서의 빈곳을 체크한다 
int cINVENTORY::InvenEmptyAearCheck(sITEM *pItem)
{
	int cntx,cnty;
	int cx,cy;

	RECT	rect;
	int		flag;

	if(AutoSetPotionToBox(pItem))return TRUE;

	cx = (22*INVENTORY_BOX_ROW)-pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22*INVENTORY_BOX_COL)-pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
	for( cntx=StartX ; cntx <= StartX+cx ; cntx+=22){
		for( cnty=StartY ; cnty <= StartY+cy ; cnty+=22){ 
			flag = 0;
			for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
				if(InvenItem[i].Flag){
					rect.left   = InvenItem[i].x;
					rect.right  = InvenItem[i].x+InvenItem[i].w;
					rect.top    = InvenItem[i].y;
					rect.bottom = InvenItem[i].y+InvenItem[i].h;

					if( cntx > rect.left-pItem->w && cntx< rect.right && 
						cnty > rect.top-pItem->h && cnty< rect.bottom )
						flag++;
				}

			}
			if ( flag==0 ) {
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0; 
				LastSetInvenItem(pItem,1); //오토셋팅일경우에는 인자1을 넘긴다 
					return TRUE;
			}
		}
	}
return FALSE;
}

//인벤토리 아이템으로 복사한다 
int cINVENTORY::LastSetInvenItem(sITEM *pItem , int AutoSet)
{
	int PassFlag=0; //물약 때문에 처리한다 
	if(!AutoSet) //오토셋팅이 아닐경우에만 처리한다 
		if(!InvenNotOpenCheck(pItem))return FALSE; //인벤 로직수행여부를 체크한다 

	for(int j=0 ; j < INVENTORY_MAXITEM ; j++){
		if(InvenItem[j].Flag == 0){
			if(sinPosionItem){ //겹쳐진 물약 처리를 위해 코드 삽입 ㅎㅎ 
				if(sinPosionItem->sItemInfo.ItemHeader.Head == InvenItem[j].sItemInfo.ItemHeader.Head)
					continue;

			}
			CheckInvenItemForm();
			memcpy(&InvenItem[j],pItem,sizeof(sITEM));
			InvenItem[j].x = pItem->SetX;
			InvenItem[j].y = pItem->SetY;
			if(InvenItem[j].ItemPosition){ //셋팅될 포지션이 있으면 값을 셋팅한다 
				SetTwoHandItem(&InvenItem[j]);//투핸드 무기인지를 체크한다 
				sInven[InvenItem[j].ItemPosition-1].ItemIndex = j+1;//셋팅될때 인벤영역에도 아이템 인덱스를 셋팅한다
				sinSetCharItem(InvenItem[j].CODE , pItem->SetModelPosi, TRUE); //무기와 갑옷을 셋팅한다 
			
			}
			pItem->Flag = 0; //초기화 
			ReFormInvenItem();
			SetItemToChar();
			CheckWeight();   //무게를 체크 
			//////////////물약의 위치를 보정한다
			if(InvenItem[j].x < StartX || InvenItem[j].x > 800 || InvenItem[j].y < StartY || InvenItem[j].y > 600){
				AutoSetInvenItem(&InvenItem[j]);
			}

			//////////////// 요기서 서버로 메세지를 보낸다 
			SendCheckItemToServer(&InvenItem[j].sItemInfo);
			CheckDamage(); //최대데미지가 나올 무기로 계산하여 서버로 보내준다 
			ReFormPotionNum(); //포션카운트를 갱신해준다

			//퍼즐을 체크한다
			CheckPuzzle();
			return TRUE;
			
		}
	}
	ReFormInvenItem();
	return FALSE;
}
//////////////////////////////////
//         *포션 
//////////////////////////////////
int cINVENTORY::UsePotion(int x , int y)
{   //포션을 사용한다 {
	for(int i=0 ; i < INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if ( InvenItem[i].x < x  && InvenItem[i].x + InvenItem[i].w > x  && 
				InvenItem[i].y < y  && InvenItem[i].y + InvenItem[i].h > y ){
				if(InvenItem[i].ItemPosition == 11 || InvenItem[i].ItemPosition == 12 || InvenItem[i].ItemPosition == 13){
					if(InvenItem[i].Class == ITEM_CLASS_POTION){ //물약일경우는 사용한다 
						if(sinActionPotion()){ //포션을 사용하는 모션을 취한다  모션이 끝난후 물약을 사용한다 
							pUsePotion = &InvenItem[i]; //사용할 포션을 백업한다 
							sinUsePotionDelayFlag = 1;
						}
						
					}

				}
				else{
					if(!OpenFlag)break;
					if(InvenItem[i].Class == ITEM_CLASS_POTION){ //물약일경우는 사용한다 
						if(sinActionPotion()){ //포션을 사용하는 모션을 취한다  모션이 끝난후 물약을 사용한다 
							pUsePotion = &InvenItem[i]; //사용할 포션을 백업한다 
							sinUsePotionDelayFlag = 1;
						}
					}
				}
			}
		}
	}
	ReFormPotionNum();	//포션 갯수를 체크한다
	return FALSE;

}

int cINVENTORY::ReSettingPotion()
{ //포션 공간이 변했을 경우 포션을 셋팅한다 
	sITEM TempPotionItem;
	int i=0,j=0,cnt=0;
	
	memset(&TempPotionItem,0,sizeof(sITEM));
	for(i=10 ; i < 13; i++){ //포션 세팅박스만 검사한다 
		if(sInven[i].ItemIndex){
			if(InvenItem[sInven[i].ItemIndex-1].Flag){
				memcpy(&TempPotionItem,&InvenItem[sInven[i].ItemIndex-1],sizeof(sITEM));
				cnt = TempPotionItem.sItemInfo.PotionCount;
				if(sinChar->Potion_Space < InvenItem[sInven[i].ItemIndex-1].sItemInfo.PotionCount){ //포션 할당 공간보다 포션이 더있으면 
					TempPotionItem.sItemInfo.PotionCount = InvenItem[sInven[i].ItemIndex-1].sItemInfo.PotionCount -sinChar->Potion_Space;
					InvenItem[sInven[i].ItemIndex-1].sItemInfo.PotionCount -= TempPotionItem.sItemInfo.PotionCount;
					if(!InvenEmptyAearCheck(&TempPotionItem)){
						NoSettingThrowItem(&TempPotionItem);// 셋팅이 안될때는 바닥에 아이템을 버린다 
					}

				}
			}
		
		}

	}
	ReFormPotionNum();	//포션 갯수를 체크한다

	return TRUE;
}
int BackUpX,BackUpY,BackUpPosi=0;
int cINVENTORY::MouseSetPotion(sITEM *pItem) { return TRUE; }

int cINVENTORY::AutoSetPotionToBox(sITEM *pItem) { return TRUE; }
int cINVENTORY::AutoSetPotion(sITEM *pItem) { return TRUE; }
//////////////////////////////////
//         *투핸드
//////////////////////////////////
int cINVENTORY::ClearTwoHandPosiAndItem(sITEM *pItem)
{
	if(pItem->Class == ITEM_CLASS_WEAPON_TWO){
		if(sInven[0].ItemIndex){  //오른손 
			InvenItem[sInven[0].ItemIndex-1].Flag = 0;
			sInven[0].ItemIndex = 0;
			if(InvenItem[sInven[0].ItemIndex-1].ItemPosition){
				if(InvenItem[sInven[0].ItemIndex-1].SetModelPosi){				
					sinSetCharItem(InvenItem[sInven[0].ItemIndex-1].CODE , InvenItem[sInven[0].ItemIndex-1].SetModelPosi, FALSE); //무기와 갑옷을 셋팅한다 
				}

			}

		}
		if(sInven[1].ItemIndex){
			InvenItem[sInven[1].ItemIndex-1].Flag = 0;
			sInven[1].ItemIndex = 0;
			if(InvenItem[sInven[1].ItemIndex-1].ItemPosition){
				if(InvenItem[sInven[1].ItemIndex-1].SetModelPosi){				
					sinSetCharItem(InvenItem[sInven[1].ItemIndex-1].CODE , InvenItem[sInven[1].ItemIndex-1].SetModelPosi, FALSE); //무기와 갑옷을 셋팅한다 
				}
			}
		}
	}
	return TRUE;
}
int cINVENTORY::SetTwoHandItem(sITEM *pItem)
{ //셋팅 

	sITEM TempTwoHandItem;

	memset(&TempTwoHandItem,0,sizeof(sITEM)); //초기화 해준다 

	if(pItem->Class == ITEM_CLASS_WEAPON_TWO){ //투핸드 무기를 체크한다 
		if(pItem->ItemPosition == 2 ){ //왼손이면 
			pItem->ItemPosition = 1;   //아이템의 영역을 오른손으로 바꾼다  좌표를 셋팅한다 
			pItem->x = sInven[0].Rect.left + (((sInven[0].Rect.right - sInven[0].Rect.left)-pItem->w)/2);
			pItem->y = sInven[0].Rect.top  + (((sInven[0].Rect.bottom - sInven[0].Rect.top)-pItem->h)/2);	


		}
		TempTwoHandItem.x = sInven[1].Rect.left + (((sInven[1].Rect.right - sInven[1].Rect.left)-pItem->w)/2);
		TempTwoHandItem.y = sInven[1].Rect.top + (((sInven[1].Rect.bottom - sInven[1].Rect.top)-pItem->h)/2);
		TempTwoHandItem.w = pItem->w; 
		TempTwoHandItem.h = pItem->h;
		TempTwoHandItem.ItemPosition = 2; //아이템의 위치는 왼손 
		TempTwoHandItem.Class = pItem->Class; 
		TempTwoHandItem.sItemInfo.CODE = pItem->CODE; //코드를 넣어준다 (아이템Load시 아이템 구분인자로 적합)
		TempTwoHandItem.sItemInfo.Price = pItem->sItemInfo.Price; //가격을 넣어준다 
		memcpy(&TempTwoHandItem.lpItem,&pItem->lpItem,sizeof(void*));
		TempTwoHandItem.Flag = 1;
		for(int j=0 ; j < INVENTORY_MAXITEM ; j++){
			if(InvenItem[j].Flag == 0){
				memcpy(&InvenItem[j],&TempTwoHandItem,sizeof(sITEM));
				sInven[1].ItemIndex = j+1; //셋팅될때 인벤영역에도 아이템 인덱스를 셋팅한다
				break;
			}
		}

	}

	return TRUE;
}

int cINVENTORY::PickUpTwoHandItem(sITEM *pItem)
{//선택 
	sITEM TempPickItem;
	memset(&TempPickItem,0,sizeof(sITEM));

	if(pItem->ItemPosition){ //무기 박스에서 집었을 경우에만 
		if(sInven[0].ItemIndex && sInven[1].ItemIndex){
			memcpy(&TempPickItem,&InvenItem[sInven[0].ItemIndex-1],sizeof(sITEM)); //오른손의 아이템을 복사한다 
			InvenItem[sInven[0].ItemIndex-1].Flag = 0; //오른손 아이템을 지워준다 
			InvenItem[sInven[1].ItemIndex-1].Flag = 0; //왼손 아이템을 지워준다 
			sInven[0].ItemIndex = 0; //무기셋팅 박스의 아이템 포지션도 지워준다 
			sInven[1].ItemIndex = 0; //무기셋팅 박스의 아이템 포지션도 지워준다 
			memcpy(pItem,&TempPickItem,sizeof(sITEM));
			ReFormInvenItem();
			ReformCharForm();//재인증 
			ReFormPotionNum();
			return TRUE;
		}
	}
	return FALSE;
}

//투핸드 아이템으로 겹치는 경우를 체크한다  (무기셋팅 영역일때만 호출된다 )
int cINVENTORY::OverlapTwoHandItem(sITEM *pItem)
{
	AutoSetItemIndex  = 0; //자동으로 셋팅될 아이템의 인덱스 
	if(pItem->Class == ITEM_CLASS_WEAPON_TWO){      //투핸드 아이템일 경우 
		if(sInven[1].ItemIndex){
			if(InvenItem[sInven[1].ItemIndex-1].Class !=ITEM_CLASS_WEAPON_TWO){ //현재 무기포지션에 있는 아이템이 투핸드 무기가 아닐경우에
				if(pItem->ItemPosition == 1 ){ //오른손 
					if(sInven[1].ItemIndex)
						AutoSetItemIndex = sInven[1].ItemIndex; //왼손 아이템의 인덱스를 저장한다 
				}
				if(pItem->ItemPosition == 2 ){ //왼손 
					if(sInven[0].ItemIndex)
						AutoSetItemIndex = sInven[0].ItemIndex; //오른손 아이템의 인덱스를 저장한다 
				}
			}
		}
	}
	return TRUE;

}

//투핸드 아이템으로 겹치는 경우를 체크한다  (무기셋팅 영역외의 영역 오른손에 아이템을 체크했을때 방패가 인덱스가온다)
int cINVENTORY::OverlapTwoHandSwitch(sITEM *pItem)
{
	if(pItem->Class == ITEM_CLASS_WEAPON_TWO){      //투핸드 아이템일 경우 
		if(pItem->ItemPosition == 1 ) //오른손 
			if(sInven[1].ItemIndex)
				CrashItemIndex[0] = sInven[1].ItemIndex;
		if(pItem->ItemPosition == 2 ) //왼손 
			if(sInven[0].ItemIndex)
				CrashItemIndex[0] = sInven[0].ItemIndex;

	}
	return TRUE;
}


//인벤토리 체크 여부 (물약박스를 체크하지 위해 )
int cINVENTORY::InvenNotOpenCheck(sITEM *pItem )
{
	if(!OpenFlag) //인벤토리가 닫혀있고  물약 포지션이 아니면 리턴한다 
		if(pItem->ItemPosition != 11 && pItem->ItemPosition != 12 && pItem->ItemPosition != 13)
			return FALSE; 
	return TRUE;
}

int cINVENTORY::PickUpInvenItem(int x , int y , int PickUpFlag) { return TRUE; }


//마우스에 아이템이 있을때 아이템이 셋팅될 영역을 체크한다 
int cINVENTORY::SetInvenItemAreaCheck(sITEM *pItem)
{

	int i,j;
	ColorIndex= 0; //초기화 
	CrashItemIndex[0] = 0; //충돌된 아이템 Index 초기화 
	CrashItemIndex[1] = 0;
	AutoSetItemIndex  = 0; //자동으로 셋팅될 아이템의 인덱스 

	///////////무기 영역 &포션 영역 
	if(!cInvenTory.OpenFlag)return FALSE; //인벤토리가 열려있지 않을시에는 아이템을 셋팅할수 없다
	for( i=0 ; i < INVENTORY_MAX_POS ; i++){ 
		if(CheckInvenPos(pItem->x+11 , pItem->y+11 ,pItem->x +pItem->w -11 ,pItem->y +pItem->h -11 , sInven[i].Rect)){
			ColorRect.left   = sInven[i].Rect.left;     //컬럭 박스의 RECT
			ColorRect.top    = sInven[i].Rect.top;
			ColorRect.right  = sInven[i].Rect.right  - sInven[i].Rect.left;
			ColorRect.bottom = sInven[i].Rect.bottom - sInven[i].Rect.top;
			pItem->SetX = sInven[i].Rect.left + ((ColorRect.right-pItem->w)/2);
			pItem->SetY = ColorRect.top  + ((ColorRect.bottom - pItem->h)/2);
			pItem->ItemPosition = i+1;

			if(!(sInven[i].Position & pItem->Class)){ //클래스가 맞지 않는다 
				ColorIndex = NOT_SETTING_COLOR;
				return FALSE;
			}
			if(pItem->sItemInfo.NotUseFlag){ //클래스가 맞지 않거나 셋팅이 될수없으면  
				ColorIndex = NOT_USE_COLOR;
				return FALSE;

			}
			/*
			if(CrashItemIndex[0] = CrashInvenItem(ColorRect)){  //겹치는 아이템이 있나 체크 
				if(InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON &&
					InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemAgingNum[0] < 4){
					ColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
			}
			*/

			if(CrashItemIndex[0] = CrashInvenItem(ColorRect)){  //겹치는 아이템이 있나 체크 
				OverlapTwoHandItem(pItem); //투핸드 무기로 겹치게 될 아이템을 체크한다
				ColorIndex = OVERLAP_BOX_COLOR;
				return TRUE;
			}
			OverlapTwoHandSwitch(pItem); //투핸드 아이템의 반대쪽 겹치기 체크 
			ColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역 
			return TRUE;
		}

	}
	
	///////////박스 영역 
	for(i=pItem->x+11 ; i<pItem->x+pItem->w ; i+=22){
		for( j=pItem->y+11 ; j<pItem->y+pItem->h ; j+=22){
			if(StartX <= i && EndX > i && StartY <=j && EndY > j){ //인벤토리 박스에 들어갔는지를 체크 
				ColorRect.left =  StartX +( ( ( i - StartX) /22)*22) ; 
				ColorRect.top  =  StartY +( ( ( j - StartY) /22)*22) ; 
				ColorRect.right  = pItem->w;
				ColorRect.bottom = pItem->h;
				if(EndX < (ColorRect.left + ColorRect.right)-11 || EndY < (ColorRect.top + ColorRect.bottom)-11){
					InitColorRect(); //컬러 박스 RECT 초기화 (컬러 박스 RECT중 하나라두 0 이면 셋팅할수 없다 
					return FALSE;
				}
				pItem->SetX = ColorRect.left;
				pItem->SetY = ColorRect.top;
				pItem->ItemPosition = 0; //배열의 인덱스를 저장한다 
				ColorIndex = SET_ITEM_CHECK_COLOR; //셋팅할영역


				/*
				if(CrashItemIndex[0] = CrashInvenItem(ColorRect)){  //겹치는 아이템이 있나 체크 
					if(InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON &&
						InvenItem[CrashItemIndex[0]-1].sItemInfo.ItemAgingNum[0] < 4){
						ColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
					
				}
				*/
				if(CrashItemIndex[0] = CrashInvenItem(ColorRect)){  //겹치는 아이템이 있나 체크
					/*
					if(!CheckSpecialItem(&InvenItem[CrashItemIndex[0]-1])){ //집을 수없는 특별한 아이템을 체크
						ColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
					*/

					if(CrashItemIndex[1] = CrashInvenItem(ColorRect,CrashItemIndex[0])){ //두개 이상 겹치나 체크 
						ColorIndex = NOT_SETTING_COLOR;
						CrashItemIndex[1] = 0; //초기화 
						return FALSE;

					}
					ColorIndex = OVERLAP_ITEM_COLOR;
					return TRUE;
				}
				return TRUE;
			}
				
		}
	
	}

	return TRUE;	
}

//컬러 박스 RECT 초기화 
int cINVENTORY::InitColorRect()
{
	ColorRect.left   = 0;   
	ColorRect.top    = 0;
	ColorRect.right  = 0;
	ColorRect.bottom = 0;
	return TRUE;

}

//아이템 끼리의 충돌영역을 체크한다
int cINVENTORY::CrashInvenItem(RECT &desRect , int PassItemIndex)
{

	RECT rect;

	for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(PassItemIndex != 0)
				if(PassItemIndex == i+1)continue;
			rect.left   = InvenItem[i].x;
			rect.right  = InvenItem[i].x+InvenItem[i].w;
			rect.top    = InvenItem[i].y;
			rect.bottom = InvenItem[i].y+InvenItem[i].h;

			if( desRect.left > rect.left - desRect.right  && desRect.left < rect.right &&
				desRect.top > rect.top - desRect.bottom && desRect.top< rect.bottom ){
					return i+1; //충돌 체크된 아이템 인덱스를 하나더해서 리턴한다 
			}
		}
	}

	return FALSE;

}
//아이템이 놓을 영역 충돌 체크 
int cINVENTORY::CheckInvenPos(int x , int y , int lx ,int ly , RECT &rect)
{
	if(x <= rect.left){
		if(rect.left >= lx)return FALSE;
	}
	else{ 
		if(x >=rect.right)return FALSE;
	}

	if(y <= rect.top ){
		if(rect.top >= ly)return FALSE;
	}
	else {
		if(y >= rect.bottom)return FALSE;
	}

	return TRUE;

}

int cINVENTORY::ChangeInvenItem(sITEM *pItem) { return TRUE; }

//셋팅이 안될때에는 바닥에 아이템을 버린다 
int cINVENTORY::NoSettingThrowItem(sITEM *pItem , int Flag)
{
	//패치를 위해 잠시 막아둔다 
	if(!pItem)return FALSE;  
	if(!pItem->Flag)return FALSE;
	if(!Flag){ //겹쳐지는 아이템은 바닦에 버린다 (겹쳐지지않은 상태에서는 상점 및 트레이드 창등이 떠있을경우에는 버릴수없다) // pluto 제련
		if(cTrade.OpenFlag || cWareHouse.OpenFlag || cCraftItem.OpenFlag || cAging.OpenFlag || SmeltingItem.OpenFlag || ManufactureItem.m_OpenFlag || cMixtureReset.OpenFlag)return FALSE; //트레이드나 창고가 떠있으면 아이템을 버리지 않는다 // 석지용 - 믹스쳐 리셋 창 추가
	}
	if(!pItem->sItemInfo.ItemHeader.Head || pItem->sItemInfo.ItemHeader.Head < 0)return FALSE;
	if(pItem->sItemInfo.CODE == ( sinGF1|sin01))return FALSE;
	//if(pItem->sItemInfo.CODE == ( sinGF1|sin02))return FALSE;

	int kk =0;
	//등록된 아이템은 버릴수없다아이템을 버릴수없다 
	for(kk = 0 ; kk < NotDrow_Item_CODECnt ; kk++){
		if(NotDrow_Item_CODE[kk] == pItem->sItemInfo.CODE)return FALSE;

	}
	for(kk = 0 ; kk < NotDrow_Item_MASKCnt ; kk++){
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotDrow_Item_MASK[kk])return FALSE;

	}
	for(kk = 0 ; kk < NotDrow_Item_KINDCnt ; kk++){
		if(NotDrow_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode)return FALSE;

	}

	if(sinThrowItemToFeild(pItem)){
		pItem->Flag = 0;
	}
	else{
		pItem->Flag = 0; //버리지 못해두 셋팅이 안되므로 지워버린다 
		
	}

	return TRUE;
}

//아이템을 버린다 
int cINVENTORY::ThrowInvenItemToField(sITEM *pItem) { return TRUE; }

//자신이 찰수있는 아이템을 착용한다 
int cINVENTORY::CharOnlySetItem(sITEM *pItem)
{
	//파티복 코스튬 착용제
	//여자
	if(sinChar->JOB_CODE == JOBCODE_PRIESTESS || sinChar->JOB_CODE == JOBCODE_ATALANTA || sinChar->JOB_CODE == JOBCODE_ARCHER){	
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA1 || (pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA2 ){
			if( (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin31 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin32 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin35 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin36 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin39 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin40 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin43 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin44 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin51 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin54 )
			{
				pItem->sItemInfo.NotUseFlag = 1; 			
			}
		}		
	}
	else{ // 남자
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA1 || (pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA2 ){
			if( (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin33 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin34 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin37 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin38 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin41 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin42 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin45 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin46 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin45 || (pItem->sItemInfo.CODE & sinITEM_MASK3) == sin46 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin52 ||
				(pItem->sItemInfo.CODE & sinITEM_MASK3) == sin55 )
			{

				pItem->sItemInfo.NotUseFlag = 1; 			
			}
		}
	}

	///////////// 법사
	if(sinChar->JOB_CODE == 7 || sinChar->JOB_CODE == 8){
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA1){
			pItem->sItemInfo.NotUseFlag = 1; //일반 갑옷은 법사가 착용할수엄따

		}
	}
	else{ //전사 
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA2 ||
			(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinOM1 ){
				pItem->sItemInfo.NotUseFlag = 1; //로브 와 오브는 는 전사가 착용할수엄따

			}
	}	

	return TRUE;
}

//현재 상태를 체크한다 (레벨 ,힘 ,민첩 ....) 
int cINVENTORY::CheckRequireItem()
{
	int NotUseItemFlag = 0;
	int i;

	for( i=0 ; i < INVENTORY_MAXITEM ; i++){   
		if(InvenItem[i].Flag){
			if(InvenItem[i].sItemInfo.Level     > sinChar->Level)NotUseItemFlag = 1;
			if(InvenItem[i].sItemInfo.Dexterity > sinChar->Dexterity)NotUseItemFlag = 1;
			if(InvenItem[i].sItemInfo.Strength  > sinChar->Strength)NotUseItemFlag = 1;
			if(InvenItem[i].sItemInfo.Talent    > sinChar->Talent)NotUseItemFlag = 1;
			if(InvenItem[i].sItemInfo.Spirit    > sinChar->Spirit)NotUseItemFlag = 1;
			if(InvenItem[i].sItemInfo.Health    > sinChar->Health)NotUseItemFlag = 1;
			if(DeleteEventItem_TimeOut( &InvenItem[i].sItemInfo)==TRUE) NotUseItemFlag = 1;  //	박재원 : 기간제 아이템 만료되었을때(클랜치프링 등등)
			if(NotUseItemFlag){
				InvenItem[i].sItemInfo.NotUseFlag = 1;
				NotUseItemFlag = 0;
			}
			else
				InvenItem[i].sItemInfo.NotUseFlag = 0;
			CharOnlySetItem(&InvenItem[i]); //캐릭터별 아이템을 체크한다
			if((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinFO1){
				int j = 0;
				for(j = 0; j < 14 ; j++){ // 박재원 - 매직 포스 추가
					if((InvenItem[i].sItemInfo.CODE&sinITEM_MASK3)== SheltomCode2[j] || (InvenItem[i].sItemInfo.CODE&sinITEM_MASK3)== MagicSheltomCode[j]){
						break;
					}
				}
				if( (InvenItem[i].sItemInfo.CODE&sinITEM_MASK3) < sin21 ) // 박재원 - 일반 포스
				{
					if(sinChar->Level < ForceOrbUseLevel[j][0] ||
						sinChar->Level > ForceOrbUseLevel[j][1]){
							InvenItem[i].sItemInfo.NotUseFlag = 1;
							NotUseItemFlag = 0;

						}
					else{
						InvenItem[i].sItemInfo.NotUseFlag = 0;

					}
				}
				else if( (InvenItem[i].sItemInfo.CODE&sinITEM_MASK3) < sin35) // 박재원 - 매직 포스
				{
					if(sinChar->Level < MagicForceOrbUseLevel[j][0] ||
						sinChar->Level > MagicForceOrbUseLevel[j][1]){
							InvenItem[i].sItemInfo.NotUseFlag = 1;
							NotUseItemFlag = 0;

						}
					else{
						InvenItem[i].sItemInfo.NotUseFlag = 0;

					}
				}
			}

		}	

	}
	return TRUE;
}

//셋팅할수 있는지를 체크 
int cINVENTORY::CheckRequireItemToSet(sITEM *pItem)
{
	int NotUseItemFlag = 0;

	if(pItem->sItemInfo.Level     > sinChar->Level )NotUseItemFlag = 1;
	if(pItem->sItemInfo.Dexterity > sinChar->Dexterity)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Strength  > sinChar->Strength)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Talent    > sinChar->Talent)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Spirit    > sinChar->Spirit)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Health    > sinChar->Health)NotUseItemFlag = 1;
	if(NotUseItemFlag){
		pItem->sItemInfo.NotUseFlag = 1;
		NotUseItemFlag = 0;
	}
	else{
		pItem->sItemInfo.NotUseFlag = 0;


	}
	CharOnlySetItem(pItem); //캐릭터별 아이템을 체크한다
	return TRUE;

}

//인벤토리 텍스트 
int cINVENTORY::DrawInvenText() { return TRUE; }

//현재의 직업을 구해온다 
int cINVENTORY::SearchJobAndSetting()
{
	int cnt;

	JobName[0] = 0;

	cnt = 0;
	//난중에 꼭빼야함!
	/*
	if(smConfig.DebugMode){ 
		if(sinChar->ChangeJob > 1)
			sinChar->ChangeJob = 1;
	}
	*/
	while(1) {
		if ( JobDataBase[cnt].JobCode==0 ) break;
		if ( JobDataBase[cnt].JobCode==sinChar->JOB_CODE ) {
			sinJobList = GetJobDataCode(sinChar->JOB_CODE , sinChar->ChangeJob);
			lstrcpy( JobName , sinJobList->szName2 );
			sinChar->JobBitMask = JobDataBase[cnt].JobBitCode;
			sinChar->LifeFunction = JobDataBase[cnt].LifeFunction; //생명력 함수 
			sinChar->ManaFunction = JobDataBase[cnt].ManaFunction; //기력 함수 
			sinChar->StaminaFunction = JobDataBase[cnt].StaminaFunction; //근력함수 
			sinDamageFunction[0] = JobDataBase[cnt].DamageFunction[0];  //근접 공격 
			sinDamageFunction[1] = JobDataBase[cnt].DamageFunction[1];  //원거리 공격 
			sinDamageFunction[2] = JobDataBase[cnt].DamageFunction[2];  //마법 공격

           	break;
		}
		cnt++;
	}
	return TRUE;

}

//캐릭터 능력치 설정 
int cINVENTORY::SetItemToChar(int CheckHackFlag) { return TRUE; }

//캐릭터 정보를 읽어온다 
void cINVENTORY::LoadCharState()
{

  
}

//아이템을 바꾼다 
int cINVENTORY::ChangeABItem(int Posi) { return TRUE; }
//아이템의 가격정보를 얻어온다 
sITEMPRICE cINVENTORY::GetInvenItemPrice(sITEM *pItem)
{
	
	sITEMPRICE sinItemPrice;

	if(pItem->Class == ITEM_CLASS_WEAPON_TWO){
		if(pItem->ItemPosition){
			pItem = &InvenItem[sInven[0].ItemIndex-1];

		}
	}

	float Dur[2];		//내구력 
	float Price;		//현재 가격 

	Dur[0] = pItem->sItemInfo.Durability[0];
	Dur[1] = pItem->sItemInfo.Durability[1];
	Price  = (float)pItem->sItemInfo.Price;
	if(Dur[0] == 0)Dur[0] = 1; //값이 0이되는것을 보정한다 
	if(Dur[1] == 0)Dur[1] = 1;

	sinItemPrice.PureSellPrice = (int)((Price*(Dur[0]/Dur[1]))+(Price-(Price*(Dur[0]/Dur[1])))*0.25);
	
	sinItemPrice.SellPrice =  (int)sinItemPrice.PureSellPrice/5;  

	sinItemPrice.RepairCost = (pItem->sItemInfo.Price -sinItemPrice.PureSellPrice)/8; 
/*
	//수표 Kyle

	if( pItem->sItemInfo.CODE == (sinBI1|sin35)	||
		pItem->sItemInfo.CODE == (sinBI1|sin36)	||
		pItem->sItemInfo.CODE == (sinBI1|sin37)	||
		pItem->sItemInfo.CODE == (sinBI1|sin38)
		)
	{
		sinItemPrice.PureSellPrice	= 0;
		sinItemPrice.SellPrice		= (int)Price;
		sinItemPrice.RepairCost		= 0; 
	}
	else
	{
		sinItemPrice.PureSellPrice = (int)((Price*(Dur[0]/Dur[1]))+(Price-(Price*(Dur[0]/Dur[1])))*0.25);
		sinItemPrice.SellPrice =  (int)sinItemPrice.PureSellPrice/5;  
		sinItemPrice.RepairCost = (pItem->sItemInfo.Price -sinItemPrice.PureSellPrice)/8; 
	}
*/
	return sinItemPrice; //구조체를 리턴한다 

}


//아이템을 수리한다 
int cINVENTORY::RepairInvenItem(sITEM *pItem , int RepairCost) 
{
	if(pItem->Class == ITEM_CLASS_WEAPON_TWO){
		if(pItem->ItemPosition){
			pItem = &InvenItem[sInven[0].ItemIndex-1];

		}
	}

	CheckCharForm();//인증 
	//ReformCharForm();//재인증
	if(!pItem->sItemInfo.Durability[1])return FALSE;
	if(sinChar->Money - RepairCost > 0){
		if(pItem->sItemInfo.Durability[0] < pItem->sItemInfo.Durability[1]){
			//sinChar->Money -= RepairCost;
			sinMinusMoney(RepairCost);
			pItem->sItemInfo.Durability[0] = pItem->sItemInfo.Durability[1];
			ReformCharForm();//재인증 
			SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
			return TRUE;
		}
	}
	return FALSE;
	
}

//무게를 체크한다 
void cINVENTORY::CheckWeight()
{

	CheckCharForm();//인증
	
	sinChar->Weight[0] = 0; //무게 초기화 후 다시 셋팅 
	for(int i=0; i <INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(InvenItem[i].Class == ITEM_CLASS_POTION){
				if( InvenItem[i].sItemInfo.PotionCount < 0 )continue;
				sinChar->Weight[0] += InvenItem[i].sItemInfo.PotionCount;
				continue;

			}
			if(InvenItem[i].sItemInfo.Weight < 0)continue;
			sinChar->Weight[0] += InvenItem[i].sItemInfo.Weight;
		}
	}
	for(int j=0; j <INVENTORY_MAXITEM ; j++){
		if(InvenItemTemp[j].Flag){
			if(InvenItemTemp[j].Class == ITEM_CLASS_POTION){
				if( InvenItemTemp[j].sItemInfo.PotionCount < 0 )continue;
				sinChar->Weight[0] += InvenItemTemp[j].sItemInfo.PotionCount;
				continue;

			}
			if(InvenItemTemp[j].sItemInfo.Weight < 0)continue;
			sinChar->Weight[0] += InvenItemTemp[j].sItemInfo.Weight;
		}
	}

	ReformCharForm();//재인증 

}

// 셋팅할수있는지를 체크한다 
int cINVENTORY::CheckSetOk(sITEM *pItem , int AutoFlag) { return TRUE; }


//복사된 아이템을 없앤다  
int cINVENTORY::CopyItemClear(sITEM *pItem)
{ //마우스에 들려있는 아이템과 같은 아이템이 있는지를 검사한다

	int CopyItemCnt = 0;
	for(int i = 0 ; i < INVENTORY_MAXITEM ; i++){ 
		if(InvenItem[i].Flag){		//앞면 인벤토리 
			if(CompareItems(&pItem->sItemInfo ,  &InvenItem[i].sItemInfo)){
				//복사된 아이템을 지워준다 
				InvenItem[i].Flag = 0;
				if(InvenItem[i].ItemPosition){
					sInven[InvenItem[i].ItemPosition-1].ItemIndex = 0;
					sinSetCharItem(InvenItem[i].CODE , InvenItem[i].SetModelPosi, FALSE); //무기와 갑옷을 셋팅한다 
					ClearTwoHandPosiAndItem(&InvenItem[i]);//투핸드 무기이면 투핸드 무기를 지워준다 

				}
				CopyItemCnt++;
				

			}

		}
		if(InvenItemTemp[i].Flag){	//뒷면 인벤토리 
			if(CompareItems(&pItem->sItemInfo ,  &InvenItemTemp[i].sItemInfo)){//복사된 아이템을 지워준다 
				InvenItemTemp[i].Flag = 0;
				if(InvenItemTemp[i].Class == ITEM_CLASS_WEAPON_TWO){
					if(InvenItemTemp[i].ItemPosition == 1 ||InvenItemTemp[i].ItemPosition == 2){
						for(int k = 0 ; k < INVENTORY_MAXITEM ; k++){ 
							if(InvenItemTemp[k].Flag){
								if(InvenItemTemp[k].ItemPosition == 1 ||InvenItemTemp[k].ItemPosition == 2){
									if(InvenItemTemp[i].sItemInfo.CODE == InvenItemTemp[k].sItemInfo.CODE){
										InvenItemTemp[k].Flag = 0;		
									}
								}
							}
						}
					}
				}
				CopyItemCnt++;
			}
		}
	}
	if(CopyItemCnt){
		SendCopyItemUser(CopyItemCnt); //서버에 카피된 아이템의 갯수를 보낸다 
		CopyItemCnt = 0;
	}
	return TRUE;	

}

//임시로 아이템 이미지를 체크하기위해서 쓰인다 
int cINVENTORY::ImsiCheckItemImageSize() { return TRUE; }


//잘못된 좌표를 보정해준다 
int cINVENTORY::CheckOverlapItem(sITEM *pItem , int PassItemIndex)
{
	int ReSetXYFlag =0 ;
	if(!pItem)return FALSE;
	if(pItem->ItemPosition){
		if(pItem->ItemPosition < 11){ //물약 포지션이 아니면 
			sInven[pItem->ItemPosition-1].ItemIndex = 0; //포지션 초기화 
			pItem->ItemPosition = 0;
			ReSetXYFlag = 1;
		}
		else 
			ReSetXYFlag = 0;
	}

	RECT Rect;
	int  TempPosiFlag = 0;
	Rect.left   = pItem->x; 
	Rect.top    = pItem->y;
	Rect.right  = pItem->w;
	Rect.bottom = pItem->h;

	int cntx,cnty;
	int cx,cy;

	RECT	rect;
	int		flag;

	cx = (22*INVENTORY_BOX_ROW)-pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22*INVENTORY_BOX_COL)-pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 

	if(CrashInvenItem(Rect,PassItemIndex)||ReSetXYFlag){
		for( cntx=StartX ; cntx <= StartX+cx ; cntx+=22){
			for( cnty=StartY ; cnty <= StartY+cy ; cnty+=22){ 
				flag = 0;
				for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
					if(InvenItem[i].Flag){
						rect.left   = InvenItem[i].x;
						rect.right  = InvenItem[i].x+InvenItem[i].w;
						rect.top    = InvenItem[i].y;
						rect.bottom = InvenItem[i].y+InvenItem[i].h;
	
						if( cntx > rect.left-pItem->w && cntx< rect.right && 
							cnty > rect.top-pItem->h && cnty< rect.bottom )
							flag++;
					}
				}
				if ( flag==0 ) {
					pItem->x = cntx;
					pItem->y = cnty;
					sInven[pItem->ItemPosition-1].ItemIndex =0;
					pItem->ItemPosition = 0; 
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

//임시로 잘못된 좌표를 보정해준다 
int cINVENTORY::ReSetPotionXY(sITEM *pItem,int kind)
{

	RECT Rect;
	Rect.left   = pItem->x; 
	Rect.top    = pItem->y;
	Rect.right  = pItem->w;
	Rect.bottom = pItem->h;

	int cntx,cnty;
	int cx,cy;

	RECT	rect;
	int		flag;

	cx = (22*INVENTORY_BOX_ROW)-pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22*INVENTORY_BOX_COL)-pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 

	if(kind == 1){
		for( cntx=StartX ; cntx <= StartX+cx ; cntx+=22){
			for( cnty=StartY ; cnty <= StartY+cy ; cnty+=22){ 
				flag = 0;
				for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
					if(InvenItemTemp[i].Flag){
						rect.left   = InvenItemTemp[i].x;
						rect.right  = InvenItemTemp[i].x+InvenItemTemp[i].w;
						rect.top    = InvenItemTemp[i].y;
						rect.bottom = InvenItemTemp[i].y+InvenItemTemp[i].h;
		
						if( cntx > rect.left-pItem->w && cntx< rect.right && 
							cnty > rect.top-pItem->h && cnty< rect.bottom )
							flag++;
					}
				}
				if ( flag==0 ) {
					pItem->x = cntx;
					pItem->y = cnty;
					//sInven[pItem->ItemPosition-2].ItemIndex =0; //요기는 템프아이템이라서 걍 냅둔다 
					pItem->ItemPosition = 0; 
					return TRUE;
				}

			}
		}
	}
	else{
		for( cntx=StartX ; cntx <= StartX+cx ; cntx+=22){
			for( cnty=StartY ; cnty <= StartY+cy ; cnty+=22){ 
				flag = 0;
				for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
					if(InvenItem[i].Flag){
						rect.left   = InvenItem[i].x;
						rect.right  = InvenItem[i].x+InvenItem[i].w;
						rect.top    = InvenItem[i].y;
						rect.bottom = InvenItem[i].y+InvenItem[i].h;
		
						if( cntx > rect.left-pItem->w && cntx< rect.right && 
							cnty > rect.top-pItem->h && cnty< rect.bottom )
							flag++;
					}
				}
				if ( flag==0 ) {
					pItem->x = cntx;
					pItem->y = cnty;
					sInven[pItem->ItemPosition-2].ItemIndex =0;
					pItem->ItemPosition = 0; 
					return TRUE;
				}

			}
		}
	}
	return FALSE;

}

//귀환 아이템을 사용한다 
int cINVENTORY::UseEcore(sITEM *pItem) { return TRUE; }

//송편을 먹는다 
int cINVENTORY::EatSongPyeun(sITEM *pItem) { return TRUE; }
//마우스에 있는 아이템을 리턴한다 
int cINVENTORY::ResetMouseItem() { return TRUE; }
//CheckInvenItemForm();
//ReFormInvenItem();
//////////////////////인벤토리 아이템의 조작을 방지한다 
int cINVENTORY::CheckInvenItemForm()
{ //데이타의 값을 더해서 변수에 넣어준다 
	DWORD TempCheckDataSum = 0;

	for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(InvenItem[i].Class != ITEM_CLASS_POTION){
				//TempCheckDataSum += (i+1)*InvenItem[i].x;
				TempCheckDataSum += (i+1)*InvenItem[i].y;
				TempCheckDataSum += (i+1)*InvenItem[i].Class;
				TempCheckDataSum += InvenItem[i].sItemInfo.ItemHeader.dwChkSum;
				TempCheckDataSum += InvenItem[i].sItemInfo.ItemHeader.Head;
			}

		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].Class != ITEM_CLASS_POTION){
				//TempCheckDataSum += (i+1)*InvenItemTemp[i].x;
				TempCheckDataSum += (i+1)*InvenItemTemp[i].y;
				TempCheckDataSum += (i+1)*InvenItemTemp[i].Class;
				TempCheckDataSum += InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum;
				TempCheckDataSum += InvenItemTemp[i].sItemInfo.ItemHeader.Head;
			}

		}

	}
	if(TempCheckDataSum != InvenDataCheckSum){
		SendSetHackUser(2); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 
		//TempCheckDataSum = 0;

	}
	return TRUE;
}

//데이타의 값을 셋팅한다 
int cINVENTORY::ReFormInvenItem()
{
	DWORD Temp = 0;
	InvenDataCheckSum = 0;

	for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(InvenItem[i].Class != ITEM_CLASS_POTION){
				//InvenDataCheckSum += (i+1)*InvenItem[i].x;
				InvenDataCheckSum += (i+1)*InvenItem[i].y;
				InvenDataCheckSum += (i+1)*InvenItem[i].Class;
				InvenDataCheckSum += InvenItem[i].sItemInfo.ItemHeader.dwChkSum;
				InvenDataCheckSum += InvenItem[i].sItemInfo.ItemHeader.Head;
			}

		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].Class != ITEM_CLASS_POTION){
				//InvenDataCheckSum += (i+1)*InvenItemTemp[i].x;
				InvenDataCheckSum += (i+1)*InvenItemTemp[i].y;
				InvenDataCheckSum += (i+1)*InvenItemTemp[i].Class;
				InvenDataCheckSum += InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum;
				InvenDataCheckSum += InvenItemTemp[i].sItemInfo.ItemHeader.Head;
			}

		}

	}
	Temp = InvenDataCheckSum;
	
	return TRUE;

}

int cINVENTORY::CheckMoneyLimit(int Money)
{

	int CheckMoney = 0;  
	if(sinChar->Level <= 10){
		if((sinChar->Money + Money) <= MAX_GOLD_LOW_LEVEL)
			return TRUE;

	}
	else{
		switch(sinChar->ChangeJob){
			case 0:
				if((sinChar->Money + Money) <= (sinChar->Level* MAX_GOLD_LOW_LEVEL)-1800000)
					return TRUE;
			break;

			case 1:
				if( ( sinChar->Money + Money) <= MAX_GOLD_TIER2)return TRUE;
			break;
			case 2:
				if( ( sinChar->Money + Money) <= MAX_GOLD_TIER3)return TRUE;

			break;
			//4차 전업(임시)
			case 3:
				if( ( sinChar->Money + Money) <= MAX_GOLD_TIER4)return TRUE;
			break;

		}
	}	

	return FALSE;

/*
	int CheckMoney = 0;  
	if(sinChar->Level <= 20){
		if((sinChar->Money + Money) <= 200000 )
			return TRUE;

	}
	else{
		if((sinChar->Money + Money) <= (sinChar->Level*200000)-1800000) 
			return TRUE;

	}
	
	cMessageBox.ShowMessage(MESSAGE_OVER_MONEY);
	return FALSE;
*/

}

//////포션의 갯수를 더한다 
int sinPotionNum[4] = {0,0,0,0};
int CheckPotionNum[4] = { 0,0,0,0};
int cINVENTORY::ReFormPotionNum()
{
	PotionCheckNumJumpFlag =1;
	
	for(int j=0;j<4;j++){
		sinPotionNum[j] = 0;
	
	}	
	for(int i=0;i<INVENTORY_MAXITEM ; i++){ 
		if(InvenItem[i].Flag){
			if(InvenItem[i].Class  == ITEM_CLASS_POTION){
				switch(InvenItem[i].CODE & sinITEM_MASK3){
				case sin01:
					sinPotionNum[0] += InvenItem[i].sItemInfo.PotionCount;

				break;
				case sin02:
					sinPotionNum[1] += InvenItem[i].sItemInfo.PotionCount;

				break;
				case sin03:
					sinPotionNum[2] += InvenItem[i].sItemInfo.PotionCount;

				break;
				case sin04:
					sinPotionNum[3] += InvenItem[i].sItemInfo.PotionCount;

				break;
				}
			}
		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].Class  == ITEM_CLASS_POTION){
				switch(InvenItemTemp[i].CODE & sinITEM_MASK3){
				case sin01:
					sinPotionNum[0] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				case sin02:
					sinPotionNum[1] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				case sin03:
					sinPotionNum[2] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				case sin04:
					sinPotionNum[3] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				}
			}
		}
	}
	/*
	if(MouseItem.Flag){
		if(MouseItem.Class  == ITEM_CLASS_POTION){
			switch(MouseItem.sItemInfo.CODE & sinITEM_MASK3){
				case sin01:
					sinPotionNum[0] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin02:
					sinPotionNum[1] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin03:
					sinPotionNum[2] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin04:
					sinPotionNum[3] += MouseItem.sItemInfo.PotionCount;

				break;
			}
		}
	}
	*/
	PotionCheckNumJumpFlag = 0;
	return TRUE;
}

//포션을 체크한다 

DWORD PotionCheckDelayTime = 0;
int cINVENTORY::CheckPotionNumForm()
{
/*	
	PotionCheckDelayTime++;
	if(PotionCheckDelayTime < 70*10)return FALSE;
	PotionCheckDelayTime = 0;
*/	
	
	for(int k=0;k<4;k++){
		CheckPotionNum[k] = 0;
	
	}	
	for(int i=0;i<INVENTORY_MAXITEM ; i++){ 
		if(InvenItem[i].Flag){
			if(InvenItem[i].Class  == ITEM_CLASS_POTION){
				switch(InvenItem[i].CODE & sinITEM_MASK3){
				case sin01:
					CheckPotionNum[0] += InvenItem[i].sItemInfo.PotionCount;

				break;
				case sin02:
					CheckPotionNum[1] += InvenItem[i].sItemInfo.PotionCount;

				break;
				case sin03:
					CheckPotionNum[2] += InvenItem[i].sItemInfo.PotionCount;

				break;
				case sin04:
					CheckPotionNum[3] += InvenItem[i].sItemInfo.PotionCount;

				break;
				}
			}
		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].Class  == ITEM_CLASS_POTION){
				switch(InvenItemTemp[i].CODE & sinITEM_MASK3){
				case sin01:
					CheckPotionNum[0] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				case sin02:
					CheckPotionNum[1] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				case sin03:
					CheckPotionNum[2] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				case sin04:
					CheckPotionNum[3] += InvenItemTemp[i].sItemInfo.PotionCount;

				break;
				}
			}
		}
	}
	/*
	if(MouseItem.Flag){
		if(MouseItem.Class  == ITEM_CLASS_POTION){
			switch(MouseItem.sItemInfo.CODE & sinITEM_MASK3){
				case sin01:
					CheckPotionNum[0] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin02:
					CheckPotionNum[1] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin03:
					CheckPotionNum[2] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin04:
					CheckPotionNum[3] += MouseItem.sItemInfo.PotionCount;

				break;
			}
		}
	}
	*/

	for(int j=0;j<4;j++){
		if(CheckPotionNum[j] !=sinPotionNum[j]){
			//SendSetHackUser(TRUE); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 
			SendSetHackUser2(1510,j); //해킹한 유저를 신고한다
		}
	}
	return TRUE;

}


//////////////// 각종 특별한 아이템을 체크한다 
int cINVENTORY::CheckSpecialItem(sITEM *pItem)
{
	//////퀘스트용 아이템일경우 
	if(pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST || pItem->sItemInfo.UniqueItem)
		return FALSE;


	return TRUE;
}


/*----------------------------------------------------------------------------*
*						인벤토리 이펙트 
*-----------------------------------------------------------------------------*/	
//인벤토리 파티클을 보여준다 
int dwInvenParticleShowTime = 0;
int cINVENTORY::ShowInvenEffectParticle()
{
	dwInvenParticleShowTime++; 
	if(dwInvenParticleShowTime <= 5)return FALSE;
	dwInvenParticleShowTime = 0;

	int TempX,TempY;
	
	if(MouseItem.Flag){
		if(MouseItem.sItemInfo.ItemKindCode == ITEM_KIND_QUEST || MouseItem.sItemInfo.UniqueItem){
			TempX = rand()%MouseItem.w;
			TempY = rand()%MouseItem.h;
			SetInvenParticle(MouseItem.x+TempX,MouseItem.y+TempY,SIN_EFFECT_SHOW_INVEN);

		}
	}

	for(int i = 0 ; i < INVENTORY_MAXITEM ; i++){  
		if(InvenItem[i].Flag){
			//////// 경품권 이펙트를 살자쿵 넣어봄다 (일단 뺌다)
			/*
			if((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2 ) == sinGP1 ){
				TempX = rand()%InvenItem[i].w;
				TempY = rand()%InvenItem[i].h;
				SetInvenParticle(InvenItem[i].x+TempX,InvenItem[i].y+TempY,SIN_EFFECT_SHOW_GYUNGFUM);

			}
			*/

			if(InvenItem[i].sItemInfo.ItemKindCode == ITEM_KIND_QUEST || InvenItem[i].sItemInfo.UniqueItem){
				TempX = rand()%InvenItem[i].w;
				TempY = rand()%InvenItem[i].h;
				SetInvenParticle(InvenItem[i].x+TempX,InvenItem[i].y+TempY,SIN_EFFECT_SHOW_INVEN);

				if(InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO){
					if(i == sInven[0].ItemIndex-1 ){ //양손무기에서두 이펙트가 나오게 살짝 땜빵을 해봅니다
						TempX = rand()%(sInven[1].Rect.right-10 - sInven[1].Rect.left+10);
						TempY = rand()%(sInven[1].Rect.bottom-10 - sInven[1].Rect.top+10);
						SetInvenParticle(sInven[1].Rect.left+TempX,sInven[1].Rect.top+TempY,SIN_EFFECT_SHOW_INVEN);
					}

				}

			}
		}
	}
	
	//트레이드 창에서 이펙트 보이기 
	for ( int i = 0; i < MAX_TRADE_ITEM; i++ )
	{
		if(sTrade.TradeItem[i].Flag){
			if(sTrade.TradeItem[i].sItemInfo.UniqueItem){
				TempX = rand()%sTrade.TradeItem[i].w;
				TempY = rand()%sTrade.TradeItem[i].h;
				SetInvenParticle(sTrade.TradeItem[i].x+TempX,sTrade.TradeItem[i].y+TempY,SIN_EFFECT_SHOW_TRADE);

			}
		}
		if(sTradeRecv.TradeItem[i].Flag){
			if(sTradeRecv.TradeItem[i].sItemInfo.UniqueItem){
				TempX = rand()%sTradeRecv.TradeItem[i].w;
				TempY = rand()%sTradeRecv.TradeItem[i].h;
				SetInvenParticle(sTradeRecv.TradeItem[i].x+TempX,sTradeRecv.TradeItem[i].y+TempY,SIN_EFFECT_SHOW_TRADE);

			}
		}

	}

	//창고아이템에서 이펙트 보이기
	for ( int i = 0; i < 100; i++ )
	{
		if(sWareHouse.WareHouseItem[i].Flag){
			if(sWareHouse.WareHouseItem[i].sItemInfo.UniqueItem){
				TempX = rand()%sWareHouse.WareHouseItem[i].w;
				TempY = rand()%sWareHouse.WareHouseItem[i].h;
				SetInvenParticle(sWareHouse.WareHouseItem[i].x+TempX,sWareHouse.WareHouseItem[i].y+TempY,SIN_EFFECT_SHOW_WAREHOUSE);
			}
		}
	}
	return TRUE;
}

//이펙트를 셋팅한다 
int cINVENTORY::SetInvenParticle(int x , int y , int Kind)
{
	
	int i , k;
	int TempSize;
	int Count=0; 

	for(i = 0 ; i < MAX_INVEN_PARTICLE ; i++){     
		if(!sInven_Effect[i].Time){
			sInven_Effect[i].Kind = Kind;
			sInven_Effect[i].Time = 1; 
			sInven_Effect[i].Max_Time = rand()%20 +3;

			sInven_Effect[i].InvenFace.x = x;
			sInven_Effect[i].InvenFace.y = y;
			TempSize = rand()%13+1;
			sInven_Effect[i].InvenFace.width  = TempSize; 
			sInven_Effect[i].InvenFace.height = TempSize;

			//sInven_Effect[i].Speed.x = rand()%3+1;
			sInven_Effect[i].Speed.y = rand()%12+3;

			if(Kind == SIN_EFFECT_SHOW_GYUNGFUM){ //요건 경품권  
				sInven_Effect[i].InvenFace.MatNum =	MatInvenParticle[4]; 
	//			sInven_Effect[i].InvenFace.r = 0;//rand()%255;
	//			sInven_Effect[i].InvenFace.g = rand()%255;
	//			sInven_Effect[i].InvenFace.b = rand()%255;
				sInven_Effect[i].InvenFace.r = 255;
				sInven_Effect[i].InvenFace.g = 255;
				sInven_Effect[i].InvenFace.b = 255;
				sInven_Effect[i].InvenFace.Transparency = rand()%55 + 200; 
				sInven_Effect[i].FadeType = 1;
				if(sInven_Effect[i].InvenFace.width > 5 ){
					sInven_Effect[i].InvenFace.width -= 2;
					sInven_Effect[i].InvenFace.height -= 2;

				}
				if(sInven_Effect[i].InvenFace.width < 5 ){
					memset(&sInven_Effect[i],0,sizeof(sINVENTORY_EFFECT));  //스페셜 왕 날치기 -_-;;

				}

			}
			else{
				sInven_Effect[i].InvenFace.MatNum =	MatInvenParticle[0];
	//			sInven_Effect[i].InvenFace.r = 0;//rand()%255;
	//			sInven_Effect[i].InvenFace.g = rand()%255;
	//			sInven_Effect[i].InvenFace.b = rand()%255;
				sInven_Effect[i].InvenFace.r = 255;
				sInven_Effect[i].InvenFace.g = 255;
				sInven_Effect[i].InvenFace.b = 255;
				sInven_Effect[i].InvenFace.Transparency = rand()%55 + 200; 
				sInven_Effect[i].FadeType = 1;
			}
			///////날치기 복사(여러개 겹치면 밝아진다나 음...) 
			if(Kind == 1){ //인벤토리
				for(k = 0 ; k < MAX_INVEN_PARTICLE ; k++){    
					if(!sInven_Effect[k].Time){
						Count++;
						if(Count > 2){
							Count = 0;
							break;

						}
						memcpy(&sInven_Effect[k],&sInven_Effect[i],sizeof(sINVENTORY_EFFECT));
					}
					
				}
			}
			break;
		}

	}
	
	return TRUE;
}

DWORD dwInvenParticleMoveTime =0;
//인벤토리 이펙트를 움직인다
int cINVENTORY::MoveInvenParticle() //앞으로 절때 건들지 말것!
{
	dwInvenParticleMoveTime++;
	if(dwInvenParticleMoveTime <= 5)return FALSE;
	dwInvenParticleMoveTime = 0;

	int i = 0;
	for(i = 0 ; i < MAX_INVEN_PARTICLE ; i++){    
		if(sInven_Effect[i].Time){ 
			sInven_Effect[i].Time++;
			//if(sInven_Effect[i].Time >= sInven_Effect[i].Max_Time)sInven_Effect[i].Time = 0;
			sInven_Effect[i].InvenFace.y -= sInven_Effect[i].Speed.y;

			if(sInven_Effect[i].FadeType == 1) 
				sInven_Effect[i].InvenFace.Transparency += 14;
			if(sInven_Effect[i].FadeType == 2)
				sInven_Effect[i].InvenFace.Transparency -= 30;
			
			if(sInven_Effect[i].InvenFace.Transparency >= 255)
				sInven_Effect[i].FadeType = 2;
			
			if(sInven_Effect[i].InvenFace.Transparency <= 0)
				sInven_Effect[i].Time = 0;

			if(sInven_Effect[i].Kind != SIN_EFFECT_SHOW_GYUNGFUM){ 
				if((sInven_Effect[i].Time%5) == 0){ 
					sInven_Effect[i].AniCount++;
					if(sInven_Effect[i].AniCount > 3)sInven_Effect[i].AniCount = 0;
					sInven_Effect[i].InvenFace.MatNum = MatInvenParticle[sInven_Effect[i].AniCount];

				}
			}

			if(sInven_Effect[i].InvenFace.Transparency <= 0)sInven_Effect[i].Time = 0; 
			
		}

	}

	return TRUE;
}

//이펙트 이미지를 그린다 
int cINVENTORY::sinDrawInvenTexImage(smFACE2D *Face) { return TRUE; }


//아이템이 셋팅될수있는 지를 체크 
int cINVENTORY::CheckSetEmptyArea(sITEM *pItem)
{
	int cntx,cnty;
	int cx,cy;

	RECT	rect={0,0,0,0};
	int		flag=0;

	cx = (22*INVENTORY_BOX_ROW)-pItem->w; //아이템이 들어갈수있는 가장 끝 X좌표 
	cy = (22*INVENTORY_BOX_COL)-pItem->h; //아이템이 들어갈수있는 가장 끝 Y좌표

	//코드가 같지 않거나 아이템이 비어있지 않은 곳에 셋팅할때 검색해서 위치를 찾아낸다 
	for( cntx=StartX ; cntx <= StartX+cx ; cntx+=22){
		for( cnty=StartY ; cnty <= StartY+cy ; cnty+=22){ 
			flag = 0;
			for(int i=0 ; i<INVENTORY_MAXITEM ; i++){
				if(InvenItem[i].Flag){
					rect.left   = InvenItem[i].x;
					rect.right  = InvenItem[i].x+InvenItem[i].w;
					rect.top    = InvenItem[i].y;
					rect.bottom = InvenItem[i].y+InvenItem[i].h;

					if( cntx > rect.left-pItem->w && cntx< rect.right && 
						cnty > rect.top-pItem->h && cnty< rect.bottom )
						flag++;
				}
			}
			if ( flag==0 ) {
				pItem->SetX = cntx;
				pItem->SetY = cnty;
				pItem->ItemPosition = 0; 
				return TRUE;

			}
		}
	}

	return FALSE;

}

//아이템 새인증 (서버 메세지) 
int cINVENTORY::ResetInvenItemToServer(DWORD CODE ,DWORD Head , DWORD CheckSum , DWORD Time )
{	
	int i = 0;
	for(i = 0; i < INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(InvenItem[i].CODE == CODE && 
				InvenItem[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItem[i].sItemInfo.ItemHeader.dwChkSum == CheckSum ){
					InvenItem[i].sItemInfo.ItemHeader.dwTime = Time;
					return TRUE;

				}

		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].CODE == CODE && 
				InvenItemTemp[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum == CheckSum ){
					InvenItemTemp[i].sItemInfo.ItemHeader.dwTime = Time;
					return TRUE;

				}

		}
	}
	return FALSE;
}

//아이템 삭제 (서버 메세지)
int cINVENTORY::DeleteInvenItemToServer(DWORD CODE ,DWORD Head , DWORD CheckSum)				
{

	//마우스 아이템을 체크해준다 
	if(MouseItem.Flag && MouseItem.CODE == CODE && 
		MouseItem.sItemInfo.ItemHeader.Head == Head &&
		MouseItem.sItemInfo.ItemHeader.dwChkSum == CheckSum ){
			MouseItem.Flag = 0;
	}

	int i = 0;
	for(i = 0; i < INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(InvenItem[i].CODE == CODE && 
				InvenItem[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItem[i].sItemInfo.ItemHeader.dwChkSum == CheckSum ){
					InvenItem[i].Flag = 0;
					if(InvenItem[i].ItemPosition){
						//양손아이템이 한손에 있을경우에는 지워준다 
						if(InvenItem[i].Class == ITEM_CLASS_WEAPON_TWO){
							if(InvenItem[i].ItemPosition == 1 ){//오른손 
								if(InvenItem[i].CODE != InvenItem[sInven[1].ItemIndex-1].CODE){
									InvenItem[sInven[1].ItemIndex-1].Flag = 0;
									InvenItem[i].Flag = 0;
									sInven[1].ItemIndex = 0; //인벤토리 박스 초기화 
									sInven[0].ItemIndex = 0; //인벤토리 박스 초기화 
									if(InvenItem[i].SetModelPosi)
										sinSetCharItem(InvenItem[i].CODE , InvenItem[i].SetModelPosi, FALSE); 

								}
							}
							if(InvenItem[i].ItemPosition == 2 ){//왼손 
								if(InvenItem[i].CODE != InvenItem[sInven[0].ItemIndex-1].CODE){
									InvenItem[sInven[0].ItemIndex-1].Flag = 0;
									InvenItem[i].Flag = 0;
									sInven[0].ItemIndex = 0; //인벤토리 박스 초기화 
									sInven[1].ItemIndex = 0; //인벤토리 박스 초기화 
									if(InvenItem[i].SetModelPosi)
										sinSetCharItem(InvenItem[i].CODE , InvenItem[i].SetModelPosi, FALSE); 

								}
							}

						}
						sInven[InvenItem[i].ItemPosition-1].ItemIndex  = 0 ;
							if(InvenItem[i].SetModelPosi)
								sinSetCharItem(InvenItem[i].CODE , InvenItem[i].SetModelPosi, FALSE); 

					}
					ReFormInvenItem();
					ReformCharForm();//재인증 
					ReFormPotionNum();
					SetItemToChar();
					break;

				}

		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].CODE == CODE && 
				InvenItemTemp[i].sItemInfo.ItemHeader.Head == Head &&
				InvenItemTemp[i].sItemInfo.ItemHeader.dwChkSum == CheckSum ){
					InvenItemTemp[i].Flag = 0;
					ReFormInvenItem();
					ReformCharForm();//재인증 
					ReFormPotionNum();
					SetItemToChar();
					break;

				}
		}
	}

	//허상 아이템이 있을경우 체크해서 다시한번 지워준다
	if(!sInven[0].ItemIndex){
		if(sInven[1].ItemIndex){
			if(InvenItem[sInven[1].ItemIndex-1].Flag){
				if(InvenItem[sInven[1].ItemIndex-1].Class == ITEM_CLASS_WEAPON_TWO){
					if(!InvenItem[sInven[1].ItemIndex-1].sItemInfo.ItemHeader.Head){
						InvenItem[sInven[1].ItemIndex-1].Flag = 0;
						sInven[1].ItemIndex = 0;
						ReFormInvenItem();
						ReformCharForm();//재인증 
						ReFormPotionNum();
						SetItemToChar();
					}
				}
			}
		}
	}
	cInvenTory.CheckWeight();
	return TRUE;
}

//캐릭터 파라메터 조작을 방지한다 
int cINVENTORY::HackCharStateCheck(smCHAR_INFO *pChar_Info , smCHAR_INFO *desChar_Info)
{
	int HackFlag = 0;

	if(pChar_Info->Attack_Damage[0] < desChar_Info->Attack_Damage[0]){
		HackFlag = 1;

	}
	if(pChar_Info->Attack_Damage[1] < desChar_Info->Attack_Damage[1]){
		HackFlag = 1;

	}
	if(pChar_Info->Defence < desChar_Info->Defence){
		HackFlag = 1;

	}
	if(pChar_Info->Absorption < desChar_Info->Absorption){
		HackFlag = 1;

	}
	if(pChar_Info->Accuracy < desChar_Info->Accuracy){
		HackFlag = 1;

	}
	if(pChar_Info->Attack_Speed < desChar_Info->Attack_Speed){
		HackFlag = 1;

	}
	if(pChar_Info->Chance_Block < desChar_Info->Chance_Block){
		HackFlag = 1;

	}
	if(pChar_Info->Critical_Hit < desChar_Info->Critical_Hit){
		HackFlag = 1;

	}
	if(pChar_Info->Shooting_Range < desChar_Info->Shooting_Range){
		HackFlag = 1;

	}
	if(HackFlag){
		smTRANS_COMMAND	smTransCommand;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.code = smTRANSCODE_FIX_ATTACK;
		smTransCommand.WParam = pChar_Info->Attack_Damage[1];
		smTransCommand.LParam = pChar_Info->Defence;
		smTransCommand.SParam = (int)(pChar_Info->Absorption*256);
		smTransCommand.EParam = 0;
		if ( smWsockDataServer ) 
			smWsockDataServer->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );

		return FALSE;
	}
	return TRUE;
}
	
//잘못 배치된 아이템을 재배치한다 
int cINVENTORY::CheckResetInvenItem()
{
	RECT rect;
	for(int i = 0; i < INVENTORY_MAXITEM ; i++){
		if(InvenItem[i].Flag){
			if(InvenItem[i].ItemPosition){
				if(!sInven[InvenItem[i].ItemPosition-1].ItemIndex || sInven[InvenItem[i].ItemPosition-1].ItemIndex-1 != i )continue;
				rect.left   = sInven[InvenItem[i].ItemPosition-1].Rect.left;
				rect.top    = sInven[InvenItem[i].ItemPosition-1].Rect.top;
				rect.right  = sInven[InvenItem[i].ItemPosition-1].Rect.right;
				rect.bottom = sInven[InvenItem[i].ItemPosition-1].Rect.bottom;
				
				if(InvenItem[i].x <	rect.left || InvenItem[i].y < rect.top ||
					InvenItem[i].x + InvenItem[i].w > rect.right+2 || InvenItem[i].y + InvenItem[i].h > rect.bottom+2 ){
						
						InvenItem[i].x = rect.left + (((rect.right - rect.left) - InvenItem[i].w)/2);
						InvenItem[i].y = rect.top + (((rect.bottom - rect.top) - InvenItem[i].h)/2);
						ReFormInvenItem();
				}
			}

		}
		if(InvenItemTemp[i].Flag){
			if(InvenItemTemp[i].ItemPosition){
				rect.left   = sInven[InvenItemTemp[i].ItemPosition-1].Rect.left;
				rect.top    = sInven[InvenItemTemp[i].ItemPosition-1].Rect.top;
				rect.right  = sInven[InvenItemTemp[i].ItemPosition-1].Rect.right;
				rect.bottom = sInven[InvenItemTemp[i].ItemPosition-1].Rect.bottom;
				
				if(InvenItemTemp[i].x <	rect.left || InvenItemTemp[i].y < rect.top ||
					InvenItemTemp[i].x + InvenItemTemp[i].w > rect.right+2 || InvenItemTemp[i].y + InvenItemTemp[i].h > rect.bottom+2 ){
						
						InvenItemTemp[i].x = rect.left + (((rect.right - rect.left) - InvenItemTemp[i].w)/2);
						InvenItemTemp[i].y = rect.top + (((rect.bottom - rect.top) - InvenItemTemp[i].h)/2);
						ReFormInvenItem();

				}
			}
		}
	}
	return TRUE;
}

//같은 코드의 아이템이 있는지를 찾는다
int cINVENTORY::SearchItemCode(DWORD CODE , int KindFlag)
{
	int ItemCount=0;
	if(CODE){
		for(int i = 0; i < INVENTORY_MAXITEM ; i++){
			if(InvenItem[i].Flag){
				if(InvenItem[i].CODE == CODE)ItemCount++;
			}
			if(InvenItemTemp[i].Flag){
				if(InvenItemTemp[i].CODE == CODE)ItemCount++;
			}
		}
		return ItemCount;
	}

	if(KindFlag){
		for(int i = 0; i < INVENTORY_MAXITEM ; i++){
			if(InvenItem[i].Flag){
				if(InvenItem[i].sItemInfo.ItemKindCode == KindFlag)ItemCount++;
			}
			if(InvenItemTemp[i].Flag){
				if(InvenItemTemp[i].sItemInfo.ItemKindCode == KindFlag)ItemCount++;
			}
		}
		return ItemCount;
	}
	return FALSE;
}

//최대데미지가 나올수있는 아이템을 체크해서 서버로 보내준다 
int cINVENTORY::CheckDamage()
{
	int Damage[2] = {0,0};
	int Critical = 0;
	int SendDamage[2] = {0,0};
	int SendCritical = 0;
	int RealDamage[2] = {0,0};
	int RealSendCritical = 0;

	int i = 0 , j = 0 , k=0 ,t=0;

	DWORD WeaponCode[7] = {sinWA1,sinWS1,sinWP1,sinWS2,sinWT1,sinWM1,sinWM1};
	int   CharCode[7]   = {SIN_CHAR_FIGHTER,SIN_CHAR_ARCHER,SIN_CHAR_PIKEMAN,SIN_CHAR_KNIGHT,SIN_CHAR_ATALANTA,SIN_CHAR_PRIESTESS,SIN_CHAR_MAGICIAN};

	sITEM *pItem=0;
	pItem = new sITEM;
	for( i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		Damage[0] = 0; Damage[1] = 0; Critical = 0; //초기화 
		if(i<100){
			if(!InvenItem[i].Flag)continue;
			if(InvenItem[i].NotUseFlag)continue;
			memcpy(pItem,&InvenItem[i],sizeof(sITEM));

		}
		else{
			if(!InvenItemTemp[i-100].Flag)continue;
			if(InvenItemTemp[i-100].NotUseFlag)continue;
			memcpy(pItem,&InvenItemTemp[i-100],sizeof(sITEM));
		}
		if(pItem->sItemInfo.Damage[0] || pItem->sItemInfo.Damage[1] || pItem->sItemInfo.Critical_Hit){
			Damage[0] += pItem->sItemInfo.Damage[0];
			Damage[1] += pItem->sItemInfo.Damage[1];
			Critical  += pItem->sItemInfo.Critical_Hit;

			//박상열휴가갔네
			if((sinDamageFunction[0] == 1 && (pItem->WeaponClass == 1 || pItem->WeaponClass == 3))){
				SendDamage[0] = 1+(Damage[0]*(sinChar->Strength+130)/130) +
											((sinChar->Talent + sinChar->Dexterity)/40)+((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Strength+130)/130) + 
											((sinChar->Talent + sinChar->Dexterity)/40);
			}

			if((sinDamageFunction[0] == 2 && (pItem->WeaponClass == 1 || pItem->WeaponClass == 3))){
			////(근접형2)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Strength+150)/150) +
											((sinChar->Talent + sinChar->Dexterity)/45)+((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Strength+150)/150) +
											((sinChar->Talent + sinChar->Dexterity)/45);
			}
			if((sinDamageFunction[0] == 3 && (pItem->WeaponClass == 1 || pItem->WeaponClass == 3))){
			////(근접형3)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Strength+190)/190) +
											((sinChar->Talent + sinChar->Dexterity)/50)+((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Strength+190)/190) +
											((sinChar->Talent + sinChar->Dexterity)/50);
			}
			if(sinDamageFunction[1] == 1 && pItem->WeaponClass == 2){
			////(발사형1)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Dexterity+130)/130) + 
											((sinChar->Talent + sinChar->Strength)/40)+ ((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Dexterity+130)/130) + 
											((sinChar->Talent + sinChar->Strength)/40);
			}

			if(sinDamageFunction[1] == 2 && pItem->WeaponClass == 2){
			////(발사형2)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Dexterity+190)/190) +
											((sinChar->Talent + sinChar->Strength)/50)+((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Dexterity+190)/190) +
											((sinChar->Talent + sinChar->Strength)/50);
			}



			if(sinDamageFunction[2] == 1 && pItem->WeaponClass == 3){
			////(법사형1)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Spirit+150)/150) +
											((sinChar->Talent)/30)+((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Spirit+150)/150) +
											((sinChar->Talent)/30);
			}

			if(sinDamageFunction[2] == 2 && pItem->WeaponClass == 3){
			////(법사형2)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Spirit+180)/180) +
											((sinChar->Talent)/30);
				SendDamage[1] = 2+(Damage[1]*(sinChar->Spirit+180)/180) +
											((sinChar->Talent)/25);
			}
			if(sinDamageFunction[2] == 2 && pItem->WeaponClass == 3){
			////(법사형2)
				SendDamage[0] = 1+(Damage[0]*(sinChar->Spirit+170)/170) +
											((sinChar->Talent)/30)+ ((Damage[0]+Damage[1])/16);
				SendDamage[1] = 3+(Damage[1]*(sinChar->Spirit+170)/170) +
											((sinChar->Talent)/30);
			}

			//주무기군 
			for( j= 0 ; j < 7 ; j++){ 
				if(CharCode[j] == sinChar->JOB_CODE){
					if(WeaponCode[j] == (pItem->sItemInfo.CODE & sinITEM_MASK2)){
						SendDamage[0] += sinChar->Level/6;
						SendDamage[1] += sinChar->Level/6;
					}
				}
			}
			//메카니션은 주무기군이 특화루 설정되어있다 
			if(sinChar->JOB_CODE == SIN_CHAR_MECANICIAN){
				if(sinChar->JobBitMask & pItem->sItemInfo.JobCodeMask){
					SendDamage[0] += sinChar->Level/6;
					SendDamage[1] += sinChar->Level/6;
				}		
			}


			//스킬
			//////////////////////// 데미지 가중치 
			for(k = 1 ; k < MAX_USESKILL  ; k++){ //노멀 어택을 빼준다 
				if(sinSkill.UseSkill[k].Flag && sinSkill.UseSkill[k].Point){ //사용 포인트가 있으면 
					//////////////같은 계열무기일경우에 해당 
					for(t = 0; t < 8 ; t++){
						switch(sinSkill.UseSkill[k].CODE){
							case SKILL_MELEE_MASTERY:       //데미지 증가 
								if(sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE&sinITEM_MASK2)){
									SendDamage[0] += (int)(((float)Melee_Mastery_DamagePercent[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[0]);
									SendDamage[1] += (int)(((float)Melee_Mastery_DamagePercent[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[1]);
								}
							break;
							case SKILL_SHOOTING_MASTERY:    //활 데미지 증가  
								if(sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE&sinITEM_MASK2)){
									SendDamage[0] += (int)(((float)S_Mastery_DamagePercent[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[0]);
									SendDamage[1] += (int)(((float)S_Mastery_DamagePercent[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[1]);
							
								}	
							break;
							case SKILL_THROWING_MASTERY:
								if(sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE&sinITEM_MASK2)){
									SendDamage[0] += (int)(((float)T_Mastery_Damage[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[0]);
									SendDamage[1] += (int)(((float)T_Mastery_Damage[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[1]);
							}
							break;
							case SKILL_CRITICAL_MASTERY:
								if(sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE&sinITEM_MASK2)){
									Critical += Critical_Mastery_Critical[sinSkill.UseSkill[k].Point-1];

								}
							break;
							case SKILL_SWORD_MASTERY:
								if(sinSkill.UseSkill[k].Skill_Info.UseWeaponCode[t] == (pItem->sItemInfo.CODE&sinITEM_MASK2)){
									SendDamage[0] += (int)(((float)Sword_Mastery_DamagePercent[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[0]);
									SendDamage[1] += (int)(((float)Sword_Mastery_DamagePercent[sinSkill.UseSkill[k].Point-1]/100)*pItem->sItemInfo.Damage[1]);
							
								}	
							break;

						}
					}
				}
			}

			//특화 
			if(sinChar->JobBitMask & pItem->sItemInfo.JobCodeMask){
				if(pItem->sItemInfo.JobItem.Lev_Damage[1]) // LV/x 특화 +데미지 sinLev_Damage[0]은 사용하고있지않다
					SendDamage[1] +=  (sinChar->Level /pItem->sItemInfo.JobItem.Lev_Damage[1]);			//공격력
				if(pItem->sItemInfo.JobItem.Add_Critical_Hit)
					Critical  += pItem->sItemInfo.JobItem.Add_Critical_Hit;	//1.5배 데미지확율
			}
			SendCritical = Critical;
			SendDamage[0] += 1; //추가 데미지 
			SendDamage[1] += 1;

			if(RealDamage[0] < SendDamage[0] ){
				RealDamage[0] = SendDamage[0];
			}
			if(RealDamage[1] < SendDamage[1] ){
				RealDamage[1] = SendDamage[1];
			}
			if(RealSendCritical < SendCritical )
				RealSendCritical = SendCritical;
		}
	}

	//요기서 값을 보내준다 SendDamage를 보낸다 
	SendMaxDamageToServer(RealDamage[0],RealDamage[1],RealSendCritical);
	return TRUE;
}


//퀘스트 아이템을 체크해서 날려버린다 
int cINVENTORY::ClearQuestItem()
{
	sITEM *pItem=0;
	for(int i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else{
			if(!InvenItemTemp[i-100].Flag)continue;
			pItem = &InvenItemTemp[i-100];
		}
		if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2){
			pItem->Flag = 0;
		}
	}	
	cInvenTory.ReFormInvenItem();
	ResetInvenItemCode();
	return TRUE;
}

// pluto 커플링
int cINVENTORY::DeleteCoupleRing()
{
	sITEM *pItem=0;
	for(int i = 0; i < INVENTORY_MAXITEM*2 ; i++)
	{
		if(i<100)
		{
			if(!InvenItem[i].Flag)continue;
			pItem = &InvenItem[i];
		}
		else
		{
			if(!InvenItemTemp[i-100].Flag)continue;
			pItem = &InvenItemTemp[i-100];
		}
		if( pItem->sItemInfo.CODE == (sinSP1|sin16) )
		{
			pItem->Flag = 0;
			break;
		}
	}	
	cInvenTory.ReFormInvenItem();
	ResetInvenItemCode();
	return TRUE;
}

//링크코어 데이타를 서버에서 받는다
int cINVENTORY::RecvLinkCore(sITEMINFO *pItemInfo)
{
	sITEMINFO *pItemInfo2=0;
	for(int i = 0; i < INVENTORY_MAXITEM*2 ; i++){
		if(i<100){
			if(!InvenItem[i].Flag)continue;
			pItemInfo2 = &InvenItem[i].sItemInfo;
		}
		else{
			if(!InvenItemTemp[i-100].Flag)continue;
			pItemInfo2 = &InvenItemTemp[i-100].sItemInfo;
		}
		if(pItemInfo->BackUpChkSum == pItemInfo2->BackUpChkSum &&
			pItemInfo->BackUpKey == pItemInfo2->BackUpKey &&
			pItemInfo->CODE == pItemInfo2->CODE){
				memcpy(pItemInfo2,pItemInfo,sizeof(sITEMINFO));
				CheckWeight();//무게 보정 
				ReFormInvenItem();
				return TRUE;

			}
	}
	return FALSE;
}
int sinMousePotionNum[4] = {0,0,0,0};
int ChecMousekPotionNum[4] = { 0,0,0,0};

//마우스의 포션을 체크한다
int cINVENTORY::CheckMousePotionNumForm()
{
	for(int k=0;k<4;k++){
		ChecMousekPotionNum[k] = 0;
	
	}	

	if(MouseItem.Flag){
		if(MouseItem.Class  == ITEM_CLASS_POTION){
			switch(MouseItem.sItemInfo.CODE & sinITEM_MASK3){
				case sin01:
					ChecMousekPotionNum[0] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin02:
					ChecMousekPotionNum[1] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin03:
					ChecMousekPotionNum[2] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin04:
					ChecMousekPotionNum[3] += MouseItem.sItemInfo.PotionCount;

				break;
			}
		}
	}
	for(int j=0;j<4;j++){
		if(ChecMousekPotionNum[j] !=sinMousePotionNum[j]){
			//SendSetHackUser(TRUE); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 
			SendSetHackUser2(1510,j); //해킹한 유저를 신고한다
			

		}

	}

	return TRUE;
}
//마우스의 포션을 인증한다 
int cINVENTORY::ReFormMousePotionNum()
{
	for(int j=0;j<4;j++){
		sinMousePotionNum[j] = 0;
	
	}	
	if(MouseItem.Flag){
		if(MouseItem.Class  == ITEM_CLASS_POTION){
			switch(MouseItem.sItemInfo.CODE & sinITEM_MASK3){
				case sin01:
					sinMousePotionNum[0] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin02:
					sinMousePotionNum[1] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin03:
					sinMousePotionNum[2] += MouseItem.sItemInfo.PotionCount;

				break;
				case sin04:
					sinMousePotionNum[3] += MouseItem.sItemInfo.PotionCount;

				break;
			}
		}
	}
	return TRUE;
}

//유니온 코어 이펙트를 셋팅한다
int cINVENTORY::SetUnionCoreEffect(int x , int y) { return TRUE; }

int cINVENTORY::UnionCoreParticleProcess() { return TRUE; }

int cINVENTORY::UnionCoreParticle(int x , int y) { return TRUE; }

//유니온 코어 터지는 이펙트 
int cINVENTORY::UnionCoreParticle2(int x , int y , int Flag) { return TRUE; }


//코드로 인벤토리 공간을 체크한다 
int cINVENTORY::CheckInvenEmpty(sITEM *pItem) { return TRUE; }

//돈이미지와 경험치 이미지를 로딩한다 
int cINVENTORY::LoadMoneyExpImage(sITEM *pItem) { return TRUE; }

//sItemInfo로 아이템을 인벤토리에 셋팅한다
int cINVENTORY::SetInvenToItemInfo(sITEMINFO *pItemInfo)
{

	//돈일 경우
	if(pItemInfo->CODE == (sinGG1|sin01)){
		CheckCharForm();//인증 
		sinPlusMoney(pItemInfo->Money);
		ReformCharForm();//재인증 	
		SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
		return TRUE;
	}

	//경험치일경우 
	if(pItemInfo->CODE == (sinGG1|sin02)){
		AddExp(pItemInfo->Money );
		return TRUE;

	}

	//아이템일 경우 
	sITEM TempItem;
	if(LoadItemImage(pItemInfo , &TempItem)){
		if(cInvenTory.CheckRequireItemToSet(&TempItem)){         //곧바로 셋팅될수있는지를 체크한다  
			if(!cInvenTory.AutoSetInvenItem(&TempItem,1)){
				TempItem.Flag = 0;
				return FALSE;
			}
		}
	}
	return TRUE;
}

/*******************************************************************************
*						Wing Item 
********************************************************************************/	
int cINVENTORY::sinWingQuestNpc() { return TRUE; }

//착용할수있는 아이템인지를 한번더 체크한다 
int cINVENTORY::CheckRequireItemToSet2(sITEM *pItem)
{
	int NotUseItemFlag = 0;

	if(pItem->sItemInfo.Level     > sinChar->Level )NotUseItemFlag = 1;
	if(pItem->sItemInfo.Dexterity > sinChar->Dexterity)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Strength  > sinChar->Strength)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Talent    > sinChar->Talent)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Spirit    > sinChar->Spirit)NotUseItemFlag = 1;
	if(pItem->sItemInfo.Health    > sinChar->Health)NotUseItemFlag = 1;
	if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinFO1){
		int i = 0;
		for(i = 0; i < 12 ; i++){ 
			if((pItem->CODE&sinITEM_MASK3)== SheltomCode2[i]){
				break;
			}
		}
		if(sinChar->Level < ForceOrbUseLevel[i][0] ||
			sinChar->Level > ForceOrbUseLevel[i][1]){
			NotUseItemFlag = 1;
		}
		else{
			NotUseItemFlag = 0;

		}

	}

	if(NotUseItemFlag){
		return FALSE;
	}
	return TRUE;
}

//스페셜아이템 소팅
int cINVENTORY::CheckSpecialItemPosi()
{
	int j = 0;
	for(int i=0 ; i < 84 ; i++){ 
		if(InvenItem[i].Flag){ 
			if(InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL){
				for( j = 84 ; j < INVENTORY_MAXITEM ; j++){
					if(!InvenItem[j].Flag){
						memcpy(&InvenItem[j],&InvenItem[i],sizeof(sITEM));
						memset(&InvenItem[i],0,sizeof(sITEM));

					}
				}
			}
		}
		if(InvenItemTemp[i].Flag){ 
			if(InvenItemTemp[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL){
				for( j = 84 ; j < INVENTORY_MAXITEM ; j++){
					if(!InvenItem[j].Flag){
						memcpy(&InvenItem[j],&InvenItemTemp[i],sizeof(sITEM));
						memset(&InvenItemTemp[i],0,sizeof(sITEM));

					}
				}
			}
		}

	}
	ReFormInvenItem();
	ReformCharForm();//재인증 
	ReFormPotionNum();
	SetItemToChar();
	return TRUE;
}

//스페셜아이템을 보여준다
int cINVENTORY::ChangeSpecialItem(int Flag)
{
	if(Flag==1){ //AB버튼으로 아이템 앞뒤가 바뀔때 스페셜아이템창이 떠있으면 바꿔준다
		if(SpecialItemShowFlag){
			SpecialItemShowFlag = 0;

		}
		else return FALSE;
	}
	else if (Flag ==2){ //아이템을 입수할때나 거래할때 창고를 열때 상점을 열때 등등 창을 바꾼다
		SpecialItemShowFlag = 0;

	}
	else SpecialItemShowFlag ^=1;


	//스페셜아이템의 좌표를 이동시킨다
	for(int i=0 ; i < INVENTORY_MAXITEM ; i++){ 
		if(InvenItem[i].Flag){ 
			if(SpecialItemShowFlag){ //특별한 아이템의 디스플레이를 다르게한다
				if(InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL){
					if(InvenItem[i].x > 10000){
						InvenItem[i].x -= 10000;
					}
				}
				else {
					if(!InvenItem[i].ItemPosition && InvenItem[i].x < 10000){
							InvenItem[i].x += 10000;
					}
				}
			}
			else{
				if(InvenItem[i].sItemInfo.SpecialItemFlag[1] == ITEM_KIND_SPECIAL){
					if(InvenItem[i].x < 10000){
						InvenItem[i].x += 10000;
					}
				}
				else{
					if(!InvenItem[i].ItemPosition){
						if(InvenItem[i].x > 10000){
							InvenItem[i].x -= 10000;
						}
					}
				}
			}
		}
	}
	return TRUE;
}
//코드나 아이템 종류로 인벤토리 아이템 인덱스를 리턴한다
int cINVENTORY::SearchItemIndex(DWORD CODE , int KindFlag)
{
	int i = 0; 
	if(CODE){
		for(i = 0; i < INVENTORY_MAXITEM ; i++){
			if(InvenItem[i].Flag){
				if(InvenItem[i].CODE == CODE){
					return i+1;

				}
			}
		}
		for(i = 0; i < INVENTORY_MAXITEM ; i++){
			if(InvenItemTemp[i].Flag){
				if(InvenItemTemp[i].CODE == CODE){
					return 100+i+1;
				}
			}
		}
	}

	if(KindFlag){
		for(int i = 0; i < INVENTORY_MAXITEM ; i++){
			if(InvenItem[i].Flag){
				if(InvenItem[i].sItemInfo.ItemKindCode == KindFlag){
					return i+1;

				}
			}
			if(InvenItemTemp[i].Flag){
				if(InvenItemTemp[i].sItemInfo.ItemKindCode == KindFlag){
					return 100+i+1;
					
				}
			}
		}
	}
	return FALSE;
}

//박재원 - 수박 모아오기
int WatermelonEventItemIndex[INVENTORY_MAXITEM*2] = {0,};

//장별 - 발렌타인 초콜렛 모아오기
int ValentineEventItemIndex[INVENTORY_MAXITEM*2] = {0,};

// 장별 - 캔디데이즈
int CandydaysEventItemIndex[INVENTORY_MAXITEM*2] = {0,};

// 장별 - 매지컬그린
int MagicalGreenEventItemIndex[INVENTORY_MAXITEM*2] = {0,}; 
int MagicalGreenEmeraldEventItemIndex[INVENTORY_MAXITEM*2] = {0,}; 

// 장별 - 카라의 눈물
int TearOfKaraEventItemIndex[INVENTORY_MAXITEM*2] = {0,};

// 장별 - 조사원을 찾아라 
int FindinvestigatorNineEventItemIndex[INVENTORY_MAXITEM*2] = {0,}; // 나인 아뮬렛
int FindinvestigatorTaleEventItemIndex[INVENTORY_MAXITEM*2] = {0,}; // 테일 아뮬렛

// 박재원 - 같은 아이템의 갯수를 리턴한다.(인벤에서 수박 갯수 리턴하기)
int cINVENTORY::CheckItemCount(DWORD CODE)
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					WatermelonEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					WatermelonEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}


// 장별 - 같은 아이템의 갯수를 리턴한다.(인벤에서 발렌타인 초콜렛 갯수 리턴하기)
int cINVENTORY::CheckValentineItemCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					ValentineEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					ValentineEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}


// 장별 - 캔디데이즈 같은 아이템의 갯수를 리턴한다.(인벤에서 캔디 갯수 리턴하기)
int cINVENTORY::CheckCandyItemCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					CandydaysEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					CandydaysEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}


// 장별 - 매지컬그린
int cINVENTORY::CheckMagicalGreenItemCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}

// 장별 - 매지컬그린
int cINVENTORY::CheckMagicalGreenItemEmeraldCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEmeraldEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					MagicalGreenEmeraldEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}


// 장별 - 카라의 눈물 - 이벤트 아이템 갯수 리턴
int cINVENTORY::CheckTearOfKaraItemCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					TearOfKaraEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					TearOfKaraEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}


// 장별 - 조사원을 찾아라 - 나인 아뮬렛 갯수 리턴
int cINVENTORY::CheckFindinvestigatorNineItemCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorNineEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorNineEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}


// 장별 - 조사원을 찾아라 - 테일 아뮬렛 갯수 리턴
int cINVENTORY::CheckFindinvestigatorTaleItemCount(DWORD CODE) 
{
	int i = 0; 
	int item_count = 0;
	if(CODE)
	{
		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItem[i].Flag)
			{
				if(InvenItem[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorTaleEventItemIndex[item_count-1] = i+1;
				}
			}
		}

		for(i = 0; i < INVENTORY_MAXITEM ; i++)
		{
			if(InvenItemTemp[i].Flag)
			{
				if(InvenItemTemp[i].CODE == CODE)
				{
					item_count++;
					FindinvestigatorTaleEventItemIndex[item_count-1] = 100+i+1;
				}
			}
		}
	}

	return item_count;
}

//복주머니에서 퍼즐로 변경한다
int cINVENTORY::ChangePuzzle(sITEM *pItem)
{
	char szTestBuff2[256];
	szTestBuff2[0] = 0; //버퍼초기화  
	if((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (pItem->sItemInfo.CODE & sinITEM_MASK2) == sinPZ2){
		if(pItem->sItemInfo.PotionCount <= 1){
			pItem->sItemInfo.PotionCount = 2;
			for(int j=0 ; j < MAX_ITEM ; j++){ 
				if(pItem->sItemInfo.CODE == sItem[j].CODE){	
					CheckInvenItemForm(); 
					//memcpy(pItem,&sItem[j],sizeof(sITEM));
					wsprintf(szTestBuff2 ,"Image\\sinImage\\Items\\%s\\it%s.bmp",sItem[j].ItemFilePath,sItem[j].LastCategory);
					pItem->lpItem = sItem[j].lpTempItem;
					pItem->w = sItem[j].w;
					pItem->h = sItem[j].h;
					pItem->CODE = sItem[j].CODE;   //개인 상점을 이용을 위해 코드도 넘겨준다.
					CheckWeight();//무게 보정 
					ReFormInvenItem();
					sinMinusMoney(1000);
					ReformCharForm();//재인증 
					SendSaveMoney(); //금액 조작을 못하게하기위해 호출한다 
					sinUsePotionDelayFlag = 1;
					cInvenTory.UnionCoreParticle2(pItem->x , pItem->y,1); //이펙트 
					cInvenTory.UnionCoreParticle2(pItem->x , pItem->y,1); //이펙트 
					cInvenTory.UnionCoreParticle2(pItem->x , pItem->y,1); //이펙트 
//					SetPuzzleEffect(pItem->x , pItem->y); //이펙트 
					SaveGameData();
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

DWORD CheckPuzzleIndex[4][4] = {
	{sin01,0,sin02,0},
	{sin03,sin04,0,sin06},
	{0,sin05,0,0},
	{sin07,0,sin08,0},
};

// 박재원 - 일본 퍼즐 이벤트(퍼즐 대형 변경)
DWORD CheckPuzzleIndex2[4][4] = {
	{sin01,0,sin02,0},
	{sin03,sin04,0,sin06},
	{0,sin05,0,0},
	{sin07,0,sin08,0},
};
/*
DWORD CheckPuzzleIndex2[4][4] = {
	{sin01,0,sin03,sin04},
	{sin02,0,0,0},
	{sin05,sin06,sin07,0},
	{0,0,sin08,0},
};
*/
DWORD CheckPuzzleX[4] = {8,9,10,11};
int   PuzzleIndex[8] = {0,};

//퍼즐을 체크한다
int cINVENTORY::CheckPuzzle()
{
	int Count = 0;
	int Count2 = 0;
	int i,j,k;
	PuzzleOkFlag = 0;
	for(int p = 0 ; p < 8 ; p++){
		PuzzleIndex[p] = 0;

	}

	for(i = 0 ; i < INVENTORY_MAXITEM ; i++){  
		if(InvenItem[i].Flag){
			if((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1){
				for(j = 0; j < 4 ; j++){
					for(k = 0; k < 4 ; k++){
						if((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) == CheckPuzzleIndex[j][k]){
							if((StartX+(CheckPuzzleX[k]*22)) == InvenItem[i].x &&
								(StartY+(j*22)) == InvenItem[i].y){
								PuzzleIndex[Count] = i+1;
								Count++;

							}
						}
					}
				}
			}

			if((InvenItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2){
				for(j = 0; j < 4 ; j++){
					for(k = 0; k < 4 ; k++){
						if((InvenItem[i].sItemInfo.CODE & sinITEM_MASK3) == CheckPuzzleIndex2[j][k]){
							if((StartX+(CheckPuzzleX[k]*22)) == InvenItem[i].x &&
								(StartY+(j*22)) == InvenItem[i].y){
								PuzzleIndex[Count2] = i+1;
								Count2++;

							}
						}
					}
				}
			}
		}
	}
	if(Count == 8 || Count2 == 8){
		//if(!PuzzleOkFlag)
		//	cMessageBox.ShowMessage(MESSAGE_PERPFECT_PUZZLE);
		PuzzleOkFlag = 1;

	}
	return TRUE;
}


//바벨의 뿔을 체크한다
int cINVENTORY::CheckEventItem()
{
	
	return TRUE;
}

//바벨의 뿔을 보낸다
int cINVENTORY::SendEventItem()
{
	return TRUE;	

}

//나인테일 아물렛을 체크한다
int cINVENTORY::CheckFoxEventItem() { return TRUE; }

//나인테일 아물렛을 보낸다
int cINVENTORY::SendFoxEventItem() { return TRUE; }


//반짝반짝 가루를 체크한다
int cINVENTORY::CheckChristMasEventItem() { return TRUE; }

//반짝반짝 가루를 보낸다
int cINVENTORY::SendChristmasEventItem() { return TRUE; }
//퍼즐을 보낸다
int cINVENTORY::CheckSendPuzzle() { return TRUE; }
	
int cINVENTORY::SendPuzzleNpc() { return TRUE; }

//보여지는 데미지를 조정한다 
int cINVENTORY::IncreViewDamage(int Persent , int Num)
{
	ViewDamagePercent = Persent;
	ViewDamageNum = Num;
	SetItemToChar();
	return TRUE;
}

//ForceOrb의 이펙트를 그린다
int cINVENTORY::DrawForceOrbEffect(int x , int y) { return TRUE; }

//sITEM UseForceOrbBak;
int cINVENTORY::UseForceOrb(sITEM *pItem , int Flag) { return TRUE; }

// 박재원 - 매직 포스 추가(매직 포스를 사용한다)
int cINVENTORY::UseMagicForceOrb(sITEM *pItem , int Flag) { return TRUE; }

// 박재원 - 빌링 매직 포스 추가(빌링 매직 포스를 사용한다)
int cINVENTORY::UseBillingMagicForceOrb(sITEM *pItem , int Flag) { return TRUE; }

// 박재원 - 매직 포스 추가 (매직 포스를 셋팅한다.)
int cINVENTORY::SetMagicForceOrb(DWORD ItemCODE , int Time) { return TRUE; }

// 박재원 - 빌링 매직 포스 추가 (빌링 매직 포스를 셋팅한다.)
int cINVENTORY::SetBillingMagicForceOrb(DWORD ItemCODE , int Time) { return TRUE; }

//포스아이템을 셋팅한다(접속종료및,기타등등)
int cINVENTORY::SetForceOrb(DWORD ItemCODE , int Time) { return TRUE; }

// 박재원 - 캡슐아이템 사용하기(호랑이 캡슐 사용)
int cINVENTORY::UseCapsuleItem(sITEM *pItem) { return TRUE; }

int CristalEventItemIndex[7]={0,};

int PristonAlphabetEventItemIndex[7] = {0,}; // 박재원 - 알파벳 조합 이벤트

//<ha>일곱가지의  보은 크리스탈 체크
int cINVENTORY::CheckCristalEventItem() { return TRUE; }
//<ha>일곱가지의 보은 크리스탈을 서버로 보낸다
int cINVENTORY::SendCristalEventItem() { return TRUE; }

//박재원 - 수박 모아오기 수박 아이템 갯수를 체크한다.
int cINVENTORY::CheckWatermelonEventItem() { return TRUE; }


//장별 - 발렌타인 초콜렛 모아오기 초콜렛 아이템 갯수를 체크한다.
int cINVENTORY::CheckValentineEventItem() { return TRUE; }


// 장별 - 캔디데이즈 캔디 모아오기 캔디 아이템 갯수를 체크한다.
int cINVENTORY::CheckCandydaysEventItem() { return TRUE; }

// 장별 - 매지컬그린  에메랄드, 비취 아이템 갯수를 체크한다.
int cINVENTORY::CheckMagicalGreenEventItem() { return TRUE; }

// 장별 - 카라의 눈물 아이템 갯수를 체크한다.
int cINVENTORY::CheckTearOfKaraEventItem() { return TRUE; }


// 장별 - 조사원을 찾아라 아이템 갯수를 체크한다. - 마력 연구가
int cINVENTORY::CheckFindinvestigatorEventItem() { return TRUE; }


// 장별 - 조사원을 찾아라 아이템 갯수를 체크한다. - 카미유
int cINVENTORY::CheckFindinvestigatorKamiuEventItem() { return TRUE; }


// 장별 - 조사원을 찾아라 아이템 갯수를 체크한다. - 에토
int cINVENTORY::CheckFindinvestigatorEtoEventItem() { return TRUE; }


//박재원 - 수박 모아오기 수박 아이템을 서버로 보낸다
int cINVENTORY::SendWatermelonEventItem() { return TRUE; }




//장별 - 발렌타인 이벤트 // 발렌타인 초콜렛 아이템을 서버로 보낸다
int cINVENTORY::SendValentineEventItem() { return TRUE; }

// 박재원 - 알파벳 조합 이벤트
int cINVENTORY::CheckPristonAlphabetEventItem() { return TRUE; }

// 박재원 - 알파벳 조합 이벤트(알파벳 아이템을 서버로 보낸다)
int cINVENTORY::SendPristonAlphabetEventItem() { return TRUE; }


// 장별 - 캔디데이즈 // 캔디 아이템을 서버로 보낸다
int cINVENTORY::SendCandydaysEventItem() { return TRUE; }



// 장별 - 매지컬그린 - 아이템을 서버로 보낸다
int cINVENTORY::SendMagicalGreenEventItem() { return TRUE; }


// 장별 - 매지컬그린 - 아이템을 서버로 보낸다
int cINVENTORY::SendMagicalGreenEmeraldEventItem() { return TRUE; }


// 장별 - 카라의 눈물 // 눈물 아이템을 서버로 보낸다
int cINVENTORY::SendTearOfKaraEventItem() { return TRUE; }

// 박재원 - 2010 월드컵 이벤트 - 축구공 포션 사용하기
int cINVENTORY::UseSoccerBallPotionItem(sITEM *pItem)
{
	pItem->sItemInfo.PotionCount = 100;
	sinThrowItemToFeild(pItem);
	pItem->Flag = 0;

	cInvenTory.SetItemToChar(); //아이템이 셋팅되면 능력치를 셋팅한다 
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();   //무게를 셋팅한다 
	sinUsePotionDelayFlag = 1;

	return TRUE;
}



// 장별 - 조사원을 찾아라 - 마력연구자
int cINVENTORY::SendFindinvestigatorNineEventItem() { return TRUE; }


// 장별 - 조사원을 찾아라 - 카미유
int cINVENTORY::SendFindinvestigatorTaleEventItem() { return TRUE; }


// 장별 - 조사원을 찾아라 - 에토
int cINVENTORY::SendFindinvestigatorTaleEtoEventItem() { return TRUE; }