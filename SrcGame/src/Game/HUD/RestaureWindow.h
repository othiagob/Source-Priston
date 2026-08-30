#pragma once
#include <Windows.h>

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

class RestaureWindow
{

public:
    static              RestaureWindow* GetInstance() { static RestaureWindow instance; return &instance; }

    bool openFlag = false;

    INT getFailedItems();
    INT ReceiveFailedItems(FailedItemsPckg* FailedItems);
    void OpenNpc(bool* p_open);

private:
};

