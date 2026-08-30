#pragma once
#include <Windows.h>
#include <vector>

#define smTRANSCODE_GET_QUEST_STATUS	0x50600001
#define smTRANSCODE_SET_QUEST_STATUS	0x50600002
#define smTRANSCODE_GET_ALL_QUESTS	    0x50600003
#define smTRANSCODE_START_NEW_QUEST     0x50600004
#define smTRANSCODE_CANCEL_QUEST        0x50600005
#define smTRANSCODE_FINISH_QUEST        0x50600006
#define smTRANSCODE_ADD_EXP_QUEST        0x50600007
#define smTRANSCODE_ALL_GOOD_ITEMSQUEST        0x50600011
#define smTRANSCODE_OPEN_NPC 0x50600012

struct sCompressedData
{
    char pCompressedData[6000];
    int compressedSize;
};

struct QUEST_COMPRESSEDPCKG {
    int size;
    int code;
    sCompressedData CompressedDataPckg;
};


    class Quest
    {
    public:
        enum QUEST_OBJECTIVE_TYPES {
            QUEST_KILL_MONSTER,
            QUEST_KILL_PLAYER,
            QUEST_GET_ITEM,
            QUEST_KILL_ANY_MONSTER
        };

        enum QUEST_TYPES {
            QUEST_UNIQUE_SOLO,
            QUEST_UNIQUE_PARTY,
            QUEST_DAILY_SOLO,
            QUEST_DAILY_PARTY,
            QUEST_UNIQUE_SOLOPARTY,
            QUEST_DAILY_SOLOPARTY,
            QUEST_REPEAT_SOLOPARTY,
            QUEST_REPEAT_SOLO,
            QUEST_REPEAT_PARTY
        };

       
        std::vector<QUESTPCKG*> vQuests;

        unsigned char* pUncompressedData = new unsigned char[3000];

        static              Quest* GetInstance() { static Quest instance; return &instance; }

        void getAllQuests(QUEST_COMPRESSEDPCKG* Data);
        int updateStatus(smCHAR* Monster);
        int CheckItemsFromInven();
        void requestStatus();
        void getAllStatus(QUEST_INFOPCKG* getStatus);
        void startNewQuest(int questID, int questType);
        void cancelQuest(int questID, int questType);
        void finishQuest(int questID, int questType, int questObjective);

    private:
    };

