//! @file	ArrayPerformanceCheck.h
//! @brief	Array�N���X���\����p
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	ArrayPerformanceCheck
	//! @brief	Array�N���X���\����p
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