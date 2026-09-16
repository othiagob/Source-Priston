#pragma once

#include <string>
#include <vector>
#include "smPacket.h"

class PostBoxWindow
{
public:
	static PostBoxWindow* GetInstance() { static PostBoxWindow instance; return &instance; }

	bool openFlag = false;
	void Open();
	void Close();
	void OpenNpc(bool* p_open);
	bool IsBlockingMouse(int x, int y) const;
	bool ShouldCaptureKeyboard() const;
	bool OnEscape();
	void ReceiveList(TRANS_POSTBOX_LIST* list);
	void ReceiveClaimResult(int result, DWORD entryId);
	void ReceiveRefuseResult(int result, DWORD entryId);
	void ReceiveSendResult(int result, DWORD code, DWORD head, DWORD chkSum);

private:
	float m_winX = 0.0f;
	float m_winY = 0.0f;
	float m_winW = 0.0f;
	float m_winH = 0.0f;
	void* m_titleTex = nullptr;
	int m_titleW = 0;
	int m_titleH = 0;
	bool m_titleTried = false;
	void* m_frameTex = nullptr;
	int m_frameW = 0;
	int m_frameH = 0;
	bool m_frameTried = false;
	int m_activeTab = 0;
	int m_selected = -1;
	int m_sendSelected = -1;
	DWORD m_pendingClaim = 0;
	bool m_passFocus = false;
	bool m_destFocus = false;
	char m_passUtf8[32] = {};
	char m_destUtf8[48] = {};
	char m_statusUtf8[192] = {};
	std::vector<TRANS_POSTBOX_ENTRY> m_entries;
	std::vector<void*> m_icons;
	void* m_sendIcon = nullptr;
	int m_sendIconFor = -1;
	int m_expectedChunks = 0;
	int m_gotChunks = 0;

	void PushWindowStyle();
	void PopWindowStyle();
	void DrawWindowChrome(float headerH);
	void DrawTitleHeader(bool* p_open);
	void DrawSectionHeader(const char* title);
	void EnsureTitleTexture();
	void EnsureFrameTexture();
	bool HasFrame() const;
	float FrameRim() const;
	float BodyInsetX() const;
	void DrawReceiveTab();
	void DrawSendTab();
	void DrawDetail();
	void DrawSendPreview();
	void RequestOpen();
	void RequestClaim();
	void RequestRefuse();
	void RequestSend();
	void LoadEntryIcon(int index);
	void ReleaseIcons();
	void ReleaseSendIcon();
	void SetStatus(const char* utf8);
	void RemoveInventoryItem(DWORD code, DWORD head, DWORD chkSum);
	bool CanSendInventoryItem(int invenIndex) const;
	std::string ToUtf8(const char* src) const;
	const TRANS_POSTBOX_ENTRY* Selected() const;
};
