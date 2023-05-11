//! @file	WatchDocTimerPerformanceCheck.cpp
//! @brief	WatchDocTimerクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#include "pch.h"
#include "WatchDocTimerPerformanceCheck.h"

using namespace PerfCheck;

WatchDocTimerPerformanceCheck::WatchDocTimerPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{

}

WatchDocTimerPerformanceCheck::~WatchDocTimerPerformanceCheck ()
{

}

BOOL WatchDocTimerPerformanceCheck::DoAction ()
{
	return this->Core ();
}

BOOL WatchDocTimerPerformanceCheck::Core ()
{
	int interval = ::GetPrivateProfileInt (
		_T ("WatchDogTimer"),
		_T ("Interval"),
		0,
		_T (".\\PerformanceCheck.ini"));
	if (interval == 0)
	{
		_tprintf (_T ("Not found Interval.\n"));
		return FALSE;
	}

	int count = ::GetPrivateProfileInt (
		_T ("WatchDogTimer"),
		_T ("Count"),
		0,
		_T (".\\PerformanceCheck.ini"));
	if (count == 0)
	{
		_tprintf (_T ("Not found count.\n"));
		return FALSE;
	}

	auto timer = new alt::WatchDogTimer ();
	auto data = new WatchDogData[count];
	ZeroMemory (data, sizeof (data));
	timer->InitTimer (_T ("DriveWatchDogTimerTest"));
	alt::QueryPerformance Q;

	timer->StartTimer (interval);

	for (int i = 0; i < count; i++)
	{
		Q.Start ();
		timer->WaitTimer ();
		Q.Stop ();

		data[i].Tick = interval;
		data[i].Elapsed = Q.PastTime ();
		data[i].StartTick = Q.GetStartTick ();
		data[i].StopTick = Q.GetStopTick ();
		data[i].Frequency = Q.GetFrequency ();
		GetLocalTime (&data[i].SystemTime);
	}

	timer->StopTimer ();

	double min = interval;
	double max = interval;
	double sum = 0;
	double average = 0;
	double sd = 0;

	for (int i = 0; i < count; i++)
	{
		if (min > data[i].Elapsed) min = data[i].Elapsed;
		if (max < data[i].Elapsed) max = data[i].Elapsed;
		sum += data[i].Elapsed;
	}
	average = sum / count;

	for (int i = 0; i < count; i++)
	{
		sd += pow (average - data[i].Elapsed, 2);
	}
	sd = pow (sd / count, 0.5);

	LONGLONG previousStartTick = 0;

	int seq = 0;
	for (int i = 0; i < count; i++)
	{
		if (data[i].Elapsed < (interval - sd) || (interval + sd) < data[i].Elapsed)
		{
			double loopInterval = (data[i].StartTick.QuadPart - previousStartTick) * 1000.0 / (double)data[i].Frequency.QuadPart;
			_tprintf (_T ("Seq=%3d No.%3d interval=%.4f Elapsed=%.4f diff=%+.4f LocalTime=%02d:%02d:%02d.%03d\n"),
					 seq++, i + 1,
					 loopInterval,
					 data[i].Elapsed,
					 data[i].Elapsed - interval,
					 data[i].SystemTime.wHour,
					 data[i].SystemTime.wMinute,
					 data[i].SystemTime.wSecond,
					 data[i].SystemTime.wMilliseconds);
		}

		previousStartTick = data[i].StartTick.QuadPart;
	}

	_tprintf (_T ("min=%.4f max=%.4f average=%.4f sd=%.4f\n"), min, max, average, sd);

	delete[] data;
	delete timer;

	_tprintf (_T ("DriveWatchDogTimerTest() OUT.\n"));

	return TRUE;
}