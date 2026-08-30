#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "..\\Database\SQLConnection.h"
#include "..\\sinbaram\\sinLinkHeader.h"

#include "Utils/strings.h"
#include "..\\nlohmann\\json.hpp"

#define MAX_MONSTERS 400

// Vetor com nome dos mapas
std::vector<std::string> FieldName;

using json = nlohmann::json;

extern int DefaultItemCount;
extern int chrMonsterCnt;

extern int SetLowerItem(sDEF_ITEMINFO* lpDefItem);
extern int ConvMoveSpeed(float fSpeed);
extern int ConvPercent8(int percent100);
extern char* SetNPCMsg(char* szMsg);

// Itens
extern sDEF_ITEMINFO* DefaultItems;
sDEF_ITEMINFO DefItems;
sDEF_ITEMINFO* lpDefItem;
sITEMINFO* lpItem;

// Monstros
extern smCHAR_INFO* chrMonsterList;
extern smCHAR_MONSTER_INFO* chrMonInfoList;

struct szSTATE_CODE {
	char* szCodeName;
	DWORD	dwCode;
};

// Efeitos dos monstros
szSTATE_CODE	charSoundConde[] = {
	{	"CYCLOPS"	,	snCHAR_SOUND_CYCLOPS },
	{	"HOBGOBLIN"	,	snCHAR_SOUND_HOBGOBLIN },
	{	"IMP"		,	snCHAR_SOUND_IMP },
	{	"MINIG"		,	snCHAR_SOUND_MINIG },
	{	"PLANT"		,	snCHAR_SOUND_PLANT },
	{	"SKELETON"	,	snCHAR_SOUND_SKELETON },
	{	"ZOMBI"		,	snCHAR_SOUND_ZOMBI },
	{	"OBIT"		,	snCHAR_SOUND_OBIT },

	{	"HOPT"		,	snCHAR_SOUND_HOPT },
	{	"BARGON"	,	snCHAR_SOUND_BARGON },
	{	"LEECH"		,	snCHAR_SOUND_LEECH },
	{	"MUSHROOM"	,	snCHAR_SOUND_MUSHROOM },

	{	"ARMA"		,	snCHAR_SOUND_ARMA },
	{	"ARMADIL"	,	snCHAR_SOUND_ARMA },
	{	"SCORPION"	,	snCHAR_SOUND_SCORPION },

	{	"HEADCUTTER",	snCHAR_SOUND_HEADCUTTER },
	{	"SANDLEM"	,	snCHAR_SOUND_SANDLEM },
	{	"WEB"		,	snCHAR_SOUND_WEB },
	{	"HOPYKING"	,	snCHAR_SOUND_HOPYKING },
	{	"CRIP"		,	snCHAR_SOUND_CRIP },
	{	"BUMA"		,	snCHAR_SOUND_BUMA },
	{	"DECOY"		,	snCHAR_SOUND_DECOY },
	{	"DORAL"		,	snCHAR_SOUND_DORAL },
	{	"FIGON"		,	snCHAR_SOUND_FIGON },
	{	"GOLEM"		,	snCHAR_SOUND_STONEGIANT },
	{	"GREVEN"	,	snCHAR_SOUND_GREVEN },
	{	"ILLUSIONKNIGHT" , snCHAR_SOUND_ILLUSIONKNIGHT },
	{	"SKELETONRANGE" , snCHAR_SOUND_SKELETONRANGE },
	{	"SKELETONMELEE" , snCHAR_SOUND_SKELETONMELEE },
	{	"WOLVERLIN" , snCHAR_SOUND_WOLVERLIN },

	{	"RABIE"			, snCHAR_SOUND_RABIE			},
	{	"MUDY"			, snCHAR_SOUND_MUDY				},
	{	"SEN"			, snCHAR_SOUND_SEN				},
	{	"EGAN"			, snCHAR_SOUND_EGAN				},
	{	"BEEDOG"		, snCHAR_SOUND_BEEDOG			},
	{	"MUTANTPLANT"	, snCHAR_SOUND_MUTANTPLANT		},
	{	"MUTANTRABIE"	, snCHAR_SOUND_MUTANTRABIE		},
	{	"MUTANTTREE"	, snCHAR_SOUND_MUTANTTREE		},
	{	"AVELISK"		, snCHAR_SOUND_AVELISK			},
	{	"NAZ"			, snCHAR_SOUND_NAZ				},
	{	"MUMMY"			, snCHAR_SOUND_MUMMY			},
	{	"HULK"			, snCHAR_SOUND_HULK				},
	{	"HUNGKY"		, snCHAR_SOUND_HUNGKY			},
	{	"SUCCUBUS"		, snCHAR_SOUND_SUCCUBUS			},
	{	"DAWLIN"		, snCHAR_SOUND_DAWLIN			},
	{	"SHADOW"		, snCHAR_SOUND_SHADOW			},
	{	"BERSERKER"		, snCHAR_SOUND_BERSERKER		},
	{	"IRONGUARD"		, snCHAR_SOUND_IRONGUARD		},
	{	"FURY"			, snCHAR_SOUND_FURY				},
	{	"SLIVER"		, snCHAR_SOUND_SLIVER			},

	{	"RATOO"			, snCHAR_SOUND_RATOO			},
	{	"STYGIANLORD"	, snCHAR_SOUND_STYGIANLORD		},
	{	"OMICRON"		, snCHAR_SOUND_OMICRON			},
	{	"D-MACHINE"		, snCHAR_SOUND_DMACHINE			},
	{	"METRON"		, snCHAR_SOUND_METRON			},
	{	"MRGHOST"		, snCHAR_SOUND_MRGHOST			},

	{	"VAMPIRICBAT"	, snCHAR_SOUND_VAMPIRICBAT		},
	{	"MIREKEEPER"	, snCHAR_SOUND_MIREKEEPER		},
	{	"MUFFIN"		, snCHAR_SOUND_MUFFIN			},
	{	"SOLIDSNAIL"	, snCHAR_SOUND_SOLIDSNAIL		},
	{	"BEEVIL"		, snCHAR_SOUND_BEEVIL			},
	{	"NIGHTMARE"		, snCHAR_SOUND_NIGHTMARE		},
	{	"STONEGOLEM"	, snCHAR_SOUND_STONEGOLEM		},
	{	"THORNCRAWLER"	, snCHAR_SOUND_THORNCRAWLER		},
	{	"HEAVYGOBLIN"	, snCHAR_SOUND_HEAVYGOBLIN		},
	{	"EVILPLANT"		, snCHAR_SOUND_EVILPLANT		},
	{	"HAUNTINGPLANT"	, snCHAR_SOUND_HAUNTINGPLANT	},
	{	"DARKKNIGHT"	, snCHAR_SOUND_DARKKNIGHT		},
	{	"GUARDIAN-SAINT", snCHAR_SOUND_GUARDIAN_SAINT	},

	{	"CHAINGOLEM"	, snCHAR_SOUND_CHAINGOLEM		},
	{	"DEADZONE"		, snCHAR_SOUND_DEADZONE			},
	{	"GROTESQUE"		, snCHAR_SOUND_GROTESQUE		},
	{	"HYPERMACHINE"	, snCHAR_SOUND_HYPERMACHINE		},
	{	"IRONFIST"		, snCHAR_SOUND_IRONFIST			},
	{	"MORGON"		, snCHAR_SOUND_MORGON			},
	{	"MOUNTAIN"		, snCHAR_SOUND_MOUNTAIN			},
	{	"RAMPAGE"		, snCHAR_SOUND_RAMPAGE			},
	{	"RUNICGUARDIAN"	, snCHAR_SOUND_RUNICGUARDIAN	},
	{	"SADNESS"		, snCHAR_SOUND_SADNESS			},
	{	"TOWERGOLEM"	, snCHAR_SOUND_TOWERGOLEM		},
	{	"VAMPIRICBEE"	, snCHAR_SOUND_VAMPRICBEE		},
	{	"VAMPIRICMACHINE", snCHAR_SOUND_VAMPRICMACHINE	},
	{	"AVELINARCHER"	, snCHAR_SOUND_AVELIN_ARCHER	},
	{	"AVELINQUEEN"	, snCHAR_SOUND_AVELIN_QUEEN		},
	{	"BABEL"			, snCHAR_SOUND_BABEL			},

	{	"MYSTIC"		, snCHAR_SOUND_MYSTIC			},
	{	"ICEGOBLIN"		, snCHAR_SOUND_ICEGOBLIN		},
	{	"COLDEYE"		, snCHAR_SOUND_COLDEYE			},
	{	"FROZEN"		, snCHAR_SOUND_FROZEN			},
	{	"ICEGOLEM"		, snCHAR_SOUND_ICEGOLEM			},
	{	"FROST"			, snCHAR_SOUND_FROST			},
	{	"CHAOSCARA"		, snCHAR_SOUND_CHAOSCARA		},
	{	"DEATHKNIGHT"	, snCHAR_SOUND_DEATHKNIGHT		},

	{	"GREATE_GREVEN"	, snCHAR_SOUND_GREATE_GREVEN	},
	{	"LIZARDFOLK"	, snCHAR_SOUND_LIZARDFOLK		},
	{	"M_LORD"		, snCHAR_SOUND_M_LORD			},
	{	"SPIDER"		, snCHAR_SOUND_SPIDER			},
	{	"STINGRAY"		, snCHAR_SOUND_STINGRAY			},
	{	"STRIDER"		, snCHAR_SOUND_STRIDER			},
	{	"OMU"			, snCHAR_SOUND_OMU				},

	{	"TURTLECANNON"	, snCHAR_SOUND_TURTLE_CANNON	},
	{	"DEVILBIRD"		, snCHAR_SOUND_DEVIL_BIRD		},
	{	"BLIZZARDGIANT"	, snCHAR_SOUND_BLIZZARD_GIANT	},
	{	"KELVEZU"		, snCHAR_SOUND_KELVEZU			},

	{	"DARKPHALANX"	, snCHAR_SOUND_DARKPHALANX		},
	{	"BLOODYKNIGHT"	, snCHAR_SOUND_BLOODYKNIGHT		},
	{	"CHIMERA"		, snCHAR_SOUND_CHIMERA			},
	{	"FIREWORM"		, snCHAR_SOUND_FIREWORM			},
	{	"HELLHOUND"		, snCHAR_SOUND_HELLHOUND		},
	{	"DARKGUARD"		, snCHAR_SOUND_DARKGUARD		},
	{	"DARKMAGE"		, snCHAR_SOUND_DARKMAGE			},

	{	"MONMOKOVA"		, snCHAR_SOUND_MOKOVA			},
	{	"MONTEMPLEGUARD", snCHAR_SOUND_TEMPLEGUARD		},
	{	"MONSETO"		, snCHAR_SOUND_SETO				},
	{	"MONKINGSPIDER"	, snCHAR_SOUND_KINGSPIDER		},

	{	"D_KN"			, snCHAR_SOUND_REVIVED_KNIGHT	},
	{	"D_MAGI"		, snCHAR_SOUND_REVIVED_MAGICIAN },
	{	"D_ATAL"		, snCHAR_SOUND_REVIVED_ATALANTA },
	{	"D_FI"			, snCHAR_SOUND_REVIVED_FIGTHER	},
	{	"D_AR"			, snCHAR_SOUND_REVIVED_ARCHER	},
	{	"D_MECA"		, snCHAR_SOUND_REVIVED_MECANICIAN },
	{	"D_PA"			, snCHAR_SOUND_REVIVED_PIKEMAN	},
	{	"D_PR"			, snCHAR_SOUND_REVIVED_PRIESTESS},
	{	"DEADHOPT"		, snCHAR_SOUND_DEADHOPT			},
	{	"DEADKINGHOPY"	, snCHAR_SOUND_DEADKINGHOPY		},
	{	"GORGON"		, snCHAR_SOUND_GORGON			},
	{	"HOBOGOLEM"		, snCHAR_SOUND_HOBOGOLEM		},

	{	"NIKEN"			, snCHAR_SOUND_NIKEN			},
	{	"MIMIC"			, snCHAR_SOUND_MIMIC			},
	{	"KINGBAT"		, snCHAR_SOUND_KINGBAT			},
	{	"GOBLINSHAMAN"	, snCHAR_SOUND_GOBLINSHAMAN		},
	{	"HEST"			, snCHAR_SOUND_HEST				},

	{	"RUCA"			, snCHAR_SOUND_RUCA				},
	{	"NAZSENIOR"		, snCHAR_SOUND_NAZSENIOR		},
	{	"IGOLATION"		, snCHAR_SOUND_IGOLATION		},
	{	"KAKOA"			, snCHAR_SOUND_KAKOA			},
	{	"SPRIN"			, snCHAR_SOUND_SPRIN			},
	{	"UNDEADMAPLE"	, snCHAR_SOUND_UNDEADMAPLE		},
	{	"XETAN"			, snCHAR_SOUND_XETAN			},

	{	"BEBECHICK"		, snCHAR_SOUND_BEBECHICK		},
	{	"PAPACHICK"		, snCHAR_SOUND_PAPACHICK		},

	{	"MORIF"			, snCHAR_SOUND_NPC_MORIF		},
	{	"MOLLYWOLF"		, snCHAR_SOUND_NPC_MOLLYWOLF	},
	{	"SKILLMASTER"	, snCHAR_SOUND_NPC_SKILLMASTER	},
	{	"MAGE"			, snCHAR_SOUND_NPC_MAGICMASTER	},
	{	"WORLDCUP"		, snCHAR_SOUND_WORLDCUP			},
	{	"WATERMELON"		, snCHAR_SOUND_WATERMELON			},

	{	"WOLVERIN"			, snCHAR_SOUND_S_WOLVERLIN		},
	{	"METALGOLEM"		, snCHAR_SOUND_S_METALGOLEM		},
	{	"FIREELEMENTAL"		, snCHAR_SOUND_S_F_ELEMENTAL	},

	{	"CASTLEDOOR"		, snCHAR_SOUND_CASTLE_DOOR		},
	{	"T_CRYSTAL_R"		, snCHAR_SOUND_CASTLE_CRYSTAL_R	},
	{	"T_CRYSTAL_G"		, snCHAR_SOUND_CASTLE_CRYSTAL_G	},
	{	"T_CRYSTAL_B"		, snCHAR_SOUND_CASTLE_CRYSTAL_B	},
	{	"T_CRYSTAL_N"		, snCHAR_SOUND_CASTLE_CRYSTAL_N	},
	{	"TOWER-B"			, snCHAR_SOUND_CASTLE_TOWER_B	},

	{	"SOLDIER_A"			, snCHAR_SOUND_CASTLE_SOLDER_A	},
	{	"SOLDIER_B"			, snCHAR_SOUND_CASTLE_SOLDER_B	},
	{	"SOLDIER_C"			, snCHAR_SOUND_CASTLE_SOLDER_C	},

	{	"S_AVELIN"				, snCHAR_SOUND_S_AVELIN	},
	{	"S_BAGON"				, snCHAR_SOUND_S_BAGON	},
	{	"S_BEEDOG"				, snCHAR_SOUND_S_BEEDOG	},
	{	"S_BEEVIL"				, snCHAR_SOUND_S_BEEVIL	},
	{	"S_BERSERKER"			, snCHAR_SOUND_S_BERSERKER	},
	{	"S_BUMA"					, snCHAR_SOUND_S_BUMA	},
	{	"S_COKRIS"				, snCHAR_SOUND_S_COKRIS	},
	{	"S_COLDEYE"			, snCHAR_SOUND_S_COLDEYE	},
	{	"S_CRYPT"				, snCHAR_SOUND_S_CRYPT	},
	{	"S_CYCLOPS"			, snCHAR_SOUND_S_CYCLOPS	},
	{	"S_DEADZONE"			, snCHAR_SOUND_S_DEADZONE	},
	{	"S_DECOY"				, snCHAR_SOUND_S_DECOY	},
	{	"S_D-Machine"			, snCHAR_SOUND_S_DMACHINE	},
	{	"S_EVILSNAIL"			, snCHAR_SOUND_S_EVILSNAIL	},
	{	"S_GREVEN"				, snCHAR_SOUND_S_GREVEN	},
	{	"S_GROTESQUE"		, snCHAR_SOUND_S_GROTESQUE	},
	{	"S_ICEGOBLIN"			, snCHAR_SOUND_S_ICEGOBLIN	},
	{	"S_ICEGOLEM"			, snCHAR_SOUND_S_ICEGOLEM	},
	{	"S_INCUBUS"				, snCHAR_SOUND_S_INCUBUS	},
	{	"S_KINGHOPY"			, snCHAR_SOUND_S_KINGHOPY	},
	{	"S_LEECH"				, snCHAR_SOUND_S_LEECH	},
	{	"S_LIZARDFOLK"		, snCHAR_SOUND_S_LIZARDFOLK	},
	{	"S_MEPHIT"				, snCHAR_SOUND_S_MEPHIT	},
	{	"S_METRON"				, snCHAR_SOUND_S_METRON	},
	{	"S_MUFFIN"				, snCHAR_SOUND_S_MUFFIN	},
	{	"S_MUMMY"				, snCHAR_SOUND_S_MUMMY	},
	{	"S_NAZ"					, snCHAR_SOUND_S_NAZ	},
	{	"S_OMEGA"				, snCHAR_SOUND_S_OMEGA	},
	{	"S_RAMPAGE"			, snCHAR_SOUND_S_RAMPAGE	},
	{	"S_SADNESS"			, snCHAR_SOUND_S_SADNESS	},
	{	"S_SLAUGHTER"			, snCHAR_SOUND_S_SLAUGHTER	},
	{	"S_SLAYON"				, snCHAR_SOUND_S_SLAYON	},
	{	"S_SLIVER"				, snCHAR_SOUND_S_SLIVER	},
	{	"S_TITAN"					, snCHAR_SOUND_S_TITAN	},
	{	"S_TOWERGOLEM"		, snCHAR_SOUND_S_TOWERGOLEM	},
	{	"S_TYPHOON"			, snCHAR_SOUND_S_TYPHOON	},
	{	"S_VAMPIRICBAT"		, snCHAR_SOUND_S_VAMPIRICBAT	},
	{	"S_WITCH"					, snCHAR_SOUND_S_WITCH	},
	{	"S_ZOMBIE"				, snCHAR_SOUND_S_ZOMBIE	},


	{	"" , 0 }
};

GameServer::GameServer()
{
}

GameServer::~GameServer()
{
}

int setSpec(short primarySpec, char secondarySpec[20]) {

	std::vector<string> SecondarySpecs;

	switch (primarySpec)
	{
	case 1:
		lpItem->JobCodeMask = JobDataBase[1].JobBitCode;
		break;
	case 2:
		lpItem->JobCodeMask = JobDataBase[0].JobBitCode;
		break;
	case 3:
		lpItem->JobCodeMask = JobDataBase[3].JobBitCode;
		break;
	case 4:
		lpItem->JobCodeMask = JobDataBase[2].JobBitCode;
		break;
	case 5:
		lpItem->JobCodeMask = JobDataBase[17].JobBitCode;
		break;
	case 6:
		lpItem->JobCodeMask = JobDataBase[16].JobBitCode;
		break;
	case 7:
		lpItem->JobCodeMask = JobDataBase[19].JobBitCode;
		break;
	case 8:
		lpItem->JobCodeMask = JobDataBase[18].JobBitCode;
		break;
	}

	SecondarySpecs.clear();
	SecondarySpecs = split(secondarySpec, ',');

	for (auto specs : SecondarySpecs)
	{
		if (boost::iequals(specs, "1"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[1].JobBitCode;
		if (boost::iequals(specs, "2"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[0].JobBitCode;
		if (boost::iequals(specs, "3"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[3].JobBitCode;
		if (boost::iequals(specs, "4"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[2].JobBitCode;
		if (boost::iequals(specs, "5"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[17].JobBitCode;
		if (boost::iequals(specs, "6"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[16].JobBitCode;
		if (boost::iequals(specs, "7"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[19].JobBitCode;
		if (boost::iequals(specs, "8"))
			DefaultItems[DefaultItemCount].dwJobBitCode_Random[DefaultItems[DefaultItemCount].JobBitCodeRandomCount++] = JobDataBase[18].JobBitCode;
	}

	return TRUE;
}

int setItemCode(char itemName[10])
{
	for (int cnt = 0; cnt < MAX_ITEM; cnt++)
	{
		if (lstrcmpi(itemName, sItem[cnt].LastCategory) == 0)
		{
			lpItem->CODE = sItem[cnt].CODE;
			break;
		}
	}
	return TRUE;
}

int addItem() {
	memcpy(&DefItems, &DefaultItems[DefaultItemCount], sizeof(sDEF_ITEMINFO));
	SetLowerItem(&DefItems);

	DefItems.Item.BackUpKey = 0;
	DefItems.Item.BackUpChkSum = 0;
	DefItems.Item.dwCreateTime = 0;

	DefaultItems[DefaultItemCount].DefCompressDataLen =
		EecodeCompress((BYTE*)&DefItems.Item, (BYTE*)DefaultItems[DefaultItemCount].DefCompressData, sizeof(sITEMINFO));

	DefaultItemCount++;

	if (DefaultItemCount >= 1024) return FALSE;

	return TRUE;
}

void setExpireTime(short expireTime) {
	if (expireTime > 0)
		lpItem->expireTime = expireTime;
}

void setQuestID(short questID) {
	if (questID > 0)
		lpItem->UniqueItem = questID;
}

DWORD findTableItem(const char* szCode) {
	DWORD itemCode;

	for (int cnt = 0; cnt < MAX_ITEM; cnt++) {
		if (STRINGCOMPAREI(szCode, sItem[cnt].LastCategory)) {
			itemCode = sItem[cnt].CODE;
			return itemCode;
		}
	}

	return NULL;
}

void setDropItems(smCHAR_MONSTER_INFO* MonsterInfo, int monsterPos) {

	auto db = SQLConnection::GetConnection(DATABASEID_GameServer);

	int dropID = 0, dropChance = 0, goldMin = 0, goldMax = 0;
	int isPublicDrop = 0, Quantity = 0;

	if (db->Open())
	{
		if (db->Prepare("SELECT * FROM DropList WHERE MonsterName=?"))
		{
			db->BindInputParameter(MonsterInfo->szName, 1, PARAMTYPE_String);

			if (db->Execute())
			{
				db->GetData(2, PARAMTYPE_Integer, &dropID);
				db->GetData(4, PARAMTYPE_Integer, &isPublicDrop);
				db->GetData(5, PARAMTYPE_Integer, &Quantity);

				MonsterInfo->AllSeeItem = isPublicDrop;
				MonsterInfo->FallItemMax = Quantity;
			}
		}

		db->Close();

		if (db->Open())
		{
			if (db->Prepare("SELECT * FROM DropItem WHERE DropID=?"))
			{
				db->BindInputParameter(&dropID, 1, PARAMTYPE_Integer);

				db->Execute(false);

				while (db->NextRow())
				{
					char dropList[512] = { 0 };

					db->GetData(2, PARAMTYPE_String, dropList, sizeof(dropList));
					db->GetData(3, PARAMTYPE_Integer, &dropChance);
					db->GetData(4, PARAMTYPE_Integer, &goldMin);
					db->GetData(5, PARAMTYPE_Integer, &goldMax);

					MonsterInfo->FallItemPerMax += dropChance;

					if (STRINGCOMPAREI(dropList, "Gold"))
					{
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].dwItemCode = sinGG1 | sin01;
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].sPrice[0] = goldMin;
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].sPrice[1] = goldMax;
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].Percentage = dropChance;
						MonsterInfo->FallItemCount++;
					}
					else if (STRINGCOMPAREI(dropList, "Air"))
					{
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].dwItemCode = 0;
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].sPrice[0] = goldMin;
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].sPrice[1] = goldMax;
						MonsterInfo->FallItems[MonsterInfo->FallItemCount].Percentage = dropChance;
						MonsterInfo->FallItemCount++;
					}

					std::vector<std::string> vItems = split(dropList, ' ');

					//std::random_shuffle(vItems.begin(), vItems.end()); // Randomiza o vector

					UINT uVecSize = vItems.size() + MonsterInfo->FallItemCount;

					if (uVecSize > 200)
						uVecSize = 200;

					int iPosition = 0;

					UINT uTotalItem = uVecSize - MonsterInfo->FallItemCount;

					for (UINT u = MonsterInfo->FallItemCount; u < uVecSize; u++)
					{
						DWORD ItemCode = findTableItem(vItems[iPosition++].c_str());

						if (ItemCode)
						{
							if (goldMin == 1) // Drop Extra
							{
								if (MonsterInfo->FallItemPlusCount < FALLITEM2_MAX)
								{
									MonsterInfo->FallItems_Plus[MonsterInfo->FallItemPlusCount].Percentage = dropChance;
									MonsterInfo->FallItems_Plus[MonsterInfo->FallItemPlusCount].dwItemCode = ItemCode;
									MonsterInfo->FallItemPlusCount++;
								}
							}
							else
							{
								MonsterInfo->FallItems[MonsterInfo->FallItemCount].dwItemCode = ItemCode;
								MonsterInfo->FallItems[MonsterInfo->FallItemCount].Percentage = dropChance / uTotalItem;
								MonsterInfo->FallItemCount++;
							}
						}
					}
				}
			}

			db->Close();
		}
	}
	chrMonsterList[monsterPos].lpMonInfo = &chrMonInfoList[monsterPos];
	chrMonInfoList[monsterPos].dwAutoCharCode = GetSpeedSum(chrMonsterList[monsterPos].szName);
	ReformCharForm(&chrMonsterList[monsterPos]);
}

void GameServer::readNpcsFromDB(smCHAR* Npc, int uniqueID)
{
	auto db = SQLConnection::GetConnection(DATABASEID_GameServer);

	Npc->smCharInfo.szModelName2[0] = 0;
	Npc->smCharInfo.wPlayClass[1] = 0;
	Npc->smCharInfo.ArrowPosi[0] = 0;
	Npc->smCharInfo.ArrowPosi[1] = 0;

	if (&Npc->smMonsterInfo) {
		ZeroMemory(&Npc->smMonsterInfo, sizeof(smCHAR_MONSTER_INFO));
		Npc->smMonsterInfo.MoveRange = 64 * fONE;
		Npc->smMonsterInfo.FallItemCount = 0;
		Npc->smMonsterInfo.FallItemMax = 0;
		Npc->smMonsterInfo.FallItemPerMax = 0;

		Npc->smMonsterInfo.DamageStunPers = 100;
		Npc->smMonsterInfo.UseEventModel = 0;
	}

	Npc->smCharInfo.State = FALSE;

	int cnt = 0, sellID = 0, Code = 0, CodeType = 1;
	int messageID = 0;
	char monsterSound[32] = { 0 };
	char message[180] = { 0 };
	char itemList[500] = { 0 };

	if (db->Open())
	{
		if (db->Prepare("SELECT * FROM NpcList WHERE Active=1 AND UniqueID=?"))
		{
			db->BindInputParameter(&uniqueID, 1, PARAMTYPE_Integer);

			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					float npcSize = 0;

					db->GetData(3, PARAMTYPE_String, Npc->smCharInfo.szModelName, sizeof(Npc->smCharInfo.szModelName));
					db->GetData(4, PARAMTYPE_String, Npc->smMonsterInfo.szName, sizeof(Npc->smMonsterInfo.szName));
					db->GetData(5, PARAMTYPE_Integer, &Code);
					db->GetData(7, PARAMTYPE_Float, &npcSize);
					db->GetData(8, PARAMTYPE_Integer, &Npc->smCharInfo.Level);
					db->GetData(9, PARAMTYPE_Integer, &messageID);
					db->GetData(10, PARAMTYPE_Integer, &sellID);
					db->GetData(12, PARAMTYPE_String, monsterSound, sizeof(monsterSound));
					db->GetData(13, PARAMTYPE_Integer, &CodeType);

					Npc->smCharInfo.wPlayClass[1] = (short)(npcSize * fONE);
					if (Npc->smCharInfo.wPlayClass[1] == fONE) Npc->smCharInfo.wPlayClass[1] = 0;

					switch (Code) {
					case 1:
						Npc->smMonsterInfo.SkillMaster = TRUE;
						Npc->smMonsterInfo.SkillChangeJob = CodeType;

						if (CodeType == 1)
							Npc->smCharInfo.wPlayClass[0] = 1003; // Quest 55 Ricarten
						else if (CodeType == 2)
							Npc->smCharInfo.wPlayClass[0] = 1004; // Quest 55 Pillai
						break;
					case 2:
						Npc->smMonsterInfo.SkillChangeJob = TRUE;
						break;
					case 3:
						Npc->smMonsterInfo.EventNPC = CodeType;
						break;
					case 4:
						Npc->smMonsterInfo.WareHouseMaster = TRUE;

						if (CodeType > 0) // Quest baú de pillai
							Npc->smCharInfo.wPlayClass[0] = CodeType;
						break;
					case 5:
						Npc->smMonsterInfo.ItemMix = TRUE;
						break;
					case 6:
						Npc->smMonsterInfo.Manufacture = TRUE;
						break;
					case 7:
						Npc->smMonsterInfo.ItemMix = 200;
						break;
					case 8:
						Npc->smMonsterInfo.ItemAging = TRUE;
						break;
					case 9:
						Npc->smMonsterInfo.MixtureReset = TRUE;
						break;
					case 10:
						Npc->smMonsterInfo.ItemShop = TRUE;
						break;
					case 11:
						Npc->smMonsterInfo.ItemShopTime = TRUE;
						break;
					case 12:
						Npc->smMonsterInfo.CollectMoney = TRUE;
						break;
					case 13:
						Npc->smMonsterInfo.EventGift = TRUE;
						break;
					case 14:
						Npc->smMonsterInfo.ClanNPC = TRUE;
						break;
					case 15:
						Npc->smMonsterInfo.GiftExpress = TRUE;
						break;
					case 16:
						Npc->smMonsterInfo.WingQuestNpc = CodeType;
						break;
					case 17:
						Npc->smMonsterInfo.WingQuestNpc = CodeType;
						break;
					case 18:
						Npc->smMonsterInfo.StarPointNpc = CodeType;
						break;
					case 19:
						Npc->smMonsterInfo.GiveMoneyNpc = TRUE;
						break;
					case 20:
						Npc->smMonsterInfo.TelePortNpc = CodeType;
						break;
					case 21:
						Npc->smMonsterInfo.BlessCastleNPC = CodeType;
						break;
					case 22:
						Npc->smMonsterInfo.PollingNpc = CodeType;
						break;
					case 23:
						Npc->smMonsterInfo.OpenCount[0] = CodeType;
						Npc->smMonsterInfo.OpenCount[1] = CodeType;
						break;
					case 24:
						Npc->smMonsterInfo.QuestCode = CodeType;
						Npc->smMonsterInfo.QuestParam = (int)monsterSound[0];
						break;
					case 25:
						Npc->smMonsterInfo.NpcQuest = TRUE;
						break;
					case 26:
						Npc->smCharInfo.wPlayClass[0] = CodeType;
						break;
					case 27:
						Npc->smMonsterInfo.NpcArena = TRUE;
						break;
					case 28:
						Npc->smMonsterInfo.Smelting = TRUE;
						break;
					case 29: // Mestre das forças Eura
						Npc->smMonsterInfo.ItemMix = 200;
						Npc->smCharInfo.wPlayClass[0] = CodeType;
						break;
					case 30: // Npc de ranking
						Npc->smMonsterInfo.NpcRanking = TRUE;
						break;
					case 31: // Npc de mix
						Npc->smMonsterInfo.NpcListaMix = TRUE;
						break;
					case 32: // Novo npc de sod
						Npc->smMonsterInfo.NpcSodRanking = TRUE;
						break;
					}

					if (monsterSound > 0 && CodeType == 0) {
						while (TRUE) {
							if (charSoundConde[cnt].dwCode == 0) break;
							if (lstrcmpi(charSoundConde[cnt].szCodeName, monsterSound) == 0) {
								chrMonsterList[cnt].dwCharSoundCode = charSoundConde[cnt].dwCode;
							}
							cnt++;
						}
						cnt = 0;
					}


					db->Close();

					if (db->Open())
					{
						if (db->Prepare("SELECT * FROM NpcMessage WHERE MessageID=?"))
						{
							db->BindInputParameter(&messageID, 1, PARAMTYPE_Integer);

							if (db->Execute(false))
							{
								while (db->NextRow())
								{
									sprintf_s(Npc->szChatMessage, "Saudações, guerreiro.");

									db->GetData(3, PARAMTYPE_String, message, sizeof(message));
									Npc->smMonsterInfo.lpNpcMessage[Npc->smMonsterInfo.NpcMsgCount++] = SetNPCMsg(message);
								}
							}
						}

						db->Close();
					}

					if (db->Open())
					{
						if (db->Prepare("SELECT * FROM NpcSellList WHERE SellID=?"))
						{
							int sellType = 0;
							db->BindInputParameter(&sellID, 1, PARAMTYPE_Integer);

							if (db->Execute(false))
							{
								while (db->NextRow())
								{

									db->GetData(3, PARAMTYPE_Integer, &sellType);
									db->GetData(4, PARAMTYPE_String, itemList, sizeof(itemList));

									std::vector<std::string> getSellIDS = split(itemList, ' ');

									switch (sellType) {
										// Itens de ataque
									case 1:
										for (size_t x = 0; x < getSellIDS.size(); x++) {

											DWORD ItemCode = findTableItem(getSellIDS[x].c_str());

											if (ItemCode)
											{
												Npc->smMonsterInfo.SellAttackItem[Npc->smMonsterInfo.SellAttackItemCount++] = (void*)ItemCode;
											}

										}
										break;
										// Itens de defesa
									case 2:
										for (size_t x = 0; x < getSellIDS.size(); x++) {

											DWORD ItemCode = findTableItem(getSellIDS[x].c_str());

											if (ItemCode)
											{
												Npc->smMonsterInfo.SellDefenceItem[Npc->smMonsterInfo.SellDefenceItemCount++] = (void*)ItemCode;
											}

										}
										break;
										// Itens premiuns
									case 3:
										for (size_t x = 0; x < getSellIDS.size(); x++) {

											DWORD ItemCode = findTableItem(getSellIDS[x].c_str());

											if (ItemCode)
											{
												Npc->smMonsterInfo.SellEtcItem[Npc->smMonsterInfo.SellEtcItemCount++] = (void*)ItemCode;
											}

										}
										break;
									}
								}
							}

						}

						db->Close();
					}
				}
			}
		}
	}
}

void GameServer::readMonstersFromDB()
{

	auto db = SQLConnection::GetConnection(DATABASEID_GameServer);

	chrMonsterCnt = 0;

	int isBoss = 0;
	int notKillable = 0;
	float monsterSize = 0;
	char race[32] = { 0 };
	char nature[32] = { 0 };
	char monsterSound[32] = { 0 };
	int cnt = 0;

	chrMonsterList[chrMonsterCnt].szModelName2[0] = 0;
	chrMonsterList[chrMonsterCnt].wPlayClass[1] = 0;
	chrMonsterList[chrMonsterCnt].ArrowPosi[0] = 0;
	chrMonsterList[chrMonsterCnt].ArrowPosi[1] = 0;

	if (chrMonInfoList) {
		ZeroMemory(chrMonInfoList, sizeof(smCHAR_MONSTER_INFO));
		chrMonInfoList[chrMonsterCnt].MoveRange = 64 * fONE;
		chrMonInfoList[chrMonsterCnt].FallItemCount = 0;
		chrMonInfoList[chrMonsterCnt].FallItemMax = 0;
		chrMonInfoList[chrMonsterCnt].FallItemPerMax = 0;

		chrMonInfoList[chrMonsterCnt].DamageStunPers = 100;
		chrMonInfoList[chrMonsterCnt].UseEventModel = 0;
	}


	if (db->Open())
	{
		if (db->Prepare("SELECT * FROM MonsterList WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					db->GetData(2, PARAMTYPE_String, chrMonsterList[chrMonsterCnt].szModelName, sizeof(chrMonsterList[chrMonsterCnt].szModelName));
					db->GetData(3, PARAMTYPE_String, chrMonInfoList[chrMonsterCnt].szName, sizeof(chrMonInfoList[chrMonsterCnt].szName));
					db->GetData(4, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Level);
					db->GetData(6, PARAMTYPE_Float, &monsterSize);
					db->GetData(7, PARAMTYPE_Integer, &isBoss);

					chrMonsterList[chrMonsterCnt].wPlayClass[1] = (short)(monsterSize * fONE);
					if (chrMonsterList[chrMonsterCnt].wPlayClass[1] == fONE) chrMonsterList[chrMonsterCnt].wPlayClass[1] = 0;

					sprintf_s(chrMonsterList[chrMonsterCnt].szName, sizeof(chrMonInfoList[chrMonsterCnt].szName), "%s", chrMonInfoList[chrMonsterCnt].szName);

					if (isBoss)
						chrMonsterList[chrMonsterCnt].wPlayClass[0] = MONSTER_CLASS_BOSS;

					db->GetData(8, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].ArrowPosi[0]);
					db->GetData(9, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].ArrowPosi[1]);
					db->GetData(10, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Sight);

					chrMonInfoList[chrMonsterCnt].Real_Sight = chrMonsterList[chrMonsterCnt].Sight;
					chrMonsterList[chrMonsterCnt].Sight *= chrMonsterList[chrMonsterCnt].Sight;

					db->GetData(11, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Life[0]);
					chrMonsterList[chrMonsterCnt].Life[1] = chrMonsterList[chrMonsterCnt].Life[0];

					db->GetData(12, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Attack_Damage[0]);
					db->GetData(13, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Attack_Damage[1]);
					db->GetData(14, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Attack_Rating);
					db->GetData(15, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Attack_Speed);
					chrMonsterList[chrMonsterCnt].Attack_Speed *= fONE;

					db->GetData(16, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Shooting_Range);

					chrMonsterList[chrMonsterCnt].Shooting_Range *= 250;

					db->GetData(17, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Defence);
					db->GetData(18, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Absorption);
					db->GetData(19, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Chance_Block);
					db->GetData(20, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Move_Speed);
					chrMonsterList[chrMonsterCnt].Move_Speed = ConvMoveSpeed(float(chrMonsterList[chrMonsterCnt].Move_Speed));

					db->GetData(21, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Resistance[sITEMINFO_BIONIC]);
					db->GetData(22, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Resistance[sITEMINFO_ICE]);
					db->GetData(23, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Resistance[sITEMINFO_FIRE]);
					db->GetData(24, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Resistance[sITEMINFO_POISON]);
					db->GetData(25, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Resistance[sITEMINFO_LIGHTING]);
					db->GetData(26, PARAMTYPE_String, race, sizeof(race));

					chrMonsterList[chrMonsterCnt].Brood = smCHAR_MONSTER_NORMAL;

					if (lstrcmp(race, "UNDEAD") == 0) {
						chrMonsterList[chrMonsterCnt].Brood = smCHAR_MONSTER_UNDEAD;
					}
					else if (lstrcmp(race, "MUTANT") == 0) {
						chrMonsterList[chrMonsterCnt].Brood = smCHAR_MONSTER_MUTANT;
					}
					else if (lstrcmp(race, "DEMON") == 0) {
						chrMonsterList[chrMonsterCnt].Brood = smCHAR_MONSTER_DEMON;
					}
					else if (lstrcmp(race, "MECHANIC") == 0) {
						chrMonsterList[chrMonsterCnt].Brood = smCHAR_MONSTER_MECHANIC;
					}

					db->GetData(27, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].SizeLevel);
					db->GetData(28, PARAMTYPE_Integer, &chrMonsterList[chrMonsterCnt].Exp);
					chrMonInfoList[chrMonsterCnt].GetExp = (INT64)chrMonsterList[chrMonsterCnt].Exp;

					db->GetData(29, PARAMTYPE_String, monsterSound, sizeof(monsterSound));

					while (TRUE) {
						if (charSoundConde[cnt].dwCode == 0) break;
						if (lstrcmpi(charSoundConde[cnt].szCodeName, monsterSound) == 0) {
							chrMonsterList[chrMonsterCnt].dwCharSoundCode = charSoundConde[cnt].dwCode;
						}
						cnt++;
					}

					cnt = 0;

					db->GetData(30, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].EventCode);
					db->GetData(31, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].EventInfo);
					db->GetData(32, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SkillDamage[0]);
					db->GetData(33, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SkillDamage[1]);
					db->GetData(34, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SkillRating);
					db->GetData(35, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SkillRange);
					db->GetData(36, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SkillDistance);
					db->GetData(37, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].DamageStunPers);
					db->GetData(38, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SkillCurse);

					db->GetData(39, PARAMTYPE_String, nature, sizeof(nature));

					chrMonInfoList[chrMonsterCnt].Nature = smCHAR_MONSTER_NATURAL;

					if (lstrcmpi(nature, "GOOD") == 0) {
						chrMonInfoList[chrMonsterCnt].Nature = smCHAR_MONSTER_GOOD;
					}
					if (lstrcmpi(nature, "EVIL") == 0) {
						chrMonInfoList[chrMonsterCnt].Nature = smCHAR_MONSTER_EVIL;
					}

					db->GetData(40, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].ClassCode);
					db->GetData(41, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].IQ);
					db->GetData(42, PARAMTYPE_Integer, &notKillable);

					// Quantidade de Potions do mob e chance de potar
					db->GetData(43, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].PotionCount);
					db->GetData(44, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].PotionPercent);

					db->GetData(45, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].SpAttackPercetage);
					db->GetData(46, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].GenerateGroup[0]);
					db->GetData(47, PARAMTYPE_Integer, &chrMonInfoList[chrMonsterCnt].GenerateGroup[1]);

					if (notKillable)
						chrMonsterList[chrMonsterCnt].State = FALSE;
					else
						chrMonsterList[chrMonsterCnt].State = TRUE;

					chrMonsterCnt++;

					if (chrMonsterCnt >= MAX_MONSTERS) break;
				}

			}
		}

		db->Close();

		for (int monsterPos = 0; monsterPos < chrMonsterCnt; monsterPos++) {
			setDropItems(&chrMonInfoList[monsterPos], monsterPos);
		}
	}

}

void GameServer::readItemsFromDB() {

	auto db = SQLConnection::GetConnection(DATABASEID_GameServer);
	char itemName[10] = { 0 };
	char secondarySpec[20] = { 0 };
	short primarySpec = 0, expireTime = 0;
	int itemEffects[7] = { 0 };
	int questID = 0;

	DefaultItemCount = 0;

	if (db->Open())
	{
		///////////////////////////////////////////////////////////////
		///////////////////////// ARMAS ///////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Weapons WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(itemEffects, sizeof(itemEffects));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, 10);

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[1]);
					db->GetData(15, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[2]);
					db->GetData(16, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[3]);
					db->GetData(17, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[0]);
					db->GetData(18, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[1]);
					db->GetData(19, PARAMTYPE_Short, &lpItem->Critical_Hit);
					db->GetData(20, PARAMTYPE_Short, &lpItem->Attack_Speed);
					db->GetData(21, PARAMTYPE_Short, &lpItem->Shooting_Range);
					db->GetData(22, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(23, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);
					db->GetData(24, PARAMTYPE_Short, &primarySpec);
					db->GetData(25, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(26, PARAMTYPE_Short, &lpItem->JobItem.Lev_Damage[0]);
					db->GetData(27, PARAMTYPE_Short, &lpItem->JobItem.Lev_Damage[1]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Lev_Attack_Rating[0]);
					db->GetData(29, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Lev_Attack_Rating[1]);
					db->GetData(30, PARAMTYPE_Short, &lpItem->JobItem.Add_Attack_Speed);
					db->GetData(31, PARAMTYPE_Short, &lpItem->JobItem.Add_Critical_Hit);
					db->GetData(32, PARAMTYPE_Short, &lpItem->JobItem.Add_Shooting_Range);
					db->GetData(33, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(34, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(35, PARAMTYPE_Short, &itemEffects[0]);
					db->GetData(36, PARAMTYPE_Short, &itemEffects[1]);
					db->GetData(37, PARAMTYPE_Short, &itemEffects[2]);
					db->GetData(38, PARAMTYPE_Short, &itemEffects[3]);
					db->GetData(39, PARAMTYPE_Short, &itemEffects[4]);
					db->GetData(40, PARAMTYPE_Short, &itemEffects[5]);
					db->GetData(41, PARAMTYPE_Short, &itemEffects[6]);

					// Defesa das garras e.e
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);

					db->GetData(44, PARAMTYPE_Short, &questID);
					db->GetData(45, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					if (itemEffects[0] == 1)
						lpItem->DispEffect = TRUE;

					if (itemEffects[1] > 0) {
						lpItem->EffectColor[0] = itemEffects[1];
						lpItem->EffectColor[1] = itemEffects[2];
						lpItem->EffectColor[2] = itemEffects[3];
						lpItem->EffectColor[3] = itemEffects[4];
						lpItem->EffectBlink[0] = itemEffects[5];

						if (lpItem->EffectBlink[0]) {
							lpItem->EffectBlink[1] =
								(lpItem->EffectColor[0] + lpItem->EffectColor[1] + lpItem->EffectColor[2] + lpItem->EffectColor[3] + (short)lpItem->DispEffect);
						}

						if (itemEffects[6])
							lpItem->ScaleBlink[0] = (int)(itemEffects[6]) * 200;
					}


					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////// ARMAS MÁGICAS ///////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM MagicWeapons WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(itemEffects, sizeof(itemEffects));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[1]);
					db->GetData(15, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[2]);
					db->GetData(16, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[3]);
					db->GetData(17, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[0]);
					db->GetData(18, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[1]);
					db->GetData(19, PARAMTYPE_Short, &lpItem->Critical_Hit);
					db->GetData(20, PARAMTYPE_Short, &lpItem->Attack_Speed);
					db->GetData(21, PARAMTYPE_Short, &lpItem->Shooting_Range);
					db->GetData(22, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(23, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);
					db->GetData(24, PARAMTYPE_Short, &primarySpec);
					db->GetData(25, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(26, PARAMTYPE_Short, &lpItem->JobItem.Lev_Damage[0]);
					db->GetData(27, PARAMTYPE_Short, &lpItem->JobItem.Lev_Damage[1]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Lev_Attack_Rating[0]);
					db->GetData(29, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Lev_Attack_Rating[1]);
					db->GetData(30, PARAMTYPE_Short, &lpItem->JobItem.Add_Attack_Speed);
					db->GetData(31, PARAMTYPE_Short, &lpItem->JobItem.Add_Critical_Hit);
					db->GetData(32, PARAMTYPE_Short, &lpItem->JobItem.Add_Shooting_Range);
					db->GetData(33, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(34, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(35, PARAMTYPE_Short, &itemEffects[0]);
					db->GetData(36, PARAMTYPE_Short, &itemEffects[1]);
					db->GetData(37, PARAMTYPE_Short, &itemEffects[2]);
					db->GetData(38, PARAMTYPE_Short, &itemEffects[3]);
					db->GetData(39, PARAMTYPE_Short, &itemEffects[4]);
					db->GetData(40, PARAMTYPE_Short, &itemEffects[5]);
					db->GetData(41, PARAMTYPE_Short, &itemEffects[6]);

					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);

					db->GetData(44, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Magic_Mastery[0]);
					db->GetData(45, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Magic_Mastery[1]);
					db->GetData(46, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(47, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);
					db->GetData(48, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(49, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(50, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(51, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(52, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(53, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(54, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[0]);
					db->GetData(55, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[1]);
					db->GetData(56, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(57, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);

					db->GetData(58, PARAMTYPE_Short, &questID);
					db->GetData(59, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					if (itemEffects[0] == 1)
						lpItem->DispEffect = TRUE;

					if (itemEffects[1] > 0) {
						lpItem->EffectColor[0] = itemEffects[1];
						lpItem->EffectColor[1] = itemEffects[2];
						lpItem->EffectColor[2] = itemEffects[3];
						lpItem->EffectColor[3] = itemEffects[4];
						lpItem->EffectBlink[0] = itemEffects[5];

						if (lpItem->EffectBlink[0]) {
							lpItem->EffectBlink[1] =
								(lpItem->EffectColor[0] + lpItem->EffectColor[1] + lpItem->EffectColor[2] + lpItem->EffectColor[3] + (short)lpItem->DispEffect);
						}

						if (itemEffects[6])
							lpItem->ScaleBlink[0] = (int)(itemEffects[6]) * fONE;
					}


					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		/////////////////////// ARMADURAS /////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Armor WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(29, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(32, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(33, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(34, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(35, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(36, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(37, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(38, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(43, PARAMTYPE_Short, &questID);
					db->GetData(44, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////// ARMADURAS TEMPORÁRIAS ///////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM ArmorT WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(29, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(32, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(33, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(34, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(35, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(36, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(37, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(38, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(43, PARAMTYPE_Short, &questID);
					db->GetData(44, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////// ROBES ///////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Robes WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(29, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(32, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(33, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(34, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(35, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(36, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(37, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(38, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);

					db->GetData(43, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(44, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);

					db->GetData(45, PARAMTYPE_Short, &questID);
					db->GetData(46, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		//////////////////////// ESCUDOS //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Shields WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(29, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(32, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(33, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(34, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(35, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(36, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(37, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(38, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);

					db->GetData(43, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(44, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);
					db->GetData(45, PARAMTYPE_Float, &lpItem->JobItem.Per_Life_Regen);
					db->GetData(46, PARAMTYPE_Float, &lpItem->JobItem.Per_Stamina_Regen);

					db->GetData(47, PARAMTYPE_Short, &questID);
					db->GetData(48, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}


		///////////////////////////////////////////////////////////////
		///////////////////////// COLARES /////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Amulets WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);

					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);

					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);

					db->GetData(11, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(12, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(13, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[0]);
					db->GetData(14, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[1]);

					db->GetData(15, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(16, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(21, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[0]);
					db->GetData(22, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[1]);


					db->GetData(23, PARAMTYPE_Float, &lpItem->JobItem.Per_Stamina_Regen);
					db->GetData(24, PARAMTYPE_Float, &lpItem->JobItem.Per_Life_Regen);
					db->GetData(25, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(26, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);

					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(29, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(30, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);

					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[0]);
					db->GetData(32, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[1]);
					db->GetData(33, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[2]);
					db->GetData(34, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[3]);
					db->GetData(35, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[0]);
					db->GetData(36, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[1]);
					db->GetData(37, PARAMTYPE_Short, &lpItem->Critical_Hit);
					db->GetData(38, PARAMTYPE_Short, &lpItem->Attack_Speed);
					db->GetData(39, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(40, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(44, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(45, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(46, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(47, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(48, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(49, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(50, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(51, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(52, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(53, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(54, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(55, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(56, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(57, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(58, PARAMTYPE_Integer, &lpItem->Price);
					db->GetData(59, PARAMTYPE_Short, &questID);
					db->GetData(60, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}


		///////////////////////////////////////////////////////////////
		////////////////////////// ANÉIS //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Rings WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;


					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));


					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));

					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);

					db->GetData(11, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(12, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(13, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[0]);
					db->GetData(14, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[1]);

					db->GetData(15, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(16, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(21, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[0]);
					db->GetData(22, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[1]);


					db->GetData(23, PARAMTYPE_Float, &lpItem->JobItem.Per_Stamina_Regen);
					db->GetData(24, PARAMTYPE_Float, &lpItem->JobItem.Per_Life_Regen);
					db->GetData(25, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(26, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);

					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(29, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(30, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);

					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[0]);
					db->GetData(32, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[1]);
					db->GetData(33, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[2]);
					db->GetData(34, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[3]);
					db->GetData(35, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[0]);
					db->GetData(36, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[1]);
					db->GetData(37, PARAMTYPE_Short, &lpItem->Critical_Hit);
					db->GetData(38, PARAMTYPE_Short, &lpItem->Attack_Speed);
					db->GetData(39, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(40, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(44, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(45, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(46, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(47, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(48, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(49, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(50, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(51, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(52, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(53, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(54, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(55, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(56, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(57, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(58, PARAMTYPE_Integer, &lpItem->Price);
					db->GetData(59, PARAMTYPE_Short, &questID);
					db->GetData(60, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		////////////////////////// BOTAS //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Boots WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);
					db->GetData(19, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpeed[0]);
					db->GetData(20, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpeed[1]);


					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(29, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(31, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(32, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(33, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(34, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(35, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(36, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(37, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_fSpeed[0]);
					db->GetData(38, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_fSpeed[1]);
					db->GetData(39, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(40, PARAMTYPE_Integer, &lpItem->Price);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(44, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(45, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(46, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(47, PARAMTYPE_Short, &questID);
					db->GetData(48, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////// LUVAS ///////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Gloves WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(29, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(32, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(33, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(34, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(35, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(36, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(37, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(38, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(43, PARAMTYPE_Short, &questID);
					db->GetData(44, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		//////////////////////// BRACELETES ///////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Bracelets WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &lpItem->Potion_Space);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(15, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(18, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[0]);
					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[1]);
					db->GetData(20, PARAMTYPE_Short, &lpItem->Critical_Hit);
					db->GetData(21, PARAMTYPE_Short, &lpItem->Attack_Speed);
					db->GetData(22, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(23, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);
					db->GetData(24, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(27, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(28, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(29, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Lev_Attack_Rating[0]);
					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Lev_Attack_Rating[1]);
					db->GetData(31, PARAMTYPE_Short, &lpItem->JobItem.Add_Attack_Speed);
					db->GetData(32, PARAMTYPE_Short, &lpItem->JobItem.Add_Critical_Hit);
					db->GetData(33, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(34, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(35, PARAMTYPE_Integer, &lpItem->Price);
					db->GetData(36, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(37, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(38, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(42, PARAMTYPE_Short, &questID);
					db->GetData(43, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		//////////////////////// COSTUMES /////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Costumes WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Durability[0]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Durability[1]);
					db->GetData(13, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(14, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);
					db->GetData(19, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpeed[0]);
					db->GetData(20, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpeed[1]);

					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(26, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(29, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(30, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(31, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(32, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(33, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(34, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(35, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(36, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(37, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(38, PARAMTYPE_Integer, &lpItem->Price);

					db->GetData(39, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(40, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(44, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(45, PARAMTYPE_Short, &questID);
					db->GetData(46, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}


		///////////////////////////////////////////////////////////////
		/////////////////////// SHELTONS //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Sheltoms WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(9, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(10, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(11, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);
					db->GetData(12, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(13, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(14, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(15, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(16, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);

					db->GetData(17, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);

					db->GetData(18, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);

					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(21, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(22, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(23, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(24, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(25, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);

					db->GetData(26, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[0]);
					db->GetData(27, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[1]);
					db->GetData(28, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[0]);
					db->GetData(29, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[1]);
					db->GetData(30, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[0]);
					db->GetData(31, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[1]);

					db->GetData(32, PARAMTYPE_Float, &lpItem->JobItem.Per_Life_Regen);
					db->GetData(33, PARAMTYPE_Float, &lpItem->JobItem.Per_Stamina_Regen);
					db->GetData(34, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(35, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);
					db->GetData(36, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(37, PARAMTYPE_Integer, &lpItem->Price);
					db->GetData(38, PARAMTYPE_Short, &questID);
					db->GetData(39, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		//////////////////////// CRISTAIS /////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Crystals WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(8, PARAMTYPE_Integer, &lpItem->Price);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////// FORCES //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Forces WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(8, PARAMTYPE_Integer, &lpItem->Price);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////// BRINCOS /////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Brincos WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));
					ZeroMemory(secondarySpec, sizeof(secondarySpec));
					primarySpec = 0;
					questID = 0;
					expireTime = 0;

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Level);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Spirit);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Strength);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Talent);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Dexterity);

					db->GetData(11, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[0]);
					db->GetData(12, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Life[1]);
					db->GetData(13, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[0]);
					db->GetData(14, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fLife_Regen[1]);

					db->GetData(15, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[0]);
					db->GetData(16, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Mana[1]);
					db->GetData(17, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[0]);
					db->GetData(18, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fMana_Regen[1]);

					db->GetData(19, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[0]);
					db->GetData(20, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].Increase_Stamina[1]);
					db->GetData(21, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[0]);
					db->GetData(22, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fStamina_Regen[1]);


					db->GetData(23, PARAMTYPE_Float, &lpItem->JobItem.Per_Stamina_Regen);
					db->GetData(24, PARAMTYPE_Float, &lpItem->JobItem.Per_Life_Regen);
					db->GetData(25, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[0]);
					db->GetData(26, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Mana_Regen[1]);

					db->GetData(27, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[0]);
					db->GetData(28, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDefence[1]);
					db->GetData(29, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[0]);
					db->GetData(30, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fAbsorb[1]);

					db->GetData(31, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[0]);
					db->GetData(32, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[1]);
					db->GetData(33, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[2]);
					db->GetData(34, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sDamage[3]);
					db->GetData(35, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[0]);
					db->GetData(36, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sAttack_Rating[1]);
					db->GetData(37, PARAMTYPE_Short, &lpItem->Critical_Hit);
					db->GetData(38, PARAMTYPE_Short, &lpItem->Attack_Speed);
					db->GetData(39, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[0]);
					db->GetData(40, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fBlock_Rating[1]);

					db->GetData(41, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][0]);
					db->GetData(42, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_BIONIC][1]);
					db->GetData(43, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][0]);
					db->GetData(44, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_FIRE][1]);
					db->GetData(45, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][0]);
					db->GetData(46, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_ICE][1]);
					db->GetData(47, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][0]);
					db->GetData(48, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_LIGHTING][1]);
					db->GetData(49, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][0]);
					db->GetData(50, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sResistance[sITEMINFO_POISON][1]);

					db->GetData(51, PARAMTYPE_String, secondarySpec, sizeof(secondarySpec));

					setSpec(primarySpec, secondarySpec);

					db->GetData(52, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[0]);
					db->GetData(53, PARAMTYPE_Short, &DefaultItems[DefaultItemCount].sSpecial_Defence[1]);
					db->GetData(54, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[0]);
					db->GetData(55, PARAMTYPE_Float, &DefaultItems[DefaultItemCount].fSpecial_Absorb[1]);
					db->GetData(56, PARAMTYPE_Float, &lpItem->JobItem.Add_fBlock_Rating);
					db->GetData(57, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(58, PARAMTYPE_Integer, &lpItem->Price);
					db->GetData(59, PARAMTYPE_Short, &questID);
					db->GetData(60, PARAMTYPE_Short, &expireTime);

					setExpireTime(expireTime);
					setQuestID(questID);

					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////// POÇÕES //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Potions WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);

					db->GetData(6, PARAMTYPE_Short, &lpItem->Mana[0]);
					db->GetData(7, PARAMTYPE_Short, &lpItem->Mana[1]);
					db->GetData(8, PARAMTYPE_Short, &lpItem->Life[0]);
					db->GetData(9, PARAMTYPE_Short, &lpItem->Life[1]);
					db->GetData(10, PARAMTYPE_Short, &lpItem->Stamina[0]);
					db->GetData(11, PARAMTYPE_Short, &lpItem->Stamina[1]);
					db->GetData(12, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(13, PARAMTYPE_Integer, &lpItem->Price);


					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		//////////////////////// PREMIUNS /////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Premiuns WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(7, PARAMTYPE_Integer, &lpItem->Price);


					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		////////////////////////// CRAFT //////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM Craft WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));

					db->GetData(2, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(3, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(4, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(7, PARAMTYPE_Integer, &lpItem->Price);


					int result = addItem();
					if (!result) break;
				}
			}
		}

		///////////////////////////////////////////////////////////////
		///////////////////////// QUEST ///////////////////////////////
		///////////////////////////////////////////////////////////////

		if (db->Prepare("SELECT * FROM QuestItems WHERE Active=1"))
		{
			if (db->Execute(false))
			{
				while (db->NextRow())
				{
					lpDefItem = &DefaultItems[DefaultItemCount];
					lpItem = &lpDefItem->Item;

					ZeroMemory(lpDefItem, sizeof(sDEF_ITEMINFO));

					db->GetData(1, PARAMTYPE_String, itemName, sizeof(itemName));

					setItemCode(itemName);

					db->GetData(2, PARAMTYPE_String, lpItem->ItemName, sizeof(lpItem->ItemName));
					db->GetData(3, PARAMTYPE_Short, &lpItem->itemType);
					db->GetData(6, PARAMTYPE_Short, &lpItem->Weight);
					db->GetData(7, PARAMTYPE_Integer, &lpItem->Price);


					int result = addItem();
					if (!result) break;
				}
			}
		}


		db->Close();
	}

}

int GameServer::readFieldNpc(smTRNAS_PLAYERINFO* Npc, char* szFile, int Pos)
{
	json j;
	Pos += 1;

	ifstream obj(szFile);

	if (obj) {
		std::ifstream i(szFile);
		i >> j;
	}
	else {
		return FALSE;
	}

	for (auto& it : j.items()) {
		json& val = it.value();

		if (val["id"] == Pos) {
			Npc->smCharInfo.Life[0] = 100;
			Npc->smCharInfo.Life[1] = 100;
			Npc->x = val["x"] * 256;
			Npc->y = val["y"] * 256;
			Npc->z = val["z"] * 256;
			Npc->ay = val["angle"];
			Npc->code = Pos;

			return TRUE;
		}
	}

	return FALSE;
}

#define IM_ARRAYSIZE(_ARR)          ((int)(sizeof(_ARR) / sizeof(*_ARR))) 


int GameServer::readFieldMob(rsSTG_MONSTER_LIST* StgMonster, smCHAR_INFO* StgCharInfo, int ActiveMonsters, char* szFile)
{
	sBOSS_MONSTER* BossField;
	int monsterCount = 0, PercentMonster = 0;
	std::string openTime;
	std::string name;

	ZeroMemory(StgMonster, sizeof(rsSTG_MONSTER) * rsSTG_MONSTER_MAX);

	// Intervalo dos mobs e limite padrão
	StgMonster->OpenInterval = 0x7F;
	StgMonster->OpenLimit = 3;

	json j;

	ifstream obj(szFile);

	if (obj)
	{
		std::ifstream i(szFile);
		i >> j;
	}
	else
	{
		return FALSE;
	}

	int x = 1;

	for (json::iterator it = j.begin(); it != j.end(); ++it)
	{
		//for (auto& it : j.items()) 

		json& val = it.value();
		char monsterChild[32] = "Monster";

		sprintf_s(monsterChild, sizeof(monsterChild), "Monster%d", x);

		if (lstrcmp(it.key().c_str(), "Settings") == 0)
		{
			StgMonster->LimitMax = val["Max_Monsters"];
			StgMonster->OpenInterval = val["Min_SpawnTime"];
			StgMonster->dwIntervalTime = val["Max_SpawnTime"] * 1000;
			StgMonster->OpenLimit = val["Monsters_Per_Flag"];
		}

		if (lstrcmp(it.key().c_str(), monsterChild) == 0)
		{
			for (int cnt = 0; cnt < ActiveMonsters; cnt++)
			{
				val.at("Name").get_to(name);
				wchar_t utf8Message[256];
				MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, utf8Message, IM_ARRAYSIZE(utf8Message));
				string monsterName = FormatString("%ls", utf8Message);

				if (boost::iequals(monsterName.c_str(), StgCharInfo[cnt].szName))
				{
					StgMonster->rsMonster[monsterCount].lpCharInfo = &StgCharInfo[cnt];

					cnt = val["Percentage"];

					StgMonster->rsMonster[monsterCount].NumOpenStart = PercentMonster;
					StgMonster->rsMonster[monsterCount].OpenPercentage = cnt;
					PercentMonster += cnt;
					monsterCount++;
					name.clear();
					x++;

					break;
				}
			}
		}


		if (lstrcmp(it.key().c_str(), "Boss") == 0)
		{
			BossField = &StgMonster->sBossMonsters[StgMonster->BossMonsterCount];

			for (int cnt = 0; cnt < ActiveMonsters; cnt++)
			{
				val.at("Name").get_to(name);
				wchar_t utf8Message[256];
				MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, utf8Message, IM_ARRAYSIZE(utf8Message));
				string monsterName = FormatString("%ls", utf8Message);

				if (boost::iequals(monsterName.c_str(), StgCharInfo[cnt].szName))
				{
					BossField->MasterMonster = &StgCharInfo[cnt];
					val.at("OpenTime").get_to(name);

					openTime = name;

					std::vector<std::string> OpenTime = split(openTime, ' ');

					for (size_t x = 0; x < OpenTime.size(); x++)
					{
						BossField->bOpenTime[BossField->OpenTimeCount++] = atoi(OpenTime[x].c_str());
					}

					StgMonster->BossMonsterCount++;
				}
			}

			for (int cnt = 0; cnt < ActiveMonsters; cnt++)
			{
				val.at("Slave").get_to(name);
				wchar_t utf8Message[256];
				MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, utf8Message, IM_ARRAYSIZE(utf8Message));
				string monsterName = FormatString("%ls", utf8Message);

				if (boost::iequals(monsterName.c_str(), StgCharInfo[cnt].szName))
				{
					BossField->SlaveMonster = &StgCharInfo[cnt];
					BossField->SlaveCount = val["SlaveCount"];
				}
			}
		}

	}

	StgMonster->Counter = monsterCount;
	StgMonster->PecetageCount = PercentMonster;


	return TRUE;
}