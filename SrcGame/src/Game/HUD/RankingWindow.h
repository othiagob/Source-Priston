#pragma once
#include <Windows.h>
#include <vector>

#define OPEN_RANKING_NPC 0x51800010

struct TopPlayers {
    char playerName[32];
    int playerClass = 0;
    int playerLevel = 0;
    int playerBps = 0;
};

struct TopPlayerPckg {
    int size;
    int code;
    TopPlayers Players[181];
};

class RankingWindow
{

public:
    static              RankingWindow* GetInstance() { static RankingWindow instance; return &instance; }
    INT ReceivePlayers(TopPlayerPckg* FailedItems);
    INT OpenNPCRanking();
    bool openFlag = false;
    void OpenNpc(bool* p_open);

private:
};

