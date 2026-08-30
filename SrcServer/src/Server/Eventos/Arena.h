#pragma once
#include <Windows.h>

#define FIELD_ARENA 49

class EventoArena
{
public:
	static              EventoArena* GetInstance() { static EventoArena instance; return &instance; }

	struct JogadoresAtivos {
		rsPLAYINFO* Jogador;
	} JogadoresAtivos[110];


	struct playersFromTeam {
		char name[32];
		char id[32];
		int equipeNum = 0;
	};

	struct playersFromTeamPckg {
		int size;
		int code;
		playersFromTeam pckgTeam[110];
	};

	struct	Ranking {
		char* equipeName;
		int equipeNum = 0;
		int	equipeScore = 0;
		int Players = 0;
	};

	struct RankingPckg
	{
		int size;
		int code;
		Ranking Ranking[50];
	};

	struct Reward
	{
		char										szName[32];
		char										szCode[32];
		int											iQuantity;
		int											iPercent;
	};

	int timesArena = 0, daysArena[7] = { 0 }, startTimesHour[10] = { 0 }, startTimesMinutes[10] = { 0 }, endTimesHour[10] = { 0 }, endTimesMinutes[10] = { 0 };
	int jogadores = 0;
	int arenaStage = 0;

	void TeleportPlayer(rsPLAYINFO* lpPlayInfo);
	void equipesArena(rsPLAYINFO* lpPlayInfo);
	void Jogadores(rsPLAYINFO* lpPlayInfo);
	void imprimeJogadores(rsPLAYINFO* lpPlayInfo);
	void verificaPlayerDC(rsPLAYINFO* Player);
	void startArena(rsPLAYINFO* lpPlayInfo);
	void endArena(rsPLAYINFO* lpPlayInfo);
	void rewardArena();
	void onArena(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int isLogin);
	int checkBoss(smCHAR* Monster);
private:

};
