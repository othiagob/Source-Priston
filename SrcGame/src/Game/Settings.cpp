#include "sinbaram\\sinlinkheader.h"
#include "HoBaram\\HoLinkHeader.h"
#include "imGui/imgui.h"
#include <d3dx9tex.h>
#include <math.h>
#include "HUD/ImGuiWindowChrome.h"

#define U8_A  "\xC3\xA1"
#define U8_E  "\xC3\xA9"
#define U8_I  "\xC3\xAD"
#define U8_O  "\xC3\xB3"
#define U8_U  "\xC3\xBA"
#define U8_AN "\xC3\xA3"
#define U8_ON "\xC3\xB5"
#define U8_C  "\xC3\xA7"
#define U8_A_UP  "\xC3\x81"
#define U8_E_UP  "\xC3\x89"
#define U8_U_UP  "\xC3\x9A"
#define U8_AN_UP "\xC3\x83"
#define U8_C_UP  "\xC3\x87"
#define U8_AC_UP "\xC3\x82"

extern int smScreenWidth;
extern int smScreenHeight;
extern int CameraSight;
extern int CameraInvRot;
extern int ConfigUseDynamicLights;
extern int ConfigUseDynamicShadows;
extern BOOL bShowFPS;
extern int GameMode;
extern int WinSizeX;
extern int WinSizeY;
extern int MidX;
extern int MidY;
extern float g_fWinSizeRatio_X;
extern float g_fWinSizeRatio_Y;
extern int viewdistZ;
extern HWND hwnd;
extern void StopBGM();
extern void SetVolumeBGM(DWORD dwVol);
extern int LastMusicVolume;

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\settings\\configuracoes.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const float kWindowW = 720.0f;
static const float kWindowH = 520.0f;
static const float kComboW = 158.0f;
static const float kTooltipDelay = 1.0f;
static const char* kIniPath = ".\\game.ini";

struct ResolutionOption
{
	int id;
	int ratio;
	int w;
	int h;
	const char* label;
};

static const ResolutionOption kResolutions[] =
{
	{ 0,  0,  800,  600, "800 x 600" },
	{ 1,  0, 1024,  768, "1024 x 768" },
	{ 2,  0, 1280,  960, "1280 x 960" },
	{ 3,  0, 1400, 1050, "1400 x 1050" },
	{ 4,  1, 1280, 1024, "1280 x 1024" },
	{ 5,  2, 1280,  720, "1280 x 720" },
	{ 6,  2, 1366,  768, "1366 x 768" },
	{ 7,  2, 1600,  900, "1600 x 900" },
	{ 8,  2, 1920, 1080, "1920 x 1080" },
	{ 9,  3, 1280,  800, "1280 x 800" },
	{ 10, 3, 1440,  900, "1440 x 900" },
	{ 11, 3, 1680, 1050, "1680 x 1050" },
	{ 12, 3, 1920, 1200, "1920 x 1200" },
};
static const int kResolutionCount = (int)(sizeof(kResolutions) / sizeof(kResolutions[0]));
static const char* kRatioNames[] = { "4:3", "5:4", "16:9", "16:10" };

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

static void CenterTextUnformatted(const char* text)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
	ImGui::TextUnformatted(text);
}

static ImVec2 s_tipPos = ImVec2(-9999.0f, -9999.0f);
static float s_tipStart = 0.0f;
static bool s_tipHover = false;

static void ResetOptionHelpFrame()
{
	if (!s_tipHover)
		s_tipPos = ImVec2(-9999.0f, -9999.0f);
	s_tipHover = false;
}

static void OptionHelp(const char* desc)
{
	if (!desc || !desc[0])
		return;
	if (!ImGui::IsItemHovered())
		return;

	s_tipHover = true;
	const ImVec2 pos = ImGui::GetItemRectMin();
	const float now = (float)ImGui::GetTime();
	if (fabsf(pos.x - s_tipPos.x) > 0.5f || fabsf(pos.y - s_tipPos.y) > 0.5f)
	{
		s_tipPos = pos;
		s_tipStart = now;
	}

	if ((now - s_tipStart) < kTooltipDelay)
		return;

	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.07f, 0.08f, 0.10f, 0.97f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.90f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.94f, 0.88f, 1.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.6f);
	ImGui::BeginTooltip();
	ImGui::PushTextWrapPos(280.0f);
	ImGui::TextUnformatted(desc);
	ImGui::PopTextWrapPos();
	ImGui::EndTooltip();
	ImGui::PopStyleVar(3);
	ImGui::PopStyleColor(3);
}

static bool CheckFlag(const char* label, int* value, const char* tip = nullptr)
{
	bool v = (*value != 0);
	const bool changed = ImGui::Checkbox(label, &v);
	if (changed)
		*value = v ? 1 : 0;
	OptionHelp(tip);
	return changed;
}

static bool LabeledBeginCombo(const char* id, const char* label, const char* preview, const char* tip)
{
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(label);
	ImGui::SameLine(0.0f, 8.0f);
	ImGui::SetNextItemWidth(kComboW);
	const bool open = ImGui::BeginCombo(id, preview);
	OptionHelp(tip);
	return open;
}

static void DrawHintBox(const char* text)
{
	if (!text || !text[0])
		return;

	const float padX = 12.0f;
	const float padY = 8.0f;
	float wrapW = ImGui::GetContentRegionAvail().x - padX * 2.0f;
	if (wrapW > 248.0f)
		wrapW = 248.0f;
	if (wrapW < 80.0f)
		wrapW = 80.0f;

	ImFont* font = ImGui::GetFont();
	const float fontSize = ImGui::GetFontSize();
	const float scale = fontSize / font->FontSize;
	const float lineH = ImGui::GetTextLineHeight();
	const char* textEnd = text + strlen(text);

	const char* lineBeg[6];
	const char* lineEnd[6];
	float lineW[6];
	int nLines = 0;
	float maxLineW = 0.0f;
	const char* s = text;
	while (s < textEnd && nLines < 6)
	{
		while (s < textEnd && (*s == ' ' || *s == '\n'))
			s++;
		if (s >= textEnd)
			break;

		const char* wrap = font->CalcWordWrapPositionA(scale, s, textEnd, wrapW);
		if (wrap <= s)
			wrap = s + 1;

		lineBeg[nLines] = s;
		lineEnd[nLines] = wrap;
		lineW[nLines] = font->CalcTextSizeA(fontSize, 9999.0f, 0.0f, s, wrap).x;
		if (lineW[nLines] > maxLineW)
			maxLineW = lineW[nLines];
		nLines++;
		s = wrap;
	}
	if (nLines <= 0)
		return;

	const float boxW = maxLineW + padX * 2.0f;
	const float boxH = (float)nLines * lineH + padY * 2.0f;

	const ImVec2 prevMin = ImGui::GetItemRectMin();
	const ImVec2 prevMax = ImGui::GetItemRectMax();
	const float prevH = prevMax.y - prevMin.y;
	ImVec2 p = ImGui::GetCursorScreenPos();
	if (prevH > boxH + 1.0f)
		p.y += (prevH - boxH) * 0.5f;

	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(p, ImVec2(p.x + boxW, p.y + boxH), IM_COL32(24, 22, 14, 248), 3.0f);
	draw->AddRect(p, ImVec2(p.x + boxW, p.y + boxH), kGold, 3.0f, 0, 1.4f);

	const ImU32 col = IM_COL32(230, 209, 133, 255);
	float y = p.y + padY;
	for (int i = 0; i < nLines; i++)
	{
		const float x = p.x + (boxW - lineW[i]) * 0.5f;
		draw->AddText(ImVec2(x, y), col, lineBeg[i], lineEnd[i]);
		y += lineH;
	}

	ImGui::Dummy(ImVec2(boxW, (prevH > boxH) ? prevH : boxH));
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
	ImGui::InvisibleButton("##SettingsClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
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

static const ResolutionOption* FindResolution(int id)
{
	for (int i = 0; i < kResolutionCount; i++)
	{
		if (kResolutions[i].id == id)
			return &kResolutions[i];
	}
	return &kResolutions[1];
}

static int FindResolutionId(int w, int h)
{
	for (int i = 0; i < kResolutionCount; i++)
	{
		if (kResolutions[i].w == w && kResolutions[i].h == h)
			return kResolutions[i].id;
	}
	return 1;
}

static int ClampInt(int v, int lo, int hi)
{
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

static BOOL ReadBoolOrOn(IniFiles& ini, const char* section, const char* key)
{
	return ini.ReadBool(section, key) || ini.ReadOnOff(section, key);
}

bool Settings::MouseAction(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x && pRealCursorPos.x < x + w && pRealCursorPos.y > y && pRealCursorPos.y < y + h)
		return TRUE;
	return FALSE;
}

void Settings::Init()
{
}

void Settings::CopyEditFromCommitted()
{
	bWindowed = cWindowed;
	bAutoAdjust = cAutoAdjust;
	bBorderless = cBorderless;
	bRatio = cRatio;
	bResolution = cResolution;
	bTexture = cTexture;
	bBPP = cBPP;
	bEffects = cEffects;
	bLights = cLights;
	bShadows = cShadows;
	vSync = cvSync;
	vBlockUI = cvBlockUI;
	bMusic = cMusic;
	bMVol = cMVol;
	bSound = cSound;
	bAmbient = cAmbient;
	bSVol = cSVol;
	bCamView = cCamView;
	bCamRange = cCamRange;
	bCamShake = cCamShake;
	bCamInv = cCamInv;
	bFilter = cFilter;
	bFilterSpec = cFilterSpec;
	bHP = cHP;
	bMP = cMP;
	bSP = cSP;
	bGold = cGold;
	bAmulets = cAmulets;
	bRings = cRings;
	bSheltoms = cSheltoms;
	bForce = cForce;
	bPremiums = cPremiums;
	bCrystal = cCrystal;
	bDefItem = cDefItem;
	bOffItem = cOffItem;
	bElse = cElse;
	bMS = cMS;
	bFS = cFS;
	bPS = cPS;
	bAS = cAS;
	bKS = cKS;
	bATS = cATS;
	bPRS = cPRS;
	bMGS = cMGS;
	bNaked = cNaked;
	bRememberLogin = cRememberLogin;
	bHidePlayerNames = cHidePlayerNames;
	bShowLife = cShowLife;
	bShowNotice = cShowNotice;
	bShowFPS = cShowFPS;
	bWeather = cWeather;
	bShowDamage = cShowDamage;
}

void Settings::CopyCommittedFromEdit()
{
	cWindowed = bWindowed;
	cAutoAdjust = bAutoAdjust;
	cBorderless = 0;
	cRatio = bRatio;
	cResolution = bResolution;
	cTexture = bTexture;
	cBPP = 32;
	cEffects = bEffects;
	cLights = bLights;
	cShadows = bShadows;
	cvSync = vSync;
	cvBlockUI = vBlockUI;
	cMusic = bMusic;
	cMVol = bMVol;
	cSound = bSound;
	cAmbient = bAmbient;
	cSVol = bSVol;
	cCamView = bCamView;
	cCamRange = bCamRange;
	cCamShake = bCamShake;
	cCamInv = bCamInv;
	cFilter = bFilter;
	cFilterSpec = bFilterSpec;
	cHP = bHP;
	cMP = bMP;
	cSP = bSP;
	cGold = bGold;
	cAmulets = bAmulets;
	cRings = bRings;
	cSheltoms = bSheltoms;
	cForce = bForce;
	cPremiums = bPremiums;
	cCrystal = bCrystal;
	cDefItem = bDefItem;
	cOffItem = bOffItem;
	cElse = bElse;
	cMS = bMS;
	cFS = bFS;
	cPS = bPS;
	cAS = bAS;
	cKS = bKS;
	cATS = bATS;
	cPRS = bPRS;
	cMGS = bMGS;
	cNaked = bNaked;
	cRememberLogin = bRememberLogin;
	cHidePlayerNames = bHidePlayerNames;
	cShowLife = bShowLife;
	cShowNotice = bShowNotice;
	cShowFPS = bShowFPS;
	cWeather = bWeather;
	cShowDamage = bShowDamage;
	smConfig.showDamage = cShowDamage != 0;
}

void Settings::Open()
{
	if (!cOpen)
		CopyEditFromCommitted();
	cOpen = true;
	m_confirmReset = false;
}

void Settings::Close()
{
	CopyEditFromCommitted();
	ApplyRuntime(false);
	cOpen = false;
	m_confirmReset = false;
}

bool Settings::IsBlockingMouse(int x, int y) const
{
	if (!cOpen)
		return false;
	if (m_confirmReset)
		return true;
	return x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH);
}

void Settings::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;
	LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH);
}

void Settings::PushWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 12.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 7.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.6f);
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
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ImVec4(0.90f, 0.78f, 0.40f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4(0.78f, 0.67f, 0.35f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4(0.90f, 0.78f, 0.40f, 1.0f));
}

void Settings::PopWindowStyle()
{
	ImGui::PopStyleColor(25);
	ImGui::PopStyleVar(10);
}

void Settings::DrawWindowChrome(float headerH)
{
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void Settings::DrawTitleHeader()
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
		const char* title = "CONFIGURA" U8_C U8_ON "ES";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + 18.0f), kGoldBright, title);
	}

	if (DrawHeaderClose())
		Close();
}

void Settings::DrawSectionHeader(const char* title)
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

void Settings::DrawVideoTab()
{
	DrawSectionHeader("TELA");

	ImGui::BeginGroup();
	if (ImGui::RadioButton("Tela cheia", bWindowed == 0))
		bWindowed = 0;
	OptionHelp("O jogo ocupa o monitor inteiro. Salvar aplica na hora.");
	ImGui::SameLine();
	if (ImGui::RadioButton("Janela", bWindowed != 0))
		bWindowed = 1;
	OptionHelp("O jogo fica numa janela. Voc" U8_E " pode maximizar sem perder o tamanho ao salvar outras op" U8_C U8_ON "es.");
	CheckFlag("Ajuste autom" U8_A "tico ao maximizar", &bAutoAdjust,
		"Quando voc" U8_E " maximiza a janela, o jogo acompanha o tamanho do monitor.");
	ImGui::EndGroup();

	ImGui::SameLine(0.0f, 16.0f);
	DrawHintBox("Passe o mouse sobre uma op" U8_C U8_AN "o e espere 1 segundo para ver o que ela faz.");

	DrawSectionHeader("RESOLU" U8_C_UP U8_AN_UP "O");

	ImGui::BeginGroup();
	if (LabeledBeginCombo("##SettingsRatio", "Propor" U8_C U8_AN "o", kRatioNames[ClampInt(bRatio, 0, 3)],
		"Mostra s" U8_O " os tamanhos daquela propor" U8_C U8_AN "o (4:3, 16:9...)."))
	{
		for (int i = 0; i < 4; i++)
		{
			const bool selected = (bRatio == i);
			if (ImGui::Selectable(kRatioNames[i], selected))
			{
				bRatio = i;
				const ResolutionOption* cur = FindResolution(bResolution);
				if (cur->ratio != bRatio)
				{
					for (int r = 0; r < kResolutionCount; r++)
					{
						if (kResolutions[r].ratio == bRatio)
						{
							bResolution = kResolutions[r].id;
							break;
						}
					}
				}
			}
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::EndGroup();

	ImGui::SameLine(0.0f, 28.0f);

	ImGui::BeginGroup();
	const ResolutionOption* selectedRes = FindResolution(bResolution);
	if (LabeledBeginCombo("##SettingsSize", "Tamanho", selectedRes->label,
		"Tamanho da janela ao restaurar. Maximizar n" U8_AN "o altera este valor."))
	{
		for (int i = 0; i < kResolutionCount; i++)
		{
			if (kResolutions[i].ratio != bRatio)
				continue;
			const bool selected = (bResolution == kResolutions[i].id);
			if (ImGui::Selectable(kResolutions[i].label, selected))
				bResolution = kResolutions[i].id;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::EndGroup();
	ImGui::NewLine();

	DrawSectionHeader("GR" U8_A_UP "FICOS");

	const char* texNames[] = { "Alto", "M" U8_E "dio", "Baixo", "Baixo +" };
	const int tex = ClampInt(bTexture, 0, 3);
	if (LabeledBeginCombo("##SettingsTex", "Textura", texNames[tex],
		"Qualidade das texturas. Valores baixos deixam o jogo mais leve."))
	{
		for (int i = 0; i < 4; i++)
		{
			const bool selected = (bTexture == i);
			if (ImGui::Selectable(texNames[i], selected))
				bTexture = i;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Columns(2, "##gfxcols", false);
	CheckFlag("VSync", &vSync, "Sincroniza com o monitor. Reduz o tearing e pode limitar o FPS.");
	CheckFlag("Luzes din" U8_AN "micas", &bLights, "Luzes extras no cen" U8_A "rio. Pode reduzir o FPS.");
	ImGui::NextColumn();
	CheckFlag("Sombras din" U8_AN "micas", &bShadows, "Sombras extras. Pode reduzir o FPS.");
	CheckFlag("Mostrar n" U8_U "meros de dano", &bShowDamage, "Mostra os n" U8_U "meros de dano durante o combate.");
	ImGui::Columns(1);
}

void Settings::DrawAudioTab()
{
	DrawSectionHeader("M" U8_U_UP "SICA");
	CheckFlag("M" U8_U "sica ligada", &bMusic, "Liga ou desliga a trilha do mapa. Vale na hora ao salvar.");
	int mvol = ClampInt(bMVol, 0, 8);
	ImGui::SetNextItemWidth(220.0f);
	if (ImGui::SliderInt("Volume da m" U8_U "sica", &mvol, 0, 8))
		bMVol = mvol;
	OptionHelp("Volume da trilha. Entra no pr" U8_O "ximo tema do mapa.");

	DrawSectionHeader("EFEITOS SONOROS");
	CheckFlag("Som ligado", &bSound, "Liga ou desliga os efeitos (golpe, drop, UI).");
	CheckFlag("Sons de ambiente", &bAmbient, "Sons de fundo do mapa (vento, cidade, etc.).");
	int svol = ClampInt(bSVol, 0, 8);
	ImGui::SetNextItemWidth(220.0f);
	if (ImGui::SliderInt("Volume dos efeitos", &svol, 0, 8))
		bSVol = svol;
	OptionHelp("Volume dos efeitos sonoros.");
}

void Settings::DrawCameraTab()
{
	DrawSectionHeader("C" U8_AC_UP "MERA");

	const char* viewNames[] = { "Perto", "Longe", "M" U8_A "ximo" };
	const int view = ClampInt(bCamView, 0, 2);
	if (LabeledBeginCombo("##SettingsCamView", "Vis" U8_AN "o", viewNames[view],
		"Dist" U8_AN "ncia padr" U8_AN "o da c" U8_AN "mera em rela" U8_C U8_AN "o ao personagem."))
	{
		for (int i = 0; i < 3; i++)
		{
			const bool selected = (bCamView == i);
			if (ImGui::Selectable(viewNames[i], selected))
				bCamView = i;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine(0.0f, 28.0f);

	const char* zoomNames[] = { "Perto", "Regular", "Longe", "M" U8_A "ximo" };
	const int zoom = ClampInt(bCamRange, 0, 3);
	if (LabeledBeginCombo("##SettingsCamZoom", "Zoom", zoomNames[zoom],
		"At" U8_E " onde voc" U8_E " pode afastar a c" U8_AN "mera com a roda do mouse."))
	{
		for (int i = 0; i < 4; i++)
		{
			const bool selected = (bCamRange == i);
			if (ImGui::Selectable(zoomNames[i], selected))
				bCamRange = i;
			if (selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::NewLine();

	CheckFlag("Vis" U8_AN "o longa (n" U8_E "voa distante)", &bCamShake,
		"Empurra o corte da n" U8_E "voa para longe. N" U8_AN "o atravessa paredes.");
	CheckFlag("Inverter c" U8_AN "mera", &bCamInv,
		"Inverte o movimento horizontal da c" U8_AN "mera.");
}

void Settings::DrawInterfaceTab()
{
	DrawSectionHeader("TELA");
	if (CheckFlag("Mostrar FPS", &bShowFPS, "Mostra os quadros por segundo no canto da tela."))
		::bShowFPS = bShowFPS ? TRUE : FALSE;
	CheckFlag("Ocultar nomes de jogadores", &bHidePlayerNames,
		"Esconde os nomes de outros jogadores. O seu continua vis" U8_I "vel.");
	CheckFlag("Barra de HP sobre o personagem", &bShowLife,
		"Mostra uma barra de vida acima do personagem.");
	if (CheckFlag("Mostrar alertas no chat", &bShowNotice,
		"Liga os avisos do servidor na janela de chat."))
	{
		if (GAMECOREHANDLE && GAMECOREHANDLE->IsInit() && CHATBOX)
			CHATBOX->ToggleNotice(bShowNotice ? TRUE : FALSE);
	}
	CheckFlag("Permitir mover janelas do HUD", &vBlockUI,
		"Permite arrastar janelas antigas do HUD (invent" U8_A "rio, skills...).");

	DrawSectionHeader("CONTA");
	CheckFlag("Lembrar conta no login", &bRememberLogin,
		"Guarda o login na tela inicial. N" U8_AN "o grava a senha.");

	DrawCameraTab();
}

void Settings::DrawFilterTab()
{
	DrawSectionHeader("FILTRO DE LOOT");
	CheckFlag("Ativar filtro (esconde no ch" U8_AN "o)", &bFilter,
		"Esconde no ch" U8_AN "o os itens desmarcados. S" U8_O " na sua tela: o servidor continua dropando.");

	if (ImGui::Button("Marcar itens"))
	{
		bHP = bMP = bSP = bGold = bAmulets = bRings = bSheltoms = 1;
		bForce = bPremiums = bCrystal = bDefItem = bOffItem = bElse = 1;
	}
	OptionHelp("Marca todos os tipos de item do filtro.");
	ImGui::SameLine();
	if (ImGui::Button("Limpar itens"))
	{
		bHP = bMP = bSP = bGold = bAmulets = bRings = bSheltoms = 0;
		bForce = bPremiums = bCrystal = bDefItem = bOffItem = bElse = 0;
	}
	OptionHelp("Desmarca todos os tipos de item do filtro.");

	const char* lootTip = "Item marcado continua vis" U8_I "vel no ch" U8_AN "o. Desmarcado some s" U8_O " para voc" U8_E ".";
	ImGui::Columns(2, "##lootcols", false);
	CheckFlag("Po" U8_C U8_AN "o de HP", &bHP, lootTip);
	CheckFlag("Po" U8_C U8_AN "o de MP", &bMP, lootTip);
	CheckFlag("Po" U8_C U8_AN "o de SP", &bSP, lootTip);
	CheckFlag("Ouro", &bGold, lootTip);
	CheckFlag("Amuletos", &bAmulets, lootTip);
	CheckFlag("An" U8_E "is", &bRings, lootTip);
	ImGui::NextColumn();
	CheckFlag("Sheltoms", &bSheltoms, lootTip);
	CheckFlag("Force Orbs", &bForce, lootTip);
	CheckFlag("Premiums", &bPremiums, lootTip);
	CheckFlag("Cristais", &bCrystal, lootTip);
	CheckFlag("Itens de defesa", &bDefItem, lootTip);
	CheckFlag("Itens de ataque", &bOffItem, lootTip);
	CheckFlag("Restante", &bElse, lootTip);
	ImGui::Columns(1);

	DrawSectionHeader("FILTRO POR CLASSE");
	CheckFlag("S" U8_O " mostrar da classe marcada", &bFilterSpec,
		"Quando ligado, esconde loot que n" U8_AN "o serve para as classes marcadas.");
	if (ImGui::Button("Marcar classes"))
		bMS = bFS = bPS = bAS = bKS = bATS = bPRS = bMGS = bNaked = 1;
	OptionHelp("Marca todas as classes.");
	ImGui::SameLine();
	if (ImGui::Button("Limpar classes"))
		bMS = bFS = bPS = bAS = bKS = bATS = bPRS = bMGS = bNaked = 0;
	OptionHelp("Desmarca todas as classes.");

	const char* classTip = "Classe marcada continua vendo o loot dela no ch" U8_AN "o.";
	ImGui::Columns(2, "##classcols", false);
	CheckFlag("Lutador", &bFS, classTip);
	CheckFlag("Mec" U8_AN "nico", &bMS, classTip);
	CheckFlag("Arqueira", &bAS, classTip);
	CheckFlag("Pikeman", &bPS, classTip);
	CheckFlag("Atalanta", &bATS, classTip);
	ImGui::NextColumn();
	CheckFlag("Cavaleiro", &bKS, classTip);
	CheckFlag("Mago", &bMGS, classTip);
	CheckFlag("Sacerdotisa", &bPRS, classTip);
	CheckFlag("Sem classe", &bNaked, classTip);
	ImGui::Columns(1);

	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.62f, 0.62f, 0.68f, 1.0f));
	ImGui::TextWrapped("S" U8_O " esconde o item na sua tela. O servidor continua dropando normalmente.");
	ImGui::PopStyleColor();
}

void Settings::DrawFooter()
{
	const float btnW = 128.0f;
	const float gap = 12.0f;
	const float total = btnW * 3.0f + gap * 2.0f;
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - total) * 0.5f);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 6.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.28f, 0.10f, 0.10f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.42f, 0.14f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.16f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.42f, 0.34f, 0.95f));
	if (ImGui::Button("Restaurar", ImVec2(btnW, 30.0f)))
		m_confirmReset = true;
	ImGui::PopStyleColor(4);

	ImGui::SameLine(0.0f, gap);
	if (ImGui::Button("Cancelar", ImVec2(btnW, 30.0f)))
		Close();

	ImGui::SameLine(0.0f, gap);
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
	if (ImGui::Button("Salvar", ImVec2(btnW, 30.0f)))
		Save(true);
	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar();
}

void Settings::DrawResetConfirm()
{
	if (!m_confirmReset)
		return;

	ImGui::OpenPopup("##SettingsResetConfirm");
	ImGui::SetNextWindowSize(ImVec2(332.0f, 176.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##SettingsResetConfirm", nullptr, flags))
	{
		DrawWindowChrome(34.0f);

		ImGui::SetCursorPosY(11.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted("Restaurar padr" U8_AN "o?");
		ImGui::PopStyleColor();

		ImGui::SetCursorPosY(56.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted("Os campos voltam ao padr" U8_AN "o. Salve depois para gravar.");
		ImGui::PopStyleColor();

		const float btnW = 128.0f;
		const float gap = 14.0f;
		ImGui::SetCursorPos(ImVec2((332.0f - (btnW * 2.0f + gap)) * 0.5f, 128.0f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_confirmReset = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);

		ImGui::SameLine(0.0f, gap);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.28f, 0.10f, 0.10f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.42f, 0.14f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.50f, 0.16f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.42f, 0.34f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.86f, 0.82f, 1.0f));
		if (ImGui::Button("CONFIRMAR", ImVec2(btnW, 30.0f)))
		{
			FillEditDefaults();
			m_confirmReset = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);

		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void Settings::Draw()
{
	if (!cOpen)
		return;
	if (!ImGui::GetCurrentContext())
		return;

	ResetOptionHelpFrame();

	ImGui::SetNextWindowSize(ImVec2(kWindowW, kWindowH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(((float)smScreenWidth - kWindowW) * 0.5f, ((float)smScreenHeight - kWindowH) * 0.5f));

	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	bool open = true;
	ImGui::Begin("##SettingsWindow", &open, flags);
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

		if (ImGui::BeginTabBar("##SettingsTabs", ImGuiTabBarFlags_None))
		{
			const float childH = -56.0f;
			if (ImGui::BeginTabItem("V" U8_I "deo"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 0;
				ImGui::BeginChild("##SettingsVideo", ImVec2(0.0f, childH), false);
				DrawVideoTab();
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("A" U8_U "dio"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 1;
				ImGui::BeginChild("##SettingsAudio", ImVec2(0.0f, childH), false);
				DrawAudioTab();
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Interface"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 2;
				ImGui::BeginChild("##SettingsInterface", ImVec2(0.0f, childH), false);
				DrawInterfaceTab();
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Filtros"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 3;
				ImGui::BeginChild("##SettingsFilter", ImVec2(0.0f, childH), false);
				DrawFilterTab();
				ImGui::EndChild();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::SetCursorPos(ImVec2(kPlayerWindowBodyInset, kWindowH - 48.0f));
		DrawFooter();

		ImGui::End();
	}

	if (!open)
		Close();

	DrawResetConfirm();
	PopWindowStyle();
}

void Settings::Button()
{
}

void Settings::ResolveResolution()
{
	bResolution = FindResolutionId(smConfig.ScreenSize.x, smConfig.ScreenSize.y);
	cResolution = bResolution;
	const ResolutionOption* res = FindResolution(bResolution);
	bRatio = res->ratio;
	cRatio = bRatio;
}

void Settings::FillEditDefaults()
{
	bWindowed = 1;
	bAutoAdjust = 1;
	bBorderless = 0;
	bRatio = 2;
	bResolution = 8;
	bTexture = 0;
	bBPP = 32;
	bEffects = 1;
	bShowDamage = 1;
	bLights = 0;
	bShadows = 1;
	vSync = 1;
	vBlockUI = 1;
	bMusic = 1;
	bMVol = 4;
	bSound = 1;
	bAmbient = 1;
	bSVol = 4;
	bCamView = 1;
	bCamRange = 2;
	bCamShake = 1;
	bCamInv = 0;
	bFilter = 0;
	bFilterSpec = 0;
	bHP = 1;
	bMP = 1;
	bSP = 1;
	bGold = 1;
	bAmulets = 1;
	bRings = 1;
	bSheltoms = 1;
	bForce = 1;
	bPremiums = 1;
	bCrystal = 1;
	bDefItem = 1;
	bOffItem = 1;
	bElse = 1;
	bMS = 1;
	bFS = 1;
	bPS = 1;
	bAS = 1;
	bKS = 1;
	bATS = 1;
	bPRS = 1;
	bMGS = 1;
	bNaked = 1;
	bRememberLogin = 1;
	bHidePlayerNames = 0;
	bShowLife = 0;
	bShowNotice = 1;
	bShowFPS = 0;
}

void Settings::Load()
{
	IniFiles ini(kIniPath);

	cWindowed = ini.ReadBool("Screen", "Windowed") ? 1 : 0;
	cAutoAdjust = ini.ReadBool("Screen", "AutoAdjust") ? 1 : 0;
	cBorderless = 0;
	smConfig.WinMode = cWindowed != 0;

	char ratio[32] = { 0 };
	ini.ReadStringTo("Screen", "Ratio", ratio, sizeof(ratio), "");
	cRatio = 2;
	if (lstrcmpiA(ratio, "4:3") == 0) cRatio = 0;
	else if (lstrcmpiA(ratio, "5:4") == 0) cRatio = 1;
	else if (lstrcmpiA(ratio, "16:9") == 0) cRatio = 2;
	else if (lstrcmpiA(ratio, "16:10") == 0) cRatio = 3;

	int w = ini.ReadInt("Screen", "Width");
	int h = ini.ReadInt("Screen", "Height");
	if (w < 640) w = 1024;
	if (h < 480) h = 768;
	smConfig.ScreenSize.x = w;
	smConfig.ScreenSize.y = h;

	char texKey[32] = { 0 };
	ini.ReadStringTo("Graphics", "TextureQuality", texKey, sizeof(texKey), "");
	if (texKey[0])
		cTexture = ClampInt(ini.ReadInt("Graphics", "TextureQuality"), 0, 3);
	else
		cTexture = ini.ReadBool("Graphics", "HighTextureQuality") ? 0 : 3;
	smConfig.TextureQuality = cTexture;

	cBPP = ini.ReadInt("Graphics", "BitDepth");
	if (cBPP != 16 && cBPP != 32)
		cBPP = 32;
	smConfig.ScreenColorBit = cBPP;

	cShowDamage = ini.ReadBool("Graphics", "Damage") ? 1 : 0;
	smConfig.showDamage = cShowDamage != 0;
	cEffects = ini.ReadBool("Graphics", "Effects") ? 1 : 0;
	cLights = ini.ReadBool("Graphics", "DynamicLights") ? 1 : 0;
	cShadows = ini.ReadBool("Graphics", "DynamicShadows") ? 1 : 0;
	cvSync = ini.ReadBool("Graphics", "VSync") ? 1 : 0;
	cvBlockUI = ini.ReadBool("Graphics", "BlockUI") ? 1 : 0;

	char weather[16] = { 0 };
	ini.ReadStringTo("Graphics", "Weather", weather, sizeof(weather), "True");
	cWeather = (lstrcmpiA(weather, "True") == 0) ? 1 : 0;
	smConfig.WeatherSwitch = cWeather;

	char audioFlag[16] = { 0 };
	ini.ReadStringTo("Audio", "Music", audioFlag, sizeof(audioFlag), "True");
	cMusic = (lstrcmpiA(audioFlag, "True") == 0) ? 1 : 0;
	ini.ReadStringTo("Audio", "Sound", audioFlag, sizeof(audioFlag), "True");
	cSound = (lstrcmpiA(audioFlag, "True") == 0) ? 1 : 0;
	ini.ReadStringTo("Audio", "Ambient", audioFlag, sizeof(audioFlag), "True");
	cAmbient = (lstrcmpiA(audioFlag, "True") == 0) ? 1 : 0;

	char volKey[16] = { 0 };
	ini.ReadStringTo("Audio", "MusicVolume", volKey, sizeof(volKey), "");
	cMVol = volKey[0] ? ClampInt(ini.ReadInt("Audio", "MusicVolume"), 0, 8) : 7;
	ini.ReadStringTo("Audio", "SoundVolume", volKey, sizeof(volKey), "");
	cSVol = volKey[0] ? ClampInt(ini.ReadInt("Audio", "SoundVolume"), 0, 8) : 7;
	LastMusicVolume = cMVol * 50;

	cCamShake = ReadBoolOrOn(ini, "Camera", "FarCameraSight") ? 1 : 0;
	cCamInv = ReadBoolOrOn(ini, "Camera", "InvertedCamera") ? 1 : 0;
	CameraSight = cCamShake;
	CameraInvRot = cCamInv;
	smConfig.CameraSight = cCamShake;
	smConfig.CameraInvRot = cCamInv != 0;

	char camKey[16] = { 0 };
	ini.ReadStringTo("Camera", "View", camKey, sizeof(camKey), "");
	cCamView = camKey[0] ? ClampInt(ini.ReadInt("Camera", "View"), 0, 2) : 1;
	ini.ReadStringTo("Camera", "Range", camKey, sizeof(camKey), "");
	cCamRange = camKey[0] ? ClampInt(ini.ReadInt("Camera", "Range"), 0, 3) : 2;

	cHP = ini.ReadBool("LootFilter", "HP") ? 1 : 0;
	cMP = ini.ReadBool("LootFilter", "MP") ? 1 : 0;
	cSP = ini.ReadBool("LootFilter", "SP") ? 1 : 0;
	cGold = ini.ReadBool("LootFilter", "Gold") ? 1 : 0;
	cAmulets = ini.ReadBool("LootFilter", "Amulets") ? 1 : 0;
	cRings = ini.ReadBool("LootFilter", "Rings") ? 1 : 0;
	cSheltoms = ini.ReadBool("LootFilter", "Sheltoms") ? 1 : 0;
	cForce = ini.ReadBool("LootFilter", "Force") ? 1 : 0;
	cPremiums = ini.ReadBool("LootFilter", "Premiums") ? 1 : 0;
	cCrystal = ini.ReadBool("LootFilter", "Crystal") ? 1 : 0;
	cDefItem = ini.ReadBool("LootFilter", "DefItem") ? 1 : 0;
	cOffItem = ini.ReadBool("LootFilter", "OffItem") ? 1 : 0;
	cElse = ini.ReadBool("LootFilter", "Else") ? 1 : 0;
	cFilter = ini.ReadBool("LootFilter", "Enabled") ? 1 : 0;

	cMS = ini.ReadBool("LootSpecFilter", "MS") ? 1 : 0;
	cFS = ini.ReadBool("LootSpecFilter", "FS") ? 1 : 0;
	cPS = ini.ReadBool("LootSpecFilter", "PS") ? 1 : 0;
	cAS = ini.ReadBool("LootSpecFilter", "AS") ? 1 : 0;
	cKS = ini.ReadBool("LootSpecFilter", "KS") ? 1 : 0;
	cATS = ini.ReadBool("LootSpecFilter", "ATS") ? 1 : 0;
	cPRS = ini.ReadBool("LootSpecFilter", "PRS") ? 1 : 0;
	cMGS = ini.ReadBool("LootSpecFilter", "MGS") ? 1 : 0;
	cNaked = ini.ReadBool("LootSpecFilter", "Naked") ? 1 : 0;
	cFilterSpec = ini.ReadBool("LootSpecFilter", "Enabled") ? 1 : 0;

	cRememberLogin = ini.ReadBool("Game", "RememberAccount") ? 1 : 0;

	char hideNames[16] = { 0 };
	ini.ReadStringTo("Game", "HideNames", hideNames, sizeof(hideNames), "");
	if (hideNames[0])
		cHidePlayerNames = ini.ReadBool("Game", "HideNames") ? 1 : 0;
	else
		cHidePlayerNames = ini.ReadBool("Game", "ShowNames") ? 1 : 0;

	cShowLife = ini.ReadBool("Game", "ShowLife") ? 1 : 0;
	cShowNotice = ini.ReadBool("Game", "ShowAlert") ? 1 : 0;
	cShowFPS = ini.ReadBool("Game", "ShowFPS") ? 1 : 0;
	::bShowFPS = cShowFPS ? TRUE : FALSE;

	ConfigUseDynamicLights = cLights;
	ConfigUseDynamicShadows = cShadows;
	vSync = cvSync;

	CopyEditFromCommitted();
	ResolveResolution();
	CopyCommittedFromEdit();
	CopyEditFromCommitted();

	if (smConfig.szServerIP[0])
		lstrcpy(smConfig.szDataServerIP, smConfig.szServerIP);
}

void Settings::ApplyRuntime(bool applyDisplay)
{
	cBorderless = 0;
	bBorderless = 0;
	smConfig.WinMode = cWindowed != 0;
	smConfig.TextureQuality = cTexture;
	smConfig.ScreenColorBit = 32;
	smConfig.showDamage = cShowDamage != 0;
	smConfig.WeatherSwitch = cWeather;
	CameraSight = cCamShake;
	CameraInvRot = cCamInv;
	smConfig.CameraSight = cCamShake;
	smConfig.CameraInvRot = cCamInv != 0;
	ConfigUseDynamicLights = cLights;
	ConfigUseDynamicShadows = cShadows;
	::bShowFPS = cShowFPS ? TRUE : FALSE;
	bRememberLogin = cRememberLogin;
	bHidePlayerNames = cHidePlayerNames;
	bShowLife = cShowLife;
	bShowNotice = cShowNotice;

	if (applyDisplay)
	{
		const ResolutionOption* res = FindResolution(cResolution);
		smConfig.ScreenSize.x = res->w;
		smConfig.ScreenSize.y = res->h;
		GRAPHICENGINE->SetVSync(cvSync != 0);
		Set(cWindowed, res->w, res->h);
		if (GRAPHICDEVICE && GameMode == 2)
			GRAPHICENGINE->Reset(res->w, res->h);
	}

	if (GAMECOREHANDLE && GAMECOREHANDLE->IsInit() && CHATBOX)
		CHATBOX->ToggleNotice(cShowNotice ? TRUE : FALSE);
}

void Settings::Save(bool resize)
{
	const int prevWindowed = cWindowed;
	const int prevResolution = cResolution;
	const bool wasMaximized = (hwnd && IsZoomed(hwnd)) ? true : false;

	CopyCommittedFromEdit();
	cBorderless = 0;
	bBorderless = 0;

	IniFiles ini(kIniPath);
	ini.WriteBool("Screen", "Windowed", cWindowed);
	ini.WriteBool("Screen", "AutoAdjust", cAutoAdjust);
	ini.WriteBool("Screen", "Borderless", 0);
	ini.WriteString("Screen", "Ratio", kRatioNames[ClampInt(cRatio, 0, 3)]);

	ini.WriteBool("Audio", "Music", cMusic);
	ini.WriteBool("Audio", "Sound", cSound);
	ini.WriteBool("Audio", "Ambient", cAmbient);
	ini.WriteBool("Graphics", "Effects", cEffects);
	ini.WriteBool("Graphics", "Damage", cShowDamage);
	ini.WriteBool("Graphics", "DynamicLights", cLights);
	ini.WriteBool("Graphics", "DynamicShadows", cShadows);
	ini.WriteBool("Graphics", "VSync", cvSync);
	ini.WriteBool("Graphics", "BlockUI", cvBlockUI);
	ini.WriteBool("Graphics", "Weather", cWeather);
	ini.WriteBool("Camera", "FarCameraSight", cCamShake);
	ini.WriteBool("Camera", "InvertedCamera", cCamInv);
	ini.WriteBool("LootFilter", "Enabled", cFilter);
	ini.WriteBool("LootFilter", "HP", cHP);
	ini.WriteBool("LootFilter", "MP", cMP);
	ini.WriteBool("LootFilter", "SP", cSP);
	ini.WriteBool("LootFilter", "Gold", cGold);
	ini.WriteBool("LootFilter", "Amulets", cAmulets);
	ini.WriteBool("LootFilter", "Rings", cRings);
	ini.WriteBool("LootFilter", "Sheltoms", cSheltoms);
	ini.WriteBool("LootFilter", "Force", cForce);
	ini.WriteBool("LootFilter", "Premiums", cPremiums);
	ini.WriteBool("LootFilter", "Crystal", cCrystal);
	ini.WriteBool("LootFilter", "DefItem", cDefItem);
	ini.WriteBool("LootFilter", "OffItem", cOffItem);
	ini.WriteBool("LootFilter", "Else", cElse);
	ini.WriteBool("LootSpecFilter", "Enabled", cFilterSpec);
	ini.WriteBool("LootSpecFilter", "MS", cMS);
	ini.WriteBool("LootSpecFilter", "FS", cFS);
	ini.WriteBool("LootSpecFilter", "PS", cPS);
	ini.WriteBool("LootSpecFilter", "AS", cAS);
	ini.WriteBool("LootSpecFilter", "KS", cKS);
	ini.WriteBool("LootSpecFilter", "ATS", cATS);
	ini.WriteBool("LootSpecFilter", "PRS", cPRS);
	ini.WriteBool("LootSpecFilter", "MGS", cMGS);
	ini.WriteBool("LootSpecFilter", "Naked", cNaked);
	ini.WriteBool("Game", "RememberAccount", cRememberLogin);
	ini.WriteBool("Game", "HideNames", cHidePlayerNames);
	ini.WriteBool("Game", "ShowNames", cHidePlayerNames);
	ini.WriteBool("Game", "ShowLife", cShowLife);
	ini.WriteBool("Game", "ShowAlert", cShowNotice);
	ini.WriteBool("Game", "ShowFPS", cShowFPS);

	if (resize)
	{
		const ResolutionOption* res = FindResolution(cResolution);
		ini.WriteInt("Screen", "Width", res->w);
		ini.WriteInt("Screen", "Height", res->h);
		ini.WriteInt("Graphics", "TextureQuality", cTexture);
		ini.WriteInt("Graphics", "BitDepth", 32);
		ini.WriteInt("Audio", "MusicVolume", cMVol);
		ini.WriteInt("Audio", "SoundVolume", cSVol);
		ini.WriteInt("Camera", "View", cCamView);
		ini.WriteInt("Camera", "Range", cCamRange);
	}

	const bool displayChanged = (cWindowed != prevWindowed) || (cResolution != prevResolution);
	ApplyRuntime(resize && displayChanged);

	if (resize)
	{
		LastMusicVolume = cMVol * 50;
		SetVolumeBGM((DWORD)LastMusicVolume);
		if (!cMusic)
			StopBGM();

		if (GAMECOREHANDLE && GAMECOREHANDLE->IsInit())
		{
			GAMECOREHANDLE->SetCanMove(cvBlockUI);
			if (CHUDCONTROLLER)
				CHUDCONTROLLER->UpdateObjectsSettings();
		}

		if (displayChanged && cWindowed && wasMaximized && hwnd)
			ShowWindow(hwnd, SW_MAXIMIZE);
	}
}

void Settings::SaveRememberLogin()
{
	cRememberLogin = bRememberLogin;
	IniFiles ini(kIniPath);
	ini.WriteBool("Game", "RememberAccount", cRememberLogin);
}

bool Settings::setAutoAdjust(int iWidth, int iHeight)
{
	extern void resizeOpening();
	extern void resizeLogin();
	extern void resizeSin();
	extern void resizePartyUI();

	smScreenWidth = iWidth;
	smScreenHeight = iHeight;
	MidX = iWidth / 2;
	MidY = iHeight / 2;

	g_fWinSizeRatio_X = float(iWidth) / 800.f;
	g_fWinSizeRatio_Y = float(iHeight) / 600.f;
	WinSizeX = iWidth;
	WinSizeY = iHeight;

	SetDxProjection((g_PI / 4.4f), iWidth, iHeight, 20.f, 4000.f);
	resizeOpening();
	resizeLogin();
	resizeSin();
	resizePartyUI();

	viewdistZ = (iHeight * 4) / 3;
	smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT;

	if (GRAPHICDEVICE)
		SetDisplayMode(hwnd, smScreenWidth, smScreenHeight, 32);

	return true;
}

void Settings::Set(int Windowed, int iWidth, int iHeight)
{
	ConfigUseDynamicLights = cLights;
	ConfigUseDynamicShadows = cShadows;

	WinSizeX = iWidth;
	WinSizeY = iHeight;
	smScreenWidth = iWidth;
	smScreenHeight = iHeight;
	smConfig.ScreenSize.x = WinSizeX;
	smConfig.ScreenSize.y = WinSizeY;
	MidX = WinSizeX / 2;
	MidY = WinSizeY / 2;
	g_fWinSizeRatio_X = float(WinSizeX) / 800.f;
	g_fWinSizeRatio_Y = float(WinSizeY) / 600.f;
	viewdistZ = ((WinSizeY / 3) * 4);
	SetDxProjection((g_PI / 4.4f), WinSizeX, WinSizeY, 20.f, 4000.f);

	if (Windowed)
	{
		RECT rc = { 0, 0, iWidth, iHeight };
		UINT uWindowStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		AdjustWindowRect(&rc, uWindowStyle, FALSE);
		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;
		SetWindowLongA(hwnd, GWL_STYLE, uWindowStyle);
		SetWindowPos(hwnd, NULL,
			((GetSystemMetrics(SM_CXSCREEN) >> 1) - (w >> 1)),
			((GetSystemMetrics(SM_CYSCREEN) >> 1) - (h >> 1)),
			w, h, SWP_NOZORDER | SWP_FRAMECHANGED);
		UpdateWindow(hwnd);
	}
	else
	{
		SetWindowPos(hwnd, NULL, 0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			SWP_DRAWFRAME | SWP_NOZORDER);
		SetWindowLongA(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}

	if (GRAPHICDEVICE && GameMode == 2)
	{
		extern void resizeOpening();
		extern void resizeLogin();
		resizeOpening();
		resizeLogin();
		GAMECOREHANDLE->OnResolutionChanged();
	}
}

void Settings::Reset()
{
	FillEditDefaults();
}
