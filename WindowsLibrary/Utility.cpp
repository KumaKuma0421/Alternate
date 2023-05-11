// ----------------------------------------------------------------------------
//! @file	Utility.cpp
//! @brief	汎用的に使えるユーティリティクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.2.24

#include "pch.h"
#include "Utility.h"
#include "File.h"

using namespace alt;

const GUID Utility::CreateGUID ()
{
	GUID guid;

	HRESULT res = ::CoCreateGuid (&guid);

	return guid;
}

const TString Utility::MakeGUID ()
{
	GUID guid = CreateGUID ();

	TString ret;
	// xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
	ret.Format (
		_T ("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X"),
		guid.Data1, guid.Data2, guid.Data3, guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);

	return ret;
}

SYSTEMTIME Utility::GetSystemTime ()
{
	SYSTEMTIME systemTime;

	::GetSystemTime (&systemTime);

	return systemTime;
}

SYSTEMTIME Utility::GetLocalTime ()
{
	SYSTEMTIME systemTime;

	::GetLocalTime (&systemTime);

	return systemTime;
}

TString Utility::GetFormatTime (const SYSTEMTIME& systemTime)
{
	TString response;
	response.Format (
		_T ("%04d/%02d/%02d %02d:%02d:%02d.%03d"), systemTime.wYear,
		systemTime.wMonth, systemTime.wDay, systemTime.wHour,
		systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds);

	return response;
}

ULARGE_INTEGER Utility::GetLocalTimeQuad ()
{
	SYSTEMTIME systemTime = Utility::GetLocalTime ();

	FILETIME fileTime;
	BOOL ret = ::SystemTimeToFileTime (&systemTime, &fileTime);

	ULARGE_INTEGER response{ 0, 0 };

	if (ret)
	{
		response.HighPart = fileTime.dwHighDateTime;
		response.LowPart = fileTime.dwLowDateTime;
	}

	return response;
}

SYSTEMTIME Utility::GetTimeByFileTime (const FILETIME& fileTime)
{
	SYSTEMTIME systemlTime{ 0, 0, 0, 0, 0, 0, 0, 0 };

	::FileTimeToSystemTime (&fileTime, &systemlTime);

	return systemlTime;
}

VOID Utility::GetSystemInfo (SYSTEM_INFO& systemInfo)
{
	::GetSystemInfo (&systemInfo);
}

BOOL Utility::Base64Encode (
	LPBYTE lpbyString, DWORD dwLength, TString& response)
{
	BOOL bResponse = FALSE;
	DWORD dwBuffer = 0;
	LPTSTR lptszBuffer;

	if (::CryptBinaryToString (
		lpbyString, dwLength, CRYPT_STRING_BASE64, NULL, &dwBuffer))
	{
		lptszBuffer = new TCHAR[dwBuffer + sizeof (TCHAR)];
		if (::CryptBinaryToString (
			lpbyString, dwLength, CRYPT_STRING_BASE64, lptszBuffer, &dwBuffer))
		{
			response = lptszBuffer;
			bResponse = TRUE;
		}

		delete[] lptszBuffer;
	}

	return bResponse;
}

BOOL Utility::Base64Decode (TString& source, LPBYTE lpbyBuffer, DWORD& dwBuffer)
{
	BOOL bResponse = FALSE;

	if (lpbyBuffer == NULL)
	{
		bResponse = ::CryptStringToBinary (
			source.Ctr (), source.Len (), CRYPT_STRING_BASE64, NULL, &dwBuffer,
			NULL, NULL);
	}
	else
	{
		bResponse = ::CryptStringToBinary (
			source.Ctr (), source.Len (), CRYPT_STRING_BASE64, lpbyBuffer,
			&dwBuffer, NULL, NULL);
	}

	return bResponse;
}

// @sa https://docs.microsoft.com/ja-jp/windows/win32/procthread/changing-environment-variables#example-3
skeleton::Array<TString> Utility::GetEnvironments ()
{
	skeleton::Array<TString> response;

	LPTCH lptchEnv = ::GetEnvironmentStrings ();
	if (lptchEnv == NULL) return response;

	LPTSTR lptszVariable = (LPTSTR)lptchEnv;

	while (*lptszVariable)
	{
		TString string (lptszVariable);
		response.Add (string);
		lptszVariable += (lstrlen (lptszVariable) + (SIZE_T)1);
	}

	FreeEnvironmentStrings (lptchEnv);

	return response;
}

TString Utility::GetEnv (LPCTSTR lpctszKeyword)
{
	DWORD dwRet;
	TString enpty;

	dwRet = ::GetEnvironmentVariable (lpctszKeyword, NULL, 0);
	if (dwRet == 0) return enpty;

	TString response (dwRet);
	::GetEnvironmentVariable (lpctszKeyword, response.Ptr (), dwRet);

	return response;
}

BOOL Utility::ReadIniFile (LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword, TString& response)
{
	BOOL ret;
	TCHAR tszBuffer[MAX_PATH];

	DWORD dwSize = ::GetPrivateProfileString (lpctszSection, lpctszKeyword, _T (""), tszBuffer, MAX_PATH, lpctszIniFile);

	if (dwSize > 0)
	{
		response = tszBuffer;
		ret = TRUE;
	}
	else
	{
		response = _T ("");
		ret = FALSE;
	}

	return ret;
}

DWORD Utility::ReadIniFile (LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword, DWORD dwDefault)
{
	return ::GetPrivateProfileInt (lpctszSection, lpctszKeyword, dwDefault, lpctszIniFile);
}

BOOL Utility::WriteIniFile (LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword, TString& strValue)
{
	return ::WritePrivateProfileString (lpctszSection, lpctszKeyword, strValue.Ctr (), lpctszIniFile);
}

BOOL Utility::WriteIniFile (LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword, INT iValue)
{
	TString strValue;
	
	strValue << iValue;

	return ::WritePrivateProfileString (lpctszSection, lpctszKeyword, strValue.Ctr (), lpctszIniFile);
}

BOOL Utility::CreateMD5 (LPBYTE lpbyData, DWORD dwSize, LPBYTE lpbyMD5)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	DWORD dwHashSize = 16;
	BYTE byHash[16];
	CHAR szDigits[] = "0123456789ABCDEF";
	BOOL ret = FALSE;

	do
	{
		ret = ::CryptAcquireContext (&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
		if (!ret) break;

		ret = ::CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash);
		if (!ret) break;

		ret = ::CryptHashData (hHash, lpbyData, dwSize, 0);
		if (!ret) break;

		ret = CryptGetHashParam (hHash, HP_HASHVAL, byHash, &dwHashSize, 0);
		if (!ret) break;

		int n = 0;
		for (DWORD i = 0; i < dwHashSize; i++)
		{
			CHAR cData1 = szDigits[byHash[i] >> 4];
			CHAR cData2 = szDigits[byHash[i] & 0x0f];
			lpbyMD5[n++] = cData1;
			lpbyMD5[n++] = cData2;
		}

		ret = ::CryptDestroyHash (hHash);
		if (!ret) break;

		ret = ::CryptReleaseContext (hProv, 0);
		if (!ret) break;
	} while (false);

	return ret;
}

BOOL Utility::CreateMD5 (LPCTSTR lpctszFileName, LPBYTE lpbyMD5)
{
	HCRYPTPROV hProv;
	HCRYPTHASH hHash;
	DWORD dwHashSize = 16;
	BYTE byHash[16];
	const CHAR cszDigits[] = "0123456789ABCDEF";
	alt::File checkFile;
	BYTE byBuffer[8192]{ 0 };
	DWORD dwReadSize;
	BOOL ret = FALSE;

	do
	{
		ret = checkFile.Create (lpctszFileName, GENERIC_READ, 0, OPEN_EXISTING);
		if (!ret) break;

		ret = ::CryptAcquireContext (&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);
		if (!ret) break;

		ret = ::CryptCreateHash (hProv, CALG_MD5, 0, 0, &hHash);
		if (!ret) break;

		while ((dwReadSize = checkFile.Read (byBuffer, sizeof (byBuffer))) > 0)
		{
			ret = ::CryptHashData (hHash, byBuffer, dwReadSize, 0);
			if (!ret) break;
		}

		ret = checkFile.Close ();
		if (!ret) break;

		ret = CryptGetHashParam (hHash, HP_HASHVAL, byHash, &dwHashSize, 0);
		if (!ret) break;

		int n = 0;
		for (DWORD i = 0; i < dwHashSize; i++)
		{
			const CHAR ccData1 = cszDigits[byHash[i] >> 4];
			const CHAR ccData2 = cszDigits[byHash[i] & 0x0f];
			lpbyMD5[n++] = ccData1;
			lpbyMD5[n++] = ccData2;
		}

		ret = ::CryptDestroyHash (hHash);
		if (!ret) break;

		ret = ::CryptReleaseContext (hProv, 0);
		if (!ret) break;
	} while (false);

	return ret;
}