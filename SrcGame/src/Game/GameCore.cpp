#include "StdAfx.h"
#include "GameCore.h"
#include "Def.h"

std::vector<CBaseHandle*> vHandleElement;
std::vector<CBaseWindow*> vWindowElement;

CGameCore::CGameCore()
{
	AddHandle(new CHAT::Handle(EHandle::EHANDLE_Chat));
	AddWindow(new CPartyHandler(EWindow::EWINDOW_HANDLEPARTY));

	AddWindow(new CHAT::Window(EWindow::EWINDOW_Chat));
	AddWindow(new HUDController(EWindow::EWINDOW_Target));
	AddWindow(new CMiniMapHandler(EWindow::EWINDOW_MiniMap));

	AddWindow(new CPartyWindow(EWindow::EWINDOW_Party));
	AddWindow(new CMessageBox(EWindow::EWINDOW_MessageBox));

	pcChangeCaravanName = std::make_shared<UI::ChangeWindow>();
	pcItemInfoBox = std::make_shared<UI::ItemInfoBox>();
	pcSkillInfoBox = std::make_shared<UI::SkillInfoBox>();
	pcMessageBalloon = std::make_shared<UI::MessageBalloon>(Rectangle2D(0, 0, 0, 0));
}
CGameCore::~CGameCore()
{
	for (auto& v : vHandleElement)
	{
		delete v;
	}

	for (auto& v : vWindowElement)
	{
		delete v;
	}

	if (pcItemInfoBox)
	{
		pcItemInfoBox.reset();
		pcItemInfoBox = NULL;
	}

	if (pcSkillInfoBox)
	{
		pcSkillInfoBox.reset();
		pcSkillInfoBox = NULL;
	}

	if (pcMessageBalloon)
	{
		pcMessageBalloon.reset();
		pcMessageBalloon = NULL;
	}

	if (pcChangeCaravanName)
	{
		pcChangeCaravanName.reset();
		pcChangeCaravanName = NULL;
	}

	vHandleElement.clear();
	vWindowElement.clear();
}

void CGameCore::Render2D()
{
	for (auto& v : vWindowElement)
	{
		v->Render();
	}
}
void CGameCore::Init()
{
	INIT(pcItemInfoBox);
	INIT(pcSkillInfoBox);
	INIT(pcMessageBalloon);
	INIT(pcChangeCaravanName);

	for (auto& v : vHandleElement)
	{
		INIT(v);
	}
	for (auto& v : vWindowElement)
	{
		INIT(v);
	}

	m_bInit = TRUE;
}
void CGameCore::SetCanMove(BOOL value)
{
	for (auto& v : vWindowElement)
	{
		v->SetCanMove(value);
	}
}
void CGameCore::SetFocus(CBaseWindow* pWindow)
{
	for (std::vector<CBaseWindow*>::iterator it = vWindowElement.begin(); it != vWindowElement.end(); it++)
	{
		if (*it == pWindow)
		{
			vWindowElement.erase(it);

			int iCount = 0;

			for (auto& v : vWindowElement)
			{
				if (v->GetWindowLevel() <= pWindow->GetWindowLevel())
				{
					iCount++;
				}
			}

			vWindowElement.insert(vWindowElement.begin() + iCount, pWindow);
			break;
		}
	}
}
BOOL CGameCore::OnKeyChar(CKeyboard* pcKeyboard)
{
	if (pcChangeCaravanName->OnKeyChar(pcKeyboard))
		return TRUE;

	for (int t = (int)vWindowElement.size() - 1; t >= 0; t--)
	{
		if (vWindowElement[t]->OnKeyChar(pcKeyboard))
			return TRUE;
	}

	return FALSE;
}
BOOL CGameCore::OnKeyPress(CKeyboard* pcKeyboard)
{
	if (pcSkillInfoBox->OnKeyPress(pcKeyboard))
		return TRUE;

	if (pcChangeCaravanName->OnKeyPress(pcKeyboard))
		return TRUE;

	for (int t = (int)vWindowElement.size() - 1; t >= 0; t--)
	{
		if (vWindowElement[t]->OnKeyPress(pcKeyboard))
			return TRUE;
	}
	return FALSE;
}
BOOL CGameCore::OnMouseClick(CMouse* pcMouse)
{
	if (SKILLMANAGERHANDLER->OnMouseClick(pcMouse))
		return TRUE;

	if (pcChangeCaravanName->OnMouseClick(pcMouse))
		return TRUE;

	for (int t = (int)vWindowElement.size() - 1; t >= 0; t--)
	{
		if (vWindowElement[t]->OnMouseClick(pcMouse))
		{
			if (pcMouse->GetEvent() == ClickDownL)
				this->SetFocus(vWindowElement[t]);
			return TRUE;
		}
	}
	return FALSE;
}
BOOL CGameCore::OnMouseScroll(CMouse* pcMouse)
{
	for (int t = (int)vWindowElement.size() - 1; t >= 0; t--)
	{
		if (vWindowElement[t]->OnMouseScroll(pcMouse))
			return TRUE;
	}
	return FALSE;
}
void CGameCore::OnMouseMove(CMouse* pcMouse)
{
	if (Caravana::GetInstance()->ChangeName)
	{
		pcChangeCaravanName->OnMouseMove(pcMouse);
	}

	for (int t = (int)vWindowElement.size() - 1; t >= 0; t--)
	{
		vWindowElement[t]->OnMouseMove(pcMouse);
	}
}
void CGameCore::Shutdown()
{
	SHUTDOWN(pcChangeCaravanName);

	for (auto& v : vHandleElement)
	{
		SHUTDOWN(v);
	}
	for (auto& v : vWindowElement)
	{
		SHUTDOWN(v);
	}
}
CBaseHandle* CGameCore::GetHandle(EHandle eID)
{
	for (auto& v : vHandleElement)
	{
		if (v->GetHandleId() == eID)
			return v;
	}

	return nullptr;
}
CBaseWindow* CGameCore::GetWindow(EWindow eID)
{
	for (auto& v : vWindowElement)
	{
		if (v->GetWindowId() == eID)
			return v;
	}

	return nullptr;
}

CBaseWindow* CGameCore::AddWindow(CBaseWindow* pWindow)
{
	vWindowElement.push_back(pWindow);

	struct
	{
		bool operator()(CBaseWindow* a, CBaseWindow* b) const
		{
			return a->GetWindowLevel() < b->GetWindowLevel();
		}

	}Less;

	std::sort(vWindowElement.begin(), vWindowElement.end(), Less);

	return pWindow;
}
CBaseHandle* CGameCore::AddHandle(CBaseHandle* pHandle)
{
	vHandleElement.push_back(pHandle);

	return pHandle;
}
void CGameCore::DelWindow(CBaseWindow* pWindow)
{
	for (auto it = vWindowElement.begin(); it != vWindowElement.end(); it++)
	{
		if (*it == pWindow)
		{
			DELET(pWindow);
			vWindowElement.erase(it);
			break;
		}
	}
}
void CGameCore::DelHandle(CBaseHandle* pHandle)
{
	for (auto it = vHandleElement.begin(); it != vHandleElement.end(); it++)
	{
		if (*it == pHandle)
		{
			DELET(pHandle);
			vHandleElement.erase(it);
			break;
		}
	}
}
void CGameCore::OnResolutionChanged()
{
	pcChangeCaravanName->OnResolutionChanged();

	for (auto& v : vWindowElement)
	{
		v->OnResolutionChanged();
	}
}
