#include "TitleBox.h"
#include "def.h"
#include "Engine/DirectX/DXFont.h"
#include "Engine/UI/UIImageLoader.h"
#include "Engine/UI/UIImageRender.h"
#include "sinbaram/sinMessageBox.h"
TitleBox::TitleBox()
{
}

TitleBox::~TitleBox()
{
}

void TitleBox::Render()
{
	int i = 0;
	for (auto& v : vTitles)
	{
		if (i == 1)
			v.d3dcolor.a = 127;
		else if (i == 2)
			v.d3dcolor.a = 70;

		v.iOpacity = (int)(float)(v.d3dcolor.a * 1.0f);

		FONTHANDLER->Draw(iFontID, RECT{ 0, v.sPosition.iY + 2, RESOLUTION_WIDTH, 24 }, v.strText, DT_CENTER | DT_EXPANDTABS | DT_NOCLIP, D3DCOLOR_ARGB((int)v.iOpacity, 0, 0, 0));
		FONTHANDLER->Draw(iFontID, RECT{ 0, v.sPosition.iY, RESOLUTION_WIDTH, 24 }, v.strText, DT_CENTER | DT_EXPANDTABS | DT_NOCLIP, D3DCOLOR_ARGB((int)v.iOpacity, (int)v.d3dcolor.r, (int)v.d3dcolor.g, (int)v.d3dcolor.b));
		i++;
	}
}

void TitleBox::SetText(char* pszText, int durationInSeconds)
{
	if (pszText && pszText[0] != 0) // posição do título y
		AddTitle(Title(Point2D(0, 90), trim((std::string)pszText), D3DXCOLOR(255, 185, 50, 255), durationInSeconds));
}

void TitleBox::SetText(int iID)
{
	struct MessageBox1
	{
		char				szMessage1[64];
		char				szMessage2[64];
		BOOL				bActive;
		int					iLength;
		int					iX;
		int					iY;
		int					iLine;
	};

	extern sMESSAGEBOX sMessageBox[100];

	if (iID >= 0 && iID < sizeof(sMessageBox) / sizeof(sMESSAGEBOX))
	{
		MessageBox1* ps = reinterpret_cast<MessageBox1*>(sMessageBox);

		if (ps[iID].szMessage1[0] != 0)
			AddTitle(Title(Point2D(0, 100), trim((std::string)ps[iID].szMessage1), D3DXCOLOR(255, 185, 50, 255), MAX_DURATION_TITLEBOX));
	}
}

void TitleBox::RenderTextMiddle(const char* pszText)
{
	if (pImageLeft && pImageCenter && pImageRight && STRLEN(pszText) > 0)
	{
		std::string str = trim((std::string)pszText).c_str();

		int iW = GETWIDTHTEXT(str.c_str());
		int iX = (RESOLUTION_WIDTH >> 1) - ((iW + 64) >> 1);
		int iY = (RESOLUTION_HEIGHT >> 1) - (pImageLeft->GetHeight() >> 1);

		UI::ImageRender::Render(pImageLeft, iX, iY, pImageLeft->GetWidth(), pImageLeft->GetHeight(), -1);
		UI::ImageRender::Render(pImageCenter, iX + 32, iY, iW, pImageLeft->GetHeight(), -1);
		UI::ImageRender::Render(pImageRight, iX + 32 + iW, iY, pImageLeft->GetWidth(), pImageLeft->GetHeight(), -1);

		FONTHANDLER->Draw(0, iX + 32, iY + 13, str, 0, -1);
	}
}

void TitleBox::AddTitle(Title psTitle)
{
	BOOL bFound = FALSE;

	for (auto& v : vTitles)
	{
		if (v.strText.compare(psTitle.strText) == 0)
		{
			bFound = TRUE;
			break;
		}
	}

	if (bFound == FALSE)
	{
		vTitles.insert(vTitles.begin(), psTitle);

		for (auto& v : vTitles)
		{
			if (!v.strText.empty())
				v.bMoveToDown = TRUE;
		}

		if (vTitles.size() > MAX_MESSAGES)
			vTitles.pop_back();
	}
}

void TitleBox::Update(float fTime)
{
	int i = 0;

	for (auto& v : vTitles)
	{

		if (v.bMoveToDown)
		{
			float fTimeUpdate = v.fTime / MAX_DURATION_ANIM;
			float f = X3D::easeInOutCircf(fTimeUpdate, 0.0f, 1.0f, 1.0f);

			if (v.iBasePositionY == 0)
				v.iBasePositionY = v.sPosition.iY;
			// distancia que ele cresce
			v.sPosition.iY = (int)(v.iBasePositionY + (80 * (1.0f - f)));
		}

		if (v.fTime > 0.0f)
			v.fTime -= fTime;
		else
		{
			v.bMoveToDown = FALSE;
			v.fTime = 100.0f;
			v.iBasePositionY = 0;
		}




		i++;

	}

	Tick(fTime);
}

void TitleBox::Tick(float time)
{
	static double fCount_1SECO = 0.0f;
	static double f1SECO = 1000.0f;

	fCount_1SECO += time;

	// 1000 ms
	while (fCount_1SECO >= f1SECO)
	{
		for (auto& v : vTitles)
		{
			v.sDuration--;

			if (v.sDuration <= 3 && !v.bCanHide)
			{
				v.bCanHide = TRUE;
				v.fTime = 100.0f;
			}

			if (v.sDuration <= 0)
				vTitles.pop_back();
		}

		fCount_1SECO -= f1SECO;
	}
}

void TitleBox::Init()
{
	iFontID = FONTHANDLER->CreateFont("Frutiger LT 55 Roman", 25, 0, FALSE, FALSE, -1);

	pImageLeft = UI::ImageLoader::LoadImage_("game\\images\\title\\left.png");
	pImageCenter = UI::ImageLoader::LoadImage_("game\\images\\title\\center.png");
	pImageRight = UI::ImageLoader::LoadImage_("game\\images\\title\\right.png");
}