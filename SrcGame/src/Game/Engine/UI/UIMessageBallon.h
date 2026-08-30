#pragma once

#include "UIImage.h"
#include "UIText.h"
#include <memory>
#include <vector>
#include <map>

struct EXETextureHandle
{
	char szName[64];
	char szNameA[64];

	LPDIRECT3DTEXTURE9 pcD3DTexture;

	int iWidth;
	int iHeight;

	int iUseCounter;

	int iTransparency;

	BOOL bSwap;
};

struct EXESurfaceOld
{
	int iReferenceCount;
	EXETextureHandle sTexture;
	void* psCache;
};

struct ClanInfoHeader
{
	char					szClanTexture[40];
	DWORD					dwClanTextureNum;
	char					szClanName[40];
	char					szClanText[90];
	char					cGserver[4];
	char					szUniqueClanInfo[64];
};

struct NewClanInfo
{
	ClanInfoHeader			sHeader;

	EXESurfaceOld* psTexture16;
	EXESurfaceOld* psTexture32;

	DWORD					dwTime;
	int						iStatus; 
	int						iSaveNum;
};

enum ESkinID
{
	SKINID_Undefined = -1,

	SKINID_Normal,
	SKINID_Blue,
	SKINID_Green,
	SKINID_Transparent
};

namespace UI
{
	struct Balloon
	{
		class Image* pImageLeftTop;
		class Image* pImageMiddleTop;
		class Image* pImageRightTop;
		class Image* pImageLeft;
		class Image* pImageRight;
		class Image* pImageLeftBottom;
		class Image* pImageMiddleBottom;
		class Image* pImageRightBottom;
		class Image* pImageTailBottom;
		class Image* pImageCenter;
	};

	enum EPositionText
	{
		POSITIONTEXT_Top,
		POSITIONTEXT_Middle,
		POSITIONTEXT_Bottom,
	};

	struct ExtraText
	{
		Text_ptr pText;
		EPositionText ePosition;
	};

	typedef std::shared_ptr<class MessageBalloon> MessageBallon_ptr;
	class MessageBalloon : public Element
	{
	public:
		//Constructor
		MessageBalloon(Rectangle2D rRectangle);
		virtual ~MessageBalloon();

		void Init();

		void SetMessage(std::string strMessage);
		void SetClanName(std::string strText);
		void SetPetName(std::string strText, DWORD dwColor = -1);
		void AddCharTitleText(char* strText, int iRarity);
		void ResetMessage();
		Text_ptr GetTextMessage() { return pTextMessage; }

		void Render(int iX, int iY, int iWidth, int iHeight, int iSourceX, int iSourceY);

		void SetHover(BOOL b) { bHover = b; }
		void SetColor(DWORD dwColor);

		void IsCharacterMessage(BOOL b);
		void SetSkin(ESkinID e) { eSkinID = e; }

		void SetBellatraIconID(int i) { iBellatraCrown = i; }

		void SetClanIconTexture(LPDIRECT3DTEXTURE9 ps);

		void DrawMessageBalloonBox(int iX, int iY, int iWidth, int iHeight);

		void							Update(float fTime);
	protected:
		const UINT						TIME_STRINGHASHBALLON_UPDATE = 15000;

		struct StringBalloonHash
		{
			std::string					strText;

			DWORD						dwTimeOut;

			int							iHighlightWidth;

			StringBalloonHash(const std::string _strText, int _iHighlightWidth, DWORD _dwTime)
			{
				strText = _strText;
				dwTimeOut = _dwTime;
				iHighlightWidth = _iHighlightWidth;
			};

			~StringBalloonHash() {};
		};


		Balloon							saMessagesBalloon[9][2];
		ESkinID							eSkinID = SKINID_Normal;


		BOOL							bHover = FALSE;
		BOOL							bIsMessage = FALSE;
		BOOL							bIsCharacterMessage = FALSE;
		BOOL							bClanName = FALSE;
		BOOL							bPetName = FALSE;
		BOOL							bHasTitle = FALSE;
		Text_ptr						pTextMessage;
		Text_ptr						pTextClanName;
		Text_ptr						pTextPetName;
		Text_ptr						pTextTitle;

		BOOL							bShowClanIcon = FALSE;
		LPDIRECT3DTEXTURE9 			psTextureClanIcon;

		int								eCharClass = 0;
		int								iBellatraCrown = -1;
		short							sBellatraSoloCrown = -1;
		class Image* pcaBellatraCrown[6];
		class Image* pcaBellatraCrownSoloSword[3];
		class Image* pcaClassIcon14Pixel[10];

		class Image* pcNotifyQuest[6];

		std::map<std::size_t, StringBalloonHash> vCache;
	};
};