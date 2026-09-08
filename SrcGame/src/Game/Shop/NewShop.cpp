#include "NewShop.h"
#include "utils\strings.h"
#include <atlconv.h>
#include "..\\HUD\\RestaureWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\HUD\\ImGuiWindowChrome.h"
#include "imGui/imgui_impl_dx9.h"
#include "../smLib3d/smDsx.h"
#include "..\\ConfirmationBox.h"

#include <boost\\algorithm\\string\\predicate.hpp>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <zlib.h>
#include <algorithm>
#include <vector>
#include <string>
#include <cmath>
#include <cfloat>

#include <curl/curl.h>
#include <TitleBox/TitleBox.h>

#ifdef _DEBUG
#pragma comment (lib, "crypt32")
#pragma comment (lib, "ws2_32")
#pragma comment (lib, "Normaliz")
#pragma comment (lib, "Wldap32")
#pragma comment (lib, "Game/API/libcurl_a_debug.lib")
#endif

#define U8_A "\xC3\xA1"
#define U8_E "\xC3\xA9"
#define U8_I "\xC3\xAD"
#define U8_O "\xC3\xB3"
#define U8_U "\xC3\xBA"
#define U8_AN "\xC3\xA3"
#define U8_ON "\xC3\xB5"
#define U8_C "\xC3\xA7"

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\shop\\loja-de-coins.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const float kWindowW = 720.0f;
static const float kWindowH = 520.0f;

extern BOOL DecryptBMP(char* pBuffer, unsigned int uBufferSize);
extern LPDIRECT3DTEXTURE9 LoadDibSurfaceOffscreen(char* Filename);
extern sITEM TempPerfectItem;
extern BOOL bIsPerfect;
extern int sinShowItemInfoFlag;
extern FailedItemsPckg failedItems;
extern int smScreenWidth;
extern int smScreenHeight;

std::vector<sITEM> ItemsDetails;
std::vector<sITEM> ItemsDetailsRestaure;
std::vector<std::string> ItemsAlreadySent;
std::vector<int> ItemsAlreadySentRestaure;

int nickIsAvailable = 0;
char NewNick[30] = { 0 };

static int selectedClass = 0;
static int selectedChangeClass = 0;
static int LastSelectedClass = 0;

static PDIRECT3DTEXTURE9 ClassImages[12] = {};
static PDIRECT3DTEXTURE9 ShopUiImages[4] = {};
static LPDIRECT3DTEXTURE9 CoinImage = nullptr;
static LPDIRECT3DTEXTURE9 RestaureImage[20] = {};
static std::vector<LPDIRECT3DTEXTURE9> ItemShopImage;
static std::vector<int> ItemShopImageW;
static std::vector<int> ItemShopImageH;
static int RestaureImageW[20] = {};
static int RestaureImageH[20] = {};

extern int g_DibLastReadWidth;
extern int g_DibLastReadHeight;

const char* Classes[] = { "Sem Classe", "Lutador", "Mec" U8_AN "nico", "Arqueira", "Pikeman", "Atalanta", "Cavaleiro", "Mago", "Sacerdotisa" };

struct ShopSubTab
{
	int id;
	const char* label;
};

struct ShopCategory
{
	int id;
	const char* label;
	const ShopSubTab* subs;
	int subCount;
};

static const ShopSubTab kAtaqueSubs[] = {
	{ 1, "Caixas" }, { 2, "Espadas" }, { 3, "Foices" }, { 4, "Garras" },
	{ 5, "Lan" U8_C "as" }, { 6, "Machados" }, { 7, "Martelos" }, { 8, "Varinhas" },
	{ 9, "Arcos" }
};
static const ShopSubTab kDefesaSubs[] = {
	{ 1, "Caixas" }, { 2, "Roup" U8_ON "es" }, { 3, "Escudos" }, { 4, "Orbitais" },
	{ 5, "Armaduras" }, { 6, "Luvas" }
};
static const ShopSubTab kAcessoriosSubs[] = {
	{ 1, "Braceletes" }, { 2, "Botas" }, { 3, "Brincos" },
	{ 4, "An" U8_E "is" }, { 5, "Colares" }, { 6, "Pedras" }
};
static const ShopSubTab kTrajesSubs[] = {
	{ 1, "Masculinos" }, { 2, "Femininos" }
};
static const ShopSubTab kPremiumSubs[] = {
	{ 1, "Aprimoramento" }, { 2, "Aging e Mix" }, { 3, "For" U8_C "as" }, { 4, "Utilit" U8_A "rios" }
};
static const ShopSubTab kServicosSubs[] = {
	{ 1, "Restaurar Item" }, { 2, "VIP" }, { 3, "Troca de Nick" }
};

static const ShopCategory kCategories[] = {
	{ 1, "Ataque", kAtaqueSubs, 9 },
	{ 2, "Defesa", kDefesaSubs, 6 },
	{ 3, "Acess" U8_O "rios", kAcessoriosSubs, 6 },
	{ 4, "Trajes", kTrajesSubs, 2 },
	{ 5, "Premium", kPremiumSubs, 4 },
	{ 6, "Servi" U8_C "os", kServicosSubs, 3 }
};
static const int kCategoryCount = 6;

static const ShopCategory* FindCategory(int id)
{
	for (int i = 0; i < kCategoryCount; i++)
	{
		if (kCategories[i].id == id)
			return &kCategories[i];
	}
	return &kCategories[0];
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

static void GetTextureSize(LPDIRECT3DTEXTURE9 tex, int* w, int* h)
{
	if (w) *w = 0;
	if (h) *h = 0;
	if (!tex)
		return;

	D3DSURFACE_DESC desc;
	if (FAILED(tex->GetLevelDesc(0, &desc)))
		return;
	if (w) *w = (int)desc.Width;
	if (h) *h = (int)desc.Height;
}

static void DrawTextureInBox(ImDrawList* draw, LPDIRECT3DTEXTURE9 tex, const ImVec2& boxMin, float boxW, float boxH, int srcW = 0, int srcH = 0)
{
	const ImVec2 boxMax(boxMin.x + boxW, boxMin.y + boxH);
	int texW = 0, texH = 0;
	GetTextureSize(tex, &texW, &texH);
	if (!tex || texW <= 0 || texH <= 0)
	{
		draw->AddRectFilled(boxMin, boxMax, IM_COL32(40, 40, 48, 255), 3.0f);
		return;
	}

	int contentW = (srcW > 0) ? srcW : texW;
	int contentH = (srcH > 0) ? srcH : texH;
	if (contentW > texW)
		contentW = texW;
	if (contentH > texH)
		contentH = texH;

	float drawW = (float)contentW;
	float drawH = (float)contentH;
	if (drawW > boxW || drawH > boxH)
	{
		const ImVec2 sz = FitImageSize(contentW, contentH, boxW, boxH);
		drawW = sz.x;
		drawH = sz.y;
	}

	const float x = boxMin.x + (boxW - drawW) * 0.5f;
	const float y = boxMin.y + (boxH - drawH) * 0.5f;
	const ImVec2 uv1((float)contentW / (float)texW, (float)contentH / (float)texH);
	draw->AddImage((ImTextureID)tex, ImVec2(x, y), ImVec2(x + drawW, y + drawH), ImVec2(0.0f, 0.0f), uv1);
}

static const char* ShopItemTypeName(DWORD code)
{
	const DWORD mask = code & sinITEM_MASK2;
	if (mask == sinOA1) return "Amuleto";
	if (mask == sinDA1) return "Armadura";
	if (mask == sinWA1) return "Machado";
	if (mask == sinDB1) return "Botas";
	if (mask == sinWS1) return "Arco";
	if (mask == sinOA2) return "Bracelete";
	if (mask == sinWC1) return "Garra";
	if (mask == sinDG1) return "Luvas";
	if (mask == sinWH1) return "Martelo";
	if (mask == sinWT1) return "Lan" U8_C "a";
	if (mask == sinOR1 || mask == sinOR2) return "Anel";
	if (mask == sinDA2) return "Roup" U8_AN "o";
	if (mask == sinWP1) return "Foice";
	if (mask == sinDS1) return "Escudo";
	if (mask == sinWS2) return "Espada";
	if (mask == sinWM1) return "Varinha";
	if (mask == sinOM1) return "Orbital";
	if (mask == sinOS1) return "Sheltom";
	if (mask == sinOE1) return "Brinco";
	if (mask == sinCA1 || mask == sinCA2) return "Traje";
	if (mask == sinFO1) return "For" U8_C "a";
	return "";
}

static void ShopStatRow(const char* label, const char* value, bool warn = false)
{
	const float startX = ImGui::GetCursorPosX();
	const float width = ImGui::GetContentRegionAvail().x;
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.86f, 0.78f, 1.0f));
	ImGui::TextUnformatted(label);
	ImGui::PopStyleColor();

	const ImVec2 vs = ImGui::CalcTextSize(value);
	float valueX = startX + width - vs.x;
	if (valueX < startX + ImGui::CalcTextSize(label).x + 8.0f)
		valueX = startX + ImGui::CalcTextSize(label).x + 8.0f;
	ImGui::SameLine(valueX);
	ImGui::PushStyleColor(ImGuiCol_Text, warn
		? ImVec4(0.85f, 0.32f, 0.32f, 1.0f)
		: ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
	ImGui::TextUnformatted(value);
	ImGui::PopStyleColor();
}

static void ShopStatSection(const char* title)
{
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.43f, 1.0f));
	ImGui::TextUnformatted(title);
	ImGui::PopStyleColor();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImVec4(0.78f, 0.67f, 0.35f, 0.40f));
	ImGui::Separator();
	ImGui::PopStyleColor();
}

static bool ShopShowsWeaponCombatStats(DWORD code)
{
	const DWORD mask = code & sinITEM_MASK2;
	return mask == sinWA1 || mask == sinWC1 || mask == sinWH1 || mask == sinWM1
		|| mask == sinWP1 || mask == sinWS1 || mask == sinWS2 || mask == sinWT1;
}

static void DrawShopItemStats(const sITEM* preview)
{
	if (!preview)
		return;

	const sITEMINFO& info = preview->sItemInfo;
	char buf[64] = { 0 };
	const smCHAR_INFO* me = lpCurPlayer ? &lpCurPlayer->smCharInfo : nullptr;
	const bool weaponCombat = ShopShowsWeaponCombatStats(info.CODE);
	const DWORD mask2 = info.CODE & sinITEM_MASK2;
	const bool showAttackRating = weaponCombat
		|| mask2 == sinOA2 || mask2 == sinOE1 || mask2 == sinOR1 || mask2 == sinOR2;

	const char* typeName = ShopItemTypeName(info.CODE);
	if (typeName[0])
	{
		if (weaponCombat && preview->WeaponClass == ITEM_CLASS_WEAPON_TWO)
			sprintf_s(buf, sizeof(buf), "Two-Handed %s", typeName);
		else if (weaponCombat && preview->WeaponClass == ITEM_CLASS_WEAPON_ONE)
			sprintf_s(buf, sizeof(buf), "One-Handed %s", typeName);
		else
			sprintf_s(buf, sizeof(buf), "%s", typeName);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.70f, 0.66f, 0.58f, 1.0f));
		ImGui::TextWrapped("%s", buf);
		ImGui::PopStyleColor();
	}

	ShopStatSection("ATRIBUTOS");

	if (weaponCombat && (info.Damage[0] || info.Damage[1]))
	{
		sprintf_s(buf, sizeof(buf), "%d-%d", info.Damage[0], info.Damage[1]);
		ShopStatRow("Poder de Ataque:", buf);
	}
	if (weaponCombat && info.Attack_Speed)
	{
		sprintf_s(buf, sizeof(buf), "%d", info.Attack_Speed);
		ShopStatRow("Velocidade:", buf);
	}
	if (weaponCombat && info.Shooting_Range)
	{
		sprintf_s(buf, sizeof(buf), "%d", info.Shooting_Range);
		ShopStatRow("Alcance:", buf);
	}
	if (weaponCombat && info.Critical_Hit)
	{
		sprintf_s(buf, sizeof(buf), "%d%%", info.Critical_Hit);
		ShopStatRow("Cr" U8_I "tico:", buf);
	}
	if (showAttackRating && info.Attack_Rating)
	{
		sprintf_s(buf, sizeof(buf), "%d", info.Attack_Rating);
		ShopStatRow("Taxa de Ataque:", buf);
	}
	if (info.Defence)
	{
		sprintf_s(buf, sizeof(buf), "%d", info.Defence);
		ShopStatRow("Defesa:", buf);
	}
	if (info.fAbsorb)
	{
		sprintf_s(buf, sizeof(buf), "%.1f", info.fAbsorb);
		ShopStatRow("Absor" U8_C U8_AN "o:", buf);
	}
	if (info.fBlock_Rating)
	{
		if (fabs(info.fBlock_Rating - std::round(info.fBlock_Rating)) < FLT_MIN)
			sprintf_s(buf, sizeof(buf), "%.0f%%", std::round(info.fBlock_Rating));
		else
			sprintf_s(buf, sizeof(buf), "%.1f%%", info.fBlock_Rating);
		ShopStatRow("Bloqueio:", buf);
	}
	if (info.fSpeed)
	{
		sprintf_s(buf, sizeof(buf), "%.1f", info.fSpeed);
		ShopStatRow("Vel. de M.:", buf);
	}
	if (info.Durability[0] || info.Durability[1])
	{
		sprintf_s(buf, sizeof(buf), "%d/%d", info.Durability[0], info.Durability[1]);
		ShopStatRow("Integridade:", buf);
	}
	if (info.Mana[0] || info.Mana[1])
	{
		sprintf_s(buf, sizeof(buf), "%d-%d", info.Mana[0], info.Mana[1]);
		ShopStatRow("Rec. de MP:", buf);
	}
	if (info.Life[0] || info.Life[1])
	{
		sprintf_s(buf, sizeof(buf), "%d-%d", info.Life[0], info.Life[1]);
		ShopStatRow("Rec. de HP:", buf);
	}
	if (info.Stamina[0] || info.Stamina[1])
	{
		sprintf_s(buf, sizeof(buf), "%d-%d", info.Stamina[0], info.Stamina[1]);
		ShopStatRow("Rec. de RES:", buf);
	}
	if (info.fLife_Regen && (info.CODE & sinITEM_MASK2) != sinFO1)
	{
		sprintf_s(buf, sizeof(buf), "%.1f", info.fLife_Regen);
		ShopStatRow("HP Regen.:", buf);
	}
	if (info.fMana_Regen && (info.CODE & sinITEM_MASK2) != sinFO1)
	{
		sprintf_s(buf, sizeof(buf), "%.1f", info.fMana_Regen);
		ShopStatRow("MP Regen.:", buf);
	}
	if (info.fStamina_Regen && (info.CODE & sinITEM_MASK2) != sinFO1)
	{
		sprintf_s(buf, sizeof(buf), "%.1f", info.fStamina_Regen);
		ShopStatRow("RES Regen.:", buf);
	}
	if (info.fMagic_Mastery)
	{
		sprintf_s(buf, sizeof(buf), "%.0f", info.fMagic_Mastery);
		ShopStatRow("Ataque M" U8_A "gico:", buf);
	}
	if (info.fIncrease_Life)
	{
		sprintf_s(buf, sizeof(buf), "%.0f", info.fIncrease_Life);
		ShopStatRow("Add HP:", buf);
	}
	if (info.fIncrease_Mana)
	{
		sprintf_s(buf, sizeof(buf), "%.0f", info.fIncrease_Mana);
		ShopStatRow("Add MP:", buf);
	}
	if (info.fIncrease_Stamina)
	{
		sprintf_s(buf, sizeof(buf), "%.0f", info.fIncrease_Stamina);
		ShopStatRow("Add RES:", buf);
	}
	if (info.Potion_Space)
	{
		sprintf_s(buf, sizeof(buf), "%d", info.Potion_Space);
		ShopStatRow("Po" U8_C U8_ON "es:", buf);
	}

	if (info.Resistance[sITEMINFO_BIONIC] || info.Resistance[sITEMINFO_FIRE] ||
		info.Resistance[sITEMINFO_ICE] || info.Resistance[sITEMINFO_LIGHTING] ||
		info.Resistance[sITEMINFO_POISON])
	{
		ShopStatSection("RESIST" U8_E "NCIAS");
		if (info.Resistance[sITEMINFO_BIONIC])
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Resistance[sITEMINFO_BIONIC]);
			ShopStatRow("Org" U8_AN "nico:", buf);
		}
		if (info.Resistance[sITEMINFO_FIRE])
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Resistance[sITEMINFO_FIRE]);
			ShopStatRow("Fogo:", buf);
		}
		if (info.Resistance[sITEMINFO_ICE])
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Resistance[sITEMINFO_ICE]);
			ShopStatRow("Gelo:", buf);
		}
		if (info.Resistance[sITEMINFO_LIGHTING])
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Resistance[sITEMINFO_LIGHTING]);
			ShopStatRow("Raio:", buf);
		}
		if (info.Resistance[sITEMINFO_POISON])
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Resistance[sITEMINFO_POISON]);
			ShopStatRow("Veneno:", buf);
		}
	}

	if (info.Level || info.Strength || info.Spirit || info.Talent || info.Dexterity || info.Health)
	{
		ShopStatSection("REQUISITOS");
		if (info.Level)
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Level);
			ShopStatRow("Req. Level:", buf, me && me->Level < info.Level);
		}
		if (info.Strength)
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Strength);
			ShopStatRow("Req. For" U8_C "a:", buf, me && me->Strength < info.Strength);
		}
		if (info.Spirit)
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Spirit);
			ShopStatRow("Req. Int.:", buf, me && me->Spirit < info.Spirit);
		}
		if (info.Talent)
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Talent);
			ShopStatRow("Req. Talento:", buf, me && me->Talent < info.Talent);
		}
		if (info.Dexterity)
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Dexterity);
			ShopStatRow("Req. Agilidade:", buf, me && me->Dexterity < info.Dexterity);
		}
		if (info.Health)
		{
			sprintf_s(buf, sizeof(buf), "%d", info.Health);
			ShopStatRow("Req. Vida:", buf, me && me->Health < info.Health);
		}
	}

	const sITEM_SPECIAL& spec = info.JobItem;
	const bool hasSpec = info.JobCodeMask != 0 && (
		spec.Add_Attack_Speed || spec.Add_Critical_Hit || spec.Add_Defence ||
		spec.Add_fAbsorb || spec.Add_fBlock_Rating || spec.Add_fMagic_Mastery ||
		spec.Add_fSpeed || spec.Add_Shooting_Range || spec.Lev_Damage[1] ||
		spec.Lev_Attack_Rating || spec.Lev_Life || spec.Lev_Mana ||
		spec.Per_Life_Regen || spec.Per_Mana_Regen || spec.Per_Stamina_Regen ||
		spec.Add_Resistance[sITEMINFO_BIONIC] || spec.Add_Resistance[sITEMINFO_FIRE] ||
		spec.Add_Resistance[sITEMINFO_ICE] || spec.Add_Resistance[sITEMINFO_LIGHTING] ||
		spec.Add_Resistance[sITEMINFO_POISON]);

	if (hasSpec)
	{
		const char* specName = (selectedClass >= 0 && selectedClass < IM_ARRAYSIZE(Classes))
			? Classes[selectedClass] : "SPEC";
		sprintf_s(buf, sizeof(buf), "%s +", specName);
		ShopStatSection(buf);

		if (weaponCombat && spec.Add_Attack_Speed)
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Attack_Speed);
			ShopStatRow("Vel. de Ataque:", buf);
		}
		if (weaponCombat && spec.Add_Critical_Hit)
		{
			sprintf_s(buf, sizeof(buf), "%d%%", spec.Add_Critical_Hit);
			ShopStatRow("Taxa de Cr" U8_I "tico:", buf);
		}
		if (spec.Add_Defence)
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Defence);
			ShopStatRow("Defesa:", buf);
		}
		if (spec.Add_fAbsorb)
		{
			sprintf_s(buf, sizeof(buf), "%.1f", spec.Add_fAbsorb);
			ShopStatRow("Absor" U8_C U8_AN "o:", buf);
		}
		if (spec.Add_fBlock_Rating)
		{
			if (fabs(spec.Add_fBlock_Rating - std::round(spec.Add_fBlock_Rating)) < FLT_MIN)
				sprintf_s(buf, sizeof(buf), "%.0f%%", std::round(spec.Add_fBlock_Rating));
			else
				sprintf_s(buf, sizeof(buf), "%.1f%%", spec.Add_fBlock_Rating);
			ShopStatRow("Bloqueio:", buf);
		}
		if (spec.Add_fMagic_Mastery)
		{
			sprintf_s(buf, sizeof(buf), "%.0f", spec.Add_fMagic_Mastery);
			ShopStatRow("Ataque M" U8_A "gico:", buf);
		}
		if (spec.Add_fSpeed)
		{
			sprintf_s(buf, sizeof(buf), "%.1f", spec.Add_fSpeed);
			ShopStatRow("Velocidade:", buf);
		}
		if (weaponCombat && spec.Add_Shooting_Range)
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Shooting_Range);
			ShopStatRow("Alcance:", buf);
		}
		if (weaponCombat && spec.Lev_Damage[1])
		{
			sprintf_s(buf, sizeof(buf), "LV/%d", spec.Lev_Damage[1]);
			ShopStatRow("Poder de Ataque:", buf);
		}
		if (showAttackRating && spec.Lev_Attack_Rating)
		{
			sprintf_s(buf, sizeof(buf), "LV/%d", spec.Lev_Attack_Rating);
			ShopStatRow("Taxa de Ataque:", buf);
		}
		if (spec.Lev_Life)
		{
			sprintf_s(buf, sizeof(buf), "LV/%d", spec.Lev_Life);
			ShopStatRow("Max. HP Boost:", buf);
		}
		if (spec.Lev_Mana)
		{
			sprintf_s(buf, sizeof(buf), "LV/%d", spec.Lev_Mana);
			ShopStatRow("Max. MP Boost:", buf);
		}
		if (spec.Per_Life_Regen && (info.CODE & sinITEM_MASK2) != sinFO1)
		{
			sprintf_s(buf, sizeof(buf), "%.1f", spec.Per_Life_Regen);
			ShopStatRow("HP Regen.:", buf);
		}
		if (spec.Per_Mana_Regen && (info.CODE & sinITEM_MASK2) != sinFO1)
		{
			sprintf_s(buf, sizeof(buf), "%.1f", spec.Per_Mana_Regen);
			ShopStatRow("MP Regen.:", buf);
		}
		if (spec.Per_Stamina_Regen && (info.CODE & sinITEM_MASK2) != sinFO1)
		{
			sprintf_s(buf, sizeof(buf), "%.1f", spec.Per_Stamina_Regen);
			ShopStatRow("RES Regen.:", buf);
		}
		if (spec.Add_Resistance[sITEMINFO_BIONIC])
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Resistance[sITEMINFO_BIONIC]);
			ShopStatRow("Org" U8_AN "nico:", buf);
		}
		if (spec.Add_Resistance[sITEMINFO_FIRE])
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Resistance[sITEMINFO_FIRE]);
			ShopStatRow("Fogo:", buf);
		}
		if (spec.Add_Resistance[sITEMINFO_ICE])
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Resistance[sITEMINFO_ICE]);
			ShopStatRow("Gelo:", buf);
		}
		if (spec.Add_Resistance[sITEMINFO_LIGHTING])
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Resistance[sITEMINFO_LIGHTING]);
			ShopStatRow("Raio:", buf);
		}
		if (spec.Add_Resistance[sITEMINFO_POISON])
		{
			sprintf_s(buf, sizeof(buf), "%d", spec.Add_Resistance[sITEMINFO_POISON]);
			ShopStatRow("Veneno:", buf);
		}
	}
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

bool LoadTextureFromFile4(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	PDIRECT3DTEXTURE9 texture = nullptr;
	if (D3DXCreateTextureFromFileA(GRAPHICDEVICE, filename, &texture) != S_OK)
		return false;

	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	*out_texture = texture;
	*out_width = (int)desc.Width;
	*out_height = (int)desc.Height;
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
	ImGui::InvisibleButton("##ShopClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
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

static void CenterTextUnformatted(const char* text)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
	ImGui::TextUnformatted(text);
}

static const char* ShopItemFolder(const ItemsByCategory& item)
{
	const char c0 = (char)toupper((unsigned char)item.ItemCode[0]);
	const char c1 = (char)toupper((unsigned char)item.ItemCode[1]);
	const char* folder = "Premium";

	if (c0 == 'W')
		folder = "Weapon";
	else if (c0 == 'D')
		folder = "Defense";
	else if (c0 == 'O')
		folder = (c1 == 'S' || c1 == 'B') ? "Defense" : "Accessory";
	else if ((c0 == 'G' && c1 == 'P') || (c0 == 'S' && c1 == 'P'))
		folder = "Event";
	else if (c0 == 'S' && c1 == 'E')
		folder = "Accessory";
	else if (c0 == 'B' && c1 == 'I')
		folder = "Premium";
	else if (c0 == 'F' && c1 == 'O')
		folder = "Accessory";
	else if (c0 == 'C' && c1 == 'A')
		folder = "Defense";
	else
	{
		switch (item.CategoryID)
		{
		case 1: folder = "Weapon"; break;
		case 2: folder = "Defense"; break;
		case 3:
			folder = (item.SubCategoryID == 2) ? "Defense" : "Accessory";
			break;
		case 4: folder = "Defense"; break;
		case 5:
			if (item.SubCategoryID == 3)
				folder = "Accessory";
			else
				folder = "Premium";
			break;
		default:
			folder = "Premium";
			break;
		}
	}

	return folder;
}

static void ReleaseShopIcons()
{
	for (size_t i = 0; i < ItemShopImage.size(); i++)
	{
		if (ItemShopImage[i])
		{
			ItemShopImage[i]->Release();
			ItemShopImage[i] = nullptr;
		}
	}
	ItemShopImage.clear();
	ItemShopImageW.clear();
	ItemShopImageH.clear();
}

int SendItemToServer(char itemCode[32], int itemHead = 0, int ItemChkSum = 0, bool isRestaure = false, int AgingNum = 0)
{
	if (!isRestaure)
	{
		auto searchItem = std::find(ItemsAlreadySent.begin(), ItemsAlreadySent.end(), itemCode);
		if (searchItem == ItemsAlreadySent.end())
		{
			struct Pck
			{
				int size;
				int code;
				int spec;
				bool shopTime;
				char itemID[32];
			};

			Pck smPacket;
			ZeroMemory(&smPacket, sizeof(Pck));
			smPacket.code = 0x43550002;
			smPacket.shopTime = false;
			smPacket.spec = selectedClass;
			sprintf_s(smPacket.itemID, sizeof(smPacket.itemID), "%s", itemCode);
			smPacket.size = sizeof(Pck);

			if (smWsockDataServer)
				smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);
			ItemsAlreadySent.push_back(itemCode);
			return TRUE;
		}
	}
	else
	{
		auto searchItem = std::find(ItemsAlreadySentRestaure.begin(), ItemsAlreadySentRestaure.end(), itemHead);
		if (searchItem == ItemsAlreadySentRestaure.end())
		{
			struct Pck
			{
				int size;
				int code;
				int ItemHead;
				int ItemChkSum;
				int AgingNum;
				char itemCode[32];
			};

			Pck smPacket;
			ZeroMemory(&smPacket, sizeof(Pck));
			smPacket.code = 0x43550003;
			smPacket.ItemHead = itemHead;
			smPacket.ItemChkSum = ItemChkSum;
			smPacket.AgingNum = AgingNum;
			sprintf_s(smPacket.itemCode, sizeof(smPacket.itemCode), "%s", itemCode);
			smPacket.size = sizeof(Pck);

			if (smWsockDataServer)
				smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);
			ItemsAlreadySentRestaure.push_back(itemHead);
			return TRUE;
		}
	}

	if (!isRestaure)
	{
		for (auto& getItemToShow : ItemsDetails)
		{
			if (boost::iequals(getItemToShow.DorpItem, itemCode))
			{
				if (LastSelectedClass != selectedClass)
				{
					ItemsAlreadySent.clear();
					ItemsDetails.clear();
				}
				else
				{
					TempPerfectItem = getItemToShow;
					bIsPerfect = true;
				}
				break;
			}
		}
	}
	else
	{
		for (auto& getItemToShow : ItemsDetailsRestaure)
		{
			if (getItemToShow.sItemInfo.ItemHeader.Head == itemHead)
			{
				TempPerfectItem = getItemToShow;
				bIsPerfect = true;
				break;
			}
		}
	}

	LastSelectedClass = selectedClass;
	return TRUE;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void NewShop::Donation(char amount[32])
{
	CURL* curl = curl_easy_init();
	std::string urlDonate;

	if (curl)
	{
		char szstring[128] = { 0 };
		sprintf_s(szstring, sizeof(szstring), "http://www.dnsprotect.com.br/PristonTale/WDPT/CreateOrder.asp?amount=%s", amount);
		urlDonate.clear();
		curl_easy_setopt(curl, CURLOPT_URL, szstring);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &urlDonate);
		curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		std::vector<std::string> vTransaction = split(urlDonate.c_str(), '=');
		if (vTransaction.size() > 1)
			sprintf_s(idPaypal, sizeof(idPaypal), "%s", vTransaction[1].c_str());
		sprintf_s(Amount, sizeof(Amount), "%s", amount);
	}

	ShellExecute(NULL, NULL, urlDonate.c_str(), NULL, NULL, NULL);
}

void SentPurchaseToServer(ItemsByCategory Item, int Spec)
{
	for (auto& getItemToShow : ItemsDetails)
	{
		if (boost::iequals(getItemToShow.DorpItem, Item.ItemCode))
		{
			if (!cInvenTory.CheckSetEmptyArea(&getItemToShow))
			{
				TitleBox::GetInstance()->SetText("Voc" "\xEA" " n" "\xE3" "o tem espa" "\xE7" "o suficiente no invent" "\xE1" "rio!", 3);
				return;
			}

			if (!cInvenTory.CheckSetOk(&getItemToShow, 1))
			{
				TitleBox::GetInstance()->SetText("Excede o peso limite.", 3);
				return;
			}
		}
	}

	sFinishPurchase sFinal;
	sFinal.Item = Item;
	sFinal.Quantity = 1;
	sFinal.Spec = Spec;
	sFinal.code = NewShopItems_FinishPurchase;
	sFinal.size = sizeof(sFinishPurchase);

	if (smWsockDataServer)
		smWsockDataServer->Send((char*)&sFinal, sFinal.size, TRUE);
}

static void SentCheckNickToServer(ItemsByCategory Item, int Spec)
{
	sFinishPurchase sCheckNick;
	sCheckNick.Item = Item;
	sCheckNick.Quantity = 1;
	sCheckNick.Spec = Spec;
	sCheckNick.code = NewShopItems_CheckNick;
	sCheckNick.size = sizeof(sFinishPurchase);
	if (smWsockDataServer)
		smWsockDataServer->Send((char*)&sCheckNick, sCheckNick.size, TRUE);
}

static void SentChangeNickToServer(ItemsByCategory Item, int Spec)
{
	sFinishPurchase sChangeNick;
	sChangeNick.Item = Item;
	sChangeNick.Quantity = 1;
	sChangeNick.Spec = Spec;
	sChangeNick.code = NewShopItems_ChangeNick;
	sChangeNick.size = sizeof(sFinishPurchase);
	if (smWsockDataServer)
		smWsockDataServer->Send((char*)&sChangeNick, sChangeNick.size, TRUE);
}

INT RecoverItem(int ItemHead, int ItemChkSum)
{
	smTRANS_COMMAND getFailedItems;
	getFailedItems.code = RECOVER_AGING_ITEM;
	getFailedItems.size = sizeof(smTRANS_COMMAND);
	getFailedItems.WParam = ItemHead;
	getFailedItems.LParam = ItemChkSum;
	getFailedItems.SParam = 0;
	if (smWsockServer)
		smWsockServer->Send((char*)&getFailedItems, getFailedItems.size, TRUE);
	return TRUE;
}

void NewShop::RecvCoin(int coin)
{
	Coin = coin;
}

void NewShop::MinusCoin(int coin)
{
	Coin = Coin - coin;
}

void NewShop::PlusCoin(int coin)
{
	Coin = Coin + coin;
}

void NewShop::ReceiveItems(NEWSHOP_COMPRESSEDPCKG* Data)
{
	if (!Data)
		return;

	int chunkIndex = 0;
	int totalChunks = 1;
	if (Data->size >= (int)sizeof(NEWSHOP_COMPRESSEDPCKG))
	{
		chunkIndex = Data->chunkIndex;
		if (Data->totalChunks > 0)
			totalChunks = Data->totalChunks;
	}

	ITEMS_INFOCKG getAllItems;
	ZeroMemory(&getAllItems, sizeof(ITEMS_INFOCKG));

	unsigned long nCompressedDataSize = Data->CompressedDataPckg.compressedSize;
	unsigned long UnCompressedDataSize = sizeof(getAllItems.Items);

	uncompress((Bytef*)&getAllItems.Items, &UnCompressedDataSize, (Bytef*)Data->CompressedDataPckg.pCompressedData, nCompressedDataSize);

	if (chunkIndex <= 0)
		ShopItems.clear();

	for (int i = 0; i < 200; i++)
	{
		if (getAllItems.Items[i].CategoryID == 0)
			break;
		ShopItems.push_back(getAllItems.Items[i]);
	}

	if (chunkIndex + 1 >= totalChunks)
	{
		m_shopReady = false;
		m_selectedIndex = -1;
		openFlag = true;
	}
}

bool NewShop::IsBlockingMouse(int x, int y) const
{
	if (!openFlag)
		return false;
	if (m_confirmBuy || m_confirmVip || m_confirmNick || m_confirmRestaure >= 0)
		return true;
	return m_winW > 0.0f && m_winH > 0.0f
		&& x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH);
}

std::string NewShop::ToUtf8(const char* src) const
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

const ItemsByCategory* NewShop::SelectedItem() const
{
	if (m_selectedIndex < 0 || m_selectedIndex >= (int)ShopItems.size())
		return nullptr;
	return &ShopItems[m_selectedIndex];
}

const sITEM* NewShop::FindPreview(const char* itemCode) const
{
	if (!itemCode || !itemCode[0])
		return nullptr;
	for (auto& item : ItemsDetails)
	{
		if (boost::iequals(item.DorpItem, itemCode))
			return &item;
	}
	return nullptr;
}

void NewShop::RequestPreview(const char* itemCode)
{
	if (!itemCode || !itemCode[0])
		return;
	char code[32] = { 0 };
	sprintf_s(code, sizeof(code), "%s", itemCode);
	SendItemToServer(code);
}

void NewShop::PlacePreviewNearMouse()
{
	const ImVec2 mouse = ImGui::GetIO().MousePos;
	TempPerfectItem.x = (int)mouse.x;
	TempPerfectItem.y = (int)mouse.y;
	TempPerfectItem.w = 0;
	TempPerfectItem.h = 0;
}

void NewShop::EnsureSelection()
{
	if (m_category == 6)
		return;

	if (m_selectedIndex >= 0 && m_selectedIndex < (int)ShopItems.size())
	{
		const ItemsByCategory& cur = ShopItems[m_selectedIndex];
		if (cur.CategoryID == m_category && cur.SubCategoryID == m_subTab)
			return;
	}

	m_selectedIndex = -1;
	for (int i = 0; i < (int)ShopItems.size(); i++)
	{
		if (ShopItems[i].CategoryID == m_category && ShopItems[i].SubCategoryID == m_subTab)
		{
			m_selectedIndex = i;
			RequestPreview(ShopItems[i].ItemCode);
			break;
		}
	}
}

std::string NewShop::PremiumDescription(const ItemsByCategory& item) const
{
	char upperName[64] = { 0 };
	sprintf_s(upperName, sizeof(upperName), "%s", item.ItemName);
	for (char* p = upperName; *p; p++)
		*p = (char)toupper((unsigned char)*p);

	if (strstr(upperName, "EXP") || strstr(upperName, "E. DE") || strstr(upperName, "EXPER"))
		return "Po" U8_C U8_AN "o de experi" U8_E "ncia. Aumenta o EXP recebido dos monstros pelo per" U8_I "odo indicado no nome.";
	if (strstr(upperName, "OLHO") || strstr(upperName, "MAGIC"))
		return "Olho m" U8_A "gico. Revela monstros e itens no minimapa pelo per" U8_I "odo indicado no nome.";
	if (strstr(upperName, "CHAVE") || strstr(upperName, "KEY"))
		return "Chave de uso especial. Confira o nome para saber em qual ba" U8_U " ou evento ela serve.";
	if (item.SubCategoryID == 2)
		return "Material de Aging ou Mix. Use na forja para aprimorar ou recombinar equipamentos.";
	if (item.SubCategoryID == 3)
		return "Pedra de for" U8_C "a. Aplica um b" U8_ON "nus tempor" U8_A "rio ao personagem ao ser usada.";
	if (item.SubCategoryID == 1)
		return "Item de aprimoramento. O nome indica o efeito e a dura" U8_C U8_AN "o ap" U8_O "s o uso.";
	return "Item premium. Use no invent" U8_A "rio para ativar o efeito descrito no nome.";
}

void NewShop::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;

	if (LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH) && m_titleTex)
		return;

	char exePath[MAX_PATH] = { 0 };
	if (GetModuleFileNameA(nullptr, exePath, MAX_PATH) <= 0)
		return;

	char* slash = strrchr(exePath, '\\');
	if (!slash)
		return;
	slash[1] = 0;

	char full[MAX_PATH] = { 0 };
	sprintf_s(full, sizeof(full), "%s%s", exePath, kTitleImagePath);
	LoadPngTexture(full, &m_titleTex, &m_titleW, &m_titleH);
}

void NewShop::EnsureShopLoaded()
{
	if (m_shopReady)
		return;

	char path[128] = { 0 };
	int tw = 0, th = 0;

	if (!CoinImage)
	{
		sprintf_s(path, sizeof(path), "game\\images\\shop\\coin.png");
		CoinImage = LoadDibSurfaceOffscreen(path);
	}

	if (!ShopUiImages[0])
	{
		sprintf_s(path, sizeof(path), "game\\images\\shop\\buyItem.png");
		LoadTextureFromFile4(path, &ShopUiImages[0], &tw, &th);
	}
	if (!ShopUiImages[1])
	{
		sprintf_s(path, sizeof(path), "game\\images\\shop\\Restaure.png");
		LoadTextureFromFile4(path, &ShopUiImages[1], &tw, &th);
	}
	if (!ShopUiImages[2])
	{
		sprintf_s(path, sizeof(path), "game\\images\\shop\\Vip.png");
		LoadTextureFromFile4(path, &ShopUiImages[2], &tw, &th);
	}
	if (!ShopUiImages[3])
	{
		sprintf_s(path, sizeof(path), "game\\images\\shop\\Rename.png");
		LoadTextureFromFile4(path, &ShopUiImages[3], &tw, &th);
	}

	if (!ClassImages[0])
	{
		for (int x = 0; x <= 10; x++)
		{
			sprintf_s(path, sizeof(path), "game\\images\\shop\\Classes\\%d.png", x);
			LoadTextureFromFile4(path, &ClassImages[x], &tw, &th);
		}
	}

	ReleaseShopIcons();
	ItemShopImage.resize(ShopItems.size(), nullptr);
	ItemShopImageW.resize(ShopItems.size(), 0);
	ItemShopImageH.resize(ShopItems.size(), 0);
	for (int i = 0; i < (int)ShopItems.size(); i++)
	{
		ShopItems[i].imgPosition = i;
		ItemShopImage[i] = LoadItemBmpWithFallback(ShopItemFolder(ShopItems[i]), ShopItems[i].ItemCode, 0);
		if (ItemShopImage[i])
		{
			ItemShopImageW[i] = g_DibLastReadWidth;
			ItemShopImageH[i] = g_DibLastReadHeight;
		}
	}

	for (int x = 0; x < 20; x++)
	{
		if (!failedItems.FailedItems[x].ItemHead)
			continue;

		failedItems.FailedItems[x].imgPosition = x;
		const char c0 = failedItems.FailedItems[x].ItemCode[0];
		const char c1 = failedItems.FailedItems[x].ItemCode[1];
		if (c0 == 'D' && c1 == 'A')
			sprintf_s(path, sizeof(path), "image\\Sinimage\\Items\\Defense\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
		else if (c0 == 'W')
			sprintf_s(path, sizeof(path), "image\\Sinimage\\Items\\Weapon\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
		else if (c0 == 'B' && c1 == 'I')
			sprintf_s(path, sizeof(path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
		else
			sprintf_s(path, sizeof(path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
		if (RestaureImage[x])
		{
			RestaureImage[x]->Release();
			RestaureImage[x] = nullptr;
		}
		RestaureImageW[x] = 0;
		RestaureImageH[x] = 0;
		RestaureImage[x] = LoadDibSurfaceOffscreen(path);
		if (RestaureImage[x])
		{
			RestaureImageW[x] = g_DibLastReadWidth;
			RestaureImageH[x] = g_DibLastReadHeight;
		}
	}

	if (lpCurPlayer)
		selectedClass = lpCurPlayer->smCharInfo.JOB_CODE;

	m_shopReady = true;
}

void NewShop::PushWindowStyle()
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

void NewShop::PopWindowStyle()
{
	ImGui::PopStyleColor(22);
	ImGui::PopStyleVar(10);
}

void NewShop::DrawWindowChrome(float headerH)
{
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void NewShop::DrawConfirmChrome()
{
	DrawWindowChrome(34.0f);
}

void NewShop::DrawSectionHeader(const char* title)
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

void NewShop::DrawTitleHeader(bool* p_open)
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
		const char* title = "LOJA DE COINS";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + 16.0f), kGoldBright, title);
	}

	char coins[48] = { 0 };
	sprintf_s(coins, sizeof(coins), "%d Coins", GetCoin());
	const ImVec2 coinTs = ImGui::CalcTextSize(coins);
	const float coinX = p0.x + size.x - kHeaderBtnW - 24.0f - coinTs.x - 28.0f;
	const float coinY = p0.y + (kMainHeaderH - coinTs.y) * 0.5f;
	if (CoinImage)
		draw->AddImage((ImTextureID)CoinImage, ImVec2(coinX - 22.0f, coinY - 4.0f), ImVec2(coinX - 2.0f, coinY + 16.0f));
	draw->AddText(ImVec2(coinX, coinY), kGoldBright, coins);

	if (p_open && DrawHeaderClose())
		*p_open = false;
}

void NewShop::DrawItemList()
{
	DrawSectionHeader("ITENS");

	int shown = 0;
	for (int i = 0; i < (int)ShopItems.size(); i++)
	{
		const ItemsByCategory& item = ShopItems[i];
		if (item.CategoryID != m_category || item.SubCategoryID != m_subTab)
			continue;

		shown++;
		ImGui::PushID(i);
		RequestPreview(item.ItemCode);

		const bool selected = (m_selectedIndex == i);
		if (ImGui::Selectable("##shopitem", selected, 0, ImVec2(0.0f, 60.0f)))
		{
			m_selectedIndex = i;
			RequestPreview(item.ItemCode);
		}

		const ImVec2 rectMin = ImGui::GetItemRectMin();
		const ImVec2 rectMax = ImGui::GetItemRectMax();
		ImDrawList* draw = ImGui::GetWindowDrawList();
		if (selected)
			draw->AddRect(rectMin, rectMax, kGold, 3.0f, 0, 1.2f);

		LPDIRECT3DTEXTURE9 icon = nullptr;
		int iconW = 0, iconH = 0;
		if (item.imgPosition >= 0 && item.imgPosition < (int)ItemShopImage.size())
		{
			icon = ItemShopImage[item.imgPosition];
			iconW = ItemShopImageW[item.imgPosition];
			iconH = ItemShopImageH[item.imgPosition];
		}

		const float iconBox = 44.0f;
		const float iconX = rectMin.x + 8.0f;
		const float iconY = rectMin.y + (rectMax.y - rectMin.y - iconBox) * 0.5f;
		DrawTextureInBox(draw, icon, ImVec2(iconX, iconY), iconBox, iconBox, iconW, iconH);

		const std::string name = ToUtf8(item.ItemName);
		const float textX = rectMin.x + 62.0f;
		draw->AddText(ImVec2(textX, rectMin.y + 8.0f), IM_COL32(255, 255, 255, 255), name.c_str());

		char price[32] = { 0 };
		sprintf_s(price, sizeof(price), "%d Coins", item.Price);
		draw->AddText(ImVec2(textX, rectMin.y + 26.0f), kGoldBright, price);

		if (const sITEM* prev = FindPreview(item.ItemCode))
		{
			if (prev->sItemInfo.Level > 0)
			{
				char level[24] = { 0 };
				sprintf_s(level, sizeof(level), "Nv. %d", prev->sItemInfo.Level);
				const ImVec2 ls = ImGui::CalcTextSize(level);
				const bool unmet = lpCurPlayer && lpCurPlayer->smCharInfo.Level < prev->sItemInfo.Level;
				draw->AddText(ImVec2(rectMax.x - ls.x - 10.0f, rectMin.y + 26.0f),
					unmet ? IM_COL32(210, 80, 80, 255) : IM_COL32(180, 180, 190, 255), level);
			}
		}

		ImGui::PopID();
	}

	if (shown == 0)
		ImGui::TextDisabled("Nenhum item nesta aba.");
}

void NewShop::DrawItemDetail()
{
	DrawSectionHeader("DETALHES");

	const ItemsByCategory* item = SelectedItem();
	if (!item)
	{
		ImGui::TextDisabled("Selecione um item na lista.");
		return;
	}

	RequestPreview(item->ItemCode);
	const sITEM* preview = FindPreview(item->ItemCode);

	LPDIRECT3DTEXTURE9 icon = nullptr;
	int iconW = 0, iconH = 0;
	if (item->imgPosition >= 0 && item->imgPosition < (int)ItemShopImage.size())
	{
		icon = ItemShopImage[item->imgPosition];
		iconW = ItemShopImageW[item->imgPosition];
		iconH = ItemShopImageH[item->imgPosition];
	}

	const ImVec2 iconPos = ImGui::GetCursorScreenPos();
	ImGui::Dummy(ImVec2(48.0f, 48.0f));
	DrawTextureInBox(ImGui::GetWindowDrawList(), icon, iconPos, 48.0f, 48.0f, iconW, iconH);

	ImGui::SameLine();
	ImGui::BeginGroup();
	const std::string name = ToUtf8(item->ItemName);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.43f, 1.0f));
	ImGui::TextWrapped("%s", name.c_str());
	ImGui::PopStyleColor();
	ImGui::Text("%d Coins", item->Price);
	ImGui::EndGroup();

	ImGui::Spacing();

	if (item->CategoryID != 5)
	{
		const char* combo_label = Classes[selectedClass];
		if (ImGui::BeginCombo("Classe (SPEC)", combo_label, ImGuiComboFlags_NoArrowButton))
		{
			for (int n = 0; n < IM_ARRAYSIZE(Classes); n++)
			{
				const bool is_selected = (selectedClass == n);
				if (ImGui::Selectable(Classes[n], is_selected))
				{
					selectedClass = n;
					ItemsAlreadySent.clear();
					ItemsDetails.clear();
					RequestPreview(item->ItemCode);
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
	}

	const bool canBuy = GetCoin() >= item->Price;
	const float buyH = 34.0f;
	const float extra = canBuy ? 8.0f : 28.0f;
	const float statsH = ImGui::GetContentRegionAvail().y - buyH - extra;
	ImGui::BeginChild("##ShopStats", ImVec2(0.0f, statsH > 60.0f ? statsH : 60.0f), true);
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.86f, 0.78f, 1.0f));
	if (item->CategoryID == 5)
	{
		ImGui::TextWrapped("%s", PremiumDescription(*item).c_str());
	}
	else if (preview)
	{
		DrawShopItemStats(preview);
	}
	else
	{
		ImGui::TextDisabled("Carregando informa" U8_C U8_ON "es do item...");
	}
	ImGui::PopStyleColor();
	ImGui::EndChild();

	if (!canBuy)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.32f, 0.32f, 1.0f));
		ImGui::TextUnformatted("Coins insuficientes.");
		ImGui::PopStyleColor();
	}

	if (ImGui::Button("COMPRAR", ImVec2(-1.0f, buyH)))
		m_confirmBuy = true;
}

void NewShop::DrawShopBody()
{
	const ShopCategory* cat = FindCategory(m_category);
	if (m_category == 6)
	{
		DrawServicePanel();
		return;
	}

	if (ImGui::BeginTabBar("##ShopSubTabs", ImGuiTabBarFlags_FittingPolicyScroll))
	{
		for (int i = 0; i < cat->subCount; i++)
		{
			if (ImGui::BeginTabItem(cat->subs[i].label))
			{
				DrawActiveTabOrnament();
				if (m_subTab != cat->subs[i].id)
				{
					m_subTab = cat->subs[i].id;
					m_selectedIndex = -1;
				}
				EnsureSelection();

				ImGui::BeginChild("##ShopList", ImVec2(300.0f, 0.0f), false);
				DrawItemList();
				ImGui::EndChild();

				ImGui::SameLine();
				DrawPlayerColumnSplit();
				ImGui::BeginChild("##ShopDetail", ImVec2(0.0f, 0.0f), false);
				DrawItemDetail();
				ImGui::EndChild();

				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
	}
}

void NewShop::DrawBuyConfirm()
{
	if (!m_confirmBuy)
		return;

	const ItemsByCategory* item = SelectedItem();
	if (!item)
	{
		m_confirmBuy = false;
		return;
	}

	ImGui::OpenPopup("##ShopBuyConfirm");
	ImGui::SetNextWindowSize(ImVec2(360.0f, 196.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##ShopBuyConfirm", nullptr, flags))
	{
		DrawConfirmChrome();
		ImGui::SetCursorPosY(11.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted("Confirmar compra?");
		ImGui::PopStyleColor();

		ImGui::SetCursorPosY(48.0f);
		const std::string name = ToUtf8(item->ItemName);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.70f, 1.0f));
		CenterTextUnformatted(name.c_str());
		ImGui::PopStyleColor();

		char line[80] = { 0 };
		if (item->CategoryID == 5)
			sprintf_s(line, sizeof(line), "Total: %d Coins", item->Price);
		else
			sprintf_s(line, sizeof(line), "Classe: %s   |   %d Coins", Classes[selectedClass], item->Price);
		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted(line);
		ImGui::PopStyleColor();

		const float btnW = 128.0f;
		const float gap = 14.0f;
		ImGui::SetCursorPos(ImVec2((360.0f - (btnW * 2.0f + gap)) * 0.5f, 148.0f));

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_confirmBuy = false;
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
			SentPurchaseToServer(*item, item->CategoryID == 5 ? 0 : selectedClass);
			m_confirmBuy = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void NewShop::DrawVipConfirm()
{
	if (!m_confirmVip)
		return;

	ImGui::OpenPopup("##ShopVipConfirm");
	ImGui::SetNextWindowSize(ImVec2(360.0f, 176.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##ShopVipConfirm", nullptr, flags))
	{
		DrawConfirmChrome();
		ImGui::SetCursorPosY(11.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted("Ativar VIP 30 dias?");
		ImGui::PopStyleColor();
		ImGui::SetCursorPosY(56.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted("Valor: 2000 Coins");
		ImGui::PopStyleColor();

		const float btnW = 128.0f;
		const float gap = 14.0f;
		ImGui::SetCursorPos(ImVec2((360.0f - (btnW * 2.0f + gap)) * 0.5f, 128.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_confirmVip = false;
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
			ItemsByCategory vip = {};
			vip.CategoryID = 6;
			vip.SubCategoryID = 100;
			sprintf_s(vip.ItemName, sizeof(vip.ItemName), "VIP 30 Dias");
			sprintf_s(vip.ItemCode, sizeof(vip.ItemCode), "VP101");
			vip.Price = 2000;
			SentPurchaseToServer(vip, 0);
			m_confirmVip = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void NewShop::DrawRestaureConfirm()
{
	if (m_confirmRestaure < 0 || m_confirmRestaure >= 20)
		return;

	FailedItems& item = failedItems.FailedItems[m_confirmRestaure];
	if (!item.ItemHead)
	{
		m_confirmRestaure = -1;
		return;
	}

	ImGui::OpenPopup("##ShopRestaureConfirm");
	ImGui::SetNextWindowSize(ImVec2(360.0f, 196.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##ShopRestaureConfirm", nullptr, flags))
	{
		DrawConfirmChrome();
		ImGui::SetCursorPosY(11.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted("Restaurar item?");
		ImGui::PopStyleColor();

		const std::string name = ToUtf8(item.ItemName);
		ImGui::SetCursorPosY(48.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.70f, 1.0f));
		CenterTextUnformatted(name.c_str());
		ImGui::PopStyleColor();

		char line[80] = { 0 };
		sprintf_s(line, sizeof(line), "Aging +%d   |   %d Coins", item.AgingNum, failedItems.price);
		ImGui::Dummy(ImVec2(0.0f, 4.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted(line);
		ImGui::PopStyleColor();

		const float btnW = 128.0f;
		const float gap = 14.0f;
		ImGui::SetCursorPos(ImVec2((360.0f - (btnW * 2.0f + gap)) * 0.5f, 148.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_confirmRestaure = -1;
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
			RecoverItem(item.ItemHead, item.ItemChkSum);
			m_confirmRestaure = -1;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void NewShop::DrawNickConfirm()
{
	if (!m_confirmNick)
		return;

	ImGui::OpenPopup("##ShopNickConfirm");
	ImGui::SetNextWindowSize(ImVec2(380.0f, 210.0f), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)smScreenWidth * 0.5f, (float)smScreenHeight * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 10.0f));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, ImVec4(0.0f, 0.0f, 0.0f, 0.80f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.78f, 0.67f, 0.35f, 0.80f));

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings;

	if (ImGui::BeginPopupModal("##ShopNickConfirm", nullptr, flags))
	{
		DrawConfirmChrome();
		ImGui::SetCursorPosY(11.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.84f, 0.48f, 1.0f));
		CenterTextUnformatted("Confirmar troca de nick?");
		ImGui::PopStyleColor();

		char line[80] = { 0 };
		sprintf_s(line, sizeof(line), "%s  >  %s", lpCurPlayer ? lpCurPlayer->smCharInfo.szName : "?", NewNick);
		ImGui::SetCursorPosY(50.0f);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.70f, 1.0f));
		CenterTextUnformatted(line);
		ImGui::PopStyleColor();
		ImGui::Dummy(ImVec2(0.0f, 6.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
		CenterTextUnformatted("5000 Coins  |  Voc" U8_E " ser" U8_A " desconectado.");
		ImGui::PopStyleColor();

		const float btnW = 128.0f;
		const float gap = 14.0f;
		ImGui::SetCursorPos(ImVec2((380.0f - (btnW * 2.0f + gap)) * 0.5f, 162.0f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.16f, 0.15f, 0.12f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.30f, 0.26f, 0.14f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.38f, 0.32f, 0.16f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.86f, 0.74f, 0.40f, 0.95f));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.88f, 0.62f, 1.0f));
		if (ImGui::Button("VOLTAR", ImVec2(btnW, 30.0f)))
		{
			m_confirmNick = false;
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
			ItemsByCategory change = {};
			change.CategoryID = 6;
			change.SubCategoryID = 101;
			sprintf_s(change.ItemName, sizeof(change.ItemName), "%s", NewNick);
			sprintf_s(change.ItemCode, sizeof(change.ItemCode), "SR101");
			change.Price = 5000;
			SentChangeNickToServer(change, 0);
			m_confirmNick = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::PopStyleColor(5);
		ImGui::EndPopup();
	}

	ImGui::PopStyleColor(3);
	ImGui::PopStyleVar(2);
}

void NewShop::RestaureItems()
{
	DrawSectionHeader("RESTAURAR");
	ImGui::TextWrapped("Itens quebrados no Aging. A restaura" U8_C U8_AN "o cobra Coins e devolve o item.");
	ImGui::Spacing();

	int shown = 0;
	for (int x = 0; x < 20; x++)
	{
		if (!failedItems.FailedItems[x].ItemHead)
			continue;

		shown++;
		FailedItems& item = failedItems.FailedItems[x];
		ImGui::PushID(x);
		if (ImGui::Selectable("##rest", false, 0, ImVec2(0.0f, 56.0f)))
			m_confirmRestaure = x;

		if (ImGui::IsItemHovered() && m_confirmRestaure < 0)
		{
			SendItemToServer(item.ItemCode, item.ItemHead, item.ItemChkSum, true, item.AgingNum);
			if (bIsPerfect && TempPerfectItem.sItemInfo.ItemHeader.Head == item.ItemHead)
			{
				PlacePreviewNearMouse();
				m_hoverPreview = true;
			}
		}

		const ImVec2 rectMin = ImGui::GetItemRectMin();
		ImDrawList* draw = ImGui::GetWindowDrawList();
		DrawTextureInBox(draw, RestaureImage[item.imgPosition], ImVec2(rectMin.x + 6.0f, rectMin.y + 4.0f), 44.0f, 44.0f,
			RestaureImageW[item.imgPosition], RestaureImageH[item.imgPosition]);

		const std::string name = ToUtf8(item.ItemName);
		draw->AddText(ImVec2(rectMin.x + 62.0f, rectMin.y + 6.0f), IM_COL32(255, 255, 255, 255), name.c_str());
		char line[80] = { 0 };
		sprintf_s(line, sizeof(line), "Aging +%d   %d Coins   %02d/%02d/%04d",
			item.AgingNum, failedItems.price, item.Date.wDay, item.Date.wMonth, item.Date.wYear);
		draw->AddText(ImVec2(rectMin.x + 62.0f, rectMin.y + 28.0f), kGoldBright, line);
		ImGui::PopID();
	}

	if (shown == 0)
		ImGui::TextDisabled("Nenhum item quebrado para restaurar.");
}

void NewShop::LoadVipOptions()
{
	DrawSectionHeader("VIP");
	if (ShopUiImages[2])
	{
		ImGui::Image((ImTextureID)ShopUiImages[2], ImVec2(48.0f, 48.0f));
		ImGui::SameLine();
	}
	ImGui::BeginGroup();
	ImGui::TextUnformatted(u8"VIP 30 dias");
	ImGui::Text("2000 Coins");
	ImGui::EndGroup();
	ImGui::Spacing();
	ImGui::TextWrapped(u8"Vantagens:");
	ImGui::BulletText(u8"/repot abre a Loja da Allya");
	ImGui::BulletText(u8"/premium abre o Negociante Mughy");
	ImGui::BulletText("+10%% de experi" U8_E "ncia dos monstros");
	ImGui::Spacing();
	if (ImGui::Button("COMPRAR VIP", ImVec2(-1.0f, 34.0f)))
		m_confirmVip = true;
}

int NewShop::TextEditCallback2(ImGuiInputTextCallbackData* data)
{
	(void)data;
	return 0;
}

static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	ImWchar c = data->EventChar;
	if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
		return 1;
	NewShop* shop = (NewShop*)data->UserData;
	return shop->TextEditCallback2(data);
}

void NewShop::ChangeNick()
{
	editingNick = true;
	DrawSectionHeader("TROCA DE NICK");
	ImGui::TextWrapped(u8"O chat fica bloqueado enquanto esta aba estiver aberta.");
	ImGui::Text("Custo: 5000 Coins");
	ImGui::Spacing();

	if (ImGui::InputText("Nick", NewNick, IM_ARRAYSIZE(NewNick),
		ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackCharFilter,
		&TextEditCallbackStub, (void*)this))
	{
		nickIsAvailable = 0;
	}

	if (ImGui::Button(u8"Verificar disponibilidade", ImVec2(-1.0f, 30.0f)))
	{
		if (NewNick[0])
		{
			ItemsByCategory check = {};
			check.CategoryID = 6;
			check.SubCategoryID = 101;
			sprintf_s(check.ItemName, sizeof(check.ItemName), "%s", NewNick);
			sprintf_s(check.ItemCode, sizeof(check.ItemCode), "SR101");
			check.Price = 5000;
			SentCheckNickToServer(check, 0);
		}
	}

	if (nickIsAvailable == 1)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.28f, 0.72f, 0.36f, 1.0f));
		ImGui::TextUnformatted("Nome dispon" U8_I "vel.");
		ImGui::PopStyleColor();
		if (ImGui::Button("CONFIRMAR TROCA", ImVec2(-1.0f, 34.0f)))
			m_confirmNick = true;
	}
	else if (nickIsAvailable == 2)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.32f, 0.32f, 1.0f));
		ImGui::TextUnformatted("Este nome j" U8_A " est" U8_A " em uso.");
		ImGui::PopStyleColor();
	}
}

void NewShop::ChangeClass()
{
	DrawSectionHeader("TROCA DE CLASSE");
	ImGui::TextWrapped("Nenhum item " U8_E " trocado junto com a classe.");
	ImGui::Text("Custo: 10000 Coins");
}

void NewShop::DrawServicePanel()
{
	const ShopCategory* cat = FindCategory(6);
	if (!ImGui::BeginTabBar("##ShopServiceTabs", ImGuiTabBarFlags_None))
		return;

	if (ImGui::BeginTabItem(cat->subs[0].label))
	{
		DrawActiveTabOrnament();
		m_subTab = 1;
		editingNick = false;
		ImGui::BeginChild("##ServiceRest", ImVec2(0.0f, 0.0f), false);
		RestaureItems();
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(cat->subs[1].label))
	{
		DrawActiveTabOrnament();
		m_subTab = 2;
		editingNick = false;
		ImGui::BeginChild("##ServiceVip", ImVec2(0.0f, 0.0f), false);
		LoadVipOptions();
		ImGui::EndChild();
		ImGui::EndTabItem();
	}
	if (ImGui::BeginTabItem(cat->subs[2].label))
	{
		DrawActiveTabOrnament();
		m_subTab = 3;
		ImGui::BeginChild("##ServiceNick", ImVec2(0.0f, 0.0f), false);
		ChangeNick();
		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	ImGui::EndTabBar();
}

void NewShop::OpenNpc(bool* p_open)
{
	EnsureShopLoaded();
	m_hoverPreview = false;
	editingNick = false;

	ImGui::SetNextWindowSize(ImVec2(kWindowW, kWindowH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2((float)(smScreenWidth / 2) - (kWindowW * 0.5f), (float)(smScreenHeight / 2) - (kWindowH * 0.5f)));

	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##LojaDeCoins", p_open, flags);
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

		if (ImGui::BeginTabBar("##ShopCats", ImGuiTabBarFlags_FittingPolicyScroll))
		{
			for (int i = 0; i < kCategoryCount; i++)
			{
				if (ImGui::BeginTabItem(kCategories[i].label))
				{
					DrawActiveTabOrnament();
					if (m_category != kCategories[i].id)
					{
						m_category = kCategories[i].id;
						m_subTab = (m_category == 6) ? 1 : kCategories[i].subs[0].id;
						m_selectedIndex = -1;
					}
					DrawShopBody();
					ImGui::EndTabItem();
				}
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	DrawBuyConfirm();
	DrawVipConfirm();
	DrawRestaureConfirm();
	DrawNickConfirm();

	if (m_hoverPreview && !m_confirmBuy && !m_confirmVip && m_confirmRestaure < 0 && !m_confirmNick)
	{
		if (smScreenWidth > 800 && smScreenHeight > 600)
		{
			sinShowItemInfoFlag = 1;
			if (GAMECOREHANDLE && GAMECOREHANDLE->pcItemInfoBox)
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&TempPerfectItem, FALSE, FALSE, FALSE);
		}
	}

	PopWindowStyle();
}