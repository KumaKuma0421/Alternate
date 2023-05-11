// ----------------------------------------------------------------------------
//! @file	SharedMemory.h
//! @brief	���L�������Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	SharedMemory
	//! @brief	���L�������Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC SharedMemory : public HandleLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY SharedMemory ();

		//! @brief �f�X�g���N�^
		APIENTRY ~SharedMemory ();

		//! @brief ���L�������̍쐬
		//! @param lpctszName ���L�������̖���
		//! @param dwSize ���L�������̃T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Create (LPCTSTR lpctszName, DWORD dwSize);

		//! @brief ���L�������̃I�[�v��
		//! @param lpctszName ���L�������̖���
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Open (LPCTSTR lpctszName);

		//! @brief ���L�������̃}�b�s���O�J�n
		//! @return LPVOID
		//! @retval ���L�������̐擪�A�h���X
		LPVOID APIENTRY Mapped ();

		//! @brief ���L�������̃}�b�s���O�I��
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Unmapped ();

	private:
		LPVOID _lpvMapped;
	};
}