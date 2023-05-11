// ----------------------------------------------------------------------------
//! @file	Process.cpp
//! @brief	プロセスに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.31

#include "pch.h"
#include "Process.h"

using namespace alt;

Process::Process ()
{
	ZeroMemory (&_pi, sizeof (_pi));
	ZeroMemory (&_si, sizeof (_si));
	_si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	_si.wShowWindow = SW_HIDE;
	_dwCreationFlags = NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT;
}

Process::~Process ()
{
	::CloseHandle (_pi.hThread);
	::CloseHandle (_pi.hProcess);
}

BOOL Process::Create (
	LPCTSTR lpctszApplicationName,
	LPTSTR lptszCommandLine,
	LPCTSTR lpctszCurrentDirectory)
{
	BOOL ret = FALSE;

	_si.cb = sizeof (STARTUPINFO);
	_si.lpReserved = NULL;
	_si.cbReserved2 = 0;
	_si.lpReserved2 = NULL;

	do
	{
		ret = ConnectHandles ();
		if (!ret) break;

		ret = ::CreateProcess (
			lpctszApplicationName,
			lptszCommandLine,
			NULL,
			NULL,
			TRUE,
			_dwCreationFlags,
			NULL,
			lpctszCurrentDirectory,
			&_si,
			&_pi);
		if (!ret) break;

		ret = TRUE;
	} while (false);

	return ret;
}

DWORD Process::Wait (DWORD dwTime) const
{
	return ::WaitForSingleObject (_pi.hProcess, dwTime);
}

BOOL Process::CreateAsUser ()
{
	// TODO:CreateProcessAsUser()
	return FALSE;
}

BOOL Process::CreateWithLogon ()
{
	// TODO:CreateProcessWithLogon()
	return FALSE;
}

DWORD Process::GetExitCode () const
{
	DWORD dwRet;
	BOOL ret = ::GetExitCodeProcess (_pi.hProcess, &dwRet);

	return ret == TRUE ? dwRet : -1;
}

BOOL Process::Terminate () const
{
	return ::TerminateProcess (_pi.hProcess, 0);
}

VOID Process::SetWindowInfo (
	DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, WORD wShowWindow)
{
	_si.dwX = dwX;
	_si.dwY = dwY;
	_si.dwXSize = dwWidth;
	_si.dwYSize = dwHeight;
	_si.dwFlags = STARTF_USEPOSITION;
	_si.dwFlags |= STARTF_USESIZE;
	_si.dwFlags |= STARTF_USESHOWWINDOW;
	_si.wShowWindow = wShowWindow;
}

VOID Process::SetConsoleInfo (
	DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, DWORD dwXChars,
	DWORD dwYChars)
{
	_si.dwX = dwX;
	_si.dwY = dwY;
	_si.dwXSize = dwWidth;
	_si.dwYSize = dwHeight;
	_si.dwXCountChars = dwXChars;
	_si.dwYCountChars = dwYChars;
	_si.dwFlags = STARTF_USECOUNTCHARS;
	_si.dwFlags |= STARTF_USESTDHANDLES;
	_si.dwFlags |= STARTF_FORCEONFEEDBACK;
	_dwCreationFlags |= CREATE_NEW_CONSOLE;
}

VOID Process::SetTitle (LPTSTR lptszConsoleTitle)
{
	_si.lpTitle = lptszConsoleTitle;
}

BOOL Process::SetPriority (Priority priority) const
{
	return ::SetPriorityClass (GetProcessHandle (), (DWORD)priority);
}

DWORD Process::Peek (LPVOID lpvBuf, DWORD dwSize) const
{
	return _pipeRead.Peek (lpvBuf, dwSize);
}

DWORD Process::Read (LPVOID lpvBuf, DWORD dwSize) const
{
	return _pipeRead.Read (lpvBuf, dwSize);
}

DWORD Process::Write (LPVOID lpvBuf, DWORD dwSize) const
{
	return _pipeWrite.Write (lpvBuf, dwSize);
}

// private functions --------------------------------------------------

BOOL Process::ConnectHandles ()
{
	BOOL ret = FALSE;

	do
	{
		ret = _pipeRead.Create ();
		if (!ret) break;

		ret = _pipeWrite.Create ();
		if (!ret) break;

		ret = _pipeError.Create ();
		if (!ret) break;

		_si.hStdInput = _pipeWrite.GetReadHandle ();
		_si.hStdOutput = _pipeRead.GetWriteHandle ();
		_si.hStdError = _pipeRead.GetWriteHandle ();

		ret = TRUE;
	} while (false);

	return ret;
}