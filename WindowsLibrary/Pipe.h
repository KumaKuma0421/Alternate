// ----------------------------------------------------------------------------
//! @file	Pipe.h
//! @brief	パイプに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	Pipe
	//! @brief	パイプに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC Pipe : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY Pipe ();

		//! @brief デストラクタ
		APIENTRY ~Pipe ();

		//! @brief パイプの作成
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Create ();

		//! @brief メッセージの確認
		//! @param lpvBuf 読み込みデータ
		//! @param dwSize 読み込みサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		DWORD APIENTRY Peek (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief メッセージの読み込み
		//! @param lpvBuf 読み込みデータ
		//! @param dwSize 読み込みサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		DWORD APIENTRY Read (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief メッセージの書き込み
		//! @param lpvBuf 書き込みデータ
		//! @param dwSize 書き込みサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Write (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief パイプのクローズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Close ();

		//! @brief 読み込み用ハンドルの取得
		//! @return ハンドル値
		HANDLE APIENTRY GetReadHandle () const { return _hRead; };

		//! @brief 書き込み用ハンドルの取得
		//! @return ハンドル値
		HANDLE APIENTRY GetWriteHandle () const { return _hWrite; };

	private:
		HANDLE _hRead;
		HANDLE _hWrite;
	};
}