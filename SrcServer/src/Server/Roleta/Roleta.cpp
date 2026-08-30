#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\Database\SQLConnection.h"
#include <boost/algorithm/string.hpp>
#include <thread>
#include <mutex>  

extern rsPLAYINFO* srFindUserFromSerial(DWORD dwObjectSerial);
extern rsPLAYINFO* rsPlayInfo;
extern int rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);

extern int DefaultItemCount;
extern sDEF_ITEMINFO* DefaultItems;
extern int rsItemOpenCount;

std::vector<TOP10_ROLETA> vSorteios[10];
std::vector<DWORD> vMobs;

bool dividirIgualmente = false;

std::mutex mtx;

struct PckItem
{
	int size;
	int code;
	int chance;
	char itemID[32];
	BYTE trans[sizeof(sITEM)];

	PckItem()
	{
		size = 0;
		code = 0;
		chance = 0;
		ZeroMemory(itemID, sizeof(itemID));
		ZeroMemory(trans, sizeof(sITEM));
	}
};

using namespace std::this_thread;
using namespace std::chrono;

void ThreadRoleta(RoletaPckg Players, smCHAR* Monster, short Slot)
{
	if (Monster)
	{
		smCHAR* pMonster = new smCHAR;
		ZeroMemory(pMonster, sizeof(smCHAR));

		memcpy_s(pMonster, sizeof(smCHAR), Monster, sizeof(smCHAR));

		sleep_until(system_clock::now() + seconds(8));

		mtx.lock();
		Roleta::GetInstance()->IniciarRoleta(Players, pMonster, Slot);
		mtx.unlock();

		if (pMonster)
			delete pMonster;
	}
}

void LogRoleta(char PlayerID[32], char PlayerName[32], char ItemCode[32], char MonsterName[32])
{
	auto db = SQLConnection::GetConnection(DATABASEID_LogDB);

	if (db && db->Open())
	{
		const char* const query = "INSERT INTO Roleta VALUES (?,?,?,?,GETDATE())";

		if (db->Prepare(query))
		{
			db->BindInputParameter(PlayerID, 1, PARAMTYPE_String);
			db->BindInputParameter(PlayerName, 2, PARAMTYPE_String);
			db->BindInputParameter(ItemCode, 3, PARAMTYPE_String);
			db->BindInputParameter(MonsterName, 4, PARAMTYPE_String);

			db->Execute();
		}

		db->Close();
	}

}

INT Roleta::AdicionarParticipante(rsPLAYINFO* Player)
{
	RoletaPckg pckg;
	pckg.code = 0x50700033;
	pckg.size = sizeof(RoletaPckg);

	int x = 0;
	int Pos = 0;
	bool pFound = false;

	// Encontra o player na roleta atual
	for (Pos = 0; Pos < 10; Pos++)
	{
		x = 0;

		for (auto& Participantes : vSorteios[Pos])
		{
			if (boost::iequals(Participantes.szName, Player->szName))
			{
				Participantes.pSorteio = true;
				pFound = true;
			}

			int chance = (Participantes.chance * 100) / vSorteios[Pos].size();

			pckg.Players[x] = Participantes;
			pckg.Players[x].Damage = chance;


			x++;
		}

		if (pFound)
			break;
	}

	if (pFound)
	{
		int rPos = Pos;

		// Envia o pacote somente para os participantes da roleta
		for (auto& Participantes : vSorteios[rPos])
		{
			if (Participantes.szName[0])
			{
				rsPLAYINFO* Participante = FindUserFromName(Participantes.szName);

				if (Participante && Participante->lpsmSock)
				{
					Participante->lpsmSock->Send((char*)&pckg, pckg.size, TRUE);
				}
			}
		}
	}

	return TRUE;
}


INT Roleta::IniciarRoleta(RoletaPckg Players, smCHAR* Monster, short Slot)
{
	if (!Monster->smCharInfo.szName[0])
		return FALSE;

	smCHAR MonstroRoleta;
	ZeroMemory(&MonstroRoleta, sizeof(smCHAR));

	memcpy_s(&MonstroRoleta, sizeof(smCHAR), Monster, sizeof(smCHAR));

	int nRoleta = MonstroRoleta.smMonsterInfo.FallItemMax;

	short SlotRoleta = Slot;

	while (nRoleta > 0)
	{
		// Adiciona os participantes na rodada
		for (int x = 0; x < 10; x++)
		{
			if (Players.Players[x].szName[0])
			{
				vSorteios[SlotRoleta].push_back(Players.Players[x]);
			}
		}

		int Scape = 0;

		psITEM* lpsItem = new psITEM;
		sITEM itemSorteado = { 0 };

		char itemCode[32] = { 0 };

		srand(static_cast<unsigned long>(time(NULL)));

		while (true)
		{
			int itemPos = GetRandomPos(0, MonstroRoleta.smMonsterInfo.FallItemCount);
			int rnd = GetRandomPos(1, 1000);

			Scape++;

			if (rnd <= MonstroRoleta.smMonsterInfo.FallItems[itemPos].Percentage)
			{
				if (!MonstroRoleta.smMonsterInfo.FallItems[itemPos].dwItemCode) return FALSE;

				auto dwCode = MonstroRoleta.smMonsterInfo.FallItems[itemPos].dwItemCode;

				if (dwCode == (sinGG1 | sin01)) continue;

				for (int cnt2 = 0; cnt2 < DefaultItemCount; cnt2++)
				{
					if (DefaultItems[cnt2].Item.CODE == dwCode && !DefaultItems[cnt2].Item.UniqueItem)
					{
						CreateDefItem(&lpsItem->ItemInfo, &DefaultItems[cnt2]);

						// Pega o código do item no jogo
						for (int cnt = 0; cnt < MAX_ITEM; cnt++)
						{
							if (lpsItem->ItemInfo.CODE == sItem[cnt].CODE)
							{
								sprintf_s(itemCode, sizeof(itemCode), "%s", sItem[cnt].LastCategory);
								itemSorteado = sItem[cnt];

								itemSorteado.sItemInfo = lpsItem->ItemInfo;
							}
						}

						for (int cnt = 0; cnt < CONNECTMAX; cnt++)
						{
							if (rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].smCharInfo.szName[0])
							{
								for (int x = 0; x < 10; x++)
								{
									if (boost::iequals(rsPlayInfo[cnt].smCharInfo.szName, Players.Players[x].szName))
									{
										Players.Players[x].totalDamage = Players.totalDamage;

										int chance = 100 / Players.participantes;

										PckItem smPacket;

										smPacket.code = ROLETA_PARTICIPAR_RODADA;
										smPacket.chance = chance;
										sprintf_s(smPacket.itemID, sizeof(smPacket.itemID), "%s", itemCode);
										int len = EecodeCompress((BYTE*)&itemSorteado, smPacket.trans, sizeof(sITEM));

										smPacket.size = len + 44;

										rsPlayInfo[cnt].lpsmSock->Send((char*)&smPacket, smPacket.size, TRUE);
									}
								}
							}
						}

					}
				}

				if (dwCode != (sinGG1 | sin01))
					break;
			}

			// Previne loop infinito se o mob não tiver itens ou chance = 0
			if (Scape > 15)
				break;

			rnd = GetRandomPos(1, 100);
		}

		if (Scape > 15)
		{
			for (int cnt = 0; cnt < CONNECTMAX; cnt++)
			{
				if (rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].smCharInfo.szName[0])
				{
					for (int x = 0; x < 10; x++)
					{
						if (boost::iequals(rsPlayInfo[cnt].smCharInfo.szName, Players.Players[x].szName))
						{
							SERVERCHAT->SendChatEx(&rsPlayInfo[cnt], EChatColor::CHATCOLOR_Global, "Roleta> Nenhum item foi sorteado na rodada. Rodadas Restantes = %d", nRoleta - 1);

							if (nRoleta > 1)
								UTILS->Alert(&rsPlayInfo[cnt], "Roleta> Próxima rodada em 10 segundos!");
							else
								UTILS->Alert(&rsPlayInfo[cnt], "Roleta> As rodadas acabaram!");
						}
					}
				}
			}
		}

		sleep_until(system_clock::now() + seconds(10));

		int nSorteado = GetRandomPos(1, 10);

		char vencedor[32] = { 0 };

		int pParticipantes = 0;
		std::vector<TOP10_ROLETA> pRoletaAtual;
		pRoletaAtual.clear();

		for (auto& Jogadores : vSorteios[SlotRoleta])
		{
			if (Jogadores.pSorteio)
			{
				pParticipantes++;
				Jogadores.chance *= 0.8;
				Jogadores.pSorteio = false;

				pRoletaAtual.push_back(Jogadores);

				for (int x = 0; x < 10; x++)
				{
					if (boost::iequals(Jogadores.szName, Players.Players[x].szName))
						Players.Players[x].chance *= 0.8;
				}
			}
		}

		if (pParticipantes > 0) // se houver participantes
		{
			int n = 0;
			while (true)
			{
				n++;

				int playerSorteado = pParticipantes > 1 ? GetRandomPos(0, pParticipantes - 1) : 0;

				if (pRoletaAtual[playerSorteado].chance > nSorteado)
				{
					sprintf_s(vencedor, sizeof(vencedor), "%s", pRoletaAtual[playerSorteado].szName);
					break;
				}

				// Não conseguiu sortear uma chance válida
				if (n > 100)
				{
					sprintf_s(vencedor, sizeof(vencedor), "%s", pRoletaAtual[playerSorteado].szName);
					break;
				}
			}
		}

		if (vencedor[0])
		{
			for (int cnt = 0; cnt < CONNECTMAX; cnt++)
			{
				if (rsPlayInfo[cnt].lpsmSock && rsPlayInfo[cnt].smCharInfo.szName[0])
				{
					if (boost::iequals(rsPlayInfo[cnt].smCharInfo.szName, vencedor))
					{
						TRANS_ITEMINFO	TransItemInfo;
						ZeroMemory(&TransItemInfo, sizeof(TRANS_ITEMINFO));

						TransItemInfo.code = smTRANSCODE_PUTITEM;
						TransItemInfo.size = sizeof(TRANS_ITEMINFO);
						memcpy(&TransItemInfo.Item, &lpsItem->ItemInfo, sizeof(sITEMINFO));

						rsRegist_ItemSecCode(&rsPlayInfo[cnt], &TransItemInfo, 1);

						rsPlayInfo[cnt].lpsmSock->Send((char*)&TransItemInfo, TransItemInfo.size, TRUE);

						// Log da roleta no sql
						LogRoleta(rsPlayInfo[cnt].szID, rsPlayInfo[cnt].smCharInfo.szName, itemCode, MonstroRoleta.smCharInfo.szName);
					}

					for (int x = 0; x < 10; x++)
					{
						if (boost::iequals(rsPlayInfo[cnt].smCharInfo.szName, Players.Players[x].szName))
						{
							SERVERCHAT->SendChatEx(&rsPlayInfo[cnt], EChatColor::CHATCOLOR_Global, "Roleta> %s venceu a rodada!", vencedor);
						}
					}
				}
			}
		}

		// Limpa o slot atual da roelta
		vSorteios[SlotRoleta].clear();

		nRoleta--;
	}

	// Remove o serial do mob do vetor
	vMobs.erase(std::remove_if(std::begin(vMobs), std::end(vMobs),
		[&MonstroRoleta](DWORD& Serial)
		{
			return Serial == MonstroRoleta.dwObjectSerial;
		}), std::end(vMobs));

	// Limpa o slot atual da roelta
	vSorteios[SlotRoleta].clear();

	return TRUE;
}

INT Roleta::SendTopPlayers(rsUSER_LIST_TOP10 Top10List, smCHAR* Monster)
{
	if (!Top10List.dwUserCode[0])
		return FALSE;

	// Procura o slot disponível para a roleta
	short Slot = -1;

	for (int x = 0; x < 10; x++)
	{
		if (vSorteios[x].size() == 0)
		{
			Slot = x;
			break;
		}
	}

	// Nenhum slot disponível para a roleta
	if (Slot == -1)
		return FALSE;

	auto mob = find(vMobs.begin(), vMobs.end(), Monster->dwObjectSerial);

	if (mob == vMobs.end())
	{
		RoletaPckg Top10;
		vMobs.push_back(Monster->dwObjectSerial);

		Top10.code = ROLETA_SEND_TOPPLAYERS;
		Top10.size = sizeof(RoletaPckg);

		rsPLAYINFO* Players[10] = { 0 };
		Top10.totalDamage = 0;
		Top10.participantes = 0;

		for (int cnt = 0; cnt < 10; cnt++)
		{
			Players[cnt] = { 0 };

			if (Top10List.dwUserCode[cnt])
			{
				Players[cnt] = srFindUserFromSerial(Top10List.dwUserCode[cnt]);

				if (Players[cnt])
				{
					strcpy_s(Top10.Players[cnt].szName, Players[cnt]->szName);
					Top10.Players[cnt].Damage = Top10List.Damage[cnt];
					Top10.Players[cnt].Counter = Top10List.Counter[cnt];
					Top10.Players[cnt].dwUpdateTime = Top10List.dwUpdateTime;
					Top10.Players[cnt].jobCode = static_cast<short>(Players[cnt]->smCharInfo.JOB_CODE);
					Top10.Players[cnt].x = Players[cnt]->Position.x;
					Top10.Players[cnt].y = Players[cnt]->Position.y;
					Top10.Players[cnt].z = Players[cnt]->Position.z;

					Top10.totalDamage += Top10List.Damage[cnt];
					Top10.participantes += 1;
				}
			}
		}

		int chance = 100 / Top10.participantes;

		for (int x = 0; x < 10; x++)
		{
			if (Players[x] && Players[x]->lpsmSock)
			{
				Top10.Players[x].chance = chance;
				Players[x]->lpsmSock->Send((char*)&Top10, Top10.size, TRUE);
			}
		}

		std::thread TRoleta(ThreadRoleta, Top10, Monster, Slot);
		TRoleta.detach();
	}

	return TRUE;
}
