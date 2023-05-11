//! @file	LeakCheck.h
//! @brief	TestProjectでは検出できない、メモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.5.23

#pragma once

#define CONFIG_FILE _T(".\\MemoryLeakCheck.ini")

namespace LeakCheck
{
	//! @class	Data
	//! @brief	データ格納クラス
	class Data
	{
	public:
		Data ()
		{
			number = -1;
		};

		Data (int i)
		{
			number = i;
		};

		virtual ~Data ()
		{

		};

		int Get ()
		{
			return number;
		};

		void Set (int i)
		{
			number = i;
		};

	private:
		int number;
	};

	//! @class	MemoryLeakCheck
	//! @brief	メモリーリークチェック用
	class LeakCheck
	{
	public:
		//! @brief 初期化処理
		//! @return BOOL
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		virtual BOOL Init () = 0;

		//! @brief 処理開始
		//! @return 処理成否
		//! @retval TRUE(成功)
		//! @retval FALSE(失敗)
		virtual BOOL DoAction () = 0;
	};

	//! @class LeakCheckTest
	//! @brief メモリーリーク結果の出力確認用
	class LeakCheckTest : public LeakCheck
	{
	public:
		BOOL Init ()
		{
			LPBYTE lpbyLeakCheck = new BYTE[16];
			ZeroMemory (lpbyLeakCheck, 16);
			return TRUE;
		};

		BOOL DoAction ()
		{
			LPBYTE lpbyLeakCheck = (LPBYTE)::malloc (16);
			ZeroMemory (lpbyLeakCheck, 16);
			return TRUE;
		};
	};
}