#pragma once

#define SIN2_ATTACK_RATE		0
#define SIN2_DAMAGE				1
#define SIN2_DEFENSE			2
#define SIN2_ABSORB				3
#define SIN2_SPEED				4
#define SIN2_LIFE				5
#define SIN2_MANA				6 
#define SIN2_STAMINA			7 

#define USE_SKILL_MASTERY_COUNT	100


struct sSKILL;

int RequireStateCheck(int Mana);
int ContinueSkillUse(sSKILL* pSkill, int Time);
void CheckContinueSkill();
void SwitchSkill(sSKILL* pSkill);


int F_Extreme_Shield();
int F_Mechanic_Bomb();
int F_Physical_Absorption();
int F_Poison_Attribute();

int F_Great_Smash();
int F_Maximize();
int F_Automation();
int F_Spark();

int F_Metal_Armor();
int F_Grand_Smash();
int F_M_W_Mastery();
int F_Spark_Shield();


int F_Impulsion();
int F_Compulsion();
int F_Magnetic_Sphere();
int F_Metal_Golem();


int F_Melee_Mastery();
int F_Fire_Attribute();
int F_Raving();
int F_Impact();

int F_Triple_Impact();
int F_Brutal_Swing();
int F_Roar();
int F_RageOfZecram();

int F_Concentration();
int F_Avanging_Crash();
int F_Swift_Axe();
int F_Bone_Crash();


int F_Destoryer();
int F_Berserker();
int F_Cyclone_Strike();
int F_Boost_Health();


int F_Pike_Wind();
int F_Ice_Attribute();
int F_Critical_Hit();
int F_Jumping_Crash();

int F_Ground_Pike();
int F_Tornado_Pike();
int F_Weapone_Defence_Mastery();
int F_Expansion();

int F_Venom_Spear();
int F_Vanish();
int F_Critical_Mastery();
int F_Chain_Lance();

int F_Assassin_Eye();
int F_Charging_Strike();
int F_Vague();
int F_Shadow_Master();


int F_Scout_Hawk();
int F_Shooting_Mastery();
int F_Wind_Arrow();
int F_Perfect_Aim();

int F_Dions_Eye();
int F_Falcon();
int F_Arrow_of_Rage();
int F_Avalanche();

int F_E_Shot();
int F_G_Falcon();
int F_B_Shot();
int F_Perforation();


int F_Recall_Wolverin();
int F_Evasion_Mastery();
int F_Phoenix_Shot();
int F_Force_Of_Nature();






extern short E_Shield_BlockRate[10];
extern short E_Shield_UseTime[10];
extern short E_Shield_UseMana[10];


extern short Mechanic_Bomb_Damage[10][2];
extern short Mechanic_Bomb_Attack_Range[10];
extern short Mechanic_Bomb_UseMana[10];


extern short P_Absorb[10][2];
extern short P_Absorb_UseTime[10];
extern short P_Absor_bUseMana[10];


extern short PlusPoison[10];


extern short G_Smash_Attack_Rate[10];
extern short G_Smash_Attack_Rage[10];
extern short G_Smash_UseMana[10];


extern short Maximize_Damage[10];
extern short Maximize_Weapon_Speed[10];
extern short Maximize_Time[10];
extern short Maximize_UseMana[10];


extern short Automation_Damage[10];
extern short Automation_Speed[10];
extern short Automation_Time[10];
extern short Automation_UseMana[10];


extern short Spark_Damage[10][2];
extern short Spark_Area[10];
extern short Spark_UseMana[10];


extern short Metal_Armor_Defense[10];
extern short Metal_Armor_Time[10];
extern short Metal_Armor_UseMana[10];


extern short Grand_Smash_AttackRate[10];
extern short Grand_Smash_Damage[10];
extern short Grand_Smash_UseMana[10];


extern short M_Weapon_Mastey[10];


extern short Spark_Shield_Damage[10];
extern short Spark_Shield_Defense[10];
extern short Spark_Shield_Time[10];
extern short Spark_Shield_UseMana[10];



extern short Impulsion_LightingDamage[10][2];
extern short Impulsion_Range[10];
extern short Impulsion_LightingNum[10];
extern short Impulsion_UseMana[10];


extern short Compulsion_AddAbsorb[10];
extern short Compulsion_Area[10];
extern short Compulsion_Time[10];
extern short Compulsion_UseMana[10];


extern short Magnetic_Sphere_AddDamage[10][2];
extern short Magnetic_Sphere_Area[10];
extern short Magnetic_Sphere_AttackDelay[10];
extern short Magnetic_Sphere_Time[10];
extern short Magnetic_Sphere_UseMana[10];


extern short Metal_Golem_Damage[10][2];
extern short Metal_Golem_Hit[10];
extern short Metal_Golem_Defense[10];
extern short Metal_Golem_Life[10];
extern short Metal_Golem_UseMana[10];



extern short Melee_Mastery_DamagePercent[10];


extern short PlusFire[10];


extern short Raving_Damage[10];
extern short Ravind_Speed[10];
extern float Raving_UseLife[10];
extern short Raving_UseMana[10];


extern short Impact_Attack_Rating[10];
extern short Impact_Damage[10];
extern short Impact_UseMana[10];


extern short T_Impact_Damage[10];
extern short T_Impact_Hit[10];
extern short T_Impact_UseMana[10];


extern short B_Swing_Damage[10];
extern short B_Swing_Critical[10];
extern short B_Swing_UseMana[10];


extern short Roar_Range[10];
extern short Roar_Time[10];
extern short Roar_UseMana[10];


extern short R_Zecram_Damage[10];
extern short R_Zecram_UseMana[10];


extern short Concentration_AttackRate[10];
extern short Concentration_Time[10];
extern short Concentration_UseMana[10];


extern short A_Crash_Damage[10];
extern short A_Crash_AttackRate[10];
extern short A_Crash_UseMana[10];


extern short Swift_Axe_Speed[10];
extern short Swift_Axe_Time[10];
extern short Swift_Axe_UseMana[10];


extern short B_Crash_Damage[10];
extern short B_Crash_DemonDamage[10];
extern short B_Crash_UseMana[10];




extern short Destoryer_DamagePercent[10];
extern short Destoryer_AddCritical[10];
extern short Destoryer_UseMana[10];


extern short Berserker_AddAttack[10];
extern short Berserker_SubAbsorb[10];
extern short Berserker_Time[10];
extern short Berserker_UseMana[10];


extern short Cyclone_Strike_DamagePercent[10];
extern short Cyclone_Strike_AreaDamage[10][2];
extern short Cyclone_Strike_AttackNum[10];
extern short Cyclone_Strike_Area[10];
extern short Cyclone_Strike_UseMana[10];


extern short Boost_Health_Life[10];




extern short Pike_Wind_Damage[10][2];
extern short Pike_Wind_Push_Lenght[10];
extern short Pike_Wind_UseMana[10];


extern short PlusIce[10];


extern short Critical_Hit_Critical[10];
extern short Critical_Hit_UseMana[10];


extern short Jumping_Crash_UseMana[10];
extern short Jumping_Crash_Attack_Rating[10];
extern short Jumping_Crash_Damage[10];


extern short G_Pike_Damage[10][2];
extern short G_Pike_Range[10];
extern short G_Pike_UseMana[10];
extern short G_Pike_Time[10];


extern short Tornado_Damage[10];
extern short Tornado_Stun[10];
extern short Tornado_Range[10];
extern short Tornado_UseMana[10];


extern short W_D_Mastery_Block[10];


extern short Expansion_Size[10];
extern short Expansion_UseMana[10];
extern short Expansion_Damage[10];


extern short VenomSpear_Damage[10][2];
extern short VenomSpear_Num[10];
extern short VenomSpear_Time[10];
extern short VenomSpear_Range[10];
extern short VenomSpear_UseMana[10];

extern short Vanish_Damage[10];
extern short Vanish_Range[10];
extern short Vanish_Time[10];
extern short Vanish_UseMana[10];


extern short Critical_Mastery_Critical[10];


extern short Chain_Lance_Damage[10];

extern short Chain_Lance_UseMana[10];



extern short Assassin_Eye_AddCritical[10];
extern short Assassin_Eye_M_SubCritical[10];
extern short Assassin_Eye_Time[10];
extern short Assassin_Eye_UseMana[10];


extern short Charging_Strike_DamagePercent[10];
extern short Charging_Strike_CharingDamagePercent[10];
extern short Charging_Strike_Time[10];
extern short Charging_Strike_UseMana[10];


extern short Vague_EvasionPercent[10];
extern short Vague_Time[10];
extern short Vague_UseMana[10];


extern short Shadow_Master_DamagePercent[10];
extern short Shadow_Master_ShadowNum[10];
extern short Shadow_Master_UseMana[10];




extern short Scout_Hawk_Attack_Rate[10];
extern short Scout_Hawk_UseTime[10];
extern short Scout_Hawk_UseMana[10];


extern short S_Mastery_DamagePercent[10];


extern short Wind_Arrow_Speed[10];
extern short Wind_Arrow_Damage[10];
extern short Wind_Arrow_UseMana[10];



extern short Perfect_Aim_Attack_Rate[10];
extern short Perfect_Aim_Damage[10];
extern short Perfect_Aim_Damage_LV[10];
extern short Perfect_Aim_UseMana[10];


extern short D_Eye_Attack_Rate[10];


extern short Falcon_Damage[10][2];
extern short Falcon_Time[10];
extern short Falcon_UseMana[10];


extern short A_Rage_ArrowNum[10];
extern short A_Rage_Damage[10];
extern short A_Rage_Area[10];
extern short A_Rage_UseMana[10];


extern short Avalanche_Speed[10];
extern short Avalanche_Damage[10];
extern short Avalanche_ArrowNum[10];
extern short Avalanche_UseMana[10];


extern short Elemental_Shot_Fire[10][2];
extern short Elemental_Shot_Lightning[10][2];
extern short Elemental_Shot_UseMana[10];


extern short   Golden_Falcon_Damage[10];
extern float Golden_Falcon_LifeRegen[10];
extern short   Golden_Falcon_Time[10];
extern short   Golden_Falcon_UseMana[10];


extern short BombShot_Damage[10];
extern short BombShot_DemonDamage[10];
extern short BombShot_AddDamage[10][2];
extern short BombShot_Area[10];
extern short BombShot_UseMana[10];


extern short Perforation_Damage[10];
extern short Perforation_Critical[10];
extern short Perforation_UseMana[10];
extern short Perforation_Attack_Range[10];



extern short Recall_Wolverin_Damage[10][2];
extern short Recall_Wolverin_Hit[10];
extern short Recall_Wolverin_Defense[10];
extern short Recall_Wolverin_Life[10];
extern short Recall_Wolverin_UseMana[10];


//extern short Evasion_Mastery_AddPercent[10];
extern short Evasion_Mastery_AddPercent[10];
extern float Evasion_Mastery_AddSpeed[10];
extern short Evasion_Mastery_AddReduce[10];
extern short Evasion_Mastery_Time[10];
extern short Evasion_Mastery_UseMana[10];


extern short   Phoenix_Shot_DamagePercent[10];
extern short   Phoenix_Shot_Time[10];
extern short   Phoenix_Shot_UseMana[10];


extern short Force_Of_Nature_AddDamage[10];
extern short Force_Of_Nature_AddHit[10];
extern short Force_Of_Nature_FalconAddDamage[10];
extern short Force_Of_Nature_Time[10];
extern short Force_Of_Nature_UseMana[10];




extern int sinMasterSkill_Money[16];
extern int sinMasterSkill_PlusMoney[16];



extern int SkillTextColor[10];


extern int NotUseSkillElement[3];
extern int sinMasteryIncreaIndex[16];