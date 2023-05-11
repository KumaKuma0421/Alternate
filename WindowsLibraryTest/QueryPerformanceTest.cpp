//! @file	QueryPerformanceTest.cpp
//! @brief	高分解能時間計測に関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.08

#include "pch.h"
#include "QueryPerformance.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (QueryPerformanceTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("QueryPerformanceTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("QueryPerformanceTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("QueryPerformanceTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("QueryPerformanceTest method cleanup.\n");
		}

		TEST_METHOD (QueryPerformance1)
		{
			const int count = 30;
			const DWORD cdwTime = 20;
			alt::QueryPerformance Q;
			TCHAR tszMsg[80];
			double dArray[count];

			for (int i = 0; i < count; i++)
			{
				Q.Start ();
				Sleep (cdwTime);
				Q.Stop ();
				dArray[i] = Q.PastTime ();
			}

			double sum = 0;
			double average = 0;
			double minValue = cdwTime;
			double maxValue = cdwTime;
			double sd = 0;

			for (int i = 0; i < count; i++)
			{
				sum += dArray[i];
				if (minValue > dArray[i]) minValue = dArray[i];
				if (maxValue < dArray[i]) maxValue = dArray[i];
			}
			average = sum / count;

			for (int i = 0; i < count; i++)
			{
				sd += pow (average - dArray[i], 2);
			}
			sd = pow (sd / count, 0.5);

			_stprintf_s (tszMsg, _T ("interval=%d minValue=%.4f maxValue=%.4f average=%.4f sd=%.4f\n"), cdwTime, minValue, maxValue, average, sd);
			Logger::WriteMessage (tszMsg);

			int outbound = 0;
			for (int i = 0; i < count; i++)
			{
				double diff = dArray[i] - cdwTime;
				if (-0.9 > diff || 0.9 < diff)
				{
					outbound++;
					_stprintf_s (tszMsg, _T ("i=%3d outbound=%3d diff=%.4f\n"), i, outbound, diff);
					Logger::WriteMessage (tszMsg);
				}
			}
		}
	};
}