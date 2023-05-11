//! @file	DequePerformanceCheck.h
//! @brief	�o�����L���[�N���X���\����p
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.31

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	DequePerformanceCheck
	//! @brief	�o�����L���[�N���X���\����p
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