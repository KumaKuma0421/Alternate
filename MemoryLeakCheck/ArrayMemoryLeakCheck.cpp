//! @file	ArrayMemoryLeakCheck.cpp
//! @brief	Arrayのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#include "pch.h"
#include "ArrayMemoryLeakCheck.h"

using namespace LeakCheck;

ArrayMemoryLeakCheck::ArrayMemoryLeakCheck ()
{

}

ArrayMemoryLeakCheck::~ArrayMemoryLeakCheck ()
{

}

BOOL ArrayMemoryLeakCheck::Init ()
{
	return TRUE;
}

BOOL ArrayMemoryLeakCheck::DoAction ()
{
	_tprintf (_T ("ArrayMemoryLeakCheck::DoAction()\n"));
	CRT_SET_DEBUG_FLAG;

	alt::skeleton::Array<Data> array;

	for (int i = 0; i < 1000; i++)
	{
		Data data (i);
		array.Add (data);
		_tprintf (_T ("now Add() value=%d counter is %d\r"), data.Get (), i);
		Sleep (10);
	}
	_tprintf (_T ("\n"));

	alt::skeleton::Array<Data> arrayCopy = array;
	array.Clear ();

	for (int i = 0; i < arrayCopy.Size (); i++)
	{
		Data* value = arrayCopy.Get (i);
		ASSERT (i == value->Get ());
		_tprintf (_T ("now Get()=%d counter is %d\r"), value->Get (), i);
		Sleep (10);
	}
	_tprintf (_T ("\n"));

	arrayCopy.Clear ();

	return TRUE;
}