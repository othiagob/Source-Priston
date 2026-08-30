#ifndef	PRIESTESS_H
#define PRIESTESS_H

#include "base.h"

struct Healing: skillPacket
{
	short Healing_UseMana[10];
	short Healing_Heal[10][2];

	Healing()
	{
		ZeroMemory(&Healing_UseMana, sizeof(Healing_UseMana));
		ZeroMemory(&Healing_Heal, sizeof(Healing_Heal));
	}
};

struct HolyBolt: skillPacket
{
	short HolyBolt_UseMana[10];
	short HolyBolt_Damage[10];

	HolyBolt()
	{
		ZeroMemory(&HolyBolt_UseMana, sizeof(HolyBolt_UseMana));
		ZeroMemory(&HolyBolt_Damage, sizeof(HolyBolt_Damage));
	}
};


struct M_Spark: skillPacket
{
	short M_Spark_Damage[10];
	short M_Spark_Num[10];

	M_Spark()
	{
		ZeroMemory(&M_Spark_Damage, sizeof(M_Spark_Damage));
		ZeroMemory(&M_Spark_Num, sizeof(M_Spark_Num));
	}
};

struct HolyMind: skillPacket
{
	short HolyMind_UseMana[10];
	short HolyMind_DecDamage[10];
	short HolyMind_Time[10];

	HolyMind()
	{
		ZeroMemory(&HolyMind_UseMana, sizeof(HolyMind_UseMana));
		ZeroMemory(&HolyMind_DecDamage, sizeof(HolyMind_DecDamage));
		ZeroMemory(&HolyMind_Time, sizeof(HolyMind_Time));
	}
};

struct Meditation: skillPacket
{
	float Meditation_Regen[10];

	Meditation()
	{
		ZeroMemory(&Meditation_Regen, sizeof(Meditation_Regen));
	}
};

struct Divine_Lightning: skillPacket
{
	short Divine_Lightning_Damage[10];
	short Divine_Lightning_Num[10];
	short Divine_Lightning_Mana[10];

	Divine_Lightning()
	{
		ZeroMemory(&Divine_Lightning_Damage, sizeof(Divine_Lightning_Damage));
		ZeroMemory(&Divine_Lightning_Num, sizeof(Divine_Lightning_Num));
		ZeroMemory(&Divine_Lightning_Mana, sizeof(Divine_Lightning_Mana));
	}
};

struct Holy_Reflection: skillPacket
{
	short Holy_Reflection_Return_Damage[10];
	short Holy_Reflection_Time[10];
	short Holy_Reflection_Mana[10];

	Holy_Reflection()
	{
		ZeroMemory(&Holy_Reflection_Return_Damage, sizeof(Holy_Reflection_Return_Damage));
		ZeroMemory(&Holy_Reflection_Time, sizeof(Holy_Reflection_Time));
		ZeroMemory(&Holy_Reflection_Mana, sizeof(Holy_Reflection_Mana));
	}
};

struct GrandHealing: skillPacket
{
	short Grand_Healing[10][2];
	short Grand_Healing_Range[10];
	short Grand_Healing_Mana[10];

	GrandHealing()
	{
		ZeroMemory(&Grand_Healing, sizeof(Grand_Healing));
		ZeroMemory(&Grand_Healing_Range, sizeof(Grand_Healing_Range));
		ZeroMemory(&Grand_Healing_Mana, sizeof(Grand_Healing_Mana));
	}
};

struct Vigor_Ball: skillPacket
{
	short Vigor_Ball_Damage[10];
	short Vigor_Ball_UseMana[10];

	Vigor_Ball()
	{
		ZeroMemory(&Vigor_Ball_Damage, sizeof(Vigor_Ball_Damage));
		ZeroMemory(&Vigor_Ball_UseMana, sizeof(Vigor_Ball_UseMana));
	}
};

struct Resurrection: skillPacket
{
	short Resurrection_Percent[10];
	short Resurrection_UseMana[10];

	Resurrection()
	{
		ZeroMemory(&Resurrection_Percent, sizeof(Resurrection_Percent));
		ZeroMemory(&Resurrection_UseMana, sizeof(Resurrection_UseMana));
	}
};

struct Extinction: skillPacket
{
	short Extinction_Percent[10];
	short Extinction_Amount[10];
	short Extinction_UseMana[10];

	Extinction()
	{
		ZeroMemory(&Extinction_Percent, sizeof(Extinction_Percent));
		ZeroMemory(&Extinction_Amount, sizeof(Extinction_Amount));
		ZeroMemory(&Extinction_UseMana, sizeof(Extinction_UseMana));
	}
};

struct Virtual_Life: skillPacket
{
	short Virtual_Life_Percent[10];
	short Virtual_Life_Time[10];
	short Virtual_Life_UseMana[10];

	Virtual_Life()
	{
		ZeroMemory(&Virtual_Life_Percent, sizeof(Virtual_Life_Percent));
		ZeroMemory(&Virtual_Life_Time, sizeof(Virtual_Life_Time));
		ZeroMemory(&Virtual_Life_UseMana, sizeof(Virtual_Life_UseMana));
	}
};

struct Glacial_Spike: skillPacket
{
	short Glacial_Spike_Damage[10];
	short Glacial_Spike_Area[10];
	short Glacial_Spike_UseMana[10];

	Glacial_Spike()
	{
		ZeroMemory(&Glacial_Spike_Damage, sizeof(Glacial_Spike_Damage));
		ZeroMemory(&Glacial_Spike_Area, sizeof(Glacial_Spike_Area));
		ZeroMemory(&Glacial_Spike_UseMana, sizeof(Glacial_Spike_UseMana));
	}
};

struct Regeneration_Field: skillPacket
{
	float Regeneration_Field_LifeRegen[10];
	float Regeneration_Field_ManaRegen[10];
	short Regeneration_Field_Time[10];
	short Regeneration_Field_Area[10];
	short Regeneration_Field_UseMana[10];

	Regeneration_Field()
	{
		ZeroMemory(&Regeneration_Field_LifeRegen, sizeof(Regeneration_Field_LifeRegen));
		ZeroMemory(&Regeneration_Field_ManaRegen, sizeof(Regeneration_Field_ManaRegen));
		ZeroMemory(&Regeneration_Field_Time, sizeof(Regeneration_Field_Time));
		ZeroMemory(&Regeneration_Field_Area, sizeof(Regeneration_Field_Area));
		ZeroMemory(&Regeneration_Field_UseMana, sizeof(Regeneration_Field_UseMana));
	}
};

struct Chain_Lightning: skillPacket
{
	short Chain_Lightning_Damage[10];
	/*short Chain_Lightning_Num[10];*/
	short Chain_Lightning_Range[10];
	short Chain_Lightning_UseMana[10];

	Chain_Lightning()
	{
		ZeroMemory(&Chain_Lightning_Damage, sizeof(Chain_Lightning_Damage));
		/*ZeroMemory(&Chain_Lightning_Num, sizeof(Chain_Lightning_Num));*/
		ZeroMemory(&Chain_Lightning_Range, sizeof(Chain_Lightning_Range));
		ZeroMemory(&Chain_Lightning_UseMana, sizeof(Chain_Lightning_UseMana));
	}
};

struct Summon_Muspell: skillPacket
{
	short Summon_Muspell_Damage[10][2];
	short Summon_Muspell_Attack_Delay[10];
	short Summon_Muspell_UndeadAbsorbPercent[10];
	short Summon_Muspell_BlockPercent[10];
	short Summon_Muspell_Time[10];
	short Summon_Muspell_UseMana[10];

	Summon_Muspell()
	{
		ZeroMemory(&Summon_Muspell_Damage, sizeof(Summon_Muspell_Damage));
		ZeroMemory(&Summon_Muspell_Attack_Delay, sizeof(Summon_Muspell_Attack_Delay));
		ZeroMemory(&Summon_Muspell_UndeadAbsorbPercent, sizeof(Summon_Muspell_UndeadAbsorbPercent));
		ZeroMemory(&Summon_Muspell_BlockPercent, sizeof(Summon_Muspell_BlockPercent));
		ZeroMemory(&Summon_Muspell_Time, sizeof(Summon_Muspell_Time));
		ZeroMemory(&Summon_Muspell_UseMana, sizeof(Summon_Muspell_UseMana));
	}
};

#endif