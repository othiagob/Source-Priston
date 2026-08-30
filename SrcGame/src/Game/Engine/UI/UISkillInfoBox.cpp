#include "UISkillInfoBox.h"
#include "sinbaram\\sinlinkheader.h"
#include <Skill/SkillManager.h>

int iaUpgradeSkillCost[20];
int iaUpgradeSkillCostPlus[20];
extern int sinMasterSkill_Money[16];
extern int sinMasterSkill_PlusMoney[16];
extern char szSkillMasterInfoBuff[5000];
extern char szSkillMasterInfoBuff2[5000];
extern int SkillMain2OpenFlag;
extern int SkillMasteryShowFlag;

#define SKILLINFOBOXWIDTH 264

namespace UI
{
	SkillInfoBox::SkillInfoBox()
	{
		iSkillInfoOffset = 0;
		iSkillInfoBoxHeight = 0;
	}

	SkillInfoBox::~SkillInfoBox()
	{
	}


	BOOL SkillInfoBox::IsDifferentSkill(sSKILL* psSkill)
	{
		if (cSkillWrapper.Skill_Info.CODE == psSkill->Skill_Info.CODE)
			return FALSE;

		return TRUE;
	}


	BOOL SkillInfoBox::CanPrepareShowSkill(sSKILL* psSkill, Rectangle2D sBox)
	{
		BOOL bRet = TRUE;

		if (cSkillWrapper.Skill_Info.CODE == psSkill->Skill_Info.CODE)
			bRet = FALSE;

		if (bInvalidate)
		{
			bRet = TRUE;
			bInvalidate = FALSE;
		}

		if (!bRet)
		{
			if (cSkillWrapper.Point != psSkill->Point)
				bRet = TRUE;

			if (!sSkillBox.Equals(&sBox))
				bRet = TRUE;
		}

		return bRet;
	}

	void SkillInfoBox::AddSkillElement(int eElementID)
	{
		switch (eElementID)
		{
		case sITEMINFO_BIONIC:
		case sITEMINFO_FIRE:
		case sITEMINFO_LIGHTING:
		case sITEMINFO_ICE:
		case sITEMINFO_POISON:
			break;
		default:
			return; //invalid
		}

		vSkillElements.push_back(eElementID);
	}

	UI::Text_ptr SkillInfoBox::AddString(int iSide, const std::string& strString, DWORD dwColor, BOOL bBold, EAlign eAlign)
	{
		switch (iSide)
		{
		case 0: //Left Side
			//Is Multiline?
			if (strString == "\n")
			{
				size_t n = std::count(strString.begin(), strString.end(), '\n');
				iSkillInfoBoxHeight += 14 * n;
			}
			else if (strString.find('\n') != std::string::npos)
			{
				UI::Text_ptr pText = std::make_shared<Text>(strString, Rectangle2D(14, iSkillInfoBoxHeight, SKILLINFOBOXWIDTH + iExtraWidth - 20, 0), bBold, dwColor);
				pText->SetText(trim((std::string)strString));
				pText->SetMultiLine(TRUE);
				pText->SetWordWrap(TRUE);
				pText->SetHorizontalAlign(eAlign);
				pText->SetValue(ieValue);
				iSkillInfoBoxHeight += pText->GetHeightText();
				vLeftText.push_back(pText);

				return pText;
			}
			else
			{
				UI::Text_ptr pText = std::make_shared<Text>(strString, Rectangle2D(14, iSkillInfoBoxHeight, SKILLINFOBOXWIDTH + iExtraWidth - 20, 0), bBold, dwColor);
				pText->SetText(trim((std::string)strString));
				pText->SetMultiLine(TRUE);
				pText->SetWordWrap(TRUE);
				pText->SetHorizontalAlign(eAlign);
				pText->SetValue(ieValue);
				iSkillInfoBoxHeight += pText->GetHeightText();
				vLeftText.push_back(pText);

				return pText;
			}
			break;
		case 1: //Right Side
		{
			UI::Text_ptr pText = std::make_shared<Text>(strString, Rectangle2D(14 + vLeftText[vLeftText.size() - 1]->GetTextWidth() + 6, iSkillInfoBoxHeight - vLeftText[vLeftText.size() - 1]->GetHeightText(), SKILLINFOBOXWIDTH + iExtraWidth - 20, 20), bBold, dwColor);
			pText->SetText(trim((std::string)strString));
			pText->SetMultiLine(TRUE);
			pText->SetWordWrap(TRUE);
			pText->SetValue(ieValue);
			pText->SetHorizontalAlign(eAlign);
			vRightText.push_back(pText);

			return pText;

			break;
		}
		case 2:
		{
			UINT uIndex = uSkillStatusPos + vCompareText.size();

			if ((uIndex < vLeftText.size()) && (uIndex < vRightText.size()))
			{
				if (eAlign == EAlign::ALIGN_Right)
				{
					UI::Text_ptr pText = std::make_shared<Text>(strString, Rectangle2D(14, vLeftText[uIndex]->GetY(), SKILLINFOBOXWIDTH + iExtraWidth - 20, 0), bBold, dwColor);
					pText->SetText(trim((std::string)strString));
					pText->SetMultiLine(TRUE);
					pText->SetWordWrap(TRUE);
					pText->SetValue(ieValue);
					pText->SetHorizontalAlign(EAlign::ALIGN_Right);
					vCompareText.push_back(pText);

					return pText;
				}
				else
				{
					UI::Text_ptr pText = std::make_shared<Text>(strString, Rectangle2D(vLeftText[uIndex]->GetTextWidth() + vRightText[uIndex]->GetTextWidth() + 24, vLeftText[uIndex]->GetY(), SKILLINFOBOXWIDTH + iExtraWidth - 20, 0), bBold, dwColor);
					pText->SetText(trim((std::string)strString));
					pText->SetMultiLine(TRUE);
					pText->SetWordWrap(TRUE);
					pText->SetValue(ieValue);
					pText->SetHorizontalAlign(EAlign::ALIGN_Left);
					vCompareText.push_back(pText);

					return pText;
				}


			}
			break;
		}
		}

		return nullptr;
	}

	void SkillInfoBox::SetLineColor(DWORD dwColor, int iSide, int iLineCount)
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

	void SkillInfoBox::Init()
	{
		//Load Images
		pImageMaskLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\topleft.png");
		pImageMaskRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\topright.png");
		pImageMaskLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\bottomleft.png");
		pImageMaskRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\mask\\bottomright.png");

		//Load Border Images
		pImageBorderLeftTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\topleft.png");
		pImageBorderRightTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\topright.png");
		pImageBorderTop = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\top.png");
		pImageBorderRightBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\bottomright.png");
		pImageBorderLeftBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\bottomleft.png");
		pImageBorderBottom = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\bottom.png");
		pImageBorderLeft = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\left.png");
		pImageBorderRight = ImageLoader::LoadImage_("game\\images\\UI\\box\\border\\right.png");

		pRenderTargetMask = GRAPHICENGINE->GetGraphicsDevice()->MakeRenderTarget(5001, 600 + iExtraWidth, 600);
		pEffectMask = GRAPHICENGINE->GetGraphicsDevice()->GetEffectFactory()->MakeEffect("game\\scripts\\shaders\\InfoBox.fx");

		for (int i = 0; i < 12; i++)
		{
			pImageIconItems[i] = ImageLoader::LoadImage_("image\\Sinimage\\skill\\WeaponIcon\\%d.bmp", i + 1);
		}

		pImageSkillElements[sITEMINFO_BIONIC] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\element\\Organic.png");
		pImageSkillElements[sITEMINFO_FIRE] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\element\\Fire.png");
		pImageSkillElements[sITEMINFO_ICE] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\element\\Ice.png");
		pImageSkillElements[sITEMINFO_LIGHTING] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\element\\Lighting.png");
		pImageSkillElements[sITEMINFO_POISON] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\element\\Poison.png");
	}

	BOOL SkillInfoBox::OnKeyPress(class CKeyboard* pcKeyboard)
	{
		if (CSKILL->OpenFlag)
		{
			if (SkillInfoShowFlag && SkillButtonIndex)
			{
				if (pcKeyboard->GetKey() == VK_RIGHT || pcKeyboard->GetKey() == VK_LEFT)
				{
					sSKILL* sCurrentSkill = &sinSkill.UseSkill[SkillButtonIndex - 1];

					if (pcKeyboard->GetEvent() == EKeyboardEvent::KeyUp)
					{
						if (pcKeyboard->GetKey() == VK_LEFT)
						{
							if ((sCurrentSkill->Point + iSkillInfoOffset) >= 2)
							{
								iSkillInfoOffset--;
							}
						}
						else
						{
							if ((sCurrentSkill->Point + iSkillInfoOffset) <= 9)
							{
								iSkillInfoOffset++;
							}
						}
					}

					return TRUE;
				}
			}
		}

		return FALSE;
	}

	void SkillInfoBox::FormatSkillInfo()
	{
		BOOL bUpdateSkill = FALSE;

		DWORD iSkillID = 0;

		if ((cSkillWrapper.CODE & sinITEM_MASK2) == GROUP_OTHERSKILL)
		{
			iSkillID = cSkillWrapper.Skill_Info.CODE;
		}
		else
		{
			iSkillID = cSkillWrapper.CODE;
		}

		uSkillStatusPos = vLeftText.size();
		UINT uSkillLevelZeroBased = cSkillWrapper.Point - 1;

		if (cSkillWrapper.Point == 0)
			uSkillLevelZeroBased = 0;

		if (SKILLMANAGERHANDLER->AddSkillInfoToBox(iSkillID, uSkillLevelZeroBased))
		{
			bUpdateSkill = TRUE;
		}

		//Upgrade Cost
		if (cSkillWrapper.Point < 10 && CSKILL->OpenFlag > 0)
		{
			//CopyMemory(iaUpgradeSkillCost, (char*)0x9BBE58, sizeof(int) * 20);
			//CopyMemory(iaUpgradeSkillCostPlus, (char*)0x9BBEA8, sizeof(int) * 20);

			strSkillNextLevelCost = FormatString("%s gp", FormatNumber(cShop.haShopItemPrice(sinMasterSkill_Money[cSkillWrapper.Skill_Info.SkillNum] + (sinMasterSkill_PlusMoney[cSkillWrapper.Skill_Info.SkillNum] * (cSkillWrapper.Point)))));
		}


		{
			std::string strBufferLeft = szSkillMasterInfoBuff;//(char*)0x036762A0;
			std::string strBufferRight = szSkillMasterInfoBuff2;// (char*)0x03677628;

			std::vector<std::string> vStringLeft;
			vStringLeft = split(strBufferLeft, '\r');

			std::vector<std::string> vStringRight;
			vStringRight = split(strBufferRight, '\r');

			if (bUpdateSkill == FALSE)
			{
				for (size_t i = 0; i < vStringLeft.size(); i++)
				{
					if (vStringLeft[i].empty())
					{
						vStringLeft[i].erase();
						vStringRight[i].erase();
					}
				}

				bool bNextLevel = false;
				for (size_t i = 0; i < vStringLeft.size(); i++)
				{
					if (i == 0)
						uSkillStatusPos = vLeftText.size() + 1;

					if (vStringLeft[i].compare("Skill Metadata") == 0)
						bNextLevel = true;

					if (!bNextLevel)
					{
						AddString(0, vStringLeft[i], D3DCOLOR_XRGB(243, 237, 216));
						AddString(1, vStringRight[i], D3DCOLOR_XRGB(238, 210, 151));
					}
					else
					{
						if (!vStringRight[i].empty())
						{
							if (vStringRight[i].find("gp") != std::string::npos)
								strSkillNextLevelCost = vStringRight[i];
							else
								AddString(2, FormatString("(%s)", vStringRight[i].c_str()), D3DCOLOR_XRGB(92, 163, 99));
						}
					}
				}
			}
		}
	}




	BOOL SkillInfoBox::PrepareShowSkill(sSKILL* pcSkill, BOOL bSkillTimer, Rectangle2D sBox)
	{
		if (ShowSkillUpInfo)
			return TRUE;

		bInfoBoxSkillTimer = bSkillTimer;

		//Save Box Position
		sSkillBox = sBox;

		vSkillElements.clear();

		//Clear Vectors
		vLeftText.clear();
		vRightText.clear();
		vCompareText.clear();
		strSkillNextLevelCost.clear();

		uSkillStatusPos = 0;
		iSkillInfoBoxHeight = 20;

		//Copy pcSkill to cSkillWrapper
		CopyMemory(&cSkillWrapper, pcSkill, sizeof(sSKILL));

		DWORD iSkillID = 0;

		if ((pcSkill->CODE & 0xFFFF0000) == GROUP_OTHERSKILL)
		{
			iSkillID = pcSkill->Skill_Info.CODE;
		}
		else
		{
			iSkillID = pcSkill->CODE;
		}

		if (iSkillInfoOffset != 0 && SkillButtonIndex != 0)
		{
			AddString(0, "PRÉ - VISUALIZAÇÃO", D3DCOLOR_XRGB(176, 171, 19), TRUE);
			AddString(1, "");
			AddString(0, "");
			AddString(1, "");
		}

		//Name Skill
		if (iSkillID == SKILL_NORMAL_ATTACK)
		{
			AddString(0, "Ataque Padrão", D3DCOLOR_XRGB(243, 237, 216), TRUE);
			AddString(1, "");
		}
		else
		{
			AddString(0, cSkillWrapper.Skill_Info.SkillName, D3DCOLOR_XRGB(243, 237, 216), TRUE);
			AddString(1, "");
		}

		//Level Skill
		if (cSkillWrapper.Point > 0 && iSkillID != SKILL_FORCE_ORB && iSkillID != CHANGE_JOB1 && iSkillID != CHANGE_JOB2 &&
			iSkillID != CHANGE_JOB3 && iSkillID != CHANGE_JOB4 && iSkillID != SCROLL_INVULNERABILITY && iSkillID != SCROLL_CRITICAL &&
			iSkillID != SCROLL_EVASION && iSkillID != STONE_R_FIRECRYTAL &&
			iSkillID != STONE_R_ICECRYTAL && iSkillID != SKILL_NORMAL_ATTACK && iSkillID != CLANSKILL_ABSORB && iSkillID != CLANSKILL_ATTACK &&
			iSkillID != CLANSKILL_EVASION && iSkillID != STONE_R_LINGHTINGCRYTAL && iSkillID != STONE_A_FIGHTER
			&& iSkillID != STONE_A_MECHANICIAN
			&& iSkillID != STONE_A_PIKEMAN
			&& iSkillID != STONE_A_ARCHER
			&& iSkillID != STONE_A_KNIGHT
			&& iSkillID != STONE_A_ATALANTA
			&& iSkillID != STONE_A_MAGICIAN
			&& iSkillID != STONE_A_PRIESTESS
			&& iSkillID != SCROLL_P_INVULNERABILITY
			&& iSkillID != SCROLL_P_CRITICAL
			&& iSkillID != SCROLL_P_EVASION
			&& iSkillID != BOOSTER_ITEM_LIFE
			&& iSkillID != BOOSTER_ITEM_MANA
			&& iSkillID != BOOSTER_ITEM_STAMINA
			&& iSkillID != SODSKILL_SILVER
			&& iSkillID != SODSKILL_BRONZE
			&& iSkillID != CHATGLOBAL
			&& iSkillID != TOPLVL
			&& iSkillID != BUFF_WARMODE
			&& iSkillID != REIPVP)
		{
			AddString(0, "Rank");
			AddString(1, FormatString("%d", cSkillWrapper.Point));
			SetLineColor(D3DCOLOR_XRGB(243, 237, 216));
		}

		//Required Level (only need to show in preview mode)
		if ((cSkillWrapper.Point >= 0 && cSkillWrapper.Point <= 10) && !bInfoBoxSkillTimer && iSkillID != SKILL_NORMAL_ATTACK)
		{
			int iRequiredLevel = 0;

			//preview mode
			//if ( iSkillInfoOffset >= 1 )
			iRequiredLevel = (cSkillWrapper.Skill_Info.RequireLevel + ((cSkillWrapper.Point - 1) * 2));
			//else
			//	iRequiredLevel = (cSkillWrapper.Skill_Info.iSkillLevelRequired + (cSkillWrapper.iLevel -1 ) * 2));

			AddString(0, "Nível Requerido:");
			AddString(1, FormatString("%d", iRequiredLevel));
			SetLineColor(D3DCOLOR_XRGB(255, 185, 89));
		}

		//Next Level
		if (cSkillWrapper.Point < 10 && cSkillWrapper.Point > 0 && !bInfoBoxSkillTimer && iSkillID != SKILL_NORMAL_ATTACK)
		{
			int iRequiredLevel = 0;

			iRequiredLevel = (cSkillWrapper.Skill_Info.RequireLevel + ((cSkillWrapper.Point + 0) * 2));

			AddString(0, "Próximo Nível Requerido:");
			AddString(1, FormatString("%d", iRequiredLevel));
			SetLineColor(D3DCOLOR_XRGB(92, 163, 99));
		}


		//Type Skill
		if (iSkillID != SKILL_FORCE_ORB &&
			iSkillID != CHANGE_JOB1 &&
			iSkillID != CHANGE_JOB2 &&
			iSkillID != CHANGE_JOB3 &&
			iSkillID != CHANGE_JOB4)
		{
			AddString(0, "");
			AddString(1, "");
			SetLineColor(D3DCOLOR_XRGB(255, 185, 89));
		}

		AddString(0, "\n");

		//Description Skill
		AddString(0, cSkillWrapper.Skill_Info.SkillDoc);
		AddString(1, "");
		SetLineColor(D3DCOLOR_XRGB(169, 169, 169));

		AddString(0, "\n");

		//Skill Locked
		/*if (SKILLDATAHANDLER->IsSkillLocked(iSkillID))
		{
			AddString(0, "Skill not avaliable!");
			AddString(1, "");
			SetLineColor(D3DCOLOR_XRGB(200, 90, 90));

			AddString(0, "\n");
		}*/

		//Correct Item Group
		if (cSkillWrapper.Skill_Info.UseWeaponCode[0] != 0x00000000)
		{
			uImageIconsItemsY = iSkillInfoBoxHeight - 4;
			AddString(0, "\n");
			AddString(0, "\n");
		}

		BOOL bAdded = FALSE;

		if (SKILLMANAGERHANDLER->AddDamageVsMonsterTypeInfo(&cSkillWrapper)) bAdded = TRUE;
		if (SKILLMANAGERHANDLER->AddSpecificMonsterTypeInfo(&cSkillWrapper)) bAdded = TRUE;
		if (SKILLMANAGERHANDLER->AddSummonInfo(&cSkillWrapper)) bAdded = TRUE;
		if (SKILLMANAGERHANDLER->AddBuffInfo(&cSkillWrapper)) bAdded = TRUE;
		if (SKILLMANAGERHANDLER->AddDebuffInfo(&cSkillWrapper)) bAdded = TRUE;
		if (SKILLMANAGERHANDLER->AddPassiveInfo(&cSkillWrapper)) bAdded = TRUE;
		if (SKILLMANAGERHANDLER->AddHintInfo(&cSkillWrapper)) bAdded = TRUE;

		if (bAdded)
		{
			AddString(0, "\n"); //add a line break
		}

		//Format Skill Info
		FormatSkillInfo();

		if (!bInfoBoxSkillTimer && cSkillWrapper.Skill_Info.USECODE != 0x04000000) // SKILL_USE_NOT
			AddString(0, "\n");

		if (cSkillWrapper.Point > 0 && !bInfoBoxSkillTimer)
		{
			//MP
			if (cSkillWrapper.Skill_Info.UseMana)
			{
				short* piaUseMana = cSkillWrapper.Skill_Info.UseMana;
				short iManaCost = piaUseMana[cSkillWrapper.Point - 1];

				if (iManaCost != 0)
				{
					AddString(0, "Custo de Mana:", D3DCOLOR_XRGB(243, 237, 216));

					//Decrease MP Item? for example MP down
					if (chaPremiumitem.m_ManaReducePotionValue > 0 && chaPremiumitem.m_ManaReducePotiontime > 0)
					{
						int iDecreaseMP = (iManaCost * chaPremiumitem.m_ManaReducePotionValue) / 100;
						AddString(1, FormatString("%d", iManaCost - iDecreaseMP), D3DCOLOR_XRGB(67, 149, 204));
						AddString(2, FormatString(" (-%d)", iDecreaseMP), D3DCOLOR_XRGB(100, 100, 100));
					}
					else
					{
						AddString(1, FormatString("%d", iManaCost), D3DCOLOR_XRGB(234, 186, 81));
						AddString(2, "");
					}
				}
			}

			//STM
			if (cSkillWrapper.Skill_Info.UseStamina[0] != 0)
			{
				AddString(0, "Custo de Stamina:", D3DCOLOR_XRGB(243, 237, 216));

				short iSP = cSkillWrapper.Skill_Info.UseStamina[0] + (cSkillWrapper.Skill_Info.UseStamina[1] * (cSkillWrapper.Point - 1));

				//Decrease STM Item?
				if (chaPremiumitem.m_StaminaReducePotionValue > 0 && chaPremiumitem.m_StaminaReducePotiontime)
				{
					short iDecreaseSP = (iSP * chaPremiumitem.m_StaminaReducePotionValue) / 100;

					AddString(1, FormatString("%d", iSP - iDecreaseSP), D3DCOLOR_XRGB(133, 204, 67));
					AddString(2, FormatString(" (-%d)", iDecreaseSP), D3DCOLOR_XRGB(100, 100, 100));
				}
				else
				{
					AddString(1, FormatString("%d", iSP), D3DCOLOR_XRGB(234, 186, 81));
					AddString(2, "");
				}

				//SetLineColor( D3DCOLOR_XRGB( 234, 186, 81 ), 1 );
			}
		}

		// Desativado -> Entender como o delay é calculado pq sempre da uma diferença de segundos
		//if (!bInfoBoxSkillTimer && (cSkillWrapper.Point > 0) && (cSkillWrapper.Skill_Info.USECODE != 0x04000000) && iSkillID != SKILL_NORMAL_ATTACK)
		//{
		//	int iMastery = cSkillWrapper.Mastery;

		//	int iMasteryBase = cSkillWrapper.Skill_Info.RequireMastery[0];
		//	int iMasteryPerLevel = cSkillWrapper.Skill_Info.RequireMastery[1];

		//	if (iMastery > MAX_MASTERY)
		//		iMastery = MAX_MASTERY;
		//	else if (iMastery < 1)
		//		iMastery = 1;

		//	float fSkillCooldown = ((MAX_MASTERY / (MAX_MASTERY / ((float)iMastery / 2.0f))) * 35) * (1000.0f / 60.0f);
		//	fSkillCooldown /= 1000.0f;

		//	int iFullyTrainedMastery = iMasteryBase + (iMasteryPerLevel * cSkillWrapper.Point) - (10000 / 100);

		//	if (iFullyTrainedMastery > MAX_MASTERY)
		//		iFullyTrainedMastery = MAX_MASTERY;
		//	else if (iFullyTrainedMastery < 1)
		//		iFullyTrainedMastery = 1;

		//	float fSkillCooldownFullyTrained = ((MAX_MASTERY / (MAX_MASTERY / ((float)iFullyTrainedMastery / 2.0f))) * 35) * (1000.0f / 60.0f);
		//	fSkillCooldownFullyTrained /= 1000.0f;

		//	BOOL bIsFullyTrained = fSkillCooldownFullyTrained == fSkillCooldown;

		//	float TrainedPercent = static_cast<float>(cSkillWrapper.UseSkillMastery) / 100.0f;

		//	if (iSkillInfoOffset != 0) // Modo preview
		//	{
		//		AddString(0, "Delay:", D3DCOLOR_XRGB(243, 237, 216));
		//		AddString(1, FormatString("%.2fs ( 100%% Treinado )", fSkillCooldownFullyTrained), D3DCOLOR_XRGB(234, 186, 81));
		//	}
		//	else
		//	{
		//		AddString(0, "Delay:", D3DCOLOR_XRGB(243, 237, 216));

		//		if (bIsFullyTrained)
		//			AddString(1, FormatString("%.2fs", fSkillCooldownFullyTrained), D3DCOLOR_XRGB(234, 186, 81));
		//		else
		//			AddString(1, FormatString("%.2fs ( %.0f%% Treinado )", fSkillCooldown, TrainedPercent), D3DCOLOR_XRGB(234, 186, 81));

		//		if (!bIsFullyTrained)
		//		{
		//			AddString(0, "Delay:", D3DCOLOR_XRGB(112, 111, 107));
		//			AddString(1, FormatString("%.2fs ( em 100%% )", fSkillCooldownFullyTrained), D3DCOLOR_XRGB(153, 123, 58));
		//		}
		//	}
		//}

		//Next Level Cost
		if (cSkillWrapper.Point < 10 && !strSkillNextLevelCost.empty() && cSkillWrapper.CODE < SODSKILL_GOLD) // Evita exibir informações nos buffs de sod, cristal etc
		{
			AddString(0, "Custo de Upgrade:", D3DCOLOR_XRGB(243, 237, 216));
			AddString(1, "0 gp", D3DCOLOR_XRGB(234, 186, 81));
		}
		else if (cSkillWrapper.Point > 0 && !bInfoBoxSkillTimer && iSkillID != SKILL_NORMAL_ATTACK && SkillButtonIndex >= 1 && cSkillWrapper.CODE < SODSKILL_GOLD)
		{
			AddString(0, "", D3DCOLOR_XRGB(100, 100, 100));
			AddString(1, "");
			AddString(0, "Utilize as setas para esquerda ou direita", D3DCOLOR_XRGB(100, 100, 100));
			AddString(1, "");
			AddString(0, "para pré-visualizar as habilidades em outros níveis!", D3DCOLOR_XRGB(100, 100, 100));
			AddString(1, "");
		}

		//Time Left
		if (bInfoBoxSkillTimer && (iSkillID & 0xFFFF0000) != 0x096A0000 && iSkillID != CLANSKILL_ABSORB && iSkillID != CLANSKILL_ATTACK && iSkillID != CLANSKILL_EVASION &&
			(iSkillID != SCROLL_CRITICAL) && (iSkillID != SCROLL_EVASION) && cSkillWrapper.CODE < SODSKILL_GOLD)
		{
			int iTimeLeft = 0;

			if ((cSkillWrapper.CheckTime / 70) > 0)
				iTimeLeft = cSkillWrapper.UseTime - (cSkillWrapper.CheckTime / 70);
			else
				iTimeLeft = (cSkillWrapper.CheckTime / 70) - cSkillWrapper.UseTime;

			int iHour = abs(iTimeLeft / 3600);
			int iMinutes = abs((iTimeLeft) / 60) % 60;
			int iSeconds = abs((iTimeLeft) % 60);

			AddString(0, "\n");
			AddString(0, "Tempo Restante:", D3DCOLOR_XRGB(243, 237, 216));
			AddString(1, FormatString("%02d:%02d:%02d", iHour, iMinutes, iSeconds), D3DCOLOR_XRGB(234, 186, 81));

			if (iSkillID == SKILL_FORCE_ORB &&
				(MAP_ID == MAPID_RicartenTown ||
					MAP_ID == MAPID_PillaiTown ||
					MAP_ID == MAPID_NaviskoTown ||
					MAP_ID == MAPID_Eura ||
					MAP_ID == MAPID_Atlantis))
			{
				AddString(0, "\n");
				AddString(0, "Tempo Pausado", D3DCOLOR_XRGB(255, 238, 0));
				AddString(1, "");
			}
		}

		return TRUE;
	}

	void SkillInfoBox::Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY)
	{
		// Pontos de habilidade disponíveis
		if (CSKILL->OpenFlag)
		{
			char strBuff[128] = { 0 };

			wsprintf(strBuff, "%d", sinSkill.SkillPoint);

			dsTextLineOut(0, 201, RESOLUTION_HEIGHT - 25,
				strBuff, lstrlen(strBuff));

			wsprintf(strBuff, "%d", sinSkill.SkillPoint4);
			dsTextLineOut(0, 245, RESOLUTION_HEIGHT - 25,
				strBuff, lstrlen(strBuff));
		}

		if (!SkillInfoShowFlag && !bInfoBoxSkillTimer)
		{
			iSkillInfoOffset = 0;
			return;
		}

		if (bInfoBoxSkillTimer && !ContinueSkillMouseIndex)
		{
			iSkillInfoOffset = 0;
			return;
		}

		//Set iWidth and iHeight of Box
		int iSkillInfoBoxWidth = SKILLINFOBOXWIDTH + iExtraWidth;

		//Adjust Box Position
		if (!bInfoBoxSkillTimer)
		{
			iX = sSkillBox.iX - (iSkillInfoBoxWidth >> 1) + 20;
			iY = sSkillBox.iY - iSkillInfoBoxHeight;
		}
		else
		{
			iX = sSkillBox.iX;
			iY = sSkillBox.iY;
		}

		//iItemSlot of box is out the window?
		if (iX < 0)
			iX = 0;

		if (iY < 0)
			iY = 16;

		if (iX + iSkillInfoBoxWidth > (int)RESOLUTION_WIDTH)
			iX -= ((iX + iSkillInfoBoxWidth) - RESOLUTION_WIDTH) + 16;

		//Fix position
		if (!bInfoBoxSkillTimer)
			iY += RESOLUTION_HEIGHT - 600;

		//Draw Skill Info Box Background
		DrawSkillInfoBox(iX, iY + 20, iSkillInfoBoxWidth, iSkillInfoBoxHeight);

		int elementIndex = 0;

		for (int element : vSkillElements)
		{
			UI::ImageRender::Render(pImageSkillElements[element], iX + (iSkillInfoBoxWidth - (elementIndex * 18) - 15 - 10), iY + 22, 15, 15, -1);
			elementIndex += 1;
		}

		//Drawing Border
		UI::ImageRender::Render(pImageBorderLeftTop, iX, iY + 10, pImageBorderLeftTop->GetWidth(), pImageBorderLeftTop->GetHeight(), -1);
		UI::ImageRender::Render(pImageBorderTop, iX + pImageBorderLeftTop->GetWidth(), iY + 10, iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderTop->GetHeight(), -1);
		UI::ImageRender::Render(pImageBorderRightTop, iX + pImageBorderLeftTop->GetWidth() + iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), iY + 10, pImageBorderRightTop->GetWidth(), pImageBorderRightTop->GetHeight(), -1);
		UI::ImageRender::Render(pImageBorderLeft, iX, iY + 10 + pImageBorderLeftTop->GetHeight(), pImageBorderLeft->GetWidth(), iSkillInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1);
		UI::ImageRender::Render(pImageBorderRight, iX + iSkillInfoBoxWidth - pImageBorderRight->GetWidth(), iY + 10 + pImageBorderLeftTop->GetHeight(), pImageBorderRight->GetWidth(), iSkillInfoBoxHeight - pImageBorderLeftTop->GetWidth() - pImageBorderLeftBottom->GetWidth(), -1);
		UI::ImageRender::Render(pImageBorderLeftBottom, iX, iY + 10 + iSkillInfoBoxHeight - pImageBorderLeftBottom->GetHeight(), pImageBorderLeftBottom->GetWidth(), pImageBorderLeftBottom->GetHeight(), -1);
		UI::ImageRender::Render(pImageBorderRightBottom, iX + iSkillInfoBoxWidth - pImageBorderRightBottom->GetWidth(), iY + 10 + iSkillInfoBoxHeight - pImageBorderRightBottom->GetHeight(), pImageBorderRightBottom->GetWidth(), pImageBorderRightBottom->GetHeight(), -1);
		UI::ImageRender::Render(pImageBorderBottom, iX + pImageBorderLeftBottom->GetWidth(), iY + 10 + iSkillInfoBoxHeight - pImageBorderBottom->GetHeight(), iSkillInfoBoxWidth - pImageBorderLeftTop->GetWidth() - pImageBorderRightTop->GetWidth(), pImageBorderBottom->GetHeight(), -1);

		//Render Text
		int iAddTextHeight = 0;
		if (vLeftText.size() == vRightText.size())
		{
			for (size_t i = 0; i < vLeftText.size(); i++)
			{
				Text_ptr pLeftText = vLeftText[i];
				Text_ptr pRightText = vRightText[i];

				if (pLeftText->GetPropertyID() == ETextPropertyID::Elemental)
				{
					UI::ImageRender::Render(pImageSkillElements[pLeftText->GetPropertyValue()], iX + (iSkillInfoBoxWidth - 15 - 10), iY + pLeftText->GetRect().iY, 15, 15, -1);
				}

				//Load Texts
				pLeftText->SetNoClip(TRUE);
				pRightText->SetNoClip(TRUE);

				//pLeftText->SetHorizontalAlign( ALIGN_Left );
				//pRightText->SetHorizontalAlign( ALIGN_Left );

				//Render Texts
				pLeftText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
				pRightText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
			}

			if (!bInfoBoxSkillTimer)
			{
				for (size_t i = 0; i < vCompareText.size(); i++)
				{
					Text_ptr pCompareText = vCompareText[i];
					if (pCompareText)
					{
						pCompareText->SetNoClip(TRUE);
						//pCompareText->SetHorizontalAlign( ALIGN_Left );
						pCompareText->Render(iX, iY, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
					}
				}
			}
		}

		//Render Correct Item Group
		if (cSkillWrapper.Skill_Info.UseWeaponCode[0] != 0x00000000 && uImageIconsItemsY > 0)
		{
			for (size_t i = 0; i < 8; i++)
			{
				if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWA1)
					UI::ImageRender::Render(pImageIconItems[1], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWM1)
					UI::ImageRender::Render(pImageIconItems[2], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWH1)
					UI::ImageRender::Render(pImageIconItems[3], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinDS1)
					UI::ImageRender::Render(pImageIconItems[4], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWP1)
					UI::ImageRender::Render(pImageIconItems[5], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWS2)
					UI::ImageRender::Render(pImageIconItems[6], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWC1)
					UI::ImageRender::Render(pImageIconItems[7], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWS1)
					UI::ImageRender::Render(pImageIconItems[8], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
				else if (cSkillWrapper.Skill_Info.UseWeaponCode[i] == sinWT1)
					UI::ImageRender::Render(pImageIconItems[9], iX + 13 + (i * 20), iY + uImageIconsItemsY, 18, 24, -1);
			}
		}
	}

	void SkillInfoBox::DrawSkillInfoBox(int iX, int iY, int iWidth, int iHeight)
	{
		//Draw Item Info Box Background
		if (pRenderTargetMask)
		{
			if (GRAPHICENGINE->GetGraphicsDevice()->SetRenderTarget(pRenderTargetMask))
			{
				//Clear Render Target
				GRAPHICENGINE->GetGraphicsDevice()->Clear(TRUE, TRUE, FALSE);

				//Draw Background
				GRAPHICENGINE->DrawRectangle2D(Rectangle2D(0, 0, iWidth, iHeight),
					(iSkillInfoOffset == 0 || SkillButtonIndex == 0) ? D3DCOLOR_ARGB(235, 45, 39, 27) : D3DCOLOR_ARGB(235, 9, 66, 71), //background
					(iSkillInfoOffset == 0 || SkillButtonIndex == 0) ? D3DCOLOR_ARGB(245, 9, 23, 22) : D3DCOLOR_ARGB(235, 9, 23, 22)); //border

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