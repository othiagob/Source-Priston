#include "sinbaram\\sinlinkheader.h"
#include "HoBaram\\HoLinkHeader.h"

int tab;
bool TextureBox = false, ViewBox = false, ZoomBox = false;

bool Settings::MouseAction(int x, int y, int w, int h)
{
	if (pRealCursorPos.x > x && pRealCursorPos.x < x + w && pRealCursorPos.y > y && pRealCursorPos.y < y + h)
		return TRUE;

	return FALSE;
}

void Settings::Init()
{
	cImages[1] = CreateTextureMaterial("game\\images\\settings\\window.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[15] = CreateTextureMaterial("game\\images\\settings\\checkbox.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[16] = CreateTextureMaterial("game\\images\\settings\\checkbox_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[18] = CreateTextureMaterial("game\\images\\settings\\combo_box.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[20] = CreateTextureMaterial("game\\images\\settings\\save_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[22] = CreateTextureMaterial("game\\images\\settings\\reset_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[24] = CreateTextureMaterial("game\\images\\settings\\exit_hover.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[26] = CreateTextureMaterial("game\\images\\settings\\video_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[28] = CreateTextureMaterial("game\\images\\settings\\audio_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[30] = CreateTextureMaterial("game\\images\\settings\\general_.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[31] = CreateTextureMaterial("game\\images\\settings\\combo_box_on.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[32] = CreateTextureMaterial("game\\images\\settings\\combo_box_down1.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[33] = CreateTextureMaterial("game\\images\\settings\\combo_box_down2.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[34] = CreateTextureMaterial("game\\images\\settings\\combo_box_down3.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[35] = CreateTextureMaterial("game\\images\\settings\\combo_box_down_select_bottom.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[36] = CreateTextureMaterial("game\\images\\settings\\combo_box_down_select.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[38] = CreateTextureMaterial("game\\images\\settings\\volumebar.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	cImages[39] = CreateTextureMaterial("game\\images\\settings\\selector.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	ReadTextures();
}

void Settings::Open()
{
	cOpen = true;
}

void Settings::Close()
{
	cOpen = false;
}

void Settings::Draw()
{
	if (cOpen)
	{
		int WinX = (smScreenWidth / 2) - 280, WinY = ((smScreenHeight - 600) / 2) + 50;

		dsDrawTexImage(cImages[1], WinX, WinY, 559, 438, 255);

		if (Tab == 0 || MouseAction(WinX + 42, WinY + 49, 122, 30))
			dsDrawTexImage(cImages[26], WinX + 42, WinY + 49, 122, 30, 255);

		if (Tab == 1 || MouseAction(WinX + 163, WinY + 49, 122, 30))
			dsDrawTexImage(cImages[28], WinX + 163, WinY + 49, 122, 30, 255);

		if (Tab == 2 || MouseAction(WinX + 284, WinY + 49, 122, 30))
			dsDrawTexImage(cImages[30], WinX + 284, WinY + 49, 122, 30, 255);

		if (MouseAction(WinX + 298, WinY + 386, 111, 45))
			dsDrawTexImage(cImages[22], WinX + 298, WinY + 386, 94, 24, 255);

		if (MouseAction(WinX + 395, WinY + 386, 111, 45))
			dsDrawTexImage(cImages[20], WinX + 395, WinY + 386, 94, 24, 255);

		if (MouseAction(WinX + 513, WinY + 7, 33, 48))
			dsDrawTexImage(cImages[24], WinX + 513, WinY + 7, 33, 48, 255);

		if (Tab == 0)
		{

			dsTextLineOut(WinX + 50, WinY + 85, "Vídeo", 5);
			dsTextLineOut(WinX + 50, WinY + 134, "Ratio", 7);
			dsTextLineOut(WinX + 155, WinY + 134, "Resolução", 12);

			if (!bWindowed)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 107, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 110, "Tela Cheia", 11);

			if (bWindowed)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 107, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 110, "Md. Janela", 11);

			if (bAutoAdjust)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 107, 20, 20, 255);

			dsTextLineOut(WinX + 285, WinY + 110, "A. Ajuste", 12);


			if (bRatio == 0)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 156, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 156, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 159, "4:3", 4);

			if (bRatio == 1)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 176, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 176, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 179, "5:4", 4);

			if (bRatio == 2)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 196, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 196, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 199, "16:9 (wd)", 18);

			if (bRatio == 3)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 216, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 216, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 219, "16:10 (wd)", 19);

			dsTextLineOut(WinX + 390, WinY + 110, "Mover HUD", 17);
			
			if (bRatio == 0)
			{
				if (bResolution == 0)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 156, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 156, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 159, "800x600", 8);

				if (bResolution == 1)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 176, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 176, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 179, "1024x768", 9);

				if (bResolution == 2)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 196, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 196, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 199, "1280x960", 9);

				if (bResolution == 3)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 216, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 216, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 219, "1400x1050", 10);
			}
			else if (bRatio == 1)
			{
				if (bResolution == 4)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 156, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 156, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 159, "1280x1024", 10);
			}
			else if (bRatio == 2)
			{
				if (bResolution == 5)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 156, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 156, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 159, "1280x720", 9);

				if (bResolution == 6)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 176, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 176, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 179, "1366x768", 9);

				if (bResolution == 7)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 196, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 196, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 199, "1600x900", 9);

				if (bResolution == 8)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 216, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 216, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 219, "1920x1080", 10);
			}
			else if (bRatio == 3)
			{

				if (bResolution == 9)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 156, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 156, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 159, "1280x800", 9);

				if (bResolution == 10)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 176, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 176, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 179, "1440x900", 9);

				if (bResolution == 11)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 196, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 196, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 199, "1680x1050", 9);

				if (bResolution == 12)
					dsDrawTexImage(cImages[16], WinX + 155, WinY + 216, 20, 20, 255);
				else
					dsDrawTexImage(cImages[15], WinX + 155, WinY + 216, 20, 20, 255);

				dsTextLineOut(WinX + 180, WinY + 219, "1920x1200", 10);
			}

			dsTextLineOut(WinX + 50, WinY + 241, "Textura", 17);
			dsTextLineOut(WinX + 260, WinY + 134, "Bit Depth", 11);

			if (bBPP == 32)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 156, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 156, 20, 20, 255);

			dsTextLineOut(WinX + 285, WinY + 159, "32 bit", 7);

			if (bBPP == 16)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 176, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 176, 20, 20, 255);

			dsTextLineOut(WinX + 285, WinY + 179, "16 bit", 7);

			dsTextLineOut(WinX + 155, WinY + 241, "Efeitos", 7);

			dsTextLineOut(WinX + 155, WinY + 312, "Dano", 7);

			dsTextLineOut(WinX + 365, WinY + 134, "Luzes Dinâmicas", 16);

			dsTextLineOut(WinX + 365, WinY + 241, "Vsync", 21);

			dsTextLineOut(WinX + 260, WinY + 241, "Sombras D.", 17);

			if (smConfig.showDamage)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 334, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 334, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 337, "Desativar", strlen("Desativar"));

			if (!smConfig.showDamage)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 356, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 356, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 359, "Ativar", strlen("Ativar"));

			if (bEffects)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 263, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 263, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 266, "Ativar", strlen("Ativar"));

			if (!bEffects)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 285, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 285, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 288, "Desativar", strlen("Desativar"));

			if (bLights)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 156, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 156, 20, 20, 255);

			dsTextLineOut(WinX + 390, WinY + 159, "Ativar", strlen("Ativar"));

			if (!bLights)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 176, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 176, 20, 20, 255);

			dsTextLineOut(WinX + 390, WinY + 179, "Desativar", strlen("Desativar"));

			if (bShadows)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 263, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 263, 20, 20, 255);

			dsTextLineOut(WinX + 285, WinY + 266, "Ativar", strlen("Ativar"));

			if (!bShadows)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 285, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 285, 20, 20, 255);

			dsTextLineOut(WinX + 285, WinY + 288, "Desativar", strlen("Desativar"));

			if(vSync)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 263, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 263, 20, 20, 255);

			dsTextLineOut(WinX + 390, WinY + 266, "Ativar", strlen("Ativar"));

			if (!vSync)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 285, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 285, 20, 20, 255);

			dsTextLineOut(WinX + 390, WinY + 288, "Desativar", strlen("Desativar"));
			
			if(vBlockUI)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 107, 20, 20, 255);

			if (TextureBox)
			{
				HoDrawTexImage(cImages[32], float(WinX + 50), float(WinY + 263), 86.f, 26.f, 0.f, 0.f, 86., 26.f, 255);
				HoDrawTexImage(cImages[33], float(WinX + 50), float(WinY + 287), 86.f, 24.f, 0.f, 0.f, 86., 24.f, 255);
				HoDrawTexImage(cImages[33], float(WinX + 50), float(WinY + 311), 86.f, 24.f, 0.f, 0.f, 86., 24.f, 255);
				HoDrawTexImage(cImages[33], float(WinX + 50), float(WinY + 335), 86.f, 24.f, 0.f, 0.f, 86., 24.f, 255);
				HoDrawTexImage(cImages[34], float(WinX + 50), float(WinY + 359), 86.f, 25.f, 0.f, 0.f, 86., 25.f, 255);

				if (MouseAction(WinX + 50, WinY + 287, 86, 26))
					HoDrawTexImage(cImages[36], float(WinX + 50), float(WinY + 287), 86.f, 24.f, 0.f, 0.f, 86., 24.f, 255);
				if (MouseAction(WinX + 50, WinY + 313, 86, 24))
					HoDrawTexImage(cImages[36], float(WinX + 50), float(WinY + 313), 86.f, 24.f, 0.f, 0.f, 86., 24.f, 255);
				if (MouseAction(WinX + 50, WinY + 337, 86, 24))
					HoDrawTexImage(cImages[36], float(WinX + 50), float(WinY + 337), 86.f, 24.f, 0.f, 0.f, 86., 24.f, 255);
				if (MouseAction(WinX + 50, WinY + 361, 86, 25))
					HoDrawTexImage(cImages[35], float(WinX + 50), float(WinY + 361), 86.f, 25.f, 0.f, 0.f, 86., 25.f, 255);

				dsTextLineOut(WinX + 78, WinY + 290, "Alto", 4);
				dsTextLineOut(WinX + 73, WinY + 315, "Médio", 5);
				dsTextLineOut(WinX + 75, WinY + 340, "Baixo", 5);
				dsTextLineOut(WinX + 71, WinY + 364, "Baixo +", 11);
			}
			else
			{

				if (MouseAction(WinX + 50, WinY + 263, 86, 26))
					HoDrawTexImage(cImages[31], float(WinX + 50), float(WinY + 263), 86.f, 26.f, 0.f, 0.f, 86.f, 26.f, 255);
				else
					HoDrawTexImage(cImages[18], float(WinX + 50), float(WinY + 263), 86.f, 26.f, 0.f, 0.f, 86.f, 26.f, 255);

			}

			if (bTexture == 0)
				dsTextLineOut(WinX + 70, WinY + 268, "Alto", 4);
			else if (bTexture == 1)
				dsTextLineOut(WinX + 65, WinY + 268, "Médio", 5);
			else if (bTexture == 2)
				dsTextLineOut(WinX + 65, WinY + 268, "Baixo", 5);
			else if (bTexture == 3)
				dsTextLineOut(WinX + 62, WinY + 268, "Baixo +", 11);


		}
		else if (Tab == 1)
		{

			if (bMusic)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 107, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 110, "Volume Principal", 16);
			dsTextLineOut(WinX + 50, WinY + 85, "Musica", 21);

			HoDrawTexImage(cImages[38], float(WinX + 50), float(WinY + 140), 187.f, 34.f, 0.f, 0.f, 187.f, 34.f, 255);

			if (bMVol == 0)
				HoDrawTexImage(cImages[39], float(WinX + 48), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 1)
				HoDrawTexImage(cImages[39], float(WinX + 69), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 2)
				HoDrawTexImage(cImages[39], float(WinX + 90), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 3)
				HoDrawTexImage(cImages[39], float(WinX + 111), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 4)
				HoDrawTexImage(cImages[39], float(WinX + 132), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 5)
				HoDrawTexImage(cImages[39], float(WinX + 153), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 6)
				HoDrawTexImage(cImages[39], float(WinX + 174), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 7)
				HoDrawTexImage(cImages[39], float(WinX + 195), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bMVol == 8)
				HoDrawTexImage(cImages[39], float(WinX + 216), float(WinY + 137), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);

			if (GetAsyncKeyState(VK_LBUTTON) != 0)
			{
				if (MouseAction(WinX + 48, WinY + 137, 21, 52))
					bMVol = 0;
				if (MouseAction(WinX + 69, WinY + 137, 21, 52))
					bMVol = 1;
				if (MouseAction(WinX + 90, WinY + 137, 21, 52))
					bMVol = 2;
				if (MouseAction(WinX + 111, WinY + 137, 21, 52))
					bMVol = 3;
				if (MouseAction(WinX + 132, WinY + 137, 21, 52))
					bMVol = 4;
				if (MouseAction(WinX + 153, WinY + 137, 21, 52))
					bMVol = 5;
				if (MouseAction(WinX + 174, WinY + 137, 21, 52))
					bMVol = 6;
				if (MouseAction(WinX + 195, WinY + 137, 21, 52))
					bMVol = 7;
				if (MouseAction(WinX + 216, WinY + 137, 21, 52))
					bMVol = 8;
			}


			if (bSound)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 217, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 217, 20, 20, 255);

			if (bAmbient)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 217, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 217, 20, 20, 255);

			dsTextLineOut(WinX + 75, WinY + 220, "Som Principal", 13);
			dsTextLineOut(WinX + 180, WinY + 220, "Sons de Ambiente", 16);
			dsTextLineOut(WinX + 50, WinY + 195, "Volume", 6);

			HoDrawTexImage(cImages[38], float(WinX + 50), float(WinY + 247), 187.f, 34.f, 0.f, 0.f, 187.f, 34.f, 255);


			if (bSVol == 0)
				HoDrawTexImage(cImages[39], float(WinX + 48), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 1)
				HoDrawTexImage(cImages[39], float(WinX + 69), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 2)
				HoDrawTexImage(cImages[39], float(WinX + 90), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 3)
				HoDrawTexImage(cImages[39], float(WinX + 111), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 4)
				HoDrawTexImage(cImages[39], float(WinX + 132), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 5)
				HoDrawTexImage(cImages[39], float(WinX + 153), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 6)
				HoDrawTexImage(cImages[39], float(WinX + 174), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 7)
				HoDrawTexImage(cImages[39], float(WinX + 195), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);
			else if (bSVol == 8)
				HoDrawTexImage(cImages[39], float(WinX + 216), float(WinY + 244), 23.f, 52.f, 0.f, 0.f, 23.f, 52.f, 255);

			if (GetAsyncKeyState(VK_LBUTTON) != 0)
			{
				if (MouseAction(WinX + 48, WinY + 244, 21, 52))
					bSVol = 0;
				if (MouseAction(WinX + 69, WinY + 244, 21, 52))
					bSVol = 1;
				if (MouseAction(WinX + 90, WinY + 244, 21, 52))
					bSVol = 2;
				if (MouseAction(WinX + 111, WinY + 244, 21, 52))
					bSVol = 3;
				if (MouseAction(WinX + 132, WinY + 244, 21, 52))
					bSVol = 4;
				if (MouseAction(WinX + 153, WinY + 244, 21, 52))
					bSVol = 5;
				if (MouseAction(WinX + 174, WinY + 244, 21, 52))
					bSVol = 6;
				if (MouseAction(WinX + 195, WinY + 244, 21, 52))
					bSVol = 7;
				if (MouseAction(WinX + 216, WinY + 244, 21, 52))
					bSVol = 8;
			}
		}
		else if (Tab == 2)
		{
			dsTextLineOut(WinX + 50, WinY + 85, "Visão", 28);
			dsTextLineOut(WinX + 260, WinY + 85, "Zoom", 27);
			dsTextLineOut(WinX + 155, WinY + 85, "Câmera A.", 14);
			dsTextLineOut(WinX + 365, WinY + 85, "Câmera I. e Add.", 17);
			dsTextLineOut(WinX + 390, WinY + 154, "Ocultar Nome", 27);
			dsTextLineOut(WinX + 390, WinY + 176, "Exibir HP", 27);
			dsTextLineOut(WinX + 390, WinY + 198, "Exibir Alerta", 27);

			if (bHidePlayerNames)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 151, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 151, 20, 20, 255);

			if (bShowLife)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 173, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 173, 20, 20, 255);

			if (bShowNotice)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 195, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 195, 20, 20, 255);

			if (bCamShake)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 107, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 110, "Ativar", strlen("Ativar"));

			if (!bCamShake)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 129, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 129, 20, 20, 255);

			dsTextLineOut(WinX + 180, WinY + 132, "Desativar", strlen("Desativar"));



			if (bCamInv)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 107, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 107, 20, 20, 255);

			dsTextLineOut(WinX + 390, WinY + 110, "Ativar", strlen("Ativar"));

			if (!bCamInv)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 129, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 129, 20, 20, 255);

			dsTextLineOut(WinX + 390, WinY + 132, "Desativar", strlen("Desativar"));


			if (bFilter)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 315, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 315, 20, 20, 255);

			if (bFilterSpec)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 295, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 295, 20, 20, 255);

			dsTextLineOut(WinX + 50, WinY + 213, "Filtros Gerais", 15);
			dsTextLineOut(WinX + 180, WinY + 318, "Habilitar F.", 15);
			/*dsTextLineOut(WinX + 365, WinY + 213, "Filtros de Spec", 20);*/
			dsTextLineOut(WinX + 390, WinY + 298, "Habilitar F. de Spec", 20);
			
			if (bHP)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 235, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 235, 20, 20, 255);

			if (bMP)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 255, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 255, 20, 20, 255);

			if (bSP)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 275, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 275, 20, 20, 255);

			if (bGold)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 295, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 295, 20, 20, 255);

			if (bAmulets)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 315, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 315, 20, 20, 255);

			if (bRings)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 335, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 335, 20, 20, 255);

			if (bSheltoms)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 355, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 355, 20, 20, 255);

			if (bForce)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 375, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 375, 20, 20, 255);

			if (bPremiums)
				dsDrawTexImage(cImages[16], WinX + 50, WinY + 395, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 50, WinY + 395, 20, 20, 255);

			if (bCrystal)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 235, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 235, 20, 20, 255);

			if (bDefItem)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 255, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 255, 20, 20, 255);

			if (bOffItem)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 275, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 275, 20, 20, 255);

			if (bElse)
				dsDrawTexImage(cImages[16], WinX + 155, WinY + 295, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 155, WinY + 295, 20, 20, 255);


			dsTextLineOut(WinX + 75, WinY + 238, "Poção de HP", 11);
			dsTextLineOut(WinX + 75, WinY + 258, "Poção de MP", 11);
			dsTextLineOut(WinX + 75, WinY + 278, "Poção de SP", 11);
			dsTextLineOut(WinX + 75, WinY + 298, "Ouro", 5);
			dsTextLineOut(WinX + 75, WinY + 318, "Amuletos", 8);
			dsTextLineOut(WinX + 75, WinY + 338, "Anéis", 6);
			dsTextLineOut(WinX + 75, WinY + 358, "Sheltons", 9);
			dsTextLineOut(WinX + 75, WinY + 378, "Force Orbs", 11);
			dsTextLineOut(WinX + 75, WinY + 398, "Premiums", 9);
			dsTextLineOut(WinX + 180, WinY + 238, "Cristais", 17);

			dsTextLineOut(WinX + 180, WinY + 258, "I. de Defesa", 14);
			dsTextLineOut(WinX + 180, WinY + 278, "I. de Ataque", 14);
			dsTextLineOut(WinX + 180, WinY + 298, "Restante", 16);


			if (bMS)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 235, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 235, 20, 20, 255);

			if (bFS)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 255, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 255, 20, 20, 255);

			if (bPS)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 275, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 275, 20, 20, 255);

			if (bAS)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 295, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 295, 20, 20, 255);

			if (bKS)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 315, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 315, 20, 20, 255);

			if (bATS)
				dsDrawTexImage(cImages[16], WinX + 260, WinY + 335, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 260, WinY + 335, 20, 20, 255);

			if (bPRS)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 235, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 235, 20, 20, 255);

			if (bMGS)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 255, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 255, 20, 20, 255);

			if (bNaked)
				dsDrawTexImage(cImages[16], WinX + 365, WinY + 275, 20, 20, 255);
			else
				dsDrawTexImage(cImages[15], WinX + 365, WinY + 275, 20, 20, 255);

			dsTextLineOut(WinX + 285, WinY + 238, "Mecânico", strlen("Mecânico"));
			dsTextLineOut(WinX + 285, WinY + 258, "Lutador", strlen("Lutador"));
			dsTextLineOut(WinX + 285, WinY + 278, "Pikeman", strlen("Pikeman"));
			dsTextLineOut(WinX + 285, WinY + 298, "Arqueira", strlen("Arqueira"));
			dsTextLineOut(WinX + 285, WinY + 318, "Cavaleiro", strlen("Cavaleiro"));
			dsTextLineOut(WinX + 285, WinY + 338, "Atalanta", strlen("Atalanta"));
			dsTextLineOut(WinX + 390, WinY + 238, "Sacerdotisa", strlen("Sacerdotisa"));
			dsTextLineOut(WinX + 390, WinY + 258, "Mago", strlen("Mago"));
			dsTextLineOut(WinX + 390, WinY + 278, "Sem Classe", strlen("Sem Classe"));
			
			if (ViewBox)//
			{
				HoDrawTexImage(cImages[32], WinX + 50, WinY + 107, 86.f, 26.f, 0, 0, 86.f, 26.f, 255);
				HoDrawTexImage(cImages[33], WinX + 50, WinY + 133, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);
				HoDrawTexImage(cImages[33], WinX + 50, WinY + 157, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);
				HoDrawTexImage(cImages[34], WinX + 50, WinY + 181, 86.f, 25.f, 0, 0, 86.f, 25.f, 255);

				if (MouseAction(WinX + 50, WinY + 133, 86, 24))
					HoDrawTexImage(cImages[36], WinX + 50, WinY + 133, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);

				if (MouseAction(WinX + 50, WinY + 157, 86, 24))
					HoDrawTexImage(cImages[36], WinX + 50, WinY + 157, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);

				if (MouseAction(WinX + 50, WinY + 181, 86, 25))
					HoDrawTexImage(cImages[35], WinX + 50, WinY + 181, 86.f, 25.f, 0, 0, 86.f, 25.f, 255);

				dsTextLineOut(WinX + 76, WinY + 136, "Perto", 6);
				dsTextLineOut(WinX + 78, WinY + 160, "Longe", 4);
				dsTextLineOut(WinX + 73, WinY + 184, "Máximo", 5);
			}
			else
			{
				if (MouseAction(WinX + 50, WinY + 107, 86, 26))
					HoDrawTexImage(cImages[31], WinX + 50, WinY + 107, 86.f, 26.f, 0, 0, 86.f, 26.f, 255);
				else
					HoDrawTexImage(cImages[18], WinX + 50, WinY + 107, 86.f, 26.f, 0, 0, 86.f, 26.f, 255);
			}


			if (bCamView == 0)
				dsTextLineOut(WinX + 69, WinY + 112, "Perto", 6);
			else if (bCamView == 1)
				dsTextLineOut(WinX + 66, WinY + 112, "Longe", 4);
			else if (bCamView == 2)
				dsTextLineOut(WinX + 63, WinY + 112, "Máximo", 5);



			if (ZoomBox)
			{
				HoDrawTexImage(cImages[32], WinX + 260, WinY + 107, 86.f, 26.f, 0, 0, 86.f, 26.f, 255);
				HoDrawTexImage(cImages[33], WinX + 260, WinY + 133, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);
				HoDrawTexImage(cImages[33], WinX + 260, WinY + 157, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);
				HoDrawTexImage(cImages[33], WinX + 260, WinY + 181, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);
				HoDrawTexImage(cImages[34], WinX + 260, WinY + 205, 86.f, 25.f, 0, 0, 86.f, 25.f, 255);

				if (MouseAction(WinX + 260, WinY + 133, 86, 24))
					HoDrawTexImage(cImages[36], WinX + 260, WinY + 133, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);

				if (MouseAction(WinX + 260, WinY + 157, 86, 24))
					HoDrawTexImage(cImages[36], WinX + 260, WinY + 157, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);

				if (MouseAction(WinX + 260, WinY + 181, 86, 24))
					HoDrawTexImage(cImages[36], WinX + 260, WinY + 181, 86.f, 24.f, 0, 0, 86.f, 24.f, 255);
				//363
				if (MouseAction(WinX + 260, WinY + 205, 86, 25))
					HoDrawTexImage(cImages[35], WinX + 260, WinY + 205, 86.f, 25.f, 0, 0, 86.f, 25.f, 255);

				dsTextLineOut(WinX + 289, WinY + 136, "Perto", 6);
				dsTextLineOut(WinX + 282, WinY + 160, "Regular", 8);
				dsTextLineOut(WinX + 287, WinY + 184, "Longe", 4);
				dsTextLineOut(WinX + 284, WinY + 208, "Máximo", 10);
			}
			else
			{

				if (MouseAction(WinX + 260, WinY + 107, 86, 26))
					HoDrawTexImage(cImages[31], WinX + 260, WinY + 107, 86.f, 26.f, 0, 0, 86.f, 26.f, 255);
				else
					HoDrawTexImage(cImages[18], WinX + 260, WinY + 107, 86.f, 26.f, 0, 0, 86.f, 26.f, 255);

			}


			if (bCamRange == 0)
				dsTextLineOut(WinX + 276, WinY + 112, "Perto", 6);
			else if (bCamRange == 1)
				dsTextLineOut(WinX + 273, WinY + 112, "Regular", 8);
			else if (bCamRange == 2)
				dsTextLineOut(WinX + 276, WinY + 112, "Longe", 4);
			else if (bCamRange == 3)
				dsTextLineOut(WinX + 272, WinY + 112, "Máximo", 10);
		}
	}
}

void Settings::Button()
{
	if (cOpen)
	{
		int WinX = (smScreenWidth / 2) - 280, WinY = ((smScreenHeight - 600) / 2) + 50;

		if (MouseAction(WinX + 43, WinY + 49, 122, 30))
			Tab = 0;

		if (MouseAction(WinX + 164, WinY + 49, 122, 30))
			Tab = 1;

		if (MouseAction(WinX + 285, WinY + 49, 122, 30))
			Tab = 2;

		if (Tab == 0)
		{
			if (MouseAction(WinX + 50, WinY + 107, 15, 15))
				bWindowed = false;

			if (MouseAction(WinX + 155, WinY + 107, 15, 15))
				bWindowed = true;

			if (MouseAction(WinX + 260, WinY + 107, 15, 15))
			{
				if (!bAutoAdjust)
					bAutoAdjust = true;
				else
					bAutoAdjust = false;
			}

			if (MouseAction(WinX + 50, WinY + 156, 15, 15))
				bRatio = 0;
			if (MouseAction(WinX + 50, WinY + 176, 15, 15))
				bRatio = 1;
			if (MouseAction(WinX + 50, WinY + 196, 15, 15))
				bRatio = 2;
			if (MouseAction(WinX + 50, WinY + 216, 15, 15))
				bRatio = 3;

			if (bRatio == 0)
			{
				if (MouseAction(WinX + 155, WinY + 156, 15, 15))
					bResolution = 0;
				if (MouseAction(WinX + 155, WinY + 176, 15, 15))
					bResolution = 1;
				if (MouseAction(WinX + 155, WinY + 196, 15, 15))
					bResolution = 2;
				if (MouseAction(WinX + 155, WinY + 216, 15, 15))
					bResolution = 3;
			}
			else if (bRatio == 1)
			{
				if (MouseAction(WinX + 155, WinY + 156, 15, 15))
					bResolution = 4;
			}
			else if (bRatio == 2)
			{
				if (MouseAction(WinX + 155, WinY + 156, 15, 15))
					bResolution = 5;
				if (MouseAction(WinX + 155, WinY + 176, 15, 15))
					bResolution = 6;
				if (MouseAction(WinX + 155, WinY + 196, 15, 15))
					bResolution = 7;
				if (MouseAction(WinX + 155, WinY + 216, 15, 15))
					bResolution = 8;
			}
			else if (bRatio == 3)
			{
				if (MouseAction(WinX + 155, WinY + 156, 15, 15))
					bResolution = 9;
				if (MouseAction(WinX + 155, WinY + 176, 15, 15))
					bResolution = 10;
				if (MouseAction(WinX + 155, WinY + 196, 15, 15))
					bResolution = 11;
				if (MouseAction(WinX + 155, WinY + 216, 15, 15))
					bResolution = 12;
			}

			if (MouseAction(WinX + 260, WinY + 156, 15, 15))
				bBPP = 32;
			if (MouseAction(WinX + 260, WinY + 176, 15, 15))
				bBPP = 16;

			if (MouseAction(WinX + 155, WinY + 334, 15, 15))
				smConfig.showDamage = true;
			if (MouseAction(WinX + 155, WinY + 356, 15, 15))
				smConfig.showDamage = false;

			if (MouseAction(WinX + 155, WinY + 263, 15, 15))
				bEffects = true;
			if (MouseAction(WinX + 155, WinY + 285, 15, 15))
				bEffects = false;

			if (MouseAction(WinX + 365, WinY + 156, 15, 15))
				bLights = true;
			if (MouseAction(WinX + 365, WinY + 176, 15, 15))
				bLights = false;

			if (MouseAction(WinX + 260, WinY + 263, 15, 15))
				bShadows = true;
			if (MouseAction(WinX + 260, WinY + 285, 15, 15))
				bShadows = false;

			if (MouseAction(WinX + 365, WinY + 263, 15, 15))
				vSync = true;
			if (MouseAction(WinX + 365, WinY + 285, 15, 15))
				vSync = false;

			if (MouseAction(WinX + 365, WinY + 107, 15, 15))
			{
				vBlockUI == true ? vBlockUI = false : vBlockUI = true;
			}

			if (TextureBox)
			{
				TextureBox = false;
				if (MouseAction(WinX + 50, WinY + 289, 86, 24))
					bTexture = 0;
				if (MouseAction(WinX + 50, WinY + 313, 86, 24))
					bTexture = 1;
				if (MouseAction(WinX + 50, WinY + 337, 86, 24))
					bTexture = 2;
				if (MouseAction(WinX + 50, WinY + 361, 87, 26))
					bTexture = 3;
			}
			else
				if (MouseAction(WinX + 50, WinY + 263, 86, 26))
					TextureBox = true;

		}
		else if (Tab == 1)
		{
			if (MouseAction(WinX + 50, WinY + 107, 15, 15))
			{
				if (!bMusic)
					bMusic = true;
				else
					bMusic = false;
			}

			if (MouseAction(WinX + 50, WinY + 217, 15, 15))
			{
				if (!bSound)
					bSound = true;
				else
					bSound = false;
			}

			if (MouseAction(WinX + 155, WinY + 217, 15, 15))
			{
				if (!bAmbient)
					bAmbient = true;
				else
					bAmbient = false;
			}

		}
		else if (Tab == 2)
		{
			if (ViewBox)
			{
				ViewBox = false;
				if (MouseAction(WinX + 50, WinY + 133, 86, 24))
					bCamView = 0;
				if (MouseAction(WinX + 50, WinY + 157, 86, 24))
					bCamView = 1;
				if (MouseAction(WinX + 50, WinY + 181, 86, 25))
					bCamView = 2;
			}
			else if (MouseAction(WinX + 50, WinY + 107, 86, 26))
				ViewBox = true;

			if (ZoomBox)
			{
				ZoomBox = false;
				if (MouseAction(WinX + 260, WinY + 133, 86, 24))
					bCamRange = 0;
				if (MouseAction(WinX + 260, WinY + 157, 86, 24))
					bCamRange = 1;
				if (MouseAction(WinX + 260, WinY + 181, 86, 24))
					bCamRange = 2;
				if (MouseAction(WinX + 260, WinY + 205, 86, 25))
					bCamRange = 3;
			}
			else if (MouseAction(WinX + 260, WinY + 107, 86, 26))
				ZoomBox = true;

			if (MouseAction(WinX + 365, WinY + 152, 15, 15))
				bHidePlayerNames == true ? bHidePlayerNames = false : bHidePlayerNames = true;
			if (MouseAction(WinX + 365, WinY + 174, 15, 15))
				bShowLife == true ? bShowLife = false : bShowLife = true;
			if (MouseAction(WinX + 365, WinY + 196, 15, 15))
				bShowNotice == true ? bShowNotice = false : bShowNotice = true;
			if (MouseAction(WinX + 155, WinY + 107, 15, 15))
				bCamShake = true;
			if (MouseAction(WinX + 155, WinY + 129, 15, 15))
				bCamShake = false;
			if (MouseAction(WinX + 365, WinY + 107, 15, 15))
				bCamInv = true;
			if (MouseAction(WinX + 365, WinY + 129, 15, 15))
				bCamInv = false;

			if (MouseAction(WinX + 155, WinY + 315, 15, 15))
				if (!bFilter)
					bFilter = true;
				else
					bFilter = false;

			if (MouseAction(WinX + 50, WinY + 235, 15, 15))
				if (!bHP)
					bHP = true;
				else
					bHP = false;

			if (MouseAction(WinX + 50, WinY + 255, 15, 15))
				if (!bMP)
					bMP = true;
				else
					bMP = false;

			if (MouseAction(WinX + 50, WinY + 275, 15, 15))
				if (!bSP)
					bSP = true;
				else
					bSP = false;

			if (MouseAction(WinX + 50, WinY + 295, 15, 15))
				if (!bGold)
					bGold = true;
				else
					bGold = false;

			if (MouseAction(WinX + 50, WinY + 315, 15, 15))
				if (!bAmulets)
					bAmulets = true;
				else
					bAmulets = false;


			if (MouseAction(WinX + 50, WinY + 335, 15, 15))
				if (!bRings)
					bRings = true;
				else
					bRings = false;

			if (MouseAction(WinX + 50, WinY + 355, 15, 15))
				if (!bSheltoms)
					bSheltoms = true;
				else
					bSheltoms = false;

			if (MouseAction(WinX + 50, WinY + 375, 15, 15))
				if (!bForce)
					bForce = true;
				else
					bForce = false;

			if (MouseAction(WinX + 50, WinY + 395, 15, 15))
				if (!bPremiums)
					bPremiums = true;
				else
					bPremiums = false;

			if (MouseAction(WinX + 155, WinY + 235, 15, 15))
				if (!bCrystal)
					bCrystal = true;
				else
					bCrystal = false;

			if (MouseAction(WinX + 155, WinY + 255, 15, 15))
				if (!bDefItem)
					bDefItem = true;
				else
					bDefItem = false;

			if (MouseAction(WinX + 155, WinY + 275, 15, 15))
				if (!bOffItem)
					bOffItem = true;
				else
					bOffItem = false;

			if (MouseAction(WinX + 155, WinY + 295, 15, 15))
				if (!bElse)
					bElse = true;
				else
					bElse = false;

			if (MouseAction(WinX + 365, WinY + 295, 15, 15))
				if (!bFilterSpec)
					bFilterSpec = true;
				else
					bFilterSpec = false;

			if (MouseAction(WinX + 260, WinY + 235, 15, 15))
				if (!bMS)
					bMS = true;
				else
					bMS = false;

			if (MouseAction(WinX + 260, WinY + 255, 15, 15))
				if (!bFS)
					bFS = true;
				else
					bFS = false;


			if (MouseAction(WinX + 260, WinY + 275, 15, 15))
				if (!bPS)
					bPS = true;
				else
					bPS = false;

			if (MouseAction(WinX + 260, WinY + 295, 15, 15))
				if (!bAS)
					bAS = true;
				else
					bAS = false;

			if (MouseAction(WinX + 260, WinY + 315, 15, 15))
				if (!bKS)
					bKS = true;
				else
					bKS = false;

			if (MouseAction(WinX + 260, WinY + 335, 15, 15))
				if (!bATS)
					bATS = true;
				else
					bATS = false;

			if (MouseAction(WinX + 365, WinY + 235, 15, 15))
				if (!bPRS)
					bPRS = true;
				else
					bPRS = false;

			if (MouseAction(WinX + 365, WinY + 255, 15, 15))
				if (!bMGS)
					bMGS = true;
				else
					bMGS = false;

			if (MouseAction(WinX + 365, WinY + 275, 15, 15))
				if (!bNaked)
					bNaked = true;
				else
					bNaked = false;
		}

		if (MouseAction(WinX + 298, WinY + 386, 94, 24))
			Reset();
		if (MouseAction(WinX + 395, WinY + 386, 94, 24))
			Save();
		if (MouseAction(WinX + 513, WinY + 7, 33, 48))
			Close();
	}
}

void Settings::Load()
{
	IniFiles ini(".\\game.ini");

	if (ini.ReadBool("Screen", "Windowed"))
	{
		smConfig.WinMode = true;
		bWindowed = true;
		cWindowed = true;
	}
	else
	{
		smConfig.WinMode = false;
		bWindowed = false;
		cWindowed = false;
	}

	if (ini.ReadBool("Screen", "AutoAdjust"))
	{
		bAutoAdjust = true;
		cAutoAdjust = true;
	}
	else
	{
		bAutoAdjust = false;
		cAutoAdjust = false;
	}

	if (lstrcmp(ini.ReadString("Screen", "Ratio"), "4:3") == 0)
	{
		bRatio = 0;
		cRatio = 0;
	}

	if (lstrcmp(ini.ReadString("Screen", "Ratio"), "5:4") == 0)
	{
		bRatio = 1;
		cRatio = 1;
	}

	if (lstrcmp(ini.ReadString("Screen", "Ratio"), "16:9") == 0)
	{
		bRatio = 2;
		cRatio = 2;
	}

	if (lstrcmp(ini.ReadString("Screen", "Ratio"), "16:10") == 0)
	{
		bRatio = 3;
		cRatio = 3;
	}

	smConfig.ScreenSize.x = ini.ReadInt("Screen", "Width");
	smConfig.ScreenSize.y = ini.ReadInt("Screen", "Height");

	if (bRatio == 0)
	{
		if (smConfig.ScreenSize.x == 800 && smConfig.ScreenSize.x == 600)
		{
			bResolution = 0;
			cResolution = 0;
		}
		else
			if (smConfig.ScreenSize.x == 1024 && smConfig.ScreenSize.x == 768)
			{
				bResolution = 1;
				cResolution = 1;
			}
			else
				if (smConfig.ScreenSize.x == 1280 && smConfig.ScreenSize.x == 960)
				{
					bResolution = 2;
					cResolution = 2;
				}
				else
					if (smConfig.ScreenSize.x == 1400 && smConfig.ScreenSize.x == 1050)
					{
						bResolution = 3;
						cResolution = 3;
					}
	}
	else if (bRatio == 1)
	{
		if (smConfig.ScreenSize.x == 1280 && smConfig.ScreenSize.x == 1024)
		{
			bResolution = 4;
			cResolution = 4;
		}
	}
	else if (bRatio == 2)
	{
		if (smConfig.ScreenSize.x == 1280 && smConfig.ScreenSize.x == 720)
		{
			bResolution = 5;
			cResolution = 5;
		}
		else
			if (smConfig.ScreenSize.x == 1366 && smConfig.ScreenSize.x == 768)
			{
				bResolution = 6;
				cResolution = 6;
			}
			else
				if (smConfig.ScreenSize.x == 1600 && smConfig.ScreenSize.x == 900)
				{
					bResolution = 7;
					cResolution = 7;
				}
				else
					if (smConfig.ScreenSize.x == 1920 && smConfig.ScreenSize.x == 1080)
					{
						bResolution = 8;
						cResolution = 8;
					}
	}
	else if (bRatio == 3)
	{
		if (smConfig.ScreenSize.x == 1280 && smConfig.ScreenSize.x == 800)
		{
			bResolution = 9;
			cResolution = 9;
		}
		else
			if (smConfig.ScreenSize.x == 1440 && smConfig.ScreenSize.x == 900)
			{
				bResolution = 10;
				cResolution = 10;
			}
			else
				if (smConfig.ScreenSize.x == 1680 && smConfig.ScreenSize.x == 1050)
				{
					bResolution = 11;
					cResolution = 11;
				}
				else
					if (smConfig.ScreenSize.x == 1920 && smConfig.ScreenSize.x == 1200)
					{
						bResolution = 12;
						cResolution = 12;
					}
	}

	bTexture = ini.ReadInt("Graphics", "TextureQuality");
	cTexture = bTexture;
	smConfig.TextureQuality = bTexture;
	bBPP = ini.ReadInt("Graphics", "BitDepth");
	cBPP = bBPP;
	smConfig.ScreenColorBit = bBPP;
	if (ini.ReadBool("Graphics", "Damage"))
	{
		smConfig.showDamage = true;
		smConfig.showDamage = true;
	}
	else
	{
		smConfig.showDamage = false;
		smConfig.showDamage = false;
	}

	if (ini.ReadBool("Graphics", "Effects"))
	{
		bEffects = true;
		cEffects = true;
	}
	else
	{
		bEffects = false;
		cEffects = false;
	}

	if (ini.ReadBool("Graphics", "DynamicLights"))
	{
		bLights = true;
		cLights = true;
	}
	else
	{
		bLights = false;
		cLights = false;
	}


	if (ini.ReadBool("Graphics", "DynamicShadows"))
	{
		bShadows = true;
		cShadows = true;
	}
	else
	{
		bShadows = false;
		cShadows = false;
	}

	if (ini.ReadBool("Graphics", "VSync"))
	{
		vSync = true;
		cvSync = true;
	}
	else
	{
		vSync = false;
		cvSync = false;
	}

	if (ini.ReadBool("Graphics", "BlockUI"))
	{
		vBlockUI = true;
		cvBlockUI = true;
	}
	else
	{
		vBlockUI = false;
		cvBlockUI = false;
	}

	if (ini.ReadBool("Audio", "Music"))
	{
		bMusic = true;
		cMusic = true;
	}
	else
	{
		bMusic = false;
		cMusic = false;
	}


	if (ini.ReadBool("Audio", "Sound"))
	{
		bSound = true;
		cSound = true;
	}
	else
	{
		bSound = false;
		cSound = false;
	}


	if (ini.ReadBool("Audio", "Ambient"))
	{
		bAmbient = true;
		cAmbient = true;
	}
	else
	{
		bAmbient = false;
		cAmbient = false;
	}

	if (ini.ReadBool("Camera", "FarCameraSight"))
	{
		bCamShake = true;
		cCamShake = true;
		CameraSight = true;
	}
	else
	{
		bCamShake = false;
		cCamShake = false;
		CameraSight = false;
	}

	if (ini.ReadBool("Camera", "InvertedCamera"))
	{
		bCamInv = true;
		cCamInv = true;
		CameraInvRot = true;
	}
	else
	{
		bCamInv = false;
		cCamInv = false;
		CameraInvRot = false;
	}


	if (ini.ReadBool("LootFilter", "HP"))
	{
		bHP = true;
		cHP = true;
	}
	else
	{
		bHP = false;
		cHP = false;
	}

	if (ini.ReadBool("LootFilter", "MP"))
	{
		bMP = true;
		cMP = true;
	}
	else
	{
		bMP = false;
		cMP = false;
	}

	if (ini.ReadBool("LootFilter", "SP"))
	{
		bSP = true;
		cSP = true;
	}
	else
	{
		bSP = false;
		cSP = false;
	}

	if (ini.ReadBool("LootFilter", "Gold"))
	{
		bGold = true;
		cGold = true;
	}
	else
	{
		bGold = false;
		cGold = false;
	}

	if (ini.ReadBool("LootFilter", "Amulets"))
	{
		bAmulets = true;
		cAmulets = true;
	}
	else
	{
		bAmulets = false;
		cAmulets = false;
	}


	if (ini.ReadBool("LootFilter", "Rings"))
	{
		bRings = true;
		cRings = true;
	}
	else
	{
		bRings = false;
		cRings = false;
	}


	if (ini.ReadBool("LootFilter", "Sheltoms"))
	{
		bSheltoms = true;
		cSheltoms = true;
	}
	else
	{
		bSheltoms = false;
		cSheltoms = false;
	}


	if (ini.ReadBool("LootFilter", "Force"))
	{
		bForce = true;
		cForce = true;
	}
	else
	{
		bForce = false;
		cForce = false;
	}

	if (ini.ReadBool("LootFilter", "Premiums"))
	{
		bPremiums = true;
		cPremiums = true;
	}
	else
	{
		bPremiums = false;
		cPremiums = false;
	}

	if (ini.ReadBool("LootFilter", "Crystal"))
	{
		bCrystal = true;
		cCrystal = true;
	}
	else
	{
		bCrystal = false;
		cCrystal = false;
	}

	if (ini.ReadBool("LootFilter", "DefItem"))
	{
		bDefItem = true;
		cDefItem = true;
	}
	else
	{
		bDefItem = false;
		cDefItem = false;
	}

	if (ini.ReadBool("LootFilter", "Else"))
	{
		bOffItem = true;
		cOffItem = true;
	}
	else
	{
		bOffItem = false;
		cOffItem = false;
	}

	if (ini.ReadBool("LootFilter", "Else"))
	{
		bElse = true;
		cElse = true;
	}
	else
	{
		bElse = false;
		cElse = false;
	}
	
	if (ini.ReadBool("LootFilter", "Enabled"))
	{
		bFilter = true;
		cFilter = true;
	}
	else
	{
		bFilter = false;
		cFilter = false;
	}



	if (ini.ReadBool("LootSpecFilter", "MS"))
	{
		bMS = true;
		cMS = true;
	}
	else
	{
		bMS = false;
		cMS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "FS"))
	{
		bFS = true;
		cFS = true;
	}
	else
	{
		bFS = false;
		cFS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "PS"))
	{
		bPS = true;
		cPS = true;
	}
	else
	{
		bPS = false;
		cPS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "AS"))
	{
		bAS = true;
		cAS = true;
	}
	else
	{
		bAS = false;
		cAS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "KS"))
	{
		bKS = true;
		cKS = true;
	}
	else
	{
		bKS = false;
		cKS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "ATS"))
	{
		bATS = true;
		cATS = true;
	}
	else
	{
		bATS = false;
		cATS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "PRS"))
	{
		bPRS = true;
		cPRS = true;
	}
	else
	{
		bPRS = false;
		cPRS = false;
	}

	if (ini.ReadBool("LootSpecFilter", "MGS"))
	{
		bMGS = true;
		cMGS = true;
	}
	else
	{
		bMGS = false;
		cMGS = false;
	}


	if (ini.ReadBool("LootSpecFilter", "Naked"))
	{
		bNaked = true;
		cNaked = true;
	}
	else
	{
		bNaked = false;
		cNaked = false;
	}

	if (ini.ReadBool("LootSpecFilter", "Enabled"))
	{
		bFilterSpec = false;//true;
		cFilterSpec = false; //true;
	}
	else
	{
		bFilterSpec = false;
		cFilterSpec = false;
	}

	if (ini.ReadBool("Game", "RememberAccount"))
	{
		bRememberLogin = true;
	}
	else
	{
		bRememberLogin = false;
	}

	if (ini.ReadBool("Game", "ShowNames"))
	{
		bHidePlayerNames = true;
	}
	else
	{
		bHidePlayerNames = false;
	}

	if (ini.ReadBool("Game", "ShowLife"))
	{
		bShowLife = true;
	}
	else
	{
		bShowLife = false;
	}

	if (ini.ReadBool("Game", "ShowAlert"))
	{
		bShowNotice = true;
	}
	else
	{
		bShowNotice = false;
	}	

	bMVol = ini.ReadInt("Audio", "MusicVolume");
	cMVol = bMVol;
	bSVol = ini.ReadInt("Audio", "SoundVolume");
	cSVol = bSVol;
	bCamView = ini.ReadInt("Camera", "View");
	cCamView = bCamView;
	bCamRange = ini.ReadInt("Camera", "Range");
	cCamRange = bCamRange;

	extern int ConfigUseDynamicLights;
	extern int ConfigUseDynamicShadows;

	ConfigUseDynamicLights = bLights;
	ConfigUseDynamicShadows = bShadows;

	// Usa o valor já lido do game.ini em vez de valor fixo
	if (smConfig.szServerIP[0])
		lstrcpy(smConfig.szDataServerIP, smConfig.szServerIP);
	else
		lstrcpy(smConfig.szDataServerIP, "127.0.0.1"); // Fallback

}

void Settings::Save(bool resize)
{
	cWindowed = bWindowed;
	cAutoAdjust = bAutoAdjust;
	cRatio = bRatio;
	cResolution = bResolution;
	cTexture = bTexture;
	cBPP = bBPP;
	cEffects = bEffects;
	smConfig.showDamage = smConfig.showDamage;
	cLights = bLights;
	cShadows = bShadows;
	cvSync = vSync;
	cvBlockUI = vBlockUI;
	cMusic = bMusic;
	cMVol = bMVol;
	cSound = bSound;
	cAmbient = bAmbient;
	cSVol = bSVol;
	cCamView = bCamView;
	cCamRange = bCamRange;
	cCamShake = bCamShake;
	cCamInv = bCamInv;
	cFilter = bFilter;
	cFilterSpec = bFilterSpec;
	cHP = bHP;
	cMP = bMP;
	cSP = bSP;
	cGold = bGold;
	cAmulets = bAmulets;
	cRings = bRings;
	cSheltoms = bSheltoms;
	cForce = bForce;
	cPremiums = bPremiums;
	cCrystal = bCrystal;
	cDefItem = bDefItem;
	cOffItem = bOffItem;
	cElse = bElse;
	cMS = bMS;
	cFS = bFS;
	cPS = bPS;
	cAS = bAS;
	cKS = bKS;
	cATS = bATS;
	cPRS = bPRS;
	cMGS = bMGS;
	cNaked = bNaked;

	IniFiles ini(".\\game.ini");
	ini.WriteBool("Screen", "Windowed", cWindowed);
	ini.WriteBool("Screen", "AutoAdjust", cAutoAdjust);
	ini.WriteBool("Audio", "Music", bMusic);
	ini.WriteBool("Audio", "Sound", bSound);
	ini.WriteBool("Graphics", "Effects", cEffects);
	ini.WriteBool("Graphics", "Damage", smConfig.showDamage);
	ini.WriteBool("Graphics", "DynamicLights", cLights);
	ini.WriteBool("Graphics", "DynamicShadows", cShadows);
	ini.WriteBool("Graphics", "VSync", cvSync);
	ini.WriteBool("Graphics", "BlockUI", cvBlockUI);
	ini.WriteBool("Camera", "FarCameraSight", cCamShake);
	ini.WriteBool("Camera", "InvertedCamera", cCamInv);
	ini.WriteBool("LootFilter", "Enabled", cFilter);
	ini.WriteBool("LootFilter", "HP", cHP);
	ini.WriteBool("LootFilter", "MP", cMP);
	ini.WriteBool("LootFilter", "SP", cSP);
	ini.WriteBool("LootFilter", "Gold", cGold);
	ini.WriteBool("LootFilter", "Amulets", cAmulets);
	ini.WriteBool("LootFilter", "Rings", cRings);
	ini.WriteBool("LootFilter", "Sheltoms", cSheltoms);
	ini.WriteBool("LootFilter", "Force", cForce);
	ini.WriteBool("LootFilter", "Premiums", cPremiums);
	ini.WriteBool("LootFilter", "Crystal", cCrystal);
	ini.WriteBool("LootFilter", "DefItem", cDefItem);
	ini.WriteBool("LootFilter", "OffItem", cOffItem);
	ini.WriteBool("LootFilter", "Else", cElse);

	ini.WriteBool("LootSpecFilter", "Enabled", cFilterSpec);
	ini.WriteBool("LootSpecFilter", "MS", cMS);
	ini.WriteBool("LootSpecFilter", "FS", cFS);
	ini.WriteBool("LootSpecFilter", "PS", cPS);
	ini.WriteBool("LootSpecFilter", "AS", cAS);
	ini.WriteBool("LootSpecFilter", "KS", cKS);
	ini.WriteBool("LootSpecFilter", "ATS", cATS);
	ini.WriteBool("LootSpecFilter", "PRS", cPRS);
	ini.WriteBool("LootSpecFilter", "MGS", cMGS);
	ini.WriteBool("LootSpecFilter", "Naked", cNaked);

	ini.WriteBool("Game", "RememberAccount", bRememberLogin);
	ini.WriteBool("Game", "ShowNames", bHidePlayerNames);
	ini.WriteBool("Game", "ShowLife", bShowLife);
	ini.WriteBool("Game", "ShowAlert", bShowNotice);
	
	if (resize)
	{
		if (cRatio == 0)
		{
			if (cResolution == 0)
			{
				smConfig.ScreenSize.x = 800;
				smConfig.ScreenSize.y = 600;
			}
			else if (cResolution == 1)
			{
				smConfig.ScreenSize.x = 1024;
				smConfig.ScreenSize.y = 768;
			}
			else if (cResolution == 2)
			{
				smConfig.ScreenSize.x = 1280;
				smConfig.ScreenSize.y = 960;
			}
			else if (cResolution == 3)
			{
				smConfig.ScreenSize.x = 1400;
				smConfig.ScreenSize.y = 1050;
			}
		}
		else if (cRatio == 1)
		{
			if (cResolution == 4)
			{
				smConfig.ScreenSize.x = 1280;
				smConfig.ScreenSize.y = 1024;
			}
		}
		else if (cRatio == 2)
		{
			if (cResolution == 5)
			{
				smConfig.ScreenSize.x = 1280;
				smConfig.ScreenSize.y = 720;
			}
			else if (cResolution == 6)
			{
				smConfig.ScreenSize.x = 1366;
				smConfig.ScreenSize.y = 768;
			}
			else if (cResolution == 7)
			{
				smConfig.ScreenSize.x = 1600;
				smConfig.ScreenSize.y = 900;
			}
			else if (cResolution == 8)
			{
				smConfig.ScreenSize.x = 1920;
				smConfig.ScreenSize.y = 1080;
			}
		}
		else if (cRatio == 3)
		{
			if (cResolution == 9)
			{
				smConfig.ScreenSize.x = 1280;
				smConfig.ScreenSize.y = 800;
			}
			else if (cResolution == 10)
			{
				smConfig.ScreenSize.x = 1440;
				smConfig.ScreenSize.y = 900;
			}
			else if (cResolution == 11)
			{
				smConfig.ScreenSize.x = 1680;
				smConfig.ScreenSize.y = 1050;
			}
			else if (cResolution == 12)
			{
				smConfig.ScreenSize.x = 1920;
				smConfig.ScreenSize.y = 1200;
			}
		}

		ini.WriteInt("Screen", "Width", smConfig.ScreenSize.x);
		ini.WriteInt("Screen", "Height", smConfig.ScreenSize.y);
		ini.WriteInt("Graphics", "TextureQuality", cTexture);
		ini.WriteInt("Graphics", "BitDepth", cBPP);
		ini.WriteInt("Audio", "MusicVolume", cMVol);
		ini.WriteInt("Audio", "SoundVolume", cSVol);
		ini.WriteInt("Camera", "View", cCamView);
		ini.WriteInt("Camera", "Range", cCamRange);
		Set(cWindowed, smConfig.ScreenSize.x, smConfig.ScreenSize.y);

		SetVolumeBGM(LastMusicVolume);

		if (!cMusic)
			StopBGM();

		GAMECOREHANDLE->SetCanMove(cvBlockUI);
		CHUDCONTROLLER->UpdateObjectsSettings();
	}

}

bool Settings::setAutoAdjust(int iWidth, int iHeight)
{
	extern void resizeOpening();
	extern void resizeLogin();
	extern void resizeSin();
	extern void resizePartyUI();

	smScreenWidth = iWidth;
	smScreenHeight = iHeight;

	//Atualiza as variáveis.
	MidX = iWidth / 2;
	MidY = iHeight / 2;
	//MidY -= 59;

	//Obtêm o endereço das variáveis.
	extern float g_fWinSizeRatio_X;
	extern float g_fWinSizeRatio_Y;

	g_fWinSizeRatio_X = float(iWidth) / 800.f;

	extern int WinSizeX;
	extern int WinSizeY;

	WinSizeX = iWidth;
	WinSizeY = iHeight;

	g_fWinSizeRatio_Y = float(iHeight) / 600.f;

	SetDxProjection((g_PI / 4.4f), iWidth, iHeight, 20.f, 4000.f);

	resizeOpening();
	resizeLogin();
	resizeSin();
	resizePartyUI();

	viewdistZ = (iHeight * 4) / 3;
	smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT;


	if (GRAPHICDEVICE)
	{
		SetDisplayMode(hwnd, smScreenWidth, smScreenHeight, bBPP);
	}

	return true;
}

void Settings::Set(int Windowed, int iWidth, int iHeight)
{
	extern int ConfigUseDynamicLights;
	extern int ConfigUseDynamicShadows;

	ConfigUseDynamicLights = bLights;
	ConfigUseDynamicShadows = bShadows;

	extern float g_fWinSizeRatio_X;
	extern float g_fWinSizeRatio_Y;

	WinSizeX = iWidth;
	WinSizeY = iHeight;

	smScreenWidth = iWidth;
	smScreenHeight = iHeight;

	smConfig.ScreenSize.x = WinSizeX;
	smConfig.ScreenSize.y = WinSizeY;

	MidX = WinSizeX / 2;
	MidY = WinSizeY / 2;

	g_fWinSizeRatio_X = float(WinSizeX) / 800.f;
	g_fWinSizeRatio_Y = float(WinSizeY) / 600.f;

	viewdistZ = ((WinSizeY / 3) * 4);

	SetDxProjection((g_PI / 4.4f), WinSizeX, WinSizeY, 20.f, 4000.f);

	if (Windowed)
	{
		RECT rc = { 0, 0, iWidth, iHeight };

		UINT uWindowStyle = WS_CAPTION | WS_SYSMENU | WS_VISIBLE | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
		AdjustWindowRect(&rc, uWindowStyle, FALSE);

		int w = rc.right - rc.left;
		int h = rc.bottom - rc.top;

		SetWindowLongA(hwnd, GWL_STYLE, uWindowStyle);
		SetWindowPos(hwnd,
			NULL,
			((GetSystemMetrics(SM_CXSCREEN) >> 1) - (w >> 1)),
			((GetSystemMetrics(SM_CYSCREEN) >> 1) - (h >> 1)),
			w,
			h,
			SWP_NOZORDER);

		UpdateWindow(hwnd);
	}
	else
	{
		SetWindowPos(hwnd,
			NULL,
			0,
			0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			SWP_DRAWFRAME | SWP_NOZORDER);

		SetWindowLongA(hwnd, GWL_STYLE, WS_POPUP | WS_VISIBLE);
	}

	if (GRAPHICDEVICE && GameMode == 2)
	{
		extern void resizeOpening();
		extern void resizeLogin();

		resizeOpening();
		resizeLogin();

		GAMECOREHANDLE->OnResolutionChanged();
	}
}

void Settings::Reset()
{
	bWindowed = true;
	bAutoAdjust = true;
	bRatio = 0;
	bResolution = 0;
	bTexture = 0;
	bBPP = 32;
	bEffects = true;
	smConfig.showDamage = false;
	bLights = false;
	bShadows = true;
	vSync = true;
	vBlockUI = true;
	bMusic = false;
	bMVol = 0;
	bSound = false;
	bAmbient = false;
	bSVol = 0;
	bCamView = 2;
	bCamRange = 3;
	bCamShake = true;
	bCamInv = false;
	bFilter = true;
	bFilterSpec = true;
	bHP = true;
	bMP = true;
	bSP = true;
	bGold = true;
	bAmulets = true;
	bRings = true;
	bSheltoms = true;
	bForce = true;
	bPremiums = true;
	bCrystal = true;
	bDefItem = true;
	bOffItem = true;
	bElse = true;
	bMS = true;
	bFS = true;
	bPS = true;
	bAS = true;
	bKS = true;
	bATS = true;
	bPRS = true;
	bMGS = true;
	bNaked = true;
	bRememberLogin = true;
	bHidePlayerNames = false;
	bShowLife = false;
	bShowNotice = true;

	GAMECOREHANDLE->OnResolutionChanged();
}
