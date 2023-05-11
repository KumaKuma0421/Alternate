//! @file	TStringMemoryLeakCheck.h
//! @brief	TStringのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#pragma once

#include "LeakCheck.h"
#include "TString.h"

namespace LeakCheck
{
	//! @class	TStringMemoryLeakCheck
	//! @brief	TStringのメモリーリークチェッククラス
	class TStringMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief コンストラクタ
		TStringMemoryLeakCheck ();

		//! @brief デストラクタ
		virtual ~TStringMemoryLeakCheck ();

		//! @brief 初期化処理
		virtual BOOL Init ();

		//! @brief メモリーリーク調査
		virtual BOOL DoAction ();

	private:
		BOOL DoAction1 ();
		BOOL DoAction2 ();
		BOOL DoAction3 ();
		BOOL DoAction4 ();
	};
}