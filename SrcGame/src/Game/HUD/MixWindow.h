#pragma once
#include <Windows.h>
#include <vector>
#include <string>

#define OPEN_MIXLIST_NPC 0x51800012

enum Sheltons
{
    null,
    Lucidy,
    Sereno,
    Fadeo,
    Sparky,
    Raident,
    Transparo,
    Murki,
    Devine,
    Celesto,
    Mirage,
    Inferna,
    Enigma,
    Bellum,
    Ordo
};

enum MixType
{
    Weapons,
    Armors,
    Shields,
    Boots,
    Gloves,
    Bracelets,
    Rings,
    Orbs,
    Amulets
};

struct MixInfo
{
    int ID = 0;
    std::string mixName;
    std::string description;
    MixType mixType;
    std::vector<std::pair<Sheltons, int>> sheltonList[100];
};

class MixWindow
{
    
public:
    static              MixWindow* GetInstance() { static MixWindow instance; return &instance; }
    INT ReadMix();
    bool openFlag = false;
    void OpenNpc(bool* p_open);

private:
};

