#ifndef NEWSHOP
#define NEWSHOP
#endif

#include "Utils\\strings.h"

#define NewShopItems_OPENNPC_Time	  0x252033
#define NewShopItems_ReceiveItems_Time 0x252034
#define NewShopItems_FinishPurchase_Time 0x252035
#define PACKET_SEND_TIME_GAME   0x252036

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
private:
	bool first = true;
	bool restaureItem = false;
	char Path[128] = { 0 };
	int w = 0;
	int h = 0;

public:
	static              NewShopTime* GetInstance() { static NewShopTime instance; return &instance; }

	void ReceiveItems(NewShopTime_COMPRESSEDPCKG* Data);

	std::vector<ItemsByCategoryTime> ShopItems;

	// Donate PayPal
	char idPaypal[32];
	char Amount[32];

	bool openFlag = false;
	void OpenNpc(bool* p_open);
	void Donation(char amount[32]);
	void RestaureItems();
	void LoadVipOptions();
	void ReceiveTime(sCoinPlayerTime* pTime);

private:
	int Time = 0;
};

