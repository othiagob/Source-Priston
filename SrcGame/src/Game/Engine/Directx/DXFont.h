#pragma once

#include <string>
#include <vector>
#include <d3dx9.h>
#include <Windows.h>

struct stFontStructure
{
	char fname[32];
	ID3DXFont* hFont;
	int Width;
	int Height;
	BOOL italic;
	BOOL bold;
	UINT uWidthWhitespace;

	stFontStructure() {
		memset(fname, 0, sizeof(fname));
		hFont = nullptr;
		Width = 0;
		Height = 0;
		italic = FALSE;
		bold = FALSE;
		uWidthWhitespace = 0;
	}
};

namespace DX
{
	class DXFont
	{
	private:
		std::vector<stFontStructure> vFontResource;

	public:
		static DXFont* getInstance()
		{
			static DXFont inst;
			return &inst;
		}
		void Init();
		int CreateFont(const char* FontName, int iHeight, int iWidth, BOOL Italic, BOOL Bold, int MemValue);
		int GetWidthText(int FontObject, const char* Text, INT Size);
		int GetHeightText(int FontObject, const char* Text, RECT rRect);
		int GetHeightText(int FontObject, UINT uFormat, const char* Text, RECT rRect);
		int GetHeightText(int iFontID);
		void Draw(int FontObject, int x, int y, const std::string& pText, UINT uFormat, D3DCOLOR Color);
		void Draw(int FontObject, RECT& rRect, LPCTSTR Text, INT Count, UINT uFormat, D3DCOLOR Color);
		void Draw(int FontObject, RECT& rRect, const std::string& strText, UINT uFormat, D3DCOLOR Color);
		void DrawBellatraFont(int x, int y, float Size, DWORD Color, const char* Text, ...);
		void OnResetDevice();
		void OnLostDevice();
		void ReCreateFontDevice();
		virtual ~DXFont();
	};

	BOOL GetTextExtentPoint(HDC hdc, LPCTSTR lpString, int c, LPSIZE  lpSize);
	void SetFontTextColor(COLORREF crColor);
	int DrawText(HDC hdc, int x, int y, LPCTSTR lpszString, int nCount);
	int DrawTextFont(int x, int y, LPCTSTR lpszString, COLORREF crColor, int Font);
}
#define FONTHANDLER (DX::DXFont::getInstance())
