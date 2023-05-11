// ----------------------------------------------------------------------------
//! @file	FunctionPointers.hpp
//! @brief	コールバック機能を集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	namespace skeleton
	{
		//! @brief CreateThread()に渡すスレッド実行関数のテンプレート
		typedef DWORD (*Invoker)(LPVOID me);

		//! @class	ICallback
		//! @brief	Threadクラスのスレッド実行関数を設定します。
		//! @note	Invoker()は直後にInvoke()を呼び出します。
		//! 		Invoke()は純粋仮想関数なので、スレッド実行させたい機能は、
		//!			継承先で実装してください。
		class DLL_DECLSPEC ICallback : public WindowsLibrary
		{
		public:
			//! @brief コールバック関数プレースホルダ
			//! @param lpvParam コールバック関数に渡されるパラメータ
			//! @return DWORD
			//! @note ユーザー側でコールバック関数の実体を記載します。
			virtual DWORD APIENTRY Invoke (LPVOID lpvParam) = 0;

			//! @brief コールバック関数呼び出し用インターフェース
			//! @param me 呼び出し元のアドレス
			//! @return Invoke()の戻り値
			static DWORD APIENTRY Invoker (LPVOID me)
			{
				ICallback* function = (ICallback*)me;
				return function->Invoke (me);
			};
		};

		//! @class	ITimeup
		//! @brief	WatchDogTimerクラスのコールバック関数を設定します。
		//! @note	TimeupCall()は直後にTimeup()を呼び出します。
		//! 		Timeup()は純粋仮想関数なので、コールバック後に実行する機能は、
		//! 		継承先で実装してください。
		class DLL_DECLSPEC ITimeup : public WindowsLibrary
		{
		public:
			//! @brief タイムアップ関数プレースホルダ
			//! @param me 呼び出し元のアドレス
			//! @param dwHiTime タイマー値（上位）
			//! @param dwLoTime タイマー値（下位）
			virtual VOID APIENTRY Timeup (
				LPVOID me, DWORD dwHiTime, DWORD dwLoTime) = 0;

			//! @brief タイムアップ関数の呼び出し用インターフェース
			//! @param me 呼び出し元のアドレス
			//! @param dwHiTime タイマー値（上位）
			//! @param dwLoTime タイマー値（下位）
			//! @return VOID
			static VOID APIENTRY TimeupCall (
				LPVOID me, DWORD dwHiTime, DWORD dwLoTime)
			{
				ITimeup* function = (ITimeup*)me;
				function->Timeup (me, dwHiTime, dwLoTime);
			}
		};
	}
}