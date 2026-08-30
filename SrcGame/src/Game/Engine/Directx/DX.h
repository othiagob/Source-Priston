#include "DXGraphicEngine.h"
#include "DXGraphicsDevice.h"
#include "DXFont.h"
#include "DXLogger.h"
#include "DXTexture.h"
#include "DXSprite.h"
#include "PostProcess.h"

namespace DX
{
	// DXGraphicEngine
	typedef std::shared_ptr<class DXGraphicEngine> Graphic_ptr;
	typedef class DXGraphicEngine Graphic;

	// DXFont
	typedef std::shared_ptr<class DXFont> Font_ptr;
	typedef class DXFont Font;

	// DXLogger
	typedef class DXLogger Logger;
	typedef std::shared_ptr<class DXLogger> Logger_ptr;

	// DXImageSceneRenderer
	typedef class DXImageSceneRenderer ImageSceneRenderer;
	typedef std::shared_ptr<class DXImageSceneRenderer> ImageSceneRenderer_ptr;

	// DXPostProcess
	typedef class DXPostProcess	PostProcess;
};