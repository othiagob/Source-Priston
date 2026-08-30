#include "stdafx.h"
#include "ofuscate.h"
#include "smwsock.h"
#include "AntiCheat.h"
#include "Quest\\QuestWindow.h"

typedef LONG NTSTATUS;

bool bLoadedNTDLL = false;

extern void sinSetLife(short Life);
extern void sinSetMana(short Mana);
extern void sinSetStamina(short Stamina);
extern int ReformCharForm();

#define STATUS_SUCCESS ((NTSTATUS)0x00000000L)
#define ThreadQuerySetWin32StartAddress 9

typedef NTSTATUS(WINAPI* NTQUERYINFOMATIONTHREAD)(HANDLE, LONG, PVOID, ULONG, PULONG);

extern smWINSOCK* smWsockDataServer;
extern CAntiCheat* pcAntiCheat;

std::vector<ChecksumFunction> vChecksumFunction;

CAntiCheat::CAntiCheat()
{
	ZeroMemory(&dwThreadsID, sizeof(DWORD) * 8);
}


CAntiCheat::~CAntiCheat()
{
}

HWND FindWindowPartialName(char* pszWindowName, char* pszTempVar)
{
	char szTempName[256];
	char szTempName2[256];
	char szNameWindow[256];
	int iLenTitle = 0;

	ZeroMemory(szNameWindow, 256);
	for (int i = 0; i < lstrlenA(pszWindowName); i++)
		szNameWindow[i] = tolower(pszWindowName[i]);

	HWND hWndTemp = FindWindowA(NULL, NULL);
	while (hWndTemp != 0)
	{
		iLenTitle = GetWindowTextA(hWndTemp, szTempName, 255);

		lstrcpy(szTempName2, szTempName);

		for (int i = 0; i < lstrlenA(szTempName); i++)
			szTempName[i] = tolower(szTempName[i]);

		if (lstrlenA(szTempName) && strstr(szTempName, szNameWindow))
		{
			lstrcpynA(pszTempVar, szTempName2, 256);
			break;
		}
		hWndTemp = GetWindow(hWndTemp, GW_HWNDNEXT);
	}



	return hWndTemp;
}


BOOL CAntiCheat::ChecksumCheckMismatch(ChecksumFunction* pChecksum)
{
	return TRUE;

	DWORD dwSize = pChecksum->dwSize;

	DWORD dwCheckSum = dwChecksumRet = 0;

	DWORD dwOld = 0;
	VirtualProtect((void*)pChecksum->dwAddress, dwSize, PAGE_EXECUTE_READWRITE, &dwOld);

	// Loop checksum generation
	for (DWORD i = 0; i < dwSize; i++)
		dwCheckSum += (DWORD)(*(BYTE*)(pChecksum->dwAddress + i)) ^ psPacketChecksum->bKey;

	// Mismatch? return sucesss
	if (dwCheckSum == pChecksum->dwCheckSum)
	{
		VirtualProtect((void*)pChecksum->dwAddress, dwSize, dwOld, NULL);
		return TRUE;
	}

	// Set checksum to global var and return fail
	dwChecksumRet = dwCheckSum;
	VirtualProtect((void*)pChecksum->dwAddress, dwSize, dwOld, NULL);

	return FALSE;
}

BOOL CAntiCheat::GetModuleName(DWORD dwThreadID, char* pszBuf, int iSizeBuf)
{
	HANDLE hModuleSnap;
	MODULEENTRY32 moduleEntry32;

	if ((hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId())) != INVALID_HANDLE_VALUE)
	{
		moduleEntry32.dwSize = sizeof(moduleEntry32);

		if (Module32First(hModuleSnap, &moduleEntry32))
		{
			do
			{
				if (moduleEntry32.hModule == GetModuleHandle(NULL))
					continue;

				PIMAGE_DOS_HEADER pdosheader = (PIMAGE_DOS_HEADER)moduleEntry32.modBaseAddr;
				PIMAGE_NT_HEADERS pntheaders = (PIMAGE_NT_HEADERS)((DWORD)moduleEntry32.modBaseAddr + pdosheader->e_lfanew);
				PIMAGE_SECTION_HEADER psectionheader = (PIMAGE_SECTION_HEADER)(pntheaders + 1);

				if (!IsGameModules(moduleEntry32.szExePath))
				{
					return TRUE;
					break;
				}
			} while (Module32Next(hModuleSnap, &moduleEntry32));
		}
	}

	CloseHandle(hModuleSnap);

	return FALSE;
}

BOOL CAntiCheat::AddNewThreadException(DWORD dwThreadID)
{
	if (!dwThreadID)
		return FALSE;

	// Already added thread? no need re-add
	for (int i = 0; i < _countof(dwThreadsID); i++)
	{
		if (dwThreadsID[i] == dwThreadID)
			return TRUE;
	}
	// Add new thread exception
	for (int i = 0; i < _countof(dwThreadsID); i++)
	{
		if (!dwThreadsID[i])
		{
			dwThreadsID[i] = dwThreadID;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CAntiCheat::IsGameModules(char* pszFile)
{
	char path[2048];
	GetModuleFileName(NULL, path, 2048);
	int iSizePathString = lstrlenA(path);
	for (int i = 0; i < iSizePathString; i++)
		path[i] = tolower(path[i]);
	std::string modulePath(path);
	std::string parentStr;
	parentStr = modulePath.replace(modulePath.length() - 8, modulePath.length(), "");

	char szModule[256] = { 0 };
	lstrcpyA(szModule, pszFile);
	int iSizeString = lstrlenA(szModule);
	for (int i = 0; i < iSizeString; i++)
		szModule[i] = tolower(szModule[i]);

	char* extract = strstr(szModule, (parentStr + "game.exe").c_str());
	if (extract)
	{
		return TRUE;
	}
	extract = strstr(szModule, "windows\\system");
	if (extract)
	{
		return TRUE;
	}
	extract = strstr(szModule, "windows\\syswow64");
	if (extract)
	{
		return TRUE;
	}
	extract = strstr(szModule, "\\gbpsv.exe");
	if (extract)
	{
		return  TRUE;
	}
	extract = strstr(szModule, "\\gbpinj.dll");
	if (extract)
	{
		return  TRUE;
	}
	extract = strstr(szModule, "\\ntdll.dll");
	if (extract)
	{
		return  TRUE;
	}
	extract = strstr(szModule, "\\d3dx9_36.dll");
	if (extract)
	{
		return  TRUE;
	}
	extract = strstr(szModule, "\\d3dx9_43.dll");
	if (extract)
	{
		return  TRUE;
	}

	return FALSE;
}

void CAntiCheat::ChecksumFunctionHandler()
{
	for (int i = 0; i < CHECKSUM_FUNCTION_TOTAL; i++)
	{
		// Have Checksum?
		if (psPacketChecksum && psPacketChecksum->sCheckSum[i].dwSize)
		{
			// Not mismatch checksum code?
			if (!ChecksumCheckMismatch(&psPacketChecksum->sCheckSum[i]))
			{
				// Send log hack checksum
				PacketLogCheat sPacket;
				sPacket.iLength = sizeof(PacketLogCheat);
				sPacket.iHeader = 0x6F6A0001;
				sPacket.iCheatID = 8954;
				sPacket.LParam = psPacketChecksum->sCheckSum[i].dwSize;
				sPacket.SParam = psPacketChecksum->sCheckSum[i].dwAddress;
				sPacket.EParam = psPacketChecksum->sCheckSum[i].dwCheckSum;
				sPacket.LxParam = dwChecksumRet;

				smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);

				break;
			}
		}
	}
}

DWORD WINAPI GetThreadStartAddress(__in HANDLE hThread) // by Echo
{
	NTSTATUS ntStatus;
	DWORD dwThreadStartAddr = 0;
	HANDLE hPeusdoCurrentProcess, hNewThreadHandle;
	NTQUERYINFOMATIONTHREAD NtQueryInformationThread;
	if (!bLoadedNTDLL)
	{
		LoadLibraryA("ntdll.dll");
		bLoadedNTDLL = true;
	}
	if ((NtQueryInformationThread = (NTQUERYINFOMATIONTHREAD)GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtQueryInformationThread")))
	{
		hPeusdoCurrentProcess = GetCurrentProcess();
		if (DuplicateHandle(hPeusdoCurrentProcess, hThread, hPeusdoCurrentProcess, &hNewThreadHandle, THREAD_QUERY_INFORMATION, FALSE, 0))
		{
			ntStatus = NtQueryInformationThread(hNewThreadHandle, ThreadQuerySetWin32StartAddress, &dwThreadStartAddr, sizeof(DWORD), NULL);
			CloseHandle(hNewThreadHandle);
			if (ntStatus != STATUS_SUCCESS) return 0;
		}

	}

	return dwThreadStartAddr;
}

BOOL MatchAddressToModule(__in DWORD dwProcId, __out_bcount(MAX_PATH + 1) LPTSTR lpstrModule, __in DWORD dwThreadStartAddr, __out_opt PDWORD pModuleStartAddr) // by Echo
{
	BOOL bRet = FALSE;
	HANDLE hSnapshot;
	MODULEENTRY32 moduleEntry32;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPALL, dwProcId);

	moduleEntry32.dwSize = sizeof(MODULEENTRY32);
	moduleEntry32.th32ModuleID = 1;

	if (Module32First(hSnapshot, &moduleEntry32))
	{
		if (dwThreadStartAddr >= (DWORD)moduleEntry32.modBaseAddr && dwThreadStartAddr <= ((DWORD)moduleEntry32.modBaseAddr + moduleEntry32.modBaseSize))
		{
			lstrcpyA(lpstrModule, moduleEntry32.szExePath);
		}
		else
		{
			while (Module32Next(hSnapshot, &moduleEntry32))
			{
				if (dwThreadStartAddr >= (DWORD)moduleEntry32.modBaseAddr && dwThreadStartAddr <= ((DWORD)moduleEntry32.modBaseAddr + moduleEntry32.modBaseSize))
				{
					lstrcpyA(lpstrModule, moduleEntry32.szExePath);
					break;
				}
			}
		}
	}

	if (pModuleStartAddr) *pModuleStartAddr = (DWORD)moduleEntry32.modBaseAddr;
	CloseHandle(hSnapshot);

	return bRet;
}


void CAntiCheat::FindHackDllThread()
{
	HANDLE hSnapshot, hThread;
	THREADENTRY32 threadEntry32;
	DWORD dwModuleBaseAddr, dwThreadStartAddr;
	TCHAR lpstrModuleName[MAX_PATH + 1] = { 0 };

	CRITICAL_SECTION	cSection;
	InitializeCriticalSection(&cSection);
	EnterCriticalSection(&cSection);


	if ((hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId())) == INVALID_HANDLE_VALUE)
		return;

	threadEntry32.dwSize = sizeof(THREADENTRY32);
	threadEntry32.cntUsage = 0;

	if (Thread32First(hSnapshot, &threadEntry32))
	{
		if (threadEntry32.th32OwnerProcessID == GetCurrentProcessId())
		{
			hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threadEntry32.th32ThreadID);
			dwThreadStartAddr = GetThreadStartAddress(hThread);
			MatchAddressToModule(GetCurrentProcessId(), lpstrModuleName, dwThreadStartAddr, &dwModuleBaseAddr);
			if (!IsGameModules(lpstrModuleName))
			{
				FreeLibraryAndExitThread(GetModuleHandleA(lpstrModuleName), 0);
			}
			CloseHandle(hThread);
		}
		while (Thread32Next(hSnapshot, &threadEntry32))
		{
			if (threadEntry32.th32OwnerProcessID == GetCurrentProcessId())
			{
				hThread = OpenThread(THREAD_QUERY_INFORMATION, FALSE, threadEntry32.th32ThreadID);
				dwThreadStartAddr = GetThreadStartAddress(hThread);
				MatchAddressToModule(GetCurrentProcessId(), lpstrModuleName, dwThreadStartAddr, &dwModuleBaseAddr);
				if (!IsGameModules(lpstrModuleName))
				{
					FreeLibraryAndExitThread(GetModuleHandleA(lpstrModuleName), 0);
				}
				CloseHandle(hThread);
			}
		}
	}
	LeaveCriticalSection(&cSection);
	DeleteCriticalSection(&cSection);
	CloseHandle(hSnapshot);
}

void CAntiCheat::WindowFunctionHandler()
{
	char szTempName[256];
	for (int i = 0; i < 50; i++)
	{
		if (lstrlenA(pcAntiCheat->psPacketWindowCheat->szaWindowName[i]) && FindWindowPartialName(pcAntiCheat->psPacketWindowCheat->szaWindowName[i], szTempName))
		{
			// Log Window Cheat
			PacketLogCheatEx sPacket;
			sPacket.iLength = sizeof(PacketLogCheatEx);
			sPacket.iHeader = 0x6F6A0001;
			sPacket.iCheatID = 8955;
			lstrcpyA(sPacket.szParam, szTempName);

			if (smWsockDataServer)
			{
				smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
			}
			else
				exit(0);
		}
	}
	return;
}

bool antidebug1()
{
	bool bRet = false;

	__asm
	{
		mov eax, dword ptr fs : [0x18]
		//  Get PEB structure address
		mov eax, dword ptr[eax + 0x30]
		// Check if isDebug byte is set
		cmp byte ptr[eax + 2], 0
		je blocEnd
		mov bRet, 1
		blocEnd :
		// etc ...
	}

	return bRet;
}

bool HasDebugger1()
{
__try { __asm INT 0x2D }
	__except (EXCEPTION_EXECUTE_HANDLER) { return false; }

	return false;
}

bool HasDebugger2()
{
	__try { __asm INT 0x03 }
	__except (EXCEPTION_EXECUTE_HANDLER) { return false; }

	return false;
}

bool HasDebugger3()
{
	CONTEXT ctx = { 0 };
	ctx.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	auto hThread = GetCurrentThread();

	if (GetThreadContext(hThread, &ctx) == 0)
	{
		return false;
	}

	//Debugger Found
	if (ctx.Dr0 != 0 || ctx.Dr1 != 0 || ctx.Dr2 != 0 || ctx.Dr3 != 0)
		return true;

	return false;
}

bool HasDebugger4()
{
	OutputDebugString("");

	//Debugger Found
	if (GetLastError() == 0)
		return true;

	return false;
}

bool HasDebugger5()
{
	__try { RaiseException(DBG_RIPEXCEPTION, 0, 0, 0); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return false; }

	//Debugger Found
	return true;
}


bool HasDebugger6()
{
#ifndef _DEBUG
	std::vector<const char*> drivers =
	{
		AY_OBFUSCATE("\\\\.\\EXTREM"),
		AY_OBFUSCATE("\\\\.\\ICEEXT"),
		AY_OBFUSCATE("\\\\.\\NDBGMSG.VXD"),
		AY_OBFUSCATE("\\\\.\\RING0"),
		AY_OBFUSCATE("\\\\.\\SIWVID"),
		AY_OBFUSCATE("\\\\.\\SYSER"),
		AY_OBFUSCATE("\\\\.\\TRW"),
		AY_OBFUSCATE("\\\\.\\SYSERBOOT") };

	for (const auto& driver : drivers)
	{
		auto h = CreateFileA(driver, 0, 0, 0, OPEN_EXISTING, 0, 0);
		if (h != INVALID_HANDLE_VALUE)
		{
			CloseHandle(h);

			//Debugger Found
			return true;
		}
	}
#endif
	return false;
}


bool HasDebugger7()
{
	unsigned char* pMem = nullptr;
	SYSTEM_INFO sysinfo = { 0 };
	DWORD OldProtect = 0;
	void* pAllocation = nullptr;

	GetSystemInfo(&sysinfo);
	pAllocation = VirtualAlloc(NULL, sysinfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	//Not Found
	if (pAllocation == NULL)
		return false;

	//Write a ret to the buffer (opcode 0xc3)
	pMem = (unsigned char*)pAllocation;
	*pMem = 0xc3;

	//Make the page a guard page         
	if (VirtualProtect(pAllocation, sysinfo.dwPageSize, PAGE_EXECUTE_READWRITE | PAGE_GUARD, &OldProtect) == 0)
		return false;

	__try
	{
		__asm
		{
			mov eax, pAllocation
			//This is the address we'll return to if we're under a debugger
			push MemBpBeingDebugged
			jmp eax //Exception or execution, which shall it be :D?
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		//The exception occured and no debugger was detected
		VirtualFree(pAllocation, NULL, MEM_RELEASE);
		return false;
	}

	__asm {MemBpBeingDebugged:}
	VirtualFree(pAllocation, NULL, MEM_RELEASE);

	return true;
}

bool HasDebugger8()
{
	__try
	{
		__asm __emit 0xF3 //0xF3 0x64 disassembles as PREFIX REP:
		__asm __emit 0x64
		__asm __emit 0xF1 //One byte INT 1
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return false;
	}

	return true;
}

bool HasDebugger9()
{
	BOOL bFound = FALSE;
	_asm
	{
		xor eax, eax;           //clear eax
		mov eax, fs: [0x30] ;   //Reference start of the PEB
		mov eax, [eax + 0x68];  //PEB+0x68 points to NtGlobalFlags
		and eax, 0x00000070;    //check three flags
		mov bFound, eax;        //Copy result into 'found'
	}

	if (bFound)
	{
		return true;
	}

	return false;
}

bool HasDebugger10()
{
	HANDLE hInvalid = (HANDLE)0xDEADBEEF; //An invalid handle
	BOOL bFound = FALSE;

	__try { CloseHandle(hInvalid); }
	__except (EXCEPTION_EXECUTE_HANDLER) { return true; }

	return false;
}

DWORD dwTickCount = 0;
DWORD dwTickCount2 = 0;

bool bdebugfound = false;
extern DWORD dwtickhack;

bool __forceinline IsHookAPI(BYTE* pbFunction)
{
	//JMP
	if (pbFunction[0] == 0xE9)
		return true;
	//JMP DWORD PTR DS:[...]
	if (pbFunction[0] == 0xFF && pbFunction[1] == 0x25)
		return true;
	return false;
}

void GetProcId(char* ProcName)
{
	PROCESSENTRY32 pe32;
	HANDLE hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			if (strcmp(pe32.szExeFile, ProcName) == 0)
			{
				HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, pe32.th32ProcessID);

				if (hProcess)
					TerminateProcess(hProcess, NULL);
			}


		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);
}

void ClasseCheckPross()
{
	GetProcId("bvkhex.exe");
	GetProcId("cheatengine-x86_64.exe");
	GetProcId("HxD.exe");
	GetProcId("procexp2.exe");
	GetProcId("Hide Toolz3.3.3.exe");
	GetProcId("SbieSvc.exe");
	GetProcId("SbieSvc*32.exe"); 
	GetProcId("SbieSvc*32.exe");
	GetProcId("SbieCtrl.exe");
	GetProcId("Start.exe");
}

DWORD WINAPI CAntiCheat::ThreadAntiCheatHandler()
{
#ifdef _DEBUG
	return 0;
#endif

	//New thread exception
	pcAntiCheat->AddNewThreadException(GetCurrentThreadId());

	while (true)
	{
		//more than 15s?
		if ((GetTickCount() - dwTickCount) >= 15000)
		{
			pcAntiCheat->ChecksumFunctionHandler();
			//pcAntiCheat->FindHackDllThread();

			if (pcAntiCheat->psPacketWindowCheat)
				pcAntiCheat->WindowFunctionHandler();

			// Speed Hacker
			if (IsHookAPI((BYTE*)&GetTickCount))
			{
				PacketLogCheatEx sPacket;
				sPacket.iLength = sizeof(PacketLogCheatEx);
				sPacket.iHeader = 0x6F6A0001;
				sPacket.iCheatID = 8964;
				lstrcpyA(sPacket.szParam, "");

				if (smWsockDataServer)
				{
					smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
				}
			}

			// Regen Hacker
			if (IsHookAPI((BYTE*)&sinSetLife))
			{
				PacketLogCheat sPacket;
				sPacket.iLength = sizeof(PacketLogCheat);
				sPacket.iHeader = 0x6F6A0001;
				sPacket.iCheatID = 8961;
				sPacket.SParam = 100;

				if (smWsockDataServer)
				{
					smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
				}
			}

			if (IsHookAPI((BYTE*)&sinSetMana))
			{
				PacketLogCheat sPacket;
				sPacket.iLength = sizeof(PacketLogCheat);
				sPacket.iHeader = 0x6F6A0001;
				sPacket.iCheatID = 8961;
				sPacket.SParam = 100;

				if (smWsockDataServer)
				{
					smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
				}
			}

			if (IsHookAPI((BYTE*)&sinSetStamina))
			{
				PacketLogCheat sPacket;
				sPacket.iLength = sizeof(PacketLogCheat);
				sPacket.iHeader = 0x6F6A0001;
				sPacket.iCheatID = 8961;
				sPacket.SParam = 100;

				if (smWsockDataServer)
				{
					smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
				}
			}

			/*
			// Dll Inject
			if (pcAntiCheat->GetModuleName(GetCurrentThreadId(), "game.exe", 1000))
			{
				PacketLogCheatEx sPacket;
				sPacket.iLength = sizeof(PacketLogCheatEx);
				sPacket.iHeader = 0x6F6A0001;
				sPacket.iCheatID = 8963;
				lstrcpyA(sPacket.szParam, "");

				if (smWsockDataServer)
				{
					smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
				}
			} */

			dwTickCount = GetTickCount();
		}

		extern int debuggerID;

		if ((GetTickCount() - dwTickCount2) >= 2000)
		{
			if (antidebug1())
			{
				bdebugfound = true;
				debuggerID = 1;
			}

			ClasseCheckPross();

#ifndef GAME_GM
			if (HasDebugger1())
			{
				bdebugfound = true;
				debuggerID = 2;
			}

			if (HasDebugger2())
			{
				bdebugfound = true;
				debuggerID = 3;
			}

			if (HasDebugger3())
			{
				bdebugfound = true;
				debuggerID = 4;
			}

			//if (HasDebugger4())
			//{
			//	bdebugfound = true;
			//	debuggerID = 5;
			//}

			if (HasDebugger5())
			{
				bdebugfound = true;
				debuggerID = 6;
			}

			if (HasDebugger6())
			{
				bdebugfound = true;
				debuggerID = 7;
			}

			if (HasDebugger7())
			{
				bdebugfound = true;
				debuggerID = 8;
			}

			if (HasDebugger8())
			{
				bdebugfound = true;
				debuggerID = 9;
			}

			if (HasDebugger9())
			{
				bdebugfound = true;
				debuggerID = 10;
			}

			if (HasDebugger10())
			{
				bdebugfound = true;
				debuggerID = 11;
			}

			if (IsDebuggerPresent())
			{
				bdebugfound = true;
				debuggerID = 12;
		}
#endif

			dwtickhack = GetTickCount();
			dwTickCount2 = GetTickCount();
	}
	}

	return TRUE;
}


void __cdecl CAntiCheat::CheckThread()
{
	DWORD dwThreadID = GetCurrentThreadId();
	BOOL bThreadFound = FALSE;

	// Find threads exception
	for (int i = 0; i < _countof(pcAntiCheat->dwThreadsID); i++)
	{
		if (pcAntiCheat->dwThreadsID[i] == dwThreadID)
		{
			bThreadFound = TRUE;
			break;
		}
	}

	// Not thread in exception?
	if (!bThreadFound)
	{
		// Send log hack module thread ( or created remote thread in game process )
		char szModuleName[128] = { 0 };
		PacketLogCheat sPacket;
		sPacket.iLength = sizeof(PacketLogCheat);
		sPacket.iHeader = 0x6F6A0001;
		sPacket.iCheatID = 8720;
		sPacket.LParam = dwThreadID;

		smWsockDataServer->Send((char*)&sPacket, sPacket.iLength, TRUE);
	}
}

void CAntiCheat::ReceivePacketHandler(PacketChecksumFunctionList* psPacket)
{
	if (!psPacketChecksum)
		psPacketChecksum = new PacketChecksumFunctionList;

	ZeroMemory(psPacketChecksum, sizeof(PacketChecksumFunctionList));

	CopyMemory(psPacketChecksum, psPacket, sizeof(PacketChecksumFunctionList));

	if (!hThread)
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadAntiCheatHandler, NULL, NULL, NULL);
}

void CAntiCheat::HandlePacket(PacketWindowList* psPacket)
{
	if (!psPacketWindowCheat)
		psPacketWindowCheat = new PacketWindowList;

	ZeroMemory(psPacketWindowCheat, sizeof(PacketWindowList));

	CopyMemory(psPacketWindowCheat, psPacket, sizeof(PacketWindowList));
}