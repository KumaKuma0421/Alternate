// ----------------------------------------------------------------------------
//! @file	Process.h
//! @brief	プロセスに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"
#include "Pipe.h"

namespace alt
{
	//! @enum	Priority
	//! @brief	プロセスの優先度設定値
	enum class Priority
	{
		//! リアルタイム
		Realtime = REALTIME_PRIORITY_CLASS,

		//! 優先：タイムクリティカル
		High = HIGH_PRIORITY_CLASS,

		//! 通常より少し上
		AboveNormal = ABOVE_NORMAL_PRIORITY_CLASS,

		//! 通常：一般的なプロセス
		Normal = NORMAL_PRIORITY_CLASS,

		//! 通常より少し下
		BelowNormal = BELOW_NORMAL_PRIORITY_CLASS,

		//! アイドル：システムがアイドル状態の時に実行される。
		Idle = IDLE_PRIORITY_CLASS
	};

	//! @class	Process
	//! @brief	プロセスに関するWindowsAPIを集約したクラス
	//! @sa		https://docs.microsoft.com/en-us/windows/win32/ProcThread/creating-a-child-process-with-redirected-input-and-output
	class DLL_DECLSPEC Process : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY Process ();

		//! @brief デストラクタ
		APIENTRY ~Process ();

		//! @brief プロセスの作成
		//! @param lpctszApplicationName モジュール名
		//! @param lptszCommandLine コマンドライン
		//! @param lpctszCurrentDirectory 実行時カレントディレクトリ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Create (
			LPCTSTR lpctszApplicationName,
			LPTSTR lptszCommandLine,
			LPCTSTR lpctszCurrentDirectory = NULL);

		//! @brief プロセス終了の待機
		//! @param dwTime 終了タイムアウト時間(ms)
		//! @return DWORD
		//! @retval WAIT_OBJECT_0 正常終了(プロセス終了)
		//! @retval WAIT_
		DWORD APIENTRY Wait (DWORD dwTime = INFINITE) const;

		BOOL APIENTRY CreateAsUser ();
		BOOL APIENTRY CreateWithLogon ();

		//! @brief プロセス終了時の終了コード取得
		//! @return DWORD 終了コード
		DWORD APIENTRY GetExitCode () const;

		//! @brief プロセスの強制終了
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY Terminate () const;

		//! @brief 起動するプロセスのウィンドウ位置を設定
		//! @param dwX X座標
		//! @param dwY Y座標
		//! @param dwWidth ウィンドウの幅
		//! @param dwHeight ウィンドウの高さ
		//! @param wShowWindow ウィンドウ表示スタイル { SW_MAXIMIZE, SW_MINIMIZE, etc }
		VOID APIENTRY SetWindowInfo (
			DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight,
			WORD wShowWindow = SW_NORMAL);

		//! @brief 起動するプロセスのコンソール位置を設定
		//! @param dwX X座標
		//! @param dwY Y座標
		//! @param dwWidth ウィンドウの幅
		//! @param dwHeight ウィンドウの高さ
		//! @param dwXChars 横方向キャラクタ数
		//! @param dwYChars 縦方向キャラクタ数
		VOID APIENTRY SetConsoleInfo (
			DWORD dwX, DWORD dwY, DWORD dwWidth, DWORD dwHeight, DWORD dwXChars,
			DWORD dwYChars);

		//! @brief コンソールタイトルの設定
		//! @param lptszConsoleTitle コンソールタイトル
		VOID APIENTRY SetTitle (LPTSTR lptszConsoleTitle);

		//! @brief プロセスハンドルの取得
		//! @return HANDLE
		HANDLE APIENTRY GetProcessHandle () const { return _pi.hProcess; };

		//! @brief 標準入力ハンドルの取得
		//! @return HANDLE
		HANDLE APIENTRY GetStdIn () const { return _si.hStdInput; };

		//! @brief 標準出力ハンドルの取得
		//! @return HANDLE
		HANDLE APIENTRY GetStdOut () const { return _si.hStdOutput; };

		//! @brief 標準エラーハンドルの取得
		//! @return HANDLE
		HANDLE APIENTRY GetStdErr () const { return _si.hStdError; };

		//! @brief 優先度の設定
		//! @param priority 設定する優先度
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		BOOL APIENTRY SetPriority (Priority priority) const;

		//! @brief プロセス出力内容の読み込み
		//! @param lpvBuf 読み込みバッファ
		//! @param dwSize バッファサイズ
		//! @return DWORD 読み込みサイズ
		DWORD APIENTRY Peek (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief プロセス出力内容の読み込み
		//! @param lpvBuf 読み込みバッファ
		//! @param dwSize バッファサイズ
		//! @return DWORD 読み込みサイズ
		DWORD APIENTRY Read (LPVOID lpvBuf, DWORD dwSize) const;

		//! @brief プロセス入力への書き込み
		//! @param lpvBuf 書き込みバッファ
		//! @param dwSize バッファサイズ
		//! @return DWORD 書き込みサイズ
		DWORD APIENTRY Write (LPVOID lpvBuf, DWORD dwSize) const;

	private:
		BOOL ConnectHandles ();

		PROCESS_INFORMATION _pi;
		STARTUPINFO         _si;
		Pipe                _pipeRead;
		Pipe                _pipeWrite;
		Pipe                _pipeError;
		DWORD               _dwCreationFlags;
	};
}