/*----------------------------------------------------------------------------*
*	파일명 :  sin3D.cpp	
*	하는일 :  3D관련 작업을 한다
*	작성일 :  최종업데이트 2002년 1월
*	적성자 :  박상열 
*-----------------------------------------------------------------------------*/	

#include "sinLinkHeader.h"

#define  _ASSA_EFFECT_START_
/*----------------------------------------------------------------------------*
*								전역변수 			
*-----------------------------------------------------------------------------*/	
cSIN3D cSin3D;

int sinStarParticleKey = 0;
int TestCount2 = 0;
/*----------------------------------------------------------------------------*
*							클래스 초기, 종료 
*-----------------------------------------------------------------------------*/	
cSIN3D::cSIN3D()
{

}
cSIN3D::~cSIN3D()
{

}
/*----------------------------------------------------------------------------*
*							     초기화 
*-----------------------------------------------------------------------------*/	
void cSIN3D::Init()
{
	char *PartBlue		= "PartBlue";
	char *PartGreen		= "PartGreen";
	char *PartRed		= "PartRed";
	char *Partyellow	= "Partyellow";
	char szPath[128];

	sinEffect2D_Line = CreateTextureMaterial( "Image\\SinImage\\Effect\\Return1.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );
	
	//파티클 Blue Red Green 을 로드한다 
	for(int i=1; i < 5 ; i++){
		wsprintf(szPath,"image\\Sinimage\\Effect\\Particle\\%s%d.tga",Partyellow,i);
		MatParticleGold[i-1] = CreateTextureMaterial( szPath , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	}
	
	Rage_Of_SinBaRam = smASE_Read("Image\\SinImage\\Effect\\test.ase");
	//넓게 퍼지는 원 
	MatWideCircle = CreateTextureMaterial( "image\\Sinimage\\Effect\\Effect2D\\WideCircle.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	//MatEffectLine = CreateTextureMaterial( "image\\Sinimage\\Effect\\Effect2D\\Line.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );
	MatEffectLine = CreateTextureMaterial( "image\\Sinimage\\Effect\\Effect2D\\YellowLine.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	MatParticle1 = CreateTextureMaterial( "image\\Sinimage\\Effect\\Particle\\Particle1.tga" , 0, 0, 0,0, SMMAT_BLEND_LAMP );

	InitInterEffect();
	InitEffect2();
	sinInitSkillEffect();
	sinInitPublicEffect();
#ifdef   _ASSA_EFFECT_START_	//AssaEffect 시작
	InitAssaEffect(); //아싸이펙트 초기화 
#endif
	InitYameEffect();
	

}
/*----------------------------------------------------------------------------*
*						    Bmp파일을 로드한다 
*-----------------------------------------------------------------------------*/	
void cSIN3D::Load()
{

}
/*----------------------------------------------------------------------------*
*						     Release
*-----------------------------------------------------------------------------*/	
void cSIN3D::Release()
{

}
/*----------------------------------------------------------------------------*
*							     그리기  
*-----------------------------------------------------------------------------*/	
void cSIN3D::Draw(int x, int y,int z,int ax, int ay, int az)
{ // x,y,z 카메라 좌표  ax , ay , az 카메라 각도 

	//smRender.SetCameraPosi
	//smRender.GetDepthZ

	POINT3D Posi = {x,y,z}; 
	POINT3D Angle = { ax , ay , az};
	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
#ifdef   _ASSA_EFFECT_START_	//AssaEffect 시작
	DrawAssaEffect(&Posi,&Angle); 
#endif
	sinDrawEffect(x,y,z,ax,ay,az); 
	sinDrawEffect2(x,y,z,ax,ay,az);
	DrawYameEffect(&Posi,&Angle); 
	cHelpPet.Draw(&Posi , &Angle); //펫을 그린다

	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	
	
	////////TEST 버텍스 드로우 
	//TestVertexDraw(cSin3D.MatWideCircle,(int)chrPlayer.pX,(int)chrPlayer.pY+10000,(int)chrPlayer.pZ,1000,128);

	
}
/*----------------------------------------------------------------------------*
*							     메인  
*-----------------------------------------------------------------------------*/	
void cSIN3D::Main()
{
	sinActiveEffect(); 
	sinActiveInterEffect();
	InterEffectAging2Process(); //에이징 이펙트를 실행한다 

	sinActiveEffect2(); //이펙트를 움직인다 
	
	
#ifdef   _ASSA_EFFECT_START_	//AssaEffect 시작
	MainAssaEffect(); //아싸 이펙트 메인 
#endif
	MainYameEffect();

}
/*----------------------------------------------------------------------------*
*							     종료 
*-----------------------------------------------------------------------------*/	
void cSIN3D::Close()
{
	CloseYameEffect();
}
/*----------------------------------------------------------------------------*
*							   LButtonDown
*-----------------------------------------------------------------------------*/	
void cSIN3D::LButtonDown(int x , int y)
{


}
/*----------------------------------------------------------------------------*
*							   LButtonUp
*-----------------------------------------------------------------------------*/	
void cSIN3D::LButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonDown
*-----------------------------------------------------------------------------*/	
void cSIN3D::RButtonDown(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							   RButtonUp
*-----------------------------------------------------------------------------*/	
void cSIN3D::RButtonUp(int x , int y)
{

}
/*----------------------------------------------------------------------------*
*							    KeyDown
*-----------------------------------------------------------------------------*/	

void cSIN3D::KeyDown()
{
	POINT3D Posi;
	Posi.x = (int)lpCurPlayer->pX;
	Posi.y = (int)lpCurPlayer->pY+13000;
	Posi.z = (int)lpCurPlayer->pZ;
		
	/////////////////// TEST EFFECTKEY

	if(smConfig.DebugMode){      
		ParticleTestKey(); //파티클 테스트 Key
		sinSkillTestKey(); //스킬 이펙트 테스트 Key
		sinPublicEffectTestKey(); //공용 이펙트 테스트 Key

		#ifdef   _ASSA_EFFECT_START_	//AssaEffect 시작
			AssaEffectKeyDown(); //아싸 이펙트 테스트 Key
		#endif
			YameEffectKeyDown(); //Yame 이펙트 Keydown
		

//		if(VRKeyBuff['8']){
			//sinSetParticleTail(Posi);
			//setBossMonsterEffect2(Posi);
			//sinStartEffect(SIN_EFFECT_RAGE_OF_SINBARAM,(int)chrPlayer.pX,(int)chrPlayer.pY+10000,(int)chrPlayer.pZ);

//		}

		if(sinGetKeyClick('A')){   
			sinReleaseEffect(); 
			TestCount2 = 0;
			//SetInterEffectAging();
			//sinStartEffect(SIN_EFFECT_RAGE_OF_SINBARAM,(int)chrPlayer.pX,(int)chrPlayer.pY+10000,(int)chrPlayer.pZ);
			

		}
		//if(sinGetKeyClick('9')){   
		//	CheeseTestEffectImage();
			//Sword_Blast_Effect(lpCurPlayer,lpCurPlayer);
			//setBossMonsterEffect2(Posi); 
			//setBossMonsterEffect(lpCurPlayer,0);
			//sinSetEffect2(SIN_EFFECT_BOSSAURA , lpCurPlayer , SIN_EFFECT_NO_TIME);
			
			//sinSetParticleHit(Posi);

			//setBossMonsterEffect(&chrPlayer,0);  
			//setBossMonsterEffect2(&chrPlayer,0);  

			//sinSetEffect_IceVigo(&chrPlayer,SIN_EFFECT_NO_TIME);
			//sinSetEffect2(SIN_EFFECT_BOSSAURA , &chrPlayer , SIN_EFFECT_NO_TIME);
		//	setBossMonsterEffect(&chrPlayer,2);
/*		
			switch(TestCount2){
				case 0:
					sinSetEffect2(SIN_EFFECT_STUN,&chrPlayer,3*70);
				break;
				case 1:
					sinSetEffect_IceVigo(&chrPlayer,100*70);

				break;
				case 2:
					sinSetEffect2(SIN_EFFECT_BOSSAURA , &chrPlayer , SIN_EFFECT_NO_TIME);
					
				break;
			}
			TestCount2++;
*/			
		//}
	}
}

/*----------------------------------------------------------------------------*
*							  이미지 그리기 
*-----------------------------------------------------------------------------*/	
int cSIN3D::sinDrawTexture(int x ,int y, int z , int AngleX, int AngleY ,int AngleZ , smFACE2D *lpFace,int sinAngle)
{
	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	GRAPHICDEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	GRAPHICDEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	smRender.Init();

	smMaterialGroup->smMaterial[lpFace->MatNum].TwoSide = FALSE;

	smRender.SetMaterialGroup(smMaterialGroup);



	smMaterialGroup->SetMaterialAnimFrame(&smMaterial[lpFace->MatNum], 2);


	smRender.SetCameraPosi(x, y, z, AngleX, AngleY, AngleZ);

	if (sinAngle)
	{
		if (smRender.AddFace2D(lpFace, sinAngle))
		{
			smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
			smRender.ClipRendFace();
			smRender.GeomVertex2D(FALSE);
			smRender.RenderD3D();

		}

	}
	else
	{
		if (smRender.AddFace2D(lpFace))
		{
			smRender.SetClipStates(SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);
			smRender.ClipRendFace();
			smRender.GeomVertex2D(FALSE);
			smRender.RenderD3D();

		}
	}

	return TRUE;
}

