//! @file	LogTest.cpp
//! @brief	Fileをベースとしたログに関する機能を集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.26

#include "pch.h"
#include "Log.h"
#include "Thread.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	alt::Log __threadLogFile;

	auto loopAction = [](int index, int loopMax)
	{
		alt::TString message;
		for (int i = 0; i < loopMax; i++)
		{
			message << _T ("ThreadFunction_") << index << _T ("_") << i << _T ("_★★★★★★★★★★★★★★★★★★★");
			__threadLogFile.Info (message.Ctr ());
			Sleep (0);
			message = _T ("");
		}

		return 0;
	};

	DWORD ThreadFunction1 (LPVOID lpvParam)
	{
		loopAction (1, 1000);
		return 0;
	}

	DWORD ThreadFunction2 (LPVOID lpvParam)
	{
		loopAction (2, 1000);
		return 0;
	}

	DWORD ThreadFunction3 (LPVOID lpvParam)
	{
		loopAction (3, 1000);
		return 0;
	}

	DWORD ThreadFunction4 (LPVOID lpvParam)
	{
		loopAction (4, 1000);
		return 0;
	}

	DWORD ThreadFunction5 (LPVOID lpvParam)
	{
		loopAction (5, 1000);
		return 0;
	}

	DWORD ThreadFunction6 (LPVOID lpvParam)
	{
		loopAction (6, 1000);
		return 0;
	}

	DWORD ThreadFunction7 (LPVOID lpvParam)
	{
		loopAction (7, 1000);
		return 0;
	}

	DWORD ThreadFunction8 (LPVOID lpvParam)
	{
		loopAction (8, 1000);
		return 0;
	}

	TEST_CLASS (LogTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("LogTest class initialize.\n");
			auto response = alt::FileUtility::Find (_T(".\\"), _T ("TestLog*"));
			for (int i = 0; i < response.Size (); i++)
			{
				alt::TString target;
				target.Format (_T ("%s\\%s"),
							   response[i]->GetFolderName().Ctr(),
							   response[i]->GetFileName().Ctr());
				alt::FileUtility::Delete (target.Ctr ());
			}
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("LogTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("LogTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("LogTest method cleanup.\n");
		}

		TEST_METHOD (LogFileTest1)
		{
			alt::Log logger (alt::LogLevel::Trace, _T (".\\TestLogA"), 10, 1024 * 1024);

			for (int i = 0; i < 900; i++)
			{
				logger.Debug (_T ("DebugLog -------------------------------------------"));
				logger.Trace (_T ("TraceLog -------------------------------------------"));
				logger.Info (_T ("InfoLog --------------------------------------------"));
				logger.Warn (_T ("WarnLog --------------------------------------------"));
				logger.Error (_T ("ErrorLog -------------------------------------------"));
				logger.Fatal (_T ("FatalLog -------------------------------------------"));
			}
		}

		TEST_METHOD (LogFileTest2)
		{
			alt::Log logger (alt::LogLevel::Debug, _T (".\\TestLogB"), 10, 1024 * 1024);

			for (int i = 0; i < 1000; i++)
			{
				logger.Debug (_T ("DebugLog --------------------------------------------------------------------"));
				logger.Trace (_T ("TraceLog --------------------------------------------------------------------"));
				logger.Info (_T ("InfoLog ---------------------------------------------------------------------"));
				logger.Warn (_T ("WarnLog ---------------------------------------------------------------------"));
				logger.Error (_T ("ErrorLog --------------------------------------------------------------------"));
				logger.Fatal (_T ("FatalLog --------------------------------------------------------------------"));
			}
		}

		TEST_METHOD (LogFileTest3)
		{
			alt::Log logger (alt::LogLevel::Info, _T (".\\TestLogC"), 10, 1024 * 1024);

			for (int i = 0; i < 1000; i++)
			{
				logger.Debug (_T ("DebugLog --------------------------------------------------------------------"));
				logger.Trace (_T ("TraceLog --------------------------------------------------------------------"));
				logger.Info (_T ("InfoLog ---------------------------------------------------------------------"));
				logger.Warn (_T ("WarnLog ---------------------------------------------------------------------"));
				logger.Error (_T ("ErrorLog --------------------------------------------------------------------"));
				logger.Fatal (_T ("FatalLog --------------------------------------------------------------------"));
			}
		}

		TEST_METHOD (LogFileTest4)
		{
			alt::Log logger (alt::LogLevel::Warn, _T (".\\TestLogD"), 10, 1024 * 1024);

			for (int i = 0; i < 1000; i++)
			{
				logger.Debug (_T ("DebugLog --------------------------------------------------------------------"));
				logger.Trace (_T ("TraceLog --------------------------------------------------------------------"));
				logger.Info (_T ("InfoLog ---------------------------------------------------------------------"));
				logger.Warn (_T ("WarnLog ---------------------------------------------------------------------"));
				logger.Error (_T ("ErrorLog --------------------------------------------------------------------"));
				logger.Fatal (_T ("FatalLog --------------------------------------------------------------------"));
			}
		}

		TEST_METHOD (LogFileTest5)
		{
			alt::Log logger (alt::LogLevel::Error, _T (".\\TestLogE"), 10, 1024 * 1024);

			for (int i = 0; i < 1000; i++)
			{
				logger.Debug (_T ("DebugLog --------------------------------------------------------------------"));
				logger.Trace (_T ("TraceLog --------------------------------------------------------------------"));
				logger.Info (_T ("InfoLog ---------------------------------------------------------------------"));
				logger.Warn (_T ("WarnLog ---------------------------------------------------------------------"));
				logger.Error (_T ("ErrorLog --------------------------------------------------------------------"));
				logger.Fatal (_T ("FatalLog --------------------------------------------------------------------"));
			}
		}

		TEST_METHOD (LogFileTest6)
		{
			alt::Log logger (alt::LogLevel::Fatal, _T (".\\TestLogF"), 10, 1024 * 1024);

			for (int i = 0; i < 1000; i++)
			{
				logger.Debug (_T ("DebugLog --------------------------------------------------------------------"));
				logger.Trace (_T ("TraceLog --------------------------------------------------------------------"));
				logger.Info (_T ("InfoLog ---------------------------------------------------------------------"));
				logger.Warn (_T ("WarnLog ---------------------------------------------------------------------"));
				logger.Error (_T ("ErrorLog --------------------------------------------------------------------"));
				logger.Fatal (_T ("FatalLog --------------------------------------------------------------------"));
			}
		}

		TEST_METHOD (LogFileThreadTest)
		{
			__threadLogFile.Init (alt::LogLevel::Trace, _T (".\\TestLog"), 10, 1024 * 1024 * 1);

			alt::Thread thread1;
			alt::Thread thread2;
			alt::Thread thread3;
			alt::Thread thread4;
			alt::Thread thread5;
			alt::Thread thread6;
			alt::Thread thread7;
			alt::Thread thread8;

			thread1.Create (ThreadFunction1);
			thread2.Create (ThreadFunction2);
			thread3.Create (ThreadFunction3);
			thread4.Create (ThreadFunction4);
			thread5.Create (ThreadFunction5);
			thread6.Create (ThreadFunction6);
			thread7.Create (ThreadFunction7);
			thread8.Create (ThreadFunction8);

			thread1.Wait ();
			thread2.Wait ();
			thread3.Wait ();
			thread4.Wait ();
			thread5.Wait ();
			thread6.Wait ();
			thread7.Wait ();
			thread8.Wait ();
		}
	};
}