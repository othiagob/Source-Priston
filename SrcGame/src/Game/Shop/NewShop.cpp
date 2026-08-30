#include "NewShop.h"
#include "utils\strings.h"
#include <atlconv.h>
#include "..\\HUD\\RestaureWindow.h"
#include "..\\sinbaram\\sinLinkHeader.h"
#include "imGui/imgui_impl_dx9.h"
#include "../smLib3d/smDsx.h"
#include "..\\ConfirmationBox.h"

#include <boost\\algorithm\\string\\predicate.hpp>
#include <d3d9.h>
#include <d3dx9tex.h>
#include <zlib.h>

#include <curl/curl.h>
#include <TitleBox/TitleBox.h>

#ifdef _DEBUG
#pragma comment (lib, "crypt32")
#pragma comment (lib, "ws2_32")
#pragma comment (lib, "Normaliz")
#pragma comment (lib, "Wldap32")
#pragma comment (lib, "Game/API/libcurl_a_debug.lib")
#endif


struct Categorias {
	std::string CategoryName;
	short CategoryID;
};

Categorias sCategorias;
std::vector<Categorias> vCategorias;
ItemsByCategory Items[300] = { 0 };

bool init = true;
bool finish = false;

// Troca de nick
int nickIsAvailable = 0;
char NewNick[30] = { 0 };

static void addCategorias()
{
	vCategorias.clear();

	sCategorias.CategoryID = 1;
	sCategorias.CategoryName = "Ataque";

	vCategorias.push_back(sCategorias);

	sCategorias.CategoryID = 2;
	sCategorias.CategoryName = "Defesa";

	vCategorias.push_back(sCategorias);

	sCategorias.CategoryID = 3;
	sCategorias.CategoryName = u8"Acessórios";

	vCategorias.push_back(sCategorias);

	sCategorias.CategoryID = 4;
	sCategorias.CategoryName = "Trajes";

	vCategorias.push_back(sCategorias);

	sCategorias.CategoryID = 5;
	sCategorias.CategoryName = "Premium";

	vCategorias.push_back(sCategorias);

	sCategorias.CategoryID = 6;
	sCategorias.CategoryName = u8"Serviços";

	vCategorias.push_back(sCategorias);
}

PDIRECT3DTEXTURE9 my_texture4[4];
PDIRECT3DTEXTURE9 ClassImages[12];
LPDIRECT3DTEXTURE9 ItemShopImage[150];
LPDIRECT3DTEXTURE9 ShopImages[1];
LPDIRECT3DTEXTURE9 RestaureImage[20];
LPDIRECT3DTEXTURE9 DonateImages[6];

extern BOOL DecryptBMP(char* pBuffer, unsigned int uBufferSize);
extern LPDIRECT3DTEXTURE9 LoadDibSurfaceOffscreen(char* Filename);
extern bool LoadTextureFromFile2(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height);
extern sITEM TempPerfectItem;
extern BOOL bIsPerfect;
extern sITEM TempPerfectItem;;
extern int sinShowItemInfoFlag;
extern char* WChar_to_UTF82(const wchar_t* string);

extern FailedItemsPckg failedItems;

std::vector<string> ItemsAlreadySent;
std::vector<int> ItemsAlreadySentRestaure;
std::vector<sITEM> ItemsDetails;
std::vector<sITEM> ItemsDetailsRestaure;
bool isShowingDetails = false;
bool isShowingDetailsRestaure = false;
int LastSelectedClass = 0;
int selectedClass = 0;
int selectedChangeClass = 0;

int SendItemToServer(char itemCode[32], int itemHead = 0, int ItemChkSum = 0, bool isRestaure = false, int AgingNum = 0)
{
	if (!isRestaure)
	{
		auto searchItem = find(ItemsAlreadySent.begin(), ItemsAlreadySent.end(), itemCode);

		if (searchItem == ItemsAlreadySent.end())
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
			smPacket.shopTime = false;
			smPacket.spec = selectedClass;
			sprintf_s(smPacket.itemID, sizeof(smPacket.itemID), "%s", itemCode);
			smPacket.size = sizeof(Pck);

			smWsockDataServer->Send((char*)&smPacket, smPacket.size, TRUE);
			ItemsAlreadySent.push_back(itemCode);

			return true;
		}
	}
	else
	{
		auto searchItem = find(ItemsAlreadySentRestaure.begin(), ItemsAlreadySentRestaure.end(), itemHead);

		if (searchItem == ItemsAlreadySentRestaure.end())
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

			ItemsAlreadySentRestaure.push_back(itemHead);

			return true;
		}
	}

	if (!isRestaure)
	{
		for (auto& getItemToShow : ItemsDetails)
		{
			if (boost::iequals(getItemToShow.DorpItem, itemCode))
			{

				if (LastSelectedClass != selectedClass)
				{
					ItemsAlreadySent.clear();
					ItemsDetails.clear();
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
		for (auto& getItemToShow : ItemsDetailsRestaure)
		{
			if (getItemToShow.sItemInfo.ItemHeader.Head == itemHead)
			{
				TempPerfectItem = getItemToShow;
				bIsPerfect = true;
				break;
			}
		}
	}

	LastSelectedClass = selectedClass;
	return TRUE;
}

int wd = 0;
int hd = 0;

bool LoadTextureFromFile4(const char* filename, PDIRECT3DTEXTURE9* out_texture, int* out_width, int* out_height)
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

const char* Classes[] = { "Sem Classe", "Lutador", u8"Mecânico", "Arqueira", "Pikeman", "Atalanta", "Cavaleiro", "Mago", "Sacerdotisa" };

const int DonationValues[] = { 5, 10, 30, 50, 100, 200 };
const int DonationResults[] = { 25, 50, 150, 250, 500, 1000 };

INT RecoverItem(int ItemHead, int ItemChkSum)
{
	smTRANS_COMMAND getFailedItems;
	getFailedItems.code = RECOVER_AGING_ITEM;
	getFailedItems.size = sizeof(smTRANS_COMMAND);
	getFailedItems.WParam = ItemHead;
	getFailedItems.LParam = ItemChkSum;
	getFailedItems.SParam = 0;

	if (smWsockServer)
		smWsockServer->Send((char*)&getFailedItems, getFailedItems.size, TRUE);

	return TRUE;
}

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

void NewShop::Donation(char amount[32]) {

	CURL* curl = curl_easy_init();

	std::string urlDonate;

	if (curl)
	{
		CURLcode res;

		char szstring[128] = { 0 };
		//Arquivo de shop desconhecido - DRACKO
		sprintf_s(szstring, sizeof(szstring), "http://www.dnsprotect.com.br/PristonTale/WDPT/CreateOrder.asp?amount=%s", amount);
		urlDonate.clear();
		curl_easy_setopt(curl, CURLOPT_URL, szstring);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &urlDonate);

		res = curl_easy_perform(curl);

		curl_easy_cleanup(curl);

		std::vector<std::string> vTransaction = split(urlDonate.c_str(), '=');

		sprintf_s(idPaypal, sizeof(idPaypal), "%s", vTransaction[1].c_str());
		sprintf_s(Amount, sizeof(Amount), "%s", amount);
	}

	ShellExecute(NULL, NULL, urlDonate.c_str(), NULL, NULL, NULL);

	//Donate packetDonate;
	//sprintf_s(packetDonate.PckDonate.Amount, sizeof(packetDonate.PckDonate.Amount), "%s", Amount);
	//sprintf_s(packetDonate.PckDonate.IDPaypal, sizeof(packetDonate.PckDonate.IDPaypal), "%s", idPaypal);
	//packetDonate.size = sizeof(NEW_DONATE);
	//packetDonate.code = PACKET_DONATE_PAYPAL;

	//if (smWsockUserServer)
	//	smWsockUserServer->Send((char*)&packetDonate, packetDonate.size, TRUE);
}

void SentPurchaseToServer(ItemsByCategory Item, int Spec)
{
	// Verifica peso e espaço no inventário

	for (auto& getItemToShow : ItemsDetails)
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

	sFinishPurchase sFinal;

	sFinal.Item = Item;
	sFinal.Quantity = 1;
	sFinal.Spec = Spec;

	sFinal.code = NewShopItems_FinishPurchase;
	sFinal.size = sizeof(sFinishPurchase);

	if (smWsockDataServer)
		smWsockDataServer->Send((char*)&sFinal, sFinal.size, TRUE);
}

void SentCheckNickToServer(ItemsByCategory Item, int Spec)
{
	sFinishPurchase sCheckNick;

	sCheckNick.Item = Item;
	sCheckNick.Quantity = 1;
	sCheckNick.Spec = Spec;

	sCheckNick.code = NewShopItems_CheckNick;
	sCheckNick.size = sizeof(sFinishPurchase);

	smWsockDataServer->Send((char*)&sCheckNick, sCheckNick.size, TRUE);
}

void SentChangeNickToServer(ItemsByCategory Item, int Spec)
{
	sFinishPurchase sChangeNick;

	sChangeNick.Item = Item;
	sChangeNick.Quantity = 1;
	sChangeNick.Spec = Spec;

	sChangeNick.code = NewShopItems_ChangeNick;
	sChangeNick.size = sizeof(sFinishPurchase);

	smWsockDataServer->Send((char*)&sChangeNick, sChangeNick.size, TRUE);
}

void SentChangeClassToServer(ItemsByCategory Item, int Spec)
{
	sFinishPurchase sChangeClass;

	sChangeClass.Item = Item;
	sChangeClass.Quantity = 1;
	sChangeClass.Spec = Spec;

	sChangeClass.code = NewShopItems_ChangeClass;
	sChangeClass.size = sizeof(sFinishPurchase);

	smWsockDataServer->Send((char*)&sChangeClass, sChangeClass.size, TRUE);
}

void NewShop::RecvCoin(int coin)
{
	Coin = coin;
}

void NewShop::MinusCoin(int coin)
{
	Coin = Coin - coin;
}

void NewShop::PlusCoin(int coin)
{
	Coin = Coin + coin;
}

static int selectedDonation = 0;

void showDonationOptions()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal(u8"Efetuar uma doação", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		int posInicial = 70;
		ImGui::Text(u8"Selecione uma opção de doação e aguarde");
		ImGui::Text(u8"Você será redirecionado automaticamente");

		for (int x = 0; x < 6; x++)
		{
			ImGui::PushID(x);

			char msg[64] = { 0 };
			sprintf_s(msg, sizeof(msg), u8"Valor da doação: R$ %d,00", DonationValues[x]);
			ImGui::SetCursorPosX(250);
			ImGui::Image(DonateImages[x], ImVec2(40, 40));

			ImGui::SetCursorPosY(posInicial); posInicial += 20;

			// Clicou na doação
			if (ImGui::Selectable(msg, selectedDonation == DonationValues[x], 0, ImVec2(240, 36)))
			{
				char amount[32] = { 0 };
				sprintf_s(amount, sizeof(amount), "%d", DonationValues[x]);
				NewShop::GetInstance()->Donation(amount);
			}

			if (x == 3)
				sprintf_s(msg, sizeof(msg), u8"Valor em Coins: %d + 50 Bônus", DonationResults[x]);
			else if (x == 4)
				sprintf_s(msg, sizeof(msg), u8"Valor em Coins: %d + 75 Bônus", DonationResults[x]);
			else if (x == 5)
				sprintf_s(msg, sizeof(msg), u8"Valor em Coins: %d + 100 Bônus", DonationResults[x]);
			else
				sprintf_s(msg, sizeof(msg), "Valor em Coins: %d", DonationResults[x]);

			ImGui::SetCursorPosY(posInicial);
			ImGui::Text(msg);
			ImGui::Separator();
			ImGui::Text("");

			ImGui::PopID();
			posInicial += 40;
		}

		ImGui::SetCursorPosX(80);
		if (ImGui::Button("Fechar", ImVec2(140, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}
}

void FinishPurchase(ItemsByCategory Item)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Confirmar Compra?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finish = true;

		ImGui::SetCursorPosX(320);

		if (TempPerfectItem.sItemInfo.JobCodeMask == 0)
			ImGui::Image(ClassImages[0], ImVec2(40, 40));
		else
			ImGui::Image(ClassImages[selectedClass], ImVec2(40, 40));

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
			sprintf_s(classe, sizeof(classe), "Classe: %s", Classes[selectedClass]);

		ImGui::Text(classe);

		char quantity[64] = { 0 };
		sprintf_s(quantity, sizeof(quantity), "Quantidade: %d", 1);
		ImGui::Text(quantity);

		char price[64] = { 0 };
		sprintf_s(price, sizeof(price), "Total: %d coins", Item.Price);
		ImGui::Text(price);

		ImGui::Separator();
		ImGui::Text(u8"Aviso: Itens sem SPEC suportado serão entregues NS!!!");
		ImGui::Separator();

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  finish = false; SentPurchaseToServer(Item, selectedClass); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finish = false; }
		ImGui::EndPopup();
	}
}

void BuyVIP()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Deseja comprar o VIP?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finish = true;

		ImGui::SetCursorPosX(320);
		ImGui::Image(my_texture4[2], ImVec2(48, 48));

		ImGui::SetCursorPosY(25);

		char msg[64] = { 0 };
		sprintf_s(msg, sizeof(msg), u8"Seu VIP será ativado imediatamente!");
		ImGui::Text(msg);

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			finish = false;

			ItemsByCategory Vip;

			Vip.CategoryID = 6;
			Vip.SubCategoryID = 100;
			sprintf_s(Vip.ItemName, sizeof(Vip.ItemName), "VIP 30 Dias");
			sprintf_s(Vip.ItemCode, sizeof(Vip.ItemCode), "VP101");
			Vip.Price = 2000;
			Vip.Discount = 0;

			SentPurchaseToServer(Vip, 0);
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finish = false; }
		ImGui::EndPopup();
	}
}

void FinishRestaure(FailedItems Item)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Deseja restaurar o Item?", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finish = true;

		ImGui::SetCursorPosX(320);
		ImGui::Image(RestaureImage[Item.imgPosition], ImVec2(33, 44));

		ImGui::SetCursorPosY(25);

		char msg[64] = { 0 };
		sprintf_s(msg, sizeof(msg), u8"Item: %s", Item.ItemName);
		CA2W pszWide2(msg);
		wsprintf(msg, "%s", WChar_to_UTF82(pszWide2));
		ImGui::Text(msg);

		sprintf_s(msg, sizeof(msg), u8"Nível do aging: %d", Item.AgingNum);
		ImGui::Text(msg);

		sprintf_s(msg, sizeof(msg), u8"Total: %d coins", failedItems.price);
		ImGui::Text(msg);

		ImGui::Separator();

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup();  finish = false; RecoverItem(Item.ItemHead, Item.ItemChkSum); }
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finish = false; }
		ImGui::EndPopup();
	}
}


void ShowDetails(std::string sItemCode)
{
	if (!bIsPerfect && !isShowingDetails)
	{
		char itemCode[32] = { 0 };
		sprintf_s(itemCode, sizeof(itemCode), "%s", sItemCode.c_str());
		SendItemToServer(itemCode);
		isShowingDetails = true;
	}
}

void ShowDetailsRestaure(char itemCode[32], int itemHead, int ItemChkSum, int AgingNum)
{
	if (!bIsPerfect && !isShowingDetailsRestaure)
	{
		SendItemToServer(itemCode, itemHead, ItemChkSum, true, AgingNum);
		isShowingDetailsRestaure = true;
	}
}

void initShop()
{
	ZeroMemory(ShopImages, sizeof(ShopImages));
	ZeroMemory(ClassImages, sizeof(ClassImages));
	ZeroMemory(my_texture4, sizeof(my_texture4));
	ZeroMemory(ItemShopImage, sizeof(ItemShopImage));
	ZeroMemory(DonateImages, sizeof(DonateImages));
	ZeroMemory(RestaureImage, sizeof(RestaureImage));
	ZeroMemory(NewNick, sizeof(NewNick));

	nickIsAvailable = 0;

	ItemsAlreadySent.clear();
	ItemsAlreadySentRestaure.clear();
	ItemsDetails.clear();
	ItemsDetailsRestaure.clear();

	addCategorias();

	char Path[64] = { 0 };
	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\coin.png");
	ShopImages[0] = LoadDibSurfaceOffscreen(Path);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\buyItem.png");
	bool ret = LoadTextureFromFile4(Path, &my_texture4[0], &wd, &hd);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Restaure.png");
	ret = LoadTextureFromFile4(Path, &my_texture4[1], &wd, &hd);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Vip.png");
	ret = LoadTextureFromFile4(Path, &my_texture4[2], &wd, &hd);
	IM_ASSERT(ret);

	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Rename.png");
	ret = LoadTextureFromFile4(Path, &my_texture4[3], &wd, &hd);
	IM_ASSERT(ret);

	int y = 0;
	int pos = 0;

	for (int x = 0; x <= 10; x++)
	{
		sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Classes\\%d.png", x);
		bool ret = LoadTextureFromFile4(Path, &ClassImages[x], &wd, &hd);
		IM_ASSERT(ret);
	}

	//for (int x = 0; x <= 5; x++)
	//{
	//	sprintf_s(Path, sizeof(Path), "game\\images\\shop\\Donates\\%d.png", x);
	//	bool ret = LoadTextureFromFile4(Path, &DonateImages[x], &wd, &hd);
	//	IM_ASSERT(ret);
	//}

	for (auto& Items : NewShop::GetInstance()->ShopItems)
	{
		Items.imgPosition = y;

		switch (Items.CategoryID)
		{
		case 1:
			sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", Items.ItemCode);
			ItemShopImage[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;

		case 2:
			sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", Items.ItemCode);
			ItemShopImage[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		case 4:
			if (Items.CategoryID == 2 && Items.SubCategoryID == 4)
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", Items.ItemCode);
			else
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Defense\\it%s.bmp", Items.ItemCode);
			ItemShopImage[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		case 3:
			if (Items.SubCategoryID == 2 || Items.SubCategoryID == 3)
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Defense\\it%s.bmp", Items.ItemCode);
			else
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", Items.ItemCode);
			ItemShopImage[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		case 5:
			if ((Items.ItemCode[0] == 'G' && Items.ItemCode[1] == 'P') || (Items.ItemCode[0] == 'g' && Items.ItemCode[1] == 'p') || (Items.ItemCode[0] == 'S' && Items.ItemCode[1] == 'P'))
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Event\\it%s.bmp", Items.ItemCode);
			else if (Items.SubCategoryID == 3 || (Items.ItemCode[0] == 'S' && Items.ItemCode[1] == 'E'))
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", Items.ItemCode);
			else
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", Items.ItemCode);
			ItemShopImage[y] = LoadDibSurfaceOffscreen(Path);
			y++;
			break;
		}
	}


	for (int x = 0; x < 20; x++)
	{
		if (failedItems.FailedItems[x].ItemHead)
		{
			failedItems.FailedItems[x].imgPosition = x;

			if (failedItems.FailedItems[x].ItemCode[0] == 'D' && failedItems.FailedItems[x].ItemCode[1] == 'A')
			{
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Defense\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
				RestaureImage[x] = LoadDibSurfaceOffscreen(Path);
			}

			else if (failedItems.FailedItems[x].ItemCode[0] == 'W')
			{
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Weapon\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
				RestaureImage[x] = LoadDibSurfaceOffscreen(Path);
			}
			else if (failedItems.FailedItems[x].ItemCode[0] == 'B' && failedItems.FailedItems[x].ItemCode[1] == 'I')
			{
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Premium\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
				RestaureImage[x] = LoadDibSurfaceOffscreen(Path);
			}
			else
			{
				sprintf_s(Path, sizeof(Path), "image\\Sinimage\\Items\\Accessory\\it%s.bmp", failedItems.FailedItems[x].ItemCode);
				RestaureImage[x] = LoadDibSurfaceOffscreen(Path);
			}
		}
	}

	selectedClass = lpCurPlayer->smCharInfo.JOB_CODE;

	init = false;
}

void selectChangeClass()
{
	const char* combo_label = Classes[selectedChangeClass];

	int isPrs = 0;
	if (lpCurPlayer->smCharInfo.JOB_CODE == 8)
		isPrs = 1;

	if (ImGui::BeginCombo("Classe", combo_label, ImGuiComboFlags_NoArrowButton | ImGuiWindowFlags_NoSavedSettings))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Classes) - isPrs; n++)
		{
			if (n == lpCurPlayer->smCharInfo.JOB_CODE)
				n++;

			const bool is_selected = (selectedChangeClass == n);

			if (ImGui::Selectable(Classes[n], is_selected))
			{
				selectedChangeClass = n;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
}

void selectSpec()
{
	const char* combo_label = Classes[selectedClass];

	if (ImGui::BeginCombo("Selecionar Classe", combo_label, ImGuiComboFlags_NoArrowButton | ImGuiWindowFlags_NoSavedSettings))
	{
		for (int n = 0; n < IM_ARRAYSIZE(Classes); n++)
		{
			const bool is_selected = (selectedClass == n);
			if (ImGui::Selectable(Classes[n], is_selected))
			{
				selectedClass = n;
				ItemsAlreadySent.clear();
				ItemsDetails.clear();
				SendItemToServer(TempPerfectItem.DorpItem);
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();

		}
		ImGui::EndCombo();
	}
}

int posInicial = 30;

void ShowSubCategories(ItemsByCategory SubCategories)
{
	ImGui::SetCursorPosY(posInicial);
	ImGui::PushID(SubCategories.imgPosition);

	// Áre de seleção de compra
	ImGui::Selectable("##BuyItem", false, 0, ImVec2(300, 90));

	if (ImGui::IsItemHovered() && !finish && SubCategories.CategoryID != 5)
		ShowDetails(SubCategories.ItemCode);
	else
		isShowingDetails = false;

	ImGui::SetCursorPosX(310);
	ImGui::SetCursorPosY(posInicial - 4);

	if (ImGui::ImageButton(my_texture4[0], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
		ImGui::OpenPopup("Confirmar Compra?");

	FinishPurchase(SubCategories);

	ImGui::SetCursorPosX(310);
	ImGui::SetCursorPosY(posInicial + 60);
	selectSpec();

	ImGui::SetCursorPosY(posInicial);
	ImGui::SetCursorPosX(10);

	if (SubCategories.CategoryID == 3 || SubCategories.CategoryID == 5) {
		ImGui::SetCursorPosY(posInicial + 12);
		ImGui::SetCursorPosX(22);
		ImGui::Image(ItemShopImage[SubCategories.imgPosition], ImVec2(22, 22));
	}
	else if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 4)
		ImGui::Image(ItemShopImage[SubCategories.imgPosition], ImVec2(44, 44));
	else if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 5)
		ImGui::Image(ItemShopImage[SubCategories.imgPosition], ImVec2(22, 22));
	else if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 4)
		ImGui::Image(ItemShopImage[SubCategories.imgPosition], ImVec2(44, 44));
	else
		ImGui::Image(ItemShopImage[SubCategories.imgPosition], ImVec2(44, 44));

	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial); posInicial += 20;
	ImGui::Text("Nome: ");
	ImGui::SameLine();

	char itemName[64] = { 0 };
	sprintf_s(itemName, sizeof(itemName), "%s", SubCategories.ItemName);
	CA2W pszWide2(itemName);
	wsprintf(itemName, "%s", WChar_to_UTF82(pszWide2));
	ImGui::Text(itemName);

	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial); posInicial += 10;
	char price[32] = { 0 };
	sprintf_s(price, sizeof(price), "%d Coins", SubCategories.Price);
	ImGui::SetCursorPosY(posInicial); posInicial += 60;
	ImGui::Text(u8"Preço: ");
	ImGui::SameLine();
	ImGui::Text(price);
	ImGui::SetCursorPosY(posInicial); posInicial += 20;
	ImGui::Separator();
	ImGui::PopID();
}

void NewShop::LoadVipOptions()
{
	posInicial = 30;

	ImGui::SetCursorPosY(posInicial);
	ImGui::Selectable("##VIP", false, 0, ImVec2(380, 120));

	ImGui::SetCursorPosX(390);
	ImGui::SetCursorPosY(posInicial);

	if (ImGui::ImageButton(my_texture4[0], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
		ImGui::OpenPopup("Deseja comprar o VIP?");

	BuyVIP();

	ImGui::SetCursorPosY(65);
	ImGui::SetCursorPosX(10);
	ImGui::Image(my_texture4[2], ImVec2(48, 48));

	char msg[64] = { 0 };
	sprintf_s(msg, sizeof(msg), u8"VIP: Válido por 30 dias");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg);

	posInicial += 20;

	sprintf_s(msg, sizeof(msg), u8"Vantagens que o VIP oferece:");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg);

	posInicial += 20;

	sprintf_s(msg, sizeof(msg), u8"Comando /repot (Abre a Loja da Allya)");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg);

	posInicial += 20;

	sprintf_s(msg, sizeof(msg), u8"Comando /premium (Abre o Negociante Mughy)");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg);

	posInicial += 20;

	sprintf_s(msg, sizeof(msg), u8"%d %s de Exp. Add. dos Monstros", 10, "%%");
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg);

	posInicial += 20;

	sprintf_s(msg, sizeof(msg), u8"Valor: %d Coins", 2000);
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg);
}

int NewShop::TextEditCallback2(ImGuiInputTextCallbackData* data)
{
	switch (data->EventFlag)
	{

	case ImGuiInputTextFlags_CallbackAlways:
	{
		break;
	}

	case ImGuiInputTextFlags_CallbackHistory:
	{
		break;
	}

	}
	return 0;
}

static int TextEditCallbackStub(ImGuiInputTextCallbackData* data)
{
	ImWchar c = data->EventChar;

	if (ImGuiInputTextFlags_CharsHexadecimal)
		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
			return true;

	NewShop* console = (NewShop*)data->UserData;
	return console->TextEditCallback2(data);
}

void ConfirmarTrocaClasse()
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Confirmar Troca de Classe", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finish = true;

		ImGui::SetCursorPosY(20);
		ImGui::SetCursorPosX(240);
		ImGui::Image(ClassImages[lpCurPlayer->smCharInfo.JOB_CODE], ImVec2(32, 32));
		ImGui::SetCursorPosY(32);
		ImGui::SetCursorPosX(285);
		ImGui::Text(">");
		ImGui::SetCursorPosY(20);
		ImGui::SetCursorPosX(300);
		ImGui::Image(ClassImages[selectedChangeClass], ImVec2(32, 32));

		ImGui::SetCursorPosY(25);

		char msg[64] = { 0 };
		sprintf_s(msg, sizeof(msg), u8"Classe Anterior: %s", Classes[lpCurPlayer->smCharInfo.JOB_CODE]);
		ImGui::Text(msg);

		sprintf_s(msg, sizeof(msg), u8"Nova Classe: %s", Classes[selectedChangeClass]);
		ImGui::Text(msg);

		ImGui::SetCursorPosY(70);
		ImGui::Separator();

		ImGui::SetCursorPosY(70);
		sprintf_s(msg, sizeof(msg), u8"O valor: 10000 Coins");
		ImGui::Text(msg);

		ImGui::Separator();

		ImGui::SetCursorPosY(100);

		sprintf_s(msg, sizeof(msg), u8"ATENÇÃO: Você será desconectado após a confirmação");
		ImGui::Text(msg);

		sprintf_s(msg, sizeof(msg), u8"Entre novamente no jogo para visualizar a nova classe");
		ImGui::Text(msg);

		ImGui::Separator();

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			finish = false;

			ItemsByCategory cChangeClass;

			cChangeClass.CategoryID = 6;
			cChangeClass.SubCategoryID = 102;
			sprintf_s(cChangeClass.ItemName, sizeof(cChangeClass.ItemName), NewNick);
			sprintf_s(cChangeClass.ItemCode, sizeof(cChangeClass.ItemCode), "SR102");
			cChangeClass.Price = 10000;
			cChangeClass.Discount = 0;

			SentChangeClassToServer(cChangeClass, selectedChangeClass);
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finish = false; }
		ImGui::EndPopup();
	}
}

void ConfirmarTrocaNick(std::string newName)
{
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("Confirmar Troca de Nick", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		finish = true;

		ImGui::SetCursorPosY(18);
		ImGui::SetCursorPosX(320);
		ImGui::Image(my_texture4[3], ImVec2(45, 45));

		ImGui::SetCursorPosY(25);

		char msg[64] = { 0 };
		sprintf_s(msg, sizeof(msg), u8"Nick Anterior: %s", lpCurPlayer->smCharInfo.szName);
		ImGui::Text(msg);

		sprintf_s(msg, sizeof(msg), u8"Novo Nick: %s", newName.c_str());
		ImGui::Text(msg);

		ImGui::Separator();

		sprintf_s(msg, sizeof(msg), u8"O preço: 5000 Coins");
		ImGui::Text(msg);

		ImGui::Separator();

		ImGui::SetCursorPosY(80);

		sprintf_s(msg, sizeof(msg), u8"ATENÇÃO: Você será desconectado após a confirmação");
		ImGui::Text(msg);

		sprintf_s(msg, sizeof(msg), u8"Entre novamente no jogo para visualizar o seu novo nome");
		ImGui::Text(msg);

		ImGui::Separator();

		ImGui::Text("");
		ImGui::SetCursorPosX(60);
		if (ImGui::Button("Confirmar", ImVec2(120, 0)))
		{
			ImGui::CloseCurrentPopup();
			finish = false;

			ItemsByCategory ChangeName;

			ChangeName.CategoryID = 6;
			ChangeName.SubCategoryID = 101;
			sprintf_s(ChangeName.ItemName, sizeof(ChangeName.ItemName), NewNick);
			sprintf_s(ChangeName.ItemCode, sizeof(ChangeName.ItemCode), "SR101");
			ChangeName.Price = 5000;
			ChangeName.Discount = 0;

			SentChangeNickToServer(ChangeName, 0);
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancelar", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); finish = false; }
		ImGui::EndPopup();
	}
}

void NewShop::ChangeClass()
{
	posInicial = 30;

	char msg[256] = { 0 };
	sprintf_s(msg, sizeof(msg), u8"NENHUM item é trocado junto com a classe!");
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg); posInicial += 30;

	ImGui::SetCursorPosY(posInicial + 20);
	ImGui::Separator();

	sprintf_s(msg, sizeof(msg), u8"Custo para troca: 10000 Coins");
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg); posInicial += 30;

	ImGui::SetCursorPosX(75);
	ImGui::SetCursorPosY(posInicial + 30);
	selectChangeClass();

	posInicial += 30;
	ImGui::SetCursorPosX(150);
	ImGui::SetCursorPosY(posInicial + 60);

	if (ImGui::Button("Confirmar Nova Classe", ImVec2(150, 40)))
	{
		ImGui::OpenPopup("Confirmar Troca de Classe");
	}

	ConfirmarTrocaClasse();
}

void NewShop::ChangeNick()
{
	posInicial = 30;
	editingNick = true;

	char msg[256] = { 0 };
	sprintf_s(msg, sizeof(msg), u8"Você não conseguirá utilizar o chat enquanto estiver nessa aba!");
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg); posInicial += 30;

	ImGui::SetCursorPosY(posInicial + 20);
	ImGui::Separator();

	sprintf_s(msg, sizeof(msg), u8"Custo para troca: 5000 Coins");
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg); posInicial += 30;

	sprintf_s(msg, sizeof(msg), u8"Preencha o novo nick desejado e clique no botão 'Verificar Disponibilidade'");
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg); posInicial += 20;

	sprintf_s(msg, sizeof(msg), u8"Se o nome escolhido esteja disponível, o botão de confirmar será habilitado!");
	ImGui::SetCursorPosX(5);
	ImGui::SetCursorPosY(posInicial);
	ImGui::Text(msg); posInicial += 30;

	ImGui::SetCursorPosY(posInicial);
	ImGui::Separator();

	posInicial += 30;
	ImGui::SetCursorPosX(80);
	ImGui::SetCursorPosY(posInicial);

	if (ImGui::InputText("Nick", NewNick, IM_ARRAYSIZE(NewNick), ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_CallbackHistory | ImGuiInputTextFlags_CallbackCharFilter, &TextEditCallbackStub, (void*)this))
	{
		nickIsAvailable = 0;
	}

	posInicial += 45;

	ImGui::SetCursorPosX(150);
	ImGui::SetCursorPosY(posInicial);

	if (ImGui::Button("Verificar Disponibilidade", ImVec2(150, 40)))
	{
		// Evita nome em branco
		if (NewNick != "" && strlen(NewNick) > 0)
		{
			ItemsByCategory CheckName;

			CheckName.CategoryID = 6;
			CheckName.SubCategoryID = 101;
			sprintf_s(CheckName.ItemName, sizeof(CheckName.ItemName), NewNick);
			sprintf_s(CheckName.ItemCode, sizeof(CheckName.ItemCode), "SR101");
			CheckName.Price = 5000;
			CheckName.Discount = 0;

			SentCheckNickToServer(CheckName, 0);
		}
	}

	if (nickIsAvailable == 1)
	{
		sprintf_s(msg, sizeof(msg), u8"O Nome escolhido está disponível!");
		ImGui::SetCursorPosX(122);
		ImGui::SetCursorPosY(posInicial - 24);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.04f, 0.74f, 0.10f, 0.78f));
		ImGui::LabelText("##NickOk", msg, 12);
		ImGui::PopStyleColor();
	}
	else if (nickIsAvailable == 2)
	{
		sprintf_s(msg, sizeof(msg), u8"O nome escolhido já está em uso!");
		ImGui::SetCursorPosX(122);
		ImGui::SetCursorPosY(posInicial - 24);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.04f, 0.04f, 0.78f));
		ImGui::LabelText("##NickNotOk", msg, 12);
		ImGui::PopStyleColor();
	}

	posInicial += 60;
	ImGui::SetCursorPosY(posInicial);
	ImGui::Separator();

	if (nickIsAvailable == 1)
	{
		posInicial += 20;
		ImGui::SetCursorPosX(150);
		ImGui::SetCursorPosY(posInicial);

		if (ImGui::Button("Confirmar Troca", ImVec2(150, 40)))
		{
			ImGui::OpenPopup("Confirmar Troca de Nick");
		}

		ConfirmarTrocaNick(NewNick);
	}

}

void NewShop::RestaureItems()
{
	posInicial = 30;

	for (int x = 0; x < 20; x++)
	{
		if (failedItems.FailedItems[x].ItemHead)
		{
			ImGui::SetCursorPosY(posInicial);
			ImGui::PushID(x);

			ImGui::Selectable("##RestaureItem", false, 0, ImVec2(300, 90));

			if (ImGui::IsItemHovered() && !finish)
				ShowDetailsRestaure(failedItems.FailedItems[x].ItemCode, failedItems.FailedItems[x].ItemHead, failedItems.FailedItems[x].ItemChkSum, failedItems.FailedItems[x].AgingNum);
			else
				isShowingDetailsRestaure = false;


			ImGui::SetCursorPosX(340);
			ImGui::SetCursorPosY(posInicial);

			if (ImGui::ImageButton(my_texture4[1], ImVec2(32, 32), ImVec2(0, 0), ImVec2(1, 1), -1, ImVec4(0, 0, 0, 0)))
				ImGui::OpenPopup("Deseja restaurar o Item?");

			FinishRestaure(failedItems.FailedItems[x]);

			ImGui::SetCursorPosY(posInicial);
			ImGui::SetCursorPosX(10);
			ImGui::Image(RestaureImage[failedItems.FailedItems[x].imgPosition], ImVec2(56, 78));

			char msg[64] = { 0 };
			sprintf_s(msg, sizeof(msg), u8"Nome do Item: %s", failedItems.FailedItems[x].ItemName);
			ImGui::SetCursorPosX(80);
			ImGui::SetCursorPosY(posInicial);
			CA2W pszWide2(msg);
			wsprintf(msg, "%s", WChar_to_UTF82(pszWide2));
			ImGui::Text(msg); posInicial += 20;

			sprintf_s(msg, sizeof(msg), u8"NÍvel do aging: %d", failedItems.FailedItems[x].AgingNum);
			ImGui::SetCursorPosX(80);
			ImGui::SetCursorPosY(posInicial);
			ImGui::Text(msg); posInicial += 20;

			sprintf_s(msg, sizeof(msg), u8"Data da quebra: %02d/%02d/%02d", failedItems.FailedItems[x].Date.wDay, failedItems.FailedItems[x].Date.wMonth, failedItems.FailedItems[x].Date.wYear);
			ImGui::SetCursorPosX(80);
			ImGui::SetCursorPosY(posInicial);
			ImGui::Text(msg); posInicial += 20;

			sprintf_s(msg, sizeof(msg), u8"O valor: %d Coins", failedItems.price);
			ImGui::SetCursorPosX(80);
			ImGui::SetCursorPosY(posInicial);
			ImGui::Text(msg); posInicial += 30;

			ImGui::SetCursorPosY(posInicial);
			ImGui::Separator();
			ImGui::PopID();
			posInicial += 20;
		}
	}
}

void NewShop::ReceiveItems(NEWSHOP_COMPRESSEDPCKG* Data)
{
	ITEMS_INFOCKG getAllItems;
	ZeroMemory(&getAllItems, sizeof(ITEMS_INFOCKG));

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

	init = true;
	openFlag = true;
}

void NewShop::OpenNpc(bool* p_open)
{
	if (init)
		initShop();

	ImGui::SetNextWindowSize(ImVec2(600, 390), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2((smScreenWidth / 2) - 280, (smScreenHeight / 2) - 220));
	ImGui::SetNextWindowBgAlpha(0.90f);

	ImGui::StyleColorArmageddon();

	if (ImGui::Begin("Loja de Itens", p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse))
	{
		// Left
		static int selected = 0;
		static int selectedItem = 0;
		editingNick = false;

		{
			ImGui::BeginChild("Categorias", ImVec2(150, 0), true);

			for (auto Categorias : vCategorias)
			{
				char label[128];
				sprintf(label, "%s", Categorias.CategoryName.c_str());

				if (ImGui::Button(label, ImVec2(140, 40)))
				{
					selected = Categorias.CategoryID;
				}
			}

			ImGui::SetCursorPosY(300);
			ImGui::Image(ShopImages[0], ImVec2(50, 50));
			char coins[32] = { 0 };
			sprintf_s(coins, sizeof(coins), "%d Coins", GetCoin());
			ImGui::SetCursorPosY(317);
			ImGui::SetCursorPosX(60);
			ImGui::Text(coins);

			//ImGui::SetCursorPosY(354);
			//if (ImGui::Button(u8"Adicionar Créditos")) // Donates
			//	ImGui::OpenPopup(u8"Efetuar uma doação");
			// 
			//showDonationOptions();

			ImGui::EndChild();
		}

		if (bIsPerfect && !finish) // Mostra os detalhes do item
		{
			if (smScreenWidth > 800 && smScreenHeight > 600) // 800x600 não suporta o preview do item
			{
				sinShowItemInfoFlag = 1;
				GAMECOREHANDLE->pcItemInfoBox->PrepareShowItem(&TempPerfectItem, FALSE, FALSE, FALSE);
			}
		}

		if (!isShowingDetailsRestaure && !isShowingDetails)
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
		case 1:

			// Armas
			if (ImGui::BeginTabBar("##Ataque", ImGuiTabBarFlags_None))
			{
				posInicial = 30;

				for (auto& SubCategories : ShopItems)
				{
					if (ImGui::BeginTabItem("Caixas"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 1)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("Espadas"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 2)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Foices"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 3)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Garras"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 4)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Lanças"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 5)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Machados"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 6)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Martelos"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 7)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem("Varinhas"))
					{
						if (SubCategories.CategoryID == 1 && SubCategories.SubCategoryID == 8)
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

		case 2:
			// Armaduras
			if (ImGui::BeginTabBar("##Defesa", ImGuiTabBarFlags_None))
			{
				posInicial = 30;

				for (auto& SubCategories : ShopItems)
				{
					if (ImGui::BeginTabItem("Caixas"))
					{
						if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 1)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem(u8"Roupões"))
					{
						if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 2)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Escudos"))
					{
						if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 3)
						{
							ShowSubCategories(SubCategories);
						}

						ImGui::EndTabItem();
					}
					if (ImGui::BeginTabItem(u8"Orbitais"))
					{
						if (SubCategories.CategoryID == 2 && SubCategories.SubCategoryID == 4)
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
		case 3:
			// Acessórios
			if (ImGui::BeginTabBar("##Acessórios", ImGuiTabBarFlags_None))
			{
				posInicial = 30;

				for (auto& SubCategories : ShopItems)
				{
					if (ImGui::BeginTabItem("Braceletes"))
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
					if (ImGui::BeginTabItem("Brincos"))
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
					}
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
				posInicial = 30;

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
				posInicial = 30;

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
		case 6:
			// Serviços
			if (ImGui::BeginTabBar("##Serviços", ImGuiTabBarFlags_None))
			{
				posInicial = 30;
				if (ImGui::BeginTabItem("Restaurar Item"))
				{
					RestaureItems();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("VIP"))
				{
					LoadVipOptions();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Troca de Nick"))
				{
					ChangeNick();
					ImGui::EndTabItem();
				}

				//if (ImGui::BeginTabItem("Troca de Classe"))
				//{
				//	ChangeClass();
				//	ImGui::EndTabItem();
				//}

				ImGui::EndTabBar();
			}
			ImGui::EndChild();
			ImGui::EndGroup();
			break;
		}
	}
	ImGui::End();
}
