#include "Mouse.h"
#include "playmain.h"
#include "character.h"
#include <GameCore.h>
#include <Login/LoginScreen.h>

BOOL bMouseCreatedHandler = FALSE;


CMouse::CMouse()
{
	cPosition.iX = 0;
	cPosition.iY = 0;
	bMouseCreatedHandler = TRUE;

	vCursor.push_back(new CursorData(EMOUSEICON_Default, LoadCursorFromFileA("game\\mouse\\default.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_Sell, LoadCursorFromFileA("game\\mouse\\sell.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_Buy, LoadCursorFromFileA("game\\mouse\\buy.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_Repair, LoadCursorFromFileA("game\\mouse\\repair.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_Attack, LoadCursorFromFileA("game\\mouse\\attack.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_GetItem1, LoadCursorFromFileA("game\\mouse\\dropsee.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_GetItem2, LoadCursorFromFileA("game\\mouse\\droppick.cur")));
	vCursor.push_back(new CursorData(EMOUSEICON_Talk, LoadCursorFromFileA("game\\mouse\\talk.cur")));
}


CMouse::~CMouse()
{
}

EMouseEvent CMouse::GetEvent(DWORD dwKey)
{
	switch (dwKey)
	{
	case WM_LBUTTONDOWN:
		return EMouseEvent::ClickDownL;
	case WM_LBUTTONUP:
		return EMouseEvent::ClickUpL;
	case WM_RBUTTONDOWN:
		return EMouseEvent::ClickDownR;
	case WM_RBUTTONUP:
		return EMouseEvent::ClickUpR;
	}
	return (EMouseEvent)-1;
}

void CMouse::OnMouseMoveHandler(int iX, int iY)
{
	if (GAMECOREHANDLE->IsInit())
	{
		cPosition.iX = iX;
		cPosition.iY = iY;

		GAMECOREHANDLE->OnMouseMove(this);
	}
	else if (GameMode == 1) // Login
	{
		cPosition.iX = iX;
		cPosition.iY = iY;

		LOGINSCREENHANDLER->OnMouseMove(this);
	}
}


BOOL CMouse::OnMouseClickHandler(DWORD dwKey)
{
	if (GAMECOREHANDLE->IsInit())
	{
		if (bEndNoMove)
		{
			bEndNoMove = FALSE;
			IsNoMove(FALSE);
		}

		eEventMouse = GetEvent(dwKey);

		return GAMECOREHANDLE->OnMouseClick(this);
	}
	else if (GameMode == 1) // Login
	{
		if (bEndNoMove)
		{
			bEndNoMove = FALSE;
			IsNoMove(FALSE);
		}

		eEventMouse = GetEvent(dwKey);

		LOGINSCREENHANDLER->OnMouseClick(this);
	}

	return FALSE;
}

BOOL CMouse::OnMouseScrollHandler(short sDistance)
{
	if (GAMECOREHANDLE->IsInit())
	{
		this->sDistance = sDistance / WHEEL_DELTA;

		if (sDistance < 0)
			eEventMouse = ScrollUp;
		else
			eEventMouse = ScrollDown;

		return GAMECOREHANDLE->OnMouseScroll(this);
	}
	return FALSE;
}

void CMouse::IsNoMove(BOOL b)
{
	bNoMove = b;

	if (bNoMove == TRUE)
		bEndNoMove = FALSE;

	lpCurPlayer->DontMoveFlag = bNoMove;
}

void CMouse::BlockMouse()
{
	iBlockedRefCount++;

	// BLOQUEIA O MOUSE
	IsNoMove(TRUE);

	// FAZ O PERSONAGEM PARAR DE ANDAR
	if (lpCurPlayer)
		lpCurPlayer->SetAction(0);
}

void CMouse::UnblockMouse()
{
	if (iBlockedRefCount > 0)
	{
		iBlockedRefCount--;
		if (iBlockedRefCount == 0)
			IsEndNoMove(TRUE);
	}
}

void CMouse::SetCursorGame(int iID)
{
	if (iID == EMOUSEICON_RepairAll)
		iID = EMOUSEICON_Repair;

	for (std::vector<CursorData*>::iterator it = vCursor.begin(); it != vCursor.end(); it++)
	{
		CursorData* pc = (*it);

		if (pc->iID == iID)
		{
			SetCursor(pc->hCursor);
		}
	}

	iLastCursorID = iID;
}
