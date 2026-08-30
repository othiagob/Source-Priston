#pragma once

#include <Chat/ChatHandle.h>
#include <Chat/ChatWindow.h>
#include <HUD/MiniMap/MiniMapHandler.h>
#include <HUD/HudController.h>
#include <Caravana/ChangeCaravanName.h>
#include "sinbaram/sinSkill.h"
#include "netplay.h"
#include "Party/CPartyHandler.h"
#include "HUD/MessageBox.h"
#include "Engine/UI/UIItemInfoBox.h"
#include "Engine/UI/UISkillInfoBox.h"
#include "Engine/UI/UIMessageBallon.h"
#include "Skill/SkillManager.h"

#define MAP_ID	StageField[UNITDATA->OnStageField]->FieldCode

#define GAMECOREHANDLE		CGameCore::GetInstance()
#define CSKILL				cSKILL::GetInstance()
#define CHATGAMEHANDLE		(GAMECOREHANDLE->GetChatHandle())
#define CHATBOX				(GAMECOREHANDLE->GetChatWindow())
#define CHUDCONTROLLER		(GAMECOREHANDLE->GetTargetHandle())
#define CMINIMAPHANDLE		(GAMECOREHANDLE->GetMiniMapHandle())
#define CHANGECARAVANNAME	(GAMECOREHANDLE->pcChangeCaravanName)
#define PARTYWINDOW			(GAMECOREHANDLE->GetWindowParty())
#define PARTYHANDLER		(GAMECOREHANDLE->GetPartyHandle())
#define MESSAGEBOX			(GAMECOREHANDLE->GetWindowMessageBox())

struct ARGBNew
{
	int red;
	int green;
	int blue;
	int alpha;
};

const ARGBNew rarityColorsGround[6] = {
	{200, 255, 255, 255},			//No Rarity
	{200, 255, 255, 255},			//Common
	{255, 184, 255, 190},				//Uncommon
	{255, 180, 192, 255},			//Rare
	{255, 238, 165, 255},			//Epic
	{255, 255, 166, 121}				//Legendary
};

class CGameCore
{
public:
	static              CGameCore* GetInstance() { static CGameCore instance; return &instance; }

	CGameCore();
	virtual ~CGameCore();

	void Init();
	void Shutdown();

	void Frame();
	void Update(float fTime);

	void Render2D();
	void Render3D();

	BOOL IsInit() { return m_bInit; }

	BOOL OnMouseClick(class CMouse* pcMouse);
	BOOL OnMouseScroll(class CMouse* pcMouse);
	void OnMouseMove(class CMouse* pcMouse);

	BOOL OnKeyChar(class CKeyboard* pcKeyboard);
	BOOL OnKeyPress(class CKeyboard* pcKeyboard);

	void SetCanMove(BOOL Value);

	void OnResolutionChanged();

	CHAT::Window* GetChatWindow() { return static_cast<CHAT::Window*>(GetWindow(EWindow::EWINDOW_Chat)); }
	CHAT::Handle* GetChatHandle() { return static_cast<CHAT::Handle*>(GetHandle(EHandle::EHANDLE_Chat)); }

	CMessageBox* GetWindowMessageBox() { return static_cast<CMessageBox*>(GetWindow(EWindow::EWINDOW_MessageBox)); }
	CPartyWindow* GetWindowParty() { return static_cast<CPartyWindow*>(GetWindow(EWindow::EWINDOW_Party)); }
	CPartyHandler* GetPartyHandle() { return static_cast<CPartyHandler*>(GetWindow(EWindow::EWINDOW_HANDLEPARTY)); }

	HUDController* GetTargetHandle() { return static_cast<HUDController*>(GetWindow(EWindow::EWINDOW_Target)); }
	CMiniMapHandler* GetMiniMapHandle() { return static_cast<CMiniMapHandler*>(GetWindow(EWindow::EWINDOW_MiniMap)); }

	UI::SkillInfoBox_ptr pcSkillInfoBox;
	UI::ItemInfoBox_ptr pcItemInfoBox;
	UI::MessageBallon_ptr pcMessageBalloon;
	UI::ChangeWindow_ptr pcChangeCaravanName;

	static void SetFocus(CBaseWindow* pWindow);
	static CBaseHandle* GetHandle(EHandle eID);
	static CBaseWindow* GetWindow(EWindow eID);
	static CBaseWindow* AddWindow(CBaseWindow* pWindow);
	static CBaseHandle* AddHandle(CBaseHandle* pHandle);
	static void DelWindow(CBaseWindow* pWindow);
	static void DelHandle(CBaseHandle* pHandle);
private:
	BOOL m_bInit = FALSE;
};