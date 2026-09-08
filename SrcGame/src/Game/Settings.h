#pragma once

class Settings
{
public:
	static Settings* GetInstance() { static Settings instance; return &instance; }

	BOOL IsOpen() { return cOpen; }
	bool IsBlockingMouse(int x, int y) const;
	bool MouseAction(int x, int y, int w, int h);

	int  cImages[45];
	void Init();
	void Open();
	void Close();
	void Draw();
	void Button();
	void Load();
	void Save(bool resize = true);
	void SaveRememberLogin();
	void Reset();
	void Set(int Windowed, int Width, int Height);
	bool setAutoAdjust(int iWidth, int iHeight);

	int  bWindowed, cWindowed;
	int  bAutoAdjust, cAutoAdjust;
	int  bBorderless, cBorderless;
	int  bRatio, cRatio;
	int  bResolution, cResolution;
	int  bTexture, cTexture;
	int  bBPP, cBPP;
	int  bEffects, cEffects;
	int  bLights, cLights;
	int  bShadows, cShadows;
	int  vSync, cvSync;
	int  vBlockUI, cvBlockUI;
	BOOL UiBlock() { return vBlockUI == 1 ? TRUE : FALSE; }

	int  bMusic, cMusic;
	int  bMVol, cMVol;
	int  bSound, cSound;
	int  bAmbient, cAmbient;
	int  bSVol, cSVol;

	int  bCamView, cCamView;
	int  bCamRange, cCamRange;
	int  bCamShake, cCamShake;
	int  bCamInv, cCamInv;

	int  bFilter, cFilter;
	int  bFilterSpec, cFilterSpec;
	int  bHP, cHP;
	int  bMP, cMP;
	int  bSP, cSP;
	int  bGold, cGold;
	int  bAmulets, cAmulets;
	int  bRings, cRings;
	int  bSheltoms, cSheltoms;
	int  bForce, cForce;
	int  bPremiums, cPremiums;
	int  bCrystal, cCrystal;
	int  bDefItem, cDefItem;
	int  bOffItem, cOffItem;
	int  bElse, cElse;
	int  bMS, cMS;
	int  bFS, cFS;
	int  bPS, cPS;
	int  bAS, cAS;
	int  bKS, cKS;
	int  bATS, cATS;
	int  bPRS, cPRS;
	int  bMGS, cMGS;
	int  bNaked, cNaked;

	int  bRememberLogin;
	int  bHidePlayerNames;
	int  bShowLife;
	int  bShowNotice;
	int  bShowFPS, cShowFPS;
	int  bWeather, cWeather;
	int  bShowDamage;

private:
	bool cOpen;
	int  m_activeTab;
	bool m_confirmReset;
	float m_winX, m_winY, m_winW, m_winH;
	int  cRememberLogin;
	int  cHidePlayerNames;
	int  cShowLife;
	int  cShowNotice;
	int  cShowDamage;

	void* m_titleTex;
	int   m_titleW;
	int   m_titleH;
	bool  m_titleTried;

	void CopyEditFromCommitted();
	void CopyCommittedFromEdit();
	void ApplyRuntime(bool resize);
	void FillEditDefaults();
	void ResolveResolution();
	void PushWindowStyle();
	void PopWindowStyle();
	void DrawWindowChrome(float headerH);
	void DrawTitleHeader();
	void DrawResetConfirm();
	void DrawVideoTab();
	void DrawAudioTab();
	void DrawCameraTab();
	void DrawInterfaceTab();
	void DrawFilterTab();
	void DrawFooter();
	void DrawSectionHeader(const char* title);
	void EnsureTitleTexture();
};
