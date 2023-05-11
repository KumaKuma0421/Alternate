// ----------------------------------------------------------------------------
//! @file	RingBuffer.h
//! @brief	�����O�o�b�t�@�Ɋւ���N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	RingBuffer
	//! @brief	�����O�o�b�t�@�Ɋւ���N���X
	class DLL_DECLSPEC RingBuffer : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY RingBuffer ();

		//! @brief �R���X�g���N�^
		//! @param size �o�b�t�@�T�C�Y
		APIENTRY RingBuffer (SIZE_T size);

		//! @brief �f�X�g���N�^
		APIENTRY ~RingBuffer ();

		//! @brief ������
		//! @param size �o�b�t�@�T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Init (SIZE_T size);

		//! @brief �o�b�t�@�̃N���A
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Clear ();

		//! @brief �o�b�t�@�̏�������
		//! @param lpbyBuffer �������݃o�b�t�@
		//! @param size �o�b�t�@�T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Write (LPBYTE lpbyBuffer, SIZE_T size);

		//! @brief �o�b�t�@�̓ǂݍ���
		//! @param lpbyBuffer �ǂݍ��݃o�b�t�@
		//! @param size �o�b�t�@�T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Read (LPBYTE lpbyBuffer, SIZE_T size);

		//! @brief �c��o�b�t�@�T�C�Y�̎擾
		//! @return SIZE_T �c��o�b�t�@��
		SIZE_T APIENTRY Remain () const;

	private:
		LPBYTE _lpbyStartPoint;
		LPBYTE _lpbyEndPoint;
		SIZE_T _size;
		SIZE_T _remainSize;

		LPBYTE _lpbyReadPoint;
		LPBYTE _lpbyWritePoint;
	};
}