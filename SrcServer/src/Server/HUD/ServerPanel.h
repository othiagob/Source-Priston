#pragma once

#include <Windows.h>

// Painel ImGui do Server.exe (mesmo hwnd dos sockets). Extra: se o DirectX
// falhar, o mundo continua só no console.

enum
{
	SERVER_PANEL_W = 1024,
	SERVER_PANEL_H = 680
};

void ServerPanel_StartLogCapture();
bool ServerPanel_Init(HWND hwnd);
void ServerPanel_Shutdown();
bool ServerPanel_IsActive();
void ServerPanel_Render();
void ServerPanel_Resize(int width, int height);
void ServerPanel_HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
