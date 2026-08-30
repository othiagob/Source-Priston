#include "RestaureItem.h"
#include "smPacket.h"
#include "Database\SQLConnection.h"
#include "Shop\\NewShop.h"
#include "GM\\GM.h"

FailedItemsPckg failedItems = {0};
FailedItems getItems = { 0 };

extern int CreateCommandItem(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, char* szItem);

RestaureItem::RestaureItem()
{
}

RestaureItem::~RestaureItem()
{
}

int setPrice()
{
	auto db = SQLConnection::GetConnection(DATABASEID_ServerDB);

	int price = 0;

	if (db->Open())
	{
		if (db->Prepare("SELECT Price FROM Restaure"))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &price);

				if (price)
					return price;
			}
		}
		db->Close();
	}

	return 1000; // Padrão
}

INT RestaureItem::RecoverItem(rsPLAYINFO* Player, int ItemHead, int ItemChkSum)
{
	int price = setPrice();

	if (NewShop::GetInstance()->GetPlayerCoin(Player) >= price)
	{
		char codeItem[64] = { 0 };
		wsprintf(codeItem, "@%d@%d", ItemHead, ItemChkSum);
		CreateCommandItem(Player, Player, codeItem);

		auto db = SQLConnection::GetConnection(DATABASEID_LogDB);

		if (db->Open())
		{
			const char* const query = "DELETE FROM AgingFailed WHERE ItemHead=? AND ItemChkSum=?";

			if (db->Prepare(query))
			{
				db->BindInputParameter(&ItemHead, 1, PARAMTYPE_Integer);
				db->BindInputParameter(&ItemChkSum, 2, PARAMTYPE_Integer);
				db->Execute();
			}

			db->Close();
		}

		sendFailedItems(Player);

		NewShop::GetInstance()->removePlayerCoins(Player, price);
		GameMasters::getInstance()->Alert(Player, "> Item restaurado com sucesso!");
	}
	else 
	{
		GameMasters::getInstance()->Alert(Player, "> Você não possui coins suficientes!");
	}

	return TRUE;
}

INT RestaureItem::sendFailedItems(rsPLAYINFO* Player)
{
	auto db = SQLConnection::GetConnection(DATABASEID_LogDB);

	ZeroMemory(&failedItems, sizeof(FailedItemsPckg));
	failedItems.size = sizeof(FailedItemsPckg);
	failedItems.code = GET_FAILED_ITEMS;
	failedItems.price = setPrice();

	if (db->Open())
	{
		if (db->Prepare("SELECT ItemName,AgingNum,ItemHead,ItemChkSum,Date,ItemCode FROM AgingFailed WHERE ID=?"))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);

			if (db->Execute(false))
			{
				int i = 0;

				while (db->NextRow())
				{
					db->GetData(1, PARAMTYPE_String, failedItems.FailedItems[i].ItemName, sizeof(failedItems.FailedItems[i].ItemName));
					db->GetData(2, PARAMTYPE_Integer, &failedItems.FailedItems[i].AgingNum);
					db->GetData(3, PARAMTYPE_Integer, &failedItems.FailedItems[i].ItemHead);
					db->GetData(4, PARAMTYPE_Integer, &failedItems.FailedItems[i].ItemChkSum);
					db->GetData(5, PARAMTYPE_Time, &failedItems.FailedItems[i].Date, 0);
					db->GetData(6, PARAMTYPE_String, failedItems.FailedItems[i].ItemCode, sizeof(failedItems.FailedItems[i].ItemCode));

					i++;
					if (i >= 20) break;
				}
			}
		}
		db->Close();
	}

	Player->lpsmSock->Send((char*)&failedItems, failedItems.size, TRUE);

	return TRUE;
}

