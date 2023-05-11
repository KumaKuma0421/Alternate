//! @file	FreightPerformanceCheck.cpp
//! @brief	Freightクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2022.7.23

#include "pch.h"
#include "FreightPerformanceCheck.h"

using namespace PerfCheck;

FreightPerformanceCheck::FreightPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
}

FreightPerformanceCheck::~FreightPerformanceCheck ()
{
}

BOOL FreightPerformanceCheck::DoAction ()
{
	BOOL ret;
	alt::BasicMemory basicMem;
	alt::HeapMemory systemHeap;
	alt::HeapMemory userHeap;
	alt::VirtualMemory1 virtualMem;

	ret = systemHeap.Open ();
	if (!ret) return FALSE;

	ret = userHeap.Create ();
	if (!ret) return FALSE;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int count = alt::Utility::ReadIniFile(CONFIG_FILE, _T("Freight"), _T("Count"), -1);
			if (count == -1) return FALSE;

			int size = static_cast<int>(pow (8, j));
			_console.Format (_T ("Freigtのサイズ:%10d ループ回数:%4d "), size, count);

			switch (i)
			{
				case 0:
				{
					ByteFreight freight;
					_console.Write (_T ("BasicMemory    "));
					ret = this->Measure (size, count, freight);
				}
					break;
				case 1:
				{
					ByteFreight freight (&systemHeap);
					_console.Write (_T ("システムヒープ "));
					ret = this->Measure (size, count, freight);
				}
					break;
				case 2:
				{
					ByteFreight freight (&userHeap);
					_console.Write (_T ("ユーザーヒープ "));
					ret = this->Measure (size, count, freight);
				}
					break;
				case 3:
				{
					ByteFreight freight (&virtualMem);
					_console.Write (_T ("仮想メモリ     "));
					ret = this->Measure (size, count, freight);
				}
					break;
			}

			if (!ret) break;
		}
	}

	return ret;
}

BOOL FreightPerformanceCheck::Measure (int size, int count, ByteFreight& freight)
{
	alt::QueryPerformance Q;
	int loop = count;

	Q.Start ();

	do
	{
		freight.New (size);

		//ZeroMemory (freight.GetData(), freight.GetSize());

		freight.Clear ();
	} while (loop--);

	Q.Stop ();
	double msec = Q.PastTime ();

	double count_per_msec = count / msec;
	double msec_per_count = msec / count;
	_console.Format (_T ("ratio %8.2f[count/msec] %8.2f[msec/count]\n"), count_per_msec, msec_per_count);

	return TRUE;
}