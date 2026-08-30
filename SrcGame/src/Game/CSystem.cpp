#include "stdafx.h"
#include "CSystem.h"

#include "smLib3d\smSin.h"

#include "DXGraphicEngine.h"

// Window Procedure
static LRESULT CALLBACK				WndProc( HWND, UINT, WPARAM, LPARAM );
static CSystem*						ApplicationHandle = 0;

// Critical Sections


BOOL FULL_SCREEN = FALSE;

CSystem::CSystem()
{
}


CSystem::~CSystem()
{
}

BOOL CSystem::Init()
{
	int iScreenWidth, iScreenHeight;
	BOOL bResult;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	iScreenWidth = 0;
	iScreenHeight = 0;

	// Initialization of Game
	initSinCos();
	InitializeCriticalSection( &cSection );
	InitializeCriticalSection( &cDrawSection );
	InitializeCriticalSection( &cSection_Main );
	srand( GetCurrentTime() );

	// Initialize the windows api.
	InitWindow( iScreenWidth, iScreenHeight );

	// Initialize DirectD3D
	DXGraphicEngine * GraphicEngine = new DXGraphicEngine();
	GraphicEngine->Init( hWindow );

	delete GraphicEngine;

	return TRUE;
}

void CSystem::Shutdown()
{
	DXGraphicEngine * GraphicEngine = new DXGraphicEngine();

	// Shutdown Direct3D
	GraphicEngine->Shutdown();

	delete GraphicEngine;

	// Shutdown the window.
	ShutdownWindow();
}

void CSystem::Run()
{
	MSG msg;
	BOOL bDone, bResult;

	// Initialize the message structure.
	ZeroMemory( &msg, sizeof( MSG ) );

	// Loop until there is a quit message from the window or the user.
	bDone = FALSE;
	while ( !bDone )
	{
		// Handle the windows messages.
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		// If windows signals to end the application then exit out.
		if ( msg.message == WM_QUIT )
			bDone = TRUE;
		else
		{
			// Otherwise do the frame processing.
			bResult = Frame();
			if ( !bResult )
				bDone = TRUE;
		}
	}

	return;
}

BOOL CSystem::Frame()
{
	BOOL bResult;

	return TRUE;
}

LRESULT CALLBACK CSystem::MessageHandler( HWND hWindow, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( uMsg )
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{

		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{

		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc( hWindow, uMsg, wParam, lParam );
		}
	}
}

void CSystem::InitWindow( int iScreenWidth, int iScreenHeight )
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int iPositionX, iPositionY;

	// Get an external pointer to this object.
	ApplicationHandle = this;

	// Get the instance of this application.
	hInst = GetModuleHandle( NULL );

	// Give the application a name.
	lAppName = "Priston Tale";

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	wc.hbrBackground = ( HBRUSH )GetStockObject( BLACK_BRUSH );
	wc.lpszMenuName = NULL;
	wc.lpszClassName = lAppName;
	wc.cbSize = sizeof( WNDCLASSEX );

	// Register the window class.
	RegisterClassEx( &wc );

	// Determine the resolution of the clients desktop screen.
	iScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	iScreenHeight = GetSystemMetrics( SM_CYSCREEN );

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if ( FULL_SCREEN )
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
		dmScreenSettings.dmSize = sizeof( dmScreenSettings );
		dmScreenSettings.dmPelsWidth = ( unsigned long )iScreenWidth;
		dmScreenSettings.dmPelsHeight = ( unsigned long )iScreenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );

		// Set the position of the window to the top left corner.
		iPositionX = iPositionY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		iScreenWidth = 800;
		iScreenHeight = 600;

		// Place the window in the middle of the screen.
		iPositionX = ( GetSystemMetrics( SM_CXSCREEN ) - iScreenWidth ) / 2;
		iPositionY = ( GetSystemMetrics( SM_CYSCREEN ) - iScreenHeight ) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	hWindow = CreateWindowEx( WS_EX_APPWINDOW, lAppName, lAppName,
							 WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
							 iPositionX, iPositionY, iScreenWidth, iScreenHeight, NULL, NULL, hInst, NULL );

	// Bring the window up on the screen and set it as main focus.
	ShowWindow( hWindow, SW_SHOW );
	SetForegroundWindow( hWindow );
	SetFocus( hWindow );

	// Hide the mouse cursor.
	ShowCursor( FALSE );
}

void CSystem::ShutdownWindow()
{
	// Show the mouse cursor.
	ShowCursor( TRUE );

	// Fix the display settings if leaving full screen mode.
	if ( FULL_SCREEN )
		ChangeDisplaySettings( NULL, 0 );

	// Remove the window.
	DestroyWindow( hWindow );
	hWindow = NULL;

	// Remove the application instance.
	UnregisterClass( lAppName, hInst );
	hInst = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}

LRESULT CALLBACK WndProc( HWND hWindow, UINT uMessage, WPARAM wParam, LPARAM lParam )
{
	switch ( uMessage )
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage( 0 );
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler( hWindow, uMessage, wParam, lParam );
		}
	}
}