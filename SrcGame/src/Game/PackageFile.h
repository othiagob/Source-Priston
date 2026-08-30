#pragma once

#include <vector>
#include <unordered_map>
#include <string>

class CZipArchive;

class PackageFile
{
public:
    const std::vector<int> vKeys = { 0xBA, 0x1B, 0x3C, 0x94 };
    static PackageFile& Get() { static PackageFile instance; return instance; }

    PackageFile(PackageFile const&) = delete;             // Copy construct
    PackageFile(PackageFile&&) = delete;                  // Move construct
    PackageFile& operator=(PackageFile const&) = delete;  // Copy assign
    PackageFile& operator=(PackageFile&&) = delete;      // Move assign

    CZipArchive* ReadPackage(const char* pszFileName);

    std::vector<char> GetFileBufferCached(const char* pszFileName);
    std::vector<char> GetFileBuffer(CZipArchive* zip, const char* pszFileName);

    FILE* GetFileCached(const char* pszFileName, unsigned int& iFileSize);
    FILE* GetFile(CZipArchive* zip, const char* pszFileName, unsigned int& iFileSize);

    void CloseFile();
    void ClosePackage(CZipArchive* zip);
protected:
    //Constructor
    PackageFile();
    ~PackageFile();
private:
    std::unordered_map<std::string, CZipArchive*> cachedPackageFiles;
    std::unordered_map<std::string, bool> notFoundFiles;

    std::unordered_map<std::string, CZipArchive*> cachedFiles;

    bool bInitialized;
    std::string strDataFolder;
    FILE* pFile;
};