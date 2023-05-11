//! @file	PerformanceDataHelper.cpp
//! @brief	パフォーマンスモニターに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.16
//! 
#include "pch.h"
#include "PerformanceDataHelper.h"
#include "Array.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (PerformanceDataHelperTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("PerformanceDataHelperTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("PerformanceDataHelperTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("PerformanceDataHelperTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("PerformanceDataHelperTest method cleanup.\n");
		}

		TEST_METHOD (PerformanceDataHelperTest1)
		{
			alt::PerformanceDataHelper pdh;
			alt::skeleton::Array<alt::PerformanceDataItem> itemArray;

			Assert::IsTrue (pdh.Open (), _T ("PerformanceDataHelper::Open() failed.\n"));
			
			auto& item01 = pdh.CreateItem (_T("\\Processor(_Total)\\% Processor Time"));itemArray.Add (item01);
			auto& item02 = pdh.CreateItem (_T("\\Processor(_Total)\\% User Time"));itemArray.Add (item02);
			auto& item03 = pdh.CreateItem (_T("\\Processor(_Total)\\% Privileged Time"));itemArray.Add (item03);
			auto& item04 = pdh.CreateItem (_T("\\Processor(_Total)\\Interrupts/sec"));itemArray.Add (item04);

			auto& item05 = pdh.CreateItem (_T("\\Memory\\Available MBytes"));itemArray.Add (item05);
			auto& item06 = pdh.CreateItem (_T("\\Memory\\Pages/sec"));itemArray.Add (item06);
			auto& item07 = pdh.CreateItem (_T("\\Memory\\Committed Bytes"));itemArray.Add (item07);
			auto& item08 = pdh.CreateItem (_T("\\Memory\\% Committed Bytes In Use"));itemArray.Add (item08);

			auto& item09 = pdh.CreateItem (_T("\\Paging File(_Total)\\% Usage"));itemArray.Add (item09);

			auto& item10 = pdh.CreateItem (_T("\\PhysicalDisk(_Total)\\% Disk Time"));itemArray.Add (item10);
			auto& item11 = pdh.CreateItem (_T("\\PhysicalDisk(_Total)\\Current Disk Queue Length"));itemArray.Add (item11);
			auto& item12 = pdh.CreateItem (_T("\\PhysicalDisk(_Total)\\Disk Writes/sec"));itemArray.Add (item12);

			auto& item13 = pdh.CreateItem (_T("\\System\\Processor Queue Length"));itemArray.Add (item13);
			auto& item14 = pdh.CreateItem (_T("\\System\\Context Switches/sec"));itemArray.Add (item14);

			auto& item15 = pdh.CreateItem (_T("\\Network Interface(Microsoft Hyper-V Network Adapter)\\Bytes Total/sec"));itemArray.Add (item15);

			Assert::IsTrue (pdh.Query (), _T ("PerformanceDataHelper::Query() failed.\n"));

			for (int i = 0; i < 3; i++)
			{
				Sleep (1000); // Do something 
				Assert::IsTrue (pdh.Query (), _T ("PerformanceDataHelper::Query() failed.\n"));
				for (auto& item : itemArray)
				{
					PDH_FMT_COUNTERVALUE value;
					item.GetValue (PDH_FMT_DOUBLE, value);
					alt::TString output;
					output.Format (_T ("%-70s:%7.3f\n"), item.GetKey ().Ctr(), value.doubleValue);
					Logger::WriteMessage (output.Ctr ());
				}
				Logger::WriteMessage (_T ("----------------------------------------------------------------------\n"));
			}

			Assert::IsTrue (pdh.Close (), _T ("PerformanceDataHelper::Close() failed.\n"));
		}
	};
}