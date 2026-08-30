/*----------------------------------------------------------------------------*
*	파일명 :  sinSkill.cpp
*	하는일 :  아이템 관리 
*	작성일 :  최종업데이트 12월 
*	적성자 :  박상열 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"
#include "Character/skillsub.h"
#include "field.h"
#include "Character/playmain.h"
#include "Character/playsub.h"
#include "netplay.h"
#include "Character/character.h"
#include "fileread.h"

// 박재원 - 부스터 아이템 사용장소를 설정하기 위해
/*----------------------------------------------------------------------------*
*								전역변수 			
*-----------------------------------------------------------------------------*/	

cSKILL	cSkill;
sUSESKILL sinSkill;
sSKILL  *pUseSkill;			//사용하기전에 값을 셋팅해준다 
sSKILL  ContinueSkill[20];	//지속적으로 사용되는 스킬 
sSKILL  TempContinueSkill[20];	//지속적으로 사용되는 스킬 
int SkillCountTime2[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int ContinueSkillMouseIndex = 0;

JOB_DATA_CODE *sinJobList;

sSKILL *pMasterSkill;  //스킬 습득 포인터 

sSKILLPASSIVE sSkillPassive; //캐릭터에 저장되는 패시브 스킬 

/////////////
int ShowSkillUpInfo = 0;     //메세지 박스와 함께 스킬정보를 보여준다 
int  SkillMasterOkFlag[2] = {0,0};
int  SkillMasterFlag = 0;    //NPC한테 스킬 을 배우고자 한다 

int SkillMasteryShowFlag = 0;		//숙련도를 표시한다 
char szUseSkillMastery[32];			//스킬 숙련도 
POINT UseSkillMasteryPosi = {0,0};	//숙련도 표시 포지션 

int MatForceOrb[12] = {0};

// 박재원 - 빌링 매직 포스
int MatBillingMagicForceOrb[3] = {0};

// 박재원 - 부스터 아이템
int Mat_BoosterItem[9] = {0};

// 장별 - 스킬 딜레이
int nMat_SkillDelayItem[3] = {0};

int MatQuestTimer = 0;

/////////////스킬 정보  보여주기 

POINT SkillUpInfo = { 13,21};       //스킬 정보 보여주기 
POINT SkillUpInfoPosi= { 90,100};

////////////
int     SkillInfoLineCnt = 0;

int     SkillInfoShowFlag = 0;
POINT	SkillInfoBoxPosi  = {0,0};
POINT   SkillInfoTextPosi = {0,0};
POINT   SkillBoxSize = {0,0};


char    szSkillInfoBuff[5000];
char    szSkillMasterInfoBuff[5000];

char    szSkillInfoBuff2[5000];
char    szSkillMasterInfoBuff2[5000];


int     SkillDocLineCnt[6] = {0,0,0,0,0,0};
int     SkillUseWeaponClass = 0;
int     NextSkillLevelLine = 0;
/////////////

int ShowSkillClose = 0; //닫기 버튼 플랙 
int SkillButtonIndex = 0;
int Skill4ButtonIndex = 0;

int DownButtonUseShorKey = 0 ; //단축키를 설정했을때는 스킬을 정션하지 않는다 

int DownButtonIndex  = 0;  //버튼을 누르는 효과를 주기위한 인덱스 
int LDownButtonIndex = 0;  //왼쪽 버튼 
int RDownButtonIndex = 0;  //오른쪽 버튼 



int ChangeJobSkillPlus = 0;
int UseSkillIndex[2] = {0,0}; //왼쪽 오른쪽 

int SelectInterSkill = 0;

/////////////////미니 Define
#define	NORMAL_SKILL_INDEX		14
#define MAX_USE_SKILL_POINT		10

#define SIN_MOUSE_POSI_LEFT		1
#define SIN_MOUSE_POSI_RIGHT	2

/////////////////

///////////////// 스킬 아이콘 
int   SkillIconIndex[10] = {0,0,0,0,0,0,0,0,0,0};
POINT SkillIconPosi = {0,0};
int   SkillInconCnt = 0;


int GageLenght[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

DWORD ClearSkillCODE = 0; //초기화될 스킬이 있나없나 
POINT ClearSkillButtonPosi = {0,0}; //스킬 초기화 버튼 위치 

int SetT_Of_ValhallaFlag = 0;
int SetT_Of_ValhallaFlag2 = 0; //순서때문에 플렉하나를 더둔다
int SetT_Of_ValhallaLV = 0;    //시전자의 레벨

int SetH_Of_ValhallaLV = 0;    //시전자의 레벨
int SetT_Of_ValhallaPOINT = 0; //시전자의 트라이엄프 오브 발할라포인트
 
int MatCircleIcon = 0;

sSKILL_INFO sSkill_Info[SIN_MAX_SKILL] = {

#include "sinSkill_Info.h"

};

//스킬 세부사항 
sSKILL sSkill[SIN_MAX_SKILL] = {
	
	//스킬 이름            스킬 코드          요구레벨   버튼 파일네임 
	{"노멀어택 "		,SKILL_NORMAL_ATTACK	,"Skill_Normal",},	 		

	//////////////메카닉 계열
	{"익스트림쉴드"		,SKILL_EXTREME_SHIELD	,"TM10 Ex_shield","Mc1_E_SHIELD.tga"},	 	
	{"메카닉범"			,SKILL_MECHANIC_BOMB	,"TM12 M_bomb"},
	{"포이즌 어트리뷰트",SKILL_POISON_ATTRIBUTE ,"TM14 P_attribute"},
	{"피지컬 업솝"		,SKILL_PHYSICAL_ABSORB	,"TM17 Ph_absorb","Mc1_P_ABSORB.tga"},

	{"라운드 임펙트"	,SKILL_GREAT_SMASH		,"TM20 G_Smash"},
	{"맥시마이즈 "		,SKILL_MAXIMIZE			,"TM23 Maximize","Mc2_MAXIMIZE.tga"},
	{"오토메이션"		,SKILL_AUTOMATION		,"TM26 Automation","Mc2_AUTOMATION.tga"},
	{"스파크"			,SKILL_SPARK			,"TM30 Spark"},

	{"메탈 아머"		,SKILL_METAL_ARMOR		,"TM40 M_Armor","Mc3_M_ARMOR.tga"},
	{"그랜드 스메쉬"	,SKILL_GRAND_SMASH		,"TM43 Grand_Smash"},
	{"메카닉 웨펀"		,SKILL_MECHANIC_WEAPON	,"TM44 M_Weapon"},
	{"스파트 쉴드"		,SKILL_SPARK_SHIELD		,"TM50 S_Shield","Mc3_S_SHIELD.tga"},

	//4차 스킬
	{"임플레이션"		,SKILL_IMPULSION		,"TM60 Impulsion"},
	{"컴플레이션"		,SKILL_COMPULSION		,"TM63 Compulsion","Mc4_COMPULSION.tga"},
	{"메네틱 피어"		,SKILL_MAGNETIC_SPHERE	,"TM66 M_Sphere","Mc4_M_SPHERE.tga"},
	{"메탈 골렘"		,SKILL_METAL_GOLEM		,"TM70 M_Golem","Mc4_M_GOLEM.tga"},

	////////////// 파이터 계열
	{"배틀 마스터리"	,SKILL_MELEE_MASTERY	,"TF10 M_mastery"},
	{"파이터 어트리뷰트",SKILL_FIRE_ATTRIBUTE	,"TF12 F_attribute"},
	{"레이빙"			,SKILL_RAVING			,"TF14 raving"},
	{"임팩트"			,SKILL_IMPACT			,"TF17 impact"},

	{"트리플 임팩트"	,SKILL_TRIPLE_IMPACT	,"TF20 T_Impact"},
	{"부루틀 스윙"		,SKILL_BRUTAL_SWING		,"TF23 B_Swing"},
	{"로어"				,SKILL_ROAR				,"TF26 Roar"},
	{"레이지오브 제크람",SKILL_RAGE_OF_ZECRAM	,"TF30 R_Zecram"},

	{"컨센트레이션"		,SKILL_CONCENTRATION	,"TF40 Concentration","Ft3_CONCENTRATION.tga"},
	{"어벤징 크래쉬"	,SKILL_AVANGING_CRASH	,"TF43 A_Crash"},
	{"스위프엑스"		,SKILL_SWIFT_AXE		,"TF46 S_Axe","Ft3_S_AXE.tga"},
	{"본 크래쉬"		,SKILL_BONE_CRASH		,"TF50 B_Crash"},

	//4차 스킬
	{"디스토리어"		,SKILL_DETORYER		    ,"TF60 Destoryer"},
	{"버서커"		    ,SKILL_BERSERKER		,"TF63 Berserker","Ft4_BERSERKER.tga"},
	{"싸이클론 스파이크",SKILL_CYCLONE_STRIKE	,"TF66 C_Strike"},
	{"부스트 헬스"		,SKILL_BOOST_HEALTH		,"TF70 B_Health"},

	////////////// 파이크맨 계열 
	{"파이크윈드"   	,SKILL_PIKE_WIND		,"TP10 P_wind"},
	{"아이스 어트리뷰트",SKILL_ICE_ATTRIBUTE	,"TP12 I_attribute"},
	{"크리티컬 힛"		,SKILL_CRITICAL_HIT		,"TP14 Cri_hit"},
	{"점핑크래쉬"		,SKILL_JUMPING_CRASH	,"TP17 J_Crash"},

	{"그라운드파이크"	,SKILL_GROUND_PIKE		,"TP20 G_Pike"},
	{"토네이도"			,SKILL_TORNADO			,"TP23 Tornado"},
	{"웨펀디펜스마스터리",SKILL_WEAPONE_DEFENCE_MASTERY	,"TP26 W_D_Mastery"},
	{"익스펜션" 		,SKILL_EXPANSION		,"TP30 Expasion"},

	{"베놈 스피어"		,SKILL_VENOM_SPEAR		,"TP40 V_Spear"},
	{"블레이드오브블레이즈",SKILL_VANISH        ,"TP43 Vanish","Pk3_VANISH.tga"},
	{"샤프마스터리"		,SKILL_CRITICAL_MASTERY	,"TP46 C_Mastery"},
	{"체인랜스" 		,SKILL_CHAIN_LANCE		,"TP50 C_Lance"},

	//4차 스킬
	{"어쌔신 아이"		,SKILL_ASSASSIN_EYE		,"TP60 A_Eye","Pk4_A_EYE.tga"},
	{"차징 스트라이크"	,SKILL_CHARGING_STRIKE	,"TP63 C_Strike"},
	{"베이그"           ,SKILL_VAGUE	        ,"TP66 Vague","Pk4_VAGUE.tga"},
	{"새됴우 마스터"	,SKILL_SHADOW_MASTER	,"TP70 S_Master"},

	////////////// 아처 계열 
	{"스카웃 호크"		,SKILL_SCOUT_HAWK		,"TA10 S_hawk","Ac1_S_HAWK.tga"},
	{"슈팅 마스터리"	,SKILL_SHOOTING_MASTERY	,"TA12 S_mastery"},
	{"윈드 애로우"		,SKILL_WIND_ARROW		,"TA14 W_arrow"},
	{"퍼펙트 에임"		,SKILL_PERFECT_AIM		,"TA17 P_aim"},

	{"디온스 아이"		,SKILL_DIONS_EYE		,"TA20 D_Eye"},
	{"팔콘"				,SKILL_FALCON			,"TA23 Falcon","Ac2_FALCON.tga"},
	{"애로우 오브레이지",SKILL_ARROW_OF_RAGE	,"TA26 A_Rage"},
	{"아발란체"			,SKILL_AVALANCHE		,"TA30 Avalanchie"},

	{"엘레멘탈샷"		,SKILL_ELEMENTAL_SHOT	,"TA40 E_Shot"},
	{"골든팔콘"			,SKILL_GOLDEN_FALCON	,"TA43 G_Falcon","Ac3_G_FALCON.tga"},
	{"범 샷"			,SKILL_BOMB_SHOT		,"TA46 B_Shot"},
	{"퍼포레이션"		,SKILL_PERFORATION		,"TA50 Perforation"},

	//4차 스킬
	{"리콜 울버린"		  ,SKILL_RECALL_WOLVERIN	,"TA60 R_Wolverin","Ac4_R_WOLVERIN.tga"},
	{"이베이션 마스터리"  ,SKILL_EVASION_MASTERY	,"TA63 E_Mastery","Ac4_E_MASTERY.tga"},
	{"피닉스 샷"          ,SKILL_PHOENIX_SHOT	    ,"TA66 P_Shot"},
	{"포스 오브 내츄럴"	  ,SKILL_FORCE_OF_NATURE	,"TA70 F_O_Nature","Ac4_F_O_NATURE.tga"},


	///////////////// 나이트
	{"스워스블레스트"	,SKILL_SWORD_BLAST	    ,"MN10 S_Blast"},
	{"홀리 바디"		,SKILL_HOLY_BODY		,"MN12 H_Body","Kt1_H_BODY.tga"},
	{"피지컬트레이닝"	,SKILL_PHYSICAL_TRANING ,"MN14 P_Traning"},
	{"떠블 크래쉬"   	,SKILL_DOUBLE_CRASH     ,"MN17 D_Crash"},

	{"홀리 밸러"	    ,SKILL_HOLY_VALOR       ,"MN20 H_Valor","Kt2_H_VALOR.tga"},
	{"브랜디쉬"	        ,SKILL_BRANDISH         ,"MN23 Brandish"},
	{"피어싱"	        ,SKILL_PIERCING         ,"MN26 Piercing"},
	{"드레스틱 스피릿"	,SKILL_DRASTIC_SPIRIT   ,"MN30 D_Spirit","Kt2_D_SPIRIT.tga"},

	{"프레임 브랜디쉬"	,SKILL_SWORD_MASTERY   ,"MN40 S_Mastery"},
	{"디바인인홀레이션" ,SKILL_DIVINE_INHALATION,"MN43 D_Inhalation","Kt3_D_INHALATION.tga"},
	{"홀리인캔테이션"	,SKILL_HOLY_INCANTATION ,"MN46 H_Incantation"},
	{"그랜드 크로스"	,SKILL_GRAND_CROSS		,"MN50 G_Cross"},

	//4차 스킬
	{"스워드 오브 저스틱스"	,SKILL_SWORD_OF_JUSTICE	,"MN60 S_O_Justice"},
	{"갓들리 쉴드"			,SKILL_GODLY_SHIELD		,"MN63 G_Shield","Kt4_G_SHIELD.tga"},
	{"갓 블레스"            ,SKILL_GOD_BLESS	    ,"MN66 G_Bless","Kt4_G_BLESS.tga"},
	{"디바인 피어싱"		,SKILL_DIVINE_PIERCING	,"MN70 D_Piercing"},
	


	///////////////// 아틀란타
	{"쉴드 스트라이크"	,SKILL_SHIELD_STRIKE	,"MA10 S_Strike"},
	{"파리나 "			,SKILL_FARINA			,"MA12 Farina"},
	{"드로잉마스터리"	,SKILL_THROWING_MASTERY ,"MA14 T_Mastery"},
	{"비거 스피어"	    ,SKILL_VIGOR_SPEAR      ,"MA17 V_Spear"},

	{"윈디"	            ,SKILL_WINDY            ,"MA20 Windy","At2_WINDY.tga"},
	{"트위스트 재블린"	,SKILL_TWIST_JAVELIN    ,"MA23 T_Javelin"},
	{"소울 썩커"	    ,SKILL_SOUL_SUCKER      ,"MA26 S_Sucker"},
	{"파이어 재블린"	,SKILL_FIRE_JAVELIN     ,"MA30 F_Javelin"},

	{"스플릿 자벨린"	,SKILL_SPLIT_JAVELIN         ,"MA40 Split_Javelin"},
	{"트라이엄프오브발할라",SKILL_TRIUMPH_OF_VALHALLA,"MA43 T_Of_Valhalla","At3_T_O_VALHALLA.tga"},
	{"라이트닝재블린"	,SKILL_LIGHTNING_JAVELIN     ,"MA46 L_Javelin"},
	{"스톰 재블린"		,SKILL_STORM_JAVELIN         ,"MA50 Storm_Javelin"},

	//4차 스킬
	{"헬 오브 발할라"		,SKILL_HALL_OF_VALHALLA ,"MA60 H_O_Valhalla","At4_H_O_VALHALLA.tga"},
	{"엑스 라지"			,SKILL_X_RAGE		    ,"MA63 X_Rage"},
	{"프로스트 재블린"      ,SKILL_FROST_JAVELIN	,"MA66 F_Javelin","At4_F_JAVELIN.tga"},
	{"벤첸"					,SKILL_VENGEANCE		,"MA70 Vengeance"},


	///////////////// 프리스티스
	{"힐링"				,SKILL_HEALING			,"MP10 Healing"},
	{"홀리볼트 "		,SKILL_HOLY_BOLT		,"MP12 HolyBolt"},
	{"멀티스파크"		,SKILL_MULTISPARK		,"MP14 M_Spark"},
	{"홀리마인드"	    ,SKILL_HOLY_MIND	    ,"MP17 HolyMind"},

	{"메디테이션"	    ,SKILL_MEDITATION	    ,"MP20 Meditation"},
	{"디바인라이트닝"	,SKILL_DIVINE_LIGHTNING ,"MP23 D_Lightning"},
	{"홀리 리플렉션"	,SKILL_HOLY_REFLECTION	,"MP26 H_Reflection","Pr2_H_REFLECTION.tga"},
	{"그레이트 힐링"	,SKILL_GRAND_HEALING	,"MP30 G_Healing"},

	{"비거 볼"		    ,SKILL_VIGOR_BALL	    ,"MP40 V_Ball"},
	{"리저렉션"			,SKILL_RESURRECTION		,"MP43 Resurrection"},
	{"익스팅션"			,SKILL_EXTINCTION		,"MP46 Extinction"},
	{"버츄얼 라이프"	,SKILL_VIRTUAL_LIFE		,"MP50 V_Life","Pr3_V_LIFE.tga"},

	//4차 스킬
	{"글래셔 스파이크"	 ,SKILL_GLACIAL_SPIKE	  ,"MP60 G_Spike"},
	{"리제너레이션 필드" ,SKILL_REGENERATION_FIELD,"MP63 R_Field","Pr4_R_FIELD.tga"},
	{"체인 라이트닝"     ,SKILL_CHAIN_LIGHTNING	  ,"MP66 C_Lightning"},
	{"서몬 뮤스펠"		 ,SKILL_SUMMON_MUSPELL	  ,"MP70 S_Muspell","Pr4_S_MUSPELL.tga"},



	///////////////// 메지션
	{"어고니"			,SKILL_AGONY			,"MM10 Agony"},
	{"파이어볼트 "		,SKILL_FIRE_BOLT		,"MM12 FireBolt"},
	{"제니쓰"			,SKILL_ZENITH			,"MM14 Zenith","Mg1_ZENITH.tga"},
	{"파이어볼"			,SKILL_FIRE_BALL	    ,"MM17 FireBall"},

	{"맨탈 마스터리"	,SKILL_MENTAL_MASTERY   ,"MM20 M_Mastery"},
	{"콜룸오브워터"		,SKILL_WATORNADO	    ,"MM23 Watornado"},
	{"인챈트 웨폰"		,SKILL_ENCHANT_WEAPON	,"MM26 E_Weapon","Mg2_E_WEAPON.tga"},
	{"데드레이"			,SKILL_DEAD_RAY  	    ,"MM30 D_Ray"},

	{"에너지 쉴드"		,SKILL_ENERGY_SHIELD    ,"MM40 E_Shield","Mg3_E_SHIELD.tga"},
	{"다이아스트로피즘"	,SKILL_DIASTROPHISM	    ,"MM43 Diastrophism"},
	{"스피릿 엘레멘탈"	,SKILL_SPIRIT_ELEMENTAL	,"MM46 S_Elemental","Mg3_S_ELEMENTAL.tga"},
	{"댄싱스워드"		,SKILL_DANCING_SWORD  	,"MM50 D_Sword","Mg3_D_SWORD.tga"},

	//4차 스킬
	{"파이어 엘레멘탈"	,SKILL_FIRE_ELEMENTAL	,"MM60 F_Elemental","Mg4_F_ELEMENTAL.tga"},
	{"플라임 웨이브"    ,SKILL_FLAME_WAVE		,"MM63 F_Wave"},
	{"디스트록션"       ,SKILL_DISTORTION	    ,"MM66 Distortion","Mg4_DISTORTION.tga"},
	{"메테오"		    ,SKILL_M_METEO		    ,"MM70 Meteo"},
		
	//공성전 클랜 스킬
	{"흡수력 상승"	    ,CLANSKILL_ABSORB	 ,"","wa1-1.tga"},
	{"공격력 상승"      ,CLANSKILL_ATTACK	 ,"","wa1-3.tga"},
	{"회피율 상승"      ,CLANSKILL_EVASION	 ,"","wa1-2.tga"},

	//공성전 아이템 스킬
	{"아타나시아"			 ,SCROLL_INVULNERABILITY	 ,"","S_Ath.tga"},
	{"데들리 에지"			 ,SCROLL_CRITICAL            ,"","S_Dea.tga"},
	{"어베일 오브 이베이드"	 ,SCROLL_EVASION	         ,"","S_Eva.tga"},
	{"디파이언스 스톤"		 ,STONE_R_FIRECRYTAL	     ,"","F_C_M.tga"},
	{"디파이언스 스톤"		 ,STONE_R_ICECRYTAL	         ,"","I_C_M.tga"},
	{"디파이언스 스톤"	     ,STONE_R_LINGHTINGCRYTAL    ,"","L_C_M.tga"},
	{"마이트 스톤"			 ,STONE_A_FIGHTER		     ,"","Ft_R.tga"},
	{"마이트 스톤"			 ,STONE_A_MECHANICIAN	     ,"","Mc_R.tga"},
	{"마이트 스톤"			 ,STONE_A_PIKEMAN		     ,"","Pk_R.tga"},
	{"마이트 스톤"	         ,STONE_A_ARCHER			 ,"","Ac_R.tga"},
	{"마이트 스톤"           ,STONE_A_KNIGHT			 ,"","Kt_R.tga"},
	{"마이트 스톤"           ,STONE_A_ATALANTA		     ,"","At_R.tga"},
	{"마이트 스톤"           ,STONE_A_MAGICIAN		     ,"","Mg_R.tga"},
	{"마이트 스톤"           ,STONE_A_PRIESTESS		     ,"","Pt_R.tga"},

	{"이터널 라이프"	     ,SCROLL_P_INVULNERABILITY	 ,"","S_Ath.tga"},
	{"페이틀 에지"			 ,SCROLL_P_CRITICAL          ,"","S_Dea.tga"},
	{"어버트 스크롤"	     ,SCROLL_P_EVASION	         ,"","S_Eva.tga"},

	// 박재원 - 부스터 아이템(생명력, 기력, 근력)
	{"생명력 부스터",		BOOSTER_ITEM_LIFE,			"",		"Booster_01.tga"},
	{"기력 부스터",			BOOSTER_ITEM_MANA,			"",		"Booster_02.tga"},
	{"근력 부스터",			BOOSTER_ITEM_STAMINA,		"",		"Booster_03.tga"},

	// 장별 - 스킬 딜레이
	{"스킬 딜레이",			SKILLDELAY_ITEM_LIFE,			"",		"Booster_01.tga"},

};


//스킬 창 박스 위치 정보 
sSKILLBOX	sSkillBox[SIN_MAX_SKILLBOX] = {
	{{142,537,142+49,537+46}},							//노말 

	{{27-10,502,27+49-10,502+46},{27+47-10,502+5,27+47+5-10,502+40}},		//1단계
	{{67-10,546,67+49-10,546+46},{67+47-10,546+5,67+47+5-10,546+40}},
	{{77-10,466,77+49-10,466+46},{77+47-10,466+5,77+47+5-10,466+40}},
	{{137-10,476,137+49-10,476+46},{137+47-10,476+5,137+47+5-10,476+40}},

	{{212-15,466,212+49-15,466+46},{212+47-15,466+5,212+47+5-15,466+40}},	//2단계
	{{232-15,516,232+49-15,516+46},{232+47-15,516+5,232+47+5-15,516+40}},
	{{272-15,466,272+49-15,466+46},{272+47-15,466+5,272+47+5-15,466+40}},
	{{332,466,332+49,466+46},{332+47,466+5,332+47+5,466+40}},

	{{402,476,402+49, 476+46},{402+47,476+5,402+47+5,476+40}},	//3단계
	{{472,456,472+49, 456+46},{472+47,456+5,472+47+5,456+40}},
	{{512,506,512+49, 506+46},{512+47,506+5,512+47+5,506+40}},
	{{532,456,532+49, 456+46},{532+47,456+5,532+47+5,456+40}},

	{{602,481,602+49, 481+46},{602+47,481+5,602+47+5,481+40}},	//4단계
	{{662,511,662+49, 511+46},{662+47,511+5,662+47+5,511+40}},
	{{681,461,681+49, 461+46},{681+47,461+5,681+47+5,461+40}},
	{{722,511,722+49, 511+46},{722+47,511+5,722+47+5,511+40}},


};

sSKILLBOX sLeftRightSkill[2] = {
	{{349,541,349+49,541+46},{349,558,349+5,558+35}},  //스킬 왼쪽 
	{{401+2,541,401+49,541+46},{446,558,446+5,558+35}},  //스킬 오른쪽 

};

int SkillTitle[4][2] = {
	{85,438-4},
	{280,438-4},
	{470,438-9},
	{675,438-4},

};
//스킬 메인=======================================
void*	lpSkillMain;
void*	lpSkillMain2; //스킬 메인2
//스킬 메인버튼
void*    lpSkillMainLeftB;
void*    lpSkillMainRightB;

int SkillMain2OpenFlag   = 0;        //스킬 메인 flag
int SkillMain2ButtonFlag[2] = {0,0};
//================================================


//4차 스킬 마스터flag
int Skill4MasterFlag=0;




int MonsterDamageLine = 0;
int ChainDamageLine   = 0;
int MatSkillIconBackGround   = 0;
/*----------------------------------------------------------------------------*
*							클래스 초기, 종료 
*-----------------------------------------------------------------------------*/	
cSKILL::cSKILL()
{
	memset(&ContinueSkill,0,sizeof(sSKILL)*10); //지속 사용스킬 초기화 

}
cSKILL::~cSKILL()
{

}
/*----------------------------------------------------------------------------*
*							     초기화 
*-----------------------------------------------------------------------------*/	
void cSKILL::Init() {}
/*----------------------------------------------------------------------------*
*						    Bmp파일을 로드한다 
*-----------------------------------------------------------------------------*/	
void cSKILL::Load() {}

//스킬이미지를 로드한다 
void cSKILL::LoadUseSkillImage() {}

/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cSKILL::Release() {}
/*----------------------------------------------------------------------------*
*							     그리기  
*-----------------------------------------------------------------------------*/	
int CheckVirtualLifeTime = 0;
int CheckBoosterLifeTime = 0; // 박재원 - 부스터 아이템(생명력)
int CheckVirtualManaTime = 0; // 박재원 - 부스터 아이템(기력)
int CheckVirtualStaminaTime = 0; // 박재원 - 부스터 아이템(근력)
int CheckSkillDelayTime = 0; // 장별 - 스킬 딜레이
void cSKILL::DrawUp() {}
void cSKILL::Draw() {}
/*----------------------------------------------------------------------------*
*							     메인  
*-----------------------------------------------------------------------------*/	
void cSKILL::Main() {}
/*----------------------------------------------------------------------------*
*							     종료 
*-----------------------------------------------------------------------------*/	
void cSKILL::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cSKILL::LButtonDown(int x , int y){}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cSKILL::LButtonUp(int x, int y) {}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cSKILL::RButtonDown(int x, int y) {}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cSKILL::RButtonUp(int x, int y) {}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	
void cSKILL::KeyDown() {}


//사용할 스킬을 찾는다 
int cSKILL::SearchUseSkill()
{

	switch(sinChar->JOB_CODE){  
		case 1:
			UseSkillBox = GROUP_FIGHTER;

		break;
		case 2:
			UseSkillBox = GROUP_MECHANICIAN;

		break;
		case 3:
			UseSkillBox = GROUP_ARCHER;

		break;
		case 4:
			UseSkillBox = GROUP_PIKEMAN;

		break;
		case 5:
			UseSkillBox = GROUP_ATALANTA;
		break;
		case 6:
			UseSkillBox = GROUP_KNIGHT;
		break;
		case 7:
			UseSkillBox = GROUP_MAGICIAN;
		break;
		case 8:
			UseSkillBox = GROUP_PRIESTESS;
		break;

	}
	if(!UseSkillBox)
		return FALSE;

	int Index = 0;
	memcpy(&sinSkill.UseSkill[0],&sSkill[0],sizeof(sSKILL));
	sinSkill.UseSkill[0].Flag = 1; //노말 어택 default설정 
	sinSkill.UseSkill[0].Use  = 1; //노말 어택 default설정 
	sinSkill.UseSkill[0].Skill_Info.USECODE = SIN_SKILL_USE_ALL;  //노말 어택 강제설정 
	sinSkill.UseSkill[0].Point = 1;

	for(int i=0;i<SIN_MAX_SKILL;i++){
		if(	(sSkill[i].CODE & sinSKILL_MASK1) == UseSkillBox ){ //자신에 맞는 스킬을 찾는다 
			for(int j=1;j < SIN_MAX_USE_SKILL; j++){
				if(!sinSkill.UseSkill[j].Flag){
					memcpy(&sinSkill.UseSkill[j],&sSkill[i],sizeof(sSKILL));
					for(int k=0; k < SIN_MAX_SKILL ; k++){
						if(sinSkill.UseSkill[j].CODE == sSkill_Info[k].CODE){
							memcpy(&sinSkill.UseSkill[j].Skill_Info,&sSkill_Info[k],sizeof(sSKILL_INFO));
							sinSkill.UseSkill[j].Skill_Info.SkillNum = j-1;
							break;
						}
					}
					sinSkill.UseSkill[j].Flag = 1;
					
					////////////TEST 를 위해서 포인트1을 준다 
					//sinSkill.UseSkill[j].Point = 1;

					break;
				}
			}
		}
	}
	memcpy(&sinSkill.SkillBox,&sSkillBox,sizeof(sSKILLBOX)*SIN_MAX_USE_SKILL); //좌표를 복사한다 

	LoadUseSkillImage(); //스킬이미지를 로드한다 (전업을 할때두 로드해준다 )

	sinSkill.UseSkill[0].Use  = 1; //노말 어택 default설정 
	sinSkill.UseSkill[0].Skill_Info.USECODE = SIN_SKILL_USE_ALL;  //노말 어택 강제설정 
	sinSkill.UseSkill[0].Point = 1;
	ReFormCharSkillInfo(); //스킬정보를 갱신한다 (해킹방지)
	return FALSE;
}

void cSKILL::DrawSkillText() {}

int JobSkillTemp = 0;
//현재 스킬의 상태를 검사한다 
void cSKILL::CheckingNowSkillState()
{

	//노말스킬을 포함한 4,4,4,4
	switch(sinChar->ChangeJob){
	case 0:
		ChangeJobSkillPlus = 5;
	break;
	case 1:
		ChangeJobSkillPlus = 9;
	break;
	case 2:
		ChangeJobSkillPlus = 13;
	break;
	case 3:
		ChangeJobSkillPlus = 17;
	break;

	}
	if(JobSkillTemp != ChangeJobSkillPlus){	//전업데이타가 나중에 들어와서 날치기 땜빵으로 보정했다  음하하 
		JobSkillTemp = ChangeJobSkillPlus;
		cInvenTory.SetItemToChar();
	}

	for(int j = 0 ; j < ChangeJobSkillPlus ; j ++){  
		if(j>12){
			if(sinSkill.SkillPoint4){ //4차스킬 포인트가 있을경우에만 활성화 시킨다 
				sinSkill.UseSkill[j].Flag = 1; //사용할수있는 스킬을 추가한다 
				if(sinSkill.UseSkill[j].Point <= MAX_USE_SKILL_POINT){ 
					if(sinSkill.UseSkill[j-1].Point){//트리를 탄다 
						if(sinSkill.UseSkill[j].Skill_Info.RequireLevel+(sinSkill.UseSkill[j].Point*2) <= sinChar->Level) //스킬을 사용할수있는 레벨인지 판다 
							sinSkill.UseSkill[j].Use = 1;
						else
							sinSkill.UseSkill[j].Use = 0;
					}
				}
			}
			else{ //없을경우는 다시 비활성화 
				sinSkill.UseSkill[j].Use = 0;
			}
			continue;
		}
		if(sinSkill.SkillPoint){ //스킬 포인트가 있을경우에만 활성화 시킨다 
			sinSkill.UseSkill[j].Flag = 1; //사용할수있는 스킬을 추가한다 
			if(sinSkill.UseSkill[j].Point <= MAX_USE_SKILL_POINT){
				if(j==0)continue; //노말 스킬 
				if(sinSkill.UseSkill[j-1].Point){//트리를 탄다 
					if(sinSkill.UseSkill[j].Skill_Info.RequireLevel+(sinSkill.UseSkill[j].Point*2) <= sinChar->Level) //스킬을 사용할수있는 레벨인지 판다 
						sinSkill.UseSkill[j].Use = 1;
					else
						sinSkill.UseSkill[j].Use = 0;
				}
			}
		}
		else{ //없을경우는 다시 비활성화 
				sinSkill.UseSkill[j].Use = 0;
		}
	}
}



//마스터리를 조정한다 
void cSKILL::CheckSkillMastery()
{
	
	int TempLenght = 0; 
	
	int TempTalent = 0;
	

	//양쪽 스킬들을 체크한다 
	if(sinSkill.pLeftSkill){
		if(!sinSkill.pLeftSkill->Point)
			sinSkill.pLeftSkill->UseSkillFlag = 0;
	}
	if(sinSkill.pRightSkill){
		if(!sinSkill.pRightSkill->Point)
			sinSkill.pRightSkill->UseSkillFlag = 0;
	}

	for(int j=1;j < SIN_MAX_USE_SKILL; j++){ 
		if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point ){

			CheckSkillMasteryForm(0,j); //스킬 마스터리를 체크한다

			if(sinSkill.UseSkill[j].Mastery == 0){
				sinSkill.UseSkill[j].GageLength = 0; //스킬 사용시
				sinSkill.UseSkill[j].GageLength2 = 0; //스킬 사용시
				SkillCountTime2[j] = 0;

			}

			//사용시 올라가는 숙련도 
			TempTalent = (int)(sinChar->Talent/3)+(int)sinAdd_fMagic_Mastery; 
			if(TempTalent > 50 )TempTalent = 50;
			sinSkill.UseSkill[j].UseSkillMastery =  (int)(TempTalent*100)+sinSkill.UseSkill[j].UseSkillCount;
			if(sinSkill.UseSkill[j].Skill_Info.Element[0]){ //4차스킬은 숙련치를 없애준다
				sinSkill.UseSkill[j].UseSkillMastery = 10000;

			}

			if(sinSkill.UseSkill[j].UseSkillMastery >= 10000)sinSkill.UseSkill[j].UseSkillMastery = 10000;
			if(sinSkill.UseSkill[j].UseSkillMastery){
				sinSkill.UseSkill[j].UseSkillMasteryGage = (int)((35*((float)sinSkill.UseSkill[j].UseSkillMastery/10000)));
				if(sinSkill.UseSkill[j].UseSkillMasteryGage > 35 )sinSkill.UseSkill[j].UseSkillMasteryGage = 35;
			}
			
			sinSkill.UseSkill[j].Mastery =   (sinSkill.UseSkill[j].Skill_Info.RequireMastery[0]+ (sinSkill.UseSkill[j].Skill_Info.RequireMastery[1]*sinSkill.UseSkill[j].Point) - 
											(sinSkill.UseSkill[j].UseSkillMastery/100));
			if(sinSkill.UseSkill[j].Mastery <= 0) sinSkill.UseSkill[j].Mastery = 1; //최소값 0.5초 
			if(sinSkill.UseSkill[j].Mastery > 70)sinSkill.UseSkill[j].Mastery = 70;

			// 장별 - 스킬 딜레이
			if( sinAddSkillDelay )
			{
				sinSkill.UseSkill[j].Mastery = 1;
			}

			else
			{
				sinSkill.UseSkill[j].Mastery =   (sinSkill.UseSkill[j].Skill_Info.RequireMastery[0]+ (sinSkill.UseSkill[j].Skill_Info.RequireMastery[1]*sinSkill.UseSkill[j].Point) - 
											(sinSkill.UseSkill[j].UseSkillMastery/100));

				if(sinSkill.UseSkill[j].Mastery <= 0) sinSkill.UseSkill[j].Mastery = 1; //최소값 0.5초 
				if(sinSkill.UseSkill[j].Mastery > 70)sinSkill.UseSkill[j].Mastery = 70;
			}

			//딜레이 값 
			TempLenght = (int)(35/((float)sinSkill.UseSkill[j].Mastery/2)); 

			sinSkill.UseSkill[j].GageLength2 += TempLenght;

			if(sinSkill.UseSkill[j].GageLength < sinSkill.UseSkill[j].GageLength2){
				SkillCountTime2[j]++;
				if(SkillCountTime2[j] >= (int)(35/(float)TempLenght)){
					sinSkill.UseSkill[j].GageLength++;
					SkillCountTime2[j] = 0;
			
				}

			}

			if(sinSkill.UseSkill[j].GageLength >=35){
				sinSkill.UseSkill[j].GageLength = 35;
				sinSkill.UseSkill[j].GageLength2 = 35;
				sinSkill.UseSkill[j].UseSkillFlag = 1;
				CheckUseSkillState(&sinSkill.UseSkill[j]); //스킬을 사용할수있는지를 체크한다 
				

			}
			else
				sinSkill.UseSkill[j].UseSkillFlag = 0;

			ReformSkillMasteryForm(0,j); //데이타를 갱신한다
				
		}
	
	}
	
	
}

DWORD UseSkillItemInfo[10] = {0,sinWA1,sinWM1,sinWH1,sinDS1,sinWP1,sinWS2,sinWC1,sinWS1,sinWT1};


int cSKILL::ShowSkillInfo(sSKILL* pSkill, sSKILLBOX* pSkillBox, int Icon) { return TRUE; }
    

int cSKILL::GetInfoBoxSize(sSKILLBOX *pSkillBox, int Col,int Icon) { return TRUE; }


//숙련도를 표시한다 
int cSKILL::GetSkillMastery(sSKILL *pSkill , sSKILLBOX *pSkillBox)
{
	int SkillMasteryTemp = 0;
	SkillMasteryTemp = ((int)pSkill->UseSkillMastery/100);
	wsprintf(szUseSkillMastery , "%d",SkillMasteryTemp);

	UseSkillMasteryPosi.x = pSkillBox->GageRect.left - (34/2);
	UseSkillMasteryPosi.y = pSkillBox->GageRect.top - 29;

	return TRUE;
}

//스킬을 사용할수있는지를 체크한다 // 장별-마나 리듀스 썼을 경우
int cSKILL::CheckUseSkillState(sSKILL *pSkill)
{
	if(pSkill->Skill_Info.UseMana)
	{
		// 장별 - 테리 속성
		// 장별 - 마나리듀스, 테리, 근력리듀스
		if( chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1 && chaPremiumitem.m_StaminaReducePotiontime > 0 )
		{
			if(sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - ((pSkill->Skill_Info.UseMana[pSkill->Point - 1] * 
				chaPremiumitem.m_ManaReducePotionValue / 100) + (pSkill->Skill_Info.UseMana[pSkill->Point-1] * 20 / 100)) && 
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))-( (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;
			}

			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 마나리듀스, 근력리듀스
		else if( chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_StaminaReducePotiontime > 0 )
		{
			if(sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) && 
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))-( (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 테리, 근력리듀스
		else if( chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1 && chaPremiumitem.m_StaminaReducePotiontime > 0 )
		{
			if(sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * 20 / 100) && 
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))-( (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 마나리듀스
		else if( chaPremiumitem.m_ManaReducePotiontime > 0)
		{
			if(sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) && 
				sinGetStamina() >= pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 테리
		else if( chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
		{
			if(sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * 20 / 100) && 
				sinGetStamina() >= pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 근력리듀스
		else if( chaPremiumitem.m_StaminaReducePotiontime > 0)
		{
			if(sinGetMana() >=  pSkill->Skill_Info.UseMana[pSkill->Point - 1] &&
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))-( (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;

			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 아이템 사용 안했다
		else 
		{
			if(sinGetMana() >=  pSkill->Skill_Info.UseMana[pSkill->Point - 1] &&
				sinGetStamina() >= pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1]*(pSkill->Point-1)))
			{
				pSkill->UseSkillFlag = 1;

			}
			else
				pSkill->UseSkillFlag = 0;
		}
	}
	return TRUE;
}


int SkillDataCheckDelayTime = 0;
int SkillDataCheckFlag2 = 0;


int cSKILL::CheckSkillData()
{
	SkillDataCheckDelayTime++;
	if (SkillDataCheckDelayTime < 70 * 10)return FALSE;
	SkillDataCheckDelayTime = 0;


	for (int j = 1; j < SIN_MAX_USE_SKILL; j++)
	{
		if (sinSkill.UseSkill[j].Flag)
		{

			if (!SkillDataCheckFlag2)
			{
				ReformSkillPointForm(&sinSkill.UseSkill[j]);
			}
			if (!CheckSkillPointForm(&sinSkill.UseSkill[j]))return FALSE;
			if (!CheckSkillMasteryForm(&sinSkill.UseSkill[j]))return FALSE;

		}
	}
	SkillDataCheckFlag2 = 1;


	return TRUE;
}


//스킬포인트 초기화하기 
int cSKILL::InitSkillPoint(int kind) { return TRUE; }

//DWORD ClearSkillPointCODE[10] = {SKILL_HOLY_BODY , SKILL_HOLY_MIND , 0,0,0,0,0,0,0,0};
DWORD ClearSkillPointCODE[10] = {0,0,0,0,0,0,0,0,0,0}; //부분 스킬 초기화 

int cSKILL::SearchClearSkillPoint(DWORD CODE)
{
	
	for(int i = 0 ; i < 10 ; i++){
		if(ClearSkillPointCODE[i] ==CODE){
			ClearSkillCODE = ClearSkillPointCODE[i];
			return TRUE;
		}
		else
			ClearSkillCODE = 0;
	}
	
	return FALSE;
	
}

////////////// 초기화할 스킬포인트에 초기화 버튼을 띄운다 
int cSKILL::ShowClearSkillPointButton(int x , int y) { return TRUE; }

////////////// 특정 스킬포인트를 초기화한다 
int cSKILL::ClearSkillPointSection( )
{

	CheckingNowSkillState(); //스킬포인트를 
	CheckCharForm();//인증 
	int Count = 0;
	for(int j = 1 ; j < ChangeJobSkillPlus ; j ++){ 
		if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point > 1){
			for(int i = 0 ; i < 10 ; i++){
				if(sinSkill.UseSkill[j].CODE== ClearSkillPointCODE[i]){ 
					cSkill.CheckSkillMasteryForm(0,j); // 스킬 마스터리를 체크한다
					CheckSkillPointForm(&sinSkill.UseSkill[j]);
					sinSkill.UseSkill[j].Point--; //스킬포인트 초기화 
					sinSkill.UseSkill[j].GageLength   = 0;
					sinSkill.UseSkill[j].GageLength2  = 0;
					sinSkill.UseSkill[j].Mastery      = 0;
					sinSkill.UseSkill[j].UseSKillIncreCount = 0;
					sinSkill.UseSkill[j].UseSkillCount = 0;
					sinSkill.UseSkill[j].UseSkillMastery = 0;
					sinSkill.UseSkill[j].UseSkillMasteryGage = 0;
					sinSkill.UseSkill[j].Use = 0;
					ReformSkillPointForm(&sinSkill.UseSkill[j]);
					ReformSkillMasteryForm(0,j);
					sinSkill.SkillPoint ++;
					ClearSkillCODE = 0;
					LearnSkillEffect(0);
					ReformCharForm();//재인증 
					return TRUE;
				}
			}		
		}
	}
	return TRUE;
}


unsigned int CharSkillInfoData[8] = {0,}; //캐릭정보 CheckSum이셋팅될 배열 
int   ChangeJob = -1;
//캐릭터별 SkillInfo를 갱신한다 
int cSKILL::ReFormCharSkillInfo()
{
	int   i = 0 ,j = 0, k = 0;
	int   CharNum = 0;
	
	int   CharCode[8] = {SIN_CHAR_MECANICIAN,SIN_CHAR_FIGHTER,SIN_CHAR_ARCHER,SIN_CHAR_PIKEMAN,SIN_CHAR_KNIGHT,SIN_CHAR_ATALANTA,SIN_CHAR_PRIESTESS,SIN_CHAR_MAGICIAN};
	//자신의 직업을 구한다 
	for(i = 0 ; i < 8 ; i++){
		if(sinChar->JOB_CODE == CharCode[i] ){
			CharNum = i;
			break;
		}
	}

	if(ChangeJob != sinChar->ChangeJob){
		ChangeJob = sinChar->ChangeJob;
		CharSkillInfoData[CharNum] = 0; //초기화 
		for(j=1;j < SIN_MAX_USE_SKILL; j++){
			if(sinSkill.UseSkill[j].Flag){
				k = j+1;
				CharSkillInfoData[CharNum] += k*sinSkill.UseSkill[j].Skill_Info.RequireLevel;
				CharSkillInfoData[CharNum] += k*sinSkill.UseSkill[j].Skill_Info.UseStamina[0];
				CharSkillInfoData[CharNum] += k*sinSkill.UseSkill[j].Skill_Info.UseStamina[1];
			}
		}
	}
	return TRUE;
}

//캐릭터별 SkillInfo를 체크한다
int cSKILL::CheckCharSkillInfo()
{
	//DWORD	dwXorKey = lpCurPlayer->dwObjectSerial&0x5F;
	int   i = 0 ,j = 0, k = 0;
	int   CharNum = 0;
	int   CharCode[8] = {SIN_CHAR_MECANICIAN,SIN_CHAR_FIGHTER,SIN_CHAR_ARCHER,SIN_CHAR_PIKEMAN,SIN_CHAR_KNIGHT,SIN_CHAR_ATALANTA,SIN_CHAR_PRIESTESS,SIN_CHAR_MAGICIAN};
	

//	unsigned int CharSkillInfoData[8] = {2549,2597,2246,2230,0,0,0,0};
	
	unsigned int CharSkillInfoDataCheckSum[8] ={0,};

	//자신의 직업을 구한다 
	for(i = 0 ; i < 8 ; i++){
		if(sinChar->JOB_CODE == CharCode[i] ){
			CharNum = i;
			break;
		}
	}

	// Corre豫o Mature

	for (j = 1; j < SIN_MAX_USE_SKILL; j++)
	{
		if (sinSkill.UseSkill[j].Flag)
		{
			k = j + 1;
			CharSkillInfoDataCheckSum[CharNum] += k * sinSkill.UseSkill[j].Skill_Info.RequireLevel;
			CharSkillInfoDataCheckSum[CharNum] += k * sinSkill.UseSkill[j].Skill_Info.UseStamina[0];
			CharSkillInfoDataCheckSum[CharNum] += k * sinSkill.UseSkill[j].Skill_Info.UseStamina[1];
			if (!sinSkill.UseSkill[j].UseSkillCount)continue;
			if (!CheckSkillMasteyData[j])
			{
				CheckSkillMasteyData[j] = sinSkill.UseSkill[j].UseSkillCount;
			}
			CheckSkillMasteyData[j] = sinSkill.UseSkill[j].UseSkillCount;
		}
	}
	if (CharSkillInfoDataCheckSum[CharNum] != CharSkillInfoData[CharNum])
	{

		SendSetHackUser3(51, CharSkillInfoDataCheckSum[CharNum], CharSkillInfoData[CharNum]);
		return FALSE;
	}


	return TRUE;
}

int cSKILL::CheckSkillMasteryForm( sSKILL *pSkill , int Index)
{
	if(pSkill){
		for(int j=1;j < SIN_MAX_USE_SKILL; j++){
			if(sinSkill.UseSkill[j].Flag){
				if(pSkill->CODE == sinSkill.UseSkill[j].CODE){
					Index = j;
					break;
				}
			}
		}
	}
	//if(!CheckMasteryData[Index])return FALSE; //데이타가 없을경우에는 넘어간다 


	int CheckMasteryDataSum = lpCurPlayer->dwObjectSerial+sinSkill.UseSkill[Index].CODE;

	CheckMasteryDataSum += (sinSkill.UseSkill[Index].GageLength*Index);
	CheckMasteryDataSum += (sinSkill.UseSkill[Index].GageLength2*Index);
	CheckMasteryDataSum += (sinSkill.UseSkill[Index].Mastery*Index);
	
	if(CheckMasteryData[Index] != CheckMasteryDataSum){
		//CheckMasteryDataSum = 0; 테스트용 
		//SendSetHackUser(52); //해킹을 하려고했던 못된 유저를 고발 TRUE 접속 종료 
		SendSetHackUser3(52,CheckMasteryData[Index],CheckMasteryDataSum);
		return FALSE;

	}

	return TRUE; 
}
int cSKILL::ReformSkillMasteryForm( sSKILL *pSkill ,int Index)
{
	
	if(pSkill){
		for(int j=1;j < SIN_MAX_USE_SKILL; j++){
			if(sinSkill.UseSkill[j].Flag){
				if(pSkill->CODE == sinSkill.UseSkill[j].CODE){
					Index = j;
				}
			}
		}
	}

	CheckMasteryData[Index] = lpCurPlayer->dwObjectSerial+sinSkill.UseSkill[Index].CODE; //초기화
	CheckMasteryData[Index] += (sinSkill.UseSkill[Index].GageLength*Index);
	CheckMasteryData[Index] += (sinSkill.UseSkill[Index].GageLength2*Index);
	CheckMasteryData[Index] += (sinSkill.UseSkill[Index].Mastery*Index);

	return TRUE;
}

//스킬포인트를 체크한다
int cSKILL::CheckSkillPointForm(sSKILL *pSkill)
{
	DWORD	dwXorKey = lpCurPlayer->dwObjectSerial&0x5F;

	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		if(sinSkill.UseSkill[j].Flag &&sinSkill.UseSkill[j].Use &&sinSkill.UseSkill[j].Point){
			if(pSkill->CODE == sinSkill.UseSkill[j].CODE){
				//if(CheckSkillPointData[j]){
					if(CheckSkillPointData[j] != (sinSkill.UseSkill[j].Point^dwXorKey) ){
						SendSetHackUser3(54,CheckSkillPointData[j],sinSkill.UseSkill[j].Point);
						return FALSE;
					}
				//}
			}
		}
	}
	
	return TRUE;
}

//스킬포인트를 저장한다
int cSKILL::ReformSkillPointForm(sSKILL *pSkill)
{
	DWORD	dwXorKey = lpCurPlayer->dwObjectSerial&0x5F;

	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		if(sinSkill.UseSkill[j].Flag ){
			if(pSkill->CODE == sinSkill.UseSkill[j].CODE){
				CheckSkillPointData[j] = ( sinSkill.UseSkill[j].Point^dwXorKey);
				break;
			}
		}
	}
	return TRUE;
}

//스킬포인트 첫포인트를 저장한다 
int cSKILL::InitReformSkillPointForm()
{
	/*
	DWORD	dwXorKey = lpCurPlayer->dwObjectSerial&0x5F;
	
	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		if(sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Use && sinSkill.UseSkill[j].Point){
			CheckSkillPointData[j] = ( sinSkill.UseSkill[j].Point^dwXorKey);
		}
	}
	*/
	
	return TRUE;
}

//스킬의 스킬포인트를 얻어온다
int cSKILL::GetSkillPoint(DWORD CODE)
{
	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		if(sinSkill.UseSkill[j].Flag){
			if(CODE == sinSkill.UseSkill[j].CODE){
				return sinSkill.UseSkill[j].Point;
			}
		}
	}

	return 0;	
}

//현재사용중인 유지형 스킬을 찾아서 포인트를 리턴한다
int cSKILL::SearchContiueSkill(DWORD CODE)
{
	for(int k = 0; k < 10 ; k++){
		if(ContinueSkill[k].Flag){
			if(ContinueSkill[k].CODE ==CODE){
				return ContinueSkill[k].Point;

			}
		}
	}
	return 0;
}

//현재 사용중인 유지형스킬의 인덱스를 찾아 리턴한다
int cSKILL::SearchContiueSkillIndex(DWORD CODE)
{
	for(int k = 0; k < 10 ; k++){
		if(ContinueSkill[k].Flag){
			if(ContinueSkill[k].CODE ==CODE){
				return k+1;
			}
		}
	}
	return 0;
}
//현재 사용중인 유지형스킬의 코드를 찾아 리턴한다
int cSKILL::SearchContiueSkillCODE(DWORD CODE)
{
	for(int k = 0; k < 10 ; k++){
		if(ContinueSkill[k].Flag){
			if(ContinueSkill[k].CODE ==CODE){
				return ContinueSkill[k].CODE;
			}
		}
	}
	return 0;
}

//버추얼라이프를 셋팅한다
int AddVirtualLife[2] = {0,0};
int sinVirtualLifeTime = 0;
int sinVirtualLifePercent = 0;

// 박재원 - 부스터 아이템(생명력)
int AddBoosterLife = 0;
int BoosterLifeTime = 0;
int BoosterLifePercent = 0;
int LifeFlag = 0;
short tempLife[2] = {0,0};

// 박재원 - 부스터 아이템(기력)
int AddVirtualMana[2] = {0,0};
int sinVirtualManaTime = 0;
int sinVirtualManaPercent = 0;
int ManaFlag = 0;
short tempMana[2] = {0,0};

// 박재원 - 부스터 아이템(근력)
int AddVirtualStamina[2] = {0,0};
int sinVirtualStaminaTime = 0;
int sinVirtualStaminaPercent = 0;
int StaminaFlag = 0;
short tempStamina[2] = {0,0};

// 장별 - 스킬 딜레이
int sinAddSkillDelay = 0;
int sinSkillDelayTime = 0;
int SkillDelayFlag = 0;

int cSKILL::SetVirtualLife(int Time , int Point)
{

	//Contiue 초기화
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == SKILL_VIRTUAL_LIFE){
				//memset(&ContinueSkill[i],0,sizeof(sSKILL));
				return FALSE; //중복시전 불가
			}
		}
	}

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_VIRTUAL_LIFE;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}

	AddVirtualLife[1]  = ((int)sinChar->Life[1] * Virtual_Life_Percent[Point-1])/100;
	sinVirtualLifeTime = Time;
	sinVirtualLifePercent = Virtual_Life_Percent[Point-1];
	CheckVirtualLifeTime = 0; //체크시간초기화
	cInvenTory.SetItemToChar();
	return TRUE;
}

// 박재원 - 부스터 아이템
int cSKILL::UseBoosterItem(sITEM *pItem) { return TRUE; }

// 박재원 - 데미지 부스터를 셋팅한다
int cSKILL::SetBoosterItem(DWORD ItemCODE , int Time)
{
	int UseFlag = 0; 
	sSKILL TempSkill;
	memset(&TempSkill,0,sizeof(sSKILL));
	char *Path;

	DWORD ItemMask;
	int cnt = 0;

	ItemMask = ItemCODE&sinITEM_MASK3;

	if(ItemMask==sin21 || ItemMask==sin24 || ItemMask==sin27) cnt = 0; // 1시간
	if(ItemMask==sin22 || ItemMask==sin25 || ItemMask==sin28) cnt = 1; // 3시간
    if(ItemMask==sin23 || ItemMask==sin26 || ItemMask==sin29) cnt = 2; // 1일

	switch(ItemMask)
	{
		// 생명력 부스터
	case sin21 :  
	case sin22 :
	case sin23 :
		UseFlag = 0;
		Path = "image\\Sinimage\\skill\\keep\\Booster_01.tga";
		TempSkill.CODE = BOOSTER_ITEM_LIFE;
		TempSkill.Point = 0; // 부스터 아이템은 레벨이 없다.
		TempSkill.UseTime = BoosterItem_UseTime[cnt];
		TempSkill.CheckTime = (BoosterItem_UseTime[cnt]*70) - (Time*70); 
		TempSkill.MatIcon = Mat_BoosterItem[UseFlag];
		break;

		// 기력 부스터
	case sin24 :  
	case sin25 :
	case sin26 :  
		UseFlag = 1;
		Path = "image\\Sinimage\\skill\\keep\\Booster_02.tga";
		TempSkill.CODE = BOOSTER_ITEM_MANA;
		TempSkill.Point = 0; // 부스터 아이템은 레벨이 없다.
		TempSkill.UseTime = BoosterItem_UseTime[cnt];
		TempSkill.CheckTime = (BoosterItem_UseTime[cnt]*70) - (Time*70); 
		TempSkill.MatIcon = Mat_BoosterItem[UseFlag];
		break;

		// 근력 부스터
	case sin27 :  
	case sin28 :
	case sin29 :   
		UseFlag = 2;
		Path = "image\\Sinimage\\skill\\keep\\Booster_03.tga";
		TempSkill.CODE = BOOSTER_ITEM_STAMINA;
		TempSkill.Point = 0; // 부스터 아이템은 레벨이 없다.
		TempSkill.UseTime = BoosterItem_UseTime[cnt];
		TempSkill.CheckTime = (BoosterItem_UseTime[cnt]*70) - (Time*70); 
		TempSkill.MatIcon = Mat_BoosterItem[UseFlag];
		break;
	}

	char szBuff[128];
	if(!Mat_BoosterItem[UseFlag]){
		wsprintf(szBuff,Path,UseFlag);
		ReadTextures();	//텍스쳐 로딩
	}

	sinContinueSkillSet(&TempSkill);

	if(UseFlag == 0)
	{
		// 박재원 - 부스터 아이템(생명력)
		AddBoosterLife  = ((int)sinChar->Life[1] * BoosterItem_DataPercent[UseFlag])/100;
		BoosterLifeTime = Time;
		BoosterLifePercent = BoosterItem_DataPercent[UseFlag];
		CheckBoosterLifeTime = 0; //체크시간초기화
	}
	else if(UseFlag == 1)
	{
		// 박재원 - 부스터 아이템(기력)
		AddVirtualMana[1]  = ((int)sinChar->Mana[1] * BoosterItem_DataPercent[UseFlag])/100;
		sinVirtualManaTime = Time;
		sinVirtualManaPercent = BoosterItem_DataPercent[UseFlag];
		CheckVirtualManaTime = 0; //체크시간초기화
	}
	else if(UseFlag==2)
	{
		// 박재원 - 부스터 아이템(근력)
		AddVirtualStamina[1]  = ((int)sinChar->Stamina[1] * BoosterItem_DataPercent[UseFlag])/100;
		sinVirtualStaminaTime = Time;
		sinVirtualStaminaPercent = BoosterItem_DataPercent[UseFlag];
		CheckVirtualStaminaTime = 0; //체크시간초기화
	}

	cInvenTory.SetItemToChar();

	return TRUE;
}

// 장별 - 스킬 딜레이
int cSKILL::SetSkillDelayItem(DWORD ItemCODE , int Time)
{
	int UseFlag = 0; 
	sSKILL TempSkill;
	memset(&TempSkill,0,sizeof(sSKILL));
	char *Path;

	DWORD ItemMask;
	int cnt = 0;

	ItemMask = ItemCODE&sinITEM_MASK3;

	if(ItemMask==sin30 || ItemMask==sin33 || ItemMask==sin36) cnt = 0;
	if(ItemMask==sin31 || ItemMask==sin34 || ItemMask==sin37) cnt = 1;
	if(ItemMask==sin32 || ItemMask==sin35 || ItemMask==sin38) cnt = 2;

	switch(ItemMask)
	{
		// 생명력 부스터
	case sin30 :  
	case sin31 :
	case sin32 :
		UseFlag = 0;
		Path = "image\\Sinimage\\skill\\keep\\Booster_01.tga";
		TempSkill.CODE = SKILLDELAY_ITEM_LIFE;
		TempSkill.Point = 0; 
		TempSkill.UseTime = nSkillDelay_UseTime[cnt];
		TempSkill.CheckTime = (nSkillDelay_UseTime[cnt]*70) - (Time*70); 
		TempSkill.MatIcon = nMat_SkillDelayItem[UseFlag];
		break;
	}


	char szBuff[128];
	if(!nMat_SkillDelayItem[UseFlag]){
		wsprintf(szBuff,Path,UseFlag);
		ReadTextures();	//텍스쳐 로딩
	}

	sinContinueSkillSet(&TempSkill);

	if(UseFlag == 0)
	{
		sinAddSkillDelay  = 10;
		sinSkillDelayTime = Time;
		BoosterLifePercent = BoosterItem_DataPercent[UseFlag];
		CheckSkillDelayTime = 0; //체크시간초기화
	}


	cInvenTory.SetItemToChar();

	return TRUE;
}


// 장별 - 스킬 딜레이
int cSKILL::UseSkillDelayItem(sITEM *pItem) { return TRUE; }


//트라이엄프 오브 발할라
int cSKILL::SetT_Of_Valhalla( int Point , int LV)
{

	//Contiue 초기화
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == SKILL_TRIUMPH_OF_VALHALLA ||ContinueSkill[i].CODE == SKILL_HALL_OF_VALHALLA){
				//memset(&ContinueSkill[i],0,sizeof(sSKILL));
				return FALSE; //중복시전 불가
			}
		}
	}

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_TRIUMPH_OF_VALHALLA;
			ContinueSkill[i].UseTime = T_Of_Valhalla_Time[Point-1];
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}

	SetT_Of_ValhallaLV = LV;

	if(SetT_Of_ValhallaFlag2){ //자신이 사용한것이면
		SetT_Of_ValhallaFlag2 = 0;
		SetT_Of_ValhallaFlag  = 0;
	}
	else{
		SetT_Of_ValhallaFlag  = 1; //남이사용해준것이면

	}
	cInvenTory.SetItemToChar();
	return TRUE;
}

//인첸트 웨폰
int cSKILL::SetEnchant_Weapon(int Time , int Point)
{

	//Contiue 초기화
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == SKILL_ENCHANT_WEAPON){
				//memset(&ContinueSkill[i],0,sizeof(sSKILL));
				return FALSE; //중복시전 불가
			}
		}
	}

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_ENCHANT_WEAPON;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}
	cInvenTory.SetItemToChar();
	return TRUE;
}

int cSKILL::Evasion_Mastery(int Time, int Point)
{
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++)
	{
		if (ContinueSkill[i].Flag)
		{
			if (ContinueSkill[i].CODE == SKILL_EVASION_MASTERY)
			{
				memset(&ContinueSkill[i], 0, sizeof(sSKILL));
				break;
			}
		}
	}

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++)
	{
		if (!ContinueSkill[i].Flag)
		{
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_EVASION_MASTERY;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}

	cInvenTory.SetItemToChar();

	return TRUE;
}

//포스오브네이쳐
int cSKILL::ForeceOfNature(int Time , int Point , int Party)
{

	//Contiue 초기화
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == SKILL_FORCE_OF_NATURE){
				if(ContinueSkill[i].Flag == 2 && !Party ){
					memset(&ContinueSkill[i],0,sizeof(sSKILL));
					break;
				}
				else return FALSE; //중복시전 불가
			}
		}
	}

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = Party+1; //파티원은 2로한다
			ContinueSkill[i].CODE = SKILL_FORCE_OF_NATURE;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}
	cInvenTory.SetItemToChar();
	return TRUE;
}

//헬오브 발할라
int cSKILL::HellOfValhalla(int Time , int Point,int TVPoint , int LV , int Party)
{

	//Contiue 초기화
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == SKILL_HALL_OF_VALHALLA){
				if(ContinueSkill[i].Flag == 2 && !Party){
					memset(&ContinueSkill[i],0,sizeof(sSKILL));
					break;
				}
				else return FALSE; //중복시전 불가
			}
		}
	}

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = Party+1; //파티원은 2로한다
			ContinueSkill[i].CODE = SKILL_HALL_OF_VALHALLA;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}
	SetT_Of_ValhallaPOINT = TVPoint;
	SetH_Of_ValhallaLV = LV;    //시전자의 레벨
	cInvenTory.SetItemToChar();
	return TRUE;
}


//리제너레이터 필드
int cSKILL::Regeneration_Field(int Time , int Point , int Party)
{

	//Contiue 초기화
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == SKILL_REGENERATION_FIELD){
				if(ContinueSkill[i].UseTime > Time)return FALSE; //같은 캐릭사용시 리턴
				ContinueSkill[i].UseTime = Time;
				return TRUE; //시간갱신
			}
		}
	}

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(!ContinueSkill[i].Flag){
			ContinueSkill[i].Flag = 1+Party; //파티원은 2로한다
			ContinueSkill[i].CODE = SKILL_REGENERATION_FIELD;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}
	cInvenTory.SetItemToChar();
	return TRUE;
}

//스킬 내용을 찾는다
int cSKILL::SearchSkillDoc(sSKILL *pSkill) { return TRUE; }

//원그리기
int cSKILL::SkillBarDraw(sSKILL *pSkill , int x , int y, int Alpha) { return TRUE; }



//지속 스킬 체크
int cSKILL::CheckContinueIcon()
{
	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){ 
		if(ContinueSkill[i].Flag){
			if(!ContinueSkill[i].MatIcon){
				for(int j = 0 ; j < SIN_MAX_SKILL; j++){
					if(ContinueSkill[i].CODE == sSkill[j].CODE){
						if(sSkill[j].MatIcon){ 
							ContinueSkill[i].MatIcon = sSkill[j].MatIcon;
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

//지속 스킬 체크
int cSKILL::CancelContinueSkill(DWORD CODE)
{

	for(int i = 0 ; i < MAX_CONTINUE_SKILL ; i++){ 
		if(ContinueSkill[i].Flag){
			if(ContinueSkill[i].CODE == CODE){
				memset(&ContinueSkill[i],0,sizeof(sSKILL));
				cInvenTory.SetItemToChar(); //스텟을 바꿔준다 (색이있을경우는 스위치로 바꿔준다)
				break;
			}
		}
	}
	return FALSE;
}

//스킬단축기 초기화
int cSKILL::SkillShortKeyInit()
{
	for(int j=1;j < SIN_MAX_USE_SKILL; j++){
		sinSkill.UseSkill[j].ShortKey = 0;
	}
	return TRUE;
}

// 박재원 - 스킬 마스터(전직별로 스킬의 숙련 여부를 체크한다.)
int cSKILL::CheckMaturedSkill(int ChangeJob)
{
	int matured_skill_count = 0;

	switch(ChangeJob)
	{
	case 1:
		for(int j=1 ; j<5 ; j++) // 1차 전직후 생기는 스킬
		{
			if( sinSkill.UseSkill[j].UseSkillCount<10000 && sinSkill.UseSkill[j].Skill_Info.USECODE != SIN_SKILL_USE_NOT && 
				sinSkill.UseSkill[j].UseSkillMastery)
			{
				matured_skill_count++;
			}
		}
		break;
	case 2:
		for(int j=5 ; j<9 ; j++) // 2차 전직후 생기는 스킬
		{
			if( sinSkill.UseSkill[j].UseSkillCount<10000 && sinSkill.UseSkill[j].Skill_Info.USECODE != SIN_SKILL_USE_NOT && 
				sinSkill.UseSkill[j].UseSkillMastery)
			{
				matured_skill_count++;
			}
		}
		break;
	case 3:
		for(int j=9 ; j<13 ; j++) // 3차 전직후 생기는 스킬
		{
			if( sinSkill.UseSkill[j].UseSkillCount<10000 && sinSkill.UseSkill[j].Skill_Info.USECODE != SIN_SKILL_USE_NOT && 
				sinSkill.UseSkill[j].UseSkillMastery)
			{
				matured_skill_count++;
			}
		}
		break;
	}

	if(matured_skill_count <= 4 && matured_skill_count>0)
		return TRUE; // 사용 가능
	else
		return FALSE;  // 사용 불가능
}
/*
//원그리기
int cSKILL::SkillBarDraw(sSKILL *pSkill , int x , int y)
{

	D3DTLVERTEX tlVertex[72][4];  //4개로 나눠서 노가다 좌표로 구한다 

	int Time = 0; 
	float Time2 = 0;

	Time = (int)(((float)pSkill->CheckTime/((float)pSkill->UseTime*70.0f))*72.0f);   
	Time2++;

	float fx , fy =0; 
	
	int i = 0 , j = 0;
	int sinY,cosY;
	for( i = 0 ; i < Time ; i++){  
		for( j = 0 ; j < 4 ; j++){
			tlVertex[i][j].rhw = 1;
			tlVertex[i][j].color	= RGBA_MAKE(255,0,0,255);
			tlVertex[i][j].specular  =  RGBA_MAKE( 0,0,0,0 );
			tlVertex[i][j].sz = 0;
		}


		sinY = GetSin[ (i* 56)&ANGCLIP];      
		cosY = GetCos[ ((i* 56)+ANGLE_180)&ANGCLIP ];

		fx = (float) (( 16*sinY + 16*cosY )>>8); 
		fy = (float) (( 16*cosY - 16*sinY )>>8);
		fx /= 256;
		fy /= 256;

		//dsDrawColorBox( sinInvenColor[0] ,fx+x,fy+y, 16, 16 );

		tlVertex[i][0].sx = (float)(fx+x);  
		tlVertex[i][0].sy = (float)(fy+y);
		tlVertex[i][0].tu = 0;
		tlVertex[i][0].tv = 0;

		tlVertex[i][1].sx = float(x);
		tlVertex[i][1].sy = float(y);
		tlVertex[i][1].tu = 0;
		tlVertex[i][1].tv = 1.0f;


		sinY = GetSin[ ((i+1)* 56)&ANGCLIP];
		cosY = GetCos[ (((i+1)* 56)+ANGLE_180)&ANGCLIP ];

		fx = (float) (( 16*sinY + 16*cosY )>>8);
		fy = (float) (( 16*cosY - 16*sinY )>>8);
		fx /= 256;
		fy /= 256;

		tlVertex[i][2].sx = (float)(fx+x);
		tlVertex[i][2].sy = (float)(fy+y);
		tlVertex[i][2].tu = 1.0f;
		tlVertex[i][2].tv = 0;
		

	}


	lpD3DDevice->SetTexture( 0, 0 );
	for( i = 0 ; i < 72 ; i++){
		lpD3DDevice->DrawPrimitive( D3DPT_TRIANGLEFAN ,
							D3DFVF_TLVERTEX  ,
							tlVertex[i],
							3,
							NULL);
	}
	return TRUE;
}
*/
