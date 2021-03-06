//! @file	PerformanceCheck.h
//! @brief	性能測定用
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#pragma once

#include "TString.h"
#include "Console.h"

#define CONFIG_FILE _T(".\\PerformanceCheck.ini")

namespace PerfCheck
{
	//! @class	PerformanceCheck
	//! @brief	性能測定用
	class PerformanceCheck
	{
	public:
		//! @brief コンストラクタ
		PerformanceCheck();

		//! @brief デストラクタ
		virtual ~PerformanceCheck();

		//! @brief		初期化処理
		//! @param[in]	console Consoleオブジェクト
		//! @note		Consoleは初期化しておくこと。
		virtual VOID Init(alt::Console& console);

		//! @brief  処理開始
		//! @return 処理成否
		//! @retval TRUE:成功
		//! @retval FALSE:失敗
		//! @note   純粋仮想関数です。
		virtual BOOL DoAction() = 0;

		void Write(LPCTSTR message);
		void Pos(SHORT x, SHORT y);
		void Size(SHORT width, SHORT height);
		void Buffer(SHORT x, SHORT y);
		void Clear();

	private:
		alt::Console* _con;
	};
}