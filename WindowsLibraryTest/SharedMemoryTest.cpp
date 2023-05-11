//! @file	SharedMemoryTest.cpp
//! @brief	共有メモリに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.2

#include "pch.h"
#include "SharedMemory.h"
#include "Thread.h"
#include "Mutex.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (SharedMemoryTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("SharedMemoryTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("SharedMemoryTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("SharedMemoryTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("SharedMemoryTest method cleanup.\n");
		}

		TEST_METHOD (SharedMemoryTest1)
		{
			alt::Thread thread1;
			alt::Thread thread2;
			alt::Mutex mutex;
			alt::SharedMemory shmem;

			auto threadFunc1 = [](LPVOID lpvParam)->DWORD
			{
				alt::SharedMemory shmem;
				alt::Mutex mutex;
				Assert::IsTrue (shmem.Open (_T ("SHMEM")), _T("SharedMemory::Open() failed.\n"));
				Assert::IsTrue (mutex.Open (_T ("MUTEX")), _T("Mutex::Open() failed.\n"));

				LPVOID lpvMem = shmem.Mapped ();
				Assert::IsNotNull (lpvMem, _T("SharedMemory::Mapped() failed.\n"));

				for (int i = 0; i < 1000; i++)
				{
					mutex.Wait ();
					FillMemory (lpvMem, 10240, '1');
					Logger::WriteMessage ("1");
					mutex.Release ();
					SwitchToThread ();
				}

				Assert::IsTrue (shmem.Unmapped (), _T("SharedMemory::Unmapped() failed.\n"));

				return 0;
			};

			auto threadFunc2 = [](LPVOID lpvParam)->DWORD
			{
				alt::SharedMemory shmem;
				alt::Mutex mutex;
				Assert::IsTrue (shmem.Open (_T ("SHMEM")), _T("SharedMemory::Open() failed.\n"));
				Assert::IsTrue (mutex.Open (_T ("MUTEX")), _T("Mutex::Open() failed.\n"));

				LPVOID lpvMem = shmem.Mapped ();
				Assert::IsNotNull (lpvMem, _T("SharedMemory::Mapped() failed.\n"));

				for (int i = 0; i < 1000; i++)
				{
					mutex.Wait ();
					FillMemory (lpvMem, 10240, '2');
					Logger::WriteMessage ("2");
					mutex.Release ();
					SwitchToThread ();
				}

				Assert::IsTrue (shmem.Unmapped (), _T("SharedMemory::Unmapped() failed.\n"));

				return 0;
			};

			Assert::IsTrue (mutex.Create (_T ("MUTEX")), _T("Mutex.Create() failed.\n"));
			Assert::IsTrue (shmem.Create (_T ("SHMEM"), 10240), _T("SharedMemory::Create() failed.\n"));

			Assert::IsTrue (thread1.Create (threadFunc1), _T("Thread::Create() failed.\n"));
			Assert::IsTrue (thread2.Create (threadFunc2), _T("Thread::Create() failed.\n"));

			HANDLE hThreads[2] = { NULL, NULL };
			hThreads[0] = thread1.GetHandle ();
			hThreads[1] = thread2.GetHandle ();
			WaitForMultipleObjects (2, hThreads, TRUE, INFINITE);
		}
	};
}