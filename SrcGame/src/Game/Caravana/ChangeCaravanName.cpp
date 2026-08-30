#include "ChangeCaravanName.h"
#include <GameCore.h>

UI::ChangeWindow::ChangeWindow()
{

}
UI::ChangeWindow::~ChangeWindow()
{

}
void UI::ChangeWindow::Init()
{
	m_pcWindow = std::make_shared<UI::Window>(Rectangle2D(0, 0, 225, 150));
	m_pcWindow->SetPosition((RESOLUTION_WIDTH / 2) - 100, (RESOLUTION_HEIGHT / 2) - 80);
	m_pcWindow->SetImage(UI::ImageLoader::LoadImage_("game\\images\\Caravan\\rename_box.png"));

	m_pcWindow->SetMoveBox(Rectangle2D(0, 0, m_pcWindow->GetWidth() - 15, m_pcWindow->GetHeight() - 80));
	m_pcWindow->SetCanMove(FALSE);
	m_pcWindow->SetColorBackground(UI::Color(0, 0, 0, 0));

	UI::Group_ptr p = std::make_shared<UI::Group>();
	m_pcWindow->AddElement(p);

	UI::InputField_ptr pInputMessage = std::make_shared<UI::InputField>(Rectangle2D(20, 62, 210, 23));
	pInputMessage->SetID(1);
	pInputMessage->SetFont("Noto Sans", 16, 0, FALSE, FALSE, -1);
	pInputMessage->SetText("");
	pInputMessage->SetGroup(p);
	pInputMessage->SetSelect(TRUE);
	pInputMessage->SetMaxLength(28);
	pInputMessage->SetTextAlign(EAlign::ALIGN_Left, EAlign::ALIGN_Middle);
	pInputMessage->SetImage(UI::ImageLoader::LoadImage_("game\\images\\caravan\\rename_space.png"));
	pInputMessage->BindActionKeyboard(UI::ActionKeyboard(VK_RETURN, UI::Event::Build(std::bind(&ChangeWindow::OnEnterKeyPressed, this, std::placeholders::_1))));
	m_pcWindow->AddElement(pInputMessage);

	//UI::Button_ptr pButtonChangeName = std::make_shared<UI::Button>(Rectangle2D(200, 62, 25, 23));
	//pButtonChangeName->SetID(2);
	//pButtonChangeName->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\send.png", "game\\images\\UI\\Chatbox\\send_.png"));
	//pButtonChangeName->SetEvent(UI::Event::Build(std::bind(&ChangeWindow::OnEnterKeyPressed, this, std::placeholders::_1)));
	//pButtonChangeName->SetSelect(TRUE);
	//m_pcWindow->AddElement(pButtonChangeName);

	UI::Button_ptr pButtonClose = std::make_shared<UI::Button>(Rectangle2D(125, 94, 100, 40));
	pButtonClose->SetID(3);
	pButtonClose->SetGroup(p);
	pButtonClose->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\Caravan\\button_no.png", "game\\images\\Caravan\\button_no_.png"));
	pButtonClose->SetEvent(UI::Event::Build(std::bind(&ChangeWindow::CloseWindow, this, std::placeholders::_1)));
	pButtonClose->SetSelect(TRUE);
	m_pcWindow->AddElement(pButtonClose);

	UI::Button_ptr pButtonOk = std::make_shared<UI::Button>(Rectangle2D(20, 94, 100, 40));
	pButtonOk->SetID(4);
	pButtonOk->SetGroup(p);
	pButtonOk->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\Caravan\\button_ok.png", "game\\images\\Caravan\\button_ok_.png"));
	pButtonOk->SetEvent(UI::Event::Build(std::bind(&ChangeWindow::ChangeName, this, std::placeholders::_1)));
	pButtonOk->SetSelect(TRUE);
	m_pcWindow->AddElement(pButtonOk);
}

BOOL UI::ChangeWindow::OnKeyPress(CKeyboard* pcKeyboard)
{
	if (!Caravana::GetInstance()->ChangeName)
		return FALSE;

	if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN)
	{
		auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

		if (pInputField && !pInputField->IsSelected())
		{
			pInputField->SetSelected(TRUE);
			pInputField->SetSelect(TRUE);
			pInputField->ClearInput();
			return TRUE;
		}
	}

	if (m_pcWindow->OnKeyPress(pcKeyboard))
		return TRUE;

	return FALSE;
}

void UI::ChangeWindow::ChangeName(UIEventArgs eArgs)
{
	if (!Caravana::GetInstance()->ChangeName)
		return;

	auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

	if (pInputField && Caravana::GetInstance()->ChangeName && IsInputFieldActive())
	{
		if (pInputField->GetText().length() > 0)
		{
			if (Caravana::GetInstance()->myCaravan.timeRemaining > 0)
			{
				struct Caravan_Info
				{
					char caravanName[32];
					int caravanType;
					int timeLeft;
				};

				struct Caravan_Info_Pckg {
					int size;
					int code;
					Caravan_Info Pckg;
				};

				Caravan_Info_Pckg Pckg;

				Pckg.code = 0x43600004;
				Pckg.size = sizeof(Caravan_Info_Pckg);
				sprintf_s(Pckg.Pckg.caravanName, sizeof(Pckg.Pckg.caravanName), "%s", pInputField->GetText().c_str());
				Caravana::GetInstance()->myCaravan.caravanName = pInputField->GetText().c_str();
				Caravana::GetInstance()->myCaravan.CaravanState = CaravanStates::FOLLOW;

				if (smWsockUserServer)
				{
					smWsockUserServer->Send2((char*)&Pckg, Pckg.size, TRUE);
				}

				auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

				if (pInputField)
				{
					pInputField->SetSelected(FALSE);
					pInputField->SetSelect(FALSE);
					pInputField->ClearInput();
				}

				Caravana::GetInstance()->ChangeName = false;
			}

		}
	}
}
void UI::ChangeWindow::CloseWindow(UIEventArgs eArgs)
{
	auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

	if (pInputField && !pInputField->IsSelected())
	{
		pInputField->SetSelected(FALSE);
		pInputField->SetSelect(FALSE);
		pInputField->ClearInput();
	}

	Caravana::GetInstance()->ChangeName = false;
}

void UI::ChangeWindow::OnEnterKeyPressed(UIEventArgs eArgs)
{
	if (!Caravana::GetInstance()->ChangeName)
		return;

	auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

	if (pInputField && Caravana::GetInstance()->ChangeName && IsInputFieldActive())
	{
		if (pInputField->GetText().length() > 0)
		{
			if (Caravana::GetInstance()->myCaravan.timeRemaining > 0)
			{
				struct Caravan_Info
				{
					char caravanName[32];
					int caravanType;
					int timeLeft;
				};

				struct Caravan_Info_Pckg {
					int size;
					int code;
					Caravan_Info Pckg;
				};

				Caravan_Info_Pckg Pckg;

				Pckg.code = 0x43600004;
				Pckg.size = sizeof(Caravan_Info_Pckg);
				sprintf_s(Pckg.Pckg.caravanName, sizeof(Pckg.Pckg.caravanName), "%s", pInputField->GetText().c_str());
				Caravana::GetInstance()->myCaravan.caravanName = pInputField->GetText().c_str();
				Caravana::GetInstance()->myCaravan.CaravanState = CaravanStates::FOLLOW;

				if (smWsockUserServer)
				{
					smWsockUserServer->Send2((char*)&Pckg, Pckg.size, TRUE);
				}

				auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

				if (pInputField)
				{
					pInputField->SetSelected(FALSE);
					pInputField->SetSelect(FALSE);
					pInputField->ClearInput();
				}

				Caravana::GetInstance()->ChangeName = false;
			}

		}
	}
}

void UI::ChangeWindow::SetCanMove(BOOL value)
{
	m_pcWindow->SetCanMove(value);
}
void UI::ChangeWindow::OnResolutionChanged()
{
	m_pcWindow->SetPosition(RESOLUTION_WIDTH / 2, RESOLUTION_HEIGHT / 2);
}

BOOL UI::ChangeWindow::OnKeyChar(CKeyboard* pcKeyboard)
{
	if (!Caravana::GetInstance()->ChangeName)
		return FALSE;

	return m_pcWindow->OnKeyChar(pcKeyboard);
}
BOOL UI::ChangeWindow::IsInputFieldActive()
{
	auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

	return pInputField ? pInputField->IsSelected() : FALSE;
}
void UI::ChangeWindow::SetInputFieldText(std::string strText)
{
	auto pInputField = m_pcWindow->GetElement<UI::InputField>(1);

	pInputField ? pInputField->AddInput(strText) : 0;
}

BOOL UI::ChangeWindow::OnMouseClick(CMouse* pcMouse)
{
	if (!Caravana::GetInstance()->ChangeName)
		return FALSE;

	auto pcInputField = m_pcWindow->GetElement<UI::InputField>(0);

	if (pcInputField)
	{
		if (!pcInputField->IsHover() && pcInputField->IsSelected() && !pcInputField->GetText().length())
		{
			pcInputField->SetSelected(FALSE);
		}
	}

	return m_pcWindow->OnMouseClick(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0) ? TRUE : FALSE;
}
void UI::ChangeWindow::OnMouseMove(CMouse* pcMouse)
{
	m_pcWindow->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}
void UI::ChangeWindow::Shutdown()
{
	if (m_pcWindow)
		m_pcWindow->Clear();
}
void UI::ChangeWindow::Render()
{
	if (CHATBOX->IsInputFieldActive())
		CHATBOX->OnDisableChatBox();

	if (Caravana::GetInstance()->OpenFlag && Caravana::GetInstance()->ChangeName)
		m_pcWindow->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}