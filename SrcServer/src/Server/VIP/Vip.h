#pragma once
#include <Windows.h>

//Sistema de VIP NPC - DRACKO
#define PREMIUM_NPC_NAME "Itens Premiuns"
#define REPOT_NPC_NAME "Loja da Gaby"
#define BAU_NPC_NAME "Baú"


class Vip
{
	typedef struct _DataVip {
		int hora;
		int minuto;
		int segundo;
		int dia;
		int mes;
		int ano;
	} DataVip;

public:
	static              Vip* GetInstance() { static Vip instance; return &instance; }
	void                setVipLevel(rsPLAYINFO* Player, int VipNum);
	void				setNpcsVIP(smCHAR *Npc);
	void				OpenNpc(rsPLAYINFO* Player, int npcNum);
	void				setPremiuns(rsPLAYINFO* Player);
	void				addNewVip(rsPLAYINFO* Player, int vipNum);
	void				getTimeVip(rsPLAYINFO* Player);
	void				checkAllVips();
	void setTimeVip(DataVip* pd, int hora, int minuto, int segundo, int dia, int mes, int ano);
	bool isBissexto(int ano);
	int contaBissextos(int ano);
	unsigned long long calculaDiferencaEmDias(DataVip d1, DataVip d2);
	unsigned long long Vip::calculaDiferencaEmSegundos(DataVip d1, DataVip d2);
	unsigned long long dataParaSegundos(DataVip d);

	struct npcsVIP {
		smCHAR* npc;
	};

private:
};

