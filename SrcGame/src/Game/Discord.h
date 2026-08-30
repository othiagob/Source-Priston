#pragma once
#include <Windows.h>
#include <sinbaram/sinLinkHeader.h>
class Discord
{
public:
	void Initialize(); 
	void Update(smCHAR* lpCurPlayer);
	void Shutdown();

	void Set_Game_Mode(int Value) { Game_Mode = Value; };
	int Get_Game_Mode() { return Game_Mode; };
private:
	int Game_Mode;
	bool OpenFlag;
};
extern Discord Discord_Handle;
