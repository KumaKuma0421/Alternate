//! @file	ArrayPerformanceCheck.h
//! @brief	Arrayクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	ArrayPerformanceCheck
	//! @brief	Arrayクラス性能測定用
	class ArrayPerformanceCheck : public PerformanceCheck
	{
	public:
		ArrayPerformanceCheck (alt::Console& console);
		virtual ~ArrayPerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1, const int param2);
	};
}