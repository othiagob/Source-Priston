#include "ChatWindow.h"
#include <Def.h>

#include "sinbaram\\sinlinkheader.h"
#include <TitleBox/TitleBox.h>
#include "HoBaram\HoNewParticleLinkHeader.h"

DWORD Start = 0;
DWORD Elapsed = 0;

extern std::string GetItemMixNameByCode(int code);

#define SAFE_DELETE_C(x) if( x ) { delete(x); (x) = NULL; }

namespace CHAT
{
	void AddDelayGlobal()
	{
		sSKILL haGlobalChat;

		for (int j = 0; j < SIN_MAX_SKILL; j++)
		{
			if (sSkill[j].CODE == CHATGLOBAL)
			{
				memcpy(&haGlobalChat, &sSkill[j], sizeof(sSKILL));
				haGlobalChat.UseTime = 10; // 10 segundos delay chat global
				sinContinueSkillSet(&haGlobalChat);
				break;
			}
		}
	}

	std::string Window::GetSaveCommand(BOOL bUP)
	{
		std::string str = "";

		if (m_vSaveCommand.size() > 0)
		{
			if (bUP == FALSE)
				str = m_vSaveCommand[(m_uSaveCommandWheel--) % m_vSaveCommand.size()];
			else
				str = m_vSaveCommand[(m_uSaveCommandWheel++) % m_vSaveCommand.size()];

			str.insert(str.length(), " ");
		}
		return str;
	}

	void Window::AddSaveCommand(std::string strMessage)
	{
		if ((strMessage.length() > 2) && (strMessage[0] == '/'))
		{
			std::string str = strMessage.substr(0, strMessage.find(' '));

			BOOL bFound = FALSE;

			for (auto& v : m_vSaveCommand)
			{
				if (v.compare(str.c_str()) == 0)
				{
					bFound = TRUE;
					break;
				}
			}

			if (bFound == FALSE)
			{
				m_vSaveCommand.push_back(str);
				if (m_vSaveCommand.size() > MAX_SAVECOMMANDSCHAT)
					m_vSaveCommand.erase(m_vSaveCommand.begin());
			}
		}
	}

	Window::Window() : CBaseWindow()
	{

	}
	Window::Window(EWindow eID) : CBaseWindow(eID)
	{

	}
	Window::~Window()
	{

	}
	void Window::Init()
	{
		BuidWindow();

		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/sweat", "e1"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/love", "e2"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/angry", "e3"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/bigeye", "e4"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/sad", "e5"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/cry", "e6"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/ops", "e7"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/nerd", "e8"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/craving", "e9"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/hurt", "e10"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/silence", "e11"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/crazy", "e12"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/hello", "e13"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/tired", "e14"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/shame", "e15"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/brokeheart", "e16"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/abused", "e17"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/kappa", "e18"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/kappapride", "e19"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/pogchamp", "e20"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e21", "e21"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e22", "e22"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e23", "e23"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e24", "e24"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e25", "e25"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e26", "e26"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e27", "e27"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e28", "e28"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e29", "e29"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e30", "e30"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e31", "e31"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e32", "e32"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e33", "e33"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e34", "e34"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e35", "e35"));
		m_sFriendlyCommands.insert(FriendlyCommands::value_type("/e36", "e36"));

		for (int i = 0; i < 7; i++)
			saItemLink[i].iID = LISTID_ChatAll + i;

		ZeroMemory(&cItemLinkDataWrapper, sizeof(sITEM));
	}
	void Window::Shutdown()
	{
		if (m_pcWindow)
			m_pcWindow->Clear();
	}
	void Window::Render()
	{
		if (!Caravana::GetInstance()->ChangeName)
		{
			m_pcWindow->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);

			if (cItemLinkDataWrapper.Flag)
			{
				sinShowItemInfoFlag = 1;
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&cItemLinkDataWrapper, FALSE, FALSE, FALSE);
			}

			CheckCollideWindow(FALSE);
		}
	}
	void Window::Update(float fTime)
	{
	}
	void Window::OnMouseMove(CMouse* pcMouse)
	{
		m_pcWindow->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
	}
	void Window::SendItemLink(sITEM* pcItemData)
	{
		if (pcItemData && pcItemData->Flag)
		{
			PacketItemLinkChat sPacket;
			ZeroMemory(&sPacket, sizeof(PacketItemLinkChat));

			sPacket.eChatColor = EChatColor::CHATCOLOR_Normal;

			BOOL bLoginServer = FALSE;
			auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

			if (pInputField)
			{
				std::string strMessage = rtrim(pInputField->GetText());

				if (strMessage.length() > 2)
				{
					//Whisper
					if ((strMessage[0] == '/') && ((strMessage[1] == ':') || (strMessage[1] == ';')))
					{
						size_t iFind = strMessage.find(" ");
						std::string strName = "";
						strName = strMessage.substr(2, iFind != std::string::npos ? (iFind - 2) : std::string::npos);


						if (strName.length() > 0)
						{
							STRINGCOPY(sPacket.szCharacterName, strName.c_str());
							sPacket.eChatColor = EChatColor::CHATCOLOR_Whisper;
							AddRecentPlayer(strMessage);
							pInputField->ClearInput();
						}

						std::string body = "";
						if (iFind != std::string::npos)
						{
							body = strMessage.substr(iFind);
						}

						if (body == "")
						{
							if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT && pcItemData->sItemInfo.iMixType > 0)
							{
								strMessage = FormatString("%s (%s)", pcItemData->sItemInfo.ItemName, GetItemMixNameByCode(pcItemData->sItemInfo.iMixType).c_str());
							}
							else if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_AGING && pcItemData->sItemInfo.ItemAgingNum[0] > 0)
							{
								strMessage = FormatString("%s (+%d)", pcItemData->sItemInfo.ItemName, pcItemData->sItemInfo.ItemAgingNum[0]);
							}
							else
							{
								strMessage = FormatString("%s", pcItemData->sItemInfo.ItemName);
							}
						}
						else
						{
							strMessage = body;
						}
					}
				}

				if (strMessage.length() >= 2)
				{
					//S>
					if ((strMessage[0] == 'S' || strMessage[0] == 's') && strMessage[1] == '>')
					{
						if (strMessage.length() > 2)
						{
							std::string body = trim(strMessage.substr(2));

							//matches: 12k, 1k, 123k, 1.2kk, 2kk, 3kkk, 1.2k, 123KK
							std::regex e("^(\\d*\\.)?\\d+k+$", std::regex_constants::icase);

							//exact match? i.e. S> 1.2kk
							if (std::regex_match(body, e))
							{
								if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT && pcItemData->sItemInfo.iMixType > 0)
								{
									strMessage = FormatString("S> %s (%s) -- %s", pcItemData->sItemInfo.ItemName, GetItemMixNameByCode(pcItemData->sItemInfo.iMixType).c_str(), body.c_str());
								}
								else if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_AGING && pcItemData->sItemInfo.ItemAgingNum[0] > 0)
								{
									strMessage = FormatString("S> %s (+%d) -- %s", pcItemData->sItemInfo.ItemName, pcItemData->sItemInfo.ItemAgingNum[0], body.c_str());
								}
								else
								{
									strMessage = FormatString("S> %s -- %s", pcItemData->sItemInfo.ItemName, body.c_str());
								}
							}
						}

						//S> or s>
						else
						{
							if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT && pcItemData->sItemInfo.iMixType > 0)
							{
								strMessage = FormatString("S> %s (%s)", pcItemData->sItemInfo.ItemName, GetItemMixNameByCode(pcItemData->sItemInfo.iMixType).c_str());
							}
							else if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_AGING && pcItemData->sItemInfo.ItemAgingNum[0] > 0)
							{
								strMessage = FormatString("S> %s (+%d)", pcItemData->sItemInfo.ItemName, pcItemData->sItemInfo.ItemAgingNum[0]);
							}
							else
							{
								strMessage = FormatString("S> %s", pcItemData->sItemInfo.ItemName);
							}
						}
					};
				}

				//empty string
				else if (strMessage == "")
				{
					if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_CRAFT && pcItemData->sItemInfo.iMixType > 0)
					{
						strMessage = FormatString("%s (%s)", pcItemData->sItemInfo.ItemName, GetItemMixNameByCode(pcItemData->sItemInfo.iMixType).c_str());
					}
					else if (pcItemData->sItemInfo.ItemKindCode == ITEM_KIND_AGING && pcItemData->sItemInfo.ItemAgingNum[0] > 0)
					{
						strMessage = FormatString("%s (+%d)", pcItemData->sItemInfo.ItemName, pcItemData->sItemInfo.ItemAgingNum[0]);
					}
					else
					{
						strMessage = FormatString("%s", pcItemData->sItemInfo.ItemName);
					}
				}


				if (strMessage[0] == '/' || strMessage[0] == '$')
				{
					if ((strMessage.compare(0, 7, "/TRADE>") == 0 && strMessage.length() > 8) || (strMessage[0] == '$' && strMessage.length() > 2))
					{
						sPacket.eChatColor = EChatColor::CHATCOLOR_Trade;

						bLoginServer = TRUE;

						if (strMessage[0] == '$')
						{
							strMessage.replace(0, 1, "/TRADE>");
						}
					}

					else if ((strMessage.compare(0, 6, "/CLAN>") == 0 && strMessage.length() > 7))
					{
						sPacket.eChatColor = EChatColor::CHATCOLOR_Clan;
						bLoginServer = TRUE;
					}
				}
				else
				{
					if (m_iTabID == CHATBOXTABID_Clan)
					{
						strMessage.insert(0, "/CLAN>");
						sPacket.eChatColor = EChatColor::CHATCOLOR_Clan;
						bLoginServer = TRUE;
					}
					else if (m_iTabID == CHATBOXTABID_Party)
					{
						sPacket.eChatColor = EChatColor::CHATCOLOR_Party;
					}
					else if (m_iTabID == CHATBOXTABID_Raid)
					{
						sPacket.eChatColor = EChatColor::CHATCOLOR_Raid;
					}
					else if (m_iTabID == CHATBOXTABID_Trade)
					{
						strMessage.insert(0, "/TRADE>");
						sPacket.eChatColor = EChatColor::CHATCOLOR_Trade;
						bLoginServer = TRUE;
					}
				}

				STRINGCOPY(sPacket.szMessage, strMessage.c_str());
			}

			int len = EecodeCompress((BYTE*)pcItemData, sPacket.trans, sizeof(sITEM));
			sPacket.size = len + 300;
			CHATGAMEHANDLE->ProcessPacket(&sPacket, bLoginServer);
		}
	}
	BOOL Window::OnMouseClick(CMouse* pcMouse)
	{
		auto pcInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

		if (pcInputField)
		{
			if (!pcInputField->IsHover() && pcInputField->IsSelected() && !pcInputField->GetText().length())
			{
				pcInputField->SetSelected(FALSE);
				pcInputField->SetSelect(FALSE);
			}
		}

		if ((pcMouse->GetEvent() == EMouseEvent::ClickDownL) && KEYBOARDHANDLER->IsKeyDown(VK_CONTROL) && pcInputField->IsOpen())
		{
			if (GetTickCount() > dwDelayItemLinkTime)
			{
				SendItemLink(&cInvenTory.InvenItem[SelectInvenItemIndex - 1]);
				dwDelayItemLinkTime = GetTickCount() + 2000;
				pcInputField->ClearInput();
				return TRUE;
			}
			else
			{
				return TRUE;
			}
		}

		return m_pcWindow->OnMouseClick(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0) ? TRUE : FALSE;
	}
	BOOL Window::OnMouseScroll(CMouse* pcMouse)
	{
		return m_pcWindow->OnMouseScroll(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0) ? TRUE : FALSE;
	}
	BOOL Window::OnKeyPress(CKeyboard* pcKeyboard)
	{
		if (Caravana::GetInstance()->ChangeName)
			return FALSE;

		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_ESCAPE && !pcKeyboard->IsKeyDown(VK_SHIFT) && m_pcWindow->IsOpen())
		{
			auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

			if (pInputField)
			{
				pInputField->SetSelected(FALSE);
				pInputField->ClearInput();
			}

			m_pcWindow->Hide();
			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN && !m_pcWindow->IsOpen())
		{
			OnEnableChatBox();
			m_pcWindow->Show();
			GAMECOREHANDLE->SetFocus(this);
			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_RETURN && !IsInputFieldActive())
		{
			OnEnableChatBox();
			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_TAB && IsInputFieldActive())
		{
			auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);
			if (pInputField)
			{
				pInputField->ClearInput();
				pInputField->AddInput("/CLAN> ");
			}
			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_UP && IsInputFieldActive())
		{
			auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);
			if (pInputField)
			{
				pInputField->ClearInput();
				pInputField->AddInput(GetSaveCommand(TRUE));
			}
			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_DOWN && IsInputFieldActive())
		{
			auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);
			if (pInputField)
			{
				pInputField->ClearInput();
				pInputField->AddInput(GetSaveCommand(FALSE));
			}
			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_PRIOR && IsInputFieldActive())
		{
			for (size_t i = 0; i < 6; i++)
			{
				auto pListChat = m_pcWindow->GetElement<UI::List>(LISTID_ChatAll + i);
				if (pListChat)
				{
					if (pListChat->IsOpen())
					{
						auto pScroll = pListChat->GetScrollBar();
						if (pScroll)
						{
							int iPercent = pScroll->GetPercent() - 5;
							if (iPercent < 0)
								iPercent = 0;

							pScroll->SetPercent(iPercent);
						}
						break;
					}
				}
			}

			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_NEXT && IsInputFieldActive())
		{
			for (size_t i = 0; i < 6; i++)
			{
				auto pListChat = m_pcWindow->GetElement<UI::List>(LISTID_ChatAll + i);
				if (pListChat)
				{
					if (pListChat->IsOpen())
					{
						auto pScroll = pListChat->GetScrollBar();
						if (pScroll)
						{
							int iPercent = pScroll->GetPercent() + 5;
							if (iPercent > 100)
								iPercent = 100;

							pScroll->SetPercent(iPercent);
						}
						break;
					}
				}
			}

			return TRUE;
		}
		if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->GetKey() == VK_DELETE && IsInputFieldActive()) // Delete
		{
			auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

			if (pInputField)
				pInputField->ClearInput();

			return TRUE;
		}
		/*if (pcKeyboard->GetEvent() == KeyDown && pcKeyboard->IsKeyDown(VK_CONTROL) && (pcKeyboard->GetKey() >= VK_F1 && pcKeyboard->GetKey() <= VK_F10))
		{
			char* pszMacroText = SETTINGHANDLE->Get().szMacro[(pcKeyboard->GetKey() & 0x0F)];

			if (pszMacroText[0] != NULL)
			{
				OnEnableChatBox();
				auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);
				if (pInputField)
					pInputField->AddInput(SETTINGHANDLE->Get().szMacro[(pcKeyboard->GetKey() & 0x0F)]);

				m_pcWindow->Show();
				return TRUE;
			}
		} */
		if (m_pcWindow->OnKeyPress(pcKeyboard))
			return TRUE;

		return FALSE;
	}
	void Window::OnEnableChatBox()
	{
		if (sinMessageBoxShowFlag)
			return;

		auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

		if (pInputField)
		{
			pInputField->SetSelected(TRUE);
			pInputField->SetSelect(TRUE);
			pInputField->ClearInput();
		}
	}
	void Window::OnDisableChatBox()
	{
		auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

		if (pInputField)
		{
			pInputField->SetSelected(FALSE);
			pInputField->SetSelect(FALSE);
			pInputField->ClearInput();
		}
	}
	BOOL Window::IsInputFieldActive()
	{
		auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

		return pInputField ? pInputField->IsSelected() : FALSE;
	}
	void Window::SetInputFieldText(std::string strText)
	{
		auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

		pInputField ? pInputField->AddInput(strText) : 0;
	}
	BOOL Window::OnKeyChar(CKeyboard* pcKeyboard)
	{
		if (Caravana::GetInstance()->ChangeName)
			return FALSE;

		return m_pcWindow->OnKeyChar(pcKeyboard);
	}
	void Window::SetCanMove(BOOL value)
	{
		m_pcWindow->SetCanMove(value);
	}
	void Window::OnResolutionChanged()
	{
		m_pcWindow->SetPosition(4, RESOLUTION_HEIGHT - m_pcWindow->GetHeight() - 4);

		CheckCollideWindow(TRUE);
	}
	void Window::BuidWindow()
	{
		m_pcWindow = std::make_shared<UI::Window>(Rectangle2D(0, 0, 295, 218));
		m_pcWindow->SetPosition(4, RESOLUTION_HEIGHT - m_pcWindow->GetHeight() - 4);

		m_pcWindow->SetMoveBox(Rectangle2D(0, 0, m_pcWindow->GetWidth() - 15, m_pcWindow->GetHeight() - 40));
		m_pcWindow->SetCanMove(Settings::GetInstance()->UiBlock());

		m_pcWindow->SetColorBackground(UI::Color(70, 0, 0, 0));

		UI::ImageBox_ptr pImageChatBoxBar = std::make_shared<UI::ImageBox>(Rectangle2D(0, 66, 295, 3));
		pImageChatBoxBar->SetID(IMAGEID_ChatBoxBar);
		pImageChatBoxBar->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\Chatbox\\chatbar.png"));
		m_pcWindow->AddElement(pImageChatBoxBar);

		for (size_t i = 0; i < 6; i++)
		{
			UI::List_ptr pListChat = std::make_shared<UI::List>(Rectangle2D(0, 74, 280, 100));
			pListChat->SetID(LISTID_ChatAll + i);
			pListChat->MakeScrollBar(Rectangle2D(285, 0, 6, 105), "game\\images\\UI\\Scroll\\barbg_v.png", "game\\images\\UI\\Scroll\\icon.png");
			pListChat->SetForceDown(TRUE);
			pListChat->SetScrollFirst(TRUE);

			pListChat->GetID() != LISTID_ChatAll ? pListChat->Hide() : 0;

			m_iTabID = CHATBOXTABID_All;

			m_pcWindow->AddElement(pListChat);
		}

		UI::List_ptr pListChatNotice = std::make_shared<UI::List>(Rectangle2D(0, 2, 280, 62));
		pListChatNotice->SetID(LISTID_ChatNotice);
		pListChatNotice->MakeScrollBar(Rectangle2D(285, 2, 6, 54), "game\\images\\UI\\Scroll\\barbg_v.png", "game\\images\\UI\\Scroll\\icon.png");
		pListChatNotice->SetForceDown(TRUE);
		m_pcWindow->AddElement(pListChatNotice);

		UI::Group_ptr p = std::make_shared<UI::Group>();
		m_pcWindow->AddElement(p);

		UI::Group_ptr pGroupTabs = std::make_shared<UI::Group>();
		m_pcWindow->AddElement(pGroupTabs);

		int iBaseX = 3;
		int iBaseY = 177;

		UI::Button_ptr pButtonTabAll = std::make_shared<UI::Button>(Rectangle2D(iBaseX, iBaseY, 43, 15));
		pButtonTabAll->SetGroup(pGroupTabs);
		pButtonTabAll->SetID(BUTTONID_TabAll);
		pButtonTabAll->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\all.png", "game\\images\\UI\\Chatbox\\all_.png"));
		pButtonTabAll->SetEvent(UI::Event::Build(std::bind(&Window::OnSelectChatBoxTab, this, pButtonTabAll->GetID())));
		//pButtonTabAll->SetAnimation(TRUE);
		pButtonTabAll->SetSelect(TRUE);
		pButtonTabAll->SetSelected(TRUE);
		m_pcWindow->AddElement(pButtonTabAll);

		UI::Button_ptr pButtonTabClan = std::make_shared<UI::Button>(Rectangle2D(iBaseX + 45, iBaseY, 43, 15));
		pButtonTabClan->SetGroup(pGroupTabs);
		pButtonTabClan->SetID(BUTTONID_TabClan);
		pButtonTabClan->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\clan.png", "game\\images\\UI\\Chatbox\\clan_.png"));
		pButtonTabClan->SetEvent(UI::Event::Build(std::bind(&Window::OnSelectChatBoxTab, this, pButtonTabClan->GetID())));
		//pButtonTabClan->SetAnimation(TRUE);
		pButtonTabClan->SetSelect(TRUE);
		m_pcWindow->AddElement(pButtonTabClan);

		UI::Button_ptr pButtonTabParty = std::make_shared<UI::Button>(Rectangle2D(iBaseX + 90, iBaseY, 43, 15));
		pButtonTabParty->SetGroup(pGroupTabs);
		pButtonTabParty->SetID(BUTTONID_TabParty);
		pButtonTabParty->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\party.png", "game\\images\\UI\\Chatbox\\party_.png"));
		pButtonTabParty->SetEvent(UI::Event::Build(std::bind(&Window::OnSelectChatBoxTab, this, pButtonTabParty->GetID())));
		//pButtonTabParty->SetAnimation(TRUE);
		pButtonTabParty->SetSelect(TRUE);
		m_pcWindow->AddElement(pButtonTabParty);

		UI::Button_ptr pButtonTabRaid = std::make_shared<UI::Button>(Rectangle2D(iBaseX + 135, iBaseY, 43, 15));
		pButtonTabRaid->SetGroup(pGroupTabs);
		pButtonTabRaid->SetID(BUTTONID_TabRaid);
		pButtonTabRaid->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\raid.png", "game\\images\\UI\\Chatbox\\raid_.png"));
		pButtonTabRaid->SetEvent(UI::Event::Build(std::bind(&Window::OnSelectChatBoxTab, this, pButtonTabRaid->GetID())));
		//pButtonTabRaid->SetAnimation(TRUE);
		pButtonTabRaid->SetSelect(TRUE);
		m_pcWindow->AddElement(pButtonTabRaid);

		UI::Button_ptr pButtonTabTrade = std::make_shared<UI::Button>(Rectangle2D(iBaseX + 180, iBaseY, 43, 15));
		pButtonTabTrade->SetGroup(pGroupTabs);
		pButtonTabTrade->SetID(BUTTONID_TabTrade);
		pButtonTabTrade->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\trade.png", "game\\images\\UI\\Chatbox\\trade_.png"));
		pButtonTabTrade->SetEvent(UI::Event::Build(std::bind(&Window::OnSelectChatBoxTab, this, pButtonTabTrade->GetID())));
		//pButtonTabTrade->SetAnimation(TRUE);
		pButtonTabTrade->SetSelect(TRUE);
		m_pcWindow->AddElement(pButtonTabTrade);

		UI::Button_ptr pButtonTabPM = std::make_shared<UI::Button>(Rectangle2D(iBaseX + 225, iBaseY, 43, 15));
		pButtonTabPM->SetGroup(pGroupTabs);
		pButtonTabPM->SetID(BUTTONID_TabPM);
		pButtonTabPM->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\pm.png", "game\\images\\UI\\Chatbox\\pm_.png"));
		pButtonTabPM->SetEvent(UI::Event::Build(std::bind(&Window::OnSelectChatBoxTab, this, pButtonTabPM->GetID())));
		//pButtonTabPM->SetAnimation(TRUE);
		pButtonTabPM->SetSelect(TRUE);
		m_pcWindow->AddElement(pButtonTabPM);

		UI::InputField_ptr pInputMessage = std::make_shared<UI::InputField>(Rectangle2D(0, 195, 270, 23));
		pInputMessage->SetID(INPUTFIELDID_Message);
		pInputMessage->SetGroup(p);
		pInputMessage->SetFont("Noto Sans", 16, 0, FALSE, FALSE, -1);
		pInputMessage->SetText("");
		pInputMessage->SetSelect(TRUE);
		pInputMessage->SetMaxLength(MAX_CHATLENGTH);
		pInputMessage->SetTextAlign(EAlign::ALIGN_Left, EAlign::ALIGN_Middle);
		pInputMessage->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\Chatbox\\input.png"));
		pInputMessage->BindActionKeyboard(UI::ActionKeyboard(VK_RETURN, UI::Event::Build(std::bind(&Window::OnEnterKeyPressed, this, std::placeholders::_1))));
		m_pcWindow->AddElement(pInputMessage);

		UI::Button_ptr pButtonSendMessage = std::make_shared<UI::Button>(Rectangle2D(270, 195, 25, 23));
		pButtonSendMessage->SetID(BUTTONID_SendMessage);
		pButtonSendMessage->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\Chatbox\\send.png", "game\\images\\UI\\Chatbox\\send_.png"));
		pButtonSendMessage->SetEvent(UI::Event::Build(std::bind(&Window::OnEnterKeyPressed, this, std::placeholders::_1)));
		pButtonSendMessage->SetSelect(TRUE);
		m_pcWindow->AddElement(pButtonSendMessage);

	}
	void Window::CheckCollideWindow(BOOL bResolutionChanged)
	{
		if (((m_pcWindow->GetY() + m_pcWindow->GetHeight() > RESOLUTION_HEIGHT - 175) && (m_pcWindow->GetX() < 800)) || bResolutionChanged)
		{
			if (cInvenTory.OpenFlag && sinMoveKindInter[SIN_INVENTORY] > 0 && (sinMoveKindInter[SIN_INVENTORY] != 256 || bResolutionChanged))
				m_pcWindow->SetPosition(m_pcWindow->GetX(), (205 + (256 - sinMoveKindInter[SIN_INVENTORY])) + (RESOLUTION_HEIGHT - 600));

			if (CSKILL->OpenFlag && sinMoveKindInter[SIN_SKILL] > 0 && (sinMoveKindInter[SIN_SKILL] != 256 || bResolutionChanged))
				m_pcWindow->SetPosition(m_pcWindow->GetX(), (205 + (256 - sinMoveKindInter[SIN_SKILL])) + (RESOLUTION_HEIGHT - 600));

			if (cCharStatus.OpenFlag && sinMoveKindInter[SIN_CHARSTATUS] > 0 && (sinMoveKindInter[SIN_CHARSTATUS] != 256 || bResolutionChanged))
				m_pcWindow->SetPosition(m_pcWindow->GetX(), (205 + (256 - sinMoveKindInter[SIN_CHARSTATUS])) + (RESOLUTION_HEIGHT - 600));
		}

		if (!cInvenTory.OpenFlag && !CSKILL->OpenFlag && !cCharStatus.OpenFlag)
		{
			if (m_pcWindow->GetY() + m_pcWindow->GetHeight() > RESOLUTION_HEIGHT - 180 && m_pcWindow->GetX() + m_pcWindow->GetWidth() < 300)
			{
				if (sinMoveKindInter[SIN_INVENTORY] > 0 && sinMoveKindInter[SIN_INVENTORY] < 256)
					m_pcWindow->SetPosition(m_pcWindow->GetX(), RESOLUTION_HEIGHT - m_pcWindow->GetHeight() - 4);

				if (sinMoveKindInter[SIN_SKILL] > 0 && sinMoveKindInter[SIN_SKILL] < 256)
					m_pcWindow->SetPosition(m_pcWindow->GetX(), RESOLUTION_HEIGHT - m_pcWindow->GetHeight() - 4);

				if (sinMoveKindInter[SIN_CHARSTATUS] > 0 && sinMoveKindInter[SIN_CHARSTATUS] < 256)
					m_pcWindow->SetPosition(m_pcWindow->GetX(), RESOLUTION_HEIGHT - m_pcWindow->GetHeight() - 4);
			}
		}
	}
	void Window::OnSelectChatBoxTab(int iID)
	{
		for (size_t i = 0; i < 6; i++)
		{
			auto pListChat = m_pcWindow->GetElement<UI::List>(LISTID_ChatAll + i);
			if (pListChat)
			{
				pListChat->Hide();

				auto pScroll = pListChat->GetScrollBar();
				if (pScroll)
					pScroll->SetPercent(100);
			}
		}

		auto pListChat = m_pcWindow->GetElement<UI::List>(LISTID_ChatAll + (iID - BUTTONID_TabAll));
		if (pListChat)
		{
			m_iTabID = iID - BUTTONID_TabAll;
			pListChat->Show();
		}
	}
	void Window::OnEnterKeyPressed(UIEventArgs eArgs)
	{
		auto pInputField = m_pcWindow->GetElement<UI::InputField>(INPUTFIELDID_Message);

		if (pInputField)
		{
			if (pInputField->GetText().length() > 0)
			{
				AddSaveCommand(pInputField->GetText());
				SendMessageChat(pInputField->GetText());

				//PM?
				if ((pInputField->GetText().length() > 2) && (pInputField->GetText()[0] == '/') && ((pInputField->GetText()[1] == ':') || (pInputField->GetText()[1] == ';')))
				{
					//Set Name
					if (pInputField->GetText().find(' ') != std::string::npos)
					{
						BOOL bAddBack = (pInputField->GetText().find(' ') + 1) == pInputField->GetText().length() ? FALSE : TRUE;

						std::string str = pInputField->GetText().substr(0, pInputField->GetText().find(' ') + 1);

						//Add it back
						pInputField->ClearInput();
						if (bAddBack)
							pInputField->AddInput(str);

						return;
					}
				}

				if ((pInputField->GetText().length() > 7) && (pInputField->GetText()[0] == '/' && pInputField->GetText()[5] == '>') && pInputField->GetText().find("/CLAN>") != std::string::npos)
				{
					pInputField->ClearInput();
					pInputField->AddInput("/CLAN> ");
					return;
				}
			}
			else
				pInputField->SetSelected(FALSE);

			pInputField->ClearInput();
		}
	}
	void Window::OnItemLinkHover(sITEM* pcItemData, UI::Window_ptr pWindowHover, UIEventArgs eArgs)
	{
		if (pWindowHover->IsHover())
		{
			if ((cItemLinkDataWrapper.sItemInfo.ItemHeader.Head != pcItemData->sItemInfo.ItemHeader.Head) ||
				(cItemLinkDataWrapper.sItemInfo.ItemHeader.dwChkSum != pcItemData->sItemInfo.ItemHeader.dwChkSum))
			{
				CopyMemory(&cItemLinkDataWrapper, pcItemData, sizeof(sITEM));

				cItemLinkDataWrapper.x = m_pcWindow->GetX() + (m_pcWindow->GetWidth() >> 1);
				cItemLinkDataWrapper.y = MOUSEHANDLER->GetPosition()->iY;
			}

			cItemLinkDataWrapper.Flag = TRUE;
		}
		else
		{
			sinShowItemInfoFlag = FALSE;
			cItemLinkDataWrapper.Flag = FALSE;
		}
	}
	// Ocultar ou não o alerta
	void Window::ToggleNotice(BOOL bOpen)
	{
		int iY = 0;
		int iHeight = 0;

		auto pImageBoxChatBar = m_pcWindow->GetElement<UI::ImageBox>(IMAGEID_ChatBoxBar);
		auto pListChatNotice = m_pcWindow->GetElement<UI::List>(LISTID_ChatNotice);

		if (!pImageBoxChatBar || !pListChatNotice)
			return;

		if (bOpen)
		{
			iY = 74;
			iHeight = 100;
			pImageBoxChatBar->Show();
			pListChatNotice->Show();
		}
		else
		{
			iY = 8;
			iHeight = 166;
			pImageBoxChatBar->Hide();
			pListChatNotice->Hide();
		}

		for (size_t i = 0; i < 6; i++)
		{
			auto pListChat = m_pcWindow->GetElement<UI::List>(LISTID_ChatAll + i);
			if (pListChat)
			{
				pListChat->SetBox(Rectangle2D(pListChat->GetX(), iY, pListChat->GetWidth(), iHeight));
				pListChat->GetScrollBar()->SetHeight(iHeight + 5);
				pListChat->GetScrollBar()->SetPercent(100);
			}
		}
	}

	void Window::AddDebugMessage(const char* format, ...)
	{
		char szBuf[256];
		ZeroMemory(szBuf, 256);
		va_list ap;
		va_start(ap, format);
		_vsnprintf_s(szBuf, 256, 255, format, ap);
		va_end(ap);

		AddMessage(FormatString("DEBUG > %s", szBuf), EChatColor::CHATCOLOR_Normal);
	}
	void Window::AddMessage(std::string strMessage, EChatColor eChatColor, std::string strLighText, sITEM* pcItemData, BOOL bSkipDuplicate)
	{
		if (!m_pcWindow)
			return;

		std::vector<UI::List_ptr> vListToAdd;

		if (eChatColor != EChatColor::CHATCOLOR_Notice)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatAll));
		else if (eChatColor == EChatColor::CHATCOLOR_Notice)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatNotice));

		if (eChatColor == EChatColor::CHATCOLOR_Party)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatParty));
		else if (eChatColor == EChatColor::CHATCOLOR_Trade || eChatColor == EChatColor::CHATCOLOR_Global)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatTrade));
		else if (eChatColor == EChatColor::CHATCOLOR_Clan || eChatColor == EChatColor::CHATCOLOR_ClanLeader)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatClan));
		else if (eChatColor == EChatColor::CHATCOLOR_Whisper || eChatColor == EChatColor::CHATCOLOR_Blue || eChatColor == EChatColor::CHATCOLOR_Error)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatPM));
		else if (eChatColor == EChatColor::CHATCOLOR_Raid)
			vListToAdd.push_back(m_pcWindow->GetElement<UI::List>(LISTID_ChatRaid));

		if (vListToAdd.size() > 0)
		{
			for (auto& v : vListToAdd)
			{
				if (v)
				{
					UI::Window_ptr pBaseWindow = std::make_shared <UI::Window>(Rectangle2D(5, 0, 275, 0));
					UI::Text_ptr pTextMessage = std::make_shared<UI::Text>(Rectangle2D(0, 0, 0, 0));

					sITEM* pcItemLinkWrapper = new sITEM();

					if (pcItemData)
						CopyMemory(pcItemLinkWrapper, pcItemData, sizeof(sITEM));
					else
						ZeroMemory(pcItemLinkWrapper, sizeof(sITEM));

					for (int i = 0; i < MAX_LISTCHAT; i++)
					{
						if (saItemLink[i].iID == v->GetID())
						{
							saItemLink[i].vItemData.push_back(pcItemLinkWrapper);
							break;
						}
					}

					if (eChatColor == EChatColor::CHATCOLOR_Normal && strMessage.find(":") != std::string::npos)
					{
						pTextMessage->SetHighlightText(TRUE);
						pTextMessage->SetHighlightTextColor(D3DCOLOR_XRGB(100, 255, 50));
					}

					pTextMessage->SetFont("Noto Sans", 16, 0, FALSE, FALSE, -1);
					pTextMessage->SetColor(GetMessageColorFromType(eChatColor));

					if (pcItemData)
					{
						pTextMessage->SetColor(D3DCOLOR_ARGB(255, 240, 240, 0));
						pBaseWindow->SetHoverEvent(UI::Event::Build(std::bind(&Window::OnItemLinkHover, this, pcItemLinkWrapper, pBaseWindow, std::placeholders::_1)));

						if (eChatColor != EChatColor::CHATCOLOR_Normal)
						{
							pTextMessage->SetHighlightText(TRUE);
							pTextMessage->SetHighlightTextColor(GetMessageColorFromType(eChatColor));
						}
					}

					pTextMessage->SetDropdownShadow(TRUE);
					pTextMessage->SetDropdownColor(UI::Color(150, 0, 0, 0).Get());
					pTextMessage->SetMultiLine(TRUE);
					pTextMessage->SetMaxTextWidth(275);
					pTextMessage->SetText(strMessage);
					pTextMessage->SetNoClip(TRUE);
					pBaseWindow->SetBox(Rectangle2D(5, 0, 275, pTextMessage->GetHeightText()));

					//Emote
					{
						std::string strReplaceEmote = "";
						int iEmoteIndex = -1;
						if (IsEmoteMessage(strMessage, strReplaceEmote, iEmoteIndex, eChatColor))
						{
							std::string strNewMessage = strMessage;
							strNewMessage.erase(strNewMessage.find(strReplaceEmote), strReplaceEmote.length());

							pTextMessage->SetText(strNewMessage);
							pBaseWindow->SetBox(Rectangle2D(5, 0, 275, pTextMessage->GetHeightText()));

							UI::ImageBox_ptr pEmoteIcon = std::make_shared<UI::ImageBox>(Rectangle2D(pTextMessage->GetTextWidth(), 0, 103, 103));
							pEmoteIcon->SetImage(UI::ImageLoader::LoadImage_("game\\images\\emoticons\\emote%d.png", iEmoteIndex));
							pEmoteIcon->SetScale(0.16f);
							pBaseWindow->AddElement(pEmoteIcon);
						}
					}

					pBaseWindow->AddElement(pTextMessage);

					v->AddWindow(pBaseWindow);

					//Analyze
					if (eChatColor == EChatColor::CHATCOLOR_Blue)
					{
						AddRecentPlayer(strMessage);
					}

					if (v->GetCountItemsList() > MAX_CHATTEXTLIST)
					{
						//Get Window
						auto pTextWindow = v->GetWindowList()[0];

						//Old Position
						int iHeight = pTextWindow->GetHeight();

						//Clear and delete it
						CLEARPTR(pTextWindow);
						RESETPTR(pTextWindow);

						//Clear window in the vector
						v->GetWindowList().pop_front();

						for (int i = 0; i < MAX_LISTCHAT; i++)
						{
							if (saItemLink[i].iID == v->GetID())
							{
								if (saItemLink[i].vItemData.size() > 0)
								{
									sITEM* pc = saItemLink[i].vItemData[0];

									if ((cItemLinkDataWrapper.sItemInfo.ItemHeader.Head == pc->sItemInfo.ItemHeader.Head) && (cItemLinkDataWrapper.sItemInfo.ItemHeader.dwChkSum == pc->sItemInfo.ItemHeader.dwChkSum))
										cItemLinkDataWrapper.Flag = FALSE;

									SAFE_DELETE_C(saItemLink[i].vItemData[0]);
									saItemLink[i].vItemData.pop_front();
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	DWORD Window::GetMessageColorFromType(EChatColor eType)
	{
		switch (eType)
		{
		case EChatColor::CHATCOLOR_Error:
			return D3DCOLOR_ARGB(255, 255, 128, 96);
		case EChatColor::CHATCOLOR_Whisper:
			return D3DCOLOR_ARGB(255, 150, 190, 255);
		case EChatColor::CHATCOLOR_Blue:
			return D3DCOLOR_ARGB(255, 150, 190, 255);
		case EChatColor::CHATCOLOR_Global:
			return D3DCOLOR_ARGB(255, 230, 160, 255);
		case EChatColor::CHATCOLOR_Party:
			return D3DCOLOR_ARGB(255, 200, 255, 0);
		case EChatColor::CHATCOLOR_Clan:
			return D3DCOLOR_ARGB(255, 255, 255, 80);
		case EChatColor::CHATCOLOR_Trade:
			return D3DCOLOR_ARGB(255, 200, 255, 255);
		case EChatColor::CHATCOLOR_Notice:
			return D3DCOLOR_ARGB(255, 200, 128, 128);
		case EChatColor::CHATCOLOR_ClanLeader:
			return D3DCOLOR_ARGB(255, 255, 180, 70);
		case EChatColor::CHATCOLOR_Normal:
			return D3DCOLOR_ARGB(255, 255, 255, 128);
		case EChatColor::CHATCOLOR_Raid:
			return D3DCOLOR_ARGB(255, 40, 240, 230);
		}

		return D3DCOLOR_XRGB(255, 255, 255);
	}
	void Window::AddRecentPlayer(std::string strMessage)
	{
		std::string strName = "";

		if (strMessage.find("De> ") == 0)
			strName = strMessage.substr(STRLEN("De> "), strMessage.find(':') - STRLEN("De> "));
		else if (strMessage.find("Para> ") == 0)
			strName = strMessage.substr(STRLEN("Para> "), strMessage.find(':') - STRLEN("Para> "));
		else
		{
			if (strMessage.length() > 2)
			{
				size_t iFind = strMessage.find(' ');

				if ((strMessage[0] == '/') && ((strMessage[1] == ':') || (strMessage[1] == ';')))
					strName = strMessage.substr(2, iFind != std::string::npos ? (iFind - 2) : 0);
			}
		}

		if (strName.length() > 0)
		{
			InterfaceParty.latest_Insert((char*)strName.c_str());
		}
	}
	void Window::SendMessageChat(std::string strMessage)
	{
		if (strMessage.size() > 0)
		{
			BOOL bSendToLoginServer = FALSE;

			std::string strEmote = strMessage;
			BOOL bEmote = OnEmoteCommand(strEmote, lpCurPlayer);

			if (m_iTabID == CHATBOXTABID_All)
			{
				if (bEmote)
				{
					//AddMessage(FormatString("%s: %s", lpCurPlayer->smCharInfo.szName, strMessage.c_str()), EChatColor::CHATCOLOR_Normal);
					CHATGAMEHANDLE->SendChat(FALSE, strEmote.c_str());
					return;
				}
			}

			BOOL iRetChat = CHATGAMEHANDLE->AnalyzeChat((char*)strMessage.c_str());

			BOOL bContinue = TRUE;

			if (strMessage[0] == '/' || strMessage[0] == '@' || strMessage[0] == '#' || strMessage[0] == '$')
			{
				bContinue = FALSE;

				if (bEmote)
					bContinue = TRUE;

				if (strMessage[0] == '/' && (strMessage[1] == ';' || strMessage[1] == ':'))
				{
					strMessage = trim(strMessage);
					bSendToLoginServer = TRUE;
				}
				else if (strMessage.size() > 1 && strMessage[1] == '!')
					bSendToLoginServer = TRUE;
				else if ((strMessage.compare(0, 7, "/TRADE>") == 0 && strMessage.length() > 8) || (strMessage[0] == '$' && strMessage.length() > 2))
				{
					if (strMessage[0] == '$')
					{
						strMessage.erase(0, 1);
						strMessage.insert(0, "/TRADE>");
					}

					bSendToLoginServer = TRUE;
				}
			}

			if (bContinue)
			{
				switch (m_iTabID)
				{
				case CHATBOXTABID_All:
				case CHATBOXTABID_PM:
					if (iRetChat == 0)
						break;

					if (true)
					{
						int iLastMessageTick = (GetTickCount() - m_dwLastMessageTick);

						if (m_dwLastMessageTick && iLastMessageTick < 2000)
						{
							m_iTotalMessages++;

							if (m_iTotalMessages > 10)
							{
								if (iLastMessageTick > 0)
								{
									STRINGCOPY(lpCurPlayer->szChatMessage, FormatString("%s: %s", lpCurPlayer->smCharInfo.szName, "Eu falo demais, minha língua vai congelar -_-"));
									lpCurPlayer->dwChatMessageTimer = GetTickCount() + 1000 * 7;
									m_dwLastMessageTick = GetTickCount() + 1000 * 7;

									OnEmoteCommand(std::string("e6"), lpCurPlayer);
								}
								return;
							}
						}
						else if (iLastMessageTick > 2000)
							m_iTotalMessages = 0;

						m_dwLastMessageTick = GetTickCount();
					}

					strMessage = FormatString("%s: %s", lpCurPlayer->smCharInfo.szName, strMessage.c_str());
					STRINGCOPY(lpCurPlayer->szChatMessage, strMessage.c_str());
					lpCurPlayer->dwChatMessageTimer = dwPlayTime + 1000 * 7;
					AddMessage(strMessage, EChatColor::CHATCOLOR_Normal, lpCurPlayer->smCharInfo.szName);
					break;
				case CHATBOXTABID_Clan:
					strMessage = FormatString("/CLAN> %s", strMessage.c_str());
					break;
				case CHATBOXTABID_Party:
					strMessage = FormatString("@%s", strMessage.c_str());
					break;
				case CHATBOXTABID_Raid:
					strMessage = FormatString("#%s", strMessage.c_str());
					break;
				case CHATBOXTABID_Trade:

					Elapsed = GetTickCount();

					if (Start == 0)
					{
						Start = GetTickCount();

						strMessage = FormatString("/TRADE> %s", strMessage.c_str());
						bSendToLoginServer = TRUE;

						AddDelayGlobal();
					}
					else if ((Elapsed - Start) > 10000)
					{
						Start = GetTickCount();

						strMessage = FormatString("/TRADE> %s", strMessage.c_str());
						bSendToLoginServer = TRUE;

						AddDelayGlobal();
					}
					else
					{
						TitleBox::GetInstance()->SetText("Aguarde 10 segundos para enviar uma nova mensagem!", 3);
					}

					break;
				}
			}

			if (iRetChat)
			{
				CHATGAMEHANDLE->SendChat(bSendToLoginServer, strMessage.c_str());
			}
		}
	}
	BOOL Window::IsEmoteMessage(std::string strMessage, std::string& strReplace, int& iEmoteIndex, EChatColor eChatColor)
	{
		BOOL bRet = FALSE;

		iEmoteIndex = -1;

		if (strMessage.length() >= 2)
		{
			iEmoteIndex = -1;

			std::string strCommand = strMessage;

			if ((eChatColor == EChatColor::CHATCOLOR_Trade) || (eChatColor == EChatColor::CHATCOLOR_Global))
			{
				if (strMessage.find(">") != std::string::npos)
					strCommand = trim(strMessage.substr(strMessage.find(">") + 1));
			}
			else
			{
				if (strMessage.find(":") != std::string::npos)
					strCommand = trim(strMessage.substr(strMessage.find(":") + 1));
			}

			if (strCommand[0] == 'e')
			{
				strReplace = strCommand;
				iEmoteIndex = atoi(strCommand.substr(1).c_str());
			}
			else if (strCommand[0] == '/')
			{
				for (std::map<std::string, std::string>::iterator it = m_sFriendlyCommands.begin(); it != m_sFriendlyCommands.end(); it++)
				{
					if (STRINGCOMPAREI(it->first.c_str(), strCommand.c_str()))
					{
						strReplace = strCommand;
						iEmoteIndex = atoi(it->second.substr(1).c_str());
						break;
					}
				}
			}

			if ((iEmoteIndex > 0) && (iEmoteIndex <= MAX_EMOTELIST))
				bRet = TRUE;
		}
		return bRet;
	}
	BOOL Window::OnEmoteCommand(std::string& strMessage, smCHAR* pcUnit)
	{
		BOOL bRet = FALSE;
		int iEmoteIndex = 0;
		char Emote[16] = { 0 };

		for (std::map<std::string, std::string>::iterator it = m_sFriendlyCommands.begin(); it != m_sFriendlyCommands.end(); it++)
		{
			if (STRINGCOMPAREI(it->first.c_str(), strMessage.c_str()))
			{
				strMessage = it->second;
				break;
			}
		}

		size_t uPos = strMessage.find_first_of(":");
		if (uPos != std::string::npos)
			if (uPos + 1 <= strMessage.length())
				strMessage = trim(strMessage.substr(strMessage.find_first_of(":") + 1));

		if ((strMessage.length() <= 3) && (strMessage.length() > 0))
		{
			iEmoteIndex = atoi(strMessage.substr(1).c_str());

			if (strMessage[0] == 'e' && isdigit(strMessage[1]) && iEmoteIndex > 0 && iEmoteIndex <= MAX_EMOTELIST)
			{
				bRet = TRUE;
				sprintf_s(Emote, sizeof(Emote), "emote%d", iEmoteIndex);
			}
		}

		if (pcUnit && bRet)
		{
			POINT3D partPos;
			partPos.x = pcUnit->pX;
			partPos.y = pcUnit->pY + 3000;
			partPos.z = pcUnit->pZ;
			g_NewParticleMgr.Start(Emote, partPos, 0.5f);
		}

		return bRet;

	}

	void Window::HandlePacket(PacketItemLinkChat* psPacket)
	{
		sITEM sItem;
		DecodeCompress((BYTE*)&psPacket->trans, (BYTE*)&sItem);

		AddMessage(psPacket->szMessage, psPacket->eChatColor, "", &sItem);
	}

	void Window::HandlePacket(PacketChatBoxMessage* psPacket)
	{
		smCHAR* pcUnit = NULL;

		BOOL bEmote = FALSE;

		if (psPacket->lID)
			pcUnit = FindChrPlayer(psPacket->lID);

		if (pcUnit)
			bEmote = OnEmoteCommand(std::string(psPacket->szChatBoxMessage), pcUnit);

		if (((pcUnit && (pcUnit->smCharInfo.State == smCHAR_STATE_USER || pcUnit->smCharInfo.Brood == smCHAR_MONSTER_USER)) || psPacket->lID == 0))
			AddMessage(psPacket->szChatBoxMessage, psPacket->lID ? EChatColor::CHATCOLOR_Normal : psPacket->iChatColor);

		if (pcUnit && (psPacket->iChatColor != EChatColor::CHATCOLOR_Party))
		{
			if (bEmote == FALSE)
			{
				STRINGCOPY(pcUnit->szChatMessage, psPacket->szChatBoxMessage);
				pcUnit->dwChatMessageTimer = dwPlayTime + 1000 * 5;
			}
		}
	}
}