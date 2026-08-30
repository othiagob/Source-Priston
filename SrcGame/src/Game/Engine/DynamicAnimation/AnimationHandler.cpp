#include "AnimationHandler.h"

#include <ostream>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <utility>
#include <algorithm>
#include <any>
#include <regex>
#include <memory>
#include <stack>
#include <mutex>
#include <deque>
#include <sstream>
#include <locale>
#include <cctype>
#include <queue>
#include <clocale>
#include <unordered_map>
#include <array>
#include <limits>
#include <chrono>
#include <random>
#include <filesystem>
#include <future>

#include <Windows.h>
#include <ShellAPI.h>
#include <tlhelp32.h>
#include <Shobjidl.h>
#include <intrin.h>
#include <Psapi.h>
#include <immintrin.h>
#include <winternl.h>
#include <assert.h>
#include <Shlwapi.h>
#include <winnt.h>
#include <mmsystem.h>

#include "..//../smLib3d/smType.h"
#include "..//../smLib3d/smTexture.h"
#include "..//../smLib3d/smStage3d.h"
#include "..//../character.h"

#include "File.h"

//Delta3D:
#include "IGraphics.h"
#include "ICore.h"
#include "IIO.h"
#include "IMath.h"
#include "IResource.h"
#include "IThirdParty.h"
#include "ILegacy.h"

#include "Import.h"
#include "Export.h"

using namespace Delta3D;

extern smDPAT* LoadINX(char* szFileName);
extern void smASE_SetPhysique(smPAT3D* p);
extern smPAT3D* smASE_ReadBone(char* file);

extern std::vector<std::string> split(const std::string& str, const std::string& delim);

void AddINXAnimation(RawINXFile* psRawFile, RawINXAnimation* psRawAnimation)
{
	unsigned int uIndex = psRawFile->uAnimationCount;

	if (uIndex >= _countof(psRawFile->saAnimation))
		return;

	CopyMemory(psRawFile->saAnimation + uIndex, psRawAnimation, sizeof(RawINXAnimation));

	psRawFile->uAnimationCount++;
}

bool DecryptBoneFile(smPAT3D* pcModelAnimation)
{
	if (pcModelAnimation)
	{
		for (UINT u = 0; u < pcModelAnimation->nObj3d; u++)
		{
			smOBJ3D* pcMesh = pcModelAnimation->obj3d[u];

			if (pcMesh)
			{
				for (int i = 0; i < pcMesh->TmRotCnt; i++)
				{
					smTM_ROT* psRot = pcMesh->TmRot + i;

					if (psRot)
					{
						float fOldX = psRot->x;

						psRot->x = psRot->y;
						psRot->y = fOldX;
					}
				}
			}
		}
	}

	return true;
}

smTM_ROT* InjectKeyRotation(smTM_ROT* pArray, int iArraySize, smTM_ROT& sInjectThis, int iFirstFrame, int iLastFrame, int& iIndexOut)
{
	for (int i = 0; i < iArraySize - 1; i++)
	{
		smTM_ROT* p1 = pArray + i;
		smTM_ROT* p2 = pArray + (i + 1);

		if (iFirstFrame > p1->frame && iLastFrame < p2->frame)
		{
			int iIndex = i + 1;

			smTM_ROT* pNewArray = new smTM_ROT[iArraySize + 2];
			CopyMemory(pNewArray, pArray, sizeof(smTM_ROT) * (i + 1));

			sInjectThis.frame = iFirstFrame;
			pNewArray[iIndex] = sInjectThis;

			sInjectThis.frame = iLastFrame;
			pNewArray[iIndex + 1] = sInjectThis;

			CopyMemory(pNewArray + i + 3, pArray + (i + 1), iArraySize - (i + 1));
			delete[] pArray;
			return pNewArray;
		}
	}

	return nullptr;
}

int FindKeyRotationIndex(smOBJ3D* psRawObject, int iStartFrame, int iEndFrame)
{
	int iIndex = 0;

	if (psRawObject->TmRotCnt > 0)
	{
		if (iEndFrame < psRawObject->TmRot[0].frame)
			return 0;

		if (iStartFrame >= psRawObject->TmRot[psRawObject->TmRotCnt - 1].frame)
			return psRawObject->TmRotCnt;

		for (int i = 1; i < psRawObject->TmRotCnt; i++)
		{
			smTM_ROT* p1 = (smTM_ROT*)psRawObject->TmRot + (i - 1);
			smTM_ROT* p2 = (smTM_ROT*)psRawObject->TmRot + i;

			if ((iStartFrame >= p1->frame) && (iEndFrame <= p2->frame))
				return i;
		}
	}

	return iIndex;
}

void InjectKeyRotation(smOBJ3D* psRawObject, int iIndex, int iFrame, smTM_ROT* psKeyRotation, smFMATRIX* psKeyRotationMatrix)
{
	if (iIndex == -1)
		iIndex = psRawObject->TmRotCnt;

	{
		smTM_ROT* paOldArray = psRawObject->TmRot;
		psRawObject->TmRot = new smTM_ROT[psRawObject->TmRotCnt + 1];

		if (iIndex > 0)
			CopyMemory(psRawObject->TmRot, paOldArray, sizeof(smTM_ROT) * iIndex);

		psRawObject->TmRot[iIndex] = *psKeyRotation;
		psRawObject->TmRot[iIndex].frame = iFrame;

		if (psRawObject->TmRotCnt > iIndex)
			CopyMemory(psRawObject->TmRot + iIndex + 1, paOldArray + iIndex, sizeof(smTM_ROT) * (psRawObject->TmRotCnt - iIndex));

		delete[] paOldArray;
	}

	{
		smFMATRIX* paOldArray = psRawObject->TmPrevRot;
		psRawObject->TmPrevRot = new smFMATRIX[psRawObject->TmRotCnt + 1];

		if (iIndex > 0)
			CopyMemory(psRawObject->TmPrevRot, paOldArray, sizeof(smFMATRIX) * iIndex);

		psRawObject->TmPrevRot[iIndex] = *psKeyRotationMatrix;

		if (psRawObject->TmRotCnt > iIndex)
			CopyMemory(psRawObject->TmPrevRot + iIndex + 1, paOldArray + iIndex, sizeof(smFMATRIX) * (psRawObject->TmRotCnt - iIndex));

		delete[] paOldArray;
	}

	psRawObject->TmRotCnt++;
}

bool MergeKeyRotations(smOBJ3D* psRawObject)
{
	int iNewSize = psRawObject->TmRotCnt;

	for (int i = 0; i < (iNewSize - 3); )
	{
		smTM_ROT* p[4];
		p[0] = psRawObject->TmRot + (i + 0);
		p[1] = psRawObject->TmRot + (i + 1);
		p[2] = psRawObject->TmRot + (i + 2);
		p[3] = psRawObject->TmRot + (i + 3);

		smFMATRIX* pm[4];
		pm[0] = psRawObject->TmPrevRot + (i + 0);
		pm[1] = psRawObject->TmPrevRot + (i + 1);
		pm[2] = psRawObject->TmPrevRot + (i + 2);
		pm[3] = psRawObject->TmPrevRot + (i + 3);

		if ((p[0]->x != 0.0f) || (p[0]->y != 0.0f) || (p[0]->z != 0.0f) || (p[0]->w != 0.0f))
		{
			i++;
			continue;
		}

		if ((p[1]->x != 0.0f) || (p[1]->y != 0.0f) || (p[1]->z != 0.0f) || (p[1]->w != 0.0f))
		{
			i++;
			continue;
		}

		if ((p[2]->x != 0.0f) || (p[2]->y != 0.0f) || (p[2]->z != 0.0f) || (p[2]->w != 0.0f))
		{
			i++;
			continue;
		}

		if ((p[3]->x != 0.0f) || (p[3]->y != 0.0f) || (p[3]->z != 0.0f) || (p[3]->w != 0.0f))
		{
			i++;
			continue;
		}

		if (memcmp(&pm[0]->_11, &pm[1]->_11, sizeof(smFMATRIX)) != 0)
		{
			i++;
			continue;
		}

		if (memcmp(&pm[0]->_11, &pm[2]->_11, sizeof(smFMATRIX)) != 0)
		{
			i++;
			continue;
		}

		if (memcmp(&pm[0]->_11, &pm[3]->_11, sizeof(smFMATRIX)) != 0)
		{
			i++;
			continue;
		}

		p[1]->frame = p[3]->frame;

		for (int j = (i + 4); j < iNewSize; j++)
		{
			smTM_ROT* c1 = psRawObject->TmRot + (j - 2);
			smTM_ROT* c2 = psRawObject->TmRot + (j + 0);

			smFMATRIX* m1 = psRawObject->TmPrevRot + (j - 2);
			smFMATRIX* m2 = psRawObject->TmPrevRot + (j + 0);

			CopyMemory(c1, c2, sizeof(smTM_ROT));
			CopyMemory(m1, m2, sizeof(smFMATRIX));
		}

		iNewSize -= 2;
	}

	if (iNewSize < psRawObject->TmRotCnt)
	{
		//WRITEDBG( "Merged Key Rotations (%d -> %d)", psRawObject->iKeyRotationCount, iNewSize );
		{
			smTM_ROT* paOldArray = psRawObject->TmRot;
			psRawObject->TmRot = new smTM_ROT[iNewSize];
			CopyMemory(psRawObject->TmRot, paOldArray, sizeof(smTM_ROT) * iNewSize);
			delete[] paOldArray;
		}

		{
			smFMATRIX* paOldArray = psRawObject->TmPrevRot;
			psRawObject->TmPrevRot = new smFMATRIX[iNewSize];
			CopyMemory(psRawObject->TmPrevRot, paOldArray, sizeof(smFMATRIX) * iNewSize);
			delete[] paOldArray;
		}

		psRawObject->TmRotCnt = iNewSize;

		return true;
	}

	return false;
}

bool FixKeyRotationArray(smOBJ3D* psRawObject, int iLastFrame)
{
	bool bReturn = false;

	if (psRawObject->TmRotCnt > 0)
	{
		if (psRawObject->TmRot[psRawObject->TmRotCnt - 1].frame != iLastFrame)
		{
			{
				smTM_ROT* paOldArray = psRawObject->TmRot;
				psRawObject->TmRot = new smTM_ROT[psRawObject->TmRotCnt + 1];
				CopyMemory(psRawObject->TmRot, paOldArray, sizeof(smTM_ROT) * psRawObject->TmRotCnt);
				psRawObject->TmRot[psRawObject->TmRotCnt] = paOldArray[psRawObject->TmRotCnt - 1];
				psRawObject->TmRot[psRawObject->TmRotCnt].frame = iLastFrame;
				delete[] paOldArray;
				bReturn = true;
			}

			{
				smFMATRIX* paOldArray = psRawObject->TmPrevRot;
				psRawObject->TmPrevRot = new smFMATRIX[psRawObject->TmRotCnt + 1];
				CopyMemory(psRawObject->TmPrevRot, paOldArray, sizeof(smFMATRIX) * psRawObject->TmRotCnt);
				psRawObject->TmPrevRot[psRawObject->TmRotCnt] = paOldArray[psRawObject->TmRotCnt - 1];
				delete[] paOldArray;
				bReturn = true;
			}

			psRawObject->TmRotCnt++;
		}
	}

	return bReturn;
}

void InjectKeyPosition(smOBJ3D* psRawObject, int iIndex, int iFrame, smTM_POS* psKeyPosition)
{
	if (iIndex == -1)
		iIndex = psRawObject->TmPosCnt;

	smTM_POS* paOldArray = psRawObject->TmPos;
	psRawObject->TmPos = new smTM_POS[psRawObject->TmPosCnt + 1];

	if (iIndex > 0)
		CopyMemory(psRawObject->TmPos, paOldArray, sizeof(smTM_POS) * iIndex);

	psRawObject->TmPos[iIndex] = *psKeyPosition;
	psRawObject->TmPos[iIndex].frame = iFrame;

	if (psRawObject->TmPosCnt > iIndex)
		CopyMemory(psRawObject->TmPos + iIndex + 1, paOldArray + iIndex, sizeof(smTM_POS) * (psRawObject->TmPosCnt - iIndex));

	delete[] paOldArray;

	psRawObject->TmPosCnt++;
}

bool MergeKeyPositions(smOBJ3D* psRawObject)
{
	int iNewSize = psRawObject->TmPosCnt;

	for (int i = 0; i < (iNewSize - 3); )
	{
		smTM_POS* p[4];
		p[0] = psRawObject->TmPos + (i + 0);
		p[1] = psRawObject->TmPos + (i + 1);
		p[2] = psRawObject->TmPos + (i + 2);
		p[3] = psRawObject->TmPos + (i + 3);

		if ((p[0]->x != p[1]->x) || (p[0]->y != p[1]->y) || (p[0]->z != p[1]->z))
		{
			i++;
			continue;
		}

		if ((p[0]->x != p[2]->x) || (p[0]->y != p[2]->y) || (p[0]->z != p[2]->z))
		{
			i++;
			continue;
		}

		if ((p[0]->x != p[3]->x) || (p[0]->y != p[3]->y) || (p[0]->z != p[3]->z))
		{
			i++;
			continue;
		}

		p[1]->frame = p[3]->frame;

		for (int j = (i + 4); j < iNewSize; j++)
		{
			smTM_POS* c1 = psRawObject->TmPos + (j - 2);
			smTM_POS* c2 = psRawObject->TmPos + (j + 0);

			CopyMemory(c1, c2, sizeof(smTM_POS));
		}

		iNewSize -= 2;
	}

	if (iNewSize < psRawObject->TmPosCnt)
	{
		//WRITEDBG( "Merged Key Positions (%d -> %d)", psRawObject->iKeyPositionCount, iNewSize );
		smTM_POS* paOldArray = psRawObject->TmPos;

		psRawObject->TmPos = new smTM_POS[iNewSize];
		CopyMemory(psRawObject->TmPos, paOldArray, sizeof(smTM_POS) * iNewSize);
		psRawObject->TmPosCnt = iNewSize;
		delete[] paOldArray;

		return true;
	}

	return false;
}

bool FixKeyPositionArray(smOBJ3D* psRawObject, int iLastFrame)
{
	if (psRawObject->TmPosCnt > 0)
	{
		if (psRawObject->TmPos[psRawObject->TmPosCnt - 1].frame != iLastFrame)
		{
			smTM_POS* paOldArray = psRawObject->TmPos;
			psRawObject->TmPos = new smTM_POS[psRawObject->TmPosCnt + 1];
			CopyMemory(psRawObject->TmPos, paOldArray, sizeof(smTM_POS) * psRawObject->TmPosCnt);
			psRawObject->TmPos[psRawObject->TmPosCnt] = paOldArray[psRawObject->TmPosCnt - 1];
			psRawObject->TmPos[psRawObject->TmPosCnt].frame = iLastFrame;
			psRawObject->TmPosCnt++;
			delete[] paOldArray;

			return true;
		}
	}

	return false;
}

int FindKeyScaleIndex(smOBJ3D* psRawObject, int iStartFrame, int iEndFrame)
{
	int iIndex = 0;

	if (psRawObject->TmScaleCnt > 0)
	{
		if (iEndFrame < psRawObject->TmScale[0].frame)
			return 0;

		if (iStartFrame >= psRawObject->TmScale[psRawObject->TmScaleCnt - 1].frame)
			return psRawObject->TmScaleCnt;

		for (int i = 1; i < psRawObject->TmScaleCnt; i++)
		{
			smTM_SCALE* p1 = psRawObject->TmScale + (i - 1);
			smTM_SCALE* p2 = psRawObject->TmScale + i;

			if ((iStartFrame >= p1->frame) && (iEndFrame <= p2->frame))
				return i;
		}
	}

	return iIndex;
}

void InjectKeyScale(smOBJ3D* psRawObject, int iIndex, int iFrame, smTM_SCALE* psKeyScale)
{
	if (iIndex == -1)
		iIndex = psRawObject->TmScaleCnt;

	smTM_SCALE* paOldArray = psRawObject->TmScale;
	psRawObject->TmScale = new smTM_SCALE[psRawObject->TmScaleCnt + 1];

	if (iIndex > 0)
		CopyMemory(psRawObject->TmScale, paOldArray, sizeof(smTM_SCALE) * iIndex);

	psRawObject->TmScale[iIndex] = *psKeyScale;
	psRawObject->TmScale[iIndex].frame = iFrame;

	if (psRawObject->TmScaleCnt > iIndex)
		CopyMemory(psRawObject->TmScale + iIndex + 1, paOldArray + iIndex, sizeof(smTM_SCALE) * (psRawObject->TmScaleCnt - iIndex));

	delete[] paOldArray;
	psRawObject->TmScaleCnt++;
}

bool MergeKeyScalings(smOBJ3D* psRawObject)
{
	int iNewSize = psRawObject->TmScaleCnt;

	for (int i = 0; i < (iNewSize - 3); )
	{
		smTM_SCALE* p[4];
		p[0] = psRawObject->TmScale + (i + 0);
		p[1] = psRawObject->TmScale + (i + 1);
		p[2] = psRawObject->TmScale + (i + 2);
		p[3] = psRawObject->TmScale + (i + 3);

		if ((p[0]->x != p[1]->x) || (p[0]->y != p[1]->y) || (p[0]->z != p[1]->z))
		{
			i++;
			continue;
		}

		if ((p[0]->x != p[2]->x) || (p[0]->y != p[2]->y) || (p[0]->z != p[2]->z))
		{
			i++;
			continue;
		}

		if ((p[0]->x != p[3]->x) || (p[0]->y != p[3]->y) || (p[0]->z != p[3]->z))
		{
			i++;
			continue;
		}

		p[1]->frame = p[3]->frame;

		for (int j = (i + 4); j < iNewSize; j++)
		{
			smTM_SCALE* c1 = psRawObject->TmScale + (j - 2);
			smTM_SCALE* c2 = psRawObject->TmScale + (j + 0);

			CopyMemory(c1, c2, sizeof(IO::SMD::KeyScale));
		}

		iNewSize -= 2;
	}

	if (iNewSize < psRawObject->TmScaleCnt)
	{
		//WRITEDBG( "Merged Key Scales (%d -> %d)", psRawObject->iKeyScaleCount, iNewSize );
		smTM_SCALE* paOldArray = psRawObject->TmScale;
		psRawObject->TmScale = new smTM_SCALE[iNewSize];
		CopyMemory(psRawObject->TmScale, paOldArray, sizeof(smTM_SCALE) * iNewSize);
		psRawObject->TmScaleCnt = iNewSize;
		delete[] paOldArray;

		return true;
	}

	return false;
}

bool FixKeyScalingArray(smOBJ3D* psRawObject, int iLastFrame)
{
	if (psRawObject->TmScaleCnt > 0)
	{
		if (psRawObject->TmScale[psRawObject->TmScaleCnt - 1].frame != iLastFrame)
		{
			smTM_SCALE* paOldArray = psRawObject->TmScale;
			psRawObject->TmScale = new smTM_SCALE[psRawObject->TmScaleCnt + 1];
			CopyMemory(psRawObject->TmScale, paOldArray, sizeof(smTM_SCALE) * psRawObject->TmScaleCnt);
			psRawObject->TmScale[psRawObject->TmScaleCnt] = paOldArray[psRawObject->TmScaleCnt - 1];
			psRawObject->TmScale[psRawObject->TmScaleCnt].frame = iLastFrame;
			psRawObject->TmScaleCnt++;
			delete[] paOldArray;
			return true;
		}
	}

	return false;
}

int FindKeyRotationIndex(RawSMDObject* psRawObject, int iStartFrame, int iEndFrame)
{
	int iIndex = 0;

	if (psRawObject->iKeyRotationCount > 0)
	{
		if (iEndFrame < psRawObject->paKeyRotation[0].iFrame)
			return 0;

		if (iStartFrame >= psRawObject->paKeyRotation[psRawObject->iKeyRotationCount - 1].iFrame)
			return psRawObject->iKeyRotationCount;

		for (int i = 1; i < psRawObject->iKeyRotationCount; i++)
		{
			RawSMDKeyRotation* p1 = psRawObject->paKeyRotation + (i - 1);
			RawSMDKeyRotation* p2 = psRawObject->paKeyRotation + i;

			if ((iStartFrame >= p1->iFrame) && (iEndFrame <= p2->iFrame))
				return i;
		}
	}

	return iIndex;
}

BOOL MergeKeyRotations(RawSMDObject* psRawObject)
{
	int iNewSize = psRawObject->iKeyRotationCount;

	for (int i = 0; i < (iNewSize - 3); )
	{
		RawSMDKeyRotation* p[4];
		p[0] = psRawObject->paKeyRotation + (i + 0);
		p[1] = psRawObject->paKeyRotation + (i + 1);
		p[2] = psRawObject->paKeyRotation + (i + 2);
		p[3] = psRawObject->paKeyRotation + (i + 3);

		RawSMDMatrixF* pm[4];
		pm[0] = psRawObject->paRotation + (i + 0);
		pm[1] = psRawObject->paRotation + (i + 1);
		pm[2] = psRawObject->paRotation + (i + 2);
		pm[3] = psRawObject->paRotation + (i + 3);

		if ((p[0]->fX != 0.0f) || (p[0]->fY != 0.0f) || (p[0]->fZ != 0.0f) || (p[0]->fW != 0.0f))
		{
			i++;
			continue;
		}

		if ((p[1]->fX != 0.0f) || (p[1]->fY != 0.0f) || (p[1]->fZ != 0.0f) || (p[1]->fW != 0.0f))
		{
			i++;
			continue;
		}

		if ((p[2]->fX != 0.0f) || (p[2]->fY != 0.0f) || (p[2]->fZ != 0.0f) || (p[2]->fW != 0.0f))
		{
			i++;
			continue;
		}

		if ((p[3]->fX != 0.0f) || (p[3]->fY != 0.0f) || (p[3]->fZ != 0.0f) || (p[3]->fW != 0.0f))
		{
			i++;
			continue;
		}

		if (memcmp(pm[0]->f, pm[1]->f, sizeof(RawSMDMatrixF)) != 0)
		{
			i++;
			continue;
		}

		if (memcmp(pm[0]->f, pm[2]->f, sizeof(RawSMDMatrixF)) != 0)
		{
			i++;
			continue;
		}

		if (memcmp(pm[0]->f, pm[3]->f, sizeof(RawSMDMatrixF)) != 0)
		{
			i++;
			continue;
		}

		p[1]->iFrame = p[3]->iFrame;

		for (int j = (i + 4); j < iNewSize; j++)
		{
			RawSMDKeyRotation* c1 = psRawObject->paKeyRotation + (j - 2);
			RawSMDKeyRotation* c2 = psRawObject->paKeyRotation + (j + 0);

			RawSMDMatrixF* m1 = psRawObject->paRotation + (j - 2);
			RawSMDMatrixF* m2 = psRawObject->paRotation + (j + 0);

			CopyMemory(c1, c2, sizeof(RawSMDKeyRotation));
			CopyMemory(m1, m2, sizeof(RawSMDMatrixF));
		}

		iNewSize -= 2;
	}

	if (iNewSize < psRawObject->iKeyRotationCount)
	{
		//WRITEDBG( "Merged Key Rotations (%d -> %d)", psRawObject->iKeyRotationCount, iNewSize );

		{
			RawSMDKeyRotation* paOldArray = psRawObject->paKeyRotation;
			psRawObject->paKeyRotation = new RawSMDKeyRotation[iNewSize];
			CopyMemory(psRawObject->paKeyRotation, paOldArray, sizeof(RawSMDKeyRotation) * iNewSize);
			delete[] paOldArray;
		}

		{
			RawSMDMatrixF* paOldArray = psRawObject->paRotation;
			psRawObject->paRotation = new RawSMDMatrixF[iNewSize];
			CopyMemory(psRawObject->paRotation, paOldArray, sizeof(RawSMDMatrixF) * iNewSize);
			delete[] paOldArray;
		}

		psRawObject->iKeyRotationCount = iNewSize;

		return TRUE;
	}

	return FALSE;
}

BOOL FixKeyRotationArray(RawSMDObject* psRawObject, int iLastFrame)
{
	BOOL bReturn = FALSE;

	if (psRawObject->iKeyRotationCount > 0)
	{
		if (psRawObject->paKeyRotation[psRawObject->iKeyRotationCount - 1].iFrame != iLastFrame)
		{
			{
				RawSMDKeyRotation* paOldArray = psRawObject->paKeyRotation;
				psRawObject->paKeyRotation = new RawSMDKeyRotation[psRawObject->iKeyRotationCount + 1];
				CopyMemory(psRawObject->paKeyRotation, paOldArray, sizeof(RawSMDKeyRotation) * psRawObject->iKeyRotationCount);
				psRawObject->paKeyRotation[psRawObject->iKeyRotationCount] = paOldArray[psRawObject->iKeyRotationCount - 1];
				psRawObject->paKeyRotation[psRawObject->iKeyRotationCount].iFrame = iLastFrame;
				delete[] paOldArray;

				bReturn = TRUE;
			}

			{
				RawSMDMatrixF* paOldArray = psRawObject->paRotation;
				psRawObject->paRotation = new RawSMDMatrixF[psRawObject->iKeyRotationCount + 1];
				CopyMemory(psRawObject->paRotation, paOldArray, sizeof(RawSMDMatrixF) * psRawObject->iKeyRotationCount);
				psRawObject->paRotation[psRawObject->iKeyRotationCount] = paOldArray[psRawObject->iKeyRotationCount - 1];
				delete[] paOldArray;

				bReturn = TRUE;
			}

			psRawObject->iKeyRotationCount++;
		}
	}

	return bReturn;
}

int FindKeyPositionIndex(RawSMDObject* psRawObject, int iStartFrame, int iEndFrame)
{
	int iIndex = 0;

	if (psRawObject->iKeyPositionCount > 0)
	{
		if (iEndFrame < psRawObject->paKeyPosition[0].iFrame)
			return 0;

		if (iStartFrame >= psRawObject->paKeyPosition[psRawObject->iKeyPositionCount - 1].iFrame)
			return psRawObject->iKeyPositionCount;

		for (int i = 1; i < psRawObject->iKeyPositionCount; i++)
		{
			RawSMDKeyPosition* p1 = psRawObject->paKeyPosition + (i - 1);
			RawSMDKeyPosition* p2 = psRawObject->paKeyPosition + i;

			if ((iStartFrame >= p1->iFrame) && (iEndFrame <= p2->iFrame))
				return i;
		}
	}

	return iIndex;
}

void InjectKeyPosition(RawSMDObject* psRawObject, int iIndex, int iFrame, RawSMDKeyPosition* psKeyPosition)
{
	if (iIndex == -1)
		iIndex = psRawObject->iKeyPositionCount;

	RawSMDKeyPosition* paOldArray = psRawObject->paKeyPosition;
	psRawObject->paKeyPosition = new RawSMDKeyPosition[psRawObject->iKeyPositionCount + 1];

	if (iIndex > 0)
		CopyMemory(psRawObject->paKeyPosition, paOldArray, sizeof(RawSMDKeyPosition) * iIndex);

	psRawObject->paKeyPosition[iIndex] = *psKeyPosition;
	psRawObject->paKeyPosition[iIndex].iFrame = iFrame;

	if (psRawObject->iKeyPositionCount > iIndex)
		CopyMemory(psRawObject->paKeyPosition + iIndex + 1, paOldArray + iIndex, sizeof(RawSMDKeyPosition) * (psRawObject->iKeyPositionCount - iIndex));

	delete[] paOldArray;

	psRawObject->iKeyPositionCount++;
}

BOOL MergeKeyPositions(RawSMDObject* psRawObject)
{
	int iNewSize = psRawObject->iKeyPositionCount;

	for (int i = 0; i < (iNewSize - 3); )
	{
		RawSMDKeyPosition* p[4];
		p[0] = psRawObject->paKeyPosition + (i + 0);
		p[1] = psRawObject->paKeyPosition + (i + 1);
		p[2] = psRawObject->paKeyPosition + (i + 2);
		p[3] = psRawObject->paKeyPosition + (i + 3);

		if ((p[0]->fX != p[1]->fX) || (p[0]->fY != p[1]->fY) || (p[0]->fZ != p[1]->fZ))
		{
			i++;
			continue;
		}

		if ((p[0]->fX != p[2]->fX) || (p[0]->fY != p[2]->fY) || (p[0]->fZ != p[2]->fZ))
		{
			i++;
			continue;
		}

		if ((p[0]->fX != p[3]->fX) || (p[0]->fY != p[3]->fY) || (p[0]->fZ != p[3]->fZ))
		{
			i++;
			continue;
		}

		p[1]->iFrame = p[3]->iFrame;

		for (int j = (i + 4); j < iNewSize; j++)
		{
			RawSMDKeyPosition* c1 = psRawObject->paKeyPosition + (j - 2);
			RawSMDKeyPosition* c2 = psRawObject->paKeyPosition + (j + 0);

			CopyMemory(c1, c2, sizeof(RawSMDKeyPosition));
		}

		iNewSize -= 2;
	}

	if (iNewSize < psRawObject->iKeyPositionCount)
	{
		//WRITEDBG( "Merged Key Positions (%d -> %d)", psRawObject->iKeyPositionCount, iNewSize );

		RawSMDKeyPosition* paOldArray = psRawObject->paKeyPosition;
		psRawObject->paKeyPosition = new RawSMDKeyPosition[iNewSize];
		CopyMemory(psRawObject->paKeyPosition, paOldArray, sizeof(RawSMDKeyPosition) * iNewSize);
		psRawObject->iKeyPositionCount = iNewSize;
		delete[] paOldArray;

		return TRUE;
	}

	return FALSE;
}

BOOL FixKeyPositionArray(RawSMDObject* psRawObject, int iLastFrame)
{
	if (psRawObject->iKeyPositionCount > 0)
	{
		if (psRawObject->paKeyPosition[psRawObject->iKeyPositionCount - 1].iFrame != iLastFrame)
		{
			RawSMDKeyPosition* paOldArray = psRawObject->paKeyPosition;
			psRawObject->paKeyPosition = new RawSMDKeyPosition[psRawObject->iKeyPositionCount + 1];
			CopyMemory(psRawObject->paKeyPosition, paOldArray, sizeof(RawSMDKeyPosition) * psRawObject->iKeyPositionCount);
			psRawObject->paKeyPosition[psRawObject->iKeyPositionCount] = paOldArray[psRawObject->iKeyPositionCount - 1];
			psRawObject->paKeyPosition[psRawObject->iKeyPositionCount].iFrame = iLastFrame;
			psRawObject->iKeyPositionCount++;
			delete[] paOldArray;

			return TRUE;
		}
	}

	return FALSE;
}

int FindKeyScaleIndex(RawSMDObject* psRawObject, int iStartFrame, int iEndFrame)
{
	int iIndex = 0;

	if (psRawObject->iKeyScaleCount > 0)
	{
		if (iEndFrame < psRawObject->paKeyScale[0].iFrame)
			return 0;

		if (iStartFrame >= psRawObject->paKeyScale[psRawObject->iKeyScaleCount - 1].iFrame)
			return psRawObject->iKeyScaleCount;

		for (int i = 1; i < psRawObject->iKeyScaleCount; i++)
		{
			RawSMDKeyScale* p1 = psRawObject->paKeyScale + (i - 1);
			RawSMDKeyScale* p2 = psRawObject->paKeyScale + i;

			if ((iStartFrame >= p1->iFrame) && (iEndFrame <= p2->iFrame))
				return i;
		}
	}

	return iIndex;
}

void InjectKeyScale(RawSMDObject* psRawObject, int iIndex, int iFrame, RawSMDKeyScale* psKeyScale)
{
	if (iIndex == -1)
		iIndex = psRawObject->iKeyScaleCount;

	RawSMDKeyScale* paOldArray = psRawObject->paKeyScale;
	psRawObject->paKeyScale = new RawSMDKeyScale[psRawObject->iKeyScaleCount + 1];

	if (iIndex > 0)
		CopyMemory(psRawObject->paKeyScale, paOldArray, sizeof(RawSMDKeyScale) * iIndex);

	psRawObject->paKeyScale[iIndex] = *psKeyScale;
	psRawObject->paKeyScale[iIndex].iFrame = iFrame;

	if (psRawObject->iKeyScaleCount > iIndex)
		CopyMemory(psRawObject->paKeyScale + iIndex + 1, paOldArray + iIndex, sizeof(RawSMDKeyScale) * (psRawObject->iKeyScaleCount - iIndex));

	delete[] paOldArray;

	psRawObject->iKeyScaleCount++;
}

BOOL MergeKeyScalings(RawSMDObject* psRawObject)
{
	int iNewSize = psRawObject->iKeyScaleCount;

	for (int i = 0; i < (iNewSize - 3); )
	{
		RawSMDKeyScale* p[4];
		p[0] = psRawObject->paKeyScale + (i + 0);
		p[1] = psRawObject->paKeyScale + (i + 1);
		p[2] = psRawObject->paKeyScale + (i + 2);
		p[3] = psRawObject->paKeyScale + (i + 3);

		if ((p[0]->iX != p[1]->iX) || (p[0]->iY != p[1]->iY) || (p[0]->iZ != p[1]->iZ))
		{
			i++;
			continue;
		}

		if ((p[0]->iX != p[2]->iX) || (p[0]->iY != p[2]->iY) || (p[0]->iZ != p[2]->iZ))
		{
			i++;
			continue;
		}

		if ((p[0]->iX != p[3]->iX) || (p[0]->iY != p[3]->iY) || (p[0]->iZ != p[3]->iZ))
		{
			i++;
			continue;
		}

		p[1]->iFrame = p[3]->iFrame;

		for (int j = (i + 4); j < iNewSize; j++)
		{
			RawSMDKeyScale* c1 = psRawObject->paKeyScale + (j - 2);
			RawSMDKeyScale* c2 = psRawObject->paKeyScale + (j + 0);

			CopyMemory(c1, c2, sizeof(RawSMDKeyScale));
		}

		iNewSize -= 2;
	}

	if (iNewSize < psRawObject->iKeyScaleCount)
	{
		//WRITEDBG( "Merged Key Scales (%d -> %d)", psRawObject->iKeyScaleCount, iNewSize );

		RawSMDKeyScale* paOldArray = psRawObject->paKeyScale;
		psRawObject->paKeyScale = new RawSMDKeyScale[iNewSize];
		CopyMemory(psRawObject->paKeyScale, paOldArray, sizeof(RawSMDKeyScale) * iNewSize);
		psRawObject->iKeyScaleCount = iNewSize;
		delete[] paOldArray;

		return TRUE;
	}

	return FALSE;
}

BOOL FixKeyScalingArray(RawSMDObject* psRawObject, int iLastFrame)
{
	if (psRawObject->iKeyScaleCount > 0)
	{
		if (psRawObject->paKeyScale[psRawObject->iKeyScaleCount - 1].iFrame != iLastFrame)
		{
			RawSMDKeyScale* paOldArray = psRawObject->paKeyScale;
			psRawObject->paKeyScale = new RawSMDKeyScale[psRawObject->iKeyScaleCount + 1];
			CopyMemory(psRawObject->paKeyScale, paOldArray, sizeof(RawSMDKeyScale) * psRawObject->iKeyScaleCount);
			psRawObject->paKeyScale[psRawObject->iKeyScaleCount] = paOldArray[psRawObject->iKeyScaleCount - 1];
			psRawObject->paKeyScale[psRawObject->iKeyScaleCount].iFrame = iLastFrame;
			psRawObject->iKeyScaleCount++;
			delete[] paOldArray;

			return TRUE;
		}
	}

	return FALSE;
}

AnimationHandler::AnimationHandler()
{
}

AnimationHandler::~AnimationHandler()
{
}

BOOL AnimationHandler::HandleBoneCache(smDPAT* psModelData)
{
	for (std::vector<smMODELINFO*>::iterator it = vAnimationModels.begin(); it != vAnimationModels.end(); it++)
	{
		smMODELINFO* ps = (*it);

		if (ps)
		{
			if (strcmpi(ps->szMotionFile, psModelData->lpModelInfo->szMotionFile) == 0)
			{
				CopyMemory(psModelData->lpModelInfo->MotionInfo, ps->MotionInfo, sizeof(smMOTIONINFO) * ps->MotionCount);
				psModelData->lpModelInfo->MotionCount = ps->MotionCount;
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL AnimationHandler::AddCacheModelData(smMODELINFO* psModelData)
{
	for (std::vector<smMODELINFO*>::iterator it = vAnimationModels.begin(); it != vAnimationModels.end(); it++)
	{
		smMODELINFO* ps = (*it);

		if (ps)
		{
			if (strcmpi(ps->szMotionFile, psModelData->szMotionFile) == 0)
				return FALSE;
		}
	}

	vAnimationModels.push_back(psModelData);
	return TRUE;
}

void AnimationHandler::LoadDynamicAnimation()
{
	char szObjectName[MAX_PATH];
	char szBipName[MAX_PATH];
	char szINX[MAX_PATH];
	char szSMB[MAX_PATH];

	char szSkillName[64];

	BOOL bLoop = FALSE;

	int iItemType = 1; //1 = all, 2 = one hand, 3 = two hand

	int iaEvent[4] = { 0 };
	int iAreaType = 0;
	int iClassFlag = 0;
	int iID = 0;
	int iTypeAnim = 336;

	File cReader("game\\objects\\animskills.ini");

	int iCount = cReader.readInt("Animations", "Count");
	int iNext = 0;

	for (int i = 0; i < iCount; i++)
	{
		sprintf(szSkillName, "Animation%d", (iNext + 1));

		strcpy(szObjectName, cReader.readString(szSkillName, "AnimationFile").c_str());

		bLoop = cReader.readInt(szSkillName, "Loop");
		iAreaType = cReader.readInt(szSkillName, "AreaType");
		iID = cReader.readInt(szSkillName, "ID");
		iaEvent[0] = cReader.readInt(szSkillName, "Event1");
		iaEvent[1] = cReader.readInt(szSkillName, "Event2");
		iaEvent[2] = cReader.readInt(szSkillName, "Event3");
		iaEvent[3] = cReader.readInt(szSkillName, "Event4");
		iItemType = cReader.readInt(szSkillName, "ItemType");
		iTypeAnim = cReader.readInt(szSkillName, "AnimType");

		if (iTypeAnim == 0)
			iTypeAnim = 336;

		if (iItemType == 0)
			iItemType++;

		std::string strClassFlag = cReader.readString(szSkillName, "Class");
		int iClassFlag = 0;

		if (strcmpi(strClassFlag.c_str(), "Fighter") == 0)
		{
			iClassFlag = 0x0001;
			strcpy(szBipName, "char\\tmABCD\\M1Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Mech") == 0)
		{
			iClassFlag = 0x0002;
			strcpy(szBipName, "char\\tmABCD\\M1Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Archer") == 0)
		{
			iClassFlag = 0x0004;
			strcpy(szBipName, "char\\tmABCD\\M2Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Pikeman") == 0)
		{
			iClassFlag = 0x0008;
			strcpy(szBipName, "char\\tmABCD\\M4Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Assassin") == 0)
		{
			iClassFlag = 0x0100;
			strcpy(szBipName, "char\\tmABCD\\M6Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Knight") == 0)
		{
			iClassFlag = 0x0020;
			strcpy(szBipName, "char\\tmABCD\\M1Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Atalanta") == 0)
		{
			iClassFlag = 0x0010;
			strcpy(szBipName, "char\\tmABCD\\M2Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Priestess") == 0)
		{
			iClassFlag = 0x0080;
			strcpy(szBipName, "char\\tmABCD\\M5Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Magician") == 0)
		{
			iClassFlag = 0x0040;
			strcpy(szBipName, "char\\tmABCD\\M3Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "Shaman") == 0)
		{
			iClassFlag = 0x0200;
			strcpy(szBipName, "char\\tmABCD\\M7Bip.ini");
		}
		else if (strcmpi(strClassFlag.c_str(), "MartialArtist") == 0)
		{
			iClassFlag = 0x0400;
			strcpy(szBipName, "char\\tmABCD\\M8Bip.ini");
		}


		int iMotionFile = cReader.readInt(szSkillName, "MotionIndex");
		if (iMotionFile)
		{
			iClassFlag = 0;
			memset(szBipName, 0, _countof(szBipName));
			sprintf(szBipName, "char\\tmABCD\\M%dBip.ini", iMotionFile);
		}

		std::string strItems = cReader.readString(szSkillName, "Items");
		auto vItems = split(strItems, ",");

		std::vector<CustomAnimation> vCustomAnimations;
		std::string strAnimations = cReader.readString(szSkillName, "CustomAnimations");

		for (const auto& animation : split(strAnimations, ";"))
		{
			if (const auto& customAnimation = split(animation, ","); customAnimation.size() == 3)
			{
				CustomAnimation sCustomAnimation;
				sCustomAnimation.iType = atoi(customAnimation[0].c_str());
				sCustomAnimation.bLoop = atoi(customAnimation[1].c_str());
				sCustomAnimation.iLength = atoi(customAnimation[2].c_str());
				vCustomAnimations.push_back(sCustomAnimation);
			}
		}

		if (szObjectName[0] == 0 || szBipName[0] == 0)
		{
			iNext++;
			continue;
		}

		smDPAT* psModelData = LoadINX(szBipName);
		smPAT3D* pcModel = smASE_ReadBone(szObjectName);

		strcpy(szINX, szBipName);
		szINX[strlen(szINX) - 1] = 'x';

		DecryptBoneFile(pcModel);

		strcpy(szSMB, psModelData->lpModelInfo->szMotionFile);

		int iLength = strlen(szSMB);
		if (iLength > 0)
		{
			szSMB[iLength - 3] = 's';
			szSMB[iLength - 2] = 'm';
			szSMB[iLength - 1] = 'b';
		}

		iLength = strlen(szObjectName);
		if (iLength > 0)
		{
			szObjectName[iLength - 3] = 's';
			szObjectName[iLength - 2] = 'm';
			szObjectName[iLength - 1] = 'b';
		}

		//FixSMBINXSubFileMotion( szSMB, szINX, szSMB, szINX );
		InjectDynamicAnimation(psModelData, pcModel, iID, bLoop, iClassFlag, iAreaType, iaEvent, iItemType, iTypeAnim, vCustomAnimations, vItems);
		iNext++;
	}
}

void AnimationHandler::FixSMBINXSubFileMotion(const char* pszSMBFile, const char* pszINXFile, const char* pszSMBFileOut, const char* pszINXFileOut)
{
	//WRITEDBG( "FixSMBINXSubFileMotion(%s,%s) Working...", pszSMBFile, pszINXFile );

	Import::SMDFile impSMDFile(pszSMBFile);
	Export::SMDFile expSMDFile(pszSMBFileOut);

	Import::INXFile impINXFile(pszINXFile);
	Export::INXFile expINXFile(pszINXFileOut);

	if (!impSMDFile.Load())
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() failed to load SMB" );
		return;
	}

	if (!impINXFile.Load())
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() failed to load INX" );
		return;
	}

	if (impSMDFile.sRawHeader.iObjectCount <= 0)
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() no objects in SMB, there's no point to this!" );
		return;
	}

	//WRITEDBG( "Frame Count: %d", impSMDFile.sRawHeader.iFrameCount );

	if (impSMDFile.sRawHeader.iFrameCount <= 1)
	{
		//WRITEDBG( "FixSMBINXSubFileMotion() SMB Frame Count is already 0 or 1... there's no point to this!" );
		return;
	}

	int iLastFrame = 0;

	//Fix SMB Header
	{
		RawSMDFrame* pFirst = impSMDFile.sRawHeader.aFrame + 0;
		pFirst->iStartFrame = 0;
		for (int i = 1; i < impSMDFile.sRawHeader.iFrameCount; i++)
		{
			RawSMDFrame* p = impSMDFile.sRawHeader.aFrame + i;

			//WRITEDBG( "Frame[%d] : (%d,%d)", i, p->iStartFrame, p->iEndFrame );

			pFirst->iEndFrame += (p->iEndFrame - p->iStartFrame);

			ZeroMemory(p, sizeof(RawSMDFrame));
		}
		pFirst->iKeyFrameStartIndex = 0;
		pFirst->iKeyFrameCount = 0;
		impSMDFile.sRawHeader.iFrameCount = 1;

		iLastFrame = pFirst->iEndFrame;
	}

	//WRITEDBG( "Last Frame: %d", iLastFrame );

	//Fix INX Animations
	{
		RawSMDObject* psFirstObject = impSMDFile.psaRawObject + 0;

		unsigned int i = 10;
		while ((i < 512) && (impINXFile.sRawFile.saAnimation[i].iType != 0))
		{
			RawINXAnimation* psRawAnimation = impINXFile.sRawFile.saAnimation + i;

			int iIndex = psRawAnimation->iSubFileFrameNumber - 1;

			if (iIndex < 0)
			{
				//WRITEDBG( "Weird Index in INX Animation [%d] -> %d", i, iIndex );
				return;
			}

			if (iIndex >= psFirstObject->iFrameCount)
			{
				//WRITEDBG( "Too High Index in INX Animation [%d] : [%d] >= [%d]", i, iIndex, psFirstObject->iFrameCount );
				return;
			}

			psRawAnimation->iStartFrame += (psFirstObject->aRotationFrame[iIndex].iStartFrame / 160);
			psRawAnimation->iEndFrame += (psFirstObject->aRotationFrame[iIndex].iStartFrame / 160);

			psRawAnimation->iSubFileFrameNumber = 1;

			i++;
		}
	}

	//Fix SMB Objects Key Frames
	for (int i = 0; i < impSMDFile.sRawHeader.iObjectCount; i++)
	{
		//WRITEDBG( "----------------------------------------------" );

		RawSMDObject* psRawObject = impSMDFile.psaRawObject + i;
		RawSMDObject* psRawObjectParent = NULL;

		if (psRawObject->szParentObjectName[0] != 0)
		{
			for (int j = 0; j < impSMDFile.sRawHeader.iObjectCount; j++)
			{
				RawSMDObject* p = impSMDFile.psaRawObject + j;

				if (strcmpi(p->szObjectName, psRawObject->szParentObjectName) == 0)
				{
					psRawObjectParent = p;
					break;
				}
			}

			if (!psRawObjectParent)
			{
				//WRITEDBG( "Warning! Parent Object %s not found of Object %s", psRawObject->szParentObjectName, psRawObject->szObjectName );
			}
		}

		//WRITEDBG( "Object %s (Parent: %s)", psRawObject->szObjectName, psRawObjectParent ? psRawObjectParent->szObjectName : "None" );

		//WRITEDBG( "Object %s has %d Key Rotations", psRawObject->szObjectName, psRawObject->iKeyRotationCount );
		//WRITEDBG( "Object %s has %d Key Positions", psRawObject->szObjectName, psRawObject->iKeyPositionCount );
		//WRITEDBG( "Object %s has %d Key Scales", psRawObject->szObjectName, psRawObject->iKeyScaleCount );

		RawSMDKeyRotation sKeyRotation;
		sKeyRotation.fW = 0.0f;
		sKeyRotation.fX = 0.0f;
		sKeyRotation.fY = 0.0f;
		sKeyRotation.fZ = 0.0f;

		RawSMDKeyRotation sKeyRotationTemp;
		sKeyRotationTemp.fW = psRawObject->sRotationQuaternion.fW;
		sKeyRotationTemp.fX = psRawObject->sRotationQuaternion.fX;
		sKeyRotationTemp.fY = psRawObject->sRotationQuaternion.fY;
		sKeyRotationTemp.fZ = psRawObject->sRotationQuaternion.fZ;

		RawSMDMatrixF sRotation;

		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				sRotation.f[i][j] = psRawObject->sRotationMatrix.i[i][j] / 256.f;

		//WRITEDBG( "Object %s default Key Rotation: W %.4f X %.4f Y %.4f Z %.4f", psRawObject->szObjectName, sKeyRotationTemp.fW, sKeyRotationTemp.fX, sKeyRotationTemp.fY, sKeyRotationTemp.fZ );

		RawSMDKeyPosition sKeyPosition;
		sKeyPosition.fX = (float)(psRawObject->sTranslationVector.iX) / 256.0f;
		sKeyPosition.fY = (float)(psRawObject->sTranslationVector.iY) / 256.0f;
		sKeyPosition.fZ = (float)(psRawObject->sTranslationVector.iZ) / 256.0f;

		//WRITEDBG( "Object %s default Key Position: X %.4f Y %.4f Z %.4f", psRawObject->szObjectName, sKeyPosition.fX, sKeyPosition.fY, sKeyPosition.fZ );

		RawSMDKeyScale sKeyScale;
		sKeyScale.iX = psRawObject->sScalingVector.iX;
		sKeyScale.iY = psRawObject->sScalingVector.iY;
		sKeyScale.iZ = psRawObject->sScalingVector.iZ;

		//WRITEDBG( "Object %s default Key Scale: X %d Y %d Z %d", psRawObject->szObjectName, sKeyScale.iX, sKeyScale.iY, sKeyScale.iZ );

		RawSMDFrame* pRot = psRawObject->aRotationFrame + 0;
		RawSMDFrame* pTra = psRawObject->aTranslationFrame + 0;
		RawSMDFrame* pScl = psRawObject->aScalingFrame + 0;

		pRot->iKeyFrameStartIndex = 0;
		pTra->iKeyFrameStartIndex = 0;
		pScl->iKeyFrameStartIndex = 0;

		BOOL bObjectKeysChanged = FALSE;

		for (int z = 1; z < psRawObject->iFrameCount; z++)
		{
			RawSMDFrame* pRotTemp = psRawObject->aRotationFrame + z;
			RawSMDFrame* pTraTemp = psRawObject->aTranslationFrame + z;
			RawSMDFrame* pSclTemp = psRawObject->aScalingFrame + z;

			pRot->iEndFrame += (pRotTemp->iEndFrame - pRotTemp->iStartFrame);
			pTra->iEndFrame += (pTraTemp->iEndFrame - pTraTemp->iStartFrame);
			pScl->iEndFrame += (pSclTemp->iEndFrame - pSclTemp->iStartFrame);

			//No Rotation Controls?
			if (pRotTemp->iKeyFrameCount == 0)
			{
				int iIndex = FindKeyRotationIndex(psRawObject, pRotTemp->iStartFrame, pRotTemp->iEndFrame);

				auto obj = (smOBJ3D*)psRawObject;
				smTM_ROT* rot = (smTM_ROT*)&sKeyRotation;
				smFMATRIX* mat = (smFMATRIX*)&sRotation;

				InjectKeyRotation(obj, iIndex, pRotTemp->iStartFrame, rot, mat);
				InjectKeyRotation(obj, iIndex + 1, pRotTemp->iEndFrame, rot, mat);

				bObjectKeysChanged = TRUE;

				pRotTemp->iKeyFrameCount += 2;
			}

			//No Translation Controls?
			if (pTraTemp->iKeyFrameCount == 0)
			{
				int iIndex = FindKeyPositionIndex(psRawObject, pTraTemp->iStartFrame, pTraTemp->iEndFrame);

				InjectKeyPosition(psRawObject, iIndex, pTraTemp->iStartFrame, &sKeyPosition);
				InjectKeyPosition(psRawObject, iIndex + 1, pTraTemp->iEndFrame, &sKeyPosition);

				bObjectKeysChanged = TRUE;

				pTraTemp->iKeyFrameCount += 2;
			}

			//No Scaling Controls?
			if (pSclTemp->iKeyFrameCount == 0)
			{
				int iIndex = FindKeyScaleIndex(psRawObject, pSclTemp->iStartFrame, pSclTemp->iEndFrame);

				InjectKeyScale(psRawObject, iIndex, pSclTemp->iStartFrame, &sKeyScale);
				InjectKeyScale(psRawObject, iIndex + 1, pSclTemp->iEndFrame, &sKeyScale);

				bObjectKeysChanged = TRUE;

				pSclTemp->iKeyFrameCount += 2;
			}

			pRot->iKeyFrameCount += pRotTemp->iKeyFrameCount;
			pTra->iKeyFrameCount += pTraTemp->iKeyFrameCount;
			pScl->iKeyFrameCount += pSclTemp->iKeyFrameCount;
		}

		psRawObject->iFrameCount = 1;

		if (MergeKeyRotations(psRawObject))
		{
			bObjectKeysChanged = TRUE;
		}

		if (MergeKeyPositions(psRawObject))
		{
			bObjectKeysChanged = TRUE;
		}

		if (MergeKeyScalings(psRawObject))
		{
			bObjectKeysChanged = TRUE;
		}

		if (FixKeyRotationArray(psRawObject, iLastFrame))
		{
			//WRITEDBG( "Fixed Key Rotation Array of Object %s", psRawObject->szObjectName );

			bObjectKeysChanged = TRUE;
		}

		if (FixKeyPositionArray(psRawObject, iLastFrame))
		{
			//WRITEDBG( "Fixed Key Position Array of Object %s", psRawObject->szObjectName );

			bObjectKeysChanged = TRUE;
		}

		if (FixKeyScalingArray(psRawObject, iLastFrame))
		{
			//WRITEDBG( "Fixed Key Scaling Array of Object %s", psRawObject->szObjectName );

			bObjectKeysChanged = TRUE;
		}

		pRot->iKeyFrameCount = psRawObject->iKeyRotationCount;
		pTra->iKeyFrameCount = psRawObject->iKeyPositionCount;
		pScl->iKeyFrameCount = psRawObject->iKeyScaleCount;

		//WRITEDBG( "Object %s now has %d Key Rotations", psRawObject->szObjectName, psRawObject->iKeyRotationCount );
		//WRITEDBG( "Object %s now has %d Key Positions", psRawObject->szObjectName, psRawObject->iKeyPositionCount );
		//WRITEDBG( "Object %s now has %d Key Scales", psRawObject->szObjectName, psRawObject->iKeyScaleCount );

		if (bObjectKeysChanged)
		{
			//WRITEDBG( "Object %s Has Changed! Health Check...", psRawObject->szObjectName );

			if (psRawObject->iKeyRotationCount > 0)
			{
				RawSMDKeyRotation* prev = psRawObject->paKeyRotation + 0;

				for (int a = 1; a < psRawObject->iKeyRotationCount; a++)
				{
					RawSMDKeyRotation* p = psRawObject->paKeyRotation + a;

					if (p->iFrame < prev->iFrame)
					{
						//WRITEDBG( "Key Rotation [%d/%d] Invalid Frame [%d <= %d]", a, psRawObject->iKeyRotationCount, p->iFrame, prev->iFrame );
					}

					prev = p;
				}
			}

			if (psRawObject->iKeyPositionCount > 0)
			{
				RawSMDKeyPosition* prev = psRawObject->paKeyPosition + 0;

				for (int a = 1; a < psRawObject->iKeyPositionCount; a++)
				{
					RawSMDKeyPosition* p = psRawObject->paKeyPosition + a;

					if (p->iFrame < prev->iFrame)
					{
						//WRITEDBG( "Key Position [%d/%d] Invalid Frame [%d <= %d]", a, psRawObject->iKeyPositionCount, p->iFrame, prev->iFrame );
					}

					prev = p;
				}
			}

			if (psRawObject->iKeyScaleCount > 0)
			{
				RawSMDKeyScale* prev = psRawObject->paKeyScale + 0;

				for (int a = 1; a < psRawObject->iKeyScaleCount; a++)
				{
					RawSMDKeyScale* p = psRawObject->paKeyScale + a;

					if (p->iFrame < prev->iFrame)
					{
						//WRITEDBG( "Key Scale [%d/%d] Invalid Frame [%d <= %d]", a, psRawObject->iKeyScaleCount, p->iFrame, prev->iFrame );
					}

					prev = p;
				}
			}

			//WRITEDBG( "Object %s Health Check Finished", psRawObject->szObjectName );
		}
	}

	//Save SMB
	expSMDFile.SaveModel(&impSMDFile.sRawHeader, &impSMDFile.sRawMaterialHeader, impSMDFile.psaRawObject);

	//Save INX
	expINXFile.Save(&impINXFile.sRawFile);

	//Unload SMB
	impSMDFile.Unload();

	//Done!
	//WRITEDBG( "FixSMBINXSubFileMotion(%s,%s) Finished!", pszSMBFile, pszINXFile );
}


bool AnimationHandler::InjectDynamicAnimation(smDPAT* psModelData, smPAT3D* pcModelAnimation, int iID, BOOL bLoop, int iClassFlag, int iAreaType, int iaEvent[4], int iItemType, int iTypeAnim, std::vector<CustomAnimation> vAnimations, std::vector<std::string> vItems)
{
	if (psModelData == NULL || pcModelAnimation == NULL || psModelData->smDinaLink == NULL || psModelData->smDinaLink->Pat == NULL || psModelData->lpModelInfo == NULL)
		return false;

	smPAT3D* pcModel = psModelData->smDinaLink->Pat;
	smMODELINFO* psModelINX = psModelData->lpModelInfo;

	int iNewLastFrame = pcModel->MaxFrame;
	int iNewAnimationStartFrame = iNewLastFrame;

	bool bFoundKeyFrames = false;

	//Inject
	for (int i = 0; i < (int)pcModelAnimation->nObj3d; i++)
	{
		smOBJ3D* pcMeshA = pcModelAnimation->obj3d[i];

		if (pcMeshA)
		{
			//Get Parent Mesh
			smOBJ3D* pcMesh = pcModel->GetObjectFromName(pcMeshA->NodeName);

			//Casting
			smOBJ3D* psRawObjectFrom = (smOBJ3D*)pcMeshA;
			smOBJ3D* psRawObjectTo = (smOBJ3D*)pcMesh;

			if (bFoundKeyFrames == false)
			{
				if (psRawObjectFrom->TmRotCnt > 0)
				{
					iNewLastFrame += psRawObjectFrom->TmRot[psRawObjectFrom->TmRotCnt - 1].frame;
					bFoundKeyFrames = true;
				}
				else if (psRawObjectFrom->TmPosCnt > 0)
				{
					iNewLastFrame += psRawObjectFrom->TmPos[psRawObjectFrom->TmPosCnt - 1].frame;
					bFoundKeyFrames = true;
				}
				else if (psRawObjectFrom->TmScaleCnt > 0)
				{
					iNewLastFrame += psRawObjectFrom->TmScale[psRawObjectFrom->TmScaleCnt - 1].frame;
					bFoundKeyFrames = true;
				}
			}

			//Not have Mesh on SMB? ignore it...
			if (pcMesh == NULL)
				continue;

			smTM_ROT sKeyRotation;
			sKeyRotation.w = 0.0f;
			sKeyRotation.x = 0.0f;
			sKeyRotation.y = 0.0f;
			sKeyRotation.z = 0.0f;

			smFMATRIX sRotation;
			sRotation._11 = psRawObjectFrom->TmRotate._11 / 256.f;
			sRotation._12 = psRawObjectFrom->TmRotate._12 / 256.f;
			sRotation._13 = psRawObjectFrom->TmRotate._13 / 256.f;
			sRotation._14 = psRawObjectFrom->TmRotate._14 / 256.f;
			sRotation._21 = psRawObjectFrom->TmRotate._21 / 256.f;
			sRotation._22 = psRawObjectFrom->TmRotate._22 / 256.f;
			sRotation._23 = psRawObjectFrom->TmRotate._23 / 256.f;
			sRotation._24 = psRawObjectFrom->TmRotate._24 / 256.f;
			sRotation._31 = psRawObjectFrom->TmRotate._31 / 256.f;
			sRotation._32 = psRawObjectFrom->TmRotate._32 / 256.f;
			sRotation._33 = psRawObjectFrom->TmRotate._33 / 256.f;
			sRotation._34 = psRawObjectFrom->TmRotate._34 / 256.f;
			sRotation._41 = psRawObjectFrom->TmRotate._41 / 256.f;
			sRotation._42 = psRawObjectFrom->TmRotate._42 / 256.f;
			sRotation._43 = psRawObjectFrom->TmRotate._43 / 256.f;
			sRotation._44 = psRawObjectFrom->TmRotate._44 / 256.f;

			smTM_POS sKeyPosition;
			sKeyPosition.x = (float)(psRawObjectFrom->px) / 256.0f;
			sKeyPosition.y = (float)(psRawObjectFrom->py) / 256.0f;
			sKeyPosition.z = (float)(psRawObjectFrom->pz) / 256.0f;

			smTM_SCALE sKeyScale;
			sKeyScale.x = psRawObjectFrom->sx;
			sKeyScale.y = psRawObjectFrom->sy;
			sKeyScale.z = psRawObjectFrom->sz;

			smFRAME_POS* psNewRotFrame = psRawObjectTo->TmRotFrame + 0;
			smFRAME_POS* psNewTraFrame = psRawObjectTo->TmPosFrame + 0;
			smFRAME_POS* psNewSclFrame = psRawObjectTo->TmScaleFrame + 0;

			if (psRawObjectFrom->TmRotCnt > 0)
			{
				for (int k = 0; k < psRawObjectFrom->TmRotCnt; k++)
				{
					smTM_ROT* psKeyRotation = psRawObjectFrom->TmRot + k;
					smFMATRIX* psRotation = psRawObjectFrom->TmPrevRot + k;

					psKeyRotation->frame += iNewAnimationStartFrame;

					InjectKeyRotation(psRawObjectTo, -1, psKeyRotation->frame, psKeyRotation, psRotation);
				}
			}
			else
			{
				auto obj = (smOBJ3D*)psRawObjectTo;
				smTM_ROT* rot = (smTM_ROT*)&sKeyRotation;
				smFMATRIX* mat = (smFMATRIX*)&sRotation;

				InjectKeyRotation(obj, -1, iNewAnimationStartFrame, rot, mat);
				InjectKeyRotation(obj, -1, iNewLastFrame, rot, mat);

				psNewRotFrame->PosCnt += 2;
			}

			if (psRawObjectFrom->TmPosCnt > 0)
			{
				for (int k = 0; k < psRawObjectFrom->TmPosCnt; k++)
				{
					smTM_POS* psKeyPosition = psRawObjectFrom->TmPos + k;

					psKeyPosition->frame += iNewAnimationStartFrame;

					InjectKeyPosition(psRawObjectTo, -1, psKeyPosition->frame, psKeyPosition);
				}
			}
			else
			{
				InjectKeyPosition(psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyPosition);
				InjectKeyPosition(psRawObjectTo, -1, iNewLastFrame, &sKeyPosition);

				psNewTraFrame->PosCnt += 2;
			}

			if (psRawObjectFrom->TmScaleCnt > 0)
			{
				for (int k = 0; k < psRawObjectFrom->TmScaleCnt; k++)
				{
					smTM_SCALE* psKeyScale = psRawObjectFrom->TmScale + k;

					psKeyScale->frame += iNewAnimationStartFrame;

					InjectKeyScale(psRawObjectTo, -1, psKeyScale->frame, psKeyScale);
				}
			}
			else
			{
				InjectKeyScale(psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyScale);
				InjectKeyScale(psRawObjectTo, -1, iNewLastFrame, &sKeyScale);

				psNewSclFrame->PosCnt += 2;
			}

			MergeKeyRotations(psRawObjectTo);
			MergeKeyPositions(psRawObjectTo);
			MergeKeyScalings(psRawObjectTo);

			psNewRotFrame->EndFrame = iNewLastFrame;
			psNewTraFrame->EndFrame = iNewLastFrame;
			psNewSclFrame->EndFrame = iNewLastFrame;

			psNewRotFrame->PosCnt = psRawObjectTo->TmRotCnt;
			psNewTraFrame->PosCnt = psRawObjectTo->TmPosCnt;
			psNewSclFrame->PosCnt = psRawObjectTo->TmScaleCnt;
		}
	}

	pcModel->MaxFrame = iNewLastFrame;

	if (vAnimations.size())
	{
		int iBaseStartFrame = iNewAnimationStartFrame / 160;

		for (const auto& sCustomAnimation : vAnimations)
		{
			ZeroMemory(&psModelINX->MotionInfo[psModelINX->MotionCount], sizeof(smMOTIONINFO));

			psModelINX->MotionInfo[psModelINX->MotionCount].State = sCustomAnimation.iType;
			psModelINX->MotionInfo[psModelINX->MotionCount].Repeat = sCustomAnimation.bLoop;
			psModelINX->MotionInfo[psModelINX->MotionCount].dwJobCodeBit = iClassFlag;
			psModelINX->MotionInfo[psModelINX->MotionCount].MapPosition = iAreaType;
			psModelINX->MotionInfo[psModelINX->MotionCount].SkillCodeList[0] = 0;
			psModelINX->MotionInfo[psModelINX->MotionCount].MotionFrame = 1;
			psModelINX->MotionInfo[psModelINX->MotionCount].StartFrame = sCustomAnimation.iLength == 0 ? psModelINX->MotionInfo[psModelINX->MotionCount - 1].StartFrame : iBaseStartFrame;
			psModelINX->MotionInfo[psModelINX->MotionCount].EndFrame = sCustomAnimation.iLength == 0 ? psModelINX->MotionInfo[psModelINX->MotionCount - 1].EndFrame : iBaseStartFrame + sCustomAnimation.iLength;
			psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[0] = iaEvent[0] * 160;
			psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[1] = iaEvent[1] * 160;
			psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[2] = iaEvent[2] * 160;
			psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[3] = iaEvent[3] * 160;
			psModelINX->MotionInfo[psModelINX->MotionCount].ItemCodeCount = 0;

			for (const auto& pItem : vItems)
				psModelINX->MotionInfo[psModelINX->MotionCount].ItemCodeList[psModelINX->MotionInfo[psModelINX->MotionCount].ItemCodeCount++] = static_cast<BYTE>(atoi(pItem.c_str()));

			iBaseStartFrame = psModelINX->MotionInfo[psModelINX->MotionCount].EndFrame + 1;
			psModelINX->MotionCount++;
		}
	}
	else
	{
		psModelINX->MotionInfo[psModelINX->MotionCount].State = iTypeAnim;
		psModelINX->MotionInfo[psModelINX->MotionCount].Repeat = bLoop;
		psModelINX->MotionInfo[psModelINX->MotionCount].dwJobCodeBit = iClassFlag;
		psModelINX->MotionInfo[psModelINX->MotionCount].MapPosition = iAreaType;
		psModelINX->MotionInfo[psModelINX->MotionCount].SkillCodeList[0] = iID;
		psModelINX->MotionInfo[psModelINX->MotionCount].MotionFrame = 1;
		psModelINX->MotionInfo[psModelINX->MotionCount].StartFrame = (iNewAnimationStartFrame / 160) + 1;
		psModelINX->MotionInfo[psModelINX->MotionCount].EndFrame = iNewLastFrame / 160;
		psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[0] = iaEvent[0] * 160;
		psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[1] = iaEvent[1] * 160;
		psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[2] = iaEvent[2] * 160;
		psModelINX->MotionInfo[psModelINX->MotionCount].EventFrame[3] = iaEvent[3] * 160;
		psModelINX->MotionInfo[psModelINX->MotionCount].ItemCodeCount = 0;

		for (const auto& pItem : vItems)
			psModelINX->MotionInfo[psModelINX->MotionCount].ItemCodeList[psModelINX->MotionInfo[psModelINX->MotionCount].ItemCodeCount++] = static_cast<BYTE>(atoi(pItem.c_str()));

		psModelINX->MotionCount++;
	}

	if (pcModel->newModel)
		for (const auto& mesh : pcModel->newModel->meshes)
			mesh->UpdateFinders();

	AddCacheModelData(psModelINX);

	return true;
}

bool AnimationHandler::InjectAnimation(const char* pszSMBFileFrom, const char* pszSMBFileTo, const char* pszINXFileTo, int eAnimationType, int iAreaType, BOOL bLoop, unsigned int uClassFlag, unsigned int uSkillCode, std::vector<CustomAnimation> vAnimations, std::vector<std::string> vItems)
{
	//WRITEDBG( "InjectionAnimation(%s,%s,%s) Working...", pszSMBFileFrom, pszSMBFileTo, pszINXFileTo );

	Import::SMDFile smbI1(pszSMBFileFrom);
	Import::SMDFile smbI2(pszSMBFileTo);

	if (!smbI1.Load())
	{
		//WRITEDBG( "Can't load SMB: %s", pszSMBFileFrom );
		return FALSE;
	}

	if (!smbI2.Load())
	{
		//WRITEDBG( "Can't load SMB: %s", pszSMBFileTo );
		return FALSE;
	}

	Import::INXFile inxI(pszINXFileTo);

	if (!inxI.Load())
	{
		//WRITEDBG( "Can't load INX: %s", pszINXFileTo );
		return FALSE;
	}

	int iFrameIndex = 0;

	int iNewLastFrame = smbI2.sRawHeader.aFrame[0].iEndFrame;
	int iNewAnimationStartFrame = iNewLastFrame;

	//Find the Last Frame first
	BOOL bFoundKeyFrames = FALSE;
	for (int i = 0; i < smbI1.sRawHeader.iObjectCount; i++)
	{
		const RawSMDObject* psRawObjectFrom = smbI1.psaRawObject + i;

		if (psRawObjectFrom->iKeyRotationCount > 0)
		{
			iNewLastFrame += psRawObjectFrom->paKeyRotation[psRawObjectFrom->iKeyRotationCount - 1].iFrame;

			bFoundKeyFrames = TRUE;
			break;
		}

		if (psRawObjectFrom->iKeyPositionCount > 0)
		{
			iNewLastFrame += psRawObjectFrom->paKeyPosition[psRawObjectFrom->iKeyPositionCount - 1].iFrame;

			bFoundKeyFrames = TRUE;
			break;
		}

		if (psRawObjectFrom->iKeyScaleCount > 0)
		{
			iNewLastFrame += psRawObjectFrom->paKeyScale[psRawObjectFrom->iKeyScaleCount - 1].iFrame;

			bFoundKeyFrames = TRUE;
			break;
		}
	}

	if (!bFoundKeyFrames)
	{
		//WRITEDBG( "No Animation Data (Key Frames) found in the source SMB file.. this is pointless!" );
		return FALSE;
	}

	//Loop through Objects in SMB to read animation from
	for (int i = 0; i < smbI1.sRawHeader.iObjectCount; i++)
	{
		const RawSMDObject* psRawObjectFrom = smbI1.psaRawObject + i;
		RawSMDObject* psRawObjectTo = NULL;

		//Find Object in Motion SMB
		int iNewObjectIndex = -1;
		for (int j = 0; j < smbI2.sRawHeader.iObjectCount; j++)
		{
			RawSMDObject* psTemp = smbI2.psaRawObject + j;

			if (strcmp(psRawObjectFrom->szObjectName, psTemp->szObjectName) == 0)
			{
				iNewObjectIndex = j;
				psRawObjectTo = psTemp;
				break;
			}
		}

		//Not Found?
		if (psRawObjectTo == NULL)
		{
			//WRITEDBG( "Object %s not found in Motion SMB, ignoring...", psRawObjectFrom->szObjectName );
			continue;
		}

		//WRITEDBG( "[%d][%d] Object %s Copying Animation Key Frames...", i, iNewObjectIndex, psRawObjectFrom->szObjectName );

		RawSMDKeyRotation sKeyRotation;
		sKeyRotation.fW = 0.0f;
		sKeyRotation.fX = 0.0f;
		sKeyRotation.fY = 0.0f;
		sKeyRotation.fZ = 0.0f;

		smFMATRIX sRotation;
		sRotation._11 = psRawObjectFrom->sRotationMatrix.i[0][0] / 256.f;
		sRotation._12 = psRawObjectFrom->sRotationMatrix.i[0][1] / 256.f;
		sRotation._13 = psRawObjectFrom->sRotationMatrix.i[0][2] / 256.f;
		sRotation._14 = psRawObjectFrom->sRotationMatrix.i[0][3] / 256.f;
		sRotation._21 = psRawObjectFrom->sRotationMatrix.i[1][0] / 256.f;
		sRotation._22 = psRawObjectFrom->sRotationMatrix.i[1][1] / 256.f;
		sRotation._23 = psRawObjectFrom->sRotationMatrix.i[1][2] / 256.f;
		sRotation._24 = psRawObjectFrom->sRotationMatrix.i[1][3] / 256.f;
		sRotation._31 = psRawObjectFrom->sRotationMatrix.i[2][0] / 256.f;
		sRotation._32 = psRawObjectFrom->sRotationMatrix.i[2][1] / 256.f;
		sRotation._33 = psRawObjectFrom->sRotationMatrix.i[2][2] / 256.f;
		sRotation._34 = psRawObjectFrom->sRotationMatrix.i[2][3] / 256.f;
		sRotation._41 = psRawObjectFrom->sRotationMatrix.i[3][0] / 256.f;
		sRotation._42 = psRawObjectFrom->sRotationMatrix.i[3][1] / 256.f;
		sRotation._43 = psRawObjectFrom->sRotationMatrix.i[3][2] / 256.f;
		sRotation._44 = psRawObjectFrom->sRotationMatrix.i[3][3] / 256.f;

		RawSMDKeyPosition sKeyPosition;
		sKeyPosition.fX = (float)(psRawObjectFrom->sTranslationVector.iX) / 256.0f;
		sKeyPosition.fY = (float)(psRawObjectFrom->sTranslationVector.iY) / 256.0f;
		sKeyPosition.fZ = (float)(psRawObjectFrom->sTranslationVector.iZ) / 256.0f;

		RawSMDKeyScale sKeyScale;
		sKeyScale.iX = psRawObjectFrom->sScalingVector.iX;
		sKeyScale.iY = psRawObjectFrom->sScalingVector.iY;
		sKeyScale.iZ = psRawObjectFrom->sScalingVector.iZ;

		RawSMDFrame* psNewRotFrame = psRawObjectTo->aRotationFrame + 0;
		RawSMDFrame* psNewTraFrame = psRawObjectTo->aTranslationFrame + 0;
		RawSMDFrame* psNewSclFrame = psRawObjectTo->aScalingFrame + 0;

		//WRITEDBG( "Object %s Frame Rot Cnt: %d", psRawObjectFrom->szObjectName, psRawObjectFrom->iKeyRotationCount );
		//WRITEDBG( "Object %s Frame Tra Cnt: %d", psRawObjectFrom->szObjectName, psRawObjectFrom->iKeyPositionCount );
		//WRITEDBG( "Object %s Frame Scl Cnt: %d", psRawObjectFrom->szObjectName, psRawObjectFrom->iKeyScaleCount );

		if (psRawObjectFrom->iKeyRotationCount > 0)
		{
			for (int k = 0; k < psRawObjectFrom->iKeyRotationCount; k++)
			{
				smTM_ROT* psKeyRotation = (smTM_ROT*)psRawObjectFrom->paKeyRotation + k;
				smFMATRIX* psRotation = (smFMATRIX*)psRawObjectFrom->paRotation + k;

				psKeyRotation->frame += iNewAnimationStartFrame;

				InjectKeyRotation((smOBJ3D*)psRawObjectTo, -1, psKeyRotation->frame, psKeyRotation, psRotation);
			}

			//WRITEDBG( "Copied %d Key Rotations for Frames %d,%d", psRawObjectFrom->iKeyRotationCount, iNewAnimationStartFrame, iNewLastFrame );
		}
		else
		{
			smTM_ROT* rot = (smTM_ROT*)&sKeyRotation;

			InjectKeyRotation((smOBJ3D*)psRawObjectTo, -1, iNewAnimationStartFrame, rot, &sRotation);
			InjectKeyRotation((smOBJ3D*)psRawObjectTo, -1, iNewLastFrame, rot, &sRotation);

			psNewRotFrame->iKeyFrameCount += 2;

			//WRITEDBG( "Created Key Rotation for Frames %d,%d", iNewAnimationStartFrame, iNewLastFrame );
		}

		if (psRawObjectFrom->iKeyPositionCount > 0)
		{
			for (int k = 0; k < psRawObjectFrom->iKeyPositionCount; k++)
			{
				RawSMDKeyPosition* psKeyPosition = psRawObjectFrom->paKeyPosition + k;

				psKeyPosition->iFrame += iNewAnimationStartFrame;

				InjectKeyPosition(psRawObjectTo, -1, psKeyPosition->iFrame, psKeyPosition);
			}

			//WRITEDBG( "Copied %d Key Positions for Frames %d,%d", psRawObjectFrom->iKeyPositionCount, iNewAnimationStartFrame, iNewLastFrame );
		}
		else
		{
			InjectKeyPosition(psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyPosition);
			InjectKeyPosition(psRawObjectTo, -1, iNewLastFrame, &sKeyPosition);

			psNewTraFrame->iKeyFrameCount += 2;

			//WRITEDBG( "Created Key Position for Frames %d,%d", iNewAnimationStartFrame, iNewLastFrame );
		}

		if (psRawObjectFrom->iKeyScaleCount > 0)
		{
			for (int k = 0; k < psRawObjectFrom->iKeyScaleCount; k++)
			{
				RawSMDKeyScale* psKeyScale = psRawObjectFrom->paKeyScale + k;

				psKeyScale->iFrame += iNewAnimationStartFrame;

				InjectKeyScale(psRawObjectTo, -1, psKeyScale->iFrame, psKeyScale);
			}

			//WRITEDBG( "Copied %d Key Scales for Frames %d,%d", psRawObjectFrom->iKeyScaleCount, iNewAnimationStartFrame, iNewLastFrame );
		}
		else
		{
			InjectKeyScale(psRawObjectTo, -1, iNewAnimationStartFrame, &sKeyScale);
			InjectKeyScale(psRawObjectTo, -1, iNewLastFrame, &sKeyScale);

			psNewSclFrame->iKeyFrameCount += 2;

			//WRITEDBG( "Created Key Scale for Frames %d,%d", iNewAnimationStartFrame, iNewLastFrame );
		}

		MergeKeyRotations(psRawObjectTo);
		MergeKeyPositions(psRawObjectTo);
		MergeKeyScalings(psRawObjectTo);

		psNewRotFrame->iEndFrame = iNewLastFrame;
		psNewTraFrame->iEndFrame = iNewLastFrame;
		psNewSclFrame->iEndFrame = iNewLastFrame;

		psNewRotFrame->iKeyFrameCount = psRawObjectTo->iKeyRotationCount;
		psNewTraFrame->iKeyFrameCount = psRawObjectTo->iKeyPositionCount;
		psNewSclFrame->iKeyFrameCount = psRawObjectTo->iKeyScaleCount;

		//WRITEDBG( "Object %s Done!", psRawObjectFrom->szObjectName );
	}

	//Set New Last Frame in SMB Header
	smbI2.sRawHeader.aFrame[0].iEndFrame = iNewLastFrame;

	if (vAnimations.size())
	{
		int iBaseStartFrame = iNewAnimationStartFrame / 160;

		for (const auto& sCustomAnimation : vAnimations)
		{
			//Create INX Animation
			RawINXAnimation sINXAnimation;
			ZeroMemory(&sINXAnimation, sizeof(sINXAnimation));
			sINXAnimation.iType = eAnimationType;
			sINXAnimation.iStartFrame = iBaseStartFrame;
			sINXAnimation.iEndFrame = iBaseStartFrame + sCustomAnimation.iLength;
			sINXAnimation.iAreaType = iAreaType;
			sINXAnimation.iSubFileFrameNumber = 1;
			sINXAnimation.uaSkill[0] = uSkillCode;
			sINXAnimation.iLoop = bLoop;
			sINXAnimation.uClassFlag = uClassFlag;
			sINXAnimation.saItemIndex[0] = 0;
			sINXAnimation.iItemIndexCount = 0;

			for (const auto& pItem : vItems)
				sINXAnimation.saItemIndex[sINXAnimation.iItemIndexCount++] = atoi(pItem.c_str());

			//Insert Animation into INX File
			AddINXAnimation(&inxI.sRawFile, &sINXAnimation);
			iBaseStartFrame = sINXAnimation.iEndFrame + 1;
		}
	}
	else
	{
		//Create INX Animation
		RawINXAnimation sINXAnimation;
		ZeroMemory(&sINXAnimation, sizeof(sINXAnimation));
		sINXAnimation.iType = eAnimationType;
		sINXAnimation.iStartFrame = iNewAnimationStartFrame / 160;
		sINXAnimation.iEndFrame = iNewLastFrame / 160;
		sINXAnimation.iAreaType = iAreaType;
		sINXAnimation.iSubFileFrameNumber = 1;
		sINXAnimation.uaSkill[0] = uSkillCode;
		sINXAnimation.iLoop = bLoop;
		sINXAnimation.uClassFlag = uClassFlag;
		sINXAnimation.saItemIndex[0] = 0;
		sINXAnimation.iItemIndexCount = 0;

		for (const auto& pItem : vItems)
			sINXAnimation.saItemIndex[sINXAnimation.iItemIndexCount++] = atoi(pItem.c_str());

		//Insert Animation into INX File
		AddINXAnimation(&inxI.sRawFile, &sINXAnimation);
	}

	//Save SMB
	Export::SMDFile smbO(pszSMBFileTo);
	smbO.SaveModel(&smbI2.sRawHeader, &smbI2.sRawMaterialHeader, smbI2.psaRawObject);

	//Save INX
	Export::INXFile inxO(pszINXFileTo);
	inxO.Save(&inxI.sRawFile);

	//Done
	//WRITEDBG( "InjectionAnimation() Done!" );

	return TRUE;
}
