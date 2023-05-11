// ----------------------------------------------------------------------------
//! @file	Console.h
//! @brief	コンソールに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @enum	ForeColor
	//! @brief	コンソール出力時の文字色設定
	enum class ForeColor
	{
		//! 黒
		Black = 0,
		//! 青
		Blue = FOREGROUND_BLUE,
		//! 青(強調)
		BlueH = Blue | FOREGROUND_INTENSITY,
		//! 緑
		Green = FOREGROUND_GREEN,
		//! 緑(強調)
		GreenH = Green | FOREGROUND_INTENSITY,
		//! 赤
		Red = FOREGROUND_RED,
		//! 赤(強調)
		RedH = Red | FOREGROUND_INTENSITY,
		//! シアン
		Cyan = (Blue | Green),
		//! シアン(強調)
		CyanH = Cyan | FOREGROUND_INTENSITY,
		//! マゼンタ
		Magenta = (Blue | Red),
		//! マゼンタ(強調)
		MagentaH = Magenta | FOREGROUND_INTENSITY,
		//! 黄
		Yellow = (Green | Red),
		//! 黄(強調)
		YellowH = Yellow | FOREGROUND_INTENSITY,
		//! 白
		White = (Blue | Green | Red),
		//! 白(強調)
		WhiteH = White | FOREGROUND_INTENSITY
	};

	//! @enum	BackColor
	//! @brief	コンソール出力時の背景色設定
	enum class BackColor
	{
		//! 黒
		Black = 0,
		//! 青
		Blue = BACKGROUND_BLUE,
		//! 青(強調)
		BlueH = Blue | BACKGROUND_INTENSITY,
		//! 緑
		Green = BACKGROUND_GREEN,
		//! 緑(強調)
		GreenH = Green | BACKGROUND_INTENSITY,
		//! 赤
		Red = BACKGROUND_RED,
		//! 赤(強調)
		RedH = Red | BACKGROUND_INTENSITY,
		//! シアン
		Cyan = (Blue | Green),
		//! シアン(強調)
		CyanH = Cyan | BACKGROUND_INTENSITY,
		//! マゼンタ
		Magenta = (Blue | Red),
		//! マゼンタ(強調)
		MagentaH = Magenta | BACKGROUND_INTENSITY,
		//!	黄
		Yellow = (Green | Red),
		//! 黄(強調)
		YellowH = Yellow | BACKGROUND_INTENSITY,
		//! 白
		White = (Blue | Green | Red),
		//! 白(強調)
		WhiteH = White | BACKGROUND_INTENSITY
	};

	//! @class	Console
	//! @brief	コンソールに関するクラス
	class DLL_DECLSPEC Console : public HandleLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY Console ();

		//! @brief デストラクタ
		APIENTRY ~Console ();

		//! @brief	コンソール出力ウィンドウを作成します。
		//! @return BOOL	実行結果
		//! @retval TRUE	成功
		//! @retval FALSE	失敗
		BOOL APIENTRY Create () const;

		//! @brief Consoleクラスを初期化します。
		//! @return BOOL	実行結果
		//! @retval TRUE	成功
		//! @retval FALSE	失敗
		BOOL APIENTRY Init ();

		//! @brief 文字色、背景色を設定します。
		//! @param[in] foreColor 文字色
		//! @param[in] backColor 背景色
		//! @return bool
		//! @retval true 成功
		//! @retval false 失敗
		BOOL APIENTRY SetColor (ForeColor foreColor, BackColor backColor) const;

		//! @brief コンソールに文字を出力します。
		DWORD APIENTRY Write (LPCTSTR lpctszString) const;

		//! @brief コンソールに文字を出力します。
		VOID APIENTRY Format (LPCTSTR format, ...) const;

		//! @brief コンソールタイトルを設定します。
		BOOL APIENTRY SetTitle (LPCTSTR lpctszTitle) const;

		//! @brief 出力位置を設定します。
		BOOL APIENTRY SetCursorPosition (SHORT x, SHORT y) const;

		//! @brief バッファサイズを設定します。
		BOOL APIENTRY SetBufferSize (SHORT x, SHORT y) const;

		//! @brief ウィンドウサイズを設定します。
		BOOL APIENTRY SetWindowInfo (SHORT width, SHORT height) const;

		//! @brief コンソールをクリアします。
		BOOL APIENTRY Clear () const;

	private:
		BOOL APIENTRY GetScreenBufferInfo (CONSOLE_SCREEN_BUFFER_INFO& csbi) const;

		//! @brief コンソールウィンドウ出力情報
		CONSOLE_SCREEN_BUFFER_INFO _prevConsoleScreenBufferInfo;
	};
}