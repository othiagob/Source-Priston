#pragma once

#include "imGui/imgui.h"

// Bezel fino de duas camadas (mockup Desafios): traço externo um pouco
// mais marcado + hairline interno, cantos leves. Sem PNG 9-slice, sem
// losango, sem barra grossa. Rounding só no desenho — WindowRounding nativo fica 0.
inline constexpr float kPlayerWindowRound = 3.5f;
inline constexpr float kPlayerWindowBodyInset = 16.0f;

inline void DrawPlayerWindowChrome(ImDrawList* draw, float headerH, ImU32 gold, ImU32 goldFill)
{
	if (!draw)
		return;

	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	const ImVec2 p1(p0.x + size.x, p0.y + size.y);

	// Recuo para o traço não ser cortado no clip da janela.
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

// Linha vertical fina no vão entre lista e detalhe (mesmo ouro da moldura).
inline void DrawPlayerColumnSplit()
{
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const float h = ImGui::GetContentRegionAvail().y;
	ImGui::GetWindowDrawList()->AddLine(
		ImVec2(p.x - 4.0f, p.y + 2.0f),
		ImVec2(p.x - 4.0f, p.y + h - 2.0f),
		IM_COL32(200, 170, 90, 150), 1.05f);
}
