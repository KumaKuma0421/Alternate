//! @file	TCPPerformanceCheck.cpp
//! @brief	TCPクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.16

#include "pch.h"
#include "TCPPerformanceCheck.h"

using namespace PerfCheck;

// --------------------------------------------------------------------
TcpServer* __tcpServer = nullptr;
TcpProxy* __tcpProxy = nullptr;
TcpClient* __tcpClient = nullptr;
// --------------------------------------------------------------------

TCPPerformanceCheck::TCPPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{

}

TCPPerformanceCheck::~TCPPerformanceCheck ()
{

}

BOOL TCPPerformanceCheck::DoAction ()
{
	alt::Thread threadServer;
	alt::Thread threadProxy;
	alt::Thread threadClient;

	auto funcServer = [](LPVOID lpvParam)->DWORD { DriveTcpServer (); return 0; };
	auto funcProxy = [](LPVOID lpvParam)->DWORD { DriveTcpProxy (); return 0; };
	auto funcClient = [](LPVOID lpvParam)->DWORD { DriveTcpClient (); return 0; };

	threadServer.Create (funcServer);
	threadServer.Create (funcProxy);
	threadServer.Create (funcClient);

	threadClient.Wait ();

	return TRUE;
}

void DriveTcpServer ()
{
	_tprintf (_T ("DriveTcpServer() IN.\n"));

	alt::TString ipAddress (MAX_PATH);
	USHORT wPort;

	DWORD dwRet = ::GetPrivateProfileString (SECTION_TCP_SERVER, KEYWORD_IP_ADDRESS, nullptr, ipAddress.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("IPAddressの指定が見つかりません。\n"));
		return;
	}

	wPort = ::GetPrivateProfileInt (SECTION_TCP_SERVER, KEYWORD_PORT, 0, CONFIG_FILE);
	if (wPort == 0)
	{
		_tprintf (_T ("Portの指定が見つかりません。\n"));
		return;
	}

	auto object = std::make_unique<TcpServer> ();
	__tcpServer = object.get ();

	SetConsoleCtrlHandler (DriveTcpServerConsoleHandler, TRUE);

	object->Init (ipAddress.Ctr (), wPort);
	object->Start ();
	object->Wait ();

	_tprintf (_T ("DriveTcpServer() OUT.\n"));
}

void DriveTcpClient ()
{
	_tprintf (_T ("DriveTcpClient() IN.\n"));

	alt::TString ipAddress (MAX_PATH);
	USHORT wPort;
	int retryCount;
	int retryInterval;

	DWORD dwRet = ::GetPrivateProfileString (SECTION_TCP_CLIENT, KEYWORD_IP_ADDRESS, nullptr, ipAddress.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("Not found IPAddress.\n"));
		return;
	}

	wPort = ::GetPrivateProfileInt (SECTION_TCP_CLIENT, KEYWORD_PORT, 0, CONFIG_FILE);
	if (wPort == 0)
	{
		_tprintf (_T ("Not found Port.\n"));
		return;
	}

	retryInterval = ::GetPrivateProfileInt (SECTION_TCP_CLIENT, KEYWORD_RETRY_INTERVAL, -1, CONFIG_FILE);

	retryCount = ::GetPrivateProfileInt (SECTION_TCP_CLIENT, KEYWORD_RETRY_COUNT, -1, CONFIG_FILE);

	int kickOffValue = ::GetPrivateProfileInt (SECTION_TCP_CLIENT, KEYWORD_KICK_OFF, 0, CONFIG_FILE);
	bool kickOff = kickOffValue == 1 ? true : false;

	auto object = std::make_unique<TcpClient> ();
	__tcpClient = object.get ();

	SetConsoleCtrlHandler (DriveTcpClientConsoleHandler, TRUE);

	object->Init ();
	object->Start (ipAddress.Ctr (), wPort, retryInterval, retryCount, kickOff);

	_tprintf (_T ("DriveTcpClient() OUT.\n"));
}

void DriveTcpProxy ()
{
	_tprintf (_T ("DriveTcpProxy() IN.\n"));

	alt::TString ServerAddress (MAX_PATH);
	alt::TString ClientAddress (MAX_PATH);
	USHORT wServerPort;
	USHORT wClientPort;
	DWORD dwRet;

	dwRet = ::GetPrivateProfileString (SECTION_TCP_PROXY, KEYWORD_SERVER_ADDRESS, nullptr, ServerAddress.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("ServerAddressの指定が見つかりません。\n"));
		return;
	}

	dwRet = ::GetPrivateProfileString (SECTION_TCP_PROXY, KEYWORD_CLIENT_ADDRESS, nullptr, ClientAddress.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("ClientAddressの指定が見つかりません。\n"));
		return;
	}

	wServerPort = ::GetPrivateProfileInt (SECTION_TCP_PROXY, KEYWORD_SERVER_PORT, 0, CONFIG_FILE);
	if (wServerPort == 0)
	{
		_tprintf (_T ("ServerPortの指定が見つかりません。\n"));
		return;
	}

	wClientPort = ::GetPrivateProfileInt (SECTION_TCP_PROXY, KEYWORD_CLIENT_PORT, 0, CONFIG_FILE);
	if (wClientPort == 0)
	{
		_tprintf (_T ("ClientPortの指定が見つかりません。\n"));
		return;
	}

	auto object = std::make_unique<TcpProxy> ();
	object->Init (ClientAddress.Ctr (), wClientPort, ServerAddress.Ctr (), wServerPort);
	object->Start ();

	_tprintf (_T ("DriveTcpProxy() OUT.\n"));
}

// --------------------------------------------------------------------

BOOL DriveTcpServerConsoleHandler (DWORD dwType)
{
	_tprintf (_T ("DriveTcpServerConsoleHandler(%d) IN.\n"), dwType);

	BOOL ret = FALSE;

	switch (dwType)
	{
		case 0: // Ctrl + C
		case 1: // Ctrl + Break
			__tcpServer->Stop ();
			ret = TRUE; // 終了させません。
			break;

		case 2: // Console Close
		case 3: // Log off
		case 4: // Shut down
			ret = FALSE; // 次に委ねます。
			break;
	}

	_tprintf (_T ("DriveTcpServerConsoleHandler(%d) OUT.\n"), dwType);
	return ret;
}

BOOL DriveTcpProxyConsoleHandler (DWORD dwType)
{
	_tprintf (_T ("DriveTcpProxyConsoleHandler(%d) IN.\n"), dwType);

	BOOL ret = FALSE;

	switch (dwType)
	{
		case 0: // Ctrl + C
		case 1: // Ctrl + Break
			__tcpProxy->Stop ();
			ret = TRUE; // 終了させません。
			break;

		case 2: // Console Close
		case 3: // Log off
		case 4: // Shut down
			ret = FALSE; // 次に委ねます。
			break;
	}

	_tprintf (_T ("DriveTcpProxyConsoleHandler(%d) OUT.\n"), dwType);
	return ret;
}

BOOL DriveTcpClientConsoleHandler (DWORD dwType)
{
	_tprintf (_T ("DriveTcpClientConsoleHandler(%d) IN.\n"), dwType);

	BOOL ret = FALSE;

	switch (dwType)
	{
		case 0: // Ctrl + C
		case 1: // Ctrl + Break
			__tcpClient->Stop ();
			ret = TRUE; // 終了させません。
			break;

		case 2: // Console Close
		case 3: // Log off
		case 4: // Shut down
			ret = FALSE; // 次に委ねます。
			break;
	}

	_tprintf (_T ("DriveTcpClientConsoleHandler(%d) OUT.\n"), dwType);
	return ret;
}

// --------------------------------------------------------------------

int MakeData (BYTE byData[])
{
	SYSTEMTIME t;

	FillMemory (byData, sizeof (byData), 'X');
	GetLocalTime (&t);
	wsprintfA ((LPSTR)byData, "%02d/%02d/%02d %02d:%02d:%02d.%03d_",
			  t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute,
			  t.wSecond, t.wMilliseconds);

	return DATA_SIZE;
}

//! @brief		バイナリデータをコンソールにダンプします。
//! @param[in]	lpctszFrom 出力スレッド名
//! @param[in]	direction (0:"Down"表示, 1:"Up"表示)
//! @param[in]	byData		ダンプデータ
//! @param[in]	iDataLen	ダンプサイズ
//! @note		最初に16進、次にキャラクタで出力します。
void DumpData (LPCTSTR lpctszFrom, int direction, BYTE byData[], int iDataLen) // 0:Down 1:Up
{
	LPCTSTR lpctszMsg =
		(direction == 0 ? _T (" Down:(%04d)") : _T (" Up  :(%04d)"));

	_tprintf (lpctszFrom);
	_tprintf (lpctszMsg, iDataLen);
	for (int i = 0; i < DUMP_COUNT; i++)
	{
		_tprintf (_T ("%02X "), byData[i]);
	}
	_tprintf (_T ("|| "));
	for (int i = 0; i < DUMP_COUNT; i++)
	{
		_tprintf (_T ("%c"), byData[i]);
	}
	_tprintf (_T ("\n"));
}

// --------------------------------------------------------------------

ServerWorker::ServerWorker (alt::TcpConnector* connector)
{
	_tprintf (_T ("ServerWorker::ServerWorker(TcpConnector*) IN.\n"));

	_connector = connector;
	_thread = new alt::Thread ((alt::skeleton::ICallback*)this);
	_stop = false;

	_tprintf (_T ("ServerWorker::ServerWorker(TcpConnector*) OUT.\n"));
}

ServerWorker::ServerWorker ()
{
	_connector = nullptr;
	_thread = nullptr;
	_stop = true;
}

ServerWorker::~ServerWorker ()
{
	_tprintf (_T ("ServerWorker::~ServerWorker() IN.\n"));

	_connector->Close ();
	_thread->Close ();

	delete _connector;
	delete _thread;

	_tprintf (_T ("ServerWorker::~ServerWorker() OUT.\n"));
}

BOOL ServerWorker::Start ()
{
	_tprintf (_T ("ServerWorker::Start() CALL.\n"));

	return _thread->Create ();
}

void ServerWorker::Stop ()
{
	_tprintf (_T ("ServerWorker::Stop() CALL.\n"));

	_stop = true;
}

DWORD ServerWorker::Invoke (LPVOID lpvParam)
{
	_tprintf (_T ("ServerWorker::Invoke() IN.\n"));

	BYTE byData[DATA_SIZE]{ 0 };
	int count = 0;

	while (_stop == false)
	{
		INT recvSize = _connector->Recv (byData, sizeof (byData), 0);
		if (recvSize <= 0)
		{
			_tprintf (_T ("ServerWorker::Invoke() recvSize=%d recv() failed. reason:%d\n"),
					 recvSize, _connector->GetErrNo ());
			break;
		}

#ifdef _DUMP_DATA
		DumpData (_T ("SVR"), 1, byData, recvSize);
#else
		_tprintf (_T ("%s %d\r"), _T ("SVR"), count++);
#endif

		INT sendSize = _connector->Send (byData, recvSize);
		if (sendSize != recvSize)
		{
			_tprintf (_T ("send=%d sended=%d send failed. reason:%d\n"),
					 recvSize, sendSize, _connector->GetErrNo ());
			break;
		}
	}

	_tprintf (_T ("ServerWorker::Invoke() OUT.\n"));
	return 0;
}

// --------------------------------------------------------------------

TcpServer::TcpServer ()
{
	_tprintf (_T ("TcpServer::TcpServer() IN.\n"));

	_stop = false;
	_builder = new alt::SocketBuilder ();
	_thread = new alt::Thread ((ICallback*)this);

	_tprintf (_T ("TcpServer::TcpServer() OUT.\n"));
}

TcpServer::~TcpServer ()
{
	_tprintf (_T ("TcpServer::~TcpServer() IN.\n"));

	_thread->Close ();

	delete _builder;
	delete _thread;

	_tprintf (_T ("TcpServer::~TcpServer() OUT.\n"));
}

bool TcpServer::Init (LPCTSTR lpctszIP, USHORT wPort)
{
	_tprintf (_T ("TcpServer::Init(%s, %d) IN.\n"), lpctszIP, wPort);

	_stop = false;
	_builder->Startup ();
	_builder->Prepare (wPort, lpctszIP);

	_tprintf (_T ("TcpServer::Init(%s, %d) OUT.\n"), lpctszIP, wPort);
	return true;
}

BOOL TcpServer::Start ()
{
	_tprintf (_T ("TcpServer::Start() CALL.\n"));

	return _thread->Create ();
}

DWORD TcpServer::Wait ()
{
	_tprintf (_T ("TcpServer::Wait() CALL.\n"));

	return _thread->Wait ();
}

void TcpServer::Stop ()
{
	_tprintf (_T ("TcpServer::Stop() IN.\n"));

	_stop = true;
	_builder->Close ();

	for (int i = 0; i < _workerHolder.Size (); i++)
	{
		_workerHolder.Get (i)->Stop ();
	}

	_tprintf (_T ("TcpServer::Stop() OUT.\n"));
}

DWORD TcpServer::Invoke (LPVOID lpvParam)
{
	_tprintf (_T ("TcpServer::Invoke(LPVOID) IN.\n"));

	while (_stop == false)
	{
		alt::TcpConnector* connector = _builder->Wait ();
		if (connector)
		{
			_tprintf (_T (" _build->Wait() success.\n"));

			connector->SetKeepAliveValue (1, 500, 300);

			ServerWorker* worker = new ServerWorker (connector);
			worker->Start ();
			_workerHolder.Add (*worker);
		}
		else
		{
			_tprintf (_T (" _builder->Wait() failed. reason=%d\n"), _builder->GetErrNo ());

		}
	}

	_tprintf (_T ("TcpServer::Invoke(LPVOID) stop detected.\n"));

	Sleep (1000);

	for (int i = 0; i < _workerHolder.Size (); i++)
	{
		_workerHolder.Get (i)->Stop ();
	}

	_tprintf (_T ("TcpServer::Invoke(LPVOID) worker->Stop() called.\n"));

	Sleep (1000);

	_workerHolder.Clear ();

	_tprintf (_T ("TcpServer::Invoke(LPVOID) worker deleted.\n"));

	_tprintf (_T ("TcpServer::Invoke(LPVOID) OUT.\n"));
	return 0;
}

// --------------------------------------------------------------------

TcpClient::TcpClient ()
{
	_tprintf (_T ("TcpClient::TcpClient() IN.\n"));

	_stop = false;
	_builder = new alt::SocketBuilder ();
	_connector = nullptr;
	_thread = new alt::Thread ((alt::skeleton::ICallback*)this);

	_tprintf (_T ("TcpClient::TcpClient() OUT.\n"));
}

TcpClient::~TcpClient ()
{
	_tprintf (_T ("TcpClient::~TcpClient() IN.\n"));

	if (_connector) _connector->Close ();
	_thread->Close ();

	delete _connector;
	delete _builder;
	delete _thread;

	_tprintf (_T ("TcpClient::~TcpClient() OUT.\n"));
}

bool TcpClient::Init ()
{
	_tprintf (_T ("TcpClient::Init() CALL.\n"));

	return _builder->Startup () == 0 ? true : false;
}

bool TcpClient::Start (LPCTSTR lpctszIP, USHORT wPort, int retryInterval, int retryCount, bool kickOff)
{
	_tprintf (_T ("TcpClient::Start(IP:%s, Port:%d, RetryInterval:%d, RetryCount:%d) IN.\n"),
			 lpctszIP, wPort, retryInterval, retryCount);

	_connector = _builder->CreateTcpConnector (lpctszIP, wPort, retryInterval, retryCount);
	if (_connector)
	{
		_connector->SetKeepAliveValue (1, 500, 300);

		if (kickOff)
		{
			BYTE byData[DATA_SIZE];
			DWORD dwDataLen;
			dwDataLen = MakeData (byData);

			INT ret = _connector->Send (byData, dwDataLen);
			if ((DWORD)ret != dwDataLen)
			{
				_tprintf (_T ("TcpClient::Start() Send() invalid send size (%d). reason=%d \n"), ret, _connector->GetErrNo ());
			}
		}

		_thread->Create ();
		_thread->Wait ();
	}

	_tprintf (_T ("TcpClient::Start(IP:%s, Port:%d, RetryInterval:%d, RetryCount:%d) OUT.\n"),
			 lpctszIP, wPort, retryInterval, retryCount);
	return true;
}

void TcpClient::Stop ()
{
	_tprintf (_T ("TcpClient::Stop() CALL.\n"));
	_stop = true;
	_connector->Close ();
}

DWORD TcpClient::Invoke (LPVOID lpvParam)
{
	_tprintf (_T ("TcpClient::Invoke(LPVOID) IN.\n"));

	BYTE byData[DATA_SIZE]{ 0 };

	int count = 0;
	while (_stop == false)
	{
		INT recvSize = _connector->Recv (byData, sizeof (byData), 0);
		if (recvSize <= 0)
		{
			_tprintf (_T ("TcpClient::Invoke(LPVOID) recvSize=%d Socket disconnected. reason:%d\n"),
					 recvSize, _connector->GetErrNo ());
			return -1;
		}
#ifdef _DUMP_DATA
		DumpData (_T ("CLT"), 0, byData, recvSize);
#else
		_tprintf (_T ("%s %d\r"), _T ("CLT"), count++);
#endif
		int dataLen = MakeData (byData);

		INT sendSize = _connector->Send (byData, dataLen);
		if (sendSize != dataLen)
		{
			_tprintf (_T ("TcpClient::Invoke(LPVOID) send=%d sended=%d Invalid send size. reason:%d\n"),
					 dataLen, sendSize, _connector->GetErrNo ());
			return -2;
		}
	}

	_tprintf (_T ("TcpClient::Invoke(LPVOID) OUT.\n"));
	return 0;
}

// --------------------------------------------------------------------

TcpProxy::TcpProxy ()
{
	_stop = false;
	_builderServer = new alt::SocketBuilder ();
	_builderClient = new alt::SocketBuilder ();
	_connectorServer = nullptr;
	_connectorClient = nullptr;
	_driverUpstream = new UpstreamDriver ();
	_driverDownstream = new DownstreamDriver ();
	_threadUpstream = new alt::Thread ((alt::skeleton::ICallback*)_driverUpstream);
	_threadDownstream = new alt::Thread ((alt::skeleton::ICallback*)_driverDownstream);
}

TcpProxy::~TcpProxy ()
{
	_threadUpstream->Close ();
	_threadDownstream->Close ();

	_connectorServer->Close ();
	_connectorClient->Close ();

	delete _driverUpstream;
	delete _driverDownstream;
	delete _threadUpstream;
	delete _threadDownstream;
	delete _builderServer;
	delete _builderClient;
}

bool TcpProxy::Init (LPCTSTR lpctszClientIP, USHORT wClientPort, LPCTSTR lpctszServerIP, USHORT wServerPort)
{
	_builderClient->Startup ();
	_connectorClient = _builderClient->CreateTcpConnector (lpctszClientIP, wClientPort, 3000, 5); //! @todo 暫定リテラル

	_builderServer->Startup ();
	_builderServer->Prepare (wServerPort, lpctszServerIP);
	_connectorServer = _builderServer->Wait ();

	_driverUpstream->SetTcpClient (_connectorClient);
	_driverUpstream->SetTcpServer (_connectorServer);
	_driverDownstream->SetTcpClient (_connectorClient);
	_driverDownstream->SetTcpServer (_connectorServer);

	_threadUpstream->Create ();
	_threadDownstream->Create ();

	return true;
}

bool TcpProxy::Start ()
{
	HANDLE handles[2]{ INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };
	handles[0] = _threadUpstream->GetHandle ();
	handles[1] = _threadDownstream->GetHandle ();

	::WaitForMultipleObjects (2, handles, FALSE, INFINITE);

	return true;
}

void TcpProxy::Stop ()
{
	_stop = true; //! @todo 要検討 上のStart()で既にブロックされている。
}

DWORD UpstreamDriver::Invoke (LPVOID lpvParam)
{
	BYTE byData[DATA_SIZE]{ 0 };
	int count = 0;

	for (;;)
	{
		int recvSize = _connectorClient->Recv (byData, sizeof (byData), 0);
		if (recvSize <= 0)
		{
			_tprintf (_T ("Down:recvSize=%d Socket disconnected. reason:%d\n"),
					 recvSize, _connectorClient->GetErrNo ());
			return -1;
		}

#ifdef _DUMP_DATA
		DumpData (_T ("PXY"), 1, byData, recvSize);
#else
		_tprintf (_T ("%s %d\r"), _T ("PXY"), count++);
#endif

		int sendSize = _connectorServer->Send (&byData, recvSize);
		if (sendSize != recvSize)
		{
			_tprintf (_T ("Down:send=%d sended=%d Invalid send size. reason:%d\n"),
					 recvSize, sendSize, _connectorServer->GetErrNo ());
			return -2;
		}
	}

	return 0;
}

DWORD DownstreamDriver::Invoke (LPVOID lpvParam)
{
	BYTE byData[DATA_SIZE]{ 0 };
	int count = 0;

	for (;;)
	{
		INT recvSize = _connectorServer->Recv (byData, sizeof (byData), 0);
		if (recvSize <= 0)
		{
			_tprintf (_T ("Up  :recvSize=%d Socket disconnected. reason:%d\n"),
					 recvSize, _connectorServer->GetErrNo ());
			return -1;
		}

#ifdef _DUMP_DATA
		DumpData (_T ("PXY"), 0, byData, recvSize);
#else
		_tprintf (_T ("%s %d\r"), _T ("PXY"), count++);
#endif

		INT sendSize = _connectorClient->Send (byData, recvSize);
		if (sendSize != recvSize)
		{
			_tprintf (_T ("Up  :send=%d sended=%d Invalid send size. reason:%d\n"),
					 recvSize, sendSize, _connectorClient->GetErrNo ());
			return -2;
		}
	}

	return 0;
}