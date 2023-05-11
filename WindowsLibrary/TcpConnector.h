// ----------------------------------------------------------------------------
//! @file	TcpConnector.h
//! @brief	TCP通信に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	TcpConnector
	//! @brief	TCP通信に関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC TcpConnector : public SocketLibrary
	{
	public:
		//! @brief コンストラクタ
		//! @param socket 使用するソケット
		APIENTRY TcpConnector (SOCKET socket) { _socket = socket; };

		//! @brief TCP送信処理
		//! @param lpvBuf 送信データ
		//! @param dwSize 送信サイズ
		//! @return INT 送信データサイズ
		INT APIENTRY Send (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief TCP受信処理
		//! @param lpvBuf 受信データ
		//! @param dwSize 受信サイズ
		//! @param isBlocking ブロッキング待機するか否か
		//! @return INT 受信データサイズ
		INT APIENTRY Recv (
			LPVOID lpvBuf, DWORD dwSize, BOOL isBlocking = TRUE) const;

	protected:
		APIENTRY TcpConnector () { _socket = INVALID_SOCKET; };
	};
}