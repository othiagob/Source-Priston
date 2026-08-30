#include "SQLConnection.h"
#include <iostream>
#include "Utils/Logs/utils_logging.h"
#include "Utils/strings.h"
#include "Utils/FileReader.h"


using namespace std;
std::vector<std::shared_ptr<SQLConnection>> vSQLConnection;


DatabaseStructureData sDatabaseData[]
{
	{ DATABASEID_UserDB, "UserDB" },
	{ DATABASEID_UserDB_VIP, "UserDB" },
	{ DATABASEID_ServerDB, "ServerDB" },
	{ DATABASEID_LogDB, "LogDB" },
	{ DATABASEID_ClanDB, "ClanDB" },
	{ DATABASEID_SoDDB, "SoDDB" },
	{ DATABASEID_EventosDB, "EventosDB" },
	{ DATABASEID_ShopCoin, "ShopCoin" },
	{ DATABASEID_Quest, "Quest" },
	{ DATABASEID_GameServer, "GameServer" },
	{ DATABASEID_ITEMLogDB, "ITEMLogDB" },
	{ DATABASEID_PainelDB, "PainelDB" }
};

SQLConnection::SQLConnection()
{

	memset(&szDatabaseName, 0, sizeof(szDatabaseName));
	memset(&szQuery, 0, sizeof(szQuery));
}

SQLConnection::~SQLConnection()
{
}

bool SQLConnection::show_errorSQL(unsigned int handletype, const SQLHANDLE& handle)
{
	SQLCHAR sqlstateSqlError[1024] = { 0 };
	SQLCHAR messageSqlError[1024] = { 0 };
	char szBufSQLError[2048] = { 0 };

	if (SQL_SUCCESS == SQLGetDiagRecA(handletype, handle, 1, sqlstateSqlError, NULL, messageSqlError, 1024, NULL))
	{
		STRINGFORMAT(szBufSQLError, "Message: %s\nSQLSTATE: %s\n", messageSqlError, sqlstateSqlError);
		//WRITEDBG(szBufSQLError);
		Utils_Log(LOG_SERVER, szBufSQLError);

		cerr << szBufSQLError << endl;
		return true;
	}

	return false;
}

void CreateSQLConnection(EDatabaseID eDatabaseID, SQLInstanceRead instance)
{

	auto pcSQL = std::make_shared<SQLConnection>();

	if (pcSQL->Init(eDatabaseID, instance))
	{

		vSQLConnection.push_back(pcSQL);
		cout << "connected with sucess!" << endl;
	}
	else
	{
		cout << "Falha ao conectar-se ao banco de dados! Log: SQL.txt" << endl;

		Utils_Log(LOG_SERVER, "Falha ao conectar-se ao banco de dados! Log: SQL.txt");

		Sleep(1500);
		exit(0);
	}


}

DatabaseStructureData* SQLConnection::GetDatabaseData(EDatabaseID eDatabaseID)
{
	for (auto& v : sDatabaseData)
	{
		if (v.eDatabaseID == eDatabaseID)
			return &v;
	}
	return NULL;
}

bool SQLConnection::Init(EDatabaseID eDatabaseID, SQLInstanceRead instance)
{
	// Copy database name to class
	strcpy_s(szDatabaseName, GetDatabaseData(eDatabaseID)->pszDatabaseName);
	eID = eDatabaseID;

	cout << "Conectando-se ao Banco de Dados: " << szDatabaseName << endl;


	// Try alloc Handle SQL ODBC
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv))
	{
		show_errorSQL(SQL_HANDLE_ENV, hStatement);
		Utils_Log(LOG_SQL, "SQLDATA: Unable to allocate handle for environment.");
		return false;
	}

	// Set Version ODBC
	if (SQL_SUCCESS != SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
	{
		Utils_Log(LOG_SQL, "SQLDATA: Unable to set ODBC version.");
		return false;
	}
	// Set statement of cursor type
	SQLSetStmtAttr(hEnv, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_DYNAMIC, 0);

	// Try alloc handle connection
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hConnection))
	{
		show_errorSQL(SQL_HANDLE_DBC, hStatement);
		Utils_Log(LOG_SQL, "SQLDATA: Unable to allocate handle for connection.");
		return false;
	}

	// Set connection timeout
	SQLSetConnectAttrA(hConnection, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);

	char szStringConnectionSQL[512];
	SQLCHAR strconn[512];
	memset(szStringConnectionSQL, 0, sizeof(szStringConnectionSQL));

	STRINGFORMAT(szStringConnectionSQL, "Persist Security Info=False; Integrated Security=False; Driver=%s; Server=%s; Database=%s; Uid=%s; Pwd=%s;", "{SQL Server}", instance.szHost, szDatabaseName, instance.szUser, instance.szPassword);

	memcpy(strconn, szStringConnectionSQL, sizeof(szStringConnectionSQL));

	SQLCHAR retconstring[1024] = { 0 };
	// Connect to driver
	SQLDriverConnectA(hConnection, NULL, strconn, SQL_NTS, retconstring, 1024, NULL, SQL_DRIVER_NOPROMPT);

	// Alloc statement of connection
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, hConnection, &hStatement))
	{
		show_errorSQL(SQL_HANDLE_DBC, hStatement);
		Utils_Log(LOG_SQL, "SQLAllocHandle failed. Error 4");
		return false;
	}

	// Create critical section to use on mutex
	InitializeCriticalSection(&sCriticalSection);

	return true;
}



BOOL SQLConnection::SQLTimeStampToSystemTime(SYSTEMTIME* psSystemTime, SQL_TIMESTAMP_STRUCT* psSQLTimeStamp)
{
	if (!psSystemTime || !psSQLTimeStamp)
		return FALSE;

	psSystemTime->wYear = psSQLTimeStamp->year;
	psSystemTime->wMonth = psSQLTimeStamp->month;
	psSystemTime->wDayOfWeek = 0;
	psSystemTime->wDay = psSQLTimeStamp->day;
	psSystemTime->wHour = psSQLTimeStamp->hour;
	psSystemTime->wMinute = psSQLTimeStamp->minute;
	psSystemTime->wSecond = psSQLTimeStamp->second;
	psSystemTime->wMilliseconds = (psSQLTimeStamp->fraction / 1000000) % 1000;

	return TRUE;
}

BOOL SQLConnection::Open()
{
	EnterCriticalSection(&sCriticalSection);
	return TRUE;
}

BOOL SQLConnection::Prepare(const char* pszQuery)
{
	// Copy Query
	strcpy_s(szQuery, pszQuery);

	// Check SQLI
	for (int i = 0; i < 2048; i++)
	{
		// End string? break it
		if (szQuery[i] == 0)
			break;

		// Have quote mark? SQLI, return error
		if (szQuery[i] == 0x27)
		{
			// Query Not secure

			return FALSE;
		}
	}

	// Free Statement
	SQLFreeStmt(hStatement, SQL_CLOSE);

	// Prepare Query error? return
	if (SQLPrepareA(hStatement, (SQLCHAR*)szQuery, SQL_NTS) == SQL_ERROR)
	{
		Utils_Log(LOG_SQL, "Error Prepare() - Query: %s", pszQuery);
		show_errorSQL(SQL_HANDLE_STMT, hStatement);
		return FALSE;
	}
	// Success
	return TRUE;
}

BOOL SQLConnection::Execute(BOOL bFetch)
{
	// Try Execute query
	if (SQLExecute(hStatement) == SQL_ERROR)
	{
		Utils_Log(LOG_SQL, "Error Execute() - Query: %s", szQuery);
		show_errorSQL(SQL_HANDLE_STMT, hStatement);
		return FALSE;
	}
	if (bFetch)
	{
		if (!NextRow())
			return FALSE;
	}
	return TRUE;
}

BOOL SQLConnection::BindInputParameter(void* pParameter, int iPosition, EDatabaseDataType eDataType)
{
	int iType = 0;
	int iSQLType = 0;
	int iLen = 0;

	switch (eDataType)
	{
	case PARAMTYPE_Integer:
		iType = SQL_C_LONG;
		iSQLType = SQL_INTEGER;
		break;
	case PARAMTYPE_String:
		iType = SQL_C_CHAR;
		iSQLType = SQL_VARCHAR;
		iLen = lstrlenA((char*)pParameter);
		break;
	case PARAMTYPE_Float:
		iType = SQL_C_FLOAT;
		iSQLType = SQL_REAL;
		break;
	case PARAMTYPE_Int64:
		iType = SQL_C_SBIGINT;
		iSQLType = SQL_BIGINT;
		break;
	case PARAMTYPE_Time:
		iType = SQL_C_TIMESTAMP;
		iSQLType = SQL_TYPE_TIMESTAMP;
		break;
	default:
		break;
	}

	if (SQLBindParameter(hStatement, iPosition, SQL_PARAM_INPUT, iType, iSQLType, iLen, 0, pParameter, 0, NULL) == SQL_ERROR)
	{
		show_errorSQL(SQL_HANDLE_STMT, hStatement);
		return FALSE;
	}
	return TRUE;
}

BOOL SQLConnection::GetData(int iPosition, EDatabaseDataType eDataType, void* pParameter, int iSize)
{
	int iType = 0;
	SQLLEN iDataSize = 0;
	SQL_TIMESTAMP_STRUCT sTimeStamp;
	void* pParam = pParameter;
	int iSizeParam = iSize;

	// Set Data Type
	switch (eDataType)
	{
	case PARAMTYPE_Integer:
		iType = SQL_C_LONG;
		break;
	case PARAMTYPE_Short:
		iType = SQL_C_SHORT;
		break;

	case PARAMTYPE_String:
		iType = SQL_C_CHAR;
		break;
	case PARAMTYPE_Float:
		iType = SQL_C_FLOAT;
		break;
	case PARAMTYPE_Int64:
		iType = SQL_C_SBIGINT;
		break;
	case PARAMTYPE_Time:
		iType = SQL_C_TYPE_TIMESTAMP;
		pParam = &sTimeStamp;
		iSizeParam = 23;
		break;
	default:
		break;
	}
	// Get Data
	if (SQLGetData(hStatement, iPosition, iType, pParam, iSizeParam, &iDataSize) == SQL_ERROR)
	{
		show_errorSQL(SQL_HANDLE_STMT, hStatement);
		return FALSE;
	}

	if (eDataType == PARAMTYPE_Time)
		SQLTimeStampToSystemTime((SYSTEMTIME*)pParameter, &sTimeStamp);

	return TRUE;
}

BOOL SQLConnection::NextRow()
{
	if (!SQL_SUCCEEDED(SQLFetch(hStatement)))
		return FALSE;
	return TRUE;
}

BOOL SQLConnection::Close()
{
	SQLFreeStmt(hStatement, SQL_CLOSE);

	SQLCancel(hStatement);

	memset(szQuery, 0, sizeof(szQuery));

	LeaveCriticalSection(&sCriticalSection);
	return TRUE;
}

void openDatabase(SQLInstanceRead instance)
{
	CreateSQLConnection(DATABASEID_UserDB, instance);
	CreateSQLConnection(DATABASEID_UserDB_VIP, instance);
	CreateSQLConnection(DATABASEID_ServerDB, instance);
	CreateSQLConnection(DATABASEID_ClanDB, instance);
	CreateSQLConnection(DATABASEID_SoDDB, instance);
	CreateSQLConnection(DATABASEID_LogDB, instance);
	CreateSQLConnection(DATABASEID_EventosDB, instance);
	CreateSQLConnection(DATABASEID_ShopCoin, instance);
	CreateSQLConnection(DATABASEID_Quest, instance);
	CreateSQLConnection(DATABASEID_GameServer, instance);
	CreateSQLConnection(DATABASEID_ITEMLogDB, instance);
	CreateSQLConnection(DATABASEID_PainelDB, instance);
}

std::shared_ptr<SQLConnection> SQLConnection::GetConnection(EDatabaseID eDatabaseID)
{
	// Find connection pointer
	for (UINT u = 0; u < vSQLConnection.size(); u++)
	{
		// Mismatch Database? return pointer of class
		if (vSQLConnection[u]->eID == eDatabaseID)
		{
			return vSQLConnection[u];
		}
	}

	return nullptr;
}

bool initializeSQL()
{
	SQLInstanceRead Instance;

	CFILE::Ini cConfigReader("Server\\Config\\SQL.ini");

	STRINGCOPY(Instance.szHost, cConfigReader.ReadString("Database", "Host").c_str());
	STRINGCOPY(Instance.szUser, cConfigReader.ReadString("Database", "User").c_str());
	STRINGCOPY(Instance.szPassword, cConfigReader.ReadString("Database", "Password").c_str());

	std::cout << "Infomation SQL Read in Archive" << std::endl;
	openDatabase(Instance);

	return true;
}
