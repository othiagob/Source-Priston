#pragma once

class UnitGame
{
public:
							UnitGame();
	virtual					~UnitGame();

	void					OnDrawText( HDC hdc, int x, int y, LPCSTR lpString, int c );
};

