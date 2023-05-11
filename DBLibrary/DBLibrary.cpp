//! @file	DBLibrary.cpp
//! @brief	DB APIを集約するクラスの基底クラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#include "pch.h"
#include "DBLibrary.h"

using namespace alt;

BOOL APIENTRY DllMain (HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved)
{
	switch (dwReasonForCall)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}

	return TRUE;
}

DBLibrary::DBLibrary ()
{

}

DBLibrary::~DBLibrary ()
{

}