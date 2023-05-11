//! @file	ConsoleTest.cpp
//! @brief	コンソールに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "Console.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (ConsoleTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("ConsoleTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("ConsoleTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("ConsoleTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("ConsoleTest method cleanup.\n");
		}

		TEST_METHOD (ConsoleTest1)
		{
			alt::Console console;
			BOOL ret;

			ret = console.Create ();
			//Assert::IsTrue(ret, _T("Console::Create() failed.\n"));

			ret = console.Init ();
			//Assert::IsTrue(ret, _T("Console::Init() failed.\n"));

			ret = console.SetColor (alt::ForeColor::Red, alt::BackColor::Black);
			DWORD dwRet = console.Write (_T ("TEST TEST TEST"));
			//Assert::AreEqual<DWORD>(14, dwRet, _T("Console::Write() failed.\n"));
		}

		TEST_METHOD (ConsoleOutTest1)
		{
			alt::Console con;

			for (int i = 0; i < 100; i++)
			{
				con.Write (_T ("this is a test.\n"));
			}
		}

		TEST_METHOD (ConsoleOutTest2)
		{
			alt::Console con;

			for (int i = 0; i < 100; i++)
			{
				con.Format (_T ("Test No.%d"), i);
			}
		}
	};
}