//! @file	TStringPerformanceCheck.h
//! @brief	TStringクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.21

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"
#include "TString.h"
#include "File.h"

//#define TEST_FILE _T(".\\zenkoku.csv")
#define TEST_FILE _T(".\\41saga.csv")

namespace PerfCheck
{
	//! @class	TStringPerformanceCheck
	//! @brief	TStringクラス性能測定用
	class TStringPerformanceCheck : public PerformanceCheck
	{
	public:
		TStringPerformanceCheck (alt::Console& console);
		virtual ~TStringPerformanceCheck ();

		virtual BOOL DoAction ();

	protected:
		virtual BOOL Core (const int param1);
		BOOL Core1 ();
		BOOL Core2 ();
		BOOL Core3 ();
	};
}