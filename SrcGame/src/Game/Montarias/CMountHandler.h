#pragma once

#include "smLib3d/smType.h"
#include "smLib3d/smd3d.h"

class smCHAR;
class CMountHandler;

enum EMountID
{
    MOUNTID_None,
    MOUNTID_Invisible,
    MOUNTID_HopyYellow,
    MOUNTID_HopyBrown,
    MOUNTID_HopyGreen,
    MOUNTID_HopyPink,
    MOUNTID_Mtt,
    MOUNTID_Chicken,
    MOUNTID_FestivalMonster,
    MOUNTID_Horse,
    MOUNTID_IceMonster,
    MOUNTID_IceTiger,
    MOUNTID_LL_BlackWolf,
    MOUNTID_LL_WhiteWolf,
    MOUNTID_Piggy,
    MOUNTID_Rabbie,
    MOUNTID_SL_BlackWolf,
    MOUNTID_SL_WhiteWolf,
    MOUNTID_Snowdeer,
    MOUNTID_SteamHorse,
    MOUNTID_SteamAircraft,
    MOUNTID_SteamMotoBike,
    MOUNTID_Turtle
};

struct MountModel
{
    std::string fileName;
    class smPAT3D* bone;
    class smPAT3D* model;
    std::pair<int, int> frameIdle;
    std::pair<int, int> frameRun;
};

class CMountManager
{
public:
    CMountManager();
    ~CMountManager();

    void Load();

    CMountHandler* GetMountHandler(smCHAR* pCharacter, bool myCharacter = false);
    void AddMount(smCHAR* pCharacter, const EMountID& id, bool myCharacter = false, bool bVisible = true);
    void DelMount(smCHAR* pCharacter, bool myCharacter = false);

    MountModel* GetMountModel(const EMountID& id);
private:
    std::vector<CMountHandler*> vMounts;
    std::unordered_map<int, MountModel> vCachedMounts;
};

extern CMountManager cMountManager;

class CMountHandler
{
public:
    CMountHandler();
    ~CMountHandler();

    smCHAR* GetAttachCharacter() { return pAttachCharacter; }

    void SetIsMyCharacter(bool b) { myCharacter = b; }
    bool IsMyCharacter() { return myCharacter; }

    void SetCurrentFrame(int motion, int frame);
    int GetCurrentFrame() { return iCurrentFrame; }

    bool CanUse();

    void SetVisible(bool b, bool spawnParticles = true);
    bool IsVisible() { return visible; }

    void SetMount(const EMountID& id);
    EMountID GetMountID() { return eMountID; }

    void SetAttachCharacter(smCHAR* pCharacter);

    void Update(float fTime);
    Math::Vector3Int Render();

    float GetHeight() { return pMountModel->model->newModel->size.y; }
public:
    int iBaseFrame;
    bool visible;
    EMountID eMountID;
    int iCurrentFrame;
    bool myCharacter;
    smCHAR* pAttachCharacter;
    MountModel* pMountModel;
    bool updateBoundingBox;
};

