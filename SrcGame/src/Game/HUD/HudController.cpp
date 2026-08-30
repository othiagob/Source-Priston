#include <Utils/_common.h>
#include <Engine/UI/UITooltip.h>
#include "sinbaram/sinLinkHeader.h"
#include "TJBOY/clanmenu/cE_CViewClanInfo.h"
#include "HudController.h"
#include "field.h"
#include "Engine/Directx/DXGraphicEngine.h"
#include <TJBOY/clanmenu/tjclan.h>

extern bool MouseAction(int x, int y, int w, int h);
extern int DrawClanMark_EachBox(LPDIRECT3DTEXTURE9 clanMark, int x, int y);
extern int GetDistanceDbl(int x1, int z1, int x2, int z2);
extern void g_IsCheckClanMember(smCHAR* pChar);

extern POINT3D EachCameraPos;

HUDController::HUDController(EWindow eID) : CBaseWindow(eID)
{
	bRenderTarget = FALSE;
}

HUDController::HUDController() : CBaseWindow()
{

}

HUDController::~HUDController()
{

}

void HUDController::Shutdown()
{
	if (GAMECOREHANDLE->IsInit())
	{
		for (int i = 0; i < 6; i++)
		{
			CLEARPTR(pcaImageTargetType[i]);
			UI::ImageLoader::DeleteImage(pcaImageTargetType[i]);
		}
	}
}

void HUDController::OnSelectTargetMenu()
{
	auto pMenu = pWindowTarget->GetElement<UI::DropdownMenu>(DROPDOWNMENU_More);

	extern smCHAR* chrEachMaster;

	if (pMenu && chrEachMaster)
	{
		int iMenuID = pMenu->GetSelectedOptionID();

		if (iMenuID == 1)
		{
			if (GetTradeDistanceFromCode(chrEachMaster->dwObjectSerial) == TRUE)
			{
				SendRequestTrade(chrEachMaster->dwObjectSerial, 0);

				CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "Pedido de troca enviado para %s", chrEachMaster->smCharInfo.szName);
			}
			else
			{
				CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Error, "%s está longe demais para enviar um pedido de troca", chrEachMaster->smCharInfo.szName);
			}
		}
		else if (iMenuID == 2)
		{
			InterfaceParty.latest_Insert(chrEachMaster->smCharInfo.szName);
			InterfaceParty.friend_Insert(chrEachMaster->smCharInfo.szName);

			InterfaceParty.Main_menuSet(2);
			InterfaceParty.chat_changeMENU(1);

			if (InterfaceParty.PartyPosState == PARTY_NONE)
				ShowParty();
		}
		else if (iMenuID == 3)
		{
			CHATGAMEHANDLE->SendChat(TRUE, FormatString("//PARTY %s", chrEachMaster->smCharInfo.szName));
		}
		else if (iMenuID == 4)
		{
			PARTYHANDLER->ProcessPacket(&PacketRequestRaid(chrEachMaster->dwObjectSerial));
		}
		else if (iMenuID == 5)
		{
			g_IsCheckClanMember(chrEachMaster);
		}
	}
}

BOOL HUDController::UpdateTargetInfo(smCHAR* pcUnitData)
{
	if (!pcUnitData)
		return FALSE;

	smCHAR* pcUnit = FindAutoPlayer(pcUnitData->dwObjectSerial);

	if (!pcUnit)
		return FALSE;

	auto pImageLifeBar = pWindowTarget->GetElement<UI::ImageBox>(IMAGEID_LifeBar);
	if (pImageLifeBar)
		pImageLifeBar->SetWidth((pcUnitData->smCharInfo.Life[0] * 208) / (pcUnitData->smCharInfo.Life[0] == 0 ? 1 : pcUnitData->smCharInfo.Life[1]));

	// Update Range Text
	auto pTextRange = pWindowTarget->GetElement<UI::Text>(TEXTID_Range);

	if (pTextRange)
	{
		int iDistance = 0;

		iDistance = GetDistanceDbl(UNITDATA->Posi.x, UNITDATA->Posi.z, pcUnitData->Posi.x, pcUnitData->Posi.z);
		pTextRange->SetTextEx("(%dm)", (int)sqrt(iDistance) / 27);
	}

	auto pTextLife = pWindowTarget->GetElement<UI::Text>(TEXTID_Life);
	if (pTextLife)
	{
		pTextLife->SetTextEx("%d/%d", pcUnit->smCharInfo.Life[0], pcUnit->smCharInfo.Life[1]);

		//Percent
		auto pTextLifePercent = pWindowTarget->GetElement<UI::Text>(TEXTID_LifePercent);
		if (pTextLifePercent)
		{
			if (pcUnit->smCharInfo.Life[0] > 0)
			{
				int iPercent = (pcUnit->smCharInfo.Life[0] * 100) / pcUnit->smCharInfo.Life[1];
				pTextLifePercent->SetTextEx("%d%%", iPercent);
			}
		}

		if (pcUnit->smCharInfo.Life[0] <= 0 || pcUnitData->smCharInfo.Life[0] <= 0)
		{
			pcLastUnitDataTarget = NULL;
			iTargetID = 0;
			return FALSE;
		}
	}

	BOOL bSameTarget = FALSE;

	if ((((pcUnitData->smCharInfo.State == smCHAR_STATE_USER || pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_USER) || pcUnitData->smCharInfo.State == smCHAR_STATE_NPC) && iTargetID == pcUnitData->dwObjectSerial) || (pcUnitData->smCharInfo.Brood != smCHAR_MONSTER_USER && pcUnitData->smCharInfo.State == smCHAR_STATE_ENEMY && iTargetID == pcUnitData->smCharInfo.dwObjectSerial))
		bSameTarget = TRUE;

	if (bSameTarget == FALSE)
	{
		bLoadClan16 = FALSE;
		bLoadClan32 = FALSE;
	}

	auto pWindowTargetInfo = pWindowTarget->GetElement<UI::Window>(WINDOWID_TargetInfo);

	if (pcUnitData->smCharInfo.ClassClan && pWindowTargetInfo && (bLoadClan16 == FALSE))
	{
		pWindowTargetInfo->Show();

		//Clan Name
		auto pTextTargetInfo = pWindowTargetInfo->GetElement<UI::Text>(TEXTID_TargetInfo);
		if (pTextTargetInfo)
			pTextTargetInfo->SetText(ClanInfo[pcUnitData->ClanInfoNum].ClanInfoHeader.ClanName);

		//Clan Icon
		auto pImageClanIcon = pWindowTargetInfo->GetElement<UI::ImageBox>(IMAGEID_ClanIcon);
		if (pImageClanIcon)
		{
			if (ClanInfo[pcUnitData->ClanInfoNum].hClanMark)
			{
				const char* clanImg = FormatString("%s\\%s\\10_%s_16.cl", CLANINFO_DIR, szConnServerName, ClanInfo[pcUnitData->ClanInfoNum].ClanInfoHeader.UniqueClaClanInfo_nNum);

				if (FILEEXIST(clanImg))
				{
					if (pImageClanIcon->GetImage())
						UI::ImageLoader::DeleteImage(pImageClanIcon->GetImage());

					pImageClanIcon->SetImage(UI::ImageLoader::LoadImage_(clanImg));

					bLoadClan16 = TRUE;
				}
			}
		}
	}
	else if (pWindowTargetInfo && !pcUnitData->smCharInfo.ClassClan)
		pWindowTargetInfo->Hide();


	auto pCharacterInfoWindow = pWindowTarget->GetElement<UI::Window>(WINDOWID_CharacterInfo);

	if (pcUnitData->smCharInfo.ClassClan && pCharacterInfoWindow && (bLoadClan32 == FALSE))
	{
		auto pImageClanIcon = pCharacterInfoWindow->GetElement<UI::ImageBox>(IMAGEID_ClanIcon);

		if (pImageClanIcon)
		{
			if (ClanInfo[pcUnitData->ClanInfoNum].hClanMark32)
			{
				const char* clanImg = FormatString("%s\\%s\\10_%s.cl", CLANINFO_DIR, szConnServerName, ClanInfo[pcUnitData->ClanInfoNum].ClanInfoHeader.UniqueClaClanInfo_nNum);

				if (FILEEXIST(clanImg))
				{
					if (pImageClanIcon->GetImage())
						UI::ImageLoader::DeleteImage(pImageClanIcon->GetImage());

					pImageClanIcon->SetImage(UI::ImageLoader::LoadImage_(clanImg));

					bLoadClan32 = TRUE;
				}
			}
		}

		auto pTextClanInfo = pCharacterInfoWindow->GetElement<UI::Text>(TEXTID_ClanInfo);
		if (pTextClanInfo)
			pTextClanInfo->SetText(trim(std::string(ClanInfo[pcUnitData->ClanInfoNum].ClanInfoHeader.ClanReport)));
	}

	if (bSameTarget)
		return FALSE;

	auto pTextName = pWindowTarget->GetElement<UI::Text>(TEXTID_Name);
	if (pTextName)
		pTextName->SetText(pcUnitData->smCharInfo.szName);

	//Update Target Level
	auto pTextLevel = pWindowTarget->GetElement<UI::Text>(TEXTID_Level);
	if (pTextLevel)
	{
		std::string strLevel = std::to_string(pcUnitData->smCharInfo.Level);
		bool bLevelType = strLevel.size() > 1 ? true : false;

		if (pcUnitData->smCharInfo.State == smCHAR_STATE_USER || pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_USER)
			pTextLevel->SetTextEx("%s%c", bLevelType ? strLevel.substr(0, strLevel.size() - 1).c_str() : strLevel.c_str(), bLevelType ? 'x' : NULL);
		else
			pTextLevel->SetTextEx("%s", strLevel.c_str());
	}

	//Show button more if it's a Player
	auto pButtonMore = pWindowTarget->GetElement<UI::Button>(BUTTONID_More);
	if (pButtonMore)
	{
		if (pcUnitData->smCharInfo.State == smCHAR_STATE_USER || pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_USER)
			pButtonMore->Show();
		else
			pButtonMore->Hide();
	}


	auto pDropdownMenu = pWindowTarget->GetElement<UI::DropdownMenu>(DROPDOWNMENU_More);
	if (pDropdownMenu)
	{
		if (pcUnitData->dwObjectSerial != iTargetID)
			pDropdownMenu->Close();

		pDropdownMenu->Clear();

		//Enemy 
		if (pcUnitData->smCharInfo.State != smCHAR_STATE_ENEMY)
			pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Pedir Troca", Rectangle2D(11, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 1);

		//Others Options
		pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Add Amigo", Rectangle2D(11, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 2);
		pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Pedir Grupo", Rectangle2D(11, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 3);

		//Raid
		if (pcUnit && pcUnit->bPartyLeader)
		{
			if (UNITDATA->bPartyLeader && pcUnit->bPartyLeader)
				pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Raid", Rectangle2D(11, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 4);
		}

		//It's a Clan Leader? So show the option to invite for clan
		if ((cldata.myPosition == 101 && !pcUnitData->dwClanCode) || (cldata.myPosition == 104 && !pcUnitData->dwClanCode))
			pDropdownMenu->AddOption(pDropdownMenu->MakeOption("Clan", Rectangle2D(13, 5, 77, 18), FALSE, -1, D3DCOLOR_XRGB(230, 180, 150)), FALSE, 5);
	}

	auto pTooltipType = pWindowTarget->GetElement<UI::Tooltip>(TOOLTIP_TargetType);
	auto pImageType = pWindowTarget->GetElement<UI::ImageBox>(IMAGEID_Type);
	if (pImageType && pTooltipType)
	{
		if (pcUnitData->smCharInfo.State == smCHAR_STATE_ENEMY)
		{
			if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_DEMON)
			{
				pImageType->SetImage(pcaImageTargetType[TARGETTYPE_Demon]);
				pTooltipType->SetText("Demônio");
			}
			else if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_MECHANIC)
			{
				pImageType->SetImage(pcaImageTargetType[TARGETTYPE_Mechanic]);
				pTooltipType->SetText("Mecânico");
			}
			else if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_MUTANT)
			{
				pImageType->SetImage(pcaImageTargetType[TARGETTYPE_Mutant]);
				pTooltipType->SetText("Mutante");
			}
			else if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD)
			{
				pImageType->SetImage(pcaImageTargetType[TARGETTYPE_Undead]);
				pTooltipType->SetText("Morto-Vivo");
			}
			else if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_NORMAL)
			{
				pImageType->SetImage(pcaImageTargetType[TARGETTYPE_Normal]);
				pTooltipType->SetText("Normal");
			}
		}
		else if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_GOOD)
		{
			pImageType->SetImage(pcaImageTargetType[TARGETTYPE_NPC]);
			pTooltipType->SetText("Amigo");
		}
		else if (pcUnitData->smCharInfo.State == smCHAR_STATE_NPC)
		{
			pImageType->SetImage(pcaImageTargetType[TARGETTYPE_NPC]);
			pTooltipType->SetText("NPC");
		}
		else if (pcUnitData->smCharInfo.State == smCHAR_STATE_USER || pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_USER)
		{
			pImageType->SetImage(pcaImageTargetType[TARGETTYPE_Player]);
			pTooltipType->SetText("Jogador");
		}
	}

	iTargetID = pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_USER ? pcUnitData->smCharInfo.dwObjectSerial : pcUnitData->dwObjectSerial;
	
	//pcLastUnitDataTarget = pcUnitData;

	return TRUE;
}

void HUDController::SetRenderTarget(BOOL b)
{
	bRenderTarget = b;

	if (b == FALSE)
		pcLastUnitDataTarget = NULL;
}

BOOL HUDController::OnMouseClick(class CMouse* pcMouse)
{
	if (pWindowTarget->OnMouseClick(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0))
		return TRUE;
}

void HUDController::OnMouseMove(class CMouse* pcMouse)
{
	pWindowTarget->OnMouseMove(pcMouse, 0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

BOOL HUDController::OnKeyPress(class CKeyboard* pcKeyboard)
{
	return FALSE;
}

BOOL HUDController::OnMouseScroll(CMouse* pcMouse)
{
	return FALSE;
}

BOOL HUDController::OnKeyChar(CKeyboard* pcKeyboard)
{
	return FALSE;
}

void HUDController::RenderFriendStatus()
{
}

void HUDController::Render()
{
	pWindowTarget->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

void HUDController::Init()
{
	iFontID = FONTHANDLER->CreateFont("Arial", 20, 0, FALSE, TRUE, -1);

	BuildTargetWindow();

	UpdateObjectsSettings();
}

void HUDController::BuildTargetWindow()
{
	//Load Images from Target Type
	pcaImageTargetType[TARGETTYPE_Demon] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\demon.png");
	pcaImageTargetType[TARGETTYPE_Mechanic] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\mechanic.png");
	pcaImageTargetType[TARGETTYPE_Mutant] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\mutant.png");
	pcaImageTargetType[TARGETTYPE_Normal] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\normal.png");
	pcaImageTargetType[TARGETTYPE_Undead] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\undead.png");
	pcaImageTargetType[TARGETTYPE_NPC] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\npc.png");
	pcaImageTargetType[TARGETTYPE_Player] = UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\type\\player.png");

	pWindowTarget = std::make_shared<UI::Window>(Rectangle2D(0, 0, 295, 218));
	pWindowTarget->SetPosition((RESOLUTION_WIDTH >> 1) - 150, 0);
	pWindowTarget->SetMoveBox(Rectangle2D(0, 0, pWindowTarget->GetWidth() - 15, pWindowTarget->GetHeight() - 140));
	pWindowTarget->SetCanMove(Settings::GetInstance()->UiBlock());

	//Dropdown Menu
	UI::DropdownMenu_ptr pDropdownMenu = std::make_shared<UI::DropdownMenu>(Rectangle2D(301, 16, 94, 24));
	pDropdownMenu->SetID(DROPDOWNMENU_More);
	pDropdownMenu->SetDropdownPosition(UI::DROPDOWNPOSITION_Left);
	pDropdownMenu->SetEvent(UI::Event::Build(std::bind(&HUDController::OnSelectTargetMenu, this)));
	pDropdownMenu->SetMenuBackground("game\\images\\UI\\menu\\topleft.png", "game\\images\\UI\\menu\\middle.png", "game\\images\\UI\\menu\\bottom.png", "game\\images\\UI\\menu\\main.png");
	pWindowTarget->AddElement(pDropdownMenu);

	//Background Target Info
	UI::Window_ptr pWindowTargetInfo = std::make_shared<UI::Window>(Rectangle2D(41, 52, 229, 22));
	pWindowTargetInfo->SetID(WINDOWID_TargetInfo);
	{
		UI::ImageBox_ptr pImageBackgroundTargetInfo = std::make_shared<UI::ImageBox>(Rectangle2D(0, 0, 229, 22));
		pImageBackgroundTargetInfo->SetID(IMAGEID_TargetInfo);
		pImageBackgroundTargetInfo->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\targetinfo.png"));
		pWindowTargetInfo->AddElement(pImageBackgroundTargetInfo);

		UI::Text_ptr pTextTargetInfo = std::make_shared<UI::Text>(Rectangle2D(65, 4, 150, 18));
		pTextTargetInfo->SetFont("Arial", 15, 0, FALSE, FALSE, D3DCOLOR_XRGB(180, 180, 125));
		pTextTargetInfo->SetID(TEXTID_TargetInfo);
		pTextTargetInfo->SetDropdownShadow(TRUE);
		pTextTargetInfo->SetDropdownColor(D3DCOLOR_XRGB(0, 0, 0));
		pTextTargetInfo->SetNoClip(TRUE);
		pWindowTargetInfo->AddElement(pTextTargetInfo);

		UI::ImageBox_ptr pImageIconClan = std::make_shared<UI::ImageBox>(Rectangle2D(45, 4, 16, 16));
		pImageIconClan->SetID(IMAGEID_ClanIcon);
		pWindowTargetInfo->AddElement(pImageIconClan);
	}
	pWindowTargetInfo->Hide();
	pWindowTarget->AddElement(pWindowTargetInfo);

	//Character Info
	/*UI::Window_ptr pWindowCharacterInfo = std::make_shared<UI::Window>(Rectangle2D(52, 76, 210, 59));
	pWindowCharacterInfo->SetID(WINDOWID_CharacterInfo);
	pWindowCharacterInfo->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\infobackground.png"));
	{
		UI::ImageBox_ptr pImageIconClan = std::make_shared<UI::ImageBox>(Rectangle2D(13, 15, 32, 32));
		pImageIconClan->SetID(IMAGEID_ClanIcon);
		pWindowCharacterInfo->AddElement(pImageIconClan);

		UI::Text_ptr pTextClanInfo = std::make_shared<UI::Text>(Rectangle2D(55, 16, 140, 32));
		pTextClanInfo->SetFont("Arial", 15, 0, FALSE, FALSE, -1);
		pTextClanInfo->SetID(TEXTID_ClanInfo);
		pTextClanInfo->SetMultiLine(TRUE);
		pTextClanInfo->SetWordWrap(TRUE);
		pTextClanInfo->SetNoClip(TRUE);
		pWindowCharacterInfo->AddElement(pTextClanInfo);
	}
	pWindowCharacterInfo->Hide();
	pWindowTarget->AddElement(pWindowCharacterInfo);*/

	//Background Image
	UI::ImageBox_ptr pImageBackgroundHead = std::make_shared<UI::ImageBox>(Rectangle2D(7, 11, 59, 59));
	pImageBackgroundHead->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\targetbackground.png"));
	pWindowTarget->AddElement(pImageBackgroundHead);

	//Overlay
	UI::ImageBox_ptr pImageOverlayTarget = std::make_shared<UI::ImageBox>(Rectangle2D(0, 0, 301, 80));
	pImageOverlayTarget->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\target.png"));
	pWindowTarget->AddElement(pImageOverlayTarget);

	//Life
	UI::ImageBox_ptr pImageLifeBar = std::make_shared<UI::ImageBox>(Rectangle2D(82, 39, 208, 12));
	pImageLifeBar->SetID(IMAGEID_LifeBar);
	pImageLifeBar->SetImage(UI::ImageLoader::LoadImage_("game\\images\\UI\\target\\hp.png"));
	pWindowTarget->AddElement(pImageLifeBar);

	//Type
	UI::ImageBox_ptr pImageType = std::make_shared<UI::ImageBox>(Rectangle2D(62, 34, 16, 16));
	pImageType->SetID(IMAGEID_Type);
	pImageType->SetImage(pcaImageTargetType[TARGETTYPE_NPC]);
	pWindowTarget->AddElement(pImageType);

	//Name of Unit
	UI::Text_ptr pTextName = std::make_shared<UI::Text>(Rectangle2D(88, 21, 195, 18));
	pTextName->SetFont("Arial", 15, 0, FALSE, FALSE, -1);
	pTextName->SetID(TEXTID_Name);
	pTextName->SetHorizontalAlign(ALIGN_Center);
	pTextName->SetDropdownShadow(TRUE);
	pTextName->SetDropdownColor(D3DCOLOR_XRGB(0, 0, 0));
	pTextName->SetNoClip(TRUE);
	pWindowTarget->AddElement(pTextName);

	//Life of Unit
	UI::Text_ptr pTextLife = std::make_shared<UI::Text>(Rectangle2D(84, 38, 204, 14));
	pTextLife->SetFont("Arial", 13, 0, FALSE, FALSE, -1);
	pTextLife->SetID(TEXTID_Life);
	pTextLife->SetHorizontalAlign(ALIGN_Center);
	pTextLife->SetNoClip(TRUE);
	pWindowTarget->AddElement(pTextLife);

	//Level of Unit
	UI::Text_ptr pTextLevel = std::make_shared<UI::Text>(Rectangle2D(10, 29, 52, 30));
	pTextLevel->SetFont("Arial", 26, 0, FALSE, TRUE, -1);
	pTextLevel->SetID(TEXTID_Level);
	pTextLevel->SetColor(D3DCOLOR_XRGB(227, 191, 152));
	pTextLevel->SetHorizontalAlign(ALIGN_Center);
	pTextLevel->SetNoClip(TRUE);
	pWindowTarget->AddElement(pTextLevel);

	//Life Percent
	UI::Text_ptr pTextLifePercent = std::make_shared<UI::Text>(Rectangle2D(0, 37, 288, 20));
	pTextLifePercent->SetFont("Arial", 14, 0, FALSE, FALSE, -1);
	pTextLifePercent->SetID(TEXTID_LifePercent);
	pTextLifePercent->SetColor(D3DCOLOR_XRGB(163, 87, 87));
	pTextLifePercent->SetHorizontalAlign(ALIGN_Right);
	pTextLifePercent->SetNoClip(TRUE);
	pWindowTarget->AddElement(pTextLifePercent);

	//Range
	UI::Text_ptr pTextRange = std::make_shared<UI::Text>(Rectangle2D(0, 56, 293, 20));
	pTextRange->SetFont("Arial", 16, 0, FALSE, FALSE, -1);
	pTextRange->SetID(TEXTID_Range);
	pTextRange->SetHorizontalAlign(ALIGN_Right);
	pTextRange->SetNoClip(TRUE);
	pWindowTarget->AddElement(pTextRange);

	//More Button
	UI::Button_ptr pButtonMore = std::make_shared<UI::Button>(Rectangle2D(262, 16, 25, 25));
	pButtonMore->SetID(BUTTONID_More);
	pButtonMore->SetActiveImage(UI::ImageLoader::LoadActiveImage("game\\images\\UI\\target\\more.png", "game\\images\\UI\\target\\more_.png"));
	pButtonMore->SetDropdownMenu(pDropdownMenu);
	pWindowTarget->AddElement(pButtonMore);

	//Tooltip of Target Type
	UI::Tooltip_ptr pTooltipType = std::make_shared<UI::Tooltip>(Rectangle2D());
	pTooltipType->Init();
	pTooltipType->SetID(TOOLTIP_TargetType);
	pTooltipType->SetLinkObject(pImageType);
	pTooltipType->SetFont("Arial", 16, -1);
	pTooltipType->SetPosition(TOOLTIPPOSITION_Top);
	pTooltipType->SetFollowMouse(FALSE);
	pWindowTarget->AddElement(pTooltipType);
}

void HUDController::OnResolutionChanged()
{
	pWindowTarget->SetPosition((RESOLUTION_WIDTH >> 1) - 150, 0);
}

void HUDController::SetCanMove(BOOL value)
{
	pWindowTarget->SetCanMove(value);
}

void HUDController::Update(float fTime)
{
	if (bRenderTarget && !pWindowTarget->IsOpen())
		pWindowTarget->Show();
	else if (!bRenderTarget && pWindowTarget->IsOpen())
		pWindowTarget->Hide();

	auto pTargetInfoWindow = pWindowTarget->GetElement<UI::Window>(WINDOWID_TargetInfo);
	auto pCharacterInfoWindow = pWindowTarget->GetElement<UI::Window>(WINDOWID_CharacterInfo);

	if ((pTargetInfoWindow && pCharacterInfoWindow) && !pCharacterInfoWindow->IsOpen() && pTargetInfoWindow->IsOpen() && pTargetInfoWindow->IsHover())
		pCharacterInfoWindow->Show();
	else if ((pTargetInfoWindow && pCharacterInfoWindow) && pCharacterInfoWindow->IsOpen() && pTargetInfoWindow->IsOpen() && !pTargetInfoWindow->IsHover())
		pCharacterInfoWindow->Hide();
	else if ((pTargetInfoWindow && pCharacterInfoWindow) && pCharacterInfoWindow->IsOpen() && !pTargetInfoWindow->IsOpen())
		pCharacterInfoWindow->Hide();

	pWindowTarget->Update(fTime);
}

void HUDController::UpdateObjectsSettings()
{
	if (CHUDCONTROLLER->GetTargetWindow())
	{
		CHUDCONTROLLER->GetTargetWindow()->SetCanMove(Settings::GetInstance()->UiBlock());

		auto pTargetDistance = CHUDCONTROLLER->GetTargetWindow()->GetElement<UI::Text>(TEXTID_Range);
		if (pTargetDistance)
		{
			pTargetDistance->Show();
		}

		auto pTargetHPValue = CHUDCONTROLLER->GetTargetWindow()->GetElement<UI::Text>(TEXTID_Life);
		if (pTargetHPValue)
		{
			pTargetHPValue->Show();
		}

		auto pTargetHPPercent = CHUDCONTROLLER->GetTargetWindow()->GetElement<UI::Text>(TEXTID_LifePercent);
		if (pTargetHPPercent)
		{
			pTargetHPPercent->Show();
		}
	}

	// Ocultar/Mostrar o alerta
	if (CHATBOX->GetWindow())
	{
		CHATBOX->ToggleNotice(Settings::GetInstance()->bShowNotice);
	}
}