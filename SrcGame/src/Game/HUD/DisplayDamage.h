#pragma once

#include <Windows.h>
#include <vector>
#include <Engine/CFont.h>

enum Type {
	Defense,
	Block,
	Evade,
	HP,
	MP,
	STM,
	Damage,
	CriticalDamage,
};

class cSHOW_DMG
{
public:
	struct sDMG
	{
		int Time;
		DWORD objSerial;
		Type Type;

		char Text[32];
		int TextWidth;
	};

	cSHOW_DMG();
	static cSHOW_DMG* getInstance();
	void AddDmg(int Dmg, DWORD Serial, Type Type);
	void AddDef(DWORD Serial, Type Type, int value = 0);
	void DrawDmg(DWORD TargetSerial, int x, int y);

private:
	std::vector<sDMG*> Damages;
	CFont* pFont;
};
