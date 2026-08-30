#include "stdafx.h"
#include "LoginScreen.h"
#include <Def.h>
#include <Engine/UI/UIImageLoader.h>
#include <Engine/Directx/DXVideoRenderer.h>
#include <Login/LoginModel.h>
#include <Settings.h>

UI::Image* p = nullptr;
const std::string CLoginScreen::loginVideoAnimationPath = "game\\textures\\misc\\login.asf";
const std::string CLoginScreen::backgroundSelectorImagePath = "game\\images\\login\\bg_selector.png";
const std::string CLoginScreen::backgroundServersSelectorImagePath = "game\\images\\login\\bg_servers.png";

extern int CurrentWorkState;

CLoginScreen::CLoginScreen()
{
	loginScreenSelector = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH >> 2) - 120, RESOLUTION_HEIGHT - 120, 200, 80));
	loginScreenSelector->SetImage(UI::ImageLoader::LoadImage_(this->backgroundSelectorImagePath.c_str()));

	this->serverSelector = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH / 2) - (395 / 2), (RESOLUTION_HEIGHT / 2), 395, 71));
	this->serverSelector2 = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH / 2) - (395 / 2), (RESOLUTION_HEIGHT / 2), 395, 71));
	//this->serverSelector2->SetImage(UI::ImageLoader::LoadImage(this->backgroundServersSelectorImagePath.c_str()));

	// Seasonal Info Text
	pWindowSeasonalInfo = std::make_shared<UI::Window>(Rectangle2D((RESOLUTION_WIDTH / 2) - (570 / 2), (RESOLUTION_HEIGHT / 2), 570, 84));
	pWindowSeasonalInfo->SetImage(UI::ImageLoader::LoadImage_("game\\images\\login\\seasonal_overlay.png"));
}

CLoginScreen::~CLoginScreen()
{
	loginScreenSelector.reset();
	this->serverSelector.reset();
	this->serverSelector2.reset();
}

BOOL CLoginScreen::Init()
{
	BOOL bImageBasedLogin = FALSE;

	int iCurrentSelectedWorld = 0;
	this->currentSelectedWorld = iCurrentSelectedWorld;

	BOOL isImageBasedLogin = FALSE;

	//Video Selector
	Rectangle2D uiPosition(10, 10, 13, 13);

	this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::REMEMBER_LOGIN, !isImageBasedLogin, "Lembrar ID", this->loginScreenSelector));

	uiPosition.iY += 20;
	this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::IS_IMAGE_BACKGROUND, isImageBasedLogin, "Somente Imagem", this->loginScreenSelector));

	uiPosition.iY += 20;
	this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::IS_VIDEO_BACKGROUND, !isImageBasedLogin, "Login Animado", this->loginScreenSelector));

	uiPosition.iY = 5;
	uiPosition.iX = 165;
	auto babelCheckBox = this->createCheckbox(uiPosition, checkboxData((int)CHECKBOX_ID::WORLD_ID_WORLD, FALSE, "Draco Priston", this->serverSelector2));
	uiPosition.iY += 20;

	this->serverSelector2->GetElement<UI::CheckBox>((int)CHECKBOX_ID::WORLD_ID_WORLD)->SetCheck(TRUE);

	this->currentSelectedLoginRender = isImageBasedLogin ? (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND : (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND;
	onCheckboxClick(isImageBasedLogin ? (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND : (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND, UIEventArgs{});

	return TRUE;
}

void CLoginScreen::Shutdown()
{
	DXVIDEORENDERER->Shutdown();
}

void CLoginScreen::Render3D()
{
	DXVIDEORENDERER->Render();
}

void CLoginScreen::Render2D()
{
	this->serverSelector->SetPosition((RESOLUTION_WIDTH / 2) - 60, (RESOLUTION_HEIGHT / 2) + 90);

	if (GameMode == 1)
	{
		// Position Seasonal Info Text
		pWindowSeasonalInfo->SetPosition((RESOLUTION_WIDTH / 2) - (570 / 2), (RESOLUTION_HEIGHT / 2) - 350);
		this->serverSelector2->SetPosition(-100, -100);

	}

	loginScreenSelector->SetPosition((RESOLUTION_WIDTH >> 2) - 120, RESOLUTION_HEIGHT - 120);
	this->serverSelector->Render(0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0);

	if (GameMode == 1)
	{
		if (CurrentWorkState != 3) // 3 = STATE_SERVER_SELECT
		{
			this->serverSelector2->Render(0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0);

		}
	}

	loginScreenSelector->Render(0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0);
}

void CLoginScreen::OnMouseMove(CMouse* pcMouse)
{
	//SETTINGSHANDLER->GetWindow()->OnMouseMove(pcMouse);

	loginScreenSelector->OnMouseMove(pcMouse, 0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0);
	this->serverSelector2->OnMouseMove(pcMouse, 0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0);
}

BOOL CLoginScreen::OnMouseClick(CMouse* pcMouse)
{
	if (loginScreenSelector->OnMouseClick(pcMouse, 0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0))
		return TRUE;

	if (this->serverSelector2->OnMouseClick(pcMouse, 0, 0, Graphics::Graphics::GetInstance()->GetBackBufferInfo().width, Graphics::Graphics::GetInstance()->GetBackBufferInfo().height, 0, 0))
		return TRUE;

	if (pcMouse->GetEvent() == EMouseEvent::ClickDownL)
		bShowNotify = FALSE;

	return FALSE;
}

void CLoginScreen::onCheckboxClick(int checkboxID, UIEventArgs e)
{
	switch (checkboxID)
	{
	case (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND:
		this->setImageModeBackground();
		break;
	case (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND:
		this->setVideoModeBackground();
		break;
	case (int)CHECKBOX_ID::REMEMBER_LOGIN:
		this->SaveRememberLogin();
		break;
	case (int)CHECKBOX_ID::WORLD_ID_WORLD:
		this->setCurrentWorld(checkboxID);
		break;
	}
}

void CLoginScreen::SaveRememberLogin()
{
	Settings::GetInstance()->bRememberLogin == true ? Settings::GetInstance()->bRememberLogin = false : Settings::GetInstance()->bRememberLogin = true;
	Settings::GetInstance()->Save(false);
}

void CLoginScreen::setVideoModeBackground()
{
	if (!DXVIDEORENDERER->Init(this->loginVideoAnimationPath))
	{
		return;
	}

	DXVIDEORENDERER->SetFPS(60);
	DXVIDEORENDERER->Loop(TRUE);
	DXVIDEORENDERER->SetPosition(DXVIDEORENDERER->GetPosition());
	DXVIDEORENDERER->Run();

	this->isLoginVideo = TRUE;
	this->loginScreenSelector->GetElement<UI::CheckBox>((int)CHECKBOX_ID::IS_VIDEO_BACKGROUND)->SetCheck(TRUE);
	this->loginScreenSelector->GetElement<UI::CheckBox>(this->currentSelectedLoginRender)->SetCheck(FALSE);
	this->currentSelectedLoginRender = (int)CHECKBOX_ID::IS_VIDEO_BACKGROUND;
}

void CLoginScreen::setImageModeBackground()
{
	DXVIDEORENDERER->Shutdown();

	this->isLoginVideo = FALSE;
	this->loginScreenSelector->GetElement<UI::CheckBox>((int)CHECKBOX_ID::IS_IMAGE_BACKGROUND)->SetCheck(TRUE);
	this->loginScreenSelector->GetElement<UI::CheckBox>(this->currentSelectedLoginRender)->SetCheck(FALSE);
	this->currentSelectedLoginRender = (int)CHECKBOX_ID::IS_IMAGE_BACKGROUND;
}

void CLoginScreen::setCurrentWorld(int worldID)
{
	if (this->currentSelectedWorld == worldID)
	{
		this->serverSelector2->GetElement<UI::CheckBox>(worldID)->SetCheck(TRUE);
		return;
	}


	this->serverSelector2->GetElement<UI::CheckBox>(this->currentSelectedWorld)->SetCheck(FALSE);
	this->serverSelector2->GetElement<UI::CheckBox>(worldID)->SetCheck(TRUE);

	this->currentSelectedWorld = worldID;
}

UI::CheckBox_ptr CLoginScreen::createCheckbox(const Rectangle2D& checkBoxRectangle, checkboxData& checkboxInformation)
{
	UI::CheckBox_ptr checkBox = std::make_shared<UI::CheckBox>(checkBoxRectangle);
	checkBox->SetFont("Arial", 16, 6, FALSE, FALSE, -1);
	checkBox->SetText(checkboxInformation.text);
	checkBox->SetID(checkboxInformation.id);
	checkBox->SetCheck(checkboxInformation.state);
	checkBox->SetEvent(UI::Event::Build(std::bind(&CLoginScreen::onCheckboxClick, this, checkboxInformation.id, std::placeholders::_1)));
	checkBox->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\checkbox.png"), UI::ImageLoader::LoadImage_("game\\images\\UI\\checkbox_.png"));
	checkboxInformation.windowSelector->AddElement(checkBox);

	return checkBox;
}
