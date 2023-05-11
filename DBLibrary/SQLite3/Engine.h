//! @file	Engine.h
//! @brief	SqLite3操作クラス(Engine)
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#pragma once

#include <Windows.h>
#include "sqlite3.h"
#include "Core.h"

namespace alt
{
	namespace db
	{
		//! @class	Engine
		//! @brief SqLite3操作クラス(Engine)
		class Engine : public Core
		{
		public:
			//! @brief コンストラクタ
			Engine ()
				:Core ()
			{
				_engine = nullptr;
				_lastError = SQLITE_OK;
			}

			//! @brief デストラクタ
			virtual ~Engine ()
			{
				if (_engine != nullptr) Close ();
			}

			//! @brief エラーメッセージの取得
			LPCTSTR GetErrorMessage ()
			{
				return (LPCTSTR)::sqlite3_errmsg16 (_engine);
			}

			//! @brief		DBファイルの作成・オープン
			//! @param[in]	lpctszDBName データベースファイル名
			bool Create (LPCTSTR lpctszDBName)
			{
				_lastError = ::sqlite3_open16 (lpctszDBName, &_engine);
				return getResponse ();
			}

			//! @brief DBファイルのクローズ
			bool Close ()
			{
				_lastError = ::sqlite3_close (_engine);
				_engine = nullptr;
				return getResponse ();
			}

			//! @brief		SQLステートメント実行
			//! @param[in]	lpcszSQL SQLステートメント
			bool Execute (LPCSTR lpcszSQL)
			{
				_lastError = ::sqlite3_exec (_engine, lpcszSQL, nullptr, nullptr, nullptr);
				return getResponse ();
			}

			//! @brief トランザクション開始
			bool BeginTransaction ()
			{
				return Execute ("BEGIN;");
			}

			//! @brief トランザクションコミット
			bool CommitTransaction ()
			{
				return Execute ("COMMIT;");
			}

			//! @brief トランザクションロールバック
			bool RollbackTransaction ()
			{
				return Execute ("ROLLBACK;");
			}

			//! @brief 空き領域の整理
			bool Vacuum ()
			{
				return Execute ("VACUUM;");
			}

			//! @brief 最後の実行時のROWIDを取得
			ULONGLONG GetLastRowID ()
			{
				return ::sqlite3_last_insert_rowid (_engine);
			}

			//! @brief エンジン部を取得
			sqlite3* GetEngine ()
			{
				return _engine;
			}

		protected:
			//! @brief SQLite3コンテキスト
			sqlite3* _engine;
		};
	}
}