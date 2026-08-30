#include "stdafx.h"
#include "UnitGame.h"
#include "globals.h"

UnitGame::UnitGame()
{
}


UnitGame::~UnitGame()
{
}

void UnitGame::OnDrawText( HDC hdc, int x, int y, LPCSTR lpString, int c )
{

}

void __cdecl OnDrawText( HDC hdc, int x, int y, LPCSTR lpString, int c )
{
	UnitGame * pUnitGame = new UnitGame();
	pUnitGame->OnDrawText( hdc, x, y, lpString, c );
	delete pUnitGame;
}