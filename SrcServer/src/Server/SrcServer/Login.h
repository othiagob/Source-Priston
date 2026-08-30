#pragma once

#include <cstring>
#include <string>

#include "smPacket.h"

#define ACCOUNT_SIZE 32
#define PASSWORD_SIZE 9
#define NAME_SIZE 32

struct LoginPacket
{
	int	size;
	int code;
	char account[ACCOUNT_SIZE];
	char password[PASSWORD_SIZE];
	char hash[33];
	char mac[30];

	LoginPacket()
	{
		size = 0;
		code = 0;
		memset(account, 0, sizeof(account));
		memset(password, 0, sizeof(password));
		memset(hash, 0, sizeof(hash));
		memset(mac, 0, sizeof(mac));
	}
};

enum LoginResult
{
	OK,
	FAILED_CONNECTION,
	ACCOUNT_LOGGED,
	SHORT_USERNAME,
	SHORT_PASSWORD,
	WRONG_VERSION,
	WRONG_PASSWORD_OR_USERNAME,	
	SERVER_FULL,
	SERVER_MAITENANCE,
	BLOCKED_USER
};


