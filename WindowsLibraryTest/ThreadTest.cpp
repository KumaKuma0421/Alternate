//! @file	ThreadTest.cpp
//! @brief	スレッドに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "Thread.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (ThreadTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("ThreadTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("ThreadTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("ThreadTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("ThreadTest method cleanup.\n");
		}

		class Callback : public alt::skeleton::ICallback
		{
		public:
			DWORD Invoke (LPVOID lpvParam)
			{
				for (int i = 0; i < 10; i++)
				{
					Logger::WriteMessage (_T ("Invoke() working...\n"));
					Sleep (20);
				}

				Logger::WriteMessage (_T ("Invoke() stopped.\n"));
				return 0;
			}
		};

		class MyThread : public alt::Thread, public alt::skeleton::ICallback
		{
		public:
			MyThread () { _callback = this; };
			~MyThread () {};

			DWORD Invoke (LPVOID lpvParam)
			{
				for (int i = 0; i < 10; i++)
				{
					Logger::WriteMessage (_T ("Invoke() working...\n"));
					Sleep (20);
				}

				Logger::WriteMessage (_T ("Invoke() stopped.\n"));
				return 0;
			}
		};

		TEST_METHOD (ThreadTest1)
		{
			static int threadCounter = 0;

			Callback* pCallback = new Callback ();
			alt::Thread* pThread = new alt::Thread (pCallback);
			BOOL ret1 = pThread->Create ();
			Assert::IsTrue (ret1, _T("Thread::Create() failed.\n"));

			for (int i = 0; i < 10; i++)
			{
				threadCounter++;
				Logger::WriteMessage (_T ("ThreadTest1() working...\n"));
				Sleep (30);
			}

			Logger::WriteMessage (_T ("ThreadTest1() stopped.\n"));

			DWORD dwReason = pThread->Wait ();
			Assert::AreEqual (WAIT_OBJECT_0, dwReason, _T("Thread::Wait() not return WAIT_OBJECT_0.\n"));
			Assert::AreEqual (10, threadCounter, _T("Thread::Wait() invalid timing.\n"));
			
			delete pThread;
			delete pCallback;
		}

		TEST_METHOD (ThreadTest2)
		{
			static int threadCounter = 0;

			auto pWorker = new MyThread ();
			BOOL ret1 = pWorker->Create ();
			Assert::IsTrue (ret1, _T("Thread::Create() failed.\n"));

			for (int i = 0; i < 10; i++)
			{
				threadCounter++;
				Logger::WriteMessage (_T ("."));
				Sleep (30);
			}

			Logger::WriteMessage (_T ("ThreadTest2() stopped.\n"));

			DWORD dwReason = pWorker->Wait ();
			Assert::AreEqual (WAIT_OBJECT_0, dwReason, _T("Thread::Wait() not return WAIT_OBJECT_0.\n"));
			Assert::AreEqual (10, threadCounter, _T("Thread::Wait() invalid timing.\n"));

			delete pWorker;
		}

		TEST_METHOD (ThreadTest3)
		{
			alt::Thread* thread = new alt::Thread ();
			static int threadCounter = 0;
			auto function = [](LPVOID lpvParam)->DWORD
			{
				int* waitTime = (int*)lpvParam;

				for (int i = 0; i < 10; i++)
				{
					threadCounter++;
					Logger::WriteMessage (_T ("."));
					Sleep (*waitTime);
				}

				Logger::WriteMessage (_T ("ThreadTest3() stopped.\n"));

				return true;
			};

			LONGLONG waitTime = 30;
			thread->Create (function, &waitTime);
			thread->Wait ();

			Assert::AreEqual (10, threadCounter, _T("Thread::Wait() invalid timing.\n"));

			delete thread;
		}

#define size 20
#define loop 20
		TEST_METHOD (ThreadTest4)
		{
			static int threadCounter[size];
			for (int i = 0; i < size; i++)
			{
				threadCounter[i] = 0;
			}

			auto function = [](LPVOID lpvParam)->DWORD
			{
				int* index = (int*)lpvParam;
				TCHAR tszMsg[128];

				for (int i = 0; i < loop; i++)
				{
					threadCounter[*index]++;
					wsprintf (tszMsg, _T ("thread(%02d) count(%02d)\n"), *index, i);
					Logger::WriteMessage (tszMsg);
					Sleep (10);
				}

				Logger::WriteMessage (_T ("ThreadTest4() stopped.\n"));

				return *index;
			};

			BOOL ret;
			HANDLE hThreads[size];
			int param[size];

			alt::Thread* thread = new alt::Thread[size];
			for (int i = 0; i < size; i++)
			{
				param[i] = i;
				ret = thread[i].Create (function, &param[i], CREATE_SUSPENDED);
				Assert::IsTrue (ret, _T("Thread::Create() failed.\n"));
				hThreads[i] = thread[i].GetHandle ();
			}

			for (int i = 0; i < size; i++)
			{
				int divide = i % 7;
				switch (divide)
				{
					case 0:
						thread[i].SetPriority (alt::ThreadPriority::Critical);
						break;
					case 1:
						thread[i].SetPriority (alt::ThreadPriority::Highest);
						break;
					case 2:
						thread[i].SetPriority (alt::ThreadPriority::AboveNormal);
						break;
					case 3:
						thread[i].SetPriority (alt::ThreadPriority::Normal);
						break;
					case 4:
						thread[i].SetPriority (alt::ThreadPriority::BelowNormal);
						break;
					case 5:
						thread[i].SetPriority (alt::ThreadPriority::Lowest);
						break;
					case 6:
						thread[i].SetPriority (alt::ThreadPriority::Idle);
						break;
				}
				thread[i].Resume ();
			}

			WaitForMultipleObjects (size, hThreads, TRUE, INFINITE);

			Logger::WriteMessage (_T ("\nThreadTest4() stopped.\n"));

			for (int i = 0; i < size; i++)
			{
				Assert::AreEqual (loop, threadCounter[i], _T("Thread running interrupted.\n"));
			}

			delete[] thread;
		}
	};
}