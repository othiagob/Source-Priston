#pragma once
#include <Windows.h>

#define OPEN_RANKING_NPC 0x51800010

using namespace std;

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

class TopRanking
{
public:
    static              TopRanking* GetInstance() { static TopRanking instance; return &instance; }
    void                sendTopPlayers(rsPLAYINFO* Player);
    void                LevelUp(rsPLAYINFO* Player);
    void                UpdateExp(rsPLAYINFO* Player);

    bool update = true;
};

