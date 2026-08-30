/*----------------------------------------------------------------------------*
*	파일명 :  sinSOD2.cpp
*	하는일 :  현재 Test 용으로 쓰이고 있따
*	작성일 :  최종업데이트 4월 
*	적성자 :  박상열 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\clan_Enti.h"
#include "tjboy\\isaocheck\\auth.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"
#include "fileread.h"
#include "field.h"
#include "SrcServer\\onserver.h"


cSINSOD2 cSinSod2;
cSINSIEGE cSinSiege;
sinMESSAGEBOX_NEW sinMessageBox_New;

void*	lpbltr_clanN;
void*	lpbltr_clanB;
int Matbltr_Paper291;
int Matbltr_Paper291_Text;

void*	lpbltr_ButtonBox;
void*	lpbltr_Button_Clan;
void*	lpbltr_Button_Clan_G;
void*	lpbltr_Button_Prize;
void*	lpbltr_Button_Prize_G;
void*	lpbltr_Button_OK;
void*	lpbltr_Button_OK_G;

int Matbltr_Logo;
void*    lpbltr_ClanRank_Title;
int Matbltr_ClanRank_KindBar;
void*    Matbltr_ClanRank_ListLine;


RECT SodButtonRect[3] = {
	{111,393,111+68,393+23},
	{189,393,189+68,393+23},
	{267,393,267+68,393+23},

};

int MatSod2Box[10]; //박스 Mat

/*----------------------------------------------------------------------------*
*						 Init
*-----------------------------------------------------------------------------*/	
void cSINSOD2::Init() {}
/*----------------------------------------------------------------------------*
*						 Load
*-----------------------------------------------------------------------------*/	
void cSINSOD2::Load() {}
/*----------------------------------------------------------------------------*
*						Release
*-----------------------------------------------------------------------------*/	
void cSINSOD2::Release()
{

}
/*----------------------------------------------------------------------------*
*						Draw
*-----------------------------------------------------------------------------*/	
void cSINSOD2::Draw() {}
/*----------------------------------------------------------------------------*
*						DrawText
*-----------------------------------------------------------------------------*/	
void cSINSOD2::DrawText() {}
/*----------------------------------------------------------------------------*
*						Main
*-----------------------------------------------------------------------------*/	
DWORD ClanRankFlagTime = 0;
void cSINSOD2::Main() {}
/*----------------------------------------------------------------------------*
*						Close
*-----------------------------------------------------------------------------*/	
void cSINSOD2::Close()
{

}
/*----------------------------------------------------------------------------*
*						LbuttonDown
*-----------------------------------------------------------------------------*/	
void cSINSOD2::LButtonDown(int x, int y) {}
/*----------------------------------------------------------------------------*
*						LbuttonUp
*-----------------------------------------------------------------------------*/	
void cSINSOD2::LButtonUp(int x , int y)
{


}
/*----------------------------------------------------------------------------*
*						RbuttonDown
*-----------------------------------------------------------------------------*/	
void cSINSOD2::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*						RbuttonUp
*-----------------------------------------------------------------------------*/	
void cSINSOD2::RButtonUp(int x, int y)
{

}
/*----------------------------------------------------------------------------*
*						KeyDown
*-----------------------------------------------------------------------------*/	
void cSINSOD2::KeyDown()
{

}
/*----------------------------------------------------------------------------*
*						Sod2박스를 닫는다
*-----------------------------------------------------------------------------*/	
void cSINSOD2::CloseSod2MessageBox() {}
/*----------------------------------------------------------------------------*
*						Sod2박스를 보여준다
*-----------------------------------------------------------------------------*/	
void cSINSOD2::ShowSod2MessageBox() {}

//새로운 메세지 박스를 띄운다
int ShowSinMessageBox_New(int PosiX, int PosiY, int SizeW, int SizeH, RECT* rect, int ButtonNum) { return TRUE; }

/*----------------------------------------------------------------------------*
*						Web Data 클라이언트로 파싱
*-----------------------------------------------------------------------------*/	
void cSINSOD2::RecvWebDaTa() //현재는 테스트로 쓴다
{
	

}
void cSINSOD2::RecvClanRank(char *szBuff)
{
	//115001132 -아마게돈- 1329660 2004-05-07
	//string Test ="1 호동프린스클랜 10010020 2004/05/05 2 신바람클랜 553340 2004/2332/1 3 펭귄클랜 12131001 2003/05/23";
	//string Test = "Code=2 CIMG=121000196 CName=BS-ClaN_아웰 CPoint=591260 CRegistDay=2004/05/04 CIMG=103001219 CName=별난온달 CPoint=546943 CRegistDay=2004/05/04 CIMG=104000979 CName=[NEO]오메가 CPoint=479030 CRegistDay=2004/05/05 CIMG=112000075 CName=도깨비 CPoint=454562 CRegistDay=2004/05/04 CIMG=115001132 CName=-아마게돈- CPoint=451679 CRegistDay=2004/05/04 CIMG=102001120 CName=[희진사랑] CPoint=438589 CRegistDay=2004/05/05 CIMG=109000660 CName=GladiaTor CPoint=364726 CRegistDay=2004/05/04 CIMG=118000957 CName=pUrplEviShop CPoint=357253 CRegistDay=2004/05/04 CIMG=111001179 CName=엽긔호러가족 CPoint=302324 CRegistDay=2004/05/04";
	//sinClanRank = Split_ClanRankDaTa(Test);

}

// Space를 키값으로 스트링을 분리한다
vector<string> cSINSOD2::Split_ClanRankDaTa(const string& s)
{

	vector<string> ret; 
	typedef string::size_type string_size;
	string_size i = 0;
	while(i != s.size()){ 
		while(i != s.size()){
			if(s[i] & 0x80)break; //한글이면 건너뛰자
			if(isspace(s[i])){
				++i;
			}
			else break;
		}
		string_size j =i;
		while(j != s.size()){
			if((j-i) > 200 ){
				i = s.size();
				j = i;
				break;
			}
			if(s[j] & 0x80){ //한글이면 인덱스 2칸을지나 다시체크 (0x80을 이진수로하면 128 ) 
				j +=2;       // 0000 0000 중에 뒷부분을 다채우고 옆으로 넘어가게되면 2Byte를 사용하는 한글이란뜻임
				continue;
			}
			if(!isspace(s[j])){
				++j;
			}
			else break;
		}
		if(i != j ){
			ret.push_back(s.substr(i,j-i));
			i = j;
		}
	}

	//Code별로 다시 파싱한다
	vector<string> ret2;
	string_size k = 0;
	string_size e = 0;
	string STempNum;
	string CodeTemp;
	int TempNumCnt = 0;

	//요기서 초기화
	for(int p = 0; p < 10 ; p++){
		ClanMarkNum[p] = -1;
	}

	i = 0; //앞부분에는 CODE가 있다
	while(i < ret.size()){ 
		while(k != ret[i].size()){
			if(ret[i][k] == '='){ 
				CodeTemp.clear();
				CodeTemp = ret[i].substr(0,k);
				if( i ==0 && CodeTemp == "Code"){
					STempNum.clear(); //이거안해주고 atoi하다가 뻑날때있다 어찌나 까다로운 string인지
					STempNum = ret[i].substr(k+1,ret[i].size()-(k+1));
					if(atoi(STempNum.c_str()) == 2){
						ret2.clear();
						return ret2;
					}
					else break;
				}
				
				ret2.push_back(ret[i].substr(k+1,ret[i].size()-(k+1)));
				if(CodeTemp == "CIMG"){
					STempNum.clear(); //이거안해주고 atoi하다가 뻑날때있다 어찌나 까다로운 string인지
					STempNum = ret[i].substr(k+1,ret[i].size()-(k+1));
					ClanMarkNum[TempNumCnt] = atoi(STempNum.c_str());
					ClanMarkIndex[TempNumCnt] = ReadClanInfo( ClanMarkNum[TempNumCnt] );
					TempNumCnt++;
				}
				k = 0;
				break;
			}
			k++;
		}
		i++;
	}

	//클랜마크를 로드한다
	for(int t = 0 ;t < TempNumCnt ; t++){ 
		if(ClanMarkIndex[t] >= 0){
			if(ClanInfo[ClanMarkIndex[t]].hClanMark){ //TempNumCnt이 한개라도 있어야 로드할이미지가있는거다
				ClanMark[t] = ClanInfo[ClanMarkIndex[t]].hClanMark;
			}
		}
	}
	return ret2;		
}
// 분할될 스트링의 길이를 기준으로 분할한다
vector<string> cSINSOD2::Split_ClanMessage(const string& s , const int Len[])
{

	vector<string> ret;  
	typedef string::size_type string_size; 
	string_size i = 0;
	string_size j = 0;
	int LenCnt = 0;
	while(i < s.size()){
		if(s[i] & 0x80)i += 2;  //한글
		else i++;
		if( (int)i-(int)j >= Len[LenCnt]){
			if(Len[LenCnt+1] == 0){
				ret.push_back(s.substr(j,i-j));
				break;
			}
			ret.push_back(s.substr(j,i-j));
			j = i;
			LenCnt++;
		}
		if(s[i] == '|'){
			ret.push_back(s.substr(j,i-j));
			break;
		}
	}

	/*
	int LenCnt = 0;  
	vector<string> ret; 
	typedef string::size_type string_size; 
	string_size i = 0;
	string_size j = 0;
	string_size k = 0;
	while(i < s.size()){ 
		if(s[i] & 0x80){
			i +=2;
			if(i == s.size()){
				ret.push_back(s.substr(j,i-j));
				break;
			}
			continue;
		}
		else{
			if( (int)i-(int)j >= Len[LenCnt]){
				if(isspace(s[i])){
					if(Len[LenCnt+1] == 0){
						i = s.size(); //짜투리 글씨는 다찍어준다
						ret.push_back(s.substr(j,i-j));
						break;
					}
					ret.push_back(s.substr(j,i-j));
					++i; 
					j = i;
					LenCnt++;
				}
			}
			++i;
			if(i == s.size()){
				ret.push_back(s.substr(j,i-j));
				break;
			}

		}
	}
	*/
	return ret;		
}

// 웹 DB에서 받은 데이타를 구분한다
vector<string> cSINSOD2::Split_Sod2DaTa(const string& s)
{
	string Temp33;
	vector<string> ret; 
	typedef string::size_type string_size;
	string_size i = 0;
	while(i < s.size()){ 
		while(i < s.size()){
			if(s[i] == '|'){
				++i;
			}
			else break;
		}
		string_size j = i;
		while(j < s.size()){
			if((j - i) > 200){
				i = s.size(); //while루프를 끝낸다
				j = i; 
				break;
			}
			if(s[j] != '|'){
				++j;
			}
			else break;
		}
		if(i != j ){
			Temp33 = s.substr(i,j-i);
			ret.push_back(s.substr(i,j-i));
			i = j;
		}
	}

	//Code별로 다시 파싱한다
	string Temp;
	string Temp2;
	string_size k = 0;
	string_size e = 0; 
	int NumTemp = 0;
	int TempArray[] = {28,22,26,0}; 
	i = 0; 
	while(i < ret.size()){
		while(k < ret[i].size()){
			if(ret[i][k] == '='){ 
				Temp.clear();
				Temp = ret[i].substr(0,k);

				Temp2.clear();
				Temp2 = ret[i].substr(k+1,ret[i].size()-(k+1));

				/*
				if(k+1 == ret[i].size()){
					if(i+1 != ret.size()){
						if(ret[i+1][0] != 'C'){  //또땜빵 -_- 바쁘다보니 할수없다
							Temp2.clear();
							Temp2 = ret[i+1].c_str();
						}
					}
				}
				*/
				
				if(Temp == "Code"){
					NumTemp = atoi(Temp2.c_str());
					switch(NumTemp){
						case 0:
						case 5:
						case 6:
							UserType = 1;
						break;
						case 1:
							UserType = 3;
						break;
						case 2:
						case 3:
							UserType = 2;
						break;
						case 4:
							UserType = 4;
						break;
					}
		
				}
				if(Temp == "CName"){
					lstrcpy(szClanName,Temp2.c_str());
				}
				if(Temp == "CNote"){
					//Temp2 ="한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글한글";
					Temp2.push_back('|');
					sinClanMessage = Split_ClanMessage(Temp2 , TempArray);
				}
				if(Temp == "CZang"){
					lstrcpy(szClanMaster,Temp2.c_str());

				}
				if(Temp == "CIMG"){
					ClanImageNum = atoi(Temp2.c_str());
					ClanMark_32Index = ReadClanInfo_32X32(ClanImageNum);
					ClanMark_32 = ClanInfo[ClanMark_32Index].hClanMark32;

				}
				if(Temp == "TotalEMoney"){
					TotalEMoney = atoi(Temp2.c_str());

				}
				if(Temp == "CTax"){
					Tax = atoi(Temp2.c_str());

				}
				if(Temp == "TotalMoney"){
					TotalMoney = atoi(Temp2.c_str());

				}
				if(Temp == "CClanMoney"){
					ClanMoney = atoi(Temp2.c_str());
				}
				k = 0;
				break;
			}
			k++;
		}
		i++;
	}

	return ret;		
}


//웹 DB에서 메세지를 받는다
int cSINSOD2::RecvWebData(int Index , const string& s)
{
	vector<string> Temp_V;   
	if(bip_port_error)return FALSE;   
	if(Index){     
		//Init(); 
		if(Index == 1){     
			Temp_V = Split_Sod2DaTa(s);  
			if(Temp_V.size() <= 0)return FALSE;
			BoxIndex = 1;      
			ShowSinMessageBox_New(62,78,381-62,426-78 ,SodButtonRect );
			//UserType = 2; //1 일반유저 , 2 클랜원 ,3 클랜마스터 ,4 돈찾을거 있는 클랜마스터
			//if(UserType == 4 || UserType == 3){
			//	SendClanMoneyToServer(0,0);
			//}
		}
		else if(Index == 3){
			//ClanRankFlag = 1;
			sinClanRank = Split_ClanRankDaTa(s);
			if(sinClanRank.size() <= 0)return FALSE;
			BoxIndex = 2;
			//ShowSinMessageBox_New(62,78,381-62,426-78 ,SodButtonRect );
		}
		//여기서 Clan 마스터인지 Clan 원인지 일반 유저인지를 가려서 메뉴를 보여준다
	}
	return TRUE;
}

//클랜칩이 금액을 찾는다
int sinRecvClanMoney(int RemainMoney , int GetMoney) { return TRUE; }

/*----------------------------------------------------------------------------*
*	
*							( 공 성 전 )
*	
*-----------------------------------------------------------------------------*/	
int sinShowSeigeMessageBox()
{
	//SeigeINFOindex(UserAccount, sinChar->szName,szConnServerName,1);
	return TRUE;
}

int RecvSeigeWebData(int Index , char *string)
{
	//char szTemp[65000];
	//lstrcpy(szTemp,string);

	return TRUE;
}





/*----------------------------------------------------------------------------*
*		 테스트				<ha>공성전 메뉴박스 
*-----------------------------------------------------------------------------*/	

//임시객체
cHASIEGE chaSiege;

/*---사용돼는 각종 플래그----*/
int haSiegeMenuFlag=0;    //공성전 메뉴플랙
int haSiegeMenuKind=0;    //공성전 메뉴종류
int ScrollButtonFlag=0;   //스크롤 사용시 필요한 플랙
int GraphLineFlag=0;    
int haSiegeBoardFlag=0;   //공성 클랜 점수창 플랙
int haSiegeMerFlag=0;     //용병 설정 플랙


/*---정보박스 관련 위치정보---*/
POINT ClanSkillBoxSize={0,0};  //클랜스킬 정보 박스 사이즈
POINT ClanSkillBoxPosi={0,0};  //클랜스킬 정보 박스 포지션

/*---수성설정 관련  인덱스들---*/
int CastleKindIndex  = 0;  //성의 종류 관련 인덱스
int TowerIconIndex = 0;  //타워 종류
int haSendTowerIndex = 0;
int MenuButtonIndex  = 0;  //메뉴 버튼 관련 인덱스

/*--저장돼고 세팅될때  사용될 임시함수들---*/
int HaTestMoney =0;        //임시세금  금액
int HaTaxRate =0;

/*----클랜 보드 설정-----*/
sHACLANDATA sHaClanData[HACLAN_MAX];  //임의 클랜 개수

int  haAlpha   = 0;				//보드의 알파값
int  BoardTime = 0; 
int  haClanSkillTime=0;

int  haTotalDamage =  0;		//토탈 데미지를 찍어줄때 사용한다.
int  haPlayTime[3] = {0};		//보드타임

/*---용병 설정 가격----*/
//임시
int haMercenrayMoney[3] = {50000,100000,300000};    //용병 가격
int	haMercenrayIndex = 0;                           //용병 설정 인덱스
int haTowerMoney =500000;

//크리스탈 카운트를 받는다.
short haCrystalTowerCount[4];    //크리스탈 카운트   

char *haC_CastleWinFilePath   = "image\\Sinimage\\help\\CastleWin.sin" ;
char *haC_CastleLoseFilePath  = "image\\Sinimage\\help\\CastleLose.sin";

char *haC_CastleWin_FilePath  = "image\\Sinimage\\help\\CastleWining.sin" ;
char *haC_CastleLose_FilePath  = "image\\Sinimage\\help\\CastleLoseing.sin";


/*---보드 클랜 이름위치-----*/
char *ScoreBoardName[] = {
	"Battle Point",    //플레이어 스킬 점수          
	"Con Rate",        //자신에 클랜 점수 기여도
	"B.P",
	"PlayTime",        //타임
	"Hit  Rate",	   //자기 클랜의 점수
	"BLESS CASTLE",    //문구
};

//버튼 위치
int SiegeButtonPosi[][4]={
	{70    ,70     ,268,302},    //메인
	{29+70 ,269+70 ,68 ,23},     //재정/방어 설정
	{144+70,269+70 ,48 ,23},     //확인
	{197+70,269+70 ,48 ,23},     //취소
	{270   ,236+70 ,48 ,23},     //돈찾기 확인
	{77+70 ,21+70  ,49 ,11},     //타워 설정버튼
	{179+70-3,21+70,49 ,11},   //용병 설정버튼
};

//공성전 아이콘 위치
int SiegeIconPosi[][4]={
	{26+70,83+70 ,16,16},        //스크롤
	{36+70,94+70 ,30,30},        //타워속성
	{36+70,216+70,30,30},        //클랜스킬 위치
	{8+70 ,45+70 ,51,22},        //성타입
	{10+70 ,63+70 ,12,13},       //타워 테두리
	{26+70,59+70,16,16},         //실제 세율 스크롤
};  
///동영상 플레이어
int  haMatPlayMenu[8]={0};
int  haMovieFlag = 0;
int  haMovieKind = 0;
char haMovieName[64];

int haPlayerPosi[4][4] = {
	{64+68+8,359,32,32},//상중하버튼
	{270    ,363,48,23},//
};

cHASIEGE::cHASIEGE()
{
	int i;
	for(i=0;i<6;i++){
		cSinSiege.TowerTypeDraw[i][1]=0;
	}
	for(i=0;i<4;i++){
		cSinSiege.MercenaryNum[i] =  0;
	}
	
}
cHASIEGE::~cHASIEGE()
{

}

void cHASIEGE::init()
{
	
	
	
}

void cHASIEGE::ImageLoad() {}

/*----------------------------------------------------------------------------*
*		              Release
*-----------------------------------------------------------------------------*/
void cHASIEGE::Release() {}
//폰트 x좌표


/*----------------------------------------------------------------------------*
*		              메인
*-----------------------------------------------------------------------------*/
void cHASIEGE::Main() {}
/*----------------------------------------------------------------------------*
*		              그림을 그리다.
*-----------------------------------------------------------------------------*/
int   haStartTga=0;                //보드 그림시 사용돼는 임시 변수들
int   haTempScore[2]={0};
int	  haStartPosiX=0,haStartPosiY=100;
void cHASIEGE::Draw() {}

/*----------------------------------------------------------------------------*
*		             서버에서 호출돼는 함수
*-----------------------------------------------------------------------------*/
//<ha>공성전 메뉴를 열어준다.
int cHASIEGE::ShowSiegeMenu(smTRANS_BLESSCASTLE *pData)
{	

	int i;
	//클랜칩 부클랜칩이 아닐경우 리턴
#ifdef _WINMODE_DEBUG
#else
	if(rsBlessCastle.dwMasterClan != GetClanCode(lpCurPlayer->smCharInfo.ClassClan))return TRUE;
#endif
	SendClanMoneyToServer(0,0,1);
	cSinSiege.ClanSkill = pData->ClanSkill;					  //클랜 스킬
	for(i=0;i<3;i++){
		cSinSiege.MercenaryNum[i] = pData->MercenaryNum[i];   //용병타입
	}
	for(i=0;i<6;i++){
		cSinSiege.TowerTypeDraw[i][0] = 0;
		cSinSiege.TowerTypeDraw[0][0] = 1;					  //내성 A를 활성화 시킨다.
		cSinSiege.TowerTypeDraw[i][1] = pData->Tower[i];      //성타입의 타워만 저장 
		
	}
 	ImageLoad();											  //이미지를 로드
	
	int Temp=0,Temp2=0;
	Temp  = (pData->TaxRate*22)+96;   
	Temp2 = (cSinSiege.TaxRate*22)+96-8;                      //현재 적용돼는 세율


	//현재의 세율을 세팅한다.
	SiegeIconPosi[0][0] = Temp-2;
	SiegeIconPosi[5][0] = Temp2-2;
	HaTaxRate = pData->TaxRate;

	
	//공성전 메뉴를 연다.
	haSiegeMenuFlag = 1;
	haSiegeMenuKind = HASIEGE_TAXRATES;//재정설정을 연다.
	
	return TRUE;
}

// MOSTRA PONTUA플O SOD
int cHASIEGE::ShowSiegeScore(rsUSER_LIST_TOP10 *pData)
{
	int i;

	for(i=0;i<HACLAN_MAX ;i++){
		 if(pData->dwUserCode[i] && pData->Damage[i]){
			
			sHaClanData[i].CLANCODE = pData->dwUserCode[i];
			sHaClanData[i].Score    = pData->Damage[i];
			haTotalDamage           = pData->dwTotalDamage;									  
			sHaClanData[i].Flag     = 1; 

			sHaClanData[i].ClanInfoNum = ReadClanInfo(sHaClanData[i].CLANCODE);
			if(sHaClanData[i].ClanInfoNum >=0){
				lstrcpy(sHaClanData[i].ClanName , ClanInfo[sHaClanData[i].ClanInfoNum].ClanInfoHeader.ClanName);
				sHaClanData[i].lpClanMark = ClanInfo[sHaClanData[i].ClanInfoNum].hClanMark;
			}
		 }
	}

	for(i=0;i<4;i++){
		haCrystalTowerCount[i] = pData->bCrystalTowerCount[i];
	}
	
	    
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						 <ha>공성전 종료 메세지를 보여준다.
*-----------------------------------------------------------------------------*/
int cHASIEGE::ShowExitMessage() { return TRUE; }
/*----------------------------------------------------------------------------*
*			<ha>공성전 보드  초기화 함수
*-----------------------------------------------------------------------------*/
int cHASIEGE::SetCastleInit()
{
	for(int i=0;i<3;i++){
		haPlayTime[i]=0;
	}
	for ( int i = 0; i<HACLAN_MAX; i++ )
	{
		sHaClanData[i].CLANCODE=0;
		sHaClanData[i].Flag=0;
		sHaClanData[i].lpClanMark=0;
		sHaClanData[i].Score=0;
		
		if(i<4){
			haCrystalTowerCount[i]=0;
		}
	}
	BoardTime = 60*30;
	
	return TRUE;
}
/*----------------------------------------------------------------------------*
*			       플레이 타임 표시
*-----------------------------------------------------------------------------*/
int cHASIEGE::ShowPlayTime(int StartTime)
{
	if(StartTime==0){
		SetCastleInit();
		return TRUE;
	}
	//플레이 타임을 세팅한다.
	haPlayTime[0] = StartTime%60;     //초
	haPlayTime[1] = StartTime/60;
	haPlayTime[1]-= StartTime/3600*60;
	haPlayTime[2] = StartTime/3600;   //시간

	//1시간 간격으로 초기화 해준다.
	//if(StartTime%60*10 == 0){
	//	SetCastleInit();
	//}

	
	if(rsBlessCastle.TimeSec[1] > 0 ){
		haSiegeBoardFlag = 1;//초기화 해준다.
		BoardTime = 0;
	}
	else{
		haSiegeBoardFlag = 0;
		haStartPosiX = 0;
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*
*			<ha>
*-----------------------------------------------------------------------------*/
int cHASIEGE::ShowPickUserKillPoint(int x,int y,int KillCount)
{	
	return TRUE;
}
/*----------------------------------------------------------------------------*
*						 클랜스킬 관련
*-----------------------------------------------------------------------------*/
//클랜스킬이 있으면 세팅한다.
int cHASIEGE::SetClanSkill(int ClanSkill) 
{
	DWORD SkillCode;
	
	haClanSkillTime = 0;
	//초기화 
	switch(ClanSkill){
		case SIN_CLANSKILL_ABSORB:
			SkillCode = CLANSKILL_ABSORB;
		break;
		case SIN_CLANSKILL_DAMAGE:
			//SkillCode = CLANSKILL_ABSORB;
			SkillCode = CLANSKILL_ATTACK;
		break;
		case SIN_CLANSKILL_EVASION:
			//SkillCode = CLANSKILL_ABSORB;
			SkillCode = CLANSKILL_EVASION;
		break;
	}
    
	if(rsBlessCastle.dwMasterClan == GetClanCode(lpCurPlayer->smCharInfo.ClassClan)){
		if(cSkill.SearchContiueSkillCODE(SkillCode)==SkillCode){
			return TRUE;
		}
		else{
			cSkill.CancelContinueSkill(CLANSKILL_ATTACK);
			cSkill.CancelContinueSkill(CLANSKILL_EVASION);
			cSkill.CancelContinueSkill(CLANSKILL_ABSORB);	
		}
	}
	else{
		cSkill.CancelContinueSkill(CLANSKILL_ATTACK);
		cSkill.CancelContinueSkill(CLANSKILL_EVASION);
		cSkill.CancelContinueSkill(CLANSKILL_ABSORB);
		return TRUE;
	}

	//클랜원이 맞으면 클랜스킬을 세팅한다.
	sSKILL haClanSkill;
    
	for(int j = 0 ; j < SIN_MAX_SKILL; j++){
		if(sSkill[j].CODE == SkillCode){
			memcpy(&haClanSkill,&sSkill[j],sizeof(sSKILL));
		    haClanSkill.UseTime=60;
			sinContinueSkillSet(&haClanSkill);
			break;
		}
	}
	cInvenTory.SetItemToChar();
	
	return TRUE;
}
/*----------------------------------------------------------------------------*
*				 haGoon 공성전 전용 아이템을 사용한다.
*-----------------------------------------------------------------------------*/	
int haCastleSkillUseFlag =0;
int cHASIEGE::SetCastleItemSkill(int ItemKind)
{
	DWORD CastleSkillCode;
	int   CastleSkillUseTime=0;

	switch(ItemKind){
		case SIN_CASTLEITEMSKILL_S_INVU:
			CastleSkillCode = SCROLL_INVULNERABILITY;
			CastleSkillUseTime = 30;
		break;
		case SIN_CASTLEITEMSKILL_S_CRITICAL:
			CastleSkillCode = SCROLL_CRITICAL;
			CastleSkillUseTime = 30;
		break;
		case SIN_CASTLEITEMSKILL_S_EVASION:
			CastleSkillCode = SCROLL_EVASION;
			CastleSkillUseTime = 30;
		break;
		case SIN_CASTLEITEMSKILL_S_P_LIFE:
			CastleSkillCode = 0;
		break;
		case SIN_CASTLEITEMSKILL_S_RES:
			CastleSkillCode = 0;
		break;
		case SIN_CASTLEITEMSKILL_R_FIRE_C:
			CastleSkillCode = STONE_R_FIRECRYTAL;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_R_ICE_C:
			CastleSkillCode = STONE_R_ICECRYTAL;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_R_LIGHTING_C:
			CastleSkillCode = STONE_R_LINGHTINGCRYTAL;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_FIGHTER:
			CastleSkillCode = STONE_A_FIGHTER;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_MECHANICIAN:
			CastleSkillCode = STONE_A_MECHANICIAN;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_PIKEMAN:
			CastleSkillCode = STONE_A_PIKEMAN;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_ARCHER:
			CastleSkillCode = STONE_A_ARCHER;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_KNIGHT:
			CastleSkillCode = STONE_A_KNIGHT;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_ATALANTA:
			CastleSkillCode = STONE_A_ATALANTA;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_MAGICIAN:
			CastleSkillCode = STONE_A_MAGICIAN;
			CastleSkillUseTime = 60;
		break;
		case SIN_CASTLEITEMSKILL_A_PRIESTESS:
			CastleSkillCode = STONE_A_PRIESTESS;
			CastleSkillUseTime = 60;
		break;
	}
	//해당 돼는 아이템이 맞으면 아이템스킬을 세팅한다.
	sSKILL haCastleSkill;

	haCastleSkillUseFlag =1;
    if(CastleSkillCode==0)return TRUE; //유지형 아이콘을 띄어주지 않는 것은 리턴 시켜준다.  

	for(int j = 0 ; j < SIN_MAX_SKILL; j++){
		if(sSkill[j].CODE == CastleSkillCode){
			memcpy(&haCastleSkill,&sSkill[j],sizeof(sSKILL));
			haCastleSkill.UseTime=CastleSkillUseTime;
			sinContinueSkillSet(&haCastleSkill);
			SwitchSkill(&haCastleSkill);
			break;
		}
	}
	
	return TRUE;
}

/*----------------------------------------------------------------------------*
*						서버로 공성메뉴 데이타를 보낸다
*-----------------------------------------------------------------------------*/	
int cHASIEGE::SendServerSiegeData()
{
	int i;

	smTRANS_BLESSCASTLE TempBlessCastle;
	//TempBlessCastle 서버에 보낸다
	TempBlessCastle.TaxRate        = HaTaxRate;							//세율
	TempBlessCastle.ClanSkill      = cSinSiege.ClanSkill;				//클랜스킬
	TempBlessCastle.Price          = cSinSiege.Price;
	for(i=0;i<3;i++){
		TempBlessCastle.MercenaryNum[i]  = cSinSiege.MercenaryNum[i];   //용병
	}
	for(i=0;i<6;i++){
		TempBlessCastle.Tower[i] = cSinSiege.TowerTypeDraw[i][1];  //성타입의 타워만 저장 
	}

	SendBlessCastleToServer(&TempBlessCastle,0);//서버로 보내다.
	SaveGameData();
	return TRUE;
}


/*----------------------------------------------------------------------------*
*				메뉴 초기화를 해준다.
*-----------------------------------------------------------------------------*/
int cHASIEGE::SetCastleMenuInit()
{
	haSiegeMenuFlag=0;			 //메뉴를 닫아준다.
	haSiegeMenuKind=0;			 //메뉴종류를 초기화 해준다.
	haSiegeMerFlag=0;			 //용병 플랙을 초기화 해준다.
	SiegeIconPosi[0][0]=70+20-7; //스크롤은 다시 배치
    SiegeIconPosi[5][0]=70+20-7; //스크롤은 다시 배치

	return TRUE;	
}
/*----------------------------------------------------------------------------*
*						 키다운
*-----------------------------------------------------------------------------*/
void cHASIEGE::KeyDown()
{
	
}
/*----------------------------------------------------------------------------*
*						 LButtonDown/UP
*-----------------------------------------------------------------------------*/
void cHASIEGE::LButtonDown(int x, int y) {}
void cHASIEGE::LButtonUp(int x, int y) {}
/*----------------------------------------------------------------------------*
*						DrawText
*-----------------------------------------------------------------------------*/
void cHASIEGE::DrawText() {}

/*----------------------------------------------------------------------------*
*						 Class cSINSIEGE
*-----------------------------------------------------------------------------*/	
int cSINSIEGE::GetTaxRate()
{	
	return TaxRate;
}
int cSINSIEGE::SetTaxRate(int TempTaxRate)
{
	TaxRate = TempTaxRate;
	return TRUE;
}

int cSINSIEGE::GetTotalTax()
{
	//서버에 클랜머니금액 정보를 요구한다
	//TotalTax = getSiegeClanMoneyToServer(0,0); 대략
	return TotalTax;
}
int cSINSIEGE::GetTaxMoney(int Money )
{
	//찾고싶은 만큼의 돈을 요구한다
	return TRUE;
}

/*----------------------------------------------------------------------------*
*					공성전 동영상 플레이를 보여준다.
*-----------------------------------------------------------------------------*/	
int cHASIEGE::ShowCastlePlayMovie(char *szPath,char *TitleName,int Option)
{

	memset(haMovieName ,0,sizeof(haMovieName));
	lstrcpy(haMovieName,TitleName);

	haMovieFlag = 1;  //플랙을 열어준다.

	if(haMovieFlag){ 
		Play_ParkPlayer( szPath ,42,100, 307,260 ,150);
	}
	return TRUE;
}





