// ----------------------------------------------------------------------------
//! @file	WindowsLibrary.cpp
//! @brief	WindowsAPIを集約したプロジェクトファイル
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "WindowsLibrary.h"

using namespace alt;

#pragma comment (linker, "/section:SharedZone,rws")
#pragma data_seg("SharedZone")
// ----------------------------------------------------------------------------
namespace SharedZone
{
	// この#pragma区間はDLLを共有するEXE間で同じデータを見ることになります。
	ULONGLONG testCounter = 0;
}
// ----------------------------------------------------------------------------
#pragma data_seg()

BOOL DllMain (
	HMODULE hModule, DWORD  dwReasonForCall, LPVOID lpvReserved)
{
	TCHAR tszMessage[80] = { 0 };

	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			wsprintf (tszMessage, _T ("WindowsLibrary.dll DLL_PROCESS_ATTACH.\n"));
			break;

		case DLL_THREAD_ATTACH:
			wsprintf (tszMessage, _T ("WindowsLibrary.dll DLL_THREAD_ATTACH.\n"));
			break;

		case DLL_THREAD_DETACH:
			wsprintf (tszMessage, _T ("WindowsLibrary.dll DLL_THREAD_DETACH.\n"));
			break;

		case DLL_PROCESS_DETACH:
			wsprintf (tszMessage, _T ("WindowsLibrary.dll DLL_PROCESS_DETACH.\n"));
			break;
	}
	OutputDebugString (tszMessage);

	return TRUE;
}

VOID WindowsLibrary::SetErrNo ()
{
	_dwErrorNo = ::GetLastError ();
}

HandleLibrary::HandleLibrary ()
{
	_hObject = INVALID_HANDLE_VALUE;
}

HandleLibrary::~HandleLibrary ()
{
	this->Close ();
}

HANDLE HandleLibrary::GetHandle () const
{
	return _hObject;
}

BOOL HandleLibrary::Close ()
{
	BOOL ret = TRUE;

	if (_hObject != INVALID_HANDLE_VALUE)
	{
		ret = ::CloseHandle (_hObject);
		_hObject = INVALID_HANDLE_VALUE;
	}

	return ret;
}

DWORD ReadWriteHandleLibrary::Read (LPVOID lpvBuffer, DWORD dwSize) const
{
	DWORD dwNumberOfBytesRead = 0;

	BOOL ret = ::ReadFile (
		_hObject,
		lpvBuffer,
		dwSize,
		&dwNumberOfBytesRead,
		NULL);
	if (ret == FALSE)
	{
		dwNumberOfBytesRead = (DWORD)-1;
	}

	return dwNumberOfBytesRead;
}

DWORD ReadWriteHandleLibrary::Write (LPCVOID lpcvBuffer, DWORD dwSize) const
{
	DWORD dwNumberOfBytesWritten = 0;

	BOOL ret = ::WriteFile (
		_hObject,
		lpcvBuffer,
		dwSize,
		&dwNumberOfBytesWritten,
		NULL);
	if (ret == FALSE)
	{
		dwNumberOfBytesWritten = (DWORD)-1;
	}

	return dwNumberOfBytesWritten;
}

BOOL ReadWriteHandleLibrary::Flush () const
{
	return ::FlushFileBuffers (_hObject);
}

DWORD WaitableHandleLibrary::Wait (DWORD dwWaitTime) const
{
	return ::WaitForSingleObject (_hObject, dwWaitTime);
}

SocketLibrary::SocketLibrary ()
{
	_socket = INVALID_SOCKET;
}

SocketLibrary::~SocketLibrary ()
{
	this->Close ();
}

int SocketLibrary::Close ()
{
	int ret = 0;

	if (_socket != INVALID_SOCKET)
	{
		ret = ::closesocket (_socket);
		_socket = INVALID_SOCKET;
	}

	return ret;
}

DWORD SocketLibrary::GetErrNo () const
{
	return static_cast<DWORD>(::WSAGetLastError ());
}

BOOL SocketLibrary::SetKeepAliveValue (
	ULONG onoff, ULONG time, ULONG interval) const
{
	struct tcp_keepalive keepAlive = { 0, 0, 0 };
	keepAlive.onoff = onoff;
	keepAlive.keepalivetime = time;
	keepAlive.keepaliveinterval = interval;
	DWORD dwVal = 0;

	int ret = this->WSAIoctl (
		SIO_KEEPALIVE_VALS, &keepAlive, sizeof (keepAlive), NULL, 0, &dwVal,
		NULL, NULL);

	return ret == 0 ? TRUE : FALSE;
}

int SocketLibrary::WSAIoctl (
	DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD dwInBuffer,
	LPVOID lpvOutBuffer, DWORD dwOutBuffer, LPDWORD lpdwBytesReturned,
	LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) const
{
	return ::WSAIoctl (
		_socket, dwIoControlCode, lpvInBuffer, dwInBuffer, lpvOutBuffer,
		dwOutBuffer, lpdwBytesReturned, lpOverlapped, lpCompletionRoutine);
}