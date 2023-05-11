// ----------------------------------------------------------------------------
//! @file	WatchDogTimer.h
//! @brief	�^�C�}�[�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "FunctionPointers.hpp"
#include "QueryPerformance.h"

namespace alt
{
	//! @class	WatchDogTimer
	//! @brief	�^�C�}�[�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC WatchDogTimer : public WaitableHandleLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		//! @note  ���ԂɂȂ�ƁA�C�x���g�Œʒm���܂��B
		APIENTRY WatchDogTimer ();

		//! @brief �R���X�g���N�^
		//! @param[in] timeup ���ԂɂȂ�ƁAtimeup�R�[���o�b�N�Œʒm���܂��B
		APIENTRY WatchDogTimer (skeleton::ITimeup* timeup);

		//! @brief ������
		//! @param lpctszName �^�C�}�[�̖���
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY InitTimer (LPCTSTR lpctszName = NULL);

		//! @brief �^�C�}�[�J�n
		//! @param dwTime �^�C�}�[�C���^�[�o��
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY StartTimer (DWORD dwTime) const;

		//! @brief �^�C���A�b�v�ҋ@
		//! @return DWORD
		//! @retval WAIT_OBJECT_O �^�C���A�b�v�ɂ��u���C�N
		DWORD APIENTRY WaitTimer () const;

		//! @brief �^�C�}�[��~
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY StopTimer () const;

	private:
		//! @brief �R�[���o�b�N�֐����`�����N���X
		skeleton::ITimeup* _timeup;
	};
}