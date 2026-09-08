#pragma once

#include "Utils\\strings.h"
#include <vector>
#include <string>

#define PACKET_SEND_TIME_GAME              0x252036
#define NewShopItems_OPENNPC_Time          0x252033
#define NewShopItems_ReceiveItems_Time     0x252034
#define NewShopItems_FinishPurchase_Time   0x252035

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
	int Time = 0;

	float m_winX = 0.0f;
	float m_winY = 0.0f;
	float m_winW = 0.0f;
	float m_winH = 0.0f;
	void* m_titleTex = nullptr;
	int m_titleW = 0;
	int m_titleH = 0;
	bool m_titleTried = false;
	int m_category = 3;
	int m_subTab = 6;
	int m_selectedIndex = -1;
	bool m_confirmBuy = false;
	bool m_shopReady = false;

	void PushWindowStyle();
	void PopWindowStyle();
	void DrawWindowChrome(float headerH);
	void DrawConfirmChrome();
	void DrawTitleHeader(bool* p_open);
	void DrawSectionHeader(const char* title);
	void EnsureTitleTexture();
	void EnsureShopLoaded();
	void DrawShopBody();
	void DrawItemList();
	void DrawItemDetail();
	void DrawBuyConfirm();
	void RequestPreview(const char* itemCode);
	void EnsureSelection();
	const ItemsByCategoryTime* SelectedItem() const;
	const struct sITEM* FindPreview(const char* itemCode) const;
	std::string ToUtf8(const char* src) const;
	std::string PremiumDescription(const ItemsByCategoryTime& item) const;

public:
	static NewShopTime* GetInstance() { static NewShopTime instance; return &instance; }

	void ReceiveItems(NewShopTime_COMPRESSEDPCKG* Data);

	std::vector<ItemsByCategoryTime> ShopItems;

	bool openFlag = false;
	void OpenNpc(bool* p_open);
	void ReceiveTime(sCoinPlayerTime* pTime);
	int  GetTime() { return Time; }
	bool IsBlockingMouse(int x, int y) const;
};
