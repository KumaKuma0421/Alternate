// ----------------------------------------------------------------------------
//! @file	FileUtility.cpp
//! @brief	ファイルハンドルを伴わないファイルIOに関するWindowsAPIを
//!         集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "FileUtility.h"
#include "Event.h"

using namespace alt;

// ----- FileInfo

FileInfo::FileInfo ()
{
	_FileSize = { 0, 0 };
}

FileInfo::~FileInfo ()
{

}

// ----- FileInfo1

FileInfo1::FileInfo1 ()
{
	_CreateTime = { 0, 0 };
	_UpdateTime = { 0, 0 };
	_AccessTime = { 0, 0 };
	_Attribute = 0;
}

FileInfo1::~FileInfo1 ()
{

}

// ----- FileInfo2

FileInfo2::FileInfo2 ()
{
	_ChangeTime = { 0, 0 };
	_FileID = 0;
	_dwAction = (DWORD)-1;
}

FileInfo2::~FileInfo2 ()
{

}

// ----- FileUtility

BOOL FileUtility::Delete (LPCTSTR name)
{
	return ::DeleteFile (name);
}

BOOL FileUtility::Copy (LPCTSTR source, LPCTSTR dest, BOOL bFailIfExists)
{
	return ::CopyFile (source, dest, bFailIfExists);
}

BOOL FileUtility::Move (LPCTSTR source, LPCTSTR dest)
{
	return ::MoveFile (source, dest);
}

TString FileUtility::GetTemporaryFileName (
	LPCTSTR lpctszPath, LPCTSTR lpctszPrefix, UINT nNumber)
{
	TString response (MAX_PATH);

	::GetTempFileName (lpctszPath, lpctszPrefix, nNumber, response.Ptr ());

	return response;
}

BOOL FileUtility::MakeDir (LPCTSTR name)
{
	return ::CreateDirectory (name, NULL);
}

BOOL FileUtility::RemoveDir (LPCTSTR name)
{
	return ::RemoveDirectory (name);
}

BOOL FileUtility::SetCurrentDir (LPCTSTR name)
{
	return ::SetCurrentDirectory (name);
}

alt::TString FileUtility::GetCurrentDir ()
{
	alt::TString response (MAX_PATH);

	BOOL ret = ::GetCurrentDirectory (MAX_PATH, response.Ptr ());

	return ret == TRUE ? response : NULL;
}

TString FileUtility::GetTemporaryPath ()
{
	TString response (MAX_PATH);

	GetTempPath (MAX_PATH, response.Ptr ());

	return response;
}

TString FileUtility::GetAbsolutePath (LPCTSTR lpctszRelativePath)
{
	TString response (MAX_PATH);

	::GetFullPathName (lpctszRelativePath, MAX_PATH, response.Ptr (), NULL);

	return response;
}

BOOL FileUtility::IsDirectory (LPCTSTR name)
{
	return ::PathIsDirectory (name);
}

BOOL FileUtility::IsExist (LPCTSTR name)
{
	return ::PathFileExists (name);
}

skeleton::Array<FileInfo1> FileUtility::Find (LPCTSTR target, LPCTSTR name, BOOL bRecurse)
{
	WIN32_FIND_DATA myFindData;
	HANDLE hFind;
	skeleton::Array<FileInfo1> response;
	TString search;

	search.Format (_T ("%s\\%s"), target, name);

	hFind = ::FindFirstFile (search.Ctr (), &myFindData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == NULL) ||
				(myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == '.' && myFindData.cFileName[2] == NULL))
			{
				continue; // skip alias
			}
			else
			{
				FileInfo1 info;

				info._FolderName = target;
				info._FileName = myFindData.cFileName;
				info._FileSize.HighPart = myFindData.nFileSizeHigh;
				info._FileSize.LowPart = myFindData.nFileSizeLow;
				info._Attribute = myFindData.dwFileAttributes;
				info._CreateTime = myFindData.ftCreationTime;
				info._UpdateTime = myFindData.ftLastWriteTime;
				info._AccessTime = myFindData.ftLastAccessTime;

				response.Add (info);
			}
		} while (::FindNextFile (hFind, &myFindData));

		// 本当なら、ここでGetLastError()がERROR_NO_MORE_DATAであることを確認したほうがいい。

		::FindClose (hFind);
	}

	if (bRecurse)
	{
		search.Format (_T ("%s\\%s"), target, _T ("\\*"));
		hFind = ::FindFirstFile (search.Ctr (), &myFindData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if ((myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == NULL) ||
					(myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == '.' && myFindData.cFileName[2] == NULL))
				{
					continue; // skip alias
				}
				else if (myFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					search.Format (_T ("%s\\%s"), target, myFindData.cFileName);
					// 以下のやり方では、失敗する。
					// response += Find (search.Ctr (), name, bRecurse);
					// 一度tempに受けてから＋＝を行うと安定する。原因不明......
					skeleton::Array<FileInfo1> temp = Find (search.Ctr (), name, bRecurse);
					response += temp;
				}

			} while (::FindNextFile (hFind, &myFindData));
		}
	}

	return response;
}

VOID FileUtility::Find2 (LPCTSTR target, LPCTSTR name, BOOL bRecurse, skeleton::Array<FileInfo1>& response)
{
	WIN32_FIND_DATA myFindData;
	HANDLE hFind;
	TString search;

	search.Format (_T ("%s\\%s"), target, name);

	hFind = ::FindFirstFile (search.Ctr (), &myFindData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == NULL) ||
				(myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == '.' && myFindData.cFileName[2] == NULL))
			{
				continue; // skip alias
			}
			else
			{
				FileInfo1 info;

				info._FolderName = target;
				info._FileName = myFindData.cFileName;
				info._FileSize.HighPart = myFindData.nFileSizeHigh;
				info._FileSize.LowPart = myFindData.nFileSizeLow;
				info._Attribute = myFindData.dwFileAttributes;
				info._CreateTime = myFindData.ftCreationTime;
				info._UpdateTime = myFindData.ftLastWriteTime;
				info._AccessTime = myFindData.ftLastAccessTime;

				response.Add (info);
			}
		} while (::FindNextFile (hFind, &myFindData));

		// 本当なら、ここでGetLastError()がERROR_NO_MORE_DATAであることを確認したほうがいい。

		::FindClose (hFind);
	}

	if (bRecurse)
	{
		search.Format (_T ("%s\\%s"), target, _T ("\\*"));
		hFind = ::FindFirstFile (search.Ctr (), &myFindData);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if ((myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == NULL) ||
					(myFindData.cFileName[0] == '.' && myFindData.cFileName[1] == '.' && myFindData.cFileName[2] == NULL))
				{
					continue; // skip alias
				}
				else if (myFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					search.Format (_T ("%s\\%s"), target, myFindData.cFileName);
					Find2 (search.Ctr (), name, bRecurse, response);
				}

			} while (::FindNextFile (hFind, &myFindData));
		}
	}

	return;
}

skeleton::Array<FileInfo2> FileUtility::DirectoryWatch (LPCTSTR lpctszWatchDirectory, DWORD dwTimeout)
{
	BOOL ret = FALSE;
	skeleton::Array<FileInfo2> response;
	HANDLE hDirectory;
	Event notifyEvent;
	DWORD dwResponse;
	DWORD dwNotifyFilter;
	BOOL bContinue = FALSE;

	dwNotifyFilter =
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE |
		FILE_NOTIFY_CHANGE_LAST_ACCESS |
		FILE_NOTIFY_CHANGE_CREATION;

	do
	{
		hDirectory =
			::CreateFile (
				lpctszWatchDirectory,
				FILE_LIST_DIRECTORY,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				NULL,
				OPEN_EXISTING,
				FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
				NULL);

		if (hDirectory == INVALID_HANDLE_VALUE)
		{
			break;
		}

		ret = notifyEvent.Create (NULL, FALSE, FALSE);
		if (ret == FALSE)
		{
			break;
		}

		bContinue = TRUE;

	} while (false);

	while (bContinue)
	{
		OVERLAPPED stOverlapped;
		ZeroMemory (&stOverlapped, sizeof (stOverlapped));
		stOverlapped.hEvent = notifyEvent.GetHandle ();

		CONST DWORD cdwBufferLength = 4096;
		WCHAR* buffer = new WCHAR[cdwBufferLength];
		ZeroMemory (buffer, sizeof (WCHAR) * cdwBufferLength);

		BOOL bWatchSubtree = TRUE;

		ret = ::ReadDirectoryChangesExW (
			hDirectory,
			buffer,
			sizeof (WCHAR) * cdwBufferLength,
			bWatchSubtree,
			dwNotifyFilter,
			&dwResponse,
			&stOverlapped,
			NULL,
			READ_DIRECTORY_NOTIFY_INFORMATION_CLASS::ReadDirectoryNotifyExtendedInformation);
		if (ret == FALSE)
		{
			bContinue = FALSE;
			break;
		}

		DWORD dwEvent = notifyEvent.Wait (dwTimeout);
		if (dwEvent != WAIT_OBJECT_0)
		{
			bContinue = FALSE;
			break;
		}
		else
		{
			ret = TRUE;
		}

		PFILE_NOTIFY_EXTENDED_INFORMATION currentPFNI = (PFILE_NOTIFY_EXTENDED_INFORMATION)buffer;
		
		for (INT i = 0; ;)
		{
			if (currentPFNI->Action > 5)
			{
				FileInfo2 errorInfo;
				errorInfo._dwAction = (DWORD)-1;
				response.Add (errorInfo);
				bContinue = FALSE;
				break;
			}

			if (currentPFNI->Action > 0)
			{
				TString fileName (MAX_PATH);
				FileInfo2 info2;

				CopyMemory (fileName.Ptr (), currentPFNI->FileName, currentPFNI->FileNameLength);
				info2._dwAction = currentPFNI->Action;
				info2._FileName = fileName;
				info2._FileSize.QuadPart = currentPFNI->FileSize.QuadPart;
				// GetSystemTime()ベースの時刻になります。
				info2._CreateTime.dwLowDateTime = currentPFNI->CreationTime.LowPart;
				info2._CreateTime.dwHighDateTime = currentPFNI->CreationTime.HighPart;
				info2._UpdateTime.dwLowDateTime = currentPFNI->LastModificationTime.LowPart;
				info2._UpdateTime.dwHighDateTime = currentPFNI->LastModificationTime.HighPart;
				info2._AccessTime.dwLowDateTime = currentPFNI->LastAccessTime.LowPart;
				info2._AccessTime.dwHighDateTime = currentPFNI->LastAccessTime.HighPart;
				info2._ChangeTime.dwLowDateTime = currentPFNI->LastChangeTime.LowPart;
				info2._ChangeTime.dwHighDateTime = currentPFNI->LastChangeTime.HighPart;
				info2._Attribute = currentPFNI->FileAttributes;
				info2._FileID = currentPFNI->FileId.QuadPart;

				response.Add (info2);
			}

			currentPFNI += currentPFNI->NextEntryOffset;
			if (currentPFNI->NextEntryOffset == 0) break;
			i += currentPFNI->NextEntryOffset;
		}

		delete[] buffer;
	}

	CloseHandle (hDirectory);
	return response;
}

#if defined (_MSC_VER) && (_MSC_VER >= 1910) // VS2017 over

BOOL FileUtility::MakeSymbolicLink (LPCTSTR source, LPCTSTR link, BOOL isFile)
{
	DWORD dwFlags = isFile == TRUE ? 0 : SYMBOLIC_LINK_FLAG_DIRECTORY;

	dwFlags |= SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;

	return ::CreateSymbolicLink (link, source, dwFlags);
}

#endif

BOOL FileUtility::MakeHardLink (LPCTSTR source, LPCTSTR link)
{
	return ::CreateHardLink (link, source, NULL);
}

INT FileUtility::SHFileOperations (
	ShellOperation operation, LPCTSTR lpctszFrom, LPCTSTR lpctszTo)
{
	INT response;
	SHFILEOPSTRUCT shellOperation{ NULL, NULL, NULL, NULL, NULL };
	TCHAR tszzFrom[MAX_PATH]{ NULL };
	TCHAR tszzTo[MAX_PATH]{ NULL };

	wsprintf (tszzFrom, _T ("%s\0"), lpctszFrom);
	wsprintf (tszzTo, _T ("%s\0"), lpctszTo);

	shellOperation.hwnd = NULL;
	shellOperation.wFunc = (WORD)operation;
	shellOperation.pFrom = tszzFrom;
	shellOperation.pTo = tszzTo;
	shellOperation.fFlags = FOF_NO_UI;

	response = ::SHFileOperation (&shellOperation);
	::SHChangeNotify (SHCNE_UPDATEDIR, SHCNF_PATH, (LPCVOID)tszzTo, 0);

	return response;
}