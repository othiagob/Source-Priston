#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\Database\SQLConnection.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <boost/algorithm/string.hpp>
#include <Chat/ChatServer.h>

#define MAX_PLAYERS 50

TRANS_CHATMESSAGE	TransChatMessage;
smTRANS_COMMAND		smTransCommand;

extern int SendToAll(char* Buff, int Size);
extern int SendToRic(char* Buff, int Size, int mapNum);
extern void LeIniStr(char* Section, char* Key, char* szFileIni, char* Var1);
extern rsPLAYINFO* srFindUserFromSerial(DWORD dwObjectSerial);
extern rsPLAYINFO* FindUserFromID(char* szID);

bool isArenaActive = FALSE;

char* noClan = "0";
int	PercentItem;

char szQuery[512];
char szName[32] = { 0 };
char clanName[32] = { 0 };

int tickCountArena = 0;
int tickCountBoss = 0;
int equipeNum = 0;
int equipeVencedora = 0;

smCHAR* Boss = 0;

std::vector<EventoArena::Reward*>  reWards;
EventoArena::playersFromTeamPckg teamPlayers;

extern int timesArena, daysArena[7], startTimesHour[10], startTimesMinutes[10], endTimesHour[10], endTimesMinutes[10];


void EventoArena::Jogadores(rsPLAYINFO* Player) {
	int flag = 0;

	if (jogadores >= MAX_PLAYERS)
	{
		UTILS->Alert(Player, "> A Arena está cheia!");
	}
	else {

		//Verifica se o player já não entrou na arena antes e coloca ele na mesma posição
		for (int x = jogadores; x > 0; x--)
		{
			if (JogadoresAtivos[x].Jogador->lpsmSock)
			{
				if (boost::iequals(Player->szName, JogadoresAtivos[x].Jogador->szName))
				{
					JogadoresAtivos[x].Jogador = Player;
					flag = 1;
				}
			}
		}

		if (flag == 0)
		{
			jogadores += 1;
			JogadoresAtivos[jogadores].Jogador = Player;
		}

		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480065;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;
		Player->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
		flag = 0;
	}
}

void EventoArena::imprimeJogadores(rsPLAYINFO* Player) 
{
	char string[128] = { 0 };
	int y = 0;

	for (int x = jogadores; x > 0; x--) {
		if (JogadoresAtivos[x].Jogador->lpsmSock) {
			sprintf_s(string, sizeof(string), "> Jogador %s - Posição %d", JogadoresAtivos[x].Jogador->szName, x);
			UTILS->Alert(Player, string);
			y++;
		}
		else {
			sprintf_s(string, sizeof(string), "> Posição %d sem jogador", x);
			UTILS->Alert(Player, string);
		}
	}

	sprintf_s(string, sizeof(string), "> Jogadores ativos: %d", y);
	UTILS->Alert(Player, string);
}


void EventoArena::verificaPlayerDC(rsPLAYINFO* Player) {

	for (int x = jogadores; x >= 0; x--)
	{
		if (boost::iequals(Player->szName, teamPlayers.pckgTeam[x].name))
		{
			Player->nEquipeArena = teamPlayers.pckgTeam[x].equipeNum;
			GameMasters::getInstance()->Packet(Player, 0x48480058, Player->nEquipeArena, jogadores, tickCountArena, 0);
			Player->lpsmSock->Send((char*)&teamPlayers, teamPlayers.size, TRUE);
			onArena(Player, Player, 1);
		}
	}

}

void EventoArena::TeleportPlayer(rsPLAYINFO* lpPlayInfo)
{
	if (isArenaActive)
	{
		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480058;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;
		lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
	}
	else
	{
		SERVERCHAT->SendChat(lpPlayInfo, EChatColor::CHATCOLOR_Error, "O evento Arena não está ativo no momento");
	}

}
void EventoArena::equipesArena(rsPLAYINFO* lpPlayInfo) {

	//Verifica certinho quantos se inscreveram e estão online no momento do sorteio
	for (int x = jogadores; x > 0; x--)
	{
		if (!JogadoresAtivos[x].Jogador || !JogadoresAtivos[x].Jogador->lpsmSock)
		{
			jogadores--;
		}
	}

	int maxEquipe = jogadores / 2;

	RankingPckg equipes[50];

	ZeroMemory(&teamPlayers, sizeof(playersFromTeamPckg));
	teamPlayers.size = sizeof(playersFromTeamPckg);
	teamPlayers.code = 0x48480064;

	ZeroMemory(&equipes, sizeof(RankingPckg));

	int randomPlayer = 0;
	int playerPos = 0;
	int w = 1;
	int y = 0;
	int sort[50] = { 0 };
	char equipeN[32] = { 0 };

	for (int x = 0; x < jogadores; x++) 
	{
		randomPlayer = rand() % jogadores + 1;

		for (int y = x; y >= 0; y--) {
			if (sort[y] == randomPlayer) {
				randomPlayer = rand() % jogadores + 1;
				y = x;
			}
		}
		sort[x] = randomPlayer;

		equipes[w].Ranking->equipeNum = x;
		equipes[w].Ranking->equipeScore = 0;

		if (equipes[w].Ranking->Players >= maxEquipe) w++;

		equipes[w].Ranking->Players += 1;

		if (JogadoresAtivos[randomPlayer].Jogador)
		{
			JogadoresAtivos[randomPlayer].Jogador->nEquipeArena = w;

			sprintf_s(teamPlayers.pckgTeam[y].id, sizeof(teamPlayers.pckgTeam[y].id), "%s", JogadoresAtivos[randomPlayer].Jogador->szID);
			sprintf_s(teamPlayers.pckgTeam[y].name, sizeof(teamPlayers.pckgTeam[y].name), "%s", JogadoresAtivos[randomPlayer].Jogador->szName);
			teamPlayers.pckgTeam[y].equipeNum = w;
			y++;

			sprintf_s(equipeN, sizeof(equipeN), "> Você está na Equipe %d", w);

			//Avisa o game de qual equipe o player é
			smTRANS_COMMAND	smTransCommand;
			smTransCommand.code = 0x48480058;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.LParam = w;
			smTransCommand.WParam = 0;
			smTransCommand.EParam = 0;
			JogadoresAtivos[randomPlayer].Jogador->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

			GameMasters::getInstance()->Alert(JogadoresAtivos[randomPlayer].Jogador, equipeN);
		}
	}

	SendToAll((char*)&teamPlayers, teamPlayers.size);

	tickCountArena = GetTickCount();
	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = 0x48480063;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.LParam = jogadores;
	smTransCommand.WParam = tickCountArena;
	smTransCommand.EParam = 0;
	SendToAll((char*)&smTransCommand, smTransCommand.size);
}


void EventoArena::endArena(rsPLAYINFO* lpPlayInfo)
{
	if (isArenaActive)
	{
		SERVERCHAT->SendChatAll(EChatColor::CHATCOLOR_Global, "Arena> A batalha terminou");

		isArenaActive = FALSE;
		arenaStage = 0;
		tickCountArena = 0;

		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480059;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;

		SendToAll((char*)&smTransCommand, smTransCommand.size);

		rewardArena();
	}
	else
	{
		UTILS->Alert(lpPlayInfo, "> O evento Arena não está ativo no momento");
	}
}


void EventoArena::startArena(rsPLAYINFO* lpPlayInfo)
{
	if (!isArenaActive)
	{
		SERVERCHAT->SendChatAll(EChatColor::CHATCOLOR_Global,"Arena> A entrada foi liberada, o evento começa em 5 minutos!");

		isArenaActive = TRUE;
		arenaStage = 1;
		jogadores = 0;

		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480060;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;

		SendToAll((char*)&smTransCommand, smTransCommand.size);

		auto db = SQLConnection::GetConnection(DATABASEID_EventosDB);

		// Limpa a tabela quando o evento inicia
		if (db && db->Open())
		{
			if (db->Prepare("DELETE FROM ArenaRanking"))
			{
				db->Execute();
			}

			db->Close();
		}
	}
	else
	{
		UTILS->Alert(lpPlayInfo, "O evento Arena já está ativo!");
	}

}

void EventoArena::onArena(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int isLogin)
{
	arenaStage = 2;
	int i = 0;
	int iScore = 0;
	RankingPckg topList;
	ZeroMemory(&topList, sizeof(RankingPckg));
	topList.size = sizeof(RankingPckg);
	topList.code = 0x48480057;

	auto db = SQLConnection::GetConnection(DATABASEID_EventosDB);

	if (isLogin == 0)
	{
		if (db->Open())
		{
			if (db->Prepare("SELECT Equipe, Pontos FROM ArenaRanking WHERE Equipe=? AND Pontos > 0"))
			{
				db->BindInputParameter(&lpPlayInfo->nEquipeArena, 1, PARAMTYPE_Integer);

				if (db->Execute())
				{
					if (db->GetData(1, PARAMTYPE_Integer, &equipeNum))
					{
						db->GetData(2, PARAMTYPE_Integer, &iScore);

						if (db->Prepare("UPDATE ArenaRanking SET Pontos=? WHERE Equipe=?")) {
							iScore++;
							db->BindInputParameter(&iScore, 1, PARAMTYPE_Integer);
							db->BindInputParameter(&lpPlayInfo->nEquipeArena, 2, PARAMTYPE_Integer);
							db->Execute();
						}

					}
				}
				else {
					if (db->Prepare("INSERT INTO ArenaRanking VALUES (?,1)"))
					{
						db->BindInputParameter(&lpPlayInfo->nEquipeArena, 1, PARAMTYPE_Integer);
						db->Execute();
					}
				}
			}
			db->Close();
		}
	}

	if (db->Open())
	{
		const char* const query = "SELECT TOP 2 * FROM ArenaRanking WHERE Pontos > 0 ORDER BY Pontos DESC";

		if (db->Prepare(query))
		{
			db->Execute(FALSE);

			while (db->NextRow())
			{
				db->GetData(1, PARAMTYPE_Integer, &topList.Ranking[i].equipeNum);
				db->GetData(2, PARAMTYPE_Integer, &topList.Ranking[i].equipeScore);
				i++;
			}
		}

		db->Close();
	}

	if (isLogin == 1)
	{
		lpPlayInfo->lpsmSock->Send((char*)&topList, topList.size, TRUE);
	}
	else if (isLogin == 0) {
		SendToAll((char*)&topList, topList.size);
	}
	else {
		SendToAll((char*)&topList, topList.size);
	}

}

extern smCHAR* OpenMonsterFromName(char* szName, int x, int y, int z, rsPLAYINFO* lpPlayInfo = 0);

void EventoArena::rewardArena()
{
	RankingPckg topList;
	ZeroMemory(&topList, sizeof(RankingPckg));

	int equipNum = 0;
	int pontuacao = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_EventosDB);

	if (db->Open())
	{
		const char* const query = "SELECT TOP 1 * FROM ArenaRanking WHERE Pontos > 0 ORDER BY Pontos DESC";

		if (db->Prepare(query))
		{
			if (db->Execute())
			{
				db->GetData(1, PARAMTYPE_Integer, &equipNum);
				db->GetData(2, PARAMTYPE_Integer, &pontuacao);
			}
		}

		db->Close();
	}

	if (pontuacao > 0)
	{
		SERVERCHAT->SendChatAllEx(EChatColor::CHATCOLOR_Global, "Arena> A Equipe %d venceu dessa vez!", equipNum);
		equipeVencedora = equipNum;
	}

	//Envia a premiação para o distribuidor

	tickCountBoss = GetTickCount();

	for (int x = 0; x <= jogadores; x++)
	{
		if (JogadoresAtivos[x].Jogador)
		{
			if (JogadoresAtivos[x].Jogador->nEquipeArena == equipeVencedora)
			{
				UTILS->Packet(JogadoresAtivos[x].Jogador, 0x50600021, tickCountBoss, equipeVencedora);
			}
			else
			{
				// Equipe perdedora volta para RIC
				if (JogadoresAtivos[x].Jogador->Position.Area == FIELD_ARENA)
				{
					UTILS->Packet(JogadoresAtivos[x].Jogador, 0x48480070);
				}
			}
		}
	}

	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = 0x48480069;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.LParam = 10;
	smTransCommand.WParam = 0;
	smTransCommand.EParam = 0;

	SendToAll((char*)&smTransCommand, smTransCommand.size);

	Boss = OpenMonsterFromName("Dark Stalker", -23877 * 256, 1371 * 256, 22248 * 256);

	arenaStage = 10;
}

void SendMessageToAll3(std::string sender, std::string message)
{
	SERVERCHAT->SendChatAllEx(EChatColor::CHATCOLOR_Global, "%s> %s", sender.c_str(), message.c_str());
}

int EventoArena::checkBoss(smCHAR* Monster)
{
	int timer = 0;

	if (!Monster)
		return FALSE;

	if (Boss != 0 && (Monster == Boss || Monster->dwObjectSerial == Boss->dwObjectSerial))
	{
		SendMessageToAll3("Arena", "A equipe vencedora derrotou o Boss!");
		arenaStage = 0;


		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480069;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;

		SendToAll((char*)&smTransCommand, smTransCommand.size);

		for (int x = 1; x <= jogadores; x++) {
			if (JogadoresAtivos[x].Jogador) {
				if (JogadoresAtivos[x].Jogador->nEquipeArena == equipeVencedora) 
				{
					UTILS->Alert(JogadoresAtivos[x].Jogador, "> Você será teletransportado em 15 segundos");
					UTILS->Packet(JogadoresAtivos[x].Jogador, 0x50600022);
				}
			}
		}

		equipeVencedora = 0;
	}

	return TRUE;
}


