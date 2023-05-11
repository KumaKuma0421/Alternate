// ----------------------------------------------------------------------------
//! @file	VirtualMemory.h
//! @brief	���z�������Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "Memory.h"

namespace alt
{
	//! @class VirtualMemory1
	//! @brief ���z�������̎擾�E������ȈՓI�Ɏ��������N���X
	class DLL_DECLSPEC VirtualMemory1 : public WindowsLibrary, public skeleton::IMemory
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY VirtualMemory1 ();

		//! @brief �������̎擾
		//! @param bytes �擾�T�C�Y
		//! @param dwFlags �擾�������ւ̐ݒ�
		//! @return LPVOID
		//! @retval �擾�����������̃A�h���X
		virtual LPVOID APIENTRY Allocate (SIZE_T bytes, DWORD dwFlags = MEM_ZERO);

		//! @brief �擾�������̊g��
		//! @param lpvMem �g�傳����A�h���X
		//! @param bytes �g��T�C�Y
		//! @param dwFlags �Ď擾�������ւ̐ݒ�
		//! @return LPVOID
		//! @retval �Ď擾�����������̃A�h���X
		virtual LPVOID APIENTRY Reallocate (
			LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags = MEM_ZERO);

		//! @brief �������̕ԋp
		//! @param lpvMem �ԋp����A�h���X
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		virtual BOOL APIENTRY Free (LPVOID lpvMem);

	private:
		SIZE_T _currentSize;
	};

	//! @class	VirtualMemory
	//! @brief	���z�������Ɋւ���N���X
	class DLL_DECLSPEC VirtualMemory : public WindowsLibrary
	{
	public:
		//! @brief ���z�������̗\��
		//! @param size �\��T�C�Y
		//! @return LPVOID �\�񃁃����A�h���X
		LPVOID APIENTRY Reserve (SIZE_T size) const;

		//! @brief ���z�������̗\��J��
		//! @param lpvMem �\�񃁃����A�h���X
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Release (LPVOID lpvMem) const;

		//! @brief �\�񃁃����̊m��
		//! @param lpvMem �\�񂵂��������̐擪�A�h���X
		//! @param size �\��T�C�Y
		//! @return LPVOID �m�肵���������̐擪�A�h���X
		LPVOID APIENTRY Commit (LPVOID lpvMem, SIZE_T size) const;

		//! @brief �m�胁�����̉���
		//! @param lpvMem �m�肵���������̐擪�A�h���X
		//! @param size �m��T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Decommit (LPVOID lpvMem, SIZE_T size) const;

		//! @brief �������̃��b�N
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Lock (LPVOID lpvMem, SIZE_T size) const;

		//! @brief �������̃A�����b�N
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Unlock (LPVOID lpvMem, SIZE_T size) const;
	};
}