#pragma once

#include "smConfig.h"
#include "sinbaram/sinItem.h"

extern smCONFIG	smConfig;

smCONFIG *smConfigDecode( char *iniFile );

int ReadCustomCracker( char *szCrackFile , TRANS_FIND_CRACKER *lpTransFindCrack  );

int smModelDecode( char *iniFile , smMODELINFO *Info );
int smCharDecode( char *szCharFile , smCHAR_INFO *lpCharInfo , smCHAR_MONSTER_INFO	*lpMonInfo , char *lpDialogMessage=0 );

extern char	szShotCutMessage[10][100];
int	ReadShotcutMessage( char *szFileName );


struct rsSTG_MONSTER {
	char	szMonsterName[32];			//몬스터 이름
	int		OpenPercentage;				//출현빈도

	smCHAR_INFO	*lpCharInfo;			//몬스터 정보 포인터
	int		NumOpenStart;				//출현빈도 시작 값
};

#define rsSTG_MONSTER_MAX	50
#define rsBOSS_MONSTER_MAX	16

struct sBOSS_MONSTER {
	smCHAR_INFO		*MasterMonster;			//보스급 몬스터
	smCHAR_INFO		*SlaveMonster;			//수행 몬스터
	int				SlaveCount;				//수행 몬스터 숫자
	BYTE			bOpenTime[32];			//나타나는 시간
	int				OpenTimeCount;			//시간 수
};

struct	rsSTG_MONSTER_LIST {

	rsSTG_MONSTER	rsMonster[rsSTG_MONSTER_MAX];

	int		PecetageCount;
	int		Counter;
	int		LimitMax;						//출현 제한수 / 최대치
	int		OpenInterval;					//출현 주기
	int		OpenLimit;						//직역당 최대 출현수
	DWORD	dwIntervalTime;					//출현 간격 제한시간

	sBOSS_MONSTER	sBossMonsters[rsBOSS_MONSTER_MAX];		//두목급 몬스터 세트
	int	BossMonsterCount;									//두목급 몬스터 설정 갯수

};


#define SPECIAL_JOB_RANDOM_MAX			12

//기본 아이템 구조
struct	sDEF_ITEMINFO	{

	sITEMINFO Item;

	short	sDurability[2];			//내구력
	short	sResistance[8][2];		//원소 저항력
	short	sDamage[4];				//공격력
	short	sAttack_Rating[2];		//명중력
	short	sDefence[2];			//방어력
	float	fBlock_Rating[2];		//블럭율
	float	fAbsorb[2];				//흡수력
	float	fSpeed[2];				//이동속도
	int		Increase_Life[2];		//라이프 증가
	int		Increase_Mana[2];		//마나 증가
	int		Increase_Stamina[2];	//스테미나 증가

	float	fMana_Regen[2];			//마나재생
	float	fLife_Regen[2];			//라이프재생
	float	fStamina_Regen[2];		//스테미나재생


	/////////////////// 특화 적용 /////////////////////
	short	sSpecial_Defence[2];			//방어력
	float	fSpecial_Absorb[2];				//흡수력
	float	fSpecial_fSpeed[2];				//이동속도

	float	fSpecial_Magic_Mastery[2];		//마법숙련도
	float	fSpecial_Mana_Regen[2];			//기력 재생

	int     Lev_Attack_Rating[2];			//명중력 

	DWORD	dwJobBitCode_Random[SPECIAL_JOB_RANDOM_MAX];			//랜덤특화 직업 목록 
	int		JobBitCodeRandomCount;

	//////////////////// 발생량 /////////////////////////

	short	sGenDay[2];										//하루에 발생할수 있는양/하루동안 발생된수

	BYTE	*DefCompressData[ sizeof(sITEMINFO) ];	//Item 데이타의 압축 정보
	int		DefCompressDataLen;						//압축데이타의 길이
};


struct JOBNAME_BIT {
	char *szName;
	DWORD dwBitMask;
};

extern JOBNAME_BIT BitMaskJobNames[];


//몬스터 출현 정보 파일에서 해독하여 설정한다
int DecodeOpenMonster( char *szCharFile , rsSTG_MONSTER_LIST *lpStgMonList , smCHAR_INFO *lpCharInfoList, int ListCnt );
//아이템 정보 파일을 해독하여 돌려준다 
int DecodeItemInfo( char *szItemFile , sDEF_ITEMINFO *lpDefItem  );
//아이템 생성
int CreateDefItem( sITEMINFO *lpItem , sDEF_ITEMINFO *lpDefItem , int SpJobCode=0 );
//아이템 생성
int CreateDefItem( sITEMINFO *lpItem , sDEF_ITEMINFO *lpDefItem , int SpJobCode , int ItemNormam );

//후진 아이템 설정
int SetLowerItem( sDEF_ITEMINFO *lpDefItem  );

//최소 최대치에서 랜덤값 산출
float GetRandomFloatPos( float Min , float Max );
int GetRandomPos( int Min , int Max );

//SOD벨라트라 몬스터 설정 불러오기
int	Open_SODConfig( char *szFileName , void *lphCoreMonster , int *SodTax , char *szSodText , int *SodScoreDiv );



/*
#define		JOB_CODE_PALADIN			0x00008000
#define		JOB_CODE_SHECRON			0x00010000
#define		JOB_CODE_MORSAGE			0x00020000
#define		JOB_CODE_ROYALKNIGHT		0x00040000
#define		JOB_CODE_ROYALWIZARD		0x00080000
#define		JOB_CODE_SAINTKNIGHT		0x00100000
#define		JOB_CODE_ARCHMAGE			0x00200000


#define		JOB_CODE_ROGUE				0x00400000
#define		JOB_CODE_PILGRIM			0x00800000
#define		JOB_CODE_PRAYER				0x01000000
#define		JOB_CODE_SHAMAN				0x02000000
#define		JOB_CODE_WANDERER			0x04000000
#define		JOB_CODE_MONK				0x08000000
#define		JOB_CODE_DARKSHAMAN			0x10000000
#define		JOB_CODE_ELDER				0x20000000
#define		JOB_CODE_SOULMASTER			0x40000000
#define		JOB_CODE_NESTOR				0x80000000
#define		JOB_CODE_NECROMANCER		0x80000000
*/
#define		BROOD_CODE_TEMPSKRON			1
#define		BROOD_CODE_MORAYION				2
#define		BROOD_CODE_SOPPHETIO			4

#define		JOB_CODE_MECHANICIAN		0x00000001	
#define		JOB_CODE_FIGHTER			0x00000002	
#define		JOB_CODE_PIKEMAN			0x00000004	
#define		JOB_CODE_ARCHER				0x00000008	

#define		JOB_CODE_MECHANICMASTER		0x00000010	
#define		JOB_CODE_WARRIOR			0x00000020
#define		JOB_CODE_COMBATANT			0x00000040
#define		JOB_CODE_HUNTERMASTER		0x00000080

#define		JOB_CODE_METALLEADER		0x00000100	
#define		JOB_CODE_CHAMPION			0x00000200
#define		JOB_CODE_LANCER				0x00000400
#define		JOB_CODE_DIONS_DISCIPLE		0x00000800

#define		JOB_CODE_HEAVYMETAL			0x00001000	
#define		JOB_CODE_IMMORTALWARRIOR	0x00002000	
#define		JOB_CODE_LANCELOT			0x00004000
#define		JOB_CODE_SAGITTARION		0x00008000


#define		JOB_CODE_KNIGHT				0x00010000
#define		JOB_CODE_ATALANTA			0x00020000
#define		JOB_CODE_PRIEST				0x00040000
#define		JOB_CODE_MAGICIAN			0x00080000

#define		JOB_CODE_PALADIN			0x00100000
#define		JOB_CODE_VALKYRIE			0x00200000
#define		JOB_CODE_SAINTESS			0x00400000
#define		JOB_CODE_WIZARD				0x00800000

#define		JOB_CODE_HOLYKNIGHT			0x01000000
#define		JOB_CODE_BRUNHILD			0x02000000
#define		JOB_CODE_BISHOP				0x04000000
#define		JOB_CODE_ROYALKNIGHT		0x08000000

#define		JOB_CODE_SAINTKNIGHT		0x10000000
#define		JOB_CODE_VALHALLA			0x20000000
#define		JOB_CODE_CELESTIAL			0x40000000
#define		JOB_CODE_ARCHMAGE			0x80000000



struct	JOB_DATA_CODE
{
	char* szName;
	char* szName2;
	DWORD	JobBitCode;
	DWORD	JobCode;
	DWORD	Brood;
	int     Sight;
	int     LifeFunction;
	int     ManaFunction;
	int     StaminaFunction;
	short   DamageFunction[3];

	JOB_DATA_CODE()
	{
		szName = nullptr;
		szName2 = nullptr;
		JobBitCode = 0;
		JobCode = 0;
		Brood = 0;
		Sight = 0;
		LifeFunction = 0;
		ManaFunction = 0;
		StaminaFunction = 0;
		DamageFunction[0] = 0;
		DamageFunction[1] = 0;
		DamageFunction[2] = 0;
	}

	JOB_DATA_CODE(
		char* szName,
		char* szName2,
		DWORD	JobBitCode,
		DWORD	JobCode,
		DWORD	Brood,
		int     Sight = 0,
		int     LifeFunction = 0,
		int     ManaFunction = 0,
		int     StaminaFunction = 0,
		short   DamageFunction1 = 0,
		short   DamageFunction2 = 0,
		short   DamageFunction3 = 0)
	{
		this->szName = szName;
		this->szName2 = szName2;
		this->JobBitCode = JobBitCode;
		this->JobCode = JobCode;
		this->Brood = Brood;
		this->Sight = Sight;
		this->LifeFunction = LifeFunction;
		this->ManaFunction = ManaFunction;
		this->StaminaFunction = StaminaFunction;
		this->DamageFunction[0] = DamageFunction1;
		this->DamageFunction[1] = DamageFunction2;
		this->DamageFunction[2] = DamageFunction3;
	}
};


struct	SKILL_DATA_CODE {
	char	*szSkillName;				//스킬 이름
	int		dwPlaySkillCode;			//스킬 코드
};

JOB_DATA_CODE* GetJobDataCode(DWORD BaseJob, int	JobCnt);

extern	SKILL_DATA_CODE	SkillDataCode[];
extern	JOB_DATA_CODE	JobDataBase[];

extern int NotItemSpecial;
