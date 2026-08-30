#include "SkillManager.h"
#include "sinbaram\\sinlinkheader.h"

extern char* sinAttackType2[2];
extern int SummonSkillCodeIndex[10];

SkillManager::SkillManager()
{
	ZeroMemory(baPetAggressive, sizeof(BOOL) * 8);

	for (int i = 0; i < _countof(baPetAggressive); i++)
	{
		baPetAggressive[i] = FALSE;
	}
}

SkillManager::~SkillManager()
{
}

void SkillManager::CancelForceOrb(int iForceOrbID)
{
	if (MESSAGEBOX->GetType() == FALSE)
		return;
	else
	{
		if (ContinueSkill[iForceOrbID].CODE == SKILL_FORCE_ORB)
		{
			ContinueSkill[iForceOrbID].CheckTime = ContinueSkill[iForceOrbID].UseTime * 70;
			cInvenTory.SetItemToChar();
		}
	}
}

extern DWORD SummonSkillCode[4];

// Modo padrão dos PETS (A incluir nas configurações)
BOOL SkillManager::SetDefaultPetMode(int iSkillID, BOOL bAgressive)
{
	for (int i = 0; i < _countof(SummonSkillCode); i++)
	{
		if (SummonSkillCode[i] == iSkillID)
		{
			baPetAggressive[i] = bAgressive;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL SkillManager::OnMouseClickSkillPet()
{
	int Cnt = 0;

	int iID = ContinueSkillMouseIndex - 1;

	if (iID >= 0)
	{
		for (int i = 0; i < _countof(SummonSkillCode); i++)
		{
			// Modo de skill de evasão da arqueira
			if (TempContinueSkill[iID].CODE == SKILL_EVASION_MASTERY)
			{
				if (UNITDATA->PlayEvasionMastery)
				{
					UNITDATA->PlayEvasionMastery = 0;
				}
				else
				{
					UNITDATA->PlayEvasionMastery = 1;
				}

				cInvenTory.SetItemToChar();

				return TRUE;
			}

			// Modo de ataque dos pets
			if (TempContinueSkill[iID].CODE == SummonSkillCode[i])
			{
				baPetAggressive[i] = baPetAggressive[i] ? FALSE : TRUE;
				SummonSkillCodeIndex[i] ^= 1;

				SetDefaultPetMode(i, SummonSkillCodeIndex[i]);
				SendUpdateSkillToServer(TempContinueSkill[iID].CODE, SummonSkillCodeIndex[i], 0, 0);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL SkillManager::OnMouseClick(class CMouse* pcMouse)
{
	if (pcMouse->GetEvent() == EMouseEvent::ClickDownR && ContinueSkillMouseIndex)
	{
		int iID = ContinueSkillMouseIndex - 1;

		if (ContinueSkill[iID].CODE == SKILL_FORCE_ORB)
		{
			MESSAGEBOX->SetBoxType(CMessageBox::BOXTYPEID_YesNo);
			MESSAGEBOX->SetTitle("Confirmar Cancelamento");
			MESSAGEBOX->SetDescription("Deseja cancelar a força ativa?");
			MESSAGEBOX->SetEvent(std::bind(&SkillManager::CancelForceOrb, this, iID));
			MESSAGEBOX->Show();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL SkillManager::OnKeyPress(class CKeyboard* pcKeyboard)
{
	return TRUE;
}

BOOL SkillManager::AddDamageVsMonsterTypeInfo(sSKILL* pcSkill)
{
	std::string text = "";

	switch (pcSkill->CODE)
	{
		//Mech
	case SKILL_SPARK:				text = "+50% Dano VS Normal";			break;
	case SKILL_MECHANIC_BOMB:		text = "+50% Dano VS Mutant";	break;
	case SKILL_AUTOMATION:			text = "+150% Dano VS Monstros";			break;
	case SKILL_GRAND_SMASH:			text = "+50% Dano VS Normal";			break;
	case SKILL_MAGNETIC_SPHERE:		text = "+30% Dano VS Normal";			break;
	case SKILL_IMPULSION:			text = "+30% Dano VS Normal";			break;
	case SKILL_SPARK_SHIELD:		text = "+50% Dano VS Normal";			break;

		//Pikeman
	case SKILL_JUMPING_CRASH:		text = "+30% Dano VS Demônio";				break;

		//Archer
	case SKILL_WIND_ARROW:			text = "+30% Dano VS Normal";				break;
	case SKILL_PERFECT_AIM:			text = "+30% Dano VS Dano";				break;
	case SKILL_BOMB_SHOT:			text = "Dano Extra VS Demônio";				break;

		//Knight
	case SKILL_SWORD_BLAST:			text = "Dano Extra Vs Morto-Vivo";			break;
	case SKILL_PIERCING:			text = "+30% Dano Vs Mutante";				break;
	case SKILL_GRAND_CROSS:			text = "+30% Dano VS Normal ou Mutante";	break;

		//Priestess
	case SKILL_DIVINE_LIGHTNING:	text = "+50% Dano VS Morto-Vivo";				break;
	case SKILL_VIGOR_BALL:			text = "+100% Dano VS Morto-Vivo";			break;
	case SKILL_CHAIN_LIGHTNING:		text = "+50% Dano VS Morto-Vivo";				break;

		//Atalanta

	case SKILL_SHIELD_STRIKE:		text = "+30% Dano VS Mutante";				break;
	case SKILL_FARINA:				text = "+20% Dano VS Normal";				break;
	case SKILL_TWIST_JAVELIN:		text = "+30% Dano VS Normal";				break;
	case SKILL_FIRE_JAVELIN:		text = "+30% Dano VS Demônios";				break;
	case SKILL_LIGHTNING_JAVELIN:	text = "+30% Dano VS Mutante";				break;
	case SKILL_STORM_JAVELIN:		text = "+30% Dano VS Morto-Vivo";				break;
	}

	if (text != "")
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, text.c_str(), D3DCOLOR_XRGB(222, 0, 14));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddSpecificMonsterTypeInfo(sSKILL* pcSkill)
{
	std::string text = "";

	switch (pcSkill->CODE)
	{
	case SKILL_HOLY_REFLECTION:		text = "Somente Morto-vivo";			break;
	case SKILL_EXTINCTION:			text = "Somente Morto-vivo";			break;
	case SKILL_HOLY_INCANTATION:	text = "Somente Morto-vivo";		break;
	}

	if (text != "")
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, text.c_str(), D3DCOLOR_XRGB(222, 0, 14));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddSummonInfo(sSKILL* pcSkill)
{
	if (pcSkill->CODE == SKILL_METAL_GOLEM || pcSkill->CODE == SKILL_FIRE_ELEMENTAL || pcSkill->CODE == SKILL_RECALL_WOLVERIN)
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "Invocação", D3DCOLOR_XRGB(224, 198, 52));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::IsDebuffSkill(DWORD eSkillID)
{
	switch (eSkillID)
	{
		//Fighter
	case SKILL_ROAR:

		//Pikeman
	case SKILL_GROUND_PIKE:
	case SKILL_VENOM_SPEAR:
	case SKILL_ASSASSIN_EYE:

		//Atalanta
	case SKILL_SHIELD_STRIKE:
	case SKILL_SOUL_SUCKER:

	case SKILL_DISTORTION:

		//Priestess
	case SKILL_HOLY_MIND:
	case SKILL_EXTINCTION:
		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddPassiveInfo(sSKILL* pcSkill)
{
	if (pcSkill->Skill_Info.USECODE == 0x04000000)
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "Habilidade Passiva", D3DCOLOR_XRGB(19, 140, 176));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddDebuffInfo(sSKILL* pcSkill)
{
	if (SKILLMANAGERHANDLER->IsDebuffSkill(pcSkill->CODE))
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "Debuff", D3DCOLOR_XRGB(202, 87, 255));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddBuffInfo(sSKILL* pcSkill)
{
	std::string text = "";

	switch (pcSkill->CODE)
	{
	case SKILL_EXTREME_SHIELD:
	case SKILL_PHYSICAL_ABSORB:
	case SKILL_AUTOMATION:
	case SKILL_METALARMOR:
	case SKILL_SPARK:
	case SKILL_COMPULSION:
	case SKILL_MAGNETIC_SPHERE:
	case SKILL_BERSERKER:
	case SKILL_CONCENTRATION:
	case SKILL_SWIFT_AXE:
		text = "Buff Individual";
		break;

	case SKILL_VIRTUAL_LIFE:
	case SKILL_HOLY_VALOR:
	case SKILL_TRIUMPH_OF_VALHALLA:
	case SKILL_HALL_OF_VALHALLA:
	case SKILL_ENCHANT_WEAPON:
	case SKILL_FORCE_OF_NATURE:
		text = "Buff em Grupo";
		break;

	case SKILL_HOLY_BODY:
	case SKILL_GRAND_HEALING:
		text = "Cura em Grupo";
		break;

	}

	if (text != "")
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, text.c_str(), D3DCOLOR_XRGB(83, 207, 83));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

BOOL SkillManager::AddHintInfo(sSKILL* pcSkill)
{
	std::string text = "";

	switch (pcSkill->CODE)
	{
	case SKILL_GRAND_SMASH:			text = "Taxa de Ataque Adicional restrita a martelos e garras";	break;
	case SKILL_METALARMOR:			text = "Requer Armadura na especialização Mecânico";			break;
	case SKILL_CHARGING_STRIKE:		text = "Pode ser carregada ao manter pressionada";				break;
	case SKILL_PHOENIX_SHOT:		text = "Pode ser carregada ao manter pressionada";				break;
	}

	//is for bubble skill icon (iCheckTime will be non-zero)

	switch (pcSkill->CODE)
	{
	case SKILL_METAL_GOLEM:
	case SKILL_RECALL_WOLVERIN:
	case SKILL_FIRE_ELEMENTAL:
	{
		if (pcSkill->CheckTime)
		{
			text = "Clique duplo na habilidade para alterar o modo de ação do PET";					break;
		}
		else
		{
			text = "Modo de ação pode ser alterado quando a habilidade estiver ativa";				break;
		}
	}
	}

	if (text != "")
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, text.c_str(), D3DCOLOR_XRGB(191, 191, 191));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");

		return TRUE;
	}

	return FALSE;
}

void SkillManager::AddSkillBoxInfoDuration(int SkillValueLevel, int iSkillLevelZeroBased, int NextSkillValueLevel, int SkillID)
{
	int minutes, seconds, value, nextValue;

	value = SkillValueLevel;

	if (SkillID == SKILL_SCOUT_HAWK)
	{
		value = static_cast<int>((value * 2.4));
	}


	if (value == 0)
	{
#ifdef DEV_MODE
		if (GM_MODE)
		{
			char hexString[16];
			itoa(iSkillArrayPointerID, hexString, 16);

			CHATBOX->AddMessage(FormatString("Duration: contains 0 value(s) (%s)", hexString), EChatColor::CHATCOLOR_White, NULL, TRUE);
		}
#endif
		return;
	}

	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "Duração: ", D3DCOLOR_XRGB(243, 237, 216));

	if (value > 60)
	{
		minutes = (int)(value / 60);
		seconds = value % 60;

		if (seconds > 0)
		{
			GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString("%dm %ds", minutes, seconds), D3DCOLOR_XRGB(238, 210, 151));
		}
		else
		{
			GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString("%dm", minutes), D3DCOLOR_XRGB(238, 210, 151));
		}
	}
	else
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString("%ds", value), D3DCOLOR_XRGB(238, 210, 151));
	}

	//has next level info?
	if (iSkillLevelZeroBased < 9)
	{
		nextValue = NextSkillValueLevel;

		if (nextValue != value)
		{
			if (nextValue > 60)
			{
				minutes = (int)(nextValue / 60);
				seconds = nextValue % 60;

				if (seconds > 0)
				{
					GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%dm %ds)", minutes, seconds), D3DCOLOR_XRGB(92, 163, 99), FALSE, EAlign::ALIGN_Left);
				}
				else
				{
					GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%dm)", minutes), D3DCOLOR_XRGB(92, 163, 99), FALSE, EAlign::ALIGN_Left);
				}
			}
			else
			{
				GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%ds)", nextValue), D3DCOLOR_XRGB(92, 163, 99), FALSE, EAlign::ALIGN_Left);
			}
		}
		else
		{
			GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
		}
	}
	else
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
	}
}

void SkillManager::AddSkillBoxInfo(const char* pszLabel, const char* pszFormat, int SkillValueLevel, int iSkillLevelZeroBased, int NextSkillValueLevel, int SkillID, BOOL bShowNextLevel, float fScaling)
{
	int value = SkillValueLevel;

	if (value != 0)
	{
		if (fScaling != 1.0f)
		{
			value = static_cast<int>(round(static_cast<float>(value) * fScaling));
		}

		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszLabel, D3DCOLOR_XRGB(243, 237, 216));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString(pszFormat, value), D3DCOLOR_XRGB(238, 210, 151));

		//has next level info?
		if (iSkillLevelZeroBased < 9 && bShowNextLevel)
		{
			value = NextSkillValueLevel;

			if (SkillID == SKILL_SCOUT_HAWK)
			{
				value = static_cast<int>(round(value * 2.4));
			}

			std::string format = FormatString(pszFormat, value);
			GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%s)", format.c_str()), D3DCOLOR_XRGB(92, 163, 99), FALSE, EAlign::ALIGN_Left);
		}
		else
		{
			GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
		}
	}
	else
	{
#ifdef DEV_MODE
		if (GM_MODE)
		{
			char hexString[16];
			itoa(iSkillArrayPointerID, hexString, 16);

			CHATBOX->AddMessage(FormatString("%s contains 0 value(s) (%s)", pszLabel, hexString), EChatColor::CHATCOLOR_White, NULL, TRUE);
		}
#endif

		GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
	}
}

void SkillManager::AddSkillBoxInfo(const char* pszLabel, const char* pszFormat, float SkillValueLevel, int iSkillLevelZeroBased, float NextSkillValueLevel, int SkillID, BOOL bShowNextLevel, float fScaling)
{
	if (STRINGCOMPARE(pszFormat, "%.0f") || STRINGCOMPARE(pszFormat, "%.1f") || STRINGCOMPARE(pszFormat, "%.2f") ||
		STRINGCOMPARE(pszFormat, "%.0f%%") || STRINGCOMPARE(pszFormat, "%.1f%%") || STRINGCOMPARE(pszFormat, "%.2f%%"))
	{
		float value = SkillValueLevel;

		if (value != 0.0f)
		{
			if (fScaling != 1.0f)
			{
				value = value * fScaling;
			}

			GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszLabel, D3DCOLOR_XRGB(243, 237, 216));
			GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString(pszFormat, value), D3DCOLOR_XRGB(238, 210, 151));

			//has next level info?
			if (iSkillLevelZeroBased < 9 && bShowNextLevel)
			{
				std::string format = FormatString(pszFormat, NextSkillValueLevel);
				GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%s)", format.c_str()), D3DCOLOR_XRGB(92, 163, 99), FALSE, EAlign::ALIGN_Left);
			}
			else
			{
				GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
			}
		}
		else
		{
#ifdef DEV_MODE
			if (GM_MODE)
			{
				char hexString[16];
				itoa(iSkillArrayPointerID, hexString, 16);

				CHATBOX->AddMessage(FormatString("%s contains 0 value(s) (%s)", pszLabel, hexString), EChatColor::CHATCOLOR_White, NULL, TRUE);
			}
#endif

			GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
		}
	}
}

void SkillManager::AddSkillBoxInfo(const char* pszLabel, const char* pszString, DWORD dwColor)
{
	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszLabel, D3DCOLOR_XRGB(243, 237, 216));
	GAMECOREHANDLE->pcSkillInfoBox->AddString(1, pszString, dwColor);
	GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
}

void SkillManager::AddSkillBoxInfo(const char* pszLabel, const char* pszString)
{
	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszLabel, D3DCOLOR_XRGB(243, 237, 216));
	GAMECOREHANDLE->pcSkillInfoBox->AddString(1, pszString, D3DCOLOR_XRGB(238, 210, 151));
	GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
}

void SkillManager::AddSkillBoxInfoRange(const char* pszLabel, const char* pszFormat, int SkillValueLevel, int iSkillLevelZeroBased, int NextSkillValueLevel, BOOL bShowNextLevel, float fScale)
{
	int value = SkillValueLevel;

	//only scale if not 1.0f
	if (fScale != 1.0f)
		value = static_cast<int>(round(static_cast<float>(value) * fScale));

	float valueMeters = static_cast<float>(value) / 27.0f;

	//don't display skill value if it's 0
	if (value == 0)
	{
		if (smConfig.DebugMode)
		{
			char hexString[16];
			itoa(SkillValueLevel, hexString, 16);

			CHATBOX->AddMessage(FormatString("%s contains 0 value(s) (%s)", pszLabel, hexString), EChatColor::CHATCOLOR_Error, NULL);
		}

		return;
	}
	std::string format = FormatString(pszFormat, value);

	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszLabel, D3DCOLOR_XRGB(243, 237, 216));
	GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString("%s [%.1fm]", format.c_str(), valueMeters), D3DCOLOR_XRGB(238, 210, 151));

	//has next level info and is not same?
	if (iSkillLevelZeroBased < 9 && bShowNextLevel)
	{
		int nextValue = NextSkillValueLevel;

		if (nextValue != value)
		{
			valueMeters = static_cast<float>(nextValue) / 27.0f;

			format = FormatString(pszFormat, NextSkillValueLevel);
			GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%s [%.1fm])", format.c_str(), valueMeters), D3DCOLOR_XRGB(92, 163, 99), FALSE, EAlign::ALIGN_Left);
		}
		else
		{
			GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
		}
	}
	else
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
	}
}

void SkillManager::AddSkillBoxInfoDual(const char* pszLabel, const char* pszFormat, int SkillValueLevel, int SkillValueLevel2, int iSkillLevelZeroBased, int NextSkillValueLevel, int NextSkillValueLeve2, BOOL bShowNextLevel, float fScaling1, float fScaling2)
{
	int iValue1 = SkillValueLevel;
	int iValue2 = SkillValueLevel2;

	if (iValue1 == 0 || iValue2 == 0)
	{
#ifdef DEV_MODE
		if (GM_MODE)
		{
			char hexString[16];
			itoa(iValue1 == 0 ? iSkillArrayPointerIDFirst : iSkillArrayPointerIDSecond, hexString, 16);
			CHATBOX->AddMessage(FormatString("%s contains 0 value(s) (%s)", pszLabel, hexString), EChatColor::CHATCOLOR_White, NULL, TRUE);
		}
#endif
	}

	if (fScaling1 != 1.0f)
	{
		iValue1 = static_cast<int>(round(static_cast<float>(iValue1) * fScaling1));
	}

	if (fScaling2 != 1.0f)
	{
		iValue2 = static_cast<int>(round(static_cast<float>(iValue2) * fScaling2));
	}

	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszLabel, D3DCOLOR_XRGB(243, 237, 216));
	GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString(pszFormat,
		iValue1,
		iValue2),
		D3DCOLOR_XRGB(238, 210, 151));

	//has next level info?
	if (iSkillLevelZeroBased < 9 && bShowNextLevel)
	{
		std::string format = FormatString(pszFormat,
			NextSkillValueLevel,
			NextSkillValueLeve2);

		GAMECOREHANDLE->pcSkillInfoBox->AddString(2, FormatString("(%s)", format.c_str()), D3DCOLOR_XRGB(92, 163, 99));
	}
	else
	{
		GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
	}
}

BOOL SkillManager::AddSkillInfoToBox(int iSkillID, int iSkillLevelZeroBased)
{
	if (AddSkillInfoToBox_AS(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_FS(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_MS(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_PS(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_ATA(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_KS(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_PRS(iSkillID, iSkillLevelZeroBased) ||
		AddSkillInfoToBox_MGS(iSkillID, iSkillLevelZeroBased))
	{
		return TRUE;
	}

	return FALSE;
}

void SkillManager::AddSkillBoxGroupTitle(const char* pszTitle, D3DCOLOR dwColor, int eElementID)
{
	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "\n");

	auto textBoxPtr = GAMECOREHANDLE->pcSkillInfoBox->AddString(0, pszTitle, dwColor, TRUE);

	if (textBoxPtr)
	{
		switch (eElementID)
		{
		case sITEMINFO_FIRE:
		case sITEMINFO_ICE:
		case sITEMINFO_BIONIC:
		case sITEMINFO_LIGHTING:
		case sITEMINFO_POISON:
		{
			textBoxPtr->SetPropertyID(ETextPropertyID::Elemental);
			textBoxPtr->SetPropertyValue(eElementID);
		}
		break;
		}
	}

	GAMECOREHANDLE->pcSkillInfoBox->AddString(1, "");
	GAMECOREHANDLE->pcSkillInfoBox->AddString(2, "");
}

void SkillManager::GetInfoBoxSize(struct sSKILLBOX* psSkillBox, int iColumn, int iIcon)
{
	CSKILL->GetInfoBoxSize(psSkillBox, iColumn, iIcon);
}

void SkillManager::AddSkillElement(int eElement)
{
	GAMECOREHANDLE->pcSkillInfoBox->AddSkillElement(eElement);
}

void SkillManager::AddSkillBoxEmptyLine()
{
	GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "\n");
}

BOOL SkillManager::AddSkillInfoToBox_AS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;
	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{

		//Archer
		//t1
	case SKILL_SCOUT_HAWK:
	{
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Scout_Hawk_Attack_Rate[iSkillLevelZeroBased], iSkillLevelZeroBased, Scout_Hawk_Attack_Rate[NextLevel]);
		AddSkillBoxInfoDuration(Scout_Hawk_UseTime[iSkillLevelZeroBased], iSkillLevelZeroBased, Scout_Hawk_UseTime[NextLevel]);
		break;
	}

	case SKILL_SHOOTING_MASTERY:
	{
		AddSkillBoxInfo("Adicional de Dano na Arma", "%d%%", S_Mastery_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, S_Mastery_DamagePercent[NextLevel]);
		break;
	}

	case SKILL_WIND_ARROW:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Wind_Arrow_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Wind_Arrow_Damage[NextLevel]);
		AddSkillBoxInfo("Vel. Ataque Adicional:", "%d", Wind_Arrow_Speed[iSkillLevelZeroBased], iSkillLevelZeroBased, Wind_Arrow_Speed[NextLevel]);
		break;
	}

	case SKILL_PERFECT_AIM:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Perfect_Aim_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Perfect_Aim_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Perfect_Aim_Attack_Rate[iSkillLevelZeroBased], iSkillLevelZeroBased, Perfect_Aim_Attack_Rate[NextLevel]);
		break;
	}

	//t2
	case SKILL_DIONS_EYE:
	{
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", D_Eye_Attack_Rate[iSkillLevelZeroBased], iSkillLevelZeroBased, D_Eye_Attack_Rate[NextLevel]);
		break;
	}

	case SKILL_FALCON:
	{
		AddSkillBoxInfoDual("Falcon Poder de Ataque:", "%d + %d%%", Falcon_Damage[iSkillLevelZeroBased][0], Falcon_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Falcon_Damage[NextLevel][0], Falcon_Damage[NextLevel][1]);
		AddSkillBoxInfoDuration(Falcon_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Falcon_Time[NextLevel]);
		break;
	}

	case SKILL_ARROW_OF_RAGE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", A_Rage_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, A_Rage_Damage[NextLevel]);
		AddSkillBoxInfo("Número de Flechas:", "%d%%", A_Rage_ArrowNum[iSkillLevelZeroBased], iSkillLevelZeroBased, A_Rage_ArrowNum[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", A_Rage_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, A_Rage_Area[NextLevel]);
		break;
	}

	case SKILL_AVALANCHE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Avalanche_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Avalanche_Damage[NextLevel]);
		AddSkillBoxInfo("Vel. Ataque Adicional:", "%d", Avalanche_Speed[iSkillLevelZeroBased], iSkillLevelZeroBased, Avalanche_Speed[NextLevel]);
		AddSkillBoxInfo("Número de Flechas:", "%d", Avalanche_ArrowNum[iSkillLevelZeroBased], iSkillLevelZeroBased, Avalanche_ArrowNum[NextLevel]);
		break;
	}

	//t3
	case SKILL_ELEMENTAL_SHOT:
	{
		AddSkillElement(sITEMINFO_FIRE);
		AddSkillElement(sITEMINFO_LIGHTING);

		AddSkillBoxInfoDual("Dano de Fogo:", "%d%% + %d", Elemental_Shot_Fire[iSkillLevelZeroBased][0], Elemental_Shot_Fire[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Elemental_Shot_Fire[NextLevel][0], Elemental_Shot_Fire[NextLevel][1]);
		AddSkillBoxInfoDual("Dano de Raio:", "%d%% + %d", Elemental_Shot_Lightning[iSkillLevelZeroBased][0], Elemental_Shot_Lightning[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Elemental_Shot_Lightning[NextLevel][0], Elemental_Shot_Lightning[NextLevel][1]);
		break;
	}

	case SKILL_GOLDEN_FALCON:
	{
		AddSkillBoxInfo("Dano Extra do Falcão:", "%d%%", Golden_Falcon_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Golden_Falcon_Damage[NextLevel]);
		AddSkillBoxInfo("Regeneração de Vida Adicional:", "%.0f", Golden_Falcon_LifeRegen[iSkillLevelZeroBased], iSkillLevelZeroBased, Golden_Falcon_LifeRegen[NextLevel]);
		AddSkillBoxInfoDuration(Golden_Falcon_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Golden_Falcon_Time[NextLevel]);
		break;
	}

	case SKILL_BOMB_SHOT:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", BombShot_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, BombShot_Damage[NextLevel]);
		AddSkillBoxInfo("Dano Extra (Demônios):", "%d%%", BombShot_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, BombShot_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", BombShot_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, BombShot_Area[NextLevel]);
		break;
	}

	case SKILL_PERFORATION:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Perforation_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Perforation_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range Adicional:", "%d", Perforation_Attack_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Perforation_Attack_Range[NextLevel]);
		AddSkillBoxInfo("Chance Extra de Crítico:", "%d%%", Perforation_Critical[iSkillLevelZeroBased], iSkillLevelZeroBased, Perforation_Critical[NextLevel]);
		break;
	}

	//t4
	case SKILL_RECALL_WOLVERIN:
	{
		AddSkillBoxInfoDual("Poder de Ataque:", "%d - %d", Recall_Wolverin_Damage[iSkillLevelZeroBased][0], Recall_Wolverin_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Recall_Wolverin_Damage[NextLevel][0], Recall_Wolverin_Damage[NextLevel][1]);
		AddSkillBoxInfo("Taxa de Ataque:", "%d", Recall_Wolverin_Hit[iSkillLevelZeroBased], iSkillLevelZeroBased, Recall_Wolverin_Hit[NextLevel]);
		AddSkillBoxInfo("Defesa:", "%d", Recall_Wolverin_Defense[iSkillLevelZeroBased], iSkillLevelZeroBased, Recall_Wolverin_Defense[NextLevel]);
		AddSkillBoxInfo("Vida:", "%d", Recall_Wolverin_Life[iSkillLevelZeroBased], iSkillLevelZeroBased, Recall_Wolverin_Life[NextLevel]);
		AddSkillBoxInfoDuration(3600, iSkillLevelZeroBased, 3600); // Ver tempo de duração do wolverine

		// Criar opção de função padrão do pet
		if (!GAMECOREHANDLE->pcSkillInfoBox->IsViewingSkillTimer())
		{
			AddSkillBoxGroupTitle("Status Padrão do PET", D3DCOLOR_XRGB(238, 210, 151));
			AddSkillBoxInfo("Modo:", baPetAggressive[2] ? "Automático" : "Manual");
		}
		else
		{
			AddSkillBoxGroupTitle("Status do PET", D3DCOLOR_XRGB(238, 210, 151));
			AddSkillBoxInfo("Modo:", baPetAggressive[2] ? "Automático" : "Manual");
		}

		break;
	}

	case SKILL_EVASION_MASTERY:
	{
		AddSkillBoxInfo("Add Evasion", "+%d%%", Evasion_Mastery_AddPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Evasion_Mastery_AddPercent[NextLevel]);
		AddSkillBoxInfo("Add Speed", "+%d%%", Evasion_Mastery_AddSpeed[iSkillLevelZeroBased], iSkillLevelZeroBased, Evasion_Mastery_AddSpeed[NextLevel]);
		AddSkillBoxInfo("Reduce Speed", "+%d%%", Evasion_Mastery_AddReduce[iSkillLevelZeroBased], iSkillLevelZeroBased, Evasion_Mastery_AddReduce[NextLevel]);
		break;
	}

	case SKILL_PHOENIX_SHOT:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Phoenix_Shot_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Phoenix_Shot_DamagePercent[NextLevel]);

		AddSkillBoxGroupTitle("Charging mode", D3DCOLOR_XRGB(238, 210, 151));
		AddSkillBoxInfo("Charging Speed:", "%d", Phoenix_Shot_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Phoenix_Shot_Time[NextLevel]);
		break;
	}

	case SKILL_FORCE_OF_NATURE:
	{
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Force_Of_Nature_AddDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, Force_Of_Nature_AddDamage[NextLevel], 0, TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_ARCHER ? 1.0f : 0.5f);
		AddSkillBoxInfo("Falcon Extra Damage:", "%d", Force_Of_Nature_FalconAddDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, Force_Of_Nature_FalconAddDamage[NextLevel]);

		AddSkillBoxInfoDuration(Force_Of_Nature_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Force_Of_Nature_Time[NextLevel]);
		break;
	}


	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_FS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;

	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
		//Fighter
		//t1
	case SKILL_MELEE_MASTERY:
	{
		AddSkillBoxInfo("Adicional de Dano na Arma", "%d%%", Melee_Mastery_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Melee_Mastery_DamagePercent[NextLevel]);
		break;
	}

	case SKILL_FIRE_ATTRIBUTE:
	{
		AddSkillBoxInfo("Resistência a Fogo:", "%d%%", PlusFire[iSkillLevelZeroBased], iSkillLevelZeroBased, PlusFire[NextLevel]);
		break;
	}

	case SKILL_RAVING:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Raving_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Raving_Damage[NextLevel]);
		AddSkillBoxInfo("Velocidade de Ataque:", "%d", Ravind_Speed[iSkillLevelZeroBased], iSkillLevelZeroBased, Ravind_Speed[NextLevel]);
		AddSkillBoxInfo("Custo de HP:", "%d", Raving_UseLife[iSkillLevelZeroBased], iSkillLevelZeroBased, Raving_UseLife[NextLevel]);
		break;
	}

	case SKILL_IMPACT:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Impact_Attack_Rating[iSkillLevelZeroBased], iSkillLevelZeroBased, Impact_Attack_Rating[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Impact_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Impact_Damage[NextLevel]);
		break;
	}

	//t2
	case SKILL_TRIPLE_IMPACT:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", T_Impact_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, T_Impact_Damage[NextLevel]);
		AddSkillBoxInfo("Number of attacks:", "%d", T_Impact_Hit[iSkillLevelZeroBased], iSkillLevelZeroBased, T_Impact_Hit[NextLevel]);
		break;
	}

	case SKILL_BRUTAL_SWING:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", B_Swing_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, B_Swing_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Crítico Adicional:", "%d%%", B_Swing_Critical[iSkillLevelZeroBased], iSkillLevelZeroBased, B_Swing_Critical[NextLevel]);
		AddSkillBoxInfo("Custo de HP:", "%d", B_Swing_UseMana[iSkillLevelZeroBased], iSkillLevelZeroBased, B_Swing_UseMana[NextLevel]);
		break;
	}

	case SKILL_ROAR:
	{
		BOOL bHalf = UNITDATA->smCharInfo.JOB_CODE != JOBCODE_FIGHTER;

		GAMECOREHANDLE->pcSkillInfoBox->AddSkillElement(sITEMINFO_BIONIC);

		AddSkillBoxInfo("Duração do Atordoamento:", "%ds", Roar_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Roar_Time[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Roar_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Roar_Range[NextLevel]);
		break;
	}

	case SKILL_RAGE_OF_ZECRAM:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", R_Zecram_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, R_Zecram_Damage[NextLevel]);
		break;
	}

	//t3
	case SKILL_CONCENTRATION:
	{
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Concentration_AttackRate[iSkillLevelZeroBased], iSkillLevelZeroBased, Concentration_AttackRate[NextLevel]);

		AddSkillBoxInfoDuration(Concentration_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Concentration_Time[NextLevel]);
		break;
	}

	case SKILL_AVANGING_CRASH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", A_Crash_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, A_Crash_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", A_Crash_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, A_Crash_Damage[NextLevel]);
		break;
	}

	case SKILL_SWIFT_AXE:
	{
		AddSkillBoxInfo("Velocidade de Ataque Adicional:", "%d%%", Swift_Axe_Speed[iSkillLevelZeroBased], iSkillLevelZeroBased, Swift_Axe_Speed[NextLevel]);
		AddSkillBoxInfoDuration(Swift_Axe_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Swift_Axe_Time[NextLevel]);
		break;
	}

	case SKILL_BONE_CRASH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", B_Crash_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, B_Crash_Damage[NextLevel]);
		AddSkillBoxInfo("Dano Extra (Demônios):", "%d%%", B_Crash_DemonDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, B_Crash_DemonDamage[NextLevel]);
		break;
	}

	//t4
	case SKILL_DETORYER:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Destoryer_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Destoryer_DamagePercent[NextLevel]);
		AddSkillBoxInfo("Taxa de Crítico Adicional:", "%d%%", Destoryer_AddCritical[iSkillLevelZeroBased], iSkillLevelZeroBased, Destoryer_AddCritical[NextLevel]);
		break;
	}

	case SKILL_BERSERKER:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Berserker_AddAttack[iSkillLevelZeroBased], iSkillLevelZeroBased, Berserker_AddAttack[NextLevel]);
		AddSkillBoxInfo("Diminuição na Absorção:", "%d%%", Berserker_SubAbsorb[iSkillLevelZeroBased], iSkillLevelZeroBased, Berserker_SubAbsorb[NextLevel]);

		AddSkillBoxInfoDuration(Berserker_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Berserker_Time[NextLevel]);
		break;
	}

	case SKILL_CYCLONE_STRIKE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Cyclone_Strike_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Cyclone_Strike_DamagePercent[NextLevel]);
		AddSkillBoxInfo("Contagem de Golpes:", "%d", Cyclone_Strike_AttackNum[iSkillLevelZeroBased], iSkillLevelZeroBased, Cyclone_Strike_AttackNum[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Cyclone_Strike_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Cyclone_Strike_Area[NextLevel]);
		break;
	}


	case SKILL_BOOST_HEALTH:
	{
		AddSkillBoxInfo("Regeneração de HP:", "+%d", Boost_Health_Life[iSkillLevelZeroBased], iSkillLevelZeroBased, Boost_Health_Life[NextLevel]);
		break;
	}


	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_MS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;

	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
		//t1
	case SKILL_EXTREME_SHIELD:
	{
		AddSkillBoxInfo("Taxa de Bloqueio Adicional:", "+%d%%", E_Shield_BlockRate[iSkillLevelZeroBased], iSkillLevelZeroBased, E_Shield_BlockRate[NextLevel]);
		AddSkillBoxInfoDuration(E_Shield_UseTime[iSkillLevelZeroBased], iSkillLevelZeroBased, E_Shield_UseTime[NextLevel]);
		break;
	}

	case SKILL_MECHANIC_BOMB:
	{
		AddSkillElement(sITEMINFO_FIRE);

		AddSkillBoxInfoDual("Dano:", "%d-%d", Mechanic_Bomb_Damage[iSkillLevelZeroBased][0], Mechanic_Bomb_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Mechanic_Bomb_Damage[NextLevel][0], Mechanic_Bomb_Damage[NextLevel][1]);
		AddSkillBoxInfoRange("Range:", "%d", Mechanic_Bomb_Attack_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Mechanic_Bomb_Attack_Range[NextLevel]);
		break;
	}

	case SKILL_PHYSICAL_ABSORB:
	{
		AddSkillBoxInfoDual("Bônus de Absorção:", "%d-%d", P_Absorb[iSkillLevelZeroBased][0], P_Absorb[iSkillLevelZeroBased][1], iSkillLevelZeroBased, P_Absorb[NextLevel][0], P_Absorb[NextLevel][1]);
		AddSkillBoxInfoDuration(P_Absorb_UseTime[iSkillLevelZeroBased], iSkillLevelZeroBased, P_Absorb_UseTime[NextLevel]);
		break;
	}

	case SKILL_POISON_ATTRIBUTE:
	{
		AddSkillBoxInfo("Poison Resistance:", "%d", PlusPoison[iSkillLevelZeroBased], iSkillLevelZeroBased, PlusPoison[NextLevel]);
		break;
	}

	//t2
	case SKILL_GREAT_SMASH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d", G_Smash_Attack_Rate[iSkillLevelZeroBased], iSkillLevelZeroBased, G_Smash_Attack_Rate[NextLevel]);
		AddSkillBoxInfoDual("Attack Max Boost:", "%d-%d", G_Smash_Attack_Rage[iSkillLevelZeroBased], iSkillLevelZeroBased, G_Smash_Attack_Rage[iSkillLevelZeroBased] + UNITDATA->smCharInfo.Strength / 5, G_Smash_Attack_Rage[NextLevel], G_Smash_Attack_Rage[NextLevel] + UNITDATA->smCharInfo.Strength / 5);
		AddSkillBoxInfo("Extra Damage:", "%d", G_Smash_Attack_Rage[iSkillLevelZeroBased], iSkillLevelZeroBased, G_Smash_Attack_Rage[NextLevel]);
		break;
	}

	case SKILL_MAXIMIZE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Maximize_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Maximize_Damage[NextLevel]);
		AddSkillBoxInfoDuration(Maximize_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Maximize_Time[NextLevel]);
		break;
	}

	case SKILL_AUTOMATION:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Automation_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Automation_Damage[NextLevel]);
		AddSkillBoxInfo("Vel. Ataque Adicional:", "%d", Automation_Speed[iSkillLevelZeroBased], iSkillLevelZeroBased, Automation_Speed[NextLevel]);
		AddSkillBoxInfoDuration(Automation_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Automation_Time[NextLevel]);
		break;
	}

	case SKILL_SPARK:
	{
		AddSkillElement(sITEMINFO_LIGHTING);

		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d-%d + LV/5", Spark_Damage[iSkillLevelZeroBased][0], Spark_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Spark_Damage[NextLevel][0], Spark_Damage[NextLevel][1]);
		AddSkillBoxInfoRange("Range:", "%d", Spark_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Spark_Area[NextLevel]);
		break;
	}

	//t3
	case SKILL_METAL_ARMOR:
	{
		AddSkillBoxInfo("Adicional de Defesa na Armadura::", "%d%%", Metal_Armor_Defense[iSkillLevelZeroBased], iSkillLevelZeroBased, Metal_Armor_Defense[NextLevel]);
		AddSkillBoxInfoDuration(Metal_Armor_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Metal_Armor_Time[NextLevel]);
		break;
	}

	case SKILL_GRAND_SMASH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Grand_Smash_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Grand_Smash_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d", Grand_Smash_AttackRate[iSkillLevelZeroBased], iSkillLevelZeroBased, Grand_Smash_AttackRate[NextLevel]);
		break;
	}

	case SKILL_MECHANIC_WEAPON:
	{
		AddSkillBoxInfo("Adicional de Dano na Arma to AP:", "%d%%", M_Weapon_Mastey[iSkillLevelZeroBased], iSkillLevelZeroBased, M_Weapon_Mastey[NextLevel]);
		break;
	}

	case SKILL_SPARK_SHIELD:
	{
		AddSkillElement(sITEMINFO_LIGHTING);

		AddSkillBoxInfo("Poder de Ataque:", "%d%%", Spark_Shield_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Spark_Shield_Damage[NextLevel]);
		AddSkillBoxInfo("Defesa Boost:", "%d", Spark_Shield_Defense[iSkillLevelZeroBased], iSkillLevelZeroBased, Spark_Shield_Defense[NextLevel]);
		AddSkillBoxInfoDuration(Spark_Shield_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Spark_Shield_Time[NextLevel]);
		break;
	}

	//t4
	case SKILL_IMPULSION:
	{
		AddSkillElement(sITEMINFO_LIGHTING);

		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d-%d", Impulsion_LightingDamage[iSkillLevelZeroBased][0], Impulsion_LightingDamage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Impulsion_LightingDamage[NextLevel][0], Impulsion_LightingDamage[NextLevel][1]);
		AddSkillBoxInfo("Alvos:", "%d", Impulsion_LightingNum[iSkillLevelZeroBased], iSkillLevelZeroBased, Impulsion_LightingNum[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Impulsion_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Impulsion_Range[NextLevel]);
		break;
	}

	case SKILL_COMPULSION:
	{
		AddSkillBoxInfo("Bônus de Absorção:", "%d", Compulsion_AddAbsorb[iSkillLevelZeroBased], iSkillLevelZeroBased, Compulsion_AddAbsorb[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Compulsion_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Compulsion_Area[NextLevel]);
		AddSkillBoxInfoDuration(Compulsion_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Compulsion_Time[NextLevel]);
		break;
	}

	case SKILL_MAGNETIC_SPHERE:
	{
		AddSkillElement(sITEMINFO_LIGHTING);

		AddSkillBoxInfoDual("Crystal Dano Extra:", "%d-%d", Magnetic_Sphere_AddDamage[iSkillLevelZeroBased][0], Magnetic_Sphere_AddDamage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Magnetic_Sphere_AddDamage[NextLevel][0], Magnetic_Sphere_AddDamage[NextLevel][1]);
		AddSkillBoxInfo("Delay de Ataque:", "%d", Magnetic_Sphere_AttackDelay[iSkillLevelZeroBased], iSkillLevelZeroBased, Magnetic_Sphere_AttackDelay[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Magnetic_Sphere_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Magnetic_Sphere_Area[NextLevel]);
		AddSkillBoxInfoDuration(Magnetic_Sphere_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Magnetic_Sphere_Time[NextLevel]);
		break;
	}

	case SKILL_METAL_GOLEM:
	{
		AddSkillBoxInfoDual("Poder de Ataque:", "%d - %d", Metal_Golem_Damage[iSkillLevelZeroBased][0], Metal_Golem_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Metal_Golem_Damage[NextLevel][0], Metal_Golem_Damage[NextLevel][1]); // Something is causing a crash here
		AddSkillBoxInfo("Taxa de Ataque:", "%d", Metal_Golem_Hit[iSkillLevelZeroBased], iSkillLevelZeroBased, Metal_Golem_Hit[NextLevel]);
		AddSkillBoxInfo("Defesa:", "%d", Metal_Golem_Defense[iSkillLevelZeroBased], iSkillLevelZeroBased, Metal_Golem_Defense[NextLevel]);
		AddSkillBoxInfo("Vida:", "%d", Metal_Golem_Life[iSkillLevelZeroBased], iSkillLevelZeroBased, Metal_Golem_Life[NextLevel]);
		AddSkillBoxInfoDuration(3600, iSkillLevelZeroBased, 3600); // Ver duração

		// Criar opção de função padrão do pet
		if (!GAMECOREHANDLE->pcSkillInfoBox->IsViewingSkillTimer())
		{
			AddSkillBoxGroupTitle("Status Padrão do PET", D3DCOLOR_XRGB(238, 210, 151));
			AddSkillBoxInfo("Modo:", baPetAggressive[1] ? "Automático" : "Manual");
		}
		else
		{
			AddSkillBoxGroupTitle("Status do PET", D3DCOLOR_XRGB(238, 210, 151));
			AddSkillBoxInfo("Modo:", baPetAggressive[1] ? "Automático" : "Manual");
		}

		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_PS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;

	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
	case SKILL_PIKE_WIND:
	{
		AddSkillBoxInfoDual("Damage:", "%d - %d", Pike_Wind_Damage[iSkillLevelZeroBased][0], Pike_Wind_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Pike_Wind_Damage[NextLevel][0], Pike_Wind_Damage[NextLevel][1]);
		AddSkillBoxInfoRange("Pushback Range:", "%d", Pike_Wind_Push_Lenght[iSkillLevelZeroBased], iSkillLevelZeroBased, Pike_Wind_Push_Lenght[NextLevel]);
		break;
	}

	case SKILL_ICE_ATTRIBUTE:
	{
		AddSkillBoxInfoRange("Plus Ice Resistance:", "%d", PlusIce[iSkillLevelZeroBased], iSkillLevelZeroBased, PlusIce[NextLevel]);
		break;
	}

	case SKILL_CRITICAL_HIT:
	{
		AddSkillBoxInfo("Add Crítico Hit:", "%d", Critical_Hit_Critical[iSkillLevelZeroBased], iSkillLevelZeroBased, Critical_Hit_Critical[NextLevel]);
		break;
	}

	case SKILL_JUMPING_CRASH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Jumping_Crash_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Jumping_Crash_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Jumping_Crash_Attack_Rating[iSkillLevelZeroBased], iSkillLevelZeroBased, Jumping_Crash_Attack_Rating[NextLevel]);
		break;
	}

	//T2
	case SKILL_GROUND_PIKE:
	{
		AddSkillElement(sITEMINFO_ICE);

		AddSkillBoxInfoDual("Dano de Congelamento:", "%d-%d", G_Pike_Damage[iSkillLevelZeroBased][0], G_Pike_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, G_Pike_Damage[NextLevel][0], G_Pike_Damage[NextLevel][1]);
		AddSkillBoxInfo("Duração do Congelamento:", "%ds", G_Pike_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, G_Pike_Time[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", G_Pike_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, G_Pike_Range[NextLevel]);
		break;
	}

	case SKILL_TORNADO:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Tornado_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Tornado_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Tornado_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Tornado_Range[NextLevel]);
		break;
	}

	case SKILL_WEAPONE_DEFENCE_MASTERY:
	{
		AddSkillBoxInfo("Taxa de Bloqueio Adicional:", "+%d%%", W_D_Mastery_Block[iSkillLevelZeroBased], iSkillLevelZeroBased, W_D_Mastery_Block[NextLevel]);
		break;
	}

	case SKILL_EXPANSION:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Expansion_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Expansion_Damage[NextLevel]);
		AddSkillBoxInfo("Tamanho da Arma:", "%d%%", Expansion_Size[iSkillLevelZeroBased], iSkillLevelZeroBased, Expansion_Size[NextLevel]);
		break;
	}

	//T3
	case SKILL_VENOM_SPEAR:
	{
		AddSkillElement(sITEMINFO_POISON);

		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d-%d", VenomSpear_Damage[iSkillLevelZeroBased][0], VenomSpear_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, VenomSpear_Damage[NextLevel][0], VenomSpear_Damage[NextLevel][1]);
		AddSkillBoxInfo("Número de Alvos:", "%d", VenomSpear_Num[iSkillLevelZeroBased], iSkillLevelZeroBased, VenomSpear_Num[NextLevel]);
		AddSkillBoxInfo("Duração do Veneno:", "%d seconds", VenomSpear_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, VenomSpear_Time[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", VenomSpear_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, VenomSpear_Range[NextLevel]);
		break;
	}

	case SKILL_VANISH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Vanish_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Vanish_Damage[NextLevel]);
		AddSkillBoxInfo("Visibilidade dos Monstros:", "%d%%", Vanish_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Vanish_Range[NextLevel]);
		AddSkillBoxInfoDuration(Vanish_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Vanish_Time[NextLevel]);
		break;
	}

	case SKILL_CRITICAL_MASTERY:
	{
		AddSkillBoxInfo("Taxa de Crítico Adicional:", "%d%%", Critical_Mastery_Critical[iSkillLevelZeroBased], iSkillLevelZeroBased, Critical_Mastery_Critical[NextLevel]);
		break;
	}

	case SKILL_CHAIN_LANCE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Chain_Lance_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Chain_Lance_Damage[NextLevel]);
		break;
	}

	//T4
	case SKILL_ASSASSIN_EYE:
	{
		AddSkillBoxInfo("Crítico Adicional:", "%d", Assassin_Eye_AddCritical[iSkillLevelZeroBased], iSkillLevelZeroBased, Assassin_Eye_AddCritical[NextLevel]);
		AddSkillBoxInfo("Taxa de Crítico Adicional:", "%d", Assassin_Eye_M_SubCritical[iSkillLevelZeroBased], iSkillLevelZeroBased, Assassin_Eye_M_SubCritical[NextLevel]);
		AddSkillBoxInfoDuration(Assassin_Eye_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Assassin_Eye_Time[NextLevel]);
		break;
	}

	case SKILL_CHARGING_STRIKE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Charging_Strike_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Charging_Strike_DamagePercent[NextLevel]);

		AddSkillBoxGroupTitle("Modo de Recarga", D3DCOLOR_XRGB(238, 210, 151));
		AddSkillBoxInfo("Velocidade de Carregamento:", "%d", Charging_Strike_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Charging_Strike_Time[NextLevel]);
		AddSkillBoxInfo("Dano Extra Quando Carregado:", "%d%%", Charging_Strike_CharingDamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Charging_Strike_CharingDamagePercent[NextLevel]);
		break;
	}

	case SKILL_VAGUE:
	{
		AddSkillBoxInfo("Evasão Adicional:", "+%d%%", Vague_EvasionPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Vague_EvasionPercent[NextLevel]);
		AddSkillBoxInfoDuration(Vague_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Vague_Time[NextLevel]);
		break;
	}

	case SKILL_SHADOW_MASTER:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Shadow_Master_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Shadow_Master_DamagePercent[NextLevel]);
		AddSkillBoxInfo("Número de Ataques:", "%d", Shadow_Master_ShadowNum[iSkillLevelZeroBased], iSkillLevelZeroBased, Shadow_Master_ShadowNum[NextLevel]);
		//AddSkillBoxInfo("Crítico chance:", "100%"); // Verificar se é 100% mesmo
		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_ATA(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;

	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
	case SKILL_SHIELD_STRIKE:
	{
		AddSkillElement(sITEMINFO_BIONIC);

		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d - %d + (LV/5)", S_Strike_Damage[iSkillLevelZeroBased][0], S_Strike_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, S_Strike_Damage[NextLevel][0], S_Strike_Damage[NextLevel][1]);
		break;
	}

	case SKILL_FARINA:
	{
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d + LV", Farina_AttackRate[iSkillLevelZeroBased], iSkillLevelZeroBased, Farina_AttackRate[NextLevel]);
		AddSkillBoxInfo("Vel. Ataque Adicional:", "%d", Farina_Speed[iSkillLevelZeroBased], iSkillLevelZeroBased, Farina_Speed[NextLevel]);
		break;
	}

	case SKILL_THROWING_MASTERY:
	{
		AddSkillBoxInfo("Adicional de Dano na Arma to AP:", "%d%%", T_Mastery_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, T_Mastery_Damage[NextLevel]);
		break;
	}

	case SKILL_VIGOR_SPEAR:
	{
		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d%%", V_Spear_Damage[iSkillLevelZeroBased][0], V_Spear_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, V_Spear_Damage[NextLevel][0], V_Spear_Damage[NextLevel][1]);
		break;
	}
	case SKILL_WINDY:
	{
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Windy_Attack_Rating[iSkillLevelZeroBased], iSkillLevelZeroBased, Windy_Attack_Rating[NextLevel]);
		AddSkillBoxInfoDuration(Windy_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Windy_Time[NextLevel]);
		break;
	}

	case SKILL_TWIST_JAVELIN:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Twist_Javelin_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Twist_Javelin_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Twist_Javelin_Attack_Rating[iSkillLevelZeroBased], iSkillLevelZeroBased, Twist_Javelin_Attack_Rating[NextLevel]);
		break;
	}


	case SKILL_SOUL_SUCKER:
	{
		AddSkillBoxInfo("Life Absorption:", "%d%%", Soul_Sucker_Absorb[iSkillLevelZeroBased], iSkillLevelZeroBased, Soul_Sucker_Absorb[NextLevel]);
		break;
	}

	case SKILL_FIRE_JAVELIN:
	{
		AddSkillElement(sITEMINFO_FIRE);
		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d-%d + (LV/3)", Fire_Javelin_Damage[iSkillLevelZeroBased][0], Fire_Javelin_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Fire_Javelin_Damage[NextLevel][0], Fire_Javelin_Damage[NextLevel][1]);
		break;
	}

	//t3
	case SKILL_SPLIT_JAVELIN:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Split_Javelin_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Split_Javelin_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Split_Javelin_AttackRate[iSkillLevelZeroBased], iSkillLevelZeroBased, Split_Javelin_AttackRate[NextLevel]);
		AddSkillBoxInfo("Number of attacks", "%d", Split_Javelin_AttackNum[iSkillLevelZeroBased], iSkillLevelZeroBased, Split_Javelin_AttackNum[NextLevel]);
		break;
	}

	case SKILL_TRIUMPH_OF_VALHALLA:
	{
		//half for non-ata
		AddSkillBoxInfo("Poder de Ataque Adicional Máximo:", "%d%%", T_Of_Valhalla_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, T_Of_Valhalla_Damage[NextLevel], TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_ATALANTA ? 1.0f : 0.5f);

		AddSkillBoxInfoDuration(T_Of_Valhalla_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, T_Of_Valhalla_Time[NextLevel]);

		break;
	}

	case SKILL_LIGHTNING_JAVELIN:
	{
		AddSkillElement(sITEMINFO_LIGHTING);

		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d-%d + (LV/3)", Lightning_Javelin_Damage[iSkillLevelZeroBased][0], Lightning_Javelin_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Lightning_Javelin_Damage[NextLevel][0], Lightning_Javelin_Damage[NextLevel][1]);
		break;
	}

	case SKILL_STORM_JAVELIN:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Storm_Javelin_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Storm_Javelin_Damage[NextLevel]);
		break;
	}
	case SKILL_HALL_OF_VALHALLA:
	{
		AddSkillBoxInfo("Aumento na Evasão:", "+%d%%", Hall_Of_Valhalla_AddEvation[iSkillLevelZeroBased], iSkillLevelZeroBased, Hall_Of_Valhalla_AddEvation[NextLevel], TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_ATALANTA ? 1.0f : 0.5f);

		AddSkillBoxInfoDuration(Hall_Of_Valhalla_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Hall_Of_Valhalla_Time[NextLevel]);

		break;
	}

	case SKILL_X_RAGE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", X_Rage_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, X_Rage_DamagePercent[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", X_Rage_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, X_Rage_Area[iSkillLevelZeroBased]);
		break;
	}

	case SKILL_FROST_JAVELIN:
	{
		AddSkillElement(sITEMINFO_ICE);

		AddSkillBoxInfoDual("Dano de Congelamento:", "%d-%d", Frost_Javelin_IceAddDamage[iSkillLevelZeroBased][0], Frost_Javelin_IceAddDamage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Frost_Javelin_IceAddDamage[NextLevel][0], Frost_Javelin_IceAddDamage[NextLevel][1]);
		AddSkillBoxInfo("Duração do Congelamento:", "%ds", Frost_Javelin_IceTime[iSkillLevelZeroBased], iSkillLevelZeroBased, Frost_Javelin_IceTime[NextLevel]);
		AddSkillBoxInfo("Redução na Velocidade:", "%d%%", Frost_Javelin_SpeedSubPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Frost_Javelin_SpeedSubPercent[NextLevel]);
		AddSkillBoxInfoDuration(Frost_Javelin_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Frost_Javelin_Time[NextLevel]);

		break;
	}

	case SKILL_VENGEANCE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Vengeance_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Vengeance_DamagePercent[NextLevel]);
		AddSkillBoxInfo("Crítico:", "%d%%", Vengeance_AddCritical[iSkillLevelZeroBased], iSkillLevelZeroBased, Vengeance_AddCritical[NextLevel]);
		AddSkillBoxInfoRange("Range Adicional:", "%d", Vengeance_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Vengeance_Range[NextLevel]);
		break;
	}


	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_KS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;
	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
		//t1
	case SKILL_SWORD_BLAST:
	{
		AddSkillBoxInfoDual("Poder de Ataque Adicional:", "%d-%d + (LV/2)", Sword_Blast_Damage[iSkillLevelZeroBased][0], Sword_Blast_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Sword_Blast_Damage[NextLevel][0], Sword_Blast_Damage[NextLevel][1]);
		AddSkillBoxInfoRange("Distância de Ataque:", "%d", Sword_Blast_ShootingRange[iSkillLevelZeroBased], iSkillLevelZeroBased, Sword_Blast_ShootingRange[NextLevel]);
		break;
	}

	case SKILL_HOLY_BODY:
	{
		AddSkillBoxInfoRange("Bônus de Absorção:", "%d%%", HolyBody_Absorb[iSkillLevelZeroBased], iSkillLevelZeroBased, HolyBody_Absorb[NextLevel]);
		AddSkillBoxInfoDuration(HolyBody_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, HolyBody_Time[NextLevel]);
		break;
	}

	case SKILL_PHYSICAL_TRANING:
	{
		AddSkillBoxInfo("Adicional de Stamina Total:", "%d%%", P_Traning_Stamina[iSkillLevelZeroBased], iSkillLevelZeroBased, P_Traning_Stamina[NextLevel]);
		break;
	}

	case SKILL_DOUBLE_CRASH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", D_Crash_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, D_Crash_Damage[NextLevel]);
		AddSkillBoxInfo("Bônus no Crítico:", "%d%%", D_Crash_Critical[iSkillLevelZeroBased], iSkillLevelZeroBased, D_Crash_Critical[NextLevel]);
		break;
	}

	case SKILL_HOLY_VALOR:
	{
		// half for non-ks
		AddSkillBoxInfo("Dano Extra VS Morto-Vivo:", "%d%%", Holy_Valor_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Holy_Valor_Damage[NextLevel], TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_KNIGHT ? 1.0f : 0.5f);

		AddSkillBoxInfoRange("Range do Buff:", "%d", 300, iSkillLevelZeroBased, 300);

		AddSkillBoxInfoDuration(Holy_Valor_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Holy_Valor_Time[NextLevel]);
		break;
	}

	case SKILL_BRANDISH:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Brandish_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Brandish_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Brandish_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Brandish_Range[NextLevel]);
		break;
	}

	case SKILL_PIERCING:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Piercing_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Piercing_Damage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d", Piercing_Attack_Rating[iSkillLevelZeroBased], iSkillLevelZeroBased, Piercing_Attack_Rating[NextLevel]);
		break;
	}

	case SKILL_DRASTIC_SPIRIT:
	{
		AddSkillBoxInfo("Defesa Adicional:", "%d%%", Drastic_Spirit_Defense[iSkillLevelZeroBased], iSkillLevelZeroBased, Drastic_Spirit_Defense[NextLevel]);
		AddSkillBoxInfoDuration(Drastic_Spirit_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Drastic_Spirit_Time[NextLevel]);
		break;
	}

	case SKILL_SWORD_MASTERY:
	{
		AddSkillBoxInfo("Adicional de Dano na Arma", "%d%%", Sword_Mastery_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Sword_Mastery_DamagePercent[NextLevel]);
		break;
	}

	case SKILL_DIVINE_INHALATION:
	{
		AddSkillBoxInfo("Taxa de Bloqueio Adicional:", "+%d%%", D_Inhalation_Block[iSkillLevelZeroBased], iSkillLevelZeroBased, D_Inhalation_Block[NextLevel]);
		AddSkillBoxInfo("Conversão de HP:", "%d%%", D_Inhalation_Life[iSkillLevelZeroBased], iSkillLevelZeroBased, D_Inhalation_Life[NextLevel]);
		AddSkillBoxInfoDuration(D_Inhalation_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, D_Inhalation_Time[NextLevel]);
		break;
	}

	case SKILL_HOLY_INCANTATION:
	{
		AddSkillBoxInfo("Taxa de Sucesso:", "%d%%", H_Incantation_Success[iSkillLevelZeroBased], iSkillLevelZeroBased, H_Incantation_Success[NextLevel]);
		AddSkillBoxInfo("Vida Adicional:", "%d%%", H_Incantation_AddLife[iSkillLevelZeroBased], iSkillLevelZeroBased, H_Incantation_AddLife[NextLevel]);
		AddSkillBoxInfoDuration(H_Incantation_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, H_Incantation_Time[NextLevel]);
		break;
	}

	case SKILL_GRAND_CROSS:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Grand_Cross_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Grand_Cross_Damage[NextLevel]);
		AddSkillBoxInfo("Dano Extra VS Morto-Vivo:", "%d%%", Grand_Cross_UndeadDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, Grand_Cross_UndeadDamage[NextLevel]);
		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d%%", Grand_Cross_AttackRate[iSkillLevelZeroBased], iSkillLevelZeroBased, Grand_Cross_AttackRate[NextLevel]);
		break;
	}
	case SKILL_SWORD_OF_JUSTICE:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Sword_Of_Justice_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Sword_Of_Justice_DamagePercent[NextLevel]);
		AddSkillBoxInfoRange("Distância de Ataque", "%d", Sword_Of_Justice_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Sword_Of_Justice_Area[NextLevel]);
		AddSkillBoxInfo("Área de Ataque", "80");
		break;
	}

	case SKILL_GODLY_SHIELD:
	{
		AddSkillBoxInfo("Shield Bônus de Absorção:", "%d%%", Godly_Shield_AbsorbPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Godly_Shield_AbsorbPercent[NextLevel]);
		AddSkillBoxInfoDuration(Godly_Shield_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Godly_Shield_Time[NextLevel]);
		break;
	}

	case SKILL_GOD_BLESS:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", God_Bless_AddDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, God_Bless_AddDamage[NextLevel]);
		AddSkillBoxInfoDuration(God_Bless_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, God_Bless_Time[NextLevel]);
		break;
	}

	case SKILL_DIVINE_PIERCING:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Divine_Piercing_DamagePercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Divine_Piercing_DamagePercent[NextLevel]);
		AddSkillBoxInfo("Bônus no Crítico:", "%d%%", Divine_Piercing_AddHit[iSkillLevelZeroBased], iSkillLevelZeroBased, Divine_Piercing_AddHit[NextLevel]);
		AddSkillBoxInfo("Quantidade de Hits:", "%d", Divine_Piercing_AttackNum[iSkillLevelZeroBased], iSkillLevelZeroBased, Divine_Piercing_AttackNum[NextLevel]);
		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_PRS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;
	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
		//tier 1
	case SKILL_HEALING:
	{
		AddSkillBoxInfoDual("Poder de Cura:", "%d-%d", Healing_Heal[iSkillLevelZeroBased][0], Healing_Heal[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Healing_Heal[NextLevel][0], Healing_Heal[NextLevel][1]);
		break;
	}

	case SKILL_HOLY_BOLT:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d", HolyBolt_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, HolyBolt_Damage[NextLevel]);
		break;
	}

	case SKILL_MULTISPARK:
	{
		AddSkillBoxInfo("Dano por Hit: ", "%d%%d", M_Spark_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, M_Spark_Damage[NextLevel]);
		AddSkillBoxInfoDual("Número de Faíscas: ", "%d - %d", (M_Spark_Num[iSkillLevelZeroBased] / 2) + 1, M_Spark_Num[iSkillLevelZeroBased], iSkillLevelZeroBased, (M_Spark_Num[NextLevel] / 2) + 1, M_Spark_Num[NextLevel]);
		break;
	}

	case SKILL_HOLY_MIND:
	{
		AddSkillBoxInfo("Dano Reduzido:", "%d%%", HolyMind_DecDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, HolyMind_DecDamage[NextLevel]);
		AddSkillBoxInfoDuration(HolyMind_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, HolyMind_Time[NextLevel]);
		break;
	}
	case SKILL_MEDITATION:
	{
		int sinTemp1 = (int)(Meditation_Regen[iSkillLevelZeroBased] * 10);
		int RightSpot = sinTemp1 % 10;
		int LeftSpot = (sinTemp1 - RightSpot) / 10;

		int sinTemp2 = (int)(Meditation_Regen[NextLevel] * 10);
		int RightSpot2 = sinTemp1 % 10;
		int LeftSpot2 = (sinTemp1 - RightSpot2) / 10;

		AddSkillBoxInfoDual("Regeneração de MP Adicional: ", "%d.%d", LeftSpot, RightSpot, iSkillLevelZeroBased, LeftSpot2, RightSpot2);
		break;
	}

	case SKILL_DIVINE_LIGHTNING:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Divine_Lightning_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Divine_Lightning_Damage[NextLevel]);
		AddSkillBoxInfo("Número de Raios:", "%d", Divine_Lightning_Num[iSkillLevelZeroBased], iSkillLevelZeroBased, Divine_Lightning_Num[NextLevel]);
		break;
	}

	case SKILL_HOLY_REFLECTION:
	{
		AddSkillBoxInfo("Dano Refletido:", "%d%%", Holy_Reflection_Return_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Holy_Reflection_Return_Damage[NextLevel]);
		AddSkillBoxInfoDuration(Holy_Reflection_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Holy_Reflection_Time[NextLevel]);
		break;
	}

	case SKILL_GRAND_HEALING:
	{
		AddSkillBoxInfoDual("Poder de Cura:", "%d-%d", Grand_Healing[iSkillLevelZeroBased][0], Grand_Healing[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Grand_Healing[NextLevel][0], Grand_Healing[NextLevel][1]);
		AddSkillBoxInfoRange("Range:", "%d", Grand_Healing_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Grand_Healing_Range[NextLevel]);
		break;
	}

	case SKILL_VIGOR_BALL:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Vigor_Ball_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Vigor_Ball_Damage[NextLevel]);
		break;
	}

	case SKILL_RESURRECTION:
	{
		AddSkillBoxInfo("Taxa de Sucesso:", "%d%%", Resurrection_Percent[iSkillLevelZeroBased], iSkillLevelZeroBased, Resurrection_Percent[NextLevel]);

		GAMECOREHANDLE->pcSkillInfoBox->AddString(0, "Range:", D3DCOLOR_XRGB(243, 237, 216));
		GAMECOREHANDLE->pcSkillInfoBox->AddString(1, FormatString("%d [%.1fm]", 300, static_cast<float>(300) / 27.0f), D3DCOLOR_XRGB(238, 210, 151));
		break;
	}

	case SKILL_EXTINCTION:
	{
		AddSkillBoxInfo("Taxa de Sucesso:", "%d + (LV/5)%%", Extinction_Percent[iSkillLevelZeroBased], iSkillLevelZeroBased, Extinction_Percent[NextLevel]);
		AddSkillBoxInfo("Redução de HP:", "%d%%", Extinction_Amount[iSkillLevelZeroBased], iSkillLevelZeroBased, Extinction_Amount[NextLevel]);
		break;
	}

	case SKILL_VIRTUAL_LIFE:
	{
		AddSkillBoxInfo("HP Extra:", "%d%%", Virtual_Life_Percent[iSkillLevelZeroBased], iSkillLevelZeroBased, Virtual_Life_Percent[NextLevel], TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS ? 1.0f : 0.5f);
		AddSkillBoxInfoDuration(Virtual_Life_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Virtual_Life_Time[NextLevel]);

		return TRUE;
	}
	case SKILL_GLACIAL_SPIKE:
	{
		AddSkillElement(sITEMINFO_ICE);

		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Glacial_Spike_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Glacial_Spike_Damage[NextLevel]);
		AddSkillBoxInfoRange("Distância de Ataque:", "%d", Glacial_Spike_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Glacial_Spike_Area[NextLevel]);
		break;
	}

	case SKILL_REGENERATION_FIELD:
	{
		int TempLife = (int)(Regeneration_Field_LifeRegen[iSkillLevelZeroBased] * 10 + 0.001f);
		int RightSpot = TempLife % 10;
		int LeftSpot = (TempLife - RightSpot) / 10;

		int TempLife2 = (int)(Regeneration_Field_LifeRegen[NextLevel] * 10 + 0.001f);
		int RightSpot2 = TempLife % 10;
		int LeftSpot2 = (TempLife - RightSpot2) / 10;

		//half for non-prs
		AddSkillBoxInfoDual("Regeneração de HP:", "%d-%d", LeftSpot, RightSpot, iSkillLevelZeroBased, LeftSpot2, RightSpot2, TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS ? 1.0f : 0.5f);

		int TempMana = (int)(Regeneration_Field_ManaRegen[NextLevel] * 10 + 0.001f);
		RightSpot = TempMana % 10;
		LeftSpot = (TempMana - RightSpot) / 10;

		RightSpot2 = TempMana % 10;
		LeftSpot2 = (TempMana - RightSpot2) / 10;

		AddSkillBoxInfoDual("Regeneração de MP:", "%d-%d", LeftSpot, RightSpot, iSkillLevelZeroBased, LeftSpot2, RightSpot2, TRUE,
			UNITDATA->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS ? 1.0f : 0.5f);

		AddSkillBoxInfoRange("Range: ", "%d", Regeneration_Field_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Regeneration_Field_Area[NextLevel]);
		AddSkillBoxInfoDuration(Regeneration_Field_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Regeneration_Field_Time[NextLevel]);
		break;
	}

	case SKILL_CHAIN_LIGHTNING:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Chain_Lightning_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Chain_Lightning_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Chain_Lightning_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Chain_Lightning_Range[NextLevel]);

		break;
	}

	case SKILL_SUMMON_MUSPELL:
	{
		AddSkillBoxInfoDual("Muspell Dano Extra:", "%d-%d", Summon_Muspell_Damage[iSkillLevelZeroBased][0], Summon_Muspell_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Summon_Muspell_Damage[NextLevel][0], Summon_Muspell_Damage[NextLevel][1]);
		AddSkillBoxInfo("Muspell Delay de Ataque:", "%d", Summon_Muspell_Attack_Delay[iSkillLevelZeroBased], iSkillLevelZeroBased, Summon_Muspell_Attack_Delay[NextLevel]);

		AddSkillBoxEmptyLine();
		AddSkillBoxInfo("Absorção Extra Contra Mortos-Vivos", "%d%%", Summon_Muspell_UndeadAbsorbPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Summon_Muspell_UndeadAbsorbPercent[NextLevel]);
		AddSkillBoxInfo("Porcentagem de Bloqueio", "%d%%", Summon_Muspell_BlockPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Summon_Muspell_BlockPercent[NextLevel]);

		AddSkillBoxEmptyLine();
		AddSkillBoxInfoDuration(iSkillLevelZeroBased, Summon_Muspell_Time[iSkillLevelZeroBased], Summon_Muspell_Time[NextLevel]);
		return TRUE;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}

BOOL SkillManager::AddSkillInfoToBox_MGS(int iSkillID, int iSkillLevelZeroBased)
{
	BOOL bAdded = TRUE;
	int NextLevel = iSkillLevelZeroBased < 9 ? iSkillLevelZeroBased + 1 : iSkillLevelZeroBased;

	switch (iSkillID)
	{
	case SKILL_AGONY:
	{
		AddSkillBoxInfo("Taxa de Conversão de HP:", "%d%%", Agony_ConvLife[iSkillLevelZeroBased], iSkillLevelZeroBased, Agony_ConvLife[NextLevel]);
		break;
	}
	case SKILL_FIRE_BOLT:
	{
		AddSkillElement(sITEMINFO_FIRE);

		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d", FireBolt_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, FireBolt_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range: ", "%d", 200, iSkillLevelZeroBased, 200);
		break;
	}
	case SKILL_ZENITH:
	{
		AddSkillBoxInfo("Add Elemental Resistance:", "%d", Zenith_Element[iSkillLevelZeroBased], iSkillLevelZeroBased, Zenith_Element[NextLevel]);
		AddSkillBoxInfoDuration(Zenith_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Zenith_Time[NextLevel]);
		break;
	}
	case SKILL_FIRE_BALL:
	{
		AddSkillElement(sITEMINFO_FIRE);
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", FireBall_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, FireBall_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", FireBall_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, FireBall_Area[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", FireBall_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, FireBall_Range[NextLevel]);
		break;
	}
	case SKILL_MENTAL_MASTERY:
	{
		AddSkillBoxInfo("Mana Total Boost:", "%d%%", Mental_Mastery_Mana[iSkillLevelZeroBased], iSkillLevelZeroBased, Mental_Mastery_Mana[NextLevel]);
		break;
	}

	case SKILL_WATORNADO:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Watornado_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Watornado_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Watornado_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Watornado_Area[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Watornado_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Watornado_Range[NextLevel]);
		break;
	}

	case SKILL_ENCHANT_WEAPON:
	{
		AddSkillBoxInfoDuration(Enchant_Weapon_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Enchant_Weapon_Time[NextLevel]);

		AddSkillBoxGroupTitle("Encantamento de Fogo (Incendiar)", TEXTCOLOR_SKILLCHAIN, sITEMINFO_FIRE);
		AddSkillBoxInfoDual("Dano de Fogo:", "%d-%d", Enchant_Weapon_Damage_Fire[iSkillLevelZeroBased][0], Enchant_Weapon_Damage_Fire[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Enchant_Weapon_Damage_Fire[NextLevel][0], Enchant_Weapon_Damage_Fire[NextLevel][1]);

		AddSkillBoxGroupTitle("Encantamento de Gelo (Congelamento)", TEXTCOLOR_SKILLCHAIN, sITEMINFO_ICE);
		AddSkillBoxInfoDual("Dano de Congelamento:", "%d-%d", Enchant_Weapon_Damage_Ice[iSkillLevelZeroBased][0], Enchant_Weapon_Damage_Ice[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Enchant_Weapon_Damage_Ice[NextLevel][0], Enchant_Weapon_Damage_Ice[NextLevel][1]);

		AddSkillBoxGroupTitle("Encantamento Relâmpago (Atordoamento)", TEXTCOLOR_SKILLCHAIN, sITEMINFO_LIGHTING);
		AddSkillBoxInfoDual("Dano de Atordoamento:", "%d-%d", Enchant_Weapon_Damage_Lightning[iSkillLevelZeroBased][0], Enchant_Weapon_Damage_Lightning[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Enchant_Weapon_Damage_Lightning[NextLevel][0], Enchant_Weapon_Damage_Lightning[NextLevel][1]);

		AddSkillBoxInfoRange("Distância de Ataque:", "%d", Enchant_Weapon_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Enchant_Weapon_Time[NextLevel]);
		break;
	}

	case SKILL_DEAD_RAY:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Dead_Ray_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Dead_Ray_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Dead_Ray_Range[iSkillLevelZeroBased], iSkillLevelZeroBased, Dead_Ray_Range[NextLevel]);
		break;
	}

	case SKILL_ENERGY_SHIELD:
	{
		AddSkillBoxInfo("Redução no Dano:", "%d%%", Energy_Shield_DecDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, Energy_Shield_DecDamage[NextLevel]);
		AddSkillBoxInfoDuration(Energy_Shield_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Energy_Shield_Time[NextLevel]);
		break;
	}

	case SKILL_DIASTROPHISM:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Diastrophism_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Diastrophism_Damage[NextLevel]);
		AddSkillBoxInfoRange("Range:", "%d", Diastrophism_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Diastrophism_Area[NextLevel]);
		break;
	}

	case SKILL_SPIRIT_ELEMENTAL:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%% - %d%%", Spirit_Elemental_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Spirit_Elemental_Damage[NextLevel]);
		AddSkillBoxInfoDuration(Spirit_Elemental_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Spirit_Elemental_Time[NextLevel]);
		break;
	}

	case SKILL_DANCING_SWORD:
	{
		AddSkillBoxInfoDuration(Dancing_Sword_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Dancing_Sword_Time[NextLevel]);

		AddSkillBoxGroupTitle(FormatString("Fire Dancing Sword"), TEXTCOLOR_SKILLCHAIN, sITEMINFO_FIRE);
		AddSkillBoxInfoDual("Burn Damage:", "%d-%d", Dancing_Sword_FireDamage[iSkillLevelZeroBased][0], Dancing_Sword_FireDamage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Dancing_Sword_FireDamage[NextLevel][0], Dancing_Sword_FireDamage[NextLevel][1]);

		AddSkillBoxGroupTitle(FormatString("Ice Dancing Sword"), TEXTCOLOR_SKILLCHAIN, sITEMINFO_ICE);
		AddSkillBoxInfoDual("Freeze Damage:", "%d-%d", Dancing_Sword_IceDamage[iSkillLevelZeroBased][0], Dancing_Sword_IceDamage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Dancing_Sword_IceDamage[NextLevel][0], Dancing_Sword_FireDamage[NextLevel][1]);

		AddSkillBoxInfo("Delay de Ataque:", "%d", Dancing_Sword_AttackDelay[iSkillLevelZeroBased], iSkillLevelZeroBased, Dancing_Sword_AttackDelay[NextLevel]);
		break;
	}
	case SKILL_FIRE_ELEMENTAL:
	{
		AddSkillElement(sITEMINFO_FIRE);

		AddSkillBoxInfoDual("Poder de Ataque:", "%d - %d", Fire_Elemental_Damage[iSkillLevelZeroBased][0], Fire_Elemental_Damage[iSkillLevelZeroBased][1], iSkillLevelZeroBased, Fire_Elemental_Damage[NextLevel][0], Fire_Elemental_Damage[NextLevel][1]);

		AddSkillBoxInfo("Taxa de Ataque Adicional:", "%d", Fire_Elemental_Hit[iSkillLevelZeroBased], iSkillLevelZeroBased, Fire_Elemental_Hit[NextLevel]);
		AddSkillBoxInfo("Vida:", "%d", Fire_Elemental_Life[iSkillLevelZeroBased], iSkillLevelZeroBased, Fire_Elemental_Life[NextLevel]);

		AddSkillBoxInfoDuration(360, iSkillLevelZeroBased, 360);

		if (!GAMECOREHANDLE->pcSkillInfoBox->IsViewingSkillTimer())
		{
			AddSkillBoxGroupTitle("Status Padrão do PET", D3DCOLOR_XRGB(238, 210, 151));
			AddSkillBoxInfo("Modo:", baPetAggressive[1] ? "Automático" : "Manual");
		}
		else
		{
			AddSkillBoxGroupTitle("Status do PET", D3DCOLOR_XRGB(238, 210, 151));
			AddSkillBoxInfo("Modo:", baPetAggressive[1] ? "Automático" : "Manual");
		}

		break;
	}

	case SKILL_FLAME_WAVE:
	{
		AddSkillElement(sITEMINFO_FIRE);

		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Flame_Wave_FireDamage[iSkillLevelZeroBased], iSkillLevelZeroBased, Flame_Wave_FireDamage[NextLevel]);

		AddSkillBoxInfoRange("Range:", "%d", Flame_Wave_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Flame_Wave_Area[NextLevel]);
		break;
	}

	case SKILL_DISTORTION:
	{
		AddSkillElement(sITEMINFO_BIONIC);

		AddSkillBoxInfo("Redução no Dano:", "%d%%", Distortion_DamageSubPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Distortion_DamageSubPercent[NextLevel]);
		AddSkillBoxInfo("Redução na Velocidade:", "%d%%", Distortion_SpeedSubPercent[iSkillLevelZeroBased], iSkillLevelZeroBased, Distortion_SpeedSubPercent[NextLevel]);

		AddSkillBoxInfoRange("Range:", "%d", Distortion_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Distortion_Area[NextLevel]);
		AddSkillBoxInfoDuration(Distortion_Time[iSkillLevelZeroBased], iSkillLevelZeroBased, Distortion_Time[NextLevel]);
		break;
	}

	case SKILL_M_METEO:
	{
		AddSkillBoxInfo("Poder de Ataque Adicional:", "%d%%", Meteo_Damage[iSkillLevelZeroBased], iSkillLevelZeroBased, Meteo_Damage[NextLevel]);

		AddSkillBoxInfoRange("Range:", "%d", Meteo_Area[iSkillLevelZeroBased], iSkillLevelZeroBased, Meteo_Area[NextLevel]);
		break;
	}

	default:
	{
		bAdded = FALSE;
		break;
	}

	}

	return bAdded;
}