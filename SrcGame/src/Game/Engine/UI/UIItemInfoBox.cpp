#include "stdafx.h"
#include "UIItemInfoBox.h"
#include <Utils/strings.h>
#include "UIImageLoader.h"
#include <Engine/Directx/DXGraphicEngine.h>
#include <playmain.h>
#include "sinbaram\\sinlinkheader.h"
#include <sinbaram/sinItem.h>

extern int LoadItemImage(sITEMINFO* sItemInfo, sITEM* TempItem);
extern BOOL bIsPerfect2;
extern std::string GetItemMixNameByCode(int code);
extern DWORD GetPlayTime_T();

DWORD dwActionItemTwoHand;

namespace UI
{
	ItemInfoBox::ItemInfoBox()
	{
		iSocketGem[0] = iSocketGem[1] = -1;
	}

	ItemInfoBox::~ItemInfoBox()
	{
	}

	void ItemInfoBox::AddString(int iSide, const std::string& strString, DWORD dwColor, BOOL bBold, BOOL bItalic)
	{
		std::vector<std::string> vString;

		//Find breakline in String
		BOOL bMultiLine = FALSE;
		if (strString.find('\n') != std::string::npos)
		{
			vString = split(strString, '\n');
			bMultiLine = TRUE;
		}

		switch (iSide)
		{
		case 0: //Left Side
			//Is Multiline?
			if (bMultiLine)
			{
				//String from vector
				int i = 0;
				for (const auto& strSplitted : vString)
				{
					vLeftText.push_back(std::make_shared<UI::Text>(strSplitted, Rectangle2D(0, 0, 180, 20), bBold, dwColor));
					i++;

					if (i != vString.size())
					{
						vRightText.push_back(std::make_shared<UI::Text>("", Rectangle2D(0, 0, 180, 20), bBold, dwColor));
					}
				}
			}
			else
			{
				vLeftText.push_back(std::make_shared<UI::Text>(strString, Rectangle2D(0, 0, 180, 20), bBold, dwColor));
			}

			break;

		case 1: //Right Side
			vRightText.push_back(std::make_shared<UI::Text>(strString, Rectangle2D(0, 0, 180, 20), bBold, dwColor));
			break;
		}
	}

	void ItemInfoBox::AddCompareString(const std::string& strString, DWORD dwColor)
	{
		vRowCompare.push_back(vLeftText.size());
		vCompareText.push_back(std::make_shared<UI::Text>(strString, Rectangle2D(0, 0, 180, 20), FALSE, dwColor));
	}

	void ItemInfoBox::SetLineColor(DWORD dwColor, int iSide, int iLineCount)
	{
		int iSetColor = 0;
		if (iLineCount < 0)
			iSetColor = vLeftText.size() - 1;
		else
			iSetColor = iLineCount;

		switch (iSide)
		{
		case -1:
			vLeftText[iSetColor]->SetColor(dwColor);
			vRightText[iSetColor]->SetColor(dwColor);
			break;
		case 0:
			vLeftText[iSetColor]->SetColor(dwColor);
			break;
		case 1:
			vRightText[iSetColor]->SetColor(dwColor);
			break;
		}
	}

	void ItemInfoBox::AppendLineText(std::string str, int iSide, int iLineCount)
	{
		int iSetText = 0;
		if (iLineCount < 0)
			iSetText = vLeftText.size() - 1;
		else
			iSetText = iLineCount;

		switch (iSide)
		{
		case -1:
			vLeftText[iSetText]->AppendText(str);
			vRightText[iSetText]->AppendText(str);
			break;
		case 0:
			vLeftText[iSetText]->AppendText(str);
			break;
		case 1:
			vRightText[iSetText]->AppendText(str);
			break;
		}
	}

	void ItemInfoBox::Init()
	{
		//Load Gradient
		pImageGradient = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\gradient.png");
		pImagePKey = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\p_key.png");

		//Load Images
		pImageMaskLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\item\\topleft.png");
		pImageMaskRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\item\\topright.png");
		pImageMaskLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\item\\bottomleft.png");
		pImageMaskRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\item\\bottomright.png");

		//Load Border Images
		pImageBorderLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\topleft.png");
		pImageBorderRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\topright.png");
		pImageBorderTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\top.png");
		pImageBorderRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\bottomright.png");
		pImageBorderLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\bottomleft.png");
		pImageBorderBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\bottom.png");
		pImageBorderLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\left.png");
		pImageBorderRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\right.png");

		//Load Border None Images
		pImageBorderNoneLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\topleft.png");
		pImageBorderNoneRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\topright.png");
		pImageBorderNoneTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\top.png");
		pImageBorderNoneRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\bottomright.png");
		pImageBorderNoneLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\bottomleft.png");
		pImageBorderNoneBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\bottom.png");
		pImageBorderNoneLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\left.png");
		pImageBorderNoneRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\right.png");

		pImageBorderNoneDivider = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\divider.png");
		pImageBorderNoneInfo = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\info.png");
		pImageBorderNoneInfoLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\infoleft.png");
		pImageBorderNoneInfoRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\infoRight.png");

		pImageBorderNoneNameFrameLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\nameframeleft.png");
		pImageBorderNoneNameFrameCenter = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\nameframecenter.png");
		pImageBorderNoneNameFrameRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\nameframeright.png");
		pImageBorderNoneNameFrameBackground = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\None\\nameframebackgroud.png");

		//Load Border Common Images
		pImageBorderCommonLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\topleft.png");
		pImageBorderCommonRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\topright.png");
		pImageBorderCommonTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\top.png");
		pImageBorderCommonRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\bottomright.png");
		pImageBorderCommonLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\bottomleft.png");
		pImageBorderCommonBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\bottom.png");
		pImageBorderCommonLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\left.png");
		pImageBorderCommonRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\right.png");

		pImageBorderCommonDivider = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\divider.png");
		pImageBorderCommonInfo = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\info.png");
		pImageBorderCommonInfoLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\infoleft.png");
		pImageBorderCommonInfoRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\infoRight.png");

		pImageBorderCommonNameFrameLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\nameframeleft.png");
		pImageBorderCommonNameFrameCenter = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\nameframecenter.png");
		pImageBorderCommonNameFrameRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\nameframeright.png");
		pImageBorderCommonNameFrameBackground = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Common\\nameframebackgroud.png");

		//Load Border Uncommon Images
		pImageBorderUncommonLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\topleft.png");
		pImageBorderUncommonRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\topright.png");
		pImageBorderUncommonTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\top.png");
		pImageBorderUncommonRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\bottomright.png");
		pImageBorderUncommonLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\bottomleft.png");
		pImageBorderUncommonBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\bottom.png");
		pImageBorderUncommonLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\left.png");
		pImageBorderUncommonRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\right.png");

		pImageBorderUncommonDivider = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\divider.png");
		pImageBorderUncommonCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\corner.png");
		pImageBorderUncommonBottomCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\bottomcorner.png");
		pImageBorderUncommonInfo = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\info.png");
		pImageBorderUncommonInfoLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\infoleft.png");
		pImageBorderUncommonInfoRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\infoRight.png");

		pImageBorderUncommonNameFrameLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\nameframeleft.png");
		pImageBorderUncommonNameFrameCenter = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\nameframecenter.png");
		pImageBorderUncommonNameFrameRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\nameframeright.png");
		pImageBorderUncommonNameFrameBackground = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Uncommon\\nameframebackgroud.png");

		//Load Border Rare Images
		pImageBorderRareLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\topleft.png");
		pImageBorderRareRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\topright.png");
		pImageBorderRareTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\top.png");
		pImageBorderRareRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\bottomright.png");
		pImageBorderRareLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\bottomleft.png");
		pImageBorderRareBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\bottom.png");
		pImageBorderRareLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\left.png");
		pImageBorderRareRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\right.png");

		pImageBorderRareDivider = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\divider.png");
		pImageBorderRareCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\corner.png");
		pImageBorderRareBottomCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\bottomcorner.png");
		pImageBorderRareInfo = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\info.png");
		pImageBorderRareInfoLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\infoleft.png");
		pImageBorderRareInfoRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\rare\\infoRight.png");

		pImageBorderRareNameFrameLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Rare\\nameframeleft.png");
		pImageBorderRareNameFrameCenter = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Rare\\nameframecenter.png");
		pImageBorderRareNameFrameRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Rare\\nameframeright.png");
		pImageBorderRareNameFrameBackground = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Rare\\nameframebackgroud.png");

		//Load Border Epic Images
		pImageBorderEpicLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\topleft.png");
		pImageBorderEpicRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\topright.png");
		pImageBorderEpicTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\top.png");
		pImageBorderEpicRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\bottomright.png");
		pImageBorderEpicLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\bottomleft.png");
		pImageBorderEpicBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\bottom.png");
		pImageBorderEpicLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\left.png");
		pImageBorderEpicRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\right.png");

		pImageBorderEpicDivider = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\divider.png");
		pImageBorderEpicCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\corner.png");
		pImageBorderEpicBottomCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\bottomcorner.png");
		pImageBorderEpicInfo = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\info.png");
		pImageBorderEpicInfoLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\infoleft.png");
		pImageBorderEpicInfoRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\infoRight.png");

		pImageBorderEpicNameFrameLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\nameframeleft.png");
		pImageBorderEpicNameFrameCenter = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\nameframecenter.png");
		pImageBorderEpicNameFrameRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\nameframeright.png");
		pImageBorderEpicNameFrameBackground = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Epic\\nameframebackgroud.png");

		//Load Border Legendary Images
		pImageBorderLegendaryLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\topleft.png");
		pImageBorderLegendaryRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\topright.png");
		pImageBorderLegendaryTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\top.png");
		pImageBorderLegendaryRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\bottomright.png");
		pImageBorderLegendaryLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\bottomleft.png");
		pImageBorderLegendaryBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\bottom.png");
		pImageBorderLegendaryLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\left.png");
		pImageBorderLegendaryRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\right.png");

		pImageBorderLegendaryDivider = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\divider.png");
		pImageBorderLegendaryCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\corner.png");
		pImageBorderLegendaryBottomCorner = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\bottomcorner.png");
		pImageBorderLegendaryInfo = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\info.png");
		pImageBorderLegendaryInfoLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\infoleft.png");
		pImageBorderLegendaryInfoRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\infoRight.png");

		pImageBorderLegendaryNameFrameLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\nameframeleft.png");
		pImageBorderLegendaryNameFrameCenter = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\nameframecenter.png");
		pImageBorderLegendaryNameFrameRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\nameframeright.png");
		pImageBorderLegendaryNameFrameBackground = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\Legendary\\nameframebackgroud.png");

		//Load Icons
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo101.bmp")); //Lucidy
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo102.bmp")); //Sereneo
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo103.bmp")); //Fadeo
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo104.bmp")); //Sparky
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo105.bmp")); //Raident
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo106.bmp")); //Transparo
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo107.bmp")); //Murky
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo108.bmp")); //Devine
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo109.bmp")); //Celesto
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo110.bmp")); //Mirage
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo111.bmp")); //Inferna
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo112.bmp")); //Enigma
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo113.bmp")); //Bellum
		vSheltomImages.push_back(ImageLoader::LoadImage_("Image\\SinImage\\Items\\Accessory\\itfo114.bmp")); //Ordo


		paSheltomCountText[0] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSheltomCountText[1] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSheltomCountText[2] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSheltomCountText[3] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSheltomCountText[4] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSheltomCountText[5] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSheltomCountText[6] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));


		paSheltomCountText[0]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
		paSheltomCountText[1]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
		paSheltomCountText[2]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
		paSheltomCountText[3]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
		paSheltomCountText[4]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
		paSheltomCountText[5]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));
		paSheltomCountText[6]->SetFont("Arial", 10, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));

		//Load Aging Bar
		pImageAgingBar = ImageLoader::LoadImage_("image\\Sinimage\\shopall\\Aging\\Bar.bmp");
		pImageAgingBarGage = ImageLoader::LoadImage_("image\\Sinimage\\shopall\\Aging\\Bar_.bmp");
		pImageAgingBarNum = ImageLoader::LoadImage_("image\\Sinimage\\shopall\\Aging\\Aging.bmp");

		pTextAging = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		pTextAging->SetFont("Arial", 14, 0, FALSE, FALSE, D3DCOLOR_ARGB(255, 215, 215, 215));

		//cBackgroundColor2 = UI::Color( 235, 45, 39, 27 );
		//cBackgroundColor2 = UI::Color( 245, 9, 23, 22 );

		paSocketText[0][0] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSocketText[0][0]->SetFont("Arial", 16, 0, FALSE, TRUE, -1);

		paSocketText[0][1] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSocketText[0][1]->SetFont("Arial", 16, 0, FALSE, FALSE, -1);

		paSocketText[1][0] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSocketText[1][0]->SetFont("Arial", 16, 0, FALSE, TRUE, -1);

		paSocketText[1][1] = std::make_shared<Text>(Rectangle2D(0, 0, 0, 0));
		paSocketText[1][1]->SetFont("Arial", 16, 0, FALSE, FALSE, -1);


		pRenderTargetMask = GRAPHICENGINE->GetGraphicsDevice()->MakeRenderTarget(5001, 600, 600);
		pEffectMask = GRAPHICENGINE->GetGraphicsDevice()->GetEffectFactory()->MakeEffect("game\\scripts\\shaders\\InfoBox.fx");

		//Socket Images
		pImageSocketBackground = ImageLoader::LoadImage_("image\\Sinimage\\Inven\\InfoBox\\socketbg.png");

		for (int i = 0; i < 6; i++)
			paSocket[i] = ImageLoader::LoadImage_("image\\Sinimage\\Inven\\InfoBox\\socket%d.png", i + 1);

		for (int i = 0; i < 15; i++)
			paSocketGems[i] = ImageLoader::LoadImage_("image\\Sinimage\\Items\\Event\\itPR5%02d.bmp", i + 1);


		// Quantidade de sheltons utilizados no aging
		vAgeSheltoms[1] = { 0, 0, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[2] = { 0, 0, 4, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[3] = { 0, 0, 6, 6, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[4] = { 0, 0, 8, 8, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[5] = { 0, 0, 10, 10, 9, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[6] = { 0, 0, 12, 12, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[7] = { 0, 0, 14, 14, 13, 9, 5, 1, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[8] = { 0, 0, 16, 16, 15, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[9] = { 0, 0, 16, 18, 17, 13, 9, 5, 1, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[10] = { 0, 0, 16, 20, 19, 15, 11, 7, 3, 0, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[11] = { 0, 0, 0, 20, 21, 17, 13, 9, 5, 1, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[12] = { 0, 0, 0, 20, 23, 19, 15, 11, 7, 3, 0, 0, 0, 0, 0, 0 };
		vAgeSheltoms[13] = { 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0, 0, 0, 0 };
		vAgeSheltoms[14] = { 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0, 0, 0, 0 };
		vAgeSheltoms[15] = { 0, 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0, 0, 0 };
		vAgeSheltoms[16] = { 0, 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0, 0, 0 };
		vAgeSheltoms[17] = { 0, 0, 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0, 0 };
		vAgeSheltoms[18] = { 0, 0, 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0, 0 };
		vAgeSheltoms[19] = { 0, 0, 0, 0, 0, 0, 0, 23, 21, 17, 13, 9, 5, 1, 0, 0 };
		vAgeSheltoms[20] = { 0, 0, 0, 0, 0, 0, 0, 23, 23, 19, 15, 11, 7, 3, 0, 0 };

		// Quantidade de sheltons utilizados no mix
		vMixSheltoms[1] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[2] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[3] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[4] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[5] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[6] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[7] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[8] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[9] = { 0, 0, 0, 0, 0, 2, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[10] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[11] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[12] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[13] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[14] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[15] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[16] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[17] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
		vMixSheltoms[18] = { 0, 0, 0, 0, 0, 0, 2, 3, 5, 0, 0, 0, 0, 0, 0, 0 };
	}

	void ItemInfoBox::ClearCompareItem()
	{
		//DEBUG("ItemInfoBox::ClearCompareItem()");
		ZeroMemory(&cItemCompare, sizeof(sITEM));
		bCompareItem = FALSE;
	}

	void ItemInfoBox::Invalidate()
	{
		ZeroMemory(&cItemWrapper, sizeof(sITEM));
		bCompareItem = FALSE;
	}

	void ItemInfoBox::SetCompareItem(const sITEM* pcCompareItemData, BOOL bEquippedItem)
	{
		//DEBUG("ItemInfoBox::SetCompareItem(%d)", bEquippedItem);

		CopyMemory(&cItemCompare, pcCompareItemData, sizeof(sITEM));

		bCompareItem = TRUE;
		bIsEquippedItem = bEquippedItem;
	}

	BOOL ItemInfoBox::IsBeginnerSet(sITEM* pcItemData)
	{
		BOOL bRet = FALSE;
		return bRet;
	}

	BOOL ItemInfoBox::IsSameItem(sITEM* pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice)
	{
		if (cItemWrapper.sItemInfo.ItemHeader.Head == pcItemData->sItemInfo.ItemHeader.Head &&
			cItemWrapper.sItemInfo.ItemHeader.dwChkSum == pcItemData->sItemInfo.ItemHeader.dwChkSum &&
			cItemWrapper.ItemPosition == pcItemData->ItemPosition &&
			cItemWrapper.CODE == pcItemData->CODE &&
			bShowingBuyPrice == bShowBuyPrice &&
			bShowingSellPrice == bShowSellPrice)
		{
			return TRUE;
		}

		return FALSE;
	}
	// pItem->sItemInfo.CODE & sinITEM_MASK2) == sinDA1

	char* ItemInfoBox::GetItemTypeName(DWORD eItemTYpe) // sinOA1
	{
		if ((eItemTYpe & sinITEM_MASK2) == sinOA1)
			return "Amuleto";
		else if ((eItemTYpe & sinITEM_MASK2) == sinDA1)
			return "Armadura";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWA1)
			return "Machado";
		else if ((eItemTYpe & sinITEM_MASK2) == sinDB1)
			return "Botas";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWS1)
			return "Arco";
		else if ((eItemTYpe & sinITEM_MASK2) == sinOA2)
			return "Bracelete";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWC1)
			return "Garra";
		else if ((eItemTYpe & sinITEM_MASK2) == sinDG1)
			return "Luvas";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWH1)
			return "Martelo";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWT1)
			return "Lança";
		else if ((eItemTYpe & sinITEM_MASK2) == sinOR1 || (eItemTYpe & sinITEM_MASK2) == sinOR2)
			return "Anel";
		else if ((eItemTYpe & sinITEM_MASK2) == sinDA2)
			return "Roupão";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWP1)
			return "Foice";
		else if ((eItemTYpe & sinITEM_MASK2) == sinDS1)
			return "Escudo";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWS2)
			return "Espada";
		else if ((eItemTYpe & sinITEM_MASK2) == sinWM1)
			return "Varinha";
		else if ((eItemTYpe & sinITEM_MASK2) == sinOM1)
			return "Orbital";
		else if ((eItemTYpe & sinITEM_MASK2) == sinOS1)
			return "Shelton";
		else
			return "";
	}

	char* ItemInfoBox::GetHanded(DWORD eItemSlot)
	{
		if ((eItemSlot && dwActionItemTwoHand))
			return "Two-Handed";
		else
			return "One-Handed";
	}

	BOOL ItemInfoBox::FormatItemInfo(sITEM* pcItemData)
	{
		iSheltomYPosExtra = 5;

		if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_QUEST || pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON)
		{
			AddString(0, "Item de Quest", D3DCOLOR_RGBA(0, 191, 255, 255));
			AddString(1, "");
		}

		if (bIsPerfect2)
		{
			AddString(0, "Item Perfeito", D3DCOLOR_RGBA(0, 191, 255, 255));
			AddString(1, "");

			iSheltomYPosExtra += 14;
		}

		if (bIsEquippedItem && bCompareItem)
		{
			AddString(0, "Item Equipado", D3DCOLOR_RGBA(70, 125, 80, 255));
			AddString(1, "");

			iSheltomYPosExtra += 14;
		}

		char* sItemType = GetItemTypeName(pcItemData->sItemInfo.CODE);
		char* sHanded = pcItemData->WeaponClass == ITEM_CLASS_WEAPON_TWO ? "Two-Handed" : "One-Handed";

		if (pcItemData->sItemInfo.itemType == 1)
		{
			AddString(0, "Item de Boss", D3DCOLOR_RGBA(0, 128, 128, 255));
			AddString(1, "");

			iSheltomYPosExtra += 14;
		}
		else if (pcItemData->sItemInfo.itemType == 99)
		{
			AddString(0, "Item Temporário", D3DCOLOR_RGBA(0, 128, 128, 255));
			AddString(1, "");

			iSheltomYPosExtra += 14;
		}

		BOOL bHandedText = FALSE;

		if (sItemType == "Machado" ||
			sItemType == "Martelo" ||
			sItemType == "Varinha" ||
			sItemType == "Foice" ||
			sItemType == "Arco" ||
			sItemType == "Espada")
		{
			bHandedText = TRUE;
		}

		//if (pcItemData->sItem.eRarity != EItemRarity::NONE)
		//{
		//	AddString(0, FormatString("%s    ", sItemRarity), D3DCOLOR_ARGB(255, 170, 160, 170));
		//	AddString(1, "");

		//	iSheltomYPosExtra += 14;
		//}

		if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT && pcItemData->sItemInfo.iMixType > 0)
		{
			std::string mixIdName = GetItemMixNameByCode(pcItemData->sItemInfo.iMixType);

			const char* sDesc;

			if (bHandedText)
				sDesc = FormatString("%s %s %s    ", mixIdName.c_str(), sHanded, sItemType);
			else
				sDesc = FormatString("%s %s    ", mixIdName.c_str(), sItemType);

			if (strlen(sDesc) > 20)
			{
				AddString(0, FormatString("%s    ", mixIdName.c_str()), D3DCOLOR_ARGB(255, 170, 160, 170));
				AddString(1, "");

				if (bHandedText)
					sDesc = FormatString("%s %s    ", sHanded, sItemType);
				else
					sDesc = FormatString("%s    ", sItemType);

				iSheltomYPosExtra += 14;
			}

			AddString(0, sDesc, D3DCOLOR_ARGB(255, 170, 160, 170));
			AddString(1, "");

			AddString(0, "");
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
		}
		else if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_AGING)
		{
			const char* sDesc;

			if (bHandedText)
				sDesc = FormatString("%s com Aging", sHanded, sItemType);
			else
				sDesc = FormatString("%s com Aging", sItemType);

			if (strlen(sDesc) > 20)
			{
				if (bHandedText)
				{
					AddString(0, FormatString("Item com Aging    "), D3DCOLOR_ARGB(255, 170, 160, 170));
					AddString(1, "");

					sDesc = FormatString("%s %s    ", sHanded, sItemType);
				}
				else
				{
					AddString(0, FormatString("Item com Aging    "), D3DCOLOR_ARGB(255, 170, 160, 170));
					AddString(1, "");

					sDesc = FormatString("%s    ", sItemType);
				}

				iSheltomYPosExtra += 14;
			}

			AddString(0, sDesc, D3DCOLOR_ARGB(255, 170, 160, 170));
			AddString(1, "");

			AddString(0, "");
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");

			iSheltomYPosExtra += 14;
			iSheltomYPosExtra += 14;
		}
		else
		{
			const char* sDesc;

			if (bHandedText)
				sDesc = FormatString("%s %s    ", sHanded, sItemType);
			else
				sDesc = FormatString("%s    ", sItemType);


			AddString(0, sDesc, D3DCOLOR_ARGB(255, 170, 160, 170));
			AddString(1, "");
		}

		bDisplayHelp = FALSE;

		if (sItemType[0])
		{
			bDisplayHelp = TRUE;

			AddString(0, " ");
			AddString(1, " ");
		}

		DWORD eItemID = pcItemData->sItemInfo.CODE;

		switch (eItemID)
		{
		case (sinEC1 | sin01):
			AddString(0, "Voltar para Ricarten", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinEC1 | sin02):
			AddString(0, "Voltar para Navisko", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinEC1 | sin03):
			AddString(0, "Voltar para Pillai", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinEC1 | sin06):
			AddString(0, "Voltar para Atlantis", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinGP1 | sin10):
			AddString(0, "Somente para membros de Clan", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinGP1 | sin22):
			AddString(0, "Somente para membros de Clan", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinEC1 | sin04):
			AddString(0, "Transportar-se ao membro do Grupo", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");


			if (pcItemData->sItemInfo.ItemName[lstrlenA(pcItemData->sItemInfo.ItemName) + 1] != 0)
			{
				AddString(0, FormatString("Assinado por: %s", pcItemData->sItemInfo.ItemName + lstrlenA(pcItemData->sItemInfo.ItemName) + 1), D3DCOLOR_ARGB(255, 110, 165, 250));
				AddString(1, "");
			}
			break;

		case (sinEC1 | sin05):
			AddString(0, "Transportar-se ao membro do Clan", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");

			if (pcItemData->sItemInfo.ItemName[lstrlenA(pcItemData->sItemInfo.ItemName) + 1] != 0)
			{
				AddString(0, FormatString("Assinado por: %s", pcItemData->sItemInfo.ItemName + lstrlenA(pcItemData->sItemInfo.ItemName) + 1), D3DCOLOR_ARGB(255, 110, 165, 250));
				AddString(1, "");
			}
			break;
		case (sinSP1 | sin05):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém 30 Manas Místicas", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinSP1 | sin06):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item aleatório da caixa", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinSP1 | sin07):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item aleatório da caixa", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinSP1 | sin08):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item aleatório da caixa", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinSP1 | sin09):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item aleatório da caixa", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinSP1 | sin10):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item aleatório da caixa", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin08):
			AddString(0, "Usado para teletransporta-se para outros locais", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin10):
			AddString(0, "Diminui a chance de Falha do Aging em 20%!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin85):
			AddString(0, "Aging Perfeito", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinSE1 | sin05):
			AddString(0, "Muda o item para a especialização Lutador.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin06):
			AddString(0, "Muda o item para a especialização Mecânico.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin07):
			AddString(0, "Muda o item para a especialização Pikeman.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin08):
			AddString(0, "Muda o item para a especialização Arqueira.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin09):
			AddString(0, "Muda o item para a especialização Cavaleiro.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin10):
			AddString(0, "Muda o item para a especialização Atalanta.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin11):
			AddString(0, "Muda o item para a especialização Mago.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinSE1 | sin12):
			AddString(0, "Muda o item para a especialização Sacerdotisa.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Os status obtidos são aleatórios.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinGP1 | sin09):
			AddString(0, "Invoca um monstro aleatório.\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Monstros possíveis:", D3DCOLOR_RGBA(255, 132, 50, 254));
			AddString(1, "");
			AddString(0, "Hopi\nDuende Macabro\nDecoy\nBargon\nDecapitador\nFigon\nRei Hopi\nHulk", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");		
			break;
		case (sinOS1 | sin01):
		case (sinOS1 | sin02):
		case (sinOS1 | sin03):
		case (sinOS1 | sin04):
		case (sinOS1 | sin05):
		case (sinOS1 | sin06):
		case (sinOS1 | sin07):
		case (sinOS1 | sin08):
		case (sinOS1 | sin09):
		case (sinOS1 | sin10):
		case (sinOS1 | sin11):
		case (sinOS1 | sin12):
		case (sinOS1 | sin13):
		case (sinOS1 | sin14):
		case (sinOS1 | sin15):
		case (sinOS1 | sin16):
		case (sinOS1 | sin17):
		case (sinOS1 | sin18):
		case (sinOS1 | sin19):
			//Texto usado na descrição de um item - DRACKO
			
			//AddString(0, "Utilizado para criar uma Forceob.\nUtilizado no Mix ou no Aging.", D3DCOLOR_RGBA(255, 132, 50, 254));
			//AddString(1, "");
			//AddString(0, "Fale com o Mestre das Forças,\nMorald ou o Moriph.", D3DCOLOR_ARGB(255, 180, 100, 175));
			//AddString(1, "");
			//AddString(0, "\n");
			//AddString(1, "");
			break;
		case (sinBI1 | sin01):
			AddString(0, "Redefine os Pontos de Status.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin02):
			AddString(0, "Redefine os Pontos de Habilidade.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin03):
			AddString(0, "Redefine os Pontos de Status e Habilidade.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin04):
			AddString(0, "Ressucita 5 segundos após a morte\ne evita perca de experiência.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin05):
			AddString(0, "Invencibilidade Temporária\nDano reduzido pela metade.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 30 Segundos.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI1 | sin06):
			AddString(0, "+ 5% de Crítico.\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 30 Minutos.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI1 | sin07):
			AddString(0, "+ 5% de Esquiva.\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 30 Minutos.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI1 | sin36):
			AddString(0, "Matura Itens de Ataque", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin37):
			AddString(0, "Matura Escudos ou Orbitais", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin38):
			AddString(0, "Matura Armaduras ou Robes", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin39):
			AddString(0, "Treina as habilidades da Tier 1", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin40):
			AddString(0, "Treina as habilidades da Tier 2", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin41):
			AddString(0, "Treina as habilidades da Tier 3", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin45):
			AddString(0, "Redefine os Status de Força.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin46):
			AddString(0, "Redefine os Status de Inteligência.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin47):
			AddString(0, "Redefine os Status de Talento.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin48):
			AddString(0, "Redefine os Status de Agilidade.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin49):
			AddString(0, "Redefine os Status de Vitalidade.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI1 | sin16):
		case (sinBI1 | sin17):
		case (sinBI1 | sin18):
		case (sinBI2 | sin13):
		case (sinBI2 | sin14):
		case (sinBI2 | sin15):
		case (sinBI2 | sin16):
		case (sinBI2 | sin17):
		case (sinBI2 | sin18):
		case (sinBI2 | sin19):
		case (sinBI2 | sin20):
		case (sinBI2 | sin21):
		case (sinBI2 | sin22):
		case (sinBI2 | sin39):
		case (sinBI2 | sin40):
		case (sinBI2 | sin41):
		case (sinBI2 | sin42):
		case (sinBI2 | sin43):
			AddString(0, "Muda o estilo do cabelo.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI2 | sin47):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item nível 80 +16 da sua classe!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Válido por 3 Horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin48):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item nível 100 +16 da sua classe!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Válido por 3 Horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin49):
			AddString(0, "Clique com o botão esquerdo para utilizar\nObtém um item nível 60 +16 da sua classe!", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Válido por 3 Horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin50):
			AddString(0, "Clique com o botão esquerdo para utilizar\nChega ao próximo nível instantaneamente.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Nível Requerido: 110-119", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin51):
			AddString(0, "Clique com o botão esquerdo para utilizar\nChega ao próximo nível instantaneamente.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Nível Requerido: 120 - 129", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin86):
			AddString(0, "Clique com o botão esquerdo para utilizar\nChega ao próximo nível instantaneamente.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Nível Requerido: 130-134", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin87):
			AddString(0, "Clique com o botão esquerdo para utilizar\nChega ao próximo nível instantaneamente.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			//AddString(0, "Nível Requerido: 125-129", D3DCOLOR_ARGB(255, 180, 100, 175));
			//AddString(1, "");
			break;
		case (sinBI2 | sin57):
		case (sinBI2 | sin58):
		case (sinBI2 | sin59):
		case (sinBI2 | sin60):
		case (sinBI2 | sin61):
		case (sinBI2 | sin62):
			AddString(0, "A Caravana guarda seus itens\ne coleta o ouro próximo (distância de 15m).", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBI2 | sin63):
			AddString(0, "+ 50 de vida por 24 Horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin64):
			AddString(0, "+ 5% de Crítico por 24 Horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin65):
			AddString(0, "+ 5 de Velocidade por 24 Horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin66):
			AddString(0, "+ 4 de Regeneração de SP por 24 horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin67):
			AddString(0, "+ 4 de Regeneração de MP por 24 horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinBI2 | sin68):
			AddString(0, "+ 4 de Regeneração de HP por 24 horas.", D3DCOLOR_ARGB(255, 180, 100, 175));
			AddString(1, "");
			break;
		case (sinCH1 | sin01):
			AddString(0, "Obtém 12% De Vida Extra.\nSomente Personagens Masculinos", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d Minutos", 5), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinCH1 | sin02):
			AddString(0, "Obtém 24% De Vida Extra.\nSomente Personagens Masculinos", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d Minutos", 5), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinCH1 | sin03):
			AddString(0, "Obtém 12% De Vida Extra.\nSomente Personagens Femininos", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d Minutos", 5), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinBC1 | sin01):
			AddString(0, "Invencibilidade Temporária\nDano reduzido pela metade.", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 30 segundos\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin02):
			AddString(0, "+ 5% de Crítico.\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 30 segundos\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin03):
			AddString(0, "+ 5% de Evasão.\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 30 segundos\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin04):
			AddString(0, "Restaure HP, RES e Mana.\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Somente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin05):
			AddString(0, "Ressucita 5 segundos após a morte\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Somente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin06):
			AddString(0, "Reduz o dano contra os cristais de fogo em 50%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin07):
			AddString(0, "Reduz o dano contra os cristais de gelo em 50%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin08):
			AddString(0, "Reduz o dano contra os cristais de raio em 50%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin09):
			AddString(0, "Aumenta o poder de ataque contra lutadores em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin10):
			AddString(0, "Aumenta o poder de ataque contra mecânicos em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin11):
			AddString(0, "Aumenta o poder de ataque contra pikes em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin12):
			AddString(0, "Aumenta o poder de ataque contra arqueiras em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin13):
			AddString(0, "Aumenta o poder de ataque contra cavaleiros em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin14):
			AddString(0, "Aumenta o poder de ataque contra atalantas em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin15):
			AddString(0, "Aumenta o poder de ataque contra magos em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinBC1 | sin16):
			AddString(0, "Aumenta o poder de ataque contra sarcerdotisas em 20%\n", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Dura 1 Minuto\nSomente em BC", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			break;
		case (sinCH1 | sin04):
			AddString(0, "Obtém 24% De Vida Extra.\nSomente Personagens Femininos", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, "");
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d Minutos", 5), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin01):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 2), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 0), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 500), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin02):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 4), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 0), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 500), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin03):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 7), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 0), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 500), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin04):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 10), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 0), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 500), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin05):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 15), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 0), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 700), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin06):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 25), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 0), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 900), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin07):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 40), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 10), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 1800), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin08):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 60), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 10), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 2400), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin09):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 80), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 10), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 3600), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin10):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 100), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 10), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 4800), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin11):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 120), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 15), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 6000), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin12):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 140), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 20), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 7200), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin13):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 160), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 23), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 8400), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		case (sinFO1 | sin14):
			AddString(0, "Dano Adicional:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", 180), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Bônus no Dano:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", 26), D3DCOLOR_ARGB(255, 240, 215, 150));
			AddString(0, "Duração:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%ds", 10000), D3DCOLOR_ARGB(255, 240, 215, 150));
			break;
		}

		return TRUE;
	}

	DWORD ItemInfoBox::GetItemPlayTime(sITEMINFO* pcItem)
	{
		return pcItem->dwCreateTime;
	}

	BOOL ItemInfoBox::IsExpiredItemTime(sITEMINFO* psItemInfo)
	{
		struct	tm	tm;
		time_t		ttm;
		DWORD		dwCreateTime;
		DWORD		dwItemCode;

		dwItemCode = psItemInfo->CODE & sinITEM_MASK2;

		DWORD NowTime = GetPlayTime_T();

		if (psItemInfo->expireTime > 0) {
			if (psItemInfo->dwCreateTime + (60 * 60 * psItemInfo->expireTime) < NowTime)
				return true;
		}

		if (psItemInfo->CODE != (sinOR2 | sin01))
			return FALSE;

		tm.tm_year = 2024 - 1900;
		tm.tm_mon = tm.tm_mon + 1;
		tm.tm_mday = tm.tm_mday;
		tm.tm_hour = tm.tm_hour;
		tm.tm_min = tm.tm_min;
		tm.tm_sec = tm.tm_sec;
		ttm = mktime(&tm);

		dwCreateTime = (DWORD)ttm;

		if (psItemInfo->dwCreateTime < dwCreateTime) return TRUE;

		return FALSE;
	}

	BOOL ItemInfoBox::FormatItemTimeInfo(sITEM* pcItemData)
	{
		DWORD dwTime = 0;
		DWORD dwItemTime = pcItemData->sItemInfo.expireTime;
		BOOL bResult = FALSE;

		if (dwItemTime == 0)
			return FALSE;

		DWORD eItemID = pcItemData->sItemInfo.CODE;

		DWORD dwTotalTime = ItemInfoBox::GetItemPlayTime(&pcItemData->sItemInfo);

		if (dwTotalTime)
		{
			if (ItemInfoBox::IsExpiredItemTime(&pcItemData->sItemInfo))
			{
				AddString(0, "");
				AddString(1, "");
				AddString(0, "Item Expirado", D3DCOLOR_ARGB(255, 125, 180, 175));
				AddString(1, "\n ");
			}
			else
			{
				AddString(0, "");
				AddString(1, "");
				dwTime = (60 * 60 * dwItemTime) - (GetPlayTime_T() - dwTotalTime);
				AddString(0, FormatString("Tempo Restante: %02dD:%02dH:%02dM", (dwTime / 60 / 60) / 24, (dwTime / 60 / 60) % 24, (dwTime / 60) % 60), D3DCOLOR_ARGB(255, 180, 100, 175));
				AddString(1, "\n ");
			}
			bResult = TRUE;
		}

		return bResult;
	}

	float RoundOffToNearestDP(float N, int dp)
	{
		double multiplier = pow(10, dp);
		float value = std::round(N * multiplier) / multiplier;
		return value;
	}

	BOOL ItemInfoBox::PrepareShowItem(sITEM* pcItemData, BOOL bShowBuyPrice, BOOL bShowSellPrice, BOOL bShowChecksums, BOOL isItemCompare, int iBaseWidth)
	{
		//Clear Vectors
		vLeftText.clear();
		vRightText.clear();
		vCompareText.clear();
		vRowCompare.clear();

		bShowingSellPrice = bShowSellPrice;
		bShowingBuyPrice = bShowBuyPrice;

		iSocketCount = 0;
		bIsPerfectItem = FALSE;
		bCantUseItem = FALSE;
		bHasLargeName = FALSE;
		//cBackgroundColor1 = UI::Color( 235, 45, 39, 27 );
		//cBackgroundColor2 = UI::Color( 245, 9, 23, 22 );

		isCompare = isItemCompare;

		if (iBaseWidth != -1)
			iBaseMaxWidth = iBaseWidth;

		sItemName = "";
#ifdef ITEMFIXES
		sPrefixName = "";
		bHasPrefix = FALSE;
		bHasSuffix = FALSE;
		sSuffixName = "";
#endif

		{
			if (pcItemData->Class == ITEM_CLASS_WEAPON_TWO &&
				pcItemData->ItemPosition &&
				pcItemData->sItemInfo.ItemHeader.Head == 0 && pcItemData->sItemInfo.ItemHeader.dwChkSum == 0)
			{
				CopyMemory(&pdwItemInfoData, &cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo, sizeof(sITEMINFO));
				CopyMemory(&cItemWrapper, &cInvenTory.InvenItem[sInven[0].ItemIndex - 1], sizeof(sITEM));

				cItemWrapper.ItemPosition = pcItemData->ItemPosition;
			}
			else
			{
				CopyMemory(&pdwItemInfoData, &pcItemData->sItemInfo, sizeof(sITEMINFO));
				CopyMemory(&cItemWrapper, pcItemData, sizeof(sITEM));

				iItemInfoBoxWidth = -1;
			}
		}

		if (IsViewingPerfectItem())
			iItemInfoBoxWidth = -1;

		//Copy psItem to cItemWrapper
		CopyMemory(&cItemWrapper.sItemInfo, &pdwItemInfoData, sizeof(sITEMINFO));

		//if (bIsViewingPerfectItem)
		//{
		//	cItemWrapper.sItem.eRarity = cItemCompare.sItem.eRarity;
		//}

		BOOL bBeginner = IsBeginnerSet(&cItemWrapper);

		bCantUseItem = cItemWrapper.NotUseFlag;

		//Perfect Item
		//if (cItemWrapper.sItem.iItemSpecialType == 1)
			//bIsPerfectItem = TRUE;

		if (bIsPerfect2)
			bIsPerfectItem = TRUE;

		//RGB rarityColor = rarityColors[cItemWrapper.sItem.eRarity];	//background color based on rarity
		RGB rarityColor = rarityColors[0];								//All background colors are the same

		if (bIsPerfectItem)
		{
			cBackgroundColor1 = UI::Color(245, rarityColor.red / 6, rarityColor.green / 6, rarityColor.blue / 6);
			cBackgroundColor2 = UI::Color(230, rarityColor.red / 2, rarityColor.green / 2, rarityColor.blue / 2);
		}
		else
		{
			cBackgroundColor1 = UI::Color(245, rarityColor.red / 3, rarityColor.green / 3, rarityColor.blue / 3);
			cBackgroundColor2 = UI::Color(230, static_cast<int>(round(rarityColor.red / 1.2f)), static_cast<int>(round(rarityColor.green / 1.2f)), static_cast<int>(rarityColor.blue / 1.2f));
		}


		D3DCOLOR FixColor = D3DCOLOR_ARGB(255, 130, 255, 188);
		D3DCOLOR FixColorLighter = D3DCOLOR_ARGB(200, 130, 255, 188);
		D3DCOLOR TitleColor = D3DCOLOR_ARGB(255, 222, 231, 255);

		//Color Name Item
		if (cItemWrapper.sItemInfo.UniqueItem)
		{
			//Unique Color
			TitleColor = D3DCOLOR_ARGB(255, 128, 255, 128);
		}
		else
		{

			if (bBeginner)
				TitleColor = D3DCOLOR_ARGB(255, 0, 222, 52);

			//Set Special Colors
			if (cItemWrapper.sItemInfo.ItemKindCode)
			{
				switch (cItemWrapper.sItemInfo.ItemKindCode)
				{
				case ITEM_KIND_CRAFT:
					TitleColor = D3DCOLOR_ARGB(255, 255, 110, 255);
					break;
				case ITEM_KIND_AGING:
					TitleColor = D3DCOLOR_ARGB(255, 100, 230, 255);
					break;
				case ITEM_KIND_QUEST:
				case ITEM_KIND_QUEST_WEAPON:
					TitleColor = D3DCOLOR_ARGB(255, 132, 50, 254);
					break;
				}
			}
		}

		char* eItemType = GetItemTypeName(cItemWrapper.sItemInfo.CODE);

		if (eItemType[0])
			bHasLargeName = TRUE;

#ifdef ITEMFIXES
		bHasSuffix = TRUE;
		sSuffixName = ""; //cItemWrapper.sItemInfo.ItemName;
		sPrefixName = ""; //cItemWrapper.sItemInfo.ItemName;
#endif

		BOOL bIsPotion = FALSE;

		if ((((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinPM1) || (cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinPS1) || (cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinPL1)
			bIsPotion = TRUE;

		if (bHasLargeName)
		{
			if (cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON)
				sItemName = FormatString("%s\nTier %d", cItemWrapper.sItemInfo.ItemName, cItemWrapper.sItemInfo.ItemAgingCount[0]);
			else
				sItemName = FormatString("%s %s %s", sPrefixName, cItemWrapper.sItemInfo.ItemName, sSuffixName);

			AddString(0, "");
			AddString(1, "");

			AddString(0, "");
			AddString(1, "");

			AddString(0, "");
			AddString(1, "");
		}
		else
		{
			if (cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON)
				AddString(0, FormatString("%s Tier %d", cItemWrapper.sItemInfo.ItemName, cItemWrapper.sItemInfo.ItemAgingCount[0]), -1, TRUE);
			else if (bIsPotion)
				AddString(0, FormatString("%s (%d)", cItemWrapper.sItemInfo.ItemName, cItemWrapper.sItemInfo.PotionCount), -1, TRUE);
			else
				AddString(0, FormatString("%s %s", sPrefixName, cItemWrapper.sItemInfo.ItemName), -1, TRUE);

			AddString(1, "");
			SetLineColor(TitleColor);
		}

		//Make Description Item
		FormatItemInfo(&cItemWrapper);

		//Aging Bar
		if (cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_AGING)
		{
			AddString(0, "\n ");
			AddString(1, "\n ");
		}

		//Quest Item
		//if (cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_QUEST_WEAPON && cItemWrapper.sItemInfo.ItemAgingNum[1] <= MAX_AGING && cItemWrapper.sItemInfo.ItemAgingCount[0])
		//{
		//	//CQUESTGAME->GetQuestInstance()->GetQuest(QUEST)

		//	auto szMonName = QUESTGAMEHANDLER->GetQuestMonsterNameFromID(cItemWrapper.sItem.uQuestMonId);

		//	if (!szMonName.empty())
		//	{
		//		AddString(0, FormatString("%s: %d/%d", szMonName.c_str(), cItemWrapper.sItem.sMatureBar.sMax - cItemWrapper.sItem.sMatureBar.sCur, cItemWrapper.sItem.sMatureBar.sMax), D3DCOLOR_ARGB(255, 125, 180, 175));
		//		AddString(1, "\n ");

		//		AddString(0, "");
		//		AddString(1, "");
		//	}
		//}

		//Respec Stone Level Range
		for (int i = 0; i < 4; i++)
		{
			DWORD eItemID = cItemWrapper.sItemInfo.CODE;

			if (sReconItem[i].dwCODE == eItemID)
			{
				AddString(0, "Level Req.:");
				AddString(1, FormatString("%d-%d", sReconItem[i].iLevel[0], sReconItem[i].iLevel[1]));
				break;
			}
		}

		//Attack Power
		if (cItemWrapper.sItemInfo.Damage[0] || cItemWrapper.sItemInfo.Damage[1])
		{
			AddString(0, "Poder de Ataque:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d-%d", cItemWrapper.sItemInfo.Damage[0], cItemWrapper.sItemInfo.Damage[1]), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_DAMAGE_MIN) || (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_DAMAGE_MAX))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemAgingNum[0]))
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWA1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWC1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWH1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWM1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWP1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWS1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWS2 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWT1)

					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalcMin = cItemWrapper.sItemInfo.Damage[0] - cItemCompare.sItemInfo.Damage[0];
				int iCalcMax = cItemWrapper.sItemInfo.Damage[1] - cItemCompare.sItemInfo.Damage[1];

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalcMin != 0 || iCalcMax != 0)
					AddCompareString(FormatString("%s%d / %s%d",
						iCalcMin > 0 ? "+" : "",
						iCalcMin,
						iCalcMax > 0 ? "+" : "",
						iCalcMax),
						(iCalcMin + iCalcMax) > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Attack fMovementSpeed
		if (cItemWrapper.sItemInfo.Attack_Speed)
		{
			AddString(0, "Velocidade:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Attack_Speed), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_ATTACK_SPEED))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Attack_Speed - cItemCompare.sItemInfo.Attack_Speed;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}

		}

		//Range
		if (cItemWrapper.sItemInfo.Shooting_Range)
		{
			AddString(0, "Alcance:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Shooting_Range), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Shooting_Range - cItemCompare.sItemInfo.Shooting_Range;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Critical
		if (cItemWrapper.sItemInfo.Critical_Hit)
		{
			AddString(0, "Crítico:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", cItemWrapper.sItemInfo.Critical_Hit), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_CRITICAL))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemAgingNum[0]))
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWC1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWP1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWS1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWT1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWS2)

					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Critical_Hit - cItemCompare.sItemInfo.Critical_Hit;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Defense
		if (cItemWrapper.sItemInfo.Defence)
		{
			AddString(0, "Defesa:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Defence), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_DEFENCE))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemAgingNum[0]))
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinOM1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDA1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDA2 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDB1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDG1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinOA2)

					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = 0;

				if (bIsPerfectItem && iDefenseOverride > 0)
				{
					iCalc = iDefenseOverride - cItemCompare.sItemInfo.Defence;
				}
				else
				{
					iCalc = cItemWrapper.sItemInfo.Defence - cItemCompare.sItemInfo.Defence;
				}

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Attack Rating
		if (cItemWrapper.sItemInfo.Attack_Rating)
		{

			AddString(0, "Taxa de Ataque:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Attack_Rating), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_ATTACK_RATE))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemAgingNum[0]))
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWA1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWC1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWM1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWH1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWP1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWS2 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinOA2)

					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Attack_Rating - cItemCompare.sItemInfo.Attack_Rating;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Absorption
		if (cItemWrapper.sItemInfo.fAbsorb)
		{
			AddString(0, "Absorção:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.fAbsorb), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_ABSORB))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemAgingNum[0]))
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinOM1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDA1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDS1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDA2 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDB1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDG1)

					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fAbsorb, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fAbsorb, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Block
		if (cItemWrapper.sItemInfo.fBlock_Rating)
		{
			AddString(0, "Bloqueio:", D3DCOLOR_ARGB(255, 245, 240, 218));

			if (fabs(cItemWrapper.sItemInfo.fBlock_Rating - round(cItemWrapper.sItemInfo.fBlock_Rating)) < FLT_MIN)
				AddString(1, FormatString("%.0f%%", round(cItemWrapper.sItemInfo.fBlock_Rating)), D3DCOLOR_ARGB(255, 240, 215, 150));
			else
				AddString(1, FormatString("%.1f%%", cItemWrapper.sItemInfo.fBlock_Rating), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_BLOCK_RATE))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemAgingNum[0]))
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinDS1)
					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fBlock_Rating, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fBlock_Rating, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f%%", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//fMovementSpeed
		if (cItemWrapper.sItemInfo.fSpeed)
		{
			AddString(0, "Vel. de M.:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.fSpeed), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if ((cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_MOVE_SPEED))
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fSpeed, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fSpeed, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Integrity
		if (cItemWrapper.sItemInfo.Durability[0] || cItemWrapper.sItemInfo.Durability[1])
		{
			AddString(0, "Integridade:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d/%d", cItemWrapper.sItemInfo.Durability[0], cItemWrapper.sItemInfo.Durability[1]), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Aging Color
			if ((cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_AGING))
				SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
		}

		//Mana
		if (cItemWrapper.sItemInfo.Mana[0] || cItemWrapper.sItemInfo.Mana[1])
		{
			AddString(0, "Rec. de MP:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d-%d", cItemWrapper.sItemInfo.Mana[0], cItemWrapper.sItemInfo.Mana[1]), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Mana[0] - cItemCompare.sItemInfo.Mana[0];
				int iCalcMax = cItemWrapper.sItemInfo.Mana[1] - cItemCompare.sItemInfo.Mana[1];

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d-%d", iCalc > 0 ? "+" : "", iCalc, iCalcMax), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//HP
		if (cItemWrapper.sItemInfo.Life[0] || cItemWrapper.sItemInfo.Life[1])
		{
			AddString(0, "Rec. de HP:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d-%d", cItemWrapper.sItemInfo.Life[0], cItemWrapper.sItemInfo.Life[1]), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Life[0] - cItemCompare.sItemInfo.Life[0];
				int iCalcMax = cItemWrapper.sItemInfo.Life[1] - cItemCompare.sItemInfo.Life[1];

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d-%d", iCalc > 0 ? "+" : "", iCalc, iCalcMax), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//STM
		if (cItemWrapper.sItemInfo.Stamina[0] || cItemWrapper.sItemInfo.Stamina[1])
		{
			AddString(0, "Rec. de RES:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d-%d", cItemWrapper.sItemInfo.Stamina[0], cItemWrapper.sItemInfo.Stamina[1]), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Stamina[0] - cItemCompare.sItemInfo.Stamina[0];
				int iCalcMax = cItemWrapper.sItemInfo.Stamina[1] - cItemCompare.sItemInfo.Stamina[1];

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d-%d", iCalc > 0 ? "+" : "", iCalc, iCalcMax), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Resistances
		{
			if (cItemWrapper.sItemInfo.Resistance[sITEMINFO_BIONIC])
			{
				AddString(0, "Orgânico:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Resistance[0]), D3DCOLOR_ARGB(255, 240, 215, 150));

				//Mix Color
				if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_BIO)
					SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.Resistance[sITEMINFO_BIONIC] - cItemCompare.sItemInfo.Resistance[sITEMINFO_BIONIC];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.Resistance[sITEMINFO_FIRE])
			{
				AddString(0, "Fogo:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Resistance[sITEMINFO_FIRE]), D3DCOLOR_ARGB(255, 240, 215, 150));

				//Mix Color
				if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_FIRE)
					SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.Resistance[sITEMINFO_FIRE] - cItemCompare.sItemInfo.Resistance[sITEMINFO_FIRE];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.Resistance[sITEMINFO_ICE])
			{
				AddString(0, "Gelo:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Resistance[3]), D3DCOLOR_ARGB(255, 240, 215, 150));

				//Mix Color
				if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_ICE)
					SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.Resistance[sITEMINFO_ICE] - cItemCompare.sItemInfo.Resistance[sITEMINFO_ICE];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.Resistance[sITEMINFO_LIGHTING])
			{
				AddString(0, "Raio:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Resistance[sITEMINFO_LIGHTING]), D3DCOLOR_ARGB(255, 240, 215, 150));

				//Mix Color
				if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_LIGHTNING)
					SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.Resistance[sITEMINFO_LIGHTING] - cItemCompare.sItemInfo.Resistance[sITEMINFO_LIGHTING];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.Resistance[sITEMINFO_POISON])
			{
				AddString(0, "Veneno:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Resistance[sITEMINFO_POISON]), D3DCOLOR_ARGB(255, 240, 215, 150));

				//Mix Color
				if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_POISON)
					SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.Resistance[sITEMINFO_POISON] - cItemCompare.sItemInfo.Resistance[sITEMINFO_POISON];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}
		}

		//HP Regen
		if (cItemWrapper.sItemInfo.fLife_Regen && (cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) != sinFO1)
		{
			AddString(0, "HP Regen.:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.fLife_Regen), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_LIFEREGEN)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fLife_Regen, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fLife_Regen, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//MP Regen
		if (cItemWrapper.sItemInfo.fMana_Regen && (cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) != sinFO1)
		{
			AddString(0, "MP Regen.:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.fMana_Regen), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_MANAREGEN)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fMana_Regen, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fMana_Regen, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//STM Regen
		if (cItemWrapper.sItemInfo.fStamina_Regen && (cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) != sinFO1)
		{
			AddString(0, "STM Regen.:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.fStamina_Regen), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_STAMINAREGEN)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fStamina_Regen, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fStamina_Regen, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Add HP
		if (cItemWrapper.sItemInfo.fIncrease_Life)
		{
			AddString(0, "Add HP:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.0f", cItemWrapper.sItemInfo.fIncrease_Life), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_LIFE)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fIncrease_Life, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fIncrease_Life, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.0f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Add MP
		if (cItemWrapper.sItemInfo.fIncrease_Mana)
		{
			AddString(0, "Add MP:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.0f", cItemWrapper.sItemInfo.fIncrease_Mana), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_MANA)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Aging Color
			if (cItemWrapper.sItemInfo.ItemAgingNum[0])
			{
				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWM1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWS1 ||
					(cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinWT1) {
					SetLineColor(D3DCOLOR_ARGB(255, 100, 230, 255), 1);
				}
			}

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fIncrease_Mana, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fIncrease_Mana, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.0f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Add STM
		if (cItemWrapper.sItemInfo.fIncrease_Stamina)
		{
			AddString(0, "Add STM:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.0f", cItemWrapper.sItemInfo.fIncrease_Stamina), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Mix Color
			if (cItemWrapper.sItemInfo.ItemKindMask & SIN_ADD_STAMINA)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 110, 255), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.fIncrease_Stamina, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.fIncrease_Stamina, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.0f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Potion Storage
		if (cItemWrapper.sItemInfo.Potion_Space)
		{
			AddString(0, "Poções:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Potion_Space), D3DCOLOR_ARGB(255, 240, 215, 150));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Potion_Space - cItemCompare.sItemInfo.Potion_Space;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		// Time on item
		FormatItemTimeInfo(&cItemWrapper);

		//Define Line before Stats
		iStatLine = 0;
		if (cItemWrapper.sItemInfo.Level ||
			cItemWrapper.sItemInfo.Strength ||
			cItemWrapper.sItemInfo.Spirit ||
			cItemWrapper.sItemInfo.Talent ||
			cItemWrapper.sItemInfo.Dexterity ||
			cItemWrapper.sItemInfo.Spirit)
		{
			iStatLine = vLeftText.size();

			AddString(0, " ");
			AddString(1, " ");
		}

		//Req Level
		if (cItemWrapper.sItemInfo.Level)
		{
			AddString(0, "Req. Level:");

			auto itemLevelStr = FormatString("%d", cItemWrapper.sItemInfo.Level);

			AddString(1, itemLevelStr);

			if (sinChar->Level < cItemWrapper.sItemInfo.Level)
			{
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
				bCantUseItem = TRUE;
			}
			else
				SetLineColor(D3DCOLOR_ARGB(255, 125, 120, 100));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.Level - cItemCompare.sItemInfo.Level;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc < 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Req Strenght
		if (cItemWrapper.sItemInfo.Strength)
		{
			AddString(0, "Req. Força:");
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Strength));

			if (sinChar->Strength < cItemWrapper.sItemInfo.Strength)
			{
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
				bCantUseItem = TRUE;
			}
			else
				SetLineColor(D3DCOLOR_ARGB(255, 125, 120, 100));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItemInfo.JobCodeMask != 0)
			{
				int iCalc = cItemWrapper.sItemInfo.Strength - cItemCompare.sItemInfo.Strength;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc < 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//DEBUG("%s = mix color = %d", cItemWrapper.sItem.szItemName, cItemWrapper.sItem.sMixColorId);

		//Req Spirit
		if (cItemWrapper.sItemInfo.Spirit)
		{
			AddString(0, "Req. Int.:");
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Spirit));

			if (sinChar->Spirit < cItemWrapper.sItemInfo.Spirit)
			{
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
				bCantUseItem = TRUE;
			}
			else
				SetLineColor(D3DCOLOR_ARGB(255, 125, 120, 100));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItemInfo.JobCodeMask != 0)
			{
				int iCalc = cItemWrapper.sItemInfo.Spirit - cItemCompare.sItemInfo.Spirit;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc < 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Req Talent
		if (cItemWrapper.sItemInfo.Talent)
		{
			AddString(0, "Req. Talento:");
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Talent));

			if (sinChar->Talent < cItemWrapper.sItemInfo.Talent)
			{
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
				bCantUseItem = TRUE;
			}
			else
				SetLineColor(D3DCOLOR_ARGB(255, 125, 120, 100));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItemInfo.JobCodeMask != 0)
			{
				int iCalc = cItemWrapper.sItemInfo.Talent - cItemCompare.sItemInfo.Talent;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc < 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Req Agility
		if (cItemWrapper.sItemInfo.Dexterity)
		{
			AddString(0, "Req. Agilidade:");
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Dexterity));

			if (sinChar->Dexterity < cItemWrapper.sItemInfo.Dexterity)
			{
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
				bCantUseItem = TRUE;
			}
			else
				SetLineColor(D3DCOLOR_ARGB(255, 125, 120, 100));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItemInfo.JobCodeMask != 0)
			{
				int iCalc = cItemWrapper.sItemInfo.Dexterity - cItemCompare.sItemInfo.Dexterity;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc < 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Req Health
		if (cItemWrapper.sItemInfo.Health)
		{
			AddString(0, "Req. Vida:");
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.Health));

			if (sinChar->Health < cItemWrapper.sItemInfo.Health)
			{
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
				bCantUseItem = TRUE;
			}
			else
				SetLineColor(D3DCOLOR_ARGB(255, 125, 120, 100));

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem) && cItemCompare.sItemInfo.JobCodeMask != 0)
			{
				int iCalc = cItemWrapper.sItemInfo.Health - cItemCompare.sItemInfo.Health;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc < 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec
		iSpecLine = 0;

		if (cItemWrapper.sItemInfo.JobCodeMask != 0)
		{
			extern char* GetCharacterClassStringByFlagItem(int flag);

			iSpecLine = vLeftText.size();

			AddString(0, FormatString("\n%s +", GetCharacterClassStringByFlagItem(cItem.GetJobCode(cItemWrapper.sItemInfo.JobCodeMask))));
			AddString(1, "\n");

			if (sinChar->JobBitMask == cItemWrapper.sItemInfo.JobCodeMask)
				SetLineColor(D3DCOLOR_ARGB(255, 255, 220, 0));
			else
				SetLineColor(D3DCOLOR_ARGB(255, 255, 0, 0));
		}

		//Spec ATK fMovementSpeed
		if (cItemWrapper.sItemInfo.JobItem.Add_Attack_Speed)
		{
			AddString(0, "Vel. de Ataque:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Attack_Speed));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Attack_Speed - cItemCompare.sItemInfo.JobItem.Add_Attack_Speed;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec Critical
		if (cItemWrapper.sItemInfo.JobItem.Add_Critical_Hit)
		{
			AddString(0, "Taxa de Crítico:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d%%", cItemWrapper.sItemInfo.JobItem.Add_Critical_Hit));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Critical_Hit - cItemCompare.sItemInfo.JobItem.Add_Critical_Hit;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d%%", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec Defense
		if (cItemWrapper.sItemInfo.JobItem.Add_Defence)
		{
			AddString(0, "Defensa:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Defence));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Defence - cItemCompare.sItemInfo.JobItem.Add_Defence;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec fAbsorbRating
		if (cItemWrapper.sItemInfo.JobItem.Add_fAbsorb)
		{
			AddString(0, "Absorção:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.JobItem.Add_fAbsorb));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Add_fAbsorb, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Add_fAbsorb, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec Block
		if (cItemWrapper.sItemInfo.JobItem.Add_fBlock_Rating)
		{
			AddString(0, "Bloqueio:", D3DCOLOR_ARGB(255, 245, 240, 218));

			if (fabs(cItemWrapper.sItemInfo.JobItem.Add_fBlock_Rating - round(cItemWrapper.sItemInfo.JobItem.Add_fBlock_Rating)) < FLT_MIN)
				AddString(1, FormatString("%.0f%%", round(cItemWrapper.sItemInfo.JobItem.Add_fBlock_Rating)));
			else
				AddString(1, FormatString("%.1f%%", cItemWrapper.sItemInfo.JobItem.Add_fBlock_Rating));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Add_fBlock_Rating, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Add_fBlock_Rating, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f%%", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec MagicAPT
		if (cItemWrapper.sItemInfo.JobItem.Add_fMagic_Mastery)
		{
			AddString(0, "Ataque Mágico:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.0f", cItemWrapper.sItemInfo.JobItem.Add_fMagic_Mastery));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Add_fMagic_Mastery, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Add_fMagic_Mastery, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.0f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec fMovementSpeed
		if (cItemWrapper.sItemInfo.JobItem.Add_fSpeed)
		{
			AddString(0, "Velocidade:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.JobItem.Add_fSpeed));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Add_fSpeed, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Add_fSpeed, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec Resistances
		{
			if (cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_BIONIC])
			{
				AddString(0, "Orgânico:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_BIONIC]));

				SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_BIONIC] - cItemCompare.sItemInfo.JobItem.Add_Resistance[sITEMINFO_BIONIC];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_FIRE])
			{
				AddString(0, "Fogo:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_FIRE]));

				SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_FIRE] - cItemCompare.sItemInfo.JobItem.Add_Resistance[sITEMINFO_FIRE];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_ICE])
			{
				AddString(0, "Gelo:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_ICE]));

				SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_ICE] - cItemCompare.sItemInfo.JobItem.Add_Resistance[sITEMINFO_ICE];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_LIGHTING])
			{
				AddString(0, "Raio:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_LIGHTING]));

				SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

				//Compare Item
				if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
				{
					int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_LIGHTING] - cItemCompare.sItemInfo.JobItem.Add_Resistance[sITEMINFO_LIGHTING];

					if (bIsEquippedItem)
						SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

					if (iCalc != 0)
						AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
				}
			}

			if (cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_POISON])
			{
				AddString(0, "Veneno:", D3DCOLOR_ARGB(255, 245, 240, 218));
				AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Resistance[sITEMINFO_POISON]));

				SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);
			}
		}

		//Spec ATK Power
		if (cItemWrapper.sItemInfo.JobItem.Lev_Damage[1])
		{
			AddString(0, "Poder de Ataque:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("LV/%d", cItemWrapper.sItemInfo.JobItem.Lev_Damage[1]));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = 0;
				BOOL bForceBlue = FALSE;

				if (cItemCompare.sItemInfo.JobItem.Lev_Damage[1] == 0)
				{
					iCalc = cItemWrapper.sItemInfo.JobItem.Lev_Damage[1];
					bForceBlue = TRUE; //force text to be blue
				}
				else
				{
					iCalc = cItemWrapper.sItemInfo.JobItem.Lev_Damage[1] - cItemCompare.sItemInfo.JobItem.Lev_Damage[1];
				}

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%sLV/%d",
						iCalc > 0 ? "+" : "",
						iCalc),
						iCalc > 0 && !bForceBlue ? D3DCOLOR_ARGB(255, 225, 30, 0) : D3DCOLOR_ARGB(255, 0, 130, 255));
			}
		}

		//Spec ATK Rtg
		if (cItemWrapper.sItemInfo.JobItem.Lev_Attack_Rating)
		{
			AddString(0, "Taxa de Ataque:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("LV/%d", cItemWrapper.sItemInfo.JobItem.Lev_Attack_Rating));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = 0;
				BOOL bForceBlue = FALSE;

				if (cItemCompare.sItemInfo.JobItem.Lev_Attack_Rating == 0)
				{
					iCalc = cItemWrapper.sItemInfo.JobItem.Lev_Attack_Rating;
					bForceBlue = TRUE; //force text to be blue
				}
				else
				{
					iCalc = cItemWrapper.sItemInfo.JobItem.Lev_Attack_Rating - cItemCompare.sItemInfo.JobItem.Lev_Attack_Rating;
				}

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%sLV/%d",
						iCalc > 0 ? "+" : "",
						iCalc),
						iCalc > 0 && !bForceBlue ? D3DCOLOR_ARGB(255, 225, 30, 0) : D3DCOLOR_ARGB(255, 0, 130, 255));
			}
		}

		//Spec Range
		if (cItemWrapper.sItemInfo.JobItem.Add_Shooting_Range)
		{
			AddString(0, "Alcance:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%d", cItemWrapper.sItemInfo.JobItem.Add_Shooting_Range));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.JobItem.Add_Shooting_Range - cItemCompare.sItemInfo.JobItem.Add_Shooting_Range;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%s%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec HP Boost
		if (cItemWrapper.sItemInfo.JobItem.Lev_Life)
		{
			AddString(0, "Max. HP Boost:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("LV/%d", cItemWrapper.sItemInfo.JobItem.Lev_Life));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.JobItem.Lev_Life - cItemCompare.sItemInfo.JobItem.Lev_Life;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%sLV/%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec MP Boost
		if (cItemWrapper.sItemInfo.JobItem.Lev_Mana)
		{
			AddString(0, "Max. MP Boost:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("LV/%d", cItemWrapper.sItemInfo.JobItem.Lev_Mana));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				int iCalc = cItemWrapper.sItemInfo.JobItem.Lev_Mana - cItemCompare.sItemInfo.JobItem.Lev_Mana;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (iCalc != 0)
					AddCompareString(FormatString("%sLV/%d", iCalc > 0 ? "+" : "", iCalc), iCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec HP Regen
		if (cItemWrapper.sItemInfo.JobItem.Per_Life_Regen && ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) != sinFO1))
		{
			AddString(0, "HP Regen.:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.JobItem.Per_Life_Regen));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Per_Life_Regen, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Per_Life_Regen, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec MP Regen
		if (cItemWrapper.sItemInfo.JobItem.Per_Mana_Regen && ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) != sinFO1))
		{
			AddString(0, "MP Regen:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.JobItem.Per_Mana_Regen));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Per_Mana_Regen, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Per_Mana_Regen, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		//Spec STM Regen
		if (cItemWrapper.sItemInfo.JobItem.Per_Stamina_Regen && ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) != sinFO1))
		{
			AddString(0, "STM Regen.:", D3DCOLOR_ARGB(255, 245, 240, 218));
			AddString(1, FormatString("%.1f", cItemWrapper.sItemInfo.JobItem.Per_Stamina_Regen));

			SetLineColor(D3DCOLOR_ARGB(255, 235, 190, 60), 1);

			//Compare Item
			if (bCompareItem && (bIsEquippedItem || bIsPerfectItem))
			{
				float fThisValueRounded = RoundOffToNearestDP(cItemWrapper.sItemInfo.JobItem.Per_Stamina_Regen, 1);
				float fCompareValueRounded = RoundOffToNearestDP(cItemCompare.sItemInfo.JobItem.Per_Stamina_Regen, 1);
				float fCalc = fThisValueRounded - fCompareValueRounded;

				if (bIsEquippedItem)
					SetLineColor(D3DCOLOR_ARGB(255, 165, 157, 140), 0);

				if (fCalc >= 0.1f || fCalc <= -0.1f)
					AddCompareString(FormatString("%s%.1f", fCalc > 0 ? "+" : "", fCalc), fCalc > 0 ? D3DCOLOR_ARGB(255, 0, 130, 255) : D3DCOLOR_ARGB(255, 225, 30, 0));
			}
		}

		// Next Line
		//AddString(0, "");
		//AddString(1, "");

		//AddString(0, "Price:");
		//AddString(1, cItemWrapper.sItem.iSalePrice > 0 ? FormatString("%s Gold", FormatNumber(cItemWrapper.sItem.iSalePrice)) : "Free");

		// Price
		// Shop is open?

		if (cShop.OpenFlag)
		{
			// Next Line
			AddString(0, "");
			AddString(1, "");

			if (bShowBuyPrice)
			{
				// Normal, price
				if (bShowBuyPrice == 2)
				{
					// Get Inventory Price Item
					sITEMPRICE sSellItemData = { 0, 0, 0 };
					sSellItemData = cInvenTory.GetInvenItemPrice(pcItemData);
					pcItemData->SellPrice = sSellItemData.SellPrice;

					if (CursorClass == EMOUSEICON_Repair)
					{
						if (pcItemData->sItemInfo.Durability[1] > 0)
						{
							AddString(0, "Preço de Reparo:");
							AddString(1, sSellItemData.RepairCost > 0 ? FormatString("%s Gold", FormatNumber(sSellItemData.RepairCost)) : " - ");
						}
					}
					else
					{
						// Set Price Item
						if (pcItemData->sItemInfo.ItemKindCode != ITEM_KIND_QUEST && pcItemData->sItemInfo.ItemKindCode != ITEM_KIND_QUEST_WEAPON)
						{
							AddString(0, "Preço de Venda:");
							AddString(1, pcItemData->SellPrice > 0 ? FormatString("%s Gold", FormatNumber(pcItemData->SellPrice)) : "Grátis");
						}

						if (pcItemData->sItemInfo.Durability[1] > 0)
						{
							AddString(0, "Preço de Reparo:");
							AddString(1, sSellItemData.RepairCost > 0 ? FormatString("%s Gold", FormatNumber(sSellItemData.RepairCost)) : " - ");
						}
					}
				}

				if (bShowBuyPrice == 1)
				{
					// Set Text
					char szTempText[32] = { 0 };
					extern int ShopItemPrice;

					AddString(0, "Preço de Compra:");
					AddString(1, ShopItemPrice > 0 ? FormatString("%s Ouro", FormatNumber(ShopItemPrice)) : "Grátis");
				}
			}
		}

		extern int MyShopItemPriceLine;
		MyShopItemPriceLine = 0;

		// Personal Shop is open?
		if (cMyShop.OpenFlag || cCharShop.OpenFlag)
		{
			if (bShowSellPrice)
			{
				// Next Line
				AddString(0, "");
				AddString(1, "");

				// Set Text
				int iPrice = MyShopItemSellMoney2; // Value from personal shop

				AddString(0, "Preço:");
				AddString(1, iPrice > 0 ? FormatString("%s Ouro", FormatNumber(iPrice)) : "Grátis");
			}
		}

		//Debug Item Info
		if (bShowChecksums)
		{
			AddString(0, "\n");
			AddString(1, "");

			AddString(0, FormatString("%d", cItemWrapper.sItemInfo.ItemHeader.Head));
			AddString(1, "");

			AddString(0, FormatString("%d", cItemWrapper.sItemInfo.ItemHeader.dwChkSum));
			AddString(1, "");
		}

		//Socket Item
		//if (cItemWrapper.sItem.sSocketData[0].eSocketType != SOCKETTYPE_None)
		//{
		//	AddString(0, "\n\n ");
		//	AddString(1, "\n\n ");
		//}

		////Can't use Item?
		//if( bCantUseItem )
		//{
		//	cBackgroundColor1 = UI::Color( 235, 10, 10, 10 );
		//	cBackgroundColor2 = UI::Color( 235, 75, 25, 25 );
		//}

		//Clear Text
		paSocketText[0][0]->SetText("");
		paSocketText[0][1]->SetText("");
		paSocketText[1][0]->SetText("");
		paSocketText[1][1]->SetText("");

		//for (int i = 0; i < 2; i++)
		//{
		//	if (cItemWrapper.sItem.sSocketData[i].eSocketType != SOCKETTYPE_None)
		//	{
		//		iSocketCount += 1;

		//		//Get Gem Image
		//		switch (cItemWrapper.sItem.sSocketData[i].eGemItem)
		//		{
		//		case GEMITEM_Ruby:
		//			iSocketGem[i] = 0;
		//			break;
		//		case GEMITEM_Topaz:
		//			iSocketGem[i] = 3;
		//			break;
		//		case GEMITEM_Emerald:
		//			iSocketGem[i] = 6;
		//			break;
		//		case GEMITEM_Aquamarine:
		//			iSocketGem[i] = 9;
		//			break;
		//		case GEMITEM_Amethyst:
		//			iSocketGem[i] = 12;
		//			break;
		//		}

		//		//Format Gem Type
		//		if (cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Triangular)
		//			paSocketText[i][1]->SetText("+2");
		//		else if (cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Square)
		//		{
		//			paSocketText[i][1]->SetText("+4");
		//			iSocketGem[i] += 1;
		//		}
		//		else if (cItemWrapper.sItem.sSocketData[i].eGemType == GEMTYPE_Marquise)
		//		{
		//			paSocketText[i][1]->SetText("+6");
		//			iSocketGem[i] += 2;
		//		}

		//		//Format Name and Stats of Gem
		//		switch (cItemWrapper.sItem.sSocketData[i].eGemItem)
		//		{
		//		case GEMITEM_Topaz:
		//			paSocketText[i][0]->SetColor(D3DCOLOR_ARGB(255, 255, 220, 0));
		//			paSocketText[i][0]->SetText("Topaz Gem");

		//			paSocketText[i][1]->AppendText(" Talent");
		//			break;
		//		case GEMITEM_Aquamarine:
		//			paSocketText[i][0]->SetColor(D3DCOLOR_ARGB(255, 0, 150, 255));
		//			paSocketText[i][0]->SetText("Aquamarine Gem");

		//			paSocketText[i][1]->AppendText(" Spirit");
		//			break;
		//		case GEMITEM_Ruby:
		//			paSocketText[i][0]->SetColor(D3DCOLOR_ARGB(255, 180, 25, 25));
		//			paSocketText[i][0]->SetText("Ruby Gem");

		//			paSocketText[i][1]->AppendText(" Strenght");
		//			break;
		//		case GEMITEM_Amethyst:
		//			paSocketText[i][0]->SetColor(D3DCOLOR_ARGB(255, 150, 5, 235));
		//			paSocketText[i][0]->SetText("Amethyst Gem");

		//			paSocketText[i][1]->AppendText(" Health");
		//			break;
		//		case GEMITEM_Emerald:
		//			paSocketText[i][0]->SetColor(D3DCOLOR_ARGB(255, 0, 170, 10));
		//			paSocketText[i][0]->SetText("Emerald Gem");

		//			paSocketText[i][1]->AppendText(" Agility");
		//			break;
		//		}
		//	}
		//}

		//Not have description or stats? So only show the Title of Item
		if (vLeftText.size() < 3)
		{
			vLeftText.erase(vLeftText.begin() + 1);
			vRightText.erase(vRightText.begin() + 1);
		}

		return TRUE;
	}

	void ItemInfoBox::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
	{
		// Flag e item válido selecionado
		if (sinShowItemInfoFlag && cItemWrapper.sItemInfo.ItemName[0])
		{
			int iFoundWidth = 0;
			int iMaxWidth = 184;

			int iItemWidth = cItemWrapper.w;
			int iItemHeight = cItemWrapper.h;

			int iItemImageXOffset = 0;
			if (iItemWidth == 22)
				iItemImageXOffset = 22;
			else if (iItemWidth == 44)
				iItemImageXOffset = 11;
			else if (iItemWidth == 66)
				iItemImageXOffset = 0;

			int iItemImageYOffset = 0;
			if (iItemHeight == 22)
				iItemImageYOffset = 26;
			else if (iItemHeight == 44)
				iItemImageYOffset = 11;
			else if (iItemHeight == 66)
				iItemImageYOffset = 0;

			int padding = 20;
			int spacing = 10;

			//Determine Width
			if ((bIsEquippedItem || bIsPerfectItem) && bCompareItem)
			{

				for (UINT i = 0; i < vRowCompare.size(); i++)
				{
					Text_ptr pLeftText = vLeftText[vRowCompare[i] - 1];
					Text_ptr pRightText = vRightText[vRowCompare[i] - 1];
					Text_ptr pCompareText = vCompareText[i];

					//Get Widest text line
					if ((pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + spacing + pCompareText->GetTextWidth() + padding * 2) > iMaxWidth)
						iMaxWidth = (pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + spacing + pCompareText->GetTextWidth() + padding * 2) > iMaxWidth ?
						pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + spacing + pCompareText->GetTextWidth() + padding * 2 : iMaxWidth;
				}
			}

			for (UINT i = 0; i < vLeftText.size(); i++)
			{
				Text_ptr pLeftText = vLeftText[i];
				Text_ptr pRightText = vRightText[i];

				//Get Widest text line
				if ((pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + padding * 2) > iMaxWidth)
					iMaxWidth = (pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + padding * 2) > iMaxWidth ?
					pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + padding * 2 : iMaxWidth;
			}

			//Apply Title if has Affixes
			Text_ptr pTitleText;
			Text_ptr pTitleText2;
			BOOL hasSecondLine = FALSE;
			if (bHasLargeName)
			{
				int iEdgeSpacing = 20;
				UI::Text_ptr pFirstLine;

				int iFormat = 0;

				//Split Name if too long
				std::string strItemName;
				strItemName.assign(sItemName.c_str());

				std::string sFirstLine;
				sFirstLine.assign(sItemName.c_str());

				std::string sSecondLine;

				int iNameLength = strlen(sItemName.c_str());

				BOOL bHasMultiLine = strItemName.find('\n') != std::string::npos;

				if (iNameLength > 15 && !bHasMultiLine)
				{
					std::vector<int> SpaceList;

					int iSpaceCount = 0;
					for (std::string::iterator it = strItemName.begin(); it != strItemName.end(); ++it)
					{
						if (*it == ' ')
						{
							SpaceList.push_back(iNameLength - strlen(it._Ptr) + 1);
						}
					}

					//Find the space that is closest to the item name center
					int iMidSpace = 0;
					int iMidSpaceDistance = 1000;  //Set high to start
					for (int i : SpaceList)
					{
						int iCurrentMidSpaceDist = abs(i - (iNameLength / 2));

						if (iMidSpaceDistance > iCurrentMidSpaceDist)
						{
							iMidSpaceDistance = iCurrentMidSpaceDist;
							iMidSpace = i;
						}
					}

					sFirstLine = strItemName.substr(0, iMidSpace - 1);
					sSecondLine = strItemName.substr(iMidSpace);

					SpaceList.clear();
					hasSecondLine = TRUE;
				}

				pTitleText = std::make_shared<Text>(FormatString("%s", sFirstLine.c_str())
					, Rectangle2D(0, 0, 180, 50), FALSE, D3DCOLOR_ARGB(255, 245, 240, 218));
				pTitleText->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
				pTitleText->SetMultiLine(TRUE);
				pTitleText->SetNoClip(TRUE);
				pTitleText->SetColor(D3DCOLOR_ARGB(255, 245, 240, 218));
				pTitleText->SetHorizontalAlign(ALIGN_Center);


				if (hasSecondLine)
				{
					pTitleText2 = std::make_shared<Text>(FormatString("%s", sSecondLine.c_str())
						, Rectangle2D(0, 0, 180, 50), FALSE, D3DCOLOR_ARGB(255, 245, 240, 218));
					pTitleText2->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
					pTitleText2->SetMultiLine(TRUE);
					pTitleText2->SetNoClip(TRUE);
					pTitleText2->SetColor(D3DCOLOR_ARGB(255, 245, 240, 218));
					pTitleText2->SetHorizontalAlign(ALIGN_Center);
				}

				int iGreatestWidth = 0;

				if ((cItemWrapper.sItemInfo.CODE & sinITEM_MASK2) == sinOA1)  //I Have absolutely no idea why, but amulats are always coming back with too small a number
					iGreatestWidth = pTitleText->GetTextWidth() + iEdgeSpacing * 2 + 15;
				else
					iGreatestWidth = pTitleText->GetTextWidth() + iEdgeSpacing * 2;

				if (hasSecondLine)
					if (iGreatestWidth < (pTitleText2->GetTextWidth() + iEdgeSpacing * 2))
						iGreatestWidth = pTitleText2->GetTextWidth() + iEdgeSpacing * 2;

				iMaxWidth = iGreatestWidth > iMaxWidth ? iGreatestWidth : iMaxWidth;

				int iItemPictureText = iGreatestWidth + 15 + cItemWrapper.w + 22 - iItemImageXOffset;

				if (bHasLargeName)
					iMaxWidth = iItemPictureText > iMaxWidth ? iItemPictureText : iMaxWidth;


				if (hasSecondLine)
				{
					pTitleText->SetBox(Rectangle2D((iMaxWidth - pTitleText->GetWidth()) / 2, 3, pTitleText->GetWidth(), pTitleText->GetHeightText()));
					pTitleText2->SetBox(Rectangle2D((iMaxWidth - pTitleText2->GetWidth()) / 2, 19, pTitleText2->GetWidth(), pTitleText2->GetHeightText()));
				}
				else
				{
					pTitleText->SetBox(Rectangle2D((iMaxWidth - pTitleText->GetWidth()) / 2, bHasMultiLine ? 3 : 11, pTitleText->GetWidth(), pTitleText->GetHeightText()));
				}

			}

			if (isCompare != TRUE)
				iBaseMaxWidth = iItemInfoBoxWidth;

			iItemInfoBoxWidth = iMaxWidth;

			//Adjusting the iItemSlot

			if (isCompare != TRUE)
				iX = (cItemWrapper.x + (cItemWrapper.w / 2)) - iItemInfoBoxWidth / 2;
			else
				iX = (cItemWrapper.x + (cItemWrapper.w / 2)) - iBaseMaxWidth / 2;

			iY = cItemWrapper.y - (vLeftText.size() * 14);

			if (bDisplayHelp)
				iY -= 37;

			if (bIsPerfectItem)
				iY += 19;

			if (!bHasLargeName)
				iY -= 17;

			int iSocketHeightExtra = 0;

			//for (int i = 0; i < 2; i++)
			//{
			//	if (cItemWrapper.sItem.sSocketData[i].eSocketType)
			//		iSocketHeightExtra += i > 0 ? 36 : 8;
			//}

			//Set iWidth and iHeight of Box
			int iItemInfoBoxHeight = 0;
			if (vLeftText.size() <= 2)
				iItemInfoBoxHeight = (14 * 3);  //Minimum Box height
			else
				iItemInfoBoxHeight = (14 * vLeftText.size());

			//iItemSlot of box is out the window?
			if (iX < 0)
				iX = 12;

			if (iY < 0)
				iY = 24;

			if (isCompare != TRUE)
			{
				if (iX + iItemInfoBoxWidth > (int)RESOLUTION_WIDTH)
					iX -= ((iX + iItemInfoBoxWidth) - RESOLUTION_WIDTH) + 16;
			}
			else
			{
				if (iX + iBaseMaxWidth > (int)RESOLUTION_WIDTH)
					iX -= ((iX + iBaseMaxWidth) - RESOLUTION_WIDTH) + 16;
			}

			if (isCompare)
				iX = iX + iBaseMaxWidth + 5;


			//Fix position if is showing from Inventory
			if (sinShowItemInfoFlag == 2)
				iY += RESOLUTION_HEIGHT - 600 - iSocketHeightExtra;

			//Update Item Info Box Width and Height
			iFoundWidth += 10;
			iItemInfoBoxHeight += 30 + iSocketHeightExtra;

			/*EItemRarity rarity = EItemRarity::NONE;

			if (bIsEquippedItem)
			{
				rarity = cItemWrapper.sItem.eRarity;
			}*/

			//if ( bIsEquippedItem  ) ? cItemCompare.sItem.eRarity : cItemWrapper.sItem.eRarity;
		/*	rarity = cItemWrapper.sItem.eRarity;*/

			//Draw New "Hold P" box
			if (iItemInfoBoxWidth != -1 && bIsPerfectItem != TRUE && bDisplayHelp && sinShowItemInfoFlag != 1)
			{
				Text_ptr pLeftText = std::make_shared<Text>("Aperte       para ver Perfeito", Rectangle2D(0, 0, 180, 20), FALSE, D3DCOLOR_ARGB(255, 240, 215, 150));

				pLeftText->SetFont("Arial", 14, 6, FALSE, FALSE, -1);

				pLeftText->SetMultiLine(TRUE);
				pLeftText->SetNoClip(TRUE);

				pLeftText->SetColor(D3DCOLOR_ARGB(220, 255, 255, 255));

				//UI::ImageRender::Render( pImageBorderNoneDivider, (iX + 90) - (pImageBorderNoneDivider->GetWidth() >> 1), iY + (iSpecLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1 );

				pLeftText->SetHorizontalAlign(ALIGN_Center);
				pLeftText->SetBox(Rectangle2D((iItemInfoBoxWidth - pLeftText->GetWidth()) / 2, iItemInfoBoxHeight - 15, 180, iHeight));

				int Width = 0;

				if (iItemInfoBoxWidth > 200)
					Width = iItemInfoBoxWidth - 30;
				else
					Width = iItemInfoBoxWidth - 15;

				int Height = 25;
				int LeftEdge = iX + (iItemInfoBoxWidth - Width) / 2;
				int TopEdge = iY + iItemInfoBoxHeight - 17;

				int WidthBG = Width - 14;
				int HeightBG = 17;
				int LeftEdgeBG = iX + (iItemInfoBoxWidth - WidthBG) / 2;
				int TopEdgeBG = iY + iItemInfoBoxHeight - 17;

				GRAPHICENGINE->DrawRectangle2D(Rectangle2D(LeftEdgeBG, TopEdgeBG, WidthBG, HeightBG), cBackgroundColor2.Get(), cBackgroundColor2.Get());

				UI::ImageRender::Render(pImageBorderNoneInfoLeft, LeftEdge, TopEdge, pImageBorderNoneInfoLeft->GetWidth(), pImageBorderNoneInfoLeft->GetHeight(), 0, 0, -1);
				UI::ImageRender::Render(pImageBorderNoneInfo, LeftEdge + pImageBorderNoneInfoLeft->GetWidth(), TopEdge, Width - pImageBorderNoneInfoLeft->GetWidth() - pImageBorderNoneInfoRight->GetWidth(), pImageBorderNoneInfo->GetHeight(), 0, 0, -1);
				UI::ImageRender::Render(pImageBorderNoneInfoRight, LeftEdge + Width - pImageBorderNoneInfoRight->GetWidth(), TopEdge, pImageBorderNoneInfoRight->GetWidth(), pImageBorderNoneInfoRight->GetHeight(), 0, 0, -1);

				/*switch (rarity)
				{
				case EItemRarity::NONE:
				{
					UI::ImageRender::Render(pImageBorderNoneInfoLeft, LeftEdge, TopEdge, pImageBorderNoneInfoLeft->GetWidth(), pImageBorderNoneInfoLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderNoneInfo, LeftEdge + pImageBorderNoneInfoLeft->GetWidth(), TopEdge, Width - pImageBorderNoneInfoLeft->GetWidth() - pImageBorderNoneInfoRight->GetWidth(), pImageBorderNoneInfo->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderNoneInfoRight, LeftEdge + Width - pImageBorderNoneInfoRight->GetWidth(), TopEdge, pImageBorderNoneInfoRight->GetWidth(), pImageBorderNoneInfoRight->GetHeight(), 0, 0, -1);				}
				break;

				case EItemRarity::COMMON:
				{
					UI::ImageRender::Render(pImageBorderCommonInfoLeft, LeftEdge, TopEdge, pImageBorderCommonInfoLeft->GetWidth(), pImageBorderCommonInfoLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderCommonInfo, LeftEdge + pImageBorderCommonInfoLeft->GetWidth(), TopEdge, Width - pImageBorderCommonInfoLeft->GetWidth() - pImageBorderCommonInfoRight->GetWidth(), pImageBorderCommonInfo->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderCommonInfoRight, LeftEdge + Width - pImageBorderCommonInfoRight->GetWidth(), TopEdge, pImageBorderCommonInfoRight->GetWidth(), pImageBorderCommonInfoRight->GetHeight(), 0, 0, -1);
				}
				break;

				case EItemRarity::UNCOMMON:
				{
					UI::ImageRender::Render(pImageBorderUncommonInfoLeft, LeftEdge, TopEdge, pImageBorderUncommonInfoLeft->GetWidth(), pImageBorderUncommonInfoLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderUncommonInfo, LeftEdge + pImageBorderUncommonInfoLeft->GetWidth(), TopEdge, Width - pImageBorderUncommonInfoLeft->GetWidth() - pImageBorderUncommonInfoRight->GetWidth(), pImageBorderUncommonInfo->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderUncommonInfoRight, LeftEdge + Width - pImageBorderUncommonInfoRight->GetWidth(), TopEdge, pImageBorderUncommonInfoRight->GetWidth(), pImageBorderUncommonInfoRight->GetHeight(), 0, 0, -1);
				}
				break;

				case EItemRarity::RARE:
				{
					UI::ImageRender::Render(pImageBorderRareInfoLeft, LeftEdge, TopEdge, pImageBorderRareInfoLeft->GetWidth(), pImageBorderRareInfoLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderRareInfo, LeftEdge + pImageBorderRareInfoLeft->GetWidth(), TopEdge, Width - pImageBorderRareInfoLeft->GetWidth() - pImageBorderRareInfoRight->GetWidth(), pImageBorderRareInfo->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderRareInfoRight, LeftEdge + Width - pImageBorderRareInfoRight->GetWidth(), TopEdge, pImageBorderRareInfoRight->GetWidth(), pImageBorderRareInfoRight->GetHeight(), 0, 0, -1);
				}
				break;

				case EItemRarity::EPIC:
				{
					UI::ImageRender::Render(pImageBorderEpicInfoLeft, LeftEdge, TopEdge, pImageBorderEpicInfoLeft->GetWidth(), pImageBorderEpicInfoLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderEpicInfo, LeftEdge + pImageBorderEpicInfoLeft->GetWidth(), TopEdge, Width - pImageBorderEpicInfoLeft->GetWidth() - pImageBorderEpicInfoRight->GetWidth(), pImageBorderEpicInfo->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderEpicInfoRight, LeftEdge + Width - pImageBorderEpicInfoRight->GetWidth(), TopEdge, pImageBorderEpicInfoRight->GetWidth(), pImageBorderEpicInfoRight->GetHeight(), 0, 0, -1);
				}
				break;

				case EItemRarity::LEGENDARY:
				{
					UI::ImageRender::Render(pImageBorderLegendaryInfoLeft, LeftEdge, TopEdge, pImageBorderLegendaryInfoLeft->GetWidth(), pImageBorderLegendaryInfoLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderLegendaryInfo, LeftEdge + pImageBorderLegendaryInfoLeft->GetWidth(), TopEdge, Width - pImageBorderLegendaryInfoLeft->GetWidth() - pImageBorderLegendaryInfoRight->GetWidth(), pImageBorderLegendaryInfo->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderLegendaryInfoRight, LeftEdge + Width - pImageBorderLegendaryInfoRight->GetWidth(), TopEdge, pImageBorderLegendaryInfoRight->GetWidth(), pImageBorderLegendaryInfoRight->GetHeight(), 0, 0, -1);
				}
				break;
				}*/

				UI::ImageRender::Render(pImageGradient, LeftEdge + 2, TopEdge, Width - 4, pImageGradient->GetHeight(), 0, 0, -1);
				UI::ImageRender::Render(pImagePKey, iX + (iItemInfoBoxWidth / 2) - 30, TopEdge + 4, pImagePKey->GetWidth(), pImagePKey->GetHeight(), 0, 0, -1);
				pLeftText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);

			}

			//Draw Item Info Box Background
			if (iItemInfoBoxWidth != -1)
			{
				DrawItemInfoBox(iX, iY, iItemInfoBoxWidth, iItemInfoBoxHeight);

				if (bHasLargeName)
				{
					int Width = iItemInfoBoxWidth - 10;
					int LeftEdge = iX + (iItemInfoBoxWidth - (Width)) / 2;
					int TopEdge = iY - 5;

					UI::ImageRender::Render(pImageBorderNoneNameFrameLeft, LeftEdge, TopEdge, pImageBorderNoneNameFrameLeft->GetWidth(), pImageBorderNoneNameFrameLeft->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderNoneNameFrameCenter, LeftEdge + pImageBorderNoneNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderNoneNameFrameLeft->GetWidth() - pImageBorderNoneNameFrameRight->GetWidth(), pImageBorderNoneNameFrameCenter->GetHeight(), 0, 0, -1);
					UI::ImageRender::Render(pImageBorderNoneNameFrameRight, LeftEdge + Width - pImageBorderNoneNameFrameRight->GetWidth(), TopEdge, pImageBorderNoneNameFrameRight->GetWidth(), pImageBorderNoneNameFrameRight->GetHeight(), 0, 0, -1);

					UI::ImageRender::Render(pImageBorderNoneNameFrameBackground, iX + (Width - pImageBorderNoneNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderNoneNameFrameBackground->GetWidth(), pImageBorderNoneNameFrameBackground->GetHeight(), 0, 0, -1);

					/*	switch (rarity)
						{
						case EItemRarity::NONE:
						{
							UI::ImageRender::Render(pImageBorderNoneNameFrameLeft, LeftEdge, TopEdge, pImageBorderNoneNameFrameLeft->GetWidth(), pImageBorderNoneNameFrameLeft->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderNoneNameFrameCenter, LeftEdge + pImageBorderNoneNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderNoneNameFrameLeft->GetWidth() - pImageBorderNoneNameFrameRight->GetWidth(), pImageBorderNoneNameFrameCenter->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderNoneNameFrameRight, LeftEdge + Width - pImageBorderNoneNameFrameRight->GetWidth(), TopEdge, pImageBorderNoneNameFrameRight->GetWidth(), pImageBorderNoneNameFrameRight->GetHeight(), 0, 0, -1);

							UI::ImageRender::Render(pImageBorderNoneNameFrameBackground, iX + (Width - pImageBorderNoneNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderNoneNameFrameBackground->GetWidth(), pImageBorderNoneNameFrameBackground->GetHeight(), 0, 0, -1);
						}
						break;
						case EItemRarity::COMMON:
						{
							UI::ImageRender::Render(pImageBorderCommonNameFrameLeft, LeftEdge, TopEdge, pImageBorderCommonNameFrameLeft->GetWidth(), pImageBorderCommonNameFrameLeft->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderCommonNameFrameCenter, LeftEdge + pImageBorderCommonNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderCommonNameFrameLeft->GetWidth() - pImageBorderCommonNameFrameRight->GetWidth(), pImageBorderCommonNameFrameCenter->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderCommonNameFrameRight, LeftEdge + Width - pImageBorderCommonNameFrameRight->GetWidth(), TopEdge, pImageBorderCommonNameFrameRight->GetWidth(), pImageBorderCommonNameFrameRight->GetHeight(), 0, 0, -1);

							UI::ImageRender::Render(pImageBorderCommonNameFrameBackground, iX + (Width - pImageBorderCommonNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderCommonNameFrameBackground->GetWidth(), pImageBorderCommonNameFrameBackground->GetHeight(), 0, 0, -1);
						}
						break;
						case EItemRarity::UNCOMMON:
						{
							UI::ImageRender::Render(pImageBorderUncommonNameFrameLeft, LeftEdge, TopEdge, pImageBorderUncommonNameFrameLeft->GetWidth(), pImageBorderUncommonNameFrameLeft->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderUncommonNameFrameCenter, LeftEdge + pImageBorderUncommonNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderUncommonNameFrameLeft->GetWidth() - pImageBorderUncommonNameFrameRight->GetWidth(), pImageBorderUncommonNameFrameCenter->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderUncommonNameFrameRight, LeftEdge + Width - pImageBorderUncommonNameFrameRight->GetWidth(), TopEdge, pImageBorderUncommonNameFrameRight->GetWidth(), pImageBorderUncommonNameFrameRight->GetHeight(), 0, 0, -1);

							UI::ImageRender::Render(pImageBorderUncommonNameFrameBackground, iX + (Width - pImageBorderUncommonNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderUncommonNameFrameBackground->GetWidth(), pImageBorderUncommonNameFrameBackground->GetHeight(), 0, 0, -1);
						}
						break;
						case EItemRarity::RARE:
						{
							UI::ImageRender::Render(pImageBorderRareNameFrameLeft, LeftEdge, TopEdge, pImageBorderRareNameFrameLeft->GetWidth(), pImageBorderRareNameFrameLeft->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderRareNameFrameCenter, LeftEdge + pImageBorderRareNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderRareNameFrameLeft->GetWidth() - pImageBorderRareNameFrameRight->GetWidth(), pImageBorderRareNameFrameCenter->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderRareNameFrameRight, LeftEdge + Width - pImageBorderRareNameFrameRight->GetWidth(), TopEdge, pImageBorderRareNameFrameRight->GetWidth(), pImageBorderRareNameFrameRight->GetHeight(), 0, 0, -1);

							UI::ImageRender::Render(pImageBorderRareNameFrameBackground, iX + (Width - pImageBorderRareNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderRareNameFrameBackground->GetWidth(), pImageBorderRareNameFrameBackground->GetHeight(), 0, 0, -1);
						}
						break;
						case EItemRarity::EPIC:
						{
							UI::ImageRender::Render(pImageBorderEpicNameFrameLeft, LeftEdge, TopEdge, pImageBorderEpicNameFrameLeft->GetWidth(), pImageBorderEpicNameFrameLeft->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderEpicNameFrameCenter, LeftEdge + pImageBorderEpicNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderEpicNameFrameLeft->GetWidth() - pImageBorderEpicNameFrameRight->GetWidth(), pImageBorderEpicNameFrameCenter->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderEpicNameFrameRight, LeftEdge + Width - pImageBorderEpicNameFrameRight->GetWidth(), TopEdge, pImageBorderEpicNameFrameRight->GetWidth(), pImageBorderEpicNameFrameRight->GetHeight(), 0, 0, -1);

							UI::ImageRender::Render(pImageBorderEpicNameFrameBackground, iX + (Width - pImageBorderEpicNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderEpicNameFrameBackground->GetWidth(), pImageBorderEpicNameFrameBackground->GetHeight(), 0, 0, -1);
						}
						break;
						case EItemRarity::LEGENDARY:
						{
							UI::ImageRender::Render(pImageBorderLegendaryNameFrameLeft, LeftEdge, TopEdge, pImageBorderLegendaryNameFrameLeft->GetWidth(), pImageBorderLegendaryNameFrameLeft->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderLegendaryNameFrameCenter, LeftEdge + pImageBorderLegendaryNameFrameLeft->GetWidth(), TopEdge, Width - pImageBorderLegendaryNameFrameLeft->GetWidth() - pImageBorderLegendaryNameFrameRight->GetWidth(), pImageBorderLegendaryNameFrameCenter->GetHeight(), 0, 0, -1);
							UI::ImageRender::Render(pImageBorderLegendaryNameFrameRight, LeftEdge + Width - pImageBorderLegendaryNameFrameRight->GetWidth(), TopEdge, pImageBorderLegendaryNameFrameRight->GetWidth(), pImageBorderLegendaryNameFrameRight->GetHeight(), 0, 0, -1);

							UI::ImageRender::Render(pImageBorderLegendaryNameFrameBackground, iX + (Width - pImageBorderLegendaryNameFrameBackground->GetWidth()) / 2, iY - 5, pImageBorderLegendaryNameFrameBackground->GetWidth(), pImageBorderLegendaryNameFrameBackground->GetHeight(), 0, 0, -1);
						}
						break;
						}*/

					pTitleText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
					if (hasSecondLine)
						pTitleText2->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);

				}



				//if (rarity == EItemRarity::COMMON)
				//{
				//	//Drawing Border
				UI::ImageRender::Render(pImageBorderCommonLeftTop, iX, iY - 10, pImageBorderCommonLeftTop->GetWidth(), pImageBorderCommonLeftTop->GetHeight(), -1);
				UI::ImageRender::Render(pImageBorderCommonTop, iX + pImageBorderCommonLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonRightTop->GetWidth(), pImageBorderCommonTop->GetHeight(), -1);
				UI::ImageRender::Render(pImageBorderCommonRightTop, iX + pImageBorderCommonLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonRightTop->GetWidth(), iY - 10, pImageBorderCommonRightTop->GetWidth(), pImageBorderCommonRightTop->GetHeight(), -1);
				UI::ImageRender::Render(pImageBorderCommonLeft, iX, iY - 10 + pImageBorderCommonLeftTop->GetHeight(), pImageBorderCommonLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonLeftBottom->GetWidth(), -1);
				UI::ImageRender::Render(pImageBorderCommonRight, iX + iItemInfoBoxWidth - pImageBorderCommonRight->GetWidth(), iY - 10 + pImageBorderCommonLeftTop->GetHeight(), pImageBorderCommonRight->GetWidth(), iItemInfoBoxHeight - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonLeftBottom->GetWidth(), -1);
				UI::ImageRender::Render(pImageBorderCommonLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderCommonLeftBottom->GetHeight(), pImageBorderCommonLeftBottom->GetWidth(), pImageBorderCommonLeftBottom->GetHeight(), -1);
				UI::ImageRender::Render(pImageBorderCommonRightBottom, iX + iItemInfoBoxWidth - pImageBorderCommonRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderCommonRightBottom->GetHeight(), pImageBorderCommonRightBottom->GetWidth(), pImageBorderCommonRightBottom->GetHeight(), -1);
				UI::ImageRender::Render(pImageBorderCommonBottom, iX + pImageBorderCommonLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderCommonBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderCommonLeftTop->GetWidth() - pImageBorderCommonRightTop->GetWidth(), pImageBorderCommonBottom->GetHeight(), -1);
				//}
				//else if (rarity == EItemRarity::UNCOMMON)
				//{
				//	//Drawing Border
				//	UI::ImageRender::Render(pImageBorderUncommonLeftTop, iX, iY - 10, pImageBorderUncommonLeftTop->GetWidth(), pImageBorderUncommonLeftTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonTop, iX + pImageBorderUncommonLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonRightTop->GetWidth(), pImageBorderUncommonTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonRightTop, iX + pImageBorderUncommonLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonRightTop->GetWidth(), iY - 10, pImageBorderUncommonRightTop->GetWidth(), pImageBorderUncommonRightTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonLeft, iX, iY - 10 + pImageBorderUncommonLeftTop->GetHeight(), pImageBorderUncommonLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonRight, iX + iItemInfoBoxWidth - pImageBorderUncommonRight->GetWidth(), iY - 10 + pImageBorderUncommonLeftTop->GetHeight(), pImageBorderUncommonRight->GetWidth(), iItemInfoBoxHeight - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonLeftBottom->GetHeight(), pImageBorderUncommonLeftBottom->GetWidth(), pImageBorderUncommonLeftBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonRightBottom, iX + iItemInfoBoxWidth - pImageBorderUncommonRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonRightBottom->GetHeight(), pImageBorderUncommonRightBottom->GetWidth(), pImageBorderUncommonRightBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderUncommonBottom, iX + pImageBorderUncommonLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderUncommonLeftTop->GetWidth() - pImageBorderUncommonRightTop->GetWidth(), pImageBorderUncommonBottom->GetHeight(), -1);

				//	UI::ImageRender::Render(pImageBorderUncommonCorner, iX - 11, iY - 22, pImageBorderUncommonCorner->GetWidth(), pImageBorderUncommonCorner->GetHeight(), 0, 0, -1, 1, 0);
				//	//UI::ImageRender::Render( pImageBorderUncommonBottomCorner, iX + iItemInfoBoxWidth - pImageBorderUncommonBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderUncommonBottomCorner->GetHeight() + 10,pImageBorderUncommonBottomCorner->GetWidth(), pImageBorderUncommonBottomCorner->GetHeight(), -1 );
				//}
				//else if (rarity == EItemRarity::RARE)
				//{
				//	//Drawing Border
				//	UI::ImageRender::Render(pImageBorderRareLeftTop, iX, iY - 10, pImageBorderRareLeftTop->GetWidth(), pImageBorderRareLeftTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderRareTop, iX + pImageBorderRareLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareRightTop->GetWidth(), pImageBorderRareTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderRareRightTop, iX + pImageBorderRareLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareRightTop->GetWidth(), iY - 10, pImageBorderRareRightTop->GetWidth(), pImageBorderRareRightTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderRareLeft, iX, iY - 10 + pImageBorderRareLeftTop->GetHeight(), pImageBorderRareLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderRareRight, iX + iItemInfoBoxWidth - pImageBorderRareRight->GetWidth(), iY - 10 + pImageBorderRareLeftTop->GetHeight(), pImageBorderRareRight->GetWidth(), iItemInfoBoxHeight - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderRareLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderRareLeftBottom->GetHeight(), pImageBorderRareLeftBottom->GetWidth(), pImageBorderRareLeftBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderRareRightBottom, iX + iItemInfoBoxWidth - pImageBorderRareRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderRareRightBottom->GetHeight(), pImageBorderRareRightBottom->GetWidth(), pImageBorderRareRightBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderRareBottom, iX + pImageBorderRareLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderRareBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderRareLeftTop->GetWidth() - pImageBorderRareRightTop->GetWidth(), pImageBorderRareBottom->GetHeight(), -1);

				//	UI::ImageRender::Render(pImageBorderRareCorner, iX - 11, iY - 22, pImageBorderRareCorner->GetWidth(), pImageBorderRareCorner->GetHeight(), 0, 0, -1, 1, 0);
				//	//UI::ImageRender::Render( pImageBorderRareBottomCorner, iX + iItemInfoBoxWidth - pImageBorderRareBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderRareBottomCorner->GetHeight() + 10,pImageBorderRareBottomCorner->GetWidth(), pImageBorderRareBottomCorner->GetHeight(), -1 );
				//}
				//else if (rarity == EItemRarity::EPIC)
				//{
				//	//Drawing Border
				//	UI::ImageRender::Render(pImageBorderEpicLeftTop, iX, iY - 10, pImageBorderEpicLeftTop->GetWidth(), pImageBorderEpicLeftTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicTop, iX + pImageBorderEpicLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicRightTop->GetWidth(), pImageBorderEpicTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicRightTop, iX + pImageBorderEpicLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicRightTop->GetWidth(), iY - 10, pImageBorderEpicRightTop->GetWidth(), pImageBorderEpicRightTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicLeft, iX, iY - 10 + pImageBorderEpicLeftTop->GetHeight(), pImageBorderEpicLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicRight, iX + iItemInfoBoxWidth - pImageBorderEpicRight->GetWidth(), iY - 10 + pImageBorderEpicLeftTop->GetHeight(), pImageBorderEpicRight->GetWidth(), iItemInfoBoxHeight - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderEpicLeftBottom->GetHeight(), pImageBorderEpicLeftBottom->GetWidth(), pImageBorderEpicLeftBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicRightBottom, iX + iItemInfoBoxWidth - pImageBorderEpicRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderEpicRightBottom->GetHeight(), pImageBorderEpicRightBottom->GetWidth(), pImageBorderEpicRightBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderEpicBottom, iX + pImageBorderEpicLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderEpicBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderEpicLeftTop->GetWidth() - pImageBorderEpicRightTop->GetWidth(), pImageBorderEpicBottom->GetHeight(), -1);

				//	UI::ImageRender::Render(pImageBorderEpicCorner, iX - 11, iY - 22, pImageBorderEpicCorner->GetWidth(), pImageBorderEpicCorner->GetHeight(), 0, 0, -1, 1, 0);
				//	//UI::ImageRender::Render( pImageBorderEpicBottomCorner, iX + iItemInfoBoxWidth - pImageBorderEpicBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderEpicBottomCorner->GetHeight() + 10,pImageBorderEpicBottomCorner->GetWidth(), pImageBorderEpicBottomCorner->GetHeight(), -1 );
				//}
				//else if (rarity == EItemRarity::LEGENDARY)
				//{
				//	//Drawing Border
				//	UI::ImageRender::Render(pImageBorderLegendaryLeftTop, iX, iY - 10, pImageBorderLegendaryLeftTop->GetWidth(), pImageBorderLegendaryLeftTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryTop, iX + pImageBorderLegendaryLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryRightTop->GetWidth(), pImageBorderLegendaryTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryRightTop, iX + pImageBorderLegendaryLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryRightTop->GetWidth(), iY - 10, pImageBorderLegendaryRightTop->GetWidth(), pImageBorderLegendaryRightTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryLeft, iX, iY - 10 + pImageBorderLegendaryLeftTop->GetHeight(), pImageBorderLegendaryLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryRight, iX + iItemInfoBoxWidth - pImageBorderLegendaryRight->GetWidth(), iY - 10 + pImageBorderLegendaryLeftTop->GetHeight(), pImageBorderLegendaryRight->GetWidth(), iItemInfoBoxHeight - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryLeftBottom->GetHeight(), pImageBorderLegendaryLeftBottom->GetWidth(), pImageBorderLegendaryLeftBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryRightBottom, iX + iItemInfoBoxWidth - pImageBorderLegendaryRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryRightBottom->GetHeight(), pImageBorderLegendaryRightBottom->GetWidth(), pImageBorderLegendaryRightBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderLegendaryBottom, iX + pImageBorderLegendaryLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderLegendaryLeftTop->GetWidth() - pImageBorderLegendaryRightTop->GetWidth(), pImageBorderLegendaryBottom->GetHeight(), -1);

				//	UI::ImageRender::Render(pImageBorderLegendaryCorner, iX - 11, iY - 22, pImageBorderLegendaryCorner->GetWidth(), pImageBorderLegendaryCorner->GetHeight(), 0, 0, -1, 1, 0);
				//	//UI::ImageRender::Render( pImageBorderLegendaryBottomCorner, iX + iItemInfoBoxWidth - pImageBorderLegendaryBottomCorner->GetWidth() + 10, iY - 10 + iItemInfoBoxHeight - pImageBorderLegendaryBottomCorner->GetHeight() + 10,pImageBorderLegendaryBottomCorner->GetWidth(), pImageBorderLegendaryBottomCorner->GetHeight(), -1 );
				//}
				//else
				//{
				//	//Drawing Border
				//	UI::ImageRender::Render(pImageBorderNoneLeftTop, iX, iY - 10, pImageBorderNoneLeftTop->GetWidth(), pImageBorderNoneLeftTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneTop, iX + pImageBorderNoneLeftTop->GetWidth(), iY - 10, iItemInfoBoxWidth - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneRightTop->GetWidth(), pImageBorderNoneTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneRightTop, iX + pImageBorderNoneLeftTop->GetWidth() + iItemInfoBoxWidth - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneRightTop->GetWidth(), iY - 10, pImageBorderNoneRightTop->GetWidth(), pImageBorderNoneRightTop->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneLeft, iX, iY - 10 + pImageBorderNoneLeftTop->GetHeight(), pImageBorderNoneLeft->GetWidth(), iItemInfoBoxHeight - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneRight, iX + iItemInfoBoxWidth - pImageBorderNoneRight->GetWidth(), iY - 10 + pImageBorderNoneLeftTop->GetHeight(), pImageBorderNoneRight->GetWidth(), iItemInfoBoxHeight - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneLeftBottom->GetWidth(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneLeftBottom, iX, iY - 10 + iItemInfoBoxHeight - pImageBorderNoneLeftBottom->GetHeight(), pImageBorderNoneLeftBottom->GetWidth(), pImageBorderNoneLeftBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneRightBottom, iX + iItemInfoBoxWidth - pImageBorderNoneRightBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderNoneRightBottom->GetHeight(), pImageBorderNoneRightBottom->GetWidth(), pImageBorderNoneRightBottom->GetHeight(), -1);
				//	UI::ImageRender::Render(pImageBorderNoneBottom, iX + pImageBorderNoneLeftBottom->GetWidth(), iY - 10 + iItemInfoBoxHeight - pImageBorderNoneBottom->GetHeight(), iItemInfoBoxWidth - pImageBorderNoneLeftTop->GetWidth() - pImageBorderNoneRightTop->GetWidth(), pImageBorderNoneBottom->GetHeight(), -1);
				//}


				//Disabled til we can fix the trade chat link bug
				if (bHasLargeName)
				{
					const DWORD itemBase = cItemWrapper.Class;
					const DWORD itemType = cItemWrapper.CODE & sinITEM_MASK2;

					if (itemBase == ITEM_CLASS_WEAPON_ONE ||
						itemBase == ITEM_CLASS_WEAPON_TWO ||
						itemType == sinDA1 ||
						itemType == sinDB1 ||
						itemType == sinDS1 ||
						itemType == sinDG1 ||
						itemType == sinDA2 ||
						itemType == sinOA1 ||
						itemType == sinOA2 ||
						itemType == sinOR1 ||
						itemType == sinOR2 ||
						itemType == sinOS1 ||
						itemType == sinOM1)
					{

						const int iItemDrawPosX = iX + iItemInfoBoxWidth - 18 - cItemWrapper.w + 22 - iItemImageXOffset;
						const int iItemDrawPosY = iY - 18 + iItemImageYOffset;

						if (LoadItemImage(&cItemWrapper.sItemInfo, &cItemWrapperImage))
						{
							DrawSprite(iItemDrawPosX, iItemDrawPosY, cItemWrapperImage.lpItem, 0, 0, cItemWrapper.w, cItemWrapper.h);

							iPerivousItemID = cItemWrapper.CODE;
							iPerivousItemImage = cItemWrapperImage.lpItem;
							iPerivousItemWidth = cItemWrapper.w;
							iPerivousItemHeight = cItemWrapper.h;
						}
						else
						{
							if (cItemWrapper.CODE == iPerivousItemID)
								DrawSprite(iItemDrawPosX, iItemDrawPosY, iPerivousItemImage, 0, 0, iPerivousItemWidth, iPerivousItemHeight);
							else
							{
								iPerivousItemID = 0;
								iPerivousItemImage = 0;
								iPerivousItemWidth = 0;
								iPerivousItemHeight = 0;
							}
						}
					}
				}
			}

			//Render Text
			if (vLeftText.size() == vRightText.size())
			{


				for (size_t i = 0; i < vLeftText.size(); i++)
				{
					Text_ptr pLeftText = vLeftText[i];
					Text_ptr pRightText = vRightText[i];

					//Load Texts
					pLeftText->SetMultiLine(TRUE);
					pLeftText->SetNoClip(TRUE);

					pRightText->SetNoClip(TRUE);

					pLeftText->SetHorizontalAlign(ALIGN_Left);
					pLeftText->SetBox(Rectangle2D(20, 5 + (14 * i), 180, iHeight));

					pRightText->SetHorizontalAlign(ALIGN_Left);
					pRightText->SetBox(Rectangle2D(pLeftText->GetTextWidth() + 25, 5 + (14 * i), 180, iHeight));

					if (pLeftText->GetTextWidth() + pRightText->GetTextWidth() + 20 > iFoundWidth)
						iFoundWidth = pLeftText->GetTextWidth() + pRightText->GetTextWidth() + 25;

					//Render Texts
					if (iItemInfoBoxWidth != -1)
					{
						pLeftText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
						pRightText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
					}
				}
			}



			/*if (iItemInfoBoxWidth != -1 && iSpecLine > 0)
			{
				switch (rarity)
				{
				case EItemRarity::NONE:
					UI::ImageRender::Render(pImageBorderNoneDivider, iX + (iItemInfoBoxWidth - pImageBorderNoneDivider->GetWidth()) / 2, iY + (iSpecLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::COMMON:
					UI::ImageRender::Render(pImageBorderCommonDivider, iX + (iItemInfoBoxWidth - pImageBorderCommonDivider->GetWidth()) / 2, iY + (iSpecLine * 14) + (pImageBorderCommonDivider->GetHeight() >> 1), pImageBorderCommonDivider->GetWidth(), pImageBorderCommonDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::UNCOMMON:
					UI::ImageRender::Render(pImageBorderUncommonDivider, iX + (iItemInfoBoxWidth - pImageBorderUncommonDivider->GetWidth()) / 2, iY + (iSpecLine * 14) + (pImageBorderUncommonDivider->GetHeight() >> 1), pImageBorderUncommonDivider->GetWidth(), pImageBorderUncommonDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::RARE:
					UI::ImageRender::Render(pImageBorderRareDivider, iX + (iItemInfoBoxWidth - pImageBorderRareDivider->GetWidth()) / 2, iY + (iSpecLine * 14) + (pImageBorderRareDivider->GetHeight() >> 1), pImageBorderRareDivider->GetWidth(), pImageBorderRareDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::EPIC:
					UI::ImageRender::Render(pImageBorderEpicDivider, iX + (iItemInfoBoxWidth - pImageBorderEpicDivider->GetWidth()) / 2, iY + (iSpecLine * 14) + (pImageBorderEpicDivider->GetHeight() >> 1), pImageBorderEpicDivider->GetWidth(), pImageBorderEpicDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::LEGENDARY:
					UI::ImageRender::Render(pImageBorderLegendaryDivider, iX + (iItemInfoBoxWidth - pImageBorderLegendaryDivider->GetWidth()) / 2, iY + (iSpecLine * 14) + (pImageBorderLegendaryDivider->GetHeight() >> 1), pImageBorderLegendaryDivider->GetWidth(), pImageBorderLegendaryDivider->GetHeight(), 0, 0, -1);
					break;
				}
			}

#ifdef ITEMFIXES
			if (iItemInfoBoxWidth != -1 && iFixLine > 0)
			{
				switch (rarity)
				{
				case EItemRarity::NONE:
					UI::ImageRender::Render(pImageBorderNoneDivider, iX + (iItemInfoBoxWidth - pImageBorderNoneDivider->GetWidth()) / 2, iY + (iFixLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::COMMON:
					UI::ImageRender::Render(pImageBorderCommonDivider, iX + (iItemInfoBoxWidth - pImageBorderCommonDivider->GetWidth()) / 2, iY + (iFixLine * 14) + (pImageBorderCommonDivider->GetHeight() >> 1), pImageBorderCommonDivider->GetWidth(), pImageBorderCommonDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::UNCOMMON:
					UI::ImageRender::Render(pImageBorderUncommonDivider, iX + (iItemInfoBoxWidth - pImageBorderUncommonDivider->GetWidth()) / 2, iY + (iFixLine * 14) + (pImageBorderUncommonDivider->GetHeight() >> 1), pImageBorderUncommonDivider->GetWidth(), pImageBorderUncommonDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::RARE:
					UI::ImageRender::Render(pImageBorderRareDivider, iX + (iItemInfoBoxWidth - pImageBorderRareDivider->GetWidth()) / 2, iY + (iFixLine * 14) + (pImageBorderRareDivider->GetHeight() >> 1), pImageBorderRareDivider->GetWidth(), pImageBorderRareDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::EPIC:
					UI::ImageRender::Render(pImageBorderEpicDivider, iX + (iItemInfoBoxWidth - pImageBorderEpicDivider->GetWidth()) / 2, iY + (iFixLine * 14) + (pImageBorderEpicDivider->GetHeight() >> 1), pImageBorderEpicDivider->GetWidth(), pImageBorderEpicDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::LEGENDARY:
					UI::ImageRender::Render(pImageBorderLegendaryDivider, iX + (iItemInfoBoxWidth - pImageBorderLegendaryDivider->GetWidth()) / 2, iY + (iFixLine * 14) + (pImageBorderLegendaryDivider->GetHeight() >> 1), pImageBorderLegendaryDivider->GetWidth(), pImageBorderLegendaryDivider->GetHeight(), 0, 0, -1);
					break;
				}
			}
#endif

			if (iItemInfoBoxWidth != -1 && iStatLine > 0)
			{
				switch (rarity)
				{
				case EItemRarity::NONE:
					UI::ImageRender::Render(pImageBorderNoneDivider, iX + (iItemInfoBoxWidth - pImageBorderNoneDivider->GetWidth()) / 2, iY + (iStatLine * 14) + (pImageBorderNoneDivider->GetHeight() >> 1), pImageBorderNoneDivider->GetWidth(), pImageBorderNoneDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::COMMON:
					UI::ImageRender::Render(pImageBorderCommonDivider, iX + (iItemInfoBoxWidth - pImageBorderCommonDivider->GetWidth()) / 2, iY + (iStatLine * 14) + (pImageBorderCommonDivider->GetHeight() >> 1), pImageBorderCommonDivider->GetWidth(), pImageBorderCommonDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::UNCOMMON:
					UI::ImageRender::Render(pImageBorderUncommonDivider, iX + (iItemInfoBoxWidth - pImageBorderUncommonDivider->GetWidth()) / 2, iY + (iStatLine * 14) + (pImageBorderUncommonDivider->GetHeight() >> 1), pImageBorderUncommonDivider->GetWidth(), pImageBorderUncommonDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::RARE:
					UI::ImageRender::Render(pImageBorderRareDivider, iX + (iItemInfoBoxWidth - pImageBorderRareDivider->GetWidth()) / 2, iY + (iStatLine * 14) + (pImageBorderRareDivider->GetHeight() >> 1), pImageBorderRareDivider->GetWidth(), pImageBorderRareDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::EPIC:
					UI::ImageRender::Render(pImageBorderEpicDivider, iX + (iItemInfoBoxWidth - pImageBorderEpicDivider->GetWidth()) / 2, iY + (iStatLine * 14) + (pImageBorderEpicDivider->GetHeight() >> 1), pImageBorderEpicDivider->GetWidth(), pImageBorderEpicDivider->GetHeight(), 0, 0, -1);
					break;
				case EItemRarity::LEGENDARY:
					UI::ImageRender::Render(pImageBorderLegendaryDivider, iX + (iItemInfoBoxWidth - pImageBorderLegendaryDivider->GetWidth()) / 2, iY + (iStatLine * 14) + (pImageBorderLegendaryDivider->GetHeight() >> 1), pImageBorderLegendaryDivider->GetWidth(), pImageBorderLegendaryDivider->GetHeight(), 0, 0, -1);
					break;
				}
			}*/

			////Render Socket System
			//if( iSocketHeightExtra && iItemInfoBoxWidth != -1 )
			//{
			//	iItemInfoBoxHeight -= iSocketHeightExtra + 20;

			//	GRAPHICENGINE->DrawRectangle2D( Rectangle2D( iX + 15, iY + iItemInfoBoxHeight - 30, 150, 1 ), D3DCOLOR_ARGB( 255, 160, 130, 105 ) );

			//	for( int i = 0; i < iSocketCount; i++ )
			//	{
			//		//Render Socket Info
			//		paSocketText[i][0]->SetNoClip( TRUE );
			//		paSocketText[i][0]->SetBox( Rectangle2D( 56, iItemInfoBoxHeight - 22 + ( i * 38 ), 120, 18 ) );
			//		paSocketText[i][0]->SetHorizontalAlign( ALIGN_Left );
			//		paSocketText[i][0]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

			//		paSocketText[i][1]->SetNoClip( TRUE );
			//		paSocketText[i][1]->SetBox( Rectangle2D( 56, iItemInfoBoxHeight - 8 + ( i * 38 ), 120, 18 ) );
			//		paSocketText[i][1]->SetHorizontalAlign( ALIGN_Left );
			//		paSocketText[i][1]->Render( iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0 );

			//		//Render Socket Background
			//		if( pImageSocketBackground )
			//			UI::ImageRender::Render( pImageSocketBackground, iX + 17, iY + iItemInfoBoxHeight - 18 + ( i * 38 ), 27, 27, -1 );

			//		//Render Socket Type
			//		for( int k = 1; k < SOCKETTYPE_Count; k++ )
			//		{
			//			if( cItemWrapper.sItem.sSocketData[i].eSocketType == (ESocketType)k )
			//			{
			//				UI::ImageRender::Render( paSocket[k-1], iX + 16, iY + iItemInfoBoxHeight - 19 + ( i * 38 ), 29, 29, -1 );
			//				break;
			//			}
			//		}

			//		//Render Socket Gem
			//		if( cItemWrapper.sItem.sSocketData[i].eGemItem != GEMITEM_None && paSocketGems[iSocketGem[i]] )
			//			UI::ImageRender::Render( paSocketGems[iSocketGem[i]], iX + 19, iY + iItemInfoBoxHeight - 15 + ( i * 38 ), 22, 22, -1 );
			//	}
			//}

			//iItemInfoBoxWidth = iFoundWidth < 184 ? 184 : iFoundWidth + 22;

			//Render Compare Text
			if ((bIsEquippedItem || bIsPerfectItem) && bCompareItem)
			{
				int padding = 20;
				int spacing = 10;

				//if (cItemCompare.sItem.sItemID.ToItemBase() == ITEMBASE_Weapon)
				//	padding = 40;

				//

				int iMaxWidth = 0;

				for (UINT i = 0; i < vRowCompare.size(); i++)
				{
					Text_ptr pLeftText = vLeftText[vRowCompare[i] - 1];
					Text_ptr pRightText = vRightText[vRowCompare[i] - 1];
					Text_ptr pCompareText = vCompareText[i];

					//Get Widest text line
					if ((pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + spacing + pCompareText->GetTextWidth() + padding * 2) > iMaxWidth)
						iMaxWidth = pLeftText->GetTextWidth() + 5 + pRightText->GetTextWidth() + spacing + pCompareText->GetTextWidth() + padding * 2;
				}

				if (iMaxWidth > iItemInfoBoxWidth)
				{
					iItemInfoBoxWidth = iMaxWidth;
				}

				for (UINT i = 0; i < vRowCompare.size(); i++)
				{
					Text_ptr pLeftText = vLeftText[vRowCompare[i] - 1];
					Text_ptr pRightText = vRightText[vRowCompare[i] - 1];
					Text_ptr pCompareText = vCompareText[i];

					//Load Texts
					pCompareText->SetNoClip(TRUE);
					pCompareText->SetHorizontalAlign(ALIGN_Right);

					pCompareText->SetBox(Rectangle2D(padding, 5 + (14 * (vRowCompare[i] - 1)), iItemInfoBoxWidth - padding * 2, iHeight));

					//Render Texts
					pCompareText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
				}

				//iItemInfoBoxWidth += padding;
			}

			//Mixing View
			//if (iItemInfoBoxWidth != -1 &&
			//	cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_CRAFT)
			//{
			//	auto it = vMixSheltoms.find(cItemWrapper.sItemInfo.iMixType);

			//	if (it != vMixSheltoms.end())
			//	{
			//		int iYExtra = 59;

			//		int iSheltomWidth = 22;
			//		int iSheltomHeight = 22;

			//		std::vector<int> sheltoms = it->second;

			//		int iSheltomTypes = 0;
			//		for (unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++)
			//			if (sheltoms[iSheltomIndex] > 0)
			//				iSheltomTypes++;

			//		int iTotalWidth = iSheltomTypes * iSheltomWidth;
			//		int iInitialOffset = (iItemInfoBoxWidth - iTotalWidth) / 2;

			//		int iOffsetX = 0;
			//		int iUniqueSheltomCount = 0;

			//		for (unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++)
			//		{
			//			if (sheltoms[iSheltomIndex] > 0)
			//			{
			//				UI::ImageRender::Render(vSheltomImages[iSheltomIndex], iX + iInitialOffset + iOffsetX, iY + 5 + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight, 0, 0, -1);

			//				//6 - there is only 6 sheltom count text
			//				paSheltomCountText[iUniqueSheltomCount]->SetText(FormatString("%d", sheltoms[iSheltomIndex]));
			//				paSheltomCountText[iUniqueSheltomCount]->SetNoClip(TRUE);
			//				paSheltomCountText[iUniqueSheltomCount]->SetBox(Rectangle2D(iInitialOffset + iOffsetX, iSheltomWidth + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight));
			//				paSheltomCountText[iUniqueSheltomCount]->SetHorizontalAlign(ALIGN_Center);
			//				paSheltomCountText[iUniqueSheltomCount]->SetVerticalAlign(ALIGN_Center);
			//				paSheltomCountText[iUniqueSheltomCount]->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);

			//				iOffsetX += iSheltomWidth;
			//				iUniqueSheltomCount++;
			//			}
			//		}
			//	}
			//}

			//Render Aging Bar
			if (iItemInfoBoxWidth != -1)
			{
				if (cItemWrapper.sItemInfo.ItemKindCode == ITEM_KIND_AGING)
				{
					int iYExtra = 30;

					if (bHasLargeName)
					{
						iYExtra += 28;
					}

					if (cItemWrapper.sItemInfo.ItemAgingCount[1])
					{
						int iBarLenght = (int)((float)((float)125 * (((float)cItemWrapper.sItemInfo.ItemAgingCount[0] / (float)cItemWrapper.sItemInfo.ItemAgingCount[1]))));

						if (pImageAgingBar)
							UI::ImageRender::Render(pImageAgingBar, iX + (iItemInfoBoxWidth - pImageAgingBar->GetWidth()) / 2, iY + iSheltomYPosExtra - 20 + iYExtra, 130, 19, 0, 0, -1);

						if (pImageAgingBarGage)
							UI::ImageRender::Render(pImageAgingBarGage, iX + (iItemInfoBoxWidth - pImageAgingBarGage->GetWidth()) / 2, iY + iSheltomYPosExtra - 13 + iYExtra, iBarLenght, 4, 0, 0, -1);
					}

					if (pImageAgingBarNum)
						UI::ImageRender::Render(pImageAgingBarNum, iX + (iItemInfoBoxWidth - pImageAgingBarNum->GetWidth()) / 2, iY + iSheltomYPosExtra - 20 + iYExtra, 26, 19, 0, 0, -1);

					//Render Aging Text
					if (pTextAging)
					{
						pTextAging->SetNoClip(TRUE);
						pTextAging->SetText(FormatString("+%d", cItemWrapper.sItemInfo.ItemAgingNum[0]));
						pTextAging->SetBox(Rectangle2D((iItemInfoBoxWidth - 24) / 2, iSheltomYPosExtra - 20 + iYExtra, 24, 18));
						pTextAging->SetHorizontalAlign(ALIGN_Center);
						pTextAging->SetVerticalAlign(ALIGN_Center);
						pTextAging->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
					}

					int iSheltomWidth = 22;
					int iSheltomHeight = 22;

					const int ageLevel = cItemWrapper.sItemInfo.ItemAgingCount[0] ? cItemWrapper.sItemInfo.ItemAgingNum[0] + 1 : cItemWrapper.sItemInfo.ItemAgingNum[0];

					//Render Sheltoms used for aging
					//auto it = vAgeSheltoms.find(ageLevel);
					//if (it != vAgeSheltoms.end())
					//{
					//	std::vector<int> sheltoms = it->second;

					//	int iSheltomTypes = 0;
					//	for (unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++)
					//		if (sheltoms[iSheltomIndex] > 0)
					//			iSheltomTypes++;

					//	int iTotalWidth = iSheltomTypes * iSheltomWidth;
					//	int iInitialOffset = (iItemInfoBoxWidth - iTotalWidth) / 2;

					//	int iOffsetX = 0;
					//	int iUniqueSheltomCount = 0;

					//	for (unsigned int iSheltomIndex = 0; iSheltomIndex < sheltoms.size(); iSheltomIndex++)
					//	{
					//		if (sheltoms[iSheltomIndex] > 0)
					//		{
					//			UI::ImageRender::Render(vSheltomImages[iSheltomIndex], iX + iInitialOffset + iOffsetX, iY + 5 + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight, 0, 0, -1);

					//			//7 - there is only 7 sheltom count text
					//			paSheltomCountText[iUniqueSheltomCount]->SetText(FormatString("%d", sheltoms[iSheltomIndex]));
					//			paSheltomCountText[iUniqueSheltomCount]->SetNoClip(TRUE);
					//			paSheltomCountText[iUniqueSheltomCount]->SetBox(Rectangle2D(iInitialOffset + iOffsetX, iSheltomWidth + iSheltomYPosExtra + iYExtra, iSheltomWidth, iSheltomHeight));
					//			paSheltomCountText[iUniqueSheltomCount]->SetHorizontalAlign(ALIGN_Center);
					//			paSheltomCountText[iUniqueSheltomCount]->SetVerticalAlign(ALIGN_Center);
					//			paSheltomCountText[iUniqueSheltomCount]->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);

					//			iOffsetX += iSheltomWidth;
					//			iUniqueSheltomCount++;
					//		}
					//	}
					//}

				}
			}

		}
	}

	void ItemInfoBox::Update(float fTime)
	{
		//if( bIsPerfectItem )
		//{
		//	if( bFadeToColor )
		//	{
		//		fProgress += 0.01f;

		//		if( fProgress > 1.0f )
		//		{
		//			bFadeToColor = FALSE;
		//			fProgress = 1.0f;
		//		}
		//	}
		//	else if( !bFadeToColor )
		//	{
		//		fProgress -= 0.005f;

		//		if( fProgress < 0.0f )
		//		{
		//			bFadeToColor = TRUE;
		//			fProgress = 0.0f;
		//		}
		//	}

		//	cBackgroundColor2 = UI::Color( 245, 9 + (int)(15.0f * fProgress), 23 + (int)(19.0f * fProgress), 22 + (int)(34.0f * fProgress) );
		//}
	}

	void ItemInfoBox::DrawItemInfoBox(int iX, int iY, int iWidth, int iHeight)
	{
		//Draw Item Info Box Background
		if (pRenderTargetMask)
		{
			if (GRAPHICENGINE->GetGraphicsDevice()->SetRenderTarget(pRenderTargetMask))
			{
				//Clear Render Target
				GRAPHICENGINE->GetGraphicsDevice()->Clear(TRUE, TRUE, FALSE);

				//Draw Background
				GRAPHICENGINE->DrawRectangle2D(Rectangle2D(8, 8, iWidth - 16, iHeight - 16), cBackgroundColor1.Get(), cBackgroundColor2.Get());

				//Drawing Mask Border
				UI::ImageRender::Render(pImageMaskLeftTop, 0, 0, pImageMaskLeftTop->GetWidth(), pImageMaskLeftTop->GetHeight(), -1);
				UI::ImageRender::Render(pImageMaskRightTop, iWidth - pImageMaskRightTop->GetWidth(), 0, pImageMaskRightTop->GetWidth(), pImageMaskRightTop->GetHeight(), -1);
				UI::ImageRender::Render(pImageMaskLeftBottom, 0, iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskLeftBottom->GetWidth(), pImageMaskLeftBottom->GetHeight(), -1);
				UI::ImageRender::Render(pImageMaskRightBottom, iWidth - pImageMaskRightTop->GetWidth(), iHeight - pImageMaskLeftBottom->GetHeight(), pImageMaskRightBottom->GetWidth(), pImageMaskRightBottom->GetHeight(), -1);

				//Back Old Render Target
				GRAPHICENGINE->GetGraphicsDevice()->UnsetRenderTarget();
			}

			//Set Sprite Effect...
			UI::ImageRender::SetEffect(pEffectMask);
			pEffectMask->SetTechnique("Mask");

			if (pEffectMask->Begin() >= 0)
			{
				//Render the Render Target and mask it on effect
				UI::ImageRender::Render(pRenderTargetMask->GetTexture(), iX, iY - 10, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB(255, 255, 255), FALSE);

				pEffectMask->End();
			}
			UI::ImageRender::SetEffect(NULL);
		}
	}
}