#include "..\\sinbaram\\sinLinkHeader.h"
#include "File.h"


BOOL showDetails = FALSE;


bool CustomHud::MouseAction(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x && pRealCursorPos.x < x + w && pRealCursorPos.y > y && pRealCursorPos.y < y + h)
		return TRUE;

	return FALSE;
}

int CustomHud::Button() 
{
	if (!Caravana::GetInstance()->OpenFlag || !GotClick)
		return FALSE;


	if (MouseAction(235 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 100 + 177 + sinInterHeight2, 28, 28))
	{
		GotClick = false;
		return 1;
	}

	if (MouseAction(270 - (256 + 128 - sinMoveKindInter[SIN_CARAVANA]), 90 + 177 + sinInterHeight2, 28, 28))
	{
		GotClick = false;
		return 2;
	}

	GotClick = false;
	return FALSE;
}

cBMPFONT::cBMPFONT(const std::string& file, LPDIRECT3DTEXTURE9 dds)
{
	ddsFont = dds;
	ReadFontData(file);
}

void cBMPFONT::ReadFontData(const std::string& file)
{
	File data(file);

	sFont.iLineHeight = data.readInt("Font", "LineHeight");
	sFont.iLettersCount = data.readInt("Font", "Count");
	sFont.iBaseY = data.readInt("Font", "BaseY");

	for (int i = 0; i < sFont.iLettersCount; i++)
	{
		LetterData sLetterWrapper;
		sLetterWrapper.iID = data.readInt(FormatString("Letter%d", i), "ID");
		sLetterWrapper.sBox.iX = data.readInt(FormatString("Letter%d", i), "X");
		sLetterWrapper.sBox.iY = data.readInt(FormatString("Letter%d", i), "Y");
		sLetterWrapper.sBox.iWidth = data.readInt(FormatString("Letter%d", i), "WidthSource");
		sLetterWrapper.sBox.iHeight = data.readInt(FormatString("Letter%d", i), "HeightSource");
		sLetterWrapper.iPaddingY = data.readInt(FormatString("Letter%d", i), "PaddingY");
		sLetterWrapper.iPaddingX = data.readInt(FormatString("Letter%d", i), "PaddingX");

		sFont.vLetters.push_back(sLetterWrapper);
	}
}

LetterData cBMPFONT::GetLetterData(int iID)
{
	for (auto& v : sFont.vLetters)
	{
		if (v.iID == iID)
			return v;
	}

	return sFont.vLetters[0];
}

int cBMPFONT::GetTextWidth(char* szText)
{
	int iWidthText = 0;
	int len = lstrlen(szText);

	if (len > 0)
	{
		iWidthText = 0;
		for (UINT i = 0; i < len; i++)
		{
			int iLetterID = szText[i];
			LetterData* p = &GetLetterData(iLetterID);

			if (p->sBox.iWidth)
			{
				int iDifference = abs(p->iPaddingX - p->sBox.iWidth);
				iWidthText += (int)((float)(p->sBox.iWidth - iDifference) * 1.0f);
			}
			else
				iWidthText += p->iPaddingX;
		}
	}

	return iWidthText;
}

void cBMPFONT::DrawBmpFont(int x, int y, char* szText, int len)
{
	for (int i = 0; i < len; i++)
	{
		int lID = szText[i];
		LetterData* p = &GetLetterData(lID);

		int winX = p->sBox.iX;
		int winY = p->sBox.iY;
		int width = p->sBox.iWidth;
		int height = p->sBox.iHeight;

		dsGetDrawOffset(&x, &y);

		RECT srcRect = { winX, winY, winX + width, winY + height };
		RECT destRect = { x, y, x + width, y + height };

		DrawTexture2D(ddsFont, destRect, srcRect, true);

		if (p->sBox.iWidth)
		{
			int iDifference = abs(p->iPaddingX - p->sBox.iWidth);
			x += (int)((float)(p->sBox.iWidth - iDifference) * 1.0f);
		}
		else
			x += p->iPaddingX;
	}
}

cBMPFONT* BmpFont[BMPFonts::MAX_FONT];

void InitBmpFont()
{
	BmpFont[BMPFonts::ORANGE_FONT] = new cBMPFONT("game\\images\\Damage\\font.dat", LoadDibSurfaceOffscreen("game\\images\\Damage\\font1.png"));
	BmpFont[BMPFonts::RED_FONT] = new cBMPFONT("game\\images\\Damage\\font.dat", LoadDibSurfaceOffscreen("game\\images\\Damage\\font2.png"));
	BmpFont[BMPFonts::BLUE_FONT] = new cBMPFONT("game\\images\\Damage\\font3.dat", LoadDibSurfaceOffscreen("game\\images\\Damage\\font3.png"));
}