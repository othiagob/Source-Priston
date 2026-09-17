#pragma once

#include <cstring>

inline DWORD WareHouseBufChkSum(const BYTE* p, int n)
{
	DWORD s = 0;
	for (int i = 0; i < n; i++)
		s += (DWORD)((unsigned char)p[i]) * (DWORD)(i + 1);
	return s;
}

inline int WareHouseSkipUnique(DWORD code)
{
	if ((code & sinITEM_MASK1) == (sinPM1 & sinITEM_MASK1))
		return 1;
	if (code == (sinGG1 | sin01))
		return 1;
	return 0;
}

inline int WareHouseCollectWire(const sITEM* items, sWAREHOUSE_WIRE_ITEM* out, int maxOut)
{
	int n = 0;
	if (!items || !out)
		return 0;
	for (int i = 0; i < WAREHOUSE_PAGE_SLOTS && n < maxOut; i++)
	{
		if (!items[i].Flag)
			continue;
		out[n].Slot = (WORD)i;
		out[n].x = items[i].x;
		out[n].y = items[i].y;
		out[n].w = items[i].w;
		out[n].h = items[i].h;
		out[n].Class = items[i].Class;
		out[n].sItemInfo = items[i].sItemInfo;
		n++;
	}
	return n;
}

inline void WareHouseApplyWireItem(sITEM* destPage, const sWAREHOUSE_WIRE_ITEM* src)
{
	if (!destPage || !src || src->Slot >= (WORD)WAREHOUSE_PAGE_SLOTS)
		return;
	sITEM* d = &destPage[src->Slot];
	ZeroMemory(d, sizeof(sITEM));
	d->Flag = 1;
	d->x = src->x;
	d->y = src->y;
	d->SetX = src->x;
	d->SetY = src->y;
	d->w = src->w;
	d->h = src->h;
	d->Class = src->Class;
	d->CODE = src->sItemInfo.CODE;
	d->sItemInfo = src->sItemInfo;
}

inline int WareHouseWirePayloadSize(int itemCount)
{
	if (itemCount < 0)
		itemCount = 0;
	return (int)sizeof(sWAREHOUSE_WIRE_HDR) + itemCount * (int)sizeof(sWAREHOUSE_WIRE_ITEM);
}

inline int WareHouseFillPayload(BYTE* dest, int destMax, const sWAREHOUSE_WIRE_HDR* hdr, const sWAREHOUSE_WIRE_ITEM* items)
{
	if (!dest || !hdr || hdr->itemCount < 0)
		return 0;
	const int need = WareHouseWirePayloadSize(hdr->itemCount);
	if (need > destMax)
		return 0;
	memcpy(dest, hdr, sizeof(sWAREHOUSE_WIRE_HDR));
	if (hdr->itemCount > 0 && items)
		memcpy(dest + sizeof(sWAREHOUSE_WIRE_HDR), items, hdr->itemCount * sizeof(sWAREHOUSE_WIRE_ITEM));
	return need;
}

inline int WareHouseReadPayload(const BYTE* src, int srcSize, sWAREHOUSE_WIRE_HDR* hdr, sWAREHOUSE_WIRE_ITEM* items, int maxItems)
{
	if (!src || !hdr || srcSize < (int)sizeof(sWAREHOUSE_WIRE_HDR))
		return 0;
	memcpy(hdr, src, sizeof(sWAREHOUSE_WIRE_HDR));
	if (hdr->itemCount < 0 || hdr->itemCount > WAREHOUSE_PAGE_SLOTS)
		return 0;
	const int need = WareHouseWirePayloadSize(hdr->itemCount);
	if (need > srcSize)
		return 0;
	if (hdr->itemCount > 0)
	{
		if (!items || hdr->itemCount > maxItems)
			return 0;
		memcpy(items, src + sizeof(sWAREHOUSE_WIRE_HDR), hdr->itemCount * sizeof(sWAREHOUSE_WIRE_ITEM));
	}
	return need;
}

inline int WareHouseItemsOverlap(const sWAREHOUSE_WIRE_ITEM* a, const sWAREHOUSE_WIRE_ITEM* b)
{
	if (!a || !b)
		return 0;
	if (a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h && a->y + a->h > b->y)
		return 1;
	return 0;
}

inline int WareHouseValidatePageItems(const sWAREHOUSE_WIRE_ITEM* items, int count)
{
	if (count < 0 || count > WAREHOUSE_PAGE_SLOTS)
		return 0;
	BYTE used[WAREHOUSE_PAGE_SLOTS];
	ZeroMemory(used, sizeof(used));
	const int maxW = ITEMSIZE * WAREHOUSE_GRID_COLS;
	const int maxH = ITEMSIZE * WAREHOUSE_GRID_ROWS;
	for (int i = 0; i < count; i++)
	{
		const int slot = items[i].Slot;
		if (slot < 0 || slot >= WAREHOUSE_PAGE_SLOTS)
			return 0;
		if (used[slot])
			return 0;
		used[slot] = 1;
		if (items[i].w < ITEMSIZE || items[i].h < ITEMSIZE)
			return 0;
		if (items[i].w > maxW || items[i].h > maxH)
			return 0;
		for (int j = 0; j < i; j++)
		{
			if (WareHouseItemsOverlap(&items[i], &items[j]))
				return 0;
		}
	}
	return 1;
}

struct sWAREHOUSE_SAVE_ITEM {
	BYTE Page;
	WORD Slot;
	int x, y, w, h;
	DWORD Class;
	sITEMINFO Info;
};
