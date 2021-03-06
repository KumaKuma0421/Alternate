//! @file	Mutex.h
//! @brief	ミューテックスに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2020.3.29

#pragma once

#include "WindowsLibrary.h"

namespace alt
{
	//! @class	Mutex
	//! @brief	ミューテックスに関するWindowsAPIを集約したクラス
	class DLL_DECLSPEC Mutex : public WaitableHandleLibrary
	{
	public:
		//! @brief		ミューテックスを作成します。
		//! @param[in]	lpctszName ミューテックス名
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL Create(LPCTSTR lpctszName);

		//! @brief		ミューテックスをオープンします。
		//! @param[in]	lpctszName ミューテックス名
		//! @return		BOOL
		//! @retval		TRUE(成功)
		//! @retval		FALSE(失敗)
		BOOL Open(LPCTSTR lpctszName);

		//! @brief	ミューテックスを解放します。
		//! @return	BOOL
		//! @retval	TRUE(成功)
		//! @retval	FALSE(失敗)
		BOOL Release();
	};
}