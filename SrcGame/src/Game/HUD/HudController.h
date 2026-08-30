#pragma once

#include <Windows.h>

class HUDController : public CBaseWindow
{
	enum EWindow1
	{
		WINDOWID_Main,
		BUTTONID_LockItem,
		IMAGEID_LockItemView,
	};

	enum ETargetWindow1
	{
		IMAGEID_LifeBar,
		IMAGEID_Type,
		IMAGEID_TargetInfo,
		IMAGEID_ClanIcon,

		TEXTID_Name,
		TEXTID_TargetInfo,
		TEXTID_ClanInfo,
		TEXTID_Life,
		TEXTID_Level,
		TEXTID_LifePercent,
		TEXTID_Range,

		BUTTONID_More,

		DROPDOWNMENU_More,

		TOOLTIP_TargetType,

		WINDOWID_TargetInfo,
		WINDOWID_CharacterInfo,
	};

	enum ETargetType
	{
		TARGETTYPE_Demon,
		TARGETTYPE_Mechanic,
		TARGETTYPE_Mutant,
		TARGETTYPE_Normal,
		TARGETTYPE_Undead,
		TARGETTYPE_NPC,
		TARGETTYPE_Player,
	};

protected:
	void										OnSelectTargetMenu();
public:
	HUDController();
	HUDController(EWindow eID);
	virtual									   ~HUDController();

	//Handler Functions
	BOOL										OnMouseClick(class CMouse* pcMouse);
	void										OnMouseMove(class CMouse* pcMouse);
	BOOL										OnKeyPress(class CKeyboard* pcKeyboard);
	BOOL										OnMouseScroll(class CMouse* pcMouse);
	BOOL										OnKeyChar(class CKeyboard* pcKeyboard);

	void										OnResolutionChanged();

	void										RenderFriendStatus();

	//Initialize Functions
	void										Init();
	void										Update(float fTime);
	void										Render();
	void										Shutdown();

	BOOL										IsLockItems();
	void										SetCanMove(BOOL value);
	void										RenderHPCrystalTower(int iX, int iY, int iMin, int iMax);

	void										UpdateObjectsSettings();

	//Trade Request
	void										OnAcceptTradeRequest(int iTargetID);

	BOOL										IsWindowOldHUDOpen();

	//Target
	int											GetTargetID() { return iTargetID; }
	BOOL										UpdateTargetInfo(smCHAR* pcUnitData);
	void										SetRenderTarget(BOOL b);
	smCHAR* GetTargetUnitData() { return pcLastUnitDataTarget; }
	UI::Image* GetTargetTypeImage(int iIndex) { return pcaImageTargetType[iIndex]; }

	UI::Window_ptr								GetTargetWindow() { return pWindowTarget; }

	smCHAR* pcLastUnitDataTarget = NULL;
private:
	//Target
	void										BuildTargetWindow();

	//Display
	void										BuildDisplayWindow1();
	int											GetShortcutSkill(int iShortcut);
private:
	int											iFontID = -1;

	UI::Window_ptr								pWindowTarget = NULL;
	UI::Window_ptr								pWindowDisplay = NULL;

	BOOL										baFriendStatus[101];
	BOOL										bRenderTarget;
	int											iTargetID;
	BOOL										bLoadClan16 = FALSE;
	BOOL										bLoadClan32 = FALSE;

	UI::Image* pcaImageCrystalTowerHPBar[2];
	UI::Image* pcaImageSkillsIcon[2][20];

	//Target
	UI::Image* pcaImageTargetType[7];
};

