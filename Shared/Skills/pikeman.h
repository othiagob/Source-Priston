#ifndef	PIKEMAN_H
#define PIKEMAN_H

#include "base.h"

struct Pike_Wind: skillPacket
{
	short Pike_Wind_Damage[10][2];
	short Pike_Wind_Push_Lenght[10];
	short Pike_Wind_UseMana[10];

	Pike_Wind()
	{
		ZeroMemory(&Pike_Wind_Damage, sizeof(Pike_Wind_Damage));
		ZeroMemory(&Pike_Wind_Push_Lenght, sizeof(Pike_Wind_Push_Lenght));
		ZeroMemory(&Pike_Wind_UseMana, sizeof(Pike_Wind_UseMana));
	}
};

struct Plus_Ice: skillPacket
{
	short PlusIce[10];

	Plus_Ice()
	{
		ZeroMemory(&PlusIce, sizeof(PlusIce));
	}
};

struct Critical_Hit: skillPacket
{
	short Critical_Hit_Critical[10];
	short Critical_Hit_UseMana[10];

	Critical_Hit()
	{
		ZeroMemory(&Critical_Hit_Critical, sizeof(Critical_Hit_Critical));
		ZeroMemory(&Critical_Hit_UseMana, sizeof(Critical_Hit_UseMana));
	}
};

struct Jumping_Crash: skillPacket
{
	short Jumping_Crash_UseMana[10];
	short Jumping_Crash_Attack_Rating[10];
	short Jumping_Crash_Damage[10];

	Jumping_Crash()
	{
		ZeroMemory(&Jumping_Crash_UseMana, sizeof(Jumping_Crash_UseMana));
		ZeroMemory(&Jumping_Crash_Attack_Rating, sizeof(Jumping_Crash_Attack_Rating));
		ZeroMemory(&Jumping_Crash_Damage, sizeof(Jumping_Crash_Damage));
	}
};

struct G_Pike: skillPacket
{
	short G_Pike_Damage[10][2];
	short G_Pike_Range[10];
	short G_Pike_UseMana[10];
	short G_Pike_Time[10];

	G_Pike()
	{
		ZeroMemory(&G_Pike_Damage, sizeof(G_Pike_Damage));
		ZeroMemory(&G_Pike_Range, sizeof(G_Pike_Range));
		ZeroMemory(&G_Pike_UseMana, sizeof(G_Pike_UseMana));
		ZeroMemory(&G_Pike_Time, sizeof(G_Pike_Time));
	}
};

struct Tornado: skillPacket
{
	short Tornado_Damage[10];
	short Tornado_Stun[10];
	short Tornado_Range[10];
	short Tornado_UseMana[10];

	Tornado()
	{
		ZeroMemory(&Tornado_Damage, sizeof(Tornado_Damage));
		ZeroMemory(&Tornado_Stun, sizeof(Tornado_Stun));
		ZeroMemory(&Tornado_Range, sizeof(Tornado_Range));
		ZeroMemory(&Tornado_UseMana, sizeof(Tornado_UseMana));
	}
};

struct W_D_Mastery: skillPacket
{
	short W_D_Mastery_Block[10];

	W_D_Mastery()
	{
		ZeroMemory(&W_D_Mastery_Block, sizeof(W_D_Mastery_Block));
	}
};

struct Expansion: skillPacket
{
	short Expansion_Size[10];
	short Expansion_Damage[10];
	short Expansion_UseMana[10];

	Expansion()
	{
		ZeroMemory(&Expansion_Size, sizeof(Expansion_Size));
		ZeroMemory(&Expansion_Damage, sizeof(Expansion_Damage));
		ZeroMemory(&Expansion_UseMana, sizeof(Expansion_UseMana));
	}
};

struct Venom_Spear: skillPacket
{
	short VenomSpear_Damage[10][2];
	short VenomSpear_Num[10];
	short VenomSpear_Time[10];
	short VenomSpear_Range[10];
	short VenomSpear_UseMana[10];

	Venom_Spear()
	{
		ZeroMemory(&VenomSpear_Damage, sizeof(VenomSpear_Damage));
		ZeroMemory(&VenomSpear_Num, sizeof(VenomSpear_Num));
		ZeroMemory(&VenomSpear_Time, sizeof(VenomSpear_Time));
		ZeroMemory(&VenomSpear_Range, sizeof(VenomSpear_Range));
		ZeroMemory(&VenomSpear_UseMana, sizeof(VenomSpear_UseMana));
	}
};

struct Vanish: skillPacket
{
	short Vanish_Damage[10];
	short Vanish_Range[10];
	short Vanish_Time[10];
	short Vanish_UseMana[10];

	Vanish()
	{
		ZeroMemory(&Vanish_Damage, sizeof(Vanish_Damage));
		ZeroMemory(&Vanish_Range, sizeof(Vanish_Range));
		ZeroMemory(&Vanish_Time, sizeof(Vanish_Time));
		ZeroMemory(&Vanish_UseMana, sizeof(Vanish_UseMana));
	}
};

struct Critical_Mastery: skillPacket
{
	short Critical_Mastery_Critical[10];

	Critical_Mastery()
	{
		ZeroMemory(&Critical_Mastery_Critical, sizeof(Critical_Mastery_Critical));
	}
};

struct Chain_Lance: skillPacket
{
	short Chain_Lance_Damage[10];
	short Chain_Lance_UseMana[10];

	Chain_Lance()
	{
		ZeroMemory(&Chain_Lance_Damage, sizeof(Chain_Lance_Damage));
		ZeroMemory(&Chain_Lance_UseMana, sizeof(Chain_Lance_UseMana));
	}
};

struct Assassin_Eye: skillPacket
{
	short Assassin_Eye_AddCritical[10];
	short Assassin_Eye_M_SubCritical[10];
	short Assassin_Eye_Time[10];
	short Assassin_Eye_UseMana[10];

	Assassin_Eye()
	{
		ZeroMemory(&Assassin_Eye_AddCritical, sizeof(Assassin_Eye_AddCritical));
		ZeroMemory(&Assassin_Eye_M_SubCritical, sizeof(Assassin_Eye_M_SubCritical));
		ZeroMemory(&Assassin_Eye_Time, sizeof(Assassin_Eye_Time));
		ZeroMemory(&Assassin_Eye_UseMana, sizeof(Assassin_Eye_UseMana));
	}
};

struct Charging_Strike: skillPacket
{
	short Charging_Strike_DamagePercent[10];
	short Charging_Strike_CharingDamagePercent[10];
	short Charging_Strike_Time[10];
	short Charging_Strike_UseMana[10];

	Charging_Strike()
	{
		ZeroMemory(&Charging_Strike_DamagePercent, sizeof(Charging_Strike_DamagePercent));
		ZeroMemory(&Charging_Strike_CharingDamagePercent, sizeof(Charging_Strike_CharingDamagePercent));
		ZeroMemory(&Charging_Strike_Time, sizeof(Charging_Strike_Time));
		ZeroMemory(&Charging_Strike_UseMana, sizeof(Charging_Strike_UseMana));
	}
};

struct Vague: skillPacket
{
	short Vague_EvasionPercent[10];
	short Vague_Time[10];
	short Vague_UseMana[10];

	Vague()
	{
		ZeroMemory(&Vague_EvasionPercent, sizeof(Vague_EvasionPercent));
		ZeroMemory(&Vague_Time, sizeof(Vague_Time));
		ZeroMemory(&Vague_UseMana, sizeof(Vague_UseMana));
	}
};

struct Shadow_Master: skillPacket
{
	short Shadow_Master_DamagePercent[10];
	short Shadow_Master_ShadowNum[10];
	short Shadow_Master_UseMana[10];

	Shadow_Master()
	{
		ZeroMemory(&Shadow_Master_DamagePercent, sizeof(Shadow_Master_DamagePercent));
		ZeroMemory(&Shadow_Master_ShadowNum, sizeof(Shadow_Master_ShadowNum));
		ZeroMemory(&Shadow_Master_UseMana, sizeof(Shadow_Master_UseMana));
	}
};

#endif