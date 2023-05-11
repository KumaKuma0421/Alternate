//! @file	DequePerformanceCheck.h
//! @brief	双方向キュークラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.31

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	DequePerformanceCheck
	//! @brief	双方向キュークラス性能測定用
	class DequePerformanceCheck : public PerformanceCheck
	{
	public:
		DequePerformanceCheck (alt::Console& console);
		virtual ~DequePerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1, const int param2);
	};
}