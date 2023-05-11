// ----------------------------------------------------------------------------
//! @file	NamedPipe.h
//! @brief	名前付きパイプに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

#define PIPE_NAME_FORMAT _T("\\\\.\\pipe\\%s")

namespace alt
{
	//! @class	NamedPipe
	//! @brief	名前付きパイプに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC NamedPipe : public ReadWriteHandleLibrary
	{
	public:
		//! @brief		パイプを作成します。
		//! @param[in]	lpctszName パイプ名
		//! @param[in]	bStreamMode ストリームモードがバイトモードか。
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		//! @details	パイプ名は内部で正式名に補完します。
		BOOL APIENTRY Create (LPCTSTR lpctszName, BOOL bStreamMode);

		//! @brief	サーバー側でクライアント接続を待機します。
		//! @return	BOOL
		//! @retval	TRUE(成功)
		//! @retval	FALSE(失敗)
		BOOL APIENTRY Connect () const;

		//! @brief	サーバー側でクライアント接続を切断します。
		//! @return	BOOL
		//! @retval	TRUE(成功)
		//! @retval	FALSE(失敗)
		BOOL APIENTRY Disconnect () const;

		//! @brief		パイプをオープンします。
		//! @param[in]	lpctszName パイプ名
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		//! @details	パイプ名は内部で正式名に補完します。
		BOOL APIENTRY Open (LPCTSTR lpctszName);

		//! @brief		クライアント側でサーバー接続を待機します。
		//! @param[in]	lpctszName	パイプ名
		//! @param[in]	dwTimeout	タイムアウト時間(ミリ秒)
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL APIENTRY Wait (
			LPCTSTR lpctszName,
			DWORD dwTimeout = NMPWAIT_USE_DEFAULT_WAIT) const;

		//! @brief		メッセージ内容を取り出さずに読み取ります。
		//! @param[in]	lpvData	読み取りバッファ
		//! @param[in]	dwSize	読み取りバッファサイズ
		//! @return		DWORD
		//! @retval		読み取りバイトサイズ(成功)
		//! @retval		-1(失敗)
		DWORD APIENTRY PeekData (LPVOID lpvData, DWORD dwSize) const;

		//! @brief	バッファ内に残っているメッセージサイズを取得します。
		//! @return	DWORD
		//! @retval	読み取りバイトサイズ(成功)
		//! @retval	-1(失敗)
		DWORD APIENTRY PeekRemain () const;
	};
}