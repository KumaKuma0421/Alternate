//! @file	ArrayPerformanceCheck.cpp
//! @brief	Arrayクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#include "pch.h"
#include "ArrayPerformanceCheck.h"
#include "Freight.hpp"

using namespace PerfCheck;

ArrayPerformanceCheck::ArrayPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
}

ArrayPerformanceCheck::~ArrayPerformanceCheck ()
{
}

BOOL ArrayPerformanceCheck::DoAction ()
{
	int count = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Array"), _T("Count"), -1);
	if (count == -1) return FALSE;
	
	int size = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Array"), _T("Size"), -1);
	if (size == -1) return FALSE;

	_console.Format (_T ("Count=%d Size=%d\n"), count, size);

	return this->Core (count, size);
}

BOOL ArrayPerformanceCheck::Core (const int param1, const int param2)
{
	alt::QueryPerformance Q;
	typedef alt::skeleton::Freight<BYTE> ByteFreight;
	LPBYTE lpbyData = new BYTE[param2];
	ZeroMemory (lpbyData, param2);
	alt::skeleton::Array<ByteFreight> container;

	Q.Start ();

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.Add (byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.Insert (byteFreight, 0);
	}

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.Insert (byteFreight, container.Size());
	}

	for (int i = 0; i < param1; i++)
	{
		container.Remove (0);
	}

	alt::skeleton::Array<ByteFreight> container2 = container;
	container.Clear ();

	for (int i = 0; i < container2.Size (); i++)
	{
		ByteFreight* value = container2.Get (i);
		LPBYTE lpbyData = value->GetData ();
		size_t size = value->GetSize ();
	}

	container2.Clear ();

	Q.Stop ();

	delete[] lpbyData;
	int msec = static_cast<int>(Q.PastTime ());

	_console.Format( _T ("It takes %dmsec.\n"), msec);

	return TRUE;
}