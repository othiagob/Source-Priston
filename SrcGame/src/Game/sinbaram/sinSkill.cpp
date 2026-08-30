#include "sinLinkHeader.h"
#include "..\\skillsub.h"
#include "..\\field.h"
#include <ConfirmationBox.h>
#include <Utils/Logs/utils.h>

sUSESKILL sinSkill;
sSKILL* pUseSkill;
sSKILL  ContinueSkill[20];
sSKILL  TempContinueSkill[20];
int SkillCountTime2[17] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int ContinueSkillMouseIndex = 0;

JOB_DATA_CODE* sinJobList;

sSKILL* pMasterSkill;  //스킬 습득 포인터 

sSKILLPASSIVE sSkillPassive; //캐릭터에 저장되는 패시브 스킬 

/////////////
int ShowSkillUpInfo = 0;     //메세지 박스와 함께 스킬정보를 보여준다 
int  SkillMasterOkFlag[2] = { 0,0 };
int  SkillMasterFlag = 0;    //NPC한테 스킬 을 배우고자 한다 

int SkillMasteryShowFlag = 0;		//숙련도를 표시한다 
char szUseSkillMastery[32];			//스킬 숙련도 
POINT UseSkillMasteryPosi = { 0,0 };	//숙련도 표시 포지션 

int MatForceOrb[12] = { 0 };

// 박재원 - 빌링 매직 포스
int MatBillingMagicForceOrb[3] = { 0 };

// 박재원 - 부스터 아이템
int Mat_BoosterItem[9] = { 0 };

// 장별 - 스킬 딜레이
int nMat_SkillDelayItem[3] = { 0 };

int MatQuestTimer = 0;

/////////////스킬 정보  보여주기 

POINT SkillUpInfo = { 13,21 };       //스킬 정보 보여주기 
POINT SkillUpInfoPosi = { 90,100 };

////////////
int     SkillInfoLineCnt = 0;

int     SkillInfoShowFlag = 0;
POINT	SkillInfoBoxPosi = { 0,0 };
POINT   SkillInfoTextPosi = { 0,0 };
POINT   SkillBoxSize = { 0,0 };


char    szSkillInfoBuff[5000];
char    szSkillMasterInfoBuff[5000];

char    szSkillInfoBuff2[5000];
char    szSkillMasterInfoBuff2[5000];


int     SkillDocLineCnt[6] = { 0,0,0,0,0,0 };
int     SkillUseWeaponClass = 0;
int     NextSkillLevelLine = 0;
/////////////

int ShowSkillClose = 0; //닫기 버튼 플랙 
int SkillButtonIndex = 0;
int Skill4ButtonIndex = 0;

int DownButtonUseShorKey = 0; //단축키를 설정했을때는 스킬을 정션하지 않는다 

extern sQUEST_CHANGEJOB3	sinQuest_Daily;

int DownButtonIndex = 0;  //버튼을 누르는 효과를 주기위한 인덱스 
int LDownButtonIndex = 0;  //왼쪽 버튼 
int RDownButtonIndex = 0;  //오른쪽 버튼 

DWORD sinSkillBoxColor = D3DCOLOR_RGBA(0, 0, 132, 128);

int ChangeJobSkillPlus = 0;
int UseSkillIndex[2] = { 0,0 }; //왼쪽 오른쪽 

int SelectInterSkill = 0;

/////////////////미니 Define
#define	NORMAL_SKILL_INDEX		14
#define MAX_USE_SKILL_POINT		10

#define SIN_MOUSE_POSI_LEFT		1
#define SIN_MOUSE_POSI_RIGHT	2

/////////////////

///////////////// 스킬 아이콘 
int   SkillIconIndex[10] = { 0,0,0,0,0,0,0,0,0,0 };
POINT SkillIconPosi = { 0,0 };
int   SkillInconCnt = 0;


int GageLenght[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

DWORD ClearSkillCODE = 0; //초기화될 스킬이 있나없나 
POINT ClearSkillButtonPosi = { 0,0 }; //스킬 초기화 버튼 위치 

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

	{ "Coroa de SOD Gold", SODSKILL_GOLD, "", "bellatracrown_1.tga" },
	{ "Coroa de SOD Prata", SODSKILL_SILVER, "", "bellatracrown_2.tga" },
	{ "Coroa de SOD Bronze", SODSKILL_BRONZE, "", "bellatracrown_3.tga" },

	{ "Chat", CHATGLOBAL, "", "megaphone.tga" },
	{ "Rei PVP", REIPVP, "", "battlepvp.tga" },
	{ "Top LVL", TOPLVL, "", "toplevel.tga" },
	{ "Modo PVP", BUFF_WARMODE, "", "battle.tga" },

	{ "Crystal", 1, 	"", "gp101.tga" },
	{ "Crystal", 1.1, "", "gp101.tga" },
	{ "Crystal", 1.2, "", "gp101.tga" },
	{ "Crystal", 1.3, "", "gp101.tga" },
	{ "Crystal", 1.4, "", "gp101.tga" },

	{ "Crystal", 2, "", "gp102.tga" },
	{ "Crystal", 2.1, "", "gp102.tga" },
	{ "Crystal", 2.2, "", "gp102.tga" },
	{ "Crystal", 2.3, "", "gp102.tga" },
	{ "Crystal", 2.4, "", "gp102.tga" },

	{ "Crystal", 3, "", "gp103.tga" },
	{ "Crystal", 3.1, "", "gp103.tga" },
	{ "Crystal", 3.2, "", "gp103.tga" },
	{ "Crystal", 3.3, "", "gp103.tga" },
	{ "Crystal", 3.4, "", "gp103.tga" },

	{ "Crystal", 4, "", "gp104.tga" },
	{ "Crystal", 4.1, "", "gp104.tga" },
	{ "Crystal", 4.2, "", "gp104.tga" },
	{ "Crystal", 4.3, "", "gp104.tga" },
	{ "Crystal", 4.4, "", "gp104.tga" },

	{ "Crystal", 5, "", "gp105.tga" },
	{ "Crystal", 5.1, "", "gp105.tga" },
	{ "Crystal", 5.2, "", "gp105.tga" },
	{ "Crystal", 5.3, "", "gp105.tga" },
	{ "Crystal", 5.4, "", "gp105.tga" },

	{ "Crystal", 6, "", "gp106.tga" },
	{ "Crystal", 6.1, "", "gp106.tga" },
	{ "Crystal", 6.2, "", "gp106.tga" },
	{ "Crystal", 6.3, "", "gp106.tga" },
	{ "Crystal", 6.4, "", "gp106.tga" },

	{ "Crystal", 7, "", "gp107.tga" },
	{ "Crystal", 7.1, "", "gp107.tga" },
	{ "Crystal", 7.2, "", "gp107.tga" },
	{ "Crystal", 7.3, "", "gp107.tga" },
	{ "Crystal", 7.4, "", "gp107.tga" },

	{ "Crystal", 8, "", "gp108.tga" },
	{ "Crystal", 8.1, "", "gp108.tga" },
	{ "Crystal", 8.2, "", "gp108.tga" },
	{ "Crystal", 8.3, "", "gp108.tga" },
	{ "Crystal", 8.4, "", "gp108.tga" },

	{ "Crystal", 9, "", "gp109.tga" },
	{ "Crystal", 9.1, "", "gp109.tga" },
	{ "Crystal", 9.2, "", "gp109.tga" },
	{ "Crystal", 9.3, "", "gp109.tga" },
	{ "Crystal", 9.4, "", "gp109.tga" },

	{ "Crystal", 10, "", "gp110.tga" },
	{ "Crystal", 10.1, "", "gp110.tga" },
	{ "Crystal", 10.2, "", "gp110.tga" },
	{ "Crystal", 10.3, "", "gp110.tga" },
	{ "Crystal", 10.4, "", "gp110.tga" },

	{ "Crystal", 11, "", "gp111.tga" },
	{ "Crystal", 11.1, "", "gp111.tga" },
	{ "Crystal", 11.2, "", "gp111.tga" },
	{ "Crystal", 11.3, "", "gp111.tga" },
	{ "Crystal", 11.4, "", "gp111.tga" },

	{ "Crystal", 12, "", "gp112.tga" },
	{ "Crystal", 12.1, "", "gp112.tga" },
	{ "Crystal", 12.2, "", "gp112.tga" },
	{ "Crystal", 12.3, "", "gp112.tga" },
	{ "Crystal", 12.4, "", "gp112.tga" },

	{ "Crystal", 13, "", "gp113.tga" },
	{ "Crystal", 13.1, "", "gp113.tga" },
	{ "Crystal", 13.2, "", "gp113.tga" },
	{ "Crystal", 13.3, "", "gp113.tga" },
	{ "Crystal", 13.4, "", "gp113.tga" },

	// Novos Cristais

	{ "Crystal", 23, "", "gp123.tga" },
	{ "Crystal", 23.1, "", "gp123.tga" },
	{ "Crystal", 23.2, "", "gp123.tga" },
	{ "Crystal", 23.3, "", "gp123.tga" },
	{ "Crystal", 23.4, "", "gp123.tga" },

	{ "Crystal", 24, "", "gp124.tga" },
	{ "Crystal", 24.1, "", "gp124.tga" },
	{ "Crystal", 24.2, "", "gp124.tga" },
	{ "Crystal", 24.3, "", "gp124.tga" },
	{ "Crystal", 24.4, "", "gp124.tga" },

	{ "Crystal", 25, "", "gp125.tga" },
	{ "Crystal", 25.1, "", "gp125.tga" },
	{ "Crystal", 25.2, "", "gp125.tga" },
	{ "Crystal", 25.3, "", "gp125.tga" },
	{ "Crystal", 25.4, "", "gp125.tga" },
};


//스킬 창 박스 위치 정보 
sSKILLBOX	sSkillBox[SIN_MAX_SKILLBOX] = {
	{{142,537,142 + 49,537 + 46}},

	{{27 - 10,502,27 + 49 - 10,502 + 46},{27 + 47 - 10,502 + 5,27 + 47 + 5 - 10,502 + 40}},
	{{67 - 10,546,67 + 49 - 10,546 + 46},{67 + 47 - 10,546 + 5,67 + 47 + 5 - 10,546 + 40}},
	{{77 - 10,466,77 + 49 - 10,466 + 46},{77 + 47 - 10,466 + 5,77 + 47 + 5 - 10,466 + 40}},
	{{137 - 10,476,137 + 49 - 10,476 + 46},{137 + 47 - 10,476 + 5,137 + 47 + 5 - 10,476 + 40}},

	{{212 - 15,466,212 + 49 - 15,466 + 46},{212 + 47 - 15,466 + 5,212 + 47 + 5 - 15,466 + 40}},
	{{232 - 15,516,232 + 49 - 15,516 + 46},{232 + 47 - 15,516 + 5,232 + 47 + 5 - 15,516 + 40}},
	{{272 - 15,466,272 + 49 - 15,466 + 46},{272 + 47 - 15,466 + 5,272 + 47 + 5 - 15,466 + 40}},
	{{332,466,332 + 49,466 + 46},{332 + 47,466 + 5,332 + 47 + 5,466 + 40}},

	{{402,476,402 + 49, 476 + 46},{402 + 47,476 + 5,402 + 47 + 5,476 + 40}},
	{{472,456,472 + 49, 456 + 46},{472 + 47,456 + 5,472 + 47 + 5,456 + 40}},
	{{512,506,512 + 49, 506 + 46},{512 + 47,506 + 5,512 + 47 + 5,506 + 40}},
	{{532,456,532 + 49, 456 + 46},{532 + 47,456 + 5,532 + 47 + 5,456 + 40}},

	{{602,481,602 + 49, 481 + 46},{602 + 47,481 + 5,602 + 47 + 5,481 + 40}},
	{{662,511,662 + 49, 511 + 46},{662 + 47,511 + 5,662 + 47 + 5,511 + 40}},
	{{681,461,681 + 49, 461 + 46},{681 + 47,461 + 5,681 + 47 + 5,461 + 40}},
	{{722,511,722 + 49, 511 + 46},{722 + 47,511 + 5,722 + 47 + 5,511 + 40}},


};

sSKILLBOX sLeftRightSkill[2] = {
	{{349,541,349 + 49,541 + 46},{349,558,349 + 5,558 + 35}},  //스킬 왼쪽 
	{{401 + 2,541,401 + 49,541 + 46},{446,558,446 + 5,558 + 35}},  //스킬 오른쪽 

};

int SkillTitle[4][2] = {
	{85,438 - 4},
	{280,438 - 4},
	{470,438 - 9},
	{675,438 - 4},

};


LPDIRECT3DTEXTURE9	lpSkillMain;
LPDIRECT3DTEXTURE9	lpSkillMain2;
LPDIRECT3DTEXTURE9    lpSkillMainLeftB;
LPDIRECT3DTEXTURE9    lpSkillMainRightB;

int SkillMain2OpenFlag = 0;
int SkillMain2ButtonFlag[2] = { 0,0 };
int Skill4MasterFlag = 0;
int MonsterDamageLine = 0;
int ChainDamageLine = 0;
int MatSkillIconBackGround = 0;

cSKILL::cSKILL()
{
	memset(&ContinueSkill, 0, sizeof(sSKILL) * 10);
}
cSKILL::~cSKILL()
{

}

void cSKILL::Init()
{

	char* FilePath = "Image\\SinImage\\skill\\keep\\";
	char szFilePath[128];
	for (int j = 0; j < SIN_MAX_SKILL; j++) {
		if (sSkill[j].IconName) {
			wsprintf(szFilePath, "%s%s", FilePath, sSkill[j].IconName);
			sSkill[j].MatIcon = CreateTextureMaterial(szFilePath, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

		}
	}


	//MatSkill[0] =  CreateTextureMaterial( "Image\\SinImage\\Skill\\Skill-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatSkill[1] =  CreateTextureMaterial( "Image\\SinImage\\Skill\\Skill-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatSkill[2] =  CreateTextureMaterial( "Image\\SinImage\\Skill\\Skill-3.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatSkill[3] =  CreateTextureMaterial( "Image\\SinImage\\Skill\\Skill-4.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	MatCircleIcon = CreateTextureMaterial("Image\\SinImage\\Skill\\keep\\GA_Mon.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);;
	MatSkillIconBackGround = CreateTextureMaterial("Image\\SinImage\\Skill\\keep\\GA_.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);;

	SearchUseSkill(); //현재 사용할수 있는 캐릭터의 스킬을 찾는다 
	Load();

	//기본공격을 셋팅한다 
	sinSkill.pLeftSkill = &sinSkill.UseSkill[0];
	sinSkill.UseSkill[0].MousePosi = 1;
	sinSkill.pLeftSkill->Position = 1;



}
/*----------------------------------------------------------------------------*
*						    Bmp파일을 로드한다
*-----------------------------------------------------------------------------*/
void cSKILL::Load()
{
	char ShortKeyDirPath[128];
	char WeaponIconPath[128];

	lpSkillDown = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\skilldown.bmp");

	//스킬 메인2=========================================================================== 
	lpSkillMain = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Skil24.bmp");
	//lpSkillMain2   = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Skil25.bmp");

	//스킬 메인 버튼
	lpSkillMainLeftB = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\LeftArrow.bmp");
	lpSkillMainRightB = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\RightArrow.bmp");
	//======================================================================================



	lpSkillGage[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage.bmp");
	lpSkillGage[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage2.bmp");
	lpSkillGage2[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage-2.bmp");
	lpSkillGage2[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage-22.bmp");
	lpSkillGage3 = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage-3.bmp");
	lpSkillGage4 = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage-4.bmp");
	lpSkillGage5 = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Gage-5.bmp");

	lpMasteryInfo = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\MasteryInfo.bmp");

	lpSelectSkill = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\SelectSkill.bmp");
	lpSkillBoxBack = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\BOX4040.bmp");

	for (int i = 0; i < 8; i++) {
		wsprintf(ShortKeyDirPath, "Image\\SinImage\\skill\\ShortKey\\f%d.bmp", i + 1);
		lpShortKey[i] = LoadDibSurfaceOffscreen(ShortKeyDirPath);//펑션키 

	}
	for (int i = 0; i < 10; i++)
	{
		wsprintf(WeaponIconPath, "Image\\SinImage\\skill\\WeaponIcon\\%d.bmp", i + 1);
		lpWeaponIcon[i] = LoadDibSurfaceOffscreen(WeaponIconPath);//무기 아이콘 

	}

}

//스킬이미지를 로드한다 
void cSKILL::LoadUseSkillImage()
{
	int i = 0;
	char TempDirPath[128];
	char TempTitleDirPath[128];
	char ButtonImageDirPath[128];

	TempDirPath[0] = 0;
	switch (UseSkillBox) {
	case GROUP_MECHANICIAN:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Mecha\\");

		break;
	case GROUP_FIGHTER:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Fighter\\");

		break;
	case GROUP_PIKEMAN:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Pikeman\\");

		break;
	case GROUP_ARCHER:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Archer\\");

		break;
	case GROUP_ATALANTA:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Atalanta\\");
		break;
	case GROUP_KNIGHT:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Knight\\");

		break;
	case GROUP_MAGICIAN:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Magician\\");

		break;
	case GROUP_PRIESTESS:
		lstrcpy(TempDirPath, "Image\\SinImage\\Skill\\Priestess\\");

		break;


	}
	if (!TempDirPath[0]) //경로명을 찾지못하면 리턴한다 
		return;


	lpSkillButton[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Skill\\Skill_Normal.bmp"); //노멀 어택 
	lpSkillButton_Gray[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Skill\\Skill_Normal.bmp"); //노멀 어택 
	for (i = 0; i < 4; i++) {
		wsprintf(TempTitleDirPath, "%s%s%d%s", TempDirPath, "JobTitle\\", i + 1, ".bmp");
		lpSkillTitle[i] = LoadDibSurfaceOffscreen(TempTitleDirPath);

	}

	/////////////////버튼 이미지 로딩 
	for (i = 1; i < 17; i++) { //노멀 어택 이미지를 뺀 나머지 스킬 이미지 
		if (sinSkill.UseSkill[i].Flag) {
			if (sinSkill.UseSkill[i].FileName[0]) {
				wsprintf(ButtonImageDirPath, "%s%s%s%s", TempDirPath, "Button\\", sinSkill.UseSkill[i].FileName, ".bmp");
				lpSkillButton[i] = LoadDibSurfaceOffscreen(ButtonImageDirPath);
				wsprintf(ButtonImageDirPath, "%s%s%s%s%s", TempDirPath, "Button\\", sinSkill.UseSkill[i].FileName, "_", ".bmp");
				lpSkillButton_Gray[i] = LoadDibSurfaceOffscreen(ButtonImageDirPath);
			}
		}
	}

}

/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/
void cSKILL::Release()
{
	int i = 0;
	if (lpSkillGage[0]) {
		lpSkillGage[0]->Release();
		lpSkillGage[0] = 0;
	}
	if (lpSkillGage[1]) {
		lpSkillGage[1]->Release();
		lpSkillGage[1] = 0;


	}
	if (lpSelectSkill) {
		lpSelectSkill->Release();
		lpSelectSkill = 0;

	}
	for (i = 0; i < 4; i++) {
		if (lpSkillTitle[i]) {
			lpSkillTitle[i]->Release();
			lpSkillTitle[i] = 0;

		}
	}
	for (i = 0; i < 4; i++) {
		if (lpSkillTitle[i]) {
			lpSkillTitle[i]->Release();
			lpSkillTitle[i] = 0;

		}
	}
	for (i = 0; i < 17; i++) {
		if (lpSkillButton[i]) {
			lpSkillButton[i]->Release();
			lpSkillButton[i] = 0;


		}
		if (lpSkillButton_Gray[i]) {
			lpSkillButton_Gray[i]->Release();
			lpSkillButton_Gray[i] = 0;

		}
	}
	for (i = 0; i < 8; i++) {
		if (lpShortKey[i]) {
			lpShortKey[i]->Release();
			lpShortKey[i] = 0;

		}
	}
	for (i = 0; i < 10; i++) {
		if (lpWeaponIcon[i]) {
			lpWeaponIcon[i]->Release();
			lpWeaponIcon[i] = 0;
		}
	}

}
/*----------------------------------------------------------------------------*
*							     그리기
*-----------------------------------------------------------------------------*/
int CheckVirtualLifeTime = 0;
int CheckBoosterLifeTime = 0; // 박재원 - 부스터 아이템(생명력)
int CheckVirtualManaTime = 0; // 박재원 - 부스터 아이템(기력)
int CheckVirtualStaminaTime = 0; // 박재원 - 부스터 아이템(근력)
int CheckSkillDelayTime = 0; // 장별 - 스킬 딜레이
bool bchecked = false;

void cSKILL::DrawUp()
{
	int i = 0;
	//지속 스킬을 그린다
	for (i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (TempContinueSkill[i].Flag) {
			if (TempContinueSkill[i].MatIcon) {
				if (!TempContinueSkill[i].IconFlag) {
					dsDrawTexImage(MatSkillIconBackGround, 10 + TempContinueSkill[i].IconPosiX, 10, 32, 32, TempContinueSkill[i].IconAlpha);
					SkillBarDraw(&TempContinueSkill[i], 10 + 16 + TempContinueSkill[i].IconPosiX, 10 + 16, TempContinueSkill[i].IconAlpha);

					if (strcmp(TempContinueSkill[i].Skill_Info.SkillName, "Desafio") == 0)
					{
						if (bchecked == false)
						{
							char* Path = "image\\Sinimage\\skill\\keep\\QuestTimer.tga";
							MatQuestTimer = CreateTextureMaterial(Path, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
							ReadTextures();	//텍스쳐 로딩
							bchecked = true;
						}
						dsDrawTexImage(MatQuestTimer, 10 + TempContinueSkill[i].IconPosiX, 10, 32, 32, TempContinueSkill[i].IconAlpha);
					}
					else
						dsDrawTexImage(TempContinueSkill[i].MatIcon, 10 + TempContinueSkill[i].IconPosiX, 10, 32, 32, TempContinueSkill[i].IconAlpha);
				}
			}
		}
	}

}
void cSKILL::Draw()
{
	int i = 0;

	/*
	//지속 스킬을 그린다
	for(i = 0 ; i < MAX_CONTINUE_SKILL ; i++){
		if(TempContinueSkill[i].Flag){
			if(TempContinueSkill[i].MatIcon){
				if(!TempContinueSkill[i].IconFlag){
					dsDrawTexImage(MatSkillIconBackGround , 10+TempContinueSkill[i].IconPosiX, 10, 32, 32 , TempContinueSkill[i].IconAlpha );
					SkillBarDraw(&TempContinueSkill[i],10+16+TempContinueSkill[i].IconPosiX,10+16,TempContinueSkill[i].IconAlpha);
					dsDrawTexImage(TempContinueSkill[i].MatIcon , 10+TempContinueSkill[i].IconPosiX, 10, 32, 32 , TempContinueSkill[i].IconAlpha );
				}
			}
		}
	}
	*/

	/////////////이미지 테스트 
/*
	for(i=0; i <  9 ; i++){
		if(lpWeaponIcon[i]){
			DrawSprite(100+(i*20),100,lpWeaponIcon[i],0,0,18,24);

		}
	}
*/
/////////////////스킬 박스 
//dsDrawTexImage( MatSkill[0] , 0, 600-sinMoveKindInter[SIN_SKILL], 256, 256 , 255 );       
//dsDrawTexImage( MatSkill[1] , 256, 600-sinMoveKindInter[SIN_SKILL], 256, 256 , 255 );   
//dsDrawTexImage( MatSkill[2] , 256+256, 600-sinMoveKindInter[SIN_SKILL], 256, 256 , 255 );       
//dsDrawTexImage( MatSkill[3] , 256+256+256, 600-sinMoveKindInter[SIN_SKILL], 32, 256 , 255 );   

//DrawSprite(0,0,lpSkillMain,0,0,800,200);

//스킬 메인2=====================================================================================
	if (SkillMain2OpenFlag) {
		DrawSprite(0, 600 - sinMoveKindInter[SIN_SKILL] + 56, lpSkillMain2, 0, 0, 800, 200);
		//왼쪽
		if (SkillMain2ButtonFlag[0]) {
			DrawSprite(13, 600 + 83 - sinMoveKindInter[SIN_SKILL] + 56, lpSkillMainLeftB, 0, 0, 20, 20);
		}
	}

	if (!SkillMain2OpenFlag)
	{
		DrawSprite(0, 600 - sinMoveKindInter[SIN_SKILL] + 56, lpSkillMain, 0, 0, 800, 200);

		if (SkillMain2ButtonFlag[1])
		{
			DrawSprite(766, 600 + 83 - sinMoveKindInter[SIN_SKILL] + 56, lpSkillMainRightB, 0, 0, 20, 20);
		}
	}

	//===========================================================================================
	if (OpenFlag && !SkillMain2OpenFlag)
	{
		for (i = 0; i < (int)sinChar->ChangeJob + 1; i++)
		{
			DrawSprite(SkillTitle[i][0], SkillTitle[i][1] + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillTitle[i], 0, 0, 100, 20);
		}

		for (i = 1; i < ChangeJobSkillPlus; i++)
		{
			if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Use && SkillMasterFlag)
			{
				if (lpSkillButton[i])
				{
					DrawSprite(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top + (256 - sinMoveKindInter[SIN_SKILL]),
						lpSkillButton[i], 0, 0, 49, 46);
				}
			}
			if (sinSkill.UseSkill[i].Flag && !sinSkill.UseSkill[i].Use && SkillMasterFlag)
			{
				if (lpSkillButton_Gray[i])
				{
					DrawSprite(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top + (256 - sinMoveKindInter[SIN_SKILL]),
						lpSkillButton_Gray[i], 0, 0, 49, 46);
				}
			}
			if (sinSkill.UseSkill[i].Flag && SkillMasterFlag && sinSkill.UseSkill[i].Point >= 10)
			{
				if (lpSkillButton_Gray[i])
				{
					DrawSprite(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top + (256 - sinMoveKindInter[SIN_SKILL]),
						lpSkillButton_Gray[i], 0, 0, 49, 46);
				}
			}
			if (!SkillMasterFlag)
			{
				if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Point)
				{
					if (lpSkillButton[i])
					{
						DrawSprite(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top + (256 - sinMoveKindInter[SIN_SKILL]),
							lpSkillButton[i], 0, 0, 49, 46);

					}
				}
				if (sinSkill.UseSkill[i].Flag && !sinSkill.UseSkill[i].Point)
				{
					if (lpSkillButton_Gray[i])
					{
						DrawSprite(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top + (256 - sinMoveKindInter[SIN_SKILL]),
							lpSkillButton_Gray[i], 0, 0, 49, 46);
					}
				}
			}

			if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Skill_Info.USECODE == SIN_SKILL_USE_NOT)
			{
				DrawSprite(sinSkill.SkillBox[i].GageRect.left, sinSkill.SkillBox[i].GageRect.top + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage4, 0, 0, 5, 35);
			}

			if (sinSkill.UseSkill[i].Flag && (sinSkill.UseSkill[i].Skill_Info.USECODE != SIN_SKILL_USE_NOT || sinSkill.UseSkill[i].Skill_Info.Element[0]))
			{
				DrawSprite(sinSkill.SkillBox[i].GageRect.left, sinSkill.SkillBox[i].GageRect.top + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage3, 0, 0, 5, 35);
				DrawSprite(sinSkill.SkillBox[i].GageRect.left, sinSkill.SkillBox[i].GageRect.top + (35 - sinSkill.UseSkill[i].UseSkillMasteryGage) + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage2[0], 0, 35 - sinSkill.UseSkill[i].UseSkillMasteryGage, 5, 35);
				if (sinSkill.UseSkill[i].UseSkillMasteryGage >= 3)
					DrawSprite(sinSkill.SkillBox[i].GageRect.left, sinSkill.SkillBox[i].GageRect.top + (35 - sinSkill.UseSkill[i].UseSkillMasteryGage) + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage2[1], 0, 0, 5, 3);

			}
			if (sinSkill.UseSkill[i].Flag)
			{
				DrawSprite(sinSkill.SkillBox[i].GageRect.right + 1, sinSkill.SkillBox[i].GageRect.top + (35 - sinSkill.UseSkill[i].GageLength) + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage[0], 0, 35 - sinSkill.UseSkill[i].GageLength, 5, 35);
				if (sinSkill.UseSkill[i].GageLength >= 3)
					DrawSprite(sinSkill.SkillBox[i].GageRect.right + 1, sinSkill.SkillBox[i].GageRect.top + (35 - sinSkill.UseSkill[i].GageLength) + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage[0], 0, 0, 5, 3);

			}

			if (sinSkill.UseSkill[i].Flag && sinSkill.UseSkill[i].Skill_Info.Element[0])
			{
				DrawSprite(sinSkill.SkillBox[i].GageRect.left, sinSkill.SkillBox[i].GageRect.top + (256 - sinMoveKindInter[SIN_SKILL]), lpSkillGage5, 0, 0, 5, 35);
			}


		}
		if (DownButtonIndex) {  //버튼 다운 액션  
			DrawSprite(sinSkill.SkillBox[DownButtonIndex - 1].BoxRect.left + 1, sinSkill.SkillBox[DownButtonIndex - 1].BoxRect.top + 1 + (256 - sinMoveKindInter[SIN_SKILL]),
				lpSkillButton[DownButtonIndex - 1], 0, 0, 49, 46); //픽셀을 옆으로이동 

		}
		if (SkillButtonIndex) {
			if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag) {
				if (DownButtonIndex) {
					DrawSprite(sinSkill.SkillBox[SkillButtonIndex - 1].BoxRect.left + 1,
						sinSkill.SkillBox[SkillButtonIndex - 1].BoxRect.top + 1 + (256 - sinMoveKindInter[SIN_SKILL]),
						lpSelectSkill, 0, 0, 49, 46);

				}
				else {
					DrawSprite(sinSkill.SkillBox[SkillButtonIndex - 1].BoxRect.left,
						sinSkill.SkillBox[SkillButtonIndex - 1].BoxRect.top + (256 - sinMoveKindInter[SIN_SKILL]),
						lpSelectSkill, 0, 0, 49, 46);
				}
			}

		}

		//////////////////// ShorKey 
		for (i = 0; i < ChangeJobSkillPlus; i++) {
			if (sinSkill.UseSkill[i].Flag) {
				if (sinSkill.UseSkill[i].ShortKey) {
					DrawSprite(sinSkill.SkillBox[i].BoxRect.left + 9, sinSkill.SkillBox[i].BoxRect.top - 6 + (256 - sinMoveKindInter[SIN_SKILL]),
						lpShortKey[sinSkill.UseSkill[i].ShortKey - 1], 0, 0, 16, 16);

				}
			}
		}
	}
	//숙련도 표시 
	if (SkillMasteryShowFlag) {
		DrawSprite(UseSkillMasteryPosi.x, UseSkillMasteryPosi.y + (256 - sinMoveKindInter[SIN_SKILL]), lpMasteryInfo, 0, 0, 39, 27);
	}

	/////스킬 게이지는 인터페이스에서 그린다 
	if (sinSkill.pLeftSkill) {		 //왼쪽 스킬 
		if (sinSkill.pLeftSkill->UseSkillFlag)
			DrawSprite(sLeftRightSkill[0].BoxRect.left, sLeftRightSkill[0].BoxRect.top, //버튼 
				lpSkillButton[sinSkill.pLeftSkill->Position - 1], 0, 0, 49, 46);
		else
			DrawSprite(sLeftRightSkill[0].BoxRect.left, sLeftRightSkill[0].BoxRect.top, //버튼 
				lpSkillButton_Gray[sinSkill.pLeftSkill->Position - 1], 0, 0, 49, 46);


	}
	else { //스킬 추가가 되지않았을때 이미지 
		DrawSprite(sLeftRightSkill[0].BoxRect.left, sLeftRightSkill[0].BoxRect.top, //버튼 
			lpSkillBoxBack, 0, 0, 49, 46);

	}
	if (sinSkill.pRightSkill) {		//오른쪽 스킬  
		if (sinSkill.pRightSkill->UseSkillFlag) //사용가능 
			DrawSprite(sLeftRightSkill[1].BoxRect.left, sLeftRightSkill[1].BoxRect.top, //버튼 
				lpSkillButton[sinSkill.pRightSkill->Position - 1], 0, 0, 49, 46);
		else	//사용 불가능 
			DrawSprite(sLeftRightSkill[1].BoxRect.left, sLeftRightSkill[1].BoxRect.top, //버튼 
				lpSkillButton_Gray[sinSkill.pRightSkill->Position - 1], 0, 0, 49, 46);


	}
	else { //스킬 추가가되지않았을때 이미지 
		DrawSprite(sLeftRightSkill[1].BoxRect.left, sLeftRightSkill[1].BoxRect.top, //버튼 
			lpSkillBoxBack, 0, 0, 49, 46);

	}
}

void cSKILL::Main()
{
	int i = 0, j = 0;
	int k = 0, Num = 0;

	sSKILL  TempSkillData;
	CheckContinueIcon();
	memset(TempContinueSkill, 0, sizeof(sSKILL) * 20);

	for (k = 0; k < MAX_CONTINUE_SKILL; k++) {
		if (ContinueSkill[k].Flag) {
			if (ContinueSkill[k].MatIcon) {
				if (Num * 32 > ContinueSkill[k].IconPosiX) {
					ContinueSkill[k].IconPosiX += 2;
				}
				else if (Num * 32 < ContinueSkill[k].IconPosiX) {
					ContinueSkill[k].IconPosiX -= 2;

				}
				if (ContinueSkill[k].CheckTime < 255) {
					ContinueSkill[k].IconAlpha++;
				}
				if (ContinueSkill[k].CheckTime >= 255) {
					ContinueSkill[k].IconAlpha = 255;

				}
				if (((ContinueSkill[k].UseTime * 70) - ContinueSkill[k].CheckTime) < 180) {
					ContinueSkill[k].IconAlpha--;
				}
				if (ContinueSkill[k].IconAlpha > 255)ContinueSkill[k].IconAlpha = 255;
				if (ContinueSkill[k].IconAlpha < 0)ContinueSkill[k].IconAlpha = 0;

				if (ContinueSkill[k].CheckTime > (ContinueSkill[k].UseTime - 4) * 70) {
					ContinueSkill[k].IconTime++;
					if (ContinueSkill[k].IconTime > 30) {
						ContinueSkill[k].IconTime = 0;
						ContinueSkill[k].IconFlag ^= 1;
					}
				}

				memcpy(&TempContinueSkill[Num], &ContinueSkill[k], sizeof(sSKILL));
				Num++;

			}
		}
	}

	int TempXPosi = 0;
	for (i = Num; i > 0; i--)
	{
		for (j = 0; j < i; j++)
		{
			if (TempContinueSkill[j].CheckTime < TempContinueSkill[j + 1].CheckTime)
			{
				if (TempContinueSkill[j].CheckTime != 0) {
					TempSkillData = TempContinueSkill[j + 1];
					TempXPosi = TempContinueSkill[j + 1].IconPosiX;
					TempContinueSkill[j + 1] = TempContinueSkill[j];
					TempContinueSkill[j + 1].IconPosiX = TempXPosi;
					TempXPosi = TempContinueSkill[j].IconPosiX;
					TempContinueSkill[j] = TempSkillData;
					TempContinueSkill[j].IconPosiX = TempXPosi;
				}
			}
		}
	}


	if (AddVirtualLife[1]) {
		CheckVirtualLifeTime++;
		if (CheckVirtualLifeTime >= sinVirtualLifeTime * 70) 
		{
			CheckVirtualLifeTime = 0;
			AddVirtualLife[0] = 0;
			AddVirtualLife[1] = 0;
			sinVirtualLifePercent = 0;
		}
	}

	if (AddBoosterLife) {
		CheckBoosterLifeTime++;
		if (CheckBoosterLifeTime >= BoosterLifeTime * 70) {
			CheckBoosterLifeTime = 0;
			AddBoosterLife = 0;
			BoosterLifePercent = 0;
			tempLife[0] = 0;
			LifeFlag = 0;
		}
	}

	if (AddVirtualLife[1] && !AddBoosterLife)
	{
		for (i = 0; i < 10; i++)
		{
			if (ContinueSkill[i].Flag)
			{
				if (ContinueSkill[i].CODE == SKILL_VIRTUAL_LIFE)
				{
					AddVirtualLife[1] = ((int)sinChar->Life[1] * Virtual_Life_Percent[ContinueSkill[i].Point - 1]) / 100;
				}
			}
		}
	}
	if (AddVirtualMana[1]) {
		CheckVirtualManaTime++;
		if (CheckVirtualManaTime >= sinVirtualManaTime * 70) 
		{
			CheckVirtualManaTime = 0;
			AddVirtualMana[0] = 0;
			AddVirtualMana[1] = 0;
			sinVirtualManaPercent = 0;
			tempMana[0] = 0;
			ManaFlag = 0;
		}
	}

	if (AddVirtualStamina[1]) {
		CheckVirtualStaminaTime++;
		if (CheckVirtualStaminaTime >= sinVirtualStaminaTime * 70) 
		{
			CheckVirtualStaminaTime = 0;
			AddVirtualStamina[0] = 0;
			AddVirtualStamina[1] = 0;
			sinVirtualStaminaPercent = 0;
			tempStamina[0] = 0;
			StaminaFlag = 0;
		}
	}

	if (sinAddSkillDelay)
	{
		CheckSkillDelayTime++;
		if (CheckSkillDelayTime >= sinSkillDelayTime * 70)
		{
			CheckSkillDelayTime = 0;
			sinAddSkillDelay = 0;
			sinSkillDelayTime = 0;
			SkillDelayFlag = 0;
		}
	}

	sinCheckContinueSkill();
	CheckSkillData();

	if (!ClearSkillCODE || !CSKILL->OpenFlag || pCursorPos.y < 400) 
	{
		ClearSkillCODE = 0;
		SkillInfoShowFlag = 0;
		GAMECOREHANDLE->pcSkillInfoBox->Invalidate();
	}

	SkillMasteryShowFlag = 0;
	SelectInterSkill = 0;
	CheckingNowSkillState();
	//CheckSkillMastery();

	sinCheckChageJob(); 

	ShowSkillClose = 0; 
	SkillButtonIndex = 0;

	int Space = 0;
	sSKILLBOX TempSkillBox;
	ContinueSkillMouseIndex = 0;

	// Renderiza as skills na nova ui >>>>

	for (i = 0; i < MAX_CONTINUE_SKILL; i++)
	{
		if (TempContinueSkill[i].Flag) {
			if (TempContinueSkill[i].MatIcon) {
				if (10 + (Space * 32) < pCursorPos.x && 10 + (Space * 32) + 32 > pCursorPos.x &&
					10 < pCursorPos.y && 10 + 32 > pCursorPos.y)
				{
					if (MouseButton[2] && TempContinueSkill[i].CODE == SKILL_FORCE_ORB)
						Beep(300, 300);

					TempSkillBox.BoxRect.left = 10 + (Space * 32);
					TempSkillBox.BoxRect.top = 10 + 32;
					SkillInfoShowFlag = 2;
					ContinueSkillMouseIndex = i + 1;

					GAMECOREHANDLE->pcSkillInfoBox->PrepareShowSkill(&TempContinueSkill[i], TRUE, Rectangle2D(TempSkillBox.BoxRect.left, TempSkillBox.BoxRect.top, TempSkillBox.BoxRect.right, TempSkillBox.BoxRect.bottom));
				}
				Space++;
			}
		}
	}


	if (269 < pCursorPos.x && 287 > pCursorPos.x && 571 < pCursorPos.y && 590 > pCursorPos.y)
		ShowSkillClose = 1;

	if (OpenFlag && !SkillMain2OpenFlag) {
		for (int i = 0; i < ChangeJobSkillPlus; i++) {
			if (sinSkill.UseSkill[i].Flag) {
				if (sinSkill.SkillBox[i].BoxRect.left < pCursorPos.x && sinSkill.SkillBox[i].BoxRect.right > pCursorPos.x &&
					sinSkill.SkillBox[i].BoxRect.top < pCursorPos.y && sinSkill.SkillBox[i].BoxRect.bottom > pCursorPos.y) 
				{
					SkillButtonIndex = i + 1;
					sinSkill.UseSkill[i].Position = i + 1;

					sSKILL* pSkillCopy = new sSKILL();
					CopyMemory(pSkillCopy, &sinSkill.UseSkill[i], sizeof(sSKILL));

					int skillInfoOffset = GAMECOREHANDLE->pcSkillInfoBox->GetSkillInfoLevelOffset();

					if (pSkillCopy->Point == 0 && skillInfoOffset == 0 && SkillButtonIndex != 0)
					{
						skillInfoOffset = 1;
					}

					if (pSkillCopy->Point + skillInfoOffset < 1 ||
						pSkillCopy->Point + skillInfoOffset > 10)
					{
						skillInfoOffset = 0;
						GAMECOREHANDLE->pcSkillInfoBox->Invalidate();
					}

					pSkillCopy->Point = pSkillCopy->Point + skillInfoOffset;

					pSkillCopy->Mastery = pSkillCopy->Skill_Info.RequireMastery[0] + pSkillCopy->Skill_Info.RequireMastery[1] * pSkillCopy->Point - (pSkillCopy->UseSkillMastery / 100);
					if (pSkillCopy->Mastery <= 0) pSkillCopy->Mastery = 1;
					if (pSkillCopy->Mastery > MAX_MASTERY) pSkillCopy->Mastery = MAX_MASTERY;

					if (SkillButtonIndex != 0 && GAMECOREHANDLE->pcSkillInfoBox->IsDifferentSkill(pSkillCopy))
					{
						skillInfoOffset = sinSkill.UseSkill[i].Point == 0 ? 1 : 0;
					}

					GAMECOREHANDLE->pcSkillInfoBox->SetSkillInfoLevelOffset(skillInfoOffset);

					if (GAMECOREHANDLE->pcSkillInfoBox->CanPrepareShowSkill(pSkillCopy, Rectangle2D(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top, sinSkill.SkillBox[i].BoxRect.right, sinSkill.SkillBox[i].BoxRect.bottom)))
					{
						GAMECOREHANDLE->pcSkillInfoBox->PrepareShowSkill(pSkillCopy, FALSE, Rectangle2D(sinSkill.SkillBox[i].BoxRect.left, sinSkill.SkillBox[i].BoxRect.top, sinSkill.SkillBox[i].BoxRect.right, sinSkill.SkillBox[i].BoxRect.bottom));
					}

					SkillInfoShowFlag = 1;

					if (sinSkill.UseSkill[i].Point && SkillMasterFlag)
						SearchClearSkillPoint(sinSkill.UseSkill[i].CODE);

					SAFE_DELETE(pSkillCopy);

				}
				if (sinSkill.SkillBox[i].GageRect.left < pCursorPos.x && sinSkill.SkillBox[i].GageRect.right > pCursorPos.x &&
					sinSkill.SkillBox[i].GageRect.top < pCursorPos.y && sinSkill.SkillBox[i].GageRect.bottom > pCursorPos.y) {
					if (sinSkill.UseSkill[i].Skill_Info.USECODE != SIN_SKILL_USE_NOT) 
					{
						GetSkillMastery(&sinSkill.UseSkill[i], &sinSkill.SkillBox[i]);
						SkillMasteryShowFlag = 1;
					}
				}
			}
		}
	}
	for (i = 0; i < 2; i++) {
		if (sLeftRightSkill[i].BoxRect.left < pCursorPos.x && sLeftRightSkill[i].BoxRect.right > pCursorPos.x &&
			sLeftRightSkill[i].BoxRect.top < pCursorPos.y && sLeftRightSkill[i].BoxRect.bottom > pCursorPos.y) 
		{
			if (i == 0 && sinSkill.pLeftSkill) {
				cHelpPet.PetMessage("*LeftSkill", 0);

				GAMECOREHANDLE->pcSkillInfoBox->PrepareShowSkill(sinSkill.pLeftSkill, FALSE, Rectangle2D(sLeftRightSkill->BoxRect.left, sLeftRightSkill->BoxRect.top, sLeftRightSkill->BoxRect.right, sLeftRightSkill->BoxRect.bottom));

				SkillInfoShowFlag = 1;
				SelectInterSkill = 1;
			}
			if (i == 1 && sinSkill.pRightSkill) {
				cHelpPet.PetMessage("*RightSkill", 0);

				GAMECOREHANDLE->pcSkillInfoBox->PrepareShowSkill(sinSkill.pRightSkill, FALSE, Rectangle2D(sLeftRightSkill->BoxRect.left, sLeftRightSkill->BoxRect.top, sLeftRightSkill->BoxRect.right, sLeftRightSkill->BoxRect.bottom));

				SkillInfoShowFlag = 1;
				SelectInterSkill = 2;
			}

		}
	}
	if (OpenFlag && SkillMain2OpenFlag) 
	{
		if (12 < pCursorPos.x && 12 + 20 > pCursorPos.x &&
			482 < pCursorPos.y && 482 + 20 > pCursorPos.y) {
			SkillMain2ButtonFlag[0] = 1;
		}
		else {
			SkillMain2ButtonFlag[0] = 0;
		}

	}
	if (OpenFlag && !SkillMain2OpenFlag) 
	{
		if (765 < pCursorPos.x && 765 + 20 > pCursorPos.x &&
			482 < pCursorPos.y && 482 + 20 > pCursorPos.y) {
			SkillMain2ButtonFlag[1] = 1;
		}
		else {
			SkillMain2ButtonFlag[1] = 0;
		}
	}
}
void cSKILL::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/
void cSKILL::LButtonDown(int x, int y)
{

	for (int i = 0; i < 2; i++) {
		if (sLeftRightSkill[i].BoxRect.left < pCursorPos.x && sLeftRightSkill[i].BoxRect.right > pCursorPos.x &&
			sLeftRightSkill[i].BoxRect.top < pCursorPos.y && sLeftRightSkill[i].BoxRect.bottom > pCursorPos.y) {
			if (i == 0 && sinSkill.pLeftSkill) {
				cHelpPet.PetMessage("*LeftSkill", 1);
			}
			if (i == 1 && sinSkill.pRightSkill) {
				cHelpPet.PetMessage("*RightSkill", 1);
			}

		}
	}

	if (ClearSkillButtonPosi.x  < pCursorPos.x && ClearSkillButtonPosi.x + 45 > pCursorPos.x &&
		ClearSkillButtonPosi.y <  pCursorPos.y && ClearSkillButtonPosi.y + 45 > pCursorPos.y) {
		ClearSkillPointSection();
	}


	DownButtonUseShorKey = 0;
	LDownButtonIndex = 0;  

	if (ShowSkillClose) 
	{
		OpenFlag = 0;
	}

	if (SkillButtonIndex) 
	{
		if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag) 
		{
			LDownButtonIndex = SkillButtonIndex;
		}
	}

	DownButtonIndex = 0;

	if (ShowSkillClose) 
	{
		OpenFlag = 0;
	}
	if (SkillButtonIndex <= 13 && SkillMasterFlag == 1 && sinSkill.SkillPoint) 
	{
		if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag && sinSkill.UseSkill[SkillButtonIndex - 1].Use && sinSkill.UseSkill[SkillButtonIndex - 1].Point < 10)
		{
			DownButtonIndex = SkillButtonIndex;
			Skill4ButtonIndex = SkillButtonIndex;
		}
	}
	if (SkillButtonIndex > 13 && SkillMasterFlag == 1 && sinSkill.SkillPoint4) 
	{
		if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag && sinSkill.UseSkill[SkillButtonIndex - 1].Use && sinSkill.UseSkill[SkillButtonIndex - 1].Point < 10) {
			DownButtonIndex = SkillButtonIndex;
			Skill4ButtonIndex = SkillButtonIndex;
		}
	}

	if (SelectInterSkill) 
	{
		if (CSKILL->OpenFlag == SIN_CLOSE)CSKILL->OpenFlag = SIN_OPEN;
		else CSKILL->OpenFlag = SIN_CLOSE;
		cInterFace.CheckAllBox(SIN_SKILL);

		if (CSKILL->OpenFlag) 
		{
			if (!sinFireShow) 
			{
				StartMenuFlame(0, 350);
				sinPlaySound(0);
			}
			else
				sinPlaySound(SIN_SOUND_SHOW_INTER);
			sinFireShow = 1;
		}
	}

	if (SkillMain2ButtonFlag[1]) 
	{
		SkillMain2ButtonFlag[1] = 0;
		SkillMain2OpenFlag = 1;
		if (!lpSkillMain2)
			lpSkillMain2 = LoadDibSurfaceOffscreen("Image\\SinImage\\skill\\Skil25.bmp");
	}

	if (SkillMain2ButtonFlag[0]) 
	{
		SkillMain2ButtonFlag[0] = 0;
		SkillMain2OpenFlag = 0;
	}

}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/
void cSKILL::LButtonUp(int x, int y)
{

	DownButtonIndex = 0;
	LDownButtonIndex = 0; //버튼 이미지 정상화 
	if (SkillButtonIndex && !DownButtonUseShorKey) {
		if ((sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_LEFT ||
			sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_ALL) && !SkillMasterFlag
			&& sinSkill.UseSkill[SkillButtonIndex - 1].Point)
			sinSkill.pLeftSkill = &sinSkill.UseSkill[SkillButtonIndex - 1];

	}
	//	if(SkillButtonIndex == 1 )  //노멀 공격 이면 
	//		sinSkill.pLeftSkill = &sinSkill.UseSkill[SkillButtonIndex-1];

		//스킬을 분배한다 
	if (SkillButtonIndex != 1 && SkillMasterFlag == 1) {
		//4차 스킬 
		if (SkillButtonIndex > 13 && sinSkill.SkillPoint4 > 0) {
			if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag && sinSkill.UseSkill[SkillButtonIndex - 1].Use) {
				if (sinSkill.UseSkill[SkillButtonIndex - 1].Point < MAX_USE_SKILL_POINT) { //최대 스킬 포인트 치를 못넘는다 
					///////////////여기서 메세지 박스를 띄워주고 돈을 지불하고 스킬을 올려준다 

					pMasterSkill = &sinSkill.UseSkill[SkillButtonIndex - 1];
					cMessageBox.ShowMessage3(MESSAGE_MASTER_SKILL, sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.SkillName);
					ShowSkillUpInfo = 1;

				}
			}
		}
		//1.2.3차 스킬
		if (SkillButtonIndex <= 13 && sinSkill.SkillPoint > 0) {
			if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag && sinSkill.UseSkill[SkillButtonIndex - 1].Use) {
				if (sinSkill.UseSkill[SkillButtonIndex - 1].Point < MAX_USE_SKILL_POINT) { //최대 스킬 포인트 치를 못넘는다 
					///////////////여기서 메세지 박스를 띄워주고 돈을 지불하고 스킬을 올려준다 

					pMasterSkill = &sinSkill.UseSkill[SkillButtonIndex - 1];
					cMessageBox.ShowMessage3(MESSAGE_MASTER_SKILL, sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.SkillName);
					ShowSkillUpInfo = 1;

				}
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/
void cSKILL::RButtonDown(int x, int y)
{
	for (int i = 0; i < 2; i++) {
		if (sLeftRightSkill[i].BoxRect.left < pCursorPos.x && sLeftRightSkill[i].BoxRect.right > pCursorPos.x &&
			sLeftRightSkill[i].BoxRect.top < pCursorPos.y && sLeftRightSkill[i].BoxRect.bottom > pCursorPos.y) {
			if (i == 0 && sinSkill.pLeftSkill) {
				cHelpPet.PetMessage("*LeftSkill", 2);
			}
			if (i == 1 && sinSkill.pRightSkill) {
				cHelpPet.PetMessage("*RightSkill", 2);
			}

		}
	}

	DownButtonUseShorKey = 0;
	if (SkillButtonIndex) {
		if (sinSkill.UseSkill[SkillButtonIndex - 1].Flag) {
			RDownButtonIndex = SkillButtonIndex;
		}
	}
}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/
void cSKILL::RButtonUp(int x, int y)
{
	RDownButtonIndex = 0;
	LDownButtonIndex = 0; //버튼 이미지 정상화 
	if (SkillButtonIndex && !DownButtonUseShorKey) {
		if ((sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_RIGHT ||
			sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_ALL) && !SkillMasterFlag &&
			sinSkill.UseSkill[SkillButtonIndex - 1].Point)
			sinSkill.pRightSkill = &sinSkill.UseSkill[SkillButtonIndex - 1];
	}

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/
void cSKILL::KeyDown()
{
	//	sSkill_Info[0].FuncPointer();


	int i = 0;
	char szKeyBuff[8] = { VK_F1,VK_F2,VK_F3,VK_F4,VK_F5,VK_F6,VK_F7,VK_F8 }; //키버퍼 
	for (i = 0; i < 8; i++) {
		if (sinGetKeyClick(szKeyBuff[i])) {
			if (SkillButtonIndex) {
				//if(SkillButtonIndex == 1)break; //주먹키
				if (LDownButtonIndex) {
					if (sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_LEFT ||
						sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_ALL &&
						sinSkill.UseSkill[SkillButtonIndex - 1].Point && !SkillMasterFlag) {
						sinSkill.UseSkill[SkillButtonIndex - 1].ShortKey = i + 1;
						sinSkill.UseSkill[SkillButtonIndex - 1].MousePosi = SIN_MOUSE_POSI_LEFT;
						DownButtonUseShorKey = 1;
					}
				}
				else {
					if (RDownButtonIndex) {
						if (sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_RIGHT ||
							sinSkill.UseSkill[SkillButtonIndex - 1].Skill_Info.USECODE == SIN_SKILL_USE_ALL &&
							sinSkill.UseSkill[SkillButtonIndex - 1].Point && !SkillMasterFlag) {
							sinSkill.UseSkill[SkillButtonIndex - 1].ShortKey = i + 1;
							sinSkill.UseSkill[SkillButtonIndex - 1].MousePosi = SIN_MOUSE_POSI_RIGHT;
							DownButtonUseShorKey = 1;
						}
					}

				}

				for (int j = 0; j < 17; j++) {
					if (j == SkillButtonIndex - 1)continue;
					if (sinSkill.UseSkill[SkillButtonIndex - 1].ShortKey == sinSkill.UseSkill[j].ShortKey) {
						sinSkill.UseSkill[j].ShortKey = 0;
					}
				}
			}
			else {// 단축키로 스킬 사용
				for (int j = 0; j < SIN_MAX_USE_SKILL; j++) {
					if (i == sinSkill.UseSkill[j].ShortKey - 1) {
						if (sinSkill.UseSkill[j].Point) {
							if (sinSkill.UseSkill[j].MousePosi == SIN_MOUSE_POSI_LEFT) { //왼쪽
								sinSkill.pLeftSkill = &sinSkill.UseSkill[j];

							}
							if (sinSkill.UseSkill[j].MousePosi == SIN_MOUSE_POSI_RIGHT) { //오른쪽 
								sinSkill.pRightSkill = &sinSkill.UseSkill[j];
							}
						}
					}
				}
			}
		}
	}
	if (sinGetKeyClick('S')) {
		cHelpPet.PetMessage("*Skill", 3);
		if (CSKILL->OpenFlag)CSKILL->OpenFlag = SIN_CLOSE;
		else CSKILL->OpenFlag = SIN_OPEN;
		cInterFace.CheckAllBox(SIN_SKILL); //창을 하나만 띄운다 
		if (CSKILL->OpenFlag) {
			if (!sinFireShow) {
				StartMenuFlame(0, 350);
				sinPlaySound(0);
			}
			else
				sinPlaySound(SIN_SOUND_SHOW_INTER);
			sinFireShow = 1;
		}
	}

}


int cSKILL::SearchUseSkill()
{

	switch (sinChar->JOB_CODE) {
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
	if (!UseSkillBox)
		return FALSE;

	int Index = 0;
	memcpy(&sinSkill.UseSkill[0], &sSkill[0], sizeof(sSKILL));
	sinSkill.UseSkill[0].Flag = 1; //노말 어택 default설정 
	sinSkill.UseSkill[0].Use = 1; //노말 어택 default설정 
	sinSkill.UseSkill[0].Skill_Info.USECODE = SIN_SKILL_USE_ALL;  //노말 어택 강제설정 
	sinSkill.UseSkill[0].Point = 1;

	for (int i = 0; i < SIN_MAX_SKILL; i++) {
		if ((sSkill[i].CODE & sinSKILL_MASK1) == UseSkillBox) { //자신에 맞는 스킬을 찾는다 
			for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
				if (!sinSkill.UseSkill[j].Flag) {
					memcpy(&sinSkill.UseSkill[j], &sSkill[i], sizeof(sSKILL));
					for (int k = 0; k < SIN_MAX_SKILL; k++) {
						if (sinSkill.UseSkill[j].CODE == sSkill_Info[k].CODE) {
							memcpy(&sinSkill.UseSkill[j].Skill_Info, &sSkill_Info[k], sizeof(sSKILL_INFO));
							sinSkill.UseSkill[j].Skill_Info.SkillNum = j - 1;
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
	memcpy(&sinSkill.SkillBox, &sSkillBox, sizeof(sSKILLBOX) * SIN_MAX_USE_SKILL); //좌표를 복사한다 

	LoadUseSkillImage(); //스킬이미지를 로드한다 (전업을 할때두 로드해준다 )

	sinSkill.UseSkill[0].Use = 1; //노말 어택 default설정 
	sinSkill.UseSkill[0].Skill_Info.USECODE = SIN_SKILL_USE_ALL;  //노말 어택 강제설정 
	sinSkill.UseSkill[0].Point = 1;
	ReFormCharSkillInfo(); //스킬정보를 갱신한다 (해킹방지)
	return FALSE;
}

int JobSkillTemp = 0;

void cSKILL::CheckingNowSkillState()
{

	//노말스킬을 포함한 4,4,4,4
	switch (sinChar->ChangeJob) {
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
	if (JobSkillTemp != ChangeJobSkillPlus) {	//전업데이타가 나중에 들어와서 날치기 땜빵으로 보정했다  음하하 
		JobSkillTemp = ChangeJobSkillPlus;
		cInvenTory.SetItemToChar();
	}

	for (int j = 0; j < ChangeJobSkillPlus; j++) {
		if (j > 12) {
			if (sinSkill.SkillPoint4) { //4차스킬 포인트가 있을경우에만 활성화 시킨다 
				sinSkill.UseSkill[j].Flag = 1; //사용할수있는 스킬을 추가한다 
				if (sinSkill.UseSkill[j].Point <= MAX_USE_SKILL_POINT) {
					if (sinSkill.UseSkill[j - 1].Point) {//트리를 탄다 
						if (sinSkill.UseSkill[j].Skill_Info.RequireLevel + (sinSkill.UseSkill[j].Point * 2) <= sinChar->Level) //스킬을 사용할수있는 레벨인지 판다 
							sinSkill.UseSkill[j].Use = 1;
						else
							sinSkill.UseSkill[j].Use = 0;
					}
				}
			}
			else { //없을경우는 다시 비활성화 
				sinSkill.UseSkill[j].Use = 0;
			}
			continue;
		}
		if (sinSkill.SkillPoint) { //스킬 포인트가 있을경우에만 활성화 시킨다 
			sinSkill.UseSkill[j].Flag = 1; //사용할수있는 스킬을 추가한다 
			if (sinSkill.UseSkill[j].Point <= MAX_USE_SKILL_POINT) {
				if (j == 0)continue; //노말 스킬 
				if (sinSkill.UseSkill[j - 1].Point) {//트리를 탄다 
					if (sinSkill.UseSkill[j].Skill_Info.RequireLevel + (sinSkill.UseSkill[j].Point * 2) <= sinChar->Level) //스킬을 사용할수있는 레벨인지 판다 
						sinSkill.UseSkill[j].Use = 1;
					else
						sinSkill.UseSkill[j].Use = 0;
				}
			}
		}
		else 
		{
			sinSkill.UseSkill[j].Use = 0;
		}
	}
}


void cSKILL::CheckSkillMastery()
{
	int TempLenght = 0;
	int TempTalent = 0;

	if (sinSkill.pLeftSkill) {
		if (!sinSkill.pLeftSkill->Point)
			sinSkill.pLeftSkill->UseSkillFlag = 0;
	}
	if (sinSkill.pRightSkill) {
		if (!sinSkill.pRightSkill->Point)
			sinSkill.pRightSkill->UseSkillFlag = 0;
	}

	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) 
	{
		if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point) 
		{
			CheckSkillMasteryForm(0, j);

			if (sinSkill.UseSkill[j].Mastery == 0) {
				sinSkill.UseSkill[j].GageLength = 0;
				sinSkill.UseSkill[j].GageLength2 = 0;
				SkillCountTime2[j] = 0;

			}

			TempTalent = (int)(sinChar->Talent / 3) + (int)sinAdd_fMagic_Mastery;
			if (TempTalent > 50)TempTalent = 50;
			sinSkill.UseSkill[j].UseSkillMastery = (int)(TempTalent * 100) + sinSkill.UseSkill[j].UseSkillCount;
			if (sinSkill.UseSkill[j].Skill_Info.Element[0]) 
			{ 
				sinSkill.UseSkill[j].UseSkillMastery = 10000;

			}

			if (sinSkill.UseSkill[j].UseSkillMastery >= 10000)sinSkill.UseSkill[j].UseSkillMastery = 10000;
			if (sinSkill.UseSkill[j].UseSkillMastery) {
				sinSkill.UseSkill[j].UseSkillMasteryGage = (int)((35 * ((float)sinSkill.UseSkill[j].UseSkillMastery / 10000)));
				if (sinSkill.UseSkill[j].UseSkillMasteryGage > 35)sinSkill.UseSkill[j].UseSkillMasteryGage = 35;
			}

			sinSkill.UseSkill[j].Mastery = (sinSkill.UseSkill[j].Skill_Info.RequireMastery[0] + (sinSkill.UseSkill[j].Skill_Info.RequireMastery[1] * sinSkill.UseSkill[j].Point) -
				(sinSkill.UseSkill[j].UseSkillMastery / 100));
			if (sinSkill.UseSkill[j].Mastery <= 0) sinSkill.UseSkill[j].Mastery = 1;
			if (sinSkill.UseSkill[j].Mastery > 70)sinSkill.UseSkill[j].Mastery = 70;

			if (sinAddSkillDelay)
			{
				sinSkill.UseSkill[j].Mastery = 1;
			}

			else
			{
				sinSkill.UseSkill[j].Mastery = (sinSkill.UseSkill[j].Skill_Info.RequireMastery[0] + (sinSkill.UseSkill[j].Skill_Info.RequireMastery[1] * sinSkill.UseSkill[j].Point) -
					(sinSkill.UseSkill[j].UseSkillMastery / 100));

				if (sinSkill.UseSkill[j].Mastery <= 0) sinSkill.UseSkill[j].Mastery = 1;
				if (sinSkill.UseSkill[j].Mastery > 70)sinSkill.UseSkill[j].Mastery = 70;
			}

			TempLenght = (int)(35 / ((float)sinSkill.UseSkill[j].Mastery / 2));

			sinSkill.UseSkill[j].GageLength2 += TempLenght;

			if (sinSkill.UseSkill[j].GageLength < sinSkill.UseSkill[j].GageLength2) {
				SkillCountTime2[j]++;
				if (SkillCountTime2[j] >= (int)(35 / (float)TempLenght)) {
					sinSkill.UseSkill[j].GageLength++;
					SkillCountTime2[j] = 0;

				}

			}

			if (sinSkill.UseSkill[j].GageLength >= 35) 
			{
				// Skill pronta para o uso
				sinSkill.UseSkill[j].GageLength = 35;
				sinSkill.UseSkill[j].GageLength2 = 35;
				sinSkill.UseSkill[j].UseSkillFlag = 1;
				CheckUseSkillState(&sinSkill.UseSkill[j]);
			}
			else
			{
			//	CHATBOX->AddDebugMessage("Gage: %d - Gage2: %d - Temp: %d", sinSkill.UseSkill[j].GageLength, sinSkill.UseSkill[j].GageLength2, TempLenght);
				sinSkill.UseSkill[j].UseSkillFlag = 0; // Skill em delay
			}

			ReformSkillMasteryForm(0, j);

		}

	}
}

DWORD UseSkillItemInfo[10] = { 0,sinWA1,sinWM1,sinWH1,sinDS1,sinWP1,sinWS2,sinWC1,sinWS1,sinWT1 };

int cSKILL::GetInfoBoxSize(sSKILLBOX* pSkillBox, int Col, int Icon)
{
	int ty, tx;

	SkillBoxSize.x = 16;
	SkillBoxSize.y = Col + 1;

	SkillInfoBoxPosi.x = (pSkillBox->BoxRect.left - 112) + 20;
	SkillInfoBoxPosi.y = pSkillBox->BoxRect.top - (SkillBoxSize.y * 16);

	if (Icon) {
		SkillInfoBoxPosi.x = pSkillBox->BoxRect.left;
		SkillInfoBoxPosi.y = pSkillBox->BoxRect.top;

	}

	ty = SkillInfoBoxPosi.y + (SkillBoxSize.y * 16);
	tx = SkillInfoBoxPosi.x + (SkillBoxSize.x * 16);

	if (SkillInfoBoxPosi.y < 0)
		SkillInfoBoxPosi.y = 0;
	if (SkillInfoBoxPosi.x < 0)
		SkillInfoBoxPosi.x = 0;

	if (ty > 600)
		SkillInfoBoxPosi.y -= (ty - 600);
	if (tx > 800)
		SkillInfoBoxPosi.x -= (tx - 800);

	return TRUE;
}


int cSKILL::GetSkillMastery(sSKILL* pSkill, sSKILLBOX* pSkillBox)
{
	int SkillMasteryTemp = 0;
	SkillMasteryTemp = ((int)pSkill->UseSkillMastery / 100);
	wsprintf(szUseSkillMastery, "%d", SkillMasteryTemp);

	UseSkillMasteryPosi.x = pSkillBox->GageRect.left - (34 / 2);
	UseSkillMasteryPosi.y = pSkillBox->GageRect.top - 29;

	return TRUE;
}


int cSKILL::CheckUseSkillState(sSKILL* pSkill)
{
	if (pSkill->Skill_Info.UseMana)
	{
		if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1 && chaPremiumitem.m_StaminaReducePotiontime > 0)
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - ((pSkill->Skill_Info.UseMana[pSkill->Point - 1] *
				chaPremiumitem.m_ManaReducePotionValue / 100) + (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * 20 / 100)) &&
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) - ((pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;
			}

			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 마나리듀스, 근력리듀스
		else if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_StaminaReducePotiontime > 0)
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) &&
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) - ((pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 테리, 근력리듀스
		else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1 && chaPremiumitem.m_StaminaReducePotiontime > 0)
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * 20 / 100) &&
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) - ((pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 마나리듀스
		else if (chaPremiumitem.m_ManaReducePotiontime > 0)
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * chaPremiumitem.m_ManaReducePotionValue / 100) &&
				sinGetStamina() >= pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1)))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 테리
		else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] - (pSkill->Skill_Info.UseMana[pSkill->Point - 1] * 20 / 100) &&
				sinGetStamina() >= pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1)))
			{
				pSkill->UseSkillFlag = 1;
			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 장별 - 근력리듀스
		else if (chaPremiumitem.m_StaminaReducePotiontime > 0)
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] &&
				sinGetStamina() >= (pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) - ((pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1))) * chaPremiumitem.m_StaminaReducePotionValue / 100))
			{
				pSkill->UseSkillFlag = 1;

			}
			else
				pSkill->UseSkillFlag = 0;
		}

		// 아이템 사용 안했다
		else
		{
			if (sinGetMana() >= pSkill->Skill_Info.UseMana[pSkill->Point - 1] &&
				sinGetStamina() >= pSkill->Skill_Info.UseStamina[0] + (pSkill->Skill_Info.UseStamina[1] * (pSkill->Point - 1)))
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
int cSKILL::InitSkillPoint(int kind)
{
	//kind 0 이벤트 초기화kind 1레드스톤 초기화
	switch (kind) {
	case 0:
		if (!sinInitPointPassFlag) {
			if (sinChar->wVersion[1] == 1) {
				cMessageBox.ShowMessage(MESSAGE_ALREADY_INITSTATE);
				return FALSE;
			}
		}
		break;
	}
	sinInitPointPassFlag = 0;

	CheckingNowSkillState(); //스킬포인트를 
	CheckCharForm();//인증 
	int Count = 0;
	int Count2 = 0;

	for (int j = 1; j < ChangeJobSkillPlus; j++) {
		if (j > 12) {
			if (sinSkill.UseSkill[j].Point > 0)
				Count2 += sinSkill.UseSkill[j].Point;
		}
		else {
			if (sinSkill.UseSkill[j].Point > 0)
				Count += sinSkill.UseSkill[j].Point;
		}

		CSKILL->CheckSkillMasteryForm(0, j); // 스킬 마스터리를 체크한다
		CheckSkillPointForm(&sinSkill.UseSkill[j]);
		sinSkill.UseSkill[j].Point = 0; //스킬포인트 초기화 
		sinSkill.UseSkill[j].GageLength = 0;
		sinSkill.UseSkill[j].GageLength2 = 0;
		sinSkill.UseSkill[j].Mastery = 0;
		sinSkill.UseSkill[j].UseSKillIncreCount = 0;
		sinSkill.UseSkill[j].UseSkillCount = 0;
		sinSkill.UseSkill[j].UseSkillMastery = 0;
		sinSkill.UseSkill[j].UseSkillMasteryGage = 0;
		sinSkill.UseSkill[j].Use = 0;
		ReformSkillPointForm(&sinSkill.UseSkill[j]);
		ReformSkillMasteryForm(0, j);

	}
	sinSkill.SkillPoint += Count;
	sinSkill.SkillPoint4 += Count2;
	sinChar->wVersion[1] = 1;
	ReformCharForm();//재인증 
	cMessageBox.ShowMessage(MESSAGE_GYUNGFUM_OK);
	cInvenTory.SetItemToChar();

	return TRUE;
}

//DWORD ClearSkillPointCODE[10] = {SKILL_HOLY_BODY , SKILL_HOLY_MIND , 0,0,0,0,0,0,0,0};
DWORD ClearSkillPointCODE[10] = { 0,0,0,0,0,0,0,0,0,0 }; //부분 스킬 초기화 

int cSKILL::SearchClearSkillPoint(DWORD CODE)
{

	for (int i = 0; i < 10; i++) {
		if (ClearSkillPointCODE[i] == CODE) {
			ClearSkillCODE = ClearSkillPointCODE[i];
			return TRUE;
		}
		else
			ClearSkillCODE = 0;
	}

	return FALSE;

}

////////////// 초기화할 스킬포인트에 초기화 버튼을 띄운다 
int cSKILL::ShowClearSkillPointButton(int x, int y)
{
	ClearSkillButtonPosi.x = -50;
	ClearSkillButtonPosi.y = -50;

	if (ClearSkillCODE && !sinMessageBoxShowFlag) {
		ClearSkillButtonPosi.x = x - 57;
		ClearSkillButtonPosi.y = y - 51;
		DrawSprite(ClearSkillButtonPosi.x, ClearSkillButtonPosi.y, lpSkillDown, 0, 0, 50, 44);
	}

	return TRUE;
}

////////////// 특정 스킬포인트를 초기화한다 
int cSKILL::ClearSkillPointSection()
{

	CheckingNowSkillState(); //스킬포인트를 
	CheckCharForm();//인증 
	int Count = 0;
	for (int j = 1; j < ChangeJobSkillPlus; j++) {
		if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point > 1) {
			for (int i = 0; i < 10; i++) {
				if (sinSkill.UseSkill[j].CODE == ClearSkillPointCODE[i]) {
					CSKILL->CheckSkillMasteryForm(0, j); // 스킬 마스터리를 체크한다
					CheckSkillPointForm(&sinSkill.UseSkill[j]);
					sinSkill.UseSkill[j].Point--; //스킬포인트 초기화 
					sinSkill.UseSkill[j].GageLength = 0;
					sinSkill.UseSkill[j].GageLength2 = 0;
					sinSkill.UseSkill[j].Mastery = 0;
					sinSkill.UseSkill[j].UseSKillIncreCount = 0;
					sinSkill.UseSkill[j].UseSkillCount = 0;
					sinSkill.UseSkill[j].UseSkillMastery = 0;
					sinSkill.UseSkill[j].UseSkillMasteryGage = 0;
					sinSkill.UseSkill[j].Use = 0;
					ReformSkillPointForm(&sinSkill.UseSkill[j]);
					ReformSkillMasteryForm(0, j);
					sinSkill.SkillPoint++;
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


unsigned int CharSkillInfoData[8] = { 0, }; //캐릭정보 CheckSum이셋팅될 배열 
int   ChangeJob = -1;
//캐릭터별 SkillInfo를 갱신한다 
int cSKILL::ReFormCharSkillInfo()
{
	int   i = 0, j = 0, k = 0;
	int   CharNum = 0;

	int   CharCode[8] = { SIN_CHAR_MECANICIAN,SIN_CHAR_FIGHTER,SIN_CHAR_ARCHER,SIN_CHAR_PIKEMAN,SIN_CHAR_KNIGHT,SIN_CHAR_ATALANTA,SIN_CHAR_PRIESTESS,SIN_CHAR_MAGICIAN };
	//자신의 직업을 구한다 
	for (i = 0; i < 8; i++) {
		if (sinChar->JOB_CODE == CharCode[i]) {
			CharNum = i;
			break;
		}
	}

	if (ChangeJob != sinChar->ChangeJob) {
		ChangeJob = sinChar->ChangeJob;
		CharSkillInfoData[CharNum] = 0; //초기화 
		for (j = 1; j < SIN_MAX_USE_SKILL; j++) {
			if (sinSkill.UseSkill[j].Flag) {
				k = j + 1;
				CharSkillInfoData[CharNum] += k * sinSkill.UseSkill[j].Skill_Info.RequireLevel;
				CharSkillInfoData[CharNum] += k * sinSkill.UseSkill[j].Skill_Info.UseStamina[0];
				CharSkillInfoData[CharNum] += k * sinSkill.UseSkill[j].Skill_Info.UseStamina[1];
			}
		}
	}
	return TRUE;
}

int cSKILL::CheckCharSkillInfo()
{
	int   i = 0, j = 0, k = 0;
	int   CharNum = 0;
	int   CharCode[8] = { SIN_CHAR_MECANICIAN,SIN_CHAR_FIGHTER,SIN_CHAR_ARCHER,SIN_CHAR_PIKEMAN,SIN_CHAR_KNIGHT,SIN_CHAR_ATALANTA,SIN_CHAR_PRIESTESS,SIN_CHAR_MAGICIAN };

	unsigned int CharSkillInfoDataCheckSum[8] = { 0, };

	for (i = 0; i < 8; i++) 
	{
		if (sinChar->JOB_CODE == CharCode[i]) 
		{
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

int cSKILL::CheckSkillMasteryForm(sSKILL* pSkill, int Index)
{
	if (pSkill) {
		for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
			if (sinSkill.UseSkill[j].Flag) {
				if (pSkill->CODE == sinSkill.UseSkill[j].CODE) {
					Index = j;
					break;
				}
			}
		}
	}

	int CheckMasteryDataSum = lpCurPlayer->dwObjectSerial + sinSkill.UseSkill[Index].CODE;

	CheckMasteryDataSum += (sinSkill.UseSkill[Index].GageLength * Index);
	CheckMasteryDataSum += (sinSkill.UseSkill[Index].GageLength2 * Index);
	CheckMasteryDataSum += (sinSkill.UseSkill[Index].Mastery * Index);

	if (CheckMasteryData[Index] != CheckMasteryDataSum) 
	{
		SendSetHackUser3(52, CheckMasteryData[Index], CheckMasteryDataSum);
		return FALSE;

	}

	return TRUE;
}
int cSKILL::ReformSkillMasteryForm(sSKILL* pSkill, int Index)
{

	if (pSkill) {
		for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
			if (sinSkill.UseSkill[j].Flag) {
				if (pSkill->CODE == sinSkill.UseSkill[j].CODE) {
					Index = j;
				}
			}
		}
	}

	CheckMasteryData[Index] = lpCurPlayer->dwObjectSerial + sinSkill.UseSkill[Index].CODE;
	CheckMasteryData[Index] += (sinSkill.UseSkill[Index].GageLength * Index);
	CheckMasteryData[Index] += (sinSkill.UseSkill[Index].GageLength2 * Index);
	CheckMasteryData[Index] += (sinSkill.UseSkill[Index].Mastery * Index);

	return TRUE;
}

int cSKILL::CheckSkillPointForm(sSKILL* pSkill)
{
	DWORD	dwXorKey = lpCurPlayer->dwObjectSerial & 0x5F;

	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) 
	{
		if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Use && sinSkill.UseSkill[j].Point) 
		{
			if (pSkill->CODE == sinSkill.UseSkill[j].CODE) 
			{
				if (CheckSkillPointData[j] != (sinSkill.UseSkill[j].Point ^ dwXorKey)) {
					SendSetHackUser3(54, CheckSkillPointData[j], sinSkill.UseSkill[j].Point);
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

int cSKILL::ReformSkillPointForm(sSKILL* pSkill)
{
	DWORD	dwXorKey = lpCurPlayer->dwObjectSerial & 0x5F;

	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		if (sinSkill.UseSkill[j].Flag) {
			if (pSkill->CODE == sinSkill.UseSkill[j].CODE) {
				CheckSkillPointData[j] = (sinSkill.UseSkill[j].Point ^ dwXorKey);
				break;
			}
		}
	}
	return TRUE;
}

int cSKILL::InitReformSkillPointForm()
{
	return TRUE;
}

int cSKILL::GetSkillPoint(DWORD CODE)
{
	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		if (sinSkill.UseSkill[j].Flag) {
			if (CODE == sinSkill.UseSkill[j].CODE) {
				return sinSkill.UseSkill[j].Point;
			}
		}
	}

	return 0;
}

int cSKILL::SearchContiueSkill(DWORD CODE)
{
	for (int k = 0; k < 18; k++) {
		if (ContinueSkill[k].Flag) {
			if (ContinueSkill[k].CODE == CODE) {
				return ContinueSkill[k].Point;

			}
		}
	}
	return 0;
}

int cSKILL::SearchContiueSkillIndex(DWORD CODE)
{
	for (int k = 0; k < 18; k++) {
		if (ContinueSkill[k].Flag) {
			if (ContinueSkill[k].CODE == CODE) {
				return k + 1;
			}
		}
	}
	return 0;
}

int cSKILL::SearchContiueSkillCODE(DWORD CODE)
{
	for (int k = 0; k < 18; k++) {
		if (ContinueSkill[k].Flag) {
			if (ContinueSkill[k].CODE == CODE) {
				return ContinueSkill[k].CODE;
			}
		}
	}
	return 0;
}

int AddVirtualLife[2] = { 0,0 };
int sinVirtualLifeTime = 0;
int sinVirtualLifePercent = 0;
int AddBoosterLife = 0;
int BoosterLifeTime = 0;
int BoosterLifePercent = 0;
int LifeFlag = 0;
short tempLife[2] = { 0,0 };
int AddVirtualMana[2] = { 0,0 };
int sinVirtualManaTime = 0;
int sinVirtualManaPercent = 0;
int ManaFlag = 0;
short tempMana[2] = { 0,0 };
int AddVirtualStamina[2] = { 0,0 };
int sinVirtualStaminaTime = 0;
int sinVirtualStaminaPercent = 0;
int StaminaFlag = 0;
short tempStamina[2] = { 0,0 };
int sinAddSkillDelay = 0;
int sinSkillDelayTime = 0;
int SkillDelayFlag = 0;

int cSKILL::SetVirtualLife(int Time, int Point)
{
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) 
		{
			if (ContinueSkill[i].CODE == SKILL_VIRTUAL_LIFE) 
			{
				return FALSE;
			}
		}
	}

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (!ContinueSkill[i].Flag) {
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_VIRTUAL_LIFE;
			ContinueSkill[i].UseTime = Time;
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}

	AddVirtualLife[1] = ((int)sinChar->Life[1] * Virtual_Life_Percent[Point - 1]) / 100;
	sinVirtualLifeTime = Time;
	sinVirtualLifePercent = Virtual_Life_Percent[Point - 1];
	CheckVirtualLifeTime = 0; //체크시간초기화
	cInvenTory.SetItemToChar();
	return TRUE;
}

// 박재원 - 부스터 아이템
int cSKILL::UseBoosterItem(sITEM* pItem)
{
	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
		return FALSE; //마을에서는 못사용한다

	DWORD BoosterCODE;
	DWORD ItemMask;
	int BoosterTime = 0;
	int cnt = 0;

	ItemMask = pItem->CODE & sinITEM_MASK3;
	if (ItemMask == sin21 || ItemMask == sin24 || ItemMask == sin27) cnt = 0;
	if (ItemMask == sin22 || ItemMask == sin25 || ItemMask == sin28) cnt = 1;
	if (ItemMask == sin23 || ItemMask == sin26 || ItemMask == sin29) cnt = 2;

	switch (ItemMask) {
	case sin21: // 생명력 - 1시간
	case sin22: // 생명력 - 3시간
	case sin23: // 생명력 - 1일
		BoosterCODE = BOOSTER_ITEM_LIFE;
		BoosterTime = BoosterItem_UseTime[cnt];

		AddBoosterLife = ((int)sinChar->Life[1] * BoosterItem_DataPercent[0]) / 100;
		BoosterLifeTime = BoosterTime;
		BoosterLifePercent = BoosterItem_DataPercent[0];
		CheckBoosterLifeTime = 0; //체크시간초기화
		break;
	case sin24: // 기력 - 1시간
	case sin25: // 기력 - 3시간
	case sin26: // 기력 - 1일
		BoosterCODE = BOOSTER_ITEM_MANA;
		BoosterTime = BoosterItem_UseTime[cnt];

		// 박재원 - 부스터 아이템(기력)
		AddVirtualMana[1] = ((int)sinChar->Mana[1] * BoosterItem_DataPercent[1]) / 100;
		sinVirtualManaTime = BoosterTime;
		sinVirtualManaPercent = BoosterItem_DataPercent[1];
		CheckVirtualManaTime = 0; //체크시간초기화
		break;
	case sin27: // 근력 - 1시간
	case sin28: // 근력 - 3시간
	case sin29: // 근력 - 1일
		BoosterCODE = BOOSTER_ITEM_STAMINA;
		BoosterTime = BoosterItem_UseTime[cnt];

		// 박재원 - 부스터 아이템(근력)
		AddVirtualStamina[1] = ((int)sinChar->Stamina[1] * BoosterItem_DataPercent[2]) / 100;
		sinVirtualStaminaTime = BoosterTime;
		sinVirtualStaminaPercent = BoosterItem_DataPercent[2];
		CheckVirtualStaminaTime = 0; //체크시간초기화
		break;
	}
	//해당 돼는 아이템이 맞으면 아이템스킬을 세팅한다.
	sSKILL TempSkill;

	if (CSKILL->SearchContiueSkillCODE(BoosterCODE) == BoosterCODE && BoosterCODE != 0) {
		cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
		haCastleSkillUseFlag = 0;
		return TRUE;
	}
	haCastleSkillUseFlag = 1;

	for (int j = 0; j < SIN_MAX_SKILL; j++) {
		if (sSkill[j].CODE == BoosterCODE) {
			memcpy(&TempSkill, &sSkill[j], sizeof(sSKILL));
			TempSkill.UseTime = BoosterTime;
			sinContinueSkillSet(&TempSkill);
			SwitchSkill(&TempSkill);
			break;
		}
	}

	tempLife[1] = sinChar->Level;
	tempMana[1] = sinChar->Level;
	tempStamina[1] = sinChar->Level;

	sinPlaySound(SIN_SOUND_EAT_POTION2);//대박 사운드
	cInvenTory.SetItemToChar();
	return TRUE;
}

// 박재원 - 데미지 부스터를 셋팅한다
int cSKILL::SetBoosterItem(DWORD ItemCODE, int Time)
{
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));
	char* Path;

	DWORD ItemMask;
	int cnt = 0;

	ItemMask = ItemCODE & sinITEM_MASK3;

	if (ItemMask == sin21 || ItemMask == sin24 || ItemMask == sin27) cnt = 0; // 1시간
	if (ItemMask == sin22 || ItemMask == sin25 || ItemMask == sin28) cnt = 1; // 3시간
	if (ItemMask == sin23 || ItemMask == sin26 || ItemMask == sin29) cnt = 2; // 1일


	switch (ItemMask)
	{
		// 생명력 부스터
	case sin21:
	case sin22:
	case sin23:
		UseFlag = 0;
		Path = "image\\Sinimage\\skill\\keep\\Booster_01.tga";
		TempSkill.CODE = BOOSTER_ITEM_LIFE;
		TempSkill.Point = 0; // 부스터 아이템은 레벨이 없다.
		TempSkill.UseTime = BoosterItem_UseTime[cnt];
		TempSkill.CheckTime = (BoosterItem_UseTime[cnt] * 70) - (Time * 70);
		TempSkill.MatIcon = Mat_BoosterItem[UseFlag];
		break;

		// 기력 부스터
	case sin24:
	case sin25:
	case sin26:
		UseFlag = 1;
		Path = "image\\Sinimage\\skill\\keep\\Booster_02.tga";
		TempSkill.CODE = BOOSTER_ITEM_MANA;
		TempSkill.Point = 0; // 부스터 아이템은 레벨이 없다.
		TempSkill.UseTime = BoosterItem_UseTime[cnt];
		TempSkill.CheckTime = (BoosterItem_UseTime[cnt] * 70) - (Time * 70);
		TempSkill.MatIcon = Mat_BoosterItem[UseFlag];
		break;

		// 근력 부스터
	case sin27:
	case sin28:
	case sin29:
		UseFlag = 2;
		Path = "image\\Sinimage\\skill\\keep\\Booster_03.tga";
		TempSkill.CODE = BOOSTER_ITEM_STAMINA;
		TempSkill.Point = 0; // 부스터 아이템은 레벨이 없다.
		TempSkill.UseTime = BoosterItem_UseTime[cnt];
		TempSkill.CheckTime = (BoosterItem_UseTime[cnt] * 70) - (Time * 70);
		TempSkill.MatIcon = Mat_BoosterItem[UseFlag];
		break;
	}

	char szBuff[128];
	if (!Mat_BoosterItem[UseFlag]) {
		wsprintf(szBuff, Path, UseFlag);
		Mat_BoosterItem[UseFlag] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//텍스쳐 로딩
	}

	sinContinueSkillSet(&TempSkill);

	if (UseFlag == 0)
	{
		// 박재원 - 부스터 아이템(생명력)
		AddBoosterLife = ((int)sinChar->Life[1] * BoosterItem_DataPercent[UseFlag]) / 100;
		BoosterLifeTime = Time;
		BoosterLifePercent = BoosterItem_DataPercent[UseFlag];
		CheckBoosterLifeTime = 0; //체크시간초기화
	}
	else if (UseFlag == 1)
	{
		// 박재원 - 부스터 아이템(기력)
		AddVirtualMana[1] = ((int)sinChar->Mana[1] * BoosterItem_DataPercent[UseFlag]) / 100;
		sinVirtualManaTime = Time;
		sinVirtualManaPercent = BoosterItem_DataPercent[UseFlag];
		CheckVirtualManaTime = 0; //체크시간초기화
	}
	else if (UseFlag == 2)
	{
		// 박재원 - 부스터 아이템(근력)
		AddVirtualStamina[1] = ((int)sinChar->Stamina[1] * BoosterItem_DataPercent[UseFlag]) / 100;
		sinVirtualStaminaTime = Time;
		sinVirtualStaminaPercent = BoosterItem_DataPercent[UseFlag];
		CheckVirtualStaminaTime = 0; //체크시간초기화
	}

	cInvenTory.SetItemToChar();

	return TRUE;
}

// 장별 - 스킬 딜레이
int cSKILL::SetSkillDelayItem(DWORD ItemCODE, int Time)
{
	int UseFlag = 0;
	sSKILL TempSkill;
	memset(&TempSkill, 0, sizeof(sSKILL));
	char* Path;

	DWORD ItemMask;
	int cnt = 0;

	ItemMask = ItemCODE & sinITEM_MASK3;

	if (ItemMask == sin30 || ItemMask == sin33 || ItemMask == sin36) cnt = 0;
	if (ItemMask == sin31 || ItemMask == sin34 || ItemMask == sin37) cnt = 1;
	if (ItemMask == sin32 || ItemMask == sin35 || ItemMask == sin38) cnt = 2;

	switch (ItemMask)
	{
		// 생명력 부스터
	case sin30:
	case sin31:
	case sin32:
		UseFlag = 0;
		Path = "image\\Sinimage\\skill\\keep\\Booster_01.tga";
		TempSkill.CODE = SKILLDELAY_ITEM_LIFE;
		TempSkill.Point = 0;
		TempSkill.UseTime = nSkillDelay_UseTime[cnt];
		TempSkill.CheckTime = (nSkillDelay_UseTime[cnt] * 70) - (Time * 70);
		TempSkill.MatIcon = nMat_SkillDelayItem[UseFlag];
		break;
	}


	char szBuff[128];
	if (!nMat_SkillDelayItem[UseFlag]) {
		wsprintf(szBuff, Path, UseFlag);
		nMat_SkillDelayItem[UseFlag] = CreateTextureMaterial(szBuff, 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
		ReadTextures();	//텍스쳐 로딩
	}

	sinContinueSkillSet(&TempSkill);

	if (UseFlag == 0)
	{
		sinAddSkillDelay = 10;
		sinSkillDelayTime = Time;
		BoosterLifePercent = BoosterItem_DataPercent[UseFlag];
		CheckSkillDelayTime = 0; //체크시간초기화
	}


	cInvenTory.SetItemToChar();

	return TRUE;
}


// 장별 - 스킬 딜레이
int cSKILL::UseSkillDelayItem(sITEM* pItem)
{
	DWORD SkillDelayCODE;
	DWORD ItemMask;
	int SkillDelayTime = 0;
	int cnt = 0;

	ItemMask = pItem->CODE & sinITEM_MASK3;
	if (ItemMask == sin30 || ItemMask == sin33 || ItemMask == sin36) cnt = 0;
	if (ItemMask == sin31 || ItemMask == sin34 || ItemMask == sin37) cnt = 1;
	if (ItemMask == sin32 || ItemMask == sin35 || ItemMask == sin38) cnt = 2;

	switch (ItemMask) {
	case sin30: // 1시간
	case sin31: // 3시간
	case sin32: // 1일
		SkillDelayCODE = SKILLDELAY_ITEM_LIFE;
		SkillDelayTime = nSkillDelay_UseTime[cnt];

		sinAddSkillDelay = 10;
		sinSkillDelayTime = SkillDelayTime;
		CheckSkillDelayTime = 0; //체크시간초기화
		break;
	}
	//해당 돼는 아이템이 맞으면 아이템스킬을 세팅한다.
	sSKILL TempSkill;

	if (CSKILL->SearchContiueSkillCODE(SkillDelayCODE) == SkillDelayCODE && SkillDelayCODE != 0) {
		cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
		haCastleSkillUseFlag = 0;
		return TRUE;
	}
	haCastleSkillUseFlag = 1;

	for (int j = 0; j < SIN_MAX_SKILL; j++) {
		if (sSkill[j].CODE == SkillDelayCODE) {
			memcpy(&TempSkill, &sSkill[j], sizeof(sSKILL));
			TempSkill.UseTime = SkillDelayTime;
			sinContinueSkillSet(&TempSkill);
			SwitchSkill(&TempSkill);
			break;
		}
	}

	sinPlaySound(SIN_SOUND_EAT_POTION2);//대박 사운드
	cInvenTory.SetItemToChar();
	return TRUE;
}


//트라이엄프 오브 발할라
int cSKILL::SetT_Of_Valhalla(int Point, int LV)
{

	//Contiue 초기화
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (ContinueSkill[i].CODE == SKILL_TRIUMPH_OF_VALHALLA || ContinueSkill[i].CODE == SKILL_HALL_OF_VALHALLA) {
				//memset(&ContinueSkill[i],0,sizeof(sSKILL));
				return FALSE; //중복시전 불가
			}
		}
	}

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (!ContinueSkill[i].Flag) {
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].CODE = SKILL_TRIUMPH_OF_VALHALLA;
			ContinueSkill[i].UseTime = T_Of_Valhalla_Time[Point - 1];
			ContinueSkill[i].Point = Point;
			SearchSkillDoc(&ContinueSkill[i]);
			break;
		}
	}

	SetT_Of_ValhallaLV = LV;

	if (SetT_Of_ValhallaFlag2) { //자신이 사용한것이면
		SetT_Of_ValhallaFlag2 = 0;
		SetT_Of_ValhallaFlag = 0;
	}
	else {
		SetT_Of_ValhallaFlag = 1; //남이사용해준것이면

	}
	cInvenTory.SetItemToChar();
	return TRUE;
}

int cSKILL::SetEnchant_Weapon(int Time, int Point)
{

	//Contiue 초기화
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (ContinueSkill[i].CODE == SKILL_ENCHANT_WEAPON) {
				memset(&ContinueSkill[i], 0, sizeof(sSKILL));
				//return FALSE; 
			}
		}
	}



	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (!ContinueSkill[i].Flag) {
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
int cSKILL::ForeceOfNature(int Time, int Point, int Party)
{

	//Contiue 초기화
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (ContinueSkill[i].CODE == SKILL_FORCE_OF_NATURE) {
				if (ContinueSkill[i].Flag == 2 && !Party) {
					memset(&ContinueSkill[i], 0, sizeof(sSKILL));
					break;
				}
				else return FALSE; //중복시전 불가
			}
		}
	}

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (!ContinueSkill[i].Flag) {
			ContinueSkill[i].Flag = Party + 1;
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
int cSKILL::HellOfValhalla(int Time, int Point, int TVPoint, int LV, int Party)
{

	//Contiue 초기화
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (ContinueSkill[i].CODE == SKILL_HALL_OF_VALHALLA) {
				if (ContinueSkill[i].Flag == 2 && !Party) {
					memset(&ContinueSkill[i], 0, sizeof(sSKILL));
					break;
				}
				else return FALSE; //중복시전 불가
			}
		}
	}

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (!ContinueSkill[i].Flag) {
			ContinueSkill[i].Flag = Party + 1; //파티원은 2로한다
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
int cSKILL::Regeneration_Field(int Time, int Point, int Party)
{

	//Contiue 초기화
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (ContinueSkill[i].CODE == SKILL_REGENERATION_FIELD) {
				if (ContinueSkill[i].UseTime > Time)return FALSE; //같은 캐릭사용시 리턴
				ContinueSkill[i].UseTime = Time;
				return TRUE; //시간갱신
			}
		}
	}

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (!ContinueSkill[i].Flag) {
			ContinueSkill[i].Flag = 1 + Party; //파티원은 2로한다
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

int cSKILL::SearchSkillDoc(sSKILL* pSkill)
{

	for (int i = 0; i < SIN_MAX_SKILL; i++) {
		if (pSkill->CODE == sSkill_Info[i].CODE) {
			if (pSkill->CODE == SKILL_FORCE_ORB) {
#ifdef _LANGUAGE_CHINESE
				wsprintf(pSkill->Skill_Info.SkillName, "%s%s", SheltomName2[pSkill->Point - 1], sSkill_Info[i].SkillName);
#else
				if (pSkill->ElementIndex == 1000)
				{
					wsprintf(pSkill->Skill_Info.SkillName, "%s %s", BillingMagicForceName, sSkill_Info[i].SkillName);
				}
				else if (pSkill->ElementIndex == 2000)
				{
					wsprintf(pSkill->Skill_Info.SkillName, "%s %s %s", MagicForceName, SheltomName2[pSkill->Point - 1], sSkill_Info[i].SkillName);
				}
				else
					wsprintf(pSkill->Skill_Info.SkillName, "%s %s", SheltomName2[pSkill->Point - 1], sSkill_Info[i].SkillName);
#endif

			}
			else {
				lstrcpy(pSkill->Skill_Info.SkillName, sSkill_Info[i].SkillName);
			}
			lstrcpy(pSkill->Skill_Info.SkillDoc, sSkill_Info[i].SkillDoc);
			break;

		}

	}

	return TRUE;
}

int cSKILL::SkillBarDraw(sSKILL* pSkill, int x, int y, int Alpha)
{

	D3DTLVERTEX tlVertex[73];
	int Time = 0;
	Time = (int)(((float)pSkill->CheckTime / ((float)pSkill->UseTime * 70.0f)) * 72.0f);
	float fx, fy = 0;
	int i = 0, j = 0;
	int sinY, cosY;
	tlVertex[0].rhw = 1;
	tlVertex[0].color = D3DCOLOR_RGBA(0, Alpha, Alpha, Alpha);
	tlVertex[0].specular = D3DCOLOR_RGBA(0, 0, 0, 0);
	tlVertex[0].sz = 0;
	tlVertex[0].sx = float(x);
	tlVertex[0].sy = float(y);
	tlVertex[0].tu = 0.5;
	tlVertex[0].tv = 0.5f;
	for (i = 1; i < Time + 1; i++)
	{
		tlVertex[i].rhw = 1;
		if (pSkill->CODE == SKILL_FORCE_ORB)
		{
			tlVertex[i].color = D3DCOLOR_RGBA(255, 190, 30, Alpha);
		}
		else
			tlVertex[i].color = D3DCOLOR_RGBA(0, 255, 200, Alpha);
		tlVertex[i].specular = D3DCOLOR_RGBA(0, 0, 0, 0);
		tlVertex[i].sz = 0;
		sinY = GetSin[(1024 / 2) + (i * 56) & ANGCLIP];
		cosY = GetCos[(1024 / 2) + ((i * 56) + ANGLE_180) & ANGCLIP];
		fx = (float)((10 * sinY + 10 * cosY) >> 8);
		fy = (float)((10 * cosY - 10 * sinY) >> 8);
		fx /= 256;
		fy /= 256;
		tlVertex[i].sx = (float)(fx + x);
		tlVertex[i].sy = (float)(fy + y);
		tlVertex[i].tu = 0.5f;
		tlVertex[i].tv = 0.5f;
	}
	GRAPHICDEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	GRAPHICDEVICE->SetTexture(0, 0);
	GRAPHICDEVICE->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1);
	if (Time - 1 > 0)
		GRAPHICDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, Time - 1, tlVertex, sizeof(D3DTLVERTEX));
	GRAPHICDEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	return TRUE;
}



//지속 스킬 체크
int cSKILL::CheckContinueIcon()
{
	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (!ContinueSkill[i].MatIcon) {
				for (int j = 0; j < SIN_MAX_SKILL; j++) {
					if (ContinueSkill[i].CODE == sSkill[j].CODE) {
						if (sSkill[j].MatIcon) {
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

	for (int i = 0; i < MAX_CONTINUE_SKILL; i++) {
		if (ContinueSkill[i].Flag) {
			if (ContinueSkill[i].CODE == CODE) {
				memset(&ContinueSkill[i], 0, sizeof(sSKILL));
				cInvenTory.SetItemToChar();
				break;
			}
		}
	}
	return FALSE;
}

//스킬단축기 초기화
int cSKILL::SkillShortKeyInit()
{
	for (int j = 1; j < SIN_MAX_USE_SKILL; j++) {
		sinSkill.UseSkill[j].ShortKey = 0;
	}
	return TRUE;
}

int cSKILL::CheckMaturedSkill(int ChangeJob)
{
	int matured_skill_count = 0;

	switch (ChangeJob)
	{
	case 1:
		for (int j = 1; j < 5; j++)
		{
			if (sinSkill.UseSkill[j].UseSkillCount < 10000 && sinSkill.UseSkill[j].Skill_Info.USECODE != SIN_SKILL_USE_NOT &&
				sinSkill.UseSkill[j].UseSkillMastery)
			{
				matured_skill_count++;
			}
		}
		break;
	case 2:
		for (int j = 5; j < 9; j++)
		{
			if (sinSkill.UseSkill[j].UseSkillCount < 10000 && sinSkill.UseSkill[j].Skill_Info.USECODE != SIN_SKILL_USE_NOT &&
				sinSkill.UseSkill[j].UseSkillMastery)
			{
				matured_skill_count++;
			}
		}
		break;
	case 3:
		for (int j = 9; j < 13; j++)
		{
			if (sinSkill.UseSkill[j].UseSkillCount < 10000 && sinSkill.UseSkill[j].Skill_Info.USECODE != SIN_SKILL_USE_NOT &&
				sinSkill.UseSkill[j].UseSkillMastery)
			{
				matured_skill_count++;
			}
		}
		break;
	}

	if (matured_skill_count <= 4 && matured_skill_count > 0)
		return TRUE;
	else
		return FALSE;
}