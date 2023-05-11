//! @file	VectorPerformanceCheck.h
//! @brief	std::vector<T>���\����p
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	VectorPerformanceCheck
	//! @brief	std::vector<T>�N���X���\����p
	class VectorPerformanceCheck : public PerformanceCheck
	{
	public:
		VectorPerformanceCheck (alt::Console& console);
		virtual ~VectorPerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1, const int param2);
	};
}