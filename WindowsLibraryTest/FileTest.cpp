//! @file	FileTest.cpp
//! @brief	ファイルIOに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21
//! 
#include "pch.h"
#include "File.h"
#include "FileUtility.h"
#include "Thread.h"
#include "Utility.h"
#include "Mutex.h"
#include "Event.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define TEST_FILE _T(".\\CreateTest.txt")
#define TEST_MUTEX _T("TEST_MUTEX")
#define TEST_EVENT _T("TEST_EVENT")

namespace WindowsLibraryTest
{
	TEST_CLASS (FileTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("FileTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("FileTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("FileTest method initialize.\n");
			alt::FileUtility::Delete (TEST_FILE);
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("FileTest method cleanup.\n");
			alt::FileUtility::Delete (TEST_FILE);
		}

		TEST_METHOD (CreateFileTest)
		{
			BOOL ret;
			alt::File* pFile = new alt::File ();

			ret = pFile->Create (TEST_FILE, GENERIC_ALL, FILE_SHARE_READ, CREATE_NEW);
			Assert::IsTrue (ret, _T ("File::Create() failed.\n"));
			Assert::IsTrue (alt::FileUtility::IsExist (TEST_FILE), _T ("File not found.\n"));

			LPCTSTR lpctszString1 = _T ("TEST TEST TEST");
			DWORD dwLen = pFile->Write (lpctszString1, static_cast<DWORD>(_tcslen (lpctszString1) * sizeof (TCHAR)));
			Assert::AreEqual<DWORD> (static_cast<DWORD>(_tcslen (lpctszString1) * sizeof (TCHAR)), dwLen, _T ("File::Write() failed.\n"));

			ret = pFile->Flush ();
			Assert::IsTrue (ret, _T ("File::Flush() failed.\n"));

			ret = pFile->SetPointer (FILE_BEGIN);
			Assert::IsTrue (ret, _T ("File::SetPointer(FILE_BEGIN) failed.\n"));

			TCHAR tszBuf[256] = { 0 };
			DWORD dwSize = pFile->Read (tszBuf, sizeof (tszBuf));
			Assert::IsTrue (ret, _T ("File::Read() failed.\n"));
			tszBuf[dwSize / sizeof (TCHAR)] = '\0';
			Assert::AreEqual<int> (0, _tcscmp (lpctszString1, tszBuf), _T("File::Read() invalid contents.\n"));

			ret = pFile->Close ();
			Assert::IsTrue (ret, _T("File::Close() failed.\n"));

			delete pFile;

			alt::FileUtility::Delete (TEST_FILE);
		}

		TEST_METHOD (OpenFileTest)
		{
			BOOL ret;
			alt::File* testFile = new alt::File ();
			ret = testFile->Create (TEST_FILE, GENERIC_ALL, FILE_SHARE_READ, CREATE_NEW);
			Assert::IsTrue (ret, _T("File::Create() failed.\n"));

			BYTE byBuffer[1000];
			FillMemory (byBuffer, 1000, '*');
			DWORD dwResult = testFile->Write (byBuffer, 1000);
			Assert::AreEqual<DWORD> (1000, dwResult, _T ("File::Write() invalid write size.\n"));
			ret = testFile->Flush ();
			Assert::AreEqual<BOOL> (TRUE, ret, _T ("File::Flush() failed.\n"));
			ret = testFile->Close ();
			Assert::AreEqual<BOOL> (TRUE, ret, _T ("File::Close() failed.\n"));
			delete testFile;

			alt::File file;
			Assert::IsFalse (file.IsOpen (), _T("File::IsOpen() failed.\n"));

			ret = file.Create (TEST_FILE, GENERIC_ALL, FILE_SHARE_READ, OPEN_EXISTING);
			Assert::IsTrue (ret, _T ("File::Create() failed.\n"));

			Assert::IsTrue (file.IsOpen (), _T("File::IsOpen() failed.\n"));

			LONGLONG fileSize = file.GetSize ();
			Assert::AreEqual<SIZE_T> (1000, fileSize, _T ("File::GetSize() invalid file size.\n"));
			
			DWORD dwFileType = file.GetType ();
			Assert::AreEqual<DWORD> (FILE_TYPE_DISK, dwFileType, _T ("File::GetType() invalid file type.\n"));

			ret = file.SetPointer (FILE_BEGIN);
			Assert::IsTrue (ret, _T ("File::SetPointer(FILE_BEGIN) failed.\n"));

			ret = file.SetPointer (FILE_END);
			Assert::IsTrue (ret, _T ("File::SetPointer(FILE_END) failed."));

			LARGE_INTEGER liDistance = { 0 };
			liDistance.QuadPart = -500;
			ret = file.SetPointer (FILE_CURRENT, liDistance);
			Assert::IsTrue (ret, _T ("File::SetPointer(FILE_CURRENT, -500) failed.\n"));

			ret = file.SetEOF ();
			Assert::IsTrue (ret, _T ("File::SetEOF() failed.\n"));
			
			fileSize = file.GetSize ();
			Assert::AreEqual<SIZE_T> (500, fileSize, _T("File::GetSize() invalid file size.\n"));

			alt::FileUtility::Delete (TEST_FILE);
		}

		TEST_METHOD (SharedFileTest)
		{
			const int loopCount = 20;

			auto writeFunction = [](LPVOID lpvParam)->DWORD
			{
				int loopCount = *(int*)lpvParam;
				alt::File file;
				alt::Mutex mutex;
				alt::Event event;
				Assert::IsTrue(event.Create (TEST_EVENT, TRUE, FALSE), _T("Event::Create() failed.\n"));
				Assert::IsTrue(mutex.Create (TEST_MUTEX), _T ("Mutex::Create() failed.\n"));
				Assert::IsTrue (file.Create (TEST_FILE, GENERIC_WRITE, FILE_SHARE_READ, CREATE_ALWAYS), _T("File::Create() failed.\n"));

				Sleep (3000);
				Assert::IsTrue(event.Set (), _T("Event::Set() failed.\n"));

				for (int i = 0; i < loopCount; i++)
				{
					Assert::AreEqual<DWORD> (WAIT_OBJECT_0, mutex.Wait (), _T("Mutex::Wait() failed.\n"));
					
					alt::TString message;
					message.Format (_T ("%02d %s"), i, alt::Utility::GetFormatTime (alt::Utility::GetLocalTime ()).Ctr());
					DWORD dwRet = file.Write (message.Ctr (), message.Byte ());
					Assert::AreEqual<DWORD> (message.Byte (), dwRet, _T("File::Write() return value is invalid.\n"));
					message.Format (_T ("W:%02d->\n"), dwRet);
					Logger::WriteMessage (message.Ctr());
					
					Assert::IsTrue (mutex.Release (), _T("Mutex::Release() failed.\n"));
					Sleep(0);
				}

				return 0;
			};

			auto readFunction = [](LPVOID lpvParam)->DWORD
			{
				int loopCount = *(int*)lpvParam;
				alt::File file;
				alt::Mutex mutex;
				alt::Event event;
				Assert::IsTrue (event.Open (TEST_EVENT), _T ("Event::Open() failed.\n"));
				Assert::IsTrue (mutex.Open (TEST_MUTEX), _T ("Mutex::Open() failed.\n"));
				Assert::IsTrue (file.Create (TEST_FILE, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, OPEN_EXISTING), _T("File::Create() failed.\n"));
				
				Assert::AreEqual<DWORD>(WAIT_OBJECT_0, event.Wait (), _T("Event::Wait() failed.\n"));

				for (int i = 0; i < loopCount; i++)
				{
					Assert::AreEqual<DWORD> (WAIT_OBJECT_0, mutex.Wait (), _T("Mutex::Wait() failed.\n"));
					
					alt::TString message(52); // 書き込みサイズ
					DWORD dwRet = file.Read (message.Ptr (), 52);
					alt::TString out;
					out.Format (_T ("R:%02d->%s\n"), dwRet, message.Ctr ());
					Logger::WriteMessage (out.Ctr());
					
					Assert::IsTrue (mutex.Release (), _T("Mutex::Release() failed.\n"));
					Sleep(0);
				}

				return 0;
			};

			alt::Thread writeThread;
			alt::Thread readThread;

			Assert::IsTrue(writeThread.Create (writeFunction, (LPVOID) & loopCount));
			Sleep (1000);
			Assert::IsTrue(readThread.Create (readFunction, (LPVOID) & loopCount));

			Assert::AreEqual<DWORD>(WAIT_OBJECT_0, writeThread.Wait (), _T("writeThread.Wait() failed."));
			Assert::AreEqual<DWORD>(WAIT_OBJECT_0, readThread.Wait (), _T("readThread.Wait() failed."));
		}
	};
}