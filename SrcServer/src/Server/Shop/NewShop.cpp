#include <zlib.h>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "..\\Database\\SQLConnection.h"
#include "NewShop.h"
#include "..\\SQL.h"
#include "..\\GM\GM.h"
#include "..\\VIP\Vip.h"
#include "Utils/_common.h"
#include "Character/record.h"

struct sTRADE
{
	sITEM TradeItem[20];
	int Money;
	int Coin;
	int Weight;
	int CheckFlag;
	int CheckMoney;
};

extern int ReformItem(sITEMINFO* lpItemInfo);
extern int	rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);
extern int	rsAddInvenItem(rsPLAYINFO* lpPlayInfo, DWORD dwCode, DWORD dwHead, DWORD dwChkSum);
extern int GetPostBoxFile(char* szID, char* szFileName);
extern int DecodeCompress(BYTE* lpSrcData, BYTE* lpDestData);
extern rsPLAYINFO* FindUserFromID(char* szID);

bool NewShop::CheckTradeCoin(rsPLAYINFO* Player, rsPLAYINFO* Player2, TRANS_TRADEITEMS* lpTransTradeItems)
{
	int UserCoin = 0;
	sTRADE	sTradeCoin;

	if (!Player || !Player2) return FALSE;

	DecodeCompress(lpTransTradeItems->TradeBuff, (BYTE*)&sTradeCoin);

	if (sTradeCoin.Coin > 0)
	{
		UserCoin = GetPlayerCoin(Player);

		if (!UserCoin || UserCoin < sTradeCoin.Coin)
		{
			return FALSE;
		}
	}

	return TRUE;
}

int NewShop::GetPlayerCoin(rsPLAYINFO* lpPlayInfo)
{
	int result = 0;
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT UserCoin FROM Users WHERE username=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(lpPlayInfo->szID, 1, PARAMTYPE_String);

			if (db->Execute())
				db->GetData(1, PARAMTYPE_Integer, &result);
		}

		db->Close();
	}

	return result;
}

bool NewShop::TransferTradeCoin(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int Coin)
{
	int UserCoin = GetPlayerCoin(lpPlayInfo);

	if (UserCoin >= Coin)
	{
		addCoinsToPlayer(lpPlayInfo, lpPlayInfo->szID, -Coin, true, WhereCoinsComeFrom::FROM_TRADE);
		addCoinsToPlayer(lpPlayInfo, lpPlayInfo2->szID, Coin, true, WhereCoinsComeFrom::FROM_TRADE);
		SendCoinToGame(lpPlayInfo);
		SendCoinToGame(lpPlayInfo2);

		auto db = SQLConnection::GetConnection(DATABASEID_LogDB);

		if (db && db->Open())
		{
			const char* const query = "INSERT INTO TradeCoin VALUES (?,?,?,?,?,?)";

			if (db->Prepare(query))
			{
				char szDateTime[20];
				GetDateTime(szDateTime);

				db->BindInputParameter(lpPlayInfo->szID, 1, PARAMTYPE_String);
				db->BindInputParameter(lpPlayInfo2->szID, 2, PARAMTYPE_String);
				db->BindInputParameter(lpPlayInfo->szName, 3, PARAMTYPE_String);
				db->BindInputParameter(lpPlayInfo2->szName, 4, PARAMTYPE_String);
				db->BindInputParameter(&Coin, 5, PARAMTYPE_Integer);
				db->BindInputParameter(szDateTime, 6, PARAMTYPE_String);
				db->Execute();
			}

			db->Close();
		}

		char msg[128] = { 0 };
		sprintf_s(msg, sizeof(msg), "> Você enviou %d Coins para %s, saldo restante %d Coins", Coin, lpPlayInfo2->szName, (UserCoin - Coin));
		GameMasters::getInstance()->Alert(lpPlayInfo, msg);

		return true;
	}

	return false;
}

int SendPostItem(char* id, char* name, char* ItemName, int iQuantity, int gold)
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

void NewShop::receivePaypalDonation(rsPLAYINFO* Player, Donate* NewDonate)
{

	int Amount = 0;
	int Value = atoi(NewDonate->PckDonate.Amount);

	// Valores de coins referentes as doações
	switch (Value) {
	case 5:
		Amount = 25;
		break;
	case 10:
		Amount = 50;
		break;
	case 30:
		Amount = 150;
		break;
	case 50:
		Amount = 300;
		break;
	case 100:
		Amount = 575;
		break;
	case 200:
		Amount = 1100;
		break;
	default:
		Amount = 0;
	}

	auto db = SQLConnection::GetConnection(DATABASEID_ShopCoin);

	if (db && db->Open())
	{
		const char* const query = "INSERT INTO PendingDonations (ID,Name,IP,Value,Amount,Date,IDPaypal,PaymentID,PayerID,Approved) values (?,?,?,?,?,GETDATE(),?,0,0,0)";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->BindInputParameter(Player->lpsmSock->szIPAddr, 3, PARAMTYPE_String);
			db->BindInputParameter(&Value, 4, PARAMTYPE_Integer);
			db->BindInputParameter(&Amount, 5, PARAMTYPE_Integer);
			db->BindInputParameter(NewDonate->PckDonate.IDPaypal, 6, PARAMTYPE_String);
			db->Execute();
		}


		db->Close();
	}
}

extern int GetUserDataFile(char* szName, char* szFileName);
extern int GetUserInfoFile(char* szID, char* szFileName);

bool checkDataServer(char nick[32])
{
	char	szFile[256];
	WIN32_FIND_DATA		ffd;
	HANDLE				hFind;

	if (lstrlen(nick) >= 32) return TRUE;

	GetUserDataFile(nick, szFile);

	hFind = FindFirstFile(szFile, &ffd);
	FindClose(hFind);
	if (hFind != INVALID_HANDLE_VALUE) {
		return TRUE;
	}

	return FALSE;
}

bool NewShop::checkNick(rsPLAYINFO* Player, sFinishPurchase* Item)
{
	if (Item->Item.SubCategoryID != 101)
		return FALSE;

	bool canChange = FALSE;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM UserInfo WHERE Name=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);

			// Nick já em uso
			if (db->Execute())
			{
				GameMasters::getInstance()->Packet(Player, NewShopItems_CheckNick, 1, 0, 0, 0);
				canChange = FALSE;
			}
			else if (checkDataServer(Item->Item.ItemName))
			{
				GameMasters::getInstance()->Packet(Player, NewShopItems_CheckNick, 1, 0, 0, 0);
				canChange = FALSE;
			}
			else
			{
				GameMasters::getInstance()->Packet(Player, NewShopItems_CheckNick, 0, 0, 0, 0);
				canChange = TRUE;
			}

		}

		db->Close();
	}

	return canChange;
}

extern int DisconnectUser(smWINSOCK* lpsmSock);

int setChangeNickPrice()
{
	auto db = SQLConnection::GetConnection(DATABASEID_ServerDB);

	int price = 5000;

	if (db->Open())
	{
		if (db->Prepare("SELECT Price FROM ChangeNick"))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &price);
			}
		}

		db->Close();
	}

	return price; // Padrão
}

int setChangeClassPrice()
{
	auto db = SQLConnection::GetConnection(DATABASEID_ServerDB);

	int price = 10000;

	if (db->Open())
	{
		if (db->Prepare("SELECT Price FROM ChangeClass"))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &price);
			}
		}

		db->Close();
	}

	return price; // Padrão
}


bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}


void changePlayerClass(char Name[32], int newClass)
{
	char	szFile[256] = { 0 };
	TRANS_RECORD_DATA	TransRecordData;
	ZeroMemory(&TransRecordData, sizeof(TRANS_RECORD_DATA));

	GetUserDataFile(Name, szFile);

	auto fp = fopen(szFile, "rb");

	if (fp)
	{
		fread(&TransRecordData, sizeof(TRANS_RECORD_DATA), 1, fp);
		fclose(fp);

		TransRecordData.smCharInfo.JOB_CODE = newClass;

		fp = fopen(szFile, "wb");

		if (fp)
		{
			fwrite(&TransRecordData, sizeof(TRANS_RECORD_DATA), 1, fp);
			fclose(fp);
		}
	}


}

void changePlayerName(char ID[32], char sOldName[32], char newName[32])
{
	char	szFile[256] = { 0 };
	std::string newFile;
	std::string oldName;

	oldName = sOldName;

	sPLAY_USER_DATA		sPlayUserData;
	TRANS_RECORD_DATA	TransRecordData;

	ZeroMemory(&sPlayUserData, sizeof(sPLAY_USER_DATA));
	ZeroMemory(&TransRecordData, sizeof(TRANS_RECORD_DATA));

	GetUserInfoFile(ID, szFile);
	auto fp = fopen(szFile, "rb");

	if (!fp)
		return;

	fread(&sPlayUserData, sizeof(sPLAY_USER_DATA), 1, fp);
	fclose(fp);

	// Salva o novo nick no .dat da conta
	int pos = 0;
	while (true)
	{
		if (boost::iequals(sPlayUserData.szCharName[pos], sOldName))
		{
			sprintf(sPlayUserData.szCharName[pos], newName);

			fp = fopen(szFile, "wb");
			if (!fp) return;
			fwrite(&sPlayUserData, sizeof(sPLAY_USER_DATA), 1, fp);
			fclose(fp);

			break;
		}
		pos++;

		if (pos > 5)
			break;
	}

	GetUserDataFile(sOldName, szFile);
	newFile = szFile;
	replace(newFile, oldName.c_str(), newName);
	auto r = rename(szFile, newFile.c_str());

	// Salva o novo nick no .dat do char
	fp = fopen(newFile.c_str(), "rb");

	if (fp)
	{
		fread(&TransRecordData, sizeof(TRANS_RECORD_DATA), 1, fp);
		fclose(fp);

		sprintf(TransRecordData.smCharInfo.szName, newName);

		fp = fopen(newFile.c_str(), "wb");

		if (fp)
		{
			fwrite(&TransRecordData, sizeof(TRANS_RECORD_DATA), 1, fp);
			fclose(fp);
		}
	}

	// Move o .dat do char para a pasta certa
	ZeroMemory(&szFile, sizeof(szFile));
	GetUserDataFile(TransRecordData.smCharInfo.szName, szFile);
	r = rename(newFile.c_str(), szFile);
}

std::string replace_substrings(std::string str,
	const std::string& original_substring,
	const std::string& new_substring)
{
	auto pos = str.find(original_substring);

	while (pos != std::string::npos)
	{
		str.replace(pos, original_substring.size(), new_substring);
		pos = str.find(original_substring, pos + new_substring.size());
	}

	return str;
}

static char* GetWord(char* q, char* p)
{
	while ((*p == 32) || (*p == 9))
	{
		p++;
	}

	while ((*p != 32) && (*p != 9))
	{
		if (*p == '\n' || *p == '\r') break;
		*q++ = *p++;
	}
	*q++ = 0;

	return p;
}

#include <regex>
#include <Chat/ChatServer.h>

void replacePostBoxName(std::string oldName, std::string newName, char ID[32])
{
	char	szFileName[64] = { 0 };

	GetPostBoxFile(ID, szFileName);

	auto fp = fopen(szFileName, "rb");

	if (!fp)
		return;

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	unsigned long dwAcess;

	char* buffer = new char[size];
	rewind(fp);

	fread(buffer, sizeof(char), size, fp);
	fclose(fp);

	remove(szFileName);

	auto hFile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}

	auto newBuffer = std::regex_replace(buffer, std::regex(oldName.c_str()), newName.c_str());
	newBuffer += "\r\n";

	int FileLength = GetFileSize(hFile, NULL);
	SetFilePointer(hFile, FileLength, NULL, FILE_BEGIN);

	WriteFile(hFile, newBuffer.c_str(), lstrlen(newBuffer.c_str()), &dwAcess, NULL);

	CloseHandle(hFile);

	delete[] buffer;
}
void NewShop::ChangeClass(rsPLAYINFO* Player, sFinishPurchase* Item)
{
	if (Item->Item.SubCategoryID != 102)
		return;

	int preco = setChangeClassPrice();
	int userCoin = GetPlayerCoin(Player);

	if (userCoin < preco)
	{
		UTILS->Alert(Player, "> Você não possui saldo suficiente!");
		return;
	}

	// Remove os coins do player
	removePlayerCoins(Player, preco);
	UTILS->Alert(Player, "> Mudança Concluída, desconectando...");

	// Atualiza a classe na tabela de BPS
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE UserInfo SET Class=? WHERE Name=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&Item->Spec, 1, PARAMTYPE_Integer);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}

	db = SQLConnection::GetConnection(DATABASEID_LogDB);

	if (db && db->Open())
	{
		const char* const query = "INSERT INTO ChangeClass VALUES (?,?,?,GETDATE(),?)";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);
			db->BindInputParameter(&Player->smCharInfo.JOB_CODE, 2, PARAMTYPE_Integer);
			db->BindInputParameter(&Item->Spec, 3, PARAMTYPE_Integer);
			db->BindInputParameter(&preco, 4, PARAMTYPE_Integer);
			db->Execute();
		}

		db->Close();
	}

	char playerName[32] = { 0 };
	sprintf(playerName, Player->szName);

	DisconnectUser(Player->lpsmSock);

	changePlayerClass(playerName, Item->Spec);
}

void NewShop::ChangeNick(rsPLAYINFO* Player, sFinishPurchase* Item)
{
	if (Item->Item.SubCategoryID != 101)
		return;

	if (!checkNick(Player, Item))
		return;

	int preco = setChangeNickPrice();
	int userCoin = GetPlayerCoin(Player);

	if (userCoin < preco)
	{
		UTILS->Alert(Player, "> Você não possui saldo suficiente!");
		return;
	}

	// Remove os coins do player
	removePlayerCoins(Player, preco);
	UTILS->Alert(Player, "> Mudança Concluída, desconectando...");

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE UserInfo SET Name=? WHERE Name=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		const char* const query2 = "UPDATE Caravans SET Name=? WHERE Name=?";

		if (db->Prepare(query2))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}

	db = SQLConnection::GetConnection(DATABASEID_Quest);

	if (db && db->Open())
	{
		const char* const query = "UPDATE PlayerActiveQuest SET PlayerName=? WHERE PlayerName=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		const char* const query2 = "UPDATE PlayerCompletedQuest SET PlayerName=? WHERE PlayerName=?";

		if (db->Prepare(query2))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}

	db = SQLConnection::GetConnection(DATABASEID_SoDDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE SodRecord SET Name=? WHERE Name=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}

	db = SQLConnection::GetConnection(DATABASEID_ClanDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE CL SET ClanZang=? WHERE ClanZang=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		const char* const query2 = "UPDATE UL SET ChName=? WHERE ChName=?";

		if (db->Prepare(query2))
		{
			db->BindInputParameter(Item->Item.ItemName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}

	// Log da mudança

	db = SQLConnection::GetConnection(DATABASEID_LogDB);

	if (db && db->Open())
	{
		const char* const query = "INSERT INTO ChangeNick VALUES (?,?,?,GETDATE(),?)";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->BindInputParameter(Item->Item.ItemName, 3, PARAMTYPE_String);
			db->BindInputParameter(&preco, 4, PARAMTYPE_Integer);
			db->Execute();
		}

		db->Close();
	}

	char playerID[32] = { 0 };
	char playerName[32] = { 0 };

	sprintf(playerID, Player->szID);
	sprintf(playerName, Player->szName);
	DisconnectUser(Player->lpsmSock);

	changePlayerName(playerID, playerName, Item->Item.ItemName);
	replacePostBoxName(playerName, Item->Item.ItemName, playerID);
}

void NewShop::confirmPaypalDonation()
{
	auto db = SQLConnection::GetConnection(DATABASEID_ShopCoin);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM StoreSettings";
		const char* const queryPeding = "SELECT * FROM PendingDonations WHERE Approved=1";


		char szID[32] = { 0 };
		char nome[32] = { 0 };
		char IP[32] = { 0 };
		char IDPaypal[32] = { 0 };
		char PaymentID[64] = { 0 };
		char PayerID[32] = { 0 };
		char szDateTime[20] = { 0 };
		int isApproved = 0;
		int Value = 0;
		int Amount = 0;


		if (db->Prepare(query))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &isDoubleDonation);
			}
		}

		if (db->Prepare(queryPeding))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_String, szID, sizeof(szID));
				db->GetData(2, PARAMTYPE_String, nome, sizeof(nome));
				db->GetData(3, PARAMTYPE_String, IP, sizeof(IP));
				db->GetData(4, PARAMTYPE_Integer, &Value);
				db->GetData(5, PARAMTYPE_Integer, &Amount);
				db->GetData(6, PARAMTYPE_String, szDateTime, sizeof(szDateTime));
				db->GetData(7, PARAMTYPE_String, IDPaypal, sizeof(IDPaypal));
				db->GetData(8, PARAMTYPE_String, PaymentID, sizeof(PaymentID));
				db->GetData(9, PARAMTYPE_String, PayerID, sizeof(PayerID));
				db->GetData(10, PARAMTYPE_Integer, &isApproved);

				if (isDoubleDonation > 1) Amount *= isDoubleDonation;

				GetDateTime(szDateTime);


				if (isApproved == 1 && Amount > 0)
				{
					const char* const queryConfirmed = "INSERT INTO ConfirmedDonations (ID,Name,IP,Value,Amount,DateConfirmed,IDPaypal,PaymentID,PayerID) values (?,?,?,?,?,GETDATE(),?,?,?)";
					const char* const queryDeletePending = "DELETE FROM PendingDonations WHERE IDPaypal=?";


					if (db->Prepare(queryConfirmed))
					{
						db->BindInputParameter(szID, 1, PARAMTYPE_String);
						db->BindInputParameter(nome, 2, PARAMTYPE_String);
						db->BindInputParameter(IP, 3, PARAMTYPE_String);
						db->BindInputParameter(&Value, 4, PARAMTYPE_Integer);
						db->BindInputParameter(&Amount, 5, PARAMTYPE_Integer);
						db->BindInputParameter(IDPaypal, 6, PARAMTYPE_String);
						db->BindInputParameter(PaymentID, 7, PARAMTYPE_String);
						db->BindInputParameter(PayerID, 8, PARAMTYPE_String);
						db->Execute();

					}

					if (db->Prepare(queryDeletePending))
					{
						db->BindInputParameter(IDPaypal, 1, PARAMTYPE_String);
						db->Execute();
					}

					rsPLAYINFO* Player = nullptr;
					Player = FindUserFromID(szID);
					addCoinsToPlayer(Player, szID, Amount, 1, WhereCoinsComeFrom::FROM_DONATION);
				}

			}
		}

		db->Close();
	}
}

void NewShop::removePlayerCoins(rsPLAYINFO* Player, int coinsToRemove)
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE Users SET UserCoin-=? WHERE username=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&coinsToRemove, 1, PARAMTYPE_Integer);
			db->BindInputParameter(Player->szID, 2, PARAMTYPE_String);
			db->Execute();
		}

		db->Close();
	}
}

void NewShop::SendItems(rsPLAYINFO* Player)
{
	NEWSHOP_COMPRESSEDPCKG pckgToSend;
	ZeroMemory(&pckgToSend, sizeof(NEWSHOP_COMPRESSEDPCKG));
	pckgToSend.size = sizeof(NEWSHOP_COMPRESSEDPCKG);
	pckgToSend.code = NewShopItems_ReceiveItems;

	ITEMS_INFOCKG pckg;
	ZeroMemory(&pckg, sizeof(ITEMS_INFOCKG));

	int i = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_ShopCoin);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM ShopItems";

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

	if (!isDoubleDonation)
		SERVERCHAT->SendChat(Player, CHATCOLOR_Global, "> A compra de Coins é feita pelo nosso Painel!");
	else
	{
		SERVERCHAT->SendChatEx(Player, CHATCOLOR_Global, "Evento x%d ativo!, você vai receber %dx a quantidade de coins mostrada!", isDoubleDonation, isDoubleDonation);
	}

}

void NewShop::addCoinsToPlayer(rsPLAYINFO* gm, char* id, int coins, int isFromDonation, WhereCoinsComeFrom WhereComeFrom) {


	char szQuery[512] = { 0 };
	char msg[128] = { 0 };
	int UserCoin = 0;
	rsPLAYINFO* Player = nullptr;



	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT UserCoin FROM Users WHERE username=?";
		const char* const queryUpdate = "UPDATE Users SET UserCoin=? WHERE username=?";


		if (db->Prepare(query))
		{
			db->BindInputParameter(id, 1, PARAMTYPE_String);

			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &UserCoin);
			}

			UserCoin += coins;

			if (db->Prepare(queryUpdate))
			{
				db->BindInputParameter(&UserCoin, 1, PARAMTYPE_Integer);
				db->BindInputParameter(id, 2, PARAMTYPE_String);
				db->Execute();
			}

			Player = FindUserFromID(id);

			if (Player != NULL && coins > 0)
			{
				SendCoinToGame(Player);
				char msg[64] = { 0 };
				sprintf_s(msg, sizeof(msg), "> Você recebeu %d Coins, novo saldo %d Coins", coins, UserCoin);
				UTILS->Alert(Player, msg);

			}

			if (!isFromDonation) {
				sprintf_s(msg, sizeof(msg), "ID: %s recebeu %d Coins - Novo saldo %d Coins", id, coins, UserCoin);
				UTILS->Alert(gm, msg);
			}
		}
		else
		{
			if (!isFromDonation)
				UTILS->Alert(gm, "ID não localizada");
		}


		db->Close();
	}

	char TempId[32] = { 0 };
	char TempName[32] = { 0 };

	if (gm != nullptr && Player != nullptr)
	{
		strcpy_s(TempId, gm->szID);
		strcpy_s(TempName, Player->szName);
	}
	else
	{
		if (gm != nullptr)
		{
			strcpy_s(TempId, gm->szID);
			strcpy_s(TempName, "Desconhecido");
		}
		else
		{
			strcpy_s(TempId, "Desconhecido");
			strcpy_s(TempName, "Desconhecido");
		}
	}

	if (coins > 0)
	{
		auto db2 = SQLConnection::GetConnection(DATABASEID_LogDB);

		if (db2 && db2->Open())
		{
			const char* const queryInsert = "INSERT INTO RecvCoins (GMEnviou,IDRecebeu,NickRecebeu,Quantidade,Motivo,Data) values(?,?,?,?,?,GETDATE())";

			if (db2->Prepare(queryInsert))
			{
				db2->BindInputParameter(TempId, 1, PARAMTYPE_String);
				db2->BindInputParameter(id, 2, PARAMTYPE_String);
				db2->BindInputParameter(TempName, 3, PARAMTYPE_String);
				db2->BindInputParameter(&coins, 4, PARAMTYPE_Integer);
				db2->BindInputParameter(&WhereComeFrom, 5, PARAMTYPE_Integer);
				db2->Execute();
			}

			db2->Close();
		}
	}


}

void NewShop::FinishPurchase(rsPLAYINFO* Player, sFinishPurchase* Item)
{
	if (Item->Item.SubCategoryID == 100 && Player->vipLevel > 0)
	{
		UTILS->Alert(Player, "> Você já possui VIP!");
		UTILS->Alert(Player, "> Não é possível renovar antes do término");
		return;
	}

	int PlayerCoins = GetPlayerCoin(Player);
	int Price = (Item->Item.Price * Item->Quantity);
	psITEM* lpsItem;
	lpsItem = new psITEM;

	if (PlayerCoins >= Price)
	{
		PlayerCoins -= Price;
		removePlayerCoins(Player, Price);

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

			SERVERCHAT->SendChatEx(Player, EChatColor::CHATCOLOR_Global, "> Item %s (x%d) comprado, Você agora possui %d Coins restantes", Item->Item.ItemName, Item->Quantity, PlayerCoins);
			SendCoinToGame(Player);

			// Log da compra
			HANDLE hFile;
			char szFileName[255];
			char szLog[500];
			SYSTEMTIME	SystemTime;

			GetLocalTime(&SystemTime);
			sprintf(szFileName, "Logs\\SHOP_LOG\\%d-%d.log", SystemTime.wMonth, SystemTime.wDay);
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
		SERVERCHAT->SendChat(Player, EChatColor::CHATCOLOR_Error, "Você não possui Coins suficientes.");
	}
}

void NewShop::SendCoinToGame(rsPLAYINFO* Player)
{
	if (Player && Player->lpsmSock)
	{
		int PlayerCoins = GetPlayerCoin(Player);

		UTILS->Packet(Player, PACKET_SEND_COIN_GAME, 0, PlayerCoins);
	}
}
