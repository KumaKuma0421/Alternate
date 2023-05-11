// ----------------------------------------------------------------------------
//! @file	SharedMemory.cpp
//! @brief	共有メモリに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.2

#include "pch.h"
#include "SharedMemory.h"

using namespace alt;

SharedMemory::SharedMemory ()
	:HandleLibrary ()
{
	_lpvMapped = NULL;
}

SharedMemory::~SharedMemory ()
{
	if (_lpvMapped != NULL)
	{
		this->Unmapped ();
	}
}

BOOL SharedMemory::Create (LPCTSTR lpctszName, DWORD dwSize)
{
	_hObject = ::CreateFileMapping (
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		dwSize,
		lpctszName);

	return _hObject == NULL ? FALSE : TRUE;
}

BOOL SharedMemory::Open (LPCTSTR lpctszName)
{
	_hObject = ::OpenFileMapping (
		FILE_MAP_ALL_ACCESS,
		FALSE,
		lpctszName);

	return _hObject == NULL ? FALSE : TRUE;
}

LPVOID SharedMemory::Mapped ()
{
	_lpvMapped = ::MapViewOfFile (
		_hObject,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		0);

	return _lpvMapped;
}

BOOL SharedMemory::Unmapped ()
{
	BOOL ret = ::UnmapViewOfFile (_lpvMapped);
	if (ret) _lpvMapped = NULL;

	return ret;
}