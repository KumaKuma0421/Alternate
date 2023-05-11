// ----------------------------------------------------------------------------
//! @file	RingBuffer.h
//! @brief	リングバッファに関するクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	RingBuffer
	//! @brief	リングバッファに関するクラス
	class DLL_DECLSPEC RingBuffer : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY RingBuffer ();

		//! @brief コンストラクタ
		//! @param size バッファサイズ
		APIENTRY RingBuffer (SIZE_T size);

		//! @brief デストラクタ
		APIENTRY ~RingBuffer ();

		//! @brief 初期化
		//! @param size バッファサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Init (SIZE_T size);

		//! @brief バッファのクリア
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Clear ();

		//! @brief バッファの書き込み
		//! @param lpbyBuffer 書き込みバッファ
		//! @param size バッファサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Write (LPBYTE lpbyBuffer, SIZE_T size);

		//! @brief バッファの読み込み
		//! @param lpbyBuffer 読み込みバッファ
		//! @param size バッファサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Read (LPBYTE lpbyBuffer, SIZE_T size);

		//! @brief 残りバッファサイズの取得
		//! @return SIZE_T 残りバッファ数
		SIZE_T APIENTRY Remain () const;

	private:
		LPBYTE _lpbyStartPoint;
		LPBYTE _lpbyEndPoint;
		SIZE_T _size;
		SIZE_T _remainSize;

		LPBYTE _lpbyReadPoint;
		LPBYTE _lpbyWritePoint;
	};
}