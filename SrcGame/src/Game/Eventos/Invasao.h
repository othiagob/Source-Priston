#pragma once
#include <Windows.h>

#define FIELD_INVASAO 53

class EventoInvasao
{
public:
	static              EventoInvasao* GetInstance() { static EventoInvasao instance; return &instance; }
	bool isInvasaoActive = FALSE;

private:
	int t;

};
