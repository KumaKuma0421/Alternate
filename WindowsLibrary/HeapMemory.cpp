// ----------------------------------------------------------------------------
//! @file	HeapMemory.cpp
//! @brief	�q�[�v�������Ɋւ���API���W�񂵂��N���X
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
	// �����̒��J�Ȑ�����Microsoft�̃T�C�g�̉E�N���b�N�a��ł��B

	// initialSize�p�����[�^�[�� 0 �̏ꍇ�A�֐��� 1 �y�[�W���R�~�b�g���܂��B
	// �l�́A�V�X�e���E�y�[�W�E�T�C�Y�̔{���ɐ؂�グ���܂��B

	// maximumSize ���[���łȂ��ꍇ�A�q�[�v�E�T�C�Y�͌Œ肳��Ă���A
	// �ő�T�C�Y�𒴂��Ċg�����邱�Ƃ͂ł��܂���B�܂��A�q�[�v����
	// ����U�邱�Ƃ��ł���ő�̃������[�E�u���b�N�́A
	// 32 �r�b�g�E�v���Z�X�ł� 512 KB ���킸���ɏ������A
	// 64 �r�b�g�E�v���Z�X�ł� 1,024 KB ���킸���ɏ������Ȃ�܂��B
	// ���傫�ȃu���b�N������U��v���́A�q�[�v�̍ő�T�C�Y���u���b�N��
	// ���e����̂ɏ\���ȑ傫���ł����Ă����s���܂��B

	// maximumSize �� 0 �̏ꍇ�A�q�[�v�̃T�C�Y���傫���Ȃ�\��������܂��B
	// �q�[�v�̃T�C�Y�́A�g�p�\�ȃ������ɂ���Ă̂ݐ�������܂��B
	// �Œ�T�C�Y�̃q�[�v�̐������傫���������[�E�u���b�N������U��v���́A
	// �����I�Ɏ��s���邱�Ƃ͂���܂���B����ɁA�V�X�e���� VirtualAlloc �֐���
	// �Ăяo���āA�傫�ȃu���b�N�ɕK�v�ȃ��������擾���܂��B
	// �傫�ȃ����� �u���b�N�����蓖�Ă�K�v������A�v���P�[�V�����ł́A
	// maximumSize �� 0 �ɐݒ肷��K�v������܂��B

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

	_hObject = INVALID_HANDLE_VALUE; // Heap�̃n���h����CloseHandle()�s�v

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
	// HeapReAlloc()�ɂ�����AMEM_ZERO�̐U�镑���ɂ���
	//
	// �Ċ���U��v�������傫���T�C�Y�ɑ΂�����̂ł���ꍇ�A
	// ���̃T�C�Y�𒴂��郁�����[�̒ǉ��̈�̓[���ɏ���������܂��B
	// ���̃T�C�Y�܂ł̃������u���b�N�̓��e�͉e�����󂯂܂���B
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