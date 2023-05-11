// ----------------------------------------------------------------------------
//! @file	NamedPipe.h
//! @brief	���O�t���p�C�v�Ɋւ���WindowsAPI���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

#define PIPE_NAME_FORMAT _T("\\\\.\\pipe\\%s")

namespace alt
{
	//! @class	NamedPipe
	//! @brief	���O�t���p�C�v�Ɋւ���WindowsAPI���W�񂵂��N���X
	class DLL_DECLSPEC NamedPipe : public ReadWriteHandleLibrary
	{
	public:
		//! @brief		�p�C�v���쐬���܂��B
		//! @param[in]	lpctszName �p�C�v��
		//! @param[in]	bStreamMode �X�g���[�����[�h���o�C�g���[�h���B
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		//! @details	�p�C�v���͓����Ő������ɕ⊮���܂��B
		BOOL APIENTRY Create (LPCTSTR lpctszName, BOOL bStreamMode);

		//! @brief	�T�[�o�[���ŃN���C�A���g�ڑ���ҋ@���܂��B
		//! @return	BOOL
		//! @retval	TRUE(����)
		//! @retval	FALSE(���s)
		BOOL APIENTRY Connect () const;

		//! @brief	�T�[�o�[���ŃN���C�A���g�ڑ���ؒf���܂��B
		//! @return	BOOL
		//! @retval	TRUE(����)
		//! @retval	FALSE(���s)
		BOOL APIENTRY Disconnect () const;

		//! @brief		�p�C�v���I�[�v�����܂��B
		//! @param[in]	lpctszName �p�C�v��
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		//! @details	�p�C�v���͓����Ő������ɕ⊮���܂��B
		BOOL APIENTRY Open (LPCTSTR lpctszName);

		//! @brief		�N���C�A���g���ŃT�[�o�[�ڑ���ҋ@���܂��B
		//! @param[in]	lpctszName	�p�C�v��
		//! @param[in]	dwTimeout	�^�C���A�E�g����(�~���b)
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		BOOL APIENTRY Wait (
			LPCTSTR lpctszName,
			DWORD dwTimeout = NMPWAIT_USE_DEFAULT_WAIT) const;

		//! @brief		���b�Z�[�W���e�����o�����ɓǂݎ��܂��B
		//! @param[in]	lpvData	�ǂݎ��o�b�t�@
		//! @param[in]	dwSize	�ǂݎ��o�b�t�@�T�C�Y
		//! @return		DWORD
		//! @retval		�ǂݎ��o�C�g�T�C�Y(����)
		//! @retval		-1(���s)
		DWORD APIENTRY PeekData (LPVOID lpvData, DWORD dwSize) const;

		//! @brief	�o�b�t�@���Ɏc���Ă��郁�b�Z�[�W�T�C�Y���擾���܂��B
		//! @return	DWORD
		//! @retval	�ǂݎ��o�C�g�T�C�Y(����)
		//! @retval	-1(���s)
		DWORD APIENTRY PeekRemain () const;
	};
}