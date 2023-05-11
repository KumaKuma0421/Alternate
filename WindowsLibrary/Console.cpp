// ----------------------------------------------------------------------------
//! @file	Console.cpp
//! @brief	コンソールに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "Console.h"

using namespace alt;

Console::Console ()
	:HandleLibrary ()
{
	ZeroMemory (
		&_prevConsoleScreenBufferInfo, sizeof (_prevConsoleScreenBufferInfo));
}

Console::~Console ()
{
	::SetConsoleTextAttribute (
		_hObject, _prevConsoleScreenBufferInfo.wAttributes);
}

BOOL Console::Create () const
{
	return ::AllocConsole ();
}

BOOL Console::Init ()
{
	BOOL response = FALSE;

	_hObject = ::GetStdHandle (STD_OUTPUT_HANDLE);
	if (_hObject != INVALID_HANDLE_VALUE)
	{
		response = this->GetScreenBufferInfo (_prevConsoleScreenBufferInfo);
		if (response != FALSE)
		{
			response = this->SetColor (ForeColor::White, BackColor::Black);
		}
	}

	return response;
}

BOOL Console::SetColor (ForeColor foreColor, BackColor backColor) const
{
	WORD wAttribute = static_cast<int>(foreColor) | static_cast<int>(backColor);

	return ::SetConsoleTextAttribute (_hObject, wAttribute);
}

DWORD Console::Write (LPCTSTR lpctszString) const
{
	DWORD dwWritten;
	BOOL ret = ::WriteConsole (
		_hObject, lpctszString, lstrlen (lpctszString), &dwWritten, NULL);

	return dwWritten;
}

VOID Console::Format (LPCTSTR format, ...) const
{
	va_list args;
	INT ilen;
	LPTSTR lptszString;

	va_start (args, format);

	ilen = (_vsctprintf (format, args) + 1) * sizeof (TCHAR);
	lptszString = new TCHAR[ilen];

	if (lptszString != NULL)
	{
		_vstprintf_s (lptszString, ilen, format, args);
	}

	this->Write (lptszString);
	delete[] lptszString;

	va_end (args);
}

BOOL Console::SetTitle (LPCTSTR lpctszTitle) const
{
	return ::SetConsoleTitle (lpctszTitle);
}

BOOL Console::SetCursorPosition (SHORT x, SHORT y) const
{
	COORD coord = { x, y };

	return ::SetConsoleCursorPosition (_hObject, coord);
}

BOOL Console::SetBufferSize (SHORT x, SHORT y) const
{
	COORD coord = { x, y };
	DWORD dwNumbberOfCharsWritten;

	::WriteConsoleOutputCharacter (
		_hObject, _T (" "), sizeof (TCHAR), coord, &dwNumbberOfCharsWritten);

	return ::SetConsoleScreenBufferSize (_hObject, coord);
}

BOOL Console::SetWindowInfo (SHORT width, SHORT height) const
{
	SMALL_RECT rect = { 0, 0, width - 1, height - 1 };

	return ::SetConsoleWindowInfo (_hObject, TRUE, &rect);
}

BOOL Console::Clear () const
{
	BOOL ret = FALSE;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	do
	{
		ret = this->GetScreenBufferInfo (csbi);
		if (!ret) break;
		DWORD dwSize = csbi.dwSize.X * csbi.dwSize.Y;

		COORD coord = { 0, 0 };
		DWORD dwWritten;
		ret = FillConsoleOutputCharacter (
			_hObject, (TCHAR)' ', dwSize, coord, &dwWritten);
		if (!ret) break;

		ret = this->GetScreenBufferInfo (csbi);
		if (!ret) break;

		ret = FillConsoleOutputAttribute (
			_hObject, csbi.wAttributes, dwSize, coord, &dwWritten);
		if (!ret) break;

		ret = this->SetCursorPosition (0, 0);
	} while (false);

	return ret;
}

// private functions --------------------------------------------------

BOOL Console::GetScreenBufferInfo (CONSOLE_SCREEN_BUFFER_INFO& csbi) const
{
	return ::GetConsoleScreenBufferInfo (_hObject, &csbi);
}