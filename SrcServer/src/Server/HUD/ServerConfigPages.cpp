#include "HUD/ServerConfigPages.h"

#include <Windows.h>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "HUD/ToolTheme.h"
#include "imgui.h"
#include "ConnectReader.h"
#include "fileread.h"
#include "Utils/FileReader.h"

extern int rsRefreshConfig();
extern rsSERVER_CONFIG rsServerConfig;

#define U8_A  "\xC3\xA1"
#define U8_E  "\xC3\xA9"
#define U8_I  "\xC3\xAD"
#define U8_O  "\xC3\xB3"
#define U8_AN "\xC3\xA3"
#define U8_C  "\xC3\xA7"

static const char* kServerIni = "Server\\Config\\Server.ini";
static const char* kBaseIni = "Server\\Config\\Base.ini";
static const char* kExpIni = "Server\\Config\\ExpManager.ini";
static const char* kConnectIni = "Server\\Config\\Connect.ini";
static const char* kSqlIni = "Server\\Config\\SQL.ini";
static const char* kDevicesIni = "Server\\Config\\Devices.ini";

static char* NC(const char* s)
{
	return const_cast<char*>(s);
}

enum ConfigPage
{
	Cfg_Server = 0,
	Cfg_World,
	Cfg_Exp,
	Cfg_Connect,
	Cfg_Sql,
	Cfg_Devices
};

static int g_cfgPage = Cfg_Server;
static bool g_loaded = false;

static bool g_maint = false;
static bool g_agingFree = false;
static bool g_mixFree = false;
static int g_levelIni = 1;
static int g_levelFin = 150;
static float g_expMul[151] = {};
static int g_expFrom = 1;
static int g_expTo = 30;
static char g_connectIp[64] = {};
static int g_connectPort = 0;
static char g_clan[128] = {};
static char g_sqlHost[128] = {};
static char g_sqlUser[64] = {};
static char g_sqlPass[64] = {};
static char g_devName[64] = {};
static int g_devMaxUsers = 0;
static int g_devVerMin = 0;
static int g_devVerMax = 0;
static int g_devPk = 0;
static char g_absDir[MAX_PATH] = {};

static void ResolveAbs(char* out, size_t outLen, const char* rel)
{
	char cwd[MAX_PATH] = {};
	GetCurrentDirectoryA(MAX_PATH, cwd);
	sprintf_s(out, outLen, "%s\\%s", cwd, rel);
}

static void BackupIfMissing(const char* rel)
{
	char src[MAX_PATH] = {};
	char dst[MAX_PATH] = {};
	ResolveAbs(src, MAX_PATH, rel);
	sprintf_s(dst, "%s.bak", src);
	if (GetFileAttributesA(src) == INVALID_FILE_ATTRIBUTES)
		return;
	CopyFileA(src, dst, TRUE);
}

static bool FileExistsRel(const char* rel)
{
	char path[MAX_PATH] = {};
	ResolveAbs(path, MAX_PATH, rel);
	return GetFileAttributesA(path) != INVALID_FILE_ATTRIBUTES;
}

static bool ParseTrue(const char* s)
{
	return s && _stricmp(s, "True") == 0;
}

static void LoadAll()
{
	GetCurrentDirectoryA(MAX_PATH, g_absDir);

	char buf[256] = {};
	LeIniStr(NC("Server"), NC("Maintenance"), NC(kServerIni), buf);
	g_maint = ParseTrue(buf);
	buf[0] = 0;
	LeIniStr(NC("Event"), NC("AgingFree"), NC(kServerIni), buf);
	g_agingFree = ParseTrue(buf);
	buf[0] = 0;
	LeIniStr(NC("Event"), NC("MixFree"), NC(kServerIni), buf);
	g_mixFree = ParseTrue(buf);

	g_levelIni = LeIniInt(NC("Config"), NC("LevelInicial"), NC(kBaseIni));
	g_levelFin = LeIniInt(NC("Config"), NC("LevelFinal"), NC(kBaseIni));
	if (g_levelIni <= 0) g_levelIni = 1;
	if (g_levelFin <= 0) g_levelFin = 150;

	for (int x = 1; x <= 150; x++)
	{
		char key[32] = {};
		char val[16] = {};
		sprintf_s(key, "Nv[%d]", x);
		LeIniStr(NC("ExpConfig"), key, NC(kExpIni), val);
		g_expMul[x] = val[0] ? (float)atof(val) : 1.0f;
	}

	LeIniStr(NC("ConnectServer"), NC("IP"), NC(kConnectIni), g_connectIp);
	g_connectPort = LerPortaConnect();
	{
		const std::string clan = LerClanConnect();
		strncpy_s(g_clan, clan.c_str(), _TRUNCATE);
	}

	CFILE::Ini sql(kSqlIni);
	strncpy_s(g_sqlHost, sql.ReadString("Database", "Host").c_str(), _TRUNCATE);
	strncpy_s(g_sqlUser, sql.ReadString("Database", "User").c_str(), _TRUNCATE);
	strncpy_s(g_sqlPass, sql.ReadString("Database", "Password").c_str(), _TRUNCATE);

	strncpy_s(g_devName, rsServerConfig.szServerName, _TRUNCATE);
	g_devMaxUsers = (int)rsServerConfig.ConnectUserMax;
	g_devVerMin = rsServerConfig.LimitVersion;
	g_devVerMax = rsServerConfig.LimitVersionMax;
	g_devPk = rsServerConfig.Enable_PK;

	char devicesPath[MAX_PATH] = {};
	ResolveAbs(devicesPath, MAX_PATH, kDevicesIni);
	std::ifstream in(devicesPath);
	if (in)
	{
		std::string line;
		while (std::getline(in, line))
		{
			if (line.size() >= 12 && _strnicmp(line.c_str(), "*SERVER_NAME", 12) == 0)
			{
				size_t q1 = line.find('"');
				size_t q2 = line.rfind('"');
				if (q1 != std::string::npos && q2 > q1)
					strncpy_s(g_devName, line.substr(q1 + 1, q2 - q1 - 1).c_str(), _TRUNCATE);
			}
			else if (line.size() >= 10 && _strnicmp(line.c_str(), "*MAX_USERS", 10) == 0)
				g_devMaxUsers = atoi(line.c_str() + 10);
			else if (line.size() >= 8 && _strnicmp(line.c_str(), "*VERSION", 8) == 0)
			{
				int a = 0, b = 0;
				sscanf_s(line.c_str() + 8, "%d %d", &a, &b);
				g_devVerMin = a;
				g_devVerMax = b != 0 ? b : a;
			}
			else if (line.size() >= 10 && _strnicmp(line.c_str(), "*ENABLE_PK", 10) == 0)
				g_devPk = atoi(line.c_str() + 10);
		}
	}

	g_loaded = true;
}

static void ReplaceOrAppendStar(const char* rel, const char* key, const char* rest)
{
	char path[MAX_PATH] = {};
	ResolveAbs(path, MAX_PATH, rel);
	std::ifstream in(path);
	std::vector<std::string> lines;
	bool found = false;
	const size_t keyLen = strlen(key);
	if (in)
	{
		std::string line;
		while (std::getline(in, line))
		{
			std::string check = line;
			while (!check.empty() && (check[0] == ' ' || check[0] == '\t'))
				check.erase(check.begin());
			if (_strnicmp(check.c_str(), key, (int)keyLen) == 0 &&
				(check.size() == keyLen || check[keyLen] == ' ' || check[keyLen] == '\t'))
			{
				char buf[256] = {};
				sprintf_s(buf, "%s %s", key, rest);
				lines.push_back(buf);
				found = true;
			}
			else
				lines.push_back(line);
		}
	}
	if (!found)
	{
		char buf[256] = {};
		sprintf_s(buf, "%s %s", key, rest);
		lines.push_back(buf);
	}

	std::ofstream out(path, std::ios::trunc);
	if (!out)
		return;
	for (size_t i = 0; i < lines.size(); i++)
		out << lines[i] << "\n";
}

static void DrawPathBanner(const char* rel)
{
	char abs[MAX_PATH] = {};
	ResolveAbs(abs, MAX_PATH, rel);
	ToolHint(abs);
	if (!FileExistsRel(rel))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolDanger));
		ImGui::TextUnformatted("Arquivo nao encontrado neste diretorio.");
		ImGui::PopStyleColor();
	}
}

void ServerConfig_ReloadFromDisk()
{
	g_loaded = false;
}

void ServerConfig_Draw(ServerStatusFn setStatus)
{
	if (!g_loaded)
		LoadAll();

	ImGui::TextDisabled("Pasta do exe: %s", g_absDir);
	ImGui::SameLine();
	if (ImGui::SmallButton("Ler do disco"))
		LoadAll();
	ImGui::Spacing();

	const char* tabs[] = {
		"Servidor",
		"Mundo",
		"EXP por n" U8_I "vel",
		"Conex" U8_AN "o",
		"SQL",
		"Devices"
	};
	for (int i = 0; i < 6; i++)
	{
		if (i)
			ImGui::SameLine();
		if (ToolNavItem(tabs[i], g_cfgPage == i, 118.0f))
			g_cfgPage = i;
	}

	ImGui::Spacing();
	ImGui::BeginChild("##CfgBody", ImVec2(0.0f, 0.0f), false);

	if (g_cfgPage == Cfg_Server)
	{
		ToolSection("Server.ini");
		DrawPathBanner(kServerIni);
		ImGui::Checkbox("Manuten" U8_C U8_AN "o (bloqueia login)", &g_maint);
		ImGui::Checkbox("Aging gratuito", &g_agingFree);
		ImGui::Checkbox("Mix gratuito", &g_mixFree);
		ImGui::Spacing();
		if (ToolPrimaryButton("Salvar Server.ini", ImVec2(180.0f, 30.0f)))
		{
			BackupIfMissing(kServerIni);
			WriteIniStr(NC("Server"), NC("Maintenance"), NC(kServerIni), g_maint ? NC("True") : NC("False"));
			WriteIniStr(NC("Event"), NC("AgingFree"), NC(kServerIni), g_agingFree ? NC("True") : NC("False"));
			WriteIniStr(NC("Event"), NC("MixFree"), NC(kServerIni), g_mixFree ? NC("True") : NC("False"));
			rsRefreshConfig();
			if (setStatus)
				setStatus("Server.ini salvo e recarregado.");
		}
	}
	else if (g_cfgPage == Cfg_World)
	{
		ToolSection("Base.ini");
		DrawPathBanner(kBaseIni);
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputInt("N" U8_I "vel inicial", &g_levelIni);
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputInt("N" U8_I "vel m" U8_A "ximo", &g_levelFin);
		if (g_levelIni < 1) g_levelIni = 1;
		if (g_levelFin < g_levelIni) g_levelFin = g_levelIni;
		if (g_levelFin > 150) g_levelFin = 150;
		if (ToolPrimaryButton("Salvar Base.ini", ImVec2(180.0f, 30.0f)))
		{
			BackupIfMissing(kBaseIni);
			WriteIniInt(NC("Config"), NC("LevelInicial"), NC(kBaseIni), (DWORD)g_levelIni);
			WriteIniInt(NC("Config"), NC("LevelFinal"), NC(kBaseIni), (DWORD)g_levelFin);
			rsRefreshConfig();
			if (setStatus)
				setStatus("Base.ini salvo e recarregado.");
		}
	}
	else if (g_cfgPage == Cfg_Exp)
	{
		ToolSection("ExpManager.ini");
		DrawPathBanner(kExpIni);
		ToolHint("Multiplicador de EXP por n" U8_I "vel (Nv[1]..Nv[150]).");
		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputInt("De", &g_expFrom);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputInt("Ate", &g_expTo);
		if (g_expFrom < 1) g_expFrom = 1;
		if (g_expTo > 150) g_expTo = 150;
		if (g_expTo < g_expFrom) g_expTo = g_expFrom;

		ImGui::BeginChild("##ExpTable", ImVec2(0.0f, -44.0f), true);
		if (ImGui::BeginTable("##ExpRows", 2, ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp))
		{
			ImGui::TableSetupColumn("N" U8_I "vel", ImGuiTableColumnFlags_WidthFixed, 80.0f);
			ImGui::TableSetupColumn("Multiplicador", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();
			for (int lv = g_expFrom; lv <= g_expTo; lv++)
			{
				ImGui::PushID(lv);
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", lv);
				ImGui::TableSetColumnIndex(1);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::InputFloat("##m", &g_expMul[lv], 0.1f, 1.0f, "%.2f");
				ImGui::PopID();
			}
			ImGui::EndTable();
		}
		ImGui::EndChild();

		if (ToolPrimaryButton("Salvar EXP visivel", ImVec2(180.0f, 30.0f)))
		{
			BackupIfMissing(kExpIni);
			for (int lv = g_expFrom; lv <= g_expTo; lv++)
			{
				char key[32] = {};
				char val[32] = {};
				sprintf_s(key, "Nv[%d]", lv);
				sprintf_s(val, "%.2f", g_expMul[lv]);
				WriteIniStr(NC("ExpConfig"), key, NC(kExpIni), val);
			}
			rsRefreshConfig();
			if (setStatus)
				setStatus("ExpManager.ini (faixa visivel) salvo e recarregado.");
		}
		ImGui::SameLine();
		if (ImGui::Button("Salvar 1-150", ImVec2(140.0f, 30.0f)))
		{
			BackupIfMissing(kExpIni);
			for (int lv = 1; lv <= 150; lv++)
			{
				char key[32] = {};
				char val[32] = {};
				sprintf_s(key, "Nv[%d]", lv);
				sprintf_s(val, "%.2f", g_expMul[lv]);
				WriteIniStr(NC("ExpConfig"), key, NC(kExpIni), val);
			}
			rsRefreshConfig();
			if (setStatus)
				setStatus("ExpManager.ini completo salvo e recarregado.");
		}
	}
	else if (g_cfgPage == Cfg_Connect)
	{
		ToolSection("Connect.ini");
		DrawPathBanner(kConnectIni);
		ToolHint("IP e porta do bind. Mudar a porta so vale depois de reiniciar o Server.exe.");
		ImGui::SetNextItemWidth(220.0f);
		ImGui::InputText("IP", g_connectIp, IM_ARRAYSIZE(g_connectIp));
		ImGui::SetNextItemWidth(120.0f);
		ImGui::InputInt("Porta", &g_connectPort);
		if (g_connectPort < 1) g_connectPort = 1;
		if (g_connectPort > 65535) g_connectPort = 65535;
		ImGui::SetNextItemWidth(280.0f);
		ImGui::InputText("Clan (host:porta)", g_clan, IM_ARRAYSIZE(g_clan));
		if (ToolPrimaryButton("Salvar Connect.ini", ImVec2(180.0f, 30.0f)))
		{
			BackupIfMissing(kConnectIni);
			WriteIniStr(NC("ConnectServer"), NC("IP"), NC(kConnectIni), g_connectIp);
			char portBuf[16] = {};
			sprintf_s(portBuf, "%d", g_connectPort);
			WriteIniStr(NC("ConnectServer"), NC("Port"), NC(kConnectIni), portBuf);
			WriteIniStr(NC("ConnectServer"), NC("Clan"), NC(kConnectIni), g_clan);
			if (setStatus)
				setStatus("Connect.ini salvo. Reinicie para aplicar IP/porta do socket.");
		}
	}
	else if (g_cfgPage == Cfg_Sql)
	{
		ToolSection("SQL.ini");
		DrawPathBanner(kSqlIni);
		ToolHint("ODBC abre so na subida. Salvar aqui nao reconecta o banco com o mundo ligado.");
		ImGui::SetNextItemWidth(280.0f);
		ImGui::InputText("Host", g_sqlHost, IM_ARRAYSIZE(g_sqlHost));
		ImGui::SetNextItemWidth(200.0f);
		ImGui::InputText("Usuario", g_sqlUser, IM_ARRAYSIZE(g_sqlUser));
		ImGui::SetNextItemWidth(200.0f);
		ImGui::InputText("Senha", g_sqlPass, IM_ARRAYSIZE(g_sqlPass), ImGuiInputTextFlags_Password);
		if (ToolPrimaryButton("Salvar SQL.ini", ImVec2(180.0f, 30.0f)))
		{
			BackupIfMissing(kSqlIni);
			CFILE::Ini sql(kSqlIni);
			sql.WriteString("Database", "Host", g_sqlHost);
			sql.WriteString("Database", "User", g_sqlUser);
			sql.WriteString("Database", "Password", g_sqlPass);
			if (setStatus)
				setStatus("SQL.ini salvo. Reinicie o Server.exe para reconectar.");
		}
	}
	else if (g_cfgPage == Cfg_Devices)
	{
		ToolSection("Devices.ini");
		DrawPathBanner(kDevicesIni);
		ToolHint("Arquivo de diretivas (*SERVER_NAME, *MAX_USERS...). So as chaves abaixo sao editadas; o resto do arquivo e preservado.");
		ImGui::SetNextItemWidth(240.0f);
		ImGui::InputText("Nome do servidor", g_devName, IM_ARRAYSIZE(g_devName));
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputInt("Max. jogadores", &g_devMaxUsers);
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputInt("Versao min", &g_devVerMin);
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputInt("Versao max", &g_devVerMax);
		ImGui::SetNextItemWidth(140.0f);
		ImGui::InputInt("PK (0/1)", &g_devPk);
		if (ToolPrimaryButton("Salvar Devices.ini", ImVec2(180.0f, 30.0f)))
		{
			BackupIfMissing(kDevicesIni);
			char rest[128] = {};
			sprintf_s(rest, "\"%s\"", g_devName);
			ReplaceOrAppendStar(kDevicesIni, "*SERVER_NAME", rest);
			sprintf_s(rest, "%d", g_devMaxUsers);
			ReplaceOrAppendStar(kDevicesIni, "*MAX_USERS", rest);
			sprintf_s(rest, "%d %d", g_devVerMin, g_devVerMax);
			ReplaceOrAppendStar(kDevicesIni, "*VERSION", rest);
			sprintf_s(rest, "%d", g_devPk);
			ReplaceOrAppendStar(kDevicesIni, "*ENABLE_PK", rest);
			rsRefreshConfig();
			if (setStatus)
				setStatus("Devices.ini (chaves curadas) salvo e recarregado.");
		}
	}

	ImGui::EndChild();
}
