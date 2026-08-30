#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\FontImage.h"
#include "..\\field.h"
#include "Quest.h"
#include <boost/algorithm/string.hpp>
#include <zlib.h>

QUESTPCKG allQuests;
QUEST_INFOPCKG questInfoPlayer;

bool isFirstPckg = true;
bool updateQuests = false;
int y = 0;

int Quest::updateStatus(smCHAR* Monster) {

	if (!Monster || Monster == nullptr)
		return FALSE;

	int x = 0, y = 0, z = 0, isSpecificMap = 0, killToGo = 0, flag = 0, isOnParty = 0;
	char monsterName[32] = { 0 };

	if (PARTYHANDLER->IsPartyMember(UNITDATA->dwObjectSerial))
	{
		isOnParty = 1;
	}

	for (std::vector<QUESTPCKG*>::iterator it = vQuests.begin(); it < vQuests.end(); it++)
	{
		QUESTPCKG* getRightQuest = (*it);

		if (questInfoPlayer.QuestPckg[x + 1].isActive) {

			if (getRightQuest->QuestPckg[x].questType == QUEST_UNIQUE_PARTY || getRightQuest->QuestPckg[x].questType == QUEST_DAILY_PARTY || getRightQuest->QuestPckg[x].questType == QUEST_REPEAT_PARTY) {
				if (!isOnParty) goto skip;
			}

			if (getRightQuest->QuestPckg[x].questType == QUEST_UNIQUE_SOLO || getRightQuest->QuestPckg[x].questType == QUEST_DAILY_SOLO || getRightQuest->QuestPckg[x].questType == QUEST_REPEAT_SOLO) {
				if (isOnParty) goto skip;
			}

			for (z = 0; z <= 2; z++)
			{
				if (getRightQuest && (boost::iequals(Monster->smCharInfo.szName, getRightQuest->QuestPckg[x].wantedMonster[y]) || getRightQuest->QuestPckg[x].questObjective == QUEST_KILL_ANY_MONSTER))
				{
					isSpecificMap = 0;

					memset(&monsterName, 0, sizeof(monsterName));
					sprintf_s(monsterName, sizeof(monsterName), Monster->smCharInfo.szName);

					killToGo = getRightQuest->QuestPckg[x].monsterNum[y];

					if (getRightQuest->QuestPckg[x].monsterMap[y] > 0)
						isSpecificMap = getRightQuest->QuestPckg[x].monsterMap[y];

					if (questInfoPlayer.QuestPckg[x + 1].questProgress[y] < killToGo) {
						if (isSpecificMap)
						{
							if (monsterName[0] && StageField[lpCurPlayer->OnStageField]->FieldCode == isSpecificMap)
								flag = 1;
						}
						else if (!isSpecificMap && monsterName[0]) {
							flag = 1;
						}

						if (flag) questInfoPlayer.QuestPckg[x + 1].questProgress[y] += 1;

						if (monsterName[0] && flag)
						{
							CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Notice, "> %s: %d/%d Restantes", monsterName, questInfoPlayer.QuestPckg[x + 1].questProgress[y], killToGo);
						}
					}
				}
				else if (getRightQuest && !getRightQuest->QuestPckg[x].wantedMonster[y] && getRightQuest->QuestPckg[x].monsterMap[y] > 0)
				{

					if (StageField[lpCurPlayer->OnStageField]->FieldCode == getRightQuest->QuestPckg[x].monsterMap[y])
						questInfoPlayer.QuestPckg[x + 1].questProgress[y] += 1;

					memset(&monsterName, 0, sizeof(monsterName));
					sprintf_s(monsterName, sizeof(monsterName), Monster->smCharInfo.szName);
					killToGo = getRightQuest->QuestPckg[x].monsterNum[y];

					CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Notice, "> %s: %d/%d Restantes", monsterName, questInfoPlayer.QuestPckg[x + 1].questProgress[y], killToGo);
				}
				y++;
				flag = 0;
			}
		}
	skip:
		flag = 0;
		y = 0;
		x++;
	}

	return TRUE;
}

void Quest::getAllQuests(QUEST_COMPRESSEDPCKG* Data)
{
	QUESTPCKG getAllQuest;
	ZeroMemory(&getAllQuest, sizeof(QUESTPCKG));

	unsigned long nCompressedDataSize = Data->CompressedDataPckg.compressedSize;
	unsigned long UnCompressedDataSize = 70000;

	uncompress((Bytef*)&getAllQuest.QuestPckg, &UnCompressedDataSize, (Bytef*)Data->CompressedDataPckg.pCompressedData, nCompressedDataSize);

	if (isFirstPckg || updateQuests) {
		vQuests.clear();
		y = 0;
	}

	int x = 1;
	while (true)
	{
		allQuests.QuestPckg[y].questID = getAllQuest.QuestPckg[x].questID;
		allQuests.QuestPckg[y].questObjective = getAllQuest.QuestPckg[x].questObjective;
		allQuests.QuestPckg[y].questType = getAllQuest.QuestPckg[x].questType;
		strcpy_s(allQuests.QuestPckg[y].questName, getAllQuest.QuestPckg[x].questName);
		strcpy_s(allQuests.QuestPckg[y].questDescription, getAllQuest.QuestPckg[x].questDescription);
		allQuests.QuestPckg[y].rewardGold = getAllQuest.QuestPckg[x].rewardGold;
		allQuests.QuestPckg[y].rewardExp = getAllQuest.QuestPckg[x].rewardExp;
		strcpy_s(allQuests.QuestPckg[y].rewardItem[0], getAllQuest.QuestPckg[x].rewardItem[0]);
		strcpy_s(allQuests.QuestPckg[y].rewardItem[1], getAllQuest.QuestPckg[x].rewardItem[1]);
		strcpy_s(allQuests.QuestPckg[y].rewardItem[2], getAllQuest.QuestPckg[x].rewardItem[2]);
		allQuests.QuestPckg[y].monsterNum[0] = getAllQuest.QuestPckg[x].monsterNum[0];
		allQuests.QuestPckg[y].monsterNum[1] = getAllQuest.QuestPckg[x].monsterNum[1];
		allQuests.QuestPckg[y].monsterNum[2] = getAllQuest.QuestPckg[x].monsterNum[2];
		strcpy_s(allQuests.QuestPckg[y].wantedMonster[0], getAllQuest.QuestPckg[x].wantedMonster[0]);
		strcpy_s(allQuests.QuestPckg[y].wantedMonster[1], getAllQuest.QuestPckg[x].wantedMonster[1]);
		strcpy_s(allQuests.QuestPckg[y].wantedMonster[2], getAllQuest.QuestPckg[x].wantedMonster[2]);
		allQuests.QuestPckg[y].monsterMap[0] = getAllQuest.QuestPckg[x].monsterMap[0];
		allQuests.QuestPckg[y].monsterMap[1] = getAllQuest.QuestPckg[x].monsterMap[1];
		allQuests.QuestPckg[y].monsterMap[2] = getAllQuest.QuestPckg[x].monsterMap[2];
		allQuests.QuestPckg[y].playersToKill = getAllQuest.QuestPckg[x].playersToKill;
		allQuests.QuestPckg[y].qtItens[0] = getAllQuest.QuestPckg[x].qtItens[0];
		allQuests.QuestPckg[y].qtItens[1] = getAllQuest.QuestPckg[x].qtItens[1];
		allQuests.QuestPckg[y].qtItens[2] = getAllQuest.QuestPckg[x].qtItens[2];
		strcpy_s(allQuests.QuestPckg[y].idItem[0], getAllQuest.QuestPckg[x].idItem[0]);
		strcpy_s(allQuests.QuestPckg[y].idItem[1], getAllQuest.QuestPckg[x].idItem[1]);
		strcpy_s(allQuests.QuestPckg[y].idItem[2], getAllQuest.QuestPckg[x].idItem[2]);
		strcpy_s(allQuests.QuestPckg[y].itemDescription[0], getAllQuest.QuestPckg[x].itemDescription[0]);
		strcpy_s(allQuests.QuestPckg[y].itemDescription[1], getAllQuest.QuestPckg[x].itemDescription[1]);
		strcpy_s(allQuests.QuestPckg[y].itemDescription[2], getAllQuest.QuestPckg[x].itemDescription[2]);
		allQuests.QuestPckg[y].minLevel = getAllQuest.QuestPckg[x].minLevel;
		allQuests.QuestPckg[y].maxLevel = getAllQuest.QuestPckg[x].maxLevel;

		y++;
		x++;
		vQuests.push_back(&allQuests);

		if (y > 59) break; // máximo de desafios
	}

	updateQuests = false;
	isFirstPckg = false;

	DWORD dwSize = sizeof(questInfoPlayer);
	DWORD dwCheckSum = 0;
	DWORD dwOld = 0;
	VirtualProtect((void*)questInfoPlayer.QuestPckg, dwSize, PAGE_EXECUTE_READWRITE, &dwOld);
}

int Quest::CheckItemsFromInven() {
	int x = 0, it1 = 0, it2 = 0, it3 = 0, totalItems = 0;

	DWORD idItem1;
	DWORD idItem2;
	DWORD idItem3;

	for (std::vector<QUESTPCKG*>::iterator it = vQuests.begin(); it < vQuests.end(); it++)
	{
		QUESTPCKG* CheckItems = (*it);

		if (questInfoPlayer.QuestPckg[x + 1].isActive)
		{
			if (CheckItems->QuestPckg[x].questObjective == QUEST_GET_ITEM)
			{
				if (CheckItems->QuestPckg[x].qtItens[0]) it1 += CheckItems->QuestPckg[x].qtItens[0];
				if (CheckItems->QuestPckg[x].qtItens[1]) it2 += CheckItems->QuestPckg[x].qtItens[1];
				if (CheckItems->QuestPckg[x].qtItens[2]) it3 += CheckItems->QuestPckg[x].qtItens[2];

				totalItems = it1 + it2 + it3;

				if (cInvenTory.SearchItemByName(CheckItems->QuestPckg[x].idItem[0], it1) >= it1)
				{
					totalItems -= it1;
				}

				if (cInvenTory.SearchItemByName(CheckItems->QuestPckg[x].idItem[1], it2) >= it2)
				{
					totalItems -= it2;
				}

				if (cInvenTory.SearchItemByName(CheckItems->QuestPckg[x].idItem[2], it3) >= it3)
				{
					totalItems -= it3;
				}

				if (totalItems == 0) {
					questInfoPlayer.QuestPckg[x + 1].isActive = FALSE;
					questInfoPlayer.QuestPckg[x + 1].isReadyToComplete = TRUE;
				}
			}
		}
		totalItems = 0;
		x++;
	}

	return TRUE;
}

void Quest::startNewQuest(int questID, int questType) {
	smTRANS_COMMAND startNewQuest;
	startNewQuest.code = smTRANSCODE_START_NEW_QUEST;
	startNewQuest.size = sizeof(smTRANS_COMMAND);
	startNewQuest.WParam = questID;
	startNewQuest.LParam = questType;
	startNewQuest.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&startNewQuest, startNewQuest.size, TRUE);
}

void Quest::cancelQuest(int questID, int questType) {
	smTRANS_COMMAND cancelQuest;
	cancelQuest.code = smTRANSCODE_CANCEL_QUEST;
	cancelQuest.size = sizeof(smTRANS_COMMAND);
	cancelQuest.WParam = questID;
	cancelQuest.LParam = questType;
	cancelQuest.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&cancelQuest, cancelQuest.size, TRUE);
}

extern DWORD GetInvenItemCode();

void Quest::finishQuest(int questID, int questType, int questObjective) {
	smTRANS_COMMAND finishQuest;
	finishQuest.code = smTRANSCODE_FINISH_QUEST;
	finishQuest.size = sizeof(smTRANS_COMMAND);
	finishQuest.WParam = questID;
	finishQuest.LParam = questType;
	finishQuest.SParam = questObjective;

	extern DWORD dwTotal_InvenItemCode;
	extern DWORD dwTotal_InvenItemPlayTime;

	sITEMINFO* itemToDelete = { 0 };

	if (questInfoPlayer.QuestPckg[questID].questObjective == QUEST_GET_ITEM)
	{
		int x = 0;

		// Localiza qual quest está sendo finalizada
		for (std::vector<QUESTPCKG*>::iterator it = vQuests.begin(); it < vQuests.end(); it++)
		{
			QUESTPCKG* getItemsToDelete = (*it);

			if (getItemsToDelete->QuestPckg[x].questID == questID)
			{
				// Encontrou a quest, agora deleta os itens do inventário
				if (getItemsToDelete->QuestPckg[x].qtItens[0])
				{
					for (int y = getItemsToDelete->QuestPckg[x].qtItens[0]; y > 0; y--)
					{
						itemToDelete = FindItemFromName(getItemsToDelete->QuestPckg[x].idItem[0]);

						if (itemToDelete) {
							cInvenTory.DeleteInvenItemToServer(itemToDelete->CODE,
								itemToDelete->ItemHeader.Head, itemToDelete->ItemHeader.dwChkSum);
		
							DWORD dwCode = GetInvenItemCode();
							dwTotal_InvenItemCode = dwPlayTime ^ dwCode;
							dwTotal_InvenItemPlayTime = dwPlayTime;
							memcpy(&itemToDelete, 0, sizeof(sITEMINFO));
						}

					}
				}

				if (getItemsToDelete->QuestPckg[x].qtItens[1])
				{
					for (int y = getItemsToDelete->QuestPckg[x].qtItens[1]; y > 0; y--)
					{
						itemToDelete = FindItemFromName(getItemsToDelete->QuestPckg[x].idItem[1]);

						if (itemToDelete) {
							cInvenTory.DeleteInvenItemToServer(itemToDelete->CODE,
								itemToDelete->ItemHeader.Head, itemToDelete->ItemHeader.dwChkSum);

							DWORD dwCode = GetInvenItemCode();
							dwTotal_InvenItemCode = dwPlayTime ^ dwCode;
							dwTotal_InvenItemPlayTime = dwPlayTime;
							memcpy(&itemToDelete, 0, sizeof(sITEMINFO));
						}
					}
				}

				if (getItemsToDelete->QuestPckg[x].qtItens[2])
				{
					for (int y = getItemsToDelete->QuestPckg[x].qtItens[2]; y > 0; y--)
					{
						itemToDelete = FindItemFromName(getItemsToDelete->QuestPckg[x].idItem[2]);

						if (itemToDelete) {
							cInvenTory.DeleteInvenItemToServer(itemToDelete->CODE,
								itemToDelete->ItemHeader.Head, itemToDelete->ItemHeader.dwChkSum);

							DWORD dwCode = GetInvenItemCode();
							dwTotal_InvenItemCode = dwPlayTime ^ dwCode;
							dwTotal_InvenItemPlayTime = dwPlayTime;
							memcpy(&itemToDelete, 0, sizeof(sITEMINFO));
						}

					}
				}

				SaveGameData();
			}
			x++;
		}
	}

	if (smWsockServer)
		smWsockServer->Send((char*)&finishQuest, finishQuest.size, TRUE);
}


void Quest::requestStatus() {
	smTRANS_COMMAND getQuestStatus;
	getQuestStatus.code = smTRANSCODE_GET_QUEST_STATUS;
	getQuestStatus.size = sizeof(smTRANS_COMMAND);
	getQuestStatus.WParam = 0;
	getQuestStatus.LParam = 0;
	getQuestStatus.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&getQuestStatus, getQuestStatus.size, TRUE);
}

void Quest::getAllStatus(QUEST_INFOPCKG* getStatus) {
	ZeroMemory(&questInfoPlayer, sizeof(QUEST_INFOPCKG));
	CopyMemory(&questInfoPlayer, getStatus, sizeof(QUEST_INFOPCKG));

	CheckItemsFromInven();
}