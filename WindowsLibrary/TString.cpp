// ----------------------------------------------------------------------------
//! @file	TString.cpp
//! @brief	文字列に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "TString.h"

using namespace alt;

TString::TString ()
{
	_lptszString = NULL;
}

TString::TString (LPCTSTR lpctszString)
	:TString ()
{
	this->Copy (lpctszString);
}

TString::TString (const TString& string)
	: TString ()
{
	this->Copy (string.Ptr ());
}

TString::TString (INT length)
{
	_lptszString = CreateMemory (length);
}

TString::~TString ()
{
	Clear ();
}

INT TString::Len () const
{
	return Length (_lptszString);
}

INT TString::Byte () const
{
	return Len () * sizeof (TCHAR);
}

BOOL TString::operator == (LPCTSTR lpctszString) const
{
	if (_lptszString == NULL && lpctszString == NULL) return TRUE;
	if (_lptszString == NULL || lpctszString == NULL) return FALSE;

	int size = max (Length (_lptszString), Length (lpctszString));
	int result = Compare (lpctszString, _lptszString, size);

	return result == 0 ? TRUE : FALSE;
}

BOOL TString::operator == (const TString& string) const
{
	int size = max (Length (_lptszString), string.Len ());
	int result = Compare (string.Ctr (), _lptszString, size);

	return result == 0 ? TRUE : FALSE;
}

BOOL TString::operator != (LPCTSTR lpctszString) const
{
	return !this->operator==(lpctszString);
}

BOOL TString::operator != (const TString& string) const
{
	return !this->operator==(string);
}

VOID TString::operator = (LPCTSTR lpctszString)
{
	this->Copy (lpctszString);
}

VOID TString::operator = (const TString& string)
{
	this->Copy (string.Ptr ());
}

TString& TString::operator << (LPCTSTR lpctszString)
{
	this->Add (lpctszString);

	return *this;
}

TString& TString::operator << (const TString& string)
{
	this->Add (string.Ptr ());

	return *this;
}

TString& TString::operator << (const int iValue)
{
	// UINT64_MAX 18446744073709551615
	TCHAR tszBuf[22];
	wsprintf (tszBuf, _T ("%d"), iValue);
	this->Add (tszBuf);

	return *this;
}

TString TString::operator + (LPCTSTR lpctszString)
{
	TString string (_lptszString);
	string += lpctszString;

	return string;
}

TString TString::operator + (TString& string)
{
	TString baseString (_lptszString);
	baseString += string;

	return baseString;
}

TString& TString::operator += (LPCTSTR lpctszString)
{
	this->Add (lpctszString);

	return *this;
}

TString& TString::operator += (const TString& string)
{
	this->Add (string.Ptr ());

	return *this;
}

bool TString::operator < (const TString& string) const
{
	int size = max (Length (_lptszString), string.Len ());
	int response = Compare (_lptszString, string.Ctr (), size);
	return response < 0 ? true : false;
};

TCHAR TString::operator [] (int index) const
{
	TCHAR ret;

	if (index <= this->Len ())
	{
		ret = _lptszString[index];
	}
	else
	{
		ret = '\0';
	}

	return ret;
}

TString& TString::TrimLeft ()
{
	int iStartPos = 0;
	int iLen = this->Len ();

	for (int i = 0; i < iLen; i++)
	{
		if (_lptszString[i] != ' ')
		{
			iStartPos = i;
			break;
		}
	}

	for (int i = 0; i < (iLen - iStartPos); i++)
	{
		_lptszString[i] = _lptszString[i + iStartPos];
	}

	_lptszString[(iLen - iStartPos)] = 0;

	return *this;
}

TString& TString::TrimRight ()
{
	int iStartPos = 0;
	int iLen = this->Len ();

	for (int i = (iLen - 1); i >= 0; i--)
	{
		if (_lptszString[i] != ' ')
		{
			iStartPos = i;
			break;
		}
	}

	for (int i = (iLen - 1); i > iStartPos; i--)
	{
		_lptszString[i] = 0;
	}

	return *this;
}

TString& TString::Trim ()
{
	this->TrimLeft ();
	this->TrimRight ();

	return *this;
}

TString TString::Substring (int startPos, int length)
{
	TString response1;
	int len = this->Len ();
	int targetLen = startPos + length;
	if (targetLen > len || targetLen == 0) return response1;

	int cutSize = length > 0 ? length + 1 : len - startPos + 1;
	TString response2 (cutSize + sizeof (TCHAR));
	LPTSTR p = lstrcpyn (response2.Ptr (), &_lptszString[startPos], cutSize);

	return response2;
}

TString TString::Replace (TString& charsBefore, TString& charsAfter)
{
	int findPos = this->Find (charsBefore);
	if (findPos == -1) return *this;

	TString returnString = this->Substring (0, findPos);
	returnString += charsAfter;
	TString wordsAfter = this->Substring (findPos + charsBefore.Len ());
	returnString += wordsAfter;

	return returnString;
}

TString TString::ReplaceAll (TString& charsBefore, TString& charsAfter)
{
	TString returnString;
	TString tempString = this->Ctr ();

	for (;;)
	{
		int findPos = tempString.Find (charsBefore);
		if (findPos == -1) return tempString;

		returnString = tempString.Substring (0, findPos);
		returnString += charsAfter;
		TString wordsAfter =
			tempString.Substring (findPos + charsBefore.Len ());
		returnString += wordsAfter;
		tempString = returnString;
	}

	return returnString;
}

skeleton::Array<TString> TString::Split (const TString& delimiters)
{
	int length = this->Len ();
	int beginPos = 0;
	TCHAR check;
	TCHAR delimiter;
	skeleton::Array<TString> response;

	for (int i = 0; i <= length; i++) // 最後のnullまで確認する
	{
		check = _lptszString[i];
		// delimitersも最後にnullを持っている
		for (int j = 0; j < delimiters.Len () + 1; j++)
		{
			delimiter = delimiters[j];
			if (check == delimiter)
			{
				TString item (i - beginPos);
				LPTSTR p = lstrcpyn (
					item.Ptr (), &_lptszString[beginPos], i - beginPos + 1);
				response.Add (item);
				beginPos = i + 1;
			}
		}
	}

	return response;
}

int TString::Find (TString keyword, int position)
{
	int response;
	int pos = 0;

	LPCTSTR pctszS1 = &_lptszString[position];
	LPCTSTR pctszS2 = keyword.Ptr ();

	int len = keyword.Len ();

	if (pctszS1 == NULL && pctszS2 == NULL)
	{
		return 0;
	}
#pragma warning(push)
#pragma warning(disable:28182)
	do
	{
		if (*pctszS1 == *pctszS2)
		{
			len--;
			pctszS2++;
		}
		else
		{
			pos++;
		}
		pctszS1++;

	} while (*pctszS1 != '\0' && *pctszS2 != '\0' && len != 0);
#pragma warning (pop)

	response = len == 0 ? pos : -1;

	return response == -1 ? -1 : response + position;
}

TString& TString::Format (LPCTSTR format, ...)
{
	va_list args;
	int     len;

	va_start (args, format);

	Clear ();
	len = (_vsctprintf (format, args) + 1) * sizeof (TCHAR);
	_lptszString = CreateMemory (len);
	_vstprintf_s (_lptszString, len, format, args);

	va_end (args);

	return *this;
}

BOOL TString::FromSJIS (LPCSTR lpcszSJIS)
{
	// 必要サイズを取得
	DWORD dwWideSize = ::MultiByteToWideChar (
		CP_ACP, MB_PRECOMPOSED, lpcszSJIS, -1, _lptszString, 0);
	TString response (dwWideSize);

	DWORD dwResponse = ::MultiByteToWideChar (
		CP_ACP, MB_PRECOMPOSED, lpcszSJIS, -1, response.Ptr (), dwWideSize);
	*this = response;

	return dwResponse > 0 ? TRUE : FALSE;
}

int TString::ToSJIS (LPSTR lpszSJIS, DWORD dwLen)
{
	return ::WideCharToMultiByte (
		CP_ACP, 0, _lptszString, -1, lpszSJIS, dwLen, NULL, NULL);
}

int TString::ParseInt () const
{
	int value = 0;
	int n = 0;

	auto power10 = [](int up)
	{
		if (up == 0) return 1;

		int response = 10;

		for (int i = 1; i < up; i++)
		{
			response *= 10;
		}

		return response;
	};

	for (int i = (Len () - 1); i >= 0; i--)
	{
		TCHAR t = _lptszString[i];
		if (t == '-')
		{
			value = -value;
			break;
		}
		value += (t - '0') * power10 (n++);
	}

	return value;
}

// private functions --------------------------------------------------

VOID TString::Clear ()
{
	if (_lptszString) delete[] _lptszString;
	_lptszString = NULL;
}

LPTSTR TString::CreateMemory (ULONGLONG size)
{
	LPTSTR lptszMemory = new TCHAR[size + sizeof (TCHAR)];
	ZeroMemory (lptszMemory, (size + sizeof (TCHAR)));
	return lptszMemory;
}

VOID TString::Copy (LPCTSTR lpctszString)
{
	if (lpctszString)
	{
		Clear ();
		DWORD dwLen = Length (lpctszString);
		_lptszString = CreateMemory (dwLen);
		LPTSTR lptszDst = _lptszString;
		LPCTSTR lpctszSrc = (LPTSTR)lpctszString;
		while ((*lpctszSrc) != '\0')
		{
			*lptszDst++ = *lpctszSrc++;
		}
		*lptszDst = NULL;
	}
}

VOID TString::Add (LPCTSTR lpctszString)
{
	if (lpctszString)
	{
		ULONGLONG length =
			static_cast<ULONGLONG>(this->Len ()) +
			Length (lpctszString) + sizeof (TCHAR);
		LPTSTR lptszNewBuffer = CreateMemory (length);
		if (_lptszString) lstrcat (lptszNewBuffer, _lptszString);
		if (lpctszString) lstrcat (lptszNewBuffer, lpctszString);
		Clear ();
		_lptszString = lptszNewBuffer;
	}
}

int TString::Compare (
	LPCTSTR lpctszString1, LPCTSTR lpctszString2, int size) const
{
	int response = 0;

	LPCTSTR lpctszS1 = lpctszString1;
	LPCTSTR lpctszS2 = lpctszString2;

	while (size-- > 0)
	{
		if ((response = *lpctszS1 - *lpctszS2++) != 0 || !*lpctszS1++)
		{
			break;
		}
	}

	return response;
}

INT TString::Length (LPCTSTR lpctszString) const
{
	INT i = 0;

	if (lpctszString != NULL)
	{
		while (lpctszString[i] != NULL)
		{
			i++;
		}
	}

	return i;
}
