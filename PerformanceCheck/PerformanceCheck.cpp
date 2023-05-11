//! @file	PerformanceCheck.cpp
//! @brief	性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#include "pch.h"
#include "PerformanceCheck.h"
#include "ConsolePerformanceCheck.h"
#include "FilePerformanceCheck.h"
#include "FreightPerformanceCheck.h"
#include "VectorPerformanceCheck.h"
#include "ArrayPerformanceCheck.h"
#include "LinkedListPerformanceCheck.h"
#include "DequePerformanceCheck.h"
#include "TStringPerformanceCheck.h"
#include "StdFilePerformanceCheck.h"
#include "HeapPerformanceCheck.h"
#include "NewPerformanceCheck.h"
#include "WatchDocTimerPerformanceCheck.h"
#include "TCPPerformanceCheck.h"
#include "UDPPerformanceCheck.h"

int _tmain (int argc, TCHAR* argv[])
{
	alt::TString message;
	alt::Console console;
	console.Init ();
	console.Clear ();

	wchar_t* locale = _tsetlocale (LC_ALL, _T ("Japanese"));
	console.Format (_T ("Now locale is %s\n"), locale);

	int action = (int)alt::Utility::ReadIniFile (
		CONFIG_FILE, _T ("Target"), _T ("Action"), -1);
	if (action == -1)
	{
		console.Format (_T ("Actionの設定値が異常(%d)です。\n"), action);
		return -1;
	}

	PerfCheck::PerformanceCheck* check = nullptr;

	auto target1 = [&]()
	{
		message = _T ("ConsolePerformanceCheck\n");
		check = new PerfCheck::ConsolePerformanceCheck (console);
	};

	auto target2 = [&]()
	{
		message = _T ("FilePerformanceCheck\n");
		check = new PerfCheck::FilePerformanceCheck (console);
	};

	auto target3 = [&]()
	{
		message = _T ("StdFilePerformanceCheck\n");
		check = new PerfCheck::StdFilePerformanceCheck (console);
	};

	auto target4 = [&]()
	{
		message = _T ("FreightPerformanceCheck\n");
		check = new PerfCheck::FreightPerformanceCheck (console);
	};

	auto target5 = [&]()
	{
		message = _T ("VectorPerformanceCheck\n");
		check = new PerfCheck::VectorPerformanceCheck (console);
	};

	auto target6 = [&]()
	{
		message = _T ("ArrayPerformanceCheck\n");
		check = new PerfCheck::ArrayPerformanceCheck (console);
	};

	auto target7 = [&]()
	{
		message = _T ("LinkedListPerformanceCheck\n");
		check = new PerfCheck::LinkedListPerformanceCheck (console);
	};

	auto target8 = [&]()
	{
		message = _T ("DequePerformanceCheck\n");
		check = new PerfCheck::DequePerformanceCheck (console);
	};

	auto target9 = [&]()
	{
		message = _T ("TStringPerformanceCheck\n");
		check = new PerfCheck::TStringPerformanceCheck (console);
	};

	auto target10 = [&]()
	{
		message = _T ("HeapPerformanceCheck\n");
		check = new PerfCheck::HeapPerformanceCheck (console);
	};

	auto target11 = [&]()
	{
		message = _T ("NewPerformanceCheck\n");
		check = new PerfCheck::NewPerformanceCheck (console);
	};

	auto target12 = [&]()
	{
		message = _T ("WatchDocTimerPerformanceCheck\n");
		check = new PerfCheck::WatchDocTimerPerformanceCheck (console);
	};

	auto target13 = [&]()
	{
		message = _T ("TCPPerformanceCheck\n");
		check = new PerfCheck::TCPPerformanceCheck (console);
	};

	auto target14 = [&]()
	{
		message = _T ("UDPPerformanceCheck\n");
		check = new PerfCheck::UDPPerformanceCheck (console);
	};

	std::map<int, std::function<void ()>> strategy;
	strategy[1] = target1;
	strategy[2] = target2;
	strategy[3] = target3;
	strategy[4] = target4;
	strategy[5] = target5;
	strategy[6] = target6;
	strategy[7] = target7;
	strategy[8] = target8;
	strategy[9] = target9;
	strategy[10] = target10;
	strategy[11] = target11;
	strategy[12] = target12;
	strategy[13] = target13;
	strategy[14] = target14;

	auto& target = strategy[action];
	target ();
	check->Init ();
	console.Write (message.Ctr());
	check->DoAction ();
	delete check;
}

PerfCheck::PerformanceCheck::PerformanceCheck (alt::Console& console)
	:_console(console)
{
}

PerfCheck::PerformanceCheck::~PerformanceCheck ()
{
}

VOID PerfCheck::PerformanceCheck::Init ()
{
}