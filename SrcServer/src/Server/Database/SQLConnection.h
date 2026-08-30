#pragma once

#include <objbase.h>
#include <vector>
#include <memory>
#include <comdef.h>
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>


struct SQLInstanceRead
{
	char szHost[64];
	char szUser[64];
	char szPassword[64];

	SQLInstanceRead()
	{
		std::memset(&szHost, 0, sizeof(szHost));
		std::memset(&szUser, 0, sizeof(szUser));
		std::memset(&szPassword, 0, sizeof(szPassword));
	}
};

enum EDatabaseID
{
	DATABASEID_None,
	DATABASEID_UserDB,
	DATABASEID_UserDB_VIP,
	DATABASEID_ServerDB,
	DATABASEID_LogDB,
	DATABASEID_ClanDB,
	DATABASEID_SoDDB,
	DATABASEID_EventosDB,
	DATABASEID_ShopCoin,
	DATABASEID_Quest,
	DATABASEID_GameServer,
	DATABASEID_ITEMLogDB,
	DATABASEID_PainelDB
};

enum EDatabaseDataType
{
	PARAMTYPE_Integer,
	PARAMTYPE_String,
	PARAMTYPE_Float,
	PARAMTYPE_Int64,
	PARAMTYPE_Short,
	PARAMTYPE_Time,
	PARAMTYPE_Null = 5000,
};

struct DatabaseStructureData
{
	EDatabaseID eDatabaseID;
	char* pszDatabaseName;
};

class SQLConnection
{
private:
	DatabaseStructureData* GetDatabaseData(EDatabaseID eDatabaseID);
	SQLHANDLE hEnv;
	SQLHANDLE hConnection;
	SQLHANDLE hStatement;
	EDatabaseID eID;
	char szDatabaseName[64];
	char szQuery[2048];
	CRITICAL_SECTION sCriticalSection;

	BOOL SQLTimeStampToSystemTime(SYSTEMTIME* psSystemTime, SQL_TIMESTAMP_STRUCT* psSQLTimeStamp);
	bool show_errorSQL(unsigned int handletype, const SQLHANDLE& handle);




public:
	SQLConnection();
	virtual ~SQLConnection();
	static std::shared_ptr<SQLConnection> GetConnection(EDatabaseID eDatabaseID);
	bool Init(EDatabaseID eDatabaseID, SQLInstanceRead instance);
	BOOL Open();
	BOOL Prepare(const char* pszQuery);
	BOOL Execute(BOOL bFetch = TRUE);
	BOOL BindInputParameter(void* pParameter, int iPosition, EDatabaseDataType eDataType);
	BOOL GetData(int iPosition, EDatabaseDataType eDataType, void* pParameter, int iSize = 0);
	BOOL NextRow();
	BOOL Close();



};



void CreateSQLConnection(EDatabaseID eDatabaseID, SQLInstanceRead instance);
void openDatabase(SQLInstanceRead instance);
extern bool initializeSQL();

#define SQLCONNECTION (SQLConnection::GetConnection)