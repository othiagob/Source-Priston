#pragma once

class CSystem
{
public:
	// Construction
							CSystem();
	virtual					~CSystem();

	// Initialization
	BOOL					Init();
	void					Shutdown();
	void					Run();

	// Message Handler
	LRESULT CALLBACK		MessageHandler( HWND, UINT, WPARAM, LPARAM );

private:
	BOOL					Frame();

	// Initialization
	void					InitWindow( int iWidth, int iHeight );
	void					ShutdownWindow();

	// Declarations
	LPCSTR					lAppName;
	HINSTANCE				hInst;
	HWND					hWindow;
};

