#include "MiniMapHandler.h"
#include <HoBaram/HoParty.h>
#include <Settings.h>
#include <Engine/UI/UI.h>
#include <HoBaram/HoLinkHeader.h>
#include <GameInfo.h>
#include <Utils/Macros.h>
#include <GameCore.h>
#include <boost/algorithm/string.hpp>
#include <set>

extern char* MapasWU8[];

CMiniMapHandler::CMiniMapHandler(EWindow eID) : CBaseWindow(eID)
{
	bSelected = FALSE;
	bMapTitleSelected = FALSE;
	sPointMouse = Point2D();
}

CMiniMapHandler::CMiniMapHandler() : CBaseWindow()
{
}

CMiniMapHandler::~CMiniMapHandler()
{
}

void CMiniMapHandler::Shutdown()
{
	if (GAMECOREHANDLE->IsInit())
	{
		CLEARPTR(pcMaskBackground);
		CLEARPTR(pcMaskBackgroundActive);
		CLEARPTR(pcBorderImage);
		CLEARPTR(pcUserPoint);
		CLEARPTR(pcNPCPoint);
		CLEARPTR(pcImageRaidIcon);
		CLEARPTR(pcImageEnemyIcon);
		CLEARPTR(pcImageFriendlyIcon);
		CLEARPTR(pcImageNextBoss);

		for (int i = 0; i < 6; i++)
		{
			CLEARPTR(pcaImagesPartyMemberIcon[i]);
		}

		UI::ImageLoader::DeleteImage(pcMaskBackground);
		UI::ImageLoader::DeleteImage(pcMaskBackgroundActive);
		UI::ImageLoader::DeleteImage(pcBorderImage);
		UI::ImageLoader::DeleteImage(pcUserPoint);
		UI::ImageLoader::DeleteImage(pcNPCPoint);
		UI::ImageLoader::DeleteImage(pcImageRaidIcon);
		UI::ImageLoader::DeleteImage(pcImageEnemyIcon);
		UI::ImageLoader::DeleteImage(pcImageFriendlyIcon);
		UI::ImageLoader::DeleteImage(pcImageNextBoss);

		for (int i = 0; i < 6; i++)
		{
			UI::ImageLoader::DeleteImage(pcaImagesPartyMemberIcon[i]);
		}

		for (int i = 0; i < 2; i++)
		{
			MiniMapData* psMaps = GetMiniMapData(i);

			if (psMaps->psImage)
			{
				CLEARPTR(psMaps->psImage);
				UI::ImageLoader::DeleteImage(psMaps->psImage);
			}

			if (psMaps->psImageTitle)
			{
				CLEARPTR(psMaps->psImageTitle);
				UI::ImageLoader::DeleteImage(psMaps->psImageTitle);
			}
		}
	}
}

void CMiniMapHandler::Init()
{
	pcMaskBackground = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\mask_minimap.tga");
	pcMaskBackgroundActive = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\mask_activeminimap.tga");
	pcBorderImage = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\border.png");
	pcBorderImageLarge = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\border_large.png");
	pcUserPoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\arrow.png");
	pcNPCPoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\npc.png");
	pcMapArrowImage = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\map_arrow.png");
	pcMapHellspawnMainImage = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\hs_main.png");
	pcMapHellspawnSecondaryImage = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\hs_sec.png");
	pcMapBossImage = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\boss_inactive.png");

	pcTeleportPoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\Warp.png");
	pcBlacksmithPoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\Anvil.png");
	pcMagicsmithPoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\Magicsmith.png");
	pcWarehousePoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\Warehouse.png");
	pcUpgradePoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\Aging-Mixing.png");
	pcSupplyPoint = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\Supply.png");

	pEffectMask = GRAPHICENGINE->GetGraphicsDevice()->GetEffectFactory()->MakeEffect("game\\scripts\\shaders\\Minimap.fx");
	pRenderTargetMask = GRAPHICENGINE->GetGraphicsDevice()->MakeRenderTarget(1000, MINIMAP_SMALL_SIZE, MINIMAP_SMALL_SIZE);
	pcImageRaidIcon = UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\raidmember.png");

	pTooltipUnit = std::make_shared<UI::Tooltip>(Rectangle2D());
	pTooltipUnit->Init();
	pTooltipUnit->SetFont("Arial", 16, -1);
	pTooltipUnit->SetPosition(TOOLTIPPOSITION_Top);
	pTooltipUnit->SetFollowMouse(FALSE);
	pTooltipUnit->SetHover(TRUE);
	pTooltipUnit->Open();

	pcImageEnemyIcon = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\enemy.png");
	pcImageFriendlyIcon = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\friendly.png");
	pcImageNextBoss = UI::ImageLoader::LoadImage_("game\\images\\UI\\minimap\\nextboss.png");

	iMiniMapIndicatorFont = FONTHANDLER->CreateFont("Arvo", 12, 0, FALSE, TRUE, -1);
	iFontID = FONTHANDLER->CreateFont("Arial", 14, 0, FALSE, FALSE, -1);

	for (size_t i = 0; i < MAX_PARTY_MEMBER; i++)
		pcaImagesPartyMemberIcon[i] = UI::ImageLoader::LoadImage_("game\\images\\UI\\party\\member%d.png", i + 1);

	ResetMiniMapScreenPositions();
}

void CMiniMapHandler::LoadMiniMap(int iIndex, sFIELD* pcMap, smSTAGE3D* pcStage)
{
	MiniMapData* psMap = GetMiniMapData(iIndex);

	psMap->pcBaseMap = pcMap;

	psMap->sRectangle = Rectangle2D(
		pcStage->StageMapRect.left,
		pcStage->StageMapRect.top,
		pcStage->StageMapRect.right,
		pcStage->StageMapRect.bottom);

	if (psMap->psImage)
	{
		CLEARPTR(psMap->psImage);
		UI::ImageLoader::DeleteImage(psMap->psImage);
		psMap->psImage = nullptr;
	}
	if (psMap->psImageTitle)
	{
		CLEARPTR(psMap->psImageTitle);
		UI::ImageLoader::DeleteImage(psMap->psImageTitle);
		psMap->psImageTitle = nullptr;
	}

	if (pcMap->szNameMap[0])
	{
		psMap->psImage = UI::ImageLoader::LoadImage_(pcMap->szNameMap);

		int iX = (pcStage->StageMapRect.right - pcStage->StageMapRect.left) >> 8;
		int iY = (pcStage->StageMapRect.bottom - pcStage->StageMapRect.top) >> 8;

		psMap->sPosition = Point2D(iX, iY);
	}

	if (pcMap->szNameTitle[0])
	{
		psMap->psImageTitle = UI::ImageLoader::LoadImage_(pcMap->szNameTitle);
	}

	firstMapId = -1;
	secondMapId = -1;

	iMiniMapSize = bLargeMiniMap ? MINIMAP_LARGE_SIZE : MINIMAP_SMALL_SIZE;
	OnMiniMapSizeChanged();

	// Atualiza npcs
	dwTickTimeNpcs = 0;
}

void CMiniMapHandler::Render()
{
	if (GameMode == 2 && ISSHOW_MINIMAP)
	{
		bIsHoverUnit = FALSE;

		RenderMap(UNITDATA->sPosition);

		int iBorderWidth = 144;
		int iBorderHeight = 158;

		if (bLargeMiniMap)
		{
			iBorderWidth += 256;
			iBorderHeight += 256;
		}

		UI::ImageRender::Render(bLargeMiniMap ? pcBorderImageLarge : pcBorderImage, GetMiniMapPostion().iX - 9, GetMiniMapPostion().iY - 24, iBorderWidth, iBorderHeight, -1);

		int i = UNITDATA->OnStageField >= 0 ? UNITDATA->OnStageField : 0;

		if (GetMiniMapData(i)->psImageTitle)
		{
			UI::ImageRender::Render(GetMiniMapData(i)->psImageTitle,
				GetMiniMapPostion().iX - 1,
				GetMiniMapPostion().iY - GetMiniMapData(i)->psImageTitle->GetHeight(),
				GetMiniMapData(i)->psImageTitle->GetWidth(),
				GetMiniMapData(i)->psImageTitle->GetHeight(), -1);
		}

		if (bShowBossTime)
		{
			int iHour = (dwNextBossTime / 3600) % 60;
			int iMinute = (dwNextBossTime / 60) % 60;
			int iSecond = dwNextBossTime % 60;

			UI::ImageRender::Render(pcImageNextBoss, GetMiniMapPostion().iX - 8, GetMiniMapPostion().iY - 44, 140, 23, -1);
			FONTHANDLER->Draw(iFontID, GetMiniMapPostion().iX + 23, GetMiniMapPostion().iY - 40, FormatString("Boss em: %02d:%02d:%02d", iHour, iMinute, iSecond), DT_NOCLIP, D3DCOLOR_XRGB(255, 180, 180));
		}

		if (bIsHoverUnit)
		{
			pTooltipUnit->SetText(strHoverUnit);
			pTooltipUnit->SetTextColor(cHoverColor);
			pTooltipUnit->Render(sHoverUnitBox.iX, sHoverUnitBox.iY, 0, 0, 0, 0);
		}
	}
}

BOOL CMiniMapHandler::OnMouseClick(CMouse* pcMouse)
{
	if (!ISSHOW_MINIMAP)
		FALSE;

	BOOL bRet = FALSE;

	if (pcMouse->GetEvent() == EMouseEvent::ClickDownL && !bActive)
		ResetMapTextureDeltas();

	if (bSelected || bMapTitleSelected)
	{
		if (pcMouse->GetEvent() == ClickUpL)
		{
			bSelected = FALSE;
			bMapTitleSelected = FALSE;
			iMiniMapDeltaXPrev = iMiniMapDeltaX;
			iMiniMapDeltaYPrev = iMiniMapDeltaY;
		}
	}
	else if (!bMapTitleSelected && Settings::GetInstance()->UiBlock() &&
		pcMouse->GetEvent() == EMouseEvent::ClickDownL &&
		pcMouse->GetPosition()->Inside(Rectangle2D(GetMiniMapPostion().iX, GetMiniMapPostion().iY - 16, iMiniMapSize, 16)))
	{
		bMapTitleSelected = TRUE;
		bRet = TRUE;

		sPointMouse.iX = abs(pcMouse->GetPosition()->iX - GetMiniMapPostion().iX);
		sPointMouse.iY = abs(pcMouse->GetPosition()->iY - GetMiniMapPostion().iY) - 16;
	}
	else if (!bSelected && bLargeMiniMap && pcMouse->GetEvent() == ClickDownL && pcMouse->GetPosition()->Inside(Rectangle2D(GetMiniMapPostion().iX, GetMiniMapPostion().iY, iMiniMapSize, iMiniMapSize)))
	{
		bSelected = TRUE;

		iMouseDownInitialX = pcMouse->GetPosition()->iX;
		iMouseDownInitialY = pcMouse->GetPosition()->iY;

		bRet = TRUE;
	}

	return bRet;
}

float CMiniMapHandler::GetMapScale(EMapID mapId)
{
	float fMapSizeScale = 15.0f - ((MINIMAP_TEXTURE_SCALE * 10.0f) / 100.0f);

	if (bLargeMiniMap)
		fMapSizeScale = 15.0f;

	switch (mapId)
	{
	case MAPID_DarkSanctuary:
	case MAPID_BeehiveCave:
	case MAPID_MushroomCave:
		return (float)(fMapSizeScale / 1.5);

	case MAPID_PillaiTown:
	case MAPID_CursedTempleF1:
	case MAPID_CursedTempleF2:
	case MAPID_CursedTempleF3:
		return (float)(fMapSizeScale / 1.3);

	case MAPID_QuestArena:
		return (float)(fMapSizeScale / 3.0);
	}

	return fMapSizeScale;
}

void CMiniMapHandler::OnMouseMove(CMouse* pcMouse)
{
	Rectangle2D MapRect = Rectangle2D(GetMiniMapPostion().iX, GetMiniMapPostion().iY - 16, iMiniMapSize, iMiniMapSize + 16);

	if (pcMouse->GetPosition()->Inside(MapRect))
		bActive = TRUE;
	else
		bActive = FALSE;

	if (bSelected && bLargeMiniMap)
	{
		iMiniMapDeltaX = iMiniMapDeltaXPrev + (pcMouse->GetPosition()->iX - iMouseDownInitialX);
		iMiniMapDeltaY = iMiniMapDeltaYPrev + (pcMouse->GetPosition()->iY - iMouseDownInitialY);
	}
	else if (bMapTitleSelected)
	{
		int iX = pcMouse->GetPosition()->iX - sPointMouse.iX;
		int iY = pcMouse->GetPosition()->iY - sPointMouse.iY;

		if (iX < 0)
			iX = 0;

		if (iY < 0)
			iY = 0;

		if (iY > RESOLUTION_HEIGHT - iMiniMapSize)
			iY = RESOLUTION_HEIGHT - iMiniMapSize;

		if (iX > RESOLUTION_WIDTH - iMiniMapSize)
			iX = RESOLUTION_WIDTH - iMiniMapSize;

		if (bLargeMiniMap)
		{
			iMiniMapScreenPositionX = iMiniMapLargeScreenPositionX = iX;
			iMiniMapScreenPositionY = iMiniMapLargeScreenPositionY = iY;
		}
		else
		{
			iMiniMapScreenPositionX = iMiniMapSmallScreenPositionX = iX;
			iMiniMapScreenPositionY = iMiniMapSmallScreenPositionY = iY;
		}

		SetMiniMapPostion(Rectangle2D(iMiniMapScreenPositionX, iMiniMapScreenPositionY, iMiniMapSize, iMiniMapSize));
	}
}

BOOL CMiniMapHandler::IsCurrentUserSecondMap(int iMapID)
{
	int iSecondMapIndex = (UNITDATA->OnStageField == 0) ? 1 : 0;

	if (StageField[iSecondMapIndex])
	{
		return StageField[iSecondMapIndex]->FieldCode == iMapID;
	}

	return FALSE;
}

BOOL CMiniMapHandler::IsCurrentUserMap(int iMapID)
{
	if (StageField[UNITDATA->OnStageField]->FieldCode >= 0)
	{
		if (StageField[UNITDATA->OnStageField]->FieldCode == iMapID)
			return TRUE;
	}

	return FALSE;
}


void CMiniMapHandler::RenderMap(Point3D sUserPosition)
{
	if (GRAPHICENGINE->GetGraphicsDevice()->SetRenderTarget(pRenderTargetMask))
	{
		// Clear target
		// Zbuffer = False pq buga os efeitos de glow
		GRAPHICENGINE->GetGraphicsDevice()->Clear(TRUE, FALSE, FALSE);

		if (IsCurrentUserMap(GetMiniMapData(0)->pcBaseMap->FieldCode))
		{
			firstMapId = GetMiniMapData(0)->pcBaseMap->FieldCode;
		}
		else if (IsCurrentUserSecondMap(GetMiniMapData(0)->pcBaseMap->FieldCode))
		{
			secondMapId = GetMiniMapData(0)->pcBaseMap->FieldCode;
		}

		if (GetMiniMapData(1) && GetMiniMapData(1)->pcBaseMap)
		{
			if (IsCurrentUserMap(GetMiniMapData(1)->pcBaseMap->FieldCode))
			{
				firstMapId = GetMiniMapData(1)->pcBaseMap->FieldCode;
			}
			else if (IsCurrentUserSecondMap(GetMiniMapData(1)->pcBaseMap->FieldCode))
			{
				secondMapId = GetMiniMapData(1)->pcBaseMap->FieldCode;
			}
		}

		if (firstMapId == -1)
			return;

		float fMapSizeScale = GetMapScale((EMapID)firstMapId);

		if (bLargeMiniMap)
		{
			SetMinimapSize(iMiniMapSize * 5.0f);
		}
		else
		{
			SetMinimapSize(static_cast<float>(iMiniMapSize));
		}

		int iMinimapSize = 256 * 256 * (int)fMapSizeScale;
		int iMinimapSizeDraw = iMinimapSize;
		int iShiftX = 0;
		int iShiftY = 0;

		if (bLargeMiniMap)
		{
			iMinimapSizeDraw *= 5;
			iShiftX = -(256 * 3);
			iShiftY = -(256 * 3);
		}

		DrawMinimap(GetMiniMapData(1), iMiniMapDeltaX + iShiftX, iMiniMapDeltaY + iShiftY, iMinimapSizeDraw);
		DrawMinimap(GetMiniMapData(0), iMiniMapDeltaX + iShiftX, iMiniMapDeltaY + iShiftY, iMinimapSizeDraw);

		bFirstMapBossActive = FALSE;
		bSecondMapBossActive = FALSE;

		if (bLargeMiniMap)
		{
			for (int i = 0; i < MAX_NPCS_MAP; i++)
			{
				// Npcs enviados pelo servidor para o mapa expandido
				Npcs* pc = chrNpcsInMapAndNext->sNpcs + i;

				if (&pc == nullptr)
					continue;

				if (!pc->szName[0])
					continue;

				if (RenderNPC(pc, sUserPosition, iMinimapSize))
					continue;
			}
		}
		else
		{
			for (int i = 0; i < OTHER_PLAYER_MAX; i++)
			{
				smCHAR* pc = chrOtherPlayer + i;
				Npcs pNpc;

				if (&pc == nullptr)
					continue;

				// Mostra só os npcs
				if (pc->smCharInfo.State == smCHAR_STATE_NPC)
				{
					pNpc.Position = Point3D(pc->pX, pc->pY, pc->pZ);
					STRINGCOPY(pNpc.szName, pc->smCharInfo.szName);

					if (!pNpc.szName[0])
						continue;

					if (RenderNPC(&pNpc, sUserPosition, iMinimapSize))
						continue;
				}

				if (pc->OnStageField >= 0 && pc->Flag && (StageField[pc->OnStageField]->FieldCode == StageField[UNITDATA->OnStageField]->FieldCode ||
					StageField[pc->OnStageField]->FieldCode == MAPID_Bellatra && StageField[UNITDATA->OnStageField]->FieldCode == MAPID_NaviskoTown))
				{
					if (RenderPlayer(pc, sUserPosition, UNITDATA->dwClanCode, iMinimapSize))
						continue;
				}
			}
		}

		auto itFirstMap = mMapIndicators.find(firstMapId);

		if (itFirstMap != mMapIndicators.end())
		{
			for (int i = 0; i < itFirstMap->second.iCount; i++)
			{
				auto indicator = &itFirstMap->second.mapIndicators[i];
				RenderMapIndicator(FALSE, indicator, sUserPosition, iMinimapSize);
			}
		}

		auto itSecondMap = mMapIndicators.find(secondMapId);
		if (itSecondMap != mMapIndicators.end())
		{
			for (int i = 0; i < itSecondMap->second.iCount; i++)
			{
				auto indicator = &itSecondMap->second.mapIndicators[i];

				if (indicator->eType == MapIndicator::Type::NextMapArrow)
				{
					BOOL bSkipRender = FALSE;

					if (itFirstMap != mMapIndicators.end())
					{
						for (int i = 0; i < itFirstMap->second.iCount; i++)
						{
							auto indicatorFirst = &itFirstMap->second.mapIndicators[i];

							if (indicatorFirst->eType != MapIndicator::Type::NextMapArrow)
								continue;

							const int iDeltaX = indicator->iCompactPosX - indicatorFirst->iCompactPosX;
							const int iDeltaZ = indicator->iCompactPosZ - indicatorFirst->iCompactPosZ;
							const int iDeltaXY = (iDeltaX * iDeltaX) + (iDeltaZ * iDeltaZ);

							if (iDeltaXY < DISTANCE_XY_60_meters)
							{
								bSkipRender = TRUE;
								break;
							}
						}
					}

					if (bSkipRender)
						break;

					RenderNextMapIndicator(indicator->iValue, indicator->iCompactPosX, indicator->iCompactPosZ, indicator->iAngleY, sUserPosition, iMinimapSize);
				}
				else
				{
					RenderMapIndicator(TRUE, indicator, sUserPosition, iMinimapSize);
				}
			}
		}

		RenderParty(sUserPosition, UNITDATA->dwObjectSerial, iMinimapSize);
		RenderUser(sUserPosition, UNITDATA->sAngle, iMiniMapSize);
		SetMinimapSize(static_cast<float>(iMiniMapSize));

		GRAPHICENGINE->GetGraphicsDevice()->UnsetRenderTarget();

		UI::ImageRender::SetEffect(pEffectMask);
		pEffectMask->SetTechnique("MaskScale");

		if (pEffectMask->Begin() >= 0)
		{
			D3DXHANDLE hTexture = pEffectMask->Get()->GetParameterByName(0, "MaskTexture");
			pEffectMask->Get()->SetTexture(hTexture, pcMaskBackgroundActive->GetTexture());

			UI::ImageRender::Render(pRenderTargetMask->GetTexture(), GetMiniMapPostion().iX, GetMiniMapPostion().iY, pRenderTargetMask->GetWidth(), pRenderTargetMask->GetHeight(), 0, 0, D3DCOLOR_XRGB(255, 255, 255), FALSE);

			pEffectMask->End();
		}

		UI::ImageRender::SetEffect(NULL);
	}
}

void CMiniMapHandler::SetMinimapSize(float fSize)
{
	MapScale1 = (fSize / 2.0f);
	MapScale2 = (fSize - 2.0f);
}

void CMiniMapHandler::RenderUser(Point3D sUserPosition, Point3D sUserAngle, int iScale)
{
	Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, sUserPosition, pcUserPoint->GetWidth() >> 1, pcUserPoint->GetHeight() >> 1, iScale);

	//Angle of User Point
	int iAngle = ((sUserAngle.iY * 360) / PTANGLE_360) + 180;

	UI::ImageRender::Render(pcUserPoint, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcUserPoint->GetWidth(), pcUserPoint->GetHeight(), 0, 0, -1, 0.7f, (float)iAngle);
}

void CMiniMapHandler::RenderMapIndicator(BOOL bIsSecondaryMap, MapIndicator* pMapIndicator, Point3D sUserPosition, int iSize)
{
	if (pMapIndicator->eType == MapIndicator::Type::NextMapArrow)
	{
		RenderNextMapIndicator(pMapIndicator->iValue, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize);
	}
	else if (pMapIndicator->eType == MapIndicator::Type::HellSpawnMain)
	{
		RenderMapIndicator(pcMapHellspawnMainImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize);
	}
	else if (pMapIndicator->eType == MapIndicator::Type::HellSpawnSecondary)
	{
		RenderMapIndicator(pcMapHellspawnSecondaryImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize);
	}
	else if (pMapIndicator->eType == MapIndicator::Type::BossIcon)
	{
		if (bIsSecondaryMap && !bSecondMapBossActive)
			RenderMapIndicator(pcMapBossImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize);

		if (!bIsSecondaryMap && !bFirstMapBossActive)
			RenderMapIndicator(pcMapBossImage, pMapIndicator->iCompactPosX, pMapIndicator->iCompactPosZ, pMapIndicator->iAngleY, sUserPosition, iSize);
	}
}

void CMiniMapHandler::RenderMapIndicator(UI::Image* pImage, int iCompactPosX, int iCompactPosZ, int iAngleY, Point3D sUserPosition, int iSize, const std::string szDescription)
{
	if (pImage == NULL)
		return;

	Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, iCompactPosX << 8, iCompactPosZ << 8, pImage->GetWidth() >> 1, pImage->GetHeight() >> 1, iSize);

	float fRotation = 0.0f;

	int iAngle = ((iAngleY * 360) / PTANGLE_360) + 180;


	int iXLocation = sPosition.iX + GetMiniMapPostion().iX + iMiniMapDeltaX;
	int iYLocation = sPosition.iY + GetMiniMapPostion().iY + iMiniMapDeltaY;

	int iMapCenterX = GetMiniMapPostion().iX + iMiniMapSize / 2;
	int iMapCenterY = GetMiniMapPostion().iY + iMiniMapSize / 2;

	int iDistFromCenterX = iXLocation - iMapCenterX;
	int iDistFromCenterY = iYLocation - iMapCenterY;

	int iInputRotX = static_cast<int>(iDistFromCenterX * cos(fRotation) - iDistFromCenterY * sin(fRotation));
	int iInputRotY = static_cast<int>(iDistFromCenterY * cos(fRotation) + iDistFromCenterX * sin(fRotation));

	int FinalX = iMapCenterX + iInputRotX + pImage->GetWidth() / 2 - 5;
	int FinalY = iMapCenterY + iInputRotY;

	Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pImage->GetWidth() + 10, pImage->GetHeight());

	float fRotationDX = 0.0f;

	UI::ImageRender::Render(pImage, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pImage->GetWidth(), pImage->GetHeight(), 0, 0, -1, 0.7f, (float)iAngle);

	if (!szDescription.empty())
	{
		FONTHANDLER->Draw(iMiniMapIndicatorFont, sPosition.iX + iMiniMapDeltaX + pcMapArrowImage->GetWidth(), sPosition.iY + iMiniMapDeltaY, szDescription.c_str(), DT_NOCLIP, D3DCOLOR_ARGB(255, 199, 195, 95));
	}
}

void CMiniMapHandler::RenderNextMapIndicator(int iNextMapID, int iCompactPosX, int iCompactPosZ, int iAngleY, Point3D sUserPosition, int iSize)
{
	if (iNextMapID < 0 || iNextMapID >= 60) // 60 = field máximo
		return;

	if (pcMapArrowImage == NULL)
		return;

	std::string desc = "";

	if (sField[iNextMapID].LimitLevel > 1)
	{
		desc = FormatString("%s (Lv. %d+)", MapasWU8[iNextMapID], sField[iNextMapID].LimitLevel);
	}
	else
	{
		desc = FormatString("%s ", MapasWU8[iNextMapID]);
	}

	RenderMapIndicator(pcMapArrowImage, iCompactPosX, iCompactPosZ, iAngleY, sUserPosition, iSize, desc);
}

bool FindNameInString(string name, string typed, string notContains = "")
{
	auto vNames = split(typed, ' ');
	auto vNotContains = split(notContains, ' ');

	bool found = false;

	for (auto p : vNames)
	{
		if (boost::iequals(p, name.c_str()))
		{
			found = true;
			break;
		}
	}

	if (notContains.length() > 0)
	{
		for (auto p : vNames)
		{
			for (auto p2 : vNotContains)
			{
				if (boost::iequals(p, p2))
				{
					found = false;
					break;
				}
			}
		}
	}

	return found;
}

BOOL CMiniMapHandler::RenderNPC(Npcs* pcUnitData, Point3D sUserPosition, int iSize)
{
	UI::Image* img = pcNPCPoint;

	// Compara nome dos npcs para exibir o ícone correspondente
	if (FindNameInString(std::string("Ferreiro"), std::string(pcUnitData->szName)))
		img = pcBlacksmithPoint;
	if (FindNameInString(std::string("Vigia"), std::string(pcUnitData->szName)))
		img = pcWarehousePoint;
	else if (FindNameInString(std::string("Loja"), std::string(pcUnitData->szName), std::string("Tempo Coins")))
		img = pcSupplyPoint;
	else if (boost::iequals("Mestre Raymon", pcUnitData->szName))
		img = pcMagicsmithPoint;
	else if (boost::iequals("Mestre Verkan", pcUnitData->szName))
		img = pcMagicsmithPoint;
	else if (boost::iequals("Inventor dos Portais", pcUnitData->szName))
		img = pcTeleportPoint;
	else if (boost::iequals("Salon, Craft de Itens", pcUnitData->szName))
		img = pcBlacksmithPoint;
	else if (boost::iequals("Misterioso Moriph", pcUnitData->szName))
		img = pcUpgradePoint;
	else if (boost::iequals("Mixador Morald", pcUnitData->szName))
		img = pcUpgradePoint;

	if (img)
	{
		Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->Position, img->GetWidth() >> 1, img->GetHeight() >> 1, iSize);

		float fRotation = 0.0f;

		int iXLocation = sPosition.iX + GetMiniMapPostion().iX + iMiniMapDeltaX;
		int iYLocation = sPosition.iY + GetMiniMapPostion().iY + iMiniMapDeltaY;

		int iMapCenterX = GetMiniMapPostion().iX + iMiniMapSize / 2;
		int iMapCenterY = GetMiniMapPostion().iY + iMiniMapSize / 2;

		int iDistFromCenterX = iXLocation - iMapCenterX;
		int iDistFromCenterY = iYLocation - iMapCenterY;

		int iInputRotX = static_cast<int>(iDistFromCenterX * cos(fRotation) - iDistFromCenterY * sin(fRotation));
		int iInputRotY = static_cast<int>(iDistFromCenterY * cos(fRotation) + iDistFromCenterX * sin(fRotation));

		int FinalX = iMapCenterX + iInputRotX + img->GetWidth() / 2 - 5;
		int FinalY = iMapCenterY + iInputRotY;

		Rectangle2D sBox = Rectangle2D(FinalX, FinalY, img->GetWidth() + 10, img->GetHeight());

		float fRotationDX = 0.0f;

		UI::ImageRender::Render(img, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, img->GetWidth(), img->GetHeight(), 0, 0, -1, 1.0f, fRotationDX);

		if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
			MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
		{
			bIsHoverUnit = TRUE;
			sHoverUnitBox = sBox;
			strHoverUnit = pcUnitData->szName;
			cHoverColor = UI::Color(255, 255, 255, 255);
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderPlayer(smCHAR* pcUnitData, Point3D sUserPosition, int iUserClanID, int iSize)
{
	if (pcUnitData->smCharInfo.Brood == smCHAR_MONSTER_USER || pcUnitData->smCharInfo.State == smCHAR_STATE_USER)
	{
		if (pcUnitData->dwClanCode > 0 && pcUnitData->dwClanCode == iUserClanID)
		{
			if (pcUnitData->sPosition.WithinPTDistance(&sUserPosition, (64 * 64)))
			{
				Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, pcImageFriendlyIcon->GetWidth() >> 1, pcImageFriendlyIcon->GetHeight() >> 1, iSize);

				UI::ImageRender::Render(pcImageFriendlyIcon, sPosition.iX, sPosition.iY, pcImageFriendlyIcon->GetWidth(), pcImageFriendlyIcon->GetHeight(), 0, 0, -1);
			}
		}
		else if (StageField[UNITDATA->OnStageField]->FieldCode == START_FIELD_CASTLE) // BC
		{
			if (pcUnitData->sPosition.WithinPTDistance(&sUserPosition, (64 * 64)))
			{
				Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, pcUnitData->sPosition, pcImageEnemyIcon->GetWidth() >> 1, pcImageEnemyIcon->GetHeight() >> 1, iSize);

				UI::ImageRender::Render(pcImageEnemyIcon, sPosition.iX, sPosition.iY, pcImageEnemyIcon->GetWidth(), pcImageEnemyIcon->GetHeight(), 0, 0, -1);
			}
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CMiniMapHandler::RenderParty(Point3D sUserPosition, int iUserID, int iSize)
{
	if (PARTYHANDLER->GetPartyData().iMembersCount > 0)
	{
		for (int i = 0; i < PARTYHANDLER->GetPartyData().iMembersCount; i++)
		{
			PartyMember sMember = PARTYHANDLER->GetPartyData().saMembers[i];
			UI::Image* pImage = pcaImagesPartyMemberIcon[i];

			if (sMember.sData.sLevel > 0 && (sMember.sData.iID != iUserID) && pImage)
			{
				Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, sMember.sData.sPosition, pImage->GetWidth() >> 1, pImage->GetHeight() >> 1, iSize);

				float fRotation = 0.0f;

				int iXLocation = sPosition.iX + GetMiniMapPostion().iX + iMiniMapDeltaX;
				int iYLocation = sPosition.iY + GetMiniMapPostion().iY + iMiniMapDeltaY;

				int iMapCenterX = GetMiniMapPostion().iX + iMiniMapSize / 2;
				int iMapCenterY = GetMiniMapPostion().iY + iMiniMapSize / 2;

				int iDistFromCenterX = iXLocation - iMapCenterX;
				int iDistFromCenterY = iYLocation - iMapCenterY;

				int iInputRotX = static_cast<int>(iDistFromCenterX * cos(fRotation) - iDistFromCenterY * sin(fRotation));
				int iInputRotY = static_cast<int>(iDistFromCenterY * cos(fRotation) + iDistFromCenterX * sin(fRotation));

				int FinalX = iMapCenterX + iInputRotX + pImage->GetWidth() / 2 - 5;
				int FinalY = iMapCenterY + iInputRotY;

				Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pImage->GetWidth() + 20, pImage->GetHeight());

				float fRotationDX = 0.0f;
				UI::ImageRender::Render(pImage, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pImage->GetWidth(), pImage->GetHeight(), 0, 0, -1, 1.0f, fRotationDX);

				if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
					MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
				{
					bIsHoverUnit = TRUE;
					sHoverUnitBox = sBox;
					strHoverUnit = sMember.szCharacterName;
					cHoverColor = UI::Color(255, 175, 230, 175);
				}
			}
		}
	}

	if (PARTYHANDLER->GetPartyData().iRaidCount > 0)
	{
		for (int i = 0; i < PARTYHANDLER->GetPartyData().iRaidCount; i++)
		{
			for (size_t j = 0; j < MAX_PARTY_MEMBERS; j++)
			{
				PartyRaidMember sMember = PARTYHANDLER->GetPartyData().saRaid[i].saMembers[j];

				if (sMember.sData.iID && sMember.szCharacterName[0] != 0)
				{
					Point2D sPosition = GetMinimapPositionFromCenter(sUserPosition, sMember.sData.sPosition, pcImageRaidIcon->GetWidth() >> 1, pcImageRaidIcon->GetHeight() >> 1, iSize);

					float fRotation = 0.0f;

					int iXLocation = sPosition.iX + GetMiniMapPostion().iX + iMiniMapDeltaX;
					int iYLocation = sPosition.iY + GetMiniMapPostion().iY + iMiniMapDeltaY;

					int iMapCenterX = GetMiniMapPostion().iX + iMiniMapSize / 2;
					int iMapCenterY = GetMiniMapPostion().iY + iMiniMapSize / 2;

					int iDistFromCenterX = iXLocation - iMapCenterX;
					int iDistFromCenterY = iYLocation - iMapCenterY;

					int iInputRotX = static_cast<int>(iDistFromCenterX * cos(fRotation) - iDistFromCenterY * sin(fRotation));
					int iInputRotY = static_cast<int>(iDistFromCenterY * cos(fRotation) + iDistFromCenterX * sin(fRotation));

					int FinalX = iMapCenterX + iInputRotX + pcImageRaidIcon->GetWidth() / 2 - 5;
					int FinalY = iMapCenterY + iInputRotY;

					Rectangle2D sBox = Rectangle2D(FinalX, FinalY, pcImageRaidIcon->GetWidth() + 10, pcImageRaidIcon->GetHeight());

					float fRotationDX = 0.0f;

					UI::ImageRender::Render(pcImageRaidIcon, sPosition.iX + iMiniMapDeltaX, sPosition.iY + iMiniMapDeltaY, pcImageRaidIcon->GetWidth(), pcImageRaidIcon->GetHeight(), 0, 0, -1, 1.0f, fRotationDX);

					if (MOUSEHANDLER->GetPosition()->Inside(sBox) &&
						MOUSEHANDLER->GetPosition()->Inside(GetMiniMapBox()))
					{
						bIsHoverUnit = TRUE;
						sHoverUnitBox = sBox;
						strHoverUnit = sMember.szCharacterName;
						cHoverColor = UI::Color(255, 40, 240, 230);
					}
				}
			}
		}
	}

	return TRUE;
}

BOOL CMiniMapHandler::RenderMapTime()
{
	return TRUE;
}

BOOL CMiniMapHandler::IsCurrentUserMap(MiniMapData* psMap)
{
	if (StageField[UNITDATA->OnStageField]->FieldCode >= 0)
	{
		if (sField[UNITDATA->OnStageField].FieldCode == psMap->pcBaseMap->FieldCode)
			return TRUE;
	}

	return FALSE;
}

void CMiniMapHandler::ResetMapTextureDeltas()
{
	iMiniMapDeltaY = 0;
	iMiniMapDeltaX = 0;
	iMiniMapDeltaXPrev = 0;
	iMiniMapDeltaYPrev = 0;
}

void CMiniMapHandler::OnMiniMapSizeChanged()
{
	pRenderTargetMask = GRAPHICENGINE->GetGraphicsDevice()->MakeRenderTarget(1000, iMiniMapSize, iMiniMapSize);

	if (bLargeMiniMap)
	{
		iMiniMapScreenPositionX = iMiniMapLargeScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapLargeScreenPositionY;
	}
	else
	{
		iMiniMapScreenPositionX = iMiniMapSmallScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapSmallScreenPositionY;
	}

	SetMiniMapPostion(Rectangle2D(iMiniMapScreenPositionX, iMiniMapScreenPositionY, iMiniMapSize, iMiniMapSize));
	ResetMapTextureDeltas();
}

void CMiniMapHandler::DrawMinimap(MiniMapData* psMap, int iX, int iY, int iSize)
{
	//Point2D sPosition = GetMinimapPositionFromCenter(UNITDATA->sPosition, pcUnitData->Position, img->GetWidth() >> 1, img->GetHeight() >> 1, iSize);

	Rectangle2D sRectangle = psMap->sRectangle;
	Point3D sPosition = UNITDATA->sPosition;

	float fRenderX = (float)iX;
	float fRenderY = (float)iY;

	int iWidth = psMap->sPosition.iX;
	int iHeight = psMap->sPosition.iY;

	int iMapX = ((sPosition.iX - sRectangle.iX) >> 8);
	int iMapZ = ((sRectangle.iHeight - sPosition.iZ) >> 8);

	double fRenderWidth = (double)(iSize >> 8);
	double fRenderHeight = (double)iWidth;

	// Organizar as variáveis aqui dps
	// Pego do game do KPT pra exibir o minimapa certinho

	float v11 = 0;
	float v12 = 0;
	float v13 = 0;
	float v14 = 0;
	float v15 = 0;
	float v16 = 0;

	float v24 = MapScale2;
	float v23 = MapScale2;
	float v21 = fRenderWidth / fRenderHeight;
	float v27 = iHeight;
	float v8 = iWidth;

	float v22 = fRenderWidth / iHeight;
	float v20 = iMapX / v8 - v21;
	double v10 = (double)iMapZ / v27;
	float v18 = iMapX / v8 + v21;

	float v19 = v10 - v22;
	float v28 = v10 + v22;

	if (v20 < 0.0 && v18 < 0.0)
		return;
	if (v20 > 1.0 && v18 > 1.0 || v19 < 0.0 && v28 < 0.0 || v19 > 1.0 && v28 > 1.0)
		return;

	if (v20 < 0.0)
	{
		v11 = MapScale1 / v21 * v20;
		v20 = 0.0;
		v12 = -v11;
		fRenderX = fRenderX + v12;
		v24 = (float)MapScale2 - v12;
	}
	if (v18 > 1.0)
	{
		v13 = v18 - 1.0;
		v18 = 1.0;
		v24 = v24 - MapScale1 / v21 * v13;
	}
	if (v19 < 0.0)
	{
		v14 = MapScale1 / v22 * v19;
		v19 = 0.0;
		v15 = -v14;
		fRenderY = fRenderY + v15;
		v23 = (float)MapScale2 - v15;
	}
	if (v28 > 1.0)
	{
		v16 = v28 - 1.0;
		v28 = 1.0;
		v23 = v23 - MapScale1 / v22 * v16;
	}

	if (psMap->psImage)
	{
		UI::ImageRender::RenderQuad(psMap->psImage, fRenderX, fRenderY, v24, v23, Rectangle2DF(v20, v19, v18, v28), -1);
	}
}

Point2D CMiniMapHandler::GetMinimapPositionFromCenter(Point3D sCenterPosition, Point3D sObjectPositiion, int iWidthPoint, int iHeightPoint, int iSize)
{
	return GetMinimapPositionFromCenter(sCenterPosition, sObjectPositiion.iX, sObjectPositiion.iZ, iWidthPoint, iHeightPoint, iSize);
}

Point2D CMiniMapHandler::GetMinimapPositionFromCenter(Point3D sCenterPosition, int iDetailedX, int iDetailedZ, int iWidthPoint, int iHeightPoint, int iSize)
{
	int iX = iDetailedX - sCenterPosition.iX;
	int iZ = iDetailedZ - sCenterPosition.iZ;

	iX /= (iSize / (iMiniMapSize / 2));
	iZ /= (iSize / (iMiniMapSize / 2));

	iZ = -iZ;

	return Point2D(iX + (iMiniMapSize >> 1) - iWidthPoint, iZ + (iMiniMapSize >> 1) - iHeightPoint);
}

void CMiniMapHandler::ResetMiniMapScreenPositions()
{
	iMiniMapSmallScreenPositionX = (RESOLUTION_WIDTH - MINIMAP_SMALL_SIZE) - 16;
	iMiniMapSmallScreenPositionY = (RESOLUTION_HEIGHT - MINIMAP_SMALL_SIZE) - 50;

	iMiniMapLargeScreenPositionX = (RESOLUTION_WIDTH - MINIMAP_LARGE_SIZE) - 16;
	iMiniMapLargeScreenPositionY = (RESOLUTION_HEIGHT - MINIMAP_LARGE_SIZE) - 50;

	if (bLargeMiniMap)
	{
		iMiniMapScreenPositionX = iMiniMapLargeScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapLargeScreenPositionY;
	}
	else
	{
		iMiniMapScreenPositionX = iMiniMapSmallScreenPositionX;
		iMiniMapScreenPositionY = iMiniMapSmallScreenPositionY;
	}

	SetMiniMapPostion(Rectangle2D(iMiniMapScreenPositionX, iMiniMapScreenPositionY, iMiniMapSize, iMiniMapSize));
}

void CMiniMapHandler::OnResolutionChanged()
{
	ResetMiniMapScreenPositions();
}

void CMiniMapHandler::Update()
{
	if (dwNextBossTime > 0)
	{
		dwNextBossTime--;

		if (dwNextBossTime == 0)
		{
			bShowBossTime = FALSE;

			OnChangeMap(StageField[UNITDATA->OnStageField >= 0 ? UNITDATA->OnStageField : 0]->FieldCode, secondMapId);
		}
	}

	if (dwTickTimeNpcs >= 0)
	{
		if (dwTickTimeNpcs > 0)
			dwTickTimeNpcs--;

		if (dwTickTimeNpcs == 0)
		{
			dwTickTimeNpcs = 10; // 10 segundos

			if (firstMapId != -1)
				OnRequestNpcs(firstMapId, secondMapId);
		}
	}
}

void CMiniMapHandler::HandlePacket(PacketNpcsInMapAndNext* psPacket)
{
	ZeroMemory(chrNpcsInMapAndNext, sizeof(PacketNpcsInMapAndNext));
	memcpy(&chrNpcsInMapAndNext, psPacket, sizeof(PacketNpcsInMapAndNext));
}

void CMiniMapHandler::HandlePacket(PacketMapIndicators* psPacket)
{
	auto it = mMapIndicators.find(psPacket->iMapID);

	if (it == mMapIndicators.end())
	{
		PacketMapIndicators packetCopy;
		mMapIndicators.insert({ psPacket->iMapID, packetCopy });
	}

	ZeroMemory(&mMapIndicators[psPacket->iMapID], sizeof(PacketMapIndicators));
	CopyMemory(&mMapIndicators[psPacket->iMapID], psPacket, psPacket->size);
}

void CMiniMapHandler::HandlePacket(PacketBossTimeUpdate* psPacket)
{
	strNextBossName = psPacket->szBossName;
	dwNextBossTime = psPacket->dwTime;

	if (dwNextBossTime)
		bShowBossTime = TRUE;
	else
		bShowBossTime = FALSE;
}

void CMiniMapHandler::OnRequestNpcs(int FirstID, int SecondID)
{
	smTRANS_COMMAND PacketRequest;
	PacketRequest.code = PACKET_GET_NPCS;
	PacketRequest.size = sizeof(smTRANS_COMMAND);
	PacketRequest.WParam = FirstID;
	PacketRequest.LParam = SecondID;
	PacketRequest.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&PacketRequest, PacketRequest.size, TRUE);
}

void CMiniMapHandler::OnChangeMap(int iMapID, int SecondMapID)
{
	PacketBossTimeUpdate psPacket = { 0 };

	STRINGCOPY(psPacket.szBossName, "");
	psPacket.size = sizeof(PacketBossTimeUpdate);
	psPacket.code = PACKET_UPDATE_BOSSTIME;
	psPacket.dwTime = 0;
	psPacket.iMapID = iMapID;
	psPacket.SecondMapID = SecondMapID;

	if (smWsockServer)
		smWsockServer->Send((char*)&psPacket, psPacket.size, FALSE);
}