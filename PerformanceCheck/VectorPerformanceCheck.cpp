//! @file	VectorPerformanceCheck.cpp
//! @brief	std::vector<T>«”\‘ª’è—p
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#include "pch.h"
#include "VectorPerformanceCheck.h"
#include "Freight.hpp"

using namespace PerfCheck;

VectorPerformanceCheck::VectorPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
}

VectorPerformanceCheck::~VectorPerformanceCheck ()
{
}

BOOL VectorPerformanceCheck::DoAction ()
{
	int count = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Vector"), _T("Count"), -1);
	if (count == -1) return FALSE;

	int size = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Vector"), _T("Size"), -1);
	if (size == -1) return FALSE;

	_console.Format (_T ("Count=%d Size=%d\n"), count, size);

	return this->Core (count, size);
}

BOOL VectorPerformanceCheck::Core (const int param1, const int param2)
{
	alt::QueryPerformance Q;
	typedef alt::skeleton::Freight<BYTE> ByteFreight;
	LPBYTE lpbyData = new BYTE[param2];
	ZeroMemory (lpbyData, param2);
	std::vector<ByteFreight> container;

	Q.Start ();

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.push_back (byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.insert (container.begin (), byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		ByteFreight byteFreight (lpbyData, param2);
		container.insert (container.end (), byteFreight);
	}

	for (int i = 0; i < param1; i++)
	{
		container.erase (container.begin ());
	}

	std::vector<ByteFreight> container2 = container;
	container.clear ();

	for (int i = 0; i < container2.size (); i++)
	{
		ByteFreight value = container2.at (i);
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