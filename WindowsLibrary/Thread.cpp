// ----------------------------------------------------------------------------
//! @file	Thread.cpp
//! @brief	スレッドに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "Thread.h"

using namespace alt;

Thread::Thread ()
{
	_callback = NULL;
	_dwThreadID = 0;
}

Thread::Thread (skeleton::ICallback* callback)
{
	_callback = callback;
	_dwThreadID = 0;
}

BOOL Thread::Create (
	PTHREAD_START_ROUTINE function, LPVOID lpvParam, DWORD dwCreationFlag)
{
	BOOL ret = TRUE;

	PTHREAD_START_ROUTINE threadFunction =
		function == NULL ? _callback->Invoker : function;
	lpvParam = function == NULL ? _callback : lpvParam;

	_hObject = ::CreateThread (
		NULL, 0, threadFunction, lpvParam, dwCreationFlag, &_dwThreadID);
	if (_hObject == NULL)
	{
		ret = FALSE;
	}

	return ret;
}

BOOL Thread::Suspend () const
{
	return ::SuspendThread (_hObject) == (DWORD)-1 ? FALSE : TRUE;
}

BOOL Thread::Resume () const
{
	return ::ResumeThread (_hObject) == (DWORD)-1 ? FALSE : TRUE;
}

BOOL Thread::SetPriority (ThreadPriority priority) const
{
	return ::SetThreadPriority (_hObject, (int)priority);
}