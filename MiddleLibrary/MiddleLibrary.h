//! @file	MiddleLibrary.h
//! @brief	MiddleLibraryプロジェクトのメインクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.05

#pragma once

#include "framework.h"
#include "TString.h"
#include "FileUtility.h"

BOOL APIENTRY DllMain (HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved);

namespace alt
{
	//! @class	MiddleLibrary
	//! @brief	MiddleLibraryプロジェクトのメインクラス
	class DLL_DECLSPEC MiddleLibrary
	{
	public:
		virtual DWORD GetErrNo ();
		virtual VOID debug (LPCTSTR lpctszMsg);
		TString GetProfileStr (LPCTSTR lpctszSection, LPCTSTR lpctszKeyword);
		UINT GetProfileValue (LPCTSTR lpctszSection, LPCTSTR lpctszKeyword, UINT nDefault = UINT_MAX);
		BOOL SetProfile (LPCTSTR lpctszFileName);
		TString GetFormattedDateTime (SYSTEMTIME& systemTime);
		DWORD GetPID () { return GetCurrentProcessId (); };

	protected:
		TString _profile;
	};
}