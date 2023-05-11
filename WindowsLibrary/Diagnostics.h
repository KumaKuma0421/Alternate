// ----------------------------------------------------------------------------
//! @file	Diagnostics.h
//! @brief	システム性能に関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	Diagnostics
	//! @brief	システム性能に関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC Diagnostics : public HandleLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY Diagnostics ();

		//! @brief プロセスメモリ情報を取得します。
		//! @sa https://blog.goo.ne.jp/masaki_goo_2006/e/7a33fc63935919da2fd3f3bec37f8695
		//typedef struct _PROCESS_MEMORY_COUNTERS {
		//  DWORD  cb;                         //! 構造体のサイズ
		//  DWORD  PageFaultCount;             //! ページ・フォールト数
		//  SIZE_T PeakWorkingSetSize;         //! 最大ワーキングセット(メモリ使用量)
		//  SIZE_T WorkingSetSize;             //! 現在ワーキングセット(最大メモリ使用量)
		//  SIZE_T QuotaPeakPagedPoolUsage;    //! 最大ページプールの使用サイズ(ページプール)
		//  SIZE_T QuotaPagedPoolUsage;        //! 現在ページプールの使用サイズ
		//  SIZE_T QuotaPeakNonPagedPoolUsage; //! 最大非ページプールの使用サイズ(非ページプール)
		//  SIZE_T QuotaNonPagedPoolUsage;     //! 現在非ページプールの使用サイズ
		//  SIZE_T PagefileUsage;              //! 現在ページングファイルの使用サイズ(仮想メモリサイズ)
		//  SIZE_T PeakPagefileUsage;          //! 最大ページングファイルの使用サイズ
		//} PROCESS_MEMORY_COUNTERS, * PPROCESS_MEMORY_COUNTERS;
		//カッコ内はタスクマネージャでの表現です。
		BOOL APIENTRY GetProcessMemory (
			PROCESS_MEMORY_COUNTERS& processMemoryCounters) const;

		//! @brief プロセスI/O情報を取得します。
		//! @sa https://blog.goo.ne.jp/masaki_goo_2006/e/caa997ca0797fe3d240438f179bdab55
		//typedef struct _IO_COUNTERS {
		//  ULONGLONG ReadOperationCount;  // I/O読み取り
		//  ULONGLONG WriteOperationCount; // I/O書き込み
		//  ULONGLONG OtherOperationCount; // I/Oその他
		//  ULONGLONG ReadTransferCount;   // I/O読み取りバイト数
		//  ULONGLONG WriteTransferCount;  // I/O書き込みバイト数
		//  ULONGLONG OtherTransferCount;  // I/Oその他バイト数
		//} IO_COUNTERS, * PIO_COUNTERS;
		BOOL APIENTRY GetProcessIO (IO_COUNTERS& ioCounters) const;

		//! @brief ネットワークI/O情報を取得します。
		//! @sa http://www.geekpage.jp/programming/iphlpapi/getipstatistics.php
		//typedef struct _MIB_IPSTATS {
		//  DWORD dwForwarding;
		//  DWORD dwDefaultTTL;
		//  DWORD dwInReceives;
		//  DWORD dwInHdrErrors;
		//  DWORD dwInAddrErrors;
		//  DWORD dwForwDatagrams;
		//  DWORD dwInUnknownProtos;
		//  DWORD dwInDiscards;
		//  DWORD dwInDelivers;
		//  DWORD dwOutRequests;
		//  DWORD dwRoutingDiscards;
		//  DWORD dwOutDiscards;
		//  DWORD dwOutNoRoutes;
		//  DWORD dwReasmTimeout;
		//  DWORD dwReasmReqds;
		//  DWORD dwReasmOks;
		//  DWORD dwReasmFails;
		//  DWORD dwFragOks;
		//  DWORD dwFragFails;
		//  DWORD dwFragCreates;
		//  DWORD dwNumIf;
		//  DWORD dwNumAddr;
		//  DWORD dwNumRoutes;
		//} MIB_IPSTATS, *PMIB_IPSTATS;
		BOOL APIENTRY GetIpStatistics (MIB_IPSTATS& mibIPStats) const;
	};
}