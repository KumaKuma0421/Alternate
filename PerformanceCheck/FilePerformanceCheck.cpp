//! @file	FilePerformanceCheck.cpp
//! @brief	Fileクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.15

#include "pch.h"
#include "FilePerformanceCheck.h"

using namespace PerfCheck;

FilePerformanceCheck::FilePerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
	_maxFileSize = -1;
}

FilePerformanceCheck::~FilePerformanceCheck ()
{
}

BOOL FilePerformanceCheck::DoAction ()
{
	_maxFileSize = 16 * 1000;
	this->Core ((int)pow (2, 0));   //           1
	this->Core ((int)pow (2, 1));   //           2
	this->Core ((int)pow (2, 2));   //           4
	this->Core ((int)pow (2, 3));   //           8
	this->Core ((int)pow (2, 4));   //          16

	_maxFileSize = 512 * 100;
	this->Core ((int)pow (2, 5));   //          32
	this->Core ((int)pow (2, 6));   //          64
	this->Core ((int)pow (2, 7));   //         128
	this->Core ((int)pow (2, 8));   //         256
	this->Core ((int)pow (2, 9));   //         512

	_maxFileSize = 16384 * 100;
	this->Core ((int)pow (2, 10));  //       1,024
	this->Core ((int)pow (2, 11));  //       2,048
	this->Core ((int)pow (2, 12));  //       4,096
	this->Core ((int)pow (2, 13));  //       8,192
	this->Core ((int)pow (2, 14));  //      16,384

	_maxFileSize = 1048576 * 100;
	this->Core ((int)pow (2, 15));  //      32,768
	this->Core ((int)pow (2, 16));  //      65,536
	this->Core ((int)pow (2, 17));  //     131,072
	this->Core ((int)pow (2, 18));  //     262,144
	this->Core ((int)pow (2, 19));  //     524.288
	this->Core ((int)pow (2, 20));  //   1,048,576

	_maxFileSize = 33554432 * 10;
	this->Core ((int)pow (2, 21));  //   2,097,152
	this->Core ((int)pow (2, 22));  //   4,194,304
	this->Core ((int)pow (2, 23));  //   8,388,608
	this->Core ((int)pow (2, 24));  //  16,777,216
	this->Core ((int)pow (2, 25));  //  33,554,432

	return TRUE;
}

BOOL FilePerformanceCheck::Core (const int param1)
{
	this->Core1 (param1);
	this->Core2 (param1);

	return TRUE;
}

BOOL FilePerformanceCheck::Core1 (const int param1)
{
	alt::QueryPerformance Q;

	Q.Start ();

	alt::File file;
	BYTE* byDummy = new BYTE[param1];
	ZeroMemory (byDummy, param1 * sizeof (BYTE));
	int writeSize = 0;

	file.Create (_T (".\\test.bin"), GENERIC_WRITE, FILE_SHARE_WRITE, CREATE_ALWAYS);

	while (writeSize < _maxFileSize)
	{
		DWORD dwWritten = file.Write (byDummy, param1 * sizeof (BYTE));
		writeSize += dwWritten;
	}

	file.Close ();

	delete[] byDummy;

	Q.Stop ();

	double writePerformance = ((writeSize / 1024 / 1024) / (Q.PastTime () / 1000));

	_console.Format (_T ("%10dbytes単位の書き込み 書き込み量:%10d 書き込み時間：%10.4fmsec 結果:%9.4fMBytes/sec\n"),
					 param1, writeSize, Q.PastTime (), writePerformance);

	return TRUE;
}

BOOL FilePerformanceCheck::Core2 (const int param1)
{
	alt::QueryPerformance Q;

	Q.Start ();

	alt::File file;
	BYTE* byDummy = new BYTE[param1];
	ZeroMemory (byDummy, param1 * sizeof (BYTE));
	int readSize = 0;

	file.Create (_T (".\\test.bin"), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);

	while (readSize < _maxFileSize)
	{
		DWORD dwRead = file.Read (byDummy, param1 * sizeof (BYTE));
		readSize += dwRead;
	}

	file.Close ();

	delete[] byDummy;

	Q.Stop ();

	double readPerformance = ((readSize / 1024 / 1024) / (Q.PastTime () / 1000));

	_console.Format (_T ("%10dbytes単位の書き込み 書き込み量:%10d 書き込み時間：%10.4fmsec 結果:%9.4fMBytes/sec\n"),
					 param1, readSize, Q.PastTime (), readPerformance);

	return TRUE;
}