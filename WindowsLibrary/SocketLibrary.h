// ----------------------------------------------------------------------------
//! @file	SocketLibrary.h
//! @brief	�\�P�b�g�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2023.5.13

#pragma once

#include <WinSock2.h>
#include "WindowsLibrary.h"

namespace alt
{
	//! @class	SocketLibrary
	//! @brief	WinSock���g�p����API���W�񂵂��N���X
	class DLL_DECLSPEC SocketLibrary : public WindowsLibrary
	{
	public:
		//! @brief	�R���X�g���N�^
		//! @note	_socket�� INVALID_SOCKET �ŏ��������܂��B
		APIENTRY SocketLibrary ();

		//! @brief	�f�X�g���N�^
		//! @note	�Ǘ����Ă���\�P�b�g���N���[�Y���܂��B
		APIENTRY ~SocketLibrary ();

		//! @brief		�\�P�b�g���g�p����Ă����ꍇ�A�N���[�Y���܂��B
		//! @return		�\�P�b�g�N���[�Y�������������ۂ��B
		//! @retval		closesocket()�̖߂�l
		//! @retval		����(0) �\�P�b�g�ł͂Ȃ�(WSAENOTSOCK)
		//! @details	WinSock API�̖߂�l�̓l�b�g�Ŋm�F
		int APIENTRY Close ();

		//! @brief		WinSock API�Ăяo�����ɃG���[�ƂȂ������A
		//!				�G���[�̏ڍׂ�Ԃ��܂��B
		//! @return		WinSock API�̃G���[���̃G���[�ԍ�
		//! @retval		DWORD �ڍׂ̓l�b�g�ŁB
		//! @sa			http://chokuto.ifdef.jp/advanced/prm/winsock_error_code.html
		//! @sa         https://learn.microsoft.com/ja-jp/windows/win32/winsock/windows-sockets-error-codes-2
		//! @details	WSAGetLastError()���g�p���܂��B
		DWORD APIENTRY GetErrNo () const;

		//! @brief		KeepAlive���o�^�C�~���O�A�Ԋu���Z�b�V�����ʂɐݒ肵�܂��B
		//! @param[in]	onoff (0:OFF 1:ON)
		//! @param[in]	time ���ʐM�ƂȂ���������KeepAlive���o�J�n�܂ł̎���(�~���b)
		//! @param[in]	interval �ؒf���m��Ƀ��g���C���o���鎞�̃C���^�[�o��
		//! @return		WinSock API�̃G���[���̃G���[�ԍ�
		//! @retval		DWORD �ڍׂ̓l�b�g�ŁB
		//! @details	SetSockOption()�Őݒ肳���KeepAlive���o�ݒ�̓��W�X�g����
		//! 			�ݒ�l(����l�͂Q���Ԍ�)�ɏ]���܂��B���̊֐��ɂ��AKeepAlive
		//! 			�̐ݒ�^�C�~���O���Z�b�V�����ʂɐݒ肷�邱�Ƃ��ł��܂��B
		BOOL APIENTRY SetKeepAliveValue (
			ULONG onoff, ULONG time, ULONG interval) const;

	protected:
		int APIENTRY WSAIoctl (
			DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD dwInBuffer,
			LPVOID lpvOutBuffer, DWORD dwOutBuffer, LPDWORD lpdwBytesReturned,
			LPWSAOVERLAPPED lpOverlapped,
			LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) const;

		//! ���̃N���X�ŊǗ�����\�P�b�g�I�u�W�F�N�g
		SOCKET _socket;
	};
}