#pragma once

#include <Engine/Mouse/Mouse.h>
#include <Engine/Keyboard/Keyboard.h>
#include <Engine/UI/UIWindow.h>
#include <Engine/UI/UIEvent.h>

enum class EHandle : int
{
	EHANDLE_None,
	EHANDLE_Settings,
	EHANDLE_Chat,
	EHANDLE_Party,
	EHANDLE_Inventory,
};
enum class EWindow : int
{
	EWINDOW_None,
	EWINDOW_Setting,
	EWINDOW_Chat,
	EWINDOW_Party,
	EWINDOW_Raid,
	EWINDOW_HANDLEPARTY,
	EWINDOW_MiniMap,
	EWINDOW_Skill,
	EWINDOW_CharacterStatus,
	EWINDOW_MessageBox,
	EWINDOW_MessageTitle,
	EWINDOW_WarpGate,
	EWINDOW_Premium,
	EWINDOW_Cursor,
	EWINDOW_Target,
	EWINDOW_ChangeCaravanName
};

class CBaseHandle
{
public:
	CBaseHandle() {}
	CBaseHandle(EHandle eID) : m_eHandleID(eID) {}
	virtual ~CBaseHandle() {}

	virtual void Init() {}
	virtual void Shutdown() {}
	virtual void Render() {}

	virtual void Update(float fTime) {}
	virtual void Tick() {}

	EHandle GetHandleId() { return m_eHandleID; }

private:
	EHandle m_eHandleID = EHandle::EHANDLE_None;
};
class CBaseWindow
{
public:
	CBaseWindow() {}
	CBaseWindow(EWindow eID) : m_eWindowID(eID) {}
	virtual ~CBaseWindow() {}

	virtual void Init() {}
	virtual void Shutdown() {}
	virtual void Render() {}

	EWindow GetWindowId() { return m_eWindowID; }

	int GetWindowLevel() { return m_iWindowLevel; }
	void SetWindowLevel(int iLevel) { m_iWindowLevel = iLevel; }

	virtual void Show() {}

	virtual BOOL OnMouseClick(CMouse* pcMouse) { return FALSE; }
	virtual BOOL OnMouseScroll(CMouse* pcMouse) { return FALSE; }
	virtual void OnMouseMove(CMouse* pcMouse) {}

	virtual BOOL OnKeyChar(CKeyboard* pcKeyboard) { return FALSE; }
	virtual BOOL OnKeyPress(CKeyboard* pcKeyboard) { return FALSE; }

	virtual void OnResolutionChanged() {}
	virtual void SetCanMove(BOOL value) {}
private:
	EWindow m_eWindowID = EWindow::EWINDOW_None;
	int m_iWindowLevel = 0;
};