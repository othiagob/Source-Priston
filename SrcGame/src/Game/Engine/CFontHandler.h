#pragma once

#include <vector>
using namespace std;

#include "CFont.h"

class CFontHandler
{
public:
	static CFontHandler* getInstance();

	CFontHandler();
	~CFontHandler();
	CFont* GetFont(char* FontName, int FontSize, bool bBold);
	CFont* GetFontDefault(bool bBold);
	CFont* CreateFontInfo(char* FontName, int FontSize, bool bBold);
	void Release();
	void Reset();
	void Lost();

private:
	vector<CFont*> vFonts;
};

#define CFONTHANDLER CFontHandler::getInstance()
