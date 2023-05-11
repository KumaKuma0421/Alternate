//! @file	LinkedListPerformanceCheck.h
//! @brief	連結リストクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.31

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	LinkedListPerformanceCheck
	//! @brief	連結リストクラス性能測定用
	class LinkedListPerformanceCheck : public PerformanceCheck
	{
	public:
		LinkedListPerformanceCheck (alt::Console& console);
		virtual ~LinkedListPerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1, const int param2);
	};
}