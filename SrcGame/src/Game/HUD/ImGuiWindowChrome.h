#pragma once

#include "imGui/imgui.h"

// Cromado vetorial compartilhado das janelas de jogador. Somente os titulos
// usam PNG; moldura, fundo, divisorias e controles ficam no ImDrawList.
inline constexpr float kPlayerWindowRound = 3.5f;
inline constexpr float kPlayerWindowBodyInset = 16.0f;
inline constexpr float kPlayerBezelThickness = 3.8f;
inline constexpr float kPlayerOverlayBezelThickness = 2.8f;

inline constexpr ImU32 kPlayerStone = IM_COL32(52, 50, 47, 255);
inline constexpr ImU32 kPlayerStoneHi = IM_COL32(139, 132, 113, 235);
inline constexpr ImU32 kPlayerStoneHiSoft = IM_COL32(91, 86, 75, 210);
inline constexpr ImU32 kPlayerStoneLo = IM_COL32(9, 10, 12, 255);
inline constexpr ImU32 kPlayerStoneLoSoft = IM_COL32(28, 27, 25, 255);
inline constexpr ImU32 kPlayerBody = IM_COL32(10, 12, 16, 248);

inline void DrawPlayerStoneBezel(ImDrawList* draw, const ImVec2& r0, const ImVec2& r1, float thickness, float round)
{
	if (!draw)
		return;
	if (thickness < 2.4f)
		thickness = 2.4f;

	// Perfil metal/pedra estreito. O contorno escuro separa a janela do
	// mundo; luz em cima/esquerda e sombra embaixo/direita dão profundidade.
	draw->AddRect(r0, r1, kPlayerStoneLo, round, 0, thickness + 1.4f);
	draw->AddRect(r0, r1, kPlayerStone, round, 0, thickness);

	const float edge = 1.1f;
	draw->AddLine(
		ImVec2(r0.x + round, r0.y + edge),
		ImVec2(r1.x - round, r0.y + edge),
		kPlayerStoneHi, 1.15f);
	draw->AddLine(
		ImVec2(r0.x + edge, r0.y + round),
		ImVec2(r0.x + edge, r1.y - round),
		kPlayerStoneHi, 1.15f);
	draw->AddLine(
		ImVec2(r0.x + round, r1.y - edge),
		ImVec2(r1.x - round, r1.y - edge),
		kPlayerStoneLo, 1.35f);
	draw->AddLine(
		ImVec2(r1.x - edge, r0.y + round),
		ImVec2(r1.x - edge, r1.y - round),
		kPlayerStoneLo, 1.35f);

	// Pequenos entalhes nos cantos, todos dentro do retangulo da janela.
	const float mark = 8.0f;
	const float inset = 2.0f;
	draw->AddLine(ImVec2(r0.x + inset, r0.y + mark), ImVec2(r0.x + inset, r0.y + inset), kPlayerStoneHiSoft, 1.0f);
	draw->AddLine(ImVec2(r0.x + inset, r0.y + inset), ImVec2(r0.x + mark, r0.y + inset), kPlayerStoneHiSoft, 1.0f);
	draw->AddLine(ImVec2(r1.x - mark, r0.y + inset), ImVec2(r1.x - inset, r0.y + inset), kPlayerStoneHiSoft, 1.0f);
	draw->AddLine(ImVec2(r1.x - inset, r0.y + inset), ImVec2(r1.x - inset, r0.y + mark), kPlayerStoneLoSoft, 1.0f);
	draw->AddLine(ImVec2(r0.x + inset, r1.y - mark), ImVec2(r0.x + inset, r1.y - inset), kPlayerStoneHiSoft, 1.0f);
	draw->AddLine(ImVec2(r0.x + inset, r1.y - inset), ImVec2(r0.x + mark, r1.y - inset), kPlayerStoneLoSoft, 1.0f);
	draw->AddLine(ImVec2(r1.x - mark, r1.y - inset), ImVec2(r1.x - inset, r1.y - inset), kPlayerStoneLoSoft, 1.0f);
	draw->AddLine(ImVec2(r1.x - inset, r1.y - inset), ImVec2(r1.x - inset, r1.y - mark), kPlayerStoneLoSoft, 1.0f);
}

inline void DrawPlayerControlBezel(ImDrawList* draw, const ImVec2& r0, const ImVec2& r1, bool hovered)
{
	if (!draw)
		return;
	const float round = 2.4f;
	draw->AddRectFilled(r0, r1, hovered ? IM_COL32(55, 46, 27, 255) : IM_COL32(17, 19, 23, 255), round);
	draw->AddRect(r0, r1, kPlayerStoneLo, round, 0, 2.2f);
	draw->AddLine(ImVec2(r0.x + 3.0f, r0.y + 1.0f), ImVec2(r1.x - 3.0f, r0.y + 1.0f), kPlayerStoneHi, 1.0f);
	draw->AddLine(ImVec2(r0.x + 1.0f, r0.y + 3.0f), ImVec2(r0.x + 1.0f, r1.y - 3.0f), kPlayerStoneHiSoft, 1.0f);
	draw->AddRect(
		ImVec2(r0.x + 2.0f, r0.y + 2.0f),
		ImVec2(r1.x - 2.0f, r1.y - 2.0f),
		hovered ? IM_COL32(230, 200, 110, 255) : IM_COL32(200, 170, 90, 205),
		1.7f, 0, 1.0f);
}

inline void DrawPlayerWindowChrome(ImDrawList* draw, float headerH, ImU32 gold, ImU32 goldFill,
	float bezelThickness = kPlayerBezelThickness)
{
	if (!draw)
		return;

	const ImVec2 p0 = ImGui::GetWindowPos();
	const ImVec2 size = ImGui::GetWindowSize();
	const ImVec2 p1(p0.x + size.x, p0.y + size.y);
	const float pad = 1.2f;
	const ImVec2 r0(p0.x + pad, p0.y + pad);
	const ImVec2 r1(p1.x - pad, p1.y - pad);
	const float round = kPlayerWindowRound;
	const float bezel = bezelThickness;

	// Corpo em gradiente grafite azulado, discreto para nao competir com
	// textos e controles.
	draw->AddRectFilled(r0, r1, kPlayerBody, round);
	const ImVec2 body0(r0.x + 2.0f, r0.y + 2.0f);
	const ImVec2 body1(r1.x - 2.0f, r1.y - 2.0f);
	draw->AddRectFilledMultiColor(
		body0, body1,
		IM_COL32(17, 20, 26, 248), IM_COL32(15, 18, 24, 248),
		IM_COL32(8, 10, 14, 250), IM_COL32(9, 11, 15, 250));

	const ImVec2 i0(r0.x + bezel, r0.y + bezel);
	const ImVec2 i1(r1.x - bezel, r1.y - bezel);
	const float innerRound = round * 0.50f;

	float headerBottom = r0.y + headerH;
	if (headerBottom > i1.y)
		headerBottom = i1.y;
	// Cor opaca e uniforme: os PNGs de titulo usam exatamente o mesmo
	// RGB, evitando um retangulo perceptivel no renderer DX9 legado.
	draw->AddRectFilled(
		i0, ImVec2(i1.x, headerBottom),
		goldFill, innerRound, ImDrawFlags_RoundCornersTop);

	DrawPlayerStoneBezel(draw, r0, r1, bezel, round);

	// Dupla divisoria de header: sombra abaixo e filete de ouro envelhecido.
	draw->AddLine(
		ImVec2(i0.x + 3.0f, headerBottom + 1.0f),
		ImVec2(i1.x - 3.0f, headerBottom + 1.0f),
		IM_COL32(0, 0, 0, 180), 1.0f);
	draw->AddLine(
		ImVec2(i0.x + 3.0f, headerBottom),
		ImVec2(i1.x - 3.0f, headerBottom),
		gold, 1.15f);

	// Filete interno fino preserva a identidade dourada sem parecer uma
	// moldura pesada.
	draw->AddRect(i0, i1, IM_COL32(200, 170, 90, 125), innerRound, 0, 1.0f);
}

inline void SetPlayerWindowBodyCursor(float headerH)
{
	ImGui::SetCursorPos(ImVec2(kPlayerWindowBodyInset, headerH + 6.0f));
}

inline void DrawPlayerColumnSplit()
{
	const ImVec2 p = ImGui::GetCursorScreenPos();
	const float h = ImGui::GetContentRegionAvail().y;
	ImGui::GetWindowDrawList()->AddLine(
		ImVec2(p.x - 4.0f, p.y + 2.0f),
		ImVec2(p.x - 4.0f, p.y + h - 2.0f),
		IM_COL32(200, 170, 90, 150), 1.05f);
}
