// ----------------------------------------------------------------------------
//! @file	TcpConnector.cpp
//! @brief	TCP通信に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "TcpConnector.h"

using namespace alt;

TcpConnector::TcpConnector (
	SOCKET socket, LPCTSTR lpctszAcceptIPAddress, USHORT wAcceptPort)
{
	_socket = socket;
	ZeroMemory (_tszAcceptedIPAddress, INET_ADDRSTRLEN);
	CopyMemory (_tszAcceptedIPAddress, lpctszAcceptIPAddress,
				_tcslen (lpctszAcceptIPAddress) * sizeof (TCHAR));
	_wAcceptedPort = wAcceptPort;
	SecureZeroMemory ((PVOID)&_RecvOverlapped, sizeof (_RecvOverlapped));
}

INT TcpConnector::Send (LPVOID lpvBuf, DWORD dwSize) const
{
	int ret = ::send (_socket, (PCHAR)lpvBuf, (int)dwSize, 0);

	return ret;
}

INT TcpConnector::Recv (LPVOID lpvBuf, DWORD dwSize, BOOL isBlocking)
{
	INT ret = INT_MIN;
	WSABUF DataBuf;
	DWORD dwRecvBytes;
	DWORD dwFlags = 0;
	DWORD dwRemain = dwSize;

	_RecvOverlapped.hEvent = ::WSACreateEvent ();
	if (_RecvOverlapped.hEvent == nullptr)
	{
		ret = -1;
		return ret;
	}

	while (dwRemain!= 0)
	{
		int rc;
		PCHAR currentPos = (PCHAR)lpvBuf + (dwSize - dwRemain);
		DataBuf.len = dwRemain;
		DataBuf.buf = currentPos;

		rc = WSARecv (_socket, &DataBuf, 1, &dwRecvBytes, &dwFlags,
					  &_RecvOverlapped, nullptr);
		if ((rc == SOCKET_ERROR) && (WSA_IO_PENDING != GetErrNo ()))
		{
			ret = -2;
			break;
		}

		rc = WSAWaitForMultipleEvents (1, &_RecvOverlapped.hEvent, TRUE,
									   INFINITE, TRUE);
		if (rc == WSA_WAIT_FAILED)
		{
			ret = -3;
			break;
		}

		rc = WSAGetOverlappedResult (_socket, &_RecvOverlapped, &dwRecvBytes,
									 FALSE, &dwFlags);
		if (rc == FALSE)
		{
			ret = -4;
			break;
		}

		rc = WSAResetEvent (_RecvOverlapped.hEvent);
		if (rc == FALSE)
		{
			ret = -5;
			break;
		}

		if (isBlocking)
		{
			dwRemain -= dwRecvBytes;
			ret = dwSize - dwRemain;
			if (dwRecvBytes == 0)
			{
				ret = 0;
				break;
			}
		}
		else
		{
			ret = (INT)dwRecvBytes;
			break;
		}
	}

	if (WSACloseEvent (_RecvOverlapped.hEvent) == FALSE)
	{
		ret = -6;
	}

	return ret;
}

BOOL TcpConnector::CancelRecv ()
{
	return ::WSASetEvent (_RecvOverlapped.hEvent);
}
