// ----------------------------------------------------------------------------
//! @file	Diagnostics.cpp
//! @brief	システム性能に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.24

#include "pch.h"
#include "Diagnostics.h"

using namespace alt;

Diagnostics::Diagnostics ()
{
	_hObject = ::GetCurrentProcess ();
}

BOOL Diagnostics::GetProcessMemory (
	PROCESS_MEMORY_COUNTERS& processMemoryCounters) const
{
	return ::GetProcessMemoryInfo (
		_hObject, &processMemoryCounters, sizeof (processMemoryCounters));
}

BOOL Diagnostics::GetProcessIO (IO_COUNTERS& ioCounters) const
{
	return ::GetProcessIoCounters (_hObject, &ioCounters);
}

BOOL Diagnostics::GetIpStatistics (MIB_IPSTATS& mibIPStats) const
{
	ULONG ret;

	ret = ::GetIpStatistics (&mibIPStats);

	return ret == NOERROR ? TRUE : FALSE;
}