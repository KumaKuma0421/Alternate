// ----------------------------------------------------------------------------
//! @file	HeapMemory.h
//! @brief	�q�[�v�������Ɋւ���API���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.21

#pragma once

#include "WindowsLibrary.h"
#include "Memory.h"

namespace alt
{
	//! @class BasicMemory
	//! @brief malloc/realloc/free���g�����W���I�ȃ������N���X
	class DLL_DECLSPEC BasicMemory : public WindowsLibrary, public skeleton::IMemory
	{
	public:
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
	};

	//! @class	HeapMemory
	//! @brief	�q�[�v�������Ɋւ���API���W�񂵂��N���X
	class DLL_DECLSPEC HeapMemory : public HandleLibrary, public skeleton::IMemory
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY HeapMemory ();

		//! @brief �R�s�[�R���X�g���N�^
		APIENTRY HeapMemory (const HeapMemory& base);

		//! @brief �f�X�g���N�^
		APIENTRY ~HeapMemory ();

		//! @brief �q�[�v�̈�̎擾
		//! @param initialSize �����T�C�Y
		//! @param maximumSize �ő�T�C�Y
		//! @param dwOptions �I�v�V����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		//! @note �����T�C�Y���O�̏ꍇ�A�P�y�[�W���R�~�b�g���܂��B
		//! @note �ő�T�C�Y���O�łȂ��ꍇ�A�s�[�v�T�C�Y�͌Œ肳��܂��B
		BOOL APIENTRY Create (
			SIZE_T initialSize = 0, SIZE_T maximumSize = 0,
			DWORD dwOptions = 0);

		//! @brief �V�X�e���q�[�v�̈�̎擾
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Open ();

		//! @brief �擾�����q�[�v�̈�̉��
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Close ();

		//! @brief �擾�����q�[�v�̈�̐���
		//! @return SIZE_T
		//! @retval ����������̃������T�C�Y
		SIZE_T APIENTRY Compact () const;

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
		SIZE_T APIENTRY BoundarySize (SIZE_T bytes) const;

		INT _iBoundary;
		BOOL _bCreateHeap;
	};
}