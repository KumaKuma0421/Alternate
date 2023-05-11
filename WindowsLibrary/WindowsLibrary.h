// ----------------------------------------------------------------------------
//! @file	WindowsLibrary.h
//! @brief	WindowsAPI���W�񂵂��v���W�F�N�g�t�@�C��
//! @note	DllMain()�̃v���g�^�C�v�������Ő錾���Ă��܂��B
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

BOOL APIENTRY DllMain (
	HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved);

namespace alt
{
	//! @class	WindowsLibrary
	//! @brief	�v���W�F�N�g���ʊ��N���X
	class DLL_DECLSPEC WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY WindowsLibrary () { _dwErrorNo = ERROR_SUCCESS; };

		//! @brief �f�X�g���N�^
		virtual APIENTRY ~WindowsLibrary () {};

		//! @brief WINAPI�G���[�̏���ݒ肵�܂��B
		virtual VOID APIENTRY SetErrNo ();

		//! @brief	WindowsAPI�̃G���[�l���擾���܂��B
		//! @return	WindowAPI�̃G���[���̃G���[�ԍ�
		//! @retval	DWORD �ڍׂ̓l�b�g�ŁB
		//! @sa		http://ir9.jp/prog/ayu/win32err.htm
		//! @sa		https://gallery-code.blogspot.com/2010/05/getlasterror.html
		virtual DWORD APIENTRY GetErrNo () const { return _dwErrorNo; };

	protected:
		DWORD _dwErrorNo;
	};

	//! @class	HandleLibrary
	//! @brief	HANDLE������WindowsAPI���W�񂵂����N���X
	class DLL_DECLSPEC HandleLibrary : public WindowsLibrary
	{
	public:
		//! @brief	�R���X�g���N�^
		//! @note	������ _hObject �� INVALID_HANDLE_VALUE �ŏ��������܂��B
		//! 		�p����͂��̃R���X�g���N�^���w�肵�ď��������܂��B
		APIENTRY HandleLibrary ();

		//! @brief	�f�X�g���N�^
		//! @note	_hObject ���N���[�Y���Ă��Ȃ��ꍇ�A�����Ŋm���ɃN���[�Y���܂��B
		APIENTRY ~HandleLibrary ();

		//! @brief	�p����͂��̊֐��Ńn���h�����擾���܂��B
		//! @return	HANDLE�l
		//! @retval	���s�����ꍇ�� INVALID_HANDLE_VALUE ���Ԃ�܂��B
		HANDLE APIENTRY GetHandle () const;

		//! @brief		�g�p���Ȃ��Ȃ����n���h���͂���ŃN���[�Y���܂��B
		//! @return		�N���[�Y�ɐ����������ۂ��B
		//! @retval		����(TRUE)
		//! @retval		���s(FALSE)
		//! @details	�g�p���Ă��Ȃ��ꍇ�́A�������܂���B
		//! 			�g�p���Ă����ꍇ�́A�n���h�����N���[�Y���A���������܂��B
		BOOL APIENTRY Close ();

	protected:
		//! �n���h�����g�p����WindowsAPI�ŕێ�����HANDLE�l
		HANDLE _hObject;
	};

	//! @class	ReadWriteHandleLibrary
	//! @brief	HANDLE��Read��Write���s��WindowsAPI���W�񂵂����N���X
	class DLL_DECLSPEC ReadWriteHandleLibrary : public HandleLibrary
	{
	public:
		//! @brief		HANDLE���g���ăf�[�^��ǂݍ��݂܂��B
		//! @param[in]	lpvBuffer	�ǂݍ��񂾃f�[�^��ۊǂ���o�b�t�@
		//! @param[in]	dwSize		�ǂݍ��񂾃f�[�^��ۊǂ���o�b�t�@�̃T�C�Y
		//! @return		���ۂɓǂݍ��񂾃f�[�^�̃T�C�Y�B
		//! 			���s�����ꍇ��(DWORD)-1 ���Ԃ�܂��B
		//! @retval		�f�[�^�T�C�Y�̓o�C�g�T�C�Y�ł��B
		DWORD APIENTRY Read (LPVOID lpvBuffer, DWORD dwSize) const;

		//! @brief		HANDLE���g���ăf�[�^���������݂܂��B
		//! @param[in]	lpcvBuffer	�������ރf�[�^��ۊǂ���o�b�t�@
		//! @param[in]	dwSize		�������ރf�[�^��ۊǂ���o�b�t�@�̃T�C�Y
		//! @return		���ۂɏ������񂾃f�[�^�̃T�C�Y
		//! 			���s�����ꍇ��(DWORD)-1 ���Ԃ�܂��B
		//! @retval		�f�[�^�T�C�Y�̓o�C�g�T�C�Y�ł��B
		DWORD APIENTRY Write (LPCVOID lpcvBuffer, DWORD dwSize) const;

		//! @brief	�܂��f�o�C�X�ɏo�͂���Ă��Ȃ��f�[�^��f���o�����܂��B
		//! @return	�f�[�^�̃t���b�V���ɐ����������ۂ��B
		//! @retval	����(TRUE)
		//! @retval	���s(FALSE)
		BOOL APIENTRY Flush () const;
	};

	//! @class	WaitableHandleLibrary
	//! @brief	HANDLE���g���ăV�O�i��������s��WindowsAPI���W�񂵂����N���X
	class DLL_DECLSPEC WaitableHandleLibrary : public HandleLibrary
	{
	public:
		//! @brief	�V�O�i����ԂɂȂ�ƃu���b�N���������܂��B
		//! @return	�u���b�N�����̗��R
		//! @retval	�n���h�����V�O�i�����(WAIT_OBJECT_O)
		//! 		�n���h���I�u�W�F�N�g���I��(WAIT_ABANDONED)
		//! 		�^�C���A�E�g(WAIT_TIMEOUT)
		//! 		�G���[����(WAIT_FAILED)
		//! 		SetWaitableTimer()�̊֐��Ăяo������(WAIT_IO_COMPLETION)
		DWORD APIENTRY Wait (DWORD dwWaitTime = INFINITE) const;
	};

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