//! @file	WatchDogTimerTest.cpp
//! @brief	タイマーに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "WatchDogTimer.h"
#include "Thread.h"
#include "Binder.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (WatchDogTimerTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("WatchDogTimer class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("WatchDogTimer class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("WatchDogTimer method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("WatchDogTimer method cleanup.\n");
		}

		class TimeupFunction : public alt::skeleton::ITimeup
		{
		public:
			TimeupFunction ()
			{
				_dwPreviousTime = 0;
				ZeroMemory (_tszMsg, sizeof (_tszMsg));
			}

			void Timeup (LPVOID me, DWORD dwHiTime, DWORD dwLoTime)
			{
				_stprintf_s (_tszMsg, _T ("--TimeupFunction::Timeup() dwHiTime=%u diff=%u\n"), dwHiTime, dwHiTime - _dwPreviousTime);
				Logger::WriteMessage (_tszMsg);
				_dwPreviousTime = dwHiTime;
			}

		private:
			DWORD _dwPreviousTime;
			TCHAR _tszMsg[80];
		};

		TEST_METHOD (WatchDogTimer1)
		{
			const int count = 30;
			const DWORD cdwTime = 20;

			timeBeginPeriod (1);

			alt::skeleton::Binder<TimeupFunction> function;
			alt::skeleton::Binder<alt::WatchDogTimer, TimeupFunction*> T;
			function.New ();
			T.New (function.Get());

			BOOL ret1 = T->InitTimer (_T ("WatchDogTimer1"));
			Assert::IsTrue (ret1);

			alt::QueryPerformance Q;

			TCHAR tszMsg[80];
			double dArray[count];
			ZeroMemory (dArray, sizeof (dArray));

			Logger::WriteMessage ("Timer Start.\n");
			BOOL ret2 = T->StartTimer (cdwTime);
			Assert::IsTrue (ret2);

			int iCounter = count - 1;
			while (true)
			{
				Q.Start ();
				DWORD dwRet = T->WaitTimer ();
				Q.Stop ();

				if (dwRet == WAIT_OBJECT_0)
				{
					dArray[iCounter] = Q.PastTime ();
					iCounter--;
					if (iCounter < 0) break;
				}
			}

			T->StopTimer ();
			timeEndPeriod (1);

			double minValue = cdwTime;
			double maxValue = cdwTime;
			double sum = 0;
			double average = 0;
			double sd = 0;

			for (int i = 0; i < count; i++)
			{
				if (minValue > dArray[i]) minValue = dArray[i];
				if (maxValue < dArray[i]) maxValue = dArray[i];
				sum += dArray[i];
			}
			average = sum / count;

			for (int i = 0; i < count; i++)
			{
				sd += pow (average - dArray[i], 2);
			}
			sd = pow (sd / count, 0.5);

			_stprintf_s (tszMsg, _T ("interval=%d minValue=%.4f maxValue=%.4f average=%.4f sd=%.4f\n"), cdwTime, minValue, maxValue, average, sd);
			Logger::WriteMessage (tszMsg);

			int outband = 0;
			for (int i = 0; i < count; i++)
			{
				double diff = dArray[i] - cdwTime;
				if (diff < -0.9 || 0.9 < diff)
				{
					outband++;
					_stprintf_s (tszMsg, _T ("i=%3d outband=%3d diff=%.4f\n"), i, outband, diff);
					Logger::WriteMessage (tszMsg);
				}

			}

			Logger::WriteMessage ("Timer Stopped.\n");
		}

		TEST_METHOD (WatchDogTimer2)
		{
			const int count = 30;
			const DWORD cdwTime = 20;

			timeBeginPeriod (1);

			alt::skeleton::Binder<alt::WatchDogTimer> T;
			T.New ();

			BOOL ret1 = T->InitTimer (_T ("WatchDogTimer2"));
			Assert::IsTrue (ret1);

			alt::QueryPerformance Q;

			TCHAR tszMsg[80];
			double dArray[count];
			ZeroMemory (dArray, sizeof (dArray));

			Logger::WriteMessage ("Timer Start.\n");
			BOOL ret2 = T->StartTimer (cdwTime);
			Assert::IsTrue (ret2);

			int iCounter = count;
			while (iCounter--)
			{
				Q.Start ();
				DWORD dwRet = T->WaitTimer ();
				Q.Stop ();

				if (dwRet == WAIT_OBJECT_0)
				{
					dArray[iCounter] = Q.PastTime ();
				}
			}

			T->StopTimer ();
			timeEndPeriod (1);

			double minValue = cdwTime;
			double maxValue = cdwTime;
			double sum = 0;
			double average = 0;
			double sd = 0;

			for (int i = 0; i < count; i++)
			{
				if (dArray[i] < minValue) minValue = dArray[i];
				if (dArray[i] > maxValue) maxValue = dArray[i];
				sum += dArray[i];
			}
			average = sum / count;

			for (int i = 0; i < count; i++)
			{
				sd += pow (average - dArray[i], 2);
			}
			sd = pow (sd / count, 0.5);

			_stprintf_s (tszMsg, _T ("interval=%d minValue=%.4f maxValue=%.4f average=%.4f sd=%.4f\n"), cdwTime, minValue, maxValue, average, sd);
			Logger::WriteMessage (tszMsg);

			int outband = 0;
			for (int i = 0; i < count; i++)
			{
				double diff = dArray[i] - cdwTime;
				if (diff < -0.9 || 0.9 < diff)
				{
					outband++;
					_stprintf_s (tszMsg, _T ("i=%3d outband=%3d diff=%.4f\n"), i, outband, diff);
					Logger::WriteMessage (tszMsg);
				}

			}

			Logger::WriteMessage ("Timer Stopped.\n");
		}
	};
}