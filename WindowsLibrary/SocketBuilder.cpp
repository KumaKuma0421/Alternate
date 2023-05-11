// ----------------------------------------------------------------------------
//! @file	SocketBuilder.cpp
//! @brief	�\�P�b�g�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2020.01.03

#include "pch.h"
#include "SocketBuilder.h"

using namespace alt;

SocketBuilder::SocketBuilder ()
{
	ZeroMemory (&_wsaData, sizeof (_wsaData));
	ZeroMemory (&_SockAddrIn, sizeof (_SockAddrIn));
}

SocketBuilder::~SocketBuilder ()
{
	this->Close ();

	::WSACleanup (); //! @todo TCPConnector��������������ꍇ���l���ł��Ă��Ȃ�
}

BOOL SocketBuilder::Startup ()
{
	TCHAR tszMessage[80];
	int ret = ::WSAStartup (MAKEWORD (2, 0), &_wsaData);

	switch (ret)
	{
		case 0:
			wsprintf (tszMessage, _T ("WSAStartup() ����"));
			break;

		case WSASYSNOTREADY:
			wsprintf (tszMessage, _T ("��b�ƂȂ�l�b�g���[�N�E�T�u�V�X�e�����A�l�b�g���[�N�ʐM�̏������ł��Ă��܂���B"));
			break;

		case WSAVERNOTSUPPORTED:
			wsprintf (tszMessage, _T ("�v�����ꂽ Windows �\�P�b�g �T�|�[�g�̃o�[�W�����́A���̓���� Windows �\�P�b�g�����ł͒񋟂���܂���B"));
			break;

		case WSAEINPROGRESS:
			wsprintf (tszMessage, _T ("�u���b�L���O Windows �\�P�b�g 1.1 ���삪�i�s���ł��B"));
			break;

		case WSAEPROCLIM:
			wsprintf (tszMessage, _T ("Windows �\�P�b�g�̎����ŃT�|�[�g����Ă���^�X�N�̐��̐����ɒB���܂����B"));
			break;

		case WSAEFAULT:
			wsprintf (tszMessage, _T ("lpWSAData �p�����[�^�[���L���ȃ|�C���^�[�ł͂���܂���B"));
			break;

		default:
			wsprintf (tszMessage, _T ("Unknown error. [%d]"), ret);
			break;
	}
	OutputDebugString (tszMessage);

	if (ret != 0)
	{
		::WSACleanup ();
	}

	return ret == 0 ? TRUE : FALSE;
}

BOOL SocketBuilder::Socket (int af, int type, int protocol)
{
	BOOL response = TRUE;

	_socket = ::socket (af, type, protocol);
	if (_socket == INVALID_SOCKET)
	{
		response = FALSE;
	}

	return response;
}

BOOL SocketBuilder::SetSockAddr (
	ADDRESS_FAMILY family, u_short portNo, LPCTSTR lpctszAddress)
{
	BOOL response = TRUE;

	_SockAddrIn.sin_family = family;
	_SockAddrIn.sin_port = htons (portNo);
	if (lpctszAddress != NULL)
	{
		INT ret = ::InetPton (family, lpctszAddress, &(_SockAddrIn.sin_addr));
		if (ret != 1)
		{
			response = FALSE;
		}
	}

	return response;
}

BOOL SocketBuilder::SetSockOpt (int level, int optname, int value) const
{
	BOOL response = TRUE;

	int ret = ::setsockopt (
		_socket, level, optname, (const char*)&value, sizeof (value));
	if (ret != 0)
	{
		response = FALSE;
	}

	return response;
}

BOOL SocketBuilder::Connect () const
{
	BOOL response = TRUE;

	int ret = ::connect (
		_socket, (PSOCKADDR)&_SockAddrIn, sizeof (_SockAddrIn));
	if (ret != 0)
	{
		response = FALSE;
	}

	return response;
}

BOOL SocketBuilder::Bind () const
{
	BOOL response = TRUE;

	int ret = ::bind (_socket, (PSOCKADDR)&_SockAddrIn, sizeof (_SockAddrIn));
	if (ret != 0)
	{
		response = FALSE;
	}

	return response;
}

BOOL SocketBuilder::Listen () const
{
	BOOL response = TRUE;

	int ret = ::listen (_socket, 5);
	if (ret != 0)
	{
		response = FALSE;
	}

	return response;
}

//! @sa https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-ioctlsocket
BOOL SocketBuilder::Ioctl (long cmd, u_long arg) const
{
	u_long value = arg;
	BOOL response = TRUE;

	int ret = ::ioctlsocket (_socket, cmd, &value);
	if (ret != 0)
	{
		response = FALSE;
	}

	return response;
}

UdpConnector* SocketBuilder::CreateUdpConnector (
	LPCTSTR lpctszIpAddr, u_short portNo)
{
	UdpConnector* response = NULL;
	BOOL ret = FALSE;

	do
	{
		ret = this->Socket (AF_INET, SOCK_DGRAM, 0);
		if (ret == FALSE) break;

		ret = this->SetSockAddr (AF_INET, portNo, lpctszIpAddr);
		if (ret == FALSE) break;

		ret = this->Bind ();
		if (ret == FALSE) break;

		response = new UdpConnector (_socket);
		_socket = INVALID_SOCKET;
		return response;

	} while (FALSE);

	this->Close ();

	return response;
}

//
// @sa http://chokuto.ifdef.jp/advanced/function/connect.html
//
TcpConnector* SocketBuilder::CreateTcpConnector (
	LPCTSTR lpctszIpAddr, u_short portNo, int retryInterval, int retryCount)
{
	TcpConnector* response = NULL;
	BOOL ret = FALSE;

	do
	{
		ret = this->Socket (AF_INET, SOCK_STREAM, 0);
		if (ret == FALSE) break;

		// KeepAlive�̓\�t�g�ł̐ݒ��ON/OFF�����ł��B
		// ���ۂ̐ݒ�l�̓��W�X�g������擾����܂��B
		// HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\Parameters
		// KeepAliveTime(ms) ����l:7,200,000�~���b(2����)
		// KeepAliveInterval(ms) ����l:1,000�~���b(1�b)
		ret = this->SetSockOpt (SOL_SOCKET, SO_KEEPALIVE, 1);
		if (ret == FALSE) break;

		ret = this->SetSockAddr (AF_INET, portNo, lpctszIpAddr);
		if (ret == FALSE) break;

		// connect()�͑Ό��T�[�o�[���ʂ̃}�V���ɂ���ꍇ�A
		// 21�b�̃u���b�L���O���ԂƂȂ�܂�(�Ό������Ȃ��ꍇ�Ȃ�)�B
		// ���̂��߁Aconnect()���Ăяo���O�ɁA�\�P�b�g�֐���񓯊�
		// (�m���u���b�L���O)���[�h�ɕύX���܂��B
		ret = this->Ioctl (FIONBIO, 1);
		if (ret == FALSE) break;

		while (retryCount--)
		{
			ret = this->Connect ();
			if (ret == TRUE || this->GetErrNo () == WSAEISCONN)
			{
				// connect()�ɐ��������ꍇ�A�񓯊�(�m���u���b�L���O)
				// ���[�h��ʏ�̃u���b�L���O���[�h�ɖ߂��܂��B
				// ���s�����ꍇ�́A���̂܂܃N���[�Y����܂��B
				ret = this->Ioctl (FIONBIO, 0);
				if (ret == TRUE)
				{
					response = new TcpConnector (_socket);
					_socket = INVALID_SOCKET;
					return response;
				}
			}
			else
			{
				TCHAR tszMessage[80];
				wsprintf (tszMessage, _T ("connect() error. reason=%d"), this->GetErrNo ());
				OutputDebugString (tszMessage);
				Sleep (retryInterval);
			}
		}
	} while (FALSE);

	this->Close ();

	return response;
}

BOOL SocketBuilder::Prepare (u_short portNo, LPCTSTR lpctszIpAddr)
{
	BOOL response = FALSE;

	do
	{
		response = this->Socket (AF_INET, SOCK_STREAM, 0);
		if (response == FALSE)
		{
			break;
		}

		// �\�P�b�g�ɑ΂���SO_REUSEADDR��ݒ肷��ƁA
		// TIME_WAIT��Ԃ̃|�[�g�����݂��Ă�bind()��
		// �ł���悤�ɂȂ�܂��B
		// @sa also www.geekpage.jp/programming/linux-network/so_reuseaddr.php
		response = this->SetSockOpt (SOL_SOCKET, SO_REUSEADDR, 1);
		if (response == FALSE)
		{
			break;
		}

		response = this->SetSockAddr (AF_INET, portNo, lpctszIpAddr);
		if (response == FALSE)
		{
			break;
		}

		response = this->Bind ();
		if (response == FALSE)
		{
			break;
		}

		// KeepAlive�̓\�t�g�ł̐ݒ��ON/OFF�����ł��B
		// ���ۂ̐ݒ�l�̓��W�X�g������擾����܂��B
		// HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\Parameters
		// KeepAliveTime(ms) ����l:7,200,000�~���b(2����)
		// KeepAliveInterval(ms) ����l:1,000�~���b(1�b)
		response = this->SetSockOpt (SOL_SOCKET, SO_KEEPALIVE, 1);
		if (response == FALSE)
		{
			break;
		}

		response = this->Listen ();
		if (response == FALSE)
		{
			break;
		}

	} while (FALSE);

	return response;
}

TcpConnector* SocketBuilder::Wait ()
{
	TcpConnector* response = NULL;
	BOOL ret = FALSE;

	SOCKADDR_IN mySockAddrIn;
	ZeroMemory (&mySockAddrIn, sizeof (mySockAddrIn));
	int length = sizeof (mySockAddrIn);
	SOCKET mySocket = ::accept (_socket, (PSOCKADDR)&mySockAddrIn, &length);
	if (mySocket != INVALID_SOCKET)
	{
		TCHAR tszAcceptIPAddress[INET_ADDRSTRLEN];
		PCWSTR ret = ::InetNtop (
			AF_INET, &mySockAddrIn.sin_addr, tszAcceptIPAddress,
			INET_ADDRSTRLEN);
		if (ret != NULL)
		{
			int iAcceptPort = ::ntohs (mySockAddrIn.sin_port);

			TCHAR tszMessage[80];
			wsprintf (
				tszMessage, _T ("Accepted (%s,%d)\n"), tszAcceptIPAddress,
				iAcceptPort);
			OutputDebugString (tszMessage);

			response = new TcpConnector (mySocket);
		}
	}

	return response;
}

BOOL SocketBuilder::GetHostByName (
	LPCTSTR lpctszHostName, LPCTSTR lptszIpAddr) const
{
	BOOL response = FALSE;
	INT ret;
	ADDRINFO addrInfo;
	ZeroMemory (&addrInfo, sizeof (addrInfo));
	addrInfo.ai_family = AF_INET;
	addrInfo.ai_socktype = SOCK_STREAM;
	addrInfo.ai_protocol = IPPROTO_TCP;
	ADDRINFO* result = NULL;
	DWORD dwBufferLen = 16;

	do
	{
		ret = ::GetAddrInfo (
			lpctszHostName,
			NULL,
			(const ADDRINFOW*)&addrInfo,
			(PADDRINFOW*)&result);
		if (ret) break;

		ret = ::WSAAddressToString (
			result->ai_addr,
			(DWORD)result->ai_addrlen,
			NULL,
			(LPWSTR)lptszIpAddr,
			&dwBufferLen);
		if (ret) break;

		response = TRUE;
	} while (false);

	return response;
}