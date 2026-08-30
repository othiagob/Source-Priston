#pragma once
#include <Windows.h>
#include "Chat\\ChatServer.h"

#define ROLETA_SEND_TOPPLAYERS 0x50700030
#define ROLETA_DIVIDIR_IGUALMENTE 0x50700031
#define ROLETA_PARTICIPAR_RODADA 0x50700032

struct TOP10_ROLETA
{
	int		x, y, z;

	DWORD	dwUpdateTime;
	int		Damage;
	int		Counter;
	int		totalDamage;
	int		chance;

	char szName[32];
	short jobCode;

	bool pSorteio;
};

struct RoletaPckg
{
	int size;
	int code;
	int totalDamage;
	int participantes;
	TOP10_ROLETA Players[10];

	RoletaPckg()
	{
		size = 0;
		code = 0;
		totalDamage = 0;
		participantes = 0;
		ZeroMemory(Players, sizeof(TOP10_ROLETA) * 10);
	}
};

struct PckgDividir
{
	int size;
	int code;
	TOP10_ROLETA Players[10];
};

struct rsUSER_LIST_TOP10;

class Roleta
{

public:
	static              Roleta* GetInstance() { static Roleta instance; return &instance; }

	INT SendTopPlayers(rsUSER_LIST_TOP10 Top10List, smCHAR* Monster);
	INT IniciarRoleta(RoletaPckg Players, smCHAR* Monster, short Slot);
	INT AdicionarParticipante(rsPLAYINFO* Player);
private:

};

