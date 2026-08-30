#include <boost/algorithm/string.hpp>
#include <zlib.h>
#include <iostream>
#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\Database\SQLConnection.h"
#include "../Shop/NewShop.h"
#include "Quest.h"
#include "../SrcServer/onserver.h"

QUESTPCKG questPckg = {};
int nQuestAtivas = 0;

void updateSQL(rsPLAYINFO* Player, smCHAR* Monster, bool PartyFlag, int questID, int y)
{
	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	if (db && db->Open())
	{
		int progress[3] = { 0 };

		const char* const query = FormatString("UPDATE PlayerActiveQuest SET questProgress%d += 1 WHERE questID = ? AND PlayerName = ?", y);

		// Atualiza o status da quest
		if (db->Prepare(query))
		{
			db->BindInputParameter(&questID, 1, PARAMTYPE_Integer);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->Execute();
		}

		if (db->Prepare("SELECT questProgress1, questProgress2, questProgress3 FROM PlayerActiveQuest WHERE questID=? AND PlayerName=?"))
		{
			db->BindInputParameter(&questID, 1, PARAMTYPE_Integer);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);

			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &progress[0]);
				db->GetData(2, PARAMTYPE_Integer, &progress[1]);
				db->GetData(3, PARAMTYPE_Integer, &progress[2]);
			}
		}

		// Encontra a quest e verifica se está completa
		for (int x = 0; x <= 60; x++)
		{
			if (questID == questPckg.QuestPckg[x].questID)
			{
				if ((progress[0] >= questPckg.QuestPckg[x].monsterNum[0]) &&
					(progress[1] >= questPckg.QuestPckg[x].monsterNum[1]) &&
					(progress[2] >= questPckg.QuestPckg[x].monsterNum[2]))
				{
					GameMasters::getInstance()->Alert(Player, "> Desafio Concluído!");
					GameMasters::getInstance()->Packet(Player, 0x50600008, questID, progress[0], progress[1], progress[2]);
				}

				break;
			}
		}

		db->Close();
	}
}

int Quest::updatePVPKill(rsPLAYINFO* Player)
{
	int pvpMap = 0, questID = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	if (db->Open())
	{
		// Atualiza o status da quest
		if (db->Prepare("SELECT questID, pvpMAP FROM PlayerActiveQuest WHERE questObjective=1 AND PlayerName=?"))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->Execute(FALSE);

			while (db->NextRow())
			{
				db->GetData(1, PARAMTYPE_Integer, &questID);
				db->GetData(2, PARAMTYPE_Integer, &pvpMap);

				if (db->Prepare("UPDATE PlayerActiveQuest SET playerKilled +=1 WHERE questID=? AND PlayerName=?"))
				{
					db->BindInputParameter(&questID, 1, PARAMTYPE_Integer);
					db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);

					if (pvpMap)
					{
						if (Player->Position.Area == pvpMap)
						{
							db->Execute();
							GameMasters::getInstance()->Packet(Player, 0x50600010, questID);
						}
					}
					else
					{
						db->Execute();
						GameMasters::getInstance()->Packet(Player, 0x50600010, questID);
					}
				}
			}
		}

		db->Close();
	}

	return TRUE;
}

int Quest::updateStatus(rsPLAYINFO* Player, smCHAR* Monster, int PartyFlag) {
	int x = 0, y = 0, z = 0, isSpecificMap = 0, killToGo = 0, flag = 0;
	char monsterName[32] = { 0 };

	getStatus(Player, false);

	for (x = 0; x <= 60; x++)
	{
		if (Player->questInfoPckg.QuestPckg[x + 1].isActive)
		{
			if (Player->questInfoPckg.QuestPckg[x + 1].questType == QUEST_UNIQUE_PARTY || 
				Player->questInfoPckg.QuestPckg[x + 1].questType == QUEST_DAILY_PARTY || 
				Player->questInfoPckg.QuestPckg[x + 1].questType == QUEST_REPEAT_PARTY) {
				if (!PartyFlag) goto skip;
			}

			if (Player->questInfoPckg.QuestPckg[x + 1].questType == QUEST_UNIQUE_SOLO || 
				Player->questInfoPckg.QuestPckg[x + 1].questType == QUEST_DAILY_SOLO || 
				Player->questInfoPckg.QuestPckg[x + 1].questType == QUEST_REPEAT_SOLO) {
				if (PartyFlag) goto skip;
			}

			for (z = 0; z <= 2; z++)
			{
				if (boost::iequals(Monster->smCharInfo.szName, Player->updatequest[x + 1].wantedMonster[y]) || Player->questInfoPckg.QuestPckg[x + 1].questObjective == QUEST_KILL_ANY_MONSTER)
				{
					isSpecificMap = 0;

					//if (lstrcmpi(Monster->smCharInfo.szName, updatequest[x + 1].wantedMonster[y]) == 0) {
					memset(&monsterName, 0, sizeof(monsterName));
					sprintf_s(monsterName, sizeof(monsterName), Monster->smCharInfo.szName);

					killToGo = Player->updatequest[x + 1].monsterNum[y];

					if (Player->updatequest[x + 1].monsterMap[y] > 0)
						isSpecificMap = Player->updatequest[x + 1].monsterMap[y];

					if (Player->questInfoPckg.QuestPckg[x + 1].questProgress[y] < killToGo)
					{
						if (isSpecificMap)
						{
							if (monsterName[0] && Player->Position.Area == isSpecificMap)
								flag = 1;

						}
						else if (!isSpecificMap && monsterName[0]) {
							flag = 1;
						}

						if (flag)
						{
							updateSQL(Player, Monster, false, Player->questInfoPckg.QuestPckg[x + 1].questID, y + 1);
							y = 0;
						}

					}
				}
				else if (!Player->updatequest[x + 1].wantedMonster[y] && Player->updatequest[x + 1].monsterMap[y] > 0)
				{
					killToGo = Player->updatequest[x + 1].monsterNum[y];

					if (Player->Position.Area == Player->updatequest[x + 1].monsterMap[y])
					{
						updateSQL(Player, Monster, false, Player->questInfoPckg.QuestPckg[x + 1].questID, y + 1);
					}

					y = 0;
				}
				y++;
				flag = 0;
			}
		}
	skip:
		flag = 0;
		y = 0;
	}

	return TRUE;
}

void Quest::getStatus(rsPLAYINFO* Player, bool Send)
{
	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	int questObjective = -1;
	int kill = 0;
	int killToGo = 0;
	int progress[3] = { 0 };
	int progresstoGo[3] = { 0 };

	int result = 0;
	int i = 0;
	int questId = 0;

	ZeroMemory(Player->updatequest, sizeof(updateQuest));
	ZeroMemory(&Player->questInfoPckg, sizeof(QUEST_INFOPCKG));

	Player->questInfoPckg.size = sizeof(QUEST_INFOPCKG);
	Player->questInfoPckg.code = smTRANSCODE_GET_QUEST_STATUS;

	std::vector<int> vIDs = vector<int>();

	if (Send)
	{
		if (db->Open())
		{
			const char* const query = "DELETE FROM PlayerCompletedQuest WHERE questType > 1 AND questType != 4 AND PlayerName=? AND DateCompleted <= (GETDATE() - 1)";

			if (db->Prepare(query))
			{
				db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
				db->Execute();
			}

			db->Close();
		}

		if (db->Open())
		{
			const char* const query = "SELECT questID, DateCompleted FROM PlayerCompletedQuest WHERE PlayerName=?";

			if (db->Prepare(query))
			{
				db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);

				if (db->Execute(false))
				{
					while (db->NextRow())
					{
						db->GetData(1, PARAMTYPE_Integer, &questId);
						db->GetData(2, PARAMTYPE_Time, &Player->questInfoPckg.QuestPckg[questId].EndTime, sizeof(SYSTEMTIME));

						Player->questInfoPckg.QuestPckg[questId].isComplete = 1;
					}
				}
			}

			db->Close();
		}
	}

	if (db->Open())
	{
		const char* const query = "SELECT questID, questProgress1, questProgress2, questProgress3, playersKilled FROM PlayerActiveQuest WHERE PlayerName=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->Execute(FALSE);

			while (db->NextRow())
			{
				db->GetData(1, PARAMTYPE_Integer, &questId);
				db->GetData(2, PARAMTYPE_Integer, &progress[0]);
				db->GetData(3, PARAMTYPE_Integer, &progress[1]);
				db->GetData(4, PARAMTYPE_Integer, &progress[2]);
				db->GetData(5, PARAMTYPE_Integer, &kill);

				Player->questInfoPckg.QuestPckg[questId].questID = questId;
				Player->questInfoPckg.QuestPckg[questId].questProgress[0] = progress[0];
				Player->questInfoPckg.QuestPckg[questId].questProgress[1] = progress[1];
				Player->questInfoPckg.QuestPckg[questId].questProgress[2] = progress[2];
				Player->questInfoPckg.QuestPckg[questId].killProgress = kill;

				vIDs.push_back(questId);
			}

			db->Close();

			while (vIDs.size() > 0)
			{
				int pos = vIDs[vIDs.size() - 1];

				for (int x = 0; x <= 60; x++)
				{
					if (questPckg.QuestPckg[x].questID == pos)
					{
						questObjective = questPckg.QuestPckg[pos].questObjective;
						Player->questInfoPckg.QuestPckg[pos].questType = questPckg.QuestPckg[pos].questType;
						Player->questInfoPckg.QuestPckg[pos].questObjective = questObjective;

						switch (questObjective)
						{
						case QUEST_KILL_MONSTER:
						case QUEST_KILL_ANY_MONSTER:
							progresstoGo[0] = questPckg.QuestPckg[pos].monsterNum[0];
							progresstoGo[1] = questPckg.QuestPckg[pos].monsterNum[1];
							progresstoGo[2] = questPckg.QuestPckg[pos].monsterNum[2];
							strcpy_s(Player->updatequest[pos].wantedMonster[0], questPckg.QuestPckg[pos].wantedMonster[0]);
							strcpy_s(Player->updatequest[pos].wantedMonster[1], questPckg.QuestPckg[pos].wantedMonster[1]);
							strcpy_s(Player->updatequest[pos].wantedMonster[2], questPckg.QuestPckg[pos].wantedMonster[2]);
							Player->questInfoPckg.QuestPckg[pos].questMap[0] = questPckg.QuestPckg[pos].monsterMap[0];
							Player->questInfoPckg.QuestPckg[pos].questMap[1] = questPckg.QuestPckg[pos].monsterMap[1];
							Player->questInfoPckg.QuestPckg[pos].questMap[2] = questPckg.QuestPckg[pos].monsterMap[2];
							break;
						case QUEST_KILL_PLAYER:
							killToGo = questPckg.QuestPckg[pos].playersToKill;
							break;
						case QUEST_GET_ITEM:
							progresstoGo[0] = questPckg.QuestPckg[pos].monsterNum[0];
							progresstoGo[1] = questPckg.QuestPckg[pos].monsterNum[1];
							progresstoGo[2] = questPckg.QuestPckg[pos].monsterNum[2];
							break;
						default:
							break;
						}

						Player->updatequest[pos].monsterNum[0] = progresstoGo[0];
						Player->updatequest[pos].monsterNum[1] = progresstoGo[1];
						Player->updatequest[pos].monsterNum[2] = progresstoGo[2];
						Player->updatequest[pos].monsterMap[0] = Player->questInfoPckg.QuestPckg[pos].questMap[0];
						Player->updatequest[pos].monsterMap[1] = Player->questInfoPckg.QuestPckg[pos].questMap[1];
						Player->updatequest[pos].monsterMap[2] = Player->questInfoPckg.QuestPckg[pos].questMap[2];

						if (questObjective == QUEST_KILL_MONSTER || questObjective == QUEST_GET_ITEM || questObjective == QUEST_KILL_ANY_MONSTER)
						{
							if ((Player->questInfoPckg.QuestPckg[pos].questProgress[0] >= progresstoGo[0]) && 
								(Player->questInfoPckg.QuestPckg[pos].questProgress[1] >= progresstoGo[1]) &&
								(Player->questInfoPckg.QuestPckg[pos].questProgress[2] >= progresstoGo[2]))
							{
								Player->questInfoPckg.QuestPckg[pos].isReadyToComplete = 1;
							}
							else 
							{
								Player->questInfoPckg.QuestPckg[pos].isReadyToComplete = 0;
							}
						}
						else
						{
							if (Player->questInfoPckg.QuestPckg[pos].killProgress >= killToGo)
							{
								Player->questInfoPckg.QuestPckg[pos].isReadyToComplete = 1;
							}
							else 
							{
								Player->questInfoPckg.QuestPckg[pos].isReadyToComplete = 0;
							}
						}

						if (!Player->questInfoPckg.QuestPckg[pos].isReadyToComplete) Player->questInfoPckg.QuestPckg[pos].isActive = 1;

						vIDs.pop_back();
						break;
					}
				}
			}
		}

		if (Send)
			Player->lpsmSock->Send((char*)&Player->questInfoPckg, Player->questInfoPckg.size, TRUE);
	}
}


struct sCompressedData
{
	char pCompressedData[6000];
	int compressedSize;
};

struct QUEST_COMPRESSEDPCKG {
	int size;
	int code;
	sCompressedData CompressedDataPckg;
};

QUEST_COMPRESSEDPCKG pckgToSend = {};

void Quest::SendAllQuests(rsPLAYINFO* Player)
{
	if (updateQuests)
	{
		ZeroMemory(&pckgToSend, sizeof(QUEST_COMPRESSEDPCKG));
		pckgToSend.size = sizeof(QUEST_COMPRESSEDPCKG);
		pckgToSend.code = smTRANSCODE_GET_ALL_QUESTS;

		ZeroMemory(&questPckg, sizeof(QUESTPCKG));
		questPckg.size = sizeof(QUESTPCKG);
		questPckg.code = smTRANSCODE_GET_ALL_QUESTS;
		//questPckg.total = 0;

		int i = 0;
		int y = 0;

		auto db = SQLConnection::GetConnection(DATABASEID_Quest);

		if (db->Open())
		{
			const char* const query = "SELECT * FROM Quests ORDER BY questID ASC";

			if (db->Prepare(query))
			{
				db->Execute(FALSE);

				int pos = 0;

				while (db->NextRow())
				{
					pos = 0;

					db->GetData(1, PARAMTYPE_Integer, &pos);

					questPckg.QuestPckg[pos].questID = pos;

					db->GetData(2, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].questObjective);
					db->GetData(3, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].questType);
					db->GetData(4, PARAMTYPE_String, questPckg.QuestPckg[pos].questName, sizeof(questPckg.QuestPckg[pos].questName));
					db->GetData(5, PARAMTYPE_String, questPckg.QuestPckg[pos].questDescription, sizeof(questPckg.QuestPckg[pos].questDescription));
					db->GetData(7, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].rewardGold);
					db->GetData(8, PARAMTYPE_Int64, &questPckg.QuestPckg[pos].rewardExp);
					db->GetData(9, PARAMTYPE_String, questPckg.QuestPckg[pos].rewardItem[0], sizeof(questPckg.QuestPckg[pos].rewardItem[0]));
					db->GetData(10, PARAMTYPE_String, questPckg.QuestPckg[pos].rewardItem[1], sizeof(questPckg.QuestPckg[pos].rewardItem[1]));
					db->GetData(11, PARAMTYPE_String, questPckg.QuestPckg[pos].rewardItem[2], sizeof(questPckg.QuestPckg[pos].rewardItem[2]));
					db->GetData(12, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].monsterNum[0]);
					db->GetData(13, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].monsterNum[1]);
					db->GetData(14, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].monsterNum[2]);

					db->GetData(15, PARAMTYPE_String, questPckg.QuestPckg[pos].wantedMonster[0], sizeof(questPckg.QuestPckg[pos].wantedMonster[0]));
					db->GetData(16, PARAMTYPE_String, questPckg.QuestPckg[pos].wantedMonster[1], sizeof(questPckg.QuestPckg[pos].wantedMonster[1]));
					db->GetData(17, PARAMTYPE_String, questPckg.QuestPckg[pos].wantedMonster[2], sizeof(questPckg.QuestPckg[pos].wantedMonster[2]));

					db->GetData(18, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].monsterMap[0]);
					db->GetData(19, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].monsterMap[1]);
					db->GetData(20, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].monsterMap[2]);

					db->GetData(21, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].playersToKill);
					db->GetData(22, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].qtItens[0]);
					db->GetData(23, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].qtItens[1]);
					db->GetData(24, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].qtItens[2]);

					db->GetData(25, PARAMTYPE_String, questPckg.QuestPckg[pos].idItem[0], sizeof(questPckg.QuestPckg[pos].idItem[0]));
					db->GetData(26, PARAMTYPE_String, questPckg.QuestPckg[pos].idItem[1], sizeof(questPckg.QuestPckg[pos].idItem[1]));
					db->GetData(27, PARAMTYPE_String, questPckg.QuestPckg[pos].idItem[2], sizeof(questPckg.QuestPckg[pos].idItem[2]));

					db->GetData(28, PARAMTYPE_String, questPckg.QuestPckg[pos].itemDescription[0], sizeof(questPckg.QuestPckg[pos].itemDescription[0]));
					db->GetData(29, PARAMTYPE_String, questPckg.QuestPckg[pos].itemDescription[1], sizeof(questPckg.QuestPckg[pos].itemDescription[1]));
					db->GetData(30, PARAMTYPE_String, questPckg.QuestPckg[pos].itemDescription[2], sizeof(questPckg.QuestPckg[pos].itemDescription[2]));

					db->GetData(31, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].minLevel);
					db->GetData(32, PARAMTYPE_Integer, &questPckg.QuestPckg[pos].maxLevel);

					i++;
				}

				unsigned long nCompressedDataSize = questPckg.size;
				unsigned char* UnpCompressedData = new unsigned char[nCompressedDataSize];

				int nResult = compress2((Bytef*)pckgToSend.CompressedDataPckg.pCompressedData, &nCompressedDataSize, (Bytef*)&questPckg.QuestPckg, sizeof(questPckg.QuestPckg) * i, 9);
				pckgToSend.CompressedDataPckg.compressedSize = nCompressedDataSize;

				if (nResult == Z_OK)
				{
					updateQuests = false;

					if (Player)
					{
						if (Player->lpsmSock)
							GameMasters::getInstance()->Alert(Player, "Desafios Recarregados!");
					}
				}
				else
				{
					if (Player)
					{
						if (Player->lpsmSock)
							GameMasters::getInstance()->Alert(Player, "Falha ao recarregar os desafios!");
					}
				}

			}

			db->Close();
		}

		nQuestAtivas = i;
	}
	else
	{
		if (Player && Player->lpsmSock)
			Player->lpsmSock->Send((char*)&pckgToSend, pckgToSend.size, TRUE);
	}
}

extern int GetPostBoxFile(char* szID, char* szFileName);

int SendReward(char* id, char* name, char* ItemName, int iQuantity, int gold)
{
	char	szFileName[64];


	if (!id[0]) return FALSE;

	GetPostBoxFile(id, szFileName);

	FILE* pFile = NULL;
	fopen_s(&pFile, szFileName, "a+");

	if (pFile)
	{
		for (int i = 0; i < iQuantity; i++)
		{
			fprintf(pFile, "%s		%s		%d		\"%s\"\r\n", name, ItemName, gold, "Recompensa de Desafio");
		}

		fclose(pFile);

		return TRUE;
	}

	return TRUE;
}

bool Quest::checkCheat(rsPLAYINFO* Player, int questID)
{
	if (!Player)
		return false;

	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	bool isOk = FALSE; // Tentando entregar quest que não está ativa no char

	if (db->Open())
	{
		const char* const query = "SELECT * FROM PlayerActiveQuest WHERE PlayerName=? AND questID=?";

		if (db->Prepare(query))
		{

			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->BindInputParameter(&questID, 2, PARAMTYPE_Integer);

			if (db->Execute())
			{
				isOk = TRUE;
			}
		}

		db->Close();
	}

	return isOk;
}

void GetDateTimeQ(char* pszDateTime)
{
	char szDate[9] = { 0 };
	char szTime[9] = { 0 };
	_strdate_s(szDate, 9);
	_strtime_s(szTime, 9);

	wsprintfA(pszDateTime, "%s %s", szDate, szTime);
}

int Quest::rewardQuest(rsPLAYINFO* Player, int questID, int questType, int questObjective)
{
	// Tentativa de crash
	if (questID > 60)
		return FALSE;

	if (!Player->questInfoPckg.QuestPckg[questID].isReadyToComplete)
	{
		return FALSE;

		/*auto db = SQLConnection::GetConnection(DATABASEID_LogDB);

		int tipoBan = 1000;
		int idCheat = 8090;
		char typeban[40] = { 0 };
		char szMac[64] = { 0 };
		char szUUID[64] = { 0 };
		char szDateTime[20];
		GetDateTimeQ(szDateTime);
		sprintf_s(typeban, sizeof(typeban), "Quest Hacker (ID: %d)", questID);

		if (db && db->Open())
		{
			if (db->Prepare("INSERT INTO CheatLog VALUES (?,?,?,?,?,?)"))
			{
				db->BindInputParameter(Player->lpsmSock->szIPAddr, 1, PARAMTYPE_String);
				db->BindInputParameter(Player->szID, 2, PARAMTYPE_String);
				db->BindInputParameter(&tipoBan, 3, PARAMTYPE_Integer);
				db->BindInputParameter(&idCheat, 4, PARAMTYPE_Integer);
				db->BindInputParameter(typeban, 5, PARAMTYPE_String);
				db->BindInputParameter(szDateTime, 6, PARAMTYPE_String);
				db->Execute();
			}

			if (db->Prepare("SELECT TOP 1 * FROM AccountLogin WHERE szID=?"))
			{
				db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);

				if (db->Execute())
				{
					db->GetData(4, PARAMTYPE_String, szMac, sizeof(szMac));
					db->GetData(7, PARAMTYPE_String, szUUID, sizeof(szUUID));
				}
			}

			db->Close();
		}

		auto db2 = SQLConnection::GetConnection(DATABASEID_UserDB);

		if (db2 && db2->Open())
		{
			if (db2->Prepare("INSERT INTO BannedMac VALUES(?,?,?,1)"))
			{
				db2->BindInputParameter("Quest Hacker", 1, PARAMTYPE_String);
				db2->BindInputParameter(szMac, 2, PARAMTYPE_String);
				db2->BindInputParameter(szUUID, 3, PARAMTYPE_String);

				db2->Execute();
			}

			db2->Close();
		}

		GameMasters::getInstance()->Ban(Player, true, Player->szID, typeban);

		return FALSE;*/
	}

	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	char rewardItemCode[3][12] = { 0 };
	int  rewardQuantity[3] = { 0 };
	INT64 rewardGold = 0, rewardEXP = 0;

	GameMasters::getInstance()->Alert(Player, "Quest> Você completou o Desafio!");
	GameMasters::getInstance()->Alert(Player, "Quest> Recompensas são enviadas ao distribuidor.");

	if (db->Open())
	{
		const char* const query = "SELECT * FROM QuestRewards WHERE questID=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(&questID, 1, PARAMTYPE_Integer);

			if (db->Execute())
			{
				int RewardGoldTemp = 0;
				INT64 RewardExpTemp = 0;

				db->GetData(2, PARAMTYPE_String, rewardItemCode[0], sizeof(rewardItemCode[0]));
				db->GetData(3, PARAMTYPE_String, rewardItemCode[1], sizeof(rewardItemCode[1]));
				db->GetData(4, PARAMTYPE_String, rewardItemCode[2], sizeof(rewardItemCode[2]));
				db->GetData(5, PARAMTYPE_Integer, &rewardQuantity[0]);
				db->GetData(6, PARAMTYPE_Integer, &rewardQuantity[1]);
				db->GetData(7, PARAMTYPE_Integer, &rewardQuantity[2]);
				db->GetData(8, PARAMTYPE_Integer, &RewardGoldTemp);
				db->GetData(9, PARAMTYPE_Int64, &RewardExpTemp);

				rewardGold = RewardGoldTemp;
				rewardEXP = RewardExpTemp;
			}

		}

		if (db->Prepare("DELETE FROM PlayerActiveQuest WHERE PlayerName=? AND questID=?"))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->BindInputParameter(&questID, 2, PARAMTYPE_Integer);
			db->Execute();
		}

		if (questType != QUEST_REPEAT_SOLOPARTY && questType != QUEST_REPEAT_PARTY && questType != QUEST_REPEAT_SOLO)
		{
			if (db->Prepare("INSERT INTO PlayerCompletedQuest VALUES (?,?,?,GETDATE())"))
			{
				db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
				db->BindInputParameter(&questID, 2, PARAMTYPE_Integer);
				db->BindInputParameter(&questType, 3, PARAMTYPE_Integer);
				db->Execute();
			}
		}

		if (rewardQuantity[0])
		{
			if (boost::iequals(rewardItemCode[0], "Coin"))
				NewShop::GetInstance()->addCoinsToPlayer(Player, Player->szID, rewardQuantity[0], 1, WhereCoinsComeFrom::FROM_QUEST);
			else
				SendReward(Player->szID, Player->szName, rewardItemCode[0], rewardQuantity[0], 0);
		}

		if (rewardQuantity[1])
		{
			if (boost::iequals(rewardItemCode[1], "Coin"))
				NewShop::GetInstance()->addCoinsToPlayer(Player, Player->szID, rewardQuantity[1], 1, WhereCoinsComeFrom::FROM_QUEST);
			else
				SendReward(Player->szID, Player->szName, rewardItemCode[1], rewardQuantity[1], 0);
		}

		if (rewardQuantity[2])
		{
			if (boost::iequals(rewardItemCode[2], "Coin"))
				NewShop::GetInstance()->addCoinsToPlayer(Player, Player->szID, rewardQuantity[2], 1, WhereCoinsComeFrom::FROM_QUEST);
			else
				SendReward(Player->szID, Player->szName, rewardItemCode[2], rewardQuantity[2], 0);
		}

		if (rewardGold)
		{
			SendReward(Player->szID, Player->szName, "GG101", 1, rewardGold);
		}
		if (rewardEXP)
		{
			// Verifica se o personagem já atingiu o nível máximo
			if (Player->smCharInfo.Level >= g_LevelFinal)
			{
				// Personagem já atingiu o nível máximo, não ganha XP da quest
				GameMasters::getInstance()->Alert(Player, "Quest> Você não ganhou XP pois já atingiu o nível máximo!");
			}
			else
			{
				GameMasters::getInstance()->AddExpToPlayer(Player, rewardEXP);
				Player->LastExp += rewardEXP;
				Player->dwGameServerExp[0] += rewardEXP;
			}
		}

		db->Close();
		getStatus(Player, true);

		return TRUE;
	}

	return FALSE;

}
int Quest::cancelQuest(rsPLAYINFO* Player, int questID, int questType) {

	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	char questName[130];

	if (db->Open())
	{
		if (db->Prepare("SELECT questName FROM Quests WHERE questID=?"))
		{
			db->BindInputParameter(&questID, 1, PARAMTYPE_Integer);

			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_String, questName, sizeof(questName));
			}
		}

		if (db->Prepare("DELETE FROM PlayerActiveQuest WHERE PlayerName=? AND questID=?"))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->BindInputParameter(&questID, 2, PARAMTYPE_Integer);

			if (db->Execute(false))
			{
				char msgOk[156];
				sprintf_s(msgOk, sizeof(msgOk), "Quest> Você cancelou o desafio [%s]!", questName);
				GameMasters::getInstance()->Alert(Player, msgOk);
			}

		}

		getStatus(Player, true);
		db->Close();

		return TRUE;
	}

	return FALSE;
}

int Quest::startNewQuest(rsPLAYINFO* Player, int questID, int questType) {

	int totalInProgress = 0;
	char questName[130];
	int questlvlMin = 0, questlvlMax = 0, pvpMap = 0, questObjective = 0;
	int x = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_Quest);

	if (db->Open())
	{
		if (db->Prepare("SELECT COUNT(*) FROM PlayerActiveQuest WHERE PlayerName=?"))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);

			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &totalInProgress);

				if (totalInProgress >= 5)
				{
					GameMasters::getInstance()->Alert(Player, "Quest> Você só pode estar em 5 Desafios por vez!");
					db->Close();
					return FALSE;
				}
			}
		}

		if (db->Prepare("SELECT * FROM Quests WHERE questID=?"))
		{
			db->BindInputParameter(&questID, 1, PARAMTYPE_Integer);

			if (db->Execute())
			{
				db->GetData(2, PARAMTYPE_Integer, &questObjective);
				db->GetData(4, PARAMTYPE_String, questName, sizeof(questName));
				db->GetData(18, PARAMTYPE_Integer, &pvpMap);
				db->GetData(31, PARAMTYPE_Integer, &questlvlMin);
				db->GetData(32, PARAMTYPE_Integer, &questlvlMax);
			}
		}

		if (Player->smCharInfo.Level < questlvlMin || Player->smCharInfo.Level > questlvlMax)
		{
			GameMasters::getInstance()->Alert(Player, "Quest> Você não atende aos requisitos de nível!");
			db->Close();
			return FALSE;
		}


		if (db->Prepare("INSERT INTO PlayerActiveQuest VALUES (?,?,?,0,0,0,0,GETDATE(),?,?)"))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
			db->BindInputParameter(&questID, 2, PARAMTYPE_Integer);
			db->BindInputParameter(&questType, 3, PARAMTYPE_Integer);
			db->BindInputParameter(&questObjective, 4, PARAMTYPE_Integer);
			db->BindInputParameter(&pvpMap, 5, PARAMTYPE_Integer);
			db->Execute();
		}

		db->Close();

		getStatus(Player, true);

		char msgOk[128] = { 0 };
		sprintf_s(msgOk, sizeof(msgOk), "Quest> Você iniciou o desafio [%s]!", questName);
		GameMasters::getInstance()->Alert(Player, msgOk);

		return TRUE;
	}

	return FALSE;
}