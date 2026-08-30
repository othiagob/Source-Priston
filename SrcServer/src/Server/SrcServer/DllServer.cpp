#include <windows.h>
#include <io.h>
#include <stdio.h>         
#include <string.h>        
#include <memory.h>
#include <process.h>       
#include "Resource/resource.h"
#include "Database\\SQLConnection.h"
#include "smwsock.h"
#include "DllServer.h"
#include "onserver.h"
#include "SQL.h"

#define		BILLING_METER_RATE

#define	SWM_DISPDEBUG	(WM_USER+77)

char szServerName[64] = "Priston";

CRITICAL_SECTION cSqlSection;

#ifdef _W_SERVER


struct sLOG_SQLDATA
{
	DWORD	dwRecCode;
	char	szUserID[ACCOUNT_SIZE];
	char	szCharName[NAME_SIZE];
	char	szRace[16];
	char	szOccupation[16];
	int 	Occupation;
	DWORD	dwExpPoint;
	INT64	Exp64;
	DWORD	Level;
	DWORD	dwMoney;
	char	szIP[16];
	char	szGuid[128];

	char	szGPCode[16];
	time_t	DisuseDay;
	int		UsePeriod;
	int		PC_RNo;

	int		Time;


	rsPLAYINFO* lpPlayInfo;
	DWORD	dwConnectCode;

	sLOG_SQLDATA()
	{
		dwRecCode = 0;
		ZeroMemory(szUserID, sizeof(szUserID));
		ZeroMemory(szCharName, sizeof(szCharName));
		ZeroMemory(szRace, sizeof(szRace));
		ZeroMemory(szOccupation, sizeof(szOccupation));
		Occupation = 0;
		dwExpPoint = 0;
		Exp64 = 0;
		Level = 0;
		dwMoney = 0;
		ZeroMemory(szIP, sizeof(szIP));
		ZeroMemory(szGuid, sizeof(szGuid));
		ZeroMemory(szGPCode, sizeof(szGPCode));
		DisuseDay = 0;
		UsePeriod = 0;
		PC_RNo = 0;
		Time = 0;
		lpPlayInfo = nullptr;
		dwConnectCode = 0;
	}


};

#include <iostream>


using namespace std;


int	bSql_LogOn(char* lpszPID, char* lpszPWD, char* lpszIP, char* lpszGPCode, char* lpszDisuseDay, char* lpszUsePeriod, char* szGuid, char* lpszPCRNo, char* lpszGubunCode) { return TRUE; }


int	bSql_ServerDown() { return TRUE; }

int	bSql_ServerExit() { return TRUE; }



int LoadLogSqlDLL() { return TRUE; }




int InitLogSql() { return TRUE; }

int CloseLogSql() { return TRUE; }

int RecordLogSql(DWORD	dwRecCode, char* szIP, const char* lpUserID, const char* lpCharName, char* szGPCode, int Race, int JobCode, INT64 Exp64, DWORD dwLevel, DWORD dwMoney) { return TRUE; }


int RecordGameLogSql(DWORD dwRecCode, char* szIP, const char* lpUserID, const char* lpCharName, char* szGPCode, int JobCode, char* szGuid, int BlockTime) { return TRUE; }







int	bSql_Logout(const char* lpszPID, const char* lpszCID, char* lpszIP, char* lpszGPCode, time_t DisuseDay, int UsePeriod, char* szGuid, int PCRNo) { return TRUE; }


int	bSql_GetQueLogOut(char* szID) { return TRUE; }

int	bSql_GetGPCode(char* GPCode, char* PG) { return TRUE; }



int	bSql_PCUse(rsPLAYINFO* lpPlayInfo) { return TRUE; }

int	bUrs_Expined(rsPLAYINFO* lpPlayInfo) { return TRUE; }




int	bSql_PCSearchReserv(rsPLAYINFO* lpPlayInfo) { return TRUE; }


int	bSql_ChangeUserIdGpCodeGame(int PCRNo) { return TRUE; }


int	bSql_EventLogon(rsPLAYINFO* lpPlayInfo) { return TRUE; }


int	bSql_EventLogOut(rsPLAYINFO* lpPlayInfo) { return TRUE; }



int	bSql_GetExpGameTime(rsPLAYINFO* lpPlayInfo, int GamePlayTime) { return TRUE; }




int	bSql_PCRNoReservProduct(rsPLAYINFO* lpPlayInfo) { return TRUE; }

void GetDateTimeSod(char* pszDateTime)
{
	char szDate[9] = { 0 };
	char szTime[9] = { 0 };
	_strdate_s(szDate, 9);
	_strtime_s(szTime, 9);

	wsprintfA(pszDateTime, "%s %s", szDate, szTime);
}

extern void SendMessageToAll(std::string sender, std::string message);


int	bSql_SODRecord(rsPLAYINFO* lpPlayInfo, int Level, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode)
{
	// Ranking Solo
	if (SodTotalScore > 0 && SodTotalUser == 1)
	{
		if (SQL::GetInstance()->IsConnected())
		{
			char szQuery[512];
			char szDateTime[20];
			int pontuacaoAtual = 0;

			GetDateTimeSod(szDateTime);

			auto db = SQLConnection::GetConnection(DATABASEID_SoDDB);

			int i = 0;

			if (db && db->Open())
			{
				const char* const query = "SELECT * FROM SodRecord WHERE Name=? AND SodTotalSucessUser == 1";

				if (db->Prepare(query))
				{
					db->BindInputParameter(lpPlayInfo->szName, 1, PARAMTYPE_String);

					if (db->Execute())
					{
						db->GetData(7, PARAMTYPE_Integer, &pontuacaoAtual);
					}
				}

				db->Close();
			}

			SQL::GetInstance()->EnterSqlSection();

			if (SodTotalScore > pontuacaoAtual)
			{
				wsprintf(szQuery, "DELETE FROM [SodDb].[dbo].[SodRecord] WHERE [Name]='%s'", lpPlayInfo->szName);
				SQL::GetInstance()->Execute(szQuery);

				wsprintf(szQuery, "INSERT INTO [SodDb].[dbo].[SodRecord] ([ID],[Name],[cLevel],[IP],[Date],[JobCode],[SodScore],[SodKillCount],[SodTotalScore],[SodTotalUser], [SodTotalSucessUser], [Rno], [SodCode]) values ('%s','%s', %d, '%s', GETDATE(), %d, %d, %d, %d, %d, %d, %d, %d)",
					lpPlayInfo->szID, lpPlayInfo->szName, lpPlayInfo->smCharInfo.Level, lpPlayInfo->lpsmSock->szIPAddr, lpPlayInfo->smCharInfo.JOB_CODE, SodScore, SodKillCount, SodTotalScore, SodTotalUser, SodTotalSucessUser, lpPlayInfo->Bl_RNo, SodCode);

				SQL::GetInstance()->Execute(szQuery);
			}

			SQL::GetInstance()->LeaveSqlSection();
		}
	}

	// Ranking Clan
	if (SodTotalScore > 0 && SodTotalSucessUser > 1)
	{
		char szQuery[512] = { 0 };
		int pontuacaoAtual = 0;

		int i = 0;
	
		auto db = SQLConnection::GetConnection(DATABASEID_SoDDB);

		if (db && db->Open())
		{
			const char* const query = "SELECT * FROM SodRecord WHERE Name=? AND SodTotalSucessUser > 1";

			if (db->Prepare(query))
			{
				db->BindInputParameter(lpPlayInfo->szName, 1, PARAMTYPE_String);

				if (db->Execute())
				{
					db->GetData(7, PARAMTYPE_Integer, &pontuacaoAtual);
				}
			}

			db->Close();
		} 

		SQL::GetInstance()->EnterSqlSection();

		if (SodTotalScore > pontuacaoAtual)
		{
			wsprintf(szQuery, "DELETE FROM [SodDb].[dbo].[SodRecord] WHERE [Name]='%s'", lpPlayInfo->szName);
			SQL::GetInstance()->Execute(szQuery);

			wsprintf(szQuery, "INSERT INTO [SodDb].[dbo].[SodRecord] ([ID],[Name],[cLevel],[IP],[Date],[JobCode],[SodScore],[SodKillCount],[SodTotalScore],[SodTotalUser], [SodTotalSucessUser], [Rno], [SodCode]) values ('%s','%s', %d, '%s', GETDATE(), %d, %d, %d, %d, %d, %d, %d, %d)",
				lpPlayInfo->szID, lpPlayInfo->szName, lpPlayInfo->smCharInfo.Level, lpPlayInfo->lpsmSock->szIPAddr, lpPlayInfo->smCharInfo.JOB_CODE, SodScore, SodKillCount, SodTotalScore, SodTotalUser, SodTotalSucessUser, lpPlayInfo->Bl_RNo, SodCode);

			SQL::GetInstance()->Execute(szQuery);
		}

		SQL::GetInstance()->LeaveSqlSection();

		int IDX = 0;

		// Pega o Clan do Player se Tiver
		if (SQL::GetInstance()->IsConnected())
		{
			char szQuery[512] = { 0 };

			SQL::GetInstance()->EnterSqlSection();

			wsprintf(szQuery, "SELECT * FROM [ClanDB].[dbo].[UL] WHERE [ChName]='%s'", lpPlayInfo->szName);

			if (SQL::GetInstance()->Execute(szQuery))
			{
				if (SQL::GetInstance()->Fetch())
				{
					SQL::GetInstance()->GetDataValue(1, &IDX);
				}
			}

			SQL::GetInstance()->LeaveSqlSection();
		}

		// Verifica pontuação do clan
		if (IDX > 0)
		{
			if (SQL::GetInstance()->IsConnected())
			{
				char szQuery[512] = { 0 };
				char clanName[64] = { 0 };
				int clanSodScore = 0;

				SQL::GetInstance()->EnterSqlSection();

				// Aviso de novo top SOD
				wsprintf(szQuery, "SELECT TOP 1 * FROM [ClanDB].[dbo].[CL] ORDER BY Cpoint DESC");

				if (SQL::GetInstance()->Execute(szQuery))
				{
					if (SQL::GetInstance()->Fetch())
					{
						SQL::GetInstance()->GetDataValue(15, &clanSodScore);

						if (SodTotalScore > clanSodScore)
						{
							wsprintf(szQuery, "SELECT * FROM [ClanDB].[dbo].[CL] WHERE [IDX]='%d'", IDX);

							if (SQL::GetInstance()->Execute(szQuery))
							{
								if (SQL::GetInstance()->Fetch())
								{
									SQL::GetInstance()->GetDataValue(2, clanName);

									char msg[128] = { 0 };
									sprintf_s(msg, sizeof(msg), "Novo Líder! O Clã %s alcançou a pontuação de %d pontos!!!", clanName, SodTotalScore);
									SendMessageToAll("SOD", msg);
								}
							}
						}
					}
				}

				clanSodScore = 0;

				wsprintf(szQuery, "SELECT * FROM [ClanDB].[dbo].[CL] WHERE [IDX]=%d", IDX);

				if (SQL::GetInstance()->Execute(szQuery))
				{
					if (SQL::GetInstance()->Fetch())
					{
						SQL::GetInstance()->GetDataValue(15, &clanSodScore);

						if (SodTotalScore > clanSodScore)
						{
							wsprintf(szQuery, "UPDATE [ClanDB].[dbo].[CL] SET Cpoint=%d WHERE [IDX]=%d", SodTotalScore, IDX);
							SQL::GetInstance()->Execute(szQuery);
						}
					}

				}

				SQL::GetInstance()->LeaveSqlSection();
			}
		}
	}

	return TRUE;
}


int	bSql_StarPoint(rsPLAYINFO* lpPlayInfo, int StarPoint, int CashFlag) { return TRUE; }


int	bSql_PollRecord(rsPLAYINFO* lpPlayInfo, int PollKind, int PollCount, BYTE* PollData) { return TRUE; }



int	bSql_EventJoin(rsPLAYINFO* lpPlayInfo) { return TRUE; }



int	bSql_ClanMoney(rsPLAYINFO* lpPlayInfo, int Cash, int CashFlag, int Kind) { return TRUE; }


int	bSql_RecordCurrency() { return TRUE; }




int	bSql_GiveMoney(rsPLAYINFO* lpPlayInfo, int Money) { return TRUE; }





int bClan_GetTicket() { return TRUE; }


int	bClan_UserTicket(rsPLAYINFO* lpPlayInfo, int gubun) { return TRUE; }



int	bClan_InsertPoint(rsPLAYINFO* lpPlayInfo, int point) { return TRUE; }


int	bClan_GetClanMarkCode(rsPLAYINFO* lpPlayInfo) { return TRUE; }






int	b_IPRecord(char* lpszServerName, struct JUNGIP* JungIP) { return TRUE; }

int	b_IPRecordTime(char* lpszServerName, struct JUNGIP* JungIP) { return TRUE; }

int b_SqlGetQueSize() { return TRUE; }

int b_SqlGetLossCount() { return TRUE; }

int b_SqlGetQueProcessTime(DWORD* pTimeMax, DWORD* pAvgTime) { return TRUE; }


HANDLE	hLogItemThread = 0;
DWORD	dwLogItemThreadID;


int	b_PushLogItemQue(int type, int size, void* lpLogItemData) { return TRUE; }


int b_PopLogItemQue(LogITEM_QUE* lpLogItemQue) { return TRUE; }


int b_SqlGetItemQueSize() { return TRUE; }
int b_SqlGetItemLossCount() { return TRUE; }



int	b_PushLogEventQue(int type, int size, void* lpLogEventData) { return TRUE; }



int b_PopLogEventQue(LogEVENT_QUE* lpLogEventQue) { return TRUE; }


int b_SqlGetEventQueSize() { return TRUE; }
int b_SqlGetEventLossCount() { return TRUE; }







#else

int InitLogSql()
{
	return TRUE;
}
int CloseLogSql()
{
	return TRUE;
}
int RecordLogSql(DWORD	dwRecCode, char* szIP, const char* lpUserID, const char* lpCharName, char* szGPCode, int Race, int JobCode, INT64 Exp64, DWORD dwLevel, DWORD dwMoney)
{
	return TRUE;
}
int RecordGameLogSql(DWORD	dwRecCode, char* szIP, const char* lpUserID, const char* lpCharName, char* szGPCode, int JobCode, char* szGuid, int BlockTime)
{
	return TRUE;
}

int	bSql_LogOn(char* lpszPID, char* lpszPWD, char* lpszIP, char* lpszGPCode, char* lpszDisuseDay, char* lpszUsePeriod, char* szGuid, char* lpszPCRNo)
{
	return TRUE;
}

int	bSql_Logout(const char* lpszPID, const char* lpszCID, char* lpszIP, char* lpszGPCode, time_t DisuseDay, int UsePeriod, char* szGuid, int PCRNo)
{
	return TRUE;
}


int	bSql_ServerDown()
{
	return TRUE;
}
int	bSql_ServerExit()
{
	return TRUE;
}
int	bSql_GetGPCode(char* GPCode, char* PG)
{
	return TRUE;
}


int	bSql_PCUse(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	bUrs_Expined(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}


int	bSql_PCSearchReserv(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	bSql_ChangeUserIdGpCodeGame(int PCRNo)
{
	return TRUE;
}

int	bSql_EventLogon(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	bSql_EventLogOut(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}



int	bSql_PCRNoReservProduct(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}


int	b_IPRecord(char* lpszServerName, struct JUNGIP* JungIP)
{
	return TRUE;
}

int	b_IPRecordTime(char* lpszServerName, struct JUNGIP* JungIP)
{
	return TRUE;
}


int	bSql_SODRecord(rsPLAYINFO* lpPlayInfo, int Level, int SodScore, int SodKillCount, int SodTotalScore, int SodTotalSucessUser, int SodTotalUser, int SodCode)
{
	return TRUE;
}

int	bSql_StarPoint(rsPLAYINFO* lpPlayInfo, int StarPoint, int CashFlag)
{
	return TRUE;
}


int	bSql_PollData(rsPLAYINFO* lpPlayInfo, int PollKind, int PollCount, BYTE* PollData)
{
	return TRUE;
}


int	bSql_GetExpGameTime(rsPLAYINFO* lpPlayInfo, int GamePlayTime)
{
	return TRUE;
}


int	bSql_EventJoin(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	bSql_ClanMoney(rsPLAYINFO* lpPlayInfo, int Cash, int CashFlag, int Kind)
{
	return	TRUE;
}


int	bClan_GetClanMarkCode(rsPLAYINFO* lpPlayInfo)
{
	return TRUE;
}

int	bSql_RecordCurrency()
{
	return	TRUE;
}


int	bSql_GiveMoney(rsPLAYINFO* lpPlayInfo, int Money)
{
	return TRUE;
}

int b_SqlGetQueSize()
{
	return TRUE;
}
int b_SqlGetLossCount()
{
	return TRUE;
}
int b_SqlGetQueProcessTime(DWORD* pTimeMax, DWORD* pAvgTime)
{
	return TRUE;
}

int b_PopLogItemQue(LogITEM_QUE* lpLogItemQue)
{
	return TRUE;
}

int	b_PushLogItemQue(int type, int size, void* lpLogItemData)
{
	return TRUE;
}


int b_SqlGetItemQueSize()
{
	return TRUE;
}
int b_SqlGetItemLossCount()
{
	return TRUE;
}


int b_PopLogEventQue(LogEVENT_QUE* lpLogEventQue)
{
	return TRUE;
}

int	b_PushLogEventQue(int type, int size, void* lpLogEventData)
{
	return TRUE;
}


int b_SqlGetEventQueSize()
{
	return TRUE;
}
int b_SqlGetEventLossCount()
{
	return TRUE;
}



int	bClan_UserTicket(rsPLAYINFO* lpPlayInfo, int gubun)
{
	return TRUE;
}

int	bClan_InsertPoint(rsPLAYINFO* lpPlayInfo, int point)
{
	return TRUE;
}


DWORD nSprite_GetResult(DWORD nRecvNum)
{
	return TRUE;
}

#endif












