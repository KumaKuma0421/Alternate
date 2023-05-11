//! @file	FreightPerformanceCheck.h
//! @brief	Freight�N���X���\����p
//! @author	kumakuma0421@gmail.com
//! @date	2022.7.23

#pragma once

#include "PerformanceCheck.h"
#include "QueryPerformance.h"
#include "Utility.h"
#include "Freight.hpp"
#include "Memory.h"
#include "HeapMemory.h"
#include "VirtualMemory.h"

namespace PerfCheck
{
	//! @class	FreightPerformanceCheck
	//! @brief	Freight�N���X���\����p
	class FreightPerformanceCheck : public PerformanceCheck
	{
	public:
		FreightPerformanceCheck (alt::Console& console);
		~FreightPerformanceCheck ();

		BOOL DoAction ();

		typedef alt::skeleton::Freight<BYTE> ByteFreight;

	protected:
		BOOL Measure (int size, int count, ByteFreight& freight);
	};
}