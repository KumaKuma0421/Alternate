// ----------------------------------------------------------------------------
//! @file	HeapMemory.h
//! @brief	ヒープメモリに関するAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.21

#pragma once

#include "WindowsLibrary.h"
#include "Memory.h"

namespace alt
{
	//! @class BasicMemory
	//! @brief malloc/realloc/freeを使った標準的なメモリクラス
	class DLL_DECLSPEC BasicMemory : public WindowsLibrary, public skeleton::IMemory
	{
	public:
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
	};

	//! @class	HeapMemory
	//! @brief	ヒープメモリに関するAPIを集約したクラス
	class DLL_DECLSPEC HeapMemory : public HandleLibrary, public skeleton::IMemory
	{
	public:
		//! @brief コンストラクタ
		APIENTRY HeapMemory ();

		//! @brief コピーコンストラクタ
		APIENTRY HeapMemory (const HeapMemory& base);

		//! @brief デストラクタ
		APIENTRY ~HeapMemory ();

		//! @brief ヒープ領域の取得
		//! @param initialSize 初期サイズ
		//! @param maximumSize 最大サイズ
		//! @param dwOptions オプション
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		//! @note 初期サイズが０の場合、１ページをコミットします。
		//! @note 最大サイズが０でない場合、ピープサイズは固定されます。
		BOOL APIENTRY Create (
			SIZE_T initialSize = 0, SIZE_T maximumSize = 0,
			DWORD dwOptions = 0);

		//! @brief システムヒープ領域の取得
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Open ();

		//! @brief 取得したヒープ領域の解放
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Close ();

		//! @brief 取得したヒープ領域の整理
		//! @return SIZE_T
		//! @retval 整理した後のメモリサイズ
		SIZE_T APIENTRY Compact () const;

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
		SIZE_T APIENTRY BoundarySize (SIZE_T bytes) const;

		INT _iBoundary;
		BOOL _bCreateHeap;
	};
}