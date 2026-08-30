#pragma once
#include <Windows.h>

#define ROLETA_RECEIVE_TOPPLAYERS 0x50700030
#define ROLETA_DIVIDIR_IGUALMENTE 0x50700031
#define ROLETA_PARTICIPAR_RODADA 0x50700032
#define ROLETA_ENTRAR_RODADA 0x50700033

struct TOP10_ROLETA
{
	int		x, y, z;

	DWORD	dwUpdateTime;
	int		Damage;
	int		Counter;
	int		totalDamage;
	int     chance;

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
};


class Roleta
{

public:
    static              Roleta* GetInstance() { static Roleta instance; return &instance; }

    bool openFlag = false;
	bool openRoleta = false;
	bool participarRodada = true;
	bool dividirIgualmente = false;
    bool init = true;

	char itemId[32];

    INT ReceiveTopPlayers(RoletaPckg* Players);
	INT UpdatePlayers(RoletaPckg* Players);

	void initRoleta();
	void updateItemImage(char itemID[32]);
    void Show(bool* p_open);
	void ShowRoleta(bool* p_open);


private:

};

