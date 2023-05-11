//! @file	Statement.h
//! @brief	SqLite3操作クラス(Statement)
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#pragma once

#include <Windows.h>
#include "sqlite3.h"
#include "Core.h"
#include "Engine.h"

namespace alt
{
	namespace db
	{
		//! @class Statement
		//! @brief SqLite3操作クラス(Statement)
		class Statement : public Core
		{
		public:
			//! @brief コンストラクタ
			Statement ()
				:Core ()
			{
				_statement = nullptr;
			}

			//! @brief デストラクタ
			virtual ~Statement ()
			{
				if (_statement) Finalize ();
			}

			//! @brief		SQLステートメントの準備
			//! @param[in]	engine		Engineインスタンス
			//! @param[in]	lpctszSQL	SQLステートメント
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			//! @todo		Engine→Engine&へ
			bool Prepare (Engine engine, LPCTSTR lpctszSQL)
			{
				_lastError = ::sqlite3_prepare16 (engine.GetEngine (), lpctszSQL, -1, &_statement, nullptr);
				return getResponse () && _statement != nullptr;
			}

			//! @brief		パラメータのバインド処理
			//! @param[in]	position 列番号
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			//! @note		カラムにnullptrを設定します
			bool Bind (int position)
			{
				_lastError = ::sqlite3_bind_null (_statement, position);
				return getResponse ();
			}

			//! @brief		パラメータのバインド処理
			//! @param[in]	position	列番号
			//! @param[in]	value		int値
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			bool Bind (int position, int value)
			{
				_lastError = ::sqlite3_bind_int (_statement, position, value);
				return getResponse ();
			}

			//! @brief		パラメータのバインド処理
			//! @param[in]	position	列番号
			//! @param[in]	value		sqlite3_int64値
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			bool Bind (int position, sqlite3_int64 value)
			{
				_lastError = ::sqlite3_bind_int64 (_statement, position, value);
				return getResponse ();
			}

			//! @brief		パラメータのバインド処理
			//! @param[in]	position	列番号
			//! @param[in]	value		double値
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			bool Bind (int position, double value)
			{
				_lastError = ::sqlite3_bind_double (_statement, position, value);
				return getResponse ();
			}

			//! @brief		パラメータのバインド処理
			//! @param[in]	position	列番号
			//! @param[in]	value		LPCSTR値
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			bool Bind (int position, LPCSTR value)
			{
				_lastError = ::sqlite3_bind_text (_statement, position, value, (int)lstrlenA (value), nullptr);
				return getResponse ();
			}

			//! @brief		パラメータのバインド処理
			//! @param[in]	position	列番号
			//! @param[in]	value		LPCWSTR値
			//! @return		実行結果
			//! @retval		true	成功
			//! @retval		false	失敗
			bool Bind (int position, LPCWSTR value)
			{
				_lastError = ::sqlite3_bind_text16 (_statement, position, value, (int)lstrlenW (value) * sizeof (wchar_t), nullptr);
				return getResponse ();
			}

			//! @todo blob, blob64

			//! @brief	ステートメント実行
			//! @return	実行結果
			//! @retval	true	成功
			//! @retval	false	失敗
			bool Execute ()
			{
				_lastError = ::sqlite3_step (_statement);
				return getResponse ();
			}

			//! @brief	バインドのリセット
			//! @return	実行結果
			//! @retval	true	成功
			//! @retval	false	失敗
			bool ResetBind ()
			{
				_lastError = ::sqlite3_reset (_statement);
				bool ret = getResponse ();
				if (!ret) return ret;

				_lastError = ::sqlite3_clear_bindings (_statement);
				return getResponse ();
			}

			//! @brief	カラム数の取得
			//! @return	カラム数
			int GetColumns ()
			{
				return ::sqlite3_column_count (_statement);
			}

			//! @brief	カラム名の取得
			//! @return	カラム名
			LPCTSTR GetColumnName (int position)
			{
				return (LPCTSTR)::sqlite3_column_name16 (_statement, position);
			}

			//! @brief	カラムタイプの取得
			//! @return カラムタイプ
			LPCTSTR GetColumnType (int position)
			{
				return (LPCTSTR)::sqlite3_column_decltype16 (_statement, position);
			}

			//! @brief		行データの取得
			//! @param[in]	position	カラム位置
			//! @return		カラム値(int)
			int GetIntValue (int position)
			{
				return ::sqlite3_column_int (_statement, position);
			}

			//! @brief		行データの取得
			//! @param[in]	position	カラム位置
			//! @return		カラム値(sqlite3_int64)
			sqlite3_int64 GetInt64Value (int position)
			{
				return ::sqlite3_column_int64 (_statement, position);
			}

			//! @brief		行データの取得
			//! @param[in]	position	カラム位置
			//! @return		カラム値(double)
			double GetDoubleValue (int position)
			{
				return ::sqlite3_column_double (_statement, position);
			}

			//! @brief		行データの取得
			//! @param[in]	position	カラム位置
			//! @return		カラム値(LPCSTR)
			LPCSTR GetStringValue (int position)
			{
				return (LPCSTR)::sqlite3_column_text (_statement, position);
			}

			//! @brief		行データの取得
			//! @param[in]	position	カラム位置
			//! @return		カラム値(LPCTSTR)
			LPCTSTR GetString16Value (int position)
			{
				return (LPCTSTR)::sqlite3_column_text16 (_statement, position);
			}

			//! @brief	終了処理
			//! @return	実行結果
			//! @retval	true	成功
			//! @retval	false	失敗
			bool Finalize ()
			{
				_lastError = sqlite3_finalize (_statement);
				_statement = nullptr;
				return getResponse ();
			}

		protected:
			//! @brief ステートメント
			sqlite3_stmt* _statement;
		};
	}
}