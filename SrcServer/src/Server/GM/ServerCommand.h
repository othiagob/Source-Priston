#pragma once

#include <Windows.h>
#include <Chat/Chat.h>
#include <smPacket.h>
#include <Eventos/Questions.h>
#include <GM/GM.h>
#include <map>

#define SERVERCOMMAND CServerCommand::GetInstance()

#define PACKET_UPDATE_BOSSTIME 0x4580607
#define PACKET_GET_NPCS 0x4580613
#define PACKET_GET_INDICATORS 0x4580614

// Máximo de npcs nos mapas do minimapa
#define MAX_NPCS_MAP 40
#define MAX_MAP_INDICATORS 30
#define DISTANCE_XY_20_meters			  291600

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

class CServerCommand
{
public:
	static              CServerCommand* GetInstance() { static CServerCommand instance; return &instance; }

	CServerCommand();
	virtual ~CServerCommand();

	void OnActivateGameMaster(rsPLAYINFO* pcUser);
	void OnGameMasterEventCommand(rsPLAYINFO* pcUser, char* pszBuff);
	void OnGameMasterAdminCommand(rsPLAYINFO* pcUser, char* pszBuff);
	void OnReceiveChat(rsPLAYINFO* pcUser, PacketChat* psPacket);

	void SendBossTime(rsPLAYINFO* pcUser, PacketBossTimeUpdate* psPacket);
	void SendNpcs(rsPLAYINFO* pcUser, smTRANS_COMMAND* Packet);

	// Carrega indicadores dos mapas
	void LoadIndicators();

	// Envia os indicadores para o game
	void SendMapIndicators(rsPLAYINFO* pcUser, int iMapID, BOOL bForceSend);

	BOOL OnGameMasterCommand(rsPLAYINFO* pcUser, char* pszBuff);
	BOOL OnPlayerCommand(rsPLAYINFO* pcUser, char* pszBuff);
	BOOL AddMapIndicator(rsPLAYINFO* pcUser, int iMapID, MapIndicator::Type eMapIndicatorType, int iValue, int iCompactPosX, int iCompactPosZ, int iAngle);
	BOOL RemoveMapIndicatorNearMe(rsPLAYINFO* pcUser);
private:
	// Indicadores
	std::map<int, PacketMapIndicators>		mMapIndicators;
};