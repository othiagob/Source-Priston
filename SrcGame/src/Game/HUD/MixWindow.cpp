#include "MixWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "ImGuiWindowChrome.h"
#include "imGui/imgui.h"

#include <cctype>
#include <d3dx9tex.h>

#define U8_E "\xC3\xA9"
#define U8_I_UP "\xC3\x8D"

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\mix\\lista-de-mix.png";
static const char* kTitleImagePathAlt = "game\\images\\Mix\\lista-de-mix.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const float kWindowW = 720.0f;
static const float kWindowH = 520.0f;

extern int smScreenWidth;
extern int smScreenHeight;

std::vector<MixInfo> allMix;

static const char* SheltonsName[] =
{
	"null",
	"Lucidy",
	"Sereno",
	"Fadeo",
	"Sparky",
	"Raident",
	"Transparo",
	"Murki",
	"Devine",
	"Celesto",
	"Mirage",
	"Inferna",
	"Enigma",
	"Bellum",
	"Ordo"
};

struct MixTab
{
	MixType type;
	const char* label;
};

static const MixTab kTabs[] =
{
	{ MixType::Weapons, "Armas" },
	{ MixType::Armors, "Armaduras" },
	{ MixType::Shields, "Escudos" },
	{ MixType::Orbs, "Orbs" },
	{ MixType::Boots, "Botas" },
	{ MixType::Gloves, "Luvas" },
	{ MixType::Bracelets, "Braceletes" },
	{ MixType::Amulets, "Amuletos" },
	{ MixType::Rings, "An" U8_E "is" },
};
static const int kTabCount = (int)(sizeof(kTabs) / sizeof(kTabs[0]));

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
	ImGui::InvisibleButton("##MixClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
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

INT MixWindow::ReadMix()
{
	allMix.clear();

	MixInfo Mix;

	// ARMAS

	Mix.mixId = 1;
	Mix.mixName = "Frost";
	Mix.description = "ATK MIN +7 - MAX +8; TAXA DE ATK +35; CR" U8_I_UP "T. +2; MANA +10";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 2;
	Mix.mixName = "Evento";
	Mix.description = "ATK MIN +8 - MAX +9; TAXA DE ATK +40; CR" U8_I_UP "T. +3; MANA +15";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);


	// ARMADURAS E ROUPES

	Mix.mixId = 3;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +80; ABS. +2.0; HP +10";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 4;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +120; ABS. +3.0; HP +15";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// ESCUDOS

	Mix.mixId = 5;
	Mix.mixName = "Frost";
	Mix.description = u8"BLOQUEIO +2%; ABS. +1.4; HP +10";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 6;
	Mix.mixName = "Evento";
	Mix.description = u8"BLOQUEIO +3%; ABS. +2.0; HP +20";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);


	// ORBITAIS

	Mix.mixId = 7;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +20; ABS. +1.4; HP +15, RES +45";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 8;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +55; ABS. +2.0; HP +20, RES +60";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Raident, 1);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);

	allMix.push_back(Mix);

	// BOTAS

	Mix.mixId = 9;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +10; ABS. +1.2; VELOCIDADE +0.9; HP +5; MANA +10; RES +15";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 10;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +15; ABS. +1.8; VELOCIDADE +1.8; HP +10; MANA +15; RES +20";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);


	// LUVAS

	Mix.mixId = 11;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +10; ABS. +1.0; HP +5; MANA +10; RES +15;";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 12;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +30; ABS. +2.0; HP +10; MANA +15; RES +20";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// BRACELETES

	Mix.mixId = 13;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +15; TAXA DE ATK +30; HP +5; MANA +10; RES +15";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 14;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +25; TAXA DE ATK +40; HP +10; MANA +15; RES +20";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// AMULETOS

	Mix.mixId = 15;
	Mix.mixName = "Frost";
	Mix.description = u8"REG. de HP +0.4; REG. de MANA +0.8; HP +4; MANA +8; RES +12";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 16;
	Mix.mixName = "Evento";
	Mix.description = u8"REG. de HP +0.6; REG. de MANA +1.0; HP +6; MANA +10; RES +14";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// ANIS

	Mix.mixId = 17;
	Mix.mixName = "Frost";
	Mix.description = u8"REG. de HP +0.4; HP +2; MANA +6";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.mixId = 18;
	Mix.mixName = "Evento";
	Mix.description = u8"REG. de HP +0.8; HP +6; MANA +10";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.mixId].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	openFlag = true;

	return TRUE;
}

bool MixWindow::IsBlockingMouse(int x, int y) const
{
	if (!openFlag)
		return false;
	return m_winW > 0.0f && m_winH > 0.0f
		&& x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH);
}

void MixWindow::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;
	if (!LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH))
		LoadPngTexture(kTitleImagePathAlt, &m_titleTex, &m_titleW, &m_titleH);
}

void MixWindow::EnsureStoneTextures()
{
	if (m_stoneTried)
		return;
	m_stoneTried = true;

	for (int i = 1; i <= 14; i++)
	{
		char path[64] = { 0 };
		sprintf_s(path, sizeof(path), "game\\images\\Mix\\%d.png", i);
		int w = 0;
		int h = 0;
		LoadPngTexture(path, &m_stoneTex[i], &w, &h);
	}
}

void MixWindow::PushWindowStyle()
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

void MixWindow::PopWindowStyle()
{
	ImGui::PopStyleColor(22);
	ImGui::PopStyleVar(10);
}

void MixWindow::DrawWindowChrome(float headerH)
{
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void MixWindow::DrawTitleHeader(bool* p_open)
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
		const ImVec2 sz = FitImageSize(m_titleW, m_titleH, 320.0f, areaH);
		const float x = p0.x + (size.x - sz.x) * 0.5f;
		const float y = headerTop + (areaH - sz.y) * 0.5f;
		draw->AddImage((ImTextureID)m_titleTex, ImVec2(x, y), ImVec2(x + sz.x, y + sz.y));
	}
	else
	{
		const char* title = "LISTA DE MIX";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + 18.0f), kGoldBright, title);
	}

	if (p_open && DrawHeaderClose())
		*p_open = false;
}

void MixWindow::DrawSectionHeader(const char* title)
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

void MixWindow::DrawMixCard(const MixInfo& mix)
{
	EnsureStoneTextures();

	char title[32] = { 0 };
	sprintf_s(title, sizeof(title), "%s", mix.mixName.c_str());
	for (char* p = title; *p; p++)
		*p = (char)toupper((unsigned char)*p);
	DrawSectionHeader(title);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.86f, 0.78f, 1.0f));
	ImGui::TextWrapped("%s", mix.description.c_str());
	ImGui::PopStyleColor();
	ImGui::Dummy(ImVec2(0.0f, 4.0f));

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.43f, 1.0f));
	ImGui::TextUnformatted("PEDRAS");
	ImGui::PopStyleColor();

	if (mix.mixId < 0 || mix.mixId >= 100)
		return;

	const auto& stones = mix.sheltonList[mix.mixId];
	for (size_t i = 0; i < stones.size(); i++)
	{
		const int stone = (int)stones[i].first;
		if (i > 0)
			ImGui::SameLine(0.0f, 16.0f);

		if (stone >= 1 && stone <= 14 && m_stoneTex[stone])
		{
			ImGui::Image((ImTextureID)m_stoneTex[stone], ImVec2(20.0f, 20.0f));
			ImGui::SameLine(0.0f, 6.0f);
		}

		char line[48] = { 0 };
		const char* name = (stone >= 0 && stone <= 14) ? SheltonsName[stone] : "?";
		sprintf_s(line, sizeof(line), "%s x%d", name, stones[i].second);
		ImGui::AlignTextToFramePadding();
		ImGui::TextUnformatted(line);
	}

	ImGui::Dummy(ImVec2(0.0f, 8.0f));
}

void MixWindow::DrawMixTab(MixType type)
{
	char childId[24] = { 0 };
	sprintf_s(childId, sizeof(childId), "##MixList%d", (int)type);
	ImGui::BeginChild(childId, ImVec2(0.0f, 0.0f), false);
	int shown = 0;
	for (const auto& mix : allMix)
	{
		if (mix.mixType != type)
			continue;
		ImGui::PushID(mix.mixId);
		DrawMixCard(mix);
		ImGui::PopID();
		shown++;
	}
	if (shown == 0)
	{
		ImGui::Dummy(ImVec2(0.0f, 18.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.62f, 0.68f, 1.0f));
		const char* empty = "Nenhum mix nesta aba.";
		const ImVec2 ts = ImGui::CalcTextSize(empty);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
		ImGui::TextUnformatted(empty);
		ImGui::PopStyleColor();
	}
	ImGui::EndChild();
}


void MixWindow::OpenNpc(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(kWindowW, kWindowH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(((float)smScreenWidth - kWindowW) * 0.5f, ((float)smScreenHeight - kWindowH) * 0.5f));

	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##ListaDeMix", p_open, flags);
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

		if (ImGui::BeginTabBar("##MixTabs", ImGuiTabBarFlags_FittingPolicyScroll))
		{
			for (int i = 0; i < kTabCount; i++)
			{
				if (ImGui::BeginTabItem(kTabs[i].label))
				{
					DrawActiveTabOrnament();
					m_activeTab = i;
					DrawMixTab(kTabs[i].type);
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsWindowHovered())
			ImGui::CaptureMouseFromApp(true);

		ImGui::End();
	}

	PopWindowStyle();
}
