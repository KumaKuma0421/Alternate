// ----------------------------------------------------------------------------
//! @file	TcpConnector.h
//! @brief	TCP�ʐM�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "SocketLibrary.h"
#include <ws2ipdef.h>

namespace alt
{
	//! @class	TcpConnector
	//! @brief	TCP�ʐM�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC TcpConnector : public SocketLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		//! @param socket �\�P�b�g���ʎq
		//! @param lpctszAcceptIPAddress �Ό���IP�A�h���X
		//! @param wAcceptPort �Ό��̃|�[�g�ԍ�
		APIENTRY TcpConnector(
			SOCKET socket, LPCTSTR lpctszAcceptIPAddress, u_short wAcceptPort);

		//! @brief �󂯓�����IP�A�h���X���擾
		//! @return LPCTSTR �����IP�A�h���X
		LPCTSTR GetAcceptedAddress () { return _tszAcceptedIPAddress; };

		//! @brief �����̃|�[�g�ԍ����擾
		//! @return USHORT �����|�[�g�ԍ�
		USHORT GetAcceptedPortNo () { return _wAcceptedPort; };

		//! @brief TCP���M����
		//! @param lpvBuf ���M�f�[�^
		//! @param dwSize ���M�T�C�Y
		//! @return INT ���M�f�[�^�T�C�Y
		INT APIENTRY Send (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief TCP��M����
		//! @param lpvBuf ��M�f�[�^
		//! @param dwSize ��M�T�C�Y
		//! @param isBlocking �u���b�L���O�ҋ@���邩�ۂ�
		//! @return INT ��M�f�[�^�T�C�Y
		INT APIENTRY Recv (
			LPVOID lpvBuf, DWORD dwSize, BOOL isBlocking = TRUE);

		//! @brief Recv()�ҋ@���̃L�����Z������
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL CancelRecv ();

	protected:
		APIENTRY TcpConnector () = delete;
		
		TCHAR _tszAcceptedIPAddress[INET_ADDRSTRLEN];
		USHORT _wAcceptedPort;
		WSAOVERLAPPED _RecvOverlapped;
	};
}