// ----------------------------------------------------------------------------
//! @file	Exception.cpp
//! @brief	—áŠO‚ÉŠÖ‚·‚éƒNƒ‰ƒX
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#include "pch.h"
#include "Exception.h"

using namespace alt::ex;

// ----------------------------------------------------------------------------
Exception::Exception ()
{}

Exception::~Exception ()
{}

LPCTSTR Exception::GetReason () const
{
	return ALT_REASON_EXCEPTION;
}

// ----------------------------------------------------------------------------
InvalidArgumentException::InvalidArgumentException ()
{}

InvalidArgumentException::~InvalidArgumentException ()
{}

LPCTSTR InvalidArgumentException::GetReason () const
{
	return ALT_REASON_INVALID_ARGUMENT;
}

// ----------------------------------------------------------------------------
RuntimeException::RuntimeException ()
{}

RuntimeException::~RuntimeException ()
{}

LPCTSTR RuntimeException::GetReason () const
{
	return ALT_REASON_RUNTIME_ERROR;
}