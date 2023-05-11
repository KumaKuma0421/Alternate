//! @file	FilePerformanceCheck.h
//! @brief	Fileクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"
#include "File.h"

namespace PerfCheck
{
	//! @class	FilePerformanceCheck
	//! @brief	Fileクラス性能測定用
	class FilePerformanceCheck : public PerformanceCheck
	{
	public:
		FilePerformanceCheck (alt::Console& console);
		virtual ~FilePerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1);
		virtual BOOL Core1 (const int param1);
		virtual BOOL Core2 (const int param1);

		size_t _maxFileSize;
	};
}