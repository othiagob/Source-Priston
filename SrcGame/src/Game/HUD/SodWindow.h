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

class SodWindow
{

public:
    static              SodWindow* GetInstance() { static SodWindow instance; return &instance; }

    bool openFlag = false;
    bool showDetails = false;
    bool init = true;

    INT RetrieveSodRanking(RankingSodPckg* AllRanking);
    INT RetrieveSodMoney();
    void OpenNpc(bool* p_open);


private:

};

