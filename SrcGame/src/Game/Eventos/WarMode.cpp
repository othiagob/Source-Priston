#include "sinbaram/sinLinkHeader.h"
#include "WarMode.h"
#include <TitleBox/TitleBox.h>

MODOWAR WarMode;
extern int PLayerPvP;
extern int ReiPVP;
extern BOOL bPlayerPvP;

unsigned int ModoPvPTimer;

void MODOWAR::Init()
{
	WarModeImg[0] = CreateTextureMaterial("game\\images\\Warmode\\window.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	WarModeImg[1] = CreateTextureMaterial("game\\images\\Warmode\\start.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	WarModeImg[2] = CreateTextureMaterial("game\\images\\Warmode\\stop.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

}

BOOL MODOWAR::IsMousePos(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x && pRealCursorPos.x < x + w && pRealCursorPos.y > y && pRealCursorPos.y < y + h)
		return TRUE;

	return FALSE;

}

void MODOWAR::DrawImage(int Image, int x, int y, int w, int h)
{
	dsDrawTexImage(Image, x, y, w - 1, h - 1, 255);
}

void MODOWAR::Draw()
{
	if (fOpen)
	{

		BaseX = (WinSizeX - 305);
		BaseY = (WinSizeY - 430) / 2;
		char szBuff[256];
		SIZE size = {};

		DrawImage(WarModeImg[0], BaseX, BaseY, 291, 304);

		wsprintf(szBuff, "Jogador: %s", lpCurPlayer->smCharInfo.szName);
		GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
		dsTextLineOut(BaseX + (131 - size.cx / 2), BaseY + 125, szBuff, strlen(szBuff));

		wsprintf(szBuff, "Pontos de Batalhas: %d", lpCurPlayer->sBlessCastle_Damage[1]);
		GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
		dsTextLineOut(BaseX + (131 - size.cx / 2), BaseY + 150, szBuff, strlen(szBuff));

		wsprintf(szBuff, "Delay: 1 minuto para desativar!");
		GetFontTextExtentPoint(szBuff, strlen(szBuff), &size);
		dsTextLineOut(BaseX + (131 - size.cx / 2), BaseY + 170, szBuff, strlen(szBuff));


		if (!PLayerPvP)
			DrawImage(WarModeImg[1], BaseX + 96, BaseY + 223, 75, 21);
		else
			DrawImage(WarModeImg[2], BaseX + 96, BaseY + 223, 75, 21);
	}
}

void MODOWAR::Button()
{
	if (fOpen)
	{
		BaseX = (WinSizeX - 305);
		BaseY = (WinSizeY - 430) / 2;

		if (!PLayerPvP)
		{
			if (IsMousePos(BaseX + 45, BaseY + 188, 200, 84))
				cMessageBox.ShowMessage3(MODOPVP, "Deseja ativar M. Guerra?");
		}
		else
		{
			if (IsMousePos(BaseX + 45, BaseY + 188, 200, 84))
				cMessageBox.ShowMessage3(MODOPVP, "Deseja desativar M. Guerra?");
		}
	}
}

int MODOWAR::ModoPVP(bool arena)
{
	if (StageField[lpCurPlayer->OnStageField]->FieldCode == FIELD_ARENA) 
	{
		TitleBox::GetInstance()->SetText("Não é possível ativar nesse mapa!", 3);
		return FALSE;
	}

	sSKILL haPlayerPvP;

	if (!PLayerPvP && !arena)
	{
		if (GetTickCount() - ModoPvPTimer < 30000)
		{
			TitleBox::GetInstance()->SetText("Aguarde 30 segundos para utilizar novamente!", 3);
		}
		else
		{
			ModoPvPTimer = GetTickCount();

			TitleBox::GetInstance()->SetText("Modo Guerra Ativado!", 3);
			CHATBOX->AddMessage("Modo Guerra> Você agora ganha 10% de Drop adicional!", EChatColor::CHATCOLOR_Global);

			PLayerPvP = TRUE;

			for (int j = 0; j < SIN_MAX_SKILL; j++)
			{
				if (sSkill[j].CODE == BUFF_WARMODE)
				{
					memcpy(&haPlayerPvP, &sSkill[j], sizeof(sSKILL));
					haPlayerPvP.UseTime = 604800;
					sinContinueSkillSet(&haPlayerPvP);
					bPlayerPvP = TRUE;
					lpCurPlayer->PlayerPvPMode = TRUE;
					break;
				}
			}
		}

	}
	else
	{
		if ((GetTickCount() - ModoPvPTimer < 60000) && !arena)
		{
			TitleBox::GetInstance()->SetText("Aguarde 1 minuto para desativar!", 3);
		}
		else
		{
			if (!arena)
			{
				//DESATIVAR WarMode

				TitleBox::GetInstance()->SetText("Modo Guerra Desativado!", 3);
				CHATBOX->AddMessage("Modo PVP> Drop adicional desativada.", EChatColor::CHATCOLOR_Global);
			}

			if (PLayerPvP) 
			{
				ModoPvPTimer = GetTickCount();

				PLayerPvP = FALSE;

				for (int j = 0; j < SIN_MAX_SKILL; j++)
				{
					if (sSkill[j].CODE == BUFF_WARMODE)
					{
						memcpy(&haPlayerPvP, &sSkill[j], sizeof(sSKILL));
						haPlayerPvP.UseTime = 0;
						CSKILL->CancelContinueSkill(BUFF_WARMODE); //TIRA BUFF
						bPlayerPvP = FALSE;
						lpCurPlayer->PlayerPvPMode = FALSE; //TIRA EFEITO
						break;
					}
				}
			}
		}

	}

	smTRANS_COMMAND smTransCommand;
	smTransCommand.code = PACKET_WAR_MODE;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = PLayerPvP;

	if (smWsockServer)
		smWsockServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}