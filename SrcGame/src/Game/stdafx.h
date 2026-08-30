#pragma once

#include "targetver.h"
// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN   

// Windows Header Files:
#include <windows.h>
#include <string>
#include <memory>
#include <time.h>
#include <vector>
#include <functional>
#include <map>
#include <fstream>
#include <list>
#include <queue>
#include <strsafe.h>
#include <TlHelp32.h>
#include <winsock.h>
#pragma comment(lib,"Imagehlp.lib")
#pragma comment(lib,"imm32.lib")
#pragma comment(lib,"winmm.lib")

// TODO: reference additional headers your program requires here