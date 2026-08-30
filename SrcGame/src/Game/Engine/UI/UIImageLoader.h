#pragma once

#include "UIImage.h"
#include <d3dx9.h>
#include <utility>

struct stImageStructure
{
	IDirect3DTexture9* hTexHandler;
	char szFilename[256];
	int width;
	int height;
	int iFileFormat;
};

namespace UI
{
	class ImageLoader
	{
	public:
		ImageLoader();
		virtual ~ImageLoader();

		static void SetScale(class Image* pImage, int iW, int iH, D3DXVECTOR2* pVector);
		static class Image* Swap(Image* ptr);
		static class Image* LoadImage_(const char* pszFile, ...);
		static std::pair<class Image*, class Image*> LoadActiveImage(char* pszFile1, char* pszFile2);

		static void DeleteImage(Image* ptr);

		void* GetDataFromFile(const char* FilePath, size_t& FileSize) const;
		void* readData(const char* filepath, size_t& filesize, bool& free) const;

		static ImageLoader* GetInstance() { static ImageLoader instance; return &instance; }
	};

	BOOL DecryptBMP(char* pBuffer, unsigned int uBufferSize);
	BOOL DecryptTGA(char* pBuffer, unsigned int uBufferSize);
}

#define IMAGELOADER (UI::ImageLoader::GetInstance())