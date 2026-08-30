#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\Database\SQLConnection.h"
#include <Chat/ChatServer.h>

void PVPRanking::ResetRanking(rsPLAYINFO* lpPlayInfo) {

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	char nickTopClass[256] = { 0 };
	char Classe[256] = { 0 };
	int bps = 0;

		for (int i = 1; i < 8;) 
		{
			switch (i)
			{
			case 1:
				sprintf_s(Classe, sizeof(Classe), "Lutador");
				break;
			case 2:
				sprintf_s(Classe, sizeof(Classe), "Mecânico");
				break;
			case 3:
				sprintf_s(Classe, sizeof(Classe), "Arqueira");
				break;
			case 4:
				sprintf_s(Classe, sizeof(Classe), "Pikeman");
				break;
			case 5:
				sprintf_s(Classe, sizeof(Classe), "Atalanta");
				break;
			case 6:
				sprintf_s(Classe, sizeof(Classe), "Cavaleiro");
				break;
			case 7:
				sprintf_s(Classe, sizeof(Classe), "Mago");
				break;
			case 8:
				sprintf_s(Classe, sizeof(Classe), "Sacerdotisa");
				break;
			}

			if (db->Open())
			{
				if (db->Prepare("SELECT TOP 1 Name FROM UserInfo WHERE Class = ? and Kills > 0 ORDER BY Kills DESC"))
					db->BindInputParameter(&i, 1, PARAMTYPE_Integer);

				db->Execute();
				
				if (db->GetData(1, PARAMTYPE_String, nickTopClass, sizeof(nickTopClass)))
				{
					SERVERCHAT->SendChatAllEx(EChatColor::CHATCOLOR_Global, "Top % s semanal > % s", Classe, nickTopClass);
				}
			}	

			i++;
		

			if (db->Prepare("UPDATE UserInfo SET Kills=?, Deads=?"))
			{
				db->BindInputParameter(&bps, 1, PARAMTYPE_Integer);
				db->BindInputParameter(&bps, 2, PARAMTYPE_Integer);

				db->Execute();
			}	
			db->Close();
		}

		SERVERCHAT->SendChatAll(EChatColor::CHATCOLOR_Global, "> Pontos de Batalha resetados!");
	
		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480056;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (lpPlayInfo[cnt].lpsmSock)
			{
				lpPlayInfo[cnt].lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
				lpPlayInfo[cnt].sBlessCastle_Damage[1] = 0;
			}
		}
}

void PVPRanking::setBuff(char* nome)
{
	int iCount = 0;
	int iTimeLeft = (60*60*24*3); // 3 DIAS
	int iItemType = 1;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db->Open())
	{
		if (db->Prepare("SELECT COUNT(*) FROM PremiumData WHERE CharacterName=?"))
		{
			db->BindInputParameter(nome, 1, PARAMTYPE_String);
			db->Execute();

			db->GetData(1, PARAMTYPE_Integer, &iCount);

			if (iCount > 0)
			{
				if (iTimeLeft > 1)
				{
					if (db->Prepare("UPDATE PremiumData SET TimeLeft=? WHERE CharacterName=?"))
					{
						db->BindInputParameter(&iTimeLeft, 1, PARAMTYPE_Integer);
						db->BindInputParameter(nome, 2, PARAMTYPE_String);
						db->Execute();
					}
				}
				else
				{
					if (db->Prepare("DELETE FROM PremiumData WHERE CharacterName=?"))
					{
						db->BindInputParameter(nome, 1, PARAMTYPE_String);
						db->Execute();
					}
				}
			}
			else
			{
				if (iTimeLeft > 1)
				{
					if (db->Prepare("INSERT INTO PremiumData (CharacterName,TimeLeft,ItemType) VALUES (?,?,?)"))
					{
						db->BindInputParameter(nome, 1, PARAMTYPE_String);
						db->BindInputParameter(&iTimeLeft, 2, PARAMTYPE_Integer);
						db->BindInputParameter(&iItemType, 3, PARAMTYPE_Integer);
						db->Execute();
					}
				}
				else
				{
					if (db->Prepare("DELETE FROM PremiumData WHERE CharacterName=?"))
					{
						db->BindInputParameter(nome, 1, PARAMTYPE_String);
						db->Execute();
					}
				}
			}
		}
		db->Close();
	}

}

void PVPRanking::sendKillStreak(rsPLAYINFO* lpPlayInfo, rsPLAYINFO* lpPlayInfo2, int killStreak) {

	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = 0x48480062;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.LParam = 0;
	smTransCommand.WParam = lpPlayInfo2->iKillStreak;
	smTransCommand.EParam = 0;
	lpPlayInfo2->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	/*
	if (killStreak == 3) {

		TRANS_CHATMESSAGE	Login;
		wsprintf(Login.szMessage, "PVP> TRIPLE KILL de %s!", lpPlayInfo2->szName);
		Login.code = smTRANSCODE_WHISPERMESSAGE;
		Login.size = 32 + lstrlen(Login.szMessage);
		Login.dwIP = 3;
		Login.dwObjectSerial = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (lpPlayInfo[cnt].lpsmSock)
			{
				lpPlayInfo[cnt].lpsmSock->Send((char*)&Login, Login.size, TRUE);
			}
		}

	}

	if (killStreak == 4) {

		TRANS_CHATMESSAGE	Login;
		wsprintf(Login.szMessage, "PVP> QUADRA KILL de %s!", lpPlayInfo2->szName);
		Login.code = smTRANSCODE_WHISPERMESSAGE;
		Login.size = 32 + lstrlen(Login.szMessage);
		Login.dwIP = 3;
		Login.dwObjectSerial = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (lpPlayInfo[cnt].lpsmSock)
			{
				lpPlayInfo[cnt].lpsmSock->Send((char*)&Login, Login.size, TRUE);
			}
		}

	}

	if (killStreak == 5) {

		TRANS_CHATMESSAGE	Login;
		wsprintf(Login.szMessage, "PVP> PENTA KILL de %s!", lpPlayInfo2->szName);
		Login.code = smTRANSCODE_WHISPERMESSAGE;
		Login.size = 32 + lstrlen(Login.szMessage);
		Login.dwIP = 3;
		Login.dwObjectSerial = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (lpPlayInfo[cnt].lpsmSock)
			{
				lpPlayInfo[cnt].lpsmSock->Send((char*)&Login, Login.size, TRUE);
			}
		}

		}
		*/
		
		
	
}

