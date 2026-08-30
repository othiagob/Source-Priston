#include "GM.h"
#include "..\\Database\SQLConnection.h"
#include <iostream>
#include <memory>
#include <Chat/ChatServer.h>

using namespace std;

GameMasters gms;

std::vector<GameMasters*> vGameMasters;
extern int DisconnectUser(smWINSOCK* lpsmSock);

GameMasters::GameMasters()
{
}

GameMasters::~GameMasters()
{
}

int GameMasters::Alert(rsPLAYINFO* lpPlayInfo, char szMessage[256])
{
	SERVERCHAT->SendChat(lpPlayInfo, CHATCOLOR_Global, szMessage);
	return true;
}

int GameMasters::PacketToAll(int code, int lParam, int WParam, int EParam, int SParam)
{
	smTRANS_COMMAND	Packet;
	Packet.code = code;
	Packet.size = sizeof(smTRANS_COMMAND);
	Packet.LParam = lParam;
	Packet.WParam = WParam;
	Packet.EParam = EParam;
	Packet.SParam = SParam;

	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock)
		{
			SENDPACKET(&rsPlayInfo[i], (char*)&Packet, Packet.size);
		}
	}

	return true;
}

void GameMasters::PacketChatToClanPlayers(rsPLAYINFO* Player, PacketItemLinkChat* Packet)
{
	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock && rsPlayInfo[i].dwObjectSerial)
		{
			if (rsPlayInfo[i].dwClanCode == Player->dwClanCode && rsPlayInfo[i].lpsmSock != Player->lpsmSock)
				SENDPACKET(&rsPlayInfo[i], (char*)Packet, Packet->size);
		}
	}
}

void GameMasters::PacketChatToAllPlayers(rsPLAYINFO* Player, PacketItemLinkChat* Packet)
{
	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock && rsPlayInfo[i].dwObjectSerial)
		{
			SENDPACKET(&rsPlayInfo[i], (char*)Packet, Packet->size);
		}
	}
}

void GameMasters::PacketChatToNearPlayers(rsPLAYINFO* Player, PacketItemLinkChat* Packet)
{
	for (int i = 0; i < CONNECTMAX; i++)
	{
		if (rsPlayInfo[i].lpsmSock && rsPlayInfo[i].dwObjectSerial)
		{
			if (rsPlayInfo[i].Position.Position.WithinPTDistance(&Player->Position.Position, USER_TALKRANGE))
				SENDPACKET(&rsPlayInfo[i], (char*)Packet, Packet->size);
		}
	}
}


int GameMasters::Packet(rsPLAYINFO* Player, int code, int lParam, int WParam, int EParam, int SParam) {
	smTRANS_COMMAND	Packet;
	Packet.code = code;
	Packet.size = sizeof(smTRANS_COMMAND);
	Packet.LParam = lParam;
	Packet.WParam = WParam;
	Packet.EParam = EParam;
	Packet.SParam = SParam;
	SENDPACKET(Player, (char*)&Packet, Packet.size, TRUE);
	return true;
}

void GameMasters::AddExpToPlayer(rsPLAYINFO* Player, INT64 Exp) {
	sAddExp	Packet;

	Packet.code = PACKET_ADD_EXP;
	Packet.size = sizeof(sAddExp);
	Packet.Exp = Exp;

	SENDPACKET(Player, (char*)&Packet, Packet.size, TRUE);
}

int GameMasters::Ban(rsPLAYINFO* lpPlayInfo, bool isOnline, char* id, char* reason) {

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		if (isOnline && lpPlayInfo != nullptr)
		{
			const char* const query = "UPDATE Users SET blocked=1 WHERE username=?";

			if (db->Prepare(query))
			{
				db->BindInputParameter(lpPlayInfo->szID, 1, PARAMTYPE_String);
				db->Execute();
			}

			auto db2 = SQLConnection::GetConnection(DATABASEID_PainelDB);

			if (db2 && db2->Open())
			{
				char admin[50] = { 0 };
				sprintf_s(admin, sizeof(admin), "Server");

				char lock[50] = { 0 };
				sprintf_s(lock, sizeof(lock), "2030-12-12 00:00:00.000");

				const char* const query2 = "IF NOT EXISTS(SELECT * FROM Banneds WHERE Account = ?) BEGIN INSERT INTO Banneds(Account, Character, Date, Reason, Operator, Unlock) VALUES(?, ?, GETDATE(), ?, ?, ?) END ELSE BEGIN UPDATE Banneds SET Character = ?, Date = GETDATE(), Reason = ?, Operator = ?, Unlock = ? WHERE Account = ? END";

				if (db2->Prepare(query2))
				{
					db2->BindInputParameter(lpPlayInfo->szID, 1, PARAMTYPE_String);
					db2->BindInputParameter(lpPlayInfo->szID, 2, PARAMTYPE_String);
					db2->BindInputParameter(lpPlayInfo->smCharInfo.szName, 3, PARAMTYPE_String);
					db2->BindInputParameter(reason, 4, PARAMTYPE_String);
					db2->BindInputParameter(admin, 5, PARAMTYPE_String);
					db2->BindInputParameter(lock, 6, PARAMTYPE_String);
					db2->BindInputParameter(lpPlayInfo->smCharInfo.szName, 7, PARAMTYPE_String);
					db2->BindInputParameter(reason, 8, PARAMTYPE_String);
					db2->BindInputParameter(admin, 9, PARAMTYPE_String);
					db2->BindInputParameter(lock, 10, PARAMTYPE_String);
					db2->BindInputParameter(lpPlayInfo->szID, 11, PARAMTYPE_String);
					db2->Execute();
				}

				db2->Close();
			}

			DisconnectUser(lpPlayInfo->lpsmSock);
		}
		else
		{
			const char* const query = "UPDATE Users SET blocked=1 WHERE username=?";

			if (db->Prepare(query))
			{
				db->BindInputParameter(id, 1, PARAMTYPE_String);
				db->Execute();
			}

			auto db2 = SQLConnection::GetConnection(DATABASEID_PainelDB);

			if (db2 && db2->Open())
			{
				char character[50] = { 0 };
				sprintf_s(character, sizeof(character), "Player");

				char admin[50] = { 0 };
				sprintf_s(admin, sizeof(admin), "Server");

				char lock[50] = { 0 };
				sprintf_s(lock, sizeof(lock), "2030-12-12 00:00:00.000");

				const char* const query2 = "IF NOT EXISTS(SELECT * FROM Banneds WHERE Account = ?) BEGIN INSERT INTO Banneds(Account, Character, Date, Reason, Operator, Unlock) VALUES(?, ?, GETDATE(), ?, ?, ?) END ELSE BEGIN UPDATE Banneds SET Character = ?, Date = GETDATE(), Reason = ?, Operator = ?, Unlock = ? WHERE Account = ? END";

				if (db2->Prepare(query2))
				{
					db2->BindInputParameter(id, 1, PARAMTYPE_String);
					db2->BindInputParameter(id, 2, PARAMTYPE_String);
					db2->BindInputParameter(character, 3, PARAMTYPE_String);
					db2->BindInputParameter(reason, 4, PARAMTYPE_String);
					db2->BindInputParameter(admin, 5, PARAMTYPE_String);
					db2->BindInputParameter(lock, 6, PARAMTYPE_String);
					db2->BindInputParameter(character, 7, PARAMTYPE_String);
					db2->BindInputParameter(reason, 8, PARAMTYPE_String);
					db2->BindInputParameter(admin, 9, PARAMTYPE_String);
					db2->BindInputParameter(lock, 10, PARAMTYPE_String);
					db2->BindInputParameter(id, 11, PARAMTYPE_String);
					db2->Execute();
				}

				db2->Close();
			}
		}

		db->Close();
	}

	return true;
}

int GameMasters::UnBan(char* id) {

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db->Open())
	{
		const char* const query = "UPDATE Users SET blocked=0 WHERE username=?";

		if (db->Prepare(query))
		{
			db->BindInputParameter(id, 1, PARAMTYPE_String);
			db->Execute();

		}
		db->Close();
	}
	return false;
}


bool GameMasters::readFromDatabase()
{
	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	char szAccountName[32] = { 0 };
	int iGameMasterLevel = 0;
	vGameMasters.clear();

	x = 0;

	if (db->Open())
	{
		if (db->Prepare("SELECT * FROM GameMasters WHERE GameMasterLevel > 0"))
		{
			if (db->Execute())
			{
				while (true)
				{
					db->GetData(2, PARAMTYPE_String, szAccountName, sizeof(szAccountName));
					db->GetData(3, PARAMTYPE_Integer, &iGameMasterLevel);

					GameMasters* p = new GameMasters();

					ZeroMemory(p->szAccountName, 32);
					wsprintf(p->szAccountName, szAccountName);

					p->iGameLevel = iGameMasterLevel;

					vGameMasters.push_back(p);

					gmLIST[x].gmLevel = iGameMasterLevel;
					gmLIST[x].gmName = (string)szAccountName;
					x++;

					if (!db->NextRow())
						break;
				}
			}
		}
		db->Close();

		return true;
	}

	return false;
}

void GameMasters::print() const
{
}

GameMasters* GameMasters::getInstance()
{
	return &gms;
}

