#pragma once
#include <Windows.h>

class VIP
{
public:
	static              VIP* GetInstance() { static VIP instance; return &instance; }

	void setVipLevel(int vipNum) { lpCurPlayer->vipLevel = vipNum; };
	int getVipLevel() { return lpCurPlayer->vipLevel; };
	int vipCommands();
	void setTimePremiuns();

private:
};
