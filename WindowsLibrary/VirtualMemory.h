// ----------------------------------------------------------------------------
//! @file	VirtualMemory.h
//! @brief	仮想メモリに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "Memory.h"

namespace alt
{
	//! @class VirtualMemory1
	//! @brief 仮想メモリの取得・解放を簡易的に実装したクラス
	class DLL_DECLSPEC VirtualMemory1 : public WindowsLibrary, public skeleton::IMemory
	{
	public:
		//! @brief コンストラクタ
		APIENTRY VirtualMemory1 ();

		//! @brief メモリの取得
		//! @param bytes 取得サイズ
		//! @param dwFlags 取得メモリへの設定
		//! @return LPVOID
		//! @retval 取得したメモリのアドレス
		virtual LPVOID APIENTRY Allocate (SIZE_T bytes, DWORD dwFlags = MEM_ZERO);

		//! @brief 取得メモリの拡大
		//! @param lpvMem 拡大させるアドレス
		//! @param bytes 拡大サイズ
		//! @param dwFlags 再取得メモリへの設定
		//! @return LPVOID
		//! @retval 再取得したメモリのアドレス
		virtual LPVOID APIENTRY Reallocate (
			LPVOID lpvMem, SIZE_T bytes, DWORD dwFlags = MEM_ZERO);

		//! @brief メモリの返却
		//! @param lpvMem 返却するアドレス
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		virtual BOOL APIENTRY Free (LPVOID lpvMem);

	private:
		SIZE_T _currentSize;
	};

	//! @class	VirtualMemory
	//! @brief	仮想メモリに関するクラス
	class DLL_DECLSPEC VirtualMemory : public WindowsLibrary
	{
	public:
		//! @brief 仮想メモリの予約
		//! @param size 予約サイズ
		//! @return LPVOID 予約メモリアドレス
		LPVOID APIENTRY Reserve (SIZE_T size) const;

		//! @brief 仮想メモリの予約開放
		//! @param lpvMem 予約メモリアドレス
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Release (LPVOID lpvMem) const;

		//! @brief 予約メモリの確定
		//! @param lpvMem 予約したメモリの先頭アドレス
		//! @param size 予約サイズ
		//! @return LPVOID 確定したメモリの先頭アドレス
		LPVOID APIENTRY Commit (LPVOID lpvMem, SIZE_T size) const;

		//! @brief 確定メモリの解除
		//! @param lpvMem 確定したメモリの先頭アドレス
		//! @param size 確定サイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Decommit (LPVOID lpvMem, SIZE_T size) const;

		//! @brief メモリのロック
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Lock (LPVOID lpvMem, SIZE_T size) const;

		//! @brief メモリのアンロック
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Unlock (LPVOID lpvMem, SIZE_T size) const;
	};
}