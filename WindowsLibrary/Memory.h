// ----------------------------------------------------------------------------
//! @file	Memory.h
//! @brief	�������Ɋւ���C���^�[�t�F�[�X���`�����N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.05

#pragma once

#include "WindowsLibrary.h"

#define MEM_ZERO HEAP_ZERO_MEMORY

namespace alt
{
	namespace skeleton
	{
		//! @class IMemory
		//! @brief �������̊l���E����̃C���^�[�t�F�[�X�N���X
		class DLL_DECLSPEC IMemory
		{
		public:
			//! @brief �������̎擾
			//! @param bytes �擾�T�C�Y
			//! @param dwFlags �擾�������ւ̐ݒ�
			//! @return LPVOID
			//! @retval �擾�����������̃A�h���X
			virtual LPVOID APIENTRY Allocate (SIZE_T bytes, DWORD dwFlags = MEM_ZERO) = 0;

			//! @brief �擾�������̊g��
			//! @param lpvMem �g�傳����A�h���X
			//! @param bytes �g��T�C�Y
			//! @param dwFlags �Ď擾�������ւ̐ݒ�
			//! @return LPVOID
			//! @retval �Ď擾�����������̃A�h���X
			virtual LPVOID APIENTRY Reallocate (
				LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags = MEM_ZERO) = 0;

			//! @brief �������̕ԋp
			//! @param lpvMem �ԋp����A�h���X
			//! @return BOOL
			//! @retval TRUE(����)
			//! @retval FALSE(���s)
			virtual BOOL APIENTRY Free (LPVOID lpvMem) = 0;
		};
	}
}