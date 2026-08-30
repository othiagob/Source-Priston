#pragma once

#include "Model.h"
#include "View.h"
#include "Controller.h"

class Game
{
public:
	// Construction
											Game();
	virtual									~Game();

	// Initialization
	BOOL									Init();
	void									Run();
	void									Shutdown();
private:

};
