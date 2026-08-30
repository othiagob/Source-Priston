#include "SodWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "imGui/imgui.h"
#include <curl/curl.h>

RankingSodPckg sRanking;

std::vector<RankingSod>vRankingSodSolo;
std::vector<RankingSod>vRankingSodClan;

LPDIRECT3DTEXTURE9 SodRankingImages[6];
LPDIRECT3DTEXTURE9 SodRankingClasses[10];
LPDIRECT3DTEXTURE9 SodClanLogos[10];

extern bool LoadTextureFromFile4(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);

int iwd = 0;
int ihd = 0;


size_t callbackfunction(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	FILE* stream = (FILE*)userdata;
	if (!stream)
	{
		printf("!!! No stream\n");
		return 0;
	}

	size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
	return written;
}

// https://stackoverflow.com/questions/10112959/download-an-image-from-server-curl-however-taking-suggestions-c
bool DownloadClanImage(char* url, std::string il)
{
	char pathLogo[256] = { 0 };
	sprintf_s(pathLogo, sizeof(pathLogo), ".\\game\\images\\Sod\\CacheImages\\%s.bmp", il.c_str());

	FILE* fp = fopen(pathLogo, "wb");
	if (!fp)
	{
		printf("!!! Failed to create file on the disk\n");
		return false;
	}

	CURL* curlCtx = curl_easy_init();
	curl_easy_setopt(curlCtx, CURLOPT_URL, url);
	curl_easy_setopt(curlCtx, CURLOPT_WRITEDATA, fp);
	curl_easy_setopt(curlCtx, CURLOPT_WRITEFUNCTION, callbackfunction);
	curl_easy_setopt(curlCtx, CURLOPT_FOLLOWLOCATION, 1);

	CURLcode rc = curl_easy_perform(curlCtx);
	if (rc)
	{
		printf("!!! Failed to download: %s\n", url);
		return false;
	}

	long res_code = 0;
	curl_easy_getinfo(curlCtx, CURLINFO_RESPONSE_CODE, &res_code);
	if (!((res_code == 200 || res_code == 201) && rc != CURLE_ABORTED_BY_CALLBACK))
	{
		printf("!!! Response code: %d\n", res_code);
		return false;
	}

	curl_easy_cleanup(curlCtx);

	fclose(fp);

	return true;
}

void initSodWindow()
{
	ZeroMemory(SodRankingImages, sizeof(SodRankingImages));
	ZeroMemory(SodRankingClasses, sizeof(SodRankingClasses));
	ZeroMemory(SodClanLogos, sizeof(SodClanLogos));

	char topClanLogo[128] = { 0 };
	char Path[128] = { 0 };

	sprintf_s(Path, sizeof(Path), "game\\images\\Sod\\bellatra.png");
	bool ret = LoadTextureFromFile4(Path, &SodRankingImages[0], &iwd, &ihd);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\Sod\\top.png");
	ret = LoadTextureFromFile4(Path, &SodRankingImages[1], &iwd, &ihd);
	IM_ASSERT(ret);

	for (int x = 0; x <= 10; x++)
	{
		sprintf_s(Path, sizeof(Path), "game\\images\\Sod\\Classes\\%d.png", x);
		bool ret = LoadTextureFromFile4(Path, &SodRankingClasses[x], &iwd, &ihd);
		IM_ASSERT(ret);
	}

	for (int i = 0; i < vRankingSodClan.size(); i++)
	{
		//Bandeira de clan sod - DRACKO
		sprintf_s(topClanLogo, sizeof(topClanLogo), "http://www.dnsprotect.com.br/PristonTale/Youtube//ClanContent//%d.bmp", vRankingSodClan[i].clanCode);

		DownloadClanImage(topClanLogo, to_string(i));

		sprintf_s(Path, sizeof(Path), ".\\game\\images\\Sod\\CacheImages\\%d.bmp", i);
		SodClanLogos[i] = LoadDibSurfaceOffscreen(Path);
	}


	SodWindow::GetInstance()->init = false;
}

bool sortByPoints(const RankingSod& a, const RankingSod& b)
{
	return a.sodTotalScore > b.sodTotalScore;
}

INT SodWindow::RetrieveSodRanking(RankingSodPckg* AllRanking)
{
	ZeroMemory(&sRanking, sizeof(RankingSodPckg));
	CopyMemory(&sRanking, AllRanking, sizeof(RankingSodPckg));

	vRankingSodSolo.clear();
	vRankingSodClan.clear();

	int i = 0;

	while (sRanking.AllRanking[i].sodTotalScore)
	{
		if (sRanking.AllRanking[i].clanCode)
			vRankingSodClan.push_back(sRanking.AllRanking[i]);
		else
			vRankingSodSolo.push_back(sRanking.AllRanking[i]);

		i++;
	}

	// Ordena os vetores pela pontuação solo/clan
	std::sort(vRankingSodClan.begin(), vRankingSodClan.end(), sortByPoints);
	std::sort(vRankingSodSolo.begin(), vRankingSodSolo.end(), sortByPoints);

	SodWindow::GetInstance()->init = true;
	showDetails = false;
	openFlag = true;

	return TRUE;
}

INT SodWindow::RetrieveSodMoney()
{
	return TRUE;
}

void SodWindow::OpenNpc(bool* p_open)
{
	if (init)
		initSodWindow();

	int posInicial = 60;

	if (!showDetails)
	{
		ImGui::SetNextWindowSize(ImVec2(450, 380), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 200, (smScreenHeight / 2) - 220));
		ImGui::SetNextWindowBgAlpha(0.90f);

		ImGui::StyleColorArmageddon();

		if (ImGui::Begin("Ranking de Bellatra", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::SetCursorPosX(188);
			ImGui::SetCursorPosY(posInicial);
			ImGui::Image(SodRankingImages[0], ImVec2(80, 86));

			ImGui::SetCursorPosX(65);
			ImGui::SetCursorPosY(posInicial);
			ImGui::Image(SodRankingImages[1], ImVec2(50, 50));

			ImGui::SetCursorPosX(70);
			ImGui::SetCursorPosY(posInicial + 80);

			if (vRankingSodClan.size() > 0)
			ImGui::Image(SodClanLogos[0], ImVec2(32, 32));

			ImGui::SetCursorPosX(40);
			ImGui::SetCursorPosY(posInicial + 115);
			
			if (vRankingSodClan.size() > 0)
			ImGui::Text("Clan: %s", vRankingSodClan[0].szName);
			else
			ImGui::Text(u8"Clan: Sem líder");

			ImGui::SetCursorPosX(40);
			ImGui::SetCursorPosY(posInicial + 130);

			if (vRankingSodClan.size() > 0)
			ImGui::Text(u8"Pontuação: %d", vRankingSodClan[0].sodTotalScore);
			else
			ImGui::Text(u8"Pontuação: 0");

			ImGui::SetCursorPosX(340);
			ImGui::SetCursorPosY(posInicial);
			ImGui::Image(SodRankingImages[1], ImVec2(50, 50));

			ImGui::SetCursorPosX(345);
			ImGui::SetCursorPosY(posInicial + 80);

			if (vRankingSodSolo.size() > 0)
			ImGui::Image(SodRankingClasses[vRankingSodSolo[0].jobCode], ImVec2(32, 32));

			ImGui::SetCursorPosX(310);
			ImGui::SetCursorPosY(posInicial + 115);

			if (vRankingSodSolo.size() > 0)
			ImGui::Text("Nome: %s", vRankingSodSolo[0].szName);
			else
			ImGui::Text(u8"Sem líder");

			ImGui::SetCursorPosX(310);
			ImGui::SetCursorPosY(posInicial + 130);

			if (vRankingSodSolo.size() > 0)
			ImGui::Text(u8"Pontuação: %d", vRankingSodSolo[0].sodTotalScore);
			else
			ImGui::Text(u8"Pontuação: 0");

			ImGui::SetCursorPosY(posInicial + 170);
			ImGui::Separator();

			ImGui::SetCursorPosX(135);

			char impostoArrecadado[80] = { 0 };
			NumLineComa64(sRanking.impostoArrecadado, impostoArrecadado);

			ImGui::Text("Imposto arrecadado: %s", impostoArrecadado);
			ImGui::Separator();

			ImGui::SetCursorPosY(posInicial + 195);

			if (ImGui::Button("Visualizar ranking detalhado", ImVec2(450, 30)))
			{
				showDetails = true;
			}

			ImGui::Separator();

			if (strcmp(sRanking.clanZang, lpCurPlayer->smCharInfo.szName) == 0)
			{
				ImGui::SetCursorPosY(posInicial + 240);
				ImGui::SetCursorPosX(170);
				ImGui::Text(u8"Você é o líder do Clan!");

				if (ImGui::Button("Resgatar imposto arrecadado", ImVec2(450, 30)))
				{
					if (strcmp(sRanking.clanZang, lpCurPlayer->smCharInfo.szName) == 0)
					{
						smTRANS_COMMAND resgatarImposto;
						resgatarImposto.code = RETRIEVE_SOD_MONEY;
						resgatarImposto.size = sizeof(smTRANS_COMMAND);
						resgatarImposto.WParam = 0;
						resgatarImposto.LParam = 0;
						resgatarImposto.SParam = 0;

						if (smWsockServer)
							smWsockServer->Send((char*)&resgatarImposto, resgatarImposto.size, TRUE);

						openFlag = false;
					}
				}
			}
		}

		ImGui::End();
	}

	if (showDetails)
	{
		ImGui::SetNextWindowSize(ImVec2(450, 380), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 200, (smScreenHeight / 2) - 220));

		ImGui::StyleColorArmageddon();

		if (ImGui::Begin("Ranking Detalhado", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("rClans", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Ranking De Clans"))
				{
					static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;

					if (ImGui::BeginTable("Ranking de Clans", 6, flags))
					{
						ImGui::TableSetupColumn("Clan", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn(u8"Participantes", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn(u8"Pontuação", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Monstros", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn(u8"Data da Pontuação", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableHeadersRow();

						for (int row = 0; row < vRankingSodClan.size(); row++)
						{
							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::Text("%s", vRankingSodClan[row].szName);
							ImGui::TableSetColumnIndex(1);
							ImGui::Text("%d", vRankingSodClan[row].sodTotalUser);
							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%d", vRankingSodClan[row].sodTotalScore);
							ImGui::TableSetColumnIndex(3);
							ImGui::Text("%d", vRankingSodClan[row].sodKillCount);
							ImGui::TableSetColumnIndex(4);
							ImGui::Text("%02d/%02d/%02d", vRankingSodClan[row].Date.wDay, vRankingSodClan[row].Date.wMonth, vRankingSodClan[row].Date.wYear);
							ImGui::TableSetColumnIndex(5);
							ImGui::Image(SodClanLogos[row], ImVec2(16, 16));
						}

						ImGui::EndTable();
					}

					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Ranking Individual"))
				{
					static ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders;

					if (ImGui::BeginTable("Ranking Individual", 6, flags))
					{
						ImGui::TableSetupColumn("Nome", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn(u8"Nível", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn(u8"Pontuação", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Monstros", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn(u8"Data da Pontuação", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableSetupColumn("Classe", ImGuiTableColumnFlags_WidthFixed);
						ImGui::TableHeadersRow();

						for (int row = 0; row < vRankingSodSolo.size(); row++)
						{
							ImGui::TableNextRow();

							ImGui::TableSetColumnIndex(0);
							ImGui::Text("%s", vRankingSodSolo[row].szName);
							ImGui::TableSetColumnIndex(1);
							ImGui::Text("%d", vRankingSodSolo[row].level);
							ImGui::TableSetColumnIndex(2);
							ImGui::Text("%d", vRankingSodSolo[row].sodTotalScore);
							ImGui::TableSetColumnIndex(3);
							ImGui::Text("%d", vRankingSodSolo[row].sodKillCount);
							ImGui::TableSetColumnIndex(4);
							ImGui::Text("%02d/%02d/%02d", vRankingSodSolo[row].Date.wDay, vRankingSodSolo[row].Date.wMonth, vRankingSodSolo[row].Date.wYear);
							ImGui::TableSetColumnIndex(5);
							ImGui::Image(SodRankingClasses[vRankingSodSolo[row].jobCode], ImVec2(32, 32));
						}

						ImGui::EndTable();
					}

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

			ImGui::End();
		}
	}
}