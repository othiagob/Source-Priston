#include "..\\sinbaram\\sinLinkHeader.h"
#include "Quest.h"
#include "imGui/imgui.h"
#include "../HUD/ImGuiWindowChrome.h"

// Sequencias UTF-8 (nao dependem do encoding do arquivo .cpp)
#define U8_A "\xC3\xA1"   // a agudo
#define U8_E "\xC3\xA9"   // e agudo
#define U8_I "\xC3\xAD"   // i agudo
#define U8_O "\xC3\xB3"   // o agudo
#define U8_U "\xC3\xBA"   // u agudo
#define U8_AN "\xC3\xA3"  // a til
#define U8_ON "\xC3\xB5"  // o til
#define U8_C "\xC3\xA7"   // c cedilha
#define U8_AC "\xC3\xA2"  // a circunflexo
#define U8_AU "\xC3\x9A"  // U agudo maiusculo

// Windows-1252 (ANSI do client). O minimapa desenha com DrawTextA e
// precisa desses bytes. ImGui converte com ToUtf8().
#define A8_A  "\xE1"
#define A8_E  "\xE9"
#define A8_I  "\xED"
#define A8_O  "\xF3"
#define A8_U  "\xFA"
#define A8_AN "\xE3"
#define A8_ON "\xF5"
#define A8_C  "\xE7"
#define A8_AC "\xE2"

extern QUEST_INFOPCKG questInfoPlayer;
extern int smScreenWidth;
extern int smScreenHeight;

// NewShop.cpp / NewShopTime.cpp ainda chamam esta funcao (extern).
// MiniMapHandler.cpp ainda usa o array MapasWU8 (extern).
// Nao remover: o linker do client depende desses simbolos.
static long getUTF8size2(const wchar_t* string)
{
	if (!string)
		return 0;
	long res = 0;
	for (; *string; string++)
	{
		if (*string < 0x80)
			res++;
		else if (*string < 0x800)
			res += 2;
		else
			res += 3;
	}
	return res;
}

char* WChar_to_UTF82(const wchar_t* string)
{
	const long fSize = getUTF8size2(string);
	char* res = new char[fSize + 1];
	res[fSize] = 0;
	if (!string)
		return res;

	long b = 0;
	for (; *string; string++, b++)
	{
		if (*string < 0x80)
			res[b] = (char)*string;
		else if (*string < 0x800)
		{
			res[b++] = (char)((*string >> 6) | 192);
			res[b] = (char)(*string & 63 | 128);
		}
		else
		{
			res[b++] = (char)((*string >> 12) | 224);
			res[b++] = (char)(((*string & 4095) >> 6) | 128);
			res[b] = (char)(*string & 63 | 128);
		}
	}
	return res;
}

char* MapasWU8[] = {
	"Mata das Ac" A8_A "cias",
	"F. Bamboo",
	"Jardim da Liberdade",
	"Cidade de Ricarten",
	"Ref" A8_U "gio dos A.",
	"Castelo dos Perdidos",
	"Vila Ruinen",
	"Terra Maldita",
	"Terra Esquecida",
	"Cidade de Navisko",
	"O" A8_A "sis",
	"C. B. A.",
	"Terra Proibida",
	"C. A. 1",
	"C. A. 2",
	"C. A. 3",
	"Sala do GM",
	"F. dos Esp" A8_I "ritos",
	"F. das Ilus" A8_ON "es",
	"Vale Tranquilo",
	"E. dos Ventos",
	"Cidade de Pillai",
	"Templo Maldito 1",
	"Templo Maldito 2",
	"C. dos Cogumelos",
	"Caverna das Abelhas",
	"Santu" A8_A "rio Sombrio",
	"E. d. F. do Caos",
	"Cora" A8_C A8_AN "o de Perum",
	"Eura",
	"S.o.D",
	"Vale Galubia",
	"Sala de Desafios",
	"Castelo Aben" A8_C "oado",
	"Lago da Gan" A8_AC "ncia",
	"Santu" A8_A "rio Congelado",
	"Covil do Kelvezu",
	"Ilha Perdida",
	"Templo Perdido",
	"Mapa 2D",
	"Torre sem Fim 1",
	"Torre sem Fim 2",
	"Templo Maldito 3",
	"Torre sem Fim 3",
	"Laborat" A8_O "rio Secreto",
	"Arma Antiga",
	"Mina de Gelo 1",
	"Arquip" A8_E "lago Perdido",
	"Covil do Eragon",
	"Arena de Batalha",
	"N" A8_U "cleo de Ferro",
	"Abismo do Mar",
};

static const int kMapCount = (int)(sizeof(MapasWU8) / sizeof(MapasWU8[0]));
static const int kQuestSlotMax = 61;

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\quest\\desafios.png";
static const char* kOverlayTitlePath = "game\\images\\quest\\emandamento.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const float kHeaderBtnGap = 4.0f;

enum HeaderIconKind
{
	HeaderIcon_Close = 0,
	HeaderIcon_Minus = 1,
	HeaderIcon_Plus = 2
};

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

static bool DrawHeaderIcon(const char* id, HeaderIconKind kind)
{
	ImGui::InvisibleButton(id, ImVec2(kHeaderBtnW, kHeaderBtnH));
	const bool hovered = ImGui::IsItemHovered();
	const bool clicked = ImGui::IsItemClicked();
	const ImVec2 p = ImGui::GetItemRectMin();
	const ImVec2 b1(p.x + kHeaderBtnW, p.y + kHeaderBtnH);
	ImDrawList* draw = ImGui::GetWindowDrawList();
	DrawPlayerControlBezel(draw, p, b1, hovered);

	const ImVec2 c((p.x + b1.x) * 0.5f, (p.y + b1.y) * 0.5f);
	const ImU32 iconCol = IM_COL32(236, 220, 160, 255);
	if (kind == HeaderIcon_Close)
	{
		const float arm = 4.8f;
		draw->AddLine(ImVec2(c.x - arm, c.y - arm), ImVec2(c.x + arm, c.y + arm), iconCol, 1.7f);
		draw->AddLine(ImVec2(c.x + arm, c.y - arm), ImVec2(c.x - arm, c.y + arm), iconCol, 1.7f);
	}
	else if (kind == HeaderIcon_Minus)
	{
		const float arm = 5.2f;
		draw->AddLine(ImVec2(c.x - arm, c.y), ImVec2(c.x + arm, c.y), iconCol, 1.7f);
	}
	else
	{
		const float arm = 5.2f;
		draw->AddLine(ImVec2(c.x - arm, c.y), ImVec2(c.x + arm, c.y), iconCol, 1.7f);
		draw->AddLine(ImVec2(c.x, c.y - arm), ImVec2(c.x, c.y + arm), iconCol, 1.7f);
	}
	return clicked;
}

static bool DrawHeaderIconAt(const char* id, const ImVec2& screenPos, HeaderIconKind kind)
{
	ImGui::SetCursorScreenPos(screenPos);
	return DrawHeaderIcon(id, kind);
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

static void DrawQuestProgressBar(float fraction, const char* overlay, float height)
{
	if (fraction < 0.0f)
		fraction = 0.0f;
	if (fraction > 1.0f)
		fraction = 1.0f;

	const ImVec2 p = ImGui::GetCursorScreenPos();
	const float w = ImGui::GetContentRegionAvail().x;
	const ImVec2 r1(p.x + w, p.y + height);
	ImDrawList* draw = ImGui::GetWindowDrawList();
	const float rnd = 3.0f;
	draw->AddRectFilled(p, r1, IM_COL32(16, 18, 22, 255), rnd);
	if (fraction > 0.005f)
	{
		float fillW = w * fraction;
		if (fillW < 6.0f)
			fillW = 6.0f;
		draw->AddRectFilled(p, ImVec2(p.x + fillW, r1.y), ImGui::GetColorU32(ImGuiCol_PlotHistogram), rnd);
	}
	draw->AddRect(p, r1, kGold, rnd, 0, 1.15f);
	if (overlay && overlay[0])
	{
		const ImVec2 ts = ImGui::CalcTextSize(overlay);
		draw->AddText(ImVec2(p.x + (w - ts.x) * 0.5f, p.y + (height - ts.y) * 0.5f),
			IM_COL32(236, 230, 210, 255), overlay);
	}
	ImGui::Dummy(ImVec2(w, height));
}

static void CenterTextUnformatted(const char* text)
{
	const float maxW = ImGui::GetWindowSize().x - 24.0f;
	const ImVec2 ts = ImGui::CalcTextSize(text);
	if (ts.x > maxW)
	{
		ImGui::SetCursorPosX(12.0f);
		ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + maxW);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		return;
	}
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
	ImGui::TextUnformatted(text);
}

static void FormatIntPt(char* buf, size_t bufSize, long long value)
{
	char raw[32];
	sprintf_s(raw, "%I64d", value);
	const bool neg = (raw[0] == '-');
	const char* digits = neg ? raw + 1 : raw;
	const int n = (int)strlen(digits);
	int out = 0;
	if (neg && out + 1 < (int)bufSize)
		buf[out++] = '-';
	for (int i = 0; i < n && out + 1 < (int)bufSize; i++)
	{
		if (i > 0 && ((n - i) % 3) == 0)
			buf[out++] = '.';
		buf[out++] = digits[i];
	}
	buf[out] = 0;
}

unsigned int QuestWindow::ColorForGroup(QuestGroup group) const
{
	switch (group)
	{
	case QuestGroup::Ready:      return IM_COL32(200, 170, 90, 255);
	case QuestGroup::Active:     return IM_COL32(80, 190, 230, 255);
	case QuestGroup::Available:  return IM_COL32(210, 160, 70, 255);
	case QuestGroup::Complete:   return IM_COL32(110, 180, 120, 255);
	}
	return IM_COL32(255, 255, 255, 255);
}

int QuestWindow::PlayerLevel() const
{
	return sinChar ? sinChar->Level : 0;
}

bool QuestWindow::IsInLevelRange(const QUEST_BODY* body) const
{
	if (!body)
		return false;

	const int lv = PlayerLevel();
	if (lv <= 0)
		return true;
	if (body->minLevel > 0 && lv < body->minLevel)
		return false;
	if (body->maxLevel > 0 && lv > body->maxLevel)
		return false;
	return true;
}

void QuestWindow::GetQuestProgress(const QUEST_BODY* body, const QUEST_INFO* info, int* current, int* total) const
{
	int cur = 0;
	int tot = 0;
	if (body && info)
	{
		for (int i = 0; i < 3; i++)
		{
			if (body->monsterNum[i] > 0)
			{
				cur += info->questProgress[i];
				tot += body->monsterNum[i];
			}
		}
		if (body->playersToKill > 0)
		{
			cur += info->killProgress;
			tot += body->playersToKill;
		}
		if (tot <= 0)
		{
			for (int i = 0; i < 3; i++)
			{
				if (body->qtItens[i] > 0)
					tot += body->qtItens[i];
			}
		}
	}
	if (current)
		*current = cur;
	if (total)
		*total = tot;
}

void QuestWindow::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;
	LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH);
}

void QuestWindow::EnsureOverlayTitleTexture()
{
	if (m_overlayTitleTried)
		return;
	m_overlayTitleTried = true;
	LoadPngTexture(kOverlayTitlePath, &m_overlayTitleTex, &m_overlayTitleW, &m_overlayTitleH);
}

void QuestWindow::DrawTitleHeader(bool* p_open)
{
	EnsureTitleTexture();

	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	ImDrawList* draw = ImGui::GetWindowDrawList();

	if (m_titleTex && m_titleW > 0 && m_titleH > 0)
	{
		const ImVec2 sz = FitImageSize(m_titleW, m_titleH, 260.0f, 38.0f);
		const float x = p0.x + (size.x - sz.x) * 0.5f;
		const float y = p0.y + (kMainHeaderH - sz.y) * 0.5f + 1.0f;
		draw->AddImage((ImTextureID)m_titleTex, ImVec2(x, y), ImVec2(x + sz.x, y + sz.y));
	}
	else
	{
		const char* title = "DESAFIOS";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + (kMainHeaderH - ts.y) * 0.5f), kGoldBright, title);
	}

	if (p_open)
	{
		ImGui::SetCursorPos(ImVec2(size.x - kHeaderBtnW - 12.0f, (kMainHeaderH - kHeaderBtnH) * 0.5f));
		if (DrawHeaderIcon("##QuestClose", HeaderIcon_Close))
			*p_open = false;
	}
}

void QuestWindow::AskCancelQuest(const char* title, int questID, int questType)
{
	m_confirmOpen = true;
	m_confirmQuestId = questID;
	m_confirmQuestType = questType;
	m_confirmTitle = ToUtf8(title);
	if (m_confirmTitle.empty())
		m_confirmTitle = u8"Desafio";
}

void QuestWindow::DrawConfirmChrome()
{
	DrawWindowChrome(34.0f);
}

void QuestWindow::DrawCancelConfirm()
{
	if (!m_confirmOpen)
		return;

	ImGui::OpenPopup("##QuestCancelConfirm");
	ImGui::SetNextWindowSize(ImVec2(332.0f, 176.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##QuestCancelConfirm", nullptr, flags))
	{
		DrawConfirmChrome();

		ImGui::SetCursorPosY(11.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted(u8"Cancelar desafio?");
		ImGui::PopStyleColor();

		ImGui::SetCursorPosY(48.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.70f, 1.0f));
		CenterTextUnformatted(m_confirmTitle.c_str());
		ImGui::PopStyleColor();

		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted("O progresso ser" U8_A " perdido.");
		ImGui::PopStyleColor();

		const float btnW = 128.0f;
		const float gap = 14.0f;
		ImGui::SetCursorPos(ImVec2((332.0f - (btnW * 2.0f + gap)) * 0.5f, 128.0f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button(u8"VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_confirmOpen = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);

		ImGui::SameLine(0.0f, gap);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.28f, 0.10f, 0.10f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.42f, 0.14f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.16f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.42f, 0.34f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.86f, 0.82f, 1.0f));
		if (ImGui::Button(u8"CONFIRMAR", ImVec2(btnW, 30.0f)))
		{
			Quest::GetInstance()->cancelQuest(m_confirmQuestId, m_confirmQuestType);
			m_confirmOpen = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void QuestWindow::PushWindowStyle()
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

void QuestWindow::PopWindowStyle()
{
	ImGui::PopStyleColor(22);
	ImGui::PopStyleVar(10);
}

void QuestWindow::DrawWindowChrome(float headerH)
{
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void QuestWindow::DrawOverlayChrome(float headerH)
{
	DrawPlayerWindowChrome(
		ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill,
		kPlayerOverlayBezelThickness);
}

std::string QuestWindow::ToUtf8(const char* src) const
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

bool QuestWindow::MatchesTab(int questType, int tab) const
{
	using QT = Quest::QUEST_TYPES;
	if (tab == 0)
		return questType == QT::QUEST_UNIQUE_SOLO || questType == QT::QUEST_UNIQUE_SOLOPARTY || questType == QT::QUEST_UNIQUE_PARTY;
	if (tab == 1)
		return questType == QT::QUEST_DAILY_SOLO || questType == QT::QUEST_DAILY_SOLOPARTY || questType == QT::QUEST_DAILY_PARTY;
	return questType == QT::QUEST_REPEAT_SOLO || questType == QT::QUEST_REPEAT_SOLOPARTY || questType == QT::QUEST_REPEAT_PARTY;
}

QuestWindow::QuestGroup QuestWindow::GetGroup(const QUEST_INFO* info) const
{
	if (!info)
		return QuestGroup::Available;
	if (info->isReadyToComplete)
		return QuestGroup::Ready;
	if (info->isActive)
		return QuestGroup::Active;
	if (info->isComplete)
		return QuestGroup::Complete;
	return QuestGroup::Available;
}

const QUEST_BODY* QuestWindow::FindBody(int questID) const
{
	auto& quests = Quest::GetInstance()->vQuests;
	if (quests.empty() || questID <= 0)
		return nullptr;

	QUESTPCKG* pkg = quests[0];
	for (int i = 0; i < kQuestSlotMax; i++)
	{
		if (pkg->QuestPckg[i].questID == questID)
			return &pkg->QuestPckg[i];
	}

	if (questID <= kQuestSlotMax)
		return &pkg->QuestPckg[questID - 1];
	return nullptr;
}

int QuestWindow::FindDefaultSelection(int tab) const
{
	auto& quests = Quest::GetInstance()->vQuests;
	if (quests.empty())
		return 0;

	int found[4] = { 0, 0, 0, 0 };
	QUESTPCKG* pkg = quests[0];
	for (int i = 0; i < kQuestSlotMax; i++)
	{
		const QUEST_BODY& body = pkg->QuestPckg[i];
		if (!body.questID || !MatchesTab(body.questType, tab))
			continue;

		const QuestGroup group = GetGroup(&questInfoPlayer.QuestPckg[body.questID]);
		const int slot = (int)group;
		if (!found[slot])
			found[slot] = body.questID;
	}

	if (found[0]) return found[0];
	if (found[1]) return found[1];
	if (found[2]) return found[2];
	return found[3];
}

void QuestWindow::EnsureSelectionForTab(int tab)
{
	const QUEST_BODY* body = FindBody(m_selected);
	if (!body || !MatchesTab(body->questType, tab))
	{
		const int nextId = FindDefaultSelection(tab);
		if (nextId)
			m_selected = nextId;
	}
}

void QuestWindow::FocusQuest(int questID)
{
	const QUEST_BODY* body = FindBody(questID);
	if (!body || !body->questID)
		return;

	m_selected = questID;
	for (int tab = 0; tab < 3; tab++)
	{
		if (MatchesTab(body->questType, tab))
		{
			m_activeTab = tab;
			m_forceSelectTab = true;
			break;
		}
	}
	openFlag = true;
}

bool QuestWindow::IsBlockingMouse(int x, int y) const
{
	if (m_confirmOpen)
		return true;
	if (openFlag && m_winW > 0.0f && m_winH > 0.0f
		&& x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH))
		return true;
	if (!openOverlay)
		return false;
	return x >= m_overlayX && x <= (m_overlayX + m_overlayW)
		&& y >= m_overlayY && y <= (m_overlayY + m_overlayH);
}

bool QuestWindow::IsShownOnOverlay(int questID) const
{
	if (questID <= 0 || questID >= kQuestOverlayHideMax)
		return true;
	return !m_overlayHidden[questID];
}

void QuestWindow::SetShownOnOverlay(int questID, bool shown)
{
	if (questID <= 0 || questID >= kQuestOverlayHideMax)
		return;
	m_overlayHidden[questID] = !shown;
}

const char* QuestWindow::TypeLabel(int questType) const
{
	using QT = Quest::QUEST_TYPES;
	switch (questType)
	{
	case QT::QUEST_UNIQUE_SOLO:       return U8_AU "nica - Solo";
	case QT::QUEST_UNIQUE_PARTY:      return U8_AU "nica - Grupo";
	case QT::QUEST_UNIQUE_SOLOPARTY:  return U8_AU "nica - Solo/Grupo";
	case QT::QUEST_DAILY_SOLO:        return "Di" U8_A "ria - Solo";
	case QT::QUEST_DAILY_PARTY:       return "Di" U8_A "ria - Grupo";
	case QT::QUEST_DAILY_SOLOPARTY:   return "Di" U8_A "ria - Solo/Grupo";
	case QT::QUEST_REPEAT_SOLO:       return "Repetitiva - Solo";
	case QT::QUEST_REPEAT_PARTY:      return "Repetitiva - Grupo";
	case QT::QUEST_REPEAT_SOLOPARTY:  return "Repetitiva - Solo/Grupo";
	default:                          return u8"Desafio";
	}
}

const char* QuestWindow::GroupLabel(QuestGroup group) const
{
	switch (group)
	{
	case QuestGroup::Ready:     return u8"Entregar";
	case QuestGroup::Active:    return u8"Em andamento";
	case QuestGroup::Available: return "Dispon" U8_I "vel";
	case QuestGroup::Complete:  return "Conclu" U8_I "da";
	}
	return "";
}

std::string QuestWindow::MapName(int mapId) const
{
	if (mapId <= 0)
		return u8"Qualquer mapa";
	if (mapId >= kMapCount)
		return u8"Mapa";
	return ToUtf8(MapasWU8[mapId]);
}

void QuestWindow::DrawObjectiveRow(const char* label, int current, int total, const char* mapName)
{
	if (total <= 0)
		return;

	ImGui::TextUnformatted(label);
	ImGui::SameLine();
	char overlay[32];
	sprintf_s(overlay, "%d/%d", current < 0 ? 0 : current, total);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.82f, 0.55f, 1.0f));
	ImGui::TextUnformatted(overlay);
	ImGui::PopStyleColor();
	const float fraction = total > 0 ? ((float)current / (float)total) : 0.0f;
	DrawQuestProgressBar(fraction, "", 14.0f);
	if (mapName && mapName[0])
		ImGui::TextDisabled("%s", mapName);
	ImGui::Spacing();
}

void QuestWindow::DrawSectionHeader(const char* title)
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

void QuestWindow::DrawBadge(const char* text, unsigned int color)
{
	const ImVec2 pad(7.0f, 3.0f);
	const ImVec2 textSize = ImGui::CalcTextSize(text);
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const ImVec2 rmax(p.x + textSize.x + pad.x * 2.0f, p.y + textSize.y + pad.y * 2.0f);
	ImDrawList* draw = ImGui::GetWindowDrawList();
	const ImU32 fill = (color & 0x00FFFFFF) | IM_COL32(0, 0, 0, 70);
	draw->AddRectFilled(p, rmax, fill, 4.0f);
	draw->AddRect(p, rmax, color, 4.0f, 0, 1.0f);
	draw->AddText(ImVec2(p.x + pad.x, p.y + pad.y), color, text);
	ImGui::Dummy(ImVec2(textSize.x + pad.x * 2.0f + 6.0f, textSize.y + pad.y * 2.0f));
	ImGui::SameLine(0.0f, 0.0f);
}

void QuestWindow::DrawListItem(const QUEST_BODY* body, const QUEST_INFO* info, bool showLevelHint)
{
	if (!body || !body->questID)
		return;

	ImGui::PushID(body->questID);
	const QuestGroup group = GetGroup(info);
	const std::string name = ToUtf8(body->questName);
	const bool selected = (m_selected == body->questID);
	const bool inRange = IsInLevelRange(body);
	const bool dimmed = showLevelHint && !inRange && group == QuestGroup::Available;

	if (ImGui::Selectable("##quest", selected, 0, ImVec2(0.0f, 38.0f)))
		m_selected = body->questID;

	const ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(min, ImVec2(min.x + 3.0f, max.y),
		dimmed ? IM_COL32(90, 90, 96, 255) : ColorForGroup(group));

	const ImU32 nameCol = dimmed ? IM_COL32(150, 150, 158, 255) : IM_COL32(255, 255, 255, 255);
	draw->AddText(ImVec2(min.x + 10.0f, min.y + 4.0f), nameCol,
		name.empty() ? u8"(sem nome)" : name.c_str());

	const char* status = GroupLabel(group);
	ImU32 statusCol = ColorForGroup(group);
	if (dimmed)
	{
		status = "Fora do n" U8_I "vel";
		statusCol = IM_COL32(190, 110, 110, 255);
	}
	draw->AddText(ImVec2(min.x + 10.0f, min.y + 20.0f), statusCol, status);

	char right[24] = {};
	if (info && (info->isActive || info->isReadyToComplete))
	{
		int current = 0, total = 0;
		for (int i = 0; i < 3; i++)
		{
			if (body->monsterNum[i] > 0)
			{
				current += info->questProgress[i];
				total += body->monsterNum[i];
			}
		}
		if (body->playersToKill > 0)
		{
			current += info->killProgress;
			total += body->playersToKill;
		}
		if (total > 0)
			sprintf_s(right, "%d/%d", current, total);
	}
	else if (showLevelHint)
	{
		sprintf_s(right, "Nv. %d-%d", body->minLevel, body->maxLevel);
	}

	if (right[0])
	{
		const ImVec2 textSize = ImGui::CalcTextSize(right);
		const ImU32 rightCol = dimmed ? IM_COL32(170, 110, 110, 255)
			: (showLevelHint && inRange && group == QuestGroup::Available)
			? IM_COL32(150, 190, 120, 255)
			: IM_COL32(170, 170, 180, 255);
		draw->AddText(ImVec2(max.x - textSize.x - 8.0f, min.y + 20.0f), rightCol, right);
	}

	ImGui::PopID();
}

void QuestWindow::DrawQuestList(int tab)
{
	ImGui::PushID(tab);
	auto& quests = Quest::GetInstance()->vQuests;
	if (quests.empty())
	{
		ImGui::TextDisabled(u8"Nenhum desafio carregado.");
		ImGui::PopID();
		return;
	}

	QUESTPCKG* pkg = quests[0];
	const QUEST_BODY* grouped[4][kQuestSlotMax] = {};
	int counts[4] = { 0, 0, 0, 0 };

	for (int i = 0; i < kQuestSlotMax; i++)
	{
		const QUEST_BODY* body = &pkg->QuestPckg[i];
		if (!body->questID || !MatchesTab(body->questType, tab))
			continue;

		const int slot = (int)GetGroup(&questInfoPlayer.QuestPckg[body->questID]);
		if (counts[slot] < kQuestSlotMax)
			grouped[slot][counts[slot]++] = body;
	}

	const char* headers[4] = {
		u8"Prontas para entregar",
		u8"Em andamento",
		"Dispon" U8_I "veis",
		"Conclu" U8_I "das"
	};

	for (int g = 0; g < 4; g++)
	{
		if (counts[g] <= 0)
			continue;

		char header[80];
		sprintf_s(header, "%s (%d)", headers[g], counts[g]);

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_None;
		if (g < 3)
			flags |= ImGuiTreeNodeFlags_DefaultOpen;

	if (g == 2)
	{
		const QUEST_BODY* fit[kQuestSlotMax] = {};
		const QUEST_BODY* unfit[kQuestSlotMax] = {};
		int nFit = 0, nUnfit = 0;
		for (int i = 0; i < counts[g]; i++)
		{
			if (IsInLevelRange(grouped[g][i]))
				fit[nFit++] = grouped[g][i];
			else
				unfit[nUnfit++] = grouped[g][i];
		}

		if (nFit > 0)
		{
			char fitHeader[80];
			sprintf_s(fitHeader, "Dispon" U8_I "veis (%d)", nFit);
			if (ImGui::CollapsingHeader(fitHeader, ImGuiTreeNodeFlags_DefaultOpen))
			{
				for (int i = 0; i < nFit; i++)
					DrawListItem(fit[i], &questInfoPlayer.QuestPckg[fit[i]->questID], true);
			}
		}
		if (nUnfit > 0)
		{
			char unfitHeader[80];
			sprintf_s(unfitHeader, "Fora do n" U8_I "vel (%d)", nUnfit);
			if (ImGui::CollapsingHeader(unfitHeader))
			{
				for (int i = 0; i < nUnfit; i++)
					DrawListItem(unfit[i], &questInfoPlayer.QuestPckg[unfit[i]->questID], true);
			}
		}
		continue;
	}

		if (ImGui::CollapsingHeader(header, flags))
		{
			for (int i = 0; i < counts[g]; i++)
			{
				const QUEST_BODY* body = grouped[g][i];
				DrawListItem(body, &questInfoPlayer.QuestPckg[body->questID], true);
			}
		}
	}

	ImGui::PopID();
}

void QuestWindow::DrawObjectives(const QUEST_BODY* body, const QUEST_INFO* info)
{
	if (!body)
		return;

	DrawSectionHeader("OBJETIVOS");

	const int current[3] = {
		info ? info->questProgress[0] : 0,
		info ? info->questProgress[1] : 0,
		info ? info->questProgress[2] : 0
	};

	switch (body->questObjective)
	{
	case Quest::QUEST_KILL_MONSTER:
	case Quest::QUEST_KILL_ANY_MONSTER:
		ImGui::TextDisabled(u8"Eliminar monstros");
		for (int i = 0; i < 3; i++)
		{
			if (body->monsterNum[i] <= 0)
				continue;

			std::string monster = ToUtf8(body->wantedMonster[i]);
			if (monster.empty() || monster == "0")
				monster = u8"Qualquer monstro";

			const std::string map = MapName(body->monsterMap[i]);
			DrawObjectiveRow(monster.c_str(), current[i], body->monsterNum[i], map.c_str());
		}
		break;

	case Quest::QUEST_KILL_PLAYER:
		ImGui::TextDisabled(u8"Eliminar jogadores");
		DrawObjectiveRow(u8"Jogadores", info ? info->killProgress : 0, body->playersToKill, nullptr);
		break;

	case Quest::QUEST_GET_ITEM:
		ImGui::TextDisabled(u8"Obter itens");
		for (int i = 0; i < 3; i++)
		{
			if (body->qtItens[i] <= 0)
				continue;
			const std::string item = ToUtf8(body->itemDescription[i]);
			DrawObjectiveRow(item.empty() ? u8"Item" : item.c_str(), 0, body->qtItens[i], nullptr);
		}
		break;

	default:
		ImGui::TextDisabled(u8"Sem objetivo definido.");
		break;
	}
}

void QuestWindow::DrawRewards(const QUEST_BODY* body)
{
	if (!body)
		return;

	ImGui::Spacing();
	DrawSectionHeader("RECOMPENSAS");

	bool any = false;
	if (body->rewardGold > 0)
	{
		char gold[32];
		FormatIntPt(gold, sizeof(gold), body->rewardGold);
		ImGui::Text("Ouro: %s", gold);
		any = true;
	}
	if (body->rewardExp > 0)
	{
		char exp[32];
		FormatIntPt(exp, sizeof(exp), body->rewardExp);
		ImGui::Text("Experi" U8_E "ncia: %s", exp);
		any = true;
	}

	for (int i = 0; i < 3; i++)
	{
		if (body->rewardItem[i][0] == 0 || body->rewardItem[i][0] == '0')
			continue;
		const std::string item = ToUtf8(body->rewardItem[i]);
		ImGui::Text(u8"Item: %s", item.c_str());
		any = true;
	}

	if (!any)
		ImGui::TextDisabled(u8"Sem recompensa listada.");
}

void QuestWindow::DrawDetailTitleActions(const QUEST_BODY* body, const QUEST_INFO* info)
{
	if (!body || !info)
		return;

	const std::string name = ToUtf8(body->questName);
	const bool canTrack = info->isActive || info->isReadyToComplete;
	const bool canCancel = info->isActive && !info->isReadyToComplete && !info->isComplete;
	const float gap = 6.0f;
	const float btnH = 24.0f;
	const float trackW = canTrack ? 86.0f : 0.0f;
	const float cancelW = canCancel ? 92.0f : 0.0f;
	float btnsW = trackW + cancelW;
	if (trackW > 0.0f && cancelW > 0.0f)
		btnsW += gap;

	const float rowW = ImGui::GetContentRegionAvail().x;
	const float nameW = (btnsW > 0.0f) ? (rowW - btnsW - 8.0f) : rowW;
	const ImVec2 origin = ImGui::GetCursorPos();

	ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + nameW);
	ImGui::Text("%s", name.empty() ? u8"(sem nome)" : name.c_str());
	ImGui::PopTextWrapPos();
	const float afterNameY = ImGui::GetCursorPosY();

	if (btnsW > 0.0f)
	{
		ImGui::SetCursorPos(ImVec2(origin.x + rowW - btnsW, origin.y));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));
		if (canTrack)
		{
			const bool shown = openOverlay && IsShownOnOverlay(body->questID);
			if (ImGui::Button(shown ? "Ocultar" : "Mostrar", ImVec2(trackW, btnH)))
			{
				if (shown)
				{
					SetShownOnOverlay(body->questID, false);
				}
				else
				{
					SetShownOnOverlay(body->questID, true);
					openOverlay = true;
					m_overlayMinimized = false;
				}
			}
			if (ImGui::IsItemHovered())
			{
				ImGui::SetTooltip(shown
					? "Ocultar este desafio da taskbar"
					: "Mostrar este desafio na taskbar");
			}
			if (canCancel)
				ImGui::SameLine(0.0f, gap);
		}
		if (canCancel)
		{
			if (ImGui::Button("Cancelar", ImVec2(cancelW, btnH)))
				AskCancelQuest(body->questName, body->questID, body->questType);
		}
		ImGui::PopStyleColor();
		const float afterBtnY = origin.y + btnH;
		ImGui::SetCursorPos(ImVec2(origin.x, (afterNameY > afterBtnY ? afterNameY : afterBtnY)));
	}

	ImGui::Dummy(ImVec2(0.0f, 4.0f));
}

void QuestWindow::DrawActionFooter(const QUEST_BODY* body, const QUEST_INFO* info)
{
	if (!body || !info)
		return;

	if (m_finishClickQuestId == body->questID && (info->isComplete || !info->isReadyToComplete))
		m_finishClickQuestId = 0;

	if (info->isComplete)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertU32ToFloat4(ColorForGroup(QuestGroup::Complete)));
		ImGui::Text("Conclu" U8_I "do em %02d/%02d/%04d  %02d:%02d",
			info->EndTime.wDay, info->EndTime.wMonth, info->EndTime.wYear,
			info->EndTime.wHour, info->EndTime.wMinute);
		ImGui::PopStyleColor();
		return;
	}

	if (info->isActive && !info->isReadyToComplete)
		return;

	const float avail = ImGui::GetContentRegionAvail().x;
	const ImVec2 btn(220.0f, 32.0f);
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));
	if (info->isReadyToComplete)
	{
		const bool pending = (m_finishClickQuestId == body->questID)
			&& (GetTickCount() - m_finishClickTick < 3000);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - btn.x) * 0.5f);
		if (pending)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.65f);
			ImGui::Button("Entregando...", btn);
			ImGui::PopStyleVar();
		}
		else if (ImGui::Button(u8"Concluir desafio", btn))
		{
			m_finishClickQuestId = body->questID;
			m_finishClickTick = GetTickCount();
			Quest::GetInstance()->finishQuest(body->questID, body->questType, body->questObjective);
		}
	}
	else if (!IsInLevelRange(body))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.45f);
		ImGui::Button("Fora do n" U8_I "vel", btn);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.78f, 0.55f, 0.50f, 1.0f));
		char hint[80];
		sprintf_s(hint, "Seu n" U8_I "vel %d precisa estar entre %d e %d.",
			PlayerLevel(), body->minLevel, body->maxLevel);
		const float hintW = ImGui::CalcTextSize(hint).x;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - hintW) * 0.5f);
		ImGui::TextUnformatted(hint);
		ImGui::PopStyleColor();
		return;
	}
	else
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - btn.x) * 0.5f);
		if (ImGui::Button(u8"Aceitar desafio", btn))
			Quest::GetInstance()->startNewQuest(body->questID, body->questType);
	}
	ImGui::PopStyleColor();
}

void QuestWindow::DrawQuestDetails()
{
	const QUEST_BODY* body = FindBody(m_selected);
	if (!body || !body->questID)
	{
		ImGui::TextDisabled(u8"Selecione um desafio na lista.");
		return;
	}

	const QUEST_INFO* info = &questInfoPlayer.QuestPckg[body->questID];
	const QuestGroup group = GetGroup(info);
	const std::string desc = ToUtf8(body->questDescription);

	DrawDetailTitleActions(body, info);

	char levelBuf[32];
	sprintf_s(levelBuf, "N" U8_I "vel %d-%d", body->minLevel, body->maxLevel);
	const bool inRange = IsInLevelRange(body);
	DrawBadge(GroupLabel(group), ColorForGroup(group));
	DrawBadge(TypeLabel(body->questType), kGold);
	DrawBadge(levelBuf, inRange ? IM_COL32(150, 190, 120, 220) : IM_COL32(190, 110, 110, 230));
	if (!inRange && group == QuestGroup::Available)
		DrawBadge("Fora do n" U8_I "vel", IM_COL32(190, 110, 110, 230));
	ImGui::NewLine();

	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.86f, 0.78f, 1.0f));
	ImGui::TextWrapped("%s", desc.empty() ? ("Sem descri" U8_C U8_AN "o.") : desc.c_str());
	ImGui::PopStyleColor();
	ImGui::Spacing();

	DrawObjectives(body, info);
	DrawRewards(body);
	DrawActionFooter(body, info);
}

void QuestWindow::openWindow(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(720.0f, 520.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)(smScreenWidth / 2) - 360.0f, (float)(smScreenHeight / 2) - 260.0f));

	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##DesafiosWindow", p_open, flags);
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

		if (ImGui::BeginTabBar("##QuestTypeTabs", ImGuiTabBarFlags_None))
		{
			const char* tabNames[3] = { U8_AU "nicas", "Di" U8_A "rias", "Repetitivas" };
			for (int tab = 0; tab < 3; tab++)
			{
				ImGuiTabItemFlags tabFlags = ImGuiTabItemFlags_None;
				if (m_forceSelectTab && tab == m_activeTab)
					tabFlags |= ImGuiTabItemFlags_SetSelected;

				if (ImGui::BeginTabItem(tabNames[tab], nullptr, tabFlags))
				{
					DrawActiveTabOrnament();
					m_activeTab = tab;
					EnsureSelectionForTab(tab);

					ImGui::BeginChild("##QuestList", ImVec2(240.0f, 0.0f), false);
					DrawQuestList(tab);
					ImGui::EndChild();

					ImGui::SameLine();
					DrawPlayerColumnSplit();

					ImGui::BeginChild("##QuestDetail", ImVec2(0.0f, 0.0f), false);
					DrawQuestDetails();
					ImGui::EndChild();

					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
			m_forceSelectTab = false;
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsWindowHovered())
			ImGui::CaptureMouseFromApp(true);

		ImGui::End();
	}

	DrawCancelConfirm();
	PopWindowStyle();
}

void QuestWindow::questOverlay(bool* p_open)
{
	if (!p_open || !*p_open)
		return;

	EnsureOverlayTitleTexture();

	ImGuiIO& io = ImGui::GetIO();
	// DisplaySize cai pra quase zero com a janela minimizada. Se a gente
	// reposicionar/clamp nesse momento, a taskbar vai parar em (0,0).
	const bool displayOk = io.DisplaySize.x >= 200.0f && io.DisplaySize.y >= 150.0f;
	if (!m_overlayHasPos)
		ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - 16.0f, 36.0f), ImGuiCond_Always, ImVec2(1.0f, 0.0f));
	else
		ImGui::SetNextWindowPos(ImVec2(m_overlayX, m_overlayY), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(260.0f, 0.0f), ImGuiCond_Always);

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6.0f, 5.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.78f, 0.62f, 0.28f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.06f, 0.07f, 0.09f, 0.92f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.22f, 0.18f, 0.10f, 0.55f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.32f, 0.26f, 0.12f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(0.38f, 0.30f, 0.14f, 0.90f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize
		| ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

	if (!ImGui::Begin("##QuestTaskbar", p_open, flags))
	{
		ImGui::End();
		ImGui::PopStyleColor(8);
		ImGui::PopStyleVar(4);
		return;
	}

	const float headerH = 36.0f;
	const float iconStackW = kHeaderBtnW * 2.0f + kHeaderBtnGap + 20.0f;

	if (!m_overlayHasPos)
	{
		const ImVec2 spawn = ImGui::GetWindowPos();
		m_overlayX = spawn.x;
		m_overlayY = spawn.y;
		m_overlayHasPos = true;
	}

	ImGui::SetCursorPos(ImVec2(8.0f, 4.0f));
	ImGui::InvisibleButton("##QuestTaskbarDrag", ImVec2(ImGui::GetWindowWidth() - iconStackW, headerH - 8.0f));
	if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
	{
		m_overlayX += io.MouseDelta.x;
		m_overlayY += io.MouseDelta.y;
		m_overlayHasPos = true;
	}

	DrawOverlayChrome(headerH);

	ImDrawList* draw = ImGui::GetWindowDrawList();
	const ImVec2 win = ImGui::GetWindowPos();
	if (m_overlayTitleTex && m_overlayTitleW > 0 && m_overlayTitleH > 0)
	{
		const float titleAreaW = ImGui::GetWindowWidth() - iconStackW;
		const ImVec2 sz = FitImageSize(m_overlayTitleW, m_overlayTitleH, 176.0f, 28.0f);
		const float x = win.x + 10.0f + (titleAreaW - 8.0f - sz.x) * 0.5f;
		const float y = win.y + (headerH - sz.y) * 0.5f + 2.0f;
		draw->AddImage((ImTextureID)m_overlayTitleTex, ImVec2(x, y), ImVec2(x + sz.x, y + sz.y));
	}
	else
	{
		draw->AddText(ImVec2(win.x + 12.0f, win.y + 10.0f), kGoldBright, "EM ANDAMENTO");
	}

	const float iconY = win.y + (headerH - kHeaderBtnH) * 0.5f;
	const ImVec2 minPos(win.x + ImGui::GetWindowWidth() - 8.0f - kHeaderBtnW * 2.0f - kHeaderBtnGap, iconY);
	if (DrawHeaderIconAt("##ovMin", minPos, m_overlayMinimized ? HeaderIcon_Plus : HeaderIcon_Minus))
		m_overlayMinimized = !m_overlayMinimized;
	if (DrawHeaderIconAt("##ovClose", ImVec2(minPos.x + kHeaderBtnW + kHeaderBtnGap, iconY), HeaderIcon_Close) && p_open)
		*p_open = false;

	if (!m_overlayMinimized)
	{
		ImGui::SetCursorPos(ImVec2(10.0f, headerH + 8.0f));
		ImGui::BeginGroup();

		auto& quests = Quest::GetInstance()->vQuests;
		int shown = 0;

		if (!quests.empty())
		{
			QUESTPCKG* pkg = quests[0];
			for (int i = 0; i < kQuestSlotMax; i++)
			{
				const QUEST_BODY& body = pkg->QuestPckg[i];
				if (!body.questID)
					continue;

				const QUEST_INFO& info = questInfoPlayer.QuestPckg[body.questID];
				if (!info.isActive && !info.isReadyToComplete)
					continue;
				if (!IsShownOnOverlay(body.questID))
					continue;

				const bool ready = info.isReadyToComplete != 0;
				const QuestGroup group = GetGroup(&info);
				const std::string name = ToUtf8(body.questName);
				const ImU32 nameCol = ready ? IM_COL32(92, 186, 108, 255) : ColorForGroup(group);

				ImGui::PushID(body.questID);

				ImDrawList* cardDraw = ImGui::GetWindowDrawList();
				cardDraw->ChannelsSplit(2);
				cardDraw->ChannelsSetCurrent(1);
				const ImVec2 cardMin = ImGui::GetCursorScreenPos();
				const float cardW = ImGui::GetContentRegionAvail().x;

				ImGui::BeginGroup();
				ImGui::Dummy(ImVec2(0.0f, 4.0f));
				ImGui::Indent(6.0f);
				const char* displayName = name.empty() ? u8"(sem nome)" : name.c_str();
				const ImVec2 nameMin = ImGui::GetCursorScreenPos();
				float nameW = ImGui::GetContentRegionAvail().x - 6.0f;
				if (nameW < 1.0f)
					nameW = 1.0f;
				const float nameH = ImGui::GetTextLineHeight() + 2.0f;
				ImGui::InvisibleButton("##QuestOverlayOpen", ImVec2(nameW, nameH));
				const bool nameHovered = ImGui::IsItemHovered();
				const bool nameClicked = ImGui::IsItemClicked();
				const ImVec2 nameMax(nameMin.x + nameW, nameMin.y + nameH);
				if (nameHovered)
					cardDraw->AddRectFilled(nameMin, nameMax, IM_COL32(65, 54, 29, 115), 2.0f);
				cardDraw->PushClipRect(nameMin, nameMax, true);
				cardDraw->AddText(ImVec2(nameMin.x + 2.0f, nameMin.y + 1.0f), nameCol, displayName);
				cardDraw->PopClipRect();
				if (nameClicked)
					FocusQuest(body.questID);

				int current = 0, total = 0;
				GetQuestProgress(&body, &info, &current, &total);
				if (ready && total > 0)
					current = total;

				if (ready)
				{
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.42f, 0.80f, 0.48f, 1.0f));
					ImGui::TextUnformatted(u8"Pronto para entregar");
					ImGui::PopStyleColor();
				}

				if (total > 0)
				{
					char overlay[32];
					sprintf_s(overlay, "%d/%d", current, total);
					if (ready)
						ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.28f, 0.68f, 0.36f, 1.0f));
					const float fraction = total > 0 ? ((float)current / (float)total) : 0.0f;
					DrawQuestProgressBar(fraction, overlay, 12.0f);
					if (ready)
						ImGui::PopStyleColor();
				}

				ImGui::Unindent(6.0f);
				ImGui::Dummy(ImVec2(0.0f, 4.0f));
				ImGui::EndGroup();

				const ImVec2 cardMax(cardMin.x + cardW, ImGui::GetItemRectMax().y);
				cardDraw->ChannelsSetCurrent(0);
				cardDraw->AddRectFilled(cardMin, cardMax, IM_COL32(16, 18, 22, 210), 3.0f);
				cardDraw->AddRect(cardMin, cardMax, IM_COL32(200, 170, 90, 70), 3.0f, 0, 1.0f);
				cardDraw->ChannelsMerge();

				ImGui::PopID();
				shown++;
				ImGui::Dummy(ImVec2(0.0f, 6.0f));
			}
		}

		if (shown == 0)
			ImGui::TextDisabled(u8"Nenhum desafio visível na taskbar.");

		ImGui::EndGroup();
	}

	const ImVec2 sz = ImGui::GetWindowSize();
	if (displayOk && sz.x > 1.0f && sz.y > 1.0f)
	{
		if (m_overlayX + sz.x > io.DisplaySize.x) m_overlayX = io.DisplaySize.x - sz.x;
		if (m_overlayY + sz.y > io.DisplaySize.y) m_overlayY = io.DisplaySize.y - sz.y;
		if (m_overlayX < 0.0f) m_overlayX = 0.0f;
		if (m_overlayY < 0.0f) m_overlayY = 0.0f;
		m_overlayW = sz.x;
		m_overlayH = sz.y;
	}

	if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsWindowHovered())
		ImGui::CaptureMouseFromApp(true);

	ImGui::End();
	ImGui::PopStyleColor(8);
	ImGui::PopStyleVar(4);
}
