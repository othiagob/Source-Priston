#include <GameCore.h>
#include "ChatHandle.h"
#include "sinbaram\\sinlinkheader.h"

extern smCONFIG	smConfig;
extern int WarpField2(int Num);
extern int ServerHideMode;
extern int WaveCameraMode;

extern smCHAR_INFO* sinChar;

namespace CHAT
{

	Handle::Handle() : CBaseHandle()
	{

	}
	Handle::Handle(EHandle eID) : CBaseHandle(eID)
	{

	}
	Handle::~Handle()
	{

	}
	void Handle::Init()
	{

	}
	void Handle::Shutdown()
	{

	}
	void Handle::ProcessPacket(PacketItemLinkChat* psPacket, BOOL bLoginServer)
	{
		psPacket->code = PACKET_CHATITEMLINK;

		bLoginServer ? smWsockServer->Send((char*)psPacket, psPacket->size, TRUE) : smWsockUserServer->Send((char*)psPacket, psPacket->size, TRUE);
	}
	void Handle::AddChatBoxText(char* pszText, EChatColor iColor)
	{
		extern int GameMode;

		if (GameMode == 2)
			CHATBOX->AddMessage(pszText, iColor);
	}
	void Handle::AddChatBoxTextEx(EChatColor iColor, char* pszText, ...)
	{
		va_list vl;

		char szText[128];

		va_start(vl, pszText);
		_vsnprintf_s(szText, _countof(szText), _TRUNCATE, pszText, vl);
		va_end(vl);

		AddChatBoxText(szText, iColor);
	}
	void Handle::SendChat(BOOL bLoginServer, const char* pszChat)
	{
		extern smWINSOCK* smWsockServer;
		extern smWINSOCK* smWsockDataServer;

		int iChatLen = STRLEN(pszChat);

		if ((iChatLen < MAX_CHATLENGTH) && (iChatLen > 0))
		{
			PacketChat sPacket;
			sPacket.size = sizeof(PacketChat) + iChatLen + 1;
			sPacket.code = PACKET_CHAT_GAME;
			STRINGCOPY(sPacket.szChat, pszChat);

			bLoginServer ? smWsockServer->Send((char*)&sPacket, sPacket.size, TRUE) : smWsockUserServer->Send((char*)&sPacket, sPacket.size, TRUE);
		}
	}
	BOOL Handle::AnalyzeChat(char* pszBuff)
	{
		bool iRet = TRUE;

		char szChatCommandParam1[256] = { 0 };

		// Comandos Player
		if (ChatCommand("/camera_wave_off", pszBuff))
		{
			WaveCameraMode = FALSE;
		}
		else if (ChatCommand("/camera_wave_on", pszBuff))
		{
			WaveCameraMode = TRUE;
		}
		else if (ChatCommand("/quests", pszBuff))
		{
			if (sinQuest_Level30.State < 3 && sinChar->Level >= 30) {
				CheckCharForm();
				sinChar->StatePoint += 5;
				cMessageBox.ShowMessage(MESSAGE_PLUS_STATE_POINT5);
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				sinQuest_levelLog |= QUESTBIT_LEVEL_30;
				sinQuest_Level30.State = 3;
				sinQuest_Level30.BackUpCode = sinQuest_Level30.CODE;
				sinQuest_Level30.CODE = 0;
				EndQuest_Code(sinQuest_Level30.BackUpCode);
				sinQuest_Level30.CODE = 0;
				ReformCharForm();
			}

			if (sinQuest_Level55.State < 2 && sinChar->Level >= 55 && sinChar->JOB_CODE < 5) {
				CheckCharForm();
				sinSkill.SkillPoint += 1;
				cMessageBox.ShowMessage(MESSAGE_PLUS_SKILL_POINT);
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				sinQuest_levelLog |= QUESTBIT_LEVEL_55;
				sinQuest_Level55.State = 3;
				sinQuest_Level55.BackUpCode = sinQuest_Level55.CODE;
				sinQuest_Level55.CODE = 0;
				EndQuest_Code(sinQuest_Level55.BackUpCode);
				sinQuest_Level55.CODE = 0;

				ReformCharForm();

			}
			if (sinQuest_Level55_2.State < 2 && sinChar->Level >= 55 && sinChar->JOB_CODE > 4) {
				CheckCharForm();
				sinSkill.SkillPoint += 1;
				cMessageBox.ShowMessage(MESSAGE_PLUS_SKILL_POINT);
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				sinQuest_levelLog |= QUESTBIT_LEVEL_55;
				sinQuest_Level55_2.State = 3;
				sinQuest_Level55_2.BackUpCode = sinQuest_Level55_2.CODE;
				sinQuest_Level55_2.CODE = 0;
				EndQuest_Code(sinQuest_Level55_2.BackUpCode);
				sinQuest_Level55_2.CODE = 0;
				ReformCharForm();
			}

			if (sinQuest_Level70.State < 2 && sinChar->Level >= 70) {
				CheckCharForm();
				sinChar->StatePoint += 5;
				sinSkill.SkillPoint += 1;
				cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_70);
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				sinQuest_levelLog |= QUESTBIT_LEVEL_70;
				sinQuest_Level70.State = 3;
				sinQuest_Level70.BackUpCode = sinQuest_Level70.CODE;
				sinQuest_Level70.CODE = 0;
				EndQuest_Code(sinQuest_Level70.BackUpCode);
				sinQuest_Level70.CODE = 0;
				ReformCharForm();
			}

			if (sinQuest_Level80.State < 2 && sinChar->Level >= 80) {
				CheckCharForm();
				sinChar->StatePoint += 5;
				sinSkill.SkillPoint += 2;
				cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_80);
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				sinQuest_levelLog |= QUESTBIT_LEVEL_80;
				sinQuest_Level80.State = 3;
				sinQuest_Level80.BackUpCode = sinQuest_Level80.CODE;
				sinQuest_Level80.CODE = 0;
				EndQuest_Code(sinQuest_Level80.BackUpCode);
				sinQuest_Level80.CODE = 0;
				ReformCharForm();
			}

			if (sinQuest_Level85.State < 4 && sinChar->Level >= 85) {
				CheckCharForm();
				sinQuest_levelLog |= QUESTBIT_LEVEL_85;
				sinQuest_Level85.State = 4;
				sinQuest_Level85.BackUpCode = sinQuest_Level85.CODE;
				sinQuest_Level85.CODE = 0;
				EndQuest_Code(sinQuest_Level85.BackUpCode);
				sinQuest_Level85.CODE = 0;
				ReformCharForm();
			}

			if (sinQuest_Level90.State < 3 && sinChar->Level >= 90) {
				CheckCharForm();
				sinQuest_levelLog |= QUESTBIT_LEVEL_90;
				sinQuest_Level90.State = 3;
				sinQuest_Level90.BackUpCode = sinQuest_Level90.CODE;
				sinQuest_Level90.CODE = 0;
				EndQuest_Code(sinQuest_Level90.BackUpCode);
				sinQuest_Level90.CODE = 0;
				ReformCharForm();
				cInvenTory.SetItemToChar();
				if (!cInvenTory.SearchItemCode(sinQT1 | sin07)) {
					SendQuestCommandToServer(SIN_QUEST_CODE_LEVEL90, 0, 0, 0);
				}
			}
			if (sinQuest_Level80_2.State == 3) {
				DeleteQuestItem(sinQT1 | sin09);
				DeleteQuestItem(sinQT1 | sin10);
				DeleteQuestItem(sinQT1 | sin11);
			}
			if (sinQuest_Level80_2.State < 5 && sinChar->Level >= 80) {
				CheckCharForm();
				sinQuest_levelLog |= QUESTBIT_LEVEL_80_2;
				sinQuest_Level80_2.BackUpCode = sinQuest_Level80_2.CODE;
				sinQuest_Level80_2.CODE = 0;
				EndQuest_Code(sinQuest_Level80_2.BackUpCode);
				sinQuest_Level80_2.CODE = 0;
				sinQuest_Level80_2.State = 5;

				if (cInvenTory.SearchItemCode(sinQT1 | sin12)) {
					DeleteQuestItem(sinQT1 | sin12);
				}

				ReformCharStatePoint(sinChar, sinQuest_levelLog);
				ReformCharForm();
				cInvenTory.SetItemToChar();
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_80_2);
				SetQuestBoard();
			}

			if (sinQuest_Level90_2.State < 4 && sinChar->Level >= 90) {
				CheckCharForm();
				sinQuest_levelLog |= QUESTBIT_LEVEL_90_2;
				sinQuest_Level90_2.BackUpCode = sinQuest_Level90_2.CODE;
				sinQuest_Level90_2.CODE = 0;
				EndQuest_Code(sinQuest_Level90_2.BackUpCode);
				sinQuest_Level90_2.CODE = 0;
				sinQuest_Level90_2.State = 4;
				if (cInvenTory.SearchItemCode(sinQT1 | sin13)) {
					DeleteQuestItem(sinQT1 | sin13);
				}
				ReformCharStatePoint(sinChar, sinQuest_levelLog);
				ReformCharForm();
				cInvenTory.SetItemToChar();
				sinPlaySound(SIN_SOUND_EAT_POTION2);
				cMessageBox.ShowMessage(MESSAGE_PLUS_POINT_90_2);
				SetQuestBoard();
			}
		}
		else if (ChatCommand("/save", pszBuff))
		{
			if (SaveGameData())
				CHATBOX->AddMessage("> Jogo salvo com sucesso.");
			else
				CHATBOX->AddMessage("> Falha ao salvar.");
		}
		else if (ChatCommand("/ranking", pszBuff))
		{
			RankingWindow::GetInstance()->OpenNPCRanking();
		}
		else if (ChatCommand("/mix", pszBuff))
		{
			MixWindow::GetInstance()->ReadMix();
		}
		else if (ChatCommand("/damage", pszBuff))
		{
			if (smConfig.showDamage)
			{
				smConfig.showDamage = FALSE;
				CHATBOX->AddMessage("> Mostrador de dano Ligado!");
			}
			else
			{
				smConfig.showDamage = TRUE;
				CHATBOX->AddMessage("> Mostrador de dano desligado!");
			}
		}
		else if (ChatCommand("/fps", pszBuff))
		{
			extern BOOL bShowFPS;

			if (bShowFPS)
			{
				bShowFPS = FALSE;
				CHATBOX->AddMessage("> FPS agora não está visível");
			}
			else
			{
				bShowFPS = TRUE;
				CHATBOX->AddMessage("> FPS está agora visível");
			}
		}
		else if (lpCurPlayer->vipLevel > 0)
		{
			if (ChatCommand("/core", pszBuff))
			{
				chaPremiumitem.UsePremiumItem(8);
			}

			if (ChatCommand("/mvip", pszBuff))
			{
				WarpField2(50);
			}
		}
		else if (ChatCommand("/vip", pszBuff))
		{
			if (VIP::GetInstance()->getVipLevel())
				VIP::GetInstance()->vipCommands();
		}

		// Comandos GM
		if (smConfig.DebugMode)
		{
			if (ChatCommand("/set_absorb", pszBuff))
			{
				if (GetParameterString(pszBuff, 1, szChatCommandParam1))
				{
					if (lpCharSelPlayer != nullptr && lpCharSelPlayer->smCharInfo.State == smCHAR_STATE_ENEMY)
					{
						SendChat(FALSE, FormatString("/set_absorb %d %d", lpCharSelPlayer->dwObjectSerial, atoi(szChatCommandParam1)));
						iRet = FALSE;
					}
				}
			}
			
			else if (ChatCommand("/talk", pszBuff))
			{
				if (GetParameterString(pszBuff, 1, szChatCommandParam1))
				{
					if (lpCharSelPlayer != nullptr && lpCharSelPlayer->smCharInfo.State != smCHAR_STATE_USER)
					{
						SendChat(FALSE, FormatString("/talk %d %s", lpCharSelPlayer->dwObjectSerial, szChatCommandParam1));
						iRet = FALSE;
					}
				}
			}

			else if (ChatCommand("/field", pszBuff))
			{
				GetParameterString(pszBuff, 1, szChatCommandParam1);
				WarpField2(atoi(szChatCommandParam1));
			}

			else if (ChatCommand("/hide", pszBuff))
			{
				ServerHideMode = TRUE;
				CHATBOX->AddMessage("> Você agora está invisível");
			}

			else if (ChatCommand("/show", pszBuff))
			{
				ServerHideMode = FALSE;
				CHATBOX->AddMessage("> Você agora está visível");
			}

			else if (ChatCommand("/PassRankUp", pszBuff))
			{
				if (sinChar->ChangeJob < 3)
				{
					sinChar->ChangeJob++;
					CSKILL->SearchUseSkill();
					CHATBOX->AddMessage("> Você passou de ranque!");
				}
				else
					CHATBOX->AddMessage("> Ranque máximo atingido!");
			}
		}

		return iRet;
	}
}