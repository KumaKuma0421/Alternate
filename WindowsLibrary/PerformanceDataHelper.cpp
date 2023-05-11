// ----------------------------------------------------------------------------
//! @file	PerformanceDataHelper.cpp
//! @brief	パフォーマンスモニターに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.15

#include "pch.h"
#include "PerformanceDataHelper.h"

using namespace alt;

// PerformanceDataItem ------------------------------------------------

PerformanceDataItem::PerformanceDataItem (HQUERY hQuery, TString key)
{
	_hQuery = hQuery;
	_counterPath = key;
	_hCounter = INVALID_HANDLE_VALUE;
}

PerformanceDataItem::~PerformanceDataItem ()
{

}

bool PerformanceDataItem::AddCounter ()
{
	PDH_STATUS stat = ::PdhAddCounter (
		_hQuery, _counterPath.Ctr (), 0, &_hCounter);
	return ERROR_SUCCESS == stat ? true : false;
}

bool PerformanceDataItem::GetValue (
	DWORD dwFormat, PDH_FMT_COUNTERVALUE& value)
{
	PDH_STATUS stat = ::PdhGetFormattedCounterValue (
		_hCounter, dwFormat, NULL, &value);
	return ERROR_SUCCESS == stat ? true : false;
}

TString& PerformanceDataItem::GetKey ()
{
	return _counterPath;
}

/* The query keys are...
	  Processor
	   _T("\\Processor(_Total)\\% Processor Time")          % Processor Time は、プロセッサがアイドル以外のスレッドを実行するために使用した経過時間の割合をパーセントで表示します。
	   _T("\\Processor(_Total)\\% User Time")               ユーザー モードで経過した非アイドルのプロセッサ時間の割合をパーセントで表示します。
	   _T("\\Processor(_Total)\\% Privileged Time")         プロセスのスレッドが特権モードでコード実行に費やした経過時間の割合をパーセントで表示します。
	   _T("\\Processor(_Total)\\Interrupts/sec")            ハードウェアの割り込みを、プロセッサが受信、処理した平均率を毎秒の回数で表したものです。
	  Memory
		_T("\\Memory\\Available MBytes")                    プロセスへの割り当てまたはシステムの使用にすぐに利用可能な物理メモリのサイズをメガバイト数で表示します。
		_T("\\Memory\\Pages/sec")                           ハード ページ フォールトを解決するためにディスクから読み取られた、またはディスクへ書き込まれたページの数です。
		_T("\\Memory\\Committed Bytes")                      コミットされた仮想メモリのサイズをバイト数で表示します。
		_T("\\Memory\\% Committed Bytes In Use")             Memory\\Committed Bytes の Memory\\Commit Limit に対する割合です。
	  Paging File
		_T("\\Paging File(_Total)\\% Usage")                使用中のページ ファイル インスタンスの合計をパーセントで表示します。
	  PhysicalDisk
		_T("\\PhysicalDisk(_Total)\\% Disk Time")           選択したディスク ドライブが読み取りまたは書き込み要求を処理していてビジー状態にあった経過時間の割合をパーセントで表示します。
		_T("\\PhysicalDisk(_Total)\\Current Disk Queue Length")  パフォーマンス データの収集時にディスクに残っている要求の数です。
		_T("\\PhysicalDisk(_Total)\\Disk Writes/sec")            ディスク上の書き込操作の速度です。
	  System
		_T("\\System\\Processor Queue Length")              プロセッサ キューにあるスレッドの数です。
		_T("\\System\\Context Switches/sec")                任意のスレッドからほかのスレッドに切り替わるすべてのプロセッサを含めた率です。
	  Network
		_T("\\Network Interface(*)\\Bytes Total/sec")       各ネットワーク アダプター上で送受信されるバイトの率です。

 */

// PerformanceDataHelper ----------------------------------------------

PerformanceDataHelper::PerformanceDataHelper ()
{
	_hQuery = INVALID_HANDLE_VALUE;
}

PerformanceDataHelper::~PerformanceDataHelper ()
{
	if (_hQuery != INVALID_HANDLE_VALUE) Close ();
	_hQuery = INVALID_HANDLE_VALUE;
}

bool PerformanceDataHelper::Open ()
{
	PDH_STATUS stat = ::PdhOpenQuery (NULL, 0, &_hQuery);
	return ERROR_SUCCESS == stat ? true : false;
}

bool PerformanceDataHelper::Close ()
{
	PDH_STATUS stat = ::PdhCloseQuery (_hQuery);
	return ERROR_SUCCESS == stat ? true : false;
}

PerformanceDataItem& PerformanceDataHelper::CreateItem (TString key)
{
	PerformanceDataItem* item = new PerformanceDataItem (_hQuery, key);
	item->AddCounter ();
	return *(item);
}

bool PerformanceDataHelper::Query ()
{
	PDH_STATUS stat = ::PdhCollectQueryData (_hQuery);
	return ERROR_SUCCESS == stat ? true : false;
}