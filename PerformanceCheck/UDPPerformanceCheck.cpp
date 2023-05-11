//! @file	UDPPerformanceCheck.cpp
//! @brief	UDPクラス性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.17

#include "pch.h"
#include "UDPPerformanceCheck.h"

using namespace PerfCheck;

// --------------------------------------------------------------------
UdpController* __udpController1 = nullptr;
UdpController* __udpController2 = nullptr;
// --------------------------------------------------------------------

UDPPerformanceCheck::UDPPerformanceCheck (alt::Console& console)
	:PerformanceCheck (console)
{
}

UDPPerformanceCheck::~UDPPerformanceCheck ()
{
}

BOOL UDPPerformanceCheck::DoAction ()
{
	alt::Thread threadServer;
	alt::Thread threadClient;

	auto funcServer = [](LPVOID lpvParam)->DWORD { DriveUdpController1 (); return 0; };
	auto funcClient = [](LPVOID lpvParam)->DWORD { DriveUdpController2 (); return 0; };

	threadServer.Create (funcServer);
	threadClient.Create (funcClient);

	threadClient.Wait ();
	threadServer.Wait ();

	return TRUE;
}

BOOL DriveUdpController1ConsoleHandler (DWORD dwType)
{
	_tprintf (_T ("DriveUdpController1ConsoleHandler(%d) IN.\n"), dwType);

	BOOL ret = FALSE;

	switch (dwType)
	{
		case 0: // Ctrl + C
		case 1: // Ctrl + Break
			__udpController1->Stop ();
			ret = TRUE; // 終了させません。
			break;

		case 2: // Console Close
		case 3: // Log off
		case 4: // Shut down
			ret = FALSE; // 次に委ねます。
			break;
	}

	_tprintf (_T ("DriveUdpController1ConsoleHandler(%d) OUT.\n"), dwType);
	return ret;
}

void DriveUdpController1 ()
{
	_tprintf (_T ("DriveUdpController1() IN.\n"));

	DWORD dwRet;
	alt::TString ipAddress (MAX_PATH);
	USHORT wPort;

	dwRet = ::GetPrivateProfileString (SECTION_UDP_CONTROLLER_1, KEYWORD_IP_ADDRESS, nullptr, ipAddress.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("IPAddressの指定が見つかりません。\n"));
		return;
	}

	wPort = ::GetPrivateProfileInt (SECTION_UDP_CONTROLLER_1, KEYWORD_PORT, 0, CONFIG_FILE);
	if (wPort == 0)
	{
		_tprintf (_T ("Portの指定が見つかりません。\n"));
		return;
	}

	int kickOffValue = ::GetPrivateProfileInt (SECTION_UDP_CONTROLLER_1, KEYWORD_KICK_OFF, 0, CONFIG_FILE);
	bool kickOff = kickOffValue == 1 ? true : false;

	alt::TString ipAddressCounter (MAX_PATH);
	USHORT wPortCounter;

	dwRet = ::GetPrivateProfileString (SECTION_UDP_CONTROLLER_2, KEYWORD_IP_ADDRESS, nullptr, ipAddressCounter.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("対向IPAddressの指定が見つかりません。\n"));
		return;
	}

	wPortCounter = ::GetPrivateProfileInt (SECTION_UDP_CONTROLLER_2, KEYWORD_PORT, 0, CONFIG_FILE);
	if (wPortCounter == 0)
	{
		_tprintf (_T ("対向Portの指定が見つかりません。\n"));
		return;
	}

	auto object = std::make_unique<UdpController> ();
	__udpController1 = object.get ();

	SetConsoleCtrlHandler (DriveUdpController1ConsoleHandler, TRUE);

	object->Init ();
	object->Start (ipAddress.Ctr (), wPort, ipAddressCounter.Ctr (), wPortCounter, kickOff);

	_tprintf (_T ("DriveUdpController1() OUT.\n"));
}

BOOL DriveUdpController2ConsoleHandler (DWORD dwType)
{
	_tprintf (_T ("DriveUdpController2ConsoleHandler(%d) IN.\n"), dwType);

	BOOL ret = FALSE;

	switch (dwType)
	{
		case 0: // Ctrl + C
		case 1: // Ctrl + Break
			__udpController2->Stop ();
			ret = TRUE; // 終了させません。
			break;

		case 2: // Console Close
		case 3: // Log off
		case 4: // Shut down
			ret = FALSE; // 次に委ねます。
			break;
	}

	_tprintf (_T ("DriveUdpController2ConsoleHandler(%d) OUT.\n"), dwType);
	return ret;
}

void DriveUdpController2 ()
{
	_tprintf (_T ("DriveUdpController2() IN.\n"));

	DWORD dwRet;
	alt::TString ipAddress (MAX_PATH);
	USHORT wPort;

	dwRet = ::GetPrivateProfileString (SECTION_UDP_CONTROLLER_2, KEYWORD_IP_ADDRESS, nullptr, ipAddress.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("IPAddressの指定が見つかりません。\n"));
		return;
	}

	wPort = ::GetPrivateProfileInt (SECTION_UDP_CONTROLLER_2, KEYWORD_PORT, 0, CONFIG_FILE);
	if (wPort == 0)
	{
		_tprintf (_T ("Portの指定が見つかりません。\n"));
		return;
	}

	int kickOffValue = ::GetPrivateProfileInt (SECTION_UDP_CONTROLLER_2, KEYWORD_KICK_OFF, 0, CONFIG_FILE);
	bool kickOff = kickOffValue == 1 ? true : false;

	alt::TString ipAddressCounter (MAX_PATH);
	USHORT wPortCounter;

	dwRet = ::GetPrivateProfileString (SECTION_UDP_CONTROLLER_1, KEYWORD_IP_ADDRESS, nullptr, ipAddressCounter.Ptr (), MAX_PATH, CONFIG_FILE);
	if (dwRet == 0)
	{
		_tprintf (_T ("対向IPAddressの指定が見つかりません。\n"));
		return;
	}

	wPortCounter = ::GetPrivateProfileInt (SECTION_UDP_CONTROLLER_1, KEYWORD_PORT, 0, CONFIG_FILE);
	if (wPortCounter == 0)
	{
		_tprintf (_T ("対向Portの指定が見つかりません。\n"));
		return;
	}

	auto object = std::make_unique<UdpController> ();
	__udpController2 = object.get ();

	SetConsoleCtrlHandler (DriveUdpController2ConsoleHandler, TRUE);

	object->Init ();
	object->Start (ipAddress.Ctr (), wPort, ipAddressCounter.Ctr (), wPortCounter, kickOff);

	_tprintf (_T ("DriveUdpController2() OUT.\n"));
}

UdpController::UdpController ()
{
	_tprintf (_T ("UdpController::UdpController() IN.\n"));

	_stop = false;
	_builder = new alt::SocketBuilder ();
	_connector = nullptr;
	_thread = new alt::Thread ((alt::skeleton::ICallback*)this);

	_tprintf (_T ("UdpController::UdpController() OUT.\n"));
}

UdpController::~UdpController ()
{
	_tprintf (_T ("UdpController::~UdpController() IN.\n"));

	if (_connector) _connector->Close ();
	_thread->Close ();

	delete _connector;
	delete _builder;
	delete _thread;

	_tprintf (_T ("UdpController::~UdpController() OUT.\n"));
}

bool UdpController::Init ()
{
	_tprintf (_T ("UdpController::Init() CALL.\n"));

	return _builder->Startup ();
}

bool UdpController::Start (LPCTSTR lpctszIP, USHORT wPort, LPCTSTR lpctszTargetIP, USHORT wTargetPort, bool kickOff)
{
	_tprintf (_T ("UdpController::Start(IP:%s, Port:%d, kickoff:%d) IN.\n"), lpctszIP, wPort, kickOff);

	_connector = _builder->CreateUdpConnector (lpctszIP, wPort);
	if (_connector)
	{
		_connector->SetSockAddr (lpctszTargetIP, wTargetPort);
		if (kickOff)
		{
			BYTE byData[DATA_SIZE];
			DWORD dwDataLen;
			dwDataLen = MakeData (byData);

			INT ret = _connector->Send (byData, dwDataLen);
			if ((DWORD)ret != dwDataLen)
			{
				_tprintf (_T (" Send() invalid send size (%d). reason=%d \n"), ret, _connector->GetErrNo ());
			}
		}

		_thread->Create ();
		_thread->Wait ();
	}

	_tprintf (_T ("UdpController::Start(IP:%s, Port:%d, kickoff:%d) OUT.\n"), lpctszIP, wPort, kickOff);
	return true;
}

bool UdpController::SetClient (LPCTSTR lpctszIP, USHORT wPort)
{
	_tprintf (_T ("UdpController::SetClient(IP:%s, Port:%d) IN.\n"), lpctszIP, wPort);
	_tprintf (_T ("UdpController::SetClient(IP:%s, Port:%d) OUT.\n"), lpctszIP, wPort);
	return true;
}

void UdpController::Stop ()
{
	_tprintf (_T ("UdpController::Stop() CALL.\n"));
	_stop = true;
	_connector->Close ();
}

DWORD UdpController::Invoke (LPVOID lpvParam)
{
	_tprintf (_T ("UdpController::Invoke(LPVOID) IN.\n"));

	BYTE byData[DATA_SIZE];

	int count = 0;
	while (_stop == false)
	{
		INT recvSize = _connector->Recv (byData, sizeof (byData));
		if (recvSize <= 0)
		{
			_tprintf (_T ("UdpController::Invoke(LPVOID)\n"));
			_tprintf (_T (" recvSize=%d Socket disconnected. reason:%d\n"),
					 recvSize, WSAGetLastError ());
			return -1;
		}
#ifdef _DUMP_DATA
		DumpData (_T ("UDP"), 0, byData, recvSize);
#else
		_tprintf (_T ("%d\r"), count++);
#endif
		int dataLen = MakeData (byData);

		INT sendSize = _connector->Send (byData, dataLen);
		if (sendSize != dataLen)
		{
			_tprintf (_T ("UdpController::Invoke(LPVOID)\n"));
			_tprintf (_T (" send=%d sended=%d Invalid send size. reason:%d\n"),
					 dataLen, sendSize, WSAGetLastError ());
			return -2;
		}
	}

	_tprintf (_T ("UdpController::Invoke(LPVOID) OUT.\n"));
	return 0;
}