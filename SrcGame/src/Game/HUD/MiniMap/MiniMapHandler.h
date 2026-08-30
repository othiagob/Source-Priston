#pragma once

#include <Engine/UI/UITooltip.h>
#include <Engine/UI/UIImageLoader.h>
#include <Engine/Directx/DXGraphicEngine.h>
#include <Engine/Directx/DXFont.h>
#include <Engine/UI/UIImageRender.h>
#include <GameInfo.h>
#include <Def.h>
#include <field.h>

//Size of Box of Minimap(in pixels)
#define MINIMAP_LARGE_SIZE	384
#define MINIMAP_SMALL_SIZE	128

//Size of Image of Minimap (0~100%)
#define MINIMAP_TEXTURE_SIZE	512
#define MINIMAP_TEXTURE_SCALE	84

#define PACKET_UPDATE_BOSSTIME 0x4580607
#define PACKET_GET_NPCS 0x4580613
#define PACKET_GET_INDICATORS 0x4580614

// Máximo de npcs nos mapas do minimapa
#define MAX_NPCS_MAP 40
#define MAX_MAP_INDICATORS 30
#define DISTANCE_XY_60_meters			 2621440

struct MapIndicator
{
	enum class Type
	{
		None = 0,
		NextMapArrow = 1,
		HellSpawnMain = 2,
		HellSpawnSecondary = 3,
		BossIcon = 4,
	};

	int iValue;
	Type eType;
	int iCompactPosX;
	int iCompactPosZ;
	int iAngleY;
};

struct PacketMapIndicators
{
	int size;
	int code;
	int iMapID;
	int iCount;
	MapIndicator mapIndicators[MAX_MAP_INDICATORS];
};

struct MiniMapData
{
	UI::Image* psImage;
	UI::Image* psImageTitle;

	Rectangle2D				sRectangle;

	sFIELD* pcBaseMap;

	Point2D						sPosition;
};

struct Npcs
{
	char szName[32];
	Point3D Position;
};

// Recupera os npcs nos mapas mostrados no minimapa
struct PacketNpcsInMapAndNext
{
	int size;
	int code;
	Npcs sNpcs[MAX_NPCS_MAP];
};

struct PacketBossTimeUpdate
{
	int					size;
	int					code;
	char				szBossName[32];
	DWORD				dwTime;
	int					iMapID;
	int					SecondMapID;
};

class CMiniMapHandler : public CBaseWindow
{
public:
	CMiniMapHandler();
	CMiniMapHandler(EWindow eID);
	virtual									   ~CMiniMapHandler();

	void					Init();
	void					Render();
	void					Update();
	void					Shutdown();

	BOOL					OnMouseClick(class CMouse* pcMouse);
	void					OnMouseMove(class CMouse* pcMouse);
	void					OnResolutionChanged();

	void					HandlePacket(PacketBossTimeUpdate* psPacket);
	void					HandlePacket(PacketNpcsInMapAndNext* psPacket);
	void					HandlePacket(PacketMapIndicators* psPacket);
	void					LoadMiniMap(int iIndex, sFIELD* pcMap, smSTAGE3D* pcStage);
	float					GetMapScale(EMapID mapId);

	void					RenderNextMapIndicator(int iNextMapID, int iCompactPosX, int iCompactPosZ, int iAngleY, Point3D sUserPosition, int iSize);
	void					RenderMapIndicator(UI::Image* pImage, int iCompactPosX, int iCompactPosZ, int iAngle, Point3D sUserPosition, int iSize, const std::string szDescription = "");
	void					RenderMapIndicator(BOOL bIsSecondaryMap, MapIndicator* pMapIndicator, Point3D sUserPosition, int iSize);

	void					OnMiniMapSizeChanged();
	void					ResetMiniMapScreenPositions();
	void				    ResetMapTextureDeltas();

	void					OnChangeMap(int iMapID, int SecondMapID);
	void					OnRequestNpcs(int FirstID, int SecondID);

	int						lastMapID = 0;
	int						firstMapId = 0;
	int						secondMapId = 0;

	float						MapScale1 = 0;
	float						MapScale2 = 0;
	int						iMiniMapSize = MINIMAP_SMALL_SIZE;
	BOOL					bLargeMiniMap = FALSE;

	int						iMiniMapDeltaX = 0;
	int						iMiniMapDeltaY = 0;
	int						iMiniMapDeltaXPrev = 0;
	int						iMiniMapDeltaYPrev = 0;
	int						iMouseDownInitialX = 0;
	int						iMouseDownInitialY = 0;

	int						iMiniMapSmallScreenPositionX = 0;
	int						iMiniMapSmallScreenPositionY = 0;
	int						iMiniMapLargeScreenPositionX = 0;
	int						iMiniMapLargeScreenPositionY = 0;
	int						iMiniMapScreenPositionX = 0;
	int						iMiniMapScreenPositionY = 0;
private:
	BOOL					bInitialized = FALSE;
	BOOL					bActive = TRUE;

	UI::Image* pcMaskBackground = NULL;
	UI::Image* pcMaskBackgroundActive = NULL;
	UI::Image* pcBorderImage = NULL;
	UI::Image* pcBorderImageLarge = NULL;
	UI::Image* pcMapImage = NULL;
	UI::Image* pcUserPoint = NULL;
	UI::Image* pcNPCPoint = NULL;
	UI::Image* pcImageNextBoss = NULL;
	UI::Image* pcMapArrowImage = NULL;
	UI::Image* pcMapHellspawnMainImage = NULL;
	UI::Image* pcMapHellspawnSecondaryImage = NULL;
	UI::Image* pcMapBossImage = NULL;
	UI::Image* pcImageEnemyIcon = NULL;
	UI::Image* pcImageFriendlyIcon = NULL;
	UI::Image* pcBlacksmithPoint = NULL;
	UI::Image* pcMagicsmithPoint = NULL;
	UI::Image* pcTeleportPoint = NULL;
	UI::Image* pcWarehousePoint = NULL;
	UI::Image* pcUpgradePoint = NULL;
	UI::Image* pcSupplyPoint = NULL;
	UI::Image* pcaImagesPartyMemberIcon[6];
	UI::Image* pcImageRaidIcon = NULL;

	DX::Effect_ptr			pEffectMask;
	DX::RenderTarget_ptr	pRenderTargetMask;

	BOOL					bSelected;
	BOOL					bMapTitleSelected;
	Point2D					sPointMouse;

	Rectangle2D				sBox;
	Point2D					saMinimaps[4];

	int						iFontID = -1;
	int						iMiniMapIndicatorFont = -1;

	BOOL					bIsHoverUnit = FALSE;
	Rectangle2D				sHoverUnitBox = Rectangle2D();
	std::string				strHoverUnit;
	UI::Color				cHoverColor = UI::Color();
	UI::Tooltip_ptr			pTooltipUnit;

	BOOL					bShowBossTime = FALSE;
	BOOL					bFirstMapBossActive = FALSE;
	BOOL					bSecondMapBossActive = FALSE;

	std::string				strNextBossName;
	DWORD					dwNextBossTime = 0;
	DWORD					dwTickTime = 0;
	DWORD					dwTickTimeNpcs = 0;

	void					RenderMap(Point3D sUserPosition);
	void					RenderUser(Point3D sUserPosition, Point3D sUserAngle, int iScale);
	BOOL					RenderNPC(Npcs* pcUnitData, Point3D sUserPosition, int iSize);
	BOOL					RenderPlayer(smCHAR* pcUnitData, Point3D sUserPosition, int iUserClanID, int iSize);
	BOOL					RenderParty(Point3D sUserPosition, int iUserID, int iSize);

	BOOL					RenderMapTime();

	MiniMapData m_saMaps[2] = { 0 };
	MiniMapData* GetMiniMapData(int i) { return i == 0 ? &m_saMaps[0] : &m_saMaps[1]; }
	PacketNpcsInMapAndNext	chrNpcsInMapAndNext[MAX_NPCS_MAP];
	BOOL					IsCurrentUserMap(int iMapID);
	BOOL					IsCurrentUserSecondMap(int iMapID);
	BOOL					IsCurrentUserMap(MiniMapData* psMap);

	Rectangle2D				GetMiniMapBox() { return sBox; }
	Point2D					GetMiniMapPostion() { return Point2D(sBox.iX, sBox.iY); }
	void					SetMiniMapPostion(Rectangle2D sPosition) { sBox = sPosition; }

	void					DrawMinimap(MiniMapData* psMap, int iX, int iY, int iSize);

	void					SetMinimapSize(float fSize);

	Point2D					GetMinimapPositionFromCenter(Point3D sCenterPosition, Point3D sObjectPosition, int iWidthPoint, int iHeightPoint, int iSize);
	Point2D					GetMinimapPositionFromCenter(Point3D sCenterPosition, int iX, int iY, int iWidthPoint, int iHeightPoint, int iSize);

	std::map<int, PacketMapIndicators>	mMapIndicators;
};