// ----------------------------------------------------------------------------
//! @file	Mutex.h
//! @brief	�~���[�e�b�N�X�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	Mutex
	//! @brief	�~���[�e�b�N�X�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC Mutex : public WaitableHandleLibrary
	{
	public:
		//! @brief		�~���[�e�b�N�X���쐬���܂��B
		//! @param[in]	lpctszName �~���[�e�b�N�X��
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		BOOL APIENTRY Create (LPCTSTR lpctszName);

		//! @brief		�~���[�e�b�N�X���I�[�v�����܂��B
		//! @param[in]	lpctszName �~���[�e�b�N�X��
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		BOOL APIENTRY Open (LPCTSTR lpctszName);

		//! @brief	�~���[�e�b�N�X��������܂��B
		//! @return	BOOL
		//! @retval	TRUE(����)
		//! @retval	FALSE(���s)
		BOOL APIENTRY Release () const;
	};
}