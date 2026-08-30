#pragma once

#include <Windows.h>
#include <Engine/UI/UIImage.h>

#define LOGINHANDLER CLoginModel::GetInstance()

struct LoginMemoryData
{
	char										szUnkData[0x84];
	int									eState;
};

class CLoginModel
{
protected:
	LoginMemoryData* psLoginData = NULL;
	const int									iWidthServerSelectBox = 230;
	const int									iHeightServerSelectBox = 161;

	UI::Image* pBoxImageHandler;
	UI::Image* pImageBGLogin = NULL;

	BOOL										bLoaded = FALSE;
public:
	static              CLoginModel* GetInstance() { static CLoginModel instance; return &instance; }

	CLoginModel();
	virtual ~CLoginModel();

	void										OnRender();
	void										Init();

	void										RenderLoginBG();
};

