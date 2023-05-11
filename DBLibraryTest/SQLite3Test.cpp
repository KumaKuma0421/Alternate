//! @file	SQLite3Test.cpp
//! @brief	SQLite3に関するAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "SQLite3/Engine.h"
#include "SQLite3/Statement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace alt;

namespace DBLibraryTest
{
	TEST_CLASS (SQLite3Test)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("-->>method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("<<--method cleanup.\n");
		}

		// TIPS
		// select * from sqlite_master;
		//      type = table
		//      name = <テーブル名>
		//  tbl_name = <テーブル名>
		//  rootpage = 数値
		//       sql = <テーブル作成時のSQL>

		// TIPS データ型の決定方法
		// @sa https://so-zou.jp/web-app/tech/database/sqlite/data/data-type.htm
		//  カラム型
		//  INTEGER	符号付き整数(1,2,4,8バイト)
		//  TEXT	テキスト(UTF-8, UTF-16(BE,LE)
		//  BLOB	Binary Large OBject
		//  REAL	浮動小数点(8バイト)
		//  NUMERIC	INTEGERまたはREALのどちらか

		// TIPS CREATE TABLE
		//  "CREATE TABLE [IF NOT EXISTS] <スキーマ名>.<テーブル名>(カラム名 カラム型, カラム名 カラム型,…);"
		//  column1 data_type PRIMARY KEY(or FOREIGN KEY, UNIQUE, CHECK)
		//  column2 data_type NOT NULL,
		//  column3 data_type DEFAULT 0,
		// @sa https://seraphy.hatenablog.com/entry/20061031/p1
		// @sa https://www.sqlitetutorial.net/sqlite-create-table/

		// CREATE TABLE IF NOT EXISTS T_DOBLEKEY (
		//   ID     INTEGER NOT NULL,
		//   SUB-ID INTEGER NOT NULL,
		//   ...
		//   PRIMARY KEY (ID, SUB-ID)
		// );

		// CREATE [UNIQUE] INDEX index_name ON table_name(column_list);

		TEST_METHOD (SQLite3Test1)
		{
			bool ret;
			db::Engine engine;

			ret = engine.Create (_T ("TestData.db"));
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			//! @todo LPCTSTRでは不可能？
			LPCSTR lpcszSQL1 = "CREATE TABLE IF NOT EXISTS TESTTBL(IDX INTEGER PRIMARY KEY, VAL1 VARCHAR, VAL2 NUMBER, VAL3 TEXT);";
			ret = engine.Execute (lpcszSQL1);
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			LPCSTR lpcszSQL2 = "DELETE FROM TESTTBL;";
			ret = engine.Execute (lpcszSQL2);
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			ret = engine.BeginTransaction ();
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			db::Statement statement;

			LPCTSTR lpctszSQL3 = _T ("INSERT INTO TESTTBL(VAL1, VAL2, VAL3) VALUES(?, ?, ?)");
			ret = statement.Prepare (engine, lpctszSQL3);
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			for (int i = 0; i < 20; i++)
			{
				ret = statement.Bind (1, i);
				Assert::IsTrue (ret, engine.GetErrorMessage ());
				if (i % 3 == 0)
				{
					ret = statement.Bind (2, i * 10);
					Assert::IsTrue (ret, engine.GetErrorMessage ());
				}
				TCHAR tszDateTime[80];
				SYSTEMTIME T;
				GetLocalTime (&T);
				wsprintf (tszDateTime, _T ("%04d/%02d/%02d %02d:%02d:%02d.%03d"), T.wYear, T.wMonth, T.wDay, T.wHour, T.wMinute, T.wSecond, T.wMilliseconds);
				ret = statement.Bind (3, tszDateTime);
				Assert::IsTrue (ret, engine.GetErrorMessage ());

				int response = statement.Execute ();
				Assert::AreEqual<int> (SQLITE_DONE, response, engine.GetErrorMessage ());

				TCHAR tszRow[20];
				wsprintf (tszRow, _T ("ROWID:%I64u\n"), engine.GetLastRowID ());
				Logger::WriteMessage (tszRow);

				ret = statement.ResetBind ();
				Assert::IsTrue (ret, engine.GetErrorMessage ());
			}

			ret = statement.Finalize ();
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			ret = engine.CommitTransaction ();
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			db::Statement statement2;

			LPCTSTR lpctszSQL4 = _T ("SELECT IDX, VAL1, VAL2, VAL3 FROM TESTTBL ORDER BY IDX");
			ret = statement2.Prepare (engine, lpctszSQL4);
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			int columnCount = statement2.GetColumns ();
			for (int col = 0; col < columnCount; col++)
			{
				TCHAR tszMsg[128];
				wsprintf (tszMsg, _T ("%d %s %s\n"),
						 col,
						 statement2.GetColumnName (col),
						 statement2.GetColumnType (col));
				Logger::WriteMessage (tszMsg);
			}

			for (;;)
			{
				int response = statement2.Execute ();
				if (response == SQLITE_DONE) break;

				int idx = statement2.GetIntValue (0);
				LPCSTR lpcszVal1 = statement2.GetStringValue (1);
				LPCSTR lpcszVal2 = statement2.GetStringValue (2);
				LPCTSTR lpctszVal3 = statement2.GetString16Value (3);

				TCHAR tszMsg[128];
				wsprintf (tszMsg, _T ("%d\t%S\t%S\t%s\n"), idx, lpcszVal1, lpcszVal2, lpctszVal3);
				Logger::WriteMessage (tszMsg);
			}

			ret = statement2.Finalize ();
			Assert::IsTrue (ret, engine.GetErrorMessage ());

			ret = engine.Close ();
			Assert::IsTrue (ret, engine.GetErrorMessage ());
		}
	};
}