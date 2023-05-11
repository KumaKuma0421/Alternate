//! @file	FreightMemoryLeakCheck.cpp
//! @brief	Freightのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#include "pch.h"
#include "FreightMemoryLeakCheck.h"

using namespace LeakCheck;

FreightMemoryLeakCheck::FreightMemoryLeakCheck ()
{

}

FreightMemoryLeakCheck::~FreightMemoryLeakCheck ()
{

}

BOOL FreightMemoryLeakCheck::Init ()
{
	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction ()
{
	DoAction1 ();
	DoAction2 ();
	DoAction3 ();
	DoAction4 ();
	DoAction5 ();
	DoAction6 ();

	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction1 ()
{
	OutputDebugString (_T ("FreightMemoryLeakCheck::DoAction1()---------->\n"));
	CRT_SET_DEBUG_FLAG;

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	alt::skeleton::Freight<CHAR> freight1 (&memory);
	alt::skeleton::Freight<CHAR> freight2 (&memory);
	alt::skeleton::Freight<CHAR> freight3 (&memory);

	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction2 ()
{
	OutputDebugString (_T ("FreightMemoryLeakCheck::DoAction2()---------->\n"));
	CRT_SET_DEBUG_FLAG;

	LPSTR lpszMessage = (LPSTR)"This is a sample text.";

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	alt::skeleton::Freight<CHAR> freight1 (lpszMessage, lstrlenA (lpszMessage), &memory);
	alt::skeleton::Freight<CHAR> freight2 (lpszMessage, lstrlenA (lpszMessage), &memory);
	alt::skeleton::Freight<CHAR> freight3 (lpszMessage, lstrlenA (lpszMessage), &memory);

	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction3 ()
{
	OutputDebugString (_T ("FreightMemoryLeakCheck::DoAction3()---------->\n"));
	CRT_SET_DEBUG_FLAG;

	LPSTR lpszMessage1 = (LPSTR)"This is a sample text No.1.";
	LPSTR lpszMessage2 = (LPSTR)"This is a sample text No.2. ...";
	LPSTR lpszMessage3 = (LPSTR)"This is a sample text No.3. ....";

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	alt::skeleton::Freight<CHAR> freight (&memory);

	freight.SetData (lpszMessage1, lstrlenA (lpszMessage1));
	freight.SetData (lpszMessage2, lstrlenA (lpszMessage2));
	freight.SetData (lpszMessage3, lstrlenA (lpszMessage3));

	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction4 ()
{
	OutputDebugString (_T ("FreightMemoryLeakCheck::DoAction4()---------->\n"));
	CRT_SET_DEBUG_FLAG;

	LPSTR lpszMessage1 = (LPSTR)"This is a sample text No.1.";
	LPSTR lpszMessage2 = (LPSTR)"This is a sample text No.2. ...";
	LPSTR lpszMessage3 = (LPSTR)"This is a sample text No.3. ....";

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	alt::skeleton::Freight<CHAR> freight1 (&memory);
	alt::skeleton::Freight<CHAR> freight2 (lpszMessage2, lstrlenA (lpszMessage2), &memory);
	alt::skeleton::Freight<CHAR> freight3 (&memory);

	freight1.SetData (lpszMessage1, lstrlenA (lpszMessage1));
	freight3.SetData (lpszMessage3, lstrlenA (lpszMessage3));

	freight1 = freight2;
	freight3 = freight2;

	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction5 ()
{
	OutputDebugString (_T ("FreightMemoryLeakCheck::DoAction5()---------->\n"));
	CRT_SET_DEBUG_FLAG;

	LPSTR lpszMessage1 = (LPSTR)"This is a sample text No.1.";

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	alt::skeleton::Freight<CHAR> freight1 (&memory);
	freight1.SetData (lpszMessage1, lstrlenA (lpszMessage1));

	alt::skeleton::Freight<CHAR> freight2 (freight1);

	int cmp = strcmp (freight1.GetData (), freight2.GetData ());
	0 == cmp ? _tprintf (_T ("ok.\n")) : _tprintf (_T ("NG. invalid seqno.\n"));

	return TRUE;
}

BOOL FreightMemoryLeakCheck::DoAction6 ()
{
	OutputDebugString (_T ("FreightMemoryLeakCheck::DoAction6()---------->\n"));
	CRT_SET_DEBUG_FLAG;

	LPSTR lpszMessage1 = (LPSTR)"This is a sample text No.1.";
	LPSTR lpszMessage2 = (LPSTR)"This is a sample text No.2. ...";
	LPSTR lpszMessage3 = (LPSTR)"This is a sample text No.3. ....";

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	alt::skeleton::Freight<CHAR> freight1 (&memory);
	freight1.SetData (lpszMessage1, lstrlenA (lpszMessage1));

	alt::skeleton::Freight<CHAR> freight2 (&memory);

	freight2 = freight1;

	int cmp = strcmp (freight1.GetData (), freight2.GetData ());
	0 == cmp ? _tprintf (_T ("ok.\n")) : _tprintf (_T ("NG. invalid seqno.\n"));

	return TRUE;
}