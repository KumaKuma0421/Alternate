// ----------------------------------------------------------------------------
//! @file	PerformanceDataHelper.cpp
//! @brief	パフォーマンスモニターに関するWindowsAPIを集約したクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.6.15

#pragma once

#include <Pdh.h>
#include "WindowsLibrary.h"
#include "TString.h"
#include "Array.hpp"

namespace alt
{
	class DLL_DECLSPEC PerformanceDataItem : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		//! @param hQuery PerformanceDataHelperで管理するHQUERY値
		//! @param key 監視カウンター名
		APIENTRY PerformanceDataItem (HQUERY hQuery, TString key);

		//! @brief デストラクタ
		APIENTRY ~PerformanceDataItem ();

		//! @brief カウンター値の取得設定
		//! @return bool
		//! @retval true(成功)
		//! @retval false(失敗)
		bool APIENTRY AddCounter ();

		//! @brief カウンター情報の取得
		//! @return TString&
		TString& APIENTRY GetKey ();

		//! @brief カウンター値の取得
		//! @param dwFormat 取得値の形式
		//!                 PDH_FMT_LONG
		//!                 PHD_FMT_DOUBLE
		//!                 PHD_FMT_LARGE
		//! @param value PDH_FMT_COUNTERVALUE
		//!              .longValue : LONG
		//!              .doubleValue : double
		//!              .largeValue : LONGLONG
		//! @return bool
		//! @retval true(成功)
		//! @retval false(失敗)
		bool APIENTRY GetValue (DWORD dwFormat, PDH_FMT_COUNTERVALUE& value);

	private:
		PerformanceDataItem () = delete;

		HQUERY _hQuery;
		HCOUNTER _hCounter;
		TString _counterPath;
	};

	class DLL_DECLSPEC PerformanceDataHelper : public WindowsLibrary
	{
	public:
		//! @brief コンストラクタ
		APIENTRY PerformanceDataHelper ();

		//! @brief デストラクタ
		APIENTRY ~PerformanceDataHelper ();

		//! @brief パフォーマンスモニター値の取得開始
		//! @return bool
		//! @retval true(成功)
		//! @retval false(失敗)
		bool APIENTRY Open ();

		//! @brief パフォーマンスモニター値の取得終了
		//! @return bool
		//! @retval true(成功)
		//! @retval false(失敗)
		bool APIENTRY Close ();

		//! @brief このクラスに属するカウンター設定クラスの生成
		//! @param key カウンター名
		//! @return PerformanceDataItem&
		PerformanceDataItem& APIENTRY CreateItem (TString key);

		//! @brief パフォーマンスカウンター値の問い合わせ
		//! @return bool
		//! @retval true(成功)
		//! @retval false(失敗)
		bool APIENTRY Query ();

	private:
		HQUERY _hQuery;
	};
}