// ----------------------------------------------------------------------------
//! @file	FileUtility.h
//! @brief	�t�@�C���n���h���𔺂�Ȃ��t�@�C��IO�Ɋւ���WindowsAPI���W�񂵂�
//!         �N���X
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "File.h"
#include "TString.h"
#include "Array.hpp"

namespace alt
{
	class FileUtility;

	//! @enum ShellOperation
	//! @brief SHFILEOPSTRUCT.wFunc�ɓn���A�l�̗�
	enum class ShellOperation : WORD
	{
		MOVE = FO_MOVE,
		COPY = FO_COPY,
		REMOVE = FO_DELETE,
		RENAME = FO_RENAME,
	};

	//! @brief �t�@�C����񋤒ʃN���X
	class DLL_DECLSPEC FileInfo
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY FileInfo ();

		//! @brief �f�X�g���N�^
		virtual APIENTRY ~FileInfo ();

		//! @brief �t�H���_���̎擾
		//! @return TString&
		const TString& APIENTRY GetFolderName () const { return _FolderName; };

		//! @brief �t�@�C�����̎擾
		//! @return TString&
		const TString& APIENTRY GetFileName () const { return _FileName; };

		//! @brief �t�@�C���T�C�Y�̎擾
		//! @return ULONGLONG&
		const ULONGLONG& APIENTRY GetFileSize () const { return _FileSize.QuadPart; };

		friend FileUtility;

	private:
		TString _FolderName;
		TString _FileName;
		ULARGE_INTEGER _FileSize;
	};

	//! @brief FileUtility::Find()���Ԃ��t�@�C�����
	class DLL_DECLSPEC FileInfo1 : public FileInfo
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY FileInfo1 ();

		//! @brief �f�X�g���N�^
		APIENTRY ~FileInfo1 ();

		//! @brief �t�@�C���쐬�����̎擾
		//! @return FILETIME&
		const FILETIME& APIENTRY GetCreateTime () const { return _CreateTime; };

		//! @brief �t�@�C���X�V�����̎擾
		//! @return FILETIME&
		const FILETIME& APIENTRY GetUpdateTime () const { return _UpdateTime; };

		//! @brief �t�@�C���A�N�Z�X�����̎擾
		//! @return FILETIME&
		const FILETIME& APIENTRY GetAccessTime () const { return _AccessTime; };

		//! @brief �t�@�C�������̎擾
		//! @return INT&
		const INT& APIENTRY GetAttribute () const { return _Attribute; };

		//! @brief �ǂݎ���p���ۂ�
		//! @return BOOL
		//! @retval TRUE(�ǂݎ�葮��ON)
		const BOOL APIENTRY IsReadOnly () const
		{
			return _Attribute & FILE_ATTRIBUTE_READONLY ? TRUE : FALSE;
		};

		//! @brief �B���t�@�C�����ۂ�
		//! @return BOOL
		//! @retval TRUE(�B������ON)
		const BOOL APIENTRY IsHidden () const
		{
			return _Attribute & FILE_ATTRIBUTE_HIDDEN ? TRUE : FALSE;
		};

		//! @brief �V�X�e���t�@�C�����ۂ�
		//! @return BOOL
		//! @retval TRUE(�V�X�e������ON)
		const BOOL APIENTRY IsSystem () const
		{
			return _Attribute & FILE_ATTRIBUTE_SYSTEM ? TRUE : FALSE;
		};

		//! @brief �f�B���N�g�����ۂ�
		//! @return BOOL
		//! @retval TRUE(����̓f�B���N�g��)
		const BOOL APIENTRY IsDirectory () const
		{
			return _Attribute & FILE_ATTRIBUTE_DIRECTORY ? TRUE : FALSE;
		};

		//! @brief �A�[�J�C�u���ۂ�
		//! @return BOOL
		//! @retval TRUE(�A�[�J�C�u����ON)
		const BOOL APIENTRY IsArchive () const
		{
			return _Attribute & FILE_ATTRIBUTE_ARCHIVE ? TRUE : FALSE;
		};

		//! @brief ���ԃt�@�C�����ۂ�
		//! @return BOOL
		//! @retval TRUE(���ԃt�@�C��)
		const BOOL APIENTRY IsTemporary () const
		{
			return _Attribute & FILE_ATTRIBUTE_TEMPORARY ? TRUE : FALSE;
		};

		//! @brief ���k�t�@�C�����ۂ�
		//! @return BOOL
		//! @retval TRUE(���k�t�@�C��)
		const BOOL APIENTRY IsCompressed () const
		{
			return _Attribute & FILE_ATTRIBUTE_COMPRESSED ? TRUE : FALSE;
		};

		friend FileUtility;

	private:
		INT      _Attribute;
		FILETIME _CreateTime;
		FILETIME _UpdateTime;
		FILETIME _AccessTime;
	};

	class DLL_DECLSPEC FileInfo2 : public FileInfo1
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY FileInfo2 ();

		//! @brief �f�X�g���N�^
		APIENTRY ~FileInfo2 ();

		//! @brief �t�@�C���Ď����ʂ��擾
		//! @return DWORD
		//! @retval �t�@�C���̏�ԕω��t���O
		//!   FILE_ACTION_ADDED,
		//!   FILE_ACTION_REMOVED,
		//!   FILE_ACTION_MODIFIED,
		//!   FILE_ACTION_RENAMED_OLD_NAME,
		//!   FILE_ACTION_RENAMED NEW_NAME
		const DWORD APIENTRY GetAction () const { return _dwAction; };

		//! @brief �t�@�C��ID�̎擾
		//! @return LONGLONG
		const LONGLONG APIENTRY GetFileID () const { return _FileID; };

		//! @brief �t�@�C���ύX���m�����̎擾
		//! @return FILETIME&
		const FILETIME& APIENTRY GetChangeTime () const { return _ChangeTime; };

		//! @brief �ǉ����ꂽ��
		//! @return BOOL
		//! @retval TRUE(�ǉ����m�F)
		const BOOL APIENTRY IsAdded () const
		{
			return _dwAction == FILE_ACTION_ADDED ? TRUE : FALSE;
		};

		//! @brief �폜���ꂽ��
		//! @return BOOL
		//! @retval TRUE(�폜���m�F)
		const BOOL APIENTRY IsRemoved () const
		{
			return _dwAction == FILE_ACTION_REMOVED ? TRUE : FALSE;
		};

		//! @brief �ύX���ꂽ��
		//! @return BOOL
		//! @retval TRUE(�ύX���m�F)
		const BOOL APIENTRY IsModified () const
		{
			return _dwAction == FILE_ACTION_MODIFIED ? TRUE : FALSE;
		};

		//! @brief ���O��ύX���ꂽ���i���t�@�C����)
		//! @return BOOL
		//! @retval TRUE(���O�ύX���m�F�F���t�@�C����)
		const BOOL APIENTRY IsOldName () const
		{
			return _dwAction == FILE_ACTION_RENAMED_OLD_NAME ? TRUE : FALSE;
		};

		//! @brief ���O��ύX���ꂽ���i�V�t�@�C����)
		//! @return BOOL
		//! @retval TRUE(���O�ύX���m�F�F�V�t�@�C����)
		const BOOL APIENTRY IsNewName () const
		{
			return _dwAction == FILE_ACTION_RENAMED_NEW_NAME ? TRUE : FALSE;
		};

		friend FileUtility;

	private:
		DWORD    _dwAction;
		FILETIME _ChangeTime;
		LONGLONG _FileID;
	};

	//! @class	FileUtility
	//! @brief	�t�@�C���n���h���𔺂�Ȃ��t�@�C��IO�Ɋւ���WindowsAPI��
	//!         �W�񂵂��N���X
	class DLL_DECLSPEC FileUtility : public WindowsLibrary
	{
	public:
		//! @brief �R���X�g���N�^
		APIENTRY FileUtility () {};

		//! @brief �f�X�g���N�^
		APIENTRY ~FileUtility () {};

		// ----- �t�@�C������

		//! @brief �t�@�C���̍폜
		//! @param name �폜����t�@�C����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY Delete (LPCTSTR name);

		//! @brief �t�@�C���̃R�s�[
		//! @param source �R�s�[���t�@�C����
		//! @param dest �R�s�[��t�@�C����
		//! @param bFailIfExists �t�@�C�������݂���ꍇ�Ɏ��s���邩�ۂ�
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY Copy (
			LPCTSTR source, LPCTSTR dest, BOOL bFailIfExists = FALSE);

		//! @brief �t�@�C���̈ړ�
		//! @param source �ړ����t�@�C����
		//! @param dest �ړ���t�@�C����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY Move (LPCTSTR source, LPCTSTR dest);

		//! @brief �e���|�����t�@�C�������擾���܂��B
		//! @param[in] lpctszPath �t�@�C���p�X
		//! @param[in] lpctszPrefix �t�@�C�����v���t�B�b�N�X(�R�����܂�)
		//! @param[in] nNumber �t�@�C�����Ɏg�p�����ԍ�(���ʂP�U�r�b�g�܂�)
		//! @note nNumber�ɂO���w�肷��ƃV�X�e�����������܂��B
		static TString APIENTRY GetTemporaryFileName (
			LPCTSTR lpctszPath, LPCTSTR lpctszPrefix, UINT nNumber);

		// ----- �f�B���N�g������

		//! @brief �f�B���N�g���̍쐬
		//! @param name �f�B���N�g����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY MakeDir (LPCTSTR name);

		//! @brief �f�B���N�g���̍폜
		//! @param name �f�B���N�g����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		//! @note �f�B���N�g�����̃t�@�C�����ċA�I�ɍ폜����ɂ́A
		//!       SHFileOperation()���g�p���܂��B
		static BOOL APIENTRY RemoveDir (LPCTSTR name);

		//! @brief �J�����g�f�B���N�g���̈ړ�
		//! @param name �f�B���N�g����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY SetCurrentDir (LPCTSTR name);

		//! @brief �J�����g�f�B���N�g�����̎擾
		//! @return �J�����g�f�B���N�g����
		static TString APIENTRY GetCurrentDir ();

		//! @brief �e���|�����t�H���_���擾���܂��B
		//! @return TString �t�H���_��
		static TString APIENTRY GetTemporaryPath ();

		//! @brief �J�����g�f�B���N�g���̃p�X��A�����Đ�΃p�X�̍쐬
		//! @param lpctszRelativePath �J�����g�f�B���N�g������̑��΃p�X
		//! @return TString �J�����g�f�B���N�g���{�w��p�X
		static TString APIENTRY GetAbsolutePath (LPCTSTR lpctszRelativePath);

		//! @brief �f�B���N�g���̑��݊m�F
		//! @param name �m�F����f�B���N�g����
		//! @return BOOL
		//! @retval TRUE(����)
		//! @retval FALSE(���s)
		static BOOL APIENTRY IsDirectory (LPCTSTR name);

		// ----- �t�@�C���E�f�B���N�g�����p����

		//! @brief �f�B���N�g���E�t�@�C���̑��݊m�F
		//! @param name �f�B���N�g���E�t�@�C����
		//! @return BOOL
		//! @retval TRUE(���݂���)
		//! @retval FALSE(���݂��Ȃ�)
		static BOOL APIENTRY IsExist (LPCTSTR name);

		//! @brief �t�@�C���̌���
		//! @param target �����Ώۃf�B���N�g����
		//! @param name �����Ώۃt�@�C����
		//! @param bRecurse �ċA���邩�ۂ�
		//! @return ��������
		static skeleton::Array<FileInfo1> APIENTRY Find (
			LPCTSTR target, LPCTSTR name, BOOL bRecurse = FALSE);

		static VOID APIENTRY Find2 (
			LPCTSTR target, LPCTSTR name, BOOL bRecurse,
			skeleton::Array<FileInfo1>& response);

		//! @brief �f�B���N�g���̊Ď�
		//! @param lpctszWatchDirectory �Ď��Ώۃf�B���N�g����
		//! @param dwTimeout �Ď�����
		//! @return alt::skeleton::Array<alt::TString>
		//! @retval �Ď����ɕω����������t�@�C�����
		static skeleton::Array<FileInfo2> APIENTRY DirectoryWatch (
			LPCTSTR lpctszWatchDirectory, DWORD dwTimeout);

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

		//! @brief �V���{���b�N�����N�̍쐬
		//! @param source �쐬���t�@�C����
		//! @param link �쐬�����N��
		//! @param isFile �쐬�����t�@�C�����ۂ�
		//! @note  SymbolicLink��dir�R�}���h��'SYMLINK'�A'SYMLINKD'��
		//!        �\�������B�܂��G�N�X�v���[���ł̓V���[�g�J�b�g��
		//!        ��󂪃A�C�R���ɕ\�������B�������A�Ǘ��Ҍ������쐬
		//!        �ɕK�v�ƂȂ�B
		//!        �R�}���h�́Amklink �`�܂���mklink /d �`�ō쐬����
		static BOOL APIENTRY MakeSymbolicLink (
			LPCTSTR source, LPCTSTR link, BOOL isFile = TRUE);

#endif
		//! @brief �n�[�h�����N�̍쐬
		//! @param source �쐬���t�@�C����
		//! @param link �쐬�����N��
		//! @note  HardLink��dir�R�}���h�ŋ�ʂ����Ȃ��B�A�C�R���ɂ�
		//!        �ω�������Ȃ��B������͊Ǘ��Ҍ������s�v�ł���B
		//!        �R�}���h�́Amklink /h �`�ō쐬����B�폜��del�B
		//!        �f�B���N�g���ɑ΂��Ă͍쐬�s�B����{�����[���̂݁B
		static BOOL APIENTRY MakeHardLink (LPCTSTR source, LPCTSTR link);

		static INT APIENTRY SHFileOperations (
			ShellOperation operation, LPCTSTR lpctszFrom, LPCTSTR lpctszTo);
	};
}