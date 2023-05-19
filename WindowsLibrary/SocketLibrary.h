// ----------------------------------------------------------------------------
//! @file	SocketLibrary.h
//! @brief	ソケットに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2023.5.13

#pragma once

#include <WinSock2.h>
#include "WindowsLibrary.h"

namespace alt
{
	//! @class	SocketLibrary
	//! @brief	WinSockを使用するAPIを集約したクラス
	class DLL_DECLSPEC SocketLibrary : public WindowsLibrary
	{
	public:
		//! @brief	コンストラクタ
		//! @note	_socketを INVALID_SOCKET で初期化します。
		APIENTRY SocketLibrary ();

		//! @brief	デストラクタ
		//! @note	管理しているソケットをクローズします。
		APIENTRY ~SocketLibrary ();

		//! @brief		ソケットが使用されていた場合、クローズします。
		//! @return		ソケットクローズが成功したか否か。
		//! @retval		closesocket()の戻り値
		//! @retval		成功(0) ソケットではない(WSAENOTSOCK)
		//! @details	WinSock APIの戻り値はネットで確認
		int APIENTRY Close ();

		//! @brief		WinSock API呼び出し時にエラーとなった時、
		//!				エラーの詳細を返します。
		//! @return		WinSock APIのエラー時のエラー番号
		//! @retval		DWORD 詳細はネットで。
		//! @sa			http://chokuto.ifdef.jp/advanced/prm/winsock_error_code.html
		//! @sa         https://learn.microsoft.com/ja-jp/windows/win32/winsock/windows-sockets-error-codes-2
		//! @details	WSAGetLastError()を使用します。
		DWORD APIENTRY GetErrNo () const;

		//! @brief		KeepAlive送出タイミング、間隔をセッション個別に設定します。
		//! @param[in]	onoff (0:OFF 1:ON)
		//! @param[in]	time 無通信となった時からKeepAlive送出開始までの時間(ミリ秒)
		//! @param[in]	interval 切断検知後にリトライ送出する時のインターバル
		//! @return		WinSock APIのエラー時のエラー番号
		//! @retval		DWORD 詳細はネットで。
		//! @details	SetSockOption()で設定されるKeepAlive送出設定はレジストリの
		//! 			設定値(既定値は２時間後)に従います。この関数により、KeepAlive
		//! 			の設定タイミングをセッション個別に設定することができます。
		BOOL APIENTRY SetKeepAliveValue (
			ULONG onoff, ULONG time, ULONG interval) const;

	protected:
		int APIENTRY WSAIoctl (
			DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD dwInBuffer,
			LPVOID lpvOutBuffer, DWORD dwOutBuffer, LPDWORD lpdwBytesReturned,
			LPWSAOVERLAPPED lpOverlapped,
			LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine) const;

		//! このクラスで管理するソケットオブジェクト
		SOCKET _socket;
	};
}