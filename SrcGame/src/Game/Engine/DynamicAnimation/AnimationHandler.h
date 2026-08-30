#pragma once

#include <Windows.h>
#include <vector>
#include <string>

class AnimationHandler
{
public:
    struct CustomAnimation
    {
        int iType;
        bool bLoop;
        int iLength;
    };

    AnimationHandler(AnimationHandler const&) = delete;             // Copy construct
    AnimationHandler(AnimationHandler&&) = delete;                  // Move construct
    AnimationHandler& operator=(AnimationHandler const&) = delete;  // Copy assign
    AnimationHandler& operator=(AnimationHandler&&) = delete;      // Move assign

    static AnimationHandler& Get() { static AnimationHandler instance; return instance; }

    BOOL HandleBoneCache(struct smDPAT* psModelData);
    BOOL AddCacheModelData(struct smMODELINFO* psModelData);

    void LoadDynamicAnimation();
    void FixSMBINXSubFileMotion(const char* pszSMBFile, const char* pszINXFile, const char* pszSMBFileOut, const char* pszINXFileOut);
    bool InjectDynamicAnimation(struct smDPAT* psModelData, class smPAT3D* pcModelAnimation, int iID, BOOL bLoop, int iClassFlag, int iAreaType, int iaEvent[4], int iItemType, int iTypeAnim, std::vector<CustomAnimation> vAnimations = {}, std::vector<std::string> vItems = {});
    bool InjectAnimation(const char* pszSMBFileFrom, const char* pszSMBFileTo, const char* pszINXFileTo, int eAnimationType, int iAreaType, BOOL bLoop, unsigned int uClassFlag, unsigned int uSkillCode = 0, std::vector<CustomAnimation> vAnimations = {}, std::vector<std::string> vItems = {});
protected:
    //Constructor
    AnimationHandler();
    ~AnimationHandler();
private:
    std::vector<struct smMODELINFO*> vAnimationModels;
};