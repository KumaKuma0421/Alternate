// ----------------------------------------------------------------------------
//! @file	SocketLibrary.cpp
//! @brief	ソケットに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2023.5.13

#include "pch.h"
#include "SocketLibrary.h"

using namespace alt;

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