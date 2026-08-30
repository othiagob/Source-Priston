#include "RestaureWindow.h"
#include "..\ConfirmationBox.h"
#include "..\\sinbaram\\sinLinkHeader.h"

#include "imGui/imgui.h"

FailedItemsPckg failedItems;

extern int smScreenWidth;
extern int smScreenHeight;
static int selected = 1;

INT RestaureWindow::getFailedItems()
{
	smTRANS_COMMAND getFailedItems;
	getFailedItems.code = GET_FAILED_ITEMS;
	getFailedItems.size = sizeof(smTRANS_COMMAND);
	getFailedItems.WParam = 0;
	getFailedItems.LParam = 0;
	getFailedItems.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&getFailedItems, getFailedItems.size, TRUE);

	return TRUE;
}


INT RestaureWindow::ReceiveFailedItems(FailedItemsPckg* FailedItems)
{
	ZeroMemory(&failedItems, sizeof(FailedItemsPckg));
	CopyMemory(&failedItems, FailedItems, sizeof(FailedItemsPckg));

	return TRUE;
}

bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	// Load texture from disk
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(GRAPHICDEVICE, filename, &texture);
	if (hr != S_OK)
		return false;

	// Retrieve description of the texture surface so we can access its size
	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;

}

extern int dsDrawTexImage(int Mat, int x, int y, int w, int h, DWORD Transp, int specular);

void RestaureWindow::OpenNpc(bool* p_open)
{
	ImGui::SetNextWindowSize(ImVec2(400, 350), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 250, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.80f);

	ImGui::StyleColorArmageddon();

	int WinX = (smScreenWidth / 2) - 200;
	int WinY = (smScreenHeight / 2) - 200;
	int SlotY = 0;

	PDIRECT3DTEXTURE9 my_texture = NULL;
	LPDIRECT3DTEXTURE9 Image;
	sITEM* Item = { 0 };

	if (ImGui::Begin("Restaure de Itens Perdidos", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		for (int x = 0; x <= 20; x++)
		{
			char itemName[32] = { 0 };
			char itemPath[64] = { 0 };

			if (failedItems.FailedItems[x].ItemHead)
			{
				Item = FindItemFromName2(failedItems.FailedItems[x].ItemName);
				
				if (Item)
				{
					sprintf_s(itemPath, sizeof(itemPath), "Image\\sinImage\\Items\\%s\\it%s.bmp", Item->ItemFilePath, Item->LastCategory);

					Image = LoadDibSurfaceOffscreen(itemPath);
					//DrawSprite(WinX + 22, WinY + 43 + SlotY, Image, 0, 0, 100, 100);
					//DrawSprite(100, 200, Image, 0, 0, 200, 200);

					int w = 0;
					int h = 0;
					bool ret = LoadTextureFromFile2(itemPath, &my_texture, &w, &h);
					IM_ASSERT(ret);
					ImGui::Image((void*)my_texture, ImVec2(40, 40));
				}

				ImGui::SameLine();

				char msg[20] = { 0 };
				sprintf_s(msg, sizeof(msg), "Aging +%d", failedItems.FailedItems[x].AgingNum);
				ImGui::Text(msg);

				ImGui::SameLine();

				char date[64] = { 0 };
				sprintf_s(date, sizeof(date), "Data: %02d/%02d/%02d", failedItems.FailedItems[x].Date.wDay, failedItems.FailedItems[x].Date.wMonth, failedItems.FailedItems[x].Date.wYear);
				ImGui::Text(date);

				ImGui::SameLine();
				ImGui::Text("Item: ");

				sprintf_s(itemName, sizeof(itemName), "%s", failedItems.FailedItems[x].ItemName);
				SlotY += 32;
			}

			ImGui::SameLine();		
		}
	}
}
