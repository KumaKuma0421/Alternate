// ----------------------------------------------------------------------------
//! @file	File.h
//! @brief	ファイルIOに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	File
	//! @brief	ファイルIOに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC File : public ReadWriteHandleLibrary
	{
	public:
		//! @brief		ファイルを作成、オープンします。
		//! @param[in]	lpctszFileName 作成するファイル名
		//! @param[in]	dwDesiredAccess {GENERIC_READ, GENERIC_WRITE, GENERIC_ALL}
		//! @param[in]	dwShareMode {FILE_SHARE_READ, FILE_SHARE_WRITE}
		//! @param[in]	dwCreationDisposition {CREATE_ALWAYS, OPEN_EXISTING}
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL APIENTRY Create (
			LPCTSTR lpctszFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
			DWORD dwCreationDisposition);

		//! @brief		ファイルポインタを移動します。
		//! @param[in]	dwMoveMethod {FILE_BEGIN, FILE_CURRENT, FILE_END}
		//! @param[in]	liDistanceToMove ファイルポインタの移動距離
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL APIENTRY SetPointer (
			DWORD dwMoveMethod, LARGE_INTEGER liDistanceToMove = { 0 }) const;

		//! @brief	今のファイル読み込み位置でファイルを終端します。
		//! @return	BOOL
		//! @retval	TRUE(成功)
		//! @retval	FALSE(失敗)
		BOOL APIENTRY SetEOF () const;

		//! @brief	ファイルサイズを取得します。
		//! @return	ファイルサイズ(バイト)
		LONGLONG APIENTRY GetSize () const;

		//! @brief	ファイルの種類を取得します。
		//! @return	DWORD
		//! @retval	FILE_TYPE_CHAR(0x0002)
		//! @retval	FILE_TYPE_DISK(0x0001)
		//! @retval	FILE_TYPE_PIPE(0x0003)
		//! @retval	FILE_TYPE_REMOTE(0x8000) unused.
		//! @retval	FILE_TYPE_UNKNOWN(0x0000)
		//! @note	WindowsAPI GetFileType()を使用します。
		DWORD APIENTRY GetType () const;

		//! @brief	ファイルがすでに操作可能か否か。
		//! @return	BOOL
		//! @retval	FALSE(まだファイル操作はできない)
		//! @retval	TRUE(ファイル操作可能)
		BOOL APIENTRY IsOpen () const;
	};
}