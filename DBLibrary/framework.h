//! @file	framework.h
//! @brief	このプロジェクトのポーティングに関する情報
//! @author	kumakuma0421@gmail.com
//! @date	2020.4.4

#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する

#ifdef DBLIBRARY_EXPORTS
#pragma message("DLL EXPORTING...")
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllimport)
#endif
#pragma comment(lib, "DBLibrary")
#pragma message("DBLibrary is now importing...")
#endif

#include <windows.h>

#pragma comment(lib, "sqlite3.lib")