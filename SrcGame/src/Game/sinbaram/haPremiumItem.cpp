/*----------------------------------------------------------------------------*
*	ÆÄÀÏ¸í :  haPremiumItemItem.h
*	ÇÏ´ÂÀÏ :  ÇÁ¸®¹Ì¾ö ¾ÆÀÌÅÛ ¼­ºñ½º°ü·Ã
*	ÀÛ¼ºÀÏ :  ÃÖÁ¾¾÷µ¥ÀÌÆ® 2005³â 9¿ù
*	ÀÛ¼ºÀÚ :  ÇÏ´ë¿ë
*-----------------------------------------------------------------------------*/
#include "sinLinkHeader.h"
#include "..\\field.h"
#include "..\\AreaServer.h"
#include "..\ConfirmationBox.h"
#include "GlobalsShared.h"
#include <string>
#include "Montarias\\CMountHandler.h"
#include <TitleBox/TitleBox.h>

using namespace std;

cHaPremiumItem  chaPremiumitem;

#define  TELEPORT_FIELDMAX 29


// Core Azul - DRACKO
int TelePort_FieldNum[TELEPORT_FIELDMAX][3] = {
	// (Number, FieldNum, FieldLevel)
	{ 0, 52, 100 },
	{ 1, 2, 100 },
	{ 2, 10, 100 },
	{ 3, 25, 100 },
	{ 4, 11, 100 },
	{ 5, 13, 120 },
	{ 6, 14, 120 },
	{ 7, 15, 120 },
	{ 8, 5, 140 },
	{ 9, 26, 140 },
	{ 10, 34, 140 },
	{ 11, 37, 100 },
	{ 12, 38, 100 },
	{ 13, 47, 149 },
	
};

/*
int TelePort_FieldNum[TELEPORT_FIELDMAX][3] = {
	{ 0, 3, 0 },
	{ 1, 3, 0 },
	{ 2, 2, 0 },
	{ 3, 0, 0 },
	{ 4, 44, 0 },
	{ 5, 59, 0 },
	{ 6, 36, 0 },
	{ 7, 47, 0 },
	{ 8, 60, 0 },
	{ 9, 33, 0 },
	{ 10, 21, 0 },
	{ 11, 51, 0 },
	{ 12, 50, 0 },
	{ 13, 49, 0 },
	{ 14, 61, 0 }
	
	{ 15, 13, 40 },
	{ 16, 14, 50 },
	{ 17, 15, 60 },
	{ 18, 22, 70 },
	{ 19, 23, 75 },
	{ 20, 42, 100 },
	{ 21, 34, 70 },
	{ 22, 27, 80 },
	{ 23, 28, 85 },
	{ 24, 31, 90 },
	{ 25, 35, 90 },
	{ 26, 36, 90 },
	{ 27, 37, 95 },
	{ 28, 38, 100 },
}; */

//
char* haTeleport_Field[] = {
	"image\\Sinimage\\help\\haTeleport_Field[1].sin",
	"image\\Sinimage\\help\\haTeleport_Field[2].sin",
	"image\\Sinimage\\help\\haTeleport_Field[3].sin",
	"image\\Sinimage\\help\\haTeleport_Field[4].sin",
};
/*----------------------------------------------------------------------------*
*					  »ý¼ºÀÚ
*-----------------------------------------------------------------------------*/
cHaPremiumItem::cHaPremiumItem()
{
	TelePortCore.PageButtonFlag[0] = 0;
	TelePortCore.PageButtonFlag[1] = 0;
	TelePortCore.FieldIndex[0] = 0;
	TelePortCore.FieldIndex[1] = 0;
	TelePortCore.PageIndex = 0;
	Using_ItemKind = 0;
	Using_SelectInvenItemIndex = 0;
	int i;
	for (i = 0; i < SINUPKEEPITEM_MAX; i++) {
		UpKeepItem[i].PosiX = 0;
		UpKeepItem[i].PosiY = 0;
		UpKeepItem[i].Visible = false;
		UpKeepItem[i].IconTime[0] = 0;
		UpKeepItem[i].IconTime[1] = 0;
		UpKeepItem[i].Alpha = 255;
		UpKeepItem[i].Alphakind = 0;
		UpKeepItem[i].Per = 0;
		memset(UpKeepItem[i].Id, NULL, sizeof(UpKeepItem[i].Id));
	}

	UseClickPremiumItemFlag = false;

	m_ThirdEyesTime = 0;
	m_ExpUpPotionTime = 0;
	m_VampiricCuspidTime = 0;
	m_ManaRechargingPTime = 0;
	m_ManaReducePotiontime = 0;
	m_ManaReducePotionValue = 0;
	m_MightofAwellTime = 0;
	m_MightofAwellWeight = 0;
	m_PhenixPetTime = 0;
	m_HelpPetTime = 0;
	m_VampiricCuspidEXTime = 0;
	m_StaminaReducePotiontime = 0;
	m_StaminaReducePotionValue = 0;

	m_Hat = 0;
	HatKind = 0;
	m_MountTime = 0;
	m_MountParam = 0;


	iBigHeadLeftTime = 0;
	iBigHeadCounter = 0;
	iBigHeadType = 0;

}
/*----------------------------------------------------------------------------*
*					  ¼Ò¸êÀÚ
*-----------------------------------------------------------------------------*/
cHaPremiumItem::~cHaPremiumItem()
{
	//int i;
	//for( i = 0 ; i < SINUPKEEPITEM_MAX ; i++ ){
	//	if(UpKeepItem[i].lpImage){
	//		UpKeepItem[i].lpImage->Release();
	//		UpKeepItem[i].lpImage = 0;
	//	}
	//}
}

/*----------------------------------------------------------------------------*
*					  init
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::init()
{
	//ÇÁ¸®¹Ì¾ö ¾ÆÀÌÅÛ »ç¿ë½Ã ¹öÇÁ Ç¥½Ã ÀÌ¹ÌÁö ·Îµù 
	UpKeepItemTGAImage[0] = CreateTextureMaterial("game\\images\\premium\\ThirdEye.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[1] = CreateTextureMaterial("game\\images\\premium\\Exp.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[2] = CreateTextureMaterial("game\\images\\premium\\HPUp.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[3] = CreateTextureMaterial("game\\images\\premium\\MPUP.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	UpKeepItemTGAImage[4] = CreateTextureMaterial("game\\images\\premium\\Gravity.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ ÀÌ¹ÌÁö
	UpKeepItemTGAImage[5] = CreateTextureMaterial("game\\images\\premium\\MPDown.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç ÀÌ¹ÌÁö
	UpKeepItemTGAImage[6] = CreateTextureMaterial("game\\images\\premium\\PhoenixFire.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// pluto Æê(ÇØ¿Ü)
	UpKeepItemTGAImage[7] = CreateTextureMaterial("game\\images\\premium\\PhoenixFire.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸®)
	UpKeepItemTGAImage[8] = CreateTextureMaterial("game\\images\\premium\\PhoenixIce.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º)
	UpKeepItemTGAImage[9] = CreateTextureMaterial("game\\images\\premium\\PhoenixLightning.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À)
	UpKeepItemTGAImage[10] = CreateTextureMaterial("game\\images\\premium\\PhoenixHeal.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ®)
	UpKeepItemTGAImage[11] = CreateTextureMaterial("game\\images\\premium\\VPEX.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA); // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	UpKeepItemTGAImage[12] = CreateTextureMaterial("game\\images\\premium\\SPDown.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);	// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç

	UpKeepItemTGAImage[13] = CreateTextureMaterial("game\\images\\premium\\TopPvp.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[14] = CreateTextureMaterial("game\\images\\premium\\BigHead.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[15] = CreateTextureMaterial("game\\images\\premium\\BigHead.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[16] = CreateTextureMaterial("game\\images\\premium\\BigHead.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	UpKeepItemTGAImage[17] = CreateTextureMaterial("game\\images\\premium\\CaravanArma.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[18] = CreateTextureMaterial("game\\images\\premium\\CaravanHopy.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[19] = CreateTextureMaterial("game\\images\\premium\\CaravanBuma.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	UpKeepItemTGAImage[20] = CreateTextureMaterial("game\\images\\premium\\SheepHat.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[21] = CreateTextureMaterial("game\\images\\premium\\TuttiFruttiHat.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[22] = CreateTextureMaterial("game\\images\\premium\\SoccerHat.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[23] = CreateTextureMaterial("game\\images\\premium\\XmasHat.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[24] = CreateTextureMaterial("game\\images\\premium\\HalloweenHat.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);
	UpKeepItemTGAImage[25] = CreateTextureMaterial("game\\images\\premium\\MagicianHat.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	UpKeepItemTGAImage[26] = CreateTextureMaterial("game\\images\\premium\\Mount.png", 0, 0, 0, 0, SMMAT_BLEND_ALPHA);

	lpUpKeepItemBackImage[0] = LoadDibSurfaceOffscreen("game\\images\\premium\\ThirdEye_.png");
	lpUpKeepItemBackImage[1] = LoadDibSurfaceOffscreen("game\\images\\premium\\Exp_.png");
	lpUpKeepItemBackImage[2] = LoadDibSurfaceOffscreen("game\\images\\premium\\HPUp_.png");
	lpUpKeepItemBackImage[3] = LoadDibSurfaceOffscreen("game\\images\\premium\\MPUP_.png");

	lpUpKeepItemBackImage[4] = LoadDibSurfaceOffscreen("game\\images\\premium\\Gravity_.png");	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ ÀÌ¹ÌÁö
	lpUpKeepItemBackImage[5] = LoadDibSurfaceOffscreen("game\\images\\premium\\MPDown_.png");  	// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç ÀÌ¹ÌÁö
	lpUpKeepItemBackImage[6] = LoadDibSurfaceOffscreen("game\\images\\premium\\PhoenixFire_.png");  	// pluto Æê(ÇØ¿Ü)
	lpUpKeepItemBackImage[7] = LoadDibSurfaceOffscreen("game\\images\\premium\\PhoenixFire_.png");	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	lpUpKeepItemBackImage[8] = LoadDibSurfaceOffscreen("game\\images\\premium\\PhoenixIce_.png");	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	lpUpKeepItemBackImage[9] = LoadDibSurfaceOffscreen("game\\images\\premium\\PhoenixLightning_.png");	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	lpUpKeepItemBackImage[10] = LoadDibSurfaceOffscreen("game\\images\\premium\\PhoenixHeal_.png");	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
	lpUpKeepItemBackImage[11] = LoadDibSurfaceOffscreen("game\\images\\premium\\VPEX_.png"); // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
	lpUpKeepItemBackImage[12] = LoadDibSurfaceOffscreen("game\\images\\premium\\SPDown_.png");	// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç

	lpUpKeepItemBackImage[13] = LoadDibSurfaceOffscreen("game\\images\\premium\\TopPvp_.png");
	lpUpKeepItemBackImage[14] = LoadDibSurfaceOffscreen("game\\images\\premium\\BigHead_.png");
	lpUpKeepItemBackImage[15] = LoadDibSurfaceOffscreen("game\\images\\premium\\BigHead_.png");
	lpUpKeepItemBackImage[16] = LoadDibSurfaceOffscreen("game\\images\\premium\\BigHead_.png");

	lpUpKeepItemBackImage[17] = LoadDibSurfaceOffscreen("game\\images\\premium\\CaravanArma_.png");
	lpUpKeepItemBackImage[18] = LoadDibSurfaceOffscreen("game\\images\\premium\\CaravanHopy_.png");
	lpUpKeepItemBackImage[19] = LoadDibSurfaceOffscreen("game\\images\\premium\\CaravanBuma_.png");

	lpUpKeepItemBackImage[20] = LoadDibSurfaceOffscreen("game\\images\\premium\\SheepHat_.png");
	lpUpKeepItemBackImage[21] = LoadDibSurfaceOffscreen("game\\images\\premium\\TuttiFruttiHat_.png");
	lpUpKeepItemBackImage[22] = LoadDibSurfaceOffscreen("game\\images\\premium\\SoccerHat_.png");
	lpUpKeepItemBackImage[23] = LoadDibSurfaceOffscreen("game\\images\\premium\\XmasHat_.png");
	lpUpKeepItemBackImage[24] = LoadDibSurfaceOffscreen("game\\images\\premium\\HalloweenHat_.png");
	lpUpKeepItemBackImage[25] = LoadDibSurfaceOffscreen("game\\images\\premium\\MagicianHat_.png");
	lpUpKeepItemBackImage[26] = LoadDibSurfaceOffscreen("game\\images\\premium\\Mount_.png");
	haItemUseDelayFlag = false;
}
/*----------------------------------------------------------------------------*
*					  Main
*-----------------------------------------------------------------------------*/

int ItemUseLeftPosi[SINUPKEEPITEM_MAX][2] = {
		{ 8,80 },{ 8,116 },{ 8,152 },{ 8,188 },{ 8,224 },{ 8,260 },{ 8,296 },{ 8,332 },{ 8,368 },{ 8,404 },{ 8, 440 }
};

DWORD dwDelayTime = 0;
extern int iSecondsInCity;
extern void Utils_Log(DWORD type, char* msg, ...);

DWORD dwTimeSave = -1;

void cHaPremiumItem::Main()
{
	if (chaPremiumitem.haItemUseDelayFlag == true) {
		dwDelayTime++;
		if (dwDelayTime > 210) {
			chaPremiumitem.haItemUseDelayFlag = false;
			dwDelayTime = 0;
		}
	}

	int i, cnt = 0;
	UseClickPremiumItemFlag = false;
	for (i = 0; i < SINUPKEEPITEM_MAX; i++) {
		if (UpKeepItem[i].IconTime[0] < UpKeepItem[i].IconTime[1]) {
			if (UpKeepItem[i].Visible == true) {
				if (UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0] <= 0) {
					UpKeepItem[i].Visible = false;
					CheckUpKeepItem();
				}
				cnt++;

				//DWORD NowTime = GetPlayTime_T();
				/*
				switch( UpKeepItem[i].TGAImageNumber )
				{
					case nsPremiumItem::THIRD_EYES:
					if( m_ThirdEyesTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::EXPUP_POTION:
					if( m_ExpUpPotionTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::VAMPIRIC_CUS:
					if( m_VampiricCuspidTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;


					case nsPremiumItem::VAMPIRIC_CUS_EX:
					if( m_VampiricCuspidEXTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;


					case nsPremiumItem::MANA_RECHAR_P:
					if( m_ManaRechargingPTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::MIGHT_OF_AWELL:
					if( m_MightofAwellTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::MANA_REDUCE_P:
					if( m_ManaReducePotiontime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::PHENIX_PET:
					if( m_PhenixPetTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::HELP_PET_TERRY:
					if( m_HelpPetTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::HELP_PET_NEPSYS:
					if( m_HelpPetTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::HELP_PET_IO:
					if( m_HelpPetTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::HELP_PET_MUTE:
					if( m_HelpPetTime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::STAMINA_REDUCE_P:
					if( m_StaminaReducePotiontime > 0 && UpKeepItem[i].IconTime[0] <= 1 )
					{
						UpKeepItem[i].IconTime[0] = NowTime;
					}
					break;
					case nsPremiumItem::BIGHEAD1:
					if( m_BigHead1 > 0 && UpKeepItem[i].IconTime[0] <= 1 )
						UpKeepItem[i].IconTime[0] = NowTime;
					break;
				} */
			}
		}
		else
			UpKeepItem[i].Visible = false;

		if (UpKeepItem[i].Visible == true) {
			/*
			if(UpKeepItem[i].PosiY > ItemUseLeftPosi[cnt-1][1])
				UpKeepItem[i].PosiY-=1;
			else{
				UpKeepItem[i].PosiY = ItemUseLeftPosi[cnt-1][1];
				UpKeepItem[i].PosiX = ItemUseLeftPosi[cnt-1][0];
			} */

			if (pRealCursorPos.x > UpKeepItem[i].PosiX && pRealCursorPos.x < UpKeepItem[i].PosiX + 30 &&
				pRealCursorPos.y > UpKeepItem[i].PosiY && pRealCursorPos.y < UpKeepItem[i].PosiY + 30) {
				UpKeepItem[i].Infoflag = true;
				UseClickPremiumItemFlag = true;
			}
			else {
				UpKeepItem[i].Infoflag = false;
				UseClickPremiumItemFlag = false;
			}
		}
	}

	// Timer Novos Premium
	for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
	{
		if (UpKeepItem[i].Visible == true)
		{
			switch (UpKeepItem[i].TGAImageNumber)
			{
			case nsPremiumItem::BIGHEAD1:
				SetBigHeadTime(UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0]);
				break;
			}
		}
	}

	switch (Using_ItemKind) {
	case 8:
		if (pCursorPos.x > 100 && pCursorPos.x < 100 + 20 &&
			pCursorPos.y > 365 && pCursorPos.y < 365 + 20)
			TelePortCore.PageButtonFlag[0] = 1;
		else if (pCursorPos.x > 330 && pCursorPos.x < 330 + 20 &&
			pCursorPos.y > 365 && pCursorPos.y < 365 + 20)
			TelePortCore.PageButtonFlag[1] = 1;
		else {
			TelePortCore.PageButtonFlag[0] = 0;
			TelePortCore.PageButtonFlag[1] = 0;
		}
		break;
	}

	// deleta item premium
	// Pegar funcao confirm box aqui
	extern void ResetHairModel();
	if (MouseButton[1] && cShop.OpenFlag == FALSE && cWareHouse.OpenFlag == FALSE &&
		cCraftItem.OpenFlag == FALSE && cMixtureReset.OpenFlag == FALSE && cAging.OpenFlag == FALSE &&
		cTrade.OpenFlag == FALSE && ManufactureItem.m_OpenFlag == FALSE && SmeltingItem.OpenFlag == FALSE && !Caravana::GetInstance()->OpenFlag)
	{
		for (i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (UpKeepItem[i].Visible == true)
			{
				if (UpKeepItem[i].Infoflag)
				{
					_ConfirmationBox.SetTitle(UpKeepItem[i].Id);
					_ConfirmationBox.SetText("Deseja cancelar", "Este item?");
					_ConfirmationBox.SetCallback([i](bool isChecked)
						{
							if (isChecked)
							{
								chaPremiumitem.SetItemPremiumTime(chaPremiumitem.UpKeepItem[i].TGAImageNumber, NULL);
								switch (chaPremiumitem.UpKeepItem[i].TGAImageNumber)
								{
								case nsPremiumItem::BIGHEAD1:
								case nsPremiumItem::BIGHEAD2:
								case nsPremiumItem::BIGHEAD3:
								case nsPremiumItem::BIGHEAD4:
									chaPremiumitem.SetBigHead1Time(0);
									chaPremiumitem.SetBigHead3Time(0);
									chaPremiumitem.SetBigHead4Time(0);
									chaPremiumitem.SetBigHeadTime(0);
									chaPremiumitem.SetBigHeadType(0);
									ResetHairModel();
									cInvenTory.SetItemToChar();
									break;
								case nsPremiumItem::EXPUP_POTION: chaPremiumitem.m_ExpUpPotionTime = 0; break;
								case nsPremiumItem::HELP_PET_IO:
								case nsPremiumItem::HELP_PET_MUTE:
								case nsPremiumItem::HELP_PET_NEPSYS:
								case nsPremiumItem::HELP_PET_TERRY: chaPremiumitem.m_HelpPetTime = 0; break;
								case nsPremiumItem::MANA_RECHAR_P: chaPremiumitem.m_ManaRechargingPTime = 0; break;
								case nsPremiumItem::MANA_REDUCE_P: chaPremiumitem.m_ManaReducePotiontime = 0; break;
								case nsPremiumItem::MIGHT_OF_AWELL: chaPremiumitem.m_MightofAwellTime = 0; break;
								case nsPremiumItem::PHENIX_PET: chaPremiumitem.m_PhenixPetTime = 0; break;
								case nsPremiumItem::STAMINA_REDUCE_P: chaPremiumitem.m_StaminaReducePotiontime = 0; break;
								case nsPremiumItem::THIRD_EYES: chaPremiumitem.m_ThirdEyesTime = 0; break;
								case nsPremiumItem::VAMPIRIC_CUS: chaPremiumitem.m_VampiricCuspidTime = 0; break;
								case nsPremiumItem::VAMPIRIC_CUS_EX: chaPremiumitem.m_VampiricCuspidEXTime = 0; break;
								case nsPremiumItem::CARAVAN_ARMA: chaPremiumitem.m_CaravanArma = 0; break;
								case nsPremiumItem::CARAVAN_HOPY: chaPremiumitem.m_CaravanHopy = 0; break;
								case nsPremiumItem::CARAVAN_BUMA: chaPremiumitem.m_CaravanBuma = 0; break;
								case nsPremiumItem::HAT:
								case nsPremiumItem::HAT2:
								case nsPremiumItem::HAT3:
								case nsPremiumItem::HAT4:
								case nsPremiumItem::HAT5:
								case nsPremiumItem::HAT6:
									chaPremiumitem.m_Hat = 0;
									chaPremiumitem.HatKind = 0;
									ResetHairModel();
									cInvenTory.SetItemToChar();
									break;
								case nsPremiumItem::MOUNT:
									cMountManager.DelMount(lpCurPlayer, true);
									chaPremiumitem.m_MountTime = 0;
									chaPremiumitem.m_MountParam = 0;
								}
								smTRANS_COMMAND Packet;
								Packet.size = sizeof(Packet);
								Packet.code = smDELETE_ITEM_PREMIUM;
								Packet.LParam = chaPremiumitem.UpKeepItem[i].TGAImageNumber;
								if (smWsockServer)
									smWsockServer->Send((char*)&Packet, Packet.size, TRUE);
							}
						});
					_ConfirmationBox.Start();
				}
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*					  Draw
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::Draw()
{
	switch (Using_ItemKind) {
	case 8:
		if (TelePortCore.PageButtonFlag[0])
			DrawSprite(100, 365, chaQuest.lpQuestButtonImage[2], 0, 0, 20, 20);
		else
			DrawSprite(100, 365, chaQuest.lpQuestButtonImage[0], 0, 0, 20, 20);
		if (TelePortCore.PageButtonFlag[1])
			DrawSprite(330, 365, chaQuest.lpQuestButtonImage[3], 0, 0, 20, 20);
		else
			DrawSprite(330, 365, chaQuest.lpQuestButtonImage[1], 0, 0, 20, 20);
	}
}

DWORD dwKeepItemTime[7] = { 0, SINITEM_TIME_1HOURS, SINITEM_TIME_3HOURS, SINITEM_TIME_ONEDAY, SINITEM_TIME_SEVENDAY, SINITEM_TIME_30DAY, SINITEM_TIME_3DAY };		//½Ã°£Å×ÀÌºí

int  cHaPremiumItem::CheckTime(DWORD CurrentTime)
{

	for (int i = 0; i < 8; i++) {
		if (dwKeepItemTime[i] < CurrentTime && CurrentTime < dwKeepItemTime[i + 1]) {
			return i + 1;
		}
	}

	return false;
}

void cHaPremiumItem::SetBigHead1Time(DWORD Time)
{
	if (Time == 0)
	{
		for (int i = 0; i < SINUPKEEPITEM_MAX; i++)
		{
			if (UpKeepItem[i].TGAImageNumber == nsPremiumItem::BIGHEAD1)
			{
				UpKeepItem[i].IconTime[0] = UpKeepItem[i].IconTime[1];
			}
		}
	}
}



extern void ResetHairModel();

void cHaPremiumItem::DrawUp()
{
	int i;
	int timerCount = 0;
	for (i = 0; i < SINUPKEEPITEM_MAX; i++) {
		if (UpKeepItem[i].Visible == true) {
			UpKeepItem[i].PosiX = (WinSizeX - 10) - (40 * (timerCount / 7)) - 38;
			UpKeepItem[i].PosiY = (((WinSizeY - 235) / 2) + (40 * (timerCount % 7))) - 100;

			dsDrawTexImage(UpKeepItemTGAImage[UpKeepItem[i].TGAImageNumber - 1], UpKeepItem[i].PosiX, UpKeepItem[i].PosiY, 39, 39, UpKeepItem[i].Alpha);  //ÁÂÃø 

			timerCount++;

			if (UpKeepItem[i].Infoflag == true) {
				InfoBox(UpKeepItem[i].PosiX - 35, UpKeepItem[i].PosiY, 13, 6);
			}
			//BackBoxImage
			int    MinLen = 0;  //ÇöÀç Âï¾îÁÙ ¹é ÀÌ¹ÌÁö °è»ê
			DWORD  dwCurrentTime = UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0];
			switch (UpKeepItem[i].TGAImageNumber) {
			case nsPremiumItem::THIRD_EYES:    //½áµå ¾ÆÀÌÁî									
				switch (CheckTime(dwCurrentTime)) {
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;
				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_ThirdEyesTime = 0;
				}

				break;
			case nsPremiumItem::EXPUP_POTION:
				switch (CheckTime(dwCurrentTime)) {
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;
				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_ExpUpPotionTime = 0;
				}

				break;
			case nsPremiumItem::MIGHT_OF_AWELL:		// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;
				}
				break;
			case nsPremiumItem::MANA_REDUCE_P:		// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;

				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_ManaReducePotiontime = 0;
					chaPremiumitem.m_ManaReducePotionValue = 0;
					chaPremiumitem.SetManaReducePotionTime(chaPremiumitem.m_ManaReducePotiontime);
				}
				break;
			case nsPremiumItem::VAMPIRIC_CUS:  // pluto ¹ìÇÇ¸¯ Ä¿½ºÇÍ
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;

				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_VampiricCuspidTime = 0;
				}
				break;


			case nsPremiumItem::VAMPIRIC_CUS_EX:  //	Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;

				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_VampiricCuspidEXTime = 0;
				}
				break;


			case nsPremiumItem::MANA_RECHAR_P:  // pluto ¸¶³ª ¸®Â÷Â¡ Æ÷¼Ç
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;
				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_ManaRechargingPTime = 0;
					chaPremiumitem.SetManaRechargingPTime(chaPremiumitem.m_ManaRechargingPTime);
				}
				break;

			case nsPremiumItem::STAMINA_REDUCE_P:	// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;
				}

				if (dwCurrentTime <= 0)
				{
					chaPremiumitem.m_StaminaReducePotiontime = 0;
					chaPremiumitem.m_StaminaReducePotionValue = 0;
					chaPremiumitem.SetStaminaReducePotionTime(chaPremiumitem.m_StaminaReducePotiontime);
				}
				break;
			case nsPremiumItem::BIGHEAD1:	// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç
				switch (CheckTime(dwCurrentTime))
				{
				case 1:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_1HOURS / 32));     //ÇÑÄ­´ç ÃÊ (1½Ã°£)
					break;
				case 2:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3HOURS / 32));    //ÇÑÄ­´ç ÃÊ (3½Ã°£)
					break;
				case 3:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_ONEDAY / 32));    //ÇÑÄ­´ç ÃÊ (1ÀÏ)
					break;
				case 4:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_3DAY / 32));
					break;
				case 5:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_SEVENDAY / 32));  //ÇÑÄ­´ç ÃÊ (7ÀÏ )
					break;
				case 6:
					MinLen = (32 - (dwCurrentTime) / (SINITEM_TIME_30DAY / 32));     //ÇÑÄ­´ç ÃÊ (30ÀÏ)
					break;
				}


				if (dwCurrentTime <= 1)
				{
					SetBigHeadTime(0);
					SetBigHeadType(0);
					ResetHairModel();
					cInvenTory.SetItemToChar();
				}

				break;
			}

			if (UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0] <= 10)
			{
				if (UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_TERRY || UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_NEPSYS ||
					UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_IO || UpKeepItem[i].TGAImageNumber == nsPremiumItem::HELP_PET_MUTE)
				{
					if (UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0] <= 0) // ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					{
						chaPremiumitem.UpKeepItem[i].PosiX = 0;
						chaPremiumitem.UpKeepItem[i].PosiY = 0;
						chaPremiumitem.UpKeepItem[i].Visible = false;
						chaPremiumitem.UpKeepItem[i].TGAImageNumber = 0;
						chaPremiumitem.UpKeepItem[i].IconTime[0] = 0;
						chaPremiumitem.UpKeepItem[i].IconTime[1] = 0;
						chaPremiumitem.UpKeepItem[i].Alpha = 255;
						chaPremiumitem.UpKeepItem[i].Alphakind = 0;
						chaPremiumitem.UpKeepItem[i].Per = 0;
						memset(chaPremiumitem.UpKeepItem[i].Id, NULL, sizeof(chaPremiumitem.UpKeepItem[i].Id));

						m_HelpPetTime = 0;
						// Àåº° - Å×¸®
						sinChar->GravityScroolCheck[1] = 0;
						cHelpPet.ClosePet();
					}
				}

				if (UpKeepItem[i].Alpha == 255)
					UpKeepItem[i].Alphakind = 1;
				if (UpKeepItem[i].Alpha == 50)
					UpKeepItem[i].Alphakind = 2;
			}
			else {
				if (UpKeepItem[i].Alpha == 0) {
					UpKeepItem[i].Alphakind = 0;
				}
			}
			if (UpKeepItem[i].Alphakind == 0) {
				DrawSprite(UpKeepItem[i].PosiX + 1, UpKeepItem[i].PosiY + (33 - MinLen), lpUpKeepItemBackImage[UpKeepItem[i].TGAImageNumber - 1], 0, 32 - MinLen, 32, 32);
			}
			//FadeÈ¿°ú
			switch (UpKeepItem[i].Alphakind)
			{
			case 0:
				if (UpKeepItem[i].Alpha < 255)
					UpKeepItem[i].Alpha++;
				else
					UpKeepItem[i].Alpha = 255;
				break;
			case 1:
				if (UpKeepItem[i].Alpha > 50)
					UpKeepItem[i].Alpha -= 5;
				else
					UpKeepItem[i].Alpha = 50;
				break;
			case 2:
				if (UpKeepItem[i].Alpha < 255)
					UpKeepItem[i].Alpha += 5;
				else
					UpKeepItem[i].Alpha = 255;
				break;
			}
		}
	}
}
/*----------------------------------------------------------------------------*
*					  LButtonDown
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::LButtonDown(int x, int y)
{
	//ÅÚ·¹Æ÷Æ® ÄÚ¾î ¹öÆ° Ã³¸® // Àåº° - ÅÚ·¹Æ÷Æ®ÄÚ¾î
	if (TelePortCore.PageButtonFlag[1]) {
		if (TelePortCore.PageIndex < 3)
			TelePortCore.PageIndex++;
		cSinHelp.sinShowHelp(SIN_HELP_KIND_TELEPORT_SCROLL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), haTeleport_Field[TelePortCore.PageIndex]);
	}
	if (TelePortCore.PageButtonFlag[0]) {
		if (0 < TelePortCore.PageIndex)
			TelePortCore.PageIndex--;
		cSinHelp.sinShowHelp(SIN_HELP_KIND_TELEPORT_SCROLL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y, D3DCOLOR_RGBA(0, 15, 128, 125), haTeleport_Field[TelePortCore.PageIndex]);
	}
}
/*----------------------------------------------------------------------------*
*					  LButtonUp
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::LButtonUp(int x, int y)
{

}
DWORD dwPremiumItemCode[] = { (sinBI1 | sin01),(sinBI1 | sin02),(sinBI1 | sin03),(sinBI1 | sin04),(sinBI1 | sin05),(sinBI1 | sin06),(sinBI1 | sin07) };

int cHaPremiumItem::UsePremiumItem(int kind, DWORD ItemCode, DWORD ItemHead, DWORD ItemChkSum)
{
	Using_ItemKind = 0;
	switch (kind) {
	case 1:
		cCharStatus.InitCharStatus(1);
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 2:
		CSKILL->InitSkillPoint(1);
		haCastleSkillUseFlag = 1;
		break;
	case 3:
		CSKILL->InitSkillPoint(1);
		cCharStatus.InitCharStatus(1);
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 5:
		SetPremiumItemSkill(1);
		cInvenTory.SetItemToChar();
		break;
	case 6:
		SetPremiumItemSkill(2);
		break;
	case 7:
		SetPremiumItemSkill(3);
		break;
	case 8:
		if (!CheckWindowOpen())break;
		Using_ItemKind = kind;
		cSinHelp.sinShowHelp(SIN_HELP_KIND_TELEPORT_SCROLL, QuestMessageBoxPosi2.x, QuestMessageBoxPosi2.y, QuestMessageBoxSize2.x, QuestMessageBoxSize2.y,
			D3DCOLOR_RGBA(0, 15, 128, 125), haTeleport_Field[TelePortCore.PageIndex]);
		haCastleSkillUseFlag = 0;
		HelpBoxKindIndex = 0;
		//cSinHelp.NPCTeleportFlag = 1; // pluto
		break;
	case 9:
		haCastleSkillUseFlag = 1;
		break;
	case 10:
	case 11:
		haCastleSkillUseFlag = 0;
		break;
	case 12:
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem(nsPremiumItem::THIRD_EYES,SINITEM_TIME_ONEDAY,true,UpKeepItemName[0], 50);
		SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_ONEDAY, true, UpKeepItemName[0], 100);
		break;
	case 13:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 30);
		//SetExpUpPotionTime(SINITEM_TIME_ONEDAY);
		break;
	case 14:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_3DAY, true, UpKeepItemName[0], 100);
		//SetVampiricCuspidTime(SINITEM_TIME_SEVENDAY);
		break;
	case 15:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3DAY, true, UpKeepItemName[1], 30);
		//SetExpUpPotionTime(SINITEM_TIME_SEVENDAY);
		break;
	case 16:
		if (UseHairtintPotion(1))
			haCastleSkillUseFlag = 1;
		break;
	case 17:
		if (UseHairtintPotion(2))
			haCastleSkillUseFlag = 1;
		break;
	case 18:
		if (UseHairtintPotion(3))
			haCastleSkillUseFlag = 1;
		break;
	case 19:
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS,SINITEM_TIME_3HOURS,true,UpKeepItemName[2]);
		//SetVampiricCuspidTime(SINITEM_TIME_3HOURS);

		SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS, (SINITEM_TIME_1HOURS / 2), true, UpKeepItemName[2]);
		SetVampiricCuspidTime((SINITEM_TIME_1HOURS / 2));
		break;
	case 20:
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS,SINITEM_TIME_ONEDAY,true,UpKeepItemName[2]);
		//SetVampiricCuspidTime(SINITEM_TIME_ONEDAY);
		SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS, (SINITEM_TIME_3DAY), true, UpKeepItemName[2]);
		SetVampiricCuspidTime(SINITEM_TIME_3DAY);
		break;
	case 21:
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P,SINITEM_TIME_3HOURS,true,UpKeepItemName[3]);
		//SetManaRechargingPTime(SINITEM_TIME_3HOURS);

		SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P, (SINITEM_TIME_1HOURS / 2), true, UpKeepItemName[3]);
		SetManaRechargingPTime((SINITEM_TIME_1HOURS / 2));
		break;
	case 22:
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P,SINITEM_TIME_ONEDAY,true,UpKeepItemName[3]);	
		//SetManaRechargingPTime(SINITEM_TIME_ONEDAY);

		SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P, (SINITEM_TIME_3DAY), true, UpKeepItemName[3]);
		SetManaRechargingPTime(SINITEM_TIME_3DAY);
		break;
	case 23:
		haCastleSkillUseFlag = 1;
		break;

	case 24:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_3HOURS, true, UpKeepItemName[0], 20, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 10);

		break;
	case 25:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_ONEDAY, true, UpKeepItemName[0], 20, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 10);

		break;
	case 26:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[0], 20, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 10);

		break;
	case 27:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_30DAY, true, UpKeepItemName[0], 20, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_30DAY, true, UpKeepItemName[1], 10);

		break;
	case 28:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_3HOURS, true, UpKeepItemName[0], 100, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 20);
		break;
	case 29:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_ONEDAY, true, UpKeepItemName[0], 30, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 20);

		break;
	case 30:
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[0], 30, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 20);


		break;
	case 31: //½Ç¹ö ÆÐÅ°Áö(30ÀÏ )  
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_30DAY, true, UpKeepItemName[0], 30, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_30DAY, true, UpKeepItemName[1], 20);

		break;

	case 32: //°ñµå ÆÐÅ°Áö(3½Ã°£)  
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_3HOURS, true, UpKeepItemName[0], 40, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 30);

		break;

	case 33: //°ñµå ÆÐÅ°Áö(1ÀÏ )  
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_ONEDAY, true, UpKeepItemName[0], 40, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 30);

		break;
	case 34: //°ñµå ÆÐÅ°Áö(7ÀÏ )  
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[0], 40, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 30);

		break;
	case 35: //°ñµå ÆÐÅ°Áö(30ÀÏ )  
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_30DAY, true, UpKeepItemName[0], 40, 1))
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_30DAY, true, UpKeepItemName[1], 30);
		break;

		//-------------------------------2Â÷ Çì¾îÆ÷¼Ç ----- ¼º±Ù 060524 -------------------------
	case 36:   //Çì¾îÆ¾Æ® DÇü
		if (UseHairtintPotion(4))
			haCastleSkillUseFlag = 1;
		break;
	case 37:   //Çì¾îÆ¾Æ® EÇü
		if (UseHairtintPotion(5))
			haCastleSkillUseFlag = 1;
		break;
	case 38:   //Çì¾îÆ¾Æ® FÇü
		if (UseHairtintPotion(6))
			haCastleSkillUseFlag = 1;
		break;
	case 39:   //Çì¾îÆ¾Æ® GÇü
		if (
			UseHairtintPotion(7))
			haCastleSkillUseFlag = 1;
		break;
	case 40:   //Çì¾îÆ¾Æ® HÇü
		if (UseHairtintPotion(8))
			haCastleSkillUseFlag = 1;
		break;

		//-------------------------------3Â÷ Çì¾îÆ÷¼Ç ----- ¼º±Ù 060809 -------------------------

	case 41:   //Çì¾îÆ¾Æ® IÇü
		if (UseHairtintPotion(9))
			haCastleSkillUseFlag = 1;
		break;
	case 42:   //Çì¾îÆ¾Æ® JÇü
		if (UseHairtintPotion(10))
			haCastleSkillUseFlag = 1;
		break;
	case 43:   //Çì¾îÆ¾Æ® KÇü
		if (UseHairtintPotion(11))
			haCastleSkillUseFlag = 1;
		break;
	case 44:   //Çì¾îÆ¾Æ® LÇü
		if (UseHairtintPotion(12))
			haCastleSkillUseFlag = 1;
		break;
	case 45:   //Çì¾îÆ¾Æ® MÇü
		if (UseHairtintPotion(13))
			haCastleSkillUseFlag = 1;
		break;
		//----------------------------- º£Æ®³²¿äÃ» 50% °æÄ¡ ¾÷ --------------- //

	case 46:   //°æÇèÄ¡ Áõ°¡ Æ÷¼Ç(1ÀÏ)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 50);

		break;
	case 47:   //°æÇèÄ¡ Áõ°¡ Æ÷¼Ç(7ÀÏ)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 50);
		break;
	case 48:	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ 7ÀÏ
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[4], 300);
		break;
	case 49:	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£ 30ÀÏ
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_30DAY, true, UpKeepItemName[4], 300 );
		SetUpKeepItem(nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_3HOURS, true, UpKeepItemName[4], 300);
		break;
	case 50:	// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 1ÀÏ
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 30 );
		SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 10);
		break;
	case 51:	// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç 7ÀÏ
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[5], 30 );

		SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_3DAY, true, UpKeepItemName[5], 10);
		break;
	case 52:	// pluto ºê·ÐÁî ÆÐÅ°Áö2 3½Ã°£
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_3HOURS, true, UpKeepItemName[5], 10, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 10);
		}
		break;
	case 53:	// pluto ºê·ÐÁî ÆÐÅ°Áö2 1ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 10, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 10);
		}
		break;
	case 54:	// pluto ºê·ÐÁî ÆÐÅ°Áö2 7ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[5], 10, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 10);
		}
		break;
	case 55:	// pluto ½Ç¹ö ÆÐÅ°Áö2 3½Ã°£
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_3HOURS, true, UpKeepItemName[5], 20, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 20);
		}
		break;
	case 56:	// pluto ½Ç¹ö ÆÐÅ°Áö2 1ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 20, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 20);
		}
		break;
	case 57:	// pluto ½Ç¹ö ÆÐÅ°Áö2 7ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[5], 20, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 20);
		}
		break;
	case 58:	// pluto °ñµå ÆÐÅ°Áö2 3½Ã°£
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_3HOURS, true, UpKeepItemName[5], 30, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 30);
		}
		break;
	case 59:	// pluto °ñµå ÆÐÅ°Áö2 1ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 30, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 30);
		}
		break;
	case 60:	// pluto °ñµå ÆÐÅ°Áö2 7ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[5], 30, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 30);
		}
		break;
	case 61:	// pluto ½´Æä¸®¾î ÆÐÅ°Áö2 3½Ã°£
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_3HOURS, true, UpKeepItemName[5], 40, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 40);
		}
		break;
	case 62:	// pluto ½´Æä¸®¾î ÆÐÅ°Áö2 1ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[5], 40, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 40);
		}
		break;
	case 63:	// pluto ½´Æä¸®¾î ÆÐÅ°Áö2 7ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[5], 40, 2))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 40);
		}
		break;
	case 64:	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£2 7ÀÏ
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[4], 500 );

		SetUpKeepItem(nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_ONEDAY, true, UpKeepItemName[4], 500);
		break;
	case 65:	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£2 30ÀÏ
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_30DAY, true, UpKeepItemName[4], 500 );
		SetUpKeepItem(nsPremiumItem::MIGHT_OF_AWELL, SINITEM_TIME_3DAY, true, UpKeepItemName[4], 500);
		break;
	case 66:	// pluto ½´Æä¸®¾î ÆÐÅ°Áö 3½Ã°£
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_3HOURS, true, UpKeepItemName[0], 50, 1))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 40);
		}
		break;
	case 67:	// pluto ½´Æä¸®¾î ÆÐÅ°Áö 1ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_ONEDAY, true, UpKeepItemName[0], 50, 1))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 40);
		}
		break;
	case 68:	// pluto ½´Æä¸®¾î ÆÐÅ°Áö 7ÀÏ
		haCastleSkillUseFlag = 1;
		if (SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[0], 50, 1))
		{
			SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 40);
		}
		break;
	case 69:	// pluto Æê(ÇØ¿Ü)
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::PHENIX_PET, SINITEM_TIME_ONEDAY, true, UpKeepItemName[6], 30 );
		SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_ONEDAY, true, UpKeepItemName[6], 30);
		//cPCBANGPet.PetKind = TRUE;
		//cPCBANGPet.ShowPet();
		break;
	case 70:	// pluto Æê(ÇØ¿Ü)
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::PHENIX_PET, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[6], 30 );
		SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_3DAY, true, UpKeepItemName[6], 30);
		break;
	case 71:	// pluto Æê(ÇØ¿Ü)
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::PHENIX_PET, SINITEM_TIME_3HOURS, true, UpKeepItemName[6], 30 );
		SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_1HOURS, true, UpKeepItemName[6], 30);
		break;

		// ¹ÚÀç¿ø  72¹øÀº ºñ¿öµÒ.

	case 73:	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(¹«±â)
		haCastleSkillUseFlag = 1;
		UseAgingMaster(0);
		break;
	case 74:	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(¹æÆÐ, ¿Àºê, ºñÁî)
		haCastleSkillUseFlag = 1;
		UseAgingMaster(1);
		break;
	case 75:	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(°©¿Ê, ·Îºê)
		haCastleSkillUseFlag = 1;
		UseAgingMaster(2);
		break;
	case 76: // ¹ÚÀç¿ø - ½ºÅ³ ¸¶½ºÅÍ(1Â÷)
		haCastleSkillUseFlag = 1;
		UseSkillMaster(1); // 1Â÷ ÀüÁ÷ ½ºÅ³
		break;
	case 77: // ¹ÚÀç¿ø - ½ºÅ³ ¸¶½ºÅÍ(2Â÷)
		haCastleSkillUseFlag = 1;
		UseSkillMaster(2); // 2Â÷ ÀüÁ÷ ½ºÅ³
		break;
	case 78: // ¹ÚÀç¿ø - ½ºÅ³ ¸¶½ºÅÍ(3Â÷)
		haCastleSkillUseFlag = 1;
		UseSkillMaster(3); // 3Â÷ ÀüÁ÷ ½ºÅ³
		break;
	case 79: // ¹ÚÀç¿ø - ÀÌµ¿ »óÁ¡ ¾ÆÀÌÅÛ
		haCastleSkillUseFlag = 1;
		cInvenTory.InvenItem[Using_SelectInvenItemIndex].sItemInfo.PotionCount = 300;
		sinThrowItemToFeild(&cInvenTory.InvenItem[Using_SelectInvenItemIndex]);
		cInvenTory.InvenItem[Using_SelectInvenItemIndex].Flag = 0;
		cInvenTory.CheckWeight();//¹«°Ô¸¦ º¸Á¤ÇØÁØ´Ù
		cInvenTory.ReFormInvenItem();
		SaveGameData();
		UseMoveShop();
		break;
	case 80:   // ¹ÚÀç¿ø - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 1ÀÏ Ãß°¡)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 100);
		break;
	case 81:   // ¹ÚÀç¿ø - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 7ÀÏ Ãß°¡)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[1], 100);
		break;
	case 82: // ¹ÚÀç¿ø - Ä³¸¯ÅÍ ¼Ó¼ºº° ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ(5Á¾ - Èû)
		cCharStatus.InitCharStatus_Attribute(1); // Èû
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();//ÀçÀÎÁõ 
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 83: // ¹ÚÀç¿ø - Ä³¸¯ÅÍ ¼Ó¼ºº° ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ(5Á¾ - Á¤½Å·Â)
		cCharStatus.InitCharStatus_Attribute(2); // Á¤½Å·Â
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();//ÀçÀÎÁõ 
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 84: // ¹ÚÀç¿ø - Ä³¸¯ÅÍ ¼Ó¼ºº° ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ(5Á¾ - Àç´É)
		cCharStatus.InitCharStatus_Attribute(3); // Àç´É
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();//ÀçÀÎÁõ 
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 85: // ¹ÚÀç¿ø - Ä³¸¯ÅÍ ¼Ó¼ºº° ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ(5Á¾ - ¹ÎÃ¸¼º)
		cCharStatus.InitCharStatus_Attribute(4); // ¹ÎÃ¸¼º
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();//ÀçÀÎÁõ 
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 86: // ¹ÚÀç¿ø - Ä³¸¯ÅÍ ¼Ó¼ºº° ½ºÅÈ ÃÊ±âÈ­ ¾ÆÀÌÅÛ(5Á¾ - °Ç°­)
		cCharStatus.InitCharStatus_Attribute(5); // °Ç°­
		cInvenTory.SetItemToChar();
		if (sinChar->Life[1] < sinGetLife()) {
			sinSetLife(sinChar->Life[1]);
			ReformCharForm();//ÀçÀÎÁõ 
			ResetEnergyGraph(4);
		}
		haCastleSkillUseFlag = 1;
		break;
	case 87:   // ¹ÚÀç¿ø - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 30ÀÏ Ãß°¡)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_30DAY, true, UpKeepItemName[1], 100);
		break;
	case 88:	// ¹ÚÀç¿ø - ÇÈ´Ð½º Æê(30ÀÏ) Ãß°¡
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_30DAY, true, UpKeepItemName[6], 30);
		break;

	case 97: // ¹ÚÀç¿ø - ¿¤´õ ÄÚÆÛ ¿À¾î Ãß°¡   
	case 98: // ¹ÚÀç¿ø - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ Ãß°¡
		haCastleSkillUseFlag = 0;
		break;
	case 99:	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸®)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_TERRY, SINITEM_TIME_30DAY, true, UpKeepItemName[7], 30, 3);
		sinChar->GravityScroolCheck[1] = 1;
		break;
	case 100:	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_NEPSYS, SINITEM_TIME_30DAY, true, UpKeepItemName[8], 30, 3);
		break;
	case 101:	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_IO, SINITEM_TIME_30DAY, true, UpKeepItemName[9], 30, 3);
		break;
	case 102:	// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ®)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_MUTE, SINITEM_TIME_30DAY, true, UpKeepItemName[10], 30, 3);
		sinChar->GravityScroolCheck[1] = 2;
		break;

	case 103:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸®) // 1ÀÏ
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_TERRY, SINITEM_TIME_ONEDAY, true, UpKeepItemName[7], 30, 3);
		sinChar->GravityScroolCheck[1] = 1;
		break;
	case 104:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_NEPSYS, SINITEM_TIME_ONEDAY, true, UpKeepItemName[8], 30, 3);
		break;
	case 105:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_IO, SINITEM_TIME_ONEDAY, true, UpKeepItemName[9], 30, 3);
		break;
	case 106:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ®)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_MUTE, SINITEM_TIME_ONEDAY, true, UpKeepItemName[10], 30, 3);
		sinChar->GravityScroolCheck[1] = 2;
		break;

	case 107:   // Àåº° - °æÇèÄ¡Áõ°¡ Æ÷¼Ç(100% 1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_ONEDAY, true, UpKeepItemName[1], 50);
		break;

	case 108:	// Àåº° - ÇÇ´Ð½ºÆê(1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::PHENIX_PET, SINITEM_TIME_ONEDAY, true, UpKeepItemName[6], 30);
		break;

	case 109:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸®)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_TERRY, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[7], 30, 3);
		sinChar->GravityScroolCheck[1] = 1;
		break;
	case 110:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_NEPSYS, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[8], 30, 3);
		break;
	case 111:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_IO, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[9], 30, 3);
		break;
	case 112:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ®)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_MUTE, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[10], 30, 3);
		sinChar->GravityScroolCheck[1] = 2;
		break;
	case 113:	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - ¾Ï¸´
		haCastleSkillUseFlag = 1;
		UseAgingMaster(3);
		break;
	case 114:	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - Àå°©
		haCastleSkillUseFlag = 1;
		UseAgingMaster(4);
		break;
	case 115:	// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - ºÎÃ÷
		haCastleSkillUseFlag = 1;
		UseAgingMaster(5);
		break;

	case 116:	// Àåº° - ±×¶óºñÆ¼ ½ºÅ©·Ñ

		if (sinChar->GravityScroolCheck[0] == 5)
		{
			cMessageBox.ShowMessage(MESSAGE_GRAVITYSCROOLFAIL);
			return FALSE;
		}

		cMessageBox.ShowMessage(MESSAGE_GRAVITYSCROOLOK);

		sinChar->GravityScroolCheck[0] += 1;


		cCharStatus.UseGravityScrool();
		cInvenTory.SetItemToChar();
		ReformCharForm();

		haCastleSkillUseFlag = 1;

		break;


	case 117:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(Å×¸® 1½Ã°£) 
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_TERRY, SINITEM_TIME_1HOURS, true, UpKeepItemName[7], 30, 3);
		sinChar->GravityScroolCheck[1] = 1;
		break;
	case 118:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(³Ü½Ã½º 1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_NEPSYS, SINITEM_TIME_1HOURS, true, UpKeepItemName[8], 30, 3);
		break;
	case 119:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(ÀÌ¿À 1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_IO, SINITEM_TIME_1HOURS, true, UpKeepItemName[9], 30, 3);
		break;
	case 120:	// Àåº° - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡(¹«Æ® 1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::HELP_PET_MUTE, SINITEM_TIME_1HOURS, true, UpKeepItemName[10], 30, 3);
		sinChar->GravityScroolCheck[1] = 2;
		break;

	case 121:   // Àåº° - ½áµå ¾ÆÀÌÁî(1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::THIRD_EYES, SINITEM_TIME_1HOURS, true, UpKeepItemName[0], 100);

		break;

	case 122: // Àåº° - ½´ÆÛ ¿¡ÀÌÂ¡ ½ºÅæ 1.5
		haCastleSkillUseFlag = 0;
		break;

	case 123:   // Àåº° - °æÇèÄ¡Áõ°¡Æ÷¼Ç(1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::EXPUP_POTION, SINITEM_TIME_3HOURS, true, UpKeepItemName[1], 50);
		break;

	case 124:  // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ(1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS, SINITEM_TIME_ONEDAY, true, UpKeepItemName[2]);
		SetVampiricCuspidTime(SINITEM_TIME_ONEDAY);
		break;

	case 125:  // Àåº° - ¸¶³ª ¸®Â÷Â¡ Æ÷¼Ç(1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::MANA_RECHAR_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[3]);
		SetManaRechargingPTime(SINITEM_TIME_ONEDAY);
		break;

	case 126:	// Àåº° - ¸¶³ª ¸®µà½º(1½Ã°£)
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::MANA_REDUCE_P, (SINITEM_TIME_1HOURS / 2), true, UpKeepItemName[5], 10);
		break;

	case 127:  // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS_EX, SINITEM_TIME_1HOURS, true, UpKeepItemName[11]);
		SetVampiricCuspidEXTime(SINITEM_TIME_1HOURS);
		break;

	case 128:  // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS_EX, SINITEM_TIME_3HOURS, true, UpKeepItemName[11]);
		SetVampiricCuspidEXTime(SINITEM_TIME_3HOURS);
		break;

	case 129:  // Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::VAMPIRIC_CUS_EX, SINITEM_TIME_ONEDAY, true, UpKeepItemName[11]);
		SetVampiricCuspidEXTime(SINITEM_TIME_ONEDAY);
		break;
	case 130:	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â ½ºÅæ
		haCastleSkillUseFlag = 0;
		break;
	case 131: // ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæ
		haCastleSkillUseFlag = 0;
		break;
	case 132: // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç(1ÀÏ)
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::STAMINA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[12], 30 ); //SINITEM_TIME_ONEDAY
		SetUpKeepItem(nsPremiumItem::STAMINA_REDUCE_P, (SINITEM_TIME_1HOURS / 2), true, UpKeepItemName[12], 10);
		break;
	case 133: // ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç(7ÀÏ)
		haCastleSkillUseFlag = 1;
		//SetUpKeepItem( nsPremiumItem::STAMINA_REDUCE_P, SINITEM_TIME_SEVENDAY, true, UpKeepItemName[12], 30 ); //SINITEM_TIME_SEVENDAY
		SetUpKeepItem(nsPremiumItem::STAMINA_REDUCE_P, SINITEM_TIME_3HOURS, true, UpKeepItemName[12], 10);
		break;
	case 134: // ¹ÚÀç¿ø - ¸®½ºÆå ½ºÅæ
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::STAMINA_REDUCE_P, SINITEM_TIME_ONEDAY, true, UpKeepItemName[12], 10);
		break;
	case 136:
		if (GetBigHeadType() != 0)
			break;

		SetUpKeepItem(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY, true, UpKeepItemName[13], 30);
		SetItemPremiumTime(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY);
		SetBigHeadType(1);
		//ChangeHairModel( 13 );
	case 137:
		if (GetBigHeadType() != 0)
			break;

		SetUpKeepItem(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY, true, UpKeepItemName[13], 30);
		SetItemPremiumTime(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY);
		SetBigHeadType(2);
		//ChangeHairModel( 14 );
	case 138:
		if (GetBigHeadType() != 0)
			break;

		SetUpKeepItem(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY, true, UpKeepItemName[13], 30);
		SetItemPremiumTime(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY);
		SetBigHeadType(3);
		//ChangeHairModel( 15 );
	case 139:
		if (GetBigHeadType() != 0)
			break;

		SetUpKeepItem(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY, true, UpKeepItemName[13], 30);
		SetItemPremiumTime(nsPremiumItem::BIGHEAD1, SINITEM_TIME_ONEDAY);
		SetBigHeadType(4);
		//ChangeHairModel( 16 );
		break;// novos cabelos xxstr
	case 140:
		if (UseHairtintPotion(14))
			haCastleSkillUseFlag = 1;
		break;
	case 141:
		if (UseHairtintPotion(15))
			haCastleSkillUseFlag = 1;
		break;
	case 142:
		if (UseHairtintPotion(16))
			haCastleSkillUseFlag = 1;
		break;
	case 143:
		if (UseHairtintPotion(17))
			haCastleSkillUseFlag = 1;
		break;
	case 144:
		if (UseHairtintPotion(18))
			haCastleSkillUseFlag = 1;
		break;
	case 145:
		if (UseHairtintPotion(19))
			haCastleSkillUseFlag = 1;
		break;
	case 146:
		if (UseHairtintPotion(20))
			haCastleSkillUseFlag = 1;
		break;

		//Lucky Box
	case 147:
		haCastleSkillUseFlag = 1;
		UseLuckyBox(1, ItemCode, ItemHead, ItemChkSum);
		break;
	case 148:
		haCastleSkillUseFlag = 1;
		UseLuckyBox(2, ItemCode, ItemHead, ItemChkSum);
		break;
	case 149:
		haCastleSkillUseFlag = 1;
		UseLuckyBox(3, ItemCode, ItemHead, ItemChkSum);
		break;

	case 150:
		haCastleSkillUseFlag = 1;
		UseLuckyBox(4, ItemCode, ItemHead, ItemChkSum);
		break;
	case 151:
		haCastleSkillUseFlag = 1;
		UseLuckyBox(5, ItemCode, ItemHead, ItemChkSum);
		break;
	case 152:
		haCastleSkillUseFlag = 1;
		UseLuckyBox(6, ItemCode, ItemHead, ItemChkSum);
		break;
	case 153:
		if (UseValeLevel(1, ItemCode, ItemHead, ItemChkSum))
			haCastleSkillUseFlag = 1;
		break;
	case 154:
		if (UseValeLevel(2, ItemCode, ItemHead, ItemChkSum))
			haCastleSkillUseFlag = 1;
		break;

		// Caravanas
	case 155:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::CARAVAN_ARMA, (60 * 60 * 24 * 3), true, "Caravana Arma");

		Caravana::GetInstance()->myCaravan.timeRemaining = (60 * 60 * 24 * 3);
		break;

	case 156:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::CARAVAN_HOPY, (60 * 60 * 24 * 3), true, "Caravana Hopi");

		Caravana::GetInstance()->myCaravan.timeRemaining = (60 * 60 * 24 * 3);
		break;

	case 157:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::CARAVAN_BUMA, (60 * 60 * 24 * 3), true, "Caravana Buma");

		Caravana::GetInstance()->myCaravan.timeRemaining = (60 * 60 * 24 * 3);
		break;

	case 158:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::CARAVAN_ARMA, (60 * 60 * 24 * 14), true, "Caravana Arma");

		Caravana::GetInstance()->myCaravan.timeRemaining = (60 * 60 * 24 * 14);
		break;

	case 159:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::CARAVAN_HOPY, (60 * 60 * 24 * 14), true, "Caravana Hopi");

		Caravana::GetInstance()->myCaravan.timeRemaining = (60 * 60 * 24 * 14);
		break;

	case 160:
		haCastleSkillUseFlag = 1;
		SetUpKeepItem(nsPremiumItem::CARAVAN_BUMA, (60 * 60 * 24 * 14), true, "Caravana Buma");

		Caravana::GetInstance()->myCaravan.timeRemaining = (60 * 60 * 24 * 14);
		break;
	case 161:
		if (!m_Hat && ChangeBigHead(1, SINITEM_TIME_ONEDAY))
		{
			haCastleSkillUseFlag = 1;
			lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
			ReformCharForm();
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;
	case 162:
		if (!m_Hat && ChangeBigHead(2, SINITEM_TIME_ONEDAY))
		{
			haCastleSkillUseFlag = 1;
			lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
			ReformCharForm();
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;
	case 163:
		if (!m_Hat && ChangeBigHead(3, SINITEM_TIME_ONEDAY))
		{
			haCastleSkillUseFlag = 1;
			lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
			ReformCharForm();
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;
	case 164:
		if (!m_Hat && ChangeBigHead(4, SINITEM_TIME_ONEDAY))
		{
			haCastleSkillUseFlag = 1;
			lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
			ReformCharForm();
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;
	case 165:
		if (!m_Hat && ChangeBigHead(5, SINITEM_TIME_ONEDAY))
		{
			haCastleSkillUseFlag = 1;
			lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
			ReformCharForm();
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;
	case 166:
		if (!m_Hat && ChangeBigHead(6, SINITEM_TIME_ONEDAY))
		{
			haCastleSkillUseFlag = 1;
			lpCurPlayer->smCharInfo.bUpdateInfo[0]++;
			ReformCharForm();
			SendSetObjectSerial(lpCurPlayer->dwObjectSerial);
		}
		break;
	case 167:
		if (UseValeLevel(3, ItemCode, ItemHead, ItemChkSum))
			haCastleSkillUseFlag = 1;
		break;
	case 168:
		if (UseValeLevel(4, ItemCode, ItemHead, ItemChkSum))
			haCastleSkillUseFlag = 1;
		break;

	}
	return TRUE;
}
/*----------------------------------------------------------------------------*
*Desc: ½ºÅ³ °ÔÀÌÁö°¡ »ý¼ºµÅ´Â  ¾ÆÀÌÅÛÀÏ°æ¿ì È£Ãâ
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetPremiumItemSkill(int kind)
{

	haCastleSkillUseFlag = 0;
	DWORD SkillCODE;
	int   SkillTime;
	switch (kind) {
	case 1:
		SkillCODE = SCROLL_P_INVULNERABILITY;
		SkillTime = 30;
		break;
	case 2:
		SkillCODE = SCROLL_P_CRITICAL;
		SkillTime = (60 * 60 * 0.5);//60*10;	//pluto 1½Ã°£À¸·Î Áõ°¡
		break;
	case 3:
		SkillCODE = SCROLL_P_EVASION;
		SkillTime = (60 * 60 * 0.5);//60*10;	//pluto 1½Ã°£À¸·Î Áõ°¡
		break;
	}
	//ÇØ´ç µÅ´Â ¾ÆÀÌÅÛÀÌ ¸ÂÀ¸¸é ¾ÆÀÌÅÛ½ºÅ³À» ¼¼ÆÃÇÑ´Ù.
	sSKILL TempSkill;

	if (CSKILL->SearchContiueSkillCODE(SkillCODE) == SkillCODE && SkillCODE != 0) {
		cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
		haCastleSkillUseFlag = 0;
		return TRUE;
	}
	haCastleSkillUseFlag = 1;

	for (int j = 0; j < SIN_MAX_SKILL; j++) {
		if (sSkill[j].CODE == SkillCODE) {
			memcpy(&TempSkill, &sSkill[j], sizeof(sSKILL));
			TempSkill.UseTime = SkillTime;
			sinContinueSkillSet(&TempSkill);
			SwitchSkill(&TempSkill);
			break;
		}
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*
*Desc: ÅÚ·¹Æ÷Æ® ÄÚ¾î »ç¿ë½Ã È£Ãâ
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::UseTeleportScrollItem(short ItemIndex)
{
	if (!chaPremiumitem.TelePortCore.FieldIndex[1])return FALSE;

	if (StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_ROOM)		// ¾Ë±î±â ¸Ê¿¡¼­ ÅÚ·¹Æ÷Æ® ÄÚ¾î ¸·´Â°Å
	{
		cMessageBox.ShowMessage(MESSAGE_NO_USE_ITEM); //ÅÚ·¹Æ÷Æ® ºÒ°¡
		return FALSE;
	}

	int i;
	for (i = 0; i < TELEPORT_FIELDMAX; i++)
	{
		if ((chaPremiumitem.TelePortCore.FieldIndex[1] - 1) == TelePort_FieldNum[i][0]) {

			if (sField[TelePort_FieldNum[i][1]].LimitLevel <= sinChar->Level) // Àåº° - ÇÊµå ·¹º§ Á¦ÇÑ ÇØÁ¦ ÀÌº¥Æ® Á¾·á½Ã ¿øº¹
			{
				//	if( i < 10 || sinChar->Level > 19 )		// <2010.04.29> ¼®Áö¿ë - Á¦º§Á¦ÇÑ ¾ø´Â ÇÊµå¸é ¿öÇÁ or ·¹º§Á¦ÇÑ ÀÖ´Â ÇÊµå(´øÀü)ÀÌ¸é ·¹º§20ºÎÅÍ ¿öÇÁ // Àåº° - ÇÊµå ·¹º§ Á¦ÇÑ ÇØÁ¦ ÀÌº¥Æ®
				//	{
				WarpField2(TelePort_FieldNum[i][1]);              //¿öÇÁÇÑ´Ù.
				cInvenTory.InvenItem[chaPremiumitem.TelePortCore.ItemIndex].sItemInfo.PotionCount = 300;
				sinThrowItemToFeild(&cInvenTory.InvenItem[chaPremiumitem.TelePortCore.ItemIndex]);
				cInvenTory.InvenItem[chaPremiumitem.TelePortCore.ItemIndex].Flag = 0;
				cInvenTory.CheckWeight();//¹«°Ô¸¦ º¸Á¤ÇØÁØ´Ù
				cInvenTory.ReFormInvenItem();
				//	}
			}

			else
				cMessageBox.ShowMessage(MESSAGE_NOT_ENOUGH_LEVEL); //·¹º§·Î ÀÎÇØ ÀÌµ¿ÇÒ¼ö ¾ø´Ù.
		}
	}
	return TRUE;
}
/*----------------------------------------------------------------------------*
*Desc: ´Ù¸¥ ÀÎÅÍÆäÀÌ½ºÃ¢ÀÌ ¶°ÀÕÀ»°æ¿ì ¸®ÅÏÇÑ´Ù.
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckWindowOpen()
{
	haCastleSkillUseFlag = 0;
	if (cWareHouse.OpenFlag)return false;
	if (cTrade.OpenFlag)return false;
	if (cAging.OpenFlag)return false;
	if (cCraftItem.OpenFlag)return false;
	if (sSinWarpGate.ShowFlag)return false;
	if (cSinHelp.NPCTeleportFlag)return false;
	if (isDrawClanMenu)return false; //Å¬·£¸Þ´º°¡ ÀÖÀ»¶§´Â ¸®ÅÏÇÑ´Ù  
	if (haMovieFlag)return false;
	if (Caravana::GetInstance()->OpenFlag) return false;
	if (WarMode.fOpen)return false;
	// pluto Á¦·Ã
	if (SmeltingItem.OpenFlag)
	{
		return false;
	}
	// pluto Á¦ÀÛ
	if (ManufactureItem.m_OpenFlag)
	{
		return false;
	}
	if (cMixtureReset.OpenFlag)	return false;	// ¼®Áö¿ë - ¹Í½ºÃÄ ¸®¼Â Ãß°¡
	return true;
}
/*----------------------------------------------------------------------------*
*Desc: ºó¹è¿­À» ´ó°ÜÁØ´Ù.
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::CheckUpKeepItem()
{
	int i;
	//¾ÆÀÌÅÛÃ¼Å©
	for (i = 0; i < SINUPKEEPITEM_MAX; i++) {
		if (UpKeepItem[i].Visible == false) {
			if (UpKeepItem[i + 1].Visible == true) {
				memcpy(&UpKeepItem[i], &UpKeepItem[i + 1], sizeof(sUpKeepItem));
				UpKeepItem[i + 1].Visible = false;
				break;
			}
		}
	}
	return true;
}
/*----------------------------------------------------------------------------*
*Desc: À¯ÁöÇü ¼Ò¸ð¼º ¾ÆÀÌÅÛÀ» »ç¿ëÇÒ °æ¿ì µð½ºÇÃ·¹ÀÌ
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::SetUpKeepItem(int kind, DWORD dwTime, bool visible, char* id, int Percent, short ItemType)
{
	haCastleSkillUseFlag = 0;
	int i;

	for (i = 0; i < SINUPKEEPITEM_MAX; i++)
	{
		if (m_VampiricCuspidEXTime > 0 && UpKeepItem[i].Visible == false)
		{
			if (UpKeepItem[i].TGAImageNumber == VAMPIRIC_CUS_EX)
			{
				UpKeepItem[i].TGAImageNumber = 0;
				UpKeepItem[i].Visible = false;
				haCastleSkillUseFlag = 0;
				//	return FALSE;
			}
		}

		if ((UpKeepItem[i].TGAImageNumber == VAMPIRIC_CUS_EX || UpKeepItem[i].TGAImageNumber == VAMPIRIC_CUS) && (m_VampiricCuspidEXTime > 0 || m_VampiricCuspidTime > 0))
		{
			if (kind == 3 || kind == 12)
			{
				cMessageBox.ShowMessage(MESSAGE_ASATTRIBUTE_USE);
				haCastleSkillUseFlag = 0;
				return  FALSE;
			}
		}
	}



	for (i = 0; i < SINUPKEEPITEM_MAX; i++)
	{
		if (UpKeepItem[i].Visible == false)
		{

			UpKeepItem[i].TGAImageNumber = kind;     //ÀÌ¹ÌÁö ÀÎµ¦½º
			UpKeepItem[i].Visible = true;
			UpKeepItem[i].IconTime[0] = 1;
			UpKeepItem[i].IconTime[1] = GetPlayTime_T() + dwTime;
			UpKeepItem[i].Alpha = 0;
			UpKeepItem[i].Alphakind = 0;
			UpKeepItem[i].Per = Percent;	//ÆÛ¼¾Æ® ¿É¼ÇÀ» Âï±âÀ§ÇØ Ãß°¡ 

			strcpy(UpKeepItem[i].Id, id);
			haCastleSkillUseFlag = 1;
			return TRUE;
		}
		else {



			/*
				ItemType  0 ÀÏ¹Ý ¾ÆÀÌÅÛ
				ItemType  1 packge ¾ÆÀÌÅÛ
			*/
			switch (ItemType) {
			case 0:
				if (UpKeepItem[i].TGAImageNumber == kind) {
					cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
					haCastleSkillUseFlag = 0;
					return FALSE;
				}

				break;
			case 1:
				for (int j = 0; j < SINUPKEEPITEM_MAX; j++) {
					if (UpKeepItem[j].TGAImageNumber == THIRD_EYES || UpKeepItem[j].TGAImageNumber == EXPUP_POTION) {
						cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
						haCastleSkillUseFlag = 0;
						return  FALSE;
					}
				}
				break;
			case 2:		// pluto  ¸¶³ª ¸®µà½º Æ÷¼Ç ÆÐÅ°Áö2
				for (int j = 0; j < SINUPKEEPITEM_MAX; j++) {
					if (UpKeepItem[j].TGAImageNumber == MANA_REDUCE_P || UpKeepItem[j].TGAImageNumber == EXPUP_POTION) {
						cMessageBox.ShowMessage(MESSAGE_CLANSKILL_USE);
						haCastleSkillUseFlag = 0;
						return  FALSE;
					}
				}
				break;
			case 3:
				for (int j = 0; j < SINUPKEEPITEM_MAX; j++)
				{
					if (cHelpPet.PetShow == 1)
					{
						cMessageBox.ShowMessage(MESSAGE_ASATTRIBUTE_USE);
						haCastleSkillUseFlag = 0;
						return  FALSE;
					}
				}
				break;
			}
		}
	}
	return TRUE;

}
/*----------------------------------------------------------------------------*
*Desc: À¯ÁöÇü¾ÆÀÌÄÜ ¼³¸í¹Ú½º
*-----------------------------------------------------------------------------*/
void cHaPremiumItem::InfoBox(int PoisX, int PosiY, int Width, int Height)
{
	for (int i = 0; i < Width; i++)
	{
		for (int j = 0; j < Height; j++)
		{
			if (i == 0 && j == 0)
				dsDrawTexImage(cItem.MatItemInfoBox_TopRight, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
			if (j == 0 && i != 0 && i + 1 < Width)
				dsDrawTexImage(cItem.MatItemInfoBox_TopCenter, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
			if (j == 0 && i + 1 == Width)
				dsDrawTexImage(cItem.MatItemInfoBox_TopLeft, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);

			if (i == 0 && j != 0 && j + 1 != Height)
				dsDrawTexImage(cItem.MatItemInfoBox_Right, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
			if (i != 0 && j != 0 && i + 1 != Width && j + 1 != Height)
				dsDrawTexImage(cItem.MatItemInfoBox_Center, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
			if (i + 1 == Width && j != 0 && j + 1 != Height)
				dsDrawTexImage(cItem.MatItemInfoBox_Left, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);

			if (i == 0 && j + 1 == Height)
				dsDrawTexImage(cItem.MatItemInfoBox_BottomRight, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
			if (i != 0 && j + 1 == Height && i + 1 != Width)
				dsDrawTexImage(cItem.MatItemInfoBox_BottomCenter, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
			if (j + 1 == Height && i + 1 == Width)
				dsDrawTexImage(cItem.MatItemInfoBox_BottomLeft, PoisX - (i * 16), PosiY + (j * 16), 16, 16, 255);
		}
	}
}

/*----------------------------------------------------------------------------*
*Desc: À¯ÁöÇü¾ÆÀÌÄÜ ¼³¸í(ÀÌ¸§/À¯Áö½Ã°£ )
*-----------------------------------------------------------------------------*/
int cHaPremiumItem::DrawText()
{

	HDC	hdc = NULL;
	//lpDDSBack->GetDC( &hdc );
	SelectObject(hdc, sinBoldFont);
	//SetBkMode( hdc, TRANSPARENT );
	SetFontTextColor(RGB(255, 255, 255));
	char strBuff[32];
	memset(strBuff, 0, sizeof(strBuff));
	int i, cnt = 0;
	int pExtra = 0;
	for (i = 0; i < SINUPKEEPITEM_MAX; i++) {
		if (cSinHelp.sinGetHelpShowFlag())break;
		if (!CheckWindowOpen())break;
		if (UpKeepItem[i].Visible == true) {
			if (UpKeepItem[i].Infoflag == true) {
				SelectObject(hdc, sinFont);
				SetFontTextColor(RGB(218, 255, 255));

				// Descricao do item
				dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 10, UpKeepItem[i].Id, strlen(UpKeepItem[i].Id));

				SelectObject(hdc, sinFont);
				SetFontTextColor(RGB(255, 255, 255));
				switch (UpKeepItem[i].TGAImageNumber) {
				case nsPremiumItem::THIRD_EYES:   ///½áµå ¾ÆÀÌÁî
					wsprintf(strBuff, UpKeepItemDoc[1], UpKeepItem[i].Per, "%");
					break;
				case nsPremiumItem::EXPUP_POTION: ///°æÇèÄ¡ Áõ°¡ Æ÷¼Ç
					wsprintf(strBuff, UpKeepItemDoc[2], UpKeepItem[i].Per, "%");
					break;
				case nsPremiumItem::VAMPIRIC_CUS:
					wsprintf(strBuff, UpKeepItemDoc[4]);
					break;
				case nsPremiumItem::MANA_RECHAR_P:
					wsprintf(strBuff, UpKeepItemDoc[5]);
					break;
				case nsPremiumItem::PHENIX_PET:
					wsprintf(strBuff, "+20%% De Experiência");
					break;
				case nsPremiumItem::MIGHT_OF_AWELL:	// pluto ¸¶ÀÌÆ® ¿Àºê ¾ÆÀ£
					wsprintf(strBuff, UpKeepItemDoc[6], UpKeepItem[i].Per);
					break;
				case nsPremiumItem::MANA_REDUCE_P:		// pluto ¸¶³ª ¸®µà½º Æ÷¼Ç
					wsprintf(strBuff, UpKeepItemDoc[7], UpKeepItem[i].Per, "%");
					break;
				case nsPremiumItem::HELP_PET_TERRY:		// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					wsprintf(strBuff, UpKeepItemDoc[8]);
					break;
				case nsPremiumItem::HELP_PET_NEPSYS:		// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					wsprintf(strBuff, UpKeepItemDoc[9]);
					break;
				case nsPremiumItem::HELP_PET_IO:		// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					wsprintf(strBuff, UpKeepItemDoc[10]);
					break;
				case nsPremiumItem::HELP_PET_MUTE:		// ¹ÚÀç¿ø - ºô¸µ µµ¿ì¹Ì Æê Ãß°¡
					wsprintf(strBuff, UpKeepItemDoc[11]);
					break;
				case nsPremiumItem::VAMPIRIC_CUS_EX:		// Àåº° - ¹ìÇÇ¸¯ Ä¿½ºÇÍ EX
					wsprintf(strBuff, UpKeepItemDoc[12]);
					break;
				case nsPremiumItem::STAMINA_REDUCE_P:		// ¹ÚÀç¿ø - ±Ù·Â ¸®µà½º Æ÷¼Ç
					wsprintf(strBuff, UpKeepItemDoc[13], UpKeepItem[i].Per, "%");
					break;
				case nsPremiumItem::BIGHEAD1:
					wsprintf(strBuff, "+3%% Poder de Ataque");
					break;
				case nsPremiumItem::CARAVAN_ARMA:
					wsprintf(strBuff, "Guarde seus itens comigo!");
					//dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 48, "/seguir -> Vou até você!", strlen("/seguir -> Vou até você!"));
					//dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 63, "/ficar -> Fico onde estou!", strlen("/ficar -> Fico onde estou!"));
					pExtra += 2;
					break;
				case nsPremiumItem::CARAVAN_HOPY:
					wsprintf(strBuff, "Guarde seus itens comigo!");
					//dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 48, "/seguir -> Vou até você!", strlen("/seguir -> Vou até você!"));
					//dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 63, "/ficar -> Fico onde estou!", strlen("/ficar -> Fico onde estou!"));
					pExtra += 2;
					break;
				case nsPremiumItem::CARAVAN_BUMA:
					wsprintf(strBuff, "Guarde seus itens comigo!");
					//dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 48, "/seguir -> Vou até você!", strlen("/seguir -> Vou até você!"));
					//dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 63, "/ficar -> Fico onde estou!", strlen("/ficar -> Fico onde estou!"));
					pExtra += 2;
					break;
				case nsPremiumItem::HAT:
					wsprintf(strBuff, "+100 de Vida");
					break;
				case nsPremiumItem::HAT2:
					wsprintf(strBuff, "+5%% Chance de Crítico");
					break;
				case nsPremiumItem::HAT3:
					wsprintf(strBuff, "+5 Vel Movimento");
					break;
				case nsPremiumItem::HAT4:
					wsprintf(strBuff, "+4 HP Regen.");
					break;
				case nsPremiumItem::HAT5:
					wsprintf(strBuff, "+4 SP Regen.");
					break;
				case nsPremiumItem::HAT6:
					wsprintf(strBuff, "+4 Mana Regen.");
					break;
				case nsPremiumItem::MOUNT:
					wsprintf(strBuff, "Mova-se rápido e com estilo");
					break;
				}

				dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 44 - pExtra, strBuff, strlen(strBuff));

				int hour = (UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0]) / 3600;
				int min = ((UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0]) / 60) % 60;
				int sec = (UpKeepItem[i].IconTime[1] - UpKeepItem[i].IconTime[0]) % 60;

				wsprintf(strBuff, UpKeepItemDoc[0], hour, min, sec);
				SelectObject(hdc, sinFont);
				SetFontTextColor(RGB(50, 250, 50));
				dsTextLineOut(hdc, UpKeepItem[i].PosiX - 20 - 12 * 16, UpKeepItem[i].PosiY + 64 + pExtra, strBuff, strlen(strBuff));
			}
		}
	}

	//lpDDSBack->ReleaseDC( hdc );
	return TRUE;
}

void cHaPremiumItem::UpdatePremiumTime()
{
	for (int j = 0; j < SINUPKEEPITEM_MAX; j++)
	{
		if (UpKeepItem[j].Visible == true)
		{
			if (UpKeepItem[j].IconTime[0] > 1)
				UpKeepItem[j].IconTime[0]++;
		}
	}
}


int cHaPremiumItem::UseValeLevel(int levelToGo, DWORD ItemCode, DWORD ItemHead, DWORD ItemChkSum)
{
	switch (funcValeLevel(levelToGo, ItemCode, ItemHead, ItemChkSum))
	{
	case 0:
		return FALSE;
	case -2:
		TitleBox::GetInstance()->SetText("Você precisa relogar para utilizar novamente!");
		return FALSE;
	}

	return TRUE;
}

int cHaPremiumItem::UseLuckyBox(int BoxType, DWORD ItemCode, DWORD ItemHead, DWORD ItemChkSum)
{
	switch (BoxType) {
	case 1:
		if (funcLuckyBox(1, ItemCode, ItemHead, ItemChkSum))
			return TRUE;
		break;
	case 2:
		if (funcLuckyBox(2, ItemCode, ItemHead, ItemChkSum))
			return TRUE;
		break;
	case 3:
		if (funcLuckyBox(3, ItemCode, ItemHead, ItemChkSum))
			return TRUE;
		break;
	case 4:
		if (funcLuckyBox(4, ItemCode, ItemHead, ItemChkSum))
			return TRUE;
		break;
	case 5:
		if (funcLuckyBox(5, ItemCode, ItemHead, ItemChkSum))
			return TRUE;
		break;
	case 6:
		if (funcLuckyBox(6, ItemCode, ItemHead, ItemChkSum))
			return TRUE;
		break;

		return FALSE;
	}

	return FALSE;
}

int cHaPremiumItem::UseHairtintPotion(int ItemKind)
{
	if (GetBigHeadType() != 0 || m_Hat)
		return false;

	switch (ItemKind) {
	case 1:  //AÇü
		if (ChangeHairModel(0))
			return TRUE;
		break;
	case 2:  //BÇü
		if (ChangeHairModel(1))
			return TRUE;
		break;
	case 3:  //CÇü
		if (ChangeHairModel(2))
			return TRUE;
		break;
	case 4:  //DÇü
		if (ChangeHairModel(3))
			return TRUE;
		break;
	case 5:  //EÇü
		if (ChangeHairModel(4))
			return TRUE;
		break;
	case 6:  //FÇü
		if (ChangeHairModel(5))
			return TRUE;
		break;
	case 7:  //GÇü
		if (ChangeHairModel(6))
			return TRUE;
		break;
	case 8:  //HÇü
		if (ChangeHairModel(7))
			return TRUE;
		break;

	case 9:  //IÇü
		if (ChangeHairModel(8))
			return TRUE;
		break;
	case 10:  //JÇü
		if (ChangeHairModel(9))
			return TRUE;
		break;
	case 11:  //KÇü
		if (ChangeHairModel(10))
			return TRUE;
		break;
	case 12:  //LÇü
		if (ChangeHairModel(11))
			return TRUE;
		break;
	case 13:  //MÇü
		if (ChangeHairModel(12))
			return TRUE;
		break;
	case 14: //novos cabelos xxstr
		if (ChangeHairModel(17))
			return TRUE;
		break;
	case 15:
		if (ChangeHairModel(18))
			return TRUE;
		break;
	case 16:
		if (ChangeHairModel(19))
			return TRUE;
		break;
	case 17:
		if (ChangeHairModel(20))
			return TRUE;
		break;
	case 18:
		if (ChangeHairModel(21))
			return TRUE;
		break;

	}
	cMessageBox.ShowMessage(MESSAGE_NOT_HAIRTINTPOTION); //°°Àº¸Ó¸®´É »ç¿ëÇÒ¼ö ¾ø½À´Ï´Ù.
	return FALSE;
}

// ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ ¾ÆÀÌÅÛ »ç¿ë
int cHaPremiumItem::UseAgingMaster(int ItemKind)
{
	switch (ItemKind)
	{
	case 0:
		sinCheckAgingLevel(SIN_AGING_ATTACK, true);
		sinCheckAgingLevel(SIN_AGING_CRITICAL, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		break;
	case 1:
		if ((cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2) == sinDS1)
		{
			sinCheckAgingLevel(SIN_AGING_BLOCK, true);
			ReformCharForm(); //ÀçÀÎÁõ 
			cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		}
		else if ((cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2) == sinOM1)
		{
			sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, true);
			ReformCharForm(); //ÀçÀÎÁõ 
			cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		}
		break;
	case 2:
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		break;
	case 3: // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - ¾Ï¸´
		sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		break;
	case 4: // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - Àå°©
		sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		break;
	case 5: // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¸¶½ºÅÍ(2Â÷) - ºÎÃ÷
		sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, true);
		ReformCharForm(); //ÀçÀÎÁõ 
		cMessageBox.ShowMessage(MESSAGE_AGING_MATURE_SUCCESS);
		break;
	}

	return TRUE;
}

// Pedras de mature
int cHaPremiumItem::UseSkillMaster(int ItemKind)
{
	switch (ItemKind)
	{
	case 1:
		for (int j = 1; j < 5; j++)
		{
			if (sinSkill.UseSkill[j].UseSkillCount < 10000)
			{
				if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					CSKILL->ReformSkillMasteryForm(0, j);
				}
			}
		}
		cMessageBox.ShowMessage(MESSAGE_SKILL_MATURE_SUCCESS);
		sinPlaySound(SIN_SOUND_EAT_POTION2);
		sinEffect_Agony(lpCurPlayer);
		break;
	case 2:
		for (int j = 5; j < 9; j++)
		{
			if (sinSkill.UseSkill[j].UseSkillCount < 10000)
			{
				if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					CSKILL->ReformSkillMasteryForm(0, j);
				}
			}
		}
		cMessageBox.ShowMessage(MESSAGE_SKILL_MATURE_SUCCESS);
		sinPlaySound(SIN_SOUND_EAT_POTION2);
		sinEffect_Agony(lpCurPlayer);
		break;
	case 3:
		for (int j = 9; j < 13; j++)
		{
			if (sinSkill.UseSkill[j].UseSkillCount < 10000)
			{
				if (sinSkill.UseSkill[j].Flag && sinSkill.UseSkill[j].Point)
				{
					sinSkill.UseSkill[j].UseSkillCount += 10000;
					CSKILL->ReformSkillMasteryForm(0, j);
				}
			}
		}
		cMessageBox.ShowMessage(MESSAGE_SKILL_MATURE_SUCCESS);
		sinPlaySound(SIN_SOUND_EAT_POTION2);
		sinEffect_Agony(lpCurPlayer);
		break;
	}

	return TRUE;
}

int cHaPremiumItem::UseMoveShop()
{
	if (cShop.OpenFlag == SIN_CLOSE)
	{
		sinShopKind = 1;
		cShop.OpenFlag = SIN_OPEN;
		cShop.UseItemFlag = 1;
	}
	else
	{
		cShop.OpenFlag = SIN_CLOSE;
		cShop.UseItemFlag = 0;
	}

	return TRUE;
}
// faces das novas big
char* szFaceNames[8][6] = {
	{
		"char\\tmABCD\\Tmh-B25.inf",
		"char\\tmABCD\\CarnivalHatFS.inf",
		"char\\tmABCD\\TmhB-18t.inf",
		"char\\tmABCD\\fsxmas.inf",
		"char\\tmABCD\\Tmh-B27.inf",
		"char\\tmABCD\\CartolaFS.inf",
	},
	{
		"char\\tmABCD\\Tmh-A25.inf",
		"char\\tmABCD\\CarnivalHatMS.inf",
		"char\\tmABCD\\TmhA-18t.inf",
		"char\\tmABCD\\msxmas.inf",
		"char\\tmABCD\\Tmh-A27.inf",
		"char\\tmABCD\\CartolaMS.inf",
	},
	{
		"char\\tmABCD\\Tfh-D25.inf",
		"char\\tmABCD\\CarnivalHatAS.inf",
		"char\\tmABCD\\TfhD-18t.inf",
		"char\\tmABCD\\asxmas.inf",
		"char\\tmABCD\\Tfh-D27.inf",
		"char\\tmABCD\\ar0nAS.inf",
	},
	{
		"char\\tmABCD\\Tmh-C25.inf",
		"char\\tmABCD\\CarnivalHatPS.inf",
		"char\\tmABCD\\TmhC-18t.inf",
		"char\\tmABCD\\psxmas.inf",
		"char\\tmABCD\\Tmh-C27.inf",
		"char\\tmABCD\\CartolaPS.inf",
	},
	{
		"char\\tmABCD\\Mfh-B25.inf",
		"char\\tmABCD\\CarnivalHatATA.inf",
		"char\\tmABCD\\MfhB-18t.inf",
		"char\\tmABCD\\ataxmas.inf",
		"char\\tmABCD\\Mfh-B27.inf",
		"char\\tmABCD\\ar0nATA.inf",
	},
	{
		"char\\tmABCD\\Mmh-A25.inf",
		"char\\tmABCD\\CarnivalHatKS.inf",
		"char\\tmABCD\\MmhA-18t.inf",
		"char\\tmABCD\\ksxmas.inf",
		"char\\tmABCD\\Mmh-A27.inf",
		"char\\tmABCD\\CartolaKS.inf",
	},
	{
		"char\\tmABCD\\Mmh-D25.inf",
		"char\\tmABCD\\CarnivalHatMG.inf",
		"char\\tmABCD\\MmhD-18t.inf",
		"char\\tmABCD\\mgxmas.inf",
		"char\\tmABCD\\Mmh-D27.inf",
		"char\\tmABCD\\CartolaMG.inf",
	},
	{
		"char\\tmABCD\\Mfh-C25.inf",
		"char\\tmABCD\\CarnivalHatPRS.inf",
		"char\\tmABCD\\MfhC-18t.inf",
		"char\\tmABCD\\prsxmas.inf",
		"char\\tmABCD\\Mfh-C27.inf",
		"char\\tmABCD\\ar0nPRS.inf",
	},

};

bool cHaPremiumItem::ChangeBigHead(DWORD Kind, DWORD Time)
{
	HatKind = Kind;
	m_Hat = Time;

	if (Kind > 0 && Kind <= 6)
	{
		SetUpKeepItem(nsPremiumItem::HAT + Kind - 1, Time, true, UpKeepItemName[16 + Kind]);
		ChangeModelSkin(lpCurPlayer, 0, 0, 0, szFaceNames[lpCurPlayer->smCharInfo.JOB_CODE - 1][Kind - 1]);
		ReformCharForm();
		cInvenTory.SetItemToChar();
		return true;
	}
	else
	{
		ChangeModelSkin(lpCurPlayer, 0, lpCurPlayer->smCharInfo.ChangeJob, 0, GetFaceHairSkin(lpCurPlayer));
		ReformCharForm();
		cInvenTory.SetItemToChar();

		return true;
	}

	return false;
}

void cHaPremiumItem::SetItemPremiumTime(int iType, int i)
{
	for (int j = 0; j < SINUPKEEPITEM_MAX; j++)
	{
		if (UpKeepItem[j].Visible == true)
		{
			if (UpKeepItem[j].TGAImageNumber == iType)
			{
				UpKeepItem[j].IconTime[0] = (UpKeepItem[j].IconTime[1]) - (i);
				break;
			}
		}
	}
}
