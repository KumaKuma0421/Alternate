// ----------------------------------------------------------------------------
//! @file	QueryPerformance.h
//! @brief	高分解能時間計測に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.06

#include "pch.h"
#include "QueryPerformance.h"

using namespace alt;

QueryPerformance::QueryPerformance ()
{
	_startTick.QuadPart = 0;
	_stopTick.QuadPart = 0;
	::QueryPerformanceFrequency (&_frequency);
}

VOID QueryPerformance::Start ()
{
	::QueryPerformanceCounter (&_startTick);
}

VOID QueryPerformance::Stop ()
{
	::QueryPerformanceCounter (&_stopTick);
}

double QueryPerformance::PastTime () const
{
	return (_stopTick.QuadPart - _startTick.QuadPart) * 1000.0 \
		/ (double)_frequency.QuadPart;
}