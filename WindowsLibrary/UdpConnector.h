// ----------------------------------------------------------------------------
//! @file	UdpConnector.h
//! @brief	UDP通信に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2021.4.25

#pragma once

#include "SocketLibrary.h"

namespace alt
{
	//! @class	UdpConnector
	//! @brief	UDP通信に関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC UdpConnector : public SocketLibrary
	{
	public:
		//! @brief コンストラクタ
		//! @param socket 使用するソケット
		APIENTRY UdpConnector (SOCKET socket);

		//! @brief UDP送信先アドレスの設定
		//! @param lpctszAddress 相手のIPアドレス
		//! @param portNo 相手のポート番号
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY SetSockAddr (LPCTSTR lpctszAddress, u_short portNo);

		//! @brief UDP送信処理
		//! @param lpvBuf 送信データ
		//! @param dwSize 送信サイズ
		//! @return INT 送信サイズ
		INT APIENTRY Send (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief UDP受信処理
		//! @param lpvBuf 受信データ
		//! @param dwSize 受信サイズ
		//! @return INT 受信サイズ
		INT APIENTRY Recv (LPVOID lpvBuf, DWORD dwSize) const;

	protected:
		UdpConnector ();
		SOCKADDR_IN	_sockAddrIn;
	};
}