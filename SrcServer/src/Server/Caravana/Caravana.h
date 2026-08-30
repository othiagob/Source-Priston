#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <memory>

#define smTRANSCODE_OPEN_CARAVAN 0x4580605
#define	smTRANSCODE_CARAVAN 0x4580606

class Caravana
{

public:
	static              Caravana* GetInstance() { static Caravana instance; return &instance; }


	struct	TRANS_CARAVAN {
		int size, code;

		DWORD	dwChkSum;
		WORD	wVersion[2];
		int		WareHouseMoney;
		int		UserMoney;
		DWORD	dwTemp[5];

		int		DataSize;
		BYTE	Data[sizeof(sITEM) * 100 + 256];
	};

	struct Caravan_Info
	{
		char caravanName[32];
		int caravanType;
		int timeLeft;

		Caravan_Info()
		{
			ZeroMemory(caravanName, sizeof(caravanName));
			caravanType = 0;
			timeLeft = 0;
		}
	};

	struct Caravan_Info_Pckg {
		int size;
		int code;
		Caravan_Info Pckg;

		Caravan_Info_Pckg()
		{
			size = 0;
			code = 0;
			Pckg = Caravan_Info();
		}
	};

	void respawnCaravan(rsPLAYINFO* Player, int x, int y, int z, CaravanTypes CaravanType, bool respawn, std::string caravanName = "", bool isNew = false, int time = 10000);
	void deleteCaravan(rsPLAYINFO* Player);
	void CheckCaravan(rsPLAYINFO* Player);
	void DeleteFromVector(rsPLAYINFO* Player);
	void SetCaravanState(rsPLAYINFO* Player, CaravanStates State);
	void SaveCaravan(rsPLAYINFO* Player);
	void NewCaravan(rsPLAYINFO* Player, CaravanTypes CaravanType, std::string caravanName, int Time);
	void SendCaravanToPlayer(rsPLAYINFO* Player);
	void RenameCaravan(rsPLAYINFO* Player, Caravan_Info_Pckg* Pckg);
	void OpenCaravan(smWINSOCK* Player, smCHAR* Caravan);

private:
};

