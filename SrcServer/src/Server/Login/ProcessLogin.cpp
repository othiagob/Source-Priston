#include <boost/algorithm/string.hpp>
#include "ProcessLogin.h"
#include "..\\Database\SQLConnection.h"

int ProcessLogin::Login(const std::string& ID, const std::string& password)
{
	auto resultado = INVALID;

	auto db = SQLConnection::GetConnection(DATABASEID_UserDB);

	if (db && db->Open())
	{
		const char* const query = "SELECT password,blocked FROM Users WHERE username=?";

		if (db->Prepare(query))
		{
			char _username[32] = { 0 };
			char _password[64] = { 0 };

			strcpy_s(_username, ID.c_str());
			strcpy_s(_password, password.c_str());

			db->BindInputParameter(_username, 1, EDatabaseDataType::PARAMTYPE_String);

			if (db->Execute())
			{
				char __password[64] = { 0 };
				int isBanned = FALSE;

				db->GetData(1, EDatabaseDataType::PARAMTYPE_String, &__password, sizeof(__password));
				db->GetData(2, EDatabaseDataType::PARAMTYPE_Short, &isBanned);

				if (boost::equals(_password, __password))
				{
					if (!isBanned)
						resultado = LOGIN_OK;
					else
						resultado = ACCOUNT_BANNED;
				}
				else
				{
					resultado = WRONG_PASSWORD;
				}
			}
			else
			{
				resultado = NO_ACCOUNT;
			}
		}

		db->Close();
	}

	return resultado;
}