#include "EragonLair.h"
#include "..\\sinbaram\\sinLinkHeader.h"

using namespace Events;

extern int SendToAll(char* Buff, int Size);
extern rsPLAYINFO* srFindUserFromSerial(DWORD dwObjectSerial);
extern void SendMessageToAll(std::string sender, std::string message);
extern smCHAR* OpenMonsterFromName(char* szName, int x, int y, int z, rsPLAYINFO* lpPlayInfo = 0);

int EragonEventStage;

smCHAR* Eragon;

void SendStatusToAll()
{
	smTRANS_COMMAND	smTransCommand;
	smTransCommand.code = 0x50600019;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.LParam = EragonEventStage;
	smTransCommand.WParam = 0;
	smTransCommand.EParam = 0;

	SendToAll((char*)&smTransCommand, smTransCommand.size);
}

EragonLair::EragonLair()
{
	Eragon = 0;
	EragonEventStage = 0;
}

EragonLair::~EragonLair()
{
}

int EragonLair::getStage()
{
	return EragonEventStage;
}

int EragonLair::setStage(int stage)
{
	EragonEventStage = stage;

	return TRUE;
}

BOOL EragonLair::StartEvent()
{
	Eragon = OpenMonsterFromName("Eragon", 198277 * 256, 1299 * 256, 242866 * 256);

	if (!Eragon)
		return FALSE;

	SendMessageToAll("Eragon Lair", "Eragon nasceu no Covil!");

	setStage(Stage_InProgress);
	SendStatusToAll();
	
	return TRUE;
}

BOOL EragonLair::CheckEragon(smCHAR* Monster)
{
	if (!Monster)
		return FALSE;

	if (Eragon != 0 && (Monster == Eragon || Monster->dwObjectSerial == Eragon->dwObjectSerial))
	{
		SendMessageToAll("Eragon Lair", "Eragon foi derrotado!");
	}

	setStage(Stage_Finished);
	SendStatusToAll();

	//delete Eragon;
	return TRUE;
}

BOOL EragonLair::MessageBegin()
{
	SendMessageToAll("Eragon Lair", "Eragon nascerá em 5 minutos!");

	setStage(Stage_ToBegin);
	SendStatusToAll();

	return TRUE;
}


