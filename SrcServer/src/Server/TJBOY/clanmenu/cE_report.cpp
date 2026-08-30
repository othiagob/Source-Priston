
#define CE_REPORT_CPP
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <math.h>
#include "tjclanDEF.h"

#ifdef USE_PROSTONTALE		

#include "..\\..\\sinbaram\\SinLinkHeader.h"

#include "..\\..\\language.h"

#endif

#include "tjclan.h"
#include "cE_report.h"


CLANEntity_report::CLANEntity_report()
{
	ZeroMemory(szReport, sizeof(char)*REPORT_MAX);
}

CLANEntity_report::~CLANEntity_report()
{

}



int CLANEntity_report::Init(int Life)
{
	life = Life;
	return 1;
}

int CLANEntity_report::Clear()
{
	return 1;
}



void CLANEntity_report::Main()
{
	int i;
	if(life == 0) return;

	switch(C_menuN)
	{



		case CLAN_ERROR:

		i = chkeckMenuN();
		switch(i)
		{
			case 0:
			menuMouseOver[0] = 1;
			break;
			default:
			memset(menuMouseOver, -1, sizeof(int) * 30);
			break;
		}
		
		{
			
			i = i;
			switch(i)
			{
				case 0:
				C_menuN = CLAN_AllEnd;



				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				default:
				i = i;
				break;

			}
		}
		break;


	}


}




void CLANEntity_report::RenderMain()
{
	if(life == 0) return;

}





void CLANEntity_report::menuInit(int num, LONG x, LONG y)
{
	int bx = 0, by = 0;
#ifdef USE_PROSTONTALE		
#else
	BITMAP	bit;
#endif

	switch(num)
	{
		case CLAN_ERROR:
		menuSu = 3;
	#ifdef USE_PROSTONTALE		
		bx = 32;
		by = 16;

	#else
		bx = 32;
		by = 16;
	#endif
		menuPos[0].left = BackStartPos.x + BUTTON1_X;
		menuPos[0].top = BackStartPos.y + BUTTON1_Y;
		menuPos[0].right = menuPos[0].left + bx;
		menuPos[0].bottom = menuPos[0].top + by;


		menuPos[1].left = BackStartPos.x + (int)x;
		menuPos[1].top = BackStartPos.y + (int)y;

	#ifdef USE_PROSTONTALE		
		bx = 64;
		by = 16;
	#else
		GetObject(hButton_Box, sizeof(BITMAP), &bit);
		bx = bit.bmWidth;
		by = bit.bmHeight;
	#endif
		menuPos[2].left = BackStartPos.x + BUTTON1_BOX_X;
		menuPos[2].top = BackStartPos.y + BUTTON1_BOX_Y;
		menuPos[2].right = menuPos[2].left + bx;
		menuPos[2].bottom = menuPos[2].top + by;

		break;
	}
}

void CLANEntity_report::InitMsg(char *msg)
{
	strcpy(szReport, msg);
}

void CLANEntity_report::Draw() {}


