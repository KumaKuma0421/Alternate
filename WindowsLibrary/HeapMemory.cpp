// ----------------------------------------------------------------------------
//! @file	HeapMemory.cpp
//! @brief	ヒープメモリに関するAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.21

#include "pch.h"
#include "HeapMemory.h"

using namespace alt;

// BasicMemory --------------------------------------------------------

LPVOID BasicMemory::Allocate (SIZE_T bytes, DWORD dwFlags)
{
	LPVOID lpvMem = ::malloc (bytes);

	if (lpvMem != NULL && dwFlags == MEM_ZERO) ZeroMemory (lpvMem, bytes);

	return lpvMem;
}

LPVOID BasicMemory::Reallocate (LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags)
{
	size_t previousSize = _msize (lpvMem);
	size_t currentSize = bytes;
	LPBYTE lpbyNewMem = (LPBYTE)::realloc (lpvMem, bytes);
	if (lpbyNewMem != NULL && currentSize > previousSize && dwFlags == MEM_ZERO)
		ZeroMemory (&lpbyNewMem[previousSize], (currentSize - previousSize));

	return lpbyNewMem;
};

BOOL BasicMemory::Free (LPVOID lpvMem)
{
	::free (lpvMem);

	return TRUE;
};

// HeapMemory ---------------------------------------------------------

HeapMemory::HeapMemory ()
{
	_hObject = INVALID_HANDLE_VALUE;

	_iBoundary = sizeof (INT);
	_bCreateHeap = FALSE;
}

HeapMemory::~HeapMemory ()
{
	if (_hObject != INVALID_HANDLE_VALUE) Close ();
}

BOOL HeapMemory::Create (
	SIZE_T initialSize, SIZE_T maximumSize, DWORD dwOptions)
{
	// ここの丁寧な説明はMicrosoftのサイトの右クリック和訳です。

	// initialSizeパラメーターが 0 の場合、関数は 1 ページをコミットします。
	// 値は、システム・ページ・サイズの倍数に切り上げられます。

	// maximumSize がゼロでない場合、ヒープ・サイズは固定されており、
	// 最大サイズを超えて拡張することはできません。また、ヒープから
	// 割り振ることができる最大のメモリー・ブロックは、
	// 32 ビット・プロセスでは 512 KB よりわずかに小さく、
	// 64 ビット・プロセスでは 1,024 KB よりわずかに小さくなります。
	// より大きなブロックを割り振る要求は、ヒープの最大サイズがブロックを
	// 収容するのに十分な大きさであっても失敗します。

	// maximumSize が 0 の場合、ヒープのサイズが大きくなる可能性があります。
	// ヒープのサイズは、使用可能なメモリによってのみ制限されます。
	// 固定サイズのヒープの制限より大きいメモリー・ブロックを割り振る要求は、
	// 自動的に失敗することはありません。代わりに、システムは VirtualAlloc 関数を
	// 呼び出して、大きなブロックに必要なメモリを取得します。
	// 大きなメモリ ブロックを割り当てる必要があるアプリケーションでは、
	// maximumSize を 0 に設定する必要があります。

	_bCreateHeap = true;

	_hObject = ::HeapCreate (dwOptions, initialSize, maximumSize);

	return _hObject == NULL ? FALSE : TRUE;
}

BOOL HeapMemory::Open ()
{
	_hObject = ::GetProcessHeap ();
	_bCreateHeap = false;

	return _hObject == NULL ? FALSE : TRUE;
}

BOOL HeapMemory::Close ()
{
	BOOL ret = TRUE;

	if (_bCreateHeap)
	{
		_bCreateHeap = false;
		ret = ::HeapDestroy (_hObject);
	}

	_hObject = INVALID_HANDLE_VALUE; // HeapのハンドルはCloseHandle()不要

	return ret;
}

SIZE_T HeapMemory::Compact () const
{
	return ::HeapCompact (_hObject, 0);
}

LPVOID HeapMemory::Allocate (SIZE_T bytes, DWORD dwFlags)
{
	return ::HeapAlloc (_hObject, dwFlags, this->BoundarySize (bytes));
}

LPVOID HeapMemory::Reallocate (LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags)
{
	// HeapReAlloc()における、MEM_ZEROの振る舞いについて
	//
	// 再割り振り要求がより大きいサイズに対するものである場合、
	// 元のサイズを超えるメモリーの追加領域はゼロに初期化されます。
	// 元のサイズまでのメモリブロックの内容は影響を受けません。
	return ::HeapReAlloc (_hObject, dwFlags, lpvMem, bytes);
}

BOOL HeapMemory::Free (LPVOID lpvMem)
{
	return ::HeapFree (_hObject, 0, lpvMem);
}

// private functions --------------------------------------------------

SIZE_T HeapMemory::BoundarySize (SIZE_T bytes) const
{
	return _iBoundary + (bytes - (bytes & (_iBoundary - 1LL)));
}