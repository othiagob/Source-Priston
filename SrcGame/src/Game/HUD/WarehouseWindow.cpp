#include "WarehouseWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\netplay.h"
#include "GlobalsShared.h"
#include "ImGuiWindowChrome.h"
#include "imGui/imgui.h"

#include <d3dx9tex.h>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <string>

#define U8_A "\xC3\xA1"
#define U8_E "\xC3\xA9"
#define U8_U "\xC3\xBA"
#define U8_A_UP "\xC3\x81"

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\warehouse\\armazem.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const int kLogicCell = ITEMSIZE;
static const float kCell = (float)ITEMSIZE;
static const int kGridN = 9;
static const float kSideW = 140.0f;
static const float kGridPad = 10.0f;
static const float kWindowW = 16.0f + (kCell * (float)kGridN + kGridPad) + 8.0f + kSideW + 16.0f;
static const float kWindowH = kMainHeaderH + 12.0f + 20.0f + 24.0f + 8.0f + 26.0f + 8.0f
	+ (kCell * (float)kGridN + kGridPad) + 16.0f;
static const int kLogicOriginX = 21;
static const float kGoldModalW = 400.0f;
static const float kGoldModalH = 368.0f;

extern int smScreenWidth;
extern int smScreenHeight;
extern int TradeColorIndex;
extern RECT TradeColorRect;
extern int TradeCrashItemIndex[2];
extern int SelectTradeItemIndex;
extern int WareHouseSaveFlag;

static int LogicOriginY()
{
	return 136 + sinInterHeight2;
}

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

static bool DrawHeaderClose()
{
	const ImVec2 win = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(win.x - kHeaderBtnW - 12.0f, (kMainHeaderH - kHeaderBtnH) * 0.5f));
	ImGui::InvisibleButton("##WarehouseClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
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

static void FormatGold(int value, char* out, int outSize)
{
	if (value < 0)
		value = 0;
	char raw[32] = {};
	sprintf_s(raw, sizeof(raw), "%d", value);
	const int len = (int)strlen(raw);
	int dest = 0;
	int lead = len % 3;
	if (lead == 0)
		lead = 3;
	for (int i = 0; i < len && dest + 1 < outSize; i++)
	{
		if (i == lead)
		{
			out[dest++] = '.';
			lead += 3;
			if (dest + 1 >= outSize)
				break;
		}
		out[dest++] = raw[i];
	}
	out[dest] = 0;
}

static void FormatGoldShort(int value, char* out, int outSize)
{
	if (!out || outSize <= 0)
		return;
	if (value < 0)
		value = 0;

	if (value < 1000)
	{
		sprintf_s(out, outSize, "%d", value);
		return;
	}

	double scaled = (double)value;
	const char* suffix = "k";
	if (value >= 1000000000)
	{
		scaled = (double)value / 1000000000.0;
		suffix = "kkk";
	}
	else if (value >= 1000000)
	{
		scaled = (double)value / 1000000.0;
		suffix = "kk";
	}
	else
		scaled = (double)value / 1000.0;

	char num[32] = {};
	sprintf_s(num, sizeof(num), "%.2f", scaled);
	char* dot = strchr(num, '.');
	if (dot)
	{
		char* end = num + strlen(num) - 1;
		while (end > dot && *end == '0')
			*end-- = 0;
		if (*end == '.')
			*end = 0;
	}
	sprintf_s(out, outSize, "%s%s", num, suffix);
}

static int ParseGoldShorthand(const char* src)
{
	if (!src || !src[0])
		return 0;

	char buf[64] = {};
	int n = 0;
	for (const char* p = src; *p && n < 63; p++)
	{
		if (*p == ' ')
			continue;
		buf[n++] = (char)tolower((unsigned char)*p);
	}

	int kCount = 0;
	while (n > 0 && buf[n - 1] == 'k')
	{
		kCount++;
		buf[--n] = 0;
	}
	if (n <= 0)
		return 0;

	if (kCount > 0)
	{
		for (int i = 0; buf[i]; i++)
		{
			if (buf[i] == ',')
				buf[i] = '.';
		}
		double v = atof(buf);
		if (kCount > 3)
			kCount = 3;
		for (int i = 0; i < kCount; i++)
			v *= 1000.0;
		if (v < 0.0)
			v = 0.0;
		if (v > 2000000000.0)
			v = 2000000000.0;
		return (int)(v + 0.5);
	}

	char digits[64] = {};
	int d = 0;
	for (int i = 0; buf[i] && d < 63; i++)
	{
		if (buf[i] >= '0' && buf[i] <= '9')
			digits[d++] = buf[i];
	}
	if (!d)
		return 0;

	const long long v = _strtoi64(digits, nullptr, 10);
	if (v < 0)
		return 0;
	if (v > 2000000000LL)
		return 2000000000;
	return (int)v;
}

static void SnapHeldItemToCursor(sITEM* item, int cursorLogX, int cursorLogY)
{
	if (!item)
		return;

	const int originX = kLogicOriginX;
	const int originY = LogicOriginY();
	const int cell = kLogicCell;
	int col = (cursorLogX - originX) / cell;
	int row = (cursorLogY - originY) / cell;
	if (col < 0)
		col = 0;
	if (row < 0)
		row = 0;

	int cellsW = item->w / cell;
	int cellsH = item->h / cell;
	if (cellsW < 1)
		cellsW = 1;
	if (cellsH < 1)
		cellsH = 1;
	if (col > kGridN - cellsW)
		col = kGridN - cellsW;
	if (row > kGridN - cellsH)
		row = kGridN - cellsH;
	if (col < 0)
		col = 0;
	if (row < 0)
		row = 0;

	item->x = originX + col * cell;
	item->y = originY + row * cell;
	item->SetX = item->x;
	item->SetY = item->y;
}

static std::string Utf8ToAcp(const char* src)
{
	if (!src || !src[0])
		return {};

	const int wideLen = MultiByteToWideChar(CP_UTF8, 0, src, -1, nullptr, 0);
	if (wideLen <= 0)
		return src;

	std::wstring wide((size_t)wideLen, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, src, -1, &wide[0], wideLen);

	const int acpLen = WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, nullptr, 0, nullptr, nullptr);
	if (acpLen <= 0)
		return src;

	std::string acp((size_t)acpLen, '\0');
	WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, &acp[0], acpLen, nullptr, nullptr);
	if (!acp.empty() && acp.back() == '\0')
		acp.pop_back();
	return acp;
}

static void CenterTextUnformatted(const char* text)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
	ImGui::TextUnformatted(text);
}

static void DrawGoldBalanceCard(const char* label, const char* full, const char* compact, float width, float height, bool active)
{
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	const ImVec2 r1(origin.x + width, origin.y + height);
	const ImU32 fill = active ? IM_COL32(36, 30, 16, 255) : IM_COL32(14, 16, 20, 255);
	const ImU32 border = active ? kGoldBright : kGold;
	draw->AddRectFilled(origin, r1, fill, 3.0f);
	draw->AddRect(origin, r1, border, 3.0f, 0, 1.15f);
	draw->AddText(ImVec2(origin.x + 10.0f, origin.y + 6.0f), IM_COL32(158, 158, 168, 255), label);
	draw->AddText(ImVec2(origin.x + 10.0f, origin.y + 22.0f), kGoldBright, full);
	draw->AddText(ImVec2(origin.x + 10.0f, origin.y + 38.0f), IM_COL32(200, 170, 90, 200), compact);
	ImGui::Dummy(ImVec2(width, height));
}

static int WarehouseGold()
{
	const int gold = sWareHouse.Money - 2023;
	return gold < 0 ? 0 : gold;
}

static int ClampDeposit(int count)
{
	if (count < 0)
		count = 0;
	if (sinChar->Money < count)
		count = sinChar->Money;
	const int stored = WarehouseGold();
	int cap = MAX_GOLD_WHAREHOUSE;
	if (MAX_GOLD_TIER4 < cap)
		cap = MAX_GOLD_TIER4;
	if (stored + count > cap)
		count = cap - stored;
	if (count < 0)
		count = 0;
	return count;
}

static int ClampWithdraw(int count)
{
	if (count < 0)
		count = 0;
	if (WarehouseGold() < count)
		count = WarehouseGold();
	if (sinChar->Level <= 10)
	{
		if (sinChar->Money + count > MAX_GOLD_LOW_LEVEL)
			count = MAX_GOLD_LOW_LEVEL - sinChar->Money;
	}
	else
	{
		switch (sinChar->ChangeJob)
		{
		case 0:
			if ((sinChar->Money + count) > (sinChar->Level * MAX_GOLD_LOW_LEVEL) - 1800000)
				count = (sinChar->Level * MAX_GOLD_LOW_LEVEL) - 1800000 - sinChar->Money;
			break;
		case 1:
			if ((sinChar->Money + count) > MAX_GOLD_TIER2)
				count = MAX_GOLD_TIER2 - sinChar->Money;
			break;
		case 2:
			if ((sinChar->Money + count) > MAX_GOLD_TIER3)
				count = MAX_GOLD_TIER3 - sinChar->Money;
			break;
		default:
			if ((sinChar->Money + count) > MAX_GOLD_TIER4)
				count = MAX_GOLD_TIER4 - sinChar->Money;
			break;
		}
	}
	if (count < 0)
		count = 0;
	return count;
}

void WarehouseWindow::ClearSearch()
{
	m_searchUtf8[0] = 0;
	cWareHouse.szSearch[0] = 0;
	m_searchFocused = false;
}

bool WarehouseWindow::ShouldCaptureKeyboard() const
{
	if (!cWareHouse.OpenFlag)
		return false;
	if (m_goldOpen || m_searchFocused)
		return true;
	return ImGui::GetCurrentContext() && ImGui::GetIO().WantTextInput;
}

bool WarehouseWindow::OnEscape()
{
	if (m_goldOpen)
	{
		m_goldOpen = false;
		return true;
	}
	if (m_searchFocused)
	{
		ClearSearch();
		return true;
	}
	return false;
}

void WarehouseWindow::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;
	LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH);
}

void WarehouseWindow::PushWindowStyle()
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

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.09f, 0.12f, 0.96f));
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

void WarehouseWindow::PopWindowStyle()
{
	ImGui::PopStyleColor(22);
	ImGui::PopStyleVar(10);
}

void WarehouseWindow::DrawWindowChrome(float headerH)
{
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void WarehouseWindow::DrawTitleHeader()
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
		const char* title = "ARMAZEM";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + 18.0f), kGoldBright, title);
	}
}

bool WarehouseWindow::ShouldHideClassicPanels() const
{
	return m_hideClassic || cWareHouse.OpenFlag != 0 || cWareHouse.IsLoadingPages();
}

void WarehouseWindow::ArmHideClassic()
{
	m_hideClassic = true;
}

void WarehouseWindow::RequestClose()
{
	m_requestClose = true;
}

bool WarehouseWindow::ConsumeCloseRequest()
{
	if (!m_requestClose)
		return false;
	m_requestClose = false;
	return true;
}

void WarehouseWindow::EndFrameClassicDraw()
{
	if (!cWareHouse.OpenFlag && !cWareHouse.IsLoadingPages())
		m_hideClassic = false;
}

bool WarehouseWindow::IsBlockingMouse(int x, int y) const
{
	if (!cWareHouse.OpenFlag && !m_eatClick && !m_hideClassic)
		return false;
	if (m_eatClick || m_goldOpen)
		return true;
	return m_winW > 0.0f && m_winH > 0.0f
		&& x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH);
}

bool WarehouseWindow::IsOverGrid(int x, int y) const
{
	if (!cWareHouse.OpenFlag || m_gridW <= 0.0f || m_gridH <= 0.0f)
		return false;
	return x >= (int)m_gridX && x < (int)(m_gridX + m_gridW)
		&& y >= (int)m_gridY && y < (int)(m_gridY + m_gridH);
}

void WarehouseWindow::ScreenToLogical(int screenX, int screenY, int* outX, int* outY) const
{
	const int vis = (int)kCell;
	if (vis <= 0)
		return;
	if (outX)
		*outX = kLogicOriginX + ((screenX - (int)m_gridX) * kLogicCell) / vis;
	if (outY)
		*outY = LogicOriginY() + ((screenY - (int)m_gridY) * kLogicCell) / vis;
}

void WarehouseWindow::LogicalToScreen(int logicX, int logicY, float* outX, float* outY) const
{
	const int vis = (int)kCell;
	if (vis <= 0)
		return;
	if (outX)
		*outX = m_gridX + (float)((logicX - kLogicOriginX) * vis) / (float)kLogicCell;
	if (outY)
		*outY = m_gridY + (float)((logicY - LogicOriginY()) * vis) / (float)kLogicCell;
}

void WarehouseWindow::DrawSearch()
{
	ImGui::TextUnformatted("Busca");
	ImGui::SetNextItemWidth(-1.0f);
	ImGui::InputTextWithHint("##WarehouseSearch", "nome do item", m_searchUtf8, IM_ARRAYSIZE(m_searchUtf8));
	m_searchFocused = ImGui::IsItemActive();
	if (ImGui::IsItemDeactivatedAfterEdit() && ImGui::IsKeyPressed(ImGuiKey_Enter))
		m_searchFocused = true;

	const std::string acp = Utf8ToAcp(m_searchUtf8);
	strncpy_s(cWareHouse.szSearch, sizeof(cWareHouse.szSearch), acp.c_str(), _TRUNCATE);

	if (cWareHouse.szSearch[0] && !MouseItem.Flag)
	{
		const int page = cWareHouse.FindSearchPage();
		if (page != cWareHouse.CurrentPage)
		{
			cWareHouse.SwitchPage(page);
			m_eatClick = true;
		}
	}
}

void WarehouseWindow::DrawPages()
{
	for (int i = 0; i < WAREHOUSE_PAGE_COUNT; i++)
	{
		if (i > 0)
			ImGui::SameLine();

		char label[24];
		sprintf_s(label, sizeof(label), "%d###WhPage%d", i + 1, i);
		ImGui::PushID(i);
		const bool selected = (cWareHouse.CurrentPage == i);
		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.36f, 0.28f, 0.14f, 1.0f));
		if (ImGui::Button(label, ImVec2(48.0f, 26.0f)))
		{
			cWareHouse.SwitchPage(i);
			m_eatClick = true;
		}
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
			m_eatClick = true;
		if (selected)
			ImGui::PopStyleColor();
		ImGui::PopID();
	}
}

void WarehouseWindow::ApplyGoldMove(bool deposit)
{
	int count = m_goldAmount;
	if (deposit)
	{
		count = ClampDeposit(count);
		if (count <= 0)
			return;
		if (sinChar->Money - count < 0)
		{
			SendSetHackUser2(4100, 3);
			count = sinChar->Money;
		}
		CheckCharForm();
		sinMinusMoney2(count);
		ReformCharForm();
		sWareHouse.Money += count;
	}
	else
	{
		count = ClampWithdraw(count);
		if (count <= 0)
			return;
		if ((sWareHouse.Money - 2023) - count < 0)
		{
			SendSetHackUser2(4100, 4);
			count = WarehouseGold();
		}
		CheckCharForm();
		sinPlusMoney2(count);
		ReformCharForm();
		sWareHouse.Money -= count;
	}
	sinPlaySound(SIN_SOUND_COIN);
	WareHouseSaveFlag = 1;
	m_goldAmount = 0;
	m_goldInput[0] = 0;
}

void WarehouseWindow::OpenGoldModal(bool deposit)
{
	m_goldOpen = true;
	m_goldDeposit = deposit;
	m_goldAmount = 0;
	m_goldInput[0] = 0;
}

void WarehouseWindow::SyncGoldInputFromAmount()
{
	if (m_goldAmount <= 0)
	{
		m_goldInput[0] = 0;
		return;
	}
	FormatGoldShort(m_goldAmount, m_goldInput, IM_ARRAYSIZE(m_goldInput));
}

void WarehouseWindow::ParseGoldInput()
{
	int count = ParseGoldShorthand(m_goldInput);
	m_goldAmount = m_goldDeposit ? ClampDeposit(count) : ClampWithdraw(count);
}

void WarehouseWindow::DrawGoldModal()
{
	if (!m_goldOpen)
		return;

	ImGui::OpenPopup("##WarehouseGold");
	ImGui::SetNextWindowSize(ImVec2(kGoldModalW, kGoldModalH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(
		ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.38f),
		ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(18.0f, 10.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 6.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##WarehouseGold", nullptr, flags))
	{
		DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), 36.0f, kGold, kGoldFill, kPlayerOverlayBezelThickness);

		ImGui::SetCursorPosY(10.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted("OURO DO ARMAZ" U8_E "M");
		ImGui::PopStyleColor();

		ImGui::SetCursorPosY(44.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted(m_goldDeposit ? "Do personagem para o ba" U8_U : "Do ba" U8_U " para o personagem");
		ImGui::PopStyleColor();

		const float pad = 18.0f;
		const float innerW = kGoldModalW - pad * 2.0f;
		const float modeW = (innerW - 10.0f) * 0.5f;
		const bool depositOn = m_goldDeposit;
		ImGui::SetCursorPos(ImVec2(pad, 68.0f));
		if (depositOn)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.36f, 0.28f, 0.14f, 1.0f));
		if (ImGui::Button("DEPOSITAR", ImVec2(modeW, 26.0f)))
			m_goldDeposit = true;
		if (depositOn)
			ImGui::PopStyleColor();
		ImGui::SameLine(0.0f, 10.0f);
		if (!depositOn)
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.36f, 0.28f, 0.14f, 1.0f));
		if (ImGui::Button("SACAR", ImVec2(modeW, 26.0f)))
			m_goldDeposit = false;
		if (!depositOn)
			ImGui::PopStyleColor();

		char charGold[48] = {};
		char charShort[24] = {};
		char whGold[48] = {};
		char whShort[24] = {};
		FormatGold(sinChar->Money, charGold, sizeof(charGold));
		FormatGoldShort(sinChar->Money, charShort, sizeof(charShort));
		FormatGold(WarehouseGold(), whGold, sizeof(whGold));
		FormatGoldShort(WarehouseGold(), whShort, sizeof(whShort));

		const float cardW = (innerW - 10.0f) * 0.5f;
		const float cardH = 58.0f;
		ImGui::SetCursorPos(ImVec2(pad, 104.0f));
		DrawGoldBalanceCard("PERSONAGEM", charGold, charShort, cardW, cardH, m_goldDeposit);
		ImGui::SameLine(0.0f, 10.0f);
		DrawGoldBalanceCard("ARMAZ" U8_E "M", whGold, whShort, cardW, cardH, !m_goldDeposit);

		ImGui::SetCursorPos(ImVec2(pad, 172.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.40f, 1.0f));
		ImGui::TextUnformatted("VALOR");
		ImGui::PopStyleColor();
		ImGui::SetCursorPos(ImVec2(pad, 190.0f));
		ImGui::SetNextItemWidth(innerW);
		ImGui::InputTextWithHint("##WarehouseGoldAmt", "650k   ou   2.5kk", m_goldInput, IM_ARRAYSIZE(m_goldInput));
		ParseGoldInput();

		char previewFull[48] = {};
		char previewShort[24] = {};
		FormatGold(m_goldAmount, previewFull, sizeof(previewFull));
		FormatGoldShort(m_goldAmount, previewShort, sizeof(previewShort));
		char preview[96] = {};
		if (m_goldAmount > 0)
			sprintf_s(preview, sizeof(preview), "%s   (%s)", previewFull, previewShort);
		else
			strcpy_s(preview, sizeof(preview), "0");

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted(preview);
		ImGui::PopStyleColor();

		const float chipGap = 6.0f;
		const float chipH = 24.0f;
		const float maxW = 72.0f;
		const float chipW = (innerW - maxW - chipGap * 4.0f) / 4.0f;
		ImGui::SetCursorPos(ImVec2(pad, 246.0f));
		const int chips[] = { 1000, 10000, 100000, 1000000 };
		const char* chipLabels[] = { "1k", "10k", "100k", "1kk" };
		for (int i = 0; i < 4; i++)
		{
			if (i > 0)
				ImGui::SameLine(0.0f, chipGap);
			if (ImGui::Button(chipLabels[i], ImVec2(chipW, chipH)))
			{
				m_goldAmount = m_goldDeposit ? ClampDeposit(chips[i]) : ClampWithdraw(chips[i]);
				SyncGoldInputFromAmount();
			}
		}
		ImGui::SameLine(0.0f, chipGap);
		if (ImGui::Button("M" U8_A_UP "XIMO", ImVec2(maxW, chipH)))
		{
			m_goldAmount = m_goldDeposit ? ClampDeposit(2000000000) : ClampWithdraw(2000000000);
			SyncGoldInputFromAmount();
		}

		const float btnW = 140.0f;
		const float btnGap = 14.0f;
		ImGui::SetCursorPos(ImVec2((kGoldModalW - (btnW * 2.0f + btnGap)) * 0.5f, 320.0f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_goldOpen = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);

		ImGui::SameLine(0.0f, btnGap);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("CONFIRMAR", ImVec2(btnW, 30.0f)))
		{
			ParseGoldInput();
			ApplyGoldMove(m_goldDeposit);
			m_goldOpen = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(3);
}

void WarehouseWindow::DrawSideInfo()
{
	DrawPlayerColumnSplit();

	ImGui::BeginChild("##WarehouseSide", ImVec2(kSideW, 0.0f), false);
	ImGui::TextUnformatted("Ouro");
	char gold[48] = {};
	char goldShort[24] = {};
	FormatGold(WarehouseGold(), gold, sizeof(gold));
	FormatGoldShort(WarehouseGold(), goldShort, sizeof(goldShort));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.40f, 1.0f));
	ImGui::Text("%s (%s)", gold, goldShort);
	ImGui::PopStyleColor();

	if (ImGui::Button("Depositar / Sacar", ImVec2(-1.0f, 28.0f)))
		OpenGoldModal(false);

	ImGui::Spacing();
	ImGui::TextUnformatted("Peso");
	char weight[32] = {};
	sprintf_s(weight, sizeof(weight), "%d / %d", sWareHouse.Weight[0] - 197, sWareHouse.Weight[1] - 196);
	ImGui::TextUnformatted(weight);

	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.62f, 0.68f, 1.0f));
	ImGui::TextWrapped("Arraste ou clique com o direito para enviar. O invent" U8_A "rio de pedra fica ao lado.");
	ImGui::PopStyleColor();
	ImGui::EndChild();
}

bool WarehouseWindow::TrySendWarehouseItemToInventory(int logX, int logY)
{
	if (MouseItem.Flag || m_goldOpen)
		return false;

	if (!cWareHouse.PickUpWareHouseItem(logX, logY, TRUE))
		return false;

	if (!cInvenTory.CheckSetOk(&MouseItem, 1))
	{
		POINT empty = {};
		if (cWareHouse.GetEmptyArea(&MouseItem, &empty))
		{
			MouseItem.SetX = empty.x;
			MouseItem.SetY = empty.y;
			cWareHouse.LastSetWareHouseItem(&MouseItem);
		}
		return true;
	}

	if (cCraftItem.CraftCheckEmptyArea(&MouseItem))
	{
		MouseItem.x = MouseItem.SetX;
		MouseItem.y = MouseItem.SetY;
		cInvenTory.LastSetInvenItem(&MouseItem);
	}
	else
	{
		cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
		POINT empty = {};
		if (cWareHouse.GetEmptyArea(&MouseItem, &empty))
		{
			MouseItem.SetX = empty.x;
			MouseItem.SetY = empty.y;
			cWareHouse.LastSetWareHouseItem(&MouseItem);
		}
	}
	return true;
}

void WarehouseWindow::HandleDragAndClick()
{
	if (m_goldOpen)
		return;

	extern POINT pCursorPos;
	const ImVec2 mouse = ImGui::GetIO().MousePos;
	const bool overGrid = mouse.x >= m_gridX && mouse.x < m_gridX + m_gridW
		&& mouse.y >= m_gridY && mouse.y < m_gridY + m_gridH;

	int logX = 0;
	int logY = 0;
	ScreenToLogical(pCursorPos.x, pCursorPos.y, &logX, &logY);

	if (MouseItem.Flag)
	{
		if (!overGrid)
			return;

		sITEM tmp = MouseItem;
		SnapHeldItemToCursor(&tmp, logX, logY);
		cWareHouse.SetWareHouseItemAreaCheck(&tmp);
		if (TradeCrashItemIndex[0] > 0)
		{
			sITEM* crash = &sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1];
			if (crash->Flag && !cWareHouse.ItemMatchesSearch(crash))
				TradeColorIndex = NOT_SETTING_COLOR;
		}
		MouseItem.SetX = tmp.SetX;
		MouseItem.SetY = tmp.SetY;
		MouseItem.ItemPosition = tmp.ItemPosition;

		if (!m_placeArmed)
		{
			if (!ImGui::IsMouseDown(ImGuiMouseButton_Left))
				m_placeArmed = true;
			return;
		}

		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			cInvenTory.CheckRequireItemToSet(&MouseItem);
			if (TradeColorIndex != NO_BOX_COLOR && TradeColorIndex != NOT_SETTING_COLOR)
			{
				if (!cWareHouse.ChangeWareHouseItem(&MouseItem))
					cWareHouse.LastSetWareHouseItem(&MouseItem);
				m_placeArmed = false;
			}
		}
		return;
	}

	m_placeArmed = false;

	if (!overGrid)
		return;

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
	{
		TrySendWarehouseItemToInventory(logX, logY);
		m_eatClick = true;
		return;
	}

	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		cWareHouse.PickUpWareHouseItem(logX, logY, 1);
	else
		cWareHouse.PickUpWareHouseItem(logX, logY, 0);
}

void WarehouseWindow::DrawGrid()
{
	m_gridW = kCell * (float)kGridN;
	m_gridH = kCell * (float)kGridN;
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	m_gridX = origin.x;
	m_gridY = origin.y;

	ImDrawList* draw = ImGui::GetWindowDrawList();
	const ImVec2 g1(m_gridX + m_gridW, m_gridY + m_gridH);
	draw->AddRectFilled(origin, g1, IM_COL32(12, 14, 18, 230), 3.0f);
	draw->AddRect(origin, g1, kGold, 3.0f, 0, 1.2f);

	for (int i = 1; i < kGridN; i++)
	{
		const float x = m_gridX + kCell * (float)i;
		const float y = m_gridY + kCell * (float)i;
		draw->AddLine(ImVec2(x, m_gridY), ImVec2(x, g1.y), IM_COL32(200, 170, 90, 40), 1.0f);
		draw->AddLine(ImVec2(m_gridX, y), ImVec2(g1.x, y), IM_COL32(200, 170, 90, 40), 1.0f);
	}

	const float scale = 1.0f;

	if (MouseItem.Flag && TradeColorRect.right > 0 && TradeColorRect.bottom > 0)
	{
		float hx = 0.0f;
		float hy = 0.0f;
		LogicalToScreen(TradeColorRect.left, TradeColorRect.top, &hx, &hy);
		ImU32 col = IM_COL32(80, 160, 80, 80);
		if (TradeColorIndex == OVERLAP_ITEM_COLOR)
			col = IM_COL32(200, 160, 40, 90);
		else if (TradeColorIndex == NOT_SETTING_COLOR)
			col = IM_COL32(180, 50, 50, 90);
		else if (TradeColorIndex != SET_ITEM_CHECK_COLOR)
			col = IM_COL32(180, 50, 50, 70);
		draw->AddRectFilled(ImVec2(hx, hy),
			ImVec2(hx + TradeColorRect.right * scale, hy + TradeColorRect.bottom * scale),
			col, 2.0f);
	}

	if (TradeColorIndex == OVERLAP_ITEM_COLOR && TradeCrashItemIndex[0] > 0)
	{
		sITEM* crash = &sWareHouse.WareHouseItem[TradeCrashItemIndex[0] - 1];
		if (crash->Flag)
		{
			float hx = 0.0f;
			float hy = 0.0f;
			LogicalToScreen(crash->x, crash->y, &hx, &hy);
			draw->AddRectFilled(ImVec2(hx, hy), ImVec2(hx + crash->w * scale, hy + crash->h * scale),
				IM_COL32(200, 160, 40, 80), 2.0f);
		}
	}

	if (SelectTradeItemIndex > 0 && sWareHouse.WareHouseItem[SelectTradeItemIndex - 1].Flag)
	{
		sITEM* sel = &sWareHouse.WareHouseItem[SelectTradeItemIndex - 1];
		float hx = 0.0f;
		float hy = 0.0f;
		LogicalToScreen(sel->x, sel->y, &hx, &hy);
		draw->AddRect(ImVec2(hx, hy), ImVec2(hx + sel->w * scale, hy + sel->h * scale),
			IM_COL32(230, 200, 110, 220), 2.0f, 0, 1.4f);
	}

	for (int i = 0; i < WAREHOUSE_PAGE_SLOTS; i++)
	{
		sITEM* item = &sWareHouse.WareHouseItem[i];
		if (!item->Flag || !cWareHouse.ItemMatchesSearch(item))
			continue;

		float sx = 0.0f;
		float sy = 0.0f;
		LogicalToScreen(item->x, item->y, &sx, &sy);
		const float dw = (float)item->w * scale;
		const float dh = (float)item->h * scale;
		if (item->lpItem)
			draw->AddImage((ImTextureID)item->lpItem, ImVec2(sx, sy), ImVec2(sx + dw, sy + dh));

		if (item->Class == ITEM_CLASS_POTION)
		{
			char count[16] = {};
			sprintf_s(count, sizeof(count), "%d", item->sItemInfo.PotionCount);
			draw->AddText(ImVec2(sx + 2.0f, sy + 2.0f), IM_COL32(255, 255, 255, 255), count);
		}
	}

	ImGui::Dummy(ImVec2(m_gridW, m_gridH));
}

void WarehouseWindow::OpenNpc(bool* p_open)
{
	m_eatClick = false;
	m_hideClassic = true;

	if (cWareHouse.OpenFlag && !m_wasOpen)
	{
		ClearSearch();
		m_placeArmed = false;
		m_goldOpen = false;
		m_goldAmount = 0;
		m_goldInput[0] = 0;
	}
	m_wasOpen = cWareHouse.OpenFlag != 0;

	const float invReserve = 210.0f;
	float posY = ((float)smScreenHeight - invReserve - kWindowH) * 0.5f;
	if (posY < 16.0f)
		posY = 16.0f;

	ImGui::SetNextWindowSize(ImVec2(kWindowW, kWindowH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(24.0f, posY));

	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##WarehouseWindow", nullptr, flags);
	{
		const ImVec2 pos = ImGui::GetWindowPos();
		const ImVec2 size = ImGui::GetWindowSize();
		m_winX = pos.x;
		m_winY = pos.y;
		m_winW = size.x;
		m_winH = size.y;

		DrawWindowChrome(kMainHeaderH);
		DrawTitleHeader();

		SetPlayerWindowBodyCursor(kMainHeaderH);

		DrawSearch();
		DrawPages();
		const ImVec2 bodyCursor = ImGui::GetCursorPos();
		if (p_open && !m_eatClick && DrawHeaderClose())
			*p_open = false;
		ImGui::SetCursorPos(bodyCursor);
		ImGui::Spacing();

		ImGui::BeginChild("##WarehouseGridCol", ImVec2(kCell * (float)kGridN + kGridPad, kCell * (float)kGridN + kGridPad), false,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		DrawGrid();
		ImGui::EndChild();

		ImGui::SameLine();
		DrawSideInfo();

		HandleDragAndClick();
		DrawGoldModal();

		ImGui::End();
	}

	PopWindowStyle();

	if (m_eatClick && p_open)
		*p_open = true;
}
