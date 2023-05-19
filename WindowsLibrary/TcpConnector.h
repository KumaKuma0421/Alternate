// ----------------------------------------------------------------------------
//! @file	TcpConnector.h
//! @brief	TCP通信に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "SocketLibrary.h"
#include <ws2ipdef.h>

namespace alt
{
	//! @class	TcpConnector
	//! @brief	TCP通信に関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC TcpConnector : public SocketLibrary
	{
	public:
		//! @brief コンストラクタ
		//! @param socket ソケット識別子
		//! @param lpctszAcceptIPAddress 対向のIPアドレス
		//! @param wAcceptPort 対向のポート番号
		APIENTRY TcpConnector(
			SOCKET socket, LPCTSTR lpctszAcceptIPAddress, u_short wAcceptPort);

		//! @brief 受け入れ先のIPアドレスを取得
		//! @return LPCTSTR 受入先IPアドレス
		LPCTSTR GetAcceptedAddress () { return _tszAcceptedIPAddress; };

		//! @brief 受入先のポート番号を取得
		//! @return USHORT 受入先ポート番号
		USHORT GetAcceptedPortNo () { return _wAcceptedPort; };

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
			LPVOID lpvBuf, DWORD dwSize, BOOL isBlocking = TRUE);

		//! @brief Recv()待機中のキャンセル処理
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL CancelRecv ();

	protected:
		APIENTRY TcpConnector () = delete;
		
		TCHAR _tszAcceptedIPAddress[INET_ADDRSTRLEN];
		USHORT _wAcceptedPort;
		WSAOVERLAPPED _RecvOverlapped;
	};
}