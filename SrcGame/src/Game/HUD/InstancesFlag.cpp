#include "..\\sinbaram\\sinLinkHeader.h"
#include "..\\imGui\\imgui_internal.h"
#include "RestaureWindow.h"
#include <atlconv.h>
#include "RankingWindow.h"
#include "MixWindow.h"
#include "..\\Shop\NewShop.h"
#include "..\\Shop\NewShopTime.h"
#include "..\\HUD\\SodWindow.h"
#include "..\\HUD\\Roleta.h"

#define TIME_GLOBAL 10

// Flag do jogo <Tipo de mensagem>
extern int ChatKindIndex;

string strMessage;
string msgWhisperParty;
string lastWhisper;
string lastReceiveWhisper;

// String do filtro de mensagens
int filtro = 0;
// Flag de mover a janela
bool isMovingEnabled = 1;

// Flag de cada janela
bool flagChat = false;
bool flagAlert = false;
bool flagFiltro = true;
bool flagNpcs = false;
bool flagResize = false;
bool flagCommand = false;
bool flagWhisperParty = false;
bool flagLogin = true;
bool canClickButtons = true;

// Histórico de mensagens
bool msgHistoryUp = false;
bool msgHistoryDown = false;

// Flag de cada botão
bool flagOnMouseButton1 = false;
bool flagOnMouseButton2 = false;
bool flagOnMouseButton3 = false;
bool flagOnMouseButton4 = false;
bool flagOnMouseButton5 = false;

bool flagFromMouse = true;

// Flag de digitação no chat
bool FOCUS_CHAT = false;

// Flag de parar o personagem enquanto reorganiza a hud
//extern bool isnewclick;

// Define a cor do texto no chat
extern int colorText;

// Portable helpers
static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
static char* Strdup(const char* str) { size_t len = strlen(str) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)str, len); }
static void  Strtrim(char* str) { char* str_end = str + strlen(str); while (str_end > str && str_end[-1] == ' ') str_end--; *str_end = 0; }
extern int	SetChatingLine(char* szMessage);

extern int flagDropItens;

int getClickStatus()
{
	if (canClickButtons)
		return TRUE;
	else
		return FALSE;
}

long getUTF8size(const wchar_t* string) {
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

char* WChar_to_UTF8(const wchar_t* string) {
	long fSize = getUTF8size(string);
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

bool                  ScrollToBottom;

struct NewChatWindow
{
	char                  InputBuf[64];
	ImVector<char*>       MessageContext;
	ImVector<char*>       MessageSender;
	ImVector<char*>       ItemsAlert;
	ImVector<int>         dwIP;
	ImVector<int>         dwIPAlert;
	ImVector<const char*> Commands;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImGuiTextFilter       Filter;
	bool                  AutoScroll;
	bool                  ScrollToBottomAlert;
	bool                  ScrollFlag;

	NewChatWindow()
	{
		ClearLog();
		memset(InputBuf, 0, sizeof(InputBuf));
		HistoryPos = -1;
		AutoScroll = true;
		ScrollToBottom = false;
		ScrollToBottomAlert = false;
	}
	~NewChatWindow()
	{
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	void    ClearLog()
	{
		for (int i = 0; i < MessageContext.Size; i++)
			free(MessageContext[i]);
		MessageContext.clear();
		MessageSender.clear();
		dwIP.clear();
	}

	void setGlobalBuff()
	{
		sSKILL haGlobalChat;

		for (int j = 0; j < SIN_MAX_SKILL; j++)
		{
			if (sSkill[j].CODE == CHATGLOBAL)
			{
				memcpy(&haGlobalChat, &sSkill[j], sizeof(sSKILL));
				haGlobalChat.UseTime = TIME_GLOBAL;
				sinContinueSkillSet(&haGlobalChat);
				break;
			}
		}
	}

	void    AddLog(const char* Context, const char* Sender, int dwIp, ...) IM_FMTARGS(2)
	{
		char buf[1024];
		va_list args;

		va_start(args, Context);
		vsnprintf(buf, IM_ARRAYSIZE(buf), Context, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		MessageContext.push_back(Strdup(buf));

		va_start(args, Sender);
		vsnprintf(buf, IM_ARRAYSIZE(buf), Sender, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		MessageSender.push_back(Strdup(buf));

		dwIP.push_back(dwIp);
	}

	void    AddLogAlert(const char* fmt, int dwIp, ...) IM_FMTARGS(2)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		ItemsAlert.push_back(Strdup(buf));

		dwIPAlert.push_back(dwIp);

		ScrollToBottomAlert = true;
	}

	void setTempGlobalBuff() {
		sSKILL haGlobalChat;

		for (int j = 0; j < SIN_MAX_SKILL; j++)
		{
			if (sSkill[j].CODE == CHATGLOBAL)
			{
				memcpy(&haGlobalChat, &sSkill[j], sizeof(sSKILL));
				haGlobalChat.UseTime = 10;
				sinContinueSkillSet(&haGlobalChat);
				break;
			}
		}
	}

	int lasxPOS = 1;

	const char* FormatString(const char* pszFormat, ...)
	{
		va_list vl;

		static char szString[4096];

		va_start(vl, pszFormat);
		_vsnprintf_s(szString, _countof(szString), _TRUNCATE, pszFormat, vl);
		va_end(vl);

		return szString;
	}

	void Draw(const char* title, bool* p_open)
	{
		ImGuiWindowFlags window_flags = 0;

		ImGui::StyleColorsRanking();

		//ImGui::StyleColorsClassic();
		//ImGui::StyleColorArmageddon();

		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		window_flags |= ImGuiWindowFlags_NoSavedSettings;

		// Se a checkbox de não mover estiver selecionada, trava o chat na posição
		if (isMovingEnabled) {
			window_flags |= ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoResize;
		}

		// Sobe o chat se alguma interface for aberta
		if ((cInvenTory.OpenFlag || CSKILL->OpenFlag || cCharStatus.OpenFlag) && (lasxPOS <= 0)) {
			ImGui::SetNextWindowPos(ImVec2(0, (smScreenHeight - 450)), 0);
			flagNpcs = true;
			ScrollFlag = true;
		}
		else if (flagNpcs && !cInvenTory.OpenFlag && !CSKILL->OpenFlag && !cCharStatus.OpenFlag) {
			if (lasxPOS == 0 || lasxPOS == -1)
				ImGui::SetNextWindowPos(ImVec2(0, (smScreenHeight - 260)), 0);

			flagNpcs = false;
		}

		// Coloca o chat na posição inicial        
		if (flagResize) {
			ImGui::SetNextWindowSize(ImVec2(303, 260), 0);
			ImGui::SetNextWindowPos(ImVec2(0, smScreenHeight - 260), 0);
			flagResize = false;
		}

		// Posição inicial do chat
		if (flagLogin) {
			ImGui::SetNextWindowPos(ImVec2(0, (smScreenHeight - 260)), ImGuiCond_Once);
			ImGui::SetNextWindowSize(ImVec2(303, 260), ImGuiCond_Once); // Tamanho do chat
			flagLogin = false;
		}

		ImGui::SetNextWindowBgAlpha(0.70f); // Transparência do chat

		if (!ImGui::Begin(title, p_open, window_flags))
		{
			ImGui::End();
			return;
		}

		lasxPOS = ImGui::GetWindowPos().x;

		if (flagAlert && !flagNpcs) {
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y - 60), 0);
			ImGui::SetNextWindowSize(ImVec2(ImGui::GetWindowWidth(), 60), 0);
			ImGui::SetNextWindowBgAlpha(0.80f);

			ImGui::Begin("Alerta", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoTitleBar);

			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));

			for (int i = 0; i < ItemsAlert.Size; i++)
			{
				const char* itemAlert = ItemsAlert[i];
				const int messageColor = dwIPAlert[i];

				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));

				ImGui::TextUnformatted(itemAlert);
				ImGui::PopStyleColor();
			}

			ImGui::PopStyleVar();

			if (ScrollToBottomAlert || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
				ImGui::SetScrollHereY(1.0f);

			ScrollToBottomAlert = false;

			ImGui::End();
		}


		///////////////////////////////////////////////////
		/// Ajusta a cor dependendo do chat selecionado ///
		//////////////////////////////////////////////////


		ImGui::PushID(1);
		if (ChatKindIndex == 0) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
			if (ImGui::SmallButton("Todos")) { ChatKindIndex = 0; DispChatMode = 4; }  ImGui::SameLine();
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else {
			if (ImGui::SmallButton("Todos")) {
				if (canClickButtons) {

					ChatKindIndex = 0;
					DispChatMode = 4;
				}

			}  ImGui::SameLine();
			if (!canClickButtons)  ImGui::CaptureMouseFromApp(false);
		}

		if (ImGui::IsItemHovered()) {
			//isnewclick = true;
			flagOnMouseButton1 = true;
		}
		else if (!flagOnMouseButton2 && !flagOnMouseButton3 && !flagOnMouseButton4 && !flagOnMouseButton5) {
			//isnewclick = false;
		}
		else {
			flagOnMouseButton1 = false;
		}

		ImGui::PushID(2);
		if (ChatKindIndex == 1) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
			if (ImGui::SmallButton("Clan")) { ChatKindIndex = 1; DispChatMode = 1; }  ImGui::SameLine();
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else {
			if (ImGui::SmallButton("Clan")) {

				if (canClickButtons) {
					ChatKindIndex = 1;
					DispChatMode = 1;
				}

			}  ImGui::SameLine();
			if (!canClickButtons)  ImGui::CaptureMouseFromApp(false);
		}

		if (ImGui::IsItemHovered()) {
			//isnewclick = true;
			flagOnMouseButton2 = true;
		}
		else if (!flagOnMouseButton1 && !flagOnMouseButton3 && !flagOnMouseButton4 && !flagOnMouseButton5) {
			//isnewclick = false;
		}
		else {
			flagOnMouseButton2 = false;
		}

		ImGui::PushID(3);
		if (ChatKindIndex == 2) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
			if (ImGui::SmallButton("Grupo")) { ChatKindIndex = 2; DispChatMode = 2; }  ImGui::SameLine();
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else {
			if (ImGui::SmallButton("Grupo")) {

				if (canClickButtons) {
					ChatKindIndex = 2;
					DispChatMode = 2;
				}

			}  ImGui::SameLine();
			if (!canClickButtons)  ImGui::CaptureMouseFromApp(false);
		}

		if (ImGui::IsItemHovered()) {
			//isnewclick = true;
			flagOnMouseButton3 = true;
		}
		else if (!flagOnMouseButton1 && !flagOnMouseButton2 && !flagOnMouseButton4 && !flagOnMouseButton5) {
			//isnewclick = false;
		}
		else {
			flagOnMouseButton3 = false;
		}

		ImGui::PushID(4);
		if (ChatKindIndex == 3) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
			if (ImGui::SmallButton("Troca")) { ChatKindIndex = 3; DispChatMode = 3; }  ImGui::SameLine();
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else {
			if (ImGui::SmallButton("Troca")) {

				if (canClickButtons) {
					ChatKindIndex = 3;
					DispChatMode = 3;
				}

			}
			ImGui::SameLine();
		}

		if (ImGui::IsItemHovered()) {
			//isnewclick = true;
			flagOnMouseButton4 = true;
		}
		else if (!flagOnMouseButton1 && !flagOnMouseButton2 && !flagOnMouseButton3 && !flagOnMouseButton5) {
			//isnewclick = false;
		}
		else {
			flagOnMouseButton4 = false;
		}

		ImGui::PushID(5);
		if (ChatKindIndex == 4) {
			ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(7.0f, 0.6f, 0.6f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(7.0f, 0.7f, 0.7f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(7.0f, 0.8f, 0.8f));
			if (ImGui::SmallButton("Privado")) { ChatKindIndex = 4;  DispChatMode = 0; }
			ImGui::PopStyleColor(3);
			ImGui::PopID();
		}
		else {
			if (ImGui::SmallButton("Privado")) {

				if (canClickButtons) {
					ChatKindIndex = 4;
					DispChatMode = 0;
				}
			}
		}

		if (ImGui::IsItemHovered()) {
			//isnewclick = true;
			flagOnMouseButton5 = true;
		}
		else if (!flagOnMouseButton1 && !flagOnMouseButton2 && !flagOnMouseButton3 && !flagOnMouseButton4) {
			//isnewclick = false;
		}
		else {
			flagOnMouseButton5 = false;
		}

		ImGui::Separator();

		/*
		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
		ImGui::Checkbox("Auto-scroll", &AutoScroll);
		ImGui::EndPopup();
		}*/

		//ImGui::SameLine();
		if (Filter.Draw("Filtro", 100));

		if (ImGui::IsItemActive() && !flagFiltro) {
			sinMessageBoxShowFlag = true;
			flagFiltro = true;

		}

		if (!ImGui::IsItemActive() && flagFiltro) {
			sinMessageBoxShowFlag = false;
			flagFiltro = false;
		}

		if (flagFiltro && sinMessageBoxShowFlag && flagChat) {
			flagFiltro = false;
			sinMessageBoxShowFlag = false;
		}

		ImGui::SameLine();
		if (ImGui::Checkbox("Fixo", &isMovingEnabled)); ImGui::SameLine();
		if (ImGui::Checkbox("Alerta", &flagAlert));
		ImGui::Separator();

		const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();

		ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve));	

		if (!isMovingEnabled)
		{
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::Selectable(u8"Posição Original")) flagResize = true;
				if (ImGui::Selectable("Limpar chat")) ClearLog();
				ImGui::EndPopup();
			}
		}

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
		for (int i = 0; i < MessageContext.Size; i++)
		{
			const char* item = MessageContext[i];
			const char* sender = MessageSender[i];
			const int messageColor = dwIP[i];

			if (!Filter.PassFilter(item))
				continue;

			bool pop_color = false;

			switch (messageColor) {
			case 1:
				ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true;
				break;
			case 3:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.5f, 1.0f, 1.0f)); pop_color = true;
				break;
			case 6:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1.0f, 1.0f, 1.0f)); pop_color = true;
				break;
			case 256:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.6f, 0.8f)); pop_color = true;
				break;
			case 9:
				ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true;
				break;
				// Clan
			case 5:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.96f, 0.90f, 0.26f, 1.0f)); pop_color = true;
				break;
				// Quiz
			case 100:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 1.0f, 0.8f, 1.0f)); pop_color = true;
				break;
			default:
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true;
				break;

			}

			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + 280);

			// Filtros do chat <Melhorar isso dps>
			if (ChatKindIndex > 0) {

				if (ChatKindIndex == 3) {
					if (messageColor == 6 || messageColor == 0 || messageColor == 100) {
						if ((Stricmp(sender, "System") != 0)) {
							ImGui::TextUnformatted(sender);
							ImGui::SameLine(0, 0);
						}
						ImGui::TextUnformatted(item);
					}
				}

				if (ChatKindIndex == 4) {
					if (messageColor == 1) {
						// ImGui::TextUnformatted(sender);
						// ImGui::SameLine(0, 0);
						ImGui::TextUnformatted(item);
					}
				}

				if (ChatKindIndex == 1) {
					if (messageColor == 5) {
						//ImGui::TextUnformatted(sender);
						//ImGui::SameLine(0, 0);
						ImGui::TextUnformatted(item);
					}
				}

				if (ChatKindIndex == 2) {
					if (messageColor == 4) {
						ImGui::TextUnformatted(sender);
						ImGui::SameLine(0, 0);
						ImGui::TextUnformatted(item);
					}
				}

			}
			else {
				if (Stricmp(sender, "System") != 0 && (messageColor == 256 || messageColor == 128)) {
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.00f, 0.79f, 0.18f, 0.78f));
					ImGui::TextUnformatted(sender);
					ImGui::PopStyleColor(1);

					ImGui::SameLine(0, 0);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.94f, 0.94f, 0.94f, 1.00));
					ImGui::TextUnformatted(item);
					ImGui::PopStyleColor(1);
				}
				else {
					if (Stricmp(sender, "System") != 0) {
						ImGui::TextUnformatted(sender);
						ImGui::SameLine(0, 0);
					}
					ImGui::TextUnformatted(item);
					ImGui::PopTextWrapPos();
				}

			}

			if (pop_color)
				ImGui::PopStyleColor();
		}


		if (ScrollToBottom || (AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
			ImGui::SetScrollHereY(1.0f);
		ScrollToBottom = false;


		ImGui::PopStyleVar();

		ImGui::EndChild();

		ImGui::Separator();

		ImGuiIO& io = ImGui::GetIO();

		// Se o checkbox de mover estiver habilitado ele para o personagem quando clicar na janela do chat
		if (!isMovingEnabled) {
			if (io.WantCaptureMouse) {
				//isnewclick = true;
			}
			else {
				//isnewclick = false;
			}
		}

		canClickButtons = true;
		// Evita trocar de chat quando ta no npc
		if (cAging.OpenFlag || cMixtureReset.OpenFlag || cWareHouse.OpenFlag || cCraftItem.OpenFlag || cShop.OpenFlag || ManufactureItem.m_OpenFlag || cSinHelp.NPCTeleportFlag || Caravana::GetInstance()->OpenFlag) {
			canClickButtons = false;
		}

		// Zera o txt quando enviar a mensagem
		if (ImGui::InputText("Mensagem", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_CallbackHistory,  &TextEditCallbackStub, (void*)this))
		{	
			char* chatMessage = InputBuf;
			wchar_t utf8Message[256];

			Strtrim(chatMessage);

			if (chatMessage[0])
			{
				// Converte a mensagem para UTF-8
				MultiByteToWideChar(CP_UTF8, 0, chatMessage, -1, utf8Message, IM_ARRAYSIZE(utf8Message));

				/*
				// Histórico de mensagens
				HistoryPos = -1;
				for (int i = History.Size - 1; i >= 0; i--)
					if (Stricmp(History[i], InputBuf) == 0)
					{
						free(History[i]);
						History.erase(History.begin() + i);
						break;
					}

				// Adiciona a mensagem ao vetor de histórico
				History.push_back(Strdup(chatMessage)); */

				if (chatMessage[0] == '/' || (smConfig.DebugMode && chatMessage[0] == '~') || chatMessage[0] == '`' || chatMessage[0] == '@')
				{
					strMessage = FormatString("%s", chatMessage);
				}
				else
				{
					switch (DispChatMode)
					{
					case 0:
					case 4:
						strMessage = FormatString("%s: %ls", PlayerName, utf8Message);
						break;
					case 1:
						strMessage = FormatString("/CLAN> %ls", utf8Message);
						break;
					case 2:
						strMessage = FormatString("@%ls", utf8Message);
						break;
					case 3:
						strMessage = FormatString("/TRADE> %ls", utf8Message);
						break;
					}
				}

				if (strncmp(strMessage.c_str(), "/TRADE>", 7) == 0)
				{
					//Elapsed = GetTickCount();

					if (true)//Start == 0)
					{
						//Start = GetTickCount();
						//SendChatMessageToServer(strMessage);

						// Buff chat global
						//setGlobalBuff();
					}
					//else if ((Elapsed - Start) > (TIME_GLOBAL * 1000))
					//{
						//Start = GetTickCount();
						//SendChatMessageToServer(strMessage);

						// Buff chat global
						//setGlobalBuff();
					//}
					//else
					//{
						//AddChatBuff("> Aguarde 10 segundos para enviar uma mensagem novamente!");
					//}
				}
				else
				{
					//SendChatMessageToServer(strMessage);
				}

				ScrollToBottom = true;
				//cInterFace.ChatFlag = true;
				sinChatInputMode = true;
				//sinChatEnter = true;
				// Limpa o buffer
				strcpy(chatMessage, "");
			}
			else {
				FOCUS_CHAT = false;
				sinChatInputMode = false;
				//cInterFace.ChatFlag = false;
				//sinChatEnter = false;
			}

			flagFromMouse = true;

			extern bool enterPartyMsg;

			if (enterPartyMsg)
			{
				if ((!strMessage.compare(msgWhisperParty)) && !flagWhisperParty)
					flagWhisperParty = true;
				else
					flagWhisperParty = false;
			}


			enterPartyMsg = false;
		}

		strMessage.clear();

		ImGui::SetItemDefaultFocus();

		ImGui::End();

	}

	void ExecCommand(const char* command_line)
	{
		if (Stricmp(command_line, "CLEAR") == 0)
		{
			ClearLog();
		}
		else if (Stricmp(command_line, "HISTORY") == 0)
		{
			int first = History.Size - 10;
			char message[32];
			for (int i = first > 0 ? first : 0; i < History.Size; i++)
				wsprintf(message, "%3d: %s\n", i, History[i]);
			AddLog(message, "System", 6);
		}
		else
		{
			char message[32];
			printf_s(message, sizeof(message), "Unknown command: '%s'", command_line);
			AddLog(message, "System", 3);
		}

		ScrollToBottom = true;
	}

	static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
	{
		NewChatWindow* console = (NewChatWindow*)data->UserData;
		return console->TextEditCallback(data);
	}


	int TextEditCallback(ImGuiInputTextCallbackData* data)
	{
		switch (data->EventFlag)
		{

		case ImGuiInputTextFlags_CallbackAlways:
		{
			const int prev_history_pos = HistoryPos;

			if (flagWhisperParty) {
				lastWhisper.clear();
				lastWhisper = msgWhisperParty;

				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, msgWhisperParty.c_str());

				flagWhisperParty = false;
			}
			/*
			if (msgHistoryUp) {
				if (HistoryPos == -1)
					HistoryPos = History.Size - 1;
				else if (HistoryPos > 0)
					HistoryPos--;

				msgHistoryUp = false;
			}

			if (msgHistoryDown) {
				if (HistoryPos != -1)
					if (++HistoryPos >= History.Size)
						HistoryPos = -1;

				msgHistoryDown = false;
			}


			if (prev_history_pos != HistoryPos)
			{
				const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, history_str);

			}*/
		}
		// break; 


		case ImGuiInputTextFlags_CallbackHistory:
		{

			if (data->EventKey == ImGuiKey_DownArrow)
			{
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, lastWhisper.c_str());

			}
			else if (data->EventKey == ImGuiKey_UpArrow)
			{
				data->DeleteChars(0, data->BufTextLen);
				data->InsertChars(0, lastReceiveWhisper.c_str());
			}

		}

		}
		return 0;
	}
};


static NewChatWindow chatwindow;

// Renderiza o chat na tela
static void NewChatWindowFunc(bool* p_open)
{
	chatwindow.Draw("Chat", 0);
}

// Envia mensagens no novo chat
void ImGuiFlags::SendChatMessage(std::string message, std::string sender, int dwIP) {

	CA2W pszWide(message.c_str());
	message = WChar_to_UTF8(pszWide);

	CA2W pszWide2(sender.c_str());
	sender = WChar_to_UTF8(pszWide2);

	if (dwIP != 9)
		chatwindow.AddLog(message.c_str(), sender.c_str(), dwIP);
	else
		chatwindow.AddLogAlert(message.c_str(), dwIP);

	message.clear();
	sender.clear();
}

void ImGuiFlags::ClearBuff() {
	strcpy(chatwindow.InputBuf, "");
	flagFromMouse = false;
	FOCUS_CHAT = false;
	flagCommand = true;
}

void ImGuiFlags::initPartyWhisper(std::string name) {
	msgWhisperParty.clear();

	msgWhisperParty = name;
	FOCUS_CHAT = true;

	flagWhisperParty = true;
}

// Atualiza frame a frame o chat
void ImGuiFlags::InstancesFlag() 
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();
	ImGui::StyleColorsRanking();

	/*
	if (flagChat)
	{
		NewChatWindowFunc(&flagChat);
	} */

	if (QuestWindow::GetInstance()->openFlag)
	{
		QuestWindow::GetInstance()->openWindow(&QuestWindow::GetInstance()->openFlag);
	}

	if (QuestWindow::GetInstance()->openOverlay)
	{
		QuestWindow::GetInstance()->questOverlay(&QuestWindow::GetInstance()->openOverlay);
	}

	if (EventoArena::GetInstance()->openFlag)
	{
		extern int horaHost;
		extern int minHost;
		extern int secHost;
		extern int diaHost;

		EventoArena::GetInstance()->arenaWindow(&EventoArena::GetInstance()->openFlag, horaHost, minHost, secHost, diaHost);
	}

	if (RestaureWindow::GetInstance()->openFlag)
	{
		RestaureWindow::GetInstance()->OpenNpc(&RestaureWindow::GetInstance()->openFlag);
	}

	if (RankingWindow::GetInstance()->openFlag)
	{
		RankingWindow::GetInstance()->OpenNpc(&RankingWindow::GetInstance()->openFlag);
	}

	if (MixWindow::GetInstance()->openFlag)
	{
		MixWindow::GetInstance()->OpenNpc(&MixWindow::GetInstance()->openFlag);
	}

	if (NewShop::GetInstance()->openFlag)
	{
		NewShop::GetInstance()->OpenNpc(&NewShop::GetInstance()->openFlag);
	}
	else 
	{
		NewShop::GetInstance()->editingNick = false;
	}

	if (NewShopTime::GetInstance()->openFlag)
	{
		NewShopTime::GetInstance()->OpenNpc(&NewShopTime::GetInstance()->openFlag);
	}

	if (SodWindow::GetInstance()->openFlag)
	{
		SodWindow::GetInstance()->OpenNpc(&SodWindow::GetInstance()->openFlag);
	}

	if (Roleta::GetInstance()->openFlag)
	{
		Roleta::GetInstance()->Show(&Roleta::GetInstance()->openFlag);
	}

	if (Roleta::GetInstance()->openRoleta)
	{
		Roleta::GetInstance()->ShowRoleta(&Roleta::GetInstance()->openRoleta);
	}

	ImGui::EndFrame();
}