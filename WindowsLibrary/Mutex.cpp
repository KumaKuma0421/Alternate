// ----------------------------------------------------------------------------
//! @file	Mutex.cpp
//! @brief	ミューテックスに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.29

#include "pch.h"
#include "Mutex.h"

using namespace alt;

BOOL Mutex::Create (LPCTSTR lpctszName)
{
	_hObject = ::CreateMutex (NULL, FALSE, lpctszName);

	return _hObject == INVALID_HANDLE_VALUE ? FALSE : TRUE;
}

BOOL Mutex::Open (LPCTSTR lpctszName)
{
	_hObject = ::OpenMutex (MUTEX_ALL_ACCESS, FALSE, lpctszName);

	return _hObject == INVALID_HANDLE_VALUE ? FALSE : TRUE;
}

BOOL Mutex::Release () const
{
	return ::ReleaseMutex (_hObject);
}