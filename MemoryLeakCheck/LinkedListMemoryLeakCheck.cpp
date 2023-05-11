//! @file	LinkedListMemoryLeakCheck.cpp
//! @brief	LinkedListのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.06.28

#include "pch.h"
#include "LinkedListMemoryLeakCheck.h"

using namespace LeakCheck;

LinkedListMemoryLeakCheck::LinkedListMemoryLeakCheck ()
{

}

LinkedListMemoryLeakCheck::~LinkedListMemoryLeakCheck ()
{

}

BOOL LinkedListMemoryLeakCheck::Init ()
{
	return TRUE;
}

BOOL LinkedListMemoryLeakCheck::DoAction ()
{
	_tprintf (_T ("LinkedListMemoryLeakCheck::DoAction()\n"));
	CRT_SET_DEBUG_FLAG;

	alt::skeleton::LinkedList<Data> linkedList;

	for (int i = 1; i <= 500; i++)
	{
		Data data (i);
		linkedList.AddFirst (data);
		_tprintf (_T ("now Add() value=%d counter is %d\r"), data.Get (), i);
		Sleep (10);
	}
	_tprintf (_T ("\n"));

	for (int i = 1000; i >= 501; i--)
	{
		Data data (i);
		linkedList.AddLast (data);
		_tprintf (_T ("now Add() value=%d counter is %d\r"), data.Get (), i);
		Sleep (10);
	}
	_tprintf (_T ("\n"));

	alt::skeleton::LinkedList<Data> linkedListCopy = linkedList;
	linkedList.Clear ();

	for (int i = 0; i < linkedListCopy.Size (); i++)
	{
		Data* value = linkedListCopy.Get (i);
		ASSERT (i == value->Get ());
		_tprintf (_T ("now Get()=%d counter is %d\r"), value->Get (), i);
		Sleep (10);
	}
	_tprintf (_T ("\n"));

	int i = 1;
	for (auto itr : linkedListCopy)
	{
		ASSERT (i++ == itr);
		_tprintf (_T ("now Get()=%d counter is %d\r"), itr.Get(), i);
		Sleep (10);
	}

	i = 1;
	for (auto& itr : linkedListCopy)
	{
		ASSERT (i++ == itr);
		_tprintf (_T ("now Get()=%d counter is %d\r"), itr.Get(), i);
		Sleep (10);
	}

	linkedListCopy.Clear ();

	return TRUE;
}