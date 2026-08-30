#pragma once

#include <d3dx9.h>
#include "sinbaram\\sinLinkHeader.h"
#include <Engine\Directx\\DXSprite.h>

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

#include "IGraphics.h"
#include "ICore.h"
#include "IIO.h"
#include "IMath.h"
#include "IResource.h"
#include "IThirdParty.h"
#include "ILegacy.h"

namespace DX
{
	enum class PostProcessEffect
	{
		None,
		Dead,
	};

	class DXPostProcess
	{
	public:

		DXPostProcess();
		virtual								   ~DXPostProcess();

		BOOL									Init();

		BOOL									Begin();
		void									End();

		void									Draw();

		void									SetEffect(PostProcessEffect eEffect) { eEffectType = eEffect; }

		void									SetIsEnabled(BOOL bValue) { bIsEnabled = bValue; }

	private:
		float									fDeadIntensity;
		BOOL									bIsEnabled;
		BOOL									bGotSomething;

		std::shared_ptr<Graphics::Shader>		pEffect;
		std::shared_ptr<Graphics::Sprite>		pSprite;
		std::shared_ptr<Graphics::RenderTarget> pRenderTarget;

		PostProcessEffect						eEffectType;
	};

	extern DXPostProcess postProcess;
}

