#include "..\globals.h"
#include "..\\SinBaram\\SinLinkHeader.h"
#include "..\\HoBaram\\HoLinkHeader.h"
#include "..\\smReg.h"
#include <TitleBox/TitleBox.h>
#include <Engine/Directx/DXVideoRenderer.h>
#include <Login/LoginModel.h>
#include <Login/LoginScreen.h>

HoLogin* lpLogin = 0;
HoOpening* lpOpening = 0;

//bool OldSkyDrawFlag = false;

char CharacterName1[256];
char CharacterName2[256];

int GameStartField = -1;

int WorkMode = OPENING_MODE;

int CursorShowFlag = TRUE;
int LoginEndFlag = FALSE;

bool bCaptureOpening = FALSE;
DWORD dwTickCap = 0;

void HoReLoad()
{
	if (lpLogin != NULL && WorkMode == LOGIN_MODE)
	{
		lpLogin->ReLoad();
	}
}

void CaptureOpening()
{
	dwTickCap = GetTickCount();
	bCaptureOpening = TRUE;
}

int InitOpening()
{
	memset(CharacterName1, 0, sizeof(CharacterName1));
	memset(CharacterName2, 0, sizeof(CharacterName2));

	InitCursor();

	lpLogin = new HoLogin(1);
	lpOpening = new HoOpening();

	if (WorkMode == OPENING_MODE)
		lpOpening->Init();
	else if (WorkMode == LOGIN_MODE)
		lpLogin->Init();

	CursorShowFlag = TRUE;
	LoginEndFlag = FALSE;

	return TRUE;
}

int DrawOpening()
{
	//int hoOldTime = GetCurrentTime();
	if (lpOpening == NULL || lpLogin == NULL)
		return FALSE;

	if (Graphics::Graphics::GetInstance()->IsDeviceReady())
	{
		if (Graphics::Graphics::GetInstance()->GetRenderer()->Begin())
		{
			if (WorkMode == OPENING_MODE)
			{
				DXVIDEORENDERER->Update();
				lpOpening->Draw();
			}
			else if (WorkMode == LOGIN_MODE)
			{
				lpLogin->Draw();
			}

	

			TitleBox::GetInstance()->Render();

			if (GRAPHICDEVICE && LoginEndFlag == FALSE)
			{
				DrawCursor();
			}

			Graphics::Graphics::GetInstance()->GetRenderer()->End();
		}
	}

	if (LoginEndFlag)
		CursorShowFlag = FALSE;
	return TRUE;
}

void resizeOpening()
{
	if (lpOpening)
	{
		lpOpening->resize();
	}
}

void resizeLogin()
{
	if (lpLogin)
	{
		lpLogin->resize();
	}
}

static int GameErrorTimeCount = 0;
int MainOpening()
{
	if (lpOpening == NULL || lpLogin == NULL)
		return FALSE;

	if (WorkMode == OPENING_MODE)
	{
		if (lpOpening->Main() == 2)
		{
			lpLogin->Init();
			WorkMode = LOGIN_MODE;
		}
	}
	else if (WorkMode = LOGIN_MODE)
	{
		int num;
		num = lpLogin->Main();
		if (num == 2)
		{
			lpOpening->Init();
			WorkMode = OPENING_MODE;
		}
		else if (num == 3)
			LoginEndFlag = TRUE;

		if (LoginEndFlag && CursorShowFlag == FALSE)
		{
			WorkMode = MODE_NONE;
			StopAllWav();
			StopBGM();

			if (hTextWnd)
			{
				SetWindowText(hTextWnd, 0);
			}

			return TRUE;
		}

	}
	return FALSE;
}

int CloseOpening()
{
	if (lpOpening != NULL)
	{
		delete lpOpening;
		lpOpening = NULL;
	}

	if (lpLogin != NULL)
	{
		delete lpLogin;
		lpLogin = NULL;
	}
	return TRUE;
}

TRANS_USERCHAR_INFO* ServerUserCharacterInfo = NULL;
smTRANS_CHAR_COMMAND2* ServerReturn = NULL;
smTRANS_COMMAND* ServerLoginError = NULL;
TRANS_SERVER_LIST* DownServerListInfo = NULL;

//ServerList 정보.
hoServerListInfo		UserServerListInfo[MAX_DOWN_SERVER_LIST_INFO];
int						UserServerListInfoCount;

//UserList 정보.
HoUserCharacterInfo		DownUserCharacterInfo[MAX_DOWN_USER_LIST_INFO];	//유저의 정보..
int						DownUserCharacterCount;

int HoRecvMessage(DWORD dwCode, void* RecvBuff)
{
	if (dwCode == smTRANSCODE_ID_SETUSERINFO)//User List를 받는다.
	{
		ServerUserCharacterInfo = (TRANS_USERCHAR_INFO*)RecvBuff;
		if (strcmp(UserAccount, ServerUserCharacterInfo->szID) != 0)
			return FALSE;

		memset(DownUserCharacterInfo, 0, sizeof(DownUserCharacterInfo));
		DownUserCharacterCount = 0;

		//UserCharacter
		for (int index = 0; index < ServerUserCharacterInfo->PlayUserCount; index++)
		{
			wsprintf(DownUserCharacterInfo[index].Name, "%s", ServerUserCharacterInfo->CharInfo[index].szName);
			DownUserCharacterInfo[index].Level = ServerUserCharacterInfo->CharInfo[index].Level;

			wsprintf(DownUserCharacterInfo[index].BodyName, "%s", ServerUserCharacterInfo->CharInfo[index].szModelName);
			wsprintf(DownUserCharacterInfo[index].FaceName, "%s", ServerUserCharacterInfo->CharInfo[index].szModelName2);

			DownUserCharacterInfo[index].Job = ServerUserCharacterInfo->CharInfo[index].JOB_CODE;
			DownUserCharacterInfo[index].StartField = ServerUserCharacterInfo->CharInfo[index].StartField;

			DownUserCharacterInfo[index].PosX = ServerUserCharacterInfo->CharInfo[index].PosX;
			DownUserCharacterInfo[index].PosZ = ServerUserCharacterInfo->CharInfo[index].PosZ;


		}
		DownUserCharacterCount = ServerUserCharacterInfo->PlayUserCount;
	}
	else if (dwCode == smTRANSCODE_SERVER_INFO) //ServerList를 받는다.
	{
		DownServerListInfo = (TRANS_SERVER_LIST*)RecvBuff;
		//서버 정보 관련.
		memset(UserServerListInfo, 0, sizeof(UserServerListInfo));
		UserServerListInfoCount = 0;

		lstrcpy(szConnServerName, DownServerListInfo->szServerName);

		//서버 리스트를 저장한다.
		for (int index = 0; index < DownServerListInfo->ServerCount; index++)
		{
			memcpy(UserServerListInfo[index].szServerName, DownServerListInfo->ServerInfo[index].szServerName, strlen(DownServerListInfo->ServerInfo[index].szServerName));
			memcpy(UserServerListInfo[index].szServerIp1, DownServerListInfo->ServerInfo[index].szServerIp1, strlen(DownServerListInfo->ServerInfo[index].szServerIp1));
			memcpy(UserServerListInfo[index].szServerIp2, DownServerListInfo->ServerInfo[index].szServerIp2, strlen(DownServerListInfo->ServerInfo[index].szServerIp2));
			memcpy(UserServerListInfo[index].szServerIp3, DownServerListInfo->ServerInfo[index].szServerIp3, strlen(DownServerListInfo->ServerInfo[index].szServerIp3));

			UserServerListInfo[index].dwServerPort1 = DownServerListInfo->ServerInfo[index].dwServerPort1;
			UserServerListInfo[index].dwServerPort2 = DownServerListInfo->ServerInfo[index].dwServerPort2;
			UserServerListInfo[index].dwServerPort3 = DownServerListInfo->ServerInfo[index].dwServerPort3;

			UserServerListInfoCount++;
		}
	}
	else if (dwCode == smTRANSCODE_FAILCONNECT)
		ServerLoginError = (smTRANS_COMMAND*)RecvBuff;
	else
		ServerReturn = (smTRANS_CHAR_COMMAND2*)RecvBuff;

	//smTRANSCODE_ISRECORDDATA:  //캐릭터가 존재하는지 확인..
	//smTRANSCODE_INSRECORDDATA: //ID에 새로운 캐릭터 삭제..
	return TRUE;
}

void DDB2DIB(HBITMAP hbit, char* Path)
{
	BITMAPFILEHEADER	fh;
	BITMAPINFOHEADER	ih;
	BITMAP				bit;
	BITMAPINFO* pih = NULL;
	int					PalSize;
	HANDLE				hFile;
	DWORD				dwWritten, Size;
	HDC					hdc = NULL;


	hdc = GetDC(NULL);



	GetObject(hbit, sizeof(BITMAP), &bit);
	ih.biSize = sizeof(BITMAPINFOHEADER);
	ih.biWidth = bit.bmWidth;
	ih.biHeight = bit.bmHeight;
	ih.biPlanes = 1;
	ih.biBitCount = bit.bmPlanes * bit.bmBitsPixel;
	if (ih.biBitCount > 8) ih.biBitCount = 24;
	ih.biCompression = BI_RGB;
	ih.biSizeImage = 0;
	ih.biXPelsPerMeter = 0;
	ih.biYPelsPerMeter = 0;
	ih.biClrUsed = 0;
	ih.biClrImportant = 0;

	PalSize = (ih.biBitCount == 24 ? 0 : 1 << ih.biBitCount) * sizeof(RGBQUAD);
	pih = (BITMAPINFO*)malloc(ih.biSize + PalSize);
	pih->bmiHeader = ih;


	GetDIBits(hdc, hbit, 0, bit.bmHeight, NULL, pih, DIB_RGB_COLORS);
	ih = pih->bmiHeader;


	if (ih.biSizeImage == 0)
	{
		ih.biSizeImage = ((((ih.biWidth * ih.biBitCount) + 31) & ~31) >> 3) * ih.biHeight;
	}


	Size = ih.biSize + PalSize + ih.biSizeImage;
	pih = (BITMAPINFO*)realloc(pih, Size);


	GetDIBits(hdc, hbit, 0, bit.bmHeight, (PBYTE)pih + ih.biSize + PalSize, pih, DIB_RGB_COLORS);

	ReleaseDC(NULL, hdc);


	fh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + PalSize;
	fh.bfReserved1 = 0;
	fh.bfReserved2 = 0;
	fh.bfSize = Size + sizeof(BITMAPFILEHEADER);
	fh.bfType = 0x4d42;


	hFile = CreateFile(Path, GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hFile, &fh, sizeof(fh), &dwWritten, NULL);
	WriteFile(hFile, pih, Size, &dwWritten, NULL);
	CloseHandle(hFile);

	if (pih != NULL)
	{
		free(pih);
		pih = NULL;
	}

}


void Capture(HWND handle)
{

	if (handle == NULL) return;
	CreateDirectory("save\\screenshots", NULL);

	HBITMAP		bitmap = NULL, bitmapOld;
	HDC			scrDC, memDC;

	RECT	tempRect;
	POINT	pt;
	GetClientRect(handle, &tempRect);
	pt.x = tempRect.left;
	pt.y = tempRect.top;
	ClientToScreen(handle, &pt);
	OffsetRect(&tempRect, pt.x, pt.y);

	scrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
	memDC = CreateCompatibleDC(scrDC);
	bitmap = CreateCompatibleBitmap(scrDC, tempRect.right - tempRect.left, tempRect.bottom - tempRect.top);
	bitmapOld = (HBITMAP)SelectObject(memDC, bitmap);

	SYSTEMTIME systemTime;
	GetLocalTime(&systemTime);

	BitBlt(memDC, 0, 0, tempRect.right, tempRect.bottom, scrDC, tempRect.left, tempRect.top, SRCCOPY);

	SelectObject(memDC, bitmapOld);
	DeleteDC(memDC);
	DeleteDC(scrDC);

	if (bitmap != NULL)
	{
		char buffer[256];
		char numBuffer[64];
		memset(buffer, 0, sizeof(buffer));

		strcpy(buffer, "save\\screenshots\\");

		memset(numBuffer, 0, sizeof(numBuffer));
		sprintf(numBuffer, "%d%s", (int)systemTime.wYear, "-");
		strcat(buffer, numBuffer);

		memset(numBuffer, 0, sizeof(numBuffer));
		sprintf(numBuffer, "%d%s", (int)systemTime.wMonth, "-");
		strcat(buffer, numBuffer);

		memset(numBuffer, 0, sizeof(numBuffer));
		sprintf(numBuffer, "%d%s", (int)systemTime.wDay, "-");
		strcat(buffer, numBuffer);

		memset(numBuffer, 0, sizeof(numBuffer));
		sprintf(numBuffer, "%d%s", (int)systemTime.wHour, "-");
		strcat(buffer, numBuffer);

		memset(numBuffer, 0, sizeof(numBuffer));
		sprintf(numBuffer, "%d%s", (int)systemTime.wMinute, "-");
		strcat(buffer, numBuffer);

		memset(numBuffer, 0, sizeof(numBuffer));
		sprintf(numBuffer, "%d", (int)systemTime.wSecond);
		strcat(buffer, numBuffer);

		strcat(buffer, ".bmp");

		DDB2DIB(bitmap, buffer);
		DeleteObject(bitmap);

		CHATGAMEHANDLE->AddChatBoxText("> Captura Salva na pasta!", EChatColor::CHATCOLOR_Error);
	}
}
int GameErrorValue = -1;
int SetGameError(int num)
{
	GameErrorValue = num;
	return TRUE;
}


int DrawInterfaceParty()
{
	InterfaceParty.Draw();

	EffectHitBoard.Draw();
	return TRUE;
}

int MainInterfaceParty()
{
	InterfaceParty.Main();
	EffectHitBoard.Main();
	return TRUE;
}


int CloseInterfaceParty()
{
	InterfaceParty.Close();
	return TRUE;
}

int	SetInterfaceParty(char* chrName, DWORD chrCode)
{
	InterfaceParty.SetParty(chrName, chrCode);
	return TRUE;
}

int MainInterfaceParty(int x, int y)
{
	InterfaceParty.MainPartyMessage(x, y);
	return TRUE;
}

int DrawInterfaceParty(int x, int y)
{
	InterfaceParty.DrawPartyMessage(x, y);
	return TRUE;
}

//ktj : 퀘스트 안움직이게 하기
extern int QuestnotCharMove;
extern int ClanCharMove;	//클랜용
extern int ClanCLCharMove;	//클랜 채팅리스트용 
BOOL HoStopCharMotion()
{
	//호동 인터페이스 나올때 캐릭터 움직이지 않게..
	if (InterfaceParty.MessageBoxOpenFlag)
	{
		if (pCursorPos.x >= 653 && pCursorPos.x <= 653 + 23 && pCursorPos.y >= 14 && pCursorPos.y <= 14 + 23)
			return TRUE;

		if (pCursorPos.x >= 653 && pCursorPos.x <= 653 + 23 && pCursorPos.y >= 41 && pCursorPos.y <= 41 + 23)
			return TRUE;
	}


	//ktj : 동료,친구 메뉴 새로고침
	if (InterfaceParty.PartyPosState == PARTY_PROCESS)
	{
		//if(pCursorPos.x >=638 && pCursorPos.x <= 800 && pCursorPos.y >= 125+33 && pCursorPos.y <= 371+33)
		if (600 <= pCursorPos.x && pCursorPos.x <= 800 && pCursorPos.y >= 125 + 33 && pCursorPos.y <= 371 + 33)
			return TRUE;
	}

	//ktj :  새로고침
	//퀘스트용
	if (QuestnotCharMove) {
		//if(pCursorPos.x >=(638-180) && pCursorPos.x <= 800 && pCursorPos.y >= 125+33 && pCursorPos.y <= 371+33)
		if (600 <= pCursorPos.x && pCursorPos.x <= 800 && pCursorPos.y >= 125 + 33 && pCursorPos.y <= 371 + 33)
			return TRUE;
	}


	//클랜용
	if (ClanCharMove) {
		//클랜 배경그림위치
		extern POINT clanBoxPos;
		if (clanBoxPos.x <= pCursorPos.x && pCursorPos.x <= (clanBoxPos.x + 64 * 5) &&
			clanBoxPos.y <= pCursorPos.y && pCursorPos.y <= (clanBoxPos.y + 64 * 5))
			return TRUE;
	}

	//클랜채팅리스트용
	if (ClanCLCharMove) {

		//클랜 배경그림위치
		extern POINT clanCLBoxPos;
		if (clanCLBoxPos.x <= pCursorPos.x && pCursorPos.x <= (clanCLBoxPos.x + 256) &&
			clanCLBoxPos.y <= pCursorPos.y && pCursorPos.y <= (clanCLBoxPos.y + 256))
			return TRUE;
	}


	return FALSE;
}


int hoDrawTexImageFloat(int Mat, float putX, float putY, float putSizeX, float putSizeY, float getTexX, float getTexY, float getTexSizeX, float getTexSizeY, float texSizeX, float texSizeY, int alpha)
{
	D3DTLVERTEX tlVertex[4];
	int cnt;
	for (cnt = 0; cnt < 4; cnt++)
	{
		tlVertex[cnt].rhw = 1;
		tlVertex[cnt].color = D3DCOLOR_RGBA(255, 255, 255, alpha);
		tlVertex[cnt].specular = D3DCOLOR_RGBA(0, 0, 0, 0);
		tlVertex[cnt].sz = 0;
	}

	float surWidth;
	float surHeight;
	surWidth = (float)texSizeX;
	surHeight = (float)texSizeY;
	tlVertex[0].sx = putX;
	tlVertex[0].sy = putY;
	tlVertex[0].tu = (1.f / surWidth) * getTexX;
	tlVertex[0].tv = (1.f / surHeight) * getTexY;
	tlVertex[1].sx = putX;
	tlVertex[1].sy = putY + putSizeY;
	tlVertex[1].tu = (1.f / surWidth) * getTexX;
	tlVertex[1].tv = (1.f / surHeight) * (getTexY + getTexSizeY);
	tlVertex[2].sx = putX + putSizeX;
	tlVertex[2].sy = putY;
	tlVertex[2].tu = (1.f / surWidth) * (getTexX + getTexSizeX);
	tlVertex[2].tv = (1.f / surHeight) * getTexY;
	tlVertex[3].sx = putX + putSizeX;
	tlVertex[3].sy = putY + putSizeY;
	tlVertex[3].tu = (1.f / surWidth) * (getTexX + getTexSizeX);
	tlVertex[3].tv = (1.f / surHeight) * (getTexY + getTexSizeY);
	DrawTexture(smMaterial[Mat].smTexture[0]->lpD3DTexture, tlVertex, true);
	return TRUE;
}


// 문자열 비교 짧은 문장의 길이만큼 비교하여 비교 종료 포인터 반환
static int CompString(char* src1, char* src2)
{
	int cnt;
	int len1, len2;
	int len;

	len1 = lstrlen(src1);         //인자로 받은 문자의 길이를 구해 저장
	len2 = lstrlen(src2);

	if (len1 < len2) len = len1;
	else len = len2;

	if (!len) return NULL;

	for (cnt = 0; cnt < len; cnt++) {
		if (src1[cnt] == 0 || src2[cnt] == 0) break;//문장의 길이가 0이면 종료
		if (src1[cnt] != src2[cnt]) return NULL;  //두문장이 같지않으면 NULL값을 반환
	}

	return cnt;
}
// 공백의 문자열을 잘라낸다
int szSpaceSorting(char* lpString)
{
	int cnt, len;
	len = lstrlen(lpString);
	for (cnt = len - 1; cnt >= 0; cnt--) {
		if (lpString[cnt] != ' ') {
			lpString[cnt + 1] = 0;
			break;
		}
	}
	if (cnt < 0) lpString[0] = 0;

	return cnt;
}

char* CpFindString(char* lpData, char* lpFindWord, char* lpSaveBuff, int buffSize)
{
	//버퍼에 저장된 문자를 인자에 맞게 검색해서 저장한다
	int cnt;
	int len;
	int ep;
	int cnt2;
	char* lpsp;

	len = lstrlen(lpData);

	for (cnt = 0; cnt < len; cnt++) {
		ep = CompString(lpData + cnt, lpFindWord);
		if (ep) {
			lpsp = lpData + cnt + ep;
			for (cnt2 = 0; cnt2 < buffSize - 1; cnt2++) {
				if (lpsp[cnt2] == 0x0D || lpsp[cnt2] == 0x0A || lpsp[cnt2] == 0 || lpsp[cnt2] == ' ') break;
			}
			if (lpSaveBuff) {
				memcpy(lpSaveBuff, lpsp, cnt2);
				lpSaveBuff[cnt2] = 0;
				szSpaceSorting(lpSaveBuff);
			}
			return lpsp;
		}
	}
	return NULL;
}




hoCommandInfo	HoCommandInfo;
BOOL			HoCommandLineFlag = FALSE;
int				HoCommunity = COMMUNITY_TRIGLOW;
/* //해외 Handes 블럭처리
void HoCommandLineDecoder()
{
	char szBuffer1[256];
	char szBuffer2[256];
	char *hanGameUserInfo;
	hanGameUserInfo = szCmdLine;
	HoCommandLineFlag = FALSE;
	memset(&HoCommandInfo, 0, sizeof(HoCommandInfo));

	hanGameUserInfo = CpFindString(hanGameUserInfo, "/HANGAME", szBuffer1, sizeof(szBuffer1));
	if(hanGameUserInfo != NULL)
		HoCommunity = COMMUNITY_HANGAME;

	if(HoCommunity == COMMUNITY_TRIGLOW)
		return;

	hanGameUserInfo = szCmdLine;
	hanGameUserInfo = CpFindString(hanGameUserInfo, "/ENC=", szBuffer1, sizeof(szBuffer1));
	if(hanGameUserInfo)
	{
		DecryptFunc( szBuffer1, szBuffer2);
		hanGameUserInfo = szBuffer2;
		hanGameUserInfo = CpFindString(hanGameUserInfo, "/ID=", szBuffer1, sizeof(szBuffer1));
		if(hanGameUserInfo)
		{
			memcpy(HoCommandInfo.Id, szBuffer1, strlen(szBuffer1));
			hanGameUserInfo = CpFindString(hanGameUserInfo, "/PWD=", szBuffer1, sizeof(szBuffer1));
			if(hanGameUserInfo)
			{
				memcpy(HoCommandInfo.Pass, szBuffer1, strlen(szBuffer1));
				HoCommandLineFlag = TRUE;
			}

		}
	}
}
*/
BOOL		HoEffectResLoadingFlag = TRUE;
void		HoSetEffectResLoading(BOOL flag)
{
	HoEffectResLoadingFlag = flag;
}

extern POINT3D	WaveCameraPosi;
extern int		WaveCameraFactor;
extern BOOL		WaveCameraFlag;
extern int		WaveCameraDelay;

extern void		EffectWaveCamera(int factor, int delay)
{
	WaveCameraFlag = TRUE;
	WaveCameraFactor = factor;
	WaveCameraDelay = delay;
}

