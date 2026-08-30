#include "NewShopTime.h"
#include <zlib.h>
#include "../SQL.h"
#include "..\\GM\GM.h"
#include "..\\VIP\Vip.h"
#include "..\\Database\\SQLConnection.h"

#pragma comment (lib, "zlib.lib")

#ifdef _DEBUG
#pragma comment (lib, "zlib.lib")
#endif
#include <Chat/ChatServer.h>

extern int ReformItem(sITEMINFO* lpItemInfo);
extern int	rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);
extern int	rsAddInvenItem(rsPLAYINFO* lpPlayInfo, DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
extern int GetPostBoxFile(char* szID, char* szFileName);

extern rsPLAYINFO* FindUserFromID(char* szID);

int SendPostItemTime(char* id, char* name, char* ItemName, int iQuantity, int gold)
{
	char	szFileName[64];
	char	strBuff[512];

	HANDLE	hFile;
	DWORD	dwAcess;

	GetPostBoxFile(id, szFileName);
	hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	int FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);

	for (int i = 0; i < iQuantity; i++)
	{
		sprintf_s(strBuff, "%s %s %d \"Shopping\"\r\n", name, ItemName, gold);

		WriteFile(hFile, strBuff, lstrlen(strBuff), &dwAcess, NULL);
	}

	CloseHandle(hFile);

	return TRUE;
}

int NewShopTime::getPlayerTime(rsPLAYINFO* Player)
{
	int result = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT UserTime FROM Users WHERE username=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);

			if (db->Execute())
				db->GetData(1, PARAMTYPE_Integer, &result);
		}

		db->Close();
	}

	return result;
}

void NewShopTime::removePlayerTime(rsPLAYINFO* Player, int coinsToRemove)
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE Users SET UserTime-=? WHERE username=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&coinsToRemove, 1, PARAMTYPE_Integer);
			db->BindInputParameter(Player->szID, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}
}

void NewShopTime::SendItems(rsPLAYINFO* Player)
{
	NewShopTime_COMPRESSEDPCKG pckgToSend;
	ZeroMemory(&pckgToSend, sizeof(NewShopTime_COMPRESSEDPCKG));
	pckgToSend.size = sizeof(NewShopTime_COMPRESSEDPCKG);
	pckgToSend.code = NewShopItems_ReceiveItems_Time;

	ITEMS_INFOCKGTIME pckg;
	ZeroMemory(&pckg, sizeof(ITEMS_INFOCKGTIME));

	int i = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_ShopCoin);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM ShopItemsTime";

		if (db->Prepare(query))
		{
			db->Execute(FALSE);

			while (db->NextRow())
			{
				db->GetData(2, PARAMTYPE_Integer, &pckg.Items[i].CategoryID);
				db->GetData(3, PARAMTYPE_Integer, &pckg.Items[i].SubCategoryID);
				db->GetData(4, PARAMTYPE_String, pckg.Items[i].ItemCode, sizeof(pckg.Items[i].ItemCode));
				db->GetData(5, PARAMTYPE_String, pckg.Items[i].ItemName, sizeof(pckg.Items[i].ItemName));
				db->GetData(6, PARAMTYPE_Integer, &pckg.Items[i].Price);
				db->GetData(7, PARAMTYPE_Integer, &pckg.Items[i].Discount);
				pckg.Items[i].imgPosition = 0;
				i++;
			}
		}

		db->Close();
	}

	unsigned long nCompressedDataSize = pckgToSend.size;
	unsigned char* UnpCompressedData = new unsigned char[nCompressedDataSize];
	int nResult = compress2((Bytef*)pckgToSend.CompressedDataPckg.pCompressedData, &nCompressedDataSize, (Bytef*)&pckg.Items, sizeof(pckg.Items), 9);
	pckgToSend.CompressedDataPckg.compressedSize = nCompressedDataSize;

	if (nResult == Z_OK)
	{
		if (Player != nullptr)
			Player->lpsmSock->Send((char*)&pckgToSend, pckgToSend.size, TRUE); // Pacote com os itens
	}

	UTILS->Alert(Player, "> Quanto mais tempo online, mais TC (Timer Coins) você recebe!");
}

void NewShopTime::addTimeToPlayer(rsPLAYINFO* Player, int Credits)
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const queryUpdate = "UPDATE Users SET UserTime+=? WHERE username=?";

		if (db->Prepare(queryUpdate))
		{
			db->BindInputParameter(&Credits, 1, PARAMTYPE_Integer);
			db->BindInputParameter(Player->szID, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}
}

void NewShopTime::FinishPurchase(rsPLAYINFO* Player, sFinishPurchaseTime* Item)
{
	if (Item->Item.SubCategoryID == 100 && Player->vipLevel > 0)
	{
		UTILS->Alert(Player, "> Você já possui VIP!");
		UTILS->Alert(Player, "> Não é possível renovar antes do término");
		return;
	}

	int PlayerCoins = getPlayerTime(Player);
	int Price = (Item->Item.Price * Item->Quantity);
	psITEM* lpsItem;
	lpsItem = new psITEM;

	if (PlayerCoins >= Price)
	{
		PlayerCoins -= Price;
		removePlayerTime(Player, Price);

		if (Item->Item.SubCategoryID != 100) // Vip
		{
			CreateItemPerf(lpsItem, Item->Item.ItemCode, Item->Spec);

			TRANS_ITEMINFO		TransItemInfo;
			ReformItem(&lpsItem->ItemInfo);
			TransItemInfo.code = smTRANSCODE_PUTITEM;
			TransItemInfo.size = sizeof(TRANS_ITEMINFO);
			memcpy(&TransItemInfo.Item, &lpsItem->ItemInfo, sizeof(sITEMINFO));
			rsRegist_ItemSecCode(Player, &TransItemInfo, 1);

			if (Player->lpsmSock)
				Player->lpsmSock->Send((char*)&TransItemInfo, TransItemInfo.size, TRUE);

			SERVERCHAT->SendChatEx(Player, EChatColor::CHATCOLOR_Global, "> Item %s (x%d) comprado, Você agora possui %d Time Coins restantes", Item->Item.ItemName, Item->Quantity, PlayerCoins);
			SendTimeToGame(Player);

			// Log da compra
			HANDLE hFile;
			char szFileName[255];
			char szLog[500];
			SYSTEMTIME	SystemTime;

			GetLocalTime(&SystemTime);
			sprintf(szFileName, "Logs\\SHOP_LOG_TIME\\%d-%d.log", SystemTime.wMonth, SystemTime.wDay);
			CreateDirectory("Logs\\SHOP_LOG", NULL);
			sprintf(szLog, "%d:%d:%d - (ID : %s) (Name : %s) Comprou (ITEM : %s) (CODE : %s)\r\n", SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond,
				Player->szID, Player->szName, Item->Item.ItemName, Item->Item.ItemCode);

			hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (hFile != INVALID_HANDLE_VALUE)
			{
				DWORD dwFileLength = GetFileSize(hFile, NULL);
				DWORD dwAccess;
				SetFilePointer(hFile, dwFileLength, NULL, FILE_BEGIN);
				WriteFile(hFile, szLog, strlen(szLog), &dwAccess, NULL);
				CloseHandle(hFile);
			}

			// Limpa o item da memória
			if (lpsItem)
				delete lpsItem;
		}
		else
		{
			Vip::GetInstance()->addNewVip(Player, 1);
		}
	}
	else
	{
		SERVERCHAT->SendChat(Player, EChatColor::CHATCOLOR_Error, "Você não possui Time Coins suficientes.");
	}

}

void NewShopTime::SendTimeToGame(rsPLAYINFO* Player)
{
	if (Player && Player->lpsmSock)
	{
		sCoinPlayerTime packet;
		ZeroMemory(&packet, sizeof(packet));

		int PlayerCoins = getPlayerTime(Player);

		packet.size = sizeof(sCoinPlayerTime);
		packet.code = PACKET_SEND_TIME_GAME;
		packet.Coin = PlayerCoins;

		Player->lpsmSock->Send((char*)&packet, packet.size, TRUE);
	}
}
