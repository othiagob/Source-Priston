#pragma once

#include "Utils/strings.h"
#include "smPacket.h"

#define NewShopItems_OPENNPC_Time	  0x252033
#define NewShopItems_ReceiveItems_Time 0x252034
#define NewShopItems_FinishPurchase_Time 0x252035

#define PACKET_SEND_TIME_GAME   0x252036

struct ItemsByCategoryTime
{
	int CategoryID;
	int SubCategoryID;
	char	ItemCode[32];
	char	ItemName[64];
	int Price;
	int Discount;

	int imgPosition;
};

struct sFinishPurchaseTime
{
	int size;
	int code;

	ItemsByCategoryTime Item;
	int Spec;
	int Quantity;
};

struct sCompressedDataShopTime
{
	char pCompressedData[6000];
	int compressedSize;
};

struct NewShopTime_COMPRESSEDPCKG {
	int size;
	int code;
	sCompressedDataShopTime CompressedDataPckg;
};

struct ITEMS_INFOCKGTIME {
	int size;
	int code;
	ItemsByCategoryTime Items[200] = { 0 };
};

struct sCoinPlayerTime
{
	int size;
	int code;
	int Coin;
};

class NewShopTime
{

public:
	static              NewShopTime* GetInstance() { static NewShopTime instance; return &instance; }

	int isDoubleDonation = 0;

	void SendItems(rsPLAYINFO* Player);
	int getPlayerTime(rsPLAYINFO* Player);
	void removePlayerTime(rsPLAYINFO* Player, int coinsToRemove = 0);
	void FinishPurchase(rsPLAYINFO* Player, sFinishPurchaseTime* Item);
	void SendTimeToGame(rsPLAYINFO* Player);
	void addTimeToPlayer(rsPLAYINFO* Player, int Credits);
};

