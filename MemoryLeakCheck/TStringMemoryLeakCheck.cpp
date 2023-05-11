//! @file	TStringMemoryLeakCheck.cpp
//! @brief	TStringのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#include "pch.h"
#include "TStringMemoryLeakCheck.h"

using namespace LeakCheck;

TStringMemoryLeakCheck::TStringMemoryLeakCheck ()
{

}

TStringMemoryLeakCheck::~TStringMemoryLeakCheck ()
{

}

BOOL TStringMemoryLeakCheck::Init ()
{
	return TRUE;
}

BOOL TStringMemoryLeakCheck::DoAction ()
{
	this->DoAction1 ();
	this->DoAction2 ();
	this->DoAction3 ();
	this->DoAction4 ();

	return TRUE;
}

BOOL TStringMemoryLeakCheck::DoAction1 ()
{
	OutputDebugString (_T ("TStringMemoryLeakCheck::DoAction1()\n"));
	CRT_SET_DEBUG_FLAG;

	alt::TString string1;
	alt::TString string2 (_T ("Check_TString01"));
	alt::TString string3 (string2);
	alt::TString string4 (MAX_PATH);

	return TRUE;
}

BOOL TStringMemoryLeakCheck::DoAction2 ()
{
	OutputDebugString (_T ("TStringMemoryLeakCheck::DoAction2()\n"));
	CRT_SET_DEBUG_FLAG;

	alt::TString* string1 = new alt::TString ();
	alt::TString* string2 = new alt::TString (_T ("Check_TString02"));
	alt::TString* string3 = new alt::TString (*string2);
	alt::TString* string4 = new alt::TString (MAX_PATH);

	delete string1;
	delete string2;
	delete string3;
	delete string4;

	return TRUE;
}

BOOL TStringMemoryLeakCheck::DoAction3 ()
{
	OutputDebugString (_T ("TStringMemoryLeakCheck::DoAction3()\n"));
	CRT_SET_DEBUG_FLAG;

	alt::TString string1;
	alt::TString string2;

	ASSERT (string1 == nullptr);
	ASSERT (string1 == string2);

	alt::TString string3 (_T ("string3"));
	alt::TString string4 (_T ("string4"));

	ASSERT (string3 == _T ("string3"));
	ASSERT (string3 != string4);

	return TRUE;
}

BOOL TStringMemoryLeakCheck::DoAction4 ()
{
	OutputDebugString (_T ("TStringMemoryLeakCheck::DoAction4()\n"));
	CRT_SET_DEBUG_FLAG;

	for (int i = 0; i < 100; i++)
	{
		alt::TString message1;
		message1.Format (_T ("TEST %d\n"), i);
	}

	alt::TString before = _T ("TEST");
	alt::TString after = _T ("SAMPLE");

	for (int i = 0; i < 100; i++)
	{
		alt::TString message2;
		message2.Format (_T ("TEST %d"), i);
		message2.Replace (before, after);
	}

	for (int i = 0; i < 100; i++)
	{
		alt::TString message2;
		message2.Format (_T ("TEST TEST TEST TEST TEST %d"), i);
		message2.ReplaceAll (before, after);
	}


	return TRUE;
}