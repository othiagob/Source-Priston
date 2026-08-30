#include "sinLinkHeader.h"
#include "field.h"
#include "AreaServer.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"

#include <string>

using namespace std;

cHaPremiumItem  chaPremiumitem;


#define  TELEPORT_FIELDMAX 33
int TelePort_FieldNum[TELEPORT_FIELDMAX][3] = {

	// (Number, FieldNum, FieldLevel)

	// Page1 -> 바람이이끄는길, 고요의계곡, 정령의숲, 아카시아숲, 자유의정원, 고대인의안식처, 폐성곽, 저주받은땅, 잊혀진땅, 오아시스
	{ 0,20, 0}, { 1,19, 0}, { 2,17, 0}, { 3, 0, 0}, { 4, 2, 0}, { 5, 4, 0}, { 6, 5, 0}, { 7, 7, 0},  { 8, 8, 0},  { 9,10, 0},  

	// Page2 -> 고대인의전장, 금단의땅, 벌집동굴, 버섯동굴, 어둠의성전, 고대감옥1층, 고대감옥2층, 고대감옥3층, 저주받은신전1층, 저주받은신전2층
	{10,11,55}, {11,12,55}, {12,25,55}, {13,24,55}, {14,26,65}, {15,13,40}, {16,14,50}, {17,15,60},	 {18,22,70},  {19,23,75},  

	// Page3 -> 저주받은신전3층, 그리디호수, 혼돈의철로, 페룸의심장, 유라빌리지, 겔루비아계곡, 얼어붙은신전, 켈베쥬레어, 혼란스런주둔지, 잃어버린사원
	{20,42,80}, {21,34,70}, {22,27,80}, {23,28,85}, {24,29,90}, {25,31,90}, {26,35,90}, {27,36,90}, {28,37,95}, {29,38,100}, 

	// Page4 -> 끝없는탑1층, 끝없는탑2층, 끝없는탑3층
	{30,40,100}, {31,41,100}, {32,43,102}
};


char *haTeleport_Field[] = {
	"image\\Sinimage\\help\\haTeleport_Field[1].sin",
	"image\\Sinimage\\help\\haTeleport_Field[2].sin",
	"image\\Sinimage\\help\\haTeleport_Field[3].sin",
	"image\\Sinimage\\help\\haTeleport_Field[4].sin",
};

cHaPremiumItem::cHaPremiumItem()
{
	TelePortCore.PageButtonFlag[0] = 0;
	TelePortCore.PageButtonFlag[1] = 0;
	TelePortCore.FieldIndex[0]     = 0;
	TelePortCore.FieldIndex[1]     = 0;
	TelePortCore.PageIndex         = 0;
	Using_ItemKind                 = 0;
	Using_SelectInvenItemIndex	   = 0;
	int i;
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		UpKeepItem[i].PosiX   = 0;
		UpKeepItem[i].PosiY   = 0;
		UpKeepItem[i].Visible = false;
		UpKeepItem[i].IconTime[0] = 0;   
		UpKeepItem[i].IconTime[1] = 0;
		UpKeepItem[i].Alpha       = 255;
		UpKeepItem[i].Alphakind   = 0;
		UpKeepItem[i].Per		  = 0;
		memset(UpKeepItem[i].Id,NULL,sizeof(UpKeepItem[i].Id));
	}

	UseClickPremiumItemFlag = false;

	//멤버 변수 초기화
	m_ThirdEyesTime   = 0;
	m_ExpUpPotionTime = 0;
	m_VampiricCuspidTime  = 0;
	m_ManaRechargingPTime = 0;
	m_ManaReducePotiontime = 0; // pluto 마나 리듀스 포션
	m_ManaReducePotionValue = 0;		// pluto 마나 리듀스 포션 감소 %값
	m_MightofAwellTime = 0;		// pluto 마이트 오브 아웰
	m_MightofAwellWeight = 0;	// pluto 마이트 오브 아웰 소지량
	m_PhenixPetTime = 0;		// pluto 펫(해외)
	m_HelpPetTime = 0; // 박재원 - 빌링 도우미 펫 추가
	m_VampiricCuspidEXTime  = 0;		// 장별 - 뱀피릭 커스핏 EX
	m_StaminaReducePotiontime = 0;	// 박재원 - 근력 리듀스 포션
	m_StaminaReducePotionValue = 0; // 박재원 - 근력 리듀스 포션 감소 %값
	
}
/*----------------------------------------------------------------------------*
*					  소멸자		
*-----------------------------------------------------------------------------*/
cHaPremiumItem::~cHaPremiumItem()
{
	//int i;
	//for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
	//	if(UpKeepItem[i].lpImage){
	//		UpKeepItem[i].lpImage->Release();
	//		UpKeepItem[i].lpImage = 0;
	//	}
	//}
}

/*----------------------------------------------------------------------------*
*					  init		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::init() {}
/*----------------------------------------------------------------------------*
*					  Main		
*-----------------------------------------------------------------------------*/
//왼쪽에 찍일 포지션 좌표값
int ItemUseLeftPosi[SINUPKEEPITEM_MAX][2]={
	{8,80},{8,116},{8,152},{8,188},{8,224},{8,260},{8,296}, {8, 332} 
};		
DWORD dwDelayTime = 0; 
void cHaPremiumItem::Main()
{
	//몬스터 크리스탈 아이템 사용시 딜레이를 준다,
	if( chaPremiumitem.haItemUseDelayFlag == true){
		dwDelayTime++;
		if( dwDelayTime > 210){  //3초로 제한한다.{
			chaPremiumitem.haItemUseDelayFlag = false;
			dwDelayTime = 0;
		}
	}


	//유지형으로 사용돼는 아이콘의 위치정보
	int i,cnt=0;	
	UseClickPremiumItemFlag = false;
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		////시간 체크
		if(UpKeepItem[i].IconTime[0] < UpKeepItem[i].IconTime[1]){
			if(UpKeepItem[i].Visible == true ){
				if(UpKeepItem[i].IconTime[1]-UpKeepItem[i].IconTime[0] <= 0 ){
					UpKeepItem[i].Visible = false;
					CheckUpKeepItem();  //종료시 한번씩 호출해준다.
				}
				cnt++;
				//서버 시간을 받아 와서 찍어준다.
				DWORD NowTime = GetPlayTime_T();			
				switch(UpKeepItem[i].TGAImageNumber){
					case nsPremiumItem::THIRD_EYES:   //써드 아이즈
						if( m_ThirdEyesTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}
					break;
					case nsPremiumItem::EXPUP_POTION:  //경험치 증가 포션
						if( m_ExpUpPotionTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}
					break;
					case nsPremiumItem::VAMPIRIC_CUS:  //뱀피릭 커스핏
						if( m_VampiricCuspidTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;

					
					case nsPremiumItem::VAMPIRIC_CUS_EX:  // 장별 - 뱀피릭 커스핏 EX
						if( m_VampiricCuspidEXTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;
					

					case nsPremiumItem::MANA_RECHAR_P:  //마나 리차징  포션
						if( m_ManaRechargingPTime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
					break;
					case nsPremiumItem::MIGHT_OF_AWELL:	// pluto 마이트 오브 아웰
						if( m_MightofAwellTime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::MANA_REDUCE_P:	// pluto 마나 리듀스 포션
						if( m_ManaReducePotiontime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::PHENIX_PET:	// pluto 펫(해외)
						if( m_PhenixPetTime > 0 )
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_TERRY: // 박재원 - 빌링 도우미 펫 추가
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_NEPSYS: // 박재원 - 빌링 도우미 펫 추가
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_IO: // 박재원 - 빌링 도우미 펫 추가
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::HELP_PET_MUTE: // 박재원 - 빌링 도우미 펫 추가
						if(m_HelpPetTime > 0 ) 
						{
							UpKeepItem[i].IconTime[0] = NowTime;
						}
						break;
					case nsPremiumItem::STAMINA_REDUCE_P:  // 박재원 - 근력 리듀스 포션 
						if( m_StaminaReducePotiontime > 0){
							UpKeepItem[i].IconTime[0] = NowTime;
						}						
						break;
				}
			}
		}
		else
			UpKeepItem[i].Visible = false;
		if(UpKeepItem[i].Visible == true){
			//위치 변경
			if(UpKeepItem[i].PosiY > ItemUseLeftPosi[cnt-1][1])
				UpKeepItem[i].PosiY-=1;
			else{
				UpKeepItem[i].PosiY = ItemUseLeftPosi[cnt-1][1];
				UpKeepItem[i].PosiX = ItemUseLeftPosi[cnt-1][0];
			}
			//마우스 위치 체크

			UpKeepItem[i].Infoflag = false;
			UseClickPremiumItemFlag = false;
			
		}
	}
}
/*----------------------------------------------------------------------------*
*					  Draw		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::Draw() {}

DWORD dwKeepItemTime[7] = { 0, SINITEM_TIME_1HOURS, SINITEM_TIME_3HOURS, SINITEM_TIME_ONEDAY, SINITEM_TIME_SEVENDAY, SINITEM_TIME_30DAY, SINITEM_TIME_3DAY };		//시간테이블

int  cHaPremiumItem::CheckTime(DWORD CurrentTime)
{

	for (int i = 0; i < 8; i++) {
		if (dwKeepItemTime[i] < CurrentTime && CurrentTime < dwKeepItemTime[i + 1]) {
			return i + 1;
		}
	}

	return false;
}
/*----------------------------------------------------------------------------*
*					  스킬 유지형 아이콘 Draw		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::DrawUp() {}
/*----------------------------------------------------------------------------*
*					  LButtonDown		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::LButtonDown(int x, int y) {}
/*----------------------------------------------------------------------------*
*					  LButtonUp		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::LButtonUp(int x,int y)
{

}
DWORD dwPremiumItemCode[]={(sinBI1|sin01),(sinBI1|sin02),(sinBI1|sin03),(sinBI1|sin04),(sinBI1|sin05),(sinBI1|sin06),(sinBI1|sin07)};
/*----------------------------------------------------------------------------*
*Desc: 프리미엄 아이템 사용시  호출		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UsePremiumItem(int kind) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: 스킬 게이지가 생성돼는  아이템일경우 호출	
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetPremiumItemSkill(int kind) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: 텔레포트 코어 사용시 호출						
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseTeleportScrollItem(short ItemIndex) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: 다른 인터페이스창이 떠잇을경우 리턴한다.		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckWindowOpen() { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: 빈배열을 댕겨준다.		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckUpKeepItem()
{
	int i;
	//아이템체크
	for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
		if(UpKeepItem[i].Visible == false){
			if(UpKeepItem[i+1].Visible == true){
				memcpy(&UpKeepItem[i],&UpKeepItem[i+1],sizeof(sUpKeepItem));
				UpKeepItem[i+1].Visible = false;
				break;
			}			
		}
	}
	return true;
}
/*----------------------------------------------------------------------------*
*Desc: 유지형 소모성 아이템을 사용할 경우 디스플레이  	
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetUpKeepItem(int kind, DWORD dwTime, bool visible, char* id, int Percent, short ItemType) { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: 유지형아이콘 설명박스 		
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::InfoBox(int PoisX, int PosiY, int Width, int Height) {}
/*----------------------------------------------------------------------------*
*Desc: 유지형아이콘 설명(이름/유지시간 ) 		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::DrawText() { return TRUE; }
/*----------------------------------------------------------------------------*
*Desc: 헤어틴트포션 사용 		
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseHairtintPotion(int ItemKind)
{
	switch(ItemKind){
		case 1:  //A형
			if( ChangeHairModel(0) )
				return TRUE;
		break;
		case 2:  //B형
			if( ChangeHairModel(1) )
				return TRUE;
		break;
		case 3:  //C형
			if( ChangeHairModel(2) )
				return TRUE;
		break;
		case 4:  //D형
			if( ChangeHairModel(3) )
				return TRUE;
		break;
		case 5:  //E형
			if( ChangeHairModel(4) )
				return TRUE;
		break;
		case 6:  //F형
			if( ChangeHairModel(5) )
				return TRUE;
		break;
		case 7:  //G형
			if( ChangeHairModel(6) )
				return TRUE;
		break;
		case 8:  //H형
			if( ChangeHairModel(7) )
				return TRUE;
		break;
			
		case 9:  //I형
			if( ChangeHairModel(8) )
				return TRUE;
		break;
		case 10:  //J형
			if( ChangeHairModel(9) )
				return TRUE;
		break;
		case 11:  //K형
			if( ChangeHairModel(10) )
				return TRUE;
		break;
		case 12:  //L형
			if( ChangeHairModel(11) )
				return TRUE;
		break;
		case 13:  //M형
			if( ChangeHairModel(12) )
				return TRUE;
		break;
		case 14: //novos cabelos xxstr
			if (ChangeHairModel(14))
				return TRUE;
		break;
		case 15:
			if (ChangeHairModel(18))
				return TRUE;
		break;
		case 16:
			if (ChangeHairModel(19))
				return TRUE;
		break;
		case 17:
			if (ChangeHairModel(20))
				return TRUE;
		break;
		case 18:
			if (ChangeHairModel(21))
				return TRUE;
		break;
		case 19:
			if (ChangeHairModel(22))
				return TRUE;
		break;
		case 20:
			if (ChangeHairModel(23))
				return TRUE;
		break;
		case 21:
			if (ChangeHairModel(24))
				return TRUE;
		break;
		

	}	

	return FALSE; 
}

// 박재원 - 에이징 마스터 아이템 사용
int cHaPremiumItem::UseAgingMaster(int ItemKind)
{
	switch(ItemKind)
	{
	case 0:
		sinCheckAgingLevel(SIN_AGING_ATTACK, true);
		sinCheckAgingLevel(SIN_AGING_CRITICAL, true);
		ReformCharForm(); //재인증 
		break;
	case 1:
		if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinDS1)
		{
			sinCheckAgingLevel(SIN_AGING_BLOCK, true);
			ReformCharForm(); //재인증 
		}
		else if((cInvenTory.InvenItem[sInven[1].ItemIndex-1].CODE &sinITEM_MASK2) == sinOM1)
		{
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, true);
			ReformCharForm(); //재인증 

		}
		break;
	case 2:
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, true);
		ReformCharForm(); //재인증 

		break;
	case 3: // 박재원 - 에이징 마스터(2차) - 암릿
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, true);
		ReformCharForm(); //재인증 

		break;
	case 4: // 박재원 - 에이징 마스터(2차) - 장갑
		sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, true);
		ReformCharForm(); //재인증 

		break;
	case 5: // 박재원 - 에이징 마스터(2차) - 부츠
		sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, true);
		ReformCharForm(); //재인증 
		break;
	}

	return TRUE;
}

// 박재원 - 스킬 마스터 아이템 사용
int cHaPremiumItem::UseSkillMaster(int ItemKind)
{
	switch(ItemKind)
	{
	case 1:
		for(int j=1 ; j<5 ; j++) // 1차 전직후 생기는 스킬
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //데이타를 갱신한다
				}
			}
		}
		break;
	case 2:
		for(int j=5 ; j<9 ; j++) // 2차 전직후 생기는 스킬
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //데이타를 갱신한다
				}
			}
		}
		break;
	case 3:
		for(int j=9 ; j<13 ; j++) // 3차 전직후 생기는 스킬
		{
			if(sinSkill.UseSkill[j].UseSkillCount<10000)
			{
				if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					cSkill.ReformSkillMasteryForm(0,j); //데이타를 갱신한다
				}
			}
		}
		break;
	}

	return TRUE;
}

// 박재원 - 이동 상점 아이템
int cHaPremiumItem::UseMoveShop()
{
	return TRUE;
}
