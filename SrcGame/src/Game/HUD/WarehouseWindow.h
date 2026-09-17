#pragma once

struct sITEM;

class WarehouseWindow
{
public:
	static WarehouseWindow* GetInstance() { static WarehouseWindow instance; return &instance; }

	void OpenNpc(bool* p_open);
	bool IsBlockingMouse(int x, int y) const;
	bool ShouldEatClick() const { return m_eatClick; }
	bool ShouldHideClassicPanels() const;
	void ArmHideClassic();
	void RequestClose();
	bool ConsumeCloseRequest();
	void EndFrameClassicDraw();
	bool IsSearchFocused() const { return m_searchFocused; }
	bool ShouldCaptureKeyboard() const;
	bool OnEscape();
	void ClearSearch();
	void ScreenToLogical(int screenX, int screenY, int* outX, int* outY) const;
	bool IsOverGrid(int x, int y) const;
	bool TrySendWarehouseItemToInventory(int logX, int logY);

private:
	float m_winX = 0.0f;
	float m_winY = 0.0f;
	float m_winW = 0.0f;
	float m_winH = 0.0f;
	float m_gridX = 0.0f;
	float m_gridY = 0.0f;
	float m_gridW = 0.0f;
	float m_gridH = 0.0f;
	void* m_titleTex = nullptr;
	int m_titleW = 0;
	int m_titleH = 0;
	bool m_titleTried = false;
	void* m_frameTex = nullptr;
	int m_frameW = 0;
	int m_frameH = 0;
	bool m_frameTried = false;
	bool m_searchFocused = false;
	bool m_wasOpen = false;
	bool m_placeArmed = false;
	bool m_goldOpen = false;
	bool m_goldDeposit = true;
	bool m_eatClick = false;
	bool m_hideClassic = false;
	bool m_requestClose = false;
	int m_goldAmount = 0;
	char m_goldInput[48] = {};
	char m_searchUtf8[64] = {};
	char m_lastSearchUtf8[64] = {};
	int m_filter = 0;
	float m_cell = 22.0f;
	void* m_filterTex[7] = {};
	bool m_filterTried = false;

	void PushWindowStyle();
	void PopWindowStyle();
	void DrawWindowChrome(float headerH);
	void DrawTitleHeader(bool* p_open);
	void EnsureTitleTexture();
	void EnsureFrameTexture();
	bool HasFrame() const;
	void DrawSearch();
	void DrawFilters();
	void DrawPages();
	void DrawGrid();
	void DrawSideInfo();
	void DrawGoldModal();
	void EnsureFilterTextures();
	bool ItemVisible(sITEM* item) const;
	bool ItemMatchesFilter(sITEM* item) const;
	void ApplyGoldMove(bool deposit);
	void HandleDragAndClick();
	void LogicalToScreen(int logicX, int logicY, float* outX, float* outY) const;
	void OpenGoldModal(bool deposit);
	void SyncGoldInputFromAmount();
	void ParseGoldInput();
};
