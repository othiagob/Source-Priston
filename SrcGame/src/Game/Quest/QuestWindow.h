#pragma once
#include <Windows.h>
#include <string>

class QuestWindow
{
public:
	bool openFlag = false;
	bool openOverlay = false;

	static QuestWindow* GetInstance() { static QuestWindow instance; return &instance; }
	void openWindow(bool* p_open);
	void questOverlay(bool* p_open);
	void FocusQuest(int questID);
	bool IsBlockingMouse(int x, int y) const;

private:
	enum class QuestGroup
	{
		Ready,
		Active,
		Available,
		Complete
	};

	int m_selected = 1;
	int m_activeTab = 0;
	bool m_confirmOpen = false;
	int m_confirmQuestId = 0;
	int m_confirmQuestType = 0;
	std::string m_confirmTitle;
	bool m_overlayMinimized = false;
	bool m_forceSelectTab = false;
	float m_winX = 0.0f;
	float m_winY = 0.0f;
	float m_winW = 0.0f;
	float m_winH = 0.0f;
	float m_overlayX = 0.0f;
	float m_overlayY = 0.0f;
	float m_overlayW = 0.0f;
	float m_overlayH = 0.0f;
	DWORD m_finishClickTick = 0;
	int m_finishClickQuestId = 0;
	static const int kQuestOverlayHideMax = 64;
	bool m_overlayHidden[kQuestOverlayHideMax] = {};

	void* m_titleTex = nullptr;
	int m_titleW = 0;
	int m_titleH = 0;
	bool m_titleTried = false;
	void* m_overlayTitleTex = nullptr;
	int m_overlayTitleW = 0;
	int m_overlayTitleH = 0;
	bool m_overlayTitleTried = false;

	void PushWindowStyle();
	void PopWindowStyle();
	void DrawWindowChrome(float headerH);
	void DrawConfirmChrome();
	void DrawOverlayChrome(float headerH);
	void DrawTitleHeader(bool* p_open);
	void EnsureTitleTexture();
	void EnsureOverlayTitleTexture();
	void GetQuestProgress(const struct QUEST_BODY* body, const struct QUEST_INFO* info, int* current, int* total) const;
	void DrawQuestList(int tab);
	void DrawQuestDetails();
	void DrawObjectives(const struct QUEST_BODY* body, const struct QUEST_INFO* info);
	void DrawRewards(const struct QUEST_BODY* body);
	void DrawDetailTitleActions(const struct QUEST_BODY* body, const struct QUEST_INFO* info);
	void DrawActionFooter(const struct QUEST_BODY* body, const struct QUEST_INFO* info);
	bool IsShownOnOverlay(int questID) const;
	void SetShownOnOverlay(int questID, bool shown);
	void DrawListItem(const struct QUEST_BODY* body, const struct QUEST_INFO* info, bool showLevelHint);
	void DrawObjectiveRow(const char* label, int current, int total, const char* mapName);
	void DrawSectionHeader(const char* title);
	void DrawBadge(const char* text, unsigned int color);
	void AskCancelQuest(const char* title, int questID, int questType);
	void DrawCancelConfirm();

	bool MatchesTab(int questType, int tab) const;
	QuestGroup GetGroup(const struct QUEST_INFO* info) const;
	const struct QUEST_BODY* FindBody(int questID) const;
	int FindDefaultSelection(int tab) const;
	const char* TypeLabel(int questType) const;
	const char* GroupLabel(QuestGroup group) const;
	std::string MapName(int mapId) const;
	std::string ToUtf8(const char* src) const;
	void EnsureSelectionForTab(int tab);
	unsigned int ColorForGroup(QuestGroup group) const;
	int PlayerLevel() const;
	bool IsInLevelRange(const struct QUEST_BODY* body) const;
};
