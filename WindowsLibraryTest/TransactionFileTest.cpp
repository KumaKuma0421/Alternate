//! @file	TransactionFileTest.cpp
//! @brief	トランザクションNTFSに関するAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.27

#include "pch.h"
#include "TransactionFile.h"
#include "FileUtility.h"
#include "Binder.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#define TEST_FOLDER _T("TranTest")
#define TEST_FILE _T("TranTest.txt")
#define TEST_FILE_1 _T("TranTest1.bin")
#define TEST_FILE_2 _T("TranTest2.bin")

namespace WindowsLibraryTest
{
	TEST_CLASS (TransactionFileTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("TransactionFileTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("TransactionFileTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			BOOL ret;
			
			ret = alt::FileUtility::Copy (_T ("C:\\Windows\\Notepad.exe"), TEST_FILE_1);
			Assert::IsTrue (ret, _T ("MethodInitialize failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE);
			if (ret)
			{
				ret = alt::FileUtility::Delete (TEST_FILE);
				Assert::IsTrue (ret, _T ("MethodInitialize failed.\n"));
			}

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			if (ret)
			{
				ret = alt::FileUtility::Delete (TEST_FILE_2);
				Assert::IsTrue (ret, _T ("MethodInitialize failed.\n"));
			}

			ret = alt::FileUtility::IsExist (TEST_FOLDER);
			if (ret)
			{
				ret = alt::FileUtility::RemoveDir (TEST_FOLDER);
				Assert::IsTrue (ret, _T ("MethodInitialize failed.\n"));
			}

			Logger::WriteMessage ("TransactinFileTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			if (alt::FileUtility::IsExist (TEST_FOLDER))
			{
				BOOL ret = alt::FileUtility::RemoveDir (TEST_FOLDER);
				Assert::IsTrue (ret, _T ("MethodInitialize failed.\n"));
			}

			if (alt::FileUtility::IsExist (TEST_FILE_1))
			{
				BOOL ret = alt::FileUtility::Delete (TEST_FILE_1);
				Assert::IsTrue (ret, _T ("MethodCleanup failed.\n"));
			}

			if (alt::FileUtility::IsExist (TEST_FILE_2))
			{
				BOOL ret = alt::FileUtility::Delete (TEST_FILE_2);
				Assert::IsTrue (ret, _T ("MethodCleanup failed.\n"));
			}

			Logger::WriteMessage ("TransactionFileTest method cleanup.\n");
		}

		TEST_METHOD (TransactionInitializeCommitTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionInitializeCommitTest"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Commit() failed.\n"));
		}

		TEST_METHOD (TransactionInitializeRollbackTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionInitializeRollbackTest"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Rollback() failed.\n"));
		}

		TEST_METHOD (TransactionCreateDirectoryCommitTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionCreateDirectoryCommitTest"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.CreateFolder (TEST_FOLDER);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::CreateFolder() failed.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Commit() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FOLDER);
			Assert::IsTrue (ret, _T ("directory is not created.\n"));

			ret = alt::FileUtility::IsDirectory (TEST_FOLDER);
			Assert::IsTrue (ret, _T ("This is not a folder.\n"));
		}

		TEST_METHOD (TransactionCreateDirectoryRollbackTest1)
		{
			BOOL ret;
			alt::skeleton::Binder<alt::TransactionFileUtility> tran;
			tran.New ();

			ret = tran->Init (_T ("TransactionCreateDirectoryRollbackTest1"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran->CreateFolder (TEST_FOLDER);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::CreateFolder() failed.\n"));

			tran.Delete ();

			ret = alt::FileUtility::IsExist (TEST_FOLDER);
			Assert::IsFalse (ret, _T ("directory is still exist.\n"));
		}

		TEST_METHOD (TransactionCreateDirectoryRollbackTest2)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionCreateDirectoryRollbackTest2"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.CreateFolder (TEST_FOLDER);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::CreateFolder() failed.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Rollback() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FOLDER);
			Assert::IsFalse (ret, _T ("directory is still exist.\n"));
		}

		TEST_METHOD (TransactionDeleteCommitTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionDeleteCommitTest"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Delete (TEST_FILE_1);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Delete() failed.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Commit() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsFalse (ret, _T ("file is still exist.\n"));
		}

		TEST_METHOD (TransactionDeleteRollbackTest1)
		{
			BOOL ret;
			alt::skeleton::Binder<alt::TransactionFileUtility> tran;
			tran.New ();

			ret = tran->Init (_T ("TransactionDeleteRollbackTest1"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran->Delete (TEST_FILE_1);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Delete() failed.\n"));

			tran.Delete();

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret, _T ("file is not exist.\n"));
		}

		TEST_METHOD (TransactionDeleteRollbackTest2)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionDeleteRollbackTest2"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Delete (TEST_FILE_1);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Delete() failed.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Rollback() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret, _T ("file is not exist.\n"));
		}

		TEST_METHOD (TransactionCopyCommitTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionCopyCommitTest"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Copy (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("can't copy file.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Commit() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsTrue (ret, _T ("file not exist.\n"));
		}

		TEST_METHOD (TransactionCopyRollbackTest1)
		{
			BOOL ret;
			alt::skeleton::Binder<alt::TransactionFileUtility> tran;
			tran.New ();

			ret = tran->Init (_T ("TransactionCopyRollbackTest1"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran->Copy (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Copy() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsTrue (ret, _T ("Transaction file copy failed.\n"));

			tran.Delete();

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsFalse (ret, _T ("file is still exist.\n"));
		}

		TEST_METHOD (TransactionCopyRollbackTest2)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionCopyRollbackTest2"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Copy (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Copy() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsTrue (ret, _T ("Transaction file copy failed.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Rollback() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsFalse (ret, _T ("file is still exist.\n"));
		}

		TEST_METHOD (TransactionMoveCommitTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionMoveCommitTest"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Move (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Move() failed.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Commit() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsFalse (ret, _T ("file is still exist.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsTrue (ret, _T ("FileUtility::IsExist() failed.\n"));
		}

		TEST_METHOD (TransactionMoveRollbackTest1)
		{
			BOOL ret;
			alt::skeleton::Binder<alt::TransactionFileUtility> tran;
			tran.New ();

			ret = tran->Init (_T ("TransactionMoveRollbackTest1"));
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Init() failed.\n"));

			ret = tran->Move (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Move() failed.\n"));

			tran.Delete();

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret, _T ("FileUtility::IsExist() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsFalse (ret, _T ("file is still exist.\n"));
		}

		TEST_METHOD (TransactionMoveRollbackTest2)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionMoveRollbackTest2"));
			Assert::IsTrue (ret, _T("TransactionFileUtility::Init() failed.\n"));

			ret = tran.Move (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Move() failed.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Rollback() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret, _T("file is not exist.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsFalse (ret, _T("file is still exist.\n"));
		}

		TEST_METHOD (TransactionMakeHardLinkCommitTest)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionMakeHardLinkCommitTest"));
			Assert::IsTrue (ret);

			ret = tran.MakeHardLink (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::MakeHardLink() failed.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Commit() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret);

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsTrue (ret);
		}

		TEST_METHOD (TransactionMakeHardLinkRollbackTest1)
		{
			BOOL ret;
			alt::skeleton::Binder<alt::TransactionFileUtility> tran;
			tran.New ();

			ret = tran->Init (_T ("TransactionMakeHardLinkRollbackTest1"));
			Assert::IsTrue (ret);

			ret = tran->MakeHardLink (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::MakeHardLink() failed.\n"));

			tran.Delete();

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret, _T("file is not exist.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsFalse (ret, _T("file is still exist.\n"));
		}

		TEST_METHOD (TransactionMakeHardLinkRollbackTest2)
		{
			BOOL ret;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionMakeHardLinkRollbackTest2"));
			Assert::IsTrue (ret);

			ret = tran.MakeHardLink (TEST_FILE_1, TEST_FILE_2);
			Assert::IsTrue (ret, _T ("TransactionFileUtility::MakeHardLink() failed.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T ("TransactionFileUtility::Rollback() failed.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_1);
			Assert::IsTrue (ret, _T("file is not exist.\n"));

			ret = alt::FileUtility::IsExist (TEST_FILE_2);
			Assert::IsFalse (ret, _T("file is still exist.\n"));
		}

		TEST_METHOD (TransactionIOCommitTest)
		{
			BOOL ret;
			DWORD dwLen;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionIOCommitTest"));
			Assert::IsTrue (ret);

			alt::TransactionFile tranFile;
			ret = tranFile.Create (
				tran.GetHandle (),
				TEST_FILE,
				GENERIC_READ | GENERIC_WRITE, // GENERIC_ALLはNG
				0,
				CREATE_ALWAYS);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("TransactionFileUtility::Create() failed.\n"));

			LPCTSTR lpctszContext = _T ("TEST TEST TEST");
			dwLen = tranFile.Write (lpctszContext, static_cast<DWORD>(_tcslen (lpctszContext) * sizeof (TCHAR)));
			Assert::AreEqual<SIZE_T> (_tcslen (lpctszContext) * sizeof (TCHAR), dwLen);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("TransactionFile::Write() failed.\n"));

			auto size = tranFile.GetSize ();
			Assert::AreEqual<SIZE_T> (_tcslen (lpctszContext) * sizeof (TCHAR), size);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tranFile.SetPointer (FILE_BEGIN);
			Assert::IsTrue (ret);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			TCHAR tszBuf[256];
			dwLen = tranFile.Read (tszBuf, sizeof (tszBuf));
			tszBuf[dwLen / sizeof (TCHAR)] = '\0';
			Assert::AreEqual<DWORD> (static_cast<DWORD>(_tcslen (lpctszContext) * sizeof (TCHAR)), dwLen);
			Assert::AreEqual<int> (0, _tcscmp (lpctszContext, tszBuf));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tranFile.Flush ();
			Assert::IsTrue (ret, _T("TransactionFile::Flush() failed.\n"));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tranFile.Close ();
			Assert::IsTrue (ret, _T("TransactionFile::Close() failed.\n"));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tran.Commit ();
			Assert::IsTrue (ret, _T("TransactionFile::Commit() failed.\n"));

			Assert::IsTrue (alt::FileUtility::IsExist (TEST_FILE));
		}

		TEST_METHOD (TransactionIORollbackTest)
		{
			BOOL ret;
			DWORD dwLen;
			alt::TransactionFileUtility tran;

			ret = tran.Init (_T ("TransactionIORollbackTest"));
			Assert::IsTrue (ret);

			alt::TransactionFile tranFile;
			ret = tranFile.Create (
				tran.GetHandle (),
				TEST_FILE,
				GENERIC_READ | GENERIC_WRITE, // GENERIC_ALLはNG
				0,
				CREATE_ALWAYS);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			LPCTSTR lpctszContext = _T ("TEST TEST TEST");
			dwLen = tranFile.Write (lpctszContext, static_cast<DWORD>(_tcslen (lpctszContext) * sizeof (TCHAR)));
			Assert::AreEqual<SIZE_T> (_tcslen (lpctszContext) * sizeof (TCHAR), dwLen);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			LONGLONG size = tranFile.GetSize ();
			Assert::AreEqual<SIZE_T> (_tcslen (lpctszContext) * sizeof (TCHAR), size);

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tranFile.SetPointer (FILE_BEGIN);
			Assert::IsTrue (ret, _T("TransactionFile::SetPointer() failed.\n"));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			TCHAR tszBuf[256];
			dwLen = tranFile.Read (tszBuf, sizeof (tszBuf));
			tszBuf[dwLen / sizeof (TCHAR)] = '\0';
			Assert::AreEqual<SIZE_T> (_tcslen (lpctszContext) * sizeof (TCHAR), dwLen);
			Assert::AreEqual<int> (0, _tcscmp (lpctszContext, tszBuf));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tranFile.Flush ();
			Assert::IsTrue (ret, _T("TransactionFile::Flush() failed.\n"));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tranFile.Close ();
			Assert::IsTrue (ret, _T("TransactionFile::Close() failed.\n"));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));

			ret = tran.Rollback ();
			Assert::IsTrue (ret, _T("TransactionFile::Rollback() failed.\n"));

			Assert::IsFalse (alt::FileUtility::IsExist (TEST_FILE), _T("file is still exist.\n"));
		}
	};
}