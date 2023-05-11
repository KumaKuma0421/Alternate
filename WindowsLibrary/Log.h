// ----------------------------------------------------------------------------
//! @file	Log.h
//! @brief	Fileをベースとしたログに関する機能を集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.26

#pragma once

#include "File.h"
#include "FileUtility.h"
#include "TString.h"
#include "CriticalSection.h"

namespace alt
{
	//! @enum	LogLevel
	//! @brief	ログの出力レベル
	enum class LogLevel
	{
		//! (0)異常終了レベル
		Fatal,
		//! (1)エラー
		Error,
		//! (2)警告
		Warn,
		//! (3)情報
		Info,
		//! (4)デバッグ
		Debug,
		//! (5)トレース
		Trace
	};

	//! @class	Log
	//! @brief	Fileをベースとしたログに関する機能を集約したクラス
	class DLL_DECLSPEC Log : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY Log ();

		//! @brief コンストラクタ
		//! @param logLevel 出力対象レベル（これ以上を出力）
		//! @param fileName 出力ファイル名
		//! @param sRotation ローテーションファイル数
		//! @param dwFileSize 出力ファイルサイズ
		APIENTRY Log (
			LogLevel logLevel, LPCTSTR fileName, SHORT sRotation,
			DWORD dwFileSize);

		//! @brief デストラクタ
		APIENTRY ~Log ();

		//! @brief 初期化
		//! @param logLevel 出力対象レベル（これ以上を出力）
		//! @param fileName 出力ファイル名
		//! @param sRotation ローテーションファイル数
		//! @param dwFileSize 出力ファイルサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Init (
			LogLevel logLevel, LPCTSTR fileName, SHORT sRotation,
			DWORD dwFileSize);

		//! @brief 致命的エラーログの出力
		//! @param message 出力メッセージ
		//! @param dwThreadID 出力対象スレッドID
		VOID APIENTRY Fatal (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief エラーログの出力
		//! @param message 出力メッセージ
		//! @param dwThreadID 出力対象スレッドID
		VOID APIENTRY Error (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief 警告ログの出力
		//! @param message 出力メッセージ
		//! @param dwThreadID 出力対象スレッドID
		VOID APIENTRY Warn (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief 情報ログの出力
		//! @param message 出力メッセージ
		//! @param dwThreadID 出力対象スレッドID
		VOID APIENTRY Info (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief デバッグログの出力
		//! @param message 出力メッセージ
		//! @param dwThreadID 出力対象スレッドID
		VOID APIENTRY Debug (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

		//! @brief トレースログの出力
		//! @param message 出力メッセージ
		//! @param dwThreadID 出力対象スレッドID
		VOID APIENTRY Trace (LPCTSTR message, DWORD dwThreadID = GetCurrentThreadId ());

	protected:
		BOOL APIENTRY AppendLog ();
		BOOL APIENTRY CreateLog ();
		BOOL APIENTRY OpenLog ();
		VOID APIENTRY FindHistory ();
		VOID APIENTRY RotateLogFile ();
		LPCTSTR APIENTRY GetLogLevelString (LogLevel logLevel);

	private:
		VOID APIENTRY LogWrite (
			LogLevel logLevel, LPCTSTR pctszMessage, DWORD dwThreadID);

		File _file;					//! 現在出力中のログファイル
		LogLevel _level;			//! 出力ログレベル
		TString _baseFileName;		//! ログファイルユーザー設定名
		TString _logFileName;		//! ログファイル完全名
		SYSTEMTIME _localTime;		//! ログに出力するシステム時刻
		DWORD _dwCurrentWriteSize;	//! 現在までに書き込んでいるログファイルサイズ
		SHORT _sFileCount;			//! 現在までに確認できる履歴ファイル数
		SHORT _sRotation;			//! ログローテーション数
		DWORD _dwFileSize;			//! ログファイルサイズ
		CriticalSection _lock;		//! 書き込み時ロック
		LPCTSTR _format = _T ("%04d/%02d/%02d %02d:%02d:%02d.%03d %5d %5s %s\r\n");

		LPCTSTR _lpctszExtension = _T (".log");
		LPCTSTR _lpctszLogFileName = _T ("%s.log");
		LPCTSTR _lpctszSearchKeyword = _T ("??.log");
		LPCTSTR _lpctszRotationFileName = _T ("%s%02d.log");

		LPCTSTR _lpctszLogLevelFatal = _T ("Fatal");
		LPCTSTR _lpctszLogLevelError = _T ("Error");
		LPCTSTR _lpctszLogLevelWarn = _T ("Warn ");
		LPCTSTR _lpctszLogLevelInfo = _T ("Info ");
		LPCTSTR _lpctszLoglevelDebug = _T ("Debug");
		LPCTSTR _lpctszLogLevelTrace = _T ("Trace");
	};
}