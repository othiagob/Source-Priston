#include "HUD/ServerPanel.h"

#include <d3d9.h>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <streambuf>

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
#define U8_AN "\xC3\xA3"
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

static HMODULE g_hD3D9 = nullptr;
static LPDIRECT3D9 g_pD3D = nullptr;
static LPDIRECT3DDEVICE9 g_pd3dDevice = nullptr;
static D3DPRESENT_PARAMETERS g_d3dpp = {};
static HWND g_hwnd = nullptr;
static bool g_active = false;
static DWORD g_startTick = 0;
static int g_nav = Nav_Status;

static int g_editExp = 1;
static int g_editDrop = 0;
static char g_notice[200] = {};
static char g_statusMsg[256] = {};
static DWORD g_statusUntil = 0;

enum ConfirmKind
{
	Confirm_None = 0,
	Confirm_Shutdown,
	Confirm_Exit
};
static ConfirmKind g_confirm = Confirm_None;

struct PlayerRow
{
	char name[32];
	char id[32];
	char ip[16];
	int area;
	int level;
};
static std::vector<PlayerRow> g_players;
static std::vector<std::string> g_logLines;
static const size_t kLogMax = 400;

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
				if (g_logLines.size() >= kLogMax)
					g_logLines.erase(g_logLines.begin());
				g_logLines.push_back(m_line);
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

static void SetStatus(const char* msg)
{
	if (!msg)
		return;
	strncpy_s(g_statusMsg, msg, _TRUNCATE);
	g_statusUntil = GetTickCount() + 5000;
	if (g_logLines.size() >= kLogMax)
		g_logLines.erase(g_logLines.begin());
	g_logLines.push_back(msg);
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
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
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

static void SnapshotPlayers()
{
	g_players.clear();
	if (!rsPlayInfo)
		return;

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
		g_players.push_back(row);
	}
	LeaveCriticalSection(&cSerSection);
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
	const char* values3[] = { bMaintenanceMode ? "Sim" : "Nao", levelBuf };
	DrawMetricGrid(labels3, values3, 2);
}

static void DrawPlayersPage()
{
	ToolSection("Conectados");
	if (g_players.empty())
	{
		ToolHint("Nenhum jogador conectado.");
		return;
	}

	const ImGuiTableFlags flags = ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp;
	if (ImGui::BeginTable("##Players", 6, flags, ImVec2(0.0f, 0.0f)))
	{
		ImGui::TableSetupColumn("Nick", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Conta", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("IP", ImGuiTableColumnFlags_WidthFixed, 118.0f);
		ImGui::TableSetupColumn("Mapa", ImGuiTableColumnFlags_WidthFixed, 52.0f);
		ImGui::TableSetupColumn("Nv.", ImGuiTableColumnFlags_WidthFixed, 42.0f);
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 72.0f);
		ImGui::TableHeadersRow();

		for (size_t i = 0; i < g_players.size(); i++)
		{
			const PlayerRow& row = g_players[i];
			ImGui::PushID((int)i);
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(ToUtf8(row.name).c_str());
			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(ToUtf8(row.id).c_str());
			ImGui::TableSetColumnIndex(2);
			ImGui::TextUnformatted(row.ip);
			ImGui::TableSetColumnIndex(3);
			ImGui::Text("%d", row.area);
			ImGui::TableSetColumnIndex(4);
			ImGui::Text("%d", row.level);
			ImGui::TableSetColumnIndex(5);
			if (ImGui::SmallButton("Kick"))
				KickPlayer(row.name);
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}

static void DrawEventsPage()
{
	ToolSection("Taxas ao vivo");
	ImGui::SetNextItemWidth(160.0f);
	ImGui::InputInt("EXP (x)", &g_editExp);
	if (g_editExp < 0) g_editExp = 0;
	ImGui::SetNextItemWidth(160.0f);
	ImGui::InputInt("DROP (x)", &g_editDrop);
	if (g_editDrop < 0) g_editDrop = 0;
	if (ToolPrimaryButton("Aplicar taxas", ImVec2(180.0f, 30.0f)))
	{
		eventoxp = g_editExp;
		eventodrop = g_editDrop;
		SetStatus("Taxas de evento aplicadas.");
	}

	ToolSection("Recarregar");
	if (ImGui::Button("Configuracao (INI)", ImVec2(200.0f, 30.0f)))
	{
		rsRefreshConfig();
		g_editExp = eventoxp;
		g_editDrop = eventodrop;
		ServerConfig_ReloadFromDisk();
		SetStatus("Configuracao recarregada.");
	}
	ImGui::SameLine();
	if (ImGui::Button("Lista de GMs", ImVec2(160.0f, 30.0f)))
	{
		GameMasters::getInstance()->readFromDatabase();
		SetStatus("Lista de GMs recarregada.");
	}
	ImGui::SameLine();
	if (ImGui::Button("Desafios", ImVec2(140.0f, 30.0f)))
	{
		Quest::GetInstance()->updateQuests = true;
		Quest::GetInstance()->SendAllQuests(nullptr);
		SetStatus("Desafios recarregados. Jogadores precisam relogar.");
	}
}

static void DrawLogPage()
{
	ToolSection("Boot / eventos");
	ImGui::BeginChild("##LogLines", ImVec2(0.0f, 0.0f), true);
	for (size_t i = 0; i < g_logLines.size(); i++)
		ImGui::TextUnformatted(ToUtf8(g_logLines[i].c_str()).c_str());
	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 24.0f)
		ImGui::SetScrollHereY(1.0f);
	ImGui::EndChild();
}

static void DrawActionsPage()
{
	ToolSection("Aviso global");
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::InputText("##Notice", g_notice, IM_ARRAYSIZE(g_notice));
	if (ToolPrimaryButton("Enviar aviso", ImVec2(180.0f, 28.0f)))
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
	if (ToolDangerButton("Desligar em 8 minutos", ImVec2(220.0f, 30.0f)))
		g_confirm = Confirm_Shutdown;
	ImGui::SameLine();
	if (ToolDangerButton("Sair agora", ImVec2(140.0f, 30.0f)))
		g_confirm = Confirm_Exit;
	ImGui::Spacing();
	ToolHint("O X da janela minimiza. O mundo continua rodando. Sair de verdade so por aqui.");
}

static void DrawConfirmPopup()
{
	if (g_confirm == Confirm_None)
		return;

	ImGui::OpenPopup("##ToolConfirm");
	ImGui::SetNextWindowSize(ImVec2(380.0f, 168.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##ToolConfirm", nullptr, flags))
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolText));
		ImGui::TextUnformatted(g_confirm == Confirm_Shutdown ? "Desligar o mundo?" : "Sair agora?");
		ImGui::PopStyleColor();
		ImGui::Spacing();
		ToolHint(g_confirm == Confirm_Shutdown
			? "Fecha em 8 minutos (igual shutdown;)."
			: "O servidor encerra na hora.");
		ImGui::Dummy(ImVec2(0.0f, 16.0f));
		if (ImGui::Button("Voltar", ImVec2(128.0f, 30.0f)))
		{
			g_confirm = Confirm_None;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ToolDangerButton("Confirmar", ImVec2(128.0f, 30.0f)))
		{
			if (g_confirm == Confirm_Shutdown)
			{
				rsShutDown();
				SetStatus("Shutdown iniciado (8 minutos).");
			}
			else
				PostQuitMessage(0);
			g_confirm = Confirm_None;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
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

	ImGui::SetCursorPos(ImVec2(kToolPad, (kToolHeaderH - ImGui::GetTextLineHeight()) * 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolAccent));
	ImGui::TextUnformatted("Servidor");
	ImGui::PopStyleColor();
	ImGui::SameLine();
	ImGui::TextDisabled("ferramenta de operacao");

	ImGui::SetCursorPos(ImVec2(size.x - 150.0f, (kToolHeaderH - 24.0f) * 0.5f));
	if (bMaintenanceMode)
		ToolStatusPill("Manutencao", kToolMaint);
	else
		ToolStatusPill("Online", kToolOnline);

	ImGui::SetCursorPos(ImVec2(10.0f, kToolHeaderH + 12.0f));
	ImGui::BeginGroup();
	const float navW = kToolSidebarW - 20.0f;
	if (ToolNavItem("Status", g_nav == Nav_Status, navW)) g_nav = Nav_Status;
	if (ToolNavItem("Jogadores", g_nav == Nav_Players, navW)) g_nav = Nav_Players;
	if (ToolNavItem("Eventos", g_nav == Nav_Events, navW)) g_nav = Nav_Events;
	if (ToolNavItem("Arquivos", g_nav == Nav_Files, navW))
	{
		g_nav = Nav_Files;
		ServerConfig_ReloadFromDisk();
	}
	if (ToolNavItem("Log", g_nav == Nav_Log, navW)) g_nav = Nav_Log;
	if (ToolNavItem("Acoes", g_nav == Nav_Actions, navW)) g_nav = Nav_Actions;
	ImGui::EndGroup();

	ImGui::SetCursorPos(ImVec2(kToolSidebarW + kToolPad, kToolHeaderH + 12.0f));
	const float bodyW = size.x - kToolSidebarW - kToolPad * 2.0f;
	const float bodyH = size.y - kToolHeaderH - 40.0f;
	ImGui::BeginChild("##Body", ImVec2(bodyW > 80.0f ? bodyW : 80.0f, bodyH > 80.0f ? bodyH : 80.0f), false);
	if (g_nav == Nav_Status) DrawStatusPage();
	else if (g_nav == Nav_Players) DrawPlayersPage();
	else if (g_nav == Nav_Events) DrawEventsPage();
	else if (g_nav == Nav_Files) ServerConfig_Draw(SetStatus);
	else if (g_nav == Nav_Log) DrawLogPage();
	else DrawActionsPage();
	ImGui::EndChild();

	if (g_statusMsg[0] && GetTickCount() < g_statusUntil)
	{
		ImGui::SetCursorPos(ImVec2(kToolSidebarW + kToolPad, size.y - 26.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolAccent));
		ImGui::TextUnformatted(g_statusMsg);
		ImGui::PopStyleColor();
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

	if (!CreateDeviceD3D(hwnd))
	{
		std::cout << "Painel grafico indisponivel (DirectX). O servidor segue no console." << std::endl;
		CleanupDeviceD3D();
		ShowWindow(hwnd, SW_HIDE);
		g_active = false;
		return false;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

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
	g_oldCout = std::cout.rdbuf(&g_logBuf);
	g_oldCerr = std::cerr.rdbuf(&g_logBuf);
}

void ServerPanel_Shutdown()
{
	if (!g_active && !g_pd3dDevice)
		return;

	if (g_active)
	{
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	CleanupDeviceD3D();
	g_active = false;
}

bool ServerPanel_IsActive()
{
	return g_active;
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
	if (!g_active || !g_pd3dDevice || !g_hwnd)
		return;
	if (IsIconic(g_hwnd))
		return;

	SnapshotPlayers();

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
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
}
