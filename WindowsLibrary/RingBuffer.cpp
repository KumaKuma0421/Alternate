//! @file	RingBuffer.cpp
//! @brief	リングバッファに関するクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#include "pch.h"
#include "RingBuffer.h"

using namespace alt;

RingBuffer::RingBuffer()
	:WindowsLibrary()
{
	_lpbyStartPoint = nullptr;
	_lpbyEndPoint = nullptr;
	_size = 0;
	_remainSize = 0;
	_lpbyReadPoint = nullptr;
	_lpbyWritePoint = nullptr;
}

RingBuffer::RingBuffer(size_t size)
	:RingBuffer()
{
	this->Init(size);
}

RingBuffer::~RingBuffer()
{
	if (_lpbyStartPoint != nullptr) delete[] _lpbyStartPoint;
}

BOOL RingBuffer::Init(size_t size)
{
	_lpbyStartPoint = new BYTE[size];
	_lpbyEndPoint = _lpbyStartPoint + size;
	_size = size;
	_remainSize = 0;
	_lpbyReadPoint = _lpbyStartPoint;
	_lpbyWritePoint = _lpbyStartPoint;

	return TRUE;
}

BOOL RingBuffer::Clear()
{
	delete[] _lpbyStartPoint;
	_lpbyStartPoint = new BYTE[_size];
	_lpbyEndPoint = _lpbyStartPoint + _size;
	_remainSize = 0;
	_lpbyReadPoint = nullptr;
	_lpbyWritePoint = nullptr;

	return TRUE;
}

BOOL RingBuffer::Write(LPBYTE lpbyBuffer, size_t size)
{
	LPBYTE lpbyWritePointEnd = _lpbyWritePoint + size;

	// 書き込み量がバッファサイズを超えてはいけない。
	if ((_remainSize + size) > _size) return FALSE;

	if (lpbyWritePointEnd <= _lpbyEndPoint)
	{
		// リングバッファの内部で完結
		CopyMemory(_lpbyWritePoint, lpbyBuffer, size);
		_lpbyWritePoint = lpbyWritePointEnd;
	}
	else
	{
		// 書き込みが最大バッファを超える場合は、分割して書き込む。
		size_t writeSize1 = _lpbyEndPoint - _lpbyWritePoint;
		CopyMemory(_lpbyWritePoint, lpbyBuffer, writeSize1);

		size_t writeSize2 = size - writeSize1;
		CopyMemory(_lpbyStartPoint, &lpbyBuffer[writeSize1], writeSize2);

		_lpbyWritePoint = _lpbyStartPoint + writeSize2;
	}

	_remainSize += size;

	return TRUE;
}

BOOL RingBuffer::Read(LPBYTE lpbyBuffer, size_t size)
{
	LPBYTE lpbyReadPointStart = _lpbyReadPoint;
	LPBYTE lpbyReadPointEnd = _lpbyReadPoint + size;

	// 読み込み量が書き込み量を超えてはいけない。
	if (size > _remainSize) return FALSE;

	if (lpbyReadPointEnd <= _lpbyEndPoint)
	{
		// リングバッファの内部で完結
		CopyMemory(lpbyBuffer, _lpbyReadPoint, size);
		_lpbyReadPoint = lpbyReadPointEnd;
	}
	else
	{
		// 読み込みが最大バッファを超える場合は、分割して読み込む。
		size_t readSize1 = _lpbyEndPoint - _lpbyReadPoint;
		CopyMemory(lpbyBuffer, _lpbyReadPoint, readSize1);

		size_t readSize2 = size - readSize1;
		CopyMemory(&lpbyBuffer[readSize1], _lpbyStartPoint, readSize2);

		_lpbyReadPoint = _lpbyStartPoint + readSize2;
	}

	_remainSize -= size;

	return TRUE;
}

size_t RingBuffer::Remain()
{
	return _remainSize;
}