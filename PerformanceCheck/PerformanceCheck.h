//! @file	PerformanceCheck.h
//! @brief	性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#pragma once

#define _WINSOCKAPI_ // Windows.h -> WinSock2.hの順にインクルードする際の呪文

#include "TString.h"
#include "Console.h"
#include "Utility.h"

#define CONFIG_FILE _T(".\\PerformanceCheck.ini")

namespace PerfCheck
{
	//! @class	PerformanceCheck
	//! @brief	性能測定用
	class PerformanceCheck
	{
	public:
		//! @brief コンストラクタ
		PerformanceCheck (alt::Console& console);

		//! @brief デストラクタ
		virtual ~PerformanceCheck ();

		//! @brief	初期化処理
		virtual VOID Init ();

		//! @brief  処理開始
		//! @return 処理成否
		//! @retval TRUE:成功
		//! @retval FALSE:失敗
		//! @note   純粋仮想関数です。
		virtual BOOL DoAction () = 0;

	protected:
		alt::Console& _console;

	private:
		PerformanceCheck () = delete;
	};
}