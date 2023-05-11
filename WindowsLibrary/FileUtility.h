// ----------------------------------------------------------------------------
//! @file	FileUtility.h
//! @brief	ファイルハンドルを伴わないファイルIOに関するWindowsAPIを集約した
//!         クラス
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
	//! @brief SHFILEOPSTRUCT.wFuncに渡す、値の列挙
	enum class ShellOperation : WORD
	{
		MOVE = FO_MOVE,
		COPY = FO_COPY,
		REMOVE = FO_DELETE,
		RENAME = FO_RENAME,
	};

	//! @brief ファイル情報共通クラス
	class DLL_DECLSPEC FileInfo
	{
	public:
		//! @brief コンストラクタ
		APIENTRY FileInfo ();

		//! @brief デストラクタ
		virtual APIENTRY ~FileInfo ();

		//! @brief フォルダ名の取得
		//! @return TString&
		const TString& APIENTRY GetFolderName () const { return _FolderName; };

		//! @brief ファイル名の取得
		//! @return TString&
		const TString& APIENTRY GetFileName () const { return _FileName; };

		//! @brief ファイルサイズの取得
		//! @return ULONGLONG&
		const ULONGLONG& APIENTRY GetFileSize () const { return _FileSize.QuadPart; };

		friend FileUtility;

	private:
		TString _FolderName;
		TString _FileName;
		ULARGE_INTEGER _FileSize;
	};

	//! @brief FileUtility::Find()が返すファイル情報
	class DLL_DECLSPEC FileInfo1 : public FileInfo
	{
	public:
		//! @brief コンストラクタ
		APIENTRY FileInfo1 ();

		//! @brief デストラクタ
		APIENTRY ~FileInfo1 ();

		//! @brief ファイル作成日時の取得
		//! @return FILETIME&
		const FILETIME& APIENTRY GetCreateTime () const { return _CreateTime; };

		//! @brief ファイル更新日時の取得
		//! @return FILETIME&
		const FILETIME& APIENTRY GetUpdateTime () const { return _UpdateTime; };

		//! @brief ファイルアクセス日時の取得
		//! @return FILETIME&
		const FILETIME& APIENTRY GetAccessTime () const { return _AccessTime; };

		//! @brief ファイル属性の取得
		//! @return INT&
		const INT& APIENTRY GetAttribute () const { return _Attribute; };

		//! @brief 読み取り専用か否か
		//! @return BOOL
		//! @retval TRUE(読み取り属性ON)
		const BOOL APIENTRY IsReadOnly () const
		{
			return _Attribute & FILE_ATTRIBUTE_READONLY ? TRUE : FALSE;
		};

		//! @brief 隠しファイルか否か
		//! @return BOOL
		//! @retval TRUE(隠し属性ON)
		const BOOL APIENTRY IsHidden () const
		{
			return _Attribute & FILE_ATTRIBUTE_HIDDEN ? TRUE : FALSE;
		};

		//! @brief システムファイルか否か
		//! @return BOOL
		//! @retval TRUE(システム属性ON)
		const BOOL APIENTRY IsSystem () const
		{
			return _Attribute & FILE_ATTRIBUTE_SYSTEM ? TRUE : FALSE;
		};

		//! @brief ディレクトリか否か
		//! @return BOOL
		//! @retval TRUE(これはディレクトリ)
		const BOOL APIENTRY IsDirectory () const
		{
			return _Attribute & FILE_ATTRIBUTE_DIRECTORY ? TRUE : FALSE;
		};

		//! @brief アーカイブか否か
		//! @return BOOL
		//! @retval TRUE(アーカイブ属性ON)
		const BOOL APIENTRY IsArchive () const
		{
			return _Attribute & FILE_ATTRIBUTE_ARCHIVE ? TRUE : FALSE;
		};

		//! @brief 中間ファイルか否か
		//! @return BOOL
		//! @retval TRUE(中間ファイル)
		const BOOL APIENTRY IsTemporary () const
		{
			return _Attribute & FILE_ATTRIBUTE_TEMPORARY ? TRUE : FALSE;
		};

		//! @brief 圧縮ファイルか否か
		//! @return BOOL
		//! @retval TRUE(圧縮ファイル)
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
		//! @brief コンストラクタ
		APIENTRY FileInfo2 ();

		//! @brief デストラクタ
		APIENTRY ~FileInfo2 ();

		//! @brief ファイル監視結果を取得
		//! @return DWORD
		//! @retval ファイルの状態変化フラグ
		//!   FILE_ACTION_ADDED,
		//!   FILE_ACTION_REMOVED,
		//!   FILE_ACTION_MODIFIED,
		//!   FILE_ACTION_RENAMED_OLD_NAME,
		//!   FILE_ACTION_RENAMED NEW_NAME
		const DWORD APIENTRY GetAction () const { return _dwAction; };

		//! @brief ファイルIDの取得
		//! @return LONGLONG
		const LONGLONG APIENTRY GetFileID () const { return _FileID; };

		//! @brief ファイル変更検知日時の取得
		//! @return FILETIME&
		const FILETIME& APIENTRY GetChangeTime () const { return _ChangeTime; };

		//! @brief 追加されたか
		//! @return BOOL
		//! @retval TRUE(追加を確認)
		const BOOL APIENTRY IsAdded () const
		{
			return _dwAction == FILE_ACTION_ADDED ? TRUE : FALSE;
		};

		//! @brief 削除されたか
		//! @return BOOL
		//! @retval TRUE(削除を確認)
		const BOOL APIENTRY IsRemoved () const
		{
			return _dwAction == FILE_ACTION_REMOVED ? TRUE : FALSE;
		};

		//! @brief 変更されたか
		//! @return BOOL
		//! @retval TRUE(変更を確認)
		const BOOL APIENTRY IsModified () const
		{
			return _dwAction == FILE_ACTION_MODIFIED ? TRUE : FALSE;
		};

		//! @brief 名前を変更されたか（旧ファイル名)
		//! @return BOOL
		//! @retval TRUE(名前変更を確認：旧ファイル名)
		const BOOL APIENTRY IsOldName () const
		{
			return _dwAction == FILE_ACTION_RENAMED_OLD_NAME ? TRUE : FALSE;
		};

		//! @brief 名前を変更されたか（新ファイル名)
		//! @return BOOL
		//! @retval TRUE(名前変更を確認：新ファイル名)
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
	//! @brief	ファイルハンドルを伴わないファイルIOに関するWindowsAPIを
	//!         集約したクラス
	class DLL_DECLSPEC FileUtility : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY FileUtility () {};

		//! @brief デストラクタ
		APIENTRY ~FileUtility () {};

		// ----- ファイル操作

		//! @brief ファイルの削除
		//! @param name 削除するファイル名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY Delete (LPCTSTR name);

		//! @brief ファイルのコピー
		//! @param source コピー元ファイル名
		//! @param dest コピー先ファイル名
		//! @param bFailIfExists ファイルが存在する場合に実行するか否か
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY Copy (
			LPCTSTR source, LPCTSTR dest, BOOL bFailIfExists = FALSE);

		//! @brief ファイルの移動
		//! @param source 移動元ファイル名
		//! @param dest 移動先ファイル名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY Move (LPCTSTR source, LPCTSTR dest);

		//! @brief テンポラリファイル名を取得します。
		//! @param[in] lpctszPath ファイルパス
		//! @param[in] lpctszPrefix ファイル名プレフィックス(３文字まで)
		//! @param[in] nNumber ファイル名に使用される番号(下位１６ビットまで)
		//! @note nNumberに０を指定するとシステムが生成します。
		static TString APIENTRY GetTemporaryFileName (
			LPCTSTR lpctszPath, LPCTSTR lpctszPrefix, UINT nNumber);

		// ----- ディレクトリ操作

		//! @brief ディレクトリの作成
		//! @param name ディレクトリ名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY MakeDir (LPCTSTR name);

		//! @brief ディレクトリの削除
		//! @param name ディレクトリ名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		//! @note ディレクトリ内のファイルを再帰的に削除するには、
		//!       SHFileOperation()を使用します。
		static BOOL APIENTRY RemoveDir (LPCTSTR name);

		//! @brief カレントディレクトリの移動
		//! @param name ディレクトリ名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY SetCurrentDir (LPCTSTR name);

		//! @brief カレントディレクトリ名の取得
		//! @return カレントディレクトリ名
		static TString APIENTRY GetCurrentDir ();

		//! @brief テンポラリフォルダを取得します。
		//! @return TString フォルダ名
		static TString APIENTRY GetTemporaryPath ();

		//! @brief カレントディレクトリのパスを連結して絶対パスの作成
		//! @param lpctszRelativePath カレントディレクトリからの相対パス
		//! @return TString カレントディレクトリ＋指定パス
		static TString APIENTRY GetAbsolutePath (LPCTSTR lpctszRelativePath);

		//! @brief ディレクトリの存在確認
		//! @param name 確認するディレクトリ名
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY IsDirectory (LPCTSTR name);

		// ----- ファイル・ディレクトリ共用操作

		//! @brief ディレクトリ・ファイルの存在確認
		//! @param name ディレクトリ・ファイル名
		//! @return BOOL
		//! @retval TRUE(存在する)
		//! @retval FALSE(存在しない)
		static BOOL APIENTRY IsExist (LPCTSTR name);

		//! @brief ファイルの検索
		//! @param target 検索対象ディレクトリ名
		//! @param name 検索対象ファイル名
		//! @param bRecurse 再帰するか否か
		//! @return 検索結果
		static skeleton::Array<FileInfo1> APIENTRY Find (
			LPCTSTR target, LPCTSTR name, BOOL bRecurse = FALSE);

		static VOID APIENTRY Find2 (
			LPCTSTR target, LPCTSTR name, BOOL bRecurse,
			skeleton::Array<FileInfo1>& response);

		//! @brief ディレクトリの監視
		//! @param lpctszWatchDirectory 監視対象ディレクトリ名
		//! @param dwTimeout 監視時間
		//! @return alt::skeleton::Array<alt::TString>
		//! @retval 監視中に変化があったファイル情報
		static skeleton::Array<FileInfo2> APIENTRY DirectoryWatch (
			LPCTSTR lpctszWatchDirectory, DWORD dwTimeout);

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

		//! @brief シンボリックリンクの作成
		//! @param source 作成元ファイル名
		//! @param link 作成リンク名
		//! @param isFile 作成元がファイルか否か
		//! @note  SymbolicLinkはdirコマンドで'SYMLINK'、'SYMLINKD'と
		//!        表示される。またエクスプローラではショートカットの
		//!        矢印がアイコンに表示される。ただし、管理者権限が作成
		//!        に必要となる。
		//!        コマンドは、mklink ～またはmklink /d ～で作成する
		static BOOL APIENTRY MakeSymbolicLink (
			LPCTSTR source, LPCTSTR link, BOOL isFile = TRUE);

#endif
		//! @brief ハードリンクの作成
		//! @param source 作成元ファイル名
		//! @param link 作成リンク名
		//! @note  HardLinkはdirコマンドで区別がつかない。アイコンにも
		//!        変化が現れない。こちらは管理者権限が不要である。
		//!        コマンドは、mklink /h ～で作成する。削除はdel。
		//!        ディレクトリに対しては作成不可。同一ボリュームのみ。
		static BOOL APIENTRY MakeHardLink (LPCTSTR source, LPCTSTR link);

		static INT APIENTRY SHFileOperations (
			ShellOperation operation, LPCTSTR lpctszFrom, LPCTSTR lpctszTo);
	};
}