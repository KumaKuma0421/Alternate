//! @file	HeapPerformanceCheck.cpp
//! @brief	HeapMemoryクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.11

#include "pch.h"
#include "HeapPerformanceCheck.h"

using namespace PerfCheck;

HeapPerformanceCheck::HeapPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
}

HeapPerformanceCheck::~HeapPerformanceCheck ()
{
}

BOOL HeapPerformanceCheck::DoAction ()
{
	return this->Core (0);
}

BOOL HeapPerformanceCheck::Core (const int param1)
{
	alt::QueryPerformance Q;

	auto threadFunc = [](LPVOID lpvParam)->DWORD
	{
		BOOL ret;
		HeapPerformanceCheck* me = (HeapPerformanceCheck*)lpvParam;
		
		alt::HeapMemory memory;
		ret = memory.Create ();
		if (!ret) return -1;

		alt::Event event;
		int loop = me->_loop;

		event.Open (_T ("START_EVENT"));
		event.Wait ();

		OutputDebugString (_T ("Thread action is started.\n"));
		do
		{
			int index = rand () % 10;
			TCHAR tsz[10];
			wsprintf (tsz, _T ("%d(%d),"), GetCurrentThreadId (), index);
			OutputDebugString (tsz);

			LPVOID lpvMem = memory.Allocate (me->_allocSize[index]);
			ZeroMemory (lpvMem, me->_allocSize[index]);

			ret = memory.Free (lpvMem);

		} while (--loop);
		OutputDebugString (_T ("Thread action is stopped.\n"));

		return 0;
	};

	alt::Event event;
	event.Create (_T ("START_EVENT"), TRUE, FALSE);
	_console.Write (_T ("Event作成\n"));

	const int numOfThreads = 48;
	this->_loop = 50000;

	alt::Thread thread[numOfThreads];
	for (auto& item : thread)
	{
		item.Create (threadFunc, (LPVOID)this);
		SwitchToThread ();
	}
	_console.Write (_T ("Thread作成\n"));

	HANDLE threadHandles[numOfThreads];
	for (int i = 0; i < numOfThreads; i++)
	{
		threadHandles[i] = thread[i].GetHandle ();
	}
	_console.Write (_T ("Thread Handle収集\n"));

	_console.Write (_T ("計測開始\n"));
	Q.Start ();

	event.Set ();
	WaitForMultipleObjects (numOfThreads, threadHandles, TRUE, INFINITE);

	Q.Stop ();
	_console.Write (_T ("計測終了\n"));

	int msec = static_cast<int>(Q.PastTime ());

	_console.Format( _T ("It takes %dmsec.\n"), msec);

	return TRUE;
}