//! @file	ConsolePerformanceCheck.h
//! @brief	Console�N���X���\����p
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	ConsolePerformanceCheck
	//! @brief	Console���\����p
	class ConsolePerformanceCheck : public PerformanceCheck
	{
	public:
		ConsolePerformanceCheck (alt::Console& console);
		virtual ~ConsolePerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1);
		int response0;
		int response1;
		int response2;
	};
}