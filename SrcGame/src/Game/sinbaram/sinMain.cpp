/*----------------------------------------------------------------------------*
*	파일명 :  sinMain.cpp
*	하는일 :  신바람 메인
*	작성일 :  최종업데이트 12월
*	적성자 :  박상열
*-----------------------------------------------------------------------------*/
#define _SINBARAM_TEST   //빌드시에는 항상 꺼준다 

#include "sinLinkHeader.h"
#include "..\\Shop\\NewShop.h"
#include "cSkinChanger.h"
/*----------------------------------------------------------------------------*
*					         전역 변수
*-----------------------------------------------------------------------------*/

smCHAR_INFO *sinChar;		 //캐릭터 정보 구조체 
smCHAR_INFO sinCharDisplay;  //캐릭터 정보 구조체 
int sinbaram_Stop = 0;

#ifndef _CHEADER
#define _CHEADER

#include "..\AntiCheat.h"

#endif

CAntiCheat * pcAntiCheat = NULL;

void sinInit()
{
	InitSub();
	InitCursor(); 
	cItem.Init();
	CSKILL->Init();
	cInvenTory.Init();
	cCharStatus.Init();
	cInterFace.Init();
	cShop.Init();
	cTrade.Init();
	cMessageBox.Init();
	cSin3D.Init();
	cSinHelp.Init();
	cSinWarpGate.Init();
	g_CreatePetSystem();
	cHelpPet.Init();
	cSinSod2.Init();

	g_PCBang_CreatePetSystem();
	cPCBANGPet.Init();
	WarMode.Init();

	chaSiege.init();
	chaQuest.init();
	chaPremiumitem.init();
	Caravana::GetInstance()->Init();
	cSkinChanger.Init();

	void web_DB_start();
	void clanMENU_INIT(int setN);
	clanMENU_INIT(1);


	Settings::GetInstance()->Init();
}

DWORD dwSinTime = 0;

void sinMain()
{
	sinMouseMain();	
	sinKeyClickMain();

	if (sinGetMouseButton(0) == TRUE)
		sinProc(SINLBUTTONDOWN);
	if (sinGetMouseButton(1) == TRUE)
		sinProc(SINRBUTTONDOWN);

	if (MouseButtonUpEvent == SINLBUTTONUP) {
		sinProc(SINLBUTTONUP);
		MouseButtonUpEvent = 0;

	}

	if (MouseButtonUpEvent == SINRBUTTONUP) {
		sinProc(SINRBUTTONUP);
		MouseButtonUpEvent = 0;

	}

	sinProc(SINMOUSEMOVE);

	MainSub();
	cItem.Main();
	cInterFace.Main();
	CSKILL->Main();
	cInvenTory.Main();
	cCharStatus.Main();
	cShop.Main();
	cTrade.Main();
	cMessageBox.Main();
	cSinHelp.Main();
	cSinWarpGate.Main();
	cHelpPet.Main();
	cSinSod2.Main();
	chaSiege.Main();
	chaQuest.Main();
	chaPremiumitem.Main();
	void ktj_imsi_clanMENU_Main();
	ktj_imsi_clanMENU_Main();

	ShowInterFace();
	cPCBANGPet.Main();


	if (sinbaram_Stop) {
		dwSinTime++;
		if (dwSinTime >= 3 * 70) sinbaram_Stop = 0;
	}
	else dwSinTime = 0;

}

void sinDraw()
{
	dsDrawOffsetArray = dsARRAY_TOP;

	// Premiuns por baixo das outras coisas
	chaPremiumitem.DrawUp();
	chaPremiumitem.DrawText();

	dsDrawOffsetArray = dsARRAY_RIGHT | dsARRAY_BOTTOM;
	DrawInterfaceParty();

	dsDrawOffsetArray = dsARRAY_BOTTOM;

	cCharStatus.Draw();

	cCharStatus.DrawCharText();

	dsDrawOffsetArray = dsARRAY_BOTTOM;

	CSKILL->Draw();

	dsDrawOffsetArray = dsARRAY_TOP;

	CSKILL->DrawUp();

	dsDrawOffsetArray = dsARRAY_BOTTOM;

	cInvenTory.Draw();

	cInvenTory.DrawInvenText();

	dsDrawOffsetArray = dsARRAY_TOP;

	cShop.Draw();

	cShop.DrawShopText();

	cTrade.Draw();

	cTrade.DrawTradeText();

	dsDrawOffsetArray = dsARRAY_BOTTOM;

	cInterFace.Draw();

	cInterFace.DrawInterText();

	cItem.Draw();

	dsDrawOffsetArray = dsARRAY_TOP;

	cSinHelp.Draw();

	cSinHelp.DrawText();

	cSinWarpGate.Draw();

	cSinWarpGate.DrawText();

	sinDrawInterEffect();

	dsDrawOffsetArray = dsARRAY_TOP;
	void ktj_imsi_clanMENU_Draw();
	ktj_imsi_clanMENU_Draw();

	dsDrawOffsetArray = dsARRAY_BOTTOM;

	sinDrawText();

	dsDrawOffsetArray = dsARRAY_TOP;

	cHelpPet.Draw();

	cHelpPet.DrawText();

	cSinSod2.Draw();

	cSinSod2.DrawText();

	chaSiege.Draw();

	chaSiege.DrawText();

	chaQuest.Draw();

	chaPremiumitem.Draw();

	cMessageBox.Draw();

	cMessageBox.DrawMessageText();

	dsDrawOffsetArray = dsARRAY_TOP;

	WarMode.Draw();
	Settings::GetInstance()->Draw();
	cSkinChanger.Draw();

	if (GRAPHICDEVICE) 
	{
		DrawCursor();
	}

	GAMECOREHANDLE->pcItemInfoBox->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
	GAMECOREHANDLE->pcSkillInfoBox->Render(0, 0, RESOLUTION_WIDTH, RESOLUTION_HEIGHT, 0, 0);
}

void sinDrawText()
{
	EffectTestDrawText();
	EffectTestDrawText2();
}

void sinProc(int Message)
{

	switch (Message)
	{
		
	case SINLBUTTONDOWN:
		if (sinbaram_Stop)return;
		cInvenTory.CheckMousePotionNumForm();
		cMessageBox.LButtonDown(pCursorPos.x, pCursorPos.y);
		cSinSod2.LButtonDown(pCursorPos.x, pCursorPos.y);    //신바람 Sod2
		if (sinMessageBoxShowFlag && !MyShopExpBox && !nName)return;   //홍보문구 입력때문에 날치기 땜빵 ㅠ.ㅠ 크흑 // 장별 - 입력창
		cInterFace.LButtonDown(pCursorPos.x, pCursorPos.y);  //인터페이스 
		cCharStatus.LButtonDown(pCursorPos.x, pCursorPos.y); //캐릭터 스테이터스 
		cInvenTory.LButtonDown(pCursorPos.x, pCursorPos.y);  //인벤토리 
		CSKILL->LButtonDown(pCursorPos.x, pCursorPos.y);      //스킬 
		cTrade.LButtonDown(pCursorPos.x, pCursorPos.y);      //트레이드 
		cItem.LButtonDown(pCursorPos.x, pCursorPos.y);       //아이템 
		cShop.LButtonDown(pCursorPos.x, pCursorPos.y);       //상점 
		cSin3D.LButtonDown(pCursorPos.x, pCursorPos.y);      //신바람 3D
		cSinHelp.LButtonDown(pCursorPos.x, pCursorPos.y);    //신바람 도우미
		cSinWarpGate.LButtonDown(pCursorPos.x, pCursorPos.y);//신바람 워프게이트
		cHelpPet.LButtonDown(pCursorPos.x, pCursorPos.y);    //신바람 펫 도우미
		cInvenTory.ReFormMousePotionNum();                  //인증해준다 
		chaSiege.LButtonDown(pCursorPos.x, pCursorPos.y);
		chaQuest.LButtonDown(pCursorPos.x, pCursorPos.y);
		chaPremiumitem.LButtonDown(pCursorPos.x, pCursorPos.y);
		WarMode.Button();
		Settings::GetInstance()->Button();
		CustomHud::GetInstance()->GotClick = true;
		cSkinChanger.LButtonDown();
		break;
	case SINLBUTTONUP:
		if (sinbaram_Stop)return;
		cMessageBox.LButtonUp(pCursorPos.x, pCursorPos.y);   //메세지 박스 
		if (sinMessageBoxShowFlag)return;
		cInterFace.LButtonUp(pCursorPos.x, pCursorPos.y);    //인터페이스 
		cCharStatus.LButtonUp(pCursorPos.x, pCursorPos.y);   //캐릭터 스테이터스 
		cInvenTory.LButtonUp(pCursorPos.x, pCursorPos.y);    //인벤토리 
		cShop.LButtonUp(pCursorPos.x, pCursorPos.y);		    //상점 
		CSKILL->LButtonUp(pCursorPos.x, pCursorPos.y);	    //스킬 
		cTrade.LButtonUp(pCursorPos.x, pCursorPos.y);	    //트레이드 
		cSin3D.LButtonUp(pCursorPos.x, pCursorPos.y);        //신바람 3D
		cSinHelp.LButtonUp(pCursorPos.x, pCursorPos.y);      //신바람 도우미
		cSinWarpGate.LButtonUp(pCursorPos.x, pCursorPos.y);  //신바람 워프게이트
		cHelpPet.LButtonUp(pCursorPos.x, pCursorPos.y);      //신바람 펫 도우미
		cSinSod2.LButtonUp(pCursorPos.x, pCursorPos.y);      //신바람 Sod2
		//공성전 테스트
		chaSiege.LButtonUp(pCursorPos.x, pCursorPos.y);
		break;

	case SINRBUTTONDOWN:
		if (MyShopSendButton)return;
		if (sinbaram_Stop)return;
		cMessageBox.RButtonDown(pCursorPos.x, pCursorPos.y); //메세지 박스 
		if (sinMessageBoxShowFlag)return;
		cInterFace.RButtonDown(pCursorPos.x, pCursorPos.y);  //인터페이스 
		cCharStatus.RButtonDown(pCursorPos.x, pCursorPos.y); //캐릭터 스테이터스 
		cShop.RButtonDown(pCursorPos.x, pCursorPos.y);       //상점 
		cInvenTory.RButtonDown(pCursorPos.x, pCursorPos.y);  //인벤토리 
		CSKILL->RButtonDown(pCursorPos.x, pCursorPos.y);      //스킬 
		cTrade.RButtonDown(pCursorPos.x, pCursorPos.y);      //트레이드 
		cItem.RButtonDown(pCursorPos.x, pCursorPos.y);       //아이템 
		cSin3D.RButtonDown(pCursorPos.x, pCursorPos.y);      //신바람 3D
		cSinHelp.RButtonDown(pCursorPos.x, pCursorPos.y);    //신바람 도우미
		cSinWarpGate.RButtonDown(pCursorPos.x, pCursorPos.y);//신바람 워프게이트
		cHelpPet.RButtonDown(pCursorPos.x, pCursorPos.y);    //신바람 펫 도우미
		cSinSod2.RButtonDown(pCursorPos.x, pCursorPos.y);    //신바람 Sod2

		break;

	case SINRBUTTONUP:
		if (sinbaram_Stop)return;
		cMessageBox.RButtonUp(pCursorPos.x, pCursorPos.y);   //메세지 박스 
		if (sinMessageBoxShowFlag)return;
		cInterFace.RButtonUp(pCursorPos.x, pCursorPos.y);    //인터페이스 
		cCharStatus.RButtonUp(pCursorPos.x, pCursorPos.y);   //캐릭터 스테이터스 
		cInvenTory.RButtonUp(pCursorPos.x, pCursorPos.y);    //인벤토리 
		cShop.RButtonUp(pCursorPos.x, pCursorPos.y);		    //상점 
		CSKILL->RButtonUp(pCursorPos.x, pCursorPos.y);	    //스킬 
		cTrade.RButtonUp(pCursorPos.x, pCursorPos.y);	    //트레이드 
		cMessageBox.RButtonUp(pCursorPos.x, pCursorPos.y);   //메세지 박스 
		cSin3D.RButtonUp(pCursorPos.x, pCursorPos.y);        //신바람 3D
		cSinHelp.RButtonUp(pCursorPos.x, pCursorPos.y);      //신바람 도우미
		cSinWarpGate.RButtonUp(pCursorPos.x, pCursorPos.y);  //신바람 워프게이트
		cHelpPet.RButtonUp(pCursorPos.x, pCursorPos.y);		//신바람 펫 도우미
		cSinSod2.RButtonUp(pCursorPos.x, pCursorPos.y);		//신바람 Sod2
		break;

	case SINMOUSEMOVE:

		break;

	case SINKEYDOWN:
		if (NewShop::GetInstance()->openFlag && NewShop::GetInstance()->editingNick)
			return;

		if (sinGetKeyClick(VK_RETURN))
		{
			extern int MessageButtonPosi;
			MessageButtonPosi = 1;
			cMessageBox.LButtonDown(pCursorPos.x, pCursorPos.y);
		}

		if (sinbaram_Stop)return;
		cMessageBox.KeyDown();
		if (sinMessageBoxShowFlag)return;
		cInterFace.KeyDown();    //인터페이스 
		cCharStatus.KeyDown();   //캐릭터 스테이터스 
		cInvenTory.KeyDown();    //인벤토리 
		cShop.KeyDown();		 //상점 
		CSKILL->KeyDown();	     //스킬 
		cTrade.KeyDown();	     //트레이드 
		cSin3D.KeyDown();		 //신바람 3D 
		cSinHelp.KeyDown();		 //신바람 도우미
		cSinWarpGate.KeyDown();  //신바람 워프게이트
		cHelpPet.KeyDown();      //신바람 키다운
		cSinSod2.KeyDown();		 //신바람 Sod2
		chaSiege.KeyDown();

		if (sinGetKeyClick('K'))
		{
			if (!WarMode.fOpen)
				WarMode.Open();
			else
				WarMode.Close();
		}

		if (sinGetKeyClick('M'))
		{
			CMINIMAPHANDLE->bLargeMiniMap = !CMINIMAPHANDLE->bLargeMiniMap;
			CMINIMAPHANDLE->iMiniMapSize = CMINIMAPHANDLE->bLargeMiniMap ? MINIMAP_LARGE_SIZE : MINIMAP_SMALL_SIZE;
			CMINIMAPHANDLE->OnMiniMapSizeChanged();
		}

		if (sinGetKeyClick('X')) {
			cHelpPet.PetMessage("*System", 3);
			if (!ExitButtonClick)ExitButtonClick = 1;
			else ExitButtonClick = 0;
			cInterFace.CheckAllBox(SIN_SYSTEM); //창을 하나만 띄운다 
		}
		if (sinGetKeyClick('R')) {
			cHelpPet.PetMessage("*RunAndWalk", 3);
			if (cInterFace.sInterFlags.RunFlag)cInterFace.sInterFlags.RunFlag = 0;
			else cInterFace.sInterFlags.RunFlag = 1;
		}
		if (sinGetKeyClick(VK_TAB)) {
			cHelpPet.PetMessage("*Mini_Map", 3);
			if (cInterFace.sInterFlags.MapOnFlag)cInterFace.sInterFlags.MapOnFlag = 0;
			else cInterFace.sInterFlags.MapOnFlag = 1;
		}
		if (sinGetKeyClick('Z')) {
			cInterFace.sInterFlags.CameraAutoFlag++;
			if (cInterFace.sInterFlags.CameraAutoFlag > 2)cInterFace.sInterFlags.CameraAutoFlag = 0;

		}
		if (sinGetKeyClick('D')) {
			cHelpPet.PetMessage("*Party", 3);
			ShowParty();
		}
		//if (sinGetKeyClick('Q')) {
		//	cHelpPet.PetMessage("*Quest", 3);
		//	ShowQuest();
		//}

		if (sinGetKeyClick('Q')) {

			if (QuestWindow::GetInstance()->openOverlay) {
				QuestWindow::GetInstance()->openOverlay = false;
			}
			else {
				QuestWindow::GetInstance()->openOverlay = true;
			}					
		}
	

		if (sinGetKeyClick(VK_SPACE)) { //창을 닫아준다 
			cMessageBox.CloseMessage();
			if (!cCraftItem.OpenFlag && !cAging.OpenFlag && !SmeltingItem.OpenFlag && !ManufactureItem.m_OpenFlag || !cMixtureReset.OpenFlag) //에이징과 믹스아이템이 닫혀있을때만 ESC가 먹는다 // 석지용 - 믹스쳐 리셋 창 추가
				cInterFace.CheckAllBox(SIN_ALL_CLOSE); //창을 다 닫아준다 

		}
		if (smConfig.DebugMode) { // 수치를 보기위한 테스트 키    
			if (sinGetKeyClick('T') && sinGetKeyClick(VK_CONTROL)) {
				if (sinTestFlag)sinTestFlag = 0;
				else sinTestFlag = 1;
			}

			if (sinGetKeyClick('F') && sinGetKeyClick(VK_CONTROL)) { //가짜 상점  
				if (cShop.OpenFlag == SIN_CLOSE)cShop.OpenFlag = SIN_OPEN;
				else cShop.OpenFlag = SIN_CLOSE;

			}
			if (sinGetKeyClick('G') && sinGetKeyClick(VK_CONTROL)) { //가짜 조합 
				if (cCraftItem.OpenFlag == SIN_CLOSE)cCraftItem.OpenFlag = SIN_OPEN;
				else cCraftItem.OpenFlag = SIN_CLOSE;
			}
			if (sinGetKeyClick('H') && sinGetKeyClick(VK_CONTROL)) { //가짜 에이징 
				if (cCraftItem.ForceFlag == SIN_CLOSE)cCraftItem.ForceFlag = SIN_OPEN;
				else cCraftItem.ForceFlag = SIN_CLOSE;
			}
			//if(sinGetKeyClick('J') && sinGetKeyClick(VK_CONTROL) ){ //가짜 창고 
			//	if(cCraftItem.ForceFlag== SIN_CLOSE)cCraftItem.ForceFlag = SIN_OPEN; 
			//	else cCraftItem.ForceFlag = SIN_CLOSE;
			//}
			//if(sinGetKeyClick('K') && sinGetKeyClick(VK_CONTROL) ){ //가짜 스킬  
			//	if(SkillMasterFlag== SIN_CLOSE)SkillMasterFlag = SIN_OPEN; 
			//	else SkillMasterFlag = SIN_CLOSE;
			//}

		}
#ifdef _SINBARAM_TEST
		sinTestKey(); //테스트 키 (빌드시에는 실행하지 않는다)
#endif


		break;

	}

}

/*----------------------------------------------------------------------------*
*							    게임 종료 									  *
*-----------------------------------------------------------------------------*/
void sinClose()
{
	//ktj : 웹db용 임시설정함수.  =============================== start
	void clanMENU_close();
	clanMENU_close();
	//=========================================================== end

	CloseSub();			 //서브메인 종료 
	cInvenTory.Close();  //인벤토리 종료 
	cCharStatus.Close(); //캐릭터 스테이터스 종료 
	cInterFace.Close();  //인터페이스 종료 
	cItem.Close();       //아이템 종료
	CSKILL->Close();		 //스킬창 종료 
	cShop.Close();		 //상점 종료 
	cTrade.Close();
	cMessageBox.Close(); //메세지 박스 종료 
	cSin3D.Close();
	cSinHelp.Close();    //신바람 도우미
	cSinWarpGate.Close();//신바람 워프게이트
	cHelpPet.Close();	 //신바람 펫도우미
	cSinSod2.Close();	 //신바람 Sod2
	//박철호 : 2005-11-21 오후 9:00:31
	cPCBANGPet.Close();
}

