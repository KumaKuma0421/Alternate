// ----------------------------------------------------------------------------
//! @file	Pipe.h
//! @brief	�p�C�v�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	Pipe
	//! @brief	�p�C�v�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC Pipe : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY Pipe ();

		//! @brief �f�X�g���N�^
		APIENTRY ~Pipe ();

		//! @brief �p�C�v�̍쐬
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Create ();

		//! @brief ���b�Z�[�W�̊m�F
		//! @param lpvBuf �ǂݍ��݃f�[�^
		//! @param dwSize �ǂݍ��݃T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		DWORD APIENTRY Peek (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief ���b�Z�[�W�̓ǂݍ���
		//! @param lpvBuf �ǂݍ��݃f�[�^
		//! @param dwSize �ǂݍ��݃T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		DWORD APIENTRY Read (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief ���b�Z�[�W�̏�������
		//! @param lpvBuf �������݃f�[�^
		//! @param dwSize �������݃T�C�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Write (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief �p�C�v�̃N���[�Y
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Close ();

		//! @brief �ǂݍ��ݗp�n���h���̎擾
		//! @return �n���h���l
		HANDLE APIENTRY GetReadHandle () const { return _hRead; };

		//! @brief �������ݗp�n���h���̎擾
		//! @return �n���h���l
		HANDLE APIENTRY GetWriteHandle () const { return _hWrite; };

	private:
		HANDLE _hRead;
		HANDLE _hWrite;
	};
}