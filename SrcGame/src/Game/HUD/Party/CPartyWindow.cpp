#include "stdafx.h"
#include "CPartyWindow.h"
#include <Engine/UI/UIWindow.h>
#include <Engine/UI/UIImageLoader.h>
#include <Party/CPartyHandler.h>
#include "HUD\MessageBox.h"
#include <Engine/UI/UITooltip.h>
#include <Settings.h>
#include <GameCore.h>

char* GetCharacterClassStringByFlagItem(int flag)
{
	char* sClass = "";

	switch (flag)
	{
	case 1:
		sClass = "Lutador";
		break;
	case 2:
		sClass = "Mecânico";
		break;
	case 3:
		sClass = "Arqueira";
		break;
	case 4:
		sClass = "Pike";
		break;
	case 5:
		sClass = "Atalanta";
		break;
	case 6:
		sClass = "Cavaleiro";
		break;
	case 7:
		sClass = "Mago";
		break;
	case 8:
		sClass = "Sacerdotisa";
		break;
	case 9:
		sClass = "Assassina";
		break;
	case 10:
		sClass = "Xamã";
		break;
	default:
		break;
	}

	return sClass;
}

char* GetCharacterClassStringByFlag(int flag)
{
	char* sClass = "";

	switch (flag)
	{
	case 1:
		sClass = "Fighter";
		break;
	case 2:
		sClass = "Mechanician";
		break;
	case 3:
		sClass = "Archer";
		break;
	case 4:
		sClass = "Pikeman";
		break;
	case 5:
		sClass = "Atalanta";
		break;
	case 6:
		sClass = "Knight";
		break;
	case 7:
		sClass = "Magician";
		break;
	case 8:
		sClass = "Priestess";
		break;
	case 9:
		sClass = "Assassin";
		break;
	case 10:
		sClass = "Shaman";
		break;
	default:
		break;
	}

	return sClass;
}

CPartyWindow::CPartyWindow()
{
}

CPartyWindow::CPartyWindow(EWindow eID) : CBaseWindow(eID)
{
}

CPartyWindow::~CPartyWindow()
{
}

void CPartyWindow::Init()
{
	pWindowParty = std::make_shared<UI::Window>(Rectangle2D(0, 0, 290, 345));
	pWindowRaid = std::make_shared<UI::Window>(Rectangle2D(0, 0, 115, 185));

	pImagePartyMode[0] = UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\normalmode.png");
	pImagePartyMode[1] = UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\expmode.png");
	pImagePartyMode[2] = UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\huntmode.png");

	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\minilifebar.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\minilifebar_.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\background.png");

	for (int i = 0; i < 6; i++)
		UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\member%d.png", i + 1);

	for (int i = 0; i < 10; i++)
	{
		char* pszClass = GetCharacterClassStringByFlag(i + 1);

		UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\%s.png", pszClass);
		UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\%s_.png", pszClass);
	}

	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\lifebar.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\manabar.png");


	UI::ImageLoader::LoadImage_("game\\images\\UI\\menu\\topleft.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\menu\\middle.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\menu\\bottom.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\menu\\main.png");

	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\raidleader.png");
	UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\leader.png");


	BuildWindowParty();
	BuildWindowRaid();
}

void CPartyWindow::Render()
{
	pWindowRaid->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
	pWindowParty->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

void CPartyWindow::Update(float fTime)
{
	UpdatePartyData(&PARTYHANDLER->GetPartyData());

	pWindowParty->Update(fTime);
	pWindowRaid->Update(fTime);
}

void CPartyWindow::OnMouseMove(CMouse* pcMouse)
{
	pWindowParty->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
	pWindowRaid->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

BOOL CPartyWindow::OnMouseClick(CMouse* pcMouse)
{
	if (pWindowParty->OnMouseClick(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0))
		return TRUE;

	if (pWindowRaid->OnMouseClick(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0))
		return TRUE;

	return FALSE;
}

void CPartyWindow::ShowPartyNotification(char* pszCharacterName, int iType)
{
	iPartyRequestType = iType;

	MESSAGEBOX->SetBoxType(CMessageBox::BOXTYPEID_YesNo);
	MESSAGEBOX->SetTitle("Pedido de Grupo");
	MESSAGEBOX->SetDescription(FormatString("Você quer aceitar o Pedido de Grupo de %s ?", pszCharacterName));
	MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnAcceptPartyButtonClick, this));
	MESSAGEBOX->SetAutoClose(10);
	MESSAGEBOX->Show();
}

void CPartyWindow::ShowRaidNotification(char* pszCharacterName)
{
	MESSAGEBOX->SetBoxType(CMessageBox::BOXTYPEID_YesNo);
	MESSAGEBOX->SetTitle("Pedido de Raid");
	MESSAGEBOX->SetDescription(FormatString("Você quer aceitar o Pedido de Raid de %s ?", pszCharacterName));
	MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnAcceptRaidButtonClick, this));
	MESSAGEBOX->SetAutoClose(10);
	MESSAGEBOX->Show();
}

void CPartyWindow::UpdateParty(CPartyData* pcParty)
{
	bool bPartyIsRaidLeader = false;
	pWindowParty->Hide();

	auto pImageParty = pWindowParty->GetElement<UI::ImageBox>(IMAGEID_PartyType);
	auto pTooltipParty = pWindowParty->GetElement<UI::Tooltip>(TOOLTIPID_PartyType);
	if (pImageParty && pTooltipParty)
	{
		pImageParty->SetImage(pImagePartyMode[pcParty->eMode]);
		pTooltipParty->SetText(GetPartyMode(pcParty->eMode));
	}

	for (size_t i = 0; i < MAX_RAID - 1; i++)
	{
		auto pListPartyRaid = pWindowRaid->GetElement<UI::List>(LISTID_PartyRaid + i);
		if (pListPartyRaid)
		{
			pListPartyRaid->Clear();
			pListPartyRaid->Hide();
			pWindowRaid->Hide();

			if (pcParty->iRaidCount > 0)
			{
				bPartyIsRaidLeader = true;

				PartyRaid* pPartyRaid = &pcParty->saRaid[i];
				UI::Window_ptr p = std::make_shared<UI::Window>(Rectangle2D(0, 0, 115, 16));

				UI::Text_ptr pText = std::make_shared<UI::Text>(FormatString("Grupo #%d", (int)pPartyRaid->cRaidID), Rectangle2D(0, 0, 110, 20), TRUE, UI::Color(255, 0, 180, 255).Get());
				pText->SetFont("Noto Sans", 16, 0, FALSE, FALSE, UI::Color(255, 0, 180, 255).Get());
				pText->SetDropdownShadow(TRUE);
				pText->SetNoClip(TRUE);
				p->AddElement(pText);

				pListPartyRaid->AddWindow(p);

				if (pPartyRaid->cRaidID == 1)
					bPartyIsRaidLeader = false;

				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
				{
					PartyRaidMember* pMemberRaid = &pPartyRaid->saMembers[j];

					if (pMemberRaid->sData.iID)
					{
						UI::Window_ptr pWindowRaidMember = std::make_shared<UI::Window>(Rectangle2D(0, 0, 115, 20));
						pWindowRaidMember->SetID(WINDOWID_Member1 + j);

						//Character Name
						UI::Text_ptr pTextCharacterName = std::make_shared<UI::Text>(pPartyRaid->saMembers[j].szCharacterName, Rectangle2D(0, 0, 110, 15), TRUE, -1);
						pTextCharacterName->SetID(TEXTID_Name);
						pTextCharacterName->SetFont("Noto Sans", 14, 0, FALSE, FALSE, -1);
						pTextCharacterName->SetDropdownShadow(TRUE);
						pTextCharacterName->SetNoClip(TRUE);
						pWindowRaidMember->AddElement(pTextCharacterName);

						//Image of Life Bar
						UI::ImageBox_ptr pImageLifeBar = std::make_shared<UI::ImageBox>(Rectangle2D(0, 14, 89, 4));
						pImageLifeBar->SetID(IMAGEID_Background);
						pImageLifeBar->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\minilifebar.png"));
						pWindowRaidMember->AddElement(pImageLifeBar);

						UI::ImageBox_ptr pImageLifeBarActive = std::make_shared<UI::ImageBox>(Rectangle2D(0, 14, 89, 4));
						pImageLifeBarActive->SetID(IMAGEID_LifeBar);
						pImageLifeBarActive->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\minilifebar_.png"));
						pWindowRaidMember->AddElement(pImageLifeBarActive);

						pListPartyRaid->AddWindow(pWindowRaidMember);
					}
				}

				if (Settings::GetInstance()->UiBlock())
				{
					pWindowRaid->SetMoveBox(Rectangle2D(0, 0, 175, 15 + pListPartyRaid->GetListHeight()));
					pWindowRaid->SetCanMove(TRUE);
				}
				else
					pWindowRaid->SetCanMove(FALSE);

				pListPartyRaid->Show();
				pWindowRaid->Show();
			}
		}
	}

	auto pListPartyMembers = pWindowParty->GetElement<UI::List>(LISTID_PartyMembers);
	if (pListPartyMembers)
	{
		pListPartyMembers->Clear();

		UI::Group_ptr pGroupDropdown = std::make_shared<UI::Group>();
		pWindowParty->AddElement(pGroupDropdown);

		//Clear Dropdown
		for (int i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			auto p = pWindowParty->GetElement<UI::DropdownMenu>(DROPDOWNMENUID_Options + (i * 6));
			if (p)
				pWindowParty->DelElement(p);
		}

		//Add Party Members
		for (size_t i = 0; i < MAX_PARTY_MEMBERS; i++)
		{
			PartyMember sPartyMember = pcParty->saMembers[i];

			if (sPartyMember.sData.iID)
			{
				UI::Window_ptr p = std::make_shared<UI::Window>(Rectangle2D(0, 0, 290, 49));
				p->SetID(WINDOWID_Member1 + i);

				UI::ImageBox_ptr pImageBackground = std::make_shared<UI::ImageBox>(Rectangle2D(0, 0, 173, 49));
				pImageBackground->SetID(IMAGEID_Background);
				pImageBackground->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\background.png"));
				p->AddElement(pImageBackground);

				//Icon of Member Party
				UI::ImageBox_ptr pImageIconPartyMember = std::make_shared<UI::ImageBox>(Rectangle2D(34, 40, 9, 9));
				pImageIconPartyMember->SetID(IMAGEID_Icon);
				pImageIconPartyMember->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\member%d.png", i + 1));
				p->AddElement(pImageIconPartyMember);

				//Text Character Name
				UI::Text_ptr pTextCharacterName = std::make_shared<UI::Text>(sPartyMember.szCharacterName, Rectangle2D(70, 7, 220, 20), TRUE, -1);
				pTextCharacterName->SetID(TEXTID_Name);
				pTextCharacterName->SetFont("Noto Sans", 15, 0, FALSE, FALSE, -1);
				pTextCharacterName->SetDropdownShadow(TRUE);
				pTextCharacterName->SetNoClip(TRUE);
				p->AddElement(pTextCharacterName);

				//Text Character Level
				UI::Text_ptr pTextCharacterLevel = std::make_shared<UI::Text>("", Rectangle2D(43, 10, 20, 12), TRUE, D3DCOLOR_XRGB(255, 238, 100));
				pTextCharacterLevel->SetID(TEXTID_CharacterLevel);
				pTextCharacterLevel->SetFont("Arial", 12, 0, FALSE, FALSE, D3DCOLOR_XRGB(255, 238, 100));
				pTextCharacterLevel->SetDropdownShadow(TRUE);
				pTextCharacterLevel->SetDropdownColor(D3DCOLOR_XRGB(30, 30, 30));
				pTextCharacterLevel->SetNoClip(TRUE);
				pTextCharacterLevel->SetHorizontalAlign(ALIGN_Center);
				p->AddElement(pTextCharacterLevel);

				//Image of Character Class
				char* pszClass = GetCharacterClassStringByFlag(sPartyMember.cCharacterClass);

				UI::ImageBox_ptr pImageCharacterClass = std::make_shared<UI::ImageBox>(Rectangle2D(6, 8, 35, 35));
				pImageCharacterClass->SetID(IMAGEID_Class);
				pImageCharacterClass->SetActiveImage(
					UI::ImageLoader::LoadActiveImage((char*)(("game\\images\\UI\\party\\" + std::string(pszClass) + ".png")).c_str(),
						(char*)(("game\\images\\UI\\party\\" + std::string(pszClass) + "_.png")).c_str()
					)
				);

				p->AddElement(pImageCharacterClass);

				//Image of Life Bar
				UI::ImageBox_ptr pImageLifeBar = std::make_shared<UI::ImageBox>(Rectangle2D(46, 25, 119, 6));
				pImageLifeBar->SetID(IMAGEID_LifeBar);
				pImageLifeBar->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\lifebar.png"));
				p->AddElement(pImageLifeBar);

				//Image of Mana Bar
				UI::ImageBox_ptr pImageManaBar = std::make_shared<UI::ImageBox>(Rectangle2D(42, 37, 123, 3));
				pImageManaBar->SetID(IMAGEID_ManaBar);
				pImageManaBar->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\manabar.png"));
				p->AddElement(pImageManaBar);

				//Dropdown Menu
				UI::DropdownMenu_ptr pDropdownMenu = std::make_shared<UI::DropdownMenu>(Rectangle2D(175, 60 + pListPartyMembers->GetListHeight(), 94, 24));
				pDropdownMenu->SetID(DROPDOWNMENUID_Options + (i * 6));
				pDropdownMenu->SetGroup(pGroupDropdown);
				pDropdownMenu->SetDropdownPosition(UI::DROPDOWNPOSITION_Left);
				pDropdownMenu->SetMouseEvent(EMouseEvent::ClickDownR);
				pDropdownMenu->SetEvent(UI::Event::Build(std::bind(&CPartyWindow::OnSelectMenuParty, this, i)));
				pDropdownMenu->SetMenuBackground("game\\images\\UI\\menu\\topleft.png", "game\\images\\UI\\menu\\middle.png", "game\\images\\UI\\menu\\bottom.png", "game\\images\\UI\\menu\\main.png");

				//Indicate it's a Party Leader
				if (i == 0)
				{
					if (bPartyIsRaidLeader)
					{
						UI::ImageBox_ptr pImageIconRaidLeader = std::make_shared<UI::ImageBox>(Rectangle2D(5, -18, 37, 30));
						pImageIconRaidLeader->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\raidleader.png"));
						p->AddElement(pImageIconRaidLeader);
					}

					UI::ImageBox_ptr pImageIconLeader = std::make_shared<UI::ImageBox>(Rectangle2D(13, -10, 21, 14));
					pImageIconLeader->SetID(IMAGEID_IconLeader);
					pImageIconLeader->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\leader.png"));
					p->AddElement(pImageIconLeader);
				}

				//Party Leader
				if (pcParty->saMembers[0].sData.iID == UNITDATA->dwObjectSerial)
				{
					//Index is not first (so it's someone else instead of the leader)
					if (i != 0)
					{
						//Options
						pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Delegar", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 1);
						pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Retirar", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 2);
					}

					//Index is number 0.
					else
					{
						//Options

						if (pcParty->iRaidCount > 0)
						{
							pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Desfazer Raid", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 7);
						}
						//else
						//{
							pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Desfazer GP.", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 3);
						//}

						pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Sair", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 4);
						pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Mudar Modo", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 5);
					}
				}
				else
				{
					if (sPartyMember.sData.iID == UNITDATA->dwObjectSerial)
					{
						//Options
						pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Sair", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 6);
					}
				}

				pWindowParty->AddElement(pDropdownMenu);

				//Button to open Dropdown Menu
				UI::Button_ptr pButtonDropdownMenu = std::make_shared<UI::Button>(Rectangle2D(0, 0, 173, 49));
				pButtonDropdownMenu->SetMouseEvent(EMouseEvent::ClickDownR);
				pButtonDropdownMenu->SetDropdownMenu(pDropdownMenu);
				p->AddElement(pButtonDropdownMenu);

				pListPartyMembers->AddWindow(p);
			}
		}

		if (Settings::GetInstance()->UiBlock())
		{
			pWindowParty->SetMoveBox(Rectangle2D(0, 0, 175, 50 + pListPartyMembers->GetListHeight()));
			pWindowParty->SetCanMove(Settings::GetInstance()->UiBlock());
		}
		else
			pWindowParty->SetCanMove(FALSE);

		pWindowParty->Show();
	}
}

void CPartyWindow::UpdatePartyData(CPartyData* pcParty)
{
	if (pcParty->iRaidCount > 0)
	{
		for (size_t i = 0; i < MAX_RAID - 1; i++)
		{
			auto pListPartyRaid = pWindowRaid->GetElement<UI::List>(LISTID_PartyRaid + i);
			if (pListPartyRaid)
			{
				for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
				{
					PartyRaidMember psMember = pcParty->saRaid[i].saMembers[j];
					auto pWindowMember = pListPartyRaid->GetWindow(WINDOWID_Member1 + j);
					if (pWindowMember)
					{
						BOOL bDead = psMember.sData.life[0] <= 0 ? TRUE : FALSE;
						BOOL bFar = !psMember.sData.sPosition.WithinXZDistance(&UNITDATA->sPosition, DISTANCE_MAX_PARTY);

						auto pBackgroundImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_Background);
						if (pBackgroundImage)
							pBackgroundImage->SetColor(bFar ? D3DCOLOR_ARGB(120, 255, 255, 255) : -1);

						auto pLifeBarImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_LifeBar);
						if (pLifeBarImage)
						{
							if (psMember.sData.life[0] > 0)
								pLifeBarImage->SetWidth((psMember.sData.life[0] * 89) / psMember.sData.life[1]);

							pLifeBarImage->SetColor(bFar ? D3DCOLOR_ARGB(120, 255, 255, 255) : -1);
						}

						auto pTextCharacterName = pWindowMember->GetElement<UI::Text>(TEXTID_Name);
						if (pTextCharacterName)
						{
							if (bDead)
								pTextCharacterName->SetColor(D3DCOLOR_XRGB(255, 45, 45));
							else if (bFar)
								pTextCharacterName->SetColor(D3DCOLOR_XRGB(185, 175, 155));
							else
								pTextCharacterName->SetColor(-1);
						}
					}
				}

			}
		}
	}

	auto pListPartyMembers = pWindowParty->GetElement<UI::List>(LISTID_PartyMembers);
	if (pListPartyMembers)
	{
		for (int i = 0; i < pcParty->iMembersCount; i++)
		{
			PartyMember psMember = pcParty->saMembers[i];

			auto pWindowMember = pListPartyMembers->GetWindow(WINDOWID_Member1 + i);
			if (pWindowMember)
			{
				BOOL bDead = psMember.sData.life[0] <= 0 ? TRUE : FALSE;
				BOOL bFar = !psMember.sData.sPosition.WithinXZDistance(&UNITDATA->sPosition, DISTANCE_MAX_PARTY);

				auto pBackgroundImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_Background);
				if (pBackgroundImage)
					pBackgroundImage->SetColor(bFar ? D3DCOLOR_ARGB(150, 255, 255, 255) : -1);

				auto pIconImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_Icon);
				if (pIconImage)
					pIconImage->SetColor(bFar ? D3DCOLOR_ARGB(150, 255, 255, 255) : -1);

				auto pLifeBarImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_LifeBar);
				if (pLifeBarImage)
				{
					if (psMember.sData.life[0] > 0)
						pLifeBarImage->SetWidth((psMember.sData.life[0] * 119) / psMember.sData.life[1]);

					pLifeBarImage->SetColor(bFar ? D3DCOLOR_ARGB(150, 255, 255, 255) : -1);
				}

				auto pManaBarImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_ManaBar);
				if (pManaBarImage)
				{
					if (psMember.sData.mana[0] > 0)
						pManaBarImage->SetWidth((psMember.sData.mana[0] * 123) / psMember.sData.mana[1]);

					pManaBarImage->SetColor(bFar ? D3DCOLOR_ARGB(150, 255, 255, 255) : -1);
				}

				auto pTextCharacterLevel = pWindowMember->GetElement<UI::Text>(TEXTID_CharacterLevel);
				if (pTextCharacterLevel)
				{
					int iLevel = psMember.sData.sLevel;
					if (psMember.sData.iID == UNITDATA->dwObjectSerial)
						iLevel = UNITDATA->smCharInfo.Level;

					pTextCharacterLevel->SetText(FormatString("%d", iLevel));
				}

				auto pCharacterClassImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_Class);
				if (pCharacterClassImage)
				{
					pCharacterClassImage->Swap(bDead ? TRUE : FALSE);
					pCharacterClassImage->SetColor(bFar ? D3DCOLOR_ARGB(150, 255, 255, 255) : -1);
				}

				auto pLeaderIconImage = pWindowMember->GetElement<UI::ImageBox>(IMAGEID_IconLeader);
				if (pLeaderIconImage)
					pLeaderIconImage->SetColor(bFar ? D3DCOLOR_ARGB(150, 255, 255, 255) : -1);

				auto pTextCharacterName = pWindowMember->GetElement<UI::Text>(TEXTID_Name);
				if (pTextCharacterName)
				{
					if (bDead)
						pTextCharacterName->SetColor(D3DCOLOR_XRGB(255, 45, 45));
					else if (bFar)
						pTextCharacterName->SetColor(D3DCOLOR_XRGB(185, 175, 155));
					else
						pTextCharacterName->SetColor(-1);
				}
			}
		}
	}
}

void CPartyWindow::LeaveParty()
{
	auto pListPartyMembers = pWindowParty->GetElement<UI::List>(LISTID_PartyMembers);
	if (pListPartyMembers)
	{
		pListPartyMembers->Clear();
		pWindowParty->Hide();
		pWindowRaid->Hide();
	}
}

void CPartyWindow::ToggleWindow()
{
	auto pListPartyMembers = pWindowParty->GetElement<UI::List>(LISTID_PartyMembers);
	if (pListPartyMembers)
	{
		if (pListPartyMembers->IsOpen())
		{
			pListPartyMembers->Hide();
			pWindowParty->SetHeight(15);
			pWindowParty->SetCanMove(Settings::GetInstance()->UiBlock());
			pWindowRaid->Hide();
		}
		else
		{
			pListPartyMembers->Show();
			pWindowParty->SetHeight(50 + pListPartyMembers->GetListHeight());
			pWindowParty->SetCanMove(Settings::GetInstance()->UiBlock());
			pWindowParty->SetMoveBox(Rectangle2D(0, 0, pWindowParty->GetWidth(), pWindowParty->GetHeight()));
			pWindowRaid->Show();
		}
	}
}

void CPartyWindow::BuildWindowParty()
{
	pWindowParty->SetPosition(0, (RESOLUTION_HEIGHT >> 1) - (345 >> 1));

	UI::Button_ptr pToggleParty = std::make_shared<UI::Button>(Rectangle2D(14, 0, 18, 14));
	pToggleParty->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\party\\expand.png", "game\\images\\UI\\party\\expand_.png"));
	pToggleParty->SetEvent(UI::Event::Build(std::bind(&CPartyWindow::ToggleWindow, this)));
	pWindowParty->AddElement(pToggleParty);

	UI::ImageBox_ptr pImageParty = std::make_shared<UI::ImageBox>(Rectangle2D(36, 0, 15, 15));
	pImageParty->SetID(IMAGEID_PartyType);
	pImageParty->SetImage(pImagePartyMode[0]);
	pImageParty->SetColor(UI::Color(220, 255, 255, 255).Get());
	pWindowParty->AddElement(pImageParty);

	UI::List_ptr pListPartyMembers = std::make_shared<UI::List>(Rectangle2D(0, 40, 290, 345));
	pListPartyMembers->SetNoClip(TRUE);
	pListPartyMembers->SetID(LISTID_PartyMembers);
	pWindowParty->AddElement(pListPartyMembers);

	UI::Tooltip_ptr pTooltipHideParty = std::make_shared<UI::Tooltip>(Rectangle2D());
	pTooltipHideParty->Init();
	pTooltipHideParty->SetFont("Arial", 16, -1);
	pTooltipHideParty->SetText("Esconder/Mostrar Grupo");
	pTooltipHideParty->SetLinkObject(pToggleParty);
	pTooltipHideParty->SetPosition(TOOLTIPPOSITION_Top);
	pTooltipHideParty->SetFollowMouse(FALSE);
	pWindowParty->AddElement(pTooltipHideParty);

	//Tooltip of Party Mode
	UI::Tooltip_ptr pTooltipPartyType = std::make_shared<UI::Tooltip>(Rectangle2D());
	pTooltipPartyType->Init();
	pTooltipPartyType->SetID(TOOLTIPID_PartyType);
	pTooltipPartyType->SetLinkObject(pImageParty);
	pTooltipPartyType->SetFont("Arial", 16, -1);
	pTooltipPartyType->SetPosition(TOOLTIPPOSITION_Top);
	pTooltipPartyType->SetFollowMouse(FALSE);
	pWindowParty->AddElement(pTooltipPartyType);

	pWindowParty->Hide();
}

void CPartyWindow::BuildWindowRaid()
{
	pWindowRaid->SetPosition(180, (RESOLUTION_HEIGHT >> 1) - (345 >> 1));

	for (size_t i = 0; i < MAX_RAID - 1; i++)
	{
		UI::List_ptr pListPartyRaid = std::make_shared<UI::List>(Rectangle2D(0, 0, 115, 180));
		pListPartyRaid->SetID(LISTID_PartyRaid + i);
		pWindowRaid->AddElement(pListPartyRaid);
	}

	pWindowRaid->Hide();
}

void CPartyWindow::ShowMessageBox(int iType, int iIndex)
{
	MESSAGEBOX->SetBoxType(CMessageBox::BOXTYPEID_YesNo);
	MESSAGEBOX->SetTitle("Tem certeza disso?");

	if (iType == 1)
	{
		MESSAGEBOX->SetDescription("Você quer desfazer o Grupo?");
		MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnDisbandPartyButtonClick, this));
	}
	else if (iType == 2)
	{
		MESSAGEBOX->SetDescription(FormatString("Você quer expulsar %s?", PARTYHANDLER->GetPartyData().saMembers[iIndex].szCharacterName));
		MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnKickPartyButtonClick, this, iIndex));
	}
	else if (iType == 3)
	{
		MESSAGEBOX->SetDescription("Você quer sair do Grupo?");
		MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnLeavePartyButtonClick, this));
	}
	else if (iType == 4)
	{
		MESSAGEBOX->SetDescription(FormatString("Você quer delegar %s como o líder?", PARTYHANDLER->GetPartyData().saMembers[iIndex].szCharacterName));
		MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnDelegatePartyButtonClick, this, iIndex));
	}
	else if (iType == 5)
	{
		MESSAGEBOX->SetDescription(FormatString("Você mudar para o %s?", GetPartyMode((EPartyMode)((PARTYHANDLER->GetPartyData().eMode + 1) % 3))));
		MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnChangePartyModeButtonClick, this));
	}
	else if (iType == 6)
	{
		MESSAGEBOX->SetDescription("Você quer desfazer o Raid?");
		MESSAGEBOX->SetEvent(std::bind(&CPartyWindow::OnDisbandRaidButtonClick, this));
	}

	MESSAGEBOX->Show();
}

void CPartyWindow::OnSelectMenuParty(int iIndex)
{
	auto pListPartyMembers = pWindowParty->GetElement<UI::List>(LISTID_PartyMembers);
	if (pListPartyMembers)
	{
		auto pWindowMember = pListPartyMembers->GetWindow(WINDOWID_Member1 + iIndex);
		if (pWindowMember)
		{
			UI::DropdownMenu_ptr pDropdownMenu = pWindowParty->GetElement<UI::DropdownMenu>(DROPDOWNMENUID_Options + (iIndex * 6));

			if (pDropdownMenu)
			{
				int iOptionID = pDropdownMenu->GetSelectedOptionID();
				if (iOptionID == 1)
					ShowMessageBox(4, iIndex);
				else if (iOptionID == 2)
					ShowMessageBox(2, iIndex);
				else if (iOptionID == 3)
					ShowMessageBox(1, iIndex);
				else if (iOptionID == 4)
					ShowMessageBox(3, iIndex);
				else if (iOptionID == 5)
					ShowMessageBox(5, iIndex);
				else if (iOptionID == 6)
					ShowMessageBox(3, iIndex);
				else if (iOptionID == 7)
					ShowMessageBox(6, iIndex);
			}
		}
	}
}

void CPartyWindow::OnAcceptPartyButtonClick()
{
	if (MESSAGEBOX->GetType())
	{
		if (iPartyRequestType > 0)
		{
			if (iPartyRequestType == 1)
				PARTYHANDLER->JoinPartyRequest();
			else if (iPartyRequestType == 2)
				PARTYHANDLER->AcceptPartyRequest();
		}
	}
}

void CPartyWindow::OnAcceptRaidButtonClick()
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->JoinRaidRequest();
}

void CPartyWindow::OnDelegatePartyButtonClick(int iIndex)
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->ProcessPacket(&PacketActionParty(PARTYACTION_Delegate, PARTYHANDLER->GetPartyData().saMembers[iIndex].sData.iID));
}

void CPartyWindow::OnKickPartyButtonClick(int iIndex)
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->ProcessPacket(&PacketActionParty(PARTYACTION_Kick, PARTYHANDLER->GetPartyData().saMembers[iIndex].sData.iID));
}

void CPartyWindow::OnLeavePartyButtonClick()
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->ProcessPacket(&PacketActionParty(PARTYACTION_Leave));
}

void CPartyWindow::OnDisbandRaidButtonClick()
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->ProcessPacket(&PacketActionParty(PARTYACTION_DisbandRaid));
}

void CPartyWindow::OnDisbandPartyButtonClick()
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->ProcessPacket(&PacketActionParty(PARTYACTION_DisbandParty));
}

void CPartyWindow::OnChangePartyModeButtonClick()
{
	if (MESSAGEBOX->GetType())
		PARTYHANDLER->ProcessPacket(&PacketActionParty(PARTYACTION_ChangeMode));
}
