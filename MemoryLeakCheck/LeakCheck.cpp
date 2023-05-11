//! @file	LeakCheck.cpp
//! @brief	TestProjectでは検出できない、メモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#include "pch.h"
#include "LeakCheck.h"
#include "Utility.h"
#include "ArrayMemoryLeakCheck.h"
#include "LinkedListMemoryLeakCheck.h"
#include "TStringMemoryLeakCheck.h"
#include "BinderMemoryLeakCheck.h"
#include "FreightMemoryLeakCheck.h"

int _tmain (int argc, TCHAR* argv[])
{
	CRT_SET_DEBUG_FLAG;

	int action = alt::Utility::ReadIniFile (CONFIG_FILE, _T ("Target"), _T ("Action"), -1);
	if (action == -1)
	{
		_tprintf (_T ("Actionの設定値が異常(%d)です。\n"), action);
		return -1;
	}

	LeakCheck::LeakCheck* check = nullptr;


	auto target0 = [&]()
	{
		check = new LeakCheck::LeakCheckTest ();
	};

	auto target1 = [&]()
	{
		check = new LeakCheck::TStringMemoryLeakCheck ();
	};

	auto target2 = [&]()
	{
		check = new LeakCheck::BinderMemoryLeakCheck ();
	};

	auto target3 = [&]()
	{
		check = new LeakCheck::FreightMemoryLeakCheck ();
	};

	auto target4 = [&]()
	{
		check = new LeakCheck::ArrayMemoryLeakCheck ();
	};

	auto target5 = [&]()
	{
		check = new LeakCheck::LinkedListMemoryLeakCheck ();
	};

	std::map<int, std::function<void()>> strategy;
	strategy[0] = target0;
	strategy[1] = target1;
	strategy[2] = target2;
	strategy[3] = target3;
	strategy[4] = target4;
	strategy[5] = target5;

	auto& target = strategy[action];
	target ();

	do
	{
		BOOL ret;
		ret = check->Init ();
		if (!ret) break;

		ret = check->DoAction ();
		if (!ret) break;

	} while (false);
	
	delete check;
}