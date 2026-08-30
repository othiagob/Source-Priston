#pragma once
#include <Windows.h>

#define FIELD_INVASAO 53

class EventoInvasao
{
public:
	static              EventoInvasao* GetInstance() { static EventoInvasao instance; return &instance; }

	void TeleportPlayer(rsPLAYINFO* lpPlayInfo);
	void startInvasao(rsPLAYINFO* lpPlayInfo);
	void endInvasao(rsPLAYINFO* lpPlayInfo, int xPosition, int zPosition);
private:

};
