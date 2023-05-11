//! @file	BinderMemoryLeakCheck.cpp
//! @brief	Binderのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.14

#include "pch.h"
#include "BinderMemoryLeakCheck.h"

using namespace LeakCheck;

BinderMemoryLeakCheck::BinderMemoryLeakCheck ()
{

}

BinderMemoryLeakCheck::~BinderMemoryLeakCheck ()
{

}

BOOL BinderMemoryLeakCheck::Init ()
{
	return TRUE;
}

BOOL BinderMemoryLeakCheck::DoAction ()
{
	DoAction1 ();
	DoAction2 ();
	DoAction3 ();
	DoAction4 ();

	return TRUE;
}

BOOL BinderMemoryLeakCheck::DoAction1 ()
{
	_tprintf (_T ("BinderMemoryLeakCheck::DoAction1()--------->\n"));
	CRT_SET_DEBUG_FLAG;

	typedef alt::skeleton::Freight<CHAR> CharFreight;
	typedef alt::skeleton::Binder<CharFreight> CharBinder;

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	CharFreight* freight = new CharFreight ((LPSTR)"ABCDEF", 6, &memory);

	CharBinder binder;
	binder.Set (freight);

	return TRUE;
}

BOOL BinderMemoryLeakCheck::DoAction2 ()
{
	_tprintf (_T ("BinderMemoryLeakCheck::DoAction2()--------->\n"));
	CRT_SET_DEBUG_FLAG;

	typedef alt::skeleton::Freight<CHAR> CharFreight;
	typedef alt::skeleton::Binder<CharFreight> CharBinder;

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	CharFreight* freight = new CharFreight ((LPSTR)"ABCDEF", 6, &memory);

	CharBinder binder (freight);

	return TRUE;
}

BOOL BinderMemoryLeakCheck::DoAction3 ()
{
	_tprintf (_T ("BinderMemoryLeakCheck::DoAction3()--------->\n"));
	CRT_SET_DEBUG_FLAG;

	typedef alt::skeleton::Freight<CHAR> CharFreight;
	typedef alt::skeleton::Binder<CharFreight, alt::HeapMemory*> CharBinder;

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	CharBinder binder;
	binder.New (&memory);
	auto freight = binder.Get ();
	freight->SetData ((LPSTR)"ABCDEF", 6);

	return TRUE;
}

BOOL BinderMemoryLeakCheck::DoAction4 ()
{
	_tprintf (_T ("BinderMemoryLeakCheck::DoAction4()--------->\n"));
	CRT_SET_DEBUG_FLAG;

	typedef alt::skeleton::Freight<CHAR> CharFreight;
	typedef alt::skeleton::Binder<CharFreight, CHAR*, size_t, alt::HeapMemory*> CharBinder;

	alt::HeapMemory memory;
	BOOL ret = memory.Create ();
	if (!ret) return FALSE;

	CharBinder binder;
	binder.New ((CHAR*)"ABCDEF", 6, &memory);
	auto freight = binder.Get ();
	auto data = binder->GetData ();

	return TRUE;
}