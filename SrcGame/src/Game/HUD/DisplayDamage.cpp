#include "..\\sinbaram\\sinLinkHeader.h"
#include "DisplayDamage.h"
#include "CustomHud.h"
#include <Engine/CFontHandler.h>
#include <Settings.h>

cSHOW_DMG showDmg;

BOOL showDamage = TRUE;

#define OUTLINE 1

cSHOW_DMG::cSHOW_DMG()
{

}

cSHOW_DMG* cSHOW_DMG::getInstance()
{
	return &showDmg;
}

void cSHOW_DMG::AddDmg(int Dmg, DWORD Serial, Type Type)
{
	sDMG* newDmg = new sDMG;

	if (newDmg)
	{
		if (!pFont)
			pFont = CFONTHANDLER->GetFont("Century Schoolbook", 25, true);

		newDmg->Time = GetCurrentTime() + 1000;
		newDmg->objSerial = Serial;
		newDmg->Type = Type;

		wsprintf(newDmg->Text, "%d", Dmg);
		SIZE sz = {};
		pFont->GetTextSize(newDmg->Text, &sz);
		newDmg->TextWidth = sz.cx;

		Damages.push_back(newDmg);
	}
}

void cSHOW_DMG::AddDef(DWORD Serial, Type Type, int value)
{
	sDMG* newDmg = new sDMG;

	if (newDmg)
	{
		if (!pFont)
			pFont = CFONTHANDLER->GetFont("Century Schoolbook", 25, true);

		newDmg->Time = GetCurrentTime() + 1000;
		newDmg->objSerial = Serial;
		newDmg->Type = Type;

		switch (newDmg->Type)
		{
		case Type::Defense:
			lstrcpy(newDmg->Text, "Defense!");
			break;
		case Type::Block:
			lstrcpy(newDmg->Text, "Block!");
			break;
		case Type::Evade:
			lstrcpy(newDmg->Text, "Evade!");
			break;
		case Type::HP:
		case Type::MP:
		case Type::STM:
			wsprintf(newDmg->Text, "+%d", value);
			break;
		case Type::Damage:
		case Type::CriticalDamage:
			wsprintf(newDmg->Text, "%d", value);
			break;
		}

		SIZE sz = {};
		pFont->GetTextSize(newDmg->Text, &sz);
		newDmg->TextWidth = sz.cx;

		Damages.push_back(newDmg);
	}
}

void cSHOW_DMG::DrawDmg(DWORD TargetSerial, int x, int y)
{
	char szBuff[255] = { 0 };

	if (smConfig.showDamage) return;
	{

#ifdef _DEBUG
		for (auto& it : Damages)
		{
			sDMG* DmgInfo = it;
#else
		for (std::vector<sDMG*>::iterator it = Damages.begin(); it < Damages.end(); it++)
		{
			sDMG* DmgInfo = (*it);
#endif
			if (DmgInfo && DmgInfo->objSerial == TargetSerial)
			{
				int Time = DmgInfo->Time - GetCurrentTime();
				if (Time > 0)
				{
					int Alpha = int(255 * (Time / 1000.f));
					// Defesa
					DWORD dwColor = D3DCOLOR_ARGB(Alpha, 50, 185, 240);
					switch (DmgInfo->Type)
					{
					case Type::HP:
						dwColor = D3DCOLOR_ARGB(Alpha, 185, 50, 50);
						break;
					case Type::MP:
						dwColor = D3DCOLOR_ARGB(Alpha, 75, 100, 210);
						break;
					case Type::STM:
						dwColor = D3DCOLOR_ARGB(Alpha, 50, 185, 70);
						break;

						// Block
					case Type::Block:
						dwColor = D3DCOLOR_ARGB(Alpha, 255, 220, 0);
						break;
						// Evade // Dano
					case Type::Evade:
					case Type::Damage:
					case Type::CriticalDamage:
						dwColor = D3DCOLOR_ARGB(Alpha, 255, 160, 57);
						break;
					}

					RECT rc = { (x - DmgInfo->TextWidth / 2) - OUTLINE, (y - 83 + (Time / 16)), WinSizeX, WinSizeY };
					pFont->SetFontColor(D3DCOLOR_ARGB(Alpha, 1, 1, 1));
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2), (y - 83 + (Time / 16)) - OUTLINE, WinSizeX, WinSizeY };
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2) + OUTLINE, (y - 83 + (Time / 16)), WinSizeX, WinSizeY };
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2), (y - 83 + (Time / 16)) + OUTLINE, WinSizeX, WinSizeY };
					pFont->DrawText(DmgInfo->Text, &rc, 0);

					rc = { (x - DmgInfo->TextWidth / 2), (y - 83 + (Time / 16)), WinSizeX, WinSizeY };
					pFont->SetFontColor(dwColor);
					pFont->DrawText(DmgInfo->Text, &rc, 0);


				}
				else {
					delete DmgInfo;
#ifndef _DEBUG
					Damages.erase(it);
#endif

				}
			}
		}
	}
}