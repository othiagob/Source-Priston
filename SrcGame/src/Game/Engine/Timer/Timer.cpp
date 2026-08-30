#include "Engine/Timer/Timer.h"

CTimer::CTimer()
{
	LARGE_INTEGER liFrequency;
	QueryPerformanceFrequency(&liFrequency);
	dFrequency = (double)liFrequency.QuadPart;

	bMultiCore = FALSE;

	liLastTick.QuadPart = 0;
}

double CTimer::Update()
{
	DWORD_PTR dwAffinityMask;

	if (bMultiCore)
		dwAffinityMask = SetThreadAffinityMask(GetCurrentThread(), 1);

	LARGE_INTEGER liNewTick;
	QueryPerformanceCounter(&liNewTick);

	if (bMultiCore)
		SetThreadAffinityMask(GetCurrentThread(), dwAffinityMask);

	double dUpdate = (((double)(liNewTick.QuadPart - liLastTick.QuadPart)) * 1000.0f) / dFrequency;
	liLastTick = liNewTick;

	return dUpdate;
}