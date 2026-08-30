#pragma once

#include <Utils\\_common.h>
#include <Utils\\Geometry.h>
#include "sinbaram/sinLinkHeader.h"

#define DISTANCE_MAX_PARTY				1228800
#define DISTANCE_XY_15_meters			  164025
#define DISTANCE_MISC_Y_EX					1000
#define MAX_PARTY_MEMBERS		6
#define MAX_RAID				2

enum ERaidState
{
	RAIDSTATE_Failed = -1,

	RAIDSTATE_None,
	RAIDSTATE_Success,
	RAIDSTATE_FullRaid,
};

enum EPartyState
{
	PARTYSTATE_Failed = -1,

	PARTYSTATE_None,
	PARTYSTATE_Success,
	PARTYSTATE_AlreadyParty,
	PARTYSTATE_FullParty,
	PARTYSTATE_RequestInvite,
};

enum EPartyAction
{
	PARTYACTION_None,

	PARTYACTION_Leave,
	PARTYACTION_Kick,
	PARTYACTION_Delegate,
	PARTYACTION_DisbandParty,
	PARTYACTION_DisbandRaid,
	PARTYACTION_ChangeMode,
};

enum EPartyMode
{
	PARTYMODE_Normal,
	PARTYMODE_EXP,
	PARTYMODE_Hunt,
};

struct PartyInfo
{
	int							iID;
	int							iMembersCount;
	int							iRaidCount;
	int							iMode;
	rsPLAYINFO* pcLeader;
	PartyInfo* pcRaidLeader;
	rsPLAYINFO* pcaMembers[MAX_PARTY_MEMBERS - 1];
	PartyInfo* pcaRaid[MAX_RAID - 1];
};

struct PartyMemberData
{
	int							iID;

	short						sLevel;
	short						life[2];
	short						mana[2];
	Point3D						sPosition;
};

struct PartyRaidMemberData
{
	int							iID;

	short						life[2];
	Point3D						sPosition;
};

struct PartyMember
{
	char						szCharacterName[32];
	char						cCharacterClass;

	int							iClanID;

	PartyMemberData				sData;
};

struct PartyRaidMember
{
	char						szCharacterName[32];

	PartyRaidMemberData			sData;
};

struct PartyRaid
{
	char						cRaidID;

	PartyRaidMember				saMembers[MAX_PARTY_MEMBERS];
};

struct PacketUpdateParty
{
	int size;
	int code;
	char						cMembersCount;
	char						cPartyMode;
	PartyMember					saMembers[MAX_PARTY_MEMBERS];

	char						cRaidCount;
	PartyRaid					saRaid[MAX_RAID - 1];

	PacketUpdateParty() { cMembersCount = 0; cRaidCount = 0; }
};

struct PacketUpdatePartyData
{
	int size;
	int code;
	char						cMembersCount;
	char						cRaidMembersCount;

	PartyMemberData				saMembers[MAX_PARTY_MEMBERS];
	PartyRaidMemberData			saRaidMember[MAX_RAID - 1][MAX_PARTY_MEMBERS];
};

struct PacketRequestParty
{
	int size;
	int code;

	PacketRequestParty() = default;
	PacketRequestParty(int _iTargetID) { iID = 0; iTargetID = _iTargetID; eState = PARTYSTATE_None; }

	int							iID;
	int							iTargetID;
	EPartyState					eState;
	char						szCharacterName[32];
};

struct PacketLeaveRaid
{
	int size;
	int code;
	int							iID;
};

struct PacketRequestRaid
{
	int size;
	int code;
	PacketRequestRaid() = default;
	PacketRequestRaid(int _iTargetID) { iID = 0; iTargetID = _iTargetID; eState = RAIDSTATE_None; }

	int							iID;
	int							iTargetID;
	ERaidState					eState;
	char						szCharacterName[32];
};

struct PacketJoinRaid
{
	int size;
	int code;
	int							iID;
};

struct PacketJoinParty
{
	int size;
	int code;
	int							iID;
};

struct PacketActionParty
{
	int size;
	int code;
	EPartyAction				eAction;
	int							iUserID;

	PacketActionParty() = default;
	PacketActionParty(EPartyAction _eAction, int _iUserID = -1) { eAction = _eAction; iUserID = _iUserID; }
};

struct PacketSkillPartyData
{
	int size;
	int code;
	int										iSkillID;
	int										iLevel;
	int										iParam1;
	int										iParam2;
	int										iParam3;
	int										iClass;
	int										iCount;
	int										iaID[(MAX_PARTY_MEMBERS * MAX_RAID) + 1];
};


class CPartyData
{
public:
	int							iMembersCount;
	EPartyMode					eMode;
	PartyMember					saMembers[MAX_PARTY_MEMBERS];

	int							iRaidCount;
	PartyRaid					saRaid[MAX_RAID - 1];
};

char* GetPartyMode(EPartyMode eMode);