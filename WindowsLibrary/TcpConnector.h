// ----------------------------------------------------------------------------
//! @file	TcpConnector.h
//! @brief	TCP�ʐM�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	TcpConnector
	//! @brief	TCP�ʐM�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC TcpConnector : public SocketLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		//! @param socket �g�p����\�P�b�g
		APIENTRY TcpConnector (SOCKET socket) { _socket = socket; };

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
			LPVOID lpvBuf, DWORD dwSize, BOOL isBlocking = TRUE) const;

	protected:
		APIENTRY TcpConnector () { _socket = INVALID_SOCKET; };
	};
}