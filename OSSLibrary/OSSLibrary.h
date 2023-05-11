//! @file	OSSLibrary.h
//! @brief	Open Source Software を集約したプロジェクトファイル
//! @note	DllMain()のプロトタイプもここで宣言しています。
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.07

#pragma once

#include <Windows.h>
#include <tchar.h>

#ifdef OSSLIBRARY_EXPORTS
#pragma message("DLL EXPORTING...")
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllimport)
#endif
#pragma comment(lib, "OSSLibrary")
#pragma message("OSSLibrary is now importing...")
#endif

BOOL APIENTRY DllMain (HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved);

namespace oss
{
	//! @class	OSSLibrary
	//! @brief	プロジェクト共通基底クラス
	class DLL_DECLSPEC OSSLibrary
	{
	public:
		OSSLibrary ();
		virtual ~OSSLibrary ();
	};
}