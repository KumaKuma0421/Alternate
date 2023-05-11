// ----------------------------------------------------------------------------
//! @file	Memory.h
//! @brief	メモリに関するインターフェースを定義したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.05

#pragma once

#include "WindowsLibrary.h"

#define MEM_ZERO HEAP_ZERO_MEMORY

namespace alt
{
	namespace skeleton
	{
		//! @class IMemory
		//! @brief メモリの獲得・解放のインターフェースクラス
		class DLL_DECLSPEC IMemory
		{
		public:
			//! @brief メモリの取得
			//! @param bytes 取得サイズ
			//! @param dwFlags 取得メモリへの設定
			//! @return LPVOID
			//! @retval 取得したメモリのアドレス
			virtual LPVOID APIENTRY Allocate (SIZE_T bytes, DWORD dwFlags = MEM_ZERO) = 0;

			//! @brief 取得メモリの拡大
			//! @param lpvMem 拡大させるアドレス
			//! @param bytes 拡大サイズ
			//! @param dwFlags 再取得メモリへの設定
			//! @return LPVOID
			//! @retval 再取得したメモリのアドレス
			virtual LPVOID APIENTRY Reallocate (
				LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags = MEM_ZERO) = 0;

			//! @brief メモリの返却
			//! @param lpvMem 返却するアドレス
			//! @return BOOL
			//! @retval TRUE(成功)
			//! @retval FALSE(失敗)
			virtual BOOL APIENTRY Free (LPVOID lpvMem) = 0;
		};
	}
}