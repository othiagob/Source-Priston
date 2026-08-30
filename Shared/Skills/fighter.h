#ifndef	FIGHTER_H
#define FIGHTER_H

#include "base.h"

struct Melee_Mastery: skillPacket
{
	short Melee_Mastery_DamagePercent[10];

	Melee_Mastery()
	{
		ZeroMemory(&Melee_Mastery_DamagePercent, sizeof(Melee_Mastery_DamagePercent));
	}
};

struct Plus_Fire: skillPacket
{
	short PlusFire[10];

	Plus_Fire()
	{
		ZeroMemory(&PlusFire, sizeof(PlusFire));
	}
};

struct Raving: skillPacket
{
	short Raving_Damage[10];
	short Ravind_Speed[10];
	short Raving_UseLife[10];
	short Raving_UseMana[10];

	Raving()
	{
		ZeroMemory(&Raving_Damage, sizeof(Raving_Damage));
		ZeroMemory(&Ravind_Speed, sizeof(Ravind_Speed));
		ZeroMemory(&Raving_UseLife, sizeof(Raving_UseLife));
		ZeroMemory(&Raving_UseMana, sizeof(Raving_UseMana));
	}
};

struct Impact: skillPacket
{
	short Impact_Attack_Rating[10];
	short Impact_Damage[10];
	short Impact_UseMana[10];

	Impact()
	{
		ZeroMemory(&Impact_Attack_Rating, sizeof(Impact_Attack_Rating));
		ZeroMemory(&Impact_Damage, sizeof(Impact_Damage));
		ZeroMemory(&Impact_UseMana, sizeof(Impact_UseMana));
	}
};

struct T_Impact: skillPacket
{
	short T_Impact_Damage[10];
	short T_Impact_Hit[10];
	short T_Impact_UseMana[10];

	T_Impact()
	{
		ZeroMemory(&T_Impact_Damage, sizeof(T_Impact_Damage));
		ZeroMemory(&T_Impact_Hit, sizeof(T_Impact_Hit));
		ZeroMemory(&T_Impact_UseMana, sizeof(T_Impact_UseMana));
	}
};

struct B_Swing: skillPacket
{
	short B_Swing_Damage[10];
	short B_Swing_Critical[10];
	short B_Swing_UseMana[10];

	B_Swing()
	{
		ZeroMemory(&B_Swing_Damage, sizeof(B_Swing_Damage));
		ZeroMemory(&B_Swing_Critical, sizeof(B_Swing_Critical));
		ZeroMemory(&B_Swing_UseMana, sizeof(B_Swing_UseMana));
	}
};

struct Roar: skillPacket
{
	short Roar_Range[10];
	short Roar_Time[10];
	short Roar_UseMana[10];

	Roar()
	{
		ZeroMemory(&Roar_Range, sizeof(Roar_Range));
		ZeroMemory(&Roar_Time, sizeof(Roar_Time));
		ZeroMemory(&Roar_UseMana, sizeof(Roar_UseMana));
	}
};

struct R_Zecram: skillPacket
{
	short R_Zecram_Damage[10];
	short R_Zecram_UseMana[10];

	R_Zecram()
	{
		ZeroMemory(&R_Zecram_Damage, sizeof(R_Zecram_Damage));
		ZeroMemory(&R_Zecram_UseMana, sizeof(R_Zecram_UseMana));
	}
};

struct Concentration: skillPacket
{
	short Concentration_AttackRate[10];
	short Concentration_Time[10];
	short Concentration_UseMana[10];

	Concentration()
	{
		ZeroMemory(&Concentration_AttackRate, sizeof(Concentration_AttackRate));
		ZeroMemory(&Concentration_Time, sizeof(Concentration_Time));
		ZeroMemory(&Concentration_UseMana, sizeof(Concentration_UseMana));
	}
};

struct A_Crash: skillPacket
{
	short A_Crash_Damage[10];
	short A_Crash_AttackRate[10];
	short A_Crash_UseMana[10];

	A_Crash()
	{
		ZeroMemory(&A_Crash_Damage, sizeof(A_Crash_Damage));
		ZeroMemory(&A_Crash_AttackRate, sizeof(A_Crash_AttackRate));
		ZeroMemory(&A_Crash_UseMana, sizeof(A_Crash_UseMana));
	}
};

struct Swift_Axe: skillPacket
{
	short Swift_Axe_Speed[10];
	short Swift_Axe_Time[10];
	short Swift_Axe_UseMana[10];

	Swift_Axe()
	{
		ZeroMemory(&Swift_Axe_Speed, sizeof(Swift_Axe_Speed));
		ZeroMemory(&Swift_Axe_Time, sizeof(Swift_Axe_Time));
		ZeroMemory(&Swift_Axe_UseMana, sizeof(Swift_Axe_UseMana));
	}
};

struct B_Crash: skillPacket
{
	short B_Crash_Damage[10];
	short B_Crash_DemonDamage[10];
	short B_Crash_UseMana[10];

	B_Crash()
	{
		ZeroMemory(&B_Crash_Damage, sizeof(B_Crash_Damage));
		ZeroMemory(&B_Crash_DemonDamage, sizeof(B_Crash_DemonDamage));
		ZeroMemory(&B_Crash_UseMana, sizeof(B_Crash_UseMana));
	}
};

struct Destroyer: skillPacket
{
	short Destoryer_DamagePercent[10];
	short Destoryer_AddCritical[10];
	short Destoryer_UseMana[10];

	Destroyer()
	{
		ZeroMemory(&Destoryer_DamagePercent, sizeof(Destoryer_DamagePercent));
		ZeroMemory(&Destoryer_AddCritical, sizeof(Destoryer_AddCritical));
		ZeroMemory(&Destoryer_UseMana, sizeof(Destoryer_UseMana));
	}
};

struct Berserker: skillPacket
{
	short Berserker_AddAttack[10];
	short Berserker_SubAbsorb[10];
	short Berserker_Time[10];
	short Berserker_UseMana[10];

	Berserker()
	{
		ZeroMemory(&Berserker_AddAttack, sizeof(Berserker_AddAttack));
		ZeroMemory(&Berserker_SubAbsorb, sizeof(Berserker_SubAbsorb));
		ZeroMemory(&Berserker_Time, sizeof(Berserker_Time));
		ZeroMemory(&Berserker_UseMana, sizeof(Berserker_UseMana));
	}
};

struct Cyclone_Strike: skillPacket
{
	short Cyclone_Strike_DamagePercent[10];
	short Cyclone_Strike_AreaDamage[10][2];
	short Cyclone_Strike_AttackNum[10];
	short Cyclone_Strike_Area[10];
	short Cyclone_Strike_UseMana[10];

	Cyclone_Strike()
	{
		ZeroMemory(&Cyclone_Strike_DamagePercent, sizeof(Cyclone_Strike_DamagePercent));
		ZeroMemory(&Cyclone_Strike_AreaDamage, sizeof(Cyclone_Strike_AreaDamage));
		ZeroMemory(&Cyclone_Strike_AttackNum, sizeof(Cyclone_Strike_AttackNum));
		ZeroMemory(&Cyclone_Strike_Area, sizeof(Cyclone_Strike_Area));
		ZeroMemory(&Cyclone_Strike_UseMana, sizeof(Cyclone_Strike_UseMana));
	}
};

struct Boost_Health: skillPacket
{
	short Boost_Health_Life[10];

	Boost_Health()
	{
		ZeroMemory(&Boost_Health_Life, sizeof(Boost_Health_Life));
	}
};

#endif