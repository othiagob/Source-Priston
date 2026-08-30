#pragma once

#include <Windows.h>

class HelpTime
{
	HelpTime();
	virtual ~HelpTime();

	typedef struct _Data {
		int hora;
		int minuto;
		int segundo;
		int dia;
		int mes;
		int ano;
	} Data;

public:
	void setTime(Data* pd, int hora, int minuto, int segundo, int dia, int mes, int ano);
	bool isBissexto(int ano);
	int contaBissextos(int ano);
	unsigned long long getData(int hora1, int minuto1, int segundo1, int dia1, int mes1, int ano1, int hora2, int minuto2, int segundo2, int dia2, int mes2, int ano2, int parameter=0);
	unsigned long long calculaDiferencaEmDias(Data d1, Data d2);
	unsigned long long calculaDiferencaEmSegundos(Data d1, Data d2);
	unsigned long long dataParaSegundos(Data d);
private:
};

