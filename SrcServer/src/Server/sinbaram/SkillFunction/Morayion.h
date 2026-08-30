#pragma once

#define MAX_CONTINUE_SKILL		20

int sinCheckSkillUseOk(sSKILL* lpSkill);
int sinUseManaStamina(sSKILL* lpSkill);
int sinContinueSkillSet(sSKILL* lpSkill);
int sinCheckContinueSkill();
int sinSwitchSkill(sSKILL* pSkill);


int F_Sword_Blast();
int F_HolyBody();
int F_Physical_Traning();
int F_Double_Crash();

int F_Holy_Valor();
int F_Brandish();
int F_Piercing();
int F_Drastic_Spirit();

int F_S_Mastery();
int F_D_Inhalation();
int F_Holy_Incantation();
int F_Grand_Cross();


int F_Divine_Piercing();
int F_Godly_Shield();
int F_God_Bless();
int F_Sword_Of_Justice();


int F_S_Strike();
int F_Farina();
int F_D_Mastery();
int F_V_Spear();

int F_Windy();
int F_Twist_Javelin();
int F_Soul_Sucker();
int F_Fire_Javelin();

int F_Split_Javelin();
int F_T_Of_Valhalla();
int F_Lightning_Javelin();
int F_Storm_Javelin();


int F_Hall_Of_Valhalla();
int F_X_Rage();
int F_Frost_Javelin();
int F_Vengeance();


int F_Healing();
int F_HolyBolt();
int F_M_Spark();
int F_HolyMind();

int F_Meditation();
int F_Divine_Lightning();
int F_Holy_Reflection();
int F_Grand_Healing();

int F_Vigor_Ball();
int F_Resurrection();
int F_Extinction();
int F_Virtual_Life();


int F_Glacial_Spike();
int F_Regeneration_Field();
int F_Chain_Lightning();
int F_Summon_Muspell();




int F_Agony();
int F_FireBolt();
int F_Zenith();
int F_ZenithFromServer(int Time);
int F_FireBall();

int F_Mental_Mastery();
int F_Watornado();
int F_Enchant_Weapon();
int F_Dead_Ray();

int F_Energy_Shield();
int F_Diastrophism();
int F_Spirit_Elemental();
int F_D_Sword();


int F_Fire_Elemental();
int F_Flame_Wave();
int F_Distortion();
int F_Meteo();



extern short Sword_Blast_UseMana[10];
extern short Sword_Blast_Damage[10][2];
extern short Sword_Blast_ShootingRange[10];


extern short HolyBody_UseMana[10];
extern short HolyBody_Absorb[10];
extern short HolyBody_Time[10];


extern short P_Traning_Stamina[10];


extern short D_Crash_UseMana[10];
extern short D_Crash_Damage[10];
extern short D_Crash_Critical[10];


extern short Holy_Valor_Damage[10];
extern short Holy_Valor_Time[10];
extern short Holy_Valor_Mana[10];


extern short Brandish_Range[10];
extern short Brandish_Damage[10];
extern short Brandish_Mana[10];


extern short Piercing_Damage[10];
extern short Piercing_Attack_Rating[10];
extern short Piercing_Mana[10];


extern short Drastic_Spirit_Defense[10];
extern short Drastic_Spirit_Time[10];
extern short Drastic_Spirit_Mana[10];




extern short Sword_Mastery_DamagePercent[10];


extern short D_Inhalation_Life[10];
extern short D_Inhalation_Block[10];
extern short D_Inhalation_Time[10];
extern short D_Inhalation_UseMana[10];


extern short H_Incantation_Success[10];
extern short H_Incantation_AddLife[10];
extern short H_Incantation_Time[10];
extern short H_Incantation_UseMana[10];


extern short Grand_Cross_Damage[10];
extern short Grand_Cross_AttackRate[10];
extern short Grand_Cross_UndeadDamage[10];
extern short Grand_Cross_UseMana[10];



extern short  Divine_Piercing_DamagePercent[10];
extern short  Divine_Piercing_AddHit[10];
extern short	Divine_Piercing_AttackNum[10];
extern short  Divine_Piercing_UseMana[10];


extern short Godly_Shield_AbsorbPercent[10];
extern short Godly_Shield_Time[10];
extern short Godly_Shield_UseMana[10];


extern short God_Bless_AddDamage[10];
extern short God_Bless_Time[10];
extern short God_Bless_UseMana[10];


extern short Sword_Of_Justice_DamagePercent[10];
extern short Sword_Of_Justice_Area[10];
extern short Sword_Of_Justice_UseMana[10];




extern short S_Strike_UseMana[10];
extern short S_Strike_Damage[10][2];



extern short Farina_UseMana[10];
extern short Farina_Speed[10];
extern short Farina_AttackRate[10];


extern short T_Mastery_Damage[10];


extern short V_Spear_UseMana[10];
extern short V_Spear_Damage[10][2];


extern short Windy_Attack_Rating[10];
extern short Windy_Time[10];
extern short Windy_Mana[10];


extern short Twist_Javelin_Damage[10];
extern short Twist_Javelin_Attack_Rating[10];
extern short Twist_Javelin_Mana[10];


extern short Soul_Sucker_Absorb[10];
extern short Soul_Sucker_Mana[10];


extern short Fire_Javelin_Damage[10][2];
extern short Fire_Javelin_Mana[10];


extern short Split_Javelin_AttackNum[10];
extern short Split_Javelin_Damage[10];
extern short Split_Javelin_AttackRate[10];
extern short Split_Javelin_UseMana[10];


extern short T_Of_Valhalla_Damage[10];
extern short T_Of_Valhalla_Time[10];
extern short T_Of_Valhalla_UseMana[10];


extern short Lightning_Javelin_Damage[10][2];
extern short Lightning_Javelin_UseMana[10];


extern short Storm_Javelin_Damage[10];
extern short Storm_Javelin_UseMana[10];



extern short Hall_Of_Valhalla_AddEvation[10];
extern short Hall_Of_Valhalla_Time[10];
extern short Hall_Of_Valhalla_UseMana[10];


extern short X_Rage_DamagePercent[10];
extern short X_Rage_Area[10];
extern short X_Rage_UseMana[10];


extern short Frost_Javelin_IceAddDamage[10][2];
extern short Frost_Javelin_SpeedSubPercent[10];
extern short Frost_Javelin_IceTime[10];
extern short Frost_Javelin_Time[10];
extern short Frost_Javelin_UseMana[10];



extern short Vengeance_DamagePercent[10];
extern short Vengeance_AddCritical[10];
extern short Vengeance_Range[10];
extern short Vengeance_UseMana[10];




extern short Healing_UseMana[10];
extern short Healing_Heal[10][2];


extern short HolyBolt_UseMana[10];
extern short HolyBolt_Damage[10];


extern short M_Spark_UseMana[10];
extern short M_Spark_Damage[10];
extern short M_Spark_Num[10];



extern short HolyMind_UseMana[10];
extern short HolyMind_DecDamage[10];
extern short HolyMind_Time[10];


extern float Meditation_Regen[10];


extern short Divine_Lightning_Damage[10];
extern short Divine_Lightning_Num[10];
extern short Divine_Lightning_Mana[10];


extern short Holy_Reflection_Return_Damage[10];
extern short Holy_Reflection_Time[10];
extern short Holy_Reflection_Mana[10];


extern short Grand_Healing[10][2];
extern short Grand_Healing_Range[10];
extern short Grand_Healing_Mana[10];


extern short Vigor_Ball_Damage[10];
extern short Vigor_Ball_UseMana[10];


extern short Resurrection_Percent[10];
extern short Resurrection_UseMana[10];


extern short Extinction_Percent[10];
extern short Extinction_Amount[10];
extern short Extinction_UseMana[10];


extern short Virtual_Life_Percent[10];
extern short Virtual_Life_Time[10];
extern short Virtual_Life_UseMana[10];



extern short Glacial_Spike_Damage[10];
extern short Glacial_Spike_Area[10];
extern short Glacial_Spike_UseMana[10];


extern float Regeneration_Field_LifeRegen[10];
extern float Regeneration_Field_ManaRegen[10];
extern short   Regeneration_Field_Time[10];
extern short   Regeneration_Field_Area[10];
extern short   Regeneration_Field_UseMana[10];


extern short Chain_Lightning_Damage[10];
//extern short Chain_Lightning_Num[10];
extern short Chain_Lightning_Range[10];
extern short Chain_Lightning_UseMana[10];


extern short Summon_Muspell_Damage[10][2];
extern short Summon_Muspell_Attack_Delay[10];
extern short Summon_Muspell_UndeadAbsorbPercent[10];
extern short Summon_Muspell_BlockPercent[10];
extern short Summon_Muspell_Time[10];
extern short Summon_Muspell_UseMana[10];



extern short Agony_UseMana[10];
extern short Agony_ConvLife[10];


extern short FireBolt_UseMana[10];
extern short FireBolt_Damage[10];


extern short Zenith_UseMana[10];
extern short Zenith_Element[10];
extern short Zenith_Time[10];



extern short FireBall_UseMana[10];
extern short FireBall_Damage[10];
extern short FireBall_Range[10];
extern short FireBall_Area[10];


extern short Mental_Mastery_Mana[10];

extern short Watornado_Damage[10];
extern short Watornado_Range[10];
extern short Watornado_Mana[10];
extern short Watornado_Area[10];


extern short Enchant_Weapon_Damage_Ice[10][2];
extern short Enchant_Weapon_Damage_Fire[10][2];
extern short Enchant_Weapon_Damage_Lightning[10][2];
extern short Enchant_Weapon_Time[10];
extern short Enchant_Weapon_Mana[10];


extern short Dead_Ray_Damage[10];
extern short Dead_Ray_Range[10];
extern short Dead_Ray_Mana[10];


extern short Energy_Shield_DecDamage[10];
extern short Energy_Shield_Time[10];
extern short Energy_Shield_UseMana[10];


extern short Diastrophism_Damage[10];
extern short Diastrophism_Area[10];
extern short Diastrophism_UseMana[10];


extern float Spirit_Elemental_RegenMana[10];
extern short Spirit_Elemental_Damage[10];
extern short Spirit_Elemental_Time[10];
extern short Spirit_Elemental_UseMana[10];


extern short Dancing_Sword_IceDamage[10][2];
extern short Dancing_Sword_FireDamage[10][2];
extern short Dancing_Sword_AttackDelay[10];
extern short Dancing_Sword_Time[10];
extern short Dancing_Sword_UseMana[10];




extern short Fire_Elemental_Damage[10][2];
extern short Fire_Elemental_Hit[10];
extern short Fire_Elemental_Life[10];
extern short Fire_Elemental_UseMana[10];


extern short Flame_Wave_FireDamage[10];
extern short Flame_Wave_Area[10];
extern short Flame_Wave_UseMana[10];


extern short Distortion_SpeedSubPercent[10];
extern short Distortion_DamageSubPercent[10];
extern short Distortion_Area[10];
extern short Distortion_Time[10];
extern short Distortion_UseMana[10];


extern short Meteo_Damage[10];
extern short Meteo_Area[10];
extern short Meteo_UseMana[10];