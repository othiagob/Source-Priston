#pragma once

#include <Engine/UI/UIInputField.h>
#include <Engine/UI/UIList.h>
#include <Engine/UI/UIImageBox.h>
#include <Engine/UI/UIImageLoader.h>
#include <Engine/UI/UIButton.h>
#include <Engine/UI/UIText.h>
#include <string>
#include <GameInfo.h>
#include <Def.h>

namespace UI
{
	typedef std::shared_ptr<class ChangeWindow> ChangeWindow_ptr;

	class ChangeWindow
	{
	public:
		ChangeWindow();
		virtual ~ChangeWindow();

		void Init();
		void Shutdown();

		void Render();

		void Update(float fTime);

		void OnMouseMove(CMouse* pcMouse);
		BOOL OnMouseClick(CMouse* pcMouse);

		BOOL OnKeyPress(CKeyboard* pcKeyboard);
		BOOL OnKeyChar(CKeyboard* pcKeyboard);

		void OnResolutionChanged();
		void SetCanMove(BOOL value);

		BOOL IsInputFieldActive();
		void SetInputFieldText(std::string strText);


		UI::Window_ptr GetWindow() { return m_pcWindow; }
	protected:
		void OnEnterKeyPressed(UIEventArgs eArgs);
		void CloseWindow(UIEventArgs eArgs);
		void ChangeName(UIEventArgs eArgs);
	private:
		UI::Window_ptr m_pcWindow = nullptr;
	};
}