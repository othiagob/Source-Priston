#pragma once
#include <Windows.h>
#include <string>
#include <vector>

#define OPEN_RANKING_NPC 0x51800010

struct TopPlayers {
    char playerName[32];
    int playerClass = 0;
    int playerLevel = 0;
    int playerBps = 0;
};

struct TopPlayerPckg {
    int size;
    int code;
    TopPlayers Players[181];
};

class RankingWindow
{
public:
    static RankingWindow* GetInstance() { static RankingWindow instance; return &instance; }
    INT ReceivePlayers(TopPlayerPckg* Pckg);
    INT OpenNPCRanking();
    bool openFlag = false;
    void OpenNpc(bool* p_open);
    bool IsBlockingMouse(int x, int y) const;

private:
    std::vector<TopPlayers> m_players;
    std::vector<TopPlayers> m_playersPvp;
    float m_winX = 0.0f;
    float m_winY = 0.0f;
    float m_winW = 0.0f;
    float m_winH = 0.0f;
    void* m_titleTex = nullptr;
    int m_titleW = 0;
    int m_titleH = 0;
    bool m_titleTried = false;
    void* m_classTex[12] = {};
    int m_classW[12] = {};
    int m_classH[12] = {};
    bool m_classTried = false;
    int m_selectedClass = 0;
    int m_activeTab = 0;

    void PushWindowStyle();
    void PopWindowStyle();
    void DrawWindowChrome(float headerH);
    void DrawTitleHeader(bool* p_open);
    void DrawSectionHeader(const char* title);
    void EnsureTitleTexture();
    void EnsureClassTextures();
    void DrawClassFilter();
    void DrawRankingBody(bool pvp);
    std::string ToUtf8(const char* src) const;
};
