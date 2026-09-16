#include "HUD/ServerPanel.h"

#include <d3d9.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <streambuf>
#include <cctype>

#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "HUD/ToolTheme.h"
#include "HUD/ServerConfigPages.h"

#include "smwsock.h"
#include "SrcServer/onserver.h"
#include "Chat/ChatServer.h"
#include "Quest/Quest.h"
#include "GM/GM.h"
#include "ConnectReader.h"

ImFont* g_ToolFont = nullptr;
ImFont* g_ToolFontBig = nullptr;
ImFont* g_ToolFontSmall = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern int eventoxp;
extern int eventodrop;
extern BOOL bMaintenanceMode;
extern int DisconnectUser(smWINSOCK* lpsmSock);
extern rsPLAYINFO* FindUserFromName(char* szName);
extern rsPLAYINFO* FindUserFromName2(char* szName);
extern int rsRefreshConfig();
extern int rsShutDown();

#define U8_A  "\xC3\xA1"
#define U8_E  "\xC3\xA9"
#define U8_I  "\xC3\xAD"
#define U8_O  "\xC3\xB3"
#define U8_U  "\xC3\xBA"
#define U8_AN "\xC3\xA3"
#define U8_ON "\xC3\xB5"
#define U8_C  "\xC3\xA7"
#define U8_A_UP  "\xC3\x81"
#define U8_O_UP  "\xC3\x93"
#define U8_I_UP  "\xC3\x8D"
#define U8_C_UP  "\xC3\x87"
#define U8_ON_UP "\xC3\x95"
#define U8_AN_UP "\xC3\x83"

enum NavPage
{
	Nav_Status = 0,
	Nav_Players,
	Nav_Events,
	Nav_Files,
	Nav_Log,
	Nav_Actions
};

static const char* kMapNames[] = {
	"Mata das Ac" U8_A "cias",
	"F. Bamboo",
	"Jardim da Liberdade",
	"Cidade de Ricarten",
	"Ref" U8_U "gio dos A.",
	"Castelo dos Perdidos",
	"Vila Ruinen",
	"Terra Maldita",
	"Terra Esquecida",
	"Cidade de Navisko",
	"O" U8_A "sis",
	"C. B. A.",
	"Terra Proibida",
	"C. A. 1",
	"C. A. 2",
	"C. A. 3",
	"Sala do GM",
	"F. dos Esp" U8_I "ritos",
	"F. das Ilus" U8_ON "es",
	"Vale Tranquilo",
	"E. dos Ventos",
	"Cidade de Pillai",
	"Templo Maldito 1",
	"Templo Maldito 2",
	"C. dos Cogumelos",
	"Caverna das Abelhas",
	"Santu" U8_A "rio Sombrio",
	"E. d. F. do Caos",
	"Cora" U8_C U8_AN "o de Perum",
	"Eura",
	"S.o.D",
	"Vale Galubia",
	"Sala de Desafios",
	"Castelo Aben" U8_C "oado",
	"Lago da Gan" U8_AN "ncia",
	"Santu" U8_A "rio Congelado",
	"Covil do Kelvezu",
	"Ilha Perdida",
	"Templo Perdido",
	"Mapa 2D",
	"Torre sem Fim 1",
	"Torre sem Fim 2",
	"Templo Maldito 3",
	"Torre sem Fim 3",
	"Laborat" U8_O "rio Secreto",
	"Arma Antiga",
	"Mina de Gelo 1",
	"Arquip" U8_E "lago Perdido",
	"Covil do Eragon",
	"Arena de Batalha",
	"N" U8_U "cleo de Ferro",
	"Abismo do Mar"
};
static const int kMapNameCount = (int)(sizeof(kMapNames) / sizeof(kMapNames[0]));

static HMODULE g_hD3D9 = nullptr;
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static D3DPRESENT_PARAMETERS g_d3dpp = {};
static HWND g_hwnd = nullptr;
static bool g_active = false;
static bool g_booting = true;
static bool g_pumping = false;
static bool g_rendering = false;
static DWORD g_startTick = 0;
static int g_nav = Nav_Status;

static int g_editExp = 1;
static int g_editDrop = 0;
static char g_notice[200] = {};
static char g_statusMsg[256] = {};
static DWORD g_statusUntil = 0;
static char g_playerFilter[48] = {};
static char g_bootStatus[256] = {};
static int g_bootSteps = 0;
static const int kBootStepsEstimate = 28;

enum ConfirmKind
{
	Confirm_None = 0,
	Confirm_Shutdown,
	Confirm_Exit,
	Confirm_Kick
};
static ConfirmKind g_confirm = Confirm_None;
static char g_kickName[32] = {};

struct PlayerRow
{
	char name[32];
	char id[32];
	char ip[16];
	int area;
	int level;
};
static std::vector<PlayerRow> g_players;
static DWORD g_lastPlayerSnap = 0;

struct LogLine
{
	char time[12];
	std::string text;
	bool error;
};
static std::vector<LogLine> g_logLines;
static const size_t kLogMax = 400;
static bool g_logAutoScroll = true;
static CRITICAL_SECTION g_logCs;
static bool g_logCsInit = false;

static std::string ToUtf8(const char* ansi);
static bool LineLooksError(const char* s);

class PanelLogBuf : public std::streambuf
{
	std::string m_line;
protected:
	int overflow(int ch) override
	{
		if (ch == EOF)
			return traits_type::eof();
		if (ch == '\n')
		{
			if (!m_line.empty())
			{
				LogLine row = {};
				SYSTEMTIME st = {};
				GetLocalTime(&st);
				sprintf_s(row.time, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
				row.error = LineLooksError(m_line.c_str());
				row.text = ToUtf8(m_line.c_str());
				if (row.text.empty())
					row.text = m_line;

				if (g_logCsInit)
					EnterCriticalSection(&g_logCs);
				if (g_logLines.size() >= kLogMax)
					g_logLines.erase(g_logLines.begin());
				g_logLines.push_back(row);
				if (g_booting)
					g_bootSteps++;
				if (g_logCsInit)
					LeaveCriticalSection(&g_logCs);
				m_line.clear();
			}
			return traits_type::not_eof((char_type)ch);
		}
		if (ch != '\r')
			m_line += (char)ch;
		return traits_type::not_eof((char_type)ch);
	}
};

static PanelLogBuf g_logBuf;
static std::streambuf* g_oldCout = nullptr;
static std::streambuf* g_oldCerr = nullptr;

static bool LineLooksError(const char* s)
{
	if (!s || !s[0])
		return false;
	const char* keys[] = { "Falha", "falha", "Error", "error", "Unable", "Nao foi", "n" U8_AN "o foi" };
	for (int i = 0; i < 7; i++)
	{
		if (strstr(s, keys[i]))
			return true;
	}
	return false;
}

static void SetStatus(const char* msg)
{
	if (!msg)
		return;
	strncpy_s(g_statusMsg, msg, _TRUNCATE);
	g_statusUntil = GetTickCount() + 5000;

	LogLine row = {};
	SYSTEMTIME st = {};
	GetLocalTime(&st);
	sprintf_s(row.time, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
	row.text = msg;
	row.error = LineLooksError(msg);
	if (g_logCsInit)
		EnterCriticalSection(&g_logCs);
	if (g_logLines.size() >= kLogMax)
		g_logLines.erase(g_logLines.begin());
	g_logLines.push_back(row);
	if (g_logCsInit)
		LeaveCriticalSection(&g_logCs);
}

static std::string ToUtf8(const char* ansi)
{
	if (!ansi || !ansi[0])
		return std::string();

	const int wlen = MultiByteToWideChar(CP_ACP, 0, ansi, -1, nullptr, 0);
	if (wlen <= 0)
		return std::string();

	std::wstring wide((size_t)wlen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, ansi, -1, &wide[0], wlen);

	const int ulen = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (ulen <= 0)
		return std::string();

	std::string utf8((size_t)ulen, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &utf8[0], ulen, nullptr, nullptr);
	if (!utf8.empty() && utf8.back() == '\0')
		utf8.pop_back();
	return utf8;
}

static std::string Utf8ToAcp(const char* utf8)
{
	if (!utf8 || !utf8[0])
		return std::string();

	const int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, nullptr, 0);
	if (wlen <= 0)
		return std::string();

	std::wstring wide((size_t)wlen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, &wide[0], wlen);

	const int alen = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (alen <= 0)
		return std::string();

	std::string acp((size_t)alen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &acp[0], alen, nullptr, nullptr);
	if (!acp.empty() && acp.back() == '\0')
		acp.pop_back();
	return acp;
}

static const char* MapName(int area)
{
	if (area >= 0 && area < kMapNameCount)
		return kMapNames[area];
	return nullptr;
}

static bool ContainsI(const char* hay, const char* needle)
{
	if (!needle || !needle[0])
		return true;
	if (!hay || !hay[0])
		return false;
	const size_t nlen = strlen(needle);
	const size_t hlen = strlen(hay);
	if (nlen > hlen)
		return false;
	for (size_t i = 0; i + nlen <= hlen; i++)
	{
		if (_strnicmp(hay + i, needle, nlen) == 0)
			return true;
	}
	return false;
}

static bool FilterMatch(const PlayerRow& row, const char* filter)
{
	return ContainsI(row.name, filter) || ContainsI(row.id, filter) || ContainsI(row.ip, filter);
}

static bool CreateDeviceD3D(HWND hWnd)
{
	g_hD3D9 = LoadLibraryA("d3d9.dll");
	if (!g_hD3D9)
		return false;

	typedef IDirect3D9* (WINAPI* PFN_Direct3DCreate9)(UINT);
	PFN_Direct3DCreate9 pfnCreate = (PFN_Direct3DCreate9)GetProcAddress(g_hD3D9, "Direct3DCreate9");
	if (!pfnCreate)
		return false;

	g_pD3D = pfnCreate(D3D_SDK_VERSION);
	if (!g_pD3D)
		return false;

	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	g_d3dpp.hDeviceWindow = hWnd;

	RECT rc = {};
	GetClientRect(hWnd, &rc);
	g_d3dpp.BackBufferWidth = (UINT)(rc.right - rc.left);
	g_d3dpp.BackBufferHeight = (UINT)(rc.bottom - rc.top);
	if (g_d3dpp.BackBufferWidth < 1)
		g_d3dpp.BackBufferWidth = SERVER_PANEL_W;
	if (g_d3dpp.BackBufferHeight < 1)
		g_d3dpp.BackBufferHeight = SERVER_PANEL_H;

	HRESULT hr = g_pD3D->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice);
	if (FAILED(hr))
	{
		hr = g_pD3D->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice);
	}
	return SUCCEEDED(hr) && g_pd3dDevice != nullptr;
}

static void CleanupDeviceD3D()
{
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = nullptr;
	}
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = nullptr;
	}
	if (g_hD3D9)
	{
		FreeLibrary(g_hD3D9);
		g_hD3D9 = nullptr;
	}
}

static void LoadToolFonts(ImGuiIO& io)
{
	char fontPath[MAX_PATH] = {};
	GetWindowsDirectoryA(fontPath, MAX_PATH);
	strcat_s(fontPath, "\\Fonts\\segoeui.ttf");

	if (GetFileAttributesA(fontPath) == INVALID_FILE_ATTRIBUTES)
		return;

	ImFontConfig cfg;
	cfg.OversampleH = 2;
	cfg.OversampleV = 2;
	cfg.PixelSnapH = false;
	const ImWchar* ranges = io.Fonts->GetGlyphRangesDefault();
	g_ToolFont = io.Fonts->AddFontFromFileTTF(fontPath, 16.0f, &cfg, ranges);
	g_ToolFontSmall = io.Fonts->AddFontFromFileTTF(fontPath, 13.0f, &cfg, ranges);
	g_ToolFontBig = io.Fonts->AddFontFromFileTTF(fontPath, 22.0f, &cfg, ranges);
	if (g_ToolFont)
		io.FontDefault = g_ToolFont;
}

static void SnapshotPlayers()
{
	const DWORD now = GetTickCount();
	if (g_lastPlayerSnap != 0 && (now - g_lastPlayerSnap) < 400)
		return;
	g_lastPlayerSnap = now;

	if (!rsPlayInfo)
		return;

	std::vector<PlayerRow> next;
	next.reserve(64);

	EnterCriticalSection(&cSerSection);
	for (int cnt = 0; cnt < CONNECTMAX; cnt++)
	{
		rsPLAYINFO& player = rsPlayInfo[cnt];
		if (!player.lpsmSock)
			continue;

		PlayerRow row = {};
		strncpy_s(row.name, player.szName[0] ? player.szName : player.smCharInfo.szName, _TRUNCATE);
		strncpy_s(row.id, player.szID, _TRUNCATE);
		if (player.lpsmSock)
			strncpy_s(row.ip, player.lpsmSock->szIPAddr, _TRUNCATE);
		row.area = (int)player.Position.Area;
		row.level = player.smCharInfo.Level;
		next.push_back(row);
	}
	LeaveCriticalSection(&cSerSection);
	g_players.swap(next);
}

static void KickPlayer(const char* name)
{
	if (!name || !name[0])
		return;

	char nick[32] = {};
	strncpy_s(nick, name, _TRUNCATE);

	EnterCriticalSection(&cSerSection);
	rsPLAYINFO* player = FindUserFromName(nick);
	if (!player)
		player = FindUserFromName2(nick);
	if (player && player->lpsmSock)
		DisconnectUser(player->lpsmSock);
	LeaveCriticalSection(&cSerSection);

	char buf[128];
	sprintf_s(buf, "Kick: %s", nick);
	SetStatus(ToUtf8(buf).c_str());
}

static void DrawMetricGrid(const char** labels, const char** values, int count)
{
	const float avail = ImGui::GetContentRegionAvail().x;
	const float gap = 10.0f;
	const int cols = ToolCardColumns(avail);
	const float cardW = (avail - gap * (float)(cols - 1)) / (float)cols;
	for (int i = 0; i < count; i++)
	{
		if (i > 0 && (i % cols) != 0)
			ImGui::SameLine(0.0f, gap);
		else if (i > 0)
			ImGui::Dummy(ImVec2(0.0f, 0.0f));
		ToolMetricCard(labels[i], values[i], cardW);
	}
}

static void DrawStatusPage()
{
	ToolPageTitle("Resumo do mundo", "Estado ao vivo. Nada aqui grava ficheiro.");

	const DWORD elapsed = (GetTickCount() - g_startTick) / 1000;
	char onlineBuf[16], uptimeBuf[24], expBuf[16], dropBuf[16], portBuf[16], levelBuf[32], ipBuf[64];
	sprintf_s(onlineBuf, "%d", (int)g_players.size());
	sprintf_s(uptimeBuf, "%02d:%02d:%02d", (int)(elapsed / 3600), (int)((elapsed / 60) % 60), (int)(elapsed % 60));
	sprintf_s(expBuf, "x%d", eventoxp);
	sprintf_s(dropBuf, "x%d", eventodrop);
	sprintf_s(portBuf, "%d", TCP_SERVPORT);
	sprintf_s(levelBuf, "%d / %d", g_LevelInicial, g_LevelFinal);
	strncpy_s(ipBuf, LerIPConnect().c_str(), _TRUNCATE);
	if (!ipBuf[0])
		strncpy_s(ipBuf, "-", _TRUNCATE);

	ToolSection("Resumo");
	const char* labels1[] = { "Jogadores", "Uptime", "Evento EXP", "Evento DROP" };
	const char* values1[] = { onlineBuf, uptimeBuf, expBuf, dropBuf };
	DrawMetricGrid(labels1, values1, 4);

	ToolSection("Conex" U8_AN "o");
	const char* labels2[] = { "IP (Connect.ini)", "Porta" };
	const char* values2[] = { ipBuf, portBuf };
	DrawMetricGrid(labels2, values2, 2);

	ToolSection("Mundo");
	const char* labels3[] = { "Manuten" U8_C U8_AN "o", "N" U8_I "veis" };
	const char* values3[] = { bMaintenanceMode ? "Sim" : "N" U8_AN "o", levelBuf };
	DrawMetricGrid(labels3, values3, 2);
}

static void DrawPlayersPage()
{
	ToolPageTitle("Jogadores ligados", "Kick pede confirma" U8_C U8_AN "o. O filtro cobre nick, conta e IP.");

	ImGui::SetNextItemWidth(-1.0f);
	ImGui::InputTextWithHint("##PlayerFilter", "Filtrar nick, conta ou IP", g_playerFilter, IM_ARRAYSIZE(g_playerFilter));
	ImGui::Spacing();

	int visible = 0;
	for (size_t i = 0; i < g_players.size(); i++)
	{
		if (FilterMatch(g_players[i], g_playerFilter))
			visible++;
	}

	if (g_players.empty())
	{
		ToolHint("Nenhum jogador ligado.");
		return;
	}
	if (visible == 0)
	{
		ToolHint("Nenhum jogador corresponde ao filtro.");
		return;
	}

	const ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;
	if (ImGui::BeginTable("##Players", 6, flags, ImVec2(0.0f, 0.0f)))
	{
		ImGui::TableSetupColumn("Nick", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Conta", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("IP", ImGuiTableColumnFlags_WidthFixed, 118.0f);
		ImGui::TableSetupColumn("Mapa", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Nv.", ImGuiTableColumnFlags_WidthFixed, 42.0f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 72.0f);
		ImGui::TableHeadersRow();

		for (size_t i = 0; i < g_players.size(); i++)
		{
			const PlayerRow& row = g_players[i];
			if (!FilterMatch(row, g_playerFilter))
				continue;

			ImGui::PushID((int)i);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(ToUtf8(row.name).c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(ToUtf8(row.id).c_str());
			ImGui::TableSetColumnIndex(2);
			ImGui::TextUnformatted(row.ip);
			ImGui::TableSetColumnIndex(3);
			if (const char* map = MapName(row.area))
				ImGui::TextUnformatted(map);
			else
				ImGui::Text("Mapa %d", row.area);
			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", row.level);
			ImGui::TableSetColumnIndex(5);
			if (ToolDangerButton("Kick", ImVec2(64.0f, 0.0f)))
			{
				strncpy_s(g_kickName, row.name, _TRUNCATE);
				g_confirm = Confirm_Kick;
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

static void DrawEventsPage()
{
	ToolPageTitle("Eventos ao vivo", "Taxas aplicam j" U8_A " ao mundo. Recarregar INI rel" U8_E " os ficheiros do disco.");

	ToolSection("Taxas ao vivo");
	ImGui::SetNextItemWidth(160.0f);
	ImGui::InputInt("EXP (x)", &g_editExp);
	if (g_editExp < 0) g_editExp = 0;
	ImGui::SetNextItemWidth(160.0f);
	ImGui::InputInt("DROP (x)", &g_editDrop);
	if (g_editDrop < 0) g_editDrop = 0;
	if (ToolPrimaryButton("Aplicar taxas", ImVec2(180.0f, 32.0f)))
	{
		eventoxp = g_editExp;
		eventodrop = g_editDrop;
		SetStatus("Taxas de evento aplicadas.");
	}

	ToolSection("Recarregar");
	if (ImGui::Button("Configura" U8_C U8_AN "o (INI)", ImVec2(200.0f, 32.0f)))
	{
		rsRefreshConfig();
		g_editExp = eventoxp;
		g_editDrop = eventodrop;
		ServerConfig_ReloadFromDisk();
		SetStatus("Configura" U8_C U8_AN "o recarregada.");
	}
	ImGui::SameLine();
	if (ImGui::Button("Lista de GMs", ImVec2(160.0f, 32.0f)))
	{
		GameMasters::getInstance()->readFromDatabase();
		SetStatus("Lista de GMs recarregada.");
	}
	ImGui::SameLine();
	if (ImGui::Button("Desafios", ImVec2(140.0f, 32.0f)))
	{
		Quest::GetInstance()->updateQuests = true;
		Quest::GetInstance()->SendAllQuests(nullptr);
		SetStatus("Desafios recarregados. Jogadores precisam relogar.");
	}
}

static void DrawLogPage()
{
	ToolPageTitle("Registo", "Mesmas linhas que o consola antiga. Erros ficam a vermelho.");

	if (ImGui::Button("Limpar", ImVec2(100.0f, 0.0f)))
	{
		if (g_logCsInit)
			EnterCriticalSection(&g_logCs);
		g_logLines.clear();
		if (g_logCsInit)
			LeaveCriticalSection(&g_logCs);
	}
	ImGui::SameLine();
	ImGui::Checkbox("Acompanhar o fim", &g_logAutoScroll);
	ImGui::Spacing();

	std::vector<LogLine> snapshot;
	if (g_logCsInit)
		EnterCriticalSection(&g_logCs);
	snapshot = g_logLines;
	if (g_logCsInit)
		LeaveCriticalSection(&g_logCs);

	ImGui::BeginChild("##LogLines", ImVec2(0.0f, 0.0f), true);
	for (size_t i = 0; i < snapshot.size(); i++)
	{
		const LogLine& row = snapshot[i];
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolMuted));
		ImGui::TextUnformatted(row.time);
		ImGui::PopStyleColor();
		ImGui::SameLine();
		if (row.error)
			ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolDanger));
		else
			ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolText));
		ImGui::TextWrapped("%s", row.text.c_str());
		ImGui::PopStyleColor();
	}
	if (g_logAutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 24.0f)
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
}

static void DrawActionsPage()
{
	ToolPageTitle("A" U8_C U8_ON "es", "O X da janela s" U8_O " minimiza. Sair de verdade " U8_E " aqui.");

	ToolSection("Aviso global");
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::InputTextWithHint("##Notice", "Mensagem para todos os jogadores", g_notice, IM_ARRAYSIZE(g_notice));
	if (ToolPrimaryButton("Enviar aviso", ImVec2(180.0f, 32.0f)))
	{
		if (g_notice[0])
		{
			const std::string acp = Utf8ToAcp(g_notice);
			SERVERCHAT->SendChatAll(CHATCOLOR_Notice, acp.c_str());
			SetStatus("Aviso enviado a todos.");
			g_notice[0] = 0;
		}
	}

	ToolSection("Servidor");
	if (ToolDangerButton("Desligar em 8 minutos", ImVec2(220.0f, 32.0f)))
		g_confirm = Confirm_Shutdown;
	ImGui::SameLine();
	if (ToolDangerButton("Sair agora", ImVec2(140.0f, 32.0f)))
		g_confirm = Confirm_Exit;
	ImGui::Spacing();
	ToolHint("O mundo continua a correr enquanto a janela estiver minimizada.");
}

static void DrawConfirmPopup()
{
	if (g_confirm == Confirm_None)
		return;

	ImGui::OpenPopup("##ToolConfirm");
	ImGui::SetNextWindowSize(ImVec2(400.0f, 188.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##ToolConfirm", nullptr, flags))
	{
		const char* title = "Confirmar";
		const char* hint = "";
		std::string kickUtf8;
		if (g_confirm == Confirm_Shutdown)
		{
			title = "Desligar o mundo?";
			hint = "Fecha em 8 minutos (igual shutdown;).";
		}
		else if (g_confirm == Confirm_Exit)
		{
			title = "Sair agora?";
			hint = "O servidor encerra na hora.";
		}
		else
		{
			title = "Expulsar jogador?";
			kickUtf8 = ToUtf8(g_kickName);
			hint = kickUtf8.empty() ? g_kickName : kickUtf8.c_str();
		}

		if (g_ToolFontBig)
			ImGui::PushFont(g_ToolFontBig);
		ImGui::TextUnformatted(title);
		if (g_ToolFontBig)
			ImGui::PopFont();
		ImGui::Spacing();
		ToolHint(hint);
		ImGui::Dummy(ImVec2(0.0f, 18.0f));
		if (ImGui::Button("Voltar", ImVec2(128.0f, 32.0f)))
		{
			g_confirm = Confirm_None;
			g_kickName[0] = 0;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ToolDangerButton("Confirmar", ImVec2(128.0f, 32.0f)))
		{
			if (g_confirm == Confirm_Shutdown)
			{
				rsShutDown();
				SetStatus("Shutdown iniciado (8 minutos).");
			}
			else if (g_confirm == Confirm_Kick)
				KickPlayer(g_kickName);
			else
				PostQuitMessage(0);
			g_confirm = Confirm_None;
			g_kickName[0] = 0;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

static void DrawSplash()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

	PushToolStyle();
	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;
	ImGui::Begin("##ServerBoot", nullptr, flags);

	const ImVec2 size = ImGui::GetWindowSize();
	const float cardW = 520.0f;
	const float cardH = 360.0f;
	const float cardX = (size.x - cardW) * 0.5f;
	const float cardY = (size.y - cardH) * 0.5f;
	const ImVec2 origin(ImGui::GetWindowPos().x + cardX, ImGui::GetWindowPos().y + cardY);
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(origin, ImVec2(origin.x + cardW, origin.y + cardH), kToolSurface, 6.0f);
	draw->AddRect(origin, ImVec2(origin.x + cardW, origin.y + cardH), kToolBorder, 6.0f, 0, 1.0f);
	draw->AddRectFilled(origin, ImVec2(origin.x + 4.0f, origin.y + cardH), kToolAccent, 6.0f, ImDrawFlags_RoundCornersLeft);

	ImGui::SetCursorPos(ImVec2(cardX + 36.0f, cardY + 28.0f));
	ImGui::BeginGroup();
	if (g_ToolFontBig)
		ImGui::PushFont(g_ToolFontBig);
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolAccent));
	ImGui::TextUnformatted("Servidor");
	ImGui::PopStyleColor();
	if (g_ToolFontBig)
		ImGui::PopFont();
	ToolHint("A iniciar o mundo. Bancos, mapas e sistemas.");

	ImGui::Dummy(ImVec2(0.0f, 12.0f));
	const ImVec2 sp = ImGui::GetCursorScreenPos();
	ToolSpinner(ImVec2(sp.x + 16.0f, sp.y + 16.0f), 14.0f, 2.6f);
	ImGui::Dummy(ImVec2(40.0f, 32.0f));
	ImGui::SameLine();

	std::string current;
	std::vector<LogLine> recent;
	if (g_logCsInit)
		EnterCriticalSection(&g_logCs);
	if (!g_logLines.empty())
		current = g_logLines.back().text;
	const size_t n = g_logLines.size();
	const size_t from = n > 6 ? n - 6 : 0;
	for (size_t i = from; i < n; i++)
		recent.push_back(g_logLines[i]);
	const int steps = g_bootSteps;
	if (g_logCsInit)
		LeaveCriticalSection(&g_logCs);

	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(0.0f, 6.0f));
	const char* step = g_bootStatus[0] ? g_bootStatus : (current.empty() ? "A preparar..." : current.c_str());
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolText));
	ImGui::PushTextWrapPos(cardX + cardW - 48.0f);
	ImGui::TextUnformatted(step);
	ImGui::PopTextWrapPos();
	ImGui::PopStyleColor();
	ImGui::EndGroup();

	ImGui::Dummy(ImVec2(0.0f, 10.0f));
	float frac = (float)steps / (float)kBootStepsEstimate;
	if (frac > 0.92f)
		frac = 0.92f;
	ToolProgressBar(frac, cardW - 72.0f);

	ImGui::Dummy(ImVec2(0.0f, 8.0f));
	ImGui::BeginChild("##BootLog", ImVec2(cardW - 72.0f, 140.0f), false);
	for (size_t i = 0; i < recent.size(); i++)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(recent[i].error ? kToolDanger : kToolMuted));
		if (g_ToolFontSmall)
			ImGui::PushFont(g_ToolFontSmall);
		ImGui::TextWrapped("%s  %s", recent[i].time, recent[i].text.c_str());
		if (g_ToolFontSmall)
			ImGui::PopFont();
		ImGui::PopStyleColor();
	}
	ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();

	ImGui::EndGroup();
	ImGui::End();
	PopToolStyle();
}

static void DrawPanel()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Always);
	ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);

	PushToolStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::Begin("##ServerPanel", nullptr, flags);

	ImDrawList* draw = ImGui::GetWindowDrawList();
	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	draw->AddRectFilled(p0, ImVec2(p0.x + size.x, p0.y + kToolHeaderH), kToolSurface);
	draw->AddLine(ImVec2(p0.x, p0.y + kToolHeaderH), ImVec2(p0.x + size.x, p0.y + kToolHeaderH), kToolBorder, 1.0f);
	draw->AddRectFilled(ImVec2(p0.x, p0.y + kToolHeaderH), ImVec2(p0.x + kToolSidebarW, p0.y + size.y), kToolSidebar);
	draw->AddLine(ImVec2(p0.x + kToolSidebarW, p0.y + kToolHeaderH), ImVec2(p0.x + kToolSidebarW, p0.y + size.y), kToolBorder, 1.0f);

	ImGui::SetCursorPos(ImVec2(kToolPad, 8.0f));
	if (g_ToolFontBig)
		ImGui::PushFont(g_ToolFontBig);
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolAccent));
	ImGui::TextUnformatted("Servidor");
	ImGui::PopStyleColor();
	if (g_ToolFontBig)
		ImGui::PopFont();
	ImGui::SetCursorPos(ImVec2(kToolPad, 32.0f));
	ImGui::TextDisabled("ferramenta de opera" U8_C U8_AN "o");

	const DWORD elapsed = (GetTickCount() - g_startTick) / 1000;
	char onlineBuf[24], uptimeBuf[40];
	sprintf_s(onlineBuf, "%d online", (int)g_players.size());
	sprintf_s(uptimeBuf, "%02d:%02d:%02d  \xC2\xB7  X minimiza", (int)(elapsed / 3600), (int)((elapsed / 60) % 60), (int)(elapsed % 60));

	if (g_ToolFontSmall)
		ImGui::PushFont(g_ToolFontSmall);
	const ImVec2 uptimeSize = ImGui::CalcTextSize(uptimeBuf);
	const ImVec2 onlineSize = ImGui::CalcTextSize(onlineBuf);
	if (g_ToolFontSmall)
		ImGui::PopFont();

	const char* pillText = bMaintenanceMode ? "Manuten" U8_C U8_AN "o" : "Online";
	const float pillW = ImGui::CalcTextSize(pillText).x + 36.0f;
	const float metaW = (uptimeSize.x > onlineSize.x ? uptimeSize.x : onlineSize.x);

	const float pillX = size.x - kToolPad - pillW;
	const float metaX = pillX - 12.0f - metaW;
	ImGui::SetCursorPos(ImVec2(metaX > kToolSidebarW ? metaX : kToolSidebarW, 8.0f));
	if (g_ToolFontSmall)
		ImGui::PushFont(g_ToolFontSmall);
	ImGui::BeginGroup();
	ImGui::TextDisabled("%s", onlineBuf);
	ImGui::TextDisabled("%s", uptimeBuf);
	ImGui::EndGroup();
	if (g_ToolFontSmall)
		ImGui::PopFont();

	ImGui::SetCursorPos(ImVec2(pillX, 16.0f));
	if (bMaintenanceMode)
		ToolStatusPill(pillText, kToolMaint);
	else
		ToolStatusPill(pillText, kToolOnline);

	ImGui::SetCursorPos(ImVec2(10.0f, kToolHeaderH + 8.0f));
	ImGui::BeginGroup();
	const float navW = kToolSidebarW - 20.0f;
	ToolNavGroup("Monitorar");
	if (ToolNavItem("Status", g_nav == Nav_Status, navW)) g_nav = Nav_Status;
	if (ToolNavItem("Jogadores", g_nav == Nav_Players, navW)) g_nav = Nav_Players;
	if (ToolNavItem("Log", g_nav == Nav_Log, navW)) g_nav = Nav_Log;
	ToolNavGroup("Operar");
	if (ToolNavItem("Eventos", g_nav == Nav_Events, navW)) g_nav = Nav_Events;
	if (ToolNavItem("Arquivos", g_nav == Nav_Files, navW))
	{
		g_nav = Nav_Files;
		ServerConfig_ReloadFromDisk();
	}
	if (ToolNavItem("A" U8_C U8_ON "es", g_nav == Nav_Actions, navW)) g_nav = Nav_Actions;
	ImGui::EndGroup();

	ImGui::SetCursorPos(ImVec2(kToolSidebarW + kToolPad, kToolHeaderH + 12.0f));
	const float bodyW = size.x - kToolSidebarW - kToolPad * 2.0f;
	const float bodyH = size.y - kToolHeaderH - kToolFooterH - 8.0f;
	ImGui::BeginChild("##Body", ImVec2(bodyW > 80.0f ? bodyW : 80.0f, bodyH > 80.0f ? bodyH : 80.0f), false);
	if (g_nav == Nav_Status) DrawStatusPage();
	else if (g_nav == Nav_Players) DrawPlayersPage();
	else if (g_nav == Nav_Events) DrawEventsPage();
	else if (g_nav == Nav_Files) ServerConfig_Draw(SetStatus);
	else if (g_nav == Nav_Log) DrawLogPage();
	else DrawActionsPage();
	ImGui::EndChild();

	ImGui::SetCursorPos(ImVec2(kToolSidebarW + kToolPad, size.y - kToolFooterH + 4.0f));
	if (g_statusMsg[0] && GetTickCount() < g_statusUntil)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolAccent));
		ImGui::TextUnformatted(g_statusMsg);
		ImGui::PopStyleColor();
	}
	else
	{
		ImGui::TextDisabled("O X minimiza. Sair ou desligar s" U8_O " em A" U8_C U8_ON "es.");
	}

	ImGui::End();
	DrawConfirmPopup();
	PopToolStyle();
}

bool ServerPanel_Init(HWND hwnd)
{
	g_hwnd = hwnd;
	g_startTick = GetTickCount();
	g_editExp = eventoxp;
	g_editDrop = eventodrop;
	g_booting = true;
	strncpy_s(g_bootStatus, "A ligar os bancos de dados...", _TRUNCATE);

	if (!CreateDeviceD3D(hwnd))
	{
		std::cout << "Painel grafico indisponivel (DirectX). O servidor segue no console." << std::endl;
		CleanupDeviceD3D();
		ShowWindow(hwnd, SW_HIDE);
		g_active = false;
		g_booting = false;
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	LoadToolFonts(io);

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	g_active = true;
	std::cout << "Painel grafico do servidor iniciado." << std::endl;
	return true;
}

void ServerPanel_StartLogCapture()
{
	if (g_oldCout)
		return;
	if (!g_logCsInit)
	{
		InitializeCriticalSection(&g_logCs);
		g_logCsInit = true;
	}
	g_oldCout = std::cout.rdbuf(&g_logBuf);
	g_oldCerr = std::cerr.rdbuf(&g_logBuf);
}

void ServerPanel_Shutdown()
{
	if (g_oldCout)
	{
		std::cout.rdbuf(g_oldCout);
		g_oldCout = nullptr;
	}
	if (g_oldCerr)
	{
		std::cerr.rdbuf(g_oldCerr);
		g_oldCerr = nullptr;
	}

	if (!g_active && !g_pd3dDevice)
	{
		if (g_logCsInit)
		{
			DeleteCriticalSection(&g_logCs);
			g_logCsInit = false;
		}
		return;
	}

	if (g_active)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
		g_ToolFont = nullptr;
		g_ToolFontBig = nullptr;
		g_ToolFontSmall = nullptr;
	}

	CleanupDeviceD3D();
	g_active = false;
	g_booting = false;

	if (g_logCsInit)
	{
		DeleteCriticalSection(&g_logCs);
		g_logCsInit = false;
	}
}

bool ServerPanel_IsActive()
{
	return g_active;
}

bool ServerPanel_IsBooting()
{
	return g_booting;
}

void ServerPanel_SetBootStatus(const char* utf8)
{
	if (!utf8)
		return;
	strncpy_s(g_bootStatus, utf8, _TRUNCATE);
}

void ServerPanel_SetReady()
{
	g_booting = false;
	g_startTick = GetTickCount();
	g_bootStatus[0] = 0;
}

void ServerPanel_PumpBoot()
{
	if (!g_active || !g_hwnd || g_pumping)
		return;

	g_pumping = true;
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ServerPanel_Render();
	g_pumping = false;
}

void ServerPanel_HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!g_active)
		return;
	ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

void ServerPanel_Resize(int width, int height)
{
	if (!g_active || !g_pd3dDevice || width < 1 || height < 1)
		return;

	g_d3dpp.BackBufferWidth = (UINT)width;
	g_d3dpp.BackBufferHeight = (UINT)height;
	ImGui_ImplDX9_InvalidateDeviceObjects();
	if (SUCCEEDED(g_pd3dDevice->Reset(&g_d3dpp)))
		ImGui_ImplDX9_CreateDeviceObjects();
}

void ServerPanel_Render()
{
	if (!g_active || !g_pd3dDevice || !g_hwnd || g_rendering)
		return;
	if (IsIconic(g_hwnd))
		return;
	g_rendering = true;

	if (!g_booting)
		SnapshotPlayers();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (g_booting)
		DrawSplash();
	else
		DrawPanel();
	ImGui::EndFrame();

	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	const D3DCOLOR clear = D3DCOLOR_RGBA(244, 241, 236, 255);
	g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear, 1.0f, 0);
	if (g_pd3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		g_pd3dDevice->EndScene();
	}

	const HRESULT hr = g_pd3dDevice->Present(nullptr, nullptr, nullptr, nullptr);
	if (hr == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		if (SUCCEEDED(g_pd3dDevice->Reset(&g_d3dpp)))
			ImGui_ImplDX9_CreateDeviceObjects();
	}
	g_rendering = false;
}
