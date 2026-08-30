#ifndef	MECHANICIAN_H
#define MECHANICIAN_H

#include "base.h"

struct E_Shield: skillPacket
{
	short E_Shield_BlockRate[10];
	short E_Shield_UseTime[10];
	short E_Shield_UseMana[10];

	E_Shield()
	{
		ZeroMemory(&E_Shield_BlockRate, sizeof(E_Shield_BlockRate));
		ZeroMemory(&E_Shield_UseTime, sizeof(E_Shield_UseTime));
		ZeroMemory(&E_Shield_UseMana, sizeof(E_Shield_UseMana));
	}
};

struct Mechanic_Bomb: skillPacket
{
	short Mechanic_Bomb_Damage[10][2];
	short Mechanic_Bomb_Attack_Range[10];
	short Mechanic_Bomb_UseMana[10];

	Mechanic_Bomb()
	{
		ZeroMemory(&Mechanic_Bomb_Damage, sizeof(Mechanic_Bomb_Damage));
		ZeroMemory(&Mechanic_Bomb_Attack_Range, sizeof(Mechanic_Bomb_Attack_Range));
		ZeroMemory(&Mechanic_Bomb_UseMana, sizeof(Mechanic_Bomb_UseMana));
	}
};

struct Physical_Absorb: skillPacket
{
	short P_Absorb[10][2];
	short P_Absorb_UseTime[10];
	short P_Absor_bUseMana[10];

	Physical_Absorb()
	{
		ZeroMemory(&P_Absorb, sizeof(P_Absorb));
		ZeroMemory(&P_Absorb_UseTime, sizeof(P_Absorb_UseTime));
		ZeroMemory(&P_Absor_bUseMana, sizeof(P_Absor_bUseMana));
	}
};

struct Poison_Resistence: skillPacket
{
	short PlusPoison[10];


	Poison_Resistence()
	{
		ZeroMemory(&PlusPoison, sizeof(PlusPoison));
	}
};

struct G_Smash: skillPacket
{
	short G_Smash_Attack_Rate[10];
	short G_Smash_Attack_Rage[10];
	short G_Smash_UseMana[10];

	G_Smash()
	{
		ZeroMemory(&G_Smash_Attack_Rate, sizeof(G_Smash_Attack_Rate));
		ZeroMemory(&G_Smash_Attack_Rage, sizeof(G_Smash_Attack_Rage));
		ZeroMemory(&G_Smash_UseMana, sizeof(G_Smash_UseMana));
	}
};

struct Maximize: skillPacket
{
	short Maximize_Damage[10];
	short Maximize_Time[10];
	short Maximize_UseMana[10];

	Maximize()
	{
		ZeroMemory(&Maximize_Damage, sizeof(Maximize_Damage));
		ZeroMemory(&Maximize_Time, sizeof(Maximize_Time));
		ZeroMemory(&Maximize_UseMana, sizeof(Maximize_UseMana));
	}
};

struct Automation: skillPacket
{
	short Automation_Damage[10];
	short Automation_Speed[10];
	short Automation_Time[10];
	short Automation_UseMana[10];

	Automation()
	{
		ZeroMemory(&Automation_Damage, sizeof(Automation_Damage));
		ZeroMemory(&Automation_Speed, sizeof(Automation_Speed));
		ZeroMemory(&Automation_Time, sizeof(Automation_Time));
		ZeroMemory(&Automation_UseMana, sizeof(Automation_UseMana));
	}
};

struct Spark: skillPacket
{
	short Spark_Damage[10][2];
	short Spark_Area[10];
	short Spark_UseMana[10];

	Spark()
	{
		ZeroMemory(&Spark_Damage, sizeof(Spark_Damage));
		ZeroMemory(&Spark_Area, sizeof(Spark_Area));
		ZeroMemory(&Spark_UseMana, sizeof(Spark_UseMana));
	}
};

struct Metal_Armor: skillPacket
{
	short Metal_Armor_Defense[10];
	short Metal_Armor_Time[10];
	short Metal_Armor_UseMana[10];

	Metal_Armor()
	{
		ZeroMemory(&Metal_Armor_Defense, sizeof(Metal_Armor_Defense));
		ZeroMemory(&Metal_Armor_Time, sizeof(Metal_Armor_Time));
		ZeroMemory(&Metal_Armor_UseMana, sizeof(Metal_Armor_UseMana));
	}
};

struct Grand_Smash: skillPacket
{
	short Grand_Smash_AttackRate[10];
	short Grand_Smash_Damage[10];
	short Grand_Smash_UseMana[10];

	Grand_Smash()
	{
		ZeroMemory(&Grand_Smash_AttackRate, sizeof(Grand_Smash_AttackRate));
		ZeroMemory(&Grand_Smash_Damage, sizeof(Grand_Smash_Damage));
		ZeroMemory(&Grand_Smash_UseMana, sizeof(Grand_Smash_UseMana));
	}
};

struct Mechanician_Weapon_Mastery: skillPacket
{
	short M_Weapon_Mastey[10];

	Mechanician_Weapon_Mastery()
	{
		ZeroMemory(&M_Weapon_Mastey, sizeof(M_Weapon_Mastey));
	}
};

struct Spark_Shield: skillPacket
{
	short Spark_Shield_Damage[10];
	short Spark_Shield_Defense[10];
	short Spark_Shield_Time[10];
	short Spark_Shield_UseMana[10];

	Spark_Shield()
	{
		ZeroMemory(&Spark_Shield_Damage, sizeof(Spark_Shield_Damage));
		ZeroMemory(&Spark_Shield_Defense, sizeof(Spark_Shield_Defense));
		ZeroMemory(&Spark_Shield_Time, sizeof(Spark_Shield_Time));
		ZeroMemory(&Spark_Shield_Time, sizeof(Spark_Shield_UseMana));
	}
};

struct Impulsion: skillPacket
{
	short Impulsion_LightingDamage[10][2];
	short Impulsion_Range[10];
	short Impulsion_LightingNum[10];
	short Impulsion_UseMana[10];

	Impulsion()
	{
		ZeroMemory(&Impulsion_LightingDamage, sizeof(Impulsion_LightingDamage));
		ZeroMemory(&Impulsion_Range, sizeof(Impulsion_Range));
		ZeroMemory(&Impulsion_LightingNum, sizeof(Impulsion_LightingNum));
		ZeroMemory(&Impulsion_UseMana, sizeof(Impulsion_UseMana));
	}
};

struct Compulsion: skillPacket
{
	short Compulsion_AddAbsorb[10];
	short Compulsion_Area[10];
	short Compulsion_Time[10];
	short Compulsion_UseMana[10];

	Compulsion()
	{
		ZeroMemory(&Compulsion_AddAbsorb, sizeof(Compulsion_AddAbsorb));
		ZeroMemory(&Compulsion_Area, sizeof(Compulsion_Area));
		ZeroMemory(&Compulsion_Time, sizeof(Compulsion_Time));
		ZeroMemory(&Compulsion_UseMana, sizeof(Compulsion_UseMana));
	}
};

struct Magnetic_Sphere: skillPacket
{
	short Magnetic_Sphere_AddDamage[10][2];
	short Magnetic_Sphere_Area[10];
	short Magnetic_Sphere_AttackDelay[10];
	short Magnetic_Sphere_Time[10];
	short Magnetic_Sphere_UseMana[10];

	Magnetic_Sphere()
	{
		ZeroMemory(&Magnetic_Sphere_AddDamage, sizeof(Magnetic_Sphere_AddDamage));
		ZeroMemory(&Magnetic_Sphere_Area, sizeof(Magnetic_Sphere_Area));
		ZeroMemory(&Magnetic_Sphere_AttackDelay, sizeof(Magnetic_Sphere_AttackDelay));
		ZeroMemory(&Magnetic_Sphere_Time, sizeof(Magnetic_Sphere_Time));
		ZeroMemory(&Magnetic_Sphere_UseMana, sizeof(Magnetic_Sphere_UseMana));
	}
};

struct Metal_Golem: skillPacket
{
	short Metal_Golem_Damage[10][2];
	short Metal_Golem_Hit[10];
	short Metal_Golem_Defense[10];
	short Metal_Golem_Life[10];
	short Metal_Golem_UseMana[10];

	Metal_Golem()
	{
		ZeroMemory(&Metal_Golem_Damage, sizeof(Metal_Golem_Damage));
		ZeroMemory(&Metal_Golem_Hit, sizeof(Metal_Golem_Hit));
		ZeroMemory(&Metal_Golem_Defense, sizeof(Metal_Golem_Defense));
		ZeroMemory(&Metal_Golem_Life, sizeof(Metal_Golem_Life));
		ZeroMemory(&Metal_Golem_UseMana, sizeof(Metal_Golem_UseMana));
	}
};

#endif