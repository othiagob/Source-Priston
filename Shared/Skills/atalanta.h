#ifndef	ATALANTA_H
#define ATALANTA_H

#include "base.h"

struct S_Strike: skillPacket
{
	short S_Strike_UseMana[10];
	short S_Strike_Damage[10][2];

	S_Strike()
	{
		ZeroMemory(&S_Strike_UseMana, sizeof(S_Strike_UseMana));
		ZeroMemory(&S_Strike_Damage, sizeof(S_Strike_Damage));
	}
};

struct Farina: skillPacket
{
	short Farina_UseMana[10];
	short Farina_Speed[10];
	short Farina_AttackRate[10];

	Farina()
	{
		ZeroMemory(&Farina_UseMana, sizeof(Farina_UseMana));
		ZeroMemory(&Farina_Speed, sizeof(Farina_Speed));
		ZeroMemory(&Farina_AttackRate, sizeof(Farina_AttackRate));
	}
};

struct T_Mastery: skillPacket
{
	short T_Mastery_Damage[10];

	T_Mastery()
	{
		ZeroMemory(&T_Mastery_Damage, sizeof(T_Mastery_Damage));
	}
};

struct V_Spear: skillPacket
{
	short V_Spear_UseMana[10];
	short V_Spear_Damage[10][2];

	V_Spear()
	{
		ZeroMemory(&V_Spear_UseMana, sizeof(V_Spear_UseMana));
		ZeroMemory(&V_Spear_Damage, sizeof(V_Spear_Damage));
	}
};

struct Wind: skillPacket
{
	short Windy_Attack_Rating[10];
	short Windy_Time[10];
	short Windy_Mana[10];

	Wind()
	{
		ZeroMemory(&Windy_Attack_Rating, sizeof(Windy_Attack_Rating));
		ZeroMemory(&Windy_Time, sizeof(Windy_Time));
		ZeroMemory(&Windy_Mana, sizeof(Windy_Mana));
	}
};

struct Twist_Javelin: skillPacket
{
	short Twist_Javelin_Damage[10];
	short Twist_Javelin_Attack_Rating[10];
	short Twist_Javelin_Mana[10];

	Twist_Javelin()
	{
		ZeroMemory(&Twist_Javelin_Damage, sizeof(Twist_Javelin_Damage));
		ZeroMemory(&Twist_Javelin_Attack_Rating, sizeof(Twist_Javelin_Attack_Rating));
		ZeroMemory(&Twist_Javelin_Mana, sizeof(Twist_Javelin_Mana));
	}
};

struct Soul_Sucker: skillPacket
{
	short Soul_Sucker_Absorb[10];
	short Soul_Sucker_Mana[10];

	Soul_Sucker()
	{
		ZeroMemory(&Soul_Sucker_Absorb, sizeof(Soul_Sucker_Absorb));
		ZeroMemory(&Soul_Sucker_Mana, sizeof(Soul_Sucker_Mana));
	}
};

struct Fire_Javelin: skillPacket
{
	short Fire_Javelin_Damage[10][2];
	short Fire_Javelin_Mana[10];

	Fire_Javelin()
	{
		ZeroMemory(&Fire_Javelin_Damage, sizeof(Fire_Javelin_Damage));
		ZeroMemory(&Fire_Javelin_Mana, sizeof(Fire_Javelin_Mana));
	}
};

struct Split_Javelin: skillPacket
{
	short Split_Javelin_AttackNum[10];
	short Split_Javelin_Damage[10];
	short Split_Javelin_AttackRate[10];
	short Split_Javelin_UseMana[10];

	Split_Javelin()
	{
		ZeroMemory(&Split_Javelin_AttackNum, sizeof(Split_Javelin_AttackNum));
		ZeroMemory(&Split_Javelin_Damage, sizeof(Split_Javelin_Damage));
		ZeroMemory(&Split_Javelin_AttackRate, sizeof(Split_Javelin_AttackRate));
		ZeroMemory(&Split_Javelin_UseMana, sizeof(Split_Javelin_UseMana));
	}
};

struct T_Of_Valhalla: skillPacket
{
	short T_Of_Valhalla_Damage[10];
	short T_Of_Valhalla_Time[10];
	short T_Of_Valhalla_UseMana[10];

	T_Of_Valhalla()
	{
		ZeroMemory(&T_Of_Valhalla_Damage, sizeof(T_Of_Valhalla_Damage));
		ZeroMemory(&T_Of_Valhalla_Time, sizeof(T_Of_Valhalla_Time));
		ZeroMemory(&T_Of_Valhalla_UseMana, sizeof(T_Of_Valhalla_UseMana));
	}
};

struct Lightning_Javelin: skillPacket
{
	short Lightning_Javelin_Damage[10][2];
	short Lightning_Javelin_UseMana[10];

	Lightning_Javelin()
	{
		ZeroMemory(&Lightning_Javelin_Damage, sizeof(Lightning_Javelin_Damage));
		ZeroMemory(&Lightning_Javelin_UseMana, sizeof(Lightning_Javelin_UseMana));
	}
};

struct Storm_Javelin: skillPacket
{
	short Storm_Javelin_Damage[10];
	short Storm_Javelin_UseMana[10];

	Storm_Javelin()
	{
		ZeroMemory(&Storm_Javelin_Damage, sizeof(Storm_Javelin_Damage));
		ZeroMemory(&Storm_Javelin_UseMana, sizeof(Storm_Javelin_UseMana));
	}
};

struct Hall_Of_Valhalla: skillPacket
{
	short Hall_Of_Valhalla_AddEvation[10];
	short Hall_Of_Valhalla_Time[10];
	short Hall_Of_Valhalla_UseMana[10];

	Hall_Of_Valhalla()
	{
		ZeroMemory(&Hall_Of_Valhalla_AddEvation, sizeof(Hall_Of_Valhalla_AddEvation));
		ZeroMemory(&Hall_Of_Valhalla_Time, sizeof(Hall_Of_Valhalla_Time));
		ZeroMemory(&Hall_Of_Valhalla_UseMana, sizeof(Hall_Of_Valhalla_UseMana));
	}
};

struct X_Rage: skillPacket
{
	short X_Rage_DamagePercent[10];
	short X_Rage_Area[10];
	short X_Rage_UseMana[10];

	X_Rage()
	{
		ZeroMemory(&X_Rage_DamagePercent, sizeof(X_Rage_DamagePercent));
		ZeroMemory(&X_Rage_Area, sizeof(X_Rage_Area));
		ZeroMemory(&X_Rage_UseMana, sizeof(X_Rage_UseMana));
	}
};

struct Frost_Javelin: skillPacket
{
	short Frost_Javelin_IceAddDamage[10][2];
	short Frost_Javelin_SpeedSubPercent[10];
	short Frost_Javelin_IceTime[10];
	short Frost_Javelin_Time[10];
	short Frost_Javelin_UseMana[10];

	Frost_Javelin()
	{
		ZeroMemory(&Frost_Javelin_IceAddDamage, sizeof(Frost_Javelin_IceAddDamage));
		ZeroMemory(&Frost_Javelin_SpeedSubPercent, sizeof(Frost_Javelin_SpeedSubPercent));
		ZeroMemory(&Frost_Javelin_IceTime, sizeof(Frost_Javelin_IceTime));
		ZeroMemory(&Frost_Javelin_Time, sizeof(Frost_Javelin_Time));
		ZeroMemory(&Frost_Javelin_UseMana, sizeof(Frost_Javelin_UseMana));
	}
};

struct Vengeance: skillPacket
{
	short Vengeance_DamagePercent[10];
	short Vengeance_Range[10];
	short Vengeance_UseMana[10];
	short Vengeance_AddCritical[10];

	Vengeance()
	{
		ZeroMemory(&Vengeance_DamagePercent, sizeof(Vengeance_DamagePercent));
		ZeroMemory(&Vengeance_Range, sizeof(Vengeance_Range));
		ZeroMemory(&Vengeance_UseMana, sizeof(Vengeance_UseMana));
		ZeroMemory(&Vengeance_AddCritical, sizeof(Vengeance_AddCritical));
	}
};

#endif