#include "Roleta.h"
#include "..\\sinbaram\\sinLinkHeader.h"

#include "imGui/imgui.h"
#include <thread>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <boost\\algorithm\\string\\predicate.hpp>

std::vector<TOP10_ROLETA> rPlayers;

RoletaPckg sPlayers;
PDIRECT3DTEXTURE9 Classes[12];
PDIRECT3DTEXTURE9 ItemImage;
PDIRECT3DTEXTURE9 Images[2];

float delayRoleta = 100;

bool dividirIgualmente = false;
bool showDetailsRoleta = false;

bool participante = false;

extern sITEM TempPerfectItem;
extern bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);

extern BOOL bIsPerfect;

using namespace std;

void Roleta::initRoleta()
{
	ZeroMemory(&Classes, sizeof(Classes));
	ZeroMemory(&Images, sizeof(Images));

	int w = 0;
	int h = 0;
	char Path[64] = { 0 };

	for (int x = 0; x <= 10; x++)
	{
		sprintf_s(Path, sizeof(Path), "game\\images\\Roleta\\Classes\\%d.png", x);
		bool ret = LoadTextureFromFile2(Path, &Classes[x], &w, &h);
		IM_ASSERT(ret);
	}

	sprintf_s(Path, sizeof(Path), "game\\images\\Roleta\\dado.png");
	bool ret = LoadTextureFromFile2(Path, &Images[0], &w, &h);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\Roleta\\pular.png");
	ret = LoadTextureFromFile2(Path, &Images[1], &w, &h);
	IM_ASSERT(ret);

	init = false;
}

void Roleta::updateItemImage(char itemID[32])
{
	ZeroMemory(&ItemImage, sizeof(ItemImage));

	int w = 0;
	int h = 0;
	char Path[64] = { 0 };
	bool found = false;

	sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Defense\\%s", itemID);
	ItemImage = LoadDibSurfaceOffscreen(Path);

	if (ItemImage == NULL)
	{
		sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Weapon\\%s", itemID);
		ItemImage = LoadDibSurfaceOffscreen(Path);
	}

	if (ItemImage == NULL)
	{
		sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\%s", itemID);
		ItemImage = LoadDibSurfaceOffscreen(Path);
	}

	if (ItemImage == NULL)
	{
		sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Event\\%s", itemID);
		ItemImage = LoadDibSurfaceOffscreen(Path);
	}

	if (ItemImage == NULL)
	{
		sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\%s", itemID);
		ItemImage = LoadDibSurfaceOffscreen(Path);
	}

	if (ItemImage == NULL)
	{
		sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Potion\\%s", itemID);
		ItemImage = LoadDibSurfaceOffscreen(Path);
	}

}

bool sortByDamage(const TOP10_ROLETA& a, const TOP10_ROLETA& b)
{
	return a.Damage > b.Damage;
}

INT Roleta::UpdatePlayers(RoletaPckg* Players)
{
	ZeroMemory(&sPlayers, sizeof(RoletaPckg));
	CopyMemory(&sPlayers, Players, sizeof(RoletaPckg));

	return TRUE;
}

INT Roleta::ReceiveTopPlayers(RoletaPckg* Players)
{
	ZeroMemory(&sPlayers, sizeof(RoletaPckg));
	CopyMemory(&sPlayers, Players, sizeof(RoletaPckg));

	rPlayers.clear();

	for (int x = 0; x < 10; x++)
	{
		rPlayers.push_back(sPlayers.Players[x]);
	}

	// Ajusta o vetor por dano causado
	std::sort(rPlayers.begin(), rPlayers.end(), sortByDamage);

	init = true;
	openFlag = true;

	return TRUE;
}

void Roleta::ShowRoleta(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();

	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	if (delayRoleta > 0)
	{
		if (ImGui::Begin("Deseja Participar da Rodada?", p_open, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			static float progress_dir = 0.4f;

			delayRoleta -= progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
			if (delayRoleta >= +1.1f) { delayRoleta = +1.1f; progress_dir *= -1.0f; }
			if (delayRoleta <= -0.1f) { delayRoleta = -0.1f; progress_dir *= -1.0f; }

			ImGui::SetCursorPosY(260);
			ImGui::ProgressBar(delayRoleta, ImVec2(-1, 0), "Tempo restante");

			ImGui::SetCursorPosX(125);
			ImGui::SetCursorPosY(20);

			ImGui::Image(ItemImage, ImVec2(44, 88));
			ImGui::SetCursorPosX(125);
			ImGui::SetCursorPosY(20);
			ImGui::Selectable("##ItemRoleta", false, ImGuiSelectableFlags_AllowItemOverlap, ImVec2(40, 120));

			if (ImGui::IsItemHovered())
			{
				showDetailsRoleta = true;
				bIsPerfect = TRUE;
			}
			else
			{
				showDetailsRoleta = false;
				bIsPerfect = FALSE;
			}

			static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;

			if (ImGui::BeginTable("Participantes", 2, flags))
			{
				ImGui::TableSetupColumn("Nick", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableSetupColumn("Chance Atual", ImGuiTableColumnFlags_WidthFixed);
				ImGui::TableHeadersRow();

				for (int row = 0; row < 10; row++)
				{
					if (sPlayers.Players[row].jobCode && sPlayers.Players[row].pSorteio)
					{
						ImGui::TableNextRow();

						ImGui::TableSetColumnIndex(0);
						ImGui::Text("%s", sPlayers.Players[row].szName);
						ImGui::TableSetColumnIndex(1);
						ImGui::Text("%d", dividirIgualmente ? 10 : sPlayers.Players[row].chance);
					}
				}

				ImGui::EndTable();
			}


			if (bIsPerfect)
			{
				sinShowItemInfoFlag = 2;
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&TempPerfectItem, FALSE, FALSE, FALSE);
			}

			ImGui::SetCursorPosX(80);

			if (participarRodada && !participante)
			{
				if (ImGui::ImageButton(Images[0], ImVec2(36, 36)))
				{
					smTRANS_COMMAND resgatarImposto;
					resgatarImposto.code = ROLETA_PARTICIPAR_RODADA;
					resgatarImposto.size = sizeof(smTRANS_COMMAND);
					resgatarImposto.WParam = 0;
					resgatarImposto.LParam = 0;
					resgatarImposto.SParam = 0;

					if (smWsockServer)
						smWsockServer->Send((char*)&resgatarImposto, resgatarImposto.size, TRUE);

					participante = true;
				}
			}
			else if (!participarRodada && participante)
			{
				ImGui::SetCursorPosX(100);
				ImGui::Button(u8"Você passou.", ImVec2(100, 30));
			}

			ImGui::SameLine();
			ImGui::SetCursorPosX(180);

			if (participarRodada && !participante && !dividirIgualmente)
			{
				if (ImGui::ImageButton(Images[1], ImVec2(36, 36)))
				{
					participarRodada = false;
					participante = true;

					smTRANS_COMMAND resgatarImposto;
					resgatarImposto.code = ROLETA_PARTICIPAR_RODADA;
					resgatarImposto.size = sizeof(smTRANS_COMMAND);
					resgatarImposto.WParam = 1;
					resgatarImposto.LParam = 0;
					resgatarImposto.SParam = 0;

					if (smWsockServer)
						smWsockServer->Send((char*)&resgatarImposto, resgatarImposto.size, TRUE);
				}
			}

			ImGui::SetCursorPos({ ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + (36 - ImGui::GetFont()->FontSize) / 2 });

			ImGui::End();
		}
	}
	else
	{
		ZeroMemory(&sPlayers, sizeof(RoletaPckg));

		delayRoleta = 100;
		openFlag = false;

		bIsPerfect = false;
		showDetailsRoleta = false;

		participarRodada = true;

		participante = false;
		openRoleta = false;

		dividirIgualmente = false;
	}
}

void Roleta::Show(bool* p_open)
{
	if (init)
		initRoleta();

	ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();

	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	int posInicial = 0;

	if (ImGui::Begin("TOP 10 Causadores de dano", p_open, ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		posInicial += 2;
		ImGui::SetCursorPosX(posInicial); posInicial += 140;
		ImGui::Text(u8"Posição");
		ImGui::SameLine();
		ImGui::SetCursorPosX(posInicial); posInicial += 140;
		ImGui::Text("Nick");
		ImGui::SameLine();
		ImGui::SetCursorPosX(posInicial); posInicial += 140;
		ImGui::Text("Dano Causado");
		ImGui::SameLine();
		ImGui::SetCursorPosX(posInicial);
		ImGui::Text("%% Total");
		ImGui::Separator();

		int z = 0;
		posInicial = 140;
		float danoCausado = 0;
		float meuDano = 0;

		for (auto& Player : rPlayers)
		{
			if (z < 10 && Player.jobCode)
			{
				ImGui::Text("%d", z + 1);
				ImGui::SameLine();
				ImGui::Image(Classes[Player.jobCode], ImVec2(18, 18));
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(Player.szName);
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text("%d", Player.Damage);
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial = 140;
				danoCausado = (Player.Damage * 100) / (sPlayers.totalDamage > 0 ? sPlayers.totalDamage : 1);
				ImGui::Text("%.4f", danoCausado);
				ImGui::Separator();

				if (boost::iequals(Player.szName, lpCurPlayer->smCharInfo.szName))
					meuDano = danoCausado;
			}

			z++;
		}

		/*if (meuDano >= 50 && !dividirIgualmente)
		{
			ImGui::SetCursorPosY(220);

			if (ImGui::Button("Dividir Igualmente!", ImVec2(-1, 20)))
			{
				RoletaPckg Pckg;
				ZeroMemory(&Pckg, sizeof(RoletaPckg));

				Pckg.code = ROLETA_DIVIDIR_IGUALMENTE;
				Pckg.size = sizeof(RoletaPckg);
				CopyMemory(Pckg.Players, sPlayers.Players, sizeof(sPlayers.Players));

				if (smWsockServer)
					smWsockServer->Send((char*)&Pckg, Pckg.size, TRUE);

				dividirIgualmente = true;
			}
		}*/

		if (delayRoleta > 0)
		{
			static float progress_dir = 0.42f;

			delayRoleta -= progress_dir * 0.4f * ImGui::GetIO().DeltaTime;
			if (delayRoleta >= +1.1f) { delayRoleta = +1.1f; progress_dir *= -1.0f; }
			if (delayRoleta <= -0.1f) { delayRoleta = -0.1f; progress_dir *= -1.0f; }

			ImGui::SetCursorPosY(260);
			ImGui::ProgressBar(delayRoleta, ImVec2(-1, 0), "Iniciando Roleta");
		}
		else
		{
			delayRoleta = 100;
			openFlag = false;

			ZeroMemory(&sPlayers, sizeof(RoletaPckg));
		}

		ImGui::End();
	}
}