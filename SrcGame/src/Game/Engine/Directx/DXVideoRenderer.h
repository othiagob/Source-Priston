#pragma once

#include <Windows.h>
#include <string>
#include <strmif.h>
#include <control.h>
#include <d3d9.h>

#define MAX_AUDIO_STREAMS		5
#define MAX_VIDEO_STREAMS		5


interface
	ISampleGrabberCB
	:
	public IUnknown
{
	virtual STDMETHODIMP SampleCB(double SampleTime, IMediaSample* pSample) = 0;
	virtual STDMETHODIMP BufferCB(double SampleTime, BYTE* pBuffer, long BufferLen) = 0;
};

static
const
IID IID_ISampleGrabberCB = { 0x0579154A, 0x2B53, 0x4994,{ 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85 } };

interface
	ISampleGrabber
	:
	public IUnknown
{
	virtual HRESULT STDMETHODCALLTYPE SetOneShot(BOOL OneShot) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetMediaType(const AM_MEDIA_TYPE* pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(AM_MEDIA_TYPE* pType) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(BOOL BufferThem) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(long* pBufferSize, long* pBuffer) = 0;
	virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(IMediaSample** ppSample) = 0;
	virtual HRESULT STDMETHODCALLTYPE SetCallback(ISampleGrabberCB* pCallback, long WhichMethodToCallback) = 0;
};

static
const
IID IID_ISampleGrabber = { 0x6B652FFF, 0x11FE, 0x4fce,{ 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F } };

static
const
CLSID CLSID_SampleGrabber = { 0xC1F400A0, 0x3F08, 0x11d3,{ 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };

static
const
CLSID CLSID_NullRenderer = { 0xC1F400A4, 0x3F08, 0x11d3,{ 0x9F, 0x0B, 0x00, 0x60, 0x08, 0x03, 0x9E, 0x37 } };

static
const
CLSID CLSID_VideoEffects1Category = { 0xcc7bfb42, 0xf175, 0x11d1,{ 0xa3, 0x92, 0x0, 0xe0, 0x29, 0x1f, 0x39, 0x59 } };

static
const
CLSID CLSID_VideoEffects2Category = { 0xcc7bfb43, 0xf175, 0x11d1,{ 0xa3, 0x92, 0x0, 0xe0, 0x29, 0x1f, 0x39, 0x59 } };

static
const
CLSID CLSID_AudioEffects1Category = { 0xcc7bfb44, 0xf175, 0x11d1,{ 0xa3, 0x92, 0x0, 0xe0, 0x29, 0x1f, 0x39, 0x59 } };

static
const
CLSID CLSID_AudioEffects2Category = { 0xcc7bfb45, 0xf175, 0x11d1,{ 0xa3, 0x92, 0x0, 0xe0, 0x29, 0x1f, 0x39, 0x59 } };

#define DXVIDEORENDERER DXVideoRenderer::GetInstance()

class DXVideoRenderer
{
public:

	static              DXVideoRenderer* GetInstance() { static DXVideoRenderer instance; return &instance; }

	DXVideoRenderer();
	virtual ~DXVideoRenderer();

	BOOL					Init(std::string _strFile);
	void					Shutdown();

	void					Run();

	void					Update();

	void					Render();

	void					Loop(BOOL b);

	double					GetPosition();
	void					SetPosition(double d);

	void					SetRepeatBegin(double d);

	double					GetDuration() { return dDuration; }

	BOOL					CanSeek();

	UINT					GetFPS() { return uFPS; }
	void					SetFPS(UINT u) { uFPS = u; }

	void					OnResetDevice();
	void					OnLostDevice();

protected:
	std::string				strFile;

	IGraphBuilder* pcGraphBuilder = NULL;
	IMediaControl* pcMediaControl = NULL;
	IVideoWindow* pcVideoWindow = NULL;
	IMediaEventEx* pcMediaEvents = NULL;
	IMediaPosition* pcMediaPosition = NULL;
	IMediaSeeking* pcMediaSeeking = NULL;
	IBaseFilter* pcBaseFilter = NULL;
	ISampleGrabber* pcSampleGrabber = NULL;
	IBaseFilter* pcNullRender = NULL;
	IBaseFilter* pcSourceFilter = NULL;

	LPDIRECT3DTEXTURE9		lpTexture = NULL;

	double					dDuration = 0.0;

	double					dRepeat = 0.0;

	double					dOldPosition = 0.0;

	BOOL					bFirstUpdate = TRUE;

	UINT					uWidth = 0;
	UINT					uHeight = 0;

	UINT					uFPS = 60;

	BOOL					bLoop = FALSE;

	BOOL					bInit = FALSE;

	char* pBufferData = NULL;
	long					lBufferSize = 0;

private:
};

