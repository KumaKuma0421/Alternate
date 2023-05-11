//! @file	TCPPerformanceCheck.h
//! @brief	TCP性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.16

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"
#include "Thread.h"
#include "SocketBuilder.h"
#include "TcpConnector.h"

#define SECTION_TCP_SERVER		    _T("TcpServer")
#define SECTION_TCP_CLIENT		    _T("TcpClient")
#define KEYWORD_IP_ADDRESS		    _T("IPAddress")
#define KEYWORD_PORT			    _T("Port")
#define KEYWORD_RETRY_COUNT		    _T("RetryCount")
#define KEYWORD_RETRY_INTERVAL      _T("RetryInterval")
#define KEYWORD_KICK_OFF		    _T("KickOff")

#define SECTION_TCP_PROXY		    _T("TcpProxy")
#define KEYWORD_SERVER_ADDRESS	    _T("ServerAddress")
#define KEYWORD_SERVER_PORT		    _T("ServerPort")
#define KEYWORD_CLIENT_ADDRESS	    _T("ClientAddress")
#define KEYWORD_CLIENT_PORT		    _T("ClientPort")

#define _DUMP_DATA
#define DUMP_COUNT 24
#define DATA_SIZE 1460

#define SLEEP_TIME_30   30000
#define SLEEP_TIME_120 120000

int MakeData (BYTE byData[]);
void DumpData (int direction, BYTE byData[], int iDataLen); // 0:Down 1:Up

void DriveTcpServer ();
void DriveTcpClient ();
void DriveTcpProxy ();

BOOL DriveTcpServerConsoleHandler (DWORD dwType);
BOOL DriveTcpProxyConsoleHandler (DWORD dwType);
BOOL DriveTcpClientConsoleHandler (DWORD dwType);

namespace PerfCheck
{
	//! @class	TCPPerformanceCheck
	//! @brief	TCP性能測定用
	class TCPPerformanceCheck : public PerformanceCheck
	{
	public:
		TCPPerformanceCheck (alt::Console& console);
		virtual ~TCPPerformanceCheck ();

		virtual BOOL DoAction ();
	};

	//! @class	ServerWorker
	//! @brief	TCP性能測定用（サーバースレッド）
	class ServerWorker : public alt::skeleton::ICallback
	{
	public:
		ServerWorker (alt::TcpConnector* connector);
		virtual ~ServerWorker ();
		BOOL Start ();
		void Stop ();

	protected:
		ServerWorker ();
		virtual DWORD Invoke (LPVOID lpvParam);

	private:
		volatile bool _stop;
		alt::TcpConnector* _connector;
		alt::Thread* _thread;
	};

	//! @class	TcpServer
	//! @brief	TCP性能測定用（サーバー）
	class TcpServer : public alt::skeleton::ICallback
	{
	public:
		TcpServer ();
		virtual ~TcpServer ();

		bool Init (LPCTSTR lpctszIP, USHORT wPort);
		BOOL Start ();
		DWORD Wait ();
		void Stop ();

	protected:
		virtual DWORD Invoke (LPVOID lpvParam);

	private:
		volatile bool _stop;
		alt::SocketBuilder* _builder;
		alt::Thread* _thread;
#pragma warning (push)
#pragma warning (disable:4251)
		alt::skeleton::Array<ServerWorker> _workerHolder;
#pragma warning (pop)
	};

	//! @class	TcpClient
	//! @brief	TCP性能測定用（クライアント）
	class TcpClient : public alt::skeleton::ICallback
	{
	public:
		TcpClient ();
		virtual ~TcpClient ();

		bool Init ();
		bool Start (LPCTSTR lpctszIP, USHORT wPort, int retryCount, int retryInterval, bool kickOff);
		void Stop ();

	protected:
		virtual DWORD Invoke (LPVOID lpvParam);

	private:
		bool _stop;
		alt::SocketBuilder* _builder;
		alt::TcpConnector* _connector;
		alt::Thread* _thread;
	};

	//! @class	UpstreamDriver
	//! @brief	TCP性能測定用（アップストリーム）
	class UpstreamDriver : public alt::skeleton::ICallback
	{
	public:
		UpstreamDriver ()
		{
			_connectorServer = nullptr;
			_connectorClient = nullptr;
		};
		virtual DWORD Invoke (LPVOID lpvParam);
		void SetTcpServer (alt::TcpConnector* connector) { _connectorServer = connector; };
		void SetTcpClient (alt::TcpConnector* connector) { _connectorClient = connector; };

	private:
		alt::TcpConnector* _connectorServer;
		alt::TcpConnector* _connectorClient;
	};

	//! @class	DownstreamDriver
	//! @brief	TCP性能測定用（ダウンストリーム）
	class DownstreamDriver : public alt::skeleton::ICallback
	{
	public:
		DownstreamDriver ()
		{
			_connectorServer = nullptr;
			_connectorClient = nullptr;
		};
		virtual DWORD Invoke (LPVOID lpvParam);
		void SetTcpServer (alt::TcpConnector* connector) { _connectorServer = connector; };
		void SetTcpClient (alt::TcpConnector* connector) { _connectorClient = connector; };

	private:
		alt::TcpConnector* _connectorServer;
		alt::TcpConnector* _connectorClient;
	};

	//! @class	TcpProxy
	//! @brief	TCP性能測定用（プロキシー）
	class TcpProxy
	{
	public:
		TcpProxy ();
		virtual ~TcpProxy ();

		bool Init (LPCTSTR lpctszClientIP, USHORT wClientPort, LPCTSTR lpctszServerIP, USHORT wServerPort);
		bool Start ();
		void Stop ();

	private:
		bool _stop;
		alt::SocketBuilder* _builderServer;
		alt::SocketBuilder* _builderClient;
		alt::TcpConnector* _connectorServer;
		alt::TcpConnector* _connectorClient;
		UpstreamDriver* _driverUpstream;
		DownstreamDriver* _driverDownstream;
		alt::Thread* _threadUpstream;
		alt::Thread* _threadDownstream;
	};
}