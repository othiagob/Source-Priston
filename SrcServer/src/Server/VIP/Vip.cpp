#include "..\\sinbaram\\sinLinkHeader.h"
#include <Database/SQLConnection.h>

extern rsPLAYINFO* FindUserFromID(char* szID);
extern int rsRecvAttackData_Old(rsPLAYINFO* lpPlayInfo, TRANS_ATTACKDATA* lpTransAttackData, smCHAR* lpTargetChar, int Type);
extern int SendToAll(char* Buff, int Size);
extern int rsSendGameServer_PrimeItem2(rsPLAYINFO* lpPlayInfo);
extern int rsSendGameServer_PrimeItem(rsPLAYINFO* lpPlayInfo);
extern int rsSendGameServer_PrimeItem3(rsPLAYINFO* lpPlayInfo);

Vip::npcsVIP npcs[20];

void Vip::setNpcsVIP(smCHAR* Npc) {

	if (lstrcmpi(Npc->smMonsterInfo.szName, PREMIUM_NPC_NAME) == 0)
	{
		npcs[0].npc = Npc;
	}

	if (lstrcmpi(Npc->smMonsterInfo.szName, REPOT_NPC_NAME) == 0)
	{
		npcs[1].npc = Npc;
	}

	if (lstrcmpi(Npc->smMonsterInfo.szName, BAU_NPC_NAME) == 0)
	{
		npcs[2].npc = Npc;
	}

}

void Vip::OpenNpc(rsPLAYINFO* Player, int npcnum) {
	TRANS_ATTACKDATA TransAttackData;
	TransAttackData.dwDestObjectSerial = Player->dwObjectSerial;
	TransAttackData.AttackSize = 0;

	GameMasters::getInstance()->Packet(Player, 0x48480067);
	rsRecvAttackData_Old(Player, &TransAttackData, npcs[npcnum].npc, 33);
}


void Vip::setPremiuns(rsPLAYINFO* Player) {

	Player->dwTime_PrimeItem_X2 = (60 * 60 * 24 * 30);
	Player->dwPrimeItem_NextSetTime = 0;
	rsSendGameServer_PrimeItem2(Player);

	Player->dwTime_PrimeItem_ManaRecharg = (60 * 60 * 24 * 30);
	Player->dwPrimeItem_NextSetTime = 0;
	rsSendGameServer_PrimeItem(Player);

	Player->dwTime_PrimeItem_VampCuspid = (60 * 60 * 24 * 30);
	Player->dwPrimeItem_NextSetTime = 0;
	rsSendGameServer_PrimeItem(Player);

	Player->dwTime_PrimeItem_PhenixPet = (60 * 60 * 24 * 30);
	Player->dwPrimeItem_NextSetTime = 0;
	rsSendGameServer_PrimeItem2(Player);

	Player->dwTime_PrimeItem_HelpPet = (60 * 60 * 24 * 30);
	Player->dwPrimeItem_NextSetTime = 0;
	rsSendGameServer_PrimeItem3(Player);
	Player->smCharInfo.GravityScroolCheck[1] = 2;

	GameMasters::getInstance()->Packet(Player, 0x48480068);

}

void Vip::setVipLevel(rsPLAYINFO* Player, int VipNum) {
	Player->vipLevel = VipNum;
}

void Vip::setTimeVip(DataVip* pd, int hora, int minuto, int segundo, int dia, int mes, int ano)
{
	pd->hora = hora;
	pd->minuto = minuto;
	pd->segundo = segundo;
	pd->dia = dia;
	pd->mes = mes;
	pd->ano = ano;
}

unsigned long long Vip::dataParaSegundos(DataVip d)
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


bool Vip::isBissexto(int ano)
{
	return (ano % 4 == 0) && (ano % 100 != 0 || ano % 400 == 0);
}

int Vip::contaBissextos(int ano)
{
	return (ano / 4) - (ano / 100) + (ano / 400);
}

unsigned long long Vip::calculaDiferencaEmSegundos(DataVip d1, DataVip d2)
{
	unsigned long long segundosd1 = dataParaSegundos(d1);
	unsigned long long segundosd2 = dataParaSegundos(d2);

	if (segundosd1 > segundosd2)
		return segundosd1 - segundosd2;
	else
		return 0;
}

unsigned long long Vip::calculaDiferencaEmDias(DataVip d1, DataVip d2)
{
	unsigned long long segundos = calculaDiferencaEmSegundos(d1, d2);
	return segundos / 86400LL;
}

int diasMes[] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };

// Soma dias a data
void getDate(std::vector<int>& arr)
{
	char re[3] = { 0 };
	int i, k = 0, p = 0, a = 0;

	for (i = 0; i < arr[1]; i++)
		k = k + diasMes[i];

	k = k + arr[0] + arr[3];

	if (k <= 365)
	{
		for (i = 0; i < 13; i++)
		{
			p = k - diasMes[i];
			if (p <= diasMes[i + 1])
			{
				a = i + 1;
				break;
			}
			else
				k = p;
		}

		arr[0] = p;
		arr[1] = a;
	}
	else
	{
		k = k - 365;
		for (i = 0; i < 13; i++)
		{
			p = k - diasMes[i];

			if (p <= diasMes[i + 1])
			{
				a = i + 1;
				break;
			}
			else
				k = p;
		}

		arr[0] = p;
		arr[1] = a;
		arr[2] = arr[2] + 1;
	}
}


void Vip::addNewVip(rsPLAYINFO* Player, int vipNum)
{
	time_t now = time(0);
	struct tm* Data = localtime(&now);

	std::vector<int> p;
	p.push_back(Data->tm_mday);
	p.push_back(Data->tm_mon + 1);
	p.push_back(Data->tm_year + 1900);
	p.push_back(31); // Dias para adicionar

	getDate(p);

	int diaExpira = p[0];
	int mesExpira = p[1];
	int anoExpira = p[2];

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "INSERT INTO VIP VALUES (?,?,?,?,?,?,?,?)";

		if (db->Prepare(query))
		{
			int mes = Data->tm_mon + 1;
			int ano = Data->tm_year + 1900;

			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);
			db->BindInputParameter(&Data->tm_mday, 2, PARAMTYPE_Integer);
			db->BindInputParameter(&mes, 3, PARAMTYPE_Integer);
			db->BindInputParameter(&ano, 4, PARAMTYPE_Integer);
			db->BindInputParameter(&diaExpira, 5, PARAMTYPE_Integer);
			db->BindInputParameter(&mesExpira, 6, PARAMTYPE_Integer);
			db->BindInputParameter(&anoExpira, 7, PARAMTYPE_Integer);
			db->BindInputParameter(&vipNum, 8, PARAMTYPE_Integer);

			db->Execute();

			setVipLevel(Player, vipNum);
			SERVERCHAT->SendChatEx(Player, EChatColor::CHATCOLOR_Global, "> Seu vip %d foi ativado por 30 dias!", vipNum);

			// Avisa o game o level do vip
			GameMasters::getInstance()->Packet(Player, 0x48480066, vipNum);
		}

		db->Close();
	}
}

void deleteVip(char id[32])
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB_VIP);

	if (db->Open())
	{
		const char* const query = "DELETE FROM VIP WHERE ID=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(id, 1, PARAMTYPE_String);
			db->Execute();

			rsPLAYINFO* Player = FindUserFromID(id);

			if (Player && Player->lpsmSock)
			{
				Player->vipLevel = 0;

				SERVERCHAT->SendChat(Player, EChatColor::CHATCOLOR_Global, "> Seu VIP Expirou!");

				// Avisa o game o level do vip
				GameMasters::getInstance()->Packet(Player, 0x48480066, 0);
			}
		}

		db->Close();
	}
}

void Vip::getTimeVip(rsPLAYINFO* Player)
{
	DataVip inicioVip, fimVip;
	int vipLevel = 0;
	int diasRestantes = 0;

	Player->vipLevel = 0;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	bool found = false;

	if (db->Open())
	{
		const char* const query = "SELECT * FROM VIP WHERE ID=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);

			if (db->Execute())
			{
				db->GetData(5, PARAMTYPE_Integer, &fimVip.dia);
				db->GetData(6, PARAMTYPE_Integer, &fimVip.mes);
				db->GetData(7, PARAMTYPE_Integer, &fimVip.ano);
				db->GetData(8, PARAMTYPE_Integer, &vipLevel);

				found = true;

				time_t now = time(0);
				struct tm* Data = localtime(&now);

				// Seta o tempo do vip atual
				setTimeVip(&inicioVip, 0, 0, 0, Data->tm_mday, Data->tm_mon + 1, Data->tm_year + 1900);
				setTimeVip(&fimVip, 0, 0, 0, fimVip.dia, fimVip.mes, fimVip.ano);
			}
		}

		db->Close();
	}

	if (found)
	{
		if (calculaDiferencaEmSegundos(fimVip, inicioVip) > 0)
		{
			diasRestantes = static_cast<int>(calculaDiferencaEmDias(fimVip, inicioVip));

			SERVERCHAT->SendChatEx(Player, EChatColor::CHATCOLOR_Global, "> Você possui %d dias de VIP restantes", diasRestantes);

			// Avisa o game o level do vip
			GameMasters::getInstance()->Packet(Player, 0x48480066, vipLevel);

			// Seta o nível do vip
			setVipLevel(Player, vipLevel);
		}
		else
		{
			// Retira o VIP quando expirar o tempo
			deleteVip(Player->szID);
		}
	}
}

void Vip::checkAllVips()
{
	DataVip inicioVip, fimVip;
	char szid[33] = { 0 };

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM VIP";

		if (db->Prepare(query))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					ZeroMemory(&fimVip, sizeof(DataVip));
					ZeroMemory(&inicioVip, sizeof(DataVip));

					db->GetData(1, PARAMTYPE_String, szid);
					db->GetData(5, PARAMTYPE_Integer, &fimVip.dia);
					db->GetData(6, PARAMTYPE_Integer, &fimVip.mes);
					db->GetData(7, PARAMTYPE_Integer, &fimVip.ano);

					time_t now = time(0);
					struct tm* Data = localtime(&now);

					//Seta o tempo do vip atual
					setTimeVip(&inicioVip, 0, 0, 0, Data->tm_mday, Data->tm_mon + 1, Data->tm_year + 1900);

					if (calculaDiferencaEmSegundos(fimVip, inicioVip) == 0)
					{
						deleteVip(szid);
					}
				}
			}
		}

		db->Close();
	}
}