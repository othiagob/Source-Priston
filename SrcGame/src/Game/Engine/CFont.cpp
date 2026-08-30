#include "..\\sinbaram\\sinLinkHeader.h"
#include "CFont.h"

CFont::CFont(char* name, int size, bool bold)
{
	lstrcpy(FontName, name);
	FontSize = size;
	bBold = bold;

	auto hr = D3DXCreateFont(GRAPHICDEVICE, FontSize, 0, bBold ? FW_BOLD : FW_NORMAL, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLEARTYPE_NATURAL_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FontName, &Font);
	if (hr != S_OK)
	{
		MessageBoxA(0, FontName, 0, 0);
		switch (hr)
		{
		case D3DERR_INVALIDCALL:
			MessageBoxA(0, "D3DERR_INVALIDCALL", 0, 0);
			break;
		case D3DXERR_INVALIDDATA:
			MessageBoxA(0, "D3DXERR_INVALIDDATA", 0, 0);
			break;
		case E_OUTOFMEMORY:
			MessageBoxA(0, "E_OUTOFMEMORY", 0, 0);
			break;
		}
	}
}

void CFont::Release()
{
	if (Font)
		Font->Release();
	Font = nullptr;
}

void CFont::Reset()
{
	if (Font)
		Font->OnResetDevice();
}

void CFont::Lost()
{
	if (Font)
		Font->OnLostDevice();
}

void CFont::GetTextSize(const char* lpString, SIZE* lpSize, int x)
{
	if (!Font)
		return;

	RECT rcRect = { 0,0,x,0 };
	Font->DrawText(NULL, lpString, lstrlen(lpString), &rcRect, DT_CALCRECT | (x ? DT_WORDBREAK : 0), FontColor);

	lpSize->cx = rcRect.right - rcRect.left;
	lpSize->cy = rcRect.bottom - rcRect.top;
}

void CFont::DrawText(LPCSTR lpString, RECT* pRect, DWORD uFormat)
{
	if (!Font)
		return;

	Font->DrawText(NULL, lpString, lstrlen(lpString), pRect, uFormat, FontColor);
}

void CFont::DrawRect(RECT rc, char* lpszString, int nCount, UINT uFormat)
{
	Font->DrawText(NULL, lpszString, lstrlen(lpszString), &rc, uFormat, FontColor);
}
