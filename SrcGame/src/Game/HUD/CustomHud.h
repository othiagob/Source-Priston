#pragma once
#include <Windows.h>

#ifndef _NEW_FONT
#define _NEW_FONT
#define DIRECTINPUT_VERSION 0x0800
#include <tchar.h>
#include <dinput.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_dx9.h"
#include "imGui/imgui_impl_win32.h"


#include <Windows.h>
#include <d3d9.h>
#include <string>
#include "..\\drawsub.h"


struct sBOX
{
	int iX;
	int iY;
	int iWidth;
	int iHeight;
};

struct LetterData
{
	int				iID;
	sBOX sBox;
	int				iPaddingX, iPaddingY;
};

struct FontData
{
	int iLineHeight;
	int iLettersCount;
	int iBaseY;

	std::vector<LetterData> vLetters;
};

enum BMPFonts
{
	ORANGE_FONT,
	RED_FONT,
	BLUE_FONT,
	MAX_FONT,
};

class cBMPFONT
{
public:
	cBMPFONT(const std::string& file, LPDIRECT3DTEXTURE9 dds);
	void DrawBmpFont(int x, int y, char* szText, int len);
	int GetTextWidth(char* szText);

private:
	LPDIRECT3DTEXTURE9 ddsFont;
	char* szData;
	FontData sFont;

	void ReadFontData(const std::string& file);
	LetterData GetLetterData(int iID);
};

extern cBMPFONT* BmpFont[BMPFonts::MAX_FONT];

void InitBmpFont();

#endif

class CustomHud
{
public:
	static              CustomHud* GetInstance() { static CustomHud instance; return &instance; }
	int                Button();
	bool                MouseAction(int x, int y, int w, int h);

	bool				GotClick;
private:
};

