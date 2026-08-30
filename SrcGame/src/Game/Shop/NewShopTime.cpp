#include "NewShopTime.h"
#include "utils\strings.h"
#include "..\\HUD\\RestaureWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "imGui/imgui_impl_dx9.h"
#include "../smLib3d/smDsx.h"
#include "..\\ConfirmationBox.h"

#include <boost\\algorithm\\string\\predicate.hpp>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <zlib.h>

#define CURL_STATICLIB
#include <curl/curl.h>
#include <TitleBox/TitleBox.h>

struct Categorias {
	std::string CategoryName;
	short CategoryID;
};

Categorias sCategoriasTime;
std::vector<Categorias> vCategoriasTime;
ItemsByCategoryTime Items[300] = { 0 };

bool initTime = true;
bool finishTime = false;

static void addCategorias()
{
	vCategoriasTime.clear();

	//sCategoriasTime.CategoryID = 1;
	//sCategoriasTime.CategoryName = "Ataque";

	//vCategoriasTime.push_back(sCategoriasTime);

	//sCategoriasTime.CategoryID = 2;
	//sCategoriasTime.CategoryName = "Defesa";

	//vCategoriasTime.push_back(sCategoriasTime);

	sCategoriasTime.CategoryID = 3;
	sCategoriasTime.CategoryName = u8"Acessórios";

	vCategoriasTime.push_back(sCategoriasTime);

	sCategoriasTime.CategoryID = 4;
	sCategoriasTime.CategoryName = "Trajes";

	vCategoriasTime.push_back(sCategoriasTime);

	sCategoriasTime.CategoryID = 5;
	sCategoriasTime.CategoryName = "Premium";

	vCategoriasTime.push_back(sCategoriasTime);

	//sCategoriasTime.CategoryID = 6;
	//sCategoriasTime.CategoryName = u8"Serviços";

	//vCategoriasTime.push_back(sCategoriasTime);
}

PDIRECT3DTEXTURE9 my_texture4Time[4];
PDIRECT3DTEXTURE9 ClassImagesTime[12];
LPDIRECT3DTEXTURE9 ItemShopImageTime[150];
LPDIRECT3DTEXTURE9 ShopImagesTime[1];
LPDIRECT3DTEXTURE9 RestaureImageTime[20];
LPDIRECT3DTEXTURE9 DonateImagesTime[6];

extern LPDIRECT3DTEXTURE9 LoadDibSurfaceOffscreen(char* Filename);
extern bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
extern sITEM TempPerfectItem;
extern BOOL bIsPerfect;
extern int sinShowItemInfoFlag;
extern char* WChar_to_UTF82(const wchar_t* string);

std::vector<string> ItemsAlreadySentTime;
std::vector<int> ItemsAlreadySentRestaureTime;
std::vector<sITEM> ItemsDetailsTime;
std::vector<sITEM> ItemsDetailsRestaureTime;
bool isShowingDetailsTime = false;
int LastSelectedClassTime = 0;
int selectedClassTime = 0;

int SendItemToServerTime(char itemCode[32], int itemHead = 0, int ItemChkSum = 0, bool isRestaure = false, int AgingNum = 0)
{
	if (!isRestaure)
	{
		auto searchItem = find(ItemsAlreadySentTime.begin(), ItemsAlreadySentTime.end(), itemCode);

		if (searchItem == ItemsAlreadySentTime.end())
		{
			struct Pck
			{
				int size;
				int code;
				int spec;
				bool shopTime;
				char itemID[32];
			};

			Pck smPacket;

			ZeroMemory(&smPacket, sizeof(Pck));
			smPacket.code = 0x43550002;		
			smPacket.shopTime = true;	
			smPacket.spec = selectedClassTime;
			sprintf_s(smPacket.itemID, sizeof(smPacket.itemID), "%s", itemCode);
			smPacket.size = sizeof(Pck);

			smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);
			ItemsAlreadySentTime.push_back(itemCode);

			return true;
		}
	}
	else
	{
		auto searchItem = find(ItemsAlreadySentRestaureTime.begin(), ItemsAlreadySentRestaureTime.end(), itemHead);

		if (searchItem == ItemsAlreadySentRestaureTime.end())
		{
			struct Pck
			{
				int size;
				int code;
				int ItemHead;
				int ItemChkSum;
				int AgingNum;
				char itemCode[32];
			};

			Pck smPacket;

			ZeroMemory(&smPacket, sizeof(Pck));
			smPacket.code = 0x43550003;
			smPacket.ItemHead = itemHead;
			smPacket.ItemChkSum = ItemChkSum;
			smPacket.AgingNum = AgingNum;
			sprintf_s(smPacket.itemCode, sizeof(smPacket.itemCode), "%s", itemCode);
			smPacket.size = sizeof(Pck);

			smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);

			ItemsAlreadySentRestaureTime.push_back(itemHead);

			return true;
		}
	}

	if (!isRestaure)
	{
		for (auto& getItemToShow : ItemsDetailsTime)
		{
			if (boost::iequals(getItemToShow.DorpItem, itemCode))
			{

				if (LastSelectedClassTime != selectedClassTime)
				{
					ItemsAlreadySentTime.clear();
					ItemsDetailsTime.clear();
				}
				else
				{
					TempPerfectItem = getItemToShow;
					bIsPerfect = true;
				}

				break;
			}
		}
	}
	else
	{
		for (auto& getItemToShow : ItemsDetailsRestaureTime)
		{
			if (getItemToShow.sItemInfo.ItemHeader.Head == itemHead)
			{
				TempPerfectItem = getItemToShow;
				bIsPerfect = true;
				break;
			}
		}
	}

	LastSelectedClassTime = selectedClassTime;
}

int wdTime = 0;
int hdTime = 0;

bool LoadTextureFromFile5(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
{
	PDIRECT3DTEXTURE9 texture;
	HRESULT hr = D3DXCreateTextureFromFileA(GRAPHICDEVICE, filename, &texture);
	if (hr != S_OK)
		return false;

	D3DSURFACE_DESC my_image_desc;
	texture->GetLevelDesc(0, &my_image_desc);
	*out_texture = texture;
	*out_width = (int)my_image_desc.Width;
	*out_height = (int)my_image_desc.Height;
	return true;

}

const char* ClassesTime[] = { "Sem Classe", "Lutador", u8"Mecânico", "Arqueira", "Pikeman", "Atalanta", "Cavaleiro", "Mago", "Sacerdotisa" };

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}


void SentPurchaseToServer(ItemsByCategoryTime Item, int Spec)
{
	for (auto& getItemToShow : ItemsDetailsTime)
	{
		if (boost::iequals(getItemToShow.DorpItem, Item.ItemCode))
		{
			if (!cInvenTory.CheckSetEmptyArea(&getItemToShow))
			{
				TitleBox::GetInstance()->SetText("Você não tem espaço suficiente no inventário!", 3);
				return;
			}

			if (!cInvenTory.CheckSetOk(&getItemToShow, 1))
			{
				TitleBox::GetInstance()->SetText("Excede o peso limite.", 3);
				return;
			}
		}
	}

	sFinishPurchaseTime sFinal;

	sFinal.Item = Item;
	sFinal.Quantity = 1;
	sFinal.Spec = Spec;

	sFinal.code = NewShopItems_FinishPurchase_Time;
	sFinal.size = sizeof(sFinishPurchaseTime);

	smWsockDataServer->Send((char*)&sFinal, sFinal.size, TRUE);
}

void FinishPurchase(ItemsByCategoryTime Item)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Confirmar Compra?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finishTime = true;

		ImGui::SetCursorPosX(320);

		if (TempPerfectItem.sItemInfo.JobCodeMask == 0)
			ImGui::Image(ClassImagesTime[0], ImVec2(40, 40));
		else
			ImGui::Image(ClassImagesTime[selectedClassTime], ImVec2(40, 40));

		ImGui::SetCursorPosY(25);
		ImGui::Text("Item:");
		ImGui::SameLine();
		char msg[64] = { 0 };
		sprintf_s(msg, sizeof(msg), u8"Item: %s", Item.ItemName);
		CA2W pszWide2(msg);
		wsprintf(msg, "%s", WChar_to_UTF82(pszWide2));
		ImGui::Text(msg);

		char classe[64] = { 0 };
		if (TempPerfectItem.sItemInfo.JobCodeMask == 0)
			sprintf_s(classe, sizeof(classe), "Classe: NS");
		else
			sprintf_s(classe, sizeof(classe), "Classe: %s", ClassesTime[selectedClassTime]);
		ImGui::Text(classe);

		char quantity[64] = { 0 };
		sprintf_s(quantity, sizeof(quantity), "Quantidade: %d", 1);
		ImGui::Text(quantity);

		char price[64] = { 0 };
		sprintf_s(price, sizeof(price), "Total: %d Time Coins", Item.Price);
		ImGui::Text(price);

		ImGui::Separator();
		ImGui::Text(u8"Aviso: Itens sem SPEC suportado serão entregues NS!!!");
		ImGui::Separator();

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  finishTime = false; SentPurchaseToServer(Item, selectedClassTime); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finishTime = false; }
		ImGui::EndPopup();
	}
}

void BuyVIPTime()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Deseja comprar o VIP?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finishTime = true;

		ImGui::SetCursorPosX(320);
		ImGui::Image(my_texture4Time[2], ImVec2(48, 48));

		ImGui::SetCursorPosY(25);

		char msg[64] = { 0 };
		sprintf_s(msg, sizeof(msg), u8"Seu VIP será ativado imediatamente");
		ImGui::Text(msg);

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			finishTime = false;

			ItemsByCategoryTime Vip;

			Vip.CategoryID = 6;
			Vip.SubCategoryID = 100;
			sprintf_s(Vip.ItemName, sizeof(Vip.ItemName), "VIP 30 Dias");
			sprintf_s(Vip.ItemCode, sizeof(Vip.ItemCode), "VP101");
			Vip.Price = 5000;
			Vip.Discount = 0;

			SentPurchaseToServer(Vip, 0);
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finishTime = false; }
		ImGui::EndPopup();
	}
}

void ShowDetailsTime(std::string sItemCode)
{
	if (!bIsPerfect && !isShowingDetailsTime)
	{
		char itemCode[32] = { 0 };
		sprintf_s(itemCode, sizeof(itemCode), "%s", sItemCode.c_str());
		SendItemToServerTime(itemCode);
		isShowingDetailsTime = true;
	}
}

void initShopTime()
{
	ZeroMemory(ShopImagesTime, sizeof(ShopImagesTime));
	ZeroMemory(ClassImagesTime, sizeof(ClassImagesTime));
	ZeroMemory(my_texture4Time, sizeof(my_texture4Time));
	ZeroMemory(ItemShopImageTime, sizeof(ItemShopImageTime));
	ZeroMemory(DonateImagesTime, sizeof(DonateImagesTime));
	ZeroMemory(RestaureImageTime, sizeof(RestaureImageTime));

	ItemsAlreadySentTime.clear();
	ItemsAlreadySentRestaureTime.clear();
	ItemsDetailsTime.clear();
	ItemsDetailsRestaureTime.clear();

	addCategorias();

	char Path[64] = { 0 };
	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\coin.png");
	ShopImagesTime[0] = LoadDibSurfaceOffscreen(Path);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\buyItem.png");
	bool ret = LoadTextureFromFile5(Path, &my_texture4Time[0], &wdTime, &hdTime);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Restaure.png");
	ret = LoadTextureFromFile5(Path, &my_texture4Time[1], &wdTime, &hdTime);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Vip.png");
	ret = LoadTextureFromFile5(Path, &my_texture4Time[2], &wdTime, &hdTime);
	IM_ASSERT(ret);

	int y = 0;
	int pos = 0;

	for (int x = 0; x <= 10; x++)
	{
		sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Classes\\%d.png", x);
		bool ret = LoadTextureFromFile5(Path, &ClassImagesTime[x], &wdTime, &hdTime);
		IM_ASSERT(ret);
	}

	for (auto& Items : NewShopTime::GetInstance()->ShopItems)
	{
		Items.imgPosition = y;

		switch (Items.CategoryID)
		{
		case 1:
			sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", Items.ItemCode);
			ItemShopImageTime[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;

		case 2:
			sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", Items.ItemCode);
			ItemShopImageTime[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		case 4:
			if (Items.CategoryID == 2 && Items.SubCategoryID == 4)
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", Items.ItemCode);
			else
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Defense\\it%s.bmp", Items.ItemCode);
			ItemShopImageTime[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		case 3:
			if (Items.SubCategoryID == 2 || Items.SubCategoryID == 3)
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Defense\\it%s.bmp", Items.ItemCode);
			else
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", Items.ItemCode);
			ItemShopImageTime[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		case 5:
			if ((Items.ItemCode[0] == 'G' && Items.ItemCode[1] == 'P') || (Items.ItemCode[0] == 'g' && Items.ItemCode[1] == 'p')  || (Items.ItemCode[0] == 'S' && Items.ItemCode[1] == 'P'))
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Event\\it%s.bmp", Items.ItemCode);
			else if (Items.SubCategoryID == 3 || (Items.ItemCode[0] == 'S' && Items.ItemCode[1] == 'E'))
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", Items.ItemCode);
			else
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", Items.ItemCode);
			ItemShopImageTime[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		}
	}

	selectedClassTime = lpCurPlayer->smCharInfo.JOB_CODE;

	initTime = false;
}

void selectSpecTime()
{
	const char* combo_label = ClassesTime[selectedClassTime];

	if (ImGui::BeginCombo("Selecionar Classe", combo_label, ImGuiComboFlags_NoArrowButton | ImGuiWindowFlags_NoSavedSettings))
	{
		for (int n = 0; n < IM_ARRAYSIZE(ClassesTime); n++)
		{
			const bool is_selected = (selectedClassTime == n);
			if (ImGui::Selectable(ClassesTime[n], is_selected))
			{
				selectedClassTime = n;
				ItemsAlreadySentTime.clear();
				ItemsDetailsTime.clear();
				SendItemToServerTime(TempPerfectItem.DorpItem);
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndCombo();
	}
}

int posInicialTime = 30;

void ShowSubCategories(ItemsByCategoryTime SubCategories)
{
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::PushID(SubCategories.imgPosition);

	// Áre de seleção de compra
	ImGui::Selectable("##BuyItem", false, 0, ImVec2(300, 90));

	if (ImGui::IsItemHovered() && !finishTime && SubCategories.CategoryID != 5)
		ShowDetailsTime(SubCategories.ItemCode);
	else
		isShowingDetailsTime = false;

	ImGui::SetCursorPosX(310);
	ImGui::SetCursorPosY(posInicialTime - 4);

	if (ImGui::ImageButton(my_texture4Time[0], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
		ImGui::OpenPopup("Confirmar Compra?");

	FinishPurchase(SubCategories);

	ImGui::SetCursorPosX(310);
	ImGui::SetCursorPosY(posInicialTime + 60);
	selectSpecTime();

	ImGui::SetCursorPosY(posInicialTime);
	ImGui::SetCursorPosX(10);

	if (SubCategories.CategoryID == 3 || SubCategories.CategoryID == 5) {
		ImGui::SetCursorPosY(posInicialTime + 12);
		ImGui::SetCursorPosX(22);
		ImGui::Image(ItemShopImageTime[SubCategories.imgPosition], ImVec2(22, 22));
	}
	else if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 4)
		ImGui::Image(ItemShopImageTime[SubCategories.imgPosition], ImVec2(44, 44));
	else if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 5)
		ImGui::Image(ItemShopImageTime[SubCategories.imgPosition], ImVec2(22, 22));
	else if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 4)
		ImGui::Image(ItemShopImageTime[SubCategories.imgPosition], ImVec2(44, 44));
	else
		ImGui::Image(ItemShopImageTime[SubCategories.imgPosition], ImVec2(44, 44));

	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime); posInicialTime += 20;
	ImGui::Text("Nome: ");
	ImGui::SameLine();

	char itemName[64] = { 0 };
	sprintf_s(itemName, sizeof(itemName), "%s", SubCategories.ItemName);
	CA2W pszWide2(itemName);
	wsprintf(itemName, "%s", WChar_to_UTF82(pszWide2));
	ImGui::Text(itemName);

	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime); posInicialTime += 10;
	char price[32] = { 0 };
	sprintf_s(price, sizeof(price), "%d Time Coins", SubCategories.Price);
	ImGui::SetCursorPosY(posInicialTime); posInicialTime += 60;
	ImGui::Text(u8"Preço: ");
	ImGui::SameLine();
	ImGui::Text(price);
	ImGui::SetCursorPosY(posInicialTime); posInicialTime += 20;
	ImGui::Separator();
	ImGui::PopID();
}

void NewShopTime::LoadVipOptions()
{
	posInicialTime = 30;

	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Selectable("##VIP", false, 0, ImVec2(380, 120));

	ImGui::SetCursorPosX(390);
	ImGui::SetCursorPosY(posInicialTime);

	if (ImGui::ImageButton(my_texture4Time[0], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
		ImGui::OpenPopup("Deseja comprar o VIP?");

	BuyVIPTime();

	ImGui::SetCursorPosY(65);
	ImGui::SetCursorPosX(10);
	ImGui::Image(my_texture4Time[2], ImVec2(48, 48));

	char msg[64] = { 0 };
	sprintf_s(msg, sizeof(msg), u8"VIP: Válido por 30 dias");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Text(msg);

	posInicialTime += 20;

	sprintf_s(msg, sizeof(msg), u8"Vantagens que o VIP oferece:");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Text(msg);

	posInicialTime += 20;

	sprintf_s(msg, sizeof(msg), u8"Comando /repot (Abre a Loja da Allya)");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Text(msg);

	posInicialTime += 20;

	sprintf_s(msg, sizeof(msg), u8"Comando /premium (Abre o Negociante Mughy)");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Text(msg);

	posInicialTime += 20;

	sprintf_s(msg, sizeof(msg), u8"%d %s de Exp. Add. dos Monstros", 10, "%%");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Text(msg);

	posInicialTime += 20;

	sprintf_s(msg, sizeof(msg), u8"O valor: %d Time Coins", 5000);
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicialTime);
	ImGui::Text(msg);
}

void NewShopTime::RestaureItems()
{
}

void NewShopTime::ReceiveItems(NewShopTime_COMPRESSEDPCKG* Data)
{
	ITEMS_INFOCKGTIME getAllItems;
	ZeroMemory(&getAllItems, sizeof(ITEMS_INFOCKGTIME));

	unsigned long nCompressedDataSize = Data->CompressedDataPckg.compressedSize;
	unsigned long UnCompressedDataSize = 30000;

	uncompress((Bytef*)&getAllItems.Items, &UnCompressedDataSize, (Bytef*)Data->CompressedDataPckg.pCompressedData, nCompressedDataSize);

	ShopItems.clear();

	int i = 0;
	while (true)
	{
		if (getAllItems.Items[i].CategoryID == 0) break;

		ShopItems.push_back(getAllItems.Items[i]);
		i++;
	}

	initTime = true;
	openFlag = true;
}

void NewShopTime::ReceiveTime(sCoinPlayerTime* pTime)
{
	Time = pTime->Coin;
}

void NewShopTime::OpenNpc(bool* p_open)
{
	if (initTime)
		initShopTime();

	ImGui::SetNextWindowSize(ImVec2(600, 390), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 280, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.90f);

	ImGui::StyleColorArmageddon();

	if (ImGui::Begin("Loja de Tempo", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		// Left
		static int selected = 0;
		static int selectedItem = 0;

		{
			ImGui::BeginChild("Categorias", ImVec2(150, 0), true);

			for (auto Categorias : vCategoriasTime)
			{
				char label[128];
				sprintf(label, "%s", Categorias.CategoryName.c_str());

				if (ImGui::Button(label, ImVec2(140, 40)))
				{
					selected = Categorias.CategoryID;
				}
			}

			ImGui::SetCursorPosY(300);
			ImGui::Image(ShopImagesTime[0], ImVec2(50, 50));
			char coins[32] = { 0 };
			sprintf_s(coins, sizeof(coins), "%d TC", Time);
			ImGui::SetCursorPosY(317);
			ImGui::SetCursorPosX(60);
			ImGui::Text(coins);


			ImGui::EndChild();
		}

		if (bIsPerfect && !finishTime) // Mostra os detalhes do item
		{
			if (smScreenWidth > 800 && smScreenHeight > 600) // 800x600 não suporta o preview do item
			{
				sinShowItemInfoFlag = 1;
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&TempPerfectItem, FALSE, FALSE, FALSE);
			}
		}

		if (!isShowingDetailsTime)
			bIsPerfect = false;

		if (selected > 0)
		{
			ImGui::SameLine();
			ImGui::BeginGroup();
			ImGui::BeginChild("Itens da Categoria", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
			ImGui::Separator();
		}

		// Categorias do shop
		switch (selected) // subcategorias
		{
		//case 1:
		//	// Armas
		//	if (ImGui::BeginTabBar("##Ataque", ImGuiTabBarFlags_None))
		//	{
		//		posInicialTime = 30;

		//		for (auto& SubCategories : ShopItems)
		//		{
		//			if (ImGui::BeginTabItem("Arcos"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 1)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}

		//			if (ImGui::BeginTabItem("Espadas"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 2)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem("Foices"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 3)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem("Garras"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 4)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem(u8"Lanças"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 5)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem("Machados"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 6)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem("Martelos"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 7)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem("Varinhas"))
		//			{
		//				if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 8)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}

		//		}


		//		ImGui::EndTabBar();
		//	}
		//	ImGui::EndChild();
		//	ImGui::EndGroup();
		//	break;

		//case 2:
		//	// Armaduras
		//	if (ImGui::BeginTabBar("##Defesa", ImGuiTabBarFlags_None))
		//	{
		//		posInicialTime = 30;

		//		for (auto& SubCategories : ShopItems)
		//		{
		//			if (ImGui::BeginTabItem("Armaduras"))
		//			{
		//				if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 1)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}

		//			if (ImGui::BeginTabItem(u8"Roupões"))
		//			{
		//				if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 2)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem(u8"Escudos"))
		//			{
		//				if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 3)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//			if (ImGui::BeginTabItem(u8"Orbitais"))
		//			{
		//				if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 4)
		//				{
		//					ShowSubCategories(SubCategories);
		//				}

		//				ImGui::EndTabItem();
		//			}
		//		}

		//		ImGui::EndTabBar();
		//	}
		//	ImGui::EndChild();
		//	ImGui::EndGroup();
		//	break;
		case 3:
			// Acessórios
			if (ImGui::BeginTabBar("##Acessórios", ImGuiTabBarFlags_None))
			{
				posInicialTime = 30;

				for (auto& SubCategories : ShopItems)
				{
					/*if (ImGui::BeginTabItem("Braceletes"))
					{
						if (SubCategories.CategoryID == 3 && SubCategories.SubCategoryID == 1)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Botas"))
					{
						if (SubCategories.CategoryID == 3 && SubCategories.SubCategoryID == 2)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Luvas"))
					{
						if (SubCategories.CategoryID == 3 && SubCategories.SubCategoryID == 3)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Anéis"))
					{
						if (SubCategories.CategoryID == 3 && SubCategories.SubCategoryID == 4)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Colares"))
					{
						if (SubCategories.CategoryID == 3 && SubCategories.SubCategoryID == 5)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}*/
					if (ImGui::BeginTabItem("Pedras"))
					{
						if (SubCategories.CategoryID == 3 && SubCategories.SubCategoryID == 6)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();
			break;
		case 4:
			// Trajes
			if (ImGui::BeginTabBar("##Trajes", ImGuiTabBarFlags_None))
			{
				posInicialTime = 30;

				for (auto& SubCategories : ShopItems)
				{
					if (ImGui::BeginTabItem("Masculinos"))
					{
						if (SubCategories.CategoryID == 4 && SubCategories.SubCategoryID == 1)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Femininos"))
					{
						if (SubCategories.CategoryID == 4 && SubCategories.SubCategoryID == 2)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					/*if (ImGui::BeginTabItem(u8"Roupões Masculinos"))
					{
						if (SubCategories.CategoryID == 4 && SubCategories.SubCategoryID == 3)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Roupões Femininos"))
					{
						if (SubCategories.CategoryID == 4 && SubCategories.SubCategoryID == 4)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}*/
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();
			break;
		case 5:
			// Premiuns
			if (ImGui::BeginTabBar("##Premium", ImGuiTabBarFlags_None))
			{
				posInicialTime = 30;

				for (auto& SubCategories : ShopItems)
				{
					if (ImGui::BeginTabItem("Aprimoramento"))
					{
						if (SubCategories.CategoryID == 5 && SubCategories.SubCategoryID == 1)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Aging e Mix"))
					{
						if (SubCategories.CategoryID == 5 && SubCategories.SubCategoryID == 2)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Forças"))
					{
						if (SubCategories.CategoryID == 5 && SubCategories.SubCategoryID == 3)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Utilitários"))
					{
						if (SubCategories.CategoryID == 5 && SubCategories.SubCategoryID == 4)
						{
							ShowSubCategories(SubCategories);
						}
						ImGui::EndTabItem();
					}
				}
				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();
			break;
		//case 6:
		//	// Serviços
		//	if (ImGui::BeginTabBar("##Serviços", ImGuiTabBarFlags_None))
		//	{
		//		posInicialTime = 30;

		//		if (ImGui::BeginTabItem("VIP"))
		//		{
		//			LoadVipOptions();
		//			ImGui::EndTabItem();
		//		}

		//		ImGui::EndTabBar();
		//	}
		//	ImGui::EndChild();
		//	ImGui::EndGroup();
		//	break;
		}
	}
	ImGui::End();
}
