#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\FontImage.h"
#include "../HelpTime.h"
#include "..\ConfirmationBox.h"

RankingPckg getRanking;
Ranking selectRanking;
HelpTime* helpTime2;
bool render = FALSE;
bool showScore = TRUE;
int arenaTimer = 10;
int timer4 = 0;
int flag4 = 1;
extern int WarpFieldNearPos(int FieldNum, int x, int z, int* mx, int* mz);
extern int WarpField2(int Num);

int equipeVencedora = 0;

void EventoArena::getRankingFromServer(RankingPckg* Ranking) {
	CopyMemory(&getRanking, Ranking, sizeof(RankingPckg));
}

void EventoArena::getTeamPlayersFromServer(playersFromTeamPckg* getPlayers) {
	CopyMemory(&getPlayersFromTeam, getPlayers, sizeof(playersFromTeamPckg));
}

void EventoArena::showTime(char* time) {

	if (atoi(time) <= 0) {
		timeArena = FALSE;
		arenaTimer = 11;
		stageArena = 2;
		smTRANS_COMMAND	smTransCommand;
		smTransCommand.code = 0x48476972;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.LParam = 0;
		smTransCommand.EParam = 0;

		if (smWsockDataServer)
			smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	}

	if (atoi(time) > 0) {

		DrawFontImage(time, WinSizeX / 2, (WinSizeY / 2) - 200, D3DCOLOR_RGBA(255, 0, 0, 255), 5.0f);
	}

}

void playerArena()
{
	if (EventoArena::GetInstance()->stageArena == 1) 
	{
		// Desativa modo PK na arena
		WarMode.ModoPVP(true);

		EventoArena::GetInstance()->openFlag = FALSE;

		smTRANS_COMMAND smPacket;
		smPacket.code = smTRANSCODE_ADD_PLAYER_ARENA;
		smPacket.WParam = 0;
		smPacket.EParam = 0;
		smPacket.size = sizeof(smPacket);

		if (smWsockDataServer)
		smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);

		//EventoArena::GetInstance()->flagClan = TRUE;
	}
	else if (EventoArena::GetInstance()->stageArena == 2 && lpCurPlayer->nEquipeArena > 0) 
	{
		// Desativa modo PK na arena
		WarMode.ModoPVP(true);

		EventoArena::GetInstance()->openFlag = FALSE;
		WarpField2(FIELD_ARENA);
	}
	else 
	{
		cMessageBox.ShowMessageEvent("A entrada está fechada!");
	}

}
int enterArena(std::string title)
{
	int i = 1;
	_ConfirmationBox.SetTitle(title);
	_ConfirmationBox.SetText("Teletransportar", "Para a Arena?");
	_ConfirmationBox.SetCallback([](bool confirmed)
		{
			if (confirmed)
			{
				if (lpCurPlayer->smCharInfo.Level < 80)
				{
					CHATGAMEHANDLE->AddChatBoxText("Você precisa ser nível 80 ou superior para participar da arena!", EChatColor::CHATCOLOR_Global);
					return FALSE;
				}

				playerArena();
			}
			else
				return FALSE;
		});

	_ConfirmationBox.Start();

	return FALSE;
}

bool LoadTextureFromFile(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFile(GRAPHICDEVICE, filename, &texture);
	if (hr != S_OK)
		return false;

	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;
}

void EventoArena::arenaWindow(bool* p_open, int hora, int minuto, int segundo, int dia)
{
	ImGuiWindowFlags window_flags = 0;

	ImGui::StyleColorArmageddon();

	window_flags |= ImGuiWindowFlags_NoSavedSettings;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowSize(ImVec2(400, 350), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();

	if (ImGui::Begin("Arena de Batalha", p_open, window_flags))
	{		
		if (flag4)
			timer4 = GetTickCount();

		if (GetTickCount() - timer4 <= 1000)
		{
			flag4 = 0;
		}
		else
		{
			smTRANS_COMMAND smPacket;
			smPacket.code = 0x50600023;
			smPacket.WParam = 0;
			smPacket.EParam = 0;
			smPacket.size = sizeof(smPacket);

			if (smWsockDataServer)
				smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);

			timer4 = 0;
			flag4 = 1;
		}

		//int my_image_width = 0;
		//int my_image_height = 0;
		//PDIRECT3DTEXTURE9 my_texture = NULL;
		//bool ret = LoadTextureFromFile("game\\images\\arena.png", &my_texture, &my_image_width, &my_image_height);
		//IM_ASSERT(ret);

		ImGui::Text(u8"Bem-vindo! A arena acontece todos os dias ás 21:00.");
		ImGui::Text(u8"\nA arena é dividida em duas equipes de forma aleatória");
		ImGui::Text(u8"O evento possui duas etapas, sendo elas:");
		ImGui::Text(u8"\nPrimeira etapa: 10 Minutos de PVP entre as duas equipes");
		ImGui::Text(u8"Segunda etapa: A equipe com mais pontos terá a chance \nde enfrentar Dark Stalker.\n\nO tempo máximo para matar o Boss é de 5 minutos");
		ImGui::Text(u8"Se Dark Stalker for derrotado a tempo, ele pode dropar\nitens de grande valor!");

		ImGui::Text(u8"\nBoa sorte, guerreiro!");

		//ImGui::SameLine();
		//ImGui::SetCursorPos(ImVec2(320, 250));
		//ImGui::Image((void*)my_texture, ImVec2(my_image_width, my_image_height));

		int horas = 0, minutos = 0, segundos = 0;

		// Começar o counter
		if (stageArena == 0)
		{
			time_t now = time(0);
			struct tm* Data = localtime(&now);

			int hour = hora;
			int day = dia;
			if (hour >= 21) day++;

			long long int Seconds = helpTime2->getData(hora, minuto, segundo, dia, Data->tm_mon + 1, Data->tm_year + 1900,
				21, 0, 0, day, Data->tm_mon + 1, Data->tm_year + 1900);

			horas = (Seconds / 3600);
			minutos = (Seconds - (3600 * horas)) / 60;
			segundos = (Seconds - (3600 * horas) - (minutos * 60));

			char msg[64] = { 0 };
			sprintf_s(msg, sizeof(msg), "%02d:%02d:%02d", horas, minutos, segundos);

			ImGui::SetCursorPos(ImVec2(150, 280));
			if (ImGui::Button(msg, ImVec2(120, 30))) { CHATGAMEHANDLE->AddChatBoxText("> A arena ainda não abriu!", EChatColor::CHATCOLOR_Error); };
		}
		else if(stageArena == 1)
		{
			ImGui::Text(u8"\nA arena está aberta!\n");

			ImGui::SetCursorPos(ImVec2(150, 280));
			if (ImGui::Button("Entrar na Arena", ImVec2(120, 30))) { enterArena("Arena De Batalha"); };
		}
		else if (stageArena == 2)
		{
			ImGui::Text(u8"\nSe você se inscreveu na arena, você pode retornar.");

			ImGui::SetCursorPos(ImVec2(150, 280));
			if (ImGui::Button("Entrar na Arena", ImVec2(120, 30))) { enterArena("Arena De Batalha"); };
		}

		ImGui::End();
	}
}

void EventoArena::renderRanking() {

	char temp[256];

	if (sinGetKeyClick('B') || sinGetKeyClick('b'))
	{
		if (showScore)
			showScore = FALSE;
		else
			showScore = TRUE;
	}

	HDC hdc = NULL;

	if (stageArena > 0) {
		if (flagClan) {
			lpCurPlayer->dwClanCode = 0;
			lpCurPlayer->smCharInfo.ClassClan = 0;
			lpCurPlayer->dwClanManageBit = 0;
			flagClan = FALSE;
		}
	}

	if (stageArena == 10)
	{
		if (lpCurPlayer->nEquipeArena != equipeVencedora)
		{
			WarpField2(3);
		}
	}

	if (stageArena == 2) 
	{
		// Player não se inscreveu na arena e não é gm? volta para ricarten
		if (lpCurPlayer->nEquipeArena == 0 && lpCurPlayer->smCharInfo.Life[1] > 0 && !smConfig.DebugMode) {
			WarpField2(3);
		}

		if (lpCurPlayer->nEquipeArena > 0) {

			int coordx = lpCurPlayer->pX >> (FLOATNS);
			int coordy = lpCurPlayer->pY >> (FLOATNS);
			int coordz = lpCurPlayer->pZ >> (FLOATNS);

			if (coordz < 21644 || coordy > 1390) {
				int x, z;
				WarpFieldNearPos(49, -23893 * fONE, 22207 * fONE, &x, &z);
				lpCurPlayer->SetPosi(-23893 * fONE, 0, 22207 * fONE, 0, 0, 0);
				TraceCameraPosi.x = lpCurPlayer->pX;
				TraceCameraPosi.y = lpCurPlayer->pY;
				TraceCameraPosi.z = lpCurPlayer->pZ;
				TraceTargetPosi.x = lpCurPlayer->pX;
				TraceTargetPosi.y = lpCurPlayer->pY;
				TraceTargetPosi.z = lpCurPlayer->pZ;
			}
		}

		if (showScore)
		{
			DrawFontImage("Placar de Lideres: ", (WinSizeX)-260, 15, D3DCOLOR_RGBA(0, 255, 0, 255), 0.9f);

			DrawFontImage("1 lugar:", (WinSizeX)-320, 50, D3DCOLOR_RGBA(255, 0, 0, 255), 0.7f);
			DrawFontImage("2 lugar:", (WinSizeX)-320, 80, D3DCOLOR_RGBA(255, 0, 0, 255), 0.7f);
			//DrawFontImage("3 lugar:", (WinSizeX)-320, 110, D3DCOLOR_RGBA(255, 0, 0, 255), 0.7f);

			wsprintf(temp, "Equipe %d", getRanking.Ranking[0].equipeNum);
			DrawFontImage(temp, (WinSizeX)-245, 51, D3DCOLOR_RGBA(0, 255, 0, 255), 0.5f);

			wsprintf(temp, "Equipe %d", getRanking.Ranking[1].equipeNum);
			DrawFontImage(temp, (WinSizeX)-245, 81, D3DCOLOR_RGBA(0, 255, 0, 255), 0.5f);

			//wsprintf(temp, "Equipe %d", getRanking.Ranking[2].equipeNum);
			//DrawFontImage(temp, (WinSizeX)-245, 111, D3DCOLOR_RGBA(0, 255, 0, 255), 0.5f);

			DrawFontImage("Pontos:", (WinSizeX)-110, 50, D3DCOLOR_RGBA(255, 0, 0, 255), 0.7f);
			DrawFontImage("Pontos:", (WinSizeX)-110, 80, D3DCOLOR_RGBA(255, 0, 0, 255), 0.7f);
			//DrawFontImage("Pontos:", (WinSizeX)-110, 110, D3DCOLOR_RGBA(255, 0, 0, 255), 0.7f);

			wsprintf(temp, "%d", getRanking.Ranking[0].equipeScore);
			DrawFontImage(temp, (WinSizeX)-40, 52, D3DCOLOR_RGBA(0, 255, 0, 255), 0.7f);
			wsprintf(temp, "%d", getRanking.Ranking[1].equipeScore);
			DrawFontImage(temp, (WinSizeX)-40, 82, D3DCOLOR_RGBA(0, 255, 0, 255), 0.7f);
			//wsprintf(temp, "%d", getRanking.Ranking[2].equipeScore);
			//DrawFontImage(temp, (WinSizeX)-40, 112, D3DCOLOR_RGBA(0, 255, 0, 255), 0.7f);

			wsprintf(temp, "Sua equipe:%d", lpCurPlayer->nEquipeArena);
			DrawFontImage(temp, (WinSizeX)-100, 120, D3DCOLOR_RGBA(0, 255, 0, 255), 0.5f);

		}
	}

}


