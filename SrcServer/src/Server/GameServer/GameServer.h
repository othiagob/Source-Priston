#pragma once
#include <Windows.h>
#include "..\\fileread.h"


class GameServer
{
public:
    GameServer();
    virtual ~GameServer();


    void readMonstersFromDB();
    void readItemsFromDB();
    void readNpcsFromDB(smCHAR* Npc, int uniqueID);
    int readFieldNpc(smTRNAS_PLAYERINFO* Npc, char* szFile, int Pos);
    int readFieldMob(rsSTG_MONSTER_LIST* StgMonster, smCHAR_INFO* StgCharInfo, int ActiveMonsters, char* szFile);

private:
};

