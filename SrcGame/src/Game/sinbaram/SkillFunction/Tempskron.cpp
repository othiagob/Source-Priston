#include "..\\sinLinkHeader.h"
#include "..\\..\\field.h"

int CheckMana;
int CheckStamina; 
int CheckMastery;

int NotUseSkillElement[3] = { 0,0,0 };

int sinMasterSkill_Money[16] = { 500,1000,1500,2000,3000,5000,7000,9000,10000,12000,15000,20000,25000,30000,35000,40000 };
int sinMasterSkill_PlusMoney[16] = { 500,600,700,800,1000,1200,1400,1600,2000,2500,3000,4000,5000,6000,7000,8000 };


#define GROUP_MECHANICIAN		0x01000000
#define GROUP_FIGHTER			0x02000000
#define GROUP_PIKEMAN			0x03000000
#define	GROUP_ARCHER			0x04000000

int sinMasteryIncreaIndex[16] = { 5,5,5,5,7,7,7,7,9,9,9,9,14,15,16,17 };
int ContinueSkillCancelFlag = 0;


int SkillTextColor[10] = { 0,0,0,0,0,0,0,0,0,0 };

int CheckContinueSkillUse(sSKILL* pSkill)
{
	for (int i = 0; i < 18; i++) {
		if (ContinueSkill[i].Flag && ContinueSkill[i].CODE == pSkill->CODE) {
			return FALSE;
		}
	}
	return TRUE;
}

int ContinueSkillUse(sSKILL* pSkill, int Time)
{
	for (int i = 0; i < 18; i++) {
		if (!ContinueSkill[i].Flag) {
			memcpy(&ContinueSkill[i], pSkill, sizeof(sSKILL));
			ContinueSkill[i].Flag = 1;
			ContinueSkill[i].UseTime = Time;
			return i + 1;
		}
	}
	return FALSE;
}


DWORD sinSkillswitchCODE[9][9] = {
	{SKILL_PHYSICAL_ABSORB,SKILL_METAL_ARMOR,0},
	{SKILL_EXTREME_SHIELD,SKILL_SPARK_SHIELD,0},
	//SKILL FS USAR JUNTO UPDATE 1034
	/*{SKILL_CONCENTRATION,SKILL_SWIFT_AXE,0},*/
	{SKILL_FALCON,SKILL_GOLDEN_FALCON,SKILL_SCOUT_HAWK,0},
	{SKILL_DIVINE_INHALATION,SKILL_GODLY_SHIELD,0},
	{STONE_R_FIRECRYTAL,STONE_R_ICECRYTAL,STONE_R_LINGHTINGCRYTAL,0},
	{STONE_A_FIGHTER,STONE_A_MECHANICIAN,STONE_A_PIKEMAN,STONE_A_ARCHER,STONE_A_KNIGHT,STONE_A_ATALANTA,STONE_A_MAGICIAN,STONE_A_PRIESTESS,0},
	0 };

void SwitchSkill(sSKILL* pSkill)
{
	int CodeLine = 0;
	for (int i = 0; i < 18; i++) {
		for (int j = 0; j < 18; j++) {
			if (sinSkillswitchCODE[i][j]) {
				if (sinSkillswitchCODE[i][j] == pSkill->CODE) {
					CodeLine = i + 1;
					break;
				}
			}
		}
	}

	if (CodeLine) {
		for (int k = 0; k < 18; k++) {
			if (ContinueSkill[k].Flag) {
				for (int t = 0; t < 18; t++) {
					if (ContinueSkill[k].CODE == sinSkillswitchCODE[CodeLine - 1][t]) {
						if (sinSkillswitchCODE[CodeLine - 1][t] == pSkill->CODE)continue;
						ContinueSkill[k].Flag = 0; ContinueSkill[k].UseTime = 0; ContinueSkill[k].CheckTime = 0;
						ContinueSkill[k].IconFlag = 0;
						ContinueSkill[k].IconPosiX = 0;
						ContinueSkill[k].IconTime = 0;
						ContinueSkill[k].IconAlpha = 0;
						ContinueSkill[k].MatIcon = 0;

						cInvenTory.SetItemToChar();
						SendCancelSkillToServer(sinSkillswitchCODE[CodeLine - 1][t], ContinueSkill[k].Point, 0, 0);
						break;
					}
				}
			}
		}
	}
}

void CheckContinueSkill()
{
	int Cancel = 0;
	for (int i = 0; i < 18; i++) {
		if (ContinueSkill[i].Flag)
		{
			if (sinGetLife() == 0 && (ContinueSkill[i].CODE != SKILL_FORCE_ORB && ContinueSkill[i].CODE != BUFF_WARMODE && ContinueSkill[i].CODE != CHANGE_JOB3_QUEST && ContinueSkill[i].CODE != CHANGE_ELEMENTARY_QUEST && ContinueSkill[i].CODE != CLANSKILL_ATTACK && ContinueSkill[i].CODE != CLANSKILL_EVASION && ContinueSkill[i].CODE != CLANSKILL_ABSORB &&
				ContinueSkill[i].CODE != SCROLL_P_CRITICAL && ContinueSkill[i].CODE != SCROLL_P_EVASION &&
				ContinueSkill[i].CODE != BOOSTER_ITEM_LIFE && ContinueSkill[i].CODE != SODSKILL_GOLD && ContinueSkill[i].CODE != SODSKILL_SILVER
				&& ContinueSkill[i].CODE != SODSKILL_BRONZE && ContinueSkill[i].CODE != BOOSTER_ITEM_MANA
				&& ContinueSkill[i].CODE != BOOSTER_ITEM_STAMINA
				&& ContinueSkill[i].CODE != CHATGLOBAL && ContinueSkill[i].CODE != REIPVP && ContinueSkill[i].CODE != TOPLVL))
			{
					ContinueSkill[i].CheckTime = ContinueSkill[i].UseTime * 70;
			}


			if (ContinueSkill[i].CODE != SKILL_HOLY_VALOR && ContinueSkill[i].CODE != BUFF_WARMODE && ContinueSkill[i].CODE != SKILL_DRASTIC_SPIRIT && SKILL_FORCE_ORB != ContinueSkill[i].CODE
				&& CHANGE_JOB3_QUEST != ContinueSkill[i].CODE && ContinueSkill[i].CODE != CHANGE_ELEMENTARY_QUEST && ContinueSkill[i].CODE != CLANSKILL_ATTACK && ContinueSkill[i].CODE != CLANSKILL_EVASION && ContinueSkill[i].CODE != CLANSKILL_ABSORB &&
				ContinueSkill[i].CODE != SCROLL_P_CRITICAL && ContinueSkill[i].CODE != SODSKILL_GOLD && ContinueSkill[i].CODE != SODSKILL_SILVER
				&& ContinueSkill[i].CODE != SODSKILL_BRONZE && ContinueSkill[i].CODE != SCROLL_P_EVASION &&
				ContinueSkill[i].CODE != BOOSTER_ITEM_LIFE && ContinueSkill[i].CODE != BOOSTER_ITEM_MANA
				&& ContinueSkill[i].CODE != CHATGLOBAL && ContinueSkill[i].CODE != REIPVP && ContinueSkill[i].CODE != TOPLVL
				&& ContinueSkill[i].CODE != BOOSTER_ITEM_STAMINA && (ContinueSkill[i].CODE <= 20))
			{
				if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
				{
					if (ContinueSkill[i].CODE == SKILL_VIRTUAL_LIFE)sinVirtualLifeTime = 0;

					ContinueSkill[i].CheckTime = ContinueSkill[i].UseTime * 70;
				}
			}

			ContinueSkill[i].CheckTime++;

			//if (ContinueSkill[i].CODE == SKILL_FORCE_ORB && lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_VILLAGE)
			//{
			//	ContinueSkill[i].CheckTime--;
			//}

			if (ContinueSkill[i].CODE == CLANSKILL_ABSORB || ContinueSkill[i].CODE == CLANSKILL_ATTACK || ContinueSkill[i].CODE == CLANSKILL_EVASION || ContinueSkill[i].CODE == CHANGE_ELEMENTARY_QUEST
				|| ContinueSkill[i].CODE == SODSKILL_GOLD || ContinueSkill[i].CODE == SODSKILL_SILVER || ContinueSkill[i].CODE == SODSKILL_BRONZE)
			{
				ContinueSkill[i].CheckTime--;
			}
			if ((ContinueSkill[i].UseTime * 70) < ContinueSkill[i].CheckTime)
			{

				// Limite de cristais aqui
				//if (ContinueSkill[i].CODE >= 1 && ContinueSkill[i].CODE <= 20) 
				//{
				//	extern int limitebuff;
				//	limitebuff--;
				//}

				if (ContinueSkill[i].CODE == CHANGE_JOB3_QUEST)
				{
					if (sinQuest_ChangeJob4.Monster[1])
						sinQuest_ChangeJob4.State--;

					sinQuest_ChangeJob4.Monster[0] = 0;
					sinQuest_ChangeJob4.Kind = 0;
					SetQuestBoard();
					cMessageBox.ShowMessage(MESSAGE_CHANGEJOB4_FAIL);
					sinPlaySound(SIN_SOUND_FAILD_MIXITEM);
				}

				ContinueSkill[i].Flag = 0;
				ContinueSkill[i].UseTime = 0;
				ContinueSkill[i].CheckTime = 0;
				ContinueSkill[i].IconFlag = 0;
				ContinueSkill[i].IconPosiX = 0;
				ContinueSkill[i].IconTime = 0;
				ContinueSkill[i].IconAlpha = 0;
				ContinueSkill[i].MatIcon = 0;

				cInvenTory.SetItemToChar();
				switch (ContinueSkill[i].CODE) {
				case SKILL_EXTREME_SHIELD:
					memset(&ContinueSkill[i], 0, sizeof(sSKILL));
					cInvenTory.SetItemToChar();
					break;
				case SKILL_PHYSICAL_ABSORB:
					memset(&ContinueSkill[i], 0, sizeof(sSKILL));
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_WHITE;
					break;
				case SKILL_AUTOMATION:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();

					break;
				case SKILL_MAXIMIZE:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();

					break;
				case SKILL_DRASTIC_SPIRIT:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;
					break;
				case SKILL_WINDY:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
				case SKILL_METAL_ARMOR:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;
					break;
				case SKILL_SPARK_SHIELD:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;
					break;
				case SKILL_CONCENTRATION:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
					break;
				case SKILL_SWIFT_AXE:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case SKILL_EVASION_MASTERY:
					ContinueSkill[i].PlusState[0] = 0;
					SkillTextColor[SIN2_ATTACK_RATE] = SIN_TEXT_COLOR_WHITE;
					SkillTextColor[SIN2_SPEED] = SIN_TEXT_COLOR_WHITE;
					lpCurPlayer->PlayEvasionMastery = 0;
					cInvenTory.SetItemToChar();
					break;
				case SKILL_DIVINE_INHALATION:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case SKILL_COMPULSION:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					SkillTextColor[SIN2_DEFENSE] = SIN_TEXT_COLOR_WHITE;
					break;
				case CLANSKILL_ABSORB:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case CLANSKILL_ATTACK:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case SODSKILL_GOLD:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case SODSKILL_SILVER:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case SODSKILL_BRONZE:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case CHATGLOBAL:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case REIPVP:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				case TOPLVL:
					ContinueSkill[i].PlusState[0] = 0;
					cInvenTory.SetItemToChar();
					break;
				}

				memset(&ContinueSkill[i], 0, sizeof(sSKILL));
			}
		}
	}
}


int RequireStateCheck(int Mana)
{

	if (pUseSkill)
	{
		CheckStamina = pUseSkill->Skill_Info.UseStamina[0] + (pUseSkill->Skill_Info.UseStamina[1] * (pUseSkill->Point - 1));

		if (chaPremiumitem.m_StaminaReducePotiontime > 0)
		{
			CheckStamina = CheckStamina - (CheckStamina * chaPremiumitem.m_StaminaReducePotionValue / 100);
		}
	}

	if ((sinGetMana() - Mana) >= 0 && (sinGetStamina() - CheckStamina) >= 0 && pUseSkill->UseSkillFlag) {
		return TRUE;
	}
	if ((sinGetMana() - Mana) < 0)
		NotUseSkillElement[0] = 1;
	else
		NotUseSkillElement[0] = 0;

	if ((sinGetStamina() - CheckStamina) < 0)
		NotUseSkillElement[1] = 1;
	else
		NotUseSkillElement[1] = 0;

	if (!pUseSkill->UseSkillFlag)
		NotUseSkillElement[2] = 1;
	else
		NotUseSkillElement[2] = 0;

	if ((sinGetMana() - Mana) >= 0 && (sinGetStamina() - CheckStamina) >= 0 && pUseSkill->UseSkillFlag) {
		return TRUE;
	}

	return FALSE;
}

int F_Extreme_Shield()
{
	int SkillIndex = 0;

	CheckMana = E_Shield_UseMana[pUseSkill->Point - 1];

	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (!CheckContinueSkillUse(pUseSkill))
		return FALSE;

	if (RequireStateCheck(CheckMana)) 
	{
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE & sinITEM_MASK2)) 
		{
			CheckCharForm();
			SwitchSkill(pUseSkill);
			SkillIndex = ContinueSkillUse(pUseSkill, E_Shield_UseTime[pUseSkill->Point - 1]);

			if (SkillIndex) 
				ContinueSkill[SkillIndex - 1].PlusState[0] = E_Shield_BlockRate[pUseSkill->Point - 1];

			sinSetMana(sinGetMana() - CheckMana);
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill);
			pUseSkill->Mastery = 0;
			CSKILL->ReformSkillMasteryForm(pUseSkill); 

			ReformCharForm();
			cInvenTory.SetItemToChar();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}
			SendProcessSKillToServer(SKILL_PLAY_EXTREME_SHIELD, pUseSkill->Point, 0, 0);
			return TRUE;
		}

	}
	return FALSE;
}

int F_Mechanic_Bomb()
{
	CheckMana = Mechanic_Bomb_UseMana[pUseSkill->Point - 1];

	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) 
	{
		CheckCharForm();
		sinSetMana(sinGetMana() - CheckMana);
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill);
		pUseSkill->Mastery = 0;
		CSKILL->ReformSkillMasteryForm(pUseSkill);

		ReformCharForm();
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

	}

	return FALSE;
}

int F_Physical_Absorption()
{//피지컬 업솝
	int SkillIndex = 0;

	CheckMana = P_Absor_bUseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (!CheckContinueSkillUse(pUseSkill))return FALSE; //현재 스킬을 사용중이다 

	if (RequireStateCheck(CheckMana)) {
		SwitchSkill(pUseSkill);
		CheckCharForm();
		SkillIndex = ContinueSkillUse(pUseSkill, P_Absorb_UseTime[pUseSkill->Point - 1]);
		if (SkillIndex) {
			ContinueSkill[SkillIndex - 1].PlusState[0] = GetRandomPos(P_Absorb[pUseSkill->Point - 1][0], P_Absorb[pUseSkill->Point - 1][1]);


		}

		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

		ReformCharForm();
		/////// 캐릭터 수치를 변경한다 
		cInvenTory.SetItemToChar();
		SkillTextColor[SIN2_ABSORB] = SIN_TEXT_COLOR_BLUE;
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

	}

	return FALSE;
}
//pUseSkill
int F_Poison_Attribute()
{//포이즌 어트리뷰트 

	/////// 캐릭터 수치를 변경한다 
	cInvenTory.SetItemToChar();

	return TRUE;
}

int F_Great_Smash()
{//그래이트 스매쉬 

	CheckMana = G_Smash_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}
				return TRUE;
			}
		}
	}

	return FALSE;

}
int F_Maximize()
{//맥시마이즈   
	int SkillIndex = 0;
	CheckMana = Maximize_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (!CheckContinueSkillUse(pUseSkill))return FALSE; //현재 스킬을 사용중이다 
	if (RequireStateCheck(CheckMana)) {
		SwitchSkill(pUseSkill);
		CheckCharForm();
		SkillIndex = ContinueSkillUse(pUseSkill, Maximize_Time[pUseSkill->Point - 1]);

		if (SkillIndex) {
			ContinueSkill[SkillIndex - 1].PlusState[0] = Maximize_Damage[pUseSkill->Point - 1];

		}

		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

		ReformCharForm();
		/////// 캐릭터 수치를 변경한다 
		cInvenTory.SetItemToChar();
		SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_YELLOW;
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

	}

	return FALSE;

}

int F_Automation()
{//오토메이션 

	int SkillIndex = 0;
	CheckMana = Automation_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (!CheckContinueSkillUse(pUseSkill))return FALSE; //현재 스킬을 사용중이다 
	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //

				SwitchSkill(pUseSkill);
				CheckCharForm();
				SkillIndex = ContinueSkillUse(pUseSkill, Automation_Time[pUseSkill->Point - 1]);
				if (SkillIndex) {
					ContinueSkill[SkillIndex - 1].PlusState[0] = Automation_Speed[pUseSkill->Point - 1];

				}
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

				ReformCharForm();
				/////// 캐릭터 수치를 변경한다 
				cInvenTory.SetItemToChar();
				//SkillTextColor[SIN2_SPEED] = SIN_TEXT_COLOR_GREEN;
				SkillTextColor[SIN2_DAMAGE] = SIN_TEXT_COLOR_YELLOW;
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}
				return TRUE;
			}
		}
	}
	return FALSE;
}


int F_Spark()
{//스파크 
	CheckMana = Spark_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	//요구 조건 
	if (RequireStateCheck(CheckMana)) {
		CheckCharForm();
		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

		ReformCharForm();
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

	}

	return FALSE;

}

////////////////////////////파이터
int F_Melee_Mastery()
{//밀리 마스터리 

	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_Fire_Attribute()
{//파이어 어트리뷰트 

	/////// 캐릭터 수치를 변경한다 
	cInvenTory.SetItemToChar();

	return TRUE;
}
int F_Raving()
{//레이빙 

	float CheckLife = 0;

	CheckMana = Raving_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	CheckLife = (float)((Raving_UseLife[pUseSkill->Point - 1] / 10) / sinChar->Life[1]) * 100;

	if (RequireStateCheck(CheckMana)) {
		if ((short)CheckLife <= sinGetLife()) {
			for (int i = 0; i < 8; i++) {
				if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
					CheckCharForm();
					sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
					sinSetStamina(sinGetStamina() - CheckStamina);
					sinSetLife(sinGetLife() - (short)CheckLife);      //사용될때 라이프를 감소시킨다 
					CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
					pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
					CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
					ReformCharForm();
					pUseSkill->UseSKillIncreCount++;
					if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
						pUseSkill->UseSKillIncreCount = 0;
						pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

					}

					return TRUE;

				}
			}
		}
	}

	return FALSE;
}
int F_Impact()
{//임펙트 

	CheckMana = Impact_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}

				return TRUE;

			}
		}
	}

	return FALSE;
}

int F_Triple_Impact()
{//트리플 임펙트 


	CheckMana = T_Impact_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}

				return TRUE;

			}
		}
	}

	return FALSE;
}

int F_Brutal_Swing()
{//부루탈 스윙


	CheckMana = B_Swing_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}

				return TRUE;

			}
		}
	}

	return FALSE;
}

int F_Roar()
{//로어 

	CheckMana = Roar_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		//무기 코드에 상관없이 사용할수있다 
		//if(pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE&sinITEM_MASK2)){ //
		CheckCharForm();
		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
		ReformCharForm();
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

		//}
	}

	return FALSE;
}

int F_RageOfZecram()
{//레이지 오브 자크람

	CheckMana = R_Zecram_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}

				return TRUE;

			}
		}
	}

	return FALSE;

}


////////////////////////////파이크맨  
int F_Pike_Wind()
{//파이크 윈드

	CheckMana = Pike_Wind_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;

		}
	}

	return FALSE;

}
int F_Ice_Attribute()
{//아이스 어트리뷰트 

	/////// 캐릭터 수치를 변경한다 
	cInvenTory.SetItemToChar();

	return TRUE;
}
int F_Critical_Hit()
{//크리티컬 힛

	CheckMana = Critical_Hit_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		CheckCharForm();
		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

		ReformCharForm();
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

	}
	return FALSE;

}
int F_Jumping_Crash()
{//점핑 크래쉬 

	CheckMana = Jumping_Crash_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}

				return TRUE;

			}
		}
	}

	return FALSE;

}

int F_Ground_Pike()
{//그라운드 스파이크 
	CheckMana = G_Pike_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;

		}
	}

	return FALSE;

}

int F_Tornado_Pike()
{//토네이도  

	CheckMana = Tornado_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  
			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;

		}
	}

	return FALSE;

}

int F_Weapone_Defence_Mastery()
{//웨폰 디펜스 마스터리
	/////// 캐릭터 수치를 변경한다 
	cInvenTory.SetItemToChar();

	return TRUE;
}

int F_Expansion()
{//익스펜션 

	CheckMana = Expansion_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		for (int i = 0; i < 8; i++) {
			if (pUseSkill->Skill_Info.UseWeaponCode[i] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //

				CheckCharForm();
				sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
				sinSetStamina(sinGetStamina() - CheckStamina);
				CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
				pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
				CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

				ReformCharForm();
				pUseSkill->UseSKillIncreCount++;
				if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
					pUseSkill->UseSKillIncreCount = 0;
					pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

				}

				return TRUE;
			}
		}
	}

	return FALSE;

}


////////////////////////////아쳐
int F_Scout_Hawk()
{//스카우트 호크 

	int SkillIndex = 0;

	CheckMana = Scout_Hawk_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	//조건 충족시 
	if (RequireStateCheck(CheckMana)) {
		CheckCharForm();
		pUseSkill->UseTime = Scout_Hawk_UseTime[pUseSkill->Point - 1];
		sinContinueSkillSet(pUseSkill);
		SwitchSkill(pUseSkill);
		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

		ReformCharForm();
		/////// 캐릭터 수치를 변경한다 
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}

		return TRUE;

	}
	return FALSE;

}
int F_Shooting_Mastery()
{//슛팅 마스터리 

	cInvenTory.SetItemToChar();
	return TRUE;

}
int F_Wind_Arrow()
{//윈드 에로우 

	CheckMana = Wind_Arrow_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;


		}
	}
	return FALSE;
}
int F_Perfect_Aim()
{//퍼펙트 에임  

	CheckMana = Perfect_Aim_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;

		}
	}
	return FALSE;

}

int F_Dions_Eye()
{//디온스 아이 
	cInvenTory.SetItemToChar();
	return TRUE;

}

int F_Falcon()
{//팔콘 

	CheckMana = Falcon_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		//if(pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex-1].CODE&sinITEM_MASK2)){ //
		CheckCharForm();
		SwitchSkill(pUseSkill);
		sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
		sinSetStamina(sinGetStamina() - CheckStamina);
		CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
		pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
		CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

		pUseSkill->UseTime = Falcon_Time[pUseSkill->Point - 1];
		sinContinueSkillSet(pUseSkill);

		ReformCharForm();
		pUseSkill->UseSKillIncreCount++;
		if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
			pUseSkill->UseSKillIncreCount = 0;
			pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

		}
		return TRUE;

		//}
	}
	return FALSE;
}

int F_Arrow_of_Rage()
{//에로우 오브 레이지 
	CheckMana = A_Rage_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;

		}
	}
	return FALSE;

}

int F_Avalanche()
{//에벨 런츠 
	CheckMana = Avalanche_UseMana[pUseSkill->Point - 1];

	/*
	if( chaPremiumitem.m_ManaReducePotiontime > 0 )	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}
	*/

	// 장별 - 테리 속성
	if (chaPremiumitem.m_ManaReducePotiontime > 0 && chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)	// pluto 마나 리듀스 포션
	{
		CheckMana = CheckMana - ((CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100) + (CheckMana * 20 / 100));
	}

	else if (chaPremiumitem.m_ManaReducePotiontime > 0)
	{
		CheckMana = CheckMana - (CheckMana * chaPremiumitem.m_ManaReducePotionValue / 100);
	}

	else if (chaPremiumitem.m_TerryTime > 0 && sinChar->GravityScroolCheck[1] == 1)
	{
		CheckMana = CheckMana - (CheckMana * 20 / 100);
	}

	if (RequireStateCheck(CheckMana)) {
		if (pUseSkill->Skill_Info.UseWeaponCode[0] == (cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE & sinITEM_MASK2)) { //
			CheckCharForm();
			sinSetMana(sinGetMana() - CheckMana);	    //사용된 마나 , 스테미나 , 숙련도 
			sinSetStamina(sinGetStamina() - CheckStamina);
			CSKILL->CheckSkillMasteryForm(pUseSkill); // 스킬 마스터리를 체크한다
			pUseSkill->Mastery = 0; //사용후 마스터리 초기화 
			CSKILL->ReformSkillMasteryForm(pUseSkill);// 스킬 마스터리 갱신  

			ReformCharForm();
			pUseSkill->UseSKillIncreCount++;
			if (pUseSkill->UseSKillIncreCount >= sinMasteryIncreaIndex[pUseSkill->Skill_Info.SkillNum] + ((pUseSkill->Point - 1) / 3)) {
				pUseSkill->UseSKillIncreCount = 0;
				pUseSkill->UseSkillCount += USE_SKILL_MASTERY_COUNT;

			}

			return TRUE;

		}
	}
	return FALSE;

}

/*----------------------------------------------------------------------------*
*							템스크론 3차 스킬
*-----------------------------------------------------------------------------*/
//메카니션
int F_Metal_Armor()
{

	pUseSkill->UseTime = Metal_Armor_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_Grand_Smash()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_M_W_Mastery()
{
	return TRUE;
}
int F_Spark_Shield()
{
	pUseSkill->UseTime = Spark_Shield_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}

//파이터
int F_Concentration()
{
	pUseSkill->UseTime = Concentration_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar();

	return TRUE;
}
int F_Avanging_Crash()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Swift_Axe()
{
	pUseSkill->UseTime = Swift_Axe_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar();

	return TRUE;
}
int F_Bone_Crash()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

//파이크맨
int F_Venom_Spear()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Vanish()
{
	pUseSkill->UseTime = Vanish_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);

	return TRUE;
}
int F_Critical_Mastery()
{
	return TRUE;
}
int F_Chain_Lance()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

//아처
int F_E_Shot()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_G_Falcon()
{
	pUseSkill->UseTime = Golden_Falcon_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	SwitchSkill(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_B_Shot()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Perforation()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
/*----------------------------------------------------------------------------*
*							템스크론 4차 스킬
*-----------------------------------------------------------------------------*/
//메카니션
int F_Impulsion()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Compulsion()
{
	pUseSkill->UseTime = Compulsion_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_Magnetic_Sphere()
{
	pUseSkill->UseTime = Magnetic_Sphere_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_Metal_Golem()
{
	pUseSkill->UseTime = 360; //고정
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar();
	SummonSkillCodeIndex[0] = 0;
	return TRUE;
}

//파이터
int F_Destoryer()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Berserker()
{
	pUseSkill->UseTime = Berserker_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_Cyclone_Strike()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Boost_Health()
{
	return TRUE;
}

//파이크맨
int F_Assassin_Eye()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Charging_Strike()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Vague()
{
	pUseSkill->UseTime = Vague_Time[pUseSkill->Point - 1];
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar();
	return TRUE;
}
int F_Shadow_Master()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}

//아쳐
int F_Recall_Wolverin()
{
	pUseSkill->UseTime = 6 * 60;
	sinContinueSkillSet(pUseSkill);
	sinUseManaStamina(pUseSkill);
	cInvenTory.SetItemToChar();
	SummonSkillCodeIndex[2] = 0;
	return TRUE;
}
int F_Evasion_Mastery()
{
	sinUseManaStamina(pUseSkill);
	SummonSkillCodeIndex[3] = 0;
	return TRUE;
}
int F_Phoenix_Shot()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}
int F_Force_Of_Nature()
{
	sinUseManaStamina(pUseSkill);
	return TRUE;
}















