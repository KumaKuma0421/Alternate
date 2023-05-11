// ----------------------------------------------------------------------------
//! @file	CriticalSection.h
//! @brief	CriticalSectionに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	CriticalSection
	//! @brief	CriticalSectionに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC CriticalSection : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY CriticalSection ();

		//! @brief デストラクタ
		APIENTRY ~CriticalSection ();

		//! @brief 排他処理開始
		VOID APIENTRY Enter ();

		//! @brief 排他処理終了
		VOID APIENTRY Leave ();

	private:
		CRITICAL_SECTION _criticalSection;
	};
}