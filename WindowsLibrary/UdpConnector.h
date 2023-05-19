// ----------------------------------------------------------------------------
//! @file	UdpConnector.h
//! @brief	UDP�ʐM�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2021.4.25

#pragma once

#include "SocketLibrary.h"

namespace alt
{
	//! @class	UdpConnector
	//! @brief	UDP�ʐM�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC UdpConnector : public SocketLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		//! @param socket �g�p����\�P�b�g
		APIENTRY UdpConnector (SOCKET socket);

		//! @brief UDP���M��A�h���X�̐ݒ�
		//! @param lpctszAddress �����IP�A�h���X
		//! @param portNo ����̃|�[�g�ԍ�
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY SetSockAddr (LPCTSTR lpctszAddress, u_short portNo);

		//! @brief UDP���M����
		//! @param lpvBuf ���M�f�[�^
		//! @param dwSize ���M�T�C�Y
		//! @return INT ���M�T�C�Y
		INT APIENTRY Send (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief UDP��M����
		//! @param lpvBuf ��M�f�[�^
		//! @param dwSize ��M�T�C�Y
		//! @return INT ��M�T�C�Y
		INT APIENTRY Recv (LPVOID lpvBuf, DWORD dwSize) const;

	protected:
		UdpConnector ();
		SOCKADDR_IN	_sockAddrIn;
	};
}