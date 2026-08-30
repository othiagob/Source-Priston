#define LEVELUP_HELP_CPP
#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include "Character/playmain.h"
#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		

#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"

#include "cE_CViewClanInfo.h"
#endif

#include "tjscroll.h"
#include "tjclan.h"
#include "clan_enti.h"
#include "cE_Notice.h"

#include "Help.h"
#include "GuideHelp.h"


int  haGuidHelpLevel_1Flag = 0;
CHelp cHelp_Menu;
extern int LevelpUpButton[3];

CHelp::CHelp()
{
	m_HelpN = -1;
	m_bDraw = TRUE;

	nHelp0 = nHelp1 = 0;
	m_ImageCount = 0;
	nBox1 = nBox2 = nBox3 = nBox4 = nBox5 = nBox6 = nBox7 = nBox8 = nBox9 = 0;

	ZeroMemory(m_HelpContent, sizeof(m_HelpContent));
	nHeight_Count = 0;
	Box_x = 5;
	Box_y = 5;
	nBoxWidth_Count = BOX_WIDTH_COUNT;
	nBoxHeight_Count = 5;
	ZeroMemory(HelpMenu, sizeof(HelpMenu));
	top_right = 0;
	nGuideHelp = 0;
	GuideImage = new smMATERIAL_GROUP(1);
	nGuideImageNum = 0;
	for(int i = 0; i < 2; i++)
	{
		hClose[i] = NULL;
	}

	m_bTime = TRUE;
	m_bOnce = FALSE;
	m_dwTime = 0;
}

CHelp::~CHelp()
{
	Clear();
}

int CHelp::Init(int Life)
{
	life = Life;

	return 1;
}

int CHelp::Clear() { return TRUE; }

void CHelp::Load() {}
void CHelp::Main() {}

void CHelp::SetStartPos(int x, int y)
{
	m_Start.x = x;
	m_Start.y = y;
}


void CHelp::Draw(BOOL bFlag) {}

void CHelp::menuInit(int num) {}


void CHelp::DrawBox(int x, int y, int x_width, int y_height) {}
void CHelp::SetHelpN(int num)
{
	m_HelpN = num;
}

void CHelp::ClearImage()
{
	GuideImage->Close();
}

void CHelp::LoadGuideImage(char* file)
{
	ClearImage();
	GuideImage->Init(1);
	nGuideImageNum = GuideImage->CreateTextureMaterial(file, 0, 0, 128, 128, SMMAT_BLEND_ALPHA);
	GuideImage->ReadTextures();

}


void CHelp::GetHelpContent(char* section, char* key, char* ImageKey)
{
	DWORD dwSaveSize = 100;
	char  szSaveStr[100];
	char  szImageDir[100];
	DWORD dwReturn, dwFileSize = 0, dwRead = 0;
	HANDLE hFile = NULL;
	char *HelpContent;

	ZeroMemory(m_HelpContent, sizeof(m_HelpContent));
	ZeroMemory(szImageDir, sizeof(szImageDir));

	dwReturn = GetPrivateProfileString(section, key, "Error", szSaveStr, 1024, HELP_CONTENT);

	if(dwReturn == 0) return;
	dwReturn = GetPrivateProfileString(section, ImageKey, "Error", szImageDir, 1024, HELP_CONTENT);
	if(dwReturn == 0) return;

	LoadGuideImage(szImageDir);

	hFile = CreateFile(szSaveStr, GENERIC_READ, FILE_SHARE_READ,
					   NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile == INVALID_HANDLE_VALUE) return;

	dwFileSize = GetFileSize(hFile, NULL);

	HelpContent = new char[dwFileSize];
	ZeroMemory(HelpContent, sizeof(char)*dwFileSize);

	ReadFile(hFile, HelpContent, dwFileSize, &dwRead, NULL);
	CloseHandle(hFile);

	Parsing(HelpContent);

	delete[] HelpContent;

}

void CHelp::Parsing(char* str)
{
	int nCnt, nCnt2, nCnt3;
	nCnt = nCnt2 = nCnt3 = 0;
	nHeight_Count = 0;
	while(1)
	{
		if(str[nCnt3] == 0) break;

		if(str[nCnt3] == 0x0d)
		{
			nCnt3++;
			if(str[nCnt3] == 0) break;
			if(str[nCnt3] == 0x0a)
			{
				if(str[nCnt3 + 1] == 0x0d) break;
				else
				{
					nCnt3++;
					nCnt++;
					nCnt2 = 0;
				}
			}
			else
			{
				nCnt++;
				nCnt2 = 0;
				m_HelpContent[nCnt][nCnt2] = str[nCnt3];
			}

			continue;
		}
		else
		{
			m_HelpContent[nCnt][nCnt2] = str[nCnt3];
		}
		nCnt3++;
		nCnt2++;
	}

	nHeight_Count = nCnt + 1;
	m_bIsContentOver = FALSE;

	if(nHeight_Count > CONTENT_MAX)
	{
		m_bIsContentOver = TRUE;
		nHeight_Count = 6;
	}
}


BOOL CHelp::CheckLevel(int level, int type)
{
	BOOL IsLevel = FALSE;;
	switch(level)
	{
		case 1:
		if(type < 5)
		{

			GetHelpContent("CreateCha_Temscron", "Key0", "Guide1");
		}
		else
		{

			GetHelpContent("CreateCha_Moraion", "Key1", "Guide2");
		}
		IsLevel = TRUE;
		break;
		case 2:
		GetHelpContent("Level_2", "Key2", "Guide3");
		IsLevel = TRUE;
		break;
		case 3:
		GetHelpContent("Level_3", "Key3", "Guide4");
		IsLevel = TRUE;
		break;
		case 4:
		GetHelpContent("Level_4", "Key4", "Guide5");
		IsLevel = TRUE;
		break;
		case 5:
		GetHelpContent("Level_5", "Key5", "Guide6");
		IsLevel = TRUE;
		break;
		case 6:
		GetHelpContent("Level_6", "Key6", "Guide7");
		IsLevel = TRUE;

		break;
		case 7:
		GetHelpContent("Level_7", "Key24", "Guide26");
		IsLevel = TRUE;
		break;
		case 8:
		GetHelpContent("Level_8", "Key7", "Guide8");
		IsLevel = TRUE;
		break;
		case 9:
		GetHelpContent("Level_9", "Key25", "Guide27");
		IsLevel = TRUE;
		break;
		case 10:

		if(type < 5)
		{

			GetHelpContent("Level_10_Temscron", "Key8", "Guide9");
		}
		else
		{

			GetHelpContent("Level_10_Moraion", "Key9", "Guide10");
		}
		IsLevel = TRUE;
		break;


		case 11:
		GetHelpContent("Level_11", "Key11", "Guide11");
		IsLevel = TRUE;
		break;

		case 12:
		GetHelpContent("Level_12", "Key10", "Guide11");
		IsLevel = TRUE;
		break;
		case 15:
		GetHelpContent("Level_15", "Key11", "Guide12");
		IsLevel = TRUE;
		break;
		case 17:
		GetHelpContent("Level_17", "Key12", "Guide13");
		IsLevel = TRUE;
		break;
		case 20:

		if(type < 5)
		{
			GetHelpContent("Level_20_Temscron", "Key13", "Guide14");
		}
		else
		{
			GetHelpContent("Level_20_Moraion", "Key24", "Guide15");
		}
		IsLevel = TRUE;
		break;
		case 22:
		GetHelpContent("Level_22", "Key14", "Guide16");
		IsLevel = TRUE;
		break;
		case 25:
		GetHelpContent("Level_25", "Key15", "Guide17");
		IsLevel = TRUE;
		break;
		case 30:
		GetHelpContent("Level_30", "Key16", "Guide18");
		IsLevel = TRUE;
		break;
		case 39:
		GetHelpContent("Level_39", "Key26", "Guide26");
		IsLevel = TRUE;
		break;
		case 40:
		GetHelpContent("Level_40", "Key17", "Guide19");
		IsLevel = TRUE;
		break;
		case 50:
		GetHelpContent("Level_50", "Key18", "Guide20");
		IsLevel = TRUE;
		break;
		case 55:
		GetHelpContent("Level_55", "Key19", "Guide21");
		IsLevel = TRUE;
		break;
		case 60:
		GetHelpContent("Level_60", "Key20", "Guide22");
		IsLevel = TRUE;
		break;
		case 65:
		GetHelpContent("Level_65", "Key21", "Guide23");
		IsLevel = TRUE;
		break;
		case 70:
		GetHelpContent("Level_70", "Key22", "Guide24");
		IsLevel = TRUE;
		break;

		case 75:
		GetHelpContent("Level_75", "Key28", "Guide24");
		IsLevel = TRUE;
		break;


		case 79:
		GetHelpContent("Level_79", "Key79", "Guide79");
		IsLevel = TRUE;
		break;


		case 80:
		GetHelpContent("Level_80", "Key23", "Guide25");
		IsLevel = TRUE;
		break;
	}

	return IsLevel;

}


void CHelp::Draw_Help_Text(char* str, int x, int y, int sw) {}




int CHelp::chkMenu(int x, int y, RECT *r)
{
	if(r->left <= x   &&   x <= r->right)
		if(r->top <= y   &&   y <= r->bottom)
			return 1;
	return 0;
}
int CHelp::chkeckMenuN()
{

	return 1;
}

int CHelp::dsDrawTexImageFloat(int Mat, float x, float y, float w, float h, int Transp, int specular) { return TRUE; }

void CHelp::HelpEnd()
{
	m_HelpN = -1;
	isDrawClanMenu = 0;
}

void CHelp::Draw_Help_Text3(char* str, int x, int y, int sw) {}

void CHelp::Draw_Help_Text2(char* str, int x, int y, int sw) {}

extern BOOL IsDrawGuideHelp;

void InitGuideContentHelp()
{
	BOOL bisEqual = FALSE;
	int SaveLevel = 0;
	extern int g_LevelLabel[];
	for(int i = 0; g_LevelLabel[i] != 0; i++)
	{
		if(g_LevelLabel[i] == sinChar->Level)
		{
			bisEqual = TRUE;
			SaveLevel = i;
			break;
		}
	}
	if(!bisEqual) return;
	g_GuideHelp.SetDoing(TRUE);
	g_GuideHelp.SetStartPos(0, 0);
	g_GuideHelp.CheckLevel(sinChar->Level);


	g_GuideHelp.CheckLevel(g_LevelLabel[SaveLevel], sinChar->JOB_CODE);
	g_GuideHelp.SetHelpN(GUIDEHELP_CONTENT);
	g_GuideHelp.menuInit(GUIDEHELP_CONTENT);

	IsDrawGuideHelp = TRUE;
}

int CHelp::haGuidHelpLevel_1(DWORD Code)
{
	if((Code&sinITEM_MASK2) == sinWA1 || (Code&sinITEM_MASK2) == sinWC1 || (Code&sinITEM_MASK2) == sinWH1 || (Code&sinITEM_MASK2) == sinWM1 ||
		(Code&sinITEM_MASK2) == sinWP1 || (Code&sinITEM_MASK2) == sinWS1 || (Code&sinITEM_MASK2) == sinWS2 || (Code&sinITEM_MASK2) == sinWT1)
	{
		haGuidHelpLevel_1Flag = 1;
	}
	else
		haGuidHelpLevel_1Flag = 2;

	return TRUE;
}