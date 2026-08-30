#include "..\globals.h"
#include "..\\sinbaram\\SinLinkHeader.h"
#include "..\\HoBaram\\HoLinkHeader.h"
#include "..\\smreg.h"
#include "..\\HoBaram\\HoTextFile.h"
#include "..\\HoBaram\\HoMinMax.h"
#include "..\\HoBaram\\HoNewParticle.h"
#include "..\\HoBaram\\HoNewParticleMgr.h"
#include "Login\LoginScreen.h"
#include <ctime>
#include <iostream>

extern int			iSettingsBg;
extern int			iSettingsButton[2];
extern int			iSettingsRelog;
extern int			iSettingsSave;
extern int			iSettingsClose;
extern int			iSettingsCheckBox[2];

#include "..\\CAutoCamera.h"
#include <TitleBox/TitleBox.h>
#include <Login/LoginModel.h>

static CAutoCamera		s_AutoCamera;

extern POINT3D WaveCameraPosi;
extern int	   WaveCameraFactor;
extern int	   WaveCameraTimeCount;
extern int	   WaveCameraDelay;
extern BOOL	   WaveCameraFlag;

char	UserAccount[256];
char	UserPassword[256];

int			WorkState;
int			CurrentWorkState;
int			NextWorkState;

HoOpening::HoOpening()
{

	LOGINSCREENHANDLER->pcStage = NULL;
	PatCamera = NULL;

	LogoBlendValue = 0;
	LoginBlendValue = 0;

	Load();
	StartConnectServerFlag = FALSE;
	WorkState = STATE_OPENING;

	MidY = (600 / 2);
	SetWindowText(hTextWnd, NULL);
	memset(UserAccount, 0, sizeof(UserAccount));
	memset(UserPassword, 0, sizeof(UserPassword));

	TarDist = 1600 * fONE;
	CaAngle.x = 220;
	CaAngle.y = 0;
	CaAngle.z = 0;

	MouseDownServerIndex = -1;
	CameraFrame = 0;
}

extern void LeIniStr(char* Section, char* Key, char* szFileIni, char* Var1);
extern int LeIniInt(char* Section, char* Key, char* szFileIni);
extern void WriteIniInt(char* Section, char* Key, char* szFileIni, DWORD Value);
extern void WriteIniStr(char* Section, char* Key, char* szFileIni, char* Value);

HoOpening::~HoOpening()
{
	Close();
}

int HoOpening::Init()
{
	LOGINSCREENHANDLER->Init();

	GameErrorValue = -1;
	ConnectErrorCount = 0;
	FocusWndFlag = FALSE;
	SetWindowText(hTextWnd, "");
	SendMessage(hTextWnd, EM_SETLIMITTEXT, 31, 0);

	LoginInputState = 0;
	ConnectError = ERROR_NONE;
	ServerWaitTimeCount = 0;
	StartQuitFlag = FALSE;

	WorkState = STATE_OPENING;
	StartConnectServerFlag = FALSE;
	ConnectServerSucessFlag = FALSE;

	FadeCurrentState = FADE_OUT;
	FadeNextState = FADE_OUT;


	LogoBlendValue = 0;
	LoginBlendValue = 0;
	ServerSelectBlendValue = 0;

	MidX = WinSizeX / 2;
	MidY = WinSizeY / 2;
	smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_WIDE_MULT_PERSPECTIVE_HEIGHT;

	SetWindowText(hTextWnd, NULL);

	memset(UserAccount, 0, sizeof(UserAccount));
	memset(UserPassword, 0, sizeof(UserPassword));

	TarDist = 1400 * fONE;

	CaAngle.x = 0;
	CaAngle.y = 0;
	CaAngle.z = 0;

	SetFade();

	smInitColorZClip(0, 0, 0);

	smRender.Color_R = 40;
	smRender.Color_G = 0;
	smRender.Color_B = -30;
	smRender.Color_A = 0;
	smRender.ClearLight();

	StopBGM();
	OpenBGM("StartImage\\Opening\\Login_CHR2.wav");
	PlayBGM();
	SetVolumeBGM(OpeningBgmVolume);

	char buf[32] = { 0 };

	if (Settings::GetInstance()->bRememberLogin)
	{
		LeIniStr("Game", "LastAccount", "game.ini", buf);

		if (buf != NULL)
		{
			memset(UserAccount, 0, sizeof(UserAccount));
			memcpy(UserAccount, buf, strlen(buf));
			LoginInputState = 1;
		}
	}


	CurrentWorkState = STATE_OPENING;
	NextWorkState = STATE_OPENING;
	MouseDownServerIndex = -1;
	MouseDownFlag = FALSE;


	CameraFrame = 160;
	ChangeSky(2);



	g_NewParticleMgr.Init();
	InitAssaEffect();

	InitCinemaFadeEffect();

	smIdentityMatrix(m_CameraRotInvert);

	m_IsDrawOpening = TRUE;
	m_AutoCameraFrameTime = 50;
	m_ParticleSpawnTimeCount = 0;

	s_AutoCamera.m_Frame = 0;

	m_AddWidth = (smScreenWidth - 800) / 2;
	m_AddHeight = (smScreenHeight - 600) / 2;

	startLogoX = 100 + m_AddWidth;
	startLogoY = 63 + m_AddHeight;

	startLogoAddX = 32;
	startAccountAddX = 230;
	startAccountAddY = 390;

	POINT3D p;
	p.x = 30.0f * 256.0f;
	p.y = 350.0f * 370.0f;
	p.z = -400.0f * 256.0f;

	g_NewParticleMgr.Start("Opening", p);

	return TRUE;
}

extern void ReadTexSettings();


void HoOpening::SetCameraCharPos(void)
{
	int x = 0;
	int y = 0;
	int z = (440 << FLOATNS);

	smEMATRIX eCameraRotInvert;
	smEMatrixInvert(eCameraRotInvert, s_AutoCamera.m_eCameraRotMat);

	memcpy((smEMATRIX*)&m_CameraRotInvert, &eCameraRotInvert, sizeof(smEMATRIX));
	smMatrixShift(m_CameraRotInvert, 4);

	int rx = x * m_CameraRotInvert._11 +
		y * m_CameraRotInvert._21 +
		z * m_CameraRotInvert._31;

	int ry = x * m_CameraRotInvert._12 +
		y * m_CameraRotInvert._22 +
		z * m_CameraRotInvert._32;

	int rz = x * m_CameraRotInvert._13 +
		y * m_CameraRotInvert._23 +
		z * m_CameraRotInvert._33;

	rx >>= FLOATS_EM;
	ry >>= FLOATS_EM;
	rz >>= FLOATS_EM;

	lpCurPlayer->pX = int(s_AutoCamera.m_fCameraPos_x) + rx;
	lpCurPlayer->pY = int(s_AutoCamera.m_fCameraPos_y) + ry;
	lpCurPlayer->pZ = int(s_AutoCamera.m_fCameraPos_z) + rz;
}

void HoOpening::SetParticleEffectSpawn(void)
{

}

int HoOpening::DebugModeLoadFieldInfo(void)
{
	FILE* fp = fopen("StartImage\\Opening\\OpeningField.txt", "rb");
	if (fp == NULL)
		return FALSE;

	LOGINSCREENHANDLER->pcStage = NULL;

	char* p, line[256], decode[256], strBuff[256];
	while (!feof(fp))
	{
		if (fgets(line, 255, fp) == NULL)
		{
			fclose(fp);
			break;
		}

		p = GetWord(decode, line);

		if (lstrcmp(decode, "*ÇÊµå¸í") == 0)
		{
			p = GetWord(strBuff, p);
			LOGINSCREENHANDLER->pcStage = smSTAGE3D_ReadASE(strBuff);
			if (LOGINSCREENHANDLER->pcStage)
				LOGINSCREENHANDLER->pcStage->Head = TRUE;
		}
		else if (lstrcmp(decode, "*Ä«¸Þ¶ó¸í") == 0)
		{
			p = GetWord(strBuff, p);
			s_AutoCamera.ReadASE_AutoCamera(strBuff);
		}
		else if (lstrcmp(decode, "*Ä«¸Þ¶ó¼Óµµ") == 0)
		{
			p = GetWord(strBuff, p);
			m_AutoCameraFrameTime = atoi(strBuff);
		}
		else if (lstrcmp(decode, "*¿öÇÁÇÁ·¹ÀÓ") == 0)
		{
			p = GetWord(strBuff, p);
			int StFrame = atoi(strBuff);

			p = GetWord(strBuff, p);
			int EndFrame = atoi(strBuff);

			s_AutoCamera.SetWarpFrame(StFrame, EndFrame);
		}
		else if (lstrcmp(decode, "*ÁöÇü¹°Ã¼") == 0)
		{
			p = GetWord(strBuff, p);
			LOGINSCREENHANDLER->pcStage->StageObject->AddObjectFile(strBuff);
		}
		else if (lstrcmp(decode, "*ÁöÇü¾Ö´Ï") == 0)
		{
			p = GetWord(strBuff, p);
			LOGINSCREENHANDLER->pcStage->StageObject->AddObjectFile(strBuff, strBuff);
		}
	}

	fclose(fp);

	if (LOGINSCREENHANDLER->pcStage == NULL)
		return FALSE;

	return TRUE;
}

int GetClick(DWORD x, DWORD y, DWORD L, DWORD A)
{
	DWORD XMouse = pCursorPos.x;
	DWORD YMouse = pCursorPos.y;
	int Resultado = 0;
	if (XMouse > x)
	{
		if (XMouse < (x + L - 1))
		{
			if (YMouse > y)
			{
				if (YMouse < (y + A - 1))
				{
					Resultado = 1;
				}
			}
		}
	}
	return Resultado;
}

extern BOOL bSettings;
char ratio_pt[4] = { 0 };
int Width, Height;

// Tela de login aqui
int HoOpening::Load()
{
	int IsLoad = FALSE;
	PatCamera = NULL;
	ObjCamera = NULL;

	if (smConfig.DebugMode)
		IsLoad = DebugModeLoadFieldInfo();

	if (IsLoad == FALSE)
	{
		//Camera Position
		s_AutoCamera.m_fCameraPos_x = 100.0f * 256.0f;
		s_AutoCamera.m_fCameraPos_y = 480.0f * 256.0f;
		s_AutoCamera.m_fCameraPos_z = -880.0f * 256.0f;

		//Camera Rotation
		s_AutoCamera.m_iCameraRot_x = 0;
		s_AutoCamera.m_iCameraRot_y = 0x0110;
		s_AutoCamera.m_iCameraRot_z = 0;

		s_AutoCamera.m_eCameraRotMat._11 = 0x7FC8;
		s_AutoCamera.m_eCameraRotMat._12 = 0xFFFFFFF4;
		s_AutoCamera.m_eCameraRotMat._13 = 0x07A4;
		s_AutoCamera.m_eCameraRotMat._14 = 0;
		s_AutoCamera.m_eCameraRotMat._21 = 0x03;
		s_AutoCamera.m_eCameraRotMat._22 = 0x8000;
		s_AutoCamera.m_eCameraRotMat._23 = 0;
		s_AutoCamera.m_eCameraRotMat._24 = 0;
		s_AutoCamera.m_eCameraRotMat._31 = 0x0D00;
		s_AutoCamera.m_eCameraRotMat._32 = 0;
		s_AutoCamera.m_eCameraRotMat._33 = 0x4030;
		s_AutoCamera.m_eCameraRotMat._34 = 0;
	}


	iWindowID = CreateTextureMaterial("game\\images\\login\\window.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	iBtnLogin[0] = CreateTextureMaterial("game\\images\\login\\btl.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	iBtnLogin[1] = CreateTextureMaterial("game\\images\\login\\btl_.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	iBtnExit[0] = CreateTextureMaterial("game\\images\\login\\bte.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	iBtnExit[1] = CreateTextureMaterial("game\\images\\login\\bte_.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	BlackBoxImage = CreateTextureMaterial("StartImage\\Login\\BlackBox.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);
	WhiteBoxImage = CreateTextureMaterial("StartImage\\Login\\WhiteBox.tga", 0, TRUE, FALSE, FALSE, SMMAT_BLEND_ALPHA);

	Message.Init();
	MessageBox.Init();

	ReadTexSettings();
	ReadTextures();

	if (LOGINSCREENHANDLER->pcStage)
	{
		LOGINSCREENHANDLER->pcStage->smMaterialGroup->CheckMatreialTextureSwap();
	}

	LeIniStr("Screen", "Ratio", "game.ini", ratio_pt);
	Width = LeIniInt("Screen", "Width", "game.ini");
	Height = LeIniInt("Screen", "Height", "game.ini");

	return TRUE;
}

BOOL bIsWidescreen = FALSE;

void HoOpening::resize()
{
	MidX = smScreenWidth / 2;
	MidY = smScreenHeight / 2;

	m_AddWidth = (smScreenWidth - 800) / 2;
	m_AddHeight = (smScreenHeight - 600) / 2;

	startLogoX = 100 + m_AddWidth;
	startLogoY = 63 + m_AddHeight;
}

void HoOpening::DrawCheckBox(HDC hdc, int x, int y, char* text, bool b)
{
	if (b)
		dsDrawTexImage(iSettingsCheckBox[1], x, y, 15, 15, 255);
	else
		dsDrawTexImage(iSettingsCheckBox[0], x, y, 15, 15, 255);

	HDC opHdc;
	//lpDDSBack->GetDC( &opHdc );

	//SetBkMode( opHdc, TRANSPARENT );
	SetFontTextColor(RGB(255, 255, 255));
	if (hFont != NULL)
	{
		HFONT oldFont;
		oldFont = (HFONT)SelectObject(opHdc, hFont);
		dsTextLineOut(opHdc, x + 20, y, text, strlen(text));

		if (oldFont)
			SelectObject(opHdc, oldFont);
	}
}

int		CaretCount = 0;
DWORD	End = 0;
extern int Client_Version;

#define	DISP_LOGIN_LEN		15

BOOL bDone = FALSE;
int iSave = 0;

extern BOOL bNoLag;
extern BOOL bAdjustWindow;

void SaveConfig()
{
	if (smConfig.WinMode)
		WriteIniStr("Screen", "Windowed", "game.ini", "True");
	else
		WriteIniStr("Screen", "Windowed", "game.ini", "False");

	if (smConfig.ScreenColorBit == 32)
		WriteIniInt("Graphics", "BitDepth", "game.ini", 32);
	else
		WriteIniInt("Graphics", "BitDepth", "game.ini", 16);

	if (smConfig.TextureQuality == 0)
		WriteIniStr("Graphics", "HighTextureQuality", "game.ini", "True");
	else
		WriteIniStr("Graphics", "HighTextureQuality", "game.ini", "False");

	if (CameraSight)
		WriteIniStr("Camera", "FarCameraSight", "game.ini", "True");
	else
		WriteIniStr("Camera", "FarCameraSight", "game.ini", "False");

	if (bIsWidescreen)
		WriteIniStr("Screen", "Ratio", "game.ini", "16:9");
	else
		WriteIniStr("Screen", "Ratio", "game.ini", "4:3");

	WriteIniInt("Screen", "Width", "game.ini", Width);
	WriteIniInt("Screen", "Height", "game.ini", Height);

	// auto resize window xxstr
	if (smConfig.WinMode)
	{
		::SetWindowPos(hwnd, 0, 0, 0, Width, Height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}


	if (CameraInvRot)
		WriteIniStr("Camera", "InvertedCamera", "game.ini", "On");
	else
		WriteIniStr("Camera", "InvertedCamera", "game.ini", "Off");

	/*if( smConfig.BGM_Mode )
		WriteIniStr( "Audio", "NoMusic", "game.ini", "True" );
	else
		WriteIniStr( "Audio", "NoMusic", "game.ini", "False" );*/

	if (bAdjustWindow)
		WriteIniStr("Screen", "AutoAdjust", "game.ini", "True");
	else
		WriteIniStr("Screen", "AutoAdjust", "game.ini", "False");

	if (bNoLag)
		WriteIniStr("Graphics", "NoLag", "game.ini", "True");
	else
		WriteIniStr("Graphics", "NoLag", "game.ini", "False");
}

extern int QuitGame();

BOOL bs = FALSE;
int iTime = 0;
extern bool bCaptureOpening;
extern DWORD dwTickCap;

void Relog()
{
	if (bs == FALSE)
	{
		STARTUPINFOA si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		CreateProcessA(NULL, "game.exe", NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

		bs = TRUE;
	}

	if (iTime >= 3)
		QuitGame();
	else
		iTime++;
}

int lastTimeRemember = 0;

int HoOpening::Draw()
{
	smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_WIDE_MULT_PERSPECTIVE_HEIGHT;

	m_AddWidth = (smConfig.ScreenSize.x - 800) / 2;
	m_AddHeight = (smConfig.ScreenSize.y - 600) / 2;

	startLogoX = 100 + m_AddWidth;
	startLogoY = 63 + m_AddHeight;

	startLogoAddX = 32;
	startAccountAddX = 230;
	startAccountAddY = 290;

	FastFade();

	int x, y, z;
	int anx, any;
	int anz;

	GRAPHICDEVICE->Clear(0, 0, 2, smBackColor, 1.0f, 0);

	// Login
	LOGINSCREENHANDLER->Render3D();

	if (!LOGINSCREENHANDLER->IsLoginVideo())
		LOGINHANDLER->RenderLoginBG();

	LOGINSCREENHANDLER->Render2D();

	x = int(s_AutoCamera.m_fCameraPos_x);
	y = int(s_AutoCamera.m_fCameraPos_y);
	z = int(s_AutoCamera.m_fCameraPos_z) + (WaveCameraFactor * 8);

	anx = s_AutoCamera.m_iCameraRot_x;
	any = s_AutoCamera.m_iCameraRot_y;
	anz = s_AutoCamera.m_iCameraRot_z;

	smRender.OpenCameraPosi(x, y, z, &s_AutoCamera.m_eCameraRotMat);

	//DrawCinemaFadeEffect();
	//g_NewParticleMgr.Draw(x, y, z, anx, any, anz);

	smRender.CloseCameraPosi();
	IncTextureFrame();

	if (NextWorkState == STATE_LOGIN_INPUT)
	{
		// Janela Única
		dsDrawTexImage(iWindowID, (smConfig.ScreenSize.x >> 1) - (528
			>> 1), (smConfig.ScreenSize.y >> 1) - (284 >> 1) + 7, 519, 210, LogoBlendValue);
	}

	if (NextWorkState == STATE_LOGIN_INPUT || CurrentWorkState == STATE_LOGIN_INPUT)
	{
		//Login Botão
		if ((pCursorPos.x > (smConfig.ScreenSize.x >> 1) - (-154 >> 1)) && (pCursorPos.x < (smConfig.ScreenSize.x >> 1) - (-156 >> 1) + 94) &&
			(pCursorPos.y > (smConfig.ScreenSize.y >> 1) + (13)) && (pCursorPos.y < (smConfig.ScreenSize.y >> 1) + (13 + 48)))
		{
			if (MouseButton[0])
			{
				CurrentWorkState = STATE_LOGIN_INPUT;
				LoginDataServer();
			}
			dsDrawTexImage(iBtnLogin[1], (smConfig.ScreenSize.x >> 1) - (-154 >> 1), (smConfig.ScreenSize.y >> 1) + 13, 94, 48, LogoBlendValue);
		}
		else
			dsDrawTexImage(iBtnLogin[0], (smConfig.ScreenSize.x >> 1) - (-154 >> 1), (smConfig.ScreenSize.y >> 1) + 13, 94, 48, LogoBlendValue);


		//Sair Botão
		if (GetClick(smConfig.ScreenSize.x - 97, smConfig.ScreenSize.y - 113, 76, 33))
		{
			dsDrawTexImage(iBtnExit[1], smConfig.ScreenSize.x - 97, smConfig.ScreenSize.y - 113, 76, 33, 255);
			if (MouseButton[0])
			{
				MouseDownFlag = TRUE;
				StartQuitFlag = TRUE;
				SetFade();
				PlayWav2(4 - 1);
			}
		}
		else
			dsDrawTexImage(iBtnExit[0], smConfig.ScreenSize.x - 97, smConfig.ScreenSize.y - 113, 76, 33, 255);
	}

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	HDC opHdc = NULL;
	SetFontTextColor(RGB(255, 255, 255));
	if (hFont != NULL)
	{
		HFONT oldFont;
		oldFont = (HFONT)SelectObject(opHdc, hFont);

		if (bCaptureOpening)
		{
			time_t t = time(0);   // get time now
			struct tm* now = localtime(&t);

			char localtime[128] = { 0 };
			strftime(localtime, sizeof(localtime), "Hora Local: %d-%m-%Y %X", now);

			dsTextLineOut(opHdc, (smConfig.ScreenSize.x / 2) - 40, 0, "Versão[0001] -", strlen("Versão[0001] -"));
			dsTextLineOut(opHdc, (smConfig.ScreenSize.x / 2) - 90, 20, localtime, strlen(localtime));

			if (GetTickCount() - dwTickCap >= 170)
			{
				dwTickCap = 0;
				bCaptureOpening = false;
			}
		}

		//wsprintf( buffer, "Versão do Game: 0001" );
		if (Client_Version < 1000) {
			float Version = (float)Client_Version / 100;
			/*sprintf(buffer, "%s%2.2f%s", "Versão: ", Version, "b");*/
		}
		else {
			int v1 = Client_Version / 1000;
			int v2 = (Client_Version / 10) % 100;
			int v3 = Client_Version % 10;
			/*wsprintf(buffer, "%s%d.%d.%d", "Versão: ", v1, v2, v3);*/
		}
		dsTextLineOut(opHdc, smConfig.ScreenSize.x - 100, smConfig.ScreenSize.y - 45, buffer, strlen(buffer));

		if (oldFont)
			SelectObject(opHdc, oldFont);
	}

	if (CurrentWorkState == STATE_LOGIN_INPUT && NextWorkState == STATE_LOGIN_INPUT && ScreenBlendValue <= 100)
	{
		if (LoginBlendValue >= 255)
		{
			if (FocusWndFlag == FALSE)
			{
				FocusWndFlag = TRUE;
				hFocusWnd = hTextWnd;
			}

			if (StartConnectServerFlag == FALSE && ConnectError == ERROR_NONE)
			{
				if (pCursorPos.x > (startLogoX + startAccountAddX + 10) &&
					pCursorPos.x < (startLogoX + startAccountAddX + 10 + 95) &&
					pCursorPos.y >(startLogoY + startAccountAddY + 20) &&
					pCursorPos.y < (startLogoY + startAccountAddY + 23 + 14) &&
					MouseButton[0])
				{
					if (LoginInputState == 1)
					{
						LoginInputState = 0;
						SetWindowText(hFocusWnd, UserAccount);
						SendMessage(hTextWnd, EM_SETSEL, lstrlen(UserAccount), lstrlen(UserAccount) + 1);
					}
				}

				if (pCursorPos.x > (startLogoX + startAccountAddX + 10) &&
					pCursorPos.x < (startLogoX + startAccountAddX + 10 + 95) &&
					pCursorPos.y >(startLogoY + startAccountAddY + 70) &&
					pCursorPos.y < (startLogoY + startAccountAddY + 73 + 14) &&
					MouseButton[0])
				{
					if (LoginInputState == 0)
					{
						SetIME_Mode(0);
						LoginInputState = 1;
						SetWindowText(hFocusWnd, UserPassword);
						SendMessage(hTextWnd, EM_SETSEL, lstrlen(UserPassword), lstrlen(UserPassword) + 1);
					}
				}
			}

			if (hFocusWnd && ConnectError == ERROR_NONE && StartConnectServerFlag == FALSE)
			{
				if (LoginInputState == 0)
				{
					GetWindowText(hFocusWnd, UserAccount, sizeof(UserAccount));
					KeyFilter(UserAccount);

				}
				else if (LoginInputState == 1)
				{
					GetWindowText(hFocusWnd, UserPassword, sizeof(UserPassword));
					KeyFilter(UserPassword);
				}
			}

			if (hFocusWnd)
				SendMessage(hFocusWnd, EM_GETSEL, NULL, (LPARAM)&End);

			HDC opHdc = NULL;
			HFONT oldFont;
			SetFontTextColor(RGB(255, 255, 255));
			if (hFont != NULL)
				oldFont = (HFONT)SelectObject(opHdc, hFont);

			if (hFocusWnd)
			{
				if (CaretCount < 16)
				{
					if (End > DISP_LOGIN_LEN) End = DISP_LOGIN_LEN;

					if (LoginInputState == 0)//Account
						DrawFontText(startLogoX + startAccountAddX - 102 + End * 7 + 1, startLogoY + startAccountAddY + -18, "|", RGB(255, 250, 250), 2);
					else if (LoginInputState == 1)//Password
						DrawFontText(startLogoX + startAccountAddX + 24 + End * 6 + 1, startLogoY + startAccountAddY + -18, "|", RGB(255, 250, 250), 2);
				}

				CaretCount++;
				if (CaretCount > 32)
					CaretCount = 0;
			}

			int sp, len;

			sp = 0;
			len = lstrlen(UserAccount);
			if (len > DISP_LOGIN_LEN) {
				sp = len - DISP_LOGIN_LEN;
				len -= sp;
			}
			DrawFontText(startLogoX + startAccountAddX - 102, startLogoY + startAccountAddY - 17, UserAccount + sp, RGB(255, 250, 250), 2);
			len = lstrlen(UserPassword);
			if (len > DISP_LOGIN_LEN) len = DISP_LOGIN_LEN;

			for (int count = 0; count < len; count++)
				DrawFontText(startLogoX + startAccountAddX + 24 + count * 6, startLogoY + startAccountAddY - 14, "*", RGB(255, 250, 250), 2);

			if (oldFont)
				SelectObject(opHdc, oldFont);
		}
	}

	if (StartConnectServerFlag && ConnectError == ERROR_NONE)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[0]);//"      ¼­¹ö¿¡ Á¢¼ÓÁß....    ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[0], 3);

	if (ConnectError == ERROR_CONNECT)
	{
		StartConnectServerFlag = FALSE;
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[1]);//"      ¼­¹ö Á¢¼Ó ºÒ°¡       ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[1], 3);
	}
	else if (ConnectError == ERROR_NO_ACCOUNT)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[2]);//"   °èÁ¤À» ´Ù½Ã ÀÔ·ÂÇÏ¼¼¿ä  ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[2], 3);
	else if (ConnectError == ERROR_NO_PASSWORD)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[3]);//"ºñ¹Ð ¹øÈ£¸¦ ´Ù½Ã ÀÔ·ÂÇÏ¼¼¿ä");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[3], 3);
	else if (ConnectError == ERROR_NO_BETA_TESTER)
		//DrawMessage(205 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[4]);//"  ÇØ´ç ¾ÆÀÌµð´Â ÇöÀç »ç¿ë±ÇÇÑÀÌ ¾øÀ¸´Ï °í°´¼¾ÅÍ·Î ¹®ÀÇ ¹Ù¶ø´Ï´Ù.  ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[4], 3);
	else if (ConnectError == ERROR_NO_ACCOUNT_IN)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[5]);//"   ¾ÆÀÌµð°¡ Á¢¼ÓÁßÀÔ´Ï´Ù   ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[5], 3);
	else if (ConnectError == ERROR_NO_VERSION)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[6]);//"  VersionÀÌ ¸ÂÁö ¾Ê½À´Ï´Ù  ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[6], 3);
	else if (ConnectError == ERROR_NO_LIMIT)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[7]);//"   ¼­¹ö¿¡ ÀÎ¿øÀÌ ¸¹½À´Ï´Ù  ");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[7], 3);
	else if (ConnectError == ERROR_NO_SERVER_CONNECT)
		//DrawMessage(275 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[8]);//"¼­¹ö¿ÍÀÇ ¿¬°áÀÌ ²÷¾î Á³½À´Ï´Ù");
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[8], 3);
	else if (ConnectError == ERROR_NO_USE_DATE_OVER)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[9]);//"  »ç¿ë±â°£ÀÌ Á¾·áµÇ¾ú½À´Ï´Ù "
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[9], 3);
	else if (ConnectError == ERROR_NO_USE_TIME_OVER)
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[10]);   //"  »ç¿ë½Ã°£ÀÌ Á¾·áµÇ¾ú½À´Ï´Ù "
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[10], 3);
	else if (ConnectError == ERROR_DELETE_ACCOUNT_NOTIFY) //" ¼­¹ö°èÁ¤ »èÁ¦ ¿äÃ»ÁßÀÔ´Ï´Ù "
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[11]);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[11], 3);
	else if (ConnectError == ERROR_SERVER_TRAFFIC_BUSY)   //¼­¹öÆøÁÖ!!! Àá½ÃÈÄ ÀÌ¿ë ¹Ù¶ø´Ï´Ù.
		//DrawMessage(275 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[12]);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[12], 3);
	else if (ConnectError == ERROR_SERVER_WAIT_MOMNENT)   //Àá½ÃÈÄ ÀÌ¿ëÇØ ÁÖ½Ê½Ã¿ä
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[13]);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[13], 3);
	else if (ConnectError == ERROR_THIRD_CONNECT)         //ºñ¹Ð¹øÈ£ 3È¸ ÀÌ»ó ¿À·ùÀÔ´Ï´Ù. È¨ÆäÀÌÁö ÂüÁ¶ ¹Ù¶ø´Ï´Ù.
		//DrawMessage(285 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[14], 30);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[14], 3);
	else if (ConnectError == ERROR_HAN_PASSWORD)          //Password¿¡ ÇÑ±Û¹®ÀÚ³ª »ç¿ëÇÒ¼ö ¾ø´Â ¹®ÀÚ°¡ ÀÖ½À´Ï´Ù.
		//DrawMessage(285 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[15], 30);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[15], 3);
	else if (ConnectError == ERROR_12YEARS)				//12¼¼ ÀÌÇÏ ÀÔÀå ºÒ°¡
		//DrawMessage(285 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[16], 30);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[16], 3);
	else if (ConnectError == ERROR_HOMEAGREE)			//12¼¼ ÀÌÇÏ ÀÔÀå ºÒ°¡
		//DrawMessage(285 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[17], 30);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[17], 3);
	else if (ConnectError == ERROR_UNTIL_LOGINDENIED)								//ÀÏÁ¤±â°£ ·Î±×ÀÎÁ¤Áö
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[18], 30);	//_LANGUAGE_JAPANESE (-23)
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[18], 3);
	else if (ConnectError == ERROR_LOGIN_DENIED)		//·Î±×ÀÎÁ¤Áö
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, HoTextLoginMessage[19], 30);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[19], 3);
	else if (ConnectError == ACCOUNT_NOT_ACTIVE)		// Conta desativada
		//DrawMessage(295 + m_AddWidth, 300 + m_AddHeight, "Conta não ativada, acesse o painel e confirme seu email!", 30);
		TitleBox::GetInstance()->SetText(HoTextLoginMessage[20], 3);

	return TRUE;
}

HANDLE	hHoLoginThread = 0;

DWORD WINAPI HoLoginThreadProc(void* pInfo)
{
	HoOpening* lpHoOpening = (HoOpening*)pInfo;

	//¼­¹ö¿ÍÀÇ Á¢¼ÓÀ» ½Ãµµ...
	if (!smWsockDataServer) {
		ConnectServer_InfoMain();

		if (smWsockDataServer)
		{
			ServerUserCharacterInfo = NULL;
			DownServerListInfo = NULL;
			ServerLoginError = NULL;
			lpHoOpening->ServerWaitTimeCount = 0;

			if (HoCommandLineFlag)
			{
				memset(UserAccount, 0, sizeof(UserAccount));
				memset(UserPassword, 0, sizeof(UserPassword));

				memcpy(UserAccount, HoCommandInfo.Id, strlen(HoCommandInfo.Id));
				memcpy(UserPassword, HoCommandInfo.Pass, strlen(HoCommandInfo.Pass));
			}
			TransUserCommand(smTRANSCODE_ID_GETUSERINFO, UserAccount, UserPassword); //UserÀÇ Á¤º¸¸¦ ¾Ë¾Æ ¿À±â...
		}
		else {
			lpHoOpening->ConnectError = ERROR_CONNECT;
			lpHoOpening->StartConnectServerFlag = FALSE;
		}
	}

	hHoLoginThread = 0;

	ExitThread(TRUE);
	return TRUE;
}

int HoOpening::LoginDataServer()
{
	DWORD dwHoLoginThreadId;

	if (!hHoLoginThread) {
		hHoLoginThread = CreateThread(NULL, 0, HoLoginThreadProc, this, 0, &dwHoLoginThreadId);

		PlayWav2(13 - 1);
		StartConnectServerFlag = TRUE;
		return TRUE;
	}
	return FALSE;
}

int HoOpening::LoginGameServer(int serverIndex)
{
	PlayWav2(13 - 1);
	DisconnectServer_GameMain();

	if (ConnectServer_GameMain(UserServerListInfo[serverIndex].szServerIp1,
		UserServerListInfo[serverIndex].dwServerPort1,
		UserServerListInfo[serverIndex].szServerIp2,
		UserServerListInfo[serverIndex].dwServerPort2,
		UserServerListInfo[serverIndex].szServerIp3,
		UserServerListInfo[serverIndex].dwServerPort3))
	{
		StartConnectServerFlag = TRUE;
	}
	else
		ConnectError = ERROR_CONNECT;

	return TRUE;
}

//KeyFilter¸¦ ÇÑ´Ù...
int HoOpening::KeyFilter(char* message)
{
	if (message == NULL || hFocusWnd == NULL)
		return FALSE;

	//if(lstrlen(message) >= 17 && message[16] != '\t') {
	//	message[16] = 0;
	if (lstrlen(message) >= 31 && message[30] != '\t')
	{
		message[30] = 0;
		SetWindowText(hFocusWnd, 0);
		SetWindowText(hFocusWnd, message);
		SendMessage(hFocusWnd, EM_SETSEL, lstrlen(message) + 1, lstrlen(message) + 1);
	}

	/*
	//Edit¿¡¼­ Left¸¦ ¾È¸Ô°Ô ÇÑ´Ù.
	int leftCaret;
	SendMessage(hFocusWnd, EM_GETSEL, (WPARAM)&leftCaret, NULL);
	if(LoginInputState == 0)
	{
		if(leftCaret < (int)lstrlen(UserAccount))
			SendMessage(hFocusWnd, EM_SETSEL, lstrlen(UserAccount), lstrlen(UserAccount)+1);
	}
	else if(LoginInputState == 1)
	{
		if(leftCaret < (int)lstrlen(UserPassword))
			SendMessage(hFocusWnd, EM_SETSEL, lstrlen(UserPassword), lstrlen(UserPassword)+1);
	}
	*/

	//¾È¾²´Â ¹®ÀÚ´Â Ã¼Å©ÇÑ´Ù.(Filter)
	for (int index = 0; index < (int)strlen(message); index++)
	{
		if (message[index] == '\t') //TabÅ°¸¦ ´­·¶À» °æ¿ì.
		{
			message[index] = 0;
			SetWindowText(hFocusWnd, 0);

			if (LoginInputState == 0)
			{
				SetIME_Mode(0);
				LoginInputState = 1;
				SetWindowText(hFocusWnd, UserPassword);
				SendMessage(hTextWnd, EM_SETSEL, lstrlen(UserPassword) + 1, lstrlen(UserPassword) + 1);
			}
			else if (LoginInputState == 1)
			{
				LoginInputState = 0;
				SetWindowText(hFocusWnd, UserAccount);
				SendMessage(hTextWnd, EM_SETSEL, lstrlen(UserAccount) + 1, lstrlen(UserAccount) + 1);
			}
		}


		if (message[index] == ' ')
		{
			message[index] = 0;
			SetWindowText(hFocusWnd, message);
			SendMessage(hTextWnd, EM_SETSEL, lstrlen(message) + 1, lstrlen(message) + 1);
			break;
		}
	}
	return TRUE;
};

//¼­¼­È÷ ¹à¾ÆÁü...
int HoOpening::MainFadeIn()
{
	if (FadeNextState == FADE_IN)
	{
		ScreenBlendValue -= 2;
		if (ScreenBlendValue < 0)
			ScreenBlendValue = 0;

		if (ScreenBlendValue <= 0)
		{
			ScreenBlendValue = 0;
			FadeCurrentState = FADE_IN;
			FadeNextState = FADE_IN;
		}
	}
	return TRUE;
}

//¼­¼­È÷ ¾îµÎ¿öÁü...
int HoOpening::MainFadeOut()
{
	if (FadeNextState == FADE_OUT)
	{
		if (ScreenBlendValue < 255)
		{
			OpeningBgmVolume -= 2;
			if (OpeningBgmVolume < 0)
				OpeningBgmVolume = 0;
			ScreenBlendValue += 3;
		}

		if (ScreenBlendValue >= 255)
		{
			ScreenBlendValue = 255;
			LogoBlendValue = 255;
			LoginBlendValue = 255;
			FadeCurrentState = FADE_OUT;
			FadeNextState = FADE_OUT;
		}
	}
	return TRUE;
}

//Cinema ÃÊ±âÈ­
void HoOpening::InitCinemaFadeEffect()
{
	CinemaFadeOutFlag = FALSE;
	CinemaBlendValue = 0;
	CinemaFadeState = STATE_FADE_NONE;
}

//Cinema ¼­¼­È÷ ¾îµÎ¿öÁü
void HoOpening::MainCinemaFadeEffect()
{
	if (CinemaFadeState == STATE_FADE_IN)
	{
		CinemaBlendValue += 5;
		if (CinemaBlendValue >= 255)
			CinemaBlendValue = 255;
	}

	if (CinemaFadeState == STATE_FADE_IN && CinemaBlendValue >= 255)
		CinemaFadeState = STATE_FADE_OUT;

	if (CinemaFadeState == STATE_FADE_OUT)
	{
		CinemaBlendValue -= 5;
		if (CinemaBlendValue <= 0)
			CinemaBlendValue = 0;
	}
}


//Cinema ¼­¼­È÷ ¾îµÎ¿öÁö´Â°Å ±×¸®±â
void HoOpening::DrawCinemaFadeEffect()
{
	if (CinemaFadeState != STATE_FADE_NONE)
	{
		if (CinemaBlendValue >= 0 && CinemaBlendValue <= 255)
		{

			dsDrawTexImage(WhiteBoxImage, 0, 0, smScreenWidth, smScreenHeight, CinemaBlendValue);

		}
	}
}


int HoOpening::FastFade()
{
	if ((FadeNextState == FADE_IN && LoginBlendValue < 255) ||
		(FadeNextState == FADE_OUT && LoginBlendValue > 0))
	{

		static bool fastBlendFlag = false;
		for (int index = 0; index < 256; index++)
		{
			if (VRKeyBuff[index] == TRUE)
			{
				fastBlendFlag = true;
				break;
			}
		}

		if (MouseDownFlag == FALSE)
		{
			if (MouseButton[0] && fastBlendFlag == false)
			{
				fastBlendFlag = true;
				MouseDownFlag = TRUE;
			}
		}

		if (fastBlendFlag)
		{
			if (FadeNextState == FADE_IN) //È¯ÇØÁø´Ù.
			{
				if ((CurrentWorkState == STATE_OPENING &&
					NextWorkState == STATE_OPENING) ||
					(CurrentWorkState == STATE_OPENING && NextWorkState == STATE_LOGIN_INPUT))
				{
					LogoBlendValue = 255;
					ScreenBlendValue = 0;
					FadeCurrentState = FADE_IN;

					if (HoCommandLineFlag == FALSE)
					{
						LoginBlendValue = 255;
						CurrentWorkState = STATE_LOGIN_INPUT;
						NextWorkState = STATE_LOGIN_INPUT;
					}

					if (HoCommandLineFlag)
						NextWorkState = STATE_LOGIN_INPUT;
					//CinemaFadeOutFlag = TRUE;
				}
			}
			else if (FadeNextState == FADE_OUT)//¾îµÎ¿ö Áø´Ù.
			{
				//				StartLogoLoginFlag = FALSE;	
				LogoBlendValue = 0;
				LoginBlendValue = 0;
				ScreenBlendValue = 255;
				OpeningBgmVolume = 0;
				FadeCurrentState = FADE_OUT;

			}
			fastBlendFlag = false;
		}
	}
	return TRUE;
}

int HoOpening::DrawFade()
{
	if (ScreenBlendValue >= 0 && ScreenBlendValue <= 255)
		dsDrawTexImage(BlackBoxImage, 0, 0, smScreenWidth, smScreenHeight, ScreenBlendValue);

	return TRUE;
}

int HoOpening::MainFade()
{
	if ((FadeNextState == FADE_IN) ||
		(FadeNextState == FADE_OUT))
	{
		if (FadeCurrentState != FadeNextState)
		{
			if (FadeNextState == FADE_IN)
				MainFadeIn();
			else
			{
				SetVolumeBGM(OpeningBgmVolume);
				MainFadeOut();
			}
		}
	}
	return TRUE;
}

void HoOpening::SetFade()
{
	//Fade InÀ» ÇØ¾ß ÇÒ°æ¿ì....(¼­¼­Èñ ¹à¾ÆÁü)

	if (FadeCurrentState == FADE_OUT)
	{
		FadeNextState = FADE_IN;
		ScreenBlendValue = 254;
		OpeningBgmVolume = 350;
	}
	else//Fade OutÀ» ÇØ¾ß ÇÒ °æ¿ì...(¾îµÎ¿öÁü)
	{
		FadeNextState = FADE_OUT;
		ScreenBlendValue = 1;
	}
}


void HoOpening::Close()
{
	if (LOGINSCREENHANDLER->pcStage)
	{
		delete LOGINSCREENHANDLER->pcStage;
		LOGINSCREENHANDLER->pcStage = NULL;

		//######################################################################################
		//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®		
		lpCurPlayer->pX = lpCurPlayer->pY = lpCurPlayer->pZ = 0;
		smGameStage[0] = NULL;
		smGameStage[1] = NULL;
		//######################################################################################
	}

	if (PatCamera)
	{
		delete PatCamera;
		PatCamera = NULL;
	}

	BackColor_R = 0;
	BackColor_G = 0;
	BackColor_B = 0;

	BackColor_mR = 0;
	BackColor_mG = 0;
	BackColor_mB = 0;

	InitSky();

	/*
	if(Bird)
	{
		delete Bird;
		Bird = NULL;
	}

	if(BirdBone)
	{
		delete BirdBone;
		BirdBone = NULL;
	}
	*/

	//InitSky();
	//smRender.Color_R = 40;
	//smRender.Color_G = 0;
	//smRender.Color_B = -30;
	//smRender.Color_A = 0;
}



//static BOOL HanGameErrorFlag = FALSE;
//return 1: ÇöÀç LoginÁøÇà½Ã.
//return 2: Loing ¼º°ø½Ã..
int HoOpening::Main()
{
	if (MouseButton[0] == FALSE)
	{
		MouseDownFlag = FALSE;
	}

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	float timeDelta = 1.f / 70.f;
	g_NewParticleMgr.Main(0.0f, timeDelta);

	MainSky();
	m_IsDrawOpening = s_AutoCamera.FrameMain(m_AutoCameraFrameTime);
	if (m_IsDrawOpening > 1 && CinemaFadeOutFlag == FALSE)
	{
		CinemaFadeState = STATE_FADE_IN;
		CinemaFadeOutFlag = TRUE;
	}


	SetCameraCharPos();
	if (m_IsDrawOpening == 1)
		SetParticleEffectSpawn();

	MainAssaEffect();
	MainCinemaFadeEffect();

	if (WaveCameraFlag)
	{
		WaveCameraTimeCount++;

		if ((WaveCameraTimeCount > WaveCameraDelay))
		{
			WaveCameraTimeCount = 0;
			if (WaveCameraDelay > 1 && WaveCameraFactor < 40)
				WaveCameraFactor = -int((float)WaveCameraFactor / 10.f * 9.f);
			else
				WaveCameraFactor = -int((float)WaveCameraFactor / 10.f * 8.f);
		}

		if (abs(WaveCameraFactor) < 1)
		{
			WaveCameraFlag = FALSE;
			WaveCameraTimeCount = 0;
		}
	}
	//######################################################################################


	//¿¡·¯ ¸Þ½ÃÁö Ãâ·Â½Ã.. ¾ø¾Ö´Â °æ¿ì...
	if (ConnectError != ERROR_NONE)
	{
		if (ConnectErrorCount >= 160)
		{
			MouseDownServerIndex = -1;
			bool errorEndFlag = false;
			//¸¶¿ì½º¸¦ Å¬¸¯ ÇßÀ» °æ¿ì.
			if (MouseButton[0] || MouseDblClick)
				errorEndFlag = true;

			//Å°º¸µå¸¦ ´­·¶À» °æ¿ì.
			for (int index = 0; index < 256; index++)
			{
				if (VRKeyBuff[index] == TRUE)
				{
					errorEndFlag = true;
					break;
				}
			}
			//½Ã°£À» ÃÊ°ú ÇßÀ» °æ¿ì.
			if (ConnectErrorCount >= 250)
				errorEndFlag = true;

			if (errorEndFlag)
			{
				ConnectError = ERROR_NONE;
				FocusWndFlag = FALSE;
				ConnectErrorCount = 0;
			}

			//ÇÑ°ÔÀÓ ¿¡·¯ Ã³¸®...
			if (errorEndFlag && HoCommandLineFlag
				&& CurrentWorkState == STATE_LOGIN_INPUT
				&& NextWorkState == STATE_LOGIN_INPUT)
			{
				//				HanGameErrorFlag		= TRUE;
				HoCommandLineFlag = FALSE;
				NextWorkState = STATE_LOGIN_INPUT;
				CurrentWorkState = STATE_OPENING;
				ConnectServerSucessFlag = FALSE;
				DisconnectServerFull();
			}
		}
		ConnectErrorCount++;
	}



	MainFade();

	//ÁöÇü
	if (CurrentWorkState == STATE_OPENING && NextWorkState == STATE_OPENING)
	{
		if (CinemaFadeState == STATE_FADE_OUT)
			NextWorkState = STATE_LOGIN_INPUT;
		//if(CameraFrame > 250*160) //OpeningÀÌ ¾î´À Á¤µµ ÁøÇàÀÌ µÇ¸é ÀÛ¾÷ »óÅÂ º¯°æ.
			//NextWorkState = STATE_LOGIN_INPUT;
	}

	//Opening »óÅÂ¿¡¼­ Login ÀÔ·Â »óÅÂ·Î º¯È­½Ã...
	if (CurrentWorkState == STATE_OPENING && NextWorkState == STATE_LOGIN_INPUT)
	{
		if (FadeNextState == FADE_IN) //FADE_INÀÌ ´ÙµÆÀ» °æ¿ì¸¸...
		{
			LogoBlendValue += 1;
			if (LogoBlendValue >= 255)
				LogoBlendValue = 255;

			if (LogoBlendValue >= 255)
			{
				//ÇÑ°ÔÀÓ ·Î±ä ¾ÆÀÌµð¸¦ ¼º°øÀûÀ¸·Î ¹Þ¾ÒÀ» °æ¿ì..
				if (HoCommandLineFlag)
				{
					CurrentWorkState = STATE_LOGIN_INPUT;
					LoginDataServer();
				}
				else//¾Æ´Ñ °æ¿ì ·Î±ä Ã¢À» º¸¿©ÁØ´Ù.
				{
					LoginBlendValue += 1;
					if (LoginBlendValue >= 255)
					{
						LoginBlendValue = 255;
						CurrentWorkState = STATE_LOGIN_INPUT;
					}
				}
			}
		}
	}

	//·Î±ä ÀÔ·ÂÀ» ÇÒ °æ¿ì.
	if (CurrentWorkState == STATE_LOGIN_INPUT
		&& NextWorkState == STATE_LOGIN_INPUT
		&& (LoginBlendValue >= 255)
		&& (ScreenBlendValue <= 0)
		&& StartConnectServerFlag == FALSE
		&& ConnectError == ERROR_NONE)
	{
		//Enter¸¦ ´­·¶À» °æ¿ì.
		if (FocusWndFlag && hFocusWnd == NULL
			&& ConnectServerSucessFlag == FALSE
			&& ConnectError == ERROR_NONE)
		{
			LoginDataServer();
			//CinemaFadeOutFlag = TRUE;
		}

		int downFlag = -1;

		if (MouseDownFlag == FALSE)
		{
			if (MouseButton[0])
			{
				if (pCursorPos.x > (startLogoX + 230 - 30) &&
					pCursorPos.y > (startLogoY + 320 + 93) &&
					pCursorPos.x < (startLogoX + 230 - 30 + 64) &&
					pCursorPos.y < (startLogoY + 320 + 93 + 25))
				{
					MouseDownFlag = TRUE;
					hFocusWnd = NULL;
					CurrentWorkState = STATE_LOGIN_INPUT;
					LoginDataServer();
				}
				else if (pCursorPos.x > (startLogoX + 230 + 32) &&
					pCursorPos.y > (startLogoY + 320 + 93) &&
					pCursorPos.x < (startLogoX + 230 + 32 + 64) &&
					pCursorPos.y < (startLogoY + 320 + 93 + 25))
				{
				}
				else if (pCursorPos.x > (startLogoX + 230 + 100) &&
					pCursorPos.y > (startLogoY + 320 + 93) &&
					pCursorPos.x < (startLogoX + 230 + 100 + 64) &&
					pCursorPos.y < (startLogoY + 320 + 93 + 25))
				{
					MouseDownFlag = TRUE;
					StartQuitFlag = TRUE;
					SetFade();
					PlayWav2(4 - 1);
				}
			}
		}

		if (bSettings)
		{
			if (MouseDownFlag == FALSE)
			{
				if (MouseButton[0])
				{

					if (GetClick(((smConfig.ScreenSize.x >> 1) - 279) + 336, ((smConfig.ScreenSize.y >> 1) - 218) + 186 + 10, 15, 15) && MouseButton[0])
					{
						if (CameraInvRot)
							CameraInvRot = FALSE;
						else
							CameraInvRot = TRUE;

						MouseDownFlag = TRUE;
					}

					if (GetClick(((smConfig.ScreenSize.x >> 1) - 279) + 336, ((smConfig.ScreenSize.y >> 1) - 218) + 203 + 10, 15, 15) && MouseButton[0])
					{
						if (smConfig.BGM_Mode)
							smConfig.BGM_Mode = FALSE;
						else
							smConfig.BGM_Mode = TRUE;

						MouseDownFlag = TRUE;
					}

					if (GetClick(((smConfig.ScreenSize.x >> 1) - 279) + 336, ((smConfig.ScreenSize.y >> 1) - 218) + 220 + 10, 15, 15) && MouseButton[0])
					{
						bAdjustWindow = !bAdjustWindow;

						MouseDownFlag = TRUE;
					}

					if (GetClick(((smConfig.ScreenSize.x >> 1) - 279) + 336, ((smConfig.ScreenSize.y >> 1) - 218) + 237 + 10, 15, 15) && MouseButton[0])
					{
						bNoLag = !bNoLag;

						MouseDownFlag = TRUE;
					}

					if ((pCursorPos.x > (smConfig.ScreenSize.x >> 1) - (177 >> 1)) && (pCursorPos.x < (smConfig.ScreenSize.x >> 1) - (177 >> 1) + 176) &&
						(pCursorPos.y > (smConfig.ScreenSize.y >> 1) + (120)) && (pCursorPos.y < (smConfig.ScreenSize.y >> 1) + (120 + 40)))
					{
						MouseDownFlag = TRUE;
						hFocusWnd = NULL;
					}
					else if ((pCursorPos.x > (smConfig.ScreenSize.x >> 1) - (177 >> 1)) && (pCursorPos.x < (smConfig.ScreenSize.x >> 1) - (177 >> 1) + 176) &&
						(pCursorPos.y > (smConfig.ScreenSize.y >> 1) + (170)) && (pCursorPos.y < (smConfig.ScreenSize.y >> 1) + (170 + 40)))
					{
						MouseDownFlag = TRUE;
						StartQuitFlag = TRUE;
						SetFade();
						PlayWav2(4 - 1);
					}
				}
			}
		}
	}

	//Game Server¸¦ ¼±ÅÃÇÏ±â
	if (CurrentWorkState == STATE_SERVER_SELECT
		&& NextWorkState == STATE_SERVER_SELECT)
	{
		LoginGameServer(0);
	}

	if (CurrentWorkState == STATE_SERVER_SELECT
		&& NextWorkState == STATE_SERVER_SELECT)
	{
		BOOL flag = FALSE;
		if (ConnectError == ERROR_NONE && !StartConnectServerFlag && StartQuitFlag == FALSE)
		{
			if (MouseDownFlag == FALSE)
			{
				if (MouseButton[0])
				{
					if ((pCursorPos.x > (smConfig.ScreenSize.x >> 1) - (177 >> 1)) && (pCursorPos.x < (smConfig.ScreenSize.x >> 1) - (177 >> 1) + 176) &&
						(pCursorPos.y > (smConfig.ScreenSize.y >> 1) + (170)) && (pCursorPos.y < (smConfig.ScreenSize.y >> 1) + (170 + 40)))
					{
						flag = TRUE;
						MouseDownFlag = TRUE;
					}
				}
			}
		}

		//DataServer¿ÍÀÇ ¿¬°áÀÌ ²÷¾î Áú°æ¿ì..
		if (!smWsockDataServer && GameErrorValue == 0)
		{
			NextWorkState = STATE_LOGIN_INPUT;
			ConnectServerSucessFlag = FALSE;
			DisconnectServerFull();
			ConnectError = ERROR_NO_SERVER_CONNECT;
		}

		//³ª°¡±â ¹öÆ°À» ´©¸¥ °æ¿ì..
		if (flag)
		{
			PlayWav2(4 - 1);
			//ÇÑ°ÔÀÓÀÌ ¾Æ´Ï¸é.
			if (!HoCommandLineFlag)
			{
				FocusWndFlag = FALSE;
				NextWorkState = STATE_LOGIN_INPUT;
				CurrentWorkState = STATE_LOGIN_INPUT;
				ConnectServerSucessFlag = FALSE;
				DisconnectServerFull();
			}
			else
			{
				DisconnectServerFull();
				StartQuitFlag = TRUE;
				SetFade();
				ConnectServerSucessFlag = FALSE;
				MouseDownFlag = FALSE;
			}
		}
	}

	//¼­¹ö¿¡ ¿¬°áÁßÀÌ¸é....
	if (StartConnectServerFlag)
	{
		if (ConnectError != ERROR_NONE)
		{
			StartConnectServerFlag = FALSE;
			ServerWaitTimeCount = 0;
		}

		if (ServerLoginError)
		{
			if (CurrentWorkState == STATE_LOGIN_INPUT
				&& NextWorkState == STATE_LOGIN_INPUT)
				DisconnectServerFull();

			if (CurrentWorkState == STATE_SERVER_SELECT
				&& NextWorkState == STATE_SERVER_SELECT)
				DisconnectServer_GameMain();

			if (ServerLoginError->WParam == 0) //¿¬°á ºÒ°¡.
				ConnectError = ERROR_CONNECT;
			else if (ServerLoginError->WParam == -1)
				ConnectError = ERROR_NO_ACCOUNT;
			else if (ServerLoginError->WParam == -2)
				ConnectError = ERROR_NO_PASSWORD;
			else if (ServerLoginError->WParam == -3)
				ConnectError = ERROR_NO_BETA_TESTER;
			else if (ServerLoginError->WParam == -4)
				ConnectError = ERROR_NO_ACCOUNT_IN;
			else if (ServerLoginError->WParam == -5)
				ConnectError = ERROR_NO_USE_DATE_OVER;
			else if (ServerLoginError->WParam == -6)
				ConnectError = ERROR_NO_USE_TIME_OVER;
			else if (ServerLoginError->WParam == -8)
				ConnectError = ERROR_DELETE_ACCOUNT_NOTIFY;
			else if (ServerLoginError->WParam == -16)
				ConnectError = ERROR_SERVER_TRAFFIC_BUSY;
			else if (ServerLoginError->WParam == -17)
				ConnectError = ERROR_SERVER_WAIT_MOMNENT;
			else if (ServerLoginError->WParam == -18)
				ConnectError = ERROR_THIRD_CONNECT;
			else if (ServerLoginError->WParam == -19)
				ConnectError = ERROR_HAN_PASSWORD;
			else if (ServerLoginError->WParam == -12)
				ConnectError = ERROR_12YEARS;
			else if (ServerLoginError->WParam == -13)
				ConnectError = ERROR_HOMEAGREE;
			else if (ServerLoginError->WParam == -23)
				ConnectError = ERROR_UNTIL_LOGINDENIED;
			else if (ServerLoginError->WParam == -24)
				ConnectError = ERROR_LOGIN_DENIED;
			else if (ServerLoginError->WParam == -25) // Conta não ativa
				ConnectError = ACCOUNT_NOT_ACTIVE;


			StartConnectServerFlag = FALSE;
			ServerWaitTimeCount = 0;
			ServerLoginError = NULL;
		}

		ServerWaitTimeCount++;
		if (ServerWaitTimeCount >= 3000)	//¼­¹ö¸¦ ±â´Ù¸®´Â ½Ã°£...
		{
			ServerWaitTimeCount = 0;
			ConnectError = ERROR_CONNECT;
			StartConnectServerFlag = FALSE;
			ConnectServerSucessFlag = FALSE;
		}

		//Data¼­¹ö ·Î±×ÀÎ½Ã..
		if (CurrentWorkState == STATE_LOGIN_INPUT &&
			NextWorkState == STATE_LOGIN_INPUT)
		{
			if (ServerUserCharacterInfo
				&& DownServerListInfo
				&& ConnectError == ERROR_NONE
				&& GameErrorValue == 0)
			{
#ifdef _XTRAP_GUARD_4_CLIENT
				XTrap_C_SetUserInfo(UserAccount, szConnServerName, NULL, NULL, 0);	//XTrapD5
#endif

				StartConnectServerFlag = FALSE;
				ConnectServerSucessFlag = TRUE;
				CurrentWorkState = STATE_SERVER_SELECT;
				NextWorkState = STATE_SERVER_SELECT;
				GameErrorValue = -1;
		}
	}

		//Game¼­¹ö ·Î±×ÀÎ½Ã..
		if (CurrentWorkState == STATE_SERVER_SELECT
			&& NextWorkState == STATE_SERVER_SELECT)
		{
			//count = 0;
			if (ConnectError == ERROR_NONE && GameErrorValue == 0)
			{
				StartConnectServerFlag = FALSE;
				SetFade();
				NextWorkState = STATE_END;
				GameErrorValue = -1;
			}
		}

		if (GameErrorValue != 0)
		{
			//VersionÀÌ ¸ÂÁö ¾ÊÀ» °æ¿ì...
			if (GameErrorValue == 1)
			{
				DisconnectServerFull();
				ConnectError = ERROR_NO_VERSION;
				StartConnectServerFlag = FALSE;
				GameErrorValue = -1;
			}

			//¼­¹ö ÀÎ¿ø¼ö Á¦ÇÑÀÌ ÀÖÀ» °æ¿ì..
			if (GameErrorValue == 2)
			{
				if (CurrentWorkState == STATE_LOGIN_INPUT
					&& NextWorkState == STATE_LOGIN_INPUT)
					DisconnectServerFull();

				if (CurrentWorkState == STATE_SERVER_SELECT
					&& NextWorkState == STATE_SERVER_SELECT)
					DisconnectServer_GameMain();

				ConnectError = ERROR_NO_LIMIT;
				StartConnectServerFlag = FALSE;
				GameErrorValue = -1;
			}
		}
}

	if (ConnectServerSucessFlag)
	{
		if (FadeCurrentState == FADE_OUT && FadeNextState == FADE_OUT)
		{
#ifdef  _LANGUAGE_KOREAN			
			SetRegString(HKEY_LOCAL_MACHINE, "SOFTWARE\\Triglow Pictures\\PristonTale", "Account", "  ");
#endif

#ifndef _LANGUAGE_KOREAN			
			//Login °èÁ¤À» ·¹Áö½ºÆ®¸®¿¡ ÀúÀåÇÑ´Ù..

#endif

			SetWindowText(hTextWnd, NULL);
			SendMessage(hTextWnd, EM_SETLIMITTEXT, 80, 0);

			StopBGM();
			return 2;
	}
	}

	if (StartQuitFlag)
	{
		if (FadeCurrentState == FADE_OUT && FadeNextState == FADE_OUT)
			quit = 1;
	}

	return 1;
		}


