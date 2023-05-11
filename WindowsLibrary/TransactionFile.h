// ----------------------------------------------------------------------------
//! @file	TransactionFile.h
//! @brief	トランザクションNTFSに関するAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "File.h"

namespace alt
{
	//! @class	TransactionFileUtility
	//! @brief	トランザクションNTFSに関するユーティリティクラス
	//! @sa		http://s-kita.hatenablog.com/entry/20121217/1355764523
	class DLL_DECLSPEC TransactionFileUtility : public HandleLibrary
	{
	public:
		//! @brief 初期化
		//! @param lpwszDescription 
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Init (LPWSTR lpwszDescription);

		//! @brief トランザクションのコミット
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Commit () const;

		//! @brief トランザクションのロールバック
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Rollback () const;

		//! @brief フォルダの作成
		//! @param lpctszFolderName フォルダ名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY CreateFolder (LPCTSTR lpctszFolderName) const;

		//! @brief ファイルの削除
		//! @param lpctszFileName ファイル名 
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Delete (LPCTSTR lpctszFileName) const;

		//! @brief ファイルのコピー
		//! @param lpctszSrcFileName コピー元ファイル名
		//! @param lpctszDstFileName コピー先ファイル名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Copy (LPCTSTR lpctszSrcFileName, LPCTSTR lpctszDstFileName) const;

		//! @brief ファイルの移動
		//! @param lpctszSrcFileName 移動元ファイル名
		//! @param lpctszDstFileName 移動先ファイル名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Move (LPCTSTR lpctszSrcFileName, LPCTSTR lpctszDstFileName) const;

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

		//! @brief シンボリックリンクの作成
		//! @param lpctszSrcFileName リンク作成元ファイル名
		//! @param lpctszLinkFileName リンクファイル名
		//! @param isFile 対象がファイルか否か
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY MakeSymbolicLink (
			LPCTSTR lpctszSrcFileName, LPCTSTR lpctszLinkFileName,
			BOOL isFile = TRUE) const;

#endif

		//! @brief ハードリンクの作成
		//! @param lpctszSrcFileName リンク作成元ファイル名
		//! @param lpctszLinkFileName リンクファイル名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY MakeHardLink (
			LPCTSTR lpctszSrcFileName, LPCTSTR lpctszLinkFileName) const;
	};

	//! @class	TransactionFile
	//! @brief	トランザクションNTFSに関するクラス
	class DLL_DECLSPEC TransactionFile : public File
	{
	public:
		//! @brief		ファイルを作成、オープンします。
		//! @param[in]	hTransaction トランザクションハンドル
		//! @param[in]	pctszFileName (作成するファイル名)
		//! @param[in]	dwDesiredAccess {GENERIC_READ, GENERIC_WRITE, GENERIC_ALL}
		//! @param[in]	dwShareMode {FILE_SHARE_READ, FILE_SHARE_WRITE}
		//! @param[in]	dwCreationDisposition {CREATE_ALWAYS, OPEN_EXISTING}
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL APIENTRY Create (
			HANDLE hTransaction, LPCTSTR pctszFileName, DWORD dwDesiredAccess,
			DWORD dwShareMode, DWORD dwCreationDisposition);
	};
}