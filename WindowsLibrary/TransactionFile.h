// ----------------------------------------------------------------------------
//! @file	TransactionFile.h
//! @brief	�g�����U�N�V����NTFS�Ɋւ���API���W�񂵂��N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "File.h"

namespace alt
{
	//! @class	TransactionFileUtility
	//! @brief	�g�����U�N�V����NTFS�Ɋւ��郆�[�e�B���e�B�N���X
	//! @sa		http://s-kita.hatenablog.com/entry/20121217/1355764523
	class DLL_DECLSPEC TransactionFileUtility : public HandleLibrary
	{
	public:
		//! @brief ������
		//! @param lpwszDescription 
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Init (LPWSTR lpwszDescription);

		//! @brief �g�����U�N�V�����̃R�~�b�g
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Commit () const;

		//! @brief �g�����U�N�V�����̃��[���o�b�N
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Rollback () const;

		//! @brief �t�H���_�̍쐬
		//! @param lpctszFolderName �t�H���_��
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY CreateFolder (LPCTSTR lpctszFolderName) const;

		//! @brief �t�@�C���̍폜
		//! @param lpctszFileName �t�@�C���� 
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Delete (LPCTSTR lpctszFileName) const;

		//! @brief �t�@�C���̃R�s�[
		//! @param lpctszSrcFileName �R�s�[���t�@�C����
		//! @param lpctszDstFileName �R�s�[��t�@�C����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Copy (LPCTSTR lpctszSrcFileName, LPCTSTR lpctszDstFileName) const;

		//! @brief �t�@�C���̈ړ�
		//! @param lpctszSrcFileName �ړ����t�@�C����
		//! @param lpctszDstFileName �ړ���t�@�C����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY Move (LPCTSTR lpctszSrcFileName, LPCTSTR lpctszDstFileName) const;

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

		//! @brief �V���{���b�N�����N�̍쐬
		//! @param lpctszSrcFileName �����N�쐬���t�@�C����
		//! @param lpctszLinkFileName �����N�t�@�C����
		//! @param isFile �Ώۂ��t�@�C�����ۂ�
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY MakeSymbolicLink (
			LPCTSTR lpctszSrcFileName, LPCTSTR lpctszLinkFileName,
			BOOL isFile = TRUE) const;

#endif

		//! @brief �n�[�h�����N�̍쐬
		//! @param lpctszSrcFileName �����N�쐬���t�@�C����
		//! @param lpctszLinkFileName �����N�t�@�C����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		BOOL APIENTRY MakeHardLink (
			LPCTSTR lpctszSrcFileName, LPCTSTR lpctszLinkFileName) const;
	};

	//! @class	TransactionFile
	//! @brief	�g�����U�N�V����NTFS�Ɋւ���N���X
	class DLL_DECLSPEC TransactionFile : public File
	{
	public:
		//! @brief		�t�@�C�����쐬�A�I�[�v�����܂��B
		//! @param[in]	hTransaction �g�����U�N�V�����n���h��
		//! @param[in]	pctszFileName (�쐬����t�@�C����)
		//! @param[in]	dwDesiredAccess {GENERIC_READ, GENERIC_WRITE, GENERIC_ALL}
		//! @param[in]	dwShareMode {FILE_SHARE_READ, FILE_SHARE_WRITE}
		//! @param[in]	dwCreationDisposition {CREATE_ALWAYS, OPEN_EXISTING}
		//! @return		BOOL
		//! @retval		TRUE(����)
		//! @retval		FALSE(���s)
		BOOL APIENTRY Create (
			HANDLE hTransaction, LPCTSTR pctszFileName, DWORD dwDesiredAccess,
			DWORD dwShareMode, DWORD dwCreationDisposition);
	};
}