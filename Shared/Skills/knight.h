#ifndef	KNIGHT_H
#define KNIGHT_H

#include "base.h"

struct Sword_Blast: skillPacket
{
	short Sword_Blast_UseMana[10];
	short Sword_Blast_Damage[10][2];
	short Sword_Blast_ShootingRange[10];

	Sword_Blast()
	{
		ZeroMemory(&Sword_Blast_UseMana, sizeof(Sword_Blast_UseMana));
		ZeroMemory(&Sword_Blast_Damage, sizeof(Sword_Blast_Damage));
		ZeroMemory(&Sword_Blast_ShootingRange, sizeof(Sword_Blast_ShootingRange));
	}
};

struct Holy_Body: skillPacket
{
	short HolyBody_UseMana[10];
	short HolyBody_Absorb[10];
	short HolyBody_Time[10];

	Holy_Body()
	{
		ZeroMemory(&HolyBody_UseMana, sizeof(HolyBody_UseMana));
		ZeroMemory(&HolyBody_Absorb, sizeof(HolyBody_Absorb));
		ZeroMemory(&HolyBody_Time, sizeof(HolyBody_Time));
	}
};

struct P_Traning: skillPacket
{
	short P_Traning_Stamina[10];

	P_Traning()
	{
		ZeroMemory(&P_Traning_Stamina, sizeof(P_Traning_Stamina));
	}
};

struct D_Crash: skillPacket
{
	short D_Crash_UseMana[10];
	short D_Crash_Damage[10];
	short D_Crash_Critical[10];

	D_Crash()
	{
		ZeroMemory(&D_Crash_UseMana, sizeof(D_Crash_UseMana));
		ZeroMemory(&D_Crash_Damage, sizeof(D_Crash_Damage));
		ZeroMemory(&D_Crash_Critical, sizeof(D_Crash_Critical));
	}
};

struct Holy_Valor: skillPacket
{
	short Holy_Valor_Damage[10];
	short Holy_Valor_Time[10];
	short Holy_Valor_Mana[10];

	Holy_Valor()
	{
		ZeroMemory(&Holy_Valor_Damage, sizeof(Holy_Valor_Damage));
		ZeroMemory(&Holy_Valor_Time, sizeof(Holy_Valor_Time));
		ZeroMemory(&Holy_Valor_Mana, sizeof(Holy_Valor_Mana));
	}
};

struct Brandish: skillPacket
{
	short Brandish_Range[10];
	short Brandish_Damage[10];
	short Brandish_Mana[10];

	Brandish()
	{
		ZeroMemory(&Brandish_Range, sizeof(Brandish_Range));
		ZeroMemory(&Brandish_Damage, sizeof(Brandish_Damage));
		ZeroMemory(&Brandish_Mana, sizeof(Brandish_Mana));
	}
};

struct Piercing: skillPacket
{
	short Piercing_Damage[10];
	short Piercing_Attack_Rating[10];
	short Piercing_Mana[10];

	Piercing()
	{
		ZeroMemory(&Piercing_Damage, sizeof(Piercing_Damage));
		ZeroMemory(&Piercing_Attack_Rating, sizeof(Piercing_Attack_Rating));
		ZeroMemory(&Piercing_Mana, sizeof(Piercing_Mana));
	}
};

struct Drastic_Spirit: skillPacket
{
	short Drastic_Spirit_Defense[10];
	short Drastic_Spirit_Time[10];
	short Drastic_Spirit_Mana[10];

	Drastic_Spirit()
	{
		ZeroMemory(&Drastic_Spirit_Defense, sizeof(Drastic_Spirit_Defense));
		ZeroMemory(&Drastic_Spirit_Time, sizeof(Drastic_Spirit_Time));
		ZeroMemory(&Drastic_Spirit_Mana, sizeof(Drastic_Spirit_Mana));
	}
};

struct Sword_Mastery: skillPacket
{
	short Sword_Mastery_DamagePercent[10];

	Sword_Mastery()
	{
		ZeroMemory(&Sword_Mastery_DamagePercent, sizeof(Sword_Mastery_DamagePercent));
	}
};

struct D_Inhalation: skillPacket
{
	short D_Inhalation_Life[10];
	short D_Inhalation_Block[10];
	short D_Inhalation_Time[10];
	short D_Inhalation_UseMana[10];

	D_Inhalation()
	{
		ZeroMemory(&D_Inhalation_Life, sizeof(D_Inhalation_Life));
		ZeroMemory(&D_Inhalation_Block, sizeof(D_Inhalation_Block));
		ZeroMemory(&D_Inhalation_Time, sizeof(D_Inhalation_Time));
		ZeroMemory(&D_Inhalation_UseMana, sizeof(D_Inhalation_UseMana));
	}
};

struct H_Incantation: skillPacket
{
	short H_Incantation_Success[10];
	short H_Incantation_AddLife[10];
	short H_Incantation_Time[10];
	short H_Incantation_UseMana[10];

	H_Incantation()
	{
		ZeroMemory(&H_Incantation_Success, sizeof(H_Incantation_Success));
		ZeroMemory(&H_Incantation_AddLife, sizeof(H_Incantation_AddLife));
		ZeroMemory(&H_Incantation_Time, sizeof(H_Incantation_Time));
		ZeroMemory(&H_Incantation_UseMana, sizeof(H_Incantation_UseMana));
	}
};

struct Grand_Cross: skillPacket
{
	short Grand_Cross_Damage[10];
	short Grand_Cross_AttackRate[10];
	short Grand_Cross_UndeadDamage[10];
	short Grand_Cross_UseMana[10];

	Grand_Cross()
	{
		ZeroMemory(&Grand_Cross_Damage, sizeof(Grand_Cross_Damage));
		ZeroMemory(&Grand_Cross_AttackRate, sizeof(Grand_Cross_AttackRate));
		ZeroMemory(&Grand_Cross_UndeadDamage, sizeof(Grand_Cross_UndeadDamage));
		ZeroMemory(&Grand_Cross_UseMana, sizeof(Grand_Cross_UseMana));
	}
};

struct Sword_Of_Justice: skillPacket
{
	short Sword_Of_Justice_DamagePercent[10];
	short Sword_Of_Justice_Area[10];
	short Sword_Of_Justice_UseMana[10];

	Sword_Of_Justice()
	{
		ZeroMemory(&Sword_Of_Justice_DamagePercent, sizeof(Sword_Of_Justice_DamagePercent));
		ZeroMemory(&Sword_Of_Justice_Area, sizeof(Sword_Of_Justice_Area));
		ZeroMemory(&Sword_Of_Justice_UseMana, sizeof(Sword_Of_Justice_UseMana));
	}
};

struct Godly_Shield: skillPacket
{
	short Godly_Shield_AbsorbPercent[10];
	short Godly_Shield_Time[10];
	short Godly_Shield_UseMana[10];

	Godly_Shield()
	{
		ZeroMemory(&Godly_Shield_AbsorbPercent, sizeof(Godly_Shield_AbsorbPercent));
		ZeroMemory(&Godly_Shield_Time, sizeof(Godly_Shield_Time));
		ZeroMemory(&Godly_Shield_UseMana, sizeof(Godly_Shield_UseMana));
	}
};

struct God_Bless: skillPacket
{
	short God_Bless_AddDamage[10];
	short God_Bless_Time[10];
	short God_Bless_UseMana[10];

	God_Bless()
	{
		ZeroMemory(&God_Bless_AddDamage, sizeof(God_Bless_AddDamage));
		ZeroMemory(&God_Bless_Time, sizeof(God_Bless_Time));
		ZeroMemory(&God_Bless_UseMana, sizeof(God_Bless_UseMana));
	}
};

struct Divine_Piercing: skillPacket
{
	short Divine_Piercing_DamagePercent[10];
	short Divine_Piercing_AddHit[10];
	short Divine_Piercing_AttackNum[10];
	short Divine_Piercing_UseMana[10];

	Divine_Piercing()
	{
		ZeroMemory(&Divine_Piercing_DamagePercent, sizeof(Divine_Piercing_DamagePercent));
		ZeroMemory(&Divine_Piercing_AddHit, sizeof(Divine_Piercing_AddHit));
		ZeroMemory(&Divine_Piercing_AttackNum, sizeof(Divine_Piercing_AttackNum));
		ZeroMemory(&Divine_Piercing_UseMana, sizeof(Divine_Piercing_UseMana));
	}
};


#endif