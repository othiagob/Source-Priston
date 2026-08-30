#include "ServerCommand.h"
#include <Utils/strings.h>
#include <Chat/ChatServer.h>
#include <GlobalsShared.h>
#include <VIP/Vip.h>
#include <Party/CPartyHandler.h>
#include <zlib.h>
#include <Database/SQLConnection.h>
#include "../SrcServer/onserver.h"

extern Events::Questions* QuestionEvent;
extern char* rsGetWord(char* q, char* p);
extern void LeIniStr(char* Section, char* Key, char* szFileIni, char* Var1);

extern int CreateCommandItem(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, char* szItem);
extern int srSetItemFromCode(psITEM* lpsItem, char* szCoed);
extern int	rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);
extern int	rsRecordAdminCommand(rsPLAYINFO* lpPlayInfo, char* szChatCommand, int Level);
extern smCHAR* OpenMonsterFromName(char* szName, int x, int y, int z, rsPLAYINFO* lpPlayInfo = 0);
extern INT64 GetExpFromLevel(int iLevel);


extern float Multiplicador[151];

CServerCommand::CServerCommand()
{
}

CServerCommand::~CServerCommand()
{
}

BOOL CServerCommand::OnPlayerCommand(rsPLAYINFO* pcUser, char* pszBuff)
{
	int iLen = 0, iLen2 = 0;

	char szCommandParam1[256] = { 0 };
	char szCommandParam2[256] = { 0 };

	// Mensagens privadas
	if (pszBuff[1] == ':' || pszBuff[1] == ';')
	{
		char* lpBuff = pszBuff + 2;
		lpBuff = rsGetWord(szCommandParam2, lpBuff);

		if (lpBuff)
		{
			rsPLAYINFO* pcUser2 = FindUserFromName(szCommandParam2);

			if (pcUser2)
			{
				rsGetWord(szCommandParam1, lpBuff);

				if (szCommandParam1[0])
					SERVERCHAT->SendWhisper(pcUser, pcUser2, lpBuff);
				else
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "%s está online agora %s", pcUser2->smCharInfo.szName, pcUser2->WhisperMode ? "(MP Desabilitadas)" : "");
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s está offline ou não foi encontrado!", szCommandParam2);
			}
		}
	}

	if (pszBuff[0] == '@')
	{
		if (pcUser)
		{
			if ((pcUser->bParty && pcUser->psParty) && pcUser->psParty->pcLeader)
			{
				std::string str(pszBuff);
				str.erase(0, 1);

				PARTYHANDLER->SendChatParty(pcUser, FormatString("%s: %s", pcUser->smCharInfo.szName, str.c_str()), CHATCOLOR_Party);
			}
		}

		return TRUE;
	}
	else if (pszBuff[0] == '#')
	{
		if (pcUser)
		{
			if ((pcUser->bParty && pcUser->psParty) && pcUser->psParty->pcLeader && pcUser->psParty->iRaidCount > 0)
			{
				std::string str(pszBuff);
				str.erase(0, 1);

				PARTYHANDLER->SendChatRaid(pcUser, FormatString("%s: %s", pcUser->smCharInfo.szName, str.c_str()), CHATCOLOR_Raid);
			}
		}
	}

	// Clan
	if (iLen = ChatCommand("/CLAN>", pszBuff))
	{
		if (pcUser->dwClanCode != 0)
		{
			BOOL bFound = FALSE;

			for (int i = iLen; i < 100; i++)
			{
				if (pszBuff[i] == 0)
					break;

				if (pszBuff[i] != ' ')
				{
					iLen = i;
					bFound = TRUE;
					break;
				}
			}

			if (bFound)
			{
				EChatColor eColor = CHATCOLOR_Clan;

				//if (SERVERCHARACTER->IsCharacterLeader(pcUser))
					//eColor = CHATCOLOR_ClanLeader;

				SERVERCHAT->SendChatAllUsersInClan(pcUser, eColor, "%s: %s", pcUser->smCharInfo.szName, pszBuff + iLen);
				SERVERCHAT->SendChatEx(pcUser, eColor, "%s: %s", pcUser->smCharInfo.szName, pszBuff + iLen);
			}

		}

		return TRUE;
	}

	// Trade
	else if (ChatCommand("/TRADE>", pszBuff))
	{
		SERVERCHAT->SendChatTrade(pcUser, pszBuff);

		if (QuestionEvent->getStage() == 2)
		{
			// Pega a resposta digitada e verifica se é a correta
			std::string msg = pszBuff + 7;

			QuestionEvent->CheckAnswers(pcUser, trim(msg));
		}
	}

	else if (ChatCommand("//PARTY", pszBuff) || ChatCommand("//party", pszBuff))
	{
		if (rsServerConfig.DisableParty)
			return FALSE;

		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcParty = FindUserFromName(szCommandParam1);

			if (pcParty && pcParty != pcUser)
			{
				PARTYHANDLER->HandlePacket(pcUser, &PacketRequestParty(pcParty->dwObjectSerial));
			}
		}
	}
	else if (ChatCommand("//raid", pszBuff) || ChatCommand("//raid", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcRaid = FindUserFromName(szCommandParam1);

			if (pcRaid && pcRaid != pcUser)
			{
				PARTYHANDLER->HandlePacket(pcUser, &PacketRequestRaid(pcRaid->dwObjectSerial));
			}
		}
	}

	else if (ChatCommand("/enable_whisper", pszBuff))
	{
		pcUser->WhisperMode = FALSE;
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Mensagens privadas habilitadas.");
	}

	else if (ChatCommand("/disable_whisper", pszBuff))
	{
		pcUser->WhisperMode = TRUE;
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Mensagens privadas desabilitadas.");
	}

	else if (ChatCommand("/premium", pszBuff))
	{
		if (pcUser && pcUser->vipLevel > 0)
		{
			Vip::GetInstance()->OpenNpc(pcUser, 0);
		}
	}
	else if (ChatCommand("/repot", pszBuff))
	{
		if (pcUser && pcUser->vipLevel > 0)
		{
			Vip::GetInstance()->OpenNpc(pcUser, 1);
		}
	}
	else if (ChatCommand("/bau", pszBuff))
	{
		if (pcUser && pcUser->vipLevel > 0)
		{
			Vip::GetInstance()->OpenNpc(pcUser, 2);
		}
	}

	return TRUE;
}

void CServerCommand::OnGameMasterAdminCommand(rsPLAYINFO* pcUser, char* pszBuff)
{
	char szCommandParam1[256] = { 0 };
	char szCommandParam2[256] = { 0 };
	char szCommandParam3[256] = { 0 };
	char szCommandParam4[256] = { 0 };
	char szCommandParam5[256] = { 0 };
	char szCommandParam6[256] = { 0 };

	// Cria itens no chão
	if (ChatCommand("/drop", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			int nItens = atoi(szCommandParam2);

			if (nItens > 0)
			{
				if (nItens > 10) nItens = 10;

				while (nItens > 0)
				{
					if (CreateCommandItem(pcUser, 0, szCommandParam1) == FALSE)
					{
						SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> '%s' não localizado.", szCommandParam1);
						break;
					}

					nItens--;
				}

				if (!nItens)
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Blue, "> '%s' (x%s) Dropado no chão.", szCommandParam1, szCommandParam2);
			}
			else
			{
				if (CreateCommandItem(pcUser, 0, szCommandParam1) == TRUE)
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Blue, "> '%s' Dropado no chão.", szCommandParam1);
				}
				else
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> '%s' não localizado.", szCommandParam1);
				}
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /drop <WA101> 1 (MAX=10)");
	}

	// Cria o item perfeito com um nível de aging específico no inventário
	else if (ChatCommand("/aging", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			int NivelAging = atoi(szCommandParam2);

			if (NivelAging <= 22)
			{
				psITEM* lpsItem;
				lpsItem = new psITEM;

				if (CreateItemPerf(lpsItem, szCommandParam1, pcUser->smCharInfo.JOB_CODE) == TRUE)
				{
					SetAgingItem(&lpsItem->ItemInfo, NivelAging);

					TRANS_ITEMINFO TransItemInfo;
					TransItemInfo.code = smTRANSCODE_PUTITEM;
					TransItemInfo.size = sizeof(TRANS_ITEMINFO);
					memcpy(&TransItemInfo.Item, &lpsItem->ItemInfo, sizeof(sITEMINFO));
					rsRegist_ItemSecCode(pcUser, &TransItemInfo, 1);

					pcUser->lpsmSock->Send((char*)&TransItemInfo, TransItemInfo.size, TRUE);
				}

				delete lpsItem;
			}
			else
			{
				SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Nível máximo = +22!");
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /aging <Codigo> <Aging>");
	}

	// Inicia BC
	else if (ChatCommand("/start_castle", pszBuff))
	{
		rsStart_CombatCastle();
	}

	// Termina BC
	else if (ChatCommand("/end_castle", pszBuff))
	{
		rsEnd_CombatCastle(0);
	}

	// Deixa bc Fácil
	else if (ChatCommand("/easy_castle", pszBuff))
	{
		rsEasy_CastleMode();;
	}
	// Exibe id pelo nick
	else if (ChatCommand("/userid", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcID = FindUserFromName(szCommandParam1);

			if (!pcID)
				pcID = FindUserFromName2(szCommandParam1);

			if (pcID)
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Nick: %s -> ID: %s", szCommandParam1, pcID->szID);
			}
			else
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Nick: %s -> ID não encontrada", szCommandParam1);
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /userid <nick>");
		}
	}

	// Upa de level
	else if (ChatCommand("/giveexp", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			// Verifica se o personagem já atingiu o nível máximo
			if (pcUser->smCharInfo.Level >= g_LevelFinal)
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Personagem já atingiu o nível máximo %d!", g_LevelFinal);
				return;
			}

			INT64 exp = atoll(szCommandParam1);

			UTILS->AddExpToPlayer(pcUser, exp);
			pcUser->LastExp += exp;
			pcUser->dwGameServerExp[0] += exp;

			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Blue, "> '%I64i' XP Adicionado.", exp);

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /giveexp <xp>");
	}

	// Desliga o servidor
	else if (ChatCommand("/shutdown", pszBuff))
	{
		rsShutDown();
	}

	// Recarrega as habilidades no .ini
	else if (ChatCommand("/reloadSkill", pszBuff))
	{
		skill.readAll();
		skill.sendAll();
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Habilidades Recarregadas!");
	}

	// Abre a arena
	else if (ChatCommand("/start_arena", pszBuff))
	{
		EventoArena::GetInstance()->startArena(pcUser);
	}

	// Entra na arena
	else if (ChatCommand("/enter_arena", pszBuff))
	{
		EventoArena::GetInstance()->Jogadores(pcUser);
	}

	// Inicia a arena
	else if (ChatCommand("/equipes_arena", pszBuff))
	{
		EventoArena::GetInstance()->equipesArena(pcUser);
	}

	// Entra em sod
	else if (ChatCommand("/sod_enter", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			int room = atoi(szCommandParam1);

			pcUser->sEventParam[0] = room;
			rsEnterHardCoreEvent(pcUser, -1);
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /sod_enter <0-4>");
		}
	}

	// Mostra ranking de sod
	else if (ChatCommand("/sod_view", pszBuff))
	{
		rsSod_GetRealTimeScore(pcUser, 0);
	}

	// Verifica o bosstime
	else if (ChatCommand("/get_bosstime", pszBuff))
	{
		extern int OpenBossTimeMin;
		SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> BossTime: xx%d", OpenBossTimeMin);
	}

	// Seta o bosstime
	else if (ChatCommand("/set_bosstime", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			extern int OpenBossTimeMin;
			int newBossTime = atoi(szCommandParam1);

			if (newBossTime > 0)
				OpenBossTimeMin = newBossTime;

			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Novo BossTime: xx%d", OpenBossTimeMin);
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /set_bosstime <Min>");
		}
	}

	// Finaliza a arena
	else if (ChatCommand("/end_arena", pszBuff))
	{
		EventoArena::GetInstance()->endArena(pcUser);
	}

	// Adiciona coins em alguma ID
	else if (ChatCommand("/addcoin", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			int nCoins = atoi(szCommandParam2);

			if (nCoins > 0)
			{
				NewShop::GetInstance()->addCoinsToPlayer(pcUser, szCommandParam1, nCoins, 0, WhereCoinsComeFrom::FROM_COMMAND);
			}
			else
			{
				SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Insira uma quantidade válida!");
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /addcoin <ID> 100");
	}

	// Seta o evento de EXP
	else if (ChatCommand("/set_eventexp", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			extern int eventoxp;
			eventoxp = atoi(szCommandParam1);
			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Evento EXP alterado: x%d", eventoxp);
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /set_eventexp <X>");
	}

	// Retorna o evento ativo de EXP
	else if (ChatCommand("/get_eventexp", pszBuff))
	{
		extern int eventoxp;
		SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Evento EXP ativo: x%d", eventoxp);
	}

	// Seta o evento de drops
	else if (ChatCommand("/set_eventdrop", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			extern int eventodrop;
			eventodrop = atoi(szCommandParam1);
			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Evento DROP alterado: x%d", eventodrop);
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /set_eventdrop <X>");
	}

	// Retorna o evento ativo de Drop
	else if (ChatCommand("/get_eventdrop", pszBuff))
	{
		extern int eventodrop;
		SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Evento DROP ativo: x%d", eventodrop);
	}

	// Recarrega os desafios ao relogar
	else if (ChatCommand("/update_quest", pszBuff))
	{
		Quest::GetInstance()->updateQuests = true;
		Quest::GetInstance()->SendAllQuests(pcUser);
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Desafios recarregados, é necessário relogar.");
	}

	// Bane uma ID online
	else if (ChatCommand("/ban", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcBan = FindUserFromName(szCommandParam1);

			if (!pcBan)
				pcBan = FindUserFromName2(szCommandParam1);

			if (pcBan)
			{
				char typeban[40] = { 0 };
				sprintf_s(typeban, sizeof(typeban), "Conta banida");
				UTILS->Ban(pcBan, true, pcBan->szID, typeban);
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> ID: '%s' banida com sucesso.", szCommandParam1);
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> ID: '%s' não localizada.", szCommandParam1);
			}

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /ban <Nick>");
	}

	// Bane uma ID offline
	else if (ChatCommand("/forceban", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			char typeban[40] = { 0 };
			sprintf_s(typeban, sizeof(typeban), "Conta banida");
			UTILS->Ban(pcUser, false, szCommandParam1, typeban);
			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> ID '%s' banida. (caso exista)", szCommandParam1);

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /forceban <ID>");
	}

	// Desbane uma ID
	else if (ChatCommand("/desban", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			UTILS->UnBan(szCommandParam1);
			SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> ID '%s' desbanida.", szCommandParam1);

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /desban <ID>");
	}

	// Reseta o rankign PVP
	else if (ChatCommand("/reset_manualpvp", pszBuff))
	{
		PVPRanking::GetInstance()->ResetRanking(pcUser);
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Ranking PVP Resetado!");
	}

	// Evento aging free ativo
	else if (ChatCommand("/ativar_agingfree", pszBuff))
	{
		extern int AgingEvento;
		AgingEvento = TRUE;
		UTILS->PacketToAll(smTRANSCODE_AGINGFREE, 0, 1);
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Evento Aging free ATIVO!");
	}

	// Evento aging free desativado
	else if (ChatCommand("/desativar_agingfree", pszBuff))
	{
		extern int AgingEvento;
		AgingEvento = FALSE;
		UTILS->PacketToAll(smTRANSCODE_AGINGFREE, 0, 0);
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Evento aging FREE DESATIVADO!");
	}

	// Atualiza ranking ingame
	else if (ChatCommand("/updateranking", pszBuff))
	{
		TopRanking::GetInstance()->update = true;
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Ranking atualizado.");
	}

	// Mostra evento donate shopcoin
	else if (ChatCommand("/show_donate", pszBuff))
	{
		SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Evento ativo: x%d.", NewShop::GetInstance()->isDoubleDonation);
	}

	// Adiciona um novo indicador no mapa
	else if (ChatCommand("/add_indicador", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1) &&
			GetParameterString(pszBuff, 2, szCommandParam2) &&
			GetParameterString(pszBuff, 3, szCommandParam3) &&
			GetParameterString(pszBuff, 4, szCommandParam4) &&
			GetParameterString(pszBuff, 5, szCommandParam5) &&
			GetParameterString(pszBuff, 6, szCommandParam6))
		{
			int iMapID = atoi(szCommandParam1);
			auto iType = (MapIndicator::Type)atoi(szCommandParam2);
			int iValue = atoi(szCommandParam3);
			int iCompactPosX = atoi(szCommandParam4);
			int iCompactPosZ = atoi(szCommandParam5);
			int iAngleY = atoi(szCommandParam6);

			if (AddMapIndicator(pcUser, iMapID, iType, iValue, iCompactPosX, iCompactPosZ, iAngleY))
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Indicador adicionado na posiçao atual (Tipo = %d)", iType);
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /add_indicador <Field> <Tipo> <Field2> <X> <Z> <Angulo>");
	}
	// Remove o indicador próximo da posição atual
	else if (ChatCommand("/remove_indicador", pszBuff))
	{
		if (RemoveMapIndicatorNearMe(pcUser))
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Indicador próximo removido!");
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Nenhum indicador encontrado em um raio de 20 metros.");
		}
	}
	else if (ChatCommand("/reload_indicador", pszBuff))
	{
		LoadIndicators();
		SendMapIndicators(pcUser, pcUser->Position.Area, TRUE);
		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Indicadores recarregados!");
	}

	// Faz a leitura do .ini multiplicador de exp novamente
	else if (ChatCommand("/reload_exp", pszBuff))
	{
		ZeroMemory(Multiplicador, sizeof(Multiplicador));

		char szBuf[10] = { 0 };
		char szBuf2[10] = { 0 };

		for (int x = 1; x <= 150; x++)
		{
			STRINGFORMAT(szBuf, "Nv[%d]", x);
			LeIniStr("ExpConfig", szBuf, "Server\\Config\\ExpManager.ini", szBuf2);
			Multiplicador[x] = atof(szBuf2);
		}

		SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Multiplicadores Recarregados!");
	}

	rsRecordAdminCommand(pcUser, pszBuff, 4);
}

void CServerCommand::OnGameMasterEventCommand(rsPLAYINFO* pcUser, char* pszBuff)
{
	char szCommandParam1[256] = { 0 };
	char szCommandParam2[256] = { 0 };
	char szCommandParam3[256] = { 0 };

	if (ChatCommand("/>", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
			SERVERCHAT->SendChatAllEx(CHATCOLOR_Global, "%s> %s", pcUser->smCharInfo.szName, pszBuff + 2);
	}

	else if (ChatCommand("/+", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
			SERVERCHAT->SendChatAllEx(CHATCOLOR_Global, "GM> %s", pszBuff + 2);
	}

	else if (ChatCommand("/inv", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			int nItens = atoi(szCommandParam2);

			if (nItens > 0)
			{
				if (nItens > 10) nItens = 10;

				while (nItens > 0)
				{
					if (CreateCommandItem(pcUser, pcUser, szCommandParam1) == FALSE)
					{
						SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> '%s' não localizado.", szCommandParam1);
						break;
					}

					nItens--;
				}

				if (!nItens)
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Blue, "> '%s' (x%s) criado no inventário.", szCommandParam1, szCommandParam2);
			}
			else
			{
				if (CreateCommandItem(pcUser, pcUser, szCommandParam1) == TRUE)
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Blue, "> '%s' criado no inventário.", szCommandParam1);
				}
				else
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> '%s' não localizado.", szCommandParam1);
				}
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /inv <WA101> 1 (MAX=10)");
	}

	else if (ChatCommand("/monster", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			int nMobs = atoi(szCommandParam2);
			smCHAR* lpChar = nullptr;

			lpChar = OpenMonsterFromName(szCommandParam1, pcUser->Position.x, pcUser->Position.y, pcUser->Position.z);

			if (lpChar)
			{
				if (nMobs > 10) nMobs = 10;

				if (nMobs > 0)
				{
					do
					{
						int x = rand() % (128 * fONE);
						int z = rand() % (128 * fONE);
						x -= 64 * fONE;
						z -= 64 * fONE;

						OpenMonsterFromName(szCommandParam1, pcUser->Position.x + x, pcUser->Position.y, pcUser->Position.z + z);

						nMobs--;
					} while (nMobs > 1);
				}
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s Monstro não localizado.", szCommandParam1);
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /monster <Bargon> 5 (MAX=10)");
	}

	else if (ChatCommand("/mymonster", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			smCHAR* lpChar = nullptr;

			lpChar = OpenMonsterFromName(szCommandParam1, pcUser->Position.x, pcUser->Position.y, pcUser->Position.z, pcUser);

			if (!lpChar)
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s Monstro não localizado.", szCommandParam1);

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /mymonster <Bargon>");
	}

	else if (ChatCommand("/event_monster", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);
			GetParameterString(pszBuff, 3, szCommandParam3);

			int Time = atoi(szCommandParam2);
			int Rate = atoi(szCommandParam3);

			if (Time == 0) Time = 1;
			if (Rate == 0) Rate = 50;
			else if (Rate > 100) Rate = 100;

			smCHAR_INFO* lpChar = FindMonsterFromName(szCommandParam1);
			STG_AREA* lpStgArea = FindStageField(pcUser->Position.x, pcUser->Position.z);

			if (lpChar)
			{
				if (lpStgArea && lpStgArea->Event_dwMonsterTime == 0)
				{
					lpStgArea->Event_lpChrMonster = lpChar;
					lpStgArea->Event_dwMonsterTime = dwPlayServTime + Time * 1000 * 60;
					lpStgArea->Event_MonsterPercentage = Rate * 10;

					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Monstro: (%s) Tempo: (%d)Min -> Rate: (%d%%). (Ativado no mapa com sucesso)", szCommandParam1, Time, Rate);
				}
				else if (lpStgArea && lpStgArea->Event_dwMonsterTime > 0)
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> Evento já ativo no mapa: Monstro: (%s) Tempo restante: (%dMin) Rate: (%d%%))", lpStgArea->Event_lpChrMonster, ((lpStgArea->Event_dwMonsterTime - dwPlayServTime) / 1000) / 60, (lpStgArea->Event_MonsterPercentage / 10));
				}
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s Monstro não localizado.", szCommandParam1);
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /event_monster <Bargon> <(Min)> <Rate>");
	}

	else if (ChatCommand("/cancel_event_monster", pszBuff))
	{
		STG_AREA* lpStgArea = FindStageField(pcUser->Position.x, pcUser->Position.z);

		if (lpStgArea)
		{
			if (lpStgArea->Event_dwMonsterTime)
			{
				lpStgArea->Event_lpChrMonster = 0;
				lpStgArea->Event_dwMonsterTime = 0;
				lpStgArea->Event_MonsterPercentage = 0;

				SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Evento no mapa cancelado com sucesso.");
			}
		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Nenhum evento ativo no mapa.");
	}

	else if (ChatCommand("/set_absorb", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			smCHAR* lpChar = srFindCharFromSerial(atoi(szCommandParam1));

			if (lpChar)
			{
				int abs = atoi(szCommandParam2);

				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> (%s) %d -> %d OK", lpChar->smCharInfo.szName, lpChar->smCharInfo.Absorption, abs);
				lpChar->smCharInfo.Absorption = abs;
			}

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /set_absorb <x>");
	}

	else if (ChatCommand("/talk", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			GetParameterString(pszBuff, 2, szCommandParam2);

			smCHAR* lpChar = srFindCharFromSerial(atoi(szCommandParam1));

			if (lpChar)
			{
				SERVERCHAT->SendUserBoxChatRange(lpChar->pX, lpChar->pZ, 0, lpChar->dwObjectSerial, szCommandParam2);
			}

		}
		else
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /talk <mensagem>");
	}

	else if (ChatCommand("/hide", pszBuff))
	{
		pcUser->HideMode = TRUE;
	}

	else if (ChatCommand("/show", pszBuff))
	{
		pcUser->HideMode = FALSE;
	}

	else if (ChatCommand("/user", pszBuff))
	{
		extern int PlayerCount;
		SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "Usuários online: %d", PlayerCount);
	}

	else if (ChatCommand("/kick", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcKick = FindUserFromName(szCommandParam1);

			if (!pcKick)
				pcKick = FindUserFromName2(szCommandParam1);

			if (pcKick && pcKick->lpsmSock)
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s desconectado com sucesso.", pcKick->smCharInfo.szName);
				DisconnectUser(pcKick->lpsmSock);
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s não encontrado.", szCommandParam1);
			}
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /kick <nick>");
		}
	}

	else if (ChatCommand("/kill", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcKick = FindUserFromName(szCommandParam1);

			if (!pcKick)
				pcKick = FindUserFromName2(szCommandParam1);

			if (pcKick && pcKick->lpsmSock)
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s morto.", pcKick->smCharInfo.szName);
				UTILS->Packet(pcKick, smTRANSCODE_DEAD_PLAYER);
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s não encontrado.", szCommandParam1);
			}
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /kill <nick>");
		}
	}

	else if (ChatCommand("/call", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcCall = FindUserFromName(szCommandParam1);

			if (!pcCall)
				pcCall = FindUserFromName2(szCommandParam1);

			if (pcCall && pcCall->lpsmSock)
			{
				STG_AREA* lpStgArea = FindStageField(pcUser->Position.x, pcUser->Position.z);

				if (lpStgArea)
				{
					UTILS->Packet(pcCall, smTRANSCODE_WARPFIELD, pcUser->Position.z, lpStgArea->lpField->FieldCode, 0, pcUser->Position.x);
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s veio até você.", pcCall->smCharInfo.szName);
				}
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s não encontrado.", szCommandParam1);
			}
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /call <nick>");
		}
	}

	else if (ChatCommand("/where", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcWhere = FindUserFromName(szCommandParam1);

			if (!pcWhere)
				pcWhere = FindUserFromName2(szCommandParam1);

			if (pcWhere && pcWhere->lpsmSock)
			{
				STG_AREA* lpStgArea = FindStageField(pcWhere->Position.x, pcWhere->Position.z);

				if (lpStgArea)
				{
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s está no mapa %s.", pcWhere->smCharInfo.szName, lpStgArea->lpField->szName);
				}
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s não encontrado.", szCommandParam1);
			}
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /where <nick>");
		}
	}

	else if (ChatCommand("/near", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			rsPLAYINFO* pcNear = FindUserFromName(szCommandParam1);

			if (!pcNear)
				pcNear = FindUserFromName2(szCommandParam1);

			if (pcNear && pcNear->lpsmSock)
			{
				STG_AREA* lpStgArea = FindStageField(pcNear->Position.x, pcNear->Position.z);

				if (lpStgArea)
				{
					UTILS->Packet(pcUser, smTRANSCODE_WARPFIELD, pcNear->Position.z, lpStgArea->lpField->FieldCode, 0, pcNear->Position.x);
					SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> teletransportado para a localização de %s.", pcNear->smCharInfo.szName);
				}
			}
			else
			{
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %s não encontrado.", szCommandParam1);
			}
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /near <nick>");
		}
	}

	else if (ChatCommand("/dropmoney", pszBuff))
	{
		if (GetParameterString(pszBuff, 1, szCommandParam1))
		{
			int money = atoi(szCommandParam1);

			if (money > MAX_GOLD_TIER3)
				money = MAX_GOLD_TIER3;

			psITEM* pGold = new psITEM;
			ZeroMemory(pGold, sizeof(psITEM));
			STRINGFORMAT(pGold->ItemInfo.ItemName, "%dOuro", money);
			pGold->ItemInfo.Money = money;
			pGold->ItemInfo.CODE = sinGG1 | sin01;
			ReformItem(&pGold->ItemInfo);

			STG_AREA* lpStgArea = FindStageField(pcUser->Position.x, pcUser->Position.z);

			if (lpStgArea)
			{
				pGold->state = TRUE;
				lpStgArea->AddItem(pGold, pcUser->Position.x >> FLOATNS, pcUser->Position.y >> FLOATNS, pcUser->Position.z >> FLOATNS);
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> %d ouro dropado no chão.", money);
			}

			if (pGold)
				delete pGold;
		}
		else
		{
			SERVERCHAT->SendChat(pcUser, CHATCOLOR_Error, "> Uso: /dropmoney <10000>");
		}
	}

	rsRecordAdminCommand(pcUser, pszBuff, 2);
}

BOOL CServerCommand::OnGameMasterCommand(rsPLAYINFO* pcUser, char* pszBuff)
{
	if (ChatCommand("/gm", pszBuff))
	{
		extern std::vector<GameMasters*> vGameMasters;

		for (size_t i = 0; i < vGameMasters.size(); i++)
		{
			GameMasters* p = vGameMasters[i];

			if (lstrcmpA(pcUser->szID, p->szAccountName) == 0)
			{
				pcUser->AdminMode = p->iGameLevel;
				SERVERCHAT->SendChatEx(pcUser, CHATCOLOR_Error, "> GM Level [%d] Ativado.", p->iGameLevel);
				UTILS->Packet(pcUser, smTRANSCODE_GAMEMASTERMODE);
				break;
			}
		}
	}


	if (pcUser->AdminMode)
		OnGameMasterEventCommand(pcUser, pszBuff);
	if (pcUser->AdminMode > 2)
		OnGameMasterAdminCommand(pcUser, pszBuff);

	return TRUE;
}

void CServerCommand::OnReceiveChat(rsPLAYINFO* pcUser, PacketChat* psPacket)
{
	if (psPacket->szChat[0] != '/' && psPacket->szChat[0] != '@' && psPacket->szChat[0] != '#')
	{
		// Emoticons
		if (psPacket->szChat[0] == 'e' && isdigit(psPacket->szChat[1]) && STRLEN(psPacket->szChat) < 4)
		{
			SERVERCHAT->SendUserBoxChatRange(pcUser->Position.x, pcUser->Position.z, 0, pcUser->dwObjectSerial, "%s: %s", pcUser->smCharInfo.szName, psPacket->szChat);
		}
		else
		{
			STRINGCOPY(pcUser->szChatMessage, psPacket->szChat);
		}
	}
	else
	{
		OnPlayerCommand(pcUser, psPacket->szChat);
		OnGameMasterCommand(pcUser, psPacket->szChat);
	}
}


SYSTEMTIME Time_tToSystemTime(time_t t)

{

	tm temptm = *localtime(&t);


	SYSTEMTIME st = { 1900 + temptm.tm_year,

								   1 + temptm.tm_mon,

								   temptm.tm_wday,

								   temptm.tm_mday,

								   temptm.tm_hour,

								   temptm.tm_min,

								   temptm.tm_sec,

								   0 };

	return st;

}

void CServerCommand::SendMapIndicators(rsPLAYINFO* pcUser, int iMapID, BOOL bForceSend)
{
	if (iMapID < 0 && iMapID >= 60)
		return;

	auto iter = mMapIndicators.find(iMapID);

	if (iter == mMapIndicators.end())
	{
		if (bForceSend)
		{
			PacketMapIndicators mPacket;
			ZeroMemory(&mPacket, sizeof(PacketMapIndicators));
			mPacket.code = PACKET_GET_INDICATORS;
			mPacket.size = sizeof(PacketMapIndicators) - ((MAX_MAP_INDICATORS) * sizeof(MapIndicator));
			mPacket.iMapID = iMapID;
			mPacket.iCount = 0;

			SENDPACKET(pcUser, (char*)&mPacket, mPacket.size, TRUE);
		}

		return;
	}

	SENDPACKET(pcUser, (char*)&iter->second, iter->second.size, TRUE);
}

void CServerCommand::LoadIndicators()
{
	mMapIndicators.clear();

	auto pcDB = SQLCONNECTION(DATABASEID_GameServer);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("SELECT Field, Indicator, NextMapID, PosX, PosZ, Angle FROM FieldIndicators ORDER BY Field"))
		{
			if (pcDB->Execute())
			{
				int iPrevMapID = -1;

				while (TRUE)
				{
					int iMapID;

					pcDB->GetData(1, PARAMTYPE_Integer, &iMapID);

					if (iPrevMapID != iMapID)
					{
						PacketMapIndicators mPacket;
						ZeroMemory(&mPacket, sizeof(PacketMapIndicators));
						mMapIndicators.insert({ iMapID, mPacket });

						mMapIndicators[iMapID].iMapID = iMapID;

						iPrevMapID = iMapID;
					}

					if (mMapIndicators[iMapID].iCount >= MAX_MAP_INDICATORS)
					{
						printf("Indicadores excedidos para o mapa: %d", iMapID);
						continue;
					}

					auto instance = &mMapIndicators[iMapID].mapIndicators[mMapIndicators[iMapID].iCount];

					int iIndicatorType;

					pcDB->GetData(2, PARAMTYPE_Integer, &iIndicatorType);
					pcDB->GetData(3, PARAMTYPE_Integer, &instance->iValue);
					pcDB->GetData(4, PARAMTYPE_Integer, &instance->iCompactPosX);
					pcDB->GetData(5, PARAMTYPE_Integer, &instance->iCompactPosZ);
					pcDB->GetData(6, PARAMTYPE_Integer, &instance->iAngleY);

					instance->eType = (MapIndicator::Type)iIndicatorType;

					mMapIndicators[iMapID].iCount++;

					if (!pcDB->NextRow())
						break;
				}
			}
		}

		pcDB->Close();
	}

	for (auto& indicator : mMapIndicators)
	{
		indicator.second.code = PACKET_GET_INDICATORS;
		indicator.second.size = sizeof(PacketMapIndicators) - ((MAX_MAP_INDICATORS - indicator.second.iCount) * sizeof(MapIndicator));
	}

}

BOOL CServerCommand::AddMapIndicator(rsPLAYINFO* pcUser, int iMapID, MapIndicator::Type eMapIndicatorType, int iValue, int iCompactPosX, int iCompactPosZ, int iAngle)
{
	if (!pcUser || !pcUser->lpsmSock)
		return FALSE;

	auto pcDB = SQLCONNECTION(DATABASEID_GameServer);

	if (pcDB && pcDB->Open())
	{
		if (pcDB->Prepare("INSERT INTO FieldIndicators (Field, Indicator, NextMapID, PosX, PosZ, Angle) VALUES ( ?, ?, ?, ?, ?, ? )"))
		{
			pcDB->BindInputParameter(&iMapID, 1, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&eMapIndicatorType, 2, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&iValue, 3, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&iCompactPosX, 4, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&iCompactPosZ, 5, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&iAngle, 6, PARAMTYPE_Integer);

			pcDB->Execute();
		}

		pcDB->Close();
	}

	LoadIndicators();
	SendMapIndicators(pcUser, iMapID, TRUE);

	return TRUE;
}

BOOL CServerCommand::RemoveMapIndicatorNearMe(rsPLAYINFO* pcUser)
{
	if (!pcUser || !pcUser->lpsmSock)
		return FALSE;

	int iMapId = (int)pcUser->Position.Area;
	auto iter = mMapIndicators.find(iMapId);

	if (iter == mMapIndicators.end())
		return FALSE;

	int iDistXY = 999999;
	int iIndex = -1;

	for (int i = 0; i < iter->second.iCount; i++)
	{
		int dX = 0, dZ = 0;
		dX = (pcUser->Position.Position.iX >> 8) - iter->second.mapIndicators[i].iCompactPosX;
		dZ = (pcUser->Position.Position.iZ >> 8) - iter->second.mapIndicators[i].iCompactPosZ;

		int iDist = dX * dX + dZ * dZ;
		if (iDist < iDistXY && iDist < DISTANCE_XY_20_meters)
		{
			iIndex = i;
			iDistXY = iDist;
		}
	}

	if (iIndex == -1)
		return FALSE;

	BOOL bResult = FALSE;
	auto pcDB = SQLCONNECTION(DATABASEID_GameServer);

	if (pcDB->Open())
	{
		if (pcDB->Prepare("DELETE FROM FieldIndicators WHERE Field = ? AND PosX = ? AND PosZ = ?"))
		{
			pcDB->BindInputParameter(&iMapId, 1, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&iter->second.mapIndicators[iIndex].iCompactPosX, 2, PARAMTYPE_Integer);
			pcDB->BindInputParameter(&iter->second.mapIndicators[iIndex].iCompactPosZ, 3, PARAMTYPE_Integer);

			bResult = pcDB->Execute();
		}

		pcDB->Close();
	}

	if (bResult)
	{
		LoadIndicators();
		SendMapIndicators(pcUser, iMapId, TRUE);
	}

	return bResult;
}

// Envia a lista de npcs para o game
void CServerCommand::SendNpcs(rsPLAYINFO* pcUser, smTRANS_COMMAND* Packet)
{
	if (Packet->WParam > 60 || Packet->LParam > 60) // Field 60 máximo
		return;

	PacketNpcsInMapAndNext sPacket;
	ZeroMemory(&sPacket, sizeof(PacketNpcsInMapAndNext));
	sPacket.size = sizeof(PacketNpcsInMapAndNext);
	sPacket.code = PACKET_GET_NPCS;

	int cnt = 0;
	int Pos = 0;

	for (cnt = 0; cnt < AUTOPLAYER_MAX; cnt++)
	{
		if (chrAutoPlayer[cnt].Flag && chrAutoPlayer[cnt].smCharInfo.State == smCHAR_STATE_NPC)
		{
			if (&StageArea[Packet->WParam] && chrAutoPlayer[cnt].lpExt1 == &StageArea[Packet->WParam])
			{
				sPacket.sNpcs[Pos].Position = Point3D(chrAutoPlayer[cnt].pX, chrAutoPlayer[cnt].pY, chrAutoPlayer[cnt].pZ);
				STRINGCOPY(sPacket.sNpcs[Pos].szName, chrAutoPlayer[cnt].smCharInfo.szName);
				Pos++;
			}
			else if (&StageArea[Packet->LParam] && chrAutoPlayer[cnt].lpExt1 == &StageArea[Packet->LParam])
			{
				sPacket.sNpcs[Pos].Position = Point3D(chrAutoPlayer[cnt].pX, chrAutoPlayer[cnt].pY, chrAutoPlayer[cnt].pZ);
				STRINGCOPY(sPacket.sNpcs[Pos].szName, chrAutoPlayer[cnt].smCharInfo.szName);
				Pos++;
			}
		}

		if (Pos == MAX_NPCS_MAP - 1)
			break;
	}

	// Pelo menos 1 npc válido
	if (sPacket.sNpcs[0].szName[0] && Pos)
	{
		if (pcUser && pcUser->lpsmSock)
			pcUser->lpsmSock->Send((char*)&sPacket, sPacket.size, TRUE);
	}
}

void CServerCommand::SendBossTime(rsPLAYINFO* pcUser, PacketBossTimeUpdate* psPacket)
{
	if (!pcUser)
		return;

	int mapID = psPacket->iMapID;
	int SecondMapID = psPacket->SecondMapID;

	if (StageArea[mapID].lpStage)
	{
		SYSTEMTIME sBossTime;
		SYSTEMTIME sServerTime = Time_tToSystemTime(tServerTime);

		CopyMemory(&sBossTime, &sServerTime, sizeof(SYSTEMTIME));

		DWORD dwLessTimeBoss = 0;
		char* pszBossName = NULL;

		extern int OpenBossTimeMin;

		if (StageArea[mapID].rsMonsterList.BossMonsterCount > 0)
		{
			for (int i = 0; i < StageArea[mapID].rsMonsterList.sBossMonsters[0].OpenTimeCount; i++)
			{
				sBossTime.wHour = StageArea[mapID].rsMonsterList.sBossMonsters[0].bOpenTime[i];
				sBossTime.wMinute = OpenBossTimeMin;
				sBossTime.wSecond = 0;

				if (sBossTime.wMinute >= sServerTime.wMinute)
				{
					if (sBossTime.wHour < sServerTime.wHour)
						sBossTime.wDay++;
				}

				DWORD dwBossTime = SystemTimeToDWORD(&sBossTime);
				DWORD dwServerTime = SystemTimeToDWORD(&sServerTime);

				if (dwBossTime >= dwServerTime)
				{
					DWORD dwDifference = dwBossTime - dwServerTime;

					if ((dwLessTimeBoss > dwDifference) || (dwLessTimeBoss == 0))
					{
						pszBossName = StageArea[mapID].rsMonsterList.sBossMonsters[0].MasterMonster->szName;
						dwLessTimeBoss = dwDifference;
					}
				}

				CopyMemory(&sBossTime, &sServerTime, sizeof(SYSTEMTIME));
			}
		}

		PacketBossTimeUpdate sPacket;
		ZeroMemory(&sPacket, sizeof(PacketBossTimeUpdate));
		sPacket.size = sizeof(PacketBossTimeUpdate);
		sPacket.code = PACKET_UPDATE_BOSSTIME;

		if (pszBossName)
		{
			STRINGCOPY(sPacket.szBossName, pszBossName);
			sPacket.dwTime = dwLessTimeBoss;
		}

		SENDPACKET(pcUser, (char*)&sPacket, sPacket.size, 1);

		// Atualiza os indicadores
		SendMapIndicators(pcUser, mapID, SecondMapID);
	}

}