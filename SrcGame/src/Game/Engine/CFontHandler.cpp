#include "..\\sinbaram\\sinLinkHeader.h"
#include "CFontHandler.h"

CFontHandler cFontHandler;

CFontHandler* CFontHandler::getInstance()
{
	return &cFontHandler;
}

CFontHandler::CFontHandler()
{
	AddFontResourceEx("game\\font\\SCHLBKB.ttf", NULL, NULL);
}

CFontHandler::~CFontHandler()
{
	RemoveFontResourceEx("game\\font\\SCHLBKB.ttf", NULL, NULL);
}

CFont* CFontHandler::GetFont(char* FontName, int FontSize, bool bBold)
{
	for (auto pFont : vFonts)
	{
		if (!lstrcmpi(pFont->GetFontName(), FontName) && pFont->GetFontSize() == FontSize && pFont->GetBold() == bBold)
			return pFont;
	}
	return CreateFontInfo(FontName, FontSize, bBold);
}

CFont* CFontHandler::GetFontDefault(bool bBold)
{
	return GetFont(bBold ? "Arial Bold" : "Arial", 15, bBold);
}

CFont* CFontHandler::CreateFontInfo(char* FontName, int FontSize, bool bBold)
{
	CFont* pFont = new CFont(FontName, FontSize, bBold);
	if (pFont)
	{
		if (pFont->GetFont())
		{
			vFonts.push_back(pFont);
			return pFont;
		}
		else
		{
			delete pFont;
			return nullptr;
		}
	}

	return nullptr;
}

void CFontHandler::Release()
{
	for (auto pFont : vFonts)
	{
		pFont->Release();
	}

	vFonts.clear();
}

void CFontHandler::Reset()
{
	for (auto pFont : vFonts)
	{
		pFont->Reset();
	}
}

void CFontHandler::Lost()
{
	for (auto pFont : vFonts)
	{
		pFont->Lost();
	}
}
