#pragma once

#include "Utils\\strings.h"
#include "imGui/imgui.h"
#include <vector>
#include <string>

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
	char pCompressedData[7800];
	int compressedSize;
};

struct NEWSHOP_COMPRESSEDPCKG {
	int size;
	int code;
	sCompressedDataShop CompressedDataPckg;
	int chunkIndex;
	int totalChunks;
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
	int Coin = 0;

	float m_winX = 0.0f;
	float m_winY = 0.0f;
	float m_winW = 0.0f;
	float m_winH = 0.0f;
	void* m_titleTex = nullptr;
	int m_titleW = 0;
	int m_titleH = 0;
	bool m_titleTried = false;
	int m_category = 1;
	int m_subTab = 1;
	int m_selectedIndex = -1;
	bool m_confirmBuy = false;
	bool m_confirmVip = false;
	int m_confirmRestaure = -1;
	bool m_confirmNick = false;
	bool m_hoverPreview = false;
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
	void DrawServicePanel();
	void DrawBuyConfirm();
	void DrawVipConfirm();
	void DrawRestaureConfirm();
	void DrawNickConfirm();
	void RequestPreview(const char* itemCode);
	void PlacePreviewNearMouse();
	void EnsureSelection();
	const ItemsByCategory* SelectedItem() const;
	const struct sITEM* FindPreview(const char* itemCode) const;
	std::string ToUtf8(const char* src) const;
	std::string PremiumDescription(const ItemsByCategory& item) const;

public:
	static NewShop* GetInstance() { static NewShop instance; return &instance; }

	void ReceiveItems(NEWSHOP_COMPRESSEDPCKG* Data);

	std::vector<ItemsByCategory> ShopItems;

	char idPaypal[32] = {};
	char Amount[32] = {};

	bool openFlag = false;
	bool editingNick = false;
	void OpenNpc(bool* p_open);
	void Donation(char amount[32]);
	void RestaureItems();
	void ChangeNick();
	void ChangeClass();
	int TextEditCallback2(ImGuiInputTextCallbackData* data);
	void LoadVipOptions();

	void MinusCoin(int coin);
	void PlusCoin(int coin);
	void RecvCoin(int coin);
	int  GetCoin() { return Coin; }
	bool IsBlockingMouse(int x, int y) const;
};
