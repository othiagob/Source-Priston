#include "RankingWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "ImGuiWindowChrome.h"
#include "imGui/imgui.h"

#include <algorithm>
#include <d3dx9tex.h>

#define U8_I "\xC3\xAD"
#define U8_AN "\xC3\xA3"
#define U8_I_UP "\xC3\x8D"
#define U8_AN_UP "\xC3\x83"
#define U8_C_UP "\xC3\x87"

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\ranking\\ranking.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const float kWindowW = 720.0f;
static const float kWindowH = 520.0f;

extern int smScreenWidth;
extern int smScreenHeight;

static const char* kClassNames[] = {
	"Todas as classes",
	"Lutador",
	"Mec" U8_AN "nico",
	"Arqueira",
	"Pikeman",
	"Atalanta",
	"Cavaleiro",
	"Mago",
	"Sacerdotisa"
};
static const int kClassFilterCount = 9;

static ImVec2 FitImageSize(int srcW, int srcH, float maxW, float maxH)
{
	if (srcW <= 0 || srcH <= 0 || maxW <= 0.0f || maxH <= 0.0f)
		return ImVec2(0.0f, 0.0f);
	const float scale = ((float)srcW / maxW > (float)srcH / maxH)
		? (maxW / (float)srcW)
		: (maxH / (float)srcH);
	return ImVec2((float)srcW * scale, (float)srcH * scale);
}

static bool LoadPngTexture(const char* path, void** outTex, int* outW, int* outH)
{
	if (!GRAPHICDEVICE || !path || !outTex)
		return false;

	LPDIRECT3DTEXTURE9 tex = nullptr;
	D3DXIMAGE_INFO info = {};
	const HRESULT hr = D3DXCreateTextureFromFileExA(
		GRAPHICDEVICE, path,
		D3DX_DEFAULT, D3DX_DEFAULT,
		1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
		D3DX_FILTER_LINEAR, D3DX_FILTER_NONE,
		0, &info, nullptr, &tex);

	if (FAILED(hr) || !tex)
		return false;

	*outTex = tex;
	if (outW) *outW = (int)info.Width;
	if (outH) *outH = (int)info.Height;
	return true;
}

static void DrawActiveTabOrnament()
{
	const ImVec2 a = ImGui::GetItemRectMin();
	const ImVec2 b = ImGui::GetItemRectMax();
	const float width = b.x - a.x;
	if (width <= 2.0f)
		return;

	const float cap = width * 0.5f - 1.0f;
	float rounding = 3.0f;
	if (rounding > cap)
		rounding = cap;
	if (rounding < 0.0f)
		rounding = 0.0f;
	const float y1 = a.y + 1.0f;
	const float y2 = b.y - 1.0f;
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->PathLineTo(ImVec2(a.x + 0.5f, y2));
	draw->PathArcToFast(ImVec2(a.x + rounding + 0.5f, y1 + rounding + 0.5f), rounding, 6, 9);
	draw->PathArcToFast(ImVec2(b.x - rounding - 0.5f, y1 + rounding + 0.5f), rounding, 9, 12);
	draw->PathLineTo(ImVec2(b.x - 0.5f, y2));
	draw->PathStroke(kGold, false, 1.5f);
}

static bool DrawHeaderClose()
{
	const ImVec2 win = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(win.x - kHeaderBtnW - 12.0f, (kMainHeaderH - kHeaderBtnH) * 0.5f));
	ImGui::InvisibleButton("##RankingClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
	const bool hovered = ImGui::IsItemHovered();
	const bool clicked = ImGui::IsItemClicked();
	const ImVec2 p = ImGui::GetItemRectMin();
	const ImVec2 b1(p.x + kHeaderBtnW, p.y + kHeaderBtnH);
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(p, b1, hovered ? IM_COL32(56, 46, 22, 255) : IM_COL32(18, 20, 24, 255), kHeaderBtnRound);
	draw->AddRect(p, b1, hovered ? kGoldBright : kGold, kHeaderBtnRound, 0, 1.2f);
	const ImVec2 c((p.x + b1.x) * 0.5f, (p.y + b1.y) * 0.5f);
	const float arm = 4.8f;
	const ImU32 xCol = IM_COL32(236, 220, 160, 255);
	draw->AddLine(ImVec2(c.x - arm, c.y - arm), ImVec2(c.x + arm, c.y + arm), xCol, 1.7f);
	draw->AddLine(ImVec2(c.x + arm, c.y - arm), ImVec2(c.x - arm, c.y + arm), xCol, 1.7f);
	return clicked;
}

static bool sortByLevel(const TopPlayers& a, const TopPlayers& b)
{
	return a.playerLevel > b.playerLevel;
}

static bool sortByKills(const TopPlayers& a, const TopPlayers& b)
{
	return a.playerBps > b.playerBps;
}

INT RankingWindow::OpenNPCRanking()
{
	smTRANS_COMMAND openNpc;
	openNpc.code = OPEN_RANKING_NPC;
	openNpc.size = sizeof(smTRANS_COMMAND);
	openNpc.WParam = 0;
	openNpc.LParam = 0;
	openNpc.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&openNpc, openNpc.size, TRUE);

	return TRUE;
}

INT RankingWindow::ReceivePlayers(TopPlayerPckg* Pckg)
{
	if (!Pckg)
		return FALSE;

	m_players.clear();
	m_playersPvp.clear();

	for (int x = 0; x < 180; x++)
	{
		if (Pckg->Players[x].playerClass > 0 && Pckg->Players[x].playerName[2] != '-')
			m_players.push_back(Pckg->Players[x]);
	}

	m_playersPvp = m_players;
	std::sort(m_players.begin(), m_players.end(), sortByLevel);
	std::sort(m_playersPvp.begin(), m_playersPvp.end(), sortByKills);
	return TRUE;
}

bool RankingWindow::IsBlockingMouse(int x, int y) const
{
	if (!openFlag)
		return false;
	return m_winW > 0.0f && m_winH > 0.0f
		&& x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH);
}

std::string RankingWindow::ToUtf8(const char* src) const
{
	if (!src || !src[0])
		return {};

	const int wideLen = MultiByteToWideChar(CP_ACP, 0, src, -1, nullptr, 0);
	if (wideLen <= 0)
		return src;

	std::wstring wide((size_t)wideLen, L'\0');
	MultiByteToWideChar(CP_ACP, 0, src, -1, &wide[0], wideLen);

	const int utfLen = WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (utfLen <= 0)
		return src;

	std::string utf((size_t)utfLen, '\0');
	WideCharToMultiByte(CP_UTF8, 0, wide.c_str(), -1, &utf[0], utfLen, nullptr, nullptr);
	if (!utf.empty() && utf.back() == '\0')
		utf.pop_back();
	return utf;
}

void RankingWindow::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;
	LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH);
}

void RankingWindow::EnsureClassTextures()
{
	if (m_classTried)
		return;
	m_classTried = true;

	for (int i = 0; i < 12; i++)
	{
		char path[64] = { 0 };
		sprintf_s(path, sizeof(path), "game\\images\\ranking\\%d.png", i);
		LoadPngTexture(path, &m_classTex[i], &m_classW[i], &m_classH[i]);
	}
}

void RankingWindow::PushWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 12.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 7.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 12.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_TabRounding, 3.0f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.06f, 0.07f, 0.09f, 0.92f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.50f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImVec4(0.62f, 0.62f, 0.68f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.12f, 0.13f, 0.16f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.32f, 0.26f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.45f, 0.36f, 0.14f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.22f, 0.18f, 0.10f, 0.85f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.30f, 0.24f, 0.12f, 0.95f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.36f, 0.28f, 0.14f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Tab, ImVec4(0.10f, 0.11f, 0.14f, 0.90f));
	ImGui::PushStyleColor(ImGuiCol_TabHovered, ImVec4(0.30f, 0.24f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TabActive, ImVec4(0.24f, 0.20f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.78f, 0.67f, 0.35f, 0.35f));
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.78f, 0.62f, 0.28f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.05f, 0.06f, 0.08f, 0.95f));
	ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.08f, 0.09f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.08f, 0.09f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.55f, 0.46f, 0.24f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ImVec4(0.78f, 0.67f, 0.35f, 0.90f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ImVec4(0.90f, 0.78f, 0.40f, 1.0f));
}

void RankingWindow::PopWindowStyle()
{
	ImGui::PopStyleColor(22);
	ImGui::PopStyleVar(10);
}

void RankingWindow::DrawWindowChrome(float headerH)
{
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void RankingWindow::DrawTitleHeader(bool* p_open)
{
	EnsureTitleTexture();

	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	ImDrawList* draw = ImGui::GetWindowDrawList();

	if (m_titleTex && m_titleW > 0 && m_titleH > 0)
	{
		const float headerTop = p0.y + 6.0f;
		const float headerBot = p0.y + kMainHeaderH - 4.0f;
		const float areaH = headerBot - headerTop;
		const ImVec2 sz = FitImageSize(m_titleW, m_titleH, 300.0f, areaH);
		const float x = p0.x + (size.x - sz.x) * 0.5f;
		const float y = headerTop + (areaH - sz.y) * 0.5f;
		draw->AddImage((ImTextureID)m_titleTex, ImVec2(x, y), ImVec2(x + sz.x, y + sz.y));
	}
	else
	{
		const char* title = "RANKING";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + 18.0f), kGoldBright, title);
	}

	if (p_open && DrawHeaderClose())
		*p_open = false;
}

void RankingWindow::DrawSectionHeader(const char* title)
{
	ImGui::Spacing();
	const ImVec2 ts = ImGui::CalcTextSize(title);
	const float avail = ImGui::GetContentRegionAvail().x;
	const float padX = 16.0f;
	const float padY = 4.0f;
	const float boxW = ts.x + padX * 2.0f;
	const float boxH = ts.y + padY * 2.0f;
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	const float boxX = origin.x + (avail - boxW) * 0.5f;
	const float midY = origin.y + boxH * 0.5f;
	ImDrawList* draw = ImGui::GetWindowDrawList();

	draw->AddLine(ImVec2(origin.x + 8.0f, midY), ImVec2(origin.x + avail - 8.0f, midY), kGoldDim, 1.2f);
	draw->AddRectFilled(ImVec2(boxX, origin.y), ImVec2(boxX + boxW, origin.y + boxH), IM_COL32(18, 20, 26, 255), 3.0f);
	draw->AddRect(ImVec2(boxX, origin.y), ImVec2(boxX + boxW, origin.y + boxH), kGold, 3.0f, 0, 1.4f);
	draw->AddText(ImVec2(boxX + padX, origin.y + padY), kGoldBright, title);
	ImGui::Dummy(ImVec2(avail, boxH + 6.0f));
}

void RankingWindow::DrawClassFilter()
{
	EnsureClassTextures();

	const float icon = 28.0f;
	const float pad = 4.0f;
	const float box = icon + pad * 2.0f;
	const float gap = 6.0f;
	const float totalW = (float)kClassFilterCount * box + (float)(kClassFilterCount - 1) * gap;
	const float avail = ImGui::GetContentRegionAvail().x;
	float startX = (avail - totalW) * 0.5f;
	if (startX < 0.0f)
		startX = 0.0f;

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + startX);

	for (int i = 0; i < kClassFilterCount; i++)
	{
		if (i > 0)
			ImGui::SameLine(0.0f, gap);

		ImGui::PushID(i);
		ImGui::InvisibleButton("##cls", ImVec2(box, box));
		const bool hovered = ImGui::IsItemHovered();
		const bool selected = (m_selectedClass == i);
		if (ImGui::IsItemClicked())
			m_selectedClass = i;

		const ImVec2 p = ImGui::GetItemRectMin();
		const ImVec2 b1(p.x + box, p.y + box);
		ImDrawList* draw = ImGui::GetWindowDrawList();
		draw->AddRectFilled(p, b1, selected ? IM_COL32(56, 46, 22, 255) : IM_COL32(18, 20, 24, 255), 3.0f);
		draw->AddRect(p, b1, (selected || hovered) ? kGoldBright : kGold, 3.0f, 0, selected ? 1.5f : 1.2f);

		if (m_classTex[i])
		{
			draw->AddImage((ImTextureID)m_classTex[i],
				ImVec2(p.x + pad, p.y + pad),
				ImVec2(p.x + pad + icon, p.y + pad + icon));
		}
		else
		{
			char n[4] = { 0 };
			sprintf_s(n, sizeof(n), "%d", i);
			const ImVec2 ts = ImGui::CalcTextSize(n);
			draw->AddText(ImVec2(p.x + (box - ts.x) * 0.5f, p.y + (box - ts.y) * 0.5f), kGoldBright, n);
		}

		if (hovered && i < (int)(sizeof(kClassNames) / sizeof(kClassNames[0])))
			ImGui::SetTooltip("%s", kClassNames[i]);

		ImGui::PopID();
	}
}

void RankingWindow::DrawRankingBody(bool pvp)
{
	DrawClassFilter();
	DrawSectionHeader("TOP 10");

	const std::vector<TopPlayers>& list = pvp ? m_playersPvp : m_players;
	const char* valueLabel = pvp ? "MORTES" : ("N" U8_I_UP "VEL");

	ImGui::BeginChild(pvp ? "##RankingListPvp" : "##RankingListLevel", ImVec2(0.0f, 0.0f), false);
	{
		const float avail = ImGui::GetContentRegionAvail().x;
		const float colPos = 16.0f;
		const float colNick = 150.0f;
		const float colVal = avail - 96.0f;
		const float headerH = 26.0f;
		const float rowH = 34.0f;

		ImDrawList* draw = ImGui::GetWindowDrawList();
		const ImVec2 hp = ImGui::GetCursorScreenPos();
		draw->AddText(ImVec2(hp.x + colPos, hp.y + 4.0f), kGoldBright, "POSI" U8_C_UP U8_AN_UP "O");
		draw->AddText(ImVec2(hp.x + colNick, hp.y + 4.0f), kGoldBright, "NICK");
		draw->AddText(ImVec2(hp.x + colVal, hp.y + 4.0f), kGoldBright, valueLabel);
		draw->AddLine(ImVec2(hp.x + 8.0f, hp.y + headerH - 1.0f), ImVec2(hp.x + avail - 8.0f, hp.y + headerH - 1.0f), kGold, 1.15f);
		ImGui::Dummy(ImVec2(avail, headerH));

		int rank = 1;
		for (const auto& player : list)
		{
			if (m_selectedClass != 0 && player.playerClass != m_selectedClass)
				continue;
			if (rank > 10)
				break;

			const ImVec2 p = ImGui::GetCursorScreenPos();
			if ((rank % 2) == 0)
				draw->AddRectFilled(p, ImVec2(p.x + avail, p.y + rowH), IM_COL32(16, 18, 24, 180), 3.0f);

			const ImU32 posCol = (rank <= 3) ? kGoldBright : IM_COL32(255, 255, 255, 255);
			char pos[8] = { 0 };
			sprintf_s(pos, sizeof(pos), "%d", rank);
			draw->AddText(ImVec2(p.x + colPos, p.y + 9.0f), posCol, pos);

			const int cls = player.playerClass;
			if (cls >= 0 && cls < 12 && m_classTex[cls])
			{
				draw->AddImage((ImTextureID)m_classTex[cls],
					ImVec2(p.x + colPos + 22.0f, p.y + 7.0f),
					ImVec2(p.x + colPos + 42.0f, p.y + 27.0f));
			}

			const std::string nick = ToUtf8(player.playerName);
			draw->AddText(ImVec2(p.x + colNick, p.y + 9.0f), IM_COL32(255, 255, 255, 255),
				nick.empty() ? player.playerName : nick.c_str());

			char val[16] = { 0 };
			sprintf_s(val, sizeof(val), "%d", pvp ? player.playerBps : player.playerLevel);
			draw->AddText(ImVec2(p.x + colVal, p.y + 9.0f), kGoldBright, val);

			draw->AddLine(
				ImVec2(p.x + 8.0f, p.y + rowH - 1.0f),
				ImVec2(p.x + avail - 8.0f, p.y + rowH - 1.0f),
				IM_COL32(200, 170, 90, 40), 1.0f);

			ImGui::Dummy(ImVec2(avail, rowH));
			rank++;
		}

		if (rank == 1)
		{
			ImGui::Dummy(ImVec2(0.0f, 18.0f));
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.62f, 0.68f, 1.0f));
			const char* empty = "Nenhum jogador neste filtro.";
			const ImVec2 ts = ImGui::CalcTextSize(empty);
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
			ImGui::TextUnformatted(empty);
			ImGui::PopStyleColor();
		}
	}
	ImGui::EndChild();
}

void RankingWindow::OpenNpc(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(kWindowW, kWindowH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(((float)smScreenWidth - kWindowW) * 0.5f, ((float)smScreenHeight - kWindowH) * 0.5f));

	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##RankingWindow", p_open, flags);
	{
		const ImVec2 pos = ImGui::GetWindowPos();
		const ImVec2 size = ImGui::GetWindowSize();
		m_winX = pos.x;
		m_winY = pos.y;
		m_winW = size.x;
		m_winH = size.y;

		DrawWindowChrome(kMainHeaderH);
		DrawTitleHeader(p_open);

		SetPlayerWindowBodyCursor(kMainHeaderH);

		if (ImGui::BeginTabBar("##RankingTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("N" U8_I "vel"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 0;
				DrawRankingBody(false);
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("PVP"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 1;
				DrawRankingBody(true);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsWindowHovered())
			ImGui::CaptureMouseFromApp(true);

		ImGui::End();
	}

	PopWindowStyle();
}
