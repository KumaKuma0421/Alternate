//! @file	WindowsLibrary.h
//! @brief	WindowsAPIを集約したプロジェクトファイル
//! @note	DllMain()のプロトタイプもここで宣言しています。
//! @author	kumakuma0421@gmail.com
//! @date	2019.12.21

#pragma once

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpvReserved);

namespace alt
{
	//! @class	WindowsLibrary
	//! @brief	プロジェクト共通基底クラス
	class DLL_DECLSPEC WindowsLibrary
	{
	public:
		//! @brief		WindowsAPIのエラー値を返します。
		//! @return		WindowAPIのエラー時のエラー番号
		//! @retval		DWORD 詳細はネットで。
		//! @sa			http://ir9.jp/prog/ayu/win32err.htm
		//! @sa			https://gallery-code.blogspot.com/2010/05/getlasterror.html
		//! @details	GetLastError()を使用します。
		virtual DWORD GetErrNo();
	};

	//! @class	HandleLibrary
	//! @brief	HANDLEを扱うWindowsAPIを集約した基底クラス
	class DLL_DECLSPEC HandleLibrary : public WindowsLibrary
	{
	public:
		//! @brief	コンストラクタ
		//! @note	ここで _hObject を INVALID_HANDLE_VALUE で初期化します。
		//! 		継承先はこのコンストラクタを指定して初期化します。
		HandleLibrary();

		//! @brief	デストラクタ
		//! @note	_hObject をクローズしていない場合、ここで確実にクローズします。
		virtual ~HandleLibrary();

		//! @brief	継承先はこの関数でハンドルを取得します。
		//! @return	HANDLE値
		//! @retval	失敗した場合は INVALID_HANDLE_VALUE が返ります。
		virtual HANDLE GetHandle();

		//! @brief		使用しなくなったハンドルはこれでクローズします。
		//! @return		クローズに成功したか否か。
		//! @retval		成功(TRUE)
		//! @retval		失敗(FALSE)
		//! @details	使用していない場合は、何もしません。
		//! 			使用していた場合は、ハンドルをクローズし、初期化します。
		virtual BOOL Close();

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
		virtual DWORD Read(LPVOID lpvBuffer, DWORD dwSize);

		//! @brief		HANDLEを使ってデータを書き込みます。
		//! @param[in]	lpcvBuffer	書き込むデータを保管するバッファ
		//! @param[in]	dwSize		書き込むデータを保管するバッファのサイズ
		//! @return		実際に書き込んだデータのサイズ
		//! 			失敗した場合は(DWORD)-1 が返ります。
		//! @retval		データサイズはバイトサイズです。
		virtual DWORD Write(LPCVOID lpcvBuffer, DWORD dwSize);

		//! @brief	まだデバイスに出力されていないデータを吐き出させます。
		//! @return	データのフラッシュに成功したか否か。
		//! @retval	成功(TRUE)
		//! @retval	失敗(FALSE)
		virtual BOOL Flush();
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
		DWORD Wait(DWORD dwWaitTime = INFINITE);
	};

	//! @class	SocketLibrary
	//! @brief	WinSockを使用するAPIを集約したクラス
	class DLL_DECLSPEC SocketLibrary : public WindowsLibrary
	{
	public:
		//! @brief	コンストラクタ
		//! @note	_socketを INVALID_SOCKET で初期化します。
		SocketLibrary();

		//! @brief	デストラクタ
		//! @note	管理しているソケットをクローズします。
		virtual ~SocketLibrary();

		//! @brief		ソケットが使用されていた場合、クローズします。
		//! @return		ソケットクローズが成功したか否か。
		//! @retval		closesocket()の戻り値
		//! @retval		成功(0) ソケットではない(WSAENOTSOCK)
		//! @details	WinSock APIの戻り値はネットで確認
		virtual int Close();

		//! @brief		WinSock API呼び出し時にエラーとなった時、エラーの詳細を返します。
		//! @return		WinSock APIのエラー時のエラー番号
		//! @retval		DWORD 詳細はネットで。
		//! @sa			http://chokuto.ifdef.jp/advanced/prm/winsock_error_code.html
		//! @details	WSAGetLastError()を使用します。
		virtual DWORD GetErrNo();

		//! @brief		KeepAlive送出タイミング、間隔をセッション個別に設定します。
		//! @param[in]	onoff (0:OFF 1:ON)
		//! @param[in]	time 無通信となった時からKeepAlive送出開始までの時間(ミリ秒)
		//! @param[in]	interval 切断検知後にリトライ送出する時のインターバル
		//! @return		WinSock APIのエラー時のエラー番号
		//! @retval		DWORD 詳細はネットで。
		//! @details	SetSockOption()で設定されるKeepAlive送出設定はレジストリの
		//! 			設定値(既定値は２時間後)に従います。この関数により、KeepAlive
		//! 			の設定タイミングをセッション個別に設定することができます。
		virtual BOOL SetKeepAliveValue(ULONG onoff, ULONG time, ULONG interval);

	protected:
		int WSAIoctl(DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD dwInBuffer,
					 LPVOID lpvOutBuffer, DWORD dwOutBuffer, LPDWORD lpdwBytesReturned,
					 LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

				 //! このクラスで管理するソケットオブジェクト
		SOCKET _socket;
	};
}