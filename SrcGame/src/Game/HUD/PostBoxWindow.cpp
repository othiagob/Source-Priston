#include "PostBoxWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "ImGuiWindowChrome.h"
#include "imGui/imgui.h"

#include <d3dx9tex.h>
#include <time.h>

#define U8_A "\xC3\xA1"
#define U8_E "\xC3\xA9"
#define U8_I "\xC3\xAD"
#define U8_O "\xC3\xB3"
#define U8_AN "\xC3\xA3"
#define U8_C "\xC3\xA7"
#define U8_A_UP "\xC3\x81"

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);
static const char* kTitleImagePath = "game\\images\\postbox\\distribuidor.png";
static const char* kFrameImagePath = "game\\images\\postbox\\frame.png";
static const float kMainHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;
static const float kWindowW = 760.0f;
static const float kWindowH = 540.0f;
static const float kFrameRim = 24.0f;

extern int smScreenWidth;
extern int smScreenHeight;
extern int SendPostBoxOpen();
extern int SendPostBoxClaim(DWORD dwEntryId, char* szPassCode);
extern int SendPostBoxRefuse(DWORD dwEntryId);
extern int SendPostBoxSend(const char* szDestName, sITEMINFO* lpItem);

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
	const float y1 = b.y - 2.0f;
	const float y2 = b.y + 1.0f;
	const float rounding = 3.0f;
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->PathClear();
	draw->PathLineTo(ImVec2(a.x + 0.5f, y2));
	draw->PathArcToFast(ImVec2(a.x + rounding + 0.5f, y1 + rounding + 0.5f), rounding, 6, 9);
	draw->PathLineTo(ImVec2(b.x - rounding - 0.5f, y1 + 0.5f));
	draw->PathArcToFast(ImVec2(b.x - rounding - 0.5f, y1 + rounding + 0.5f), rounding, 9, 12);
	draw->PathLineTo(ImVec2(b.x - 0.5f, y2));
	draw->PathStroke(kGold, false, 1.5f);
	(void)width;
}

static bool DrawHeaderClose(float rightInset, float topOffset)
{
	const ImVec2 win = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(win.x - kHeaderBtnW - rightInset, topOffset + (kMainHeaderH - kHeaderBtnH) * 0.5f));
	ImGui::InvisibleButton("##PostBoxClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
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

static const char* ItemFolderFromCode(const char* itemCode)
{
	if (!itemCode || !itemCode[0])
		return "Premium";
	const char c0 = (char)toupper((unsigned char)itemCode[0]);
	const char c1 = (char)toupper((unsigned char)itemCode[1]);
	if (c0 == 'W')
		return "Weapon";
	if (c0 == 'D')
		return "Defense";
	if (c0 == 'O')
		return (c1 == 'S' || c1 == 'B') ? "Defense" : "Accessory";
	if ((c0 == 'G' && c1 == 'P') || (c0 == 'S' && c1 == 'P'))
		return "Event";
	if (c0 == 'P')
		return "Potion";
	if (c0 == 'Q')
		return "Quest";
	if (c0 == 'S' && c1 == 'E')
		return "Accessory";
	if (c0 == 'F' && c1 == 'O')
		return "Accessory";
	if (c0 == 'C' && c1 == 'A')
		return "Defense";
	return "Premium";
}

static void FormatTimeLeft(int seconds, char* out, int outSize)
{
	if (seconds < 0)
	{
		strcpy_s(out, outSize, "Sem prazo");
		return;
	}
	if (seconds <= 0)
	{
		strcpy_s(out, outSize, "Expirado");
		return;
	}
	const int days = seconds / 86400;
	const int hours = (seconds % 86400) / 3600;
	const int mins = (seconds % 3600) / 60;
	if (days > 0)
		sprintf_s(out, outSize, "%dd %dh", days, hours);
	else if (hours > 0)
		sprintf_s(out, outSize, "%dh %dmin", hours, mins);
	else
		sprintf_s(out, outSize, "%d min", mins);
}

static void FormatArrivedDate(DWORD depositedAt, char* out, int outSize)
{
	if (!depositedAt)
	{
		strcpy_s(out, outSize, "--");
		return;
	}

	const time_t t = (time_t)depositedAt;
	struct tm local = {};
	if (localtime_s(&local, &t) != 0)
	{
		strcpy_s(out, outSize, "--");
		return;
	}
	sprintf_s(out, outSize, "%02d/%02d/%04d  %02d:%02d",
		local.tm_mday, local.tm_mon + 1, local.tm_year + 1900,
		local.tm_hour, local.tm_min);
}

static void DrawInfoLine(const char* label, const char* value)
{
	ImGui::TextDisabled("%s", label);
	ImGui::SameLine(92.0f);
	ImGui::TextUnformatted(value && value[0] ? value : "--");
}

void PostBoxWindow::SetStatus(const char* utf8)
{
	if (!utf8)
		utf8 = "";
	strcpy_s(m_statusUtf8, utf8);
}

void PostBoxWindow::ReleaseIcons()
{
	for (size_t i = 0; i < m_icons.size(); i++)
	{
		if (m_icons[i])
			((LPDIRECT3DTEXTURE9)m_icons[i])->Release();
	}
	m_icons.clear();
	ReleaseSendIcon();
}

void PostBoxWindow::ReleaseSendIcon()
{
	if (m_sendIcon)
	{
		((LPDIRECT3DTEXTURE9)m_sendIcon)->Release();
		m_sendIcon = nullptr;
	}
	m_sendIconFor = -1;
}

void PostBoxWindow::LoadEntryIcon(int index)
{
	while ((int)m_icons.size() <= index)
		m_icons.push_back(nullptr);
	if (m_icons[index] || index < 0 || index >= (int)m_entries.size())
		return;
	const char* code = m_entries[index].szItemCode;
	if (!code[0])
		return;
	m_icons[index] = LoadItemBmpWithFallback(ItemFolderFromCode(code), code, 0);
}

void PostBoxWindow::Open()
{
	openFlag = true;
	m_selected = -1;
	m_sendSelected = -1;
	ReleaseSendIcon();
	m_pendingClaim = 0;
	m_passUtf8[0] = 0;
	m_destUtf8[0] = 0;
	m_statusUtf8[0] = 0;
	m_entries.clear();
	ReleaseIcons();
	m_expectedChunks = 0;
	m_gotChunks = 0;
}

void PostBoxWindow::Close()
{
	openFlag = false;
	m_passFocus = false;
	m_destFocus = false;
	m_pendingClaim = 0;
	ReleaseSendIcon();
}

void PostBoxWindow::RequestOpen()
{
	SendPostBoxOpen();
}

bool PostBoxWindow::OnEscape()
{
	if (!openFlag)
		return false;
	Close();
	return true;
}

bool PostBoxWindow::ShouldCaptureKeyboard() const
{
	return openFlag && (m_passFocus || m_destFocus);
}

bool PostBoxWindow::IsBlockingMouse(int x, int y) const
{
	if (!openFlag)
		return false;
	return m_winW > 0.0f && m_winH > 0.0f
		&& x >= m_winX && x <= (m_winX + m_winW)
		&& y >= m_winY && y <= (m_winY + m_winH);
}

std::string PostBoxWindow::ToUtf8(const char* src) const
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

const TRANS_POSTBOX_ENTRY* PostBoxWindow::Selected() const
{
	if (m_selected < 0 || m_selected >= (int)m_entries.size())
		return nullptr;
	return &m_entries[m_selected];
}

void PostBoxWindow::ReceiveList(TRANS_POSTBOX_LIST* list)
{
	if (!list)
		return;
	if (list->chunkIndex <= 0)
	{
		m_entries.clear();
		ReleaseIcons();
		m_gotChunks = 0;
		m_expectedChunks = list->totalChunks > 0 ? list->totalChunks : 1;
		if (m_selected >= (int)list->totalEntries)
			m_selected = -1;
	}
	for (int i = 0; i < list->entryCount; i++)
		m_entries.push_back(list->Entries[i]);
	m_gotChunks++;
	if (m_selected < 0 && !m_entries.empty())
		m_selected = 0;
	if (m_selected >= (int)m_entries.size())
		m_selected = m_entries.empty() ? -1 : 0;
}

void PostBoxWindow::ReceiveClaimResult(int result, DWORD)
{
	m_pendingClaim = 0;
	if (result == POSTBOX_RESULT_OK)
		SetStatus("Item recebido.");
	else if (result == POSTBOX_RESULT_PASSFAIL)
		SetStatus("Senha incorreta.");
	else if (result == POSTBOX_RESULT_LOCKED)
		SetStatus("Muitas tentativas de senha. Fale com o GM.");
	else if (result == POSTBOX_RESULT_NOTFOUND)
		SetStatus("Item n" U8_AN "o est" U8_A " mais na lista.");
	else
		SetStatus("N" U8_AN "o foi poss" U8_I "vel receber o item.");
}

void PostBoxWindow::ReceiveRefuseResult(int result, DWORD)
{
	if (result == POSTBOX_RESULT_REFUSED)
		SetStatus("Item recusado.");
	else
		SetStatus("N" U8_AN "o foi poss" U8_I "vel recusar.");
}

void PostBoxWindow::RemoveInventoryItem(DWORD code, DWORD head, DWORD chkSum)
{
	for (int i = 0; i < INVENTORY_MAXITEM; i++)
	{
		sITEM* it = &cInvenTory.InvenItem[i];
		if (!it->Flag)
			continue;
		if (it->sItemInfo.CODE != code)
			continue;
		const int isPotion = ((code & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1));
		if (!isPotion && (it->sItemInfo.ItemHeader.Head != head || it->sItemInfo.ItemHeader.dwChkSum != chkSum))
			continue;
		it->Flag = 0;
		ZeroMemory(it, sizeof(sITEM));
		break;
	}
	ResetInvenItemCode();
	SaveGameData();
}

void PostBoxWindow::ReceiveSendResult(int result, DWORD code, DWORD head, DWORD chkSum)
{
	if (result == POSTBOX_RESULT_OK)
	{
		RemoveInventoryItem(code, head, chkSum);
		SetStatus("Item enviado.");
		m_sendSelected = -1;
		ReleaseSendIcon();
	}
	else if (result == POSTBOX_RESULT_SELF)
		SetStatus("N" U8_AN "o d" U8_A " para enviar para o mesmo personagem.");
	else if (result == POSTBOX_RESULT_DEST)
		SetStatus("Destino n" U8_AN "o encontrado.");
	else if (result == POSTBOX_RESULT_FULLBOX)
		SetStatus("A caixa do destino est" U8_A " cheia.");
	else if (result == POSTBOX_RESULT_ITEM)
		SetStatus("Este item n" U8_AN "o pode ser enviado.");
	else
		SetStatus("Falha ao enviar.");
}

bool PostBoxWindow::CanSendInventoryItem(int invenIndex) const
{
	if (invenIndex < 0 || invenIndex >= INVENTORY_MAXITEM)
		return false;
	const sITEM* it = &cInvenTory.InvenItem[invenIndex];
	if (!it->Flag || !it->sItemInfo.CODE)
		return false;
	if (it->ItemPosition)
		return false;
	if (it->sItemInfo.ItemKindCode == ITEM_KIND_QUEST || it->sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON)
		return false;
	if ((it->sItemInfo.CODE & sinITEM_MASK2) == sinQT1)
		return false;
	if (it->sItemInfo.UniqueItem)
		return false;
	if (it->sItemInfo.SpecialItemFlag[0] == CHECK_COPY_ITEM)
		return false;
	if (it->CODE == (sinGG1 | sin01) || it->sItemInfo.CODE == (sinGG1 | sin01))
		return false;
	if (MouseItem.Flag && MouseItem.sItemInfo.ItemHeader.Head == it->sItemInfo.ItemHeader.Head
		&& MouseItem.sItemInfo.ItemHeader.dwChkSum == it->sItemInfo.ItemHeader.dwChkSum)
		return false;
	return true;
}

void PostBoxWindow::RequestClaim()
{
	const TRANS_POSTBOX_ENTRY* e = Selected();
	if (!e || m_pendingClaim)
		return;

	if (e->dwItemCode != (sinGG1 | sin01) && e->dwItemCode != (sinGG1 | sin02))
	{
		if (sinChar && e->Weight > 0 && sinChar->Weight[0] + e->Weight > sinChar->Weight[1])
		{
			cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
			SetStatus("Peso excedido. O item continua na lista.");
			return;
		}
		sITEM tmp = {};
		tmp.CODE = e->dwItemCode;
		if (!cInvenTory.CheckInvenEmpty(&tmp))
		{
			cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
			SetStatus("Invent" U8_A "rio cheio. O item continua na lista.");
			return;
		}
	}

	char passAnsi[32] = {};
	if (e->HasPassCode)
	{
		if (!m_passUtf8[0])
		{
			SetStatus("Informe a senha do item.");
			return;
		}
		const int wideLen = MultiByteToWideChar(CP_UTF8, 0, m_passUtf8, -1, nullptr, 0);
		std::wstring wide((size_t)(wideLen > 0 ? wideLen : 1), L'\0');
		if (wideLen > 0)
			MultiByteToWideChar(CP_UTF8, 0, m_passUtf8, -1, &wide[0], wideLen);
		WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, passAnsi, sizeof(passAnsi), nullptr, nullptr);
	}

	m_pendingClaim = e->dwEntryId;
	SendPostBoxClaim(e->dwEntryId, e->HasPassCode ? passAnsi : nullptr);
}

void PostBoxWindow::RequestRefuse()
{
	const TRANS_POSTBOX_ENTRY* e = Selected();
	if (!e)
		return;
	SendPostBoxRefuse(e->dwEntryId);
}

void PostBoxWindow::RequestSend()
{
	if (m_sendSelected < 0 || !CanSendInventoryItem(m_sendSelected))
	{
		SetStatus("Escolha um item do invent" U8_A "rio.");
		return;
	}
	if (!m_destUtf8[0])
	{
		SetStatus("Informe o nick de destino.");
		return;
	}

	char destAnsi[32] = {};
	const int wideLen = MultiByteToWideChar(CP_UTF8, 0, m_destUtf8, -1, nullptr, 0);
	std::wstring wide((size_t)(wideLen > 0 ? wideLen : 1), L'\0');
	if (wideLen > 0)
		MultiByteToWideChar(CP_UTF8, 0, m_destUtf8, -1, &wide[0], wideLen);
	WideCharToMultiByte(CP_ACP, 0, wide.c_str(), -1, destAnsi, sizeof(destAnsi), nullptr, nullptr);

	if (lpCurPlayer && lstrcmpi(destAnsi, lpCurPlayer->smCharInfo.szName) == 0)
	{
		SetStatus("N" U8_AN "o d" U8_A " para enviar para o mesmo personagem.");
		return;
	}

	SendPostBoxSend(destAnsi, &cInvenTory.InvenItem[m_sendSelected].sItemInfo);
}

void PostBoxWindow::EnsureTitleTexture()
{
	if (m_titleTried)
		return;
	m_titleTried = true;
	LoadPngTexture(kTitleImagePath, &m_titleTex, &m_titleW, &m_titleH);
}

void PostBoxWindow::EnsureFrameTexture()
{
	if (m_frameTried)
		return;
	m_frameTried = true;
	if (!LoadPngTexture(kFrameImagePath, &m_frameTex, &m_frameW, &m_frameH))
		return;
	if (m_frameW != (int)kWindowW || m_frameH != (int)kWindowH)
	{
		if (m_frameTex)
		{
			((LPDIRECT3DTEXTURE9)m_frameTex)->Release();
			m_frameTex = nullptr;
		}
		m_frameW = 0;
		m_frameH = 0;
	}
}

bool PostBoxWindow::HasFrame() const
{
	return m_frameTex != nullptr && m_frameW == (int)kWindowW && m_frameH == (int)kWindowH;
}

float PostBoxWindow::FrameRim() const
{
	return HasFrame() ? kFrameRim : 0.0f;
}

float PostBoxWindow::BodyInsetX() const
{
	return HasFrame() ? (kFrameRim + 4.0f) : kPlayerWindowBodyInset;
}

void PostBoxWindow::PushWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(BodyInsetX(), HasFrame() ? kFrameRim : 12.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 6.0f));
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

void PostBoxWindow::PopWindowStyle()
{
	ImGui::PopStyleColor(22);
	ImGui::PopStyleVar(10);
}

void PostBoxWindow::DrawWindowChrome(float headerH)
{
	EnsureFrameTexture();
	if (HasFrame())
	{
		ImDrawList* draw = ImGui::GetWindowDrawList();
		const ImVec2 p0 = ImGui::GetWindowPos();
		const ImVec2 p1(p0.x + kWindowW, p0.y + kWindowH);
		draw->AddRectFilled(p0, p1, kGoldFill);
		const float rim = kFrameRim;
		const float headerBottom = p0.y + rim + headerH;
		draw->AddRectFilled(
			ImVec2(p0.x + rim, p0.y + rim),
			ImVec2(p1.x - rim, headerBottom),
			kGoldFill);
		draw->AddImage((ImTextureID)m_frameTex, p0, p1);
		return;
	}
	DrawPlayerWindowChrome(ImGui::GetWindowDrawList(), headerH, kGold, kGoldFill);
}

void PostBoxWindow::DrawTitleHeader(bool* p_open)
{
	EnsureTitleTexture();

	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	const float rim = FrameRim();
	const float headerTop = p0.y + rim + 6.0f;
	const float headerBot = p0.y + rim + kMainHeaderH - 4.0f;

	if (m_titleTex && m_titleW > 0 && m_titleH > 0)
	{
		const float areaH = headerBot - headerTop;
		const ImVec2 sz = FitImageSize(m_titleW, m_titleH, 300.0f, areaH);
		const float x = p0.x + (size.x - sz.x) * 0.5f;
		const float y = headerTop + (areaH - sz.y) * 0.5f;
		draw->AddImage((ImTextureID)m_titleTex, ImVec2(x, y), ImVec2(x + sz.x, y + sz.y));
	}
	else
	{
		const char* title = "DISTRIBUIDOR";
		const ImVec2 ts = ImGui::CalcTextSize(title);
		draw->AddText(ImVec2(p0.x + (size.x - ts.x) * 0.5f, p0.y + rim + 18.0f), kGoldBright, title);
	}

	const float closeRight = HasFrame() ? (kFrameRim + 8.0f) : 12.0f;
	if (p_open && DrawHeaderClose(closeRight, rim))
		*p_open = false;
}

void PostBoxWindow::DrawSectionHeader(const char* title)
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

void PostBoxWindow::DrawDetail()
{
	const TRANS_POSTBOX_ENTRY* e = Selected();
	DrawSectionHeader("DETALHE");
	if (!e)
	{
		ImGui::TextDisabled("Selecione um item na lista.");
		return;
	}

	LoadEntryIcon(m_selected);
	ImGui::BeginGroup();
	if (m_selected >= 0 && m_selected < (int)m_icons.size() && m_icons[m_selected])
	{
		ImGui::Image((ImTextureID)m_icons[m_selected], ImVec2(56.0f, 56.0f));
		ImGui::SameLine();
	}

	ImGui::BeginGroup();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.43f, 1.0f));
	ImGui::TextUnformatted(ToUtf8(e->szItemName).c_str());
	ImGui::PopStyleColor();
	ImGui::TextDisabled("%s", e->nKind == POSTBOX_KIND_PLAYER ? "Correio de jogador" : "Recompensa do sistema");
	ImGui::EndGroup();
	ImGui::EndGroup();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	char arrived[40] = {};
	FormatArrivedDate(e->dwDepositedAt, arrived, sizeof(arrived));
	char timeBuf[32] = {};
	FormatTimeLeft(e->SecondsLeft, timeBuf, sizeof(timeBuf));
	char weightBuf[24] = {};
	if (e->Weight > 0)
		sprintf_s(weightBuf, "%d", e->Weight);
	else
		strcpy_s(weightBuf, "--");

	DrawInfoLine("De", ToUtf8(e->szSenderName).c_str());
	DrawInfoLine("Chegou", arrived);
	DrawInfoLine("Prazo", timeBuf);
	DrawInfoLine("Peso", weightBuf);

	if (e->szDoc[0])
	{
		ImGui::Spacing();
		ImGui::TextDisabled("Mensagem");
		ImGui::TextWrapped("%s", ToUtf8(e->szDoc).c_str());
	}

	ImGui::Spacing();
	if (e->HasPassCode)
	{
		ImGui::TextUnformatted("Senha");
		ImGui::SetNextItemWidth(180.0f);
		ImGui::InputText("##PostPass", m_passUtf8, sizeof(m_passUtf8), ImGuiInputTextFlags_Password);
		m_passFocus = ImGui::IsItemActive();
		ImGui::Spacing();
	}

	if (ImGui::Button("ACEITAR", ImVec2(128.0f, 28.0f)))
		RequestClaim();
	ImGui::SameLine();
	if (ImGui::Button("RECUSAR", ImVec2(128.0f, 28.0f)))
		RequestRefuse();
}

void PostBoxWindow::DrawReceiveTab()
{
	ImGui::BeginChild("##PostBoxList", ImVec2(268.0f, 0.0f), false);
	DrawSectionHeader("CAIXA");
	if (m_entries.empty())
	{
		ImGui::TextDisabled("Nenhum item pendente.");
	}
	else
	{
		for (int i = 0; i < (int)m_entries.size(); i++)
		{
			LoadEntryIcon(i);
			ImGui::PushID(i);
			const bool selected = (m_selected == i);
			if (ImGui::Selectable("##pbrow", selected, 0, ImVec2(0.0f, 48.0f)))
				m_selected = i;

			const ImVec2 min = ImGui::GetItemRectMin();
			const ImVec2 max = ImGui::GetItemRectMax();
			ImDrawList* draw = ImGui::GetWindowDrawList();
			if (i < (int)m_icons.size() && m_icons[i])
				draw->AddImage((ImTextureID)m_icons[i], ImVec2(min.x + 6.0f, min.y + 6.0f), ImVec2(min.x + 42.0f, min.y + 42.0f));

			const float textX = min.x + 50.0f;
			draw->AddText(ImVec2(textX, min.y + 6.0f),
				selected ? kGoldBright : IM_COL32(255, 255, 255, 255),
				ToUtf8(m_entries[i].szItemName).c_str());

			char timeBuf[32] = {};
			FormatTimeLeft(m_entries[i].SecondsLeft, timeBuf, sizeof(timeBuf));
			char sub[96] = {};
			sprintf_s(sub, "%s  ·  %s", ToUtf8(m_entries[i].szSenderName).c_str(), timeBuf);
			draw->AddText(ImVec2(textX, min.y + 26.0f), IM_COL32(158, 158, 173, 255), sub);
			(void)max;
			ImGui::PopID();
		}
	}
	ImGui::EndChild();

	ImGui::SameLine();
	DrawPlayerColumnSplit();
	ImGui::BeginChild("##PostBoxDetail", ImVec2(0.0f, 0.0f), false);
	DrawDetail();
	if (m_statusUtf8[0])
	{
		ImGui::Spacing();
		ImGui::TextWrapped("%s", m_statusUtf8);
	}
	ImGui::EndChild();
}

void PostBoxWindow::DrawSendPreview()
{
	DrawSectionHeader("ITEM");
	if (m_sendSelected < 0 || !CanSendInventoryItem(m_sendSelected))
	{
		ImGui::TextDisabled("Selecione um item na lista.");
		return;
	}

	sITEM* it = &cInvenTory.InvenItem[m_sendSelected];
	void* tex = it->lpItem;
	if (!tex)
	{
		if (m_sendIconFor != m_sendSelected)
		{
			ReleaseSendIcon();
			m_sendIcon = LoadItemBmpWithFallback(ItemFolderFromCode(it->LastCategory), it->LastCategory, it->Class);
			m_sendIconFor = m_sendSelected;
		}
		tex = m_sendIcon;
	}

	if (tex)
	{
		ImGui::Image((ImTextureID)tex, ImVec2(64.0f, 64.0f));
		ImGui::SameLine();
	}

	ImGui::BeginGroup();
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.90f, 0.78f, 0.43f, 1.0f));
	ImGui::TextUnformatted(ToUtf8(it->sItemInfo.ItemName).c_str());
	ImGui::PopStyleColor();
	if (it->LastCategory[0])
		ImGui::TextDisabled("Codigo: %s", it->LastCategory);
	ImGui::EndGroup();

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	char weightBuf[24] = {};
	sprintf_s(weightBuf, "%d", it->sItemInfo.Weight);
	DrawInfoLine("Peso", weightBuf);

	if (it->sItemInfo.Level > 0)
	{
		char levelBuf[24] = {};
		sprintf_s(levelBuf, "%d", it->sItemInfo.Level);
		DrawInfoLine("N" U8_I "vel", levelBuf);
	}

	const int isPotion = ((it->sItemInfo.CODE & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1));
	if (isPotion && it->sItemInfo.PotionCount > 0)
	{
		char qtyBuf[24] = {};
		sprintf_s(qtyBuf, "%d", it->sItemInfo.PotionCount);
		DrawInfoLine("Qtd", qtyBuf);
	}

	if (it->sItemInfo.Durability[1] > 0)
	{
		char durBuf[32] = {};
		sprintf_s(durBuf, "%d / %d", it->sItemInfo.Durability[0], it->sItemInfo.Durability[1]);
		DrawInfoLine("Durab.", durBuf);
	}
}

void PostBoxWindow::DrawSendTab()
{
	ImGui::TextUnformatted("Nick de destino");
	ImGui::SetNextItemWidth(240.0f);
	ImGui::InputText("##PostDest", m_destUtf8, sizeof(m_destUtf8));
	m_destFocus = ImGui::IsItemActive();
	ImGui::SameLine();
	ImGui::TextDisabled("O destino pode estar offline.");

	ImGui::Spacing();
	ImGui::BeginChild("##PostSendList", ImVec2(300.0f, 0.0f), false);
	DrawSectionHeader("INVENT" U8_A_UP "RIO");
	int shown = 0;
	for (int i = 0; i < INVENTORY_MAXITEM; i++)
	{
		if (!CanSendInventoryItem(i))
			continue;
		shown++;
		sITEM* it = &cInvenTory.InvenItem[i];
		ImGui::PushID(i);
		const bool selected = (m_sendSelected == i);
		if (ImGui::Selectable("##sendrow", selected, 0, ImVec2(0.0f, 44.0f)))
		{
			m_sendSelected = i;
			ReleaseSendIcon();
		}

		const ImVec2 min = ImGui::GetItemRectMin();
		ImDrawList* draw = ImGui::GetWindowDrawList();
		if (it->lpItem)
			draw->AddImage((ImTextureID)it->lpItem, ImVec2(min.x + 6.0f, min.y + 4.0f), ImVec2(min.x + 40.0f, min.y + 40.0f));
		draw->AddText(ImVec2(min.x + 48.0f, min.y + 6.0f),
			selected ? kGoldBright : IM_COL32(255, 255, 255, 255),
			ToUtf8(it->sItemInfo.ItemName).c_str());

		char sub[64] = {};
		if (it->sItemInfo.Weight > 0)
			sprintf_s(sub, "Peso %d", it->sItemInfo.Weight);
		else if (it->LastCategory[0])
			strcpy_s(sub, it->LastCategory);
		if (sub[0])
			draw->AddText(ImVec2(min.x + 48.0f, min.y + 24.0f), IM_COL32(158, 158, 173, 255), sub);
		ImGui::PopID();
	}
	if (!shown)
		ImGui::TextDisabled("Nenhum item envi" U8_A "vel no invent" U8_A "rio.");
	ImGui::EndChild();

	ImGui::SameLine();
	DrawPlayerColumnSplit();
	ImGui::BeginChild("##PostSendDetail", ImVec2(0.0f, 0.0f), false);
	DrawSendPreview();
	ImGui::Spacing();
	if (ImGui::Button("ENVIAR", ImVec2(160.0f, 30.0f)))
		RequestSend();
	if (m_statusUtf8[0])
	{
		ImGui::Spacing();
		ImGui::TextWrapped("%s", m_statusUtf8);
	}
	ImGui::EndChild();
}

void PostBoxWindow::OpenNpc(bool* p_open)
{
	if (!p_open || !*p_open)
		return;

	ImGui::SetNextWindowSize(ImVec2(kWindowW, kWindowH), ImGuiCond_Always);
	ImGui::SetNextWindowPos(ImVec2(((float)smScreenWidth - kWindowW) * 0.5f, ((float)smScreenHeight - kWindowH) * 0.5f));

	EnsureFrameTexture();
	PushWindowStyle();

	const ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("##PostBoxWindow", p_open, flags);
	{
		const ImVec2 pos = ImGui::GetWindowPos();
		const ImVec2 size = ImGui::GetWindowSize();
		m_winX = pos.x;
		m_winY = pos.y;
		m_winW = size.x;
		m_winH = size.y;

		DrawWindowChrome(kMainHeaderH);
		DrawTitleHeader(p_open);

		ImGui::SetCursorPos(ImVec2(BodyInsetX(), FrameRim() + kMainHeaderH + 6.0f));

		if (ImGui::BeginTabBar("##PostBoxTabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Receber"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 0;
				DrawReceiveTab();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Enviar"))
			{
				DrawActiveTabOrnament();
				m_activeTab = 1;
				DrawSendTab();
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) || ImGui::IsWindowHovered())
			ImGui::CaptureMouseFromApp(true);

		ImGui::End();
	}

	PopWindowStyle();
	if (p_open && !*p_open)
		Close();
}
