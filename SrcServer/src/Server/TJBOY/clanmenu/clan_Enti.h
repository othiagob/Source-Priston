#pragma once
#ifndef __CLAN_ENTI_HPP__
#define __CLAN_ENTI_HPP__




class CLANEntity
{
public:
	int				life;
	CLANEntity      *parent;


public:
	CLANEntity();
	~CLANEntity();

	virtual int Init();
	virtual int Clear();


	virtual void Main();
	virtual void RenderMain();


	void	setParent(CLANEntity      *p)
	{
		parent = p;
	}
	void			Kill()
	{
		life = 0;
	};
	int				IsDead()
	{
		return(life);
	};


	virtual void Draw();

	virtual void menuInit(int num);


	void Draw_C_Text(char *str, int x, int y, int sw = 0);
	void Draw_C_Text2(char *str, int x, int y, int sw = 0);
	void Draw_C_Text3(char *str, int x, int y, int sw = 0);



	void Draw_C_TextPark(char *str, int x, int y, int RedColor, int GreenColor, int BlueColor, int sw);







	int  chkMenu(int x, int y, RECT *r);
	int  chkeckMenuN();




	void ParsingIsClanMember(char* data);
	void ParsingIsClanMember2(char *data);
	int chk_readStrBuf(char *cmd, char *readbuf, char *retBuf);
	void ParsingData(char* cmd, char* data, char* save);
	void ParsingMakeClanMoney(char* data, int* money);


	void ParsigClanWon(char* data);

	void ParsingClanWonStrLen();

	void LoadToFile();



	void LoadHelp(char *help);
	void DestroyClanHelpIcon();
	void LoadHelpImage();
	BOOL ReadClanHelpScr(char *filename);
	void ReMoveClanWon(char *src);

	void WriteMyClanInfo();
	int CheckMark(BOOL bFlag);

	void CheckClanExp(char* byte);






	void Load();
	void Load2();

	virtual void Close();
	void copy_LoadResource(CLANEntity *copyDest);
	void copy_LoadResource2(CLANEntity *copyDest);

	inline void SetBase(CLANEntity *copyDest)
	{
		parent = copyDest;
	}

#ifdef USE_PROSTONTALE		
#else
	inline HBITMAP GetClanMark()
	{
		return parent->hMark;
	}
#endif


#ifdef USE_PROSTONTALE		

	int	MatPartyBackGround;
	int	hBoxCorner, hBoxLine;
	int	hBox1, hBox2, hBox3, hBox4, hBox6, hBox7, hBox8, hBox9;
	int	hScl_Bar_tj;
	int	hScl_icon_tj;
	int   hLine;





	void*	hBt_Help;
	void*	hMark_box;
	void*	hSecession[2];
	void*	hClose[2];
	void*	hIcon;
	void*	hIcon_Click;
	void*	hHelp1;
	void*	hClinetMark;
	void*	hServerMark;
	void*	hWareHouse[2];
	void*	hWHRight[2];
	void*	hClanMark[2];
	void*	hRemove[2];
	void*	hClanRelease[2];
	void*	hHelp[2];
	void*	hOut[2];

	void*	hButton_Box;
	void*	hClanTitle;
	void*	hRemoveToMenu[2];
	void*	hScl_Bar;
	void*	hScl_icon;
	void*	hClanHelpIcon[CLANHELPICON_NUM][4];
	void*    hClanNameInput;

	void*   hTxt_Remove[2];
	void*   hTxt_Release[2];
	void*   hTxt_Mark[2];
	void*   hTxt_Help[2];
	void*   hTxt_Secession[2];
	void*   hTxt_LeaveClan[2];
	void*   hTxt_SubChip[2];
	void*   hSubChip;




	char		szHelpMsg1[HELPMSG_MAX];
	char		szErrorMsg[256];

#else

	HBITMAP	hLine;

	HBITMAP	hBox1, hBox2, hBox3, hBox4, hBox6, hBox7, hBox8, hBox9;
	HBITMAP	MatPartyBackGround;
	HBITMAP	hMark_box;
	HBITMAP	hBt_Help;

	HBITMAP	hOk_Gray, hOk_Yellow;
	HBITMAP	hCancel_Gray, hCancel_Yellow;
	HBITMAP	hYes_Gray, hYes_Yellow;
	HBITMAP	hNo_Gray, hNo_Yellow;

	HBITMAP	hMark, hClanName;
	HBITMAP	hSecession[2];
	HBITMAP	hClose[2];
	HBITMAP	hIcon;
	HBITMAP   hIcon_Click;
	HBITMAP	hHelp1;
	HBITMAP	hClinetMark;
	HBITMAP	hServerMark;
	HBITMAP	hWareHouse[2];
	HBITMAP	hWHRight[2];
	HBITMAP	hClanMark[2];
	HBITMAP	hRemove[2];
	HBITMAP	hClanRelease[2];
	HBITMAP	hHelp[2];
	HBITMAP	hOut[2];
	HBITMAP	hButton_Box;
	HBITMAP	hClanTitle;
	HBITMAP	hRemoveToMenu[2];
	HBITMAP	hScl_Bar;
	HBITMAP	hScl_icon;
	HBITMAP   hClanHelpIcon[CLANHELPICON_NUM][4];

	HBITMAP   hTxt_Remove[2];
	HBITMAP   hTxt_Release[2];
	HBITMAP   hTxt_Mark[2];
	HBITMAP   hTxt_Help[2];
	HBITMAP   hTxt_Secession[2];
	HBITMAP   hTxt_LeaveClan[2];
	HBITMAP   hTxt_SubChip[2];
	HBITMAP   hSubChip;

	HBITMAP    hClanNameInput;

	HWND		m_hClanName;
	char		szErrorMsg[256];

#endif
	BOOL bImageDown;
};

#endif 





#if defined (CLAN_EITI_CPP)


class CLANEntity  cBaseMenu;

#else

extern class CLANEntity  cBaseMenu;


#endif




