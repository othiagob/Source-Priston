#include "smd3d.h"


smSTAGE_OBJECT::smSTAGE_OBJECT()
{
	int x,y;
	nObj = 0;

	for(y=0;y<MAP_SIZE;y++) {
		for(x=0;x<MAP_SIZE;x++) {
			ObjectMap[x][y] = 0;
		}
	}

	SumCnt = 0;
}

smSTAGE_OBJECT::~smSTAGE_OBJECT()
{
	int cnt;

	for( cnt=nObj-1;cnt>=0;cnt--) {
		if ( mObj[cnt].Pattern ) delete mObj[cnt].Pattern;
	}

}

int smSTAGE_OBJECT::AddObject( smPAT3D *Pat, int x, int y, int z , int angX, int angY, int angZ )
{
	int w,h;
	smSTAGE_OBJ3D *obj;

	if ( nObj>=MAX_STAGEOBJ ) return FALSE;

	mObj[ nObj ].Pattern = Pat;

	mObj[ nObj ].Posi.x = x;
	mObj[ nObj ].Posi.y = y;
	mObj[ nObj ].Posi.z = z;

	mObj[ nObj ].Angle.x = angX;
	mObj[ nObj ].Angle.y = angY;
	mObj[ nObj ].Angle.z = angZ;

	mObj[ nObj ].sum = 0;
	mObj[ nObj ].LinkObj = 0;
	mObj[ nObj ].LastDrawTime = GetCurrentTime();

	w = (x>>(6+FLOATNS))&0xFF;
	h = (z>>(6+FLOATNS))&0xFF;

	if ( ObjectMap[w][h]==0 ) {
		ObjectMap[w][h] = &mObj[nObj];
	}
	else {
		obj = ObjectMap[w][h];
		while( obj!=0 ) {
			if ( obj->LinkObj==0 ) break;
			obj = obj->LinkObj;
		}
		obj->LinkObj = &mObj[nObj];
	}

	nObj++;

	return TRUE;
}

//오브젝트 삽입 자동좌표 ( 맵에 삽입된 오브젝트 )
int smSTAGE_OBJECT::AddObject( smPAT3D *Pat )
{
	int w,h;
	smSTAGE_OBJ3D *obj;
	int x,y,z;

	if ( nObj>=MAX_STAGEOBJ ) return FALSE;

	mObj[ nObj ].Pattern = Pat;

	x = Pat->obj3d[0]->Tm._41;
	y = Pat->obj3d[0]->Tm._43;
	z = Pat->obj3d[0]->Tm._42;

	mObj[ nObj ].Posi.x = x;
	mObj[ nObj ].Posi.y = y;
	mObj[ nObj ].Posi.z = z;

	mObj[ nObj ].Angle.x = 0;
	mObj[ nObj ].Angle.y = 0;
	mObj[ nObj ].Angle.z = 0;

	mObj[ nObj ].sum = 0;
	mObj[ nObj ].LinkObj = 0;
	mObj[ nObj ].LastDrawTime = GetCurrentTime();

	w = (x>>(6+FLOATNS))&0xFF;
	h = (z>>(6+FLOATNS))&0xFF;

	if ( ObjectMap[w][h]==0 ) {
		ObjectMap[w][h] = &mObj[nObj];
	}
	else {
		obj = ObjectMap[w][h];
		while( obj!=0 ) {
			if ( obj->LinkObj==0 ) break;
			obj = obj->LinkObj;
		}
		obj->LinkObj = &mObj[nObj];
	}

	nObj++;

	return TRUE;
}


//오브젝트 삽입 자동좌표 ( 맵에 삽입된 오브젝트 )
int smSTAGE_OBJECT::AddObjectFile( char *szFile , char *szBipFile )
{
	int w,h;
	smSTAGE_OBJ3D *obj;
	smPAT3D *Pat;
	smPAT3D *BipPat;
	int x,y,z;

	if ( nObj>=MAX_STAGEOBJ ) return FALSE;

	if ( szBipFile ) {
		//######################################################################################
		//작 성 자 : 오 영 석
		smASE_SetPhysique( 0 );
		//######################################################################################

		BipPat = smASE_ReadBone( szBipFile );
		smASE_SetPhysique( BipPat );
		if ( !BipPat ) return FALSE;
	}
	else {
		smASE_SetPhysique( 0 );
		BipPat = 0;
	}

	Pat = smASE_Read( szFile );
	if ( !Pat ) {
		if ( BipPat ) {
			delete BipPat;
			BipPat = 0;
		}
		return FALSE;
	}

	mObj[ nObj ].Pattern = Pat;
	mObj[ nObj ].BipPattern = BipPat;

	x = Pat->obj3d[0]->Tm._41;
	y = Pat->obj3d[0]->Tm._43;
	z = Pat->obj3d[0]->Tm._42;

	mObj[ nObj ].Posi.x = x;
	mObj[ nObj ].Posi.y = y;
	mObj[ nObj ].Posi.z = z;

	mObj[ nObj ].Angle.x = 0;
	mObj[ nObj ].Angle.y = 0;
	mObj[ nObj ].Angle.z = 0;

	mObj[ nObj ].sum = 0;
	mObj[ nObj ].LinkObj = 0;
	mObj[ nObj ].LastDrawTime = GetCurrentTime();

	w = (x>>(6+FLOATNS))&0xFF;
	h = (z>>(6+FLOATNS))&0xFF;

	if ( ObjectMap[w][h]==0 ) {
		ObjectMap[w][h] = &mObj[nObj];
	}
	else {
		obj = ObjectMap[w][h];
		while( obj!=0 ) {
			if ( obj->LinkObj==0 ) break;
			obj = obj->LinkObj;
		}
		obj->LinkObj = &mObj[nObj];
	}

	nObj++;

	return TRUE;
}





int smSTAGE_OBJECT::Draw( smSTAGE_OBJ3D *mobj ) { return TRUE; }


int smSTAGE_OBJECT::Draw( int x, int y, int z, int ax, int ay, int az ) { return TRUE; }


int smSTAGE_OBJECT::Draw2( int x, int y, int z, int ax, int ay, int az ) { return TRUE; }


//######################################################################################
//작 성 자 : 오 영 석
int smSTAGE_OBJECT::DrawOpening( smSTAGE_OBJ3D *mobj, int FrameStep ) { return TRUE; }


int smSTAGE_OBJECT::DrawOpening( int x, int y, int z, int ax, int ay, int az, int FrameStep ) { return TRUE; }
//######################################################################################