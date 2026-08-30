#include "HelpTime.h"
#include <time.h>

unsigned long long HelpTime::dataParaSegundos(Data d)
{
	unsigned long long total = 0LL;

	total += (d.ano - 1) * 31536000LL;

	int meses[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	for (int mes = 1; mes < d.mes; mes++)
		total += meses[mes - 1] * 86400LL;

	total += (d.dia - 1) * 86400LL;

	total += d.hora * 3600LL;

	total += d.minuto * 60LL;

	total += d.segundo;

	int diasExtras = contaBissextos(d.ano - 1);
	total += diasExtras * 86400LL;

	if (isBissexto(d.ano) && (d.mes - 1) >= 2)
		total += 86400LL;

	return total;
}


bool HelpTime::isBissexto(int ano)
{
	return (ano % 4 == 0) && (ano % 100 != 0 || ano % 400 == 0);
}

int HelpTime::contaBissextos(int ano)
{
	return (ano / 4) - (ano / 100) + (ano / 400);
}

unsigned long long HelpTime::calculaDiferencaEmSegundos(Data d1, Data d2)
{
	unsigned long long segundosd1 = dataParaSegundos(d1);
	unsigned long long segundosd2 = dataParaSegundos(d2);

	if (segundosd1 > segundosd2)
		return segundosd1 - segundosd2;
	else
		return segundosd2 - segundosd1;
}

unsigned long long HelpTime::calculaDiferencaEmDias(Data d1, Data d2)
{
	unsigned long long segundos = calculaDiferencaEmSegundos(d1, d2);
	return segundos / 86400LL;
}

void HelpTime::setTime(Data* pd, int hora, int minuto, int segundo, int dia, int mes, int ano)
{
	pd->hora = hora;
	pd->minuto = minuto;
	pd->segundo = segundo;
	pd->dia = dia;
	pd->mes = mes;
	pd->ano = ano;
}

unsigned long long HelpTime::getData(int hora1, int minuto1, int segundo1, int dia1, int mes1, int ano1, int hora2, int minuto2, int segundo2, int dia2, int mes2, int ano2, int parameter)
{
	Data DataInicio, DataFim;
	time_t now = time(0);
	struct tm* Data = localtime(&now);

	setTime(&DataInicio, hora1, minuto1, segundo1, dia1, mes1, ano1);
	setTime(&DataFim, hora2, minuto2, segundo2, dia2, mes2, ano2);

	if(!parameter)
	return calculaDiferencaEmSegundos(DataInicio, DataFim);
	else
	return calculaDiferencaEmDias(DataInicio, DataFim);
}

HelpTime::HelpTime()
{

}

HelpTime::~HelpTime()
{
}