// ----------------------------------------------------------------------------
//! @file	WatchDogTimer.cpp
//! @brief	タイマーに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "WatchDogTimer.h"

using namespace alt;

WatchDogTimer::WatchDogTimer (UINT uPeriod)
{
	_timeup = NULL;
	_uPeriod = uPeriod;
}

WatchDogTimer::WatchDogTimer (skeleton::ITimeup* timeup, UINT uPeriod)
{
	_timeup = timeup;
	_uPeriod = uPeriod;
}

WatchDogTimer::~WatchDogTimer ()
{
	::timeEndPeriod (_uPeriod);
}

BOOL WatchDogTimer::InitTimer (LPCTSTR lpctszName)
{
	BOOL ret = TRUE;

	::timeBeginPeriod (_uPeriod);

	_hObject = ::CreateWaitableTimer (NULL, 0, lpctszName);
	if (_hObject == INVALID_HANDLE_VALUE)
	{
		ret = FALSE;
	}

	return ret;
}

BOOL WatchDogTimer::StartTimer (DWORD dwTime) const
{
	BOOL ret = FALSE;
	LARGE_INTEGER interval = { 0, 0 };
	// マイナス設定は現在時刻から、プラス設定はエポックタイムから
	interval.QuadPart = -((LONGLONG)dwTime * 10000);

	ret = ::SetWaitableTimer (
		_hObject, &interval, (LONG)dwTime,
		_timeup == NULL ? NULL : _timeup->TimeupCall,
		_timeup, FALSE);
	if (ret == FALSE)
	{
		return ret;
	}

	return TRUE;
}

DWORD WatchDogTimer::WaitTimer () const
{
	return ::WaitForSingleObjectEx (_hObject, INFINITE, TRUE);
}

BOOL WatchDogTimer::StopTimer () const
{
	return ::CancelWaitableTimer (_hObject);
}