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
#include <windows.h>
#include <tchar.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <functional>
#include <memory>
#include <time.h>
#include <timeapi.h>

#pragma comment(lib, "Winmm.lib")