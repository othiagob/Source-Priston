#pragma once

#define     MAX_SHOP_CLASS_ITEM	30  //각 클래스당 30씩

#define     SHOP_ROW			9  //임시 상점 아이템 영역 
#define     SHOP_COL			9
	
#define 	SINALLREPAIR		1 
#define 	SINREPAIR			2    
#define 	SINSELL				3      
#define 	SINBUY				4       
#define 	SINRIGHTARROW		5
#define 	SINLEFTARROW		6  
#define     SINSHOPEXIT			7

#define 	SIN_WEAPON			1 
#define 	SIN_DEFENSE			2 
#define 	SIN_ETC				3 


struct	sMYSHOP_ITEM{
	sITEMINFO	sItem;
	int			Price;
	short       Posi[2];
	DWORD		dwTemp[3];
};

struct sMYSHOP{
	DWORD			CHAR_CODE;
	DWORD			dwTemp[6];
	sMYSHOP_ITEM	sMyShopItem[30]; //보내고 받는 아이템
};

struct sMYSHOP_ITEM_SERVER{ //개인상점으로 구매할 아이템 
	DWORD CODE;
	DWORD CkSum;
	DWORD Head;
	DWORD sTime;
	DWORD Price;
	DWORD SendFlag;
	DWORD Buyer;

	DWORD Temp[4];

};

class cSHOP{


public:

	int OpenFlag;	//현재 인벤토리가 열려있나 아닌가에 대한 플랙 
	int UseItemFlag; // 박재원 - 이동 상점 아이템

	int PosX;
	int PosY;

	int MatShop[10];

	int SelectShop; // 1무기 ,2 방어구 ,3 그외 것들 

	int ShopIconPosi;

	
	sITEM ShowShopItem[30];	//한번에 보여질 아이템 최대 30
	sITEM ShopItem[60]; //저장될 아이템 
	

	int RevWeaponCnt;
	int RevDefenseCnt;
	int RevEtcCnt;
	int MatMyShop_Button;
	
	LPDIRECT3DTEXTURE9 lpGoldEdit;

	LPDIRECT3DTEXTURE9 lpAllRepair;
	LPDIRECT3DTEXTURE9 lpRepair;

	LPDIRECT3DTEXTURE9 lpSell;
	LPDIRECT3DTEXTURE9 lpBuy;

	LPDIRECT3DTEXTURE9 lpAllRepair_D;
	LPDIRECT3DTEXTURE9 lpRepair_D;

	LPDIRECT3DTEXTURE9 lpSell_D;
	LPDIRECT3DTEXTURE9 lpBuy_D;

	LPDIRECT3DTEXTURE9 lpRightArrow;
	LPDIRECT3DTEXTURE9 lpLeftArrow;

	
	LPDIRECT3DTEXTURE9 lpTabAttack;
	LPDIRECT3DTEXTURE9 lpTabDefense;
	LPDIRECT3DTEXTURE9 lpTabEtc;

	LPDIRECT3DTEXTURE9 lpTabAttack_D;
	LPDIRECT3DTEXTURE9 lpTabDefense_D;
	LPDIRECT3DTEXTURE9 lpTabEtc_D;

	LPDIRECT3DTEXTURE9 lpExit;
	LPDIRECT3DTEXTURE9 lpGrid;

	LPDIRECT3DTEXTURE9 lpRepairAllInfo;	
	LPDIRECT3DTEXTURE9 lpRepairInfo;
	LPDIRECT3DTEXTURE9 lpDefaultRepairAll;	
	LPDIRECT3DTEXTURE9 lpDefaultRepair;

	LPDIRECT3DTEXTURE9 lpSellInfo;
	LPDIRECT3DTEXTURE9 lpBuyInfo;


	LPDIRECT3DTEXTURE9 lpTitle_Attack;
	LPDIRECT3DTEXTURE9 lpTitle_T;
	LPDIRECT3DTEXTURE9 lpTitle_Etc;
	LPDIRECT3DTEXTURE9 lpTitle_Trade;
	LPDIRECT3DTEXTURE9 lpTitle_WareHouse;

	LPDIRECT3DTEXTURE9 lpWeightEdit;

	LPDIRECT3DTEXTURE9 lpNoUseBox;

	LPDIRECT3DTEXTURE9 lpCraftItemMain;
	LPDIRECT3DTEXTURE9 lpCraftItemButton;
	LPDIRECT3DTEXTURE9 lpCraftItemButtonInfo;
	LPDIRECT3DTEXTURE9 lpTitle_CraftItem;

	// pluto 제련
	LPDIRECT3DTEXTURE9 lpSmeltingItemMain;
	LPDIRECT3DTEXTURE9 lpTitle_SmeltingItem;
	LPDIRECT3DTEXTURE9 lpSmeltingItemButtonInfo;

	// pluto 제작
	LPDIRECT3DTEXTURE9 lpManufactureItemMain;
	LPDIRECT3DTEXTURE9 lpTitle_ManufactureItem;
	LPDIRECT3DTEXTURE9 lpManufactureItemButtonInfo;

	// 석지용 - 믹스쳐 리셋용 이미지 로딩 추가
	LPDIRECT3DTEXTURE9 lpMResetTitle;
	LPDIRECT3DTEXTURE9 lpMResetMain;
	LPDIRECT3DTEXTURE9 lpMResetButton;
	LPDIRECT3DTEXTURE9 lpMResetButtonInfo;
	
	LPDIRECT3DTEXTURE9 lpTitle_Aging;
	LPDIRECT3DTEXTURE9 lpAging_Info;
	

	LPDIRECT3DTEXTURE9	lpShopMain;
	LPDIRECT3DTEXTURE9	lpShopMain2;


	//개인 상점
	LPDIRECT3DTEXTURE9	lpMyShopExp;
	LPDIRECT3DTEXTURE9	lpMyShopExp_;
	LPDIRECT3DTEXTURE9	lpMyShopExp_T;
	LPDIRECT3DTEXTURE9	lpMyShopNoSale;
	LPDIRECT3DTEXTURE9	lpMyShopNoSale_;
	LPDIRECT3DTEXTURE9	lpMyShopNoSale_T;
	LPDIRECT3DTEXTURE9	lpMyShopSale;
	LPDIRECT3DTEXTURE9	lpMyShopSale_;
	LPDIRECT3DTEXTURE9	lpMyShopSale_T;
	LPDIRECT3DTEXTURE9    lpTitle_MyShop;
	LPDIRECT3DTEXTURE9    lpImage_MyShop;
	LPDIRECT3DTEXTURE9    lpMyShop_T;
	

	LPDIRECT3DTEXTURE9    MainCaravan_Arma;
	LPDIRECT3DTEXTURE9    MainCaravan_Hopy;
	LPDIRECT3DTEXTURE9    MainCaravan_Buma;

	LPDIRECT3DTEXTURE9    Caravan_Stay;
	LPDIRECT3DTEXTURE9    Caravan_Stay_;
	LPDIRECT3DTEXTURE9    Caravan_Stay_gray;

	LPDIRECT3DTEXTURE9    Caravan_Follow;
	LPDIRECT3DTEXTURE9    Caravan_Follow_;
	LPDIRECT3DTEXTURE9    Caravan_Follow_gray;

	LPDIRECT3DTEXTURE9    Caravan_Rename;
	LPDIRECT3DTEXTURE9    Caravan_Rename_;

	LPDIRECT3DTEXTURE9    Caravan_BtnOkRename;
	LPDIRECT3DTEXTURE9    Caravan_BtnOkRename_;

	LPDIRECT3DTEXTURE9    Caravan_BtnCancel;
	LPDIRECT3DTEXTURE9    Caravan_BtnCancel_;

	LPDIRECT3DTEXTURE9    Caravan_BtnUndo;
	LPDIRECT3DTEXTURE9    Caravan_BtnUndo_;

	LPDIRECT3DTEXTURE9    Caravan_RenameBox;

	LPDIRECT3DTEXTURE9    Caravan_TipFollow;
	LPDIRECT3DTEXTURE9    Caravan_TipRename;
	LPDIRECT3DTEXTURE9    Caravan_TipStay;
	
public:
	cSHOP();
	~cSHOP();

	void Init(); //클래스 초기화
	void Load();
	void Release();
	void Draw();
	void Close();//클래스 종료 
	void Main();
	void LButtonDown(int x , int y);
	void LButtonUp(int x , int y);
	void RButtonDown(int x , int y);
	void RButtonUp(int x, int y);
	void KeyDown(); 

	void CheckShopNpcState();  //NPC와 헤어짐을 체크한다 

	//void CopyShopItemToShow(int Index);//서버에서 받아온 아이템을 보여줄 아이템으로 복사한다 
	void CopyShopItemToShow(int Index,int Kind=0);

	int GetShopItemXY(sITEM *pItem); //아이템의 좌표를 구한다 

	void DrawShopText(); //상점 텍스트를 표시한다 

	////////////////////아아템 사고 팔기 
	int RecvBuyItemToServer(sITEM *pItem ,int ItemCount=1); //아이템을 산다 
	int SellItemToShop(sITEM *pItem,int ItemCount=1); //아아템을 판다 

	int SendBuyItemToServer(sITEM *pItem , int ItemCount=1 ); //서버에서아이템을 사온다 

	void DeleteShopItem(); //상점을 닫을때 상점아이템을 초기화한다

	int CheckHighRankItem(sITEM *pItem); //좋은아이템 체크

	//현재 아이템을 살수있는 지 체크한다.
	int haBuyMoneyCheck(int BuyMoney);
	//상점 아이템 세율적용된 가격을 표시해준다.
	int haShopItemPrice(int Money);
};

/*----------------------------------------------------------------------------*
*								개인상점 등록 클래스 
*-----------------------------------------------------------------------------*/
class cMYSHOP{
public:
	int OpenFlag;	
	sITEM MyShopItem[30];

	char szDoc[128];     //홍보문구
	char szSendDoc[128]; //서버로 보낼 홍보문구

public:
	
	//아이템이 셋팅될공간을 찾는다
	int SetMyShopItem(sITEM *pItem);
	int CanSellItem(sITEM* pItem);
	int GetEmptyArea(sITEM* pItem, POINT* EmptyPos);

	//셋팅한다
	int LastSetMyShopItem(sITEM *pItem);

	//인벤아이템을 등록한다
	int SetShopItemToInven(sITEM *pItem);

	//상점에 등록한 아이템을 찾는다
	int SearchShopItemToInven(sITEM *pItem);

	//아이템을 서버로 보낸다 
	int SendMyShopItem(); 

	//서버에서 구매요청이 들어온 아이템을 받는다
	int RecvMyShopItem( DWORD dwCharCode , sMYSHOP_ITEM_SERVER *lpShopItem );

	//자신이 가질수 있는 금액을 리턴한다
	int GetLimitMoney(int Money);

	//물건이 다팔리면 상점을 닫는다
	int AutoCloseShop();

	//개인상점의 돈을 구해온다
	int GetTotalMyShopItemMoney();

};
/*----------------------------------------------------------------------------*
*								개인상점 클래스 
*-----------------------------------------------------------------------------*/
class cCHARSHOP{
public:
	int OpenFlag;	

public:
	sITEM CharShopItem[30];
	
	//아이템을 받는다
	int RecvShopItem(sMYSHOP *sMyShop);

	//받은 아이템을 찾는다
	int SearchMyShopItem(DWORD CODE ,DWORD Head , DWORD CheckSum);

	//개인상점에서산 아이템을 인벤토리로 셋팅한다
	int SetCharShopItemToInven(sITEMINFO *pItem_Info);

	//개인상점에서 아이템을 산다
	int BuyItem(int Index);

};

extern cSHOP cShop;

extern int sinShopKind;  //현재 상점의 종류  1  무기 2 방어 3 물약 (defalt 는 상점)

extern int TalkNpcState; //현재 상점 NPC와 함께 있을경우 

extern int ShopGoldEdit[2][4];

extern int ShopArrowPosi; //상점 화살표 방향 

extern int BuyItemServerFlag; //서버에서 물건을 살때 메세지를 받지않으면 살수없다 

extern cMYSHOP cMyShop;
extern cCHARSHOP cCharShop;

extern sMYSHOP     sMyShop;       //보내는 MyShop 구조체
extern sMYSHOP     sRecvMyShop;   //받는 MyShop 구조체
extern int MyShopSendButton;

extern int MyShopExpBox;
extern int MyShopItemSellMoney2;
extern sMYSHOP_ITEM_SERVER sMyShop_Server;
extern sITEMINFO MyShopPotion;
extern int MyShopItemCancelIndex[2];
