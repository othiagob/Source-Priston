#pragma once
#include <Windows.h>

#define FIELD_ARENA 49


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

extern RankingPckg getRanking;
extern Ranking selectRanking;


class EventoArena
{
public:
	static              EventoArena* GetInstance() { static EventoArena instance; return &instance; }

	bool openFlag = false;

	playersFromTeamPckg getPlayersFromTeam;
	playersFromTeam SelectPlayersFromTeam;

	void renderRanking();
	void showTime(char* time);
	void getRankingFromServer(RankingPckg* Ranking);
	void getTeamPlayersFromServer(playersFromTeamPckg* getPlayers);
	void arenaWindow(bool* p_open, int hora, int minuto, int segundo, int dia);
	int stageArena = 0;
	int qtJogadores = 0;
	int timeArena = FALSE;
	bool flagClan = FALSE;
private:
	int t;

};
