// ----------------------------------------------------------------------------
//! @file	UdpConnector.cpp
//! @brief	UDP通信に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.01.03

#include "pch.h"
#include "UdpConnector.h"

using namespace alt;

UdpConnector::UdpConnector (SOCKET socket)
{
	_socket = socket;
	ZeroMemory (&_sockAddrIn, sizeof (_sockAddrIn));
}

UdpConnector::UdpConnector ()
	:SocketLibrary ()
{
	ZeroMemory (&_sockAddrIn, sizeof (_sockAddrIn));
}

INT UdpConnector::Send (LPVOID lpvBuf, DWORD dwSize) const
{
	int ret = ::sendto (
		_socket, (PCHAR)lpvBuf, (int)dwSize, 0, (const sockaddr*)&_sockAddrIn,
		sizeof (_sockAddrIn));

	return ret;
}

INT UdpConnector::Recv (LPVOID lpvBuf, DWORD dwSize) const
{
	INT ret = ::recv (_socket, (PCHAR)lpvBuf, (int)dwSize, 0);

	return ret;
}

BOOL UdpConnector::SetSockAddr (LPCTSTR lpctszAddress, u_short portNo)
{
	BOOL response = TRUE;

	_sockAddrIn.sin_family = AF_INET;
	_sockAddrIn.sin_port = htons (portNo);
	if (lpctszAddress != NULL)
	{
		INT ret = ::InetPton (AF_INET, lpctszAddress, &(_sockAddrIn.sin_addr));
		if (ret != 1)
		{
			response = FALSE;
		}
	}

	return response;
}