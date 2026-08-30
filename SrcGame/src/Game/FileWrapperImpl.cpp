#include "sinbaram/sinLinkHeader.h"
#include "FileWrapperImpl.h"

BOOL ReadFileNew(FILE* hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    if (hFile)
        return fread(lpBuffer, 1, nNumberOfBytesToRead, hFile);

    return 0;
}

int SetFilePointerNew(FILE* hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
{
    return fseek(hFile, lDistanceToMove, dwMoveMethod);
}

BOOL WriteFileNew(FILE* hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    return fwrite(lpBuffer, 1, nNumberOfBytesToWrite, hFile);
}