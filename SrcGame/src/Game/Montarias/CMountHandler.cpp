#include "../sinbaram/sinLinkHeader.h"
#include "CMountHandler.h"

#include "character.h"

extern smCHAR* lpCurPlayer;
extern smPAT3D* smASE_ReadBone(char* file);
extern void smASE_SetPhysique(smPAT3D* p);
extern smPAT3D* smASE_Read(char* file, char* szModelName);

int backupHeight = 0;

CMountManager cMountManager;

CMountManager::CMountManager()
{
    vCachedMounts[MOUNTID_HopyYellow].fileName = "char\\mount\\hopy\\hopy.ase";
    vCachedMounts[MOUNTID_HopyBrown].fileName = "char\\mount\\hopy\\hopy_brown.ase";
    vCachedMounts[MOUNTID_HopyGreen].fileName = "char\\mount\\hopy\\hopy_green.ase";
    vCachedMounts[MOUNTID_HopyPink].fileName = "char\\mount\\hopy\\hopy_pink.ase";
    vCachedMounts[MOUNTID_Mtt].fileName = "char\\mount\\mtt\\mtt.ase";
    vCachedMounts[MOUNTID_Chicken].fileName = "char\\mount\\chicken\\chicken.ase";
    vCachedMounts[MOUNTID_FestivalMonster].fileName = "char\\mount\\festivalmonster\\festivalmonster.ase";
    vCachedMounts[MOUNTID_Horse].fileName = "char\\mount\\horse\\horse.ase";
    vCachedMounts[MOUNTID_IceMonster].fileName = "char\\mount\\icemonster\\icemonster.ase";
    vCachedMounts[MOUNTID_IceTiger].fileName = "char\\mount\\icetiger\\icetiger.ase";
    vCachedMounts[MOUNTID_LL_BlackWolf].fileName = "char\\mount\\wolf\\ll_blackwolf.ase";
    vCachedMounts[MOUNTID_LL_WhiteWolf].fileName = "char\\mount\\wolf\\ll_whitewolf.ase";
    vCachedMounts[MOUNTID_Piggy].fileName = "char\\mount\\piggy\\piggy.ase";
    vCachedMounts[MOUNTID_Rabbie].fileName = "char\\mount\\rabbie\\rabbie.ase";
    vCachedMounts[MOUNTID_SL_BlackWolf].fileName = "char\\mount\\wolf\\sl_blackwolf.ase";
    vCachedMounts[MOUNTID_SL_WhiteWolf].fileName = "char\\mount\\wolf\\sl_whitewolf.ase";
    vCachedMounts[MOUNTID_Snowdeer].fileName = "char\\mount\\snowdeer\\snowdeer.ase";
    vCachedMounts[MOUNTID_SteamHorse].fileName = "char\\mount\\steam\\horse.ase";
    vCachedMounts[MOUNTID_SteamAircraft].fileName = "char\\mount\\steam\\aircraft.ase";
    vCachedMounts[MOUNTID_SteamMotoBike].fileName = "char\\mount\\steam\\motobike.ase";
    vCachedMounts[MOUNTID_Turtle].fileName = "char\\mount\\turtle\\turtle.ase";
}

CMountManager::~CMountManager()
{
}

void CMountManager::Load()
{
    for (auto& mount : vCachedMounts)
    {
        smRender.LoadUsingNewRender = true;
        mount.second.bone = smASE_ReadBone((char*)mount.second.fileName.c_str());
        smRender.LoadUsingNewRender = false;

        if (mount.second.bone)
        {
            smASE_SetPhysique(mount.second.bone);

            smRender.LoadUsingNewRender = true;
            mount.second.model = smASE_Read((char*)mount.second.fileName.c_str(), nullptr);
            smRender.LoadUsingNewRender = false;
        }

        //Mount Animation Frame
        if (mount.first == MOUNTID_Rabbie)
        {
            mount.second.frameIdle = std::pair(17, 106);
            mount.second.frameRun = std::pair(0, 15);
        }
        else if (mount.first == MOUNTID_Piggy)
        {
            mount.second.frameIdle = std::pair(17, 106);
            mount.second.frameRun = std::pair(0, 15);
        }
    }
}

CMountHandler* CMountManager::GetMountHandler(smCHAR* pCharacter, bool myCharacter)
{
    for (auto pMount : vMounts)
    {
        if ((myCharacter == true) && pMount->IsMyCharacter())
            return pMount;
        else if (pMount->GetAttachCharacter() == pCharacter)
            return pMount;
    }

    return nullptr;
}

void CMountManager::AddMount(smCHAR* pCharacter, const EMountID& id, bool myCharacter, bool bVisible)
{
    if (id < MOUNTID_HopyYellow)
        return;

    auto pMountFound = GetMountHandler(pCharacter, myCharacter);
    if (pMountFound == nullptr)
    {
        CMountHandler* pcMountHandler = new CMountHandler();
        pcMountHandler->SetMount(id);
        pcMountHandler->SetIsMyCharacter(myCharacter);
        pcMountHandler->SetAttachCharacter(pCharacter);
        pcMountHandler->SetVisible(bVisible, false);
        pcMountHandler->SetCurrentFrame(CHRMOTION_STATE_STAND, pCharacter->frame);
        pcMountHandler->Render();
        vMounts.push_back(pcMountHandler);

        pCharacter->pMount = pcMountHandler;
    }
    else
    {
        bool newMount = pMountFound->GetMountID() != id;
        bool stateHasChanged = newMount || (pMountFound->IsVisible() != bVisible);

        if (newMount)
        {
            pMountFound->SetMount(id);
            pMountFound->SetAttachCharacter(pCharacter);
        }

        if (stateHasChanged)
        {
            pMountFound->SetVisible(bVisible);
        }

        if (newMount)
        {
            pCharacter->SetMotionFromCode(CHRMOTION_STATE_STAND);
            pCharacter->ChangeMotion(0);
        }
    }
}

void CMountManager::DelMount(smCHAR* pCharacter, bool myCharacter)
{
    for (auto it = vMounts.begin(); it != vMounts.end(); )
    {
        auto pMountHandler = (*it);

        if (myCharacter == true && pMountHandler->IsMyCharacter())
        {
            if (pMountHandler->IsVisible())
                pMountHandler->SetVisible(false);

            delete pMountHandler;

            pCharacter->pMount = nullptr;
            it = vMounts.erase(it);
            return;
        }
        else if (pMountHandler->GetAttachCharacter() == pCharacter)
        {
            if (pMountHandler->IsVisible())
                pMountHandler->SetVisible(false);

            delete pMountHandler;

            pCharacter->pMount = nullptr;
            it = vMounts.erase(it);
            return;
        }

        ++it;
    }
}

MountModel* CMountManager::GetMountModel(const EMountID& id)
{
    auto it = vCachedMounts.find(id);

    if (it != vCachedMounts.end())
        return &it->second;

    return nullptr;
}

CMountHandler::CMountHandler() : pMountModel(nullptr), eMountID(MOUNTID_None), pAttachCharacter(nullptr), myCharacter(false), iCurrentFrame(0), visible(false), updateBoundingBox(false)
{
}

CMountHandler::~CMountHandler()
{
}

void CMountHandler::SetCurrentFrame(int motion, int frame)
{
    int iNewFrame = frame - (iBaseFrame * 160);

    if (iNewFrame > 0 && iNewFrame <= pMountModel->model->TmParent->MaxFrame)
    {
        if (pMountModel->frameRun.second && motion == CHRMOTION_STATE_RUN)
        {
            if (iNewFrame < pMountModel->frameRun.first * 160)
                iNewFrame = pMountModel->frameRun.first * 160;
            else if (iNewFrame > pMountModel->frameRun.second * 160)
                iNewFrame = pMountModel->frameRun.second * 160;
        }
        else if (pMountModel->frameIdle.second && motion == CHRMOTION_STATE_STAND)
        {
            if (iNewFrame < pMountModel->frameIdle.first * 160)
                iNewFrame = pMountModel->frameIdle.first * 160;
            else if (iNewFrame > pMountModel->frameIdle.second * 160)
                iNewFrame = pMountModel->frameIdle.second * 160;
        }

        iCurrentFrame = iNewFrame;
    }
}

bool CMountHandler::CanUse()
{
    return true;
}

void CMountHandler::SetVisible(bool b, bool spawnParticles)
{
    if (visible == false && !CanUse())
        return;

    if (b != visible)
    {
        if (pAttachCharacter)
        {
            visible = b;

            if (visible && backupHeight != 0)
                pAttachCharacter->PatHeight = ((int)(GetHeight() * 256));
            else if (pAttachCharacter->Pattern && backupHeight != 0)
                pAttachCharacter->PatHeight = backupHeight;

            pAttachCharacter->SetMotionFromCode(CHRMOTION_STATE_STAND);
            pAttachCharacter->ChangeMotion(0);
            pMountModel->model->Frame = 35;
        }
    }

    visible = b;
}

void CMountHandler::SetMount(const EMountID& id)
{
    if (eMountID != id)
        updateBoundingBox = true;

    eMountID = id;
    pMountModel = cMountManager.GetMountModel(id);
}

void CMountHandler::SetAttachCharacter(smCHAR* pCharacter)
{
    pAttachCharacter = pCharacter;

    if (backupHeight == 0)
        backupHeight = pAttachCharacter->PatHeight;

    //Procurar pela base frame
    if (pCharacter->smMotionInfo)
    {
        for (int i = 0; i < pCharacter->smMotionInfo->MotionCount; i++)
        {
            auto motionInfo = pCharacter->smMotionInfo->MotionInfo[i];

            if (motionInfo.ItemCodeList[0] == 0xFE)
            {
                if (motionInfo.ItemCodeCount > 10)
                    continue;
                for (int j = 1; j < motionInfo.ItemCodeCount; j++)
                {
                    if (motionInfo.ItemCodeList[j] == eMountID)
                    {
                        iBaseFrame = motionInfo.StartFrame;
                        return;
                    }
                }
            }
        }
    }
}

void CMountHandler::Update(float fTime)
{
    if (myCharacter)
        pAttachCharacter = lpCurPlayer;
}

Math::Vector3Int CMountHandler::Render()
{
    Math::Vector3Int ret;

    if ((pMountModel) && pAttachCharacter && pMountModel->model && visible)
    {
        if (!CanUse())
        {
            SetVisible(false);
            return ret;
        }

        if (pAttachCharacter->action < 50)
            return ret;

        POINT3D rotation = { pAttachCharacter->Angle.x, pAttachCharacter->Angle.y, pAttachCharacter->Angle.z };
        POINT3D pos = { pAttachCharacter->pX, pAttachCharacter->pY, pAttachCharacter->pZ };

        if (eMountID == MOUNTID_IceTiger || eMountID == MOUNTID_Horse || eMountID == MOUNTID_Snowdeer)
            rotation.y += ANGLE_180;

        rotation.y = -rotation.y;

        if (pMountModel->model->newModel->skeleton)
            pMountModel->model->newModel->skeleton->forceUpdate = true;

        pMountModel->model->newModel->forceUpdate = true;
        pMountModel->model->Frame = iCurrentFrame;

        pMountModel->model->SetPosi(&pos, &rotation);
        pMountModel->model->newModel->Render();
    }

    return ret;
}