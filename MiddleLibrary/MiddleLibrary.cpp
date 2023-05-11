//! @file	MiddleLibrary.cpp
//! @brief	MiddleLibraryプロジェクトのメインクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.05

#include "pch.h"
#include "MiddleLibrary.h"

using namespace alt;

BOOL APIENTRY DllMain (HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved)
{
	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
			OutputDebugString (_T ("WindowsLibrary.dll DLL_PROCESS_ATTACH.\n"));
			break;
		case DLL_THREAD_ATTACH:
			OutputDebugString (_T ("WindowsLibrary.dll DLL_THREAD_ATTACH.\n"));
			break;
		case DLL_THREAD_DETACH:
			OutputDebugString (_T ("WindowsLibrary.dll DLL_THREAD_DETACH.\n"));
			break;
		case DLL_PROCESS_DETACH:
			OutputDebugString (_T ("WindowsLibrary.dll DLL_PROCESS_DETACH.\n"));
			break;
	}

	return TRUE;
}

DWORD MiddleLibrary::GetErrNo ()
{
	return ::GetLastError ();
}

VOID MiddleLibrary::debug (LPCTSTR lpctszMsg)
{
	SYSTEMTIME T;
	GetLocalTime (&T);

	TCHAR tszMsg[1024];
	ZeroMemory (tszMsg, sizeof (tszMsg));

	wsprintf (tszMsg, _T ("%04d/%02d/%02d %02d:%02d:%02d.%03d %s\n"),
			 T.wYear, T.wMonth, T.wDay,
			 T.wHour, T.wMinute, T.wSecond, T.wMilliseconds, lpctszMsg);

	OutputDebugString (tszMsg);
}

TString MiddleLibrary::GetProfileStr (LPCTSTR lpctszSection, LPCTSTR lpctszKeyword)
{
	TCHAR tszResult[MAX_PATH];
	TString response;

	DWORD dwRet = ::GetPrivateProfileString (lpctszSection, lpctszKeyword,
											nullptr, tszResult, MAX_PATH, _profile.Ctr ());

	if (0 < dwRet)
	{
		response = tszResult;
	}

	return response;
}

UINT MiddleLibrary::GetProfileValue (LPCTSTR lpctszSection, LPCTSTR lpctszKeyword, UINT nDefault)
{
	return ::GetPrivateProfileInt (lpctszSection, lpctszKeyword, nDefault, _profile.Ctr ());
}

BOOL MiddleLibrary::SetProfile (LPCTSTR lpctszFileName)
{
	BOOL ret = TRUE;

	BOOL isExist = FileUtility::IsExist (lpctszFileName);
	if (!isExist) return FALSE;

	_profile = lpctszFileName;

	return ret;
}

TString MiddleLibrary::GetFormattedDateTime (SYSTEMTIME& systemTime)
{
	TString dateTime (24);

	wsprintf (dateTime.Ptr (), _T ("%04d/%02d/%02d %02d:%02d:%02d.%03d"),
			 systemTime.wYear, systemTime.wMonth, systemTime.wDay,
			 systemTime.wHour, systemTime.wMinute, systemTime.wSecond,
			 systemTime.wMilliseconds);

	return dateTime;
}