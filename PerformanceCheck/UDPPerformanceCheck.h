//! @file	UDPPerformanceCheck.h
//! @brief	UDP性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.05.17

#pragma once

#include "PerformanceCheck.h"
#include "Console.h"
#include "QueryPerformance.h"
#include "Thread.h"
#include "SocketBuilder.h"
#include "UdpConnector.h"

#define _DUMP_DATA
#define DUMP_COUNT 24
#define DATA_SIZE 1460

#define SECTION_UDP_CONTROLLER_1    _T("UdpController1")
#define SECTION_UDP_CONTROLLER_2    _T("UdpController2")
#define KEYWORD_IP_ADDRESS          _T("IPAddress")
#define KEYWORD_PORT                _T("Port")
#define KEYWORD_KICK_OFF		    _T("KickOff")

#define SLEEP_TIME_30   30000
#define SLEEP_TIME_120 120000

int MakeData (BYTE byData[]);
void DumpData (LPCTSTR lpctszFrom, int direction, BYTE byData[], int iDataLen); // 0:Down 1:Up

BOOL DriveUdpController1ConsoleHandler (DWORD dwType);
BOOL DriveUdpController2ConsoleHandler (DWORD dwType);

void DriveUdpController1 ();
void DriveUdpController2 ();

namespace PerfCheck
{
	//! @class	UDPPerformanceCheck
	//! @brief	UDP性能測定用
	class UDPPerformanceCheck : public PerformanceCheck
	{
	public:
		UDPPerformanceCheck (alt::Console& console);
		virtual ~UDPPerformanceCheck ();

		virtual BOOL DoAction ();
	};

	//! @class	UdpController
	//! @brief	UDP性能測定用
	class UdpController : public alt::skeleton::ICallback
	{
	public:
		UdpController ();
		virtual ~UdpController ();

		bool Init ();
		bool Start (LPCTSTR lpctszIP, USHORT wPort, LPCTSTR lpctszTargetIP, USHORT wTargetPort, bool kickOff);
		bool SetClient (LPCTSTR lpctszIP, USHORT wPort);
		void Stop ();

	protected:
		virtual DWORD Invoke (LPVOID lpvParam);

	private:
		bool _stop;
		alt::SocketBuilder* _builder;
		alt::UdpConnector* _connector;
		alt::Thread* _thread;
	};
}