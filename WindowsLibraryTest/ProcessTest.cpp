//! @file	ProcessTest.cpp
//! @brief	プロセスに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.31

#include "pch.h"
#include "Process.h"
#include "TString.h"
#include "FileUtility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (ProcessTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("ProcessTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("ProcessTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("ProcessTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("ProcessTest method cleanup.\n");
		}

		TEST_METHOD (ConsoleProcessTest1)
		{
			alt::Process process;
			TCHAR tszCmdline[MAX_PATH]{ _T ("PING www.yahoo.co.jp") };
			BOOL ret = process.Create (nullptr, tszCmdline);
			Assert::IsTrue (ret, _T ("Process::Create() failed.\n"));

			CHAR szBuf[1024];
			DWORD dwSize;

			auto ReadBuffer = [&]()
			{
				ZeroMemory (szBuf, sizeof (szBuf));
				dwSize = process.Peek (szBuf, sizeof (szBuf) - 1);
				if (dwSize > 0)
				{
					dwSize = process.Read (szBuf, dwSize);
					Logger::WriteMessage (szBuf);
				}
			};

			for (;;)
			{
				ReadBuffer ();

				DWORD dwRet = process.Wait (500);
				if (dwRet == WAIT_TIMEOUT)
				{
					Logger::WriteMessage (_T ("WAIT_TIMEOUT & continue.\n"));
					continue;
				}
				if (dwRet == WAIT_OBJECT_0)
				{
					ReadBuffer ();
					Logger::WriteMessage (_T ("WAIT_OBJECT_0 & break.\n"));
					break;
				}
			}

			process.Wait ();
			DWORD dwExitCode = process.GetExitCode ();
			Assert::AreEqual<DWORD> (0, dwExitCode, _T ("Process::GetExitCode() not return 0.\n"));
		}

		TEST_METHOD (ConsoleProcessTest2)
		{
			alt::Process process;
			TCHAR tszCmdline[MAX_PATH]{ _T ("PING -t 127.0.0.1") };
			::AllocConsole ();
			process.SetTitle (_T ("ConsoleProcessTest2"));
			//process.SetConsoleInfo (0, 0, 120, 25, 80, 25);
			BOOL ret = process.Create (nullptr, tszCmdline);
			Assert::IsTrue (ret, _T ("Process::Create() failed.\n"));

			CHAR szBuf[1024];
			DWORD dwSize;

			auto ReadBuffer = [&]()
			{
				ZeroMemory (szBuf, sizeof (szBuf));
				dwSize = process.Peek (szBuf, sizeof (szBuf) - 1);
				if (dwSize > 0)
				{
					dwSize = process.Read (szBuf, dwSize);
					Logger::WriteMessage (szBuf);
				}
			};

			for (int retry = 0; retry < 10;)
			{
				ReadBuffer ();

				DWORD dwRet = process.Wait (500);
				if (dwRet == WAIT_TIMEOUT)
				{
					Logger::WriteMessage (_T ("WAIT_TIMEOUT & continue.\n"));
					retry++;
					continue;
				}
				if (dwRet == WAIT_OBJECT_0)
				{
					ReadBuffer ();
					Logger::WriteMessage (_T ("WAIT_OBJECT_0 & break.\n"));
					break;
				}
			}

			process.Wait (5000);
			Assert::IsTrue (process.Terminate (), _T ("Process::Terminate() failed.\n"));
			process.Wait (INFINITE);
			DWORD dwExitCode = process.GetExitCode ();
			Assert::AreEqual<DWORD> (0, dwExitCode, _T ("Process::GetExitCode() not return 0.\n"));
		}

		TEST_METHOD (ConsoleProcessTest5)
		{
			alt::Process process;
			TCHAR tszCmdline[MAX_PATH]{ _T ("CMD /K") };
			process.SetTitle (_T ("ConsoleProcessTest5"));
			//process.SetConsoleInfo (0, 0, 80, 25, 80, 25);
			BOOL ret = process.Create (NULL, tszCmdline);
			Assert::IsTrue (ret, _T ("Process::Create() failed.\n"));

			CHAR szBuf[1024];
			DWORD dwSize;

			auto ReadBuffer = [&]()
			{
				ZeroMemory (szBuf, sizeof (szBuf));
				dwSize = process.Peek (szBuf, sizeof (szBuf) - 1);
				if (dwSize > 0)
				{
					dwSize = process.Read (szBuf, dwSize);
					Logger::WriteMessage (szBuf);
				}
			};

			sprintf_s (szBuf, "DIR C:\\Windows\r\n");
			dwSize = (DWORD)strlen (szBuf);

			DWORD dwWrite = process.Write (szBuf, dwSize);
			Assert::AreEqual<DWORD> (dwWrite, dwSize, _T ("Process::Write() return invalid size.\n"));

			for (int retry = 0; retry < 10;)
			{
				ReadBuffer ();

				DWORD dwRet = process.Wait (100);
				if (dwRet == WAIT_TIMEOUT)
				{
					retry++;
					continue;
				}
				if (dwRet == WAIT_OBJECT_0)
				{
					ReadBuffer ();
					break;
				}
			}

			process.Wait (5000);
			Assert::IsTrue (process.Terminate (), _T ("Process::Terminate() failed.\n"));
			process.Wait (INFINITE);
			DWORD dwExitCode = process.GetExitCode ();
			Assert::AreEqual<DWORD> (0, dwExitCode, _T ("Process::GetExitCode() not return 0.\n"));
		}

		TEST_METHOD (WindowProcessTest1)
		{
			alt::Process process;
			TCHAR tszCmdline[MAX_PATH]{ _T ("NotePad.exe ..\\..\\README.MD") };
			process.SetWindowInfo (2, 2, 320, 240); // NotePadが座標情報を無視してる。
			BOOL ret = process.Create (NULL, tszCmdline);
			Assert::IsTrue (ret, _T ("Process::Create() failed.\n"));

			process.Wait (5000);
			Assert::IsTrue (process.Terminate (), _T ("Process::Terminate() failed.\n"));
			process.Wait (INFINITE);
			DWORD dwExitCode = process.GetExitCode ();
			Assert::AreEqual<DWORD> (0, dwExitCode, _T ("Process::GetExitCode() not return 0.\n"));
		}

		TEST_METHOD (WindowProcessTest2)
		{
			alt::Process process;
			TCHAR tszCmdline[MAX_PATH]{ _T ("NotePad.exe ..\\..\\README.MD") };
			process.SetWindowInfo (2, 2, 320, 240, SW_MAXIMIZE); // NotePadが座標情報を無視してる。
			BOOL ret = process.Create (NULL, tszCmdline);
			Assert::IsTrue (ret, _T ("Process::Create() failed.\n"));

			process.Wait (5000);
			Assert::IsTrue (process.Terminate (), _T ("Process::Terminate() failed.\n"));
			process.Wait (INFINITE);
			DWORD dwExitCode = process.GetExitCode ();
			Assert::AreEqual<DWORD> (0, dwExitCode, _T ("Process::GetExitCode() not return 0.\n"));
		}
	};
}