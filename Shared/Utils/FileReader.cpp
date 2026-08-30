#include "FileReader.h"
#include "strings.h"

namespace CFILE
{
	Ini::Ini(std::string strFileName)
	{
		DWORD dwFlag;
		char szDirectory[MAX_PATH];
		dwFlag = GetCurrentDirectoryA(MAX_PATH, szDirectory);

		m_strFileName = "";

		if (dwFlag > 0)
		{
			char szFormat[MAX_PATH];

			STRINGFORMAT(szFormat, "%s\\%s", szDirectory, strFileName.c_str());

			m_strFileName = szFormat;
		}
	}
	Ini::~Ini()
	{
	}
	std::string Ini::ReadString(const char* pszSection, const char* pszKey)
	{
		char szBuffer[512];
		STRINGEMPTY(szBuffer);

		//Read
		GetPrivateProfileStringA(pszSection, pszKey, "", szBuffer, m_strFileName.length(), m_strFileName.c_str());

		return (std::string)szBuffer;
	}
	int Ini::ReadInt(const char* pszSection, const char* pszKey)
	{
		//Read
		return GetPrivateProfileIntA(pszSection, pszKey, 0, m_strFileName.c_str());
	}
	BOOL Ini::ReadOnOff(const char* pszSection, const char* pszKey)
	{
		char szBuffer[512];
		STRINGEMPTY(szBuffer);

		//Read
		GetPrivateProfileStringA(pszSection, pszKey, "", szBuffer, m_strFileName.length(), m_strFileName.c_str());

		if (STRINGCOMPAREI(szBuffer, "on"))
			return TRUE;

		return FALSE;
	}
	BOOL Ini::ReadBool(const char* pszSection, const char* pszKey)
	{
		char szBuffer[512];
		STRINGEMPTY(szBuffer);

		//Read
		GetPrivateProfileStringA(pszSection, pszKey, "", szBuffer, m_strFileName.length(), m_strFileName.c_str());

		if (STRINGCOMPAREI(szBuffer, "TRUE"))
			return TRUE;

		return FALSE;
	}
	void Ini::WriteString(const char* pszSection, const char* pszKey, std::string strText)
	{
		//Write
		WritePrivateProfileStringA(pszSection, pszKey, strText.c_str(), m_strFileName.c_str());
	}
	void Ini::WriteInt(const char* pszSection, const char* pszKey, int iValue)
	{
		char szBuffer[32];

		STRINGEMPTY(szBuffer);
		STRINGFORMAT(szBuffer, "%d", iValue);

		//Write
		WritePrivateProfileStringA(pszSection, pszKey, szBuffer, m_strFileName.c_str());
	}
	void Ini::WriteOnOff(const char* pszSection, const char* pszKey, BOOL bValue)
	{
		char szBuffer[32];

		STRINGEMPTY(szBuffer);
		STRINGFORMAT(szBuffer, "%s", bValue ? "On" : "Off");

		//Write
		WritePrivateProfileStringA(pszSection, pszKey, szBuffer, m_strFileName.c_str());
	}
	void Ini::WriteBool(const char* pszSection, const char* pszKey, BOOL bValue)
	{
		char szBuffer[32];

		STRINGEMPTY(szBuffer);
		STRINGFORMAT(szBuffer, "%s", bValue ? "True" : "False");

		//Write
		WritePrivateProfileStringA(pszSection, pszKey, szBuffer, m_strFileName.c_str());
	}
}