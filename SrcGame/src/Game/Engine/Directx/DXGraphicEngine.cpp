#include <memory>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "DXGraphicEngine.h"
#include "DX.h"
#include "DXSelectGlow.h"
#include "smLib3d\\smDsx.h"
#include "..\\UI\\UIImage.h"
#include "..\\UI\\UIImageRender.h"
#include "Utils\\Macros.h"
#include "Utils\\strings.h"
#include "Utils\\_common.h"
#include "Utils\\Geometry.h"
#include "Utils\\EXEVertex.h"
#include "Utils\\debug.h"
#include "..\\..\\Def.h"
#include "fileread.h"
#include "..\\CFontHandler.h"
#include "Settings.h"
#include "smLib3d\\smObj3d.h"
#include "imGui/imgui_impl_dx9.h"
#include <PackageFile.h>

#pragma comment(lib, "d3d9.lib") 
#pragma comment(lib, "d3dx9.lib")

Graphics::Camera* camera = nullptr;
Graphics::Camera* frameCamera = nullptr;

Graphics::Material* dropItemMaterial = nullptr;
Graphics::Material* itemQuestMaterial = nullptr;
Graphics::Material* itemAgingMaterial[30] = { 0 };
Graphics::Material* itemMixingMaterial[20] = { 0 };

extern void AnimateMeshGate(void* mesh, int frame, void* angle, void* psFrameInfo, void* talkBone, void* out);

DXGraphicEngine::DXGraphicEngine()
{
	ZeroMemory(&d3dDM, sizeof(D3DDISPLAYMODE));
	ZeroMemory(&d3dCaps, sizeof(D3DCAPS9));
	ZeroMemory(&d3dPP, sizeof(D3DPRESENT_PARAMETERS));

	lpD3D = NULL;
	lpD3DDev = NULL;
	lpPostProcess = NULL;
	lpLightEffect = NULL;

	antiAliasing = false;
	vSync = Settings::GetInstance()->vSync;
}

DXGraphicEngine::~DXGraphicEngine()
{
	DELET(lpPostProcess);


	for (int i = 0; i < _countof(itemMixingMaterial); i++)
	{
		delete itemMixingMaterial[i];
		itemMixingMaterial[i] = nullptr;
	}

	for (int i = 0; i < _countof(itemAgingMaterial); i++)
	{
		delete itemAgingMaterial[i];
		itemAgingMaterial[i] = nullptr;
	}

	delete dropItemMaterial;
	dropItemMaterial = nullptr;

	delete itemQuestMaterial;
	itemQuestMaterial = nullptr;

	delete frameCamera;
	frameCamera = nullptr;

	delete camera;
	camera = nullptr;
}

void DXGraphicEngine::Log(char* pszFormat, ...)
{
	va_list vl;

	DX::Logger Log;

	char szError[16384];

	va_start(vl, pszFormat);
	_vsnprintf_s(szError, _countof(szError), _TRUNCATE, pszFormat, vl);
	va_end(vl);

	char szMessage[4096];
	STRINGFORMAT(szMessage, "[%08X][%s] %s\r\n", hr, Log.ResultToString(hr), szError);

	FILE* file;
	CreateDirectoryA("save\\logs", NULL);
	if (fopen_s(&file, "save\\logs\\init.log", "a") == 0)
	{
		fputs(szMessage, file);
		fflush(file);
		fclose(file);
	}
}

void DXGraphicEngine::Error(char* pszFormat, ...)
{
	va_list vl;

	DX::Logger DXLog;

	char szError[16384];

	va_start(vl, pszFormat);
	_vsnprintf_s(szError, _countof(szError), _TRUNCATE, pszFormat, vl);
	va_end(vl);

	char szMessage[4096];
	STRINGFORMAT(szMessage, "[%08X][%s] %s", hr, DXLog.ResultToString(hr), szError);

	MessageBoxA(0, szMessage, "Graphic Engine Error!", MB_OK | MB_ICONEXCLAMATION);

	Log(szError);
}

std::vector<char> LoadResourceProvider(const char* pszFileName)
{
	return PackageFile::Get().GetFileBufferCached(pszFileName);
}

BOOL DXGraphicEngine::InitD3D(HWND hWindow, const int screenWidth, const int screenHeight)
{
	auto graphics = Graphics::Graphics::GetInstance();

	graphics->loadResourceProvider = std::bind(&LoadResourceProvider, std::placeholders::_1);
	graphics->animateMeshGate = std::bind(&AnimateMeshGate, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5, std::placeholders::_6);
	graphics->RegisterEvent(Graphics::GraphicEvents::ResetDevice, std::bind(&OnResetDevice));
	graphics->RegisterEvent(Graphics::GraphicEvents::LostDevice, std::bind(&OnLostDevice));
	graphics->GetRenderer()->RegisterEvent(Graphics::RendererEvents::Rendering3D, std::bind(&Render3D));

	camera = new Graphics::Camera();
	frameCamera = new Graphics::Camera();

	dropItemMaterial = new Graphics::Material();
	dropItemMaterial->Load("game\\materials\\items\\dropitem.xml", true);

	itemQuestMaterial = new Graphics::Material();
	itemQuestMaterial->Load("game\\materials\\items\\itemquest100.xml", true);

	for (int i = 0; i < _countof(itemAgingMaterial); i++)
	{
		char szFilePath[256] = { 0 };
		wsprintfA(szFilePath, "game\\materials\\items\\aging\\level%d.xml", i + 4);

		itemAgingMaterial[i] = new Graphics::Material();
		itemAgingMaterial[i]->Load(szFilePath, true);
	}

	for (int i = 0; i < _countof(itemMixingMaterial); i++)
	{
		char szFilePath[256] = { 0 };
		wsprintfA(szFilePath, "game\\materials\\items\\mixing\\mixing%d.xml", i + 1);

		itemMixingMaterial[i] = new Graphics::Material();
		itemMixingMaterial[i]->Load(szFilePath, true);
	}

	// Rectangle UI
	if (!bCreatedRectangle)
	{
		D3DXCreateLine(GRAPHICENGINE->GetDevice(), &pRectangleUI);
		bCreatedRectangle = TRUE;
	}

	pGraphicsDevice = std::make_shared<DX::GraphicsDevice>(lpD3DDev);

	return TRUE;
}

void SaveDeviceFromGraphics()
{
	GRAPHICENGINE->lpD3DDev = Graphics::Graphics::GetInstance()->GetDevice();
	CopyMemory(&GRAPHICENGINE->d3dPP, &Graphics::Graphics::GetInstance()->presentParams, sizeof(D3DPRESENT_PARAMETERS));
	/* CopyMemory(&GRAPHICENGINE->d3dDM, &Graphics::Graphics::GetInstance()->displayMode, sizeof(D3DDISPLAYMODE));
	CopyMemory(&GRAPHICENGINE->d3dCaps, &Graphics::Graphics::GetInstance()->deviceCaps, sizeof(D3DCAPS9));
	CopyMemory(&GRAPHICENGINE->lpD3D, &Graphics::Graphics::GetInstance()->interfaced3d, sizeof(IDirect3D9)); */
}

BOOL SetDisplayMode(HWND hWnd, DWORD Width, DWORD Height, DWORD BPP)
{
	extern int ConfigUseDynamicShadows;
	auto graphics = Graphics::Graphics::GetInstance();

	//Initialize Graphics
	if (!graphics->SetMode(Width, Height, WindowMode, BPP, false, false, hWnd)) 
	{
		ImGui_ImplDX9_CreateDeviceObjects();
		return FALSE;
	}

	smTextureBPP = BPP;
	smScreenWidth = Width;
	smScreenHeight = Height;

	//Setup Camera
	if (camera)
	{
		graphics->GetRenderer()->SetRenderShadowMap(ConfigUseDynamicShadows);
		graphics->GetRenderer()->SetViewport(Graphics::Viewport(Width, Height));
		graphics->GetRenderer()->PushCamera(camera);
		graphics->GetRenderer()->SetFogEnd(1500.f);
	}

	//Save Device
	SaveDeviceFromGraphics();

	return TRUE;
}


void DXGraphicEngine::DrawRectangle2D(Rectangle2D rRect, DWORD dwColor)
{
	D3DXVECTOR2 vLine[2];

	pRectangleUI->SetWidth((float)rRect.iWidth);
	pRectangleUI->SetAntialias(FALSE);
	pRectangleUI->SetGLLines(true);

	vLine[0].x = (float)rRect.iX + (float)rRect.iWidth / 2;
	vLine[0].y = (float)rRect.iY;
	vLine[1].x = (float)rRect.iX + (float)rRect.iWidth / 2;
	vLine[1].y = (float)rRect.iY + (float)rRect.iHeight;

	pRectangleUI->Begin();
	pRectangleUI->Draw(vLine, 2, dwColor);
	pRectangleUI->End();
}

void DXGraphicEngine::DrawRectangle2D(Rectangle2D sBox, DWORD dwColor1, DWORD dwColor2)
{
	float u, v, u2, v2;

	//Compute texture coordinates, it's easy
	u = 0.0f;
	v = 0.0f;
	u2 = 1.0f;
	v2 = 1.0f;

	EXEDrawVertex rVertex[4];
	rVertex[0] = { float(sBox.iX), float(sBox.iY), 0, 1, dwColor1, 0, u, v };
	rVertex[1] = { float(sBox.iX + sBox.iWidth), float(sBox.iY), 0, 1, dwColor1, 0, u2, v };
	rVertex[2] = { float(sBox.iX), float(sBox.iY + sBox.iHeight), 0, 1, dwColor2, 0, u, v2 };
	rVertex[3] = { float(sBox.iX + sBox.iWidth), float(sBox.iY + sBox.iHeight), 0, 1, dwColor2, 0, u2, v2 };

	GRAPHICDEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


	GRAPHICDEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);


	GRAPHICDEVICE->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	GRAPHICDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	GRAPHICDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GRAPHICDEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//Texture Stage States
	GRAPHICDEVICE->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	GRAPHICDEVICE->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	GRAPHICDEVICE->SetTexture(0, 0);

	GRAPHICDEVICE->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR);
	GRAPHICDEVICE->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, (LPVOID)rVertex, sizeof(EXEDrawVertex));
}

BOOL DXGraphicEngine::Reset(const int& screenWidth, const int& screenHeight)
{
	if (!lpD3D || !lpD3DDev)
		return FALSE;

	if (screenWidth == d3dPP.BackBufferWidth &&
		screenHeight == d3dPP.BackBufferHeight)
	{
		Log("DXGraphicEngine::Reset(%d, %d) aborted", screenWidth, screenHeight);
		return FALSE;
	}

	//Save current PP
	D3DPRESENT_PARAMETERS d3dOldPP;
	CopyMemory(&d3dOldPP, &d3dPP, sizeof(D3DPRESENT_PARAMETERS));

	//Back Buffer
	if (FAILED(hr = lpD3D->CheckDeviceType(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		d3dDM.Format,
		d3dPP.BackBufferFormat,
		smConfig.WinMode)))
	{
		Error("Reset(): CheckDeviceType() failed");

		if (FAILED(hr = lpD3D->CheckDeviceType(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			d3dDM.Format,
			d3dPP.BackBufferFormat == D3DFMT_X8R8G8B8 ? D3DFMT_X4R4G4B4 : D3DFMT_X8R8G8B8,
			smConfig.WinMode)))
			return FALSE;

		d3dPP.BackBufferFormat = (d3dPP.BackBufferFormat == D3DFMT_X8R8G8B8 ? D3DFMT_X4R4G4B4 : D3DFMT_X8R8G8B8);
	}

	if (d3dPP.BackBufferFormat == D3DFMT_X8R8G8B8)
	{
		//32 bit Back Buffer
		Log("DXGraphicEngine::Reset(): 32 bit Back Buffer");
	}
	else if (d3dPP.BackBufferFormat == D3DFMT_X4R4G4B4)
	{
		//16 bit Back Buffer
		Log("DXGraphicEngine::Reset(): 16 bit Back Buffer");
	}
	else
	{
		Error("DXGraphicEngine::Reset(): Unknown Back Buffer Format");
		return FALSE;
	}

	//New Back Buffer Size
	d3dPP.BackBufferWidth = screenWidth;
	d3dPP.BackBufferHeight = screenHeight;

	//Multi Sample
	d3dPP.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dPP.MultiSampleQuality = 0;

	if (antiAliasing)
	{
		D3DMULTISAMPLE_TYPE d3daMultipleSampleType[] = {
			D3DMULTISAMPLE_16_SAMPLES,
			D3DMULTISAMPLE_15_SAMPLES,
			D3DMULTISAMPLE_14_SAMPLES,
			D3DMULTISAMPLE_13_SAMPLES,
			D3DMULTISAMPLE_12_SAMPLES,
			D3DMULTISAMPLE_11_SAMPLES,
			D3DMULTISAMPLE_10_SAMPLES,
			D3DMULTISAMPLE_9_SAMPLES,
			D3DMULTISAMPLE_8_SAMPLES,
			D3DMULTISAMPLE_7_SAMPLES,
			D3DMULTISAMPLE_6_SAMPLES,
			D3DMULTISAMPLE_5_SAMPLES,
			D3DMULTISAMPLE_4_SAMPLES,
			D3DMULTISAMPLE_3_SAMPLES,
			D3DMULTISAMPLE_2_SAMPLES,
		};

		for (int i = 0; i < _countof(d3daMultipleSampleType); i++)
		{
			D3DMULTISAMPLE_TYPE t = d3daMultipleSampleType[i];
			DWORD q;

			if (SUCCEEDED(hr = lpD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				d3dPP.BackBufferFormat,
				smConfig.WinMode,
				t,
				&q)) &&
				SUCCEEDED(hr = lpD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
					D3DDEVTYPE_HAL,
					d3dPP.AutoDepthStencilFormat,
					smConfig.WinMode,
					t,
					&q)))
			{
				d3dPP.MultiSampleType = t;
				d3dPP.MultiSampleQuality = q - 1;
				break;
			}
		}
	}

	Log("DXGraphicEngine::Reset(): Multi Sample: %d %d", d3dPP.MultiSampleType, d3dPP.MultiSampleQuality);

	//Window specific PP settings
	if (smConfig.WinMode)
	{
		d3dPP.Windowed = TRUE;
		d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}
	else
	{
		d3dPP.Windowed = FALSE;
		d3dPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	}

	d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if (d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_IMMEDIATE)
	{
		Log("DXGraphicEngine::Reset() Presentation Interval Immediate");
		d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}

	if (vSync)
	{
		if (d3dCaps.PresentationIntervals & D3DPRESENT_INTERVAL_ONE)
		{
			Log("DXGraphicEngine::Reset() Presentation Interval One");
			d3dPP.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
	}

	//Lose Device and attempt Reset on new PP
	OnLostDevice();
	ImGui_ImplDX9_InvalidateDeviceObjects();

	if (FAILED(hr = lpD3DDev->Reset(&d3dPP)))
	{
		CopyMemory(&d3dPP, &d3dOldPP, sizeof(D3DPRESENT_PARAMETERS));

		//Log
		Error("DXGraphicEngine::Resize() Device Reset Failed");

		//Yield CPU
		Sleep(400);

		//Reset back to previous (working) PP
		OnLostDevice();

		if (hr == D3DERR_INVALIDCALL)
			IM_ASSERT(0);

		ImGui_ImplDX9_CreateDeviceObjects();

		if (FAILED(hr = lpD3DDev->Reset(&d3dPP)))
		{
			//Log
			Error("DXGraphicEngine::Resize() Device Rollback Failed");

			//Yield CPU
			Sleep(400);
		}
		else
		{
			OnResetDevice();

			//Log
			Log("DXGraphicEngine::Resize() Device Rollbacked");
		}

		return FALSE;
	}

	//Save new mode
	//SETTINGSHANDLER->Set(!Settings::GetInstance()->GetCFG()->bFullscreen, screenWidth, screenHeight);

	InitRender();
	//Reset Device
	OnResetDevice();
	//Log
	Log("DXGraphicEngine::Resize() Device Reset");

	return TRUE;
}

void Render3D()
{
	extern void RenderCharacter();
	RenderCharacter();
}
void OnLostDevice()
{
	SaveDeviceFromGraphics();

	ImGui_ImplDX9_InvalidateDeviceObjects();

	FONTHANDLER->OnLostDevice();
	CFONTHANDLER->Lost();

	// Sprite 2D
	UI::ImageRender::OnLostDevice();

	// Rectangle 2D
	GRAPHICENGINE->pRectangleUI->OnLostDevice();
	GRAPHICENGINE->pGraphicsDevice->OnLostDevice();
}

void OnResetDevice()
{
	SaveDeviceFromGraphics();

	FONTHANDLER->OnResetDevice();
	CFONTHANDLER->Reset();

	// Sprite 2D
	UI::ImageRender::OnResetDevice();

	// Rectangle 2D
	GRAPHICENGINE->pRectangleUI->OnResetDevice();
	GRAPHICENGINE->pGraphicsDevice->OnResetDevice();
}

void DXGraphicEngine::OnResetDevice_()
{
	OnResetDevice();
}

void DXGraphicEngine::OnLostDevice_()
{
	OnLostDevice();
}

void DXGraphicEngine::ReloadPostProcess()
{
}

BOOL DXGraphicEngine::CheckDevice()
{
	return (hr == S_OK);
}


UINT DXGraphicEngine::GetVertexSize(DWORD dwVertexTypeDesc)
{
	UINT uVertexSize = 0;

	//Get Vertex Size
	if (dwVertexTypeDesc == 324)
		uVertexSize = 28;
	else if (dwVertexTypeDesc == 452)
		uVertexSize = sizeof(EXEDrawVertex);
	else if (dwVertexTypeDesc == 708)
		uVertexSize = sizeof(EXEDrawVertex) + 8;
	else if (dwVertexTypeDesc == 964)
		uVertexSize = sizeof(EXEDrawVertex) + 16;
	else if (dwVertexTypeDesc == 1220)
		uVertexSize = sizeof(EXEDrawVertex) + 24;

	return uVertexSize;
}

