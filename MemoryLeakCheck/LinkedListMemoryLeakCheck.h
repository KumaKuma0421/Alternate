//! @file	LinkedListMemoryLeakCheck.h
//! @brief	LinkedListのメモリーリークを確認します。
//! @author	kumakuma0421@gmail.com
//! @date	2022.06.28

#pragma once

#include "LeakCheck.h"
#include "LinkedList.hpp"

namespace LeakCheck
{
	//! @class	LinkedListMemoryLeakCheck
	//! @brief	LinkedListのメモリーリークチェッククラス
	class LinkedListMemoryLeakCheck : public LeakCheck
	{
	public:
		//! @brief コンストラクタ
		LinkedListMemoryLeakCheck ();

		//! @brief デストラクタ
		virtual ~LinkedListMemoryLeakCheck ();

		//! @brief 初期化処理
		virtual BOOL Init ();

		//! @brief メモリーリーク調査
		virtual BOOL DoAction ();
	};
}