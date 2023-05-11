// ----------------------------------------------------------------------------
//! @file	Thread.h
//! @brief	�X���b�h�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "FunctionPointers.hpp"

namespace alt
{
	//! @enum	ThreadPriority
	//! @brief	�X���b�h�̗D�揇��
	enum class ThreadPriority
	{
		//! �N���e�B�J��
		Critical = THREAD_PRIORITY_TIME_CRITICAL,

		//! �D��x��
		Highest = THREAD_PRIORITY_HIGHEST,

		//! �ʏ��菭����
		AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,

		//! �ʏ�
		Normal = THREAD_PRIORITY_NORMAL,

		//! �ʏ��菭����
		BelowNormal = THREAD_PRIORITY_BELOW_NORMAL,

		//! �D��x��
		Lowest = THREAD_PRIORITY_LOWEST,

		//! �A�C�h��
		Idle = THREAD_PRIORITY_IDLE
	};

	//! @class	Thread
	//! @brief	�X���b�h�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC Thread : public WaitableHandleLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY Thread ();

		//! @brief �R���X�g���N�^
		//! @param callback �X���b�h���s�֐�
		APIENTRY Thread (skeleton::ICallback* callback);

		//! @brief		�X���b�h���쐬���܂��B
		//! @param[in]	function �X���b�h�N������֐�
		//! @param[in]	lpvParam �X���b�h�@�\����֐��ւ̃p�����[�^
		//! @param[in]	dwCreationFlag CREATE_SUSPENDED���w�肷���
		//!             �N���Ɠ����ɒ�~���܂��B
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		BOOL APIENTRY Create (
			PTHREAD_START_ROUTINE function = NULL, LPVOID lpvParam = NULL,
			DWORD dwCreationFlag = 0);

		//! @brief	�X���b�h���ꎞ��~���܂��B
		//! @return	BOOL
		//! @retval	TRUE(����)
		//! @retval	FALSE(���s)
		BOOL APIENTRY Suspend () const;

		//! @brief	�X���b�h���ĊJ���܂��B
		//! @return	BOOL
		//! @retval	TRUE(����)
		//! @retval	FALSE(���s)
		BOOL APIENTRY Resume () const;

		//! @brief		�X���b�h���s�D��x��ύX���܂��B
		//! @param[in]	priority �X���b�h���s�D��x
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		BOOL APIENTRY SetPriority (ThreadPriority priority) const;

	protected:
		skeleton::ICallback* _callback;
		DWORD				 _dwThreadID;
	};
}