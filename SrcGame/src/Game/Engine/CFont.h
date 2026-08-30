#pragma once

class CFont
{
public:
	CFont(char* name, int size, bool bold);

	ID3DXFont* GetFont() { return Font; }
	char* GetFontName() { return FontName; }
	int GetFontSize() { return FontSize; }
	bool GetBold() { return bBold; }

	void Release();
	void Reset();
	void Lost();

	void GetTextSize(const char* lpString, SIZE* lpSize, int x = 0);
	void SetFontColor(D3DCOLOR color) { FontColor = color; }
	void DrawText(LPCSTR lpString, RECT* pRect, DWORD uFormat);
	void DrawRect(RECT rc, char* lpszString, int nCount, UINT uFormat);

private:
	char FontName[64];
	int FontSize;
	bool bBold;
	ID3DXFont* Font = nullptr;
	D3DCOLOR FontColor;
};
