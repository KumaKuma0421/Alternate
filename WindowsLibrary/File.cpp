// ----------------------------------------------------------------------------
//! @file	File.cpp
//! @brief	ファイルIOに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "File.h"

using namespace alt;

BOOL File::Create (
	LPCTSTR lpctszFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
	DWORD dwCreationDisposition)
{
	BOOL ret = TRUE;
	PSECURITY_ATTRIBUTES pSecurityAttributes = NULL;
	DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	HANDLE hTemplateFile = NULL;

	_hObject = ::CreateFile (
		lpctszFileName,
		dwDesiredAccess,
		dwShareMode,
		pSecurityAttributes,
		dwCreationDisposition,
		dwFlagsAndAttributes,
		hTemplateFile);
	if (_hObject == INVALID_HANDLE_VALUE)
	{
		ret = FALSE;
	}

	return ret;
}

BOOL File::SetPointer (DWORD dwMoveMethod, LARGE_INTEGER liDistanceToMove) const
{
	return ::SetFilePointerEx (
		_hObject,
		liDistanceToMove,
		NULL,
		dwMoveMethod);
}

BOOL File::SetEOF () const
{
	return ::SetEndOfFile (_hObject);
}

LONGLONG File::GetSize () const
{
	LARGE_INTEGER liFileSize;
	LONGLONG response;

	BOOL ret = ::GetFileSizeEx (_hObject, &liFileSize);
	if (ret == TRUE)
	{
		response = liFileSize.QuadPart;
	}
	else
	{
		response = (LONGLONG)-1;
	}

	return response;
}

DWORD File::GetType () const
{
	return ::GetFileType (_hObject);
}

BOOL File::IsOpen () const
{
	return _hObject == INVALID_HANDLE_VALUE ? FALSE : TRUE;
}