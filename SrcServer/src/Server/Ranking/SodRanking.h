#pragma once
#include <Windows.h>

#define OPEN_SODWINDOW_NPC 0x50600033
#define RETRIEVE_SOD_MONEY 0x50600034

struct RankingSod
{
    char szName[32];
    short level;
    short jobCode;
    int sodScore;
    int sodKillCount;
    int sodTotalScore;
    short sodTotalUser;
    SYSTEMTIME Date;

    int clanCode;
};

struct RankingSodPckg
{
    int size;
    int code;
    float impostoArrecadado;
    char clanZang[32];
    RankingSod AllRanking[91];
};

class SodRanking
{
public:
	static              SodRanking* GetInstance() { static SodRanking instance; return &instance; }
	void                ResetRanking();

    INT SendSodRanking(rsPLAYINFO* Player);
    INT RetrieveSodMoney(rsPLAYINFO* Player);

private:
	int                teste;
};

