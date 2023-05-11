// ----------------------------------------------------------------------------
//! @file	SocketBuilder.h
//! @brief	ソケットに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "SocketLibrary.h"
#include "TcpConnector.h"
#include "UdpConnector.h"

namespace alt
{
	//! @class	SocketBuilder
	//! @brief	ソケットに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC SocketBuilder : public SocketLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY SocketBuilder ();

		//! @brief デストラクタ
		APIENTRY ~SocketBuilder ();

		//! @brief Windowsソケットの使用準備
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Startup ();

		//! @brief UdpConnectorの作成
		//! @param lpctszIpAddr 設定する自身のIPアドレス
		//! @param portNo 設定する自身のポート番号
		//! @return UdpConnector*
		UdpConnector* APIENTRY CreateUdpConnector (
			LPCTSTR lpctszIpAddr, u_short portNo);

		//! @brief TcpConnectorの作成
		//! @param lpctszIpAddr 設定する自身のIPアドレス
		//! @param portNo 設定する自身のポート番号
		//! @param retryInterval リトライ間隔(ms)
		//! @param retryCount リトライ回数
		//! @return TcpConnector*
		TcpConnector* APIENTRY CreateTcpConnector (
			LPCTSTR lpctszIpAddr, u_short portNo, int retryInterval,
			int retryCount);

		//! @brief TCPポートのlisten(),accept()の準備
		//! @param portNo 待機ポート番号
		//! @param lpctszIpAddr 待機IPアドレス
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Prepare (u_short portNo, LPCTSTR lpctszIpAddr = NULL);

		//! @brief TCP接続のlisten(),accept()待機関数
		//! @return TcpConnector*
		TcpConnector* APIENTRY Wait ();

		//! @brief ホスト名からIPアドレスのDNS解決
		//! @param lpctszHostName ホスト名
		//! @param lptszIpAddr DNS名前解決されたIPアドレス
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY GetHostByName (
			LPCTSTR lpctszHostName, LPCTSTR lptszIpAddr) const;

	protected:
		BOOL APIENTRY Socket (int af, int type, int protocol);
		BOOL APIENTRY SetSockAddr (
			ADDRESS_FAMILY family, u_short portNo, LPCTSTR lpctszAddress);
		BOOL APIENTRY SetSockOpt (int level, int optname, int value) const;
		BOOL APIENTRY Connect () const;
		BOOL APIENTRY Bind () const;
		BOOL APIENTRY Listen () const;
		BOOL APIENTRY Ioctl (long cmd, u_long arg) const;

		SOCKADDR_IN	_SockAddrIn;
		WSADATA		_wsaData;
	};
}