#pragma once

#include "imgui.h"

// C++14: sem inline constexpr (o servidor não usa C++17).
static const float kPlayerWindowRound = 3.5f;
static const float kPlayerWindowBodyInset = 16.0f;
static const float kAdminHeaderH = 50.0f;
static const float kHeaderBtnW = 28.0f;
static const float kHeaderBtnH = 22.0f;
static const float kHeaderBtnRound = 3.0f;

static const ImU32 kGold = IM_COL32(200, 170, 90, 220);
static const ImU32 kGoldBright = IM_COL32(230, 200, 110, 255);
static const ImU32 kGoldDim = IM_COL32(200, 170, 90, 90);
static const ImU32 kGoldFill = IM_COL32(20, 24, 32, 255);

inline void DrawPlayerWindowChrome(ImDrawList* draw, float headerH, ImU32 gold, ImU32 goldFill)
{
	if (!draw)
		return;

	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	const ImVec2 p1(p0.x + size.x, p0.y + size.y);

	const float pad = 1.5f;
	const ImVec2 r0(p0.x + pad, p0.y + pad);
	const ImVec2 r1(p1.x - pad, p1.y - pad);
	const float round = kPlayerWindowRound;

	draw->AddRectFilled(r0, r1, IM_COL32(10, 12, 16, 248), round);
	draw->AddRectFilled(r0, ImVec2(r1.x, r0.y + headerH), goldFill, round, ImDrawFlags_RoundCornersTop);

	const float inner = 2.8f;
	const ImVec2 i0(r0.x + inner, r0.y + inner);
	const ImVec2 i1(r1.x - inner, r1.y - inner);

	draw->AddLine(
		ImVec2(i0.x + 4.0f, r0.y + headerH),
		ImVec2(i1.x - 4.0f, r0.y + headerH),
		gold, 1.15f);

	draw->AddRect(r0, r1, gold, round, 0, 1.9f);
	draw->AddRect(i0, i1, IM_COL32(214, 184, 108, 155), round * 0.72f, 0, 1.05f);
}

inline void SetPlayerWindowBodyCursor(float headerH)
{
	ImGui::SetCursorPos(ImVec2(kPlayerWindowBodyInset, headerH + 6.0f));
}

inline void DrawActiveTabOrnament()
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

inline bool DrawHeaderClose()
{
	const ImVec2 win = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(win.x - kHeaderBtnW - 12.0f, (kAdminHeaderH - kHeaderBtnH) * 0.5f));
	ImGui::InvisibleButton("##AdminClose", ImVec2(kHeaderBtnW, kHeaderBtnH));
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

inline void DrawSectionHeader(const char* title)
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

inline void PushAdminWindowStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16.0f, 12.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
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

inline void PopAdminWindowStyle()
{
	ImGui::PopStyleColor(25);
	ImGui::PopStyleVar(10);
}

inline void CenterTextUnformatted(const char* text)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ts.x) * 0.5f);
	ImGui::TextUnformatted(text);
}

inline void DrawMetricCard(const char* label, const char* value, float width, float height = 68.0f)
{
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(origin, ImVec2(origin.x + width, origin.y + height), IM_COL32(18, 20, 26, 255), 3.0f);
	draw->AddRect(origin, ImVec2(origin.x + width, origin.y + height), kGold, 3.0f, 0, 1.4f);
	draw->AddText(ImVec2(origin.x + 12.0f, origin.y + 10.0f), IM_COL32(158, 158, 173, 255), label);
	draw->AddText(ImVec2(origin.x + 12.0f, origin.y + 32.0f), kGoldBright, value);
	ImGui::Dummy(ImVec2(width, height));
}

inline void DrawHintPlaque(const char* text)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	const float padX = 18.0f;
	const float padY = 10.0f;
	const float boxW = ts.x + padX * 2.0f;
	const float boxH = ts.y + padY * 2.0f;
	const float avail = ImGui::GetContentRegionAvail().x;
	const float availY = ImGui::GetContentRegionAvail().y;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (avail - boxW) * 0.5f);
	if (availY > boxH + 8.0f)
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (availY - boxH) * 0.35f);

	const ImVec2 origin = ImGui::GetCursorScreenPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(origin, ImVec2(origin.x + boxW, origin.y + boxH), IM_COL32(18, 20, 26, 255), 3.0f);
	draw->AddRect(origin, ImVec2(origin.x + boxW, origin.y + boxH), kGold, 3.0f, 0, 1.4f);
	draw->AddText(ImVec2(origin.x + padX, origin.y + padY), kGoldBright, text);
	ImGui::Dummy(ImVec2(boxW, boxH));
}

inline bool DrawHeaderStatusPill(const char* text, ImU32 border)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	const float padX = 10.0f;
	const float w = ts.x + padX * 2.0f;
	const float h = kHeaderBtnH;
	const ImVec2 win = ImGui::GetWindowSize();
	ImGui::SetCursorPos(ImVec2(win.x - kHeaderBtnW - 18.0f - w, (kAdminHeaderH - h) * 0.5f));
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(origin, ImVec2(origin.x + w, origin.y + h), IM_COL32(18, 20, 24, 255), 3.0f);
	draw->AddRect(origin, ImVec2(origin.x + w, origin.y + h), border, 3.0f, 0, 1.2f);
	draw->AddText(ImVec2(origin.x + padX, origin.y + (h - ts.y) * 0.5f), border, text);
	ImGui::Dummy(ImVec2(w, h));
	return true;
}
