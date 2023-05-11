// ----------------------------------------------------------------------------
//! @file	Pipe.cpp
//! @brief	パイプに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.6

#include "pch.h"
#include "Pipe.h"

using namespace alt;

Pipe::Pipe ()
{
	_hRead = INVALID_HANDLE_VALUE;
	_hWrite = INVALID_HANDLE_VALUE;
}

Pipe::~Pipe ()
{
	this->Close ();
}

BOOL Pipe::Create ()
{
	SECURITY_ATTRIBUTES sa = { 0, 0, 0 };
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	return ::CreatePipe (&_hRead, &_hWrite, &sa, 0);
}

DWORD Pipe::Peek (LPVOID lpvBuf, DWORD dwSize) const
{
	DWORD dwReadSize;
	DWORD dwAvail;
	DWORD dwLeft;

	BOOL ret = ::PeekNamedPipe (
		_hRead, lpvBuf, dwSize, &dwReadSize, &dwAvail, &dwLeft);

	return ret == TRUE ? dwReadSize : -1;
}

DWORD Pipe::Read (LPVOID lpvBuf, DWORD dwSize) const
{
	DWORD dwReadSize;

	BOOL ret = ::ReadFile (_hRead, lpvBuf, dwSize, &dwReadSize, NULL);

	return ret == TRUE ? dwReadSize : -1;
}

BOOL Pipe::Write (LPVOID lpvBuf, DWORD dwSize) const
{
	DWORD dwWrittenSize;

	BOOL ret = ::WriteFile (_hWrite, lpvBuf, dwSize, &dwWrittenSize, NULL);

	return ret == TRUE ? dwWrittenSize : -1;
}

BOOL Pipe::Close ()
{
	BOOL ret1 = ::CloseHandle (_hRead);
	_hRead = INVALID_HANDLE_VALUE;
	BOOL ret2 = ::CloseHandle (_hWrite);
	_hWrite = INVALID_HANDLE_VALUE;

	return ret1 == TRUE && ret2 == TRUE ? TRUE : FALSE;
}