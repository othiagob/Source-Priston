#pragma once

#include "DXRenderer.h"
#include "DXEffect.h"
#include "DXDepthStencilBuffer.h"
#include "DXRenderTarget.h"

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

#include "IGraphics.h"
#include "ICore.h"
#include "IIO.h"
#include "IMath.h"
#include "IResource.h"
#include "IThirdParty.h"
#include "ILegacy.h"

using namespace Delta3D;

namespace DX
{

	class SelectGlow
	{
	public:
		void Init();
		void Shutdown();

		BOOL Begin(D3DCOLOR d3dColor, BOOL bSmallPass);
		void End(BOOL bSmallPass);

		void Draw();

		BOOL IsDisabled() { return bDisabled; }
		void SetDisabled(BOOL bValue) { bDisabled = bValue; }

		std::shared_ptr<Graphics::Shader> GetEffect() { return pEffect; }

		float* GetOffset() { return faOffset; }
		float* GetWeight() { return faWeight; }

	private:
		void GaussianFunc();
		float Gaussian(float x);

	private:
		Renderer_ptr pRenderer;

		std::shared_ptr<Graphics::Shader> pEffect;

		std::shared_ptr<Graphics::DepthStencilBuffer> pDepthStencilBuffer;
		std::shared_ptr<Graphics::RenderTarget> pRenderTarget1;
		std::shared_ptr<Graphics::RenderTarget> pRenderTarget2;

		std::shared_ptr<Graphics::Texture> pTexture;

		std::shared_ptr<Graphics::Sprite> pSprite;

		float faOffset[15];
		float faWeight[15];

		Math::Color cColor;
		BOOL bGotSomething = FALSE;

		BOOL bFirstTime = TRUE;

		BOOL bDisabled = FALSE;

		Graphics::Viewport vOldViewport;
	};

	extern SelectGlow cSelectGlow1; //Characters
	extern SelectGlow cSelectGlow2; //Loot

}