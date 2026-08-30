#include "..\\sinbaram\\sinLinkHeader.h"
#include "TopRanking.h"
#include "..\\Database\SQLConnection.h"
#include <boost/algorithm/string.hpp>


TopPlayerPckg topPlayers;

void TopRanking::sendTopPlayers(rsPLAYINFO* Player)
{

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (update)
	{
		ZeroMemory(&topPlayers, sizeof(TopPlayerPckg));
		topPlayers.size = sizeof(TopPlayerPckg);
		topPlayers.code = OPEN_RANKING_NPC;

		char nick[32] = { 0 };
		int classe = 0;
		int level = 0;
		int bps = 0;
		int cnt = 0;

		for (int x = 1; x < 9; x++)
		{
			if (db->Open())
			{
				if (db->Prepare("SELECT TOP 10 Name, Lvl, Kills, Class FROM UserInfo WHERE Class=? AND Name != ? AND Name != ? AND Name != ? AND Name != ? ORDER BY Kills DESC"))
				{
					db->BindInputParameter(&x, 1, PARAMTYPE_Integer);
					db->BindInputParameter("Maldade", 2, PARAMTYPE_String);
					db->BindInputParameter("Major", 3, PARAMTYPE_String);
					db->BindInputParameter("DyaBlos", 5, PARAMTYPE_String);

					if (db->Execute())
					{
						cnt = 0;

						while (true)
						{
							db->GetData(1, PARAMTYPE_String, topPlayers.Players[cnt].playerName, sizeof(topPlayers.Players[cnt].playerName));
							db->GetData(2, PARAMTYPE_Integer, &topPlayers.Players[cnt].playerLevel);
							db->GetData(3, PARAMTYPE_Integer, &topPlayers.Players[cnt].playerBps);
							db->GetData(4, PARAMTYPE_Integer, &topPlayers.Players[cnt].playerClass);

							cnt++;

							if (!db->NextRow())
								break;
						}
					}

				}

				db->Close();
			}
		}

		for (int x = 1; x < 9; x++)
		{
			if (db->Open())
			{
				if (db->Prepare("SELECT TOP 10 Name, Lvl, Kills, Class FROM UserInfo WHERE Class=? AND Name != ? AND Name != ? AND Name != ? AND Name != ? ORDER BY Lvl DESC, Exp DESC"))
				{
					db->BindInputParameter(&x, 1, PARAMTYPE_Integer);
					db->BindInputParameter("Maldade", 2, PARAMTYPE_String);
					db->BindInputParameter("Major", 3, PARAMTYPE_String);
					db->BindInputParameter("DyaBlos", 5, PARAMTYPE_String);

					if (db->Execute())
					{
						while (true)
						{
							int z = 0;
							bool found = false;

							db->GetData(1, PARAMTYPE_String, nick, sizeof(nick));

							for (z = 0; z < cnt; z++)
							{
								if (boost::iequals(topPlayers.Players[z].playerName, nick))
								{
									found = true;
									break;
								}
							}

							if (!found)
							{
								cnt++;
								sprintf(topPlayers.Players[cnt].playerName, nick);

								db->GetData(2, PARAMTYPE_Integer, &topPlayers.Players[cnt].playerLevel);
								db->GetData(3, PARAMTYPE_Integer, &topPlayers.Players[cnt].playerBps);
								db->GetData(4, PARAMTYPE_Integer, &topPlayers.Players[cnt].playerClass);
							}


							if (!db->NextRow())
								break;
						}
					}

				}

				db->Close();
			}
		}

		update = false;
	}

	Player->lpsmSock->Send((char*)&topPlayers, topPlayers.size, TRUE);
}

INT64 getPlayerExp(rsPLAYINFO* Player)
{
	INT64   sinExp = 0;
	sinExp = GetXorExp64(&Player->smCharInfo);

	return sinExp;
}

INT64 getPlayerPercent(rsPLAYINFO* Player)
{
	INT64 ExpSize = GetNextExp(Player->smCharInfo.Level) - GetNextExp(Player->smCharInfo.Level - 1);
	INT64 ExpNowSize = getPlayerExp(Player) - GetNextExp(Player->smCharInfo.Level - 1);
	INT64 Percent = (100000.0f / (float)ExpSize * (float)ExpNowSize);
	Percent = Percent / 1000;

	if (Percent >= 100)
		Percent = 99;

	if (Percent < 0)
		Percent = 0;

	return Percent;
}

void TopRanking::LevelUp(rsPLAYINFO* Player)
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE UserInfo SET Lvl=?, Exp=?, Class=?, DateUp=GETDATE() Where Name=?";

		if (db->Prepare(query))
		{
			INT64 PlayeEXP = getPlayerPercent(Player);

			db->BindInputParameter(&Player->smCharInfo.Level, 1, PARAMTYPE_Integer);
			db->BindInputParameter(&PlayeEXP, 2, PARAMTYPE_Int64);
			db->BindInputParameter(&Player->smCharInfo.JOB_CODE, 3, PARAMTYPE_Integer);
			db->BindInputParameter(Player->smCharInfo.szName, 4, PARAMTYPE_String);

			db->Execute();
		}

		db->Close();
	}
}

void TopRanking::UpdateExp(rsPLAYINFO* Player)
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE UserInfo SET Exp=? Where Name=?";

		if (db->Prepare(query))
		{
			INT64 PlayeEXP = getPlayerPercent(Player);

			db->BindInputParameter(&PlayeEXP, 1, PARAMTYPE_Int64);
			db->BindInputParameter(Player->smCharInfo.szName, 2, PARAMTYPE_String);

			db->Execute();
		}

		db->Close();
	}
}


