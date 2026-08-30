#pragma once

#include <Windows.h>
#include <string>

class ProcessLogin
{
	enum Return_Types
	{
		INVALID = 0,
		LOGIN_OK = 1,
		NO_ACCOUNT = -1,
		WRONG_PASSWORD = -2,
		ACCOUNT_BANNED = -3
	};

public:
	int Login(const std::string& ID, const std::string& password);

private:
};

