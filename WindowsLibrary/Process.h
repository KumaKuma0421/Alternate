// ----------------------------------------------------------------------------
//! @file	Process.h
//! @brief	�v���Z�X�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "Pipe.h"

namespace alt
{
	//! @enum	Priority
	//! @brief	�v���Z�X�̗D��x�ݒ�l
	enum class Priority
	{
		//! ���A���^�C��
		Realtime = REALTIME_PRIORITY_CLASS,

		//! �D��F�^�C���N���e�B�J��
		High = HIGH_PRIORITY_CLASS,

		//! �ʏ��菭����
		AboveNormal = ABOVE_NORMAL_PRIORITY_CLASS,

		//! �ʏ�F��ʓI�ȃv���Z�X
		Normal = NORMAL_PRIORITY_CLASS,

		//! �ʏ��菭����
		BelowNormal = BELOW_NORMAL_PRIORITY_CLASS,

		//! �A�C�h���F�V�X�e�����A�C�h����Ԃ̎��Ɏ��s�����B
		Idle = IDLE_PRIORITY_CLASS
	};

	//! @class	Process
	//! @brief	�v���Z�X�Ɋւ���WindowsAPI���W�񂵂��N���X
	//! @sa		https://docs.microsoft.com/en-us/windows/win32/ProcThread/creating-a-child-process-with-redirected-input-and-output
	class DLL_DECLSPEC Process : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY Process ();

		//! @brief �f�X�g���N�^
		APIENTRY ~Process ();

		//! @brief �v���Z�X�̍쐬
		//! @param lpctszApplicationName ���W���[����
		//! @param lptszCommandLine �R�}���h���C��
		//! @param lpctszCurrentDirectory ���s���J�����g�f�B���N�g��
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Create (
			LPCTSTR lpctszApplicationName,
			LPTSTR lptszCommandLine,
			LPCTSTR lpctszCurrentDirectory = NULL);

		//! @brief �v���Z�X�I���̑ҋ@
		//! @param dwTime �I���^�C���A�E�g����(ms)
		//! @return DWORD
		//! @retval WAIT_OBJECT_0 ����I��(�v���Z�X�I��)
		//! @retval WAIT_
		DWORD APIENTRY Wait (DWORD dwTime = INFINITE) const;

		BOOL APIENTRY CreateAsUser ();
		BOOL APIENTRY CreateWithLogon ();

		//! @brief �v���Z�X�I�����̏I���R�[�h�擾
		//! @return DWORD �I���R�[�h
		DWORD APIENTRY GetExitCode () const;

		//! @brief �v���Z�X�̋����I��
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Terminate () const;

		//! @brief �N������v���Z�X�̃E�B���h�E�ʒu��ݒ�
		//! @param dwX X���W
		//! @param dwY Y���W
		//! @param dwWidth �E�B���h�E�̕�
		//! @param dwHeight �E�B���h�E�̍���
		//! @param wShowWindow �E�B���h�E�\���X�^�C�� { SW_MAXIMIZE, SW_MINIMIZE, etc }
		VOID APIENTRY SetWindowInfo (
			DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight,
			WORD wShowWindow = SW_NORMAL);

		//! @brief �N������v���Z�X�̃R���\�[���ʒu��ݒ�
		//! @param dwX X���W
		//! @param dwY Y���W
		//! @param dwWidth �E�B���h�E�̕�
		//! @param dwHeight �E�B���h�E�̍���
		//! @param dwXChars �������L�����N�^��
		//! @param dwYChars �c�����L�����N�^��
		VOID APIENTRY SetConsoleInfo (
			DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, DWORD dwXChars,
			DWORD dwYChars);

		//! @brief �R���\�[���^�C�g���̐ݒ�
		//! @param lptszConsoleTitle �R���\�[���^�C�g��
		VOID APIENTRY SetTitle (LPTSTR lptszConsoleTitle);

		//! @brief �v���Z�X�n���h���̎擾
		//! @return HANDLE
		HANDLE APIENTRY GetProcessHandle () const { return _pi.hProcess; };

		//! @brief �W�����̓n���h���̎擾
		//! @return HANDLE
		HANDLE APIENTRY GetStdIn () const { return _si.hStdInput; };

		//! @brief �W���o�̓n���h���̎擾
		//! @return HANDLE
		HANDLE APIENTRY GetStdOut () const { return _si.hStdOutput; };

		//! @brief �W���G���[�n���h���̎擾
		//! @return HANDLE
		HANDLE APIENTRY GetStdErr () const { return _si.hStdError; };

		//! @brief �D��x�̐ݒ�
		//! @param priority �ݒ肷��D��x
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY SetPriority (Priority priority) const;

		//! @brief �v���Z�X�o�͓��e�̓ǂݍ���
		//! @param lpvBuf �ǂݍ��݃o�b�t�@
		//! @param dwSize �o�b�t�@�T�C�Y
		//! @return DWORD �ǂݍ��݃T�C�Y
		DWORD APIENTRY Peek (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief �v���Z�X�o�͓��e�̓ǂݍ���
		//! @param lpvBuf �ǂݍ��݃o�b�t�@
		//! @param dwSize �o�b�t�@�T�C�Y
		//! @return DWORD �ǂݍ��݃T�C�Y
		DWORD APIENTRY Read (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief �v���Z�X���͂ւ̏�������
		//! @param lpvBuf �������݃o�b�t�@
		//! @param dwSize �o�b�t�@�T�C�Y
		//! @return DWORD �������݃T�C�Y
		DWORD APIENTRY Write (LPVOID lpvBuf, DWORD dwSize) const;

	private:
		BOOL ConnectHandles ();

		PROCESS_INFORMATION _pi;
		STARTUPINFO         _si;
		Pipe                _pipeRead;
		Pipe                _pipeWrite;
		Pipe                _pipeError;
		DWORD               _dwCreationFlags;
	};
}