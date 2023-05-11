//! @file	ConsolePerformanceCheck.cpp
//! @brief	Consoleクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#include "pch.h"
#include "ConsolePerformanceCheck.h"

using namespace PerfCheck;

ConsolePerformanceCheck::ConsolePerformanceCheck (alt::Console& console)
	:PerformanceCheck(console)
{
	response0 = 0;
	response1 = 0;
	response2 = 0;
}

ConsolePerformanceCheck::~ConsolePerformanceCheck ()
{

}

BOOL ConsolePerformanceCheck::DoAction ()
{
	this->Core (0);
	this->Core (1);
	this->Core (2);

	_console.Format (_T ("response0 = %dmsec response1 = %dmsec response2 = %dmsec\n"),
					 response0, response1, response2);

	return TRUE;
}

BOOL ConsolePerformanceCheck::Core (const int param1)
{
	const int count = 1000;
	alt::QueryPerformance Q;

	Q.Start ();

	_console.Clear ();

	if (param1 == 2)
	{
		_console.SetWindowInfo (80, 25);
		_console.SetBufferSize (80, 25);
	}

	for (int i = 0; i < count; i++)
	{
		if (param1 == 0)
		{
			_tprintf (_T ("ABCDEFGHIJKLMLOPQRSTUVWXYZ ABCDEFGHIJKLMLOPQRSTUVWXYZ ABCDEFGHIJKLMLOPQRSTUVWXYZ\n"));
			_tprintf (_T ("☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆\n"));
		}
		else if (param1 == 1)
		{
			_console.Write (_T ("abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz\n"));
			_console.Write (_T ("★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★\n"));
		}
		else
		{
			_console.SetCursorPosition (10, 10);
			_console.Write (_T ("abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz\n"));
			_console.SetCursorPosition (10, 20);
			_console.Write (_T ("◆◇□■◆◇□■◆◇◆◇□■◆◇□■◆◇◆◇□■◆◇□■◆◇◆◇□■◆◇□■◆◇\n"));
		}
	}

	Q.Stop ();

	int msec = static_cast<int>(Q.PastTime ());
	if (param1 == 0) response0 = msec;
	if (param1 == 1) response1 = msec;
	if (param1 == 2) response2 = msec;

	return TRUE;
}