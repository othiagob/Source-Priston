#pragma once

#include "Utils/strings.h"
#include "smPacket.h"

#define NewShopItems_OPENNPC	  0x252030
#define NewShopItems_ReceiveItems 0x252031
#define NewShopItems_FinishPurchase 0x252032

#define NewShopItems_CheckNick 0x252040
#define NewShopItems_ChangeNick 0x252041
#define NewShopItems_ChangeClass 0x252042

#define PACKET_SEND_COIN_GAME   0x49470002
#define PACKET_DONATE_PAYPAL	0x49470004

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

struct ITEMS_INFOCKG {
	int size;
	int code;
	ItemsByCategory Items[200] = { 0 };
};

struct NEW_DONATE
{
	char IDPaypal[32];
	char Amount[32];
};

struct Donate
{
	int size;
	int code;
	NEW_DONATE PckDonate;
};

enum WhereCoinsComeFrom
{
	FROM_DONATION,
	FROM_COMMAND,
	FROM_QUEST,
	FROM_ERAGON,
	FROM_TRADE,
	FROM_QUESTIONS
};
	

class NewShop
{

public:
	static              NewShop* GetInstance() { static NewShop instance; return &instance; }

	int isDoubleDonation = 0;

	void SendItems(rsPLAYINFO* Player);
	void removePlayerCoins(rsPLAYINFO* Player, int coinsToRemove = 0);
	void FinishPurchase(rsPLAYINFO* Player, sFinishPurchase* Item);
	void SendCoinToGame(rsPLAYINFO* Player);
	void addCoinsToPlayer(rsPLAYINFO* gm, char* id, int coins, int isFromDonation, WhereCoinsComeFrom WhereComeFrom);

	// Troca de nick
	bool checkNick(rsPLAYINFO* Player, sFinishPurchase* Item);
	void ChangeNick(rsPLAYINFO* Player, sFinishPurchase* Item);
	void ChangeClass(rsPLAYINFO* Player, sFinishPurchase* Item);

	// Coins
	int GetPlayerCoin(rsPLAYINFO* lpPlayInfo);
	bool TransferTradeCoin(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Coin);
	bool CheckTradeCoin(rsPLAYINFO* Player, rsPLAYINFO* Player2, TRANS_TRADEITEMS* lpTransTradeItems);


	// Donate PayPal
	void receivePaypalDonation(rsPLAYINFO* Player, Donate* NewDonate);
	void confirmPaypalDonation();
};

