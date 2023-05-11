//! @file	EventTest.cpp
//! @brief	イベントに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "Event.h"
#include "Thread.h"
#include "TString.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (EventTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("EventTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("EventTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("EventTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("EventTest method cleanup.\n");
		}

		TEST_METHOD (AutoEventTest)
		{
			auto EventFunction = [](LPVOID lpvParam)->DWORD
			{
				BOOL ret;
				alt::Event event;
				alt::Thread* thread = (alt::Thread*)lpvParam;

				ret = event.Create (_T ("AutoEventTest"), FALSE, TRUE); // 自動リセットイベント

				for (int i = 0; i < 10; i++)
				{
					ret = event.Pulse ();
					Assert::IsTrue (ret, _T ("Event::PulseEvent() failed.\n"));
					Logger::WriteMessage (_T ("PulseFunction() Event pulsed.\n"));
					Sleep (100);
				}

				return 0;
			};

			auto RecvFunction = [](LPVOID lpvParam)->DWORD
			{
				alt::Event event;
				alt::Thread* thread = (alt::Thread*)lpvParam;

				BOOL ret = event.Open (_T ("AutoEventTest"));

				for (int i = 0; i < 10; i++)
				{
					ret = event.Wait (10000); // 10秒
					if (ret == WAIT_ABANDONED_0 || ret == WAIT_FAILED || ret == WAIT_TIMEOUT) break;
					if (ret == WAIT_OBJECT_0)
					{
						alt::TString message;
						message.Format (_T ("Event::Wait() wait breaked. ThreadID=%d\n"),::GetThreadId(thread->GetHandle()));
						Logger::WriteMessage (message.Ctr());
					}
				}

				return 0;
			};

			alt::Thread EventThread;
			alt::Thread RecvThread1;
			alt::Thread RecvThread2;
			alt::Thread RecvThread3;

			Assert::IsTrue (EventThread.Create (EventFunction, &EventThread), _T ("Failed to create PulseThread.\n"));
			Assert::IsTrue (RecvThread1.Create (RecvFunction, &RecvThread1), _T ("Failed to create RecvThread1.\n"));
			Assert::IsTrue (RecvThread2.Create (RecvFunction, &RecvThread2), _T ("Failed to create RecvThread2.\n"));
			Assert::IsTrue (RecvThread3.Create (RecvFunction, &RecvThread3), _T ("Failed to create RecvThread3.\n"));

			EventThread.Wait ();
		}

		TEST_METHOD (ManualEventTest)
		{
			auto EventFunction = [](LPVOID lpvParam)->DWORD
			{
				BOOL ret;
				alt::Event event;
				alt::Thread* thread = (alt::Thread*)lpvParam;

				ret = event.Create (_T ("ManualEventTest"), TRUE, FALSE); // 手動リセットイベント

				for (int i = 0; i < 10; i++)
				{
					ret = event.Set ();
					Logger::WriteMessage (_T ("Event::Set()\n"));
					Assert::IsTrue (ret, _T ("Event::SetEvent() failed.\n"));
					
					Sleep (100);

					ret = event.Reset ();
					Logger::WriteMessage (_T ("Event::Reset()\n"));
					Assert::IsTrue (ret, _T ("Event::ResetEvent() failed.\n"));

					Sleep (100);
				}

				return 0;
			};

			auto RecvFunction = [](LPVOID lpvParam)->DWORD
			{
				alt::Event event;
				alt::Thread* thread = (alt::Thread*)lpvParam;

				BOOL ret = event.Open (_T ("ManualEventTest"));

				for (;;)
				{
					ret = event.Wait (1000);
					if (ret == WAIT_ABANDONED_0 || ret == WAIT_FAILED) break;

					if (ret == WAIT_OBJECT_0)
					{
						alt::TString message;
						message.Format (_T ("Event::Wait() wait breaked. ThreadID=%d\n"),::GetThreadId(thread->GetHandle()));
						Logger::WriteMessage (message.Ctr());
					}
					if (ret == WAIT_TIMEOUT)
					{
						alt::TString message;
						message.Format (_T ("Event::Wait() wait timeout. ThreadID=%d\n"),::GetThreadId(thread->GetHandle()));
						Logger::WriteMessage (message.Ctr());
					}

					Sleep (200);
				}

				return 0;
			};

			alt::Thread EventThread;
			alt::Thread RecvThread1;
			alt::Thread RecvThread2;
			alt::Thread RecvThread3;

			Assert::IsTrue (EventThread.Create (EventFunction, &EventThread), _T ("Failed to create EventThread.\n"));
			Assert::IsTrue (RecvThread1.Create (RecvFunction, &RecvThread1), _T ("Failed to create RecvThread1.\n"));
			Assert::IsTrue (RecvThread2.Create (RecvFunction, &RecvThread2), _T ("Failed to create RecvThread2.\n"));
			Assert::IsTrue (RecvThread3.Create (RecvFunction, &RecvThread3), _T ("Failed to create RecvThread3.\n"));

			EventThread.Wait ();
		}
	};
}