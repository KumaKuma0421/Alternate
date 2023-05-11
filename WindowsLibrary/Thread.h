// ----------------------------------------------------------------------------
//! @file	Thread.h
//! @brief	スレッドに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "FunctionPointers.hpp"

namespace alt
{
	//! @enum	ThreadPriority
	//! @brief	スレッドの優先順位
	enum class ThreadPriority
	{
		//! クリティカル
		Critical = THREAD_PRIORITY_TIME_CRITICAL,

		//! 優先度高
		Highest = THREAD_PRIORITY_HIGHEST,

		//! 通常より少し上
		AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,

		//! 通常
		Normal = THREAD_PRIORITY_NORMAL,

		//! 通常より少し下
		BelowNormal = THREAD_PRIORITY_BELOW_NORMAL,

		//! 優先度低
		Lowest = THREAD_PRIORITY_LOWEST,

		//! アイドル
		Idle = THREAD_PRIORITY_IDLE
	};

	//! @class	Thread
	//! @brief	スレッドに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC Thread : public WaitableHandleLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY Thread ();

		//! @brief コンストラクタ
		//! @param callback スレッド実行関数
		APIENTRY Thread (skeleton::ICallback* callback);

		//! @brief		スレッドを作成します。
		//! @param[in]	function スレッド起動する関数
		//! @param[in]	lpvParam スレッド機能する関数へのパラメータ
		//! @param[in]	dwCreationFlag CREATE_SUSPENDEDを指定すると
		//!             起動と同時に停止します。
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL APIENTRY Create (
			PTHREAD_START_ROUTINE function = NULL, LPVOID lpvParam = NULL,
			DWORD dwCreationFlag = 0);

		//! @brief	スレッドを一時停止します。
		//! @return	BOOL
		//! @retval	TRUE(成功)
		//! @retval	FALSE(失敗)
		BOOL APIENTRY Suspend () const;

		//! @brief	スレッドを再開します。
		//! @return	BOOL
		//! @retval	TRUE(成功)
		//! @retval	FALSE(失敗)
		BOOL APIENTRY Resume () const;

		//! @brief		スレッド実行優先度を変更します。
		//! @param[in]	priority スレッド実行優先度
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL APIENTRY SetPriority (ThreadPriority priority) const;

	protected:
		skeleton::ICallback* _callback;
		DWORD				 _dwThreadID;
	};
}