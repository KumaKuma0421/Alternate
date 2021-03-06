//! @file	CriticalSection.cpp
//! @brief	CriticalSectionに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#include "pch.h"
#include "CriticalSection.h"

using namespace alt;

CriticalSection::CriticalSection()
{
	::InitializeCriticalSection(&_criticalSection);
}

CriticalSection::~CriticalSection()
{
	::DeleteCriticalSection(&_criticalSection);
}

VOID CriticalSection::Enter()
{
	::EnterCriticalSection(&_criticalSection);
};

VOID CriticalSection::Leave()
{
	::LeaveCriticalSection(&_criticalSection);
};