#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\Database\SQLConnection.h"
#include <boost/algorithm/string.hpp>
#include <Chat/ChatServer.h>

void SodRanking::ResetRanking() {

	auto db = SQLConnection::GetConnection(DATABASEID_ClanDB);

	if (db->Open())
	{
		if (db->Prepare("UPDATE CL SET Cpoint=0"))
			db->Execute();

		db->Close();
	}


	db = SQLConnection::GetConnection(DATABASEID_SoDDB);

	if (db->Open())
	{
		if (db->Prepare("DELETE FROM SodRecord"))
			db->Execute();

		db->Close();
	}

	SERVERCHAT->SendChatAll(EChatColor::CHATCOLOR_Global, "Bellatra> Os pontos foram resetados!");
}

RankingSod getClanScoreDetails(int sodScore, RankingSod sRanking, int i)
{
	auto db = SQLConnection::GetConnection(DATABASEID_SoDDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM SodRecord WHERE SodTotalUser > 1 AND SodTotalScore=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&sodScore, 1, PARAMTYPE_Integer);

			db->Execute(FALSE);

			int monstrosMortos = 0;
			bool first = true;

			sRanking.sodKillCount = 0;

			while (db->NextRow())
			{
				db->GetData(5, PARAMTYPE_Time, &sRanking.Date, sizeof(SYSTEMTIME));
				db->GetData(8, PARAMTYPE_Integer, &monstrosMortos);
				db->GetData(9, PARAMTYPE_Integer, &sRanking.sodTotalScore);
				db->GetData(11, PARAMTYPE_Integer, &sRanking.sodTotalUser);

				sRanking.sodKillCount += monstrosMortos;
			}
		}

		db->Close();
	}

	return sRanking;
}

INT SodRanking::SendSodRanking(rsPLAYINFO* Player)
{
	RankingSodPckg* sRanking = new RankingSodPckg{ 0 };

	sRanking->code = OPEN_SODWINDOW_NPC;
	sRanking->size = sizeof(RankingSodPckg);

	auto db = SQLConnection::GetConnection(DATABASEID_SoDDB);

	int i = 0;

	if (db && db->Open())
	{
		const char* const query = "SELECT TOP 10 * FROM SodRecord WHERE SodTotalUser=1 ORDER BY SodTotalScore DESC";

		if (db->Prepare(query))
		{
			db->Execute(FALSE);

			while (db->NextRow())
			{
				db->GetData(2, PARAMTYPE_String, sRanking->AllRanking[i].szName, sizeof(sRanking->AllRanking[i].szName));
				db->GetData(3, PARAMTYPE_Integer, &sRanking->AllRanking[i].level);
				db->GetData(5, PARAMTYPE_Time, &sRanking->AllRanking[i].Date, sizeof(SYSTEMTIME));
				db->GetData(6, PARAMTYPE_Integer, &sRanking->AllRanking[i].jobCode);
				db->GetData(7, PARAMTYPE_Integer, &sRanking->AllRanking[i].sodScore);
				db->GetData(8, PARAMTYPE_Integer, &sRanking->AllRanking[i].sodKillCount);
				db->GetData(9, PARAMTYPE_Integer, &sRanking->AllRanking[i].sodTotalScore);
				db->GetData(10, PARAMTYPE_Integer, &sRanking->AllRanking[i].sodScore);

				i++;
			}
		}

		// Pega o imposto arrecadado
		const char* const queryImpost = "SELECT * FROM Imposto";

		if (db->Prepare(queryImpost))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Float, &sRanking->impostoArrecadado);
			}
		}

		db->Close();
	}

	db = SQLCONNECTION(DATABASEID_ClanDB);

	bool top1 = true;

	char clanZang[32] = { 0 };

	if (db && db->Open())
	{
		const char* const query = "SELECT TOP 10 * FROM CL ORDER BY Cpoint DESC";

		if (db->Prepare(query))
		{
			db->Execute(FALSE);

			while (db->NextRow())
			{
				db->GetData(2, PARAMTYPE_String, sRanking->AllRanking[i].szName, sizeof(sRanking->AllRanking[i].szName));
				db->GetData(6, PARAMTYPE_String, clanZang, sizeof(clanZang));
				db->GetData(9, PARAMTYPE_Integer, &sRanking->AllRanking[i].clanCode);
				db->GetData(15, PARAMTYPE_Integer, &sRanking->AllRanking[i].sodTotalScore);

				if (top1)
				{
					sprintf_s(sRanking->clanZang, sizeof(sRanking->clanZang), "%s", clanZang);
					top1 = false;
				}

				sRanking->AllRanking[i] = getClanScoreDetails(sRanking->AllRanking[i].sodTotalScore, sRanking->AllRanking[i], i);
				sRanking->AllRanking[i].Date.wYear = 2022;
				i++;
			}
		}

		db->Close();
	}

	if (Player && Player->lpsmSock)
		Player->lpsmSock->Send((char*)sRanking, sRanking->size, TRUE);

	if (sRanking)
		delete sRanking;

	return TRUE;
}

INT SodRanking::RetrieveSodMoney(rsPLAYINFO* Player)
{
	auto db = SQLConnection::GetConnection(DATABASEID_ClanDB);

	char liderClan[32] = { 0 };

	if (db && db->Open())
	{
		const char* const query = "SELECT TOP 1 * FROM CL ORDER BY CPoint DESC";

		if (db->Prepare(query))
		{
			if (db->Execute())
			{
				db->GetData(6, PARAMTYPE_String, liderClan, sizeof(liderClan));
			}
		}

		db->Close();
	}

	// Verifica se o player é o lider do clan top 1 de pontuações de sod
	if (boost::iequals(Player->szName, liderClan))
	{
		float imposto = 0;

		db = SQLCONNECTION(DATABASEID_SoDDB);

		if (db && db->Open())
		{
			const char* const query = "SELECT * FROM Imposto";

			if (db->Prepare(query))
			{
				if (db->Execute())
				{
					db->GetData(1, PARAMTYPE_Float, &imposto);
				}
			}

			db->Close();
		}

		if (imposto > 0)
		{
			if (db && db->Open())
			{
				const char* const query = "UPDATE Imposto SET Imposto=0";

				if (db->Prepare(query))
				{
					db->Execute();
				}

				db->Close();
			}

			Player->AddServerMoney(imposto, 220);
			GameMasters::getInstance()->Packet(Player, RETRIEVE_SOD_MONEY, imposto);
			GameMasters::getInstance()->Alert(Player, "> Imposto resgatado com sucesso!");
		}
		else
		{
			GameMasters::getInstance()->Alert(Player, "> Nenhum imposto arrecadado!");
		}
	}

	return TRUE;
}