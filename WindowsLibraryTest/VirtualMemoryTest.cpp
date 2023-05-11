//! @file	VirtualMemoryTest.cpp
//! @brief	仮想メモリに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.2.24

#include "pch.h"
#include "VirtualMemory.h"
#include "Thread.h"
#include "TString.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (VirtualMemoryTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("VirtualMemoryTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("VirtualMemoryTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			SYSTEM_INFO si;
			alt::Utility::GetSystemInfo (si);
			_dwPageSize = si.dwPageSize;
			alt::TString message;
			message.Format (_T ("PageSize is %d\n"), _dwPageSize);
			Logger::WriteMessage ("VirtualMemoryTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("VirtualMemoryTest method cleanup.\n");
		}

		DWORD _dwPageSize; // Initialized by 'METHOD_INITIALIZE'.
		TEST_METHOD (VirtualMemory1Test1)
		{
			alt::TString message;
			alt::VirtualMemory1* memory = new alt::VirtualMemory1 ();
			SIZE_T size = (SIZE_T)_dwPageSize * 20;
			message.Format (_T ("PageSize=%d size=%d\n"), _dwPageSize, size);
			Logger::WriteMessage (message.Ctr ());

			LPVOID lpvMem = memory->Allocate (size);
			Assert::IsNotNull (lpvMem, _T("Allocate() failed.\n"));

			FillMemory (lpvMem, size, 'A');

			Assert::IsTrue(memory->Free (lpvMem), _T("Free() failed.\n"));

			delete memory;
		}

		TEST_METHOD (VirtualMemoryTest1)
		{
			BOOL ret;
			alt::TString message;
			alt::VirtualMemory* memory = new alt::VirtualMemory ();
			SIZE_T size = (SIZE_T)(1024 * 1024 * 10);

			LPBYTE lpbyMemReserve = static_cast<LPBYTE>(memory->Reserve (size));
			Assert::IsNotNull (lpbyMemReserve, _T("Reserve() failed.\n"));
			message.Format (_T ("Reserved address is %08X\n"), lpbyMemReserve);
			Logger::WriteMessage (message.Ctr ());

			SIZE_T chunkSize = SIZE_T(1024 * 1024);

			LPBYTE lpbyOffset1 = static_cast<LPBYTE>(lpbyMemReserve);
			LPBYTE lpbyMemCommit1 = static_cast<LPBYTE>(memory->Commit (lpbyOffset1, chunkSize));
			Assert::IsNotNull (lpbyMemCommit1, _T("Commit(1) failed.\n"));
			message.Format (_T ("Committed address1 is %08X\n"), lpbyMemCommit1);
			Logger::WriteMessage (message.Ctr ());
			
			FillMemory (lpbyMemCommit1, chunkSize, 'A');

			LPBYTE lpbyOffset2 = static_cast<LPBYTE>(lpbyMemReserve + chunkSize);
			LPBYTE lpbyMemCommit2 = static_cast<LPBYTE>(memory->Commit (lpbyOffset2, chunkSize));
			Assert::IsNotNull (lpbyMemCommit1, _T("Commit(2) failed.\n"));
			message.Format (_T ("Committed address2 is %08X\n"), lpbyMemCommit2);
			Logger::WriteMessage (message.Ctr ());

			FillMemory (lpbyMemCommit2, chunkSize, 'B');

			ret = memory->Decommit (lpbyMemCommit1, chunkSize);
			Assert::IsTrue (ret, _T("Decommit(1) failed.\n"));

			ret = memory->Decommit (lpbyMemCommit2, chunkSize);
			Assert::IsTrue (ret, _T("Decommit(2) failed.\n"));

			ret = memory->Release (lpbyMemReserve);
			Assert::IsTrue (ret, _T("Release() failed.\n"));

			delete memory;
		}

#ifdef _DEBUG
		const int counter = 100;
#else
		const int counter = 10000;
#endif
		typedef std::function<LPBYTE (int)> MemoryCreator;
		typedef std::function<void (LPBYTE)> MemoryDeleter;

		class Parameters
		{
		public:
			Parameters (int size, int count, MemoryCreator creator, MemoryDeleter deleter)
				:iSize (size), iCount (count), creator (creator), deleter (deleter)
			{};
			int iSize;
			int iCount;
			MemoryCreator creator;
			MemoryDeleter deleter;
		};

		void ByteMemTest (int size, int count, MemoryCreator creator, MemoryDeleter deleter)
		{
			alt::Thread* thread = new alt::Thread[10];
			Parameters params (size, count, creator, deleter);

			auto function = [](LPVOID lpvParam)->DWORD
			{
				Parameters* params = (Parameters*)lpvParam;

				for (int i = 0; i < params->iCount; i++)
				{
					LPBYTE lpbyMemory = params->creator (params->iSize);
					BYTE data = ('a' + (i % 20));
					FillMemory (lpbyMemory, params->iSize, data);
					Assert::AreEqual<BYTE> (data, lpbyMemory[params->iSize - 1], _T ("character mismatched.\n"));
					params->deleter (lpbyMemory);
				}

				return true;
			};

			for (int i = 0; i < 10; i++)
			{
				thread[i].Create (function, &params);
			}

			HANDLE hThreads[10]{ 0,0,0,0,0,0,0,0,0,0 };
			for (int i = 0; i < 10; i++)
			{
				hThreads[i] = thread[i].GetHandle ();
			}
			::WaitForMultipleObjects (10, hThreads, TRUE, INFINITE);

			delete[] thread;
		}

		alt::VirtualMemory1 vMemory;
		MemoryCreator virtualCreator = [&](int size)->LPBYTE { return (LPBYTE)vMemory.Allocate (size); };
		MemoryDeleter virtualDeleter = [&](LPBYTE lpbyData)->void { vMemory.Free (lpbyData); };

		TEST_METHOD (ByteMemTest_Virtual_512k)
		{
			ByteMemTest (1024 * 512, counter, virtualCreator, virtualDeleter);
		}

		TEST_METHOD (ByteMemTest_Virtual_1M)
		{
			ByteMemTest (1024 * 1024, counter, virtualCreator, virtualDeleter);
		}
	};
}