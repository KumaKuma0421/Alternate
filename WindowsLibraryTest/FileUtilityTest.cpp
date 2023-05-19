//! @file	FileUtilityTest.cpp
//! @brief	ファイルハンドルを伴わないファイルIOに関するWindowsAPIを集約したクラスのテストクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "FileUtility.h"
#include "Thread.h"
#include "Utility.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WindowsLibraryTest
{
	TEST_CLASS (FileUtilityTest)
	{
	public:
		TEST_CLASS_INITIALIZE (ClassInitialize)
		{
			Logger::WriteMessage ("FileUtilityTest class initialize.\n");
		}

		TEST_CLASS_CLEANUP (ClassCleanup)
		{
			Logger::WriteMessage ("FileUtilityTest class cleanup.\n");
		}

		TEST_METHOD_INITIALIZE (MethodInitialize)
		{
			Logger::WriteMessage ("FileUtilityTest method initialize.\n");
		}

		TEST_METHOD_CLEANUP (MethodCleanup)
		{
			Logger::WriteMessage ("FileUtilityTest method cleanup.\n");
		}

		void ShowFileInformation1 (alt::FileInfo1& item)
		{
			alt::TString output;

			output.Format (_T ("%s %s %lld\n"),
						   item.GetFolderName ().Ctr (),
						   item.GetFileName ().Ctr (),
						   item.GetFileSize ());
			Logger::WriteMessage (output.Ctr ());

			BOOL isReadOnly = item.IsReadOnly ();
			BOOL isHidden = item.IsHidden ();
			BOOL isSystem = item.IsSystem ();
			BOOL isDirectory = item.IsDirectory ();
			BOOL isArchive = item.IsArchive ();
			BOOL isTemporary = item.IsTemporary ();
			BOOL isCompressed = item.IsCompressed ();
			output.Format (_T ("D(%d) R(%d) H(%d) S(%d) A(%d) T(%d) C(%d)\n"),
						  isDirectory, isReadOnly, isHidden, isSystem,
						  isArchive, isTemporary, isCompressed);
			Logger::WriteMessage (output.Ctr ());

			SYSTEMTIME ct = alt::Utility::GetTimeByFileTime (item.GetCreateTime ());
			output.Format (_T ("CT:%04d/%02d/%02d %02d:%02d:%02d.%03d\n"),
						   ct.wYear, ct.wMonth, ct.wDay,
						   ct.wHour, ct.wMinute, ct.wSecond,
						   ct.wMilliseconds);
			Logger::WriteMessage (output.Ctr ());

			SYSTEMTIME ut = alt::Utility::GetTimeByFileTime (item.GetUpdateTime ());
			output.Format (_T ("UT:%04d/%02d/%02d %02d:%02d:%02d.%03d\n"),
						   ut.wYear, ut.wMonth, ut.wDay,
						   ut.wHour, ut.wMinute, ut.wSecond,
						   ut.wMilliseconds);
			Logger::WriteMessage (output.Ctr ());

			SYSTEMTIME at = alt::Utility::GetTimeByFileTime (item.GetAccessTime ());
			output.Format (_T ("AT:%04d/%02d/%02d %02d:%02d:%02d.%03d\n"),
						   at.wYear, at.wMonth, at.wDay,
						   at.wHour, at.wMinute, at.wSecond,
						   at.wMilliseconds);
			Logger::WriteMessage (output.Ctr ());

			Logger::WriteMessage (_T ("\n"));
		}

		void ShowFileInformation2 (alt::FileInfo2& item)
		{
			alt::TString output;

			if (item.GetAction () == (DWORD)-1)
			{
				Logger::WriteMessage ("FileUtility::DirectoryWatch() returns error.\n");
				return;
			}

			output.Format (_T ("%s %lld\n"),
						   item.GetFileName ().Ctr (),
						   item.GetFileSize ());
			Logger::WriteMessage (output.Ctr ());

			BOOL isReadOnly = item.IsReadOnly ();
			BOOL isHidden = item.IsHidden ();
			BOOL isSystem = item.IsSystem ();
			BOOL isDirectory = item.IsDirectory ();
			BOOL isArchive = item.IsArchive ();
			BOOL isTemporary = item.IsTemporary ();
			BOOL isCompressed = item.IsCompressed ();
			output.Format (_T ("D(%d) R(%d) H(%d) S(%d) A(%d) T(%d) C(%d)\n"),
						  isDirectory, isReadOnly, isHidden, isSystem,
						  isArchive, isTemporary, isCompressed);
			Logger::WriteMessage (output.Ctr ());

			BOOL isAdded = item.IsAdded ();
			BOOL isRemoved = item.IsRemoved ();
			BOOL isModified = item.IsModified ();
			BOOL isOldName = item.IsOldName ();
			BOOL isNewName = item.IsNewName ();
			output.Format (_T ("Added(%d) Removed(%d) Modified(%d) OldName(%d) NewName(%d)\n"),
						  isAdded, isRemoved, isModified, isOldName, isNewName);
			Logger::WriteMessage (output.Ctr ());

			SYSTEMTIME ct = alt::Utility::GetTimeByFileTime (item.GetCreateTime ());
			output.Format (_T ("CT:%04d/%02d/%02d %02d:%02d:%02d.%03d\n"),
						   ct.wYear, ct.wMonth, ct.wDay,
						   ct.wHour, ct.wMinute, ct.wSecond,
						   ct.wMilliseconds);
			Logger::WriteMessage (output.Ctr ());

			SYSTEMTIME ut = alt::Utility::GetTimeByFileTime (item.GetUpdateTime ());
			output.Format (_T ("UT:%04d/%02d/%02d %02d:%02d:%02d.%03d\n"),
						   ut.wYear, ut.wMonth, ut.wDay,
						   ut.wHour, ut.wMinute, ut.wSecond,
						   ut.wMilliseconds);
			Logger::WriteMessage (output.Ctr ());

			SYSTEMTIME at = alt::Utility::GetTimeByFileTime (item.GetAccessTime ());
			output.Format (_T ("AT:%04d/%02d/%02d %02d:%02d:%02d.%03d\n"),
						   at.wYear, at.wMonth, at.wDay,
						   at.wHour, at.wMinute, at.wSecond,
						   at.wMilliseconds);
			Logger::WriteMessage (output.Ctr ());

			output.Format (_T ("FileID:%lld\n"), item.GetFileID ());

			Logger::WriteMessage (_T ("\n"));
		}

		TEST_METHOD (DeleteTest)
		{
			BOOL ret;
			alt::TString tempFolder =
				alt::FileUtility::GetTemporaryPath ();
			alt::TString tempFile =
				alt::FileUtility::GetTemporaryFileName (
					tempFolder.Ctr (), _T ("FUTL"), 0); // 0指定で作成される。
			Logger::WriteMessage (tempFile.Ctr ());
			Logger::WriteMessage (_T ("\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("テンポラリファイルが作成されませんでした。\n"));

			ret = alt::FileUtility::Delete (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("failed Delete()\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsFalse (ret, _T ("failed Delete(). still file is exist.\n"));
		}

		TEST_METHOD (CopyTest)
		{
			BOOL ret;
			alt::TString tempFolder =
				alt::FileUtility::GetTemporaryPath ();
			alt::TString tempFile =
				alt::FileUtility::GetTemporaryFileName (
					tempFolder.Ctr (), _T ("FUTL"), 0); // 0指定で作成される。
			Logger::WriteMessage (tempFile.Ctr ());
			Logger::WriteMessage (_T ("\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("テンポラリファイルが作成されませんでした。\n"));

			alt::TString tempFile2 =
				alt::FileUtility::GetTemporaryFileName (
					tempFolder.Ctr (), _T ("FUTL"), 1);
			Logger::WriteMessage (tempFile2.Ctr ());
			Logger::WriteMessage (_T ("\n"));

			ret = alt::FileUtility::Copy (tempFile.Ctr (), tempFile2.Ctr ());
			Assert::IsTrue (ret, _T ("failed Copy()\n"));

			ret = alt::FileUtility::IsExist (tempFile2.Ctr ());
			Assert::IsTrue (ret, _T ("failed Copy(). file is not copied.\n"));

			alt::FileUtility::Delete (tempFile.Ctr ());
			alt::FileUtility::Delete (tempFile2.Ctr ());
		}

		TEST_METHOD (MoveTest)
		{
			BOOL ret;
			alt::TString tempFolder =
				alt::FileUtility::GetTemporaryPath ();
			alt::TString tempFile =
				alt::FileUtility::GetTemporaryFileName (
					tempFolder.Ctr (), _T ("FUTL"), 0); // 0指定で作成される。
			Logger::WriteMessage (tempFile.Ctr ());
			Logger::WriteMessage (_T ("\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("テンポラリファイルが作成されませんでした。\n"));

			alt::TString tempFile2 =
				alt::FileUtility::GetTemporaryFileName (
					tempFolder.Ctr (), _T ("FUTL"), 1);
			Logger::WriteMessage (tempFile2.Ctr ());
			Logger::WriteMessage (_T ("\n"));

			ret = alt::FileUtility::Move (tempFile.Ctr (), tempFile2.Ctr ());
			Assert::IsTrue (ret, _T ("failed Move()\n"));

			ret = alt::FileUtility::IsExist (tempFile2.Ctr ());
			Assert::IsTrue (ret, _T ("failed Move(). file is not moved.\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsFalse (ret, _T ("failed Move(). file is still exist.\n"));

			alt::FileUtility::Delete (tempFile.Ctr ());
			alt::FileUtility::Delete (tempFile2.Ctr ());
		}

		TEST_METHOD (MakeAndRemoveDirTest)
		{
			BOOL ret;
			alt::TString tempFolder =
				alt::FileUtility::GetTemporaryPath ();
			alt::TString tempFile =
				alt::FileUtility::GetTemporaryFileName (
					tempFolder.Ctr (), _T ("FUTL"), 1);
			Logger::WriteMessage (tempFile.Ctr ());
			Logger::WriteMessage (_T ("\n"));

			ret = alt::FileUtility::MakeDir (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("failed MakeDir()\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("failed MakeDir()\n"));

			ret = alt::FileUtility::IsDirectory (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("failed MakeDir()\n"));

			ret = alt::FileUtility::RemoveDir (tempFile.Ctr ());
			Assert::IsTrue (ret, _T ("failed RemoveDir()\n"));

			ret = alt::FileUtility::IsExist (tempFile.Ctr ());
			Assert::IsFalse (ret, _T ("failed RemoveDir()\n"));
		}

		TEST_METHOD (FindTest1)
		{
			alt::TString currentDir = alt::FileUtility::GetCurrentDir ();
			alt::FileUtility::SetCurrentDir (_T ("C:\\Windows\\system32"));
			auto response = alt::FileUtility::Find (_T ("."), _T ("*.cpl"));
			int i = 0;
			for (auto& item : response)
			{
				ShowFileInformation1 (item);
				if (++i == 100) break;
			}
			alt::FileUtility::SetCurrentDir (currentDir.Ctr ());
		}

		TEST_METHOD (FindTest2)
		{
			LPCTSTR lpctszFolder = _T ("C:\\Windows\\system32");
			LPCTSTR lpctszFile = _T ("*.cpl");
			auto response = alt::FileUtility::Find (lpctszFolder, lpctszFile, TRUE);
			int i = 0;
			for (auto& item : response)
			{
				ShowFileInformation1 (item);
				if (++i == 100) break;
			}
			response.Clear ();
		}

		TEST_METHOD (FindTest3)
		{
			LPCTSTR lpctszFolder = _T ("C:\\Windows\\system32");
			LPCTSTR lpctszFile = _T ("*");
			auto response = alt::FileUtility::Find (lpctszFolder, lpctszFile, TRUE);
			int i = 0;
			for (auto& item : response)
			{
				ShowFileInformation1 (item);
				if (++i == 100) break;
			}
			response.Clear ();
		}

		TEST_METHOD (Find2Test3)
		{
			LPCTSTR lpctszFolder = _T ("C:\\Windows\\system32");
			LPCTSTR lpctszFile = _T ("*");
			alt::skeleton::Array<alt::FileInfo1> response;

			alt::FileUtility::Find2 (lpctszFolder, lpctszFile, TRUE, response);
			int i = 0;
			for (auto& item : response)
			{
				ShowFileInformation1 (item);
				if (++i == 100) break;
			}
			response.Clear ();
		}

		TEST_METHOD (DirectoryWatchTest)
		{
			typedef alt::skeleton::Array<alt::FileInfo2> Response;

			auto watchFunction = [](LPVOID lpvParam)->DWORD
			{
				Response* response = static_cast<Response*>(lpvParam);
				alt::TString temporaryPath = alt::FileUtility::GetTemporaryPath ();

				*response = alt::FileUtility::DirectoryWatch (temporaryPath.Ctr (), 3000);

				Logger::WriteMessage (_T ("[watchFunction] finished.\n"));
				return 0;
			};

			auto eventFunction = [](LPVOID lpvParam)->DWORD
			{
				BOOL ret;
				alt::TString temporaryPath = alt::FileUtility::GetTemporaryPath ();
				Logger::WriteMessage (temporaryPath.Ctr ());
				alt::File temporaryFile;

				for (int i = 1; i <= 5; i++)
				{
					alt::TString temporaryFileName = alt::FileUtility::GetTemporaryFileName (temporaryPath.Ctr (), _T ("DWT"), i);
					Logger::WriteMessage (temporaryFileName.Ctr ());
					Logger::WriteMessage (_T ("\n"));

					ret = temporaryFile.Create (temporaryFileName.Ctr (), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_ALWAYS);
					Assert::IsTrue (ret, _T ("File::Create() failed.\n"));
					Logger::WriteMessage (_T ("[eventFunction] file created.\n"));

					BYTE byData[]{ 1,2,3,4,5,6,7,8,9,10 };
					DWORD dwSize = temporaryFile.Write (byData, sizeof (byData));
					Logger::WriteMessage (_T ("[eventFunction] file modified.\n"));

					ret = temporaryFile.Close ();
					Assert::IsTrue (ret, _T ("File::Close() failed.\n"));
					Logger::WriteMessage (_T ("[eventFunction] file closed.\n"));

					ret = alt::FileUtility::Delete (temporaryFileName.Ctr ());
					Assert::IsTrue (ret, _T ("FileUtility::Delete() failed.\n"));
					Logger::WriteMessage (_T ("[eventFunction file deleted.\n"));

					Sleep (50);
				}

				Logger::WriteMessage (_T ("[eventFunction] finished.\n"));
				return 0;
			};

			alt::Thread watchThread;
			Response response;
			Assert::IsTrue (watchThread.Create (watchFunction, (LPVOID)&response));

			alt::Thread eventThread;
			Assert::IsTrue (eventThread.Create (eventFunction));

			Assert::AreEqual<DWORD> (WAIT_OBJECT_0, eventThread.Wait (), _T ("eventThread.Wait() failed."));
			Logger::WriteMessage (_T ("[eventThread] finished.\n"));

			Assert::AreEqual<DWORD> (WAIT_OBJECT_0, watchThread.Wait (), _T ("watchThread.Wait() failed."));
			Logger::WriteMessage (_T ("[watchThread] finished.\n"));

			for (auto& item : response)
			{
				ShowFileInformation2 (item);
			}
		}

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

		// TODO:管理者権限か開発者モードにしないと実行できません。
		//TEST_METHOD(SymbolicLinkFileTest)
		//{
		//	BOOL ret;
		//	LPCTSTR lpctszBase = _T(".\\WindowsLibrary.dll");
		//	LPCTSTR lpctszTarget = _T(".\\linkBase.bin");
		//	LPCTSTR lpctszSymlink = _T(".\\linkSymbolic.bin");

		//	ret = FileUtility::Copy(lpctszBase, lpctszTarget);
		//	Assert::IsTrue(ret, _T("FileUtility::Copy() failed.\n"));
		//	ret = FileUtility::MakeSymbolicLink(lpctszTarget, lpctszSymlink);
		//	Assert::IsTrue(ret, _T("FileUtility::MakeSymbolicLink() failed.\n"));
		//	ret = FileUtility::Delete(lpctszSymlink);
		//	Assert::IsTrue(ret, _T("FileUtility::Delete() failed.\n"));
		//	ret = FileUtility::Delete(lpctszTarget);
		//	Assert::IsTrue(ret, _T("FileUtility::Delete() failed.\n"));
		//}

#endif

		//TEST_METHOD(SymbolicLinkDirectoryTest)
		//{

		//}

		TEST_METHOD (HardLinkFileTest)
		{
			BOOL ret;
			LPCTSTR lpctszBase = _T (".\\WindowsLibrary.dll");
			LPCTSTR lpctszTarget = _T (".\\linkBase.bin");
			LPCTSTR lpctszSymlink = _T (".\\linkHard.bin");

			ret = alt::FileUtility::Copy (lpctszBase, lpctszTarget);
			Assert::IsTrue (ret, _T ("FileUtility::Copy() failed.\n"));
			ret = alt::FileUtility::MakeHardLink (lpctszTarget, lpctszSymlink);
			Assert::IsTrue (ret, _T ("FileUtility::MakeHardLink() failed.\n"));
			ret = alt::FileUtility::Delete (lpctszSymlink);
			Assert::IsTrue (ret, _T ("FileUtility::Delete() failed.\n"));
			ret = alt::FileUtility::Delete (lpctszTarget);
			Assert::IsTrue (ret, _T ("FileUtility::Delete() failed.\n"));
		}

		TEST_METHOD (GetTemporaryPathTest)
		{
			alt::TString response = alt::FileUtility::GetTemporaryPath ();
			Logger::WriteMessage (response.Ctr ());
			Logger::WriteMessage ("\n");
		}

		TEST_METHOD (GetTemporaryFileNameTest)
		{
			{
				alt::TString response = alt::FileUtility::GetTemporaryFileName (
					alt::FileUtility::GetTemporaryPath ().Ctr (),
					_T ("alt"), 0);

				Logger::WriteMessage (response.Ctr ());
				Logger::WriteMessage ("\n");
			}

			{
				alt::TString response = alt::FileUtility::GetTemporaryFileName (
					alt::FileUtility::GetTemporaryPath ().Ctr (),
					_T ("alt"), 1);

				Logger::WriteMessage (response.Ctr ());
				Logger::WriteMessage ("\n");
			}

		}

		TEST_METHOD (GetAbsolutePathTest1)
		{
			alt::TString relativePath = _T ("Notepad.exe");
			alt::TString response = alt::FileUtility::GetAbsolutePath (relativePath.Ctr ());
			Logger::WriteMessage (response.Ctr ());
			Logger::WriteMessage (_T ("\n"));
		}

		TEST_METHOD (GetAbsolutePathTest2)
		{
			alt::TString relativePath = _T ("Temporary\\Sample\\");
			alt::TString response = alt::FileUtility::GetAbsolutePath (relativePath.Ctr ());
			Logger::WriteMessage (response.Ctr ());
			Logger::WriteMessage (_T ("\n"));
		}

		TEST_METHOD (SHFileOperationsCopy)
		{
			BOOL ret;

			alt::TString temporaryPath1;
			temporaryPath1 = alt::FileUtility::GetTemporaryPath ();
			temporaryPath1 += _T ("\\CopyTest1");
			ret = alt::FileUtility::MakeDir (temporaryPath1.Ctr ());
			Assert::IsTrue (ret, _T ("FileUtility::MakeDir() failed.\n"));
			Logger::WriteMessage (temporaryPath1.Ctr ());

			alt::TString temporaryFileName;

			for (int i = 0; i < 10; i++)
			{
				temporaryFileName = alt::FileUtility::GetTemporaryFileName (temporaryPath1.Ctr (), _T ("DWT"), i);
				Logger::WriteMessage (temporaryFileName.Ctr ());
				Logger::WriteMessage (_T ("\n"));

				alt::File temporaryFile;
				ret = temporaryFile.Create (temporaryFileName.Ctr (), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_ALWAYS);
				Assert::IsTrue (ret, _T ("File::Create() failed.\n"));
				Logger::WriteMessage (_T ("file created.\n"));

				BYTE byData[]{ 1,2,3,4,5,6,7,8,9,10 };
				DWORD dwSize = temporaryFile.Write (byData, sizeof (byData));
				Logger::WriteMessage (_T ("file modified.\n"));

				ret = temporaryFile.Close ();
				Assert::IsTrue (ret, _T ("File::Close() failed.\n"));
				Logger::WriteMessage (_T ("file closed.\n"));
			}

			alt::TString temporaryPath2;
			temporaryPath2 = alt::FileUtility::GetTemporaryPath ();
			temporaryPath2 += _T ("\\CopyTest2");
			Logger::WriteMessage (temporaryPath2.Ctr ());

			alt::ShellOperation operation = alt::ShellOperation::COPY;
			int response = alt::FileUtility::SHFileOperations (operation, temporaryPath1.Ctr (), temporaryPath2.Ctr ());

			Assert::IsTrue (alt::FileUtility::IsExist (temporaryPath2.Ctr ()), _T ("The folder is not exist.\n"));

			// 後片付け
			response = alt::FileUtility::SHFileOperations (alt::ShellOperation::REMOVE, temporaryPath1.Ctr (), NULL);
			response = alt::FileUtility::SHFileOperations (alt::ShellOperation::REMOVE, temporaryPath2.Ctr (), NULL);
		}

		TEST_METHOD (SHFileOperationsMove)
		{
			BOOL ret;

			alt::TString temporaryPath1;
			temporaryPath1 = alt::FileUtility::GetTemporaryPath ();
			temporaryPath1 += _T ("\\CopyTest1");
			ret = alt::FileUtility::MakeDir (temporaryPath1.Ctr ());
			Assert::IsTrue (ret, _T ("FileUtility::MakeDir() failed.\n"));
			Logger::WriteMessage (temporaryPath1.Ctr ());

			alt::TString temporaryFileName;

			for (int i = 0; i < 10; i++)
			{
				temporaryFileName = alt::FileUtility::GetTemporaryFileName (temporaryPath1.Ctr (), _T ("DWT"), i);
				Logger::WriteMessage (temporaryFileName.Ctr ());
				Logger::WriteMessage (_T ("\n"));

				alt::File temporaryFile;
				ret = temporaryFile.Create (temporaryFileName.Ctr (), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_ALWAYS);
				Assert::IsTrue (ret, _T ("File::Create() failed.\n"));
				Logger::WriteMessage (_T ("file created.\n"));

				BYTE byData[]{ 1,2,3,4,5,6,7,8,9,10 };
				DWORD dwSize = temporaryFile.Write (byData, sizeof (byData));
				Logger::WriteMessage (_T ("file modified.\n"));

				ret = temporaryFile.Close ();
				Assert::IsTrue (ret, _T ("File::Close() failed.\n"));
				Logger::WriteMessage (_T ("file closed.\n"));
			}

			alt::TString temporaryPath2;
			temporaryPath2 = alt::FileUtility::GetTemporaryPath ();
			temporaryPath2 += _T ("\\CopyTest2");
			Logger::WriteMessage (temporaryPath2.Ctr ());

			alt::ShellOperation operation = alt::ShellOperation::MOVE;
			int response = alt::FileUtility::SHFileOperations (operation, temporaryPath1.Ctr (), temporaryPath2.Ctr ());

			Assert::IsFalse (alt::FileUtility::IsExist (temporaryPath1.Ctr ()), _T ("The folder is still exist.\n"));
			Assert::IsTrue (alt::FileUtility::IsExist (temporaryPath2.Ctr ()), _T ("The folder is not exist.\n"));

			// 後片付け
			response = alt::FileUtility::SHFileOperations (alt::ShellOperation::REMOVE, temporaryPath1.Ctr (), NULL);
			response = alt::FileUtility::SHFileOperations (alt::ShellOperation::REMOVE, temporaryPath2.Ctr (), NULL);
		}

		TEST_METHOD (SHFileOperationsDelete)
		{
			BOOL ret;

			alt::TString temporaryPath;
			temporaryPath = alt::FileUtility::GetTemporaryPath ();
			temporaryPath += _T ("\\DeleteTest");
			ret = alt::FileUtility::MakeDir (temporaryPath.Ctr ());
			Assert::IsTrue (ret, _T ("FileUtility::MakeDir() failed.\n"));
			Logger::WriteMessage (temporaryPath.Ctr ());

			alt::TString temporaryFileName;

			for (int i = 0; i < 10; i++)
			{
				temporaryFileName = alt::FileUtility::GetTemporaryFileName (temporaryPath.Ctr (), _T ("DWT"), i);
				Logger::WriteMessage (temporaryFileName.Ctr ());
				Logger::WriteMessage (_T ("\n"));

				alt::File temporaryFile;
				ret = temporaryFile.Create (temporaryFileName.Ctr (), GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, CREATE_ALWAYS);
				Assert::IsTrue (ret, _T ("File::Create() failed.\n"));
				Logger::WriteMessage (_T ("file created.\n"));

				BYTE byData[]{ 1,2,3,4,5,6,7,8,9,10 };
				DWORD dwSize = temporaryFile.Write (byData, sizeof (byData));
				Logger::WriteMessage (_T ("file modified.\n"));

				ret = temporaryFile.Close ();
				Assert::IsTrue (ret, _T ("File::Close() failed.\n"));
				Logger::WriteMessage (_T ("file closed.\n"));
			}

			alt::ShellOperation operation = alt::ShellOperation::REMOVE;
			int response = alt::FileUtility::SHFileOperations (operation, temporaryPath.Ctr (), NULL);

			Assert::IsFalse (alt::FileUtility::IsExist (temporaryPath.Ctr ()), _T ("The file is still exist.\n"));
		}
	};
}