//! @file	DequePerformanceCheck.cpp
//! @brief	双方向キュークラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.31

#include "pch.h"
#include "DequePerformanceCheck.h"
#include "Freight.hpp"

using namespace PerfCheck;

DequePerformanceCheck::DequePerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
}

DequePerformanceCheck::~DequePerformanceCheck ()
{
}

BOOL DequePerformanceCheck::DoAction ()
{
	int count = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Deque"), _T("Count"), -1);
	if (count == -1) return FALSE;
	
	int size = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Deque"), _T("Size"), -1);
	if (size == -1) return FALSE;

	_console.Format (_T ("Count=%d Size=%d\n"), count, size);

	return this->Core (count, size);
}

BOOL DequePerformanceCheck::Core (const int param1, const int param2)
{
	alt::QueryPerformance Q;
	typedef alt::skeleton::Freight<BYTE> ByteFreight;
	LPBYTE lpbyData = new BYTE[param2];
	ZeroMemory (lpbyData, param2);
	std::deque<ByteFreight> container;

	Q.Start ();

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.push_back (byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.push_front (byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.push_front (byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		container.erase (container.begin ());
	}

	std::deque<ByteFreight> container2 = container;
	container.clear ();

	for (int i = 0; i < container2.size (); i++)
	{
		ByteFreight& value = container2.at (i);
		LPBYTE lpbyData = value.GetData ();
		size_t size = value.GetSize ();
	}

	container2.clear ();

	Q.Stop ();

	delete[] lpbyData;
	int msec = static_cast<int>(Q.PastTime ());

	_console.Format( _T ("It takes %dmsec.\n"), msec);

	return TRUE;
}