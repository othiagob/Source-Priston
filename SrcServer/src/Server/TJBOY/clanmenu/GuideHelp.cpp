#define GUIDEHELP_CPP

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <stdio.h>
#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		
#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"

#include "cE_CViewClanInfo.h"
#endif

#include "tjscroll.h"
#include "tjclan.h"
#include "cE_Notice.h"
#include "GuideHelp.h"

extern int GuideHelpButton[3];
extern BOOL IsDrawGuideHelp;


int g_LevelLabel[] = { 1,2,3,4,5,6,7,8,9,10,11,12,15,17,20,22,25,30,39,40,50,55,60,65,70,75,79,80 };
int g_MorianLabel[] = { 1,2,3,4,5,6,7,8,9,10,11,12,15,17,20,22,25,30,39,40,50,55,60,65,70,75,79,80 };
int nStrLen[] = { 178,102,92,56,108,72,102,56,204,216,168,216,118,246,196,112,224,174,112,148,128,128 };

POINT ScrollSavePoint;

extern BOOL bKeyDown;
extern int nKeyDown;
extern BOOL bKeyUp;
extern int nKeyUp;
extern BOOL bLButtonDown;

CGuideHelp::CGuideHelp()
{
	m_GuideHelpN = -1;
	nQuetion_Icon = 0, nQuetion_Icon1 = 0;
	nHelp = nHelpSee = nQuideHelp = nDuru = nDuru1 = 0;
	hOff = NULL;
	hOn = NULL;
	bIsOn = TRUE;
	nLevelHelpCount = 0;
	ZeroMemory(LevelHelp, sizeof(LevelHelp));
	ZeroMemory(bHelpIcon, sizeof(bHelpIcon));
	bDoing = FALSE;
	nHelpCountNamugi = 0;

	GuideImage = new smMATERIAL_GROUP(1);
	nGuideImageNum = 0;

}

CGuideHelp::~CGuideHelp()
{
	delete GuideImage;
	Clear();
}

void CGuideHelp::ClearImage()
{
	GuideImage->Close();
}

void CGuideHelp::LoadGuideImage(char* file)
{
	ClearImage();
	GuideImage->Init(1);
	nGuideImageNum = GuideImage->CreateTextureMaterial(file, 0, 0, 128, 128, SMMAT_BLEND_ALPHA);
	GuideImage->ReadTextures();

}

int CGuideHelp::Init(int Life)
{
	life = Life;

	return 1;
}

int CGuideHelp::Clear()
{

	return 1;
}


int nPos = 0;
void CGuideHelp::Main()
{
	int i;

	if(!bDoing) return;
	switch(m_GuideHelpN)
	{
		case GUIDEHELP_LIST:
		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;
			case 2:
			menuMouseOver[2] = 1;
			break;
			case 5:
			menuMouseOver[2] = 1;
			break;
			case 12:
			case 13:
			case 14:
			case 15:
			case 16:
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			if(!bHelpIcon[i + nScl_Bar - 12])
			{
				ZeroMemory(bHelpIcon, sizeof(bHelpIcon));
				bHelpIcon[i + nScl_Bar - 12] = !bHelpIcon[i + nScl_Bar - 12];
			}
			break;
			default:
			ZeroMemory(bHelpIcon, sizeof(bHelpIcon));
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}
		if(TJwheel != 0)
		{
			scrIconPos.top = tscr_Guide.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}

		if(GuideHelpButton[0])
		{


			if(bKeyUp || bKeyDown)
			{
				i = 11;
			}
			switch(i)
			{
				case 11:
				if(bKeyDown)
				{
					ScrollSavePoint.y += 10;
					if(ScrollSavePoint.y > menuPos[11].top + 246)
					{
						ScrollSavePoint.y = menuPos[11].top + 246;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(ScrollSavePoint.y);
					if(nHelpCountNamugi == nScl_Bar)
					{

						scrIconPos.top = menuPos[11].top + 246;
						ScrollSavePoint.y = scrIconPos.top;
					}
					else
					{

						scrIconPos.top = ScrollSavePoint.y - 8;
					}
					GuideHelpButton[0] = 0;
					break;
				}
				if(bKeyUp)
				{
					ScrollSavePoint.y -= 10;
					if(ScrollSavePoint.y < menuPos[11].top)
					{
						ScrollSavePoint.y = menuPos[11].top;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(ScrollSavePoint.y);

					scrIconPos.top = ScrollSavePoint.y - 8;
					GuideHelpButton[0] = 0;

					break;
				}
				bLButtonDown = TRUE;
				break;

				case 0:
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				bDoing = FALSE;
				isDrawClanMenu = 0;
				GuideHelpButton[0] = 0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				break;
				case 5:
				case 2:
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				isDrawClanMenu = 0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				GuideHelpButton[0] = 0;
				break;
				case 9:
				bIsOn = !bIsOn;
				GuideHelpButton[0] = 0;
				break;

				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 17:
				case 18:
				case 19:
				case 20:
				case 21:
				case 22:
				case 23:
				if(!bLButtonDown)
				{
					nPos = (i + nScl_Bar - 12);
					if(sinChar->JOB_CODE < 5)
					{
						CheckLevel(g_LevelLabel[nLevelHelpCount - nPos - 1], sinChar->JOB_CODE);
					}
					else
					{

						CheckLevel(g_MorianLabel[nLevelHelpCount - nPos - 1], sinChar->JOB_CODE);
					}

					m_GuideHelpN = GUIDEHELP_CONTENT;
					menuInit(m_GuideHelpN);
					GuideHelpButton[0] = 0;
				}
				break;
				default:
				break;
			}
		}

		break;
		case GUIDEHELP_CONTENT:
		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;
			case 2:
			menuMouseOver[2] = 1;
			break;
			case 5:
			menuMouseOver[2] = 1;
			break;
			case 10:
			case 11:
			bHelpIcon[10] = 1;
			break;
			default:
			ZeroMemory(bHelpIcon, sizeof(bHelpIcon));
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}

		if(TJwheel != 0)
		{
			scrIconPos.top = tscr_Guide.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}


		if(GuideHelpButton[0])
		{


			if(bKeyUp || bKeyDown)
			{
				i = 15;
			}
			switch(i)
			{
				case 0:
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				bDoing = FALSE;
				isDrawClanMenu = 0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				GuideHelpButton[0] = 0;
				break;
				case 5:
				case 2:
				IsDrawGuideHelp = FALSE;
				m_GuideHelpN = -1;
				isDrawClanMenu = 0;
				TJBscrollWheelClear();
				TJBscrollWheelClear_Guide();
				GuideHelpButton[0] = 0;
				break;
				case 9:
				bIsOn = !bIsOn;
				GuideHelpButton[0] = 0;
				break;
				case 6:
				case 7:
				case 11:
				case 10:
				SetDoing(TRUE);
				CheckLevel(sinChar->Level);
				SetHelpN(GUIDEHELP_LIST);
				menuInit(GUIDEHELP_LIST);
				IsDrawGuideHelp = TRUE;
				GuideHelpButton[0] = 0;
				break;
				case 15:
				if(bKeyDown)
				{

					scrIconPos.top += 10;
					if(scrIconPos.top > menuPos[15].top + 246)
					{
						scrIconPos.top = menuPos[15].top + 246;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);


					GuideHelpButton[0] = 0;
					break;
				}
				if(bKeyUp)
				{

					scrIconPos.top -= 10;
					if(scrIconPos.top < menuPos[15].top)
					{
						scrIconPos.top = menuPos[15].top;
					}
					nScl_Bar = tscr_Guide.topScrollLineNum(scrIconPos.top);


					GuideHelpButton[0] = 0;

					break;
				}
				bLButtonDown = TRUE;

				break;
				default:
				break;
			}
		}

		break;
	}
}



void CGuideHelp::Draw() {}



void CGuideHelp::Load() {}




void CGuideHelp::menuInit(int num) {}

void CGuideHelp::SetHelpN(int num)
{
	m_GuideHelpN = num;
}

void CGuideHelp::SetStartPos(int x, int y)
{
	m_Start.x = x;
	m_Start.y = y;
}

void CGuideHelp::InitLevelHelpLabel() {}



void CGuideHelp::CheckLevel(int level)
{
	if(level == 1)
		nLevelHelpCount = 1;

	if(level == 2)
		nLevelHelpCount = 2;

	if(level == 3)
		nLevelHelpCount = 3;

	if(level == 4)
		nLevelHelpCount = 4;

	if(level == 5)
		nLevelHelpCount = 5;

	if(level == 6)
		nLevelHelpCount = 6;

	if(level == 7)
		nLevelHelpCount = 7;

	if((level == 8) || (level == 9))
		nLevelHelpCount = 8;

	if(level == 9)
		nLevelHelpCount = 9;

	if(level == 10)
		nLevelHelpCount = 10;

	if(level == 11)
		nLevelHelpCount = 11;

	if((level >= 12) && (level < 15))
		nLevelHelpCount = 12;

	if((level == 15) || (level == 16))
		nLevelHelpCount = 13;

	if((level >= 17) && (level < 20))
		nLevelHelpCount = 14;

	if((level == 20) || (level == 21))
	{

		nLevelHelpCount = 15;
	}
	if((level >= 22) && (level < 25))
		nLevelHelpCount = 16;

	if((level >= 25) && (level < 30))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 17;
		}
		else
		{
			nLevelHelpCount = 17;
		}
	}
	if((level >= 30) && (level < 40))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 18;
		}
		else
		{
			nLevelHelpCount = 18;
		}
	}

	if((level >= 39))
		nLevelHelpCount = 19;

	if((level >= 40) && (level < 50))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 20;
		}
		else
		{
			nLevelHelpCount = 20;
		}
	}
	if((level >= 50) && (level < 55))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 21;
		}
		else
		{
			nLevelHelpCount = 21;
		}
	}
	if((level >= 55) && (level < 60))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 22;
		}
		else
		{
			nLevelHelpCount = 22;
		}
	}
	if((level >= 60) && (level < 65))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 23;
		}
		else
		{
			nLevelHelpCount = 23;
		}
	}
	if((level >= 65) && (level < 70))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 24;
		}
		else
		{
			nLevelHelpCount = 24;
		}
	}


	if((level >= 70) && (level < 75))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 25;
		}
		else
		{
			nLevelHelpCount = 25;
		}
	}

	if((level >= 75) && (level < 79))
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 26;
		}
		else
		{
			nLevelHelpCount = 26;
		}
	}

	if(level == 79)
		nLevelHelpCount = 27;

	if(level >= 80)
	{
		if(sinChar->JOB_CODE < 5)
		{
			nLevelHelpCount = 28;
		}
		else
		{
			nLevelHelpCount = 28;
		}
	}


	if(nLevelHelpCount > LEVELHELP_COUNT_MAX)
	{
		nHelpCountNamugi = nLevelHelpCount - LEVELHELP_COUNT_MAX;
	}
}

void CGuideHelp::GetHelpContent(char* section, char* key, char* ImageKey)
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

void CGuideHelp::Parsing(char* str)
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
	if(nHeight_Count > GUIDEHLEP_CONTENT_HEIGHT_MAX)
		nSaveNamugi = nHeight_Count - GUIDEHLEP_CONTENT_HEIGHT_MAX;



}

void CGuideHelp::CheckLevel(int level, int type)
{

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
		break;
		case 2:
		GetHelpContent("Level_2", "Key2", "Guide3");

		break;
		case 3:
		GetHelpContent("Level_3", "Key3", "Guide4");

		break;
		case 4:
		GetHelpContent("Level_4", "Key4", "Guide5");
		break;
		case 5:
		GetHelpContent("Level_5", "Key5", "Guide6");
		break;
		case 6:
		GetHelpContent("Level_6", "Key6", "Guide7");

		break;
		case 7:
		GetHelpContent("Level_7", "Key24", "Guide26");

		break;
		case 8:
		GetHelpContent("Level_8", "Key7", "Guide8");
		break;
		case 9:
		GetHelpContent("Level_9", "Key25", "Guide27");
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
		break;


		case 11:
		GetHelpContent("Level_11", "Key11", "Guide11");
		break;

		case 12:

		GetHelpContent("Level_12", "Key10", "Guide11");
		break;
		case 15:

		GetHelpContent("Level_15", "Key11", "Guide12");
		break;
		case 17:
		GetHelpContent("Level_17", "Key12", "Guide13");
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
		break;
		case 22:

		GetHelpContent("Level_22", "Key14", "Guide16");
		break;
		case 25:

		GetHelpContent("Level_25", "Key15", "Guide17");
		break;
		case 30:

		GetHelpContent("Level_30", "Key16", "Guide18");
		break;
		case 39:
		GetHelpContent("Level_39", "Key26", "Guide26");
		break;
		case 40:

		GetHelpContent("Level_40", "Key17", "Guide19");
		break;
		case 50:

		GetHelpContent("Level_50", "Key18", "Guide20");
		break;
		case 55:
		GetHelpContent("Level_55", "Key19", "Guide21");
		break;
		case 60:
		GetHelpContent("Level_60", "Key20", "Guide22");
		break;
		case 65:

		GetHelpContent("Level_65", "Key21", "Guide23");
		break;
		case 70:

		GetHelpContent("Level_70", "Key22", "Guide24");
		break;

		case 75:
		GetHelpContent("Level_75", "Key28", "Guide24");
		break;

		case 79:
		GetHelpContent("Level_79", "Key79", "Guide79");
		break;

		case 80:
		GetHelpContent("Level_80", "Key23", "Guide25");
		break;
	}

}



void CGuideHelp::Draw_Help_Text(char* str, int x, int y, int sw) {}

void CGuideHelp::Draw_Help_Text2(char* str, int x, int y, int sw) {}

void CGuideHelp::Draw_Title_Text(char* str, int x, int y, int sw) {}




int CGuideHelp::dsDrawTexImageFloat(int Mat, float x, float y, float w, float h, int Transp, int specular) { return TRUE; }

void CGuideHelp::GuidHelpEnd() {}


