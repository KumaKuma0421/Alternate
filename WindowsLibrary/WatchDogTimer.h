// ----------------------------------------------------------------------------
//! @file	WatchDogTimer.h
//! @brief	タイマーに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "FunctionPointers.hpp"
#include "QueryPerformance.h"

namespace alt
{
	//! @class	WatchDogTimer
	//! @brief	タイマーに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC WatchDogTimer : public WaitableHandleLibrary
	{
	public:
		//! @brief コンストラクタ
		//! @note  時間になると、イベントで通知します。
		APIENTRY WatchDogTimer ();

		//! @brief コンストラクタ
		//! @param[in] timeup 時間になると、timeupコールバックで通知します。
		APIENTRY WatchDogTimer (skeleton::ITimeup* timeup);

		//! @brief 初期化
		//! @param lpctszName タイマーの名称
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY InitTimer (LPCTSTR lpctszName = NULL);

		//! @brief タイマー開始
		//! @param dwTime タイマーインターバル
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY StartTimer (DWORD dwTime) const;

		//! @brief タイムアップ待機
		//! @return DWORD
		//! @retval WAIT_OBJECT_O タイムアップによるブレイク
		DWORD APIENTRY WaitTimer () const;

		//! @brief タイマー停止
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY StopTimer () const;

	private:
		//! @brief コールバック関数を定義したクラス
		skeleton::ITimeup* _timeup;
	};
}