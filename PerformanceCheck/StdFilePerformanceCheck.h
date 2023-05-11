//! @file	StdFilePerformanceCheck.h
//! @brief	File�N���X�̔�r�p
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.24

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"

namespace PerfCheck
{
	//! @class	StdFilePerformanceCheck
	//! @brief	File�N���X�̔�r�p
	class StdFilePerformanceCheck : public PerformanceCheck
	{
	public:
		StdFilePerformanceCheck (alt::Console& console);
		virtual ~StdFilePerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1);
		virtual BOOL Core1 (const int param1);
		virtual BOOL Core2 (const int param1);

		size_t _maxFileSize;
	};
}