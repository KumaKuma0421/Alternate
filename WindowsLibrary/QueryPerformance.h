// ----------------------------------------------------------------------------
//! @file	QueryPerformance.h
//! @brief	高分解能時間計測に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.06

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	QueryPerformance
	//! @brief	高分解能時間計測クラス
	class DLL_DECLSPEC QueryPerformance : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY QueryPerformance ();

		//! @brief デストラクタ
		APIENTRY ~QueryPerformance () {};

		//! @brief 計測を開始します。
		VOID APIENTRY Start ();

		//! @brief 計測を終了します。
		VOID APIENTRY Stop ();

		//! @brief	計測開始～計測終了までの時間を出力します。
		//! @note	単位はミリ秒で出力されます。
		//! @note	有効値は小数点第４位まで
		double APIENTRY PastTime () const;

		//! @brief Start()時のカウンター値を取得します。
		LARGE_INTEGER APIENTRY GetStartTick () const { return _startTick; };

		//! @brief Stop()時のカウンター値を取得します。
		LARGE_INTEGER APIENTRY GetStopTick () const { return _stopTick; };

		//! @brief 現在のタイマー分解能を取得します。
		LARGE_INTEGER APIENTRY GetFrequency () const { return _frequency; };

	private:
		//! @brief 開始時タイマー値
		LARGE_INTEGER _startTick;

		//! @brief 終了時タイマー値
		LARGE_INTEGER _stopTick;

		//! @brief 現在のタイマー分解能
		LARGE_INTEGER _frequency;
	};
}