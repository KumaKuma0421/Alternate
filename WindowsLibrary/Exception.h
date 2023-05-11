// ----------------------------------------------------------------------------
//! @file	Exception.h
//! @brief	例外に関するクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

#define ALT_REASON_EXCEPTION        _T("例外が発生しました。")
#define ALT_REASON_INVALID_ARGUMENT _T("引数の設定が異常です。")
#define ALT_REASON_RUNTIME_ERROR	_T("実行時にエラーが発生しました。")

namespace alt
{
	namespace ex
	{
		//! @class	Exception
		//! @brief	基底となる例外クラス
		class DLL_DECLSPEC Exception : public WindowsLibrary
		{
		public:
			//! @brief コンストラクタ
			APIENTRY Exception ();

			//! @brief デストラクタ
			APIENTRY ~Exception ();

			//! @brief 原因を取得
			//! @return 発生原因
			LPCTSTR APIENTRY GetReason () const;
		};

		//! @class	InvalidArgmentException
		//! @brief	引数設定エラー例外
		class DLL_DECLSPEC InvalidArgumentException : Exception
		{
		public:
			//! @brief コンストラクタ
			APIENTRY InvalidArgumentException ();

			//! @brief デストラクタ
			APIENTRY ~InvalidArgumentException ();

			//! @brief 原因を取得
			//! @return 発生原因
			LPCTSTR APIENTRY GetReason () const;
		};

		//! @class	RuntimeException
		//! @brief	実行時エラー例外
		class DLL_DECLSPEC RuntimeException : Exception
		{
		public:
			//! @brief コンストラクタ
			APIENTRY RuntimeException ();

			//! @brief デストラクタ
			APIENTRY ~RuntimeException ();

			//! @brief 原因を取得
			//! @return 発生原因
			LPCTSTR APIENTRY GetReason () const;
		};
	}
}