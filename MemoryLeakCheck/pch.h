//! @file	pch.h
//! @brief	プリコンパイル済みヘッダー ファイルです。
//! @note   次のファイルは、その後のビルドのビルド パフォーマンスを向上させるため 1 回だけコンパイルされます。
//! 		コード補完や多くのコード参照機能などの IntelliSense パフォーマンスにも影響します。
//! 		ただし、ここに一覧表示されているファイルは、ビルド間でいずれかが更新されると、すべてが再コンパイルされます。
//! 		頻繁に更新するファイルをここに追加しないでください。追加すると、パフォーマンス上の利点がなくなります。
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#pragma once

// プリコンパイルするヘッダーをここに追加します
#include "framework.h"
#include <functional>
#include <map>

#ifdef _DEBUG
#include "crtdbg.h"
#define malloc(X) _malloc_dbg(X, _NORMAL_BLOCK, __FILE__, __LINE__) 
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define CRT_SET_DEBUG_FLAG _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF)
#else
#define CRT_SET_DEBUG_FLAG
#endif