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
		pImageBGLogin = UI::ImageLoader::LoadImage_("game\\images\\login\\bg1.png"); // Imagem estática
	}

	if (pImageBGLogin)
	{
		UI::ImageRender::Render(pImageBGLogin, (RESOLUTION_WIDTH >> 1) - (pImageBGLogin->GetWidth() >> 1), (RESOLUTION_HEIGHT >> 1) - (pImageBGLogin->GetHeight() >> 1), pImageBGLogin->GetWidth(), pImageBGLogin->GetHeight(), -1);
	}
}
