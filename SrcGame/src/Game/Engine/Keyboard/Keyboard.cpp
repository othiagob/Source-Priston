#include "Keyboard.h"
#include "Utils\\Macros.h"
#include <GameCore.h>

CKeyboard::CKeyboard()
{
	for (int i = 0; i < 256; i++)
		baKeyDown[i] = FALSE;
}

CKeyboard::~CKeyboard()
{
}

void CKeyboard::SetEvent(EKeyboardEvent eKeyEvent)
{
	eEvent = eKeyEvent;
}

BOOL CKeyboard::IsKeyDown(int iKey)
{
	if (iKey >= 0 && iKey < 256)
		return baKeyDown[iKey];

	return FALSE;
}

BOOL CKeyboard::OnKeyPress(int iKey, BOOL bKeyDown)
{
	if (GAMECOREHANDLE->IsInit())
	{
		if (bKeyDown)
			SetEvent(KeyDown);
		else
			SetEvent(KeyUp);

		if (iKey >= 0 && iKey < sizeof(baKeyDown))
		{
			if (bKeyDown)
				baKeyDown[iKey] = TRUE;
			else
				baKeyDown[iKey] = FALSE;

			iKeyPress = iKey;
		}

		if (this->GetEvent() == KeyDown && this->IsKeyDown(VK_CONTROL) && baKeyDown['A'])
		{
			extern BOOL bShowDrops;

			bShowDrops ? bShowDrops = FALSE : bShowDrops = TRUE;

			bShowDrops ? CHATBOX->AddMessage("> Visualizador de itens no chão ligado", EChatColor::CHATCOLOR_Error) : CHATBOX->AddMessage("> Visualizador de itens no chão desligado", EChatColor::CHATCOLOR_Error);
		}

		return GAMECOREHANDLE->OnKeyPress(this);
	}

	return FALSE;
}

BOOL CKeyboard::OnKeyChar(char cKey)
{
	if (GAMECOREHANDLE->IsInit())
	{
		SetEvent(EKeyboardEvent::KeyChar);

		cKeyChar = cKey;

		return GAMECOREHANDLE->OnKeyChar(this);
	}

	return FALSE;
}

