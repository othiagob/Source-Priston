#pragma once

#include "imgui.h"

// Identidade visual das ferramentas desktop (Server.exe).
// Nao reutilizar o cromado de jogador (15-imgui-windows / AdminChrome).
// C++14: static const, sem inline constexpr.

static const float kToolHeaderH = 48.0f;
static const float kToolSidebarW = 196.0f;
static const float kToolRound = 4.0f;
static const float kToolPad = 16.0f;
static const int kToolStyleVarCount = 9;
static const int kToolStyleColorCount = 28;

static const ImU32 kToolBg = IM_COL32(244, 241, 236, 255);
static const ImU32 kToolSurface = IM_COL32(255, 254, 252, 255);
static const ImU32 kToolSidebar = IM_COL32(236, 232, 226, 255);
static const ImU32 kToolBorder = IM_COL32(216, 212, 204, 255);
static const ImU32 kToolText = IM_COL32(42, 44, 48, 255);
static const ImU32 kToolMuted = IM_COL32(110, 108, 104, 255);
static const ImU32 kToolAccent = IM_COL32(58, 110, 168, 255);
static const ImU32 kToolAccentHi = IM_COL32(46, 92, 148, 255);
static const ImU32 kToolOnline = IM_COL32(62, 140, 92, 255);
static const ImU32 kToolMaint = IM_COL32(180, 128, 48, 255);
static const ImU32 kToolDanger = IM_COL32(176, 64, 56, 255);

inline ImVec4 ToolC(ImU32 c)
{
	return ImVec4(
		((c >> IM_COL32_R_SHIFT) & 255) / 255.0f,
		((c >> IM_COL32_G_SHIFT) & 255) / 255.0f,
		((c >> IM_COL32_B_SHIFT) & 255) / 255.0f,
		((c >> IM_COL32_A_SHIFT) & 255) / 255.0f);
}

inline void PushToolStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, kToolRound);
	ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8.0f, 8.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarSize, 10.0f);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ToolC(kToolBg));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, ToolC(kToolSurface));
	ImGui::PushStyleColor(ImGuiCol_PopupBg, ToolC(kToolSurface));
	ImGui::PushStyleColor(ImGuiCol_Border, ToolC(kToolBorder));
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolText));
	ImGui::PushStyleColor(ImGuiCol_TextDisabled, ToolC(kToolMuted));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.96f, 0.95f, 0.93f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.88f, 0.91f, 0.96f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ToolC(kToolAccent));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.90f, 0.93f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.84f, 0.89f, 0.95f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ToolC(kToolAccent));
	ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.94f, 0.96f, 0.99f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.90f, 0.93f, 0.97f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_CheckMark, ToolC(kToolAccent));
	ImGui::PushStyleColor(ImGuiCol_SliderGrab, ToolC(kToolAccent));
	ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ToolC(kToolAccentHi));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarBg, ImVec4(0.94f, 0.93f, 0.91f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrab, ImVec4(0.72f, 0.70f, 0.66f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabHovered, ToolC(kToolAccent));
	ImGui::PushStyleColor(ImGuiCol_ScrollbarGrabActive, ToolC(kToolAccentHi));
	ImGui::PushStyleColor(ImGuiCol_Separator, ToolC(kToolBorder));
	ImGui::PushStyleColor(ImGuiCol_TableHeaderBg, ImVec4(0.93f, 0.91f, 0.88f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TableBorderLight, ToolC(kToolBorder));
	ImGui::PushStyleColor(ImGuiCol_TableBorderStrong, ToolC(kToolBorder));
	ImGui::PushStyleColor(ImGuiCol_TableRowBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_TableRowBgAlt, ImVec4(0.97f, 0.96f, 0.94f, 1.0f));
}

inline void PopToolStyle()
{
	ImGui::PopStyleColor(kToolStyleColorCount);
	ImGui::PopStyleVar(kToolStyleVarCount);
}

inline void ToolSection(const char* title)
{
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolAccent));
	ImGui::TextUnformatted(title);
	ImGui::PopStyleColor();
	const ImVec2 a = ImGui::GetCursorScreenPos();
	const float w = ImGui::GetContentRegionAvail().x;
	ImGui::GetWindowDrawList()->AddLine(a, ImVec2(a.x + w, a.y), kToolBorder, 1.0f);
	ImGui::Dummy(ImVec2(w, 6.0f));
}

inline void ToolHint(const char* text)
{
	ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolMuted));
	ImGui::TextWrapped("%s", text);
	ImGui::PopStyleColor();
}

inline void ToolMetricCard(const char* label, const char* value, float width, float height = 64.0f)
{
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(origin, ImVec2(origin.x + width, origin.y + height), kToolSurface, kToolRound);
	draw->AddRect(origin, ImVec2(origin.x + width, origin.y + height), kToolBorder, kToolRound, 0, 1.0f);
	draw->AddText(ImVec2(origin.x + 12.0f, origin.y + 10.0f), kToolMuted, label);
	draw->AddText(ImVec2(origin.x + 12.0f, origin.y + 30.0f), kToolText, value);
	ImGui::Dummy(ImVec2(width, height));
}

inline int ToolCardColumns(float avail)
{
	if (avail < 420.0f)
		return 1;
	if (avail < 760.0f)
		return 2;
	return 4;
}

inline bool ToolNavItem(const char* label, bool selected, float width)
{
	if (selected)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ToolC(kToolAccent));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ToolC(kToolAccentHi));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ToolC(kToolAccentHi));
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ToolC(kToolSidebar));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.90f, 0.88f, 0.85f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.86f, 0.84f, 0.80f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_Text, ToolC(kToolText));
	}

	const bool clicked = ImGui::Button(label, ImVec2(width, 32.0f));
	ImGui::PopStyleColor(4);
	return clicked;
}

inline bool ToolPrimaryButton(const char* label, const ImVec2& size)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ToolC(kToolAccent));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ToolC(kToolAccentHi));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ToolC(kToolAccentHi));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ToolC(kToolAccent));
	const bool clicked = ImGui::Button(label, size);
	ImGui::PopStyleColor(5);
	return clicked;
}

inline bool ToolDangerButton(const char* label, const ImVec2& size)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ToolC(kToolDanger));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.78f, 0.32f, 0.28f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.62f, 0.20f, 0.18f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Border, ToolC(kToolDanger));
	const bool clicked = ImGui::Button(label, size);
	ImGui::PopStyleColor(5);
	return clicked;
}

inline void ToolStatusPill(const char* text, ImU32 color)
{
	const ImVec2 ts = ImGui::CalcTextSize(text);
	const float padX = 10.0f;
	const float w = ts.x + padX * 2.0f;
	const float h = 24.0f;
	const ImVec2 origin = ImGui::GetCursorScreenPos();
	ImDrawList* draw = ImGui::GetWindowDrawList();
	draw->AddRectFilled(origin, ImVec2(origin.x + w, origin.y + h), kToolSurface, 3.0f);
	draw->AddRect(origin, ImVec2(origin.x + w, origin.y + h), color, 3.0f, 0, 1.0f);
	draw->AddText(ImVec2(origin.x + padX, origin.y + (h - ts.y) * 0.5f), color, text);
	ImGui::Dummy(ImVec2(w, h));
}
