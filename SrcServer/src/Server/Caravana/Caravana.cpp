#include "..\\sinbaram\\sinLinkHeader.h"
#include "../SrcServer/onserver.h"
#include "..\\Database\SQLConnection.h"

#include <boost/algorithm/string.hpp>


extern int chrMonsterCnt;
extern smCHAR_INFO* chrMonsterList;

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string& s)
{
	size_t start = s.find_first_not_of(WHITESPACE);
	return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string& s)
{
	size_t end = s.find_last_not_of(WHITESPACE);
	return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string& s) {
	return rtrim(ltrim(s));
}

void Caravana::RenameCaravan(rsPLAYINFO* Player, Caravan_Info_Pckg* Pckg)
{
	Player->sCaravan.caravanName = Pckg->Pckg.caravanName;
	Player->sCaravan.caravanName = trim(Player->sCaravan.caravanName.c_str());
	sprintf_s(Player->sCaravan.Model->smCharInfo.szName, sizeof(Player->sCaravan.Model->smCharInfo.szName), "%s", Player->sCaravan.caravanName.c_str());
	sprintf_s(Player->sCaravan.Model->smCharInfo.lpMonInfo->szName, sizeof(Player->sCaravan.Model->smCharInfo.lpMonInfo->szName), "%s", Player->sCaravan.caravanName.c_str());

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "UPDATE Caravans SET CaravanName=? WHERE Name=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->sCaravan.Model->smCharInfo.szName, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);

			if (db->Execute(false))
			{
				GameMasters::getInstance()->Alert(Player, "> Caravana renomeada com sucesso!");

				if (Player->sCaravan.Model)
				{
					Player->sCaravan.Model->smCharInfo.Life[0] = 0;
					Player->sCaravan.Model->SetMotionFromCode(CHRMOTION_STATE_DEAD);
					Player->sCaravan.Model->Close();
				}

				respawnCaravan(Player, Player->Position.x, Player->Position.y, Player->Position.z, Player->sCaravan.CaravanType, 1, Player->sCaravan.caravanName, false, Player->sCaravan.timeRemaining);
			}
		}

		db->Close();
	}
}

void Caravana::CheckCaravan(rsPLAYINFO* Player)
{
	char caravanName[64] = { 0 };
	int caravanTime = 0;
	int caravanType = 0;

	CaravanTypes cType;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT * FROM Caravans WHERE Name=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);

			if (db->Execute())
			{
				db->GetData(3, PARAMTYPE_String, caravanName, sizeof(caravanName));
				db->GetData(4, PARAMTYPE_Integer, &caravanType);
				db->GetData(5, PARAMTYPE_Integer, &caravanTime);
			}
		}

		db->Close();
	}

	if (caravanTime > 0)
	{
		switch (caravanType)
		{
		case 0:
			cType = CaravanTypes::CARAVAN_ARMA;
			Player->dwTime_CaravanArma = caravanTime;
			break;
		case 1:
			cType = CaravanTypes::CARAVAN_HOPY;
			Player->dwTime_CaravanHopy = caravanTime;
			break;
		case 2:
			cType = CaravanTypes::CARAVAN_BUMA;
			Player->dwTime_CaravanBuma = caravanTime;
			break;
		default:
			cType = CaravanTypes::CARAVAN_ARMA;
			Player->dwTime_CaravanArma = caravanTime;
			break;
		}

		// Verifica se já não tem alguma caravana ativa
		if (Player->sCaravan.Model)
		{
			Player->sCaravan.Model->smCharInfo.Life[0] = 0;
			Player->sCaravan.Model->SetMotionFromCode(CHRMOTION_STATE_DEAD);
			Player->sCaravan.Model->Close();
		}

		respawnCaravan(Player, Player->Position.x, Player->Position.y, Player->Position.z, cType, false, caravanName, false, caravanTime);
	}
}

void Caravana::SaveCaravan(rsPLAYINFO* Player)
{
	// Atualiza o time das caravanas
	if (Player && Player->lpsmSock && Player->sCaravan.CaravanType != CaravanTypes::EMPTY && Player->sCaravan.timeRemaining > 0)
	{
		auto db = SQLConnection::GetConnection(DATABASEID_UserDB);
		int time = 0;

		switch (Player->sCaravan.CaravanType)
		{
		case CaravanTypes::CARAVAN_ARMA:
			time = Player->dwTime_CaravanArma;
			break;
		case CaravanTypes::CARAVAN_HOPY:
			time = Player->dwTime_CaravanHopy;
			break;
		case CaravanTypes::CARAVAN_BUMA:
			time = Player->dwTime_CaravanBuma;
			break;
		}

		// Caravana com tempo restante ainda
		if (time > 0)
		{
			if (db && db->Open())
			{
				Player->sCaravan.timeRemaining = time;

				const char* const query = "UPDATE Caravans SET RemainingTime=? WHERE Name=?";

				if (db->Prepare(query))
				{
					db->BindInputParameter(&time, 1, PARAMTYPE_Integer);
					db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);

					db->Execute();
				}

				db->Close();
			}
		}
		else // Caravana venceu, deleta e mata ela no jogo
		{
			if (Player->sCaravan.timeRemaining <= 2 && time == 0)
				deleteCaravan(Player);
		}

	}

}

void Caravana::OpenCaravan(smWINSOCK* Player, smCHAR* Caravan)
{
	if (Caravan == nullptr || Player == nullptr) return;

	int cnt = 0;
	bool found = false;

	for (cnt = 0; cnt < CONNECTMAX; cnt++)
	{
		if (rsPlayInfo[cnt].lpsmSock)
		{
			if (rsPlayInfo[cnt].lpsmSock == Player)
			{
				found = true;
				break;
			}
		}
	}

	if (found)
	{
		// Encontra o player e verifica se a caravana é dele
		if (Player && Caravan && Caravan->lpMasterPlayInfo)
		{
			if (Caravan->lpMasterPlayInfo->dwObjectSerial == rsPlayInfo[cnt].dwObjectSerial)
			{
				SendOpenCaravan(Player);
			}
		}
	}
}

void Caravana::NewCaravan(rsPLAYINFO* Player, CaravanTypes CaravanType, std::string caravanName, int Time)
{
	int caravanType = 0;

	switch (CaravanType)
	{
	case CaravanTypes::CARAVAN_ARMA:
		caravanType = 0;
		break;
	case CaravanTypes::CARAVAN_HOPY:
		caravanType = 1;
		break;
	case CaravanTypes::CARAVAN_BUMA:
		caravanType = 2;
		break;
	}

	char strcaravanName[50] = { 0 };
	sprintf_s(strcaravanName, sizeof(strcaravanName), "%s", caravanName.c_str());

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "INSERT INTO Caravans VALUES (?,?,?,?,?)";

		if (db->Prepare(query))
		{
			db->BindInputParameter(Player->szID, 1, PARAMTYPE_String);
			db->BindInputParameter(Player->szName, 2, PARAMTYPE_String);
			db->BindInputParameter(strcaravanName, 3, PARAMTYPE_String);
			db->BindInputParameter(&caravanType, 4, PARAMTYPE_Integer);
			db->BindInputParameter(&Time, 5, PARAMTYPE_Integer);

			db->Execute();
		}
		db->Close();

	}
}

void Caravana::respawnCaravan(rsPLAYINFO* Player, int x, int y, int z, CaravanTypes CaravanType, bool respawn, std::string caravanName, bool isNew, int time)
{
	STG_CHAR_INFO StageCaravan;
	ZeroMemory(&StageCaravan, sizeof(STG_CHAR_INFO));

	STG_AREA* lpStgArea = new STG_AREA();
	ZeroMemory(lpStgArea, sizeof(STG_AREA));

	Caravan sCaravan;

	int flag = 0;

	std::string caravanInGameName;

	switch (CaravanType)
	{
	case CaravanTypes::CARAVAN_ARMA:
		caravanInGameName = "Caravana Arma";
		break;
	case CaravanTypes::CARAVAN_HOPY:
		caravanInGameName = "Caravana Hopi";
		break;
	case CaravanTypes::CARAVAN_BUMA:
		caravanInGameName = "Caravana Buma";
		break;
	}

	if (Player->sCaravan.caravanName[0])
	{
		caravanName = Player->sCaravan.caravanName;
		flag = 1;
	}

	if (!respawn)
	{
		flag = 1;
	}

	for (int cnt = 0; cnt < chrMonsterCnt; cnt++)
	{
		if (boost::iequals(chrMonsterList[cnt].szName, caravanInGameName))
		{
			memcpy(&StageCaravan, &chrMonsterList[cnt], sizeof(smCHAR_INFO));

			if ((!isNew && !flag) || isNew)
				sprintf_s(StageCaravan.smCharInfo.szName, sizeof(StageCaravan.smCharInfo.szName), "C. de %s", Player->szName); // Nome inicial da caravana	
			else
				sprintf_s(StageCaravan.smCharInfo.szName, sizeof(StageCaravan.smCharInfo.szName), "%s", caravanName.c_str());

			break;
		}
	}

	if (StageCaravan.smCharInfo.szName[0])
	{
		lpStgArea = FindStageField(Player->Position.x, Player->Position.z);

		StageCaravan.smCharInfo.lpMonInfo->Caravana = TRUE;
		StageCaravan.smCharInfo.lpMonInfo->CaravanType = CaravanType;
		StageCaravan.smCharInfo.lpMonInfo->CaravanMapNum = Player->Position.Area;
		StageCaravan.smCharInfo.lpMonInfo->CaravanState = CaravanStates::FOLLOW;


		if (lpStgArea)
		{
			Player->sCaravan.Model = lpStgArea->OpenCaravan(&StageCaravan, Player);
			lpStgArea->EvnMonster = TRUE;
		}

		if (Player->sCaravan.Model)
		{
			Player->sCaravan.Model->pX = x;
			Player->sCaravan.Model->pY = y;
			Player->sCaravan.Model->pZ = z;
			Player->sCaravan.Model->DistAroundDbl = 0;
			Player->sCaravan.Model->TargetMoveCount = 2;

			lstrcpy(Player->sCaravan.Model->smCharInfo.szModelName2 + 1, Player->smCharInfo.szName);
			Player->sCaravan.Model->smCharInfo.Next_Exp = Player->dwObjectSerial;
			Player->sCaravan.Model->lpMasterPlayInfo = Player;
			Player->sCaravan.Model->smMonsterInfo.dwAutoCharCode = 0;
			Player->sCaravan.Model->dwUpdateCharInfoTime = dwPlayServTime + 4 * 60 * 1000;
			Player->sCaravan.Model->smCharInfo.Brood = smCHAR_CARAVAN;

			// Cristais Amigaveis
			Player->sCaravan.Model->smCharInfo.ClassClan = 0;
			Player->sCaravan.Model->dwClanManageBit = 0;
			Player->sCaravan.Model->HoSkillMode = 0;
			Player->sCaravan.Model->HoSkillCode = 1;

			Player->sCaravan.Model->smMonsterInfo.Caravana = TRUE;
			Player->sCaravan.Model->smMonsterInfo.CaravanType = CaravanType;
			Player->sCaravan.Model->smMonsterInfo.CaravanMapNum = Player->Position.Area;

			Player->sCaravan.Model->Clan_CastleMasterFlag = 0;

			smTRANS_COMMAND_EX	TransCommandEx;

			ZeroMemory(&TransCommandEx, sizeof(smTRANS_COMMAND_EX));
			TransCommandEx.WParam = smCOMMNAD_USER_WARP;
			TransCommandEx.LParam = x;
			TransCommandEx.SParam = y;
			TransCommandEx.EParam = z;

			TransCommandEx.LxParam = FALSE;

			if (respawn)
				TransCommandEx.WxParam = 502;
			else
				TransCommandEx.WxParam = 501;

			TransCommandEx.ExParam = Player->sCaravan.Model->dwObjectSerial;
			TransCommandEx.SxParam = Player->dwObjectSerial;

			rsSendCommandUser(Player, &TransCommandEx);

			Player->sCaravan.caravanName = StageCaravan.smCharInfo.szName;
			Player->sCaravan.CaravanState = CaravanStates::FOLLOW;
			Player->sCaravan.CaravanType = CaravanType;
			Player->sCaravan.timeRemaining = time;

			if (!respawn)
			{
				int ctype = 0;

				if (CaravanType == CaravanTypes::CARAVAN_ARMA)
				{
					ctype = 0;
				}
				else if (CaravanType == CaravanTypes::CARAVAN_HOPY)
				{
					ctype = 1;
				}
				else if (CaravanType == CaravanTypes::CARAVAN_BUMA)
				{
					ctype = 2;
				}

				if (!isNew)
				{
					Caravan_Info_Pckg s;

					s.code = 0x43600004;
					s.size = sizeof(Caravan_Info_Pckg);

					sprintf_s(s.Pckg.caravanName, sizeof(s.Pckg.caravanName), "%s", StageCaravan.smCharInfo.szName);
					s.Pckg.caravanType = ctype;
					s.Pckg.timeLeft = time;

					Player->lpsmSock->Send((char*)&s, s.size, TRUE);
				}

				if (isNew)
					NewCaravan(Player, Player->sCaravan.CaravanType, StageCaravan.smCharInfo.szName, time);

			}
			else
			{
				if (Player->sCaravan.CaravanType != CaravanTypes::EMPTY)
				{
					Player->sCaravan.timeRemaining = time;
				}
			}
		}
	}
}

void Caravana::SendCaravanToPlayer(rsPLAYINFO* Player)
{
	if (Player)
	{
		if (Player->sCaravan.Model->smCharInfo.lpMonInfo->Caravana)
		{
			Player->sCaravan.Model->smCharInfo.Life[0] = 0;
			Player->sCaravan.Model->SetMotionFromCode(CHRMOTION_STATE_DEAD);
			Player->sCaravan.Model->Close();

			STG_AREA* lpStgArea;
			lpStgArea = FindStageField(Player->Position.x, Player->Position.z);

			if (lpStgArea)
			{
				lpStgArea->DeleteMonTable(Player->sCaravan.Model);
				lpStgArea->MonsterCount--;
			}
		}

		GameMasters::getInstance()->Alert(Player, "> Sua caravana chegou!");

		respawnCaravan(Player, Player->Position.x, Player->Position.y, Player->Position.z, Player->sCaravan.CaravanType, 1, Player->sCaravan.caravanName, false, Player->sCaravan.timeRemaining);
	}
}

void Caravana::DeleteFromVector(rsPLAYINFO* Player)
{
	if (Player)
	{
		if (Player->sCaravan.Model && Player->sCaravan.Model->lpExt1)
		{
			STG_AREA* lpStgArea = (STG_AREA*)Player->sCaravan.Model->lpExt1;

			if (lpStgArea)
			{
				if (lpStgArea->MonsterCount > 0)
					lpStgArea->MonsterCount--;

				lpStgArea->DeleteMonTable(Player->sCaravan.Model);
			}
		}

		if (Player->sCaravan.Model)
		{
			Player->sCaravan.Model->smCharInfo.Life[0] = 0;
			Player->sCaravan.Model->SetMotionFromCode(CHRMOTION_STATE_DEAD);
			Player->sCaravan.Model->Close();
		}

		Player->sCaravan.caravanName = "";
		Player->sCaravan.CaravanState = CaravanStates::STAY;
		Player->sCaravan.Model = { 0 };
		Player->sCaravan.timeRemaining = 0;
		Player->sCaravan.CaravanType = CaravanTypes::EMPTY;
	}
}

void Caravana::SetCaravanState(rsPLAYINFO* Player, CaravanStates State)
{
	if (State == CaravanStates::FOLLOW)
	{
		Player->sCaravan.CaravanState = State;
		Player->sCaravan.Model->smCharInfo.lpMonInfo->CaravanState = State;

		GameMasters::getInstance()->Alert(Player, "> Sua Caravana seguirá você!");
	}

	if (State == CaravanStates::STAY)
	{
		Player->sCaravan.CaravanState = State;
		Player->sCaravan.Model->smCharInfo.lpMonInfo->CaravanState = State;

		GameMasters::getInstance()->Alert(Player, "> Caravana parada no local!");
	}
}

void Caravana::deleteCaravan(rsPLAYINFO* Player)
{
	if (Player->sCaravan.CaravanType != CaravanTypes::EMPTY)
	{
		Player->sCaravan.Model->smCharInfo.Life[0] = 0;
		Player->sCaravan.Model->SetMotionFromCode(CHRMOTION_STATE_DEAD);
		Player->sCaravan.Model->Close();

		STG_AREA* lpStgArea;
		lpStgArea = FindStageField(Player->Position.x, Player->Position.z);

		if (lpStgArea)
		{
			lpStgArea->DeleteMonTable(Player->sCaravan.Model);
			lpStgArea->MonsterCount--;
		}

		Player->sCaravan.caravanName = "";
		Player->sCaravan.CaravanState = CaravanStates::STAY;
		Player->sCaravan.Model = { 0 };
		Player->sCaravan.timeRemaining = 0;
		Player->sCaravan.CaravanType = CaravanTypes::EMPTY;

		auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

		if (db && db->Open())
		{
			const char* const query = "DELETE FROM Caravans WHERE Name=?";

			if (db->Prepare(query))
			{
				db->BindInputParameter(Player->szName, 1, PARAMTYPE_String);
				db->Execute();

				deleteCaravan(Player);
			}

			db->Close();
		}
	}

}