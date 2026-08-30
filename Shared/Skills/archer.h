#ifndef	ARCHER_H
#define ARCHER_H

#include "base.h"

struct Scout_Hawk: skillPacket
{
	short Scout_Hawk_Attack_Rate[10];
	short Scout_Hawk_UseTime[10];
	short Scout_Hawk_UseMana[10];

	Scout_Hawk()
	{
		ZeroMemory(&Scout_Hawk_Attack_Rate, sizeof(Scout_Hawk_Attack_Rate));
		ZeroMemory(&Scout_Hawk_UseTime, sizeof(Scout_Hawk_UseTime));
		ZeroMemory(&Scout_Hawk_UseMana, sizeof(Scout_Hawk_UseMana));
	}
};

struct S_Mastery: skillPacket
{
	short S_Mastery_DamagePercent[10];

	S_Mastery()
	{
		ZeroMemory(&S_Mastery_DamagePercent, sizeof(S_Mastery_DamagePercent));
	}
};

struct Wind_Arrow: skillPacket
{
	short Wind_Arrow_Speed[10];
	short Wind_Arrow_Damage[10];
	short Wind_Arrow_UseMana[10];

	Wind_Arrow()
	{
		ZeroMemory(&Wind_Arrow_Speed, sizeof(Wind_Arrow_Speed));
		ZeroMemory(&Wind_Arrow_Damage, sizeof(Wind_Arrow_Damage));
		ZeroMemory(&Wind_Arrow_UseMana, sizeof(Wind_Arrow_UseMana));
	}
};

struct Perfect_Aim: skillPacket
{
	short Perfect_Aim_Attack_Rate[10];
	short Perfect_Aim_Damage[10];
	short Perfect_Aim_Damage_LV[10];
	short Perfect_Aim_UseMana[10];

	Perfect_Aim()
	{
		ZeroMemory(&Perfect_Aim_Attack_Rate, sizeof(Perfect_Aim_Attack_Rate));
		ZeroMemory(&Perfect_Aim_Damage, sizeof(Perfect_Aim_Damage));
		ZeroMemory(&Perfect_Aim_Damage_LV, sizeof(Perfect_Aim_Damage_LV));
		ZeroMemory(&Perfect_Aim_UseMana, sizeof(Perfect_Aim_UseMana));
	}
};

struct D_Eye: skillPacket
{
	short D_Eye_Attack_Rate[10];

	D_Eye()
	{
		ZeroMemory(&D_Eye_Attack_Rate, sizeof(D_Eye_Attack_Rate));
	}
};

struct Falcon: skillPacket
{
	short Falcon_Damage[10][2];
	short Falcon_Time[10];
	short Falcon_UseMana[10];

	Falcon()
	{
		ZeroMemory(&Falcon_Damage, sizeof(Falcon_Damage));
		ZeroMemory(&Falcon_Time, sizeof(Falcon_Time));
		ZeroMemory(&Falcon_UseMana, sizeof(Falcon_UseMana));
	}
};

struct A_Rage: skillPacket
{
	short A_Rage_ArrowNum[10];
	short A_Rage_Damage[10];
	short A_Rage_Area[10];
	short A_Rage_UseMana[10];

	A_Rage()
	{
		ZeroMemory(&A_Rage_ArrowNum, sizeof(A_Rage_ArrowNum));
		ZeroMemory(&A_Rage_Damage, sizeof(A_Rage_Damage));
		ZeroMemory(&A_Rage_Area, sizeof(A_Rage_Area));
		ZeroMemory(&A_Rage_UseMana, sizeof(A_Rage_UseMana));
	}
};

struct Avalanche: skillPacket
{
	short Avalanche_Speed[10];
	short Avalanche_Damage[10];
	short Avalanche_ArrowNum[10];
	short Avalanche_UseMana[10];

	Avalanche()
	{
		ZeroMemory(&Avalanche_Speed, sizeof(Avalanche_Speed));
		ZeroMemory(&Avalanche_Damage, sizeof(Avalanche_Damage));
		ZeroMemory(&Avalanche_ArrowNum, sizeof(Avalanche_ArrowNum));
		ZeroMemory(&Avalanche_UseMana, sizeof(Avalanche_UseMana));
	}
};

struct Elemental_Shot: skillPacket
{
	short Elemental_Shot_Fire[10][2];
	short Elemental_Shot_Lightning[10][2];
	short Elemental_Shot_UseMana[10];

	Elemental_Shot()
	{
		ZeroMemory(&Elemental_Shot_Fire, sizeof(Elemental_Shot_Fire));
		ZeroMemory(&Elemental_Shot_Lightning, sizeof(Elemental_Shot_Lightning));
		ZeroMemory(&Elemental_Shot_UseMana, sizeof(Elemental_Shot_UseMana));
	}
};

struct Golden_Falcon: skillPacket
{
	short Golden_Falcon_Damage[10];
	float Golden_Falcon_LifeRegen[10];
	short Golden_Falcon_Time[10];
	short Golden_Falcon_UseMana[10];

	Golden_Falcon()
	{
		ZeroMemory(&Golden_Falcon_Damage, sizeof(Golden_Falcon_Damage));
		ZeroMemory(&Golden_Falcon_LifeRegen, sizeof(Golden_Falcon_LifeRegen));
		ZeroMemory(&Golden_Falcon_Time, sizeof(Golden_Falcon_Time));
		ZeroMemory(&Golden_Falcon_UseMana, sizeof(Golden_Falcon_UseMana));
	}
};

struct Bomb_Shot: skillPacket
{
	short BombShot_Damage[10];
	short BombShot_DemonDamage[10];
	short BombShot_AddDamage[10][2];
	short BombShot_Area[10];
	short BombShot_UseMana[10];

	Bomb_Shot()
	{
		ZeroMemory(&BombShot_Damage, sizeof(BombShot_Damage));
		ZeroMemory(&BombShot_DemonDamage, sizeof(BombShot_DemonDamage));
		ZeroMemory(&BombShot_AddDamage, sizeof(BombShot_AddDamage));
		ZeroMemory(&BombShot_Area, sizeof(BombShot_Area));
		ZeroMemory(&BombShot_UseMana, sizeof(BombShot_UseMana));
	}
};

struct Perforation: skillPacket
{
	short Perforation_Damage[10];
	short Perforation_Critical[10];
	short Perforation_UseMana[10];
	short Perforation_Attack_Range[10];

	Perforation()
	{
		ZeroMemory(&Perforation_Damage, sizeof(Perforation_Damage));
		ZeroMemory(&Perforation_Critical, sizeof(Perforation_Critical));
		ZeroMemory(&Perforation_UseMana, sizeof(Perforation_UseMana));
		ZeroMemory(&Perforation_Attack_Range, sizeof(Perforation_Attack_Range));
	}
};

struct Recall_Wolverin: skillPacket
{
	short Recall_Wolverin_Damage[10][2];
	short Recall_Wolverin_Hit[10];
	short Recall_Wolverin_Defense[10];
	short Recall_Wolverin_Life[10];
	short Recall_Wolverin_UseMana[10];

	Recall_Wolverin()
	{
		ZeroMemory(&Recall_Wolverin_Damage, sizeof(Recall_Wolverin_Damage));
		ZeroMemory(&Recall_Wolverin_Hit, sizeof(Recall_Wolverin_Hit));
		ZeroMemory(&Recall_Wolverin_Defense, sizeof(Recall_Wolverin_Defense));
		ZeroMemory(&Recall_Wolverin_Life, sizeof(Recall_Wolverin_Life));
		ZeroMemory(&Recall_Wolverin_UseMana, sizeof(Recall_Wolverin_UseMana));
	}
};

struct Evasion_Mastery: skillPacket
{
	short Evasion_Mastery_AddPercent[10];
	float Evasion_Mastery_AddSpeed[10];
	short Evasion_Mastery_AddReduce[10];
	short Evasion_Mastery_Time[10];
	short Evasion_Mastery_UseMana[10];

	Evasion_Mastery()
	{
		ZeroMemory(&Evasion_Mastery_AddPercent, sizeof(Evasion_Mastery_AddPercent));
		ZeroMemory(&Evasion_Mastery_AddSpeed, sizeof(Evasion_Mastery_AddSpeed));
		ZeroMemory(&Evasion_Mastery_AddReduce, sizeof(Evasion_Mastery_AddReduce));
		ZeroMemory(&Evasion_Mastery_Time, sizeof(Evasion_Mastery_Time));
		ZeroMemory(&Evasion_Mastery_UseMana, sizeof(Evasion_Mastery_UseMana));
	}
};

struct Phoenix_Shot: skillPacket
{
	short Phoenix_Shot_DamagePercent[10];
	short Phoenix_Shot_Time[10];
	short Phoenix_Shot_UseMana[10];

	Phoenix_Shot()
	{
		ZeroMemory(&Phoenix_Shot_DamagePercent, sizeof(Phoenix_Shot_DamagePercent));
		ZeroMemory(&Phoenix_Shot_Time, sizeof(Phoenix_Shot_Time));
		ZeroMemory(&Phoenix_Shot_UseMana, sizeof(Phoenix_Shot_UseMana));
	}
};

struct Force_Of_Nature: skillPacket
{
	short Force_Of_Nature_AddDamage[10];
	short Force_Of_Nature_AddHit[10];
	short Force_Of_Nature_FalconAddDamage[10];
	short Force_Of_Nature_Time[10];
	short Force_Of_Nature_UseMana[10];

	Force_Of_Nature()
	{
		ZeroMemory(&Force_Of_Nature_AddDamage, sizeof(Force_Of_Nature_AddDamage));
		ZeroMemory(&Force_Of_Nature_AddHit, sizeof(Force_Of_Nature_AddHit));
		ZeroMemory(&Force_Of_Nature_FalconAddDamage, sizeof(Force_Of_Nature_FalconAddDamage));
		ZeroMemory(&Force_Of_Nature_Time, sizeof(Force_Of_Nature_Time));
		ZeroMemory(&Force_Of_Nature_UseMana, sizeof(Force_Of_Nature_UseMana));
	}
};

#endif