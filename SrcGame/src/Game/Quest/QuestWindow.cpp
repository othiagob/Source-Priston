#include "..\\sinbaram\\sinLinkHeader.h"
#include "Quest.h"

int lastSelected = 0;
int selected = 1;
char questDescription[512] = { 0 };
char questName[128] = { 0 };
char OverlayDescription[512] = { 0 };
char rangeLvl[32] = { 0 };
int questType = 0;
int questObjective = 0;
int p = 1;

extern QUEST_INFOPCKG questInfoPlayer;

char* Mapas[] = {
u8"Mata das Acácias",
u8"F. Bamboo",
u8"Jardim da Liberdade",
u8"Cidade de Ricarten",
u8"Refúgio dos A.",
u8"Castelo dos Perdidos",
u8"Vila Ruinen",
u8"Terra Maldita",
u8"Terra Esquecida",
u8"Cidade de Navisko",
u8"Oásis",
u8"C. B. A.",
u8"Terra Proibida",
u8"C. A. 1",
u8"C. A. 2",
u8"C. A. 3",
u8"Sala do GM",
u8"F. dos Espíritos",
u8"F. das Ilusões",
u8"Vale Tranquilo",
u8"E. dos Ventos",
u8"Cidade de Pillai",
u8"Templo Maldito 1",
u8"Templo Maldito 2",
u8"C. dos Cogumelos",
u8"Caverna das Abelhas",
u8"Santuário Sombrio",
u8"E. d. F. do Caos",
u8"Coração de Perum",
u8"Eura",
u8"S.o.D",
u8"Vale Galubia",
u8"Sala de Desafios",
u8"Castelo Abençoado",
u8"Lago da Ganância",
u8"Santuario Congelado",
u8"Covil do Kelvezu",
u8"Ilha Perdida",
u8"Templo Perdido",
u8"Mapa 2D",
u8"Torre sem Fim 1",
u8"Torre sem Fim 2",
u8"Templo Maldito 3",
u8"Torre sem Fim 3",
u8"Laboratório Secreto",
u8"Arma Antiga",
u8"Mina de Gelo 1",
u8"Arquipélago Perdido",
u8"Covil do Eragon",
u8"Arena de Batalha",
u8"Núcleo de Ferro",
u8"Abismo do Mar",
};

char* MapasWU8[] = {
"Mata das Acácias",
"F. Bamboo",
"Jardim da Liberdade",
"Cidade de Ricarten",
"Refúgio dos A.",
"Castelo dos Perdidos",
"Vila Ruinen",
"Terra Maldita",
"Terra Esquecida",
"Cidade de Navisko",
"Oásis",
"C. B. A.",
"Terra Proibida",
"C. A. 1",
"C. A. 2",
"C. A. 3",
"Sala do GM",
"F. dos Espíritos",
"F. das Ilusões",
"Vale Tranquilo",
"E. dos Ventos",
"Cidade de Pillai",
"Templo Maldito 1",
"Templo Maldito 2",
"C. dos Cogumelos",
"Caverna das Abelhas",
"Santuário Sombrio",
"E. d. F. do Caos",
"Coração de Perum",
"Eura",
"S.o.D",
"Vale Galubia",
"Sala de Desafios",
"Castelo Abençoado",
"Lago da Ganância",
"Santuario Congelado",
"Covil do Kelvezu",
"Ilha Perdida",
"Templo Perdido",
"Mapa 2D",
"Torre sem Fim 1",
"Torre sem Fim 2",
"Templo Maldito 3",
"Torre sem Fim 3",
"Laboratório Secreto",
"Arma Antiga",
"Mina de Gelo 1",
"Arquipélago Perdido",
"Covil do Eragon",
"Arena de Batalha",
"Núcleo de Ferro",
"Abismo do Mar",
};

static void Ajuda(const char* desc)
{
	ImGui::TextDisabled("Ajuda");

	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

long getUTF8size2(const wchar_t* string) {
	if (!string)
		return 0;
	long res = 0;
	for (; *string; string++) {
		if (*string < 0x80)
			res++;
		else if (*string < 0x800)
			res += 2;
		else
			res += 3;
	}
	return res;
}

char* WChar_to_UTF82(const wchar_t* string) {
	long fSize = getUTF8size2(string);
	char* res = new char[fSize + 1];
	res[fSize] = 0;
	if (!string)
		return res;
	long b = 0;
	for (; *string; string++, b++) {
		if (*string < 0x80)
			res[b] = (char)*string;
		else if (*string < 0x800) {
			res[b++] = (*string >> 6) | 192;
			res[b] = *string & 63 | 128;
		}
		else {
			res[b++] = (*string >> 12) | 224;
			res[b++] = ((*string & 4095) >> 6) | 128;
			res[b] = *string & 63 | 128;
		}
	}
	return res;
}

#include "..\ConfirmationBox.h"

int cancelQuest(std::string title, int questID, int QuestType)
{
	int i = 1;
	_ConfirmationBox.SetTitle(title);
	_ConfirmationBox.SetText("Deseja cancelar", "Este Desafio?");
	_ConfirmationBox.SetCallback([questID, QuestType](bool confirmed)
		{
			if (confirmed)
				Quest::GetInstance()->cancelQuest(questID, QuestType);
			else
				return FALSE;
		});

	_ConfirmationBox.Start();

	return FALSE;
}

void QuestWindow::questOverlay(bool* p_open) {
	const float DISTANCE = 40.0f;
	static int corner = 1;
	int y = 0;
	float h, s, v;
	int x = 0;

	ImGuiIO& io = ImGui::GetIO();

	if (corner != -1)
	{
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	}

	ImGui::SetNextWindowBgAlpha(0.75f);

	ImGui::StyleColorArmageddon();

	if (ImGui::Begin("Desafios em andamento", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		extern QUESTPCKG allQuests;
		char questProgress[32];
		x = 0;

		for (std::vector<QUESTPCKG*>::iterator it = Quest::GetInstance()->vQuests.begin(); it < Quest::GetInstance()->vQuests.end(); it++)
		{
			if (questInfoPlayer.QuestPckg[x].isActive || questInfoPlayer.QuestPckg[x].isReadyToComplete) {

				for (auto getRightQuest : Quest::GetInstance()->vQuests)
				{
					if (getRightQuest && getRightQuest->QuestPckg[y].questID == x)
						break;

					y++;
				}

				//ImGui::Text(allQuests.QuestPckg[y].questName);

				if (questInfoPlayer.QuestPckg[x].isActive) {
					ImGui::ColorConvertRGBtoHSV(0, 178, 238, h, s, v);
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
				}
				else
				{
					ImGui::ColorConvertRGBtoHSV(139, 0, 139, h, s, v);
					ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
				}

				sprintf_s(OverlayDescription, sizeof(OverlayDescription), "%s", allQuests.QuestPckg[y].questDescription);

				// Converte a descrição da quest para UTF8
				CA2W pszWide2(OverlayDescription);
				wsprintf(OverlayDescription, "%s", WChar_to_UTF82(pszWide2));
				ImGui::Text(OverlayDescription);

				ImGui::PopStyleColor();

				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

				char monsterName[32] = { 0 };
				if (allQuests.QuestPckg[y].monsterNum[0] > 0) {
					CA2W pszWide2(allQuests.QuestPckg[y].wantedMonster[0]);
					wsprintf(monsterName, "%s", WChar_to_UTF82(pszWide2));

					if (STRINGCOMPAREI(monsterName, "0"))
						sprintf_s(monsterName, sizeof(monsterName), "%s", "Monstros");

					ImGui::Text(monsterName);
					ImGui::SameLine();
					sprintf(questProgress, "%d/%d", questInfoPlayer.QuestPckg[x].questProgress[0], allQuests.QuestPckg[y].monsterNum[0]);
					ImGui::Text(questProgress);
				}

				if (allQuests.QuestPckg[y].monsterNum[1] > 0) {
					CA2W pszWide2(allQuests.QuestPckg[y].wantedMonster[1]);
					wsprintf(monsterName, "%s", WChar_to_UTF82(pszWide2));

					if (STRINGCOMPAREI(monsterName, "0"))
						sprintf_s(monsterName, sizeof(monsterName), "%s", "Monstros");

					ImGui::Text(monsterName);
					ImGui::SameLine();
					sprintf(questProgress, "%d/%d", questInfoPlayer.QuestPckg[x].questProgress[1], allQuests.QuestPckg[y].monsterNum[1]);
					ImGui::Text(questProgress);
				}

				if (allQuests.QuestPckg[y].monsterNum[2] > 0) {
					CA2W pszWide2(allQuests.QuestPckg[y].wantedMonster[2]);
					wsprintf(monsterName, "%s", WChar_to_UTF82(pszWide2));

					if (STRINGCOMPAREI(monsterName, "0"))
						sprintf_s(monsterName, sizeof(monsterName), "%s", "Monstros");

					ImGui::Text(monsterName);
					ImGui::SameLine();
					sprintf(questProgress, "%d/%d", questInfoPlayer.QuestPckg[x].questProgress[2], allQuests.QuestPckg[y].monsterNum[2]);
					ImGui::Text(questProgress);
				}

				if (allQuests.QuestPckg[y].playersToKill > 0) {
					ImGui::Text("Jogadores mortos");
					ImGui::SameLine();
					sprintf(questProgress, "%d/%d", questInfoPlayer.QuestPckg[x].killProgress, allQuests.QuestPckg[y].playersToKill);
					ImGui::Text(questProgress);
				}

				ImGui::PopStyleColor();
				ImGui::Separator();
			}
			x++;
		}

		ImGui::End();
	}

}

void QuestWindow::openWindow(bool* p_open) {

	float h, s, v;
	int lastid = 0;

	ImGui::SetNextWindowSize(ImVec2(550, 440), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.90f);

	ImGui::StyleColorArmageddon();

	if (ImGui::Begin("Desafios", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		ImGui::BeginGroup();

		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			////// Quests únicas /////////
			if (ImGui::BeginTabItem(u8"Quest Únicas"))
			{
				ImGui::BeginChild("Painel Lateral", ImVec2(200, 0), true);

				int x = 0;

				for (auto Quests : Quest::GetInstance()->vQuests)
				{
					///////////////////////////////////////////////
					// Cores conforme o status da Quest :
					// Pronta para entregar = Roxo
					// Quest em andamento = Azul
					// Quest completada = Verde
					// Quest disponível = Vermelho
					///////////////////////////////////////////////

					if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isReadyToComplete) {
						ImGui::ColorConvertRGBtoHSV(139, 0, 139, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isActive) {
						ImGui::ColorConvertRGBtoHSV(0, 178, 238, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isComplete) {
						ImGui::ColorConvertRGBtoHSV(0, 255, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else {
						ImGui::ColorConvertRGBtoHSV(255, 0, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}

					if ((Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_UNIQUE_SOLO || Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_UNIQUE_SOLOPARTY) && Quests->QuestPckg[x].questID) {

						if (selected == 1) {
							sprintf_s(questDescription, sizeof(questDescription), "%s", Quests->QuestPckg[0].questDescription);
							CA2W pszWide2(questDescription);
							wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));
							sprintf_s(rangeLvl, sizeof(rangeLvl), "%d-%d", Quests->QuestPckg[0].minLevel, Quests->QuestPckg[0].maxLevel);
						}

						sprintf_s(questName, sizeof(questName), "%s", Quests->QuestPckg[x].questName);
						CA2W pszWide2(questName);
						wsprintf(questName, "%s", WChar_to_UTF82(pszWide2));

						if (ImGui::Selectable(questName, selected == Quests->QuestPckg[x].questID)) {
							selected = Quests->QuestPckg[x].questID;

							if (lastid != selected) p = 1;

							if (p) {
								lastid = selected;
								ZeroMemory(questDescription, sizeof(questDescription));
								sprintf_s(questDescription, sizeof(questDescription), "%s", Quests->QuestPckg[x].questDescription);

								// Converte a descrição para UTF8
								CA2W pszWide2(questDescription);
								wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));

								sprintf_s(rangeLvl, sizeof(rangeLvl), "%d-%d", Quests->QuestPckg[x].minLevel, Quests->QuestPckg[x].maxLevel);

								questType = Quests->QuestPckg[x].questType;
								questObjective = Quests->QuestPckg[x].questObjective;
								p = 0;
							}
						}
					}

					ImGui::PopStyleColor();
					x++;
				}

				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("Detalhes da Quest", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
				ImGui::Separator();

				///////////////////////////////////////////////////////
				/////////// ABA DE DETALHES DE QUESTS ÚNICAS //////////
				///////////////////////////////////////////////////////

				if (ImGui::BeginTabBar("##TabsUnicas", ImGuiTabBarFlags_None))
				{
					ImGui::SameLine();
					ImGui::SetCursorPosX(300);
					ImGui::SetCursorPosY(5);
					Ajuda(u8"Itens em vermelho: Quests disponíveis que você ainda não iniciou.\nItens em Azul: Quests em andamento não completadas\nItens em Roxo: Quests prontas para entregar\nItens em Verde: Quests já completadas.");

					if (ImGui::BeginTabItem("Detalhes"))
					{
						ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
						ImGui::Text(u8"Tipo: Única e Solo");
						ImGui::SameLine();
						ImGui::Text(u8"Nível requerido:");
						ImGui::SameLine();
						ImGui::Text(rangeLvl);
						ImGui::Spacing();

						ImGui::PopStyleColor();
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

						ImGui::TextWrapped(questDescription);
						ImGui::Separator();
						ImGui::PopStyleColor();

						switch (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questObjective)
						{
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_MONSTER:
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_ANY_MONSTER:

							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Eliminar os monstros\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[0] > 0)
							{
								char monsternum[64];

								sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[0]);

								if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[0], "0") == 0) {
									ImGui::TextWrapped("Nome: Qualquer monstro");
								}
								else {
									CA2W pszWide2(monsternum);
									wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
									ImGui::TextWrapped(monsternum);
								}

								sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[0]);
								ImGui::TextWrapped(monsternum);

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[0] > 0) {
									char monsterMap[32];
									sprintf(monsterMap, "Mapa: %s\n", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[0]]);
									ImGui::TextWrapped(monsterMap);
									ImGui::Separator();
								}
								else {
									ImGui::TextWrapped("Mapa: Qualquer mapa\n");
									ImGui::Separator();
								}



								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[1] > 0)
								{
									sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[1]);

									if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[1], "0") == 0) {
										ImGui::TextWrapped("Nome: Qualquer monstro");
									}
									else {
										CA2W pszWide2(monsternum);
										wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
										ImGui::TextWrapped(monsternum);
									}

									sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[1]);
									ImGui::TextWrapped(monsternum);

									if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[1] > 0) {
										char monsterMap[32];
										sprintf(monsterMap, "Mapa: %s\n", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[1]]);
										ImGui::TextWrapped(monsterMap);
										ImGui::Separator();
									}
									else {
										ImGui::TextWrapped("Mapa: Qualquer mapa\n");
										ImGui::Separator();
									}
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[2] > 0)
								{
									sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[2]);

									if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[2], "0") == 0) {
										ImGui::TextWrapped("Nome: Qualquer monstro");
									}
									else {
										CA2W pszWide2(monsternum);
										wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
										ImGui::TextWrapped(monsternum);
									}

									sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[2]);
									ImGui::TextWrapped(monsternum);

									if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[2] > 0) {
										char monsterMap[32];
										sprintf(monsterMap, "Mapa: %s", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[2]]);
										ImGui::TextWrapped(monsterMap);
									}
									else {
										ImGui::TextWrapped("Mapa: Qualquer mapa");
									}
								}
							}

							ImGui::PopStyleColor();

							break;

						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_PLAYER:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Eliminar Jogadores\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							char playersToKill[10];

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].playersToKill > 0)
							{
								ImGui::TextWrapped(u8"Jogadores para eliminar:");
								sprintf(playersToKill, "%d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].playersToKill);
								ImGui::SameLine();
								ImGui::TextWrapped(playersToKill);
							}

							ImGui::PopStyleColor();

							break;
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_GET_ITEM:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Consiga os itens necessários\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

							char qtItens[64];

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens > 0) {

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[0] > 0) {

									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[0]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[0]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[1] > 0) {

									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[1]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[1]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[2] > 0) {
									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[2]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[2]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}
							}

							ImGui::PopStyleColor();

							break;
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Recompensas"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
						ImGui::TextWrapped(u8"Cada desafio tem sua premiação baseada no nível de dificuldade para completá-lo\n\nConfira abaixo as premiações para esse desafio\n\n");
						ImGui::Separator();
						ImGui::PopStyleColor();

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardGold > 0) {
							char ouro[32];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em ouro\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(ouro, "Ouro: %d\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardGold);
							ImGui::TextWrapped(ouro);
							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardExp > 0) {
							char exp[32];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em Experiência\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(exp, u8"Experiência: %I64d\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardExp);
							ImGui::TextWrapped(exp);
							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[0][0] != '0') {
							char szItem[64];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em Itens\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(szItem, "Item 1: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[0]);
							CA2W pszWide2(szItem);
							wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
							ImGui::TextWrapped(szItem);

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[1][0] != '0') {
								sprintf(szItem, "Item 2: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[1]);
								CA2W pszWide2(szItem);
								wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
								ImGui::TextWrapped(szItem);
							}

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[2][0] != '0') {
								sprintf(szItem, "Item 3: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[2]);
								CA2W pszWide2(szItem);
								wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
								ImGui::TextWrapped(szItem);
							}

							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						//ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
						ImGui::EndTabItem();
					}

					if (!questInfoPlayer.QuestPckg[selected].isComplete) {
						if (questInfoPlayer.QuestPckg[selected].isReadyToComplete) {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Concluir desafio", ImVec2(120, 30))) { Quest::GetInstance()->finishQuest(selected, questType, questObjective); };
						}
						else if (questInfoPlayer.QuestPckg[selected].isActive) {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Cancelar Desafio", ImVec2(120, 30))) { cancelQuest(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questName, selected, questType); };
						}
						else {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Aceitar Desafio", ImVec2(120, 30))) { Quest::GetInstance()->startNewQuest(selected, questType); };
						}
					}
					else {
						char msgCompleted[150];
						ImGui::ColorConvertRGBtoHSV(0, 255, 0, h, s, v);
						wsprintf(msgCompleted, "\n\n Desafio completado em: %02d/%02d/%02d - %02d:%02d:%02d", questInfoPlayer.QuestPckg[selected].EndTime.wDay, questInfoPlayer.QuestPckg[selected].EndTime.wMonth, questInfoPlayer.QuestPckg[selected].EndTime.wYear, questInfoPlayer.QuestPckg[selected].EndTime.wHour, questInfoPlayer.QuestPckg[selected].EndTime.wMinute, questInfoPlayer.QuestPckg[selected].EndTime.wSecond);
						ImGui::TextColored((ImVec4)ImColor::HSV(h, s, v), msgCompleted);
					}

					ImGui::EndTabItem();
					ImGui::EndChild();
					ImGui::EndTabBar();
				}
			}


			////// Quests Diárias /////////
			if (ImGui::BeginTabItem(u8"Quest Diárias"))
			{
				ImGui::BeginChild("Painel Lateral", ImVec2(200, 0), true);

				int x = 0;

				for (auto Quests : Quest::GetInstance()->vQuests)
				{

					///////////////////////////////////////////////
					// Cores conforme o status da Quest :
					// Pronta para entregar = Roxo
					// Quest em andamento = Azul
					// Quest completada = Verde
					// Quest disponível = Vermelho
					///////////////////////////////////////////////

					if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isReadyToComplete) {
						ImGui::ColorConvertRGBtoHSV(139, 0, 139, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isActive) {
						ImGui::ColorConvertRGBtoHSV(0, 178, 238, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isComplete) {
						ImGui::ColorConvertRGBtoHSV(0, 255, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else {
						ImGui::ColorConvertRGBtoHSV(255, 0, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}

					if ((Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_DAILY_SOLO || Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_DAILY_SOLOPARTY) && Quests->QuestPckg[x].questID) {
						if (selected == 1) {
							sprintf_s(questDescription, sizeof(questDescription), "%s", Quests->QuestPckg[0].questDescription);
							CA2W pszWide2(questDescription);
							wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));
							sprintf_s(rangeLvl, sizeof(rangeLvl), "%d-%d", Quests->QuestPckg[0].minLevel, Quests->QuestPckg[0].maxLevel);
						}

						sprintf_s(questName, sizeof(questName), "%s", Quests->QuestPckg[x].questName);
						CA2W pszWide2(questName);
						wsprintf(questName, "%s", WChar_to_UTF82(pszWide2));

						if (ImGui::Selectable(questName, selected == Quests->QuestPckg[x].questID)) {
							selected = Quests->QuestPckg[x].questID;

							if (lastid != selected) p = 1;

							if (p) {
								lastid = selected;
								ZeroMemory(questDescription, sizeof(questDescription));
								sprintf_s(questDescription, sizeof(questDescription), "%s", Quests->QuestPckg[x].questDescription);

								// Converte a descrição para UTF8
								CA2W pszWide2(questDescription);
								wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));

								sprintf_s(rangeLvl, sizeof(rangeLvl), "%d-%d", Quests->QuestPckg[x].minLevel, Quests->QuestPckg[x].maxLevel);

								questType = Quests->QuestPckg[x].questType;
								questObjective = Quests->QuestPckg[x].questObjective;
								p = 0;
							}

						}
					}

					ImGui::PopStyleColor();
					x++;
				}

				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("Detalhes da Quest", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
				ImGui::Separator();

				///////////////////////////////////////////////////////
				/////////// ABA DE DETALHES DE QUESTS DIÁRIAS //////////
				///////////////////////////////////////////////////////

				if (ImGui::BeginTabBar("##TabsUnicas", ImGuiTabBarFlags_None))
				{
					ImGui::SameLine();
					ImGui::SetCursorPosX(300);
					ImGui::SetCursorPosY(5);
					Ajuda(u8"Itens em vermelho: Quests disponíveis que você ainda não iniciou.\nItens em Azul: Quests em andamento não completadas\nItens em Roxo: Quests prontas para entregar\nItens em Verde: Quests já completadas.");
					if (ImGui::BeginTabItem("Detalhes"))
					{
						ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
						ImGui::Text(u8"Tipo: Diária e Solo");
						ImGui::SameLine();
						ImGui::Text(u8"Nível requerido:");
						ImGui::SameLine();
						ImGui::Text(rangeLvl);
						ImGui::Spacing();

						ImGui::PopStyleColor();
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

						char questDescription[512];
						sprintf_s(questDescription, sizeof(questDescription), "%s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questDescription);
						CA2W pszWide2(questDescription);
						wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));

						ImGui::TextWrapped(questDescription);
						ImGui::PopStyleColor();
						ImGui::Separator();

						switch (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questObjective) {
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_MONSTER:
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_ANY_MONSTER:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Eliminar os monstros\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[0] > 0)
							{
								char monsternum[64];

								sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[0]);

								if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[0], "0") == 0) {
									ImGui::TextWrapped("Nome: Qualquer monstro");
								}
								else {
									CA2W pszWide2(monsternum);
									wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
									ImGui::TextWrapped(monsternum);
								}

								sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[0]);
								ImGui::TextWrapped(monsternum);

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[0] > 0) {
									char monsterMap[32];
									sprintf(monsterMap, "Mapa: %s\n", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[0]]);
									ImGui::TextWrapped(monsterMap);
									ImGui::Separator();
								}
								else {
									ImGui::TextWrapped("Mapa: Qualquer mapa\n");
									ImGui::Separator();
								}



								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[1] > 0)
								{
									sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[1]);

									if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[1], "0") == 0) {
										ImGui::TextWrapped("Nome: Qualquer monstro");
									}
									else {
										CA2W pszWide2(monsternum);
										wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
										ImGui::TextWrapped(monsternum);
									}

									sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[1]);
									ImGui::TextWrapped(monsternum);

									if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[1] > 0) {
										char monsterMap[32];
										sprintf(monsterMap, "Mapa: %s\n", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[1]]);
										ImGui::TextWrapped(monsterMap);
										ImGui::Separator();
									}
									else {
										ImGui::TextWrapped("Mapa: Qualquer mapa\n");
										ImGui::Separator();
									}
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[2] > 0)
								{
									sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[2]);


									if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[2], "0") == 0) {
										ImGui::TextWrapped("Nome: Qualquer monstro");
									}
									else {
										CA2W pszWide2(monsternum);
										wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
										ImGui::TextWrapped(monsternum);
									}

									sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[2]);
									ImGui::TextWrapped(monsternum);

									if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[2] > 0) {
										char monsterMap[32];
										sprintf(monsterMap, "Mapa: %s", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[2]]);
										ImGui::TextWrapped(monsterMap);
									}
									else {
										ImGui::TextWrapped("Mapa: Qualquer mapa");
									}
								}
							}

							ImGui::PopStyleColor();
							break;

						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_PLAYER:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Eliminar Jogadores\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							char playersToKill[10];

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].playersToKill > 0)
							{
								ImGui::TextWrapped(u8"Jogadores para eliminar:");
								sprintf(playersToKill, "%d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].playersToKill);
								ImGui::SameLine();
								ImGui::TextWrapped(playersToKill);
							}
							ImGui::PopStyleColor();
							break;
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_GET_ITEM:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Consiga os itens necessários\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

							char qtItens[64];

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens > 0) {

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[0] > 0) {

									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[0]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[0]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[1] > 0) {

									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[1]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[1]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[2] > 0) {
									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[2]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[2]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}
							}
							ImGui::PopStyleColor();
							break;
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Recompensas"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
						ImGui::TextWrapped(u8"Cada desafio tem sua premiação baseada no nível de dificuldade para completá-lo\n\nConfira abaixo as premiações para esse desafio\n\n");
						ImGui::PopStyleColor();
						ImGui::Separator();

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardGold > 0) {
							char ouro[32];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em ouro\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(ouro, "Ouro: %d\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardGold);
							ImGui::TextWrapped(ouro);
							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardExp > 0) {
							char exp[32];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em Experiência\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(exp, u8"Experiência: %I64d\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardExp);
							ImGui::TextWrapped(exp);
							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[0][0] != '0') {
							char szItem[64];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em Itens\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(szItem, "Item 1: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[0]);
							CA2W pszWide2(szItem);
							wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
							ImGui::TextWrapped(szItem);

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[1][0] != '0') {
								sprintf(szItem, "Item 2: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[1]);
								CA2W pszWide2(szItem);
								wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
								ImGui::TextWrapped(szItem);
							}

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[2][0] != '0') {
								sprintf(szItem, "Item 3: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[2]);
								CA2W pszWide2(szItem);
								wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
								ImGui::TextWrapped(szItem);
							}

							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						ImGui::EndTabItem();
					}

					if (!questInfoPlayer.QuestPckg[selected].isComplete) {
						if (questInfoPlayer.QuestPckg[selected].isReadyToComplete) {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Concluir desafio", ImVec2(120, 30))) { Quest::GetInstance()->finishQuest(selected, questType, questObjective); };
						}
						else if (questInfoPlayer.QuestPckg[selected].isActive) {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Cancelar Desafio", ImVec2(120, 30))) { cancelQuest(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questName, selected, questType); };
						}
						else {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Aceitar Desafio", ImVec2(120, 30))) { Quest::GetInstance()->startNewQuest(selected, questType); };
						}
					}
					else {
						char msgCompleted[150];
						ImGui::ColorConvertRGBtoHSV(0, 255, 0, h, s, v);
						wsprintf(msgCompleted, "\n\n Desafio completado em: %02d/%02d/%02d - %02d:%02d:%02d", questInfoPlayer.QuestPckg[selected].EndTime.wDay, questInfoPlayer.QuestPckg[selected].EndTime.wMonth, questInfoPlayer.QuestPckg[selected].EndTime.wYear, questInfoPlayer.QuestPckg[selected].EndTime.wHour, questInfoPlayer.QuestPckg[selected].EndTime.wMinute, questInfoPlayer.QuestPckg[selected].EndTime.wSecond);
						ImGui::TextColored((ImVec4)ImColor::HSV(h, s, v), msgCompleted);
					}

					ImGui::EndTabItem();
					ImGui::EndChild();
					ImGui::EndTabBar();
				}
			}

			////// Quests Repetitivas /////////
			if (ImGui::BeginTabItem(u8"Quest repetitivas"))
			{

				ImGui::BeginChild("Painel Lateral", ImVec2(200, 0), true);

				int x = 0;

				for (auto Quests : Quest::GetInstance()->vQuests)
				{

					///////////////////////////////////////////////
					// Cores conforme o status da Quest :
					// Pronta para entregar = Roxo
					// Quest em andamento = Azul
					// Quest completada = Verde
					// Quest disponível = Vermelho
					///////////////////////////////////////////////

					if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isReadyToComplete) {
						ImGui::ColorConvertRGBtoHSV(139, 0, 139, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isActive) {
						ImGui::ColorConvertRGBtoHSV(0, 178, 238, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else if (questInfoPlayer.QuestPckg[Quests->QuestPckg[x].questID].isComplete) {
						ImGui::ColorConvertRGBtoHSV(0, 255, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}
					else {
						ImGui::ColorConvertRGBtoHSV(255, 0, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
					}

					if ((Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_REPEAT_SOLOPARTY || Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_REPEAT_PARTY || Quests->QuestPckg[x].questType == Quest::QUEST_TYPES::QUEST_REPEAT_SOLO) && Quests->QuestPckg[x].questID)
					{
						if (selected == 1) {
							sprintf_s(questDescription, sizeof(questDescription), "%s", Quests->QuestPckg[0].questDescription);
							CA2W pszWide2(questDescription);
							wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));
							sprintf_s(rangeLvl, sizeof(rangeLvl), "%d-%d", Quests->QuestPckg[0].minLevel, Quests->QuestPckg[0].maxLevel);
						}

						sprintf_s(questName, sizeof(questName), "%s", Quests->QuestPckg[x].questName);
						CA2W pszWide2(questName);
						wsprintf(questName, "%s", WChar_to_UTF82(pszWide2));

						if (ImGui::Selectable(questName, selected == Quests->QuestPckg[x].questID)) {
							selected = Quests->QuestPckg[x].questID;

							if (lastid != selected) p = 1;

							if (p) {
								lastid = selected;
								ZeroMemory(questDescription, sizeof(questDescription));
								sprintf_s(questDescription, sizeof(questDescription), "%s", Quests->QuestPckg[x].questDescription);

								// Converte a descrição para UTF8
								CA2W pszWide2(questDescription);
								wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));

								sprintf_s(rangeLvl, sizeof(rangeLvl), "%d-%d", Quests->QuestPckg[x].minLevel, Quests->QuestPckg[x].maxLevel);

								questType = Quests->QuestPckg[x].questType;
								questObjective = Quests->QuestPckg[x].questObjective;
								p = 0;
							}

						}
					}

					ImGui::PopStyleColor();
					x++;
				}

				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("Detalhes da Quest", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
				ImGui::Separator();

				/////////////////////////////////////////////////////////
				/////////// ABA DE DETALHES DE QUESTS EM PARTY //////////
				/////////////////////////////////////////////////////////


				if (ImGui::BeginTabBar("##TabsemGrupo", ImGuiTabBarFlags_None))
				{
					ImGui::SameLine();
					ImGui::SetCursorPosX(300);
					ImGui::SetCursorPosY(5);
					Ajuda(u8"Itens em vermelho: Quests disponíveis que você ainda não iniciou.\nItens em Azul: Quests em andamento não completadas\nItens em Roxo: Quests prontas para entregar\nItens em Verde: Quests já completadas.");
					if (ImGui::BeginTabItem("Detalhes"))
					{
						ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
						ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questType == Quest::QUEST_TYPES::QUEST_REPEAT_SOLOPARTY)
							ImGui::Text("Tipo: Repetitiva Solo/Grupo");
						else if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questType == Quest::QUEST_TYPES::QUEST_REPEAT_SOLO)
							ImGui::Text("Tipo: Repetitiva Solo");
						else if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questType == Quest::QUEST_TYPES::QUEST_REPEAT_PARTY)
							ImGui::Text("Tipo: Repetitiva em Grupo");

						ImGui::SameLine();
						ImGui::Text(u8"Nível requerido:");
						ImGui::SameLine();
						ImGui::Text(rangeLvl);
						ImGui::Spacing();

						ImGui::PopStyleColor();
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

						char questDescription[512];
						sprintf_s(questDescription, sizeof(questDescription), "%s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questDescription);
						CA2W pszWide2(questDescription);
						wsprintf(questDescription, "%s", WChar_to_UTF82(pszWide2));

						ImGui::TextWrapped(questDescription);
						ImGui::PopStyleColor();
						ImGui::Separator();

						switch (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questObjective) {
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_MONSTER:
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_ANY_MONSTER:

							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Eliminar os monstros\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[0] > 0)
							{
								char monsternum[64];

								sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[0]);


								if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[0], "0") == 0) {
									ImGui::TextWrapped("Nome: Qualquer monstro");
								}
								else {
									CA2W pszWide2(monsternum);
									wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
									ImGui::TextWrapped(monsternum);
								}

								sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[0]);
								ImGui::TextWrapped(monsternum);

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[0] > 0) {
									char monsterMap[32];
									sprintf(monsterMap, "Mapa: %s\n", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[0]]);
									ImGui::TextWrapped(monsterMap);
									ImGui::Separator();
								}
								else {
									ImGui::TextWrapped("Mapa: Qualquer mapa\n");
									ImGui::Separator();
								}



								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[1] > 0)
								{
									sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[1]);


									if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[1], "0") == 0) {
										ImGui::TextWrapped("Nome: Qualquer monstro");
									}
									else {
										CA2W pszWide2(monsternum);
										wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
										ImGui::TextWrapped(monsternum);
									}

									sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[1]);
									ImGui::TextWrapped(monsternum);

									if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[1] > 0) {
										char monsterMap[32];
										sprintf(monsterMap, "Mapa: %s\n", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[1]]);
										ImGui::TextWrapped(monsterMap);
										ImGui::Separator();
									}
									else {
										ImGui::TextWrapped("Mapa: Qualquer mapa\n");
										ImGui::Separator();
									}
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[2] > 0)
								{
									sprintf(monsternum, "Nome: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[2]);


									if (lstrcmpi(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].wantedMonster[2], "0") == 0) {
										ImGui::TextWrapped("Nome: Qualquer monstro");
									}
									else {
										CA2W pszWide2(monsternum);
										wsprintf(monsternum, "%s", WChar_to_UTF82(pszWide2));
										ImGui::TextWrapped(monsternum);
									}


									sprintf(monsternum, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterNum[2]);
									ImGui::TextWrapped(monsternum);

									if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[2] > 0) {
										char monsterMap[32];
										sprintf(monsterMap, "Mapa: %s", Mapas[Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].monsterMap[2]]);
										ImGui::TextWrapped(monsterMap);
									}
									else {
										ImGui::TextWrapped("Mapa: Qualquer mapa");
									}
								}
							}

							ImGui::PopStyleColor();
							break;

						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_KILL_PLAYER:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Eliminar Jogadores\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							char playersToKill[10];

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].playersToKill > 0)
							{
								ImGui::TextWrapped(u8"Jogadores para eliminar:");
								sprintf(playersToKill, "%d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].playersToKill);
								ImGui::SameLine();
								ImGui::TextWrapped(playersToKill);
							}
							ImGui::PopStyleColor();
							break;
						case Quest::QUEST_OBJECTIVE_TYPES::QUEST_GET_ITEM:
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Objetivo: Consiga os itens necessários\n\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));

							char qtItens[64];

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens > 0) {

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[0] > 0) {

									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[0]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[0]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[1] > 0) {

									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[1]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[1]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}

								if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[2] > 0) {
									sprintf(qtItens, "Item: %s", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].itemDescription[2]);
									ImGui::TextWrapped(qtItens);
									sprintf(qtItens, "Quantidade: %d", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].qtItens[2]);
									ImGui::TextWrapped(qtItens);
									ImGui::Separator();
								}
							}
							ImGui::PopStyleColor();
							break;
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Recompensas"))
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
						ImGui::TextWrapped(u8"Cada desafio tem sua premiação baseada no nível de dificuldade para completá-lo\n\nConfira abaixo as premiações para esse desafio\n\n");
						ImGui::PopStyleColor();
						ImGui::Separator();

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardGold > 0) {
							char ouro[32];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em ouro\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(ouro, "Ouro: %d\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardGold);
							ImGui::TextWrapped(ouro);
							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardExp > 0) {
							char exp[32];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em Experiência\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(exp, u8"Experiência: %I64d\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardExp);
							ImGui::TextWrapped(exp);
							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[0][0] != '0') {
							char szItem[64];
							ImGui::ColorConvertRGBtoHSV(255, 55, 0, h, s, v);
							ImGui::PushStyleColor(ImGuiCol_Text, (ImVec4)ImColor::HSV(h, s, v));
							ImGui::TextWrapped(u8"Premiação em Itens\n");

							ImGui::PopStyleColor();
							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f));
							sprintf(szItem, "Item 1: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[0]);
							CA2W pszWide2(szItem);
							wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
							ImGui::TextWrapped(szItem);

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[1][0] != '0') {
								sprintf(szItem, "Item 2: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[1]);
								CA2W pszWide2(szItem);
								wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
								ImGui::TextWrapped(szItem);
							}

							if (Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[2][0] != '0') {
								sprintf(szItem, "Item 3: %s\n", Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].rewardItem[2]);
								CA2W pszWide2(szItem);
								wsprintf(szItem, "%s", WChar_to_UTF82(pszWide2));
								ImGui::TextWrapped(szItem);
							}

							ImGui::PopStyleColor();
							ImGui::Separator();
						}

						ImGui::EndTabItem();

					}

					if (!questInfoPlayer.QuestPckg[selected].isComplete) {
						if (questInfoPlayer.QuestPckg[selected].isReadyToComplete) {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Concluir desafio", ImVec2(120, 30))) { Quest::GetInstance()->finishQuest(selected, questType, questObjective); };
						}
						else if (questInfoPlayer.QuestPckg[selected].isActive) {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Cancelar Desafio", ImVec2(120, 30))) { cancelQuest(Quest::GetInstance()->vQuests[0]->QuestPckg[selected - 1].questName, selected, questType); };
						}
						else {
							ImGui::SetCursorPos(ImVec2(100, 320));
							if (ImGui::Button("Aceitar Desafio", ImVec2(120, 30))) { Quest::GetInstance()->startNewQuest(selected, questType); };
						}
					}
					else {
						char msgCompleted[150];
						ImGui::ColorConvertRGBtoHSV(0, 255, 0, h, s, v);
						wsprintf(msgCompleted, "\n\n Desafio completado em: %02d/%02d/%02d - %02d:%02d:%02d", questInfoPlayer.QuestPckg[selected].EndTime.wDay, questInfoPlayer.QuestPckg[selected].EndTime.wMonth, questInfoPlayer.QuestPckg[selected].EndTime.wYear, questInfoPlayer.QuestPckg[selected].EndTime.wHour, questInfoPlayer.QuestPckg[selected].EndTime.wMinute, questInfoPlayer.QuestPckg[selected].EndTime.wSecond);
						ImGui::TextColored((ImVec4)ImColor::HSV(h, s, v), msgCompleted);
					}

					ImGui::EndTabItem();
					ImGui::EndChild();
					ImGui::EndTabBar();
				}

			}

			ImGui::EndTabBar();
		}

		ImGui::EndGroup();
		ImGui::End();
	}
}