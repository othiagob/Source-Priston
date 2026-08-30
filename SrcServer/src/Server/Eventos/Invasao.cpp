#include "..\\sinbaram\\sinLinkHeader.h"
#include <iostream>

#define SQLCONNECTION					(SQLConnection::GetConnection)

TRANS_CHATMESSAGE	TransChatMessage2;
smTRANS_COMMAND		smTransCommand2;

bool isInvasaoActive = FALSE;
int invasaoStage = 0;

void EventoInvasao::TeleportPlayer(rsPLAYINFO* lpPlayInfo) 
{

	if (isInvasaoActive)
	{
		smTRANS_COMMAND	smTransCommand2;
		smTransCommand2.code = 0x48480055;
		smTransCommand2.size = sizeof(smTRANS_COMMAND);
		smTransCommand2.LParam = 0;
		smTransCommand2.WParam = 0;
		smTransCommand2.EParam = 0;
		lpPlayInfo->lpsmSock->Send((char*)&smTransCommand2, smTransCommand2.size, TRUE);
	}
	else 
	{
		wsprintf(TransChatMessage2.szMessage, "Invasão> O evento não está ativo no momento");
		TransChatMessage2.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage2.size = 32 + lstrlen(TransChatMessage2.szMessage);
		TransChatMessage2.dwIP = 0;
		TransChatMessage2.dwObjectSerial = 0;
		lpPlayInfo->lpsmSock->Send((char*)&TransChatMessage2, TransChatMessage2.size, TRUE);
	}

}

void EventoInvasao::endInvasao(rsPLAYINFO* lpPlayInfo, int xPosition, int zPosition)
{

	if (isInvasaoActive)
	{
		TRANS_CHATMESSAGE TransChatMessage2;

		wsprintf(TransChatMessage2.szMessage, "Invasão> O evento terminou!");
		TransChatMessage2.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage2.size = 32 + lstrlen(TransChatMessage2.szMessage);
		TransChatMessage2.dwIP = 3;
		TransChatMessage2.dwObjectSerial = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (lpPlayInfo[cnt].lpsmSock)
			{
				lpPlayInfo[cnt].lpsmSock->Send((char*)&TransChatMessage2, TransChatMessage2.size, TRUE);
			}
		}

		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480059;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++) {
			if (lpPlayInfo[cnt].lpsmSock && lpPlayInfo[cnt].dwObjectSerial) {
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
			}
		}

		isInvasaoActive = FALSE;
		invasaoStage = 0;

		smTransCommand2.WParam = 3;
		smTransCommand2.code = smTRANSCODE_WARPFIELD;
		smTransCommand2.size = sizeof(smTRANS_COMMAND);
		smTransCommand2.SParam = xPosition;
		smTransCommand2.LParam = zPosition;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++) {
			if (lpPlayInfo[cnt].lpsmSock && lpPlayInfo[cnt].dwObjectSerial) {
				if (lpPlayInfo[cnt].Position.Area == FIELD_INVASAO)
					lpPlayInfo[cnt].lpsmSock->Send((char*)&smTransCommand2, smTransCommand2.size, TRUE);
			}
		}

	}
	else 
	{
		wsprintf(TransChatMessage2.szMessage, "Invasão> evento não ativo no momento");
		TransChatMessage2.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage2.size = 32 + lstrlen(TransChatMessage2.szMessage);
		TransChatMessage2.dwIP = 0;
		TransChatMessage2.dwObjectSerial = 0;
		lpPlayInfo->lpsmSock->Send((char*)&TransChatMessage2, TransChatMessage2.size, TRUE);
	}
}


void EventoInvasao::startInvasao(rsPLAYINFO* lpPlayInfo) 
{
	if (!isInvasaoActive)
	{
		TRANS_CHATMESSAGE TransChatMessage2;

		wsprintf(TransChatMessage2.szMessage, "Invasão> O evento começou! Digite /Invasao");
		TransChatMessage2.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage2.size = 32 + lstrlen(TransChatMessage2.szMessage);
		TransChatMessage2.dwIP = 3;
		TransChatMessage2.dwObjectSerial = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++)
		{
			if (lpPlayInfo[cnt].lpsmSock)
			{
				lpPlayInfo[cnt].lpsmSock->Send((char*)&TransChatMessage2, TransChatMessage2.size, TRUE);
			}
		}

		isInvasaoActive = TRUE;
		invasaoStage = 1;


		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48480061;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.LParam = 0;
		smTransCommand.WParam = 0;
		smTransCommand.EParam = 0;

		for (int cnt = 0; cnt < CONNECTMAX; cnt++) {
			if (lpPlayInfo[cnt].lpsmSock && lpPlayInfo[cnt].dwObjectSerial) {
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
			}
		}

	}
	else 
	{
		wsprintf(TransChatMessage2.szMessage, "Invasão> Evento já está ativo!");
		TransChatMessage2.code = smTRANSCODE_WHISPERMESSAGE;
		TransChatMessage2.size = 32 + lstrlen(TransChatMessage2.szMessage);
		TransChatMessage2.dwIP = 0;
		TransChatMessage2.dwObjectSerial = 0;
		lpPlayInfo->lpsmSock->Send((char*)&TransChatMessage2, TransChatMessage2.size, TRUE);
	}

}

