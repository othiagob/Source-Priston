#pragma once

#include <Windows.h>
#include "..\\sinbaram\\SkillFunction\\Tempskron.h"
#include "..\\sinbaram\\SkillFunction\\Morayion.h"
#include "..\\sinbaram\\sinSkill.h"
#include "smPacket.h"
#include "SkillPacket.h"

class rsPLAYINFO;


class Skills
{
private:
	char Buffer[20];
	int MeuInt;
	float MeuFloat;

public:
	Skills(); // construtor
	~Skills();


	void readMechanician();
	void readFighter();
	void readPikeman();
	void readArcher();
	void readKnight();
	void readAtalanta();
	void readPriestess();
	void readMagician();

	void sendMechanician(rsPLAYINFO* player) const;
	void sendFighter(rsPLAYINFO* player) const;
	void sendPikeman(rsPLAYINFO* player) const;
	void sendArcher(rsPLAYINFO* player) const;
	void sendKnight(rsPLAYINFO* player) const;
	void sendAtalanta(rsPLAYINFO* player) const;
	void sendPriestess(rsPLAYINFO* player) const;
	void sendMagician(rsPLAYINFO* player) const;

	void readAll();
	void sendAll() const;
	void sendPlayer(rsPLAYINFO* player) const;

private:
	void send(rsPLAYINFO* player, skillPacket* packet) const;
};


extern Skills skill;
extern void openSkills();