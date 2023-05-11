// ----------------------------------------------------------------------------
//! @file	Event.cpp
//! @brief	イベントに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "Event.h"

using namespace alt;

BOOL Event::Create (
	LPCTSTR lpctszName, BOOL bManualReset, BOOL bInitialState)
{
	BOOL ret = TRUE;

	_hObject = ::CreateEvent (
		NULL, bManualReset, bInitialState, lpctszName);
	if (_hObject == INVALID_HANDLE_VALUE)
	{
		ret = FALSE;
	}

	return ret;
}

BOOL Event::Open (LPCTSTR lpctszName)
{
	BOOL ret = TRUE;

	_hObject = ::OpenEvent (EVENT_ALL_ACCESS, FALSE, lpctszName);
	if (_hObject == INVALID_HANDLE_VALUE)
	{
		ret = FALSE;
	}

	return ret;
}

BOOL Event::Set () const
{
	return ::SetEvent (_hObject);
}

BOOL Event::Reset () const
{
	return ::ResetEvent (_hObject);
}

BOOL Event::Pulse () const
{
	return ::PulseEvent (_hObject);
}