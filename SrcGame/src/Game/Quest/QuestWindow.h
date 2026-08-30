#pragma once
#include <Windows.h>
#include <tchar.h>
#include <dinput.h>
#include <Windows.h>
#include <string>

class QuestWindow
{
public:

	bool openFlag = false;
	bool openOverlay = false;

	static              QuestWindow* GetInstance() { static QuestWindow instance; return &instance; }
	void				openWindow(bool* p_open);
	void				questOverlay(bool* p_open);
private:
};