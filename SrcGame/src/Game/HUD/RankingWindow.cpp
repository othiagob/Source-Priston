#include "RankingWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"

#include "imGui/imgui.h"
#include <algorithm>

TopPlayerPckg PlayersOnRanking;

std::vector<TopPlayers> Players;
std::vector<TopPlayers> PlayersPVP;

static int selected = 1;
static int selectedClass = 0;

INT RankingWindow::OpenNPCRanking()
{
	smTRANS_COMMAND openNpc;
	openNpc.code = OPEN_RANKING_NPC;
	openNpc.size = sizeof(smTRANS_COMMAND);
	openNpc.WParam = 0;
	openNpc.LParam = 0;
	openNpc.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&openNpc, openNpc.size, TRUE);

	return TRUE;
}

PDIRECT3DTEXTURE9 my_texture[12];
LPDIRECT3DTEXTURE9 Image[12];
int w = 0;
int h = 0;
char Path[64] = { 0 };

extern bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);


bool sortByLevel(const TopPlayers& a, const TopPlayers& b)
{
	return a.playerLevel > b.playerLevel;
}

bool sortByKills(const TopPlayers& a, const TopPlayers& b)
{
	return a.playerBps > b.playerBps;
}

INT RankingWindow::ReceivePlayers(TopPlayerPckg* Pckg)
{
	ZeroMemory(&PlayersOnRanking, sizeof(TopPlayerPckg));
	CopyMemory(&PlayersOnRanking, Pckg, sizeof(TopPlayerPckg));

	Players.clear();
	PlayersPVP.clear();

	for (int x = 0; x < 180; x++) {
		if (PlayersOnRanking.Players[x].playerClass > 0 && PlayersOnRanking.Players[x].playerName[2] != '-') //gm no ranking
			Players.push_back(PlayersOnRanking.Players[x]);
	}

	for (int x = 0; x <= 10; x++)
	{
		sprintf_s(Path, sizeof(Path), "game\\images\\ranking\\%d.png", x);
		bool ret = LoadTextureFromFile2(Path, &my_texture[x], &w, &h);
		IM_ASSERT(ret);

		Image[x] = LoadDibSurfaceOffscreen(Path);
	}

	PlayersPVP = Players;

	std::sort(Players.begin(), Players.end(), sortByLevel);
	std::sort(PlayersPVP.begin(), PlayersPVP.end(), sortByKills);

	return TRUE;
}

void RankingWindow::OpenNpc(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(450, 350), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();

	int WinX = (smScreenWidth / 2) - 200;
	int WinY = (smScreenHeight / 2) - 200;
	int SlotY = 0;

	ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

	int posInicial = 0;

	if (ImGui::Begin("Ranking de Jogadores", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		if (ImGui::BeginTabBar("Tabelas", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(u8"Ranking por nível"))
			{
				for (int x = 0; x <= 8; x++)
				{
					if (ImGui::ImageButton(my_texture[x], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
						selectedClass = x;

					ImGui::SameLine();
				}

				ImGui::Separator();
				ImGui::SetCursorPosY(100);

				posInicial += 2;
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(u8"Posição");
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(u8"Nick");
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(u8"Nível");
				ImGui::Separator();

				int z = 1;

				posInicial = 140;

				for (auto& Player : Players)
				{
					if (Player.playerClass == selectedClass) // Ranking por classe
					{
						if (z <= 10) {
							char pos[12];
							sprintf_s(pos, "%d", z);
							ImGui::Text(pos);
							ImGui::SameLine();
							ImGui::Image(my_texture[selectedClass], ImVec2(18, 18));
							ImGui::SameLine();
							ImGui::SetCursorPosX(posInicial); posInicial += 140;
							ImGui::Text(Player.playerName);
							ImGui::SameLine();
							char pLevel[12];
							sprintf_s(pLevel, "%d", Player.playerLevel);
							ImGui::SetCursorPosX(posInicial); posInicial = 140;
							ImGui::Text(pLevel);
							ImGui::Separator();
							z++;
						}
					}
					else if (selectedClass == 0) // Ranking Geral
					{
						posInicial = 140;

						if (z <= 10) {
							char pos[12];
							sprintf_s(pos, "%d", z);
							ImGui::Text(pos);
							ImGui::SameLine();
							ImGui::Image(my_texture[Player.playerClass], ImVec2(18, 18));
							ImGui::SameLine();
							ImGui::SetCursorPosX(posInicial); posInicial += 140;
							ImGui::Text(Player.playerName);
							ImGui::SameLine();
							char pLevel[12];
							sprintf_s(pLevel, "%d", Player.playerLevel);
							ImGui::SetCursorPosX(posInicial); posInicial += 140;
							ImGui::Text(pLevel);
							ImGui::Separator();
							z++;
						}
					}

				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Ranking PVP"))
			{
				for (int x = 0; x <= 8; x++)
				{
					if (ImGui::ImageButton(my_texture[x], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
						selectedClass = x;

					ImGui::SameLine();
				}


				ImGui::Separator();
				ImGui::SetCursorPosY(100);

				posInicial = 2;
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(u8"Posição");
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(u8"Nick");
				ImGui::SameLine();
				ImGui::SetCursorPosX(posInicial); posInicial += 140;
				ImGui::Text(u8"Mortes");
				ImGui::Separator();

				int z = 1;

				posInicial = 140;

				for (auto& Player : PlayersPVP)
				{
					if (Player.playerClass == selectedClass) // Ranking por classe
					{
						if (z <= 10) {
							char pos[12];
							sprintf_s(pos, "%d", z);
							ImGui::Text(pos);
							ImGui::SameLine();
							ImGui::Image(my_texture[selectedClass], ImVec2(18, 18));
							ImGui::SameLine();
							ImGui::SetCursorPosX(posInicial); posInicial += 140;
							ImGui::Text(Player.playerName);
							ImGui::SameLine();
							char pLevel[12];
							sprintf_s(pLevel, "%d", Player.playerBps);
							ImGui::SetCursorPosX(posInicial); posInicial = 140;
							ImGui::Text(pLevel);
							ImGui::Separator();
							z++;
						}
					}
					else if (selectedClass == 0) // Ranking Geral
					{
						posInicial = 140;

						if (z <= 10) {
							char pos[12];
							sprintf_s(pos, "%d", z);
							ImGui::Text(pos);
							ImGui::SameLine();
							ImGui::Image(my_texture[Player.playerClass], ImVec2(18, 18));
							ImGui::SameLine();
							ImGui::SetCursorPosX(posInicial); posInicial += 140;
							ImGui::Text(Player.playerName);
							ImGui::SameLine();
							char pBPS[12];
							sprintf_s(pBPS, "%d", Player.playerBps);
							ImGui::SetCursorPosX(posInicial); posInicial = 140;
							ImGui::Text(pBPS);
							ImGui::Separator();
							z++;
						}
					}

				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
			ImGui::End();
		}
	}
}