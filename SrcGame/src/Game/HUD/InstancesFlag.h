#pragma once

#include <Windows.h>
#define DIRECTINPUT_VERSION 0x0800
#include <tchar.h>
#include <dinput.h>
#include "imGui/imgui.h"
#include "imGui/imgui_impl_dx9.h"
#include "imGui/imgui_impl_win32.h"

#include <Windows.h>
#include <d3d9.h>
#include <string>
#include "..\\drawsub.h"


class ImGuiFlags
{
public:
	static              ImGuiFlags* GetInstance() { static ImGuiFlags instance; return &instance; }
	void				InstancesFlag();
	void				SendChatMessage(std::string message, std::string sender, int dwIP);
	void				ClearBuff();
	void				initPartyWhisper(std::string name);
private:
};