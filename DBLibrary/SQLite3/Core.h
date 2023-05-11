//! @file	Core.h
//! @brief	SqLite3操作クラス(Core)
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#pragma once

#include <Windows.h>
#include "sqlite3.h"

namespace alt
{
	namespace db
	{
		//! @class Core
		//! @brief SqLite3操作クラス(Core)
		//! @sa https://www.sqlite.org/cintro.html
		class Core
		{
		public:
			//! @brief コンストラクタ
			Core () { _lastError = SQLITE_OK; }

			//! @brief デストラクタ
			virtual ~Core () {}

		protected:
			//! @brief 各関数の戻り値判定共通
			bool getResponse () { return _lastError != SQLITE_OK ? false : true; }

			//! @brief SQLite3関数エラー値
			int _lastError;
		};
	}
}