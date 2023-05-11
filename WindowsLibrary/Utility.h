// ----------------------------------------------------------------------------
//! @file	Utility.h
//! @brief	汎用的に使えるユーティリティクラス
//! @author	kumakuma0421@gmail.com
//! @date	2022.4.25

#pragma once

#include "TString.h"

namespace alt
{
	//! @class	Utility
	//! @brief	汎用的に使えるユーティリティクラス
	class DLL_DECLSPEC Utility
	{
	public:
		//! @brief GUIDの作成
		//! @return GUID(GUID構造体)
		static const GUID APIENTRY CreateGUID ();

		//! @brief GUIDの作成
		//! @return GUID（文字列）
		static const TString APIENTRY MakeGUID ();

		//! @brief システム時間の取得
		//! @return SYSTEMTIME
		//! @note wDayOfWeeは日曜日が０です。
		static SYSTEMTIME APIENTRY GetSystemTime ();

		//! @brief ローカル時間の取得
		//! @return SYSTEMTIME
		//! @note wDayOfWeeは日曜日が０です。
		static SYSTEMTIME APIENTRY GetLocalTime ();

		//! @brief SYSTEMTIME構造体の値を文字列化
		//! @param systemTime SYSTEMTIME構造体
		//! @return TString
		static TString APIENTRY GetFormatTime (const SYSTEMTIME& systemTime);

		//! @brief ローカル時間を６４ビット値で取得
		//! @return ULARGE_INTEGER
		static ULARGE_INTEGER APIENTRY GetLocalTimeQuad ();

		//! @brief FILETIME型からSYSTEMTIMEを取得
		//! @param fileTime FILETIME構造体
		//! @return SYSTEMTIME構造体
		//! @note 引数のfileTimeのタイムゾーンを変更しません。ファイルの
		//!       タイムスタンプは、システム時刻で格納されています。
		static SYSTEMTIME APIENTRY GetTimeByFileTime (const FILETIME& fileTime);

		//! @brief Windowsが起動してからの時間を返します。
		//! @note 49.7日でゼロに戻ります。
#pragma warning (push)
#pragma warning (disable:28159)
		static DWORD APIENTRY GetPastTick () { return ::GetTickCount (); };
#pragma warning (pop)

		//! @brief Windowsが起動してからの時間を返します。
		//! @return ULONGLONG
		static ULONGLONG APIENTRY GetPastTick64 () { return ::GetTickCount64 (); };

		//! @brief システム情報を取得します。
		//! @param systemInfo SYSTEM_INFO構造体
		//! DWORD     dwPageSize;
		//! LPVOID    lpMinimumApplicationAddress;
		//! LPVOID    lpMaximumApplicationAddress;
		//! DWORD_PTR dwActiveProcessorMask;
		//! DWORD     dwNumberOfProcessors;
		//! DWORD     dwProcessorType;
		//! DWORD     dwAllocationGranularity;
		//! WORD      wProcessorLevel;
		//! WORD      wProcessorRevision;
		static VOID APIENTRY GetSystemInfo (SYSTEM_INFO& systemInfo);

		//! @brief BASE64でデータをエンコード
		//! @param lpbyString 文字列
		//! @param dwLength 文字列長
		//! @param response 変換結果
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY Base64Encode (
			LPBYTE lpbyString, DWORD dwLength, TString& response);

		//! @brief BASE64データをデコード
		//! @param source BASE64文字列
		//! @param lpbyBuffer バッファ
		//! @param dwBuffer バッファサイズ
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY Base64Decode (
			TString& source, LPBYTE lpbyBuffer, DWORD& dwBuffer);

		//! @brief 環境変数の取得
		//! @return Array<TString>
		//! @note 配列形式ですべての環境変数を取得します。
		static skeleton::Array<TString> APIENTRY GetEnvironments ();

		//! @brief 環境変数の取得
		//! @param lpctszKeyword 取得するキーワード
		//! @return TString 対象の環境変数
		static TString APIENTRY GetEnv (LPCTSTR lpctszKeyword);

		//! @brief 設定フィルの読み込み
		//! @param lpctszIniFile INIファイル名
		//! @param lpctszSection セクション名
		//! @param lpctszKeyword キーワード名
		//! @param response 読み込み結果
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY ReadIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			TString& response);

		//! @brief 設定フィルの読み込み
		//! @param lpctszIniFile INIファイル名
		//! @param lpctszSection セクション名
		//! @param lpctszKeyword キーワード名
		//! @param dwDefault 既定値
		//! @return DWORD 取得価
		static DWORD APIENTRY ReadIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			DWORD dwDefault);

		//! @brief 設定フィルの書き込み
		//! @param lpctszIniFile INIファイル名
		//! @param lpctszSection セクション名
		//! @param lpctszKeyword キーワード名
		//! @param strValue 書き込み内容
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY WriteIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			TString& strValue);

		//! @brief 設定フィルの書き込み
		//! @param lpctszIniFile INIファイル名
		//! @param lpctszSection セクション名
		//! @param lpctszKeyword キーワード名
		//! @param iValue 書き込み内容
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		static BOOL APIENTRY WriteIniFile (
			LPCTSTR lpctszIniFile, LPCTSTR lpctszSection, LPCTSTR lpctszKeyword,
			INT iValue);

		//! @brief データからMD5を計算
		//! @param[in] lpbyData MD5を計算するデータ
		//! @param[in] dwSize MD5を計算するデータのサイズ
		//! @param[out lpbyMD5 MD5の計算結果
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		//! @note MD5は１２８ビットの値です。出力結果の格納に３２バイト必要です。
		static BOOL APIENTRY CreateMD5 (LPBYTE lpbyData, DWORD dwSize, LPBYTE lpbyMD5);

		//! @brief ファイルからMD5を計算
		//! @param[in] lpctszFileName ファイル名
		//! @param[out] lpbyMD5 MD5の計算結果
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		//! @note MD5は１２８ビットの値です。出力結果の格納に３２バイト必要です。
		static BOOL APIENTRY CreateMD5 (LPCTSTR lpctszFileName, LPBYTE lpbyMD5);

		//! @brief 配列の内容をクイックソート
		//! @tparam T 配列の型
		//! @param array 配列
		//! @param rangeLeft ソート範囲の最小添字
		//! @param rangeRight ソート範囲の最大添字
		//! @note テンプレートは宣言、本体をヘッダーに書かなければならない。
		//! @sa https://webpia.jp/quick_sort/
		template<typename T>
		static VOID APIENTRY QuickSort (T array[], int rangeLeft, int rangeRight)
		{
			// 変数定義
			int leftValue;
			int rightValue;
			T pivot;
			T temp;

			leftValue = rangeLeft; rightValue = rangeRight;

			// 基準は真ん中に設定（値の平均値（中央値）もある）
			pivot = array[(rangeLeft + rangeRight) / 2];

			while (1)
			{
				// 基準より小さい値を左から見つけていく
				// これをクラスで比較するために、operator < ()の実装が必要
				while (array[leftValue] < pivot) leftValue++;

				// 基準より大きい値を右から見つけていく
				// これをクラスで比較するために、operator < ()の実装が必要
				while (pivot < array[rightValue]) rightValue--;

				// 見つかった値の順序が逆になったら終了
				if (leftValue >= rightValue) break;

				// 安全にオブジェクトを作るために、operator = ()の実装が必要
				temp = array[leftValue];
				array[leftValue] = array[rightValue];
				array[rightValue] = temp;

				leftValue++;
				rightValue--;
			}

			//左のデータ群を対象としてクイックソートを再帰
			if (rangeLeft < leftValue - 1)
				QuickSort (array, rangeLeft, leftValue - 1);

			//右のデータ群を対象としてクイックソートを再帰
			if (rightValue + 1 < rangeRight)
				QuickSort (array, rightValue + 1, rangeRight);
		}

	private:
		Utility () = delete;
		~Utility () = delete;
	};
}