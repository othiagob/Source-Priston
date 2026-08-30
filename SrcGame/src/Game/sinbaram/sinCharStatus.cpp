#include "..\globals.h"
#include "sinLinkHeader.h"
#include "..\\tjboy\\clanmenu\\tjclanDEF.h"
#include "..\\tjboy\\clanmenu\\tjclan.h"
#include "..\\tjboy\\clanmenu\\cE_CViewClanInfo.h"

cCHARSTATUS cCharStatus;
sDISPLAYSTATE sDisplayState[10];
sDISPLAYSTATE sDisplayStateShow;


int OldLevel;

int OldLevelTemp;

int sinLevelPoint = 0;

int sinPointButtonDown = 0;

int ShowCharArrow = 0;

int RegiInfoPosi = 0;

int ReHeight3 = 2;

sCHARRECTPOSI	sCharRectPosi[MAX_CHAR_RECT_POSI] = {

	{"Class"    ,{ 96   , 446 + ReHeight3  , 160 , 454  }},
	{"Name"     ,{ 82   , 466 + ReHeight3  , 160 , 479  }},
	{"Level"    ,{ 108  , 506 + ReHeight3  , 128 , 504  }},
	{"Exp"      ,{ 80   , 545 + ReHeight3  , 160 , 549  }},
	{"Next_Exp" ,{ 80   , 565 + ReHeight3  , 160 , 574  }},

	{"Life"		,{ 356  , 426 + ReHeight3  , 470 , 440  }},
	{"Mana"     ,{ 356  , 451 + ReHeight3  , 470 , 466  }},
	{"Stamina"  ,{ 356  , 476 + ReHeight3  , 470 , 490  }},

	{"Strength" ,{ 585 , 431 + ReHeight3 , 632 , 444  }},
	{"Spirit"   ,{ 585 , 452 + ReHeight3 , 632 , 464  }},
	{"Talent"   ,{ 585 , 472 + ReHeight3 , 632 , 485  }},
	{"Dexterity",{ 585 , 492 + ReHeight3 , 632 , 504  }},
	{"Health"   ,{ 585 , 512 + ReHeight3 , 632 , 524  }},

	{"Bionic"   ,{ 184 + 3 , 433 + 8 + ReHeight3 , 248  , 468 }},
	{"Poison"   ,{ 204 + 3 , 433 + 8 + ReHeight3 , 315  , 469 }},
	{"Fire"     ,{ 300 + 3 , 433 + 8 + ReHeight3 , 315  , 469 }},
	{"Lighting" ,{ 184 + 3 , 455 + 8 + ReHeight3 , 248  , 468 }},
	{"Ice"      ,{ 204 + 3 , 455 + 8 + ReHeight3 , 315  , 469 }},

	{"Attack_Rating" ,{ 715 , 455 + ReHeight3 - 1 , 769 , 464 }},
	{"Attack_Damage" ,{ 715 , 474 + ReHeight3 - 1 , 769 , 484 }},
	{"Defence"       ,{ 715 , 495 + ReHeight3 - 1 , 769 , 504 }},
	{"Speed"         ,{ 715 , 515 + ReHeight3 - 1 , 769 , 524 }},
	{"Absorption"    ,{ 715 , 534 + ReHeight3 - 1 , 769 , 545 }},
	{"Point"         ,{ 585 , 532 + ReHeight3 - 1 , 632 , 544 }},

	{"LifeRegen"     ,{ 471 , 426 + ReHeight3  , 470 , 440  }},
	{"ManaRegen"     ,{ 471 , 451 + ReHeight3  , 470 , 440  }},
	{"StaminaRegen"  ,{ 471 , 476 + ReHeight3  , 470 , 440  }},

};

int RegiBox[5][4] = {
	{ 185 , 445 , 216  , 475 },
	{ 185 , 482 , 216  , 512 },
	{ 185 , 519 , 216  , 550 },
	{ 251 , 445 , 281  , 475 },
	{ 251 , 482 , 281  , 512 },
};

int PointButton[6][4] = {
	{565 + 1,431 + 1,583,449},
	{565 + 1,452 + 1,583,469},
	{565 + 1,472 + 1,583,488},
	{565 + 1,491 + 1,583,508},
	{565 + 1,511 + 1,583,529},
	{565 + 1,531 + 1,583,549},
};

int TempStatePoint[5] = { 0,0,0,0,0 };

int PointButtonPosi = 0;

LPDIRECT3DTEXTURE9	lpStatusMain;
int MatChatBotton_Glay = 0;
LPDIRECT3DTEXTURE9 lpChatButton[5];
LPDIRECT3DTEXTURE9 lpChatButton_Notice[3];
LPDIRECT3DTEXTURE9 lpChatLine;
// scroll chat xxstr
LPDIRECT3DTEXTURE9 lpChatScrol;
//LPDIRECT3DTEXTURE9 lpChatScrolIcon;
int lpChatScrolIcon = 0;
int NoticeChatIndex = 0;
int NoticeChatIndexNum = 0;
int ChatKindIndex = 0;
int ChatKindIndexNum = 0;

int sinMatMike = 0;
//int lpChatScrolIcon = 0;

cCHARSTATUS::cCHARSTATUS()
{
	OpenFlag = 0;
}

cCHARSTATUS::~cCHARSTATUS()
{

}

int chatico, chatbar;
void cCHARSTATUS::Init()
{

	//MatStatus[0] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-1.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatStatus[1] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-2.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatStatus[2] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-3.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );
	//MatStatus[3] =  CreateTextureMaterial( "Image\\SinImage\\Status\\Status-4.tga" , 0, 0, 0,0, SMMAT_BLEND_ALPHA );

	Load();

	SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_WHITE;
	SkillTextColor[SIN2_SPEED] = SIN_TEXT_COLOR_WHITE;


	MatChatBotton_Glay = CreateTextureMaterial("Image\\SinImage\\Inter\\Chat\\Chatting06.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	sinMatMike = CreateTextureMaterial("Image\\SinImage\\Inter\\mike.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	chatico = CreateTextureMaterial("Image\\SinImage\\help\\scl_icon.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	chatbar = CreateTextureMaterial("Image\\SinImage\\help\\scl_bar.tga", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

}
void cCHARSTATUS::Load()
{
	lpRegiBox[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\BioInfo.bmp");
	lpRegiBox[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\FireInfo.bmp");
	lpRegiBox[2] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\IceInfo.bmp");
	lpRegiBox[3] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\PoisonInfo.bmp");
	lpRegiBox[4] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\RegiInfo\\LightInfo.bmp");


	lpChangeArrow[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\ChangeArrow.bmp");
	lpSelectArrow[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\SelectArrow.bmp");
	lpSelectArrow[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\SelectArrow2.bmp");
	lpStatusPoint = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\StatusPoint.bmp");

	lpStatusMain = LoadDibSurfaceOffscreen("Image\\SinImage\\Status\\Status.bmp");

	lpChatButton[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_01.bmp");
	lpChatButton[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_02.bmp");
	lpChatButton[2] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_03.bmp");
	lpChatButton[3] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_04.bmp");
	lpChatButton[4] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_05.bmp");

	lpChatButton_Notice[0] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_N11.bmp");
	lpChatButton_Notice[1] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_N12.bmp");
	lpChatButton_Notice[2] = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ChatTap_N22.bmp");

	// scroll chat xxstr
	lpChatScrol = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ScrollBar.bmp");
	//lpChatScrolIcon = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\ScrollBar_Button.bmp");

	lpChatLine = LoadDibSurfaceOffscreen("Image\\SinImage\\Inter\\Chat\\n_line.bmp");

}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/
void cCHARSTATUS::Release()
{
	int i;
	for (i = 0; i < 5; i++) {
		if (lpRegiBox[i]) {
			lpRegiBox[i]->Release();
			lpRegiBox[i] = 0;

		}

	}
	if (lpChangeArrow[0]) {
		lpChangeArrow[0]->Release();
		lpChangeArrow[0] = 0;
	}
	if (lpSelectArrow[0]) {
		lpSelectArrow[0]->Release();
		lpSelectArrow[0] = 0;
	}
	if (lpSelectArrow[1]) {
		lpSelectArrow[1]->Release();
		lpSelectArrow[1] = 0;
	}
	if (lpStatusPoint) {
		lpStatusPoint->Release();

	}

}

extern BOOL bMic;

void cCHARSTATUS::Draw()
{
	int TempLen = 0;
	int TempCount2 = 0;

	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	// Render 2d aqui
	GAMECOREHANDLE->Render2D();

	//Chat antigo

	/*
	if(sinChatEnter){
		if(SubChatHeight){
			dsDrawTexImage( cInterFace.MatChatDoc[0] , 3, 576 - SubChatHeight, 256, 32 , 255 );    //Ã¤ÆÃÃ¢³»¿ë
			dsDrawTexImage( cInterFace.MatChatDoc[1] , 259, 576 - SubChatHeight, 128, 32 , 255 );    //Ã¤ÆÃÃ¢³»¿ë
		}
		else
			dsDrawTexImage( cInterFace.MatChatDoc[0] , 4, 576 - SubChatHeight, 296, 32 , 255 );    //Ã¤ÆÃÃ¢³»¿ë

	}

	if(cInterFace.ChatFlag){
		if(SubChatHeight){
				dsDrawTexImage( cInterFace.MatChatSmall[0] , 0, 294, 300, 116 , 255 );     //»õ³¢  Ã¤ÆÃÃ¢

				if (ChatBuffCnt >= 10)

				{
					dsDrawTexImage(chatbar, 285, 302, 8, 106, 255);
					dsDrawTexImage(chatico, 281, 302 + 93 - static_cast<int>(ChatScrollPoint*3.0), 16, 16, 255);
				}

				//dsDrawTexImage( cInterFace.MatChatSmall[1] , 256, 600-152-128-21, 128, 128 , 255 );   //»õ³¢  Ã¤ÆÃÃ¢
		}
		else{
			dsDrawTexImage( cInterFace.MatChatDefault[0] , 0, 600-192, 305, 210 , 255 );   //±âº» Ã¤ÆÃÃ¢

			if (ChatBuffCnt >= 14)

			{
				dsDrawTexImage(chatbar, 289, 416, 8, 150, 255);
				dsDrawTexImage(chatico, 285, 416 + 133 - static_cast<int>(ChatScrollPoint*4.2), 16, 16, 255);
			}

			dsDrawTexImage( MatChatBotton_Glay , 100, 564, 256, 16 , 255 );   //±âº» ÆÃÃ¢
			DrawSprite(100+ChatKindIndex*40,564 , lpChatButton[ChatKindIndex],0,0,40,16);

			//°øÁö
			DrawSprite(4,564 , lpChatButton_Notice[NoticeChatIndex],0,0,45,16);

			if(NoticeChatIndex == 1){
				DrawSprite(10,460 , lpChatLine,0,0,287,3);
				//¸¶ÀÌÅ©
				if(bMic)
					dsDrawTexImage( sinMatMike , 283, 464, 32, 32 , 255 );   //±âº» ÆÃÃ¢
			}
			else{
				//¸¶ÀÌÅ©
				if(bMic)
					dsDrawTexImage( sinMatMike , 283, 409, 32, 32 , 255 );   //±âº» ÆÃÃ¢
			}

		}
	} */


	if (sinMoveKindInter[SIN_CHARSTATUS] != 0) {
		DrawSprite(0, 600 - sinMoveKindInter[SIN_CHARSTATUS] + 56, lpStatusMain, 0, 0, 800, 200);
		// x
		if (ShowCharArrow)
		{
			DrawSprite(251, 565 + (256 - sinMoveKindInter[SIN_CHARSTATUS]), cShop.lpExit, 0, 0, 20, 20);
			DrawSprite(251, 565 - 27 + (256 - sinMoveKindInter[SIN_CHARSTATUS]), cInvenTory.lpExitInfo, 0, 0, 47, 27);
		}

		if (sinChar->StatePoint)
		{
			DrawSprite(520, 531 + (256 - sinMoveKindInter[SIN_CHARSTATUS]), lpStatusPoint, 0, 0, 115, 19);
			for (int i = 0; i < 5; i++)
			{
				DrawSprite(PointButton[i][0], PointButton[i][1] + (256 - sinMoveKindInter[SIN_CHARSTATUS]), lpChangeArrow[0], 0, 0, 18, 17);
			}
			if (sinPointButtonDown)
			{
				if (PointButtonPosi <= 5 && PointButtonPosi != 0)
					DrawSprite(PointButton[PointButtonPosi - 1][0] + 1, PointButton[PointButtonPosi - 1][1] + (256 - sinMoveKindInter[SIN_CHARSTATUS]) + 1, lpSelectArrow[0], 0, 0, 18, 17);

				if (PointButtonPosi == 6)
					DrawSprite(PointButton[PointButtonPosi - 1][0] + 1, PointButton[PointButtonPosi - 1][1] + (256 - sinMoveKindInter[SIN_CHARSTATUS]) + 1, lpSelectArrow[1], 0, 0, 18, 17);
			}
		}
#ifdef  __CLANSUBCHIP__
		if (cldata.myPosition == 100 || cldata.myPosition == 101 || cldata.myPosition == 104) {
#else
		if (cldata.myPosition == 100 || cldata.myPosition == 101) {
#endif
			if (cldata.hClanMark16) {
				TempLen = lstrlen(cldata.name);
				TempCount2 = 6 - (TempLen / 2);
				DrawSprite(CheckEditSize(sCharRectPosi[1].Rect.left, sCharRectPosi[1].Rect.right, cldata.name) - ((12 - TempLen / 2)) - TempCount2,
					sCharRectPosi[1].Rect.top + 13 + (256 - sinMoveKindInter[SIN_CHARSTATUS]), cldata.hClanMark16, 0, 0, 16, 16);
			}
		}
	}

}
DWORD dwSkillTextTime = 0;
DWORD dwSkillTextTimeFlag = 0;
void cCHARSTATUS::Main()
{

	if (CSKILL->SearchContiueSkill(SKILL_FROST_JAVELIN)) {
		dwSkillTextTime++;
		if (dwSkillTextTime > 70) {
			switch (dwSkillTextTimeFlag) {
			case 0:
				dwSkillTextTimeFlag = 1;
				break;
			case 1:
				dwSkillTextTimeFlag = 2;
				break;
			case 2:
				dwSkillTextTimeFlag = 1;
				break;
			}
			dwSkillTextTime = 0;
		}
	}
	else dwSkillTextTimeFlag = 0;

	int  i;
	ShowCharArrow = 0;
	RegiInfoPosi = 0;
	PointButtonPosi = 0;


	ChatKindIndexNum = 0;
	if (cInterFace.ChatFlag) {
		if (!SubChatHeight) {
			for (int t = 0; t < 5; t++) {
				if (pCursorPos.x > 100 + (t * 40) && pCursorPos.x < 100 + ((t + 1) * 40) && pCursorPos.y > 563 && pCursorPos.y < 563 + 16)
					ChatKindIndexNum = t + 1;
				switch (ChatKindIndexNum) {
				case 1:
					cHelpPet.PetMessage("*ChatTab_All", 0);

					break;
				case 2:
					cHelpPet.PetMessage("*ChatTab_Clan", 0);

					break;
				case 3:
					cHelpPet.PetMessage("*ChatTab_Party", 0);

					break;
				case 4:
					cHelpPet.PetMessage("*ChatTab_Trade", 0);

					break;
				case 5:
					cHelpPet.PetMessage("*ChatTab_Whisper", 0);

					break;
				}

			}
		}
	}

	NoticeChatIndexNum = 0;
	if (cInterFace.ChatFlag) {
		if (!SubChatHeight) {
			if (pCursorPos.x > 0 && pCursorPos.x < 45 && pCursorPos.y > 563 && pCursorPos.y < 563 + 16) {
				NoticeChatIndexNum = 1;
			}
		}
	}
	// x
	if (pCursorPos.x > 251 && pCursorPos.x < 269 && pCursorPos.y > 565 && pCursorPos.y < 583)
		ShowCharArrow = 1;
	if (OpenFlag) {
		for (i = 0; i < 8; i++)
			if (pCursorPos.x >= RegiBox[i][0] && pCursorPos.x <= RegiBox[i][2] &&
				pCursorPos.y >= RegiBox[i][1] && pCursorPos.y <= RegiBox[i][3]) {
				RegiInfoPosi = i + 1;

			}
		for (i = 0; i < 6; i++)
			if (pCursorPos.x >= PointButton[i][0] && pCursorPos.x <= PointButton[i][2] &&
				pCursorPos.y >= PointButton[i][1] && pCursorPos.y <= PointButton[i][3]) {
				PointButtonPosi = i + 1;
			}

	}
	else {
		for (i = 0; i < 5; i++)
			TempStatePoint[i] = 0;

	}

	if (sinLoadOK)
		CheckLevel();

}
void cCHARSTATUS::Close()
{

}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/
void cCHARSTATUS::LButtonDown(int x, int y)
{
	int cnt;

	if (NoticeChatIndexNum) {
		NoticeChatIndex++;
		if (NoticeChatIndex >= 3)NoticeChatIndex = 0;

	}

	if (ChatKindIndexNum) {
		// Quando clica nos botoes do chat pra mudar a tab
		ChatKindIndex = ChatKindIndexNum - 1;
		SetChatMsgFilter(ChatKindIndex);
		switch (ChatKindIndexNum) {
		case 1:
			cHelpPet.PetMessage("*ChatTab_All", 1);

			break;
		case 2:
			cHelpPet.PetMessage("*ChatTab_Clan", 1);

			break;
		case 3:
			cHelpPet.PetMessage("*ChatTab_Party", 1);

			break;
		case 4:
			cHelpPet.PetMessage("*ChatTab_Trade", 1);

			break;
		case 5:
			cHelpPet.PetMessage("*ChatTab_Whisper", 1);

			break;
		}

	}
	// X
	if (OpenFlag) {
		if (x > 619 && x < 637 && y > 414 && y < 432)
			OpenFlag = SIN_CLOSE;

		// Status 10 em 10 xxstr
		if (VRKeyBuff['B']) {
			if (sinChar->StatePoint > 9) {
				sinPointButtonDown = 1;
				switch (PointButtonPosi) {
				case 1:
					CheckCharForm();
					sinChar->Strength += 9;
					sinChar->StatePoint -= 9;
					for (cnt = 0; cnt < 4; cnt++)
						TempStatePoint[cnt] = TempStatePoint[cnt + 1];
					TempStatePoint[4] = POINT_STRENGTH;
					ReformCharForm();
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage();

					break;
				case 2:
					CheckCharForm();
					sinChar->Spirit += 9;
					sinChar->StatePoint -= 9;
					for (cnt = 0; cnt < 4; cnt++)
						TempStatePoint[cnt] = TempStatePoint[cnt + 1];
					TempStatePoint[4] = POINT_SPIRIT;
					ReformCharForm();
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage();


					break;

				case 3:
					CheckCharForm();
					sinChar->Talent += 9;
					sinChar->StatePoint -= 9;
					for (cnt = 0; cnt < 4; cnt++)
						TempStatePoint[cnt] = TempStatePoint[cnt + 1];
					TempStatePoint[4] = POINT_TALENT;
					ReformCharForm();
					cInvenTory.SetItemToChar();

					break;

				case 4:
					CheckCharForm();
					sinChar->Dexterity += 9;
					sinChar->StatePoint -= 9;
					for (cnt = 0; cnt < 4; cnt++)
						TempStatePoint[cnt] = TempStatePoint[cnt + 1];
					TempStatePoint[4] = POINT_DEXTERITY;
					ReformCharForm();
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage();

					break;

				case 5:
					CheckCharForm();
					sinChar->Health += 9;
					sinChar->StatePoint -= 9;
					for (cnt = 0; cnt < 4; cnt++)
						TempStatePoint[cnt] = TempStatePoint[cnt + 1];
					TempStatePoint[4] = POINT_HEALTH;
					ReformCharForm();
					cInvenTory.SetItemToChar();
					cInvenTory.CheckDamage();

					break;

				case 6:
					for (cnt = 4; cnt >= 0; cnt--) {
						if (TempStatePoint[cnt]) {
							switch (TempStatePoint[cnt]) {
							case POINT_STRENGTH:
								CheckCharForm();
								sinChar->Strength -= 9;
								sinChar->StatePoint += 9;
								TempStatePoint[cnt] = 0;
								ReformCharForm();
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage();
								break;
							case POINT_SPIRIT:
								CheckCharForm();
								sinChar->Spirit -= 9;
								sinChar->StatePoint += 9;
								TempStatePoint[cnt] = 0;
								ReformCharForm();
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage();
								break;
							case POINT_TALENT:
								CheckCharForm();
								sinChar->Talent -= 9;
								sinChar->StatePoint += 9;
								TempStatePoint[cnt] = 0;
								ReformCharForm();
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage();
								break;
							case POINT_DEXTERITY:
								CheckCharForm();
								sinChar->Dexterity -= 9;
								sinChar->StatePoint += 9;
								TempStatePoint[cnt] = 0;
								ReformCharForm();
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage();

								break;
							case POINT_HEALTH:
								CheckCharForm();
								sinChar->Health -= 9;
								sinChar->StatePoint += 9;
								TempStatePoint[cnt] = 0;
								ReformCharForm();
								cInvenTory.SetItemToChar();
								cInvenTory.CheckDamage();
								break;

							}
							break;
						}
					}

					break;
				}
			}

		}


		if (sinChar->StatePoint > 0) {
			sinPointButtonDown = 1;
			switch (PointButtonPosi) {
			case 1: //Èû
				CheckCharForm(); //ÀÎÁõ 
				sinChar->Strength++;
				sinChar->StatePoint--;
				for (cnt = 0; cnt < 4; cnt++) //ÇÑÄ­¾¿ ¾ÕÀ¸·Î ÀüÁø    
					TempStatePoint[cnt] = TempStatePoint[cnt + 1];
				TempStatePoint[4] = POINT_STRENGTH;
				ReformCharForm(); //ÀçÀÎÁõ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù

				break;
			case 2: //Á¤½Å·Â 
				CheckCharForm(); //ÀÎÁõ 
				sinChar->Spirit++;
				sinChar->StatePoint--;
				for (cnt = 0; cnt < 4; cnt++)
					TempStatePoint[cnt] = TempStatePoint[cnt + 1];
				TempStatePoint[4] = POINT_SPIRIT;
				ReformCharForm(); //ÀçÀÎÁõ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù


				break;

			case 3: //Àç´É 
				CheckCharForm(); //ÀÎÁõ 
				sinChar->Talent++;
				sinChar->StatePoint--;
				for (cnt = 0; cnt < 4; cnt++) //ÇÑÄ­¾¿ ¾ÕÀ¸·Î ÀüÁø    
					TempStatePoint[cnt] = TempStatePoint[cnt + 1];
				TempStatePoint[4] = POINT_TALENT;
				ReformCharForm(); //ÀçÀÎÁõ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù

				break;

			case 4: //¹ÌÃ¸¼º 
				CheckCharForm(); //ÀÎÁõ 
				sinChar->Dexterity++;
				sinChar->StatePoint--;
				for (cnt = 0; cnt < 4; cnt++) //ÇÑÄ­¾¿ ¾ÕÀ¸·Î ÀüÁø    
					TempStatePoint[cnt] = TempStatePoint[cnt + 1];
				TempStatePoint[4] = POINT_DEXTERITY;
				ReformCharForm(); //ÀçÀÎÁõ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù

				break;

			case 5: //°Ç°­ 
				CheckCharForm(); //ÀÎÁõ 
				sinChar->Health++;
				sinChar->StatePoint--;
				for (cnt = 0; cnt < 4; cnt++) //ÇÑÄ­¾¿ ¾ÕÀ¸·Î ÀüÁø    
					TempStatePoint[cnt] = TempStatePoint[cnt + 1];
				TempStatePoint[4] = POINT_HEALTH;
				ReformCharForm(); //ÀçÀÎÁõ 
				cInvenTory.SetItemToChar();
				cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù

				break;

			case 6: //½ºÅ×ÀÌÅÍ½º´É·ÂÄ¡¸¦ µÇµ¹¸°´Ù 
				for (cnt = 4; cnt >= 0; cnt--) {
					if (TempStatePoint[cnt]) {
						switch (TempStatePoint[cnt]) {
						case POINT_STRENGTH:
							CheckCharForm(); //ÀÎÁõ 
							sinChar->Strength--;
							sinChar->StatePoint++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //ÀçÀÎÁõ 
							cInvenTory.SetItemToChar();
							break;
						case POINT_SPIRIT:
							CheckCharForm(); //ÀÎÁõ 
							sinChar->Spirit--;
							sinChar->StatePoint++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //ÀçÀÎÁõ 
							cInvenTory.SetItemToChar();
							cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù
							break;
						case POINT_TALENT:
							CheckCharForm(); //ÀÎÁõ 
							sinChar->Talent--;
							sinChar->StatePoint++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //ÀçÀÎÁõ 
							cInvenTory.SetItemToChar();
							cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù
							break;
						case POINT_DEXTERITY:
							CheckCharForm(); //ÀÎÁõ 
							sinChar->Dexterity--;
							sinChar->StatePoint++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //ÀçÀÎÁõ 
							cInvenTory.SetItemToChar();
							cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù

							break;
						case POINT_HEALTH:
							CheckCharForm(); //ÀÎÁõ 
							sinChar->Health--;
							sinChar->StatePoint++;
							TempStatePoint[cnt] = 0;
							ReformCharForm(); //ÀçÀÎÁõ 
							cInvenTory.SetItemToChar();
							cInvenTory.CheckDamage(); //¼­¹ö¿¡ ÃÖ°íµ¥¹ÌÁö¸¦ º¸³½´Ù
							break;

						}
						break;
					}
				}

				break;
			}

		}

	}


}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/
void cCHARSTATUS::LButtonUp(int x, int y)
{
	sinPointButtonDown = 0; //¹öÆ°¾÷ÀÏ¶§ ÃÊ±âÈ­ 
}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/
void cCHARSTATUS::RButtonDown(int x, int y)
{
	if (ChatKindIndexNum) {
		switch (ChatKindIndexNum) {
		case 1: //Ã¤ÆÃÀüÃ¼
			cHelpPet.PetMessage("*ChatTab_All", 2);

			break;
		case 2: //Ã¤ÆÃÅ¬·£
			cHelpPet.PetMessage("*ChatTab_Clan", 2);

			break;
		case 3: //Ã¤ÆÃµ¿·á
			cHelpPet.PetMessage("*ChatTab_Party", 2);

			break;
		case 4: //Ã¤ÆÃ °Å·¡
			cHelpPet.PetMessage("*ChatTab_Trade", 2);

			break;
		case 5: //Ã¤ÆÃ ±Ó¸»
			cHelpPet.PetMessage("*ChatTab_Whisper", 2);

			break;
		}

	}

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/
void cCHARSTATUS::RButtonUp(int x, int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/
void cCHARSTATUS::KeyDown()
{

	if (sinGetKeyClick('C')) {
		cHelpPet.PetMessage("*CarStatus", 3);
		if (cCharStatus.OpenFlag)cCharStatus.OpenFlag = SIN_CLOSE;
		else cCharStatus.OpenFlag = SIN_OPEN;
		cInterFace.CheckAllBox(SIN_CHARSTATUS); //Ã¢À» ÇÏ³ª¸¸ ¶ç¿î´Ù 
		if (cCharStatus.OpenFlag) {
			if (!sinFireShow) {
				StartMenuFlame(0, 350);
				sinPlaySound(0);

			}
			else
				sinPlaySound(SIN_SOUND_SHOW_INTER);
			sinFireShow = 1;
		}
	}
}

/*----------------------------------------------------------------------------*
*
*-----------------------------------------------------------------------------*/
DWORD CharStatusColor[10] = { RGB(255,255,255) , RGB(255,0,0) ,RGB(255,220,0),
					RGB(142,230,254) , RGB(128,255,128) ,RGB(255,225,0),
					RGB(255,180,200) , RGB(255,230,10) ,   RGB(255,220,0),
					RGB(255,220,0)

};

int sinBackupTextColor = 0;
void cCHARSTATUS::DrawCharText()
{

	CheckChageStateDisplay();

	if (!sinMoveKindInter[SIN_CHARSTATUS])
		return;

	INT64 ExpNow;
	INT64 NextExp;
	int TempLen = 0;
	HDC	hdc = NULL;
	char strBuff[128];
	memset(strBuff, 0, sizeof(strBuff));
	SelectObject(hdc, sinFont);
	SetFontTextColor(RGB(255, 255, 255));


	// DESATIVADO: SISTEMA DE TITULO
	wsprintf(strBuff, "%s", cInvenTory.JobName); //Titulo char
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[0].Rect.left, sCharRectPosi[0].Rect.right, strBuff), sCharRectPosi[0].Rect.top + (253 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%s", sinCharDisplay.szName); //Nome Char
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[1].Rect.left, sCharRectPosi[1].Rect.right, strBuff), sCharRectPosi[1].Rect.top + (253 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

#ifdef  __CLANSUBCHIP__
	if (cldata.myPosition == 100 ||
		cldata.myPosition == 101 || cldata.myPosition == 104) {
#else
	if (cldata.myPosition == 100 || cldata.myPosition == 101) {
#endif
		TempLen = lstrlen(cldata.name);
		wsprintf(strBuff, "%s", cldata.name); //Clan Name Char
		dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[1].Rect.left, sCharRectPosi[1].Rect.right, strBuff) + TempLen, sCharRectPosi[1].Rect.top + 20 + (253 - sinMoveKindInter[SIN_CHARSTATUS]),
			strBuff, lstrlen(strBuff));
	}

	wsprintf(strBuff, "%d", sinCharDisplay.Level); //Char Level
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[2].Rect.left, sCharRectPosi[2].Rect.right, strBuff), sCharRectPosi[2].Rect.top + (253 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));


	// EXP do char aqui
	std::fill_n(strBuff, 128, 0);
	ExpNow = cCharStatus.sinGetNowExp();
	NumLineComa64(ExpNow, strBuff);

	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[3].Rect.left, sCharRectPosi[3].Rect.right, strBuff), sCharRectPosi[3].Rect.top + (253 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlenA(strBuff));

	std::fill_n(strBuff, 128, 0);
	NextExp = cCharStatus.sinGetNextExp();
	NumLineComa64(NextExp, strBuff);

	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[4].Rect.left, sCharRectPosi[4].Rect.right, strBuff), sCharRectPosi[4].Rect.top + (253 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlenA(strBuff));


	if (AddVirtualLife[1])
	{
		AddVirtualLife[0] = ((int)sinGetLife() * sinVirtualLifePercent) / 100;
	}

	if (AddVirtualMana[1])
	{
		AddVirtualMana[0] = ((int)sinGetMana() * sinVirtualManaPercent) / 100;
	}

	if (AddVirtualStamina[1])
	{
		AddVirtualStamina[0] = ((int)sinGetStamina() * sinVirtualStaminaPercent) / 100;
	}

	SetFontTextColor(RGB(255, 255, 255));

	sprintf(strBuff, "%.1f", sinCharDisplay.Life_Regen);
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[24].Rect.left, sCharRectPosi[24].Rect.right, strBuff), sCharRectPosi[24].Rect.top + (259 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(RGB(255, 255, 255));

	sprintf(strBuff, "%.1f", sinCharDisplay.Mana_Regen);
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[25].Rect.left, sCharRectPosi[25].Rect.right, strBuff), sCharRectPosi[25].Rect.top + (254 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(RGB(255, 255, 255));

	sprintf(strBuff, "%.1f", sinCharDisplay.Stamina_Regen);
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[26].Rect.left, sCharRectPosi[26].Rect.right, strBuff), sCharRectPosi[26].Rect.top + (249 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_LIFE]]);
	wsprintf(strBuff, "%d/%d", sinGetLife() + AddVirtualLife[0], sinCharDisplay.Life[1] + AddVirtualLife[1]); // CHAR HP 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[5].Rect.left, sCharRectPosi[5].Rect.right, strBuff), sCharRectPosi[5].Rect.top + (259 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_MANA]]);
	wsprintf(strBuff, "%d/%d", sinGetMana(), sinCharDisplay.Mana[1]); //CHAR MANA  
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[6].Rect.left, sCharRectPosi[6].Rect.right, strBuff), sCharRectPosi[6].Rect.top + (254 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_STAMINA]]);
	wsprintf(strBuff, "%d/%d", sinGetStamina(), sinCharDisplay.Stamina[1]); //CHAR STAMINA
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[7].Rect.left, sCharRectPosi[7].Rect.right, strBuff), sCharRectPosi[7].Rect.top + (249 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(RGB(255, 255, 255));

	wsprintf(strBuff, "%d", sinCharDisplay.Strength); //FOR 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[8].Rect.left, sCharRectPosi[8].Rect.right, strBuff), sCharRectPosi[8].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));


	wsprintf(strBuff, "%d", sinCharDisplay.Spirit); //INT 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[9].Rect.left, sCharRectPosi[9].Rect.right, strBuff), sCharRectPosi[9].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));



	wsprintf(strBuff, "%d", sinCharDisplay.Talent); //TALENT
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[10].Rect.left, sCharRectPosi[10].Rect.right, strBuff), sCharRectPosi[10].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));



	wsprintf(strBuff, "%d", sinCharDisplay.Dexterity); //DEXT 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[11].Rect.left, sCharRectPosi[11].Rect.right, strBuff), sCharRectPosi[11].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%d", sinCharDisplay.Health); //VIDA 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[12].Rect.left, sCharRectPosi[12].Rect.right, strBuff), sCharRectPosi[12].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	if (sinChar->StatePoint) {
		wsprintf(strBuff, "%d", sinChar->StatePoint); //PONTOS   
		dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[23].Rect.left, sCharRectPosi[23].Rect.right, strBuff), sCharRectPosi[23].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
			strBuff, lstrlen(strBuff));

	}

	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_ATTACK_RATE]]);
	wsprintf(strBuff, "%d", sinCharDisplay.Attack_Rating + sDisplayStateShow.Attack_Rate); //ATK MIN - ATK MAX
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[18].Rect.left, sCharRectPosi[18].Rect.right, strBuff), sCharRectPosi[18].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	if (dwSkillTextTimeFlag == 1) {
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_BLUE;
	}
	if (dwSkillTextTimeFlag == 2) {
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_YELLOW;
	}

	int DisplayDamage2[2] = { 0,0 };
	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_DAMAGE]]);
	DisplayDamage2[0] = sinCharDisplay.Attack_Damage[0] + sinTempDamage2[0] + sDisplayStateShow.Damage[0];
	DisplayDamage2[1] = sinCharDisplay.Attack_Damage[1] + sinTempDamage2[1] + sDisplayStateShow.Damage[1];
	if (DisplayDamage2[0] < 0)DisplayDamage2[0] = 0;
	if (DisplayDamage2[1] < 0)DisplayDamage2[1] = 0;
	wsprintf(strBuff, "%d-%d", DisplayDamage2[0], DisplayDamage2[1]); //DAMAGE
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[19].Rect.left, sCharRectPosi[19].Rect.right, strBuff), sCharRectPosi[19].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_DEFENSE]]);
	wsprintf(strBuff, "%d", sinCharDisplay.Defence + sDisplayStateShow.Defense); //DEFENSE 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[20].Rect.left, sCharRectPosi[20].Rect.right, strBuff), sCharRectPosi[20].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_ABSORB]]);
	wsprintf(strBuff, "%d", sinCharDisplay.Absorption + sinTempAbsorb + sDisplayStateShow.Absorb); //ABS
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[21].Rect.left, sCharRectPosi[21].Rect.right, strBuff), sCharRectPosi[21].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));


	SetFontTextColor(CharStatusColor[SkillTextColor[SIN2_SPEED]]);
	wsprintf(strBuff, "%d", sinCharDisplay.Move_Speed); //SPEED 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[22].Rect.left, sCharRectPosi[22].Rect.right, strBuff), sCharRectPosi[22].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	SetFontTextColor(RGB(255, 255, 255));
	wsprintf(strBuff, "%d", sinCharDisplay.Resistance[0]);
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[13].Rect.left, sCharRectPosi[13].Rect.right, strBuff), sCharRectPosi[13].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%d", sinCharDisplay.Resistance[5]); 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[14].Rect.left, sCharRectPosi[14].Rect.right, strBuff), sCharRectPosi[14].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%d", sinCharDisplay.Resistance[2]);
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[15].Rect.left, sCharRectPosi[15].Rect.right, strBuff), sCharRectPosi[15].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%d", sinCharDisplay.Resistance[4]); 
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[16].Rect.left, sCharRectPosi[16].Rect.right, strBuff), sCharRectPosi[16].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%d", sinCharDisplay.Resistance[3]);
	dsTextLineOut(hdc, CheckEditSize(sCharRectPosi[17].Rect.left, sCharRectPosi[17].Rect.right, strBuff), sCharRectPosi[17].Rect.top + (256 - sinMoveKindInter[SIN_CHARSTATUS]),
		strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "%d%%", sinCharDisplay.Critical_Hit);
	dsTextLineOut(hdc, 262, 507 + (254 - sinMoveKindInter[SIN_CHARSTATUS]), strBuff, strlen(strBuff));

	int TotalBlock = sinCharDisplay.Chance_Block;

	if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN) {

		int cnt = CSKILL->SearchContiueSkill(SKILL_EXTREME_SHIELD);
		if (cnt > 0 && cnt <= 10) {
			if (sInven[1].ItemIndex) {
				if (sinDS1 == (cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					cnt = E_Shield_BlockRate[cnt - 1];
					TotalBlock += cnt;
				}
			}
		}
	}

	if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT) {

		int cnt = CSKILL->SearchContiueSkill(SKILL_DIVINE_INHALATION);
		if (cnt > 0 && cnt <= 10) {
			if (sInven[1].ItemIndex) {
				if (sinDS1 == (cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					cnt = D_Inhalation_Block[cnt - 1];
					TotalBlock += cnt;
				}
			}
		}
	}

	wsprintf(strBuff, "%d%%", TotalBlock);
	dsTextLineOut(hdc, 261, 526 + (255 - sinMoveKindInter[SIN_CHARSTATUS]), strBuff, strlen(strBuff));

	int TotalEvade = 0;

	if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PIKEMAN) {

		int cnt = CSKILL->SearchContiueSkill(SKILL_VAGUE);
		if (cnt > 0 && cnt <= 10) {
			if (sInven[0].ItemIndex) {
				if (sinWP1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					cnt = Vague_EvasionPercent[cnt - 1];
					TotalEvade += cnt;
				}
			}
		}
	}


	if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER) {

		int cnt = CSKILL->SearchContiueSkill(SKILL_EVASION_MASTERY);
		if (cnt > 0 && cnt <= 10) {
			if (sInven[0].ItemIndex) {
				if (sinWS1 == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) {
					cnt = Evasion_Mastery_AddPercent[cnt - 1];
					TotalEvade += cnt;
				}
			}
		}
	}


	if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS) {

		int cnt = CSKILL->SearchContiueSkill(SKILL_SUMMON_MUSPELL);
		if (cnt > 0 && cnt <= 10) {
			cnt = Summon_Muspell_BlockPercent[cnt - 1];
			TotalEvade += cnt;
		}
	}


	int cnt = CSKILL->SearchContiueSkill(SKILL_HALL_OF_VALHALLA);
	if (cnt > 0 && cnt <= 10) {
		cnt = Hall_Of_Valhalla_AddEvation[cnt - 1];
		TotalEvade += cnt;
	}

	if (CSKILL->SearchContiueSkillCODE(SCROLL_P_EVASION))
		TotalEvade += 5;

	if (CSKILL->SearchContiueSkillCODE(SCROLL_EVASION))
		TotalEvade += 5;


	if (CSKILL->SearchContiueSkill(SIN_CLANSKILL_EVASION)) {
		TotalEvade += 10;
	}

	wsprintf(strBuff, "%d%%", TotalEvade);
	dsTextLineOut(hdc, 261, 547 + (254 - sinMoveKindInter[SIN_CHARSTATUS]), strBuff, strlen(strBuff));



}

void cCHARSTATUS::OtherStateShow()
{
	HDC	hdc = NULL;
	char strBuff[128];
	memset(strBuff, 0, sizeof(strBuff));

	SetFontTextColor(RGB(255, 255, 255));
	wsprintf(strBuff, "°ø°Ý¼Óµµ:%d", sinCharDisplay.Attack_Speed);
	dsTextLineOut(hdc, 100, 300, strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "ºí·°À²:%d", sinCharDisplay.Chance_Block);
	dsTextLineOut(hdc, 200, 300, strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "Å©¸®Æ¼ÄÃ:%d", sinCharDisplay.Critical_Hit);
	dsTextLineOut(hdc, 300, 300, strBuff, lstrlen(strBuff));

	wsprintf(strBuff, "½´ÆÃ±æÀÌ:%d", sinCharDisplay.Shooting_Range);
	dsTextLineOut(hdc, 400, 300, strBuff, lstrlen(strBuff));

}

void cCHARSTATUS::CheckLevel()
{
	int UpLevelPoint;
	if (OldLevelTemp - 176 != OldLevel)return;
	if (sinChar->Level > OldLevel) {
		cInvenTory.SetItemToChar();
		UpLevelPoint = sinChar->Level - OldLevel;
		CheckCharForm();
		if ((sinChar->Level % 2) == 0 && sinChar->Level >= 10) {
			sinSkill.SkillPoint++; 
		}
		if ((sinChar->Level % 2) == 0 && sinChar->Level >= 60) {	
			sinSkill.SkillPoint4++; 
		}


		sinLevelPoint = sinChar->Level - OldLevel;

		if ((sinQuest_levelLog & QUESTBIT_LEVEL_80_2) == QUESTBIT_LEVEL_80_2) {
			if ((sinQuest_levelLog & QUESTBIT_LEVEL_90_2) == QUESTBIT_LEVEL_90_2) {
				if (sinChar->Level >= 90) {
					sinChar->StatePoint += 3;
				}
			}
			if (sinChar->Level >= 80) 
			{
				sinChar->StatePoint += 2;
			}
		}
		sinChar->StatePoint += sinLevelPoint * 5;
		OldLevel = sinChar->Level;
		OldLevelTemp = OldLevel + 176;
		ReformCharForm();
		sinSetLife(sinChar->Life[1]);
		sinSetMana(sinChar->Mana[1]);
		sinSetStamina(sinChar->Stamina[1]);
		cInvenTory.CheckDamage();

		if (cHelpPet.PetKind && cHelpPet.PetShow) {
			cHelpPet.LevelFlag = 1;
			switch (sinChar->Level) {
			case 2:
				cHelpPet.PetMessageHelp("*Level_Message2-1");
				cHelpPet.PetMessageHelp("*Level_Message2-2");
				break;
			case 3:
				cHelpPet.PetMessageHelp("*Level_Message3");
				break;
			case 4:
				cHelpPet.PetMessageHelp("*Level_Message4");
				break;
			case 5:
				cHelpPet.PetMessageHelp("*Level_Message5");
				break;
			case 6:
				cHelpPet.PetMessageHelp("*Level_Message6-1");
				cHelpPet.PetMessageHelp("*Level_Message6-2");
				break;
			case 7:
				cHelpPet.PetMessageHelp("*Level_Message7");
				break;
			case 8:
				cHelpPet.PetMessageHelp("*Level_Message8");
				break;
			case 9:
				cHelpPet.PetMessageHelp("*Level_Message9-1");
				cHelpPet.PetMessageHelp("*Level_Message9-2");
				break;
			case 10:
				cHelpPet.PetMessageHelp("*Level_Message10-1");
				cHelpPet.PetMessageHelp("*Level_Message10-2");
				break;
			}

		}

		ResetEnergyGraph(3);

		void InitHelp();
		InitHelp();


	}
}

void cCHARSTATUS::BackUpsinCharState(smCHAR * pChar)
{






}


INT64 cCHARSTATUS::sinGetNowExp()
{
	INT64 sinExp64 = 0;
	INT64   sinExp = 0;
	sinExp64 = GetXorExp64(sinChar);
	if (sinChar->Level > 79) {
		sinExp = (INT64)(sinExp64);

	}
	else {
		sinExp = (INT64)(sinExp64);
	}

	return sinExp;

}

INT64 cCHARSTATUS::sinGetNextExp()
{
	INT64 sinExp = 0;
	if (sinChar->Level > 79) {

		sinExp = (INT64)((GetNextExp(sinChar->Level)));

	}
	else {
		sinExp = (INT64)GetNextExp(sinChar->Level);
	}

	return sinExp;
}


int cCHARSTATUS::InitCharStatus(int kind)
{
	int Total, Total2;

	switch (kind) {
	case 0:
		if (sinChar->wVersion[1] == 1) {
			sinSkillPontInitButtonFlag = 0;
			cMessageBox.ShowMessage(MESSAGE_ALREADY_INITSTATE);
			return FALSE;
		}
		sinSkillPontInitButtonFlag = 1;
		sinInitPointPassFlag = 1;
		break;
	}
	cMessageBox.ShowMessage(MESSAGE_GYUNGFUM_OK);
	Total = sinChar->Strength + sinChar->Spirit + sinChar->Talent + sinChar->Dexterity + sinChar->Health + sinChar->StatePoint;

	for (int i = 0; i < 4; i++) {
		if (sinChar->JOB_CODE == TempNewCharacterInit[i][0]) {
			sinChar->Strength = TempNewCharacterInit[i][1];
			sinChar->Spirit = TempNewCharacterInit[i][2];
			sinChar->Talent = TempNewCharacterInit[i][3];
			sinChar->Dexterity = TempNewCharacterInit[i][4];
			sinChar->Health = TempNewCharacterInit[i][5];
			Total2 = sinChar->Strength + sinChar->Spirit + sinChar->Talent + sinChar->Dexterity + sinChar->Health;
			sinChar->StatePoint = Total - Total2;
			sinChar->wVersion[1] = 1;
			ReformCharForm();
			cInvenTory.SetItemToChar();
			return TRUE;
		}
		if (sinChar->JOB_CODE == MorNewCharacterInit[i][0]) {
			sinChar->Strength = MorNewCharacterInit[i][1];
			sinChar->Spirit = MorNewCharacterInit[i][2];
			sinChar->Talent = MorNewCharacterInit[i][3];
			sinChar->Dexterity = MorNewCharacterInit[i][4];
			sinChar->Health = MorNewCharacterInit[i][5];
			Total2 = sinChar->Strength + sinChar->Spirit + sinChar->Talent + sinChar->Dexterity + sinChar->Health;
			sinChar->StatePoint = Total - Total2;
			sinChar->wVersion[1] = 1;
			ReformCharForm();
			cInvenTory.SetItemToChar();
			return TRUE;
		}

	}

	return TRUE;
}

int cCHARSTATUS::CheckChageStateDisplay()
{
	memset(&sDisplayStateShow, 0, sizeof(sDISPLAYSTATE));
	for (int i = 0; i < 10; i++) {
		if (sDisplayState[i].MaxTime) {
			sDisplayStateShow.Absorb += sDisplayState[i].Absorb;
			sDisplayStateShow.Attack_Rate += sDisplayState[i].Attack_Rate;
			sDisplayStateShow.Damage[0] += sDisplayState[i].Damage[0];
			sDisplayStateShow.Damage[1] += sDisplayState[i].Damage[1];
			if (sDisplayState[i].PercentDamage[0])
				sDisplayStateShow.Damage[0] += (short)(sinChar->Attack_Damage[0] * sDisplayState[i].PercentDamage[0]) / 100;
			if (sDisplayState[i].PercentDamage[1])
				sDisplayStateShow.Damage[1] += (short)(sinChar->Attack_Damage[1] * sDisplayState[i].PercentDamage[1]) / 100;

			sDisplayStateShow.Defense += sDisplayState[i].Defense;

			if (sDisplayStateShow.Absorb < 0)
				SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_RED;
			if (sDisplayStateShow.Defense < 0)
				SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_RED;
			if (sDisplayStateShow.Damage[0] < 0 || sDisplayStateShow.Damage[1])
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_RED;


			if (sDisplayState[i].MaxTime <= dwPlayTime) {
				memset(&sDisplayState[i], 0, sizeof(sDISPLAYSTATE));
				cInvenTory.SetItemToChar();
			}

		}

	}
	//LV 90Äù½ºÆ® ÀúÁÖ 
	if (sinQuest_Level90_2.CODE && sinQuest_Level90_2.State >= 2) {
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_RED;

	}
	return TRUE;
}


// Àåº° - ±×¶óºñÆ¼ ½ºÅ©·Ñ
int cCHARSTATUS::UseGravityScrool()
{

	float	fstrength, fhealth, ftalent, fLevel, fSpirit;


	fstrength = (float)sinChar->Strength;
	fhealth = (float)sinChar->Health;
	ftalent = (float)sinChar->Talent;
	fSpirit = (float)sinChar->Spirit;
	fLevel = (float)sinChar->Level;


	if (sinChar->GravityScroolCheck[0] == 0)
	{
		sinChar->Weight[1] = (short)(((fstrength * 2) + (fhealth * 1.5) + fLevel * 3) + 60);

	}

	if (sinChar->GravityScroolCheck[0] > 0)
	{
		sinChar->Weight[1] = (short)(((fstrength * 2) + (fhealth * 1.5) + fLevel * 3) + 60) + (sinChar->GravityScroolCheck[0]) * 50;

	}



	sinChar->wVersion[1] = 1;
	ReformCharForm();//ÀçÀÎÁõ 11111111111111
	cInvenTory.SetItemToChar();

	return TRUE;
}


// ¹ÚÀç¿ø - Ä³¸¯ÅÍ ¼Ó¼ºº° ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ
int cCHARSTATUS::InitCharStatus_Attribute(int kind)
{
	int Total, Total2;
	int Brood_code = 0; // Á¾Á· ±¸ºÐ
	int Job_Num = 0;

	cMessageBox.ShowMessage(MESSAGE_GYUNGFUM_OK);
	Total = sinChar->Strength + sinChar->Spirit + sinChar->Talent + sinChar->Dexterity + sinChar->Health + sinChar->StatePoint;

	for (int i = 0; i < 4; i++)
	{
		if (sinChar->JOB_CODE == TempNewCharacterInit[i][0])
		{
			Brood_code = BROOD_CODE_TEMPSKRON;
			Job_Num = i;
		}
		if (sinChar->JOB_CODE == MorNewCharacterInit[i][0])
		{
			Brood_code = BROOD_CODE_MORAYION;
			Job_Num = i;
		}
	}

	if (Brood_code == BROOD_CODE_TEMPSKRON)  // ÅÛ½ºÅ©·Ð
	{
		switch (kind)
		{
		case 1: // Èû
			sinChar->Strength = TempNewCharacterInit[Job_Num][1];
			break;
		case 2: // Á¤½Å·Â
			sinChar->Spirit = TempNewCharacterInit[Job_Num][2];
			break;
		case 3: // Àç´É
			sinChar->Talent = TempNewCharacterInit[Job_Num][3];
			break;
		case 4: // ¹ÎÃ¸¼º
			sinChar->Dexterity = TempNewCharacterInit[Job_Num][4];
			break;
		case 5: // °Ç°­
			sinChar->Health = TempNewCharacterInit[Job_Num][5];
			break;
		}
	}
	else // ¸ð¶óÀÌ¿Â
	{
		switch (kind)
		{
		case 1: // Èû
			sinChar->Strength = MorNewCharacterInit[Job_Num][1];
			break;
		case 2: // Á¤½Å·Â
			sinChar->Spirit = MorNewCharacterInit[Job_Num][2];
			break;
		case 3: // Àç´É
			sinChar->Talent = MorNewCharacterInit[Job_Num][3];
			break;
		case 4: // ¹ÎÃ¸¼º
			sinChar->Dexterity = MorNewCharacterInit[Job_Num][4];
			break;
		case 5: // °Ç°­
			sinChar->Health = MorNewCharacterInit[Job_Num][5];
			break;
		}
	}

	sinPlaySound(SIN_SOUND_EAT_POTION2);//´ë¹Ú »ç¿îµå
	StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_AGING); //¿¡ÀÌÂ¡ÀÌÆåÆ®

	Total2 = sinChar->Strength + sinChar->Spirit + sinChar->Talent + sinChar->Dexterity + sinChar->Health;
	sinChar->StatePoint = Total - Total2;
	sinChar->wVersion[1] = 1;
	ReformCharForm();//ÀçÀÎÁõ 
	cInvenTory.SetItemToChar();

	return TRUE;

}
