#include "def.h"
#include "DX.h"
#include "PostProcess.h"
#include "DXGraphicEngine.h"

namespace DX
{
	DXPostProcess postProcess;

	DXPostProcess::DXPostProcess() : eEffectType(PostProcessEffect::None), bGotSomething(FALSE), bIsEnabled(TRUE), fDeadIntensity(1.0f), pEffect(nullptr), pSprite(nullptr), pRenderTarget(nullptr)
	{
	}

	DXPostProcess::~DXPostProcess()
	{
	}

	void DXPostProcess::Draw()
	{
		//Not Enabled
		if (bIsEnabled == FALSE)
			return;

		if (eEffectType == PostProcessEffect::Dead)
			pEffect->SetTechnique("Dead");
		else
			return;

		if (pEffect->Begin() >= 0)
		{
			if (pSprite->Begin(TRUE))
			{
				if (pEffect->BeginPass(0))
				{
					pSprite->Draw(pRenderTarget);
					pEffect->EndPass();
				}

				pSprite->End();
			}

			pEffect->End();
		}

	}

	BOOL DXPostProcess::Begin()
	{
		//Reset Value
		bGotSomething = FALSE;

		//Not Enabled
		if (bIsEnabled == FALSE)
			return FALSE;

		//No Effect to Apply
		if (eEffectType == PostProcessEffect::None)
			return FALSE;

		//Set Render Target
		if (Graphics::Graphics::GetInstance()->SetRenderTarget(pRenderTarget))
		{
			Graphics::Graphics::GetInstance()->Clear();

			bGotSomething = TRUE;
			return TRUE;
		}

		return FALSE;
	}

	void DXPostProcess::End()
	{
		//Not Enabled
		if (bIsEnabled == FALSE)
			return;

		//No Effect to Apply
		if (eEffectType == PostProcessEffect::None)
			return;

		//Unset Render Target
		if (bGotSomething == TRUE)
			Graphics::Graphics::GetInstance()->UnsetRenderTarget();
	}

	BOOL DXPostProcess::Init()
	{
		//Not Enabled
		if (bIsEnabled == FALSE)
			return FALSE;

		//Initialize Post Process
		pEffect = Graphics::Graphics::GetInstance()->GetShaderFactory()->Create(("game\\scripts\\shaders\\PostProcess.fx"));
		pSprite = Graphics::Graphics::GetInstance()->GetSpriteFactory()->Create();
		pRenderTarget = Graphics::Graphics::GetInstance()->GetRenderTargetFactory()->Create(false);

		return TRUE;
	}
}