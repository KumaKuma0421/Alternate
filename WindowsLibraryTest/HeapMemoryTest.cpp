//! @file	HeapMemoryTest.cpp
//! @brief	ヒープメモリに関するAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.21

#include "pch.h"
#include "HeapMemory.h"
#include "TString.h"
#include "Thread.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (HeapMemoryTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("HeapMemoryTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("HeapMemoryTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("HeapMemoryTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("HeapMemoryTest method cleanup.\n");
		}

		TEST_METHOD (HeapMemoryBasicTest)
		{
			alt::HeapMemory memory;
			Assert::IsTrue(memory.Create ());

			for (char n = 'A'; n < 'Z'; n++)
			{
				for (int i = 0; i < 100; i++)
				{
					auto mem = memory.Allocate (100);
					Assert::IsNotNull (mem);
					FillMemory (mem, 100, n);
					Assert::IsTrue(memory.Free (mem));
				}
				Assert::IsTrue(memory.Compact ());
			}

			Assert::IsTrue(memory.Close ());
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

			for (int i = 0; i < 10; i++)
			{
				thread[i].Close ();
			}

			delete[] thread;
		}

		MemoryCreator newCreator = [](int size)->LPBYTE { return new BYTE[size]; };
		MemoryDeleter newDeleter = [](LPBYTE lpbyData)->void { delete lpbyData; };

		TEST_METHOD (ByteMemTest_New____16)
		{
			ByteMemTest (16, counter, newCreator, newDeleter);
		}

		TEST_METHOD (ByteMemTest_New____64)
		{
			ByteMemTest (64, counter, newCreator, newDeleter);
		}

		TEST_METHOD (ByteMemTest_New___256)
		{
			ByteMemTest (256, counter, newCreator, newDeleter);
		}

		TEST_METHOD (ByteMemTest_New__1024)
		{
			ByteMemTest (1024, counter, newCreator, newDeleter);
		}

		TEST_METHOD (ByteMemTest_New__4096)
		{
			ByteMemTest (4096, counter, newCreator, newDeleter);
		}

		TEST_METHOD (ByteMemTest_New_16384)
		{
			ByteMemTest (16384, counter, newCreator, newDeleter);
		}

		TEST_METHOD (ByteMemTest_New_512k)
		{
			ByteMemTest (1024 * 512, counter, newCreator, newDeleter);
		}

		alt::BasicMemory basicMemory;
		MemoryCreator basicCreator = [&](int size)->LPBYTE { return (LPBYTE)basicMemory.Allocate (size,0); };
		MemoryDeleter basicDeleter = [&](LPBYTE lpbyData)->void { basicMemory.Free (lpbyData); };

		TEST_METHOD (ByteMemTest_Basic____16)
		{
			ByteMemTest (16, counter, basicCreator, basicDeleter);
		}

		TEST_METHOD (ByteMemTest_Basic____64)
		{
			ByteMemTest (64, counter, basicCreator, basicDeleter);
		}

		TEST_METHOD (ByteMemTest_Basic___256)
		{
			ByteMemTest (256, counter, basicCreator, basicDeleter);
		}

		TEST_METHOD (ByteMemTest_Basic__1024)
		{
			ByteMemTest (1024, counter, basicCreator, basicDeleter);
		}

		TEST_METHOD (ByteMemTest_Basic__4096)
		{
			ByteMemTest (4096, counter, basicCreator, basicDeleter);
		}

		TEST_METHOD (ByteMemTest_Basic_16384)
		{
			ByteMemTest (16384, counter, basicCreator, basicDeleter);
		}

		TEST_METHOD (ByteMemTest_Basic_512k)
		{
			ByteMemTest (1024 * 512, counter, basicCreator, basicDeleter);
		}

		alt::HeapMemory* heapMemory;
		MemoryCreator heapCreator = [&](int size)->LPBYTE { return (LPBYTE)heapMemory->Allocate (size,0); };
		MemoryDeleter heapDeleter = [&](LPBYTE lpbyData)->void { heapMemory->Free (lpbyData); };

		TEST_METHOD (ByteMemTest_SystemHeap____16)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue (heapMemory->Open ());
			ByteMemTest (16, counter, heapCreator, heapDeleter);
			Assert::IsTrue (heapMemory->Close ());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_SystemHeap____64)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Open());
			ByteMemTest(64, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_SystemHeap___256)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Open());
			ByteMemTest(256, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_SystemHeap__1024)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Open());
			ByteMemTest(1024, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_SystemHeap__4096)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Open());
			ByteMemTest(4096, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_SystemHeap_16384)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Open());
			ByteMemTest(16384, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_SystemHeap_512k)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Open());
			ByteMemTest(1024 * 512, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap____16)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(16, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap____64)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(64, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap___256)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(256, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap__1024)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(1024, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap__4096)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(4096, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap_16384)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(16384, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (ByteMemTest_UserHeap_512k)
		{
			heapMemory = new alt::HeapMemory();
			Assert::IsTrue(heapMemory->Create());
			ByteMemTest(1024 * 512, counter, heapCreator, heapDeleter);
			Assert::IsTrue(heapMemory->Close());
			delete heapMemory;
		}

		TEST_METHOD (HeapReallocTest)
		{
			alt::TString message;
			alt::HeapMemory memory;
			BOOL ret = memory.Create ();
			Assert::IsTrue (ret, _T ("HeapMemory::Create() failed.\n"));

			int allocSize = 1024;
			message.Format (_T ("now allocSize is %d\n"), allocSize);
			Logger::WriteMessage (message.Ctr ());

			char* lpszMemory = static_cast<char*>(memory.Allocate (allocSize));
			FillMemory (lpszMemory, allocSize, 'a');

			for (int i = 0; i < 100; i++)
			{
				allocSize += 1024;
				message.Format (_T ("now allocSize is %d\n"), allocSize);
				Logger::WriteMessage (message.Ctr ());

				lpszMemory = static_cast<char*>(memory.Reallocate (lpszMemory, allocSize));
				FillMemory (lpszMemory, allocSize, ('a' + i));

				Assert::AreEqual<char> (('a' + i), lpszMemory[allocSize - 1], _T ("character mismatched.\n"));
			}

			ret = memory.Free (lpszMemory);
			Assert::IsTrue (ret, _T ("HeapMemory::Free() failed.\n"));

			ret = memory.Close();
			Assert::IsTrue (ret, _T ("HeapMemory::Close() failed.\n"));
		}
	};
}