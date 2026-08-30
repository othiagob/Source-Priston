#include "Utils\common.h"

#include "smlib3d\\smd3d.h"
#include "smwsock.h"
#include "character.h"
#include "fileread.h"
#include "playmain.h"
#include "netplay.h"
#include "field.h"
#include "sinbaram\\sinlinkheader.h"
#include "effectsnd.h"
#include "playsub.h"
#include "skillsub.h"
#include "language.h"
#include "tjboy\\clanmenu\\tjclanDEF.h"
#include "tjboy\\clanmenu\\tjclan.h"
#include "tjboy\\clanmenu\\cE_CViewClanInfo.h"

#include "damage.h"
#include "srcserver\\svr_Damage.h"
#include "srcserver\\onserver.h"
#include "Chat/ChatServer.h"

//Ãß°¡ È¸ÇÇ µ¥¹ÌÁö Ãâ·Â ¿É¼Ç
#define DISP_DAMAGE

#ifdef	DISP_DAMAGE
int rsDisplayDamgeToClient(rsPLAYINFO* lpPlayInfo, char* szDmgMsg);
#endif

#define		SUCCUBUS_CURSE_TIME		10

//PK½Ã 40% Èí¼ö·Â Àû¿ë
#define		PKMODE_ABSORB			30

extern BYTE VRKeyBuff[256];
extern DWORD dwPlayServTime;					//¼­¹ö¸ðµåÀÏ¶§ ½Ã°£

static int MatMBlur;							//¸ÞÆ®¸®¾ó ¹øÈ£

static smTEXLINK smTexLinkMotionBlur[128];		//ÅØ½ºÃÄ ÁÂÇ¥
static smVERTEX MbVertex[64];
static smFACE MbFace[64];

static smTEXLINK smTexLinkMotionBlur2[64];		//ÅØ½ºÃÄ ÁÂÇ¥
static smVERTEX MbVertex2[32];
static smFACE MbFace2[32];

#define	AGING_SUB_LEVEL	10

int	SkillChargingFlag = 0;			//½ºÅ³ Â÷Â¡ ÇÃ·¢
int	SkillEventDamageCount;			//½ºÅ³ µ¥¹ÌÁö Ä«¿îÅÍ

TRANS_SKIL_ATTACKDATA	Trans_SplashCharList;		//ÆÃ°Ü¼­ ¸Â´Â °ø°Ý µ¥ÀÌÅ¸ ÀÓ½ÃÀúÀå

//Å©¸®Æ¼ÄÃ ¼½¼Ç
CRITICAL_SECTION	cLoadSection;

int DrawMotionBlur();

/////////////////////// ¼­¹öÃø ÇÔ¼ö ºÒ·¯´Ù ¾´´Ù //////////////////////
//ÀÌº¥Æ® Áö¿ªÀÎÁö Ã½Å©
int rsCheckHardCoreEventArea(int x, int y, int z);
extern int	rsHardCoreEventInterLineZ;
extern int	rsHardCoreEventInterLineZ2;
extern int	rsHardCoreEventPosY;
extern int	rsHardCoreEventOutLineZ;
extern rsSERVER_CONFIG		rsServerConfig;

//Æ¯Á¤À§Ä¡¿¡ º¸Á¶¸ó½ºÅÍ °­Á¦ ÃâÇö ½ÃÅ´
smCHAR* rsOpenSubMonster(smCHAR* lpMainChar, DWORD dwCharSoundCode, int x, int y, int z, int MaxFlag);
//ÇØ´ç ¸ó½ºÅÍ Ãâ½Å ±ê¹ß ÀÇ È°¼ºÈ­ ¸ó½ºÅÍ ¼ö
int rsGetMonCountFlag(smCHAR* lpChar);


//»ó´ë Ä³¸¯ÅÍ¿¡ ±â¼ú °ø°Ý °¡ÇÔ 
int rsSendTransSkillAttack(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo);
//¼­¹ö - È¦¸® ¸®ÇÃ·º¼Ç °ø°Ý Ã³¸®
int rsProcessAttack_SkillHolyReflection(rsPLAYINFO* lpPlayInfo, smCHAR* lpChar);
//ÁÖº¯ ¸ó½ºÅÍ¸¦ Ã£´Â´Ù
smCHAR* rsFindAroundMonster(smCHAR* lpChar, int RoundDist);
//ÁÖº¯ ¸ó½ºÅÍ¸¦ Ã£¾Æ µ¥¹ÌÁö °ø°Ý
smCHAR* rsSkillDamage_AroundMonster(smCHAR* lpChar);
//ÁÖº¯ ¸ó½ºÅÍ¸¦ Ã£¾Æ ½ºÅ³ ( ¶óÅõ ÁÖº¯ ¸ó½ºÅÍ ½ºÅÏÁ¦°Å )
smCHAR* rsSkillMonster_AroundMonster(smCHAR* lpChar, int RoundDist);

//////////////////////////////////////////////////////////////////////

int GetProjPosi(int& x3d, int& y3d, int& z3d, POINT3D* CameraPosi, POINT3D* CameraAngle);
//»ó´ë Ä³¸¯ÅÍ¿¡ ±â¼ú °ø°Ý °¡ÇÔ 
int rsSendTransSkillAttack(smCHAR* lpChar, rsPLAYINFO* lpPlayInfo);

extern int rsRegist_ItemSecCode(rsPLAYINFO* lpPlayInfo, TRANS_ITEMINFO* lpTransItemInfo, int NewItem);
extern int	rsRecord_ItemLog(rsPLAYINFO* lpPlayInfo, DWORD dwItemCode, DWORD	dwHead, DWORD dwChkSum, int Flag, char itemName[50]);

extern int rsPutItem2(rsPLAYINFO* lpPlayInfo, sITEMINFO* lpItem);

int	SkillShadowMasterEvent[9][7] = {
	{ 0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0 },

	{ 1,3,5,0,0,0,0 },		//3 ¹ø ¶§¸²
	{ 1,3,5,6,0,0,0 },
	{ 1,2,3,4,6,0,0 },
	{ 1,2,3,4,6,7,0 },
	{ 1,2,3,4,5,6,7 },		//7 ¹ø ¶§¸²

	{ 1,2,3,4,5,6,7 }
};

//°ø¼ºÀü ÇÊµå ÁÂÇ¥ Á¤º¸
int CastleBattleZone_LineZ = -24043 * fONE;
int	CastleBattleZone_DoorLineZ = -25545 * fONE;								//¼º¹® ÀÔ±¸
int	CastleBattleZone_DoorLineX[2] = { 34407 * fONE , 34695 * fONE };			//¼º¹® ÀÔ±¸
int	CastleBattleZone_BridgeZ = -25258 * fONE;									//´Ù¸® ÀÔ±¸
int CastleSoulFountain_Area[4] = { 37040 * fONE , 37158 * fONE , -30286 * fONE, -30102 * fONE };	//¿µÈ¥ÀÇ»ùÅÍ



//ÀúÇ× ¼Ó¼º °è»ê
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


//######################################################################################
//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
int			Debug_TalkZoomMode;
int			Debug_TalkRepeatMode;
int			Debug_TalkRepeatModeType;

void DrawTalkZoom(smCHAR* lpChar, smPAT3D* lpPattern, float fx, float fy) {}
//######################################################################################


//µµ±¸ À§Ä¡ Æ÷ÀÎÆ® ±¸ÇÏ±â
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



//°ø°Ý Æ÷ÀÎÆ® ÁÂÇ¥¸¦ ±¸ÇÑ´Ù
int smCHAR::GetAttackPoint(int* nX, int* nY, int* nZ)
{

	smMATRIX* mWorld;
	POINT3D angle;
	int tx, ty, tz;
	smCHARTOOL* ChrTool = 0;
	smOBJ3D* lpObj;
	int pframe;
	int rx, ry, rz;
	/*
		if ( HvLeftHand.PatTool )
			ChrTool = &HvLeftHand;

		if ( HvRightHand.PatTool )
	*/
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



//¾ÆÀÌÅÛ ¹øÈ£ Ã£±â ÄÚµå·Î
int GetSinItemNumFromCode(DWORD CODE)
{
	for (int j = 0; j < MAX_ITEM; j++) {
		if (sItem[j].CODE == CODE) {
			return j;
		}

	}
	return -1;
}

//2Á¡ÀÇ °Å¸®¸¦ ±¸ÇÑ´Ù ( ¹ÝÈ¯Àº °Å¸®ÀÇ Á¦°ö°ª )
int GetDistanceDbl(int x1, int z1, int x2, int z2)
{
	int ax, az;

	ax = (x1 - x2) >> FLOATNS;
	az = (z1 - z2) >> FLOATNS;
	ax *= ax;
	az *= az;
	return ax + az;
}

//À§Ä¡¸¦ °Ë»çÇÏ¿© ÀÏÁ¤Å©±â ¹Ú½ºÅ©±â·Î È®ÀÎ
int CheckLocateBox(int x1, int y1, int z1, int x2, int y2, int z2, int size)
{
	if (abs(x1 - x2) < size && abs(y1 - y2) < size && abs(z1 - z2) < size)
		return TRUE;

	return FALSE;
}


//Æ¯Á¤ ¿ÀºêÁ§Æ®ÀÇ Æ®¸®¸¦ ÃßÀûÇÏ¿© ¾Ö´Ï¸ÞÀÌ¼Ç ½ÃÅ´
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


// Ä³¸¯ÅÍ¿¡ »ç¿ëÇÒ Ä«¸Þ¶ó À§Ä¡¸¦ ¼³Á¤ÇÑ´Ù
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

////////////////////////////////////////////////////////////////////


//°í¼Ó Ã½Å©¿ë ÄÚµå Á¦ÀÛ
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
		if (ch >= 'a' && ch <= 'z') {//´ë¹®ÀÚ ¼Ò¹®ÀÚ·Î
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

//######################################################################################
// ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®	
smPATTERN::smPATTERN(void)
{
	Init();
}


smPATTERN::~smPATTERN(void)
{
	Close();
}
//######################################################################################


//ÃÊ±âÈ­
void smPATTERN::Init()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++)
		ZeroMemory(&DinaPat[cnt], sizeof(smDPAT));

	BipPattern = 0;
}

//¸»±âÈ­
int smPATTERN::Close()
{
	int cnt;

	//smMODELINFO

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


//ÀÌ¸ªÀ¸·Î ÆÐÅÏ¹øÈ£¸¦ Ã£À½
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

//ÀÌ¸ªÀ¸·Î ÆÐÅÏ¹øÈ£¸¦ Ã£À½
int smPATTERN::FindFromCode(DWORD dwCode)
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat && dwCode == DinaPat[cnt].dwSpeedFindSum)
			return cnt;
	}

	return -1;
}

//»õ·Î¿î ÆÐÅÏ À§Ä¡¸¦ ¾òÀ½
int smPATTERN::GetNew()
{
	int cnt;

	for (cnt = 0; cnt < DPAT_MAX; cnt++) {
		if (DinaPat[cnt].Pat == 0)
			return cnt;
	}

	return -1;
}

//»Áµû±Í ¸ð¼Ç µ¥ÀÌÅ¸¸¦ ºÒ·¯¿Í µî·Ï
smDPAT* smPATTERN::LoadBipPattern(char* szFileName)
{
	int Num;
	DWORD dwCode;


	//Num = FindFromName( szFileName );
	dwCode = GetSpeedSum(szFileName);
	Num = FindFromCode(dwCode);

	if (Num < 0) {
		//AddChatBuff( szFileName );

		Num = GetNew();
		if (Num >= 0) {
			DinaPat[Num].Pat = smASE_ReadBone(szFileName);		// ¾Ö´Ï¸ÞÀÌ¼Ç ÀÐ±â
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

//²®µ¥±â µ¥ÀÌÅ¸¸¦ ºÒ·¯¿Í µî·Ï
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

		DinaPat[Num].Pat = smASE_Read(modelInfo.szModelFile);// , modelInfo.szModelName );
		if (!DinaPat[Num].Pat) {
			if (smConfig.DebugMode) {
				//·Îµù ¿¡·¯ ¸Þ¼¼Áö Ãâ·Â
				char	szErrorMsg[128];
				wsprintf(szErrorMsg, "Loading Error , ( %s )", modelInfo.szModelFile);
				AddChatBuff(szErrorMsg);
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
		return &DinaPat[Num];
	}

	DinaPat[Num].UseCount++;
	return &DinaPat[Num];
}


extern smPAT3D* PatKnife;
char* szDefaultCharName = "char\\woman.ini";

//¸ðµ¨ ·Î´õ ÃÊ±âÈ­
int InitLoader();
//¸ðµ¨ ·ÎµùÀ» À§ÇÑ ¾²·¹µå »ý¼º
int CreateLoaderThread();
DWORD WINAPI smLoaderThreadProc(void* pInfo);

struct smDATALOADER {
	smSTAGE3D* lpStage;
	smCHAR* lpChar;
	scITEM* lpItem;


	char		szLoadName[64];
	char		szLoadName2[64];

	int			Result;
	int			Cancel;				//Ãë¼Ò ¿ä±¸
};

#define DATALOADER_MAX		64
#define DATALOADER_MASK		63

smDATALOADER	smLoader[DATALOADER_MAX];
int	smLoaderCnt;
int smLoaderEnd;
int	ThreadLoaderStats;				//¾²·¹µå »óÅÂ

smPATTERN smBipPattern;
smPATTERN smPattern;

//¹è°æÀ» »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
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

	//¸ðµ¨ ·ÎµùÀ» À§ÇÑ ¾²·¹µå »ý¼º
	CreateLoaderThread();

	return smLoaderEnd;
}

//¾ÆÀÌÅÛÀ» »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
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
	lstrcpy(smLoader[cnt].szLoadName, szName);

	if (szName2)
		lstrcpy(smLoader[cnt].szLoadName2, szName2);
	else
		smLoader[cnt].szLoadName2[0] = 0;

	smLoader[cnt].lpChar->PatLoading = TRUE;

	//¸ðµ¨ ·ÎµùÀ» À§ÇÑ ¾²·¹µå »ý¼º
	CreateLoaderThread();

	return TRUE;
}




//Ä³¸¯ÅÍ¸¦ »õ ¾²·¹µå¿¡ ÅëÇÏ¿© ·Îµå ÇÑ´Ù
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

	//¸ðµ¨ ·ÎµùÀ» À§ÇÑ ¾²·¹µå »ý¼º
	CreateLoaderThread();

	return TRUE;
}


DWORD WINAPI smLoaderThreadProc(void* pInfo)
{
	//	HANDLE	hThread;

	int	cnt;
	smDPAT* smDPat;
	smDPAT* smDPat2;
	//	int Result;

	smDPAT* lpPatOld;
	smDPAT* lpPatOld2;

	//	char szDebugPrint[256];

		//Å©¸®Æ¼Ä® ¼½¼Ç ¼±¾ð
	EnterCriticalSection(&cLoadSection);

	//	while( smLoaderCnt<smLoaderEnd ) {
	cnt = smLoaderCnt & DATALOADER_MASK;

	//¹è°æ ·Îµå
	if (smLoader[cnt].lpStage) {
		//·ÎµùÁß È­¸é Ç¥½Ã
		OpenDispLoading();

		if (smSTAGE3D_ReadASE(smLoader[cnt].szLoadName, smLoader[cnt].lpStage))
		{

			smLoader[cnt].Result = TRUE;
			smLoader[cnt].lpStage->Head = TRUE;
		}
		//·Îµù Ç¥½ÃÁ¾·á
		CloseDispLoading();
	}

	//¾ÆÀÌÅÛ ·Îµå
	if (smLoader[cnt].lpItem) {
		smLoader[cnt].lpItem->PatLoading = TRUE;
		//AddChatBuff( smLoader[cnt].szLoadName );

		lpPatOld = smLoader[cnt].lpItem->lpDinaPattern;
		smDPat = smPattern.LoadCharactor(smLoader[cnt].szLoadName);

		if (smDPat)
		{


			//Ä³¸¯ÅÍ ½ºÅ² È®ÀÎ ÃÊ±âÈ­
			//smLoader[cnt].lpChar->Init();

			smLoader[cnt].lpItem->DisplayFlag = 0;

			smLoader[cnt].lpItem->SetPattern(smDPat->Pat);
			smLoader[cnt].lpItem->lpDinaPattern = smDPat;
			lstrcpy(smLoader[cnt].lpItem->szModelName, smDPat->szPatName);
			smLoader[cnt].lpItem->DisplayFlag = TRUE;

		}
		smLoader[cnt].Result = TRUE;

		if (smDPat && lpPatOld)
			smDPAT_Delete(lpPatOld);

		smLoader[cnt].lpItem->PatLoading = FALSE;
	}



	//Ä³¸¯ÅÍ ·Îµå
	if (smLoader[cnt].lpChar) {
		smDPat = 0;
		smDPat2 = 0;

		smLoader[cnt].lpChar->PatLoading = TRUE;
		//AddChatBuff( smLoader[cnt].szLoadName );

		lpPatOld = smLoader[cnt].lpChar->lpDinaPattern;
		lpPatOld2 = smLoader[cnt].lpChar->lpDinaPattern2;

		smDPat = smPattern.LoadCharactor(smLoader[cnt].szLoadName);

		if (smDPat) {
			//Ä³¸¯ÅÍ ½ºÅ² È®ÀÎ ÃÊ±âÈ­
			//smLoader[cnt].lpChar->Init();

			smLoader[cnt].lpChar->DisplayFlag = 0;

			smLoader[cnt].lpChar->SetDinaPattern(smDPat);

			smLoader[cnt].lpChar->SetMotionFromCode(CHRMOTION_STATE_STAND);

			//				smLoader[cnt].lpChar->SetTool( PatKnife ,  hvPOSI_RHAND );
							//smLoader[cnt].lpChar->ActionPattern = 99;
			smLoader[cnt].lpChar->Flag = 1;
			lstrcpy(smLoader[cnt].lpChar->smCharInfo.szModelName, smDPat->szPatName);


			smLoader[cnt].lpChar->smCharInfo.szModelName2[0] = 0;

			if (smDPat->lpModelInfo->szSubModelFile[0]) {
				//º¸Á¶ ¸ðµ¨ °¡Á®¿À±â ( ¸ÞÀÎ¸ðµ¨¿¡ º¸Á¶ ¸ðµ¨ ÆÄÀÏÀÌ Á¸ÀçÇÒ°æ¿ì )
				smDPat2 = smPattern.LoadCharactor(smDPat->lpModelInfo->szSubModelFile);
			}
			else {
				//º¸Á¶ ¸ðµ¨ ·ÎµùÀÌ ¼³Á¤µÇ ÀÖ´Â °æ¿ì 
				if (smLoader[cnt].szLoadName2[0]) {
					smDPat2 = smPattern.LoadCharactor(smLoader[cnt].szLoadName2);
					lstrcpy(smLoader[cnt].lpChar->smCharInfo.szModelName2, smDPat2->szPatName);
				}
			}

			smLoader[cnt].lpChar->SetDinaPattern2(smDPat2);
			smLoader[cnt].lpChar->DisplayFlag = TRUE;

		}
		else {
			/*
			//ÇöÀç ÇÏµå¿¡ ¾ø´Â Ä³¸¯ÅÍ ½ºÅ²
			smDPat = smPattern.LoadCharactor( szDefaultCharName );

			if ( lpD3DDevice && smDPat->UseCount==1 ) {
				//wsprintf( szDebugPrint , "TEX_READ:%s",szDefaultCharName );
				//AddChatBuff( szDebugPrint );
				ReadTextures();
				//½º¿Ò ÅØ½ºÃÄ È®ÀÎ
				if ( smDPat && smDPat->Pat ) smDPat->Pat->smMaterialGroup->CheckMatreialTextureSwap();
			}

			smLoader[cnt].lpChar->DisplayFlag = 0;

			//Ä³¸¯ÅÍ ½ºÅ² È®ÀÎ ÃÊ±âÈ­
			//smLoader[cnt].lpChar->Init();
			smLoader[cnt].lpChar->SetDinaPattern( smDPat );
			smLoader[cnt].lpChar->SetMotionFromCode( CHRMOTION_STATE_STAND );

//				smLoader[cnt].lpChar->SetTool( PatKnife ,  hvPOSI_RHAND );
				//smLoader[cnt].lpChar->ActionPattern = 99;
				smLoader[cnt].lpChar->Flag = 1;
				lstrcpy( smLoader[cnt].lpChar->smCharInfo.szModelName , smDPat->szPatName );
				smLoader[cnt].lpChar->DisplayFlag = TRUE;
				*/
			Utils_Log(LOG_DEBUG, "SetDisconnectFlag: 7");
			DisconnectFlag = dwPlayTime;
		}
		smLoader[cnt].Result = TRUE;

		if (lpPatOld) smDPAT_Delete(lpPatOld);
		if (lpPatOld2) smDPAT_Delete(lpPatOld2);
		/*
					if ( smDPat && lpPatOld ) {
						if ( smDPat2 && lpPatOld2 ) smDPAT_Delete( lpPatOld2 );
						smDPAT_Delete( lpPatOld );
					}
		*/

		smLoader[cnt].lpChar->PatLoading = FALSE;
	}
	smLoaderCnt++;
	//	}

		//Å©¸®Æ¼Ä® ¼½¼Ç ÇØÁ¦
	LeaveCriticalSection(&cLoadSection);

	ThreadLoaderStats = FALSE;				//¾²·¹µå »óÅÂ
	//ExitThread( TRUE );


	return TRUE;
}

//¸ðµ¨ ·ÎµùÀ» À§ÇÑ ¾²·¹µå »ý¼º
int CreateLoaderThread()
{
	//	DWORD dwMsgId;
	//	HANDLE	handle;
	/*
	//	if ( ThreadLoaderStats==FALSE ) {
			handle = CreateThread( NULL , 0, smLoaderThreadProc , 0 , 0, &dwMsgId );
	//	}

		SetThreadPriority( handle , THREAD_PRIORITY_HIGHEST );		//Å¬·¡½º¸¦ ³ô°Ô
	*/

	smLoaderThreadProc(0);

	return FALSE;
}
//¸ðµ¨ ·Î´õ ÃÊ±âÈ­
int InitLoader()
{
	smLoaderCnt = 0;
	smLoaderEnd = 0;

	return TRUE;
}

//ÆÐÅÏ¹öÆÛ¸¦ ÃÊ±âÈ­
int InitPatterns()
{
	smBipPattern.Init();
	smPattern.Init();

	smPattern.BipPattern = &smBipPattern;

	//¸ðµ¨ ·Î´õ ÃÊ±âÈ­
	InitLoader();


	//Å©¸®Æ¼ÄÃ ¼½¼Ç ÃÊ±âÈ­
	InitializeCriticalSection(&cLoadSection);

	//	CRITICAL_SECTION	cPatSection;


	return TRUE;
}

//ÆÐÅÏ¹öÆÛ¸¦ ¸»±âÈ­
int ClosePatterns()
{
	smBipPattern.Close();
	smPattern.Close();

	return TRUE;
}

//Á¦°Å
int smDPAT_Delete(smDPAT* dPat)
{
	if (dPat->UseCount > 0) {
		dPat->UseCount--;

		//######################################################################################
		//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
		if (dPat->lpTalkLink && dPat->UseCount == 0)
			smDPAT_Delete(dPat->lpTalkLink);
		//######################################################################################

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


//Ä³¸¯ÅÍ¿¡ ÆÐÅÏÀ» ¼³Á¤
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
			//º¸Á¶ ¸ðµ¨ °¡Á®¿À±â ( ¸ÞÀÎ¸ðµ¨¿¡ º¸Á¶ ¸ðµ¨ ÆÄÀÏÀÌ Á¸ÀçÇÒ°æ¿ì )
			smDPat2 = smPattern.LoadCharactor(smDPat->lpModelInfo->szSubModelFile);
		}
		else {
			//º¸Á¶ ¸ðµ¨ ·ÎµùÀÌ ¼³Á¤µÇ ÀÖ´Â °æ¿ì 
			if (szName2 && szName2[0]) {
				smDPat2 = smPattern.LoadCharactor(szName2);
				lstrcpy(smChar->smCharInfo.szModelName2, smDPat2->szPatName);
			}
		}

		//if ( smDPat2 ) {
		smChar->SetDinaPattern2(smDPat2);
		//}

		return TRUE;
	}

	return FALSE;
}

//Ä³¸¯ÅÍ¿¡ ÆÐÅÏÀ» ¼³Á¤
int SetLoadPattern(smCHAR* smChar, char* szName, POINT3D* Posi, POINT3D* Angle)
{

	return SetLoadPattern(smChar, szName, 0, Posi, Angle);
}




////////////////////////// smCHAR ÇÔ¼ö /////////////////////////////
smCHAR::smCHAR()
{
	Pattern = 0;
	Pattern2 = 0;

	Init();
}

smCHAR::~smCHAR()
{

}

void smCHAR::Init()
{

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	TalkPattern = NULL;
	TalkFrame = TalkSoundSum = 0;
	lpTalkModelParent = NULL;
	lpTalkMotionInfo = NULL;

	::ZeroMemory(&FrameInfo, sizeof(SMotionStEndInfo));
	//######################################################################################

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

	//	int		ColorBlink;
	//	short	sColors[4];
		/*
		HvRightHand.sColors[SMC_R] = 0;
		HvRightHand.sColors[SMC_G] = 0;
		HvRightHand.sColors[SMC_B] = 190;
		HvRightHand.sColors[SMC_A] = 96;

		HvLeftHand.sColors[SMC_R] = 0;
		HvLeftHand.sColors[SMC_G] = 0;
		HvLeftHand.sColors[SMC_B] = 0;
		HvLeftHand.sColors[SMC_A] = -180;
		*/


	wStickItems[0] = -1;
	wStickItems[1] = -1;
	wStickItems[2] = -1;
	wStickItems[3] = -1;

	DisplayFlag = 0;
	AnimDispMode = 0;
	PatDispMode = DISP_MODE_PATMAIN;
	MotionSelectFrame = 0;

	smMotionInfo = 0;

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	smMotionInfo2 = 0;
	//######################################################################################


	DisplayTools = TRUE;

	dwTarget = FALSE;

	if (smStage) lpStage = smStage;

	//¿òÁ÷ÀÏ ´ÙÀ½ ¸ñÀû ÁÂÇ¥
	ptNextTarget.x = 0;
	ptNextTarget.y = 0;
	ptNextTarget.z = 0;

	//¸ñÀû Ä³¸¯ÅÍ ~~ 
	lpTargetPlayInfo = 0;		//¸ñÇ¥ Ä³¸¯ÅÍ (¼­¹ö¿ë)
	lpTargetChar = 0;		//¸ñÇ¥ Ä³¸¯ÅÍ (Å¬¶óÀÌ¾ðÆ®¿ë)

	lpMasterPlayInfo = 0;		//ÁÖÀÎ Ä³¸¯ÅÍ ( ¼­¹ö¿ë )

	//Ã¼·Â ÀÓ½Ã ¼³Á¤
	smCharInfo.Life[0] = 100;
	smCharInfo.Life[1] = 100;
	smCharInfo.State = 0;
	//smCharInfo.Brood = 0;

	dwLastTransTime = dwPlayTime;

	FrameCounter = 0;

	PatWidth = 0;
	PatHeight = 0;
	PatSizeLevel = 0;

	FrameStep = 80;
	MoveSpeed = 250;		//ÀÌµ¿ ¿òÁ÷ÀÓ °ª
	AttackSpeed = fONE;		//°ø°Ý ¼Óµµ

	lpExt1 = 0;
	lpExt2 = 0;
	lpExt3 = 0;
	lpExt4 = 0;

	//Ã¤ÆÃ ¹®ÀÚ¿­ ÃÊ±âÈ­
	dwChatMessageTimer = dwPlayTime;
	//Àü¼Û ¹öÆÛ ÃÊ±âÈ­
	srTransBuffSize = 0;

	dwActionItemCode = 0;		//±âº» ¾×¼Æ ¾ÆÀÌÅÛ 

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
	lpCharOverLap = 0;			//°ãÄ£ Ä³¸¯ÅÍÀÇ Æ÷ÀÎÆ®

	TargetMoveCount = 0;		//¸ñÇ¥ ÀÌµ¿ÁÂÇ¥ Ä«¿îÅÍ

	AttackObjBip = 0;

	NearPlayCount = 256;		//±ÙÃ³¿¡ À¯Àú È®ÀÎ Ä«¿îÅÍ ( ¼­¹ö¿¡¼­ »ç¿ë )
	AttackCritcal = 0;			//Å©¸®Æ¼ÄÃ Èý ¼³Á¤

	AutoMoveStep = 0;

	LevelLog = 0;

	dwNextMotionCode = 0;
	PosBeginCount = 0;

	EnableStateBar = 0;
	DisplayAlpha = 0;			//¹ÝÅõ¸íµµ
	RendAlpha = 0;				//¹ÝÅõ¸í Ä³¸¯
	AttackSkil = 0;				//½ºÅ³°ø°Ý
	AttackEffect = 0;			//°ø°Ý¿ë È¿°ú ÃÊ±âÈ­

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

	PlayHolyMind[1] = 0;			//µ¥¹ÌÁö ¾àÈ­ ±â´É ( [0] °¨¼ÒµÈ µ¥¹ÌÁö% [1] À¯Áö½Ã°£ )
	PlayHolyPower[1] = 0;		//µ¥¹ÌÁö °­È­ ±â´É ( [0] °­È­µÈ µ¥¹ÌÁö% [1] À¯Áö½Ã°£ )
	PlayPoison[1] = 0;			//µ¶ °ø°Ý ¹ÞÀ½
	DispPoison = 0;				//µ¶ Ç¥½Ã
	PlayHolyIncantation[1] = 0;	//¸ó½ºÅÍ ÇöÈ¤
	PlayVanish = 0;				//¹è´Ï½¬ ½ºÅ³ Àû¿ë»óÅÂ
	PlayCurseQuest = 0;			//ÀúÁÖ Äù½ºÆ®
	PlayVague = 0;
	PlayDistortion = 0;
	PlayInvincible = 0;
	//	Bless_HealLimit = 0;

	dwForceOfNatureTime = 0;	//Æ÷½º¿Àºê ³×ÀÌÃÄ À¯Áö½Ã°£
	dwHallOfValhallaTime = 0;	//È¦¿Àºê¹ßÇÒ¶ó À¯Áö½Ã°£


	AttackSkillRange = 0;

	RecvExp = 0;				//ÀÌÄ³¸¯À¸·Î ºÎÅÍ ¹ÞÀº °æÇèÄ¡ 

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

	dwForceOrbCode = 0;		//Æ÷½º¿Àºê Àû¿ë ÄÚµå
	dwForceOrbTime = 0;		//Æ÷½º¿Àºê À¯Áö½Ã°£

	// ¹ÚÀç¿ø - µ¥¹ÌÁö ºÎ½ºÅÍ(»ý¸í·Â)
	dwLifeBoosterCode = 0; // ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) ÄÚµå
	dwLifeBoosterTime = 0; // ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(»ý¸í·Â) À¯Áö½Ã°£
	// ¹ÚÀç¿ø - µ¥¹ÌÁö ºÎ½ºÅÍ(±â·Â)
	dwManaBoosterCode = 0; // ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) ÄÚµå
	dwManaBoosterTime = 0; // ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±â·Â) À¯Áö½Ã°£
	// ¹ÚÀç¿ø - µ¥¹ÌÁö ºÎ½ºÅÍ(±Ù·Â)
	dwStaminaBoosterCode = 0; // ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) ÄÚµå
	dwStaminaBoosterTime = 0; // ¹ÚÀç¿ø - ºÎ½ºÅÍ ¾ÆÀÌÅÛ(±Ù·Â) À¯Áö½Ã°£

	// Àåº° - ½ºÅ³ µô·¹ÀÌ
	dwSkillDelayCode = 0;
	dwSkillDelayTime = 0;

	dwLinkObjectCode = 0;
	lpLinkPlayInfo = 0;
	lpLinkChar = 0;

	PkMode_CharState = 0;
	dwClanCode = 0;
	UseObject_VirtualLife = 0;


#ifndef _W_SERVER
	//( Å¬¶óÀÌ¾ðÆ® ¿¡¼­¸¸ »ç¿ë )
	//¹«±â¿ë ÀÌÆåÆ® 
	WeaponEffect = 0;
	dwWeaponEffectTime = 0;

	ClanInfoNum = -1;					//Å¬·£ Á¤º¸ ÃÊ±âÈ­
	Clan_CastleMasterFlag = 0;				//¼ºÁÖ Å¬·£
	dwClanInfoTime = 0;
	TryAttackCount = 0;

	((DWORD*)sBlessCastle_Damage)[0] = 0;	//°ø¼º °³ÀÎº° µ¥¹ÌÁö Ãß°¡Ä¡


#else
	dwTargetLockTime = 0;

	lpCompulsionPlayInfo = 0;	//½Ã¼±²ø±â Ä³¸¯ÅÍ
	dwCompulsionTime = 0;		//½Ã¼±²ø±â ½Ã°£

	dwAssassinEyeTime = 0;		//¾î¼¼½Å¾ÆÀÌ Àû¿ë ¸ó½ºÅÍ
	AssassinEyeParam = 0;		//¾î¼¼½Å¾ÆÀÌ Àû¿ë ¸ó½ºÅÍ

	//¸ó½ºÅÍ ÀÌµ¿ ½ºÅÜ ±¸¼º
	sMoveStepCount[0] = 0;
	sMoveStepCount[1] = 0;

	DontMoveFlag = 0;

	Clan_CastleMasterFlag = 0;				//ºô¸µ ¸ó½ºÅÍ ÇÃ·¢ ( ¼­¹ö¿¡¼­ ¼­·Î °ø°Ý¾ÈÇÏµµ·Ï ÇÏ±âÀ§ÇÑ ±¸ºÐ¿ë )


#endif

}


int smCHAR::Close()
{
	Flag = 0;

	if (lpDinaPattern2) {
		AddRemoveDynPattern(lpDinaPattern2);
		//smDPAT_Delete( lpDinaPattern2 );
		lpDinaPattern2 = 0;
	}



	if (lpDinaPattern) {
		AddRemoveDynPattern(lpDinaPattern);
		//smDPAT_Delete( lpDinaPattern );
		lpDinaPattern = 0;
	}

	if (lpDinaLeftPattern) {
		AddRemoveDynPattern(lpDinaLeftPattern);
		//smDPAT_Delete( lpDinaLeftPattern );
		lpDinaLeftPattern = 0;
	}

	if (lpDinaRightPattern) {
		AddRemoveDynPattern(lpDinaRightPattern);
		//smDPAT_Delete( lpDinaRightPattern );
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


//ÆÐÅÏ¼³Á¤
int smCHAR::SetPattern(smPAT3D* pat)
{
	smPAT3D* AnimPat;

	Pattern = pat;

	if (pat->TmParent) AnimPat = pat->TmParent;
	else AnimPat = pat;

	HvRightHand.ObjBip = AnimPat->GetObjectFromName(szBipName_RightHand);

	//¿ÞÂÊ µµ±¸
	if ((HvLeftHand.dwItemCode & sinITEM_MASK2) == sinDS1)
		HvLeftHand.ObjBip = AnimPat->GetObjectFromName(szBipName_Shield);		//¹æÆÐ
	else
		HvLeftHand.ObjBip = AnimPat->GetObjectFromName(szBipName_LeftHand);	//±âÅ¸ È°µî


	BackObjBip[0] = AnimPat->GetObjectFromName(szBipName_BackSpine);
	BackObjBip[1] = AnimPat->GetObjectFromName(szBipName_BackSpineBow);
	BackObjBip[2] = AnimPat->GetObjectFromName(szBipName_BackSpineCross);
	ShieldObjBip = AnimPat->GetObjectFromName(szBipName_Shield2);

	//	if ( !BackObjBip[1] ) BackObjBip[1]=BackObjBip[0];
	//	if ( !BackObjBip[2] ) BackObjBip[2]=BackObjBip[0];

	AttackObjBip = 0;

	//¿©ÀÚ ±âº» °ø°Ý ¹ßÂ÷±â ( ¾ÆÃÄ , ¾ÆÅ»¶õÅ¸ )
	if (smCharInfo.JOB_CODE == 3 || smCharInfo.JOB_CODE == 5)
		AttackObjBip = AnimPat->GetObjectFromName(szBipName_LFoot);

	if (smCharInfo.JOB_CODE == 7) //¸ÞÁö¼Ç ¹ÚÄ¡±â
		AttackObjBip = AnimPat->GetObjectFromName(szBipName_Head);

	AnimPattern = AnimPat;

	PatHeight = pat->SizeHeight;
	PatWidth = (pat->SizeWidth * 160) >> FLOATNS;

	//	PatSizeLevel

	return TRUE;
}

//ÆÐÅÏ¸Ó¸® ¼³Á¤
int smCHAR::SetPattern2(smPAT3D* pat)
{
	//	smPAT3D *AnimPat;

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
				//smDPAT_Delete( lpDinaRightPattern );
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
					//¿À¸¥ÂÊ µµ±¸´Â ¼Õ¿¡ ºÙÀÓ
					HvRightHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_RightHand);
					dwActionItemCode = dwItemCode;

					//wStickItems[smPLAYBUFF_ITEM_ACTION] = GetSinItemNumFromCode( dwActionItemCode );
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
				//smDPAT_Delete( lpDinaLeftPattern );
				lpDinaLeftPattern = 0;
			}
			if (dwItemCode) {
				//				lpDinaLeftPattern = smPattern.LoadCharactor( "Shield\\Ds101.ASE" );
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

					//¿ÞÂÊ µµ±¸
					if ((dwItemCode & sinITEM_MASK2) == sinDS1 || (dwItemCode & sinITEM_MASK2) == sinOM1) {
						if ((dwItemCode & sinITEM_MASK2) == sinDS1) {
							//¹æÆÐ
							HvLeftHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_Shield);		//¹æÆÐ
						}
						else {
							//¸ÞÁöÄÃ ½ºÅÍÇÁ
							HvLeftHand.PatTool = 0;
							HvLeftHand.dwItemCode = 0;
							wStickItems[smPLAYBUFF_ITEM_LEFT] = -1;
						}
					}
					else {
						HvLeftHand.ObjBip = AnimPattern->GetObjectFromName(szBipName_LeftHand);	//±âÅ¸ È°µî
						dwActionItemCode = dwItemCode;
						//wStickItems[smPLAYBUFF_ITEM_ACTION] = GetSinItemNumFromCode( dwActionItemCode );
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
			//wStickItems[smPLAYBUFF_ITEM_ACTION] = -1;
		}

		//°ø°Ý »çÁ¤°Å¸® ¼³Á¤
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
		//È° , ¸¶¹ý ÁöÆÎÀÌ
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
			//¾ç¼Õ ¹«±â Ã½Å©
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

	/*
		if ( lpD3DDevice ) {
			ReadTextures();
			if ( HvRightHand.PatTool ) HvRightHand.PatTool->smMaterialGroup->CheckMatreialTextureSwap();
			if ( HvLeftHand.PatTool ) HvLeftHand.PatTool->smMaterialGroup->CheckMatreialTextureSwap();
		}
	*/
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

	if (pY == 0) {

		if (smGameStage[0])
			pY = smGameStage[0]->GetHeight(pX, pZ);

		if (smGameStage[1]) {
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

	if (ActionPattern == 0) {
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


//´ÙÀ½ ¸ñÀû ÁÂÇ¥ ¼³Á¤
int smCHAR::SetNextTarget(int x, int y, int z)
{
	ptNextTarget.x = x;
	ptNextTarget.y = y;
	ptNextTarget.z = z;

	return TRUE;
}

int smCHAR::SetAction(int nAction)
{
	//			if ( CheckLocateBox( pX, pZ , tx, tz , 16*fONE )==TRUE )

	if (nAction == 0) MoveFlag = FALSE;

	return TRUE;

}

//¸ÕÁö »ý¼º
int	smCHAR::SetSmoking(int level)
{
	return TRUE;
}

//ÀÌµ¿ ( Á¤¹Ð )
int smCHAR::MoveAngle2(int step)
{
	int x, z, y;
	int he, hy;
	int OldStageField;
	POINT3D MovePosi;
	int MovLevel[2];

	WaterHeight = CLIP_OUT;

#ifndef _W_SERVER

	if (smConfig.DebugMode && VRKeyBuff['N']) {

		x = pX + (((GetSin[Angle.y & ANGCLIP] >> FLOATNS) * step) >> FLOATNS);
		z = pZ + (((GetCos[Angle.y & ANGCLIP] >> FLOATNS) * step) >> FLOATNS);
		y = pY;

		//y = smStage->GetHeight( x , z );
		//if ( abs( pY-y )>(16*fONE) ) return FALSE;

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
		if (lpStage && Pattern && lpStage->CheckNextMove(&Posi, &Angle, &MovePosi, step, Pattern->SizeWidth, Pattern->SizeHeight, OverLapPosi2) > 0) {
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
						if (he < MovePosi.y) {//Posi.y ) {
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


//ÀÌµ¿
int smCHAR::MoveAngle(int step)
{
	return	MoveAngle2(step * fONE);
}

int smCHAR::SetDinaPattern(smDPAT* lpDPat)
{

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
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
	//######################################################################################

	lpDinaPattern = lpDPat;
	smMotionInfo = lpDinaPattern->lpModelInfo;
	SetPattern(lpDPat->Pat);

	SetMotionFromCode(CHRMOTION_STATE_STAND);

	return TRUE;
}

int smCHAR::SetDinaPattern2(smDPAT* lpDPat)
{
	lpDinaPattern2 = lpDPat;

	// Correção CRASH
	if (lpDinaPattern2 == nullptr || !lpDPat)
		return FALSE;
	// CORREÇÃO CRASH

	if (lpDinaPattern2) {
		smMotionInfo2 = lpDinaPattern2->lpModelInfo;
		SetPattern2(lpDPat->Pat);

		if (Pattern->TmParent == Pattern2->TmParent) {
			AnimDispMode = 0;				//°°ÀÌ Ãâ·Â
			PatDispMode = DISP_MODE_PATMAIN | DISP_MODE_PATSUB;
		}
		else {
			AnimDispMode = 1;				//ÇÏ³ª¾¿ Ãâ·Â
			PatDispMode = DISP_MODE_PATMAIN;
		}
	}
	else {
		AnimDispMode = 0;				//°°ÀÌ Ãâ·Â
		PatDispMode = DISP_MODE_PATMAIN;
		SetPattern2(0);
	}

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
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
			// ¿¡·¯ Ã³¸®. ÀÛµ¿µÇÁö ¾Ê°Ô²û.
			PatDispMode &= (~DISP_MODE_PATTALK);
			TalkPattern = NULL;
		}
	}
	//######################################################################################

	return TRUE;
}

//¿òÁ÷ÀÓ µ¿ÀÛ ¼³Á¤
int smCHAR::ChangeMotion(int Motion, int DpMode)
{

	if (DpMode) {
		//´ÙÁß ¸ðµ¨ÀÏ °æ¿ì ¿òÁ÷ÀÓ ¼±ÅÃ
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
		//±âº» ¸ðµ¨ ¿òÁ÷ÀÓÀ¸·Î ¼³Á¤
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

	//######################################################################################
	//ÀÛ ¼º ÀÚ : ¿À ¿µ ¼®
	FrameInfo.StartFrame = frame;
	FrameInfo.EndFrame = MotionInfo->EndFrame * 160;
	//######################################################################################

	return TRUE;
}

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

	if (lpTalkMotionInfo)
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

	if (smCharInfo.JOB_CODE && smCharInfo.JOB_CODE < 16)
		dwJobCodeMask = BitMaskJobNames[smCharInfo.JOB_CODE - 1].dwBitMask;
	else
		dwJobCodeMask = 0;


	FindCnt = 0;
	FindCnt2 = 0;
	MotionSelectFrame = 0;

	dwActItemCode = dwActionItemCode;

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

	// Correção crash motion aqui
	if (!smMotionInfo || smMotionInfo == NULL)
		return FALSE;

	lpMotionInfo = smMotionInfo;

	if (MotionCode == CHRMOTION_STATE_SKILL && AttackSkil) {
		atSkill = AttackSkil;
	}

	for (cnt = 0; cnt < (int)smMotionInfo->MotionCount; cnt++) {
		//°¢Á¾ Á¶°Ç¿¡ ¸Â´Â ¸ð¼ÇÀ» Ã£´Â´Ù ( µ¿ÀÛ/¸ÊÀ§Ä¡/Á÷¾÷ µîÀ» °í·Á )
		if (smMotionInfo->MotionInfo[cnt].State == MotionCode &&
			(!smMotionInfo->MotionInfo[cnt].MapPosition || (smMotionInfo->MotionInfo[cnt].MapPosition & StageVillage)) &&
			(!smMotionInfo->MotionInfo[cnt].dwJobCodeBit || !dwJobCodeMask ||
				(smMotionInfo->MotionInfo[cnt].dwJobCodeBit & dwJobCodeMask))) {

			if (atSkill) {
				//½ºÅ³ ¸ð¼ÇÀÏ °æ¿ì
				cnt2 = 0;
				skillCode = atSkill & 0xFF;
				while (1) {
					//½ºÅ³ ÄÚµå °Ë»ö
					bSkill = smMotionInfo->MotionInfo[cnt].SkillCodeList[cnt2];
					if (bSkill == 0) break;
					if (SkillDataCode[bSkill].dwPlaySkillCode == skillCode) {
						break;
					}
					cnt2++;
				}
				if (bSkill) {
					//½ºÅ³ °Ë»ö ¼º°ø
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
		//2Â÷ ¸ðµ¨¿¡¼­ Á¤º¸¸¦ Ã£À½
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

	/*
	#define MOTION_LIST_MAX			32
	#define MOTION_INFO_MAX			512
	#define	MOTION_TOOL_MAX			20
	#define	MOTION_JOB_MAX			12


	struct smMOTIONINFO {
		DWORD	State;							//»óÅÂ TRUE¸é À¯È¿
		DWORD	StartFrame;						//½ÃÀÛ ÇÁ·¹ÀÓ
		DWORD	EndFrame;						//Á¾·á ÇÁ·¹ÀÓ
		DWORD	EventFrame[4];					//ÀÌº¥Æ® »ý¼º ÇÁ·¹ÀÓ

		int		ItemCodeCount;					//ÇØ´ç ¾ÆÀÌÅÛ ¸®½ºÆ® Ä«¿îÅÍ ( 0 ¾øÀ½  -1 ÀüÃ¼ ÇØ´ç )
		DWORD	ItemCodeList[MOTION_TOOL_MAX];	//ÇØ´ç ¾ÆÀÌÅÛ ÄÚµå ¸®½ºÆ®
		BYTE	JobCodeList[MOTION_JOB_MAX];	//ÇØ´ç Á÷¾÷ ÄÚµå ¸®½ºÆ®

		int		MapPosition;					//ÇØ´ç ¸Ê Àû¿ë ( 0-°ü°è¾øÀ½ ¸¶À» 1 - ÇÊµå 2 )

		DWORD	Repeat;							//¹Ýº¹ ¿©ºÎ
		CHAR	KeyCode;						//µ¿ÀÛ ½ÃÀÛ Å°
		int		MotionFrame;					//¸ð¼Ç ¿¬°á ÆÄÀÏ ¹øÈ£
	};
	*/

	if (FindCnt2 == 0) {
		if (FindCnt == 1)
			ChangeMotion(FoundNum[0], AnimDispMode);
		else
			if (FindCnt) ChangeMotion(FoundNum[(rand() % FindCnt)], AnimDispMode);

		if (atSkill) AttackSkil = atSkill;

		return NULL;
	}

	//FoundNum2[0] = 115;FindCnt2=1;

	if (FindCnt2 == 1) ChangeMotion(FoundNum2[0], AnimDispMode);
	else
		if (FindCnt2) ChangeMotion(FoundNum2[(rand() % FindCnt2)], AnimDispMode);


	if (atSkill) AttackSkil = atSkill;

	return FindCnt;
}


//ÇÁ·¹ÀÓ ¹øÈ£·Î µ¿ÀÛÀ» Ã£´Â´Ù
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

//¸ó½ºÅÍ ¹× ÀÚµ¿Ä³¸¯ÅÍ¿ë ÇÁ·¹ÀÓ ÀúÀå (¼­¹ö¿ë)
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



//ÀÌµ¿ ¸ðµå º¯°æ
int smCHAR::ChangeMoveMode()
{
	//CHRMOTION_STATE_RUN
	/*
								if ( !SetMotionFromCode( CHRMOTION_STATE_SOMETIME ) ) {
									Angle.y = (cnt<<4)&ANGCLIP;
									ChangeMotion( CHRMOTION_WALK );
								}
							}
							else {
								if ( !SetMotionFromCode( TRUE ) ) {
									Angle.y = (cnt<<4)&ANGCLIP;
									ChangeMotion( CHRMOTION_WALK );
	*/
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

//°ø°Ý¹ÞÀº ½Ã°£ °Ë»çÇÏ¿© ¿ø°Å¸® °ø°Ý Å½Áö
int smCHAR::CheckShootingTest_LastAttackTime(smCHAR* lpChar)
{
	if (dwLastRecvAttackTime<dwPlayTime && (dwLastRecvAttackTime + 2000)>dwPlayTime)
		return TRUE;

	return CheckShootingTest(lpChar);
}

//¹ß»çÇü ¹«±â ¸ÞÀÎÇÔ¼ö
int smCHAR::CheckShootingTest(smCHAR* lpChrTarget)
{
	//	POINT3D Posi;
	int x, y, z;
	int he;//,hy;
	int step;
	int cnt;

	int angX, angY;
	int	SucessCnt;

	if (lpChrTarget->smCharInfo.dwCharSoundCode == snCHAR_SOUND_CASTLE_TOWER_B) 	//¹ßÇÒ¶óÅ¸¿ö Á¦¿Ü
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

		//ÁöÁ¤µÈ ¹æÇâÀÇ °Å¸®¸¦ ÀÕ´Â ¼±ºÐ¿¡ Æú¸®°ÇÀÌ Ãæµ¹ÇÏ´ÂÁö È®ÀÎ 

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
				//return TRUE;
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



//¹ß»çÇü ¹«±â ¸ÞÀÎÇÔ¼ö
int smCHAR::ShootingMain()
{

	POINT3D Posi;
	int x, y, z;
	int he;//,hy;
	int ShootSize;
	int step;
	int r, g, b;
	int EffectFlag;

	he = CLIP_OUT;
	EffectFlag = 0;

	ShootSize = 512;

	//	step = 1*fONE;
	if ((dwActionItemCode & sinITEM_MASK2) == sinWS1)
		step = 8 * fONE;				//È­»ì
	else
		step = 6 * fONE;				//Ã¢ÀÌ³ª ±âÅ¸ ´øÁö´Â ¹«±â

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
				EffectFlag++;
			}
		}
		if (dwActionItemCode == HvLeftHand.dwItemCode) {
			if (HvLeftHand.PatTool && HvLeftHand.ColorBlink && !HvLeftHand.EffectKind) {

				r = (HvLeftHand.sColors[SMC_R] >> 1) + 128;
				g = (HvLeftHand.sColors[SMC_G] >> 1) + 128;
				b = (HvLeftHand.sColors[SMC_B] >> 1) + 128;
				EffectFlag++;
			}
		}
	}

	//	y += fONE* ;
	/*
			he = TRUE;

			//ÁöÁ¤µÈ ¹æÇâÀÇ °Å¸®¸¦ ÀÕ´Â ¼±ºÐ¿¡ Æú¸®°ÇÀÌ Ãæµ¹ÇÏ´ÂÁö È®ÀÎ

			if ( lpStage ) {
				he = lpStage->CheckVecImpact( &ShootingPosi, &ShootingAngle , step );
			}
			else {
				if ( smGameStage[0] ) {
					he = smGameStage[0]->CheckVecImpact( &ShootingPosi, &ShootingAngle , step );
				}

				if ( he==TRUE && smGameStage[1] ) {
					he = smGameStage[1]->CheckVecImpact( &ShootingPosi, &ShootingAngle , step );
				}
			}
	*/
	if (ShootingCount > 60 * 2) { //|| he==FALSE  ) {
		ShootingFlag = FALSE;
	}

	ShootingCount++;

	if (ShootingFlag > 1 && chrAttackTarget) {
		ShootingFlag++;
		if (ShootingFlag > 8) ShootingFlag = FALSE;
		return TRUE;
	}

	if (ShootingFlag == 1 && chrAttackTarget && AttackCritcal >= 0) {

		x = abs(ShootingPosi.x - chrAttackTarget->pX);
		z = abs(ShootingPosi.z - chrAttackTarget->pZ);
		y = abs(ShootingPosi.y - chrAttackTarget->pY);

		if (x < 64 * fONE && z < 64 * fONE && y < 80 * fONE) {
			ShootingFlag++;


			if (ShootingKind == SKILL_PLAY_FARINA) {
				//ÆÄ¸®³ª ÀÌÆÑÆ®
				Posi.x = chrAttackTarget->pX;
				Posi.y = chrAttackTarget->pY + 22 * fONE;
				Posi.z = chrAttackTarget->pZ;

			}
			//°ø°Ý¼Ò¸®
			//PlayWav( 9 , 0 , 300 , 2205 );

			//°æÇèÄ¡ ÀÏºÎ »ó½Â
			if (this == lpCurPlayer && AttackExp > 0) {
				//AddExp( AttackExp );
				//SetTotalAddExp( AttackExp );			//°æÇèÄ¡ Áõ°¡ ±â·Ï
				AttackExp = 0;
			}

		}

	}

	return TRUE;
}

//½ºÅ³ Â÷Â¡
int smCHAR::ChargingSkill()
{
	DWORD dwFrame;

	if (this != lpCurPlayer) return FALSE;

	dwFrame = frame - MotionInfo->StartFrame * 160;

	if (SkillChargingFlag) SkillChargingFlag = 0;

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

//°ø°Ý ÀÌº¥Æ®
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

	//	if ( MotionInfo->State<0x100 ) return FALSE;

	ChargingSkill();

	if (AttackSkil > 0 || (smCharInfo.State == smCHAR_STATE_ENEMY && MotionInfo->State == CHRMOTION_STATE_SKILL)) {
		if (Flag || ((MotionInfo->State == CHRMOTION_STATE_ATTACK || MotionInfo->State == CHRMOTION_STATE_SKILL) && MotionInfo->EventFrame[0])) {
			compFrame = frame - (MotionInfo->StartFrame * 160);
			//if ( MotionInfo->EventFrame[0]>(compFrame-FrameStep) && MotionInfo->EventFrame[0]<=compFrame ) {
			if ((MotionInfo->EventFrame[0] > (compFrame - FrameStep) && MotionInfo->EventFrame[0] <= compFrame) ||
				(MotionInfo->EventFrame[1] && MotionInfo->EventFrame[1] > (compFrame - FrameStep) && MotionInfo->EventFrame[1] <= compFrame) ||
				(MotionInfo->EventFrame[2] && MotionInfo->EventFrame[2] > (compFrame - FrameStep) && MotionInfo->EventFrame[2] <= compFrame) ||
				(MotionInfo->EventFrame[3] && MotionInfo->EventFrame[3] > (compFrame - FrameStep) && MotionInfo->EventFrame[3] <= compFrame) ||

				(dwEventFrameExt[0] && dwEventFrameExt[0] > (frame - FrameStep) && dwEventFrameExt[0] <= frame) ||
				(dwEventFrameExt[1] && dwEventFrameExt[1] > (frame - FrameStep) && dwEventFrameExt[1] <= frame) ||
				(dwEventFrameExt[2] && dwEventFrameExt[2] > (frame - FrameStep) && dwEventFrameExt[2] <= frame) ||
				(dwEventFrameExt[3] && dwEventFrameExt[3] > (frame - FrameStep) && dwEventFrameExt[3] <= frame) ||

				(MotionEvent == 0 && MotionInfo->EventFrame[0] <= compFrame)) {

				//½ºÅ³ÀÌº¥Æ®
				MotionEvent++;

				//°æÇèÄ¡ ÀÏºÎ »ó½Â ( ÁÖÀÎ°ø ¸¸ )
				if (this == lpCurPlayer) {//&& AttackExp>0 ) { 
					//AddExp( AttackExp );
					//SetTotalAddExp( AttackExp );			//°æÇèÄ¡ Áõ°¡ ±â·Ï
					AttackExp = 0;

					if (chrAttackTarget && abs(chrAttackTarget->smCharInfo.Level - smCharInfo.Level) <= AGING_SUB_LEVEL) {
						//·¹º§ 7Â÷ÀÌ ÀÌÇÏ´Â ¿¡ÀÌÂ¡ Àû¿ë
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

			//°æÇèÄ¡ ÀÏºÎ »ó½Â ( ÁÖÀÎ°ø ¸¸ )
			if (this == lpCurPlayer && AttackExp > 0) {

				//AttackExp = 500000;

				//AddExp( AttackExp );
				//SetTotalAddExp( AttackExp );			//°æÇèÄ¡ Áõ°¡ ±â·Ï
				AttackExp = 0;

				//SaveGameData();					

				if (chrAttackTarget && abs(chrAttackTarget->smCharInfo.Level - smCharInfo.Level) <= AGING_SUB_LEVEL) {
					//·¹º§ 7Â÷ÀÌ ÀÌÇÏ´Â ¿¡ÀÌÂ¡ Àû¿ë
					if (AttackCritcal == TRUE)
						sinCheckAgingLevel(SIN_AGING_CRITICAL, false);
					else
						sinCheckAgingLevel(SIN_AGING_ATTACK, false);
				}

			}

			//ÁÖÀÎ°øÀ» ÇâÇÑ °ø°Ý
			if (chrAttackTarget == lpCurPlayer) {
				//°ø°Ý ¹ÞÀº ¸ð¼Ç Ä«¿îÅÍ
				AttackMotionCountUp();
			}

			switch (smCharInfo.dwCharSoundCode) {
			case snCHAR_SOUND_MUSHROOM:
				//¹ö¼¸ µ¶°¡½º ¹ß»ç
				break;

			case snCHAR_SOUND_IMP:
				//ÀÓÇÁ ¸ÅÇÍ
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 10 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_SHADOW:
				//½¦µµ¿ì °ø°Ý
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_FIGON:
				//ÆÄÀÌ°ï °ø°Ý
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_WEB:
				//´«Æ¢¾î³ª¿Â °Å¹Ì
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);
					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_SKELETONRANGE:
				//È°½î´Â ÇØ°ñ
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
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_WOLVERLIN:
				//À»¹ö¸°
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 50 * fONE, 0, Angle.y, 0);
					x = pX + GeoResult_X;
					z = pZ + GeoResult_Z;
					y = pY + 18 * fONE;

				}
				break;

			case snCHAR_SOUND_EGAN:
				//ÀÌ°£
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						GetMoveLocation(0, 0, 10 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;

					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_MUTANTTREE:
				//¹ÂÅÏÆ® Æ®¸®
				if (chrAttackTarget) {
					if (CheckShootingTest_LastAttackTime(chrAttackTarget)) {
						GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
						x = pX + GeoResult_X;
						y = pY + 8 * fONE;
						z = pZ + GeoResult_Z;


					}
					else {
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_HULK:
				//ÇæÅ©
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
				}
				break;


			case snCHAR_SOUND_DMACHINE:
				//µð¸Ó½Å
				if (chrAttackTarget) {
					GetMoveLocation(0, 30 * fONE, 10 * fONE, 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_SUCCUBUS:
				//½áÅ¥¹ö½º
				if (chrAttackTarget)
				{

					GetMoveLocation(0, 20 * fONE, 20 * fONE, 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_STYGIANLORD:
				//½ºÆ¼Áö¾ð·Îµå
				if (chrAttackTarget) {
					//GetMoveLocation( 0 , 20*fONE, 20*fONE , 0, Angle.y , 0 );
					//StartEffectMonsterDest(pX+GeoResult_X, pY+GeoResult_Y,pZ+GeoResult_Z, 

				}
				break;

			case snCHAR_SOUND_MOKOVA:				//¸ðÄÚ¹Ù
			case snCHAR_SOUND_OMICRON:
				//ÇæÅ© ²¨¶û µ¿ÀÏ
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_GUARDIAN_SAINT:
				//°¡µð¾È - ¼¼ÀÎÆ®
				break;

			case snCHAR_SOUND_HYPERMACHINE:		//ÇÏÀÌÆÛ ¸Ó½Å
				if (chrAttackTarget) {
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY + 20 * fONE;
					pos.z = chrAttackTarget->pZ;
				}
				break;

			case snCHAR_SOUND_MOUNTAIN:			//¸¶¿îÆ¾
				break;

			case snCHAR_SOUND_TOWERGOLEM:
				break;

			case snCHAR_SOUND_OMU:
				break;

			case snCHAR_SOUND_CHAINGOLEM:
			case snCHAR_SOUND_DEADZONE:
			case snCHAR_SOUND_IRONFIST:
			case snCHAR_SOUND_MORGON:
			case snCHAR_SOUND_RAMPAGE:
			case snCHAR_SOUND_VAMPRICBEE:
			case snCHAR_SOUND_IRONGUARD:
			case snCHAR_SOUND_GORGON:	// pluto Ãß°¡ ¸ó½ºÅÍ °í¸£°ï
			case snCHAR_SOUND_HOBOGOLEM: // pluto Ãß°¡ ¸ó½ºÅÍ È£º¸°í·½

				GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y + 32 * fONE;
				pos.z = pZ + GeoResult_Z;
				break;

			case snCHAR_SOUND_GOBLINSHAMAN: // ¹ÚÀç¿ø - ÀúÁÖ¹ÞÀº ½ÅÀü 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) - °íºí¸°»þ¸Õ

				break;

			case snCHAR_SOUND_GROTESQUE:
				GetMoveLocation(0, 0, 75 * fONE, 0, Angle.y, 0);
				pos.x = pX + GeoResult_X;
				pos.y = pY + GeoResult_Y + 32 * fONE;
				pos.z = pZ + GeoResult_Z;
				break;


			case snCHAR_SOUND_AVELIN_ARCHER:

				break;

			case snCHAR_SOUND_AVELIN_QUEEN:

				break;

			case snCHAR_SOUND_RUNICGUARDIAN:

				break;

			case snCHAR_SOUND_BABEL:				//¹Ùº§ (·ç´Ð°¡µð¾È º¸½º)

				if (chrAttackTarget) {
					if (Trans_SplashCharList.code == smTRANSCODE_ATTACK_SLASH_LIST && Trans_SplashCharList.TargetCount > 1) {
						ChrCnt = 0;
						for (cnt = 0; cnt < Trans_SplashCharList.TargetCount; cnt++) {
							if (cnt >= 32) break;
							lpCharList[ChrCnt] = FindChrPlayer(Trans_SplashCharList.dwTarObjectSerial[cnt]);
							if (lpCharList[ChrCnt]) ChrCnt++;
						}
						Trans_SplashCharList.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_SADNESS:			//»õµå´Ï½º

				break;

				// Àåº° - ¼ö¹Ú¸ó½ºÅÍ ÀÏ¹Ý°ø°Ýµ¿ÀÛ
			case snCHAR_SOUND_WATERMELON:
				if (chrAttackTarget)
				{
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;
				}
				break;


				////////////////// ¾ÆÀÌ½º ¸ó½ºÅÍ /////////////////////
			case snCHAR_SOUND_COLDEYE:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 24 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + 64 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 64 * fONE;
					pos2.z = chrAttackTarget->pZ;
				}
				break;

			case snCHAR_SOUND_FROZEN:

				break;

			case snCHAR_SOUND_FROST:

				break;

			case snCHAR_SOUND_CHAOSCARA:

				break;

			case snCHAR_SOUND_DEATHKNIGHT:			//¹ß·»Åä

				break;


				////////////////// ±×¸®µðÈ£¼ö ¸ó½ºÅÍ /////////////////////
			case snCHAR_SOUND_GREATE_GREVEN:
			case snCHAR_SOUND_LIZARDFOLK:
			case snCHAR_SOUND_SPIDER:
			case snCHAR_SOUND_STRIDER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_M_LORD:

				break;


				////////////////// // ¹ÚÀç¿ø - ³¡¾ø´Â Å¾ 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) /////////////////////
			case snCHAR_SOUND_IGOLATION: // ÀÌ°ñ¶óÆ¼¿Â : ÀÏ¹Ý °ø°Ý
				if (chrAttackTarget && MotionInfo->KeyCode == 'H')
				{
					GetMoveLocation(0, 0, 68 * fONE, 0, Angle.y, 0);
				}
				break;
			case snCHAR_SOUND_NIKEN:  // ´ÏÄË : ÀÏ¹Ý °ø°Ý
				if (chrAttackTarget)
				{
					pos.x = chrAttackTarget->pX;
					pos.y = chrAttackTarget->pY;
					pos.z = chrAttackTarget->pZ;

				}
				break;
			case snCHAR_SOUND_XETAN: // Á¦Åº : ÀÏ¹Ý °ø°Ý
				if (chrAttackTarget && MotionInfo->KeyCode == 'G')
				{
					GetMoveLocation(0, 40 * fONE, 20 * fONE, 0, Angle.y, 0);

				}
				break;
			case snCHAR_SOUND_SPRIN: // ½ºÇÁ¸° : ÀÏ¹Ý °ø°Ý
				if (chrAttackTarget && (MotionInfo->KeyCode == 'G' || MotionInfo->KeyCode == 'H'))
				{
					if (CheckShootingTest_LastAttackTime(chrAttackTarget) ||
						CheckLocateBox(pX, pY, pZ, chrAttackTarget->pX, chrAttackTarget->pY, chrAttackTarget->pZ, 80 * fONE)) {

						GetMoveLocation(0, 0, 32 * fONE, 0, Angle.y, 0);

					}
					else {
						ResetAttackTrans();
					}
				}
				break;


				////////////////// // ¹ÚÀç¿ø - º¹³¯ ÀÌº¥Æ® ¸ó½ºÅÍ Ãß°¡ /////////////////////

			case snCHAR_SOUND_BEBECHICK:	// ¾Æ±â ²¿²¿
				if (chrAttackTarget && (MotionInfo->KeyCode == 'H' || MotionInfo->KeyCode == 'J'))
				{
					//  ÀÌÆåÆ® Á¤ÀÇ ¾ÈÇÔ.(¸ð¼Ç¸¸ ¼³Á¤)
				}
				break;
			case snCHAR_SOUND_PAPACHICK:	// ¾Æºü ²¿²¿
				if (chrAttackTarget && MotionInfo->KeyCode == 'H')
				{
					//  ÀÌÆåÆ® Á¤ÀÇ ¾ÈÇÔ.(¸ð¼Ç¸¸ ¼³Á¤)
				}
				break;

				/////////////////////////////////////////////////////////////////////////////////////

			case snCHAR_SOUND_KELVEZU:

				break;

				////////////////////////////////////////////////////
/*
					if (chrAttackTarget) {
						GetMoveLocation( 0 , 0, 54*fONE , 0, Angle.y , 0 );
						StartEffectMonster( pX+GeoResult_X, pY+48*fONE,pZ+GeoResult_Z, MONSTER_HULK_HIT1);
					}
*/
			case snCHAR_SOUND_DARKPHALANX:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;
			case snCHAR_SOUND_BLOODYKNIGHT:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_CHIMERA:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_FIREWORM:

				break;
			case snCHAR_SOUND_HELLHOUND:
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;
			case snCHAR_SOUND_DARKMAGE:

				break;
			case snCHAR_SOUND_DARKGUARD:
				//È°½î´Â ´ÙÅ©°¡µå
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
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			case snCHAR_SOUND_TEMPLEGUARD:
				if (MotionInfo->KeyCode != 'I') {
				}
				else {
					GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;
				}
				break;

			case snCHAR_SOUND_SETO:
				if (chrAttackTarget) {
					GetMoveLocation(0, 24 * fONE, 24 * fONE, 0, Angle.y, 0);
				}
				break;

			case snCHAR_SOUND_KINGSPIDER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 70 * fONE, 0, Angle.y, 0);
				}
				break;

				////////////////////////////////////////////////////

			case snCHAR_SOUND_NPC_MORIF:		//¹Í½ºÀåÀÎ ¸ð¸®ÇÁ
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_NPC_MOLLYWOLF:		//¼ÛÆíÈÉÄ£´Á´ë
				if (chrAttackTarget) {

					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_WORLDCUP:				//¿ùµåÄÅ ¸ó½ºÅÍ

				break;

			case snCHAR_SOUND_S_F_ELEMENTAL:		//¼ÒÈ¯¸÷ È­ÀÌ¾î¿¤¸®¸àÅ»
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 32 * fONE;
					pos.z = pZ + GeoResult_Z;

					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_S_METALGOLEM:		//¼ÒÈ¯¸÷ ¸ÞÅ»°í·½
				if (chrAttackTarget) {
					if (!smCharInfo.wPlayClass[1])
						GetMoveLocation(0, 0, 54 * fONE, 0, Angle.y, 0);
					else
						GetMoveLocation(0, 0, 54 * smCharInfo.wPlayClass[1], 0, Angle.y, 0);

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
				}
				break;

			case snCHAR_SOUND_NPC_SKILLMASTER:
				break;

			case snCHAR_SOUND_NPC_MAGICMASTER:
				if (chrAttackTarget) {
					GetMoveLocation(0, 0, 25 * fONE, 0, Angle.y, 0);

				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_R:	//»¡°£ Å©¸®½ºÅ» Å¸¿ö
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;


				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_G:	//±×¸° Å©¸®½ºÅ» Å¸¿ö
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;
				}
				break;

			case snCHAR_SOUND_CASTLE_CRYSTAL_B:	//ÆÄ¶û Å©¸®½ºÅ» Å¸¿ö
				if (chrAttackTarget) {
					y = GetRadian2D(pX, pZ, chrAttackTarget->pX, chrAttackTarget->pZ);
					GetMoveLocation(0, 0, 16 * fONE, 0, y, 0);
					pos.x = pX + GeoResult_X;
					pos.y = pY + GeoResult_Y + 136 * fONE;
					pos.z = pZ + GeoResult_Z;
					pos2.x = chrAttackTarget->pX;
					pos2.y = chrAttackTarget->pY + 12 * fONE;
					pos2.z = chrAttackTarget->pZ;

				}
				break;

			case snCHAR_SOUND_CASTLE_SOLDER_A:
			case snCHAR_SOUND_CASTLE_SOLDER_B:
			case snCHAR_SOUND_CASTLE_SOLDER_C:

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
						ResetAttackTrans();		//AttackTrans.code = 0;
					}
				}
				break;

			}

			if (this != lpCurPlayer) {
				PlayAttack();									//°ø°Ý Àû¿ë
			}
			if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER) return TRUE;

			if (!ShootingMode) {
				if (GetAttackPoint(&x, &y, &z) == TRUE) {
					if (WeaponEffect) {
						pos.x = x;
						pos.y = y;
						pos.z = z;
					}
					else
					{



						if (dwForceOrbTime > dwPlayTime)
						{		//Æ÷½º¿Àºê °ø°Ý ÀÌÆåÆ®
							pos.x = x;
							pos.y = y;
							pos.z = z;

						}
					}
				}
			}
			else {
				//¹ß»çÇü °ø°Ý
				wp = (dwActionItemCode & sinITEM_MASK2);
				if (!shootFlag && chrAttackTarget) {
					GetAttackPoint(&x, &y, &z);

					if (wp == sinWS1 || wp == sinWT1) {


						if (smCharInfo.JOB_CODE == JOBCODE_ATALANTA &&
							(dwDispAppSkill & SKILL_APPMASK_GODLY_SHIELD) && wp == sinWT1) {
							//ÇÁ·Î½ºÆ® Á¦ºí¸° ½ºÅ³ ÀÌÆåÆ® (¾ÆÅ»¶õÅ¸)
							pos.x = x;
							pos.y = y;
							pos.z = z;
							pos2.x = chrAttackTarget->pX;
							pos2.y = chrAttackTarget->pY + 24 * fONE;
							pos2.z = chrAttackTarget->pZ;



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
						//¸¶¹ý ÁöÆÎÀÌ

						if (WeaponEffect) {
							switch (WeaponEffect) {
							case (sITEMINFO_LIGHTING + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								break;
							case (sITEMINFO_FIRE + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;
								break;
							case (sITEMINFO_ICE + 1):
								pos.x = x;
								pos.y = y;
								pos.z = z;
								pos2.x = chrAttackTarget->pX;
								pos2.y = chrAttackTarget->pY + 24 * fONE;
								pos2.z = chrAttackTarget->pZ;

								break;
							}
						}
					}
				}
			}
		}
	}


	return TRUE;

}

extern int MatLight[2];

//¼Õ¿¡ È¿°ú ÁÖ±â ( ÆÄÀÌ°ï ¿ë )
int smCHAR::StartHandEffect(int Mode) { return TRUE; }

#define SERVER_CHAR_FRAME_STEP			320
#define SERVER_CHAR_SKIP				4

int smCHAR::Main()
{

	int x, z, y;
	int cnt;
	DWORD compFrame;
	smPLAYBUFF* playBuff;
	int OldAction;
	int dist;
	//	int FallMode;
	POINT3D OldPos;
	//	int step1,step2;
	int	frameStep, moveStep;
	smTRANS_COMMAND	smTransCommand;
	smCHAR* lpChar;
	int EvtFrame;
	int dm1, dm2;
	int UserSlowSpeed = 128;

	int countCaravan = 0;

	if (!Pattern || PatLoading || !MotionInfo) {
		//PlayBuffCnt++;
		return FALSE;
	}

	//¸ðµ¨µ¥ÀÌÅ¸ »èÁ¦µÈ ¿À·ù
	if (lpDinaPattern->UseCount <= 0) {
		Close();
		if (smConfig.DebugMode) AddChatBuff("¸ðµ¨Á¦°Å ¿À·ù");
		return FALSE;
	}


	switch (ActionPattern) {
	case 0:
	case 1:	//Æë±Ï
		break;

	case 5:	//¸ó½ºÅÍ

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
			//¿öÇÁ µ¿ÀÛ
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


		if (MotionInfo->State && MotionInfo->State < 0x100 && moveStep)
		{
			if ((MotionInfo->State == CHRMOTION_STATE_WALK || MotionInfo->State == CHRMOTION_STATE_RUN)) //&& ((!smCharInfo.lpMonInfo->Caravana) || smCharInfo.lpMonInfo->Caravana && smCharInfo.lpMonInfo->CaravanMapNum != 33))
			{
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

				//¸ó½ºÅÍ ¹°¾à »ç¿ë
				if (smMonsterInfo.PotionCount) {
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30) {
						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_NPC_MAGICMASTER) {
							//¸ÅÁ÷¸¶½ºÅÍ´Â ¹°¾à´ë½Å Èú¸µ
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
						else {	//ÀÏ¹Ý ¸ó½ºÅÍ´Â ¹°¾à »ç¿ë
							SetMotionFromCode(CHRMOTION_STATE_EAT);
							PotionLog = 1;
						}

						smMonsterInfo.PotionCount--;
					}
				}

				if (dwNextMotionCode) {
					//´ÙÀ½ µ¿ÀÛ ¿¹¾à ½ÇÇà
					if (SetMotionFromCode(dwNextMotionCode) == FALSE) {
						SetMotionFromCode(CHRMOTION_STATE_STAND);
					}
					dwNextMotionCode = 0;
				}
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_HUNGKY) {
					//º¯½Å
					cnt = (smCharInfo.Life[0] * 100) / smCharInfo.Life[1];
					if (cnt < 30 || AttackAnger>80) {
						SetMotionFromCode(CHRMOTION_STATE_WARP);
						lpTargetPlayInfo = 0;
					}
				}

				// ¹ÚÀç¿ø - º¹³¯ ÀÌº¥Æ®
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
		if (DontMoveFlag == 2)
		{
			Angle.x = 0;
			Angle.y = 0;
			Angle.z = 0;
		}
		SavePlayBuff2();

		if (lpMasterPlayInfo && HoSkillCode && lpMasterPlayInfo->lpsmSock && smCharInfo.Life[0] > 0 && !smCharInfo.lpMonInfo->Caravana)
		{
			x = (lpMasterPlayInfo->Position.x - pX) >> FLOATNS;
			z = (lpMasterPlayInfo->Position.z - pZ) >> FLOATNS;
			dist = x * x + z * z;

			STG_AREA* lpStgArea;

			lpStgArea = FindStageField(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);

			if (lpStgArea)
			{
				int iMapID = lpMasterPlayInfo->Position.Area;

				// Mapas que os cristais não podem ir
				if (iMapID == 3 || iMapID == 21 || iMapID == 33 || iMapID == 49)
				{
					smCharInfo.Life[0] = 0;
					SetMotionFromCode(CHRMOTION_STATE_DEAD);
					return FALSE;
				}

				if (dist >= (500 * 500) || lpStgArea->lpStage != lpStage)
				{
					y = lpStgArea->lpStage->GetHeight(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);

					lpStage = lpStgArea->lpStage;

					if (y < 0)
					{
						smCharInfo.Life[0] = 0;
						SetMotionFromCode(CHRMOTION_STATE_DEAD);
					}

					if (smCharInfo.Life[0] > 0)
					{
						pX = lpMasterPlayInfo->Position.x;
						pY = lpMasterPlayInfo->Position.y;
						pZ = lpMasterPlayInfo->Position.z;

						ptNextTarget.x = ptNextTarget.z = 0;
						TargetMoveCount = 0;
						TragetTraceMode = 0;

						lpTargetChar = 0;
					}
				}
				else if (dist >= (300 * 300) || (!lpTargetChar && dist >= (150 * 150)))
				{
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

		// Caravana seguindo pelos mapas aqui
		if (lpMasterPlayInfo && smCharInfo.lpMonInfo && smCharInfo.lpMonInfo->Caravana)
		{
			x = (lpMasterPlayInfo->Position.x - pX) >> FLOATNS;
			z = (lpMasterPlayInfo->Position.z - pZ) >> FLOATNS;
			dist = x * x + z * z;

			STG_AREA* lpStgArea;

			lpStgArea = FindStageField(lpMasterPlayInfo->Position.x, lpMasterPlayInfo->Position.z);

			if (lpStgArea)
			{
				// Caravana pegando ouro sozinha aqui
				if (!smCharInfo.lpMonInfo->GoldTimeIn && dist <= (350 * 350)) // Distância do player até a caravana
				{
					auto lpStgItem = lpStgArea->FindNearGold(pX, pY, pZ); // Distância do ouro até a caravana

					if (lpStgItem)
					{
						smCharInfo.lpMonInfo->GoldTimeIn = GetTickCount();
						smCharInfo.lpMonInfo->GoldToCollet = lpStgItem;
					}
				}
				else if (smCharInfo.lpMonInfo->GoldTimeIn)
				{
					// Tempo limite para tentar pegar o gold
					if ((GetTickCount() - smCharInfo.lpMonInfo->GoldTimeIn) > 3000) // 3sgs = tempo máximo de coleta de um ouro
					{
						smCharInfo.lpMonInfo->GoldToCollet = nullptr;
						smCharInfo.lpMonInfo->GoldTimeIn = 0;
					}
					else
					{
						if (smCharInfo.lpMonInfo->GoldToCollet)
						{
							if (rsPutItem2(lpMasterPlayInfo, &smCharInfo.lpMonInfo->GoldToCollet->Item.ItemInfo))
							{
								lpStgArea->DeleteItem(smCharInfo.lpMonInfo->GoldToCollet);
								smCharInfo.lpMonInfo->GoldToCollet = nullptr;
								smCharInfo.lpMonInfo->GoldTimeIn = 0;
							}
						}
					}
				}

				// Caravana seguindo o player aqui
				if ((dist >= (500 * 500) || lpStgArea->lpStage != lpStage) && smCharInfo.lpMonInfo->CaravanState == CaravanStates::FOLLOW)
				{
					int iMapID = lpMasterPlayInfo->Position.Area;

					if (iMapID != START_FIELD_CASTLE &&
						iMapID != rsSOD_FIELD &&
						iMapID != FIELD_ARENA)
					{
						lpStage = lpStgArea->lpStage;

						pX = lpMasterPlayInfo->Position.x;
						pY = lpMasterPlayInfo->Position.y;
						pZ = lpMasterPlayInfo->Position.z;

						ptNextTarget.x = 0;
						ptNextTarget.z = 0;
						TargetMoveCount = 0;
						TragetTraceMode = 0;

						chrAttackTarget = 0;
					}
				}
				else if ((dist >= (300 * 300) || dist >= (150 * 150)) && smCharInfo.lpMonInfo->CaravanState == CaravanStates::FOLLOW)
				{
					int iMapID = lpMasterPlayInfo->Position.Area;

					if (iMapID != START_FIELD_CASTLE &&
						iMapID != rsSOD_FIELD &&
						iMapID != FIELD_ARENA)
					{
						SetTargetPosi(lpMasterPlayInfo->Position.x >> FLOATNS, lpMasterPlayInfo->Position.z >> FLOATNS);
						TargetMoveCount = 2;
						TragetTraceMode = 0;
						chrAttackTarget = 0;

						if (MotionInfo->State != CHRMOTION_STATE_DEAD && (MotionInfo->State >= 0x100 || MotionInfo->State == CHRMOTION_STATE_STAND)) {
							if (!SetMotionFromCode(CHRMOTION_STATE_RUN))
								SetMotionFromCode(CHRMOTION_STATE_WALK);
						}
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
					if (lpLinkPlayInfo && lpLinkPlayInfo->dwObjectSerial == dwLinkObjectCode && lpLinkPlayInfo->lpsmSock) {
						rsProcessQuest(lpLinkPlayInfo, this, SIN_QUEST_CODE_CHANGEJOB4, dwObjectSerial);
					}
					lpChar = rsGetLinkChar(this);		//¿¬°áÄ³¸¯ »èÁ¦
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
						rsProcessQuest(lpLinkPlayInfo, this, HAQUEST_CODE_FURYOFPHANTOM, dwObjectSerial);
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

						//¸ó½ºÅÍ »ý¸í·Â Àç»ýÄ¡ ±¸ÇÏ´Â ÇÔ¼ö
						smCharInfo.Life[0] += rsGetMonsterLifeRegen(this);

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
									//°¡µð¾ð ¼¼ÀÎÆ® ( ¹üÀ§ °ø°Ý )
									rsSkillDamage_AroundMonster(this);
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
			//ÇØ´ç ¸ó½ºÅÍ Ãâ½Å ±ê¹ß ÀÇ È°¼ºÈ­ ¸ó½ºÅÍ ¼ö
			if (rsGetMonCountFlag(this) > 3) cnt = FALSE;
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
											lpChar = rsFindAroundMonster(this, 200);
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
										//ÁÖº¯ ¸ó½ºÅÍ¸¦ Ã£¾Æ ½ºÅ³ ( ¶óÅõ ÁÖº¯ ¸ó½ºÅÍ ½ºÅÏÁ¦°Å )
										rsSkillMonster_AroundMonster(this, 200);
										break;
									case snCHAR_SOUND_NPC_SKILLMASTER:
										cnt = rand() % 100;
										if (cnt < 3 && !rsGetLinkChar(this) && lpLinkPlayInfo) {
											if ((cnt & 1) == 0) {
												lpChar = OpenMonsterFromSkill(SKILL_PLAY_METAL_GOLEM, pX, pY, pZ, lpLinkPlayInfo, this);
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
												lpChar = OpenMonsterFromSkill(SKILL_PLAY_RECALL_WOLVERIN, pX, pY, pZ, lpLinkPlayInfo, this);
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
										if (cnt < 3 && !rsGetLinkChar(this) && lpLinkPlayInfo) {
											lpChar = OpenMonsterFromSkill(SKILL_PLAY_FIRE_ELEMENTAL, pX, pY, pZ, lpLinkPlayInfo, this);
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
					if (MotionInfo->State == CHRMOTION_STATE_ATTACK &&
						rsProcessAttack_SkillHolyReflection(lpTargetPlayInfo, this) == TRUE) {

						if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_VAMPRICMACHINE) {		//¹ìÇÇ¸¯ ¸Ó½Å ¹ú¹ß»ç
							cnt = 0;
							//Æ¯Á¤À§Ä¡¿¡ º¸Á¶¸ó½ºÅÍ °­Á¦ ÃâÇö ½ÃÅ´
							rsOpenSubMonster(this, snCHAR_SOUND_VAMPRICBEE, pX, pY, pZ, 4);
						}
						else {
							if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BABEL && (rand() % 100) < 60) {	//60%È®·ü
								//¹Ùº§ ÀÏ¹Ý°ø°Ý½Ã ½ºÅ³µµ °°ÀÌ Àû¿ë (¹üÀ§ÁöÁ¤)
								rsSendRangeDamage(this, lpTargetPlayInfo, 100, fONE / 2);

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

								rsSendCommandUser2(lpTargetPlayInfo, (smTRANS_COMMAND*)&TransChatMessage);
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
						smCharInfo.Life[0] += rsGetMonsterLifeRegen(this);

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
								rsSendTransSkillAttack(this, lpTargetPlayInfo);
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

					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_KELVEZU && (sObject_VirtualLife[0] & 0x1) == 0) {
						//ÄÌº£Áê (Ä«¿À½º È£Ãâ)
						rsOpenSubMonster(this, snCHAR_SOUND_CHAOSCARA, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
						rsOpenSubMonster(this, snCHAR_SOUND_CHAOSCARA, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
					}
					else if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_MOKOVA && (sObject_VirtualLife[0] & 0x1) == 0) {
						//¸ðÄÚ¹Ù (´ÙÅ©°¡µå È£Ãâ)
						rsOpenSubMonster(this, snCHAR_SOUND_DARKGUARD, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
						rsOpenSubMonster(this, snCHAR_SOUND_DARKGUARD, pX + ((rand() % (200 * fONE)) - 100 * fONE), 0, pZ + ((rand() % (200 * fONE)) - 100 * fONE), 8);
					}

				}
			}

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
			TransDelayCheck();

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
						//}
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
						break;
					case CHRMOTION_STATE_WARP:
						break;

					case CHRMOTION_STATE_DEAD:			//Ä³¸¯ Á×À½µ¿ÀÛ ½ÃÀÛ
						FrameCounter = 0;
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
					}
				}

				if (MotionInfo->EventFrame[1]) {
					compFrame = frame - (MotionInfo->StartFrame * 160);
					if (MotionInfo->EventFrame[1] > (compFrame - 80) && MotionInfo->EventFrame[1] <= compFrame) {
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

	case 101:		//È®´ë Ä³¸¯ÅÍ

		frame += 80;
		compFrame = frame / 160;

		if (compFrame >= MotionInfo->EndFrame)
		{
			SetMotionFromCode(CHRMOTION_STATE_STAND);
		}

		if (TalkFrame && lpTalkMotionInfo)
		{
			if (TalkFrame >= lpTalkMotionInfo->EndFrame * 160)
				TalkFrame = 0;
			else
				TalkFrame += 80;
		}
		//######################################################################################
		break;

	}

	if (ShootingFlag) {
		//¹ß»çÇü ¹«±â ¸ÞÀÎÇÔ¼ö
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

#define VIEW_HIGH_DIST	36864			//(64*3)		
#define VIEW_MID_DIST	102400			//(64*5)		


int bb_r, bb_g, bb_b, bb_a;

int SetRenderBlinkColor(smCHARTOOL* smCharTool) { return TRUE; }

int RestoreRenderBlinkColor() { return TRUE; }

//°¡»óÀ¸·Î ±×¸°´Ù ( ½ÇÁ¦·Î´Â ±×¸®Áö ¾Ê°í °ª¸¸ ¼¼ÆÃ )
int smCHAR::VirtualDraw() { return TRUE; }

int smCHAR::Draw() { return TRUE; }

//±×¸²ÀÚ ±×¸®±â
int smCHAR::DrawShadow() { return TRUE; }


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
int smCHAR::DrawIceFoot(void) { return TRUE; }



#define STATE_BAR_WIDTH		100
#define STATE_BAR_HEIGHT	16
#define STATE_BAR_SIZE		92


// Ã¼·Â¹Ù ±×¸®±â
int smCHAR::DrawStateBar(int x, int y) { return TRUE; }


// Ã¼·Â¹Ù ±×¸®±â
int smCHAR::DrawStateBar2(int x, int y) { return TRUE; }


// Â÷Â¡ °ÔÀÌÁö ±×¸®±â
int smCHAR::DrawChargingBar(int x, int y, int Charging, int ChargingMax) { return TRUE; }

//¿òÁ÷ÀÓ¿¡ ÀÜ»ó ³²±â±â
int smCHAR::DrawMotionBlur() { return TRUE; }



//¹«±â ¿òÁ÷ÀÓ¿¡ ÀÜ»ó ³²±â±â
int smCHAR::DrawMotionBlurTool(smCHARTOOL* ChrTool) { return TRUE; }





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

		if (cnt < lpTransAttackData->AttackSize && sinGetLife()>0 && (len < (330 * 330) || act) && DisplayAlpha == 0) {
			/*
							lpChar = 0;

							//°ø°ÝÇÑ »ó´ë¹æÀ» Ã£¾Æ ¹æÇâÀ» ¹Ù²Û´Ù
							for( cnt=0;cnt<OTHER_PLAYER_MAX;cnt++ ) {
								if ( chrOtherPlayer[cnt].Flag && chrOtherPlayer[cnt].dwObjectSerial==lpTransAttackData->dwDestObjectSerial ) {
									lpChar = &chrOtherPlayer[cnt];
									break;
								}
							}
			*/

			lpChar = this;

			block = FALSE;

			if (lpChar) {

				if (lpCurPlayer->smCharInfo.JOB_CODE == JOBCODE_ARCHER) {			//¾ÆÃ³
					cnt = cSkill.GetSkillPoint(SKILL_EVASION_MASTERY);			//Ãß°¡ È¸ÇÇ ¸¶½ºÅÍ¸®
					if (cnt > 0 && cnt <= 10) {
						cnt = Evasion_Mastery_AddPercent[cnt - 1];
						if (cnt <= 26) { // Àåº° - ½ºÅ³¹ë·±½º Á¶Á¤(10.08.10)
							if ((rand() % 100) <= cnt) {
								//Ãß°¡È¸ÇÇ ¼º°ø
								block = 100;
								Record_BlockCount++;
							}
						}
					}
				}



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
							cnt = cSkill.GetSkillPoint(SKILL_DIVINE_INHALATION);		//µð¹ÙÀÎ ½¯µå ÀÎ°è
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
						//¿¡³ÊÁö ½¯µå Àû¿ë È®ÀÎ
						temp = lpTransAttackData->AttackState >> 16;
						if (temp > 0) {
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
						}
						break;
					case SKILL_PLAY_GROUND_PIKE:
						lpCurPlayer->PlaySlowCount = GetResistancePower(Param2 * 70, sITEMINFO_ICE);
						lpCurPlayer->PlaySlowSpeed = 180;
						lpCurPlayer->AttackIce = 1;
						break;
					case SKILL_PLAY_GLACIAL_SPIKE:
					case SKILL_PLAY_ICE_CRYSTAL:		//°ø¼º ¾óÀ½Å¾
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
							if (chrAttackTarget == lpCurPlayer) {
								lpCurPlayer->PlayPoison[0] = 1;
								lpCurPlayer->PlayPoison[1] = len;
							}
						}
						break;

					case snCHAR_SOUND_KINGBAT: // ¹ÚÀç¿ø - ÀúÁÖ¹ÞÀº ½ÅÀü 3Ãþ(½Å±ÔÇÊµå ¸ó½ºÅÍ) - Å·¹î
						if (chrAttackTarget == lpCurPlayer) {
							lpCurPlayer->PlayPoison[0] = 1;
							lpCurPlayer->PlayPoison[1] = len;
						}
						break;

					case snCHAR_SOUND_DEVIL_BIRD:
						if (chrAttackTarget) {
						}
						break;

						/////////////////////////

					case snCHAR_SOUND_BLOODYKNIGHT:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
							lpCurPlayer->PlayStunCount = 70 * 3;
						}
						break;
					case snCHAR_SOUND_HELLHOUND:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
						}
						break;
					case snCHAR_SOUND_CHIMERA:
						if (chrAttackTarget && (lpTransAttackData->AttackState & 0xFF) == 0x80) {					//½ºÅ³°ø°Ý
							lpCurPlayer->PlayStunCount = 70 * 3;
						}
						break;


					}


					//////////////////////////////////////////////

					//Èí¼öÀ² °è»ê
					if (PkMode_CharState == smCHAR_STATE_USER)
						cnt = (cnt - ((lpCurPlayer->smCharInfo.Absorption * PKMODE_ABSORB) / 100));
					else
						cnt = (cnt - lpCurPlayer->smCharInfo.Absorption);
					/*
											if ( (lpCurPlayer->dwDispAppSkill&SKILL_APPMASK_HOLY_BODY) ) {
												if ( smCharInfo.Brood==smCHAR_MONSTER_UNDEAD ) {
													//È¦¸® ¹Ùµð ½ºÅ³ Àû¿ë ( ¾ðµ¥µå¸¸ Àû¿ë )
													cnt -= HolyBody_Absorb[DispApp_SkillPoint-1];
													HolyBody_Undead = TRUE;
												}
											}
					*/
					//if ( smCharInfo.State==smCHAR_STATE_USER )	cnt = (cnt*80)/100;		//PK¸ðµå »ý¸í°¨¼Ò ¹ÝÀ¸·Î ÁÙÀÓ

					if (cnt <= 0) cnt = 1;

					if (abs(lpChar->smCharInfo.Level - lpCurPlayer->smCharInfo.Level) <= AGING_SUB_LEVEL) {
						//·¹º§ 7Â÷ÀÌ ÀÌÇÏ´Â ¿¡ÀÌÂ¡ Àû¿ë
					//	sinCheckAgingLevel( SIN_AGING_DEFENSE , false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMOR, false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ORB, false);
						sinCheckAgingLevel(SIN_AGING_DEFENSE_BOOTS, false);   // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(ºÎÃ÷)
						sinCheckAgingLevel(SIN_AGING_DEFENSE_GLOVES, false);  // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(Àå°©)
						sinCheckAgingLevel(SIN_AGING_DEFENSE_ARMLETS, false); // ¹ÚÀç¿ø - ¿¡ÀÌÂ¡ ¾ÆÀÌÅÛ Ãß°¡(¾Ï¸´)
					}

					CheckCharForm();

					sinSetLife(sinGetLife() - cnt);
					//¸íÁß ( °ø°Ý ¼º°ø )
					//lpCurPlayer->smCharInfo.Life[0] -= cnt;

					if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_SUCCUBUS) {
						if (lpTransAttackData->sRating[0] > 75) {
							if (lpTransAttackData->sRating[0] < 80)	//¼­Å¥¹ö½º
								sinSetStamina(sinGetStamina() - ((lpCurPlayer->smCharInfo.Stamina[1] * 6) / 100));
							else 	//ÀÎÅ¥¹ö½º
								sinSetMana(sinGetMana() - ((lpCurPlayer->smCharInfo.Mana[1] * 6) / 100));
						}
					}


					if (((lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_PHYSICAL_ABSORB) && (lpCurPlayer->dwDispAppSkill & SKILL_APPMASK_HOLY_BODY) == 0) || HolyBody_Undead) {
						//ÇÇÁöÅ¬¾÷¼Ù Àû¿ëÀÌÆåÆ®
					}

					if (lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_DAMAGE && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_EAT && lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_ATTACK &&
						lpCurPlayer->MotionInfo->State != CHRMOTION_STATE_SKILL && cnt > 1) {

						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DAMAGE);
					}

					//ÀÓ½Ã Ã³¸®
					if (sinGetLife() <= 0) {

						if (sinQuest_ChangeJob4.CODE == SIN_QUEST_CODE_CHANGEJOB4 &&
							lpCurPlayer->OnStageField >= 0 && StageField[lpCurPlayer->OnStageField]->FieldCode == QUEST_ARENA_FIELD) {
							//Äù½ºÆ® ´ëÀüÀå
							OpenEachPlayer(this);
							sinBattleNpcResult(0);		//3Â÷ Àü¾÷ ¸¶½ºÅÍ ÇÑÅ× Á³´Ù
							sinSetLife(0);
							lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
							ResetAttackTrans();
							return TRUE;
						}
						lpCurPlayer->SetMotionFromCode(CHRMOTION_STATE_DEAD);
						sinSetLife(0);

						SendCommand_AreaServer(smTRANSCODE_DEAD_USER, dwObjectSerial, 0, 0, 0);
						SendCommand_DataServer(smTRANSCODE_DEAD_USER, dwObjectSerial, 1, 0, 0);
						WeaponEffect = 0;
						dwWeaponEffectTime = 0;

#ifdef LOG_THE_PK
						if (smCharInfo.State != smCHAR_STATE_USER && PkMode_CharState != smCHAR_STATE_USER)
#else
						if (smCharInfo.State != smCHAR_STATE_USER)
#endif
						{//ÇØ¿Ü
							DeadPlayerExp(lpCurPlayer);				//Á×¾î¼­ °æÇèÄ¡ ±ïÀÓ (PK´çÇßÀ»¶§´Â Á¦¿Ü)
						}

						sinCheckQuest85Die();		//Äù½ºÆ® ¼öÇàÁß Á×¾úÀ» °æ¿ì

						//°ÔÀÓÀ» ÀúÀå
						SaveGameData();

#ifdef LOG_THE_PK
						if (smCharInfo.State == smCHAR_STATE_USER || PkMode_CharState == smCHAR_STATE_USER)
#else
						if (smCharInfo.State == smCHAR_STATE_USER)
#endif
						{//ÇØ¿Ü
							//PK´çÇØ¼­ Á×À½
							PlayerKilling(dwObjectSerial);
						}
						ResetAttackTrans();	//lpTransAttackData->code = 0;
						return TRUE;
					}
					ReformCharForm();					//Ä³¸¯ÅÍ Á¤º¸ ÀÎÁõ

				}
				else {
					//È¸ÇÇ(¹æ¾î) ¼º°ø
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

//Àü¼Û ¹ÞÀº µ¥ÀÌÅ¸·Î PLAYBUFF¿¡ ¿¹Ãø µ¥ÀÌÅ¸¸¦ ¸¸µé¾î ³Ö´Â´Ù
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

	if (StartBuff->frame < CHAR_FRAME_MASK) {
		frameState = smMotionInfo->MotionInfo[StartBuff->angle[3]].State;
		if (frameState == CHRMOTION_STATE_WALK || frameState == CHRMOTION_STATE_RUN) {
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


//Åë½Å ¿À·ù Ã¼Å©ÇÏ¿© º¸Á¤
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

//°Å¸®º° Àü¼Û ½Ã°£Â÷¸¦ Á¶Àý ÇÑ´Ù
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
		TransDelayMax = PLAYCOUNT_PER_SECOND * 4;		//4ÃÊ
		return TRUE;
	}
	if (dist < DIST_TRANSLEVEL_MID) {
		TransMinCnt = PLAYCOUNT_PER_SECOND;
		TransLevel = 1;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 6;		//8ÃÊ
		return TRUE;
	}
	if (dist < DIST_TRANSLEVEL_LOW) {
		TransMinCnt = PLAYCOUNT_PER_SECOND * 5;
		TransLevel = 2;
		TransDelayMax = PLAYCOUNT_PER_SECOND * 10;		//5ÃÊ
		return TRUE;
	}

	TransMinCnt = PLAYCOUNT_PER_SECOND * 15;		//15ÃÊ
	TransLevel = 3;
	TransDelayMax = PLAYCOUNT_PER_SECOND * 25;		//25ÃÊ

	return TRUE;
}

//»ó´ë ÇÃ·¹ÀÌ¾îÀÇ µ¥ÀÌÅ¸ ÃÊ±â ¼³Á¤
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

		break;

	case  snCHAR_SOUND_CHAINGOLEM:			//Ã¼ÀÎ°ñ·½
		break;

	case snCHAR_SOUND_RUNICGUARDIAN:		//·ç´Ð °¡µð¾È
		break;

	case snCHAR_SOUND_BABEL:				//¹Ùº§ (·ç´Ð°¡µð¾È º¸½º)
		break;

	case snCHAR_SOUND_CHAOSCARA:
		break;

	case snCHAR_SOUND_DEATHKNIGHT:			//¹ß·»Åä
		break;

	case snCHAR_SOUND_S_F_ELEMENTAL:		//¼ÒÈ¯¸÷ È­ÀÌ¾î¿¤¸®¸àÅ»
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

	if (smCharInfo.ClassClan) {
		//Å¬·£ Á¤º¸
		ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
		dwClanInfoTime = dwPlayTime;

		if (GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)	//Ä³½½ ¸¶½ºÅÍ Å¬·£
			Clan_CastleMasterFlag = TRUE;
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

	if (smCharInfo.ClassClan) {
		//Å¬·£ Á¤º¸
		ClanInfoNum = ReadClanInfo(smCharInfo.ClassClan);
		dwClanInfoTime = dwPlayTime;

		if (GetClanCode(smCharInfo.ClassClan) == rsBlessCastle.dwMasterClan)	//Ä³½½ ¸¶½ºÅÍ Å¬·£
			Clan_CastleMasterFlag = TRUE;
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

		smCharInfo.Life[0] = lpTransPlayData->Hp[0];		//Ã¼·Â
		smCharInfo.Life[1] = lpTransPlayData->Hp[1];	//Ã¼·ÂÃÖ´ëÄ¡

		if (LevelLog) {
			if (smCharInfo.State == smCHAR_STATE_USER && lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_LEVEL] > LevelLog) {
				//»ó´ë¹æ ·¹º§¾÷
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

					// ¿À¸¥ÂÊ ÀåÂø ¾ÆÀÌÅÛ
					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT] != wStickItems[smPLAYBUFF_ITEM_RIGHT]) {
						if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT] >= 0) {
							SetTool(sItem[lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_RIGHT]].CODE, hvPOSI_RHAND);
							HvRightHand.ColorBlink = 0;
							HvRightHand.DispEffect = 0;
						}
						else
							SetTool(0, hvPOSI_RHAND);
					}

					// ¸ö ÀåÂø ¾ÆÀÌÅÛ( °©¿Ê µî )
					if (lpTransPlayBuff->PlayBuff.wStickItems[smPLAYBUFF_ITEM_BODY] != wStickItems[smPLAYBUFF_ITEM_BODY]) {
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

					//º¯½Å ¼Ò¸®
					if (MotionInfo) {
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						MotionInfo->State = cnt;
					}
				}
				// pluto ¾ðµ¥µå Å·È£ÇÇ º¯½Å
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_DEADHOPT && !dwUpdateCharInfoTime)
				{

					if (MotionInfo)
					{
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						MotionInfo->State = cnt;
					}
				}

				// ¹ÚÀç¿ø - º¹³¯ ÀÌº¥Æ®
				if (smCharInfo.dwCharSoundCode == snCHAR_SOUND_BEBECHICK && !dwUpdateCharInfoTime)
				{

					if (MotionInfo)
					{
						cnt = MotionInfo->State;
						MotionInfo->State = CHRMOTION_STATE_WARP;
						MotionInfo->State = cnt;
					}
				}


				dwUpdateCharInfoTime = dwPlayTime;
			}
		}


		////////////////// °ø¼ºÀü ÇÊµå È®ÀÎ ////////////////////

		if (lpCurPlayer->OnStageField >= 0 &&
			(
				(StageField[lpCurPlayer->OnStageField]->State == FIELD_STATE_CASTLE && CastleBattleZone_LineZ > lpCurPlayer->pZ) ||
				(StageField[lpCurPlayer->OnStageField]->FieldCode == PK_FieldState.FieldCode && PK_FieldState.dwPlay_FieldTime > dwPlayTime && !PartyFlag)
				)) {

			//ÁÖÀÎ°ø °ø¼º ¸ðµå
			if (PkMode_CharState == 0 && smCharInfo.State == smCHAR_STATE_USER &&
				(smCharInfo.ClassClan == 0 || smCharInfo.ClassClan != lpCurPlayer->smCharInfo.ClassClan))
			{

				if (StageField[lpCurPlayer->OnStageField]->FieldCode != PK_FieldState.FieldCode || !PartyFlag) {
					PkMode_CharState = smCharInfo.State;
					smCharInfo.State = smCHAR_STATE_ENEMY;
				}
			}
		}
		else {
			if (PkMode_CharState) {
				//°ø¼º¸ðµå ÇØÁ¦
				smCharInfo.State = PkMode_CharState;
				PkMode_CharState = 0;
			}
		}

		int ChkIceFlag = 0;
		int ChkPoisonFlag = 0;
		DWORD	TradeMsgCode = 0;
		//DWORD	dwMsgCode;

		PlayVanish = 0;
		TradeMsgCode = dwTradeMsgCode;
		dwTradeMsgCode = 0;
		PlayCurseQuest = 0;

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
						break;

				case smTRANSCODE_EFFECT_CURSE:
					//½áÅ¥¹ö½º ÀúÁÖ ( HolyPower Àû¿ëÁß )
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

				case smTRANSCODE_EFFECT_VANISH:
					PlayVanish = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					PlayVague = ((smEFFECT_ITEM*)lpBuff)->ColorBlink;
					break;

				case smTRANSCODE_EFFECT_KEEPSKILL:
					//½ºÅ³ ÀÌÆåÆ® ÇÃ·¢ Á¤º¸
					switch (((smEFFECT_ITEM*)lpBuff)->dwItemCode) {
					case SKILL_PLAY_FORCE_OF_NATURE:
						break;
					case SKILL_PLAY_HALL_OF_VALHALLA:
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

					break;

				case smTRANSCODE_EFFECT_PCBANGPET:		//PC¹æ Æê

					if (((smEFFECT_ITEM*)lpBuff)->ColorBlink)
						lpChar = FindChrPlayer(((smEFFECT_ITEM*)lpBuff)->ColorBlink);
					else
						lpChar = 0;

					break;

				case smTRANSCODE_EFFECT_CLANMGR:		//¼øÀ§±Ç Å¬·£ Ç¥½Ã
					dwClanManageBit = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					break;

				case smTRANSCODE_EFFECT_FORCEORB:		//Æ÷½º ¿Àºê ½ÃÀüÁß
					dwForceOrbCode = ((smEFFECT_ITEM*)lpBuff)->dwItemCode;
					dwForceOrbTime = ((smEFFECT_ITEM*)lpBuff)->ColorBlink;
					dwForceOrbTime += dwPlayTime;

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

		smCharInfo.Life[0] = lpTransPlayData->Hp[0];		//Ã¼·Â

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

	//°ÔÀÓ µ¥ÀÌÅ¸ Àü¼ÛÀÌ Ã³À½ µÇ¾úÀ»¶§ Ä³¸¯ÅÍ È­¸é¿¡ Ç¥½Ã
	//if ( Pattern ) DisplayFlag = TRUE;		

	return TRUE;
}

//µ¥ÀÌÅ¸¸¦ º¸³¿
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

	//if ( AutoPlayer ) return NULL;

	SendTime = PlayCounter - TransLastSendCnt;

	if (TransSendWait == 0 && SendTime >= TransMinCnt) {

		//¸ñÇ¥¸¦ °Ü³ÉÇÑ µ¿ÀÛÀÏ °æ¿ì~~ ¸ñÇ¥Á¡ Ç¥½ÃÇÑ´Ù
		if (lpCharMsTrace && lpCharMsTrace == this) {
			PlayData2.dwTarget = TRUE;
			PlayData3.dwTarget = TRUE;
		}
		else {
			PlayData2.dwTarget = FALSE;
			PlayData3.dwTarget = FALSE;
		}

		if (TransLevel > 1) {
			//¹öÆÛ¸µ ¾ø´Â µ¥ÀÌÅ¸ Àü¼Û ( Åë½ÅÀÇ Áß¿äµµ°¡ ³·À½ )
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

			if (TransSock->Send((char*)&PlayData, sizeof(smPLAYDATA))) {
				TransSendWait = 1;
				TransLastSendCnt = PlayCounter;
				TransLastSendTime = SendTime;
				SetDistSendCnt(player);				//Åë½Å ½Ã°£Â÷ Á¶Àý
			}
		}
		else {
			//¹öÆÛ¸µ µ¥ÀÌÅ¸ Àü¼Û ( °íÇ°ÁúÀÇ µ¥ÀÌÅ¸ Àü¼Û )
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
					SetDistSendCnt(player);				//Åë½Å ½Ã°£Â÷ Á¶Àý
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
					SetDistSendCnt(player);				//Åë½Å ½Ã°£Â÷ Á¶Àý
				}
			}
		}
	}

	return TRUE;
}
//Åë½Å¿ë ÇÃ·¹ÀÌ µ¥ÀÌÅ¸¸¦ ÁöÁ¤ÇÑ ¹öÆÛ¿¡ ÀÛ¼º 
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

	//	int PlayerBuffCnt;

	int	buffCnt;
	int cnt;
	int LastCnt;
	int	SameAction;

	lpTransPlayData = (smTRANS_PLAYDATA*)lpTargetBuff;
	lpTransPlayBuff = (smTRANS_PLAYBUFF*)(lpTargetBuff + sizeof(smTRANS_PLAYDATA));

	lpTransPlayData->code = smTRANSCODE_TRANSPLAYDATA;
	lpTransPlayData->dwObjectSerial = dwObjectSerial;
	lpTransPlayData->dwTarget = 0;
	lpTransPlayData->StartPosi = PlayBuffCnt - SendTime;
	if (this == lpCurPlayer) {
		if (MotionInfo && MotionInfo->State == CHRMOTION_STATE_DEAD)
			lpTransPlayData->Hp[0] = 0;
		else
			lpTransPlayData->Hp[0] = sinGetLife();
	}
	else {
		lpTransPlayData->Hp[0] = smCharInfo.Life[0];
	}
	lpTransPlayData->Hp[1] = smCharInfo.Life[1];

	if (UseObject_VirtualLife) {		//°¡»ó »ý¸í·Â »ç¿ë (È®Àå)
		lpTransPlayData->Hp[0] = sObject_DisplayLife[0];
		lpTransPlayData->Hp[1] = sObject_DisplayLife[1];
	}



	if (smCharInfo.State == smCHAR_STATE_ENEMY)
		lpTransPlayData->dwAutoCharCode = smMonsterInfo.dwAutoCharCode;
	else
		lpTransPlayData->dwAutoCharCode = 0;

	smCharInfo.bUpdateInfo[1] = sinSkill.SkillPoint;				//½ºÅ³ Æ÷ÀÎÆ® ÀúÀå

	lpTransPlayData->bUpdateInfo[0] = smCharInfo.bUpdateInfo[0];
	lpTransPlayData->bUpdateInfo[1] = smCharInfo.bUpdateInfo[1];
	lpTransPlayData->bUpdateInfo[2] = 0;
	lpTransPlayData->bUpdateInfo[3] = 0;
	lpTransPlayData->bPartyLeader = smCharInfo.bPartyLeader;

	int	MainArea, SubArea;

	if (lpCurPlayer && lpCurPlayer->OnStageField >= 0) {
		MainArea = (lpCurPlayer->OnStageField) & 1;
		SubArea = (lpCurPlayer->OnStageField + 1) & 1;
	}
	else {
		MainArea = 0;
		SubArea = 0;
	}

	//·ÎµùµÈ ÇØ´ç Áö¿ª ÄÚµå »ðÀÔ
	if (StageField[0])
		lpTransPlayData->wArea[0] = StageField[MainArea]->FieldCode;
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

	/*
		if ( smCharInfo.State!=smCHAR_STATE_USER ) {
			//¸ó½ºÅÍ
			if ( PlaySlowCount>0 ) lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL] |= SKILL_APPMASK_ICE;
		}
	*/
	//lpCurPlayer->dwDispAppSkill = 0;

//	BYTE	bEventInfo[4];		//¸ð¼Ç ÀÌº¥Æ® Á¤º¸ ( 0-Level 1-Potion»ç¿ë 2-½ºÅ³»ç¿ë 3-¿¹ºñ )


	StartPosi = (lpTransPlayData->StartPosi) & PLAYBUFF_MASK;

	buffCnt = 0;

	//½ÃÀÛ Á¤º¸
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
			//if ( (cnt-LastCnt)>4 ) {
				//¹Ù²ï ¸ð¼Ç ÀÌº¥Æ® ±â·Ï
			mAction = PlayBuff[BuffPosi_Old].angle[3];
			memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi_Old], sizeof(smPLAYBUFF));
			lpTransPlayBuff->BuffCount = (cnt * pBuffStep) - 1;
			lpTransPlayBuff++;
			buffCnt++;
			//}

			//¹Ù²ï ¸ð¼Ç ÀÌº¥Æ® ±â·Ï
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

	//¸¶Áö¸· ±â·Ï
	mAction = PlayBuff[BuffPosi].angle[3];
	memcpy(&lpTransPlayBuff->PlayBuff, &PlayBuff[BuffPosi], sizeof(smPLAYBUFF));
	lpTransPlayBuff->BuffCount = (SendTime * pBuffStep) - 1;
	lpTransPlayBuff++;
	buffCnt++;

	lpTransPlayData->StartPosi = (lpTransPlayData->StartPosi * pBuffStep) & PLAYBUFF_MASK;

	if (SameAction == TRUE && smCharInfo.State == smCHAR_STATE_USER && MotionInfo->State == CHRMOTION_STATE_STAND)
		buffCnt = 1;			//°¡¸¸È÷ ¼­ÀÖ´Â µ¿ÀÛÀÇ ¿¬¼Ó

	lpTransPlayData->PlayBuffCnt = buffCnt;
	lpTransPlayData->size = sizeof(smTRANS_PLAYDATA) + (sizeof(smTRANS_PLAYBUFF) * buffCnt);

	/////////////////// Ãß°¡ Á¤º¸ µÚ¿¡ ±â·Ï /////////////////////


	MotionRecordCount++;


	//	if ( (MotionRecordCount&0x7)==0 ) {

			//Ãß°¡ ÀÌÆåÆ® Á¤º¸°¡ ÀÖÀ¸¸é ÁÖ±âÀûÀ¸·Î Àü¼Û
	lpEffectItem = (smEFFECT_ITEM*)(lpTargetBuff + lpTransPlayData->size);

	if (HvLeftHand.PatTool && HvLeftHand.dwItemCode && (HvLeftHand.ColorBlink || HvLeftHand.DispEffect)) {

		lpEffectItem->code = smTRANSCODE_EFFECT_ITEM;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = HvLeftHand.dwItemCode;
		lpEffectItem->ColorBlink = HvLeftHand.ColorBlink;
		memcpy(lpEffectItem->sColors, HvLeftHand.sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = HvLeftHand.DispEffect;
		lpEffectItem->BlinkScale = HvLeftHand.BlinkScale;


		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (HvRightHand.PatTool && HvRightHand.dwItemCode && (HvRightHand.ColorBlink || HvRightHand.DispEffect)) {

		lpEffectItem->code = smTRANSCODE_EFFECT_ITEM;
		lpEffectItem->size = sizeof(smEFFECT_ITEM);
		lpEffectItem->dwItemCode = HvRightHand.dwItemCode;
		lpEffectItem->ColorBlink = HvRightHand.ColorBlink;
		memcpy(lpEffectItem->sColors, HvRightHand.sColors, sizeof(short) * 4);
		lpEffectItem->DispEffect = HvRightHand.DispEffect;
		lpEffectItem->BlinkScale = HvRightHand.BlinkScale;

		lpTransPlayData->size += lpEffectItem->size;
		lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_EXT] ++;
		lpEffectItem++;
	}

	if (dwWeaponEffectTime) {
		//¼Ó¼º ÀÌÆåÆ®
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
	/*
		if ( smCharInfo.State!=smCHAR_STATE_USER ) {
			//¸ó½ºÅÍ
			if ( PlaySlowCount>0 ) lpTransPlayData->bEventInfo[TRANS_PLAYDATA_EVENT_SKIL] |= SKILL_APPMASK_ICE;
		}
	*/
	if (PlaySlowCount > 0 && Life > 0) {
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
	if (dwClanManageBit) {
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

#ifndef _W_SERVER
	if (OnStageField >= 0 && StageField[OnStageField]->State == FIELD_STATE_CASTLE) {
		//ºí·¡½º Ä³½½ °³ÀÎ Á¤º¸
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

#endif

	srTransBuffSize = lpTransPlayData->size;

	return TRUE;
	}

//¼ö½ÅµÈ µ¥ÀÌÅ¸ÀÇ ÀåÂø ¾ÆÀÌÅÛ¿¡ ÀÌÆåÆ® Àû¿ë 
int smCHAR::SetTransEffectItems(smEFFECT_ITEM* lpEffectItem) { return TRUE; }

//¼ö½ÅµÈ µ¥ÀÌÅ¸ÀÇ ÀåÂø ¹«±â¿¡ ¼Ó¼º ÀÌÆåÆ® Àû¿ë 
int smCHAR::SetTransEffectWeapon(smEFFECT_ITEM* lpEffectItem) { return TRUE; }

//ÀÚ½ÅÀÇ µ¥ÀÌÅ¸¸¦ ¼ÒÄÏÀ» ÅëÇÏ¿© ´Ù¸¥ °÷À¸·Î Àü¼Û ( ¼­¹ö¿¡¼­ Å¬¶óÀÌ¾ðÆ®·Î Àü¼Û¿ë )
int smCHAR::SendPlayDataSock(smWINSOCK* lpsmsock, char* lpTransBuff, int ex, int ey, int ez)
{
	int SendTime;
	smPLAYDATA PlayData;
	int dist;
	int x, y, z;


	//	TransLastSendCnt++;

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

	if (TransLevel > 1) {
		//¹öÆÛ¸µ ¾ø´Â µ¥ÀÌÅ¸ Àü¼Û ( Åë½ÅÀÇ Áß¿äµµ°¡ ³·À½ )
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

		if (lpsmsock->Send((char*)&PlayData, sizeof(smPLAYDATA), TRUE)) {
			//SetDistSendCnt( player );				//Åë½Å ½Ã°£Â÷ Á¶Àý
		}
	}
	else {
		/*
		//¹öÆÛ¸µ µ¥ÀÌÅ¸ Àü¼Û ( °íÇ°ÁúÀÇ µ¥ÀÌÅ¸ Àü¼Û )
		SendMode = 0;
		PlayerBuffCnt = PlayBuffCnt&PLAYBUFF_MASK;
		StartPosi = ( PlayerBuffCnt - SendTime) & PLAYBUFF_MASK;

		mAction = PlayBuff[ PlayerBuffCnt ].angle[3];


		for( cnt=SendTime; cnt>=0; cnt-- ) {
			BuffPosi = (StartPosi + cnt ) & PLAYBUFF_MASK;
			if ( PlayBuff[BuffPosi].angle[3]!=mAction ) {
				SendMode = 1;
				memcpy( &PlayData3.PlayBuff[2] , &PlayBuff[BuffPosi] , sizeof( smPLAYBUFF ) );
				PlayData3.BuffCount[2] = cnt;
				break;
			}
		}

		if ( !SendMode ) {
			memcpy( &PlayData2.PlayBuff[0] , &PlayBuff[ StartPosi ] , sizeof( smPLAYBUFF ) );
			memcpy( &PlayData2.PlayBuff[1] , &PlayBuff[ PlayerBuffCnt ] , sizeof( smPLAYBUFF ) );
			PlayData2.code = smTRANSCODE_PLAYDATA2;
			PlayData2.size = sizeof( smPLAYDATA2 );
			PlayData2.BuffCount[0] = 0;
			PlayData2.BuffCount[1] = SendTime;
			PlayData2.dwObjectSerial = dwObjectSerial;

			if ( lpsmsock->Send( (char *)&PlayData2 , sizeof( smPLAYDATA2 ) ) ) {
				//TransSendWait = 1;
				//TransLastSendCnt = PlayCounter;
				//TransLastSendTime = SendTime;
				//SetDistSendCnt( player );				//Åë½Å ½Ã°£Â÷ Á¶Àý
			}
		}
		else {
			memcpy( &PlayData3.PlayBuff[0] , &PlayBuff[ StartPosi ] , sizeof( smPLAYBUFF ) );
			memcpy( &PlayData3.PlayBuff[1] , &PlayBuff[ PlayerBuffCnt ] , sizeof( smPLAYBUFF ) );
			PlayData3.code = smTRANSCODE_PLAYDATA3;
			PlayData3.size = sizeof( smPLAYDATA3 );
			PlayData3.BuffCount[0] = 0;
			PlayData3.BuffCount[1] = SendTime;
			PlayData3.dwObjectSerial = dwObjectSerial;

			if ( lpsmsock->Send( (char *)&PlayData3 , sizeof( smPLAYDATA3 ) ) ) {
				//TransSendWait = 1;
				//TransLastSendCnt = PlayCounter;
				//TransLastSendTime = SendTime;
				//SetDistSendCnt( player );				//Åë½Å ½Ã°£Â÷ Á¶Àý
			}
		}
		*/

		//					lpCurPlayer->SetMotionFromCode( CHRMOTION_STATE_DEAD );
		//					if ( MotionInfo->State==CHRMOTION_STATE_ATTACK )


		if (lpsmsock->Send(lpTransBuff, ((smTRANS_PLAYDATA*)lpTransBuff)->size), TRUE) {
			//TransSendWait = 1;
			//TransLastSendCnt = PlayCounter;
			//TransLastSendTime = SendTime;
			//SetDistSendCnt( player );				//Åë½Å ½Ã°£Â÷ Á¶Àý
		}

	}
	//	}

	return TRUE;
}



//Ä³¸¯ÅÍÀÇ Æ÷°ýÀû Á¤º¸¸¦ ¼ÒÄÏÀ¸·Î »ó´ë¹æ¿¡ Àü¼Û
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

//½ºÅ³½ÃÀÛ
int smCHAR::BeginSkill(int SkilCode, int Level, smCHAR* lpTarChar, int x, int y, int z) { return TRUE; }

//¸ó½ºÅÍ ÀÏ¹Ý °ø°Ý ½ÃÀÛ
int smCHAR::BeginAttack_Monster() { return TRUE; }

//¸ó½ºÅÍ ½ºÅ³ ½ÃÀÛ
int smCHAR::BeginSkill_Monster() { return TRUE; }

//¸ó½ºÅÍ ½ºÅ³ ÀÌº¥Æ®
int smCHAR::EventSkill_Monster() { return TRUE; }

//½ºÅ³µ¿ÀÛÁ¾·á
int smCHAR::EndSkill() { return TRUE; }

//½ºÅ³ÀÌº¥Æ®
int smCHAR::EventSkill() { return TRUE; }


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
	/*
		if ( this==lpCurPlayer &&
			(abs(dwPlayTime-dwLastMouseMoveTime)>1024*60*3 ||
			 abs(dwPlayTime-dwLastCharMoveTime)>1024*60*3 ||
			 abs(dwFuncChkTime-dwPlayTime)>14*60*1024 ) ||
			 RestartPlayCount!=0 ) {
			//2~3ºÐ ÀÌ»ó ¸¶¿ì½º ¿òÁ÷ÀÓ ¾øÀ»¶© °ø°Ý ¹«È¿ ( º¸Ã÷ ÇÃ·¹ÀÌ ÀÚµ¿ ¹æ¾î )
			//2~3ºÐ ÀÌ»ó Ä³¸¯ÅÍ Á¦ÀÚ¸®¿¡ ¼­ÀÖÀ»¶§ °ø°Ý ¹«È¿ ( º¸Ã÷ ÇÃ·¹ÀÌ ÀÚµ¿ ¹æ¾î )
			return FALSE;
		}
	*/
	/*
		if ( smConfig.DebugMode ) {
			//½ºÅ³ Å×½ºÆ®
			if ( lpChar && VRKeyBuff[ VK_CONTROL ] && MouseButton[1] ) {
				//¹üÀ§Çü °ø°Ý
				//BeginSkill( SKILL_PLAY_METEO , 0 , lpChar ,0,0,0 );
				AddChatBuff( "¸ÞÅ×¿À »ç¿ëºÒ°¡~ ¸Þ·Õ" );
				return TRUE;
			}
		}
	*/

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
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
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
				rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
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
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
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
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
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
					rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
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
						rsDisplayDamgeToClient(lpPlayInfo, szDispDamage);
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
int InitMotionBlur() { return TRUE; }








int GetProjPosi(int& x3d, int& y3d, int& z3d, POINT3D* CameraPosi, POINT3D* CameraAngle)
{

	int x2d, y2d;
	smMATRIX	trans, rX, rY, rZ, rm;
	//	smPOINT3D	*pV;
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

	lstrcpy(szName, lpTransItem->szName);
	//lstrcpy( szModelName , lpTransItem->szModelName );
	SetPosi(lpTransItem->x, lpTransItem->y, lpTransItem->z);
	lpStgArea = lpTransItem->lpStgArea;

	ItemCode = lpTransItem->ItemCode;


	Angle.y = ((pX + pZ) >> 2) & ANGCLIP;

	if (sinGetItemInfo(ItemCode, szModelName) == FALSE || ItemCode == 0) {
		lstrcpy(szModelName, "char\\flag\\flag.ini");
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

int scITEM::Draw() { return TRUE; }


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