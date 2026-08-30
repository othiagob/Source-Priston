#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <utility>
#include "Utils\\Geometry.h"

struct EMOJIINFO
{
	int Mat;
	char szCode[32];
	float w, h;

	EMOJIINFO() {
		Mat = 0;
		memset(szCode, 0, sizeof(szCode));
		w = 0;
		h = 0;
	}
};

struct FormattedStringEmoji {
	std::string str;
	std::vector<std::pair<int, EMOJIINFO*>> emojis;
};

using namespace std;

class cEMOJI
{
public:
	cEMOJI();
	~cEMOJI();

	void Init();
	FormattedStringEmoji handleEmoji(const string& text);
	int getTextSize(const string& text);
	int getCutPos(const string& text, int StrMax);
	bool render(EMOJIINFO* emoji, int x, int y);

private:

	vector<EMOJIINFO*> Emojis;

	void AddEmoji(int Mat, string szCode);
	EMOJIINFO* getEmojiInfo(char* szCode);

};

extern cEMOJI cEmoji;