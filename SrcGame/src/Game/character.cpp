#include "globals.h"
#include "Utils\common.h"

#include "smlib3d\\smd3d.h"
#include "smwsock.h"

#define LOG_THE_PK

#include "character.h"
#include "srcsound\\dxwav.h"
#include "particle.h"
#include "fileread.h"
#include "playmain.h"
#include "drawsub.h"
#include "netplay.h"
#include "field.h"
#include "hobaram\\holinkheader.h"
#include "sinbaram\\sinlinkheader.h"

#include "effectsnd.h"
#include "playsub.h"
#include "skillsub.h"

#include <vector>

#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "damage.h"
#include "srcServer\onserver.h"
#include "FullZoomMap.h"
#include "Utils\\Geometry.h"
#include "Engine/DynamicAnimation/AnimationHandler.h"
#include "Montarias\\CMountHandler.h"

#define DISP_DAMAGE

#ifdef	DISP_DAMAGE
//int rsDisplayDamgeToClient(rsPLAYINFO* lpPlayInfo, char* szDmgMsg);
#endif

#define		SUCCUBUS_CURSE_TIME		10
#define		PKMODE_ABSORB			30

extern BYTE VRKeyBuff[256];
extern int MouseButton[3];
extern DWORD dwPlayServTime;

static int MatMBlur;

static smTEXLINK smTexLinkMotionBlur[128];
static smVERTEX MbVertex[64];
static smFACE MbFace[64];

static smTEXLINK smTexLinkMotionBlur2[64];
static smVERTEX MbVertex2[32];
static smFACE MbFace2[32];

#define	AGING_SUB_LEVEL	10

int	SkillChargingFlag = 0;
int	SkillEventDamageCount;

TRANS_SKIL_ATTACKDATA	Trans_SplashCharList;

CRITICAL_SECTION	cLoadSection;

int DrawMotionBlur();

int rsCheckHardCoreEventArea(int x, int y, int z);
extern int	rsHardCoreEventInterLineZ;
extern int	rsHardCoreEventInterLineZ2;
extern int	rsHardCoreEventPosY;
extern int	rsHardCoreEventOutLineZ;
extern rsSERVER_CONFIG		rsServerConfig;










int GetProjPosi(int& x3d, int& y3d, int& z3d, POINT3D* CameraPosi, POINT3D* CameraAngle);


int	SkillShadowMasterEvent[9][7] = {
	{ 0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0 },

	{ 1,3,5,0,0,0,0 },
	{ 1,3,5,6,0,0,0 },
	{ 1,2,3,4,6,0,0 },
	{ 1,2,3,4,6,7,0 },
	{ 1,2,3,4,5,6,7 },

	{ 1,2,3,4,5,6,7 }
};

int CastleBattleZone_LineZ = -24043 * fONE;
int	CastleBattleZone_DoorLineZ = -25545 * fONE;
int	CastleBattleZone_DoorLineX[2] = { 34407 * fONE , 34695 * fONE };
int	CastleBattleZone_BridgeZ = -25258 * fONE;
int CastleSoulFountain_Area[4] = { 37040 * fONE , 37158 * fONE , -30286 * fONE, -30102 * fONE };


int GetResistancePower(int power, int ResCode)
{
	int rs, result;

	rs = lpCurPlayer->smCharInfo.Resistance[ResCode];

	if (rs < 0) rs = 0;
	if (rs >= 100) rs = 100;
	rs = 100 - rs;

	result = (power * rs) / 100;

	if (result <= 0) result = 4;

	return result;
}


int			Debug_TalkZoomMode;
int			Debug_TalkRepeatMode;
int			Debug_TalkRepeatModeType;

void DrawTalkZoom(smCHAR* lpChar, smPAT3D* lpPattern, float fx, float fy)
{
	if (lpChar == NULL)
		return;

	POINT3D TalkCameraPos;

	smOBJ3D* lpObj = lpPattern->GetObjectFromName("Head");
	if (lpObj == NULL)
	{
		TalkCameraPos.y = lpChar->PatHeight - 10 * fONE;
		TalkCameraPos.x = 0;
		TalkCameraPos.z = 0;
	}
	else
	{
		smMATRIX* mWorld;
		POINT3D angle;

		ZeroMemory(&angle, sizeof(POINT3D));
		angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

		mWorld = &lpObj->mWorld;

		AnimObjectTree(lpObj, lpChar->frame, angle.x, angle.y, angle.z);

		TalkCameraPos.x = mWorld->_41;
		TalkCameraPos.z = mWorld->_42;
		TalkCameraPos.y = mWorld->_43;
	}

	if (lpChar->smCharInfo.State == smCHAR_STATE_ENEMY)
	{
		TalkCameraPos.y += lpChar->smCharInfo.ArrowPosi[0] * fONE;
		TalkCameraPos.z += lpChar->smCharInfo.ArrowPosi[1] * fONE;
	}

	int x, y, z, w, h;
	int mx, my, he;
	POINT3D angle;
	int dist;
	int	ObjSizeBack;

	mx = MidX;
	my = MidY;

	MidX = (int)((float)WinSizeX * fx);
	MidY = (int)((float)WinSizeY * fy);

	int chrHeight;
	if (lpChar->smCharInfo.State <= TRUE)
		chrHeight = lpChar->PatHeight - 8 * fONE;
	else
		chrHeight = lpChar->PatHeight;

	w = (int)((float)WinSizeX / 4);
	h = (int)((float)WinSizeY / 2);

	w += 20;
	h += 20;

	smRender.SMSHIFT_PERSPECTIVE_WIDTH = 0;
	smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT * 2;

	dist = 30 * fONE;
	he = -1 * fONE;

	dist += TalkCameraPos.y;

	extern int	MatEachBox;
	if (MatEachBox >= 0)
	{

		dsDrawTexImage(MatEachBox, MidX - (w >> 1), MidY - (h >> 1), w, h, 255, 0);

	}

	GRAPHICDEVICE->Clear(0, 0, 2, smBackColor, 1.0f, 0);
	smCHAR_SetCameraPosi(TalkCameraPos.x, TalkCameraPos.y + he, dist + TalkCameraPos.z, 0, ANGLE_180, 0);

	x = lpChar->pX;
	y = lpChar->pY;
	z = lpChar->pZ;
	memcpy(&angle, &lpChar->Angle, sizeof(POINT3D));

	lpChar->pX = 0;
	lpChar->pY = 0;
	lpChar->pZ = 0;
	ZeroMemory(&lpChar->Angle, sizeof(POINT3D));

	ObjSizeBack = lpChar->smCharInfo.wPlayClass[1];
	lpChar->smCharInfo.wPlayClass[1] = 0;

	lpChar->Draw();

	lpChar->smCharInfo.wPlayClass[1] = ObjSizeBack;

	lpChar->pX = x;
	lpChar->pY = y;
	lpChar->pZ = z;
	memcpy(&lpChar->Angle, &angle, sizeof(POINT3D));

	MidX = mx;
	MidY = my;

	smRender.SMSHIFT_PERSPECTIVE_WIDTH = 0;
	smRender.SMMULT_PERSPECTIVE_HEIGHT = RENDCLIP_DEFAULT_MULT_PERSPECTIVE_HEIGHT;
}

int smCHAR::GetToolBipPoint(smCHARTOOL* ChrTool, int* nX, int* nY, int* nZ)
{

	smMATRIX* mWorld;
	POINT3D angle;
	int tx, ty, tz;
	smOBJ3D* lpObj;
	int pframe;
	int rx, ry, rz;

	if (!DisplayFlag) return FALSE;

	tx = 0;
	ty = 0;
	tz = 0;

	memcpy(&angle, &Angle, sizeof(POINT3D));
	angle.y = (-angle.y + ANGLE_180) & ANGCLIP;
	lpObj = ChrTool->ObjBip;

	if (lpObj) {
		mWorld = &lpObj->mWorld;
		pframe = frame;

		AnimObjectTree(lpObj, pframe, angle.x, angle.y, angle.z);

		rx = tx * mWorld->_11 +
			ty * mWorld->_21 +
			tz * mWorld->_31;

		ry = tx * mWorld->_12 +
			ty * mWorld->_22 +
			tz * mWorld->_32;

		rz = tx * mWorld->_13 +
			ty * mWorld->_23 +
			tz * mWorld->_33;

		*nX = pX + (rx >> FLOATNS) + mWorld->_41;
		*nZ = pZ + (ry >> FLOATNS) + mWorld->_42;
		*nY = pY + (rz >> FLOATNS) + mWorld->_43;

		return TRUE;
	}

	return FALSE;
}

int smCHAR::GetAttackPoint(int* nX, int* nY, int* nZ)
{

	smMATRIX* mWorld;
	POINT3D angle;
	int tx, ty, tz;
	smCHARTOOL* ChrTool = 0;
	smOBJ3D* lpObj;
	int pframe;
	int rx, ry, rz;

	ChrTool = &HvRightHand;

	if (!DisplayFlag) return FALSE;

	tx = 0;
	ty = 0;

	if (ChrTool->PatTool)
		tz = ChrTool->SizeMax / 2;
	else
		tz = 0;

	memcpy(&angle, &Angle, sizeof(POINT3D));
	angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

	if (AttackObjBip && !tz && !ShootingMode)
		lpObj = AttackObjBip;
	else
		lpObj = ChrTool->ObjBip;

	if (lpObj) {
		mWorld = &lpObj->mWorld;
		pframe = frame;

		AnimObjectTree(lpObj, pframe, angle.x, angle.y, angle.z);

		rx = tx * mWorld->_11 +
			ty * mWorld->_21 +
			tz * mWorld->_31;

		ry = tx * mWorld->_12 +
			ty * mWorld->_22 +
			tz * mWorld->_32;

		rz = tx * mWorld->_13 +
			ty * mWorld->_23 +
			tz * mWorld->_33;

		*nX = pX + (rx >> FLOATNS) + mWorld->_41;
		*nZ = pZ + (ry >> FLOATNS) + mWorld->_42;
		*nY = pY + (rz >> FLOATNS) + mWorld->_43;

		return TRUE;
	}

	return FALSE;
}


int GetSinItemNumFromCode(DWORD CODE)
{
	for (int j = 0; j < MAX_ITEM; j++) {
		if (sItem[j].CODE == CODE) {
			return j;
		}

	}
	return -1;
}

int GetDistanceDbl(int x1, int z1, int x2, int z2)
{
	int ax, az;

	ax = (x1 - x2) >> FLOATNS;
	az = (z1 - z2) >> FLOATNS;
	ax *= ax;
	az *= az;
	return ax + az;
}

int CheckLocateBox(int x1, int y1, int z1, int x2, int y2, int z2, int size)
{
	if (abs(x1 - x2) < size && abs(y1 - y2) < size && abs(z1 - z2) < size)
		return TRUE;

	return FALSE;
}


int AnimObjectTree(smOBJ3D* tObj, int frame, int ax, int ay, int az)
{
	smOBJ3D* ObjList[32];
	smOBJ3D* obj;
	int ObjCnt;
	int cnt;

	ObjCnt = 0;
	obj = tObj;

	while (1) {
		ObjList[ObjCnt++] = obj;
		if (!obj->pParent) break;
		obj = obj->pParent;
	}

	for (cnt = ObjCnt - 1; cnt >= 0; cnt--) {
		if (ObjList[cnt]->newMesh)
			ObjList[cnt]->newMesh->Animate(frame, Delta3D::Math::Vector3Int(ax, ay, az));
		else
			ObjList[cnt]->TmAnimation(frame, ax, ay, az);
	}


	return TRUE;
}


char* szBipName_RightHand = "Bip weapon01";//"Bip01 R Hand";//
char* szBipName_LeftHand = "Bip01 L Hand";
char* szBipName_Shield = "Bip01 L Forearm";
char* szBipName_Shield2 = "Bip-shield";
char* szBipName_BackSpine = "Bip in01";
char* szBipName_BackSpineCross = "Bip in-bow";
char* szBipName_BackSpineBow = "Bip in-cro";
char* szBipName_RFoot = "Bip01 R Foot";
char* szBipName_LFoot = "Bip01 L Foot";
char* szBipName_Head = "Bip01 Head";

//char *szBipName_BackSpine = "Bip01 Neck";

int smCHAR_CameraX;
int smCHAR_CameraY;
int smCHAR_CameraZ;

int smCHAR_CameraAngX;
int smCHAR_CameraAngY;
int smCHAR_CameraAngZ;


int smCHAR_SetCameraPosi(int x, int y, int z, int angX, int angY, int angZ)
{
	smCHAR_CameraX = x;
	smCHAR_CameraY = y;
	smCHAR_CameraZ = z;

	smCHAR_CameraAngX = angX;
	smCHAR_CameraAngY = angY;
	smCHAR_CameraAngZ = angZ;

	return TRUE;
}

DWORD GetSpeedSum(char* szName)
{
	DWORD cnt;
	DWORD Sum1, Sum2;
	BYTE ch;
	DWORD  dwSum;

	Sum2 = 0;
	dwSum = 0;

	cnt = 0;

	while (1) {
		ch = (BYTE)szName[cnt];
		if (ch == 0) break;
		if (ch >= 'a' && ch <= 'z') {
			Sum2 += (ch - 0x20) * (cnt + 1);
			dwSum += (ch - 0x20) * (cnt * cnt);
		}
		else {
			Sum2 += (ch * (cnt + 1));
			dwSum += ch * (cnt * cnt);
		}
		cnt++;
	}

	Sum1 = cnt;

	return (dwSum << 24) | (Sum1 << 16) | Sum2;
}


smPATTERN::smPATTERN(void)
{
	Init();
}


smPATTERN::~smPATTERN(void)
{
	Close();
}
void smPATTERN::Init()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++)
		ZeroMemory(&DinaPat[cnt], sizeof(smDPAT));

	BipPattern = 0;
}

int smPATTERN::Close()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat) {
			delete DinaPat[cnt].Pat;
			DinaPat[cnt].Pat = 0;
			if (DinaPat[cnt].lpModelInfo) {
				delete DinaPat[cnt].lpModelInfo;
				DinaPat[cnt].lpModelInfo = 0;
			}
		}
	}

	return TRUE;
}


int smPATTERN::FindFromName(char* szName)
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat) {
			if (lstrcmpi(szName, DinaPat[cnt].szPatName) == 0) {
				return cnt;
			}
		}
	}

	return -1;
}

int smPATTERN::FindFromCode(DWORD dwCode)
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat && dwCode == DinaPat[cnt].dwSpeedFindSum)
			return cnt;
	}

	return -1;
}

int smPATTERN::GetNew()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat == 0)
			return cnt;
	}

	return -1;
}

smDPAT* smPATTERN::LoadBipPattern(char* szFileName)
{
	int Num;
	DWORD dwCode;

	dwCode = GetSpeedSum(szFileName);
	Num = FindFromCode(dwCode);

	if (Num < 0) {
		//AddChatBuff( szFileName );

		Num = GetNew();
		if (Num >= 0) {
			smRender.LoadUsingNewRender = true;
			DinaPat[Num].Pat = smASE_ReadBone(szFileName);
			smRender.LoadUsingNewRender = false;
			if (DinaPat[Num].Pat) {
				DinaPat[Num].smDinaLink = 0;
				lstrcpy(DinaPat[Num].szPatName, szFileName);
				DinaPat[Num].dwSpeedFindSum = dwCode;
				DinaPat[Num].UseCount = 1;
				return &DinaPat[Num];
			}
		}
		else
			return NULL;
	}

	DinaPat[Num].UseCount++;

	return &DinaPat[Num];
}

smDPAT* LoadINX(char* szFileName)
{
	return smPattern.LoadCharactor(szFileName);
}

smDPAT* smPATTERN::LoadCharactor(char* szFileName)
{
	int Num, sframe;
	DWORD cnt;
	smDPAT* dMotionPat;
	smPAT3D* AnimPat;
	DWORD	dwCode;

	smMODELINFO	modelInfo;


	//Num = FindFromName( szFileName );
	dwCode = GetSpeedSum(szFileName);
	Num = FindFromCode(dwCode);//GetSpeedSum(szFileName) );

	if (Num < 0) {

		if (smModelDecode(szFileName, &modelInfo) == NULL)
			return NULL;

		//AddChatBuff( szFileName );

		smASE_SetPhysique(0);

		Num = GetNew();

		if (Num < 0) return NULL;

		if (BipPattern && modelInfo.szMotionFile[0]) {
			dMotionPat = BipPattern->LoadBipPattern(modelInfo.szMotionFile);
			if (dMotionPat) {
				smASE_SetPhysique(dMotionPat->Pat);
				DinaPat[Num].smDinaLink = dMotionPat;
			}
			else return NULL;
		}
		else
			DinaPat[Num].smDinaLink = 0;

		//######################################################################################
		//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
		if (BipPattern && modelInfo.szTalkMotionFile[0])
		{
			smDPAT* lpTalkLink = BipPattern->LoadBipPattern(modelInfo.szTalkMotionFile);
			if (lpTalkLink)
				DinaPat[Num].lpTalkLink = lpTalkLink;
			else
				return NULL;
		}
		else
			DinaPat[Num].lpTalkLink = NULL;
		//######################################################################################

		smRender.LoadUsingNewRender = true;
		DinaPat[Num].Pat = smASE_Read(modelInfo.szModelFile);
		smRender.LoadUsingNewRender = false;

		if (!DinaPat[Num].Pat)
		{
			if (smConfig.DebugMode)
			{
				CHATGAMEHANDLE->AddChatBoxTextEx(EChatColor::CHATCOLOR_Blue, "Loading Error , ( %s )", modelInfo.szModelFile);
			}
			return NULL;
		}

		if (DinaPat[Num].smDinaLink)
			AnimPat = DinaPat[Num].smDinaLink->Pat;
		else
			AnimPat = DinaPat[Num].Pat;

		//¿¡´Ï¸ÞÀÌ¼Ç ÇÁ·¹ÀÓ º¸Á¤
		if (AnimPat && AnimPat->TmFrameCnt > 0) {
			for (cnt = 0; cnt < modelInfo.MotionCount; cnt++) {
				sframe = AnimPat->TmFrame[modelInfo.MotionInfo[cnt].MotionFrame - 1].StartFrame / 160;
				modelInfo.MotionInfo[cnt].StartFrame += sframe;
				modelInfo.MotionInfo[cnt].EndFrame += sframe;
			}
		}

		DinaPat[Num].lpModelInfo = new smMODELINFO;
		memcpy(DinaPat[Num].lpModelInfo, &modelInfo, sizeof(smMODELINFO));
		lstrcpy(DinaPat[Num].szPatName, szFileName);
		DinaPat[Num].dwSpeedFindSum = dwCode;
		DinaPat[Num].UseCount = 1;

		if (DinaPat[Num].Pat)
		{
			if (DinaPat[Num].Pat->newModel)
			{
				//Tudo foi carregado corretamente? Então verificar os grupos (LOD)
				DinaPat[Num].sModelsGroup = new smDPAT::ModelsGroup();

				if (DinaPat[Num].sModelsGroup)
				{
					//High
					if (modelInfo.HighModel.ModelNameCnt > 0)
					{
						for (int i = 0; i < 4; i++)
						{
							if (modelInfo.HighModel.szModelName[i][0] != 0)
							{
								for (const auto& mesh : DinaPat[Num].Pat->newModel->GetMeshes(modelInfo.HighModel.szModelName[i]))
								{
									if (mesh)
										DinaPat[Num].sModelsGroup->sHighModel.meshes.push_back(mesh);
								}
							}
						}
					}

					//Low
					if (modelInfo.LowModel.ModelNameCnt > 0)
					{
						for (int i = 0; i < 4; i++)
						{
							if (modelInfo.LowModel.szModelName[i][0] != 0)
							{
								for (const auto& mesh : DinaPat[Num].Pat->newModel->GetMeshes(modelInfo.LowModel.szModelName[i]))
								{
									if (mesh)
										DinaPat[Num].sModelsGroup->sLowModel.meshes.push_back(mesh);
								}
							}
						}
					}

					//Default
					if (modelInfo.DefaultModel.ModelNameCnt > 0)
					{
						for (int i = 0; i < 4; i++)
						{
							if (modelInfo.DefaultModel.szModelName[i][0] != 0)
							{
								for (const auto& mesh : DinaPat[Num].Pat->newModel->GetMeshes(modelInfo.DefaultModel.szModelName[i]))
								{
									if (mesh)
										DinaPat[Num].sModelsGroup->sDefaultModel.meshes.push_back(mesh);
								}
							}
						}
					}
				}
			}
		}

		auto ret = &DinaPat[Num];
		if (ret)
			AnimationHandler::Get().HandleBoneCache(ret);

		return ret;
	}

	DinaPat[Num].UseCount++;

	auto ret = &DinaPat[Num];
	if (ret)
		AnimationHandler::Get().HandleBoneCache(ret);

	return ret;
}


extern smPAT3D* PatKnife;
char* szDefaultCharName = "char\\woman.ini";

int InitLoader();
int CreateLoaderThread();
DWORD WINAPI smLoaderThreadProc(void* pInfo);

struct smDATALOADER {
	smSTAGE3D* lpStage;
	smCHAR* lpChar;
	scITEM* lpItem;


	char		szLoadName[64];
	char		szLoadName2[64];

	int			Result;
	int			Cancel;
};

#define DATALOADER_MAX		64
#define DATALOADER_MASK		63

smDATALOADER	smLoader[DATALOADER_MAX];
int	smLoaderCnt;
int smLoaderEnd;
int	ThreadLoaderStats;

smPATTERN smBipPattern;
smPATTERN smPattern;


int AddLoaderStage(smSTAGE3D* lpStage, char* szName)
{
	int cnt;

	cnt = smLoaderEnd & DATALOADER_MASK;

	smLoaderEnd++;

	smLoader[cnt].lpStage = lpStage;
	smLoader[cnt].lpChar = 0;
	smLoader[cnt].lpItem = 0;
	smLoader[cnt].Cancel = 0;
	smLoader[cnt].Result = 0;
	lstrcpy(smLoader[cnt].szLoadName, szName);

	CreateLoaderThread();

	return smLoaderEnd;
}

int	AddLoaderPattern(smCHAR* lpChar, char* szName, char* szName2)
{
	int cnt;

	cnt = smLoaderEnd & DATALOADER_MASK;

	smLoaderEnd++;

	smLoader[cnt].lpStage = 0;
	smLoader[cnt].lpChar = lpChar;
	smLoader[cnt].lpItem = 0;
	smLoader[cnt].Cancel = 0;
	smLoader[cnt].Result = 0;
	strcpy_s(smLoader[cnt].szLoadName, szName);

	if (szName2)
		strcpy_s(smLoader[cnt].szLoadName2, szName2);
	else
		smLoader[cnt].szLoadName2[0] = 0;

	smLoader[cnt].lpChar->PatLoading = TRUE;

	CreateLoaderThread();

	return TRUE;
}


int	AddLoaderItem(scITEM* lpItem, char* szName)
{
	int cnt;

	cnt = smLoaderEnd & DATALOADER_MASK;

	smLoaderEnd++;

	smLoader[cnt].lpStage = 0;
	smLoader[cnt].lpChar = 0;
	smLoader[cnt].lpItem = lpItem;
	smLoader[cnt].Cancel = 0;
	smLoader[cnt].Result = 0;
	lstrcpy(smLoader[cnt].szLoadName, szName);

	smLoader[cnt].lpItem->PatLoading = TRUE;

	CreateLoaderThread();

	return TRUE;
}


DWORD WINAPI smLoaderThreadProc(void* pInfo)
{

	smDPAT* smDPat = nullptr;
	smDPAT* smDPat2 = nullptr;

	smDPAT* lpPatOld = nullptr;
	smDPAT* lpPatOld2 = nullptr;

	EnterCriticalSection(&cLoadSection);

	int cnt = smLoaderCnt & DATALOADER_MASK;

	if (smLoader[cnt].lpStage)
	{
		if (smSTAGE3D_ReadASE(smLoader[cnt].szLoadName, smLoader[cnt].lpStage))
		{
			if (GRAPHICDEVICE)
			{
				smLoader[cnt].lpStage->smMaterialGroup->ReadTextures(1);

				smLoader[cnt].lpStage->smMaterialGroup->CheckMatreialTextureSwap();
			}

			smLoader[cnt].Result = TRUE;
			smLoader[cnt].lpStage->Head = TRUE;
		}

		CloseDispLoading();
	}


	if (smLoader[cnt].lpItem)
	{
		smLoader[cnt].lpItem->PatLoading = TRUE;


		lpPatOld = smLoader[cnt].lpItem->lpDinaPattern;
		smDPat = smPattern.LoadCharactor(smLoader[cnt].szLoadName);

		if (smDPat)
		{
			if (GRAPHICDEVICE && smDPat->UseCount == 1)
			{
				if (smDPat && smDPat->Pat)
				{
					if (smDPat->Pat->smMaterialGroup)
					{
						smDPat->Pat->smMaterialGroup->ReadTextures();
						smDPat->Pat->smMaterialGroup->CheckMatreialTextureSwap();
					}
				}
			}

			smLoader[cnt].lpItem->DisplayFlag = 0;

			smLoader[cnt].lpItem->SetPattern(smDPat->Pat);
			smLoader[cnt].lpItem->lpDinaPattern = smDPat;
			strcpy_s(smLoader[cnt].lpItem->szModelName, sizeof(smLoader[cnt].lpItem->szModelName), smDPat->szPatName);
			smLoader[cnt].lpItem->DisplayFlag = TRUE;
		}

		smLoader[cnt].Result = TRUE;

		if (smDPat && lpPatOld)
			smDPAT_Delete(lpPatOld);

		smLoader[cnt].lpItem->PatLoading = FALSE;
	}


	if (smLoader[cnt].lpChar)
	{
		smDPat = 0;
		smDPat2 = 0;

		smLoader[cnt].lpChar->PatLoading = TRUE;


		lpPatOld = smLoader[cnt].lpChar->lpDinaPattern;
		lpPatOld2 = smLoader[cnt].lpChar->lpDinaPattern2;

		smDPat = smPattern.LoadCharactor(smLoader[cnt].szLoadName);

		if (smDPat)
		{
			smLoader[cnt].lpChar->DisplayFlag = 0;
			smLoader[cnt].lpChar->SetDinaPattern(smDPat);
			smLoader[cnt].lpChar->SetMotionFromCode(CHRMOTION_STATE_STAND);

			smLoader[cnt].lpChar->Flag = 1;
			strcpy_s(smLoader[cnt].lpChar->smCharInfo.szModelName, sizeof(smLoader[cnt].lpChar->smCharInfo.szModelName), smDPat->szPatName);

			smLoader[cnt].lpChar->smCharInfo.szModelName2[0] = 0;

			if (smDPat->lpModelInfo->szSubModelFile[0])
			{
				smDPat2 = smPattern.LoadCharactor(smDPat->lpModelInfo->szSubModelFile);
			}
			else
			{
				if (smLoader[cnt].szLoadName2[0])
				{
					smDPat2 = smPattern.LoadCharactor(smLoader[cnt].szLoadName2);
					strcpy_s(smLoader[cnt].lpChar->smCharInfo.szModelName2, smDPat2->szPatName);
				}
			}

			smLoader[cnt].lpChar->SetDinaPattern2(smDPat2);


			if (GRAPHICDEVICE && (smDPat->UseCount == 1 || (smDPat2 && smDPat2->UseCount == 1)))
			{
				if (smDPat && smDPat->Pat)
				{
					if (smDPat->Pat->smMaterialGroup)
					{
						smDPat->Pat->smMaterialGroup->ReadTextures();
						smDPat->Pat->smMaterialGroup->CheckMatreialTextureSwap();
					}
				}
				if (smDPat2 && smDPat2->Pat)
				{
					if (smDPat2->Pat->smMaterialGroup)
					{
						smDPat2->Pat->smMaterialGroup->ReadTextures();
						smDPat2->Pat->smMaterialGroup->CheckMatreialTextureSwap();
					}
				}
			}

			smLoader[cnt].lpChar->DisplayFlag = TRUE;
		}

		smLoader[cnt].Result = TRUE;

		if (lpPatOld)
		{
			smDPAT_Delete(lpPatOld);
		}

		if (lpPatOld2)
		{
			smDPAT_Delete(lpPatOld2);
		}

		smLoader[cnt].lpChar->PatLoading = FALSE;
	}

	smLoaderCnt++;

	LeaveCriticalSection(&cLoadSection);

	ThreadLoaderStats = FALSE;


	return TRUE;
}

int CreateLoaderThread()
{
	smLoaderThreadProc(0);
	return FALSE;
}

int InitLoader()
{
	smLoaderCnt = 0;
	smLoaderEnd = 0;

	return TRUE;
}

int InitPatterns()
{
	smBipPattern.Init();
	smPattern.Init();

	smPattern.BipPattern = &smBipPattern;

	InitLoader();
	InitializeCriticalSection(&cLoadSection);

	return TRUE;
}

int ClosePatterns()
{
	smBipPattern.Close();
	smPattern.Close();

	return TRUE;
}

int smDPAT_Delete(smDPAT* dPat)
{
	if (dPat->UseCount > 0) {
		dPat->UseCount--;

		if (dPat->lpTalkLink && dPat->UseCount == 0)
			smDPAT_Delete(dPat->lpTalkLink);

		if (dPat->smDinaLink && dPat->UseCount == 0)
			smDPAT_Delete(dPat->smDinaLink);

		if (dPat->UseCount <= 0) {
			delete dPat->Pat;
			dPat->Pat = 0;
			if (dPat->lpModelInfo) {
				delete dPat->lpModelInfo;
				dPat->lpModelInfo = 0;
			}
		}
	}
	return NULL;
}


int SetLoadPattern(smCHAR* smChar, char* szName, char* szName2, POINT3D* Posi, POINT3D* Angle)
{
	smDPAT* smDPat;
	smDPAT* smDPat2;

	smDPat = 0;
	smDPat2 = 0;

	smDPat = smPattern.LoadCharactor(szName);

	if (smDPat) {
		smChar->Init();
		if (Posi && Angle)
			smChar->SetPosi(Posi->x, Posi->y, Posi->z, Angle->x, Angle->y, Angle->z);

		smChar->SetDinaPattern(smDPat);
		smChar->ChangeMotion(0);
		smChar->DisplayFlag = TRUE;
		lstrcpy(smChar->smCharInfo.szModelName, smDPat->szPatName);

		smChar->smCharInfo.szModelName2[0] = 0;

		if (smDPat->lpModelInfo->szSubModelFile[0]) {
			smDPat2 = smPattern.LoadCharactor(smDPat->lpModelInfo->szSubModelFile);
		}
		else {
			if (szName2 && szName2[0]) {
				smDPat2 = smPattern.LoadCharactor(szName2);
				lstrcpy(smChar->smCharInfo.szModelName2, smDPat2->szPatName);
			}
		}

		smChar->SetDinaPattern2(smDPat2);

		return TRUE;
	}

	return FALSE;
}


int SendShowDmg(int Dmg, Type Type, DWORD objectSerial = lpCurPlayer->smCharInfo.dwObjectSerial)
{
	smTRANS_COMMAND smTransCommand;
	smTransCommand.code = SMTRANSCODE_DISPLAY_DAMAGE;
	smTransCommand.size = sizeof(smTRANS_COMMAND);
	smTransCommand.WParam = lpCurPlayer->smCharInfo.dwObjectSerial;
	smTransCommand.LParam = Dmg;
	smTransCommand.SParam = (int)Type;

	if (smWsockServer)
		smWsockServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

	return TRUE;
}

int SetLoadPattern(smCHAR* smChar, char* szName, POINT3D* Posi, POINT3D* Angle)
{

	return SetLoadPattern(smChar, szName, 0, Posi, Angle);
}

smCHAR::smCHAR()
{
	Pattern = 0;
	Pattern2 = 0;
	PlayEvasionMastery = 0;

	PlayCursePvP = 0; //rei pvp xxstr
	PlayCurseTopLVL = 0; // top lvl server xxstr
	PlayerPvPMode = 0; // Modo PVP

	Init();
}

smCHAR::~smCHAR()
{

}

void smCHAR::Init()
{

	TalkPattern = NULL;
	TalkFrame = TalkSoundSum = 0;
	lpTalkModelParent = NULL;
	lpTalkMotionInfo = NULL;

	::ZeroMemory(&FrameInfo, sizeof(SMotionStEndInfo));

	frame = 0;
	action = 0;
	Flag = 0;
	pX = pY = pZ = 0;
	Angle.x = Angle.y = Angle.z = 0;
	MoveFlag = FALSE;
	FrameCnt = 0;
	Counter = 0;

	frame = 0;

	ZeroMemory(PlayBuff, sizeof(smPLAYBUFF) * PLAYBUFF_SIZE);
	PlayBuffCnt = 0;
	PlayBuffPosi_End = 0;

	TransMinCnt = 8;
	TransLevel = 0;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 5;

	lpDinaPattern = 0;
	lpDinaPattern2 = 0;
	PatLoading = 0;

	lpDinaLeftPattern = 0;
	lpDinaRightPattern = 0;

	HvLeftHand.PatTool = 0;
	HvRightHand.PatTool = 0;
	HvLeftHand.dwItemCode = 0;
	HvRightHand.dwItemCode = 0;

	HvLeftHand.ColorBlink = 0;
	HvRightHand.ColorBlink = 0;

	wStickItems[0] = -1;
	wStickItems[1] = -1;
	wStickItems[2] = -1;
	wStickItems[3] = -1;

	DisplayFlag = 0;
	AnimDispMode = 0;
	PatDispMode = DISP_MODE_PATMAIN;
	MotionSelectFrame = 0;

	smMotionInfo = 0;
	smMotionInfo2 = 0;

	DisplayTools = TRUE;

	dwTarget = FALSE;

	if (smStage) lpStage = smStage;

	ptNextTarget.x = 0;
	ptNextTarget.y = 0;
	ptNextTarget.z = 0;

	lpTargetPlayInfo = 0;
	lpTargetChar = 0;

	lpMasterPlayInfo = 0;

	smCharInfo.Life[0] = 100;
	smCharInfo.Life[1] = 100;
	smCharInfo.State = 0;

	dwLastTransTime = dwPlayTime;

	FrameCounter = 0;

	PatWidth = 0;
	PatHeight = 0;
	PatSizeLevel = 0;

	FrameStep = 80;
	MoveSpeed = 250;
	AttackSpeed = fONE;

	lpExt1 = 0;
	lpExt2 = 0;
	lpExt3 = 0;
	lpExt4 = 0;

	dwChatMessageTimer = dwPlayTime;

	srTransBuffSize = 0;
	dwActionItemCode = 0;

	ShootingMode = FALSE;
	ShootingFlag = FALSE;
	chrAttackTarget = 0;

	AttackToolRange = 0;
	AttackAnger = 0;
	AttackIce = 0;
	OnStageField = -1;
	OnStageFieldState = 0;
	OldMotionState = 0;

	OverLapPosi = 0;
	OverLapPosi2 = 0;
	lpCharOverLap = 0;

	TargetMoveCount = 0;

	AttackObjBip = 0;

	NearPlayCount = 256;
	AttackCritcal = 0;

	AutoMoveStep = 0;

	LevelLog = 0;

	dwNextMotionCode = 0;
	PosBeginCount = 0;

	EnableStateBar = 0;
	DisplayAlpha = 0;
	RendAlpha = 0;
	AttackSkil = 0;
	AttackEffect = 0;

	dwDispAppSkill = 0;
	dwDispAppSkillMask = 0;

	HoSkillCode = 0;
	PartyFlag = 0;
	dwAttackPlayTime = 0;

	PlaySlowCount = 0;
	PlaySlowSpeed = 0;
	PlayStunCount = 0;
	PlayStopCount = 0;
	ServerCode = 0;

	PlayHolyMind[1] = 0;
	PlayHolyPower[1] = 0;
	PlayPoison[1] = 0;
	DispPoison = 0;
	PlayHolyIncantation[1] = 0;
	PlayVanish = 0;
	PlayCurseQuest = 0;
	PlayVague = 0;
	PlayDistortion = 0;
	PlayInvincible = 0;

	dwForceOfNatureTime = 0;
	dwHallOfValhallaTime = 0;

	AttackSkillRange = 0;

	RecvExp = 0;

	sAddColorEffect[0] = 0;
	sAddColorEffect[1] = 0;
	sAddColorEffect[2] = 0;
	sAddColorEffect[3] = 0;

	((DWORD*)smCharInfo.wPlayClass)[0] = 0;

	RenderLatter = FALSE;
	dwUpdateCharInfoTime = 0;

	dwTradeMsgCode = 0;
	EventAttackCount = 0;

	dwClanManageBit = 0;

	lpAttackDamageList = 0;
	lpAttackDamageList_BlessCastle = 0;

	dwForceOrbCode = 0;
	dwForceOrbTime = 0;

	dwLifeBoosterCode = 0;
	dwLifeBoosterTime = 0;

	dwManaBoosterCode = 0;
	dwManaBoosterTime = 0;

	dwStaminaBoosterCode = 0;
	dwStaminaBoosterTime = 0;

	dwSkillDelayCode = 0;
	dwSkillDelayTime = 0;

	dwLinkObjectCode = 0;
	lpLinkPlayInfo = 0;
	lpLinkChar = 0;

	PkMode_CharState = 0;
	dwClanCode = 0;
	UseObject_VirtualLife = 0;
	PlayCursePvP = 0; // rei pvp xxstr
	PlayCurseTopLVL = 0; // top lvl server xxstr
	PlayerPvPMode = 0; // Movo PVP
	PlayEvasionMastery = 0;

#ifndef _W_SERVER

	WeaponEffect = 0;
	dwWeaponEffectTime = 0;

	ClanInfoNum = -1;
	Clan_CastleMasterFlag = 0;
	dwClanInfoTime = 0;
	TryAttackCount = 0;

	((DWORD*)sBlessCastle_Damage)[0] = 0;

#else
	dwTargetLockTime = 0;

	lpCompulsionPlayInfo = 0;
	dwCompulsionTime = 0;

	dwAssassinEyeTime = 0;
	AssassinEyeParam = 0;

	sMoveStepCount[0] = 0;
	sMoveStepCount[1] = 0;

	DontMoveFlag = 0;

	Clan_CastleMasterFlag = 0;


#endif

	pMount = nullptr;
	iMountID = 0;

}

extern Graphics::Material* itemAgingMaterial[30];
extern Graphics::Material* itemMixingMaterial[20];
extern Graphics::Material* dropItemMaterial;

int smCHAR::Close()
{
	DissolveAmount = 0.f;
	Flag = 0;

	cMountManager.DelMount(this);

	pMount = nullptr;
	iMountID = 0;

	if (lpDinaPattern2) {
		AddRemoveDynPattern(lpDinaPattern2);
		lpDinaPattern2 = 0;
	}

	if (lpDinaPattern) {
		AddRemoveDynPattern(lpDinaPattern);
		lpDinaPattern = 0;
	}

	if (lpDinaLeftPattern) {
		AddRemoveDynPattern(lpDinaLeftPattern);
		lpDinaLeftPattern = 0;
	}

	if (lpDinaRightPattern) {
		AddRemoveDynPattern(lpDinaRightPattern);
		lpDinaRightPattern = 0;
	}


	Pattern = 0;
	Pattern2 = 0;

	HvLeftHand.PatTool = 0;
	HvRightHand.PatTool = 0;

	smMotionInfo = 0;
	smMotionInfo2 = 0;

	dwDispAppSkill = 0;

	if (lpAttackDamageList) {
		delete	lpAttackDamageList;
		lpAttackDamageList = 0;
	}

	return TRUE;
}


int smCHAR::SetPattern(smPAT3D* pat)
{
	smPAT3D* AnimPat;

	Pattern = pat;

	if (pat->TmParent) AnimPat = pat->TmParent;
	else AnimPat = pat;

	HvRightHand.ObjBip = AnimPat->GetObjectFromName(szBipName_RightHand);

	if ((HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1)
		HvLeftHand.ObjBip = AnimPat->GetObjectFromName(szBipName_Shield);
	else
		HvLeftHand.ObjBip = AnimPat->GetObjectFromName(szBipName_LeftHand);


	BackObjBip[0] = AnimPat->GetObjectFromName(szBipName_BackSpine);
	BackObjBip[1] = AnimPat->GetObjectFromName(szBipName_BackSpineBow);
	BackObjBip[2] = AnimPat->GetObjectFromName(szBipName_BackSpineCross);
	ShieldObjBip = AnimPat->GetObjectFromName(szBipName_Shield2);



	AttackObjBip = 0;

	if (smCharInfo.JOB_CODE == 3 || smCharInfo.JOB_CODE == 5)
		AttackObjBip = AnimPat->GetObjectFromName(szBipName_LFoot);

	if (smCharInfo.JOB_CODE == 7)
		AttackObjBip = AnimPat->GetObjectFromName(szBipName_Head);

	AnimPattern = AnimPat;

	PatHeight = pat->SizeHeight;
	PatWidth = (pat->SizeWidth * 160) >> FLOATNS;

	return TRUE;
}

int smCHAR::SetPattern2(smPAT3D* pat)
{
	Pattern2 = pat;

	return TRUE;
}

// Posição das Garras
DWORD	dwItemCodeFromVillage[] = {
	sinWC1 | sin01 , sinWC1 | sin02 , sinWC1 | sin03 , sinWC1 | sin04 , sinWC1 | sin05 ,
	sinWC1 | sin06 , sinWC1 | sin07 , sinWC1 | sin08 , sinWC1 | sin09 , sinWC1 | sin10 ,
	sinWC1 | sin11 , sinWC1 | sin12 , sinWC1 | sin13 , sinWC1 | sin14 , sinWC1 | sin15 ,
	sinWC1 | sin16 , sinWC1 | sin17 , sinWC1 | sin18 , sinWC1 | sin19 , sinWC1 | sin20 ,
	sinWC1 | sin21 , sinWC1 | sin22 , sinWC1 | sin23 , sinWC1 | sin24 , sinWC1 | sin25 ,
	sinWC1 | sin26 , sinWC1 | sin27 , sinWC1 | sin28 , sinWC1 | sin29 , sinWC1 | sin30 ,

	sinWM1 | sin01 , sinWM1 | sin02 , sinWM1 | sin03 , sinWM1 | sin06 ,

	sinWS2 | sin01 , sinWS2 | sin02 , sinWS2 | sin03 ,

	sinWS1 | sin02, sinWS1 | sin03,
	sinWS1 | sin09, sinWS1 | sin29,
	0
};

// Posição dos Arcos (Besta)
DWORD	dwItemCodeFromVillage_Bow[] = {
	sinWS1 | sin04, sinWS1 | sin08, sinWS1 | sin10, sinWS1 | sin13, sinWS1 | sin17, sinWS1 | sin20,
	0
};

// Posição dos Arcos (Normais)
DWORD	dwItemCodeFromVillage_Cross[] = {
	sinWS1 | sin01, sinWS1 | sin05, sinWS1 | sin06, sinWS1 | sin07,
	sinWS1 | sin11, sinWS1 | sin12, sinWS1 | sin14, sinWS1 | sin15,
	sinWS1 | sin16, sinWS1 | sin18, sinWS1 | sin19,
	sinWS1 | sin21, sinWS1 | sin22, sinWS1 | sin23, sinWS1 | sin24,
	sinWS1 | sin25, sinWS1 | sin26, sinWS1 | sin27, sinWS1 | sin28,
	sinWS1 | sin30, sinWS1 | sin31, sinWS1 | sin32,

	0
};



int smCHAR::SetTool(DWORD dwItemCode, int hvPosi)
{
	char szToolMeshName[64];
	int cnt, szX, szN;

	szX = szN = 0;

	if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER) return FALSE;

	if (dwItemCode)
		sinGetItemInfo(dwItemCode, szToolMeshName);

	switch (hvPosi) {
	case hvPOSI_RHAND:
		if (HvRightHand.ObjBip) {
			if (lpDinaRightPattern) {
				AddRemoveDynPattern(lpDinaRightPattern);
				lpDinaRightPattern = 0;
			}
			if (dwItemCode) {
				lpDinaRightPattern = smPattern.LoadCharactor(szToolMeshName);
				if (lpDinaRightPattern) {
					HvRightHand.PatTool = lpDinaRightPattern->Pat;
					for (cnt = 0; cnt < lpDinaRightPattern->Pat->nObj3d; cnt++) {
						if (szX < lpDinaRightPattern->Pat->obj3d[cnt]->maxY)
							szX = lpDinaRightPattern->Pat->obj3d[cnt]->maxY;
						if (szN < lpDinaRightPattern->Pat->obj3d[cnt]->minY)
							szN = lpDinaRightPattern->Pat->obj3d[cnt]->minY;
					}
					HvRightHand.SizeMax = szX;
					HvRightHand.SizeMin = szN;
					HvRightHand.dwItemCode = dwItemCode;
					HvRightHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_RightHand);
					dwActionItemCode = dwItemCode;

					wStickItems[smPLAYBUFF_ITEM_RIGHT] = GetSinItemNumFromCode(dwItemCode);
				}
				else {
					HvRightHand.PatTool = 0;
					HvRightHand.dwItemCode = 0;
					wStickItems[smPLAYBUFF_ITEM_RIGHT] = -1;
				}
			}
			else {
				HvRightHand.PatTool = 0;
				HvRightHand.dwItemCode = 0;
				wStickItems[smPLAYBUFF_ITEM_RIGHT] = -1;
			}
		}
		else
			return FALSE;
		break;

	case hvPOSI_LHAND:
		if (HvLeftHand.ObjBip) {
			if (lpDinaLeftPattern) {
				AddRemoveDynPattern(lpDinaLeftPattern);
				lpDinaLeftPattern = 0;
			}
			if (dwItemCode) {
				lpDinaLeftPattern = smPattern.LoadCharactor(szToolMeshName);
				if (lpDinaLeftPattern) {
					HvLeftHand.PatTool = lpDinaLeftPattern->Pat;
					for (cnt = 0; cnt < lpDinaLeftPattern->Pat->nObj3d; cnt++) {
						if (szX < lpDinaLeftPattern->Pat->obj3d[cnt]->maxY)
							szX = lpDinaLeftPattern->Pat->obj3d[cnt]->maxY;
						if (szN < lpDinaLeftPattern->Pat->obj3d[cnt]->minY)
							szN = lpDinaLeftPattern->Pat->obj3d[cnt]->minY;
					}
					HvLeftHand.SizeMax = szX;
					HvLeftHand.SizeMin = szN;
					HvLeftHand.dwItemCode = dwItemCode;
					wStickItems[smPLAYBUFF_ITEM_LEFT] = GetSinItemNumFromCode(dwItemCode);

					if ((dwItemCode & sinITEM_MASK2) == sinDS1 || (dwItemCode & sinITEM_MASK2) == sinOM1) {
						if ((dwItemCode & sinITEM_MASK2) == sinDS1) {
							HvLeftHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_Shield);
						}
						else {
							HvLeftHand.PatTool = 0;
							HvLeftHand.dwItemCode = 0;
							wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
						}
					}
					else {
						HvLeftHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_LeftHand);
						dwActionItemCode = dwItemCode;
					}
				}
				else {
					HvLeftHand.PatTool = 0;
					HvLeftHand.dwItemCode = 0;
					wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
				}
			}
			else {
				HvLeftHand.PatTool = 0;
				HvLeftHand.dwItemCode = 0;
				wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
			}

		}
		else
			return FALSE;
		break;
	}

	if (dwActionItemCode) {
		if (dwActionItemCode != HvLeftHand.dwItemCode && dwActionItemCode != HvRightHand.dwItemCode) {
			dwActionItemCode = 0;
		}

		if (dwActionItemCode == HvLeftHand.dwItemCode) {
			AttackToolRange = (HvLeftHand.SizeMax * 200) >> 8;
		}
		if (dwActionItemCode == HvRightHand.dwItemCode) {
			AttackToolRange = (HvRightHand.SizeMax * 200) >> 8;
		}

		cnt = 0;
		dwItemSetting = 1;

		while (1) {
			if (dwItemCodeFromVillage[cnt] == 0) break;
			if (dwItemCodeFromVillage[cnt] == dwActionItemCode) {
				dwItemSetting = 0;
				break;
			}
			cnt++;
		}

		cnt = 0;
		while (1) {
			if (dwItemCodeFromVillage_Bow[cnt] == 0) break;
			if (dwItemCodeFromVillage_Bow[cnt] == dwActionItemCode) {
				dwItemSetting = 2;
				break;
			}
			cnt++;
		}

		cnt = 0;
		while (1) {
			if (dwItemCodeFromVillage_Cross[cnt] == 0) break;
			if (dwItemCodeFromVillage_Cross[cnt] == dwActionItemCode) {
				dwItemSetting = 3;
				break;
			}
			cnt++;
		}

	}
	else
		AttackToolRange = 0;


	if ((dwActionItemCode & sinITEM_MASK2) == sinWS1 || (dwActionItemCode & sinITEM_MASK2) == sinWT1 ||
		((dwActionItemCode & sinITEM_MASK2) == sinWM1 && (smCharInfo.JOB_CODE == 7 || smCharInfo.JOB_CODE == 8))) {
		ShootingMode = TRUE;
	}
	else
		ShootingMode = FALSE;

	if ((smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) && dwItemCode == dwActionItemCode) {
		if (MotionInfo->State != CHRMOTION_STATE_DEAD && MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
			MotionInfo->State != CHRMOTION_STATE_RESTART) {
			SetMotionFromCode(MotionInfo->State);
			AttackCritcal = -1;
		}

		if (dwItemCode) {
			for (cnt = 0; cnt < MAX_ITEM; cnt++) {
				if (sItem[cnt].CODE == dwItemCode) {
					if (sItem[cnt].Class == ITEM_CLASS_WEAPON_TWO && !ShootingMode)
						dwActionItemTwoHand = TRUE;
					else
						dwActionItemTwoHand = FALSE;

					break;
				}
			}
		}
	}

	DisplayTools = TRUE;
	return TRUE;
}

int smCHAR::SetPosi(int x, int y, int z, int angX, int angY, int angZ)
{
	int sy;

	pX = x;
	pZ = z;
	pY = y;

	if (y == 0 && lpStage)
		pY = lpStage->GetHeight(pX, pZ);

	if (pY == 0)
	{
		if (smGameStage[0])
			pY = smGameStage[0]->GetHeight(pX, pZ);

		if (smGameStage[1])
		{
			sy = smGameStage[1]->GetHeight(pX, pZ);
			if (pY < sy) pY = sy;
		}
	}

	pY -= 2 * fONE;

	PHeight = pY;

	Angle.x = angX;
	Angle.y = angY;
	Angle.z = angZ;

	return TRUE;
}

int smCHAR::SetTargetPosi(int x, int z)
{
	tx = x << FLOATNS;
	tz = z << FLOATNS;
	MoveFlag = TRUE;

	Angle.y = GetRadian2D(pX, pZ, tx, tz);

	if (ActionPattern == 0)
	{
		if (MoveMode)
			SetMotionFromCode(CHRMOTION_STATE_RUN);
		else
			SetMotionFromCode(CHRMOTION_STATE_WALK);
	}

	return Angle.y;
}

int smCHAR::SetTargetPosi2(int x, int z)
{

	tx = x << FLOATNS;
	tz = z << FLOATNS;

	Angle.y = GetRadian2D(pX, pZ, tx, tz);

	return Angle.y;
}


int smCHAR::SetNextTarget(int x, int y, int z)
{
	ptNextTarget.x = x;
	ptNextTarget.y = y;
	ptNextTarget.z = z;

	return TRUE;
}

int smCHAR::SetAction(int nAction)
{
	if (nAction == 0) MoveFlag = FALSE;

	return TRUE;
}

int	smCHAR::SetSmoking(int level)
{
	GetMoveLocation(4 * fONE, 0, 0, 0, Angle.y, 0);
	StartEffect(pX + GeoResult_X, pY + 8 * fONE, pZ + GeoResult_Z, 20, 20, EFFECT_DUST1);

	GetMoveLocation(-4 * fONE, 0, 0, 0, Angle.y, 0);
	StartEffect(pX + GeoResult_X, pY + 8 * fONE, pZ + GeoResult_Z, 20, 20, EFFECT_DUST1);

	return TRUE;
}

int smCHAR::MoveAngle2(int step)
{
	int x, z, y;
	int he, hy;
	int OldStageField;
	POINT3D MovePosi;
	int MovLevel[2];

	WaterHeight = CLIP_OUT;

	//Aumentar speed com mount
	//speed mount
	if ((lpCurPlayer->pMount) && lpCurPlayer->pMount->IsVisible())
		step += 300;

#ifndef _W_SERVER

	if (smConfig.DebugMode && VRKeyBuff['N']) {

		x = pX + (((GetSin[Angle.y & ANGCLIP] >> FLOATNS) * step) >> FLOATNS);
		z = pZ + (((GetCos[Angle.y & ANGCLIP] >> FLOATNS) * step) >> FLOATNS);
		y = pY;

		pY = y;
		pX = x;
		pZ = z;

		return TRUE;
	}
#else
	if (DontMoveFlag) return FALSE;
#endif

	POINT3D Posi;

	he = 0;

	Posi.x = x = pX;
	Posi.y = y = pY;
	Posi.z = z = pZ;

	OldStageField = OnStageField;
	OnStageField = -1;

	if (step == 0 || PHeight < y) {
		if (lpStage) {
			he = lpStage->GetFloorHeight(x, y, z, Pattern->SizeHeight);
		}
		else {
			if (smGameStage[0]) {
				he = smGameStage[0]->GetFloorHeight(x, y, z, Pattern->SizeHeight);
				OnStageField = 0;
			}

			if (smGameStage[1]) {
				hy = smGameStage[1]->GetFloorHeight(x, y, z, Pattern->SizeHeight);
				if (he < hy) {
					he = hy;
					OnStageField = 1;
				}
			}
		}
		if (he != CLIP_OUT)
			y = he;
		else
			y = -80 * fONE;

		if ((pY - y) > 8 * fONE) {
			if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN && (pY - y) > 32 * fONE) {
				FallHeight = pY - y;
				SetMotionFromCode(CHRMOTION_STATE_FALLDOWN);
			}
			PHeight = y;
			pY -= 8 * fONE;
		}
		else {
			pY = y;
			PHeight = y;
			if (MotionInfo->State == CHRMOTION_STATE_FALLDOWN) {
				if (FallHeight > 200 * fONE)
					SetMotionFromCode(CHRMOTION_STATE_FALLDAMAGE);
				else
					SetMotionFromCode(CHRMOTION_STATE_FALLSTAND);

				SetSmoking(0);

			}
		}
		return TRUE;
	}
	else {
		if (lpStage && lpStage->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2) > 0) {
			x = MovePosi.x;
			y = MovePosi.y;
			z = MovePosi.z;
		}
		else {
			MovLevel[0] = 0;
			MovLevel[1] = 0;

			if (smGameStage[0] && (MovLevel[0] = smGameStage[0]->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2)) > 0) {

				if (smStage_WaterChk != CLIP_OUT)
					WaterHeight = smStage_WaterChk;


				if (StageField[0]->LimitLevel <= smCharInfo.Level) {

					OnStageField = 0;

					he = MovePosi.y;
					x = MovePosi.x;
					y = MovePosi.y;
					z = MovePosi.z;
				}

			}
			if (smGameStage[1] && (MovLevel[1] = smGameStage[1]->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2) > 0)) {

				if (smStage_WaterChk != CLIP_OUT)
					WaterHeight = smStage_WaterChk;

				if (StageField[1]->LimitLevel <= smCharInfo.Level) {
					if (MovLevel[0] && abs(he - MovePosi.y) < fONE * 8) {
						if (MovLevel[0] > MovLevel[1]) {
							x = MovePosi.x;
							y = MovePosi.y;
							z = MovePosi.z;
							OnStageField = 1;
						}
					}
					else {
						if (he < MovePosi.y) {
							x = MovePosi.x;
							y = MovePosi.y;
							z = MovePosi.z;
							OnStageField = 1;
						}
					}
				}
			}
		}
	}

	pX = x;
	pZ = z;

	if ((pY - y) > 8 * fONE) {

		if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN && (pY - y) > 32 * fONE) {
			FallHeight = pY - y;
			SetMotionFromCode(CHRMOTION_STATE_FALLDOWN);
		}

		PHeight = y;
		pY -= 8 * fONE;
	}
	else {
		pY = y;
		PHeight = y;
		if (MotionInfo->State == CHRMOTION_STATE_FALLDOWN) {
			if (FallHeight > 200 * fONE)
				SetMotionFromCode(CHRMOTION_STATE_FALLDAMAGE);
			else
				SetMotionFromCode(CHRMOTION_STATE_FALLSTAND);

			SetSmoking(0);

		}
	}

	if (OnStageField >= 0 && OldStageField != OnStageField) {
		OnStageFieldState = StageField[OnStageField]->State;
		if (OldStageField < 0 || OnStageFieldState != StageField[OldStageField]->State) {
			SetMotionFromCode(MotionInfo->State);
		}
	}
	if (OnStageField < 0)
		OnStageField = OldStageField;

	return TRUE;
}


int smCHAR::MoveAngle(int step)
{
	return	MoveAngle2(step * fONE);
}

int smCHAR::SetDinaPattern(smDPAT* lpDPat)
{

	TalkPattern = NULL;
	TalkFrame = TalkSoundSum = 0;
	lpTalkModelParent = NULL;
	lpTalkMotionInfo = NULL;

	if (smConfig.DebugMode)
	{
		Debug_TalkZoomMode = 0;
		Debug_TalkRepeatMode = 0;
		Debug_TalkRepeatModeType = -1;
	}

	lpDinaPattern = lpDPat;
	smMotionInfo = lpDinaPattern->lpModelInfo;
	SetPattern(lpDPat->Pat);

	SetMotionFromCode(CHRMOTION_STATE_STAND);

	return TRUE;
}

int smCHAR::SetDinaPattern2(smDPAT* lpDPat)
{
	lpDinaPattern2 = lpDPat;

	if (lpDinaPattern2) {
		smMotionInfo2 = lpDinaPattern2->lpModelInfo;
		SetPattern2(lpDPat->Pat);

		if (Pattern->TmParent == Pattern2->TmParent) {
			AnimDispMode = 0;
			PatDispMode = DISP_MODE_PATMAIN | DISP_MODE_PATSUB;
		}
		else {
			AnimDispMode = 1;
			PatDispMode = DISP_MODE_PATMAIN;
		}
	}
	else {
		AnimDispMode = 0;
		PatDispMode = DISP_MODE_PATMAIN;
		SetPattern2(0);
	}

	if (lpDinaPattern->lpTalkLink)
	{
		PatDispMode |= DISP_MODE_PATTALK;
		TalkPattern = lpDinaPattern->lpTalkLink->Pat;
	}
	else if (lpDinaPattern2 && lpDinaPattern2->lpTalkLink)
	{
		PatDispMode |= DISP_MODE_PATTALK;
		TalkPattern = lpDinaPattern2->lpTalkLink->Pat;
	}

	if (TalkPattern && (PatDispMode & DISP_MODE_PATTALK))
	{
		if (smMotionInfo->szTalkLinkFile[0])
			lpTalkModelParent = smMotionInfo;
		else if (smMotionInfo2 && smMotionInfo2->szTalkLinkFile[0])
			lpTalkModelParent = smMotionInfo2;
		else
		{
			PatDispMode &= (~DISP_MODE_PATTALK);
			TalkPattern = NULL;
		}
	}

	return TRUE;
}

int smCHAR::ChangeMotion(int Motion, int DpMode)
{
	if (DpMode) {
		if (MotionSelectFrame) {
			MotionInfo = &smMotionInfo2->MotionInfo[Motion];
			PatDispMode = DISP_MODE_PATSUB;
		}
		else {
			MotionInfo = &smMotionInfo->MotionInfo[Motion];
			PatDispMode = DISP_MODE_PATMAIN;
		}
	}
	else {
		if (AnimDispMode)
			PatDispMode = DISP_MODE_PATMAIN;

		MotionInfo = &smMotionInfo->MotionInfo[Motion];
		MotionSelectFrame = 0;
	}

	frame = MotionInfo->StartFrame * 160;
	action = Motion;
	FrameCounter = 0;

	AttackSkil = 0;
	HideWeapon = 0;
	MotionLoop = 0;

	dwEventFrameExt[0] = 0;
	dwEventFrameExt[1] = 0;
	dwEventFrameExt[2] = 0;
	dwEventFrameExt[3] = 0;

	FrameInfo.StartFrame = frame;
	FrameInfo.EndFrame = MotionInfo->EndFrame * 160;

	return TRUE;
}

#
int smCHAR::AutoChangeTalkMotion(int TalkModeType)
{
	if (TalkPattern == NULL)
		return 0;

	int TalkCnt = 0;
	if (lpTalkModelParent->TalkMotionRate[CHRMOTION_EXT])
	{
		TalkCnt = rand() % 100;
		TalkCnt = lpTalkModelParent->TalkMotionRateCnt[TalkModeType][TalkCnt];
	}
	else
		TalkCnt = rand() % lpTalkModelParent->TalkMotionCount;

	lpTalkMotionInfo = &lpTalkModelParent->TalkMotionInfo[TalkCnt];
	TalkFrame = lpTalkMotionInfo->StartFrame * 160;

	return 1;
}

int smCHAR::FindMotionCountFromCode(DWORD MotionCode)
{
	int cnt;
	int FindCnt = 0;

	for (cnt = 0; cnt < (int)smMotionInfo->MotionCount; cnt++) {
		if (smMotionInfo->MotionInfo[cnt].State == MotionCode) {
			FindCnt++;
		}
	}

	return FindCnt;
}

enum EItemType
{
	ITEMTYPE_1Hand,
	ITEMTYPE_2Hand,
};

struct ItemPivot
{
	DWORD itemID;
	EItemType eItemType;
};

std::vector<ItemPivot> vItemPivot;
BOOL bPivotItems = FALSE;

int smCHAR::SetMotionFromCode(DWORD MotionCode)
{
	int cnt, cnt2, itcnt;
	int FoundNum[64];
	int FindCnt;

	int FoundNum2[64];
	int FindCnt2;
	int StageVillage;
	int	dwActItemCode;
	int	atSkill = 0;
	int	skillCode;
	BYTE	bSkill;

	DWORD	dwJobCodeMask;

	DWORD dwFindCode;

	smMODELINFO* lpMotionInfo;

	//Esta na montaria?
	if ((pMount) && pMount->IsVisible())
	{
		if (MotionCode == CHRMOTION_STATE_DEAD)
			pMount->SetVisible(false);
		else if (MotionCode != CHRMOTION_STATE_WALK && MotionCode != CHRMOTION_STATE_RUN && MotionCode != CHRMOTION_STATE_STAND && MotionCode != CHRMOTION_STATE_YAHOO)
			return 0;
	}

	// Tabela Completa (Pivot)
	if (bPivotItems == FALSE)
	{
		// Pivot Machados
		vItemPivot.push_back({ sinWA1 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin04, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin05, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin06, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin07, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin08, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin09, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin10, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin11, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin12, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin13, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin14, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin15, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin16, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin17, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin18, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin19, ITEMTYPE_2Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWA1 | sin20, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin21, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin22, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWA1 | sin23, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin24, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin25, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin26, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin27, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin28, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin29, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWA1 | sin30, ITEMTYPE_2Hand });

		// Pivot Garras
		vItemPivot.push_back({ sinWC1 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin04, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin05, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin06, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin07, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin08, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin09, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin10, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin11, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin12, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin13, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin14, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin15, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin16, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin17, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin18, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin19, ITEMTYPE_1Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWC1 | sin20, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin21, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin22, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin23, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin24, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin25, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin26, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin27, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin28, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin29, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWC1 | sin30, ITEMTYPE_1Hand });

		// Pivot Martelos
		vItemPivot.push_back({ sinWH1 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin04, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin05, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin06, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin07, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin08, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin09, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin10, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin11, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin12, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin13, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin14, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin15, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin16, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin17, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWH1 | sin18, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin19, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin20, ITEMTYPE_1Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWH1 | sin21, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin22, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin23, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin24, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin25, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin26, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin27, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin28, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin29, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin30, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWH1 | sin31, ITEMTYPE_2Hand });

		// Pivot Cajados e Varinhas
		vItemPivot.push_back({ sinWM1 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin04, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin05, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin06, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin07, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin08, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin09, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin10, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin11, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin12, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin13, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin14, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin15, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin16, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin17, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin18, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin19, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin20, ITEMTYPE_1Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWM1 | sin21, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin22, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin23, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin24, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin25, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin26, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin27, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWM1 | sin28, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin29, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin30, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWM1 | sin31, ITEMTYPE_2Hand });

		// Pivot Lanças
		vItemPivot.push_back({ sinWT1 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin04, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin05, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin06, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin07, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin08, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin09, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin10, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin11, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin12, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin13, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin14, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin15, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin16, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin17, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin18, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin19, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin20, ITEMTYPE_1Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWT1 | sin21, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin22, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin23, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin24, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin25, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin26, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin27, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin28, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin29, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin30, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWT1 | sin31, ITEMTYPE_1Hand });

		// Pivot Espadas
		vItemPivot.push_back({ sinWS2 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin04, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin05, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin06, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin07, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin08, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin09, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin10, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin11, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin12, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin13, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin14, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin15, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin16, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin17, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin18, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin19, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin20, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin21, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin22, ITEMTYPE_1Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWS2 | sin23, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin24, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin25, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin26, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin27, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin28, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin29, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin30, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS2 | sin31, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin32, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS2 | sin33, ITEMTYPE_2Hand });

		// Pivot Arcos
		vItemPivot.push_back({ sinWS1 | sin01, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS1 | sin05, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin06, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin07, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin09, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS1 | sin11, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin12, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin14, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin15, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin16, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin18, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin19, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin21, ITEMTYPE_2Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWS1 | sin22, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin23, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin24, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin25, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin26, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin27, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin28, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin29, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWS1 | sin30, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin31, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWS1 | sin32, ITEMTYPE_2Hand });

		// Pivot Foices
		vItemPivot.push_back({ sinWP1 | sin01, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWP1 | sin02, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWP1 | sin03, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWP1 | sin04, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin05, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin06, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin07, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin08, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin09, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin10, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin11, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin12, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin13, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin14, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin15, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin16, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin17, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin18, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin19, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin20, ITEMTYPE_2Hand });
		// Limite atual do Servidor
		vItemPivot.push_back({ sinWP1 | sin21, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin22, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin23, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin24, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin25, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin26, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin27, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin28, ITEMTYPE_1Hand });
		vItemPivot.push_back({ sinWP1 | sin29, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin30, ITEMTYPE_2Hand });
		vItemPivot.push_back({ sinWP1 | sin31, ITEMTYPE_2Hand });

		bPivotItems = TRUE;
	}

	if (smCharInfo.JOB_CODE && smCharInfo.JOB_CODE < 16)
		dwJobCodeMask = BitMaskJobNames[smCharInfo.JOB_CODE - 1].dwBitMask;
	else
		dwJobCodeMask = 0;

	// Comparar Pivot
	FindCnt = 0;
	FindCnt2 = 0;
	MotionSelectFrame = 0;

	dwActItemCode = dwActionItemCode;

	if ((dwActItemCode & sinITEM_MASK2) == sinWS1)
	{
		switch (dwActItemCode & 0x0000FFFF)
		{
		case sin04:
		case sin08:
		case sin10:
		case sin13:
		case sin17:
		case sin20:
			dwActItemCode = sinWS1 | sin04;
			break;
		}
	}

	//Aplicar Pivot nos Items
	for (size_t i = 0; i < vItemPivot.size(); i++)
	{
		if (dwActionItemCode == vItemPivot[i].itemID)
		{
			DWORD itemPivot = 0;

			if (vItemPivot[i].eItemType == ITEMTYPE_1Hand)
			{
				if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWA1)
					dwActItemCode = sinWA1 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWH1)
					dwActItemCode = sinWH1 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWM1)
					dwActItemCode = sinWM1 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWP1)
					dwActItemCode = sinWP1 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWS1)
					dwActItemCode = sinWS1 | sin09;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWS2)
					dwActItemCode = sinWS2 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWT1)
					dwActItemCode = sinWT1 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWC1)
					dwActItemCode = sinWC1 | sin01;
			}
			else if (vItemPivot[i].eItemType == ITEMTYPE_2Hand)
			{
				if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWA1)
					dwActItemCode = sinWA1 | sin05;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWH1)
					dwActItemCode = sinWH1 | sin06;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWM1)
					dwActItemCode = sinWM1 | sin04;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWP1)
					dwActItemCode = sinWP1 | sin04;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWS1)
					dwActItemCode = sinWS1 | sin07;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWS2)
					dwActItemCode = sinWS2 | sin05;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWT1)
					dwActItemCode = sinWT1 | sin01;
				else if ((vItemPivot[i].itemID & sinITEM_MASK2) == sinWC1)
					dwActItemCode = sinWC1 | sin01;
			}

			break;
		}
	}

	if (OnStageField >= 0) {
		if (StageField[OnStageField]->State == FIELD_STATE_VILLAGE) {
			StageVillage = 1;
			dwActItemCode = 0;
		}
		else
			StageVillage = 2;
	}
	else {
#ifdef _W_SERVER
		StageVillage = 3;
#else
		if (GameMode == 1) StageVillage = 1;
		else StageVillage = 3;
#endif
	}

	lpMotionInfo = smMotionInfo;

	//Animacao da montaria
	if ((pMount) && pMount->IsVisible())
	{
		if (MotionCode == CHRMOTION_STATE_WALK)
			MotionCode = CHRMOTION_STATE_RUN;

		for (int i = 0; i < (int)smMotionInfo->MotionCount; i++)
		{
			if (i >= MOTION_INFO_MAX)
				break;

			if (smMotionInfo->MotionInfo[i].State == MotionCode && smMotionInfo->MotionInfo[i].ItemCodeList[0] == 0xFE)
			{
				for (int j = 1; j < smMotionInfo->MotionInfo[i].ItemCodeCount; j++)
				{
					if (smMotionInfo->MotionInfo[i].ItemCodeList[j] == pMount->GetMountID())
					{
						MotionSelectFrame = 1;
						lpMotionInfo = smMotionInfo2;
						ChangeMotion(i);
						return TRUE;
					}
				}
			}
		}

		return TRUE;
	}

	if (MotionCode == CHRMOTION_STATE_SKILL && AttackSkil) {
		atSkill = AttackSkil;
	}

	for (cnt = 0; cnt < (int)smMotionInfo->MotionCount; cnt++) {
		if (smMotionInfo->MotionInfo[cnt].State == MotionCode &&
			(!smMotionInfo->MotionInfo[cnt].MapPosition || (smMotionInfo->MotionInfo[cnt].MapPosition & StageVillage)) &&
			(!smMotionInfo->MotionInfo[cnt].dwJobCodeBit || !dwJobCodeMask ||
				(smMotionInfo->MotionInfo[cnt].dwJobCodeBit & dwJobCodeMask))) {

			if (atSkill) {
				cnt2 = 0;
				if (smCharInfo.JOB_CODE == 3 && atSkill == 403)
				{
					skillCode = 79;
				}
				else
				{
					skillCode = atSkill & 0xFF;
				}

				/*char szBuff[128];
				wsprintf(szBuff, "Debug [ %d -> %d ]", atSkill, skillCode);
				Record_ClinetLogFile(szBuff);*/

				while (1) {
					bSkill = smMotionInfo->MotionInfo[cnt].SkillCodeList[cnt2];
					if (bSkill == 0) break;
					if (SkillDataCode[bSkill].dwPlaySkillCode == skillCode) {
						break;
					}
					cnt2++;
				}
				if (bSkill) {
					FoundNum[FindCnt++] = cnt;
					if (FindCnt >= 32) break;
				}
			}
			else {
				FoundNum[FindCnt++] = cnt;
				if (FindCnt >= 32) break;
			}
		}
	}


	if (FindCnt == 0 && AnimDispMode && lpDinaPattern2) {
		for (cnt = 0; cnt < (int)smMotionInfo2->MotionCount; cnt++) {
			if (smMotionInfo2->MotionInfo[cnt].State == MotionCode &&
				(!smMotionInfo2->MotionInfo[cnt].MapPosition || (smMotionInfo2->MotionInfo[cnt].MapPosition & StageVillage))) {
				FoundNum[FindCnt++] = cnt;
				if (FindCnt >= 32) break;
			}
		}

		if (FindCnt) {
			MotionSelectFrame = 1;
			lpMotionInfo = smMotionInfo2;
		}
	}

	for (cnt = 0; cnt < FindCnt; cnt++) {
		itcnt = lpMotionInfo->MotionInfo[FoundNum[cnt]].ItemCodeCount;
		if (itcnt == 0) {
			FoundNum2[FindCnt2++] = FoundNum[cnt];
		}
		else {
			if (dwActItemCode) {
				for (cnt2 = 0; cnt2 < itcnt; cnt2++) {
					dwFindCode = lpMotionInfo->MotionInfo[FoundNum[cnt]].ItemCodeList[cnt2];
					if (dwActItemCode == (int)sItem[dwFindCode].CODE) {
						FoundNum2[FindCnt2++] = FoundNum[cnt];
						break;
					}
				}
			}
			else {
				for (cnt2 = 0; cnt2 < itcnt; cnt2++) {
					dwFindCode = lpMotionInfo->MotionInfo[FoundNum[cnt]].ItemCodeList[cnt2];
					if (dwFindCode == 0xFF) {
						FoundNum2[FindCnt2++] = FoundNum[cnt];
						break;
					}
				}
			}
		}
	}

	if (FindCnt2 == 0) {
		if (FindCnt == 1)
			ChangeMotion(FoundNum[0], AnimDispMode);
		else
			if (FindCnt)
				ChangeMotion(FoundNum[(rand() % FindCnt)], AnimDispMode);

		if (atSkill) AttackSkil = atSkill;

		return NULL;
	}

	if (FindCnt2 == 1)
		ChangeMotion(FoundNum2[0], AnimDispMode);
	else
		if (FindCnt2)
		{
			if ((FoundNum2[0] <= 35) &&
				FoundNum2[1] == 76 || FoundNum2[1] == 77 ||
				FoundNum2[1] == 79 || FoundNum2[1] == 80 ||
				FoundNum2[1] == 104 || FoundNum2[1] == 105 ||
				FoundNum2[1] == 107 || FoundNum2[1] == 108 ||
				FoundNum2[1] == 118 || FoundNum2[1] == 119 ||
				FoundNum2[1] == 146 || FoundNum2[1] == 147)
				ChangeMotion(FoundNum2[0], AnimDispMode);
			else
				ChangeMotion(FoundNum2[(rand() % FindCnt2)], AnimDispMode);
		}


	if (atSkill) AttackSkil = atSkill;

	return FindCnt;
}

void CancelBuffSkills()
{
	bool bContinue = false;

	for (int k = 0; k < 19; k++)
	{
		if (ContinueSkill[k].Flag)
		{
			if (ContinueSkill[k].CODE == SKILL_HOLY_BODY)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_HOLY_VALOR)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_GOD_BLESS)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_DRASTIC_SPIRIT)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_DIVINE_INHALATION)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_HOLY_INCANTATION)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_GODLY_SHIELD)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_GOD_BLESS)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_FALCON)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_GOLDEN_FALCON)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_RECALL_WOLVERIN)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_EVASION_MASTERY)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_FORCE_OF_NATURE)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_CONCENTRATION)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_SWIFT_AXE)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_BERSERKER)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_EXTREME_SHIELD)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_PHYSICAL_ABSORB)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_MAXIMIZE)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_METAL_ARMOR)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_SPARK_SHIELD)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_COMPULSION)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_MAGNETIC_SPHERE)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_METAL_GOLEM)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_VENOM_SPEAR)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_VANISH)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_VAGUE)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_WINDY)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_TRIUMPH_OF_VALHALLA)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_HALL_OF_VALHALLA)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_FROST_JAVELIN)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_ZENITH)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_ENCHANT_WEAPON)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_ENERGY_SHIELD)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_SPIRIT_ELEMENTAL)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_DANCING_SWORD)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_FIRE_ELEMENTAL)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_HOLY_INCANTATION)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_VIRTUAL_LIFE)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_REGENERATION_FIELD)
				bContinue = true;
			else if (ContinueSkill[k].CODE == SKILL_SUMMON_MUSPELL)
				bContinue = true;

			if (bContinue)
			{
				ContinueSkill[k].Flag = 0; ContinueSkill[k].UseTime = 0; ContinueSkill[k].CheckTime = 0;
				ContinueSkill[k].IconFlag = 0;
				ContinueSkill[k].IconPosiX = 0;
				ContinueSkill[k].IconTime = 0;
				ContinueSkill[k].IconAlpha = 0;
				ContinueSkill[k].MatIcon = 0;

				lpCurPlayer->PlayEvasionMastery = 0;

				cInvenTory.SetItemToChar();
				SendCancelSkillToServer(0, ContinueSkill[k].Point, 0, 0);

				bContinue = false;

				continue;
			}
		}
	}
}


int smCHAR::FindActionFromFrame(int sframe)
{
	DWORD cnt;
	DWORD compFrame;

	compFrame = sframe / 160;

	for (cnt = 0; cnt < smMotionInfo->MotionCount; cnt++) {
		if (smMotionInfo->MotionInfo[cnt].StartFrame<compFrame &&
			smMotionInfo->MotionInfo[cnt].EndFrame>compFrame)
			return cnt;
	}

	return -1;
}

int smCHAR::SavePlayBuff()
{

	PlayBuffCnt = (PlayBuffCnt + 1) & PLAYBUFF_MASK;

	PlayBuff[PlayBuffCnt].angle[0] = Angle.x;
	PlayBuff[PlayBuffCnt].angle[1] = Angle.y;
	PlayBuff[PlayBuffCnt].angle[2] = Angle.z;
	PlayBuff[PlayBuffCnt].angle[3] = action;

	PlayBuff[PlayBuffCnt].x = pX;
	PlayBuff[PlayBuffCnt].y = pY;
	PlayBuff[PlayBuffCnt].z = pZ;

	PlayBuff[PlayBuffCnt].frame = (MotionSelectFrame << CHAR_FRAME_SELSHIFT) | frame;

	if (chrAttackTarget)
		PlayBuff[PlayBuffCnt].dwTargetSerial = chrAttackTarget->dwObjectSerial;
	else
		PlayBuff[PlayBuffCnt].dwTargetSerial = NULL;

	memcpy(PlayBuff[PlayBuffCnt].wStickItems, wStickItems, sizeof(short) * 4);

	return PlayBuffCnt;
}

int smCHAR::SavePlayBuff2()
{

	PlayBuffCnt = (PlayBuffCnt + 1) & PLAYBUFF_MASK;

	PlayBuff[PlayBuffCnt].angle[0] = Angle.x;
	PlayBuff[PlayBuffCnt].angle[1] = Angle.y;
	PlayBuff[PlayBuffCnt].angle[2] = Angle.z;
	PlayBuff[PlayBuffCnt].angle[3] = action;

	PlayBuff[PlayBuffCnt].x = pX;
	PlayBuff[PlayBuffCnt].y = pY;
	PlayBuff[PlayBuffCnt].z = pZ;

	PlayBuff[PlayBuffCnt].frame = (MotionSelectFrame << CHAR_FRAME_SELSHIFT) | frame;

	PlayBuff[PlayBuffCnt].dwTargetSerial = NULL;

	if (lpTargetChar)
		PlayBuff[PlayBuffCnt].dwTargetSerial = lpTargetChar->dwObjectSerial;

	if (lpTargetPlayInfo)
		PlayBuff[PlayBuffCnt].dwTargetSerial = lpTargetPlayInfo->dwObjectSerial;

	memcpy(PlayBuff[PlayBuffCnt].wStickItems, wStickItems, sizeof(short) * 4);

	return PlayBuffCnt;
}

int smCHAR::ChangeMoveMode()
{
	if (MoveMode == 0) MoveMode = 1;
	else MoveMode = 0;

	if (MoveFlag) {
		if (MoveMode)
			SetMotionFromCode(CHRMOTION_STATE_RUN);
		else
			SetMotionFromCode(CHRMOTION_STATE_WALK);
	}

	return MoveMode;
}

int smCHAR::CheckShootingTest_LastAttackTime(smCHAR* lpChar)
{
	if (dwLastRecvAttackTime<dwPlayTime && (dwLastRecvAttackTime + 2000)>dwPlayTime)
		return TRUE;

	return CheckShootingTest(lpChar);
}

int smCHAR::CheckShootingTest(smCHAR* lpChrTarget)
{
	int x, y, z;
	int he;
	int step;
	int cnt;

	int angX, angY;
	int	SucessCnt;

	if (lpChrTarget->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B)
		return TRUE;

	GetRadian3D(pX, pY, pZ, lpChrTarget->pX, lpChrTarget->pY, lpChrTarget->pZ);
	angX = GeoResult_X;
	angY = GeoResult_Y;

	ShootingPosi.x = pX;
	ShootingPosi.y = pY + 34 * fONE;
	ShootingPosi.z = pZ;

	ShootingAngle.x = angX;
	ShootingAngle.y = angY;
	ShootingAngle.z = 0;

	SucessCnt = 0;

	step = 16 * fONE;

	GetMoveLocation(0, 0, step, angX, angY, 0);

	for (cnt = 0; cnt < 40; cnt++) {
		ShootingPosi.x += GeoResult_X;
		ShootingPosi.y += GeoResult_Y;
		ShootingPosi.z += GeoResult_Z;

		he = TRUE;

		if (lpStage) {
			he = lpStage->CheckVecImpact(&ShootingPosi, &ShootingAngle, step);
		}
		else {
			if (smGameStage[0]) {
				he = smGameStage[0]->CheckVecImpact(&ShootingPosi, &ShootingAngle, step);
			}

			if (he == TRUE && smGameStage[1]) {
				he = smGameStage[1]->CheckVecImpact(&ShootingPosi, &ShootingAngle, step);
			}
		}

		if (chrAttackTarget) {

			x = abs(ShootingPosi.x - lpChrTarget->pX);
			z = abs(ShootingPosi.z - lpChrTarget->pZ);
			y = abs(ShootingPosi.y - lpChrTarget->pY);

			if (x < 48 * fONE && z < 48 * fONE && y < 64 * fONE && !SucessCnt) {
				SucessCnt = cnt + 2;
			}
		}

		if (SucessCnt && SucessCnt < cnt)
			return TRUE;


		if (he == FALSE)
			return FALSE;


	}

	return TRUE;


}


int smCHAR::ShootingMain()
{

	POINT3D Posi, pos;
	int x, y, z;
	int he;
	int ShootSize;
	int step;
	int r, g, b;
	int EffectFlag;

	he = CLIP_OUT;
	EffectFlag = 0;

	ShootSize = 512;

	if ((dwActionItemCode & sinITEM_MASK2) == sinWS1)
		step = 8 * fONE;
	else
		step = 6 * fONE;

	Posi.x = x = ShootingPosi.x;
	Posi.y = y = ShootingPosi.y;
	Posi.z = z = ShootingPosi.z;

	GetMoveLocation(0, 0, step, ShootingAngle.x, ShootingAngle.y, ShootingAngle.z);
	ShootingPosi.x += GeoResult_X;
	ShootingPosi.y += GeoResult_Y;
	ShootingPosi.z += GeoResult_Z;

	if (smCharInfo.State == smCHAR_STATE_USER) {

		if (dwActionItemCode == HvRightHand.dwItemCode) {
			if (HvRightHand.PatTool && HvRightHand.ColorBlink && !HvRightHand.EffectKind) {

				r = (HvRightHand.sColors[SMC_R] >> 1) + 128;
				g = (HvRightHand.sColors[SMC_G] >> 1) + 128;
				b = (HvRightHand.sColors[SMC_B] >> 1) + 128;

				StartEffect(ShootingPosi.x, ShootingPosi.y, ShootingPosi.z,
					r, g, b, 255, EFFECT_PARTICLE_BOW1);

				EffectFlag++;
			}
		}
		if (dwActionItemCode == HvLeftHand.dwItemCode) {
			if (HvLeftHand.PatTool && HvLeftHand.ColorBlink && !HvLeftHand.EffectKind) {

				r = (HvLeftHand.sColors[SMC_R] >> 1) + 128;
				g = (HvLeftHand.sColors[SMC_G] >> 1) + 128;
				b = (HvLeftHand.sColors[SMC_B] >> 1) + 128;

				StartEffect(ShootingPosi.x, ShootingPosi.y, ShootingPosi.z,
					r, g, b, 255, EFFECT_PARTICLE_BOW1);

				EffectFlag++;
			}
		}

		switch (ShootingKind) {
		case SKILL_PLAY_FARINA:
			sinEffect_Farina_Particle(&ShootingPosi);
			break;
		}
	}

	if (ShootingCount > 60 * 2) {
		ShootingFlag = FALSE;
	}

	ShootingCount++;

	if (ShootingFlag > 1 && chrAttackTarget) {
		ShootingFlag++;
		if (ShootingFlag > 8) ShootingFlag = FALSE;

		if (EffectFlag) {
			StartEffect(chrAttackTarget->pX, ShootingPosi.y, chrAttackTarget->pZ,
				r, g, b, 255, EFFECT_PARTICLE_BOW1);
		}
		return TRUE;
	}

	if (ShootingFlag == 1 && chrAttackTarget && AttackCritcal >= 0) {

		x = abs(ShootingPosi.x - chrAttackTarget->pX);
		z = abs(ShootingPosi.z - chrAttackTarget->pZ);
		y = abs(ShootingPosi.y - chrAttackTarget->pY);

		if (x < 64 * fONE && z < 64 * fONE && y < 80 * fONE) {
			ShootingFlag++;


			if (ShootingKind == SKILL_PLAY_FARINA) {
				Posi.x = chrAttackTarget->pX;
				Posi.y = chrAttackTarget->pY + 22 * fONE;
				Posi.z = chrAttackTarget->pZ;
				sinEffect_SkillHit(SKILL_FARINA, &Posi);
				SetDynLight(chrAttackTarget->pX, chrAttackTarget->pY + 22 * fONE, chrAttackTarget->pZ, 100, 0, 0, 0, 400);
				PlayWaponSoundDirect(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 16);
			}
			else {
				if (WeaponEffect) {
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY + 22 * fONE;
					pos.z = chrAttackTarget->pZ;

					switch (WeaponEffect) {
					case (sITEMINFO_LIGHTING + 1):
						AssaParticle_LightHit1(&pos);
						SetDynLight(x, y, z, 0, 0, 100, 0, 400);
						if ((rand() % 2) == 0)
							SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING1, pX, pY, pZ);
						else
							SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING2, pX, pY, pZ);
						break;
					case (sITEMINFO_FIRE + 1):
						AssaParticle_FireHit1(&pos);
						SetDynLight(x, y, z, 100, 0, 0, 0, 400);
						if ((rand() % 2) == 0)
							SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE1, pX, pY, pZ);
						else
							SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE2, pX, pY, pZ);
						break;
					case (sITEMINFO_ICE + 1):
						AssaParticle_IceHit1(&pos);
						SetDynLight(x, y, z, 0, 0, 100, 0, 400);
						if ((rand() % 2) == 0)
							SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE1, pX, pY, pZ);
						else
							SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE2, pX, pY, pZ);
						break;
					}
				}
				else {
					if (AttackCritcal) {
						StartEffect(chrAttackTarget->pX, chrAttackTarget->pY + 22 * fONE, chrAttackTarget->pZ, EFFECT_CRITICAL_HIT1);
						SetDynLight(chrAttackTarget->pX, chrAttackTarget->pY + 22 * fONE, chrAttackTarget->pZ, 100, 0, 0, 0, 400);
						PlayWaponSoundDirect(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 16);
					}
					else {
						StartEffect(chrAttackTarget->pX, chrAttackTarget->pY + 22 * fONE, chrAttackTarget->pZ, EFFECT_NORMAL_HIT1);
						SetDynLight(chrAttackTarget->pX, chrAttackTarget->pY + 22 * fONE, chrAttackTarget->pZ, 40, 0, 0, 0, 400);
					}

					if (dwForceOrbTime > dwPlayTime) {
						pos.x = chrAttackTarget->pX;
						pos.y = chrAttackTarget->pY + 22 * fONE;
						pos.z = chrAttackTarget->pZ;

						AssaParticle_ShelltomWeaponHit(&pos);
					}

				}
			}
			PlayWaponSoundDirect(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 14);

			if (this == lpCurPlayer && AttackExp > 0) {
				AttackExp = 0;
			}

		}

	}

	return TRUE;
}

int smCHAR::ChargingSkill()
{
	DWORD dwFrame;

	if (this != lpCurPlayer) return FALSE;

	dwFrame = frame - MotionInfo->StartFrame * 160;

	if (SkillChargingFlag && MouseButton[1] == 0) SkillChargingFlag = 0;

	switch (AttackSkil & 0xFF) {
	case SKILL_PLAY_PHOENIX_SHOT:
		if (dwFrame >= MotionInfo->EventFrame[0] && dwFrame < MotionInfo->EventFrame[1]) {
			if (SkillChargingFlag == 0) {
				frame = MotionInfo->StartFrame * 160 + MotionInfo->EventFrame[1];
				ChargingFlag = 1;
			}
		}
		break;
	case SKILL_PLAY_CHARGING_STRIKE:
		if (dwFrame >= MotionInfo->EventFrame[0] && dwFrame < MotionInfo->EventFrame[1]) {
			if (SkillChargingFlag == 0) {
				frame = MotionInfo->StartFrame * 160 + MotionInfo->EventFrame[1];
				ChargingFlag = 1;
			}
		}
		break;
	}

	return TRUE;
}

int smCHAR::EventAttack(int Flag)
{

	int shootFlag;
	DWORD compFrame;
	int x, y, z;
	int wp;
	DWORD NearFrame;
	POINT3D	pos, pos2;

	int	cnt, ChrCnt;
	smCHAR* lpCharList[32];

	ChargingSkill();

	if (AttackSkil > 0 || (smCharInfo.State == smCHAR_STATE_ENEMY && MotionInfo->State == CHRMOTION_STATE_SKILL)) {
		if (Flag || ((MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) && MotionInfo->EventFrame[0])) {
			compFrame = frame - (MotionInfo->StartFrame * 160);
			if ((MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) ||
				(MotionInfo->EventFrame[1] && MotionInfo->EventFrame[1] > (compFrame - FrameStep) && MotionInfo->EventFrame[1] <= compFrame) ||
				(MotionInfo->EventFrame[2] && MotionInfo->EventFrame[2] > (compFrame - FrameStep) && MotionInfo->EventFrame[2] <= compFrame) ||
				(MotionInfo->EventFrame[3] && MotionInfo->EventFrame[3] > (compFrame - FrameStep) && MotionInfo->EventFrame[3] <= compFrame) ||

				(dwEventFrameExt[0] && dwEventFrameExt[0] > (frame - FrameStep) && dwEventFrameExt[0] <= frame) ||
				(dwEventFrameExt[1] && dwEventFrameExt[1] > (frame - FrameStep) && dwEventFrameExt[1] <= frame) ||
				(dwEventFrameExt[2] && dwEventFrameExt[2] > (frame - FrameStep) && dwEventFrameExt[2] <= frame) ||
				(dwEventFrameExt[3] && dwEventFrameExt[3] > (frame - FrameStep) && dwEventFrameExt[3] <= frame) ||

				(MotionEvent == 0 && MotionInfo->EventFrame[0] <= compFrame)) {

				MotionEvent++;

				if (this == lpCurPlayer) {

					AttackExp = 0;

					if (chrAttackTarget && abs(chrAttackTarget->smCharInfo.Level - smCharInfo.Level) <= AGING_SUB_LEVEL) {
						if (AttackCritcal == TRUE)
							sinCheckAgingLevel(SIN_AGING_CRITICAL, false);
						else
							sinCheckAgingLevel(SIN_AGING_ATTACK, false);
					}
				}

				if (EventSkill() == TRUE)
					return TRUE;
			}
		}
	}


	if (MotionInfo->State != CHRMOTION_STATE_ATTACK && MotionInfo->State != CHRMOTION_STATE_SKILL) return FALSE;

	if (ActionPattern)
		shootFlag = 0;
	else {
		shootFlag = ShootingFlag;
	}

	if (MotionInfo->EventFrame[0]) {
		compFrame = frame - (MotionInfo->StartFrame * 160);
		NearFrame = compFrame - FrameStep;

		if (Flag || (MotionInfo->EventFrame[0] > NearFrame && MotionInfo->EventFrame[0] <= compFrame) ||
			(MotionInfo->EventFrame[1] && MotionInfo->EventFrame[1] > NearFrame && MotionInfo->EventFrame[1] <= compFrame)) {

			MotionEvent++;

			if (smCharInfo.State == smCHAR_STATE_ENEMY) {
				CharPlaySound(this);
			}
			else {
				WeaponPlaySound(this);
				if (AttackCritcal < 0) return FALSE;
			}

			if (this == lpCurPlayer && AttackExp > 0) {

				AttackExp = 0;

				if (chrAttackTarget && abs(chrAttackTarget->smCharInfo.Level - smCharInfo.Level) <= AGING_SUB_LEVEL) {
					if (AttackCritcal == TRUE)
						sinCheckAgingLevel(SIN_AGING_CRITICAL, false);
					else
						sinCheckAgingLevel(SIN_AGING_ATTACK, false);
				}

			}

			if (chrAttackTarget == lpCurPlayer) {
				AttackMotionCountUp();
			}

			switch (smCharInfo.dwCharSoundCode) {
			case snCHAR_SOUND_MUSHROOM:
				StartEffect(pX, pY + (24 * fONE), pZ, EFFECT_GAS1);
				break;

			case snCHAR_SOUND_IMP:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 10 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

						if (smCharInfo.Level < 15) {
							StartEffectMonsterDest(x, y, z,
								chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
								MONSTER_IMP_SHOT1);
						}
						else {
							StartEffectMonsterDest(x, y, z,
								chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
								MONSTER_MEPHIT_SHOT1);
						}
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_SHADOW:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						StartEffectMonsterDest(pX, pY + 32 * fONE, pZ,
							chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
							MONSTER_IMP_SHOT3);
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_FIGON:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						StartEffectMonsterDest(pX, pY + 32 * fONE, pZ,
							chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
							MONSTER_PIGON_SHOT1);
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_WEB:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);
						StartEffectMonsterDest(pX + GeoResult_X, pY + 12 * fONE, pZ + GeoResult_Z,
							chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
							MONSTER_WEB_SHOT1);
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_SKELETONRANGE:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						ShootingPosi.x = pX;
						ShootingPosi.y = pY + 28 * fONE;
						ShootingPosi.z = pZ;
						GetRadian3D(pX, pY + 28 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
						ShootingAngle.x = GeoResult_X;
						ShootingAngle.y = GeoResult_Y;
						ShootingAngle.z = Angle.z;
						ShootingFlag = TRUE;
						ShootingCount = 0;
						dwActionItemCode = sinWS1;
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_WOLVERLIN:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 50 * fONE, 0, Angle.y, 0);
					x = pX + GeoResult_X;
					z = pZ + GeoResult_Z;
					y = pY + 18 * fONE;

					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);

				}
				break;

			case snCHAR_SOUND_EGAN:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						GetMoveLocation(0, 0, 10 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

						StartEffectMonsterDest(x, y, z,
							chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
							MONSTER_IMP_SHOT1);
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_MUTANTTREE:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

						sinEffect_MutentTree_Attack(this, chrAttackTarget);
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_HULK:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 48 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;


			case snCHAR_SOUND_DMACHINE:
				if (chrAttackTarget) {
					GetMoveLocation(0, 30 * fONE, 10 * fONE, 0, Angle.y, 0);
					StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
						chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
						MONSTER_DMACHINE_MISSILE1);
				}
				break;

			case snCHAR_SOUND_TURTLE_CANNON:
				if (chrAttackTarget) {
					ParkAssaParticle_MonsterTurtleCanonNormal(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_SUCCUBUS:
				if (chrAttackTarget) {

					if (smCharInfo.Level <= 75)
						wp = MONSTER_SERQBUS_SHOT1;
					else if (smCharInfo.Level < 80)
						wp = MONSTER_SERQBUS_SHOT2;
					else
						wp = MONSTER_SERQBUS_SHOT3;

					GetMoveLocation(0, 20 * fONE, 20 * fONE, 0, Angle.y, 0);
					StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
						chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
						wp);
				}
				break;

			case snCHAR_SOUND_STYGIANLORD:
				if (chrAttackTarget) {
					StartEffectMonsterDest(pX, pY, pZ,
						chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
						0, Angle.y, 0, MONSTER_STYGIANLORD_SHOT1);
				}
				break;

			case snCHAR_SOUND_MOKOVA:
			case snCHAR_SOUND_OMICRON:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 22 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;

			case snCHAR_SOUND_GUARDIAN_SAINT:
				AssaParticleClanMonsterHit2(this);
				break;

			case snCHAR_SOUND_HYPERMACHINE:
				if (chrAttackTarget) {
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY + 20 * fONE;
					pos.z = chrAttackTarget->pZ;
					AssaParticle_MonsterIronHyperHit1(this, &pos);
				}
				break;

			case snCHAR_SOUND_MOUNTAIN:
				AssaParticle_MonsterIronMountainHit1(this);
				break;

			case snCHAR_SOUND_TOWERGOLEM:
				AssaParticle_MonsterIronTowerGolemHit1(this);
				break;

			case snCHAR_SOUND_OMU:
				AssaParticle_MonsterIronOmuHit1(this);
				break;

			case snCHAR_SOUND_CHAINGOLEM:
			case snCHAR_SOUND_DEADZONE:
			case snCHAR_SOUND_IRONFIST:
			case snCHAR_SOUND_MORGON:
			case snCHAR_SOUND_RAMPAGE:
			case snCHAR_SOUND_VAMPRICBEE:
			case snCHAR_SOUND_IRONGUARD:
			case snCHAR_SOUND_GORGON:
			case snCHAR_SOUND_HOBOGOLEM:

				GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y + 32 * fONE;
				pos.z = pZ + GeoResult_Z;
				AssaParticle_MonsterIronHit1(&pos);
				break;

			case snCHAR_SOUND_GOBLINSHAMAN:
				if (chrAttackTarget)
				{
					ParkAssaParticle_MummyShot(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_GROTESQUE:
				GetMoveLocation(0, 0, 75 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y + 32 * fONE;
				pos.z = pZ + GeoResult_Z;
				AssaParticle_MonsterIronHit1(&pos);
				break;


			case snCHAR_SOUND_AVELIN_ARCHER:
				if (chrAttackTarget) {
					AssaParticle_MonsterAvelArcher(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_AVELIN_QUEEN:
				if (chrAttackTarget) {
					AssaParticle_MonsterAvelQueen(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_RUNICGUARDIAN:
				if (chrAttackTarget) {
					AssaParticle_MonsterRunicGuardianShot(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_BABEL:

				if (chrAttackTarget) {
					if (Trans_SplashCharList.code == smTRANSCODE_ATTACK_SLASH_LIST && Trans_SplashCharList.TargetCount > 1) {
						ChrCnt = 0;
						for (cnt = 0; cnt < Trans_SplashCharList.TargetCount; cnt++) {
							if (cnt >= 32) break;
							lpCharList[ChrCnt] = FindChrPlayer(Trans_SplashCharList.dwTarObjectSerial[cnt]);
							if (lpCharList[ChrCnt]) ChrCnt++;
						}
						AssaParticle_MonsterRunicGuardianBossHit(this, chrAttackTarget, lpCharList, ChrCnt);
						Trans_SplashCharList.code = 0;
					}
					else
						AssaParticle_MonsterRunicGuardianBossHit(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_SADNESS:
				if (chrAttackTarget) {
					AssaParticle_MonsterIronSadNessHit1(this);
				}
				break;

			case snCHAR_SOUND_WATERMELON:
				if (chrAttackTarget)
				{
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;
					AssaParticle_MonsterIronHit1(&pos);
				}
				break;

			case snCHAR_SOUND_COLDEYE:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + 64 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 64 * fONE;
					pos2.z = chrAttackTarget->pZ;
					ParkAssaParticle_ColdEye(&pos, &pos2);
				}
				break;

			case snCHAR_SOUND_FROZEN:
				if (chrAttackTarget) {
					ParkAssaParticle_MistIceBolt(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_FROST:
				if (chrAttackTarget && MotionInfo->KeyCode == 'U') {
					ParksinSkillEffect_AcientPiercing(this);
				}
				break;

			case snCHAR_SOUND_CHAOSCARA:
				if (chrAttackTarget) {
					ParkAssaParticle_ChaosKara1(chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_DEATHKNIGHT:
				if (MotionInfo->KeyCode == 'P') {
					if (MotionEvent == 1)
						ParkAssaParticle_Valento1(this);
					else
						ParkAssaParticle_Valento3_1(this);
				}
				else if (MotionInfo->KeyCode == 'L') {
					if (MotionEvent == 1)
						ParkAssaParticle_Valento2_1(this);
					else
						ParkAssaParticle_Valento2_2(this);
				}
				break;

			case snCHAR_SOUND_GREATE_GREVEN:
			case snCHAR_SOUND_LIZARDFOLK:
			case snCHAR_SOUND_SPIDER:
			case snCHAR_SOUND_STRIDER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 22 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;

			case snCHAR_SOUND_M_LORD:
				if (chrAttackTarget) {
					ParkAssaParticle_MummyShot(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_IGOLATION:
				if (chrAttackTarget && MotionInfo->KeyCode == 'H')
				{
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 22 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;
			case snCHAR_SOUND_NIKEN:
				if (chrAttackTarget)
				{
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY;
					pos.z = chrAttackTarget->pZ;

					AssaParticle_SnailShot(this, &pos);
				}
				break;
			case snCHAR_SOUND_XETAN:
				if (chrAttackTarget && MotionInfo->KeyCode == 'G')
				{
					GetMoveLocation(0, 40 * fONE, 20 * fONE, 0, Angle.y, 0);
					StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
						chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
						MONSTER_SERQBUS_SHOT2);
				}
				break;
			case snCHAR_SOUND_SPRIN:
				if (chrAttackTarget && (MotionInfo->KeyCode == 'G' || MotionInfo->KeyCode == 'H'))
				{
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);
						StartEffectMonsterDest(pX + GeoResult_X, pY + 12 * fONE, pZ + GeoResult_Z,
							chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
							MONSTER_WEB_SHOT1);
					}
					else {
						ResetAttackTrans();
					}
				}
				break;


			case snCHAR_SOUND_BEBECHICK:
				if (chrAttackTarget && (MotionInfo->KeyCode == 'H' || MotionInfo->KeyCode == 'J'))
				{
				}
				break;
			case snCHAR_SOUND_PAPACHICK:
				if (chrAttackTarget && MotionInfo->KeyCode == 'H')
				{
				}
				break;

			case snCHAR_SOUND_KELVEZU:
				if (chrAttackTarget) {

					if (MotionInfo->KeyCode != 'P') {
						ParkKelvezuNormal2(this);
						ParkKelvezuHit(chrAttackTarget);
					}
					else {
						ParkKelvezuNormal1(chrAttackTarget);
					}
				}
				break;

			case snCHAR_SOUND_DARKPHALANX:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 42 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;
			case snCHAR_SOUND_BLOODYKNIGHT:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 42 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;

			case snCHAR_SOUND_CHIMERA:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 42 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;

			case snCHAR_SOUND_FIREWORM:
				if (chrAttackTarget) {
					ParkAssaParticle_FireWorm(chrAttackTarget, 0);
				}
				break;
			case snCHAR_SOUND_HELLHOUND:
				if (chrAttackTarget) {
					ParkAssaParticle_HellHoundNormal(chrAttackTarget);

					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 42 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;
			case snCHAR_SOUND_DARKMAGE:
				if (chrAttackTarget) {
					ParkAssaParticle_DarkMageNormal(this, chrAttackTarget);
				}
				break;
			case snCHAR_SOUND_DARKGUARD:
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						ShootingPosi.x = pX;
						ShootingPosi.y = pY + 38 * fONE;
						ShootingPosi.z = pZ;
						GetRadian3D(pX, pY + 38 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
						ShootingAngle.x = GeoResult_X;
						ShootingAngle.y = GeoResult_Y;
						ShootingAngle.z = Angle.z;
						ShootingFlag = TRUE;
						ShootingCount = 0;
						dwActionItemCode = sinWS1;
					}
					else {
						ResetAttackTrans();
					}
				}
				break;

			case snCHAR_SOUND_TEMPLEGUARD:
				if (MotionInfo->KeyCode != 'I') {
					AssaParticle_MonsterIronMorgonHit(this);
				}
				else {
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;
					AssaParticle_MonsterIronHit1(&pos);
				}
				break;

			case snCHAR_SOUND_SETO:
				if (chrAttackTarget) {
					GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
					StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
						chrAttackTarget->pX, chrAttackTarget->pY + 20 * fONE, chrAttackTarget->pZ,
						MONSTER_PIGON_SHOT1);

					if (((dwPlayTime >> 4) & 1) == 0)
						SkillPlaySound(SKILL_SOUND_SKILL_FIREBALL1, pX, pY, pZ);
					else
						SkillPlaySound(SKILL_SOUND_SKILL_FIREBALL2, pX, pY, pZ);
				}
				break;

			case snCHAR_SOUND_KINGSPIDER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
					StartEffectMonster(pX + GeoResult_X, pY + 42 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;


			case snCHAR_SOUND_NPC_MORIF:
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

					AssaParticle_ShelltomShot(&pos, &pos2, rand() % 3);
				}
				break;

			case snCHAR_SOUND_NPC_MOLLYWOLF:
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

					AssaParticle_BamShot(&pos, &pos2);
				}
				break;

			case snCHAR_SOUND_WORLDCUP:
				if (chrAttackTarget) {
					ParkAssaParticle_WorldCupMonsterAttack(this, chrAttackTarget);
				}
				break;

			case snCHAR_SOUND_S_F_ELEMENTAL:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

					SkillArchMageFireElementalShot(&pos, &pos2);
				}
				break;

			case snCHAR_SOUND_S_METALGOLEM:
				if (chrAttackTarget) {
					if (!smCharInfo.wPlayClass[1])
						GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					else
						GetMoveLocation(0, 0, 54 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

					StartEffectMonster(pX + GeoResult_X, pY + 48 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
				}
				break;

			case snCHAR_SOUND_S_WOLVERLIN:
				if (chrAttackTarget) {
					if (!smCharInfo.wPlayClass[1])
						GetMoveLocation(0, 0, 50 * fONE, 0, Angle.y, 0);
					else
						GetMoveLocation(0, 0, 50 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

					x = pX + GeoResult_X;
					z = pZ + GeoResult_Z;
					y = pY + 18 * fONE;
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
					StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
				}
				break;

			case snCHAR_SOUND_NPC_SKILLMASTER:
				if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
					StartEffect(x, y, z, EFFECT_CRITICAL_HIT1);
					SetDynLight(x, y, z, 40, 0, 0, 0, 400);
				}
				break;

			case snCHAR_SOUND_NPC_MAGICMASTER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					StartEffectMonsterDest(pX + GeoResult_X, pY + 32 * fONE, pZ + GeoResult_Z,
						chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
						MONSTER_IMP_SHOT1);
				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_R:
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;
					AssaParticle_FireShot(&pos, &pos2);

					if ((rand() % 2) == 0)
						SkillPlaySound(SKILL_SOUND_SKILL_FIREBOLT1, pos.x, pos.y, pos.z);
					else
						SkillPlaySound(SKILL_SOUND_SKILL_FIREBOLT2, pos.x, pos.y, pos.z);

				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_G:
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;
					AssaParticle_LightShot(&pos, &pos2);

					if ((rand() % 2) == 0)
						SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING1, pos.x, pos.y, pos.z);
					else
						SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING2, pos.x, pos.y, pos.z);
				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_B:
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;
					AssaParticle_IceShot(&pos, &pos2);

					if ((rand() % 2) == 0)
						SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_ICE1, pos.x, pos.y, pos.z);
					else
						SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_ICE2, pos.x, pos.y, pos.z);

				}
				break;

			case snCHAR_SOUND_CASTLE_SOLDER_A:
			case snCHAR_SOUND_CASTLE_SOLDER_B:
			case snCHAR_SOUND_CASTLE_SOLDER_C:
				if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
					StartEffect(x, y, z, EFFECT_CRITICAL_HIT1);
					SetDynLight(x, y, z, 40, 0, 0, 0, 400);
				}
				break;
			case snCHAR_SOUND_REVIVED_ARCHER:
				if (chrAttackTarget)
				{
					if (CheckShootingTest_LastAttackTime(chrAttackTarget))
					{
						ShootingPosi.x = pX;
						ShootingPosi.y = pY + 38 * fONE;
						ShootingPosi.z = pZ;
						GetRadian3D(pX, pY + 38 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
						ShootingAngle.x = GeoResult_X;
						ShootingAngle.y = GeoResult_Y;
						ShootingAngle.z = Angle.z;
						ShootingFlag = TRUE;
						ShootingCount = 0;
						dwActionItemCode = sinWS1;
					}
					else
					{
						ResetAttackTrans();
					}
				}
				break;

			}

			if (this != lpCurPlayer) {
				PlayAttack();
			}
			if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER) return TRUE;

			if (!ShootingMode) {
				if (GetAttackPoint(&x, &y, &z) == TRUE) {
					if (WeaponEffect) {
						pos.x = x;
						pos.y = y;
						pos.z = z;

						switch (WeaponEffect) {
						case (sITEMINFO_LIGHTING + 1):
							AssaParticle_LightHit1(&pos);
							SetDynLight(x, y, z, 0, 0, 100, 0, 200);
							if ((rand() % 2) == 0)
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING1, pX, pY, pZ);
							else
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING2, pX, pY, pZ);

							break;
						case (sITEMINFO_FIRE + 1):
							AssaParticle_FireHit1(&pos);
							SetDynLight(x, y, z, 100, 0, 0, 0, 200);
							if ((rand() % 2) == 0)
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE1, pX, pY, pZ);
							else
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE2, pX, pY, pZ);
							break;
						case (sITEMINFO_ICE + 1):
							AssaParticle_IceHit1(&pos);
							SetDynLight(x, y, z, 0, 0, 100, 0, 200);
							if ((rand() % 2) == 0)
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE1, pX, pY, pZ);
							else
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE2, pX, pY, pZ);
							break;
						}
					}
					else {
						if (dwActionItemCode) {
							if (AttackCritcal == TRUE || AttackEffect) {
								StartEffect(x, y, z, EFFECT_CRITICAL_HIT1);
								SetDynLight(x, y, z, 100, 0, 0, 0, 400);
							}
							else {
								StartEffect(x, y, z, EFFECT_NORMAL_HIT1);
								SetDynLight(x, y, z, 40, 0, 0, 0, 400);
							}

						}
						else {
							StartEffect(x, y, z, EFFECT_NORMAL_HIT1);
						}

						if (dwForceOrbTime > dwPlayTime) {
							pos.x = x;
							pos.y = y;
							pos.z = z;

							AssaParticle_ShelltomWeaponHit(&pos);
						}
					}
				}
			}
			else {
				wp = (dwActionItemCode & sinITEM_MASK2);
				if (!shootFlag && chrAttackTarget) {
					GetAttackPoint(&x, &y, &z);

					if (wp == sinWS1 || wp == sinWT1) {


						if (smCharInfo.JOB_CODE == JOBCODE_ATALANTA &&
							(dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) && wp == sinWT1) {
							pos.x = x;
							pos.y = y;
							pos.z = z;
							pos2.x = chrAttackTarget->pX;
							pos2.y = chrAttackTarget->pY + 24 * fONE;
							pos2.z = chrAttackTarget->pZ;
							SkillFrostJavelin(&pos, &pos2);

							if ((rand() % 2) == 0)
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE1, pX, pY, pZ);
							else
								SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_ICE2, pX, pY, pZ);

							return TRUE;
						}

						ShootingPosi.x = x;
						ShootingPosi.y = y;
						ShootingPosi.z = z;

						if (chrAttackTarget) {
							GetRadian3D(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ);
							ShootingAngle.x = GeoResult_X;
							ShootingAngle.y = GeoResult_Y;
							ShootingAngle.z = Angle.z;
						}
						else {
							ShootingAngle.x = Angle.x;
							ShootingAngle.y = Angle.y;
							ShootingAngle.z = Angle.z;
						}

						ShootingCount = 0;
						ShootingFlag = TRUE;

						if (AttackSkil)
							ShootingKind = AttackSkil & 0xFF;
						else
							ShootingKind = TRUE;

					}

					if (wp == sinWM1) {
						if (WeaponEffect) {
							switch (WeaponEffect) {
							case (sITEMINFO_LIGHTING + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								AssaParticle_LightShot(&pos, &pos2);
								if ((rand() % 2) == 0)
									SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING1, pX, pY, pZ);
								else
									SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_LIGHTNING2, pX, pY, pZ);
								break;
							case (sITEMINFO_FIRE + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								AssaParticle_FireShot(&pos, &pos2);
								if ((rand() % 2) == 0)
									SkillPlaySound(SKILL_SOUND_SKILL_FIREBOLT1, pX, pY, pZ);
								else
									SkillPlaySound(SKILL_SOUND_SKILL_FIREBOLT2, pX, pY, pZ);
								break;
							case (sITEMINFO_ICE + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								AssaParticle_IceShot(&pos, &pos2);
								if ((rand() % 2) == 0)
									SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_ICE1, pX, pY, pZ);
								else
									SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_MAGIC_ICE2, pX, pY, pZ);
								break;
							}
						}
						else {
							StartEffectMonsterDest(x, y, z,
								chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
								MONSTER_IMP_SHOT1);
						}
					}

				}
			}
		}
	}


	return TRUE;

}

extern int MatLight[2];
extern DWORD sinSkillswitchCODE[10][10];

int smCHAR::StartHandEffect(int Mode)
{
	POINT3D angle;
	POINT3D posi;
	smMATRIX* mWorld;
	int x, y, z;

	if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_FIGON) {

		memcpy(&angle, &Angle, sizeof(POINT3D));
		angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

		AnimObjectTree(HvLeftHand.ObjBip, frame, angle.x, angle.y, angle.z);
		mWorld = &HvLeftHand.ObjBip->mWorld;
		x = pX + mWorld->_41;
		z = pZ + mWorld->_42;
		y = pY + mWorld->_43;
		StartEffectMonster(x, y, z, MONSTER_PIGON_POWER1);


		AnimObjectTree(HvRightHand.ObjBip, frame, angle.x, angle.y, angle.z);
		mWorld = &HvRightHand.ObjBip->mWorld;
		x = pX + mWorld->_41;
		z = pZ + mWorld->_42;
		y = pY + mWorld->_43;
		StartEffectMonster(x, y, z, MONSTER_PIGON_POWER1);

	}
	if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_SEN) {
		if (MotionInfo->State != CHRMOTION_STATE_DEAD) {
			memcpy(&angle, &Angle, sizeof(POINT3D));
			angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

			AnimObjectTree(HvLeftHand.ObjBip, frame, angle.x, angle.y, angle.z);
			mWorld = &HvLeftHand.ObjBip->mWorld;
			x = pX + mWorld->_41;
			z = pZ + mWorld->_42;
			y = pY + mWorld->_43;

			posi.x = x;
			posi.y = y;
			posi.z = z;
			sinEffect_Sen(&posi);
		}
	}

	if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_MUTANTTREE) {
		if (MotionInfo->State != CHRMOTION_STATE_DEAD) {
			memcpy(&angle, &Angle, sizeof(POINT3D));
			angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

			AnimObjectTree(HvLeftHand.ObjBip, frame, angle.x, angle.y, angle.z);
			mWorld = &HvLeftHand.ObjBip->mWorld;
			x = pX + mWorld->_41;
			z = pZ + mWorld->_42;
			y = pY + mWorld->_43;

			posi.x = x;
			posi.y = y + 5 * fONE;
			posi.z = z;
			sinEffect_MutentTree_Smoke(&posi);
		}
	}

	if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_RATOO) {
		if (MotionInfo->State != CHRMOTION_STATE_DEAD) {
			sinEffect_Ratoo_Aura2(this);
		}
	}

	return TRUE;

}

#define SERVER_CHAR_FRAME_STEP			320
#define SERVER_CHAR_SKIP				4

int iLoadedMapIndex = -1;

int smCHAR::Main()
{
	int x, z, y;
	int keycode;
	int cnt;
	DWORD compFrame;
	smPLAYBUFF* playBuff;
	int OldAction;
	int dist;
	POINT3D OldPos;
	int	frameStep, moveStep;
	smTRANS_COMMAND	smTransCommand;
	smCHAR* lpChar;
	int EvtFrame;
	int dm1, dm2;
	int UserSlowSpeed = 128;



	if (!Pattern || PatLoading || !MotionInfo) {
		return FALSE;
	}


	switch (ActionPattern) {
	case 0:
		if (pMount)
			pMount->Update(1.0f / 70);

		if (MotionInfo->State && MotionInfo->State < 0x100) {

			if (sinChangeSetFlag)
			{
				cInvenTory.ChangeABItem(sinChangeSetFlag);
				sinChangeSetFlag = 0;
			}

			// new costume xxstr
			if (cInvenTory.CostumeChangeFlag > 0)
			{
				if (cInvenTory.CostumeChangeFlag == 3)
				{

					cInvenTory.ChangeACItem(true);
				}
				else if (cInvenTory.CostumeChangeFlag == 2)
				{
					cInvenTory.ChangeACItem(false);
				}
				else
				{
					cInvenTory.ChangeACItem(cInvenTory.CostumeFlag == SIN_OPEN ? true : false);
				}
				cInvenTory.CostumeChangeFlag = 0;
			}

			if (iLoadedMapIndex == -1 && OnStageField != -1)
				iLoadedMapIndex = StageField[OnStageField]->FieldCode;

			if (OnStageField != -1)
			{
				if ((iLoadedMapIndex != StageField[OnStageField]->FieldCode))
				{
					if ((StageField[OnStageField]->FieldCode == 3 || StageField[OnStageField]->FieldCode == 21))
					{
						iLoadedMapIndex = StageField[OnStageField]->FieldCode;
						CancelBuffSkills();
					}
				}
			}

			if (OnStageField != -1)
				iLoadedMapIndex = StageField[OnStageField]->FieldCode;


			if (sinGetLife() == 0) {
				sinSetLife(smCharInfo.Life[1] / 2);
				ResetEnergyGraph(0);

			}

			if (PlaySlowCount) {
				if (PlaySlowSpeed) {
					UserSlowSpeed = PlaySlowSpeed;
				}
				else
					UserSlowSpeed = 128;

				if (PlaySlowSpeed > 200 || PlaySlowSpeed < 0) UserSlowSpeed = 128;
			}

			if (MoveFlag == TRUE) {
				// Velocidade das botas
				cnt = smCharInfo.Move_Speed - 1;
				if (cnt < 4) cnt = 4;
				if (cnt > 35) cnt = 35;
				if ((cInvenTory.InvenItem[sInven[9].ItemIndex - 1].sItemInfo.CODE & sinITEM_MASK2) == sinDB1)
				{
					if (DeleteEventItem_TimeOut(&cInvenTory.InvenItem[sInven[9].ItemIndex - 1].sItemInfo) == TRUE)
						cnt = 1;
					else
						cnt = smCharInfo.Move_Speed;
				}
				MoveSpeed = 200 + (12 * cnt);

				if (PlaySlowCount)
					MoveSpeed = (MoveSpeed * UserSlowSpeed) >> 8;


				if (MoveCnt > 0) {
					MoveCnt--;
					if (MoveCnt <= 0)
						MoveFlag = FALSE;
				}

				OldPos.x = pX;
				OldPos.y = pY;
				OldPos.z = pZ;

				if (!ShootingFlag) chrAttackTarget = 0;

				if (!smConfig.DebugMode && !Pattern2) MoveMode = 0;
				if (smCharInfo.Weight[0] > smCharInfo.Weight[1]) MoveMode = 0;

				if (((pMount) && !pMount->IsVisible() || pMount == nullptr) && MoveMode == 0 || ShootingFlag || smCharInfo.Stamina[0] == 0) {
					if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN &&
						MotionInfo->State != CHRMOTION_STATE_WALK)
						SetMotionFromCode(CHRMOTION_STATE_WALK);

					if (OnStageFieldState != FIELD_STATE_VILLAGE)
						MoveAngle2((MoveSpeed * 180) >> 8);
					else
						MoveAngle2(MoveSpeed);

					dwLastCharMoveTime = dwPlayTime;
				}
				else {
					if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN && MotionInfo->State != CHRMOTION_STATE_RUN)
						SetMotionFromCode(CHRMOTION_STATE_RUN);

#ifdef _WINMODE_DEBUG
					if (smConfig.DebugMode && VRKeyBuff[VK_SHIFT])
#ifdef GAME_GM
						MoveAngle2(3000);
#else
						MoveAngle2((MoveSpeed * 460) >> 8);
#endif
					else
#endif
						MoveAngle2((MoveSpeed * 460) >> 8);

					dwLastCharMoveTime = dwPlayTime;

					if ((Counter & 0x7) == 0 && WeatherMode < 2) {
						if (OnStageField >= 0 && StageField[OnStageField]->FieldCode != rsSOD_FIELD &&
							(StageField[OnStageField]->State == FIELD_STATE_RUIN ||
								StageField[OnStageField]->State == FIELD_STATE_DESERT)) {

							if ((Counter & 8) == 0)
								GetMoveLocation(4 * fONE, 0, 0, 0, Angle.y, 0);
							else
								GetMoveLocation(-4 * fONE, 0, 0, 0, Angle.y, 0);

							cnt = 10 + ((dwPlayTime >> 2) & 0xF);
							StartEffect(pX + GeoResult_X, pY + 12 * fONE, pZ + GeoResult_Z, cnt, cnt, EFFECT_DUST1);
						}
					}
				}


				if (OverLapPosi || pX != OldPos.x || pZ != OldPos.z) {
					lpChar = CheckOtherPlayPosi(pX, pY, pZ);
					if (lpChar) {
						if (!OverLapPosi) {
							pX = OldPos.x;
							pZ = OldPos.z;
							if (ActionGameMode) {
								if (lpChar->smCharInfo.State == smCHAR_STATE_USER) {
									if (chrEachMaster != lpChar) {
										if (chrEachMaster)
											CloseEachPlayer();
										OpenEachPlayer(lpChar);
									}
									if (chrEachMaster)
										EachTradeButton = TRUE;

								}
							}
						}

						if (CheckOtherPlayPosi(pX, pY, pZ)) {
							OverLapPosi = TRUE;
							OverLapPosi2 = FALSE;
						}
						else {
							OverLapPosi = FALSE;
							OverLapPosi2 = TRUE;
						}

					}
					else {
						OverLapPosi = FALSE;
						OverLapPosi2 = FALSE;
					}
				}

				if (smCharInfo.wPlayerKilling[0] > 0) {
					if (pX<PrisonRect.left || pX>PrisonRect.right || pZ<PrisonRect.top || pZ>PrisonRect.bottom) {
						pX = OldPos.x;
						pY = OldPos.y;
						pZ = OldPos.z;
					}
				}
				if (OnStageField >= 0 && StageField[OnStageField]->FieldCode == rsCASTLE_FIELD) {
					if (pZ<CastleBattleZone_DoorLineZ + 16 * fONE && pZ>CastleBattleZone_DoorLineZ - 48 * fONE &&
						pX > CastleBattleZone_DoorLineX[0] && pX < CastleBattleZone_DoorLineX[1])
					{
						if (rsBlessCastle.CastleMode && rsBlessCastle.DefenceLevel == 0) {
							pX = OldPos.x;
							pY = OldPos.y;
							pZ = OldPos.z;
						}
					}
					else {
						if (!smConfig.DebugMode && pZ < CastleBattleZone_BridgeZ && !rsBlessCastle.CastleMode) {
							if (!smCharInfo.ClassClan || GetClanCode(smCharInfo.ClassClan) != rsBlessCastle.dwMasterClan) {
								pX = OldPos.x;
								pY = OldPos.y;
								pZ = OldPos.z;
							}
						}
					}
				}

				if (OnStageField >= 0 && StageField[OnStageField]->FieldCode == 44)
				{
					if (pZ > -21415 * fONE)
					{
						pX = OldPos.x;
						pY = OldPos.y;
						pZ = OldPos.z;
					}
				}

				//Prevention Crash Lost3
				if (OnStageField >= 0 && StageField[OnStageField]->FieldCode == 47)
				{
					Point2D saPoints[] = { Point2D(113165, 93472), Point2D(114656, 93810), Point2D(113423, 94606) };

					int iX = pX >> 8;
					int iZ = pZ >> 8;

					for (int i = 0; i < _countof(saPoints); i++)
					{
						Point2D& p = saPoints[i];

						int iDX = iX - p.iX;
						int iDZ = iZ - p.iY;

						iDX *= iDX;
						iDZ *= iDZ;

						int iRange = 150;
						iRange *= iRange;

						int iRD = iDX + iDZ;

						if (iRD < iRange)
						{
							pX = OldPos.x;
							pY = OldPos.y;
							pZ = OldPos.z;
						}
					}
				}

				if (pX == OldPos.x && pY == OldPos.y && pZ == OldPos.z) {
					if (TraceMode_DblClick) {
						MoveFlag = 0;
						TraceMode_DblClick = 0;
					}
				}

				int limit_FrameStep = 0;
				if (MoveSpeed > 350)
				{
					limit_FrameStep = 350;
					FrameStep = (80 * limit_FrameStep) / 300;
				}
				else
					FrameStep = (80 * MoveSpeed) / 300;

			}
			else {
				if (MotionInfo->State != CHRMOTION_STATE_FALLDOWN && MotionInfo->State != CHRMOTION_STATE_STAND)
					SetMotionFromCode(CHRMOTION_STATE_STAND);
				if (PHeight < pY)
					MoveAngle2(0);

				FrameStep = 80;

				if (FrameCounter == 0) {
					if (CheckOtherPlayPosi(pX, pY, pZ)) {
						OverLapPosi = TRUE;
						OverLapPosi2 = FALSE;
					}
				}

			}

			if (smConfig.DebugMode)
			{
				if (VRKeyBuff[VK_SHIFT] && VRKeyBuff[VK_CONTROL] && TalkPattern)
				{
					if (VRKeyBuff[VK_F1])
						Debug_TalkZoomMode = TRUE;
					else if (VRKeyBuff[VK_F2])
						Debug_TalkZoomMode = FALSE;
					else if (VRKeyBuff[VK_F3])
					{
						Debug_TalkRepeatMode = TRUE;
						Debug_TalkRepeatModeType = TALK_MOTION_FILE;
					}
					else if (VRKeyBuff[VK_F4])
					{
						Debug_TalkRepeatMode = FALSE;
						Debug_TalkRepeatModeType = -1;
					}
					else if (VRKeyBuff[VK_F5])
					{
						Debug_TalkRepeatMode = TRUE;
						Debug_TalkRepeatModeType = FACIAL_MOTION_FILE;
					}
					else if (VRKeyBuff[VK_F6])
					{
						Debug_TalkRepeatMode = FALSE;
						Debug_TalkRepeatModeType = -1;
					}

					if (Debug_TalkRepeatMode == FALSE)
					{
						for (cnt = 0; cnt < (int)lpTalkModelParent->TalkMotionCount; cnt++)
						{
							keycode = lpTalkModelParent->TalkMotionInfo[cnt].KeyCode;
							if (keycode)
							{
								if (VRKeyBuff[keycode])
								{
									lpTalkMotionInfo = &lpTalkModelParent->TalkMotionInfo[cnt];
									TalkFrame = lpTalkMotionInfo->StartFrame * 160;
									break;
								}
							}
						}
					}
				}
				else
				{
					for (cnt = 0; cnt < (int)smMotionInfo->MotionCount; cnt++)
					{
						keycode = smMotionInfo->MotionInfo[cnt].KeyCode;
						if (keycode && keycode >= 'A')
						{
							if (VRKeyBuff[keycode])
							{
								ChangeMotion(cnt);
								break;
							}
						}
					}
				}
			}
		}

		if (MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) {
			if (!MotionLoop)
			{
				FrameStep = (80 * AttackSpeed) >> FLOATNS;

				if (smCharInfo.JOB_CODE == 8 &&
					MotionInfo->ItemCodeCount && sItem[MotionInfo->ItemCodeList[0]].CODE == (sinWM1 | sin04)) {
					FrameStep -= 16;
				}
			}
			else
				FrameStep = MotionLoopSpeed;

			if (PlaySlowCount)
				FrameStep = (FrameStep * UserSlowSpeed) >> 8;
		}
		else {
			if (MotionInfo->State == CHRMOTION_STATE_DAMAGE || MotionInfo->State == CHRMOTION_STATE_EAT) {
				FrameStep = 92;

				if (PlaySlowCount)
					FrameStep = (FrameStep * UserSlowSpeed) >> 8;
			}
		}

		if (PlaySlowCount > 0) {
			PlaySlowCount--;
			if (PlaySlowCount == 0 && AttackIce != 0) {
				AttackIce = -AttackIce;
			}
		}
		if (PlayDistortion > 0)
			PlayDistortion--;

		if (PlayVanish > 0) {
			PlayVanish--;
			if (PlayVanish <= 0 && dwM_BlurTime) dwM_BlurTime = dwPlayTime;
		}
		if (PlayStunCount > 0)
			PlayStunCount--;

		if (AttackIce != 0) {
			if (AttackIce > 0 && AttackIce < 160) AttackIce++;
			if (AttackIce < 0) AttackIce++;
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) AttackIce = 0;
		}

		if (PlayInvincible > 0)	PlayInvincible--;

		if (PlayPoison[1] > 0) {
			if (DispPoison <= 0) DispPoison = 1;
			PlayPoison[1]--;

			if (PlayPoison[0] && (PlayPoison[1] & 1) == 0 && MotionInfo->State != CHRMOTION_STATE_DEAD) {
				sinSetLife(sinGetLife() - PlayPoison[0]);
				if (sinGetLife() <= 0) {
					sinSetLife(1);
				}
			}
		}
		else {
			if (DispPoison > 0) DispPoison = -DispPoison;
		}

		if (DispPoison != 0) {
			if (DispPoison > 0 && DispPoison < 100) DispPoison++;
			if (DispPoison < 0) DispPoison++;
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) DispPoison = 0;
		}

		if (TalkFrame)
		{
			if (TalkFrame >= lpTalkMotionInfo->EndFrame * 160)
				TalkFrame = 0;
			else
				TalkFrame += 80;
		}

		if (smConfig.DebugMode)
		{
			if (Debug_TalkRepeatMode == TRUE && TalkFrame == 0)
			{
				int TalkCnt = 0;
				if (lpTalkModelParent->TalkMotionRate[CHRMOTION_EXT])
				{
					TalkCnt = rand() % 100;
					TalkCnt = lpTalkModelParent->TalkMotionRateCnt[Debug_TalkRepeatModeType][TalkCnt];
				}
				else
					TalkCnt = rand() % lpTalkModelParent->TalkMotionCount;

				lpTalkMotionInfo = &lpTalkModelParent->TalkMotionInfo[TalkCnt];
				TalkFrame = lpTalkMotionInfo->StartFrame * 160;
			}
		}

		frame += FrameStep;

		EventAttack();

		if (MoveFlag == TRUE) {

			int IsStateIce = 0;

			if (OnStageField >= 0) {
				if (StageField[OnStageField]->FieldCode != 0 && StageField[OnStageField]->FieldCode != 1 &&
					StageField[OnStageField]->FieldCode != 19 && StageField[OnStageField]->FieldCode != 18 &&
					StageField[OnStageField]->FieldCode != 24 && StageField[OnStageField]->FieldCode != 26 &&
					StageField[OnStageField]->FieldCode != 30 && StageField[OnStageField]->FieldCode != 33 &&
					StageField[OnStageField]->FieldCode != 34)

					WaterHeight = CLIP_OUT;

				if (lpCurPlayer == this && StageField[OnStageField]->State == FIELD_STATE_ICE)
					IsStateIce = 1;

			}


			if (MotionInfo->EventFrame[0]) {
				compFrame = frame - (MotionInfo->StartFrame * 160);
				if (MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) {
					PlayFootStep(this);

					if (WaterHeight != CLIP_OUT && WaterHeight > pY && WaterHeight < (pY + 32 * fONE) && OnStageField >= 0) {
						GetMoveLocation(0, 0, 8 * fONE, 0, Angle.y, 0);
						StartTerrainEffect(pX + GeoResult_X, WaterHeight + 128, pZ + GeoResult_Z, EFFECT_TERRAIN_WAVE);
					}
					else if (IsStateIce)
					{
						GetMoveLocation(3 * fONE, 0, 5 * fONE, 0, Angle.y, 0);
						SetIceFoot(0, dwPlayTime, GeoResult_X, GeoResult_Z);
					}
				}

			}

			if (MotionInfo->EventFrame[1]) {
				compFrame = frame - (MotionInfo->StartFrame * 160);
				if (MotionInfo->EventFrame[1] > (compFrame - FrameStep) && MotionInfo->EventFrame[1] <= compFrame) {
					PlayFootStep(this);

					if (WaterHeight != CLIP_OUT && WaterHeight > pY && WaterHeight < (pY + 32 * fONE) && OnStageField >= 0) {
						GetMoveLocation(0, 0, 8 * fONE, 0, Angle.y, 0);
						StartTerrainEffect(pX + GeoResult_X, WaterHeight + 128, pZ + GeoResult_Z, EFFECT_TERRAIN_WAVE);
					}
					else if (IsStateIce)
					{
						GetMoveLocation(-3 * fONE, 0, 5 * fONE, 0, Angle.y, 0);
						SetIceFoot(1, dwPlayTime, GeoResult_X, GeoResult_Z);
					}
				}
			}
		}

		if (MotionInfo->State == CHRMOTION_STATE_EAT) {
			if (MotionInfo->EventFrame[0]) {
				compFrame = frame - (MotionInfo->StartFrame * 160);
				if (MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) {
					if (UseEtherCoreCode) {
						PotionLog = POTION_ETHERCORE;
					}
					else {
						if (pUsePotion && pUsePotion->sItemInfo.CODE) {
							SendUseItemCodeToServer(&pUsePotion->sItemInfo);
						}

						switch (sinUsePotion()) {
						case 1:
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_POTION1);
							PotionLog = 1;
							ResetEnergyGraph(0);
							break;
						case 2:
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_POTION2);
							PotionLog = 2;
							ResetEnergyGraph(1);
							break;
						case 3:
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_POTION3);
							PotionLog = 3;
							ResetEnergyGraph(2);
							break;
						case 4:
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_POTION3);
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_POTION2);
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_POTION1);
							PotionLog = 4;
							ResetEnergyGraph(3);

							for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
								if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && chrOtherPlayer[cnt].PartyFlag &&
									chrOtherPlayer[cnt].MotionInfo->State != CHRMOTION_STATE_DEAD && chrOtherPlayer[cnt].RendSucess) {

									x = chrOtherPlayer[cnt].pX;
									y = chrOtherPlayer[cnt].pY + 48 * fONE;
									z = chrOtherPlayer[cnt].pZ;

									StartEffect(x, y, z, EFFECT_POTION1);
									StartEffect(x, y, z, EFFECT_POTION2);
									StartEffect(x, y, z, EFFECT_POTION3);

								}
							}
							break;
						case 5:
							SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
							CharPlaySound(lpCurPlayer);
							break;
						case 6:
							//StartEffect( pX,pY+48*fONE,pZ, EFFECT_POTION1 );
							break;
						case 7:
							SetMotionFromCode(CHRMOTION_STATE_YAHOO);
							break;
						}

					}
					SetDynLight(pX, pY + 48 * fONE, pZ, 50, 100, 70, 0, 250, 1);
				}
			}
		}

		if (frame >= MotionInfo->EndFrame * 160) {
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (g_IsDxProjectZoomIn)
				{
					SetFullZoomMap(0);
				}
				if (ReStartOptionIndex) {

					if (OnStageField >= 0 && ReStartOptionIndex == RESTART_FEILD) {
						if (StageField[OnStageField]->FieldCode == rsSOD_FIELD)
							WarpFieldNearPos(rsSOD_VILLAGE, pX, pZ, &x, &z);
						else if (StageField[OnStageField]->FieldCode == FIELD_ARENA) // Renasce no centro da arena durante o evento
							WarpFieldNearPos(FIELD_ARENA, -23879, 22340, &x, &z);
						else
							WarpFieldNearPos(StageField[OnStageField]->FieldCode, pX, pZ, &x, &z);

						if (!PlayUsed_Resurrection) {
							ContGameExp(this);
						}
						PlayUsed_Resurrection = 0;
						RestartPlayCount = 700;
					}

					if (ReStartOptionIndex == RESTART_TOWN) {

						//quando morre em bc e é lider de bc nasce no castelo de bc
						if (OnStageField >= 0 && StageField[OnStageField]->State == FIELD_STATE_CASTLE &&
							smCharInfo.ClassClan && GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)
						{
							WarpCastleField(1, &x, &z); // respaw nc se o clan for lider de bc
						}
						/*else if (OnStageField >= 0 && StageField[OnStageField]->State == FIELD_STATE_CASTLE)
						{
							// xxstr
							WarpCastleField(0, &x, &z); // quando morre nasce em BC
						}*/
						else
						{
							WarpStartField(&x, &z);	// quando morre nasce na cidade xxstr						
						}
						RestartPlayCount = 700;
						PlayUsed_Resurrection = 0;
					}

					if (ReStartOptionIndex == RESTART_EXIT)
					{
						CheckCharForm();
						sinSetLife(0);
						ResetEnergyGraph(0);

						quit = TRUE;
						SaveGameData();
						QuitSave = TRUE;
						PlayUsed_Resurrection = 0;
					}
					else
					{
						SetPosi(x, 0, z, 0, 0, 0);
						TraceCameraPosi.x = pX;
						TraceCameraPosi.y = pY;
						TraceCameraPosi.z = pZ;
						TraceTargetPosi.x = pX;
						TraceTargetPosi.y = pY;
						TraceTargetPosi.z = pZ;

						if (smCharInfo.State == smCHAR_STATE_ENEMY)
						{
							SetMotionFromCode(CHRMOTION_STATE_STAND);
						}
						else
						{
							SetMotionFromCode(CHRMOTION_STATE_RESTART);
							StartEffect(pX, pY, pZ, EFFECT_GAME_START1);
							SetDynLight(pX, pY, pZ, 100, 100, 100, 0, 200);
							CharPlaySound(this);
						}
						dwBattleTime = 0;
						dwBattleQuitTime = 0;

						OnStageField = -1;
						if (smGameStage[0])
						{
							cnt = smGameStage[0]->GetFloorHeight(pX, pY, pZ, Pattern->SizeHeight);
							lpCurPlayer->OnStageField = 0;
						}
						if (cnt == CLIP_OUT && smGameStage[1])
						{
							lpCurPlayer->OnStageField = 1;
						}
					}

					CheckCharForm();
					sinSetLife(smCharInfo.Life[1] / 2);
					ResetEnergyGraph(0);
					ReformCharForm();
					MoveFlag = FALSE;

					ReStartOptionIndex = 0;
					ReStartFlag = 0;

				}
				else {
					frame = MotionInfo->EndFrame * 160 - 80;
				}

				if (cTrade.OpenFlag && !dwTradeMaskTime) {
					SendRequestTrade(cTrade.TradeCharCode, 3);
					cTrade.CancelTradeItem();
				}

				if (FrameCounter >= 70 * 10 && lpCurPlayer->OnStageField >= 0) {
					sITEMINFO* lpItemInfo = 0;

					if (StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_CASTLE)
						lpItemInfo = FindItemFromCode(sinBC1 | sin05);

					if (lpItemInfo || PlayUsed_Resurrection) {
						if (lpItemInfo) {
							SendUseItemCodeToServer(lpItemInfo);
							cInvenTory.DeleteInvenItemToServer(lpItemInfo->CODE, lpItemInfo->ItemHeader.Head, lpItemInfo->ItemHeader.dwChkSum);
						}

						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_RESTART);
						StartEffect(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, EFFECT_GAME_START1);
						SetDynLight(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 100, 100, 100, 0, 200);
						CharPlaySound(lpCurPlayer);

						sinSetLife(lpCurPlayer->smCharInfo.Life[1] / 2);
						ResetEnergyGraph(4);
						lpCurPlayer->MoveFlag = FALSE;
						RestartPlayCount = 350;
						ReStartFlag = 0;
						PlayUsed_Resurrection = 0;

					}
				}

			}
			else {

				if (MotionLoop) MotionLoop--;

				if (MotionInfo->State == CHRMOTION_STATE_YAHOO)
					SendClanYahooMotion();

				if (MotionInfo->State == CHRMOTION_STATE_EAT) {
					if (UseEtherCoreCode) {
						if (OnStageField < 0 ||
							(StageField[OnStageField]->FieldCode != rsSOD_FIELD && StageField[OnStageField]->FieldCode != QUEST_ARENA_FIELD)) {

							switch (UseEtherCoreCode) {
							case (sinEC1 | sin01):
								SaveGameData();
								WarpStartField(START_FIELD_NUM, &pX, &pZ);
								SetPosi(pX, 0, pZ, 0, 0, 0);
								break;
							case (sinEC1 | sin02):
								SaveGameData();
								WarpStartField(START_FIELD_NEBISCO, &pX, &pZ);
								SetPosi(pX, 0, pZ, 0, 0, 0);
								break;
							case (sinEC1 | sin03):
								SaveGameData();
								WarpStartField(START_FIELD_MORYON, &pX, &pZ);
								SetPosi(pX, 0, pZ, 0, 0, 0);
								break;
							case (sinEC1 | sin06):
								SaveGameData();
								WarpStartField(52, &pX, &pZ);
								SetPosi(pX, 0, pZ, 0, 0, 0);
								break;
							}

							TraceCameraPosi.x = pX;
							TraceCameraPosi.y = pY;
							TraceCameraPosi.z = pZ;
							TraceTargetPosi.x = pX;
							TraceTargetPosi.y = pY;
							TraceTargetPosi.z = pZ;

							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
							SkillPlaySound(SKILL_SOUND_LEARN, pX, pY, pZ);
							RestartPlayCount = 700;
						}
						MoveFlag = 0;
						MouseButton[0] = 0;
						dwNextMotionCode = 0;
						UseEtherCoreCode = 0;
					}
					else {
						ComparePotion();
					}

				}

				if (lpTransVirtualPotion) {
					x = (smCharInfo.Life[1] * 40) / 100;
					if (lpTransVirtualPotion->WParam < x) x = lpTransVirtualPotion->WParam;
					sinSetLife(sinGetLife() + x);

					sinSetMana(sinGetMana() + lpTransVirtualPotion->LParam);
					sinSetStamina(sinGetStamina() + lpTransVirtualPotion->SParam);
					ResetEnergyGraph(3);

					ZeroMemory(lpTransVirtualPotion, sizeof(smTRANS_COMMAND));
					delete lpTransVirtualPotion;
					lpTransVirtualPotion = 0;
				}

				if (sinChangeSetFlag && !MotionLoop) {
					cInvenTory.ChangeABItem(sinChangeSetFlag);
					sinChangeSetFlag = 0;
				}

				if (dwNextMotionCode && !MotionLoop) {
					if (SetMotionFromCode(dwNextMotionCode) == FALSE) {
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					}
					else {
						if (UseEtherCoreCode && dwNextMotionCode == CHRMOTION_STATE_EAT) {
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
							SkillPlaySound(SKILL_SOUND_LEARN, pX, pY, pZ);
						}
					}
					dwNextMotionCode = 0;
				}
				else {
					if (MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) {

						if (MotionInfo->State == CHRMOTION_STATE_SKILL && AttackSkil)
							EndSkill();

						if (AttackSkil && MotionLoop) {
							frame = MotionInfo->StartFrame * 160;
							PlaySkillLoop(AttackSkil, this);
						}
						else {
							if (!AttAutoCont && MouseButton[0] == 0 && MouseButton[1] == 0 && lpCharMsTrace && lpCharMsTrace == chrAttackTarget) {
								if (CancelAttack() == TRUE)
									MoveFlag = FALSE;
							}
							if (MoveFlag && !MsTraceMode) {
								Angle.y = GetPlayMouseAngle();
							}
						}
					}
					else if (MotionInfo->State == CHRMOTION_STATE_EAT)
					{
						if (!AttAutoCont && MouseButton[0] == 0 && MouseButton[1] == 0 && lpCharMsTrace && lpCharMsTrace == chrAttackTarget)
						{
							if (CancelAttack() == TRUE)
								MoveFlag = FALSE;
						}
					}

					if (MotionInfo->Repeat == TRUE) {
						frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
					}
					else {
						if (!MotionLoop) {
							if (dwNextMotionCode) {
								if (SetMotionFromCode(dwNextMotionCode) == FALSE) {
									SetMotionFromCode(CHRMOTION_STATE_STAND);
								}
								dwNextMotionCode = 0;
							}
							else {
								SetMotionFromCode(CHRMOTION_STATE_STAND);
							}
						}
					}
				}
			}

			AttackSpeed = GetAttackSpeedMainFrame(smCharInfo.Attack_Speed);

			if (dwActionItemTwoHand)
				AttackSpeed += 16;

			for (cnt = 0; cnt < 10; cnt++) {
				if (ContinueSkill[cnt].Flag) {
					if (ContinueSkill[cnt].CODE == SKILL_SWIFT_AXE) {
						if ((dwActionItemCode & sinITEM_MASK2) == sinWA1) {
							x = ContinueSkill[cnt].Point - 1;
							if (x >= 0 && x < 10) {
								AttackSpeed += (AttackSpeed * Swift_Axe_Speed[x]) / 100;
							}
							break;
						}
					}
				}
			}
			if (!MotionLoop)
			{
				AttackEffect = 0;
			}

			MotionEvent = 0;
		}

		if (AttackAnger > 0)
			AttackAnger--;

		if (dwWeaponEffectTime) {
			if (dwWeaponEffectTime < dwPlayTime) {
				dwWeaponEffectTime = 0;
				WeaponEffect = 0;
			}
		}

		wStickItems[smPLAYBUFF_ITEM_SKIL] = AttackSkil;


		if (WaterHeight != CLIP_OUT && WaterHeight > pY && WaterHeight < (pY + 32 * fONE) && OnStageField >= 0) {

			dist = 0;
			cnt = 0x3F;
			switch (MotionInfo->State) {
			case CHRMOTION_STATE_WALK:
				cnt = 0x1F;
				dist = 8;
				break;
			case CHRMOTION_STATE_RUN:
				dist = 14;
			case CHRMOTION_STATE_ATTACK:
			case CHRMOTION_STATE_SKILL:
				cnt = 0xF;
				break;
			}

			if ((Counter & cnt) == 0) {
				GetMoveLocation(0, 0, dist << FLOATNS, 0, Angle.y, 0);
				StartTerrainEffect(pX + GeoResult_X, WaterHeight + 128, pZ + GeoResult_Z, EFFECT_TERRAIN_WAVE);
			}
		}

		SavePlayBuff();
		break;

	case 1:
		break;

	case 5:

		if (PlaySlowCount) {
			if (PlaySlowSpeed) {
				moveStep = (SERVER_CHAR_SKIP * PlaySlowSpeed) >> 8;
				frameStep = (SERVER_CHAR_FRAME_STEP * PlaySlowSpeed) >> 8;
			}
			else {
				moveStep = SERVER_CHAR_SKIP >> 1;
				frameStep = SERVER_CHAR_FRAME_STEP >> 1;
			}
		}
		else {
			moveStep = SERVER_CHAR_SKIP;
			frameStep = SERVER_CHAR_FRAME_STEP;
		}

		if (MotionInfo->State == CHRMOTION_STATE_WARP) {
			frame += frameStep;

			if (frame > MotionInfo->EndFrame * 160) {
				Close();
				return TRUE;
			}
			SavePlayBuff2();
			FrameCounter += 3;
			break;
		}

		//¹ìÇÇ¸¯ ¸Ó½Å ¿òÁ÷ÀÌÁö ¾Êµµ·Ï
		//if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_VAMPRICMACHINE || smCharInfo.dwCharSoundCode==snCHAR_SOUND_CASTLE_DOOR )
		if (DontMoveFlag)	moveStep = 0;


		//¹Ùº§ ÃâÇö À¯ÁöÁß
		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL) {
			if (smCharInfo.Life[0] > 0)	dwBabelPlayTime = dwPlayServTime + 5000;
		}
		/*
				if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_KELVEZU ) {
					if ( smCharInfo.Life[0]>0 )	dwKelvezuPlayTime = dwPlayServTime+5000;
				}
		*/
		if (MotionInfo->State && MotionInfo->State < 0x100 && moveStep) {
			if (MotionInfo->State == CHRMOTION_STATE_WALK || MotionInfo->State == CHRMOTION_STATE_RUN) {
				OldPos.x = pX;
				OldPos.y = pY;
				OldPos.z = pZ;

				if (sMoveStepCount[0]) {
					//ÀÌµ¿ ½ºÅÜ ¼öµ¿ ¼³Á¤
					if (sMoveStepCount[1]) moveStep *= sMoveStepCount[1];

					for (cnt = 0; cnt < sMoveStepCount[0]; cnt++) {
						MoveAngle(moveStep);
						if (MotionInfo->State == CHRMOTION_STATE_RUN)
							MoveAngle(moveStep);
					}
				}
				else {

					if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS && smCharInfo.dwCharSoundCode == snCHAR_SOUND_DARKKNIGHT) {
						//µÒ·Îµå
						MoveAngle(moveStep * 2);
					}
					else
						MoveAngle(moveStep);

					if (MotionInfo->State == CHRMOTION_STATE_RUN) {
						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_GREVEN) {
							//°ø·æ ´Þ¸®±â ¹«Áö »¡¸®
							if (lpTargetPlayInfo && TragetTraceMode) {
								SetTargetPosi2(lpTargetPlayInfo->Position.x >> FLOATNS,
									lpTargetPlayInfo->Position.z >> FLOATNS);
							}
							MoveAngle(moveStep * 2);
							MoveAngle(moveStep * 2);
						}
						else {
							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN) {
								//¿ï¹ö¸° ´Þ¸®±â
								if (lpTargetPlayInfo && TragetTraceMode) {
									SetTargetPosi2(lpTargetPlayInfo->Position.x >> FLOATNS,
										lpTargetPlayInfo->Position.z >> FLOATNS);
								}
								MoveAngle(moveStep * 2);
							}
							else
								MoveAngle(moveStep);
						}
					}
				}

				if (MotionInfo->State != CHRMOTION_STATE_WALK && MotionInfo->State != CHRMOTION_STATE_RUN)
					SetMotionFromCode(CHRMOTION_STATE_WALK);



				if (OverLapPosi || pX != OldPos.x || pZ != OldPos.z) {
					lpCharOverLap = CheckOtherPlayPosi(pX, pY, pZ);
					if (lpCharOverLap) {
						if (!OverLapPosi) {
							pX = OldPos.x;
							pZ = OldPos.z;
							lpCharOverLap = CheckOtherPlayPosi(pX, pY, pZ);
						}

						if (lpCharOverLap) {
							OverLapPosi = TRUE;
							OverLapPosi2 = FALSE;
						}
						else {
							OverLapPosi = FALSE;
							OverLapPosi2 = TRUE;
						}

					}
					else {
						OverLapPosi = FALSE;
						OverLapPosi2 = FALSE;
					}
				}
				if (!OverLapPosi) lpCharOverLap = 0;
			}
			else {
				if (MotionInfo->State != CHRMOTION_STATE_STAND) SetMotionFromCode(CHRMOTION_STATE_STAND);
			}
		}

		frame += frameStep;

		if (frame > MotionInfo->EndFrame * 160) {

			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				frame = MotionInfo->EndFrame * 160;
			}
			else {

				if (MotionInfo->State == CHRMOTION_STATE_EAT && PotionLog == 1) {
					//¹°¾à »ç¿ë ¼º°ø
					smCharInfo.Life[0] += (smCharInfo.Life[1] >> 1);
					if (smCharInfo.Life[0] > smCharInfo.Life[1])
						smCharInfo.Life[0] = smCharInfo.Life[1];
				}

				if (smMonsterInfo.SkillCurse && smCharInfo.Brood != smCHAR_MONSTER_USER) {
					if (MotionInfo->State == CHRMOTION_STATE_ATTACK) {
						lpTargetPlayInfo = 0;
					}

					if (lpTargetChar && MotionInfo->State == CHRMOTION_STATE_SKILL) {
						lpTargetChar->PlayHolyPower[0] = GetRandomPos(10, 20);
						lpTargetChar->PlayHolyPower[1] = 16 * 20;
						lpTargetChar = 0;
					}
				}

				if ((lpTargetPlayInfo || TargetMoveCount) && MotionInfo->State < 0x100)
					PosBeginCount++;

				if (MotionInfo->Repeat == TRUE) {
					frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
				}
				else {
					if (MotionInfo->State == CHRMOTION_STATE_DAMAGE && !PlayStunCount && !PlayStopCount &&
						SetMotionFromCode(CHRMOTION_STATE_WALK))
					{
						TargetMoveCount = 2;
					}
					else
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					AutoMoveStep = 1;
				}

				if (PosBeginCount > 4) {
					x = (pX - PosBeginMove.x) >> FLOATNS;
					z = (pZ - PosBeginMove.z) >> FLOATNS;
					dist = x * x + z * z;

					PosBeginMove.x = pX;
					PosBeginMove.z = pZ;
					PosBeginCount = 0;

					if (dist < (32 * 32)) {
						SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
						TragetTraceMode = 0;

						y = (rand() % ANGLE_90) - ANGLE_45;
						Angle.y = (Angle.y + y) & ANGCLIP;

						TargetMoveCount = 10;
						OverLapPosi2 = FALSE;

						ptNextTarget.x = 0;
						ptNextTarget.y = 0;
						ptNextTarget.z = 0;

						if (!SetMotionFromCode(CHRMOTION_STATE_RUN)) {
							SetMotionFromCode(CHRMOTION_STATE_WALK);
						}

						//lpTargetPlayInfo = 0;
						goto Skip_5;
					}
				}

				if (TargetMoveCount > 0) {
					TargetMoveCount--;
					if (TargetMoveCount <= 0) SetMotionFromCode(CHRMOTION_STATE_STAND);
				}

				if (DistAroundDbl && (MotionInfo->State == CHRMOTION_STATE_WALK ||
					MotionInfo->State == CHRMOTION_STATE_RUN)) {
					//°Å¸®¸¦ ¹þ¾î³­°æ¿ì Ã³¸®
					x = (pX - PosiAround.x) >> FLOATNS;
					z = (pZ - PosiAround.z) >> FLOATNS;
					dist = x * x + z * z;
					if (dist > DistAroundDbl) {
						SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
						ptNextTarget.x = ptNextTarget.z = 0;
						TargetMoveCount = 6;
						TragetTraceMode = 0;
					}
				}

				if (!TargetMoveCount && ptNextTarget.x && ptNextTarget.z) {

					SetTargetPosi(ptNextTarget.x >> FLOATNS, ptNextTarget.z >> FLOATNS);
					TragetTraceMode = TRUE;

					if (OverLapPosi && lpCharOverLap) {
						//Ä³¸¯ÅÍ°¡ °ãÃÄ ÀÖÀ½ ( ¶³¾îÁöµµ·Ï ³ë·Â ÇÔ )
						Angle.y = GetRadian2D(lpCharOverLap->pX >> FLOATNS, lpCharOverLap->pZ >> FLOATNS,
							pX >> FLOATNS, pZ >> FLOATNS);

						SetMotionFromCode(CHRMOTION_STATE_WALK);
						OverLapPosi2 = FALSE;
						AutoMoveStep = 0;
					}
					else {
						if (AutoMoveStep <= 0) {
							if (MotionInfo->State == CHRMOTION_STATE_STAND) {
								//Áö´É¿¡ ºñ·ÊÇÑ ÀÌµ¿
								AutoMoveStep = smMonsterInfo.IQ - 5;
								if (AutoMoveStep < 0) AutoMoveStep = 0;

								x = (pX - ptNextTarget.x) >> FLOATNS;
								z = (pZ - ptNextTarget.z) >> FLOATNS;
								dist = x * x + z * z;

								if (smMonsterInfo.IQ >= 6 && dist < (64 * 8 * 64 * 8)) {
									if (SetMotionFromCode(CHRMOTION_STATE_RUN)) {
										AutoMoveStep <<= 2;			//´Þ¸®±â´Â Á»´õ ¿À·¡
									}
									else
										SetMotionFromCode(CHRMOTION_STATE_WALK);
								}
								else
									SetMotionFromCode(CHRMOTION_STATE_WALK);
							}
							else
								SetMotionFromCode(CHRMOTION_STATE_STAND);
						}
						else {
							SetTargetPosi2(ptNextTarget.x >> FLOATNS, ptNextTarget.z >> FLOATNS);
							AutoMoveStep--;
						}
					}

					ptNextTarget.x = 0;
					ptNextTarget.y = 0;
					ptNextTarget.z = 0;
				}

				if (smMonsterInfo.PotionCount) {
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30) {
						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER) {
							smTransCommand.code = smTRANSCODE_PROCESS_SKILL2;
							smTransCommand.size = sizeof(smTRANS_COMMAND);
							smTransCommand.WParam = SKILL_PLAY_HEALING;
							smTransCommand.LParam = dwObjectSerial;
							smTransCommand.SParam = 0;
							smTransCommand.EParam = 0;
							if (lpLinkPlayInfo && lpLinkPlayInfo->lpsmSock)
								lpLinkPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

							smCharInfo.Life[0] += (smCharInfo.Life[1] >> 2);
							if (smCharInfo.Life[0] > smCharInfo.Life[1])
								smCharInfo.Life[0] = smCharInfo.Life[1];

						}
						else {
							SetMotionFromCode(CHRMOTION_STATE_EAT);
							PotionLog = 1;
						}

						smMonsterInfo.PotionCount--;
					}
				}

				if (dwNextMotionCode) {
					if (SetMotionFromCode(dwNextMotionCode) == FALSE) {
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					}
					dwNextMotionCode = 0;
				}
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HUNGKY) {

					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30 || AttackAnger>80) {
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEBECHICK) // ¾Æ±â ²¿²¿ -> ¾Æºü ²¿²¿ ·Î º¯½Å
				{
					//º¯½Å
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30 || AttackAnger>80) {
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEADHOPT)	// pluto ¾ðµ¥µå Å·È£ÇÇ º¯½Å
				{
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 20 || AttackAnger > 80)
					{
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

			}
		}
	Skip_5:
		if (DontMoveFlag == 2) {
			//°¢µµ °íÁ¤
			Angle.x = 0;
			Angle.y = 0;
			Angle.z = 0;
		}
		SavePlayBuff2();

		if (lpMasterPlayInfo && HoSkillCode && lpMasterPlayInfo->lpsmSock && smCharInfo.Life[0] > 0) {

			if (lpMasterPlayInfo->lpLinkChar != this) {
				smCharInfo.Life[0] = 0;
				SetMotionFromCode(CHRMOTION_STATE_DEAD);

			}
			else {

				x = (lpMasterPlayInfo->Position.x - pX) >> FLOATNS;
				z = (lpMasterPlayInfo->Position.z - pZ) >> FLOATNS;
				dist = x * x + z * z;
				if (dist >= (500 * 500)) {
					if (lpStage) {
						y = lpStage->GetHeight(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);
						if (y < 0) {
							smCharInfo.Life[0] = 0;
							SetMotionFromCode(CHRMOTION_STATE_DEAD);
						}
					}
					if (smCharInfo.Life[0] > 0) {
						pX = lpMasterPlayInfo->Position.x;
						pY = lpMasterPlayInfo->Position.y;
						pZ = lpMasterPlayInfo->Position.z;

						ptNextTarget.x = ptNextTarget.z = 0;
						TargetMoveCount = 0;
						TragetTraceMode = 0;

						lpTargetChar = 0;

					}
				}
				else if (dist >= (300 * 300) || (!lpTargetChar && dist >= (150 * 150))) {

					SetTargetPosi(lpMasterPlayInfo->Position.x >> FLOATNS, lpMasterPlayInfo->Position.z >> FLOATNS);
					TargetMoveCount = 2;
					TragetTraceMode = 0;
					lpTargetChar = 0;
					if (MotionInfo->State != CHRMOTION_STATE_DEAD && (MotionInfo->State >= 0x100 || MotionInfo->State == CHRMOTION_STATE_STAND)) {
						if (!SetMotionFromCode(CHRMOTION_STATE_RUN))
							SetMotionFromCode(CHRMOTION_STATE_WALK);
					}
				}
			}
		}

		if (dwLinkObjectCode) {
			switch (smCharInfo.dwCharSoundCode) {
			case snCHAR_SOUND_NPC_SKILLMASTER:
			case snCHAR_SOUND_NPC_MAGICMASTER:

				if (smCharInfo.Life[0] <= (smCharInfo.Life[1] >> 4)) {
					//½ºÅ³¸¶½ºÅÍ¸¦ ÀÌ°å´Ù!
					smCharInfo.Life[0] = (smCharInfo.Life[1] >> 4);
					lpTargetPlayInfo = 0;
					lpTargetChar = 0;
					SetMotionFromCode(CHRMOTION_STATE_STAND);
					//smCharInfo.State=smCHAR_STATE_NPC;
					ActionPattern = 12;
					PosiAround.x = pX;
					PosiAround.y = pY;
					PosiAround.z = pZ;
					DistAroundDbl = 256 * 256;
					if (lpChar) {
						lpChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
						lpChar->smCharInfo.Life[0] = 0;
					}
					/*
					else {
						chrAutoPlayer[cnt].Close();
						((STG_AREA *)chrAutoPlayer[cnt].lpExt1)->MonsterCount--;
						((STG_AREA *)chrAutoPlayer[cnt].lpExt1)->DeleteMonTable( &chrAutoPlayer[cnt] );
					}
					*/
				}
				else {
					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER) {
						//·¹ÀÌ¸ó ¼ø°£ÀÌµ¿ È¸ÇÇ
						if ((rand() % 100) < 3 && MotionInfo->State != CHRMOTION_STATE_SKILL && MotionInfo->State != CHRMOTION_STATE_EAT) {
							if (lpLinkPlayInfo) {
								x = (pX - lpLinkPlayInfo->Position.x) >> FLOATNS;
								z = (pZ - lpLinkPlayInfo->Position.z) >> FLOATNS;
								dist = x * x + z * z;
								if (dist < (300 * 300)) {
									pX = QUEST_ARENA_NPC_POS_X + ((rand() % 600) - 300) * fONE;
									pZ = QUEST_ARENA_NPC_POS_Z + ((rand() % 600) - 300) * fONE;
								}
							}
						}
					}
				}
				break;

			case snCHAR_SOUND_FURY:

				if (smCharInfo.Life[0] <= 0 && !smMonsterInfo.PotionCount && smCharInfo.Life[1] > 1) {
					//½ºÅ³¸¶½ºÅÍ¸¦ ÀÌ°å´Ù!
					if (lpLinkPlayInfo && lpLinkPlayInfo->dwObjectSerial == dwLinkObjectCode && lpLinkPlayInfo->lpsmSock) {
						smCharInfo.Life[1] = 1;
					}
				}
				break;
			}
		}

		if (lpTargetChar) {
			if (smMonsterInfo.SkillCurse && smCharInfo.Brood != smCHAR_MONSTER_USER && lpTargetChar->smCharInfo.Brood != smCHAR_MONSTER_USER) {
				//¸ó½ºÅÍ °ø°Ý·Â °­È­¿ë
				if (MotionInfo->State < 0x100) {
					TargetMoveCount = 1;
					AutoMoveStep = 1;
					lpTargetChar = 0;
				}
			}
			else if (lpTargetChar->Flag && !PlayStunCount && !PlayStopCount) {

				dist = smCharInfo.Shooting_Range + 32;			//½´ÆÃ °Å¸®°¡ ¸ó½ºÅÍÀÇ °ø°Ý °Å¸®
				if (dist < 80) dist = 80;						//°Å¸® º¸Á¤

				if (MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
					MotionInfo->State != CHRMOTION_STATE_DEAD &&
					MotionInfo->State != CHRMOTION_STATE_EAT) {

					if (MotionInfo->State != CHRMOTION_STATE_ATTACK &&
						MotionInfo->State != CHRMOTION_STATE_SKILL && MotionInfo->State != CHRMOTION_STATE_DEAD) {

						cnt = GetDistanceDbl(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);

						dist = dist >> FLOATNS;
						dist *= dist;

						if (cnt < dist && abs(pY - lpTargetChar->pY) < 64 * fONE) {

							PosBeginMove.x = 0;
							PosBeginMove.z = 0;
							PosBeginCount = 0;

							dwLastTransTime = dwPlayServTime;		//ÃÖ±Ù È°¼ºÈ­µÈ ½Ã°£

							SetMotionFromCode(CHRMOTION_STATE_ATTACK);

							if (smMonsterInfo.SkillRating > 0 && MotionInfo->State == CHRMOTION_STATE_ATTACK) {
								//¸ó½ºÅÍ ½ºÅ³ °ø°Ý
								if ((rand() % 100) < smMonsterInfo.SkillRating)
									SetMotionFromCode(CHRMOTION_STATE_SKILL);
							}

							Angle.y = GetRadian2D(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);
							AttackX = lpTargetChar->pX;
							AttackY = lpTargetChar->pY;
							AttackZ = lpTargetChar->pZ;

						}
						else {
							if (cnt > DIST_TRANSLEVEL_LOW) {
								lpTargetChar = 0;
							}
						}
					}
				}

				if (smCharInfo.Brood != smCHAR_MONSTER_USER) {
					if (FrameCounter == 80 && smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL) {		//º¸½º ¹Ùº§
						AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;

						if (smCharInfo.Life[0] > smCharInfo.Life[1] || smCharInfo.Life[0] < 0)
							smCharInfo.Life[0] = smCharInfo.Life[1];
					}
				}


				if (FrameCounter == 40 && lpTargetChar) {

					if (MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) {
						//SendTransAttack( 0 , lpTargetPlayInfo->lpsmSock , 0);
						AttackUserFlag = TRUE;
						AttackAnger++;				//°ø°Ý È½¼ö Ä«¿îÅÍ
						cnt = sinGetPVPAccuracy(smCharInfo.Level, smCharInfo.Attack_Rating, lpTargetChar->smCharInfo.Level, lpTargetChar->smCharInfo.Defence);
						if ((rand() % 100) < cnt && lpTargetChar->smCharInfo.Life[0] > 0) {
							//¸íÁß
							if (!lpTargetChar->lpTargetPlayInfo) {
								lpTargetChar->lpTargetChar = this;
							}
							else {
								dist = GetDistanceDbl(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);
								if (GetDistanceDbl(pX, pZ, lpTargetChar->lpTargetPlayInfo->Position.x, lpTargetChar->lpTargetPlayInfo->Position.z) > dist) {
									lpTargetChar->lpTargetChar = this;
									lpTargetChar->lpTargetPlayInfo = 0;
								}
							}

							// smCharInfo.Attack_Damage[0]  ¹Ì½ºÆ½ Ã³¸®

							if (MotionInfo->State == CHRMOTION_STATE_SKILL && smCharInfo.Attack_Damage[0]) {
								cnt = GetRandomPos(smMonsterInfo.SkillDamage[0], smMonsterInfo.SkillDamage[1]);

								switch (smCharInfo.dwCharSoundCode) {
								case snCHAR_SOUND_GUARDIAN_SAINT:

									cnt = 0;
									break;
								}
							}
							else {
								cnt = GetRandomPos(smCharInfo.Attack_Damage[0], smCharInfo.Attack_Damage[1]);
							}

							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_CHAOSCARA ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU ||
								smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA)
							{
								cnt *= 8;
								if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU)
								{
									lpTargetChar->smCharInfo.Life[0] = 0;
									cnt = 1;
								}
							}


							if (lpTargetChar)
								cnt -= (cnt * lpTargetChar->smCharInfo.Absorption) / 100;
							else
								cnt = 0;

							if (cnt > 0)
							{
								lpTargetChar->smCharInfo.Life[0] -= cnt;

								if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WEB)
								{
									if (lpTargetChar->smCharInfo.dwCharSoundCode != snCHAR_SOUND_KELVEZU)
										lpTargetChar->PlaySlowCount = -36;
								}


								if (lpTargetChar->smCharInfo.Life[0] <= 0)
								{
									lpTargetChar->smCharInfo.Life[0] = 0;
									lpTargetChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
									if (lpMasterPlayInfo && lpMasterPlayInfo->lpsmSock && lpMasterPlayInfo->dwObjectSerial && lpMasterPlayInfo->dwObjectSerial == smCharInfo.Next_Exp)
									{
										lpTargetChar->lpExt2 = lpMasterPlayInfo;

										lpTargetChar->lpExt3 = this;
									}
									else
									{
										if (lpMasterPlayInfo)
										{

											smCharInfo.Life[0] = 0;
											SetMotionFromCode(CHRMOTION_STATE_DEAD);
										}
										else
										{
											lpTargetChar = 0;
										}
									}
								}
								else
								{
									if (lpTargetChar->MotionInfo->State < 0x100)
									{
										lpTargetChar->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
									}
								}
							}
						}
					}
				}
			}
			else
			{
				SetNextTarget(0, 0, 0);
			}

			if (lpTargetChar && (!lpTargetChar->Flag || lpTargetChar->smCharInfo.Life[0] <= 0)) {
				lpTargetChar = 0;
				if (MotionInfo->State < 0x100) {
					TargetMoveCount = 1;
					AutoMoveStep = 1;
				}
			}

			//if ( (smCharInfo.dwCharSoundCode==snCHAR_SOUND_DEATHKNIGHT || smCharInfo.dwCharSoundCode==snCHAR_SOUND_KELVEZU ) && lpTargetChar ) 
			if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT && lpTargetChar)
			{		//¹ß·»Åä ¾å»ðÀÌ »ó´ë °Ë»ç
				if (GetDistanceDbl(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ) > 120 * 120) {
					lpTargetChar = 0;			//¾Ð»ðÀÌ »ó´ë ¾ÈÇÔ
					SetNextTarget(0, 0, 0);
				}
			}

		}


		cnt = TRUE;

		switch (smCharInfo.dwCharSoundCode) {		//¹ìÇÇ¸¯ ¸Ó½Å °ø°Ý Á¦ÇÑ (±ê¹ß³» ¸ó½ºÅÍ¼ö È®ÀÎ)
		case snCHAR_SOUND_VAMPRICMACHINE:
			break;

		case snCHAR_SOUND_MYSTIC:				//Á×À»¶§ ¹Ì½ºÆ½ ÀÚÆø
			if (MotionInfo->State == CHRMOTION_STATE_DEAD && FrameCounter < 16) {
				AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;
				AttackUserFlag = TRUE;
				FrameCounter = 16;
			}
			if (MotionInfo->State != CHRMOTION_STATE_DEAD) {
				if (Counter > 16 * 60 * 2) {
					smCharInfo.Life[0] = 0;
					SetMotionFromCode(CHRMOTION_STATE_DEAD);
					AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;
					AttackUserFlag = TRUE;
					FrameCounter = 16;
				}
			}
			if (lpTargetChar && MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (FrameCounter == 16 && lpTargetChar->MotionInfo->State != CHRMOTION_STATE_DEAD && lpTargetChar->smCharInfo.Life[0] > 0) {
					lpTargetChar->smCharInfo.Life[0] -= GetRandomPos(smMonsterInfo.SkillDamage[0], smMonsterInfo.SkillDamage[1]) * 2;
					if (lpTargetChar->smCharInfo.Life[0] <= 0) {
						lpTargetChar->smCharInfo.Life[0] = 0;
						lpTargetChar->SetMotionFromCode(CHRMOTION_STATE_DEAD);
					}
				}
				lpTargetChar = 0;
			}
			break;

		case snCHAR_SOUND_CHAOSCARA:
			if (MotionInfo->State == CHRMOTION_STATE_SKILL && MotionInfo->KeyCode != 'J') {
				smMonsterInfo.SkillRangeRect.left = 0;
				smMonsterInfo.SkillRangeRect.right = 0;
				smMonsterInfo.SkillRangeRect.top = 0;
				smMonsterInfo.SkillRangeRect.bottom = 0;
			}
			break;
		}




		//¼­¹ö¿ë Ã³¸® ( ¸ñÇ¥ Ä³¸¯À» ÃßÀûÇÏ¿© °ø°Ý ÇÏµµ·Ï ¼³Á¤ )
		if (!lpTargetChar && lpTargetPlayInfo && cnt) {
			if (lpTargetPlayInfo->lpsmSock && !PlayStunCount && !PlayStopCount) {

				dist = smCharInfo.Shooting_Range;			//½´ÆÃ °Å¸®°¡ ¸ó½ºÅÍÀÇ °ø°Ý °Å¸®

				if (MotionInfo->State != CHRMOTION_STATE_DAMAGE &&
					MotionInfo->State != CHRMOTION_STATE_DEAD &&
					MotionInfo->State != CHRMOTION_STATE_EAT && dist >= 0) {
					if (PlayAttackFromPosi(
						lpTargetPlayInfo->Position.x,
						lpTargetPlayInfo->Position.y,
						lpTargetPlayInfo->Position.z,
						dist) == TRUE) {

						PosBeginMove.x = 0;
						PosBeginMove.z = 0;
						PosBeginCount = 0;

						dwLastTransTime = dwPlayServTime;		//ÃÖ±Ù È°¼ºÈ­µÈ ½Ã°£

						if (smMonsterInfo.SkillRating > 0 && MotionInfo->State == CHRMOTION_STATE_ATTACK) {
							//¸ó½ºÅÍ ½ºÅ³ °ø°Ý
							if ((rand() % 100) < smMonsterInfo.SkillRating) {
								if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER && (rand() % 100) < 20) {
									//¸ÅÁ÷ ¸¶½ºÅÍ ±â·Â ¹°¾à¸Ô±â
									SetMotionFromCode(CHRMOTION_STATE_EAT);
									PotionLog = 2;
								}
								else if (SetMotionFromCode(CHRMOTION_STATE_SKILL) > 0) {
									if (smMonsterInfo.SkillCurse) {
										//ÀúÁÖ °É±â ±â¼ú
										switch (smMonsterInfo.SkillCurse) {
										case 102:	//´Ù¸¥ ¸ó½ºÅÍ °ø°Ý·Â °­È­ ±â¼ú

											if (lpChar) {
												lpTargetChar = lpChar;
												lpTargetPlayInfo = 0;
												Angle.y = GetRadian2D(pX, pZ, lpTargetChar->pX, lpTargetChar->pZ);
											}
											else {
												//ÀúÁÖ ±â¼ú ½ÇÆÐ
												if (SetMotionFromCode(CHRMOTION_STATE_ATTACK) == FALSE)
													SetMotionFromCode(CHRMOTION_STATE_STAND);
											}
											break;

										case 103:	//À¯Àú¿¡°Ô ÀúÁÖ°É±â ( °ø°Ý·Â ¾àÈ­ )
											lpTargetPlayInfo->dwCurse_Attack_Time = dwPlayServTime + SUCCUBUS_CURSE_TIME * 1000;
											lpTargetPlayInfo->dwCurse_Attack_Param = GetRandomPos(10, 15);
											break;

										case 101:	//À¯Àú¿¡°Ô ÀúÁÖ°É±â ( Èí¼ö·Â ¾àÈ­ )
											lpTargetPlayInfo->dwCurse_Defence_Time = dwPlayServTime + SUCCUBUS_CURSE_TIME * 1000;
											lpTargetPlayInfo->dwCurse_Defence_Param = GetRandomPos(2, 6);

											if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_MYSTIC)
												lpTargetPlayInfo->dwCurse_Defence_Param = GetRandomPos(10, 16);

											break;

										}
									}


									if (smMonsterInfo.SkillRange > 0) {
										//¹üÀ§Çü
										//AttackSkillRange = smMonsterInfo.SkillRange*smMonsterInfo.SkillRange;
										AttackSkillRange = 0;

										if (smMonsterInfo.SkillDistance) {

											GetMoveLocation(0, 0, smMonsterInfo.SkillDistance * fONE, 0, Angle.y, 0);
											x = pX + GeoResult_X;
											z = pZ + GeoResult_Z;

											dist = smMonsterInfo.SkillRange << FLOATNS;

											smMonsterInfo.SkillRangeRect.left = x - dist;
											smMonsterInfo.SkillRangeRect.right = x + dist;
											smMonsterInfo.SkillRangeRect.top = z - dist;
											smMonsterInfo.SkillRangeRect.bottom = z + dist;
										}
									}

									switch (smCharInfo.dwCharSoundCode) {
									case snCHAR_SOUND_RATOO:
										break;

										//Summons Seguindo
									case snCHAR_SOUND_NPC_SKILLMASTER:
										cnt = rand() % 100;
										if (cnt < 3 && lpLinkPlayInfo) {
											if ((cnt & 1) == 0) {
												if (lpChar) {
													lpChar->smCharInfo.wPlayClass[1] = fONE + 5 * 15;		//Å©±â È®´ë
													lpChar->sMoveStepCount[0] = 1;		//ÀÌµ¿ ¼Óµµ 2x2
													lpChar->sMoveStepCount[1] = 2;
													//lpChar->smCharInfo.Attack_Damage[0]/=2;
													//lpChar->smCharInfo.Attack_Damage[1]/=2;
													//lpChar->smMonsterInfo.SkillDamage[0]/=2;
													//lpChar->smMonsterInfo.SkillDamage[1]/=2;
												}
											}
											else {
												if (lpChar) {
													lpChar->smCharInfo.wPlayClass[1] = (fONE / 2) + 5 * 12;		//Å©±â È®´ë
													lpChar->sMoveStepCount[0] = 1;		//ÀÌµ¿ ¼Óµµ 2x2
													lpChar->sMoveStepCount[1] = 2;
													//lpChar->smCharInfo.Attack_Damage[0]/=2;
													//lpChar->smCharInfo.Attack_Damage[1]/=2;
													//lpChar->smMonsterInfo.SkillDamage[0]/=2;
													//lpChar->smMonsterInfo.SkillDamage[1]/=2;
												}
											}
										}
										break;
									case snCHAR_SOUND_NPC_MAGICMASTER:
										cnt = rand() % 100;
										if (cnt < 3 && lpLinkPlayInfo) {
											if (lpChar) {
												lpChar->sMoveStepCount[0] = 1;		//ÀÌµ¿ ¼Óµµ 2x2
												lpChar->sMoveStepCount[1] = 2;
												//lpChar->smCharInfo.Attack_Damage[0]/=2;
												//lpChar->smCharInfo.Attack_Damage[1]/=2;
												//lpChar->smMonsterInfo.SkillDamage[0]/=2;
												//lpChar->smMonsterInfo.SkillDamage[1]/=2;
											}
										}
										break;
									}
								}
							}
						}
					}
				}

				EvtFrame = 0;
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL) {		//º¸½º ¹Ùº§
					if (FrameCounter == 40 || FrameCounter == 80)
						EvtFrame = 1;
				}


				if ((!EvtFrame && FrameCounter == 40) || EvtFrame) {
					if (MotionInfo->State == CHRMOTION_STATE_ATTACK)
					{

						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_VAMPRICMACHINE) {		//¹ìÇÇ¸¯ ¸Ó½Å ¹ú¹ß»ç
							cnt = 0;
						}
						else {
							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL && (rand() % 100) < 60) {	//60%È®·ü
								//¹Ùº§ ÀÏ¹Ý°ø°Ý½Ã ½ºÅ³µµ °°ÀÌ Àû¿ë (¹üÀ§ÁöÁ¤)


								x = (lpTargetPlayInfo->Position.x - pX) >> FLOATNS;
								z = (lpTargetPlayInfo->Position.z - pZ) >> FLOATNS;
								dist = x * x + z * z;
								if (dist > (200 * 200)) {
									//¿ø°Å¸® 70%µ¥¹ÌÁö
									dm1 = smCharInfo.Attack_Damage[0];
									dm2 = smCharInfo.Attack_Damage[1];

									smCharInfo.Attack_Damage[0] = (smCharInfo.Attack_Damage[0] * 70) / 100;
									smCharInfo.Attack_Damage[1] = (smCharInfo.Attack_Damage[1] * 70) / 100;
									cnt = SendTransAttack(0, lpTargetPlayInfo->lpsmSock, 0);

									smCharInfo.Attack_Damage[0] = dm1;
									smCharInfo.Attack_Damage[1] = dm2;
								}
								else {
									//±Ù°Å¸® 100% µ¥¹ÌÁö
									cnt = SendTransAttack(0, lpTargetPlayInfo->lpsmSock, 0);
								}

							}
							else {
								cnt = SendTransAttack(0, lpTargetPlayInfo->lpsmSock, 0);
							}
						}

						if (cnt) {
							//°ø°Ý µ¥ÀÌÅ¸ ±â·Ï
							lpTargetPlayInfo->Send_AttackCount++;
							lpTargetPlayInfo->Send_AttackDamage += cnt;

							//ÇØ¿Ü//////////////////////////////////////////////////////////////////////////////////////////////////
							/*							if ( rsServerConfig.Event_Child==1 && PartyFlag!=rsHARDCORE_EVENT_FLAG )
														{
															//¾î¸°ÀÌ ÀÌº¥Æ®
															if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_BUMA ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_SEN ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_BEEDOG ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_MINIG ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_DECOY ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_MUMMY ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_HULK ||
																smCharInfo.dwCharSoundCode==snCHAR_SOUND_SANDLEM ) {

																if ( (rand()%70)==1 ) {
																	smTransCommand.size = sizeof(smTRANS_COMMAND);
																	smTransCommand.code = smTRANSCODE_COMIC_SKIN;
																	smTransCommand.WParam = (rand()%2)+1;
																	smTransCommand.LParam = 60*10;
																	smTransCommand.SParam = 0;
																	smTransCommand.EParam = 0;
																	lpTargetPlayInfo->lpsmSock->Send( (char *)&smTransCommand , smTransCommand.size , TRUE );
																}
															}
														}
							*/   /////////////////////////////////////////////////////////////////////////////////////////////////////
							//»Í¸ÁÄ¡ ÀÌº¥Æ®
							if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_HAMMER &&
								lpTargetPlayInfo->smCharInfo.SizeLevel <= 0x1000 || lpTargetPlayInfo->smCharInfo.SizeLevel > 0x1002) {

								if (dwEventAttackParam != lpTargetPlayInfo->dwObjectSerial) {

									EventAttackCount = 0;
									dwEventAttackParam = lpTargetPlayInfo->dwObjectSerial;
								}

								EventAttackCount++;

								if (EventAttackCount >= 10) {

									smTransCommand.size = sizeof(smTRANS_COMMAND);
									smTransCommand.code = smTRANSCODE_COMIC_SKIN;
									smTransCommand.WParam = (rand() % 2) + 1;
									smTransCommand.LParam = 60 * 10;
									smTransCommand.SParam = 0;
									smTransCommand.EParam = 0;
									lpTargetPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

									if (smMonsterInfo.UseEventModel && smCharInfo.szModelName2[1]) {
										//ÀÌº¥Æ® ¸ðµ¨ »ç¿ëÁß

										Close();

										lstrcpy(smCharInfo.szModelName, smCharInfo.szModelName2 + 1);
										SetLoadPattern(smCharInfo.szModelName);
										smCharInfo.bUpdateInfo[0]++;
										SetMotionFromCode(CHRMOTION_STATE_STAND);

									}

									smCharInfo.wPlayClass[0] = MONSTER_CLASS_NORMAL;
									smMonsterInfo.UseEventModel = 0;
								}

								TRANS_CHATMESSAGE	TransChatMessage;
								wsprintf(TransChatMessage.szMessage, "%s: %d ", smCharInfo.szName, EventAttackCount);
								TransChatMessage.code = smTRANSCODE_WHISPERMESSAGE;
								TransChatMessage.size = 32 + lstrlen(TransChatMessage.szMessage);
								TransChatMessage.dwIP = 0;
								TransChatMessage.dwObjectSerial = dwObjectSerial;

							}
						}

						AttackUserFlag = TRUE;
						AttackAnger++;				//°ø°Ý È½¼ö Ä«¿îÅÍ

						if (lpTargetPlayInfo->BadPlayer == 4 && lpTargetPlayInfo->lpsmSock) {
							if ((smCharInfo.Level + 6) > lpTargetPlayInfo->smCharInfo.Level && (rand() % 100) > 92) {
								//¹èµå ÇÃ·¹ÀÌ¾î È®ÀÎ °­Á¦·Î Á×ÀÌ±â
								smTransCommand.code = smTRANSCODE_DEAD_PLAYER;
								smTransCommand.size = sizeof(smTRANS_COMMAND);
								smTransCommand.WParam = 0;
								smTransCommand.LParam = 0;
								smTransCommand.SParam = 0;
								smTransCommand.EParam = 0;
								lpTargetPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
								lpTargetPlayInfo->BadPlayer = 2;		//Á×ÀÎÈÄ ÇØÁ¦
							}
						}
					}
				}

				EvtFrame = 32;

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL || smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEATHKNIGHT) {		//º¸½º ¹Ùº§
					EvtFrame = 120;
					if (FrameCounter == EvtFrame) {							//ÇÇ Ã¤¿öÁØ´Ù
						//¸ó½ºÅÍ »ý¸í·Â Àç»ýÄ¡ ±¸ÇÏ´Â ÇÔ¼ö


						if (smCharInfo.Life[0] > smCharInfo.Life[1] || smCharInfo.Life[0] < 0)
							smCharInfo.Life[0] = smCharInfo.Life[1];

					}
				}

				if (FrameCounter == EvtFrame) {
					if (MotionInfo->State == CHRMOTION_STATE_SKILL) {
						if (smCharInfo.dwCharSoundCode != snCHAR_SOUND_MYSTIC) {				//¹Ì½ºÆ½ ÀúÁÖ µ¥¹ÌÁö ¾øÀ½
							AttackSkillRange = smMonsterInfo.SkillRange * smMonsterInfo.SkillRange;
							if (!AttackSkillRange && lpTargetPlayInfo) {
								//»ó´ë Ä³¸¯ÅÍ¿¡ ±â¼ú °ø°Ý °¡ÇÔ 

							}
							AttackUserFlag = TRUE;
						}
					}
				}

			}
			else {
				SetNextTarget(0, 0, 0);
			}
		}


		if (PlaySlowCount > 0) 		//¸ó½ºÅÍ´Â Àú¼Ó »óÅÂ À¯Áö ¸ðµå·Î »ç¿ë 
			PlaySlowCount--;

		if (PlaySlowCount < 0) 		//¸ó½ºÅÍ´Â Àú¼Ó »óÅÂ À¯Áö ¸ðµå·Î »ç¿ë 
			PlaySlowCount++;

		if (PlayStunCount > 0)		//¸ó½ºÅÍ´Â Áß¸³ »óÅÂ À¯Áö ¸ðµå·Î »ç¿ë (·Î¾î °ø°Ý ÇØ´ç )
			PlayStunCount--;

		if (PlayStopCount > 0)		//¸ó½ºÅÍ´Â ¿òÁ÷ÀÓÀÌ ¸ØÃç ÀÖ´Ù
			PlayStopCount--;

		if (PlayHolyMind[1] > 0)	//µ¥¹ÌÁö ¾àÈ­
			PlayHolyMind[1]--;

		if (PlayHolyPower[1] > 0)	//µ¥¹ÌÁö °­È­
			PlayHolyPower[1]--;

		if (PlayPoison[1] > 0) {
			PlayPoison[1]--;			//µ¶ °¨¿°
			if ((Counter & 0xF) == 0) {
				if (smCharInfo.Life[0] > 0 && MotionInfo->State != CHRMOTION_STATE_DEAD) {
					smCharInfo.Life[0] -= PlayPoison[0];			//µ¥¹ÌÁö °¨¼Ò
					if (smCharInfo.Life[0] < 1) {				//Á×À½
						smCharInfo.Life[0] = 1;
						//SetMotionFromCode( CHRMOTION_STATE_DEAD );
						PlayPoison[1] = 0;
					}
				}
			}
		}

		if (PlayHolyIncantation[1] > 0) {
			PlayHolyIncantation[1]--;
			if (PlayHolyIncantation[1] == 0 && smCharInfo.Brood == smCHAR_MONSTER_USER) {
				smCharInfo.Brood = PlayHolyIncantation[0];	//smCHAR_MONSTER_USER
				smCharInfo.bUpdateInfo[0]++;
				lpMasterPlayInfo = 0;
			}
		}

		if (UseObject_VirtualLife) {
			//°¡»óÀÇ »ý¸í·Â
			if (sObject_VirtualLife[0] > 0) {
				cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
				if (cnt <= 10) { //10%ÀÌÇÏ Àç»ý
					sObject_VirtualLife[0] --;
					smCharInfo.Life[0] = smCharInfo.Life[1];
				}
			}

			//Ãâ·Â¿ë »ý¸í·Â
			cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
			sObject_DisplayLife[0] = sObject_VirtualLife[0] * 100;
			sObject_DisplayLife[0] += cnt;
			sObject_DisplayLife[1] = (sObject_VirtualLife[1] + 1) * 100;

			if (sObject_DisplayLife[0] <= 0 && smCharInfo.Life[0] > 0) {
				sObject_DisplayLife[0] = 1;
			}

			if (sObject_DisplayLife[0] > sObject_DisplayLife[1])
				sObject_DisplayLife[0] = sObject_DisplayLife[1];

		}

		FrameCounter += 3;
		break;


	case 10:	//µ¿¹°
	case 11:	//NPC
		if (MotionInfo->State && MotionInfo->State < 0x100) {
			if (MotionInfo->State == CHRMOTION_STATE_WALK) {
				MoveAngle(SERVER_CHAR_SKIP);
				if (MotionInfo->State != CHRMOTION_STATE_WALK) SetMotionFromCode(CHRMOTION_STATE_WALK);
			}
			else {
				if (MotionInfo->State != CHRMOTION_STATE_STAND) SetMotionFromCode(CHRMOTION_STATE_STAND);
			}

		}

		frame += SERVER_CHAR_FRAME_STEP;

		if (frame > MotionInfo->EndFrame * 160) {

			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				frame = MotionInfo->EndFrame * 160;
			}
			else {

				if (MotionInfo->Repeat == TRUE) {
					frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
				}
				else SetMotionFromCode(CHRMOTION_STATE_STAND);



				if (MotionInfo->State == CHRMOTION_STATE_STAND) {
					cnt = rand() % ANGCLIP;
					if (((cnt >> 2) & 3) == 0) {
						//¶§¶§·Î ÇÏ´Â µ¿ÀÛ
						if (ActionPattern == 10) {
							if (!SetMotionFromCode(CHRMOTION_STATE_SOMETIME)) {
								Angle.y = cnt & ANGCLIP;
								SetMotionFromCode(CHRMOTION_STATE_WALK);
							}
						}
						else {
							if (!SetMotionFromCode(TRUE)) {
								Angle.y = cnt & ANGCLIP;
								SetMotionFromCode(CHRMOTION_STATE_WALK);
							}
						}
					}
					else {
						Angle.y = cnt & ANGCLIP;
						SetMotionFromCode(CHRMOTION_STATE_WALK);
					}

					if (MotionInfo->State == CHRMOTION_STATE_WALK) {
						//°Å¸®¸¦ ¹þ¾î³­°æ¿ì Ã³¸®
						x = (pX - PosiAround.x) >> FLOATNS;
						z = (pZ - PosiAround.z) >> FLOATNS;
						dist = x * x + z * z;
						if (dist > DistAroundDbl) {
							SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
						}
					}
				}
				else SetMotionFromCode(CHRMOTION_STATE_STAND);

				if (OverLapPosi && lpCharOverLap) {
					//Ä³¸¯ÅÍ°¡ °ãÃÄ ÀÖÀ½ ( ¶³¾îÁöµµ·Ï ³ë·Â ÇÔ )
					Angle.y = GetRadian2D(lpCharOverLap->pX >> FLOATNS, lpCharOverLap->pZ >> FLOATNS,
						pX >> FLOATNS, pZ >> FLOATNS);

					SetMotionFromCode(CHRMOTION_STATE_WALK);
					OverLapPosi2 = FALSE;
				}
			}
		}
		SavePlayBuff();
		FrameCounter += 3;
		break;

	case 12:	//NPC
		//######################################################################################
		// ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®	 ( ¼­¹ö ÄÚµå¿¡ Ãß°¡ )
		// NPC ´Â smMotionInfo2 »ç¿ëÄ¡ ¾ÊÀ½. NPC ´Â °íÁ¤ÀÌ´Ù.		
		if (smMotionInfo->NpcMotionRate[CHRMOTION_EXT])
		{
			///////////////////////////////////////////////////////////////////
			if (MotionInfo->State == CHRMOTION_STATE_WARP)
			{
				//¿öÇÁ µ¿ÀÛ
				frame += SERVER_CHAR_FRAME_STEP;
				if (frame > MotionInfo->EndFrame * 160)
				{
					Close();
					return TRUE;
				}
				SavePlayBuff();
				FrameCounter += 3;
				break;
			}
			///////////////////////////////////////////////////////////////////

			int TalkCnt = 0;
			if (MotionInfo->State == CHRMOTION_STATE_WALK)
			{
				x = pX; z = pZ;
				MoveAngle(SERVER_CHAR_SKIP);

				if (x == pX && z == pZ)
				{
					TalkCnt = rand() % 100;
					TalkCnt = smMotionInfo->NpcMotionRateCnt[TalkCnt];

					if (TalkCnt == action && smMotionInfo->MotionInfo[TalkCnt].State == CHRMOTION_STATE_WALK)
					{
						//°Å¸®¸¦ ¹þ¾î³­°æ¿ì Ã³¸®
						x = (pX - PosiAround.x) >> FLOATNS;
						z = (pZ - PosiAround.z) >> FLOATNS;
						dist = x * x + z * z;
						if (dist > DistAroundDbl)
							SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
					}
					else
					{
						frame = MotionInfo->EndFrame * 160;
						MotionInfo->State = CHRMOTION_STATE_STAND;
					}
				}
			}

			frame += SERVER_CHAR_FRAME_STEP;
			if (frame > MotionInfo->EndFrame * 160)
			{
				if (MotionInfo->State == CHRMOTION_STATE_WALK)
					frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
				else if (smCharInfo.dwCharSoundCode != snCHAR_SOUND_FURY)	// Ç»¸®´Â °ÈÁö ¾Ê°Ô
				{
					if (TalkCnt == 0)
						TalkCnt = rand() % 100;

					ChangeMotion(smMotionInfo->NpcMotionRateCnt[TalkCnt]);
					if (MotionInfo->State == CHRMOTION_STATE_WALK)
					{
						//°Å¸®¸¦ ¹þ¾î³­°æ¿ì Ã³¸®
						x = (pX - PosiAround.x) >> FLOATNS;
						z = (pZ - PosiAround.z) >> FLOATNS;
						dist = x * x + z * z;
						if (dist > DistAroundDbl)
							SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
					}
					else
						Angle.y = rand() % ANGCLIP;
				}
				else // Ç»¸® Àü¿ë.
					SetMotionFromCode(CHRMOTION_STATE_STAND);
			}
		}
		//######################################################################################
		else // NpcMotionRate °ªÀÌ ¾ø´Ù. (±âÁ¸ NPC )
		{
			if (MotionInfo->State == CHRMOTION_STATE_WARP) {
				//¿öÇÁ µ¿ÀÛ
				frame += SERVER_CHAR_FRAME_STEP;

				if (frame > MotionInfo->EndFrame * 160) {
					Close();
					return TRUE;
				}
				SavePlayBuff();
				FrameCounter += 3;
				break;
			}


			if (MotionInfo->State && MotionInfo->State < 0x100) {
				if (MotionInfo->State == CHRMOTION_STATE_WALK) {
					x = pX; z = pZ;

					MoveAngle(SERVER_CHAR_SKIP);

					if (x == pX && z == pZ) {
						Angle.y = rand() % ANGCLIP;
					}

					if (MotionInfo->State != CHRMOTION_STATE_WALK) SetMotionFromCode(CHRMOTION_STATE_WALK);
				}
				else {
					if (MotionInfo->State != CHRMOTION_STATE_STAND) SetMotionFromCode(CHRMOTION_STATE_STAND);
				}
			}

			frame += SERVER_CHAR_FRAME_STEP;

			if (frame > MotionInfo->EndFrame * 160) {

				if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
					frame = MotionInfo->EndFrame * 160;
				}
				else {

					if (MotionInfo->Repeat == TRUE) {
						frame = (MotionInfo->StartFrame * 160) + (frame - MotionInfo->EndFrame * 160);
					}
					else SetMotionFromCode(CHRMOTION_STATE_STAND);



					if (MotionInfo->State == CHRMOTION_STATE_STAND && smCharInfo.dwCharSoundCode != snCHAR_SOUND_FURY) {	//Ç»¸®´Â °ÈÁö ¾Ê°Ô
						cnt = rand() % ANGCLIP;
						if (((cnt >> 2) & 3) == 0) {
							//¶§¶§·Î ÇÏ´Â µ¿ÀÛ
							if (!SetMotionFromCode(TRUE)) {
								if (MoveSpeed) Angle.y = cnt & ANGCLIP;

								if (DistAroundDbl)
									SetMotionFromCode(CHRMOTION_STATE_WALK);
								else
									SetMotionFromCode(CHRMOTION_STATE_STAND);
							}
						}
						else {
							SetMotionFromCode(CHRMOTION_STATE_STAND);
						}

						if (MotionInfo->State == CHRMOTION_STATE_WALK) {
							//°Å¸®¸¦ ¹þ¾î³­°æ¿ì Ã³¸®
							x = (pX - PosiAround.x) >> FLOATNS;
							z = (pZ - PosiAround.z) >> FLOATNS;
							dist = x * x + z * z;
							if (dist > DistAroundDbl) {
								SetTargetPosi(PosiAround.x >> FLOATNS, PosiAround.z >> FLOATNS);
							}
						}
					}
					else SetMotionFromCode(CHRMOTION_STATE_STAND);
					/*
									if ( OverLapPosi && lpCharOverLap ) {
										//Ä³¸¯ÅÍ°¡ °ãÃÄ ÀÖÀ½ ( ¶³¾îÁöµµ·Ï ³ë·Â ÇÔ )
										Angle.y = GetRadian2D( lpCharOverLap->pX>>FLOATNS , lpCharOverLap->pZ>>FLOATNS ,
											pX>>FLOATNS , pZ>>FLOATNS );

										SetMotionFromCode( CHRMOTION_STATE_WALK );
										OverLapPosi2 = FALSE;
									}
					*/
				}
			}
		}

		SavePlayBuff();
		FrameCounter += 3;
		break;

		/*
	case 12:	//NPC
		if ( MotionInfo->State == CHRMOTION_STATE_WARP ) {
			//¿öÇÁ µ¿ÀÛ
			frame += SERVER_CHAR_FRAME_STEP;

			if ( frame>MotionInfo->EndFrame*160 ) {
				Close();
				return TRUE;
			}
			SavePlayBuff();
			FrameCounter+=3;
			break;
		}


		if ( MotionInfo->State && MotionInfo->State<0x100 ) {
			if ( MotionInfo->State==CHRMOTION_STATE_WALK ) {
				x=pX; z=pZ;

				MoveAngle( SERVER_CHAR_SKIP );

				if ( x==pX && z==pZ ) {
					Angle.y = rand()%ANGCLIP;
				}

				if ( MotionInfo->State!=CHRMOTION_STATE_WALK ) SetMotionFromCode( CHRMOTION_STATE_WALK );
			}
			else {
				if ( MotionInfo->State!=CHRMOTION_STATE_STAND ) SetMotionFromCode( CHRMOTION_STATE_STAND );
			}
		}

		frame += SERVER_CHAR_FRAME_STEP;

		if ( frame>MotionInfo->EndFrame*160 ) {

			if ( MotionInfo->State == CHRMOTION_STATE_DEAD ) {
				frame = MotionInfo->EndFrame*160;
			}
			else {

				if ( MotionInfo->Repeat == TRUE ) {
					frame = (MotionInfo->StartFrame*160)+(frame - MotionInfo->EndFrame*160);
				}
				else SetMotionFromCode( CHRMOTION_STATE_STAND );



				if ( MotionInfo->State==CHRMOTION_STATE_STAND && smCharInfo.dwCharSoundCode!=snCHAR_SOUND_FURY ) {	//Ç»¸®´Â °ÈÁö ¾Ê°Ô
					cnt = rand()%ANGCLIP;
					if ( ((cnt>>2)&3)==0 ) {
						//¶§¶§·Î ÇÏ´Â µ¿ÀÛ
							if ( !SetMotionFromCode( TRUE ) ) {
								if ( MoveSpeed ) Angle.y = cnt&ANGCLIP;

								if ( DistAroundDbl )
									SetMotionFromCode( CHRMOTION_STATE_WALK );
								else
									SetMotionFromCode( CHRMOTION_STATE_STAND );
							}
					}
					else {
						SetMotionFromCode( CHRMOTION_STATE_STAND );
					}

					if ( MotionInfo->State==CHRMOTION_STATE_WALK ) {
						//°Å¸®¸¦ ¹þ¾î³­°æ¿ì Ã³¸®
						x = (pX-PosiAround.x)>>FLOATNS;
						z = (pZ-PosiAround.z)>>FLOATNS;
						dist = x*x+z*z;
						if ( dist>DistAroundDbl ) {
							SetTargetPosi( PosiAround.x>>FLOATNS , PosiAround.z>>FLOATNS );
						}
					}
				}
				else SetMotionFromCode( CHRMOTION_STATE_STAND );
			}
		}
		SavePlayBuff();
		FrameCounter+=3;
		break;
		*/



	case 99:

		if (PlayBuffCnt == PlayBuffPosi_End) {

			if (MotionInfo->State != CHRMOTION_STATE_STAND && MotionInfo->State != CHRMOTION_STATE_DEAD && MotionInfo->State != CHRMOTION_STATE_WARP &&
				MotionInfo->State != CHRMOTION_STATE_ATTACK && MotionInfo->State != CHRMOTION_STATE_SKILL)
				SetMotionFromCode(CHRMOTION_STATE_STAND);

			frame += 80;
			compFrame = frame / 160;

			if (compFrame >= MotionInfo->EndFrame) {
				if (MotionInfo->Repeat) {
					ChangeMotion(action, AnimDispMode);
				}
				else
					frame = (MotionInfo->EndFrame) * 160;

				if (MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL)
					SetMotionFromCode(CHRMOTION_STATE_STAND);

			}
			TransDelayCheck(); //crap.. ignore

		}
		else {
			playBuff = &PlayBuff[PlayBuffCnt & PLAYBUFF_MASK];
			OldAction = action;

			Angle.x = playBuff->angle[0];
			Angle.y = playBuff->angle[1];
			Angle.z = playBuff->angle[2];
			action = playBuff->angle[3];

			if (!action) {
				action = OldAction;

				x = abs(pX - playBuff->x);
				y = abs(pY - playBuff->y);
				z = abs(pZ - playBuff->z);

				cnt = x + y + z;

				if (cnt > 32) {
					if (MotionInfo->State != CHRMOTION_STATE_WALK && MotionInfo->State != CHRMOTION_STATE_RUN) {

						if (cnt < 512) {
							//°È±â
							SetMotionFromCode(CHRMOTION_STATE_WALK);
						}
						else {
							//¶Ù±â
							if (!SetMotionFromCode(CHRMOTION_STATE_RUN))
								SetMotionFromCode(CHRMOTION_STATE_WALK);
						}
					}
				}
				else {
					if (MotionInfo->State != CHRMOTION_STATE_STAND) {
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					}
				}
			}

			pX = playBuff->x;
			pY = playBuff->y;
			pZ = playBuff->z;

			PHeight = pY;


			if (!playBuff->dwTargetSerial) chrAttackTarget = 0;
			else {
				if (chrAttackTarget && chrAttackTarget->dwObjectSerial == playBuff->dwTargetSerial) {

				}
				else
					chrAttackTarget = FindChrPlayer(playBuff->dwTargetSerial);
			}

			if (playBuff->frame < smPLAYBUFF_FRAME_NONE) {
				//				if ( OldAction!=action ) {// || !MotionInfo->State || MotionInfo->State>=0x100 ) {

								//ÇÁ·¹ÀÓ ¼³Á¤
				frame = playBuff->frame & CHAR_FRAME_MASK;

				//ÇØ´ç ÇÁ·¹ÀÓÀÇ ¸ðµ¨¼±ÅÃ
				MotionSelectFrame = playBuff->frame >> CHAR_FRAME_SELSHIFT;
				if (AnimDispMode) {
					if (MotionSelectFrame)
						PatDispMode = DISP_MODE_PATSUB;			//º¸Á¶ ¸ðµ¨
					else {
						PatDispMode = DISP_MODE_PATMAIN;		//ÁÖ ¸ðµ¨
					}
				}

				if (action) {

					if (PatDispMode == DISP_MODE_PATSUB)
						MotionInfo = &smMotionInfo2->MotionInfo[action];
					else
						MotionInfo = &smMotionInfo->MotionInfo[action];

					//######################################################################################
					//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
					FrameInfo.StartFrame = MotionInfo->StartFrame * 160;
					FrameInfo.EndFrame = MotionInfo->EndFrame * 160;
					//######################################################################################
				}

				if (OldMotionState != MotionInfo->State) {

					if (MotionInfo->State >= 0x100 && MotionInfo->State != CHRMOTION_STATE_ATTACK && MotionInfo->State != CHRMOTION_STATE_SKILL) {
						//if ( MotionInfo->State!=CHRMOTION_STATE_DEAD || (OnStageField>=0 && StageField[OnStageField]->State!=FIELD_STATE_VILLAGE) ) {
							//Ä³¸¯ÅÍ »ç¿îµå
						CharPlaySound(this);
						//}
					}
					else {
						//°¡²û ³»´Â ¸ó½ºÅÍ ¼Ò¸®
						if (smCharInfo.State == smCHAR_STATE_ENEMY && MotionInfo->State == CHRMOTION_STATE_STAND && (rand() % 4) == 0)
							CharPlaySound(this);
					}

					/*
										//´Ù½Ã ½ÃÀÛ ÀÌÆåÆ®
										if ( MotionInfo->State==CHRMOTION_STATE_DAMAGE )
											//ÇÇÆ¢±â´Â ÆÄÆ¼Å¬
											StartEffect( pX, pY+18*fONE, pZ, 50,50 , EFFECT_DAMAGE1 );
					*/
					//if ( AttackSkil && !MotionEvent && (OldMotionState==CHRMOTION_STATE_ATTACK || OldMotionState==CHRMOTION_STATE_SKILL) ) {
					if (!MotionEvent && (OldMotionState == CHRMOTION_STATE_ATTACK || OldMotionState == CHRMOTION_STATE_SKILL)) {
						if (MotionInfo->EventFrame[0]) {
							EventAttack();				//½ºÅµµÈ ÀÌº¥Æ® ½ÇÇà
							//PlayAttack(TRUE);									//°ø°Ý Àû¿ë
							dwAttackPlayTime = 0;		//¿ø°Å¸® Ä³¸¯ÀÇ °ø°ÝÀº Á¦¿Ü
							ResetAttackTrans();		//AttackTrans.code = 0;
						}
						PlayAttack();									//°ø°Ý Àû¿ë
					}
					MotionEvent = 0;

					if (playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL] >= 0) {
						//½ºÅ³½ÃÀÛ
						BeginSkill(playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL], 0, chrAttackTarget, 0, 0, 0);
						//AttackSkil = playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL];
					}

					if (smCharInfo.State == smCHAR_STATE_ENEMY) {
						if (MotionInfo->State == CHRMOTION_STATE_SKILL)
							BeginSkill(0, 0, chrAttackTarget, 0, 0, 0);

						if (MotionInfo->State == CHRMOTION_STATE_ATTACK)
							BeginAttack_Monster();

						if (OldMotionState == CHRMOTION_STATE_SKILL) {
							PlayAttack();									//°ø°Ý Àû¿ë
						}

					}

					switch (MotionInfo->State) {
					case CHRMOTION_STATE_RESTART:
						StartEffect(pX, pY, pZ, EFFECT_GAME_START1);
						SetDynLight(pX, pY, pZ, 100, 100, 100, 0, 200);
						CharPlaySound(this);
						break;
					case CHRMOTION_STATE_WARP:
						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN) {
							//¼ø°£ ÀÌµ¿ ÀÌÆåÆ®
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
							SkillPlaySound(SKILL_SOUND_LEARN, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
							SetDynLight(pX, pY, pZ, 50, 100, 70, 0, 300, 1);
							CharPlaySound(this);
						}
						break;

					case CHRMOTION_STATE_DEAD:
						FrameCounter = 0;

						// Velocidade do efeito dissolve quando o mob morre
						DissolveAmount = 0.006f;

						switch (smCharInfo.dwCharSoundCode)
						{
						case snCHAR_SOUND_DEADZONE:
							AssaParticle_MonsterDeadZoneDead(this);
							break;
						case snCHAR_SOUND_HYPERMACHINE:
							AssaParticle_MonsterHyperDead(this);
							break;
						case snCHAR_SOUND_MYSTIC:
							ParkAssaParticle_MisticSpinel(this);
							break;
						case snCHAR_SOUND_DARKMAGE:
							ParkAssaParticle_DarkMageDead(this);
							break;
						}
						break;
					}
				}
			}
			else {
				frame += playBuff->frame & 0xFFFF;
				//frame += 80;
			}

			/*
						if ( PatDispMode==DISP_MODE_PATSUB && MotionInfo->State<0x100  ) {

							compFrame = frame / 160;

						}
			*/

			AttackSkil = playBuff->wStickItems[smPLAYBUFF_ITEM_SKIL];
			EventAttack();			//°ø°Ý ÀÌº¥Æ®

			if (smCharInfo.State != smCHAR_STATE_USER &&
				(MotionInfo->State == CHRMOTION_STATE_WALK || MotionInfo->State == CHRMOTION_STATE_RUN)) {
				//¹ßÀÚ±¹ ¼Ò¸® ( ¹Ù°ï ¹× °Å´ë ¸ó½ºÅÍ )
				if (MotionInfo->EventFrame[0]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[0] > (compFrame - 80) && MotionInfo->EventFrame[0] <= compFrame) {
						CharPlaySound(this);
					}
				}

				if (MotionInfo->EventFrame[1]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[1] > (compFrame - 80) && MotionInfo->EventFrame[1] <= compFrame) {
						CharPlaySound(this);
					}
				}
			}

			FrameStep = 80;

			if (MotionInfo->State == CHRMOTION_STATE_EAT) {
				//¹°¾à ¸Ô±â µ¿ÀÛ
				if (MotionInfo->EventFrame[0]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) {
						if (smCharInfo.State == smCHAR_STATE_USER)
							cnt = pY + 48 * fONE;
						else
							cnt = pY + PatHeight;

						switch (PotionLog) {
						case 1:
							StartEffect(pX, cnt, pZ, EFFECT_POTION1);
							sinPlaySound(SIN_SOUND_EAT_POTION, GetDistVolume(pX, cnt, pZ));			//¹°¾à¸Ô´Â ¼Ò¸®
							break;
						case 2:
							StartEffect(pX, cnt, pZ, EFFECT_POTION2);
							sinPlaySound(SIN_SOUND_EAT_POTION, GetDistVolume(pX, cnt, pZ));			//¹°¾à¸Ô´Â ¼Ò¸®
							break;
						case 3:
							StartEffect(pX, cnt, pZ, EFFECT_POTION3);
							sinPlaySound(SIN_SOUND_EAT_POTION, GetDistVolume(pX, cnt, pZ));			//¹°¾à¸Ô´Â ¼Ò¸®
							break;
						case 4:
							StartEffect(pX, cnt, pZ, EFFECT_POTION3);
							StartEffect(pX, cnt, pZ, EFFECT_POTION2);
							StartEffect(pX, cnt, pZ, EFFECT_POTION1);
							sinPlaySound(SIN_SOUND_EAT_POTION, GetDistVolume(pX, cnt, pZ));			//¹°¾à¸Ô´Â ¼Ò¸®
							break;

						case POTION_ETHERCORE:
							StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
							SkillPlaySound(SKILL_SOUND_LEARN, pX, cnt, pZ);			//½ºÅ³ È¿°úÀ½
						}
						SetDynLight(pX, cnt, pZ, 50, 100, 70, 0, 250, 1);
					}
				}
			}

			compFrame = frame / 160;

			if (compFrame < MotionInfo->StartFrame)
				frame = MotionInfo->StartFrame * 160;


			if (compFrame >= MotionInfo->EndFrame) {
				if (MotionInfo->Repeat) {
					ChangeMotion(action, AnimDispMode);
				}
				else
					frame = (MotionInfo->EndFrame) * 160;

				AttackEffect = 0;
			}

			/*
						if ( compFrame>=smMotionInfo->MotionInfo[action].EndFrame ) {
							if ( smMotionInfo->MotionInfo[action].Repeat ) {
								ChangeMotion( action );
							}
							else
								frame = (smMotionInfo->MotionInfo[action].EndFrame)*160;
						}
			*/

			////// ÀåÂøÇÑ ¾ÆÀÌÅÛ ÄÚµå È®ÀÎÇÏ¿© º¯°æ »çÇ× Àû¿ë ////////////
			if ((smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) && DisplayFlag) {

				// ¿ÞÂÊ ÀåÂø ¾ÆÀÌÅÛ
				if (playBuff->wStickItems[smPLAYBUFF_ITEM_LEFT] != wStickItems[smPLAYBUFF_ITEM_LEFT]) {
					if (playBuff->wStickItems[smPLAYBUFF_ITEM_LEFT] >= 0) {
						SetTool(sItem[playBuff->wStickItems[smPLAYBUFF_ITEM_LEFT]].CODE, hvPOSI_LHAND);
						HvLeftHand.ColorBlink = 0;
						HvLeftHand.DispEffect = 0;
					}
					else
						SetTool(0, hvPOSI_LHAND);
				}
				// ¿À¸¥ÂÊ ÀåÂø ¾ÆÀÌÅÛ
				if (playBuff->wStickItems[smPLAYBUFF_ITEM_RIGHT] != wStickItems[smPLAYBUFF_ITEM_RIGHT]) {
					if (playBuff->wStickItems[smPLAYBUFF_ITEM_RIGHT] >= 0) {
						SetTool(sItem[playBuff->wStickItems[smPLAYBUFF_ITEM_RIGHT]].CODE, hvPOSI_RHAND);
						HvRightHand.ColorBlink = 0;
						HvRightHand.DispEffect = 0;
					}
					else
						SetTool(0, hvPOSI_RHAND);
				}
				// ¸ö ÀåÂø ¾ÆÀÌÅÛ( °©¿Ê µî )
				if (playBuff->wStickItems[smPLAYBUFF_ITEM_BODY] != wStickItems[smPLAYBUFF_ITEM_BODY]) {
					SetChrArmor(this, playBuff->wStickItems[smPLAYBUFF_ITEM_BODY]);
					wStickItems[smPLAYBUFF_ITEM_BODY] = playBuff->wStickItems[smPLAYBUFF_ITEM_BODY];
				}
			}



			PlayBuffCnt++;
			MoveCnt = 0;

			//ºÐ³ëÄ¡ º¯È­
			//if ( (( MotionInfo->State==CHRMOTION_STATE_ATTACK || MotionInfo->State==CHRMOTION_STATE_DAMAGE )&& 
			//	PatDispMode==DISP_MODE_PATSUB) ) {
			if (((MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_DAMAGE) &&
				smCharInfo.dwCharSoundCode == snCHAR_SOUND_MINIG)) {
				AttackAnger += 2;
				if (AttackAnger > 100) AttackAnger = 100;
			}
			else {
				AttackAnger -= 1;
				if (AttackAnger <= 0) AttackAnger = 0;
			}

			if (AttackIce != 0) {
				//¾óÀ½ °ø°Ý
				if (AttackIce > 0 && AttackIce < 160) AttackIce++;
				if (AttackIce < 0) AttackIce++;
				if (MotionInfo->State == CHRMOTION_STATE_DEAD) AttackIce = 0;
			}

			if (DispPoison != 0) {
				//µ¶¿¡ °¨¿°
				if (DispPoison > 0 && DispPoison < 100) DispPoison++;
				if (DispPoison < 0) DispPoison++;
				if (MotionInfo->State == CHRMOTION_STATE_DEAD) DispPoison = 0;
			}

			if (PlayDistortion > 0) {
				//¿Ö°î ±â´É
				PlayDistortion--;
			}

			//¹Ù´Ú È®ÀÎ¿ë
			if ((PlayBuffCnt & 0x3F) == 0 && smCharInfo.State != TRUE) {
				OnStageField = -1;
				if (smGameStage[0]) {
					cnt = smGameStage[0]->GetFloorHeight(pX, pY, pZ, Pattern->SizeHeight);
					OnStageField = 0;
				}
				if (cnt == CLIP_OUT && smGameStage[1]) {
					OnStageField = 1;
				}
			}

			if (dwAttackPlayTime && dwAttackPlayTime < dwPlayTime) {
				//°ø°Ý ½Ã°£¿¹¾à Àû¿ë
				PlayAttack(TRUE);
				dwAttackPlayTime = 0;
			}


			OldMotionState = MotionInfo->State;


			//°ÔÀÓ µ¥ÀÌÅ¸ Àü¼ÛÀÌ Ã³À½ µÇ¾úÀ»¶§ Ä³¸¯ÅÍ È­¸é¿¡ Ç¥½Ã
			DisplayFlag = TRUE;
		}

		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN && smCharInfo.State == smCHAR_STATE_ENEMY) {
			if ((Counter & 0x1) == 0 && MotionInfo->State != CHRMOTION_STATE_DEAD && MotionInfo->State == CHRMOTION_STATE_RUN) {
				GetMoveLocation(0, 0, ((rand() % 48) - 24) * fONE, 0, Angle.y, 0);
				sinSetEffect(SIN_PARTICLE_SPOUT, pX + GeoResult_X, pY + 30 * fONE, pZ + GeoResult_Z, 1000, SIN_PARTICLE_COLOR_GOLD);
			}
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (sAddColorEffect[SMC_R] > 0) sAddColorEffect[SMC_R]--;
				if (sAddColorEffect[SMC_G] > 0) sAddColorEffect[SMC_G]--;
				if (sAddColorEffect[SMC_B] > 0) sAddColorEffect[SMC_B]--;
			}
		}

		if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS) {
			//º¸½º¸ó½ºÅÍ Á×À½
			if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
				if (sAddColorEffect[SMC_R] > 0) sAddColorEffect[SMC_R]--;
				if (sAddColorEffect[SMC_G] > 0) sAddColorEffect[SMC_G]--;
				if (sAddColorEffect[SMC_B] > 0) sAddColorEffect[SMC_B]--;
			}
			/*
			else {
				if ( RendSucess && RendPoint.z>0 && RendPoint.z<64*14*fONE && (Counter&1)==0 ) {
					sinSetEffect( SIN_PARTICLE_ROTATE_RISE ,  pX,pY ,pZ , 1000 , 0 , 1500+(smCharInfo.SizeLevel*500) );
				}
			}
			*/
		}

		if (smCharInfo.wPlayClass[1]) {
			if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HOBGOBLIN && smCharInfo.Level == 60) {
				//Ç³¼±°°Àº »êÅ¸ °íºí¸° (¸ÂÀ»¼ö·Ï Ä¿Áø´Ù)
				cnt = ((smCharInfo.Life[1] - smCharInfo.Life[0]) * 460) / smCharInfo.Life[1];
				cnt += 120;
				smCharInfo.wPlayClass[1] = cnt;
			}
		}

		// ¹ÚÀç¿ø - ¹Ì¸® ¸ð¸®ÇÁ ÀÌº¥Æ®(¸ð¸®ÇÁ¸¦ »ç³ÉÇÏ¸é ¸ð¸®ÇÁ°¡ Á¡Á¡ Ä¿Áö°í ¹Ì´Ï ¸ð¸®ÇÁ°¡ ³ª¿Â´Ù)
		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MORIF && smCharInfo.Level == 60) {
			//Ç³¼±°°ÀÌ Á¡Á¡ Ä¿Áö´Â ¸ð¸®ÇÁ (¸ÂÀ»¼ö·Ï Ä¿Áø´Ù)
			cnt = ((smCharInfo.Life[1] - smCharInfo.Life[0]) * 700) / smCharInfo.Life[1];
			cnt += 300;
			smCharInfo.wPlayClass[1] = cnt;
		}

		if (smCharInfo.Life[0] != DispLifeBar) {
			if (DispLifeBar < smCharInfo.Life[0]) DispLifeBar++;
			else DispLifeBar--;
		}

		if (MotionInfo->State == CHRMOTION_STATE_WARP) {
			if (FrameCounter > 10 && smCharInfo.dwCharSoundCode == snCHAR_SOUND_WOLVERLIN) {
				if (sAddColorEffect[SMC_A] > -256) sAddColorEffect[SMC_A] -= 8;
				if (sAddColorEffect[SMC_R] > 0) sAddColorEffect[SMC_R] -= 8;
				if (sAddColorEffect[SMC_G] > 0) sAddColorEffect[SMC_G] -= 8;
				if (sAddColorEffect[SMC_B] > 0) sAddColorEffect[SMC_B] -= 8;
				DisplayAlpha = -16;
			}
		}

		//¹«±â ¼Ó¼º ÀÌÆåÆ®
		if (dwWeaponEffectTime) {
			if (dwWeaponEffectTime < dwPlayTime) {
				dwWeaponEffectTime = 0;
				WeaponEffect = 0;
			}
		}

		if (PlayVanish > 0) PlayVanish--;
		if (PlayInvincible > 0)	PlayInvincible--;		//¹«Àû ¾ÆÀÌÅÛ »ç¿ëÁß

		//Å¬·£ Á¤º¸ ¾÷µ¥ÀÌÆ® È®ÀÎ
		if (smCharInfo.ClassClan && ClanInfoNum < 0 && dwClanInfoTime && (dwClanInfoTime + 60 * 1000) < dwPlayTime) {
			//Å¬·£¿øÀÎµ¥ Å¬·£Á¤º¸°¡ ¾÷µ¥ÀÌÆ® µÇÁö ¾ÊÀº°æ¿ì 1 ºÐ ¸¶´Ù Àç ½Ãµµ
			ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
			dwClanInfoTime = dwPlayTime;
		}
		break;

	case 101:

		frame += 80;
		compFrame = frame / 160;

		if (compFrame >= MotionInfo->EndFrame) {
			SetMotionFromCode(CHRMOTION_STATE_STAND);

		}

		if (TalkFrame)
		{
			if (TalkFrame >= lpTalkMotionInfo->EndFrame * 160)
				TalkFrame = 0;
			else
				TalkFrame += 80;
		}
		break;

	}

	if (ShootingFlag) {
		ShootingMain();
	}

	if (DisplayAlpha < 0) {
		DisplayAlpha += 4;
		if (DisplayAlpha > 0) DisplayAlpha = 0;
	}
	if (DisplayAlpha > 0) {
		DisplayAlpha -= 4;
		if (DisplayAlpha < 0) DisplayAlpha = 0;
	}


	Counter++;
	FrameCounter++;


	return TRUE;
}

#define VIEW_HIGH_DIST	36864	
#define VIEW_MID_DIST	102400		


int bb_r, bb_g, bb_b, bb_a;

int SetRenderBlinkColor(smCHARTOOL* smCharTool)
{
	int BlinkMsk;
	int BlinkTime;
	int bTime;
	int Blink, r, g, b, a;

	bb_r = smRender.Color_R;
	bb_g = smRender.Color_G;
	bb_b = smRender.Color_B;
	bb_a = smRender.Color_A;

	if (smCharTool->ColorBlink == 0) return NULL;

	Blink = smCharTool->ColorBlink;
	r = smCharTool->sColors[SMC_R];
	g = smCharTool->sColors[SMC_G];
	b = smCharTool->sColors[SMC_B];
	a = smCharTool->sColors[SMC_A];

	if (Blink < 0) {
		smRender.Color_R += r;
		smRender.Color_G += g;
		smRender.Color_B += b;
		smRender.Color_A += a;

		return NULL;
	}

	BlinkTime = (1 << Blink);
	BlinkMsk = BlinkTime - 1;

	bTime = dwPlayTime & BlinkMsk;

	if (dwPlayTime & BlinkTime) {
		smRender.Color_R += (bTime * r) >> Blink;
		smRender.Color_G += (bTime * g) >> Blink;
		smRender.Color_B += (bTime * b) >> Blink;
		if (a < 0) smRender.Color_A += (bTime * a) >> Blink;
	}
	else {
		bTime = BlinkMsk - bTime;
		smRender.Color_R += (bTime * r) >> Blink;
		smRender.Color_G += (bTime * g) >> Blink;
		smRender.Color_B += (bTime * b) >> Blink;
		if (a < 0) smRender.Color_A += (bTime * a) >> Blink;
	}

	if (a > 0)
		smRender.Color_A -= a;

	bTime = (bTime * fONE) >> Blink;
	if (!bTime) bTime = 1;

	if (smCharTool->TexMixCode >= 0) {
		SetItem2PassTexture(smCharTool->TexMixCode, smCharTool->TexScroll);
	}

	return  bTime;
}

int RestoreRenderBlinkColor()
{
	smRender.Color_R = bb_r;
	smRender.Color_G = bb_g;
	smRender.Color_B = bb_b;
	smRender.Color_A = bb_a;

	SetItem2PassTexture(-1);

	return TRUE;
}

int smCHAR::VirtualDraw()
{

	POINT	sPos;
	POINT3D CameraPosi, CameraAngle;
	int		He;

	//ÆÐÅÏ À¯¹« Ã¼Å© ( Åë½Å½Ã ÆÄÀÏ ¸ðµ¨¼ö½ÅÀÌ ¾ÈµÈ°æ¿ì )
	if (!DisplayFlag || !MotionInfo) return FALSE;

	//¼ö½Å½Ã°£ È®ÀÎÇÏ¿© 3ÃÊ ÃÊ°ú Ä³¸¯ÅÍ È­¸é ºñÇ¥½Ã
	if (ActionPattern == 99) {
		if (LastPlayDataType) {
			//¿ø°Å¸® ±³½Å ( µ¥ÀÌÅ¸ÀÇ Á¤È®µµ°¡ ¶³¾îÁü )
			if ((dwLastTransTime + DISPLAY_TRANS_TIME_OVER_FAR) < dwPlayTime) {
				dwDispAppSkill = 0;
				return TRUE;
			}
		}
		else {
			//±Ù°Å¸® ±³½Å
			if ((dwLastTransTime + DISPLAY_TRANS_TIME_OVER) < dwPlayTime) {
				dwDispAppSkill = 0;
				return TRUE;
			}
		}
	}
	CameraPosi.x = smCHAR_CameraX;
	CameraPosi.y = smCHAR_CameraY;
	CameraPosi.z = smCHAR_CameraZ;

	CameraAngle.x = smCHAR_CameraAngX;
	CameraAngle.y = smCHAR_CameraAngY;
	CameraAngle.z = smCHAR_CameraAngZ;

	He = PatHeight;
	if (He < 44 * fONE) He = 44 * fONE;

	smRender.SetCameraPosi(CameraPosi.x, CameraPosi.y, CameraPosi.z, CameraAngle.x, CameraAngle.y, CameraAngle.z);
	RendPoint.z = smRender.GetRect2D(pX, pY + (He >> 1), pZ, 44 * fONE, He, &RendRect2D, &sPos);
	RendPoint.x = sPos.x;
	RendPoint.y = sPos.y;

	if (RendPoint.z > 0 && sPos.x > 0 && sPos.x < WinSizeX && sPos.y>0 && sPos.y < WinSizeY) {
		RendSucess = TRUE;
		return TRUE;
	}

	return FALSE;
}

int smCHAR::Draw(bool DrawCharacterTarget)
{

	POINT3D CameraPosi = { 0,0,0 };
	POINT3D CameraAngle = { 0,0,0 };
	POINT3D angle = { 0,0,0 };
	smPAT3D* pat;
	POINT	sPos;
	POINT3D	sPos3;
	int		He;

	int dx, dy, dz, dDist;
	_MODELGROUP* lpModelGroup;
	Graphics::ModelGroup* newModelGroup = nullptr;

	int	BlinkFlag = 0;
	smMATRIX	Mat;
	int		BlinkLevel;
	int		BlinkScale;


	RendSucess = 0;
	FlagShow = 0;

	if (!DisplayFlag || !MotionInfo || lpDinaPattern == nullptr) return FALSE;

	if (lpDinaPattern->UseCount <= 0) {
		Close();
		if (smConfig.DebugMode) CHATGAMEHANDLE->AddChatBoxText("Load Patern Draw", EChatColor::CHATCOLOR_Error);
		return FALSE;
	}

	if (ActionPattern == 99) {
		if (LastPlayDataType) {
			if ((dwLastTransTime + DISPLAY_TRANS_TIME_OVER_FAR) < dwPlayTime) {
				dwDispAppSkill = 0;
				return TRUE;
			}
		}
		else {
			if ((dwLastTransTime + DISPLAY_TRANS_TIME_OVER) < dwPlayTime) {
				dwDispAppSkill = 0;
				return TRUE;
			}
		}
	}

	FlagShow = TRUE;

	dx = (smCHAR_CameraX - pX) >> FLOATNS;
	dy = (smCHAR_CameraY - pY) >> FLOATNS;
	dz = (smCHAR_CameraZ - pZ) >> FLOATNS;

	dDist = dx * dx + dy * dy + dz * dz;

	CameraPosi.x = smCHAR_CameraX;
	CameraPosi.y = smCHAR_CameraY;
	CameraPosi.z = smCHAR_CameraZ;

	CameraAngle.x = smCHAR_CameraAngX;
	CameraAngle.y = smCHAR_CameraAngY;
	CameraAngle.z = smCHAR_CameraAngZ;

	int ObjSizeBack = smCharInfo.wPlayClass[1];

	if (DrawCharacterTarget)
	{
		memset(&angle, 0, sizeof(POINT3D));
		smCharInfo.wPlayClass[1] = 0;
	}
	else
		memcpy(&angle, &Angle, sizeof(POINT3D));

	angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

	if (ActionGameMode) {
		if (angle.y > ANGLE_180)
			angle.y = ANGLE_270;
		else
			angle.y = ANGLE_90;
	}

	Posi.x = DrawCharacterTarget ? 0 : pX;
	Posi.y = DrawCharacterTarget ? 0 : pY;
	Posi.z = DrawCharacterTarget ? 0 : pZ;

	Pattern->SetPosi(&Posi, &angle);

	if (Pattern->TmParent && Pattern->TmParent->MaxFrame > (int)frame)
		Pattern->Frame = frame;
	else
		Pattern->Frame = 80;

	// montaria aqui
	if (pMount)
	{
		pMount->SetCurrentFrame(MotionInfo->State, frame);
		pMount->Render();
	}

	char* ModelNameList[4];

	int	cr, cg, cb, ca, cnt, sR, sG, sB, sA;
	int nl = 0;

	cr = smRender.Color_R;
	cg = smRender.Color_G;
	cb = smRender.Color_B;
	ca = smRender.Color_A;

	sR = sAddColorEffect[SMC_R];
	sG = sAddColorEffect[SMC_G];
	sB = sAddColorEffect[SMC_B];
	sA = sAddColorEffect[SMC_A];

	if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS) {
		cnt = Counter & 0x1F;
		if ((Counter & 0x20) == 0) {
			cnt = 0x1F - cnt;
		}
		sR = (sR * cnt) >> 5;
		sG = (sG * cnt) >> 5;
		sB = (sB * cnt) >> 5;
	}

	smRender.Color_R = cr + AttackAnger + sR;
	smRender.Color_G = cg - (AttackAnger >> 1) + sG + abs(DispPoison);
	smRender.Color_B = cb + abs(AttackIce) + sB;
	smRender.Color_A = ca + DisplayAlpha - (abs(AttackIce) >> 1) + sA;

	if (PlayCurseQuest) {
		//Effect color quest 90
		smRender.Color_R += 100;
		smRender.Color_B += 100;
	}

	// DESATIVADO: TOP LVL
	//extern BOOL TopLVLEffect;
	//extern BOOL OtherPlayerTopLVLEffect;
	// efeito top lvl srv xxstr
	/*if (PlayCurseTopLVL)
	{
		if (lpCurPlayer->PlayCurseTopLVL)
		{
			if (TopLVLEffect == FALSE)
			{
				//AssaParticle_TopLVL(lpCurPlayer);
				TopLVLEffect = FALSE; //TRUE;
			}
		}
		else// verifica se o outros player é top lvl seta efeito xxstr
		{
			for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
				if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && chrOtherPlayer[cnt].PlayCurseTopLVL) {
					if (OtherPlayerTopLVLEffect == FALSE)
					{
						AssaParticle_TopLVL(&chrOtherPlayer[cnt]);
						OtherPlayerTopLVLEffect = TRUE;
					}

				}
			}
		}
	}
	*/

	extern BOOL ReiPVPEffect;
	extern BOOL OtherPlayerReiPVPEffect;
	// efect rei pvp xxstr
	/*
	if (PlayCursePvP)
	{
		smRender.Color_R += 139;
		smRender.Color_G += 0;
		smRender.Color_B += 0;

		// verifica se o player atual é reipvp e seta efeito xxstr
		if (lpCurPlayer->PlayCursePvP)
		{
			if (ReiPVPEffect == FALSE)
			{
				AssaParticle_KingPVP(lpCurPlayer);
				ReiPVPEffect = TRUE;
			}
		}
		else// verifica se o outros player é reipvp seta efeito xxstr
		{
			for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
				if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial && chrOtherPlayer[cnt].PlayCursePvP) {
					if (OtherPlayerReiPVPEffect == FALSE)
					{
						AssaParticle_KingPVP(&chrOtherPlayer[cnt]);
						OtherPlayerReiPVPEffect = TRUE;
					}

				}
			}
		}

	} */

	if (PlayEvasionMastery)
	{
		AssaParticle_EvasionPhoenix(this);
	}

	if (PlayDistortion && MotionInfo->State != CHRMOTION_STATE_DEAD)
	{
		Graphics::Graphics::GetInstance()->GetRenderer()->SetApplyDistortionFlag(true);
		smRender.DistortionMesh = TRUE;
		smRender.Color_R += 80;
		smRender.Color_G += 80;
	}

	bool IsPlayer = smCharInfo.State == 0x80 || PkMode_CharState == 0x80;

	if (DissolveAmount > 0.f && IsPlayer == false)
	{
		DissolveAmount += 0.001f;

		if (DissolveAmount > 1.f)
			DissolveAmount = 1.f;

		Graphics::Graphics::GetInstance()->GetRenderer()->SetDissolveAmount(DissolveAmount);
	}


	Math::Vector3 scaling;

	if (smCharInfo.wPlayClass[1]) {
		scaling = Math::Vector3(smCharInfo.wPlayClass[1] / 256.0f, smCharInfo.wPlayClass[1] / 256.0f, smCharInfo.wPlayClass[1] / 256.0f);
		smRender.ObjectVertexScale = smCharInfo.wPlayClass[1];
	}
	else
		scaling = Math::Vector3(1.0f, 1.0f, 1.0f);

	if ((Pattern) && Pattern->newModel)
	{
		if (Pattern->newModel->skeleton)
			Pattern->newModel->skeleton->scaling = scaling;
	}

	if ((Pattern2) && Pattern2->newModel)
	{
		if (Pattern2->newModel->skeleton)
			Pattern2->newModel->skeleton->scaling = scaling;
	}

	if (DarkLevel > 0 && OnStageField >= 0 && StageField[OnStageField]->State == FIELD_STATE_VILLAGE) {
		//¸¶À»¿¡¼­´Â µ¿ÀûÁ¶¸í Àû¿ë ¾ÈµÇµµ·Ï °­Á¦ ¼³Á¤
		nl = DarkLevel >> 1;
		smRender.Color_R += nl;
		smRender.Color_G += nl;
		smRender.Color_B += nl;

		if (smRender.nRendLight > 0 && smRender.smRendLight[0].Range == DarkLightRange) {
			smRender.smRendLight[0].Range = 0;
			nl = 1;
		}
		else
			nl = 0;
	}


	if (MotionInfo->State == CHRMOTION_STATE_DEAD) {
		if (FrameCounter > 400) {
			cnt = 100;
			smRender.Color_R -= cnt;
			smRender.Color_G -= cnt;
			smRender.Color_B -= cnt;
		}
		else {
			if (FrameCounter > 200) {
				cnt = (FrameCounter - 200) >> 1;
				smRender.Color_R -= cnt;
				smRender.Color_G -= cnt;
				smRender.Color_B -= cnt;

			}
		}

		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_FIGON || smCharInfo.dwCharSoundCode == snCHAR_SOUND_STYGIANLORD ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_GUARDIAN_SAINT || smCharInfo.dwCharSoundCode == snCHAR_SOUND_S_F_ELEMENTAL ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CHAOSCARA) {

			//ÆÄÀÌ°ï Á×À»¶§ ¼­¼­È÷ »ç¶óÁü
			smRender.Color_A = ca - (FrameCounter << 1);
			if (smRender.Color_A < -400) goto rendSkip;
			DisplayAlpha = 100;
		}
	}


	//°Å¸®º° ¸Þ½¬ Á¤¹Ðµµ º¯°æÇØ °¡¸ç ·»´õ¸µ
	lpModelGroup = &smMotionInfo->DefaultModel;

	if (dDist < VIEW_HIGH_DIST && smMotionInfo->HighModel.ModelNameCnt)
		lpModelGroup = &smMotionInfo->HighModel;

	if (dDist > VIEW_MID_DIST && smMotionInfo->LowModel.ModelNameCnt)
		lpModelGroup = &smMotionInfo->LowModel;
	/*
		//ÇÁ·¹ÀÓ ¹üÀ§ ¹þ¾î³ª´ÂÁö È®ÀÎÇÏ±â ( µð¹ö±×¿ë )
		if ( frame<(MotionInfo->StartFrame*160) || frame>(MotionInfo->EndFrame*160) ) {
			cnt = cnt;
		}
	*/
	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	if (TalkFrame) // && (PatDispMode & DISP_MODE_PATTALK) )
	{
		if (TalkPattern->MaxFrame > (int)TalkFrame)
			TalkPattern->Frame = TalkFrame;
		else
			TalkFrame = 0;	// ÇÁ·¹ÀÓ ¿À·ù
	}
	//######################################################################################

	if (PatDispMode & DISP_MODE_PATMAIN) {

		//New Frame Info
		IO::SMD::FrameInfo sNewFrameInfo{ FrameInfo.StartFrame, FrameInfo.EndFrame };

		if (Pattern->TmParent) Pattern->TmParent->TmLastFrame = -1;
		else Pattern->TmLastFrame = -1;

		//Use the new render
		if (Pattern->newModel)
		{
			Pattern->newModel->SetAddColor(Math::Color(smRender.Color_R, smRender.Color_G, smRender.Color_B, smRender.Color_A));
			Pattern->newModel->SetDiffuseColor(Math::Color(255, 255, 255, 255 + smRender.Color_A));

			newModelGroup = (Graphics::ModelGroup*)&lpDinaPattern->sModelsGroup->sDefaultModel;

			if (!lpDinaPattern->sModelsGroup->sHighModel.meshes.empty())
				newModelGroup = (Graphics::ModelGroup*)&lpDinaPattern->sModelsGroup->sHighModel;

			if (!newModelGroup->meshes.empty())
			{
				RendSucess += Pattern->newModel->Render(&sNewFrameInfo, newModelGroup);
			}
			else
			{
				RendSucess += Pattern->newModel->Render(&sNewFrameInfo, nullptr);
			}
		}
		else
		{
			if (lpModelGroup->ModelNameCnt)
			{
				ModelNameList[0] = lpModelGroup->szModelName[0];
				ModelNameList[1] = lpModelGroup->szModelName[1];
				ModelNameList[2] = lpModelGroup->szModelName[2];
				ModelNameList[3] = lpModelGroup->szModelName[3];
				//######################################################################################
				//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
				if (TalkFrame && lpTalkMotionInfo->State)
					RendSucess += Pattern->ChoiseRenderD3D(&CameraPosi, &CameraAngle, ModelNameList, lpModelGroup->ModelNameCnt, TalkPattern, &FrameInfo);
				else
					RendSucess += Pattern->ChoiseRenderD3D(&CameraPosi, &CameraAngle, ModelNameList, lpModelGroup->ModelNameCnt, &FrameInfo);
				//######################################################################################			
			}
			//######################################################################################
			//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
			else
			{
				if (TalkFrame && lpTalkMotionInfo->State)
					RendSucess += Pattern->RenderD3D(&CameraPosi, &CameraAngle, TalkPattern, &FrameInfo);
				else
					RendSucess += Pattern->RenderD3D(&CameraPosi, &CameraAngle, &FrameInfo);
			}
		}
	}

	//¸Ó¸®¹× º¸Á¶ ÆÐÅÏ Ãâ·Â
	if (lpDinaPattern2 && Pattern2 && PatDispMode & DISP_MODE_PATSUB) {

		//New Frame Info
		IO::SMD::FrameInfo sNewFrameInfo{ FrameInfo.StartFrame, FrameInfo.EndFrame };

		lpModelGroup = &lpDinaPattern2->lpModelInfo->DefaultModel;

		if (dDist < VIEW_HIGH_DIST && lpDinaPattern2->lpModelInfo->HighModel.ModelNameCnt)
			lpModelGroup = &lpDinaPattern2->lpModelInfo->HighModel;

		if (dDist > VIEW_MID_DIST && lpDinaPattern2->lpModelInfo->LowModel.ModelNameCnt)
			lpModelGroup = &lpDinaPattern2->lpModelInfo->LowModel;


		Pattern2->SetPosi(&Posi, &angle);
		if (Pattern2->TmParent && Pattern2->TmParent->MaxFrame > (int)frame)
			Pattern2->Frame = frame;

		//ÇÁ·¹ÀÓ Ä³½Ã ÃÊ±âÈ­ ( ¾È±×·¯¸é Ä³¸¯ÅÍ Á×À»¶§ Âî±×·¯Áü )
		if (Pattern2->TmParent)
		{
			if (IsPlayer)
			{
			}
			else
				Pattern2->TmParent->TmLastFrame = -1;
		}
		else Pattern2->TmLastFrame = -1;

		//Use the new render
		if (Pattern2->newModel)
		{
			Pattern2->newModel->SetDiffuseColor(Math::Color(255, 255, 255, 255 + smRender.Color_A));
			Pattern2->newModel->SetAddColor(Math::Color(smRender.Color_R, smRender.Color_G, smRender.Color_B, smRender.Color_A));

			newModelGroup = (Graphics::ModelGroup*)&lpDinaPattern2->sModelsGroup->sDefaultModel;

			if (!lpDinaPattern2->sModelsGroup->sHighModel.meshes.empty())
				newModelGroup = (Graphics::ModelGroup*)&lpDinaPattern2->sModelsGroup->sHighModel;

			if (!newModelGroup->meshes.empty())
			{
				RendSucess += Pattern2->newModel->Render(&sNewFrameInfo, newModelGroup);
			}
			else
			{
				RendSucess += Pattern2->newModel->Render(&sNewFrameInfo);
			}
		}
		else
		{
			if (lpModelGroup->ModelNameCnt)
			{
				ModelNameList[0] = lpModelGroup->szModelName[0];
				ModelNameList[1] = lpModelGroup->szModelName[1];
				ModelNameList[2] = lpModelGroup->szModelName[2];
				ModelNameList[3] = lpModelGroup->szModelName[3];
				//######################################################################################
				//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
				if (TalkFrame && lpTalkMotionInfo->State)
					RendSucess += Pattern2->ChoiseRenderD3D(&CameraPosi, &CameraAngle, ModelNameList, lpModelGroup->ModelNameCnt, TalkPattern, &FrameInfo);
				else
					RendSucess += Pattern2->ChoiseRenderD3D(&CameraPosi, &CameraAngle, ModelNameList, lpModelGroup->ModelNameCnt, &FrameInfo);
				//######################################################################################			
			}
			//######################################################################################
			//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
			else
			{
				if (TalkFrame && lpTalkMotionInfo->State)
					RendSucess += Pattern2->RenderD3D(&CameraPosi, &CameraAngle, TalkPattern, &FrameInfo);
				else
					RendSucess += Pattern2->RenderD3D(&CameraPosi, &CameraAngle, &FrameInfo);
			}
		}
		//######################################################################################
	}

	smRender.ObjectVertexScale = 0;

	if (RendSucess) {

		int We = 44 * fONE;
		He = PatHeight;

		if (smCharInfo.wPlayClass[1]) {
			//Å©±â Å°¿ì±â
			He = (He * smCharInfo.wPlayClass[1]) >> FLOATNS;
		}

		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU) {
			We = He * 2;
		}

		if (He < 44 * fONE) He = 44 * fONE;

		smRender.SetCameraPosi(CameraPosi.x, CameraPosi.y, CameraPosi.z, CameraAngle.x, CameraAngle.y, CameraAngle.z);
		//RendPoint.z = smRender.GetRect2D( pX , pY+(24*fONE), pZ , 32*fONE , 40*fONE , &RendRect2D , &sPos );
		RendPoint.z = smRender.GetRect2D(pX, pY + (He >> 1), pZ, We, He, &RendRect2D, &sPos);
		RendPoint.x = sPos.x;
		RendPoint.y = sPos.y;
	}


	if (!Pattern->TmParent || !DisplayTools) {
		smRender.Color_R = cr;
		smRender.Color_G = cg;
		smRender.Color_B = cb;
		if (nl) smRender.smRendLight[0].Range = DarkLightRange;
		return TRUE;
	}


	//¹«±â¹× ºÎÂø ¹°Ç° Ãâ·Â

//  if ( !HideWeapon ) {

	if (MotionInfo->State == CHRMOTION_STATE_ATTACK && dwPlayTime & 0x20)
		BlinkFlag = TRUE;

	if (BlinkFlag && !ShootingMode) {
		smRender.Color_R += 80;
		smRender.Color_G += 80;
		smRender.Color_B += 80;
	}

	Rend_HvLeftHand = TRUE;			//¹«±â ·»´õ¸µ À¯¹«
	Rend_HvRightHand = TRUE;		//¹«±â ·»´õ¸µ À¯¹«

	if (smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) {

		if (dwItemSetting == 0 || (OnStageField >= 0 && StageField[OnStageField]->State != FIELD_STATE_VILLAGE && MotionInfo->State != CHRMOTION_STATE_YAHOO)) {
			if (HvLeftHand.PatTool) { //&& !HideWeapon ) {
				pat = HvLeftHand.PatTool;

				if ((HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1) {
					if ((dwDispAppSkill & SKILL_APPMASK_SPARK_SHIELD) && PatSparkShield && smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN) {
						if (MotionInfo->State == CHRMOTION_STATE_SKILL && (AttackSkil & 0xFF) == SKILL_PLAY_SPARK_SHIELD && MotionEvent == 0)
							pat = HvLeftHand.PatTool;
						else
							pat = PatSparkShield;
					}
					if ((dwDispAppSkill & SKILL_APPMASK_SPARK_SHIELD) && PatDivineShield && smCharInfo.JOB_CODE == JOBCODE_KNIGHT) {
						if (MotionInfo->State == CHRMOTION_STATE_SKILL && (AttackSkil & 0xFF) == SKILL_PLAY_DIVINE_INHALATION && MotionEvent == 0)
							pat = HvLeftHand.PatTool;
						else
							pat = PatDivineShield;
					}
					if ((dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) && PatGodlyShield && smCharInfo.JOB_CODE == JOBCODE_KNIGHT) {
						if (MotionInfo->State == CHRMOTION_STATE_SKILL && (AttackSkil & 0xFF) == SKILL_PLAY_GODLY_SHIELD && MotionEvent == 0)
							pat = HvLeftHand.PatTool;
						else
							pat = PatGodlyShield;
					}
				}


				if (MotionInfo->State == CHRMOTION_STATE_SKILL &&
					((AttackSkil & 0xFF) == SKILL_PLAY_SHIELD_STRIKE || (AttackSkil & 0xFF) == SKILL_PLAY_SOUL_SUCKER) &&
					ShieldObjBip) {

					pat->LinkParentObject(AnimPattern, ShieldObjBip);

					cr = smRender.Color_R;
					cg = smRender.Color_G;
					cb = smRender.Color_B;

					smRender.Color_R += 100;
					smRender.Color_G += 100;
					smRender.Color_B += 80;

				}
				else
					pat->LinkParentObject(AnimPattern, HvLeftHand.ObjBip);

				pat->SetPosi(&Posi, &angle);
				pat->Frame = frame;

				if (!DrawCharacterTarget)
					memcpy(&HvLeftHand_Matrix, &HvLeftHand.ObjBip->mWorld, sizeof(smMATRIX));

				if (!HvLeftHand.ColorBlink || !HvLeftHand.sColors[SMC_A]) {
					//Á¡¸ê»ö»ó ¼³Á¤
					BlinkLevel = SetRenderBlinkColor(&HvLeftHand);

					//½ºÄÉÀÏ Á¡¸ê
					if (HvLeftHand.BlinkScale && BlinkLevel) {
						smIdentityMatrix(Mat);
						if (HvLeftHand.BlinkScale < 0)
							BlinkScale = -HvLeftHand.BlinkScale;
						else
							BlinkScale = (BlinkLevel * HvLeftHand.BlinkScale) >> FLOATNS;
						Mat._11 = BlinkScale;
						Mat._22 = BlinkScale;
						Mat._33 = BlinkScale;
						pat->Frame = -1;
						memcpy(&pat->obj3d[0]->mWorld, &HvLeftHand.ObjBip->mWorld, sizeof(smMATRIX));
						smMatrixMult(pat->obj3d[0]->mWorld, Mat, pat->obj3d[0]->mWorld);
					}

					if ((pat) && pat->newModel)
						if (pat->newModel->GetMaterialCollection())
							pat->newModel->GetMaterialCollection()->SetBlendingMaterial(HvLeftHand.blendingMaterial);

					pat->RenderD3D(&CameraPosi, &CameraAngle);

					//½¯µå ½ºÆ®¶óÀÌÅ© ¹æÆÐ¿¡ ºÙÀÌ´Â ÀÌÆåÆ®
					if (MotionInfo->State == CHRMOTION_STATE_SKILL && (AttackSkil & 0xFF) == SKILL_PLAY_SHIELD_STRIKE && ShieldObjBip)
					{
						smRender.Color_R = cr;
						smRender.Color_G = cg;
						smRender.Color_B = cb;

						sPos3.x = (pat->newModel->meshes[0]->world._41 * 256.f) + pX;
						sPos3.z = (pat->newModel->meshes[0]->world._42 * 256.f) + pZ;
						sPos3.y = (pat->newModel->meshes[0]->world._43 * 256.f) + pY;
						sinEffect_Shield_Strike(&sPos3);
					}
				}
				else
					Rend_HvLeftHand = FALSE;

				pat->LinkParentObject(0, 0);
				pat->TmLastFrame = -1;
			}

			//HideSkillWeapon
			//&& AttackSkil != SKILL_PLAY_MECHANIC_BOM 
			if (HvRightHand.PatTool && !HideWeapon) {
				if ((dwActionItemCode & sinITEM_MASK2) == sinWT1 && ShootingFlag) {

				}
				else {
					//if ( MotionInfo->State!=CHRMOTION_STATE_EAT ) {	//¹°¾à È­¸é ±úÁü ¶«¹æ
					pat = HvRightHand.PatTool;
					pat->LinkParentObject(AnimPattern, HvRightHand.ObjBip);
					pat->SetPosi(&Posi, &angle);
					pat->Frame = frame;

					if (!DrawCharacterTarget)
						memcpy(&HvRightHand_Matrix, &HvRightHand.ObjBip->mWorld, sizeof(smMATRIX));

					if (!HvRightHand.ColorBlink || !HvRightHand.sColors[SMC_A]) {
						//Á¡¸ê»ö»ó ¼³Á¤
						BlinkLevel = SetRenderBlinkColor(&HvRightHand);

						//½ºÄÉÀÏ Á¡¸ê
						if (HvRightHand.BlinkScale && BlinkLevel) {
							smIdentityMatrix(Mat);
							if (HvRightHand.BlinkScale < 0)
								BlinkScale = -HvRightHand.BlinkScale;
							else
								BlinkScale = (BlinkLevel * HvRightHand.BlinkScale) >> FLOATNS;

							Mat._11 = BlinkScale;
							Mat._22 = BlinkScale;
							Mat._33 = BlinkScale;
							pat->Frame = -1;
							memcpy(&pat->obj3d[0]->mWorld, &HvRightHand.ObjBip->mWorld, sizeof(smMATRIX));
							smMatrixMult(pat->obj3d[0]->mWorld, Mat, pat->obj3d[0]->mWorld);
						}

						if ((pat) && pat->newModel)
							if (pat->newModel->GetMaterialCollection())
								pat->newModel->GetMaterialCollection()->SetBlendingMaterial(HvRightHand.blendingMaterial);

						pat->RenderD3D(&CameraPosi, &CameraAngle);
						RestoreRenderBlinkColor();		//»ö»óº¹±¸
					}
					else
						Rend_HvRightHand = FALSE;

					pat->LinkParentObject(0, 0);
					pat->TmLastFrame = -1;
					//}
				}
			}
		}
		else {
			if (dwActionItemCode) {

				BlinkScale = 0;

				//µî¿¡ ÀåÂø
				if (HvLeftHand.dwItemCode == dwActionItemCode) {
					pat = HvLeftHand.PatTool;
					if (!HvLeftHand.ColorBlink || !HvLeftHand.sColors[SMC_A]) {
					}
					else
						Rend_HvLeftHand = FALSE;
				}
				if (HvRightHand.dwItemCode == dwActionItemCode) {
					pat = HvRightHand.PatTool;
					if (!HvRightHand.ColorBlink || !HvRightHand.sColors[SMC_A]) {
					}
					else
						Rend_HvRightHand = FALSE;
				}

				pat->LinkParentObject(AnimPattern, BackObjBip[dwItemSetting - 1]);
				pat->SetPosi(&Posi, &angle);
				pat->Frame = frame;

				//Çà·Ä º¹»ç
				if (HvLeftHand.dwItemCode == dwActionItemCode) {
					if (!DrawCharacterTarget)
						memcpy(&HvLeftHand_Matrix, &BackObjBip[dwItemSetting - 1]->mWorld, sizeof(smMATRIX));

					if (Rend_HvLeftHand) {

						if ((pat) && pat->newModel)
							if (pat->newModel->GetMaterialCollection())
								pat->newModel->GetMaterialCollection()->SetBlendingMaterial(HvLeftHand.blendingMaterial);

						if (pat->newModel)
							pat->newModel->Render();

						RestoreRenderBlinkColor();
					}
				}
				else {
					if (!DrawCharacterTarget)
						memcpy(&HvRightHand_Matrix, &BackObjBip[dwItemSetting - 1]->mWorld, sizeof(smMATRIX));

					if (Rend_HvRightHand) {
						if ((pat) && pat->newModel)
							if (pat->newModel->GetMaterialCollection())
								pat->newModel->GetMaterialCollection()->SetBlendingMaterial(HvRightHand.blendingMaterial);

						if (pat->newModel)
							pat->newModel->Render();
						RestoreRenderBlinkColor();
					}
				}


				pat->LinkParentObject(0, 0);
				pat->TmLastFrame = -1;
			}

			if (HvLeftHand.PatTool && dwActionItemCode != HvLeftHand.dwItemCode) {
				pat = HvLeftHand.PatTool;
				pat->LinkParentObject(AnimPattern, HvLeftHand.ObjBip);
				pat->SetPosi(&Posi, &angle);
				pat->Frame = frame;

				if (!DrawCharacterTarget)
					memcpy(&HvLeftHand_Matrix, &HvLeftHand.ObjBip->mWorld, sizeof(smMATRIX));

				if (!HvLeftHand.ColorBlink || !HvLeftHand.sColors[SMC_A]) {
					//Á¡¸ê»ö»ó ¼³Á¤
					BlinkLevel = SetRenderBlinkColor(&HvLeftHand);

					//½ºÄÉÀÏ Á¡¸ê
					if (HvLeftHand.BlinkScale && BlinkLevel) {
						smIdentityMatrix(Mat);
						if (HvLeftHand.BlinkScale < 0)
							BlinkScale = -HvLeftHand.BlinkScale;
						else
							BlinkScale = (BlinkLevel * HvLeftHand.BlinkScale) >> FLOATNS;

						Mat._11 = BlinkScale;
						Mat._22 = BlinkScale;
						Mat._33 = BlinkScale;
						pat->Frame = -1;
						memcpy(&pat->obj3d[0]->mWorld, &HvLeftHand.ObjBip->mWorld, sizeof(smMATRIX));
						smMatrixMult(pat->obj3d[0]->mWorld, Mat, pat->obj3d[0]->mWorld);
					}

					if ((pat) && pat->newModel)
						if (pat->newModel->GetMaterialCollection())
							pat->newModel->GetMaterialCollection()->SetBlendingMaterial(HvLeftHand.blendingMaterial);

					pat->RenderD3D(&CameraPosi, &CameraAngle);
					RestoreRenderBlinkColor();		//»ö»óº¹±¸
				}
				else
					Rend_HvLeftHand = FALSE;

				pat->LinkParentObject(0, 0);
				pat->TmLastFrame = -1;
			}

		}

		//È­»ì ½ÃÀ§ ´ç±â±â
		if ((MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL)
			&& (dwActionItemCode & sinITEM_MASK2) == sinWS1
			&& !HvRightHand.PatTool && !ShootingFlag && !HideWeapon) {
			pat = PatArrow;
			pat->LinkParentObject(AnimPattern, HvRightHand.ObjBip);
			pat->SetPosi(&Posi, &angle);
			pat->Frame = frame;
			pat->RenderD3D(&CameraPosi, &CameraAngle);
			pat->LinkParentObject(0, 0);
			pat->TmLastFrame = -1;
		}
	}


	if (ShootingFlag)
	{
		memcpy(&angle, &ShootingAngle, sizeof(POINT3D));
		angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

		if ((dwActionItemCode & sinITEM_MASK2) == sinWS1) {
			pat = PatArrow;
		}
		else {
			pat = HvRightHand.PatTool;
			angle.x = (angle.x - ANGLE_90) & ANGCLIP;
		}

		if (!pat) goto rendSkip;

		pat->SetPosi(&ShootingPosi, &angle);
		pat->Frame = 0;
		pat->RenderD3D(&CameraPosi, &CameraAngle);

	}

rendSkip:

	smCharInfo.wPlayClass[1] = ObjSizeBack;

	Graphics::Graphics::GetInstance()->GetRenderer()->SetDissolveAmount(0.f);
	Graphics::Graphics::GetInstance()->GetRenderer()->SetApplyDistortionFlag(false);

	smRender.Color_R = cr;
	smRender.Color_G = cg;
	smRender.Color_B = cb;
	smRender.Color_A = ca;
	if (nl) smRender.smRendLight[0].Range = DarkLightRange;
	smRender.DistortionMesh = FALSE;

	return TRUE;
}

//±×¸²ÀÚ ±×¸®±â
int smCHAR::DrawShadow()
{
	extern int ConfigUseDynamicShadows;

	int a;
	POINT3D	pos;
	POINT3D angle;
	POINT3D CameraPosi, CameraAngle;

	if (PatSizeLevel < 0 || !MotionInfo) return FALSE;

	CameraPosi.x = smCHAR_CameraX;
	CameraPosi.y = smCHAR_CameraY;
	CameraPosi.z = smCHAR_CameraZ;

	CameraAngle.x = smCHAR_CameraAngX;
	CameraAngle.y = smCHAR_CameraAngY;
	CameraAngle.z = smCHAR_CameraAngZ;

	if (Pattern && PatShadow[PatSizeLevel] && MotionInfo->State != CHRMOTION_STATE_RESTART &&
		RendSucess && RendPoint.z < 12 * 64 * fONE && PlayVanish <= 0) {

		pos.x = pX;
		pos.y = PHeight + (1 * fONE);	//pY;
		pos.z = pZ;


		angle.x = Angle.x;
		angle.y = (-Angle.y + ANGLE_180) & ANGCLIP;
		angle.z = Angle.z;

		a = smRender.Color_A;

		if (MotionInfo->State == CHRMOTION_STATE_DEAD)
			smRender.Color_A = a - (FrameCounter << 1);			//Á×À»¶§ ±×¸²ÀÚ ¼­¼­È÷ »ç¶óÁü

		smRender.Color_A -= 160;

		if (smRender.Color_A > -240) {
			GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			smRender.IgnoreRender = ConfigUseDynamicShadows;
			smRender.SetMaterialGroup(smMaterialGroup);
			PatShadow[PatSizeLevel]->SetPosi(&pos, &angle);
			PatShadow[PatSizeLevel]->RenderD3D(&CameraPosi, &CameraAngle);
			smRender.IgnoreRender = false;
			GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}

		smRender.Color_A = a;

	}

	//Å×½ºÆ®
	smPAT3D* pat;
	smMATRIX Mat;
	int BlinkLevel, BlinkScale;



	if (RendSucess && !HideWeapon) {
		if (!Rend_HvRightHand) {
			if (HvRightHand.ColorBlink) {
				pat = HvRightHand.PatTool;
				pat->SetPosi(&Posi, &angle);
				pat->Frame = -1;
				//Á¡¸ê»ö»ó ¼³Á¤
				BlinkLevel = SetRenderBlinkColor(&HvRightHand);
				memcpy(&pat->obj3d[0]->mWorld, &HvRightHand_Matrix, sizeof(smMATRIX));
				//½ºÄÉÀÏ Á¡¸ê
				if (HvRightHand.BlinkScale && BlinkLevel) {
					smIdentityMatrix(Mat);

					if (HvRightHand.BlinkScale < 0)
						BlinkScale = -HvRightHand.BlinkScale;
					else
						BlinkScale = (BlinkLevel * HvRightHand.BlinkScale) >> FLOATNS;

					Mat._11 = BlinkScale;
					Mat._22 = BlinkScale;
					Mat._33 = BlinkScale;
					smMatrixMult(pat->obj3d[0]->mWorld, Mat, pat->obj3d[0]->mWorld);
				}

				pat->RenderD3D(&CameraPosi, &CameraAngle);
				RestoreRenderBlinkColor();		//»ö»óº¹±¸
			}
		}
		if (!Rend_HvLeftHand) {
			if (HvLeftHand.ColorBlink) {
				pat = HvLeftHand.PatTool;
				pat->SetPosi(&Posi, &angle);
				pat->Frame = -1;
				//Á¡¸ê»ö»ó ¼³Á¤
				BlinkLevel = SetRenderBlinkColor(&HvLeftHand);
				memcpy(&pat->obj3d[0]->mWorld, &HvLeftHand_Matrix, sizeof(smMATRIX));
				//½ºÄÉÀÏ Á¡¸ê
				if (HvLeftHand.BlinkScale && BlinkLevel) {
					smIdentityMatrix(Mat);

					if (HvLeftHand.BlinkScale < 0)
						BlinkScale = -HvLeftHand.BlinkScale;
					else
						BlinkScale = (BlinkLevel * HvLeftHand.BlinkScale) >> FLOATNS;

					Mat._11 = BlinkScale;
					Mat._22 = BlinkScale;
					Mat._33 = BlinkScale;
					smMatrixMult(pat->obj3d[0]->mWorld, Mat, pat->obj3d[0]->mWorld);
				}
				pat->RenderD3D(&CameraPosi, &CameraAngle);
				RestoreRenderBlinkColor();		//»ö»óº¹±¸
			}
		}
		/*
						BlinkLevel = SetRenderBlinkColor( &HvLeftHand );

						//½ºÄÉÀÏ Á¡¸ê
						if ( HvLeftHand.BlinkScale && BlinkLevel ) {
							smIdentityMatrix( Mat );
							BlinkScale = (BlinkLevel*HvLeftHand.BlinkScale)>>FLOATNS;
							Mat._11 = fONE+BlinkScale;
							Mat._22 = fONE+BlinkScale;
							Mat._33 = fONE+BlinkScale;
							pat->Frame =  -1;
							memcpy( &pat->obj3d[0]->mWorld , &HvLeftHand.ObjBip->mWorld , sizeof(smMATRIX) );
							smMatrixMult( pat->obj3d[0]->mWorld , Mat , pat->obj3d[0]->mWorld );
						}

		*/

	}

	int r, g, b, c, d;
	int cnt;

	//#define	SKILL_APPMASK_EXTREME_SHIELD		0x0001
	//	DWORD			dwDispAppSkill;			//½ºÅ³ Àû¿ëÇ¥½Ã

	//	for(cnt=0;cnt<10;cnt++) {
	//		if ( ContinueSkill[cnt].Flag && ContinueSkill[cnt].CODE==SKILL_EXTREME_SHIELD ) {

	pat = 0;

	if ((dwDispAppSkill & SKILL_APPMASK_EXTREME_SHIELD) &&
		(MotionInfo->State != CHRMOTION_STATE_SKILL || (AttackSkil & 0xFF) != SKILL_PLAY_EXTREME_SHIELD)) {
		pat = PatEffectShield;
	}
	/*
		if ( (dwDispAppSkill&SKILL_APPMASK_SPARK_SHIELD) &&
			( MotionInfo->State!=CHRMOTION_STATE_SKILL || (AttackSkil&0xFF)!=SKILL_PLAY_SPARK_SHIELD ) ) {
				pat = PatSparkShield;
			}
	*/
	if (pat) {
		//ÀÍ½ºÆ®¸² ½¯µå Ç¥½Ã
		if (RendSucess && HvLeftHand.PatTool &&
			(HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1) {

			memcpy(&angle, &Angle, sizeof(POINT3D));
			angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

			Posi.x = pX;
			Posi.y = pY;
			Posi.z = pZ;
			a = smRender.Color_A;
			r = smRender.Color_R;
			g = smRender.Color_G;
			b = smRender.Color_B;
			smRender.Color_B = +128;

			//PatSparkShield
			//pat = PatEffectShield;//HvLeftHand.PatTool;;//PatEffectShield;

			pat->SetPosi(&Posi, &angle);

			d = dwPlayTime >> 4;
			c = d & 0x3F;
			if ((d & 0x40)) {
				c = 0x40 - c;
			}

			c += (int)(fONE * 0.9f) + c;

			smIdentityMatrix(Mat);
			if (PatEffectShield == pat) {
				Mat._11 = c;
				Mat._22 = c;
				Mat._33 = c;
			}

			for (cnt = 0; cnt < pat->nObj3d; cnt++) {
				memcpy(&pat->obj3d[cnt]->mWorld, &HvLeftHand_Matrix, sizeof(smMATRIX));
				smMatrixMult(pat->obj3d[cnt]->mWorld, Mat, pat->obj3d[cnt]->mWorld);
			}

			smRender.Color_R = 0;
			smRender.Color_G = 0;
			smRender.Color_B = 0;
			smRender.Color_A = 0;

			pat->Frame = -1;
			pat->RenderD3D(&CameraPosi, &CameraAngle);

			smRender.Color_A = a;
			smRender.Color_R = r;
			smRender.Color_G = g;
			smRender.Color_B = b;
			/*
							HvLeftHand.sColors[SMC_R] = 0;
							HvLeftHand.sColors[SMC_G] = 0;
							HvLeftHand.sColors[SMC_B] = 0;
							HvLeftHand.sColors[SMC_A] = -160;
			*/
		}
	}


	int s, sz, maxSize;

	//ÀÍ½ºÆÒ¼Ç ½ºÅ³
	if ((AttackSkil & 0xFF) == SKILL_PLAY_EXPANSION && RendSucess && HvRightHand.PatTool && MotionInfo->State == CHRMOTION_STATE_ATTACK) {

		sz = Expansion_Size[(AttackSkil >> 8) - 1];

		memcpy(&angle, &Angle, sizeof(POINT3D));
		angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

		Posi.x = pX;
		Posi.y = pY;
		Posi.z = pZ;
		a = smRender.Color_A;
		r = smRender.Color_R;
		smRender.Color_A = -96 - (FrameCounter << 1);
		smRender.Color_R = +128;
		if (smRender.Color_A < -192) smRender.Color_A = -192;

		pat = HvRightHand.PatTool;
		pat->SetPosi(&Posi, &angle);
		pat->Frame = -1;

		smIdentityMatrix(Mat);
		//Mat._11 = (int)(fONE*1.05f);
		//Mat._22 = (int)(fONE*1.05f);
		//Mat._33 = (int)(fONE*1.05f);
		s = FrameCounter << 3;
		maxSize = (sz * fONE) / 100;
		if (s > maxSize) s = maxSize;
		s += (fONE / 4);

		Mat._11 = fONE + s;
		Mat._22 = fONE + s;
		Mat._33 = fONE + s;

		memcpy(&pat->obj3d[0]->mWorld, &HvRightHand_Matrix, sizeof(smMATRIX));
		smMatrixMult(pat->obj3d[0]->mWorld, Mat, pat->obj3d[0]->mWorld);
		pat->RenderD3D(&CameraPosi, &CameraAngle);

		smRender.Color_A = a;
		smRender.Color_R = r;
	}

	// Efeito REI PVP aqui
	if (PlayCursePvP == 1 && MotionInfo->State != CHRMOTION_STATE_DEAD)
	{
		EffectPVP(this);
	}

	if (PlayCurseTopLVL == 1 && MotionInfo->State != CHRMOTION_STATE_DEAD)
	{
		EffectTopLevel(this);
	}

	//ºí·¹½º Ä³½½ ¿Õ°ü
	if (Clan_CastleMasterFlag && smCharInfo.ClassClan && PatCrown && RendSucess && RendPoint.z < 12 * 64 * fONE && PlayVanish <= 0) {

		GetMoveLocation(0, 0, 0 * fONE, 0, Angle.y, 0);

		pos.x = pX + GeoResult_X;
		pos.y = pY + PatHeight + 10 * fONE;	//pY;
		pos.z = pZ + GeoResult_Z;

		cnt = (dwPlayTime >> 2) & 0xFF;
		if ((cnt & 0x80)) {
			pos.y += (cnt & 0x7F);
		}
		else {
			pos.y += 0x80 - (cnt & 0x7F);
		}

		angle.x = 0;
		angle.y = (dwPlayTime >> 2) & ANGCLIP;
		angle.z = 0;

		//a = smRender.Color_A;
		//smRender.Color_A -=160;

		GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		smRender.SetMaterialGroup(smMaterialGroup);
		PatCrown->SetPosi(&pos, &angle);
		PatCrown->RenderD3D(&CameraPosi, &CameraAngle);
		GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		//smRender.Color_A = a;
	}


	return TRUE;
}


//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
void smCHAR::SetIceFoot(int TypeNum, DWORD dwPlayTime, int x, int z)
{
	if (smGameStage[OnStageField] == NULL)
		return;
	int loop = 0;
	for (loop = 0; loop < ICE_FOOT_COUNT_MAX; loop++)
	{
		if (m_IceFootInfo[loop].Life == 0)
			break;
	}

	if (loop >= ICE_FOOT_COUNT_MAX)
		return;

	POINT3D FootPos;
	FootPos.x = pX + x;
	//FootPos.y = PHeight;
	FootPos.y = pY + (10 * fONE);
	FootPos.z = pZ + z;

	POINT3D FaceAngle;
	if (smGameStage[OnStageField]->CheckFaceIceFoot(&FootPos, &FaceAngle, 32 * fONE) == 0)
		return;

	m_IceFootInfo[loop].Life = 1;
	m_IceFootInfo[loop].dwPlayTime = dwPlayTime;
	m_IceFootInfo[loop].TypeNum = TypeNum;
	m_IceFootInfo[loop].FootPos.x = FootPos.x;
	m_IceFootInfo[loop].FootPos.y = FootPos.y;
	m_IceFootInfo[loop].FootPos.z = FootPos.z;
	m_IceFootInfo[loop].FootAngle.x = -(FaceAngle.x - ANGLE_180) & ANGCLIP;
	m_IceFootInfo[loop].FootAngle.y = (-Angle.y) & ANGCLIP;
	m_IceFootInfo[loop].FootAngle.z = (FaceAngle.z - ANGLE_180) & ANGCLIP;
}

//
int smCHAR::DrawIceFoot(void)
{
	if (MotionInfo == NULL || Pattern == NULL || MotionInfo->State == CHRMOTION_STATE_RESTART || RendSucess == 0)
		return FALSE;

	POINT3D CameraPosi, CameraAngle;

	CameraPosi.x = smCHAR_CameraX;
	CameraPosi.y = smCHAR_CameraY;
	CameraPosi.z = smCHAR_CameraZ;

	CameraAngle.x = smCHAR_CameraAngX;
	CameraAngle.y = smCHAR_CameraAngY;
	CameraAngle.z = smCHAR_CameraAngZ;

	int b_r, b_g, b_b, b_a;
	b_r = smRender.Color_R;
	b_g = smRender.Color_G;
	b_b = smRender.Color_B;
	b_a = smRender.Color_A;

	smRender.Color_R = 0;
	smRender.Color_G = 0;
	smRender.Color_B = 0;
	smRender.Color_A = 0;

	smRender.m_dwRenderType = smRENDER_TYPE_ICE;

	smRender.SetMaterialGroup(::smMaterialGroup);
	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	for (int loop = 0; loop < ICE_FOOT_COUNT_MAX; loop++)
	{
		if (m_IceFootInfo[loop].Life)
		{
			float Alpha = float(dwPlayTime - m_IceFootInfo[loop].dwPlayTime) * ICE_FOOT_ALPHA_STEP;
			if (Alpha >= 200)
				m_IceFootInfo[loop].Life = 0;
			else
			{
				smRender.Color_A = -int(Alpha);
				PatIceFoot[m_IceFootInfo[loop].TypeNum]->SetPosi(&m_IceFootInfo[loop].FootPos, &m_IceFootInfo[loop].FootAngle);
				PatIceFoot[m_IceFootInfo[loop].TypeNum]->RenderD3D(&CameraPosi, &CameraAngle);
			}
		}
	}
	GRAPHICDEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	smRender.Color_R = b_r;
	smRender.Color_G = b_g;
	smRender.Color_B = b_b;
	smRender.Color_A = b_a;

	smRender.m_dwRenderType = 0;
	return TRUE;
}


#define STATE_BAR_WIDTH		125
#define STATE_BAR_HEIGHT	16
#define STATE_BAR_SIZE		115

#define STATE_BAR_WIDTH2	100
#define STATE_BAR_HEIGHT2	16
#define STATE_BAR_SIZE2		92

BOOL isEnemy = FALSE;
extern BOOL showDetails;

int smCHAR::DrawStateBar(int x, int y)
{
	if (!isEnemy) return NULL;

	showDetails = FALSE;

	int Percent;
	int	Percent2;
	int	Percent3;
	HDC hdc = NULL;
	DWORD dwColor;

	if (!EnableStateBar && smCharInfo.State == smCHAR_STATE_ENEMY) {
		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL && TryAttackCount >= 10) {
			EnableStateBar = TRUE;
		}
		else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_DOOR ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_R ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_G ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_B ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_N ||
			// Hp do boss visível para todos
			smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS)
		{
			EnableStateBar = TRUE;
		}

		EnableStateBar = TRUE;
	}

	//if ( smCharInfo.Life[1]==0 || ( !DisplayDebug && !EnableStateBar ) ) return NULL;
	if (x < 0 || (x + STATE_BAR_WIDTH) >= smScreenWidth) return NULL;
	if (y < 0 || (y + STATE_BAR_HEIGHT) >= smScreenHeight) return NULL;

	//psSetDefaultRenderState();
	//GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	Percent = (smCharInfo.Life[0] * STATE_BAR_SIZE) / smCharInfo.Life[1];
	Percent2 = (smCharInfo.Life[0] * 511) / smCharInfo.Life[1];
	Percent2 = (Percent2 * Percent2) / 512;
	Percent3 = Percent2 & 0xFF;


	if (Percent2 < 256)
		dwColor = D3DCOLOR_RGBA(255, Percent3, 0, 255);
	else
		dwColor = D3DCOLOR_RGBA(255 - Percent3, 255, 0, 255);

	//Percent = (DispLifeBar*STATE_BAR_SIZE)/smCharInfo.Life[1];
/*
	if( Percent==STATE_BAR_SIZE ) {
		dsDrawTexImage(MatEnergyBox[1] , x , y , STATE_BAR_WIDTH , STATE_BAR_HEIGHT,
			128 , 16 , 0 , 0, STATE_BAR_WIDTH , 16, 255);
	}
	else {
		dsDrawTexImage(MatEnergyBox[1] , x , y , Percent+3 , STATE_BAR_HEIGHT,
			128 , 16 , 0 , 0,(float)Percent+3 , 16, 255);

		dsDrawTexImage(MatEnergyBox[0] , x+Percent+3 , y , 100-Percent+3 , STATE_BAR_HEIGHT,
			128 , 16 , (float)Percent+4 , 0, 101 , 16, 255);
	}
*/
	float fSize = 800.0f;

	int iHeightCalc = (WinSizeX / 4) * 3;
	if (iHeightCalc == WinSizeY)
		fSize = (float)WinSizeX / 800.0f;
	else
		fSize = (float)WinSizeX / 1000.0f;

	char szBuff[256];


	dsDrawTexImageFloat(MatEnergyBox[0], (float)x, (float)y, 301, 82, 128, 16, 0, 0, STATE_BAR_WIDTH, 15, 255);
	dsDrawTexImageFloat(MatEnergyBox[2], (float)x + 5, (float)y, 59, 59, 90, 90, 0, 0, 10, 15, 255);

	SetFontTextColor(RGB(255, 160, 120));
	wsprintf(szBuff, "Vida: %d", smCharInfo.Life[0]);

	if (smCharInfo.Life[0] > 0)
	{
		dsTextLineOut(hdc, ((float)x + 140) - strlen(chrEachPlayer.smCharInfo.szName), (float)y + 20, chrEachPlayer.smCharInfo.szName, lstrlen(chrEachPlayer.smCharInfo.szName));
		dsTextLineOut(hdc, (float)x + 220, (float)y + 20, szBuff, lstrlen(szBuff));

		if (haSiegeBoardFlag) // Vida com bc ativo
			dsDrawTexImageFloatColor(MatEnergyBox[1], (float)x + 83, (float)y + 52, (float)(Percent + 3) * 1.79, (float)y - 22, 128, 16, 0, 0, (float)Percent + 3, 15, dwColor);
		else
			dsDrawTexImageFloatColor(MatEnergyBox[1], (float)x + 83, (float)y + 40, (float)(Percent + 3) * 1.79, (float)y + 2, 128, 16, 0, 0, (float)Percent + 3, 15, dwColor);
	}


	return TRUE;
}


int smCHAR::DrawStateBar2(int x, int y)
{
	int Percent;
	int	Percent2;
	int	Percent3;

	DWORD dwColor;

	if (smCharInfo.Life[1] == 0) return NULL;
	if (x < 0 || (x + (STATE_BAR_WIDTH / 2)) >= smScreenWidth) return NULL;
	if (y < 0 || (y + (STATE_BAR_HEIGHT / 2)) >= smScreenHeight) return NULL;

	//psSetDefaultRenderState();
	//GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	Percent = (smCharInfo.Life[0] * STATE_BAR_SIZE) / smCharInfo.Life[1];
	Percent2 = (smCharInfo.Life[0] * 511) / smCharInfo.Life[1];
	Percent2 = (Percent2 * Percent2) / 512;
	Percent3 = Percent2 & 0xFF;

	if (Percent2 < 256)
		dwColor = D3DCOLOR_RGBA(255, Percent3, 0, 255);
	else
		dwColor = D3DCOLOR_RGBA(255 - Percent3, 255, 0, 255);
	/*
		if( Percent==STATE_BAR_SIZE ) {
			dsDrawTexImageFloat(MatEnergyBox[1] , (float)x , (float)y , STATE_BAR_WIDTH*0.6f , STATE_BAR_HEIGHT*0.7f,
				128 , 16 , 0 , 0, STATE_BAR_WIDTH , 15, 255);
		}
		else {
			dsDrawTexImageFloat(MatEnergyBox[1] , (float)x , (float)y , ((float)Percent+3)*0.6f , STATE_BAR_HEIGHT*0.7f,
				128 , 16 , 0 , 0,(float)Percent+3 , 15, 255);

			dsDrawTexImageFloat(MatEnergyBox[0] , (float)x+((float)Percent+3)*0.6f , (float)y , (100-(float)Percent+3)*0.6f , STATE_BAR_HEIGHT*0.7f,
				128 , 16 , (float)Percent+4 , 0, 101 , 15, 255);
		}
	*/

	dsDrawTexImageFloat(MatEnergyBox[3], (float)x + 5, (float)y, STATE_BAR_WIDTH * 0.6f, STATE_BAR_HEIGHT * 0.7f, 128, 16, 0, 0, STATE_BAR_WIDTH, 15, 255);
	dsDrawTexImageFloatColor(MatEnergyBox[4], (float)x + 5, (float)y, ((float)Percent + 3) * 0.6f, STATE_BAR_HEIGHT * 0.7f, 128, 16, 0, 0, (float)Percent + 3, 15, dwColor);

	return TRUE;
}


// Â÷Â¡ °ÔÀÌÁö ±×¸®±â
int smCHAR::DrawChargingBar(int x, int y, int Charging, int ChargingMax)
{
	int	Percent;

	DWORD dwColor;

	if (smCharInfo.Life[1] == 0) return NULL;
	if (x < 0 || (x + (STATE_BAR_WIDTH2 / 2)) >= smScreenWidth) return NULL;
	if (y < 0 || (y + (STATE_BAR_HEIGHT2 / 2)) >= smScreenHeight) return NULL;

	//psSetDefaultRenderState();
	//GRAPHICDEVICE->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, FALSE );

	Percent = (Charging * STATE_BAR_SIZE) / ChargingMax;
	dwColor = D3DCOLOR_RGBA(160, 160, 255, 255);

	dsDrawTexImageFloat(MatEnergyBox[3], (float)x, (float)y, STATE_BAR_WIDTH2 * 0.6f, STATE_BAR_HEIGHT2 * 0.7f, 128, 16, 0, 0, STATE_BAR_WIDTH2, 15, 255);
	dsDrawTexImageFloatColor(MatEnergyBox[4], (float)x, (float)y, ((float)Percent + 3) * 0.6f, STATE_BAR_HEIGHT2 * 0.7f, 128, 16, 0, 0, (float)Percent + 3, 15, dwColor);

	return TRUE;
}

int smCHAR::DrawMotionBlur()
{

	if (HideWeapon || !DisplayFlag || !RendSucess || !smMotionInfo) return FALSE;

	if (RendPoint.z < 24 * 64 * fONE)
	{
		StartHandEffect(0);
	}

	if ((smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER) || RendPoint.z > 10 * 64 * fONE) return FALSE;

	if (ShootingMode || (smMotionInfo->MotionInfo[action].State != CHRMOTION_STATE_ATTACK && smMotionInfo->MotionInfo[action].State != CHRMOTION_STATE_SKILL))
		return FALSE;

	if (HvLeftHand.PatTool)
		DrawMotionBlurTool(&HvLeftHand);

	if (HvRightHand.PatTool)
		DrawMotionBlurTool(&HvRightHand);

	return TRUE;
}

int smCHAR::DrawMotionBlurTool(smCHARTOOL* ChrTool)
{
	DWORD ClipStates;
	smVERTEX* vp1, * vp2;
	smVERTEX* vp3, * vp4;

	int cnt, vcnt;
	int	sin, cos;
	DWORD pframe;
	int rx, ry, rz;
	smMATRIX* mWorld;
	POINT3D angle;
	POINT3D pTop, pBot;
	int szLevel;
	int mLevel;
	int mFrames;
	int mLevel2;
	smVERTEX* lpVertex;
	smFACE* lpFace;

	int r, g, b;


	if (!DisplayFlag || !dwActionItemCode || (dwActionItemCode & sinITEM_MASK2) == sinDS1) return FALSE;//|| ChrTool->SizeMax<fONE*8 ) return FALSE;

	if (ActionPattern == 0)
	{
		mLevel = 32;
		mFrames = 40;
		mLevel2 = mLevel << 1;
		lpVertex = MbVertex;
		lpFace = MbFace;
	}
	else {
		mLevel = 16;
		mFrames = 80;
		mLevel2 = mLevel << 1;
		lpVertex = MbVertex2;
		lpFace = MbFace2;
	}

	pTop.x = 0;
	pTop.y = 0;
	pTop.z = 0;

	pBot.x = 0;
	pBot.y = 0;
	pBot.z = ChrTool->SizeMax;

	szLevel = pBot.z / (mLevel << 1);

	if (dwActionItemTwoHand) {
		cnt = (-ANGLE_45 / 5) & ANGCLIP;

		sin = GetSin[cnt] >> 8;
		cos = GetCos[cnt] >> 8;

		pTop.y = -(pTop.z * sin) >> 8;
		pTop.z = (pTop.z * cos) >> 8;

		pBot.y = -(pBot.z * sin) >> 8;
		pBot.z = (pBot.z * cos) >> 8;
	}

	memcpy(&angle, &Angle, sizeof(POINT3D));
	angle.y = (-angle.y + ANGLE_180) & ANGCLIP;

	if (ActionGameMode) {
		if (angle.y > ANGLE_180)
			angle.y = ANGLE_270;
		else
			angle.y = ANGLE_90;
	}

	mWorld = &ChrTool->ObjBip->mWorld;

	if (ChrTool->PatTool && ChrTool->ObjBip) {
		for (cnt = 0; cnt < mLevel; cnt++) {
			pframe = frame - (cnt * mFrames);
			if (pframe < MotionInfo->StartFrame) pframe = MotionInfo->StartFrame;

			AnimObjectTree(ChrTool->ObjBip, pframe, angle.x, angle.y, angle.z);

			vcnt = cnt << 1;

			rx = pTop.x * mWorld->_11 +
				pTop.y * mWorld->_21 +
				pTop.z * mWorld->_31;

			ry = pTop.x * mWorld->_12 +
				pTop.y * mWorld->_22 +
				pTop.z * mWorld->_32;

			rz = pTop.x * mWorld->_13 +
				pTop.y * mWorld->_23 +
				pTop.z * mWorld->_33;

			lpVertex[vcnt].x = pX + (rx >> FLOATNS) + mWorld->_41;
			lpVertex[vcnt].z = pZ + (ry >> FLOATNS) + mWorld->_42;
			lpVertex[vcnt].y = pY + (rz >> FLOATNS) + mWorld->_43;

			vcnt++;

			rx = pBot.x * mWorld->_11 +
				pBot.y * mWorld->_21 +
				pBot.z * mWorld->_31;

			ry = pBot.x * mWorld->_12 +
				pBot.y * mWorld->_22 +
				pBot.z * mWorld->_32;

			rz = pBot.x * mWorld->_13 +
				pBot.y * mWorld->_23 +
				pBot.z * mWorld->_33;

			lpVertex[vcnt].x = pX + (rx >> FLOATNS) + mWorld->_41;
			lpVertex[vcnt].z = pZ + (ry >> FLOATNS) + mWorld->_42;
			lpVertex[vcnt].y = pY + (rz >> FLOATNS) + mWorld->_43;

			pTop.z += szLevel;
			pBot.z -= szLevel;
		}
	}

	//Å¬¸®ÇÎ ¸ðµå°ª
	ClipStates = (SMCLIP_NEARZ | SMCLIP_FARZ | SMCLIP_LEFT | SMCLIP_RIGHT | SMCLIP_TOP | SMCLIP_BOTTOM);

	smRender.Init();
	smRender.SetCameraPosi(smCHAR_CameraX, smCHAR_CameraY, smCHAR_CameraZ,
		smCHAR_CameraAngX, smCHAR_CameraAngY, smCHAR_CameraAngZ);


	for (cnt = 0; cnt < mLevel2; cnt++)
		smRender.AddObjectVertex(&lpVertex[cnt]);

	for (cnt = 0; cnt < (mLevel2 - 2); cnt += 2) {
		vp1 = &lpVertex[cnt];
		vp2 = &lpVertex[cnt + 1];
		vp3 = &lpVertex[cnt + 2];
		vp4 = &lpVertex[cnt + 3];

		if (vp1->x != vp3->x || vp1->y != vp3->y || vp1->z != vp3->z ||
			vp2->x != vp4->x || vp2->y != vp4->y || vp2->z != vp4->z) {
			smRender.AddObjectFace(&lpFace[cnt]);
			smRender.AddObjectFace(&lpFace[cnt + 1]);
		}
	}
	/*
					lpCurPlayer->HvRightHand.ColorBlink = 9;
					lpCurPlayer->HvRightHand.sColors[0] = 220;
					lpCurPlayer->HvRightHand.sColors[1] = 80;
					lpCurPlayer->HvRightHand.sColors[2] = 0;
					lpCurPlayer->HvRightHand.sColors[3] = 0;
	*/

	//		int r,g,b;

	r = smRender.Color_R;
	g = smRender.Color_G;
	b = smRender.Color_B;

	//½ºÅ³ »ö»ó Àû¿ë ( ¿ì¼± )
	if (AttackSkil) {
		cnt = SetSkillMotionBlurColor(AttackSkil);
	}

	//¹«±â »ö»ó Àû¿ë
	if (!cnt && ChrTool->ColorBlink) {
		smRender.Color_R += ChrTool->sColors[SMC_R] >> 1;
		smRender.Color_G += ChrTool->sColors[SMC_G] >> 1;
		smRender.Color_B += ChrTool->sColors[SMC_B] >> 1;
	}


	if (smRender.nRendFace) {
		smRender.SetClipStates(ClipStates);				// Å¬¸®ÇÎ ¼±ÅÃ
		smRender.ClipRendFace();							// ÀüÃ¼ ·»´õ¸µ ÆäÀÌ½º¸¦ Å¬¸®ÇÎ
		smRender.GeomVertex2D(FALSE);				// ¹öÅØ½º¸¦ 2DÁÂÇ¥·Î º¯È¯  
		smRender.RenderD3D();
	}

	smRender.Color_R = r;
	smRender.Color_G = g;
	smRender.Color_B = b;

	return TRUE;
}





//Ä³¸¯ÅÍ¿¡ ÆÐÅÏÀ» ¼³Á¤
int smCHAR::SetLoadPattern(char* szName)
{
	Pattern = 0;
	Flag = 1;
	return AddLoaderPattern((smCHAR*)&Head, szName);

}

//¹ÞÀº µ¥¹ÌÁö ¾ÏÈ­È­ ÄÚµå
DWORD smCHAR::GetAttackTrans_XorCode()
{
	return dwObjectSerial ^ lpCurPlayer->smCharInfo.dwObjectSerial ^ (((DWORD)this) >> 8);
}

//¹ÞÀºµ¥¹ÌÁö ¼öÄ¡ ÃÊ±âÈ­
int smCHAR::ResetAttackTrans()
{

	AttackTrans.code = GetAttackTrans_XorCode();
	return TRUE;
}

//¹ÞÀºµ¥¹ÌÁö ¼öÄ¡ ¾ÏÈ£È­ ¹ÝÀü
int smCHAR::XorAttackTrans()
{
	int cnt, len;
	DWORD dwCode;

	len = (sizeof(TRANS_ATTACKDATA) / sizeof(DWORD));//+2;
	DWORD* lpdwBuff = (DWORD*)&AttackTrans;

	dwCode = GetAttackTrans_XorCode();

	lpdwBuff[0] ^= dwCode;
	lpdwBuff[1] ^= dwCode;

	for (cnt = 2; cnt < len; cnt++) {
		lpdwBuff[cnt] ^= dwCode;
		dwCode += dwCode;
	}

	return TRUE;
}


//°ø°Ý ½ÇÇà
int smCHAR::PlayAttack(int EventFlag)
{
	int cnt, len, act;
	int block, ChanceBlock;
	smCHAR* lpChar;
	int	HolyBody_Undead = 0;
	int x, y, z;
	POINT3D	Pos1;//,Pos2;
	int temp;
	int Param1, Param2;
	int	bStunCount;


	TRANS_ATTACKDATA* lpTransAttackData;

	lpTransAttackData = &AttackTrans;

	XorAttackTrans();

	if (!lpTransAttackData->code || (RestartPlayCount > 0 && RestartPlayCount < 1000) ||
		(UseEtherCoreCode && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_EAT)) {
		//ResetAttackTrans();//lpTransAttackData->code = 0;
		AttackTrans.code = GetAttackTrans_XorCode();
		if (RestartPlayCount) Ptect_IncAttackCount(0);
		return FALSE;
	}

	if (lpTransAttackData->dwDamageChkSum != dm_GetDamgeChkSum_S2V(lpTransAttackData)) {	//ÄÚµå °Ë»ç ¿À·ù
		ResetAttackTrans();
		SendSetHackUser3(8580, lpTransAttackData->Power, 0);
		return FALSE;
	}

	Ptect_IncAttackCount(-1);

	if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD && DisplayFlag) {
		len = GetDistanceDbl(lpCurPlayer->pX, lpCurPlayer->pZ, pX, pZ);
		cnt = GetDistanceDbl(lpCurPlayer->pX, lpCurPlayer->pZ, lpTransAttackData->x, lpTransAttackData->z);

		if (smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER) {
			len = 0;			//PK ¸ðµå
			cnt = 0;
		}

		if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_FIGON || smCharInfo.dwCharSoundCode == snCHAR_SOUND_WEB ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SKELETONRANGE || smCharInfo.dwCharSoundCode == snCHAR_SOUND_IMP ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SCORPION ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NAZ ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NAZSENIOR || // ¹ÚÀç¿ø - ³¡¾ø´Â Å¾ 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ)
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SHADOW ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEEDOG ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_MUTANTTREE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SUCCUBUS ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_BERSERKER ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_STYGIANLORD ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_AVELIN_ARCHER ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_AVELIN_QUEEN ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_DMACHINE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SADNESS ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_RUNICGUARDIAN ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_HYPERMACHINE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_TURTLE_CANNON ||


			smCharInfo.dwCharSoundCode == snCHAR_SOUND_FROZEN ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CHAOSCARA ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_COLDEYE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_M_LORD ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_DARKGUARD ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_DARKMAGE ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_FIREWORM ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_SETO ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_SKILLMASTER ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_R ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_G ||
			smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_B ||

			smCharInfo.dwCharSoundCode == snCHAR_SOUND_GOBLINSHAMAN || // ¹ÚÀç¿ø - ÀúÁÖ¹ÞÀº ½ÅÀü 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) - °íºí¸°»þ¸Õ

			lpTransAttackData->AttackState == 0x80						//½ºÅ³°ø°Ý
			) {
			//¿ø°Å¸® °ø°Ý
			if (len < (460 * 460)) len = 0;

			cnt = 0;

			if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_WEB) {
				lpCurPlayer->PlaySlowCount = 70 * 2;			//2ÃÊµ¿¾È ´À¸®°Ô ¿òÁ÷ÀÓ
				lpCurPlayer->PlaySlowSpeed = 180;
			}
			if (!EventFlag) { //&& !dwAttackPlayTime ) {	
				dwAttackPlayTime = dwPlayTime + 1100;
				XorAttackTrans();
				return TRUE;
			}
		}

		if (lpTransAttackData->sRating[0] < 0) {
			Param1 = -lpTransAttackData->sRating[0];
			Param2 = lpTransAttackData->sRating[1];
			lpTransAttackData->sRating[0] = 0;
			lpTransAttackData->sRating[1] = 0;
		}
		else {
			Param1 = 0;
			Param2 = 0;
		}


		//ÁÖÀÎ°ø ¸ó½ºÅÍ ½ºÅ²ÀÏ °æ¿ì
		if (lpCurPlayer->smCharInfo.State == smCHAR_STATE_ENEMY) act = 1;
		else act = 0;

		if (lpTransAttackData->AttackState == 0x80) {
			x = (lpTransAttackData->x - lpCurPlayer->pX) >> FLOATNS;
			y = (lpTransAttackData->y - lpCurPlayer->pY) >> FLOATNS;
			z = (lpTransAttackData->z - lpCurPlayer->pZ) >> FLOATNS;
			if ((x * x + y * y + z * z) > (140 * 140)) {
				cnt = lpTransAttackData->AttackSize;
			}
			act = 1;			//¸ó½ºÅÍÀÇ ½ºÅ³°ø°Ý ( ¸íÁß 100% )
		}

		Record_RecvDamageCount++;			//µ¥¹ÌÁö Ä«¿îÅÍ

		if (cnt < lpTransAttackData->AttackSize && sinGetLife()>0 && (len < (330 * 330) || act) && DisplayAlpha == 0)
		{

			lpChar = this;

			block = FALSE;

			if (lpChar) {

				//if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER) {			//¾ÆÃ³
				//	cnt = CSKILL->GetSkillPoint(SKILL_EVASION_MASTERY);			//Ãß°¡ È¸ÇÇ ¸¶½ºÅÍ¸®
				//	if (cnt > 0 && cnt <= 10) {
				//		cnt = Evasion_Mastery_AddPercent[cnt - 1];
				//		if (cnt <= 36) {
				//			if ((rand() % 100) <= cnt) {
				//				cInterFace.SetStringEffect(HA_EVASION);
				//				block = 100;
				//				Record_BlockCount++;
				//			}
				//		}
				//	}
				//}

				if (lpCurPlayer->smCharInfo.Chance_Block) {
					ChanceBlock = sinGetBlockRating(lpChar->smCharInfo.Level);

					if ((lpCurPlayer->HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1) {
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_EXTREME_SHIELD) != 0) {		//ÀÍ½ºÆ®¸² ½¯µå
							ChanceBlock += (lpTransAttackData->AttackState >> 16);
						}
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_SPARK_SHIELD) != 0) {		//ÀÍ½ºÆ®¸² ½¯µå
							ChanceBlock += (lpTransAttackData->AttackState >> (16 + 8));
						}
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) != 0) {		//°¡µé¸® ½¯µå
							cnt = CSKILL->GetSkillPoint(SKILL_DIVINE_INHALATION);		//µð¹ÙÀÎ ½¯µå ÀÎ°è
							if (cnt > 0 && D_Inhalation_Block[cnt - 1] <= 10) {
								ChanceBlock += D_Inhalation_Block[cnt - 1];
							}
						}
						if (ChanceBlock > 50) ChanceBlock = 50;
					}
				}
				else
					ChanceBlock = 0;

				if ((rand() % 100) < ChanceBlock) {
					//ºí·°¼º°ø
					cInterFace.SetStringEffect(HA_BLOCK);
					block = TRUE;
					Record_BlockCount++;

					if (abs(lpChar->smCharInfo.Level - lpCurPlayer->smCharInfo.Level) <= AGING_SUB_LEVEL) {
						//·¹º§ 7Â÷ÀÌ ÀÌÇÏ´Â ¿¡ÀÌÂ¡ Àû¿ë
						sinCheckAgingLevel(SIN_AGING_BLOCK, false);
					}

					if ((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_SPARK_SHIELD) && lpChar) {

						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MECHANICIAN) {
							//½ºÆÄÅ© ½¯µå Àû¿ë
							dm_SendTransDamage(lpChar, 0, 0, 0, 0, SKILL_PLAY_SPARK_SHIELD | 0x100, FALSE);
							/*
							//½ºÆÄÅ© ½¯µå
							if ( lpCurPlayer->HvLeftHand.PatTool ) {
								if ( lpCurPlayer->GetToolBipPoint( &lpCurPlayer->HvLeftHand , &x,&y,&z ) ) {

									Pos1.x = x;//lpCurPlayer->pX;
									Pos1.y = y;//lpCurPlayer->pY;
									Pos1.z = z;//lpCurPlayer->pZ;

									Pos2.x = lpChar->pX;
									Pos2.y = lpChar->pY+24*fONE;
									Pos2.z = lpChar->pZ;

									AssaParticle_SparkShieldDefence( &Pos1 , &Pos2 );
									SkillPlaySound( SKILL_SOUND_SKILL_SPARK2	 , lpCurPlayer->pX,lpCurPlayer->pY,lpCurPlayer->pZ );
								}
							}
							*/
						}
						//µð¹ÙÀÎ ÀÎÈ¦·¹ÀÌ¼Ç
						if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_KNIGHT) {
							cnt = (lpTransAttackData->AttackState >> 16) & 0xFF;
							if (cnt > 0) {
								sinSetLife((sinGetLife() + cnt));
								ResetEnergyGraph(0);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­

								if (lpCurPlayer->HvLeftHand.PatTool) {
									if (lpCurPlayer->GetToolBipPoint(&lpCurPlayer->HvLeftHand, &x, &y, &z)) {
										Pos1.x = x;
										Pos1.y = y;
										Pos1.z = z;

										AssaParticle_DivineInhalationDefence(&Pos1);
									}
								}
							}
						}
					}
				}
			}

			//ºí·°À² °è»ê
			if (lpChar && (!block || act)) {

				//¹æ¾îÀ² °è»ê
				//cnt = GetPlayDefence( &smCharInfo , &lpCurPlayer->smCharInfo );
				if (sizeof(TRANS_ATTACKDATA) == lpTransAttackData->size)
					cnt = sinGetMonsterAccuracy(lpTransAttackData->sRating[0], lpTransAttackData->sRating[1]);
				else
					cnt = sinGetMonsterAccuracy(smCharInfo.Level, smCharInfo.Attack_Rating);

				if (dmAttackCount < 2) cnt <<= 2;			//°ø°ÝÀ» ¾ÈÇÒ½Ã¿¡´Â ¹æ¾î·ü Àý¹Ý

				//cnt = 100;
				if ((rand() % 100) <= cnt || act) {

					cnt = lpTransAttackData->Power;

					if ((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_HOLY_BODY)) {
						if (smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
							//È¦¸® ¹Ùµð ½ºÅ³ Àû¿ë ( ¾ðµ¥µå¸¸ Àû¿ë )
							//cnt -= (cnt*HolyBody_Absorb[DispApp_SkillPoint-1])/100;
							HolyBody_Undead = TRUE;
						}
					}

					if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_MAGICIAN) {
						temp = lpTransAttackData->AttackState >> 16;
						if (temp > 0) {
							AssaParticle_EnergyShieldDefence(lpCurPlayer);
							temp = sinGetMana() - temp;
							if (temp < 0) temp = 0;
							sinSetMana(temp);
							ResetEnergyGraph(1);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
						}
					}

					if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_PRIESTESS) {
						//¼­¸ó ¹«½ºÆç
						if (lpChar->smCharInfo.Brood == smCHAR_MONSTER_UNDEAD && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) != 0) {
							temp = lpTransAttackData->AttackState >> 16;
							if (temp > 0) {
								temp = sinGetLife() + temp;
								sinSetLife(temp);
								ResetEnergyGraph(0);						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
							}
						}
					}

					bStunCount = lpCurPlayer->PlayStunCount;
					lpCurPlayer->PlayStunCount = 0;

					/////////////////// PKMODE ///////////////////
					switch (Param1) {
					case SKILL_PLAY_ROAR:
					case SKILL_PLAY_SHIELD_STRIKE:
						if (!bStunCount) {
							lpCurPlayer->PlayStunCount = GetResistancePower(Param2 * 70, sITEMINFO_BIONIC);
							AssaParticle_Stune(lpCurPlayer, Param2 + 1);
						}
						break;
					case SKILL_PLAY_GROUND_PIKE:
						lpCurPlayer->PlaySlowCount = GetResistancePower(Param2 * 70, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 180;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_GLACIAL_SPIKE:
					case SKILL_PLAY_ICE_CRYSTAL:
						//case SKILL_PLAY_CHAIN_LIGHTNING://°ø¼º ¾óÀ½Å¾ // congelar personagens
						lpCurPlayer->PlaySlowCount = GetResistancePower(Param2 * 70, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 200;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_DISTORTION:
						lpCurPlayer->PlaySlowCount = Param2 * 70;
						lpCurPlayer->PlaySlowSpeed = 210;
						lpCurPlayer->PlayDistortion = lpCurPlayer->PlaySlowCount;
						lpCurPlayer->AttackIce = 0;
						break;
					case SKILL_PLAY_ENCHANT_WEAPON:
						lpCurPlayer->PlaySlowCount = GetResistancePower(100, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = Param2 * 70;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_DANCING_SWORD:
						lpCurPlayer->PlaySlowCount = GetResistancePower(140, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = Param2 * 70;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_FROST_JAVELIN:
						lpCurPlayer->PlaySlowCount = GetResistancePower(210, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = Param2 * 70;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_FIRE_CRYSTAL:		//°ø¼º ºÒÅ¾
						cnt = GetResistancePower(cnt, sITEMINFO_FIRE);
						break;
					case SKILL_PLAY_LIGHTNING_CRYSTAL:	//°ø¼º Àü±âÅ¾
						if (!bStunCount) {
							lpCurPlayer->PlayStunCount = GetResistancePower(Param2 * 70, sITEMINFO_LIGHTING);
							AssaParticle_Stune(lpCurPlayer, Param2 + 1);
						}
						break;
					}


					switch (smCharInfo.dwCharSoundCode) {
					case snCHAR_SOUND_COLDEYE:
					case snCHAR_SOUND_FROZEN:
						//case snCHAR_SOUND_ICEGOLEM:
						if ((lpTransAttackData->AttackState & 0xFF) == 0x80)						//½ºÅ³°ø°Ý
							lpCurPlayer->PlaySlowCount = GetResistancePower(70 * 4, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 200;
						lpCurPlayer->AttackIce = 1;
						break;
					case snCHAR_SOUND_FROST:
					case snCHAR_SOUND_DEATHKNIGHT:
						if ((lpTransAttackData->AttackState & 0xFF) == 0x80)						//½ºÅ³°ø°Ý
							lpCurPlayer->PlaySlowCount = GetResistancePower(70 * 6, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 200;
						lpCurPlayer->AttackIce = 1;
						break;

					case snCHAR_SOUND_CHAOSCARA:
						//Ä«¿À½ºÄ«¶ó ½ºÅ³
						if (Param1 == SKILL_PLAY_CHAOSCARA_VAMP && LastSkillParam) {
							sinSetMana(sinGetMana() - cnt);
						}
						break;

					case snCHAR_SOUND_STINGRAY:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
							ParkAssaParticle_Posion(chrAttackTarget, STINGRAY_POSION_TIME);		// µ¶°É¸°°Å Ç¥Çö	(Ä³¸¯ÅÍ)
							if (chrAttackTarget == lpCurPlayer) {
								len = GetResistancePower(70 * STINGRAY_POSION_TIME, sITEMINFO_POISON);
								lpCurPlayer->PlayPoison[0] = 1;
								lpCurPlayer->PlayPoison[1] = len;
							}
						}
						break;

					case snCHAR_SOUND_KINGBAT: // ¹ÚÀç¿ø - ÀúÁÖ¹ÞÀº ½ÅÀü 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) - Å·¹î
						ParkAssaParticle_Posion(chrAttackTarget, STINGRAY_POSION_TIME);		// µ¶°É¸°°Å Ç¥Çö	(Ä³¸¯ÅÍ)
						if (chrAttackTarget == lpCurPlayer) {
							len = GetResistancePower(70 * STINGRAY_POSION_TIME, sITEMINFO_POISON);
							lpCurPlayer->PlayPoison[0] = 1;
							lpCurPlayer->PlayPoison[1] = len;
						}
						break;

					case snCHAR_SOUND_DEVIL_BIRD:
						if (chrAttackTarget) {
							ParkDevilBirdAttackNormal(chrAttackTarget);
						}
						break;

						/////////////////////////

					case snCHAR_SOUND_BLOODYKNIGHT:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
							ParkAssaParticle_BloodySkillStun(chrAttackTarget);
							lpCurPlayer->PlayStunCount = 70 * 3;
						}
						break;
					case snCHAR_SOUND_HELLHOUND:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
							ParkAssaParticle_HellHoundSkillTaget(chrAttackTarget);
						}
						break;
					case snCHAR_SOUND_CHIMERA:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
							lpCurPlayer->PlayStunCount = 70 * 3;
						}
						break;


					}

					// Teste diferença dano em modo PK
					if (PkMode_CharState == smCHAR_STATE_USER)
						cnt = (cnt - ((lpCurPlayer->smCharInfo.Absorption * PKMODE_ABSORB) / 100));
					else
						cnt = (cnt - lpCurPlayer->smCharInfo.Absorption);

					if (cnt <= 0) cnt = 1;

					if (abs(lpChar->smCharInfo.Level - lpCurPlayer->smCharInfo.Level) <= AGING_SUB_LEVEL) {
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, false);   // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(ºÎÃ÷)
						sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, false);  // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(Àå°©)
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, false); // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(¾Ï¸´)
					}

					CheckCharForm();

					sinSetLife(sinGetLife() - cnt);


					SendShowDmg(cnt, Type::Damage, dwObjectSerial);


					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_SUCCUBUS) {
						if (lpTransAttackData->sRating[0] > 75) {
							if (lpTransAttackData->sRating[0] < 80)	//¼­Å¥¹ö½º
								sinSetStamina(sinGetStamina() - ((lpCurPlayer->smCharInfo.Stamina[1] * 6) / 100));
							else 	//ÀÎÅ¥¹ö½º
								sinSetMana(sinGetMana() - ((lpCurPlayer->smCharInfo.Mana[1] * 6) / 100));
						}
					}

					//ÇÇÆ¢±â´Â ÆÄÆ¼Å¬
					StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 16 * fONE, lpCurPlayer->pZ, EFFECT_DAMAGE1);
					SetDynLight(lpCurPlayer->pX, lpCurPlayer->pY + 16 * fONE, lpCurPlayer->pZ, 160, 0, 0, 0, 180);

					if (((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_PHYSICAL_ABSORB) && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_HOLY_BODY) == 0) || HolyBody_Undead) {
						//ÇÇÁöÅ¬¾÷¼Ù Àû¿ëÀÌÆåÆ®
						StartEffect(lpCurPlayer->pX, lpCurPlayer->pY + 24 * fONE, lpCurPlayer->pZ, EFFECT_PHYSICAL_ABSORB_DAMAGE);
					}

					if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DAMAGE && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_EAT && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_ATTACK &&
						lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_SKILL && cnt > 1) {

						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
						CharPlaySound(lpCurPlayer);
					}

					//Char Morreu
					if (sinGetLife() <= 0)
					{

						if (sinQuest_ChangeJob4.CODE == SIN_QUEST_CODE_CHANGEJOB4 &&
							lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == QUEST_ARENA_FIELD)
						{
							OpenEachPlayer(this);
							sinBattleNpcResult(0);
							sinSetLife(0);
							lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							CharPlaySound(lpCurPlayer);
							ResetAttackTrans();
							return TRUE;
						}

						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
						sinSetLife(0);
						CharPlaySound(lpCurPlayer);

						SendCommand_AreaServer(smTRANSCODE_DEAD_USER, dwObjectSerial, 0, 0, 0);
						SendCommand_DataServer(smTRANSCODE_DEAD_USER, dwObjectSerial, 1, 0, 0);
						WeaponEffect = 0;
						dwWeaponEffectTime = 0;

						CancelBuffSkills();

						DeadPlayerExp(lpCurPlayer);

						sinCheckQuest85Die();

						SaveGameData();

						PlayerKilling(dwObjectSerial);
						ResetAttackTrans();
						SetFullZoomMap(0);
						return TRUE;
					}

					ReformCharForm();

				}
				else {
					//È¸ÇÇ(¹æ¾î) ¼º°ø
					cInterFace.SetStringEffect(HA_DEFANCE);
					Record_DefenceCount++;
				}
			}
			dwBattleTime = dwPlayTime;			//ÀüÅõ ½Ã°£ ±â·Ï
		}
	}

	ResetAttackTrans();	//lpTransAttackData->code = 0;

	return TRUE;
}


//À§Ä¡¸¦ ¹Þ¾Æ ¿¹Ãøµ¥ÀÌÅ¸¸¦ ¸¸µé¾î ³Ö´Â´Ù
int smCHAR::MakePlayBuffFromPosi(int px, int py, int pz, int anX, int anY, int anZ, int act)
{
	int len;
	int sx, sy, sz;
	int x, y, z;
	int mx, my, mz;
	int PlayBuffPosi;
	int cnt;
	int aY;
	int dist;
	//smMOTIONINFO	*lpMotionInfo;
	//int SelFrame;

	if (!smMotionInfo) return FALSE;

	len = 0x40;


	if (PlayBuffPosi_End <= 0) {
		sx = px;
		sy = py;
		sz = pz;
	}
	else {
		PlayBuffPosi = (PlayBuffPosi_End - 1) & PLAYBUFF_MASK;
		sx = PlayBuff[PlayBuffPosi].x;
		sy = PlayBuff[PlayBuffPosi].y;
		sz = PlayBuff[PlayBuffPosi].z;
	}

	if (sx == px && sy == py && sz == pz)
		aY = anY;
	else
		aY = GetRadian2D(sx, sz, px, pz);

	x = (px - sx) >> FLOATNS;
	z = (py - sy) >> FLOATNS;

	dist = x * x + z * z;
	if (dist > (110 * 110)) {
		//¿¹Ãø µ¥ÀÌÅ¸ °£°ÝÀÌ ³Ê¹« ±ä °æ¿ì

		PlayBuffPosi = PlayBuffPosi_End & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = px;
		PlayBuff[PlayBuffPosi].y = py;
		PlayBuff[PlayBuffPosi].z = pz;

		PlayBuff[PlayBuffPosi].angle[0] = 0;
		PlayBuff[PlayBuffPosi].angle[1] = aY;
		PlayBuff[PlayBuffPosi].angle[2] = 0;
		PlayBuff[PlayBuffPosi].angle[3] = 0;

		PlayBuff[PlayBuffPosi].frame = smPLAYBUFF_FRAME_NONE | 80;
		PlayBuff[PlayBuffPosi].dwTargetSerial = 0;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, wStickItems, sizeof(short) * 4);

		PlayBuffPosi_End++;
		return TRUE;
	}

	mx = ((px - sx) << 8) / len;
	my = ((py - sy) << 8) / len;
	mz = ((pz - sz) << 8) / len;

	x = 0;	y = 0;	z = 0;

	for (cnt = 0; cnt < len; cnt++) {
		PlayBuffPosi = (PlayBuffPosi_End + cnt) & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = sx + (x >> 8);
		PlayBuff[PlayBuffPosi].y = sy + (y >> 8);
		PlayBuff[PlayBuffPosi].z = sz + (z >> 8);

		PlayBuff[PlayBuffPosi].angle[0] = 0;
		PlayBuff[PlayBuffPosi].angle[1] = aY;
		PlayBuff[PlayBuffPosi].angle[2] = 0;
		PlayBuff[PlayBuffPosi].angle[3] = 0;

		PlayBuff[PlayBuffPosi].frame = smPLAYBUFF_FRAME_NONE | 80;
		PlayBuff[PlayBuffPosi].dwTargetSerial = 0;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, wStickItems, sizeof(short) * 4);

		x += mx;
		y += my;
		z += mz;
	}
	PlayBuffPosi_End = PlayBuffPosi_End + len;

	return TRUE;
}

int smCHAR::MakePlayBuffFromRecvData(smPLAYBUFF* StartBuff, smPLAYBUFF* EndBuff, int len)
{
	int cnt;
	int x, y, z;
	int px, py, pz;
	int mx, my, mz;
	int PlayBuffPosi;
	int aX, aY, aZ;
	int saX, saY, saZ;
	int taX, taY, taZ;
	int frameState;
	int frameStep;

	if (!smMotionInfo || len <= 1) return FALSE;

	if (!EndBuff) {
		PlayBuffPosi = PlayBuffPosi_End & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = StartBuff->x;
		PlayBuff[PlayBuffPosi].y = StartBuff->y;
		PlayBuff[PlayBuffPosi].z = StartBuff->z;

		PlayBuff[PlayBuffPosi].angle[0] = StartBuff->angle[0] & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[1] = StartBuff->angle[1] & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[2] = StartBuff->angle[2] & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[3] = StartBuff->angle[3];

		PlayBuff[PlayBuffPosi].frame = StartBuff->frame;
		PlayBuff[PlayBuffPosi].dwTargetSerial = StartBuff->dwTargetSerial;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, StartBuff->wStickItems, sizeof(short) * 4);
		PlayBuffPosi_End++;
		return TRUE;
	}

	px = StartBuff->x;
	py = StartBuff->y;
	pz = StartBuff->z;

	mx = ((EndBuff->x - px) << 8) / len;
	my = ((EndBuff->y - py) << 8) / len;
	mz = ((EndBuff->z - pz) << 8) / len;

	aX = StartBuff->angle[0];
	aY = StartBuff->angle[1];
	aZ = StartBuff->angle[2];

	taX = ANGLE_180 - EndBuff->angle[0];
	taY = ANGLE_180 - EndBuff->angle[1];
	taZ = ANGLE_180 - EndBuff->angle[2];

	saX = (aX + taX) & ANGCLIP;
	saY = (aY + taY) & ANGCLIP;
	saZ = (aZ + taZ) & ANGCLIP;

	saX = ((saX - ANGLE_180) << 8) / len;
	saY = ((saY - ANGLE_180) << 8) / len;
	saZ = ((saZ - ANGLE_180) << 8) / len;

	if (StartBuff->frame < CHAR_FRAME_MASK)
	{
		frameState = smMotionInfo->MotionInfo[StartBuff->angle[3]].State;

		if (frameState == CHRMOTION_STATE_WALK || frameState == CHRMOTION_STATE_RUN)
		{
			aY = GetRadian2D(px, pz, EndBuff->x, EndBuff->z);
			saY = 0;
		}
	}


	x = 0;
	y = 0;
	z = 0;

	taX = 0;
	taY = 0;
	taZ = 0;

	cnt = len - 1;
	if (cnt <= 0) {
		frameStep = 80;
	}
	else {
		frameStep = (EndBuff->frame - StartBuff->frame) / cnt;
	}

	if (frameStep >= 4096 || frameStep < 20) {
		frameStep = 80;
	}

	for (cnt = 0; cnt < len; cnt++) {
		PlayBuffPosi = (PlayBuffPosi_End + cnt) & PLAYBUFF_MASK;

		PlayBuff[PlayBuffPosi].x = px + (x >> 8);
		PlayBuff[PlayBuffPosi].y = py + (y >> 8);
		PlayBuff[PlayBuffPosi].z = pz + (z >> 8);
		/*
				PlayBuff[ PlayBuffPosi ].angle[0] = (aX+(taX>>8)) & ANGCLIP;
				PlayBuff[ PlayBuffPosi ].angle[1] = (aY+(taY>>8)) & ANGCLIP;
				PlayBuff[ PlayBuffPosi ].angle[2] = (aZ+(taZ>>8)) & ANGCLIP;
		*/
		PlayBuff[PlayBuffPosi].angle[0] = aX & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[1] = aY & ANGCLIP;
		PlayBuff[PlayBuffPosi].angle[2] = aZ & ANGCLIP;

		PlayBuff[PlayBuffPosi].angle[3] = StartBuff->angle[3];

		PlayBuff[PlayBuffPosi].frame = smPLAYBUFF_FRAME_NONE | frameStep;

		PlayBuff[PlayBuffPosi].dwTargetSerial = StartBuff->dwTargetSerial;

		memcpy(PlayBuff[PlayBuffPosi].wStickItems, StartBuff->wStickItems, sizeof(short) * 4);

		x += mx;
		y += my;
		z += mz;

		taX += saX;
		taY += saY;
		taZ += saZ;
	}

	PlayBuff[PlayBuffPosi_End & PLAYBUFF_MASK].frame = StartBuff->frame;
	PlayBuff[(PlayBuffPosi_End + len - 1) & PLAYBUFF_MASK].frame = EndBuff->frame;

	PlayBuffPosi_End = PlayBuffPosi_End + len;

	return TRUE;
}

int smCHAR::TransDelayCheck()
{
	int Delay;

	if (TransSock && TransSock->ConnectDirection) {
		Delay = PlayCounter - TransLastSendCnt;

		if (Delay > TransDelayMax) {
			TransSendWait = 0;
			TransLastSendCnt = PlayCounter - 16;
			return TRUE;
		}
	}

	return FALSE;
}

int smCHAR::SetDistSendCnt(smCHAR* player)
{
	int x, y, z;
	int dist;

	x = (pX - player->pX) >> FLOATNS;
	y = (pY - player->pY) >> FLOATNS;
	z = (pZ - player->pZ) >> FLOATNS;

	dist = x * x + y * y + z * z;

	if (dist < DIST_TRANSLEVEL_HIGH || !DisplayFlag) {
		TransMinCnt = 8;
		TransLevel = 0;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 4;
		return TRUE;
	}
	if (dist < DIST_TRANSLEVEL_MID) {
		TransMinCnt = PLAYCOUNT_PER_SECOND;
		TransLevel = 1;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 6;
		return TRUE;
	}
	if (dist < DIST_TRANSLEVEL_LOW) {
		TransMinCnt = PLAYCOUNT_PER_SECOND * 5;
		TransLevel = 2;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 10;
		return TRUE;
	}

	TransMinCnt = PLAYCOUNT_PER_SECOND * 15;
	TransLevel = 3;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 25;

	return TRUE;
}

int smCHAR::FormSetPlayerInfo()
{

	int cnt = 0;

	if (!Pattern) {
		//ÆÐÅÏ ¿À·ù °æ°í - ·Î±×¿¡ ±â·Ï
		char szBuff[128];

		wsprintf(szBuff, "Mesh Pattern Error [ %s ] ( %s %s )", smCharInfo.szName, smCharInfo.szModelName, smCharInfo.szModelName2);
		Record_ClinetLogFile(szBuff);
	}


	PHeight = pY;

	smCharInfo.bUpdateInfo[1] = 0xFF;			//½ºÅ³Æ÷ÀÎÆ® ÃÊ±âÈ­

	if (this != lpCurPlayer && lpCurPlayer->MotionInfo->State == CHRMOTION_STATE_STAND &&
		abs(lpCurPlayer->pX - pX) < 32 * fONE && abs(lpCurPlayer->pZ - pZ) < 32 * fONE) {

		if (lpCurPlayer->OnStageField >= 0) {
			if (StageField[lpCurPlayer->OnStageField]->CheckStartPoint(lpCurPlayer->pX, lpCurPlayer->pZ) == TRUE) {
				lpCurPlayer->MoveFlag = TRUE;
				lpCurPlayer->MoveCnt = 100;
			}
		}
	}

	//½Å±Ô ÀÔ·Â Ä³¸¯ÅÍ ¹Ù´Ú È®ÀÎ
	if (smCharInfo.State != smCHAR_STATE_ENEMY) {
		OnStageField = -1;
		if (smGameStage[0] && Pattern) {
			cnt = smGameStage[0]->GetFloorHeight(pX, pY, pZ, Pattern->SizeHeight);
			OnStageField = 0;
		}
		if (cnt == CLIP_OUT && smGameStage[1]) {
			OnStageField = 1;
		}
	}

	PatSizeLevel = smCharInfo.SizeLevel;
	if (PatSizeLevel >= 0x1000) PatSizeLevel = 1;

	if (TransSock && TransSock->ConnectDirection) 	TransSendWait = 0;
	else TransSendWait = 1;

	TransLastSendCnt = PlayCounter;

	TransMinCnt = 8;
	TransLevel = 0;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 4;		//4ÃÊ

	PlayBuffPosi_End = 0;
	PlayBuffCnt = 0;

	DisplayAlpha = -256;
	DispLifeBar = 0;

	switch (smCharInfo.dwCharSoundCode) {
	case snCHAR_SOUND_FIGON:
		//ÆÄÀÌ°ï ¼Õ¿¡ ºÒ ºÙÀÌ±â
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 L Finger1");
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R Finger1");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_ILLUSIONKNIGHT:
		//ÀÏ·çÀü ³ªÀÌÆ® - ¹ÝÅõ¸í
		RendAlpha = -180;
		break;

	case snCHAR_SOUND_MUDY:
		//¸Óµð
		RenderLatter = TRUE;
		break;

	case snCHAR_SOUND_WOLVERLIN:
		//¿ï¹ö¸° »ö»ó
		if (smCharInfo.State == smCHAR_STATE_ENEMY) {
			sAddColorEffect[SMC_R] = 100;
			sAddColorEffect[SMC_G] = 100;
		}
		break;

	case snCHAR_SOUND_SEN:
		//¼¾ Á¶¸í
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_MUTANTTREE:
		//¹ÂÅÏÆ® Æ®¸® ¿¬±â
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip05");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_SHADOW:
		//½¦µµ¿ì
		if (Pattern && Pattern->TmParent) {
			HvLeftHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 L UpperArm");
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R UpperArm");
		}
		else {
			HvLeftHand.ObjBip = 0;
			HvRightHand.ObjBip = 0;
		}
		break;

	case snCHAR_SOUND_MRGHOST:
		RendAlpha = -80;
		break;

	case snCHAR_SOUND_FURY:
		AssaParticle_Fury_Eye(this);			//Ç»¸® ´«ºû

		switch (smCharInfo.Level)				//Äù½ºÆ®¿ë Ç»¸®¿¡ »ö»ó ¼³Á¤
		{
		case 118:
			sAddColorEffect[SMC_R] = 100;
			sAddColorEffect[SMC_G] = 100;
			RendAlpha = -80;
			break;
		case 119:
			sAddColorEffect[SMC_R] = 100;
			RendAlpha = -80;
			break;
		case 120:
			sAddColorEffect[SMC_B] = 100;
			RendAlpha = -80;
			break;
		}
		break;

	case snCHAR_SOUND_BEEVIL:							//¹ú
		RenderLatter = TRUE;
		break;

	case snCHAR_SOUND_GUARDIAN_SAINT:		//°¡µð¾ð-¼¼ÀÎÆ® ( ¸ö¿¡¼­ ºûÀÌ Á¡¸ê )
		sAddColorEffect[SMC_R] = 80;
		sAddColorEffect[SMC_G] = 80;
		sAddColorEffect[SMC_B] = 20;

		AssaParticleClanMonster(this);
		break;

	case  snCHAR_SOUND_CHAINGOLEM:			//Ã¼ÀÎ°ñ·½
		AssaParticle_MonsterIronChainGolem(this);
		break;

	case snCHAR_SOUND_RUNICGUARDIAN:		//·ç´Ð °¡µð¾È
		AssaParticle_MonsterRunicGuardianHand(this);
		break;

	case snCHAR_SOUND_BABEL:				//¹Ùº§ (·ç´Ð°¡µð¾È º¸½º)
		AssaParticle_MonsterRunicGuardianBossWeapon(this);
		break;

	case snCHAR_SOUND_CHAOSCARA:
		ParkAssaParticle_MonsterIceChaosKara(this);
		break;

	case snCHAR_SOUND_DEATHKNIGHT:			//¹ß·»Åä
		ParkAssaParticle_MonsterIceValentoBody(this);
		break;

	case snCHAR_SOUND_S_F_ELEMENTAL:		//¼ÒÈ¯¸÷ È­ÀÌ¾î¿¤¸®¸àÅ»
		SkillArchMageFireElementalHand(this);
		break;

	case snCHAR_SOUND_NPC_SKILLMASTER:		//½ºÅ³¸¶½ºÅÍ
		if (Pattern && Pattern->TmParent)
			AttackObjBip = Pattern->TmParent->GetObjectFromName("bip01 weapon");
		break;

	case snCHAR_SOUND_NPC_MAGICMASTER:		//¸ÅÁ÷¸¶½ºÅÍ
		if (Pattern && Pattern->TmParent)
			AttackObjBip = Pattern->TmParent->GetObjectFromName("Bip01 staff02");
		break;

	case snCHAR_SOUND_CASTLE_SOLDER_A:
	case snCHAR_SOUND_CASTLE_SOLDER_B:
	case snCHAR_SOUND_CASTLE_SOLDER_C:
		if (Pattern && Pattern->TmParent)
			AttackObjBip = Pattern->TmParent->GetObjectFromName("Bip01 Effect");
		break;

	case snCHAR_SOUND_KELVEZU:
		ParkKelvezuJin(this);
		break;
	case snCHAR_SOUND_REVIVED_KNIGHT:	// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ³ªÀÌÆ®
		if (Pattern && Pattern->TmParent)
		{
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R Hand");
		}
		break;
	case snCHAR_SOUND_REVIVED_PIKEMAN:
		if (Pattern && Pattern->TmParent)
		{
			AttackObjBip = Pattern->TmParent->GetObjectFromName("bip01 wea");
		}
		break;
	case snCHAR_SOUND_REVIVED_ARCHER:
		if (Pattern && Pattern->TmParent)
		{
			HvRightHand.ObjBip = Pattern->TmParent->GetObjectFromName("Bip01 R Hand");
		}
		break;
	}



	if (smCharInfo.wPlayClass[0] == MONSTER_CLASS_BOSS) {
		//º¸½º ¸ó½ºÅÍ
		if (smCharInfo.State == smCHAR_STATE_ENEMY) {
			sAddColorEffect[SMC_R] = 130;
			sAddColorEffect[SMC_G] = 50;
			sAddColorEffect[SMC_B] = 70;
		}
	}

	return TRUE;
}

//¼ö½Å ¹ÞÀº »ó´ë ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸ ¼³Á¤
int smCHAR::SetTransPlayerInfo(smTRNAS_PLAYERINFO* lpTransPlayerInfo)
{

	memcpy(&smCharInfo, &lpTransPlayerInfo->smCharInfo, sizeof(smCHAR_INFO));
	ResetAttackTrans();

	if (!PatLoading)
		AddLoaderPattern((smCHAR*)&Head, smCharInfo.szModelName, smCharInfo.szModelName2);

	pX = lpTransPlayerInfo->x;
	pY = lpTransPlayerInfo->y;
	pZ = lpTransPlayerInfo->z;

	FormSetPlayerInfo();

	if (StageField[lpCurPlayer->OnStageField]->FieldCode == FIELD_ARENA && EventoArena::GetInstance()->stageArena > 0) {
		smCharInfo.ClassClan = 0;
	}
	else {
		if (smCharInfo.ClassClan) {
			//Å¬·£ Á¤º¸
			ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
			dwClanInfoTime = dwPlayTime;

			if (GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)	//Ä³½½ ¸¶½ºÅÍ Å¬·£
				Clan_CastleMasterFlag = TRUE;
		}
	}

	PkMode_CharState = 0;

	return TRUE;
}

//¼ö½Å ¹ÞÀº »ó´ë ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸ ¼³Á¤
int smCHAR::SetTransPlayerInfoQuick(smTRNAS_PLAYERINFO_QUICK* lpTransPlayerInfo)
{
	int len;

	len = sizeof(smTRNAS_PLAYERINFO_QUICK) - lpTransPlayerInfo->size;

	ZeroMemory(&smCharInfo, sizeof(smCHAR_INFO));
	smCharInfo.Life[0] = 10;
	smCharInfo.Life[1] = 50;

	memcpy(&smCharInfo, &lpTransPlayerInfo->smCharInfo, sizeof(smCHAR_INFO) - len);
	ResetAttackTrans();

	if (!PatLoading)
		AddLoaderPattern((smCHAR*)&Head, smCharInfo.szModelName, smCharInfo.szModelName2);

	pX = lpTransPlayerInfo->x;
	pY = lpTransPlayerInfo->y;
	pZ = lpTransPlayerInfo->z;

	FormSetPlayerInfo();


	if (StageField[lpCurPlayer->OnStageField]->FieldCode == FIELD_ARENA && EventoArena::GetInstance()->stageArena > 0) {
		smCharInfo.ClassClan = 0;
	}
	else {
		if (smCharInfo.ClassClan) {
			//Å¬·£ Á¤º¸
			ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
			dwClanInfoTime = dwPlayTime;

			if (GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)	//Ä³½½ ¸¶½ºÅÍ Å¬·£
				Clan_CastleMasterFlag = TRUE;
		}
	}


	PkMode_CharState = 0;

	return TRUE;
}

//µ¥ÀÌÅ¸ ÀÔ¼ö ( ±×·ì Ã³¸®¿ë )
int smCHAR::RecvPlayData2(char* lpData)
{
	smPLAYDATA* lpPlayData;
	int RecvCode;
	int cnt;
	int len;
	int LastBuffPosi_End;
	int act;
	char* lpBuff;
	int	Code, Size;
	smCHAR* lpChar;

	RecvCode = ((int*)lpData)[1];

	switch (RecvCode) {
	case smTRANSCODE_PLAYDATA1:
		//°ÔÀÓ ÁøÇà ¼ö½Å
		lpPlayData = (smPLAYDATA*)lpData;

		act = lpPlayData->angle[3];
		//À§Ä¡¸¦ ¹Þ¾Æ ¿¹Ãøµ¥ÀÌÅ¸¸¦ ¸¸µé¾î ³Ö´Â´Ù
		MakePlayBuffFromPosi(lpPlayData->x, lpPlayData->y, lpPlayData->z, lpPlayData->angle[0], lpPlayData->angle[1], lpPlayData->angle[2], act);
		MoveCnt = 0;

		/*
				if ( PlayBuffCnt==PlayBuffPosi_End ) {

					pX = lpPlayData->x;
					pY = lpPlayData->y;
					pZ = lpPlayData->z;

					Angle.x = lpPlayData->angle[0] & ANGCLIP;
					Angle.y = lpPlayData->angle[1] & ANGCLIP;
					Angle.z = lpPlayData->angle[2] & ANGCLIP;
					act = lpPlayData->angle[3];

					if ( act!=action ) {//|| ( act!=CHRMOTION_WALK && act!=CHRMOTION_STAND ) )
						if ( Pattern )
							ChangeMotion( act );
						else
							act = 0;
					}

					action = act;

					MoveCnt = 0;
					dwTarget = 0;
					//PlayBuffCnt = PlayBuffPosi_End;
				}
		*/
		LastPlayDataType = 1;			//¿ø°Å¸® ±³½Å
		smCharInfo.bUpdateInfo[1] = 0xFF;
		break;

	case smTRANSCODE_TRANSPLAYDATA:
		//°ÔÀÓ ÁøÇà ¼ö½Å

		smTRANS_PLAYDATA* lpTransPlayData;
		smTRANS_PLAYBUFF* lpTransPlayBuff;
		smTRANS_PLAYBUFF* lpTransPlayBuff2;

		LastPlayDataType = 0;			//±Ù°Å¸® ±³½Å

		lpTransPlayData = (smTRANS_PLAYDATA*)lpData;
		lpTransPlayBuff = (smTRANS_PLAYBUFF*)(lpData + sizeof(smTRANS_PLAYDATA));

		LastBuffPosi_End = PlayBuffPosi_End;

		smCharInfo.Life[0] = lpTransPlayData->Hp[0];
		smCharInfo.Life[1] = lpTransPlayData->Hp[1];
		smCharInfo.Mana[0] = lpTransPlayData->Mp[0];
		smCharInfo.Mana[1] = lpTransPlayData->Mp[1];
		bPartyLeader = lpTransPlayData->bPartyLeader;

		if (smCharInfo.State == 0x80 || PkMode_CharState == 0x80)
		{
			iMountID = lpTransPlayData->iMountID;

			if (iMountID != 0)
			{
				auto pFoundMount = cMountManager.GetMountHandler(this);

				if (iMountID == MOUNTID_Invisible && pFoundMount)
					pFoundMount->SetVisible(false);
				else
					cMountManager.AddMount(this, (EMountID)iMountID);
			}
		}
		else
		{
			iMountID = 0;
			pMount = nullptr;
		}

		if (LevelLog) {
			if (smCharInfo.State == smCHAR_STATE_USER && lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL] > LevelLog) {
				//»ó´ë¹æ ·¹º§¾÷
				StartEffect(pX, pY + 32 * fONE, pZ, EFFECT_LEVELUP1);
				esPlaySound(7, GetDistVolume(pX, pY, pZ));
			}
		}

		LevelLog = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL];
		PotionLog = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_POTION] & 0x0F;
		CriticalLog = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_POTION] >> 4;
		dwDispAppSkill = lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL];

		/*
				if ( dwDispAppSkill&SKILL_APPMASK_ICE ) {
					//¾óÀ½ °ø°Ý¹ÞÀ½
					if ( AttackIce<=0 ) AttackIce = 1;
				}
				else {
					if ( AttackIce>0 ) AttackIce = -AttackIce;
				}
		*/

		if (CriticalLog >= 8) CriticalLog = -1;
		AttackCritcal = CriticalLog;

		if (lpTransPlayData->PlayBuffCnt > 1) {

			for (cnt = 0; cnt < lpTransPlayData->PlayBuffCnt - 1; cnt++) {
				lpTransPlayBuff2 = lpTransPlayBuff + 1;

				len = lpTransPlayBuff2->BuffCount - lpTransPlayBuff->BuffCount;
				MakePlayBuffFromRecvData(&lpTransPlayBuff->PlayBuff, &lpTransPlayBuff2->PlayBuff, len);

				lpTransPlayBuff++;
			}

			dwTarget = lpTransPlayData->dwTarget;
			/*
						if ( lpTransPlayData->bUpdateInfo[0]!=smCharInfo.bUpdateInfo[0]) {
							//³»¿ë Á¤º¸°¡ ¹Ù²ñ
							//Close();				//Ä³¸¯ÅÍ ´Ý±â
							//smCharInfo.bUpdateInfo[1] = lpTransPlayData->bUpdateInfo[0];


							//smCharInfo.bUpdateInfo[0] = lpTransPlayData->bUpdateInfo[0];

							if ( !dwUpdateCharInfoTime || (dwUpdateCharInfoTime+1000)<dwPlayTime ) {
								Send_GetCharInfo( dwObjectSerial , ServerCode );

								if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_HUNGKY && !dwUpdateCharInfoTime) {
									//ÀÌÆåÆ® ( ÇæÅ© º¯½Å )
									StartEffect( pX,pY+48*fONE,pZ, EFFECT_RETURN1 );
									SetDynLight( pX,pY,pZ, 50, 100,70,0, 300 , 1 );

									//º¯½Å ¼Ò¸®
									cnt = MotionInfo->State;
									MotionInfo->State=CHRMOTION_STATE_WARP;
									CharPlaySound( this );
									MotionInfo->State = cnt;
								}
								dwUpdateCharInfoTime = dwPlayTime;
							}
						}
			*/
			if (smCharInfo.bUpdateInfo[1] == 0xFF) {
				//½ºÅ³Æ÷ÀÎÅÍ ¿À·¡µÇ¼­ »õ·Î °»½Å
				smCharInfo.bUpdateInfo[1] = lpTransPlayData->bUpdateInfo[1];
			}
			else {
				if (lpTransPlayData->bUpdateInfo[1] != smCharInfo.bUpdateInfo[1]) {
					//½ºÅ³ Æ÷ÀÎÆ® º¯È­
					if (lpTransPlayData->bUpdateInfo[1] < smCharInfo.bUpdateInfo[1])
						LearnSkillEffect(this);			//½ºÅ³ ½Àµæ ÀÌÆåÆ®

					smCharInfo.bUpdateInfo[1] = lpTransPlayData->bUpdateInfo[1];
				}
			}


			lpTransPlayBuff++;
		}
		else {		//¼­ÀÖ´Â µ¿ÀÛ ¿¬¼Ó
			MakePlayBuffFromRecvData(&lpTransPlayBuff->PlayBuff, 0, 0);
			if (PlayBuffCnt == PlayBuffPosi_End) {

				if (DisplayFlag) {

					pX = lpTransPlayBuff->PlayBuff.x;
					pY = lpTransPlayBuff->PlayBuff.y;
					pZ = lpTransPlayBuff->PlayBuff.z;

					// ¿ÞÂÊ ÀåÂø ¾ÆÀÌÅÛ
					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_LEFT] != wStickItems[smPLAYBUFF_ITEM_LEFT]) {
						if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_LEFT] >= 0) {
							SetTool(sItem[lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_LEFT]].CODE, hvPOSI_LHAND);
							HvLeftHand.ColorBlink = 0;
							HvLeftHand.DispEffect = 0;
						}
						else
							SetTool(0, hvPOSI_LHAND);
					}

					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT] != wStickItems[smPLAYBUFF_ITEM_RIGHT]) {
						if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT] >= 0) {
							SetTool(sItem[lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT]].CODE, hvPOSI_RHAND);
							HvRightHand.ColorBlink = 0;
							HvRightHand.DispEffect = 0;
						}
						else
							SetTool(0, hvPOSI_RHAND);
					}

					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY] != wStickItems[smPLAYBUFF_ITEM_BODY])
					{

						SetChrArmor(this, lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY]);
						wStickItems[smPLAYBUFF_ITEM_BODY] = lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY];

					}
				}
			}

			lpTransPlayBuff++;
		}

		if (lpTransPlayData->bUpdateInfo[0] != smCharInfo.bUpdateInfo[0]) {

			if (!dwUpdateCharInfoTime || (dwUpdateCharInfoTime + 1000) < dwPlayTime) {
				Send_GetCharInfo(dwObjectSerial, ServerCode);

				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HUNGKY && !dwUpdateCharInfoTime) {
					//ÀÌÆåÆ® ( ÇæÅ© º¯½Å )
					StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
					SetDynLight(pX, pY, pZ, 50, 100, 70, 0, 300, 1);

					//º¯½Å ¼Ò¸®
					if (MotionInfo) {
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						CharPlaySound(this);
						MotionInfo->State = cnt;
					}
				}
				// pluto ¾ðµ¥µå Å·È£ÇÇ º¯½Å
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEADHOPT && !dwUpdateCharInfoTime)
				{
					StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
					SetDynLight(pX, pY, pZ, 50, 100, 70, 0, 300, 1);

					if (MotionInfo)
					{
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						CharPlaySound(this);
						MotionInfo->State = cnt;
					}
				}

				// ¹ÚÀç¿ø - º¹³¯ ÀÌº¥Æ®
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEBECHICK && !dwUpdateCharInfoTime)
				{
					StartEffect(pX, pY + 48 * fONE, pZ, EFFECT_RETURN1);
					SetDynLight(pX, pY, pZ, 50, 100, 70, 0, 300, 1);

					if (MotionInfo)
					{
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						CharPlaySound(this);
						MotionInfo->State = cnt;
					}
				}


				dwUpdateCharInfoTime = dwPlayTime;
			}
		}

		if (lpCurPlayer->OnStageField >= 0 &&
			(
				(StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_CASTLE && CastleBattleZone_LineZ > lpCurPlayer->pZ && CastleBattleZone_LineZ > pZ) ||
				(StageField[lpCurPlayer->OnStageField]->FieldCode == PK_FieldState.FieldCode && PK_FieldState.dwPlay_FieldTime > dwPlayTime && !PartyFlag)
				)) {


			if (PkMode_CharState == 0 && smCharInfo.State == smCHAR_STATE_USER &&
				(smCharInfo.ClassClan == 0 || smCharInfo.ClassClan != lpCurPlayer->smCharInfo.ClassClan))
			{

				if (StageField[lpCurPlayer->OnStageField]->FieldCode != PK_FieldState.FieldCode || !PartyFlag) {
					PkMode_CharState = smCharInfo.State;
					smCharInfo.State = smCHAR_STATE_ENEMY;
				}
			}
		}

		// Modo PVP
		else if (lpCurPlayer->OnStageField >= 0 &&
			PlayerPvPMode == 1 &&
			!PartyFlag &&
			PlayerPvPMode == lpCurPlayer->PlayerPvPMode &&
			StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_CASTLE &&
			StageField[lpCurPlayer->OnStageField]->State != FIELD_STATE_VILLAGE &&
			StageField[lpCurPlayer->OnStageField]->FieldCode != 23)

		{
			if (PkMode_CharState == 0 && smCharInfo.State == smCHAR_STATE_USER)
			{
				PkMode_CharState = smCharInfo.State;
				smCharInfo.State = smCHAR_STATE_ENEMY;
			}
		}
		else
		{
			if (PkMode_CharState)
			{
				smCharInfo.State = PkMode_CharState;
				PkMode_CharState = 0;
			}
		}

		int ChkIceFlag = 0;
		int ChkPoisonFlag = 0;
		DWORD	TradeMsgCode = 0;

		PlayVanish = 0;
		TradeMsgCode = dwTradeMsgCode;
		dwTradeMsgCode = 0;
		PlayCurseQuest = 0;
		PlayCursePvP = 0; // rei pvp xxstr
		PlayCurseTopLVL = 0; // top lvl xxstr
		PlayEvasionMastery = 0;

		if (lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT]) {
			lpBuff = (char*)lpTransPlayBuff;

			for (cnt = 0; cnt < lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT]; cnt++) {

				Code = ((int*)lpBuff)[1];
				Size = ((int*)lpBuff)[0];

				switch (Code) {
				case smTRANSCODE_EFFECT_ITEM:
					//Âø¿ë ¹«±â ÀÌÆåÆ® Àû¿ë
					SetTransEffectItems((smEFFECT_ITEM*)lpBuff);
					break;

				case smTRANSCODE_EFFECT_WEAPON:
					//¼ö½ÅµÈ µ¥ÀÌÅ¸ÀÇ ÀåÂø ¹«±â¿¡ ¼Ó¼º ÀÌÆåÆ® Àû¿ë 
					SetTransEffectWeapon((smEFFECT_ITEM*)lpBuff);
					break;

				case smTRANSCODE_EFFECT_STUN:
					//Ä³¸¯ÅÍ°¡ ½ºÅÏ »óÅÂ
					if (smCharInfo.Life[0] > 0)
						AssaParticle_Stune(this, 4);//(((smEFFECT_ITEM *)lpBuff)->dwItemCode>>4)+4 );
					break;

				case smTRANSCODE_EFFECT_CURSE:
					//½áÅ¥¹ö½º ÀúÁÖ ( HolyPower Àû¿ëÁß )
					if (smCharInfo.Life[0] > 0)
						StartEffect_Serqbus_Skill2(this, 4);
					break;

				case smTRANSCODE_EFFECT_ICE:
					//²Ç²Ç ¾ó¾úÀ½ 
					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink) {
						PlayDistortion = 120;//((smEFFECT_ITEM *)lpBuff)->ColorBlink*4;
					}
					else {
						PlayDistortion = 0;	//µð½ºÅä¼ÇÈÄ ¾óÀ½°ø°ÝÀÏ¶§ µð½ºÅä¼Ç È¿°ú À¯Áö ¹ö±× ¼öÁ¤( 2005³â 7¿ù14ÀÏ ) 
						if (AttackIce <= 0) AttackIce = 1;
						ChkIceFlag = TRUE;
					}
					break;

				case smTRANSCODE_EFFECT_POISION:
					if (DispPoison <= 0) DispPoison = 1;
					ChkPoisonFlag = TRUE;
					break;
				case smTRANSCODE_EFFECT_CURSE_QUEST:
					PlayCurseQuest = 1;
					break;
					// DESATIVADO: EFEITO REI PVP
				case smTRANSCODE_EFFECT_CURSE_PVP:
					PlayCursePvP = 1;
					break;
					// DESATIVADO: EFEITO TOP LVL
				case smTRANSCODE_EFFECT_CURSE_TOPLVLSRV:
					PlayCurseTopLVL = 1;
					break;
				case smTRANSCODE_EFFECT_VANISH:
					PlayVanish = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					PlayVague = ((smEFFECT_ITEM*)lpBuff)->ColorBlink;
					break;

				case smTRANSCODE_EFFECT_KEEPSKILL:
					//½ºÅ³ ÀÌÆåÆ® ÇÃ·¢ Á¤º¸
					switch (((smEFFECT_ITEM*)lpBuff)->dwItemCode) {
					case SKILL_PLAY_FORCE_OF_NATURE:
						SkillSagittarionForceOfNature2(this, 5.0f);		//Æ÷½º¿Àºê³×ÀÌÃÄ ÀÌÆåÆ®
						break;
					case SKILL_PLAY_HALL_OF_VALHALLA:
						SkillValhallaHallOfValhallaHandEffect(this, 5.0f);	//È¦ ¿Àºê ¹ßÇÒ¶ó ÀÌÆåÆ®
						break;
					case SKILL_PLAY_EVASION_MASTERY:
						PlayEvasionMastery = 1;	//È¦ ¿Àºê ¹ßÇÒ¶ó ÀÌÆåÆ®
						break;
					case SKILL_PLAY_BLESS_SIEGE_ITEM:
						PlayInvincible = 70 * 5;					//¹«Àû ¾ÆÀÌÅÛ
						break;
					}
					break;

				case smTRANSCODE_EFFECT_PERSHOP:		//°³ÀÎ»óÁ¡ ¿ÀÇÂ
					dwTradeMsgCode = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;

					if (TradeMsgCode != dwTradeMsgCode) {
						/*
						if ( szTradeMessage[0] )
							dwMsgCode = GetSpeedSum( szTradeMessage );
						else
							dwMsgCode = 0;
						*/

						//ÄÚµå º¯°æ
						wsprintf(szTradeMessage, "%s :", smCharInfo.szName);
						//Áö¿ª¼­¹ö¿¡ ¸í·É ÄÚµå Àü¼Û
						SendCommand_AreaServer(smTRANSCODE_MYSHOP_MESSAGE, 0, dwObjectSerial, dwTradeMsgCode, 0);
					}
					break;

				case smTRANSCODE_EFFECT_LOWPET:		//Àú·¹º§ µµ¿ò¹Ì Æê

					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink)
						lpChar = FindChrPlayer(((smEFFECT_ITEM*)lpBuff)->ColorBlink);
					else
						lpChar = 0;

					switch (((smEFFECT_ITEM*)lpBuff)->dwItemCode) {
					case 1:
						g_LowLevelAddIdlePet(this, PET_TERRY, lpChar);
						break;
					case 2:
						g_LowLevelAddIdlePet(this, PET_NEPSYS, lpChar);
						break;
					case 3:
						g_LowLevelAddIdlePet(this, PET_IO, lpChar);
						break;
					case 4:
						g_LowLevelAddIdlePet(this, PET_MUTE, lpChar);
						break;

					}
					break;

				case smTRANSCODE_EFFECT_PCBANGPET:		//PC¹æ Æê

					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink)
						lpChar = FindChrPlayer(((smEFFECT_ITEM*)lpBuff)->ColorBlink);
					else
						lpChar = 0;

					switch (((smEFFECT_ITEM*)lpBuff)->dwItemCode) {
					case 1:
						g_HoPCBangAddIdlePet(this, PCBANG_PET_BURSTP, lpChar);
						break;
					case 2:
						g_HoPCBangAddIdlePet(this, PCBANG_PET_BURSTS, lpChar);
						break;
					case 3:
						g_HoPCBangAddIdlePet(this, PCBANG_PET_BURSTL, lpChar);
						break;
					case 4:
						g_HoPCBangAddIdlePet(this, PCBANG_PET_BURSTD, lpChar);
						break;

					}
					break;

				case smTRANSCODE_EFFECT_CLANMGR:		//¼øÀ§±Ç Å¬·£ Ç¥½Ã
					dwClanManageBit = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					break;

				case smTRANSCODE_EFFECT_FORCEORB:		//Æ÷½º ¿Àºê ½ÃÀüÁß
					dwForceOrbCode = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					dwForceOrbTime = ((smEFFECT_ITEM*)lpBuff)->ColorBlink;
					dwForceOrbTime += dwPlayTime;

					AssaParticle_ShelltomWeapon(this, 70 * 5, ((dwForceOrbCode & 0xFFFF) >> 8) - 1);
					break;
				case smTRANSCODE_EFFECT_PVP_ON:
					PlayerPvPMode = 1;
					break;
				case smTRANSCODE_EFFECT_PVP_OFF:
					PlayerPvPMode = 0;
					break;
				case smTRANSCODE_EFFECT_BLESS_SCORE:
					//ºí·¡½º Ä³½½ µæÁ¡ Á¤º¸ ÀÔ¼ö
					sBlessCastle_Damage[0] = ((smEFFECT_ITEM*)lpBuff)->sColors[0];
					sBlessCastle_Damage[1] = ((smEFFECT_ITEM*)lpBuff)->sColors[1];
					break;
				}
				lpBuff += Size;
			}
		}
		if (!ChkIceFlag && AttackIce > 0) AttackIce = -AttackIce;
		if (!ChkPoisonFlag && DispPoison > 0) DispPoison = -DispPoison;

	}


	TransSendWait = 0;
	dwLastTransTime = dwPlayTime;

	return TRUE;
}

//µ¥ÀÌÅ¸¸¦ ¹ÞÀ½
int smCHAR::RecvPlayData(smTHREADSOCK* pData)
{
	int act;
	smPLAYDATA PlayData;
	smPLAYDATA2 PlayData2;
	smPLAYDATA3 PlayData3;
	smTRNAS_PLAYERINFO* lpTransPlayerInfo;
	int RecvCode;
	int cnt;
	int len;
	int LastBuffPosi_End;
	//smCHAR	*lpChar;

	RecvCode = ((int*)pData->Buff)[1];

	switch (RecvCode) {
	case smTRANSCODE_PLAYERINFO:
		//»ó´ë ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸ ¼ö½Å ¹ÞÀ½
		lpTransPlayerInfo = (smTRNAS_PLAYERINFO*)pData->Buff;
		SetTransPlayerInfo(lpTransPlayerInfo);
		return TRUE;

	case smTRANSCODE_PLAYERINFO_Q:
		//»ó´ë ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸ ¼ö½Å ¹ÞÀ½ ( ÆÐÅ¶¾ç ÀûÀº µ¥ÀÌÅ¸ )
		SetTransPlayerInfoQuick((smTRNAS_PLAYERINFO_QUICK*)pData->Buff);
		break;

	case smTRANSCODE_PLAYDATA1:
		//°ÔÀÓ ÁøÇà ¼ö½Å
		memcpy(&PlayData, pData->Buff, sizeof(smPLAYDATA));

		pX = PlayData.x;
		pY = PlayData.y;
		pZ = PlayData.z;

		Angle.x = PlayData.angle[0] & ANGCLIP;
		Angle.y = PlayData.angle[1] & ANGCLIP;
		Angle.z = PlayData.angle[2] & ANGCLIP;
		act = PlayData.angle[3];

		if (act != action)//|| ( act!=CHRMOTION_WALK && act!=CHRMOTION_STAND ) )
			frame = PlayData.frame;

		action = act;

		PlayBuffCnt = PlayBuffPosi_End;
		MoveCnt = 0;

		dwTarget = 0;
		break;

	case smTRANSCODE_PLAYDATA2:
		//°ÔÀÓ ÁøÇà ¼ö½Å
		LastBuffPosi_End = PlayBuffPosi_End;
		memcpy(&PlayData2, pData->Buff, sizeof(smPLAYDATA2));
		len = PlayData2.BuffCount[1] - PlayData2.BuffCount[0];
		MakePlayBuffFromRecvData(&PlayData2.PlayBuff[0], &PlayData2.PlayBuff[1], len);
		if ((PlayBuffPosi_End - PlayBuffCnt) > 32) PlayBuffCnt = LastBuffPosi_End;

		dwTarget = PlayData2.dwTarget;
		break;

	case smTRANSCODE_PLAYDATA3:
		//°ÔÀÓ ÁøÇà ¼ö½Å
		LastBuffPosi_End = PlayBuffPosi_End;
		memcpy(&PlayData3, pData->Buff, sizeof(smPLAYDATA3));
		len = PlayData3.BuffCount[2] - PlayData3.BuffCount[0];
		MakePlayBuffFromRecvData(&PlayData3.PlayBuff[0], &PlayData3.PlayBuff[2], len);
		len = PlayData3.BuffCount[1] - PlayData3.BuffCount[2];
		MakePlayBuffFromRecvData(&PlayData3.PlayBuff[2], &PlayData3.PlayBuff[1], len);
		if ((PlayBuffPosi_End - PlayBuffCnt) > 32) PlayBuffCnt = LastBuffPosi_End;

		dwTarget = PlayData3.dwTarget;
		break;

	case smTRANSCODE_TRANSPLAYDATA:
		//°ÔÀÓ ÁøÇà ¼ö½Å

		smTRANS_PLAYDATA* lpTransPlayData;
		smTRANS_PLAYBUFF* lpTransPlayBuff;
		smTRANS_PLAYBUFF* lpTransPlayBuff2;

		lpTransPlayData = (smTRANS_PLAYDATA*)pData->Buff;
		lpTransPlayBuff = (smTRANS_PLAYBUFF*)(pData->Buff + sizeof(smTRANS_PLAYDATA));

		LastBuffPosi_End = PlayBuffPosi_End;

		smCharInfo.Life[0] = lpTransPlayData->Hp[0];
		smCharInfo.Mana[0] = lpTransPlayData->Mp[0];
		bPartyLeader = lpTransPlayData->bPartyLeader;

		for (cnt = 0; cnt < lpTransPlayData->PlayBuffCnt - 1; cnt++) {
			lpTransPlayBuff2 = lpTransPlayBuff + 1;

			len = lpTransPlayBuff2->BuffCount - lpTransPlayBuff->BuffCount;
			MakePlayBuffFromRecvData(&lpTransPlayBuff->PlayBuff, &lpTransPlayBuff2->PlayBuff, len);

			lpTransPlayBuff++;
		}

		dwTarget = lpTransPlayData->dwTarget;
		break;

	case smTRANSCODE_ATTACKDATA2:
	case smTRANSCODE_ATTACKDATA:
		//»ó´ë¹æÇÑÅ× °ø°Ý ÇÔ
		AttackRecvCountUp();

		Record_TotalRecvDamageCount++;

		if (((TRANS_ATTACKDATA*)pData->Buff)->AttackState == 0x80) {
			memcpy(&AttackTrans, (TRANS_ATTACKDATA*)pData->Buff, sizeof(TRANS_ATTACKDATA));
			XorAttackTrans();
			if (PkMode_CharState || (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsCASTLE_FIELD))
				PlayAttack(TRUE);								//¹Ð¸° °ø°Ý Àû¿ë
			else
				PlayAttack();									//¹Ð¸° °ø°Ý Àû¿ë
			break;
		}

		if (AttackTrans.code ^ GetAttackTrans_XorCode()) {
			PlayAttack(TRUE);									//¹Ð¸° °ø°Ý Àû¿ë
		}

		memcpy(&AttackTrans, (TRANS_ATTACKDATA*)pData->Buff, sizeof(TRANS_ATTACKDATA));
		XorAttackTrans();

		/*
		#ifdef	_S2C_DAMAGE_PACKET
		#ifdef _LANGUAGE_CHINESE
				PlayAttack( TRUE );									//¹Ð¸° °ø°Ý Àû¿ë
		#endif
		#endif
		*/
		/*
		TRANS_ATTACKDATA	*lpTransAttackData;

		lpTransAttackData = (TRANS_ATTACKDATA *)pData->Buff;

		if ( lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DAMAGE && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DEAD && DisplayFlag ) {

			len = GetDistanceDbl( lpCurPlayer->pX , lpCurPlayer->pZ , pX, pZ );
			cnt = GetDistanceDbl( lpCurPlayer->pX , lpCurPlayer->pZ , lpTransAttackData->x, lpTransAttackData->z );

			if ( smCharInfo.dwCharSoundCode==snCHAR_SOUND_FIGON || smCharInfo.dwCharSoundCode==snCHAR_SOUND_WEB ) {
				//¿ø°Å¸® °ø°Ý
				len = 0;
				cnt = 0;
			}

			//ÁÖÀÎ°ø ¸ó½ºÅÍ ½ºÅ²ÀÏ °æ¿ì
			if ( lpCurPlayer->smCharInfo.State==smCHAR_STATE_ENEMY ) act = 1;
			else act = 0;

			if ( cnt<lpTransAttackData->AttackSize && lpCurPlayer->smCharInfo.Life[0]>0 && ( len<(128*128) || act ) && DisplayAlpha==0 ) {

				lpChar = 0;

				//°ø°ÝÇÑ »ó´ë¹æÀ» Ã£¾Æ ¹æÇâÀ» ¹Ù²Û´Ù
				for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
					if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial==lpTransAttackData->dwDestObjectSerial ) {
						lpChar = &chrOtherPlayer[cnt];
						break;
					}
				}

				//ºí·°À² °è»ê
				if ( lpChar && ( (rand()%100)>=sinGetBlockRating( lpChar->smCharInfo.Level ) || act ) ) {

					//¹æ¾îÀ² °è»ê
					//cnt = GetPlayDefence( &smCharInfo , &lpCurPlayer->smCharInfo );
					cnt = sinGetMonsterAccuracy( smCharInfo.Level , smCharInfo.Attack_Rating );
					//cnt = 100;
					if ( (rand()%100)<=cnt || act ) {

						//Èí¼öÀ² °è»ê
						cnt = (lpTransAttackData->Power - lpCurPlayer->smCharInfo.Absorption);

						if ( cnt<=0 ) cnt = 1;

							CheckCharForm();
							//¸íÁß ( °ø°Ý ¼º°ø )
							lpCurPlayer->smCharInfo.Life[0] -= cnt;

							//ÇÇÆ¢±â´Â ÆÄÆ¼Å¬
							StartEffect( lpCurPlayer->pX, lpCurPlayer->pY+16*fONE, lpCurPlayer->pZ, EFFECT_DAMAGE1 );
							SetDynLight( lpCurPlayer->pX, lpCurPlayer->pY+16*fONE, lpCurPlayer->pZ, 160, 0,0,0, 180 );

							if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_PHYSICAL_ABSORB) ) {
								//ÇÇÁöÅ¬¾÷¼Ù Àû¿ëÀÌÆåÆ®
								StartEffect( lpCurPlayer->pX, lpCurPlayer->pY+24*fONE , lpCurPlayer->pZ , EFFECT_PHYSICAL_ABSORB_DAMAGE );
							}

							if ( lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_EAT && lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_ATTACK &&
								lpCurPlayer->MotionInfo->State!=CHRMOTION_STATE_SKILL && cnt>1 ) {
								lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DAMAGE );
								CharPlaySound( lpCurPlayer );
							}

							//ÀÓ½Ã Ã³¸®
							if ( lpCurPlayer->smCharInfo.Life[0]<=0 ) {
								lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DEAD );
								lpCurPlayer->smCharInfo.Life[0] = 0;
								ReformCharForm();					//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ
								CharPlaySound( lpCurPlayer );
								DeadPlayerExp( lpCurPlayer );
								//°ÔÀÓÀ» ÀúÀå
								SaveGameData();
								break;
							}
							ReformCharForm();					//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ

					}
				}
				dwBattleTime = dwPlayTime;			//ÀüÅõ ½Ã°£ ±â·Ï
				//°ø°ÝÇÑ »ó´ë¹æÀ» Ã£¾Æ ¹æÇâÀ» ¹Ù²Û´Ù
				//lpCurPlayer->Angle.y = GetRadian2D( lpCurPlayer->pX , lpCurPlayer->pZ ,lpChar->pX, lpChar->pZ );
			}
		}
		*/
		break;
	}

	TransSendWait = 0;
	dwLastTransTime = dwPlayTime;

	return TRUE;
}

int smCHAR::SendPlayData(smCHAR* player)
{
	int SendTime;
	smPLAYDATA PlayData;
	smPLAYDATA2 PlayData2;
	smPLAYDATA3 PlayData3;
	int PlayerBuffCnt;
	int cnt;
	int StartPosi;
	int BuffPosi;
	int mAction;
	int SendMode;

	SendTime = PlayCounter - TransLastSendCnt;

	if (TransSendWait == 0 && SendTime >= TransMinCnt) 
	{
		if (lpCharMsTrace && lpCharMsTrace == this) {
			PlayData2.dwTarget = TRUE;
			PlayData3.dwTarget = TRUE;
		}
		else {
			PlayData2.dwTarget = FALSE;
			PlayData3.dwTarget = FALSE;
		}

		if (TransLevel > 1) 
		{
			PlayData.size = sizeof(smPLAYDATA);
			PlayData.code = smTRANSCODE_PLAYDATA1;
			PlayData.x = player->pX;
			PlayData.y = player->pY;
			PlayData.z = player->pZ;

			PlayData.angle[0] = player->Angle.x;
			PlayData.angle[1] = player->Angle.y;
			PlayData.angle[2] = player->Angle.z;
			PlayData.angle[3] = player->action;

			PlayData.frame = player->frame;

			if (TransSock->Send((char*)&PlayData, sizeof(smPLAYDATA))) 
			{
				TransSendWait = 1;
				TransLastSendCnt = PlayCounter;
				TransLastSendTime = SendTime;
				SetDistSendCnt(player);
			}
		}
		else
		{
			SendMode = 0;
			PlayerBuffCnt = player->PlayBuffCnt & PLAYBUFF_MASK;
			StartPosi = (PlayerBuffCnt - SendTime) & PLAYBUFF_MASK;

			mAction = player->PlayBuff[PlayerBuffCnt].angle[3];

			for (cnt = SendTime; cnt >= 0; cnt--) {
				BuffPosi = (StartPosi + cnt) & PLAYBUFF_MASK;
				if (player->PlayBuff[BuffPosi].angle[3] != mAction) {
					SendMode = 1;
					memcpy(&PlayData3.PlayBuff[2], &player->PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
					PlayData3.BuffCount[2] = cnt;
					break;
				}
			}

			if (!SendMode) {
				memcpy(&PlayData2.PlayBuff[0], &player->PlayBuff[StartPosi], sizeof(smPLAYBUFF));
				memcpy(&PlayData2.PlayBuff[1], &player->PlayBuff[PlayerBuffCnt], sizeof(smPLAYBUFF));
				PlayData2.code = smTRANSCODE_PLAYDATA2;
				PlayData2.size = sizeof(smPLAYDATA2);
				PlayData2.BuffCount[0] = 0;
				PlayData2.BuffCount[1] = SendTime;

				if (TransSock->Send((char*)&PlayData2, sizeof(smPLAYDATA2))) {
					TransSendWait = 1;
					TransLastSendCnt = PlayCounter;
					TransLastSendTime = SendTime;
					SetDistSendCnt(player);
				}
			}
			else {
				memcpy(&PlayData3.PlayBuff[0], &player->PlayBuff[StartPosi], sizeof(smPLAYBUFF));
				memcpy(&PlayData3.PlayBuff[1], &player->PlayBuff[PlayerBuffCnt], sizeof(smPLAYBUFF));
				PlayData3.code = smTRANSCODE_PLAYDATA3;
				PlayData3.size = sizeof(smPLAYDATA3);
				PlayData3.BuffCount[0] = 0;
				PlayData3.BuffCount[1] = SendTime;

				if (TransSock->Send((char*)&PlayData3, sizeof(smPLAYDATA3))) {
					TransSendWait = 1;
					TransLastSendCnt = PlayCounter;
					TransLastSendTime = SendTime;
					SetDistSendCnt(player);
				}
			}
		}
	}

	return TRUE;
}

int smCHAR::MakeTransPlayData(char* lpTargetBuff, int SendTime, int pBuffStep)
{
	smTRANS_PLAYDATA* lpTransPlayData;
	smTRANS_PLAYBUFF* lpTransPlayBuff;
	smEFFECT_ITEM* lpEffectItem;

	int StartPosi;
	int BuffPosi;
	int BuffPosi_Old;
	int mAction;
	int mFrameSelect;
	DWORD mFrame;
	int	Life;

	int	buffCnt;
	int cnt;
	int LastCnt;
	int	SameAction;
	int iSendMountID = 0;

	lpTransPlayData = (smTRANS_PLAYDATA*)lpTargetBuff;
	lpTransPlayBuff = (smTRANS_PLAYBUFF*)(lpTargetBuff + sizeof(smTRANS_PLAYDATA));

	lpTransPlayData->code = smTRANSCODE_TRANSPLAYDATA;
	lpTransPlayData->dwObjectSerial = dwObjectSerial;
	lpTransPlayData->dwTarget = 0;
	lpTransPlayData->StartPosi = PlayBuffCnt - SendTime;

	if (this == lpCurPlayer)
	{
		if (MotionInfo && MotionInfo->State == CHRMOTION_STATE_DEAD)
		{
			lpTransPlayData->Hp[0] = 0;
			lpTransPlayData->Mp[0] = 0;
		}
		else
		{
			lpTransPlayData->Hp[0] = sinGetLife();
			lpTransPlayData->Mp[0] = sinGetMana();
		}

		if (pMount)
			iSendMountID = pMount->IsVisible() ? pMount->GetMountID() : MOUNTID_Invisible;
	}
	else
	{
		lpTransPlayData->Hp[0] = smCharInfo.Life[0];
		lpTransPlayData->Mp[0] = smCharInfo.Mana[0];
	}

	lpTransPlayData->Hp[1] = smCharInfo.Life[1];
	lpTransPlayData->Mp[1] = smCharInfo.Mana[1];
	lpTransPlayData->bPartyLeader = bPartyLeader;

	if (UseObject_VirtualLife)
	{
		lpTransPlayData->Hp[0] = sObject_DisplayLife[0];
		lpTransPlayData->Hp[1] = sObject_DisplayLife[1];
	}

	if (smCharInfo.State == smCHAR_STATE_ENEMY)
		lpTransPlayData->dwAutoCharCode = smMonsterInfo.dwAutoCharCode;
	else
		lpTransPlayData->dwAutoCharCode = 0;

	smCharInfo.bUpdateInfo[1] = sinSkill.SkillPoint;

	lpTransPlayData->bUpdateInfo[0] = smCharInfo.bUpdateInfo[0];
	lpTransPlayData->bUpdateInfo[1] = smCharInfo.bUpdateInfo[1];
	lpTransPlayData->iMountID = iSendMountID;
	lpTransPlayData->bUpdateInfo[2] = 0;
	lpTransPlayData->bUpdateInfo[3] = 0;


	int	MainArea, SubArea;

	if (lpCurPlayer && lpCurPlayer->OnStageField >= 0)
	{
		MainArea = (lpCurPlayer->OnStageField) & 1;
		SubArea = (lpCurPlayer->OnStageField + 1) & 1;
	}
	else
	{
		MainArea = 0;
		SubArea = 0;
	}

	if (StageField[0])
	{
		lpTransPlayData->wArea[0] = StageField[MainArea]->FieldCode;

		if (CMINIMAPHANDLE->lastMapID != lpTransPlayData->wArea[0])
		{
			CMINIMAPHANDLE->lastMapID = lpTransPlayData->wArea[0];
			CMINIMAPHANDLE->OnChangeMap(lpTransPlayData->wArea[0], lpTransPlayData->wArea[1]);
		}
	}
	else
		lpTransPlayData->wArea[0] = -1;

	if (StageField[1])
		lpTransPlayData->wArea[1] = StageField[SubArea]->FieldCode;
	else
		lpTransPlayData->wArea[1] = -1;

	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL] = smCharInfo.Level;
	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_POTION] = (PotionLog & 0x0F) | ((AttackCritcal & 0xF) << 4);
	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] = 0;
	lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL] = (BYTE)dwDispAppSkill;


	StartPosi = (lpTransPlayData->StartPosi) & PLAYBUFF_MASK;

	buffCnt = 0;

	mAction = PlayBuff[StartPosi].angle[3];
	mFrame = PlayBuff[StartPosi].frame;
	memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[StartPosi], sizeof(smPLAYBUFF));
	lpTransPlayBuff->BuffCount = 0;
	lpTransPlayBuff++;
	buffCnt++;
	LastCnt = 0;
	BuffPosi_Old = StartPosi;
	mFrameSelect = PlayBuff[StartPosi].frame >> CHAR_FRAME_SELSHIFT;

	SameAction = TRUE;

	for (cnt = 1; cnt < SendTime - 1; cnt++) {
		BuffPosi = (StartPosi + cnt) & PLAYBUFF_MASK;

		if (PlayBuff[BuffPosi].angle[3] != mAction) SameAction = FALSE;

		if (PlayBuff[BuffPosi].frame < mFrame || PlayBuff[BuffPosi].angle[3] != mAction || mFrameSelect != (int)(PlayBuff[BuffPosi].frame >> CHAR_FRAME_SELSHIFT)) {

			mAction = PlayBuff[BuffPosi_Old].angle[3];
			memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi_Old], sizeof(smPLAYBUFF));
			lpTransPlayBuff->BuffCount = (cnt * pBuffStep) - 1;
			lpTransPlayBuff++;
			buffCnt++;

			mAction = PlayBuff[BuffPosi].angle[3];
			mFrame = PlayBuff[BuffPosi].frame;
			memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
			lpTransPlayBuff->BuffCount = cnt * pBuffStep;
			lpTransPlayBuff++;
			buffCnt++;
			LastCnt = cnt;
			mFrameSelect = PlayBuff[BuffPosi].frame >> CHAR_FRAME_SELSHIFT;
		}

		BuffPosi_Old = BuffPosi;
	}

	BuffPosi = (StartPosi + SendTime - 1) & PLAYBUFF_MASK;

	mAction = PlayBuff[BuffPosi].angle[3];
	memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
	lpTransPlayBuff->BuffCount = (SendTime * pBuffStep) - 1;
	lpTransPlayBuff++;
	buffCnt++;

	lpTransPlayData->StartPosi = (lpTransPlayData->StartPosi * pBuffStep) & PLAYBUFF_MASK;

	if (SameAction == TRUE && smCharInfo.State == smCHAR_STATE_USER && MotionInfo->State == CHRMOTION_STATE_STAND)
		buffCnt = 1;

	lpTransPlayData->PlayBuffCnt = buffCnt;
	lpTransPlayData->size = sizeof(smTRANS_PLAYDATA) + (sizeof(smTRANS_PLAYBUFF) * buffCnt);

	MotionRecordCount++;


	lpEffectItem = (smEFFECT_ITEM*)(lpTargetBuff + lpTransPlayData->size);

	if (HvLeftHand.PatTool && HvLeftHand.dwItemCode)
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_ITEM;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = HvLeftHand.dwItemCode;
		lpEffectItem->ColorBlink = HvLeftHand.ColorBlink;
		memcpy(lpEffectItem->sColors, HvLeftHand.sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = HvLeftHand.DispEffect;
		lpEffectItem->BlinkScale = HvLeftHand.BlinkScale;
		lpEffectItem->iAgeLevel = HvLeftHand.iAgeLevel;
		lpEffectItem->iMixID = HvLeftHand.iMixID;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (HvRightHand.PatTool && HvRightHand.dwItemCode)
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_ITEM;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = HvRightHand.dwItemCode;
		lpEffectItem->ColorBlink = HvRightHand.ColorBlink;
		memcpy(lpEffectItem->sColors, HvRightHand.sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = HvRightHand.DispEffect;
		lpEffectItem->BlinkScale = HvRightHand.BlinkScale;
		lpEffectItem->iAgeLevel = HvRightHand.iAgeLevel;
		lpEffectItem->iMixID = HvRightHand.iMixID;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwWeaponEffectTime) {

		lpEffectItem->code = smTRANSCODE_EFFECT_WEAPON;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = WeaponEffect;
		lpEffectItem->ColorBlink = (dwWeaponEffectTime - dwPlayTime) + 500;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = EnchantEffect_Point;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (this == lpCurPlayer)
		Life = sinGetLife();
	else
		Life = smCharInfo.Life[0];

	if (PlayStunCount && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_STUN;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayStunCount;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayHolyPower[1] && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_CURSE;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayHolyPower[1];
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlaySlowCount > 0 && Life > 0) 
	{
		if (smCharInfo.State != smCHAR_STATE_USER || AttackIce || PlayDistortion) {
			lpEffectItem->code = smTRANSCODE_EFFECT_ICE;
			lpEffectItem->size = sizeof(smEFFECT_ITEM);
			lpEffectItem->dwItemCode = PlaySlowCount;
			lpEffectItem->ColorBlink = PlayDistortion;
			ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
			lpEffectItem->DispEffect = 0;
			lpEffectItem->BlinkScale = 0;

			lpTransPlayData->size += lpEffectItem->size;
			lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
			lpEffectItem++;
		}
	}

	if (PlayPoison[1] && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_POISION;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayPoison[0];
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayCurseQuest && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_CURSE_QUEST;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayCurseQuest;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	extern BOOL bReiPVP;
	// Efeito Rei PVP
	if (PlayCursePvP && Life > 0 && bReiPVP)
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_CURSE_PVP;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayCursePvP;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	extern BOOL bTopLVL;
	// Efeito Top Level
	if (PlayCurseTopLVL && Life > 0 && bTopLVL)
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_CURSE_TOPLVLSRV;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayCurseTopLVL;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}


	if (PlayVanish > 0 && Life > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_VANISH;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = PlayVanish;
		lpEffectItem->ColorBlink = PlayVague;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwForceOfNatureTime > dwPlayTime) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_FORCE_OF_NATURE;
		lpEffectItem->ColorBlink = dwForceOfNatureTime - dwPlayTime;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwHallOfValhallaTime > dwPlayTime) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_HALL_OF_VALHALLA;
		lpEffectItem->ColorBlink = dwHallOfValhallaTime - dwPlayTime;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayInvincible > 0) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_BLESS_SIEGE_ITEM;
		lpEffectItem->ColorBlink = PlayInvincible;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (PlayEvasionMastery) {
		lpEffectItem->code = smTRANSCODE_EFFECT_KEEPSKILL;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = SKILL_PLAY_EVASION_MASTERY;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwTradeMsgCode) {
		lpEffectItem->code = smTRANSCODE_EFFECT_PERSHOP;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = dwTradeMsgCode;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	extern BOOL bPlayerPvP;

	if (PlayerPvPMode && bPlayerPvP)
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_PVP_ON;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = 0;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;
		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}
	if (!PlayerPvPMode && !bPlayerPvP)
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_PVP_OFF;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = 0;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;
		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwForceOrbTime && dwForceOrbTime > dwPlayTime) {
		lpEffectItem->code = smTRANSCODE_EFFECT_FORCEORB;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = dwForceOrbCode;
		lpEffectItem->ColorBlink = dwPlayTime - dwForceOrbTime;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (cHelpPet.PetShow) 
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_LOWPET;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = cHelpPet.PetKind;
		lpEffectItem->ColorBlink = g_LowLevelPetAttackSerial;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;

		if (g_LowLevelPetAttackSerial) g_LowLevelPetAttackSerial = 0;
	}

	if (cPCBANGPet.PetShow) 
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_PCBANGPET;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = cPCBANGPet.PetKind;
		lpEffectItem->ColorBlink = g_HoPCBangPetAttackSerial;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;

		if (g_HoPCBangPetAttackSerial) g_HoPCBangPetAttackSerial = 0;
	}

	if (dwClanManageBit) 
	{
		lpEffectItem->code = smTRANSCODE_EFFECT_CLANMGR;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = dwClanManageBit;
		lpEffectItem->ColorBlink = 0;
		ZeroMemory(lpEffectItem->sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = 0;
		lpEffectItem->BlinkScale = 0;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (OnStageField >= 0 && StageField[OnStageField] && StageField[OnStageField]->State == FIELD_STATE_CASTLE)
	{
		if (sBlessCastle_Damage[0] || sBlessCastle_Damage[1]) {
			lpEffectItem->code = smTRANSCODE_EFFECT_BLESS_SCORE;
			lpEffectItem->size = sizeof(smEFFECT_ITEM);
			lpEffectItem->dwItemCode = 0;
			lpEffectItem->ColorBlink = 0;
			lpEffectItem->sColors[0] = sBlessCastle_Damage[0];
			lpEffectItem->sColors[1] = sBlessCastle_Damage[1];
			lpEffectItem->sColors[2] = 0;
			lpEffectItem->sColors[3] = 0;
			lpEffectItem->DispEffect = 0;
			lpEffectItem->BlinkScale = 0;

			lpTransPlayData->size += lpEffectItem->size;
			lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
			lpEffectItem++;
		}
	}

	srTransBuffSize = lpTransPlayData->size;

	return TRUE;
}


int smCHAR::SetTransEffectItems(smEFFECT_ITEM* lpEffectItem)
{
	if (HvRightHand.PatTool && HvRightHand.dwItemCode == lpEffectItem->dwItemCode) {
		HvRightHand.ColorBlink = lpEffectItem->ColorBlink;
		memcpy(HvRightHand.sColors, lpEffectItem->sColors, sizeof(short) * 4);
		HvRightHand.DispEffect = lpEffectItem->DispEffect;
		HvRightHand.BlinkScale = lpEffectItem->BlinkScale;
		HvRightHand.blendingMaterial = nullptr;


		if (HvRightHand.iAgeLevel > 0)
		{
			int iMatID = HvRightHand.iAgeLevel - 4;

			if (iMatID >= 0 && iMatID < _countof(itemAgingMaterial))
				HvRightHand.blendingMaterial = itemAgingMaterial[HvRightHand.iAgeLevel - 4];
		}
		else if (HvRightHand.iMixID > 0)
		{
			int iMatID = HvRightHand.iMixID - 1;

			if (iMatID >= 0 && iMatID < _countof(itemMixingMaterial))
				HvRightHand.blendingMaterial = itemMixingMaterial[iMatID];
		}

		GetItemKindFromBliankColor(&HvRightHand);
		if (HvRightHand.DispEffect) AssaParticle_ClassUpWeapon(this);

	}

	if (HvLeftHand.PatTool && HvLeftHand.dwItemCode == lpEffectItem->dwItemCode) {
		HvLeftHand.ColorBlink = lpEffectItem->ColorBlink;
		memcpy(HvLeftHand.sColors, lpEffectItem->sColors, sizeof(short) * 4);
		HvLeftHand.DispEffect = lpEffectItem->DispEffect;
		HvLeftHand.BlinkScale = lpEffectItem->BlinkScale;
		HvLeftHand.blendingMaterial = nullptr;

		if (HvLeftHand.iAgeLevel > 0)
		{
			int iMatID = HvLeftHand.iAgeLevel - 4;

			if (iMatID >= 0 && iMatID < _countof(itemAgingMaterial))
				HvLeftHand.blendingMaterial = itemAgingMaterial[HvLeftHand.iAgeLevel - 4];
		}
		else if (HvLeftHand.iMixID > 0)
		{
			int iMatID = HvLeftHand.iMixID - 1;

			if (iMatID >= 0 && iMatID < _countof(itemMixingMaterial))
				HvLeftHand.blendingMaterial = itemMixingMaterial[iMatID];
		}


		GetItemKindFromBliankColor(&HvLeftHand);
		if (HvLeftHand.DispEffect) AssaParticle_ClassUpWeapon(this);
	}

	return TRUE;
}

int smCHAR::SetTransEffectWeapon(smEFFECT_ITEM* lpEffectItem)
{
	int	time;

	if (dwWeaponEffectTime && WeaponEffect == lpEffectItem->dwItemCode) {
		time = abs((long)dwWeaponEffectTime - (long)dwPlayTime);
		if (abs(time - lpEffectItem->ColorBlink) > 6000) {
			switch (lpEffectItem->dwItemCode) {
			case (sITEMINFO_FIRE + 1):
				AssaParticle_EnchantWeaponFireMember(this, lpEffectItem->ColorBlink / 1000);
				break;
			case (sITEMINFO_ICE + 1):
				AssaParticle_EnchantWeaponIceMember(this, lpEffectItem->ColorBlink / 1000);
				break;
			case (sITEMINFO_LIGHTING + 1):
				AssaParticle_EnchantWeaponLightMember(this, lpEffectItem->ColorBlink / 1000);
				break;
			}
			dwWeaponEffectTime = dwPlayTime + lpEffectItem->ColorBlink;
			EnchantEffect_Point = lpEffectItem->DispEffect;
		}
	}
	else {
		switch (lpEffectItem->dwItemCode) {
		case (sITEMINFO_FIRE + 1):
			AssaParticle_EnchantWeaponFireMember(this, lpEffectItem->ColorBlink / 1000);
			break;
		case (sITEMINFO_ICE + 1):
			AssaParticle_EnchantWeaponIceMember(this, lpEffectItem->ColorBlink / 1000);
			break;
		case (sITEMINFO_LIGHTING + 1):
			AssaParticle_EnchantWeaponLightMember(this, lpEffectItem->ColorBlink / 1000);
			break;
		}
		WeaponEffect = lpEffectItem->dwItemCode;
		dwWeaponEffectTime = dwPlayTime + lpEffectItem->ColorBlink;
		EnchantEffect_Point = lpEffectItem->DispEffect;
	}

	return TRUE;
}

int smCHAR::SendPlayDataSock(smWINSOCK* lpsmsock, char* lpTransBuff, int ex, int ey, int ez)
{
	int SendTime;
	smPLAYDATA PlayData;
	int dist;
	int x, y, z;

	if ((PlayBuffCnt & 0x3F) != 0) return FALSE;

	SendTime = 0x40;

	x = (pX - ex) >> FLOATNS;
	y = (pY - ey) >> FLOATNS;
	z = (pZ - ez) >> FLOATNS;

	dist = x * x + y * y + z * z;

	TransLevel = 3;

	if (dist < DIST_TRANSLEVEL_HIGH || !DisplayFlag) {
		TransLevel = 0;
	}
	else {
		if (dist < DIST_TRANSLEVEL_MID) {
			TransLevel = 1;
		}
		else {
			if (dist < DIST_TRANSLEVEL_LOW) {
				TransLevel = 2;
			}
		}
	}

	if (TransLevel > 1)
	{
		if ((PlayBuffCnt & 0xFF) != 0) return FALSE;

		PlayData.size = sizeof(smPLAYDATA);
		PlayData.code = smTRANSCODE_PLAYDATA1;
		PlayData.x = pX;
		PlayData.y = pY;
		PlayData.z = pZ;

		PlayData.angle[0] = Angle.x;
		PlayData.angle[1] = Angle.y;
		PlayData.angle[2] = Angle.z;
		PlayData.angle[3] = action;

		PlayData.frame = frame;
		PlayData.dwObjectSerial = dwObjectSerial;

		lpsmsock->Send((char*)&PlayData, sizeof(smPLAYDATA), TRUE);
	}
	else
	{
		lpsmsock->Send(lpTransBuff, ((smTRANS_PLAYDATA*)lpTransBuff)->size);
	}

	return TRUE;
}

int smCHAR::SendCharInfo(smWINSOCK* lpsmsock)
{
	smTRNAS_PLAYERINFO	TransPlayerInfo;

	memcpy(&TransPlayerInfo.smCharInfo, &smCharInfo, sizeof(smCHAR_INFO));
	TransPlayerInfo.size = sizeof(smTRNAS_PLAYERINFO);
	TransPlayerInfo.code = smTRANSCODE_PLAYERINFO;
	TransPlayerInfo.dwObjectSerial = dwObjectSerial;

	TransPlayerInfo.x = pX;
	TransPlayerInfo.y = pY;
	TransPlayerInfo.z = pZ;

	return lpsmsock->Send((char*)&TransPlayerInfo, TransPlayerInfo.size, TRUE);
}

int smCHAR::BeginSkill(int SkilCode, int Level, smCHAR* lpTarChar, int x, int y, int z)
{
	int point = 0;
	int param = 0;
	POINT3D pos = { 0 };
	int cnt = 0;
	POINT3D	Pos1 = { 0 }, Pos2 = { 0 };

	if (smCharInfo.State == smCHAR_STATE_ENEMY && PkMode_CharState != smCHAR_STATE_USER) {
		return BeginSkill_Monster();
	}

	AttackEffect = 0;
	point = (SkilCode >> 8) & 0xF;
	param = (SkilCode >> 12) & 0xF;

	switch (SkilCode & 0xFF)
	{

	case SKILL_PLAY_EXTREME_SHIELD:
		SkillPlaySound(SKILL_SOUND_EXTREME_SHIELD, pX, pY, pZ);
		break;

	case SKILL_PLAY_MECHANIC_BOMB:
		// Esconde a arma na animação da skill do mecânico
		HideWeapon = TRUE;
		break;

	case SKILL_PLAY_PHYSICAL_ABSORB:
		dwDispAppSkill |= SKILL_APPMASK_PHYSICAL_ABSORB;
		StartSkill(pX, pY, pZ, 0, Angle.y, 0, SKILL_PHYSICAL_ABSORB);
		StartSkill(pX, pY, pZ, 0, Angle.y, 0, this, SKILL_SHIELD1, point);
		SkillPlaySound(SKILL_SOUND_PHYSICAL_ABSORB1, pX, pY, pZ);
		break;

	case SKILL_PLAY_POISON_ATTRIBUTE:
		StartSkill(pX, pY, pZ, 0, Angle.y, 0, SKILL_UP1);
		SetMotionFromCode(CHRMOTION_STATE_ATTACK);
		break;


	case SKILL_PLAY_RAVING:
	case SKILL_PLAY_IMPACT:
		AttackEffect = TRUE;
		break;

	case SKILL_PLAY_PIKEWIND:
		break;

	case SKILL_PLAY_JUMPING_CRASH:
		AttackEffect = TRUE;
		SkillPlaySound(SKILL_SOUND_JUMPING_CRASH1, pX, pY, pZ);
		break;

	case SKILL_PLAY_SCOUT_HAWK:
		if (HoSkillCode) {
			HoSkillCode = 0;
			EffectMgr.Main();
		}

		StartSkill(pX, pY, pZ, 0, Angle.y, 0, this, SKILL_SCOUT_HAWK, point);
		SkillPlaySound(SKILL_SOUND_SCOUT_HAWK1, pX, pY, pZ);
		SkillPlaySound(SKILL_SOUND_SCOUT_HAWK2, pX, pY, pZ);
		HideWeapon = TRUE;
		break;

	case SKILL_PLAY_WIND_ARROW:
		break;

	case SKILL_PLAY_PERFECT_AIM:
		break;

	case SKILL_PLAY_MAXIMIZE:
		dwDispAppSkill |= SKILL_APPMASK_AUTOMATION;
		StartSkill(pX, pY, pZ, Angle.x, Angle.y, Angle.z, SKILL_MAXIMIZE);
		StartSkill(pX, pY, pZ, 0, Angle.y, 0, this, SKILL_SHIELD2, point);
		SkillPlaySound(SKILL_SOUND_SKILL_MAXIMIZE1, pX, pY, pZ);
		break;

	case SKILL_PLAY_AUTOMATION:
		dwDispAppSkill |= SKILL_APPMASK_AUTOMATION;
		StartSkill(pX, pY, pZ, Angle.x, Angle.y, Angle.z, SKILL_AUTOMATION);
		StartSkill(pX, pY, pZ, 0, Angle.y, 0, this, SKILL_SHIELD2, point);
		SkillPlaySound(SKILL_SOUND_SKILL_AUTOMATION1, pX, pY, pZ);
		break;

	case SKILL_PLAY_SPARK:
		HideWeapon = TRUE;
		SkillPlaySound(SKILL_SOUND_SKILL_SPARK1, pX, pY, pZ);
		break;


	case SKILL_PLAY_METAL_ARMOR:
		AssaParticle_MetalArmor(this, Metal_Armor_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_METAL_ARMOR, pX, pY, pZ);
		break;

	case SKILL_PLAY_SPARK_SHIELD:
		SkillPlaySound(SKILL_SOUND_SKILL_SPARK1, pX, pY, pZ);
		break;

		//	case SKILL_PLAY_GROUND_PIKE:
		//		break;

		/*
			case SKILL_PLAY_SPARK:
				//½ºÆÄÅ©
				StartSkillDest( pX,pY,pZ, Angle.x , Angle.y , Angle.z , SKILL_AUTOMATION );
				break;
		*/

	case SKILL_PLAY_GROUND_PIKE:
		//±×¶ó¿îµå ÆÄÀÌÅ©
		//if( chrAttackTarget ) {
		//StartSkill( pX,pY,pZ, 0, Angle.y,0, SKILL_GROUND_PIKE , point );
		//}
		break;

	case SKILL_PLAY_TORNADO:
		//StartSkillDest( pX,pY,pZ, lpTarChar->pX,lpTarChar->pY,lpTarChar->pZ, SKILL_TORNADO , point );
		break;

	case SKILL_PLAY_EXPANSION:
		SkillPlaySound(SKILL_SOUND_SKILL_EXPANSION1, pX, pY, pZ);
		break;

	case SKILL_PLAY_VENOM_SPEAR:
		AssaParticle_VeonmSpearPike(this);
		SkillPlaySound(SKILL_SOUND_JUMPING_CRASH1, pX, pY, pZ);
		break;

	case SKILL_PLAY_CHAIN_LANCE:
		//SkillPlaySound( SKILL_SOUND_SKILL_CHAIN_LANCE1 , pX,pY,pZ );
		break;

	case SKILL_PLAY_VANISH:
		AssaParticle_Vanish(this);
		SkillPlaySound(SKILL_SOUND_SKILL_VANISH1, pX, pY, pZ);
		break;
	case SKILL_PLAY_FALCON:
		if (HoSkillCode) {
			HoSkillCode = 0;
			EffectMgr.Main();
		}

		StartSkill(pX, pY, pZ, 0, 0, 0, this, SKILL_FALCON, point - 1);
		HideWeapon = TRUE;
		SkillPlaySound(SKILL_SOUND_SKILL_FALCON, pX, pY, pZ);
		break;

	case SKILL_PLAY_GOLDEN_FALCON:
		if (HoSkillCode) {
			HoSkillCode = 0;
			EffectMgr.Main();
		}

		AssaParticle_GoldenFalcon(this, Golden_Falcon_Time[point - 1]);
		HideWeapon = TRUE;
		SkillPlaySound(SKILL_SOUND_SKILL_GOLDEN_FALCON, pX, pY, pZ);
		break;

	case SKILL_PLAY_RAGEOF_ZECRAM:
		StartSkill(pX, pY + 24 * fONE, pZ, 0, 0, 0, SKILL_RAGE_OF_ZECRAM_POWER1);
		SkillPlaySound(SKILL_SOUND_SKILL_RAGE_OF_ZECRAM3, pX, pY, pZ);
		break;

	case SKILL_PLAY_ROAR:
		StartSkill(pX, pY, pZ, 0, 0, 0, SKILL_ROAR, point);

		if ((dwPlayTime & 1) == 0)
			SkillPlaySound(SKILL_SOUND_SKILL_ROAR1, pX, pY, pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_ROAR2, pX, pY, pZ);

		break;

	case SKILL_PLAY_CONCENTRATION:
		AssaParticle_Concentration(this, Concentration_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_CONCENTRATION1, pX, pY, pZ);
		break;

	case SKILL_PLAY_SWIFT_AXE:
		AssaParticle_SwiftAxe(this, Swift_Axe_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_SWIFT_AXE1, pX, pY, pZ);
		break;

		/*
			case SKILL_PLAY_METEO:
				//¸ÞÅ×¿À
				if ( lpTarChar ) {
					Skil_RangeAttack( lpTarChar->pX, lpTarChar->pY, lpTarChar->pZ, 160*fONE , 20 , 0 );
					SetMotionFromCode( CHRMOTION_STATE_ATTACK );
					StartSkillDest( pX,pY,pZ , lpTarChar->pX,lpTarChar->pY,lpTarChar->pZ , SKILL_METEO );
				}
				else
					return FALSE;
		*/

	case SKILL_PLAY_SWORD_BLAST:
		SkillPlaySound(SKILL_SOUND_SKILL_SWORDBLAST1, pX, pY, pZ);
		break;

	case SKILL_PLAY_BRANDISH:
		SkillPlaySound(SKILL_SOUND_SKILL_BLANDISH1, pX, pY, pZ);
		break;

	case SKILL_PLAY_HOLY_BODY:
		dwDispAppSkill |= SKILL_APPMASK_PHYSICAL_ABSORB;
		sinEffect_Holy_Body(this);
		StartSkill(pX, pY, pZ, 0, Angle.y, 0, this, SKILL_SHIELD1, point);
		SkillPlaySound(SKILL_SOUND_SKILL_HOLYBODY, pX, pY, pZ);
		break;

	case SKILL_PLAY_HOLY_VALOR:
		AssaParticle_HolyValor_Jang(this, Holy_Valor_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_HOLY_VALOR1, pX, pY, pZ);
		break;

	case SKILL_PLAY_PIERCING:
		sinSkillEffect_Piercing(this);
		SkillPlaySound(SKILL_SOUND_SKILL_PIERCING, pX, pY, pZ);
		break;

	case SKILL_PLAY_FLAME_BRANDISH:
		SkillPlaySound(SKILL_SOUND_SKILL_BLANDISH1, pX, pY, pZ);
		break;

	case SKILL_PLAY_DRASTIC_SPIRIT:
		AssaParticle_DrasticSpirit_Jang(this, Drastic_Spirit_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_DRASTICSPIRIT1, pX, pY, pZ);
		break;

	case SKILL_PLAY_DIVINE_INHALATION:
		SkillPlaySound(SKILL_SOUND_SKILL_DIVINE_INHALATION, pX, pY, pZ);
		break;

	case SKILL_PLAY_HOLY_INCANTATION:
		AssaParticle_HolyIncantationHand(this);
		break;
	case SKILL_PLAY_WINDY:
		sinSkillEffect_Windy(this);
		AssaParticle_Windy(this, Windy_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_WINDY1, pX, pY, pZ);
		break;

	case SKILL_PLAY_SPLIT_JAVELIN:
		if (chrAttackTarget && AttackCritcal >= 0 && GetAttackPoint(&x, &y, &z) == TRUE) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 28 * fONE;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_SplitJavelin(&Pos1, &Pos2, param);
		}

		SkillPlaySound(SKILL_SOUND_SKILL_SPLIT_JAVELIN1, pX, pY, pZ);
		break;

	case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
		SkillPlaySound(SKILL_SOUND_SKILL_TRIUMPH_OF_VALHALLA1, pX, pY, pZ);
		break;

	case SKILL_PLAY_HEALING:
		if (!lpTarChar && this == lpCurPlayer) {
			sinEffect_Healing2(this);
			SkillPlaySound(SKILL_SOUND_SKILL_HEALING, pX, pY, pZ);
		}
		else {
			if (lpTarChar) {
				sinEffect_Healing2(lpTarChar);
				SkillPlaySound(SKILL_SOUND_SKILL_HEALING, lpTarChar->pX, lpTarChar->pY, lpTarChar->pZ);
			}
		}

		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		break;

	case SKILL_PLAY_HOLY_MIND:
		if (lpTarChar) {
			cnt = lpTarChar->smCharInfo.Resistance[sITEMINFO_BIONIC];
			if (cnt < 0) cnt = 0;
			if (cnt > 100) cnt = 100;
			cnt = (15 * (100 - cnt)) / 100;
			if (cnt > 0)
				AssaParticle_HolyMind_Attack(lpTarChar, cnt);
		}

		SkillPlaySound(SKILL_SOUND_SKILL_HOLYMIND, pX, pY, pZ);
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		break;

	case SKILL_PLAY_HOLY_BOLT:
	case SKILL_PLAY_MULTI_SPARK:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		break;

	case SKILL_PLAY_DIVINE_LIGHTNING:
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);
		break;
	case SKILL_PLAY_HOLY_REFLECTION:
		sinSkillEffect_Holy_Reflection(this, Holy_Reflection_Time[point - 1]);
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);
		SkillPlaySound(SKILL_SOUND_SKILL_HOLYREFLECTION1, pX, pY, pZ);
		break;
	case SKILL_PLAY_GREAT_HEALING:
		sinSkillEffect_Great_Healing(this);
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);
		SkillPlaySound(SKILL_SOUND_SKILL_GRANDHEALING, pX, pY, pZ);
		break;

	case SKILL_PLAY_VIGOR_BALL:
	case SKILL_PLAY_RESURRECTION:
	case SKILL_PLAY_EXTINCTION:
	case SKILL_PLAY_VIRTUAL_LIFE:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);
		break;

	case SKILL_PLAY_AGONY:
		sinEffect_Agony(this);
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);
		SkillPlaySound(SKILL_SOUND_SKILL_AGONY, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);
		break;
	case SKILL_PLAY_ZENITH:
		sinEffect_Zenith(this, Zenith_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);
		SkillPlaySound(SKILL_SOUND_SKILL_ZENITH, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);
		break;

	case SKILL_PLAY_FIRE_BALL:
	case SKILL_PLAY_FIRE_BOLT:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);
		break;

	case SKILL_PLAY_COLUMN_OF_WATER:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);
		break;
	case SKILL_PLAY_ENCHANT_WEAPON:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);

		pos.x = pX;
		pos.y = pY;
		pos.z = pZ;

		if (this == lpCurPlayer) {
			if (!chrAttackTarget) {
				cnt = Enchant_Weapon_Time[point - 1];
				EnchantEffect_Point = point;
				switch ((param)) {
				case 0:
					AssaParticle_EnchantWeaponIceJang(this, cnt);
					WeaponEffect = sITEMINFO_ICE + 1;
					dwWeaponEffectTime = dwPlayTime + cnt * 1000;
					break;
				case 1:
					AssaParticle_EnchantWeaponLightJang(this, cnt);
					WeaponEffect = sITEMINFO_LIGHTING + 1;
					dwWeaponEffectTime = dwPlayTime + cnt * 1000;
					break;
				case 2:
					AssaParticle_EnchantWeaponFireJang(this, cnt);
					WeaponEffect = sITEMINFO_FIRE + 1;
					dwWeaponEffectTime = dwPlayTime + cnt * 1000;
					break;
				}
			}
		}
		SkillPlaySound(SKILL_SOUND_SKILL_ENCHANTWEAPON_CAST, pX, pY, pZ);
		break;

	case SKILL_PLAY_DEAD_RAY:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_ENERGY_SHIELD:
	case SKILL_PLAY_DIASTROPHISM:
	case SKILL_PLAY_SPIRIT_ELEMENTAL:
	case SKILL_PLAY_DANCING_SWORD:

		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;


	case SKILL_PLAY_COMPULSION:
		SkillPlaySound(SKILL_SOUND_SKILL_COMPULSION1, pX, pY, pZ);
		break;

	case SKILL_PLAY_IMPULSION:
		SkillImpulsionLight(this, 2);
		break;

	case SKILL_PLAY_MAGNETIC_SPHERE:
		SkillPlaySound(SKILL_SOUND_SKILL_SPARK1, pX, pY, pZ);
		break;

	case SKILL_PLAY_METAL_GOLEM:
		SkillPlaySound(SKILL_SOUND_SKILL_METAL_GOLEM, pX, pY, pZ);
		break;

	case SKILL_PLAY_DESTROYER:
		SkillWarriorDestroyerBlade(this, 3.0f);          //ÀÜ»ó
		break;

	case SKILL_PLAY_BERSERKER:
		SkillPlaySound(SKILL_SOUND_SKILL_BERSERKER, pX, pY, pZ);
		break;

	case SKILL_PLAY_CYCLONE_STRIKE:
		SkillWarriorCycloneStrikeBlade(this, 3);
		SkillPlaySound(SKILL_SOUND_SKILL_CYCLONE_STRIKE, pX, pY, pZ);
		break;

	case SKILL_PLAY_ASSASSIN_EYE:
		SkillPlaySound(SKILL_SOUND_SKILL_ASSASSIN_EYE, pX, pY, pZ);
		break;

	case SKILL_PLAY_CHARGING_STRIKE:
	case SKILL_PLAY_SHADOW_MASTER:
		if (this == lpCurPlayer) SkillEventDamageCount = 0;
		break;

	case SKILL_PLAY_PHOENIX_SHOT:
		//SkillPlaySound( SKILL_SOUND_SKILL_CHARGING	 , pX,pY,pZ );
		break;

	case SKILL_PLAY_VAGUE:
		SkillPlaySound(SKILL_SOUND_SKILL_VAGUE, pX, pY, pZ);
		break;

	case SKILL_PLAY_GODS_BLESS:
		SkillSaintGodBless(this, (float)God_Bless_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_GODS_BLESS, pX, pY, pZ);
		break;

	case SKILL_PLAY_RECALL_WOLVERIN:
		break;

	case SKILL_PLAY_EVASION_MASTERY:
		pos.x = pX;
		pos.y = pY - 2000;
		pos.z = pZ;
		AssaParticle_RisingLine(&pos, "attack02_02.bmp", 300, 1, 300, 10, 300);

		SkillPlaySound(SKILL_SOUND_SKILL_EVASION_MASTERY, pX, pY, pZ);
		break;

	case SKILL_PLAY_FORCE_OF_NATURE:
		SkillPlaySound(SKILL_SOUND_SKILL_FORCE_OF_NATURE, pX, pY, pZ);
		break;

	case SKILL_PLAY_GODLY_SHIELD:
		SkillPlaySound(SKILL_SOUND_SKILL_GODLY_SHIELD, pX, pY, pZ);
		break;

	case SKILL_PLAY_SWORD_OF_JUSTICE:
		SkillPlaySound(SKILL_SOUND_SKILL_SWORD_OF_JUSTICE, pX, pY, pZ);
		break;

	case SKILL_PLAY_HALL_OF_VALHALLA:
		SkillPlaySound(SKILL_SOUND_SKILL_HALL_OF_VALHALLA, pX, pY, pZ);
		break;
	case SKILL_PLAY_FROST_JAVELIN:
		SkillPlaySound(SKILL_SOUND_SKILL_FROST_JAVELIN, pX, pY, pZ);
		break;

	case SKILL_PLAY_GLACIAL_SPIKE:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_REGENERATION_FIELD:
		SkillPlaySound(SKILL_SOUND_SKILL_REGEN_FIELD2, pX, pY, pZ);
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_CHAIN_LIGHTNING:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_PRIEST, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_SUMMON_MUSPELL:
		SkillPlaySound(SKILL_SOUND_SKILL_MUSPELL1, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®

		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 1);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_FIRE_ELEMENTAL:
		SkillPlaySound(SKILL_SOUND_SKILL_FIRE_ELEMENTAL, pX, pY, pZ);
		//SkillPlaySound( SKILL_SOUND_SKILL_CASTING_MAGICIAN	 , pX,pY,pZ );		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_FLAME_WAVE:
		SkillPlaySound(SKILL_SOUND_SKILL_FLAME_WAVE3, pX, pY, pZ);
		//SkillPlaySound( SKILL_SOUND_SKILL_CASTING_MAGICIAN	 , pX,pY,pZ );		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;

	case SKILL_PLAY_DISTORTION:
		SkillPlaySound(SKILL_SOUND_SKILL_DISTORTION, pX, pY, pZ);
		//SkillPlaySound( SKILL_SOUND_SKILL_CASTING_MAGICIAN	 , pX,pY,pZ );		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;


	case SKILL_PLAY_METEO:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);
		break;
	}

	//AttackSkil = SkilCode;

	return TRUE;
}

//¸ó½ºÅÍ ÀÏ¹Ý °ø°Ý ½ÃÀÛ
int smCHAR::BeginAttack_Monster()
{
	switch (smCharInfo.dwCharSoundCode) {
	case snCHAR_SOUND_DARKKNIGHT:
		//µÒ °¡µå
		if (smCharInfo.Level >= 65)
			AssaParticle_DoomGuardHit1(this);
		break;

	case snCHAR_SOUND_GUARDIAN_SAINT:
		//°¡µð¾È - ¼¼ÀÎÆ®
		//AssaParticleClanMonsterHit2( this );
		break;

	case snCHAR_SOUND_DEATHKNIGHT:			//¹ß·»Åä
		if (MotionInfo->KeyCode != 'L')
			ParkValentSwordShow(this, 70);
		break;

	case snCHAR_SOUND_DEVIL_BIRD:			//µ¥ºí¹öµå
		if (MotionInfo->KeyCode != 'B')
			ParkDevilBirdAttackBlur(this);
		break;


	case snCHAR_SOUND_KELVEZU:
		if (MotionInfo->KeyCode == 'P') {
			ParkKelvezuWingShow(this, 70);
		}
		else {
			ParkKelvezuFingerShow(this, 70);
		}

		break;

	case snCHAR_SOUND_CHIMERA:
		ParkAssaParticle_ChimeraNormal(this);
		break;
	case snCHAR_SOUND_REVIVED_ARCHER:
		//È­»ì ½ÃÀ§ ´ç±â±â
		break;
	}

	return TRUE;
}

//¸ó½ºÅÍ ½ºÅ³ ½ÃÀÛ
int smCHAR::BeginSkill_Monster()
{
	POINT3D	pos;

	switch (smCharInfo.dwCharSoundCode) {
	case snCHAR_SOUND_NAZ:
		GetMoveLocation(0, 0, 100 * fONE, 0, Angle.y, 0);
		StartSkillDest(pX, pY, pZ, pX + GeoResult_X, pY, pZ + GeoResult_Z, SKILL_TORNADO2, 1);
		break;

	case snCHAR_SOUND_FURY:
		StartEffectMonster(pX, pY, pZ, 0, Angle.y, 0, MONSTER_FURY_MAGIC1);
		break;

	case snCHAR_SOUND_DARKKNIGHT:
		//µÒ °¡µå
		if (smCharInfo.Level >= 65)
			AssaParticle_DoomGuardHit2(this);
		break;

	case snCHAR_SOUND_HEAVYGOBLIN:
		//Çìºñ °íºí¸°
		AssaParticle_HeavyGoblinHit1(this);
		break;

	case snCHAR_SOUND_ICEGOBLIN:			//¾ÆÀÌ½º °íºí¸°
		ParkAssaParticle_IceGoblinHit1(this);
		break;

	case snCHAR_SOUND_COLDEYE:
		ParkParticle_ColdEyeSkill(this);
		break;

	case snCHAR_SOUND_CHAOSCARA:
		if (MotionInfo->KeyCode == 'J') {
			if (chrAttackTarget) {
				ParkAssaParticle_ChaosKara2(chrAttackTarget);
			}
			LastSkillParam = 0;
		}
		else {
			ParkAssaParticle_ChaosKaraSkill_Monster(this);
			//¸ó½ºÅÍ ½ºÅ³ ÀÌÆåÆ® ½ÇÇö ( ÀÌÆåÆ® ½ÇÇö )
			SkillPlay_Monster_Effect(this, SKILL_PLAY_CHAOSCARA_VAMP, 220);
			LastSkillParam = 1;
		}
		break;

	case snCHAR_SOUND_DEATHKNIGHT:			//¹ß·»Åä
		ParkValentSwordShow(this, 70);
		break;

	case snCHAR_SOUND_DEVIL_BIRD:
		DevilBird_Skill(this);
		break;

	case snCHAR_SOUND_KELVEZU:
		if (MotionInfo->KeyCode == 'I') {
			ParkKelvezuWingShow(this, 200);
		}
		else {
			ParkKelvezuTaleShow(this, 90);
			ParkKelvezuSkill2(this);
		}
		break;

	case snCHAR_SOUND_NPC_SKILLMASTER:
		switch (MotionInfo->KeyCode) {
		case 'A':	//½ºÆÄÅ©
			break;
		case 'B':	//º» Å©·¡½¬
		case 'N':	//Ã¼ÀÎ ·£½º
		case 'Y':	//µð½ºÆ®·ÎÀÌ¾î
			AssaMotionBlur(this, "Bip01 R Hand", "bip01 weapon", 80);
			break;
		case 'M':	//°ñµçÆÓÄÁ
			break;

		case 'L':	//Â÷Â¡½ºÆ®¶óÀÌÅ©
			SkillLancelotChargingStrike(this);
			break;

		}
		break;

	case snCHAR_SOUND_NPC_MAGICMASTER:
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;


	case snCHAR_SOUND_CASTLE_SOLDER_A:
		AssaMotionBlur(this, "Bip01_w", "Bip01 Effect", 80);
		break;
	case snCHAR_SOUND_CASTLE_SOLDER_B:
		AssaMotionBlur(this, "Bip01 waraxe", "Bip01 Effect", 80);
		break;
	case snCHAR_SOUND_CASTLE_SOLDER_C:
		AssaMotionBlur(this, "Bip01 Sword", "Bip01 Effect", 80);
		break;
	case snCHAR_SOUND_REVIVED_KNIGHT:	// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ³ªÀÌÆ®
		if (chrAttackTarget)
		{
			if (MotionInfo->KeyCode == 'H')
			{
				//ÇÇ¾î½Ì
				//sinSkillEffect_Piercing( this );
				SkillPlaySound(SKILL_SOUND_SKILL_PIERCING, pX, pY, pZ);
			}
		}
		break;
	case snCHAR_SOUND_REVIVED_PRIESTESS:	// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ÇÁ¸®½ºÆ¼½º
		SkillPlaySound(SKILL_SOUND_SKILL_CASTING_MAGICIAN, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
		pos.x = pX;	pos.y = pY;	pos.z = pZ;
		sinEffect_StartMagic(&pos, 2);			//¸¶¹ýÁø ÀÌÆåÆ®
		break;
	case snCHAR_SOUND_REVIVED_PIKEMAN:		// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ÆÄÀÌÅ©¸Ç
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'Z':	//Ã¼ÀÎ ·£½º
				AssaMotionBlur(this, "Bip01 R Hand", "bip01 wea", 80);
				break;
			}
		}
		break;
	}

	return TRUE;
}

//¸ó½ºÅÍ ½ºÅ³ ÀÌº¥Æ®
int smCHAR::EventSkill_Monster()
{
	POINT3D	pos, pos2;
	RECT rect;
	int x, y, z;

	switch (smCharInfo.dwCharSoundCode) {
	case snCHAR_SOUND_NAZ:

		break;

	case snCHAR_SOUND_HEADCUTTER:
		GetMoveLocation(0, 0, 60 * fONE, 0, Angle.y, 0);
		StartEffectMonster(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z, 0, Angle.y, 0, MONSTER_HEADER_CUTTER_HIT1);
		break;

	case snCHAR_SOUND_TEMPLEGUARD:
	case snCHAR_SOUND_DMACHINE:
	case snCHAR_SOUND_TURTLE_CANNON:
		//µð¸Ó½Å
		if (chrAttackTarget) {
			GetMoveLocation(0, 40 * fONE, 0, 0, Angle.y, 0);
			StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
				MONSTER_DMACHINE_MISSILE2);
		}
		break;
	case snCHAR_SOUND_REVIVED_KNIGHT:	// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ³ªÀÌÆ®
		switch (MotionInfo->KeyCode)
		{
		case 'H':		// ÇÇ¾î½Ì
			if (chrAttackTarget)
			{
				sinSkillEffect_Piercing_Attack(this);
			}
			break;
		case 'Z':		// ±×·£µåÅ©·Î½º
			if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE)
			{
				pos.x = x;
				pos.y = y;
				pos.z = z;
				if (MotionEvent == 1)
				{
					AssaParticle_GrandCrossHorz(&pos, Angle.y);
					SkillPlaySound(SKILL_SOUND_SKILL_SWING2, pX, pY, pZ);
				}
				else
				{
					AssaParticle_GrandCrossVert(&pos, Angle.y);
					SkillPlaySound(SKILL_SOUND_SKILL_GRAND_CROSS, pX, pY, pZ);
				}
			}
			else
			{
				//¹Ì½º ¼Ò¸®
				//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
				PlayWaponSoundDirect(pX, pY, pZ, 13);
			}
			break;

		}
		break;

	case snCHAR_SOUND_OMICRON:
		//¿À¹ÌÅ©·Ð
		if (chrAttackTarget) {
			GetMoveLocation(0, 20 * fONE, 20 * fONE, 0, Angle.y, 0);
			StartEffectMonster(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				0, Angle.y, 0, MONSTER_OMICRON_HIT1);
		}
		break;

	case snCHAR_SOUND_RATOO:
		//¶óÅõ
		if (chrAttackTarget) {
			AssaParticle_RatooHit1(this);
		}
		break;

	case snCHAR_SOUND_MOKOVA:			//¸ðÄÚ¹Ù
	case snCHAR_SOUND_STYGIANLORD:
		//½ºÆ¼Áö¾ð·Îµå
		if (chrAttackTarget) {
			StartEffectMonster(pX, pY, pZ, MONSTER_STYGIANLORD_MAGIC1);
		}
		break;

	case snCHAR_SOUND_SHADOW:
		//½¦µµ¿ì ½ºÅ³
		if (chrAttackTarget) {
			GetMoveLocation(0, 140 * fONE, 32 * fONE, 0, Angle.y, 0);
			StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
				Angle.x, Angle.y, Angle.z, MONSTER_SHADOW_SHOT1);
		}
		break;

	case snCHAR_SOUND_SUCCUBUS:
		//½áÅ¥¹ö½º
		if (chrAttackTarget) {

			if (smCharInfo.Level <= 75) {	//ÈÖÄ¡
				StartEffectMonster(pX, pY, pZ, MONSTER_SERQBUS_MAGIC1);
				StartEffect_Serqbus_Skill1(chrAttackTarget, SUCCUBUS_CURSE_TIME);
				StartEffectMonster(chrAttackTarget->pX, chrAttackTarget->pY + chrAttackTarget->PatHeight, chrAttackTarget->pZ, MONSTER_SERQBUS_MAGIC_ATTACK1);

				if (chrAttackTarget == lpCurPlayer) {		//Èí¼ö ¾àÈ­ Ãâ·Â
					if (!sDisplayState[0].Absorb)
						sDisplayState[0].Absorb = -GetRandomPos(2, 6);
					sDisplayState[0].MaxTime = dwPlayTime + SUCCUBUS_CURSE_TIME * 1000;
				}

			}
			else if (smCharInfo.Level < 80) {	//¼­Å¥¹ö½º
				if (chrAttackTarget->smCharInfo.State != smCHAR_STATE_USER) {
					StartEffectMonster(pX, pY, pZ, MONSTER_SERQBUS_MAGIC2);
					StartEffectMonster(chrAttackTarget->pX, chrAttackTarget->pY + chrAttackTarget->PatHeight, chrAttackTarget->pZ, MONSTER_SERQBUS_MAGIC_ATTACK2);
				}
			}
			else if (smCharInfo.Level < 90) {	//ÀÎÅ¥¹ö½º
				StartEffectMonster(pX, pY, pZ, MONSTER_SERQBUS_MAGIC3);
				StartEffect_Serqbus_Skill3(chrAttackTarget, SUCCUBUS_CURSE_TIME);
				StartEffectMonster(chrAttackTarget->pX, chrAttackTarget->pY + chrAttackTarget->PatHeight, chrAttackTarget->pZ, MONSTER_SERQBUS_MAGIC_ATTACK3);

				if (chrAttackTarget == lpCurPlayer) {		//°ø°Ý ¾àÈ­ Ãâ·Â
					if (!sDisplayState[1].PercentDamage[0]) {
						sDisplayState[1].PercentDamage[0] = -GetRandomPos(10, 15);
						sDisplayState[1].PercentDamage[1] = sDisplayState[1].PercentDamage[0];
					}
					sDisplayState[1].MaxTime = dwPlayTime + SUCCUBUS_CURSE_TIME * 1000;
				}
			}
			else {	//ÀÎÅ¥¹ö½º ¼­¸Ó³Ê
				ParkIncubusSummonerSkill(this);
			}
		}
		break;


	case snCHAR_SOUND_EVILPLANT:
		//ÀÌºí ÇÃ·£Æ® ¸ÞÀÌÇÃ
		if (smCharInfo.Level >= 60) {
			//ÀÌºí ÇÃ·£Æ®
			AssaParticle_EvilPlantShot(this);
		}
		else {
			//ÀÌºí ¸ÞÀÌÇÃ
			AssaParticle_EvilMapleShot(this);
		}
		break;

	case snCHAR_SOUND_STONEGOLEM:
		//½ºÅæ °í·½
		AssaParticle_StoneGolemShot(this);
		break;


	case snCHAR_SOUND_SOLIDSNAIL:
		//³ªÀÌÆ® ¸Þ¾î
		if (chrAttackTarget) {
			pos.x = chrAttackTarget->pX;
			pos.y = chrAttackTarget->pY;
			pos.z = chrAttackTarget->pZ;

			AssaParticle_SnailShot(this, &pos);
		}

	case snCHAR_SOUND_SCORPION:
		//ÀÌºí ½º³×ÀÏ
		if (chrAttackTarget && smCharInfo.Level == 50) {
			pos.x = chrAttackTarget->pX;
			pos.y = chrAttackTarget->pY;
			pos.z = chrAttackTarget->pZ;

			AssaParticle_SnailShot(this, &pos);
		}
		break;

	case snCHAR_SOUND_THORNCRAWLER:
		//¼ð Å©·Î¿ï·¯
		if (chrAttackTarget) {
			pos.x = chrAttackTarget->pX;
			pos.y = chrAttackTarget->pY;
			pos.z = chrAttackTarget->pZ;

			AssaParticle_ThornCrawlerShot(this, &pos);
		}
		break;

	case snCHAR_SOUND_NIGHTMARE:
	case snCHAR_SOUND_HEST: // ¹ÚÀç¿ø - ÀúÁÖ¹ÞÀº ½ÅÀü 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) - Çì½ºÆ®
		//³ªÀÌÆ® ¸Þ¾î
		AssaParticle_NightMare(this);
		break;

	case snCHAR_SOUND_GUARDIAN_SAINT:
		AssaParticleClanMonsterHit1(this);
		break;

	case snCHAR_SOUND_HYPERMACHINE:
		//ÇÏÀÌÆÛ¸Ó½Å
		if (chrAttackTarget) {
			pos.x = chrAttackTarget->pX;
			pos.y = chrAttackTarget->pY;
			pos.z = chrAttackTarget->pZ;
			AssaParticle_MonsterIronHyperSkill1(this, &pos);
		}
		break;

	case snCHAR_SOUND_RUNICGUARDIAN:		//·ç´Ð°¡µð¾È
		if (chrAttackTarget) {
			AssaParticle_MonsterIronRunicGuardian(this, chrAttackTarget);
		}
		break;

	case snCHAR_SOUND_BABEL:				//¹Ùº§ (·ç´Ð°¡µð¾È º¸½º)
		if (chrAttackTarget) {
			AssaParticle_MonsterRunicGrardianBossSkill(this);
		}
		break;


	case snCHAR_SOUND_MOUNTAIN:			//¸¶¿îÆ¾
		AssaParticle_MonsterIronMountainSkill1(this);
		break;

	case snCHAR_SOUND_TOWERGOLEM:		//Å¸¿ö°í·½
		AssaParticle_MonsterIronTowerGolemSkill1(this);
		break;

	case snCHAR_SOUND_SADNESS:			//»õµå´Ï½º
		AssaParticle_MonsterIronSadNessSkill1(this);
		break;

	case snCHAR_SOUND_IRONFIST:
		AssaParticle_MonsterIronFist(this);
		break;

	case snCHAR_SOUND_MORGON:
	case snCHAR_SOUND_GORGON: // pluto Ãß°¡ ¸ó½ºÅÍ °í¸£°ï
		AssaParticle_MonsterIronMorgonHit(this);
		break;

		///////////////// ¾ÆÀÌ½º ¸ó½ºÅÍ ///////////////

	case snCHAR_SOUND_ICEGOLEM:
		ParkAssaParticle_IceGolemShot(this);
		break;
	case snCHAR_SOUND_FROST:
		ParkAssaParticle_AcientDia(this);
		break;
	case snCHAR_SOUND_FROZEN:
		if (chrAttackTarget) {
			ParkAssaParticle_MistIceBall(this, chrAttackTarget);
		}
		break;

	case snCHAR_SOUND_MYSTIC:
		if (chrAttackTarget) {
			StartEffectMonster(pX, pY, pZ, MONSTER_SERQBUS_MAGIC2);
			StartEffect_Serqbus_Skill1(chrAttackTarget, SUCCUBUS_CURSE_TIME);
			StartEffectMonster(chrAttackTarget->pX, chrAttackTarget->pY + chrAttackTarget->PatHeight, chrAttackTarget->pZ, MONSTER_SERQBUS_MAGIC_ATTACK1);

			if (chrAttackTarget == lpCurPlayer) {		//Èí¼ö ¾àÈ­ Ãâ·Â
				if (!sDisplayState[0].Absorb)
					sDisplayState[0].Absorb = -GetRandomPos(10, 16);
				sDisplayState[0].MaxTime = dwPlayTime + SUCCUBUS_CURSE_TIME * 1000;
			}
		}
		break;

	case snCHAR_SOUND_DEATHKNIGHT:			//¹ß·»Åä
		if (MotionInfo->KeyCode == 'N')
			ParkAssaParticle_Valento1(this);
		break;


		////////////////// ±×¸®µðÈ£¼ö ¸ó½ºÅÍ /////////////////////
	case snCHAR_SOUND_SPIDER:
		ParkAssaParticle_SpiderTrooperHit1(this);
		break;

	case snCHAR_SOUND_STINGRAY:
		ParkAssaParticle_StingRay(this);
		break;

	case snCHAR_SOUND_M_LORD:
		SkillPlay_MummyLord_Effect(this, 200);
		break;

	case snCHAR_SOUND_GOBLINSHAMAN:	// ¹ÚÀç¿ø - ÀúÁÖ¹ÞÀº ½ÅÀü 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) - °íºí¸°»þ¸Õ
		SkillPlay_MummyLord_Effect(this, 200);
		break;


		///////////////////////////////////////////////

	case snCHAR_SOUND_BLIZZARD_GIANT:
		if (chrAttackTarget) {
			ParkBlizzardGiantSkillToTarget(chrAttackTarget);
		}
		break;
	case snCHAR_SOUND_DEVIL_BIRD:
		if (chrAttackTarget) {
			ParkDevilBirdSkillToTarget(chrAttackTarget);
		}
		break;

	case snCHAR_SOUND_KELVEZU:
		if (chrAttackTarget) {
			if (MotionInfo->KeyCode == 'I') {
				ParkKelvezuSkillHit(chrAttackTarget);
				ParkKelvezuSkill1(this);
			}
			else {
				ParkKelvezuSkillHit(chrAttackTarget);
			}
		}
		break;

		///////////////////////////////////////////////

	case snCHAR_SOUND_DARKPHALANX:
		ParkAssaParticle_DarkPhalanx(this);
		break;
	case snCHAR_SOUND_FIREWORM:
		if (chrAttackTarget) {
			ParkAssaParticle_FireWorm(chrAttackTarget, 1);
		}
		break;

	case snCHAR_SOUND_CHIMERA:
		if (chrAttackTarget) {
			ParkAssaParticle_ChimeraSkill(chrAttackTarget);
		}
		break;

	case snCHAR_SOUND_BLOODYKNIGHT:
		ParkAssaParticle_BloodySkill(this);
		break;
	case snCHAR_SOUND_HELLHOUND:
		ParkAssaParticle_HellHoundSkillAttack(this);
		break;
	case snCHAR_SOUND_DARKGUARD:
		if (chrAttackTarget) {
			ParkAssaParticle_DarkGuardSkill(this, chrAttackTarget);
		}
		break;
	case snCHAR_SOUND_DARKMAGE:
		if (chrAttackTarget) {
			ParkAssaParticle_DarkMageSkill(chrAttackTarget);
		}
		break;

	case snCHAR_SOUND_SETO:
		SkillArchMageFlameWave(this);
		break;

	case snCHAR_SOUND_KINGSPIDER:
		//ÇæÅ© ²¨¶û µ¿ÀÏ
		if (chrAttackTarget) {
			GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
			StartEffectMonster(pX + GeoResult_X, pY + 22 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
		}
		break;
		///////////////////////////////////////////////


	case snCHAR_SOUND_S_METALGOLEM:		//¼ÒÈ¯¸÷ ¸ÞÅ»°í·½
		if (chrAttackTarget) {
			if (!smCharInfo.wPlayClass[1])
				GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
			else
				GetMoveLocation(0, 0, 54 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

			StartEffectMonster(pX + GeoResult_X, pY + 48 * fONE, pZ + GeoResult_Z, MONSTER_HULK_HIT1);
		}
		break;

	case snCHAR_SOUND_S_WOLVERLIN:		//¼ÒÈ¯¸÷ ¿ï¹ö¸°
		if (chrAttackTarget) {
			if (!smCharInfo.wPlayClass[1])
				GetMoveLocation(0, 0, 50 * fONE, 0, Angle.y, 0);
			else
				GetMoveLocation(0, 0, 50 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

			x = pX + GeoResult_X;
			z = pZ + GeoResult_Z;
			y = pY + 18 * fONE;
			StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
			StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
			StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
			StartEffectMonster(x, y, z, MONSTER_PIGON_PARTICLE1);
		}
		break;

	case snCHAR_SOUND_NPC_SKILLMASTER:
		if (chrAttackTarget) {
			switch (MotionInfo->KeyCode) {
			case 'A':	//½ºÆÄÅ©
				StartSkillDest(pX, pY + 24 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ, SKILL_SPARK, 8);
				SkillPlaySound(SKILL_SOUND_SKILL_SPARK2, pX, pY, pZ);
				break;
			case 'B':	//º» Å©·¡½¬
				if (GetAttackPoint(&x, &y, &z) == TRUE) {
					StartSkill(x, y, z, 0, 0, 0, SKILL_TRIPLE_IMPACT);
					SetDynLight(x, y, z, 100, 0, 0, 0, 300);
					SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
				}
				break;
			case 'N':	//Ã¼ÀÎ ·£½º
				if (GetAttackPoint(&x, &y, &z) == TRUE) {
					pos.x = x;
					pos.y = y;
					pos.z = z;
					AssaParticle_ChainLance(&pos);
				}
				break;
			case 'M':	//°ñµçÆÓÄÁ
				GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y;
				pos.z = pZ + GeoResult_Z;
				pos2.x = chrAttackTarget->pX;
				pos2.y = chrAttackTarget->pY + 24 * fONE;
				pos2.z = chrAttackTarget->pZ;
				SkillSagittarionPhoneixShot(this, &pos, &pos2);
				SkillPlaySound(SKILL_SOUND_SKILL_PHOENIX_SHOT, pX, pY, pZ);
				break;
			case 'Y':	//µð½ºÆ®·ÎÀÌ¾î
				if (GetAttackPoint(&pos.x, &pos.y, &pos.z) == TRUE) {
					SkillWarriorDestroyerHit(&pos);                       //Å¸°Ý
					if (MotionEvent < 3)
						SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
					else
						SkillPlaySound(SKILL_SOUND_SKILL_EXPANSION2, pX, pY, pZ);
				}
				break;
			case 'L':	//Â÷Â¡½ºÆ®¶óÀÌÅ©
				if (GetAttackPoint(&x, &y, &z) == TRUE) {
					StartSkill(x, y, z, 0, 0, 0, SKILL_TRIPLE_IMPACT);
					SetDynLight(x, y, z, 100, 0, 0, 0, 400);
					switch (MotionEvent) {
					case 1:
						SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE2, pX, pY, pZ);
						break;
					case 2:
						SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
						break;
					case 3:
					case 4:
						SkillPlaySound(SKILL_SOUND_SKILL_CHARGING_STRIKE, pX, pY, pZ);
						break;
					}
				}
				break;
			}
		}
		break;
	case snCHAR_SOUND_NPC_MAGICMASTER:
		if (chrAttackTarget) {
			switch (MotionInfo->KeyCode) {
			case 'R':	//¿¡³ÊÁö ½¯µå
				AssaParticle_EnergyShield(this, 5);
				SkillPlaySound(SKILL_SOUND_SKILL_ENERGY_SHIELD, pX, pY, pZ);
				break;
			case 'E':	//¸ÖÆ¼ ½ºÆÄÅ©
				sinEffect_MultiSpark(this, chrAttackTarget, 5);
				SkillPlaySound(SKILL_SOUND_SKILL_MULTISPARK, pX, pY, pZ);
				break;
			case 'U':	//µ¥½º ·¹ÀÌ
				GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y;
				pos.z = pZ + GeoResult_Z;
				pos2.x = chrAttackTarget->pX;
				pos2.y = chrAttackTarget->pY + 32 * fONE;
				pos2.z = chrAttackTarget->pZ;

				AssaParticle_DeadLay(&pos, &pos2);
				SkillPlaySound(SKILL_SOUND_SKILL_DEATHRAY, pX, pY, pZ);
				break;
			case 'I':	//ÆÄÀÌ¾î º¼
				GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
					chrAttackTarget->pX, chrAttackTarget->pY + 20 * fONE, chrAttackTarget->pZ,
					MONSTER_PIGON_SHOT1);

				if (((dwPlayTime >> 4) & 1) == 0)
					SkillPlaySound(SKILL_SOUND_SKILL_FIREBALL1, pX, pY, pZ);
				else
					SkillPlaySound(SKILL_SOUND_SKILL_FIREBALL2, pX, pY, pZ);
				break;

			case 'H':	//±Û·¡¼È ½ºÆÄÀÌÅ©
				Angle.y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
				SkillCelestialGlacialSpike(this);

				GetMoveLocation(0, 0, 64 * fONE, 0, Angle.y, 0);
				SetDynLight(pX + GeoResult_X, pY, pZ + GeoResult_Z, 0, 0, 100, 0, 700);

				if (rand() % 2)
					SkillPlaySound(SKILL_SOUND_SKILL_GLACIAL_SPIKE1, pX, pY, pZ);
				else
					SkillPlaySound(SKILL_SOUND_SKILL_GLACIAL_SPIKE2, pX, pY, pZ);

				if (chrAttackTarget == lpCurPlayer) {
					lpCurPlayer->PlaySlowCount = 90;			//1ÃÊµ¿¾È ´À¸®°Ô ¿òÁ÷ÀÓ
					lpCurPlayer->AttackIce -= 160;
				}

				break;

			case 'G':	//¹ÌÆ¼¾î
				pos.x = chrAttackTarget->pX;
				pos.y = chrAttackTarget->pY;
				pos.z = chrAttackTarget->pZ;
				SkillArchMageMeteo(&pos);
				SkillPlaySound(SKILL_SOUND_SKILL_METEO1, pX, pY, pZ);
				break;

			}
		}
		break;

	case snCHAR_SOUND_CASTLE_SOLDER_A:
	case snCHAR_SOUND_CASTLE_SOLDER_B:
	case snCHAR_SOUND_CASTLE_SOLDER_C:
		if (chrAttackTarget) {
			if (GetAttackPoint(&x, &y, &z) == TRUE) {
				StartSkill(x, y, z, 0, 0, 0, SKILL_TRIPLE_IMPACT);
				SetDynLight(x, y, z, 100, 0, 0, 0, 300);
				SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
			}
		}
		break;

	case snCHAR_SOUND_CASTLE_CRYSTAL_R:
		AssaParticle_CastleRed(this);
		break;

	case snCHAR_SOUND_CASTLE_CRYSTAL_G:
		AssaParticle_CastleGreen(this);
		break;

	case snCHAR_SOUND_CASTLE_CRYSTAL_B:
		AssaParticle_CastleBlue(this);
		break;
	case snCHAR_SOUND_REVIVED_MAGICIAN:		// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ¸ÅÁö¼Ç
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'O':	//µ¥½º ·¹ÀÌ
				GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y;
				pos.z = pZ + GeoResult_Z;
				pos2.x = chrAttackTarget->pX;
				pos2.y = chrAttackTarget->pY + 32 * fONE;
				pos2.z = chrAttackTarget->pZ;

				AssaParticle_DeadLay(&pos, &pos2);
				SkillPlaySound(SKILL_SOUND_SKILL_DEATHRAY, pX, pY, pZ);
				break;
			case 'Q':	// ÇÃ·¹ÀÓ ¿þÀÌºê
				SkillArchMageFlameWave(this);

				if (rand() % 2)
					SkillPlaySound(SKILL_SOUND_SKILL_FLAME_WAVE1, pX, pY, pZ);
				else
					SkillPlaySound(SKILL_SOUND_SKILL_FLAME_WAVE2, pX, pY, pZ);
				break;
			case 'Z':	//¹ÌÆ¼¾î
				pos.x = chrAttackTarget->pX;
				pos.y = chrAttackTarget->pY;
				pos.z = chrAttackTarget->pZ;
				SkillArchMageMeteo(&pos);
				SkillPlaySound(SKILL_SOUND_SKILL_METEO1, pX, pY, pZ);
				break;
			}
		}
		break;
	case snCHAR_SOUND_REVIVED_ARCHER:		// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ¾ÆÃ³
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'O':            //À©µå ¾Ö·Î¿ì
				GetAttackPoint(&x, &y, &z);
				StartSkillDest(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY + (24 * fONE), chrAttackTarget->pZ, SKILL_WIND_ARROW);
				SkillPlaySound(SKILL_SOUND_WIND_ARROW, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
				break;
			case 'Z':			// ÇÇ´Ð½º ¼¦
				int cnt = frame - MotionInfo->StartFrame * 80;
				if (MotionInfo->EventFrame[1] <= (DWORD)cnt)
				{
					GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 24 * fONE;
					pos2.z = chrAttackTarget->pZ;
					SkillSagittarionPhoneixShot(this, &pos, &pos2);
					SkillPlaySound(SKILL_SOUND_SKILL_PHOENIX_SHOT, pX, pY, pZ);
				}
				break;
			}
		}
		break;
	case snCHAR_SOUND_REVIVED_ATALANTA:		// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ¾ÆÅ»¶õÅ¸
		switch (MotionInfo->KeyCode)
		{
		case 'O':	// ½ºÅè Àçºí¸°
			if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE)
			{
				pos.x = x;
				pos.y = y;
				pos.z = z;
				pos2.x = chrAttackTarget->pX;
				pos2.y = chrAttackTarget->pY + 28 * fONE;
				pos2.z = chrAttackTarget->pZ;

				AssaParticle_StormJavelin(&pos, &pos2);

				rect.left = -30;
				rect.right = 30;
				rect.top = 0;
				rect.bottom = 240;
				rect.bottom += 20;
				rect.bottom += 20;

				for (int cnt = 0; cnt < dmSelected_CharCnt; cnt++)
				{
					if (lpSelected_Char[cnt])
					{
						AssaParticle_StormJavelinHit(lpSelected_Char[cnt], this, 0.5f);
					}
				}
				switch (rand() % 2)
				{
				case 0:
					SkillPlaySound(SKILL_SOUND_SKILL_STORM_JAVELIN1, pX, pY, pZ);
					break;
				case 1:
					SkillPlaySound(SKILL_SOUND_SKILL_STORM_JAVELIN2, pX, pY, pZ);
					break;
				}
			}
			break;
		case 'Z':	// º¥Á¯½º
			if (chrAttackTarget)
			{
				GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y;
				pos.z = pZ + GeoResult_Z;
				pos2.x = chrAttackTarget->pX;
				pos2.y = chrAttackTarget->pY + 40 * fONE;
				pos2.z = chrAttackTarget->pZ;
				//if ( AttackCritcal>=0 )
				SkillVengeance(&pos, &pos2);

				//if ( this==lpCurPlayer && MotionEvent==1 )
				//{
				//	cnt = dm_SendTransDamage( chrAttackTarget , 0,0 , 0, 0, AttackSkil , TRUE );
				//	if ( !cnt )
				//		AttackCritcal = -1;				//ºø³ª°¨
				//}
				if (MotionEvent == 1)
					SkillPlaySound(SKILL_SOUND_SKILL_VENGEANCE1, pX, pY, pZ);
				else
					SkillPlaySound(SKILL_SOUND_SKILL_VENGEANCE2, pX, pY, pZ);
			}
			break;
		}
		break;
	case snCHAR_SOUND_REVIVED_FIGTHER:		// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ÆÄÀÌÅÍ
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'G':
				//¾îº¥Â¡ Å©·¡½¬
				AssaParticle_AvangingCrash(this);
				SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
				break;
			case 'Z':	// µð½ºÆ®·ÎÀÌ¾î
				if (GetAttackPoint(&pos.x, &pos.y, &pos.z) == TRUE)
				{
					SkillWarriorDestroyerHit(&pos);                       //Å¸°Ý
					if (MotionEvent < 3)
						SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
					else
						SkillPlaySound(SKILL_SOUND_SKILL_EXPANSION2, pX, pY, pZ);
				}
				break;
			}
		}
		break;
	case snCHAR_SOUND_REVIVED_MECANICIAN:	// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ¸ÞÄ«´Ï¼Ç
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'O':	// ½ºÆÄÅ©
				StartSkillDest(pX, pY + 24 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ, SKILL_SPARK, 8);
				SkillPlaySound(SKILL_SOUND_SKILL_SPARK2, pX, pY, pZ);
				break;
			case 'Z':	// ±×·£µå ½º¸Å½¬
				if (GetAttackPoint(&x, &y, &z) == TRUE)
				{
					// µµ±¸ °ø°Ý
					StartSkill(x, y, z, 0, Angle.y, 0, SKILL_GREAT_SMASH);
					SkillPlaySound(SKILL_SOUND_SKILL_SWING1, pX, pY, pZ);
				}
				break;
			}
		}
		break;
	case snCHAR_SOUND_REVIVED_PIKEMAN:		// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ÆÄÀÌÅ©¸Ç
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'G':	// Á¡ÇÎ Å©·¡½¬
				if (GetAttackPoint(&x, &y, &z) == TRUE)
				{
					StartEffect(x, y, z, EFFECT_CRITICAL_HIT1);
					SetDynLight(x, y, z, 100, 0, 0, 0, 400);
					SkillPlaySound(SKILL_SOUND_JUMPING_CRASH2, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
				}
				break;
			case 'Z':	// Ã¼ÀÎ ·£½º
				if (GetAttackPoint(&x, &y, &z) == TRUE)
				{
					pos.x = x;
					pos.y = y;
					pos.z = z;
					AssaParticle_ChainLance(&pos);
					switch (MotionEvent)
					{
					case 1:
						SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE1, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
						break;
					case 2:
						SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE2, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
						break;
					case 3:
						SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE3, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
						break;
					}
				}
				break;
			}
		}
		break;
	case snCHAR_SOUND_REVIVED_PRIESTESS:	// pluto ½Å±ÔÇÊµå ¸ó½ºÅÍ µÇ»ì¾Æ³­ ÇÁ¸®½ºÆ¼½º
		if (chrAttackTarget)
		{
			switch (MotionInfo->KeyCode)
			{
			case 'O':	// ¸ÖÆ¼ ½ºÆÄÅ©
				sinEffect_MultiSpark(this, chrAttackTarget, 5);
				SkillPlaySound(SKILL_SOUND_SKILL_MULTISPARK, pX, pY, pZ);
				break;
			case 'H':	// ºñ°Åº¼
				AssaParticle_VigorBall(this, chrAttackTarget);
				switch (rand() % 2)
				{
				case 0:
					SkillPlaySound(SKILL_SOUND_SKILL_VIGOR_BALL1, pX, pY, pZ);
					break;
				case 1:
					SkillPlaySound(SKILL_SOUND_SKILL_VIGOR_BALL2, pX, pY, pZ);
					break;
				}
				break;
			case 'Z':	// ±Û·¡¼È ½ºÆÄÀÌÅ©
				Angle.y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
				SkillCelestialGlacialSpike(this);
				GetMoveLocation(0, 0, 64 * fONE, 0, Angle.y, 0);
				SetDynLight(pX + GeoResult_X, pY, pZ + GeoResult_Z, 0, 0, 100, 0, 700);
				if (rand() % 2)
					SkillPlaySound(SKILL_SOUND_SKILL_GLACIAL_SPIKE1, pX, pY, pZ);
				else
					SkillPlaySound(SKILL_SOUND_SKILL_GLACIAL_SPIKE2, pX, pY, pZ);

				if (chrAttackTarget == lpCurPlayer)
				{
					lpCurPlayer->PlaySlowCount = 90;			//1ÃÊµ¿¾È ´À¸®°Ô ¿òÁ÷ÀÓ
					lpCurPlayer->AttackIce -= 160;
				}
				break;
			}
		}
		break;
	case snCHAR_SOUND_HOBOGOLEM:	// pluto Ãß°¡ ¸ó½ºÅÍ È£º¸°í·½
		if (chrAttackTarget)
		{
			GetMoveLocation(7000, 30 * fONE, 50 * fONE, 0, Angle.y, 0);
			StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ,
				MONSTER_DMACHINE_MISSILE1);
		}
		break;

		////////////////// // ¹ÚÀç¿ø - ³¡¾ø´Â Å¾ 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) /////////////////////
	case snCHAR_SOUND_SPRIN: // ½ºÇÁ¸° : ½ºÅ³ °ø°Ý
		if (chrAttackTarget && MotionInfo->KeyCode == 'J')
		{
			ParkAssaParticle_HellHoundSkillAttack(this);
		}
		break;
	case snCHAR_SOUND_XETAN: // Á¦Åº : ½ºÅ³ °ø°Ý
		if (chrAttackTarget && MotionInfo->KeyCode == 'J')
		{
			ParkAssaParticle_DarkMageSkill(chrAttackTarget);
		}
		break;
	case snCHAR_SOUND_RUCA: // ·çÄ« : ½ºÅ³ °ø°Ý
		if (chrAttackTarget && MotionInfo->KeyCode == 'J')
		{
			GetMoveLocation(0, 0, 64 * fONE, 0, Angle.y, 0);
			pos.x = pX + GeoResult_X;
			pos.y = pY + GeoResult_Y + 32 * fONE;
			pos.z = pZ + GeoResult_Z;
			AssaParticle_MonsterIronHit1(&pos);
		}
		break;
	case snCHAR_SOUND_NAZSENIOR: // ³ªÁî ½Ã´Ï¾î : ½ºÅ³ °ø°Ý
		if (chrAttackTarget && MotionInfo->KeyCode == 'k')
		{
			GetMoveLocation(0, 0, 100 * fONE, 0, Angle.y, 0);
			StartSkillDest(pX, pY, pZ, pX + GeoResult_X, pY, pZ + GeoResult_Z, SKILL_TORNADO2, 1);
		}
		break;
	case snCHAR_SOUND_UNDEADMAPLE: // ¾ðµ¥µå ¸ÞÀÌÇÃ : ½ºÅ³ °ø°Ý
		if (chrAttackTarget && MotionInfo->KeyCode == 'J')
		{
			AssaParticle_EvilPlantShot(this);
		}
		break;


		// Àåº° - ¼ö¹Ú¸ó½ºÅÍ ±â¼úµ¿ÀÛ
	case snCHAR_SOUND_WATERMELON:

		pos.x = pX;
		pos.y = pY;
		pos.z = pZ;
		pos2.x = chrAttackTarget->pX;
		pos2.y = chrAttackTarget->pY + 32 * fONE;
		pos2.z = chrAttackTarget->pZ;

		AssaParticle_IceShot(&pos, &pos2);
		break;
		/////////////////////////////////////////////////////////////////////////////////////
	}

	PlayAttack();									//°ø°Ý Àû¿ë
	CharPlaySound(this);

	return TRUE;
}

//½ºÅ³µ¿ÀÛÁ¾·á
int smCHAR::EndSkill()
{
	int	point = 0;
	int	param = 0;

	point = (AttackSkil >> 8) & 0xF;
	param = (AttackSkil >> 12) & 0xF;

	switch (AttackSkil & 0xFF)
	{
	case SKILL_PLAY_DIASTROPHISM:
		if (this == lpCurPlayer && MotionEvent < 3) {
			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
			dm_SelectRange(pX, pY, pZ, Diastrophism_Area[point - 1], FALSE);

			//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
			dm_SendRangeDamage(pX, pY, pZ, 0,
				cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
				cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
				0, 0, AttackSkil);
		}
		break;
		/*
			case SKILL_PLAY_SHADOW_MASTER:
				if ( lpCurPlayer==this && MotionEvent<Shadow_Master_ShadowNum[point-1] && chrAttackTarget ) {
					cnt = dm_SendTransDamage( chrAttackTarget , 0,0 , 0, 0, lpCurPlayer->AttackSkil , TRUE );
					if ( !cnt )
						lpCurPlayer->AttackCritcal = -1;				//ºø³ª°¨
					else
						lpCurPlayer->AttackCritcal = 0;
				}
				break;
		*/
	case SKILL_PLAY_METEO:
		if (chrAttackTarget && point) {
			if (this == lpCurPlayer && MotionEvent < 4)
			{
				dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, Meteo_Area[point - 1], FALSE);
				dm_SendRangeDamage(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 0, 0, 0, 0, 0, lpCurPlayer->AttackSkil);
			}
		}
		break;

	case SKILL_PLAY_CHAIN_LIGHTNING:
		if (chrAttackTarget && point)
		{
			if (this == lpCurPlayer && MotionEvent < 4)
			{
				dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, Chain_Lightning_Range[point - 1], FALSE);

				dm_SendRangeDamage(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 0, 0, 0, 0, 0, lpCurPlayer->AttackSkil);
			}
		}
		break;
	}

	MotionEvent++;

	return TRUE;
}

//½ºÅ³ÀÌº¥Æ®
int smCHAR::EventSkill()
{
	int x, y, z;
	int	point;
	int	param;
	int	damage;
	int	temp;
	int cnt;
	POINT3D	Pos1, Pos2;
	RECT rect;
	smCHAR* lpChar;

	if (smCharInfo.State == smCHAR_STATE_ENEMY && PkMode_CharState != smCHAR_STATE_USER) {
		return EventSkill_Monster();
	}

	point = (AttackSkil >> 8) & 0xF;
	param = (AttackSkil >> 12) & 0xF;


	switch (AttackSkil & 0xFF)
	{
	case SKILL_PLAY_EXTREME_SHIELD:
		//ÀÍ½ºÆ®¸² ½¯µå
		if (HvLeftHand.PatTool) {
			if (GetToolBipPoint(&HvLeftHand, &x, &y, &z)) {
				StartSkill(x, y, z, 0, 0, 0, SKILL_EXTREME_SHIELD);
			}
		}
		break;

	case SKILL_PLAY_MECHANIC_BOMB:
		//¸ÞÄ«´ÐºÕ
		if (chrAttackTarget) {
			if (point) {
				damage = GetRandomPos(Mechanic_Bomb_Damage[point - 1][0], Mechanic_Bomb_Damage[point - 1][1]) / 2;
				StartSkillDest(pX, pY + 30 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, SKILL_MECHANIC_BOMB, point);
				SkillPlaySound(SKILL_SOUND_MECHANIC_BOMB1, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½

				if (this == lpCurPlayer) {
					/*
					Skil_RangeAttack( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, (80*fONE)+(point*5*fONE) , damage , 0 );
					//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ
					QuickSendTransAttack( chrAttackTarget , damage );
					*/
					//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
					dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 + (point * 5), FALSE);
					//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
					dm_SendRangeDamage(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil);
				}
			}
		}
		break;
	case SKILL_PLAY_PHYSICAL_ABSORB:
		//ÇÈÁöÅ¬ ¿ÉÁ®ºê
		break;

	case SKILL_PLAY_RAVING:
		//½ºÅ³ °ø°Ý
		return FALSE;

	case SKILL_PLAY_IMPACT:
		//ÆÄÀÌÅÍ ÀÓÆÑÆ®
		if (this == lpCurPlayer && chrAttackTarget && MotionEvent < 2) {
			if (AttackCritcal >= 0) {
				temp = smCharInfo.Attack_Rating;
				smCharInfo.Attack_Rating += Impact_Attack_Rating[point - 1];
				//°ø°Ý Àü¼Û
				//cnt = SendTransAttack( chrAttackTarget , smWsockServer , 0 , Impact_Damage[point-1] );
				cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);
				smCharInfo.Attack_Rating = temp;

				if (cnt > 0) {
					DamageExp(chrAttackTarget, cnt);					//µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ »ó½Â
					if (AttackExp) {

						AttackExp = 0;
					}
				}
			}
		}
		return FALSE;

	case SKILL_PLAY_PIKEWIND:
		//ÆÄÀÌÅ© À©µå
		if (point) {
			StartSkill(pX, pY, pZ, 0, 0, 0, SKILL_PIKE_WIND, point);
			SkillPlaySound(SKILL_SOUND_PIKE_WIND, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
		}
		break;

	case SKILL_PLAY_CRITICAL_HIT:
		//Å©¸®Æ¼ÄÃ Èý
		return FALSE;

	case SKILL_PLAY_JUMPING_CRASH:
		//½ºÅ³ °ø°Ý
		if (AttackSkil >= 0)
			SkillPlaySound(SKILL_SOUND_JUMPING_CRASH2, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
		return FALSE;


	case SKILL_PLAY_WIND_ARROW:
		//À©µå ¾Ö·Î¿ì
		if (chrAttackTarget) {
			GetAttackPoint(&x, &y, &z);
			StartSkillDest(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY + (24 * fONE), chrAttackTarget->pZ, SKILL_WIND_ARROW);
			SkillPlaySound(SKILL_SOUND_WIND_ARROW, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
		}
		break;

	case SKILL_PLAY_PERFECT_AIM:
		//ÆÛÆåÆ® ¾Ï
		if (chrAttackTarget) {
			GetAttackPoint(&x, &y, &z);
			StartSkillDest(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY + (24 * fONE), chrAttackTarget->pZ, SKILL_PERFECT_AIM);
			SkillPlaySound(SKILL_SOUND_PERFECT_AIM, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
		}
		break;

		////////////////////////////////// 2Â÷ ½ºÅ³ ///////////////////////////////
	case SKILL_PLAY_GREAT_SMASH:
		//±×·¹ÀÌÆ® ½º¸Å½Ã
		if (GetAttackPoint(&x, &y, &z) == TRUE) {
			if (dwActionItemCode) {		//HvRightHand.PatTool ) {
				// µµ±¸ °ø°Ý
				StartSkill(x, y, z, 0, Angle.y, 0, SKILL_GREAT_SMASH);
				SkillPlaySound(SKILL_SOUND_SKILL_SWING1, pX, pY, pZ);
			}
		}
		break;
		/*
			case SKILL_PLAY_MAXIMIZE:
				//¸Æ½Ã¸¶ÀÌÁî
				SkillPlaySound( SKILL_SOUND_SKILL_MAXIMIZE2 , pX,pY,pZ );
				break;

			case SKILL_PLAY_AUTOMATION:
				//¿ÀÅä¸ÅÀÌ¼Ç
				SkillPlaySound( SKILL_SOUND_SKILL_AUTOMATION3 , pX,pY,pZ );
				break;
		*/
	case SKILL_PLAY_SPARK:
		//½ºÆÄÅ©
		if (chrAttackTarget) {
			if (point) {
				//½ºÆÄÅ©
				StartSkillDest(pX, pY + 24 * fONE, pZ, chrAttackTarget->pX, chrAttackTarget->pY + 24 * fONE, chrAttackTarget->pZ, SKILL_SPARK, point);
				//SkillPlaySound( SKILL_SOUND_MECHANIC_BOMB1 , pX,pY,pZ );			//½ºÅ³ È¿°úÀ½

				if (this == lpCurPlayer) {
					/*
					damage = GetRandomPos( Spark_Damage[point-1][0] , Spark_Damage[point-1][1] )/2;
					Skil_RangeAttack( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, Spark_Area[point-1]*fONE , damage , 0 );
					//»ó´ë Ä³¸¯ÅÍ¿¡ °ø°Ý °¡ÇÔ
					QuickSendTransAttack( chrAttackTarget , damage );
					*/
					//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
					dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 + (point * 5), FALSE);
					//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
					dm_SendRangeDamage(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 0, 0, 0, 0, 0, lpCurPlayer->AttackSkil);
					//dm_SendRangeDamage( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, chrAttackTarget ,0, 0, 0, 0, lpCurPlayer->AttackSkil );
				}
				SkillPlaySound(SKILL_SOUND_SKILL_SPARK2, pX, pY, pZ);
			}
		}
		break;

	case SKILL_PLAY_GRAND_SMASH:
		//±×·£µå ½º¸Å½¬
		//if ( GetAttackPoint( &x,&y,&z )==TRUE ) {
		if (dwActionItemCode && AttackCritcal >= 0) {
			// µµ±¸ °ø°Ý
			AssaParticle_GrandSmash(this);
			SkillPlaySound(SKILL_SOUND_SKILL_SWING2, pX, pY, pZ);
		}
		else {
			//¹Ì½º ¼Ò¸®
			//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
			PlayWaponSoundDirect(pX, pY, pZ, 13);
		}
		//}

		if (this == lpCurPlayer && chrAttackTarget && MotionEvent == 1) {
			temp = smCharInfo.Attack_Rating;

			int iAdd = (temp * Grand_Smash_AttackRate[point - 1]) / 100;

			smCharInfo.Attack_Rating += iAdd;

			cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);
			smCharInfo.Attack_Rating = temp;

			if (!cnt)
				AttackCritcal = -1;				//ºø³ª°¨
			else
				AttackCritcal = 0;					//¸íÁß
		}

		break;

	case SKILL_PLAY_SPARK_SHIELD:
		//½ºÆÄÅ© ½¯µå
		if (HvLeftHand.PatTool) {
			/*
			if ( GetToolBipPoint( &HvLeftHand , &x,&y,&z ) ) {
				Pos1.x = x;
				Pos1.y = y;
				Pos1.z = z;
				*/
			AssaParticle_SparkShieldStart(this, Spark_Shield_Time[point - 1]);
			SkillPlaySound(SKILL_SOUND_SKILL_SPARK3, pX, pY, pZ);
			//}
		}
		break;


	case SKILL_PLAY_GROUND_PIKE:
		//±×¶ó¿îµå ÆÄÀÌÅ©
		if (point) {
			GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);
			StartSkill(pX + GeoResult_X, pY, pZ + GeoResult_Z, 0, Angle.y, 0, SKILL_GROUND_PIKE, point);
			SetDynLight(pX, pY, pZ, 0, 0, 100, 0, 700);

			SkillPlaySound(SKILL_SOUND_SKILL_GROUND_PIKE, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_EXPANSION:
		//ÀÍ½ºÆÒ¼Ç
		SkillPlaySound(SKILL_SOUND_SKILL_EXPANSION2, pX, pY, pZ);
		if (GetAttackPoint(&x, &y, &z) == TRUE) {
			if (dwActionItemCode) {
				StartSkill(x, y, z, 0, Angle.y, 0, SKILL_GREAT_SMASH);
				//StartEffect( x,y,z , EFFECT_CRITICAL_HIT1 );
				SetDynLight(x, y, z, 100, 0, 0, 0, 400);
			}
		}
		return FALSE;


	case SKILL_PLAY_TORNADO:
		//Åä³×ÀÌµµ
		if (chrAttackTarget) {
			x = (pX - chrAttackTarget->pX) >> FLOATNS;
			z = (pZ - chrAttackTarget->pZ) >> FLOATNS;

			if ((x * x + z * z) < (50 * 50)) {
				y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);

				GetMoveLocation(0, 0, 50 * fONE, 0, y, 0);
				x = pX + GeoResult_X;
				z = pZ + GeoResult_Z;
				y = chrAttackTarget->pY;
				StartSkillDest(pX, pY, pZ, x, y, z, SKILL_TORNADO, point);

			}
			else {
				StartSkillDest(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, SKILL_TORNADO, point);
			}

			SkillPlaySound(SKILL_SOUND_SKILL_TORNADO1, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_VENOM_SPEAR:
		//º£³ð ½ºÇÇ¾î


		if (this == lpCurPlayer) {
			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (µð¹ÙÀÎ ¶óÀÌÆ®´×)
			dm_SelectDamageCount(this, VenomSpear_Num[point - 1], VenomSpear_Range[point - 1]);
			//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
			dm_SendRangeDamage(pX, pY, pZ, 0, smCharInfo.Attack_Damage[0], smCharInfo.Attack_Damage[1], 0, 0, AttackSkil);
		}
		SkillPlaySound(SKILL_SOUND_SKILL_VENOM_SPEAR1, pX, pY, pZ);

		if (SkillPlay_VenomSpear_Effect(this, point) > 0) {
			SkillPlaySound(SKILL_SOUND_SKILL_VENOM_SPEAR2, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_VANISH:
		SkillPlaySound(SKILL_SOUND_SKILL_VANISH2, pX, pY, pZ);

		if (lpCurPlayer == this)
		{
			PlayVanish = Vanish_Time[point - 1] * 70;
			PlayVague = 0;
			SendProcessSKillToServer(SKILL_PLAY_VANISH, point, 0, 0);
			dwM_BlurTime = dwPlayTime + Vanish_Time[point - 1] * 1000;
		}
		break;

	case SKILL_PLAY_CHAIN_LANCE:
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			if (lpCurPlayer->AttackCritcal >= 0) {
				AssaParticle_ChainLance(&Pos1);

				switch (MotionEvent) {
				case 1:
					SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE1, pX, pY, pZ);
					break;
				case 2:
					SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE2, pX, pY, pZ);
					break;
				case 3:
					SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE3, pX, pY, pZ);
					break;
				}
			}
			else {

				PlayWaponSoundDirect(pX, pY, pZ, 13);
			}
		}

		if (this == lpCurPlayer && MotionEvent < 3 && chrAttackTarget) {
			//¸íÁß·Â º¸°­
			//x = smCharInfo.Attack_Rating;
			//smCharInfo.Attack_Rating += Chain_Lance_AttackRate[point-1];

			cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, AttackSkil, TRUE);
			if (!cnt)
				lpCurPlayer->AttackCritcal = -1;				//ºø³ª°¨
			else
				lpCurPlayer->AttackCritcal = 0;					//¸íÁß

			//smCharInfo.Attack_Rating = x;
		}
		break;

	case SKILL_PLAY_FALCON:
		//ÆÈÄÜ
		break;

	case SKILL_PLAY_ARROWOF_RAGE:
		//¿¤·Î¿ì¿Àºê·¹ÀÌÁö
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			StartSkillDest(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, SKILL_ARROW_OF_RAGE, point);

			/*
			//¿¡·Î¿ì ¿Àºê ·¹ÀÌÁö
			int A_Rage_ArrowNum[10]   = {2,3,4,5,6,7,8,9,10,11};
			int A_Rage_Damage[10]     = {4,4,4,4,3,3,3,3,2,2};
			int A_Rage_Area[10]	  = {60,65,70,75,80,85,90,95,100,110};
			*/
			if (this == lpCurPlayer && chrAttackTarget) {
				//°ø°Ý Àü¼Û
				/*
				cnt = GetRandomPos( smCharInfo.Attack_Damage[0] , smCharInfo.Attack_Damage[1] );
				cnt += smCharInfo.Level/A_Rage_Damage[ point-1 ];
				Skil_RangeAttack( chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, A_Rage_Area[point-1]*fONE , cnt , 0 );
				*/
				//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
				dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, A_Rage_Area[point - 1], FALSE);
				//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
				dm_SendRangeDamage(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 0, 0, 0, 0, 0, lpCurPlayer->AttackSkil);

			}
			SkillPlaySound(SKILL_SOUND_WIND_ARROW, pX, pY, pZ);			//½ºÅ³ È¿°úÀ½
		}
		break;

	case SKILL_PLAY_AVALANCHE:
		//¾Æ¹ß¶õÃ¼
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {

			if (AttackCritcal >= 0) {
				StartSkillDest(x, y, z, chrAttackTarget->pX, chrAttackTarget->pY + (24 * fONE), chrAttackTarget->pZ, SKILL_AVALANCHE);

				switch (rand() % 3) {
				case 0:
					SkillPlaySound(SKILL_SOUND_SKILL_AVALANCHE1, pX, pY, pZ);
					break;
				case 1:
					SkillPlaySound(SKILL_SOUND_SKILL_AVALANCHE2, pX, pY, pZ);
					break;
				case 2:
					SkillPlaySound(SKILL_SOUND_SKILL_AVALANCHE3, pX, pY, pZ);
					break;
				}
			}
			else
				WeaponPlaySound(this);

		}
		break;

	case SKILL_PLAY_ELEMENTAL_SHOT:
		//¿¤¸®¸àÅ» ¼¦
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			if (AttackCritcal >= 0) {

				Pos1.x = x;
				Pos1.y = y;
				Pos1.z = z;

				Pos2.x = chrAttackTarget->pX;
				Pos2.y = chrAttackTarget->pY + (24 * fONE);
				Pos2.z = chrAttackTarget->pZ;

				if (AttackCritcal >= 0) {
					switch (param) {
					case 0:
						AssaParticle_ElementalShotFire(&Pos1, &Pos2);

						/*
												//È£µ¿¾¾°¡ ³ÖÀÚ
													if ( (rand()%2)==0 )
														SkillPlaySound( SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING1	 , pX,pY,pZ );
													else
														SkillPlaySound( SKILL_SOUND_SKILL_ENCHANTWEAPON_LIGHTNING2	 , pX,pY,pZ );
													break;
												case (sITEMINFO_FIRE+1):
													AssaParticle_FireHit1( &pos );
													SetDynLight( x,y,z , 100, 0,0,0, 400 );
													if ( (rand()%2)==0 )
														SkillPlaySound( SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE1	 , pX,pY,pZ );
													else
														SkillPlaySound( SKILL_SOUND_SKILL_ENCHANTWEAPON_FIRE2	 , pX,pY,pZ );
													break;
						*/
						break;
					case 1:
						AssaParticle_ElementalShotLighting(&Pos1, &Pos2);
						break;
					}
					SkillPlaySound(SKILL_SOUND_SKILL_ELEMENTAL_SHOT, pX, pY, pZ);
				}
			}
		}
		break;

	case SKILL_PLAY_BOMB_SHOT:
		//¹ü¼¦
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			if (AttackCritcal >= 0) {
				Pos1.x = x;
				Pos1.y = y;
				Pos1.z = z;

				Pos2.x = chrAttackTarget->pX;
				Pos2.y = chrAttackTarget->pY + (24 * fONE);
				Pos2.z = chrAttackTarget->pZ;

				AssaParticle_BombShot(&Pos1, &Pos2);

				SkillPlaySound(SKILL_SOUND_SKILL_BOMB_SHOT1, pX, pY, pZ);

			}
		}
		break;

	case SKILL_PLAY_PERFORATION:
		//ÆÛÆ÷·¹ÀÌ¼Ç
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			if (AttackCritcal >= 0) {
				Pos1.x = x;
				Pos1.y = y;
				Pos1.z = z;

				Pos2.x = chrAttackTarget->pX;
				Pos2.y = chrAttackTarget->pY + (24 * fONE);
				Pos2.z = chrAttackTarget->pZ;

				AssaParticle_Perforation(&Pos1, &Pos2);
				SkillPlaySound(SKILL_SOUND_SKILL_PERFORATION, pX, pY, pZ);

				rect.left = -40;
				rect.right = 40;
				rect.top = 0;
				rect.bottom = 240;
				rect.bottom += 20;
				rect.bottom += 20;

				//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¹Ú½º)
				dm_SelectRangeBox(this, &rect, FALSE);

				for (cnt = 0; cnt < dmSelected_CharCnt; cnt++) {
					if (lpSelected_Char[cnt]) {
						AssaParticle_PerforationHit(lpSelected_Char[cnt], this, 0.5f);
					}
				}
			}
		}
		break;

	case SKILL_PLAY_TRIPLE_IMPACT:
		//Æ®¸®ÇÃ ÀÓÆÑÆ®
		if (AttackCritcal >= 0) {
			if (GetAttackPoint(&x, &y, &z) == TRUE) {
				StartSkill(x, y, z, 0, 0, 0, SKILL_TRIPLE_IMPACT);
				SetDynLight(x, y, z, 100, 0, 0, 0, 400);
			}

			AttackCritcal = 1;
		}
		WeaponPlaySound(this);

		if (this == lpCurPlayer && chrAttackTarget && MotionEvent < T_Impact_Hit[point - 1]) {
			if (AttackCritcal >= 0) {
				/*
								if ( (rand()%100)<sinGetCritical( chrAttackTarget->smCharInfo.Level ) )
									AttackCritcal = TRUE;			//Å©¸®Æ¼ÄÃ °ø°Ý ¼º°ø
								else
									AttackCritcal = FALSE;

								if ( AttackCritcal==FALSE ) {

									if ( MotionEvent==1 )
										AttackCritcal = TRUE;

									//°ø°Ý Àü¼Û
									cnt = SendTransAttack( chrAttackTarget , smWsockServer , 0 , T_Impact_Damage[point-1] );

									AttackCritcal = FALSE;
								}
								else {
									//°ø°Ý Àü¼Û
									cnt = SendTransAttack( chrAttackTarget , smWsockServer , 0 , T_Impact_Damage[point-1] );
								}
				*/
				cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);

				if (cnt > 0) {
					DamageExp(chrAttackTarget, cnt);					//µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ »ó½Â
					if (AttackExp) {

						AttackExp = 0;
					}
				}
			}
		}
		break;

	case SKILL_PLAY_BRUTAL_SWING:
		//ºê·çÅ» ½ºÀ®
		if (AttackCritcal >= 0) {
			if (GetAttackPoint(&x, &y, &z) == TRUE) {
				StartSkill(x, y, z, 0, 0, 0, SKILL_BRUTAL_SWING);
				SetDynLight(x, y, z, 100, 0, 0, 0, 400);
				SkillPlaySound(SKILL_SOUND_SKILL_SWING1, pX, pY, pZ);
			}
		}
		else	//¹Ì½º
			WeaponPlaySound(this);
		break;

	case SKILL_PLAY_RAGEOF_ZECRAM:
		if (GetAttackPoint(&x, &y, &z) == TRUE) {
			if (MotionEvent == 1) {
				StartSkill(x, y, z, 0, 0, 0, SKILL_RAGE_OF_ZECRAM_HIT1);
				//ÀÌÆåÆ® ¿¹¾à Ãß°¡ ( 0.6ÃÊÈÄ )
				AddTimeEffect(SKILL_RAGE_OF_ZECRAM_HIT2, dwPlayTime + 600, x, y, z);

				if (this == lpCurPlayer && chrAttackTarget) {
					//¸¶Áö¸· °ø°Ý ¹Ì¸® º¸³¿
					/*
					cnt = GetRandomPos( smCharInfo.Attack_Damage[0] , smCharInfo.Attack_Damage[1] );
					cnt += (cnt*R_Zecram_Damage[point-1])/100;
					cnt >>=1;
					Skil_RangeAttack( lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 70*fONE , cnt , 100 );	//100Àº ºÒ ¼Ó¼º
					*/
					//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
					dm_SelectRange(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 70, FALSE);
					//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
					dm_SendRangeDamage(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, 0, 0, lpCurPlayer->AttackSkil);

					SkillPlaySound(SKILL_SOUND_SKILL_RAGE_OF_ZECRAM1, pX, pY, pZ, FALSE);
					SetDynLight(x, y, z, 100, 0, 0, 0, 400);

				}
			}
		}
		break;
	case SKILL_PLAY_ROAR:
		//·Î¾î
		//Skil_RangeEffect( this , Roar_Range[point-1] , SKILL_PLAY_ROAR , 0 , point );
		break;

	case SKILL_PLAY_AVANGING_CRASH:
		//¾îº¥Â¡ Å©·¡½¬
		if (GetAttackPoint(&x, &y, &z) == TRUE) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;

			if (AttackCritcal >= 0) {
				AssaParticle_AvangingCrash(this);
				SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
			}
			else {
				//¹Ì½º ¼Ò¸®
				//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
				PlayWaponSoundDirect(pX, pY, pZ, 13);
			}
		}

		if (this == lpCurPlayer && MotionEvent == 1 && chrAttackTarget) {
			//¸íÁß·Â Áõ°¡
			x = lpCurPlayer->smCharInfo.Attack_Rating;

			smCharInfo.Attack_Rating += (smCharInfo.Attack_Rating * A_Crash_AttackRate[point - 1]) / 100;

			cnt = dm_SendTransDamage(chrAttackTarget, CSKILL->GetSkillPoint(SKILL_BRUTAL_SWING), 0, 0, 0, AttackSkil, TRUE);

			if (!cnt)
				AttackCritcal = -1;				//ºø³ª°¨
			else {
				//µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ »ó½Â
				DamageExp(chrAttackTarget, cnt);
				AttackCritcal = 0;
			}
			smCharInfo.Attack_Rating = x;
		}

		break;

	case SKILL_PLAY_BONE_SMASH:
		//º»½º¸Å½¬
		/*
		if ( GetAttackPoint( &x,&y,&z )==TRUE ) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			*/
		if (AttackCritcal >= 0) {
			AssaParticle_BoneSmash1(this);
			SkillPlaySound(SKILL_SOUND_SKILL_BONE_CRASH1, pX, pY, pZ, FALSE);
		}
		else {
			//¹Ì½º ¼Ò¸®
			//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
			PlayWaponSoundDirect(pX, pY, pZ, 13);
		}

		//}

		if (this == lpCurPlayer && MotionEvent == 1 && chrAttackTarget) {
			//°ø°Ý 
			cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, AttackSkil, TRUE);

			if (!cnt)
				AttackCritcal = -1;				//ºø³ª°¨
			else {
				//µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ »ó½Â
				DamageExp(chrAttackTarget, cnt);
				AttackCritcal = 0;
			}
		}

		break;

		//////////////////////////////// ³ªÀÌÆ® ///////////////////////////////
	case SKILL_PLAY_HOLY_BODY:
		break;

	case SKILL_PLAY_SWORD_BLAST:

		if (chrAttackTarget) {
			sinEffect_Sword_Blast(this, chrAttackTarget);

			SkillPlaySound(SKILL_SOUND_SKILL_SWORDBLAST2, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_DOUBLE_CRASH:
		if (AttackCritcal >= 0) {
			if (GetAttackPoint(&x, &y, &z) == TRUE) {
				if (AttackCritcal == 0) {
					StartEffect(x, y, z, EFFECT_NORMAL_HIT1);
					SetDynLight(x, y, z, 80, 0, 0, 0, 300);
				}
				else {
					StartEffect(x, y, z, EFFECT_CRITICAL_HIT1);
					SetDynLight(x, y, z, 100, 0, 0, 0, 400);
				}
			}
		}
		WeaponPlaySound(this);

		if (this == lpCurPlayer && chrAttackTarget && MotionEvent < 2) {
			/*
						if ( (rand()%100)<(sinGetCritical( chrAttackTarget->smCharInfo.Level )+D_Crash_Critical[point-1] ))
							lpCurPlayer->AttackCritcal = TRUE;			//Å©¸®Æ¼ÄÃ °ø°Ý ¼º°ø
						else
							lpCurPlayer->AttackCritcal = FALSE;

						cnt = lpCurPlayer->SendTransAttack( chrAttackTarget , smWsockServer , 0 , D_Crash_Damage[point-1] );
			*/

			cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);

			if (cnt > 0) {
				lpCurPlayer->AttackCritcal = 0;
				DamageExp(chrAttackTarget, cnt);					//µ¥¹ÌÁö¿¡ µû¸¥ °æÇèÄ¡ »ó½Â
				if (AttackExp) {
					AttackExp = 0;
				}
			}
			else
				lpCurPlayer->AttackCritcal = -1;
		}
		break;

		///////////////////////////// 2Â÷ ///////////////////////////
	case SKILL_PLAY_HOLY_VALOR:
		if (this == lpCurPlayer) {
			//ÆÄÆ¼¿ø¿¡°Ô ½ºÅ³ »ç¿ë Á¤º¸ º¸³»±â
			SendPartySkillToServer(SKILL_PLAY_HOLY_VALOR, point, 200, 0, 0, 0, 0);
		}
		break;

	case SKILL_PLAY_BRANDISH:
		sinSkillEffect_Brandish(this);
		SkillPlaySound(SKILL_SOUND_SKILL_BLANDISH2, pX, pY, pZ);
		break;

	case SKILL_PLAY_PIERCING:
		sinSkillEffect_Piercing_Attack(this);
		break;

	case SKILL_PLAY_FLAME_BRANDISH:
		AssaParticle_FlameBrandish(this);
		SkillPlaySound(SKILL_SOUND_SKILL_FLAME_BRANDISH, pX, pY, pZ);

		if (chrAttackTarget) {
			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
			dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80, FALSE);
			for (cnt = 0; cnt < dmSelected_CharCnt; cnt++) {
				if (lpSelected_Char[cnt]) {
					AssaParticle_FlameBrandishHit1(lpSelected_Char[cnt]);
				}
			}
		}
		break;

	case SKILL_PLAY_DIVINE_INHALATION:
		//µð¹ÙÀÎÀÎÈ¦·¹ÀÌ¼Ç
		AssaParticle_DivineInhalationStart(this, D_Inhalation_Time[point - 1]);
		break;

	case SKILL_PLAY_HOLY_INCANTATION:
		if (chrAttackTarget) {
			AssaParticle_HolyIncantationShot(chrAttackTarget);

			SkillPlaySound(SKILL_SOUND_SKILL_HOLY_INCANTATION, pX, pY, pZ);

			if (this == lpCurPlayer)
				dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, FALSE);
		}
		break;

	case SKILL_PLAY_GRAND_CROSS:
		if (chrAttackTarget && AttackCritcal >= 0 && GetAttackPoint(&x, &y, &z) == TRUE) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			if (MotionEvent == 1) {
				AssaParticle_GrandCrossHorz(&Pos1, Angle.y);
				SkillPlaySound(SKILL_SOUND_SKILL_SWING2, pX, pY, pZ);
			}
			else {
				AssaParticle_GrandCrossVert(&Pos1, Angle.y);
				SkillPlaySound(SKILL_SOUND_SKILL_GRAND_CROSS, pX, pY, pZ);
			}
		}
		else {
			//¹Ì½º ¼Ò¸®
			//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
			PlayWaponSoundDirect(pX, pY, pZ, 13);
		}

		if (this == lpCurPlayer && chrAttackTarget && MotionEvent == 1) {

			x = smCharInfo.Attack_Rating;

			smCharInfo.Attack_Rating += (smCharInfo.Attack_Rating * Grand_Cross_AttackRate[point - 1]) / 100;

			cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);

			if (cnt > 0)
				lpCurPlayer->AttackCritcal = 0;
			else
				lpCurPlayer->AttackCritcal = -1;

			smCharInfo.Attack_Rating = x;
		}

		break;



		//////////////////////////////// ¾ÆÅ»¶õÅ¸ ///////////////////////////////
	case SKILL_PLAY_SHIELD_STRIKE:
		SkillPlaySound(SKILL_SOUND_SKILL_SHIELDSTRIKE, pX, pY, pZ);
		break;

	case SKILL_PLAY_VIGOR_SPEAR:
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			sinEffect_VigorSpear(this, chrAttackTarget);

			SkillPlaySound(SKILL_SOUND_SKILL_VIGORSPEAR, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_FARINA:
		if (chrAttackTarget && AttackCritcal >= 0)
			SkillPlaySound(SKILL_SOUND_SKILL_FARINA, pX, pY, pZ);

		return FALSE;

	case SKILL_PLAY_TWIST_JAVELIN:
		if (chrAttackTarget && AttackCritcal >= 0 && GetAttackPoint(&x, &y, &z) == TRUE) {

			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 28 * fONE;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_TwistJavelinShot(&Pos1, &Pos2);
			SkillPlaySound(SKILL_SOUND_SKILL_TWISTJAVELIN, pX, pY, pZ);
		}
		return FALSE;
		//break;
	case SKILL_PLAY_SOUL_SUCKER:
		//¹üÀ§Çü °ø°Ý ÀÌÆåÆ® ¿¬Ãâ
		rect.left = -30;
		rect.right = 30;
		rect.top = 0;
		rect.bottom = 200;
		if (Skil_RangeBoxEffect(this, &rect, SKILL_PLAY_SOUL_SUCKER, 0) > 0)
			SkillPlaySound(SKILL_SOUND_SKILL_SOULSUCKER2, pX, pY, pZ);
		else
			SkillPlaySound(SKILL_SOUND_SKILL_SHIELDSTRIKE, pX, pY, pZ);
		break;

	case SKILL_PLAY_FIRE_JAVELIN:
		if (chrAttackTarget && AttackCritcal >= 0 && GetAttackPoint(&x, &y, &z) == TRUE) {

			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 28 * fONE;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_FireJavelin(&Pos1, &Pos2);
			SkillPlaySound(SKILL_SOUND_SKILL_FIREJAVELIN, pX, pY, pZ);
		}
		//break;
		return FALSE;

	case SKILL_PLAY_SPLIT_JAVELIN:
		if (chrAttackTarget && AttackCritcal >= 0 && GetAttackPoint(&x, &y, &z) == TRUE) {
			/*
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY+28*fONE;
			Pos2.z = chrAttackTarget->pZ;
			AssaParticle_SplitJavelin( &Pos1, &Pos2 , Split_Javelin_AttackNum[point-1] );
			*/
			SkillPlaySound(SKILL_SOUND_SKILL_SPLIT_JAVELIN2, pX, pY, pZ);

		}
		return TRUE;

	case SKILL_PLAY_TRIUMPH_OF_VALHALLA:
		if (chrAttackTarget) {
			if (chrAttackTarget->smCharInfo.State == smCHAR_STATE_USER) {
				AssaParticle_TriumphOfValhallaMember(this, chrAttackTarget, T_Of_Valhalla_Time[point - 1]);
				if (this == lpCurPlayer) {
					SendProcessSKillToServer(SKILL_PLAY_TRIUMPH_OF_VALHALLA, point, chrAttackTarget->dwObjectSerial, smCharInfo.Level);
				}
			}
		}
		else {
			AssaParticle_TriumphOfValhalla(this, T_Of_Valhalla_Time[point - 1]);
		}
		break;

	case SKILL_PLAY_LIGHTNING_JAVELIN:
		if (chrAttackTarget && GetAttackPoint(&x, &y, &z) == TRUE) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 28 * fONE;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_LightingJavelin(&Pos1, &Pos2);
			switch (rand() % 2) {
			case 0:
				SkillPlaySound(SKILL_SOUND_SKILL_LIGHTNING_JAVELIN1, pX, pY, pZ);
				break;
			case 1:
				SkillPlaySound(SKILL_SOUND_SKILL_LIGHTNING_JAVELIN2, pX, pY, pZ);
				break;
			}
		}
		break;

	case SKILL_PLAY_STORM_JAVELIN:
		if (chrAttackTarget && AttackCritcal >= 0 && GetAttackPoint(&x, &y, &z) == TRUE) {
			Pos1.x = x;
			Pos1.y = y;
			Pos1.z = z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 28 * fONE;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_StormJavelin(&Pos1, &Pos2);

			rect.left = -30;
			rect.right = 30;
			rect.top = 0;
			rect.bottom = 240;
			rect.bottom += 20;
			rect.bottom += 20;

			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¹Ú½º)
			dm_SelectRangeBox(this, &rect, FALSE);

			for (cnt = 0; cnt < dmSelected_CharCnt; cnt++) {
				if (lpSelected_Char[cnt]) {
					AssaParticle_StormJavelinHit(lpSelected_Char[cnt], this, 0.5f);
				}
			}

			switch (rand() % 2) {
			case 0:
				SkillPlaySound(SKILL_SOUND_SKILL_STORM_JAVELIN1, pX, pY, pZ);
				break;
			case 1:
				SkillPlaySound(SKILL_SOUND_SKILL_STORM_JAVELIN2, pX, pY, pZ);
				break;
			}
		}

		return TRUE;


		//////////////////////////////// ÇÁ¸®½ºÆ¼½º ///////////////////////////////

	case SKILL_PLAY_HOLY_BOLT:
		if (chrAttackTarget) {
			GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);

			StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				chrAttackTarget->pX, chrAttackTarget->pY + 20 * fONE, chrAttackTarget->pZ,
				MONSTER_MEPHIT_SHOT2);

			SkillPlaySound(SKILL_SOUND_SKILL_HOLYBOLT, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_MULTI_SPARK:
		if (chrAttackTarget) {
			sinEffect_MultiSpark(this, chrAttackTarget, param);

			SkillPlaySound(SKILL_SOUND_SKILL_MULTISPARK, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_DIVINE_LIGHTNING:
		//SkillPlay_DivineLightning_Select( this , point );

		if (this == lpCurPlayer) {
			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (µð¹ÙÀÎ ¶óÀÌÆ®´×)
			dm_SelectDamageCount(this, Divine_Lightning_Num[point - 1], 180);
			//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
			dm_SendRangeDamage(pX, pY, pZ, 0,
				cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
				cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
				0, 0, AttackSkil);
		}

		if (SkillPlay_DivineLightning_Effect(this, point) > 0) {
			switch (rand() % 3) {
			case 0:
				SkillPlaySound(SKILL_SOUND_SKILL_DIVINELIGHTNING1, pX, pY, pZ);
				break;
			case 1:
				SkillPlaySound(SKILL_SOUND_SKILL_DIVINELIGHTNING2, pX, pY, pZ);
				break;
			case 2:
				SkillPlaySound(SKILL_SOUND_SKILL_DIVINELIGHTNING3, pX, pY, pZ);
				break;
			}
		}
		break;

	case SKILL_PLAY_HOLY_REFLECTION:
		if (this == lpCurPlayer) {
			SendProcessSKillToServer(SKILL_PLAY_HOLY_REFLECTION, point, 0, 0);
		}
		break;
	case SKILL_PLAY_GREAT_HEALING:
		if (this == lpCurPlayer) {
			/*
			CheckCharForm();
			sinSetLife( sinGetLife()+GetRandomPos(Grand_Healing[point-1][0] , Grand_Healing[point-1][1] ));
			ResetEnergyGraph( 0 );						//¿¡³ÊÁö ±×·¡ÇÁ Ã½Å© ÃÊ±âÈ­
			ReformCharForm();
			*/
			int dist, ddist;

			ddist = Grand_Healing_Range[point - 1];
			ddist *= ddist;

			//ÆÄÆ¼¿øµé Èú¸µ ÀÌÆåÆ®
			for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
				if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial &&
					chrOtherPlayer[cnt].smCharInfo.State == smCHAR_STATE_USER &&
					chrOtherPlayer[cnt].PartyFlag &&
					chrOtherPlayer[cnt].MotionInfo->State != CHRMOTION_STATE_DEAD) {

					x = (chrOtherPlayer[cnt].pX - lpCurPlayer->pX) >> FLOATNS;
					y = (chrOtherPlayer[cnt].pY - lpCurPlayer->pY) >> FLOATNS;
					z = (chrOtherPlayer[cnt].pZ - lpCurPlayer->pZ) >> FLOATNS;
					dist = x * x + y * y + z * z;

					if (dist < ddist) {
						sinSkillEffect_Great_Healing_Party(&chrOtherPlayer[cnt]);
					}
				}
			}

		}
		break;


	case SKILL_PLAY_VIGOR_BALL:
		if (chrAttackTarget) {
			if (this == lpCurPlayer && MotionEvent < 3) {
				cnt = dm_SendTransDamage(chrAttackTarget,
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
					0, 0, AttackSkil, FALSE);
			}

			AssaParticle_VigorBall(this, chrAttackTarget);

			switch (rand() % 2) {
			case 0:
				SkillPlaySound(SKILL_SOUND_SKILL_VIGOR_BALL1, pX, pY, pZ);
				break;
			case 1:
				SkillPlaySound(SKILL_SOUND_SKILL_VIGOR_BALL2, pX, pY, pZ);
				break;
			}
		}
		break;

	case SKILL_PLAY_RESURRECTION:

		if (this == lpCurPlayer) 
		{
			// Encontra o primeiro player morto da PT (inclui raid)
			lpChar = PARTYHANDLER->GetDeadPartyMember();

			if (lpChar) 
			{
				SendProcessSKillToServer(SKILL_PLAY_RESURRECTION, point, lpChar->dwObjectSerial, 0);
				chrAttackTarget = lpChar;
			}
			else
				chrAttackTarget = 0;
		}

		if (chrAttackTarget)
			AssaParticle_ResurrectionShot(chrAttackTarget);

		break;

	case SKILL_PLAY_EXTINCTION:
		break;

	case SKILL_PLAY_VIRTUAL_LIFE:
		if (chrAttackTarget) {
			AssaParticle_VirtualLife(chrAttackTarget, Virtual_Life_Time[point - 1]);
		}
		else {
			AssaParticle_VirtualLife(this, Virtual_Life_Time[point - 1]);
		}
		SkillPlaySound(SKILL_SOUND_SKILL_VIRTUAL_LIFE, pX, pY, pZ);
		break;

		//////////////////////////// ¸ÅÁö¼Ç /////////////////////////////////////

	case SKILL_PLAY_FIRE_BOLT:
		if (chrAttackTarget) {

			GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);

			StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				chrAttackTarget->pX, chrAttackTarget->pY + 20 * fONE, chrAttackTarget->pZ,
				MONSTER_IMP_SHOT2);

			if (((dwPlayTime >> 4) & 1) == 0)
				SkillPlaySound(SKILL_SOUND_SKILL_FIREBOLT1, pX, pY, pZ);
			else
				SkillPlaySound(SKILL_SOUND_SKILL_FIREBOLT2, pX, pY, pZ);

		}
		break;

	case SKILL_PLAY_FIRE_BALL:
		if (chrAttackTarget) {
			GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
			StartEffectMonsterDest(pX + GeoResult_X, pY + GeoResult_Y, pZ + GeoResult_Z,
				chrAttackTarget->pX, chrAttackTarget->pY + 20 * fONE, chrAttackTarget->pZ,
				MONSTER_PIGON_SHOT1);

			if (((dwPlayTime >> 4) & 1) == 0)
				SkillPlaySound(SKILL_SOUND_SKILL_FIREBALL1, pX, pY, pZ);
			else
				SkillPlaySound(SKILL_SOUND_SKILL_FIREBALL2, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_COLUMN_OF_WATER:
		if (chrAttackTarget) {
			Pos1.x = pX;
			Pos1.y = pY;
			Pos1.z = pZ;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_ColumnOfWater(&Pos1, &Pos2, point);

			switch (rand() % 2) {
			case 0:
				SkillPlaySound(SKILL_SOUND_SKILL_WATORNADO1, pX, pY, pZ);
				break;
			case 1:
				SkillPlaySound(SKILL_SOUND_SKILL_WATORNADO2, pX, pY, pZ);
				break;
			}
		}
		break;

	case SKILL_PLAY_ENCHANT_WEAPON:
		if (chrAttackTarget) {
			AssaParticle_EnchantWeaponJinMember(chrAttackTarget); //Member¿¡°Ô ¾²´Â È¿°ú ÀÌÆåÆ®
/*
			//½ÃÇö½Ã ÇØ´ç Ä³¸¯¿¡ Àû¿ë ½ÃÀÛ ÀÌÆåÆ® Ãâ·Â
			//cnt = Enchant_Weapon_Time[(point&0xF)-1];
			switch( (point>>4) ) {
				case 0:
					//AssaParticle_EnchantWeaponIceJang( lpTarChar , cnt );
					break;
				case 1:
					//AssaParticle_EnchantWeaponLightJang( lpTarChar , cnt );
					break;
				case 2:
					//AssaParticle_EnchantWeaponFireJang( lpTarChar , cnt );
					break;
			}
*/
			SendProcessSKillToServer(SKILL_PLAY_ENCHANT_WEAPON, point, chrAttackTarget->dwObjectSerial, param);
		}
		else {
			if (this == lpCurPlayer) {
				SendProcessSKillToServer(SKILL_PLAY_ENCHANT_WEAPON, point, lpCurPlayer->dwObjectSerial, param);
				CSKILL->SetEnchant_Weapon(Enchant_Weapon_Time[point - 1], point);
			}
		}
		break;

	case SKILL_PLAY_DEAD_RAY:
		if (chrAttackTarget) {
			GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
			Pos1.x = pX + GeoResult_X;
			Pos1.y = pY + GeoResult_Y;
			Pos1.z = pZ + GeoResult_Z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 32 * fONE;
			Pos2.z = chrAttackTarget->pZ;

			AssaParticle_DeadLay(&Pos1, &Pos2);
			SkillPlaySound(SKILL_SOUND_SKILL_DEATHRAY, pX, pY, pZ);
		}
		break;


	case SKILL_PLAY_ENERGY_SHIELD:
		AssaParticle_EnergyShield(this, Energy_Shield_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_ENERGY_SHIELD, pX, pY, pZ);
		break;

	case SKILL_PLAY_DIASTROPHISM:
		AssaParticle_Diastrophism(this);
		SkillPlaySound(SKILL_SOUND_SKILL_DIASTROPHISM, pX, pY, pZ);

		if (this == lpCurPlayer && MotionEvent < 2) {
			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
			dm_SelectRange(pX, pY, pZ, Diastrophism_Area[point - 1], FALSE);

			//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
			dm_SendRangeDamage(pX, pY, pZ, 0,
				cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
				cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
				0, 0, AttackSkil);
		}
		break;

	case SKILL_PLAY_SPIRIT_ELEMENTAL:
		AssaParticle_SpiritElemental(this, Spirit_Elemental_Time[point - 1]);
		SkillPlaySound(SKILL_SOUND_SKILL_SPIRIT_ELEMENTAL, pX, pY, pZ);
		break;

	case SKILL_PLAY_DANCING_SWORD:
		switch (param) {
		case 0:		//¾óÀ½
			AssaParticle_DancingSwordIce(this, Dancing_Sword_Time[point - 1], point);
			break;

		case 1:		//ºÒ
			AssaParticle_DancingSwordFire(this, Dancing_Sword_Time[point - 1], point);
			break;
		}
		SkillPlaySound(SKILL_SOUND_SKILL_DANCING_SWORD, pX, pY, pZ);
		break;


		/////////////////// 4Â÷½ºÅ³ ///////////////////
	case SKILL_PLAY_IMPULSION:
		if (point && chrAttackTarget) {

			if (GetAttackPoint(&Pos2.x, &Pos2.y, &Pos2.z) == TRUE) {
				SkillImpulsionLightHit(&Pos2);                     //Å¸°Ý ÀÌÆåÆ®
			}

			if (lpCurPlayer != this) {
				//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¿ø)
				dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, Impulsion_Range[point - 1], TRUE, Impulsion_LightingNum[point - 1]);
			}
			else {
				if (MotionEvent == 1)
					dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, FALSE);
			}

			SkillMetalionImpulsionHit(lpSelected_Char, dmSelected_CharCnt);

			if (dmSelected_CharCnt)
				SkillPlaySound(SKILL_SOUND_SKILL_IMPULSION, pX, pY, pZ);

			SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE2, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_COMPULSION:
		if (point) {
			SkillMetalionCompulsion(this);
		}
		break;

	case SKILL_PLAY_MAGNETIC_SPHERE:
		if (point) {
			SkillMetalionSphere(this, (float)Magnetic_Sphere_Time[point - 1], (float)Magnetic_Sphere_AttackDelay[point - 1]);

			SkillPlaySound(SKILL_SOUND_SKILL_SPARK3, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_METAL_GOLEM:
		if (point) {
			SkillMetalionGolem(this);

		}
		break;

	case SKILL_PLAY_DESTROYER:

		if (chrAttackTarget) {
			if (AttackCritcal >= 0) {
				GetAttackPoint(&Pos2.x, &Pos2.y, &Pos2.z);
				SkillWarriorDestroyerHit(&Pos2);                       //Å¸°Ý
				SetDynLight(Pos2.x, Pos2.y, Pos2.z, 100, 0, 0, 0, 400);

				if (MotionEvent < 3)
					SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
				else
					SkillPlaySound(SKILL_SOUND_SKILL_EXPANSION2, pX, pY, pZ);
			}
			else {
				//¹Ì½º ¼Ò¸®
				//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
				PlayWaponSoundDirect(pX, pY, pZ, 13);
			}


			if (this == lpCurPlayer && MotionEvent < 3) {
				cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);
				if (!cnt)
					lpCurPlayer->AttackCritcal = -1;				//ºø³ª°¨
				else
					lpCurPlayer->AttackCritcal = 0;					//¸íÁß

			}
		}


		break;

	case SKILL_PLAY_BERSERKER:
		if (point) {
			SkillWarriorBetserker(this, (float)Berserker_Time[point - 1]);
			AttackAnger = 200;
		}
		break;

	case SKILL_PLAY_CYCLONE_STRIKE:
		if (lpCurPlayer != this) {
			dm_SelectRange(pX, pY, pZ, Cyclone_Strike_Area[point - 1], TRUE, Cyclone_Strike_AttackNum[point - 1]);
			dm_AddRangeDamage(chrAttackTarget);			//¼±ÅÃµÈ ¹üÀ§°ø°Ý ¸ñ·Ï¿¡ Ä³¸¯ °­Á¦ Ãß°¡
		}

		for (cnt = 0; cnt < dmSelected_CharCnt; cnt++) {
			StartEffect(lpSelected_Char[cnt]->pX, lpSelected_Char[cnt]->pY + 16 * fONE, lpSelected_Char[cnt]->pZ, EFFECT_NORMAL_HIT1);
		}
		break;

		//ÆÄÀÌÅ©¸Ç
	case SKILL_PLAY_ASSASSIN_EYE:
		if (point && chrAttackTarget) {
			SkillLancelotAssassinEye(chrAttackTarget, (float)Assassin_Eye_Time[point - 1]);
		}
		break;

	case SKILL_PLAY_CHARGING_STRIKE:
		if (chrAttackTarget && point) {
			cnt = frame - MotionInfo->StartFrame * 160;

			if (MotionInfo->EventFrame[1] <= (DWORD)cnt) {
				SkillLancelotChargingStrike(this);

				if (MotionInfo->EventFrame[2] <= (DWORD)cnt && GetAttackPoint(&x, &y, &z) == TRUE) {
					if (AttackCritcal >= 0) {
						StartSkill(x, y, z, 0, 0, 0, SKILL_TRIPLE_IMPACT);
						SetDynLight(x, y, z, 100, 0, 0, 0, 400);

						switch (MotionEvent) {
						case 2:
							SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LANCE2, pX, pY, pZ);
							break;
						case 3:
							SkillPlaySound(SKILL_SOUND_SKILL_AVANGING_CRASH, pX, pY, pZ);
							break;
						case 4:
						case 5:
							SkillPlaySound(SKILL_SOUND_SKILL_CHARGING_STRIKE, pX, pY, pZ);
							break;
						}
					}
					else {
						//¹Ì½º ¼Ò¸®
						//¹«±â ¼Ò¸® Á÷Á¢ ¿¬ÁÖ
						PlayWaponSoundDirect(pX, pY, pZ, 13);
					}
				}

				if (this == lpCurPlayer && SkillEventDamageCount < 3) {

					if (SkillEventDamageCount == 0)
						SkillStopSound(SKILL_SOUND_SKILL_CHARGING);

					temp = dm_SendTransDamage(chrAttackTarget, ChargingFlag, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);

					if (!temp)
						AttackCritcal = -1;				//ºø³ª°¨
					else
						AttackCritcal = 0;					//¸íÁß

					SkillEventDamageCount++;
					MotionLoopSpeed = GetAttackSpeedFrame(7);

					MouseButton[1] = 0;
					CancelAttack();
				}
			}
			else {
				if (this == lpCurPlayer) ChargingFlag = 0;
			}

			cnt = (MotionInfo->EventFrame[3] / 160) - MotionInfo->StartFrame;

			/*
			//¸ð¼Ç ÀÌº¥Æ® È®Àå ¼³Á¤
			switch( cnt ) {
				case 226:
					dwEventFrameExt[0] = (MotionInfo->StartFrame+240)*160;
					break;
				case 396:
					dwEventFrameExt[0] = (MotionInfo->StartFrame+410)*160;
					break;
				case 576:
					dwEventFrameExt[0] = (MotionInfo->StartFrame+590)*160;
					break;
			}
			*/
			dwEventFrameExt[0] = (MotionInfo->StartFrame + 140) * 160;	//½ÃÀÛ ÇÁ·¹ÀÓ¿¡¼­ 140ÇÁ·¹ÀÓ ÈÄ¿¡ °­Á¦·Î »ðÀÔ
		}


		break;

	case SKILL_PLAY_VAGUE:
		if (lpCurPlayer == this)
		{
			PlayVanish = Vague_Time[point - 1] * 70;
			PlayVague = point;
			SendProcessSKillToServer(SKILL_PLAY_VAGUE, point, 0, 0);
			dwM_BlurTime = dwPlayTime + Vanish_Time[point - 1] * 1000;
		}
		break;

	case SKILL_PLAY_SHADOW_MASTER:

		dwEventFrameExt[0] = (793 - 710 + MotionInfo->StartFrame) * 160;
		dwEventFrameExt[1] = (794 - 710 + MotionInfo->StartFrame) * 160;
		dwEventFrameExt[2] = (795 - 710 + MotionInfo->StartFrame) * 160;

		//793	794	795
		if (chrAttackTarget && point) {
			for (cnt = 0; cnt < 7; cnt++) {
				if (SkillShadowMasterEvent[Shadow_Master_ShadowNum[point - 1]][cnt] == MotionEvent) {

					if (AttackCritcal >= 0) {
						if (GetAttackPoint(&x, &y, &z) == TRUE) {
							StartSkill(x, y, z, 0, 0, 0, SKILL_TRIPLE_IMPACT);
							SetDynLight(x, y, z, 100, 0, 0, 0, 400);

							switch (rand() % 3) {
							case 0:
								SkillPlaySound(SKILL_SOUND_SKILL_SHADOW_STRIKE1, pX, pY, pZ);
								break;
							case 1:
								SkillPlaySound(SKILL_SOUND_SKILL_SHADOW_STRIKE2, pX, pY, pZ);
								break;
							case 2:
								SkillPlaySound(SKILL_SOUND_SKILL_SHADOW_STRIKE3, pX, pY, pZ);
								break;
							}
						}
						AttackCritcal = 1;
					}
					else {
						//¹Ì½º ¼Ò¸®
						PlayWaponSoundDirect(pX, pY, pZ, 13);
						//WeaponPlaySound( this );
					}

					if (lpCurPlayer == this && (SkillEventDamageCount + 1) < Shadow_Master_ShadowNum[point - 1]) {
						x = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);
						if (!x)
							lpCurPlayer->AttackCritcal = -1;				//ºø³ª°¨
						else
							lpCurPlayer->AttackCritcal = 0;

						SkillEventDamageCount++;
					}
					break;
				}
			}
		}
		break;

		//¾ÆÃ³
	case SKILL_PLAY_RECALL_WOLVERIN:
		SkillPlaySound(SKILL_SOUND_SKILL_RECALL_WOLVERIN, pX, pY, pZ);
		break;

	case SKILL_PLAY_EVASION_MASTERY:

		break;

	case SKILL_PLAY_PHOENIX_SHOT:
		if (chrAttackTarget && point) {

			cnt = frame - MotionInfo->StartFrame * 160;

			if (MotionInfo->EventFrame[1] <= (DWORD)cnt) {
				GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				Pos1.x = pX + GeoResult_X;
				Pos1.y = pY + GeoResult_Y;
				Pos1.z = pZ + GeoResult_Z;
				Pos2.x = chrAttackTarget->pX;
				Pos2.y = chrAttackTarget->pY + 24 * fONE;
				Pos2.z = chrAttackTarget->pZ;
				SkillSagittarionPhoneixShot(this, &Pos1, &Pos2);

				//ChargingFlag

				if (this == lpCurPlayer) {

					SkillStopSound(SKILL_SOUND_SKILL_CHARGING);

					if (ChargingFlag != 0) {
						dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, FALSE);
					}
					else {
						rect.left = -50;
						rect.right = 50;
						rect.top = 0;
						rect.bottom = 300;
						rect.bottom += 20;
						rect.bottom += 20;

						//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¹Ú½º)
						dm_SelectRangeBox(lpCurPlayer, &rect, FALSE);
						//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
						dm_SendRangeDamage(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0, 0, 0, 0, 0, lpCurPlayer->AttackSkil);
					}
					lpCurPlayer->MotionLoopSpeed = GetAttackSpeedFrame(7);
					lpCurPlayer->MotionLoop = 1;

					MouseButton[1] = 0;
					//CancelAttack();
				}
				SkillPlaySound(SKILL_SOUND_SKILL_PHOENIX_SHOT, pX, pY, pZ);

			}
			else {
				if (this == lpCurPlayer) ChargingFlag = 0;
			}
		}
		break;

	case SKILL_PLAY_FORCE_OF_NATURE:
		if (point) {
			SkillSagittarionForceOfNature1(this, (float)Force_Of_Nature_Time[point - 1]);
		}
		break;
		//³ªÀÌÆ®
	case SKILL_PLAY_DIVINE_PIERCING:
		if (chrAttackTarget && point) {
			if (AttackCritcal >= 0)	SkillSaintDivinePiercing(this);


			if (Divine_Piercing_AttackNum[point - 1] >= 7) {
				dwEventFrameExt[0] = (81 - 10 + MotionInfo->StartFrame) * 160;
				dwEventFrameExt[1] = (102 - 10 + MotionInfo->StartFrame) * 160;
				dwEventFrameExt[2] = (120 - 10 + MotionInfo->StartFrame) * 160;
			}
			else if (Divine_Piercing_AttackNum[point - 1] >= 5) {
				dwEventFrameExt[0] = (227 - 150 + MotionInfo->StartFrame) * 160;
			}


			if (MotionEvent == Divine_Piercing_AttackNum[point - 1] && AttackCritcal >= 0)
				SkillPlaySound(SKILL_SOUND_SKILL_DVINE_PIERCING, pX, pY, pZ);
			else
				WeaponPlaySound(this);


			if (lpCurPlayer == this && MotionEvent < Divine_Piercing_AttackNum[point - 1]) {
				//temp = lpCurPlayer->smCharInfo.Attack_Rating;
				//lpCurPlayer->smCharInfo.Attack_Rating += Divine_Piercing_AddHit[point-1];				//¸íÁß·Â º¸°­
				cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, lpCurPlayer->AttackSkil, TRUE);
				//lpCurPlayer->smCharInfo.Attack_Rating = temp;

				if (!cnt)
					lpCurPlayer->AttackCritcal = -1;				//ºø³ª°¨
				else
					lpCurPlayer->AttackCritcal = 0;
			}

		}
		break;
	case SKILL_PLAY_GODLY_SHIELD:
		if (point) {
			SkillSaintGodlyShield(this, (float)Godly_Shield_Time[point - 1]);
		}
		break;
	case SKILL_PLAY_GODS_BLESS:
		break;

	case SKILL_PLAY_SWORD_OF_JUSTICE:
		if (point) {
			SkillSwordOfUstice(this);
		}
		break;

		//¾ÆÅ»¶õÅ¸
	case SKILL_PLAY_HALL_OF_VALHALLA:
		if (point) {
			SkillValhallaHallOfValhalla1(this);
			SkillValhallaHallOfValhallaHandEffect(this, (float)Hall_Of_Valhalla_Time[point - 1]);
		}
		break;
	case SKILL_PLAY_X_RAGE:
		if (chrAttackTarget && point) {
			GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
			Pos1.x = pX + GeoResult_X;
			Pos1.y = pY + GeoResult_Y;
			Pos1.z = pZ + GeoResult_Z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 40 * fONE;
			Pos2.z = chrAttackTarget->pZ;
			SkillValhallaXRage(&Pos1, &Pos2);

			switch (rand() % 3) {
			case 0:
				SkillPlaySound(SKILL_SOUND_SKILL_EXTREM_RAGE1, pX, pY, pZ);
				break;
			case 1:
				SkillPlaySound(SKILL_SOUND_SKILL_EXTREM_RAGE2, pX, pY, pZ);
				break;
			case 2:
				SkillPlaySound(SKILL_SOUND_SKILL_EXTREM_RAGE3, pX, pY, pZ);
				break;
			}
		}
		break;
	case SKILL_PLAY_FROST_JAVELIN:
		if (point) {
			SkillParticle_FrostJavelin(this, (float)Frost_Javelin_Time[point - 1]);
			/*
						GetMoveLocation( 0 , 24*fONE, 24*fONE , 0, Angle.y , 0 );
						Pos1.x = pX+GeoResult_X;
						Pos1.y = pY+GeoResult_Y;
						Pos1.z = pZ+GeoResult_Z;
						Pos2.x = chrAttackTarget->pX;
						Pos2.y = chrAttackTarget->pY+40*fONE;
						Pos2.z = chrAttackTarget->pZ;
						SkillFrostJavelin( &Pos1 , &Pos2 );
			*/
		}
		break;
	case SKILL_PLAY_VENGEANCE:
		if (chrAttackTarget && point) {
			GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
			Pos1.x = pX + GeoResult_X;
			Pos1.y = pY + GeoResult_Y;
			Pos1.z = pZ + GeoResult_Z;
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY + 40 * fONE;
			Pos2.z = chrAttackTarget->pZ;
			if (AttackCritcal >= 0) SkillVengeance(&Pos1, &Pos2);

			if (this == lpCurPlayer && MotionEvent == 1) {
				cnt = dm_SendTransDamage(chrAttackTarget, 0, 0, 0, 0, AttackSkil, TRUE);
				if (!cnt)
					AttackCritcal = -1;				//ºø³ª°¨
			}
			if (MotionEvent == 1)
				SkillPlaySound(SKILL_SOUND_SKILL_VENGEANCE1, pX, pY, pZ);
			else
				SkillPlaySound(SKILL_SOUND_SKILL_VENGEANCE2, pX, pY, pZ);
		}
		break;

		//ÇÁ¸®½ºÆ¼½º
	case SKILL_PLAY_GLACIAL_SPIKE:
		if (point && MotionEvent == 1) {
			SkillCelestialGlacialSpike(this);

			GetMoveLocation(0, 0, 64 * fONE, 0, Angle.y, 0);
			SetDynLight(pX + GeoResult_X, pY, pZ + GeoResult_Z, 0, 0, 100, 0, 700);

			if (this == lpCurPlayer) {
				rect.left = -50;
				rect.right = 50;
				rect.top = 0;
				rect.bottom = 300;
				rect.bottom += 20;
				rect.bottom += 20;

				//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¹Ú½º)
				dm_SelectRangeBox(lpCurPlayer, &rect, FALSE);
				//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
				dm_SendRangeDamage(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0,
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
					0, 0, lpCurPlayer->AttackSkil);
			}

			if (rand() % 2)
				SkillPlaySound(SKILL_SOUND_SKILL_GLACIAL_SPIKE1, pX, pY, pZ);
			else
				SkillPlaySound(SKILL_SOUND_SKILL_GLACIAL_SPIKE2, pX, pY, pZ);

		}
		break;
	case SKILL_PLAY_REGENERATION_FIELD:
		if (point) {
			SkillCelestialRegnerationField(this, 2.0f);

			if (this == lpCurPlayer) {
				//cnt = frame-MotionInfo->StartFrame*160;
				//if ( MotionInfo->EventFrame[1]<=(DWORD)cnt ) {
				if (!MouseButton[0] && CSKILL->SearchContiueSkill(SKILL_REGENERATION_FIELD)) {
					//¸¶¿ì½º ¹öÆ° ´©¸£°í ÀÖÀ¸¸é µ¿ÀÛ ¹Ýº¹
					frame = MotionInfo->StartFrame * 160 + MotionInfo->EventFrame[0];
				}
				else
				{
					CSKILL->CancelContinueSkill(SKILL_REGENERATION_FIELD);
				}
			}
			else {
				cnt = Regeneration_Field_Area[point - 1];
				cnt *= cnt;
				if (GetDistanceDbl(pX, pZ, lpCurPlayer->pX, lpCurPlayer->pZ) < cnt && abs(lpCurPlayer->pY - pY) < 16 * fONE) {
					CSKILL->Regeneration_Field(5, point, 1);		//Àç»ý·Â Ãß°¡
				}
			}
		}
		break;
	case SKILL_PLAY_CHAIN_LIGHTNING:
		if (chrAttackTarget && point) {

			//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (Ã¼ÀÎ¶óÀÌÆ®´×)
			//dm_SelectDamageCount( this , chrAttackTarget , Chain_Lightning_Num[ point-1 ] , Chain_Lightning_Range[point-1] );
			//dm_SelectDamageChainCount(this, chrAttackTarget, Chain_Lightning_Num[point - 1], Chain_Lightning_Range[point - 1]);

			//if (dmSelected_CharCnt > 0) {
			//	if (this == lpCurPlayer) {
			//		//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
			//		dm_SendRangeDamage(pX, pY, pZ, 0,
			//			cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
			//			cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
			//			0, 0, AttackSkil);
			//	}

			//	SkillCelestialChainLighting(lpSelected_Char, dmSelected_CharCnt);

			//	if (rand() % 2)
			//		SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LIGHTNING1, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
			//	else
			//		SkillPlaySound(SKILL_SOUND_SKILL_CHAIN_LIGHTNING2, pX, pY, pZ);		//¸¶¹ý ¼Ò¸®
			//}

			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY;
			Pos2.z = chrAttackTarget->pZ;
			SkillPriest_IceMeteor(&Pos2);

			if (this == lpCurPlayer && MotionEvent < 3)
			{
				dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, Chain_Lightning_Range[point - 1], FALSE);

				dm_SendRangeDamage(pX, pY, pZ, 0,
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
					0, 0, AttackSkil);
			}

			SkillPlaySound(SKILL_SOUND_SKILL_METEO1, pX, pY, pZ);
		}
		break;

	case SKILL_PLAY_SUMMON_MUSPELL:
		if (point) {
			SkillCelestialMusPel(this, (float)Summon_Muspell_Time[point - 1], (float)Summon_Muspell_Attack_Delay[point - 1]);
			SkillPlaySound(SKILL_SOUND_SKILL_MUSPELL2, pX, pY, pZ);
		}
		break;

		//¸ÞÁö¼Ç
	case SKILL_PLAY_FIRE_ELEMENTAL:
		if (point) {

		}
		break;
	case SKILL_PLAY_FLAME_WAVE:
		if (point) {
			SkillArchMageFlameWave(this);

			if (rand() % 2)
				SkillPlaySound(SKILL_SOUND_SKILL_FLAME_WAVE1, pX, pY, pZ);
			else
				SkillPlaySound(SKILL_SOUND_SKILL_FLAME_WAVE2, pX, pY, pZ);

			if (this == lpCurPlayer && MotionEvent == 1) {

				rect.left = -60;
				rect.right = 60;
				rect.top = 0;
				rect.bottom = 300;
				rect.bottom += 20;
				rect.bottom += 20;

				//¹üÀ§Çü °ø°Ý ¹üÀ§ ¼±Á¤ (¹Ú½º)
				dm_SelectRangeBox(lpCurPlayer, &rect, FALSE);
				//¹üÀ§°ø°Ý ¼³Á¤ÈÄ Àü¼Û
				dm_SendRangeDamage(lpCurPlayer->pX, lpCurPlayer->pY, lpCurPlayer->pZ, 0,
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
					0, 0, lpCurPlayer->AttackSkil);

			}

		}
		break;
	case SKILL_PLAY_DISTORTION:
		if (point) {

		}
		break;

	case SKILL_PLAY_METEO:
		if (chrAttackTarget && point)
		{
			Pos2.x = chrAttackTarget->pX;
			Pos2.y = chrAttackTarget->pY;
			Pos2.z = chrAttackTarget->pZ;
			SkillArchMageMeteo(&Pos2);

			if (this == lpCurPlayer && MotionEvent < 3)
			{
				dm_SelectRange(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, Meteo_Area[point - 1], FALSE);
				dm_SendRangeDamage(chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 0,
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[0],
					cInvenTory.InvenItem[sInven[0].ItemIndex - 1].sItemInfo.Damage[1],
					0, 0, lpCurPlayer->AttackSkil);
			}

			SkillPlaySound(SKILL_SOUND_SKILL_METEO1, pX, pY, pZ);
		}
		break;
	}
	//¡ÚÀÌ¸§

	return TRUE;
}

/*
SkillPlaySound( SKILL_SOUND_SKILL_FIREBOLT1	 , x,y,z );
#define	SKILL_SOUND_SKILL_CASTING_MAGICIAN	0x1800
#define	SKILL_SOUND_SKILL_CASTING_PRIEST	0x1700

#define	SKILL_SOUND_SKILL_SWORDBLAST1		0x1510
#define	SKILL_SOUND_SKILL_SWORDBLAST2		0x1511
#define	SKILL_SOUND_SKILL_HOLYBODY			0x1520

#define	SKILL_SOUND_SKILL_SHIELDSTRIKE		0x1610
#define	SKILL_SOUND_SKILL_FARINA			0x1620
#define	SKILL_SOUND_SKILL_VIGORSPEAR		0x1640

#define	SKILL_SOUND_SKILL_HEALING			0x1710
#define	SKILL_SOUND_SKILL_HOLYBOLT			0x1720
#define	SKILL_SOUND_SKILL_MULTISPARK		0x1730
#define	SKILL_SOUND_SKILL_HOLYMIND			0x1740

#define	SKILL_SOUND_SKILL_AGONY				0x1810
#define	SKILL_SOUND_SKILL_FIREBOLT1			0x1820
#define	SKILL_SOUND_SKILL_FIREBOLT2			0x1821
#define	SKILL_SOUND_SKILL_ZENITH			0x1830
#define	SKILL_SOUND_SKILL_FIREBALL1			0x1840
#define	SKILL_SOUND_SKILL_FIREBALL2			0x1841
*/

int	rsEncodeDamagePacket(rsPLAYINFO* lpPlayInfo, void* lpPacket)
{
	DWORD size, code;
	DWORD cnt, cnt2, cnt3;
	DWORD dwCode;
	DWORD* lpDword = (DWORD*)lpPacket;

	size = lpDword[0];
	code = lpDword[1];
	size /= 4;


	dwCode = (code ^ (code << 5)) + size * lpPlayInfo->dwDamagePacketKey2[1];
	cnt2 = (code + lpPlayInfo->dwDamagePacketKey2[0]) & 0xBCAFF5;
	cnt3 = cnt2 & 0x5E;

	for (cnt = 2; cnt < size; cnt++)
	{
		dwCode = dwCode ^ (cnt2 << 7) ^ lpDword[cnt];
		lpDword[cnt] = dwCode;
		cnt2 += cnt2 + cnt3;
	}

	return TRUE;
}

//Æ¯Á¤ »ó´ë¿¡°Ô °ø°ÝÀ» °¡ÇÑ´Ù ( »ó´ë¹æÀÇ ¼ÒÄÏ°ª ¶Ç´Â Ä³¸¯ÅÍ Æ÷ÀÎÅÍ [µÑÁß ¼±ÅÃ] )
int smCHAR::SendTransAttack(smCHAR* lpChar, smWINSOCK* lpsmsock, int AttackCode, int Add_Damage, int Resistance)
{
	TRANS_ATTACKDATA	TransAttackData;
	smWINSOCK* lpsmSendSock;
	rsPLAYINFO* lpPlayInfo = 0;
	char szDispDamage[128];
	smTRANS_COMMAND	smTransCommand;

	int	pow;

	int temp;
	int	accuracy;

#ifndef _W_SERVER
	//Å¬¶óÀÌ¾ðÆ® ·Î±× ÆÄÀÏ ±â·Ï
	Record_ClinetLogFile("±¸¹öÀü °ø°ÝÆÐÅ¶ - ::SendTransAttack ");
	quit = 1;
	return TRUE;
#endif


	if (lpsmsock) {
		lpPlayInfo = (rsPLAYINFO*)lpsmsock->ExtData1;
	}

	TransAttackData.code = smTRANSCODE_ATTACKDATA;
	TransAttackData.size = sizeof(TRANS_ATTACKDATA);

	TransAttackData.x = pX;
	TransAttackData.y = pY;
	TransAttackData.z = pZ;

	TransAttackData.AttackState = 1;
	TransAttackData.AttackSize = 48 * fONE;
	TransAttackData.Power = GetRandomPos(smCharInfo.Attack_Damage[0], smCharInfo.Attack_Damage[1]);

	TransAttackData.sRating[0] = smCharInfo.Level;
	TransAttackData.sRating[1] = smCharInfo.Attack_Rating;

	if (PlayHolyMind[1] > 0)		//°ø°Ý·Â °¨¼Ò - È¦¸®¸¶ÀÎµå
		TransAttackData.Power -= ((TransAttackData.Power * PlayHolyMind[0]) / 100);

	if (!lpPlayInfo) return FALSE;


	//Á¾¼º ¾ÆÀÌÅÛ ½Ã°£Á¾·á È®ÀÎ
	if (lpPlayInfo->dwSiegeItem_Scroll_Time && lpPlayInfo->dwSiegeItem_Scroll_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Scroll_Code = 0;
		lpPlayInfo->dwSiegeItem_Scroll_Time = 0;
	}
	if (lpPlayInfo->dwSiegeItem_Stone1_Time && lpPlayInfo->dwSiegeItem_Stone1_Time < dwPlayServTime) {
		lpPlayInfo->dwSiegeItem_Stone1_Code = 0;
		lpPlayInfo->dwSiegeItem_Stone1_Time = 0;
	}

	if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_B) {
		//°ø¼º Å¸¿öÅ©¸®½ºÅ» (¾ÆÀÌ½º)
		TransAttackData.sRating[0] = -SKILL_PLAY_ICE_CRYSTAL;
		TransAttackData.sRating[1] = 3;
	}
	else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_G)
	{
		//°ø¼º Å¸¿öÅ©¸®½ºÅ» (Àü±â)
		TransAttackData.sRating[0] = -SKILL_PLAY_LIGHTNING_CRYSTAL;
		TransAttackData.sRating[1] = 2;
	}
	else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_CRYSTAL_R) {
		//°ø¼º Å¸¿öÅ©¸®½ºÅ» (ºÒ)
		TransAttackData.sRating[0] = -SKILL_PLAY_FIRE_CRYSTAL;
		TransAttackData.sRating[1] = 1;
	}

	if (lpPlayInfo->dwSiegeItem_Stone1_Code && lpPlayInfo->dwSiegeItem_Stone1_Code == smCharInfo.dwCharSoundCode) {
		//Å©¸®½ºÅ» ÀúÇ×¼®
		TransAttackData.Power /= 2;		//°ø°Ý·Â Àý¹Ý
		TransAttackData.sRating[0] = smCharInfo.Level;
		TransAttackData.sRating[1] = smCharInfo.Attack_Rating;
	}

	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBC1 | sin01)) {	//¹«Àû½ºÅ©·Ñ
		if (lpPlayInfo->Position.Area == rsCASTLE_FIELD) return FALSE;
	}
	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBI1 | sin05))		//¹«Àû½ºÅ©·Ñ
		return FALSE;


	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBC1 | sin03)) {	//Ãß°¡ È¸ÇÇ
		if (lpPlayInfo->Position.Area == rsCASTLE_FIELD) {
			if ((rand() % 100) < (int)lpPlayInfo->dwSiegeItem_Scroll_Param) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s °ø°Ý Ãß°¡È¸ÇÇ ( Evade Scroll[%d] )", smCharInfo.szName, lpPlayInfo->dwSiegeItem_Scroll_Param);
					//rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
				//Ãß°¡È¸ÇÇ Á¤º¸ Å¬¶ó¸®¾ðÆ®¿¡ Ãâ·Â
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = lpPlayInfo->dwSiegeItem_Scroll_Code;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
				return FALSE;
			}
		}
	}

	if (lpPlayInfo->dwSiegeItem_Scroll_Code == (sinBI1 | sin07)) {	//Ãß°¡ È¸ÇÇ
		if ((rand() % 100) < (int)lpPlayInfo->dwSiegeItem_Scroll_Param) {
#ifdef DISP_DAMAGE
			if (lpPlayInfo->AdminMode > 2) {
				wsprintf(szDispDamage, ">%s °ø°Ý Ãß°¡È¸ÇÇ ( Evade Scroll[%d] )", smCharInfo.szName, lpPlayInfo->dwSiegeItem_Scroll_Param);
				//rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
			}
#endif
			//Ãß°¡È¸ÇÇ Á¤º¸ Å¬¶ó¸®¾ðÆ®¿¡ Ãâ·Â
			smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
			smTransCommand.size = sizeof(smTRANS_COMMAND);
			smTransCommand.WParam = 0;
			smTransCommand.LParam = lpPlayInfo->dwSiegeItem_Scroll_Code;
			smTransCommand.SParam = dwObjectSerial;
			smTransCommand.EParam = 0;
			lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
			return FALSE;
		}
	}

	if (lpPlayInfo->dwSkill_PhysicalAbsorb_Time) {	//°ø°Ý·Â °¨¼Ò (Èí¼ö·Â ¹Ì¸®Àû¿ë)- ÇÇÁöÄÃ »ç¿ë
		if (lpPlayInfo->dwSkill_PhysicalAbsorb_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo->dwSkill_PhysicalAbsorb_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_PhysicalAbsorb_Time = 0;
			lpPlayInfo->dwSkill_PhysicalAbsorb_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_MetalArmor_Time) {	//°ø°Ý·Â °¨¼Ò (Èí¼ö·Â ¹Ì¸®Àû¿ë)- ¸ÞÅ»¾Æ¸Ó »ç¿ë
		if (lpPlayInfo->dwSkill_MetalArmor_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo->dwSkill_MetalArmor_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_MetalArmor_Time = 0;
			lpPlayInfo->dwSkill_MetalArmor_Param = 0;
		}
	}


	if (lpPlayInfo->dwSkill_HolyBody_Time) {	//¾ðµ¥µå x% °ø°Ý·Â °¨¼Ò (Èí¼ö·Â ¹Ì¸®Àû¿ë)- È¦¸®¹Ùµð »ç¿ë
		if (lpPlayInfo->dwSkill_HolyBody_Time > dwPlayServTime) {
			if (smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
				TransAttackData.Power -= (TransAttackData.Power * lpPlayInfo->dwSkill_HolyBody_Param) / 100;
				if (TransAttackData.Power < 0) return FALSE;
			}
		}
		else {
			lpPlayInfo->dwSkill_HolyBody_Time = 0;
			lpPlayInfo->dwSkill_HolyBody_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_ExtreamShield_Time) {	//ºí·°À²Áõ°¡ - ÀÍ½ºÆ®¸² ½¯µå
		if (lpPlayInfo->dwSkill_ExtreamShield_Time > dwPlayServTime) {
			TransAttackData.AttackState |= (lpPlayInfo->dwSkill_ExtreamShield_Param << 16);
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_ExtreamShield_Time = 0;
			lpPlayInfo->dwSkill_ExtreamShield_Param = 0;
		}
	}


	if (lpPlayInfo->dwSkill_DivineInhalation_Time && smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {	//ºí·°½Ã »ý¸í·Â
		if (lpPlayInfo->dwSkill_DivineInhalation_Time > dwPlayServTime) {

			temp = smCharInfo.Resistance[sITEMINFO_BIONIC];
			if (temp) {
				if (temp >= 100) temp = 100;
				if (temp <= -100) temp = -100;
				temp = (D_Inhalation_Life[lpPlayInfo->dwSkill_DivineInhalation_Param] * temp) / 100;
				temp &= 0xFF;
				temp |= D_Inhalation_Block[lpPlayInfo->dwSkill_DivineInhalation_Param] << 8;
				if (temp) {
					TransAttackData.AttackState |= (temp << 16);
				}
			}
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_DivineInhalation_Time = 0;
			lpPlayInfo->dwSkill_DivineInhalation_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_VirtualLife_Time) {	//°¡»ó »ý¸í·ÂÀ» ³ô¿©¼­ °ø°Ý·Â °¨¼Ò
		if (lpPlayInfo->dwSkill_VirtualLife_Time > dwPlayServTime) {
			TransAttackData.Power -= (TransAttackData.Power * lpPlayInfo->dwSkill_VirtualLife_Param) / 100;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_VirtualLife_Time = 0;
			lpPlayInfo->dwSkill_VirtualLife_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_EnergyShield_Time) {	//¿¡³ÊÁö½¯µå °ø°Ý·Â °¨¼Ò
		if (lpPlayInfo->dwSkill_EnergyShield_Time > dwPlayServTime) {
			temp = (TransAttackData.Power * lpPlayInfo->dwSkill_EnergyShield_Param) / 100;
			TransAttackData.Power -= temp;
			TransAttackData.AttackState |= (temp << 16);
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_EnergyShield_Time = 0;
			lpPlayInfo->dwSkill_EnergyShield_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_Compulsion_Time) {	//Compulsion Èí¼ö·Â Ãß°¡
		if (lpPlayInfo->dwSkill_Compulsion_Time > dwPlayServTime) {
			TransAttackData.Power -= lpPlayInfo->dwSkill_Compulsion_Param;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_Compulsion_Time = 0;
			lpPlayInfo->dwSkill_Compulsion_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_Berserker_Time) {	//¹ö¼­Ä¿ Èí¼ö·Â °¨¼Ò
		if (lpPlayInfo->dwSkill_Berserker_Time > dwPlayServTime) {
			TransAttackData.Power -= Berserker_SubAbsorb[lpPlayInfo->dwSkill_Berserker_Param];
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_Berserker_Time = 0;
			lpPlayInfo->dwSkill_Berserker_Param = 0;
		}
	}


	//º£ÀÌ±× Ãß°¡È¸ÇÇ
	if (lpPlayInfo->dwSkill_Vague_Time) {
		if (lpPlayInfo->dwSkill_Vague_Time > dwPlayServTime) {
			if ((rand() % 100) < Vague_EvasionPercent[lpPlayInfo->dwSkill_Vague_Param]) {

#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s °ø°Ý Ãß°¡È¸ÇÇ ( Vague[%d] )", smCharInfo.szName, Vague_EvasionPercent[lpPlayInfo->dwSkill_Vague_Param]);
					//rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif

				//Ãß°¡È¸ÇÇ Á¤º¸ Å¬¶ó¸®¾ðÆ®¿¡ Ãâ·Â
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_VAGUE;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//Ãß°¡ È¸ÇÇ ÆÇÁ¤
			}
		}
		else {
			lpPlayInfo->dwSkill_Vague_Time = 0;
			lpPlayInfo->dwSkill_Vague_Param = 0;
		}
	}

	if (lpPlayInfo->dwSkill_GodlyShied_Time) {	//°ø°Ý·Â °¨¼Ò (Èí¼ö·Â ¹Ì¸®Àû¿ë)- °¡µé¸®½¯µå»ç¿ë
		if (lpPlayInfo->dwSkill_GodlyShied_Time > dwPlayServTime) {
			TransAttackData.Power -= (TransAttackData.Power * Godly_Shield_AbsorbPercent[lpPlayInfo->dwSkill_GodlyShied_Param]) / 100;
			if (TransAttackData.Power < 0) return FALSE;
		}
		else {
			lpPlayInfo->dwSkill_GodlyShied_Time = 0;
			lpPlayInfo->dwSkill_GodlyShied_Param = 0;
		}
	}


	//È¦¿Àºê¹ßÇÒ¶ó Ãß°¡È¸ÇÇ
	if (lpPlayInfo->dwSkill_HallOfValhalla_Time) {
		if (lpPlayInfo->dwSkill_HallOfValhalla_Time > dwPlayServTime) {
			if ((rand() % 100) < (int)lpPlayInfo->wSkill_HallOfValhalla_Param[0]) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s °ø°Ý Ãß°¡È¸ÇÇ ( Hall of Valhalla [%d] )", smCharInfo.szName, lpPlayInfo->wSkill_HallOfValhalla_Param[0]);
					//rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
				//Ãß°¡È¸ÇÇ Á¤º¸ Å¬¶ó¸®¾ðÆ®¿¡ Ãâ·Â
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_HALL_OF_VALHALLA;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;		//Ãß°¡ È¸ÇÇ ÆÇÁ¤
			}
		}
		else {
			lpPlayInfo->dwSkill_HallOfValhalla_Time = 0;
		}
	}

	if (lpPlayInfo->dwSkill_SummonMuspell_Time) {	//¼­¸ó ¹«½ºÆç
		if (lpPlayInfo->dwSkill_SummonMuspell_Time > dwPlayServTime) {

			if ((rand() % 100) < Summon_Muspell_BlockPercent[lpPlayInfo->dwSkill_SummonMuspell_Param]) {
#ifdef DISP_DAMAGE
				if (lpPlayInfo->AdminMode > 2) {
					wsprintf(szDispDamage, ">%s °ø°Ý Ãß°¡ È¸ÇÇ ( Summon Muspell [%d] )", smCharInfo.szName, Summon_Muspell_BlockPercent[lpPlayInfo->dwSkill_SummonMuspell_Param]);
					//rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
				}
#endif
				//Ãß°¡È¸ÇÇ Á¤º¸ Å¬¶ó¸®¾ðÆ®¿¡ Ãâ·Â
				smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
				smTransCommand.size = sizeof(smTRANS_COMMAND);
				smTransCommand.WParam = 0;
				smTransCommand.LParam = SKILL_PLAY_SUMMON_MUSPELL;
				smTransCommand.SParam = dwObjectSerial;
				smTransCommand.EParam = 0;
				lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

				return FALSE;
			}
			if (smCharInfo.Brood == smCHAR_MONSTER_UNDEAD) {
				//¾ðµ¥µå °ø°Ý·Â Èí¼ö
				temp = (TransAttackData.Power * Summon_Muspell_UndeadAbsorbPercent[lpPlayInfo->dwSkill_SummonMuspell_Param]) / 100;
				TransAttackData.AttackState |= (temp << 16);
				if (TransAttackData.Power < 0) return FALSE;
			}
		}
		else {
			lpPlayInfo->dwSkill_SummonMuspell_Time = 0;
			lpPlayInfo->dwSkill_SummonMuspell_Param = 0;
		}
	}


	if (lpPlayInfo->dwBlessSkill_Code) {
		if (lpPlayInfo->dwBlessSkill_Code == SKILL_PLAY_BLESS_ABSORB) {	//Å¬·£½ºÅ³ (Èí¼ö)
			if (lpPlayInfo->dwBlessSkill_Time > dwPlayServTime) {
				TransAttackData.Power -= lpPlayInfo->dwBlessSkill_Param;
				if (TransAttackData.Power < 0) return FALSE;
			}
			else {
				lpPlayInfo->dwBlessSkill_Code = 0;
				lpPlayInfo->dwBlessSkill_Time = 0;
			}
		}
		else if (lpPlayInfo->dwBlessSkill_Code == SKILL_PLAY_BLESS_EVADE)
		{	//Å¬·£½ºÅ³ (ÀÌº£ÀÌµå)
			if (lpPlayInfo->dwBlessSkill_Time > dwPlayServTime) {

				if ((rand() % 100) < (int)lpPlayInfo->dwBlessSkill_Param) {
#ifdef DISP_DAMAGE
					if (lpPlayInfo->AdminMode > 2) {
						wsprintf(szDispDamage, ">%s °ø°Ý Ãß°¡ È¸ÇÇ ( Clan Skill [%d] )", smCharInfo.szName, lpPlayInfo->dwBlessSkill_Param);
						//rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
					}
#endif
					//Ãß°¡È¸ÇÇ Á¤º¸ Å¬¶ó¸®¾ðÆ®¿¡ Ãâ·Â
					smTransCommand.code = smTRANSCODE_SUCCESS_EVATION;
					smTransCommand.size = sizeof(smTRANS_COMMAND);
					smTransCommand.WParam = 0;
					smTransCommand.LParam = SKILL_PLAY_BLESS_EVADE;
					smTransCommand.SParam = dwObjectSerial;
					smTransCommand.EParam = 0;
					lpPlayInfo->lpsmSock->Send((char*)&smTransCommand, smTransCommand.size, TRUE);

					return FALSE;
				}
			}
			else {
				lpPlayInfo->dwBlessSkill_Code = 0;
				lpPlayInfo->dwBlessSkill_Time = 0;
			}
		}
	}



	//ÇØ´ç ÇÃ·¹ÀÌ¾î Èí¼ö·Â ÀúÁÖ °É·ÈÀ½ ( ¸ó½ºÅÍ °ø°Ý·Â Áõ°¡ )
	if (lpPlayInfo->dwCurse_Defence_Time) {
		if (lpPlayInfo->dwCurse_Defence_Time > dwPlayServTime)
			TransAttackData.Power += lpPlayInfo->dwCurse_Defence_Param;
		else
			lpPlayInfo->dwCurse_Defence_Time = 0;
	}

	//°ø°Ý·Â °­È­
	if (PlayHolyPower[1]) {
		TransAttackData.Power += (TransAttackData.Power * PlayHolyPower[0]) / 100;
	}


	//Ãß°¡ µ¥¹ÌÁö
	if (Add_Damage) {
		TransAttackData.Power += (TransAttackData.Power * Add_Damage) / 100;
	}

	if (AttackCritcal) {
		//Å©¸®Æ¼ÄÃ Èý ( °ø°Ý ÆÄ¿ö¸¦ 1.5¹è·Î )
		TransAttackData.Power = (TransAttackData.Power * 170) / 100;
		TransAttackData.AttackState = 2;			//Å©¸®Æ¼ÄÃ µ¥¹ÌÁö
	}

	//°ø°Ý È¿°ú
	if (AttackEffect)
		TransAttackData.AttackState = 2;

	if (TransAttackData.Power < 1) TransAttackData.Power = 1;

	if (this == lpCurPlayer && smConfig.DebugMode && smConfig.szFile_Player[0]) {
		//¸ó½ºÅÍ ½ºÅ³ °ø°Ý·Â ¾÷
		if (VRKeyBuff[VK_CONTROL])
			TransAttackData.Power = 200;
		else
			TransAttackData.Power = 30;
	}
	else {
		if (lpChar) {
			switch (lpChar->smCharInfo.State) {
			case smCHAR_STATE_ENEMY:
				//¸íÁß·üÀ» ¸®ÅÏÇÑ´Ù 
				dwBattleTime = dwPlayTime;						//ÀüÅõ ½Ã°£ ±â·Ï


				temp = smCharInfo.Attack_Rating;
				//½ºÄ«¿ìÆ®È£Å© ( ¸íÁßÀ² Áõ°¡ )
				if (HoSkillCode) {
					switch (HoSkillCode) {
					case SKILL_SCOUT_HAWK:
						pow = GetSkillPower(this, lpChar, 0, 0, 0);
						if (pow) {
							smCharInfo.Attack_Rating += (smCharInfo.Attack_Rating * pow) / 100;
						}
						break;
					}
				}
				accuracy = sinGetAccuracy(lpChar->smCharInfo.Level, lpChar->smCharInfo.Defence);
				smCharInfo.Attack_Rating = temp;
				if ((rand() % 100) > accuracy)
					return FALSE;		//¹æ¾îÀ² ½ÇÆÐ
				break;

			case smCHAR_STATE_NPC:
				TransAttackData.AttackState = Resistance;
				Resistance = 0;
				break;
			}
		}
	}

	if (Resistance) {
		TransAttackData.AttackState |= (Resistance << 16);
		pow = Resistance >> 4;
		if (pow > 0 && pow < 512) {
			TransAttackData.Power += pow;			//¼Ó¼º °ø°Ý·Â Ãß°¡ ( ¼­¹ö¿¡¼­ Ãß°¡µÈ °ø°Ý·Â µû·Î °è»ê )
		}
	}

	LastAttackDamage = TransAttackData.Power;			//ÃÖ½Å °ø°Ý µ¥¹ÌÁö ±â·Ï
	TransAttackData.dwDestObjectSerial = dwObjectSerial;
	TransAttackData.dwTarObjectSerial = 0;
	TransAttackData.dwChkSum = TransAttackData.Power * 2002 + (TransAttackData.x * TransAttackData.y * TransAttackData.z);// Á¶ÀÛ ¹æÁö ÄÚµå
	lpsmSendSock = lpsmsock;


	if (lpChar) {
		if (lpChar->TransSock)
			lpsmSendSock = lpChar->TransSock;

		if (lpChar->ServerCode) {
			lpsmSendSock = GetServerSock(lpChar->ServerCode);		//¼­¹ö ÄÚµå ±¸ÇÏ±â
		}

		TransAttackData.dwTarObjectSerial = lpChar->dwObjectSerial;

		//Ã³À½ °ø°ÝÇÏ´Â Ä³¸¯ÇÑÅ×¸¸ Ã¼·Â¹Ù Ãâ·Â
		if (lpChar->smCharInfo.State == smCHAR_STATE_ENEMY && lpChar->smCharInfo.Life[1] && lpChar->smCharInfo.Life[0] == lpChar->smCharInfo.Life[1]) {
			lpChar->EnableStateBar = TRUE;
		}
	}


	if (lpsmSendSock) {
		if (!ServerMode) {
			//º¸³½ °ø°Ý·Â ±â·Ï
			Record_SendDamage(lpsmSendSock, TransAttackData.Power);
		}

		pow = TransAttackData.Power;

		if (lpsmsock) {
			if (rsServerConfig.CrazyPacketMode && lpPlayInfo && lpPlayInfo->dwDecPacketCode &&
				lpPlayInfo->dwDecPacketTime2 < dwPlayServTime) {					//Å©·¹ÀÌÁö ÆÐÅ¶ ¸ðµå

				//S2C °ø°Ý Ã½Å©¼¶ ( Å©·¹ÀÌÁö ÆÐÅ¶ )
				TransAttackData.code = smTRANSCODE_ATTACKDATA2;
				TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V(&TransAttackData);
				rsEncodeDamagePacket(lpPlayInfo, &TransAttackData);
			}
			else
				TransAttackData.dwDamageChkSum = dm_GetDamgeChkSum_S2V(&TransAttackData);
		}

		if (lpsmSendSock->Send((char*)&TransAttackData, TransAttackData.size, TRUE)) {
			return pow;
		}
	}


	return FALSE;
}



//°ø°Ý »çÁ¤°Å¸®¿¡ µé¾î¿ÔÀ¸¸é °ø°Ý ( ÁÂÇ¥ ¿Í »çÁ¤°Å¸® )
int smCHAR::PlayAttackFromPosi(int ex, int ey, int ez, int Dist, int attack)
{
	int AtDist;
	int	dDist;
	int ay;

	ay = abs(pY - ey);

	if (OnStageField >= 0 && StageField[OnStageField]->FieldCode == rsCASTLE_FIELD) {
		//°ø¼ºÀü ÇÊµå °ø°Ý ¼³Á¤ º¸Á¤
		if (ShootingMode) {
			if (ay > 80 * fONE) return NULL;
		}
		else {
			if (ay > 52 * fONE) return NULL;
		}

		//¼º¹® °æÀ¯ÇØ¼­ °ø°Ý ±ÝÁö
		if ((pZ<CastleBattleZone_DoorLineZ && ez>CastleBattleZone_DoorLineZ) ||
			(pZ > CastleBattleZone_DoorLineZ && ez < CastleBattleZone_DoorLineZ))
		{
			if (rsBlessCastle.CastleMode && rsBlessCastle.DefenceLevel == 0) {
				return NULL;
			}
		}
	}
	else
		if (ay > 64 * fONE) return NULL;

	AtDist = GetDistanceDbl(pX, pZ, ex, ez);
	dDist = (Dist >> FLOATNS);
	dDist *= dDist;

	if (AtDist < dDist) {
		if (MotionInfo->State != CHRMOTION_STATE_ATTACK &&
			MotionInfo->State != CHRMOTION_STATE_SKILL && MotionInfo->State != CHRMOTION_STATE_DEAD) {

			if (attack) {
				//¸¶À»¿¡¼± °ø°Ý ºÒ°¡
				if (smCharInfo.State != smCHAR_STATE_USER || (OnStageField >= 0 && StageField[OnStageField]->State != FIELD_STATE_VILLAGE)) {
					SetMotionFromCode(CHRMOTION_STATE_ATTACK);
					Angle.y = GetRadian2D(pX, pZ, ex, ez);
					AttackX = ex;
					AttackY = ey;
					AttackZ = ez;
				}
			}

			return TRUE;
		}
	}

	return NULL;
}



//¸ð¼ÇºÎ·Á ÃÊ±âÈ­
int InitMotionBlur()
{
	int cnt;
	int TlCnt;
	float fVCnt, fVCnt2;
	float fBlurLevel;
	int MbCnt, MvCnt;


	MatMBlur = CreateTextureMaterial("image\\MotionMask.bmp", NULL, TRUE, FALSE, FALSE, SMMAT_BLEND_ADDCOLOR);//ALPHA );

	///////////////////////// 32´Ü°è /////////////////////////

	fBlurLevel = 1.0f / 32;
	TlCnt = 0;
	fVCnt = 0;
	fVCnt2 = 0 + fBlurLevel;

	for (cnt = 0; cnt < 32; cnt++) {

		smTexLinkMotionBlur[TlCnt].NextTex = 0;
		smTexLinkMotionBlur[TlCnt].hTexture = 0;
		smTexLinkMotionBlur[TlCnt].v[0] = fVCnt;
		smTexLinkMotionBlur[TlCnt].v[1] = fVCnt;
		smTexLinkMotionBlur[TlCnt].v[2] = fVCnt2;
		smTexLinkMotionBlur[TlCnt].u[0] = 0;
		smTexLinkMotionBlur[TlCnt].u[1] = 1;
		smTexLinkMotionBlur[TlCnt].u[2] = 0;

		TlCnt++;

		smTexLinkMotionBlur[TlCnt].NextTex = 0;
		smTexLinkMotionBlur[TlCnt].hTexture = 0;
		smTexLinkMotionBlur[TlCnt].v[0] = fVCnt2;
		smTexLinkMotionBlur[TlCnt].v[1] = fVCnt;
		smTexLinkMotionBlur[TlCnt].v[2] = fVCnt2;
		smTexLinkMotionBlur[TlCnt].u[0] = 0;
		smTexLinkMotionBlur[TlCnt].u[1] = 1;
		smTexLinkMotionBlur[TlCnt].u[2] = 1;

		TlCnt++;

		fVCnt = fVCnt2;
		fVCnt2 += fBlurLevel;

	}

	MbCnt = 0;
	MvCnt = 0;

	ZeroMemory(MbVertex, sizeof(smVERTEX) * 64);

	for (cnt = 0; cnt < 32; cnt++) {

		MbFace[MbCnt].lpTexLink = &smTexLinkMotionBlur[MbCnt];
		MbFace[MbCnt].v[0] = MvCnt;
		MbFace[MbCnt].v[1] = MvCnt + 1;
		MbFace[MbCnt].v[2] = MvCnt + 2;
		MbFace[MbCnt].v[3] = MatMBlur;
		MbCnt++;

		MbFace[MbCnt].lpTexLink = &smTexLinkMotionBlur[MbCnt];
		MbFace[MbCnt].v[0] = MvCnt + 2;
		MbFace[MbCnt].v[1] = MvCnt + 1;
		MbFace[MbCnt].v[2] = MvCnt + 3;
		MbFace[MbCnt].v[3] = MatMBlur;
		MbCnt++;
		MvCnt += 2;

	}


	///////////////////////// 16´Ü°è /////////////////////////


	fBlurLevel = 1.0f / 16;
	TlCnt = 0;
	fVCnt = 0;
	fVCnt2 = 0 + fBlurLevel;

	for (cnt = 0; cnt < 16; cnt++) {

		smTexLinkMotionBlur2[TlCnt].NextTex = 0;
		smTexLinkMotionBlur2[TlCnt].hTexture = 0;
		smTexLinkMotionBlur2[TlCnt].v[0] = fVCnt;
		smTexLinkMotionBlur2[TlCnt].v[1] = fVCnt;
		smTexLinkMotionBlur2[TlCnt].v[2] = fVCnt2;
		smTexLinkMotionBlur2[TlCnt].u[0] = 0;
		smTexLinkMotionBlur2[TlCnt].u[1] = 1;
		smTexLinkMotionBlur2[TlCnt].u[2] = 0;

		TlCnt++;

		smTexLinkMotionBlur2[TlCnt].NextTex = 0;
		smTexLinkMotionBlur2[TlCnt].hTexture = 0;
		smTexLinkMotionBlur2[TlCnt].v[0] = fVCnt2;
		smTexLinkMotionBlur2[TlCnt].v[1] = fVCnt;
		smTexLinkMotionBlur2[TlCnt].v[2] = fVCnt2;
		smTexLinkMotionBlur2[TlCnt].u[0] = 0;
		smTexLinkMotionBlur2[TlCnt].u[1] = 1;
		smTexLinkMotionBlur2[TlCnt].u[2] = 1;

		TlCnt++;

		fVCnt = fVCnt2;
		fVCnt2 += fBlurLevel;

	}

	MbCnt = 0;
	MvCnt = 0;

	ZeroMemory(MbVertex, sizeof(smVERTEX) * 32);

	for (cnt = 0; cnt < 16; cnt++) {

		MbFace2[MbCnt].lpTexLink = &smTexLinkMotionBlur2[MbCnt];
		MbFace2[MbCnt].v[0] = MvCnt;
		MbFace2[MbCnt].v[1] = MvCnt + 1;
		MbFace2[MbCnt].v[2] = MvCnt + 2;
		MbFace2[MbCnt].v[3] = MatMBlur;
		MbCnt++;

		MbFace2[MbCnt].lpTexLink = &smTexLinkMotionBlur2[MbCnt];
		MbFace2[MbCnt].v[0] = MvCnt + 2;
		MbFace2[MbCnt].v[1] = MvCnt + 1;
		MbFace2[MbCnt].v[2] = MvCnt + 3;
		MbFace2[MbCnt].v[3] = MatMBlur;
		MbCnt++;
		MvCnt += 2;

	}

	return TRUE;
}


int GetProjPosi(int& x3d, int& y3d, int& z3d, POINT3D* CameraPosi, POINT3D* CameraAngle)
{

	int x2d, y2d;
	smMATRIX	trans, rX, rY, rZ, rm;
	int sx, sy, sz;
	int cx, cy, cz;

	cx = x3d - CameraPosi->x;
	cy = y3d - CameraPosi->y;
	cz = z3d - CameraPosi->z;

	smIdentityMatrix(rX);
	smIdentityMatrix(rY);
	smIdentityMatrix(rZ);
	smIdentityMatrix(trans);
	smZeroMatrix(rm);

	smRotateXMatrix(rX, (-CameraAngle->x) & ANGCLIP);
	smRotateYMatrix(rY, (-CameraAngle->y) & ANGCLIP);
	smRotateZMatrix(rZ, (-CameraAngle->z) & ANGCLIP);
	smTranslateMatrix(trans, cx, cy, cz);

	smMatrixMult(rm, trans, rY);
	smMatrixMult(rm, rm, rX);
	smMatrixMult(rm, rm, rZ);


	sx = x3d * rm._11 +
		y3d * rm._21 +
		z3d * rm._31;


	sy = x3d * rm._12 +
		y3d * rm._22 +
		z3d * rm._32;

	sz = x3d * rm._13 +
		y3d * rm._23 +
		z3d * rm._33;

	x3d = (sx >> FLOATNS) + rm._41;
	y3d = (sy >> FLOATNS) + rm._42;
	z3d = (sz >> FLOATNS) + rm._43;


	if (z3d > 0)
	{
		x3d = MidX + ((x3d * viewdistZ) / z3d);
		y3d = MidY - ((y3d * viewdistZ) / z3d);
		z3d >>= FLOATNS;
		return TRUE;

	}
	else
	{
		x2d = CLIP_OUT;
		y2d = CLIP_OUT;
		return FALSE;
	}

}




scITEM::scITEM()
{
	Init();
}

scITEM::~scITEM()
{
	Close();
}

void scITEM::Init()
{
	ZeroMemory(&Head, sizeof(scITEM));
}

int scITEM::Close()
{

	Flag = 0;

	if (lpDinaPattern) {
		AddRemoveDynPattern(lpDinaPattern);
		//smDPAT_Delete( lpDinaPattern );
		lpDinaPattern = 0;
	}

	Pattern = 0;

	return TRUE;
}

//ÆÐÅÏ¼³Á¤
int scITEM::SetPattern(smPAT3D* pat)
{
	Pattern = pat;
	return TRUE;
}


//À§Ä¡¼³Á¤
int scITEM::SetPosi(int x, int y, int z)
{
	//	int py1;
	int py2 = 0;

	pX = x;
	pZ = z;
	pY = y;

	if (pY == 0) {

		if (smGameStage[0])
			pY = smGameStage[0]->GetHeight(pX, pZ);

		if (smGameStage[1])
			py2 = smGameStage[1]->GetHeight(pX, pZ);

		if (py2 > pY) pY = py2;
	}

	return TRUE;
}


//¼ö½ÅµÈ ¾ÆÀÌÅÛ ¼³Á¤
int scITEM::SetTransItem(TRANS_ITEM* lpTransItem)
{
	dwLastTransTime = dwPlayTime;

	//	state = lpTransItem->state

	Flag = TRUE;

	strcpy_s(szName, lpTransItem->szName);
	//lstrcpy( szModelName , lpTransItem->szModelName );
	SetPosi(lpTransItem->x, lpTransItem->y, lpTransItem->z);
	lpStgArea = lpTransItem->lpStgArea;

	ItemCode = lpTransItem->ItemCode;

	iClass = lpTransItem->iClass;

	Angle.y = ((pX + pZ) >> 2) & ANGCLIP;

	if (sinGetItemInfo(ItemCode, szModelName) == FALSE || ItemCode == 0) {
		strcpy_s(szModelName, "char\\flag\\flag.ini");
	}

	/*
		switch ( ItemCode ) {
		case 0:
			lstrcpy( szModelName , "char\\flag\\flag.ini" );
			break;
		case 1:
			lstrcpy( szModelName , "DropItem\\DRcoin.ASE" );
			break;
		case 2:
			lstrcpy( szModelName , "weapons\\Weapon_low\\Iw07.ASE" );
			break;
		}
	*/

	if (!PatLoading)
		AddLoaderItem(this, szModelName);

	return TRUE;
}


#define ITEM_TRANS_LIMIT_TIME		10000

int scITEM::Draw()
{

	POINT3D CameraPosi, CameraAngle;
	POINT3D angle;
	int r, g, b;
	int scale;

	if (ItemCode == 0 && !DisplayDebug) return FALSE;

	if (dwPlayTime > (dwLastTransTime + ITEM_TRANS_LIMIT_TIME)) {
		Close();
		return FALSE;
	}

	if (!DisplayFlag || !Pattern) return FALSE;

	CameraPosi.x = smCHAR_CameraX;
	CameraPosi.y = smCHAR_CameraY;
	CameraPosi.z = smCHAR_CameraZ;

	CameraAngle.x = smCHAR_CameraAngX;
	CameraAngle.y = smCHAR_CameraAngY;
	CameraAngle.z = smCHAR_CameraAngZ;

	memcpy(&angle, &Angle, sizeof(POINT3D));

	Posi.x = pX;
	Posi.y = pY + 6 * fONE;
	Posi.z = pZ;

	int	BlinkFlag;

	BlinkFlag = FALSE;

	if ((ItemCode & sinITEM_MASK1) == sinITEM_WEAPON) {
		angle.x = ANGLE_90;
	}

	if (ItemCode && (dwPlayTime & 0x80))
		BlinkFlag = TRUE;


	if (lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == rsSOD_FIELD) {
		if ((ItemCode & sinITEM_MASK2) == sinPM1 || (ItemCode & sinITEM_MASK2) == sinSD2) {

			scale = (dwPlayTime >> 2) & 0x1FF;

			if ((scale & 0x100) != 0) {
				scale = fONE - (scale & 0xFF);
			}
			if ((ItemCode & sinITEM_MASK2) == sinPM1)
				scale *= 3;

			smRender.ObjectVertexScale = scale + fONE;
		}
	}


	if (Pattern->newModel)
	{

		if (Pattern->newModel->materialCollection)
			Pattern->newModel->materialCollection->SetBlendingMaterial(dropItemMaterial);

		Pattern->SetPosi(&Posi, &angle);
		Pattern->newModel->Render();
	}

	smRender.ObjectVertexScale = 0;

	return TRUE;

	return TRUE;
}


//////////////////////////////// ¹«Àû¸ðµå ¾Ç¿ë ´ëºñ °¨½ÃÇÔ¼ö ////////////////////////////

DWORD	Ptect_RestartCounter = 0;
DWORD	Ptect_RestartMask = 0;

//°ø°Ý ¸¶Å© º¸¾È Ä«¿îÅÍ Áõ°¡
int	Ptect_IncAttackCount(int Flag)
{
	DWORD	rc;
	smTRANS_COMMAND	smTransCommand;

	rc = Ptect_RestartCounter ^ Ptect_RestartMask;
	Ptect_RestartMask = dwPlayTime;

	switch (Flag) {
	case -1:
		rc = 0;
		break;
	case 0:
		rc++;
		break;
	}

	if (rc > 160) {
		//¼­¹ö¿¡ °æ°í º¸³¿
		smTransCommand.code = smTRANSCODE_WARNING_BLINK_ATT;
		smTransCommand.size = sizeof(smTRANS_COMMAND);
		smTransCommand.WParam = rc;
		smTransCommand.LParam = 0;
		smTransCommand.SParam = 0;
		smTransCommand.EParam = 0;
		if (smWsockDataServer) smWsockDataServer->Send((char*)&smTransCommand, smTransCommand.size, TRUE);
		rc = 0;
	}

	rc = rc ^ Ptect_RestartMask;
	Ptect_RestartCounter = rc;

	return TRUE;
}

//°ø°Ý ¸¶Å© º¸¾È Ä«¿îÅÍ ¾ò±â
DWORD Ptect_GetAttackCount()
{
	DWORD	rc;
	rc = Ptect_RestartCounter ^ Ptect_RestartMask;
	Ptect_RestartMask = dwPlayTime;
	Ptect_RestartCounter = rc ^ Ptect_RestartMask;

	return rc;
}

//Ä³½½ ¸¶½ºÅÍ ¾÷µ¥ÀÌÆ®
int UpdateCastleMasterClan(DWORD dwMasterClan)
{
	int cnt;

	SetClanMaster_Player(dwMasterClan);

	for (cnt = 0; cnt < OTHER_PLAYER_MAX; cnt++) {
		if (chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial) {
			if (chrOtherPlayer[cnt].smCharInfo.ClassClan && GetClanCode(chrOtherPlayer[cnt].smCharInfo.ClassClan) == dwMasterClan)
				lpCurPlayer->Clan_CastleMasterFlag = TRUE;
			else
				lpCurPlayer->Clan_CastleMasterFlag = FALSE;
		}
	}

	return TRUE;
}

//ÇÃ·¹ÀÌ¾î Å¬·£ ¸¶½ºÅÍ È®ÀÎ
int SetClanMaster_Player(DWORD dwMasterClan)
{
	if (lpCurPlayer->smCharInfo.ClassClan && GetClanCode(lpCurPlayer->smCharInfo.ClassClan) == dwMasterClan)
		lpCurPlayer->Clan_CastleMasterFlag = TRUE;
	else
		lpCurPlayer->Clan_CastleMasterFlag = FALSE;

	return lpCurPlayer->Clan_CastleMasterFlag;

}