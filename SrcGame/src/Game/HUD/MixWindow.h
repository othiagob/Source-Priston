#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <utility>

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
	int mixId = 0;
	std::string mixName;
	std::string description;
	MixType mixType;
	std::vector<std::pair<Sheltons, int>> sheltonList[100];
};

class MixWindow
{
public:
	static MixWindow* GetInstance() { static MixWindow instance; return &instance; }
	INT ReadMix();
	bool openFlag = false;
	void OpenNpc(bool* p_open);
	bool IsBlockingMouse(int x, int y) const;

private:
	float m_winX = 0.0f;
	float m_winY = 0.0f;
	float m_winW = 0.0f;
	float m_winH = 0.0f;
	void* m_titleTex = nullptr;
	int m_titleW = 0;
	int m_titleH = 0;
	bool m_titleTried = false;
	void* m_stoneTex[15] = {};
	bool m_stoneTried = false;
	int m_activeTab = 0;

	void PushWindowStyle();
	void PopWindowStyle();
	void DrawWindowChrome(float headerH);
	void DrawTitleHeader(bool* p_open);
	void DrawSectionHeader(const char* title);
	void EnsureTitleTexture();
	void EnsureStoneTextures();
	void DrawMixTab(MixType type);
	void DrawMixCard(const MixInfo& mix);
};
