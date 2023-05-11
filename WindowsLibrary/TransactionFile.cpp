// ----------------------------------------------------------------------------
//! @file	TransactionFile.cpp
//! @brief	トランザクションNTFSに関するAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.27

#include "pch.h"
#include "TransactionFile.h"

using namespace alt;

// -- TransactionFileUtility ------------------------------------------------

BOOL TransactionFileUtility::Init (LPWSTR lpwszDescription)
{
	DWORD dwOption = 0;

	_hObject = ::CreateTransaction (
		NULL,
		0,
		dwOption,
		0,
		0,
		INFINITE,
		lpwszDescription);

	return _hObject == INVALID_HANDLE_VALUE ? FALSE : TRUE;
}

BOOL TransactionFileUtility::Commit () const
{
	return ::CommitTransaction (_hObject);
}

BOOL TransactionFileUtility::Rollback () const
{
	return ::RollbackTransaction (_hObject);
}

BOOL TransactionFileUtility::CreateFolder (LPCTSTR lpctszFolderName) const
{
	return ::CreateDirectoryTransacted (
		NULL,
		lpctszFolderName,
		NULL,
		_hObject);
}

BOOL TransactionFileUtility::Delete (LPCTSTR lpctszFileName) const
{
	return ::DeleteFileTransacted (lpctszFileName, _hObject);
}

BOOL TransactionFileUtility::Copy (
	LPCTSTR lpctszSrcFileName, LPCTSTR lpctszDstFileName) const
{
	DWORD dwFlags = COPY_FILE_FAIL_IF_EXISTS;

	return ::CopyFileTransacted (
		lpctszSrcFileName,
		lpctszDstFileName,
		NULL,
		NULL,
		NULL,
		dwFlags,
		_hObject);
}

BOOL TransactionFileUtility::Move (
	LPCTSTR lpctszSrcFileName, LPCTSTR lpctszDstFileName) const
{
	DWORD dwFlags = MOVEFILE_REPLACE_EXISTING;

	return ::MoveFileTransacted (
		lpctszSrcFileName,
		lpctszDstFileName,
		NULL,
		NULL,
		dwFlags,
		_hObject);
}

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

BOOL TransactionFileUtility::MakeSymbolicLink (
	LPCTSTR lpctszSrcFileName, LPCTSTR lpctszLinkFileName, BOOL isFile) const
{
	DWORD dwFlags = isFile == TRUE ? 0 : SYMBOLIC_LINK_FLAG_DIRECTORY;
	dwFlags |= SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

	return ::CreateSymbolicLinkTransacted (
		lpctszLinkFileName,
		lpctszSrcFileName,
		dwFlags,
		_hObject);
}

#endif

BOOL TransactionFileUtility::MakeHardLink (
	LPCTSTR lpctszSrcFileName, LPCTSTR lpctszLinkFileName) const
{
	return ::CreateHardLinkTransacted (
		lpctszLinkFileName,
		lpctszSrcFileName,
		NULL,
		_hObject);
}

// -- TransactionFile ------------------------------------------------

BOOL TransactionFile::Create (
	HANDLE hTransaction, LPCTSTR pctszFileName, DWORD dwDesiredAccess,
	DWORD dwShareMode, DWORD dwCreationDisposition)
{
	PSECURITY_ATTRIBUTES pSecurityAttributes = NULL;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hTemplateFile = NULL;

	_hObject = ::CreateFileTransacted (
		pctszFileName,
		dwDesiredAccess,
		dwShareMode,
		pSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile,
		hTransaction,
		NULL,
		NULL);

	return _hObject == INVALID_HANDLE_VALUE ? FALSE : TRUE;
}