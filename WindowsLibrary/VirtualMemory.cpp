// ----------------------------------------------------------------------------
//! @file	VirtualMemory.cpp
//! @brief	仮想メモリに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.2.24

#include "pch.h"
#include "VirtualMemory.h"

using namespace alt;

// VirtualMemory1 -----------------------------------------------------

VirtualMemory1::VirtualMemory1 ()
{
	_currentSize = 0;
}

LPVOID VirtualMemory1::Allocate (SIZE_T bytes, DWORD dwFlags)
{
	_currentSize = bytes;
	return ::VirtualAlloc (NULL, bytes, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
}

LPVOID VirtualMemory1::Reallocate (LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags)
{
	SIZE_T previousSize = _currentSize;
	LPVOID lpvRenew = Allocate (bytes, dwFlags);
	if (lpvRenew != NULL)
	{
		CopyMemory (lpvRenew, lpvMem, previousSize);
		Free (lpvMem);
		_currentSize = bytes;
	}

	return lpvRenew;
}

BOOL VirtualMemory1::Free (LPVOID lpvMem)
{
	_currentSize = 0;
	return ::VirtualFree (lpvMem, 0, MEM_RELEASE);
}

// VirtualMemory -----------------------------------------------------

LPVOID VirtualMemory::Reserve (SIZE_T size) const
{
	return ::VirtualAlloc (NULL, size, MEM_RESERVE, PAGE_READWRITE);
}

BOOL VirtualMemory::Release (LPVOID lpvMem) const
{
	return ::VirtualFree (lpvMem, 0, MEM_RELEASE);
}

LPVOID VirtualMemory::Commit (LPVOID lpvMem, SIZE_T size) const
{
	return ::VirtualAlloc (lpvMem, size, MEM_COMMIT, PAGE_READWRITE);
}

BOOL VirtualMemory::Decommit (LPVOID lpvMem, SIZE_T size) const
{
#pragma warning(push)
#pragma warning(disable:6250)
	return ::VirtualFree (lpvMem, size, MEM_DECOMMIT);
#pragma warning(pop)
}

BOOL VirtualMemory::Lock (LPVOID lpvMem, SIZE_T size) const
{
	return ::VirtualLock (lpvMem, size);
}

BOOL VirtualMemory::Unlock (LPVOID lpvMem, SIZE_T size) const
{
	return ::VirtualUnlock (lpvMem, size);
}