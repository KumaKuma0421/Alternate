//! @file	WatchDocTimerPerformanceCheck.h
//! @brief	WatchDocTimerクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#pragma once

#include "PerformanceCheck.h"
#include "ObjectHolder.h"
#include "Utility.h"
#include "WatchDogTimer.h"
#include "Console.h"

namespace PerfCheck
{
	//! @struct	WatchDogData
	//! @brief	WatchDocTimerデータ保存用
	typedef struct _WatchDogData
	{
		DWORD Tick;
		double Elapsed;
		SYSTEMTIME SystemTime;
		LARGE_INTEGER StartTick;
		LARGE_INTEGER StopTick;
		LARGE_INTEGER Frequency;
	} WatchDogData;

	//! @class	WatchDocTimerPerformanceCheck
	//! @brief	WatchDocTimerクラス性能測定用
	class WatchDocTimerPerformanceCheck : public PerformanceCheck
	{
	public:
		WatchDocTimerPerformanceCheck();
		virtual ~WatchDocTimerPerformanceCheck();

		virtual BOOL DoAction();

	protected:
		virtual BOOL Core();
	};
}