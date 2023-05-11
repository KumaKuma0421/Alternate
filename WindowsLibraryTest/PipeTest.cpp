//! @file	PipeTest.cpp
//! @brief	パイプに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.6

#include "pch.h"
#include "Pipe.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (PipeTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("PipeTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("PipeTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("PipeTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("PipeTest method cleanup.\n");
		}

		TEST_METHOD (PipeTest1)
		{
			alt::Pipe pipe;
			BOOL ret;

			ret = pipe.Create ();
			Assert::IsTrue (ret, _T("Pipe::Create() failed.\n"));

			CHAR szBuf[256];
			FillMemory (szBuf, sizeof (szBuf), 'A');

			ret = pipe.Write (szBuf, sizeof (szBuf));
			Assert::IsTrue (ret, _T("Pipe::Write() failed.\n"));

			ZeroMemory (szBuf, sizeof (szBuf));
			Assert::AreEqual<DWORD> (sizeof (szBuf), pipe.Peek (szBuf, sizeof (szBuf)), _T("Pipe::Peek() invalid size.\n"));
			Assert::AreEqual<DWORD> (sizeof (szBuf), pipe.Read (szBuf, sizeof (szBuf)), _T("Pipe::Read() invalid size.\n"));
			Logger::WriteMessage (szBuf);

			Assert::IsTrue (pipe.Close (), _T("Pipe::Close() failed.\n"));
		}

		TEST_METHOD (PipeTest2)
		{
			alt::Pipe pipe;
			BOOL ret;

			ret = pipe.Create ();
			Assert::IsTrue (ret, _T("Pipe::Create() failed.\n"));

			CHAR szBuf[256];
			ZeroMemory (szBuf, sizeof (szBuf));

			Assert::AreEqual<DWORD> (0, pipe.Peek (szBuf, sizeof (szBuf)), _T("Pipe::Peek() invalid size.\n"));

			ret = pipe.Close ();
			Assert::IsTrue (ret, _T("Pipe::Close() failed.\n"));
		}
	};
}