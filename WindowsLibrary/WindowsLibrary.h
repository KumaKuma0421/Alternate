// ----------------------------------------------------------------------------
//! @file	WindowsLibrary.h
//! @brief	WindowsAPIを集約したプロジェクトファイル
//! @note	DllMain()のプロトタイプもここで宣言しています。
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include <Windows.h>
#include <tchar.h>

#ifdef WINDOWSLIBRARY_EXPORTS
#pragma message("DLL EXPORTING...")
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllexport)
#endif
#else
#ifndef DLL_DECLSPEC
#define DLL_DECLSPEC __declspec(dllimport)
#endif
#pragma comment(lib, "WindowsLibrary")
#pragma message("WindowsLibrary is now importing...")
#endif

BOOL APIENTRY DllMain (
	HMODULE hModule, DWORD dwReasonForCall, LPVOID lpvReserved);

namespace alt
{
	//! @class	WindowsLibrary
	//! @brief	プロジェクト共通基底クラス
	class DLL_DECLSPEC WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY WindowsLibrary () { _dwErrorNo = ERROR_SUCCESS; };

		//! @brief デストラクタ
		virtual APIENTRY ~WindowsLibrary () {};

		//! @brief WINAPIエラーの情報を設定します。
		virtual VOID APIENTRY SetErrNo ();

		//! @brief	WindowsAPIのエラー値を取得します。
		//! @return	WindowAPIのエラー時のエラー番号
		//! @retval	DWORD 詳細はネットで。
		//! @sa		http://ir9.jp/prog/ayu/win32err.htm
		//! @sa		https://gallery-code.blogspot.com/2010/05/getlasterror.html
		virtual DWORD APIENTRY GetErrNo () const { return _dwErrorNo; };

	protected:
		DWORD _dwErrorNo;
	};

	//! @class	HandleLibrary
	//! @brief	HANDLEを扱うWindowsAPIを集約した基底クラス
	class DLL_DECLSPEC HandleLibrary : public WindowsLibrary
	{
	public:
		//! @brief	コンストラクタ
		//! @note	ここで _hObject を INVALID_HANDLE_VALUE で初期化します。
		//! 		継承先はこのコンストラクタを指定して初期化します。
		APIENTRY HandleLibrary ();

		//! @brief	デストラクタ
		//! @note	_hObject をクローズしていない場合、ここで確実にクローズします。
		APIENTRY ~HandleLibrary ();

		//! @brief	継承先はこの関数でハンドルを取得します。
		//! @return	HANDLE値
		//! @retval	失敗した場合は INVALID_HANDLE_VALUE が返ります。
		HANDLE APIENTRY GetHandle () const;

		//! @brief		使用しなくなったハンドルはこれでクローズします。
		//! @return		クローズに成功したか否か。
		//! @retval		成功(TRUE)
		//! @retval		失敗(FALSE)
		//! @details	使用していない場合は、何もしません。
		//! 			使用していた場合は、ハンドルをクローズし、初期化します。
		BOOL APIENTRY Close ();

	protected:
		//! ハンドルを使用するWindowsAPIで保持するHANDLE値
		HANDLE _hObject;
	};

	//! @class	ReadWriteHandleLibrary
	//! @brief	HANDLEでReadやWriteを行うWindowsAPIを集約した基底クラス
	class DLL_DECLSPEC ReadWriteHandleLibrary : public HandleLibrary
	{
	public:
		//! @brief		HANDLEを使ってデータを読み込みます。
		//! @param[in]	lpvBuffer	読み込んだデータを保管するバッファ
		//! @param[in]	dwSize		読み込んだデータを保管するバッファのサイズ
		//! @return		実際に読み込んだデータのサイズ。
		//! 			失敗した場合は(DWORD)-1 が返ります。
		//! @retval		データサイズはバイトサイズです。
		DWORD APIENTRY Read (LPVOID lpvBuffer, DWORD dwSize) const;

		//! @brief		HANDLEを使ってデータを書き込みます。
		//! @param[in]	lpcvBuffer	書き込むデータを保管するバッファ
		//! @param[in]	dwSize		書き込むデータを保管するバッファのサイズ
		//! @return		実際に書き込んだデータのサイズ
		//! 			失敗した場合は(DWORD)-1 が返ります。
		//! @retval		データサイズはバイトサイズです。
		DWORD APIENTRY Write (LPCVOID lpcvBuffer, DWORD dwSize) const;

		//! @brief	まだデバイスに出力されていないデータを吐き出させます。
		//! @return	データのフラッシュに成功したか否か。
		//! @retval	成功(TRUE)
		//! @retval	失敗(FALSE)
		BOOL APIENTRY Flush () const;
	};

	//! @class	WaitableHandleLibrary
	//! @brief	HANDLEを使ってシグナル操作を行うWindowsAPIを集約した基底クラス
	class DLL_DECLSPEC WaitableHandleLibrary : public HandleLibrary
	{
	public:
		//! @brief	シグナル状態になるとブロックを解除します。
		//! @return	ブロック解除の理由
		//! @retval	ハンドルがシグナル状態(WAIT_OBJECT_O)
		//! 		ハンドルオブジェクトが終了(WAIT_ABANDONED)
		//! 		タイムアウト(WAIT_TIMEOUT)
		//! 		エラー発生(WAIT_FAILED)
		//! 		SetWaitableTimer()の関数呼び出し完了(WAIT_IO_COMPLETION)
		DWORD APIENTRY Wait (DWORD dwWaitTime = INFINITE) const;
	};
}