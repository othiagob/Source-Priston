#pragma once
#include <Windows.h>
#include "Utils/strings.h"
#include "smPacket.h"

#define OPEN_RECOVERY_AGING_NPC 0x50600030
#define GET_FAILED_ITEMS 0x50600031
#define RECOVER_AGING_ITEM 0x50600032

struct FailedItems {
    char ItemName[32];
    int AgingNum;
    int ItemHead;
    int ItemChkSum;
    SYSTEMTIME Date;
    char ItemCode[32];

    int imgPosition;
};

struct FailedItemsPckg {
    int size;
    int code;
    int price;
    FailedItems FailedItems[20];
};

class RestaureItem
{
    RestaureItem();
    virtual ~RestaureItem();

public:
    static              RestaureItem* GetInstance() { static RestaureItem instance; return &instance; }

    INT sendFailedItems(rsPLAYINFO* Player);
    INT RecoverItem(rsPLAYINFO* Player, int ItemHead, int ItemChkSu);
    INT OpenNpc();

private:
};

