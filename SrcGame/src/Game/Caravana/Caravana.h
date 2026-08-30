#pragma once
#include <Windows.h>
#include "smPacket.h"

#define smTRANSCODE_OPEN_CARAVAN 0x4580605
#define	smTRANSCODE_CARAVAN 0x4580606

class Caravana
{
public:
	static              Caravana* GetInstance() { static Caravana instance; return &instance; }

	int CloseCaravanCheckFlag2 = 0;
	int CloseCaravanCheckFlag3 = 0;

	void showCaravan(smCHAR* Player);

	int OpenFlag;
	int CaravanCheckSum;
	Caravan myCaravan;

	bool ChangeName;

	int Init();
	int SetCaravanItemAreaCheck(sITEM* pItem);
	int CrashWareHouseItem(RECT& desRect, int PassItemIndex = 0);
	int ChangeCaravanItem(sITEM* pItem);
	int LastSetCaravanItem(sITEM* pItem);
	int PickUpCaravanItem(int x, int y, int PickUpFlag = 0);
	bool GetEmptyArea(sITEM* pItem, POINT* EmptyPos);
	bool CanStoreItem(sITEM* pItem);
	int GetWeight();
	int LoadCaravanItemImage();
	int BackUpInvenItem();
	int RestoreInvenItem();
	int CloseCaravan();
	int ReFormCaravan();
	int CheckCaravanForm();
	int CopyItemNotPickUp(sITEM* pItem, int JumpIndex);
	int CheckCopyItem();
	int DeleteCopyItem();

	struct	TRANS_CARAVAN {
		int size, code;

		DWORD	dwChkSum;
		WORD	wVersion[2];
		int		WareHouseMoney;
		int		UserMoney;
		DWORD	dwTemp[5];

		int		DataSize;
		BYTE	Data[sizeof(sITEM) * 100 + 256];
	};

private:
};
