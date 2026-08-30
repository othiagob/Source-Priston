#define CE_NOTICE_CPP
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
#include "tjscroll.h"
#include "cE_report.h"
#include "cE_CViewClanInfo.h"
#include "..\\isaocheck\\auth.h"
#include "..\\isaocheck\\isaocheck.h"
#include "cE_Notice.h"

extern int m_nNoticeCount;
extern int GoNotice_chking;
int nHeight_Count[NOTICECOUNT] = { -1, };

CLANEntity_Notice::CLANEntity_Notice()
{
	ZeroMemory(szNotice, sizeof(szNotice));

	nScl_Bar = 0;



}

CLANEntity_Notice::~CLANEntity_Notice()
{
}


int CLANEntity_Notice::Init(int Life)
{
	life = Life;
	return 1;
}
int CLANEntity_Notice::Clear()
{
	return 1;
}

void CLANEntity_Notice::Main()
{
	int i;
	switch(C_menuN)
	{

		case CLAN_DRAWNOTICE_END:
		--m_nNoticeCount;
		if(m_nNoticeCount > -1)
		{
			OpenFlag = 0;

			C_menuN = CLAN_DRAWNOTICE;
			menuInit(C_menuN);
			C_menuN2 = 0;
			break;
		}

		extern BOOL bip_port_error;
		if(bip_port_error == 1)
		{
			OpenFlag = 0;
			C_menuN = 0;
			C_menuN2 = 0;

			ClanCharMove = 0;
			ClanCLCharMove = 0;

			void TJBscrollWheelClear();
			TJBscrollWheelClear();
			isDrawClanMenu = 0;
		}
		else
		{
			C_menuN = CLAN_AllEnd;
			OpenFlag = 0;
			C_menuN = 0;
			C_menuN2 = 0;

			ClanCharMove = 0;
			ClanCLCharMove = 0;

			void TJBscrollWheelClear();
			TJBscrollWheelClear();
			isDrawClanMenu = 0;
		}
		break;




		case CLAN_DRAWNOTICE:











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



		if(TJwheel != 0)
		{
			scrIconPos.top = tscr.ret_WHEELpoint(TJwheel, scrIconPos.top);
			nScl_Bar = tscr.topScrollLineNum(scrIconPos.top);
			TJwheel = 0;
		}

		
		{
			
			i = i;
			switch(i)
			{
				case 0:
				C_menuN = CLAN_DRAWNOTICE_END;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;
				break;
				case 5:

				break;
				default:
				i = i;
				break;

			}
		}
		break;
		case CLAN_NOTICE:
		switch(C_menuN2)
		{
			case 0:
			ANSdata.ret_val = -1;

			WebDB.Resume();
			WebDB.ReadNotice(cldata.szID, cldata.ChaName, cldata.gserver);
			C_menuN2 = 1;
			break;
			case 1:

			if(ANSdata.ret_val != -1)
			{
				ANSdata.ret_val = ANSdata.ret_val;
			}

			switch(ANSdata.ret_val)
			{

				case 0:
				OpenFlag = 0;
				WebDB.Suspend();
				C_menuN = CLAN_DRAWNOTICE_END;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;







				break;
				case 1:
				OpenFlag = 0;
				WebDB.Suspend();
				NoticeCheck(ANSdata.ret_Buf);




				C_menuN = CLAN_DRAWNOTICE;
				menuInit(C_menuN);
				break;

				case 100:
				OpenFlag = 0;
				WebDB.Suspend();
				C_menuN = CLAN_DRAWNOTICE_END;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;









				break;
				case ISAO_ERR_TCP_CONNECT:
				OpenFlag = 0;
				WebDB.Suspend();
				C_menuN = CLAN_DRAWNOTICE_END;
				C_menuN2 = -1;
				CLAN_ERROR_ret = -1;





				break;


				default:
				if(ANSdata.ret_val != -1)
				{
					OpenFlag = 0;
					WebDB.Suspend();
					C_menuN = CLAN_DRAWNOTICE_END;
					C_menuN2 = -1;
					CLAN_ERROR_ret = -1;







				}
				break;
			}
			break;
		}


		break;
	}

}

void CLANEntity_Notice::Draw() {}


void CLANEntity_Notice::menuInit(int num) {}

#define MAX_CHACOUNT 58
void CLANEntity_Notice::NoticeCheck(char* buf)
{
	int nStrlen = 0, i = 0, nCount = 0, nCnt = 0;
	nHeight_Count[m_nNoticeCount] = 0;


	buf = strstr(buf, "Txt=");
	buf = &buf[lstrlen("Txt=")];
	nStrlen = lstrlen(buf);


	ZeroMemory(szNotice[m_nNoticeCount], sizeof(szNotice));

	if(nStrlen > NOTICE_MAX)
	{
		nStrlen = NOTICE_MAX;
	}
	while(nHeight_Count[m_nNoticeCount] < NOTICE_HEIGHT)
	{
		if(buf[i] == 0)
		{
			break;
		}

		if(buf[i] == '\r')
		{
			if(buf[i + 1] == '\n')
			{
				nCount = 0;
				szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0;
				++nCount;
				if(i != 0)
				{
					nHeight_Count[m_nNoticeCount]++;
				}

				nCnt = 0;
				i++;
				i++;
				continue;

			}
			else
			{
				nCount = 0;
				++nCount;
				if(i != 0)
				{
					nHeight_Count[m_nNoticeCount]++;
				}
				nCnt = 0;
				i++;
				continue;
			}

		}
		else
		{
			if(IsDBCSLeadByte(buf[i]))
			{
				szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = buf[i];
				nCnt += 1;
				i += 1;
				szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = buf[i];
				nCount += 2;;
			}
			else
			{






				if(buf[i] == 0x09)
				{
					szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
					nCnt++;
					szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
					nCnt++;
					szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
					nCnt++;
					szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = 0x20;
				}
				else
				{
					szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = buf[i];
				}
				++nCount;


			}


		}
		if(nCount >= (MAX_CHACOUNT))
		{
			nCount = 0;
			nCnt += 1;
			szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt] = '\n';
			szNotice[m_nNoticeCount][nHeight_Count[m_nNoticeCount]][nCnt + 1] = 0;

			nHeight_Count[m_nNoticeCount]++;
			nCnt = 0;
			i++;
			continue;
		}

		nCnt++;
		i++;

	}
}



 