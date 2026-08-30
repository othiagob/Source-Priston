#ifndef	MAGICIAN_H
#define MAGICIAN_H

#include "base.h"

struct Agony: skillPacket
{
	short Agony_UseMana[10];
	short Agony_ConvLife[10];

	Agony()
	{
		ZeroMemory(&Agony_UseMana, sizeof(Agony_UseMana));
		ZeroMemory(&Agony_ConvLife, sizeof(Agony_ConvLife));
	}
};

struct FireBolt: skillPacket
{
	short FireBolt_UseMana[10];
	short FireBolt_Damage[10][2];

	FireBolt()
	{
		ZeroMemory(&FireBolt_UseMana, sizeof(FireBolt_UseMana));
		ZeroMemory(&FireBolt_Damage, sizeof(FireBolt_Damage));
	}
};

struct Zenith: skillPacket
{
	short Zenith_UseMana[10];
	short Zenith_Element[10];
	short Zenith_Time[10];

	Zenith()
	{
		ZeroMemory(&Zenith_UseMana, sizeof(Zenith_UseMana));
		ZeroMemory(&Zenith_Element, sizeof(Zenith_Element));
		ZeroMemory(&Zenith_Time, sizeof(Zenith_Time));
	}
};

struct FireBall: skillPacket
{
	short FireBall_UseMana[10];
	short FireBall_Damage[10];
	short FireBall_Area[10];
	short FireBall_Range[10];

	FireBall()
	{
		ZeroMemory(&FireBall_UseMana, sizeof(FireBall_UseMana));
		ZeroMemory(&FireBall_Damage, sizeof(FireBall_Damage));
		ZeroMemory(&FireBall_Area, sizeof(FireBall_Area));
		ZeroMemory(&FireBall_Range, sizeof(FireBall_Range));
	}
};

struct Mental_Mastery: skillPacket
{
	short Mental_Mastery_Mana[10];

	Mental_Mastery()
	{
		ZeroMemory(&Mental_Mastery_Mana, sizeof(Mental_Mastery_Mana));
	}
};

struct Watornado: skillPacket
{
	short Watornado_Damage[10];
	short Watornado_Area[10];
	short Watornado_Range[10];
	short Watornado_Mana[10];

	Watornado()
	{
		ZeroMemory(&Watornado_Damage, sizeof(Watornado_Damage));
		ZeroMemory(&Watornado_Area, sizeof(Watornado_Area));
		ZeroMemory(&Watornado_Range, sizeof(Watornado_Range));
		ZeroMemory(&Watornado_Mana, sizeof(Watornado_Mana));
	}
};

struct Enchant_Weapon: skillPacket
{
	short Enchant_Weapon_Damage_Ice[10][2];
	short Enchant_Weapon_Damage_Fire[10][2];
	short Enchant_Weapon_Damage_Lightning[10][2];
	short Enchant_Weapon_Time[10];
	short Enchant_Weapon_Mana[10];

	Enchant_Weapon()
	{
		ZeroMemory(&Enchant_Weapon_Damage_Ice, sizeof(Enchant_Weapon_Damage_Ice));
		ZeroMemory(&Enchant_Weapon_Damage_Fire, sizeof(Enchant_Weapon_Damage_Fire));
		ZeroMemory(&Enchant_Weapon_Damage_Lightning, sizeof(Enchant_Weapon_Damage_Lightning));
		ZeroMemory(&Enchant_Weapon_Time, sizeof(Enchant_Weapon_Time));
		ZeroMemory(&Enchant_Weapon_Mana, sizeof(Enchant_Weapon_Mana));
	}
};

struct Dead_Ray: skillPacket
{
	short Dead_Ray_Damage[10];
	short Dead_Ray_Range[10];
	short Dead_Ray_Mana[10];

	Dead_Ray()
	{
		ZeroMemory(&Dead_Ray_Damage, sizeof(Dead_Ray_Damage));
		ZeroMemory(&Dead_Ray_Range, sizeof(Dead_Ray_Range));
		ZeroMemory(&Dead_Ray_Mana, sizeof(Dead_Ray_Mana));
	}
};

struct Energy_Shield: skillPacket
{
	short Energy_Shield_DecDamage[10];
	short Energy_Shield_Time[10];
	short Energy_Shield_UseMana[10];

	Energy_Shield()
	{
		ZeroMemory(&Energy_Shield_DecDamage, sizeof(Energy_Shield_DecDamage));
		ZeroMemory(&Energy_Shield_Time, sizeof(Energy_Shield_Time));
		ZeroMemory(&Energy_Shield_UseMana, sizeof(Energy_Shield_UseMana));
	}
};

struct Diastrophism: skillPacket
{
	short Diastrophism_Damage[10];
	short Diastrophism_Area[10];
	short Diastrophism_UseMana[10];

	Diastrophism()
	{
		ZeroMemory(&Diastrophism_Damage, sizeof(Diastrophism_Damage));
		ZeroMemory(&Diastrophism_Area, sizeof(Diastrophism_Area));
		ZeroMemory(&Diastrophism_UseMana, sizeof(Diastrophism_UseMana));
	}
};

struct Spirit_Elemental: skillPacket
{
	float Spirit_Elemental_RegenMana[10];
	short Spirit_Elemental_Damage[10];
	short Spirit_Elemental_Time[10];
	short Spirit_Elemental_UseMana[10];

	Spirit_Elemental()
	{
		ZeroMemory(&Spirit_Elemental_RegenMana, sizeof(Spirit_Elemental_RegenMana));
		ZeroMemory(&Spirit_Elemental_Damage, sizeof(Spirit_Elemental_Damage));
		ZeroMemory(&Spirit_Elemental_Time, sizeof(Spirit_Elemental_Time));
		ZeroMemory(&Spirit_Elemental_UseMana, sizeof(Spirit_Elemental_UseMana));
	}
};

struct Dancing_Sword: skillPacket
{
	short Dancing_Sword_IceDamage[10][2];
	short Dancing_Sword_FireDamage[10][2];
	short Dancing_Sword_AttackDelay[10];
	short Dancing_Sword_Time[10];
	short Dancing_Sword_UseMana[10];

	Dancing_Sword()
	{
		ZeroMemory(&Dancing_Sword_IceDamage, sizeof(Dancing_Sword_IceDamage));
		ZeroMemory(&Dancing_Sword_FireDamage, sizeof(Dancing_Sword_FireDamage));
		ZeroMemory(&Dancing_Sword_AttackDelay, sizeof(Dancing_Sword_AttackDelay));
		ZeroMemory(&Dancing_Sword_Time, sizeof(Dancing_Sword_Time));
		ZeroMemory(&Dancing_Sword_UseMana, sizeof(Dancing_Sword_UseMana));
	}
};

struct Fire_Elemental: skillPacket
{
	short Fire_Elemental_Damage[10][2];
	short Fire_Elemental_Hit[10];
	short Fire_Elemental_Life[10];
	short Fire_Elemental_UseMana[10];

	Fire_Elemental()
	{
		ZeroMemory(&Fire_Elemental_Damage, sizeof(Fire_Elemental_Damage));
		ZeroMemory(&Fire_Elemental_Hit, sizeof(Fire_Elemental_Hit));
		ZeroMemory(&Fire_Elemental_Life, sizeof(Fire_Elemental_Life));
		ZeroMemory(&Fire_Elemental_UseMana, sizeof(Fire_Elemental_UseMana));
	}
};

struct Flame_Wave: skillPacket
{
	short Flame_Wave_FireDamage[10];
	short Flame_Wave_Area[10];
	short Flame_Wave_UseMana[10];

	Flame_Wave()
	{
		ZeroMemory(&Flame_Wave_FireDamage, sizeof(Flame_Wave_FireDamage));
		ZeroMemory(&Flame_Wave_Area, sizeof(Flame_Wave_Area));
		ZeroMemory(&Flame_Wave_UseMana, sizeof(Flame_Wave_UseMana));
	}
};

struct Distortion: skillPacket
{
	short Distortion_SpeedSubPercent[10];
	short Distortion_DamageSubPercent[10];
	short Distortion_Area[10];
	short Distortion_Time[10];
	short Distortion_UseMana[10];

	Distortion()
	{
		ZeroMemory(&Distortion_SpeedSubPercent, sizeof(Distortion_SpeedSubPercent));
		ZeroMemory(&Distortion_DamageSubPercent, sizeof(Distortion_DamageSubPercent));
		ZeroMemory(&Distortion_Area, sizeof(Distortion_Area));
		ZeroMemory(&Distortion_Time, sizeof(Distortion_Time));
		ZeroMemory(&Distortion_UseMana, sizeof(Distortion_UseMana));
	}
};

struct Meteo: skillPacket
{
	short Meteo_Damage[10];
	short Meteo_Area[10];
	short Meteo_UseMana[10];

	Meteo()
	{
		ZeroMemory(&Meteo_Damage, sizeof(Meteo_Damage));
		ZeroMemory(&Meteo_Area, sizeof(Meteo_Area));
		ZeroMemory(&Meteo_UseMana, sizeof(Meteo_UseMana));
	}
};

#endif