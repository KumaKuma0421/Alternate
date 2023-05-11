// ----------------------------------------------------------------------------
//! @file	SharedMemory.h
//! @brief	共有メモリに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	SharedMemory
	//! @brief	共有メモリに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC SharedMemory : public HandleLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY SharedMemory ();

		//! @brief デストラクタ
		APIENTRY ~SharedMemory ();

		//! @brief 共有メモリの作成
		//! @param lpctszName 共有メモリの名称
		//! @param dwSize 共有メモリのサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Create (LPCTSTR lpctszName, DWORD dwSize);

		//! @brief 共有メモリのオープン
		//! @param lpctszName 共有メモリの名称
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Open (LPCTSTR lpctszName);

		//! @brief 共有メモリのマッピング開始
		//! @return LPVOID
		//! @retval 共有メモリの先頭アドレス
		LPVOID APIENTRY Mapped ();

		//! @brief 共有メモリのマッピング終了
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Unmapped ();

	private:
		LPVOID _lpvMapped;
	};
}