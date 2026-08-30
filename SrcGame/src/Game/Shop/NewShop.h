#ifndef NEWSHOP
#define NEWSHOP
#endif

#include "Utils\\strings.h"
#include "imGui/imgui.h"

#define PACKET_SEND_COIN_GAME   0x49470002

#define NewShopItems_OPENNPC	  0x252030
#define NewShopItems_ReceiveItems 0x252031
#define NewShopItems_FinishPurchase 0x252032

#define NewShopItems_CheckNick 0x252040
#define NewShopItems_ChangeNick 0x252041
#define	NewShopItems_ChangeClass 0x252042

#define CURL_STATICLIB

struct sCompressedDataShop
{
	char pCompressedData[6000];
	int compressedSize;
};

struct NEWSHOP_COMPRESSEDPCKG {
	int size;
	int code;
	sCompressedDataShop CompressedDataPckg;
};

struct ItemsByCategory
{
	int CategoryID;
	int SubCategoryID;
	char	ItemCode[32];
	char	ItemName[64];
	int Price;
	int Discount;

	int imgPosition;
};

struct sFinishPurchase
{
	int size;
	int code;

	ItemsByCategory Item;
	int Spec;
	int Quantity;
};

struct ITEMS_INFOCKG {
	int size;
	int code;
	ItemsByCategory Items[200] = { 0 };
};

class NewShop
{
private:
	bool first = true;
	bool restaureItem = false;
	char Path[128] = { 0 };
	int w = 0;
	int h = 0;

	int Coin;
public:
	static              NewShop* GetInstance() { static NewShop instance; return &instance; }

	void ReceiveItems(NEWSHOP_COMPRESSEDPCKG* Data);

	std::vector<ItemsByCategory> ShopItems;

	// Donate PayPal
	char idPaypal[32];
	char Amount[32];

	bool openFlag = false;
	bool editingNick;
	void OpenNpc(bool* p_open);
	void Donation(char amount[32]);
	void RestaureItems();
	void ChangeNick();
	void ChangeClass();
	int TextEditCallback2(ImGuiInputTextCallbackData* data);
	void LoadVipOptions();

	void	 MinusCoin(int coin);
	void	 PlusCoin(int coin);
	void RecvCoin(int coin);
	int  GetCoin() { return Coin; };
};

