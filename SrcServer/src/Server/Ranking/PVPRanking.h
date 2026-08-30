#pragma once
#include <Windows.h>

class PVPRanking
{
public:
	static              PVPRanking* GetInstance() { static PVPRanking instance; return &instance; }
	void                ResetRanking(rsPLAYINFO* lpPlayInfo);
	void                sendKillStreak(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int killStreak);
	void				setBuff(char* nome);
private:
	int                teste;
};

