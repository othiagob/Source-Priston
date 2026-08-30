#pragma once

#include <Windows.h>
#include "Engine/UI/UISkillInfoBox.h"
#include <sinbaram/SkillFunction/Tempskron.h>
#include <sinbaram/SkillFunction/Morayion.h>
#include <sinbaram/sinItem.h>

#define TEXTCOLOR_SKILLCHAIN			D3DCOLOR_XRGB( 105, 144, 176 )
#define TEXTCOLOR_SKILLSTATS			D3DCOLOR_XRGB( 34, 91, 138 )

#define SKILLMANAGERHANDLER				SkillManager::GetInstance()

class SkillManager
{
public:
	static              SkillManager* GetInstance() { static SkillManager instance; return &instance; }

	SkillManager();
	virtual										~SkillManager();

	BOOL										OnKeyPress(class CKeyboard* pcKeyboard);
	BOOL										OnMouseClick(class CMouse* pcMouse);
	BOOL										OnMouseClickSkillPet();
	BOOL										SetDefaultPetMode(int iSkillID, BOOL bAgressive);
	void										Clear() {}
	void										AddSkillElement(int eElementID);
	void										CancelForceOrb(int iForceOrbID);
	int											GetLevelSkill(UINT uSkillCode);

	void										GetInfoBoxSize(struct sSKILLBOX* psSkillBox, int iColumn, int iIcon);
	void										AddSkillInfo(int iColumn, const char* pszText, ...);
	void										AddSkillBoxEmptyLine();
	void										AddSkillBoxGroupTitle(const char* pszTitle, D3DCOLOR dwColor, int eElementID = sITEMINFO_NONE);
	void										AddSkillBoxInfo(const char* pszLabel, const char* pszString);
	void										AddSkillBoxInfo(const char* pszLabel, const char* pszString, DWORD dwColor);
	void										AddSkillBoxInfo(const char* pszLabel, const char* pszFormat, float SkillValueLevel, int iSkillLevelZeroBased, float NextSkillValueLevel, int SkillID = 0, BOOL bShowNextLevel = TRUE, float fScaling = 1.0f);
	void										AddSkillBoxInfo(const char* pszLabel, const char* pszFormat, int SkillValueLevel, int iSkillLevelZeroBased, int NextSkillValueLevel, int SkillID = 0, BOOL bShowNextLevel = TRUE, float fScaling = 1.0f);
	void										AddSkillBoxInfoDuration(int SkillValueLevel, int iSkillLevelZeroBased, int NextSkillValueLevel, int SkillID = 0);
	void										AddSkillBoxInfoRange(const char* pszLabel, const char* pszFormat, int SkillValueLevel, int iSkillLevelZeroBased, int NextSkillValueLevel, BOOL bShowNextLevel = TRUE, float fScaling = 1.0f);
	void										AddSkillBoxInfoDual(const char* pszLabel, const char* pszFormat, int SkillValueLevel, int SkillValueLevel2, int iSkillLevelZeroBased, int NextSkillValueLevel, int NextSkillValueLeve2, BOOL bShowNextLevel = TRUE, float fScaling1 = 1.0f, float fScaling2 = 1.0f);

	BOOL										AddSkillInfoToBox(int iSkillID, int iSkillLevelZeroBased);

	BOOL										AddSkillInfoToBox_FS(int iSkillID, int iSkillLevelZeroBased);
	BOOL										AddSkillInfoToBox_AS(int iSkillID, int iSkillLevelZeroBased);
	BOOL										AddSkillInfoToBox_MS(int iSkillID, int iSkillLevelZeroBased);
	BOOL										AddSkillInfoToBox_PS(int iSkillID, int iSkillLevelZeroBased);

	BOOL										AddSkillInfoToBox_ATA(int iSkillID, int iSkillLevelZeroBased);
	BOOL										AddSkillInfoToBox_KS(int iSkillID, int iSkillLevelZeroBased);
	BOOL										AddSkillInfoToBox_PRS(int iSkillID, int iSkillLevelZeroBased);
	BOOL										AddSkillInfoToBox_MGS(int iSkillID, int iSkillLevelZeroBased);

	BOOL										IsDebuffSkill(DWORD eSkillID);

	BOOL										AddDamageVsMonsterTypeInfo(sSKILL* pcSkill);
	BOOL										AddSpecificMonsterTypeInfo(sSKILL* pcSkill);
	BOOL										AddSummonInfo(sSKILL* pcSkill);
	BOOL										AddPassiveInfo(sSKILL* pcSkill);
	BOOL										AddDebuffInfo(sSKILL* pcSkill);
	BOOL										AddBuffInfo(sSKILL* pcSkill);
	BOOL										AddHintInfo(sSKILL* pcSkill);
private:
	BOOL										baPetAggressive[8];
};