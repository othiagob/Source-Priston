#include <memory>
#include <algorithm>
#include "DXFont.h"
#include "DXGraphicEngine.h"
#include "Utils/strings.h"
#include "FontImage.h"
#include "drawsub.h"
#include "Def.h"
#include "Engine/Emojize.h"

using namespace DX;

typedef std::shared_ptr<stFontStructure> FontSt_ptr;
bool bCreatedFonts = true;

void DXFont::Init()
{
	AddFontResourceExA("game\\fonts\\Frutiger.ttf", FR_PRIVATE, NULL);
	AddFontResourceExA("game\\fonts\\NotoSans-Regular.ttf", FR_PRIVATE, NULL);
	AddFontResourceExA("game\\fonts\\Arvo-Regular.ttf", FR_PRIVATE, NULL);
	AddFontResourceExA("game\\fonts\\Arvo-Bold.ttf", FR_PRIVATE, NULL);
}

int DXFont::CreateFont(const char* FontName, int iHeight, int iWidth, BOOL Italic, BOOL Bold, int MemValue)
{
	for (UINT i = 0; i < vFontResource.size(); i++)
	{
		if (strcmp(FontName, vFontResource[i].fname) == 0)
		{
			if (vFontResource[i].bold == Bold && vFontResource[i].italic == Italic && vFontResource[i].Width == iWidth && vFontResource[i].Height == iHeight)
				return i;
		}
	}

	stFontStructure pFont;
	memset(&pFont, 0, sizeof(stFontStructure));

	UINT uWeight = FW_NORMAL;

	if (Bold == TRUE)
		uWeight = FW_BOLD;

	D3DXFONT_DESCA s;
	s.Height = iHeight;
	s.Width = iWidth;
	s.Weight = uWeight;
	s.MipLevels = D3DX_DEFAULT;
	s.Italic = Italic;
	s.CharSet = DEFAULT_CHARSET;
	s.OutputPrecision = OUT_DEFAULT_PRECIS;
	s.Quality = CLEARTYPE_QUALITY;
	s.PitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	strcpy_s(s.FaceName, FontName);

	D3DXCreateFontIndirectA(GRAPHICENGINE->GetDevice(), &s, &pFont.hFont);

	pFont.Width = iWidth;
	pFont.Height = iHeight;
	pFont.italic = Italic;
	pFont.bold = Bold;
	strcpy_s(pFont.fname, sizeof(pFont.fname), FontName);
	pFont.uWidthWhitespace = 0;

	if (vFontResource.size() == 0)
	{
		pFont.hFont->PreloadCharacters(32, 127);
		pFont.hFont->PreloadCharacters(32, 127);
	}


	int iWidthUnderline = 0;
	if (pFont.hFont)
	{
		RECT r = { 0, 0, 0, 0 };
		pFont.hFont->DrawTextA(nullptr, "_", 1, &r, DT_CALCRECT, -1);
		iWidthUnderline = r.right - r.left;

		r = { 0, 0, 0, 0 };
		pFont.hFont->DrawTextA(nullptr, " _", 2, &r, DT_CALCRECT, -1);
		pFont.uWidthWhitespace = (r.right - r.left) - iWidthUnderline;
	}


	if (MemValue < 0)
		vFontResource.push_back(pFont);
	else
		vFontResource[MemValue] = pFont;

	return vFontResource.size() - 1;
}

int DXFont::GetWidthText(int FontObject, const char* Text, INT Size)
{
	if (static_cast<int>(vFontResource.size()) <= FontObject)
		return 0;

	const stFontStructure pFont = vFontResource[FontObject];
	if (!pFont.hFont)
		return 0;

	const std::string strText(Text);
	int iWidthText = 0;

	for (int i = 0; i < Size; ++i)
	{
		if (strText[i] == 0x20)
			iWidthText += pFont.uWidthWhitespace;
		else
		{
			RECT rcRect = { 0, 0, 0, 0 };

			pFont.hFont->DrawTextA(nullptr, &strText[i], 1, &rcRect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
			iWidthText += rcRect.right - rcRect.left;
		}
	}

	return iWidthText;
}

int DXFont::GetHeightText(int FontObject, const char* Text, RECT rRect)
{
	if (static_cast<int>(vFontResource.size()) <= FontObject)
		return 0;

	const stFontStructure pFont = vFontResource[FontObject];

	if (!pFont.hFont)
		return 0;

	pFont.hFont->DrawTextA(nullptr, Text, lstrlenA(Text), &rRect, DT_WORDBREAK | DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return rRect.bottom - rRect.top;
}

int DXFont::GetHeightText(int FontObject, UINT uFormat, const char* Text, RECT rRect)
{
	if (static_cast<int>(vFontResource.size()) <= FontObject)
		return 0;

	const stFontStructure pFont = vFontResource[FontObject];

	if (!pFont.hFont)
		return 0;

	pFont.hFont->DrawTextA(nullptr, Text, lstrlenA(Text), &rRect, uFormat | DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));
	return rRect.bottom - rRect.top;
}

int DXFont::GetHeightText(int iFontID)
{
	if (static_cast<int>(vFontResource.size()) <= iFontID)
		return 0;


	const stFontStructure pFont = vFontResource[iFontID];

	RECT rcRect = { 0, 0, 0, 0 };


	if (!pFont.hFont)
		return 0;

	return pFont.Height;
}

void DXFont::Draw(int FontObject, int x, int y, const std::string& pText, UINT uFormat, D3DCOLOR Color)
{
	RECT rect{ x, y, RESOLUTION_WIDTH + x, RESOLUTION_HEIGHT + y };

	Draw(FontObject, rect, pText.c_str(), pText.length(), uFormat, Color);
}

void DXFont::Draw(int FontObject, RECT& rRect, LPCTSTR Text, INT Count, UINT uFormat, D3DCOLOR Color)
{
	if (static_cast<int>(vFontResource.size()) <= FontObject)
		return;


	const stFontStructure pFont = vFontResource[FontObject];
	int iCount = Count;

	if (iCount <= 0)
		iCount = lstrlenA(Text);

	static char szBuf[2048];
	memset(szBuf, 0, 2048);
	wsprintfA(szBuf, "%s ", Text);


	if (bCreatedFonts)
	{
		if (pFont.hFont)
		{
			pFont.hFont->DrawTextA(nullptr, szBuf, iCount + 1, &rRect, uFormat, Color);
		}
	}
}

void DXFont::Draw(int FontObject, RECT& rRect, const std::string& strText, UINT uFormat, D3DCOLOR Color)
{
	Draw(FontObject, rRect, strText.c_str(), strText.length(), uFormat, Color);
}

void DXFont::DrawBellatraFont(int x, int y, float Size, DWORD Color, const char* Text, ...)
{
	va_list ap;
	char szBuf[2048];
	memset(szBuf, 0, 2048);
	va_start(ap, Text);
	_vsnprintf_s(szBuf, 2048, 2047, Text, ap);
	va_end(ap);

	DrawFontImage(szBuf, x, y, Color, Size);
}


void DXFont::OnResetDevice()
{
	if (vFontResource.size() == 0)
		return;

	for (int i = 0; i < static_cast<int>(vFontResource.size()); i++)
		vFontResource[i].hFont->OnResetDevice();
}

void DXFont::OnLostDevice()
{
	if (vFontResource.size() == 0)
		return;

	for (int i = 0; i < static_cast<int>(vFontResource.size()); i++)
		vFontResource[i].hFont->OnLostDevice();
}

void DXFont::ReCreateFontDevice()
{
	if (vFontResource.size() == 0)
		return;

	auto pFont = std::make_shared<stFontStructure>();

	for (int i = 0; i < static_cast<int>(vFontResource.size()); i++)
	{

	}

	bCreatedFonts = true;
}


DXFont::~DXFont()
{
}

D3DCOLOR fontColor = D3DCOLOR_ARGB(255, 255, 255, 255);

int DX::DrawText(HDC hdc, int x, int y, LPCTSTR lpszString, int nCount)
{
	if (dsDrawOffset_X || dsDrawOffset_Y)
	{
		if (dsDrawOffsetArray & dsARRAY_RIGHT)
			x += dsDrawOffset_X;

		if (dsDrawOffsetArray & dsARRAY_BOTTOM)
			y += dsDrawOffset_Y;
	}

	RECT rc;
	rc.left = x;
	rc.right = x + RESOLUTION_WIDTH;
	rc.top = y;
	rc.bottom = y + RESOLUTION_HEIGHT;

	FONTHANDLER->Draw(0, rc, lpszString, nCount, DT_WORDBREAK, fontColor);

	return TRUE;
}

int DX::DrawTextFont(int x, int y, LPCTSTR lpszString, COLORREF crColor, int Font)
{
	if (dsDrawOffset_X || dsDrawOffset_Y)
	{
		if (dsDrawOffsetArray & dsARRAY_RIGHT)
			x += dsDrawOffset_X;

		if (dsDrawOffsetArray & dsARRAY_BOTTOM)
			y += dsDrawOffset_Y;
	}

	RECT rc;
	rc.left = x;
	rc.right = x + RESOLUTION_WIDTH;
	rc.top = y;
	rc.bottom = y + RESOLUTION_HEIGHT;

	FONTHANDLER->Draw(Font, rc, lpszString, strlen(lpszString), DT_WORDBREAK, D3DCOLOR_RGBA(GetRValue(crColor), GetGValue(crColor), GetBValue(crColor), 255));

	return TRUE;
}

void DX::SetFontTextColor(COLORREF crColor)
{
	fontColor = D3DCOLOR_RGBA(GetRValue(crColor), GetGValue(crColor), GetBValue(crColor), 255);
}


BOOL DX::GetTextExtentPoint(HDC hdc, LPCTSTR lpString, int c, LPSIZE  lpSize)
{

	RECT rcRect = { 0,0,0,0 };

	lpSize->cx = FONTHANDLER->GetWidthText(0, lpString, c);
	lpSize->cy = FONTHANDLER->GetHeightText(0, lpString, RECT{ 0,0,0,0 });

	return TRUE;
}