#include "..\\sinbaram\\sinLinkHeader.h"


extern sITEM* BackUpInvenItem2;
extern sITEM* BackUpInvenTempItem2;
extern sITEM TempPerfectItem2;
extern sINVENTORY* BackUpsInven;
extern int InvenGold;
extern int RealGold;
extern int	TradeCrashItemIndex[2];
extern int TradeColorIndex;
extern int SelectTradeItemIndex;

extern int	SaveCaravan(sCARAVAN* lpCaravan);
extern BOOL bIsPerfect2;

int UpTradePosi2[9][4] = {
	{ 21, 136 + sinInterHeight2, 21 + (22 * 9), 136 + (22 * 4) + sinInterHeight2 },
	{ 224, 90, 300, 108 },
	{ 248, 124, 324, 142 },
	{ 248, 174, 298, 192 },
	{ 300, 217 + sinInterHeight2, 300 + 26, 217 + 26 + sinInterHeight2 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 0, 0, 0, 0 },
	{ 248, 150, 324, 176 },
};

int DownTradePosi2[9][4] = {
	{ 21, 224 + 21 + sinInterHeight2, 21 + (22 * 9), 224 + 21 + (22 * 4) + sinInterHeight2 },
	{ 225, 205, 250, 226 },
	{ 248, 212, 324, 230 },
	{ 248, 262, 298, 280 },
	{ 300, 305 + sinInterHeight2, 300 + 26, 305 + 26 + sinInterHeight2 },
	{ 305, 314 + sinInterHeight2, 304 + 20, 314 + 20 + sinInterHeight2 },
	{ 311, 136 + sinInterHeight2, 310 + 20, 136 + 20 + sinInterHeight2 },
	{ 225, 231, 250, 252 },
	{ 248, 238, 324, 256 },
};

int TradeStartX2 = DownTradePosi2[T_SET_BOX][0], TradeStartY2 = DownTradePosi2[T_SET_BOX][1], TradeEndX2 = DownTradePosi2[T_SET_BOX][2], TradeEndY2 = DownTradePosi2[T_SET_BOX][3];

int Caravana::Init()
{
	return TRUE;
}

int Caravana::CloseCaravan()
{
	int RestoreFlag = 1;

	CloseCaravanCheckFlag3 = 1;

	OpenFlag = 0;

	if (CaravanSaveFlag)
	{
		if (!SaveCaravan(&sCaravan))
		{
			RestoreInvenItem();
			cMessageBox.ShowMessage(MESSAGE_OVER_ITEM_NUM);
			RestoreFlag = 0;
		}
		else
		{
			if (BackUpInvenItem2) {
				delete BackUpInvenItem2;
				BackUpInvenItem2 = 0;

			}
			if (BackUpInvenTempItem2) {
				delete BackUpInvenTempItem2;
				BackUpInvenTempItem2 = 0;

			}
			if (BackUpsInven) {
				delete BackUpsInven;
				BackUpsInven = 0;

			}

		}

	}

	CaravanSaveFlag = 0;
	ResetInvenItemCode();
	ResetInvenMoney();
	return RestoreFlag;
}

extern int CopyItemIndex7[10];

int Caravana::CopyItemNotPickUp(sITEM* pItem, int JumpIndex)
{
	int i;
	if (smConfig.DebugMode)return TRUE;
	for (i = 0; i < 100; i++) {
		if (sCaravan.CaravanItem[i].Flag)
		{
			if (JumpIndex == i)continue;
			if (CompareItems(&pItem->sItemInfo, &sCaravan.CaravanItem[i].sItemInfo))
			{
				CopyItemIndex7[i] = i + 1;
				return FALSE;

			}
		}
	}
	return TRUE;
}

bool Caravana::CanStoreItem(sITEM* pItem)
{
	if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
		(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
		pItem->sItemInfo.expireTime > 0)
	{
		return false;
	}

	int kk = 0;
	for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
		if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
			return false;
		}
	}
	for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
		if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
			return false;
		}
	}

	if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_EVENT) {
		return false;
	}
	if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1) {
		return false;
	}
	return true;
}

bool Caravana::GetEmptyArea(sITEM* pItem, POINT* EmptyPos)
{
	int X = 21;
	int Y = 136 + sinInterHeight2;
	int Max_X = X + (22 * 9);
	int Max_Y = Y + (22 * 9);

	RECT Rectangle = { 0 };
	int Indexs[2] = { 0 };

	for (int i = 21; i < Max_X; i += 22) {
		for (int j = Y; j < Max_Y; j += 22) {
			if (X <= i && Max_X > i && Y <= j && Max_Y > j) {
				Rectangle.left = X + (((i - X) / 22) * 22);
				Rectangle.top = Y + (((j - Y) / 22) * 22);
				Rectangle.right = pItem->w;
				Rectangle.bottom = pItem->h;

				if (Rectangle.left + Rectangle.right > Max_X)
					continue;
				if (Rectangle.top + Rectangle.bottom > Max_Y)
					continue;
			}

			if (Indexs[0] = cTrade.CrashTradeItem(Rectangle, 0, 7)) {
				if (!CopyItemNotPickUp(&sCaravan.CaravanItem[Indexs[0] - 1], Indexs[0] - 1)) {
					continue;
				}
				if (Indexs[1] = cTrade.CrashTradeItem(Rectangle, Indexs[0], 7)) {
					continue;
				}
				continue;
			}

			EmptyPos->x = i;
			EmptyPos->y = j;
			return true;
		}
	}
	return false;
}

int Caravana::PickUpCaravanItem(int x, int y, int PickUpFlag)
{
	int i = 0;
	SelectTradeItemIndex = 0;
	for (i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag)
		{
			if (sCaravan.CaravanItem[i].x < x && sCaravan.CaravanItem[i].x + sCaravan.CaravanItem[i].w > x &&
				sCaravan.CaravanItem[i].y < y && sCaravan.CaravanItem[i].y + sCaravan.CaravanItem[i].h > y)
			{
				if (PickUpFlag)
				{
					sITEM TempItem;
					TempItem.w = ITEMSIZE * 2;
					TempItem.h = ITEMSIZE * 4;
					TempItem.Flag = 1;
					if (cInvenTory.CheckSetEmptyArea(&TempItem))
					{
						if (CopyItemNotPickUp(&sCaravan.CaravanItem[i], i))
						{
							CheckCaravanForm();
							memcpy(&MouseItem, &sCaravan.CaravanItem[i], sizeof(sITEM));
							sCaravan.CaravanItem[i].Flag = 0;
							sinPlaySound(sCaravan.CaravanItem[i].SoundIndex);
							TradeColorIndex = 0;
							GetWeight();
							ReFormCaravan();
							return TRUE;
						}
					}
					else
					{
						cMessageBox.ShowMessage(MESSAGE_OVER_SPACE);
						break;
					}
					if (CopyItemNotPickUp(&sCaravan.CaravanItem[i], i))
					{
						CheckCaravanForm();
						memcpy(&MouseItem, &sCaravan.CaravanItem[i], sizeof(sITEM));
						sCaravan.CaravanItem[i].Flag = 0;
						sinPlaySound(sCaravan.CaravanItem[i].SoundIndex);
						TradeColorIndex = 0;
						GetWeight();
						ReFormCaravan();
						return TRUE;
					}

				}
				else {
					SelectTradeItemIndex = i + 1;
					TradeColorIndex = SELECT_ITEM_COLOR;
					sinShowItemInfoFlag = 1;

					GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&sCaravan.CaravanItem[i], FALSE, FALSE, FALSE);
				}
			}
		}
	}
	return FALSE;
}

int Caravana::CheckCaravanForm()
{
	int TempCheckDataSum = 0;

	for (int i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag)
		{
			TempCheckDataSum += (i + 1) * sCaravan.CaravanItem[i].x;
			TempCheckDataSum += (i + 1) * sCaravan.CaravanItem[i].y;
			TempCheckDataSum += sCaravan.CaravanItem[i].sItemInfo.ItemHeader.dwChkSum;
			TempCheckDataSum += sCaravan.CaravanItem[i].sItemInfo.ItemHeader.Head;

		}
	}

	if (TempCheckDataSum != CaravanCheckSum)
		SendSetHackUser(11);

	return TRUE;

}

int Caravana::ReFormCaravan()
{
	CaravanCheckSum = 0;

	for (int i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag)
		{
			CaravanCheckSum += (i + 1) * sCaravan.CaravanItem[i].x;
			CaravanCheckSum += (i + 1) * sCaravan.CaravanItem[i].y;
			CaravanCheckSum += sCaravan.CaravanItem[i].sItemInfo.ItemHeader.dwChkSum;
			CaravanCheckSum += sCaravan.CaravanItem[i].sItemInfo.ItemHeader.Head;
		}
	}
	return TRUE;
}

extern RECT TradeColorRect;

int Caravana::LastSetCaravanItem(sITEM* pItem)
{
	CheckCaravanForm();

	for (int j = 0; j < 100; j++)
	{
		if (sCaravan.CaravanItem[j].Class == ITEM_CLASS_POTION)
		{
			if (sCaravan.CaravanItem[j].CODE == pItem->CODE && sCaravan.CaravanItem[j].Flag)
			{
				sCaravan.CaravanItem[j].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0;
				ZeroMemory(pItem, sizeof(sITEM));
				sinPlaySound(sCaravan.CaravanItem[j].SoundIndex);
				GetWeight();
				ReFormCaravan();


				return TRUE;
			}
		}

		if (!sCaravan.CaravanItem[j].Flag)
		{
			memcpy(&sCaravan.CaravanItem[j], pItem, sizeof(sITEM));
			sCaravan.CaravanItem[j].x = pItem->SetX;
			sCaravan.CaravanItem[j].y = pItem->SetY;
			pItem->Flag = 0;
			sinPlaySound(sCaravan.CaravanItem[j].SoundIndex);

			GetWeight();
			ReFormCaravan();
			return TRUE;
		}
	}
	ReFormCaravan();
	return FALSE;
}

int Caravana::SetCaravanItemAreaCheck(sITEM* pItem)
{
	int i, j;
	TradeColorIndex = 0;
	TradeCrashItemIndex[0] = 0;
	cTrade.InitTradeColorRect();

	TradeStartX2 = 21;
	TradeStartY2 = 136 + sinInterHeight2;
	TradeEndX2 = TradeStartX2 + (22 * 9);
	TradeEndY2 = TradeStartY2 + (22 * 9);

	for (i = pItem->x + 11; i < pItem->x + pItem->w; i += 22) {
		for (j = pItem->y + 11; j < pItem->y + pItem->h; j += 22) {
			if (TradeStartX2 <= i && TradeEndX2 > i && TradeStartY2 <= j && TradeEndY2 > j)
			{
				TradeColorRect.left = TradeStartX2 + (((i - TradeStartX2) / 22) * 22);
				TradeColorRect.top = TradeStartY2 + (((j - TradeStartY2) / 22) * 22);
				TradeColorRect.right = pItem->w;
				TradeColorRect.bottom = pItem->h;
				if (TradeEndX2 < (TradeColorRect.left + TradeColorRect.right) - 11 || TradeEndY2 < (TradeColorRect.top + TradeColorRect.bottom) - 11) {
					cTrade.InitTradeColorRect();
					return FALSE;
				}

				if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_QUEST ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA1 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinMA2 ||
					(pItem->sItemInfo.CODE & sinITEM_MASK2) == sinQW1 ||
					pItem->sItemInfo.CODE == (sinGF1 | sin01) ||
					pItem->sItemInfo.expireTime > 0) {

					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}
				int kk = 0;

				for (kk = 0; kk < NotSet_Item_CODECnt; kk++) {
					if (NotSet_Item_CODE[kk] == pItem->sItemInfo.CODE) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}

				}
				for (kk = 0; kk < NotSet_Item_MASKCnt; kk++) {
					if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == NotSet_Item_MASK[kk]) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}

				}
				for (kk = 0; kk < NotSet_Item_KINDCnt; kk++) {
					if (NotSet_Item_KIND[kk] == pItem->sItemInfo.ItemKindCode) {
						TradeColorIndex = NOT_SETTING_COLOR;
						return FALSE;
					}
				}

				if (pItem->sItemInfo.ItemKindCode == ITEM_KIND_EVENT) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;

				}
				if ((pItem->sItemInfo.CODE & sinITEM_MASK2) == sinCH1) {
					TradeColorIndex = NOT_SETTING_COLOR;
					return FALSE;
				}

				pItem->SetX = TradeColorRect.left;
				pItem->SetY = TradeColorRect.top;
				pItem->ItemPosition = 0;
				TradeColorIndex = SET_ITEM_CHECK_COLOR;

				if (TradeCrashItemIndex[0] = cTrade.CrashTradeItem(TradeColorRect, 0, 7))
				{
					if (!CopyItemNotPickUp(&sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1], TradeCrashItemIndex[0] - 1)) {
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[0] = 0;
						TradeCrashItemIndex[1] = 0;
						return FALSE;

					}
					if (TradeCrashItemIndex[1] = cTrade.CrashTradeItem(TradeColorRect, TradeCrashItemIndex[0], 7))
					{
						TradeColorIndex = NOT_SETTING_COLOR;
						TradeCrashItemIndex[1] = 0;
						return FALSE;
					}
					TradeColorIndex = OVERLAP_ITEM_COLOR;

				}

				return TRUE;
			}

		}

	}

	return TRUE;
}

int Caravana::ChangeCaravanItem(sITEM* pItem)
{
	// Poções na caravana
	//if (pItem->Class == ITEM_CLASS_POTION)
	//{
	//	cMessageBox.ShowMessage(MESSAGE_POTION_NOT_SETTING);
	//	return TRUE;
	//}

	if (pItem->sItemInfo.Weight + (sCaravan.Weight[0] - 197) > sCaravan.Weight[1] - 196)
	{
		cMessageBox.ShowMessage(MESSAGE_OVER_WEIGHT);
		return TRUE;
	}

	CheckCaravanForm();

	if (TradeCrashItemIndex[0])
	{
		if (sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].Class == ITEM_CLASS_POTION)
		{
			if (sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].CODE == pItem->CODE)
			{
				sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].sItemInfo.PotionCount += pItem->sItemInfo.PotionCount;
				pItem->Flag = 0;
				GetWeight();
				ReFormCaravan();
				return TRUE;

			}
		}

		memcpy(&TempItem, &sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1], sizeof(sITEM));
		sCaravan.CaravanItem[TradeCrashItemIndex[0] - 1].Flag = 0;
		TempItem.x = pItem->x;
		TempItem.y = pItem->y;
		ReFormCaravan();
		LastSetCaravanItem(pItem);
		memcpy(pItem, &TempItem, sizeof(sITEM));
		GetWeight();
		ReFormCaravan();
		return TRUE;

	}
	ReFormCaravan();
	return FALSE;
}

int Caravana::GetWeight()
{
	sCaravan.Weight[0] = 197;
	for (int i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag)
		{
			if (sCaravan.CaravanItem[i].Class == ITEM_CLASS_POTION)
				sCaravan.Weight[0] += sCaravan.CaravanItem[i].sItemInfo.PotionCount;
			else
				sCaravan.Weight[0] += sCaravan.CaravanItem[i].sItemInfo.Weight;

		}

	}

	CaravanSaveFlag = 1;

	return TRUE;
}


int Caravana::LoadCaravanItemImage()
{
	char szFilePath[256];
	int  cnt = 0;
	for (int i = 0; i < 100; i++) {
		if (sCaravan.CaravanItem[i].Flag) {
			for (int j = 0; j < MAX_ITEM; j++) {
				if (sCaravan.CaravanItem[i].sItemInfo.CODE == sItem[j].CODE) {
					sCaravan.CaravanItem[i].w = sItem[j].w;
					sCaravan.CaravanItem[i].h = sItem[j].h;
					wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
					if (!sItem[j].lpTempItem)
						sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);

					if ((sCaravan.CaravanItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ1 || (sCaravan.CaravanItem[i].sItemInfo.CODE & sinITEM_MASK2) == sinPZ2) {
						if (sCaravan.CaravanItem[i].sItemInfo.PotionCount == 2) {
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[j].ItemFilePath, sItem[j].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
							sCaravan.CaravanItem[i].w = sItem[j].w;
							sCaravan.CaravanItem[i].h = sItem[j].h;

						}
						else
						{
							for (cnt = 0; cnt < MAX_ITEM; cnt++) {
								if (sItem[cnt].sItemInfo.CODE == (sinPZ1 | sin00)) {
									break;
								}
							}
							wsprintf(szFilePath, "Image\\sinImage\\Items\\%s\\it%s.bmp", sItem[cnt].ItemFilePath, sItem[cnt].LastCategory);
							sItem[j].lpTempItem = LoadDibSurfaceOffscreen(szFilePath);
							sCaravan.CaravanItem[i].w = sItem[cnt].w;
							sCaravan.CaravanItem[i].h = sItem[cnt].h;
						}

					}
					sCaravan.CaravanItem[i].lpItem = sItem[j].lpTempItem;
					break;
				}
			}
		}
	}

	cInterFace.CheckAllBox(SIN_CARAVANA);
	return TRUE;
}

int Caravana::BackUpInvenItem()
{
	BackUpInvenItem2 = 0;
	BackUpInvenTempItem2 = 0;
	BackUpsInven = 0;

	BackUpInvenItem2 = new sITEM[100];
	BackUpInvenTempItem2 = new sITEM[100];
	BackUpsInven = new sINVENTORY[INVENTORY_MAX_POS];

	memcpy(BackUpInvenItem2, &cInvenTory.InvenItem, sizeof(sITEM) * 100);
	memcpy(BackUpInvenTempItem2, &cInvenTory.InvenItemTemp, sizeof(sITEM) * 100);
	memcpy(BackUpsInven, &sInven, sizeof(sINVENTORY) * INVENTORY_MAX_POS);

	InvenGold = sinChar->Money + 2023;

	RealGold = InvenGold * 3;

	return TRUE;
}

extern int SendServerFlag7;

int Caravana::CheckCopyItem()
{
	int i, j;

	if (SendServerFlag7)return FALSE;

	for (i = 0; i < 100; i++) {
		if (sCaravan.CaravanItem[i].Flag) {
			for (j = 0; j < 100; j++) {
				if (i != j && sCaravan.CaravanItem[j].Flag)
				{
					if (CompareItems(&sCaravan.CaravanItem[i].sItemInfo, &sCaravan.CaravanItem[j].sItemInfo)) {
						SendSetHackUser2(1010, sCaravan.CaravanItem[i].CODE);
						SendServerFlag7 = 1;
						break;
					}
				}
			}
		}
	}
	return TRUE;
}

int Caravana::DeleteCopyItem()
{
	int i, j;
	for (i = 0; i < 100; i++)
	{
		if (sCaravan.CaravanItem[i].Flag)
		{
			for (j = 0; j < 100; j++) {
				if (i != j && sCaravan.CaravanItem[j].Flag)
				{
					if (CompareItems(&sCaravan.CaravanItem[i].sItemInfo, &sCaravan.CaravanItem[j].sItemInfo))
					{
						sCaravan.CaravanItem[j].Flag = 0;
						ReFormCaravan();
						break;
					}
				}
			}
		}
	}
	return TRUE;
}

int Caravana::RestoreInvenItem()
{
	sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, FALSE);
	sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, FALSE);

	memcpy(&cInvenTory.InvenItem, BackUpInvenItem2, sizeof(sITEM) * 100);
	memcpy(&cInvenTory.InvenItemTemp, BackUpInvenTempItem2, sizeof(sITEM) * 100);
	memcpy(&sInven, BackUpsInven, sizeof(sINVENTORY) * INVENTORY_MAX_POS);

	if (MouseItem.Flag)
		MouseItem.Flag = 0;

	if (BackUpInvenItem2) {
		delete BackUpInvenItem2;
		BackUpInvenItem2 = 0;

	}
	if (BackUpInvenTempItem2) {
		delete BackUpInvenTempItem2;
		BackUpInvenTempItem2 = 0;

	}
	if (BackUpsInven) {
		delete BackUpsInven;
		BackUpsInven = 0;

	}

	if (sInven[0].ItemIndex) {
		sinSetCharItem(cInvenTory.InvenItem[sInven[0].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[0].ItemIndex - 1].SetModelPosi, TRUE);

	}

	if (sInven[1].ItemIndex) {
		sinSetCharItem(cInvenTory.InvenItem[sInven[1].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[1].ItemIndex - 1].SetModelPosi, TRUE);

	}



	if (sInven[2].ItemIndex) {
		sinSetCharItem(cInvenTory.InvenItem[sInven[2].ItemIndex - 1].CODE, cInvenTory.InvenItem[sInven[2].ItemIndex - 1].SetModelPosi, TRUE);

	}
	if ((InvenGold - 2023) == (RealGold / 3) - 2023) {
		CheckCharForm();
		sinChar->Money = InvenGold - 2023;
		ReformCharForm();

	}
	else {
		SendSetHackUser(6);

	}
	InvenGold = 0;
	RealGold = 0;

	cInvenTory.SetItemToChar();
	cInvenTory.ReFormInvenItem();
	cInvenTory.CheckWeight();

	return TRUE;
}

void Caravana::showCaravan(smCHAR* Player)
{

}