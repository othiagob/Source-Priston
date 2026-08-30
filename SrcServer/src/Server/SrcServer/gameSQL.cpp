#define SQLSOR_CPP

#include <windows.h>
#include <windowsx.h>
#include <winbase.h>
#include <stdio.h>
#include <tchar.h>

#include "Utils/common.h"
#include "GameSql.h"

// Declaração externa da função LeIniStr
extern void LeIniStr(char* Section, char* Key, char* szFileIni, char* Var1);

#ifdef _W_SERVER

//±âÅ¸ ÀÓ½Ã ±â·Ï ÆÄÀÏ·Î ³²±è
int Record_TempLogFile(char* szMessage);


SQLDATA::SQLDATA()
{
}

SQLDATA::~SQLDATA()
{
}

/*///////////////////////////////////////////////////////////////////////////
: ÇÔ¼ö  ¸í :  Start_ODBC(), End_ODBC()
: ÇÏ´Â  ÀÏ :  ¼ÂÆÃÇÔ¼ö
: ÀÎ    ÀÚ :
: ¹ÝÈ¯  °ª :
///////////////////////////////////////////////////////////////////////////*/

void SQLDATA::ShowErrorInfo(SQLRETURN rc, SQLSMALLINT hType, SQLHANDLE h)
{
	SQLRETURN retcode;
	SQLSMALLINT iRecord = 1;
	SQLCHAR szSqlState[REM_LEN];
	SQLINTEGER pfNativeError;
	SQLCHAR szErrorMsg[REM_LEN];
	SQLSMALLINT cbErrorMsgMax = REM_LEN - 1;
	SQLSMALLINT pcbErrorMsg;


	Utils_Log(LOG_ERROR, "Handle type: %s\n", (hType == SQL_HANDLE_STMT) ? "Statement" :
		(hType == SQL_HANDLE_ENV) ? "Environment" :
		(hType == SQL_HANDLE_DBC) ? "DBC" : "???");


	retcode = SQLGetDiagRec(hType, h, // SQL_HANDLE_STMT, m_hstmt,
		iRecord, szSqlState, &pfNativeError,
		szErrorMsg, cbErrorMsgMax, &pcbErrorMsg);


	Utils_Log(LOG_ERROR, " *** %s *** sqlstate '%s'       errormsg '%s'\n",
		(rc == SQL_ERROR) ? "SQL_ERROR" :
		(rc == SQL_SUCCESS_WITH_INFO) ? "SQL_SUCCESS_WITH_INFO" : "",
		szSqlState, szErrorMsg);

	/*
	if (m_nLastError == 0)
	{
	m_nLastError = rc;
	m_strLastErrorSqlState = (char*)szSqlState;
	m_strLastErrorMessage = (char*)szErrorMsg;
	}
	*/
}

int SQLDATA::Start_ODBC()
{
	char* svr_name;
	char* user_name;
	char* user_pswd;

	if (Odbc_Config.Dsn[0])
	{
		svr_name = Odbc_Config.Dsn;
		user_name = Odbc_Config.Name;
		user_pswd = Odbc_Config.Password;
	}
	else
	{
		// CORREÇÃO: Ler configurações do arquivo SQL.ini
		static char szHost[128] = { 0 };
		static char szUser[128] = { 0 };
		static char szPassword[128] = { 0 };
		
		// Ler configurações do SQL.ini
		LeIniStr("Database", "Host", "Server\\Config\\SQL.ini", szHost);
		LeIniStr("Database", "User", "Server\\Config\\SQL.ini", szUser);
		LeIniStr("Database", "Password", "Server\\Config\\SQL.ini", szPassword);
		
		// Verificar se conseguiu ler as configurações
		if (strlen(szHost) > 0 && strlen(szUser) > 0 && strlen(szPassword) > 0)
		{
			svr_name = szHost;
			user_name = szUser;
			user_pswd = szPassword;
			
		//	std::cout << "SQL Config carregado do SQL.ini - Host: " << svr_name << std::endl;
		}
		else
		{
			// Fallback para valores padrão se não conseguir ler do arquivo
			svr_name = "LOCALHOST\\SQLEXPRESS";
			user_name = "sa";
			user_pswd = "PadraoDraco123@";

			std::cout << "SQL Config usando valores padrão - Host: " << svr_name << std::endl;
		}
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env_hdl))
	{
		Utils_Log(LOG_ERROR, "SQLDATA: Unable to allocate handle for environment.");
		return 0;
	}

	if (SQL_SUCCESS != SQLSetEnvAttr(env_hdl, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
	{
		Utils_Log(LOG_ERROR, "SQLDATA: Unable to set ODBC version.");
		return 0;
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, env_hdl, &conn_hdl))
	{
		Utils_Log(LOG_ERROR, "SQLDATA: Unable to allocate handle for connection.");
		return 0;
	}

	wsprintf(connectString, "DRIVER={SQL Server};SERVER=%s;DATABASE=UserDB;UID=%s;PWD=%s;", svr_name, user_name, user_pswd);
	Utils_Log(LOG_DEBUG, "User Sql Config Server: %s  User: %s Pass: %s", svr_name, user_name, user_pswd);

	SQLRETURN retcode = SQLDriverConnect(conn_hdl, NULL, (SQLCHAR*)TEXT(connectString), SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
	if (SQL_SUCCESS != retcode && SQL_SUCCESS_WITH_INFO != retcode)
	{
		ShowErrorInfo(retcode, SQL_HANDLE_DBC, conn_hdl);
		return 0;
	}

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, conn_hdl, &stmt_hdl))
	{
		Utils_Log(LOG_ERROR, "SQLAllocHandle failed. Error 4");
		return 0;
	}

	return 1;
}

int SQLDATA::End_ODBC()
{
	int r;

	if (stmt_hdl != NULL)
	{
		r = SQLFreeHandle(SQL_HANDLE_STMT, stmt_hdl);
		if (r == SQL_ERROR)
			return 0;
	}

	if (conn_hdl != NULL)
	{
		r = SQLDisconnect(conn_hdl);
		if (r == SQL_ERROR)
			return 0;
		r = SQLFreeHandle(SQL_HANDLE_DBC, conn_hdl);
		if (r == SQL_ERROR)
			return 0;
	}

	if (env_hdl != NULL)
	{
		r = SQLFreeHandle(SQL_HANDLE_ENV, env_hdl);
		if (r == SQL_ERROR)
			return 0;
	}
	return 1;
}

int SQLDATA::IsGameMaster(char* szID)
{
	return TRUE;
}

int	SQLDATA::LogOn(char* szID, char* szPassword)
{
	char id[128] = { 0, };
	char pass[128] = { 0, };
	char  block;
	//char	Temp[256];
	int r;

	///////////////// ID Find a table according to the first letter ////////////////////// 

	wsprintf(stmt_buf, "SELECT * FROM UserInfo WHERE Account='%s'", szID);


	//wsprintf( stmt_buf , "SELECT * FROM AllGameUser WHERE userid='%s'" , szID );

	//Other leaving a temporary log file
	//wsprintf( Temp , "%s\r\n" , stmt_buf );
	//Record_TempLogFile( Temp );
	r = SQLExecDirect(stmt_hdl, (unsigned char*)stmt_buf, SQL_NTS);
	if (r == SQL_ERROR)
		return 0;											//No connection

	// Changed parts
	r = SQLBindCol(stmt_hdl, 2, SQL_C_CHAR, &pass, 9, NULL);
	r = SQLBindCol(stmt_hdl, 3, SQL_C_UTINYINT, &block, 1, NULL);


	if (SQLFetch(stmt_hdl) == SQL_NO_DATA_FOUND)			//No account
		return -1;

	SQLFreeStmt(stmt_hdl, SQL_CLOSE);

	if (lstrcmpi(szPassword, pass) != 0)
		return -2;											//Password check
	if (block)
		return -3;											//Account block

	return TRUE;		//Certification Success
}

SQLDATA		SqlData;

int SqlLogOn(char* szID, char* szPass)
{
	int val;
	SqlData.Start_ODBC();
	val = SqlData.LogOn(szID, szPass);
	SqlData.End_ODBC();

	return val;
}

#endif

int	InitODBC()
{
	return TRUE;
}

int	CloseODBC()
{
	return TRUE;
}
