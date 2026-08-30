#pragma once

#include "Chat.h"
#include <Engine/UI/UIInputField.h>
#include <Engine/UI/UIList.h>
#include <Engine/UI/UIImageBox.h>
#include <Engine/UI/UIImageLoader.h>
#include <Engine/UI/UIButton.h>
#include <Engine/UI/UIText.h>
#include <string>
#include <map>
#include <Windows.h>

#define MAX_CHATTEXTLIST			32

#define MAX_SAVECOMMANDSCHAT		10

#define MAX_EMOTELIST			36

#define MAX_LISTCHAT				7

class	smCHAR;

struct ItemLinkData;
struct PacketItemLinkChat;

typedef std::map<std::string, std::string> FriendlyCommands;

namespace CHAT
{
	class Window : public CBaseWindow
	{
		enum EChatBoxTabID
		{
			CHATBOXTABID_All,
			CHATBOXTABID_Clan,
			CHATBOXTABID_Party,
			CHATBOXTABID_Raid,
			CHATBOXTABID_Trade,
			CHATBOXTABID_PM,
		};
		enum EElementsID
		{
			//Windows
			WINDOWID_Main,

			//Lists
			LISTID_ChatAll,
			LISTID_ChatClan,
			LISTID_ChatParty,
			LISTID_ChatRaid,
			LISTID_ChatTrade,
			LISTID_ChatPM,
			LISTID_ChatNotice,

			//Button
			BUTTONID_TabAll,
			BUTTONID_TabClan,
			BUTTONID_TabParty,
			BUTTONID_TabRaid,
			BUTTONID_TabTrade,
			BUTTONID_TabPM,
			BUTTONID_SendMessage,

			//Images
			IMAGEID_ChatBoxBar,

			//Input Field
			INPUTFIELDID_Message,
		};
	public:
		Window();
		Window(EWindow eID);
		virtual ~Window();

		void Init();
		void Shutdown();

		void Render();

		void Update(float fTime);

		void OnMouseMove(CMouse* pcMouse);
		BOOL OnMouseClick(CMouse* pcMouse);
		BOOL OnMouseScroll(CMouse* pcMouse);

		BOOL OnKeyPress(CKeyboard* pcKeyboard);
		BOOL OnKeyChar(CKeyboard* pcKeyboard);

		void OnResolutionChanged();
		void SetCanMove(BOOL value);

		BOOL IsInputFieldActive();
		void SetInputFieldText(std::string strText);
		void AddMessage(std::string strMessage, EChatColor eChatColor = EChatColor::CHATCOLOR_Error, std::string strLighText = "", sITEM* pcItemData = NULL, BOOL bSkipDuplicate = FALSE);
		void AddDebugMessage(const char* format, ...);
		void ToggleNotice(BOOL bOpen);
		void OnEnableChatBox();
		void OnDisableChatBox();

		void SendItemLink(sITEM* pcItemData);
		void OnItemLinkHover(sITEM* pcItemData, UI::Window_ptr pWindowHover, UIEventArgs eArgs);

		void HandlePacket(PacketChatBoxMessage* psPacket);
		void HandlePacket(PacketItemLinkChat* psPacket);

		UI::Window_ptr GetWindow() { return m_pcWindow; }
	protected:
		void BuidWindow();
		void OnSelectChatBoxTab(int iID);
		void OnEnterKeyPressed(UIEventArgs eArgs);
		DWORD GetMessageColorFromType(EChatColor eType);
		void SendMessageChat(std::string strMessage);
		std::string GetSaveCommand(BOOL bUP);
		void AddSaveCommand(std::string strMessage);
		BOOL OnEmoteCommand(std::string& strMessage, smCHAR* pcUnit = NULL);
		BOOL IsEmoteMessage(std::string strMessage, std::string& strReplace, int& iEmoteIndex, EChatColor eChatColor);
		void CheckCollideWindow(BOOL bResolutionChanged);
		void AddRecentPlayer(std::string strMessage);
	private:
		UI::Window_ptr m_pcWindow = nullptr;
		int m_iTabID = 0;

		DWORD m_dwLastMessageTick = 0;
		int m_iTotalMessages = 0;

		FriendlyCommands m_sFriendlyCommands;

		std::vector<std::string> m_vSaveCommand;
		UINT m_uSaveCommandWheel = 0;

		ItemLinkData					saItemLink[MAX_LISTCHAT];
		sITEM							cItemLinkDataWrapper;
		DWORD							dwDelayItemLinkTime = 0;
	};
}