#include "MixWindow.h"
#include "imGui/imgui.h"
#include <algorithm>
#include <d3d9.h>

#define MAX_MIX_COUNT 2

static int selected = 1;

std::vector<MixInfo> allMix;

char* SheltonsName[] =
{
	"null",
	"Lucidy",
	"Sereno",
	"Fadeo",
	"Sparky",
	"Raident",
	"Transparo",
	"Murki",
	"Devine",
	"Celesto",
	"Mirage",
	"Inferna",
	"Enigma",
	"Bellum",
	"Ordo"
};

PDIRECT3DTEXTURE9 my_texture2[15];
LPDIRECT3DTEXTURE9 Image2[15];

extern LPDIRECT3DTEXTURE9 LoadDibSurfaceOffscreen(char* Filename);
extern bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);

char Path2[64] = { 0 };
bool first = true;

void readTextures()
{
	int w = 0;
	int h = 0;

	for (int x = 1; x <= 14; x++)
	{
		sprintf_s(Path2, sizeof(Path2), "game\\images\\Mix\\%d.png", x);
		bool ret = LoadTextureFromFile2(Path2, &my_texture2[x], &w, &h);
		IM_ASSERT(ret);

		Image2[x] = LoadDibSurfaceOffscreen(Path2);
	}

	first = false;
}

INT MixWindow::ReadMix()
{
	allMix.clear();

	if (first)
		readTextures();

	MixInfo Mix;

	// ARMAS

	Mix.ID = 1;
	Mix.mixName = "Frost";
	Mix.description = u8"ATK MIN +7 - MAX +8; TAXA DE ATK +35; CRÍT. +2; MANA +10";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 2;
	Mix.mixName = "Evento";
	Mix.description = u8"ATK MIN +8 - MAX +9; TAXA DE ATK +40; CRÍT. +3; MANA +15";
	Mix.mixType = MixType::Weapons;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);


	// ARMADURAS E ROUPÕES

	Mix.ID = 3;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +80; ABS. +2.0; HP +10";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 4;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +120; ABS. +3.0; HP +15";
	Mix.mixType = MixType::Armors;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// ESCUDOS

	Mix.ID = 5;
	Mix.mixName = "Frost";
	Mix.description = u8"BLOQUEIO +2%; ABS. +1.4; HP +10";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 6;
	Mix.mixName = "Evento";
	Mix.description = u8"BLOQUEIO +3%; ABS. +2.0; HP +20";
	Mix.mixType = MixType::Shields;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);


	// ORBITAIS

	Mix.ID = 7;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +20; ABS. +1.4; HP +15, RES +45";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 8;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +55; ABS. +2.0; HP +20, RES +60";
	Mix.mixType = MixType::Orbs;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Raident, 1);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);

	allMix.push_back(Mix);

	// BOTAS

	Mix.ID = 9;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +10; ABS. +1.2; VELOCIDADE +0.9; HP +5; MANA +10; RES +15";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 10;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +15; ABS. +1.8; VELOCIDADE +1.8; HP +10; MANA +15; RES +20";
	Mix.mixType = MixType::Boots;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);


	// LUVAS

	Mix.ID = 11;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +10; ABS. +1.0; HP +5; MANA +10; RES +15;";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 12;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +30; ABS. +2.0; HP +10; MANA +15; RES +20";
	Mix.mixType = MixType::Gloves;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// BRACELETES

	Mix.ID = 13;
	Mix.mixName = "Frost";
	Mix.description = u8"DEF. +15; TAXA DE ATK +30; HP +5; MANA +10; RES +15";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 14;
	Mix.mixName = "Evento";
	Mix.description = u8"DEF. +25; TAXA DE ATK +40; HP +10; MANA +15; RES +20";
	Mix.mixType = MixType::Bracelets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// AMULETOS

	Mix.ID = 15;
	Mix.mixName = "Frost";
	Mix.description = u8"REG. de HP +0.4; REG. de MANA +0.8; HP +4; MANA +8; RES +12";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 16;
	Mix.mixName = "Evento";
	Mix.description = u8"REG. de HP +0.6; REG. de MANA +1.0; HP +6; MANA +10; RES +14";
	Mix.mixType = MixType::Amulets;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	// ANÉIS

	Mix.ID = 17;
	Mix.mixName = "Frost";
	Mix.description = u8"REG. de HP +0.4; HP +2; MANA +6";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Transparo, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 4);

	allMix.push_back(Mix);

	Mix.ID = 18;
	Mix.mixName = "Evento";
	Mix.description = u8"REG. de HP +0.8; HP +6; MANA +10";
	Mix.mixType = MixType::Rings;
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Murki, 2);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Devine, 3);
	Mix.sheltonList[Mix.ID].emplace_back(Sheltons::Celesto, 5);

	allMix.push_back(Mix);

	openFlag = true;

	return TRUE;
}

extern int smScreenWidth;
extern int smScreenHeight;

int ID = 0;

void MixWindow::OpenNpc(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(450, 500), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 260));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();


	if (ImGui::Begin("Lista de Mix", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;

		if (ImGui::BeginTabBar("Listas", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(u8"Armas"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Weapons)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);




						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::Separator();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Armaduras e Robes"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Armors)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}


			if (ImGui::BeginTabItem(u8"Escudos"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Shields)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Orbs"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Orbs)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Botas"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Boots)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Luvas"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Gloves)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Braceletes"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Bracelets)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Amuletos"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Amulets)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem(u8"Anéis"))
			{
				for (auto& Mix : allMix)
				{
					ID = Mix.ID;

					if (Mix.mixType == MixType::Rings)
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Nome: ");
						ImGui::PopStyleColor();
						ImGui::SameLine();
						ImGui::Text(Mix.mixName.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Atributos: ");
						ImGui::PopStyleColor();
						ImGui::Text(Mix.description.c_str());
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Text(u8"Pedras necessárias: ");
						ImGui::PopStyleColor();

						auto match = std::find_if(allMix.begin(), allMix.end(), [](const MixInfo& v) {return v.ID == ID; });
						int index = std::distance(allMix.begin(), match);

						for (int x = 0; x < allMix[index].sheltonList[Mix.ID].size(); x++)
						{
							ImGui::Image(my_texture2[allMix[index].sheltonList[Mix.ID][x].first], ImVec2(16, 16));
							ImGui::SameLine();
							ImGui::Text(SheltonsName[allMix[index].sheltonList[Mix.ID][x].first]);
							ImGui::SameLine();
							char qt[12] = { 0 };
							sprintf_s(qt, "x%d", allMix[index].sheltonList[Mix.ID][x].second);
							ImGui::Text(qt);
						}

						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f));
						ImGui::Separator();
						ImGui::PopStyleColor();
					}
				}

				ImGui::EndTabItem();
			}


			ImGui::EndTabBar();
		}

		ImGui::End();
	}

}
