#pragma once

#include <string>
#include <Windows.h>
#include <memory>
#include "DXRenderer.h"

#include <ostream>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <utility>
#include <algorithm>
#include <any>
#include <regex>
#include <memory>
#include <stack>
#include <mutex>
#include <deque>
#include <sstream>
#include <locale>
#include <cctype>
#include <queue>
#include <clocale>
#include <unordered_map>
#include <array>
#include <limits>
#include <chrono>
#include <random>
#include <filesystem>
#include <future>

//Windows Header Files:
#include <Windows.h>
#include <ShellAPI.h>
#include <tlhelp32.h>
#include <Shobjidl.h>
#include <intrin.h>
#include <Psapi.h>
#include <immintrin.h>
#include <winternl.h>
#include <assert.h>
#include <Shlwapi.h>
#include <winnt.h>

#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dsound.h>
#include <D3Dcompiler.h>

//Delta3D:
#include "IGraphics.h"
#include "ICore.h"
#include "IIO.h"
#include "IMath.h"
#include "IResource.h"
#include "IThirdParty.h"
#include "ILegacy.h"

using namespace Delta3D;

struct ResolutionStruct
{
	UINT uWidth;
	UINT uHeight;
};

#include "DXGraphicsDevice.h"
//#include "DXGeometryInstance.h"

extern HWND hwnd;

void OnLostDevice();
void OnResetDevice();
void Render3D();
BOOL SetDisplayMode(HWND hWnd, DWORD Width, DWORD Height, DWORD BPP);

class DXGraphicEngine
{
protected:
	// Post Processing
	class DXPostProcess* lpPostProcess;

	// Light
	class CLightEffect* lpLightEffect;

	//Test Geometry
	//DXGeometryInstance* pcGeometryEngine;

public:

	static DXGraphicEngine* getInstance()
	{
		static DXGraphicEngine inst;
		return &inst;
	}

	int iLastPrimitiveCount = 0;
	int iLastPrimitiveType = 0;
	DWORD dwLastVertexType = 0;
	PVOID pLastRenderPtr = NULL;

	ResolutionStruct sResolution;

	virtual ~DXGraphicEngine();

	void Log(char* pszFormat, ...);
	void Error(char* pszFormat, ...);

	// Initialization
	BOOL InitD3D(HWND hWindow, const int screenWidth, const int screenHeight);

	// Getters 
	LPDIRECT3DDEVICE9 GetDevice() { return lpD3DDev; };
	DX::GraphicsDevice_ptr GetGraphicsDevice() { return pGraphicsDevice; };
	int GetWidthBackBuffer() { return d3dPP.BackBufferWidth; };
	int GetHeightBackBuffer() { return d3dPP.BackBufferHeight; };
	HWND GetWindowHandler() { return hwnd; };
	D3DPRESENT_PARAMETERS& Get3DPP() { return d3dPP; }
	LPDIRECT3DVERTEXBUFFER9 GetVertexBuffer(int i) { return lpVertexBuffer[i]; }
	LPDIRECT3DINDEXBUFFER9 GetIndexBuffer(int i) { return lpIndexBuffer[i]; }

	//Post Process
	DXPostProcess* GetPostProcess() { return lpPostProcess; }

	void DrawRectangle2D(class Rectangle2D rRect, DWORD dwColor);
	void DrawRectangle2D(class Rectangle2D sBox, DWORD dwColor1, DWORD dwColor2);

	// Scene
	BOOL Reset(const int& screenWidth, const int& screenHeight);
	UINT GetVertexSize(DWORD dwVertexTypeDesc);
	void OnResetDevice_();
	void OnLostDevice_();
	void ReloadPostProcess();
	BOOL CheckDevice();


	DX::Renderer_ptr pRenderer;
	DX::GraphicsDevice_ptr pGraphicsDevice;
	D3DPRESENT_PARAMETERS d3dPP;
	LPDIRECT3D9 lpD3D;
	LPDIRECT3DDEVICE9 lpD3DDev;
	D3DDISPLAYMODE d3dDM;
	D3DCAPS9 d3dCaps;
	LPDIRECT3DVERTEXBUFFER9 lpVertexBuffer[5];
	LPDIRECT3DINDEXBUFFER9 lpIndexBuffer[5];

	ID3DXLine* pRectangleUI;
	BOOL bCreatedRectangle;
private:
	DXGraphicEngine();

	HRESULT hr;
	bool antiAliasing;
	bool vSync;
};

#define GRAPHICENGINE (DXGraphicEngine::getInstance())
#define GRAPHICDEVICE GRAPHICENGINE->GetDevice()

