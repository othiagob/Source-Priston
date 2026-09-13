#include "stdafx.h"
#include "LoginModel.h"
#include <Engine/UI/UIBitmapFont.h>
#include <Login/LoginScreen.h>
#include <Engine/UI/UIImageRender.h>
#include <Def.h>
#include <Engine/UI/UIImageLoader.h>

UI::Bitmapfont_ptr pBitmapFont = nullptr;

extern int CurrentWorkState;
extern int NextWorkState;

CLoginModel::CLoginModel()
{
}

CLoginModel::~CLoginModel()
{
}

void CLoginModel::OnRender()
{

}

void CLoginModel::Init()
{

}

void CLoginModel::RenderLoginBG()
{
	if (pImageBGLogin == NULL)
	{
		srand((UINT)time(NULL));
		pImageBGLogin = UI::ImageLoader::LoadImage_("game\\images\\login\\bg1.png"); // Imagem estatica
	}

	if (!pImageBGLogin || !pImageBGLogin->GetTexture())
		return;

	const int imgW = pImageBGLogin->GetWidth();
	const int imgH = pImageBGLogin->GetHeight();
	const int screenW = RESOLUTION_WIDTH;
	const int screenH = RESOLUTION_HEIGHT;
	if (imgW <= 0 || imgH <= 0 || screenW <= 0 || screenH <= 0)
		return;

	// Cover: preenche a tela sem mudar a proporcao da arte (logo nao amassa).
	const float scaleX = (float)screenW / (float)imgW;
	const float scaleY = (float)screenH / (float)imgH;
	const float scale = (scaleX > scaleY) ? scaleX : scaleY;
	const int drawW = (int)((float)imgW * scale + 0.5f);
	const int drawH = (int)((float)imgH * scale + 0.5f);
	const int x = (screenW - drawW) / 2;
	const int y = (screenH - drawH) / 2;

	UI::ImageRender::Render(pImageBGLogin->GetTexture(), x, y, imgW, imgH, 0, 0, (DWORD)-1, scale, scale, 0.0f);
}
