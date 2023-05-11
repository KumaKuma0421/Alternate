//! @file	ArrayMemoryLeakCheck.h
//! @brief	Arrayのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2020.03.01

#pragma once

#include "LeakCheck.h"
#include "Array.hpp"

namespace LeakCheck
{
	//! @class	ArrayMemoryLeakCheck
	//! @brief	Arrayのメモリーリークチェッククラス
	class ArrayMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief コンストラクタ
		ArrayMemoryLeakCheck ();

		//! @brief デストラクタ
		virtual ~ArrayMemoryLeakCheck ();

		//! @brief 初期化処理
		virtual BOOL Init ();

		//! @brief メモリーリーク調査
		virtual BOOL DoAction ();
	};
}